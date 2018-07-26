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

#include "char.h"
#include "bitmap.h"
#include "bugworkaround.h"
#include "devcons.h"
#include "game.h"
#include "graphics.h"
#include "iconf.h"
#include "message.h"
#include "rawbit.h"
#include "stack.h"
#include "whandler.h"

#include "dbgmsgproj.h"

bool bAlertConfirmFixMsgDraw=false;
festring fsLastAlertConfirmFixMsg;
void bugfixdp::DrawAlertConfirmFix(bitmap* Buffer)
{
  if(!bAlertConfirmFixMsgDraw)return;

  v2 v2Border(700,100);
  v2 v2TL(RES.X/2-v2Border.X/2,RES.Y/2-v2Border.Y/2);

  Buffer->Fill(v2TL,v2Border,DARK_GRAY);
  graphics::DrawRectangleOutlineAround(Buffer, v2TL, v2Border, YELLOW, true);

  v2TL+=v2(16,16);

  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y   ), YELLOW, "%s", fsLastAlertConfirmFixMsg.CStr());

  cchar* c1="Please backup your savegame files.";
  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y+16),   BLUE, "%s", c1);

  cchar* c2="Confirm? (y/...)";
  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y+32),  WHITE, "%s", c2);
}

bool bugfixdp::AlertConfirmFixMsg(const char* cMsg){
  fsLastAlertConfirmFixMsg.Empty();
  fsLastAlertConfirmFixMsg<<"PROBLEM! "<<cMsg;

  static bool bDummyInit = [](){
    graphics::AddDrawAboveAll(&DrawAlertConfirmFix,100000,"DrawAlertConfirmFix"); return true;}();

  if(game::IsRunning())
    ADD_MESSAGE("%s",cMsg);

  bool Accepted=false;
  bAlertConfirmFixMsgDraw=true;
  if(GET_KEY() == 'y')
    Accepted=true;
  bAlertConfirmFixMsgDraw=false;

  return Accepted;
}

void bugfixdp::CharEquipmentsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  for(int i=0;i<CharAsked->GetEquipments();i++)
    bugfixdp::ItemWork(CharAsked,CharAsked->GetEquipment(i),bFix,"CharFix:Equipped",pvItem,bSendToHell);
}
void bugfixdp::CharInventoryWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  stack* stk=CharAsked->GetStack(); //inventory
  for(int i=0;i<stk->GetItems();i++)
    bugfixdp::ItemWork(CharAsked,stk->GetItem(i),bFix,"CharFix:Inventory",pvItem,bSendToHell);
}
void bugfixdp::CharBodypartsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  for(int i=0;i<CharAsked->GetBodyParts();i++)
    bugfixdp::ItemWork(CharAsked,CharAsked->GetBodyPart(i),bFix,"CharFix:BodyPart",pvItem,bSendToHell);
}

void bugfixdp::CharAllItemsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  bugfixdp::CharEquipmentsWork(CharAsked, bFix, bSendToHell, pvItem);
  bugfixdp::CharInventoryWork (CharAsked, bFix, bSendToHell, pvItem);
  bugfixdp::CharBodypartsWork (CharAsked, bFix, bSendToHell, pvItem);
}
void bugfixdp::CharAllItemsInfo(character* CharAsked){
  bugfixdp::CharAllItemsWork(CharAsked, false, false, NULL);
}
void bugfixdp::CharAllItemsCollect(character* CharAsked,std::vector<item*>* pvItem){
  bugfixdp::CharAllItemsWork(CharAsked, false, false, pvItem);
}

void bugfixdp::CollectAllCharactersOnLevel(std::vector<character*>* pvCharsOnLevel)
{
  std::vector<bugWorkaroundDupPlayerCharItem> pvAllCharAndOrItemsInLevel;
  ScanLevelForCharactersAndItemsWork(NULL,true,false,&pvAllCharAndOrItemsInLevel);

  character* last = NULL;
  character* current = NULL;
  for(int i=0;i<pvAllCharAndOrItemsInLevel.size();i++){
    current = pvAllCharAndOrItemsInLevel[i].Char;
    if(current==NULL)continue; //items on floor

    if(current!=last){ //the char's item is added per char, so there is no gaps
      pvCharsOnLevel->push_back(current);
      last=current;
    }
  }
}

