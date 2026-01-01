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

/* Compiled through charset.cpp */

#include "dbgmsgproj.h"

/**
 * 5 seems good, broken cheap weapons, stones, very cheap weapons non broken etc
 * btw, lantern price is currently 10.
 */
int wizautoplay::iMaxValueless = 5;

#ifdef WIZARD

int wizautoplay::AutoPlayMode=AUTOPLAYMODE_DISABLED;

character* wizautoplay::P=NULL;

bool bSafePrayOnce=false;
void wizautoplay::AutoPlayAITeleport(bool bDeathCountBased)
{
  bool bTeleportNow=false;

  if(bDeathCountBased){ // this is good to prevent autoplay AI getting stuck endless dieing
    static int iDieMax=10;
    static int iDieTeleportCountDown=iDieMax;
    if(iDieTeleportCountDown==0){ //this helps on defeating not so strong enemies in spot
      if(IsPlayerAutoPlay())
        bTeleportNow=true;
      iDieTeleportCountDown=iDieMax;
      bSafePrayOnce=true;
    }else{
      static v2 v2DiePos(0,0);
      if(v2DiePos==P->GetPos()){
        iDieTeleportCountDown--;
      }else{
        v2DiePos=P->GetPos();
        iDieTeleportCountDown=iDieMax;
      }
    }
  }

  if(bTeleportNow)
    P->Move(P->GetLevel()->GetRandomSquare(P), true); //not using teleport function to avoid prompts, but this code is from there
}

character* AutoPlayLastChar=NULL;
const int iMaxWanderTurns=20;
const int iMinWanderTurns=3;

v2 v2KeepGoingTo=v2(0,0);
v2 v2TravelingToAnotherDungeon=v2(0,0);
int iWanderTurns=iMinWanderTurns;
bool bAutoPlayUseRandomNavTargetOnce=false;
std::vector<v2> vv2DebugDrawSqrPrevious;
v2 v2LastDropPlayerWasAt=v2(0,0);
std::vector<v2> vv2FailTravelToTargets;
std::vector<v2> vv2WrongGoingTo;

void wizautoplay::AutoPlayAIReset(bool bFailedToo)
{ DBG7(bFailedToo,iWanderTurns,DBGAV2(v2KeepGoingTo),DBGAV2(v2TravelingToAnotherDungeon),DBGAV2(v2LastDropPlayerWasAt),vv2FailTravelToTargets.size(),vv2DebugDrawSqrPrevious.size());
  v2KeepGoingTo=v2(0,0); //will retry
  v2TravelingToAnotherDungeon=v2(0,0);
  iWanderTurns=0; // warning: this other code was messing the logic ---> if(iWanderTurns<iMinWanderTurns)iWanderTurns=iMinWanderTurns; //to wander just a bit looking for random spot from where Route may work
  bAutoPlayUseRandomNavTargetOnce=false;
  v2LastDropPlayerWasAt=v2(0,0);
  vv2DebugDrawSqrPrevious.clear();

  PLAYER->TerminateGoingTo();

  if(bFailedToo){
    vv2FailTravelToTargets.clear();
    vv2WrongGoingTo.clear();
  }
}
truth wizautoplay::AutoPlayAISetAndValidateKeepGoingTo(v2 v2KGTo)
{
  v2KeepGoingTo=v2KGTo;

  bool bOk=true;

  if(bOk){
    lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2KeepGoingTo);
    if(!P->CanTheoreticallyMoveOn(lsqr))
      bOk=false;
//    olterrain* olt = game::GetCurrentLevel()->GetLSquare(v2KeepGoingTo)->GetOLTerrain();
//    if(olt){
//      if(bOk && !CanMoveOn(olt)){
//        DBG4(DBGAV2(v2KeepGoingTo),"olterrain? fixing it...",olt->GetNameSingular().CStr(),PLAYER->GetPanelName().CStr());
//        bOk=false;
//      }
//
//      /****
//       * keep these commented for awhile, may be useful later
//       *
//      if(bOk && olt->IsWall()){ //TODO this may be unnecessary cuz  of above test
//        //TODO is this a bug in the CanMoveOn() code? navigation AI is disabled when player is ghost TODO confirm about ethereal state, ammy of phasing
//        DBG4(DBGAV2(v2KeepGoingTo),"walls? fixing it...",olt->GetNameSingular().CStr(),PLAYER->GetPanelName().CStr());
//        bOk=false;
//      }
//
//      if(bOk && (olt->GetWalkability() & ETHEREAL)){ //TODO this may be too much unnecessary test
//        bOk=false;
//      }
//      */
//    }
  }

  if(bOk){
    P->SetGoingTo(v2KeepGoingTo); DBG3(DBGAV2(P->GetPos()),DBGAV2(P->GoingTo),DBGAV2(v2KeepGoingTo));
    P->CreateRoute();
    if(P->Route.empty()){
      P->TerminateGoingTo(); //redundant?
      bOk=false;
    }
  }

  if(!bOk){
    DBG1("RouteCreationFailed");
    vv2FailTravelToTargets.push_back(v2KeepGoingTo); DBG3("BlockGoToDestination",DBGAV2(v2KeepGoingTo),vv2FailTravelToTargets.size());
    bAutoPlayUseRandomNavTargetOnce=true;

    AutoPlayAIReset(false); //v2KeepGoingTo is reset here too
  }

  return bOk;
}

void wizautoplay::AutoPlayAIDebugDrawSquareRect(v2 v2SqrPos, col16 color, int iPrintIndex, bool bWide, bool bKeepColor)
{
  static v2 v2ScrPos=v2(0,0); //static to avoid instancing
  static int iAddPos;iAddPos=bWide?2:1;
  static int iSubBorder;iSubBorder=bWide?3:2;
  if(game::OnScreen(v2SqrPos)){
    v2ScrPos=game::CalculateScreenCoordinates(v2SqrPos);

    DOUBLE_BUFFER->DrawRectangle(
        v2ScrPos.X+iAddPos, v2ScrPos.Y+iAddPos,
        v2ScrPos.X+TILE_SIZE-iSubBorder, v2ScrPos.Y+TILE_SIZE-iSubBorder,
        color, bWide);

    if(iPrintIndex>-1)
      FONT->Printf(DOUBLE_BUFFER, v2(v2ScrPos.X+1,v2ScrPos.Y+5), DARK_GRAY, "%d", iPrintIndex);

    if(!bKeepColor)
      vv2DebugDrawSqrPrevious.push_back(v2SqrPos);
  }
}

const int iVisitAgainMax=10;
int iVisitAgainCount=iVisitAgainMax;
std::vector<lsquare*> vv2AllDungeonSquares;
bool wizautoplay::AutoPlayAICheckAreaLevelChangedAndReset()
{
  static area* areaPrevious=NULL;
  area* Area = game::GetCurrentArea();
  if(Area != areaPrevious){
    areaPrevious=Area;

    iVisitAgainCount=iVisitAgainMax;

    vv2DebugDrawSqrPrevious.clear();

    vv2AllDungeonSquares.clear();
    if(!game::IsInWilderness())
      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){ for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        vv2AllDungeonSquares.push_back(game::GetCurrentLevel()->GetLSquare(iX, iY));
      }}

    return true;
  }

  return false;
}

