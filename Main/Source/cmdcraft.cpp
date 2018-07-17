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
      SafelySendToHell(ivC[i]);
//      ivC[i]->RemoveFromSlot();
//      ivC[i]->SendToHell();
    }

    bEmptied = true;
  }

  return bEmptied;
}

void craftcore::SafelySendToHell(item* it)
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
    if(!prpd->IsCanBeSuspended())
      ABORT("action can't be suspended %s",prpd->dbgInfo().CStr());
    (*prpdSuspended)=(*prpd); //copy
//    prpdSuspended->ClearRefs();
    return;
  }

  ABORT("there is already a recipedata set %s VS %s",prpdSuspended->dbgInfo().CStr(),prpd->dbgInfo().CStr());
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

  if(prpdSuspended->IsCanBeSuspended())
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

  if(prpdSuspended->v2PlayerCraftingAt==Char->GetPos()){
    Char->SwitchToCraft(*prpdSuspended);
    craftcore::SetSuspended(NULL); //was resumed so discard it
  }else{
    ADD_MESSAGE("I need to be were I was crafting before."); //TODO add indicator to help player find that place again, could be a simple mapnote #Crafting :)
  }
}
//void craftcore::TerminateSuspendedAction(){
//  if(prpdSuspended==NULL)ABORT("no craft action set!");
//  if(!prpdSuspended->bCanBeSuspended)ABORT("is not a suspended craft action!"); //TODO show what was being done
//  craftAction->Terminate(false);
//}

void recipedata::Save(outputfile& SaveFile) const
{
  SaveFile //commented ones are just to keep the clarity/organization
    << bCanBeSuspended

//    int Selected;
    << ingredientsIDs
    << iAddDexterity

    << iBaseTurnsToFinish
//  bool bSpendCurrentTurn;
//  bool bAlreadyExplained;
    << itSpawnTot
    << v2ForgeLocation

//  item* itTool;//itToolID
//  item* itSpawn;//itSpawnID
//  lsquare* lsqrWhere;
//  lsquare* lsqrCharPos;

    << v2PlaceAt
//  bool bHasAllIngredients;
//  bool bCanStart;
//  bool bCanBePlaced;

    << bSuccesfullyCompleted
    << v2AnvilLocation
    << bFailed
    << v2PlayerCraftingAt
//    << itSpawnID

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

    ;

//  if(otSpawnType!=CTT_NONE)
//    SaveFile << otSpawn;
}

void recipedata::Load(inputfile& SaveFile)
{
  SaveFile //commented ones are just to keep the clarity/organization
    >> bCanBeSuspended

//    int Selected;
    >> ingredientsIDs
    >> iAddDexterity

    >> iBaseTurnsToFinish
//  bool bSpendCurrentTurn;
//  bool bAlreadyExplained;
    >> itSpawnTot
    >> v2ForgeLocation

//  item* itTool;//itToolID
//  item* itSpawn;//itSpawnID
//  lsquare* lsqrWhere;
//  lsquare* lsqrCharPos;

    >> v2PlaceAt
//  bool bHasAllIngredients;
//  bool bCanStart;
//  bool bCanBePlaced;

    >> bSuccesfullyCompleted
    >> v2AnvilLocation
    >> bFailed
    >> v2PlayerCraftingAt
//    >> itSpawnID

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

    ;

//  if(otSpawnType!=CTT_NONE)
//    SaveFile >> otSpawn;
}

cfestring recipedata::id()
{
  /**
   * this is a simple way to detect if the recipedata is the same,
   * here shall only contain things that would not change thru the
   * whole crafting process/time
   */

  festring fs;

  #define RPDINFO(o) if(o!=NULL)fs<<o->GetName(DEFINITE);fs<<";";

  fs<<bCanBeSuspended<<";";

  fs<<itToolID<<";";
  RPDINFO(itTool);

  fs<<itSpawnTot<<";";
  fs<<itSpawnCfg<<";";
  fs<<itSpawnMatMainCfg<<";";
  fs<<itSpawnMatMainVol<<";";
  fs<<itSpawnMatSecCfg<<";";
  fs<<itSpawnMatSecVol<<";";
//  RPDINFO(itSpawn);

//  RPDINFO(otSpawn);
  fs<<otSpawnCfg<<";";
  fs<<otSpawnMatMainCfg<<";";
  fs<<otSpawnMatMainVol<<";";
  fs<<otSpawnMatSecCfg<<";";
  fs<<otSpawnMatSecVol<<";";

  return fs;
}