/**
 * it may be filled with duplicated items references (pointers) and IDs,
 * that's the idea, to look for each for DUPs (or multiples) later.
 */
void bugfixdp::CollectAllItemsOnLevel(std::vector<item*>* pvAllItemsOnLevel)
{
  std::vector<bugWorkaroundDupPlayerCharItem> pvAllCharAndOrItemsInLevel;
  ScanLevelForCharactersAndItemsWork(NULL,true,false,&pvAllCharAndOrItemsInLevel);

  for(int i=0;i<pvAllCharAndOrItemsInLevel.size();i++)
    pvAllItemsOnLevel->push_back(pvAllCharAndOrItemsInLevel[i].it);
}

void bugfixdp::CollectAllCharactersAndItemsOnLevel(std::vector<bugWorkaroundDupPlayerCharItem>* pvAllCharAndOrItemsInLevel)
{
  ScanLevelForCharactersAndItemsWork(NULL,false,false,pvAllCharAndOrItemsInLevel);
}

bool bugfixdp::FindDupItemOnLevel(item* itWork, bool bIgnoreBodyParts, bool bAbortOnMultiples)
{
  return ScanLevelForCharactersAndItemsWork(itWork,bIgnoreBodyParts,bAbortOnMultiples,NULL);
}

bool bugfixdp::ScanLevelForCharactersAndItemsWork(
    item* itWork,
    bool bIgnoreBodyParts,
    bool bAbortOnMultiples, // more than dups
    std::vector<bugWorkaroundDupPlayerCharItem>* pvAllCharAndOrItemsInLevel
){
  // scan each map/level's square for dups
  int iDupIDCount=0;
  int iPointerMatchCount=0;
  for(int iY=0;iY<game::GetCurrentArea()->GetYSize();iY++){//if(bChangeItemID)break;
    for(int iX=0;iX<game::GetCurrentArea()->GetXSize();iX++){//if(bChangeItemID)break;
      square* sqr = game::GetCurrentArea()->GetSquare({iX,iY});

      std::vector<item*> vSqrItems;

      level* lvl = game::GetCurrentLevel();
      if(lvl!=NULL){
        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare({iX,iY});

        stack* stk = lsqr->GetStack();
        for(int i=0;i<stk->GetItems();i++){//if(bChangeItemID)break;
          vSqrItems.push_back(stk->GetItem(i));

          if(pvAllCharAndOrItemsInLevel!=NULL){
            bugWorkaroundDupPlayerCharItem ci;
            ci.Char=NULL;
            ci.it=stk->GetItem(i);
            pvAllCharAndOrItemsInLevel->push_back(ci);
          }
        }
      }

      character* SqrChar = sqr->GetCharacter();
      //if(SqrChar!=NULL && SqrChar!=Char){
      if(SqrChar!=NULL){ //w/o skipping the Char, this will check for dups inside self Char inventory!
        std::vector<item*> vCharItems;
        bugfixdp::CharAllItemsCollect(SqrChar,&vCharItems);

        for(int i=0;i<vCharItems.size();i++)vSqrItems.push_back(vCharItems[i]);

        if(pvAllCharAndOrItemsInLevel!=NULL){
          for(int i=0;i<vCharItems.size();i++){
            bugWorkaroundDupPlayerCharItem ci;
            ci.Char=SqrChar;
            ci.it=vCharItems[i];
            pvAllCharAndOrItemsInLevel->push_back(ci);
          }
        }
      }

      if(bAbortOnMultiples){
        #ifdef DBGMSG
          #define DBGSQRITEM(msg) DBG7(msg,DBGAV2(sqr->GetPos()),vSqrItems[i],itWork,itWork->GetID(),(SqrChar==NULL?0:SqrChar),(SqrChar==NULL?0:SqrChar->GetID()))
        #else
          #define DBGSQRITEM(msg)
        #endif
        for(int i=0;i<vSqrItems.size();i++){
          if(bIgnoreBodyParts){
            bodypart* bp = dynamic_cast<bodypart*> (vSqrItems[i]);
            if(bp!=NULL)continue;
          }

          if(itWork==vSqrItems[i]){
            iPointerMatchCount++;
            if(iPointerMatchCount>1)DBGSQRITEM("CharFix:LSqr:DupRef:ItemID"); //the 1st is "expectedly" the real (valid) self...
            continue;
          }

          if(itWork->GetID()==vSqrItems[i]->GetID()){
            iDupIDCount++;
            if(iDupIDCount>1)DBGSQRITEM("CharFix:LSqr:DupID:ItemID"); //the 1st is "expectedly" the real (valid) self...
          }
        }
      }

    }//X
  }//Y

  if(bAbortOnMultiples)
    if(iDupIDCount>1 || iPointerMatchCount>1)
      ABORT( //more than 2 copies (one valid and one invalid) is more complicated and the code is not ready yet to deal with that.
        "iDupCount=%d found for item 0x%x id=%d."
        "tot=%d item pointer/object/reference 0x%x found for id=%d.",
        iDupIDCount,itWork,itWork->GetID(),
        iPointerMatchCount,itWork,itWork->GetID()
      );

  return iDupIDCount>0;
}

