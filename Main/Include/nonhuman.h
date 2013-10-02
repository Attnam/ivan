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

#ifndef __NONHUMAN_H__
#define __NONHUMAN_H__

#include "char.h"

CHARACTER(nonhumanoid, character)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void CalculateUnarmedDamage();
  void CalculateKickDamage();
  void CalculateBiteDamage();
  void CalculateUnarmedToHitValue();
  void CalculateKickToHitValue();
  void CalculateBiteToHitValue();
  void CalculateUnarmedAPCost();
  void CalculateKickAPCost();
  void CalculateBiteAPCost();
  double GetUnarmedDamage() const { return UnarmedDamage; }
  int GetUnarmedMinDamage() const;
  int GetUnarmedMaxDamage() const;
  double GetKickDamage() const { return KickDamage; }
  int GetKickMinDamage() const;
  int GetKickMaxDamage() const;
  double GetBiteDamage() const { return BiteDamage; }
  int GetBiteMinDamage() const;
  int GetBiteMaxDamage() const;
  double GetUnarmedToHitValue() const { return UnarmedToHitValue; }
  double GetKickToHitValue() const { return KickToHitValue; }
  double GetBiteToHitValue() const { return BiteToHitValue; }
  long GetUnarmedAPCost() const { return UnarmedAPCost; }
  long GetKickAPCost() const { return KickAPCost; }
  long GetBiteAPCost() const { return BiteAPCost; }
  virtual void Kick(lsquare*, int, truth = false);
  virtual truth Hit(character*, v2, int, int = 0);
  virtual void UnarmedHit(character*, v2, int, truth = false);
  virtual void InitSpecialAttributes();
  virtual double GetTimeToKill(ccharacter*, truth) const;
  virtual int GetAttribute(int, truth = true) const;
  virtual truth EditAttribute(int, int);
  virtual void EditExperience(int, double, double);
  virtual int DrawStats(truth) const;
  virtual void Bite(character*, v2, int, truth = false);
  virtual int GetCarryingStrength() const;
  virtual void CalculateBattleInfo();
  void CalculateUnarmedAttackInfo();
  void CalculateKickAttackInfo();
  void CalculateBiteAttackInfo();
  virtual truth UseMaterialAttributes() const;
  virtual void AddSpecialStethoscopeInfo(felist&) const;
  virtual truth EditAllAttributes(int);
  virtual void AddAttributeInfo(festring&) const;
  virtual void AddAttackInfo(felist&) const;
 protected:
  double StrengthExperience;
  double AgilityExperience;
  double UnarmedDamage;
  double KickDamage;
  double BiteDamage;
  double UnarmedToHitValue;
  double KickToHitValue;
  double BiteToHitValue;
  long UnarmedAPCost;
  long KickAPCost;
  long BiteAPCost;
};

CHARACTER(frog, nonhumanoid)
{
 public:
  virtual truth MoveRandomly() { return MoveRandomlyInRoom(); }
};

CHARACTER(billswill, nonhumanoid)
{
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual cchar* FirstPersonBiteVerb() const;
  virtual cchar* FirstPersonCriticalBiteVerb() const;
  virtual cchar* ThirdPersonBiteVerb() const;
  virtual cchar* ThirdPersonCriticalBiteVerb() const;
  virtual truth AttackIsBlockable(int) const { return false; }
  virtual truth AttackMayDamageArmor() const { return false; }
};

CHARACTER(mommo, nonhumanoid)
{
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual truth CanVomit() const { return true; }
  virtual void CreateCorpse(lsquare*);
  virtual truth Hit(character*, v2, int, int = 0);
  virtual void GetAICommand();
};

CHARACTER(canine, nonhumanoid)
{
};

CHARACTER(wolf, canine)
{
 protected:
  virtual col16 GetSkinColor() const;
};

CHARACTER(dog, canine)
{
 public:
  virtual truth Catches(item*);
  virtual void BeTalkedTo();
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
};

CHARACTER(spider, nonhumanoid)
{
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth);
  virtual void GetAICommand();
  virtual bodypart* MakeBodyPart(int) const;
};

CHARACTER(jackal, nonhumanoid)
{
};

