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
#include "confdef.h"
#include "craft.h"
#include "database.h"
#include "felist.h"
#include "game.h"
#include "gear.h"
#include "god.h"
#include "graphics.h"
#include "human.h"
#include "iconf.h"
#include "lterras.h"
#include "materia.h"
#include "materias.h"
#include "message.h"
#include "miscitem.h"
#include "nonhuman.h"
#include "proto.h"
#include "room.h"
#include "stack.h"
#include "team.h"
#include "whandler.h"
#include "worldmap.h"
#include "wsquare.h"
#include "wterras.h"

#ifdef WIZARD
#include "proto.h"
#endif

#include "dbgmsgproj.h"

command::command(truth (*LinkedFunction)(character*), cchar* Description, char Key1, char Key2, char Key3,
                 truth UsableInWilderness, truth WizardModeFunction)
: LinkedFunction(LinkedFunction), Description(Description), Key1(Key1), Key2(Key2), Key3(Key3),
  UsableInWilderness(UsableInWilderness), WizardModeFunction(WizardModeFunction)
{
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
  new command(&Talk, "chat", 'C', 'C', 'C', false),
  new command(&Close, "close", 'c', 'c', 'c', false),
  new command(&Craft, "craft", 'f', 'F', 'f', false),
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
  new command(&Throw, "throw", 't', 't', 't', false),
  new command(&ToggleRunning, "toggle running", 'u', 'U', 'U', true),
  new command(&ForceVomit, "vomit", 'V', 'V', 'V', false),
  new command(&NOP, "wait", '.', '.', '.', true),
  new command(&WieldInRightArm, "wield in right arm", 'w', 'w', 'w', true),
  new command(&WieldInLeftArm, "wield in left arm", 'W', 'W', 'W', true),
#ifdef WIZARD
  new command(&WizardMode, "wizard mode activation", 'X', 'X', 'X', true),
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

truth commandsystem::IsForRegionListItem(int iIndex){ //see code generator helper script prepareCmdsDescrCode.sh (use cygwin)
  cchar* str = Command[iIndex]->GetDescription();
  if(strcmp(str,"apply")==0)return true;
//  if(strcmp(str,"chat")==0)return true;
//  if(strcmp(str,"close")==0)return true;
  if(strcmp(str,"dip")==0)return true;
  if(strcmp(str,"drink")==0)return true;
  if(strcmp(str,"drop")==0)return true;
  if(strcmp(str,"eat")==0)return true;
//  if(strcmp(str,"engrave")==0)return true;
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
//  if(strcmp(str,"engrave")==0)return true;
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
  festring Msg; //DBG1(Char->GetSquareUnder());
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
        festring finalWhat;
        finalWhat << game::MapNoteToken();
        finalWhat << What;
        lsqrN->Engrave(finalWhat);
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

//struct rpdata {
//  humanoid* h; //TODO protect: set only once
//  int Selected; //TODO protect: set only once
//
//  //TODO protect: none of these should be modified outside this class and every change should be dbgmsg logged.
//  int iBaseTurnsToFinish;
//  item* itTool;
//  lsquare* lsqrWhere;
//  lsquare* lsqrCharPos;
//  olterrain* otSpawn;
//  bool bSpendCurrentTurn;
//  bool bHasAllIngredients;
//  bool bCanStart;
//  bool bCanBePlaced;
//  item* itSpawn;
//  int itSpawnTot;
//  object* craftWhat;
//
//  // no init req
//  v2 v2PlaceAt;
//  std::vector<ulong> ingredients; //TODO must be filled based on required volume to craft something
//
//  rpdata(humanoid* H){
//    h=H;
//
//    Selected=-2; //default is -1 means not set, -2 to init
//
//    otSpawn=NULL;
//    itSpawn=NULL;
//    itSpawnTot=1;
//    lsqrCharPos = game::GetCurrentLevel()->GetLSquare(h->GetPos());
//    lsqrWhere = NULL;
//    bCanStart=false;
//    bCanBePlaced=false;
//    bHasAllIngredients=false;
//    bSpendCurrentTurn=false;
//    craftWhat=NULL;
//    itTool=NULL;
//    iBaseTurnsToFinish=1; //TODO should be based on attributes
//  }
//};
rpdata::rpdata(humanoid* H)
{
  h=H;

  Selected=-2; //default is -1 means not set, -2 to init

  otSpawn=NULL;
  itSpawn=NULL;
  itSpawnTot=1;
  lsqrCharPos = NULL;
  lsqrWhere = NULL;
  bCanStart=false;
  bCanBePlaced=false;
  bHasAllIngredients=false;
  bSpendCurrentTurn=false;
  bAlreadyExplained=false;
  craftWhat=NULL;
  itTool=NULL;
  iBaseTurnsToFinish=1; //TODO should be based on attributes
  v2PlaceAt=v2(0,0);
  ingredientsIDs.clear(); //just to init
}

bool canBeCrafted(item* it){
  if(
    game::IsQuestItem(it) ||
    it->GetEnchantment()!=0 ||
    dynamic_cast<amulet*>(it)!=NULL ||
    dynamic_cast<horn*  >(it)!=NULL ||
    dynamic_cast<ring*  >(it)!=NULL ||
    dynamic_cast<scroll*>(it)!=NULL ||
    dynamic_cast<wand*  >(it)!=NULL ||
    false // just to make it easier to re-organize and add checks above
  )return false;

  return true;
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

  virtual bool work(rpdata&){return false;}
  virtual ~recipe(){}

  static int calcTurns(material* mat,float fMult=1.0){ //local function trick TODO anything better?
    /**
     * min is gold: str 55 and spends 5 turns each 1000cm3.
     * TODO quite arbritrary but gameplay wise enough?
     * TODO should use density? or something else than str? fireresistance is not melting point...
     */
    DBG2(mat->GetStrengthValue(),mat->GetVolume());
    float f = 5 * fMult * (mat->GetStrengthValue()/55.0) * (mat->GetVolume()/1000.0); //float for precision, vol is in cm3, so per 1L or 1Kg(water)
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
    static const int aiTypes[iTotToolTypes]={HAMMER,FRYING_PAN,WAR_HAMMER,MACE}; //TODO could be based on volume and weight vs strengh and dexterity to determine how hard is to use the tool
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
      cfestring fsQ, long volume, rpdata& rpd, int& iWeakestCfg, bool bMultSelect = true, int iReqCfg=0,
      bool bMainMaterRemainsBecomeLump=false
  ){DBGLN;
    if(volume==0)
      ABORT("ingredient required 0 volume?");

    // prepare the filter for ALL items also resetting them first!
    const itemvector vi = vitInv(rpd.h);
    for(int i=0;i<vi.size();i++){
      vi[i]->SetValidRecipeIngredient(false);
      if(dynamic_cast<T*>(vi[i])!=NULL){
        if(vi[i]->IsBurning())continue;

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
      rpd.h->GetStack()->DrawContents(ToUse, rpd.h,
        festring("What ingredient(s) will you use ")+fsQ+" ["+volume+"cm3]"+festring("? (hit ESC for more options if available)"), flags, &item::IsValidRecipeIngredient);
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

      if(volume<=0){
        for(int i=0;i<tmpIngredientsIDs.size();i++)
          rpd.ingredientsIDs.push_back(tmpIngredientsIDs[i]);
        break;
      }
    }

    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);

    return volume<=0;
  }

  template <typename T> static void choseOneIngredient(rpdata& rpd){
    int iWeakestCfgDummy;
    choseIngredients<T>(
      festring(""),
      1, //just to chose one of anything
      rpd,
      iWeakestCfgDummy,
      false);
  }

  static itemvector vitInv(humanoid* h){
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

          lumpToMix->RemoveFromSlot();
          lumpToMix->SendToHell(); DBG5("SentToHell",lumpToMix,lumpToMix->GetID(),lumpAtInv,lumpAtInv->GetID());
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

    if(mat->IsLiquid()){
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
struct srpChair : public recipe{
  bool work(rpdata& rpd){
    static const int iReqStickVol=20000;
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("build","a chair");
      desc << "Use hammers, a frying pan or even a mace with sticks."; //TODO this sounds a bit weird :)
    }

    if(rpd.Selected != iListIndex)
      return false;

    rpd.iBaseTurnsToFinish=10;
    rpd.itTool = FindHammeringTool(rpd.h,rpd.iBaseTurnsToFinish);

    rpd.lsqrWhere=rpd.lsqrCharPos;
    if(rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.itTool!=NULL){
      rpd.bCanBePlaced=true;

      festring fsQ("to build ");fsQ<<name;
      int iCfg=-1;
      //TODO this volume should be on the .dat file as chair attribute...
      if(!rpd.bHasAllIngredients){
        rpd.ingredientsIDs.clear();
        rpd.bHasAllIngredients=choseIngredients<stick>(fsQ,iReqStickVol, rpd, iCfg);
      }
      if(!rpd.bHasAllIngredients){
        rpd.ingredientsIDs.clear();
        rpd.bHasAllIngredients=choseIngredients<bone>(fsQ,iReqStickVol, rpd, iCfg);
      }
      if(rpd.bHasAllIngredients){
        rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
        rpd.otSpawn=decoration::Spawn(CHAIR);
        rpd.otSpawn->SetMainMaterial(material::MakeMaterial(iCfg,iReqStickVol));
        rpd.bCanStart=true;
      }
    }

    return true;
  }
};srpChair rpChair;

struct srpWall : public recipe{
  bool work(rpdata& rpd){
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("construct","a wall");
      desc << "Pile stones or skulls to create " << name;
    }

    if(rpd.Selected != iListIndex) // a wall will destroy whatever is in the place
      return false;

    int Dir = game::DirectionQuestion("Build it where?", false, false);DBGLN;
    if(Dir != DIR_ERROR && rpd.h->GetArea()->IsValidPos(rpd.h->GetPos() + game::GetMoveVector(Dir)))
      rpd.lsqrWhere = rpd.h->GetNearLSquare(rpd.h->GetPos() + game::GetMoveVector(Dir));

    if(rpd.lsqrWhere!=NULL && rpd.lsqrWhere->GetOLTerrain()==NULL && rpd.lsqrWhere->GetCharacter()==NULL){
      rpd.bCanBePlaced=true;

      festring fsQ("to build ");fsQ<<name;
      int iCfg=-1;
      int iVol=-1;
      if(!rpd.bHasAllIngredients){
        rpd.ingredientsIDs.clear();
        iVol=9000; //TODO is this too little? a broken wall drops 3 rocks that is about 1000 each, so 3 walls to build one is ok?
        rpd.bHasAllIngredients=choseIngredients<stone>(fsQ,iVol, rpd, iCfg);
      }
      if(!rpd.bHasAllIngredients){
        rpd.ingredientsIDs.clear();
        iVol=10000; //TODO is this too little? necromancers can spawn skeletons making it easy to get skulls, but the broken bone wall will drop bones and not skulls...
        rpd.bHasAllIngredients=choseIngredients<skull>(fsQ,iVol, rpd, iCfg);
      }
      //TODO this doesnt look good. anyway this volume should be on the .dat file as wall/earthWall attribute...
      if(rpd.bHasAllIngredients){
        rpd.v2PlaceAt = rpd.lsqrWhere->GetPos();
        rpd.otSpawn=wall::Spawn(STONE_WALL);//earth::Spawn();
        rpd.otSpawn->SetMainMaterial(material::MakeMaterial(iCfg,iVol));
        rpd.iBaseTurnsToFinish=20;

        rpd.bCanStart=true;
      }
    }

    return true;
  }
};srpWall rpWall;

struct srpMelt : public recipe{
  bool work(rpdata& rpd){
    // lumps are not usable until melt into an ingot.
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("melt","an ingot");
      desc << "Near a forge you can melt things.";
    }

    if(rpd.Selected != iListIndex) //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
      return false;

    ////////////// find the FORGE
    lsquare* lsqrFORGE = NULL;
    int iDist = 1; //TODO improve this (despite fun, is wrong..)
    for(int i=0;i<(8*iDist);i++){
      int iDir = i%8;
      int iMult = 1 + i/8;
      v2 v2Add = game::GetMoveVector(iDir) * iMult;
      v2 v2Pos = rpd.h->GetPos() + v2Add; DBG5(DBGAV2(v2Add),DBGAV2(v2Pos),iMult,iDir,i);
      if(game::GetCurrentLevel()->IsValidPos(v2Pos)){
        lsquare* lsqr = rpd.h->GetNearLSquare(v2Pos);
        olterrain* ot = lsqr->GetOLTerrain();
        if(ot!=NULL && ot->GetConfig() == FORGE && lsqr->CanBeSeenBy(rpd.h)){
          lsqrFORGE = lsqr;
          break;
        }
      }
    }
    if(lsqrFORGE==NULL){
      ADD_MESSAGE("No forge nearby."); //TODO allow user miss-chose
      rpd.bAlreadyExplained=true;
      return true;
    }

    ///////////////////// chose item to melt/smash
    game::DrawEverythingNoBlit();
    choseOneIngredient<item>(rpd);DBGLN;
    if(rpd.ingredientsIDs.empty())
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

    /////////////////////// smash into lumps
    item* LumpMeltable = NULL;
    if(dynamic_cast<lump*>(itToUse)!=NULL){
      if(IsMeltable(itToUse->GetMainMaterial()))
        LumpMeltable = itToUse;
    }else{ // not a lump? it is a destroyable item then..
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
      itToUse->RemoveFromSlot(); //important to not crash elsewhere!!!
      itToUse->SendToHell();  DBG4("SentToHell",itToUse->GetID(),itToUse,LumpMeltable); //TODO if has any magic should release it and also harm

      rpd.bSpendCurrentTurn=true; //this is necessary or item wont be sent to hell...
    }

    if(LumpMeltable==NULL){
      ADD_MESSAGE("Can't melt that.");
      rpd.bAlreadyExplained=true;
      return true;
    }

    if(bJustLumpfyTheIngot){
      lumpMix(vitInv(rpd.h),LumpMeltable,rpd.bSpendCurrentTurn);
      rpd.bAlreadyExplained=true;
      return true;
    }

    ///////////////////////////////////////////////////////////////////////
    ////////////////////////// melt the lump ////////////////////////
    ///////////////////////////////////////////////////////////////////////
    rpd.iBaseTurnsToFinish = calcTurns(LumpMeltable->GetMainMaterial()); DBG1(rpd.iBaseTurnsToFinish);

    rpd.bHasAllIngredients=true;

    rpd.itSpawn = stone::Spawn(INGOT, NO_MATERIALS);

    /**
     * 100cm3 is each of the 2 parts of a dagger.
     * Smaller ingots are easier to manage, less user interaction as they fit better.
     * TODO make this a numeric option?
     */
    int iIngotVol = 100;

    long lVolRemaining = 0;
    long lVolM = LumpMeltable->GetMainMaterial()->GetVolume();
    if(lVolM <= iIngotVol){
      rpd.itSpawnTot = 1;
      iIngotVol = lVolM;
    }else{
      rpd.itSpawnTot = lVolM / iIngotVol;
      lVolRemaining = lVolM % iIngotVol;
      if(lVolRemaining>0){ //split
        LumpMeltable->GetMainMaterial()->SetVolume(lVolM - lVolRemaining); // exact volume that will be used

        /**
         * IMPORTANT!!!
         * the duplicator will vanish with the item ID that is being duplicated
         */
        item* itLumpR = LumpMeltable->DuplicateToStack(rpd.h->GetStack());

        itLumpR->GetMainMaterial()->SetVolume(lVolRemaining);
      }
    }
    DBG4(lVolRemaining,rpd.itSpawnTot,lVolM,iIngotVol);

    rpd.itSpawn->SetMainMaterial(material::MakeMaterial(
        LumpMeltable->GetMainMaterial()->GetConfig(), iIngotVol ));

    rpd.ingredientsIDs.push_back(LumpMeltable->GetID()); //must be AFTER the duplicator

    rpd.bCanStart=true;

    return true;
  }
};srpMelt rpMelt;

struct srpForgeItem : public recipe{
  bool work(rpdata& rpd){
    // also a block to reuse var names w/o specifying the recipe name on them
    if(desc.GetSize()==0){ //TODO automate the sync of req ingredients description
      init("forge","an item");
      desc << "Using something as a hammer, close to an anvil and with a forge nearby you can create items.";
    }

    if(rpd.Selected != iListIndex) //TODO wands should xplode, other magical items should release something harmful beyond the very effect they are imbued with.
      return false;

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
          break;
        }
      }
    }
    if(lsqrAnvil==NULL){
      ADD_MESSAGE("No anvil nearby."); //TODO allow user miss-chose
      rpd.bAlreadyExplained=true;
      return true;
    }

    ////////////// find the forge
    lsquare* lsqrForge = NULL;
    for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){
      for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2(iX,iY));DBG3(lsqr,iX,iY);
        olterrain* ot = lsqr->GetOLTerrain();
        if(ot!=NULL && ot->GetConfig() == FORGE && lsqr->CanBeSeenBy(rpd.h)){
          lsqrForge = lsqr;
          break;
        }
      }
    }
    if(lsqrForge==NULL){
      ADD_MESSAGE("No visible forge nearby."); //TODO allow user miss-chose
      rpd.bAlreadyExplained=true;
      return true;
    }

    //////////////// let user type the item name
    static festring Default; //static to help on reusing! like creating more of the same
    item* itCreate = NULL;
    for(;;){
      festring Temp;
      Temp << Default; // to let us fix previous instead of having to fully type it again

      if(game::DefaultQuestion(Temp, CONST_S("What do you want to create?"), Default, true) == ABORTED){DBGLN;
        break;
      }

      itCreate = protosystem::CreateItemToCraft(Temp);DBGLN;

      if(itCreate){DBGLN;
        if(!canBeCrafted(itCreate)){
          ADD_MESSAGE("You can't enchant it!");
          itCreate->RemoveFromSlot(); //just in case to prevent problems later...
          itCreate->SendToHell();
        }else{
          break;
        }
      }else{
        ADD_MESSAGE("Be more precise!");
      }

      Default.Empty();DBGLN;
      Default << Temp;
    }

    if(itCreate==NULL){
      rpd.bAlreadyExplained=true; //actually was just cancelled by user
      return true;
    }

    material* matM = itCreate->GetMainMaterial();

    long lVolM = matM->GetVolume();
    if(lVolM==0)
      ABORT("main material 0 volume??? %s",itCreate->GetName(DEFINITE).CStr());
    material* matS = itCreate->GetSecondaryMaterial();

    long lVolS = 0;
    if(matS!=NULL){
      lVolS = matS->GetVolume();
      if(lVolS==0)
        ABORT("secondary material set with 0 volume??? %s",itCreate->GetName(DEFINITE).CStr());
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
      return true;
    }

    materialcontainer* mc = dynamic_cast<materialcontainer*>(itCreate);DBGLN;
    bool bIsContainer =
      itCreate->GetStorageVolume()>0 || //chests
      mc!=NULL; //potions, mines... also bananas xD

    /**
     * TODO problem: basically sulphuric acid can already be stored on a metal can ...
     * TODO every materialcontainer should rust depending on it's contents, if made of anything else than glass
     * Keeping allowing creating materialcontainer of non glass because the fix is already required for the existing metal can,
     * so preventing it would still not fix how metal can works...
     */

    bool bIsWeapon = itCreate->IsWeapon(rpd.h);
    bool bReqS = bIsWeapon;
    bool bAllowS = true;
    if(bIsContainer)bAllowS=false;
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
        return true;
      }
    }

    if(bReqS && !bAllowS)
      ABORT("item reqs secondary mat but doesnt allow it??? %s",itCreate->GetName(DEFINITE).CStr());

    rpd.bHasAllIngredients=true;

    itCreate->SetMainMaterial(material::MakeMaterial(iCfgM,lVolM));
    if(bAllowS)
      itCreate->SetSecondaryMaterial(material::MakeMaterial(iCfgS,lVolS));
    else{
      if(mc!=NULL)
        delete mc->RemoveSecondaryMaterial(); //prevents: ex. random liquids like antidote
    }
    rpd.itSpawn = itCreate;

    float fMult=10;//hammering to form it takes time even if the volume is low.
    rpd.iBaseTurnsToFinish=calcTurns(matM,fMult); DBG4(rpd.iBaseTurnsToFinish,matM->GetName(DEFINITE).CStr(),matM->GetConfig(),matM->GetVolume());
    if(bAllowS && matS!=NULL){
      rpd.iBaseTurnsToFinish+=calcTurns(matS,fMult); DBG4(rpd.iBaseTurnsToFinish,matS->GetName(DEFINITE).CStr(),matS->GetConfig(),matS->GetVolume());
    }

    //TODO glass should require proper tools (don't know what but sure not a hammer)
    //TODO bone should require a dagger
    //TODO 2 tools, one for meltables and the other for glass or bone
    rpd.itTool = FindHammeringTool(rpd.h,rpd.iBaseTurnsToFinish);
    if(rpd.itTool==NULL)
      return true;

    rpd.bCanStart=true;

    return true;
  }
};srpForgeItem rpForgeItem;

