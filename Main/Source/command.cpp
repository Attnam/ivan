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

#include "actions.h"
#include "bitmap.h"
#include "char.h"
#include "command.h"
#include "database.h"
#include "devcons.h"
#include "felist.h"
#include "game.h"
#include "god.h"
#include "graphics.h"
#include "human.h"
#include "iconf.h"
#include "materia.h"
#include "message.h"
#include "miscitem.h"
#include "room.h"
#include "specialkeys.h"
#include "stack.h"
#include "team.h"
#include "whandler.h"
#include "worldmap.h"
#include "wsquare.h"
#include "wterras.h"

#include "dbgmsgproj.h"

#ifdef WIZARD
#include "proto.h"
#endif

#include "cmdswapweap.cpp"

command::command(truth (*LinkedFunction)(character*), cchar* Description, char Key1, char Key2, char Key3,
                 truth UsableInWilderness, truth WizardModeFunction)
: LinkedFunction(LinkedFunction), Description(Description), Key1(Key1), Key2(Key2), Key3(Key3),
  UsableInWilderness(UsableInWilderness), WizardModeFunction(WizardModeFunction)
{
  game::ValidateCommandKeys(Key1,Key2,Key3);
}

char command::GetKey() const
{
  switch(ivanconfig::GetDirectionKeyMap())
  {
   case DIR_NORM: // Normal
    return Key1;
   case DIR_ALT: // Alternative
    return Key2;
   case DIR_HACK: // Nethack
    return Key3;
   default:
    ABORT("This is not Vim!");
    return Key1;
  }
}

command* commandsystem::Command[] =
{
  0,

  /* Sort according to description */

  new command(&Apply, "apply", 'a', 'a', 'a', false),
  new command(&ApplyAgain, "apply last item again", 'A', 'A', 'A', false),
  new command(&Talk, "chat", 'C', 'C', 'C', false),
  new command(&Close, "close", 'c', 'c', 'c', false),
  new command(&Dip, "dip", '!', '!', '!', false),
  new command(&Drink, "drink", 'D', 'D', 'D', true),
  new command(&Taste, "taste", 'T', 'T', 'T', true),
  new command(&Drop, "drop", 'd', 'd', 'd', true),
  new command(&Eat, "eat", 'e', 'e', 'e', true),
  new command(&WhatToEngrave, "engrave", 'G', 'G', 'G', false),
  new command(&EquipmentScreen, "equipment menu", 'E', 'E', 'E', true),
  new command(&Go, "go", 'g', 'g', 'g', false),
  new command(&GoDown, "go down/enter area", '>', '>', '>', true),
  new command(&GoUp, "go up", '<', '<', '<', true),
  new command(&IssueCommand, "issue command(s) to team member(s)", 'I', 'I', 'I', false),
  new command(&Kick, "kick", 'k', 'K', 'K', false),
  new command(&Look, "look", 'l', 'L', 'L', true),
  new command(&ShowMap, "show map", 'm', 'm', 'm', false),
  new command(&AssignName , "name", 'n', 'n', 'N', false),
  new command(&Offer, "offer", 'O', 'f', 'O', false),
  new command(&Open, "open", 'o', 'O', 'o', false),
  new command(&PickUp, "pick up", ',', ',', ',', false),
  new command(&Pray, "pray", 'p', 'p', 'p', false),
  new command(&Quit, "quit", 'Q', 'Q', 'Q', true),
  new command(&Read, "read", 'r', 'r', 'r', false),
  new command(&Rest, "rest/heal", 'h', 'h', 'H', true),
  new command(&Save, "save game", 'S', 'S', 'S', true),
  new command(&ScrollMessagesDown, "scroll messages down", '+', '+', '+', true),
  new command(&ScrollMessagesUp, "scroll messages up", '-', '-', '-', true),
  new command(&ShowConfigScreen, "show config screen", '\\', '\\', '\\', true),
  new command(&ShowInventory, "show inventory", 'i', 'i', 'i', true),
  new command(&ShowKeyLayout, "show key layout", '?', '?', '?', true),
  new command(&DrawMessageHistory, "show message history", 'M', 'M', 'M', true),
  new command(&ShowWeaponSkills, "show weapon skills", '@', '@', '@', true),
  new command(&Search, "search", 's', 's', 's', false),
  new command(&Sit, "sit", '_', '_', '_', false),
  new command(&SwapWeapons, "swap weapons", 'x', 'x', 'x', false),
  new command(&SwapWeaponsCfg, "swap weapons configuration", 'X', 'X', 'X', false),
  new command(&Throw, "throw", 't', 't', 't', false),
  new command(&ToggleRunning, "toggle running", 'u', 'U', 'U', true),
  new command(&ForceVomit, "vomit", 'V', 'V', 'V', false),
  new command(&NOP, "wait", '.', '.', '.', true),
  new command(&WieldInRightArm, "wield in right arm", 'w', 'w', 'w', true),
  new command(&WieldInLeftArm, "wield in left arm", 'W', 'W', 'W', true),
#ifdef WIZARD
  new command(&WizardMode, "wizard mode activation (Ctrl+ to access console commands)", '`', '`', '`', true),
#else
  new command(&DevConsCmd, "access console commands", '`', '`', '`', true), //works w/o Ctrl in this case
#endif
  new command(&Zap, "zap", 'z', 'z', 'z', false),

#ifdef WIZARD

  /* Sort according to key */

  new command(&AutoPlay, "auto play the game (hold ESC to stop)", '~', '~', '~', true, true), //there is more AI than NPC's one to let it work better
  new command(&RaiseStats, "raise stats", '1', '1', '1', true, true),
  new command(&LowerStats, "lower stats", '2', '2', '2', true, true),
  new command(&SeeWholeMap, "see whole map", '3', '3', '3', true, true),
  new command(&WalkThroughWalls, "toggle walk through walls mode", '4', '4', '4', true, true),
  new command(&RaiseGodRelations, "raise your relations to the gods", '5', '5', '5', true, true),
  new command(&LowerGodRelations, "lower your relations to the gods", '6', '6', '6', true, true),
  new command(&GainDivineKnowledge, "gain knowledge of all gods", '\"', '\"', '\"', true, true),
  new command(&GainAllItems, "gain all items", '$', '$', '$', true, true),
  new command(&SecretKnowledge, "reveal secret knowledge", '*', '*', '*', true, true),
  new command(&DetachBodyPart, "detach a limb", '0', '0', '0', true, true),
  new command(&SetFireToBodyPart, "set fire to a limb", ']', ']', ']', true, true),
  new command(&SummonMonster, "summon monster", '&', '&', '&', false, true),
  new command(&LevelTeleport, "level teleport", '|', '|', '|', false, true),
  new command(&Possess, "possess creature", '{', '{', '{', false, true),
  new command(&Polymorph, "polymorph", '[', '[', '[', true, true),

#endif

  0
};

#ifndef WIZARD
truth commandsystem::DevConsCmd(character* Char)
{
  devcons::OpenCommandsConsole();
  return false;
}
#endif