void wizautoplay::AutoPlayAIDebugDrawOverlay()
{
  if(!game::WizardModeIsActive())return;

  AutoPlayAICheckAreaLevelChangedAndReset();

  // redraw previous to clean them
  area* Area = game::GetCurrentArea(); //got the Area to draw in the wilderness too and TODO navigate there one day
  std::vector<v2> vv2DebugDrawSqrPreviousCopy(vv2DebugDrawSqrPrevious);
  for(int i=0;i<vv2DebugDrawSqrPreviousCopy.size();i++){
//    Area->GetSquare(vv2DebugDrawSqrPrevious[i])->SendNewDrawRequest();
//    square* sqr = Area->GetSquare(vv2DebugDrawSqrPrevious[i]);
//    if(sqr)sqr->SendStrongNewDrawRequest(); //TODO sqr NULL?
    AutoPlayAIDebugDrawSquareRect(vv2DebugDrawSqrPreviousCopy[i],DARK_GRAY);
  }

  // draw new ones
  vv2DebugDrawSqrPrevious.clear(); //empty before fillup below

  for(int i=0;i<vv2FailTravelToTargets.size();i++)
    AutoPlayAIDebugDrawSquareRect(vv2FailTravelToTargets[i],RED,i==(vv2FailTravelToTargets.size()-1),i,true);

  if(!PLAYER->Route.empty())
    for(int i=0;i<PLAYER->Route.size();i++)
      AutoPlayAIDebugDrawSquareRect(PLAYER->Route[i],GREEN);

  if(!v2KeepGoingTo.Is0())
    AutoPlayAIDebugDrawSquareRect(v2KeepGoingTo,BLUE,PLAYER->Route.size(),true);
  else if(iWanderTurns>0)
    AutoPlayAIDebugDrawSquareRect(PLAYER->GetPos(),YELLOW,iWanderTurns);

  for(int i=0;i<vv2WrongGoingTo.size();i++)
    AutoPlayAIDebugDrawSquareRect(vv2WrongGoingTo[i],BLUE,i,false,true);
}

truth wizautoplay::AutoPlayAIcanApply(item* it)
{
  if(!it->IsAppliable(P))return false;
  if(it->IsZappable(P))return false; //not here, see zap section
  if(dynamic_cast<key*>(it))return false; // too complex to make it auto work
  if(dynamic_cast<stethoscope*>(it))return false; // too complex to make it auto work
  return true;
}

truth wizautoplay::AutoPlayAIDropThings()
{
//  level* lvl = game::GetCurrentLevel(); DBG1(lvl);
//  area* Area = game::GetCurrentArea();

  /**
   *  unburden
   */
  bool bDropSomething = false;
  static item* eqDropChk=NULL;
  item* eqBroken=NULL;
  for(int i=0;i<P->GetEquipments();i++){
    eqDropChk=P->GetEquipment(i);
    if(eqDropChk!=NULL && eqDropChk->IsBroken()){ DBG2("chkDropBroken",eqDropChk);
      eqBroken=eqDropChk;
      bDropSomething=true;
      break;
    }
  }

  if(!bDropSomething && P->GetBurdenState() == STRESSED){
    if(clock()%100<5){ //5% chance to drop something weighty randomly every turn
      bDropSomething=true; DBGLN;
    }
  }

  if(!bDropSomething && P->GetBurdenState() == OVER_LOADED){
    bDropSomething=true;
  }

  if(bDropSomething){ DBG1("DropSomething");
    item* dropMe=NULL;
    if(eqBroken!=NULL)dropMe=eqBroken;

    item* heaviest=NULL;
    item* cheapest=NULL;

//    bool bFound=false;
//    for(int k=0;k<2;k++){
//      if(dropMe!=NULL)break;
//    static item* eqDropChk=NULL;
//    for(int i=0;i<GetEquipments();i++){
//      eqDropChk=GetEquipment(i);
//      if(eqDropChk!=NULL && eqDropChk->IsBroken()){
//        dropMe=eqDropChk;
//        break;
//      }
//    }

    if(dropMe==NULL){
      static itemvector vit;vit.clear();P->GetStack()->FillItemVector(vit);
      for(int i=0;i<vit.size();i++){ DBG4("CurrentChkToDrop",vit[i]->GetName(DEFINITE).CStr(),vit[i]->GetTruePrice(),vit[i]->GetWeight());
        if(vit[i]->IsEncryptedScroll())continue;
//        if(!bPlayerHasLantern && dynamic_cast<lantern*>(vit[i])!=NULL){
//          bPlayerHasLantern=true; //will keep only the 1st lantern
//          continue;
//        }

        if(vit[i]->IsBroken()){ //TODO use repair scroll?
          dropMe=vit[i];
          break;
        }

        if(heaviest==NULL)heaviest=vit[i];
        if(cheapest==NULL)cheapest=vit[i];

//        switch(k){
//        case 0: //better not implement this as a user function as that will remove the doubt about items values what is another fun challenge :)
          if(vit[i]->GetTruePrice() < cheapest->GetTruePrice()) //cheapest
            cheapest=vit[i];
//          break;
//        case 1: //this could be added as user function to avoid browsing the drop list, but may not be that good...
          if(vit[i]->GetWeight() > heaviest->GetWeight()) //heaviest
            heaviest=vit[i];
//          break;
//        }
      }
    }

    if(heaviest!=NULL && cheapest!=NULL){
      if(dropMe==NULL && heaviest==cheapest)
        dropMe=heaviest;

      if(dropMe==NULL && cheapest->GetTruePrice()<=iMaxValueless){ DBG2("DropValueless",cheapest->GetName(DEFINITE).CStr());
        dropMe=cheapest;
      }

      if(dropMe==NULL){
        // the worst price VS weight will be dropped
        float fC = cheapest ->GetTruePrice()/(float)cheapest ->GetWeight();
        float fW = heaviest->GetTruePrice()/(float)heaviest->GetWeight(); DBG3("PriceVsWeightRatio",fC,fW);
        if(fC < fW){
          dropMe = cheapest;
        }else{
          dropMe = heaviest;
        }
      }

      if(dropMe==NULL)
        dropMe = clock()%2==0 ? heaviest : cheapest;
    }

    // chose a throw direction
    if(dropMe!=NULL){
      static std::vector<int> vv2DirBase;static bool bDummyInit = [](){for(int i=0;i<8;i++)vv2DirBase.push_back(i);return true;}();
      std::vector<int> vv2Dir(vv2DirBase);
      int iDirOk=-1;
      v2 v2DropAt(0,0);
      lsquare* lsqrDropAt=NULL;
      for(int i=0;i<8;i++){
        int k = clock()%vv2Dir.size(); //random chose from remaining TODO could be where there is NPC foe
        int iDir = vv2Dir[k]; //collect direction value
        vv2Dir.erase(vv2Dir.begin() + k); //remove using the chosen index to prepare next random choice

        v2 v2Dir = game::GetMoveVector(iDir);
        v2 v2Chk = P->GetPos() + v2Dir;
        if(game::GetCurrentLevel()->IsValidPos(v2Chk)){
          lsquare* lsqrChk=game::GetCurrentLevel()->GetLSquare(v2Chk);
          if(lsqrChk->IsFlyable()){
            iDirOk = iDir;
            v2DropAt = v2Chk;
            lsqrDropAt=lsqrChk;
            break;
          }
        }
      };DBGLN;

      if(iDirOk==-1){iDirOk=clock()%8;DBG2("RandomDir",iDirOk);}DBGLN; //TODO should just drop may be? unless hitting w/e is there could help

      bool bApplyDropped=false; //or vanished
      if(AutoPlayAIcanApply(dropMe) && dropMe->Apply(P)){
        static itemvector ivChkDrop;ivChkDrop.clear();
        P->GetStack()->FillItemVector(ivChkDrop);
        bApplyDropped=true;
        for(int i6=0;i6<ivChkDrop.size();i6++){
          if(ivChkDrop[i6]==dropMe){
            bApplyDropped=false;
            break;
          }
        }
      }
      
      if(!bApplyDropped){
        if(iDirOk>-1){DBG2("KickOrThrow",iDirOk);
          static itemcontainer* itc;itc = dynamic_cast<itemcontainer*>(dropMe);DBGLN;
          static humanoid* h;h = dynamic_cast<humanoid*>(P);DBGLN;
          DBG8("CanKickLockedChest",lsqrDropAt,itc,itc?itc->IsLocked():-1,P->CanKick(),h,h?h->GetLeftLeg():0,h?h->GetRightLeg():0);
          if(lsqrDropAt && itc && itc->IsLocked() && P->CanKick() && h && h->GetLeftLeg() && h->GetRightLeg()){DBGLN;
            dropMe->MoveTo(lsqrDropAt->GetStack());DBGLN; //drop in front..
            P->Kick(lsqrDropAt,iDirOk,true);DBGLN; // ..to kick it
          }else{DBGLN;
            P->ThrowItem(iDirOk, dropMe); DBG5("DropThrow",iDirOk,dropMe->GetName(DEFINITE).CStr(),dropMe->GetTruePrice(),dropMe->GetWeight());
          }
        }else{DBGLN;
          dropMe->MoveTo(P->GetLSquareUnder()->GetStack());DBGLN; //just drop
        }

        v2LastDropPlayerWasAt=P->GetPos();DBGSV2(v2LastDropPlayerWasAt);
      }

      return true;
    }

    DBG1("AutoPlayNeedsImprovement:DropItem");
    ADD_MESSAGE("%s says \"I need more intelligence to drop trash...\"", P->GetName(DEFINITE).CStr()); // improve the dropping AI
    //TODO stop autoplay mode? if not, something random may happen some time and wont reach here ex.: spoil, fire, etc..
  }

  return false;
}

