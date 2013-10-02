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

#ifndef __GOD_H__
#define __GOD_H__

#include "ivandef.h"

class outputfile;
class inputfile;
class god;
class liquid;
class team;
struct materialdatabase;

typedef god* (*godspawner)();

class godprototype
{
 public:
  godprototype(godspawner, cchar*);
  god* Spawn() const { return Spawner(); }
  god* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  godspawner Spawner;
  cchar* ClassID;
};

class god
{
 public:
  typedef godprototype prototype;
  god();
  virtual void Pray();
  virtual cchar* GetName() const = 0;
  virtual cchar* GetDescription() const = 0;
  cchar* GetPersonalPronoun() const;
  cchar* GetObjectPronoun() const;
  virtual int GetAlignment() const = 0;
  festring GetCompleteDescription() const;
  void ApplyDivineTick();
  void AdjustRelation(god*, int, truth);
  void AdjustRelation(int);
  void AdjustTimer(long);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetRelation(int Value) { Relation = Value; }
  void SetTimer(long Value) { Timer = Value; }
  truth ReceiveOffer(item*);
  virtual int GetBasicAlignment() const;
  int GetRelation() const { return Relation; }
  void PrintRelation() const;
  void SetIsKnown(truth What) { Known = What; }
  truth IsKnown() const { return Known; }
  void PlayerKickedAltar() { AdjustRelation(-100); }
  void PlayerKickedFriendsAltar() { AdjustRelation(-50); }
  virtual truth PlayerVomitedOnAltar(liquid*);
  character* CreateAngel(team*, int = 0);
  virtual col16 GetColor() const = 0;
  virtual col16 GetEliteColor() const = 0;
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual truth ForceGiveBodyPart() const { return false; }
  virtual truth HealRegeneratingBodyParts() const { return false; }
  virtual truth LikesMaterial(const materialdatabase*, ccharacter*) const;
  truth TryToAttachBodyPart(character*);
  truth TryToHardenBodyPart(character*);
  virtual truth MutatesBodyParts() const { return false; }
  virtual int GetSex() const = 0;
  void SignalRandomAltarGeneration(const std::vector<v2>&);
  virtual truth LikesVomit() const { return false; }
 protected:
  virtual void PrayGoodEffect() = 0;
  virtual void PrayBadEffect() = 0;
  int Relation, LastPray;
  long Timer;
  truth Known;
};

#ifdef __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__
#define GOD_PROTO(name)\
template<> const godprototype\
  name##sysbase::ProtoType((godspawner)(&name##sysbase::Spawn), #name);
#else
#define GOD_PROTO(name)
#endif

#define GOD(name, base)\
class name;\
typedef simplesysbase<name, base, godprototype> name##sysbase;\
GOD_PROTO(name)\
class name : public name##sysbase

#endif
