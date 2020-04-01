/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

/* compiled thru command.cpp */

#include <typeinfo>

#include "hiteffect.h"
#include "traps.h"
#include "dbgmsgproj.h"

#include "cmdcraftfilters.cpp"

std::vector<recipedata> vSuspended; //TODO suspendable action should be more global to be reused by other actions than crafting!

//recipedata* craftcore::prpdSuspended=NULL;

void craftcore::AddSuspended(const recipedata& rpd)
{
  rpd.rc.integrityCheck();

  if(!rpd.rc.IsCanBeSuspended())
    ABORT("action can't be suspended %s",rpd.dbgInfo().CStr());

  for(int i=0;i<vSuspended.size();i++)
    if(vSuspended[i].id()==rpd.id())
      ABORT("this crafting was already suspended '%s'",vSuspended[i].id().CStr());

//  rpd.rc.ClearRefs();

  vSuspended.push_back(rpd);
}

void craftcore::RemoveIfSuspended(const recipedata&rpd)
{
  for(int i=0;i<vSuspended.size();i++)
    if(vSuspended[i].id()==rpd.id()){
      vSuspended.erase(vSuspended.begin()+i);
      break;
    }
}

recipedata craftcore::FindRecipedata(festring fsRpdId)
{
  for(int i=0;i<vSuspended.size();i++)
    if(vSuspended[i].id()==fsRpdId)
      return vSuspended[i];

  ABORT("unable to find recipedata id='%s'",fsRpdId.CStr());
  return recipedata(); //dummy... just to let it be compiled...
}

void craftcore::ClearSuspendedList()
{
  vSuspended.clear();
}

bool craftcore::EmptyContentsIfPossible(recipedata& rpd,item* itContainer, bool bMoveToInventory)
{
  materialcontainer* mc = dynamic_cast<materialcontainer*>(itContainer); //potions, mines... also bananas xD
  itemcontainer* ic = dynamic_cast<itemcontainer*>(itContainer); //chests

  bool bEmptied = false;

  if(mc!=NULL){
    if(mc->GetSecondaryMaterial()!=NULL){
      if(bMoveToInventory)
        PrepareRemains(rpd,mc->GetSecondaryMaterial());
      else
        delete mc->RemoveSecondaryMaterial(); //prevents keeping: ex. random liquids like antidote
    }
    bEmptied = true;
  }

  if(ic!=NULL){ //empty chests etc
    stack* stkC = ic->GetContained();
    itemvector ivC;
    stkC->FillItemVector(ivC);
    for(int i=0;i<ivC.size();i++){
      if(bMoveToInventory)
        ivC[i]->MoveTo(rpd.rc.H()->GetStack());
      else
        SendToHellSafely(ivC[i]);
    }

    bEmptied = true;
  }

  return bEmptied;
}

void craftcore::SendToHellSafely(item* it)
{
  it->RemoveFromSlot(); //just in case to prevent problems later... like crashing elsewhere!!!
  it->SendToHell(); DBG3("SentToHell",it,it->GetID());//,lumpAtInv,lumpAtInv->GetID());
//  **rit=NULL;
}

float craftcore::CraftSkill(character* Char){ //is the current capability of successfully crafting
  float fBonus = 0; // influence/weights of each stat will be the FINAL divider!
  float fWeight = 0;
  float fDivFinal = 0;

  #define CALCSK(weig,attr) fWeight = weig; fBonus += Char->GetAttribute(attr)*weig; fDivFinal+=weig;
  CALCSK(15.0,DEXTERITY); //by importance order
  CALCSK(7.5,WISDOM);
  CALCSK(3.0,PERCEPTION); //TODO could counter fumbles directly
  CALCSK(3.0,ARM_STRENGTH); //TODO half if only one arm?
  CALCSK(2.5,AGILITY);
  CALCSK(2.5,INTELLIGENCE);
  CALCSK(2.0,ENDURANCE); //TODO could lower 0.1 per turn crafting, til 0.0
  CALCSK(1.5,WILL_POWER); //TODO could lower 0.1 per turn crafting, til 0.0
  CALCSK(0.25,LEG_STRENGTH);
  //TODO CHARISMA //if one day there is item quality, well finished, attribute that could increase sell price
  //TODO MANA //if one day anything magical is allowed to be crafted

  float fSkill = 0;
  fSkill += Char->GetCWeaponSkill(CRAFTING)->GetLevel(); // base/learned
  fSkill += fBonus/fDivFinal; // in short, if all stats are 10, craft skill would be 10
  fSkill -= 10.0; // to make advancing important
  if(fSkill<1.0)fSkill=1.0; //safety
  return fSkill;
}

bool craftcore::canBeCrafted(item* it){
  if(dynamic_cast<lump*>(it)!=NULL)
    return true;
  if(dynamic_cast<stick*>(it)!=NULL) //checked when splitting
    return true;
  if(dynamic_cast<itemcontainer*>(it)!=NULL) //to allow crafting chests again
    return true;

  const itemdatabase* itdb = it->GetDataBase();

  if(it->GetCategory()==POTION)
  {
    item* Bottle = dynamic_cast<potion*>(it);
    if(Bottle && (Bottle->GetNameSingular() == "bottle" || Bottle->GetNameSingular() == "vial")) //TODO really necessary the singular name check? excessive?
    {
      if(!Bottle->GetSecondaryMaterial())
        return true;
      if( // extracting from corpses
          // TODO may be the "kind of action" (as a parameter for this func) could allow some specific materials only,  so a new action EXTRACT_FROM_CORPSE would allow only the ones below
         Bottle->GetSecondaryMaterial()->GetConfig()==SULPHURIC_ACID ||
         Bottle->GetSecondaryMaterial()->GetConfig()==POISON_LIQUID ||
         Bottle->GetSecondaryMaterial()->GetConfig()==MAGIC_LIQUID
        )
        return true;
    }

    DBG3(Bottle->GetSecondaryMaterial()->GetConfig(),SULPHURIC_ACID,POISON_LIQUID);
    return false;
  }

  // TODO allow near oven, but the problem is the ingredients... also may be blocked below too
  if(it->GetCategory()==FOOD)
  {
    item* Can = dynamic_cast<can*>(it);
    if(Can && !Can->GetSecondaryMaterial())
      return true;

    DBGLN;
    return false;
  }

  //TODO all these things should have some easier kind of property to be checked
  if(!craftcore::MoreCraftDeniedFilters(it)){
    DBGLN;
    return false;
  }

  if( // more complex/generic filter, better keep as last check?
    game::IsQuestItem(it) ||
    it->GetEnchantment()!=0 ||
    it->GetCategory()==BOOK ||
    it->GetCategory()==MISC ||
    it->GetCategory()==SCROLL ||
    !itdb->CanBeWished ||
    itdb->Possibility <= 0 ||
    !itdb->PostFix.IsEmpty() ||
    false // just to make it easier to re-organize and add checks above
  ){
    DBGLN;
    return false;
  }

  return true;
}

bool craftcore::HasSuspended() {
  return vSuspended.size()>0;
}
bool craftcore::ResumeSuspendedTo(character* Char,recipedata& rpd)
{
  if(!HasSuspended())
    ABORT("no suspended craft action to set to %s!",Char->GetName(DEFINITE).CStr());

  if(Char->GetAction()!=NULL){DBG1("AlreadySomethingElse,How?");DBGSTK; //may never happen tho... TODO ABORT() ?
    ADD_MESSAGE("You are already doing something else.");
    return false;
  }

  rpd.rc.ClearRefs(); //good to cleanup, will be set again also

  rpd.rc.integrityCheck();

  bool bReqSamePos = false;
//  if(rpd.bMeltable)bReqSamePos=true;
  if(!rpd.v2AnvilLocation.Is0())bReqSamePos=true; DBGSV2(rpd.v2AnvilLocation);
  if(!rpd.v2ForgeLocation.Is0())bReqSamePos=true; DBGSV2(rpd.v2ForgeLocation);
  if(!rpd.v2WorkbenchLocation.Is0())bReqSamePos=true; DBGSV2(rpd.v2WorkbenchLocation);
  if(rpd.otSpawnType!=CTT_NONE && !rpd.v2PlaceAt.Is0())bReqSamePos=true; DBG1(rpd.otSpawnType);
  if(bReqSamePos){
    if(rpd.rc.GetDungeonLevelID() != craftcore::CurrentDungeonLevelID()){
      //TODO better message: place? location? dungeon level sounds a bit non-immersive, or not?
      ADD_MESSAGE("You need to be in the same dungeon as before to continue crafting.");
      return false;
    }

    if(rpd.v2PlayerCraftingAt != Char->GetPos()){
      festring fsDist;
      int iDist = (rpd.v2PlayerCraftingAt - Char->GetPos()).GetLengthSquare();
      if(iDist<=2)fsDist<<"and you are almost there!";
      else
      if(iDist<=10)fsDist<<"and you are nearby.";
      else
      if(iDist<=20)fsDist<<"but you are still a bit far away from it...";
      else
        fsDist<<"but it seems to be quite far from here.";
      ADD_MESSAGE("You need to be where you were crafting before, %s",fsDist.CStr());
      game::SetDrawMapOverlay(true); //TODO make this optional?
      game::PositionQuestion(CONST_S("It was here!"), rpd.v2PlayerCraftingAt, 0, 0, false);
      game::SetDrawMapOverlay(false);
      return false;
    }
  }else{
    rpd.v2PlayerCraftingAt = Char->GetPos();
  }

  Char->SwitchToCraft(rpd);

  return true;
}

void recipecore::Save(outputfile& SaveFile) const
{
  integrityCheck();

  SaveFile //commented ones are just to keep the clarity/organization
    << bCanBeSuspended
    << iDungeonLevelID
    ;
}

void recipedata::Save(outputfile& SaveFile) const
{
  rc.Save(SaveFile);

  SaveFile //commented ones are just to keep the clarity/organization
    << ingredientsIDs
    << iAddDexterity
    << iBaseTurnsToFinish
    << itSpawnTot
    << v2ForgeLocation

    << v2PlaceAt
    << bSuccesfullyCompleted
    << v2AnvilLocation
    << v2PlayerCraftingAt
    << fsCraftInfo

    << itToolID
    << itTool2ID
    << itSpawnType
    << fsItemSpawnSearchPrototype

    << itSpawnCfg
    << itSpawnMatMainCfg
    << itSpawnMatMainVol
    << itSpawnMatSecCfg
    << itSpawnMatSecVol

    << otSpawnCfg
    << otSpawnMatMainCfg
    << otSpawnMatMainVol
    << otSpawnMatSecCfg
    << otSpawnMatSecVol

    << otSpawnType
    << bSpawnBroken
    << fDifficulty
    << bCanBeBroken
    << bMeltable

    << v2WorkbenchLocation
    << iRemainingTurnsToFinish
    << bGradativeCraftOverride
    << bTailoringMode

    ;
}

void recipecore::Load(inputfile& SaveFile)
{
  SaveFile //commented ones are just to keep the clarity/organization
    >> bCanBeSuspended
    >> iDungeonLevelID
    ;
}

void recipedata::Load(inputfile& SaveFile)
{
  rc.Load(SaveFile);

  SaveFile //commented ones are just to keep the clarity/organization
    >> ingredientsIDs
    >> iAddDexterity
    >> iBaseTurnsToFinish
    >> itSpawnTot
    >> v2ForgeLocation

    >> v2PlaceAt
    >> bSuccesfullyCompleted
    >> v2AnvilLocation
    >> v2PlayerCraftingAt
    >> fsCraftInfo

    >> itToolID
    >> itTool2ID
    >> itSpawnType
    >> fsItemSpawnSearchPrototype

    >> itSpawnCfg
    >> itSpawnMatMainCfg
    >> itSpawnMatMainVol
    >> itSpawnMatSecCfg
    >> itSpawnMatSecVol

    >> otSpawnCfg
    >> otSpawnMatMainCfg
    >> otSpawnMatMainVol
    >> otSpawnMatSecCfg
    >> otSpawnMatSecVol

    >> otSpawnType
    >> bSpawnBroken
    >> fDifficulty
    >> bCanBeBroken
    >> bMeltable

    >> v2WorkbenchLocation
    >> iRemainingTurnsToFinish
    >> bGradativeCraftOverride

    ;
  
  if(game::GetCurrentSavefileVersion() >= 135)
    SaveFile >> bTailoringMode;

//  if(otSpawnType!=CTT_NONE)
//    SaveFile >> otSpawn;
  rc.integrityCheck();
}
cfestring recipedata::id() const
{
  /**
   * this is a simple way to detect if the recipedata is the same,
   * here shall only contain things that would not change thru the
   * whole crafting process/time
   * TODO just use an ulong ID like chars and items do? but keep this as debug info at least!!!
   */

  festring fs;

  #define RPDINFO(o) fs<<(#o)<<"="<<(o)<<"; ";
  RPDINFO(rc.IsCanBeSuspended());

  RPDINFO(itToolID);
  RPDINFO(itTool2ID);

  RPDINFO(itSpawnCfg);
  RPDINFO(itSpawnMatMainCfg);
  RPDINFO(itSpawnMatMainVol);
  RPDINFO(itSpawnMatSecCfg);
  RPDINFO(itSpawnMatSecVol);

  RPDINFO(otSpawnCfg);
  RPDINFO(otSpawnMatMainCfg);
  RPDINFO(otSpawnMatMainVol);
  RPDINFO(otSpawnMatSecCfg);
  RPDINFO(otSpawnMatSecVol);

  RPDINFO(fsItemSpawnSearchPrototype);
  RPDINFO(fsCraftInfo);

  #define RPDINFOV2(o) fs<<(#o)<<"="<<(o.X)<<","<<(o.Y)<<"; ";
  RPDINFOV2(v2AnvilLocation);
  RPDINFOV2(v2ForgeLocation);
  RPDINFOV2(v2WorkbenchLocation);
  RPDINFOV2(v2PlaceAt);
  RPDINFOV2(v2PlayerCraftingAt);

  return fs;
}

cfestring recipedata::dbgInfo() const
{
  festring fs;
  fs << id();
  return fs;
}

clock_t RPDInitKey = clock();

void recipecore::SetHumanoid(character* C){
  integrityCheck(C);

  if(C==NULL)
    ABORT("actor can't be null, btw h='%s'",h==NULL?"NULL":h->GetName(DEFINITE).CStr());

  if(C==h)
    return;

  humanoid* hNew = dynamic_cast<humanoid*>(C);
  if(hNew==NULL)
    ABORT("Only humanoids can craft C='%s' h='%s'",C->GetName(DEFINITE).CStr(),h==NULL?"NULL":h->GetName(DEFINITE).CStr());

  h = hNew;
}

void recipecore::integrityCheck(character* Actor) const
{
  if(initKey!=RPDInitKey){ //FIRST TEST!!!!!!!!!!!!!!!!!!!!!
    //TODO bools get crazy values too if not initialized
    ABORT("recipedata corrupted, not initialized or invalid");// it will not be possible to show info, would crash on it... , dbgInfo().CStr());
  }
}

recipecore::recipecore(humanoid* H,uint sel){
  initKey = RPDInitKey;

  h=H;

  bCanBeSuspended=true;
  iDungeonLevelID=craftcore::CurrentDungeonLevelID();
}

recipedata::recipedata(humanoid* H,uint sel) : rc(H,sel)
{
  itTool=NULL;
  itTool2=NULL;
  lsqrPlaceAt = NULL;
  lsqrCharPos = rc.H()==NULL ? NULL : game::GetCurrentLevel()->GetLSquare(rc.H()->GetPos());
  itWeakestIngredient = NULL;
  lsqrActor=NULL;

  // no need to save
  SelectedRecipe=sel;
  bSpendCurrentTurn=false; //TODO review everywhere to let it work as expected instead of always spending a turn what ignores this
  bAlreadyExplained=false;
  bHasAllIngredients=false;
  bCanStart=false;

  bCanBePlaced=false;
  xplodStr=0;
  iStrongerXplod=0;
  v2XplodAt=v2(0,0);
  bOnlyXplodIfCriticalFumble=false;

  bSpecialExtendedTurns=false;
  iMinTurns=0;
  bFailedTerminateCancel=false;
  bFailedSuspend=false;

  ////////////////////////////////////////////////////////////////////////////////////
  /// saveables
  //////////////////////////////////

  ingredientsIDs.clear(); //just to init
  iAddDexterity=0;
  iBaseTurnsToFinish=1; //TODO should be based on attributes
  itSpawnTot=1;
  v2ForgeLocation=v2(0,0);

  v2PlaceAt=v2(0,0);
  bSuccesfullyCompleted=false;
  v2AnvilLocation=v2(0,0);
  v2PlayerCraftingAt=v2(0,0);
  fsCraftInfo="";

  itToolID=0;
  itTool2ID=0;
  itSpawnType=CIT_NONE;
  fsItemSpawnSearchPrototype="";

  itSpawnCfg=0;
  itSpawnMatMainCfg=0;
  itSpawnMatMainVol=0;
  itSpawnMatSecCfg=0;
  itSpawnMatSecVol=0;

  otSpawnCfg=0;
  otSpawnMatMainCfg=0;
  otSpawnMatMainVol=0;
  otSpawnMatSecCfg=0;
  otSpawnMatSecVol=0;

  otSpawnType=CTT_NONE;
  bSpawnBroken=false;
  fDifficulty=1.0;
  bCanBeBroken=true; //most can, anyway b4 breaking should be revalidated
  bMeltable=false;

  v2WorkbenchLocation=v2(0,0);
  iRemainingTurnsToFinish=iBaseTurnsToFinish;
  bGradativeCraftOverride=false;
  bTailoringMode=false;
}

int craftcore::CurrentDungeonLevelID(){
  return game::GetCurrentDungeonIndex()*100+game::GetCurrentLevelIndex();
}

void recipedata::CopySpawnItemCfgFrom(item* itCfg)
{
  rc.integrityCheck();
  if(itCfg==NULL)
    ABORT("NULL itCfg");

  itSpawnCfg = itCfg->GetConfig();
  material* matM = itCfg->GetMainMaterial();
  itSpawnMatMainCfg = matM->GetConfig();
  itSpawnMatMainVol = matM->GetVolume();
//  itSpawnMatMainSpoilLevel = matM->GetSpoilLevel();
  if(itCfg->GetSecondaryMaterial()!=NULL){
    material* matS = itCfg->GetSecondaryMaterial();
    itSpawnMatSecCfg = matS->GetConfig();
    itSpawnMatSecVol = matS->GetVolume();
//    itSpawnMatSecSpoilLevel = matS->GetSpoilLevel();
  }
}

void recipedata::CopySpawnTerrainCfgFrom(olterrain* otCfg){
  rc.integrityCheck();
  if(otCfg==NULL)
    ABORT("NULL otCfg");

  otSpawnCfg = otCfg->GetConfig();
  otSpawnMatMainCfg = otCfg->GetMainMaterial()->GetConfig();
  otSpawnMatMainVol = otCfg->GetMainMaterial()->GetVolume();
  if(otCfg->GetSecondaryMaterial()!=NULL){
    otSpawnMatSecCfg = otCfg->GetSecondaryMaterial()->GetConfig();
    otSpawnMatSecVol = otCfg->GetSecondaryMaterial()->GetVolume();
  }
}