bool wizautoplay::IsAutoplayAICanPickup(item* it,bool bPlayerHasLantern)
{
  if(!it->CanBeSeenBy(P))return false;
  
  if(!it->IsPickable(P))return false;
  
  if(it->GetSquaresUnder()!=1)return false; //avoid big corpses 2x2

  if(!bPlayerHasLantern && (it->IsOnFire(P) || it->GetEmitation()>0)){
    //pickup priority
  }else{
    if(it->IsBroken())return false;
    if(it->GetTruePrice()<=iMaxValueless)return false; //mainly to avoid all rocks from broken walls
    if(clock()%3!=0 && it->GetSpoilLevel()>0)return false; //some spoiled may be consumed to randomly test diseases flows
  }

  return true;
}

truth wizautoplay::AutoPlayAIEquipAndPickup(bool bPlayerHasLantern)
{
  static humanoid* h;h = dynamic_cast<humanoid*>(P);
  if(h==NULL)return false;
  // other invalid equippers
  if(dynamic_cast<ghost*>(P) != NULL)return false;
  if(dynamic_cast<golem*>(P) != NULL)return false;

  if(AutoPlayAIequipConsumeZapReadApply())
    return true;

  if(P->GetBurdenState()!=OVER_LOADED){ DBG4(P->CommandFlags&DONT_CHANGE_EQUIPMENT,P,P->GetNameSingular().CStr(),P->GetSquareUnder());
    if(v2LastDropPlayerWasAt!=P->GetPos()){
      static bool bHoarder=true; //TODO wizard autoplay AI config exclusive felist

      if(P->CheckForUsefulItemsOnGround(false))
        if(!bHoarder)
          return true;

      //just pick up any useful stuff
      static itemvector vit;vit.clear();P->GetStackUnder()->FillItemVector(vit);
      for(uint c = 0; c < vit.size(); ++c){
        if(!IsAutoplayAICanPickup(vit[c],bPlayerHasLantern))continue;

        static itemcontainer* itc;itc = dynamic_cast<itemcontainer*>(vit[c]);
        if(itc && !itc->IsLocked()){ //get items from unlocked container
          static itemvector vitItc;vitItc.clear();itc->GetContained()->FillItemVector(vitItc);
          for(uint d = 0; d < vitItc.size(); ++d)
            vitItc[d]->MoveTo(itc->GetLSquareUnder()->GetStack());
          continue;
        }

        vit[c]->MoveTo(P->GetStack()); DBG2("pickup",vit[c]->GetNameSingular().CStr());
//          if(GetBurdenState()==OVER_LOADED)ThrowItem(clock()%8,ItemVector[c]);
//          return true;
        if(!bHoarder)
          return true;
      }
    }
  }

  return false;
}

static const int iMoreThanMaxDist=10000000; //TODO should be max integer but this will do for now in 2018 :)
truth wizautoplay::AutoPlayAITestValidPathTo(v2 v2To)
{
  return AutoPlayAIFindWalkDist(v2To) < iMoreThanMaxDist;
}

int wizautoplay::AutoPlayAIFindWalkDist(v2 v2To)
{
  static bool bUseSimpleDirectDist=false; //very bad navigation this is
  if(bUseSimpleDirectDist)return (v2To - P->GetPos()).GetLengthSquare();

  static v2 GoingToBkp;GoingToBkp = P->GoingTo; //IsGoingSomeWhere() ? GoingTo : v2(0,0);

  P->SetGoingTo(v2To);
  P->CreateRoute();
  static int iDist;iDist=P->Route.size();
  P->TerminateGoingTo();

  if(GoingToBkp!=ERROR_V2){ DBG2("Warning:WrongUsage:ShouldBeGoingNoWhere",DBGAV2(GoingToBkp));
    P->SetGoingTo(GoingToBkp);
    P->CreateRoute();
  }

  return iDist>0?iDist:iMoreThanMaxDist;
}