cfestring recipedata::dbgInfo()
{
  festring fs;
  fs << id();
  return fs;
}

recipedata::recipedata(humanoid* H)
{
//  rpw = new recipework;

  bCanBeSuspended=false;

  Selected=-2; //default is -1 means not set, -2 to init
  ingredientsIDs.clear(); //just to init
  iAddDexterity=0;

  iBaseTurnsToFinish=1; //TODO should be based on attributes
  bSpendCurrentTurn=false;
  bAlreadyExplained=false;
  itSpawnTot=1;
  v2ForgeLocation=v2(0,0);

  v2PlaceAt=v2(0,0);
  bHasAllIngredients=false;
  bCanStart=false;
  bCanBePlaced=false;

  bSuccesfullyCompleted=false;
  v2AnvilLocation=v2(0,0);
  bFailed=false;
  v2PlayerCraftingAt=v2(0,0);
//  itSpawnID=0;

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

//  otSpawn=NULL;

  itTool=NULL;
//  itSpawn=NULL;
  lsqrWhere = NULL;
  lsqrCharPos = NULL;

  Actor=h=H;
}

void recipedata::CopySpawnItemCfgFrom(item* itCfg)
{
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

cfestring recipedata::SpawnItem(){
  item* itSpawn = NULL;
//  if(itSpawn!=NULL)
//    ABORT("craft: when spawning item, it should not be already(still) spawned.");

//  long cfg = itSpawnCfg;
//  if(bSpawnBroken)cfg|=BROKEN;
  material* matS = NULL;
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

  if(bSpawnBroken)
    itSpawn->Break(NULL);

  itSpawn->MoveTo(h->GetStack());

  festring fsCreated;
  if(itSpawnTot > 1){DBGLN;
    fsCreated << itSpawnTot << " " << itSpawn->GetNamePlural();DBGLN;
    for(int i=0;i<itSpawnTot-1;i++){ //-1 as the last one will be the original
      /**
       * IMPORTANT!!! the duplicator will vanish with the item ID that is being duplicated
       * so the duplication source item's ID will vanish. TODO could it be safely kept at DuplicateToStack() ?
       */
      itSpawn->DuplicateToStack(h->GetStack());
    }
  }else{DBGLN;
    fsCreated << itSpawn->GetName(INDEFINITE);
  }

  itSpawn->MoveTo(h->GetStack());DBGLN;

  return fsCreated;
}

void recipedata::CopySpawnTerrainCfgFrom(olterrain* otCfg){
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
  olterrain* otSpawn = NULL;

  switch(otSpawnType){
  case CTT_FURNITURE:
    otSpawn=decoration::Spawn(otSpawnCfg);//earth::Spawn();
    break;
  case CTT_DOOR:
    otSpawn=door::Spawn(otSpawnCfg);//earth::Spawn();
    ((door*)otSpawn)->SetIsLocked(false);
    ((door*)otSpawn)->SetIsOpened(true);
    break;
  case CTT_WALL:
    otSpawn=wall::Spawn(otSpawnCfg);//earth::Spawn();
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

//void recipedata::ClearRefs(){ //this is important to revalidate all pointers from IDs in case saving to main menu and loading again w/o exiting the game application
////  if(otSpawn!=NULL)
////    otSpawn->SendToHell();
//  otSpawn=NULL;
//
//  itTool=NULL;
////  if(itSpawn!=NULL){
////    itSpawn->RemoveFromSlot(); //just in case it is required one day, this prevents a lot of trouble...
////    itSpawn->SendToHell();
////  }
//  itSpawn=NULL;
//  lsqrWhere = NULL;
//  lsqrCharPos = NULL;
//
//  h = NULL;
//}

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

  virtual bool work(recipedata& rpd){
    if(rpd.Selected != iListIndex)
      return false;
    return true;
  }

  virtual ~recipe(){}

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

  static item* FindHammeringTool(humanoid* h, int& iBaseTurnsToFinish){
    int iBaseTurns = iBaseTurnsToFinish;
    static const int iTotToolTypes=4;
    //TODO could be based on volume and weight vs strengh and dexterity to determine how hard is to use the tool
    //mace is not necessarily spiked despite the gfx
    static const int aiTypes[iTotToolTypes]={HAMMER,FRYING_PAN,WAR_HAMMER,MACE};
    float fIncTurnsStep=0.25;
    itemvector vi = vitInv(h);
    item* it = NULL;
    for(int j=0;j<iTotToolTypes;j++){DBG2(j,aiTypes[j]);
      it = FindTool(vi, aiTypes[j]);
      if(it){DBG2(it->GetConfig(),it->GetName(DEFINITE).CStr());
        int iAddTurns=iBaseTurns*(j*fIncTurnsStep);
        iBaseTurnsToFinish = iBaseTurns + iAddTurns;
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
    const itemvector vi = vitInv(rpd.h);
    for(int i=0;i<vi.size();i++){
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

      rpd.h->GetStack()->DrawContents(ToUse, rpd.h,
        fsFullQ, flags, &item::IsValidRecipeIngredient);
      if(ToUse.empty())
        break;

      for(int i=0;i<ToUse.size();i++){
        material* mat = ToUse[i]->GetMainMaterial();
        if(iWeakest==-1 || iWeakest > mat->GetStrengthValue()){
          iWeakest = mat->GetStrengthValue();
          iWeakestCfg = mat->GetConfig();
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

            item* lumpR = CreateLumpAtCharStack(matM, rpd.h);
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

  static itemvector vitInv(const humanoid* h){
    itemvector vi;
    h->GetStack()->FillItemVector(vi);
    if(h->GetLeftWielded ())vi.push_back(h->GetLeftWielded ());
    if(h->GetRightWielded())vi.push_back(h->GetRightWielded());
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

          craftcore::SafelySendToHell(lumpToMix); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
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

  static item* CreateLumpAtCharStack(material* mat, character* C, bool bWoodenCreateStick=false){
    if(mat==NULL)
      ABORT("NULL lump material");

    DBG2(mat->GetName(DEFINITE).CStr(),mat->GetVolume());
    bool bLiquid = mat->IsLiquid();
    if(mat->IsPowder())bLiquid=false; //TODO if explosive could have a chance to xplod

    if(dynamic_cast<gas*>(mat)!=NULL)return NULL; //TODO should have a chance to release the gas effect

    if(bLiquid){
      C->SpillFluid(NULL,liquid::Spawn(mat->GetConfig(),mat->GetVolume()));
    }else{
      item* LumpTmp = lump::Spawn(0, NO_MATERIALS);
      if(bWoodenCreateStick){
        LumpTmp = stick::Spawn(0, NO_MATERIALS);
      }else{
        LumpTmp = lump::Spawn(0, NO_MATERIALS);
      }
      LumpTmp->SetMainMaterial(material::MakeMaterial(mat->GetConfig(),mat->GetVolume()));
      C->GetStack()->AddItem(LumpTmp);
      ADD_MESSAGE("%s was recovered.", LumpTmp->GetName(DEFINITE).CStr());
      return LumpTmp;
    }

    return NULL;
  }

};

struct srpUseForge : public recipe{ //TODO how to prevent this intermediary recipe from being instantiated w/o being extended?
  lsquare* lsqrFORGE = NULL;

  bool chkForge(recipedata& rpd,lsquare* lsqr){DBGLN;
    olterrain* ot = lsqr->GetOLTerrain();
    if(ot!=NULL && ot->GetConfig() == FORGE && lsqr->CanBeSeenBy(rpd.h)){
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
        v2 v2Pos = rpd.h->GetPos() + v2Add; DBG5(DBGAV2(v2Add),DBGAV2(v2Pos),iMult,iDir,i);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.h->GetNearLSquare(v2Pos);
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

struct srpOLT : public recipe{
 protected:
  int iReqVol=0;
  int iTurns=0;

  virtual bool spawnCfg(recipedata& rpd){return false;}

 public:
  bool work(recipedata& rpd){
    if(desc.GetSize()==0)ABORT("Missing recipe description for OLT");
    if(iReqVol==0)ABORT("Recipe vol is 0 for OLT");
    if(iTurns==0)ABORT("Recipe turns is 0 for OLT");

    if(!recipe::work(rpd))
      return false;

    rpd.iBaseTurnsToFinish=iTurns;
    rpd.itTool = FindHammeringTool(rpd.h,rpd.iBaseTurnsToFinish);

    rpd.lsqrWhere=rpd.lsqrCharPos;
    if(rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.itTool!=NULL){
      rpd.bCanBePlaced=true;

      festring fsQ("to build ");fsQ<<name;
      int iCfg=-1;
      bool bH=false;
      if(!bH)bH=choseIngredients<stick>(fsQ,iReqVol, rpd, iCfg);
      if(!bH)bH=choseIngredients<bone>(fsQ,iReqVol, rpd, iCfg);
      if(!bH)bH=choseIngredients<stone>(fsQ,iReqVol, rpd, iCfg, true, INGOT, true);
      if(bH){
        rpd.bHasAllIngredients=bH;
        rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
//        rpd.otSpawn=
        if(!spawnCfg(rpd))ABORT("Recipe spawn cfg not set %s",desc.CStr());
        // if(rpd.otSpawn==NULL)ABORT("Recipe spawn is NULL for OLT");
//        rpd.otSpawn->SetMainMaterial(material::MakeMaterial(iCfg,iReqVol)); //TODO secondary material?
        rpd.otSpawnMatMainCfg=iCfg;
        rpd.otSpawnMatMainVol=iReqVol;
        rpd.bCanStart=true;
      }
    }

    return true;
  }
};
struct srpDoor : public srpOLT{
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_DOOR;
    rpd.otSpawnCfg=NONE;
//  virtual void spawnCfg(){
//    door* Door = door::Spawn();
//    Door->SetIsLocked(false);
//    Door->SetIsOpened(true);
//    //TODO configure lock type based randomly in one of the keys available on player's inventory
//    return Door;
    return true;
  }

  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("build","a door");
      desc << "You will need a hammer, a frying pan or even a mace.";
    }

    iReqVol=30000;       //TODO this volume should be on the .dat file
    iTurns=30;
    return srpOLT::work(rpd);
  }
};srpDoor rpDoor;
struct srpChair : public srpOLT{
//  virtual olterrain* spawn(recipedata& rpd){
  virtual bool spawnCfg(recipedata& rpd){
    rpd.otSpawnType=CTT_FURNITURE;
    rpd.otSpawnCfg=CHAIR;
//    return decoration::Spawn(CHAIR);
    return true;
  }

  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("build","a chair");
      desc << "You will need a hammer, a frying pan or even a mace.";
    }

    iReqVol=15000;       //TODO this volume should be on the .dat file
    iTurns=15;
    return srpOLT::work(rpd);
  }
};srpChair rpChair;

struct srpWall : public recipe{ //TODO can it use srpOLT ?
  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("construct","a wall");
      desc << "Pile stones or skulls to create " << name;
    }

    if(!recipe::work(rpd))
      return false;

    int Dir = game::DirectionQuestion("Build it where?", false, false);DBGLN;
    if(Dir != DIR_ERROR && rpd.h->GetArea()->IsValidPos(rpd.h->GetPos() + game::GetMoveVector(Dir)))
      rpd.lsqrWhere = rpd.h->GetNearLSquare(rpd.h->GetPos() + game::GetMoveVector(Dir));

    if(rpd.lsqrWhere!=NULL && rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.lsqrWhere->GetCharacter()==NULL){
      rpd.bCanBePlaced=true;

      festring fsQ("to build ");fsQ<<name;
      int iCfg=-1;
      int iVol=-1;
      bool bH=false;
      if(!bH){
        iVol=9000; //TODO is this too little? a broken wall drops 3 rocks that is about 1000 each, so 3 walls to build one is ok?
        bH=choseIngredients<stone>(fsQ,iVol, rpd, iCfg);
      }
      if(!bH){
        iVol=10000; //TODO is this too little? necromancers can spawn skeletons making it easy to get skulls, but the broken bone wall will drop bones and not skulls...
        bH=choseIngredients<skull>(fsQ,iVol, rpd, iCfg);
      }
      //TODO this doesnt look good. anyway this volume should be on the .dat file as wall/earthWall attribute...
      if(bH){
        rpd.bHasAllIngredients=true;
        rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
        rpd.otSpawnType=CTT_WALL;
        rpd.otSpawnCfg=STONE_WALL;
        rpd.otSpawnMatMainCfg=iCfg;
        rpd.otSpawnMatMainVol=iVol;
//        rpd.otSpawn=wall::Spawn(STONE_WALL);//earth::Spawn();
//        rpd.otSpawn->SetMainMaterial(material::MakeMaterial(iCfg,iVol));
        rpd.iBaseTurnsToFinish=20;

        rpd.bCanStart=true;
      }
    }

    return true;
  }
};srpWall rpWall;

struct srpMelt : public srpUseForge{
  bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("melt","an ingot");
      desc << "Near a forge, meltable lumps can be used to prepare ingots.";
    }

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
    if(!srpUseForge::work(rpd))
      return false;

    if(!srpUseForge::findForge(rpd))
      return true;

    int iWeakestCfgDummy;
    choseIngredients<lump>(
      festring("First lump's chosen material will be mixed with further ones, hit ESC to accept."),
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

      craftcore::SafelySendToHell(LumpAdd);
//      LumpAdd->RemoveFromSlot();
//      LumpAdd->SendToHell();
    }

    rpd.iBaseTurnsToFinish = calcTurns(LumpMeltable->GetMainMaterial(),5); DBG1(rpd.iBaseTurnsToFinish);

    rpd.bHasAllIngredients=true;

    rpd.itSpawnType = CIT_STONE;
    rpd.itSpawnCfg = INGOT;
//    rpd.itSpawn = stone::Spawn(INGOT, NO_MATERIALS);

    /**
     * 25cm3 is each of the 2 parts of a dagger, but is too small?
     * Smaller ingots are easier to manage, less user interaction as they fit better.
     * BUT it may generate a HUGE LOT of tiny ingots and slow down the game when dropping/picking up :(
     */
    static long iIngotVol = 250;
    long iIngotVolTmp = game::NumberQuestion(festring("What volume shall the ingots have? [min 25cm3, last/default ")+iIngotVol+"cm3]",WHITE,true);
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
        item* itLumpR = LumpMeltable->DuplicateToStack(rpd.h->GetStack());

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

    rpd.SetCanBeSuspended();

    rpd.bCanStart=true;

    return true;
  }
};srpMelt rpMelt;

struct srpDismantle : public srpUseForge{ //TODO this is instantaneous, should take time?
  bool work(recipedata& rpd){
    // lumps are not usable until melt into an ingot.
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("dismantle","some materials as lumps and sticks");
      desc << "Near a forge, any item can be dismantled to recover it's materials.";
    }

    //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
    if(!srpUseForge::work(rpd))return false;

    if(!srpUseForge::findForge(rpd))
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
      item* LumpM = CreateLumpAtCharStack(matM,rpd.h,canBeAWoodenStick(itToUse,matM));
      if(IsMeltable(LumpM->GetMainMaterial()))
        LumpMeltable = LumpM;
    }

    if(matS!=NULL){
      item* LumpS = CreateLumpAtCharStack(matS,rpd.h,canBeAWoodenStick(itToUse,matS)); //must always be prepared to not lose it
      if(LumpS!=NULL)
        if(IsMeltable(LumpS->GetMainMaterial()) )
          if(LumpMeltable==NULL)
            LumpMeltable = LumpS;
    }

    // OBS.: one of the lumps will have to be turned into ingot later by user action

    ADD_MESSAGE("%s was completely dismantled.", itToUse->GetName(DEFINITE).CStr());
    rpd.bAlreadyExplained=true;
    craftcore::SafelySendToHell(itToUse);
//    itToUse->RemoveFromSlot(); //important to not crash elsewhere!!!
//    itToUse->SendToHell();
    DBG4("SentToHell",itToUse->GetID(),itToUse,LumpMeltable); //TODO if has any magic should release it and also harm

    rpd.bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...

    if(bJustLumpfyTheIngot){
      lumpMix(vitInv(rpd.h),LumpMeltable,rpd.bSpendCurrentTurn);
      rpd.bAlreadyExplained=true;
      return true;
    }

    return true;
  }
};srpDismantle rpDismantle;

struct srpSplitLump : public recipe{
  bool work(recipedata& rpd){
    if(desc.GetSize()==0){
      init("split","some lumps");
      desc << "Split one lump to be easier to work with.";
    }

    if(!recipe::work(rpd))
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
        CreateLumpAtCharStack(Lump->GetMainMaterial(), rpd.h);

      if(volRest>0)
        Lump->GetMainMaterial()->SetVolume(volPart+volRest);

      rpd.bAlreadyExplained=true; //no need to say anything
    }

    return true;
  }
};srpSplitLump rpSplitLump;