struct srpFluids : public recipe{
  int iAddVolMin;
  int iAddVolExtra;
  festring fsTool;
  festring fsCorpse;
  festring fsBottle;
  int iLiqCfg;
  int iMatEff;

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

  virtual bool chkCorpse(const materialdatabase* blood,const materialdatabase* flesh){return false;}

  virtual bool work(rpdata& rpd){
    //extract fluids (not blood as it can be used as nutrition right? *eww* :P)
    if(rpd.Selected != iListIndex)
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

    rpd.itSpawn = potion::Spawn(itBottle->GetConfig()); //may be a vial

    rpd.itSpawn->SetSecondaryMaterial(liquid::Spawn(iLiqCfg, volume));

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

  bool work(rpdata& rpd){
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

  bool work(rpdata& rpd){
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

  rpdata rpd(h);
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

  recipe* prp=NULL;
  #define RP(rp) if(rp.work(rpd))prp=&rp;
  RP(rpAcid);
  RP(rpChair);
  RP(rpForgeItem);
  RP(rpMelt);
  RP(rpPoison);
  RP(rpWall);

  /******************************************************************************************
   * 1st call it just initializes the recipes list after all recipes have been configured!
   */
  if(vrp.size()==0){
    addRecipe((recipe*)&rpChair);
    addRecipe((recipe*)&rpWall);
    addRecipe((recipe*)&rpPoison);
    addRecipe((recipe*)&rpAcid);
    addRecipe((recipe*)&rpMelt);
    addRecipe((recipe*)&rpForgeItem);
    return Craft(Char); //init recipes descriptions at least, one time recursion :>
  }DBGLN;

  if(prp==NULL){DBGLN;
    return false;
  }DBGLN;

  //TODO these messages are generic, therefore dont look good... improve it
  if(rpd.bCanStart){DBGLN;
    if(rpd.ingredientsIDs.size()==0)
      ABORT("no ingredients chosen?");

    if(rpd.itTool!=NULL)
      ADD_MESSAGE("Let me see.. I will use %s as tool.",rpd.itTool->GetName(INDEFINITE).CStr());

    object* pChk=NULL;
    for(int i=0;i<2;i++){
      bool bAbort=false;

      switch(i){
      case 0:
        if(rpd.otSpawn!=NULL){
          // anything to check here?
        }
        break;
      case 1:
        if(rpd.itSpawn!=NULL){
          if(!canBeCrafted(rpd.itSpawn))
            bAbort=true;
        }
        break;
      }

      if(bAbort){
        ABORT(
          "Dear developer, for the sake of balance and challenge do not create recipes for:\n"
          "- Quest items.\n"
          "- Magical items as rings, amulets, wands, scrolls, horns etc.\n"
          "Crafting any of this would be unbalanced as hell and unrealistic given your characters upbringing.\n"
          "You're after all a slave, with no knowledge of magic, and crafting magical items should be beyond most craftsmen.\n"
        );
      }
    }

    if(rpd.otSpawn!=NULL || rpd.itSpawn!=NULL) {
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

      if(rpd.v2PlaceAt.Is0())
        rpd.v2PlaceAt = rpd.lsqrWhere!=NULL ? rpd.lsqrWhere->GetPos() : rpd.lsqrCharPos->GetPos(); //may be ignored anyway, is just a fallback

      if(rpd.itSpawn!=NULL){
        Char->GetStack()->AddItem(rpd.itSpawn); //this is important because during crafting::handle it may item::Be on this item and it may require checking the item's slot that would be NULL w/o this line ex.: during a bone item spoilage
        if(rpd.itSpawn->GetSlot()==NULL)
          ABORT("tmp crafting item should be on a slot for consistency with code everywhere.");
      }

      Char->SwitchToCraft(rpd);

      Char->DexterityAction(5); //TODO is this good? should this be here at all or only after crafting finishes?

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

    // cleanups
    if(rpd.itSpawn!=NULL){
      rpd.itSpawn->RemoveFromSlot(); //just in case it is required one day, this prevents a lot of trouble...
      rpd.itSpawn->SendToHell();
    }

    if(rpd.otSpawn!=NULL)
      rpd.otSpawn->SendToHell();
  }

  if(rpd.bSpendCurrentTurn)
    return true;

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
  /** No multi-tile support */

  if(!Char->CheckKick())
    return false;

  if(Char->GetBurdenState() == OVER_LOADED)
  {
    ADD_MESSAGE("You try to kick, but you collapse under your load.");
    Char->EditAP(-100000 / APBonus(Char->GetAttribute(AGILITY)));
    return true;
  }

  int Dir = game::DirectionQuestion(CONST_S("In what direction do you wish to kick? [press a direction key]"), false);

  if(Dir == DIR_ERROR || !Char->GetArea()->IsValidPos(Char->GetPos() + game::GetMoveVector(Dir)))
    return false;

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

  if(!Char->CheckThrow()){
    return false;
  }

  if(!Char->GetStack()->GetItems())
  {
    ADD_MESSAGE("You have nothing to throw!");
    return false;
  }

  item* Item = Char->GetStack()->DrawContents(Char, CONST_S("What do you want to throw?"));

  if(Item)
  {
    int Answer = game::DirectionQuestion(CONST_S("In what direction do you wish to throw?  "
                                                 "[press a direction key]"), false);

    if(Answer == DIR_ERROR){
      return false;
    }

    Char->ThrowItem(Answer, Item);
    Char->EditExperience(ARM_STRENGTH, 75, 1 << 8);
    Char->EditExperience(DEXTERITY, 75, 1 << 8);
    Char->EditExperience(PERCEPTION, 75, 1 << 8);
    Char->EditNP(-50);
    Char->DexterityAction(5);
    return true;
  }
  else
  {
    return false;
  }
}

truth commandsystem::Apply(character* Char)
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

  item* Item = Char->SelectFromPossessions(CONST_S("What do you want to apply?"), &item::IsAppliable);
  bool b = Item && Item->Apply(Char);

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
  static humanoid* h;h = dynamic_cast<humanoid*>(PLAYER);

  if( h && (h->GetLeftArm() || h->GetRightArm()) ){
    if(game::ToggleDrawMapOverlay()){
      lsquare* lsqrH=NULL;
      while(true){
        v2 noteAddPos = Char->GetPos();
        switch(game::KeyQuestion(CONST_S("Cartography notes action: (t)oggle, (e)dit/add, (l)ook mode, (r)otate, (d)elete."),
          KEY_ESC, 6, 't', 'l', 'r','d','e')
        ){
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
              noteAddPos = game::PositionQuestion(CONST_S(
                "Where do you wish to add a map note? [direction keys move cursor, space accepts]"),
                Char->GetPos(), NULL, NULL, true); DBGSV2(noteAddPos);
              if(noteAddPos==ERROR_V2)
                continue;
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

truth commandsystem::Go(character* Char)
{
  int Dir = game::DirectionQuestion(CONST_S("In what direction do you want to go? [press a direction key]"), false);

  if(Dir == DIR_ERROR)
    return false;

  go* Go = go::Spawn(Char);
  Go->SetDirection(Dir);
  int OKDirectionsCounter = 0;

  for(int d = 0; d < Char->GetNeighbourSquares(); ++d)
  {
    lsquare* Square = Char->GetNeighbourLSquare(d);

    if(Square && Char->CanMoveOn(Square))
      ++OKDirectionsCounter;
  }

  Go->SetIsWalkingInOpen(OKDirectionsCounter > 2);
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

  List.PrintToFile(game::GetHomeDir() + "secret" + Chosen + ".txt");
  ADD_MESSAGE("Info written also to %ssecret%d.txt.", game::GetHomeDir().CStr(), Chosen);
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
