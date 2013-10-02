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

#ifndef __CHAR_H__
#define __CHAR_H__

#include "bodypart.h"
#include "script.h"
#include "wskill.h"
#include "save.h"

#define CHAR_PERSONAL_PRONOUN GetPersonalPronoun(true).CStr()
#define CHAR_POSSESSIVE_PRONOUN GetPossessivePronoun(true).CStr()
#define CHAR_OBJECT_PRONOUN GetObjectPronoun(true).CStr()
#define CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW GetPersonalPronoun(false).CStr()
#define CHAR_POSSESSIVE_PRONOUN_THIRD_PERSON_VIEW GetPossessivePronoun(false).CStr()
#define CHAR_OBJECT_PRONOUN_THIRD_PERSON_VIEW GetObjectPronoun(false).CStr()

class go;
class team;
class wsquare;
class cweaponskill;
class action;
class characterprototype;
class web;
class mindworm;

struct homedata;
struct trapdata;
struct blitdata;

typedef std::vector<std::pair<double, int> > blockvector;
typedef truth (item::*sorter)(ccharacter*) const;
typedef truth (character::*petmanagementfunction)();
typedef character* (*characterspawner)(int, int);
typedef character* (*charactercloner)(ccharacter*);

inline int APBonus(int Attribute)
{
  return Attribute >= 10 ? 90 + Attribute : 50 + Attribute * 5;
}

struct expid
{
  bool operator<(expid) const;
  int ActID, SrcID;
};

inline bool expid::operator<(expid E) const
{
  return ActID != E.ActID ? ActID < E.ActID : SrcID < E.SrcID;
}

RAW_SAVE_LOAD(expid);

typedef std::map<expid, double> expmodifiermap;

struct characterdatabase : public databasebase
{
  typedef characterprototype prototype;
  void InitDefaults(const prototype*, int);
  truth AllowRandomInstantiation() const { return CanBeGenerated && !IsUnique; }
  void PostProcess();
  const prototype* ProtoType;
  double NaturalExperience[ATTRIBUTES];
  ulong Flags;
  truth IsAbstract;
  truth CanRead;
  truth CanBeGenerated;
  truth CanOpen;
  truth IsUnique;
  truth IsNameable;
  truth IsPolymorphable;
  truth CanUseEquipment;
  truth CanKick;
  truth CanTalk;
  truth CanBeWished;
  truth CreateDivineConfigurations;
  truth CreateGolemMaterialConfigurations;
  truth CanBeCloned;
  truth CanZap;
  truth HasALeg;
  truth IgnoreDanger;
  truth IsExtraCoward;
  truth SpillsBlood;
  truth HasEyes;
  truth HasHead;
  truth CanThrow;
  truth UsesNutrition;
  truth BodyPartsDisappearWhenSevered;
  truth CanBeConfused;
  truth CanApply;
  truth BiteCapturesBodyPart;
  truth IsPlant;
  truth DestroysWalls;
  truth IsRooted;
  truth HasSecondaryMaterial;
  truth IsImmuneToLeprosy;
  truth AutomaticallySeen;
  truth CanHear;
  truth WillCarryItems;
  truth Sweats;
  truth IsImmuneToItemTeleport;
  truth AlwaysUseMaterialAttributes;
  truth IsEnormous;
  truth IsExtraFragile;
  truth AllowUnconsciousness;
  truth CanChoke;
  truth IsImmuneToStickiness;
  truth ForceCustomStandVerb;
  truth VomittingIsUnhealthy;
  int DefaultEndurance;
  int DefaultPerception;
  int DefaultIntelligence;
  int DefaultWisdom;
  int DefaultWillPower;
  int DefaultCharisma;
  int DefaultMana;
  int DefaultArmStrength;
  int DefaultLegStrength;
  int DefaultDexterity;
  int DefaultAgility;
  long DefaultMoney;
  int TotalSize;
  int Sex;
  int CriticalModifier;
  festring StandVerb;
  int Frequency;
  int EnergyResistance;
  int FireResistance;
  int PoisonResistance;
  int ElectricityResistance;
  int AcidResistance;
  int ConsumeFlags;
  long TotalVolume;
  packv2 HeadBitmapPos;
  packv2 TorsoBitmapPos;
  packv2 ArmBitmapPos;
  packv2 LegBitmapPos;
  packv2 RightArmBitmapPos;
  packv2 LeftArmBitmapPos;
  packv2 RightLegBitmapPos;
  packv2 LeftLegBitmapPos;
  packv2 GroinBitmapPos;
  packcol16 ClothColor;
  packcol16 SkinColor;
  packcol16 CapColor;
  packcol16 HairColor;
  packcol16 EyeColor;
  packcol16 TorsoMainColor;
  packcol16 BeltColor;
  packcol16 BootColor;
  packcol16 TorsoSpecialColor;
  packcol16 ArmMainColor;
  packcol16 GauntletColor;
  packcol16 ArmSpecialColor;
  packcol16 LegMainColor;
  packcol16 LegSpecialColor;
  col24 BaseEmitation;
  truth UsesLongArticle;
  festring Adjective;
  truth UsesLongAdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  int ArticleMode;
  int BaseUnarmedStrength;
  int BaseBiteStrength;
  int BaseKickStrength;
  int AttackStyle;
  long ClassStates;
  fearray<festring> Alias;
  contentscript<item> Helmet;
  contentscript<item> Amulet;
  contentscript<item> Cloak;
  contentscript<item> BodyArmor;
  contentscript<item> Belt;
  contentscript<item> RightWielded;
  contentscript<item> LeftWielded;
  contentscript<item> RightRing;
  contentscript<item> LeftRing;
  contentscript<item> RightGauntlet;
  contentscript<item> LeftGauntlet;
  contentscript<item> RightBoot;
  contentscript<item> LeftBoot;
  int AttributeBonus;
  fearray<long> KnownCWeaponSkills;
  fearray<long> CWeaponSkillHits;
  int RightSWeaponSkillHits;
  int LeftSWeaponSkillHits;
  int PanicLevel;
  fearray<contentscript<item> > Inventory;
  int DangerModifier;
  festring DefaultName;
  fearray<festring> FriendlyReplies;
  fearray<festring> HostileReplies;
  int FleshMaterial;
  festring DeathMessage;
  int HPRequirementForGeneration;
  int DayRequirementForGeneration;
  int AttackWisdomLimit;
  int AttachedGod;
  packv2 WieldedPosition;
  int NaturalSparkleFlags;
  int MoveType;
  int BloodMaterial;
  int VomitMaterial;
  int PolymorphIntelligenceRequirement;
  ulong DefaultCommandFlags;
  ulong ConstantCommandFlags;
  festring ForceVomitMessage;
  int SweatMaterial;
  fearray<festring> ScienceTalkAdjectiveAttribute;
  fearray<festring> ScienceTalkSubstantiveAttribute;
  fearray<festring> ScienceTalkPrefix;
  fearray<festring> ScienceTalkName;
  int ScienceTalkPossibility;
  int ScienceTalkIntelligenceModifier;
  int ScienceTalkWisdomModifier;
  int ScienceTalkCharismaModifier;
  int ScienceTalkIntelligenceRequirement;
  int ScienceTalkWisdomRequirement;
  int ScienceTalkCharismaRequirement;
  int DisplacePriority;
  festring RunDescriptionLineOne;
  festring RunDescriptionLineTwo;
  truth AllowPlayerToChangeEquipment;
  int TamingDifficulty;
  truth IsMasochist;
  truth IsSadist;
  truth IsCatacombCreature;
  truth CreateUndeadConfigurations;
  truth UndeadVersions;
  int UndeadAttributeModifier;
  int UndeadVolumeModifier;
  truth UndeadCopyMaterials;
  truth CanBeGeneratedOnlyInTheCatacombs;
  truth IsAlcoholic;
  truth IsImmuneToWhipOfThievery;
};

