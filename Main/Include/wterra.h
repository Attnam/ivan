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

#ifndef __WTERRA_H__
#define __WTERRA_H__

#include "terra.h"
#include "wsquare.h"

struct blitdata;

typedef gwterrain* (*gwterrainspawner)();
typedef owterrain* (*owterrainspawner)();

class wterrain
{
 public:
  wterrain() : WSquareUnder(0), AnimationFrames(1) { }
  virtual ~wterrain() { }
  virtual void Load(inputfile&);
  v2 GetPos() const { return WSquareUnder->GetPos(); }
  void SetWSquareUnder(wsquare* What) { WSquareUnder = What; }
  worldmap* GetWorldMap() const { return WSquareUnder->GetWorldMap(); }
  void AddName(festring&, int) const;
  festring GetName(int) const;
  truth IsAnimated() const { return AnimationFrames > 1; }
  void SetAnimationFrames(int What) { AnimationFrames = What; }
  virtual cchar* GetNameStem() const = 0;
 protected:
  virtual truth UsesLongArticle() const { return false; }
  virtual v2 GetBitmapPos(int) const = 0;
  wsquare* WSquareUnder;
  int AnimationFrames;
};

class gwterrainprototype
{
 public:
  gwterrainprototype(gwterrainspawner, cchar*);
  gwterrain* Spawn() const { return Spawner(); }
  gwterrain* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  gwterrainspawner Spawner;
  cchar* ClassID;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  typedef gwterrainprototype prototype;
  virtual void Save(outputfile&) const;
  void Draw(blitdata&) const;
  virtual int GetPriority() const = 0;
  virtual int GetEntryDifficulty() const { return 10; }
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  void CalculateNeighbourBitmapPoses();
  virtual int GetWalkability() const;
 protected:
  std::pair<v2, int> Neighbour[8];
};

class owterrainprototype
{
 public:
  owterrainprototype(owterrainspawner, cchar*);
  owterrain* Spawn() const { return Spawner(); }
  owterrain* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  owterrainspawner Spawner;
  cchar* ClassID;
};

class owterrain : public wterrain, public oterrain
{
 public:
  typedef owterrainprototype prototype;
  virtual void Save(outputfile&) const;
  void Draw(blitdata&) const;
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual int GetAttachedDungeon() const { return 0; }
  virtual int GetAttachedArea() const { return 0; }
  virtual int GetAttachedEntry() const;
  virtual truth Enter(truth) const;
  virtual int GetWalkability() const;
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__
#define WTERRAIN_PROTO(name, protobase)\
template<> const protobase##prototype\
  name##sysbase::ProtoType((protobase##spawner)(&name##sysbase::Spawn),\
			   #name);
#else
#define WTERRAIN_PROTO(name, protobase)
#endif

#define WTERRAIN(name, base, protobase)\
class name;\
typedef simplesysbase<name, base, protobase##prototype> name##sysbase;\
WTERRAIN_PROTO(name, protobase)\
class name : public name##sysbase

#define GWTERRAIN(name, base) WTERRAIN(name, base, gwterrain)
#define OWTERRAIN(name, base) WTERRAIN(name, base, owterrain)

#endif
