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

#ifndef __BODYPART_H__
#define __BODYPART_H__

#include "item.h"
#include "save.h"

class humanoid;
class sweaponskill;

struct scar 
{
  int Severity;
  mutable bitmap* PanelBitmap;
};

outputfile& operator<<(outputfile&, const scar&);
inputfile& operator>>(inputfile&, scar&);

struct damageid
{
  int SrcID, Amount;
};

RAW_SAVE_LOAD(damageid);

ITEM(bodypart, item)
{
 public:
  friend class corpse;
  bodypart() : Master(0) { }
  bodypart(const bodypart&);
  virtual ~bodypart();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetGraphicsContainerIndex() const;
  character* GetMaster() const { return Master; }
  humanoid* GetHumanoidMaster() const;
  void SetMaster(character* What) { Master = What; }
  virtual int GetStrengthValue() const;
  int GetMaxHP() const { return MaxHP; }
  void SetHP(int);
  int GetHP() const { return HP; }
  void EditHP(int, int);
  void IncreaseHP();
  virtual int GetTotalResistance(int) const { return 0; }
  virtual truth ReceiveDamage(character*, int, int, int);
  cfestring& GetOwnerDescription() const { return OwnerDescription; }
  void SetOwnerDescription(cfestring& What) { OwnerDescription = What; }
  truth IsUnique() const { return Flags & UNIQUE; }
  void SetIsUnique(truth);
  virtual void DropEquipment(stack* = 0) { }
  virtual void InitSpecialAttributes() { }
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*, citem*);
  virtual void Mutate();
  long GetBodyPartVolume() const { return BodyPartVolume; }
  long GetCarriedWeight() const { return CarriedWeight; }
  virtual item* GetEquipment(int) const { return 0; }
  virtual int GetEquipments() const { return 0; }
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  void CalculateMaxHP(ulong = MAY_CHANGE_HPS|CHECK_USABILITY);
  virtual void SignalVolumeAndWeightChange();
  void FastRestoreHP();
  void RestoreHP();
  virtual void CalculateDamage() { }
  virtual void CalculateToHitValue() { }
  virtual void CalculateAPCost() { }
  void CalculateAttackInfo();
  double GetTimeToDie(int, double, double, truth, truth) const;
  virtual double GetRoughChanceToHit(double, double) const;
  cfestring& GetBodyPartName() const { return GetNameSingular(); }
  void RandomizePosition();
  void ResetPosition() { SpecialFlags &= ~0x7; }
  virtual void SignalSpoil(material*);
  virtual truth CanBePiledWith(citem*, ccharacter*) const;
  truth IsAlive() const;
  void SpillBlood(int);
  void SpillBlood(int, v2);
  virtual void Be();
  int GetConditionColorIndex() const;
  void SetBitmapPos(v2 What) { BitmapPos = What; }
  void SetSpecialFlags(int What) { SpecialFlags = What; }
  void SetWobbleData(int What) { WobbleData = What; }
  void SetMaterialColorB(col16 What) { ColorB = What; }
  void SetMaterialColorC(col16 What) { ColorC = What; }
  void SetMaterialColorD(col16 What) { ColorD = What; }
  virtual void SignalEnchantmentChange();
  virtual void CalculateAttributeBonuses() { }
  virtual void SignalSpoilLevelChange(material*);
  virtual truth CanBeEatenByAI(ccharacter*) const;
  virtual truth DamageArmor(character*, int, int) { return false; }
  truth CanBeSevered(int) const;
  virtual truth EditAllAttributes(int) { return false; }
  virtual void Draw(blitdata&) const;
  void SetSparkleFlags(int);
  virtual int GetSpecialFlags() const;
  virtual truth IsRepairable(ccharacter*) const;
  truth IsWarm() const;
  truth UseMaterialAttributes() const;
  truth CanRegenerate() const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual lsquare* GetLSquareUnder(int = 0) const;
  virtual item* GetArmorToReceiveFluid(truth) const { return 0; }
  virtual void SpillFluid(character*, liquid*, int = 0);
  void StayOn(liquid*);
  void SetBloodMaterial(int What) { BloodMaterial = What; }
  int GetBloodMaterial() const { return BloodMaterial; }
  liquid* CreateBlood(long) const;
  virtual truth UpdateArmorPictures() { return false; }
  virtual void DrawArmor(blitdata&) const { }
  virtual void UpdatePictures();
  item* GetExternalBodyArmor() const;
  item* GetExternalCloak() const;
  virtual void ReceiveAcid(material*, cfestring&, long);
  virtual truth ShowFluids() const { return false; }
  virtual void TryToRust(long);
  virtual truth AllowFluidBe() const;
  virtual material* RemoveMaterial(material*);
  virtual void CopyAttributes(const bodypart*) { }
  virtual void DestroyBodyPart(stack*);
  virtual void SetLifeExpectancy(int, int);
  virtual void SpecialEatEffect(character*, int);
  virtual character* GetBodyPartMaster() const { return Master; }
  virtual truth AllowFluids() const { return true; }
  truth IsBadlyHurt() const { return Flags & BADLY_HURT; }
  truth IsStuck() const { return Flags & STUCK; }
  truth IsUsable() const { return !(Flags & (BADLY_HURT|STUCK)); }
  virtual void SignalPossibleUsabilityChange() { UpdateFlags(); }
  void SetIsInfectedByLeprosy(truth);
  virtual int GetSparkleFlags() const;
  virtual truth MaterialIsChangeable(ccharacter*) const;
  virtual void RemoveRust();
  virtual item* Fix();
  virtual long GetFixPrice() const;
  virtual truth IsFixableBySmith(ccharacter*) const;
  virtual truth IsFixableByTailor(ccharacter*) const;
  virtual void SignalMaterialChange();
  void SetNormalMaterial(int What) { NormalMaterial = What; }
  virtual truth IsBroken() const { return HP < MaxHP; }
  virtual truth IsDestroyable(ccharacter*) const;
  void DrawScars(cblitdata&) const;
  static truth DamageTypeCanScar(int);
  void GenerateScar(int, int);
  int CalculateScarAttributePenalty(int) const;
 protected:
  virtual alpha GetMaxAlpha() const;
  virtual void GenerateMaterials() { }
  virtual void AddPostFix(festring&, int) const;
  virtual truth ShowMaterial() const;
  virtual int GetArticleMode() const;
  virtual col16 GetMaterialColorA(int) const;
  virtual col16 GetMaterialColorB(int) const { return ColorB; }
  virtual col16 GetMaterialColorC(int) const { return ColorC; }
  virtual col16 GetMaterialColorD(int) const { return ColorD; }
  virtual v2 GetBitmapPos(int) const { return BitmapPos; }
  virtual int GetWobbleData() const { return WobbleData; }
  void UpdateArmorPicture(graphicdata&, item*, int, v2 (item::*)(int) const, truth = false) const;
  void DrawEquipment(const graphicdata&, blitdata&) const;
  void UpdateFlags();
  truth MasterIsAnimated() const;
  void SignalAnimationStateChange(truth);
  virtual truth AddAdjective(festring&, truth) const;
  void RemoveDamageIDs(int);
  void AddDamageID(int, int);
  festring OwnerDescription;
  character* Master;
  long CarriedWeight;
  long BodyPartVolume;
  packv2 BitmapPos;
  packcol16 ColorB;
  packcol16 ColorC;
  packcol16 ColorD;
  ushort SpecialFlags;
  short HP;
  short MaxHP;
  short BloodMaterial;
  short NormalMaterial;
  uchar SpillBloodCounter;
  uchar WobbleData;
  std::vector<scar> Scar;
  std::deque<damageid> DamageID;
};