void bugfixdp::ItemWork(character* Char, item* itWork, bool bFix, const char* cInfo, std::vector<item*>* pvItem, bool bSendToHell){
  if(itWork!=NULL){
    if(pvItem!=NULL)pvItem->push_back(itWork);

    if(bFix){
      bool bChangeItemID = false;
      bool bMakeItConsistent = false;

      item* itExistingValid = game::SearchItem(itWork->GetID());
      if(itExistingValid==NULL){
        DBG6(Char,Char->GetID(),cInfo,"CharFix:NothingFoundFor:ItemID",DBGI(itWork->GetID()),itWork);
        bMakeItConsistent=true;
      }else{
        if(itWork!=itExistingValid){ // if the object found on the list with the same ID is the same working object pointer, it is already consistent
          bChangeItemID=true;
        }
      }

      if(!bChangeItemID){
        bChangeItemID = bugfixdp::FindDupItemOnLevel(itWork,false,true);
      }

      if(bChangeItemID){
        int iOldID=itWork->GetID();
        itWork->_BugWorkaround_ItemDup(game::CreateNewItemID(itWork));
        DBG9(Char,Char->GetID(),cInfo,"CharFix:Changing:ItemID",itWork,"OldID",iOldID,"NewID",DBGI(itWork->GetID()));
        bMakeItConsistent=true;
      }

      if(bMakeItConsistent){ // !!!!!!!!!!!!!!!!!! AFTER CHANGING THE ID (if it happened) !!!!!!!!!!!!!!!!!!!!!!
        DBG6(Char,Char->GetID(),cInfo,"CharFix:MakingConsistent:ItemID",DBGI(itWork->GetID()),itWork);
        game::AddItemID(itWork,itWork->GetID()); // make it consistent
      }

      if(bSendToHell){
        itWork->SendToHell();
        DBG3("CharFix:SentToHell:ItemsID",DBGI(itWork->GetID()),itWork);
      }
    }else{
      DBG6(Char,Char->GetID(),cInfo,"CharFix:ItemID",DBGI(itWork->GetID()),itWork); //some helpful info for comparison and understanding
    }
  }
}

void bugfixdp::ValidateFullLevel()
{
  DEVCMDMSG("%s","validate full level against dup stuff");

  // validate full level against other possible dup items
  std::vector<item*> vAllItemsOnLevel;
  bugfixdp::CollectAllItemsOnLevel(&vAllItemsOnLevel);
  for(int i=0;i<vAllItemsOnLevel.size();i++){
    if(bugfixdp::FindDupItemOnLevel(vAllItemsOnLevel[i],true,true)){
      ABORT("Full level validation against dups failed.");
    }
  }
}

