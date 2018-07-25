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
void bugWorkaroundDupPlayer::DrawAlertConfirmFix(bitmap* Buffer)
{
  if(!bAlertConfirmFixMsgDraw)return;

  v2 v2Border(700,100);
  v2 v2TL(RES.X/2-v2Border.X/2,RES.Y/2-v2Border.Y/2);

  Buffer->Fill(v2TL,v2Border,RED);
  graphics::DrawRectangleOutlineAround(Buffer, v2TL, v2Border, YELLOW, true);
  Buffer->Fill(v2TL,v2Border,RED);

  v2TL+=v2(16,16);

  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y   ), YELLOW, "%s", fsLastAlertConfirmFixMsg.CStr());

  cchar* c1="Please backup your savegame files.";
  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y+16),   BLUE, "%s", c1);

  cchar* c2="Try to fix it (experimental)? (y/...)";
  FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y+32),  WHITE, "%s", c2);
}

bool bugWorkaroundDupPlayer::Accepted=false;
void bugWorkaroundDupPlayer::AlertConfirmFixMsg(const char* cMsg, bool bAbortIfNot){
  if(bugWorkaroundDupPlayer::Accepted)return;

  fsLastAlertConfirmFixMsg.Empty();
  fsLastAlertConfirmFixMsg<<cMsg;

  static bool bDummyInit  = [](){
    graphics::AddDrawAboveAll(&DrawAlertConfirmFix,100000,"DrawAlertConfirmFix"); return true;}();

  if(game::IsRunning()){ //yes 5 times!
    cchar* c = "BugFix missing/DUP player (experimental/slow), re-enable again after dups died and before saving, to fix their buggy corpses too!";
    ADD_MESSAGE("%s",c);
    ADD_MESSAGE("%s",c);
    ADD_MESSAGE("%s",c);
    ADD_MESSAGE("%s",c);
    ADD_MESSAGE("%s",c);
  }

  bAlertConfirmFixMsgDraw=true;
  if(GET_KEY() == 'y')bugWorkaroundDupPlayer::Accepted=true;
  bAlertConfirmFixMsgDraw=false;

  if(bAbortIfNot && !bugWorkaroundDupPlayer::Accepted)ABORT("%s. Rejected. Can't continue or it will crash...",cMsg);
}

void bugWorkaroundDupPlayer::CharEquipmentsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  for(int i=0;i<CharAsked->GetEquipments();i++)
    bugWorkaroundDupPlayer::ItemWork(CharAsked,CharAsked->GetEquipment(i),bFix,"CharFix:Equipped",pvItem,bSendToHell);
}
void bugWorkaroundDupPlayer::CharInventoryWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  stack* stk=CharAsked->GetStack(); //inventory
  for(int i=0;i<stk->GetItems();i++)
    bugWorkaroundDupPlayer::ItemWork(CharAsked,stk->GetItem(i),bFix,"CharFix:Inventory",pvItem,bSendToHell);
}
void bugWorkaroundDupPlayer::CharBodypartsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  for(int i=0;i<CharAsked->GetBodyParts();i++)
    bugWorkaroundDupPlayer::ItemWork(CharAsked,CharAsked->GetBodyPart(i),bFix,"CharFix:BodyPart",pvItem,bSendToHell);
}

void bugWorkaroundDupPlayer::CharAllItemsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem){
  bugWorkaroundDupPlayer::CharEquipmentsWork(CharAsked, bFix, bSendToHell, pvItem);
  bugWorkaroundDupPlayer::CharInventoryWork (CharAsked, bFix, bSendToHell, pvItem);
  bugWorkaroundDupPlayer::CharBodypartsWork (CharAsked, bFix, bSendToHell, pvItem);
}
void bugWorkaroundDupPlayer::CharAllItemsInfo(character* CharAsked){
  bugWorkaroundDupPlayer::CharAllItemsWork(CharAsked, false, false, NULL);
}
void bugWorkaroundDupPlayer::CharAllItemsCollect(character* CharAsked,std::vector<item*>* pvItem){
  bugWorkaroundDupPlayer::CharAllItemsWork(CharAsked, false, false, pvItem);
}