olterrain* crafthandle::SpawnTerrain(recipedata& rpd, festring& fsCreated){
  rpd.rc.integrityCheck();

  olterrain* otSpawn = NULL;

  switch(rpd.otSpawnType){
    case CTT_FURNITURE:
      otSpawn=decoration::Spawn(rpd.otSpawnCfg);
      break;
    case CTT_DOOR:
      otSpawn=door::Spawn(rpd.otSpawnCfg);
      ((door*)otSpawn)->SetIsLocked(false);
      ((door*)otSpawn)->SetIsOpened(true);
      //TODO configure lock type based randomly in one of the keys available on player's inventory
      break;
    case CTT_WALL:
      otSpawn=wall::Spawn(rpd.otSpawnCfg); //earth::Spawn();
      break;
  }

  if(otSpawn==NULL)
    ABORT("craft spawned no terrain.");

  delete otSpawn->SetMainMaterial(material::MakeMaterial(rpd.otSpawnMatMainCfg,rpd.otSpawnMatMainVol));
  if(rpd.otSpawnMatSecCfg>0)
    delete otSpawn->SetSecondaryMaterial(material::MakeMaterial(rpd.otSpawnMatSecCfg,rpd.otSpawnMatSecVol));

  fsCreated << "You built ";
  rpd.lsqrPlaceAt->ChangeOLTerrainAndUpdateLights(otSpawn); //TODO a forge seems to not be emitting light...

  fsCreated << otSpawn->GetName(INDEFINITE);

  return otSpawn;
}

void recipecore::ClearRefs()
{
  h = NULL;
}

void recipedata::ClearRefs(){
  /**
   * This is to help on granting consistency.
   * As in case of save/load all pointers will change.
   * This is important to revalidate all pointers from IDs.
   */

  itTool = NULL;
  itTool2 = NULL;
  lsqrPlaceAt = NULL;
  lsqrCharPos = NULL;
  itWeakestIngredient = NULL;
  lsqrActor = NULL;

  rc.ClearRefs();
}

struct ci{ //create item info/helper/data/config/param
  bool bMultSelect = true;

  int iReqCfg=0;
  bool bMainMaterRemainsBecomeLump=false;
  bool bOverridesQuestion=false;
  bool bMsgInsuficientMat=true;
  bool bInstaAddIngredients=false;

  bool bFirstMainMaterIsFilter=true;
  int iReqMatCfgMain=0; //setting this overrides bFirstMainMaterIsFilter
  int iReqMatCfgSec=0;
  bool bJustAcceptFirstChosenAndReturn=false;
  bool bAllowDegradation = false;

  bool bFirstItemMustHaveFullVolumeRequired=false;
  bool bAllowMeltables=true;
  bool bAllowWood=true;
  bool bAllowBones=true;
  int iMinMainMaterStr=0;

  float fUsablePercVol=1.0;
  bool bMustBeTailorable=false;
  bool bMixRemainingLump=true;
};
struct recipe{
  festring action;
  festring name;
  int iListIndex;

  festring desc;

  void init(cchar* act,cchar* nm){
    action=(act);
    name=(nm);
    iListIndex=(-1);//,desc(""){};
  }

  virtual void fillInfo(){ ABORT("missing recipe info implementation"); }

  void failPlacementMsg(recipedata& rpd){
    ADD_MESSAGE("%s can't be placed here.",name.CStr());
    rpd.bAlreadyExplained=true;
  }
  void failIngredientsMsg(recipedata& rpd){
    festring fsMsg;
    fsMsg<<"Required ingredients to "<<action<<" "<<name<<" are not met.";
    ADD_MESSAGE(fsMsg.CStr());
    rpd.bAlreadyExplained=true;
  }
  void failToolMsg(recipedata& rpd,festring tool){
    ADD_MESSAGE("You don't have a strong enough %s to work on the requested materials.",tool.CStr());
    // "strong enough" means that a too weak material wont be able to work on a much stronger material
    rpd.bAlreadyExplained=true;
  }

  bool IsTheSelectedOne(recipedata& rpd){
    if(desc.IsEmpty())
      fillInfo();
    if(desc.IsEmpty())
      ABORT("recipe info is still empty");

    return rpd.SelectedRecipe == iListIndex;
  }

  virtual bool work(recipedata& rpd){ return false; }

  virtual ~recipe(){}

  bool whereRaw(recipedata& rpd,festring fsMsg,bool acceptSelfLocation=false){
    int Dir = game::DirectionQuestion(fsMsg, false, acceptSelfLocation);
    if(Dir != DIR_ERROR) {
      v2 v2W = rpd.rc.H()->GetPos() + game::GetMoveVector(Dir);
      if(rpd.rc.H()->GetArea()->IsValidPos(v2W)){
        rpd.lsqrPlaceAt = rpd.rc.H()->GetNearLSquare(v2W);
        return true;
      }
    }
    return false;
  }

  bool where(recipedata& rpd,bool acceptSelfLocation=false)
  {
    if(whereRaw(rpd,"Build it where?",acceptSelfLocation))
      if(rpd.lsqrPlaceAt != NULL && rpd.lsqrPlaceAt->GetOLTerrain() == NULL &&
         rpd.lsqrPlaceAt->GetCharacter() == NULL &&
         !(rpd.lsqrPlaceAt->GetRoom() && rpd.lsqrPlaceAt->GetRoom()->GetMaster() &&
         rpd.lsqrPlaceAt->GetRoom()->GetMaster() != PLAYER)) // Prevents building in owned rooms.
      {
        rpd.v2PlaceAt = rpd.lsqrPlaceAt->GetPos();
        rpd.bCanBePlaced=true;
        return true;
      }

    failPlacementMsg(rpd);
    return false;
  }

  static int calcMeltableTurns(material* mat,float fMult=1.0){
    /**
     * min is gold: str 55 and spends 5 turns each 1000cm3.
     * TODO quite arbritrary but gameplay wise enough?
     * TODO should use density? or something else than str? fireresistance is not defined for most and is not melting point either...
     */
    DBG2(mat->GetStrengthValue(),mat->GetVolume());
    static const int iBaseTurns = 5;
    static const float fMinStr = 55.0; //float for precision
    static const float fBaseVol = 1000.0; //float for precision
    float f = iBaseTurns * fMult * (mat->GetStrengthValue()/fMinStr) * (mat->GetVolume()/fBaseVol); //vol is in cm3, so per 1L or 1Kg(water)
    if(f>0 && f<1)f=1;
    return f;
  }

  static bool chkWeapon(item* it, int iCfg, int iWeaponCategory, int iMainMatterMinStrengh){
    return
      dynamic_cast<meleeweapon*>(it)!=NULL
      &&
      it->GetMainMaterial()->GetStrengthValue()>=iMainMatterMinStrengh
      &&
      (
        (iCfg!=0 && it->GetConfig()==iCfg) ||
        (iWeaponCategory!=0 && it->GetWeaponCategory()==iWeaponCategory)
      );
  }

  static bool setTools(recipedata& rpd,item* itTool,item* itTool2){
    addTool(rpd,itTool);
    addTool(rpd,itTool2);
    return rpd.itTool2!=NULL;
  }
  static void addTool(recipedata& rpd,item* itTool){
    if(rpd.itTool==itTool)return;
    if(rpd.itTool2==itTool)return;

    if(rpd.itTool==NULL){
      rpd.itTool=itTool;
      return;
    }

    if(rpd.itTool2==NULL){
      rpd.itTool2=itTool;
      return;
    }

    ABORT("at most 2 tools '%s' '%s' '%s'",rpd.itTool->GetName(INDEFINITE).CStr(),rpd.itTool2->GetName(INDEFINITE).CStr(),itTool->GetName(INDEFINITE).CStr());
  }

  static item* findCarvingToolSpecific(recipedata& rpd, item* itTool,int iMinCarvingStr, int iType, int& riMult, int iIncMult){ //this one is to avoid using #define mess :>, but instead of a simple if() we get several method calls... tho more readable...
    if(itTool==NULL){
      itTool = FindTool(rpd, iType, 0, iMinCarvingStr);
      if(itTool!=NULL)riMult+=iIncMult;
    }
    return itTool;
  }
  static item* findTailoringTool(recipedata& rpd,item* itToWorkOn){
    int iCarvingStr=0;

    material* matM = itToWorkOn->GetMainMaterial();
    material* matS = itToWorkOn->GetSecondaryMaterial();
    if(!craftcore::IsMeltable(matM))
      iCarvingStr=matM->GetStrengthValue();
    if(matS!=NULL && !craftcore::IsMeltable(matS))
      iCarvingStr=Max(iCarvingStr,matS->GetStrengthValue());

    int iMinCarvingStr = iCarvingStr/2;

    // any blanded thing bug preferably a dagger
    int iMult=1;
    item* itTool = FindTool(rpd, DAGGER, 0, iMinCarvingStr); //carving: tool cant be too much weaker
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,DAGGER,iMult,0); //TODO should be SCISSORS
    if(itTool!=NULL){
      if(iCarvingStr>1){
        int itStr=itTool->GetMainMaterial()->GetStrengthValue();
        if(itStr<iCarvingStr)
          iMult++;
        if(itStr==iMinCarvingStr)
          iMult++;
      }
      calcToolTurns(rpd,iMult);

      if(!recipe::findOLT(rpd,WORK_BENCH)){
        ADD_MESSAGE("As you lack a workbench, it will take a while."); //it is good to measure, hold tight, has a good height etc...
        rpd.iBaseTurnsToFinish *= 3;
      }
    }else{
      //ADD_MESSAGE("You have no carving tool good enough to work on the requested material.");
      // Already covered by failToolMsg()
      rpd.bAlreadyExplained=true;
    }

    return itTool;
  }
  static item* findCarvingTool(recipedata& rpd,item* itToWorkOn){
    int iCarvingStr=0;

    material* matM = itToWorkOn->GetMainMaterial();
    material* matS = itToWorkOn->GetSecondaryMaterial();
    if(!craftcore::IsMeltable(matM))
      iCarvingStr=matM->GetStrengthValue();
    if(matS!=NULL && !craftcore::IsMeltable(matS))
      iCarvingStr=Max(iCarvingStr,matS->GetStrengthValue());

    int iMinCarvingStr = iCarvingStr/2;

    // any blanded thing bug preferably a dagger
    int iMult=1;
    item* itTool = FindTool(rpd, DAGGER, 0, iMinCarvingStr); //carving: tool cant be too much weaker
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,DAGGER,iMult,0);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,SICKLE,iMult,1);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,SHORT_SWORD,iMult,1);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,AXE,iMult,2);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,MEAT_CLEAVER,iMult,2);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,LONG_SWORD,iMult,2);
    itTool = findCarvingToolSpecific(rpd,itTool,iMinCarvingStr,SPEAR,iMult,3); // mmm... spear is easy enough to be found already TODO all other more difficult to be used weapons with blades may be added below here :), basically create a balsa dagger using a balsa spear and you are good to go xD
    if(itTool!=NULL){
      if(iCarvingStr>1){
        int itStr=itTool->GetMainMaterial()->GetStrengthValue();
        if(itStr<iCarvingStr)
          iMult++;
        if(itStr==iMinCarvingStr)
          iMult++;
      }
      calcToolTurns(rpd,iMult);

      if(!recipe::findOLT(rpd,WORK_BENCH)){
        ADD_MESSAGE("As you lack a workbench, it will take a while."); //it is good to measure, hold tight, has a good height etc...
        rpd.iBaseTurnsToFinish *= 3;
      }
    }else{
      //ADD_MESSAGE("You have no carving tool good enough to work on the requested material.");
      // Already covered by failToolMsg()
      rpd.bAlreadyExplained=true;
    }

    return itTool;
  }

  static item* FindTool(recipedata& rpd, int iCfg, int iWeaponCategory=0, int iMainMatterMinStrengh=0){
    itemvector vit = vitInv(rpd);
    for(int i=0;i<vit.size();i++)
      if(chkWeapon(vit[i],iCfg,iWeaponCategory,iMainMatterMinStrengh))
        return vit[i];
    for(int i=0;i<vit.size();i++)
      if(chkWeapon(vit[i],iCfg|BROKEN,iWeaponCategory,iMainMatterMinStrengh))
        return vit[i];
    return NULL;
  }

  static item* FindByWeaponCategory(recipedata& rpd,int iWCategory){
    return FindTool(rpd,0,iWCategory);
  }

  static void calcToolTurns(recipedata& rpd,int iPrecalculatedMultBasedOnTool){
    if(iPrecalculatedMultBasedOnTool==0)
      return;

    int iBaseTurns = rpd.iBaseTurnsToFinish;
    float fIncTurnsStep=0.25;
    int iAddTurns=iBaseTurns*(iPrecalculatedMultBasedOnTool*fIncTurnsStep);
    rpd.iBaseTurnsToFinish = iBaseTurns + iAddTurns;
  }

  static item* FindBluntTool(recipedata& rpd){
    //TODO could be based on volume and weight vs strengh and dexterity to determine how hard is to use the tool, unbalanced considering the required precision
    static const int iTotToolTypes=3;
    static const int aiTypes[iTotToolTypes]={HAMMER, FRYING_PAN, WAR_HAMMER};

    item* it = NULL;
    int iMult = 0;
    for(int j=0;j<iTotToolTypes;j++){DBG2(j,aiTypes[j]);
      it = FindTool(rpd, aiTypes[j]);
      if(it){DBG2(it->GetConfig(),it->GetName(DEFINITE).CStr());
        iMult=j;
        break;
      }
    }

    if(it==NULL){
      it = FindByWeaponCategory(rpd,BLUNT_WEAPONS);
      iMult=iTotToolTypes; //max
    }

    if(it!=NULL)
      calcToolTurns(rpd,iMult);
    else{
      ADD_MESSAGE("You have no blunt tool.");
      rpd.bAlreadyExplained=true;
    }

    return it;
  }

  static item* FindCuttingTool(recipedata& rpd){//,bool bReversedTimeMult=false){
    int iBaseTurns = rpd.iBaseTurnsToFinish;

    item* it = NULL;
    int iMult = 0;
    int iMaxMult = 0;

    #define FBWC(WC) \
      if(it==NULL){ \
        it = FindByWeaponCategory(rpd,WC); \
        if(it==NULL)iMult++; \
      } \
      iMaxMult++;
    FBWC(SMALL_SWORDS); //DAGGER is here
    FBWC(AXES); //has more cutting power but less precision and takes more time TODO is this good?
    FBWC(LARGE_SWORDS); //slower
    FBWC(POLE_ARMS); //even slower, hard to use

    if(it!=NULL){
//      if(bReversedTimeMult)
//        iMult = iMaxMult-iMult;

      calcToolTurns(rpd,iMult);
    }else{
      ADD_MESSAGE("You have no cutting tool.");
      rpd.bAlreadyExplained=true;
    }

    return it;
  }

  /**
   * prepare the filter for ALL items also resetting them first!
   */
  template <typename T> static void prepareFilter(
    recipedata& rpd,
    const itemvector& vi,
    long reqVol,
    ci CI=ci(),
    std::vector<ulong>* ptmpIngredientsIDs = NULL
  ){
    for(int i=0;i<vi.size();i++){
      vi[i]->SetValidRecipeIngredient(false);
      if(dynamic_cast<T*>(vi[i])!=NULL){
        if(vi[i]->IsBurning())continue;

        /**
         * w/o this, things can be indirectly repaired to their original state,
         * unless spawining/cloning/copying changes to have the same
         * (or average in case of many to one) degradation data
         *
         * degradation should only be "fixable" thru magic (scroll of repair)
         */
        if(!CI.bAllowDegradation && craftcore::IsDegraded(vi[i])) //TODO lower the volume of "remaining perfect materials" based on the degradation levels ?
          continue;

        if(CI.iReqCfg>0 && vi[i]->GetConfig()!=CI.iReqCfg)
          continue;

        if(CI.bFirstItemMustHaveFullVolumeRequired && vi[i]->GetVolume()<reqVol)
          continue;

        if(!CI.bAllowMeltables && craftcore::IsMeltable(vi[i]))
          continue;
        
        if(CI.bMustBeTailorable && !(vi[i]->GetMainMaterial()->GetCategoryFlags() & CAN_BE_TAILORED))
          continue;

        if(vi[i]->GetStrengthValue() < CI.iMinMainMaterStr)
          continue;

        bool bAlreadyUsed=false;
        for(int j=0;j<rpd.ingredientsIDs.size();j++){
          if(vi[i]->GetID()==rpd.ingredientsIDs[j]){
            bAlreadyUsed=true;
            break;
          }
        }
        if(ptmpIngredientsIDs!=NULL)
          for(int j=0;j < ptmpIngredientsIDs->size();j++){
            if(vi[i]->GetID() == (*ptmpIngredientsIDs)[j]){
              bAlreadyUsed=true;
              break;
            }
          }
        if(bAlreadyUsed)
          continue;

        if(CI.iReqMatCfgMain > 0 && vi[i]->GetMainMaterial()->GetConfig() != CI.iReqMatCfgMain)
          continue;

        if(CI.iReqMatCfgSec > 0 && vi[i]->GetMainMaterial()->GetConfig() != CI.iReqMatCfgSec)
          continue;

        vi[i]->SetValidRecipeIngredient(true);
      }
    }
  }

  template <typename T> static truth choseIngredients(
      cfestring fsQ,
      long reqVolPrecise,
      recipedata& rpd,
      int& iWeakestCfg,
      ci CI=ci()
  ){DBGLN;
    if(CI.fUsablePercVol>1.0 || CI.fUsablePercVol<=0)
      ABORT("usable vol is max 100%% %f",CI.fUsablePercVol);

    /**
     * ex.: vol=100; usable=0.5; req=200=100/0.5;
     * remaining lump: after shapping the stone, should become lump, ex.: dagger 25cm3 req 33cm3,
     * prepare a stone with 25cm3 and a lump with 8cm3
     */
    long reqVolTotal = reqVolPrecise/CI.fUsablePercVol;

    long reqVol = reqVolTotal;

    if(reqVol==0)
      ABORT("ingredient required 0 volume?");

    const itemvector vi = vitInv(rpd);
    prepareFilter<T>(rpd,vi,reqVol,CI);

    int iWeakest=-1;
    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true);
    std::vector<ulong> tmpIngredientsIDs;
    for(;;)
    {
      itemvector ToUse;
      game::DrawEverythingNoBlit();
      int flags = CI.bMultSelect ? REMEMBER_SELECTED : REMEMBER_SELECTED|NO_MULTI_SELECT;

      festring fsFullQ;
      festring specialDesc;
      if(CI.bOverridesQuestion)
        fsFullQ=fsQ;
      else{
        fsFullQ = festring("What ingredient(s) will you use? (hit ESC for more options if available)");
        specialDesc = fsQ+" ["+reqVol+"cm3]";
      }

//      rpd.rc.H()->GetStack()->DrawContents(ToUse, rpd.rc.H(),
//        fsFullQ, flags, &item::IsValidRecipeIngredient);
      rpd.rc.H()->GetStack()->DrawContents(ToUse, 0, rpd.rc.H(), fsFullQ, CONST_S(""),
        CONST_S(""), specialDesc, WHITE, flags, &item::IsValidRecipeIngredient);

      if(ToUse.empty())
        break;

      if(CI.bJustAcceptFirstChosenAndReturn){
        tmpIngredientsIDs.push_back(ToUse[0]->GetID());
      }else for(int i=0;i<ToUse.size();i++){
        material* matM = ToUse[i]->GetMainMaterial();
        if(CI.bFirstMainMaterIsFilter && CI.iReqMatCfgMain==0)
          CI.iReqMatCfgMain=matM->GetConfig();

        if(iWeakest==-1 || iWeakest > matM->GetStrengthValue()){
          iWeakest = matM->GetStrengthValue();
          iWeakestCfg = matM->GetConfig();
          rpd.itWeakestIngredient = ToUse[i];
        }

        tmpIngredientsIDs.push_back(ToUse[i]->GetID()); DBG1(ToUse[i]->GetID());
        reqVol -= ToUse[i]->GetVolume(); DBG2(reqVol,ToUse[i]->GetVolume());
        ToUse[i]->SetValidRecipeIngredient(false); //just to not be shown again on the list

        if(reqVol<=0){
          long lRemainingVol = reqVol * -1; //beyond required

          if(CI.fUsablePercVol<1.0) //while shapping this is what is "lost"
            lRemainingVol += reqVolTotal * (1.0 - CI.fUsablePercVol); //ex.: a round rock being shaped into a spear tip

          if(lRemainingVol>0 && CI.bMainMaterRemainsBecomeLump){
            long lVolM = matM->GetVolume();
            lVolM -= lRemainingVol; //to sub
            if(lVolM<=0)
              ABORT("ingredient volume reduced to negative or zero %ld %ld %s %s",lVolM,lRemainingVol,matM->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
            if(!CI.bMultSelect && lVolM!=reqVolPrecise) //TODO use error margin because of float VS integer calc? ex.: if diff is +1 or -1, just allow it.
              ABORT("remaining vol calc needs fixing %ld != %ld, %f, %ld",lVolM,reqVolPrecise,CI.fUsablePercVol,lRemainingVol);
            matM->SetVolume(lVolM); //TODO there is a big problem here... if the crafting cannot be started because a tool is missing, this ingredient will have less volume than required and may not be reused in case it is a non-meltable!!!

//            bool bForceLump = CI.fUsablePercVol<1.0;
//            item* lumpR = craftcore::PrepareRemains(rpd,matM,bForceLump);
            item* lumpR = craftcore::PrepareRemains(rpd,matM,CIT_NONE,lRemainingVol);
            //lumpR->GetMainMaterial()->SetVolume(lRemainingVol);

            if(CI.bMixRemainingLump)
              lumpMix(vi,lumpR,rpd.bSpendCurrentTurn);

            material* matS = ToUse[i]->GetSecondaryMaterial();
            if(matS!=NULL && matS->GetVolume()>0)
              ABORT("ingredient secondary material should not have volume %ld %s %s",matS->GetVolume(),matS->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
          }

          break; //SUCCESS!
        }

//        if(CI.bFirstItemMustHaveFullVolumeRequired)
//          break; //may fail
      }

      if(reqVol<=0 || CI.bInstaAddIngredients || CI.bJustAcceptFirstChosenAndReturn){
        for(int i=0;i<tmpIngredientsIDs.size();i++)
          rpd.ingredientsIDs.push_back(tmpIngredientsIDs[i]);

        if(CI.bJustAcceptFirstChosenAndReturn)
          break;

        if(CI.bInstaAddIngredients)
          tmpIngredientsIDs.clear(); //to avoid dups
        else
          break; //success
      }

      if(CI.bFirstMainMaterIsFilter) //prepare filter for next list display
        prepareFilter<T>(rpd,vi,reqVol,CI,&tmpIngredientsIDs);

//      if(CI.bFirstItemMustHaveFullVolumeRequired)
//        break;
    }

    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);

    if(!CI.bJustAcceptFirstChosenAndReturn)
      if(CI.bMsgInsuficientMat && reqVol>0){ // && rpd.ingredientsIDs.size()>0)
        festring fsIngTpNm;
        if(CI.iReqCfg==INGOT)
          fsIngTpNm = "ingot";
        else
        {
          // TODO:
          //  demangling simple like that may give weird text one day if other types are added
          //  This is way too confusing, just use generic "material" and rework later.
          //fsIngTpNm = std::string(typeid(T).name()).substr(1).c_str();
          fsIngTpNm = "material";
        }

        ADD_MESSAGE("You don't have enough %ss.",fsIngTpNm.CStr());
      }

    if(reqVol<=0)
      return true;

    if(CI.bJustAcceptFirstChosenAndReturn && rpd.ingredientsIDs.size()>0)
      return true;

    return false;
  }

  template <typename T> static bool choseOneIngredient(recipedata& rpd, ci* pCI=NULL){
    int iWeakestCfgDummy;
    ci CIok;
    if(pCI)CIok=*pCI;
    CIok.bMultSelect=false;
    CIok.bJustAcceptFirstChosenAndReturn=true;
    return choseIngredients<T>(
      festring(""),
      1, //just to chose one of anything
      rpd,
      iWeakestCfgDummy,
      CIok);
  }

  static itemvector vitInv(recipedata& rpd){
    itemvector vi;

    //prefer already equipped
    if(rpd.rc.H()->GetLeftWielded ())vi.push_back(rpd.rc.H()->GetLeftWielded ());
    if(rpd.rc.H()->GetRightWielded())vi.push_back(rpd.rc.H()->GetRightWielded());

    rpd.rc.H()->GetStack()->FillItemVector(vi); //TODO once, the last item from here had an invalid pointer, HOW?

    return vi;
  }

  static void lumpMix(itemvector vi,item* lumpToMix, bool& bSpendCurrentTurn){
    // to easily (as possible) create a big lump
    for(int i=0;i<vi.size();i++){
      if(lumpToMix==vi[i])continue;

      if(dynamic_cast<lump*>(vi[i])!=NULL){
        lump* lumpAtInv = (lump*)vi[i];
        if(lumpAtInv->GetMainMaterial()->GetConfig() == lumpToMix->GetMainMaterial()->GetConfig()){
          lumpAtInv->GetMainMaterial()->SetVolume(
            lumpAtInv->GetMainMaterial()->GetVolume() + lumpToMix->GetMainMaterial()->GetVolume());
          lumpAtInv->CalculateAll();

          craftcore::SendToHellSafely(lumpToMix); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
//          lumpToMix->RemoveFromSlot();
//          lumpToMix->SendToHell(); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
          bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...
          break;
        }
      }
    }
  }

  static void SetOLT(recipedata& rpd,lsquare* lsqr,int iCfgOLT){
    switch(iCfgOLT){
    case FORGE:
      rpd.v2ForgeLocation = lsqr->GetPos();
      break;
    case ANVIL:
      rpd.v2AnvilLocation = lsqr->GetPos();
      break;
    case WORK_BENCH:
      rpd.v2WorkbenchLocation = lsqr->GetPos();
      break;
    }
  }

  static bool chkOLT(recipedata& rpd,lsquare* lsqr,int iCfgOLT){DBGLN;
    olterrain* ot = lsqr->GetOLTerrain();
    if(ot!=NULL && ot->GetConfig() == iCfgOLT){
      return true;
    }
    return false;
  }
  static bool findOLT(recipedata& rpd,int iCfgOLT,bool bReqOnlyVisible=false){
    bool bFound=false;
    //obs.: lsqr->CanBeFeltByPlayer() will NOT work if player CAN MOVE on it!
    if(bReqOnlyVisible){DBGLN; //even if far away
      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2(iX,iY));DBG3(lsqr,iX,iY);
        if( chkOLT(rpd,lsqr,iCfgOLT) && (lsqr->GetPos().IsAdjacent(rpd.rc.H()->GetPos()) || lsqr->CanBeSeenBy(rpd.rc.H())) ){
          SetOLT(rpd,lsqr,iCfgOLT);
          bFound = true;
          break;
        }
      }}
    }else{DBGLN; //must be on adjacent square
      for(int iDir=0;iDir<8;iDir++){
        v2 v2Add = game::GetMoveVector(iDir);
        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG3(DBGAV2(v2Add),DBGAV2(v2Pos),iDir);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);DBG1(lsqr);
//          if(lsqr->CanBeFeltBy(rpd.rc.H()) && chkOLT(rpd,lsqr,iCfgOLT)){
          if(chkOLT(rpd,lsqr,iCfgOLT) && lsqr->GetPos().IsAdjacent(rpd.rc.H()->GetPos())){
            SetOLT(rpd,lsqr,iCfgOLT);
            bFound = true;
            break;
          }
        }
      }
    }

    if(!bFound){
      festring fsWhat;
      switch(iCfgOLT){
      case FORGE:fsWhat="forge";break;
      case ANVIL:fsWhat="anvil";break;
      case WORK_BENCH:fsWhat="workbench";break;
      }

      festring fsMsg="No ";
      if(bReqOnlyVisible)
        fsMsg<<"visible ";
      fsMsg<<fsWhat<<" nearby.";
      ADD_MESSAGE(fsMsg.CStr());

      rpd.bAlreadyExplained=true;

      return false;
    }

    return true;
  }
};

