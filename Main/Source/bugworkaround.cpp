#include "char.h"
#include "bitmap.h"
#include "bugworkaround.h"
#include "game.h"
#include "graphics.h"
#include "iconf.h"
#include "rawbit.h"
#include "stack.h"
#include "whandler.h"

#include "dbgmsgproj.h"

bool bugWorkaroundDupPlayer::Accepted=false;
void bugWorkaroundDupPlayer::AlertConfirmFixMsg(const char* cMsg, bool bAbortIfNot=true){
  if(bugWorkaroundDupPlayer::Accepted)return;

  v2 v2Border(700,100);
  v2 v2TL(RES.X/2-v2Border.X/2,RES.Y/2-v2Border.Y/2);

  DOUBLE_BUFFER->Fill(v2TL,v2Border,RED);
  graphics::DrawRectangleOutlineAround(DOUBLE_BUFFER, v2TL, v2Border, YELLOW, true);
  DOUBLE_BUFFER->Fill(v2TL,v2Border,RED);

  v2TL+=v2(16,16);
  FONT->Printf(DOUBLE_BUFFER, v2(v2TL.X,v2TL.Y   ), YELLOW, "%s", cMsg);
  FONT->Printf(DOUBLE_BUFFER, v2(v2TL.X,v2TL.Y+16),   BLUE, "%s", "Please backup your savegame files.");
  FONT->Printf(DOUBLE_BUFFER, v2(v2TL.X,v2TL.Y+32),  WHITE, "%s", "Try to fix it? (y/...)");

  graphics::BlitDBToScreen(); //as the final blit may be from StretchedBuffer

  if(GET_KEY() == 'y')bugWorkaroundDupPlayer::Accepted=true;

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
      lsquare* lsqr = game::GetCurrentLevel()->GetLSquare({iX,iY});

      std::vector<item*> vSqrItems;

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

      character* SqrChar = lsqr->GetCharacter();
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
          #define DBGSQRITEM(msg) DBG7(msg,DBGAV2(lsqr->GetPos()),vSqrItems[i],itWork,itWork->GetID(),(SqrChar==NULL?0:SqrChar),(SqrChar==NULL?0:SqrChar->GetID()))
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

character* bugWorkaroundDupPlayer::BugWorkaroundDupPlayer(character* CharAsked, v2 v2AskedPos){ DBG2(CharAsked,DBGAV2(v2AskedPos));
  bool bAllowPlayerBugFix = ivanconfig::GetBugWorkaroundDupPlayer()!=0;
  bool bAllowJustMissingPlayerBugFix = ivanconfig::GetBugWorkaroundDupPlayer()==1;
  bool bNewPlayerInstanceShallWin = ivanconfig::GetBugWorkaroundDupPlayer()==3; //==2 is old player instance

  if(CharAsked!=NULL && !CharAsked->IsPlayer()){
    DBGCHAR(CharAsked,"CharFix:CharAsked:IsNotThePlayer");
    CharAsked=NULL; //deny invalid character
  }

  if(!bAllowPlayerBugFix)
    if(CharAsked!=NULL) //would crash elsewhere if player is NULL
      return CharAsked;

  if(CharAsked==NULL){
    if(!bAllowJustMissingPlayerBugFix){
      ABORT("Player not found at asked pos=(%d,%d), you can try the 'missing player workaround option'",v2AskedPos.X,v2AskedPos.Y);
    }
  }

  bugWorkaroundDupPlayer::Accepted=false; //init to ask again if needed, so the user knows what is happening

  character* CharPlayer = game::SearchCharacter(1);
  if(CharPlayer==NULL){
    std::vector<character*> vCharsOnLevel;
    bugWorkaroundDupPlayer::CollectAllCharactersOnLevel(&vCharsOnLevel);
    for(int i=0;i<vCharsOnLevel.size();i++){
      DBGCHAR(vCharsOnLevel[i],DBGAV2(vCharsOnLevel[i]->GetPos()));

      if(vCharsOnLevel[i]->GetID()==1){
        DBGCHAR(vCharsOnLevel[i],"CharWithID=1");
        DBG2("CharFix:CharID1FoundAt",DBGAV2(vCharsOnLevel[i]->GetPos()));
      }

      if(vCharsOnLevel[i]->IsPlayer()){
        CharPlayer=vCharsOnLevel[i];
        DBG2("CharFix:PlayerFoundAt",DBGAV2(CharPlayer->GetPos()));
        break;
      }
    }

    if(CharPlayer==NULL)ABORT("Player cant be found anywhere"); //OMG... may be this is pointless, better just fix to prevent such bugs from ever happening...
  }
  DBGCHAR(CharPlayer,"CharFix:CharPlayer");

  if(bAllowJustMissingPlayerBugFix)return CharPlayer;

  character* CharWins = CharAsked;
  character* CharPlayerOld = NULL;
  bool bLevelItemsCollected=false;
  if(CharPlayer!=CharAsked){
    bugWorkaroundDupPlayer::AlertConfirmFixMsg(
      bNewPlayerInstanceShallWin ?
      "Duplicated player found. Fix new player instance (experimental)?"    :
      "Duplicated player found. Restore old player instance (experimental)?" );

    CharPlayerOld=CharPlayer;DBGLN;
    DBGCHAR(CharPlayerOld,"CharFix:CharPlayerOld");

    character* CharToBeLost = bNewPlayerInstanceShallWin ? CharPlayerOld : CharAsked;

    if(bNewPlayerInstanceShallWin){
      // the old char is valid and consistent at char's list (so remove it as will receive new ID)
      game::RemoveCharacterID(CharPlayerOld->GetID()); //actually id 1
      // the new char is not valid and not consistent at char's list yet, so...
      game::AddCharacterID(CharAsked,CharAsked->GetID()); //this will "update" the player ID (actually id 1) to the new character making it consistent on the list
    }

//    // prepare to check for dup item's ids
//    bugWorkaroundDupPlayer::GatherAllItemInLevel();DBGLN;
//    bLevelItemsCollected=true;

    //TODO transfer old player items (pointers/objects/instances) to new player instance if they have the same ID?

    if(bNewPlayerInstanceShallWin){
      // old player's items are consistent (on the list), they will get a new ID and be sent to hell
      // new player's items have the same ID of old player's ones, their pointers will become consistent later...
      bugWorkaroundDupPlayer::CharEquipmentsWork(CharPlayerOld,true,true);DBGLN;
      bugWorkaroundDupPlayer::CharInventoryWork(CharPlayerOld,true,true);DBGLN;
    }
    CharToBeLost->_BugWorkaround_PlayerDup(game::CreateNewCharacterID(CharToBeLost));DBGLN; // make it consistent as removing it is crashing (also empties inv)
    for(int i=0;i<CharToBeLost->GetEquipments();i++){ // clear equipments too
      if(CharToBeLost->CanUseEquipment(i)){
        item* it = CharToBeLost->GetEquipment(i);
        if(it!=NULL){
          CharToBeLost->SetEquipment(i,NULL); //this leaves untracked objects in memory. TODO really untracked?
//          if(SearchItem(it->GetID())!=NULL){
//            RemoveItemID(it->GetID()); //TODO could such item pointer or ID be still referenced somewhere?
//          }
          DBG5(CharToBeLost,"CharFix:EquipmentRemoved",i,DBGI(it->GetID()),it);
        }
      }
    }
    CharToBeLost->RemoveFlags(C_PLAYER);DBGLN;
    CharToBeLost->SetTeam(game::GetTeam(MONSTER_TEAM));DBGLN;
    CharToBeLost->SetAssignedName("_DupPlayerBug_");DBGLN; //non immersive naming, shall not be, this bug shall be properly fixed one day.

//    // prepare to check for dup item's ids
//    bugWorkaroundDupPlayer::GatherAllItemInLevel();DBGLN;
//    bLevelItemsCollected=true;

    bugWorkaroundDupPlayer::CharBodypartsWork(CharToBeLost,true,false);DBGLN;
    // this leads to crash //CharAsked->Remove();

    DBGCHAR(CharToBeLost,"CharFix:CharToBeLost");

    CharWins = bNewPlayerInstanceShallWin ? CharAsked : CharPlayerOld;
  }

  // now, grants the valid player has no item issues compared to other items on the level/chars TODO this may create duplicated items?
//  if(!bLevelItemsCollected){bugWorkaroundDupPlayer::GatherAllItemInLevel();DBGLN;}
  // new player's items will be made consistent if required (added to the list)
  bugWorkaroundDupPlayer::CharEquipmentsWork(CharWins,true,false);DBGLN;
  bugWorkaroundDupPlayer::CharInventoryWork(CharWins,true,false);DBGLN;

  // validate full level against other possible dup items
  std::vector<item*> vAllItemsOnLevel;
  bugWorkaroundDupPlayer::CollectAllItemsOnLevel(&vAllItemsOnLevel);
  for(int i=0;i<vAllItemsOnLevel.size();i++){
    if(bugWorkaroundDupPlayer::FindDupItemOnLevel(vAllItemsOnLevel[i],true,true)){
      ABORT("Full level validation against dups failed.");
    }
  }

  DBGCHAR(CharWins,"CharFix:CharWins");
  return CharWins;
}
