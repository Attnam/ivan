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

bool craftcore::EmptyContentsIfPossible(item* itContainer)
{
  materialcontainer* mc = dynamic_cast<materialcontainer*>(itContainer); //potions, mines... also bananas xD
  itemcontainer* ic = dynamic_cast<itemcontainer*>(itContainer); //chests

  bool bEmptied = false;

  if(mc!=NULL){
    delete mc->RemoveSecondaryMaterial(); //prevents: ex. random liquids like antidote
    bEmptied = true;
  }

  if(ic!=NULL){ //empty chests etc
    stack* stkC = ic->GetContained();
    itemvector ivC;
    stkC->FillItemVector(ivC);
    for(int i=0;i<ivC.size();i++){
      SendToHellSafely(ivC[i]);
//      ivC[i]->RemoveFromSlot();
//      ivC[i]->SendToHell();
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
  if(
    game::IsQuestItem(it) ||
    it->GetEnchantment()!=0 ||
    dynamic_cast<amulet*>(it)!=NULL ||
    dynamic_cast<horn*  >(it)!=NULL ||
    dynamic_cast<ring*  >(it)!=NULL ||
    dynamic_cast<scroll*>(it)!=NULL ||
    dynamic_cast<wand*  >(it)!=NULL ||
    false // just to make it easier to re-organize and add checks above
  ){
    return false;
  }

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

  if(prpdSuspended->rc.GetDungeonLevelID()!=craftcore::CurrentDungeonLevelID()){
    ADD_MESSAGE("I need to be in the same dungeon I was before."); //TODO better message: place? location? dungeon level sounds a bit non-immersive, or not?
    return;
  }

  if(prpdSuspended->v2PlayerCraftingAt==Char->GetPos()){
    Char->SwitchToCraft(*prpdSuspended);
    craftcore::SetSuspended(NULL); //was resumed so discard it
  }else{
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
  fs<<(i++)<<":"<<itToolID<<";";
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

  fs<<(i++)<<":"<<v2AnvilLocation.X<<","<<v2AnvilLocation.Y<<";";
  fs<<(i++)<<":"<<v2BuildWhere.X<<","<<v2BuildWhere.Y<<";";
  fs<<(i++)<<":"<<v2ForgeLocation.X<<","<<v2ForgeLocation.Y<<";";
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

void recipecore::integrityCheck() const
{
  if(initKey!=RPDInitKey) //TODO bools get crazy values too if not initialized
    ABORT("recipedata corrupted, not initialized or invalid");// it will not be possible to show info, would crash on it... , dbgInfo().CStr());
}

recipecore::recipecore(humanoid* H,uint sel){
  initKey = RPDInitKey;

  h=H;

  bCanBeSuspended=false;
  iDungeonLevelID=craftcore::CurrentDungeonLevelID();
}

recipedata::recipedata(humanoid* H,uint sel) : rc(H,sel)
{
  itTool=NULL;
  lsqrWhere = NULL;
  lsqrCharPos = rc.H()==NULL ? NULL : game::GetCurrentLevel()->GetLSquare(rc.H()->GetPos());
  itWeakestIngredient = NULL;

  SelectedRecipe=sel;
  bSpendCurrentTurn=false;
  bAlreadyExplained=false;
  bHasAllIngredients=false;
  bCanStart=false;
  bCanBePlaced=false;

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
  itSpawnMatMainCfg = itCfg->GetMainMaterial()->GetConfig();
  itSpawnMatMainVol = itCfg->GetMainMaterial()->GetVolume();
  if(itCfg->GetSecondaryMaterial()!=NULL){
    itSpawnMatSecCfg = itCfg->GetSecondaryMaterial()->GetConfig();
    itSpawnMatSecVol = itCfg->GetSecondaryMaterial()->GetVolume();
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

cfestring recipedata::SpawnTerrain(){
  rc.integrityCheck();

  olterrain* otSpawn = NULL;

  switch(otSpawnType){
  case CTT_FURNITURE:
    otSpawn=decoration::Spawn(otSpawnCfg);
    break;
  case CTT_DOOR:
    otSpawn=door::Spawn(otSpawnCfg);
    ((door*)otSpawn)->SetIsLocked(false);
    ((door*)otSpawn)->SetIsOpened(true);
    //TODO configure lock type based randomly in one of the keys available on player's inventory
    break;
  case CTT_WALL:
    otSpawn=wall::Spawn(otSpawnCfg); //earth::Spawn();
    break;
  }

  if(otSpawn==NULL)
    ABORT("craft spawned no terrain.");

  otSpawn->SetMainMaterial(material::MakeMaterial(otSpawnMatMainCfg,otSpawnMatMainVol));
  if(otSpawnMatSecCfg>0)
    otSpawn->SetSecondaryMaterial(material::MakeMaterial(otSpawnMatSecCfg,otSpawnMatSecVol));

  festring fsCreated;
  lsqrWhere->ChangeOLTerrainAndUpdateLights(otSpawn);
//  if(dynamic_cast<wall*>(otSpawn)!=NULL)
//    iWallMaterialConfig = otSpawn->GetMainMaterial()->GetConfig();

//      if(lsqrWhere->CanBeSeenByPlayer())
  fsCreated << otSpawn->GetName(INDEFINITE);
//  fsMsg << "You built " << fsCreated.CStr();

//  rpd.otSpawn=NULL; //see ~craft

  return fsCreated;
}

//void recipedata::SendTerrainToHell()
//{
//  if(otSpawn && otSpawn->Exists()){DBGLN; //TODO otSpawn->Exists() is this safe or even necessary?
//    otSpawn->SendToHell();
//  }
//}

//void recipedata::SendSpawnItemToHell(){
//  if(itSpawn!=NULL || itSpawnID!=0){
//    //better not use itSpawn->Exists() as it if got deleted may segfault and would be inconsistent anyway
//    item* it = game::SearchItem(itSpawnID);
//    if(itSpawn!=it){ DBG3("howThisHappened?",it,itSpawn);
//      itSpawn=it;
//    }
//    itSpawn->RemoveFromSlot(); //just in case it is required one day, this prevents a lot of trouble...
//    itSpawn->SendToHell();
//  }
//
//  itSpawn=NULL;
//  itSpawnID=0;
//}

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
  lsqrWhere = NULL;
  lsqrCharPos = NULL;
  itWeakestIngredient = NULL;

  rc.ClearRefs();
}

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

  bool IsTheSelectedOne(recipedata& rpd){
    return rpd.SelectedRecipe == iListIndex;
  }

  virtual ~recipe(){}

  static bool where(recipedata& rpd){
    int Dir = game::DirectionQuestion("Build it where?", false, false);DBGLN;
    if(Dir != DIR_ERROR && rpd.rc.H()->GetArea()->IsValidPos(rpd.rc.H()->GetPos() + game::GetMoveVector(Dir)))
      rpd.lsqrWhere = rpd.rc.H()->GetNearLSquare(rpd.rc.H()->GetPos() + game::GetMoveVector(Dir));

    if(rpd.lsqrWhere!=NULL && rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.lsqrWhere->GetCharacter()==NULL){
      rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
      rpd.bCanBePlaced=true;
      return true;
    }

    return false;
  }

  static int calcTurns(material* mat,float fMult=1.0){
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

  static truth IsMeltable(material* mat){ //TODO add all meltables
    if(mat->GetCategoryFlags() & IS_METAL)
      return true;
    if(mat->GetConfig()==GLASS)
      return true;
    return false;
  }

  static item* FindTool(itemvector vitInv, int iCfg){
    for(int i=0;i<vitInv.size();i++)
      if(dynamic_cast<meleeweapon*>(vitInv[i])!=NULL && vitInv[i]->GetConfig()==iCfg)
        return vitInv[i];
    for(int i=0;i<vitInv.size();i++)
      if(dynamic_cast<meleeweapon*>(vitInv[i])!=NULL && vitInv[i]->GetConfig()==(iCfg|BROKEN))
        return vitInv[i];
    return NULL;
  }

  static item* FindHammeringTool(recipedata& rpd){
    int iBaseTurns = rpd.iBaseTurnsToFinish;
    static const int iTotToolTypes=4;
    //TODO could be based on volume and weight vs strengh and dexterity to determine how hard is to use the tool
    //mace is not necessarily spiked despite the gfx
    static const int aiTypes[iTotToolTypes]={HAMMER,FRYING_PAN,WAR_HAMMER,MACE};
    float fIncTurnsStep=0.25;
    itemvector vi = vitInv(rpd);
    item* it = NULL;
    for(int j=0;j<iTotToolTypes;j++){DBG2(j,aiTypes[j]);
      it = FindTool(vi, aiTypes[j]);
      if(it){DBG2(it->GetConfig(),it->GetName(DEFINITE).CStr());
        int iAddTurns=iBaseTurns*(j*fIncTurnsStep);
        rpd.iBaseTurnsToFinish = iBaseTurns + iAddTurns;
        return it;
      }
    }
    return NULL;
  }

  template <typename T> static truth choseIngredients(
      cfestring fsQ,
      long volume,
      recipedata& rpd,
      int& iWeakestCfg,
      bool bMultSelect = true,

      int iReqCfg=0,
      bool bMainMaterRemainsBecomeLump=false,
      bool bOverridesQuestion=false,
      bool bMsgInsuficientMat=true,
      bool bInstaAddIngredients=false
  ){DBGLN;
    if(volume==0)
      ABORT("ingredient required 0 volume?");

//    std::vector<ulong>& rIngIDs = bMain ? rpd.ingMainIDs : rpd.ingSecondaryIDs;

    // prepare the filter for ALL items also resetting them first!
    const itemvector vi = vitInv(rpd);
    for(int i=0;i<vi.size();i++){
//      if(!vi[i]->Exists())continue;
//      if(game::SearchItem(vi[i]->GetID())!=vi[i]) //this will crash if the invalid is outside app memory...
//        ABORT("item's list contains invalid pointer %d %d",vi[i]->GetID(),vi[i]); //should just continue instead? but may leave inconsistent data.
      vi[i]->SetValidRecipeIngredient(false);
      if(dynamic_cast<T*>(vi[i])!=NULL){
        if(vi[i]->IsBurning())continue;
        if(vi[i]->GetSpoilLevel()>0)continue;

        if(iReqCfg>0 && vi[i]->GetConfig()!=iReqCfg)continue;

        bool bAlreadyUsed=false;
        for(int j=0;j<rpd.ingredientsIDs.size();j++){
          if(vi[i]->GetID()==rpd.ingredientsIDs[j]){
            bAlreadyUsed=true;
            break;
          }
        }
        if(bAlreadyUsed)continue;

        vi[i]->SetValidRecipeIngredient(true);
      }
    }

    int iWeakest=-1;
    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true);
    std::vector<unsigned long> tmpIngredientsIDs;
    for(;;)
    {
      itemvector ToUse;
      game::DrawEverythingNoBlit();
      int flags = bMultSelect ? REMEMBER_SELECTED : REMEMBER_SELECTED|NO_MULTI_SELECT;

      festring fsFullQ;
      if(bOverridesQuestion)
        fsFullQ=fsQ;
      else
        fsFullQ = festring("What ingredient(s) will you use ")+fsQ+" ["+volume+"cm3]"+festring("? (hit ESC for more options if available)");

      rpd.rc.H()->GetStack()->DrawContents(ToUse, rpd.rc.H(),
        fsFullQ, flags, &item::IsValidRecipeIngredient);
      if(ToUse.empty())
        break;

      for(int i=0;i<ToUse.size();i++){
        material* mat = ToUse[i]->GetMainMaterial();
        if(iWeakest==-1 || iWeakest > mat->GetStrengthValue()){
          iWeakest = mat->GetStrengthValue();
          iWeakestCfg = mat->GetConfig();
          rpd.itWeakestIngredient = ToUse[i];
        }

        tmpIngredientsIDs.push_back(ToUse[i]->GetID()); DBG1(ToUse[i]->GetID());
        volume -= ToUse[i]->GetVolume(); DBG2(volume,ToUse[i]->GetVolume());
        ToUse[i]->SetValidRecipeIngredient(false); //just to not be shown again on the list

        if(volume<=0){
          long lRemainingVol=volume*-1;
          if(lRemainingVol>0 && bMainMaterRemainsBecomeLump){
            material* matM = ToUse[i]->GetMainMaterial();
            long lVolM = matM->GetVolume();
            lVolM -= lRemainingVol; //to sub
            if(lVolM<=0)
              ABORT("ingredient volume reduced to negative or zero %d %d %s",lVolM,lRemainingVol,matM->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
            matM->SetVolume(lVolM);

            item* lumpR = CreateLumpAtCharStack(matM, rpd);
            lumpR->GetMainMaterial()->SetVolume(lRemainingVol);

            lumpMix(vi,lumpR,rpd.bSpendCurrentTurn);

            material* matS = ToUse[i]->GetSecondaryMaterial();
            if(matS!=NULL && matS->GetVolume()>0)
              ABORT("ingredient secondary material should not have volume %d %s %s",matS->GetVolume(),matS->GetName(DEFINITE).CStr(),ToUse[i]->GetNameSingular().CStr());
          }

          break;
        }
      }

      if(volume<=0 || bInstaAddIngredients){
        for(int i=0;i<tmpIngredientsIDs.size();i++)
          rpd.ingredientsIDs.push_back(tmpIngredientsIDs[i]);

        if(bInstaAddIngredients)
          tmpIngredientsIDs.clear(); //to avoid dups
        else
          break; //success
      }
    }

    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);

    if(bMsgInsuficientMat && volume>0)
      ADD_MESSAGE("This amount of materials won't work...");

    return volume<=0;
  }

  template <typename T> static bool choseOneIngredient(recipedata& rpd){
    int iWeakestCfgDummy;
    return choseIngredients<T>(
      festring(""),
      1, //just to chose one of anything
      rpd,
      iWeakestCfgDummy,
      false);
  }

  static itemvector vitInv(recipedata& rpd){
    itemvector vi;
    rpd.rc.H()->GetStack()->FillItemVector(vi); //TODO once, the last item from here had an invalid pointer, HOW?
    if(rpd.rc.H()->GetLeftWielded ())vi.push_back(rpd.rc.H()->GetLeftWielded ());
    if(rpd.rc.H()->GetRightWielded())vi.push_back(rpd.rc.H()->GetRightWielded());
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

  static bool canBeAWoodenStick(item* it,material* mat){
//    material* matM = it->GetMainMaterial();
//    material* matS = it->GetSecondaryMaterial();
//
//    if(matM->GetConfig()==mat->GetConfig()){
//      DBG6(matM->GetConfig(),matM->GetAdjectiveStem().CStr(),matM->GetType(),matM->GetNaturalForm().GetCategory(),matM->GetNaturalForm().GetContentType(),matM->GetProtoType()->GetClassID());
//      if(IsMeltable(matM))return false;
//      if(matM->GetConfig()>=FUNGI_WOOD && matM->GetConfig()<=PETRIFIED_WOOD)return true;
//    }
//
//    if(matS!=NULL){
//      DBG6(matS->GetConfig(),matS->GetAdjectiveStem().CStr(),matS->GetType(),matS->GetNaturalForm().GetCategory(),matS->GetNaturalForm().GetContentType(),matS->GetProtoType()->GetClassID());
//      if(matS->GetConfig()==mat->GetConfig()){
//        if(IsMeltable(matS))return false;
//      }
//    }

    material* matChk = it->GetMainMaterial();
    for(;;){
      if(matChk->GetConfig()==mat->GetConfig()){
        //DBG6(matChk->GetConfig(),matChk->GetAdjectiveStem().CStr(),matChk->GetType(),matChk->GetNaturalForm().GetCategory(),matChk->GetNaturalForm().GetContentType(),matChk->GetProtoType()->GetClassID());
        if(IsMeltable(matChk))
          return false;
        if(matChk->GetConfig()>=FUNGI_WOOD && matChk->GetConfig()<=PETRIFIED_WOOD)
          return true;
      }

      if(matChk==it->GetSecondaryMaterial())
        break;

      matChk=it->GetSecondaryMaterial();
      if(matChk==NULL)
        break;
    }

    return false;
  }

  static item* CreateLumpAtCharStack(material* mat, recipedata& rpd, bool bWoodenCreateStick=false){
    if(mat==NULL)
      ABORT("NULL lump material");

    DBG2(mat->GetName(DEFINITE).CStr(),mat->GetVolume());
    bool bLiquid = mat->IsLiquid();
    if(mat->IsPowder())bLiquid=false; //TODO if explosive could have a chance to xplod

    if(dynamic_cast<gas*>(mat)!=NULL)return NULL; //TODO should have a chance to release the gas effect

    if(bLiquid){
      rpd.rc.H()->SpillFluid(NULL,liquid::Spawn(mat->GetConfig(),mat->GetVolume()));
    }else{
      item* LumpTmp = lump::Spawn(0, NO_MATERIALS);
      if(bWoodenCreateStick){
        LumpTmp = stick::Spawn(0, NO_MATERIALS);
      }else{
        LumpTmp = lump::Spawn(0, NO_MATERIALS);
      }
      LumpTmp->SetMainMaterial(material::MakeMaterial(mat->GetConfig(),mat->GetVolume()));
      rpd.rc.H()->GetStack()->AddItem(LumpTmp);
      ADD_MESSAGE("%s was recovered.", LumpTmp->GetName(DEFINITE).CStr());
      return LumpTmp;
    }

    return NULL;
  }

  lsquare* lsqrFORGE = NULL;
  bool chkForge(recipedata& rpd,lsquare* lsqr){DBGLN;
    olterrain* ot = lsqr->GetOLTerrain();
    if(ot!=NULL && ot->GetConfig() == FORGE && lsqr->CanBeSeenBy(rpd.rc.H())){
      lsqrFORGE = lsqr;
      rpd.v2ForgeLocation = lsqrFORGE->GetPos();
      return true;
    }
    return false;
  }
  bool findForge(recipedata& rpd,bool bReqOnlyVisible=false){
    if(bReqOnlyVisible){DBGLN;
      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2(iX,iY));DBG3(lsqr,iX,iY);
        if(chkForge(rpd,lsqr))break;
      }}
    }else{DBGLN; //must be on adjacent square
      int iDist = 1; //TODO improve this (despite fun, is wrong..)
      for(int i=0;i<(8*iDist);i++){
        int iDir = i%8;
        int iMult = 1 + i/8;
        v2 v2Add = game::GetMoveVector(iDir) * iMult;
        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG5(DBGAV2(v2Add),DBGAV2(v2Pos),iMult,iDir,i);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);
          if(chkForge(rpd,lsqr))break;
        }
      }
    }

    if(lsqrFORGE==NULL){
      ADD_MESSAGE(bReqOnlyVisible?"No visible forge nearby.":"No forge nearby.");
      rpd.bAlreadyExplained=true;
      return false;
    }

    return true;
  }

};