/**
 * As we can't (shouldn't) kick webs...
 *
 * This is a special kind of "recipe"
 * is a way to change the existing environment, like engrave does,
 * but this one was implemented as crafting code
 * so in short, this could be a command like engrave is, but was implemented thru crafting.
 *
 * This action is like a simple slash on the web, so will spend only one turn.
 *
 * TODO
 * may be, more functionality could be added, like collect web (spiker silk) to be able to
 * craft leather/clothing or other things.
 * may be even recreate a single square spider web with it's strength based on craft skill!
 */
struct srpCutWeb : public recipe{
  virtual void fillInfo(){
    init("cut","a web");
    desc << "Cut a web down. Much safer than tearing it down, especially if you use a cutting tool. You might still get stuck, though.";
  }

  virtual bool work(recipedata& rpd){
    if(!recipe::whereRaw(rpd,"Cut a web where?",true)){
      rpd.bAlreadyExplained=true; //no need to explain a cancelled action
      return false;
    }

    web* w=NULL;
    std::vector<trap*> TrapVector;
    rpd.lsqrPlaceAt->FillTrapVector(TrapVector);
    for(int i=0;i<TrapVector.size();i++){
      if(dynamic_cast<web*>(TrapVector[i])==NULL)
        continue;
      w = (web*)TrapVector[i];
      break;
    }
    if(w==NULL){
      ADD_MESSAGE("There is no web there.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    humanoid* h = rpd.rc.H();

    arm* ra = h->GetRightArm();
    arm* la = h->GetLeftArm();
    if(ra && !ra->IsUsable())ra=NULL;
    if(la && !la->IsUsable())la=NULL;
    if(!ra && !la){
      ADD_MESSAGE("You have no usable arm to do that.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    bool bSelfPos = rpd.lsqrPlaceAt->GetPos() == h->GetPos();

    rpd.itTool = FindCuttingTool(rpd); // no blunt, no non-cutting, imagine a web that can wold on air the weight of a whole body, only cutting tools
    rpd.bAlreadyExplained=false;
    item* wieldBkp=NULL;
    bool bIsWBkpRHand = true;
    if(rpd.itTool!=NULL){
      bool bWielded=false;
      #define RLWIELD(rl,br) \
        if(!bWielded && h->Get##rl##Arm() && !h->Get##rl##Arm()->IsStuck()){ \
          if(h->Get##rl##Wielded()){ \
            wieldBkp = h->Get##rl##Wielded(); \
            wieldBkp->MoveTo(h->GetStack()); \
          } \
          rpd.itTool->RemoveFromSlot(); \
          h->Set##rl##Wielded(rpd.itTool); \
          bWielded=true; \
          bIsWBkpRHand=br; \
        }
      RLWIELD(Right,true);
      RLWIELD(Left,false);
    }

    /**
     * IMPORTANT!
     * this repetition is about action quality and NOT time taken
     * because trying to tear down the web is a random check
     * based on a fixed modifier (vanilla one) per turn!!!
     */
    int tot=1;
    if(ra && la)
      tot+=2; //means something like using both arms to destroy a single web spot
    if(bSelfPos)
      tot *= 3; //to make it worther than just trying to move, and to compensate for not moving too as player won't insta flee from attacks
    if(rpd.itTool!=NULL) //float multiplier last thing!
      tot *= 1 + craftcore::CraftSkill(h)/10.0;
    DBG1(tot);
    bool bSuccess = false;
    for(int i=0;i<tot;i++){
      if(w->TryToTearDown(h)){
        bSuccess=true;
        break;
      }
    }

    if(bSuccess){
      rpd.bAlreadyExplained=true;
      material* matSSilk=material::MakeMaterial(SPIDER_SILK);
      craftcore::FinishSpawning(rpd, craftcore::PrepareRemains(rpd,matSSilk,CIT_LUMP,clock()%6+3));
    }else{
      bool bGetStuckOnTheWeb=false;
      bool bLoseWeapon=false; //TODO if has no weapon, lose one glove instead!
      bool bCriticalFumble=false;
      int iFumblePower=0;
      if(craftcore::CheckFumble(rpd, bCriticalFumble, iFumblePower)){DBGLN;
        if(bCriticalFumble){DBGLN;
          bGetStuckOnTheWeb=true;
          if(rpd.itTool!=NULL)
            if(clock()%100 < 50){DBGLN;
              bLoseWeapon=true;
            }
        }else{DBGLN;
          if(rpd.itTool!=NULL){DBGLN;
            bLoseWeapon=true;
          }else{DBGLN;
            bGetStuckOnTheWeb=true;
          }
        }
      }

      if(bGetStuckOnTheWeb){
        if(rpd.lsqrPlaceAt->GetCharacter()==NULL){
          h->Remove();
          h->PutTo(rpd.lsqrPlaceAt->GetPos()); //TODO check for walkability first!
          w->StepOnEffect(h);
          ADD_MESSAGE("You've got stuck on the web!");
          rpd.bAlreadyExplained=true;
        }else{
          if(rpd.itTool!=NULL){
            bLoseWeapon=true;
          }
        }
      }

      if(bLoseWeapon){
        rpd.itTool->RemoveFromSlot();
        rpd.itTool->MoveTo(rpd.lsqrPlaceAt->GetStack()); //TODO check if is not a WALL!!!
        ADD_MESSAGE("You lost your %s!",rpd.itTool->GetName(UNARTICLED).CStr());
        rpd.bAlreadyExplained=true;
      }

      if(bSelfPos && !bGetStuckOnTheWeb && !bLoseWeapon){
        w->StepOnEffect(h); //so every try will make it more difficult!! :)
      }

      int iSt = w->GetTrapBaseModifier();DBG1(iSt);
      iSt -= 1 + clock()%5; // small spider = 10, big = 25, wand beam = 50
      if(iSt<=0)
        iSt=1;
      w->SetStrength(iSt);

      if(!rpd.bAlreadyExplained){
        ADD_MESSAGE("You fail to tear down the web.");
        rpd.bAlreadyExplained=true;
      }

    }

    if(wieldBkp!=NULL){
      if(wieldBkp->GetSlot()->FindCarrier() == h){
        wieldBkp->RemoveFromSlot();;
        if(bIsWBkpRHand){
          if(h->GetRightWielded())
            h->GetRightWielded()->MoveTo(h->GetStack());
          h->SetRightWielded(wieldBkp);
        }else{
          if(h->GetLeftWielded())
            h->GetLeftWielded()->MoveTo(h->GetStack());
          h->SetLeftWielded(wieldBkp);
        }
      }
    }

    h->EditAP(-500); //to let time pass
    craftcore::CraftSkillAdvance(rpd); //TODO this should be related to collect spider silk to craft one day with it

    rpd.bSpendCurrentTurn=true;

    return true;
  }
};srpCutWeb rpCutWeb;

struct srpOltBASE : public recipe{
 protected:
  int iReqVol=0;
  int iReqVolSkulls=0;
  int iTurns=0;
  bool bRequiresWhere=false;
  bool bAllowSimpleStones=false;
  bool bReqForge=false;
  bool bReqToolBlunt=true;
  ci CIdefault;
  festring fsDescBASE = "You will need a hammer or other blunt tool.";

  virtual bool spawnCfg(recipedata& rpd){return false;}

 public:
  virtual bool work(recipedata& rpd){DBGLN;
    if(iReqVol==0)ABORT("Recipe vol is 0 for OLT");
    if(iTurns==0)ABORT("Recipe turns is 0 for OLT");

    if(bReqForge)
      if(!recipe::findOLT(rpd,FORGE))
        return false;

    DBGLN;
    if(bRequiresWhere){DBGLN;
      if(!recipe::where(rpd))
        return false;
    }else{DBGLN;
      rpd.lsqrPlaceAt=rpd.lsqrCharPos;
    }

    rpd.iBaseTurnsToFinish=iTurns;

    if(bReqToolBlunt){
      rpd.itTool = FindBluntTool(rpd);
      if(rpd.itTool==NULL)
        return false;
    }

    if(rpd.lsqrPlaceAt->GetOLTerrain()!=NULL){
      ADD_MESSAGE("It can't be placed here.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    rpd.bCanBePlaced=true;

    festring fsQ("to build ");fsQ<<name;
    int iCfg=-1;
    bool bH=false;
    CIdefault.bFirstMainMaterIsFilter=false; //TODO move to constructor
    if(!bH){
      ci CI = CIdefault;
      CI.iReqCfg = bAllowSimpleStones?0:INGOT;
      CI.bMainMaterRemainsBecomeLump = true;
      bH=choseIngredients<stone>(fsQ, iReqVol, rpd, iCfg, CI); //true, bAllowSimpleStones?0:INGOT, true);
    }
    if(!bH && CIdefault.bAllowWood)
      bH=choseIngredients<stick>(fsQ, iReqVol, rpd, iCfg, CIdefault);
    if(CIdefault.bAllowBones){
      if(!bH && iReqVolSkulls>0)
        bH=choseIngredients<skull> (fsQ, iReqVolSkulls, rpd, iCfg, CIdefault);
      if(!bH)
        bH=choseIngredients<bone> (fsQ, iReqVol, rpd, iCfg, CIdefault);
    }

    if(bH){
      rpd.bHasAllIngredients=bH;
      rpd.v2PlaceAt = rpd.lsqrPlaceAt->GetPos();
      if(!spawnCfg(rpd))ABORT("Recipe spawn cfg not set %s",desc.CStr());
      rpd.otSpawnMatMainCfg=iCfg;
      rpd.otSpawnMatMainVol=iReqVol;
      rpd.bCanStart=true;
    }else
      failIngredientsMsg(rpd);

    return true;
  }
};
struct srpDoor : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_DOOR;
    rpd.otSpawnCfg=SECRET_DOOR; //it currently just looks like BRICK_OLD
    return true;
  }

  virtual void fillInfo(){
    init("build","a secret door");
    desc << "Build a door. You will need a hammer or some other blunt tool, plus sticks, bones or ingots.";
  }

  virtual bool work(recipedata& rpd){
    iReqVol=30000;  //TODO this volume should be on the .dat file TODO breaking a door will only drop a few lumps about 2500cm3 only... makes no sense for metal doors
    iTurns=30;
    return srpOltBASE::work(rpd);
  }
};srpDoor rpDoor;
struct srpChair : public srpOltBASE{
//  virtual olterrain* spawn(recipedata& rpd){
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_FURNITURE;
    rpd.otSpawnCfg=CHAIR;
//    return decoration::Spawn(CHAIR);
    return true;
  }

  virtual void fillInfo(){
    init("build","a chair");
    desc << "Create a chair to rest your tired legs. You will need a hammer or some other blunt tool.";
  }

  virtual bool work(recipedata& rpd){
    iReqVol=15000;       //TODO this volume should be on the .dat file
    iTurns=15;
    return srpOltBASE::work(rpd);
  }
};srpChair rpChair;
struct srpAnvil : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_FURNITURE;
    rpd.otSpawnCfg=ANVIL;
    return true;
  }

  virtual void fillInfo(){
    init("build","an anvil");
    desc << "Create an anvil for further crafting, using ingots. You must be near a forge.\n " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=750*3; //when destroyed provides 250 to 750 x3, so lets use the max to avoid spawning extra material volume
    iTurns=15;
    bReqForge=true;
    CIdefault.bAllowBones=false;
    CIdefault.bAllowWood=false;
    return srpOltBASE::work(rpd);
  }
};srpAnvil rpAnvil;
struct srpForge : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_FURNITURE;
    rpd.otSpawnCfg=FORGE;
    return true;
  }

  virtual void fillInfo(){
    init("build","a forge");
    desc << "Build a forge for further crafting. " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=15000;
    iTurns=30;
    bRequiresWhere=true;
    bAllowSimpleStones=true;
    CIdefault.bAllowBones=false;
    CIdefault.bAllowWood=false;
    //TODO require fire source like fireball wand or 3 lanterns
    return srpOltBASE::work(rpd);
  }
};srpForge rpForge;
struct srpWorkBench : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_FURNITURE;
    rpd.otSpawnCfg=WORK_BENCH;
    return true;
  }

  virtual void fillInfo(){
    init("build","a workbench");
    desc << "Build a workbench for further crafting. " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=3000;
    iTurns=10;
    bRequiresWhere=true;
    return srpOltBASE::work(rpd);
  }
};srpWorkBench rpWorkBench;
struct srpWall2 : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_WALL;

    rpd.otSpawnCfg=BRICK_OLD;
