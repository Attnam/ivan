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

#ifndef __SLOT_H__
#define __SLOT_H__

#include "typedef.h"

class item;
class character;
class outputfile;
class inputfile;
class bodypart;
class square;

class slot
{
 public:
  slot() : Item(0) { }
  virtual void Empty() = 0;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  item* GetItem() const { return Item; }
  item* operator->() const { return Item; }
  item* operator*() const { return Item; }
  virtual void AddFriendItem(item*) const = 0;
  virtual truth IsOnGround() const { return false; }
  virtual void PutInItem(item*) = 0;
  virtual square* GetSquareUnder(int = 0) const = 0;
  virtual void SignalVolumeAndWeightChange() = 0;
  virtual void SignalEmitationIncrease(col24) = 0;
  virtual void SignalEmitationDecrease(col24) = 0;
  virtual void DonateTo(item*);
  virtual truth CanBeSeenBy(ccharacter*) const = 0;
  virtual void SignalEnchantmentChange() { }
  virtual truth IsVisible() const = 0;
  virtual truth IsGearSlot() const { return false; }
  virtual ccharacter* FindCarrier() const = 0;
 protected:
  item* Item;
};

class stackslot : public slot
{
 public:
  friend class stack;
  friend class stackiterator;
  stackslot(stack* MotherStack, stackslot* Last) : MotherStack(MotherStack), Last(Last), Next(0) { }
  virtual void Empty();
  virtual void AddFriendItem(item*) const;
  virtual truth IsOnGround() const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(col24);
  virtual void SignalEmitationDecrease(col24);
  virtual void DonateTo(item*);
  virtual truth CanBeSeenBy(ccharacter*) const;
  stack* GetMotherStack() const { return MotherStack; }
  void SetMotherStack(stack* What) { MotherStack = What; }
  virtual truth IsVisible() const;
  virtual void PutInItem(item*);
  virtual void Load(inputfile&);
  virtual ccharacter* FindCarrier() const;
 protected:
  stack* MotherStack;
  stackslot* Last;
  stackslot* Next;
};

class bodypartslot : public slot
{
 public:
  virtual void Empty();
  character* GetMaster() const { return Master; }
  void SetMaster(character* What) { Master = What; }
  virtual void AddFriendItem(item*) const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(col24);
  virtual void SignalEmitationDecrease(col24);
  virtual void PutInItem(item*);
  virtual void Load(inputfile&);
  virtual truth CanBeSeenBy(ccharacter*) const;
  virtual truth IsVisible() const { return false; }
  virtual ccharacter* FindCarrier() const { return Master; }
 protected:
  character* Master;
};

class gearslot : public slot
{
 public:
  virtual void Empty();
  bodypart* GetBodyPart() const { return BodyPart; }
  void SetBodyPart(bodypart* What) { BodyPart = What; }
  virtual void AddFriendItem(item*) const;
  void Init(bodypart*, int);
  int GetEquipmentIndex() const { return EquipmentIndex; }
  void SetEquipmentIndex(int What) { EquipmentIndex = What; }
  virtual void PutInItem(item*);
  virtual square* GetSquareUnder(int = 0) const;
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalEmitationIncrease(col24);
  virtual void SignalEmitationDecrease(col24);
  virtual truth CanBeSeenBy(ccharacter*) const;
  virtual void SignalEnchantmentChange();
  virtual truth IsVisible() const { return false; }
  virtual truth IsGearSlot() const { return true; }
  virtual ccharacter* FindCarrier() const;
 protected:
  bodypart* BodyPart;
  int EquipmentIndex;
};

inline outputfile& operator<<(outputfile& SaveFile, const slot& Slot)
{
  Slot.Save(SaveFile);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, slot& Slot)
{
  Slot.Load(SaveFile);
  return SaveFile;
}

#endif