CHARACTER(ass, nonhumanoid)
{
};

CHARACTER(bear, nonhumanoid)
{
};

CHARACTER(dolphin, nonhumanoid)
{
 protected:
  virtual int GetSpecialBodyPartFlags(int) const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
};

CHARACTER(bat, nonhumanoid)
{
 protected:
  virtual bodypart* MakeBodyPart(int) const;
};

CHARACTER(largecat, nonhumanoid)
{
 public:
  largecat() : Lives(7) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth SpecialSaveLife();
 protected:
  int Lives;
};

CHARACTER(largerat, nonhumanoid)
{
};

CHARACTER(mammoth, nonhumanoid)
{
};

CHARACTER(unicorn, nonhumanoid)
{
 public:
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual truth SpecialEnemySightedReaction(character*);
 protected:
  void MonsterTeleport(cchar*);
};

CHARACTER(lion, nonhumanoid)
{
};

CHARACTER(carnivorousplant, nonhumanoid)
{
 protected:
  virtual col16 GetTorsoSpecialColor() const;
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(buffalo, nonhumanoid)
{
};

CHARACTER(snake, nonhumanoid)
{
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth);
};

CHARACTER(ostrich, nonhumanoid)
{
 public:
  ostrich() : HasDroppedBananas(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual truth HandleCharacterBlockingTheWay(character*, v2, int);
  virtual void GetAICommand();
  truth HasDroppedBananas;
};

CHARACTER(chameleon, nonhumanoid)
{
 public:
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual truth SpecialEnemySightedReaction(character*);
 protected:
  virtual col16 GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
};

CHARACTER(floatingeye, nonhumanoid)
{
 public:
  floatingeye() : NextWayPoint(0) { }
  virtual truth Hit(character*, v2, int, int = 0);
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual void SetWayPoints(const fearray<packv2>&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsSpy() const { return true; }
  virtual truth CanAttack() const { return false; }
 protected:
  virtual void GetAICommand();
  std::vector<v2> WayPoints;
  uint NextWayPoint;
};

CHARACTER(eddy, nonhumanoid)
{
 public:
  virtual truth Hit(character*, v2, int, int = 0);
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
};

CHARACTER(mushroom, nonhumanoid)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SetSpecies(int);
  int GetSpecies() const { return Species; }
  virtual truth IsMushroom() const { return true; }
 protected:
  virtual void PostConstruct();
  virtual void GetAICommand();
  virtual col16 GetTorsoMainColor() const { return Species; }
  int Species;
};

CHARACTER(magicmushroom, mushroom)
{
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void GetAICommand();
};

CHARACTER(ghost, nonhumanoid)
{
 public:
  ghost() : Active(true) { }
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
  virtual int GetBodyPartWobbleData(int) const;
  virtual cchar* FirstPersonBiteVerb() const;
  virtual cchar* FirstPersonCriticalBiteVerb() const;
  virtual cchar* ThirdPersonBiteVerb() const;
  virtual cchar* ThirdPersonCriticalBiteVerb() const;
  virtual truth AttackIsBlockable(int) const { return false; }
  virtual truth AttackMayDamageArmor() const { return false; }
  virtual void GetAICommand();
  festring OwnerSoul;
  truth Active;
};

CHARACTER(twoheadedmoose, nonhumanoid)
{
 public:
  virtual truth Hit(character*, v2, int, int = 0);
};

CHARACTER(magpie, nonhumanoid)
{
 public:
  virtual void GetAICommand();
  virtual truth IsRetreating() const;
 protected:
  virtual cchar* FirstPersonBiteVerb() const;
  virtual cchar* FirstPersonCriticalBiteVerb() const;
  virtual cchar* ThirdPersonBiteVerb() const;
  virtual cchar* ThirdPersonCriticalBiteVerb() const;
};

CHARACTER(skunk, nonhumanoid)
{
 public:
  virtual void GetAICommand();
};

CHARACTER(invisiblestalker, nonhumanoid)
{
};

CHARACTER(largecreature, nonhumanoid)
{
 public:
  virtual void CalculateSquaresUnder() { SquaresUnder = 4; }
  virtual int GetSquareIndex(v2) const;
  virtual int GetNeighbourSquares() const { return 12; }
  virtual int GetExtendedNeighbourSquares() const { return 16; }
  virtual square* GetNeighbourSquare(int) const;
  virtual lsquare* GetNeighbourLSquare(int) const;
  virtual wsquare* GetNeighbourWSquare(int) const;
  virtual int CalculateNewSquaresUnder(lsquare**, v2) const;
  virtual truth IsFreeForMe(square*) const;
  virtual truth CanMoveOn(const lsquare*) const;
  virtual truth CanMoveOn(const square*) const;
  virtual void PutTo(v2);
  virtual void Remove();
  virtual truth CreateRoute();
  virtual truth CanTheoreticallyMoveOn(const lsquare*) const;
  virtual int GetFlySymbolSquareIndex() const { return 1; }
  virtual int GetSwimmingSymbolSquareIndex() const { return 3; }
  virtual int GetUnconsciousSymbolSquareIndex() const { return 2; }
  virtual truth PlaceIsIllegal(v2, v2) const;
  truth PartCanMoveOn(const lsquare*) const;
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  virtual void CreateCorpse(lsquare*);
  virtual void LoadSquaresUnder();
};

CHARACTER(elpuri, largecreature)
{
 public:
  elpuri() : Active(false) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth Hit(character*, v2, int, int = 0);
  virtual int ReceiveBodyPartDamage(character*, int, int, int, int = 8, truth = false, truth = false, truth = true, truth = false);
  virtual truth SpecialEnemySightedReaction(character*);
  virtual truth MustBeRemovedFromBone() const;
  virtual truth TryToRiseFromTheDead();
 protected:
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  truth Active;
};

CHARACTER(genetrixvesana, largecreature)
{
 public:
  genetrixvesana() : TurnsExisted(0) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void FinalProcessForBone();
 protected:
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  virtual truth MustBeRemovedFromBone() const;
  long TurnsExisted;
};

CHARACTER(hedgehog, nonhumanoid)
{
 public:
  virtual void SpecialBodyDefenceEffect(character*, bodypart*, int);
};

CHARACTER(bunny, nonhumanoid)
{
 public:
  virtual truth CheckIfSatiated() { return GetNP() > BLOATED_LEVEL; }
  virtual void SignalNaturalGeneration();
  virtual truth IsBunny() const { return true; }
  virtual truth Catches(item*);
 protected:
  truth CheckForMatePartner();
  virtual void GetAICommand();
};

CHARACTER(vladimir, largecreature)
{
 public:
  virtual truth MustBeRemovedFromBone() const;
  virtual col16 GetSkinColor() const;
  virtual void SpecialTurnHandler() { UpdatePictures(); }
};

CHARACTER(hattifattener, nonhumanoid)
{
 public:
  truth Hit(character*, v2, int, int = 0) { return false; }
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual int GetSpecialBodyPartFlags(int) const;
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
};

CHARACTER(blinkdog, dog)
{
 public:
  blinkdog();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int TakeHit(character*, item*, bodypart*, v2, double, double, int, int, int, truth, truth);
  virtual truth SpecialEnemySightedReaction(character*);
 protected:
  virtual bodypart* MakeBodyPart(int) const;
  void MonsterTeleport(cchar*);
  truth SummonFriend();
  int SummonModifier;
};

CHARACTER(mysticfrog, frog)
{
 public:
  virtual void GetAICommand();
 protected:
  virtual int GetBodyPartWobbleData(int) const;
  virtual bodypart* MakeBodyPart(int) const;
  int GetSpellAPCost() const { return 1500; }
};

CHARACTER(lobhse, largecreature)
{
 protected:
  virtual truth SpecialBiteEffect(character*, v2, int, int, truth);
  virtual void GetAICommand();
  virtual void CreateCorpse(lsquare*);
  virtual truth MustBeRemovedFromBone() const;
  virtual bodypart* MakeBodyPart(int) const;
};

CHARACTER(mindworm, nonhumanoid)
{
 protected:
  virtual void GetAICommand();
  virtual void TryToImplantLarvae(character*);
  virtual void PsiAttack(character*);
};
#endif