class characterprototype
{
 public:
  friend class databasecreator<character>;
  friend class protosystem;
  characterprototype(const characterprototype*, characterspawner, charactercloner, cchar*);
  character* Spawn(int Config = 0, int SpecialFlags = 0) const { return Spawner(Config, SpecialFlags); }
  character* SpawnAndLoad(inputfile&) const;
  character* Clone(ccharacter* Char) const { return Cloner(Char); }
  int GetIndex() const { return Index; }
  const characterprototype* GetBase() const { return Base; }
  cchar* GetClassID() const { return ClassID; }
  int CreateSpecialConfigurations(characterdatabase**, int, int);
  const characterdatabase* ChooseBaseForConfig(characterdatabase** TempConfig, int, int) { return *TempConfig; }
  const characterdatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  const characterprototype* Base;
  characterdatabase** ConfigData;
  characterdatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  characterspawner Spawner;
  charactercloner Cloner;
  cchar* ClassID;
};

class character : public entity, public id
{
 public:
  friend class databasecreator<character>;
  friend class corpse;
  typedef characterprototype prototype;
  typedef characterdatabase database;
  character();
  character(ccharacter&);
  virtual ~character();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth CanWield() const { return false; }
  virtual truth Catches(item*) { return false; }
  truth CheckDeath(cfestring&, ccharacter* = 0, ulong = 0);
  truth DodgesFlyingItem(item*, double);
  virtual truth Hit(character*, v2, int, int = 0) = 0;
  truth ReadItem(item*);
  truth TestForPickup(item*) const;
  void ThrowItem(int, item*);
  truth TryMove(v2, truth, truth);
  truth HasHeadOfElpuri() const;
  truth HasGoldenEagleShirt() const;
  truth HasPetrussNut() const;
  truth RemoveEncryptedScroll();
  truth IsPlayer() const { return Flags & C_PLAYER; }
  truth Engrave(cfestring&);
  void AddScoreEntry(cfestring&, double = 1., truth = true) const;
  long GetAP() const { return AP; }
  long GetNP() const { return NP; }
  stack* GetStack() const { return Stack; }
  int GetBurdenState() const { return BurdenState; }
  truth MakesBurdened(long What) const { return long(GetCarryingStrength()) * 2500 < What; }
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  int GetLOSRange() const;
  int GetLOSRangeSquare() const { return GetLOSRange() * GetLOSRange(); }
  int GetESPRange() const { return GetAttribute(INTELLIGENCE) / 3; }
  int GetESPRangeSquare() const { return GetESPRange() * GetESPRange(); }
  int GetTeleportRange() const { return GetAttribute(INTELLIGENCE); }
  int GetTeleportRangeSquare() const { return GetTeleportRange() * GetTeleportRange(); }
  void AddMissMessage(ccharacter*) const;
  void AddPrimitiveHitMessage(ccharacter*, cfestring&, cfestring&, int) const;
  void AddWeaponHitMessage(ccharacter*, citem*, int, truth = false) const;
  virtual void BeTalkedTo();
  void ReceiveDarkness(long);
  void Die(ccharacter* = 0, cfestring& = CONST_S(""), ulong = 0);
  void HasBeenHitByItem(character*, item*, int, double, int);
  void Hunger();
  void Move(v2, truth, truth = false);
  virtual truth MoveRandomly();
  void ReceiveNutrition(long);
  void ReceiveOmmelUrine(long);
  void ReceivePepsi(long);
  void ReceiveSchoolFood(long);
  void Regenerate();
  void SetAP(long What) { AP = What; }
  void SetNP(long);
  void Vomit(v2, int, truth = true);
  virtual void Be();
  truth Polymorph(character*, int);
  void BeKicked(character*, item*, bodypart*, v2, double, double, int, int, truth, truth);
  void FallTo(character*, v2);
  truth CheckCannibalism(cmaterial*) const;
  void ActivateTemporaryState(long What) { TemporaryState |= What; }
  void DeActivateTemporaryState(long What) { TemporaryState &= ~What; }
  void ActivateEquipmentState(long What) { EquipmentState |= What; }
  void DeActivateEquipmentState(long What) { EquipmentState &= ~What; }
  truth TemporaryStateIsActivated(long What) const { return TemporaryState & What; }
  truth EquipmentStateIsActivated(long What) const { return EquipmentState & What; }
  truth StateIsActivated(long What) const { return TemporaryState & What || EquipmentState & What; }
  truth LoseConsciousness(int, truth = false);
  void SetTemporaryStateCounter(long, int);
  void DeActivateVoluntaryAction(cfestring& = CONST_S(""));
  void ActionAutoTermination();
  team* GetTeam() const { return Team; }
  void SetTeam(team*);
  void ChangeTeam(team*);
  virtual int GetMoveEase() const;
  double GetDodgeValue() const { return DodgeValue; }
  long GetMoney() const { return Money; }
  void SetMoney(long What) { Money = What; }
  void EditMoney(long What) { Money += What; }
  truth Displace(character*, truth = false);
  truth CheckStarvationDeath(cfestring&);
  void ShowNewPosInfo() const;
  void Hostility(character*);
  stack* GetGiftStack() const;
  truth MoveRandomlyInRoom();
  std::list<character*>::iterator GetTeamIterator();
  void SetTeamIterator(std::list<character*>::iterator);
  void ReceiveKoboldFlesh(long);
  truth ChangeRandomAttribute(int);
  int RandomizeReply(long&, int);
  virtual void CreateInitialEquipment(int);
  void DisplayInfo(festring&);
  virtual truth SpecialEnemySightedReaction(character*) { return false; }
  void TestWalkability();
  void EditAP(long);
  void EditNP(long);
  void SetSize(int);
  virtual int GetSize() const;
  torso* GetTorso() const { return static_cast<torso*>(*BodyPartSlot[TORSO_INDEX]); }
  humanoidtorso* GetHumanoidTorso() const { return static_cast<humanoidtorso*>(*BodyPartSlot[TORSO_INDEX]); }
  void SetTorso(torso* What) { SetBodyPart(TORSO_INDEX, What); }
  bodypart* GetBodyPart(int I) const { return static_cast<bodypart*>(*BodyPartSlot[I]); }
  void SetBodyPart(int, bodypart*);
  void SetMainMaterial(material*, int = 0);
  void ChangeMainMaterial(material*, int = 0);
  void SetSecondaryMaterial(material*, int = 0);
  void ChangeSecondaryMaterial(material*, int = 0);
  void RestoreHP();
  void RestoreLivingHP();
  void RestoreStamina() { Stamina = MaxStamina; }
  virtual truth ReceiveDamage(character*, int, int, int = ALL, int = 8, truth = false, truth = false, truth = false, truth = true);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, truth = false, truth = false, truth = true, truth = false);
  virtual truth BodyPartIsVital(int) const { return true; }
  void RestoreBodyParts();
  cfestring& GetAssignedName() const { return AssignedName; }
  void SetAssignedName(cfestring& What) { AssignedName = What; }
  festring GetDescription(int) const;
  festring GetPersonalPronoun(truth = true) const;
  festring GetPossessivePronoun(truth = true) const;
  festring GetObjectPronoun(truth = true) const;
  virtual truth BodyPartCanBeSevered(int) const;
  virtual void AddName(festring&, int) const;
  void ReceiveHeal(long);
  virtual item* GetMainWielded() const { return 0; }
  virtual item* GetSecondaryWielded() const { return 0; }
  int GetHungerState() const;
  truth ConsumeItem(item*, cfestring&);
  virtual truth CanConsume(material*) const;
  action* GetAction() const { return Action; }
  void SetAction(action* What) { Action = What; }
  virtual void SwitchToDig(item*, v2) { }
  virtual void SetRightWielded(item*) { }
  virtual void SetLeftWielded(item*) { }
  void GoOn(go*, truth = false);
  virtual truth CheckKick() const;
  virtual int OpenMultiplier() const { return 2; }
  virtual int CloseMultiplier() const { return 2; }
  virtual truth CheckThrow() const;
  virtual truth CheckOffer() const { return true; }
  int GetTemporaryStateCounter(long) const;
  void EditTemporaryStateCounter(long, int);
  static truth AllowDamageTypeBloodSpill(int);
  int GetResistance(int) const;
  virtual int GetGlobalResistance(int Type) const { return GetResistance(Type); }
  virtual cchar* GetEquipmentName(int) const;
  virtual bodypart* GetBodyPartOfEquipment(int) const { return 0; }
  virtual item* GetEquipment(int) const { return 0; }
  virtual int GetEquipments() const { return 0; }
  virtual sorter EquipmentSorter(int) const { return 0; }
  virtual void SetEquipment(int, item*) { }
  void AddHealingLiquidConsumeEndMessage() const;
  void AddSchoolFoodConsumeEndMessage() const;
  void AddSchoolFoodHitMessage() const;
  void AddOmmelConsumeEndMessage() const;
  void AddPepsiConsumeEndMessage() const;
  void AddFrogFleshConsumeEndMessage() const;
  void AddKoboldFleshConsumeEndMessage() const;
  void AddKoboldFleshHitMessage() const;
  void AddBoneConsumeEndMessage() const;
  void AddBlackUnicornConsumeEndMessage() const;
  void AddGrayUnicornConsumeEndMessage() const;
  void AddWhiteUnicornConsumeEndMessage() const;
  void AddOmmelBoneConsumeEndMessage() const;
  void PrintInfo() const;
  virtual item* SevereBodyPart(int, truth = false, stack* = 0);
  virtual truth TryToRiseFromTheDead();
  virtual truth RaiseTheDead(character*);
  bodypart* CreateBodyPart(int, int = 0);
  virtual truth EquipmentIsAllowed(int) const { return true; }
  truth CanUseEquipment(int) const;
  const database* GetDataBase() const { return DataBase; }
  void SetParameters(int) { }
  virtual double GetNaturalExperience(int) const;
  DATA_BASE_VALUE(const prototype*, ProtoType);
  DATA_BASE_VALUE(int, Config);
  DATA_BASE_VALUE(int, DefaultEndurance);
  DATA_BASE_VALUE(int, DefaultPerception);
  DATA_BASE_VALUE(int, DefaultIntelligence);
  DATA_BASE_VALUE(int, DefaultWisdom);
  DATA_BASE_VALUE(int, DefaultWillPower);
  DATA_BASE_VALUE(int, DefaultCharisma);
  DATA_BASE_VALUE(int, DefaultMana);
  DATA_BASE_VALUE(int, DefaultArmStrength);
  DATA_BASE_VALUE(int, DefaultLegStrength);
  DATA_BASE_VALUE(int, DefaultDexterity);
  DATA_BASE_VALUE(int, DefaultAgility);
  DATA_BASE_VALUE(long, DefaultMoney);
  DATA_BASE_VALUE(int, TotalSize);
  DATA_BASE_TRUTH(CanRead);
  DATA_BASE_VALUE(int, Sex);
  DATA_BASE_TRUTH(CanBeGenerated);
  DATA_BASE_VALUE(int, CriticalModifier);
  DATA_BASE_TRUTH(CanOpen);
  DATA_BASE_VALUE(int, EnergyResistance);
  DATA_BASE_VALUE(int, FireResistance);
  DATA_BASE_VALUE(int, PoisonResistance);
  DATA_BASE_VALUE(int, ElectricityResistance);
  DATA_BASE_VALUE(int, AcidResistance);
  DATA_BASE_VALUE(int, ConsumeFlags);
  DATA_BASE_VALUE(long, TotalVolume);
  virtual DATA_BASE_VALUE(v2, HeadBitmapPos);
  virtual DATA_BASE_VALUE(v2, TorsoBitmapPos);
  virtual DATA_BASE_VALUE(v2, ArmBitmapPos);
  virtual DATA_BASE_VALUE(v2, LegBitmapPos);
  virtual DATA_BASE_VALUE(v2, RightArmBitmapPos);
  virtual DATA_BASE_VALUE(v2, LeftArmBitmapPos);
  virtual DATA_BASE_VALUE(v2, RightLegBitmapPos);
  virtual DATA_BASE_VALUE(v2, LeftLegBitmapPos);
  virtual DATA_BASE_VALUE(v2, GroinBitmapPos);
  virtual DATA_BASE_VALUE(col16, ClothColor);
  virtual DATA_BASE_VALUE(col16, SkinColor);
  virtual DATA_BASE_VALUE(col16, CapColor);
  virtual DATA_BASE_VALUE(col16, HairColor);
  virtual DATA_BASE_VALUE(col16, EyeColor);
  virtual DATA_BASE_VALUE(col16, TorsoMainColor);
  virtual DATA_BASE_VALUE(col16, BeltColor);
  virtual DATA_BASE_VALUE(col16, BootColor);
  virtual DATA_BASE_VALUE(col16, TorsoSpecialColor);
  virtual DATA_BASE_VALUE(col16, ArmMainColor);
  virtual DATA_BASE_VALUE(col16, GauntletColor);
  virtual DATA_BASE_VALUE(col16, ArmSpecialColor);
  virtual DATA_BASE_VALUE(col16, LegMainColor);
  virtual DATA_BASE_VALUE(col16, LegSpecialColor);
  virtual DATA_BASE_TRUTH(IsNameable);
  virtual DATA_BASE_VALUE(col24, BaseEmitation); // devirtualize ASAP
  DATA_BASE_TRUTH(UsesLongArticle);
  DATA_BASE_VALUE(cfestring&, Adjective);
  DATA_BASE_TRUTH(UsesLongAdjectiveArticle);
  DATA_BASE_VALUE(cfestring&, NameSingular);
  DATA_BASE_VALUE(cfestring&, NamePlural);
  DATA_BASE_VALUE(cfestring&, PostFix);
  DATA_BASE_VALUE(int, ArticleMode);
  DATA_BASE_TRUTH(CanZap);
  virtual DATA_BASE_TRUTH(IsPolymorphable);
  DATA_BASE_VALUE(int, BaseUnarmedStrength);
  DATA_BASE_VALUE(int, BaseBiteStrength);
  DATA_BASE_VALUE(int, BaseKickStrength);
  DATA_BASE_VALUE(int, AttackStyle);
  DATA_BASE_TRUTH(CanUseEquipment);
  DATA_BASE_TRUTH(CanKick);
  DATA_BASE_TRUTH(CanTalk);
  DATA_BASE_TRUTH(CanBeWished);
  DATA_BASE_VALUE(long, ClassStates);
  DATA_BASE_VALUE(const fearray<festring>&, Alias);
  DATA_BASE_TRUTH(CreateGolemMaterialConfigurations);
  DATA_BASE_VALUE(const fearray<long>&, KnownCWeaponSkills);
  DATA_BASE_VALUE(const fearray<long>&, CWeaponSkillHits);
  DATA_BASE_VALUE(int, RightSWeaponSkillHits);
  DATA_BASE_VALUE(int, LeftSWeaponSkillHits);
  DATA_BASE_VALUE(int, PanicLevel);
  DATA_BASE_TRUTH(CanBeCloned);
  DATA_BASE_VALUE(cfestring&, DefaultName);
  DATA_BASE_VALUE(const fearray<festring>&, FriendlyReplies);
  DATA_BASE_VALUE(const fearray<festring>&, HostileReplies);
  DATA_BASE_VALUE(int, FleshMaterial);
  virtual DATA_BASE_TRUTH(HasALeg);
  virtual DATA_BASE_VALUE(cfestring&, DeathMessage);
  DATA_BASE_VALUE(int, HPRequirementForGeneration);
  DATA_BASE_TRUTH(IsExtraCoward);
  DATA_BASE_TRUTH(SpillsBlood);
  DATA_BASE_TRUTH(HasEyes);
  virtual DATA_BASE_TRUTH(HasHead);
  DATA_BASE_TRUTH(CanThrow);
  DATA_BASE_TRUTH(UsesNutrition);
  DATA_BASE_VALUE(int, AttackWisdomLimit);
  DATA_BASE_TRUTH(IsUnique);
  DATA_BASE_VALUE(int, AttachedGod);
  DATA_BASE_TRUTH(BodyPartsDisappearWhenSevered);
  DATA_BASE_VALUE(int, Frequency);
  DATA_BASE_TRUTH(CanBeConfused);
  DATA_BASE_TRUTH(CanApply);
  DATA_BASE_VALUE(v2, WieldedPosition);
  virtual DATA_BASE_VALUE(int, NaturalSparkleFlags);
  DATA_BASE_TRUTH(IgnoreDanger);
  DATA_BASE_TRUTH(BiteCapturesBodyPart);
  DATA_BASE_TRUTH(IsPlant);
  DATA_BASE_TRUTH(DestroysWalls);
  DATA_BASE_TRUTH(IsRooted);
  DATA_BASE_VALUE(int, BloodMaterial);
  DATA_BASE_VALUE(int, VomitMaterial);
  DATA_BASE_TRUTH(AutomaticallySeen);
  DATA_BASE_VALUE(ulong, DefaultCommandFlags);
  DATA_BASE_TRUTH(WillCarryItems);
  DATA_BASE_VALUE(int, SweatMaterial);
  DATA_BASE_TRUTH(Sweats);
  DATA_BASE_TRUTH(IsImmuneToItemTeleport);
  DATA_BASE_TRUTH(AlwaysUseMaterialAttributes);
  DATA_BASE_TRUTH(IsEnormous);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkAdjectiveAttribute);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkSubstantiveAttribute);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkPrefix);
  DATA_BASE_VALUE(const fearray<festring>&, ScienceTalkName);
  DATA_BASE_VALUE(int, ScienceTalkPossibility);
  DATA_BASE_VALUE(int, ScienceTalkIntelligenceModifier);
  DATA_BASE_VALUE(int, ScienceTalkWisdomModifier);
  DATA_BASE_VALUE(int, ScienceTalkCharismaModifier);
  DATA_BASE_VALUE(int, ScienceTalkIntelligenceRequirement);
  DATA_BASE_VALUE(int, ScienceTalkWisdomRequirement);
  DATA_BASE_VALUE(int, ScienceTalkCharismaRequirement);
  DATA_BASE_TRUTH(IsExtraFragile);
  DATA_BASE_TRUTH(IsImmuneToStickiness);
  DATA_BASE_VALUE(festring, ForceVomitMessage);
  DATA_BASE_TRUTH(CanChoke);
  DATA_BASE_VALUE(int, DisplacePriority);
  DATA_BASE_VALUE(cfestring&, RunDescriptionLineOne);
  DATA_BASE_VALUE(cfestring&, RunDescriptionLineTwo);
  DATA_BASE_TRUTH(ForceCustomStandVerb);
  DATA_BASE_TRUTH(VomittingIsUnhealthy);
  DATA_BASE_TRUTH(AllowPlayerToChangeEquipment);
  DATA_BASE_VALUE(int, TamingDifficulty);
  DATA_BASE_TRUTH(IsMasochist);
  DATA_BASE_TRUTH(IsSadist);
  DATA_BASE_TRUTH(IsCatacombCreature);
  DATA_BASE_TRUTH(CreateUndeadConfigurations);
  DATA_BASE_TRUTH(UndeadVersions);
  DATA_BASE_VALUE(int, UndeadAttributeModifier);
  DATA_BASE_VALUE(int, UndeadVolumeModifier);
  DATA_BASE_TRUTH(UndeadCopyMaterials);
  DATA_BASE_TRUTH(CanBeGeneratedOnlyInTheCatacombs);
  DATA_BASE_TRUTH(IsAlcoholic);
  DATA_BASE_TRUTH(IsImmuneToWhipOfThievery);
  int GetType() const { return GetProtoType()->GetIndex(); }
  void TeleportRandomly(truth = false);
  truth TeleportNear(character*);
  virtual void InitSpecialAttributes() { }
  virtual void Kick(lsquare*, int, truth = false) = 0;
  virtual int GetAttribute(int, truth = true) const;
  virtual truth EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  truth RawEditAttribute(double&, int) const;
  void DrawPanel(truth) const;
  virtual int DrawStats(truth) const = 0;
  virtual int GetCarryingStrength() const = 0;
  static truth DamageTypeAffectsInventory(int);
  virtual int GetRandomStepperBodyPart() const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  virtual int CheckForBlock(character*, item*, double, int, int, int);
  int CheckForBlockWithArm(character*, item*, arm*, double, int, int, int);
  void AddBlockMessage(ccharacter*, citem*, cfestring&, truth) const;
  character* GetPolymorphBackup() const { return PolymorphBackup; }
  void SetPolymorphBackup(character* What) { PolymorphBackup = What; }
  cweaponskill* GetCWeaponSkill(int I) const { return &CWeaponSkill[I]; }
  virtual truth AddSpecialSkillInfo(felist&) const { return false; }
  virtual truth CheckBalance(double);
  long GetStateAPGain(long) const;
  virtual long GetMoveAPRequirement(int) const;
  virtual void SignalEquipmentAdd(int);
  virtual void SignalEquipmentRemoval(int, citem*);
  void BeginTemporaryState(long, int);
  void GainIntrinsic(long);
  void HandleStates();
  void PrintBeginPolymorphControlMessage() const;
  void PrintEndPolymorphControlMessage() const;
  void PrintBeginLifeSaveMessage() const;
  void PrintEndLifeSaveMessage() const;
  void PrintBeginLycanthropyMessage() const;
  void PrintEndLycanthropyMessage() const;
  void PrintBeginHasteMessage() const;
  void PrintEndHasteMessage() const;
  void PrintBeginSlowMessage() const;
  void PrintEndSlowMessage() const;
  void PrintBeginSearchingMessage() const;
  void PrintEndSearchingMessage() const;
  void PrintBeginHiccupsMessage() const;
  void PrintEndHiccupsMessage() const;
  void EndPolymorph();
  character* ForceEndPolymorph();
  void LycanthropyHandler();
  void SearchingHandler();
  void SaveLife();
  void BeginInvisibility();
  void BeginInfraVision();
  void BeginESP();
  void EndInvisibility();
  void EndInfraVision();
  void EndESP();
  void HiccupsHandler();
  character* PolymorphRandomly(int, int, int);
  virtual truth EquipmentEasilyRecognized(int) const { return true; }
  void StartReading(item*, long);
  void DexterityAction(int);
  void IntelligenceAction(int);
  virtual void SWeaponSkillTick() { }
  void PrintBeginInvisibilityMessage() const;
  void PrintEndInvisibilityMessage() const;
  void PrintBeginInfraVisionMessage() const;
  void PrintEndInfraVisionMessage() const;
  void PrintBeginESPMessage() const;
  void PrintEndESPMessage() const;
  truth CanBeSeenByPlayer(truth = false, truth = false) const;
  truth CanBeSeenBy(ccharacter*, truth = false, truth = false) const;
  void AttachBodyPart(bodypart*);
  truth HasAllBodyParts() const;
  bodypart* FindRandomOwnBodyPart(truth) const;
  bodypart* GenerateRandomBodyPart();
  void PrintBeginPoisonedMessage() const;
  void PrintEndPoisonedMessage() const;
  truth IsWarm() const;
  void CalculateEquipmentState();
  void Draw(blitdata&) const;
  virtual void DrawBodyParts(blitdata&) const;
  god* GetMasterGod() const;
  void PoisonedHandler();
  void PrintBeginTeleportMessage() const;
  void PrintEndTeleportMessage() const;
  void TeleportHandler();
  void PrintEndTeleportControlMessage() const;
  void PrintBeginTeleportControlMessage() const;
  void PolymorphHandler();
  void PrintEndPolymorphMessage() const;
  void PrintBeginPolymorphMessage() const;
  virtual void DisplayStethoscopeInfo(character*) const;
  virtual truth CanUseStethoscope(truth) const;
  virtual truth IsUsingArms() const;
  virtual truth IsUsingLegs() const;
  virtual truth IsUsingHead() const;
  dungeon* GetDungeon() const { return static_cast<level*>(GetSquareUnder()->GetArea())->GetDungeon(); }
  level* GetLevel() const { return static_cast<level*>(GetSquareUnder()->GetArea()); }
  area* GetArea() const { return GetSquareUnder()->GetArea(); }
  virtual square* GetNeighbourSquare(int) const;
  virtual lsquare* GetNeighbourLSquare(int) const;
  virtual wsquare* GetNeighbourWSquare(int) const;
  stack* GetStackUnder(int I = 0) const { return static_cast<lsquare*>(GetSquareUnder(I))->GetStack(); }
  square* GetNearSquare(v2 Pos) const { return GetSquareUnder()->GetArea()->GetSquare(Pos); }
  square* GetNearSquare(int x, int y) const { return GetSquareUnder()->GetArea()->GetSquare(x, y); }
  lsquare* GetNearLSquare(v2 Pos) const { return static_cast<lsquare*>(GetSquareUnder()->GetArea()->GetSquare(Pos)); }
  lsquare* GetNearLSquare(int x, int y) const { return static_cast<lsquare*>(GetSquareUnder()->GetArea()->GetSquare(x, y)); }
  wsquare* GetNearWSquare(v2) const;
  wsquare* GetNearWSquare(int, int) const;
  v2 GetPos(int I = 0) const { return GetSquareUnder(I)->GetPos(); }
  square* GetSquareUnder(int I = 0) const { return !MotherEntity ? SquareUnder[I] : MotherEntity->GetSquareUnderEntity(I); }
  virtual square* GetSquareUnderEntity(int I = 0) const { return GetSquareUnder(I); }
  lsquare* GetLSquareUnder(int I = 0) const { return static_cast<lsquare*>(GetSquareUnder(I)); }
  int GetRandomNonVitalBodyPart() const;
  void TeleportSomePartsAway(int);
  virtual void SignalVolumeAndWeightChange();
  virtual void SignalBodyPartVolumeAndWeightChange() { }
  void CalculateVolumeAndWeight();
  long GetVolume() const { return Volume; }
  long GetBodyVolume() const { return BodyVolume; }
  long GetWeight() const { return Weight; }
  long GetCarriedWeight() const { return CarriedWeight; }
  virtual void SignalEmitationIncrease(col24);
  virtual void SignalEmitationDecrease(col24);
  void CalculateEmitation();
  void CalculateAll();
  void CalculateHP();
  void CalculateMaxHP();
  int GetHP() const { return HP; }
  int GetMaxHP() const { return MaxHP; }
  void CalculateBodyPartMaxHPs(ulong = MAY_CHANGE_HPS|CHECK_USABILITY);
  truth IsInitializing() const { return Flags & C_INITIALIZING; }
  truth IsInNoMsgMode() const { return Flags & C_IN_NO_MSG_MODE; }
  truth ActivateRandomState(int, int, long = 0);
  long GetRandomState(int) const;
  truth GainRandomIntrinsic(int);
  virtual void CalculateBattleInfo() = 0;
  void CalculateBurdenState();
  virtual void CalculateDodgeValue();
  virtual void CalculateBodyParts() { BodyParts = 1; }
  virtual void CalculateAllowedWeaponSkillCategories();
  int GetBodyParts() const { return BodyParts; }
  int GetAllowedWeaponSkillCategories() const { return AllowedWeaponSkillCategories; }
  double GetRelativeDanger(ccharacter*, truth = false) const;
  double GetTimeToDie(ccharacter*, int, double, truth, truth) const;
  virtual double GetTimeToKill(ccharacter*, truth) const = 0;
  virtual void AddSpecialEquipmentInfo(festring&, int) const { }
  virtual festring GetBodyPartName(int, truth = false) const;
  item* SearchForItem(ulong) const;
  truth SearchForItem(citem*) const;
  item* SearchForItem(const sweaponskill*) const;
  truth ContentsCanBeSeenBy(ccharacter*) const;
  festring GetBeVerb() const;
  virtual void CreateBlockPossibilityVector(blockvector&, double) const { }
  virtual truth SpecialUnarmedEffect(character*, v2, int, int, truth) { return false; }
  virtual truth SpecialKickEffect(character*, v2, int, int, truth) { return false; }
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth) { return false; }
  truth HitEffect(character*, item*, v2, int, int, int, truth);
  void WeaponSkillHit(item*, int, int);
  character* Duplicate(ulong = 0);
  room* GetRoom(int I = 0) const { return GetLSquareUnder(I)->GetRoom(); }
  truth TryToEquip(item*);
  truth TryToConsume(item*);
  void UpdateESPLOS() const;
  int GetCWeaponSkillLevel(citem*) const;
  virtual int GetSWeaponSkillLevel(citem*) const { return 0; }
  void PrintBeginPanicMessage() const;
  void PrintEndPanicMessage() const;
  void CheckPanic(int);
  character* DuplicateToNearestSquare(character*, ulong = 0);
  void SignalSpoil();
  void SignalSpoilLevelChange();
  virtual truth UseMaterialAttributes() const = 0;
  truth IsPolymorphed() const { return Flags & C_POLYMORPHED; }
  truth IsInBadCondition() const { return HP * 3 < MaxHP; }
  truth IsInBadCondition(int HP) const { return HP * 3 < MaxHP; }
  int GetCondition() const;
  void UpdatePictures();
  truth CanHeal() const;
  void SetGoingTo(v2);
  int GetRelation(ccharacter*) const;
  truth CalculateAttributeBonuses();
  void ApplyEquipmentAttributeBonuses(item*);
  void ReceiveAntidote(long);
  void AddAntidoteConsumeEndMessage() const;
  truth IsDead() const;
  void AddOriginalBodyPartID(int, ulong);
  void AddToInventory(const fearray<contentscript<item> >&, int);
  truth HasHadBodyPart(citem*) const;
  void ProcessAndAddMessage(festring) const;
  virtual truth CheckZap();
  void SetEndurance(int);
  void SetPerception(int);
  void SetIntelligence(int);
  void SetWisdom(int);
  void SetWillPower(int);
  void SetCharisma(int);
  void SetMana(int);
  void DamageAllItems(character*, int, int);
  truth Equips(citem*) const;
  void PrintBeginConfuseMessage() const;
  void PrintEndConfuseMessage() const;
  v2 ApplyStateModification(v2) const;
  void AddConfuseHitMessage() const;
  item* SelectFromPossessions(cfestring&, sorter = 0);
  truth SelectFromPossessions(itemvector&, cfestring&, int, sorter = 0);
  truth EquipsSomething(sorter = 0);
  truth CheckTalk();
  virtual truth CanCreateBodyPart(int) const { return true; }
  virtual truth HandleCharacterBlockingTheWay(character*, v2, int) { return false; }
  virtual festring& ProcessMessage(festring&) const;
  virtual truth IsHumanoid() const { return false; }
  virtual truth IsHuman() const { return false; }
  truth IsOnGround() const;
  virtual truth CheckIfEquipmentIsNotUsable(int) const { return false; }
  virtual truth MoveTowardsHomePos();
  virtual void SetWayPoints(const fearray<packv2>&) { }
  truth TryToChangeEquipment(stack*, stack*, int);
  void PrintBeginParasitizedMessage() const;
  void PrintEndParasitizedMessage() const;
  void ParasitizedHandler();
  truth CanFollow() const;
  truth LeftOversAreUnique() const;
  virtual festring GetKillName() const;
  festring GetPanelName() const;
  virtual void AddSpecialStethoscopeInfo(felist&) const = 0;
  virtual item* GetPairEquipment(int) const { return 0; }
  bodypart* HealHitPoint();
  void CreateHomeData();
  room* GetHomeRoom() const;
  truth HomeDataIsValid() const;
  void SetHomePos(v2);
  void RemoveHomeData();
  ulong GetID() const { return ID; }
  void AddESPConsumeMessage() const;
  const std::list<ulong>& GetOriginalBodyPartID(int) const;
  void GetHitByExplosion(const explosion*, int);
  truth CanBePoisoned() const { return TorsoIsAlive(); }
  truth CanBeParasitized() const { return TorsoIsAlive(); }
  void SortAllItems(const sortdata&);
  character* GetRandomNeighbourEnemy() const;
  void Search(int);
  character* GetRandomNeighbour(int = (HOSTILE|UNCARING|FRIEND)) const;
  virtual truth IsRetreating() const;
  virtual truth IsMushroom() const { return false; }
  void ResetStates();
  virtual head* Behead() { return 0; }
  void PrintBeginGasImmunityMessage() const;
  void PrintEndGasImmunityMessage() const;
  void ShowAdventureInfo() const;
  virtual truth BoundToUse(citem*, int) const { return false; }
  virtual truth IsBananaGrower() const { return false; }
  virtual int GetRandomApplyBodyPart() const;