const int iDesperateResetCountDownDefault=5;
const int iDesperateEarthQuakeCountDownDefault=iDesperateResetCountDownDefault*2;
const int iAutoPlayAIResetCountDownDefault = iDesperateEarthQuakeCountDownDefault*2;
int iAutoPlayAIResetCountDown = iAutoPlayAIResetCountDownDefault;
truth wizautoplay::AutoPlayAINavigateDungeon(bool bPlayerHasLantern)
{
  /**
   * navigate the unknown dungeon
   */
  festring fsDL;fsDL<<P->GetDungeon()->GetIndex()<<P->GetLevel()->GetIndex();
  festring fsStayOnDL;{const char* pc = std::getenv("IVAN_DebugStayOnDungeonLevel");if(pc!=NULL)fsStayOnDL<<pc;}
  std::vector<v2> v2Exits;
  std::vector<v2> v2Altars;
  if(v2KeepGoingTo.Is0()){ DBG1("TryNewMoveTarget");
    // target undiscovered squares to explore
    v2 v2PreferedTarget(0,0);

    int iNearestLanterOnFloorDist = iMoreThanMaxDist;
    v2 v2PreferedLanternOnFloorTarget(0,0);

    v2 v2NearestUndiscovered(0,0);
    int iNearestUndiscoveredDist=iMoreThanMaxDist;
    std::vector<v2> vv2UndiscoveredValidPathSquares;

    lsquare* lsqrNearestSquareWithWallLantern=NULL;
    lsquare* lsqrNearestDropWallLanternAt=NULL;
    stack* stkNearestDropWallLanternAt = NULL;
    int iNearestSquareWithWallLanternDist=iMoreThanMaxDist;
    item* itNearestWallLantern=NULL;

    /***************************************************************
     * scan whole dungeon squares
     */
    for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){ for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
      lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(iX,iY);

      olterrain* olt = lsqr->GetOLTerrain();
      if(olt && (olt->GetConfig() == STAIRS_UP || olt->GetConfig() == STAIRS_DOWN)){
        if(fsDL!=fsStayOnDL){
          v2Exits.push_back(v2(lsqr->GetPos()));
          DBGSV2(v2Exits[v2Exits.size()-1]);
        }
      }
      
      altar* Altar = dynamic_cast<altar*>(olt);
      if(olt && Altar){
        if(!Altar->GetMasterGod()->IsKnown())
          v2Altars.push_back(v2(lsqr->GetPos()));
      }

      stack* stkSqr = lsqr->GetStack();
      static itemvector vit;vit.clear();stkSqr->FillItemVector(vit);
      bool bAddValidTargetSquare=true;

      // find nearest wall lantern
      if(!bPlayerHasLantern && olt && olt->IsWall()){
        for(int n=0;n<vit.size();n++){
          if(vit[n]->IsLanternOnWall() && !vit[n]->IsBroken()){
            static stack* stkDropWallLanternAt;stkDropWallLanternAt = lsqr->GetStackOfAdjacentSquare(vit[n]->GetSquarePosition());
            static lsquare* lsqrDropWallLanternAt;lsqrDropWallLanternAt =
              stkDropWallLanternAt?stkDropWallLanternAt->GetLSquareUnder():NULL;

            if(stkDropWallLanternAt && lsqrDropWallLanternAt && P->CanTheoreticallyMoveOn(lsqrDropWallLanternAt)){
              int iDist = AutoPlayAIFindWalkDist(lsqrDropWallLanternAt->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();
              if(lsqrNearestSquareWithWallLantern==NULL || iDist<iNearestSquareWithWallLanternDist){
                iNearestSquareWithWallLanternDist=iDist;

                lsqrNearestSquareWithWallLantern=lsqr;
                itNearestWallLantern=vit[n]; DBG3(iNearestSquareWithWallLanternDist,DBGAV2(lsqr->GetPos()),DBGAV2(P->GetPos()));
                lsqrNearestDropWallLanternAt=lsqrDropWallLanternAt;
                stkNearestDropWallLanternAt=stkDropWallLanternAt;
              }
            }

            break;
          }
        }
      }

      if(bAddValidTargetSquare && !P->CanTheoreticallyMoveOn(lsqr))
        bAddValidTargetSquare=false;

      bool bIsFailToTravelSquare=false;
      if(bAddValidTargetSquare){
        for(int j=0;j<vv2FailTravelToTargets.size();j++)
          if(vv2FailTravelToTargets[j]==lsqr->GetPos()){
            bAddValidTargetSquare=false;
            bIsFailToTravelSquare=true;
            break;
          }
      }

      if(!bIsFailToTravelSquare){

//          if(bAddValidTargetSquare && v2PreferedTarget.Is0() && (lsqr->HasBeenSeen() || !bPlayerHasLantern)){
        if(bAddValidTargetSquare && (lsqr->HasBeenSeen() || !bPlayerHasLantern)){
          bool bVisitAgain=false;
          if(iVisitAgainCount>0 || !bPlayerHasLantern){
            if(stkSqr!=NULL && stkSqr->GetItems()>0){
              for(int n=0;n<vit.size();n++){ DBG1(vit[n]);DBG1(vit[n]->GetID());DBG1(vit[n]->GetType());DBG3("VisitAgain:ChkItem",vit[n]->GetNameSingular().CStr(),vit.size());
                if(vit[n]->IsBroken())continue; DBGLN;

                static bool bIsLanternOnFloor;bIsLanternOnFloor = dynamic_cast<lantern*>(vit[n])!=NULL;// || vit[n]->IsOnFire(P); DBGLN;

                if( // if is useful to the AutoPlay AI endless tests
                  vit[n]->IsShield  (P) ||
                  vit[n]->IsWeapon  (P) ||
                  vit[n]->IsArmor   (P) ||
                  vit[n]->IsAmulet  (P) ||
                  vit[n]->IsZappable(P) || //wands
                  vit[n]->IsAppliable(P) || //mines, beartraps etc
                  vit[n]->IsRing    (P) ||
                  vit[n]->IsReadable(P) || //books and scrolls
                  vit[n]->IsDrinkable(P)|| //potions and vials
                  bIsLanternOnFloor
                )
                  if(IsAutoplayAICanPickup(vit[n],bPlayerHasLantern))
                  {
                    bVisitAgain=true;

                    if(bIsLanternOnFloor && !bPlayerHasLantern){
                      static int iDist;iDist = AutoPlayAIFindWalkDist(lsqr->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();
                      if(iDist<iNearestLanterOnFloorDist){
                        iNearestLanterOnFloorDist=iDist;
                        v2PreferedLanternOnFloorTarget = lsqr->GetPos(); DBG2("PreferLanternAt",DBGAV2(lsqr->GetPos()))
                      }
                    }else{
                      iVisitAgainCount--;
                    }

                    DBG4(bVisitAgain,DBGAV2(lsqr->GetPos()),iVisitAgainCount,bIsLanternOnFloor);
                    break;
                  }
              }
            }
          }

          if(!bVisitAgain)bAddValidTargetSquare=false;
        }

      }

      if(bAddValidTargetSquare)
        if(!P->CanTheoreticallyMoveOn(lsqr)) //if(olt && !CanMoveOn(olt))
          bAddValidTargetSquare=false;

      if(bAddValidTargetSquare){ DBG2("addValidSqr",DBGAV2(lsqr->GetPos()));
        static int iDist;iDist=AutoPlayAIFindWalkDist(lsqr->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();

        if(iDist<iMoreThanMaxDist) //add only valid paths
          vv2UndiscoveredValidPathSquares.push_back(lsqr->GetPos());

        if(iDist<iNearestUndiscoveredDist){
          iNearestUndiscoveredDist=iDist;
          v2NearestUndiscovered=lsqr->GetPos(); DBG3(iNearestUndiscoveredDist,DBGAV2(lsqr->GetPos()),DBGAV2(P->GetPos()));
        }
      }
    }} DBG2(DBGAV2(v2PreferedTarget),vv2UndiscoveredValidPathSquares.size());

    /***************************************************************
     * define prefered navigation target
     */
    if(!bPlayerHasLantern && v2PreferedTarget.Is0()){
      bool bUseWallLantern=false;
      if(!v2PreferedLanternOnFloorTarget.Is0() && lsqrNearestSquareWithWallLantern!=NULL){
        if(iNearestLanterOnFloorDist <= iNearestSquareWithWallLanternDist){
          v2PreferedTarget=v2PreferedLanternOnFloorTarget;
        }else{
          bUseWallLantern=true;
        }
      }else if(!v2PreferedLanternOnFloorTarget.Is0()){
        v2PreferedTarget=v2PreferedLanternOnFloorTarget;
      }else if(lsqrNearestSquareWithWallLantern!=NULL){
        bUseWallLantern=true;
      }

      if(bUseWallLantern){
        /**
         * target to nearest wall lantern
         * check for lanterns on walls of adjacent squares if none found on floors
         */
        itNearestWallLantern->MoveTo(stkNearestDropWallLanternAt); // the AI is prepared to get things from the floor only so "magically" drop it :)
        v2PreferedTarget = lsqrNearestDropWallLanternAt->GetPos(); DBG2("PreferWallLanternAt",DBGAV2(lsqrNearestDropWallLanternAt->GetPos()))
      }

    }

    /***************************************************************
     * validate and set new navigation target
     */
//    DBG9("AllNavigatePossibilities",DBGAV2(v2PreferedTarget),DBGAV2(v2PreferedLanternOnFloorTarget),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2());
    v2 v2NewKGTo=v2(0,0);

    if(v2NewKGTo.Is0()){
      //TODO if(!v2PreferedTarget.Is0){ // how can this not be compiled? error: cannot convert ‘v2::Is0’ from type ‘truth (v2::)() const {aka bool (v2::)() const}’ to type ‘bool’
      if(v2PreferedTarget.GetLengthSquare()>0)
        if(AutoPlayAITestValidPathTo(v2PreferedTarget))
          v2NewKGTo=v2PreferedTarget; DBGSV2(v2PreferedTarget);
    }

    if(v2NewKGTo.Is0()){
      if(bAutoPlayUseRandomNavTargetOnce){ //these targets were already path validated and are safe to use!
        v2NewKGTo=vv2UndiscoveredValidPathSquares[clock()%vv2UndiscoveredValidPathSquares.size()]; DBG2("RandomTarget",DBGAV2(v2NewKGTo));
        bAutoPlayUseRandomNavTargetOnce=false;
      }else{    //find nearest
        if(!v2NearestUndiscovered.Is0()){
          v2NewKGTo=v2NearestUndiscovered; DBGSV2(v2NearestUndiscovered);
        }
      }
    }

    if(v2NewKGTo.Is0()){ //no new destination: fully explored
      if(v2Altars.size()>0){
        for(int i=0;i<v2Altars.size();i++){
          if(AutoPlayAITestValidPathTo(v2Altars[i])){
            v2NewKGTo = v2Altars[i];
            break;
          }
        }
      }
    }
    
    if(v2NewKGTo.Is0()){
      if(v2Exits.size()>0){
        if(game::GetCurrentDungeonTurnsCount()==0){ DBG1("Dungeon:FullyExplored:FirstTurn");
          iWanderTurns=100+clock()%300; DBG2("WanderALotOnFullyExploredLevel",iWanderTurns); //just move around a lot, some NPC may spawn
        }else{
          // travel between dungeons if current fully explored
          v2 v2Try;
          for(int i=0;i<10;i++){
            v2Try = v2Exits[clock()%v2Exits.size()];
            if(v2Try!=P->GetPos())break;
          }
          if(AutoPlayAITestValidPathTo(v2Try) || iAutoPlayAIResetCountDown==0)
            v2NewKGTo = v2TravelingToAnotherDungeon = v2Try; DBGSV2(v2TravelingToAnotherDungeon);
        }
      }else{
        DBG1("AutoPlayNeedsImprovement:Navigation")
        ADD_MESSAGE("%s says \"I need more intelligence to move around...\"", P->GetName(DEFINITE).CStr()); // improve the dropping AI
        //TODO stop autoplay mode?
      }
    }

    if(v2NewKGTo.Is0()){ DBG1("Desperately:TryAnyRandomTargetNavWithValidPath");
      std::vector<lsquare*> vlsqrChk(vv2AllDungeonSquares);

      while(vlsqrChk.size()>0){
        static int i;i=clock()%vlsqrChk.size();
        static v2 v2Chk; v2Chk = vlsqrChk[i]->GetPos();

        if(!AutoPlayAITestValidPathTo(v2Chk)){
          vlsqrChk.erase(vlsqrChk.begin()+i);
        }else{
          v2NewKGTo=v2Chk;
          break;
        }
      }
    }

    if(!v2NewKGTo.Is0()){
      AutoPlayAISetAndValidateKeepGoingTo(v2NewKGTo);
    }else{
      DBG1("TODO:too complex paths are failing... improve CreateRoute()?");
    }
  }

  if(!v2KeepGoingTo.Is0()){
    if(v2KeepGoingTo==P->GetPos()){ DBG3("ReachedDestination",DBGAV2(v2KeepGoingTo),DBGAV2(P->GoingTo));
      //wander a bit before following new target destination
      iWanderTurns=(clock()%iMaxWanderTurns)+iMinWanderTurns; DBG2("WanderAroundAtReachedDestination",iWanderTurns);

//      v2KeepGoingTo=v2(0,0);
//      TerminateGoingTo();
      AutoPlayAIReset(false);
      return true;
    }

//    CheckForUsefulItemsOnGround(false); DBGSV2(GoingTo);
//    CheckForEnemies(false, true, false, false); DBGSV2(GoingTo);

//    if(!IsGoingSomeWhere() || v2KeepGoingTo!=GoingTo){ DBG3("ForceKeepGoingTo",DBGAV2(v2KeepGoingTo),DBGAV2(GoingTo));
//      SetGoingTo(v2KeepGoingTo);
//    }
    static int iForceGoingToCountDown=10;
    static v2 v2GoingToBkp;v2GoingToBkp=P->GoingTo;
    if(!v2KeepGoingTo.IsAdjacent(P->GoingTo)){
      if(iForceGoingToCountDown==0){
        DBG4("ForceKeepGoingTo",DBGAV2(v2KeepGoingTo),DBGAV2(P->GoingTo),DBGAV2(P->GetPos()));

        if(!AutoPlayAISetAndValidateKeepGoingTo(v2KeepGoingTo)){
          static int iSetFailTeleportCountDown=10;
          iSetFailTeleportCountDown--;
          vv2WrongGoingTo.push_back(v2GoingToBkp);
          if(iSetFailTeleportCountDown==0){
            AutoPlayAITeleport(false);
            AutoPlayAIReset(true); //refresh to test/try it all again
            iSetFailTeleportCountDown=10;
          }
        }
        DBGSV2(P->GoingTo);
        return true;
      }else{
        iForceGoingToCountDown--; DBG1(iForceGoingToCountDown);
      }
    }else{
      iForceGoingToCountDown=10;
    }

    /**
     * Determinedly blindly moves towards target, the goal is to Navigate!
     *
     * this has several possible status if returning false...
     * so better do not decide anything based on it?
     */
    P->MoveTowardsTarget(false);

//    if(!MoveTowardsTarget(false)){ DBG3("OrFailedGoingTo,OrReachedDestination...",DBGAV2(GoingTo),DBGAV2(GetPos())); // MoveTowardsTarget may break the GoingTo EVEN if it succeeds?????
//      TerminateGoingTo();
//      v2KeepGoingTo=v2(0,0); //reset only this one to try again
//      GetAICommand(); //wander once for randomicity
//    }

    return true;
  }

  return false;
}

bool wizautoplay::AutoPlayAIChkInconsistency()
{
  if(P->GetSquareUnder()==NULL){
    DBG9(P,P->GetNameSingular().CStr(),P->IsPolymorphed(),P->IsHuman(),P->IsHumanoid(),P->IsPolymorphable(),P->IsPlayerKind(),P->IsTemporary(),P->IsPet());
    DBG6("GetSquareUnderIsNULLhow?",P->IsHeadless(),P->IsPlayer(),wizautoplay::GetAutoPlayMode(),IsPlayerAutoPlay(),P->GetName(DEFINITE).CStr());
    return true; //to just ignore this turn expecting on next it will be ok.
  }
  return false;
}

truth wizautoplay::AutoPlayAIPray()
{
  bool bSPO = bSafePrayOnce;
  bSafePrayOnce=false;

  if(bSPO){}
  else if(P->StateIsActivated(PANIC) && clock()%10==0){
    iWanderTurns=1; DBG1("Wandering:InPanic"); // to regain control as soon it is a ghost anymore as it can break navigation when inside walls
  }else return false;

  // check for known gods
  int aiKGods[GODS];
  int iKGTot=0;
  int aiKGodsP[GODS];
  int iKGTotP=0;
  static int iPleased=50; //see god::PrintRelation()
  for(int c = 1; c <= GODS; ++c){
    if(!game::GetGod(c)->IsKnown())continue;
    // even known, praying to these extreme ones will be messy if Relation<1000
    if(dynamic_cast<valpurus*>(game::GetGod(c))!=NULL && game::GetGod(c)->GetRelation()<1000)continue;
    if(dynamic_cast<mortifer*>(game::GetGod(c))!=NULL && game::GetGod(c)->GetRelation()<1000)continue;

    aiKGods[iKGTot++]=c;

    if(game::GetGod(c)->GetRelation() > iPleased){
//      //TODO could this help?
//      switch(game::GetGod(c)->GetBasicAlignment()){ //game::GetGod(c)->GetAlignment();
//        case GOOD:
//          if(game::GetPlayerAlignment()>=2){}else continue;
//          break;
//        case NEUTRAL:
//          if(game::GetPlayerAlignment()<2 && game::GetPlayerAlignment()>-2){}else continue;
//          break;
//        case EVIL:
//          if(game::GetPlayerAlignment()<=-2){}else continue;
//          break;
//      }
      aiKGodsP[iKGTotP++] = c;
    }
  }
  if(iKGTot==0){
    if(clock()%10==0)
      for(int c = 1; c <= GODS; ++c)
        game::LearnAbout(game::GetGod(c));
    return false;
  }
//  if(bSPO && iKGTotP==0)return false;

  // chose and pray to one god
  god* g = NULL;
  if(iKGTotP>0 && (bSPO || clock()%10!=0))
    g = game::GetGod(aiKGodsP[clock()%iKGTotP]); //only good effect
  else
    g = game::GetGod(aiKGods[clock()%iKGTot]); //can have bad effect too

  if(bSPO || clock()%10!=0){ //it may not recover some times to let pray unsafely
    int iRecover=0;
    if(iKGTotP==0){
      if(iRecover==0 && g->GetRelation()==-1000)iRecover=1000; //to test all relation range
      if(iRecover==0 && g->GetRelation() <= iPleased)iRecover=iPleased; //to alternate tests on many with low good relation
    }
    if(iRecover>0)
      g->SetRelation(iRecover);

    g->AdjustTimer(-1000000000); //TODO filter gods using timer too instead of this reset?
  }

  g->Pray(); DBG2("PrayingTo",g->GetName());

  return true;
}

truth wizautoplay::AutoPlayAICommand(int& rKey)
{
  DBGLN;if(AutoPlayAIChkInconsistency())return true;
  DBGSV2(P->GetPos());

  if(AutoPlayLastChar!=P){
    AutoPlayAIReset(true);
    AutoPlayLastChar=P;
  }

  DBGLN;if(AutoPlayAIChkInconsistency())return true;
  if(AutoPlayAICheckAreaLevelChangedAndReset())
    AutoPlayAIReset(true);

  static bool bDummy_initDbg = [](){game::AddDebugDrawOverlayFunction(&AutoPlayAIDebugDrawOverlay);return true;}();

  truth bPlayerHasLantern=false;
  static itemvector vit;vit.clear();P->GetStack()->FillItemVector(vit);
  for(uint i=0;i<vit.size();i++){
    if(dynamic_cast<lantern*>(vit[i])!=NULL || vit[i]->IsOnFire(P) || vit[i]->GetEmitation()>0){
      bPlayerHasLantern=true; //will keep only the 1st lantern
      break;
    }
  }

  DBGLN;if(AutoPlayAIChkInconsistency())return true;
  AutoPlayAIPray();

  //TODO this doesnt work??? -> if(IsPolymorphed()){ //to avoid some issues TODO but could just check if is a ghost
//  if(dynamic_cast<humanoid*>(P) == NULL){ //this avoid some issues TODO but could just check if is a ghost
//  if(StateIsActivated(ETHEREAL_MOVING)){ //this avoid many issues
  static bool bPreviousTurnWasGhost=false;
  if(dynamic_cast<ghost*>(P) != NULL){ DBG1("Wandering:Ghost"); //this avoid many issues mainly related to navigation
    iWanderTurns=1; // to regain control as soon it is a ghost anymore as it can break navigation when inside walls
    bPreviousTurnWasGhost=true;
  }else{
    if(bPreviousTurnWasGhost){
      AutoPlayAIReset(true); //this may help on navigation
      bPreviousTurnWasGhost=false;
      return true;
    }
  }

  DBGLN;if(AutoPlayAIChkInconsistency())return true;
  if(AutoPlayAIDropThings())
    return true;

  DBGLN;if(AutoPlayAIChkInconsistency())return true;
  if(AutoPlayAIEquipAndPickup(bPlayerHasLantern))
    return true;

  if(iWanderTurns>0){
    if(!P->IsPlayer() || wizautoplay::GetAutoPlayMode()==AUTOPLAYMODE_DISABLED || !IsPlayerAutoPlay()){ //redundancy: yep
      DBG9(P,P->GetNameSingular().CStr(),P->IsPolymorphed(),P->IsHuman(),P->IsHumanoid(),P->IsPolymorphable(),P->IsPlayerKind(),P->IsTemporary(),P->IsPet());
      DBG5(P->IsHeadless(),P->IsPlayer(),wizautoplay::GetAutoPlayMode(),IsPlayerAutoPlay(),P->GetName(DEFINITE).CStr());
      ABORT("autoplay is inconsistent %d %d %d %d %d %s %d %s %d %d %d %d %d",
        P->IsPolymorphed(),P->IsHuman(),P->IsHumanoid(),P->IsPolymorphable(),P->IsPlayerKind(),
        P->GetNameSingular().CStr(),wizautoplay::GetAutoPlayMode(),P->GetName(DEFINITE).CStr(),
        P->IsTemporary(),P->IsPet(),P->IsHeadless(),P->IsPlayer(),IsPlayerAutoPlay());
    }
    P->GetAICommand(); DBG2("Wandering",iWanderTurns); //fallback to default TODO never reached?
    iWanderTurns--;
    return true;
  }

  /***************************************************************************************************
   * WANDER above here
   * NAVIGATE below here
   ***************************************************************************************************/

  /**
   * travel between dungeons
   */
  bool bTBD = false;
  if(!v2TravelingToAnotherDungeon.Is0()){
    if(P->GetPos() == v2TravelingToAnotherDungeon)
      bTBD=true;
    else
    if(iAutoPlayAIResetCountDown==0){
      P->Move(v2TravelingToAnotherDungeon,true);
      iAutoPlayAIResetCountDown=iAutoPlayAIResetCountDownDefault;
      bTBD=true;
    }
  }
  if(bTBD){
    bool bTravel=false;
    lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2TravelingToAnotherDungeon);
//    square* sqr = Area->GetSquare(v2TravelingToAnotherDungeon);
    olterrain* ot = lsqr->GetOLTerrain();
//    oterrain* ot = sqr->GetOTerrain();
    if(ot){
      if(ot->GetConfig() == STAIRS_UP){
        rKey='<';
        bTravel=true;
      }

      if(ot->GetConfig() == STAIRS_DOWN){
        rKey='>';
        bTravel=true;
      }
    }

    if(bTravel){ DBG3("travel",DBGAV2(v2TravelingToAnotherDungeon),rKey);
      AutoPlayAIReset(true);
      return false; //so the new/changed key will be used as command, otherwise it would be ignored
    }
  }

  static int iDesperateEarthQuakeCountDown=iDesperateEarthQuakeCountDownDefault;
  if(AutoPlayAINavigateDungeon(bPlayerHasLantern)){
    iDesperateEarthQuakeCountDown=iDesperateEarthQuakeCountDownDefault;
    return true;
  }else{
    if(iDesperateEarthQuakeCountDown==0){
      iDesperateEarthQuakeCountDown=iDesperateEarthQuakeCountDownDefault;
      /**
       * this changes the dungeon level paths, 
       * so applying pickaxe or using fireballs etc are not required!
       */
      scrollofearthquake::Spawn()->FinishReading(P);
      DBG1("UsingTerribleEarthquakeSolution"); // xD
    }else{
      iDesperateEarthQuakeCountDown--;
      DBG1(iDesperateEarthQuakeCountDown);
    }
  }

  /****************************************
   * Twighlight zone
   */

  ADD_MESSAGE("%s says \"I need more intelligence to do things by myself...\"", P->GetName(DEFINITE).CStr()); DBG1("TODO: AI needs improvement");

  static int iDesperateResetCountDown=iDesperateResetCountDownDefault;
  if(iDesperateResetCountDown==0){
    iDesperateResetCountDown=iDesperateResetCountDownDefault;

    AutoPlayAIReset(true);
    iAutoPlayAIResetCountDown--;

    // AFTER THE RESET!!!
    iWanderTurns=iMaxWanderTurns; DBG2("DesperateResetToSeeIfAIWorksAgain",iWanderTurns);
  }else{
    P->GetAICommand(); DBG2("WanderingDesperatelyNotKnowingWhatToDo",iDesperateResetCountDown); // :)
    iDesperateResetCountDown--;
  }

  return true;
}