void bugfixdp::DupPlayerFix(character* DupPlayer)
{
  //TODO transfer old player items (pointers/objects/instances) to new player instance if they have the same ID?

  /**
   * TODO confirm this info:
   * old player's items are consistent (on the list), they will get a new ID and be sent to hell
   * new player's items have the same ID of old player's ones, their pointers will become consistent later...
   */
  CharEquipmentsWork(DupPlayer,true,true);DBGLN;
  CharInventoryWork (DupPlayer,true,true);DBGLN;

  ulong idOld = DupPlayer->GetID();
  DupPlayer->_BugWorkaround_PlayerDup(game::CreateNewCharacterID(DupPlayer));DBGLN; // make it consistent as removing it is crashing (also empties inv)
  for(int i=0;i<DupPlayer->GetEquipments();i++){ // clear equipments too
    if(DupPlayer->CanUseEquipment(i)){
      item* it = DupPlayer->GetEquipment(i);
      if(it!=NULL){
        DupPlayer->SetEquipment(i,NULL); //this leaves untracked objects in memory. TODO really untracked?
//          if(SearchItem(it->GetID())!=NULL){
//            RemoveItemID(it->GetID()); //TODO could such item pointer or ID be still referenced somewhere?
//          }
        DBG5(DupPlayer,"CharFix:EquipmentRemoved",i,DBGI(it->GetID()),it);
      }
    }
  }
  DupPlayer->RemoveFlags(C_PLAYER);DBGLN;
  DupPlayer->SetTeam(game::GetTeam(MONSTER_TEAM));DBGLN;
  DupPlayer->SetAssignedName("_DupPlayerBug_");DBGLN; //non immersive naming, shall not be, this bug shall be properly fixed one day.

  //TODO add other effects to auto-kill it, poison? spill sulf acid? must gas? polymorph into y.bunny?
  DupPlayer->LoseConsciousness(100000,false); //may not fall asleep tho
  DupPlayer->SetNP(1); //to die soon at least

  CharBodypartsWork(DupPlayer,true,false);DBGLN; //bodyparts sent to hell would crash!!! TODO only torso?
  //BEWARE!!! this leads to crash: DupPlayer->Remove();

  DEVCMDMSG("fixed dup player '%s' id=%d/%d 0x%X",DupPlayer->GetName(DEFINITE).CStr(),idOld,DupPlayer->GetID(),DupPlayer);

  DBGCHAR(DupPlayer,"CharFix:CharToBeLost");
}

void bugfixdp::init()
{
  devcons::AddDevCmd("FixDupPlayer",bugfixdp::DevConsCmd,
    "BugFix DUP player (experimental/slow). Ps.: If the player went missing, it will kickin automatically on loading.");
//    "[full] BugFix DUP player (experimental/slow). The dup player may just be the polymorphed, otherwise the full optional mode will try harder to fix it. Ps.: If the player went missing, it will kickin automatically on loading.");
}

character* bugfixdp::ValidatePlayerAt(square* sqr)
{
  v2 Pos = sqr->GetPos();
  character* CharAtPos = sqr->GetCharacter();

  std::vector<character*> vCL = FindPlayersOnLevel();

  if(CharAtPos!=NULL && CharAtPos->IsPlayer() && vCL.size()==1) //very consistent!
    return CharAtPos;

  if(vCL.size()==0){
    character* CharOk = FindValidPlayer(Pos,true);
    if(CharOk==NULL) //TODO let user chose one NPC to become the player? sounds too messy tho...
      ABORT("Player can't be found anywhere! Try to restore a backup if available.");
  }

  if(CharAtPos!=NULL && !CharAtPos->IsPlayer()){
    if(AlertConfirmFixMsg("Requested location has NPC, try to fix this?")){
      if(vCL.size()==1)
        return vCL[0];
    }else{
      return CharAtPos; //TODO what happens in this case as game behavior as assings a NPC as player reference?
    }
  }

  if(vCL.size()>1){
    festring fsMsg;
    fsMsg << "Multiple player instances found (x" << vCL.size() << "), try to fix this?";
    if(AlertConfirmFixMsg(fsMsg.CStr()))
      return BugWorkaroundDupPlayer();
  }

  return CharAtPos;
}

