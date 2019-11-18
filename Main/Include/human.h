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

#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "char.h"
#include "confdef.h"

CHARACTER(humanoid, character)
{
 public:
  friend class rightarm;
  friend class leftarm;
  humanoid() : CurrentRightSWeaponSkill(0), CurrentLeftSWeaponSkill(0) { }
  humanoid(const humanoid&);
  virtual ~humanoid();
  virtual truth CanWield() const;
  virtual truth Hit(character*, v2, int, int = 0);
  virtual int GetSize() const;
  head* GetHead() const { return static_cast<head*>(*BodyPartSlot[HEAD_INDEX]); }
  arm* GetRightArm() const { return static_cast<arm*>(*BodyPartSlot[RIGHT_ARM_INDEX]); }
  arm* GetLeftArm() const { return static_cast<arm*>(*BodyPartSlot[LEFT_ARM_INDEX]); }
  groin* GetGroin() const { return static_cast<groin*>(*BodyPartSlot[GROIN_INDEX]); }
  leg* GetRightLeg() const { return static_cast<leg*>(*BodyPartSlot[RIGHT_LEG_INDEX]); }
  leg* GetLeftLeg() const { return static_cast<leg*>(*BodyPartSlot[LEFT_LEG_INDEX]); }
  item* GetHelmet() const { return GetHead() ? GetHead()->GetHelmet() : 0; }
  item* GetAmulet() const { return GetHead() ? GetHead()->GetAmulet() : 0; }
  item* GetCloak() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetCloak() : 0; }
  item* GetBodyArmor() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetBodyArmor() : 0; }
  item* GetBelt() const { return GetHumanoidTorso() ? GetHumanoidTorso()->GetBelt() : 0; }
  item* GetRightWielded() const { return GetRightArm() ? GetRightArm()->GetWielded() : 0; }
  item* GetLeftWielded() const { return GetLeftArm() ? GetLeftArm()->GetWielded() : 0; }
  item* GetRightRing() const { return GetRightArm() ? GetRightArm()->GetRing() : 0; }
  item* GetLeftRing() const { return GetLeftArm() ? GetLeftArm()->GetRing() : 0; }
  item* GetRightGauntlet() const { return GetRightArm() ? GetRightArm()->GetGauntlet() : 0; }
  item* GetLeftGauntlet() const { return GetLeftArm() ? GetLeftArm()->GetGauntlet() : 0; }
  item* GetRightBoot() const { return GetRightLeg() ? GetRightLeg()->GetBoot() : 0; }
  item* GetLeftBoot() const { return GetLeftLeg() ? GetLeftLeg()->GetBoot() : 0; }
  void SetHelmet(item* What) { GetHead()->SetHelmet(What); }
  void SetAmulet(item* What) { GetHead()->SetAmulet(What); }
  void SetCloak(item* What) { GetHumanoidTorso()->SetCloak(What); }
  void SetBodyArmor(item* What) { GetHumanoidTorso()->SetBodyArmor(What); }
  void SetBelt(item* What) { GetHumanoidTorso()->SetBelt(What); }
  void SetRightWielded(item* What) { GetRightArm()->SetWielded(What); }
  void SetLeftWielded(item* What) { GetLeftArm()->SetWielded(What); }
  void SetRightRing(item* What) { GetRightArm()->SetRing(What); }
  void SetLeftRing(item* What) { GetLeftArm()->SetRing(What); }
  void SetRightGauntlet(item* What) { GetRightArm()->SetGauntlet(What); }
  void SetLeftGauntlet(item* What) { GetLeftArm()->SetGauntlet(What); }
  void SetRightBoot(item* What) { GetRightLeg()->SetBoot(What); }
  void SetLeftBoot(item* What) { GetLeftLeg()->SetBoot(What); }
  arm* GetMainArm() const;
  arm* GetSecondaryArm() const;
  virtual truth ReceiveDamage(character*, int, int, int = ALL, int = 8, truth = false, truth = false, truth = false, truth = true);
  virtual truth BodyPartIsVital(int) const;
  virtual truth BodyPartCanBeSevered(int) const;
  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;
  virtual cchar* GetEquipmentName(int) const;
  virtual bodypart* GetBodyPartOfEquipment(int) const;
  virtual item* GetEquipment(int) const;
  virtual int GetEquipments() const { return 13; }
  virtual void SwitchToCraft(recipedata rpd);
  virtual void SwitchToDig(item*, v2);
  virtual int GetUsableLegs() const;
  virtual int GetUsableArms() const;
  virtual truth CheckKick() const;
  virtual int OpenMultiplier() const;
  virtual int CloseMultiplier() const;
  virtual truth CheckThrow() const;
  virtual truth CheckOffer() const;
  virtual sorter EquipmentSorter(int) const;
  virtual void SetEquipment(int, item*);
  virtual void DrawSilhouette(truth) const;
  virtual int GetGlobalResistance(int) const;
  virtual truth TryToRiseFromTheDead();
  virtual truth HandleNoBodyPart(int);
  virtual void Kick(lsquare*, int, truth = false);
  virtual double GetTimeToKill(ccharacter*, truth) const;
  virtual int GetAttribute(int, truth = true) const;
  virtual truth EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  virtual int DrawStats(truth) const;
  virtual void Bite(character*, v2, int, truth = false);
  virtual int GetCarryingStrength() const;
  virtual int GetRandomStepperBodyPart() const;
  virtual int CheckForBlock(character*, item*, double, int, int, int);
  virtual truth AddSpecialSkillInfo(felist&) const;
  virtual truth CheckBalance(double);
  virtual long GetMoveAPRequirement(int) const;
  virtual v2 GetEquipmentPanelPos(int) const;
  virtual truth EquipmentEasilyRecognized(int) const;
  sweaponskill* GetCurrentRightSWeaponSkill() const { return CurrentRightSWeaponSkill; }
  void SetCurrentRightSWeaponSkill(sweaponskill* What) { CurrentRightSWeaponSkill = What; }
  sweaponskill* GetCurrentLeftSWeaponSkill() const { return CurrentLeftSWeaponSkill; }
  void SetCurrentLeftSWeaponSkill(sweaponskill* What) { CurrentLeftSWeaponSkill = What; }
  virtual void SWeaponSkillTick();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SignalEquipmentAdd(int);
  virtual void SignalEquipmentRemoval(int, citem*);
  virtual void DrawBodyParts(blitdata&) const;
  virtual truth CanUseStethoscope(truth) const;
  virtual truth IsUsingArms() const;
  virtual truth IsUsingLegs() const;
  virtual truth IsUsingHead() const;
  virtual void CalculateBattleInfo();
  virtual void CalculateBodyParts();
  virtual void CalculateAllowedWeaponSkillCategories();
  virtual truth HasALeg() const { return HasAUsableLeg(); }
  virtual void AddSpecialEquipmentInfo(festring&, int) const;
  virtual void CreateInitialEquipment(int);
  virtual festring GetBodyPartName(int, truth = false) const;
  virtual void CreateBlockPossibilityVector(blockvector&, double) const;
  virtual item* SevereBodyPart(int, truth = false, stack* = 0);
  virtual int GetSWeaponSkillLevel(citem*) const;
  virtual truth UseMaterialAttributes() const;
  virtual void CalculateDodgeValue();
  virtual truth CheckZap();
  virtual truth IsHumanoid() const { return true; }
  virtual truth CheckTalk();
  virtual truth CheckIfEquipmentIsNotUsable(int) const;
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual item* GetPairEquipment(int) const;
  virtual truth HasHead() const { return truth(GetHead()); }
  virtual cfestring& GetStandVerb() const;
  virtual head* Behead();
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const;
  virtual void AddDefenceInfo(felist&) const;
  virtual void DetachBodyPart();
  virtual void SetFireToBodyPart();
  virtual int GetRandomApplyBodyPart() const;
  void EnsureCurrentSWeaponSkillIsCorrect(sweaponskill*&, citem*);
  virtual int GetSumOfAttributes() const;
  virtual truth CheckConsume(cfestring&) const;
  virtual truth CanConsume(material*) const;
  virtual truth PreProcessForBone();
  virtual void FinalProcessForBone();
  virtual void StayOn(liquid*);
  virtual head* GetVirtualHead() const { return GetHead(); }
  virtual character* CreateZombie() const;
  virtual void LeprosyHandler();
  virtual void DropRandomNonVitalBodypart();
  virtual void DropBodyPart(int);
  virtual void DuplicateEquipment(character*, ulong);
  virtual int GetAttributeAverage() const;
  virtual truth CanVomit() const;
  virtual truth CheckApply() const;
  virtual truth CanForceVomit() const { return TorsoIsAlive() && HasAUsableArm(); }
  virtual truth IsTransparent() const;
  virtual void ModifySituationDanger(double&) const;
  virtual int RandomizeTryToUnStickBodyPart(ulong) const;
  virtual truth AllowUnconsciousness() const;
  virtual truth CanChokeOnWeb(web*) const;
  virtual truth BrainsHurt() const;
  virtual truth IsHeadless() const;
  virtual cchar* GetRunDescriptionLine(int) const;
  virtual cchar* GetNormalDeathMessage() const;
  virtual void ApplySpecialAttributeBonuses();
  virtual truth MindWormCanPenetrateSkull(mindworm*) const;
  truth HasSadistWeapon() const;
  truth CheckAIZapOpportunity();
  virtual truth HasSadistAttackMode() const;
  truth AutoPlayAIequip();
  static v2 GetSilhouetteWhere(){return SilhouetteWhere;}
  static v2 GetSilhouetteWhereDefault(){return SilhouetteWhereDefault;}
  static void SetSilhouetteWhere(v2 pos){SilhouetteWhere=pos;}
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth, truth, int);
  virtual v2 GetBodyPartBitmapPos(int, truth = false) const;
  virtual col16 GetBodyPartColorB(int, truth = false) const;
  virtual col16 GetBodyPartColorC(int, truth = false) const;
  virtual col16 GetBodyPartColorD(int, truth = false) const;
  virtual int GetBodyPartSparkleFlags(int) const;
  virtual long GetBodyPartSize(int, int) const;
  virtual long GetBodyPartVolume(int) const;
  virtual bodypart* MakeBodyPart(int) const;
  virtual cfestring& GetDeathMessage() const;
  truth HasAUsableArm() const;
  truth HasAUsableLeg() const;
  truth HasTwoUsableLegs() const;
  truth CanAttackWithAnArm() const;
  truth RightArmIsUsable() const;
  truth LeftArmIsUsable() const;
  truth RightLegIsUsable() const;
  truth LeftLegIsUsable() const;
  std::list<sweaponskill*> SWeaponSkill;
  sweaponskill* CurrentRightSWeaponSkill;
  sweaponskill* CurrentLeftSWeaponSkill;
  static cint DrawOrder[];
  static v2 SilhouetteWhereDefault;
  static v2 SilhouetteWhere;
};

