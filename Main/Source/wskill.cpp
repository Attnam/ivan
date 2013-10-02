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

#include "wskill.h"
#include "message.h"
#include "save.h"
#include "item.h"

int CWeaponSkillLevelMap[]
= { 0, 1500, 2000, 3000, 5000, 7500, 10000, 15000,
    20000, 30000, 50000, 75000, 100000, 150000,
    200000, 300000, 500000, 750000, 1000000,
    1500000, 2000000 };

ulong CWeaponSkillUnuseTickMap[]
= { 500000, 400000, 300000, 250000, 200000,
    150000, 125000, 100000, 80000, 62500,
    50000, 40000, 30000, 25000, 20000,
    15000, 12500, 10000, 8000, 6250, 5000 };

int CWeaponSkillUnusePenaltyMap[]
= { 100, 125, 150, 200, 250, 300, 400, 500,
    625, 800, 1000, 1250, 1500, 2000, 2500,
    3000, 4000, 5000, 6250, 8000, 10000 };

cchar* CWeaponSkillName[WEAPON_SKILL_CATEGORIES]
= { "unarmed combat",
    "kicking",
    "biting",
    "uncategorized",
    "small swords",
    "large swords",
    "blunt weapons",
    "axes",
    "polearms",
    "whips",
    "shields"
};

int SWeaponSkillLevelMap[]
= { 0, 500, 750, 1000, 1500, 2000, 3000, 5000,
    7500, 10000, 15000, 20000, 30000, 50000,
    75000, 100000, 150000, 200000, 300000,
    500000, 750000 };

ulong SWeaponSkillUnuseTickMap[]
= { 250000, 200000, 150000, 125000, 100000,
    80000, 62500, 50000, 40000, 30000,
    25000, 20000, 15000, 12500, 10000,
    8000, 6250, 5000, 4000, 3000, 2500 };

int SWeaponSkillUnusePenaltyMap[]
= { 250, 300, 400, 500, 625, 800, 1000,
    1250, 1500, 2000, 2500, 3000, 4000,
    5000, 6250, 8000, 10000, 12500, 15000,
    20000, 25000 };

int cweaponskill::GetLevelMap(int I) const
{ return CWeaponSkillLevelMap[I]; }
ulong cweaponskill::GetUnuseTickMap(int I) const
{ return CWeaponSkillUnuseTickMap[I]; }
int cweaponskill::GetUnusePenaltyMap(int I) const
{ return CWeaponSkillUnusePenaltyMap[I]; }
cchar* cweaponskill::GetName(int Category) const
{ return CWeaponSkillName[Category]; }

sweaponskill::sweaponskill(citem* Item)
: ID(Item->GetID()), Weight(Item->GetWeight()), Config(Item->GetConfig()) { }
int sweaponskill::GetLevelMap(int I) const
{ return SWeaponSkillLevelMap[I]; }
ulong sweaponskill::GetUnuseTickMap(int I) const
{ return SWeaponSkillUnuseTickMap[I]; }
int sweaponskill::GetUnusePenaltyMap(int I) const
{ return SWeaponSkillUnusePenaltyMap[I]; }

void weaponskill::Save(outputfile& SaveFile) const
{
  SaveFile << (int)Level << (int)Hits << (int)HitCounter;
}

void weaponskill::Load(inputfile& SaveFile)
{
  SaveFile >> (int&)Level >> (int&)Hits >> (int&)HitCounter;
}

truth weaponskill::AddHit(int AddHits)
{
  if(!AddHits)
    return false;

  HitCounter = 0;

  if(Hits <= 5000000 - AddHits)
    Hits += AddHits;
  else
    Hits = 5000000;

  int OldLevel = Level;

  while(Level < 20 && Hits >= GetLevelMap(Level + 1))
    ++Level;

  return Level != OldLevel;
}

truth weaponskill::SubHit(int SubHits)
{
  if(!SubHits)
    return false;

  if(Hits >= SubHits)
    Hits -= SubHits;
  else
    Hits = 0;

  int OldLevel = Level;

  while(Level && Hits < GetLevelMap(Level))
    --Level;

  return Level != OldLevel;
}

void cweaponskill::AddLevelUpMessage(int Category) const
{
  ADD_MESSAGE("You advance to skill level %d with %s!",
	      Level, CWeaponSkillName[Category]);
}

void cweaponskill::AddLevelDownMessage(int Category) const
{
  ADD_MESSAGE("You have not practised enough with %s lately. "
	      "Your skill level is reduced to %d!",
	      CWeaponSkillName[Category], Level);
}

void sweaponskill::AddLevelUpMessage(cchar* WeaponName) const
{
  ADD_MESSAGE("You advance to skill level %d with your %s!",
	      Level, WeaponName);
}

void sweaponskill::AddLevelDownMessage(cchar* WeaponName) const
{
  ADD_MESSAGE("You have not practised enough with your %s lately. "
	      "Your skill level is reduced to %d!", WeaponName, Level);
}

void sweaponskill::Save(outputfile& SaveFile) const
{
  weaponskill::Save(SaveFile);
  SaveFile << ID << Weight << (int)Config;
}

void sweaponskill::Load(inputfile& SaveFile)
{
  weaponskill::Load(SaveFile);
  SaveFile >> ID >> Weight >> (int&)Config;
}

truth weaponskill::Tick()
{
  if(Hits && HitCounter++ >= GetUnuseTickMap(Level))
  {
    HitCounter -= GetUnuseTickMap(Level);

    if(SubHit(GetUnusePenaltyMap(Level)))
      return true;
  }

  return false;
}

truth sweaponskill::IsSkillOf(citem* Item) const
{
  return (ID == Item->GetID()
	  && Weight == Item->GetWeight()
	  && Config == Item->GetConfig());
}

truth sweaponskill::IsSkillOfCloneMother(citem* Item, ulong CMID) const
{
  return (ID == CMID
	  && Weight == Item->GetWeight()
	  && Config == Item->GetConfig());
}
