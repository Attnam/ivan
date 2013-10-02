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

#ifndef __SMOKE_H__
#define __SMOKE_H__

#include <vector>

#include "entity.h"
#include "v2.h"

class gas;
class lsquare;
class bitmap;
class inputfile;
class outputfile;

class smoke : public entity
{
 public:
  /* Come To The Dark Side */
  smoke* Next;
 public:
  smoke();
  smoke(gas*, lsquare*);
  virtual ~smoke();
  virtual void Be();
  virtual void Draw(blitdata&) const;
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  void Save(outputfile&) const;
  void Load(inputfile&);
  virtual truth IsOnGround() const { return true; }
  void AddBreatheMessage() const;
  void Merge(gas*);
  cmaterial* GetGas() const { return Gas; }
  truth IsDangerousToBreathe(ccharacter*) const;
  truth IsScaryToBreathe(ccharacter*) const;
 protected:
  material* Gas;
  std::vector<bitmap*> Picture;
  lsquare* LSquareUnder;
  alpha Alpha;
};

outputfile& operator<<(outputfile&, const smoke*);
inputfile& operator>>(inputfile&, smoke*&);

#endif