ITEM(head, bodypart)
{
 public:
  head();
  head(const head&);
  virtual ~head();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetHelmet(item* What) { HelmetSlot.PutInItem(What); }
  item* GetHelmet() const { return *HelmetSlot; }
  void SetAmulet(item* What) { AmuletSlot.PutInItem(What); }
  item* GetAmulet() const { return *AmuletSlot; }
  virtual void DropEquipment(stack* = 0);
  virtual int GetBodyPartIndex() const;
  double GetBiteDamage() const { return BiteDamage; }
  int GetBiteMinDamage() const;
  int GetBiteMaxDamage() const;
  double GetBiteToHitValue() const { return BiteToHitValue; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void InitSpecialAttributes();
  virtual item* GetEquipment(int) const;
  virtual int GetEquipments() const { return 2; }
  int GetBaseBiteStrength() const { return BaseBiteStrength; }
  void SetBaseBiteStrength(long What) { BaseBiteStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  virtual truth DamageArmor(character*, int, int);
  virtual head* Behead();
  virtual item* GetArmorToReceiveFluid(truth) const;
  virtual void SignalPossibleUsabilityChange();
 protected:
  void UpdateHeadArmorPictures(graphicdata&) const;
  gearslot HelmetSlot;
  gearslot AmuletSlot;
  int BaseBiteStrength;
  double BiteToHitValue;
  double BiteDamage;
  long BiteAPCost;
};

ITEM(torso, bodypart)
{
 public:
  virtual int GetBodyPartIndex() const;
  virtual double GetRoughChanceToHit(double, double) const;
};

ITEM(normaltorso, torso)
{
 public:
  virtual int GetGraphicsContainerIndex() const;
  virtual int GetTotalResistance(int) const;
};

ITEM(humanoidtorso, torso)
{
 public:
  humanoidtorso();
  humanoidtorso(const humanoidtorso&);
  virtual ~humanoidtorso();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetBodyArmor(item* What) { BodyArmorSlot.PutInItem(What); }
  item* GetBodyArmor() const { return *BodyArmorSlot; }
  void SetCloak(item* What) { CloakSlot.PutInItem(What); }
  item* GetCloak() const { return *CloakSlot; }
  void SetBelt(item* What) { BeltSlot.PutInItem(What); }
  item* GetBelt() const { return *BeltSlot; }
  virtual void DropEquipment(stack* = 0);
  virtual item* GetEquipment(int) const;
  virtual int GetEquipments() const { return 3; }
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalVolumeAndWeightChange();
  virtual truth DamageArmor(character*, int, int);
  virtual item* GetArmorToReceiveFluid(truth) const;
 protected:
  void UpdateTorsoArmorPictures(graphicdata&, graphicdata&, graphicdata&) const;
  gearslot BodyArmorSlot;
  gearslot CloakSlot;
  gearslot BeltSlot;
};

ITEM(arm, bodypart)
{
 public:
  arm() : StrengthBonus(0), DexterityBonus(0) { }
  arm(const arm&);
  virtual ~arm();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  double GetWieldedDamage() const;
  double GetWieldedToHitValue() const;
  void SetWielded(item* What) { WieldedSlot.PutInItem(What); }
  item* GetWielded() const { return *WieldedSlot; }
  void SetGauntlet(item* What) { GauntletSlot.PutInItem(What); }
  item* GetGauntlet() const { return *GauntletSlot; }
  void SetRing(item* What) { RingSlot.PutInItem(What); }
  item* GetRing() const { return *RingSlot; }
  virtual void DropEquipment(stack* = 0);
  double GetUnarmedToHitValue() const;
  double GetUnarmedDamage() const;
  void Hit(character*, v2, int, int = 0);
  int GetAttribute(int, truth = true) const;
  truth EditAttribute(int, int);
  void EditExperience(int, double, double);
  void SetStrength(int What) { StrengthExperience = What * EXP_MULTIPLIER; }
  void SetDexterity(int What) { DexterityExperience = What * EXP_MULTIPLIER; }
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  virtual arm* GetPairArm() const { return 0; }
  long GetWieldedAPCost() const;
  long GetUnarmedAPCost() const;
  virtual item* GetEquipment(int) const;
  virtual int GetEquipments() const { return 3; }
  int GetBaseUnarmedStrength() const { return BaseUnarmedStrength; }
  void SetBaseUnarmedStrength(long What) { BaseUnarmedStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  double GetDamage() const { return Damage; }
  int GetMinDamage() const;
  int GetMaxDamage() const;
  double GetToHitValue() const { return ToHitValue; }
  long GetAPCost() const { return APCost; }
  truth PairArmAllowsMelee() const;
  virtual void SignalVolumeAndWeightChange();
  truth TwoHandWieldIsActive() const;
  double GetBlockChance(double) const;
  int GetBlockCapability() const;
  void WieldedSkillHit(int);
  double GetBlockValue() const;
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  int GetWieldedHitStrength() const;
  virtual void SignalEquipmentAdd(gearslot*);
  virtual void SignalEquipmentRemoval(gearslot*, citem*);
  void ApplyDexterityPenalty(item*);
  virtual truth DamageArmor(character*, int, int);
  truth CheckIfWeaponTooHeavy(cchar*) const;
  virtual truth EditAllAttributes(int);
  void AddAttackInfo(felist&) const;
  void AddDefenceInfo(felist&) const;
  void UpdateWieldedPicture();
  void DrawWielded(blitdata&) const;
  virtual truth IsRightArm() const { return 0; }
  virtual void UpdatePictures();
  virtual double GetTypeDamage(ccharacter*) const;
  virtual item* GetArmorToReceiveFluid(truth) const;
  virtual void CopyAttributes(const bodypart*);
  double GetStrengthExperience() const { return StrengthExperience; }
  double GetDexterityExperience() const { return DexterityExperience; }
  virtual void SignalPossibleUsabilityChange();
  virtual truth IsAnimated() const;
  truth HasSadistWeapon() const;
 protected:
  virtual sweaponskill** GetCurrentSWeaponSkill() const { return 0; }
  void UpdateArmArmorPictures(graphicdata&, graphicdata&, int) const;
  int GetCurrentSWeaponSkillBonus() const;
  gearslot WieldedSlot;
  gearslot GauntletSlot;
  gearslot RingSlot;
  double StrengthExperience;
  double DexterityExperience;
  int BaseUnarmedStrength;
  double Damage;
  double ToHitValue;
  long APCost;
  int StrengthBonus;
  int DexterityBonus;
  graphicdata WieldedGraphicData;
};

ITEM(rightarm, arm)
{
 public:
  rightarm();
  rightarm(const rightarm&);
  virtual int GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual truth IsRightArm() const { return true; }
  virtual int GetSpecialFlags() const;
 protected:
  virtual sweaponskill** GetCurrentSWeaponSkill() const;
};

ITEM(leftarm, arm)
{
 public:
  leftarm();
  leftarm(const leftarm&);
  virtual int GetBodyPartIndex() const;
  virtual arm* GetPairArm() const;
  virtual truth IsRightArm() const { return false; }
  virtual int GetSpecialFlags() const;
 protected:
  virtual sweaponskill** GetCurrentSWeaponSkill() const;
};

ITEM(groin, bodypart)
{
 public:
  virtual int GetTotalResistance(int) const;
  virtual int GetBodyPartIndex() const;
  virtual truth DamageArmor(character*, int, int);
  virtual int GetSpecialFlags() const;
  virtual item* GetArmorToReceiveFluid(truth) const;
  void UpdateGroinArmorPictures(graphicdata&) const;
};

ITEM(leg, bodypart)
{
 public:
  leg() : StrengthBonus(0), AgilityBonus(0) { }
  leg(const leg&);
  virtual ~leg();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetTotalResistance(int) const;
  void SetBoot(item* What) { BootSlot.PutInItem(What); }
  item* GetBoot() const { return *BootSlot; }
  virtual void DropEquipment(stack* = 0);
  double GetKickToHitValue() const { return KickToHitValue; }
  double GetKickDamage() const { return KickDamage; }
  int GetKickMinDamage() const;
  int GetKickMaxDamage() const;
  int GetAttribute(int, truth = true) const;
  truth EditAttribute(int, int);
  void EditExperience(int, double, double);
  virtual void InitSpecialAttributes();
  virtual void Mutate();
  long GetKickAPCost() const { return KickAPCost; }
  virtual item* GetEquipment(int) const;
  virtual int GetEquipments() const { return 1; }
  long GetBaseKickStrength() const { return BaseKickStrength; }
  void SetBaseKickStrength(long What) { BaseKickStrength = What; }
  virtual void CalculateDamage();
  virtual void CalculateToHitValue();
  virtual void CalculateAPCost();
  void ApplyEquipmentAttributeBonuses(item*);
  virtual void CalculateAttributeBonuses();
  virtual void SignalEquipmentAdd(gearslot*);
  void ApplyAgilityPenalty(item*);
  virtual void SignalVolumeAndWeightChange();
  virtual truth DamageArmor(character*, int, int);
  virtual truth EditAllAttributes(int);
  void AddAttackInfo(felist&) const;
  virtual item* GetArmorToReceiveFluid(truth) const;
  virtual void CopyAttributes(const bodypart*);
  double GetStrengthExperience() const { return StrengthExperience; }
  double GetAgilityExperience() const { return AgilityExperience; }
  virtual void SignalPossibleUsabilityChange();
 protected:
  void UpdateLegArmorPictures(graphicdata&, graphicdata&, int) const;
  gearslot BootSlot;
  double StrengthExperience;
  double AgilityExperience;
  int BaseKickStrength;
  double KickDamage;
  double KickToHitValue;
  long KickAPCost;
  int StrengthBonus;
  int AgilityBonus;
};

ITEM(rightleg, leg)
{
 public:
  rightleg();
  rightleg(const rightleg&);
  virtual int GetBodyPartIndex() const;
  virtual int GetSpecialFlags() const;
 protected:
};

ITEM(leftleg, leg)
{
 public:
  leftleg();
  leftleg(const leftleg&);
  virtual int GetBodyPartIndex() const;
  virtual int GetSpecialFlags() const;
 protected:
};

ITEM(corpse, item)
{
 public:
  corpse() { }
  corpse(const corpse&);
  virtual ~corpse();
  virtual int GetOfferValue(int) const;
  virtual double GetWeaponStrength() const;
  virtual truth CanBeEatenByAI(ccharacter*) const;
  virtual int GetStrengthValue() const;
  character* GetDeceased() const { return Deceased; }
  void SetDeceased(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsDestroyable(ccharacter*) const;
  virtual long GetTruePrice() const;
  virtual int GetMaterials() const { return 2; }
  virtual truth RaiseTheDead(character*);
  virtual void CalculateVolumeAndWeight();
  virtual void CalculateEmitation();
  virtual void SignalSpoil(material*);
  virtual truth CanBePiledWith(citem*, ccharacter*) const;
  virtual int GetSpoilLevel() const;
  virtual material* GetMaterial(int) const;
  virtual head* Behead();
  virtual truth CanBeCloned() const;
  virtual int GetAttachedGod() const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual truth SuckSoul(character*, character* = 0);
  virtual character* TryNecromancy(character*);
  virtual void Cannibalize();
  virtual material* GetConsumeMaterial(ccharacter*, materialpredicate) const;
  virtual truth DetectMaterial(cmaterial*) const;
  virtual void SetLifeExpectancy(int, int);
  virtual void Be();
  virtual void SignalDisappearance();
  virtual truth IsValuable() const;
  virtual truth AddRustLevelDescription(festring&, truth) const { return false; }
  virtual truth Necromancy(character*);
  virtual int GetSparkleFlags() const;
  virtual truth IsRusted() const { return false; }
  virtual truth CanBeHardened(ccharacter*) const { return false; }
 protected:
  virtual void GenerateMaterials() { }
  virtual col16 GetMaterialColorA(int) const;
  virtual col16 GetMaterialColorB(int) const;
  virtual alpha GetAlphaA(int) const;
  virtual alpha GetAlphaB(int) const;
  virtual truth ShowMaterial() const { return false; }
  virtual void AddPostFix(festring&, int) const;
  virtual v2 GetBitmapPos(int) const;
  virtual int GetSize() const;
  virtual int GetArticleMode() const;
  virtual int GetRustDataA() const;
  virtual truth AddStateDescription(festring&, truth) const;
  character* Deceased;
};

ITEM(eddytorso, normaltorso)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 8; }
  virtual v2 GetBitmapPos(int) const;
};

ITEM(largetorso, normaltorso)
{
 public:
  virtual void SignalStackAdd(stackslot*, void (stack::*)(item*, truth));
  virtual int GetSquareIndex(v2) const;
  virtual void Draw(blitdata&) const;
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
 protected:
  virtual v2 GetBitmapPos(int I) const { return GetLargeBitmapPos(BitmapPos, I); }
  virtual void ModifyAnimationFrames(int& AF) const { AF <<= 2; }
};

ITEM(largecorpse, corpse)
{
 public:
  virtual void SignalStackAdd(stackslot*, void (stack::*)(item*, truth));
  virtual int GetSquareIndex(v2) const;
  virtual void Draw(blitdata&) const;
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
 protected:
  virtual v2 GetBitmapPos(int I) const { return GetLargeBitmapPos(item::GetBitmapPos(I), I); }
  virtual void ModifyAnimationFrames(int& AF) const { AF <<= 2; }
};

ITEM(ennerhead, head)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual v2 GetBitmapPos(int) const;
};

ITEM(playerkindhead, head)
{
 public:
  playerkindhead() { }
  playerkindhead(const playerkindhead& Head) : mybase(Head) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata HelmetGraphicData;
};

ITEM(playerkindtorso, humanoidtorso)
{
 public:
  playerkindtorso() { }
  playerkindtorso(const playerkindtorso& Torso) : mybase(Torso) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual void SignalVolumeAndWeightChange();
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata TorsoArmorGraphicData;
  graphicdata CloakGraphicData;
  graphicdata BeltGraphicData;
};

ITEM(playerkindrightarm, rightarm)
{
 public:
  playerkindrightarm() { }
  playerkindrightarm(const playerkindrightarm& Arm) : mybase(Arm) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata ArmArmorGraphicData;
  graphicdata GauntletGraphicData;
};

ITEM(playerkindleftarm, leftarm)
{
 public:
  playerkindleftarm() { }
  playerkindleftarm(const playerkindleftarm& Arm) : mybase(Arm) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata ArmArmorGraphicData;
  graphicdata GauntletGraphicData;
};

ITEM(playerkindgroin, groin)
{
 public:
  playerkindgroin() { }
  playerkindgroin(const playerkindgroin& Groin) : mybase(Groin) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata GroinArmorGraphicData;
};

ITEM(playerkindrightleg, rightleg)
{
 public:
  playerkindrightleg() { }
  playerkindrightleg(const playerkindrightleg& Leg) : mybase(Leg) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata LegArmorGraphicData;
  graphicdata BootGraphicData;
};

ITEM(playerkindleftleg, leftleg)
{
 public:
  playerkindleftleg() { }
  playerkindleftleg(const playerkindleftleg& Leg) : mybase(Leg) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth UpdateArmorPictures();
  virtual void DrawArmor(blitdata&) const;
  virtual truth ShowFluids() const { return true; }
  virtual truth IsAnimated() const { return true; }
 protected:
  graphicdata LegArmorGraphicData;
  graphicdata BootGraphicData;
};

ITEM(magicmushroomtorso, normaltorso)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 64; }
  virtual v2 GetBitmapPos(int) const;
};

ITEM(dogtorso, normaltorso)
{
 protected:
  virtual void Draw(blitdata&) const;
  virtual int GetClassAnimationFrames() const { return 16; }
  virtual v2 GetBitmapPos(int) const;
};

ITEM(blinkdogtorso, dogtorso)
{
 protected:
  virtual alpha GetAlphaA(int) const;
  virtual int GetClassAnimationFrames() const { return 64; }
};

ITEM(mysticfrogtorso, normaltorso)
{
 public:
  virtual truth AllowAlphaEverywhere() const { return true; }
 protected:
  virtual int GetClassAnimationFrames() const { return 128; }
  virtual col16 GetOutlineColor(int) const;
  virtual alpha GetOutlineAlpha(int) const;
};

ITEM(lobhsetorso, largetorso)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual col16 GetMaterialColorD(int) const;
};

ITEM(battorso, normaltorso)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 16; }
  virtual v2 GetBitmapPos(int) const;
};

ITEM(spidertorso, normaltorso)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 16; }
  virtual v2 GetBitmapPos(int) const;
};

#endif
