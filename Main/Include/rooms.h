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

#ifndef __ROOMS_H__
#define __ROOMS_H__

#include "room.h"

ROOM(normalroom, room) { };

ROOM(shop, room)
{
 public:
  virtual void Enter(character*);
  virtual truth PickupItem(character*, item*, int);
  virtual truth DropItem(character*, item*, int);
  virtual void KickSquare(character*, lsquare*);
  virtual truth ConsumeItem(character*, item*, int);
  virtual truth AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual truth AllowSpoil(citem*) const;
  virtual truth AllowKick(ccharacter*,const lsquare*) const;
  virtual void HostileAction(character*) const;
  virtual truth AllowFoodSearch() const { return false; }
  virtual void ReceiveVomit(character*);
};

ROOM(cathedral, room)
{
 public:
  cathedral();
  virtual void Enter(character*);
  virtual truth PickupItem(character*, item*, int);
  virtual truth DropItem(character*, item*, int);
  virtual void KickSquare(character*, lsquare*);
  virtual truth ConsumeItem(character*, item*, int);
  virtual void SetEntered(truth What) { Entered = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth AllowDropGifts() const { return false; }
  virtual truth Drink(character*) const;
  virtual truth HasDrinkHandler() const { return true; }
  virtual truth Dip(character*) const;
  virtual truth HasDipHandler() const { return true; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual truth AllowSpoil(citem*) const { return false; }
  virtual int GetGodRelationAdjustment() const { return -150; }
  virtual truth AllowKick(ccharacter*,const lsquare*) const;
  virtual void HostileAction(character*) const;
  virtual truth AllowAltarPolymorph() const { return false; }
  virtual truth AllowFoodSearch() const { return false; }
  virtual void AddItemEffect(item*);
  character* FindRandomExplosiveReceiver() const;
 protected:
  truth Entered;
};

ROOM(library, room)
{
 public:
  virtual void Enter(character*);
  virtual truth PickupItem(character*, item*, int);
  virtual truth DropItem(character*, item*, int);
  virtual void KickSquare(character*, lsquare*);
  virtual truth ConsumeItem(character*, item*, int);
  virtual truth AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual truth AllowKick(ccharacter*, const lsquare*) const;
  virtual void HostileAction(character*) const;
};

ROOM(bananadroparea, room)
{
 public:
  virtual truth PickupItem(character*, item*, int);
  virtual truth DropItem(character*, item*, int);
  virtual void KickSquare(character*, lsquare*);
  virtual truth ConsumeItem(character*, item*, int);
  virtual truth AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual truth AllowKick(ccharacter*, const lsquare*) const;
  virtual void HostileAction(character*) const;
};

ROOM(sumoarena, room)
{
 public:
  virtual void DestroyTerrain(character*);
  virtual void HostileAction(character*) const;
  virtual truth CheckDestroyTerrain(character*);
};

#endif