struct srpOltBASE : public recipe{
 protected:
  int iReqVol=0;
  int iReqVolSkulls=0;
  int iTurns=0;
  bool bRequiresWhere=false;
  bool bAllowSimpleStones=false;
  bool bReqForge=false;

  virtual bool spawnCfg(recipedata& rpd){return false;}

 public:
  bool work(recipedata& rpd){DBGLN;
    if(desc.IsEmpty())ABORT("Missing recipe description for OLT");
    if(iReqVol==0)ABORT("Recipe vol is 0 for OLT");
    if(iTurns==0)ABORT("Recipe turns is 0 for OLT");

    DBGLN;
    if(!recipe::IsTheSelectedOne(rpd))
      return false;

    if(bReqForge)
      if(!recipe::findForge(rpd))
        return true;

    DBGLN;
    if(bRequiresWhere){DBGLN;
      if(!recipe::where(rpd))
        return true;
    }else{DBGLN;
      rpd.lsqrWhere=rpd.lsqrCharPos;
    }

    rpd.iBaseTurnsToFinish=iTurns;
    rpd.itTool = FindHammeringTool(rpd);

    if(rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.itTool!=NULL){
      rpd.bCanBePlaced=true;

      festring fsQ("to build ");fsQ<<name;
      int iCfg=-1;
      bool bH=false;
      if(!bH)
        bH=choseIngredients<stick>(fsQ, iReqVol, rpd, iCfg);
      if(!bH && iReqVolSkulls>0)
        bH=choseIngredients<skull> (fsQ, iReqVolSkulls, rpd, iCfg);
      if(!bH)
        bH=choseIngredients<bone> (fsQ, iReqVol, rpd, iCfg);
      if(!bH)
        bH=choseIngredients<stone>(fsQ, iReqVol, rpd, iCfg, true, bAllowSimpleStones?0:INGOT, true);
      if(bH){
        rpd.bHasAllIngredients=bH;
        rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
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

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("build","a door");
      desc << "You will need a hammer, a frying pan or even a mace.";
    }

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

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("build","a chair");
      desc << "You will need a hammer, a frying pan or even a mace.";
    }

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

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("build","an anvil");
      desc << "Near a forge you can create an anvil using a hammer, a frying pan or even a mace.";
    }

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

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("build","a forge");
      desc << "You can build a forge using a hammer, a frying pan or even a mace.";
    }

    iReqVol=15000;
    iTurns=30;
    bRequiresWhere=true;
    //TODO require fire source like fireball wand or 3 lanterns
    return srpOltBASE::work(rpd);
  }
};srpForge rpForge;
struct srpWall2 : public srpOltBASE{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_WALL;

