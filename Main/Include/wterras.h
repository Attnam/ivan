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
  virtual cchar* GetNameStem() const;
  virtual truth UsesLongArticle() const { return true; }
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 10; }
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
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 90; }
};

GWTERRAIN(desert, gwterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 20; }
};

GWTERRAIN(snow, gwterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 80; }
};

GWTERRAIN(jungle, gwterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 50; }
};

GWTERRAIN(leafyforest, gwterrain)
{
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 60; }
};

GWTERRAIN(evergreenforest, gwterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual truth UsesLongArticle() const { return true; }
  virtual int GetPriority() const { return 70; }
};

GWTERRAIN(steppe, gwterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetPriority() const { return 30; }
};

OWTERRAIN(attnam, owterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
};

OWTERRAIN(elpuricave, owterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
};

OWTERRAIN(newattnam, owterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
};

OWTERRAIN(underwatertunnel, owterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual truth UsesLongArticle() const { return true; }
};

OWTERRAIN(underwatertunnelexit, owterrain)
{
 public:
  virtual cchar* GetNameStem() const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual truth UsesLongArticle() const { return true; }
  virtual int GetAttachedArea() const { return 2; }
};

#endif