bool IsPlayerPB(character* C1, character* C2){
  if(C1->GetPolymorphBackup()==C2 && C2->IsPlayer())return true;
  if(C2->GetPolymorphBackup()==C1 && C1->IsPlayer())return true;
  return false;
}

std::vector<character*> bugfixdp::FindPlayersOnLevel()
{
  return FindCharactersOnLevel(true);
}

std::vector<character*> bugfixdp::FindCharactersOnLevel(bool bOnlyPlayers)
{
  std::vector<character*> v;
  for(int iY=0;iY<game::GetCurrentArea()->GetYSize();iY++){for(int iX=0;iX<game::GetCurrentArea()->GetXSize();iX++){
    square* sqr = game::GetCurrentArea()->GetSquare({iX,iY});
    character* C=sqr->GetCharacter();
    if(C==NULL)continue;
    if(!bOnlyPlayers || C->IsPlayer())
      v.push_back(C);
  }}
  return v;
}

bool bBufFixDPFullMode=false;
void bugfixdp::DevConsCmd(std::string strCmdParams)
{
  if(strCmdParams=="full")
    bBufFixDPFullMode=true;
  BugWorkaroundDupPlayer();
}

character* bugfixdp::FindValidPlayer(v2 ReqPosL,bool bAndFixIt)
{
  character* CharID1 = game::SearchCharacter(1); //this can ONLY return one char with ID=1 EVER, so there wont be a DUP char with ID=1 on the characters' map
  if(CharID1==NULL)
    return NULL;

  character* PBID1=NULL;
  character* PPolymL = NULL;
  std::vector<character*> vCL = FindCharactersOnLevel();
  for(int j=0;j<vCL.size();j++){
    character* PBtmp = vCL[j]->GetPolymorphBackup();
//    if(PB!=NULL && PB==CharID1){
    if(PBtmp!=NULL && PBtmp->GetID()==1){
      PBID1 = PBtmp;
      DEVCMDMSG("polymorphed has backup with id=1 ref 0x%X",PBID1);
      PPolymL=vCL[j];
      break;
    }
  }

//  if(bAndFixIt && PBID1!=NULL && PBID1!=CharID1){
//    PPolymL->SetPolymorphBackup(CharID1);
//    DEVCMDMSG("fixing PB ref to correct id=1 ref 0x%X",CharID1);
//
//    characteridmap Cmap = game::GetCharacterIDMapCopy();
////    bool b=false;
//    for(characteridmap::iterator itr=Cmap.begin();itr!=Cmap.end();itr++){
//      if(itr->second==PBID1){
////        b=true;
//        DEVCMDMSG("ref 0x%X actually has id=%d ",PBID1,itr->first); //TODO remove? send to hell? needed?
////        if(itr->first!=1)
////          game::RemoveCharacterID(itr->first);
//        break;
//      }
//    }
////    if(b){
////      DEVCMDMSG("inconsistent PBID1 ref 0x%X",PBID1);
////      if(PBID1->GetSquareUnder()!=NULL){
////        //THIS CRASHES!!! pointing to invalid SquareUnder reference ----> PBID1->Remove();
////      }
////      //there is no point in sending to hell as is not on consistent map ---> PBID1->SendToHell();
////    }
//  }

  character* CharPlayerOk = NULL;
  if(bAndFixIt && PPolymL!=NULL){
//    DEVCMDMSG("killing polymorphed id=%d",PPolymL->GetID());
    DEVCMDMSG("vanishing polymorphed id=%d",PPolymL->GetID());
    PPolymL->SetPolymorphBackup(NULL);
    PPolymL->RemoveTraps();
    PPolymL->Remove();
//    PPolymL->Disable();
    PPolymL->SendToHell();
//    PPolymL->Die(NULL,CONST_S(""), FORBID_REINCARNATION|DISALLOW_MSG|DISALLOW_CORPSE);

//    DEVCMDMSG("polymorphed found id=%d",PPolymL->GetID());
//    CharPlayerOk = PPolymL;
//    if(bAndFixIt && CharID1->GetSquareUnder()!=NULL){
////      DEVCMDMSG("keeping the polymorphed id=%d and removing from level the ID1",PPolymL->GetID());
//      DEVCMDMSG("re-applying the polymorph to id=%d",PPolymL->GetID());
//      PPolymL->Remove();
//      CharID1->Polymorph(PPolymL,PPolymL->GetTemporaryStateCounter(POLYMORPHED));
////      // fix based on character::Polymorph()
////      CharID1->RemoveFlags(C_PLAYER);
////      CharID1->Remove(); //the polymorphed will go back to it later
//    }
  }
//  }else{
    CharPlayerOk = CharID1;
    DEVCMDMSG("%s","ID1 will be ok now");
    if(bAndFixIt && CharID1->GetSquareUnder()==NULL){
      DEVCMDMSG("placing the ID1 at %d,%d",ReqPosL.X,ReqPosL.Y);
      CharID1->PutToOrNear(ReqPosL); //place he where expected
    }
//  }

  if(bAndFixIt)
    if(!CharPlayerOk->IsPlayer() || PLAYER!=CharPlayerOk){
      DEVCMDMSG("restoring player reference to id=%d",CharPlayerOk->GetID());
      game::SetPlayer(CharPlayerOk);
    }

  return CharPlayerOk;
}