    rpd.otSpawnCfg=BRICK_OLD;
//    if(dynamic_cast<stone>(game::SearchItem(rpd.ingredientsIDs[0]))!=NULL)
    if(dynamic_cast<stone*>(rpd.itWeakestIngredient)!=NULL)
      rpd.otSpawnCfg=STONE_WALL;

    return true;
  }

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("construct","a wall");
      desc << "You can construct a wall piling stones, sticks or bones.";
    }

    //TODO this doesnt look good. anyway this volume should be on the .dat file as wall/earthWall attribute...
    iReqVol=9000; //TODO is this too little? a broken wall drops 3 rocks that is about 1000 each, so 3 walls to build one is ok?
    iReqVolSkulls=12000; //TODO is this too little? necromancers can spawn skeletons making it easy to get skulls, but the broken bone wall will drop bones and not skulls...
    iTurns=20;
    bRequiresWhere=true;
    bAllowSimpleStones=true;
    return srpOltBASE::work(rpd);
  }
};srpWall2 rpWall2;

struct srpMelt : public recipe{
  bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("melt","an ingot");
      desc << "Near a forge, meltable lumps can be used to prepare ingots.";
    }

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
    if(!recipe::IsTheSelectedOne(rpd))
      return false;

    if(!recipe::findForge(rpd))
      return true;

    int iWeakestCfgDummy;
    choseIngredients<lump>(
      festring("First chosen lump's material will be mixed with further ones of same material only, hit ESC to accept."),
      1000000, //just any huge volume as "limit"
      rpd, iWeakestCfgDummy, true,
      0, false, true, false, true);
    if(rpd.ingredientsIDs.empty())
      return true;

    item* Lump = game::SearchItem(rpd.ingredientsIDs[0]);

    item* LumpMeltable=NULL;
    if(IsMeltable(Lump->GetMainMaterial()))
      LumpMeltable = Lump;

    if(LumpMeltable==NULL){
      ADD_MESSAGE("Can't melt %s.",Lump->GetName(INDEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return true;
    }

    material* matM=LumpMeltable->GetMainMaterial();

    // multiple (compatible with 1st) lumps will be mixed in a big one again
    for(int i=1;i<rpd.ingredientsIDs.size();i++){
      item* LumpAdd = game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
      if(dynamic_cast<lump*>(LumpAdd)==NULL)continue;

      material* lumpAddM = LumpAdd->GetMainMaterial();DBGLN;
      if(lumpAddM->GetConfig()!=matM->GetConfig())continue;

      // join
      matM->SetVolume(matM->GetVolume()+lumpAddM->GetVolume());DBGLN;

      craftcore::SendToHellSafely(LumpAdd);
//      LumpAdd->RemoveFromSlot();
//      LumpAdd->SendToHell();
    }

    rpd.iBaseTurnsToFinish = calcTurns(matM,5); DBG1(rpd.iBaseTurnsToFinish);

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

    rpd.rc.SetCanBeSuspended();

    rpd.bCanStart=true;

    return true;
  }
};srpMelt rpMelt;