truth commandsystem::IsForRegionListItem(int iIndex){ //see code generator helper script prepareCmdsDescrCode.sh (use cygwin)
  cchar* str = Command[iIndex]->GetDescription();
  if(strcmp(str,"apply")==0)return true;
//  if(strcmp(str,"chat")==0)return true;
//  if(strcmp(str,"close")==0)return true;
  if(strcmp(str,"dip")==0)return true;
  if(strcmp(str,"drink")==0)return true;
  if(strcmp(str,"drop")==0)return true;
  if(strcmp(str,"eat")==0)return true;
  if(strcmp(str,"engrave")==0)return true;
  if(strcmp(str,"equipment menu")==0)return true;
//  if(strcmp(str,"go")==0)return true;
//  if(strcmp(str,"go down/enter area")==0)return true;
//  if(strcmp(str,"go up")==0)return true;
//  if(strcmp(str,"issue command(s) to team member(s)")==0)return true;
//  if(strcmp(str,"kick")==0)return true;
//  if(strcmp(str,"look")==0)return true;
//  if(strcmp(str,"name")==0)return true;
  if(strcmp(str,"offer")==0)return true;
  if(strcmp(str,"open")==0)return true;
  if(strcmp(str,"pick up")==0)return true;
  if(strcmp(str,"pray")==0)return true;
//  if(strcmp(str,"quit")==0)return true;
  if(strcmp(str,"read")==0)return true;
//  if(strcmp(str,"rest/heal")==0)return true;
//  if(strcmp(str,"save game")==0)return true;
//  if(strcmp(str,"scroll messages down")==0)return true;
//  if(strcmp(str,"scroll messages up")==0)return true;
//  if(strcmp(str,"show config screen")==0)return true;
//  if(strcmp(str,"show inventory")==0)return true;
//  if(strcmp(str,"show key layout")==0)return true;
//  if(strcmp(str,"show message history")==0)return true;
//  if(strcmp(str,"show weapon skills")==0)return true;
//  if(strcmp(str,"search")==0)return true;
//  if(strcmp(str,"sit")==0)return true;
//  if(strcmp(str,"swap weapons")==0)return true;
//  if(strcmp(str,"swap weapons configuration")==0)return true;
  if(strcmp(str,"throw")==0)return true;
//  if(strcmp(str,"toggle running")==0)return true;
//  if(strcmp(str,"vomit")==0)return true;
//  if(strcmp(str,"wait")==0)return true;
  if(strcmp(str,"wield in right arm")==0)return true;
  if(strcmp(str,"wield in left arm")==0)return true;
//  if(strcmp(str,"wizard mode activation")==0)return true;
  if(strcmp(str,"zap")==0)return true;
//  if(strcmp(str,"raise stats")==0)return true;
//  if(strcmp(str,"lower stats")==0)return true;
//  if(strcmp(str,"see whole map")==0)return true;
//  if(strcmp(str,"toggle walk through walls mode")==0)return true;
//  if(strcmp(str,"raise your relations to the gods")==0)return true;
//  if(strcmp(str,"lower your relations to the gods")==0)return true;
//  if(strcmp(str,"gain knowledge of all gods")==0)return true;
//  if(strcmp(str,"gain all items")==0)return true;
//  if(strcmp(str,"reveal secret knowledge")==0)return true;
//  if(strcmp(str,"detach a limb")==0)return true;
//  if(strcmp(str,"set fire to a limb")==0)return true;
//  if(strcmp(str,"summon monster")==0)return true;
//  if(strcmp(str,"level teleport")==0)return true;
//  if(strcmp(str,"possess creature")==0)return true;
  if(strcmp(str,"polymorph")==0)return true;
  return false;
}
truth commandsystem::IsForRegionSilhouette(int iIndex){ //see code generator helper script prepareCmdsDescrCode.sh (use cygwin)
  cchar* str = Command[iIndex]->GetDescription();
  if(strcmp(str,"apply")==0)return true;
//  if(strcmp(str,"chat")==0)return true;
//  if(strcmp(str,"close")==0)return true;
  if(strcmp(str,"dip")==0)return true;
  if(strcmp(str,"drink")==0)return true;
  if(strcmp(str,"drop")==0)return true;
  if(strcmp(str,"eat")==0)return true;
  if(strcmp(str,"engrave")==0)return true;
  if(strcmp(str,"equipment menu")==0)return true;
//  if(strcmp(str,"go")==0)return true;
//  if(strcmp(str,"go down/enter area")==0)return true;
//  if(strcmp(str,"go up")==0)return true;
//  if(strcmp(str,"issue command(s) to team member(s)")==0)return true;
//  if(strcmp(str,"kick")==0)return true;
//  if(strcmp(str,"look")==0)return true;
//  if(strcmp(str,"name")==0)return true;
  if(strcmp(str,"offer")==0)return true;
  if(strcmp(str,"open")==0)return true;
  if(strcmp(str,"pick up")==0)return true;
  if(strcmp(str,"pray")==0)return true;
//  if(strcmp(str,"quit")==0)return true;
  if(strcmp(str,"read")==0)return true;
//  if(strcmp(str,"rest/heal")==0)return true;
//  if(strcmp(str,"save game")==0)return true;
//  if(strcmp(str,"scroll messages down")==0)return true;
//  if(strcmp(str,"scroll messages up")==0)return true;
//  if(strcmp(str,"show config screen")==0)return true;
  if(strcmp(str,"show inventory")==0)return true;
//  if(strcmp(str,"show key layout")==0)return true;
//  if(strcmp(str,"show message history")==0)return true;
//  if(strcmp(str,"show weapon skills")==0)return true;
//  if(strcmp(str,"search")==0)return true;
//  if(strcmp(str,"sit")==0)return true;
//  if(strcmp(str,"swap weapons")==0)return true;
  if(strcmp(str,"swap weapons configuration")==0)return true;
  if(strcmp(str,"throw")==0)return true;
//  if(strcmp(str,"toggle running")==0)return true;
//  if(strcmp(str,"vomit")==0)return true;
//  if(strcmp(str,"wait")==0)return true;
  if(strcmp(str,"wield in right arm")==0)return true;
  if(strcmp(str,"wield in left arm")==0)return true;
//  if(strcmp(str,"wizard mode activation")==0)return true;
  if(strcmp(str,"zap")==0)return true;
//  if(strcmp(str,"raise stats")==0)return true;
//  if(strcmp(str,"lower stats")==0)return true;
//  if(strcmp(str,"see whole map")==0)return true;
//  if(strcmp(str,"toggle walk through walls mode")==0)return true;
//  if(strcmp(str,"raise your relations to the gods")==0)return true;
//  if(strcmp(str,"lower your relations to the gods")==0)return true;
//  if(strcmp(str,"gain knowledge of all gods")==0)return true;
//  if(strcmp(str,"gain all items")==0)return true;
//  if(strcmp(str,"reveal secret knowledge")==0)return true;
//  if(strcmp(str,"detach a limb")==0)return true;
//  if(strcmp(str,"set fire to a limb")==0)return true;
//  if(strcmp(str,"summon monster")==0)return true;
//  if(strcmp(str,"level teleport")==0)return true;
//  if(strcmp(str,"possess creature")==0)return true;
  if(strcmp(str,"polymorph")==0)return true;
  return false;
}

char findCmdKey(truth (*func)(character*))
{
  char cKey=0;
  for(int i = 1; command* cmd = commandsystem::GetCommand(i); ++i)
    if(cmd->GetLinkedFunction()==func){
      cKey = cmd->GetKey();
      break;
    }
  if(cKey==0)ABORT("can't find key for command."); //TODO how to show what command from *func???
  return cKey;
}

truth commandsystem::GoUp(character* Char)
{
  if(!Char->TryToUnStickTraps(ZERO_V2))
    return false;

  /*if(!game::IsInWilderness() && game::WizardModeIsActive() && game::GetCurrentLevelIndex() >= 1)
    if(game::TryTravel(game::GetCurrentDungeonIndex(), game::GetCurrentLevelIndex() - 1, RANDOM, true))
    return true;*/

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(!Terrain)
  {
    if(game::IsInWilderness())
    {
      if(!Char->IsFlying())
        ADD_MESSAGE("You jump into the air. For some reason you don't get too far above.");
      else
        ADD_MESSAGE("You fly around for some time.");
    }
    else
      ADD_MESSAGE("You can't go up.");

    return false;
  }

  if(Terrain->Enter(true))
  {
    Char->EditExperience(LEG_STRENGTH, 150, 1 << 6);
    Char->EditNP(-20);
    Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
    return true;
  }
  else
    return false;
}

truth commandsystem::GoDown(character* Char)
{
  if(!Char->TryToUnStickTraps(ZERO_V2))
    return false;

  /*if(!game::IsInWilderness() && game::WizardModeIsActive() && game::GetCurrentLevelIndex() < game::GetLevels() - 1)
    if(game::TryTravel(game::GetCurrentDungeonIndex(), game::GetCurrentLevelIndex() + 1, RANDOM, true))
    return true;*/

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(!Terrain)
  {
    if(game::IsInWilderness())
      ADD_MESSAGE("There seems to be nothing of interest here.");
    else
      ADD_MESSAGE("You can't go down.");

    return false;
  }

  if(Terrain->Enter(false))
  {
    Char->EditExperience(AGILITY, 150, 1 << 6);
    Char->EditNP(-10);
    Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
    return true;
  }
  else
    return false;
}