void bugWorkaroundDupPlayer::CollectAllCharactersOnLevel(std::vector<character*>* pvCharsOnLevel)
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
void bugWorkaroundDupPlayer::CollectAllItemsOnLevel(std::vector<item*>* pvAllItemsOnLevel)
{
  std::vector<bugWorkaroundDupPlayerCharItem> pvAllCharAndOrItemsInLevel;
  ScanLevelForCharactersAndItemsWork(NULL,true,false,&pvAllCharAndOrItemsInLevel);

  for(int i=0;i<pvAllCharAndOrItemsInLevel.size();i++)
    pvAllItemsOnLevel->push_back(pvAllCharAndOrItemsInLevel[i].it);
}

void bugWorkaroundDupPlayer::CollectAllCharactersAndItemsOnLevel(std::vector<bugWorkaroundDupPlayerCharItem>* pvAllCharAndOrItemsInLevel)
{
  ScanLevelForCharactersAndItemsWork(NULL,false,false,pvAllCharAndOrItemsInLevel);
}

bool bugWorkaroundDupPlayer::FindDupItemOnLevel(item* itWork, bool bIgnoreBodyParts, bool bAbortOnMultiples)
{
  return ScanLevelForCharactersAndItemsWork(itWork,bIgnoreBodyParts,bAbortOnMultiples,NULL);
}

bool bugWorkaroundDupPlayer::ScanLevelForCharactersAndItemsWork(
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
        bugWorkaroundDupPlayer::CharAllItemsCollect(SqrChar,&vCharItems);

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

void bugWorkaroundDupPlayer::ItemWork(character* Char, item* itWork, bool bFix, const char* cInfo, std::vector<item*>* pvItem, bool bSendToHell){
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
        bChangeItemID = bugWorkaroundDupPlayer::FindDupItemOnLevel(itWork,false,true);
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
        DBG3("SentToHell:ItemsID",DBGI(itWork->GetID()),itWork);
      }
    }else{
      DBG6(Char,Char->GetID(),cInfo,"CharFix:ItemID",DBGI(itWork->GetID()),itWork); //some helpful info for comparison and understanding
    }
  }
}

void bugWorkaroundDupPlayer::ValidateFullLevel()
{
  // validate full level against other possible dup items
  std::vector<item*> vAllItemsOnLevel;
  bugWorkaroundDupPlayer::CollectAllItemsOnLevel(&vAllItemsOnLevel);
  for(int i=0;i<vAllItemsOnLevel.size();i++){
    if(bugWorkaroundDupPlayer::FindDupItemOnLevel(vAllItemsOnLevel[i],true,true)){
      ABORT("Full level validation against dups failed.");
    }
  }
}

void bugWorkaroundDupPlayer::DupPlayerFix(character* DupPlayer)
{
  //TODO transfer old player items (pointers/objects/instances) to new player instance if they have the same ID?

  /**
   * TODO confirm this info:
   * old player's items are consistent (on the list), they will get a new ID and be sent to hell
   * new player's items have the same ID of old player's ones, their pointers will become consistent later...
   */
  CharEquipmentsWork(DupPlayer,true,true);DBGLN;
  CharInventoryWork(DupPlayer,true,true);DBGLN;

  if(DupPlayer!=NULL){
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
    DupPlayer->LoseConsciousness(100000,false); //may not fall asleep tho
    DupPlayer->SetNP(1); //to die soon at least
    //TODO add other effects to auto-kill it
  }

  CharBodypartsWork(DupPlayer,true,false);DBGLN; //bodyparts sent to hell would crash!!!
  //BEWARE!!! this leads to crash: DupPlayer->Remove();

  DBGCHAR(DupPlayer,"CharFix:CharToBeLost");
}

void bugWorkaroundDupPlayer::init()
{
  devcons::AddDevCmd("FixDupPlayer",bugWorkaroundDupPlayer::DevConsCmd,
    "BugFix DUP player (experimental/slow). If the player went missing, it will kickin automatically on loading.");
}