struct srpDismantle : public recipe{ //TODO this is instantaneous, should take time?
  bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("dismantle","some materials as lumps and sticks");
      desc << "Near a forge, any item can be dismantled to recover it's materials.";
    }

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
    if(!recipe::IsTheSelectedOne(rpd))return false;

    if(!recipe::findForge(rpd))
      return true;

    ///////////////////// chose item to melt/smash
    game::DrawEverythingNoBlit();
    if(!choseOneIngredient<item>(rpd))
      return true;

    item* itToUse = game::SearchItem(rpd.ingredientsIDs[0]); DBG2(rpd.ingredientsIDs[0],itToUse);
    rpd.ingredientsIDs.clear();

    if(game::IsQuestItem(itToUse)){
      ADD_MESSAGE("You feel that would be a bad idea and carefully stores it back in your inventory.");
      rpd.bAlreadyExplained=true;
      return true;
    }

    material* matM=NULL;
    material* matS=NULL;
    material* matIngot=NULL;

    matM = itToUse->GetMainMaterial();
    matS = itToUse->GetSecondaryMaterial();

    bool bJustLumpfyTheIngot=false;
    if(dynamic_cast<stone*>(itToUse)!=NULL && itToUse->GetConfig()==INGOT){
      bJustLumpfyTheIngot=true;
    }

    /////////////////////// dismantle into lumps
    if(dynamic_cast<lump*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a lump.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return true;
    }

    if(dynamic_cast<stick*>(itToUse)!=NULL){
      ADD_MESSAGE("%s is already a stick.", itToUse->GetName(DEFINITE).CStr());
      rpd.bAlreadyExplained=true;
      return true;
    }

    item* LumpMeltable = NULL;
    // for now, uses just one turn to smash anything into lumps but needs to be near a FORGE TODO should actually require a stronger hammer than the material's hardness being smashed, and could be anywhere...

    { // main material ALWAYS exist
      item* LumpM = CreateLumpAtCharStack(matM,rpd,canBeAWoodenStick(itToUse,matM));
      if(IsMeltable(LumpM->GetMainMaterial()))
        LumpMeltable = LumpM;
    }

    if(matS!=NULL){
      item* LumpS = CreateLumpAtCharStack(matS,rpd,canBeAWoodenStick(itToUse,matS)); //must always be prepared to not lose it
      if(LumpS!=NULL)
        if(IsMeltable(LumpS->GetMainMaterial()) )
          if(LumpMeltable==NULL)
            LumpMeltable = LumpS;
    }

    // OBS.: one of the lumps will have to be turned into ingot later by user action

    ADD_MESSAGE("%s was completely dismantled.", itToUse->GetName(DEFINITE).CStr());
    rpd.bAlreadyExplained=true;
    craftcore::SendToHellSafely(itToUse);