truth commandsystem::Open(character* Char)
{
  if(Char->CanOpen())
  {
    int Key;

    if(ivanconfig::GetSmartOpenCloseApply())
    {
      std::vector<lsquare*> SquaresWithOpenableItems;
      std::vector<olterrain*> OpenableOLTerrains;
      std::vector<olterrain*> AlreadyOpenOLTerrains;

      for(int d = 0; d < Char->GetExtendedNeighbourSquares(); ++d)
      {
        lsquare* Square = Char->GetNeighbourLSquare(d);

        if(Square)
        {
          if(Square->GetStack()->SortedItems(Char, &item::IsOpenable))
            SquaresWithOpenableItems.push_back(Square);

          if(Square->GetOLTerrain())
          {
            if(Square->GetOLTerrain()->CanBeOpened())
              OpenableOLTerrains.push_back(Square->GetOLTerrain());
            else if(Square->GetOLTerrain()->IsOpen())
              AlreadyOpenOLTerrains.push_back(Square->GetOLTerrain());
          }
        }
      }

      if(Char->GetStack()->SortedItems(Char, &item::IsOpenable))
      {
        if(SquaresWithOpenableItems.empty() && OpenableOLTerrains.empty())
          Key = 'i';
        else
          Key = game::AskForKeyPress(CONST_S("What do you wish to open? "
                                             "[press a direction key, space or i]"));

        if(Key == 'i')
        {
          item* Item = Char->GetStack()->DrawContents(Char,
                                                      CONST_S("What do you want to open?"),
                                                      0, &item::IsOpenable);
          return Item && Item->Open(Char);
        }
      }
      else if(SquaresWithOpenableItems.size() == 1 && OpenableOLTerrains.empty())
        return SquaresWithOpenableItems[0]->Open(Char);
      else if(SquaresWithOpenableItems.empty() && OpenableOLTerrains.size() == 1)
        return OpenableOLTerrains[0]->Open(Char);
      else if(SquaresWithOpenableItems.empty() && OpenableOLTerrains.empty())
      {
        if(!AlreadyOpenOLTerrains.empty())
          return AlreadyOpenOLTerrains[0]->Open(Char);
        else
        {
          ADD_MESSAGE("Find something to open first, %s.", game::Insult());
          return false;
        }
      }
      else
        Key = game::AskForKeyPress(CONST_S("What do you wish to open? "
                                           "[press a direction key or space]"));
    }
    else
    {
      truth OpenableItems = Char->GetStack()->SortedItems(Char, &item::IsOpenable);

      if(OpenableItems)
        Key = game::AskForKeyPress(CONST_S("What do you wish to open? "
                                           "[press a direction key, space or i]"));
      else
        Key = game::AskForKeyPress(CONST_S("What do you wish to open? "
                                           "[press a direction key or space]"));

      if(Key == 'i' && OpenableItems)
      {
        item* Item = Char->GetStack()->DrawContents(Char,
                                                    CONST_S("What do you want to open?"),
                                                    0, &item::IsOpenable);
        return Item && Item->Open(Char);
      }
    }

    v2 DirVect = game::GetDirectionVectorForKey(Key);

    if(DirVect != ERROR_V2 && Char->GetArea()->IsValidPos(Char->GetPos() + DirVect)){
      return Char->GetNearLSquare(Char->GetPos() + DirVect)->Open(Char);
    }
  }
  else
    ADD_MESSAGE("This monster type cannot open anything.");

  return false;
}

truth commandsystem::Close(character* Char)
{
  if(Char->CanOpen())
  {
    if(ivanconfig::GetSmartOpenCloseApply())
    {
      /* See if there's only a single open door nearby, otherwise ask for a direction */

      int ThingsToClose = 0;
      int ThingsAlreadyClosed = 0;
      lsquare* SquareWithThingToClose;
      lsquare* SquareWithThingAlreadyClosed;

      for(int d = 0; d < Char->GetExtendedNeighbourSquares(); ++d)
      {
        lsquare* Square = Char->GetNeighbourLSquare(d);

        if(!Square || !Square->GetOLTerrain())
          continue;

        if(Square->GetOLTerrain()->IsOpen())
        {
          ++ThingsToClose;

          if(ThingsToClose > 1)
            break;

          SquareWithThingToClose = Square;
        }
        else if(Square->GetOLTerrain()->IsCloseable())
        {
          ++ThingsAlreadyClosed;
          SquareWithThingAlreadyClosed = Square;
        }
      }

      if(ThingsToClose == 0)
      {
        if(ThingsAlreadyClosed == 0)
          ADD_MESSAGE("Find something to close first, %s.", game::Insult());
        else
          SquareWithThingAlreadyClosed->Close(Char);
      }
      else if(ThingsToClose == 1)
        return SquareWithThingToClose->Close(Char);
      else
      {
        int Dir = game::DirectionQuestion(CONST_S("What do you wish to close?  [press a direction key]"), false);

        if(Dir != DIR_ERROR && Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
          return Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir))->Close(Char);
      }
    }
    else
    {
      int Dir = game::DirectionQuestion(CONST_S("What do you wish to close?  [press a direction key]"), false);

      if(Dir != DIR_ERROR && Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
        return Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir))->Close(Char);
    }
  }
  else
    ADD_MESSAGE("This monster type cannot close anything.");

  return false;
}

truth commandsystem::Drop(character* Char)
{
  if(!Char->GetStack()->GetItems())
  {
    ADD_MESSAGE("You have nothing to drop!");
    return false;
  }

  truth Success = false;
  stack::SetSelected(0);

  for(;;)
  {
    itemvector ToDrop;
    game::DrawEverythingNoBlit();
    Char->GetStack()->DrawContents(ToDrop, Char, CONST_S("What do you want to drop?"), REMEMBER_SELECTED);

    if(ToDrop.empty())
      break;

    if(game::IsInWilderness())
    {
      for(uint c = 0; c < ToDrop.size(); ++c)
      {
        if(game::TruthQuestion(CONST_S("Are you sure? You will never see ") + ToDrop[c]->CHAR_NAME(DEFINITE)
                               + " again! [y/N]"))
        {

          ADD_MESSAGE("You drop %s.", ToDrop[c]->CHAR_NAME(DEFINITE));
          ToDrop[c]->RemoveFromSlot();
          ToDrop[c]->SendToHell();
        }
      }
    }
    else if(!Char->GetRoom() || Char->GetRoom()->DropItem(Char, ToDrop[0], ToDrop.size()))
    {
      ADD_MESSAGE("%s dropped.", ToDrop[0]->GetName(INDEFINITE, ToDrop.size()).CStr());
      for(uint c = 0; c < ToDrop.size(); ++c)
      {
        ToDrop[c]->MoveTo(Char->GetStackUnder());
        if(ivanconfig::IsAutoPickupThrownItems())
          ToDrop[c]->ClearTag('t'); //throw: to avoid auto-pickup
      }
      Success = true;
    }
  }

  if(Success)
  {
    Char->DexterityAction(2);
    return true;
  }

  return false;
}

truth commandsystem::Eat(character* Char)
{
  if(!Char->CheckConsume(CONST_S("eat"))){
    return false;
  }

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasEatEffect())
  {
    if(Square->GetOLTerrain()->Eat(Char)){
      return true;
    }
  }

  return Consume(Char, "eat", "eating", &item::IsEatable);
}

truth commandsystem::Drink(character* Char)
{

  if(!Char->CheckConsume(CONST_S("drink"))){
    return false;
  }

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasDrinkEffect())
  {
    if(Square->GetOLTerrain()->Drink(Char)){
      return true;
    }
  }

  return Consume(Char, "drink", "drinking", &item::IsDrinkable);
}

truth commandsystem::Taste(character* Char)
{
  if(!Char->CheckConsume(CONST_S("drink")))
    return false;

  lsquare* Square = Char->GetLSquareUnder();

  if(!game::IsInWilderness() && Square->GetOLTerrain() && Square->GetOLTerrain()->HasDrinkEffect())
  {
    if(Square->GetOLTerrain()->Drink(Char))
      return true;
  }

  return Consume(Char, "sip", "sipping", &item::IsDrinkable, true);
}

truth commandsystem::Consume(character* Char, cchar* ConsumeVerb, cchar* ConsumeVerbPresentParticiple,
                             sorter Sorter, truth nibbling)
{
  lsquare* Square = Char->GetLSquareUnder();
  stack* Inventory = Char->GetStack();
  stack* StackUnder = Square->GetStack();

  if((game::IsInWilderness() || !StackUnder->SortedItems(Char, Sorter)) && !Inventory->SortedItems(Char, Sorter))
  {
    ADD_MESSAGE("You have nothing to %s!", ConsumeVerb);
    return false;
  }

  itemvector Item;
  festring Question = CONST_S("What do you wish to ") + ConsumeVerb + '?';

  if(!game::IsInWilderness() && StackUnder->SortedItems(Char, Sorter))
    Inventory->DrawContents(Item, StackUnder, Char, Question, CONST_S("Items in your inventory"),
                            CONST_S("Items on the ground"), CONST_S(""), 0, NO_MULTI_SELECT, Sorter);
  else
    Inventory->DrawContents(Item, Char, Question, NO_MULTI_SELECT, Sorter);

  return !Item.empty() ? Char->ConsumeItem(Item[0], ConsumeVerbPresentParticiple, nibbling) : false;
}

truth commandsystem::ShowInventory(character* Char)
{
  festring Title("Your inventory (total weight: ");
  Title << Char->GetStack()->GetWeight();
  Title << "g)";
  Char->GetStack()->DrawContents(Char, Title, NO_SELECT);

  return false;
}

