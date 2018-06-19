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
class blitdata;

/**
 * temporary, less than 1s, not to be saved
 */
struct hiteffectSetup { //TODO some of these are not actually an external setup...
  truth Critical;
  int GivenDir;
  int Type;

  character* WhoHits;
  ulong lWhoHitsID;

  character* WhoIsHit;
  ulong lWhoIsHitID;

  item* itemEffectReference;
  ulong lItemEffectReferenceID;

  int iMode;
  lsquare* LSquareUnder;
};
class hiteffect : public entity
{
 public:
  /* Come To The Dark Side */
  hiteffect* Next;
 public:
  hiteffect();
  hiteffect(hiteffectSetup);
  virtual ~hiteffect();
  virtual void Be();
  virtual void Draw() const;
  truth DrawStep();
  void PrepareBlitdata(const blitdata& bld);
  virtual square* GetSquareUnderEntity(int = 0) const;
  virtual truth IsOnGround() const { return true; } //TODO false?
  void End();
 protected:
  bool ItemEfRefExists() const;
  bool WhoIsHitExists() const;
  bool WhoHitsExists() const;
  truth CheckIntegrity(int iDbgState) const;
  void cleanup();
  void chkCleanupAlready() const;
  int iDrawCount; //TODO use realtime??? may not be good on slow machines?
  int iCleanCount;
  bitmap* bmpHitEffect;
  hiteffectSetup setup;
  blitdata bldFinalDraw;
  v2 v2CamPos;
  truth bBlitdataWasSet;
  lsquare* LSquareUnderOfWhoHits;
  std::vector<lsquare*> vExtraSquares;
  v2 v2HitFromSqrPos;
  v2 v2HitToSqrPos;
  v2 v2HitFromToSqrDiff;
  bool bWhoIsHitDied;
  long lStartTime;

  int iState;
  void SetIntegrityState(int i);
};

#endif //__HITEFFECT_H_