truth wizautoplay::IsPlayerAutoPlay(character* C)
{
  return C->IsPlayer() && wizautoplay::GetAutoPlayMode()>0;
}

truth wizautoplay::AutoPlayAIequipConsumeZapReadApply()
{
  humanoid* H = dynamic_cast<humanoid*>(P);
  if(!H)
    return false;
  
  bool bDidSomething=false;
  
  ///////////////////////////////// WIELD
  
  item* iL = H->GetEquipment(LEFT_WIELDED_INDEX);
  item* iR = H->GetEquipment(RIGHT_WIELDED_INDEX);

  //every X turns remove all equipments
  bool bTryWieldNow=false;
  static int iLastReEquipAllTurn=-1;
  if(game::GetTurn()>(iLastReEquipAllTurn+100)){ DBG2(game::GetTurn(),iLastReEquipAllTurn);
    iLastReEquipAllTurn=game::GetTurn();
    DBG1("UnequipAll");
    for(int i=0;i<MAX_EQUIPMENT_SLOTS;i++){
      item* eq = H->GetEquipment(i);
      if(eq){eq->MoveTo(H->GetStack());H->SetEquipment(i,NULL);} //eq is moved to end of stack!
      if(iL==eq)iL=NULL;
      if(iR==eq)iR=NULL;
    }
//        if(iL!=NULL){iL->MoveTo(GetStack());iL=NULL;SetEquipment(LEFT_WIELDED_INDEX ,NULL);}
//        if(iR!=NULL){iR->MoveTo(GetStack());iR=NULL;SetEquipment(RIGHT_WIELDED_INDEX,NULL);}
    bTryWieldNow=true;
  }

  //wield some weapon from the inventory as the NPC AI is not working for the player TODO why?
  //every X turns try to wield
  static int iLastTryToWieldTurn=-1;
  if(bTryWieldNow || game::GetTurn()>(iLastTryToWieldTurn+10)){ DBG2(game::GetTurn(),iLastTryToWieldTurn);
    iLastTryToWieldTurn=game::GetTurn();
    bool bDoneLR=false;
    bool bL2H = iL && iL->IsTwoHanded();
    bool bR2H = iR && iR->IsTwoHanded();

    //2handed
    static int iTryWieldWhat=0; iTryWieldWhat++; DBG1(iTryWieldWhat);
    if(iTryWieldWhat%2==0){ //will try 2handed first, alternating. If player has only 2handeds, the 1handeds will not be wielded and it will use punches, what is good too for tests.
      if( !bDoneLR &&
          iL==NULL && H->GetBodyPartOfEquipment(LEFT_WIELDED_INDEX )!=NULL &&
          iR==NULL && H->GetBodyPartOfEquipment(RIGHT_WIELDED_INDEX)!=NULL
      ){
        static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
        for(uint c = 0; c < vitEqW.size(); ++c){
          if(vitEqW[c]->IsWeapon(P) && vitEqW[c]->IsTwoHanded()){  DBG1(vitEqW[c]->GetNameSingular().CStr());
            vitEqW[c]->RemoveFromSlot();
            DBG2("Wield2hd",vitEqW[c]->GetNameSingular().CStr());
            H->SetEquipment(clock()%2==0 ? LEFT_WIELDED_INDEX : RIGHT_WIELDED_INDEX, vitEqW[c]); //DBG3("Wield",iEqIndex,vitEqW[c]->GetName(DEFINITE).CStr());
            bDoneLR=true;
            break;
          }
        }
      }
    }

    //dual 1handed (if not 2hd already)
    if(!bDoneLR){
      for(int i=0;i<2;i++){
        int iChk=-1;
        if(i==0)iChk=LEFT_WIELDED_INDEX;
        if(i==1)iChk=RIGHT_WIELDED_INDEX;

        if(
            !bDoneLR &&
            (
              (iChk==LEFT_WIELDED_INDEX  && iL==NULL && H->GetBodyPartOfEquipment(LEFT_WIELDED_INDEX ) && !bR2H)
              ||
              (iChk==RIGHT_WIELDED_INDEX && iR==NULL && H->GetBodyPartOfEquipment(RIGHT_WIELDED_INDEX) && !bL2H)
            )
        ){
          static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
          for(uint c = 0; c < vitEqW.size(); ++c){
            if(
                (vitEqW[c]->IsWeapon(P) && !vitEqW[c]->IsTwoHanded())
                ||
                vitEqW[c]->IsShield(P)
            ){ 
              DBG2("WieldDual",vitEqW[c]->GetNameSingular().CStr());
              vitEqW[c]->RemoveFromSlot();
              H->SetEquipment(iChk, vitEqW[c]);
              bDoneLR=true;
              break;
            }
          }
        }
      }
    }

  }

  //every X turns try to use stuff from inv
  static int iLastTryToUseInvTurn=-1;
  if(game::GetTurn()>(iLastTryToUseInvTurn+5)){
    DBG2(game::GetTurn(),iLastTryToUseInvTurn);
    iLastTryToUseInvTurn=game::GetTurn();

    //////////////////////////////// consume food/drink
    { //TODO let this happen for non-human too?
      static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(clock()%3!=0 && H->GetHungerState() >= BLOATED)break; //randomly let it vomit and activate all related flows *eew* xD

        //if(TryToConsume(vitEqW[c]))
        material* ConsumeMaterial = vitEqW[c]->GetConsumeMaterial(P);
        if(
          ConsumeMaterial!=NULL &&
          vitEqW[c]->IsConsumable() &&
          !H->HasHadBodyPart(vitEqW[c]) && //this avoids a slow interactive question
          H->ConsumeItem(vitEqW[c], vitEqW[c]->GetConsumeMaterial(P)->GetConsumeVerb())
        ){
          DBG2("AutoPlayConsumed",vitEqW[c]->GetNameSingular().CStr());
          bDidSomething=true;
          break;
        }
      }
    }

    //////////////////////////////// equip armor ring amulet etc
    {
      static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(H->TryToEquip(vitEqW[c],true)){ 
          DBG2("EquipItem",vitEqW[c]->GetNameSingular().CStr());
          bDidSomething=true;
          break;
        }else{
          vitEqW[c]->MoveTo(H->GetStack()); //was dropped, get back, will be in the end of the stack! :)
        }
      }
    }

    //////////////////////////////// zap
    static int iLastZapTurn=-1;
    if(game::GetTurn()>(iLastZapTurn+30)){ 
      DBG2(game::GetTurn(),iLastZapTurn); //every X turns try to use stuff from inv
      iLastZapTurn=game::GetTurn();

      int iDir=clock()%(8+1); // index 8 is the macro YOURSELF already... if(iDir==8)iDir=YOURSELF;
      static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(!vitEqW[c]->IsZappable(P))continue;

        if(vitEqW[c]->IsZapWorthy(P)){
          if(vitEqW[c]->Zap(P, H->GetPos(), iDir)){
            DBG2(iLastZapTurn,vitEqW[c]->GetNameSingular().CStr()); //TODO try to aim at NPCs
            bDidSomething=true;
            break;
          }
        }else{
          if(vitEqW[c]->Apply(P)){ 
            DBG2(iLastZapTurn,vitEqW[c]->GetNameSingular().CStr());
            bDidSomething=true;
            break;
          }
        }
      }
    }

    //////////////////////////////// read books and scrolls
    static int iLastReadTurn=-1;
    if(game::GetTurn()>(iLastReadTurn+15)){ DBG2(game::GetTurn(),iLastReadTurn); //every X turns try to use stuff from inv
      iLastReadTurn=game::GetTurn();

      static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(!vitEqW[c]->IsReadable(P))continue;
        
        static holybook* hb;hb = dynamic_cast<holybook*>(vitEqW[c]);
        if(hb){
          if(vitEqW[c]->Read(P)){ DBG1(vitEqW[c]->GetNameSingular().CStr()); //TODO try to aim at NPCs
            DBG2(iLastReadTurn,vitEqW[c]->GetNameSingular().CStr());
            bDidSomething=true;
            break;
          }
        }
        
        static scroll* Scroll; Scroll = dynamic_cast<scroll*>(vitEqW[c]);
        if( //some are simple (just read to work imediately)
           dynamic_cast<scrollofearthquake*>(Scroll) ||
           dynamic_cast<scrolloftaming*>(Scroll) ||
           dynamic_cast<scrollofteleportation*>(Scroll) ||
           false //dummy
        ){
          DBG2(iLastReadTurn,vitEqW[c]->GetNameSingular().CStr());
          Scroll->Read(P);
          bDidSomething=true;
          break;
        }
      }
    }
    
    //////////////////////////////// apply things
    static int iLastApplyTurn=-1;
    if(game::GetTurn()>(iLastApplyTurn+40)){ 
      DBG2(game::GetTurn(),iLastApplyTurn); //every X turns try to use stuff from inv
      iLastApplyTurn=game::GetTurn();

      static itemvector vitEqW;vitEqW.clear();H->GetStack()->FillItemVector(vitEqW);
      static itemvector vitA;vitA.clear();
      if(H->GetLeftWielded())vitEqW.push_back(H->GetLeftWielded());
      if(H->GetRightWielded())vitEqW.push_back(H->GetRightWielded());
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(AutoPlayAIcanApply(vitEqW[c]))
          vitA.push_back(vitEqW[c]);
      }
      
      if(vitA.size()){
        item* itA = vitA[clock()%vitA.size()];
        DBG2(iLastApplyTurn,itA->GetNameSingular().CStr());
        itA->Apply(P);
        bDidSomething=true;
      }
    }
    
  }

  return bDidSomething;
}

