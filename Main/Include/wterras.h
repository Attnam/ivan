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

#ifndef __WTERRAS_H__
#define __WTERRAS_H__

#include "wterra.h"

GWTERRAIN(ocean, gwterrain)
{
 public:
  ocean() { SetAnimationFrames(32); }
  virtual v2 GetBitmapPos(int) const;
  virtual cchar* SurviveMessage() const;
  virtual cchar* MonsterSurviveMessage() const;
  virtual cchar* DeathMessage() const;
  virtual cchar* MonsterDeathVerb() const;
  virtual cchar* ScoreEntry() const;
  virtual truth IsFatalToStay() const { return true; }
  virtual int GetWalkability() const;
};

GWTERRAIN(glacier, gwterrain)
{
};

GWTERRAIN(desert, gwterrain)
{
};

GWTERRAIN(snow, gwterrain)
{
};

GWTERRAIN(jungle, gwterrain)
{
};

GWTERRAIN(leafyforest, gwterrain)
{
};

GWTERRAIN(evergreenforest, gwterrain)
{
};

GWTERRAIN(steppe, gwterrain)
{
};

OWTERRAIN(attnam, owterrain)
{
};

OWTERRAIN(elpuricave, owterrain)
{
};

OWTERRAIN(newattnam, owterrain)
{
};

OWTERRAIN(underwatertunnel, owterrain)
{
};

OWTERRAIN(underwatertunnelexit, owterrain)
{
};

OWTERRAIN(aslonacastle, owterrain)
{
};

OWTERRAIN(rebelcamp, owterrain)
{
};

OWTERRAIN(goblinfort, owterrain)
{
};

OWTERRAIN(blackmarket, owterrain)
{
};

OWTERRAIN(locationAA, owterrain)
{
};

OWTERRAIN(locationAB, owterrain)
{
};

OWTERRAIN(locationAC, owterrain)
{
};

OWTERRAIN(locationAD, owterrain)
{
};

OWTERRAIN(locationAE, owterrain)
{
};

OWTERRAIN(locationAF, owterrain)
{
};

OWTERRAIN(locationAG, owterrain)
{
};

OWTERRAIN(locationAH, owterrain)
{
};

OWTERRAIN(locationAI, owterrain)
{
};

OWTERRAIN(locationAJ, owterrain)
{
};


OWTERRAIN(locationAK, owterrain)
{
};

OWTERRAIN(locationAL, owterrain)
{
};

OWTERRAIN(locationAM, owterrain)
{
};

OWTERRAIN(locationAN, owterrain)
{
};

OWTERRAIN(locationAO, owterrain)
{
};

OWTERRAIN(locationAP, owterrain)
{
};

OWTERRAIN(locationAQ, owterrain)
{
};

OWTERRAIN(locationAR, owterrain)
{
};

OWTERRAIN(locationAS, owterrain)
{
};

OWTERRAIN(locationAT, owterrain)
{
};

OWTERRAIN(locationAU, owterrain)
{
};

OWTERRAIN(locationAV, owterrain)
{
};

OWTERRAIN(xinrochtomb, owterrain)
{
};

OWTERRAIN(locationAX, owterrain)
{
};

OWTERRAIN(locationAY, owterrain)
{
};

OWTERRAIN(locationAZ, owterrain)
{
};

OWTERRAIN(locationBA, owterrain)
{
};

OWTERRAIN(locationBB, owterrain)
{
};

OWTERRAIN(locationBC, owterrain)
{
};

OWTERRAIN(locationBD, owterrain)
{
};

OWTERRAIN(locationBE, owterrain)
{
};

OWTERRAIN(locationBF, owterrain)
{
};

#endif