struct srpForgeItem : public srpUseForge{
  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("forge","an item");
      desc << "Using something as a hammer, close to an anvil and with a forge nearby you can create items.";
    }

    if(!srpUseForge::work(rpd))
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
          craftcore::SafelySendToHell(itSpawn);
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

    material* matM = itSpawn->GetMainMaterial();

    long lVolM = matM->GetVolume();
    if(lVolM==0)
      ABORT("main material 0 volume??? %s",itSpawn->GetName(DEFINITE).CStr());
    material* matS = itSpawn->GetSecondaryMaterial();

    long lVolS = 0;
    if(matS!=NULL){
      lVolS = matS->GetVolume();
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
      craftcore::SafelySendToHell(itSpawn);
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

    bool bIsWeapon = itSpawn->IsWeapon(rpd.h);
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
        craftcore::SafelySendToHell(itSpawn);
        return true;
      }
    }

    if(bReqS && !bAllowS)
      ABORT("item reqs secondary mat but doesnt allow it??? %s",itSpawn->GetName(DEFINITE).CStr());

    rpd.bHasAllIngredients=true;

    itSpawn->SetMainMaterial(material::MakeMaterial(iCfgM,lVolM));
    if(bAllowS)
      itSpawn->SetSecondaryMaterial(material::MakeMaterial(iCfgS,lVolS));
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
      if(!srpUseForge::findForge(rpd,true)){
        craftcore::SafelySendToHell(itSpawn);
        return true;
      }

      ////////////// find the anvil
      lsquare* lsqrAnvil = NULL;
      for(int iDir=0;iDir<8;iDir++){
        v2 v2Add = game::GetMoveVector(iDir);
        v2 v2Pos = rpd.h->GetPos() + v2Add; DBG3(DBGAV2(v2Add),DBGAV2(v2Pos),iDir);
        if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
          lsquare* lsqr = rpd.h->GetNearLSquare(v2Pos);DBG1(lsqr);
          olterrain* ot = lsqr->GetOLTerrain();
          if(ot!=NULL && ot->GetConfig() == ANVIL && lsqr->CanBeSeenBy(rpd.h)){
            lsqrAnvil = lsqr;
            rpd.v2AnvilLocation=lsqrAnvil->GetPos();
            break;
          }
        }
      }
      if(lsqrAnvil==NULL){
        ADD_MESSAGE("No anvil nearby."); //TODO allow user miss-chose
        rpd.bAlreadyExplained=true;
        craftcore::SafelySendToHell(itSpawn);
        return true;
      }
    }

    //TODO glass should require proper tools (don't know what but sure not a hammer)
    //TODO bone should require a dagger
    //TODO 2 tools, one for meltables and the other for glass or bone
    rpd.itTool = FindHammeringTool(rpd.h,rpd.iBaseTurnsToFinish);DBG1(rpd.iBaseTurnsToFinish);
    if(rpd.itTool==NULL){
      craftcore::SafelySendToHell(itSpawn);
      return true;
    }

    rpd.SetCanBeSuspended();

    rpd.CopySpawnItemCfgFrom(itSpawn);
    craftcore::SafelySendToHell(itSpawn);

    rpd.bCanStart=true;

    return true;
  }
};srpForgeItem rpForgeItem;

