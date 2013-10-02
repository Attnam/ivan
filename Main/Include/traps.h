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

#ifndef __TRAPS_H__
#define __TRAPS_H__

#include "trap.h"

class bitmap;

TRAP(web, trap)
{
 public:
  web();
  virtual ~web();
  virtual void AddDescription(festring&) const;
  virtual truth TryToUnStick(character*, v2);
  virtual int GetTrapBaseModifier() const { return Strength; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
  virtual int GetTrapType() const { return GetType(); }
  virtual ulong GetTrapID() const { return TrapData.TrapID; }
  virtual ulong GetVictimID() const { return TrapData.VictimID; }
  virtual void AddTrapName(festring&, int) const;
  virtual void UnStick() { TrapData.VictimID = 0; }
  virtual void UnStick(int I) { TrapData.BodyParts &= ~(1 << I); }
  void SetStrength(int What) { Strength = What; }
  virtual void Draw(blitdata&) const;
  truth IsStuckToBodyPart(int) const;
  virtual void ReceiveDamage(character*, int, int, int);
  virtual void Destroy();
  virtual truth IsDangerous(ccharacter* Char) const { return CanBeSeenBy(Char); }
  virtual truth CanBeSeenBy(ccharacter*) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void Untrap();
 protected:
  trapdata TrapData;
  bitmap* Picture;
  int Strength; /* must be more than 0 */
};

#endif
