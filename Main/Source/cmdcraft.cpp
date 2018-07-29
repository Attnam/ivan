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

#include "dbgmsgproj.h"

#include "cmdcraftfilters.cpp"

recipedata* craftcore::prpdSuspended=NULL;

//void craftcore::reinitIfNeeded(){
//  if(player!=PLAYER){
//    player=PLAYER;
//    craftAction=NULL;
//  }
//}

void craftcore::ResetSuspended()
{
  if(prpdSuspended!=NULL){
    DBG2("deleting",prpdSuspended->dbgInfo().CStr());
    delete prpdSuspended;
    prpdSuspended=NULL; //resets
  }
}

bool craftcore::EmptyContentsIfPossible(recipedata& rpd,item* itContainer, bool bMoveToInventory)
{
  materialcontainer* mc = dynamic_cast<materialcontainer*>(itContainer); //potions, mines... also bananas xD
  itemcontainer* ic = dynamic_cast<itemcontainer*>(itContainer); //chests

  bool bEmptied = false;

  if(mc!=NULL){
    if(mc->GetSecondaryMaterial()!=NULL){
      if(bMoveToInventory)
        PrepareRemains(mc->GetSecondaryMaterial(),rpd);
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

void craftcore::SetSuspended(recipedata* prpd){DBG2(prpd,prpdSuspended);
  if(prpd==NULL){ //calling as this must be sure to delete the craft* object outside here!
    ResetSuspended();
    return;
  }

  if(prpdSuspended!=NULL && prpd!=NULL)
    if(prpdSuspended->id()==prpd->id()){ DBG2("settingAgainToSameNotAProblemButShouldNotHappen",prpd->dbgInfo().CStr());
      ResetSuspended(); //in case some properties got modified, to let it be updated
    }

  if(prpdSuspended==NULL){DBGLN;
    prpdSuspended=new recipedata(NULL);
    if(!prpd->rc.IsCanBeSuspended())
      ABORT("action can't be suspended %s",prpd->dbgInfo().CStr());
    (*prpdSuspended)=(*prpd); //copy
//    prpdSuspended->ClearRefs();
    prpdSuspended->rc.integrityCheck();
    return;
  }

  ABORT("there is already a recipedata set %s VS %s",prpdSuspended->dbgInfo().CStr(),prpd->dbgInfo().CStr());
}

float craftcore::CraftSkill(character* Char){ //is the current capability of crafting
  return (Char->GetAttribute(DEXTERITY)+Char->GetAttribute(WISDOM))/2.0; //ex.: D=10 W=10 Skill=10, D=5 W=10 Skill=7.5
}

bool craftcore::canBeCrafted(item* it){
  if(dynamic_cast<lump*>(it)!=NULL)
    return true;

  const itemdatabase* itdb = it->GetProtoType()->GetConfigData()[0];

  if(
    game::IsQuestItem(it) ||
    it->GetEnchantment()!=0 ||
    it->GetCategory()==BOOK ||
    it->GetCategory()==FOOD || //TODO allow near oven, but the problem is the ingredients... also may be blocked below too
    !itdb->CanBeWished ||
    false // just to make it easier to re-organize and add checks above
  ){
    return false;
  }

  //TODO all these things should have some easier kind of property to be checked
  if(!craftcore::MoreCraftDeniedFilters(it))
    return false;

  return true;
}

bool craftcore::HasSuspended() {
  if(prpdSuspended==NULL)
    return false;

  if(prpdSuspended->bSuccesfullyCompleted)
    return false;

  if(prpdSuspended->rc.IsCanBeSuspended())
    return true;

  return false;
}
void craftcore::ResumeSuspendedTo(character* Char){
  if(!HasSuspended())
    ABORT("no suspended craft action to set to %s!",Char->GetName(DEFINITE).CStr());

//  if(Char->GetAction()==craftAction){DBG1("Already,How?") //should not happen tho...
//    return;
//  }

  if(Char->GetAction()!=NULL){DBG1("AlreadySomethingElse,How?");DBGSTK; //should never happen tho... TODO ABORT() ?
    ADD_MESSAGE("I am already doing something else.");
    return;
  }

  prpdSuspended->rc.integrityCheck();

  if(prpdSuspended->bMeltable || prpdSuspended->otSpawnCfg!=0){ //TODO facilities requirements like oven, workbench etc later must be considered too here!
    if(prpdSuspended->rc.GetDungeonLevelID()!=craftcore::CurrentDungeonLevelID()){
      //TODO better message: place? location? dungeon level sounds a bit non-immersive, or not?
      ADD_MESSAGE("I need to be in the same dungeon I was before to continue preparing it.");
      return;
    }

    if(prpdSuspended->v2PlayerCraftingAt!=Char->GetPos()){
      festring fsDist;
      int iDist = (prpdSuspended->v2PlayerCraftingAt - Char->GetPos()).GetLengthSquare();
      if(iDist<=2)fsDist<<"and I am almost there!";
      else
      if(iDist<=10)fsDist<<"and I am near it.";
      else
      if(iDist<=20)fsDist<<"but I am still a bit far away from it...";
      else
        fsDist<<"but it seems to be quite far from here.";
      ADD_MESSAGE("I need to be were I was crafting before, %s",fsDist.CStr());
    }
  }

  prpdSuspended->v2PlayerCraftingAt = Char->GetPos();
  Char->SwitchToCraft(*prpdSuspended);
  craftcore::SetSuspended(NULL); //was resumed so discard it
}
//void craftcore::TerminateSuspendedAction(){
//  if(prpdSuspended==NULL)ABORT("no craft action set!");
//  if(!prpdSuspended->bCanBeSuspended)ABORT("is not a suspended craft action!"); //TODO show what was being done
//  craftAction->Terminate(false);
//}

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
    << bFailed
    << v2PlayerCraftingAt

    << itToolID
    << itTool2ID
    << v2BuildWhere
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

    ;

//  if(otSpawnType!=CTT_NONE)
//    SaveFile << otSpawn;
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
    >> bFailed
    >> v2PlayerCraftingAt

    >> itToolID
    >> itTool2ID
    >> v2BuildWhere
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
   */

  festring fs;

  int i=0;

  fs<<(i++)<<":"<<rc.IsCanBeSuspended()<<";";

  #define RPDINFO(o) if(o!=NULL)fs<<(i++)<<":"<<o->GetID()<<","<<o->GetName(DEFINITE);fs<<";";
  fs<<(i++)<<":"<<itToolID<<";"<<itTool2ID<<";";
  RPDINFO(itTool);

  fs<<(i++)<<":"<<itSpawnTot<<";";
  fs<<(i++)<<":"<<itSpawnCfg<<";";
  fs<<(i++)<<":"<<itSpawnMatMainCfg<<";";
  fs<<(i++)<<":"<<itSpawnMatMainVol<<";";
  fs<<(i++)<<":"<<itSpawnMatSecCfg<<";";
  fs<<(i++)<<":"<<itSpawnMatSecVol<<";";
//  RPDINFO(itSpawn);

//  RPDINFO(otSpawn);
  fs<<(i++)<<":"<<otSpawnCfg<<";";
  fs<<(i++)<<":"<<otSpawnMatMainCfg<<";";
  fs<<(i++)<<":"<<otSpawnMatMainVol<<";";
  fs<<(i++)<<":"<<otSpawnMatSecCfg<<";";
  fs<<(i++)<<":"<<otSpawnMatSecVol<<";";

  fs<<(i++)<<":"<<v2BuildWhere.X<<","<<v2BuildWhere.Y<<";";
  fs<<(i++)<<":"<<v2AnvilLocation.X<<","<<v2AnvilLocation.Y<<";";
  fs<<(i++)<<":"<<v2ForgeLocation.X<<","<<v2ForgeLocation.Y<<";";
  fs<<(i++)<<":"<<v2WorkbenchLocation.X<<","<<v2WorkbenchLocation.Y<<";";
  fs<<(i++)<<":"<<v2PlaceAt.X<<","<<v2PlaceAt.Y<<";";
  fs<<(i++)<<":"<<v2PlayerCraftingAt.X<<","<<v2PlayerCraftingAt.Y<<";";

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
  if(h!=NULL)
    ABORT("Humanoid actor already set '%s' '%s'",C->GetName(DEFINITE).CStr(),h->GetName(DEFINITE).CStr());

  h = dynamic_cast<humanoid*>(C);
  if(h==NULL)
    ABORT("Only humanoids can craft '%s'",C->GetName(DEFINITE).CStr());
}

void recipecore::integrityCheck(character* Actor) const
{
  if(initKey!=RPDInitKey){ //FIRST TEST!!!!!!!!!!!!!!!!!!!!!
    ABORT("recipedata corrupted, not initialized or invalid");// it will not be possible to show info, would crash on it... , dbgInfo().CStr());
  }

  //TODO bools get crazy values too if not initialized

  if(Actor!=NULL && h!=NULL && h!=Actor)
    ABORT("actor changed? '%s' '%s'",h->GetName(DEFINITE).CStr(),Actor->GetName(DEFINITE).CStr());
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

  lvl=NULL;

  // no need to save
  SelectedRecipe=sel;
  bSpendCurrentTurn=false;
  bAlreadyExplained=false;
  bHasAllIngredients=false;
  bCanStart=false;

  bCanBePlaced=false;
  xplodStr=0;
  iStrongerXplod=0;
  v2XplodAt=v2(0,0);
  bOnlyXplodIfCriticalFumble=false;

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
  bFailed=false;
  v2PlayerCraftingAt=v2(0,0);

  itToolID=0;
  itTool2ID=0;
  v2BuildWhere=v2(0,0);
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

olterrain* craftcore::SpawnTerrain(recipedata& rpd, festring& fsCreated){
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

  otSpawn->SetMainMaterial(material::MakeMaterial(rpd.otSpawnMatMainCfg,rpd.otSpawnMatMainVol));
  if(rpd.otSpawnMatSecCfg>0)
    otSpawn->SetSecondaryMaterial(material::MakeMaterial(rpd.otSpawnMatSecCfg,rpd.otSpawnMatSecVol));

  fsCreated << "You built ";
  rpd.lsqrPlaceAt->ChangeOLTerrainAndUpdateLights(otSpawn);

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

  lvl = NULL;

  rc.ClearRefs();
}

struct ci{
  bool bMultSelect = true;

  int iReqCfg=0;
  bool bMainMaterRemainsBecomeLump=false;
  bool bOverridesQuestion=false;
  bool bMsgInsuficientMat=true;
  bool bInstaAddIngredients=false;

  bool bFirstMainMaterIsFilter=true;
  int iReqMatCfgMain=0;
  int iReqMatCfgSec=0;
  bool bJustAcceptFirstChosenAndReturn=false;
  bool bDenyDegradation = true;

  bool bFirstItemMustHaveFullVolumeRequired=false;
  bool bAllowMeltables=true;
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

  bool IsTheSelectedOne(recipedata& rpd){
    if(desc.IsEmpty())
      fillInfo();
    if(desc.IsEmpty())
      ABORT("recipe info is still empty");

    return rpd.SelectedRecipe == iListIndex;
  }

  virtual bool work(recipedata& rpd){ return false; }

  virtual ~recipe(){}

  static bool where(recipedata& rpd){
    int Dir = game::DirectionQuestion("Build it where?", false, false);DBGLN;
    if(Dir != DIR_ERROR && rpd.rc.H()->GetArea()->IsValidPos(rpd.rc.H()->GetPos() + game::GetMoveVector(Dir)))
      rpd.lsqrPlaceAt = rpd.rc.H()->GetNearLSquare(rpd.rc.H()->GetPos() + game::GetMoveVector(Dir));

    if(rpd.lsqrPlaceAt!=NULL && rpd.lsqrPlaceAt->GetOLTerrain()==NULL && rpd.lsqrPlaceAt->GetCharacter()==NULL){
      rpd.v2PlaceAt = rpd.lsqrPlaceAt->GetPos();
      rpd.bCanBePlaced=true;
      return true;
    }

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

  static item* findCarvingTool(recipedata& rpd,item* itToWorkOn){
    int iCarvingStr=0;

    material* matM = itToWorkOn->GetMainMaterial();
    material* matS = itToWorkOn->GetSecondaryMaterial();
    if(!craftcore::IsMeltable(matM))
      iCarvingStr=matM->GetStrengthValue();
    if(matS!=NULL && !craftcore::IsMeltable(matS))
      iCarvingStr=Max(iCarvingStr,matS->GetStrengthValue());

    int iMinCarvingStr = iCarvingStr/2;

    item* itTool = FindTool(rpd, DAGGER, 0, iMinCarvingStr); //carving: tool cant be too much weaker

    if(itTool!=NULL){
//      addTool(itTool);
//      if(rpd.itTool==NULL)
//        rpd.itTool=itTool;
//      else
//        rpd.itTool2=itTool;

      int iMult=1;
      if(iCarvingStr>1){
        int itStr=itTool->GetMainMaterial()->GetStrengthValue();
        if(itStr<iCarvingStr)
          iMult++;
        if(itStr==iMinCarvingStr)
          iMult++;
      }
      calcToolTurns(rpd,iMult);

      if(!recipe::findOLT(rpd,WORK_BENCH)){
        ADD_MESSAGE("There is no workbench here, this will take time..."); //it is good to measure, hold tight, has a good height etc...
        rpd.iBaseTurnsToFinish *= 3;
      }

//      return true;
    }
//    return false;
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
      iMult=iTotToolTypes;
    }

    calcToolTurns(rpd,iMult);

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
    FBWC(LARGE_SWORDS);
    FBWC(POLE_ARMS);

    if(it!=NULL){
//      if(bReversedTimeMult)
//        iMult = iMaxMult-iMult;

      calcToolTurns(rpd,iMult);
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
         * unless spawining/cloning/copying changes to have to same
         * (or average in case of many to one) degradation data
         *
         * degradation should only be "fixable" thru magic (scroll of repair)
         */
        if(CI.bDenyDegradation && craftcore::IsDegraded(vi[i])) //TODO lower the volume of "remaining perfect materials" based on the degradation levels ?
          continue;

        if(CI.iReqCfg>0 && vi[i]->GetConfig()!=CI.iReqCfg)
          continue;

        if(CI.bFirstItemMustHaveFullVolumeRequired && vi[i]->GetVolume()<reqVol)
          continue;

        if(!CI.bAllowMeltables && craftcore::IsMeltable(vi[i]))
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
      long reqVol,
      recipedata& rpd,
      int& iWeakestCfg,
      ci CI=ci()
  ){DBGLN;
    if(reqVol==0)
      ABORT("ingredient required 0 volume?");

    if(CI.bFirstMainMaterIsFilter && CI.iReqMatCfgMain!=0)
      ABORT("not implemented how to deal with bFirstMainMaterIsFilter=true VS iReqMatCfgMain!=0 %d",CI.iReqMatCfgMain);

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
      if(CI.bOverridesQuestion)
        fsFullQ=fsQ;
      else
        fsFullQ = festring("What ingredient(s) will you use ")+fsQ+" ["+reqVol+"cm3]"+festring("? (hit ESC for more options if available)");

      rpd.rc.H()->GetStack()->DrawContents(ToUse, rpd.rc.H(),
        fsFullQ, flags, &item::IsValidRecipeIngredient);
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
          long lRemainingVol=reqVol*-1;
          if(lRemainingVol>0 && CI.bMainMaterRemainsBecomeLump){
            long lVolM = matM->GetVolume();
            lVolM -= lRemainingVol; //to sub
            if(lVolM<=0)
              ABORT("ingredient volume reduced to negative or zero %d %d %s",lVolM,lRemainingVol,matM->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
            matM->SetVolume(lVolM);

            item* lumpR = craftcore::PrepareRemains(matM, rpd);
            lumpR->GetMainMaterial()->SetVolume(lRemainingVol);

            lumpMix(vi,lumpR,rpd.bSpendCurrentTurn);

            material* matS = ToUse[i]->GetSecondaryMaterial();
            if(matS!=NULL && matS->GetVolume()>0)
              ABORT("ingredient secondary material should not have volume %d %s %s",matS->GetVolume(),matS->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
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
      if(CI.bMsgInsuficientMat && reqVol>0)
        ADD_MESSAGE("This amount of materials won't work...");

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

          craftcore::SendToHellSafely(lumpToMix); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
//          lumpToMix->RemoveFromSlot();
//          lumpToMix->SendToHell(); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
          bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...
          break;
        }
      }
    }
  }

//  lsquare* lsqrFORGE = NULL;
//  lsquare* lsqrANVIL = NULL;
//  lsquare* lsqrWorkbench = NULL;
  static void SetOLT(recipedata& rpd,lsquare* lsqr,int iCfgOLT){
    switch(iCfgOLT){
    case FORGE:
//      lsqrFORGE = lsqr;
      rpd.v2ForgeLocation = lsqr->GetPos();
      break;
    case ANVIL:
//      lsqrANVIL = lsqr;
      rpd.v2AnvilLocation = lsqr->GetPos();
      break;
    case WORK_BENCH:
//      lsqrWorkbench = lsqr;
      rpd.v2WorkbenchLocation = lsqr->GetPos();
      break;
    }
  }

  static bool chkOLT(recipedata& rpd,lsquare* lsqr,int iCfgOLT){DBGLN;
    olterrain* ot = lsqr->GetOLTerrain();
    if(ot!=NULL && ot->GetConfig() == iCfgOLT && lsqr->CanBeSeenBy(rpd.rc.H())){
      SetOLT(rpd,lsqr,iCfgOLT);
      return true;
    }
    return false;
  }
  static bool findOLT(recipedata& rpd,int iCfgOLT,bool bReqOnlyVisible=false){
    bool bFound=false;
    if(bReqOnlyVisible){DBGLN;
      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2(iX,iY));DBG3(lsqr,iX,iY);
        if(chkOLT(rpd,lsqr,iCfgOLT)){
          bFound = true;
          break;
        }
      }}
    }else{DBGLN; //must be on adjacent square
//      int iDist = 1; //TODO improve this (despite fun, is wrong..)
//      for(int i=0;i<(8*iDist);i++){
//        int iDir = i%8;
//        int iMult = 1 + i/8;
//        v2 v2Add = game::GetMoveVector(iDir) * iMult;
//        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG5(DBGAV2(v2Add),DBGAV2(v2Pos),iMult,iDir,i);
//        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
//          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);
//          if(chkOLT(rpd,lsqr,iCfgOLT)){
//            bFound = true;
//            break;
//          }
//        }
//      }
      for(int iDir=0;iDir<8;iDir++){
        v2 v2Add = game::GetMoveVector(iDir);
        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG3(DBGAV2(v2Add),DBGAV2(v2Pos),iDir);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);DBG1(lsqr);
          if(chkOLT(rpd,lsqr,iCfgOLT)){
            bFound = true;
            break;
          }
//          olterrain* ot = lsqr->GetOLTerrain();
//          if(ot!=NULL && ot->GetConfig() == ANVIL && lsqr->CanBeSeenBy(rpd.rc.H())){
//            lsqrAnvil = lsqr;
//            rpd.v2AnvilLocation=lsqrAnvil->GetPos();
//            break;
//          }
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

//  bool chkForge(recipedata& rpd,lsquare* lsqr){DBGLN;
//    olterrain* ot = lsqr->GetOLTerrain();
//    if(ot!=NULL && ot->GetConfig() == FORGE && lsqr->CanBeSeenBy(rpd.rc.H())){
//      lsqrFORGE = lsqr;
//      rpd.v2ForgeLocation = lsqrFORGE->GetPos();
//      return true;
//    }
//    return false;
//  }
//  bool findForge(recipedata& rpd,bool bReqOnlyVisible=false){
//    if(bReqOnlyVisible){DBGLN;
//      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
//        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2(iX,iY));DBG3(lsqr,iX,iY);
//        if(chkForge(rpd,lsqr))break;
//      }}
//    }else{DBGLN; //must be on adjacent square
//      int iDist = 1; //TODO improve this (despite fun, is wrong..)
//      for(int i=0;i<(8*iDist);i++){
//        int iDir = i%8;
//        int iMult = 1 + i/8;
//        v2 v2Add = game::GetMoveVector(iDir) * iMult;
//        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG5(DBGAV2(v2Add),DBGAV2(v2Pos),iMult,iDir,i);
//        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
//          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);
//          if(chkForge(rpd,lsqr))break;
//        }
//      }
//    }
//
//    if(lsqrFORGE==NULL){
//      ADD_MESSAGE(bReqOnlyVisible?"No visible forge nearby.":"No forge nearby.");
//      rpd.bAlreadyExplained=true;
//      return false;
//    }
//
//    return true;
//  }

};

struct srpOltBASE : public recipe{
 protected:
  int iReqVol=0;
  int iReqVolSkulls=0;
  int iTurns=0;
  bool bRequiresWhere=false;
  bool bAllowSimpleStones=false;
  bool bReqForge=false;
  ci CIdefault;
  festring fsDescBASE = "You will need a hammer or other blunt weapon.";

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
    rpd.itTool = FindBluntTool(rpd);

    if(rpd.lsqrPlaceAt->GetOLTerrain()==NULL && rpd.itTool!=NULL){
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
      if(!bH)
        bH=choseIngredients<stick>(fsQ, iReqVol, rpd, iCfg, CIdefault);
      if(!bH && iReqVolSkulls>0)
        bH=choseIngredients<skull> (fsQ, iReqVolSkulls, rpd, iCfg, CIdefault);
      if(!bH)
        bH=choseIngredients<bone> (fsQ, iReqVol, rpd, iCfg, CIdefault);
      if(bH){
        rpd.bHasAllIngredients=bH;
        rpd.v2PlaceAt = rpd.lsqrPlaceAt->GetPos();
        if(!spawnCfg(rpd))ABORT("Recipe spawn cfg not set %s",desc.CStr());
        rpd.otSpawnMatMainCfg=iCfg;
        rpd.otSpawnMatMainVol=iReqVol;
        rpd.bCanStart=true;
      }
    }

    return true;
  }
};
struct srpDoor : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_DOOR;
    rpd.otSpawnCfg=NONE;
    return true;
  }

  virtual void fillInfo(){
    init("build","a door");
    desc << "You will need a hammer or other blunt weapon.";
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
    desc << fsDescBASE;
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
    desc << "Near a forge you can create an anvil. " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=750*3; //when destroyed provides 250 to 750 x3, so lets use the max to avoid spawning extra material volume
    iTurns=15;
    bReqForge=true;
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
    desc << "You can build a forge. " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=15000;
    iTurns=30;
    bRequiresWhere=true;
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
    desc << "You can build a workbench. " << fsDescBASE;
  }

  virtual bool work(recipedata& rpd){
    iReqVol=3000;
    iTurns=10;
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
    desc << "You can construct a wall piling stones, sticks or bones.";
  }

  virtual bool work(recipedata& rpd){
    //TODO this doesnt look good. anyway this volume should be on the .dat file as wall/earthWall attribute...
    iReqVol=9000; //TODO is this too little? a broken wall drops 3 rocks that is about 1000 each, so 3 walls to build one is ok?
    iReqVolSkulls=12000; //TODO is this too little? necromancers can spawn skeletons making it easy to get skulls, but the broken bone wall will drop bones and not skulls...
    iTurns=20;
    bRequiresWhere=true;
    bAllowSimpleStones=true;
    return srpOltBASE::work(rpd);
  }
};srpWall2 rpWall2;