#ifdef WIZARD
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const = 0;
  virtual void AddDefenceInfo(felist&) const;
  virtual void DetachBodyPart();
#endif
  void ReceiveHolyBanana(long);
  void AddHolyBananaConsumeEndMessage() const;
  virtual truth PreProcessForBone();
  truth PostProcessForBone(double&, int&);
  truth PostProcessForBone();
  virtual void FinalProcessForBone();
  virtual truth EditAllAttributes(int);
  virtual void SetSoulID(ulong);
  virtual truth SuckSoul(character*) { return false; }
  virtual truth MustBeRemovedFromBone() const;
  truth TorsoIsAlive() const { return GetTorso()->IsAlive(); }
  truth PictureUpdatesAreForbidden() const { return Flags & C_PICTURE_UPDATES_FORBIDDEN; }
  virtual int GetUsableArms() const { return 0; }
  truth IsPet() const;
  virtual void PutTo(v2);
  void PutTo(lsquare*);
  void PutNear(v2);
  void PutToOrNear(v2);
  virtual void Remove();
  truth IsSmall() const { return SquaresUnder == 1; }
  truth IsOver(v2) const;
  truth IsOver(citem*) const;
  truth SquareUnderCanBeSeenByPlayer(truth) const;
  truth SquareUnderCanBeSeenBy(ccharacter*, truth) const;
  int GetDistanceSquareFrom(ccharacter*) const;
  virtual truth CanTheoreticallyMoveOn(const lsquare*) const;
  virtual truth CanMoveOn(const lsquare*) const;
  virtual truth CanMoveOn(const square*) const;
  truth CanMoveOn(const olterrain*) const;
  truth CanMoveOn(const oterrain*) const;
  truth IsMainPos(v2 What) const { return GetPos() == What; }
  virtual void CalculateSquaresUnder() { SquaresUnder = 1; }
  int GetSquaresUnder() const { return SquaresUnder; }
  virtual int GetSquareIndex(v2) const { return 0; }
  virtual int GetNeighbourSquares() const { return 8; }
  virtual int GetExtendedNeighbourSquares() const { return 9; }
  virtual int CalculateNewSquaresUnder(lsquare**, v2) const;
  virtual truth IsFreeForMe(square*) const;
  void SendNewDrawRequest() const;
  square* GetNaturalNeighbourSquare(int I) const { return character::GetNeighbourSquare(I); }
  lsquare* GetNaturalNeighbourLSquare(int I) const { return character::GetNeighbourLSquare(I); }
  void SignalStepFrom(lsquare**);
  virtual void SpecialBodyDefenceEffect(character*, bodypart*, int) { }
  virtual int GetSumOfAttributes() const;
  truth IsGoingSomeWhere() const { return GoingTo != ERROR_V2; }
  virtual truth CreateRoute();
  void TerminateGoingTo();
  virtual truth IsSpy() const { return false; }
  truth CheckForFood(int);
  truth CheckForFoodInSquare(v2);
  virtual truth CheckIfSatiated() { return GetNP() > SATIATED_LEVEL; }
  virtual void SignalNaturalGeneration() { }
  virtual truth IsBunny() const { return false; }
  void SetConfig(int, int = 0);
  bodypartslot* GetBodyPartSlot(int I) { return &BodyPartSlot[I]; }
  virtual truth CheckConsume(cfestring&) const;
  virtual int GetTameSymbolSquareIndex() const { return 0; }
  virtual int GetFlySymbolSquareIndex() const { return 0; }
  virtual int GetSwimmingSymbolSquareIndex() const { return 0; }
  virtual int GetUnconsciousSymbolSquareIndex() const { return 0; }
  virtual truth PlaceIsIllegal(v2 Pos, v2 Illegal) const { return Pos == Illegal; }
  liquid* CreateBlood(long) const;
  void SpillFluid(character*, liquid*, int = 0);
  virtual void StayOn(liquid*);
  virtual head* GetVirtualHead() const { return 0; }
  truth IsAlly(ccharacter*) const;
  virtual truth CanVomit() const { return TorsoIsAlive(); }
  ulong GetLastAcidMsgMin() const { return LastAcidMsgMin; }
  void SetLastAcidMsgMin(ulong What) { LastAcidMsgMin = What; }
  virtual truth AllowSpoil() const { return false; }
  void Disappear(corpse*, cchar*, truth (item::*)() const);
  void ResetSpoiling();
  virtual character* GetLeader() const;
  int GetMoveType() const;
  virtual truth IsSumoWrestler() const { return false; }
  void InitMaterials(const materialscript*, const materialscript*, truth) { }
  item* SearchForItem(ccharacter*, sorter) const;
  virtual character* CreateZombie() const { return 0; }
  virtual festring GetZombieDescription() const;
  virtual truth CanAttack() const { return true; }
  truth DetectMaterial(cmaterial*) const;
  truth CheckIfTooScaredToHit(ccharacter*) const;
  void PrintBeginLevitationMessage() const;
  void PrintEndLevitationMessage() const;
  void EditDealExperience(long);
  int RawEditExperience(double&, double, double, double) const;
  virtual void LeprosyHandler();
  virtual void TryToInfectWithLeprosy(ccharacter*);
  void PrintBeginLeprosyMessage() const;
  void PrintEndLeprosyMessage() const;
  void SignalGeneration();
  void CheckIfSeen();
  void SignalSeen();
  truth HasBeenSeen() const;
  int GetPolymorphIntelligenceRequirement() const;
  void RemoveAllItems();
  int CalculateWeaponSkillHits(ccharacter*) const;
  void DonateEquipmentTo(character*);
  void ReceivePeaSoup(long);
  void AddPeaSoupConsumeEndMessage() const;
  void CalculateMaxStamina();
  void EditStamina(int, truth);
  void RegenerateStamina();
  void BeginPanic();
  void EndPanic();
  int GetTirednessState() const;
  int GetStamina() const { return Stamina; }
  int GetMaxStamina() const { return MaxStamina; }
  void SetGenerationDanger(double What) { GenerationDanger = What; }
  double GetGenerationDanger() const { return GenerationDanger; }
  void ReceiveBlackUnicorn(long);
  void ReceiveGrayUnicorn(long);
  void ReceiveWhiteUnicorn(long);
  void DecreaseStateCounter(long, int);
  truth IsImmuneToLeprosy() const;
  bodypart* SearchForOriginalBodyPart(int) const;
  void SetLifeExpectancy(int, int);
  virtual void DuplicateEquipment(character*, ulong);
  virtual void SignalDisappearance();
  virtual truth HornOfFearWorks() const;
  virtual truth CanHear() const;
  void BeginLeprosy();
  void EndLeprosy();
  void ReceiveOmmelCerumen(long);
  void ReceiveOmmelSweat(long);
  void ReceiveOmmelTears(long);
  void ReceiveOmmelSnot(long);
  void ReceiveOmmelBone(long);
  truth IsSameAs(ccharacter*) const;
  ulong GetCommandFlags() const;
  void SetCommandFlags(ulong What) { CommandFlags = What; }
  ulong GetPossibleCommandFlags() const;
  ulong GetConstantCommandFlags() const;
  virtual truth AllowEquipment(citem*, int) const { return true; }
  truth ChatMenu();
  truth ChangePetEquipment();
  truth TakePetItems();
  truth GivePetItems();
  truth IssuePetCommands();
  truth ChatIdly();
  truth EquipmentScreen(stack*, stack*);
  ulong GetManagementFlags() const;
  cchar* GetVerbalBurdenState() const;
  col16 GetVerbalBurdenStateColor() const;
  virtual int GetAttributeAverage() const;
  virtual cfestring& GetStandVerb() const;
  virtual truth CheckApply() const;
  virtual truth CanForceVomit() const { return CanVomit(); }
  void EndLevitation();
  virtual truth CanMove() const;
  void CalculateEnchantments();
  truth GetNewFormForPolymorphWithControl(character*&);
  liquid* CreateSweat(long) const;
  truth IsTemporary() const;
  truth TeleportRandomItem(truth);
  truth HasClearRouteTo(v2) const;
  virtual truth IsTransparent() const;
  void SignalPossibleTransparencyChange();
  int GetCursorData() const;
  void TryToName();
  double GetSituationDanger(ccharacter*, v2, v2, truth) const;
  virtual void ModifySituationDanger(double&) const;
  void LycanthropySituationDangerModifier(double&) const;
  void PoisonedSituationDangerModifier(double&) const;
  void PolymorphingSituationDangerModifier(double&) const;
  void PanicSituationDangerModifier(double&) const;
  void ConfusedSituationDangerModifier(double&) const;
  void ParasitizedSituationDangerModifier(double&) const;
  void LeprosySituationDangerModifier(double&) const;
  void HiccupsSituationDangerModifier(double&) const;
  truth TryToTalkAboutScience();
  truth IsUsingWeaponOfCategory(int) const;
  virtual truth IsKamikazeDwarf() const { return false; }
  void AddRandomScienceName(festring&) const;
  truth IsStuck() const { return truth(TrapData); }
  festring GetTrapDescription() const;
  truth TryToUnStickTraps(v2);
  void RemoveTrap(ulong);
  void AddTrap(ulong, ulong);
  truth IsStuckToTrap(ulong) const;
  void RemoveTraps();
  void RemoveTraps(int);
  int RandomizeHurtBodyPart(ulong) const;
  virtual int RandomizeTryToUnStickBodyPart(ulong) const { return NONE_INDEX; }
  truth BodyPartIsStuck(int) const;
  void PrintAttribute(cchar*, int, int, int) const;
  virtual truth AllowUnconsciousness() const;
  truth CanPanic() const;
  int GetRandomBodyPart(ulong = ALL_BODYPART_FLAGS) const;
  virtual truth CanChokeOnWeb(web*) const { return CanChoke(); }
  virtual truth BrainsHurt() const { return false; }
  truth IsSwimming() const;
  truth IsAnimated() const;
  virtual truth IsPlayerKind() const { return false; }
  truth HasBodyPart(sorter) const;
  truth PossessesItem(sorter) const;
  truth IsFlying() const { return GetMoveType() & FLY; }
  virtual cchar* GetRunDescriptionLine(int) const;
  void VomitAtRandomDirection(int);
  virtual truth SpecialSaveLife() { return false; }
  void RemoveLifeSavers();
  virtual ccharacter* FindCarrier() const;
  virtual cchar* GetNormalDeathMessage() const;
  virtual bool IsConscious() const;
  void ForcePutNear(v2);
  virtual void ApplySpecialAttributeBonuses() { }
  void ReceiveMustardGas(int, long);
  void ReceiveMustardGasLiquid(int, long);
  truth IsBadPath(v2) const;
  double& GetExpModifierRef(expid);
  truth ForgetRandomThing(); 
  void ApplyAllGodsKnownBonus();
  item* GiveMostExpensiveItem(character*);
  void ReceiveItemAsPresent(item*);
  item* FindMostExpensiveItem() const;
  void ReceiveSirenSong(character* Siren);
  character* GetNearestEnemy() const;
  truth IsInfectedByMindWorm() const { return !CounterToMindWormHatch; }
  void SetCounterToMindWormHatch(int What) { CounterToMindWormHatch = What; }
  truth MindWormCanPenetrateSkull(mindworm*) const;
  truth CanTameWithDulcis(const character*) const;
  truth CanTameWithLyre(const character*) const;
  truth CanTameWithScroll(const character*) const;
  truth IsCharmable() const { return GetTamingDifficulty() != NO_TAMING; }
  truth CheckSadism();
  virtual truth HasSadistAttackMode() const { return IsUsingLegs(); }
  truth CheckForBeverage();
  void Haste();
  void Slow();
 protected:
  static truth DamageTypeDestroysBodyPart(int);
  virtual void LoadSquaresUnder();
  virtual bodypart* MakeBodyPart(int) const;
  virtual void SpecialTurnHandler() { }
  void Initialize(int, int);
  virtual void PostConstruct() { }
  void LoadDataBaseStats();
  virtual v2 GetBodyPartBitmapPos(int, truth = false) const;
  virtual col16 GetBodyPartColorA(int, truth = false) const;
  virtual col16 GetBodyPartColorB(int, truth = false) const;
  virtual col16 GetBodyPartColorC(int, truth = false) const;
  virtual col16 GetBodyPartColorD(int, truth = false) const;
  virtual int GetBodyPartSparkleFlags(int) const;
  virtual long GetBodyPartSize(int, int) const;
  virtual long GetBodyPartVolume(int) const;
  void UpdateBodyPartPicture(int I, truth);
  int ChooseBodyPartToReceiveHit(double, double);
  virtual void CreateBodyParts(int);
  virtual material* CreateBodyPartMaterial(int, long) const;
  virtual truth ShowClassDescription() const { return true; }
  void SeekLeader(ccharacter*);
  virtual truth CheckForUsefulItemsOnGround(truth = true);
  truth CheckForDoors();
  truth CheckForEnemies(truth, truth, truth, truth = false);
  truth FollowLeader(character*);
  void StandIdleAI();
  virtual void CreateCorpse(lsquare*);
  void GetPlayerCommand();
  virtual void GetAICommand();
  truth MoveTowardsTarget(truth);
  virtual cchar* FirstPersonUnarmedHitVerb() const;
  virtual cchar* FirstPersonCriticalUnarmedHitVerb() const;
  virtual cchar* ThirdPersonUnarmedHitVerb() const;
  virtual cchar* ThirdPersonCriticalUnarmedHitVerb() const;
  virtual cchar* FirstPersonKickVerb() const;
  virtual cchar* FirstPersonCriticalKickVerb() const;
  virtual cchar* ThirdPersonKickVerb() const;
  virtual cchar* ThirdPersonCriticalKickVerb() const;
  virtual cchar* FirstPersonBiteVerb() const;
  virtual cchar* FirstPersonCriticalBiteVerb() const;
  virtual cchar* ThirdPersonBiteVerb() const;
  virtual cchar* ThirdPersonCriticalBiteVerb() const;
  virtual cchar* UnarmedHitNoun() const;
  virtual cchar* KickNoun() const;
  virtual cchar* BiteNoun() const;
  virtual truth AttackIsBlockable(int) const { return true; }
  virtual truth AttackMayDamageArmor() const { return true; }
  virtual int GetSpecialBodyPartFlags(int) const { return ST_NORMAL; }
  virtual int GetBodyPartWobbleData(int) const { return 0; }
  virtual int ModifyBodyPartHitPreference(int, int Modifier) const { return Modifier; }
  virtual int ModifyBodyPartToHitChance(int, int Chance) const { return Chance; }
  virtual truth CanPanicFromSeveredBodyPart() const { return true; }
  virtual void SpecialBodyPartSeverReaction() { }
  truth AttackAdjacentEnemyAI();
  double RandomizeBabyExperience(double);
  static truth IsLimbIndex(int);
  virtual truth AllowExperience() const { return true; }
  virtual const prototype* FindProtoType() const { return &ProtoType; }
  static const prototype ProtoType;
  stack* Stack;
  long NP, AP;
  long TemporaryState;
  int TemporaryStateCounter[STATES];
  team* Team;
  v2 GoingTo;
  long Money;
  std::list<character*>::iterator TeamIterator;
  bodypartslot* BodyPartSlot;
  festring AssignedName;
  action* Action;
  const database* DataBase;
  double BaseExperience[BASE_ATTRIBUTES];
  std::list<ulong>* OriginalBodyPartID;
  entity* MotherEntity;
  character* PolymorphBackup;
  cweaponskill* CWeaponSkill;
  long EquipmentState;
  square** SquareUnder;
  long Volume;
  long Weight;
  long CarriedWeight;
  long BodyVolume;
  int HP;
  int MaxHP;
  int BurdenState;
  double DodgeValue;
  int AllowedWeaponSkillCategories;
  int BodyParts;
  long RegenerationCounter;
  int AttributeBonus[BASE_ATTRIBUTES];
  int CarryingBonus;
  homedata* HomeData;
  ulong ID;
  int SquaresUnder;
  std::vector<v2> Route;
  std::set<v2> Illegal;
  ulong LastAcidMsgMin;
  int Stamina;
  int MaxStamina;
  int BlocksSinceLastTurn;
  double GenerationDanger;
  ulong CommandFlags;
  ulong WarnFlags;
  int ScienceTalks;
  trapdata* TrapData;
  expmodifiermap ExpModifierMap;
  int CounterToMindWormHatch;
};

#ifdef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__
#define CHARACTER_PROTO(name, base)\
template<> const characterprototype\
  name##sysbase::ProtoType(&base::ProtoType,\
			   (characterspawner)(&name##sysbase::Spawn),\
			   (charactercloner)(&name##sysbase::Clone), #name);
#else
#define CHARACTER_PROTO(name, base)
#endif

#define CHARACTER(name, base)\
class name;\
typedef sysbase<name, base, characterprototype> name##sysbase;\
CHARACTER_PROTO(name, base)\
class name : public name##sysbase

#endif
