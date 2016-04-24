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

class gwterrain;
class owterrain;
class gwterrainprototype;
class owterrainprototype;
template <class type> class databasecreator;
struct blitdata;

typedef gwterrain* (*gwterrainspawner)(int, int);
typedef owterrain* (*owterrainspawner)(int, int);

struct wterraindatabase : public databasebase
{
  void PostProcess() { }
  truth IsAbstract;
  v2 BitmapPos;
};

class wterrain
{
 public:
  wterrain() : WSquareUnder(0), AnimationFrames(1) { }
  virtual ~wterrain() = default;
  virtual void Save(outputfile&) const;
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
  void Initialize(int, int);
  //virtual void PostConstruct() { } //?
  virtual void InstallDataBase(int) = 0;
};

struct gwterraindatabase : public wterraindatabase
{
  typedef gwterrainprototype prototype;
  void InitDefaults(const prototype*, int);
  const prototype* ProtoType;
};

class gwterrainprototype
{
 public:
  friend class databasecreator<gwterrain>;
  gwterrainprototype(const gwterrainprototype*, gwterrainspawner, cchar*);
  gwterrain* Spawn(int Config = 0, int SpecialFlags = 0) const { return Spawner(Config, SpecialFlags); }
  gwterrain* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const gwterrainprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(gwterraindatabase**, int Configs, int) { return Configs; }
  const gwterraindatabase* ChooseBaseForConfig(gwterraindatabase** TempConfig, int, int) { return *TempConfig; }
  const gwterraindatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  const gwterrainprototype* Base;
  gwterraindatabase** ConfigData;
  gwterraindatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  gwterrainspawner Spawner;
  cchar* ClassID;
};

class gwterrain : public wterrain, public gterrain
{
 public:
  friend class databasecreator<gwterrain>;
  typedef gwterrainprototype prototype;
  typedef gwterraindatabase database;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  int GetType() const { return GetProtoType()->GetIndex(); }
  const database* GetDataBase() const { return DataBase; }
  DATA_BASE_VALUE(const prototype*, ProtoType);
  DATA_BASE_VALUE(int, Config);
  void Draw(blitdata&) const;
  virtual int GetPriority() const = 0;
  virtual int GetEntryDifficulty() const { return 10; }
  void CalculateNeighbourBitmapPoses();
  virtual int GetWalkability() const;
 protected:
  std::pair<v2, int> Neighbour[8];
  virtual void InstallDataBase(int);
  virtual const prototype* FindProtoType() const { return &ProtoType; }
  virtual v2 GetBitmapPos(int) const;
  static const prototype ProtoType;
  const database* DataBase;
};

struct owterraindatabase : public wterraindatabase
{
  typedef owterrainprototype prototype;
  void InitDefaults(const prototype*, int);
  const prototype* ProtoType;
//  int GWTerrainType;
//  int WalkabilityType;
//  festring LocationDescription;
};

class owterrainprototype
{
 public:
  friend class databasecreator<owterrain>;
  owterrainprototype(const owterrainprototype*, owterrainspawner, cchar*);
  owterrain* Spawn(int Config = 0, int SpecialFlags = 0) const { return Spawner(Config, SpecialFlags); }
  owterrain* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const owterrainprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(owterraindatabase**, int Configs, int) { return Configs; }
  const owterraindatabase* ChooseBaseForConfig(owterraindatabase** TempConfig, int, int) { return *TempConfig; }
  const owterraindatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  const owterrainprototype* Base;
  owterraindatabase** ConfigData;
  owterraindatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  owterrainspawner Spawner;
  cchar* ClassID;
};

class owterrain : public wterrain, public oterrain
{
 public:
  friend class databasecreator<owterrain>;
  typedef owterrainprototype prototype;
  typedef owterraindatabase database;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void Draw(blitdata&) const;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual int GetAttachedDungeon() const { return 0; }
  virtual int GetAttachedArea() const { return 0; }
  virtual int GetAttachedEntry() const;
  virtual truth Enter(truth) const;
  virtual int GetWalkability() const;
  const database* GetDataBase() const { return DataBase; }
  DATA_BASE_VALUE(const prototype*, ProtoType);
  DATA_BASE_VALUE(int, Config);
 protected:
  virtual v2 GetBitmapPos(int) const;
  virtual void InstallDataBase(int);
  virtual const prototype* FindProtoType() const { return &ProtoType; }
  static const prototype ProtoType;
  const database* DataBase;
};

#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__
#define WTERRAIN_PROTO(name, base, protobase)\
template<> const protobase##prototype\
  name##sysbase::ProtoType(&base::ProtoType,\
                           reinterpret_cast<protobase##spawner>(&name##sysbase::Spawn),\
                           #name);
#else
#define WTERRAIN_PROTO(name, base, protobase)
#endif

#define WTERRAIN(name, base, protobase)\
class name;\
typedef sysbase<name, base, protobase##prototype> name##sysbase;\
WTERRAIN_PROTO(name, base, protobase)\
class name : public name##sysbase

#define GWTERRAIN(name, base) WTERRAIN(name, base, gwterrain)
#define OWTERRAIN(name, base) WTERRAIN(name, base, owterrain)

#endif
/*
 //OLD
#ifdef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__
#define WTERRAIN_PROTO(name, protobase)\
template<> const protobase##prototype\
  name##sysbase::ProtoType(reinterpret_cast<protobase##spawner>(&name##sysbase::Spawn),\
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
*/