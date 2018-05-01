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

#ifndef __HITEFFECT_H_
#define __HITEFFECT_H_

#include <vector>

#include "entity.h"
#include "v2.h"

class lsquare;

/**
 * temporary, less than 1s, not to be saved
 */
class hiteffect : public entity
{
 public:
  /* Come To The Dark Side */
  hiteffect* Next;
 public:
  hiteffect();
  hiteffect(item*, lsquare*, int iMode, character* WhoIsHit, character* WhoHits, int Type, int GivenDir);
  virtual ~hiteffect();
  virtual void Be();
  virtual void Draw(blitdata&) const;
  truth DrawStep(blitdata bld);
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual truth IsOnGround() const { return true; } //TODO false?
  void End(){iDrawTimes=0;}
 protected:
  lsquare* LSquareUnder;
  int iDrawTimes; //TODO use realtime??? may not be good on slow machines?
  bitmap* bmpHitEffect;
};

#endif //__HITEFFECT_H_
