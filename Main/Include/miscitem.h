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

#ifndef __MISCITEM_H__
#define __MISCITEM_H__

#include "item.h"
#include "game.h" /// check
#include "trap.h"

ITEM(materialcontainer, item)
{
 public:
  materialcontainer() = default;
  materialcontainer(const materialcontainer&);
  virtual ~materialcontainer();
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, truth = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual void SignalSpoil(material*);
  virtual truth CanBePiledWith(citem*, ccharacter*) const;
  virtual void Be();
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual int GetAttachedGod() const;
  virtual material* GetConsumeMaterial(ccharacter*, materialpredicate = TrueMaterialPredicate) const;
  virtual material* RemoveMaterial(material*);
  material* RemoveMainMaterial();
  virtual material* RemoveSecondaryMaterial();
  virtual void CalculateEmitation();
  virtual void InitMaterials(const materialscript*, const materialscript*, truth);
  virtual int GetSparkleFlags() const;
  virtual void SignalBurn(material*);
 protected:
  virtual long GetMaterialPrice() const;
  virtual truth CalculateHasBe() const;
  virtual void GenerateMaterials();
  virtual col16 GetMaterialColorB(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual int GetRustDataB() const;
  virtual int GetBurnDataB() const;
  material* SecondaryMaterial;
};

ITEM(banana, materialcontainer)
{
 public:
  banana() : TimesUsed(0), Charges(6), Jammed(false) { }
  virtual truth Zap(character*, v2, int);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual truth IsZappable(ccharacter*) const { return true; }
  virtual truth IsChargeable(ccharacter*) const { return true; }
  virtual void SignalSpoil(material*);
  virtual truth IsBanana() const { return true; }
  virtual material* RemoveSecondaryMaterial();
 protected:
  int TimesUsed;
  int Charges;
  truth Jammed;
};

ITEM(holybanana, banana)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual truth Zap(character*, v2, int);
  virtual void Be() { }
  virtual int GetSpecialFlags() const;
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
  virtual truth ReceiveDamage(character*, int, int, int);
};

ITEM(lantern, item)
{
 public:
  virtual void SignalSquarePositionChange(int);
  virtual truth AllowAlphaEverywhere() const { return true; }
  virtual int GetSpecialFlags() const;
  virtual truth IsLanternOnWall() const { return GetSquarePosition() != CENTER; }
  virtual void Break(character*, int);
 protected:
  virtual int GetClassAnimationFrames() const { return !IsBroken() ? 32 : 1; }
  virtual col16 GetMaterialColorA(int) const;
  virtual col16 GetMaterialColorB(int) const;
  virtual col16 GetMaterialColorC(int) const;
  virtual col16 GetMaterialColorD(int) const;
  virtual alpha GetAlphaA(int) const { return 255; }
  virtual alpha GetAlphaB(int) const;
  virtual alpha GetAlphaC(int) const;
  virtual alpha GetAlphaD(int) const;
  virtual v2 GetBitmapPos(int) const;
};

ITEM(can, materialcontainer)
{
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual truth IsDippable(ccharacter*) const { return !SecondaryMaterial; }
  virtual truth IsDipDestination(ccharacter*) const;
  virtual liquid* CreateDipLiquid(long);
  virtual truth AllowSpoil() const { return false; } // temporary
  virtual truth Spoils() const { return false; } // temporary
  virtual truth HasBetterVersion() const { return !SecondaryMaterial; }
  virtual long DipIntoVolume() const { return GetDefaultSecondaryVolume(); }
 protected:
  virtual void AddPostFix(festring& String, int) const { AddContainerPostFix(String); }
  virtual truth AddAdjective(festring&, truth) const;
  virtual v2 GetBitmapPos(int) const;
};