truth commandsystem::PickUp(character* Char)
{

  if(!Char->GetStackUnder()->GetVisibleItems(Char))
  {
    ADD_MESSAGE("There is nothing here to pick up!");
    return false;
  }

  itemvectorvector PileVector;
  Char->GetStackUnder()->Pile(PileVector, Char, CENTER);

  for(int c = 0; c < 4; ++c)
  {
    stack* Stack = Char->GetLSquareUnder()->GetStackOfAdjacentSquare(c);

    if(Stack)
      Stack->Pile(PileVector, Char, 3 - c);
  }

  if(PileVector.size() == 1)
  {
    if(PileVector[0][0]->CanBePickedUp())
    {
      int Amount = PileVector[0].size();

      if(Amount > 1)
        Amount = game::ScrollBarQuestion(CONST_S("How many ") + PileVector[0][0]->GetName(PLURAL) + '?',
                                         Amount, 1, 0, Amount, 0, WHITE, LIGHT_GRAY, DARK_GRAY);

      if(!Amount){
        return false;
      }

      if((!PileVector[0][0]->GetRoom()
          || PileVector[0][0]->GetRoom()->PickupItem(Char, PileVector[0][0], Amount))
         && PileVector[0][0]->CheckPickUpEffect(Char))
      {
        for(int c = 0; c < Amount; ++c)
          PileVector[0][c]->MoveTo(Char->GetStack());

        ADD_MESSAGE("%s picked up.", PileVector[0][0]->GetName(INDEFINITE, Amount).CStr());
        Char->DexterityAction(2);
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      ADD_MESSAGE("%s too large to pick up!", PileVector[0].size() == 1 ? "It is" : "They are");
      return false;
    }
  }

  truth Success = false;
  stack::SetSelected(0);

  for(;;)
  {
    itemvector ToPickup;
    game::DrawEverythingNoBlit();
    Char->GetStackUnder()->DrawContents(ToPickup, Char, CONST_S("What do you want to pick up?"), REMEMBER_SELECTED);

    if(ToPickup.empty())
      break;

    if(ToPickup[0]->CanBePickedUp())
    {
      if((!ToPickup[0]->GetRoom()
          || ToPickup[0]->GetRoom()->PickupItem(Char, ToPickup[0], ToPickup.size()))
         && ToPickup[0]->CheckPickUpEffect(Char))
      {
        for(uint c = 0; c < ToPickup.size(); ++c)
          ToPickup[c]->MoveTo(Char->GetStack());

        ADD_MESSAGE("%s picked up.", ToPickup[0]->GetName(INDEFINITE, ToPickup.size()).CStr());
        Success = true;
      }
    }
    else
      ADD_MESSAGE("%s too large to pick up!", ToPickup.size() == 1 ? "It is" : "They are");
  }

  if(Success)
  {
    Char->DexterityAction(2);
    return true;
  }

  return false;
}

truth commandsystem::Quit(character* Char)
{
  if(game::TruthQuestion(CONST_S("Your quest is not yet completed! Really quit? [y/N]")))
  {
    Char->ShowAdventureInfo();
    festring Msg = CONST_S("cowardly quit the game");
    Char->AddScoreEntry(Msg, 0.75);
    game::End(Msg, !game::WizardModeIsActive() || game::TruthQuestion(CONST_S("Remove saves? [y/N]")));
    return true;
  }
  else
    return false;
}

truth commandsystem::Talk(character* Char)
{
  if(!Char->CheckTalk())
    return false;

  character* ToTalk = 0;
  int Characters = 0;

  for(int d = 0; d < 8; ++d)
  {
    lsquare* Square = Char->GetNaturalNeighbourLSquare(d);

    if(Square)
    {
      character* Dude = Square->GetCharacter();

      if(Dude)
      {
        ToTalk = Dude;
        ++Characters;
      }
    }
  }

  if(!Characters)
  {
    ADD_MESSAGE("Find yourself someone to talk to first!");
    return false;
  }
  else if(Characters == 1)
    return ToTalk->ChatMenu();
  else
  {
    int Dir = game::DirectionQuestion(CONST_S("To whom do you wish to talk? [press a direction key]"), false, true);

    if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
      return false;

    character* Dude = Char->GetNearSquare(Char->GetPos() + game::GetMoveVector(Dir))->GetCharacter();

    if(Dude == Char)
    {
      ADD_MESSAGE("You talk to yourself for some time.");
      Char->EditExperience(WISDOM, -50, 1 << 7);
      Char->EditAP(-1000);
      return true;
    }
    else if(Dude)
      return Dude->ChatMenu();
    else
      ADD_MESSAGE("You get no response.");
  }

  return false;
}

truth commandsystem::NOP(character* Char)
{
  Char->EditExperience(DEXTERITY, -25, 1 << 3);
  Char->EditExperience(AGILITY, -25, 1 << 3);
  Char->EditAP(-Char->GetStateAPGain(1000));
  return true;
}

truth commandsystem::Save(character*)
{
  if(game::TruthQuestion(CONST_S("Do you truly wish to save and flee? [y/N]")))
  {
    game::Save();
    game::SRegionAroundDisable();
    game::End("", false);
    return true;
  }
  else
    return false;
}

truth commandsystem::Read(character* Char)
{
  if(!Char->CanRead() && !game::GetSeeWholeMapCheatMode())
  {
    ADD_MESSAGE("You can't read.");
    return false;
  }

  if(!Char->GetStack()->SortedItems(Char, &item::IsReadable))
  {
    ADD_MESSAGE("You have nothing to read!");
    return false;
  }

  if(Char->GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
  {
    ADD_MESSAGE("It is too dark to read.");
    return false;
  }

  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to read?"), 0, &item::IsReadable);
  return Item && Char->ReadItem(Item);
}

truth commandsystem::Dip(character* Char)
{

  if(!Char->GetStack()->SortedItems(Char, &item::IsDippable) && !Char->EquipsSomething(&item::IsDippable))
  {
    ADD_MESSAGE("You have nothing to dip!");
    return false;
  }

  truth HasDipDestination = Char->PossessesItem(&item::IsDipDestination);
  truth DipDestinationNear = false;

  for(int d = 0; d < 9; ++d)
  {
    lsquare* Square = Char->GetNaturalNeighbourLSquare(d);

    if(Square && Square->IsDipDestination())
      DipDestinationNear = true;
  }

  if(!HasDipDestination && !DipDestinationNear)
  {
    ADD_MESSAGE("There is nothing to dip anything into.");
    return false;
  }

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to dip?"), &item::IsDippable);

  if(Item)
  {
    if(!HasDipDestination
       || (DipDestinationNear && game::TruthQuestion(CONST_S("Do you wish to dip in a nearby square? [y/N]"))))
    {
      int Dir = game::DirectionQuestion(CONST_S("Where do you want to dip ") + Item->GetName(DEFINITE)
                                        + "? [press a direction key or '.']", false, true);
      v2 Pos = Char->GetPos() + game::GetMoveVector(Dir);

      if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Pos) || !Char->GetNearLSquare(Pos)->IsDipDestination()){
        return false;
      }

      bool b = Char->GetNearLSquare(Pos)->DipInto(Item, Char);

      return b;
    }
    else
    {
      item* DipTo = Char->SelectFromPossessions(CONST_S("Into what do you wish to dip it?"), &item::IsDipDestination);

      if(DipTo)
      {
        if(Item == DipTo)
        {
          ADD_MESSAGE("Very funny...");
          return false;
        }

        Item->DipInto(DipTo->CreateDipLiquid(Item->DipIntoVolume()), Char);
        return true;
      }
    }
  }

  return false;
}