struct srpJoinLumps : public recipe{
  virtual void fillInfo(){
    init("join","a lump");
    desc << "Join lumps of same material.";
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

    if(rpd.ingredientsIDs.empty())
      return false;

    joinLumpsEqualToFirst(rpd);

    rpd.bAlreadyExplained = true;

    return true;
  }
};srpJoinLumps rpJoinLumps;

struct srpMelt : public srpJoinLumps{
  virtual void fillInfo(){
    init("melt","an ingot");
    desc << "Near a forge, meltable lumps can be used to prepare ingots.";
  }

  virtual bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.

    if(!recipe::findOLT(rpd,FORGE))
      return false;

    askForEqualLumps(rpd);

    if(rpd.ingredientsIDs.empty())
      return false;

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

//    rpd.itSpawn->SetMainMaterial(material::MakeMaterial(
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
    init("dismantle","some materials as lumps and sticks");
    desc << "Near a forge, any meltable item can be dismantled to recover it's materials. Otherwise you just need a cutting tool.";
  }

  virtual bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.

    ///////////////////// chose item to melt/smash
    game::DrawEverythingNoBlit();
    ci CI;
    CI.bDenyDegradation=false; //to let user know what is happening w/o spamming it.
    if(!choseOneIngredient<item>(rpd,&CI))
      return false;

    item* itToUse = game::SearchItem(rpd.ingredientsIDs[0]); DBG2(rpd.ingredientsIDs[0],itToUse);
    rpd.ingredientsIDs.clear();

    if(game::IsQuestItem(itToUse)){
      ADD_MESSAGE("You feel that would be a bad idea and carefully stores it back in your inventory.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(dynamic_cast<corpse*>(itToUse)!=NULL){
      ADD_MESSAGE("I should try to split %s instead.",itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(craftcore::IsDegraded(itToUse,true)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    material* matM=NULL;
    material* matS=NULL;
    material* matIngot=NULL;

    matM = itToUse->GetMainMaterial();
    matS = itToUse->GetSecondaryMaterial();

    rpd.bMeltable = craftcore::IsMeltable(itToUse);
    if(rpd.bMeltable){
      if(!recipe::findOLT(rpd,FORGE))
        return false;
    }else{
      rpd.itTool = FindCuttingTool(rpd);//,true);
      if(!rpd.itTool)
        return false;
    }

    bool bJustLumpfyTheIngot=false;
    if(dynamic_cast<stone*>(itToUse)!=NULL && itToUse->GetConfig()==INGOT){
      bJustLumpfyTheIngot=true;
    }

    /////////////////////// dismantle into lumps
    if(dynamic_cast<lump*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a lump.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    if(dynamic_cast<stick*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a stick.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return false;
    }

    item* LumpMeltable = NULL;
    // for now, uses just one turn to smash anything into lumps but needs to be near a FORGE TODO should actually require a stronger hammer than the material's hardness being smashed, and could be anywhere...

    { // main material ALWAYS exist
      item* LumpM = craftcore::PrepareRemains(matM,rpd);
      if(craftcore::IsMeltable(LumpM->GetMainMaterial()))
        LumpMeltable = LumpM;
    }

    if(matS!=NULL){
      item* LumpS = craftcore::PrepareRemains(matS,rpd); //must always be prepared to not lose it
      if(LumpS!=NULL)
        if(craftcore::IsMeltable(LumpS->GetMainMaterial()) )
          if(LumpMeltable==NULL)
            LumpMeltable = LumpS;
    }

    craftcore::EmptyContentsIfPossible(rpd,itToUse,true);

    ADD_MESSAGE("%s was completely dismantled.", itToUse->GetName(DEFINITE).CStr());
    rpd.bAlreadyExplained=true;
    craftcore::SendToHellSafely(itToUse);
    DBG4("SentToHell",itToUse->GetID(),itToUse,LumpMeltable); //TODO if has any magic should release it and also harm

    rpd.bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...

    if(bJustLumpfyTheIngot){
      lumpMix(vitInv(rpd),LumpMeltable,rpd.bSpendCurrentTurn);
      rpd.bAlreadyExplained=true;
      return false;
    }

    return true;
  }
};srpDismantle rpDismantle;

struct srpSplitLump : public recipe{
  void explain(recipedata& rpd,festring fs){
    ADD_MESSAGE("I need a cutting weapon to split %s.",fs.CStr());
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
    init("split","some base materials");
    desc << "Split them to be easier to work with.";
  }

  virtual bool work(recipedata& rpd){
    item* ToSplit = NULL;

    if(ToSplit==NULL && choseOneIngredient<lump>(rpd)){ //can  be split with hands only
      ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
      rpd.itSpawnType = CIT_LUMP;
    }

    if(ToSplit==NULL && choseOneIngredient<corpse>(rpd)){ //this is just a simplified conversion from corpse to a big lump...
      corpse* Corpse = (corpse*)game::SearchItem(rpd.ingredientsIDs[0]);

      if(!reqCut(rpd,Corpse->GetName(INDEFINITE)))
        return false;

      //TODO this should take time as an action
      character* D = Corpse->GetDeceased(); DBG2(Corpse->GetName(DEFINITE).CStr(),D->GetName(DEFINITE).CStr())
      static const materialdatabase* flesh;flesh = material::GetDataBase(D->GetFleshMaterial());
      ToSplit = craftcore::PrepareRemains(material::MakeMaterial(flesh->Config,Corpse->GetVolume()), rpd);
      rpd.itSpawnType = CIT_LUMP;

      craftcore::SendToHellSafely(Corpse);
      rpd.ingredientsIDs.clear();

      rpd.ingredientsIDs.push_back(ToSplit->GetID());

      rpd.bAlreadyExplained=true; //no need to say anything
    }

    if(ToSplit==NULL && choseOneIngredient<stick>(rpd)){
      ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
      if(!reqCut(rpd,ToSplit->GetName(INDEFINITE)))
        return false;
      rpd.itSpawnType = CIT_STICK;
    }

    {
      ci CI;
      CI.bAllowMeltables=false;
      if(ToSplit==NULL && choseOneIngredient<stone>(rpd,&CI)){
        ToSplit = game::SearchItem(rpd.ingredientsIDs[0]);
        if(!setTools(rpd,FindBluntTool(rpd),findCarvingTool(rpd,ToSplit))){ //the blunt is to hit the cutting one
          explain(rpd,ToSplit->GetName(INDEFINITE));
          return false;
        }
//        bool bHasAllTools=true;
//        addTool(rpd,FindBluntTool(rpd));
//        //        if(!reqCut(rpd,ToSplit->GetName(INDEFINITE)))
//        addTool(findCarvingTool(rpd,ToSplit));
//        if(!findCarvingTool(rpd,ToSplit)){
//          explain(rpd,ToSplit->GetName(INDEFINITE));
//          return false;
//        }
        rpd.itSpawnType = CIT_STONE;
      }
    }

    if(ToSplit==NULL)
      return false;

    if(craftcore::IsDegraded(ToSplit)){
      rpd.bAlreadyExplained=true;
      return false;
    }

    long volM=ToSplit->GetMainMaterial()->GetVolume();

    festring fsInfo;
    ToSplit->AddInventoryEntry(rpd.rc.H(),fsInfo,1,true);
    long totParts = game::NumberQuestion(festring()+"Split "+fsInfo+" in how many parts?", WHITE, true);
    if(totParts<=1)
      return false;

    long volPart = volM/totParts;
    if(volPart < 1){
      ADD_MESSAGE("The splitted part must have some volume.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    long volRest = volM%totParts; //gradative work should take care of it

    rpd.CopySpawnItemCfgFrom(ToSplit);
    rpd.itSpawnMatMainVol = volPart;
    rpd.itSpawnTot = totParts;

    float fTotTurns=0;
    for(int i=1;i<=totParts;i++){ //TODO should be based on cut area/length
      float fCurrentPartVol=volM/(float)i;
      float fTurns=fCurrentPartVol/100000.0; //bear is 150000cm3
      fTotTurns+=fTurns; //turn time is one minute but when fighting it also spends 1 min... w/e...
    }
    rpd.iBaseTurnsToFinish = fTotTurns;
    if(rpd.iBaseTurnsToFinish<1)
      rpd.iBaseTurnsToFinish=1;

    rpd.bGradativeCraftOverride=true;

    rpd.bAlreadyExplained=true; //no need to say anything

    rpd.bCanStart = true;

    return true;
  }
};srpSplitLump rpSplitLump;

struct srpForgeItem : public recipe{
  virtual void fillInfo(){
    init("forge","an item");
    desc << "Using a blunt weapon as hammer, close to an anvil and with a forge nearby you can create items.";
  }

  virtual bool work(recipedata& rpd){DBGLN;
    //////////////// let user type the item name
    static festring Default; //static to help on reusing! like creating more of the same
//    if(rpd.itSpawn!=NULL) // this is important to grant the cleanup
//      ABORT("item to be crafted should be NULL %d %s %s",rpd.itSpawn->GetID(),rpd.itSpawn->GetNameSingular().CStr(),rpd.dbgInfo().CStr());

    item* itSpawn = NULL;
    for(;;){
      festring Temp;
      Temp << Default;DBG4(Default.CStr(),Default.GetSize(),Temp.CStr(),Temp.GetSize()); // to let us fix previous instead of having to fully type it again

      if(game::DefaultQuestion(Temp, CONST_S("What do you probably want to create?"), Default, true) == ABORTED){DBGLN;
        /**
         * The wishing system will try to guess what item matches whatever we type,
         * so it may not be what we typed exactly...
         */
        break;
      }DBG1(Temp.CStr());

      itSpawn = protosystem::CreateItemToCraft(Temp);DBGLN;

      if(itSpawn!=NULL){DBGLN;
        if(craftcore::canBeCrafted(itSpawn)){DBG4("SendingToHellRejectedCraftItem",itSpawn->GetID(),itSpawn->GetNameSingular().CStr(),itSpawn);
          rpd.itSpawnType = CIT_PROTOTYPE;
          rpd.fsItemSpawnSearchPrototype = Temp;
          break;
        }else{
          festring fsDoWhat = "I don't have the power required to enchant";
          if(itSpawn->GetCategory()==FOOD)fsDoWhat="I never learned how to cook"; //TODO sounds weird for kiwi/banana TODO could at least roast stuff TODO needs seeds to create flour and bake breads
          if(itSpawn->GetCategory()==BOOK)fsDoWhat="I don't have time to write";
          ADD_MESSAGE("%s %s!",fsDoWhat.CStr(),itSpawn->GetName(INDEFINITE).CStr()); //itCreate->GetNameSingular());//
          craftcore::SendToHellSafely(itSpawn);
          itSpawn = NULL; //IMPORTANT!!! if user press ESC...
        }
      }else{
        ADD_MESSAGE("I don't know how to create %s.",Temp.CStr());
      }

      Default.Empty();DBG1(Default.CStr());
      Default << Temp;
    }

    if(itSpawn==NULL){
      rpd.bAlreadyExplained=true; //actually was just cancelled by user
      return false;
    }

    if(Default == itSpawn->GetName(INDEFINITE)) //TODO compare for EQUAL ignoring case
      ADD_MESSAGE("Now I need the material(s) to create a %s.",itSpawn->GetName(INDEFINITE).CStr());
    else
      ADD_MESSAGE("Now I need the material(s) to create a %s as I would probably create %s.",Default.CStr(),itSpawn->GetName(INDEFINITE).CStr());

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

    bool bM = false;
    festring fsM("as MAIN material");DBGLN;
    if(!bM){
      ci CI = CIM;
      CI.iReqCfg=INGOT; //meltables
      bM = choseIngredients<stone>(fsM,lVolM, rpd, iCfgM, CI);
    }
    if(!bM){
      ci CI = CIM; //carving: only one stone per material allowed, so it must have required volume
      CI.bFirstItemMustHaveFullVolumeRequired=true;
      CI.bMultSelect=false;
      bM = choseIngredients<stone>(fsM,lVolM, rpd, iCfgM, CI);
    }
    if(!bM){
      ci CI = CIM;
      bM = choseIngredients<bone>(fsM,lVolM, rpd, iCfgM, CI);
    }
    if(!bM){
      ci CI = CIM;
      bM = choseIngredients<stick>(fsM,lVolM, rpd, iCfgM, CI);
    }
    if(!bM){
      ADD_MESSAGE("I will craft it later...");
      rpd.bAlreadyExplained=true;
      craftcore::SendToHellSafely(itSpawn);
      return false;
    }

    bool bContainerEmptied = craftcore::EmptyContentsIfPossible(rpd,itSpawn);

    /**
     * TODO problem: basically sulphuric acid can already be stored on a metal can ...
     * TODO every materialcontainer should rust depending on it's contents, if made of anything else than glass
     * Keeping allowing creating materialcontainer of non glass because the fix is already required for the existing metal can,
     * so preventing it would still not fix how 'metal can' works...
     */

    bool bIsWeapon = itSpawn->IsWeapon(rpd.rc.H());
    bool bReqS = bIsWeapon;
    bool bAllowS = true;
//    if(mc)bAllowS=false;
    if(bContainerEmptied)bAllowS=false;
    if(lVolS==0)bAllowS=false;
    if(bAllowS){DBGLN;
      bool bS = false;
      festring fsS("as Secondary material");DBGLN;
      if(!bS){
        ci CI=CIS;
        CI.iReqCfg=INGOT;
        bS = choseIngredients<stone>(fsS,lVolS, rpd, iCfgS, CI);
      }
      if(!bS){
        ci CI=CIS; //carving: only one stone per material allowed, so it must have required volume
        CI.bFirstItemMustHaveFullVolumeRequired=true;
        CI.bMultSelect=false;
        bS = choseIngredients<stone>(fsS,lVolS, rpd, iCfgS, CI);
      }
      if(bIsWeapon){DBGLN; //this is mainly to prevent mc being filled with non-sense materials TODO powders one day would be ok
        if(!bS){
          ci CI=CIS;
          bS = choseIngredients<bone>(fsS,lVolS, rpd, iCfgS, CI);
        }
        if(!bS){
          ci CI=CIS;
          bS = choseIngredients<stick>(fsS,lVolS, rpd, iCfgS, CI);
        }
      }

      if(!bS){
        ADD_MESSAGE("I will craft it later...");
        rpd.bAlreadyExplained=true;
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }

    if(bReqS && !bAllowS)
      ABORT("item reqs secondary mat but doesnt allow it??? %s",itSpawn->GetName(DEFINITE).CStr());

    rpd.bHasAllIngredients=true;

    rpd.bCanBeBroken = itSpawn->CanBeBroken();

    itSpawn->SetMainMaterial(material::MakeMaterial(iCfgM,lVolM));
    if(bAllowS)
      itSpawn->SetSecondaryMaterial(material::MakeMaterial(iCfgS,lVolS));

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

    if(rpd.bMeltable){
      if(!recipe::findOLT(rpd,FORGE,true)){
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }

      if(!recipe::findOLT(rpd,ANVIL,true)){
        craftcore::SendToHellSafely(itSpawn);
        return false;
      }
    }

    // HAMMER like for meltables (still hot and easy to work, any hammer will do) TODO damage the hammer thru the heat of the forge
    bool bMissingTools=false;
    if(rpd.bMeltable){ //TODO glass should require proper tools (don't know what but sure not a hammer)
      rpd.itTool = FindBluntTool(rpd);
      if(rpd.itTool==NULL)
        bMissingTools=true;
    }

    // DAGGER for carving
    if(!bMissingTools){
      if(!bMeltableM || !bMeltableS){
        if(!findCarvingTool(rpd,itSpawn))
          bMissingTools=true;
      }
    }

    DBG1(rpd.iBaseTurnsToFinish);

    if(bMissingTools){
      rpd.itTool=rpd.itTool2=NULL; //to make it easy to check/inform, wont start if missing one anyway
      craftcore::SendToHellSafely(itSpawn);
      return false;
    }

    //TODO are these difficulties below well balanced (not cheap neither excessive/notFun)?

    for(;;){ // just to break on match
      whistle* itWist = dynamic_cast<whistle*>(itSpawn);
      if(itWist){
        rpd.fDifficulty = 3.5;
        break;
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

        break;
      }

      stethoscope* itSte = dynamic_cast<stethoscope*>(itSpawn);
      if(itSte){
        rpd.fDifficulty = 3.5;
        break;
      }

      meleeweapon* itMW = dynamic_cast<meleeweapon*>(itSpawn);
      if(itMW!=NULL){ //TODO are these difficulties below good?
        rpd.fDifficulty = 2.0;
        if(itMW->IsTwoHanded())
          rpd.fDifficulty = 3.5;
        break;
      }

      shield* itSH = dynamic_cast<shield*>(itSpawn);
      if(itSH!=NULL){
        rpd.fDifficulty = 2.5;
        break;
      }

      armor* itAR = dynamic_cast<armor*>(itSpawn);
      if(itAR!=NULL){
        rpd.fDifficulty = 1.5;
        bodyarmor* itBAR = dynamic_cast<bodyarmor*>(itSpawn);
        if(itBAR!=NULL)
          rpd.fDifficulty = 3.0;
        break;
      }

      break; //block trick exiter
    }

    { // special extra turns (dont use difficulty to calc turns as it is a per-turn check)
      int iBaseTTF = rpd.iBaseTurnsToFinish;

      if(!rpd.bMeltable) //carving
        rpd.iBaseTurnsToFinish += iBaseTTF;

      if(!rpd.bCanBeBroken){ //after created will be granted til the end of the game...
        rpd.iBaseTurnsToFinish += iBaseTTF*10;
      }

      if(!rpd.bMeltable)
        if(!itSpawn->CanBeBurned()) //another special quality
          rpd.iBaseTurnsToFinish += iBaseTTF*10;
    }

//    rpd.rc.SetCanBeSuspended();

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
    if(rpd.itTool==NULL)
      return false;

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

    mat = itBottle->GetSecondaryMaterial();
    if(mat!=NULL)
      mat->GetEffectStrength(); //TODO mmm seems to have no strengh diff? only takes more time if "stronger" like not from large spider

    rpd.itSpawnType = CIT_POTION;
    rpd.itSpawnCfg = itBottle->GetConfig(); //may be a vial
    rpd.itSpawnMatMainCfg = itBottle->GetMainMaterial()->GetConfig();
    rpd.itSpawnMatMainVol = itBottle->GetMainMaterial()->GetVolume();
    rpd.itSpawnMatSecCfg = iLiqCfg;
    rpd.itSpawnMatSecVol = volume;
//    rpd.itSpawn = potion::Spawn(itBottle->GetConfig()); //may be a vial
//    rpd.itSpawn->SetSecondaryMaterial(liquid::Spawn(iLiqCfg, volume));
//    rpd.SetSpawnItemCfg(rpd.itSpawn);

    rpd.ingredientsIDs.push_back(itBottle->GetID()); //just to be destroyed too if crafting completes
    rpd.ingredientsIDs.push_back(itCorpse->GetID());
    rpd.bHasAllIngredients=true;

    rpd.iBaseTurnsToFinish=5;

    rpd. bCanStart=true;

    return true;
  }
};

struct srpPoison : public srpFluidsBASE{

  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Effect==EFFECT_POISON || flesh->Effect==EFFECT_POISON);
  }

  virtual void fillInfo(){
    init("extract","some poison");
    desc << "Use a " << fsTool << " to " << action << " " << name << " from "
      << fsCorpse << " into a " << fsBottle <<  ".";
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
    init("extract","some acidous fluid");
    desc   << "Use a " << fsTool << " to " << action   << " " << name   << " from "
      << fsCorpse << " into a " << fsBottle <<  ".";
  }

  virtual bool work(recipedata& rpd){
    iLiqCfg=SULPHURIC_ACID;

    return srpFluidsBASE::work(rpd);
  }
};srpAcid rpAcid;

felist craftRecipes(CONST_S("What do you want to craft?"));
std::vector<recipe*> vrp;
void addRecipe(recipe* prp){
  prp->iListIndex=vrp.size();
  if(prp->name.IsEmpty())
    ABORT("empty recipe name '%s' '%s' %d",prp->name.CStr(),prp->desc.CStr(),prp->iListIndex);
  craftRecipes.AddEntry(prp->name+" - "+prp->desc, LIGHT_GRAY, 20, prp->iListIndex, true); DBG2(prp->name.CStr(),prp->iListIndex);
  vrp.push_back(prp);
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
truth craftcore::Craft(character* Char) //TODO currently this is an over simplified crafting system... should be easy to add recipes and show their formulas...
{
//  craftcore::reinitIfNeeded();
  if(craftcore::HasSuspended()){
    /**
     * TODO
     * do this only for suspendable recipes but:
     * - ingredients must be stored in an inaccessible place
     * - crafted spawning item too
     *
     * ex.: so it wont prevent extracting poison if you were melting mithril
     */

    if(game::TruthQuestion(festring("Continue crafting? ['n' to cancel this crafting]"),YES)){
      craftcore::ResumeSuspendedTo(Char);
      return true;
    }else{
//      craftcore::TerminateSuspended();
      craftcore::SetSuspended(NULL); //discards it
    }
  }

  humanoid* h = dynamic_cast<humanoid*>(Char);
  if(h==NULL){
    ADD_MESSAGE("This monster type cannot craft.");
    return false;
  }

  bool bLOk = h->GetLeftArm() !=NULL && h->GetLeftArm()->IsUsable();
  bool bROk = h->GetRightArm()!=NULL && h->GetRightArm()->IsUsable();
  if(!bLOk && !bROk){
    ADD_MESSAGE("I need at least one usable arm to be able to craft.");
    return false;
  }

  int iCraftTimeMult=1;
  if(!bLOk || !bROk){ //using only one hand will take more time
    iCraftTimeMult++;
  }


  uint sel = FELIST_ERROR_BIT;
  if(vrp.size()>0){
    game::SetStandardListAttributes(craftRecipes);
    craftRecipes.AddFlags(SELECTABLE);
    sel = craftRecipes.Draw(); DBG1(sel);

    if(sel & FELIST_ERROR_BIT)
      return false;
  }
  recipedata rpd(h,sel);

  /******************************************************************************************
   * 1st call it just initializes the recipes list after all recipes have been configured!
   */
  bool bInitRecipes = vrp.size()==0;
  recipe* prp=NULL;
  #define RP(rp) \
    if(prp==NULL && rp.IsTheSelectedOne(rpd))prp=&rp; \
    DBG3(prp,&rp,rp.desc.CStr()); \
    if(bInitRecipes)addRecipe((recipe*)&rp);
  // these are kind of grouped and not ordered like a-z
  RP(rpChair);
  RP(rpDoor);

  RP(rpWall2);

  RP(rpPoison);
  RP(rpAcid);

  RP(rpDismantle);
  RP(rpSplitLump);
  RP(rpJoinLumps);

  RP(rpMelt);
  RP(rpForgeItem);
  RP(rpAnvil);
  RP(rpForge);

  RP(rpWorkBench);
  if(bInitRecipes)
    return Craft(Char); //init recipes descriptions at least, one time recursion :>

  if(prp==NULL){DBGLN;
    return false;
  }DBGLN;

  bool bDummy = prp->work(rpd); //bDummy as there is more detailed fail status from rpd bools

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
      ADD_MESSAGE("Let me see.. I will use %s as tool(s).",fsTools.CStr());

    if(rpd.otSpawnType!=CTT_NONE || rpd.itSpawnType!=CIT_NONE) {
      if(rpd.itTool !=NULL && rpd.itTool ->IsBroken())
        iCraftTimeMult++;
      if(rpd.itTool2!=NULL && rpd.itTool2->IsBroken())
        iCraftTimeMult++;

      if(rpd.iBaseTurnsToFinish<1)
        ABORT("invalid iBaseTurnsToFinish %d",rpd.iBaseTurnsToFinish);

      DBGEXEC( //solved, the problem was the duplicate item code that modifies the duplicated ID ...
        for(int iDbg123=0;iDbg123<rpd.ingredientsIDs.size();iDbg123++){
          item* itDbg123=game::SearchItem(rpd.ingredientsIDs[iDbg123]);
          if(itDbg123==NULL)ABORT("ingredient id %d vanished?",rpd.ingredientsIDs[iDbg123]);
        }
      );DBG1(rpd.iBaseTurnsToFinish);

      rpd.iBaseTurnsToFinish*=iCraftTimeMult;

      /**
       * LAST turn calc thing!!!
       * ex.: dex=10 wis=10 -> 1.0
       */
      rpd.iBaseTurnsToFinish /=
        ( Char->GetAttribute(WISDOM   )/10.0 +
          Char->GetAttribute(DEXTERITY)/10.0   ) / 2.0;
      craftcore::CraftSkill(Char);
      if(rpd.iBaseTurnsToFinish==0)
        rpd.iBaseTurnsToFinish=1;

      if(rpd.v2PlaceAt.Is0())
        rpd.v2PlaceAt = rpd.lsqrPlaceAt!=NULL ? rpd.lsqrPlaceAt->GetPos() : rpd.lsqrCharPos->GetPos(); //may be ignored anyway, is just a fallback

      rpd.iAddDexterity=5; //TODO crafting difficult things should give more dexterity (wisdom too?)

      rpd.v2PlayerCraftingAt = Char->GetPos();

      if(rpd.itTool!=NULL && rpd.itTool2!=NULL)
        if(rpd.itTool==rpd.itTool2)
          ABORT("both tools are the same item %d:%s %d:%s",rpd.itTool->GetID(),rpd.itTool->GetName(INDEFINITE).CStr(),rpd.itTool2->GetID(),rpd.itTool2->GetName(INDEFINITE).CStr());
      if(rpd.itTool !=NULL)rpd.itToolID =rpd.itTool ->GetID();
      if(rpd.itTool2!=NULL)rpd.itTool2ID=rpd.itTool2->GetID();

      if(rpd.lsqrPlaceAt!=NULL)rpd.v2BuildWhere=rpd.lsqrPlaceAt->GetPos();

      rpd.iRemainingTurnsToFinish = rpd.iBaseTurnsToFinish;

      rpd.ClearRefs(); //pointers must be revalidated on the action handler

      Char->SwitchToCraft(rpd); // everything must be set before this!!!

      ADD_MESSAGE("Let me work on %s now.",prp->name.CStr());
    }else{
      ABORT("requested to craft nothing? %s",rpd.dbgInfo().CStr());
    }

    return true; //spends current turn
  }else{
    if(!rpd.bAlreadyExplained){
      if(rpd.lsqrPlaceAt!=NULL && !rpd.bCanBePlaced)
        ADD_MESSAGE("%s can't be placed here!",prp->name.CStr());
      else if(!rpd.bHasAllIngredients){
        festring fsMsg;
        fsMsg<<"Required ingredients to "<<prp->action<<" "<<prp->name<<" are not met.";
        ADD_MESSAGE(fsMsg.CStr());
      }else if(rpd.itTool == NULL){
        ADD_MESSAGE("Required tool is missing.");
      }else{
        ABORT("explain why crafting won't work.");
      }
    }
  }

  /**
   * ATTENTION!!!
   * the (crafting code) complexity of granting one turn wont be lost isnt worth letting the game crash randomly somewhere
   * that is hard to track the source of the problem...
   * so to ALWAYS spend current turn, when things sent to hell will properly be applied everywhere required
   * is the SAFEST thing! dont change this please even if you are sure all looks perfect! ;)
   if(rpd.bSpendCurrentTurn)return true;else return false; //old code
   */
  return true;
}


item* craftcore::CheckBreakItem(bool bAllowBreak, recipedata& rpd, item* itSpawn, festring& fsCreated)
{
  bool bBreak = rpd.bSpawnBroken;

  if(bAllowBreak && bBreak && !itSpawn->IsBroken()){
    if(itSpawn->CanBeBroken()){
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
      itSpawn->SetConfig(rpd.itSpawnCfg | BROKEN);
      itSpawn->SetSize(itSpawn->GetSize() >> 1);
    }else{
      /**
       * things that can't be broken are special.
       * if it can't be broken, will just create a messy lump.
       */
      ADD_MESSAGE("My lack of skill broke %s into pieces...",itSpawn->GetName(DEFINITE).CStr());
      craftcore::PrepareRemains(itSpawn->GetMainMaterial(), rpd);
      if(itSpawn->GetSecondaryMaterial()!=NULL)
        craftcore::PrepareRemains(itSpawn->GetSecondaryMaterial(), rpd);
      craftcore::SendToHellSafely(itSpawn);
      itSpawn=NULL; //because the result can be 2 items (2 lumps!)
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
////material* craftcore::CreateMaterial(bool bMain,recipedata* prpd,material* matOverride)
//material* craftcore::CreateMaterial(bool bMain,recipedata* prpd)
//{
////  if(prpd==NULL && matOverride==NULL)
////    ABORT("to create material, one must not be NULL");
////
//  material* mat = material::MakeMaterial(
//    (bMain ? prpd->itSpawnMatMainCfg : prpd->itSpawnMatSecCfg),
//    (bMain ? prpd->itSpawnMatMainVol : prpd->itSpawnMatSecVol)
//  );
//
////  if(mat!=NULL){
////    if(dynamic_cast<organic*>(mat)!=NULL){
////      ((organic*)mat)->SetSpoilLevel(
////        (bMain ? prpd->itSpawnMatMainSpoilLevel : prpd->itSpawnMatSecSpoilLevel) );
////    }
////  }
//
//  return mat;
//}
//material* craftcore::CreateMaterial(bool bMain,recipedata* prpd,material* matOverride)
//{
//  if(prpd==NULL && matOverride==NULL)
//    ABORT("to create material, one must not be NULL");
//
//  material* mat = material::MakeMaterial(
//    matOverride!=NULL ? matOverride->GetConfig() : (bMain ? prpd->itSpawnMatMainCfg : prpd->itSpawnMatSecCfg),
//    matOverride!=NULL ? matOverride->GetVolume() : (bMain ? prpd->itSpawnMatMainVol : prpd->itSpawnMatSecVol)
//  );
//
//  if(mat!=NULL){
//    mat->SetSpoilLevel(
//      matOverride!=NULL ? matOverride->GetSpoilLevel() : (bMain ? prpd->itSpawnMatMainSpoilLevel : prpd->itSpawnMatSecSpoilLevel) );
//  }
//
//  return mat;
//}

item* craftcore::SpawnItem(recipedata& rpd, festring& fsCreated)
{
  rpd.rc.integrityCheck();

  item* itSpawn = NULL;
  material* matS = NULL;
  bool bAllowBreak=false;
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

  if(!craftcore::canBeCrafted(itSpawn)){
    ABORT(
      "Dear developer, for the sake of balance and challenge do not create recipes for:\n"
      "- Quest items.\n"
      "- Magical items as rings, amulets, wands, scrolls, horns etc.\n"
      "Crafting any of this would be unbalanced as hell and unrealistic given your characters upbringing.\n"
      "You're after all a slave, with no knowledge of magic, and crafting magical items should be beyond most craftsmen.\n"
    );
  }

  if(rpd.itSpawnMatMainCfg==0 || rpd.itSpawnMatMainVol==0)
    ABORT("main material and/or volume is 0 %s %s",rpd.itSpawnMatMainCfg,rpd.itSpawnMatMainVol);

//  material* matM = material::MakeMaterial(rpd.itSpawnMatMainCfg,rpd.itSpawnMatMainVol);
//  matM->SetSpoilCounter(rpd.itSpawnMatMainSpoilLevel);
  material* matM = CreateMaterial(true,rpd);
  itSpawn->SetMainMaterial(matM);

  if(rpd.itSpawnMatSecCfg==0)
    craftcore::EmptyContentsIfPossible(rpd,itSpawn);
  else{
    if(matS==NULL)
      matS = CreateMaterial(false,rpd);
//      matS = material::MakeMaterial(rpd.itSpawnMatSecCfg,rpd.itSpawnMatSecVol);
    if(matS!=NULL){
//      matS->SetSpoilCounter(rpd.itSpawnMatSecSpoilLevel);
      itSpawn->SetSecondaryMaterial(matS);
    }
  }

  itSpawn->MoveTo(rpd.rc.H()->GetStack());

  if(!fsCreated.IsEmpty())
    fsCreated << " ";
  fsCreated << "I crafted ";

  itSpawn = CheckBreakItem(bAllowBreak, rpd, itSpawn, fsCreated);
  if(itSpawn!=NULL){
    fsCreated << itSpawn->GetName(INDEFINITE);
    itSpawn->MoveTo(rpd.rc.H()->GetStack());DBGLN;
  }

  return itSpawn;
}

void craftcore::CraftWorkTurn(recipedata& rpd){ DBG1(rpd.iRemainingTurnsToFinish);
  rpd.iRemainingTurnsToFinish--;
  rpd.bSuccesfullyCompleted = rpd.iRemainingTurnsToFinish==0;

  if(rpd.bGradativeCraftOverride)
    GradativeCraftOverride(rpd);

  if(rpd.bSuccesfullyCompleted && !rpd.bGradativeCraftOverride)
  {DBGLN;
    festring fsMsg("");

    if(rpd.itSpawnType!=CIT_NONE){DBGLN;
      craftcore::SpawnItem(rpd,fsMsg);
    }

    if(rpd.otSpawnType!=CTT_NONE){DBGLN;
      craftcore::SpawnTerrain(rpd,fsMsg);
    }

    fsMsg << craftcore::DestroyIngredients(rpd);
    fsMsg << ".";

    ADD_MESSAGE(fsMsg.CStr());
  }
}

/**
 * ex.:
 * - a big meltable lump into ingots
 * - a big stick into tiny ones
 * - a rock into smaller ones
 */
void craftcore::GradativeCraftOverride(recipedata& rpd){DBGLN;
  if(rpd.ingredientsIDs.size()!=1)
    ABORT("incompatible gradative mode and ingredients count %d",rpd.ingredientsIDs.size());

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
    rpdTmp.fDifficulty=1.1; //just a little bit difficult then to allow fumble
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
    item* itSpawned = craftcore::SpawnItem(rpdTmp,fsMsg);

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
      ADD_MESSAGE("I worked well and gained %d minutes!",rpd.iRemainingTurnsToFinish); //TODO despite this actually means a lack of calc precision :P, not less fun tho :)
      rpd.iRemainingTurnsToFinish=0; //overriding for consistency.
    }
  }

  if(spawnedVol==matMRemVol)
    DestroyIngredients(rpd); //cant be left with 0 volume
  else
  if(spawnedVol>matMRemVol) //the total calc being precise, and leaving a tiny remaining lump behind, this will never happen, just in case something is changed...
    ABORT("lump volume would become negative (if not unsigned long) or 0 as %d > %d",spawnedVol,matMRemVol);
  else
    matM->SetVolume(matMRemVol-spawnedVol); //the remaining volume becomes the action control/limit

  matMRemVol=matM->GetVolume();
  DBG7(iSpawnNow,spawnedVol,matMRemVol,rpd.itSpawnTot,rpd.iBaseTurnsToFinish,rpd.iRemainingTurnsToFinish,fRemain);
}

void craftcore::CheckFumble(recipedata& rpd,bool bChangeTurns)
{
  rpd.rc.integrityCheck();
//  if(!v2XplodAt.Is0()){DBGSV2(v2XplodAt);
  if(rpd.fDifficulty>1.0){
    int xplodXtra=0;
    for(int i=0;i<rpd.iStrongerXplod;i++)
      xplodXtra+=clock()%5;

    /**
     * To fumble, base reference is 15% chance at a craft skill of 20.
     * ex.: Craft skill of 10 will have 30% fumble chance.
     */
    int iFumblePower=0;
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
    bool bCriticalFumble=false;
    if(iLuckPerc<=1){
      iFumblePower++; //always have 1% weakest xplod chance
      bCriticalFumble=true;
    }
    //current max chance per round of spawning broken is 5%
    if(clock()%100<=iFumblePower){
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

    DBG9(fBaseCraftSkillToNormalFumble,iFumbleBase,iLuckPerc,iFumblePower,bXplod,rpd.xplodStr,rpd.bCanBeBroken,rpd.bSpawnBroken,rpd.iBaseTurnsToFinish);
  }
}

void craftcore::CheckIngredients(recipedata& rpd){
  rpd.rc.integrityCheck();
  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL){ //ABORT("ingredient id %d not found",rpd.ingredientsIDs[i]);
      /**
       * ex.: if something catches fire and is destroyed before the crafting ends.
       */
      ADD_MESSAGE("a required ingredient was destroyed...");DBG1(rpd.ingredientsIDs[i]);
      rpd.bFailed=true; //TODO a crash happens in this line, how? memory corruption? if the tiny explosions trigger things on the floor like wands
      break;
    }

    // a magpie or siren may have taken it
    if(it->GetSquareUnder()!=rpd.lsqrActor){
      ADD_MESSAGE("%s ingredient went missing...",it->GetName(DEFINITE).CStr());
      rpd.bFailed=true;
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

void craftcore::CheckFacilities(recipedata& rpd){
  rpd.rc.integrityCheck();
  rpd.lvl = rpd.lsqrActor->GetLevel();
  if(!rpd.v2AnvilLocation.Is0()){
    olterrain* ot = rpd.lvl->GetLSquare(rpd.v2AnvilLocation)->GetOLTerrain();
    if(ot==NULL || ot->GetConfig()!=ANVIL){
      ADD_MESSAGE("The anvil was destroyed!");
      rpd.bFailed=true;
    }

    rpd.v2XplodAt=rpd.v2AnvilLocation;
  }else
  if(!rpd.v2ForgeLocation.Is0()){
    olterrain* otForge = rpd.lvl->GetLSquare(rpd.v2ForgeLocation)->GetOLTerrain();
    if(otForge==NULL || otForge->GetConfig()!=FORGE){
      ADD_MESSAGE("The forge was destroyed!");
      rpd.bFailed=true;
    }

    rpd.v2XplodAt=rpd.v2ForgeLocation;
  }else
  if(!rpd.v2WorkbenchLocation.Is0()){
    olterrain* otWorkbench = rpd.lvl->GetLSquare(rpd.v2WorkbenchLocation)->GetOLTerrain();
    if(otWorkbench==NULL || otWorkbench->GetConfig()!=WORK_BENCH){
      ADD_MESSAGE("The workbench was destroyed!");
      rpd.bFailed=true;
    }

    rpd.bOnlyXplodIfCriticalFumble=true;
    rpd.v2XplodAt=rpd.v2WorkbenchLocation;
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

void craftcore::CheckTools(recipedata& rpd){
  rpd.lsqrActor = rpd.rc.H()->GetLSquareUnder(); DBGSV2(rpd.lsqrActor->GetPos());

  if(rpd.itToolID>0){
    rpd.itTool=checkTool(rpd.itToolID,rpd.lsqrActor);
    if(rpd.itTool==NULL)
      rpd.bFailed=true;
  }

  if(rpd.itTool2ID>0){
    rpd.itTool2=checkTool(rpd.itTool2ID,rpd.lsqrActor);
    if(rpd.itTool2ID>0 && rpd.itTool2==NULL)
      rpd.bFailed=true;
  }

  if(rpd.bFailed)
    rpd.itTool=rpd.itTool2=NULL; //consistency or have the two (if required), or have none

//  if(rpd.itToolID!=0){
//    rpd.itTool = game::SearchItem(rpd.itToolID); //must keep searching it as it may have been destroyed.
//    //TODO if a tool was broken and gets fixed, it's old ID will vanish!!! how to handle it!??!?!
//    if(rpd.itTool==NULL){
//      ADD_MESSAGE("The unmodified tool to craft this is missing or destroyed.");//,rpd.itTool->GetName(DEFINITE).CStr());
//      rpd.bFailed=true;
//    }
//    DBGEXEC(if(rpd.itTool!=NULL)DBGSV2(rpd.itTool->GetLSquareUnder()->GetPos()));
//  }
//  if(rpd.itTool2ID!=0){
//    rpd.itTool2 = game::SearchItem(rpd.itTool2ID); //must keep searching it as it may have been destroyed.
//    //TODO if a tool was broken and gets fixed, it's old ID will vanish!!! how to handle it!??!?!
//    if(rpd.itTool2==NULL){
//      ADD_MESSAGE("The unmodified tool to craft this is missing or destroyed.");//,rpd.itTool2->GetName(DEFINITE).CStr());
//      rpd.bFailed=true;
//    }
//    DBGEXEC(if(rpd.itTool2!=NULL)DBGSV2(rpd.itTool2->GetLSquareUnder()->GetPos()));
//  }

  DBG8(rpd.itToolID,rpd.itTool,rpd.itTool2ID,rpd.itTool2,rpd.itSpawnCfg,rpd.otSpawnCfg,rpd.itSpawnType,rpd.otSpawnType);

//  if(rpd.itTool!=NULL && rpd.itTool->GetLSquareUnder()!=rpd.lsqrActor)//rpd.itTool!=Actor->GetMainWielded())
//  {DBGLN; //TODO re-mainWield it
//    ADD_MESSAGE("%s went missing.",rpd.itTool->GetName(DEFINITE).CStr());
//    rpd.bFailed=true;
//  }
//  if(rpd.itTool2!=NULL && rpd.itTool2->GetLSquareUnder()!=rpd.lsqrActor)//rpd.itTool!=Actor->GetMainWielded())
//  {DBGLN; //TODO re-mainWield it
//    ADD_MESSAGE("%s went missing.",rpd.itTool2->GetName(DEFINITE).CStr());
//    rpd.bFailed=true;
//  }

}

void craftcore::CheckEverything(recipedata& rpd){
  if(!rpd.bFailed)
    craftcore::CheckTools(rpd);

  if(rpd.rc.H()->GetPos()!=rpd.v2PlayerCraftingAt){ //in case player is teleported
    ADD_MESSAGE("I need to move back to where I started crafting.");
    rpd.bFailed=true;
  }

  rpd.rc.integrityCheck();
  DBGSV2(rpd.v2PlaceAt);
  if(!rpd.bFailed){
    rpd.lsqrPlaceAt = rpd.rc.H()->GetNearLSquare(rpd.v2PlaceAt); //near? it is abs map pos...
    olterrain* oltExisting = rpd.lsqrPlaceAt->GetOLTerrain();
    if(rpd.otSpawnType!=CTT_NONE && oltExisting!=NULL){DBGLN;
  //  ADD_MESSAGE("%s cannot be placed there.", rpd.otSpawn->GetName(DEFINITE).CStr()); //TODO like in case something is placed there before ending the construction?
      ADD_MESSAGE("Unable to place it there."); //TODO like in case something is placed there before ending the construction? but what and how?
      rpd.bFailed=true;
    }
  }

  if(!rpd.bFailed)
    craftcore::CheckIngredients(rpd);

  if(!rpd.bFailed)
    craftcore::CheckFacilities(rpd);

  if(!rpd.bFailed)
    craftcore::CheckFumble(rpd);
}

cfestring craftcore::DestroyIngredients(recipedata& rpd){
  festring fsIng,fsIngP;
  festring fsIngPrev,fsIngPPrev;
  int iCountEqual=1;
  festring fsIngMsg("");
  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL)ABORT("ingredient id %d not found %s",rpd.ingredientsIDs[i],rpd.dbgInfo().CStr());
    it->RemoveFromSlot();DBGLN;

    bool bSendToHell=true;
    if(rpd.otSpawnMatMainCfg!=0)
      if(it->GetMainMaterial()->GetConfig() != rpd.otSpawnMatMainCfg)
        if(rpd.rc.H()->GetPos()!=rpd.lsqrPlaceAt->GetPos()) //TODO this shouldnt be necessary, CanMoveOn() should suffice..
          if(!rpd.rc.H()->CanMoveOn(rpd.lsqrPlaceAt)) //keep inaccessible ingredients TODO but as ghost they would still be recoverable...
            bSendToHell=false;

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

item* craftcore::PrepareRemains(material* mat, recipedata& rpd)
{
  bool bWoodenCreateStick = IsWooden(mat);

  if(mat==NULL)
    ABORT("NULL lump material");

  DBG2(mat->GetName(DEFINITE).CStr(),mat->GetVolume());
  bool bLiquid = mat->IsLiquid();
  if(mat->IsPowder())bLiquid=false; //TODO if explosive could have a chance to xplod use the fumble function TODO generalize it

  if(dynamic_cast<gas*>(mat)!=NULL)return NULL; //TODO should have a chance to release the gas effect if not 100% :)

  if(bLiquid){
    rpd.rc.H()->SpillFluid(NULL,liquid::Spawn(mat->GetConfig(),mat->GetVolume()));
  }else{
    item* itTmp = NULL;
    if(bWoodenCreateStick){
      itTmp = stick::Spawn(0, NO_MATERIALS);
    }else{
      itTmp = lump::Spawn(0, NO_MATERIALS);
    }
//    itTmp->SetMainMaterial(material::MakeMaterial(mat->GetConfig(),mat->GetVolume()));
    itTmp->SetMainMaterial(CreateMaterial(mat));
    rpd.rc.H()->GetStack()->AddItem(itTmp);
    ADD_MESSAGE("%s was recovered.", itTmp->GetName(DEFINITE).CStr());
    return itTmp;
  }

  return NULL;
}

bool craftcore::IsWooden(material* mat){
  if(IsMeltable(mat))
    return false;
  if(mat->GetConfig()>=FUNGI_WOOD && mat->GetConfig()<=PETRIFIED_WOOD)
    return true;
  return false;
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