ITEM(lump, item)
{
 protected:
  void AddLumpyPostFix(festring&) const;
  virtual void AddPostFix(festring& String, int) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(potion, materialcontainer)
{
 public:
  virtual item* BetterVersion() const;
  virtual void DipInto(liquid*, character*);
  virtual liquid* CreateDipLiquid(long);
  virtual truth IsDippable(ccharacter*) const { return !SecondaryMaterial; }
  virtual void Break(character*, int);
  virtual truth IsDipDestination(ccharacter*) const;
  virtual truth IsExplosive() const;
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth HasBetterVersion() const { return !SecondaryMaterial; }
  virtual truth EffectIsGood() const;
  virtual truth IsKamikazeWeapon(ccharacter*) const { return IsExplosive(); }
  virtual long DipIntoVolume() const { return GetDefaultSecondaryVolume(); }
 protected:
  virtual void AddPostFix(festring& String, int) const { AddContainerPostFix(String); }
  virtual truth AddAdjective(festring&, truth) const;
};

ITEM(bananapeels, item)
{
 public:
  virtual item* BetterVersion() const;
  virtual truth HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
  virtual truth IsBananaPeel() const { return true; }
  virtual truth IsDangerous(ccharacter*) const;
  virtual truth RaiseTheDead(character*);
};

ITEM(brokenbottle, item)
{
 public:
  virtual truth IsBroken() const { return true; }
  virtual item* BetterVersion() const;
  virtual truth HasBetterVersion() const { return true; }
  virtual void StepOnEffect(character*);
  virtual item* Fix();
  virtual truth IsDangerous(ccharacter*) const;
};

ITEM(scroll, item)
{
 public:
  virtual truth CanBeRead(character*) const;
  virtual truth IsReadable(ccharacter*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth IsDippable(ccharacter*) const;
  virtual truth AllowFluids() const { return true; }
  virtual void DipInto(liquid*, character*);
};

ITEM(scrollofteleportation, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofcharging, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(nut, item)
{
};

ITEM(leftnutofpetrus, nut)
{
 public:
  virtual void Be() { }
  virtual truth IsPetrussNut() const { return true; }
  virtual truth IsConsumable() const { return false; }
};

ITEM(bone, item)
{
 public:
  virtual truth DogWillCatchAndConsume(ccharacter*) const;
  virtual truth IsABone() const { return true; }
  virtual truth Necromancy(character*);
};

ITEM(loaf, item)
{
 protected:
  virtual void AddPostFix(festring& String, int) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
};

ITEM(scrollofwishing, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(copyofleftnutofpetrus, nut)
{
};

ITEM(wand, item)
{
 public:
  virtual truth Apply(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { TimesUsed = 0; }
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual truth IsZappable(ccharacter*) const { return true; }
  virtual truth IsChargeable(ccharacter*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth Zap(character*, v2, int);
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
  virtual long GetPrice() const;
  virtual truth IsExplosive() const { return true; }
 protected:
  virtual void PostConstruct();
  void BreakEffect(character*, cfestring&);
  ulong GetSpecialParameters() const;
  int Charges;
  int TimesUsed;
};

ITEM(scrollofchangematerial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(avatarofvalpurus, item)
{
 public:
  virtual void Be() { }
  virtual truth IsTheAvatar() const { return true; }
  virtual truth IsConsumable() const { return false; }
};

ITEM(kiwi, item)
{
};

ITEM(pineapple, item)
{
};

ITEM(palmbranch, item)
{
 public:
  virtual truth IsShield(ccharacter*) const { return true; }
};

ITEM(backpack, materialcontainer)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth IsExplosive() const;
  virtual long GetTotalExplosivePower() const;
  virtual void SpillFluid(character*, liquid*, int = 0);
 protected:
  virtual void AddPostFix(festring& String, int) const { AddContainerPostFix(String); }
};

ITEM(holybook, item)
{
 public:
  virtual truth CanBeRead(character*) const;
  virtual truth IsReadable(ccharacter*) const { return true; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual void FinishReading(character*);
  virtual truth IsDippable(ccharacter*) const;
  virtual truth AllowFluids() const { return true; }
  virtual void DipInto(liquid*, character*);
 protected:
  virtual col16 GetMaterialColorA(int) const;
  virtual truth ShowMaterial() const { return false; }
};

ITEM(fiftymillionroubles, item)
{
};

ITEM(oillamp, item)
{
 public:
  oillamp();
  oillamp(const oillamp&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth GetInhabitedByGenie() const { return InhabitedByGenie; }
  virtual void SetInhabitedByGenie(truth What) { InhabitedByGenie = What; }
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
 protected:
  truth InhabitedByGenie;
};

ITEM(stone, item)
{
 public:
  virtual long GetTruePrice() const;
  virtual truth IsLuxuryItem(ccharacter*) const { return GetTruePrice() > 0; }
 protected:
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(scrolloftaming, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(mine, itemtrap<materialcontainer>)
{
 public:
  virtual void StepOnEffect(character*);
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth Apply(character* User);
  virtual truth IsDangerous(ccharacter* Stepper) const { return WillExplode(Stepper); }
  virtual truth WillExplode(ccharacter*) const;
  virtual truth CheckPickUpEffect(character*);
 protected:
  virtual truth AddAdjective(festring&, truth) const;
};

ITEM(key, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual truth CanOpenDoors() const { return true; }
  virtual truth CanOpenLockType(int AnotherLockType) const { return GetConfig() == AnotherLockType; }
};

ITEM(headofelpuri, item) // can't wear equipment, so not "head"
{
 public:
  virtual truth IsHeadOfElpuri() const { return true; }
  virtual truth IsConsumable() const { return false; }
  virtual void Be() { }
};

ITEM(whistle, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual void BlowEffect(character*);
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(magicalwhistle, whistle)
{
 public:
  magicalwhistle() : LastUsed(0) { }
  virtual void BlowEffect(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  ulong LastUsed;
};

ITEM(itemcontainer, lockableitem)
{
 public:
  itemcontainer();
  itemcontainer(const itemcontainer&);
  virtual ~itemcontainer();
  virtual truth Open(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual truth Polymorph(character*, stack*);
  virtual void CalculateVolumeAndWeight();
  virtual truth ContentsCanBeSeenBy(ccharacter*) const;
  virtual long GetTruePrice() const;
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual void DrawContents(ccharacter*);
  virtual truth Apply(character* Applier) { return Open(Applier); }
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual void SetItemsInside(const fearray<contentscript<item>>&, int);
  virtual truth AllowContentEmitation() const { return false; }
  virtual truth IsDestroyable(ccharacter*) const;
  virtual int GetOfferValue(int) const;
  virtual void SortAllItems(const sortdata&) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual material* RemoveMaterial(material*);
  virtual void SetLifeExpectancy(int, int);
  virtual void CalculateEnchantment();
  virtual int GetTeleportPriority() const;
  virtual void SetParameters(int);
  virtual void Disappear();
  virtual stack* GetContained() const { return Contained; }
 protected:
  virtual col16 GetMaterialColorB(int) const;
  virtual void PostConstruct();
  stack* Contained;
};

ITEM(beartrap, itemtrap<item>)
{
 public:
  beartrap();
  beartrap(const beartrap&);
  virtual ~beartrap();
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void StepOnEffect(character*);
  virtual truth CheckPickUpEffect(character*);
  virtual truth IsPickable(character*) const;
  virtual truth Apply(character*);
  virtual v2 GetBitmapPos(int) const;
  virtual truth IsDangerous(ccharacter*) const { return Active; }
  virtual truth ReceiveDamage(character*, int, int, int);
  virtual truth NeedDangerSymbol() const { return IsActive(); }
  virtual void Fly(character*, int, int);
  virtual ulong GetTrapID() const { return TrapData.TrapID; }
  virtual ulong GetVictimID() const { return TrapData.VictimID; }
  virtual void UnStick() { TrapData.VictimID = 0; }
  virtual void UnStick(int I) { TrapData.BodyParts &= ~(1 << I); }
  virtual truth TryToUnStick(character*, v2);
  virtual void RemoveFromSlot();
  virtual int GetTrapType() const { return GetType() | ITEM_TRAP; }
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void DonateSlotTo(item*);
  virtual truth AllowFluids() const { return true; }
 protected:
  virtual truth AddAdjective(festring&, truth) const;
  truth IsStuck() const { return TrapData.VictimID; }
  int GetBaseTrapDamage() const;
  trapdata TrapData;
};

ITEM(stethoscope, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
};

ITEM(scrollofenchantweapon, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofenchantarmor, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(skull, item)
{
 public:
  virtual truth IsASkull() const { return true; }
};

ITEM(skullofxinroch, item)
{
 public:
  virtual truth IsASkull() const { return true; }
  virtual void Be() { }
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(scrollofrepair, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(encryptedscroll, scroll)
{
 public:
  virtual void Be() { }
  virtual truth Read(character*);
  virtual truth ReceiveDamage(character*, int, int, int) { return false; }
  virtual truth IsEncryptedScroll() const { return true; }
};

ITEM(horn, item)
{
 public:
  horn() : LastUsed(0) { }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual void FinalProcessForBone();
 protected:
  ulong LastUsed;
};

ITEM(carrot, item)
{
 public:
  virtual truth BunnyWillCatchAndConsume(ccharacter*) const;
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(charmlyre, item)
{
 public:
  charmlyre();
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void FinalProcessForBone();
 protected:
  virtual col16 GetMaterialColorB(int) const;
  ulong LastUsed;
};

ITEM(scrollofdetectmaterial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(stick, item)
{
 protected:
  virtual truth AddAdjective(festring&, truth) const;
  virtual void AddPostFix(festring& String, int) const { AddLumpyPostFix(String); }
  virtual truth ShowMaterial() const { return false; }
  virtual truth WeightIsIrrelevant() const { return true; }
};

ITEM(scrollofhardenmaterial, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(scrollofgolemcreation, scroll)
{
 public:
  virtual void FinishReading(character*);
};

ITEM(gasgrenade, materialcontainer)
{
 protected:
  virtual void AddPostFix(festring& String, int) const { AddContainerPostFix(String); }
  truth ReceiveDamage(character* Damager, int Damage, int Type, int);
};

ITEM(holyhandgrenade, item)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const { return true; }
  virtual truth CalculateHasBe() const;
  virtual void Explode();
  virtual void Be();
  virtual v2 GetBitmapPos(int) const;
  virtual int GetClassAnimationFrames() const;
  virtual alpha GetOutlineAlpha(int) const;
  virtual col16 GetOutlineColor(int) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void PreProcessForBone();
  virtual void PostConstruct();
  virtual truth AllowAlphaEverywhere() const { return true; }
  virtual col16 GetMaterialColorB(int) const;
  virtual bool WillExplodeSoon() const;
  virtual truth IsKamikazeWeapon(ccharacter*) const { return CalculateHasBe(); }
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
 protected:
  ulong PinPulledTick;
  ulong Count;
  ulong PinPullerID;
};

ITEM(pantheonbook, holybook)
{
 public:
  virtual void FinishReading(character*);
 protected:
  virtual col16 GetMaterialColorA(int) const;
};

ITEM(gorovitscopyoflenin, item)
{
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(firstbornchild, item)
{
 public:
  virtual bool SpecialOfferEffect(int);
  virtual truth AllowSpoil() const { return false; } // temporary
  virtual truth Spoils() const { return false; } // temporary
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

#endif