//    if(dynamic_cast<stone>(game::SearchItem(rpd.ingredientsIDs[0]))!=NULL)
    if(dynamic_cast<stone*>(rpd.itWeakestIngredient)!=NULL)
      rpd.otSpawnCfg=STONE_WALL;

    return true;
  }

  virtual void fillInfo(){
    init("construct","a wall");
    desc << "Construct a wall by piling stones, sticks or bones.";
  }

  virtual bool work(recipedata& rpd){
    //TODO this doesnt look good. anyway this volume should be on the .dat file as wall/earthWall attribute...
    iReqVol=9000; //TODO is this too little? a broken wall drops 3 rocks that is about 1000 each, so 3 walls to build one is ok?
    iReqVolSkulls=12000; //TODO is this too little? necromancers can spawn skeletons making it easy to get skulls, but the broken bone wall will drop bones and not skulls...
    iTurns=20;
    bRequiresWhere=true;
    bAllowSimpleStones=true;
    bReqToolBlunt=false;
    return srpOltBASE::work(rpd);
  }
};srpWall2 rpWall2;

struct srpJoinLumps : public recipe{
  virtual void fillInfo(){
    init("merge","lumps");
    desc << "Merge lumps of the same material into a single, bigger one.";
  }

  void askForEqualLumps(recipedata& rpd){
    ci CI;
    CI.bOverridesQuestion=true;
    CI.bMsgInsuficientMat=false;
    CI.bInstaAddIngredients=true;
    int iWeakestCfgDummy;
    bool bDummy = choseIngredients<lump>(
      festring("First chosen lump's material will be mixed with further ones of same material only, hit ESC to accept."),
      1000000, //just any "impossible" huge volume as "limit"
      rpd, iWeakestCfgDummy, CI); // true, 0, false, true, false, true);
  }

  void joinLumpsEqualTo(recipedata& rpd,material* matM){
    // multiple (compatible with 1st) lumps will be mixed in a big one again
    for(int i=1;i<rpd.ingredientsIDs.size();i++){
      item* LumpToAdd = game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
      if(dynamic_cast<lump*>(LumpToAdd)==NULL)continue;

      material* LumpToAddM = LumpToAdd->GetMainMaterial();DBGLN;
      if(LumpToAddM->GetConfig()!=matM->GetConfig())continue;

      // join
      matM->SetVolume(matM->GetVolume()+LumpToAddM->GetVolume());DBGLN;

      craftcore::SendToHellSafely(LumpToAdd);
    }
  }

  void joinLumpsEqualToFirst(recipedata& rpd){
    item* Lump = game::SearchItem(rpd.ingredientsIDs[0]);
    material* matM=Lump->GetMainMaterial();
    joinLumpsEqualTo(rpd,matM);
  }

  virtual bool work(recipedata& rpd){ // it is just like to put them all together, no effort, instant.
    askForEqualLumps(rpd);

    if(rpd.ingredientsIDs.empty()){
      ADD_MESSAGE("You have no lumps to work with.");
      rpd.bAlreadyExplained = true;
      return false;
    }

    joinLumpsEqualToFirst(rpd);
    craftcore::CraftSkillAdvance(rpd);

    rpd.bAlreadyExplained = true;
    
    return true;
  }
};srpJoinLumps rpJoinLumps;

struct srpMelt : public srpJoinLumps{
  virtual void fillInfo(){
    init("melt","an ingot");
    desc << "Melt lumps of metal into ingots, later usable in crafting. You must be near a forge.";
  }

  virtual bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.

    if(!recipe::findOLT(rpd,FORGE))
      return false;

    askForEqualLumps(rpd);

    if(rpd.ingredientsIDs.empty()){
      failIngredientsMsg(rpd);
      return false;
    }

    item* Lump = game::SearchItem(rpd.ingredientsIDs[0]);

    item* LumpMeltable=NULL;
    if(craftcore::IsMeltable(Lump))
      LumpMeltable = Lump;

