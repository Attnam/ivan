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

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "ivandef.h"

typedef truth (item::*sorter)(ccharacter*) const;

class command
{
 public:
  command(truth (*)(character*), cchar*, char, char, char, truth, truth = false);
  truth (*GetLinkedFunction() const)(character*) { return LinkedFunction; }
  cchar* GetDescription() const { return Description; }
  int GetKey() const;
  truth IsUsableInWilderness() const { return UsableInWilderness; }
  truth IsWizardModeFunction() const { return WizardModeFunction; }
  int SetCustomKey(int iKey){ int iKeyBkp=Key4; Key4 = iKey; return iKeyBkp; }
 private:
  truth (*LinkedFunction)(character*);
  cchar* Description;
  char Key1;
  char Key2;
  char Key3;
  int Key4; // custom keys can be more than 1 char long as ex.: KEY_HOME is 0x147 or 0x0147
  truth UsableInWilderness;
  truth WizardModeFunction;
};

class commandsystem
{
 public:
  static command* GetCommand(int I) { return Command[I]; }
  static truth IsForRegionListItem(int iIndex);
  static truth IsForRegionSilhouette(int iIndex);
  static void PlayerDiedLookMode(bool bSeeWholeMapCheatMode=false);
  static void PlayerDiedWeaponSkills();
  static void SaveSwapWeapons(outputfile& SaveFile);
  static void LoadSwapWeapons(inputfile& SaveFile);
  static void ClearSwapWeapons();
  static std::vector<v2> GetRouteGoOnCopy();
 private:
  static truth Apply(character*);
  static truth ApplyWork(character* Char,item* itOverride=NULL);
  static truth ApplyAgain(character* Char);
  static truth Close(character*);
  static truth Eat(character*);
  static truth Drink(character*);
  static truth Taste(character*);
  static truth Dip(character*);
  static truth DrawMessageHistory(character*);
  static truth SwapWeapons(character* Char);
  static truth SwapWeaponsWork(character* Char, int iIndexOverride=-1);
  static truth SwapWeaponsCfg(character* Char);
  static truth Drop(character*);
  static truth ForceVomit(character*);
  static truth GoDown(character*);
  static truth GoUp(character*);
  static truth Kick(character*);
  static truth Look(character*);
  static truth NOP(character*);
  static truth Offer(character*);
  static truth Open(character*);
  static truth PickUp(character*);
  static truth Pray(character*);
  static truth AskFavour(character*);
  static truth Craft(character*);
  static truth Quit(character*);
  static truth Read(character*);
  static truth Save(character*);
  static truth ShowInventory(character*);
  static truth ShowKeyLayout(character*);
  static truth ShowWeaponSkills(character*);
  static truth Talk(character*);
  static truth Throw(character*);
  static truth EquipmentScreen(character*);
  static truth WhatToEngrave(character*);
  static truth WhatToEngrave(character* Char,bool bEngraveNote,v2 v2EngraveNotePos);
  static truth Zap(character*);
  static truth Rest(character*);
  static truth Sit(character*);
  static truth ShowMap(character*);
  static truth ShowMapWork(character* Char,v2* pv2ChoseLocation=NULL);
  static truth Go(character*);
  static truth ShowConfigScreen(character*);
  static truth ScrollMessagesDown(character*);
  static truth ScrollMessagesUp(character*);
  static truth WieldInRightArm(character*);
  static truth WieldInLeftArm(character*);
  static truth AssignName(character*);
  static truth Search(character*);
  static truth Consume(character*, cchar*, cchar*, sorter, truth = false);
#ifdef WIZARD
  static truth WizardMode(character*);
  static truth AutoPlay(character* Char);
  static truth RaiseStats(character*);
  static truth LowerStats(character*);
  static truth SeeWholeMap(character*);
  static truth WalkThroughWalls(character*);
  static truth RaiseGodRelations(character*);
  static truth LowerGodRelations(character*);
  static truth GainDivineKnowledge(character*);
  static truth GainAllItems(character*);
  static truth SecretKnowledge(character*);
  static truth DetachBodyPart(character*);
  static truth SetFireToBodyPart(character*);
  static truth SummonMonster(character*);
  static truth LevelTeleport(character*);
  static truth Possess(character*);
  static truth Polymorph(character*);
#else
  static truth DevConsCmd(character* Char);
#endif
  static truth ToggleRunning(character*);
  static truth IssueCommand(character*);
  static command* Command[];
};

#endif