//    itToUse->RemoveFromSlot(); //important to not crash elsewhere!!!
//    itToUse->SendToHell();
    DBG4("SentToHell",itToUse->GetID(),itToUse,LumpMeltable); //TODO if has any magic should release it and also harm

    rpd.bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...

    if(bJustLumpfyTheIngot){
      lumpMix(vitInv(rpd),LumpMeltable,rpd.bSpendCurrentTurn);
      rpd.bAlreadyExplained=true;
      return true;
    }

    return true;
  }
};srpDismantle rpDismantle;

struct srpSplitLump : public recipe{
  bool work(recipedata& rpd){
    if(desc.IsEmpty()){
      init("split","some lumps");
      desc << "Split one lump to be easier to work with.";
    }

    if(!recipe::IsTheSelectedOne(rpd))
      return false;

    if(choseOneIngredient<lump>(rpd)){
      item* Lump = game::SearchItem(rpd.ingredientsIDs[0]);
      long vol=Lump->GetMainMaterial()->GetVolume();

      long div = game::NumberQuestion(CONST_S("Split in how many parts?"), WHITE, true);
      if(div<=1)return true;

      long volPart = vol/div;
      if(volPart < 1){
        ADD_MESSAGE("The splitted part must have some volume.");
        rpd.bAlreadyExplained=true;
        return true;
      }

      long volRest = vol%div;

      Lump->GetMainMaterial()->SetVolume(volPart);
      for(int i=0;i<(div-1);i++)
        CreateLumpAtCharStack(Lump->GetMainMaterial(), rpd);

      if(volRest>0)
        Lump->GetMainMaterial()->SetVolume(volPart+volRest);

      rpd.bAlreadyExplained=true; //no need to say anything
    }

    return true;
  }
};srpSplitLump rpSplitLump;