CHARACTER(playerkind, humanoid)
{
 public:
  playerkind();
  playerkind(const playerkind&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetSoulID(ulong);
  virtual truth SuckSoul(character*);
  virtual truth TryToRiseFromTheDead();
  virtual void FinalProcessForBone();
  virtual void BeTalkedTo();
  virtual truth IsHuman() const { return true; }
  virtual col16 GetHairColor() const { return HairColor; }
  virtual col16 GetEyeColor() const { return EyeColor; }
  virtual v2 GetHeadBitmapPos() const;
  virtual v2 GetRightArmBitmapPos() const;
  virtual v2 GetLeftArmBitmapPos() const;
  virtual int GetNaturalSparkleFlags() const;
  virtual truth IsPlayerKind() const { return true; }
  virtual double GetNaturalExperience(int) const;
  virtual int GetTalent() const { return Talent; }
  virtual int GetWeakness() const { return Weakness; }
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void PostConstruct();
  ulong SoulID;
  col16 HairColor;
  col16 EyeColor;
  int Talent;
  int Weakness;
  truth IsBonePlayer;
  truth IsClone;
};

CHARACTER(petrus, humanoid)
{
 public:
  petrus();
  virtual ~petrus();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void BeTalkedTo();
  truth HealFully(character*);
  virtual void FinalProcessForBone();
  virtual truth MoveTowardsHomePos();
 protected:
  virtual void CreateCorpse(lsquare*);
  virtual void GetAICommand();
  ulong LastHealed;
};

CHARACTER(farmer, humanoid)
{
 public:
  virtual v2 GetHeadBitmapPos() const;
  virtual v2 GetRightArmBitmapPos() const;
  virtual v2 GetLeftArmBitmapPos() const { return GetRightArmBitmapPos(); }
};

CHARACTER(guard, humanoid)
{
 public:
  guard() : NextWayPoint(0) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void GetAICommand();
  virtual void SetWayPoints(const fearray<packv2>&);
  virtual truth MoveTowardsHomePos();
  virtual void BeTalkedTo();
 protected:
  std::vector<v2> WayPoints;
  uint NextWayPoint;
};

CHARACTER(shopkeeper, humanoid)
{
 public:
  shopkeeper();
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(priest, humanoid)
{
 protected:
  virtual void GetAICommand();
  virtual void BeTalkedTo();
  void CallForMonsters();
};

CHARACTER(oree, humanoid)
{
 public:
  virtual void Bite(character*, v2, int, truth = false);
  virtual void GetAICommand();
 protected:
  virtual cchar* FirstPersonBiteVerb() const;
  virtual cchar* FirstPersonCriticalBiteVerb() const;
  virtual cchar* ThirdPersonBiteVerb() const;
  virtual cchar* ThirdPersonCriticalBiteVerb() const;
  virtual cchar* BiteNoun() const;
  void CallForMonsters();
};

CHARACTER(darkknight, humanoid)
{
 public:
  virtual truth SpecialEnemySightedReaction(character*);
  virtual truth CheckForUsefulItemsOnGround(truth = true);
 protected:
  virtual int ModifyBodyPartHitPreference(int, int) const;
  virtual int ModifyBodyPartToHitChance(int, int) const;
  virtual truth CanPanicFromSeveredBodyPart() const { return false; }
  virtual void SpecialBodyPartSeverReaction();
};

CHARACTER(ennerbeast, humanoid)
{
 public:
  virtual truth Hit(character*, v2, int, int = 0);
  virtual truth MustBeRemovedFromBone() const;
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
  virtual truth AttackIsBlockable(int) const { return false; }
};

CHARACTER(ennerchild, humanoid)
{
 public:
  virtual truth Hit(character*, v2, int, int = 0);
  virtual truth MustBeRemovedFromBone() const;
  virtual truth ReceiveDamage(character*, int, int, int = ALL, int = 8, truth = false, truth = false, truth = false, truth = true);
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
  virtual truth AttackIsBlockable(int) const { return false; }
};

CHARACTER(skeleton, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual item* SevereBodyPart(int, truth = false, stack* = 0);
  virtual truth BodyPartIsVital(int) const;
  virtual long GetBodyPartVolume(int) const;
 protected:
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(goblin, humanoid)
{
 public:
  virtual void GetAICommand();
};

CHARACTER(golem, humanoid)
{
 public:
  golem();
  virtual truth MoveRandomly();
  virtual truth CheckForUsefulItemsOnGround(truth = true) { return false; }
  virtual void BeTalkedTo();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetItemVolume(long What) { ItemVolume = What; }
 protected:
  virtual truth AddAdjective(festring&, truth) const;
  virtual material* CreateBodyPartMaterial(int, long) const;
  virtual void CreateCorpse(lsquare*);
  long ItemVolume;
};

CHARACTER(communist, humanoid)
{
 public:
  virtual truth MoveRandomly();
  virtual void BeTalkedTo();
  virtual truth BoundToUse(citem*, int) const;
  virtual truth MustBeRemovedFromBone() const;
 protected:
  virtual truth ShowClassDescription() const;
};

CHARACTER(hunter, humanoid)
{
 public:
  virtual void BeTalkedTo();
 protected:
  virtual void CreateBodyParts(int);
};

CHARACTER(slave, playerkind)
{
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand();
  virtual col16 GetHairColor() const { return humanoid::GetHairColor(); }
  virtual col16 GetEyeColor() const { return humanoid::GetEyeColor(); }
 protected:
  virtual void PostConstruct();
};

CHARACTER(petrusswife, humanoid)
{
 public:
  petrusswife() : GiftTotal(0) { }
  virtual truth MoveRandomly() { return MoveRandomlyInRoom(); }
  virtual truth SpecialEnemySightedReaction(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void BeTalkedTo();
 protected:
  int GiftTotal;
};

CHARACTER(housewife, humanoid)
{
 public:
  virtual truth SpecialEnemySightedReaction(character*);
  virtual void CreateInitialEquipment(int);
 protected:
  virtual int GetHairColor() const;
  virtual v2 GetHeadBitmapPos() const;
};

CHARACTER(femaleslave, humanoid)
{
 public:
  virtual void BeTalkedTo();
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(librarian, humanoid)
{
 public:
  virtual void BeTalkedTo();
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(zombie, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual truth BodyPartIsVital(int) const;
  virtual void CreateBodyParts(int);
  virtual truth AllowSpoil() const { return GetConfig() == ZOMBIE_OF_KHAZ_ZADM; }
  void SetDescription(cfestring What) { Description = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual festring GetZombieDescription() const;
 protected:
  virtual void PostConstruct();
  virtual void AddPostFix(festring&, int) const;
  virtual void GetAICommand();
  virtual truth AllowExperience() const { return GetConfig() == ZOMBIE_OF_KHAZ_ZADM; }
  festring Description;
};

CHARACTER(ghost, humanoid)
{
 public:
  ghost() : Active(true) { }
  virtual truth BodyPartIsVital(int) const;
//  virtual void CreateBodyParts(int); // as per zombies, in case some body parts are missing?
  void SetDescription(cfestring What) { Description = What; }
  virtual festring GetGhostDescription() const;
  virtual void AddName(festring&, int) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetOwnerSoul(cfestring& What) { OwnerSoul = What; }
  virtual truth IsNameable() const { return OwnerSoul.IsEmpty(); }
  virtual truth RaiseTheDead(character*);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, truth = false, truth = false, truth = true, truth = false);
  virtual truth SpecialEnemySightedReaction(character*);
  void SetIsActive(truth What) { Active = What; }
  virtual truth IsPolymorphable() const { return MaxHP < 100; }
 protected:
  virtual truth AllowExperience() const { return false; }
  virtual cchar* FirstPersonUnarmedHitVerb() const;
  virtual cchar* FirstPersonCriticalUnarmedHitVerb() const;
  virtual cchar* ThirdPersonUnarmedHitVerb() const;
  virtual cchar* ThirdPersonCriticalUnarmedHitVerb() const;
  virtual truth AttackIsBlockable(int) const { return false; }
  virtual truth AttackMayDamageArmor() const { return false; }
  virtual void GetAICommand();
  festring OwnerSoul;
  truth Active;
  festring Description;
};

CHARACTER(bonesghost, ghost)
{
 public:
  virtual col16 GetHairColor() const { return HairColor; }
  virtual col16 GetEyeColor() const { return EyeColor; }
  virtual v2 GetHeadBitmapPos() const;
  virtual v2 GetRightArmBitmapPos() const;
  virtual v2 GetLeftArmBitmapPos() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void PostConstruct();
  virtual truth IsPolymorphable() const { return false; }
 protected:
  col16 HairColor;
  col16 EyeColor;
};

CHARACTER(xinrochghost, ghost)
{
 public:
  virtual truth IsNameable() const { return false; }
  virtual truth IsPolymorphable() const { return false; }
  virtual truth CheckForUsefulItemsOnGround(truth = true) { return false; }
 protected:
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(imp, humanoid)
{
};

CHARACTER(crimsonimp, imp)
{
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth, truth, int);
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(mirrorimp, imp)
{
 public:
  virtual truth IsMagicDrinker() const { return true; }
  virtual truth DrinkMagic(const beamdata&);
 protected:
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(mistress, humanoid)
{
 public:
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual truth ReceiveDamage(character*, int, int, int = ALL, int = 8, truth = false, truth = false, truth = false, truth = true);
  virtual truth AllowEquipment(citem*, int) const;
};

CHARACTER(werewolfhuman, humanoid)
{
};

CHARACTER(werewolfwolf, humanoid)
{
 public:
  virtual festring GetKillName() const;
  virtual void GetAICommand();
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth, truth, int);
};

CHARACTER(vampire, humanoid)
{
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth, truth, int);
};

CHARACTER(kobold, humanoid)
{
};

CHARACTER(gibberling, humanoid)
{
};

CHARACTER(angel, humanoid)
{
 public:
  angel() : LastHealed(0) { }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual truth AttachBodyPartsOfFriendsNear();
  virtual truth BodyPartIsVital(int) const;
  virtual int GetAttribute(int, truth = true) const;
  virtual col24 GetBaseEmitation() const;
  virtual truth CanCreateBodyPart(int) const;
  virtual cfestring& GetStandVerb() const { return character::GetStandVerb(); }
  virtual void FinalProcessForBone();
  virtual void CreateInitialEquipment(int);
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetArmMainColor() const;
  virtual void GetAICommand();
  ulong LastHealed;
};

CHARACTER(kamikazedwarf, humanoid)
{
 public:
  virtual truth Hit(character*, v2, int, int = 0);
  virtual truth CheckForUsefulItemsOnGround(truth = true) { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment(int);
  virtual truth IsKamikazeDwarf() const { return true; }
  virtual void SingRandomSong();
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetGauntletColor() const;
  virtual int GetLegMainColor() const;
  virtual int GetWSkillHits() const { return 10000; }
  virtual truth IsElite() const { return false; }
};

CHARACTER(genie, humanoid)
{
 public:
  virtual truth BodyPartIsVital(int) const;
  virtual int GetAttribute(int, truth = true) const;
  virtual truth CanCreateBodyPart(int) const;
  virtual cfestring& GetStandVerb() const { return character::GetStandVerb(); }
};

CHARACTER(orc, humanoid)
{
 public:
  virtual truth MoveRandomly();
 protected:
  virtual void PostConstruct();
};

CHARACTER(cossack, humanoid)
{
};

CHARACTER(bananagrower, humanoid)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void BeTalkedTo();
  virtual festring& ProcessMessage(festring&) const;
  virtual truth IsBananaGrower() const { return true; }
  festring GetProfession() const { return Profession; }
 protected:
  virtual truth HandleCharacterBlockingTheWay(character*, v2, int);
  virtual void PostConstruct();
  virtual void GetAICommand();
  void RandomizeProfession();
  festring Profession;
  truth HasDroppedBananas;
  truth FeedingSumo;
};

CHARACTER(imperialist, humanoid)
{
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void BeTalkedTo();
  virtual void DisplayStethoscopeInfo(character*) const;
};

CHARACTER(smith, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(elder, humanoid)
{
 public:
  elder() : HasBeenSpokenTo(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void GetAICommand();
  virtual void CreateBodyParts(int);
  virtual void BeTalkedTo();
  truth HasBeenSpokenTo;
};

CHARACTER(encourager, humanoid)
{
 /*public:
  encourager() : LastHit(0) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void FinalProcessForBone();
 protected:
  virtual void GetAICommand();
  ulong LastHit;*/
 protected:
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(darkmage, humanoid)
{
 protected:
  virtual void GetAICommand();
  int GetSpellAPCost() const;
};

CHARACTER(necromancer, humanoid)
{
 public:
  virtual truth TryToRaiseZombie();
  virtual void RaiseSkeleton();
  virtual void BeTalkedTo();
 protected:
  virtual void GetAICommand();
  int GetSpellAPCost() const;
};

CHARACTER(sumowrestler, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual truth CheckIfSatiated() { return GetNP() > BLOATED_LEVEL; }
  virtual truth IsSumoWrestler() const { return true; }
  virtual truth EquipmentIsAllowed(int) const;
 protected:
  virtual void GetAICommand();
};

CHARACTER(tourist, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual character* GetLeader() const;
 protected:
  virtual void GetAICommand();
};

CHARACTER(veterankamikazedwarf, kamikazedwarf)
{
 protected:
  virtual void PostConstruct();
  virtual int GetTorsoMainColor() const;
  virtual int GetGauntletColor() const;
  virtual int GetLegMainColor() const;
  virtual int GetWSkillHits() const { return 50000; }
  virtual truth IsElite() const { return true; }
};

CHARACTER(archangel, angel)
{
 public:
  virtual void CreateInitialEquipment(int);
 protected:
  virtual int GetTorsoMainColor() const;
  virtual int GetArmMainColor() const;
};

CHARACTER(tailor, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(siren, humanoid)
{
 public:
  virtual void GetAICommand();
  virtual truth MoveRandomly();
 protected:
  virtual truth TryToSing();
};

CHARACTER(punisher, humanoid)
{
};

CHARACTER(child, humanoid)
{
 public:
  virtual truth MoveRandomly();
  virtual truth IsKing() const { return GetConfig() == KING; }
  virtual truth MustBeRemovedFromBone() const;
 protected:
  virtual void BeTalkedTo();
};

CHARACTER(bum, humanoid)
{
};

CHARACTER(nihil, humanoid)
{
 public:
  virtual truth BodyPartIsVital(int) const;
  virtual truth CanCreateBodyPart(int) const;
  virtual int GetAttribute(int, truth = true) const;
  virtual col24 GetBaseEmitation() const { return MakeRGB24(150, 110, 110); }
  virtual cfestring& GetStandVerb() const { return character::GetStandVerb(); }
};

CHARACTER(terra, priest)
{
 public:
  terra() : HasBeenSpokenTo(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void BeTalkedTo();
  truth HasBeenSpokenTo;
};

CHARACTER(aslonawizard, humanoid)
{
 public:
  aslonawizard() : HasBeenSpokenTo(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void GetAICommand();
  int GetSpellAPCost() const;
  virtual void BeTalkedTo();
  virtual void CreateCorpse(lsquare*);
  truth HasBeenSpokenTo;
};

CHARACTER(aslonacaptain, guard)
{
 public:
  aslonacaptain() : HasBeenSpokenTo(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void BeTalkedTo();
  truth HasBeenSpokenTo;
};

CHARACTER(aslonapriest, priest)
{
 public:
  aslonapriest() : HasBeenSpokenTo(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void BeTalkedTo();
  truth HasBeenSpokenTo;
};

CHARACTER(gasghoul, zombie)
{
 public:
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual truth AllowSpoil() const { return false; }
};

CHARACTER(harvan, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
};

CHARACTER(lordregent, humanoid)
{
 public:
  virtual void BeTalkedTo();
  virtual void GetAICommand() { StandIdleAI(); }
 protected:
  virtual void SpecialBodyPartSeverReaction();
};

#endif