struct srpFluids : public recipe{
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
  srpFluids(){
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
    if(!recipe::work(rpd))
      return false;

    itemvector vi;

    ///////////// tool ////////////////
    rpd.itTool = FindTool(vitInv(rpd.h), DAGGER);
    if(rpd.itTool==NULL)
      return true;

    item* itCorpse=NULL;
    vi = vitInv(rpd.h);
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
    vi = vitInv(rpd.h);
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
      vi = vitInv(rpd.h);
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

struct srpPoison : public srpFluids{

  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Effect==EFFECT_POISON || flesh->Effect==EFFECT_POISON);
  }

  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("extract","some poison");
      desc << "Use a " << fsTool << " to " << action << " " << name << " from "
        << fsCorpse << " into a " << fsBottle <<  ".";
    }

    iLiqCfg=POISON_LIQUID;
    iMatEff=EFFECT_POISON;

    return srpFluids::work(rpd);
  }
};srpPoison rpPoison;

struct srpAcid : public srpFluids{
  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){
    return (blood->Acidicity)>0 || (flesh->Acidicity)>0;
  }

  bool work(recipedata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("extract","some acidous fluid");
      desc   << "Use a " << fsTool << " to " << action   << " " << name   << " from "
        << fsCorpse << " into a " << fsBottle <<  ".";
    }

    iLiqCfg=SULPHURIC_ACID;

    return srpFluids::work(rpd);
  }
};srpAcid rpAcid;