struct srpForgeItem : public recipe{
  bool work(recipedata& rpd){DBGLN;
    if(desc.IsEmpty()){DBGLN;
      init("forge","an item");
      desc << "Using something as a hammer, close to an anvil and with a forge nearby you can create items.";
    }DBGLN;

    if(!recipe::IsTheSelectedOne(rpd))
      return false;

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
          ADD_MESSAGE("You can't enchant %s!",itSpawn->GetName(INDEFINITE).CStr()); //itCreate->GetNameSingular());//
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
      return true;
    }

    ADD_MESSAGE("Now I need the material(s) to create a %s as I would probably create %s.",Default.CStr(),itSpawn->GetName(INDEFINITE).CStr()); //itCreate->GetNameSingular());//

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

    bool bM = false;
    festring fsM("as MAIN material");DBGLN;
    if(!bM)bM = choseIngredients<stone>(fsM,lVolM, rpd, iCfgM, true, INGOT, true);
    if(!bM)bM = choseIngredients<bone>(fsM,lVolM, rpd, iCfgM, true, 0, true);
    if(!bM)bM = choseIngredients<stick>(fsM,lVolM, rpd, iCfgM, true, 0, true);
    if(!bM){
      ADD_MESSAGE("I will craft it later...");
      rpd.bAlreadyExplained=true;
      craftcore::SendToHellSafely(itSpawn);
      return true;
    }

    bool bContainerEmptied = craftcore::EmptyContentsIfPossible(itSpawn);
//    materialcontainer* mc = dynamic_cast<materialcontainer*>(itSpawn);DBGLN; //potions, mines... also bananas xD
//    itemcontainer* ic = dynamic_cast<itemcontainer*>(itSpawn);DBGLN; //chests
//    bool bIsContainer =
//      itSpawn->GetStorageVolume()>0 || //something else TODO what?
//      ic!=NULL || //chests
//      mc!=NULL; //potions, mines... also bananas xD

