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

#ifndef __ACTION_H__
#define __ACTION_H__

#include "typedef.h"
#include "ivandef.h"

class character;
class action;
class outputfile;
class inputfile;

typedef action* (*actionspawner)(character*);

class actionprototype
{
 public:
  actionprototype(actionspawner, cchar*);
  action* SpawnAndLoad(inputfile&) const;
  cchar* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
 private:
  int Index;
  actionspawner Spawner;
  cchar* ClassID;
};

class action
{
 public:
  typedef actionprototype prototype;
  action()  : Actor(0), Flags(0) { }
  virtual ~action() { }
  virtual void Handle() = 0;
  virtual void Terminate(truth);
  character* GetActor() const { return Actor; }
  void SetActor(character* What) { Actor = What; }
  virtual truth IsVoluntary() const { return true; }
  virtual truth AllowUnconsciousness() const { return true; }
  virtual truth AllowFoodConsumption() const { return true; }
  virtual truth TryDisplace() { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsRest() const { return false; }
  virtual const prototype* GetProtoType() const = 0;
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual cchar* GetDescription() const = 0;
  truth InDNDMode() const { return Flags & IN_DND_MODE; }
  void ActivateInDNDMode() { Flags |= IN_DND_MODE; }
  virtual truth ShowEnvironment() const { return true; }
  virtual cchar* GetDeathExplanation() const { return ""; }
  virtual truth CanBeTalkedTo() const { return true; }
  virtual truth IsUnconsciousness() const { return false; }
 protected:
  character* Actor;
  ulong Flags;
};

template <class type, class base>
class actionsysbase : public base
{
 public:
  typedef actionsysbase mybase;
  static type* Spawn(character* Actor)
  {
    type* T = new type;
    T->Actor = Actor;
    return T;
  }
  virtual const actionprototype* GetProtoType() const { return &ProtoType; }
  static const actionprototype ProtoType;
};

#ifdef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__
#define ACTION_PROTO(name)\
template<> const actionprototype\
  name##sysbase::ProtoType((actionspawner)(&name##sysbase::Spawn), #name);
#else
#define ACTION_PROTO(name)
#endif

#define ACTION(name, base)\
class name;\
typedef actionsysbase<name, base> name##sysbase;\
ACTION_PROTO(name)\
class name : public name##sysbase

#endif