character* bugWorkaroundDupPlayer::BugWorkaroundDupPlayer(square* sqr)
{
  v2 Pos =  sqr->GetPos();

  character* CharAtPos = sqr->GetCharacter(); DBG3(CharAtPos,PLAYER,DBGAV2(Pos));

  if(CharAtPos==NULL || !CharAtPos->IsPlayer()) //the file is already corrupted, so bug fix will kick in automatically
    CharAtPos = bugWorkaroundDupPlayer::BugWorkaroundDupPlayer(CharAtPos,Pos);

  if(CharAtPos==NULL) //was unable to fix
    ABORT("Player not found! If there are backup files, try the 'restore backup' option.");

  return (CharAtPos);
}

void bugWorkaroundDupPlayer::DevConsCmd(std::string strCmdParams)
{
  BugWorkaroundDupPlayer(PLAYER,PLAYER->GetPos(),true);
}

character* bugWorkaroundDupPlayer::BugWorkaroundDupPlayer(character* CharAskedNewInstance, v2 v2AskedPos,bool bForceDupPlayerCheck){ DBG2(CharAskedNewInstance,DBGAV2(v2AskedPos));
  bugWorkaroundDupPlayer::Accepted=false; //init to ask again if needed, so the user knows what is happening

  DBGCHAR(CharAskedNewInstance,"CharFix:CharAskedNewInstance");

//  bool bAllowPlayerBugFix = ivanconfig::GetBugWorkaroundDupPlayer()!=0;
//  if(!bAllowPlayerBugFix && CharAskedNewInstance==NULL)
//    ABORT("Player not found at asked pos=(%d,%d), you can try the 'missing or dup player' workaround option",v2AskedPos.X,v2AskedPos.Y);

  //this can ONLY return one char with ID=1 EVER, so there wont be a DUP char with ID=1 on the characters' map
  character* CharPlayerConsistent = game::SearchCharacter(1);
  DBGCHAR(CharPlayerConsistent,"CharFix:CharPlayerConsistent");

  if(!bForceDupPlayerCheck)
    if(CharPlayerConsistent==CharAskedNewInstance) //is theoretically ok
      return CharAskedNewInstance;

  std::vector<character*> vCharsOnLevel;
  bugWorkaroundDupPlayer::CollectAllCharactersOnLevel(&vCharsOnLevel);

  festring fsAllC;

  #define ADDNPCTOMSG(C) fsAllC<<C->GetName(DEFINITE)<<"(ID="<<C->GetID()<<");";

  std::vector<character*> vOtherPlayersOnLevel;
  for(int i=0;i<vCharsOnLevel.size();i++){
    DBGCHAR(CharPlayerConsistent,"CharFix:CharOnLevel");
    ADDNPCTOMSG(vCharsOnLevel[i]);

    if(vCharsOnLevel[i]->IsPlayer() || vCharsOnLevel[i]->GetID()==1){
      if(
          vCharsOnLevel[i]!=CharAskedNewInstance &&
          vCharsOnLevel[i]!=CharPlayerConsistent
      ){
        vOtherPlayersOnLevel.push_back(vCharsOnLevel[i]);
        DBG4("CharFix:OtherPlayerFoundAt",DBGAV2(vCharsOnLevel[i]->GetPos()),vCharsOnLevel[i]->IsPlayer(),vCharsOnLevel[i]->GetID());
      }
    }
  }

  if(
      CharAskedNewInstance == NULL ||
      CharAskedNewInstance != CharPlayerConsistent ||
      vOtherPlayersOnLevel.size() > 0
  ){
    bugWorkaroundDupPlayer::AlertConfirmFixMsg("Detected missing or duplicated player bug!",true);
  }

  /**
   * now, grants the valid player has no item issues compared to other items on the level/chars TODO this may create duplicated items?
   * new player's items will be made consistent if required (added to the list)
   */
  character* CharWins = NULL;
  if(CharWins==NULL && CharAskedNewInstance!=NULL){
    CharWins=CharAskedNewInstance;
    if(CharAskedNewInstance!=CharPlayerConsistent){
      // the old char is valid and consistent at char's list (so remove it as will receive new ID)
      game::RemoveCharacterID(CharPlayerConsistent->GetID()); //actually id 1
      /**
       * the new char is not valid and not consistent at char's list yet, so...
       * this will "update" the player ID (actually id 1) to the new character making it consistent on the list with it's pointer
       */
      game::AddCharacterID(CharAskedNewInstance,CharAskedNewInstance->GetID());

      vOtherPlayersOnLevel.push_back(CharPlayerConsistent);
    }
  }
  if(CharWins==NULL && CharPlayerConsistent!=NULL)
    CharWins=CharPlayerConsistent;
  if(CharWins==NULL && vOtherPlayersOnLevel.size()>0){
    /**
     * TODO
     * let the level load and using the look mode let user select what valid player
     * character shall be possessed permanently,
     * and remove it from the list of the ones that will be sent to hell.
     */
    CharWins=vOtherPlayersOnLevel[0];
    vOtherPlayersOnLevel.erase(vOtherPlayersOnLevel.begin());
  }

  if(CharWins==NULL){
    std::vector<character*> vc = game::GetAllCharacters();
    for(int i=0;i<vc.size();i++){
      //TODO how to make this work????? --->  if( std::find(std::begin(vOtherPlayersOnLevel), std::end(vOtherPlayersOnLevel), vc[i]) == std::end(vOtherPlayersOnLevel) )
      bool bFound=false;
      for(int j=0;j<vOtherPlayersOnLevel.size();j++){
        if(vc[i]==vOtherPlayersOnLevel[j]){
          bFound=true;
          break;
        }
      }
      if(!bFound){DBGLN; //reaching here is improbable tho
        ADDNPCTOMSG(vc[i]);
      }
    }
    ABORT("Unfortunately the player character can't be found anywhere. NPCs found: %s",fsAllC.IsEmpty()?"none":fsAllC.CStr()); //OMG... may be this is pointless, better just fix to prevent such bugs from ever happening...
  }

  for(int j=0;j<vOtherPlayersOnLevel.size();j++)
    DupPlayerFix(vOtherPlayersOnLevel[j]);

  // last thing is grant player's stuff is consistent
  CharEquipmentsWork(CharWins,true,false);DBGLN;
  CharInventoryWork (CharWins,true,false);DBGLN;

  // just a final validation, may abort on failure
  ValidateFullLevel();

  return CharWins;
}

