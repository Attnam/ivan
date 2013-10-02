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

#ifndef __WSKILL_H__
#define __WSKILL_H__

#include "ivandef.h"

class outputfile;
class inputfile;

class weaponskill
{
 public:
  weaponskill() : Level(0), Hits(0), HitCounter(0) { }
  int GetLevel() const { return Level; }
  int GetHits() const { return Hits; }
  truth Tick();
  truth AddHit(int);
  truth SubHit(int);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetLevelMap(int) const = 0;
  virtual ulong GetUnuseTickMap(int) const = 0;
  virtual int GetUnusePenaltyMap(int) const = 0;
 protected:
  int Level;
  int Hits;
  uint HitCounter;
};

class cweaponskill : public weaponskill
{
 public:
  virtual int GetLevelMap(int) const;
  virtual ulong GetUnuseTickMap(int) const;
  virtual int GetUnusePenaltyMap(int) const;
  cchar* GetName(int) const;
  int GetBonus() const { return 1000 + 50 * Level; }
  void AddLevelUpMessage(int) const;
  void AddLevelDownMessage(int) const;
};

inline outputfile& operator<<(outputfile& SaveFile, const cweaponskill& WeaponSkill)
{
  WeaponSkill.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, cweaponskill& WeaponSkill)
{
  WeaponSkill.Load(SaveFile);
  return SaveFile;
}

class sweaponskill : public weaponskill
{
 public:
  sweaponskill() : ID(0), Weight(0), Config(0) { }
  sweaponskill(citem*);
  virtual int GetLevelMap(int) const;
  virtual ulong GetUnuseTickMap(int) const;
  virtual int GetUnusePenaltyMap(int) const;
  int GetBonus() const { return Level ? 1150 + 25 * (Level - 1) : 1000; }
  void AddLevelUpMessage(cchar*) const;
  void AddLevelDownMessage(cchar*) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  truth IsSkillOf(citem*) const;
  truth IsSkillOfCloneMother(citem*, ulong) const;
  void SetID(ulong What) { ID = What; }
  ulong GetID() const { return ID; }
  void PreProcessForBone() { ID = -ID; }
 private:
  ulong ID;
  long Weight;
  int Config;
};

inline outputfile& operator<<(outputfile& SaveFile, const sweaponskill* WeaponSkill)
{
  WeaponSkill->Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, sweaponskill*& WeaponSkill)
{
  WeaponSkill = new sweaponskill;
  WeaponSkill->Load(SaveFile);
  return SaveFile;
}

#endif
