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

#ifndef __GEAR_H__
#define __GEAR_H__

#include "item.h"

ITEM(meleeweapon, item)
{
 public:
  meleeweapon() { }
  meleeweapon(const meleeweapon&);
  virtual ~meleeweapon();
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual void DipInto(liquid*, character*);
  virtual long GetPrice() const;
  virtual truth IsDippable(ccharacter*) const;
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, truth = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
  virtual void SignalSpoil(material*);
  virtual void Be();
  virtual truth IsWeapon(ccharacter*) const { return true; }
  virtual int GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(int);
  virtual void EditEnchantment(int);
  virtual int GetStrengthValue() const;
  virtual truth IsFixableBySmith(ccharacter*) const { return IsBroken() || IsRusted(); }
  virtual truth IsFixableByTailor(ccharacter*) const { return IsBroken(); }
  virtual double GetTHVBonus() const;
  virtual double GetDamageBonus() const;
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual void TryToRust(long);
  virtual material* GetConsumeMaterial(ccharacter*, materialpredicate = TrueMaterialPredicate) const;
  virtual pixelpredicate GetFluidPixelAllowedPredicate() const;
  virtual material* RemoveMaterial(material*);
  material* RemoveMainMaterial();
  material* RemoveSecondaryMaterial();
  virtual v2 GetWieldedBitmapPos(int) const;
  virtual void CalculateEmitation();
  virtual void InitMaterials(const materialscript*, const materialscript*, truth);
  virtual item* Fix();
  virtual void CalculateEnchantment();
  virtual truth AllowFluids() const { return true; }
  virtual int GetSparkleFlags() const;
 protected:
  virtual long GetMaterialPrice() const;
  virtual truth CalculateHasBe() const;
  virtual void PostConstruct();
  virtual void AddPostFix(festring&, int) const;
  virtual void GenerateMaterials();
  virtual col16 GetMaterialColorB(int) const;
  virtual col16 GetMaterialColorC(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual int GetRustDataB() const;
  virtual col16 GetDripColor() const;
  virtual truth AllowRegularColors() const;
  material* SecondaryMaterial;
  int Enchantment;
};

ITEM(justifier, meleeweapon)
{
 public:
  virtual truth AllowAlphaEverywhere() const { return true; }
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(neercseulb, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual truth AllowAlphaEverywhere() const { return true; }
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(pickaxe, meleeweapon)
{
 public:
  virtual truth Apply(character*);
  virtual truth IsAppliable(ccharacter*) const;
};

ITEM(whip, meleeweapon)
{
 public:
  virtual truth IsWhip() const { return true; }
 protected:
  virtual int GetFormModifier() const;
};

ITEM(flamingsword, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual int GetSpecialFlags() const;
};

ITEM(mjolak, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
};

ITEM(vermis, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
};

ITEM(turox, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
};

ITEM(whipofthievery, whip)
{
 public:
  virtual long GetPrice() const;
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
 protected:
  virtual truth CleptiaHelps(ccharacter*, ccharacter*) const;
};

ITEM(gorovitsweapon, meleeweapon)
{
 public:
  virtual truth IsGorovitsFamilyRelic() const { return true; }
  virtual truth AllowAlphaEverywhere() const { return true; }
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(thunderhammer, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual int GetSpecialFlags() const;
  virtual truth ReceiveDamage(character*, int, int, int);
};

ITEM(saalthul, meleeweapon)
{
};

ITEM(armor, item)
{
 public:
  virtual long GetPrice() const;
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsArmor(ccharacter*) const { return true; }
  virtual int GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(int);
  virtual void EditEnchantment(int);
  virtual int GetStrengthValue() const;
  virtual truth CanBePiledWith(citem*, ccharacter*) const;
  virtual int GetInElasticityPenalty(int) const;
  virtual int GetCarryingBonus() const;
  virtual truth IsFixableBySmith(ccharacter*) const { return IsBroken() || IsRusted(); }
  virtual truth IsFixableByTailor(ccharacter*) const { return IsBroken(); }
  virtual truth AllowFluids() const { return true; }
  virtual void CalculateEnchantment();
  virtual double GetTHVBonus() const;
  virtual double GetDamageBonus() const;
 protected:
  virtual void AddPostFix(festring&, int) const;
  virtual void PostConstruct();
  int Enchantment;
};

ITEM(bodyarmor, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsBodyArmor(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
 protected:
  virtual cchar* GetBreakVerb() const;
  virtual truth AddAdjective(festring&, truth) const;
  virtual cfestring& GetNameSingular() const;
};

ITEM(goldeneagleshirt, bodyarmor)
{
 public:
  virtual truth IsGoldenEagleShirt() const { return true; }
  virtual truth IsConsumable() const { return false; }
  virtual truth AllowAlphaEverywhere() const { return true; }
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(shield, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsShield(ccharacter*) const { return true; }
  virtual void AddInventoryEntry(ccharacter*, festring&, int, truth) const;
};

ITEM(cloak, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsCloak(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
  virtual truth ReceiveDamage(character*, int, int, int);
 protected:
  virtual int GetSpecialFlags() const;
  virtual cchar* GetBreakVerb() const;
  virtual truth AddAdjective(festring&, truth) const;
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(boot, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsBoot(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
};

ITEM(gauntlet, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsGauntlet(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
};

ITEM(belt, armor)
{
 public:
  virtual long GetPrice() const;
  virtual truth IsBelt(ccharacter*) const { return true; }
  virtual int GetFormModifier() const;
  virtual truth IsInCorrectSlot(int) const;
  virtual col16 GetMaterialColorB(int Frame) const { return GetMaterialColorA(Frame); }
};

typedef lockable<belt, itemlock> lockablebelt;

ITEM(ring, item)
{
 public:
  virtual truth IsRing(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
  virtual truth IsLuxuryItem(ccharacter*) const { return true; }
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(amulet, item)
{
 public:
  virtual truth IsAmulet(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
  virtual truth IsLuxuryItem(ccharacter*) const { return true; }
 protected:
  virtual col16 GetMaterialColorB(int) const;
};

ITEM(helmet, armor)
{
 public:
  virtual truth IsGorovitsFamilyRelic() const;
  virtual long GetPrice() const;
  virtual truth IsHelmet(ccharacter*) const { return true; }
  virtual truth IsInCorrectSlot(int) const;
 protected:
  virtual col16 GetMaterialColorB(int) const;
  virtual col16 GetMaterialColorC(int) const;
};

ITEM(chameleonwhip, whip)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
 protected:
  virtual truth ScabiesHelps(ccharacter*, ccharacter*) const;
};

ITEM(wondersmellstaff, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
  virtual truth AllowAlphaEverywhere() const { return true; }
  virtual void Break(character*, int);
 protected:
  virtual int GetClassAnimationFrames() const;
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(decosadshirt, bodyarmor)
{
 public:
  decosadshirt();
  virtual void Be();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  ulong GetEquippedTicks() { return EquippedTicks; }
  void SetEquippedTicks(ulong What) { EquippedTicks = What; }
  virtual truth IsDecosAdShirt(ccharacter*) const { return true; }
 protected:
  virtual truth CalculateHasBe() const { return true; }
  ulong EquippedTicks;
};

ITEM(weepblade, meleeweapon)
{
 public:
  virtual truth HitEffect(character*, character*, v2, int, int, truth);
};

ITEM(daggerofvenom, meleeweapon)
{
 public:
  daggerofvenom() { Enable(); }
  virtual void Be();
protected:
  virtual truth CalculateHasBe() const { return true; }
};


ITEM(acidshield, shield)
{
 public:
  virtual void BlockEffect(character*, character*, item*, int Type);
}; 

ITEM(chastitybelt, lockablebelt)
{
 public:
  virtual int GetFormModifier() const { return item::GetFormModifier(); }
};

#endif