    /**
     * TODO problem: basically sulphuric acid can already be stored on a metal can ...
     * TODO every materialcontainer should rust depending on it's contents, if made of anything else than glass
     * Keeping allowing creating materialcontainer of non glass because the fix is already required for the existing metal can,
     * so preventing it would still not fix how metal can works...
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
      if(!bS)bS = choseIngredients<stone>(fsS,lVolS, rpd, iCfgS, true, INGOT, true);
      if(bIsWeapon){DBGLN; //this is mainly to prevent mc being filled with non-sense materials TODO powders one day would be ok
        if(!bS)bS = choseIngredients<bone>(fsS,lVolS, rpd, iCfgS, true, 0, true);
        if(!bS)bS = choseIngredients<stick>(fsS,lVolS, rpd, iCfgS, true, 0, true);
      }

      if(!bS){
        ADD_MESSAGE("I will craft it later...");
        rpd.bAlreadyExplained=true;
        craftcore::SendToHellSafely(itSpawn);
        return true;
      }
    }

    if(bReqS && !bAllowS)
      ABORT("item reqs secondary mat but doesnt allow it??? %s",itSpawn->GetName(DEFINITE).CStr());

    rpd.bHasAllIngredients=true;

    itSpawn->SetMainMaterial(material::MakeMaterial(iCfgM,lVolM));
    if(bAllowS)
      itSpawn->SetSecondaryMaterial(material::MakeMaterial(iCfgS,lVolS));

    material* matM = itSpawn->GetMainMaterial();
    material* matS = itSpawn->GetSecondaryMaterial();
//    else{
//      if(mc!=NULL)
//        delete mc->RemoveSecondaryMaterial(); //prevents: ex. random liquids like antidote
//    }

//    craftcore::EmptyContentsIfPossible(itSpawn);
//    if(ic!=NULL){ //empty chests etc
//      stack* stkC = ic->GetContained();
//      itemvector ivC;
//      stkC->FillItemVector(ivC);
//      for(int i=0;i<ivC.size();i++){
//        ivC[i]->RemoveFromSlot();
//        ivC[i]->SendToHell();
//      }
//    }

    float fMult=10;//hammering to form it takes time even if the volume is low.
    rpd.iBaseTurnsToFinish=calcTurns(matM,fMult); DBG4(rpd.iBaseTurnsToFinish,matM->GetName(DEFINITE).CStr(),matM->GetConfig(),matM->GetVolume());
    if(bAllowS && matS!=NULL){
      rpd.iBaseTurnsToFinish+=calcTurns(matS,fMult);
      DBG4(rpd.iBaseTurnsToFinish,matS->GetName(DEFINITE).CStr(),matS->GetConfig(),matS->GetVolume());
    }

    if(IsMeltable(matM) || (matS!=NULL && IsMeltable(matS))){
      if(!recipe::findForge(rpd,true)){
        craftcore::SendToHellSafely(itSpawn);
        return true;
      }

      ////////////// find the anvil
      lsquare* lsqrAnvil = NULL;
      for(int iDir=0;iDir<8;iDir++){
        v2 v2Add = game::GetMoveVector(iDir);
        v2 v2Pos = rpd.rc.H()->GetPos() + v2Add; DBG3(DBGAV2(v2Add),DBGAV2(v2Pos),iDir);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.rc.H()->GetNearLSquare(v2Pos);DBG1(lsqr);
          olterrain* ot = lsqr->GetOLTerrain();
          if(ot!=NULL && ot->GetConfig() == ANVIL && lsqr->CanBeSeenBy(rpd.rc.H())){
            lsqrAnvil = lsqr;
            rpd.v2AnvilLocation=lsqrAnvil->GetPos();
            break;
          }
        }
      }
      if(lsqrAnvil==NULL){
        ADD_MESSAGE("No anvil nearby."); //TODO allow user miss-chose
        rpd.bAlreadyExplained=true;
        craftcore::SendToHellSafely(itSpawn);
        return true;
      }
    }

    //TODO glass should require proper tools (don't know what but sure not a hammer)
    //TODO bone should require a dagger
    //TODO 2 tools, one for meltables and the other for glass or bone
    rpd.itTool = FindHammeringTool(rpd);DBG1(rpd.iBaseTurnsToFinish);
    if(rpd.itTool==NULL){
      craftcore::SendToHellSafely(itSpawn);
      return true;
    }

    //TODO are these difficulties below well balanced (not cheap neither excessive/notFun)?

    key* itKey = dynamic_cast<key*>(itSpawn);
    if(itKey!=NULL){ //TODO are these difficulties below good?
      rpd.fDifficulty = 3.0;
      if(itKey->CanOpenLockType(HEXAGONAL_LOCK))
        rpd.fDifficulty = 4.0;
      if(itKey->CanOpenLockType(OCTAGONAL_LOCK))
        rpd.fDifficulty = 4.0;
      if(itKey->CanOpenLockType(HEART_SHAPED_LOCK))
        rpd.fDifficulty = 5.0;

      rpd.iBaseTurnsToFinish *= rpd.fDifficulty;
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

    rpd.bCanBeBroken = itSpawn->CanBeBroken();

    rpd.rc.SetCanBeSuspended();

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
    if(!recipe::IsTheSelectedOne(rpd))
      return false;

    itemvector vi;

    ///////////// tool ////////////////
    rpd.itTool = FindTool(vitInv(rpd), DAGGER);
    if(rpd.itTool==NULL)
      return true;

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
      return true;
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
      return true;
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

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("extract","some poison");
      desc << "Use a " << fsTool << " to " << action << " " << name << " from "
        << fsCorpse << " into a " << fsBottle <<  ".";
    }

    iLiqCfg=POISON_LIQUID;
    iMatEff=EFFECT_POISON;

    return srpFluidsBASE::work(rpd);
  }
};srpPoison rpPoison;

struct srpAcid : public srpFluidsBASE{
  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Acidicity)>0 || (flesh->Acidicity)>0;
  }

  bool work(recipedata& rpd){
    if(desc.IsEmpty()){ //TODO automate the sync of req ingredients description
      init("extract","some acidous fluid");
      desc   << "Use a " << fsTool << " to " << action   << " " << name   << " from "
        << fsCorpse << " into a " << fsBottle <<  ".";
    }

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

  if(h->GetLeftArm()==NULL && h->GetRightArm()==NULL){
    ADD_MESSAGE("You need at least one arm to be able to craft.");
    return false;
  }

  int iCraftTimeMult=1;
  if(h->GetLeftArm()==NULL || h->GetRightArm()==NULL){ //using only one hand will take more time
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
    if(prp==NULL && rp.work(rpd))prp=&rp; \
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
  RP(rpMelt);
  RP(rpForgeItem);
  RP(rpAnvil);

  RP(rpForge);
  if(bInitRecipes)
    return Craft(Char); //init recipes descriptions at least, one time recursion :>

  if(prp==NULL){DBGLN;
    return false;
  }DBGLN;

  //TODO these messages are generic, therefore dont look good... improve it
  if(rpd.bCanStart){DBGLN;
    if(rpd.ingredientsIDs.size()==0)
      ABORT("no ingredients chosen?");

    if(rpd.itTool!=NULL)
      ADD_MESSAGE("Let me see.. I will use %s as tool.",rpd.itTool->GetName(INDEFINITE).CStr());

    if(rpd.otSpawnType!=CTT_NONE || rpd.itSpawnType!=CIT_NONE) {
      if(rpd.itTool!=NULL && rpd.itTool->IsBroken())
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
        rpd.v2PlaceAt = rpd.lsqrWhere!=NULL ? rpd.lsqrWhere->GetPos() : rpd.lsqrCharPos->GetPos(); //may be ignored anyway, is just a fallback

      rpd.iAddDexterity=5; //TODO crafting difficult things should give more dexterity (wisdom too?)

      rpd.v2PlayerCraftingAt = Char->GetPos();

      if(rpd.itTool!=NULL)rpd.itToolID=rpd.itTool->GetID();

      if(rpd.lsqrWhere!=NULL)rpd.v2BuildWhere=rpd.lsqrWhere->GetPos();

      rpd.ClearRefs(); //pointers must be revalidated on the action handler

      Char->SwitchToCraft(rpd); // everything must be set before this!!!

      ADD_MESSAGE("Let me work on %s now.",prp->name.CStr());
    }else{
      ABORT("requested to craft nothing? %s",rpd.dbgInfo().CStr());
    }

    return true;
  }else{
    if(!rpd.bAlreadyExplained){
      if(rpd.lsqrWhere!=NULL && !rpd.bCanBePlaced)
        ADD_MESSAGE("%s can't be placed here!",prp->name.CStr());
      else if(!rpd.bHasAllIngredients){
        festring fsMsg;
        fsMsg<<"Required ingredients to "<<prp->action<<" "<<prp->name<<" are not met.";
        ADD_MESSAGE(fsMsg.CStr());
      }else if(rpd.itTool == NULL){ //TODO a bool to determine if tools is req?
        ADD_MESSAGE("Required tool is missing.");
      }else{
        ABORT("explain why crafting won't work.");
      }
    }

//    // cleanups if needed
//    if(rpd.itSpawn!=NULL){
//      rpd.itSpawn->RemoveFromSlot(); //just in case it is required one day, this prevents a lot of trouble...
//      rpd.itSpawn->SendToHell();
//    }
//
//    if(rpd.otSpawn!=NULL)
//      rpd.otSpawn->SendToHell();
  }

  /**
   * ATTENTION!!!
   * the complexity of granting one turn wont be lost doesnt worth letting the game crash randomly somewhere
   * hard to track the source of the problem...
   * so to ALWAYS spend current turn, when things sent to hell will properly be applied everywhere required
   * is the SAFEST thing! dont change this please!
   */
  return true;
  //  if(rpd.bSpendCurrentTurn)
  //    return true;
  //
  //  return false;
}