void wizautoplay::IncAutoPlayMode() {
//  if(!globalwindowhandler::IsKeyTimeoutEnabled()){
//    if(AutoPlayMode>=2){
//      AutoPlayMode=0; // TIMEOUT was disabled there at window handler! so reset here.
//      AutoPlayModeApply();
//    }
//  }

  ++AutoPlayMode;
  if(AutoPlayMode>AUTOPLAYMODE_FRENZY)AutoPlayMode=AUTOPLAYMODE_DISABLED;

  AutoPlayModeApply();
}

void wizautoplay::AutoPlayModeApply(){
  int iTimeout=0;
  bool bPlayInBackground=false;

  const char* msg;
  switch(wizautoplay::AutoPlayMode){
  case AUTOPLAYMODE_DISABLED:
    // disabled
    msg="%s says \"I can rest now.\"";
    break;
  case AUTOPLAYMODE_NOTIMEOUT:
    // no timeout, user needs to hit '.' to it autoplay once, the behavior is controled by AutoPlayMode AND the timeout delay that if 0 will have no timeout but will still autoplay.
    msg="%s says \"I won't rest!\"";
    break;
  case AUTOPLAYMODE_SLOW: // TIMEOUTs key press from here to below
    msg="%s says \"I can't wait anymore!\"";
    iTimeout=(1000);
    bPlayInBackground=true;
    break;
  case AUTOPLAYMODE_FAST:
    msg="%s says \"I am in a hurry!\"";
    iTimeout=(1000/2);
    bPlayInBackground=true;
    break;
  case AUTOPLAYMODE_FRENZY:
    msg="%s says \"I... *frenzy* yeah! Try to follow me now! Hahaha!\"";
    iTimeout=10;//min possible to be fastest //(1000/10); // like 10 FPS, so user has 100ms chance to disable it
    bPlayInBackground=true;
    break;
  }
  ADD_MESSAGE(msg, P->GetName(DEFINITE).CStr());

  globalwindowhandler::SetPlayInBackground(bPlayInBackground);

  if(!ivanconfig::IsXBRZScale()){
    /**
     * TODO
     * This is an horrible gum solution...
     * I still have no idea why this happens.
     * Autoplay will timeout 2 times slower if xBRZ is disabled! why!??!?!?
     * But the debug log shows the correct timeouts :(, clueless for now...
     */
    iTimeout/=2;
  }

  globalwindowhandler::SetKeyTimeout(iTimeout,'.');//,'~');
}