felist craftRecipes(CONST_S("What do you want to craft?"));
std::vector<recipe*> vrp;
void addRecipe(recipe* prp){
  static int iEntryIndex=0;
  craftRecipes.AddEntry(prp->name+" - "+prp->desc, LIGHT_GRAY, 20, prp->iListIndex=iEntryIndex++, true); DBG2(prp->name.CStr(),prp->iListIndex);
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

//  stack* pStackIngredients = new stack(NULL,Char);

  // collect requirements to display recipes
//  itemvector vitInv;
//  Char->GetStack()->FillItemVector(vitInv);
//  if(h->GetLeftWielded ())vitInv.push_back(h->GetLeftWielded ());
//  if(h->GetRightWielded())vitInv.push_back(h->GetRightWielded());

  recipedata rpd(h);
  rpd.lsqrCharPos = game::GetCurrentLevel()->GetLSquare(rpd.h->GetPos());

  //TODO check requirements and display recipes
  int iEntryIndex=0;

//  int Selected=-2; //default is -1 means not set, -2 to init
  if(vrp.size()>0){
    game::SetStandardListAttributes(craftRecipes);
    craftRecipes.AddFlags(SELECTABLE);
    rpd.Selected = craftRecipes.Draw(); DBG1(rpd.Selected);

    if(rpd.Selected & FELIST_ERROR_BIT)
      return false;
  }

  /******************************************************************************************
   * 1st call it just initializes the recipes list after all recipes have been configured!
   */
  bool bInitRecipes = vrp.size()==0;
  recipe* prp=NULL;
  #define RP(rp) \
    if(rp.work(rpd))prp=&rp; \
    if(bInitRecipes)addRecipe((recipe*)&rp);
  // these are kind of grouped and not ordered like a-z
  RP(rpChair);
  RP(rpDoor);
  RP(rpWall);
  RP(rpPoison);
  RP(rpAcid);
  RP(rpDismantle);
  RP(rpSplitLump);
  RP(rpMelt);
  RP(rpForgeItem);
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

//    object* pChk=NULL;
//    for(int i=0;i<2;i++){
//      bool bAbort=false;
//
//      switch(i){
//      case 0:
//        if(rpd.otSpawn!=NULL){
//          // anything to check here?
//        }
//        break;
//      case 1:
//        if(rpd.itSpawn!=NULL){
//          if(!craftcore::canBeCrafted(rpd.itSpawn))
//            bAbort=true;
//        }
//        break;
//      }
//
//      if(bAbort){
//        ABORT(
//          "Dear developer, for the sake of balance and challenge do not create recipes for:\n"
//          "- Quest items.\n"
//          "- Magical items as rings, amulets, wands, scrolls, horns etc.\n"
//          "Crafting any of this would be unbalanced as hell and unrealistic given your characters upbringing.\n"
//          "You're after all a slave, with no knowledge of magic, and crafting magical items should be beyond most craftsmen.\n"
//        );
//      }
//    }

    if(rpd.otSpawnCfg>0 || rpd.itSpawnCfg>0) {
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
      if(rpd.iBaseTurnsToFinish==0)
        rpd.iBaseTurnsToFinish=1;

      if(rpd.v2PlaceAt.Is0())
        rpd.v2PlaceAt = rpd.lsqrWhere!=NULL ? rpd.lsqrWhere->GetPos() : rpd.lsqrCharPos->GetPos(); //may be ignored anyway, is just a fallback

//      if(rpd.itSpawn!=NULL)
//        ABORT("crafting item shall only be configured, not spawned here.");

//      if(rpd.itSpawn!=NULL){
//        /**
//         * this is important because during crafting::handle it may item::Be on this item and it may require
//         * checking the item's slot that would be NULL w/o this line ex.: during a bone item spoilage
//         */
////        rpd.itSpawn->GetMainMaterial()->GetAlpha();
//        Char->GetStack()->AddItem(rpd.itSpawn);
//        if(rpd.itSpawn->GetSlot()==NULL)
//          ABORT("tmp crafting item should be on 'a slot' for consistency with code everywhere.");
//
//        rpd.itSpawnID=rpd.itSpawn->GetID();
//
//        rpd.itSpawnCfg = rpd.itSpawn->GetConfig();
//        rpd.itSpawnMatMainCfg = rpd.itSpawn->GetMainMaterial()->GetConfig();
//        rpd.itSpawnMatMainVol = rpd.itSpawn->GetMainMaterial()->GetVolume();
//        if(rpd.itSpawn->GetSecondaryMaterial()!=NULL){
//          rpd.itSpawnMatSecCfg = rpd.itSpawn->GetSecondaryMaterial()->GetConfig();
//          rpd.itSpawnMatSecVol = rpd.itSpawn->GetSecondaryMaterial()->GetVolume();
//        }
//
////        if(rpd.IsCanBeSuspended())//TODO should not depend on suspendability, should ALWAYS only spawn things when finished!!!
//          rpd.SendSpawnItemToHell();
//      }

//      if(rpd.otSpawn!=NULL)
//        ABORT("crafting terrain shall only be configured, not spawned here.");

//      if(rpd.otSpawn!=NULL){
//        rpd.otSpawnCfg = rpd.otSpawn->GetConfig();
//        rpd.otSpawnMatMainCfg = rpd.otSpawn->GetMainMaterial()->GetConfig();
//        rpd.otSpawnMatMainVol = rpd.otSpawn->GetMainMaterial()->GetVolume();
//        if(rpd.otSpawn->GetSecondaryMaterial()!=NULL){
//          rpd.otSpawnMatSecCfg = rpd.otSpawn->GetSecondaryMaterial()->GetConfig();
//          rpd.otSpawnMatSecVol = rpd.otSpawn->GetSecondaryMaterial()->GetVolume();
//        }
//
////        if(rpd.IsCanBeSuspended())//TODO should not depend on suspendability, should ALWAYS only spawn things when finished!!!
//          rpd.SendTerrainToHell();
//      }

      rpd.iAddDexterity=5; //TODO crafting difficult things should give more dexterity (wisdom too?)

      rpd.v2PlayerCraftingAt = Char->GetPos();

      if(rpd.itTool!=NULL)rpd.itToolID=rpd.itTool->GetID();

      if(rpd.lsqrWhere!=NULL)rpd.v2BuildWhere=rpd.lsqrWhere->GetPos();

//      rpd.ClearRefs(); //this is mainly to help on granting consistency. As in case of save/load w/o exiting the game app it will be required, at least during development this will help seeing were such requirements are missing.

      Char->SwitchToCraft(rpd); // everything must be set before this!!!

      ADD_MESSAGE("Let me work on %s now.",prp->name.CStr());
    }else{
      ABORT("crafting nothing?");
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

  if(rpd.bSpendCurrentTurn)
    return true;

  return false;
}