cfestring recipedata::SpawnItem(){
  rc.integrityCheck();

  item* itSpawn = NULL;
  material* matS = NULL;
  bool bAllowBreak=false;
  switch(itSpawnType){
  case CIT_POTION:
    /**
     * IMPORTANT!!!
     * do not use NO_MATERIALS here,
     * apparently the main material is always required for items
     * and the main material would remain uninitialized (instead of NULL)
     * leading to SEGFAULT when trying to set the main material!
     */
    itSpawn = potion::Spawn(itSpawnCfg); //may be a vial
    if(itSpawnMatSecCfg>0)
      matS = liquid::Spawn(itSpawnMatSecCfg,itSpawnMatSecVol);
    break;
  case CIT_PROTOTYPE:
    itSpawn = protosystem::CreateItemToCraft(fsItemSpawnSearchPrototype);
    bAllowBreak=true;
    if(itSpawnTot>1)
      ABORT("crafting multiple from prototype is not supported yet %d '%s' '%s'",
        itSpawnTot,fsItemSpawnSearchPrototype.CStr(),dbgInfo().CStr());
    break;
  case CIT_STONE:
    itSpawn = stone::Spawn(itSpawnCfg, NO_MATERIALS);
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

  if(itSpawnMatMainCfg==0 || itSpawnMatMainVol==0)
    ABORT("main material and/or volume is 0 %s %s",itSpawnMatMainCfg,itSpawnMatMainVol);
  itSpawn->SetMainMaterial(material::MakeMaterial(itSpawnMatMainCfg,itSpawnMatMainVol));

  if(itSpawnMatSecCfg==0)
    craftcore::EmptyContentsIfPossible(itSpawn);
  else{
    if(matS==NULL)
      matS = material::MakeMaterial(itSpawnMatSecCfg,itSpawnMatSecVol);
    if(matS!=NULL)
      itSpawn->SetSecondaryMaterial(matS);
  }

  itSpawn->MoveTo(rc.H()->GetStack());

  festring fsCreated;

  if(bAllowBreak && bSpawnBroken && !itSpawn->IsBroken()){
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
      itSpawn->SetConfig(itSpawnCfg | BROKEN);
      itSpawn->SetSize(itSpawn->GetSize() >> 1);
    }else{
      ADD_MESSAGE("My lack of skill broke %s into pieces...",itSpawn->GetName(DEFINITE).CStr());
      recipe::CreateLumpAtCharStack(itSpawn->GetMainMaterial(), *this);
      if(itSpawn->GetSecondaryMaterial()!=NULL)
        recipe::CreateLumpAtCharStack(itSpawn->GetSecondaryMaterial(), *this);
      craftcore::SendToHellSafely(itSpawn);
      itSpawn=NULL;
//      fsCreated = "nothing useful"; //TODO "a messy lump of nothing" ? :)
      fsCreated = "a funny looking messy lump"; //TODO "a messy lump of nothing" ? :)
    }
  }

  if(itSpawn!=NULL){
    if(itSpawnTot > 1){DBGLN;
      fsCreated << itSpawnTot << " " << itSpawn->GetNamePlural();DBGLN;
      for(int i=0;i<itSpawnTot-1;i++){ //-1 as the last one will be the original
        /**
         * IMPORTANT!!! the duplicator will vanish with the item ID that is being duplicated
         * so the duplication source item's ID will vanish. TODO could it be safely kept at DuplicateToStack() ?
         */
        itSpawn->DuplicateToStack(rc.H()->GetStack());
      }
    }else{DBGLN;
      fsCreated << itSpawn->GetName(INDEFINITE);
    }

    itSpawn->MoveTo(rc.H()->GetStack());DBGLN;
  }

  return fsCreated;
}