    if(LumpMeltable==NULL){
      ADD_MESSAGE("Can't melt %s.",Lump->GetName(INDEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    material* matM=LumpMeltable->GetMainMaterial();

    joinLumpsEqualTo(rpd,matM);

    rpd.iBaseTurnsToFinish = calcMeltableTurns(matM,5); DBG1(rpd.iBaseTurnsToFinish);

    rpd.bGradativeCraftOverride=true;

    rpd.bHasAllIngredients=true;

    rpd.itSpawnType = CIT_STONE;
    rpd.itSpawnCfg = INGOT;
//    rpd.itSpawn = stone::Spawn(INGOT, NO_MATERIALS);

    /**
     * 25cm3 is each of the 2 parts of a dagger, but is too small?
     * Smaller ingots are easier to manage, less user interaction as they fit better.
     * BUT it may generate a HUGE LOT of tiny ingots and slow down the game when dropping/picking up :(
     */
    static long iIngotVol = 250; //TODO savegame this
    long iIngotVolTmp = game::NumberQuestion(festring("What volume shall the ingots have? [min 25cm3, ENTER/ESC accepts last (default) ")+iIngotVol+"cm3]",WHITE,true); //TODO how to let ESC cancel it?
    if(iIngotVolTmp>0){
      if(iIngotVolTmp<25)
        iIngotVolTmp=25;
      iIngotVol=iIngotVolTmp;
    }

    long lVolRemaining = 0;
    long lVolM = LumpMeltable->GetMainMaterial()->GetVolume();
    if(lVolM <= iIngotVol){
      rpd.itSpawnTot = 1;
      iIngotVol = lVolM;
    }else{
      rpd.itSpawnTot = lVolM / iIngotVol;
      lVolRemaining = lVolM % iIngotVol;
      if(lVolRemaining>0){ //split
        LumpMeltable->GetMainMaterial()->SetVolume(lVolM - lVolRemaining); // keep the exact remaining volume of what will be used

        /**
         * IMPORTANT!!!
         * the duplicator will vanish with the item ID that is being duplicated
         */
        item* itLumpR = LumpMeltable->DuplicateToStack(rpd.rc.H()->GetStack());

        itLumpR->GetMainMaterial()->SetVolume(lVolRemaining);
      }
    }
    DBG4(lVolRemaining,rpd.itSpawnTot,lVolM,iIngotVol);

//    delete rpd.itSpawn->SetMainMaterial(material::MakeMaterial(
//        LumpMeltable->GetMainMaterial()->GetConfig(), iIngotVol ));
    rpd.itSpawnMatMainCfg = LumpMeltable->GetMainMaterial()->GetConfig();
    rpd.itSpawnMatMainVol = iIngotVol;

    rpd.ingredientsIDs.clear(); //only the final meltable lump shall be sent to hell when it finishes
    rpd.ingredientsIDs.push_back(LumpMeltable->GetID()); //must be AFTER the duplicator

    rpd.bMeltable = true;

    rpd.bCanStart=true;

    return true;
  }
};srpMelt rpMelt;

struct srpDismantle : public recipe{ //TODO this is instantaneous, should take time?
  virtual void fillInfo(){
    init("dismantle","item");
    desc << "Dismantle an item to recover its materials.\nMetal items require a forge to dismantle, all other need just a cutting tool.";
  }

  virtual bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.

    ///////////////////// chose item to melt/smash
    game::DrawEverythingNoBlit();
    ci CI;
    CI.bAllowDegradation=true; //to let user know what is happening w/o spamming it.
    if(!choseOneIngredient<item>(rpd,&CI)){
      rpd.bAlreadyExplained=true; //no need to explain if nothing chosen
      return false;
    }

    item* itToUse = game::SearchItem(rpd.ingredientsIDs[0]); DBG2(rpd.ingredientsIDs[0],itToUse);
    rpd.ingredientsIDs.clear();

    material* matM=NULL;
    material* matS=NULL;
    material* matIngot=NULL;

    matM = itToUse->GetMainMaterial();
    matS = itToUse->GetSecondaryMaterial();

    if(game::IsQuestItem(itToUse)){
      ADD_MESSAGE("You feel that would be a bad idea and carefully store it back in your inventory.");
      rpd.bAlreadyExplained=true;
      return false;
    }
    // This is a quick fix, maybe allow dismantling mirrored items into mirrored lumps?
    if(itToUse->GetLifeExpectancy())
    {
      ADD_MESSAGE("%s is made of temporary magical force rather than physical matter and cannot be dismantled.",itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(dynamic_cast<corpse*>(itToUse)!=NULL || matM==NULL){ //TODO may be there are other things than corpses that also have no main material?
      ADD_MESSAGE("You should try to split %s instead.",itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(craftcore::IsDegraded(itToUse,true)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    rpd.bMeltable = craftcore::IsMeltable(itToUse);
    if(rpd.bMeltable){
      if(!recipe::findOLT(rpd,FORGE))
        return false;
    }else{
      rpd.itTool = FindCuttingTool(rpd);//,true);
      if(!rpd.itTool){
//        failToolMsg(rpd,"a cutting weapon");
        return false;
      }
    }

    /////////////////////// dismantle into lumps
    if(dynamic_cast<lump*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a lump.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    /**
     * Meltable sticks should be lumpable to easify the code.
     * Kept in case of wanting to re-enabling for some reason...
     *
    if(dynamic_cast<stick*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a stick.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }
     */

    // for now, uses just one turn to smash anything into lumps but needs to be near a FORGE
    // TODO should actually require a stronger hammer than the material's hardness being smashed, and could be anywhere...
    bool bW = itToUse->IsWeapon(rpd.rc.H());
    item* RmnM = craftcore::PrepareRemains(rpd, matM, (bW && itToUse->GetConfig()==QUARTER_STAFF) ? CIT_STICK : CIT_NONE);
    item* RmnS = matS==NULL ? NULL : craftcore::PrepareRemains(rpd, matS, bW ? CIT_STICK : CIT_LUMP); //must always be created to not lose it

    craftcore::EmptyContentsIfPossible(rpd,itToUse,true);

    ADD_MESSAGE("%s was completely dismantled.", itToUse->GetName(DEFINITE).CStr());
    rpd.bAlreadyExplained=true;

    craftcore::SendToHellSafely(itToUse); DBG3("SentToHell",itToUse->GetID(),itToUse); //TODO if has any magic should release it and also harm

    rpd.bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...

    if(dynamic_cast<lump*>(RmnM)!=NULL)
      lumpMix(vitInv(rpd),RmnM,rpd.bSpendCurrentTurn);
    if(dynamic_cast<lump*>(RmnS)!=NULL)
      lumpMix(vitInv(rpd),RmnS,rpd.bSpendCurrentTurn);
    
    craftcore::CraftSkillAdvance(rpd);
    
    return true;
  }
};srpDismantle rpDismantle;

struct srpInspect : public recipe{ //TODO this is instantaneous, should take time?
  virtual void fillInfo(){
    init("inspect","item materials");
    desc << "Carefully inspect what materials an item is made of.";
  }

  virtual bool work(recipedata& rpd){
    ci CI;
    if(!choseOneIngredient<item>(rpd,&CI)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    //TODO add each material details if in possesion of the book of matarials!
    item* it0 = game::SearchItem(rpd.ingredientsIDs[0]);
    material* matM = it0->GetMainMaterial();
    material* matS = it0->GetSecondaryMaterial();
    festring fs;
    fs<<it0->GetName(DEFINITE)<<" is made of ";
    if(matM)fs<<matM->GetName(UNARTICLED);
    if(matS){
      if(matM)fs<<" and "; //actually, there is only 2nd material if there is main but anyway...
      fs<<matS->GetName(UNARTICLED);
    }
    fs<<".";
    if(matM||matS){
      ADD_MESSAGE("%s",fs.CStr());
      craftcore::CraftSkillAdvance(rpd);
    }else{
      ADD_MESSAGE("You can't inspect %s.",it0->GetName(INDEFINITE).CStr());
    }
    rpd.bAlreadyExplained=true;
    return true;
  }
};srpInspect rpInspect;

struct srpResistanceVS : public recipe{ //TODO this is instantaneous, should take time?
  virtual void fillInfo(){
    init("check","material strength");
    desc << "Check the relative hardness of two items. The item made of softer material will receive a scratch.";
  }

  virtual bool work(recipedata& rpd){
    ci CI;
    CI.iMinMainMaterStr=1;
    if(!choseOneIngredient<item>(rpd,&CI)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    // yes, a 2nd time
    if(!choseOneIngredient<item>(rpd,&CI)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(rpd.ingredientsIDs.size()!=2){
      rpd.bAlreadyExplained=true;
      return false;
    }

    item* it0 = game::SearchItem(rpd.ingredientsIDs[0]);
    item* it1 = game::SearchItem(rpd.ingredientsIDs[1]);
    item* itWeaker=NULL;
    item* itStronger=NULL;
    int iStr0=it0->GetMainMaterial()->GetStrengthValue(); //main materials only
    int iStr1=it1->GetMainMaterial()->GetStrengthValue();
    int iStrDiff=iStr0-iStr1;
    float fRatio=Min(iStr0,iStr1)/(float)Max(iStr0,iStr1);

    if(iStrDiff<0){
      ADD_MESSAGE("%s receives a scratch.",it0->GetName(DEFINITE).CStr());
      itWeaker=it0;
      itStronger=it1;
    }else
    if(iStrDiff>0){
      ADD_MESSAGE("%s receives a scratch.",it1->GetName(DEFINITE).CStr());
      itWeaker=it1;
      itStronger=it0;
    }else
    if(iStrDiff==0)
      ADD_MESSAGE("It seems both items are equally hard.");

    if(itWeaker!=NULL){ //the item may break
      float dmg =
        ( itStronger->GetWeight()*itStronger->GetMainMaterial()->GetStrengthValue() ) /
        (float)
        ( itWeaker  ->GetWeight()*itWeaker  ->GetMainMaterial()->GetStrengthValue() )   ;
      itWeaker->ReceiveDamage(rpd.rc.H(), (int)dmg, THROW|PHYSICAL_DAMAGE); //based on item::Fly() "breaks" but not that much
    }

    rpd.bAlreadyExplained=true;
    craftcore::CraftSkillAdvance(rpd);

    return true;
  }
};srpResistanceVS rpResistanceVS;

struct srpSplitLump : public recipe{
  void explain(recipedata& rpd,festring fs){
    ADD_MESSAGE("You need a cutting tool to split %s.",fs.CStr());
    rpd.bAlreadyExplained=true;
  }

  bool reqCut(recipedata& rpd,cfestring fs){
    rpd.itTool = FindCuttingTool(rpd);
    if(rpd.itTool==NULL){
      explain(rpd,fs);
      return false;
    }
    return true;
  }

  virtual void fillInfo(){
    init("split","raw materials");
    desc << "Split raw materials to make them easier to work with.\n"
            "To remove some volume, specify a negative value in cm3.\nIt is good to cut precisely not to waste materials.";
  }

  virtual bool work(recipedata& rpd){
    item* ToSplit = NULL;

    rpd.bGradativeCraftOverride=true; //may be disabled below

    if(ToSplit==NULL && choseOneIngredient<lump>(rpd)){ //can  be split with hands only
      ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
      rpd.itSpawnType = CIT_LUMP;
    }

    bool bHumanoidCorpse=false;
    if(ToSplit==NULL && choseOneIngredient<corpse>(rpd)){ //this is just a simplified conversion from corpse to a big lump...
      corpse* Corpse = (corpse*)game::SearchItem(rpd.ingredientsIDs[0]);

      if(!reqCut(rpd,Corpse->GetName(INDEFINITE)))
        return false;

      //TODO this should take time as an action
      character* D = Corpse->GetDeceased(); DBG2(Corpse->GetName(DEFINITE).CStr(),D->GetName(DEFINITE).CStr())
      static const materialdatabase* flesh;flesh = material::GetDataBase(D->GetFleshMaterial());
      ToSplit = craftcore::PrepareRemains(rpd,material::MakeMaterial(flesh->Config,Corpse->GetVolume()));
      if(dynamic_cast<humanoid*>(D)!=NULL){
        /**
         * this is to try to keep necromancers, raising zombies, challenging,
         * despite their action is random and not timed, so let it be random here too :)
         * TODO instead of just lump, also remove head and limbs (if available) so a friendly zombie could attach it?
         */
        rpd.iMinTurns = 3 + clock()%3;
        rpd.bGradativeCraftOverride=false;

        // tmp flesh lump
        rpd.itSpawnCfg = ToSplit->GetConfig();
        rpd.itSpawnMatMainCfg = ToSplit->GetMainMaterial()->GetConfig();
        craftcore::SendToHellSafely(ToSplit);

        ToSplit = Corpse;
        bHumanoidCorpse=true;
      }else{
        craftcore::SendToHellSafely(Corpse);
      }

      rpd.ingredientsIDs.clear();
      rpd.ingredientsIDs.push_back(ToSplit->GetID());

      rpd.itSpawnType = CIT_LUMP;

      rpd.bAlreadyExplained=true; //no need to say anything
    }

    if(ToSplit==NULL && choseOneIngredient<stick>(rpd)){
      ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
      if(!reqCut(rpd,ToSplit->GetName(INDEFINITE)))
        return false;
      rpd.itSpawnType = CIT_STICK;
    }

    if(ToSplit==NULL){
      ci CI;
      CI.bAllowMeltables=false;
      if(choseOneIngredient<stone>(rpd,&CI)){
        ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
        if(!setTools(rpd,FindBluntTool(rpd),findCarvingTool(rpd,ToSplit))){ //the blunt is to hit the cutting one
          explain(rpd,ToSplit->GetName(INDEFINITE));
          return false;
        }
        rpd.itSpawnType = CIT_STONE;
      }
    }

    if(ToSplit==NULL){
      rpd.bAlreadyExplained=true; //no need to say anything
      return false;
    }

    /*
    if(craftcore::IsDegraded(ToSplit)){
      rpd.bAlreadyExplained=true;
      return false;
    }
    */

    if(ToSplit->GetSecondaryMaterial()!=NULL)
      ABORT("can only split items without secondary material");

    // some items may not have main material like corpses
    long volTot=ToSplit->GetVolume();

    festring fsInfo;
    ToSplit->AddInventoryEntry(rpd.rc.H(),fsInfo,1,true);
    rpd.itSpawnTot = game::NumberQuestion(festring()+"Split "+fsInfo+" in how many parts? [2 or more]", WHITE, true);
    if(rpd.itSpawnTot==1 || rpd.itSpawnTot==0){
      rpd.bAlreadyExplained=true; //no need to say anything
      return false;
    }

    if(rpd.itSpawnTot<0){ //single cut mode
      if(bHumanoidCorpse){
        ADD_MESSAGE("This needs to be split first."); //see 'why' about necromancers above... TODO a better message?
        rpd.bAlreadyExplained=true;
        return false;
      }

      int iCutVol = rpd.itSpawnTot * -1;
      material* matM = ToSplit->GetMainMaterial();
      if(matM==NULL)
        ABORT("main material is null for %s?",ToSplit->GetName(DEFINITE).CStr());
      if((matM->GetVolume() - iCutVol) < 1){
        rpd.bAlreadyExplained=true; //no need to say anything
        return false;
      }

      item* cut = craftcore::PrepareRemains(rpd,matM,craftcore::CitType(ToSplit),iCutVol);
      //cut->GetMainMaterial()->SetVolume(iCutVol);
      matM->SetVolume(matM->GetVolume() - iCutVol);
      ToSplit->CalculateAll();
      rpd.bAlreadyExplained=true; //no need to say anything
      return true; //TODO should take more turns?
    }

    rpd.itSpawnMatMainVol = volTot/rpd.itSpawnTot;
    if(rpd.itSpawnMatMainVol < 1){
      ADD_MESSAGE("The split part must have some volume.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    /**
     * gradative work takes care of remaining vol
     * if not gradative, this little remainder will just be lost (probably only in case of corpses)
     * kept the code active as reference if someone thinks a good way to use that value :)
     */
    long volRest = volTot%rpd.itSpawnTot;

    if(rpd.itSpawnCfg==0)
      rpd.itSpawnCfg = ToSplit->GetConfig();
    if(rpd.itSpawnMatMainCfg==0)
      rpd.itSpawnMatMainCfg = ToSplit->GetMainMaterial()->GetConfig();

    float fTotTurns=0;
    for(int i=1;i<=rpd.itSpawnTot;i++){ //TODO should be based on cut area/length
      float fCurrentPartVol=volTot/(float)i;
      float fTurns=fCurrentPartVol/100000.0; //reference: bear is 150000cm3
      fTotTurns+=fTurns; //turn time is one minute but when fighting it also spends 1 min... w/e...
    }
    rpd.iBaseTurnsToFinish = fTotTurns;
    if(rpd.iBaseTurnsToFinish<1)
      rpd.iBaseTurnsToFinish=1;

    rpd.bAlreadyExplained=true; //no need to say anything

    rpd.bCanStart = true;

    return true;
  }
};srpSplitLump rpSplitLump;

struct srpForgeItem : public recipe{
  virtual void fillInfo(){
    init("create","an item");
    desc << "Try to craft an item.\n"
    "You will need an appropriate tool. Normally a hammer or a dagger are used, "
    "but other blunt and cutting weapons can serve as well. To work with metals, "
    "you need to have ingots prepared and to be standing near a forge and an anvil. "
    "Working on a workbench will be faster.\n"
    "Ingots and stick are always fully used up, while stones can be used up partially. "
    "Any remaining materials will become lumps.";
  }

  /**
   * IMPORTANT!!!
   *
   * dont use difficulty to calc/increase turns as it is already a per-turn check
   */
  void applyDifficulty(recipedata& rpd,item* itSpawn){
    //TODO are these difficulties below well balanced (not cheap neither excessive/notFun)?
    whistle* itWist = dynamic_cast<whistle*>(itSpawn);
    if(itWist){
      rpd.fDifficulty = 3.5;
      rpd.bSpecialExtendedTurns=true;
    }

    key* itKey = dynamic_cast<key*>(itSpawn);
    if(itKey!=NULL){ //TODO are these difficulties below good?
      rpd.fDifficulty = 3.0;
      if(itKey->CanOpenLockType(HEXAGONAL_LOCK))
        rpd.fDifficulty = 4.0;
      if(itKey->CanOpenLockType(OCTAGONAL_LOCK))
        rpd.fDifficulty = 4.0;
      if(itKey->CanOpenLockType(HEART_SHAPED_LOCK))
        rpd.fDifficulty = 5.0;

      rpd.bSpecialExtendedTurns=true;
    }

    stethoscope* itSte = dynamic_cast<stethoscope*>(itSpawn);
    if(itSte){
      rpd.fDifficulty = 3.5;
      rpd.bSpecialExtendedTurns=true;
    }

    meleeweapon* itMW = dynamic_cast<meleeweapon*>(itSpawn);
    if(itMW!=NULL){ //TODO are these difficulties below good?
      rpd.fDifficulty = 2.0;
      if(itMW->IsTwoHanded())
        rpd.fDifficulty = 3.5;
    }

    shield* itSH = dynamic_cast<shield*>(itSpawn);
    if(itSH!=NULL){
      rpd.fDifficulty = 2.5;
    }

    armor* itAR = dynamic_cast<armor*>(itSpawn);
    if(itAR!=NULL){
      rpd.fDifficulty = 1.5;
      bodyarmor* itBAR = dynamic_cast<bodyarmor*>(itSpawn);
      if(itBAR!=NULL)
        rpd.fDifficulty = 3.0;
    }
  }

  virtual bool work(recipedata& rpd){DBGLN;
    // let user type the item name
    static festring Default; //static to help on reusing! like creating more of the same
    item* itSpawn;

    for(;;){
      festring Temp;
      Temp << Default;DBG4(Default.CStr(),Default.GetSize(),Temp.CStr(),Temp.GetSize()); // to let us fix previous instead of having to fully type it again

      if(game::DefaultQuestion(Temp, CONST_S("What do you want to try to create?"), Default, true) == ABORTED){DBGLN;
        /**
         * The wishing system will try to guess what item matches whatever we type,
         * so it may not be what we typed exactly...
         */
        break;
      }DBG1(Temp.CStr());

      itSpawn = protosystem::CreateItemToCraft(Temp);DBGLN;

      if(itSpawn){DBGLN;
        if(craftcore::canBeCrafted(itSpawn)){DBG4("SendingToHellRejectedCraftItem",itSpawn->GetID(),itSpawn->GetNameSingular().CStr(),itSpawn);
          rpd.itSpawnType = CIT_PROTOTYPE;
          rpd.fsItemSpawnSearchPrototype = Temp;
          break;
        }else{
          festring fsDoWhat = "You don't have the power required to enchant";
          if(itSpawn->GetCategory()==FOOD)fsDoWhat="You never learned how to cook"; //TODO sounds weird for kiwi/banana TODO could at least roast stuff TODO needs seeds to create flour and bake breads
          if(itSpawn->GetCategory()==POTION)fsDoWhat="You never learned how to make";
          if(itSpawn->GetCategory()==BOOK)fsDoWhat="You don't have time to write";
          if(itSpawn->GetCategory()==MISC)fsDoWhat="You are overwhelmed by the complexity of";
          ADD_MESSAGE("%s %s!",fsDoWhat.CStr(),itSpawn->GetName(INDEFINITE).CStr()); //itCreate->GetNameSingular());//
          craftcore::SendToHellSafely(itSpawn);
          itSpawn = NULL; //IMPORTANT!!! if user press ESC...
        }
      }else{
        ADD_MESSAGE("You don't know how to create %s.",Temp.CStr());
      }

      Default.Empty();DBG1(Default.CStr());
      Default << Temp;
    }

    if(itSpawn==NULL){
      rpd.bAlreadyExplained=true; //actually was just cancelled by user
      return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /* This message ends up being rather confusing.
    if(Default == itSpawn->GetName(INDEFINITE)) //TODO compare for EQUAL ignoring case
      ADD_MESSAGE("Now you need the materials to create a %s.",itSpawn->GetName(INDEFINITE).CStr());
    else
      ADD_MESSAGE("Now you need the materials to create a %s as you would probably create %s.",Default.CStr(),itSpawn->GetName(INDEFINITE).CStr());
    */

    long lVolM = itSpawn->GetMainMaterial()->GetVolume();
    if(lVolM==0)
      ABORT("main material 0 volume??? %s",itSpawn->GetName(DEFINITE).CStr());

    long lVolS = 0;
    if(itSpawn->GetSecondaryMaterial()!=NULL){
      lVolS = itSpawn->GetSecondaryMaterial()->GetVolume();
      if(lVolS==0)
        ABORT("secondary material set with 0 volume??? %s",itSpawn->GetName(DEFINITE).CStr());
    }

    DBG2(lVolM,lVolS);
    int iCfgM=-1;
    int iCfgS=-1;

    ci CIM;
    CIM.bMainMaterRemainsBecomeLump=true;
    ci CIS;
    CIS.bMainMaterRemainsBecomeLump=true;

    // some material constraints/limitations
    if(dynamic_cast<potion*>(itSpawn)!=NULL){
      CIM.iReqMatCfgMain=GLASS; //TODO there are other materials that can hold sulphuric acid?
    }

    bool bIsItemContainer = dynamic_cast<itemcontainer*>(itSpawn) !=NULL;
    bool bIsWeapon = itSpawn->IsWeapon(rpd.rc.H());
    
    bool bMainMatOk = false;
    bool bMustTailor = (bIsWeapon && dynamic_cast<whip*>(itSpawn)) || dynamic_cast<cloak*>(itSpawn);
    bool bCanTailor = dynamic_cast<armor*>(itSpawn) && !itSpawn->IsHelmet(rpd.rc.H());
    if(bMustTailor || bCanTailor){ // tailoring
      festring fsM("as MAIN material (cloth "); // only main can be cloth
      float fPerc = 0.85;
      fsM<<(int)(fPerc*100)<<"%)";
      /**
       * cloth will be cut and sewed
       */
      if(!bMainMatOk){
        ci CI = CIM;
        CI.fUsablePercVol=fPerc;
        CI.bMustBeTailorable = rpd.bTailoringMode = true;
        CI.bMixRemainingLump = false;
        bMainMatOk = choseIngredients<lump>(fsM,lVolM, rpd, iCfgM, CI); //TODO instead of <lump> should be a new item with new graphics called <cloth>
      }
    }
    if(!bMustTailor){
      // crafting with stones or ingots
      if(!bMainMatOk){
        ci CI = CIM;
        CI.iReqCfg=INGOT; //meltables are 100% usable
        festring fsM("as MAIN material (ingots 100%)");
        bMainMatOk = choseIngredients<stone>(fsM,lVolM, rpd, iCfgM, CI);
      }
      
      if(!bMainMatOk){
        ci CI = CIM;
        CI.bFirstItemMustHaveFullVolumeRequired=true; //carving: only one ingredient piece per material allowed, so it must have required volume
        CI.bMultSelect=false;
        CI.fUsablePercVol=0.75;
        festring fsM("as MAIN material (stones "); //roundy shape loses material
        fsM<<(int)(CI.fUsablePercVol*100)<<"%)";
        bMainMatOk = choseIngredients<stone>(fsM,lVolM, rpd, iCfgM, CI);
      }
      
      // crafting with sticks and bones
      if(!bMainMatOk){ 
        festring fsM("as MAIN material (sticks/bones ");
        float fPerc = bIsItemContainer ? 1.0 : 0.5;
        fsM<<(int)(fPerc*100)<<"%)";
        /**
         * stick shape can't provide enough to the required dimensions (this is a xtremely wild simplification btw :))
         * so, this will require twice as much sticks if not a container, to be crafted ex.: 35/0.5=70
         */
        if(!bMainMatOk){
          ci CI = CIM;
          CI.fUsablePercVol=fPerc;
          CI.bFirstItemMustHaveFullVolumeRequired=true; //carving: only one ingredient piece per material allowed, so it must have required volume
          bMainMatOk = choseIngredients<bone>(fsM,lVolM, rpd, iCfgM, CI);
        }
        if(!bMainMatOk){
          ci CI = CIM;
          CI.fUsablePercVol=fPerc;
          if(!bIsItemContainer)
            CI.bFirstItemMustHaveFullVolumeRequired=true; //carving: only one ingredient piece per material allowed, so it must have required volume
          CI.bFirstMainMaterIsFilter=false; //wooden things are cheap (resistances, strength etc), so getting mixed into weakest will cause no trouble like losing good meltables (as they arent even), so let user chose any wood
          bMainMatOk = choseIngredients<stick>(fsM,lVolM, rpd, iCfgM, CI);
        }
      }
    }
    
    if(!bMainMatOk){
      ADD_MESSAGE("You don't have the materials to craft a %s.", Default.CStr());
      rpd.bAlreadyExplained=true;
      craftcore::SendToHellSafely(itSpawn);
      return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    bool bContainerEmptied = craftcore::EmptyContentsIfPossible(rpd,itSpawn);

    /**
     * TODO problem: basically sulphuric acid can already be stored on a metal can ...
     * TODO every materialcontainer should rust depending on it's contents, if made of anything else than glass
     * Keeping allowing creating materialcontainer of non glass because the fix is already required for the existing metal can,
     * so preventing it would still not fix how 'metal can' works...
     */

    bool bReqS = bIsWeapon;
    bool bAllowS = true;
//    if(mc)bAllowS=false;
    if(bContainerEmptied)bAllowS=false;
    if(lVolS==0)bAllowS=false;
    if(bAllowS){DBGLN;
      bool bSecondMatOk = false;
      festring fsS("as Secondary material");DBGLN;
      if(!bSecondMatOk){
        ci CI=CIS;
        CI.iReqCfg=INGOT;
        bSecondMatOk = choseIngredients<stone>(fsS,lVolS, rpd, iCfgS, CI);
      }
      if(!bSecondMatOk){
        ci CI=CIS; //carving: only one stone per material allowed, so it must have required volume
        CI.bFirstItemMustHaveFullVolumeRequired=true;
        CI.bMultSelect=false;
        bSecondMatOk = choseIngredients<stone>(fsS,lVolS, rpd, iCfgS, CI);
      }
      if(bIsWeapon){DBGLN; //this is mainly to prevent "material containers" being filled with non-sense materials like a bottle fille with wood... TODO powders one day would be ok
        if(!bSecondMatOk){
          ci CI=CIS;
          bSecondMatOk = choseIngredients<bone>(fsS,lVolS, rpd, iCfgS, CI);
        }
        if(!bSecondMatOk){
          ci CI=CIS;
          bSecondMatOk = choseIngredients<stick>(fsS,lVolS, rpd, iCfgS, CI);
        }
      }

      if(!bSecondMatOk){
        ADD_MESSAGE("You will craft it later...");
        rpd.bAlreadyExplained=true;
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }

    if(bReqS && !bAllowS)
      ABORT("item reqs secondary mat but doesnt allow it??? %s",itSpawn->GetName(DEFINITE).CStr());
    
    if(rpd.bTailoringMode){
      long lVolSewing = lVolM/100;
      if(lVolSewing==0)lVolSewing=1;
      int iSCfg=-1;
      ci CISW;
      CISW.bMainMaterRemainsBecomeLump=true;
      CISW.bMixRemainingLump = false;
      CISW.iReqMatCfgMain=SPIDER_SILK;
      if(!choseIngredients<lump>(cfestring("as sewing material"),lVolSewing,rpd,iSCfg,CISW)){ //TODO instead of <lump> should be <sewingthread> with new graphics
        ADD_MESSAGE("You don't have enough sewing thread...");
        rpd.bAlreadyExplained=true;
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }
    
    rpd.bHasAllIngredients=true;

    rpd.bCanBeBroken = itSpawn->CanBeBroken();

    delete itSpawn->SetMainMaterial(material::MakeMaterial(iCfgM,lVolM));
    if(bAllowS)
      delete itSpawn->SetSecondaryMaterial(material::MakeMaterial(iCfgS,lVolS));

    material* matM = itSpawn->GetMainMaterial();
    material* matS = itSpawn->GetSecondaryMaterial();
    bool bMeltableM = craftcore::IsMeltable(matM);
    bool bMeltableS = (matS!=NULL && craftcore::IsMeltable(matS));
    rpd.bMeltable = bMeltableM || bMeltableS;

    /**
     * ex.:
     * working with valpurium will take much longer,
     * and the longer it takes the more are the checks to determine if it will fumble and break.
     */

    float fMult=10;//hammering to form it takes time even if the volume is low.
    rpd.iBaseTurnsToFinish=calcMeltableTurns(matM,fMult); DBG4(rpd.iBaseTurnsToFinish,matM->GetName(DEFINITE).CStr(),matM->GetConfig(),matM->GetVolume());
    if(bAllowS && matS!=NULL){
      rpd.iBaseTurnsToFinish+=calcMeltableTurns(matS,fMult);
      DBG4(rpd.iBaseTurnsToFinish,matS->GetName(DEFINITE).CStr(),matS->GetConfig(),matS->GetVolume());
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    if(rpd.bMeltable){
      if(!recipe::findOLT(rpd,FORGE,true)){
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }

      if(!recipe::findOLT(rpd,ANVIL)){ //must be near the anvil to use it!!!
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }
    
    if(rpd.bTailoringMode){
      if(!recipe::findOLT(rpd,WORK_BENCH)){ //must be near it //TODO should be a new bench called TAILORING_BENCH with new graphics one day...
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    applyDifficulty(rpd,itSpawn);

    if(!itSpawn->CanBeBroken() || !itSpawn->CanBeBurned())
      rpd.bSpecialExtendedTurns=true;

    if(rpd.bSpecialExtendedTurns){DBGLN;
      /**
       * special extra turns
       * for special items
       */
      int iBaseTTF = rpd.iBaseTurnsToFinish;

      rpd.iBaseTurnsToFinish += iBaseTTF*10;

      if(!rpd.bMeltable) //carving takes longer
        rpd.iBaseTurnsToFinish *= 1.25;
    }

    /************************************************************************************************
     * this sector OVERRIDES all turns calculations from above if necessary,
     * while keeping all other prepared data.
     * These things, mainly by their volume, wont fit well in the complex turns calcs above.
     * Consider it like ingots/sticks/planks/bones/etc are ready just need to be joined.
     ****************************************************************************************/
    itemcontainer* ic = dynamic_cast<itemcontainer*>(itSpawn);
    if(ic!=NULL){ DBGLN;
      /**
       * Reference is: (from current .dat values)
       * - large biggest stg. volume
            StorageVolume = 1000000;
            DamageDivider = 4;
            StrengthModifier = 200;
       * - strongbox biggest damage divider.
            StorageVolume = 5000;
            DamageDivider = 5;
            StrengthModifier = 150;
       * - small chest (weakest)
            StorageVolume = 10000;
            DamageDivider = 2;
            StrengthModifier = 100;
       */

      int iMaxTurns=60*3; //for large chest reference 3h

      /**
       * transform large chest field values into max turns.
       */
      float SVolRef=1000000;
      float DDivRef=4;
      float StrModRef=200;

      float SVolRatio = ic->GetStorageVolume()/SVolRef; //large is 10x normal chest, strongbox is 1/2 small chest
      float DDivRatio = ic->GetDamageDivider()/DDivRef; //strongbox has biggest value
      float StrModRatio = ic->GetStrengthModifier()/StrModRef; //strongbox equals to normal chest

      float SVolW=1.0;
      float DDivW=3.0;
      float StrMW=2.0;
      float W = SVolW+DDivW+StrMW;

      float Ratio = (SVolW*SVolRatio + DDivW*DDivRatio + StrMW*StrModRatio)/W; //div by total weight of summed ratios

      rpd.iBaseTurnsToFinish = iMaxTurns * Ratio; DBG3(rpd.iBaseTurnsToFinish,ic->GetNameSingular().CStr(),ic->GetAdjective().CStr());
    }

    /// TOOLS ///////////////////////////////////////////////////////////////////////////////////////////////
    // HAMMER like for meltables (still hot and easy to work, any hammer will do)
    //TODO glass should require proper tools (don't know what but sure not a hammer)
    bool bMissingTools=false;
    if(rpd.bTailoringMode){
      rpd.itTool = findTailoringTool(rpd,itSpawn); // only main material can be tailored
      if(rpd.itTool==NULL)
        bMissingTools=true;
      
      if(!bMissingTools){
        if(bMeltableS){
          rpd.itTool2 = FindBluntTool(rpd);
          if(rpd.itTool2==NULL)
            bMissingTools=true;
        }else{
          rpd.itTool2 = findCarvingTool(rpd,itSpawn);
          if(rpd.itTool2==NULL)
            bMissingTools=true;
        }
      }
    }else{ 
      if(bMeltableM){ 
        rpd.itTool = FindBluntTool(rpd); 
        if(rpd.itTool==NULL)
          bMissingTools=true;
      }else{
        rpd.itTool = findCarvingTool(rpd,itSpawn);
        if(rpd.itTool==NULL)
          bMissingTools=true;
      }

      if(!bMissingTools){
        if(bMeltableS){ 
          rpd.itTool2 = FindBluntTool(rpd);
          if(rpd.itTool2==NULL)
            bMissingTools=true;
        }else{
          rpd.itTool2 = findCarvingTool(rpd,itSpawn);
          if(rpd.itTool2==NULL)
            bMissingTools=true;
        }
      }
    }
    
    if(!bMissingTools){
      if(rpd.itTool2==rpd.itTool)
        rpd.itTool2=NULL;
    }

    DBG1(rpd.iBaseTurnsToFinish);

    if(bMissingTools){
      rpd.itTool=rpd.itTool2=NULL; //to make it easy to check/inform, wont start if missing one anyway
      failToolMsg(rpd,"tool");
      craftcore::SendToHellSafely(itSpawn);
      return false;
    }

    /// finishing ///////////////////////////////////////////////////////////////////////////////////////////////
    rpd.CopySpawnItemCfgFrom(itSpawn);
    craftcore::SendToHellSafely(itSpawn);

    rpd.bCanStart=true;

    return true;
  }
};srpForgeItem rpForgeItem;

struct srpFluidsBASE : public recipe{
 protected:
  int iAddVolMin;
  int iAddVolExtra;
  festring fsTool;
  festring fsCorpse;
  festring fsBottle;
  int iLiqCfg;
  int iMatEff;

  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){return false;}

 public:
  srpFluidsBASE(){
    //TODO call super class constructor?

    iAddVolMin = 25;
    iAddVolExtra = 75;
    fsTool="dagger";
    fsCorpse="creature corpse";
    fsBottle="bottle";
    iLiqCfg = -1;
    iMatEff = -1;
  }

  virtual bool work(recipedata& rpd){
    //extract fluids (not blood as it can be used as nutrition right? *eww* :P)

    itemvector vi;

    ///////////// tool ////////////////
    rpd.itTool = FindTool(rpd, DAGGER);
    if(rpd.itTool==NULL){
      failToolMsg(rpd,"dagger");
      return false;
    }

    item* itCorpse=NULL;
    vi = vitInv(rpd);
    for(int i=0;i<vi.size();i++){
      corpse* Corpse = dynamic_cast<corpse*>(vi[i]);
      if(Corpse!=NULL){
        character* D = Corpse->GetDeceased(); DBG2(Corpse->GetName(DEFINITE).CStr(),D->GetName(DEFINITE).CStr())
        static const materialdatabase* blood;blood = material::GetDataBase(D->GetBloodMaterial());
        static const materialdatabase* flesh;flesh = material::GetDataBase(D->GetFleshMaterial());

        if(chkCorpse(blood,flesh)){DBGLN;
          itCorpse=Corpse;
          break;
        }
      }
    }DBGLN;

    if(itCorpse==NULL){
      ADD_MESSAGE("No useful corpse to work with.");
      rpd.bAlreadyExplained=true;
      return false;
    }


    //TODO extract poison glands as a new item (so a new recipe to create it) to be used here instead of the corpse?
    item* itBottle=NULL;
    material* mat = NULL;
    long currentVolume=0;

    // look for compatible bottle first
    vi = vitInv(rpd);
    for(int i=0;i<vi.size();i++){DBGLN;
      potion* pot = dynamic_cast<potion*>(vi[i]);
      if(pot!=NULL){DBGLN;
        mat = pot->GetSecondaryMaterial();
        if(mat==NULL)continue;

        bool bChkVol=false;DBG3(mat->GetEffect(),iMatEff,iLiqCfg);
        if(iMatEff>-1){
          if(mat->GetEffect()==iMatEff) //TODO should be more precise like the material actually be the same poison of that kind of spider...
            bChkVol=true;
        }else{
          if(mat->GetConfig()==iLiqCfg)
            bChkVol=true;
        }

        if(bChkVol){DBGLN;
          long vol = mat->GetVolume();
          if(vol < pot->GetDefaultSecondaryVolume()){ //less than max
            itBottle = pot;
            currentVolume = vol;
            break;
          }
        }
      }
    }DBGLN;

    // look for empty bottle
    if(itBottle==NULL){DBGLN;
      vi = vitInv(rpd);
      for(int i=0;i<vi.size();i++){
        potion* pot = dynamic_cast<potion*>(vi[i]);
        if(pot!=NULL){DBGLN;
          mat = pot->GetSecondaryMaterial();
          if(mat==NULL){ //empty
            itBottle = pot;
            break;
          }
        }
      }
    }DBGLN;

    if(itBottle==NULL){
      ADD_MESSAGE("No bottle available.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    // ready
    int iAddVolume = +iAddVolMin +(clock()%iAddVolExtra);
    int volume = currentVolume + iAddVolume;

    if(volume > itBottle->GetDefaultSecondaryVolume())
      volume = itBottle->GetDefaultSecondaryVolume();

    /***
     * TODO
     * mmm seems to have no strengh diff?
     * only takes more time if "stronger" like "not from large spider"
    mat = itBottle->GetSecondaryMaterial();
    if(mat!=NULL)mat->GetEffectStrength(); //TODO could average current poison strengh in some way if ever one day
    */

    rpd.itSpawnType = CIT_POTION;
    rpd.itSpawnCfg = itBottle->GetConfig(); //may be a vial
    rpd.itSpawnMatMainCfg = itBottle->GetMainMaterial()->GetConfig();
    rpd.itSpawnMatMainVol = itBottle->GetMainMaterial()->GetVolume();
    rpd.itSpawnMatSecCfg = iLiqCfg;DBG1(iLiqCfg);
    rpd.itSpawnMatSecVol = volume;
//    rpd.itSpawn = potion::Spawn(itBottle->GetConfig()); //may be a vial
//    delete rpd.itSpawn->SetSecondaryMaterial(liquid::Spawn(iLiqCfg, volume));
//    rpd.SetSpawnItemCfg(rpd.itSpawn);

    rpd.ingredientsIDs.push_back(itBottle->GetID()); //just to be destroyed too if crafting completes
    rpd.ingredientsIDs.push_back(itCorpse->GetID());
    rpd.bHasAllIngredients=true;

    rpd.iBaseTurnsToFinish=5;

    rpd.bCanStart=true;

    return true;
  }
};

struct srpPoison : public srpFluidsBASE{

  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Effect==EFFECT_POISON || flesh->Effect==EFFECT_POISON);
  }

  virtual void fillInfo(){
    init("extract","poison");
    desc << "Use a " << fsTool << " to " << action << " " << name << " from a poisonous corpse into a " << fsBottle << ".";
  }

  virtual bool work(recipedata& rpd){
    iLiqCfg=POISON_LIQUID;
    iMatEff=EFFECT_POISON;

    return srpFluidsBASE::work(rpd);
  }
};srpPoison rpPoison;

struct srpAcid : public srpFluidsBASE{
  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Acidicity)>0 || (flesh->Acidicity)>0;
  }

  virtual void fillInfo(){
    init("extract","acid");
    desc << "Use a " << fsTool << " to " << action << " " << name << " from an acidic corpse into a " << fsBottle << ".";
  }

  virtual bool work(recipedata& rpd){
    iLiqCfg=SULPHURIC_ACID;

    return srpFluidsBASE::work(rpd);
  }
};srpAcid rpAcid;

struct srpMagic : public srpFluidsBASE{
  virtual bool chkCorpse(const materialdatabase* blood, const materialdatabase* flesh){
    return (blood->Effect == EFFECT_MUSHROOM || flesh->Effect == EFFECT_MUSHROOM ||
            blood->Effect == EFFECT_MAGIC_MUSHROOM || flesh->Effect == EFFECT_MAGIC_MUSHROOM);
  }

  virtual void fillInfo(){
    init("extract","raw magic");
    desc << "Use a " << fsTool << " to " << action << " raw liquefied magic from a mushroom into a " << fsBottle <<  ".";
  }

  virtual bool work(recipedata& rpd){
    iLiqCfg=MAGIC_LIQUID;

    return srpFluidsBASE::work(rpd);
  }
};srpMagic rpMagic;

felist craftRecipes(CONST_S("What do you want to craft?"));
std::vector<recipe*> vrpAllRecipes;

void updateCraftDesc(){
  craftRecipes.EmptyDescription();

  float fSkill=craftcore::CraftSkill(PLAYER); //TODO should this dynamic value show too where stats are?
  festring fsSkill="Crafting Proficiency: ";  // It's actually different from skills, so don't call it a skill.
  static char cSkill[20];
  sprintf(cSkill, "%.1f",fSkill);
  fsSkill<<cSkill;

  festring fsDesc=fsSkill;
  if(vSuspended.size()>0)
    fsDesc<<" (Suspended Actions: "<<vSuspended.size()<<")";

  craftRecipes.AddDescription(fsDesc,fSkill<10?RED:WHITE);
}

void addRecipe(recipe* prp){
  prp->iListIndex=vrpAllRecipes.size()+1; //+1 is about the 1st entry related to suspended actions

  if(prp->name.IsEmpty())
    ABORT("empty recipe name '%s' '%s' %d",prp->name.CStr(),prp->desc.CStr(),prp->iListIndex);

  craftRecipes.AddEntry(festring()+prp->action+" "+prp->name, LIGHT_GRAY, 20, prp->iListIndex, true); DBG2(prp->name.CStr(),prp->iListIndex);
  craftRecipes.SetLastEntryHelp(prp->desc);

  vrpAllRecipes.push_back(prp);
}
void addMissingMsg(festring& where, cfestring& what){
  if(where.GetSize()>0)
    where<<", ";
  where<<what;
}
/**
 * Dear developer, be sure to read the ABORT message before adding a recipe! :)
 */
truth commandsystem::Craft(character* Char) //TODO currently this is an over simplified crafting system... should be easy to add recipes and show their formulas...
{
  return craftcore::Craft(Char);
}
truth craftcore::CheckArms(humanoid* h)
{
  bool bLOk,bROk; //dummy
  return CheckArms(h,bLOk,bROk);
}
truth craftcore::CheckArms(humanoid* h,bool& bLOk,bool& bROk)
{
  bLOk = h->GetLeftArm() !=NULL && h->GetLeftArm()->IsUsable();
  bROk = h->GetRightArm()!=NULL && h->GetRightArm()->IsUsable();
  if(!bLOk && !bROk){
    ADD_MESSAGE("You need at least one usable arm to be able to craft.");
    return false;
  }

  /**
   * TODO
   * resuming a suspended,
   * if begun with 1 arm now have 2, should lower remaining turns to speed up
   * if begun with 2 arms now have 1, should increase remaining turns
   * also, changes on crafting skill (based on stats) should also dynamically affect remaining turns
   * !!!BEWARE!!! calculations depending on remaining turns may break!!!
   */

  return true;
}
humanoid* craftcore::CheckHumanoid(character* Char)
{
  humanoid* h = dynamic_cast<humanoid*>(Char);
  if(h==NULL){
    ADD_MESSAGE("This monster cannot craft.");
    return NULL;
  }
  return h;
}

recipe* CheckSelectedAndInitRecipeIfNeeded(bool bInitRecipes,recipedata& rpd,recipe* prp, recipe& rp){ // to avoid macro hard readability
  if(prp==NULL && rp.IsTheSelectedOne(rpd))prp=&rp; \
  DBG3(prp,&rp,rp.desc.CStr()); \
  if(bInitRecipes)addRecipe((recipe*)&rp);
  return prp;
}

truth craftcore::Craft(character* Char) //TODO currently this is an over simplified crafting system... should be easy to add recipes and show their formulas...
{
  humanoid* h = CheckHumanoid(Char);
  if(h==NULL)
    return false;

  bool bLOk,bROk;
  if(!CheckArms(h,bLOk,bROk))
    return false;

  uint sel = FELIST_ERROR_BIT;
  if(vrpAllRecipes.size()>0){
    game::SetStandardListAttributes(craftRecipes);
    craftRecipes.AddFlags(SELECTABLE);
    craftRecipes.ClearFilter();
    updateCraftDesc();
    sel = craftRecipes.Draw(); DBG1(sel);

    if(sel & FELIST_ERROR_BIT)
      return false;

    if(sel==0 && craftcore::HasSuspended()){
      int key = game::KeyQuestion(CONST_S("There are suspended crafting actions: (r)esume/ENTER or (c)ancel?"),
        KEY_ESC, 3, 'r', 'c', KEY_ENTER);
      if(key==KEY_ESC)return false;

      if(key==KEY_ENTER)key='r';

      felist LSusp("Suspended crafting actions:",WHITE);
      game::SetStandardListAttributes(LSusp);
      LSusp.AddFlags(SELECTABLE);
      for(int i=0;i<vSuspended.size();i++)
        LSusp.AddEntry(vSuspended[i].fsCraftInfo,LIGHT_GRAY);

      festring fsDo;
      bool bResume=false;
      col16 col=WHITE;
      switch(key){
      case 'r':
        fsDo="Resume";
        bResume=true;
        break;
      case 'c':
        fsDo="Cancel (cannot be restored after this)"; //TODO could...
        col=RED;
        bResume=false;
        break;
      default:
        return false; //TODO ever reached?
      }

      fsDo<<" which crafting action?";
      LSusp.AddDescription(fsDo,col);
      uint Sel = LSusp.Draw();
      if(Sel<vSuspended.size()){ //TODO is necessary to check esc key, error bit etc?
        bool bErase=true;
        if(bResume)
          bErase = craftcore::ResumeSuspendedTo(Char,vSuspended[Sel]);
        if(bErase)
          vSuspended.erase(vSuspended.begin()+Sel);
        return bResume;
      }
      return false;
    }

  }
  recipedata rpd(h,sel);

  /******************************************************************************************
   * 1st call it just initializes the recipes list after all recipes have been configured!
   */
  bool bInitRecipes = vrpAllRecipes.size()==0;

  if(bInitRecipes){
    craftRecipes.AddEntry(festring()+"Suspended crafting actions", LIGHT_GRAY, 20, 0, true);
    craftRecipes.SetLastEntryHelp("Resume or remove already started crafting actions.");
  }

  // these are kind of grouped and not ordered like a-z
  recipe* prp=NULL;
  #define RP(MACRO_PARAM_rp) prp=CheckSelectedAndInitRecipeIfNeeded(bInitRecipes,rpd,prp,MACRO_PARAM_rp); // just a simple macro to easify maintenance

  if(bInitRecipes)craftRecipes.AddEntry(festring()+"Construction:", DARK_GRAY, 0, NO_IMAGE, false);
  RP(rpAnvil);
  RP(rpChair);
  RP(rpDoor);
  RP(rpForge);
  RP(rpWall2);
  RP(rpWorkBench);

  if(bInitRecipes)craftRecipes.AddEntry(festring()+"Alchemy:", DARK_GRAY, 0, NO_IMAGE, false);
  RP(rpAcid);
  RP(rpPoison);
  RP(rpMagic);

  if(bInitRecipes)craftRecipes.AddEntry(festring()+"Material crafting:", DARK_GRAY, 0, NO_IMAGE, false);
  RP(rpDismantle);
  RP(rpSplitLump);
  RP(rpJoinLumps);
  RP(rpResistanceVS);
  RP(rpInspect);

  if(bInitRecipes)craftRecipes.AddEntry(festring()+"Item crafting:", DARK_GRAY, 0, NO_IMAGE, false);
  RP(rpForgeItem);
  RP(rpMelt);

  if(bInitRecipes)craftRecipes.AddEntry(festring()+"Tailoring:", DARK_GRAY, 0, NO_IMAGE, false);
  RP(rpCutWeb);

  if(bInitRecipes)
    return Craft(Char); //init recipes descriptions at least, one time recursion and returns here :>

  if(prp==NULL){DBGLN;
    return false;
  }DBGLN;

  //ADD_MESSAGE("Your chosen crafting action is to %s %s.",prp->action.CStr(),prp->name.CStr());
  bool bDummy = prp->work(rpd); //bDummy(fied) as there is more detailed fail status from rpd bools

  //TODO these messages are generic, therefore dont look good... improve it
  if(rpd.bCanStart){DBGLN;
    if(rpd.ingredientsIDs.size()==0)
      ABORT("no ingredients chosen?");

    festring fsTools;
    if(rpd.itTool!=NULL)
      fsTools=rpd.itTool->GetName(INDEFINITE);
    if(rpd.itTool2!=NULL){
      if(!fsTools.IsEmpty())
        fsTools<<" and ";
      fsTools<<rpd.itTool2->GetName(INDEFINITE);
    }
    if(!fsTools.IsEmpty())
      ADD_MESSAGE("You will use %s as a tool.",fsTools.CStr());

    if(rpd.otSpawnType!=CTT_NONE || rpd.itSpawnType!=CIT_NONE) {
      int iCraftTimeMult=1;

      if(!bLOk || !bROk){ //using only one hand will take more time even if only one tool is required as even if 2 were, only 1 would be handled per time
        ADD_MESSAGE("You only have one arm, this will take longer.");
        iCraftTimeMult++;
      }

      if(rpd.itTool !=NULL && rpd.itTool ->IsBroken()){
        ADD_MESSAGE("The first tool is broken, this will take longer.");
        iCraftTimeMult++;
      }
      if(rpd.itTool2!=NULL && rpd.itTool2->IsBroken()){
        ADD_MESSAGE("The second tool is broken, this will take longer.");
        iCraftTimeMult++;
      }

      if(rpd.iBaseTurnsToFinish<1)
        ABORT("invalid iBaseTurnsToFinish %d",rpd.iBaseTurnsToFinish);

      DBGEXEC( //solved, the problem was the duplicate item code that modifies the duplicated ID ...
        for(int iDbg123=0;iDbg123<rpd.ingredientsIDs.size();iDbg123++){
          item* itDbg123=game::SearchItem(rpd.ingredientsIDs[iDbg123]);
          if(itDbg123==NULL)ABORT("ingredient id %d vanished?",rpd.ingredientsIDs[iDbg123]);
        }
      );DBG1(rpd.iBaseTurnsToFinish);

      rpd.iBaseTurnsToFinish*=iCraftTimeMult;

      /**********************************************************************
       * LAST turn calc thing!!!
       * ex.: initial dex=10 wis=10 is 1.0 means wont modify turns
       **********************************************************************/
      rpd.iBaseTurnsToFinish /= craftcore::CraftSkill(Char)/10.0;
      if(rpd.iBaseTurnsToFinish==0) //if div zeroed it
        rpd.iBaseTurnsToFinish=1;
      if(rpd.iBaseTurnsToFinish<rpd.iMinTurns)
        rpd.iBaseTurnsToFinish=rpd.iMinTurns;
      rpd.iRemainingTurnsToFinish = rpd.iBaseTurnsToFinish;
      // warn if will take too long
      int iH = rpd.iBaseTurnsToFinish/60;
      int iD = iH/24;
      iH = iH%24;
      int iM = rpd.iBaseTurnsToFinish%60;
      if(iH>=1 || iD>1){
        festring fs;
        fs<<"It will take ";
        if(iD>1)
          fs<<iD<<" days "; //this may happen in case the stats/skill went too low, so user has a chance to recover from the debuff
        fs<<iH<<" hours and "<<iM<<" minutes to complete";
        fs<<", Continue? [y/N]";
        if(!game::TruthQuestion(fs))
          return true; // see at the end why
      }

      if(rpd.otSpawnType!=CTT_NONE && rpd.v2PlaceAt.Is0())
        rpd.v2PlaceAt = rpd.lsqrPlaceAt!=NULL ? rpd.lsqrPlaceAt->GetPos() : rpd.lsqrCharPos->GetPos(); //may be ignored anyway, is just a fallback

      rpd.iAddDexterity=5; //TODO crafting difficult things should give more dexterity (wisdom too?)

      rpd.v2PlayerCraftingAt = Char->GetPos();

      if(rpd.itTool!=NULL && rpd.itTool2!=NULL)
        if(rpd.itTool==rpd.itTool2) //keep this check to fix any code bofore this.
          ABORT("both tools are the same item %lu:%s %lu:%s",rpd.itTool->GetID(),rpd.itTool->GetName(INDEFINITE).CStr(),rpd.itTool2->GetID(),rpd.itTool2->GetName(INDEFINITE).CStr());
      if(rpd.itTool !=NULL)rpd.itToolID =rpd.itTool ->GetID();
      if(rpd.itTool2!=NULL)rpd.itTool2ID=rpd.itTool2->GetID();

      rpd.fsCraftInfo =
        prp->action+" "+prp->name+
        (rpd.itSpawnCfg!=0 ? festring(" ("+rpd.fsItemSpawnSearchPrototype+")") : festring())+
        ", started at "+game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex(), true);

      rpd.ClearRefs(); //pointers must be revalidated on the action handler
      DBG1(rpd.dbgInfo().CStr());
      Char->SwitchToCraft(rpd); // everything must be set before this!!!

      ADD_MESSAGE("You will work on %s now.",prp->name.CStr());
    }else{
      ABORT("requested to craft nothing? %s",rpd.dbgInfo().CStr());
    }

    return true; //spends current turn
  }else{
    if(!rpd.bAlreadyExplained)
      ABORT("explain why crafting won't work.");
  }

  /**
   * ATTENTION!!!
   * the (crafting code) complexity of granting one turn wont be lost isnt worth letting the game crash randomly somewhere
   * that is hard to track the source of the problem (mainly because of sendtohell() not being applied if turn is not spent).
   * so to ALWAYS spend current turn, when things sent to hell will properly be applied everywhere required
   * is the SAFEST thing! dont change this please even if you are sure all looks perfect! ;)
   if(rpd.bSpendCurrentTurn)return true;else return false; //old code
   */
  return true;
}

/**
 *
 * @param bAllowBreak
 * @param rpd
 * @param itSpawn
 * @param fsCreated
 * @return beware, can be NULL but still spawn lumps!
 */
item* crafthandle::CheckBreakItem(bool bAllowBreak, recipedata& rpd, item* itSpawn, festring& fsCreated)
{
  bool bBreak = rpd.bSpawnBroken;

  if(bAllowBreak && bBreak && !itSpawn->IsBroken()){
    bool bCanBeBroken = itSpawn->CanBeBroken();
    if(bCanBeBroken){
      /**
       * IMPORTANT!!!
       *
       * can only break after placed somewhere like on player's inv
       *
       * breaking it with
       *   itSpawn->Break(NULL);
       * on the same turn it was spawned will create inconsistent inventory,
       * The last item will point to invalid memory and may segfault anywhere from next turn on,
       * or cause unpredictable results,
       * so do not use it!
       *
       * This below was taken from Break() and seems safe.
       * TODO create a method there like SetSelfAsBroken() to re-use the code to grant they will be in sync
       */
      if(itSpawn->SetConfigIfPossible(rpd.itSpawnCfg | BROKEN)){
        itSpawn->SetSize(itSpawn->GetSize() >> 1);
      }else{
        bCanBeBroken=false; // missing BROKEN config at .dat file but no problem, see below
      }
    }
    
    if(!bCanBeBroken){
      /**
       * things that can't be broken are special.
       * if it can't be broken, will just create a messy lump.
       */
      ADD_MESSAGE("Your lack of skill broke %s into pieces!",itSpawn->GetName(DEFINITE).CStr());
      craftcore::PrepareRemains(rpd,itSpawn->GetMainMaterial(),CIT_LUMP);
      if(itSpawn->GetSecondaryMaterial()!=NULL)
        craftcore::PrepareRemains(rpd,itSpawn->GetSecondaryMaterial(),CIT_LUMP);
      craftcore::SendToHellSafely(itSpawn);
      itSpawn=NULL; //because the result can be 2 item* (2 lumps, one for each material Main and Secondary!)
      fsCreated << "a funny looking lump";
    }
  }

  return itSpawn;
}

material* craftcore::CreateMaterial(material* matCopyFrom)
{
  material* mat = matCopyFrom->Duplicate();

  DBG4(mat->GetSpoilLevel(),matCopyFrom->GetSpoilLevel(),mat->GetRustLevel(),matCopyFrom->GetRustLevel());
  if( //TODO temporary, remove as soon all looks good, faster than scanning the whole code manually to grant it works...
      mat->GetSpoilLevel() != matCopyFrom->GetSpoilLevel() ||
      mat->GetRustLevel() != matCopyFrom->GetRustLevel()
  ) ABORT("material full duplication failed");

//  return CreateMaterial(true/*dummy*/,NULL,matCopyFrom);
  return mat;
}
material* craftcore::CreateMaterial(bool bMain,recipedata& rpd)
{
  material* mat = material::MakeMaterial(
    (bMain ? rpd.itSpawnMatMainCfg : rpd.itSpawnMatSecCfg),
    (bMain ? rpd.itSpawnMatMainVol : rpd.itSpawnMatSecVol)
  );
  return mat;
}

item* crafthandle::SpawnItem(recipedata& rpd, festring& fsCreated)
{
  rpd.rc.integrityCheck();

  item* itSpawn = NULL;
  material* matS = NULL;
  bool bAllowBreak=false;DBG3(rpd.itSpawnType,rpd.itSpawnCfg,rpd.itSpawnMatSecCfg);
  switch(rpd.itSpawnType){
    case CIT_POTION:
      /**
       * IMPORTANT!!!
       * do not use NO_MATERIALS here,
       * apparently the main material is always required for it
       * and the main material would remain uninitialized (instead of NULL TODO fix that?)
       * leading to SEGFAULT when trying to set the main material!
       */
      itSpawn = potion::Spawn(rpd.itSpawnCfg); //may be a vial
      if(rpd.itSpawnMatSecCfg>0)
        matS = liquid::Spawn(rpd.itSpawnMatSecCfg,rpd.itSpawnMatSecVol);
      break;
    case CIT_PROTOTYPE:
      itSpawn = protosystem::CreateItemToCraft(rpd.fsItemSpawnSearchPrototype);

      bAllowBreak=true;
      break;
    case CIT_STONE:
      itSpawn = stone::Spawn(rpd.itSpawnCfg, NO_MATERIALS);
      break;
    case CIT_LUMP:
      itSpawn = lump::Spawn(rpd.itSpawnCfg, NO_MATERIALS);
      break;
    case CIT_STICK:
      itSpawn = stick::Spawn(rpd.itSpawnCfg, NO_MATERIALS);
      break;
  }

  if(itSpawn==NULL)
    ABORT("craft spawned no item.");

  itemcontainer* ic = dynamic_cast<itemcontainer*>(itSpawn);
  if(ic!=NULL){
    if(ic->IsLocked())
      ic->SetIsLocked(false); //to prevent annoyance of not having the key TODO should instead be always a broken lock considering the complexity of the locking system itself?
  }

  if(rpd.itSpawnMatMainCfg==0 || rpd.itSpawnMatMainVol==0)
    ABORT("main material and/or volume is 0 %lu %lu",rpd.itSpawnMatMainCfg,rpd.itSpawnMatMainVol);

//  material* matM = material::MakeMaterial(rpd.itSpawnMatMainCfg,rpd.itSpawnMatMainVol);
//  matM->SetSpoilCounter(rpd.itSpawnMatMainSpoilLevel);
  material* matM = craftcore::CreateMaterial(true,rpd);
  delete itSpawn->SetMainMaterial(matM);

  if(rpd.itSpawnMatSecCfg==0)
    craftcore::EmptyContentsIfPossible(rpd,itSpawn);
  else{
    if(matS==NULL)
      matS = craftcore::CreateMaterial(false,rpd);
//      matS = material::MakeMaterial(rpd.itSpawnMatSecCfg,rpd.itSpawnMatSecVol);
    if(matS!=NULL){
//      matS->SetSpoilCounter(rpd.itSpawnMatSecSpoilLevel);
      delete itSpawn->SetSecondaryMaterial(matS);
    }
  }

  itSpawn->MoveTo(rpd.rc.H()->GetStack());

  if(!fsCreated.IsEmpty())
    fsCreated << " ";
  fsCreated << "You crafted ";

  // this check is more detailed and must be when the item is completely ready!
  if(!craftcore::canBeCrafted(itSpawn)){
    ABORT(
      "Dear developer, for the sake of balance and challenge do not create recipes for:\n"
      "- Quest items.\n"
      "- Magical items as rings, amulets, wands, scrolls, horns etc.\n"
      "Crafting any of this would be unbalanced as hell and unrealistic given your characters upbringing.\n"
      "You're after all a slave, with no knowledge of magic, and crafting magical items should be beyond most craftsmen.\n"
      "This attempt: %s",
      itSpawn->GetName(DEFINITE).CStr()
    );
  }
  
  itSpawn = CheckBreakItem(bAllowBreak, rpd, itSpawn, fsCreated);
  if(itSpawn!=NULL){
    if(fsCreated.GetSize()<200) // this will prevent a crash about "stack smashing detected". TODO to test it and provide a better solution, just comment this `if` line and split a corpse in 40 parts or more
      fsCreated << itSpawn->GetName(INDEFINITE);
    craftcore::FinishSpawning(rpd,itSpawn);
    return itSpawn;
  }
  return NULL;
}

void crafthandle::CraftWorkTurn(recipedata& rpd){ DBG1(rpd.iRemainingTurnsToFinish);
  rpd.iRemainingTurnsToFinish--;
  rpd.bSuccesfullyCompleted = rpd.iRemainingTurnsToFinish==0;

  if(clock()%2==0 ? rpd.iRemainingTurnsToFinish%3==0 : rpd.iRemainingTurnsToFinish%5==0){ // to avoid unnecessarily spamming hiteffects
    // keep this preference order!
    lsquare* lsqrHF=NULL;
    if(!lsqrHF)lsqrHF=rpd.lsqrPlaceAt;
    if(!lsqrHF)lsqrHF=rpd.v2AnvilLocation.Is0()     ? NULL : rpd.rc.H()->GetNearLSquare(rpd.v2AnvilLocation);
    if(!lsqrHF)lsqrHF=rpd.v2WorkbenchLocation.Is0() ? NULL : rpd.rc.H()->GetNearLSquare(rpd.v2WorkbenchLocation);
    if(!lsqrHF)lsqrHF=rpd.v2PlaceAt.Is0()           ? NULL : rpd.rc.H()->GetNearLSquare(rpd.v2PlaceAt);
    if(!lsqrHF)lsqrHF=rpd.v2ForgeLocation.Is0()     ? NULL : rpd.rc.H()->GetNearLSquare(rpd.v2ForgeLocation);
    if(!lsqrHF)lsqrHF=rpd.v2XplodAt.Is0()           ? NULL : rpd.rc.H()->GetNearLSquare(rpd.v2XplodAt);
    if(lsqrHF){
      hiteffectSetup* pHitEff=new hiteffectSetup();

      pHitEff->Type = (rpd.itTool || rpd.itTool2) ? WEAPON_ATTACK : UNARMED_ATTACK;
      pHitEff->WhoHits=rpd.rc.H();
      pHitEff->HitAtSquare=lsqrHF;

      item* itHF=NULL; //keep the below order
      if(!itHF)itHF = rpd.itTool  ? rpd.itTool : NULL;
      if(!itHF)itHF = rpd.itTool2 ? rpd.itTool2 : NULL;
      if(!itHF)itHF = rpd.rc.H()->GetRightArm() ? rpd.rc.H()->GetRightArm() : NULL;
      if(!itHF)itHF = rpd.rc.H()->GetLeftArm()  ? rpd.rc.H()->GetLeftArm() : NULL;
      if(itHF){
        pHitEff->lItemEffectReferenceID = itHF->GetID();
        lsqrHF->AddHitEffect(*pHitEff);
      }
      delete pHitEff;
    }
  }

  if(rpd.bGradativeCraftOverride){
    GradativeCraftOverride(rpd);
  }else{
    if(rpd.bSuccesfullyCompleted)
    {
      festring fsMsg("");

      if(rpd.itSpawnType!=CIT_NONE){DBGLN;
        for(int i=0;i<rpd.itSpawnTot;i++){
          item* itSp = SpawnItem(rpd,fsMsg);
          if(itSp!=NULL) //TODO apply degradation to lumps as the item broke (therefore is NULL)?
            craftcore::CopyDegradationIfPossible(rpd,itSp);
        }
      }

      if(rpd.otSpawnType!=CTT_NONE){DBGLN;
        SpawnTerrain(rpd,fsMsg);
      }

      fsMsg << DestroyIngredients(rpd);
      fsMsg << ".";

      ADD_MESSAGE(fsMsg.CStr());
    }
  }
}

/**
 * Always 1 to many.
 * ex.:
 * - one big meltable lump into ingots
 * - one big stick into tiny ones
 * - one rock into smaller ones
 * - one organic
 */
void crafthandle::GradativeCraftOverride(recipedata& rpd)
{DBGLN;
  if(rpd.ingredientsIDs.size()!=1)
    ABORT("incompatible gradative mode and ingredients count %lu",rpd.ingredientsIDs.size());

  material* matM = game::SearchItem(rpd.ingredientsIDs[0])->GetMainMaterial();DBGLN;
  long matMRemVol = matM->GetVolume();

  /**
   * ex.: tot=20 base=10 remain=9
   * base  rem    rem*tot
   * __  = __ ; x=_____=18 ; tot-x=SpawnNow=2
   * tot   x       base
   */
  float fRemain = 0;
  if(rpd.iBaseTurnsToFinish>1)
    fRemain = (rpd.iRemainingTurnsToFinish*rpd.itSpawnTot)/((float)rpd.iBaseTurnsToFinish);
  int iSpawnNow = rpd.itSpawnTot-fRemain;
  ulong spawnedVol = rpd.itSpawnMatMainVol*iSpawnNow;
  DBG7(iSpawnNow,spawnedVol,matMRemVol,rpd.itSpawnTot,rpd.iBaseTurnsToFinish,rpd.iRemainingTurnsToFinish,fRemain);
  if(iSpawnNow==0)
    return; //holding until required time is met ex.: each spawned requires more than one turn, even if a fraction.

  recipedata rpdTmp = rpd; // a copy to avoid messing it
  if(rpdTmp.itSpawnType==CIT_STICK && rpdTmp.fDifficulty<=1.0)
    rpdTmp.fDifficulty=1.1; //just a little bit difficult to allow fumbling
  for(int i=0;i<iSpawnNow;i++){
    if(rpdTmp.itSpawnType==CIT_STICK)
      CheckFumble(rpdTmp,false);

    festring fsMsg;

    /**
     * IMPORTANT!!! the duplicator would vanish with the item ID that is being duplicated (the original one)
     * so the duplication source item's ID will vanish. TODO could it be safely kept at DuplicateToStack() ?
     * ex.:
    item* itDup = itSpawn->DuplicateToStack(rpd.rc.H()->GetStack());
     * so just avoiding using it here.
     */
    item* itSpawned = SpawnItem(rpdTmp,fsMsg);
    if(itSpawned==NULL)
      ABORT("gradative craft should not spawn things that can be broken %s",rpdTmp.dbgInfo().CStr()); //if broken, will be NULL and the inventory may contain lumps
    craftcore::CopyDegradation(matM,itSpawned->GetMainMaterial());

    ADD_MESSAGE("%s.",fsMsg.CStr());

    if(rpdTmp.itSpawnType==CIT_STICK)
      rpdTmp.bSpawnBroken=false; //reset for next stick fumble check
  }

  rpd.itSpawnTot -= iSpawnNow;
  rpd.iBaseTurnsToFinish = rpd.iRemainingTurnsToFinish; //TODO should a total turns required be always kept?

  if(rpd.itSpawnTot<0)
    ABORT("crafting remaining to spawn went negative %d %s",rpd.itSpawnTot,rpd.dbgInfo().CStr());
  if(rpd.itSpawnTot==0){
    rpd.bSuccesfullyCompleted=true;
    if(rpd.iRemainingTurnsToFinish>0){
      ADD_MESSAGE("You worked well and gained %d minutes!",rpd.iRemainingTurnsToFinish); //TODO despite this actually means a lack of calc precision :P, not less fun tho :)
      rpd.iRemainingTurnsToFinish=0; //overriding for consistency.
    }
  }

  if(spawnedVol==matMRemVol)
    DestroyIngredients(rpd); //cant be left with 0 volume
  else if(spawnedVol>matMRemVol) // Can happen with sol stone, TODO something about it but don't crash.
  {
    ADD_MESSAGE("Your work disappears in a puff of logic!");
    DestroyIngredients(rpd);
  }
  else
    matM->SetVolume(matMRemVol-spawnedVol); //the remaining volume becomes the action control/limit

  matMRemVol=matM->GetVolume();
  DBG7(iSpawnNow,spawnedVol,matMRemVol,rpd.itSpawnTot,rpd.iBaseTurnsToFinish,rpd.iRemainingTurnsToFinish,fRemain);
}

void craftcore::CraftSkillAdvance(recipedata& rpd){
  /**
   * the minimum to advance 1st level on success is at GetLevelMap(1)
   */
  int iAddCraftSkill = rpd.rc.H()->GetCWeaponSkill(CRAFTING)->GetLevelMap(1) * rpd.fDifficulty;
  if(rpd.fDifficulty <= 1.0) iAddCraftSkill /= 10.0; // too easy stuff will learn less
  if(rpd.bSpawnBroken) iAddCraftSkill /= 3.0; // learns something if fumble
  if(iAddCraftSkill<1) iAddCraftSkill=1; // add a minimum
  rpd.rc.H()->GetCWeaponSkill(CRAFTING)->AddHit(iAddCraftSkill);
}

bool craftcore::CheckFumble(recipedata& rpd, bool& bCriticalFumble,int& iFumblePower)
{
  /**
   * To fumble, base reference is 15% chance at a craft skill of 20.
   * ex.: Craft skill of 10 will have 30% fumble chance.
   */
  int iLuckPerc=clock()%100;
  float fBaseCraftSkillToNormalFumble=20.0*rpd.fDifficulty;
  static const int iBaseFumbleChancePerc=15;
  int iFumbleBase=iBaseFumbleChancePerc/(craftcore::CraftSkill(rpd.rc.H())/fBaseCraftSkillToNormalFumble); //ex.: 30%
  if(iFumbleBase>99)iFumbleBase=99; //%1 granted luck
  int iDiv=0;
  iDiv=1;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <=30%
  iDiv=2;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <=15%
  iDiv=4;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <= 7%
  iDiv=8;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <= 3%
  if(iLuckPerc<=1){
    iFumblePower++; //always have 1% granted fumble chance
    bCriticalFumble=true;
  }
  //current max chance per round of spawning broken is 5%
  if(clock()%100<=iFumblePower)
    return true;

  return false;
}

void crafthandle::CheckFumble(recipedata& rpd,bool bChangeTurns)
{
  rpd.rc.integrityCheck();
//  if(!v2XplodAt.Is0()){DBGSV2(v2XplodAt);
  if(rpd.fDifficulty>1.0){
    int xplodXtra=0;
    for(int i=0;i<rpd.iStrongerXplod;i++)
      xplodXtra+=clock()%5;

    bool bCriticalFumble=false;
    int iFumblePower=0;
    if(craftcore::CheckFumble(rpd,bCriticalFumble,iFumblePower)){
      if(!rpd.bSpawnBroken && bChangeTurns){
        rpd.iBaseTurnsToFinish/=2; //just complete the broken item (as AV gets halved) TODO repair system
        /**
         * this will insta provide the obvious messy lump as the item can't be broken
         * and won't even be half-usable, so dont wast player's time to craft nothing useful.
         */
        if(!rpd.bCanBeBroken && rpd.iBaseTurnsToFinish>1)
          rpd.iBaseTurnsToFinish=1;
      }
      rpd.bSpawnBroken=true;
    }

    bool bXplod=true;
    if(rpd.bOnlyXplodIfCriticalFumble && !bCriticalFumble)
      bXplod=false;
    rpd.xplodStr=0;
    if(bXplod){
      rpd.xplodStr = iFumblePower;
      if(rpd.xplodStr>0){DBG2(rpd.xplodStr,rpd.dbgInfo().CStr());
        rpd.xplodStr+=clock()%5+xplodXtra; //reference: weak lantern xplod str is 5
        //TODO anvil should always be near the forge. Anvil have no sparks. Keeping messages like that til related code is improved
      }
    }

//    DBG9(fBaseCraftSkillToNormalFumble,iFumbleBase,iLuckPerc,iFumblePower,bXplod,rpd.xplodStr,rpd.bCanBeBroken,rpd.bSpawnBroken,rpd.iBaseTurnsToFinish);
  }
}

void crafthandle::CheckIngredients(recipedata& rpd){
  rpd.rc.integrityCheck();
  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL){ //ABORT("ingredient id %d not found",rpd.ingredientsIDs[i]);
      /**
       * ex.: if something catches fire and is destroyed before the crafting ends.
       */
      ADD_MESSAGE("a required ingredient was destroyed...");DBG1(rpd.ingredientsIDs[i]);
      rpd.bFailedTerminateCancel=true; //TODO a crash happens in this line, how? memory corruption? if the tiny explosions trigger things on the floor like wands
      break;
    }

    // a magpie or siren may have taken it
    if(it->GetSquareUnder()!=rpd.lsqrActor){
      ADD_MESSAGE("%s ingredient went missing...",it->GetName(DEFINITE).CStr());
      rpd.bFailedTerminateCancel=true;
      break;
    }

    //TODO once: apply wands, release rings/ammys effects, xplod str 5+ if enchanteds +1 +2 etc
    if(!craftcore::canBeCrafted(it)){ //basically contains some kind of magic
      if(it->GetEnchantment()!=0)
        rpd.iStrongerXplod+=abs(it->GetEnchantment());
      else
        rpd.iStrongerXplod++; //TODO could add based on how hazardous the magic type is ex. fireball wand would be +100 or something like that..
    }
  }
}

void crafthandle::CheckFacilities(recipedata& rpd){
  rpd.rc.integrityCheck();
  if(!rpd.v2AnvilLocation.Is0()){
    olterrain* ot = game::GetCurrentLevel()->GetLSquare(rpd.v2AnvilLocation)->GetOLTerrain();
    if(ot==NULL || ot->GetConfig()!=ANVIL){
      ADD_MESSAGE("The anvil was destroyed!");
      rpd.bFailedTerminateCancel=true;
    }

    rpd.v2XplodAt=rpd.v2AnvilLocation;
  }else
  if(!rpd.v2ForgeLocation.Is0()){
    olterrain* otForge = game::GetCurrentLevel()->GetLSquare(rpd.v2ForgeLocation)->GetOLTerrain();
    if(otForge==NULL || otForge->GetConfig()!=FORGE){
      ADD_MESSAGE("The forge was destroyed!");
      rpd.bFailedTerminateCancel=true;
    }

    rpd.v2XplodAt=rpd.v2ForgeLocation;
  }else
  if(!rpd.v2WorkbenchLocation.Is0()){
    olterrain* otWorkbench = game::GetCurrentLevel()->GetLSquare(rpd.v2WorkbenchLocation)->GetOLTerrain();
    if(otWorkbench==NULL || otWorkbench->GetConfig()!=WORK_BENCH){
      ADD_MESSAGE("The workbench was destroyed!");
      rpd.bFailedTerminateCancel=true;
    }

    //TODO workbench should be damaged w/o explosions (that is area effect related to fire/forge)
    rpd.bOnlyXplodIfCriticalFumble=true; //TODO kept as WIP
    //explode/sparks at workbench doesnt make much sense: rpd.v2XplodAt=rpd.v2WorkbenchLocation;
  }
}

item* checkTool(ulong id,lsquare* lsqrActor){
  item* it=NULL;
  if(id!=0){
    it = game::SearchItem(id); //must keep searching it as it may have been destroyed.
    //TODO if a tool was broken and gets fixed, it's old ID will vanish!!! how to handle it!??!?!
    if(it==NULL){
      ADD_MESSAGE("One unmodified tool to craft this is missing or destroyed.");
      return NULL;
    }
    DBGEXEC(if(it!=NULL)DBGSV2(it->GetLSquareUnder()->GetPos()));
  }

  if(it!=NULL && it->GetLSquareUnder()!=lsqrActor)//rpd.itTool!=Actor->GetMainWielded())
  {DBGLN; //TODO re-mainWield it
    ADD_MESSAGE("%s went missing.",it->GetName(DEFINITE).CStr());
    return NULL;
  }

  return it;
}

void crafthandle::CheckTools(recipedata& rpd){
  rpd.lsqrActor = rpd.rc.H()->GetLSquareUnder(); DBGSV2(rpd.lsqrActor->GetPos());

  if(rpd.itToolID>0){
    rpd.itTool=checkTool(rpd.itToolID,rpd.lsqrActor);
    if(rpd.itTool==NULL)
      rpd.bFailedTerminateCancel=true;
  }

  if(rpd.itTool2ID>0){
    rpd.itTool2=checkTool(rpd.itTool2ID,rpd.lsqrActor);
    if(rpd.itTool2ID>0 && rpd.itTool2==NULL)
      rpd.bFailedTerminateCancel=true;
  }

  if(rpd.bFailedTerminateCancel)
    rpd.itTool=rpd.itTool2=NULL; //consistency or have the two (if required), or have none

  DBG8(rpd.itToolID,rpd.itTool,rpd.itTool2ID,rpd.itTool2,rpd.itSpawnCfg,rpd.otSpawnCfg,rpd.itSpawnType,rpd.otSpawnType);
}

void crafthandle::CheckEverything(recipedata& rpd, character* CurrentActor){
  rpd.rc.integrityCheck(CurrentActor);

  if(!rpd.IsFailedSuspendOrCancel())
    if(craftcore::CheckHumanoid(CurrentActor)==NULL)
      rpd.bFailedSuspend=true; //may have polymorphed

  if(!rpd.IsFailedSuspendOrCancel())
    if(rpd.rc.H()!=CurrentActor){
      /**
       * it is of ultimate importance to update the current actor
       */
      rpd.rc.SetHumanoid(CurrentActor);
    }

  if(!rpd.IsFailedSuspendOrCancel())
    if(!craftcore::CheckArms(rpd.rc.H()))
      rpd.bFailedSuspend=true;

  if(!rpd.IsFailedSuspendOrCancel())
    CheckTools(rpd);

  DBG2(DBGAV2(CurrentActor->GetPos()),DBGAV2(rpd.v2PlayerCraftingAt));
  if(CurrentActor->GetPos() != rpd.v2PlayerCraftingAt){ //in case player is teleported
    ADD_MESSAGE("You need to move back to where you started crafting.");
    rpd.bFailedSuspend=true;
  }

  rpd.rc.integrityCheck();
  DBGSV2(rpd.v2PlaceAt);
  if(!rpd.IsFailedSuspendOrCancel()){
    if(rpd.otSpawnType!=CTT_NONE){
      rpd.lsqrPlaceAt = CurrentActor->GetNearLSquare(rpd.v2PlaceAt); //TODO near? it is absolute map pos... confusing method name or I missed something???
      olterrain* oltExisting = rpd.lsqrPlaceAt->GetOLTerrain();
      if(oltExisting!=NULL){DBGLN;
    //  ADD_MESSAGE("%s cannot be placed there.", rpd.otSpawn->GetName(DEFINITE).CStr()); //TODO like in case something is placed there before ending the construction?
        ADD_MESSAGE("Unable to place it there."); //TODO like in case something is placed there before ending the construction? but what and how?
        rpd.bFailedTerminateCancel=true;
      }
    }
  }

  if(!rpd.IsFailedSuspendOrCancel())
    CheckIngredients(rpd);

  if(!rpd.IsFailedSuspendOrCancel())
    CheckFacilities(rpd);

  if(!rpd.IsFailedSuspendOrCancel())
    CheckFumble(rpd);

  rpd.rc.integrityCheck();
}

cfestring crafthandle::DestroyIngredients(recipedata& rpd){
  festring fsIng,fsIngP;
  festring fsIngPrev,fsIngPPrev;
  int iCountEqual=1;
  festring fsIngMsg("");
  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL)ABORT("ingredient id %lu not found %s",rpd.ingredientsIDs[i],rpd.dbgInfo().CStr());
    it->RemoveFromSlot();DBGLN;

    bool bSendToHell=true;
    if(rpd.otSpawnMatMainCfg!=0)
      if(it->GetMainMaterial()->GetConfig() != rpd.otSpawnMatMainCfg)
        if(rpd.rc.H()->GetPos()!=rpd.lsqrPlaceAt->GetPos()){
          /**
           * TODO this diff pos check shouldnt be necessary, CanMoveOn() should suffice but didnt work as it always can't move on, therefore always "true" TODO confirm again...
          if(!rpd.rc.H()->CanMoveOn(rpd.lsqrPlaceAt))
           */

          /**
           * this keep still available but inaccessible ingredients differing from constructed main material
           * TODO but as ghost they would still be recoverable, keep as trick/feature/secret? or destroy them? or just prevent diff materials at all during their selection?
           */

          bSendToHell=false;
        }

    if(bSendToHell){DBGLN;
      it->SendToHell();
    }else{DBGLN;
      //this way, the lower quality wall will still contain all stones in a non destructive way, is more fair
      //TODO what about amulet of phasing or ghost mode?
      it->MoveTo(rpd.lsqrPlaceAt->GetStack());
    }DBGLN;

    fsIng.Empty();fsIng << it->GetName(INDEFINITE);DBGLN;
    fsIngP.Empty();fsIngP << it->GetName(PLURAL);DBGLN;

    bool bNewType = fsIngPrev!=fsIng;DBGLN;

    bool bDumpPrev = false;
    if(bNewType)
      bDumpPrev=true;
    if(i==rpd.ingredientsIDs.size()-1){DBGLN;
      bDumpPrev=true;
      fsIngPrev=fsIng;
    }

    if(bDumpPrev){DBGLN;
      if(fsIngMsg.GetSize()>0)
        fsIngMsg<<", ";

      if(iCountEqual>1){
        fsIngMsg << (iCountEqual+1) << " " << fsIngPPrev;
      }else{
        fsIngMsg << fsIngPrev;
      }

      iCountEqual=1;
    }else
      iCountEqual++;

    fsIngPrev.Empty();fsIngPrev<<fsIng;
    fsIngPPrev.Empty();fsIngPPrev<<fsIngP;
  }

  if(fsIngMsg.GetSize()>0) //TODO this needs improving, for plural etc, to look good
    return festring() << " using " << fsIngMsg.CStr();

  return "";
}

/**
 *
 * @param rpd
 * @param mat
 * @param ForceType CIT_... stick, lump, stone
 * @param volume is the main material volume, it is important to be set before item->CalculateAll()
 * @return
 */
item* craftcore::PrepareRemains(recipedata& rpd, material* mat, int ForceType, long volume) //TODO force type could be a class (type) reference?
{
  if(mat==NULL)
    ABORT("NULL remains material");

  DBG2(mat->GetName(DEFINITE).CStr(),mat->GetVolume());
  bool bLiquid = mat->IsLiquid();
  if(mat->IsPowder())bLiquid=false; //TODO if explosive could have a chance to xplod use the fumble function TODO generalize it

  if(dynamic_cast<gas*>(mat)!=NULL)return NULL; //TODO should have a chance to release the gas effect if not 100% :)

  if(bLiquid){
    rpd.rc.H()->SpillFluid(NULL,liquid::Spawn(mat->GetConfig(),mat->GetVolume())); //TODO use a fumble check to determine on floor or on character (worse)
    return NULL;
  }

  item* itTmp = NULL;

  int Type = CIT_NONE;

  if(ForceType!=CIT_NONE){
    Type=ForceType;DBGLN;
  }else{
    /**
     * TODO
     * create leather/cloth pieces, seweing tools and cloth crafting.
     * Chain mail should be this too and require metal cutting tool.
     */
    if(Type==CIT_NONE) //specific lumps
      if(craftcore::IsMeltable(mat) || mat->IsFlesh() || (mat->GetCategoryFlags() & CAN_BE_TAILORED))
        Type = CIT_LUMP;

    if(Type==CIT_NONE)
      if(IsWooden(mat) || IsBone(mat))
        Type = CIT_STICK;

    /**
     * the requested could be a shaped stone, not suitable to all re-uses.
     * also see item creation that already won't use full volume of non meltable stones
     * TODO mat->IsSolid() will not help to determine if it should be a stone, ex.: ruby material returned false :(
     */
    if(Type==CIT_NONE)
      Type = CIT_STONE;

    /**
     * TODO generic fallback should be lump
    if(itTmp==NULL){
      Type = CIT_LUMP;DBGLN;
    }
    */
  }

  switch(Type){
    case CIT_LUMP:DBGLN;
      itTmp = lump::Spawn(0, NO_MATERIALS);
      break;
    case CIT_STICK:DBGLN;
      itTmp = stick::Spawn(0, NO_MATERIALS);
      break;
    case CIT_STONE:DBGLN;
      /**
       * the requested could be a shaped stone, not suitable to all re-uses.
       * also see item creation that already won't use full volume of non meltable stones
       */
      itTmp = stone::Spawn(0, NO_MATERIALS);
      break;
  }

//    delete itTmp->SetMainMaterial(material::MakeMaterial(mat->GetConfig(),mat->GetVolume()));
  delete itTmp->SetMainMaterial(CreateMaterial(mat));
  
  if(volume>0)
    itTmp->GetMainMaterial()->SetVolume(volume);

  craftcore::CopyDegradation(mat,itTmp->GetMainMaterial());

  FinishSpawning(rpd,itTmp);

  ADD_MESSAGE("%s was recovered.", itTmp->GetName(DEFINITE).CStr());

  return itTmp;
}

void craftcore::FinishSpawning(recipedata& rpd,item* itSpawn){
  itSpawn->MoveTo(rpd.rc.H()->GetStack());DBGLN;

  DBG3("EmitDbgSpawned",itSpawn->GetEmitation(),itSpawn->GetVolume());
  if(itSpawn->GetEmitation()>0){ //TODO is there a better way to do this emitation fix?
    // uses the previous emitation to fix everywhere before recalculating item emitation
    rpd.rc.H()->SignalEmitationDecrease(itSpawn->GetEmitation());
    rpd.rc.H()->CalculateEmitation();

    rpd.rc.H()->GetLSquareUnder()->SignalEmitationDecrease(itSpawn->GetEmitation());
    rpd.rc.H()->GetLSquareUnder()->CalculateLuminance();
    
    // last
    itSpawn->SignalEmitationDecrease(itSpawn->GetEmitation());
    itSpawn->CalculateEmitation();
  }
}

std::vector<uint> vBone;
bool craftcore::IsBone(material* mat)
{
  static bool bDummyInit = [](){
    vBone.push_back(BONE);
    vBone.push_back(DRAGON_BONE);
    vBone.push_back(WHALE_BONE);
    vBone.push_back(WRAITH_BONE);
    vBone.push_back(OMMEL_BONE);
    // new IDs wont be sequential, so no range...
    return true;
  }();bDummyInit=true;//assigning just to remove IDE's unused warnings.. is dead code anyway, should not exist...

  if(IsMeltable(mat))
    return false;

  for(auto pcfg = vBone.begin(); pcfg != vBone.end(); pcfg++)
    if(mat->GetConfig() == *pcfg)
      return true;

  return false;
}

bool craftcore::IsWooden(material* mat){DBG3(mat->GetConfig(),FUNGI_WOOD,PETRIFIED_WOOD);
  if(IsMeltable(mat)){
    return false;DBGLN;
  }
  if(mat->GetConfig()>=FUNGI_WOOD && mat->GetConfig()<=PETRIFIED_WOOD) //if a new wood type is ever added, use a vector like vBone
    return true;
  return false;DBGLN;
}

int craftcore::CitType(item* it)
{
  if(dynamic_cast<stick*>(it)!=NULL)return CIT_STICK;
  if(dynamic_cast<lump*>(it)!=NULL)return CIT_LUMP;
  if(dynamic_cast<stone*>(it)!=NULL)return CIT_STONE;
  if(dynamic_cast<potion*>(it)!=NULL)return CIT_POTION;
  return CIT_NONE;
}

truth craftcore::IsMeltable(item* it)
{
  return IsMeltable(it->GetMainMaterial(),it->GetSecondaryMaterial());
}

truth craftcore::IsMeltable(material* matM,material* matS)
{
  if(!craftcore::IsMeltable(matM))
    return false;
  if(matS!=NULL && !craftcore::IsMeltable(matS))
    return false;
  return true;
}

truth craftcore::IsMeltable(material* mat){
  if(mat->GetCategoryFlags() & IS_METAL)
    return true;
  if(mat->GetConfig()==GLASS)
    return true;
  //TODO find more meltables and add them here
  return false;
}

bool IsDegradedMat(material* mat){
  if(mat==NULL)return false;
  if(mat->GetBurnLevel()>0)return true;
  if(mat->GetSpoilLevel()>0)return true;
  if(mat->GetRustLevel()>0)return true;
  return false;
}
bool craftcore::IsDegraded(item* it,bool bShowMsg){
  bool b=false;
  if(IsDegradedMat(it->GetMainMaterial()))b=true;
  if(IsDegradedMat(it->GetSecondaryMaterial()))b=true;

  if(b && bShowMsg)
    ADD_MESSAGE("%s is too degraded to work with.",it->GetName(DEFINITE).CStr());

  return b;
}

void craftcore::CopyDegradation(item* itFrom,material* matTo)
{
  material* matFromM = itFrom->GetMainMaterial();
  if(matFromM!=NULL){
    CopyDegradation(matFromM,matTo);
  }else{
    /**
     * w/o main material there is:
     * - no rust available
     * - no burn available
     * //TODO will this make things easier to player?
     * PS.: this ends being probably only for corpses.
     */
    ushort SC=0;
    while(true){ //TODO reverse SpoilLevel into SpoilCounter value
      matTo->SetSpoilCounter(SC);
      if(matTo->GetSpoilLevel()>=itFrom->GetSpoilLevel())
        break;
      SC+=5; // from slowest at organic::Be
    }
  }
}
void craftcore::CopyDegradation(material* matFrom,material* matTo)
{
  if(dynamic_cast<organic*>(matTo)!=NULL)
    ((organic*)matTo)->SetSpoilCounter(((organic*)matFrom)->GetSpoilCounter());
  matTo->SetBurnLevel(matFrom->GetBurnLevel(),false);
  matTo->SetRustLevel(matFrom->GetRustLevel());
}

void craftcore::CopyDegradationIfPossible(recipedata& rpd, item* itTo)
{
  /**
   * will work with simple recipes only
   * where the work is from one simple ingredient
   */
  if(rpd.ingredientsIDs.size()==1){
    item* itIng = game::SearchItem(rpd.ingredientsIDs[0]);
    if(itIng->GetSecondaryMaterial()==NULL)
      CopyDegradation(itIng,itTo->GetMainMaterial());
  }
}

void craftcore::Save(outputfile& SaveFile)
{
  int size = vSuspended.size();
  SaveFile << size; //DBG2(size,SaveFile.GetFullFilename().CStr()); //token
  if(size>0){
    for(int i=0;i<size;i++)
      vSuspended[i].Save(SaveFile);
  }
}

void craftcore::Load(inputfile& SaveFile)
{
  ClearSuspendedList(); //make sure it is always cleaned from memory!
  if(game::GetCurrentSavefileVersion()<133)
    return;

  int size = 0;
  SaveFile >> size; //DBG2(size,SaveFile.GetFullFilename().CStr()); //token
  if(size>0){
    for(int i=0;i<size;i++){
      recipedata rpd;
      rpd.Load(SaveFile);
      vSuspended.push_back(rpd);
    }
  }
}

//recipeprototype::recipeprototype(recipespawner Spawner, cchar* ClassID)
//: Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<recipedatacore>::Add(this); }
//
//recipedatacore* recipeprototype::SpawnAndLoad(inputfile& SaveFile) const
//{
//  recipedatacore* prpd = Spawner(0);
//  prpd->Load(SaveFile);
//  return prpd;
//}