///**
// * this became an unnecessarily complex mess TODO remove
// */
//character* bugWorkaroundDupPlayer::BugWorkaroundDupPlayer(character* CharAskedNewInstance, v2 v2AskedPos){ DBG2(CharAskedNewInstance,DBGAV2(v2AskedPos));
//  bool bAllowPlayerBugFix = ivanconfig::GetBugWorkaroundDupPlayer()!=0;
//  bool bAllowJustMissingPlayerBugFix = ivanconfig::GetBugWorkaroundDupPlayer()==1;
//  if(CharAskedNewInstance!=NULL)
//    DBGCHAR(CharAskedNewInstance,"CharFix:CharAskedNewInstance");
//  if(CharAskedNewInstance!=NULL && !CharAskedNewInstance->IsPlayer()){ //TODO allow turning such NPC into player?
//    DBGCHAR(CharAskedNewInstance,"CharFix:CharAskedNewInstance:IsNotThePlayer");
//    CharAskedNewInstance=NULL; //deny invalid character
//  }
//  bool bNewPlayerInstanceShallWin = CharAskedNewInstance!=NULL && ivanconfig::GetBugWorkaroundDupPlayer()==3; //==2 is old player instance
//
//  if(!bAllowPlayerBugFix){
//    if(CharAskedNewInstance==NULL)
//      if(!bAllowJustMissingPlayerBugFix)
//        ABORT("Player not found at asked pos=(%d,%d), you can try the 'missing player workaround option'",v2AskedPos.X,v2AskedPos.Y);
//
//    if(CharAskedNewInstance!=NULL) //would crash elsewhere if player is NULL
//      return CharAskedNewInstance;
//  }
//
//  bugWorkaroundDupPlayer::Accepted=false; //init to ask again if needed, so the user knows what is happening
//
//  character* CharPlayerConsistent = game::SearchCharacter(1); //this can ONLY return one char with ID=1 EVER, so there wont be a DUP char with ID=1 on the map (I expect...)
//  if(CharPlayerConsistent!=NULL)
//    DBGCHAR(CharPlayerConsistent,"CharFix:CharPlayerFound");
//
//  std::vector<character*> vCharsOnLevel;
//  std::vector<character*> vOtherDupPlayersOnLevel;
//  bugWorkaroundDupPlayer::CollectAllCharactersOnLevel(&vCharsOnLevel);
//  for(int i=0;i<vCharsOnLevel.size();i++){
//    DBGCHAR(vCharsOnLevel[i],DBGAV2(vCharsOnLevel[i]->GetPos()));
//
//    if(vCharsOnLevel[i]->GetID()==1){
//      DBGCHAR(vCharsOnLevel[i],"CharWithID=1");
//      DBG2("CharFix:CharID1FoundAt",DBGAV2(vCharsOnLevel[i]->GetPos()));
//    }
//
//    if(vCharsOnLevel[i]->IsPlayer()){
//      if(
//          vCharsOnLevel[i]!=CharAskedNewInstance &&
//          vCharsOnLevel[i]!=CharPlayerConsistent
//      ){
//        vOtherDupPlayersOnLevel.push_back(vCharsOnLevel[i]);
//        DBG2("CharFix:PlayerFoundAt",DBGAV2(vCharsOnLevel[i]->GetPos()));
//      }
//    }
//  }
//  if(CharPlayerConsistent==NULL && CharAskedNewInstance==NULL && vOtherDupPlayersOnLevel.size()==0){
//    std::vector<character*> vc = game::GetAllCharacters();
//    festring fsAllC="none";
//    for(int i=0;i<vc.size();i++){
//      if(i==0)fsAllC="";
//      fsAllC<<vc[i]->GetName(DEFINITE)<<"(ID="<<vc[i]->GetID()<<");";
//    }
//    ABORT("Unfortunately the player character can't be found anywhere. NPCs found: %s",fsAllC.CStr()); //OMG... may be this is pointless, better just fix to prevent such bugs from ever happening...
//  }
//
//  if(bAllowJustMissingPlayerBugFix){
//    if(CharAskedNewInstance!=NULL)
//      return CharAskedNewInstance;
//    if(CharPlayerConsistent!=NULL)
//      return CharPlayerConsistent;
//    if(vOtherDupPlayersOnLevel.size()>0)
//      return vOtherDupPlayersOnLevel[0]; //utter luck TODO could use look mode to let user select what valid player char to possess and send other to hell
//  }
//
//  character* CharWins = NULL;
//  // by preference order
//  if(CharWins==NULL && CharAskedNewInstance!=NULL) // asked 1st
//    CharWins = CharAskedNewInstance;
//  if(CharWins==NULL && CharPlayerConsistent!=NULL) // consistent is better
//    CharWins = CharPlayerConsistent;
//  if(CharWins==NULL && vOtherDupPlayersOnLevel.size()>0) // anything remaining is better than nothing
//    CharWins = vOtherDupPlayersOnLevel[0]; //utter luck TODO could use look mode to let user select what valid player char to possess and send other to hell
//
//  character* CharPlayerOld = NULL;
//  bool bLevelItemsCollected=false;
//  if(CharPlayerFound!=CharAskedNewInstance){
//    bugWorkaroundDupPlayer::AlertConfirmFixMsg(
//      bNewPlayerInstanceShallWin ?
//      "Duplicated player found. Fix new player instance (experimental)?"    :
//      "Duplicated player found. Restore old player instance (experimental)?" );
//
//    CharPlayerOld=CharPlayerFound;DBGLN;
//    DBGCHAR(CharPlayerOld,"CharFix:CharPlayerOld");
//
//    character* CharToBeLost = bNewPlayerInstanceShallWin ? CharPlayerOld : CharAskedNewInstance;
//
//    if(bNewPlayerInstanceShallWin){
//      // the old char is valid and consistent at char's list (so remove it as will receive new ID)
//      game::RemoveCharacterID(CharPlayerOld->GetID()); //actually id 1
//      // the new char is not valid and not consistent at char's list yet, so...
//      game::AddCharacterID(CharAskedNewInstance,CharAskedNewInstance->GetID()); //this will "update" the player ID (actually id 1) to the new character making it consistent on the list
//    }
//
////    // prepare to check for dup item's ids
////    bugWorkaroundDupPlayer::GatherAllItemInLevel();DBGLN;
////    bLevelItemsCollected=true;
//
//    //TODO transfer old player items (pointers/objects/instances) to new player instance if they have the same ID?
//
//    if(bNewPlayerInstanceShallWin){
//      // old player's items are consistent (on the list), they will get a new ID and be sent to hell
//      // new player's items have the same ID of old player's ones, their pointers will become consistent later...
//      bugWorkaroundDupPlayer::CharEquipmentsWork(CharPlayerOld,true,true);DBGLN;
//      bugWorkaroundDupPlayer::CharInventoryWork(CharPlayerOld,true,true);DBGLN;
//    }
//
//    if(CharToBeLost!=NULL){
//      CharToBeLost->_BugWorkaround_PlayerDup(game::CreateNewCharacterID(CharToBeLost));DBGLN; // make it consistent as removing it is crashing (also empties inv)
//      for(int i=0;i<CharToBeLost->GetEquipments();i++){ // clear equipments too
//        if(CharToBeLost->CanUseEquipment(i)){
//          item* it = CharToBeLost->GetEquipment(i);
//          if(it!=NULL){
//            CharToBeLost->SetEquipment(i,NULL); //this leaves untracked objects in memory. TODO really untracked?
//  //          if(SearchItem(it->GetID())!=NULL){
//  //            RemoveItemID(it->GetID()); //TODO could such item pointer or ID be still referenced somewhere?
//  //          }
//            DBG5(CharToBeLost,"CharFix:EquipmentRemoved",i,DBGI(it->GetID()),it);
//          }
//        }
//      }
//      CharToBeLost->RemoveFlags(C_PLAYER);DBGLN;
//      CharToBeLost->SetTeam(game::GetTeam(MONSTER_TEAM));DBGLN;
//      CharToBeLost->SetAssignedName("_DupPlayerBug_");DBGLN; //non immersive naming, shall not be, this bug shall be properly fixed one day.
//    }
//
////    // prepare to check for dup item's ids
////    bugWorkaroundDupPlayer::GatherAllItemInLevel();DBGLN;
////    bLevelItemsCollected=true;
//
//    bugWorkaroundDupPlayer::CharBodypartsWork(CharToBeLost,true,false);DBGLN;
//    // this leads to crash //CharAskedNewInstance->Remove();
//
//    DBGCHAR(CharToBeLost,"CharFix:CharToBeLost");
//
//    CharWins = bNewPlayerInstanceShallWin ? CharAskedNewInstance : CharPlayerOld;
//  }
//
//  /**
//   * now, grants the valid player has no item issues compared to other items on the level/chars TODO this may create duplicated items?
//   * new player's items will be made consistent if required (added to the list)
//   */
//  bugWorkaroundDupPlayer::CharEquipmentsWork(CharWins,true,false);DBGLN;
//  bugWorkaroundDupPlayer::CharInventoryWork(CharWins,true,false);DBGLN;
//
//  // validate full level against other possible dup items
//  std::vector<item*> vAllItemsOnLevel;
//  bugWorkaroundDupPlayer::CollectAllItemsOnLevel(&vAllItemsOnLevel);
//  for(int i=0;i<vAllItemsOnLevel.size();i++){
//    if(bugWorkaroundDupPlayer::FindDupItemOnLevel(vAllItemsOnLevel[i],true,true)){
//      ABORT("Full level validation against dups failed.");
//    }
//  }
//
//  DBGCHAR(CharWins,"CharFix:CharWins");
//  return CharWins;
//}
