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

#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "festring.h"
#include "ivandef.h"
#include "v2.h"

class area;
class gterrain;
class oterrain;
class outputfile;
class inputfile;
class character;

class square
{
 public:
  square(area*, v2);
  virtual ~square();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  virtual character* GetCharacter() const { return Character; }
  ulong GetLastSeen() const { return LastSeen; }
  v2 GetPos() const { return Pos; }
  area* GetArea() const { return AreaUnder; }
  virtual gterrain* GetGTerrain() const = 0;
  virtual oterrain* GetOTerrain() const = 0;
  festring GetMemorizedDescription() { return MemorizedDescription; }
  void SetMemorizedDescription(cfestring& What) { MemorizedDescription = What; }
  virtual truth CanBeSeenByPlayer(truth = false) const = 0;
  virtual truth CanBeSeenFrom(v2, long, truth = false) const = 0;
  void SendNewDrawRequest() { Flags |= NEW_DRAW_REQUEST; }
  void SendStrongNewDrawRequest() { Flags |= STRONG_NEW_DRAW_REQUEST; }
  cchar* SurviveMessage(character*) const;
  cchar* MonsterSurviveMessage(character*) const;
  cchar* DeathMessage(character*) const;
  cchar* MonsterDeathVerb(character*) const;
  cchar* ScoreEntry(character*) const;
  truth IsFatalToStay() const;
  int GetEntryDifficulty() const;
  int GetRestModifier() const;
  void IncStaticAnimatedEntities()
  {
    ++StaticAnimatedEntities;
    ++AnimatedEntities;
  }
  void DecStaticAnimatedEntities()
  {
    if(!StaticAnimatedEntities)
      int esko = esko = 2;

    --StaticAnimatedEntities;
    --AnimatedEntities;
  }
  void IncAnimatedEntities() { ++AnimatedEntities; }
  void DecAnimatedEntities() { --AnimatedEntities; }
  truth CanBeSeenBy(ccharacter*, truth = false) const;
  col24 GetLuminance() const { return Luminance; }
  square* GetNeighbourSquare(int) const;
  square* GetNearSquare(v2) const;
  virtual int GetSquareWalkability() const = 0;
  void SetCharacter(character* What) { Character = What; }
  void AddFlags(ulong What) { Flags |= What; }
  void RemoveFlags(ulong What) { Flags &= ~What; }
  virtual truth HasBeenSeen() const { return LastSeen; }
  virtual void SurviveEffect(character*);
 protected:
  festring MemorizedDescription;
  area* AreaUnder;
  character* Character;
  v2 Pos;
  col24 Luminance;
  mutable ulong Flags;
  ushort StaticAnimatedEntities;
  ushort AnimatedEntities;
  mutable uchar LastSeen;
};

#endif