character* bugfixdp::BugWorkaroundDupPlayer(){
  character* CharID1 = game::SearchCharacter(1); //this can ONLY return one char with ID=1 EVER, so there wont be a DUP char with ID=1 on the characters' map
  if(CharID1==NULL)
    ABORT("Can't find the valid player.");

  std::vector<character*> vCL = FindCharactersOnLevel(); DBG1(vCL.size());
  v2 ReqPosL; //TODO default to the entrance (not the exit). Or random?
  for(int j=0;j<vCL.size();j++){
    if(vCL[j]->IsPlayer() && vCL[j]->GetSquareUnder()!=NULL){
      ReqPosL=vCL[j]->GetPos();
      break;
    }
  }

  character* CharPlayerOk = FindValidPlayer(ReqPosL,true);
  if(CharPlayerOk==NULL)
    ABORT("Unable to fix the valid player.");

//  if(!bBufFixDPFullMode)
//    return CharPlayerOk;



  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////// below is in case the dup player is not consistent on the characters map /////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for(int j=0;j<vCL.size();j++){ DBG4(vCL[j],vCL[j]->GetID(),vCL[j]->IsPlayer(),vCL[j]->GetPolymorphBackup());
    if(vCL[j]==CharPlayerOk)
      continue;
    if(vCL[j]->GetID()==1)
      continue;
    if(vCL[j]->IsPlayer() || vCL[j]->GetPolymorphBackup()==CharID1)
      DupPlayerFix(vCL[j]);
  }

  characteridmap Cmap = game::GetCharacterIDMapCopy();
  for(characteridmap::iterator itr=Cmap.begin();itr!=Cmap.end();itr++){
    ulong Cid = itr->first;if(Cid==0)continue;
    character* C = itr->second;if(C==NULL)continue;
    bool bFound=false;
    for(int j=0;j<vCL.size();j++)
      if(vCL[j]==C || vCL[j]->GetPolymorphBackup()==C){
        bFound=true;
        break;
      }
    if(!bFound){
//      game::RemoveCharacterID(Cid); //causes weird crashes elsewhere
//      DEVCMDMSG("removed inconsistent character id '%d'",Cid);
      DEVCMDMSG("possibly inconsistent character id '%d'",Cid);
    }
  }

  // last thing is grant player's stuff is consistent
  CharEquipmentsWork(CharPlayerOk,true,false);DBGLN;
  DEVCMDMSG("fixed player '%s' equipments",CharPlayerOk->GetName(DEFINITE).CStr());
  CharInventoryWork (CharPlayerOk,true,false);DBGLN;
  DEVCMDMSG("fixed player '%s' inventory",CharPlayerOk->GetName(DEFINITE).CStr());

  // just a final validation, may abort on failure
  ValidateFullLevel();

  return CharPlayerOk;
}