void wizautoplay::AutoPlayCommandKey(character* C,int& Key,truth& HasActed,truth& ValidKeyPressed)
{
  P = C;

  if(wizautoplay::IsPlayerAutoPlay()){
    bool bForceStop = false;
    if(wizautoplay::GetAutoPlayMode()>=AUTOPLAYMODE_SLOW)
      bForceStop = globalwindowhandler::IsKeyPressed(SDL_SCANCODE_ESCAPE);

    if(!bForceStop && Key=='.'){ // pressed or simulated
      if(game::IsInWilderness()){
        Key='>'; //blindly tries to go back to the dungeon safety :) TODO target and move to other dungeons/towns in the wilderness
      }else{
        HasActed = wizautoplay::AutoPlayAICommand(Key); DBG2("Simulated",Key);
        if(HasActed)ValidKeyPressed = true; //valid simulated action
      }
    }else{
      /**
       * if the user hits any key during the autoplay mode that runs by itself, it will be disabled.
       * at non auto mode, can be moved around but cannot rest or will move by itself
       */
      if(wizautoplay::GetAutoPlayMode()>=AUTOPLAYMODE_SLOW && (Key!='~' || bForceStop)){
        wizautoplay::DisableAutoPlayMode();
        AutoPlayAIReset(true); // this will help on re-randomizing things, mainly paths
      }
    }
  }
}

#endif