truth commandsystem::ShowKeyLayout(character*)
{
  felist List(CONST_S("Keyboard Layout"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Key       Description"));
  festring Buffer;

  for(int c = 1; GetCommand(c); ++c)
    if(!GetCommand(c)->IsWizardModeFunction())
    {
      Buffer.Empty();
      Buffer << GetCommand(c)->GetKey();
      Buffer.Resize(10);
      List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
    }

  if(game::WizardModeIsActive())
  {
    List.AddEntry(CONST_S(""), WHITE);
    List.AddEntry(CONST_S("Wizard mode functions:"), WHITE);
    List.AddEntry(CONST_S(""), WHITE);

    for(int c = 1; GetCommand(c); ++c)
      if(GetCommand(c)->IsWizardModeFunction())
      {
        Buffer.Empty();
        Buffer << GetCommand(c)->GetKey();
        Buffer.Resize(10);
        List.AddEntry(Buffer + GetCommand(c)->GetDescription(), LIGHT_GRAY);
      }
  }

  game::SetStandardListAttributes(List);
  List.Draw();
  return false;
}

void commandsystem::PlayerDiedLookMode(bool bSeeWholeMapCheatMode){
  //TODO why this does not work??? if(!PLAYER->IsDead())return;
#ifdef WIZARD
  if(bSeeWholeMapCheatMode && !game::GetSeeWholeMapCheatMode()){
    game::SeeWholeMap(); //1
    game::SeeWholeMap(); //2
  }
#endif
  commandsystem::Look(PLAYER);
}

truth commandsystem::Look(character* Char)
{
  festring Msg; DBG1(Char->GetSquareUnder());
  if(!game::IsInWilderness()){
    if(Char->GetSquareUnder()==NULL){ //dead (removed) Char (actually PlayerDiedLookMode())
      game::GetCurrentLevel()->AddSpecialCursors(); //TODO isnt, this alone, enough?
    }else{
      Char->GetLevel()->AddSpecialCursors();
    }
  }

  if(!game::IsInWilderness())
    Msg = CONST_S("Direction keys move cursor, ESC exits, 'i' examines items, 'c' examines a character.");
  else
    Msg = CONST_S("Direction keys move cursor, ESC exits, 'c' examines a character.");

  v2 pos = Char->GetPosSafely();
  if(pos.Is0())pos = game::GetCamera()+v2(game::GetScreenXSize(),game::GetScreenYSize())/2; // gum: this may happen if player died, the probably position is around screen center, if it is not good enough just deny it and add a log message saying unable to.
  game::PositionQuestion(Msg,pos,&game::LookHandler, &game::LookKeyHandler, ivanconfig::GetLookZoom());
  game::RemoveSpecialCursors();
  return false;
}

truth commandsystem::WhatToEngrave(character* Char)
{
  return WhatToEngrave(Char,false,v2());
}
truth commandsystem::WhatToEngrave(character* Char,bool bEngraveMapNote,v2 v2EngraveMapNotePos)
{
  if(!Char->CanRead())
  {
    ADD_MESSAGE("You can't even read.");
    return false;
  }

  int Key = 0;
  while(!(Key == KEY_ESC || Key == ' '))
  {
    if(!bEngraveMapNote)
      Key = game::AskForKeyPress(CONST_S("Where do you want to engrave? "
                                         "'.' square, 'i' inventory, ESC exits"));

    int iLSqrLimit=80;
    if(bEngraveMapNote)
    {
      festring What;

      lsquare* lsqrN = game::GetCurrentLevel()->GetLSquare(v2EngraveMapNotePos);
      if(lsqrN!=NULL){ //TODO can this be NULL?
        if(lsqrN->GetEngraved()!=NULL){
          cchar* c = lsqrN->GetEngraved();
          if(c!=NULL)What=c;
          if(What.GetSize()>0 && What[0]==game::MapNoteToken()){
            std::string str=What.CStr();What.Empty();What<<str.substr(1).c_str(); //TODO add substr to festring
          }
        }
      }

      if(game::StringQuestion(What, CONST_S("Write your map note (optionally position mouse cursor over it before editing):"), WHITE, 0, iLSqrLimit, true) == NORMAL_EXIT){
        game::SetMapNote(lsqrN,What);
      }

      break;
    }

    if(Key == '.')
    {
      festring What;

      if(game::StringQuestion(What, CONST_S("What do you want to engrave here?"), WHITE, 0, iLSqrLimit, true) == NORMAL_EXIT)
        Char->GetNearLSquare(Char->GetPos())->Engrave(What);

      break;
    }

    if(Key == 'i')
    {
      if(!Char->GetStack()->GetItems())
      {
        ADD_MESSAGE("You have nothing to inscribe on!");
        return false;
      }

      stack::SetSelected(0);

      for(;;)
      {
        itemvector ToAddLabel;
        game::DrawEverythingNoBlit();
        Char->GetStack()->DrawContents(ToAddLabel, Char, CONST_S("What item do you want to inscribe on?"), REMEMBER_SELECTED);

        if(ToAddLabel.empty())
          break;

        festring What = ToAddLabel[0]->GetLabel();
        if(game::StringQuestion(What, CONST_S("What would you like to inscribe on this item?"), WHITE, 0, 20, true) == NORMAL_EXIT)
          for(int i=0;i<ToAddLabel.size();i++)
            ToAddLabel[i]->SetLabel(What);
      }

      break;
    }
  }

  return false;
}

truth commandsystem::Pray(character* Char)
{
  felist Panthenon(CONST_S("To Whom you want to address your prayers?"));
  Panthenon.SetEntryDrawer(game::GodEntryDrawer);
  int Known[GODS];
  int Index = 0;
  int DivineMaster = Char->GetLSquareUnder()->GetDivineMaster();

  if(DivineMaster == ATHEIST)
  {
    ADD_MESSAGE("Somehow you feel that no god will help you here.");
    return false;
  }

  if(!DivineMaster)
  {
    festring desc;
    for(int c = 1; c <= GODS; ++c)
      if(game::GetGod(c)->IsKnown())
      {
        desc.Empty();
        desc << game::GetGod(c)->GetCompleteDescription();
        if(ivanconfig::IsShowGodInfo())desc << " ("<<game::GetGod(c)->GetDescription()<<")";
        Panthenon.AddEntry(desc, LIGHT_GRAY, 20, c);
        Known[Index++] = c;
      }
  }
  else
    if(game::GetGod(DivineMaster)->IsKnown())
    {
      Panthenon.AddEntry(game::GetGod(DivineMaster)->GetCompleteDescription(), LIGHT_GRAY, 20, DivineMaster);
      Known[0] = DivineMaster;
    }
    else
    {
      ADD_MESSAGE("Somehow you feel that no deity you know can hear your prayers from this place.");
      return false;
    }

  game::SetStandardListAttributes(Panthenon);
  Panthenon.AddFlags(SELECTABLE);
  int Select = Panthenon.Draw();

  if(Select == LIST_WAS_EMPTY)
  {
    ADD_MESSAGE("You do not know any gods.");
    return false;
  }

  if(Select & FELIST_ERROR_BIT)
    return false;
  else
  {
    if(Char->GetLSquareUnder()->GetDivineMaster())
    {
      if(!Select)
      {
        if(game::TruthQuestion(CONST_S("Do you really wish to pray to ")
                               + game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->GetName() + "? [y/N]"))
          game::GetGod(Char->GetLSquareUnder()->GetDivineMaster())->Pray();
        else
          return false;
      }
      else
        return false;
    }
    else
    {
      if(game::TruthQuestion(CONST_S("Do you really wish to pray to ")
                             + game::GetGod(Known[Select])->GetName() + "? [y/N]"))
      {
        if(Char->StateIsActivated(CONFUSED) && !(RAND() & 7))
        {
          int Index;
          for(Index = 1 + RAND() % GODS;
              Index == Known[Select];
              Index = 1 + RAND() % GODS);

          if(game::GetGod(Index)->IsKnown())
            ADD_MESSAGE("You feel something went wrong in the rituals. You have "
                        "accidentally prayed to %s!", game::GetGod(Index)->GetName());
          else
            ADD_MESSAGE("Your rituals were seriously incorrect. You have accidentally "
                        "prayed to an unknown god, but have no idea how!");

          game::GetGod(Index)->Pray();
        }
        else
          game::GetGod(Known[Select])->Pray();
      }
      else
        return false;
    }

    Char->EditAP(-1000);
    return true;
  }
}

truth commandsystem::Kick(character* Char)
{
  static char cmdKey = findCmdKey(&Kick);

  /** No multi-tile support */

  if(!Char->CheckKick())
    return false;

  if(Char->GetBurdenState() == OVER_LOADED)
  {
    ADD_MESSAGE("You try to kick, but you collapse under your load.");
    Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
    return true;
  }

  festring fsQ;
  fsQ<<"In what direction do you wish to kick? [press a direction key or '"<<cmdKey<<"']";
  static int iPreviousDirChosen = DIR_ERROR;
  int Dir = game::DirectionQuestion(fsQ, false, false, cmdKey, iPreviousDirChosen);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

  iPreviousDirChosen = Dir;
  lsquare* Square = Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir));

  if(!Square->CheckKick(Char))
    return false;

  character* Enemy = Square->GetCharacter();

  if(Enemy && !(Enemy->IsMasochist() && Char->GetRelation(Enemy) == FRIEND) && Char->GetRelation(Enemy) != HOSTILE
     && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
    return false;

  /*if(Square->GetCharacter() && Char->GetRelation(Square->GetCharacter()) != HOSTILE)
    if(!game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
    else
      */

  Char->Hostility(Square->GetCharacter());
  Char->Kick(Square, Dir);
  return true;
}

truth commandsystem::Offer(character* Char)
{
  if(!Char->CheckOffer())
    return false;

  lsquare* Square = Char->GetLSquareUnder();

  if(Square->GetOLTerrain() && Square->GetOLTerrain()->AcceptsOffers())
  {
    if(!Char->GetStack()->GetItems())
    {
      ADD_MESSAGE("You have nothing to offer!");
      return false;
    }

    item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to offer?"));

    if(Item)
    {
      if(game::GetGod(Square->GetDivineMaster())->ReceiveOffer(Item))
      {
        Item->RemoveFromSlot();
        Item->SendToHell();
        Char->DexterityAction(5); /** C **/
        return true;
      }
      else
        return false;
    }
    else
      return false;
  }
  else
    ADD_MESSAGE("There isn't any altar here!");

  return false;
}

truth commandsystem::DrawMessageHistory(character*)
{
  msgsystem::DrawMessageHistory();
  return false;
}

truth commandsystem::Throw(character* Char)
{
  static char cmdKey = findCmdKey(&Throw);

  if(!Char->CheckThrow()){
    return false;
  }

  if(!Char->GetStack()->GetItems())
  {
    ADD_MESSAGE("You have nothing to throw!");
    return false;
  }

  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to throw?"), REMEMBER_SELECTED);

  if(Item)
  {
    static int iPreviousDirChosen = DIR_ERROR;
    int Answer = game::DirectionQuestion(CONST_S("In what direction do you wish to throw?  "
                                                 "[press a direction key or Enter]"), false, false, KEY_ENTER, iPreviousDirChosen);

    if(Answer == DIR_ERROR){
      return false;
    }

    iPreviousDirChosen = Answer;

    Char->ThrowItem(Answer, Item);
    Char->EditExperience(ARM_STRENGTH, 75, 1 << 8);
    Char->EditExperience(DEXTERITY, 75, 1 << 8);
    Char->EditExperience(PERCEPTION, 75, 1 << 8);
    Char->EditNP(-50);
    Char->DexterityAction(5);
    if(ivanconfig::IsAutoPickupThrownItems())
      Item->SetTag('t');
    return true;
  }
  else
  {
    return false;
  }
}

ulong itLastApplyID=0; //save it?
truth commandsystem::ApplyAgain(character* Char)
{
  if(itLastApplyID==0){
    ADD_MESSAGE("I need to apply something first.");
    return false;
  }

  item* it=game::SearchItem(itLastApplyID);
  if(!it){
    itLastApplyID=0;
    ADD_MESSAGE("I can't re-apply, it was destroyed.");
    return false;
  }

  if(it->FindCarrier()==Char){
    ADD_MESSAGE("I will apply my %s again.",it->GetName(UNARTICLED).CStr());
    return ApplyWork(Char,it);
  }else
    ADD_MESSAGE("I need to get my %s back!",it->GetName(UNARTICLED).CStr());

  return false;
}

truth commandsystem::Apply(character* Char)
{
  return ApplyWork(Char);
}

truth commandsystem::ApplyWork(character* Char,item* itOverride)
{
  if(!Char->CanApply())
  {
    ADD_MESSAGE("This monster type cannot apply.");
    return false;
  }

  if(!Char->CheckApply()){
    return false;
  }

  if(!Char->PossessesItem(&item::IsAppliable))
  {
    ADD_MESSAGE("You have nothing to apply!");
    return false;
  }

  item* Item = itOverride;
  if(Item==NULL)
    Item = Char->SelectFromPossessions(CONST_S("What do you want to apply?"), &item::IsAppliable);
  bool b = Item && Item->Apply(Char);

  if(b)
    itLastApplyID=Item->GetID();

  return b;
}

truth commandsystem::ForceVomit(character* Char)
{
  if(Char->CanForceVomit())
  {
    int Dir = game::DirectionQuestion(CONST_S("Where do you wish to vomit?  [press a direction key]"), false, true);

    if(Dir != DIR_ERROR)
    {
      v2 VomitPos = Char->GetPos() + game::GetMoveVector(Dir);

      if(Char->GetArea()->IsValidPos(VomitPos))
      {
        ccharacter* Other = Char->GetArea()->GetSquare(VomitPos)->GetCharacter();

        if(Other && Other->GetTeam() != Char->GetTeam()
           && Other->GetRelation(Char) != HOSTILE
           && Other->CanBeSeenBy(Char)
           && !game::TruthQuestion("Do you really want to vomit at " + Other->GetObjectPronoun() + "? [y/N]"))
           return false;

        ADD_MESSAGE(Char->GetForceVomitMessage().CStr());
        Char->Vomit(Char->GetPos() + game::GetMoveVector(Dir), 500 + RAND() % 500, false);
        Char->EditAP(-1000);
        return true;
      }
    }
  }
  else
    ADD_MESSAGE("You can't vomit.");

  return false;
}

truth commandsystem::Zap(character* Char)
{

  if(!Char->CheckZap()){
    return false;
  }

  if(!Char->PossessesItem(&item::IsZappable))
  {
    ADD_MESSAGE("You have nothing to zap with, %s.", game::Insult());
    return false;
  }

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to zap with?"), &item::IsZappable);

  if(Item)
  {
    int Answer = game::DirectionQuestion(CONST_S("In what direction do you wish to zap?  "
                                                 "[press a direction key or '.']"), false, true);

    if(Answer == DIR_ERROR){
      return false;
    }

    if(Item->Zap(Char, Char->GetPos(), Answer))
    {
      Char->EditAP(-100000 / APBonus(Char->GetAttribute(PERCEPTION)));
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

truth commandsystem::Rest(character* Char)
{
  if(Char->StateIsActivated(PANIC))
  {
    ADD_MESSAGE("You are too scared to rest.");
    return false;
  }

  truth Error = false;

  if(Char->GetHP() == Char->GetMaxHP())
  {
    ADD_MESSAGE("You HP is already at its maximum.");
    Error = true;
  }
  else if(!Char->CanHeal())
  {
    ADD_MESSAGE("You cannot heal.");
    Error = true;
  }

  if(Error)
  {
    long MinutesToRest = game::NumberQuestion(CONST_S("How many minutes to wait?"), WHITE, true);

    if(MinutesToRest > 0)
    {
      oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

      if(Terrain)
        Terrain->ShowRestMessage(Char);

      rest* Rest = rest::Spawn(Char);
      Rest->SetMinToStop(game::GetTotalMinutes() + MinutesToRest);
      Rest->SetGoalHP(0);
      Char->SetAction(Rest);
      return true;
    }
    else
      return false;
  }

  long HPToRest = game::ScrollBarQuestion(CONST_S("How many hit points you desire?"),
                                          Char->GetMaxHP(), 1, 0, Char->GetMaxHP(), 0,
                                          WHITE, LIGHT_GRAY, DARK_GRAY);

  if(HPToRest <= Char->GetHP())
  {
    if(HPToRest != 0)
      ADD_MESSAGE("Your HP is already %d.", Char->GetHP());

    return false;
  }

  oterrain* Terrain = Char->GetSquareUnder()->GetOTerrain();

  if(Terrain)
    Terrain->ShowRestMessage(Char);

  rest* Rest = rest::Spawn(Char);
  Rest->SetMinToStop(0);
  Rest->SetGoalHP(HPToRest);
  Char->SetAction(Rest);
  return true;
}

truth commandsystem::ShowMap(character* Char)
{
  return ShowMapWork(Char);
}
truth commandsystem::ShowMapWork(character* Char,v2* pv2ChoseLocation)
{
  static humanoid* h;h = dynamic_cast<humanoid*>(PLAYER);

  bool bChoseLocationMode = pv2ChoseLocation!=NULL;
  
  festring fsHelp;fsHelp<<
    "[Map Help:]\n"
    " F1 - show this message\n"
    " Map notes containing '!' or '!!' will be highlighted.\n"
    " Position mouse cursor over a map note to edit or delete it.\n"
    " In look mode, clicking on a map note will navigate to that location.\n";
  
  if(bChoseLocationMode)
    if(!game::ToggleShowMapNotes())
      game::ToggleShowMapNotes();
  
  if( h && (h->GetLeftArm() || h->GetRightArm()) ){
    if(game::ToggleDrawMapOverlay()){
      lsquare* lsqrH=NULL;
      while(true){
        v2 noteAddPos = Char->GetPos();

        int key;
        if(bChoseLocationMode)
          key='l';
        else
          key = game::KeyQuestion(CONST_S("Cartography notes action: (t)oggle, (e)dit/add, (l)ook mode, (r)otate, (d)elete. (F1 help)"), //TODO KeyQuestion() should detect F1 and return a default answer, currently F1 will just override any other key press
            KEY_ESC, 5, 't', 'l', 'r', 'd', 'e');

        if(specialkeys::IsRequestedEvent(specialkeys::FocusedElementHelp)){
          specialkeys::ConsumeEvent(specialkeys::FocusedElementHelp,fsHelp);
          continue;
        }
        
        switch(key){
          case 'd':
            lsqrH = game::GetHighlightedMapNoteLSquare();
            if(lsqrH!=NULL){
              lsqrH->Engrave(festring());
              game::RefreshDrawMapOverlay();
            }
            continue;
          case 'r':
            game::RotateMapNotes();
            continue;
          case 't':
            if(game::ToggleShowMapNotes())
              ADD_MESSAGE("Let me see my map notes...");
            continue;
          case 'l':
            if(noteAddPos==Char->GetPos()){
              game::RefreshDrawMapOverlay();

              festring fsMsg = pv2ChoseLocation!=NULL ? "Chose a location." :
                "Where do you wish to add a map note?";
              fsMsg<<" [direction keys move cursor, space accepts]";

              v2 start;
              if(pv2ChoseLocation!=NULL){
                if(!(*pv2ChoseLocation).Is0())
                  if(Char->GetLevel()->IsValidPos((*pv2ChoseLocation)))
                    start=(*pv2ChoseLocation);
              }
              if(start.Is0())
                start=Char->GetPos();

              noteAddPos = game::PositionQuestion(fsMsg, start, NULL, NULL, true); DBGSV2(noteAddPos);
              if(noteAddPos==ERROR_V2){
                game::ToggleDrawMapOverlay();
                return false; //continue;
              }
              if(pv2ChoseLocation!=NULL){
                (*pv2ChoseLocation)=noteAddPos;
                game::ToggleDrawMapOverlay();
                return (*pv2ChoseLocation) != Char->GetPos();
              }
            }
            /* no break */
          case 'e':
            if(noteAddPos==Char->GetPos()){
              lsqrH = game::GetHighlightedMapNoteLSquare();
              if(lsqrH!=NULL)
                noteAddPos=lsqrH->GetPos();
            }
            WhatToEngrave(Char,true,noteAddPos);
            game::RefreshDrawMapOverlay();
            continue;
        }
        break;
      }

      game::ToggleDrawMapOverlay();
    }
  }else{
    ADD_MESSAGE("I can't hold the map!");
  }

  return true;
}

truth commandsystem::Sit(character* Char)
{
  lsquare* Square = Char->GetLSquareUnder();
  return (Square->GetOLTerrain() && Square->GetOLTerrain()->SitOn(Char)) || Square->GetGLTerrain()->SitOn(Char);
}

std::vector<v2> RouteGoOn;
level* LevelRouteGoOn=NULL;
v2 v2RouteTarget=v2(0,0); //TODO savegame this?

std::vector<v2> commandsystem::GetRouteGoOnCopy(){
  if(game::GetCurrentLevel()!=LevelRouteGoOn || v2RouteTarget.Is0()){
    std::vector<v2> empty;
    return empty;
  }
  return RouteGoOn;
}

truth commandsystem::Go(character* Char)
{
  int Dir = DIR_ERROR;

  if(LevelRouteGoOn!=Char->GetLevel())
    v2RouteTarget=v2(0,0);

  if(Char->GetPos()==v2RouteTarget) //TODO is near by 1 dist (2 or more may have a wall in-between)
    v2RouteTarget=v2(0,0);

  if(!v2RouteTarget.Is0()){
    switch(game::KeyQuestion(CONST_S("Continue going thru the route? [y/n]"), KEY_ESC, 2, 'y', 'n')){
      case 'y':
        Dir = YOURSELF;
        break;
      case 'n':
        v2RouteTarget=v2(0,0);
        break;
      default:
        return false;
    }
  }

  if(Dir == DIR_ERROR)
    Dir = game::DirectionQuestion(CONST_S("In what direction do you want to go? [press a direction key or '.' for map route]"), false, true);

  if(Dir == DIR_ERROR)
    return false;

  RouteGoOn.clear();
  if(Dir == YOURSELF){
    if(v2RouteTarget.Is0())
      if(!ShowMapWork(Char,&v2RouteTarget)){
        v2RouteTarget=v2(0,0);
        return false;
      }

    if(Char->GetPos()==v2RouteTarget){
      v2RouteTarget=v2(0,0);
      return false;
    }

    std::set<v2> Illegal;
    node* Node = Char->GetLevel()->FindRoute(Char->GetPos(), v2RouteTarget, Illegal, 0, Char);
    if(Node){
      RouteGoOn.clear();
      while(Node->Last)
      {
        RouteGoOn.push_back(Node->Pos);
        Node = Node->Last;
      }
    }
  }

  if(Dir == YOURSELF && RouteGoOn.size()==0){
    v2RouteTarget=v2(0,0);
    return false;
  }

  go* Go = go::Spawn(Char);
  if(Dir == YOURSELF){
    Go->SetRoute(RouteGoOn);
    Go->SetDirectionFromRoute();
    Go->SetIsWalkingInOpen(true); //prevents stopping on path crosses/forks
    LevelRouteGoOn=Char->GetLevel();
  }else{
    Go->SetDirection(Dir);

    int OKDirectionsCounter = 0;

    for(int d = 0; d < Char->GetNeighbourSquares(); ++d)
    {
      lsquare* Square = Char->GetNeighbourLSquare(d);

      if(Square && Char->CanMoveOn(Square))
        ++OKDirectionsCounter;
    }

    Go->SetIsWalkingInOpen(OKDirectionsCounter > 2);
  }

  Char->SetAction(Go);
  Char->EditAP(Char->GetStateAPGain(100)); // gum solution
  Char->GoOn(Go, true);
  return truth(Char->GetAction());
}

truth commandsystem::ShowConfigScreen(character*)
{
  ivanconfig::Show();
  return false;
}

truth commandsystem::AssignName(character*)
{
  game::NameQuestion();
  return false;
}

truth commandsystem::EquipmentScreen(character* Char)
{
  return Char->EquipmentScreen(Char->GetStack(), 0);
}

truth commandsystem::ScrollMessagesDown(character*)
{
  msgsystem::ScrollDown();
  return false;
}

truth commandsystem::ScrollMessagesUp(character*)
{
  msgsystem::ScrollUp();
  return false;
}

truth commandsystem::ShowWeaponSkills(character* Char)
{
  felist List(CONST_S("Your experience in weapon categories"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Category name                 Level     Points    Needed    Battle bonus"));
  truth Something = false;
  festring Buffer;

  for(int c = 0; c < Char->GetAllowedWeaponSkillCategories(); ++c)
  {
    cweaponskill* Skill = Char->GetCWeaponSkill(c);

    if(Skill->GetHits() / 100 || (Char->IsUsingWeaponOfCategory(c)))
    {
      Buffer = Skill->GetName(c);
      Buffer.Resize(30);
      Buffer << Skill->GetLevel();
      Buffer.Resize(40);
      Buffer << Skill->GetHits() / 100;
      Buffer.Resize(50);

      if(Skill->GetLevel() != 20)
        Buffer << (Skill->GetLevelMap(Skill->GetLevel() + 1) - Skill->GetHits()) / 100;
      else
        Buffer << '-';

      Buffer.Resize(60);
      Buffer << '+' << (Skill->GetBonus() - 1000) / 10;

      if(Skill->GetBonus() % 10)
        Buffer << '.' << Skill->GetBonus() % 10;

      Buffer << '%';

      if(Char->IsUsingWeaponOfCategory(c))
        List.AddEntry(Buffer, WHITE);
      else
        List.AddEntry(Buffer, LIGHT_GRAY);

      Something = true;
    }
  }

  if(Char->AddSpecialSkillInfo(List))
    Something = true;

  if(Something)
  {
    game::SetStandardListAttributes(List);
    List.Draw();
  }
  else
    ADD_MESSAGE("You are not experienced in any weapon skill yet!");

  return false;
}

truth commandsystem::WieldInRightArm(character* Char)
{
  if(!Char->CanUseEquipment())
    ADD_MESSAGE("You cannot wield anything.");
  else if(Char->TryToChangeEquipment(Char->GetStack(), 0, RIGHT_WIELDED_INDEX))
  {
    Char->DexterityAction(5);
    return true;
  }

  return false;
}

truth commandsystem::WieldInLeftArm(character* Char)
{

  if(!Char->CanUseEquipment())
    ADD_MESSAGE("You cannot wield anything.");
  else if(Char->TryToChangeEquipment(Char->GetStack(), 0, LEFT_WIELDED_INDEX))
  {
    Char->DexterityAction(5);
    return true;
  }

  return false;
}

truth commandsystem::Search(character* Char)
{
  Char->Search(Char->GetAttribute(PERCEPTION) << 2);
  return true;
}

#ifdef WIZARD

truth commandsystem::WizardMode(character* Char)
{
  if(!game::WizardModeIsActive())
  {
    if(game::TruthQuestion(CONST_S("Do you want to cheat, cheater? This action cannot be undone. [y/N]")))
    {
      game::ActivateWizardMode();
      ADD_MESSAGE("Wizard mode activated.");

      if(game::IsInWilderness())
      {
        v2 ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
        game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(elpuricave::Spawn());
        game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);

        v2 XinrochTombPos = game::GetWorldMap()->GetEntryPos(0, XINROCH_TOMB);
        game::GetWorldMap()->GetWSquare(XinrochTombPos)->ChangeOWTerrain(locationAW::Spawn());
        game::GetWorldMap()->RevealEnvironment(XinrochTombPos, 1);

        game::GetWorldMap()->SendNewDrawRequest();
      }
      else
      {
        game::LoadWorldMap();
        v2 ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
        game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(elpuricave::Spawn());
        game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);

        v2 XinrochTombPos = game::GetWorldMap()->GetEntryPos(0, XINROCH_TOMB);
        game::GetWorldMap()->GetWSquare(XinrochTombPos)->ChangeOWTerrain(locationAW::Spawn());
        game::GetWorldMap()->RevealEnvironment(XinrochTombPos, 1);

        game::SaveWorldMap();
      }

      game::Save();
      game::Save(game::GetAutoSaveFileName());
    }
    else
      return false;
  }
  else
    ADD_MESSAGE("Got some scrolls of wishing.");

  for(int c = 0; c < 5; ++c)
    Char->GetStack()->AddItem(scrollofwishing::Spawn());

  return false;
}

truth commandsystem::AutoPlay(character* Char)
{
  game::IncAutoPlayMode();
  return false;
}

truth commandsystem::RaiseStats(character* Char)
{
  Char->EditAllAttributes(1);
  return false;
}

truth commandsystem::LowerStats(character* Char)
{
  Char->EditAllAttributes(-1);
  return false;
}

truth commandsystem::GainAllItems(character* Char)
{
  itemvectorvector AllItems;
  protosystem::CreateEveryItem(AllItems);
  stack* Stack = game::IsInWilderness() ? Char->GetStack() : Char->GetStackUnder();

  for(uint c = 0; c < AllItems.size(); ++c)
    Stack->AddItem(AllItems[c][0]);

  return false;
}

truth commandsystem::SeeWholeMap(character*)
{
  game::SeeWholeMap();
  return false;
}

truth commandsystem::WalkThroughWalls(character*)
{
  game::GoThroughWalls();
  return false;
}

truth commandsystem::RaiseGodRelations(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(50);

  return false;
}

truth commandsystem::LowerGodRelations(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::GetGod(c)->AdjustRelation(-50);

  return false;
}

truth commandsystem::GainDivineKnowledge(character*)
{
  for(int c = 1; c <= GODS; ++c)
    game::LearnAbout(game::GetGod(c));

  return false;
}

truth commandsystem::SecretKnowledge(character* Char)
{
  felist List(CONST_S("Knowledge of the ancients"));
  List.AddEntry(CONST_S("Character attributes"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character attack info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character defence info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Character danger values"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Item attack info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Miscellaneous item info"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Material info"), LIGHT_GRAY);
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  uint c, Chosen = List.Draw();
  festring Entry;

  if(Chosen & FELIST_ERROR_BIT)
    return false;

  List.Empty();
  List.RemoveFlags(SELECTABLE);

  if(Chosen < 4)
  {
    charactervector& Character = game::GetCharacterDrawVector();
    int TeamSize = 0;

    for(character* p : Char->GetTeam()->GetMember())
      if(p->IsEnabled())
      {
        Character.push_back(p);
        ++TeamSize;
      }

    protosystem::CreateEveryCharacter(Character);
    List.SetEntryDrawer(game::CharacterEntryDrawer);

    switch(Chosen)
    {
     case 0:
      List.AddDescription(CONST_S("                                                AS LS DX AG EN PE IN WI CH MA"));

      for(c = 0; c < Character.size(); ++c)
      {
        Entry.Empty();
        Character[c]->AddName(Entry, UNARTICLED);
        Character[c]->AddAttributeInfo(Entry);
        List.AddEntry(Entry, LIGHT_GRAY, 0, c);
      }

      List.SetPageLength(15);
      break;
     case 1:
      List.AddDescription(CONST_S("                                                  BD        THV       APC"));

      for(c = 0; c < Character.size(); ++c)
      {
        Entry.Empty();
        Character[c]->AddName(Entry, UNARTICLED);
        List.AddEntry(Entry, LIGHT_GRAY, 0, c);
        Character[c]->AddAttackInfo(List);
      }

      List.SetPageLength(20);
      break;
     case 2:
      List.AddDescription(CONST_S("                                                  DV/BV     HP        AV/BS"));

      for(c = 0; c < Character.size(); ++c)
      {
        Entry.Empty();
        Character[c]->AddName(Entry, UNARTICLED);
        Entry.Resize(47);
        Entry << int(Character[c]->GetDodgeValue());
        Entry.Resize(57);
        Entry << Character[c]->GetMaxHP();
        List.AddEntry(Entry, LIGHT_GRAY, 0, c);
        Character[c]->AddDefenceInfo(List);
      }

      List.SetPageLength(25);
      break;
     case 3:
      List.AddDescription(CONST_S("                                                  Danger    NGM       EGM"));

      for(c = 0; c < Character.size(); ++c)
      {
        Entry.Empty();
        Character[c]->AddName(Entry, UNARTICLED);
        Entry.Resize(47);
        Entry << int(Character[c]->GetRelativeDanger(Char, true) * 1000);
        Entry.Resize(57);
        const dangerid& DI = game::GetDangerMap().find(configid(Character[c]->GetType(),
                                                                Character[c]->GetConfig()))->second;
        Entry << int(DI.NakedDanger * 1000);
        Entry.Resize(67);
        Entry << int(DI.EquippedDanger * 1000);
        List.AddEntry(Entry, LIGHT_GRAY, 0, c);
      }

      List.SetPageLength(15);
      break;
    }

    List.Draw();

    for(c = TeamSize; c < Character.size(); ++c)
      delete Character[c];

    game::ClearCharacterDrawVector();
  }
  else if(Chosen < 6)
  {
    itemvectorvector& Item = game::GetItemDrawVector();
    protosystem::CreateEveryItem(Item);
    List.SetEntryDrawer(game::ItemEntryDrawer);
    List.SetPageLength(20);

    switch(Chosen)
    {
     case 4:
      List.AddDescription(CONST_S("                                        Weight    Size      SR        DAM"));

      for(c = 0; c < Item.size(); ++c)
      {
        Entry.Empty();
        Item[c][0]->AddName(Entry, UNARTICLED);
        List.AddEntry(Entry, LIGHT_GRAY, 0, c, true);
        Item[c][0]->AddAttackInfo(List);
      }

      break;
     case 5:
      List.AddDescription(CONST_S("                                        \177              OV             NV"));

      for(c = 0; c < Item.size(); ++c)
      {
        Entry.Empty();
        Item[c][0]->AddName(Entry, UNARTICLED);
        List.AddEntry(Entry, LIGHT_GRAY, 0, c, true);
        Item[c][0]->AddMiscellaneousInfo(List);
      }

      break;
    }

    List.Draw();

    for(c = 0; c < Item.size(); ++c)
      delete Item[c][0];

    game::ClearItemDrawVector();
  }
  else if(Chosen < 7)
  {
    std::vector<material*> Material;
    protosystem::CreateEveryMaterial(Material);
    List.SetPageLength(30);
    List.AddDescription(CONST_S("                                        Str.  Flex. Dens. Int.  God"));

    for(c = 0; c < Material.size(); ++c)
    {
      Entry.Empty();
      Material[c]->AddName(Entry, false, false);
      Entry.Resize(40);
      Entry << Material[c]->GetStrengthValue();
      Entry.Resize(46);
      Entry << Material[c]->GetFlexibility();
      Entry.Resize(52);
      Entry << Material[c]->GetDensity();
      Entry.Resize(58);
      Entry << Material[c]->GetIntelligenceRequirement();
      Entry.Resize(64);
      Entry << game::GetGod(Material[c]->GetAttachedGod())->GetName();
      List.AddEntry(Entry, Material[c]->GetColor());
    }

    List.Draw();

    for(c = 0; c < Material.size(); ++c)
      delete Material[c];
  }

  List.PrintToFile(game::GetUserDataDir() + "secret" + Chosen + ".txt");
  ADD_MESSAGE("Info written also to %ssecret%d.txt.", game::GetUserDataDir().CStr(), Chosen);
  return false;
}

truth commandsystem::DetachBodyPart(character* Char)
{
  Char->DetachBodyPart();
  return false;
}

truth commandsystem::SetFireToBodyPart(character* Char)
{
  Char->SetFireToBodyPart();
  return false;
}

truth commandsystem::SummonMonster(character* Char)
{
  character* Summoned = 0;

  while(!Summoned)
  {
    festring Temp;

    if(game::DefaultQuestion(Temp, CONST_S("Summon which monster?"), game::GetDefaultSummonMonster(), true) == ABORTED)
      return false;

    Summoned = protosystem::CreateMonster(Temp);
  }

  Summoned->SetTeam(game::GetTeam(MONSTER_TEAM));
  Summoned->PutNear(Char->GetPos());
  return false;
}

truth commandsystem::LevelTeleport(character*)
{
  long Level = game::NumberQuestion(CONST_S("To which level?"), WHITE);

  if(Level <= 0 || Level > game::GetLevels())
  {
    ADD_MESSAGE("There is no level %ld in this dungeon, %s!", Level, game::Insult());
    return false;
  }

  if(Level == game::GetCurrentLevelIndex() + 1)
  {
    ADD_MESSAGE("You are already here, %s!", game::Insult());
    return false;
  }

  return game::TryTravel(game::GetCurrentDungeonIndex(), Level - 1, RANDOM, true);
}

truth commandsystem::Possess(character* Char)
{
  int Dir = game::DirectionQuestion(CONST_S("Choose creature to possess. [press a direction key]"), false);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

  character* ToPossess = Char->GetNearLSquare(Char->GetPos() + game::GetMoveVector(Dir))->GetCharacter();

  if(ToPossess)
  {
    Char->RemoveFlags(C_PLAYER);
    game::SetPlayer(ToPossess);
  }
  else
    ADD_MESSAGE("There's no one to possess, %s!", game::Insult());

  return true; // The old player's turn must end
}

truth commandsystem::Polymorph(character* Char)
{
  character* NewForm;

  if(!Char->GetNewFormForPolymorphWithControl(NewForm)){
    return true;
  }

  Char->Polymorph(NewForm, game::NumberQuestion(CONST_S("For how long?"), WHITE));
  return true;
}

#endif

truth commandsystem::ToggleRunning(character* Char)
{
  if(game::PlayerIsRunning()
     && PLAYER->StateIsActivated(PANIC)
     && PLAYER->GetTirednessState() != FAINTING)
  {
    ADD_MESSAGE("You are too scared to move at a normal pace.");
    return false;
  }

  if(!Char->CanMove())
  {
    ADD_MESSAGE("Well, well, aren't we funny today, eh?");
    return false;
  }

  game::SetPlayerIsRunning(!game::PlayerIsRunning());
  return false;
}

truth commandsystem::IssueCommand(character* Char)
{
  if(!Char->CheckTalk())
    return false;

  return game::CommandQuestion();
}
