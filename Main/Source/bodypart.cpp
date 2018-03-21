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

/* Compiled through itemset.cpp */

int bodypart::GetGraphicsContainerIndex() const { return GR_HUMANOID; }
int bodypart::GetArticleMode() const { return IsUnique() ? FORCE_THE : 0; }
truth bodypart::IsAlive() const { return MainMaterial->GetBodyFlags() & IS_ALIVE; }
int bodypart::GetSpecialFlags() const { return SpecialFlags|ST_OTHER_BODYPART; }
col16 bodypart::GetMaterialColorA(int) const { return GetMainMaterial()->GetSkinColor(); }
truth bodypart::IsWarm() const { return MainMaterial->GetBodyFlags() & IS_WARM || IsBurning(); }
truth bodypart::IsWarmBlooded() const { return MainMaterial->GetBodyFlags() & IS_WARM_BLOODED; }
truth bodypart::UseMaterialAttributes() const
{ return MainMaterial->GetBodyFlags() & USE_MATERIAL_ATTRIBUTES || !Master || Master->AlwaysUseMaterialAttributes(); }
truth bodypart::CanRegenerate() const { return MainMaterial->GetBodyFlags() & CAN_REGENERATE; }
square* bodypart::GetSquareUnder(int I) const
{ return Master ? Slot[0]->GetSquareUnder(I) : Slot[I]->GetSquareUnder(); }
lsquare* bodypart::GetLSquareUnder(int I) const
{ return static_cast<lsquare*>(Master ? Slot[0]->GetSquareUnder(I) : Slot[I]->GetSquareUnder()); }
item* bodypart::GetExternalBodyArmor() const { return GetHumanoidMaster()->GetBodyArmor(); }
item* bodypart::GetExternalCloak() const { return GetHumanoidMaster()->GetCloak(); }
item* bodypart::GetExternalHelmet() const { return GetHumanoidMaster()->GetHelmet(); }
item* bodypart::GetExternalBelt() const { return GetHumanoidMaster()->GetBelt(); }
truth bodypart::AllowFluidBe() const { return !Master || !Master->IsPolymorphed(); }

int head::GetBodyPartIndex() const { return HEAD_INDEX; }
int head::GetBiteMinDamage() const { return int(BiteDamage * 0.75); }
int head::GetBiteMaxDamage() const { return int(BiteDamage * 1.25 + 1); }

int torso::GetBodyPartIndex() const { return TORSO_INDEX; }

int normaltorso::GetGraphicsContainerIndex() const { return GR_CHARACTER; }

int arm::GetMinDamage() const { return int(Damage * 0.75); }
int arm::GetMaxDamage() const { return int(Damage * 1.25 + 1); }
double arm::GetBlockValue() const { return GetToHitValue() * GetWielded()->GetBlockModifier() / 10000; }

int rightarm::GetBodyPartIndex() const { return RIGHT_ARM_INDEX; }
int rightarm::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_ARM; }

int leftarm::GetBodyPartIndex() const { return LEFT_ARM_INDEX; }
int leftarm::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_ARM; }

int groin::GetBodyPartIndex() const { return GROIN_INDEX; }
int groin::GetSpecialFlags() const { return SpecialFlags|ST_GROIN; }

int leg::GetKickMinDamage() const { return int(KickDamage * 0.75); }
int leg::GetKickMaxDamage() const { return int(KickDamage * 1.25 + 1); }

int rightleg::GetBodyPartIndex() const { return RIGHT_LEG_INDEX; }
int rightleg::GetSpecialFlags() const { return SpecialFlags|ST_RIGHT_LEG; }

int leftleg::GetBodyPartIndex() const { return LEFT_LEG_INDEX; }
int leftleg::GetSpecialFlags() const { return SpecialFlags|ST_LEFT_LEG; }

v2 eddytorso::GetBitmapPos(int Frame) const { return torso::GetBitmapPos(Frame) + v2((Frame&0x6) << 3, 0); }

head* corpse::Behead() { return Deceased->Behead(); }
truth corpse::CanBeCloned() const { return GetDeceased()->CanBeCloned(); }
int corpse::GetAttachedGod() const { return GetDeceased()->GetTorso()->GetAttachedGod(); }

v2 ennerhead::GetBitmapPos(int Frame) const
{ return Frame & 16 ? head::GetBitmapPos(Frame) : head::GetBitmapPos(Frame) + v2(16, 0); }

alpha blinkdogtorso::GetAlphaA(int Frame) const { return (Frame & 31) != 31 ? 255 : 0; }

void bodypart::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << BitmapPos << ColorB << ColorC << ColorD << SpecialFlags << WobbleData << HP;
  SaveFile << OwnerDescription << BloodMaterial << NormalMaterial << Scar << DamageID;
}

void bodypart::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> BitmapPos >> ColorB >> ColorC >> ColorD >> SpecialFlags >> WobbleData >> HP;
  SaveFile >> OwnerDescription >> BloodMaterial >> NormalMaterial >> Scar >> DamageID;
}

int bodypart::GetStrengthValue() const
{
  if(!UseMaterialAttributes())
    return long(GetStrengthModifier()) * Master->GetAttribute(ENDURANCE) / 2000;
  else
    return long(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000;
}

int head::GetTotalResistance(int Type) const
{
  if(Master)
  {
    int Resistance = GetResistance(Type) + Master->GetGlobalResistance(Type);

    if(GetHelmet())
      Resistance += GetHelmet()->GetResistance(Type);

    if(GetExternalBodyArmor())
      Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 2;

    return Resistance;
  }
  else
    return GetResistance(Type);
}

int normaltorso::GetTotalResistance(int Type) const
{
  if(Master)
    return GetResistance(Type) + Master->GetGlobalResistance(Type);
  else
    return GetResistance(Type);
}

int humanoidtorso::GetTotalResistance(int Type) const
{
  if(Master)
  {
    int Resistance = GetResistance(Type) + Master->GetGlobalResistance(Type);

    if(GetBodyArmor())
      Resistance += GetBodyArmor()->GetResistance(Type);

    if(GetBelt())
      Resistance += GetBelt()->GetResistance(Type);

    return Resistance;
  }
  else
    return GetResistance(Type);
}

int arm::GetTotalResistance(int Type) const
{
  if(Master)
  {
    int Resistance = GetResistance(Type) + Master->GetGlobalResistance(Type);

    if(GetExternalBodyArmor())
      Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 1;

    if(GetGauntlet())
      Resistance += GetGauntlet()->GetResistance(Type);

    return Resistance;
  }
  else
    return GetResistance(Type);
}

int groin::GetTotalResistance(int Type) const
{
  if(Master)
  {
    int Resistance = GetResistance(Type) + Master->GetGlobalResistance(Type);

    if(GetExternalBodyArmor())
      Resistance += GetExternalBodyArmor()->GetResistance(Type);

    if(GetHumanoidMaster()->GetBelt())
      Resistance += GetHumanoidMaster()->GetBelt()->GetResistance(Type);

    return Resistance;
  }
  else
    return GetResistance(Type);
}

int leg::GetTotalResistance(int Type) const
{
  if(Master)
  {
    int Resistance = GetResistance(Type) + Master->GetGlobalResistance(Type);

    if(GetExternalBodyArmor())
      Resistance += GetExternalBodyArmor()->GetResistance(Type) >> 1;

    if(GetBoot())
      Resistance += GetBoot()->GetResistance(Type);

    return Resistance;
  }
  else
    return GetResistance(Type);
}

void head::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BaseBiteStrength << BonusBiteStrength;
  SaveFile << HelmetSlot << AmuletSlot;
}

void head::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseBiteStrength >> BonusBiteStrength;
  SaveFile >> HelmetSlot >> AmuletSlot;
}

void humanoidtorso::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BodyArmorSlot << CloakSlot << BeltSlot;
}

void humanoidtorso::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BodyArmorSlot >> CloakSlot >> BeltSlot;
}

void arm::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BaseUnarmedStrength;
  SaveFile << StrengthExperience << DexterityExperience;
  SaveFile << WieldedSlot << GauntletSlot << RingSlot;
  SaveFile << WieldedGraphicData;
}

void arm::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseUnarmedStrength;
  SaveFile >> StrengthExperience >> DexterityExperience;
  SaveFile >> WieldedSlot >> GauntletSlot >> RingSlot;
  SaveFile >> WieldedGraphicData;
}

void leg::Save(outputfile& SaveFile) const
{
  bodypart::Save(SaveFile);
  SaveFile << BaseKickStrength << StrengthExperience << AgilityExperience;
  SaveFile << BootSlot;
}

void leg::Load(inputfile& SaveFile)
{
  bodypart::Load(SaveFile);
  SaveFile >> BaseKickStrength >> StrengthExperience >> AgilityExperience;
  SaveFile >> BootSlot;
}

truth bodypart::ReceiveDamage(character* Damager, int Damage, int Type, int Direction)
{
  if(Master)
  {
    if(Type & POISON && !IsAlive())
      return false;

    int BHP = HP;

    if(HP <= Damage && !CanBeSevered(Type))
      Damage = GetHP() - 1;

    if(!Damage)
      return false;

    EditHP(1, -Damage);

    if(Type & DRAIN && IsAlive())
      for(int c = 0; c < Damage; ++c)
        Damager->HealHitPoint();

    truth WasBadlyHurt = IsBadlyHurt();

    if(HP <= 0)
      return true;

    if(DamageTypeCanScar(Type) && !(RAND_N(25 + 25 * HP / MaxHP)))
      GenerateScar(Damage, Type);

    if(Master->IsPlayer())
    {
      if(HP == 1 && BHP > 1)
      {
        if(IsAlive())
          ADD_MESSAGE("Your %s bleeds very badly.", GetBodyPartName().CStr());
        else
          ADD_MESSAGE("Your %s is in very bad condition.", GetBodyPartName().CStr());

        if(Master->BodyPartIsVital(GetBodyPartIndex()))
          game::AskForKeyPress(CONST_S("Vital bodypart in serious danger! [press any key to continue]"));
      }
      else if(IsBadlyHurt() && !WasBadlyHurt)
      {
        if(IsAlive())
          ADD_MESSAGE("Your %s bleeds.", GetBodyPartName().CStr());
        else
          ADD_MESSAGE("Your %s is in bad condition.", GetBodyPartName().CStr());

        if(Master->BodyPartIsVital(GetBodyPartIndex()))
          game::AskForKeyPress(CONST_S("Vital bodypart in danger! [press any key to continue]"));
      }
    }

    SignalPossibleUsabilityChange();
  }
  else
    return item::ReceiveDamage(Damager, Damage, Type, Direction);

  return false;
}

truth bodypart::CanBeSevered(int Type) const
{
  if((HP == MaxHP && HP != 1 && !Master->IsExtraFragile())
     || (Type & (POISON|SOUND) && GetBodyPartIndex() != TORSO_INDEX))
    return false;

  if(!Master->BodyPartIsVital(GetBodyPartIndex()) || Master->IsExtraFragile())
    return true;

  bodypart* Torso = Master->GetTorso();
  return Torso->HP != Torso->MaxHP || Torso->HP == 1;
}

double arm::GetUnarmedDamage() const
{
  double WeaponStrength = GetBaseUnarmedStrength() * GetBaseUnarmedStrength();
  item* Gauntlet = GetGauntlet();

  if(Gauntlet)
    WeaponStrength += Gauntlet->GetWeaponStrength();

  double Base = sqrt(5e-5 * WeaponStrength);

  if(Gauntlet)
    Base += Gauntlet->GetDamageBonus();

  double Damage = Base * sqrt(1e-7 * GetAttribute(ARM_STRENGTH))
                  * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus();

  return Damage;
}

double arm::GetUnarmedToHitValue() const
{
  double BonusMultiplier = 10.;
  item* Gauntlet = GetGauntlet();

  if(Gauntlet)
    BonusMultiplier += Gauntlet->GetTHVBonus();

  return GetAttribute(DEXTERITY)
    * sqrt(2.5 * Master->GetAttribute(PERCEPTION))
    * GetHumanoidMaster()->GetCWeaponSkill(UNARMED)->GetBonus()
    * Master->GetMoveEase()
    * BonusMultiplier / 5000000;
}

long arm::GetUnarmedAPCost() const
{
  return long(10000000000. / (APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase()
                              * Master->GetCWeaponSkill(UNARMED)->GetBonus()));
}

void arm::CalculateDamage()
{
  if(!Master)
    return;

  if(!IsUsable())
    Damage = 0;
  else if(GetWielded())
    Damage = GetWieldedDamage();
  else if(PairArmAllowsMelee())
    Damage = GetUnarmedDamage();
  else
    Damage = 0;
}

void arm::CalculateToHitValue()
{
  if(!Master)
    return;

  if(GetWielded())
    ToHitValue = GetWieldedToHitValue();
  else if(PairArmAllowsMelee())
    ToHitValue = GetUnarmedToHitValue();
  else
    ToHitValue = 0;
}

void arm::CalculateAPCost()
{
  if(!Master)
    return;

  if(GetWielded())
    APCost = GetWieldedAPCost();
  else if(PairArmAllowsMelee())
    APCost = GetUnarmedAPCost();
  else return;

  if(APCost < 100)
    APCost = 100;
}

truth arm::PairArmAllowsMelee() const
{
  const arm* PairArm = GetPairArm();
  return !PairArm || !PairArm->IsUsable() || !PairArm->GetWielded()
    || PairArm->GetWielded()->IsShield(Master);
}

double arm::GetWieldedDamage() const
{
  citem* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = Wielded->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
  {
    HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
    Requirement >>= 2;
  }

  if(HitStrength > Requirement)
  {
    /* I have no idea whether this works. It needs to be checked */

    return Wielded->GetBaseDamage() * sqrt(1e-13 * HitStrength)
        * GetCurrentSWeaponSkillBonus()
        * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus();
  }
  else
    return 0;
}

double arm::GetWieldedToHitValue() const
{
  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  citem* Wielded = GetWielded();

  double Base = 2e-11
                * Min(HitStrength, 10)
                * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus()
                * GetCurrentSWeaponSkillBonus()
                * Master->GetMoveEase()
                * (10000. / (1000 + Wielded->GetWeight()) + Wielded->GetTHVBonus());
  double ThisToHit = GetAttribute(DEXTERITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION));
  const arm* PairArm = GetPairArm();

  if(PairArm && PairArm->IsUsable())
  {
    citem* PairWielded = PairArm->GetWielded();

    if(!PairWielded)
    {
      if(Wielded->IsTwoHanded() && !Wielded->IsShield(Master))
        return Base * (ThisToHit + PairArm->GetAttribute(DEXTERITY)
                       * sqrt(2.5 * Master->GetAttribute(PERCEPTION))) / 2;
    }
    else if(!Wielded->IsShield(Master) && !PairWielded->IsShield(Master))
      return Base * ThisToHit / (1.0 + (500.0 + PairWielded->GetWeight())
                                 / (1000.0 + (Wielded->GetWeight() << 1)));
  }

  return Base * ThisToHit;
}

long arm::GetWieldedAPCost() const
{
  citem* Wielded = GetWielded();

  if(Wielded->IsShield(Master))
    return 0;

  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return long(1 / (1e-14 * APBonus(GetAttribute(DEXTERITY)) * Master->GetMoveEase()
                   * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus()
                   * (GetCurrentSWeaponSkillBonus() * Min(HitStrength, 10))));
}

void head::CalculateDamage()
{
  if(!Master)
    return;

  if(Master->StateIsActivated(VAMPIRISM))
    BiteDamage = 7.07e-6 * (GetBaseBiteStrength() + GetBonusBiteStrength()) * GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetBonus();
  else
    BiteDamage = 7.07e-6 * GetBaseBiteStrength() * GetHumanoidMaster()->GetCWeaponSkill(BITE)->GetBonus();
}

void head::CalculateToHitValue()
{
  if(!Master)
    return;

  BiteToHitValue = Master->GetAttribute(AGILITY) * sqrt(2.5 * Master->GetAttribute(PERCEPTION))
                   * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus() * Master->GetMoveEase() / 1000000;
}

void head::CalculateAPCost()
{
  if(!Master)
    return;

  BiteAPCost = Max(long(10000000000. / (APBonus(Master->GetAttribute(AGILITY)) * Master->GetMoveEase()
                                        * Master->GetCWeaponSkill(BITE)->GetBonus())), 100L);
}

void leg::CalculateDamage()
{
  if(!Master)
    return;

  double WeaponStrength = GetBaseKickStrength() * GetBaseKickStrength();
  item* Boot = GetBoot();

  if(Boot)
    WeaponStrength += Boot->GetWeaponStrength();

  double Base = sqrt(5e-5 * WeaponStrength);

  if(Boot)
    Base += Boot->GetDamageBonus();

  KickDamage = Base * sqrt(1e-7 * GetAttribute(LEG_STRENGTH))
               * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus();
}

void leg::CalculateToHitValue()
{
  if(!Master)
    return;

  double BonusMultiplier = 10.;
  item* Boot = GetBoot();

  if(Boot)
    BonusMultiplier += Boot->GetTHVBonus();

  KickToHitValue = GetAttribute(AGILITY)
                   * sqrt(2.5 * Master->GetAttribute(PERCEPTION))
                   * GetHumanoidMaster()->GetCWeaponSkill(KICK)->GetBonus()
                   * Master->GetMoveEase()
                   * BonusMultiplier / 10000000;
}

void leg::CalculateAPCost()
{
  if(!Master)
    return;

  KickAPCost = Max(long(20000000000. / (APBonus(GetAttribute(AGILITY)) * Master->GetMoveEase()
                                        * Master->GetCWeaponSkill(KICK)->GetBonus())), 100L);
}

humanoid* bodypart::GetHumanoidMaster() const
{
  return static_cast<humanoid*>(Master);
}

void corpse::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Deceased;
}

void corpse::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Deceased;
  Deceased->SetMotherEntity(this);
  Enable();
}

void corpse::AddPostFix(festring& String, int) const
{
  String << " of ";
  GetDeceased()->AddName(String, INDEFINITE);
}

int corpse::GetOfferValue(int Receiver) const
{
  int OfferValue = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart)
      OfferValue += BodyPart->GetOfferValue(Receiver);
  }

  return OfferValue;
}

double corpse::GetWeaponStrength() const
{
  return GetFormModifier() * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue()
         * sqrt(GetDeceased()->GetTorso()->GetMainMaterial()->GetWeight());
}

truth corpse::CanBeEatenByAI(ccharacter* Eater) const
{
  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart && !BodyPart->CanBeEatenByAI(Eater))
      return false;
  }

  return true;
}

int corpse::GetStrengthValue() const
{
  return long(GetStrengthModifier()) * GetDeceased()->GetTorso()->GetMainMaterial()->GetStrengthValue() / 2000;
}

corpse::~corpse()
{
  delete Deceased;
}

col16 corpse::GetMaterialColorA(int) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetColor();
}

alpha corpse::GetAlphaA(int) const
{
  return GetDeceased()->GetTorso()->GetMainMaterial()->GetAlpha();
}

col16 corpse::GetMaterialColorB(int) const
{
  torso* Torso = GetDeceased()->GetTorso();

  if(Torso->IsAlive())
    return material::GetDataBase(GetDeceased()->GetBloodMaterial())->Color;
  else
    return Torso->GetMainMaterial()->GetColor();
}

alpha corpse::GetAlphaB(int) const
{
  torso* Torso = GetDeceased()->GetTorso();
  return Torso->IsAlive() ? 175 : Torso->GetMainMaterial()->GetAlpha();
}

int corpse::GetSparkleFlags() const
{
  torso* Torso = GetDeceased()->GetTorso();
  material* Material = Torso->GetMainMaterial();
  return Material->IsSparkling() ? SPARKLING_A|(!Torso->IsAlive() ? SPARKLING_B : 0) : 0;
}

v2 corpse::GetBitmapPos(int) const
{
  if(GetDeceased()->GetSize() < 50)
    return v2(32, 64);
  else if(GetDeceased()->GetSize() < 150)
    return v2(16, 192);
  else
    return v2(16, 272);
}

int corpse::GetSize() const
{
  return GetDeceased()->GetSize();
}

void corpse::SetDeceased(character* What)
{
  Deceased = What;
  Deceased->SetMotherEntity(this);
  SignalVolumeAndWeightChange();
  SignalEmitationIncrease(Deceased->GetEmitation());
  UpdatePictures();
  Enable();
}

void head::DropEquipment(stack* Stack)
{
  if(Stack)
  {
    if(GetHelmet())
      GetHelmet()->MoveTo(Stack);

    if(GetAmulet())
      GetAmulet()->MoveTo(Stack);
  }
  else
  {
    if(GetHelmet())
      GetSlot()->AddFriendItem(GetHelmet());

    if(GetAmulet())
      GetSlot()->AddFriendItem(GetAmulet());
  }
}

void humanoidtorso::DropEquipment(stack* Stack)
{
  if(Stack)
  {
    if(GetBodyArmor())
      GetBodyArmor()->MoveTo(Stack);

    if(GetCloak())
      GetCloak()->MoveTo(Stack);

    if(GetBelt())
      GetBelt()->MoveTo(Stack);
  }
  else
  {
    if(GetBodyArmor())
      GetSlot()->AddFriendItem(GetBodyArmor());

    if(GetCloak())
      GetSlot()->AddFriendItem(GetCloak());

    if(GetBelt())
      GetSlot()->AddFriendItem(GetBelt());
  }
}

void arm::DropEquipment(stack* Stack)
{
  if(Stack)
  {
    if(GetWielded())
      GetWielded()->MoveTo(Stack);

    if(GetGauntlet())
      GetGauntlet()->MoveTo(Stack);

    if(GetRing())
      GetRing()->MoveTo(Stack);
  }
  else
  {
    if(GetWielded())
      GetSlot()->AddFriendItem(GetWielded());

    if(GetGauntlet())
      GetSlot()->AddFriendItem(GetGauntlet());

    if(GetRing())
      GetSlot()->AddFriendItem(GetRing());
  }
}

void leg::DropEquipment(stack* Stack)
{
  if(Stack)
  {
    if(GetBoot())
      GetBoot()->MoveTo(Stack);
  }
  else
  {
    if(GetBoot())
      GetSlot()->AddFriendItem(GetBoot());
  }
}

head::~head()
{
  delete GetHelmet();
  delete GetAmulet();
}

humanoidtorso::~humanoidtorso()
{
  delete GetBodyArmor();
  delete GetCloak();
  delete GetBelt();
}

arm::~arm()
{
  delete GetWielded();
  delete GetGauntlet();
  delete GetRing();
}

leg::~leg()
{
  delete GetBoot();
}

truth corpse::IsDestroyable(ccharacter* Char) const
{
  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart && !BodyPart->IsDestroyable(Char))
      return false;
  }

  return true;
}

long corpse::GetTruePrice() const
{
  long Price = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart)
      Price += BodyPart->GetTruePrice();
  }

  return Price;
}

material* corpse::GetMaterial(int I) const
{
  return GetDeceased()->GetTorso()->GetMaterial(I);
}

int bodypart::GetSparkleFlags() const
{
  return (GetMainMaterial()->SkinColorIsSparkling() ? SPARKLING_A : 0)
    | (Flags >> BODYPART_SPARKLE_SHIFT & (SPARKLING_B|SPARKLING_C|SPARKLING_D));
}

truth corpse::RaiseTheDead(character* Summoner)
{
  if(Summoner && Summoner->IsPlayer())
    game::DoEvilDeed(50);

  GetDeceased()->Enable();

  if(GetDeceased()->TryToRiseFromTheDead())
  {
    v2 Pos = GetPos();
    RemoveFromSlot();
    GetDeceased()->SetMotherEntity(0);

    if(Summoner && GetDeceased()->CanTameWithResurrection(Summoner)
                && !GetDeceased()->IsPlayer())
      GetDeceased()->ChangeTeam(Summoner->GetTeam());

    GetDeceased()->PutToOrNear(Pos);
    GetDeceased()->SignalStepFrom(0);
    Deceased = 0;
    SendToHell();
    return true;
  }
  else
  {
    GetDeceased()->Disable();
    return false;
  }
}

head::head()
{
  HelmetSlot.Init(this, HELMET_INDEX);
  AmuletSlot.Init(this, AMULET_INDEX);
}

humanoidtorso::humanoidtorso()
{
  BodyArmorSlot.Init(this, BODY_ARMOR_INDEX);
  CloakSlot.Init(this, CLOAK_INDEX);
  BeltSlot.Init(this, BELT_INDEX);
}

rightarm::rightarm()
{
  WieldedSlot.Init(this, RIGHT_WIELDED_INDEX);
  GauntletSlot.Init(this, RIGHT_GAUNTLET_INDEX);
  RingSlot.Init(this, RIGHT_RING_INDEX);
}

leftarm::leftarm()
{
  WieldedSlot.Init(this, LEFT_WIELDED_INDEX);
  GauntletSlot.Init(this, LEFT_GAUNTLET_INDEX);
  RingSlot.Init(this, LEFT_RING_INDEX);
}

rightleg::rightleg()
{
  BootSlot.Init(this, RIGHT_BOOT_INDEX);
}

leftleg::leftleg()
{
  BootSlot.Init(this, LEFT_BOOT_INDEX);
}

void arm::Hit(character* Enemy, v2 HitPos, int Direction, int Flags)
{
  long StrExp = 50, DexExp = 50;
  truth THW = false;
  item* Wielded = GetWielded();

  if(Wielded)
  {
    long Weight = Wielded->GetWeight();
    StrExp = Limit(15 * Weight / 200L, 75L, 300L);
    DexExp = Weight ? Limit(75000L / Weight, 75L, 300L) : 300;
    THW = TwoHandWieldIsActive();
  }

  switch(Enemy->TakeHit(Master, Wielded ? Wielded : GetGauntlet(), this, HitPos, GetTypeDamage(Enemy),
                        GetToHitValue(), RAND() % 26 - RAND() % 26, Wielded ? WEAPON_ATTACK : UNARMED_ATTACK,
                        Direction, !(RAND() % Master->GetCriticalModifier()), Flags & SADIST_HIT))
  {
   case HAS_HIT:
   case HAS_BLOCKED:
   case HAS_DIED:
   case DID_NO_DAMAGE:
    EditExperience(ARM_STRENGTH, StrExp, 1 << 9);

    if(THW && GetPairArm())
      GetPairArm()->EditExperience(ARM_STRENGTH, StrExp, 1 << 9);

   case HAS_DODGED:
    EditExperience(DEXTERITY, DexExp, 1 << 9);

    if(THW && GetPairArm())
      GetPairArm()->EditExperience(DEXTERITY, DexExp, 1 << 9);
  }
}

int arm::GetAttribute(int Identifier, truth AllowBonus) const
{
  if(Identifier == ARM_STRENGTH)
  {
    int Base = !UseMaterialAttributes()
               ? int(StrengthExperience * EXP_DIVISOR)
               : GetMainMaterial()->GetStrengthValue();

    if(AllowBonus)
      Base += StrengthBonus;

    return Max(!IsBadlyHurt() || !AllowBonus ? Base : Base / 3, 1);
  }
  else if(Identifier == DEXTERITY)
  {
    int Base = !UseMaterialAttributes()
               ? int(DexterityExperience * EXP_DIVISOR)
               : GetMainMaterial()->GetFlexibility() << 2;

    if(AllowBonus)
      Base += DexterityBonus;

    return Max(IsUsable() || !AllowBonus ? Base : Base / 3, 1);
  }
  else
  {
    ABORT("Illegal arm attribute %d request!", Identifier);
    return 0xACCA;
  }
}

truth arm::EditAttribute(int Identifier, int Value)
{
  if(!Master)
    return false;

  if(Identifier == ARM_STRENGTH)
  {
    if(!UseMaterialAttributes()
       && Master->RawEditAttribute(StrengthExperience, Value))
    {
      Master->CalculateBattleInfo();

      if(Master->IsPlayerKind())
        UpdatePictures();

      return true;
    }
  }
  else if(Identifier == DEXTERITY)
    if(!UseMaterialAttributes()
       && Master->RawEditAttribute(DexterityExperience, Value))
    {
      Master->CalculateBattleInfo();
      return true;
    }

  return false;
}

void arm::EditExperience(int Identifier, double Value, double Speed)
{
  if(!Master)
    return;

  if(Identifier == ARM_STRENGTH)
  {
    if(!UseMaterialAttributes())
    {
      int Change = Master->RawEditExperience(StrengthExperience,
                                             Master->GetNaturalExperience(ARM_STRENGTH),
                                             Value, Speed);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "stronger" : "weaker";

        if(Master->IsPlayer())
          ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
        else if(Master->IsPet() && Master->CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

        Master->CalculateBattleInfo();

        if(Master->IsPlayerKind())
          UpdatePictures();
      }
    }
  }
  else if(Identifier == DEXTERITY)
  {
    if(!UseMaterialAttributes())
    {
      int Change = Master->RawEditExperience(DexterityExperience,
                                             Master->GetNaturalExperience(DEXTERITY),
                                             Value, Speed);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "quite dextrous" : "clumsy";

        if(Master->IsPlayer())
          ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
        else if(Master->IsPet() && Master->CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

        Master->CalculateBattleInfo();
      }
    }
  }
  else
    ABORT("Illegal arm attribute %d experience edit request!", Identifier);
}

int leg::GetAttribute(int Identifier, truth AllowBonus) const
{
  if(Identifier == LEG_STRENGTH)
  {
    int Base = !UseMaterialAttributes()
               ? int(StrengthExperience * EXP_DIVISOR)
               : GetMainMaterial()->GetStrengthValue();

    if(AllowBonus)
      Base += StrengthBonus;

    return Max(!IsBadlyHurt() || !AllowBonus ? Base : Base / 3, 1);
  }
  else if(Identifier == AGILITY)
  {
    int Base = !UseMaterialAttributes()
               ? int(AgilityExperience * EXP_DIVISOR)
               : GetMainMaterial()->GetFlexibility() << 2;

    if(AllowBonus)
      Base += AgilityBonus;

    return Max(IsUsable() || !AllowBonus ? Base : Base / 3, 1);
  }
  else
  {
    ABORT("Illegal leg attribute %d request!", Identifier);
    return 0xECCE;
  }
}

truth leg::EditAttribute(int Identifier, int Value)
{
  if(!Master)
    return false;

  if(Identifier == LEG_STRENGTH)
  {
    if(!UseMaterialAttributes()
       && Master->RawEditAttribute(StrengthExperience, Value))
    {
      Master->CalculateBurdenState();
      Master->CalculateBattleInfo();
      return true;
    }
  }
  else if(Identifier == AGILITY)
    if(!UseMaterialAttributes()
       && Master->RawEditAttribute(AgilityExperience, Value))
    {
      Master->CalculateBattleInfo();
      return true;
    }

  return false;
}

void leg::EditExperience(int Identifier, double Value, double Speed)
{
  if(!Master)
    return;

  if(Identifier == LEG_STRENGTH)
  {
    if(!UseMaterialAttributes())
    {
      int Change = Master->RawEditExperience(StrengthExperience,
                                             Master->GetNaturalExperience(LEG_STRENGTH),
                                             Value, Speed);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "stronger" : "weaker";

        if(Master->IsPlayer())
          ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
        else if(Master->IsPet() && Master->CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

        Master->CalculateBurdenState();
        Master->CalculateBattleInfo();
      }
    }
  }
  else if(Identifier == AGILITY)
  {
    if(!UseMaterialAttributes())
    {
      int Change = Master->RawEditExperience(AgilityExperience,
                                             Master->GetNaturalExperience(AGILITY),
                                             Value, Speed);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "very agile" : "slower";

        if(Master->IsPlayer())
          ADD_MESSAGE("Your %s feels %s!", GetBodyPartName().CStr(), Adj);
        else if(Master->IsPet() && Master->CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", Master->CHAR_NAME(DEFINITE), Adj);

        Master->CalculateBattleInfo();
      }
    }
  }
  else
    ABORT("Illegal leg attribute %d experience edit request!", Identifier);
}

void head::InitSpecialAttributes()
{
  BaseBiteStrength = Master->GetBaseBiteStrength();
  BonusBiteStrength = Master->GetBonusBiteStrength();
}

void arm::InitSpecialAttributes()
{
  if(!Master->IsHuman() || Master->IsInitializing())
  {
    StrengthExperience = Master->GetNaturalExperience(ARM_STRENGTH);
    DexterityExperience = Master->GetNaturalExperience(DEXTERITY);
  }
  else
  {
    StrengthExperience = game::GetAveragePlayerArmStrengthExperience();
    DexterityExperience = game::GetAveragePlayerDexterityExperience();
  }

  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience, MIN_EXP, MAX_EXP);
  BaseUnarmedStrength = Master->GetBaseUnarmedStrength();
}

void leg::InitSpecialAttributes()
{
  if(!Master->IsHuman() || Master->IsInitializing())
  {
    StrengthExperience = Master->GetNaturalExperience(LEG_STRENGTH);
    AgilityExperience = Master->GetNaturalExperience(AGILITY);
  }
  else
  {
    StrengthExperience = game::GetAveragePlayerLegStrengthExperience();
    AgilityExperience = game::GetAveragePlayerAgilityExperience();
  }

  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience, MIN_EXP, MAX_EXP);
  BaseKickStrength = Master->GetBaseKickStrength();
}

void bodypart::SignalEquipmentAdd(gearslot* Slot)
{
  if(Master)
    Master->SignalEquipmentAdd(Slot->GetEquipmentIndex());
}

void bodypart::SignalEquipmentRemoval(gearslot* Slot, citem* Item)
{
  if(Master)
    Master->SignalEquipmentRemoval(Slot->GetEquipmentIndex(), Item);
}

void bodypart::Mutate()
{
  GetMainMaterial()->SetVolume(long(GetVolume() * (1.5 - (RAND() & 1023) / 1023.)));
}

void arm::Mutate()
{
  bodypart::Mutate();
  StrengthExperience = StrengthExperience * (1.5 - (RAND() & 1023) / 1023.);
  DexterityExperience = DexterityExperience * (1.5 - (RAND() & 1023) / 1023.);
  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience, MIN_EXP, MAX_EXP);
}

void leg::Mutate()
{
  bodypart::Mutate();
  StrengthExperience = StrengthExperience * (1.5 - (RAND() & 1023) / 1023.);
  AgilityExperience = AgilityExperience * (1.5 - (RAND() & 1023) / 1023.);
  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience, MIN_EXP, MAX_EXP);
}

arm* rightarm::GetPairArm() const
{
  return GetHumanoidMaster() ? GetHumanoidMaster()->GetLeftArm() : 0;
}

arm* leftarm::GetPairArm() const
{
  return GetHumanoidMaster() ? GetHumanoidMaster()->GetRightArm() : 0;
}

sweaponskill** rightarm::GetCurrentSWeaponSkill() const
{
  return &GetHumanoidMaster()->CurrentRightSWeaponSkill;
}

sweaponskill** leftarm::GetCurrentSWeaponSkill() const
{
  return &GetHumanoidMaster()->CurrentLeftSWeaponSkill;
}

alpha bodypart::GetMaxAlpha() const
{
  if(Master && (Master->StateIsActivated(INVISIBLE) || !!Master->GhostCopyMaterials()))
    return 150;
  else
    return 255;
}

void bodypart::AddPostFix(festring& String, int) const
{
  if(!OwnerDescription.IsEmpty())
    String << ' ' << OwnerDescription;
}

void corpse::CalculateVolumeAndWeight()
{
  Volume = Deceased->GetVolume();
  Weight = Deceased->GetWeight();
}

item* head::GetEquipment(int I) const
{
  switch(I)
  {
   case 0: return GetHelmet();
   case 1: return GetAmulet();
  }

  return 0;
}

item* humanoidtorso::GetEquipment(int I) const
{
  switch(I)
  {
   case 0: return GetBodyArmor();
   case 1: return GetCloak();
   case 2: return GetBelt();
  }

  return 0;
}

item* arm::GetEquipment(int I) const
{
  switch(I)
  {
   case 0: return GetWielded();
   case 1: return GetGauntlet();
   case 2: return GetRing();
  }

  return 0;
}

item* leg::GetEquipment(int I) const
{
  return !I ? GetBoot() : 0;
}

void bodypart::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  CarriedWeight = 0;
  BodyPartVolume = Volume;

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
    {
      Volume += Equipment->GetVolume();
      CarriedWeight += Equipment->GetWeight();
    }
  }

  Weight += CarriedWeight;
}

void corpse::CalculateEmitation()
{
  Emitation = Deceased->GetEmitation();
}

void bodypart::CalculateEmitation()
{
  item::CalculateEmitation();

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
      game::CombineLights(Emitation, Equipment->GetEmitation());
  }
}

void bodypart::CalculateMaxHP(ulong Flags)
{
  int HPDelta = MaxHP - HP, OldMaxHP = MaxHP;
  MaxHP = 0;
  int BurnLevel = 0;

  if(Master)
  {
    if(!UseMaterialAttributes())
    {
      long Endurance = Master->GetAttribute(ENDURANCE);
      double DoubleHP = GetBodyPartVolume() * Endurance * Endurance / 200000;

      for(size_t c = 0; c < Scar.size(); ++c)
        DoubleHP *= (100. - Scar[c].Severity * 4) / 100;

      if(MainMaterial)
      {
        BurnLevel = MainMaterial->GetBurnLevel();
        DoubleHP *= 1. * (4 - BurnLevel) / 4;
      }

      MaxHP = int(DoubleHP);
    }
    else
    {
      long SV = GetMainMaterial()->GetStrengthValue();
      MaxHP = (GetBodyPartVolume() * SV >> 4) * SV / 250000;
    }

    if(MaxHP < 1)
      MaxHP = 1;

    if(Flags & MAY_CHANGE_HPS)
    {
      if(MaxHP - HPDelta > 1)
        HP = MaxHP - HPDelta;
      else
        HP = 1;
    }
    else
    {
      //OldMaxHP - MaxHP;
    }

    if(Flags & CHECK_USABILITY)
      SignalPossibleUsabilityChange();
  }
}

void bodypart::SignalVolumeAndWeightChange()
{
  item::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
  {
    CalculateMaxHP();
    Master->CalculateHP();
    Master->CalculateMaxHP();
    Master->SignalBodyPartVolumeAndWeightChange();
    square* SquareUnder = GetSquareUnder();

    if(UpdateArmorPictures() && SquareUnder)
      SquareUnder->SendNewDrawRequest();
  }
}

/*{
  for(;;)
  {
    damageid& D = DamageID.back();
    D.
  }
}*/

void bodypart::SetHP(int What)
{
  HP = What;

  if(Master)
  {
    Master->CalculateHP();
    SignalPossibleUsabilityChange();
  }
}

void bodypart::EditHP(int SrcID, int What)
{
  HP += What;

  if(What < 0)
    RemoveDamageIDs(-What);
  else
    AddDamageID(SrcID, What);

  if(Master)
  {
    Master->CalculateHP();
    SignalPossibleUsabilityChange();
  }
}

void arm::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
  {
    GetHumanoidMaster()->EnsureCurrentSWeaponSkillIsCorrect(*GetCurrentSWeaponSkill(), GetWielded());
    CalculateAttributeBonuses();
    CalculateAttackInfo();
    UpdateWieldedPicture();

    if(GetSquareUnder())
      GetSquareUnder()->SendNewDrawRequest();
  }
}

void leg::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
  {
    CalculateAttributeBonuses();
    CalculateAttackInfo();
  }
}

void humanoidtorso::SignalVolumeAndWeightChange()
{
  bodypart::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
  {
    humanoid* HumanoidMaster = GetHumanoidMaster();

    if(HumanoidMaster->GetRightArm())
      HumanoidMaster->GetRightArm()->CalculateAttributeBonuses();

    if(HumanoidMaster->GetLeftArm())
      HumanoidMaster->GetLeftArm()->CalculateAttributeBonuses();

    if(HumanoidMaster->GetRightLeg())
      HumanoidMaster->GetRightLeg()->CalculateAttributeBonuses();

    if(HumanoidMaster->GetLeftLeg())
      HumanoidMaster->GetLeftLeg()->CalculateAttributeBonuses();
  }
}

void bodypart::CalculateAttackInfo()
{
  CalculateDamage();
  CalculateToHitValue();
  CalculateAPCost();
}

truth arm::TwoHandWieldIsActive() const
{
  citem* Wielded = GetWielded();

  if(Wielded->IsTwoHanded() && !Wielded->IsShield(Master))
  {
    arm* PairArm = GetPairArm();
    return PairArm && PairArm->IsUsable() && !PairArm->GetWielded();
  }
  else
    return false;
}

double bodypart::GetTimeToDie(int Damage, double ToHitValue, double DodgeValue,
                              truth AttackIsBlockable, truth UseMaxHP) const
{
  double Durability;
  int TotalResistance = GetTotalResistance(PHYSICAL_DAMAGE);
  int Damage3 = (Damage << 1) + Damage;
  int Damage5 = (Damage << 2) + Damage;
  int TrueDamage = (19 * (Max((Damage3 >> 2) - TotalResistance, 0)
                          + Max((Damage5 >> 2) + 1 - (TotalResistance >> 1), 0))
                    + (Max(((Damage3 + (Damage3 >> 1)) >> 2) - TotalResistance, 0)
                       + Max(((Damage5 + (Damage5 >> 1)) >> 2) + 3 - (TotalResistance >> 1), 0))) / 40;

  int HP = UseMaxHP ? GetMaxHP() : GetHP();

  if(TrueDamage > 0)
  {
    double AverageDamage;

    if(AttackIsBlockable)
    {
      blockvector Block;
      Master->CreateBlockPossibilityVector(Block, ToHitValue);

      if(Block.size())
      {
        double ChanceForNoBlock = 1.0;
        AverageDamage = 0;

        for(uint c = 0; c < Block.size(); ++c)
        {
          ChanceForNoBlock -= Block[c].first;

          if(TrueDamage - Block[c].second > 0)
            AverageDamage += Block[c].first * (TrueDamage - Block[c].second);
        }

        AverageDamage += ChanceForNoBlock * TrueDamage;
      }
      else
        AverageDamage = TrueDamage;
    }
    else
      AverageDamage = TrueDamage;

    Durability = HP / (AverageDamage * GetRoughChanceToHit(ToHitValue, DodgeValue));

    if(Durability < 1)
      Durability = 1;

    if(Durability > 1000)
      Durability = 1000;
  }
  else
    Durability = 1000;

  return Durability;
}

double bodypart::GetRoughChanceToHit(double ToHitValue, double DodgeValue) const
{
  return GLOBAL_WEAK_BODYPART_HIT_MODIFIER * ToHitValue * GetBodyPartVolume()
         / ((DodgeValue / ToHitValue + 1) * DodgeValue * Master->GetBodyVolume() * 100);
}

double torso::GetRoughChanceToHit(double ToHitValue, double DodgeValue) const
{
  return 1 / (DodgeValue / ToHitValue + 1);
}

void bodypart::RandomizePosition()
{
  SpecialFlags |= 1 + RAND() % 7;
  UpdatePictures();
}

double arm::GetBlockChance(double EnemyToHitValue) const
{
  citem* Wielded = GetWielded();
  return Wielded ? Min(1.0 / (1 + EnemyToHitValue / (GetToHitValue() * Wielded->GetBlockModifier()) * 10000), 1.0) : 0;
}

int arm::GetBlockCapability() const
{
  citem* Wielded = GetWielded();

  if(!Wielded)
    return 0;

  int HitStrength = GetWieldedHitStrength();

  if(HitStrength <= 0)
    return 0;

  return Min(HitStrength, 10) * Wielded->GetStrengthValue()
         * GetHumanoidMaster()->GetCWeaponSkill(Wielded->GetWeaponCategory())->GetBonus()
         * (*GetCurrentSWeaponSkill())->GetBonus() / 10000000;
}

void arm::WieldedSkillHit(int Hits)
{
  item* Wielded = GetWielded();

  if(Master->GetCWeaponSkill(Wielded->GetWeaponCategory())->AddHit(Hits))
  {
    CalculateAttackInfo();

    if(Master->IsPlayer())
    {
      int Category = Wielded->GetWeaponCategory();
      GetHumanoidMaster()->GetCWeaponSkill(Category)->AddLevelUpMessage(Category);
    }
  }

  if((*GetCurrentSWeaponSkill())->AddHit(Hits))
  {
    CalculateAttackInfo();

    if(Master->IsPlayer())
      (*GetCurrentSWeaponSkill())->AddLevelUpMessage(Wielded->CHAR_NAME(UNARTICLED));
  }
}

head::head(const head& Head) : mybase(Head), BaseBiteStrength(Head.BaseBiteStrength), BonusBiteStrength(Head.BonusBiteStrength)
{
  HelmetSlot.Init(this, HELMET_INDEX);
  AmuletSlot.Init(this, AMULET_INDEX);
}

humanoidtorso::humanoidtorso(const humanoidtorso& Torso) : mybase(Torso)
{
  BodyArmorSlot.Init(this, BODY_ARMOR_INDEX);
  CloakSlot.Init(this, CLOAK_INDEX);
  BeltSlot.Init(this, BELT_INDEX);
}

arm::arm(const arm& Arm)
: mybase(Arm),
  StrengthExperience(Arm.StrengthExperience),
  DexterityExperience(Arm.DexterityExperience),
  BaseUnarmedStrength(Arm.BaseUnarmedStrength)
{
}

rightarm::rightarm(const rightarm& Arm) : mybase(Arm)
{
  WieldedSlot.Init(this, RIGHT_WIELDED_INDEX);
  GauntletSlot.Init(this, RIGHT_GAUNTLET_INDEX);
  RingSlot.Init(this, RIGHT_RING_INDEX);
}

leftarm::leftarm(const leftarm& Arm) : mybase(Arm)
{
  WieldedSlot.Init(this, LEFT_WIELDED_INDEX);
  GauntletSlot.Init(this, LEFT_GAUNTLET_INDEX);
  RingSlot.Init(this, LEFT_RING_INDEX);
}

leg::leg(const leg& Leg)
: mybase(Leg),
  StrengthExperience(Leg.StrengthExperience),
  AgilityExperience(Leg.AgilityExperience),
  BaseKickStrength(Leg.BaseKickStrength)
{
}

rightleg::rightleg(const rightleg& Leg) : mybase(Leg)
{
  BootSlot.Init(this, RIGHT_BOOT_INDEX);
}

leftleg::leftleg(const leftleg& Leg) : mybase(Leg)
{
  BootSlot.Init(this, LEFT_BOOT_INDEX);
}

corpse::corpse(const corpse& Corpse) : mybase(Corpse)
{
  Deceased = Corpse.Deceased->Duplicate();
  Deceased->SetMotherEntity(this);
}

void bodypart::SignalSpoil(material* Material)
{
  if(Master)
    Master->SignalSpoil();
  else
    item::SignalSpoil(Material);
}

void corpse::SignalSpoil(material*)
{
  GetDeceased()->Disappear(this, "spoil", &item::IsVeryCloseToSpoiling);
}

truth bodypart::TestActivationEnergy(int Damage)
{
//  if(MainMaterial)
//  {
//    int molamola = ((GetMainMaterial()->GetStrengthValue() >> 1)
//                    + 5 * MainMaterial->GetFireResistance() + GetResistance(FIRE));
//    ADD_MESSAGE("%s is being tested (Damage is %d, AE is %d)", CHAR_NAME(DEFINITE), Damage, molamola);
//  }
  if(Damage <= 0)
    return false;

  character* Owner = GetMaster();
  truth Success = false;

  if(Owner)
    if(Owner->BodyPartIsVital(GetBodyPartIndex()) || !CanBeBurned())
      return Success;

  if(MainMaterial)
  {
    int TestDamage = Damage + MainMaterial->GetTransientThermalEnergy();
    GetMainMaterial()->AddToTransientThermalEnergy(Damage);
    if((GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
       && TestDamage >= ((GetMainMaterial()->GetStrengthValue() >> 1)
                         + 5 * MainMaterial->GetFireResistance() + GetResistance(FIRE)))
    {
      if(Owner)
      {
        if(Owner->IsPlayer())
          ADD_MESSAGE("Your %s catches fire!", GetBodyPartName().CStr());
        else if(Owner->CanBeSeenByPlayer())
          ADD_MESSAGE("The %s of %s catches fire!", GetBodyPartName().CStr(), Owner->CHAR_NAME(DEFINITE));
      }
      //ADD_MESSAGE("%s catches fire! (TestDamage was %d)", CHAR_NAME(DEFINITE), TestDamage); //CLEANUP
      Ignite();
      GetMainMaterial()->AddToSteadyStateThermalEnergy(Damage);
      Success = true;
    }
  }
  return Success;
}

void bodypart::SignalBurn(material* Material)
{
  int BodyPartIndex = GetBodyPartIndex();

  if(Master)
  {
    character* Owner = GetMaster();

    if(Owner->IsPlayer())
      ADD_MESSAGE("Your %s burns away completely!", GetBodyPartName().CStr());
    else if(Owner->CanBeSeenByPlayer())
      ADD_MESSAGE("The %s of %s burns away completely!", GetBodyPartName().CStr(), Owner->CHAR_NAME(DEFINITE));

    /*GetBodyPart(BodyPartIndex)->*/DropEquipment(!game::IsInWilderness() ? Owner->GetStackUnder() : Owner->GetStack());
    /*item* Burnt = */Owner->SevereBodyPart(BodyPartIndex, true);
/* // create lumps of (charred?) flesh... (note, remove 'true' from item* Burnt = SevereBodyPart(BodyPartIndex, true);
    if(Burnt)
      Burnt->DestroyBodyPart(!game::IsInWilderness() ? GetStackUnder() : GetStack());
*/
    Owner->SendNewDrawRequest();

    if(Owner->IsPlayer())
      game::AskForKeyPress(CONST_S("Bodypart destroyed! [press any key to continue]"));
  }
  else
    item::SignalBurn(Material);
}

void bodypart::Extinguish(truth SendMessages)
{
  if(Master)
  {
    item::Extinguish(SendMessages); //Master->Extinguish();
    Master->UpdatePictures();
  }
  else
    item::Extinguish(SendMessages);
}

void corpse::SignalBurn(material*)
{
  GetDeceased()->Disappear(this, "burn", &item::IsVeryCloseToBurning);
}

void bodypart::AddExtinguishMessage()
{
  if(Master)
  {
    character* Owner = GetMaster();

    if(Owner->IsPlayer())
      ADD_MESSAGE("The flames on your %s die away.", GetBodyPartName().CStr());
    else if(Owner->CanBeSeenByPlayer())
      ADD_MESSAGE("The flames on the %s of %s die away.", GetBodyPartName().CStr(), Owner->CHAR_NAME(DEFINITE));
  }
  else
    item::AddExtinguishMessage();
}

void bodypart::AddSpecialExtinguishMessageForPF()
{
  if(Master)
  {
    character* Owner = GetMaster();

    if(Owner->IsPlayer())
      ADD_MESSAGE("Your %s burns even more! But lo', even as it does so, the ashes "
                  "peel away from your %s and it is made new by some innate virtue!",
                  CHAR_NAME(UNARTICLED), GetBodyPartName().CStr());
    else if(Owner->CanBeSeenByPlayer())
      ADD_MESSAGE("The %s of %s burns even more! But lo', even as it does so, the "
                  "ashes peel away from %s %s and it is made new by some innate virtue!",
                  GetBodyPartName().CStr(), Owner->CHAR_NAME(DEFINITE),
                  Owner->GetPossessivePronoun().CStr(), GetBodyPartName().CStr());
  }
  else
    item::AddSpecialExtinguishMessageForPF();
}

void corpse::Extinguish(truth SendMessages)
{
  GetDeceased()->Extinguish(SendMessages);
}

void corpse::SignalDisappearance()
{
  GetDeceased()->Disappear(this, "disappear", &item::IsVeryCloseToDisappearance);
}

truth bodypart::CanBePiledWith(citem* Item, ccharacter* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer)
    && OwnerDescription == static_cast<const bodypart*>(Item)->OwnerDescription;
}

truth corpse::CanBePiledWith(citem* Item, ccharacter* Viewer) const
{
  if(GetType() != Item->GetType()
     || GetConfig() != Item->GetConfig()
     || GetWeight() != Item->GetWeight()
     || Viewer->GetCWeaponSkillLevel(this) != Viewer->GetCWeaponSkillLevel(Item)
     || Viewer->GetSWeaponSkillLevel(this) != Viewer->GetSWeaponSkillLevel(Item))
    return false;

  const corpse* Corpse = static_cast<const corpse*>(Item);

  if(Deceased->GetBodyParts() != Corpse->Deceased->GetBodyParts())
    return false;

  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
  {
    bodypart* BodyPart1 = Deceased->GetBodyPart(c);
    bodypart* BodyPart2 = Corpse->Deceased->GetBodyPart(c);

    if(!BodyPart1 && !BodyPart2)
      continue;

    if(!BodyPart1 || !BodyPart2 || !BodyPart1->CanBePiledWith(BodyPart2, Viewer))
      return false;
  }

  if(Deceased->GetName(UNARTICLED) != Corpse->Deceased->GetName(UNARTICLED))
    return false;

  return true;
}

void bodypart::Be()
{
  if(Master)
  {
    if(HP < MaxHP && ++SpillBloodCounter >= 4)
    {
      if(Master->IsEnabled())
      {
        if(IsBadlyHurt() && !Master->IsPolymorphed() && !(RAND() & 3))
          SpillBlood(1);
      }
      else if(!Master->IsPolymorphed() && !(RAND() & 3))
      {
        SpillBlood(1);
        HP += Max(((MaxHP - HP) >> 2), 1);
      }

      SpillBloodCounter = 0;
    }
    // Organics can have an active Be() function, if they are burning...
    // they will normally burn completely before they spoil
    if(Master->AllowSpoil() || !Master->IsEnabled() || !!IsBurning())
      MainMaterial->Be(ItemFlags);

    if(Exists() && LifeExpectancy)
    {
      if(LifeExpectancy == 1)
        Master->SignalDisappearance();
      else
        --LifeExpectancy;
    }
  }
  else
  {
    if(HP < MaxHP && ++SpillBloodCounter >= 4)
    {
      if(!(RAND() & 3))
      {
        SpillBlood(1);
        HP += Max(((MaxHP - HP) >> 2), 1);
      }

      SpillBloodCounter = 0;
    }

    item::Be();
  }
}

void bodypart::SpillBlood(int HowMuch, v2 Pos)
{
  if(HowMuch
     && (!Master || Master->SpillsBlood())
     && (IsAlive() || MainMaterial->IsLiquid())
     && !game::IsInWilderness())
    GetNearLSquare(Pos)->SpillFluid(0, CreateBlood(long(HowMuch * sqrt(BodyPartVolume) / 2)), false, false);
}

void bodypart::SpillBlood(int HowMuch)
{
  if(HowMuch
     && (!Master || Master->SpillsBlood())
     && (IsAlive() || MainMaterial->IsLiquid())
     && !game::IsInWilderness())
    for(int c = 0; c < GetSquaresUnder(); ++c)
      if(GetLSquareUnder(c))
        GetLSquareUnder(c)->SpillFluid(0, CreateBlood(long(HowMuch * sqrt(BodyPartVolume) / 2)), false, false);
}

void bodypart::SignalEnchantmentChange()
{
  if(Master && !Master->IsInitializing())
  {
    Master->CalculateAttributeBonuses();
    Master->CalculateBattleInfo();
  }
}

void arm::SignalEquipmentAdd(gearslot* Slot)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
  {
    item* Equipment = Slot->GetItem();

    if(Equipment->IsInCorrectSlot(EquipmentIndex))
      ApplyEquipmentAttributeBonuses(Equipment);

    if(EquipmentIndex == RIGHT_GAUNTLET_INDEX || EquipmentIndex == LEFT_GAUNTLET_INDEX)
      ApplyDexterityPenalty(Equipment);

    if(EquipmentIndex == RIGHT_WIELDED_INDEX || EquipmentIndex == LEFT_WIELDED_INDEX)
    {
      UpdateWieldedPicture();
      GetSquareUnder()->SendNewDrawRequest();
    }
  }

  if(Master)
    Master->SignalEquipmentAdd(EquipmentIndex);
}

void arm::SignalEquipmentRemoval(gearslot* Slot, citem* Item)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
    if(EquipmentIndex == RIGHT_WIELDED_INDEX || EquipmentIndex == LEFT_WIELDED_INDEX)
    {
      UpdateWieldedPicture();
      square* Square = GetSquareUnder();

      if(Square)
        Square->SendNewDrawRequest();
    }

  if(Master)
    Master->SignalEquipmentRemoval(EquipmentIndex, Item);
}

void leg::SignalEquipmentAdd(gearslot* Slot)
{
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(Master && !Master->IsInitializing())
  {
    item* Equipment = Slot->GetItem();

    if(Equipment->IsInCorrectSlot(EquipmentIndex))
      ApplyEquipmentAttributeBonuses(Equipment);

    if(EquipmentIndex == RIGHT_BOOT_INDEX || EquipmentIndex == LEFT_BOOT_INDEX)
      ApplyAgilityPenalty(Equipment);
  }

  if(Master)
    Master->SignalEquipmentAdd(EquipmentIndex);
}

void arm::ApplyEquipmentAttributeBonuses(item* Item)
{
  if(Item->AffectsArmStrength())
    StrengthBonus += Item->GetEnchantment();

  if(Item->AffectsDexterity())
    DexterityBonus += Item->GetEnchantment();
}

void leg::ApplyEquipmentAttributeBonuses(item* Item)
{
  if(Item->AffectsLegStrength())
  {
    StrengthBonus += Item->GetEnchantment();

    if(Master)
      Master->CalculateBurdenState();
  }

  if(Item->AffectsAgility())
    AgilityBonus += Item->GetEnchantment();
}

void arm::CalculateAttributeBonuses()
{
  StrengthBonus = DexterityBonus = 0;

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && Equipment->IsInCorrectSlot())
      ApplyEquipmentAttributeBonuses(Equipment);
  }

  ApplyDexterityPenalty(GetGauntlet());

  if(Master)
  {
    ApplyDexterityPenalty(GetExternalCloak());
    ApplyDexterityPenalty(GetExternalBodyArmor());
    ApplyStrengthBonus(GetExternalHelmet());
    ApplyStrengthBonus(GetExternalCloak());
    ApplyStrengthBonus(GetExternalBodyArmor());
    ApplyStrengthBonus(GetExternalBelt());
    ApplyDexterityBonus(GetExternalHelmet());
    ApplyDexterityBonus(GetExternalCloak());
    ApplyDexterityBonus(GetExternalBodyArmor());
    ApplyDexterityBonus(GetExternalBelt());
  }

  if(!UseMaterialAttributes())
  {
    StrengthBonus -= CalculateScarAttributePenalty(GetAttribute(ARM_STRENGTH, false));
    DexterityBonus -= CalculateScarAttributePenalty(GetAttribute(DEXTERITY, false));

    StrengthBonus -= CalculateBurnAttributePenalty(GetAttribute(ARM_STRENGTH, false));
    DexterityBonus -= CalculateBurnAttributePenalty(GetAttribute(DEXTERITY, false));
  }
}

void leg::CalculateAttributeBonuses()
{
  StrengthBonus = AgilityBonus = 0;

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && Equipment->IsInCorrectSlot())
      ApplyEquipmentAttributeBonuses(Equipment);
  }

  ApplyAgilityPenalty(GetBoot());

  if(Master)
  {
    ApplyAgilityPenalty(GetExternalCloak());
    ApplyAgilityPenalty(GetExternalBodyArmor());
    ApplyStrengthBonus(GetExternalHelmet());
    ApplyStrengthBonus(GetExternalCloak());
    ApplyStrengthBonus(GetExternalBodyArmor());
    ApplyStrengthBonus(GetExternalBelt());
    ApplyAgilityBonus(GetExternalHelmet());
    ApplyAgilityBonus(GetExternalCloak());
    ApplyAgilityBonus(GetExternalBodyArmor());
    ApplyAgilityBonus(GetExternalBelt());
  }

  if(!UseMaterialAttributes())
  {
    StrengthBonus -= CalculateScarAttributePenalty(GetAttribute(LEG_STRENGTH, false));
    AgilityBonus -= CalculateScarAttributePenalty(GetAttribute(AGILITY, false));

    StrengthBonus -= CalculateBurnAttributePenalty(GetAttribute(LEG_STRENGTH, false));
    AgilityBonus -= CalculateBurnAttributePenalty(GetAttribute(AGILITY, false));
  }
}

void humanoidtorso::SignalEquipmentAdd(gearslot* Slot)
{
  if(!Master)
    return;

  humanoid* Master = GetHumanoidMaster();
  int EquipmentIndex = Slot->GetEquipmentIndex();

  if(!Master->IsInitializing()
     && (EquipmentIndex == CLOAK_INDEX || EquipmentIndex == BODY_ARMOR_INDEX))
  {
    item* Item = Slot->GetItem();

    if(Master->GetRightArm())
      Master->GetRightArm()->ApplyDexterityPenalty(Item);

    if(Master->GetLeftArm())
      Master->GetLeftArm()->ApplyDexterityPenalty(Item);

    if(Master->GetRightLeg())
      Master->GetRightLeg()->ApplyAgilityPenalty(Item);

    if(Master->GetLeftLeg())
      Master->GetLeftLeg()->ApplyAgilityPenalty(Item);
  }

  Master->SignalEquipmentAdd(EquipmentIndex);
}

int arm::GetWieldedHitStrength() const
{
  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = GetWielded()->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
  {
    HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
    Requirement >>= 2;
  }

  return HitStrength - Requirement;
}

void arm::ApplyStrengthBonus(item* Item)
{
  if(Item && Item->AffectsArmStrength())
    StrengthBonus += Item->GetEnchantment() / 2;
}

void arm::ApplyDexterityBonus(item* Item)
{
  if(Item && Item->AffectsDexterity())
    DexterityBonus += Item->GetEnchantment() / 2;
}

void leg::ApplyStrengthBonus(item* Item)
{
  if(Item && Item->AffectsLegStrength())
    StrengthBonus += Item->GetEnchantment() / 2;
}

void leg::ApplyAgilityBonus(item* Item)
{
  if(Item && Item->AffectsAgility())
    AgilityBonus += Item->GetEnchantment() / 2;
}

void arm::ApplyDexterityPenalty(item* Item)
{
  if(Item)
    DexterityBonus -= Item->GetInElasticityPenalty(GetAttribute(DEXTERITY, false));
}

void leg::ApplyAgilityPenalty(item* Item)
{
  if(Item)
    AgilityBonus -= Item->GetInElasticityPenalty(GetAttribute(AGILITY, false));
}

int corpse::GetSpoilLevel() const
{
  int FlyAmount = 0;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart && FlyAmount < BodyPart->GetSpoilLevel())
      FlyAmount = BodyPart->GetSpoilLevel();
  }

  return FlyAmount;
}

void bodypart::SignalSpoilLevelChange(material* Material)
{
  if(Master)
    Master->SignalSpoilLevelChange();
  else
    item::SignalSpoilLevelChange(Material);
}

void bodypart::SignalBurnLevelChange()
{
//  if(Master)
//    Master->SignalBurnLevelChange();
//  else
    item::SignalBurnLevelChange();
}

void bodypart::SignalBurnLevelTransitionMessage()
{
cchar* MoreMsg = MainMaterial->GetBurnLevel() == NOT_BURNT ? "" : " more";

  if(Master)
  {
    if(Master->IsPlayer())
      ADD_MESSAGE("Your %s burns%s.", CHAR_NAME(UNARTICLED), MoreMsg);
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("The %s of %s burns%s.", CHAR_NAME(UNARTICLED), Master->CHAR_NAME(DEFINITE), MoreMsg);
  }
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s burns%s.", CHAR_NAME(DEFINITE), MoreMsg);
  else
    item::SignalBurnLevelTransitionMessage();
}

truth head::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetHelmet()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 2, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

truth humanoidtorso::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetBodyArmor()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetBelt()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue(), 1);

  if((Armor[2] = GetCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

truth arm::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetGauntlet()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 1, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

truth groin::DamageArmor(character* Damager, int Damage, int Type)
{
  return Master->GetTorso()->DamageArmor(Damager, Damage, Type);
}

truth leg::DamageArmor(character* Damager, int Damage, int Type)
{
  long AV[3] = { 0, 0, 0 }, AVSum = 0;
  item* Armor[3];

  if((Armor[0] = GetBoot()))
    AVSum += AV[0] = Max(Armor[0]->GetStrengthValue(), 1);

  if((Armor[1] = GetExternalBodyArmor()))
    AVSum += AV[1] = Max(Armor[1]->GetStrengthValue() >> 1, 1);

  if((Armor[2] = GetExternalCloak()))
    AVSum += AV[2] = Max(Armor[2]->GetStrengthValue(), 1);

  return AVSum ? Armor[femath::WeightedRand(AV, AVSum)]->ReceiveDamage(Damager, Damage, Type) : false;
}

truth bodypart::CanBeEatenByAI(ccharacter* Who) const
{
  return item::CanBeEatenByAI(Who) && !(Who->IsPet() && PLAYER->HasHadBodyPart(this));
}

int bodypart::GetConditionColorIndex() const
{
  if(HP <= 1 && MaxHP > 1)
    return 0;
  else if((HP << 1) + HP < MaxHP)
    return 1;
  else if((HP << 1) + HP < MaxHP << 1)
    return 2;
  else if(HP < MaxHP)
    return 3;
  else
    return 4;
}

truth arm::CheckIfWeaponTooHeavy(cchar* WeaponDescription) const
{
  if(!IsUsable())
  {
    ADD_MESSAGE("%s %s is not usable.", Master->CHAR_POSSESSIVE_PRONOUN, GetBodyPartName().CStr());
    return !game::TruthQuestion(CONST_S("Continue anyway? [y/N]"));
  }

  int HitStrength = GetAttribute(ARM_STRENGTH);
  int Requirement = GetWielded()->GetStrengthRequirement();

  if(TwoHandWieldIsActive())
  {
    HitStrength += GetPairArm()->GetAttribute(ARM_STRENGTH);
    Requirement >>= 2;

    if(HitStrength - Requirement < 10)
    {
      if(HitStrength <= Requirement)
        ADD_MESSAGE("%s cannot use %s. Wielding it with two hands requires %d strength.",
                    Master->CHAR_DESCRIPTION(DEFINITE), WeaponDescription, (Requirement >> 1) + 1);
      else if(HitStrength - Requirement < 4)
        ADD_MESSAGE("Using %s even with two hands is extremely difficult for %s.",
                    WeaponDescription, Master->CHAR_DESCRIPTION(DEFINITE));
      else if(HitStrength - Requirement < 7)
        ADD_MESSAGE("%s %s much trouble using %s even with two hands.",
                    Master->CHAR_DESCRIPTION(DEFINITE), Master->IsPlayer() ? "have" : "has", WeaponDescription);
      else
        ADD_MESSAGE("It is somewhat difficult for %s to use %s even with two hands.",
                    Master->CHAR_DESCRIPTION(DEFINITE), WeaponDescription);

      return !game::TruthQuestion(CONST_S("Continue anyway? [y/N]"));
    }
  }
  else
  {
    if(HitStrength - Requirement < 10)
    {
      festring OtherHandInfo;
      cchar* HandInfo = "";

      if(GetWielded()->IsTwoHanded())
      {
        if(GetPairArm() && !GetPairArm()->IsUsable())
          OtherHandInfo = Master->GetPossessivePronoun() + " other arm is unusable. ";

        HandInfo = " with one hand";
      }

      if(HitStrength <= Requirement)
        ADD_MESSAGE("%s%s cannot use %s. Wielding it%s requires %d strength.", OtherHandInfo.CStr(),
                    Master->GetDescription(DEFINITE).CapitalizeCopy().CStr(),
                    WeaponDescription, HandInfo, Requirement + 1);
      else if(HitStrength - Requirement < 4)
        ADD_MESSAGE("%sUsing %s%s is extremely difficult for %s.", OtherHandInfo.CStr(),
                    WeaponDescription, HandInfo, Master->CHAR_DESCRIPTION(DEFINITE));
      else if(HitStrength - Requirement < 7)
        ADD_MESSAGE("%s%s %s much trouble using %s%s.", OtherHandInfo.CStr(),
                    Master->GetDescription(DEFINITE).CapitalizeCopy().CStr(),
                    Master->IsPlayer() ? "have" : "has", WeaponDescription, HandInfo);
      else
        ADD_MESSAGE("%sIt is somewhat difficult for %s to use %s%s.", OtherHandInfo.CStr(),
                    Master->CHAR_DESCRIPTION(DEFINITE), WeaponDescription, HandInfo);

      return !game::TruthQuestion(CONST_S("Continue anyway? [y/N]"));
    }
  }

  return false;
}

int corpse::GetArticleMode() const
{
  return Deceased->LeftOversAreUnique() ? FORCE_THE : 0;
}

head* head::Behead()
{
  RemoveFromSlot();
  return this;
}

truth arm::EditAllAttributes(int Amount)
{
  LimitRef(StrengthExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  LimitRef(DexterityExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  return (Amount < 0
          && (StrengthExperience != MIN_EXP || DexterityExperience != MIN_EXP))
    || (Amount > 0
        && (StrengthExperience != MAX_EXP || DexterityExperience != MAX_EXP));
}

truth leg::EditAllAttributes(int Amount)
{
  LimitRef(StrengthExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  return (Amount < 0
          && (StrengthExperience != MIN_EXP || AgilityExperience != MIN_EXP))
    || (Amount > 0
        && (StrengthExperience != MAX_EXP || AgilityExperience != MAX_EXP));
}

#ifdef WIZARD

void arm::AddAttackInfo(felist& List) const
{
  if(GetDamage())
  {
    festring Entry = CONST_S("   ");

    if(GetWielded())
    {
      GetWielded()->AddName(Entry, UNARTICLED);

      if(TwoHandWieldIsActive())
        Entry << " (b)";
    }
    else
      Entry << "melee attack";

    Entry.Resize(50);
    Entry << GetMinDamage() << '-' << GetMaxDamage();
    Entry.Resize(60);
    Entry << int(GetToHitValue());
    Entry.Resize(70);
    Entry << GetAPCost();
    List.AddEntry(Entry, LIGHT_GRAY);
  }
}

void arm::AddDefenceInfo(felist& List) const
{
  if(GetWielded())
  {
    festring Entry = CONST_S("   ");
    GetWielded()->AddName(Entry, UNARTICLED);
    Entry.Resize(50);
    Entry << int(GetBlockValue());
    Entry.Resize(70);
    Entry << GetBlockCapability();
    List.AddEntry(Entry, LIGHT_GRAY);
  }
}

#else

void arm::AddAttackInfo(felist&) const { }
void arm::AddDefenceInfo(felist&) const { }

#endif

void arm::UpdateWieldedPicture()
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    truth WasAnimated = MasterIsAnimated();
    item* Wielded = GetWielded();

    if(Wielded && Master)
    {
      int SpecialFlags = (IsRightArm() ? 0 : MIRROR)|ST_WIELDED|(Wielded->GetSpecialFlags()&~0x3F);
      Wielded->UpdatePictures(WieldedGraphicData,
                              Master->GetWieldedPosition(),
                              SpecialFlags,
                              GetMaxAlpha(),
                              GR_HUMANOID,
                              static_cast<bposretriever>(&item::GetWieldedBitmapPos));

      if(ShowFluids())
        Wielded->CheckFluidGearPictures(Wielded->GetWieldedBitmapPos(0), SpecialFlags, false);
    }
    else
      WieldedGraphicData.Retire();

    if(!WasAnimated != !MasterIsAnimated())
      SignalAnimationStateChange(WasAnimated);
  }
}

void arm::DrawWielded(blitdata& BlitData) const
{
  DrawEquipment(WieldedGraphicData, BlitData);

  if(ShowFluids() && GetWielded())
    GetWielded()->DrawFluidGearPictures(BlitData, IsRightArm() ? 0 : MIRROR);
}

void arm::UpdatePictures()
{
  bodypart::UpdatePictures();
  UpdateWieldedPicture();
}

void bodypart::Draw(blitdata& BlitData) const
{
  cint AF = GraphicData.AnimationFrames;
  cint F = !(BlitData.CustomData & ALLOW_ANIMATE) || AF == 1 ? 0 : GET_TICK() & (AF - 1);
  cbitmap* P = GraphicData.Picture[F];

  if(BlitData.CustomData & ALLOW_ALPHA)
    P->AlphaPriorityBlit(BlitData);
  else
    P->MaskedPriorityBlit(BlitData);

  if(Fluid && ShowFluids())
    DrawFluids(BlitData);

  DrawArmor(BlitData);
}

void leg::AddAttackInfo(felist& List) const
{
  festring Entry = CONST_S("   kick attack");
  Entry.Resize(50);
  Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
  Entry.Resize(60);
  Entry << int(GetKickToHitValue());
  Entry.Resize(70);
  Entry << GetKickAPCost();
  List.AddEntry(Entry, LIGHT_GRAY);
}

void corpse::PreProcessForBone()
{
  item::PreProcessForBone();

  if(!Deceased->PreProcessForBone())
  {
    RemoveFromSlot();
    SendToHell();
  }
}

void corpse::PostProcessForBone()
{
  item::PostProcessForBone();

  if(!Deceased->PostProcessForBone())
  {
    RemoveFromSlot();
    SendToHell();
  }
}

void corpse::FinalProcessForBone()
{
  item::FinalProcessForBone();
  Deceased->FinalProcessForBone();
}

truth bodypart::IsRepairable(ccharacter*) const
{
  return !CanRegenerate() && (GetHP() < GetMaxHP() || IsRusted() || IsBurnt());
}

truth corpse::SuckSoul(character* Soul, character* Summoner)
{
  v2 Pos = Soul->GetPos();

  if(Deceased->SuckSoul(Soul))
  {
    Soul->Remove();
    character* Deceased = GetDeceased();

    if(RaiseTheDead(Summoner))
    {
      Soul->SendToHell();
      return true;
    }
    else
    {
      Deceased->SetSoulID(Soul->GetID());
      Soul->PutTo(Pos);
      return false;
    }
  }
  else
    return false;
}

double arm::GetTypeDamage(ccharacter* Enemy) const
{
  if(!GetWielded() || !GetWielded()->IsGoodWithPlants() || !Enemy->IsPlant())
    return Damage;
  else
    return Damage * 1.5;
}

void largetorso::Draw(blitdata& BlitData) const
{
  LargeDraw(BlitData);
}

void largecorpse::Draw(blitdata& BlitData) const
{
  LargeDraw(BlitData);
}

void largetorso::SignalStackAdd(stackslot* StackSlot, void (stack::*AddHandler)(item*, truth))
{
  if(!Slot[0])
  {
    Slot[0] = StackSlot;
    v2 Pos = GetPos();
    level* Level = GetLevel();

    for(int c = 1; c < 4; ++c)
      (Level->GetLSquare(Pos + game::GetLargeMoveVector(12 + c))->GetStack()->*AddHandler)(this, false);
  }
  else
    for(int c = 1; c < 4; ++c)
      if(!Slot[c])
      {
        Slot[c] = StackSlot;
        return;
      }
}

int largetorso::GetSquareIndex(v2 Pos) const
{
  v2 RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

void largecorpse::SignalStackAdd(stackslot* StackSlot, void (stack::*AddHandler)(item*, truth))
{
  if(!Slot[0])
  {
    Slot[0] = StackSlot;
    v2 Pos = GetPos();
    level* Level = GetLevel();

    for(int c = 1; c < 4; ++c)
      (Level->GetLSquare(Pos + game::GetLargeMoveVector(12 + c))->GetStack()->*AddHandler)(this, false);
  }
  else
    for(int c = 1; c < 4; ++c)
      if(!Slot[c])
      {
        Slot[c] = StackSlot;
        return;
      }
}

int largecorpse::GetSquareIndex(v2 Pos) const
{
  v2 RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

character* corpse::TryNecromancy(character* Summoner)
{
  if(Summoner && Summoner->IsPlayer())
    game::DoEvilDeed(50);

  character* Zombie = GetDeceased()->CreateZombie();

  if(Zombie)
  {
    v2 Pos = GetPos();
    RemoveFromSlot();
    Zombie->ChangeTeam(Summoner ? Summoner->GetTeam() : game::GetTeam(MONSTER_TEAM));
    Zombie->PutToOrNear(Pos);
    Zombie->SignalStepFrom(0);
    SendToHell();
    return Zombie;
  }

  return 0;
}

item* head::GetArmorToReceiveFluid(truth) const
{
  item* Helmet = GetHelmet();

  if(Helmet && Helmet->GetCoverPercentile() > RAND() % 100)
    return Helmet;
  else
    return 0;
}

item* humanoidtorso::GetArmorToReceiveFluid(truth) const
{
  item* Cloak = GetCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* Belt = GetBelt();

  if(Belt && !(RAND() % 10))
    return Belt;

  item* BodyArmor = GetBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* arm::GetArmorToReceiveFluid(truth) const
{
  item* Cloak = GetExternalCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* Wielded = GetWielded();

  if(Wielded && !(RAND() % 3))
    return Wielded;

  item* Gauntlet = GetGauntlet();

  if(Gauntlet && RAND() & 1)
    return Gauntlet;

  item* BodyArmor = GetExternalBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* groin::GetArmorToReceiveFluid(truth) const
{
  item* Cloak = GetExternalCloak();

  if(Cloak && !(RAND() % 3))
    return Cloak;

  item* BodyArmor = GetExternalBodyArmor();
  return BodyArmor ? BodyArmor : 0;
}

item* leg::GetArmorToReceiveFluid(truth SteppedOn) const
{
  if(SteppedOn)
  {
    item* Boot = GetBoot();
    return Boot ? Boot : 0;
  }
  else
  {
    item* Cloak = GetExternalCloak();

    if(Cloak && !(RAND() % 3))
      return Cloak;

    item* Boot = GetBoot();

    if(Boot && RAND() & 1)
      return Boot;

    item* BodyArmor = GetExternalBodyArmor();
    return BodyArmor ? BodyArmor : 0;
  }
}

void bodypart::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  if(Master)
  {
    item* Armor = GetArmorToReceiveFluid(false);

    if(Armor)
      Armor->SpillFluid(Spiller, Liquid);
    else if(GetMaster())
    {
      if(Liquid->GetVolume())
        AddFluid(Liquid, "", SquareIndex, false);
      else
        delete Liquid;
    }
  }
  else
    item::SpillFluid(Spiller, Liquid, SquareIndex);
}

void bodypart::StayOn(liquid* Liquid)
{
  item* Armor = GetArmorToReceiveFluid(true);

  if(Armor)
    Liquid->TouchEffect(Armor, CONST_S(""));
  else if(GetMaster())
    Liquid->TouchEffect(GetMaster(), GetBodyPartIndex());
}

liquid* bodypart::CreateBlood(long Volume) const
{
  return liquid::Spawn(GetBloodMaterial(), Volume);
}

int corpse::GetRustDataA() const
{
  return Deceased->GetTorso()->GetMainMaterial()->GetRustData();
}

int corpse::GetBurnDataA() const
{
  return Deceased->GetTorso()->GetMainMaterial()->GetBurnData();
}

void bodypart::UpdateArmorPicture(graphicdata& GData, item* Armor, int SpecialFlags,
                                  v2 (item::*Retriever)(int) const, truth BodyArmor) const
{
  if(Armor && Master)
  {
    Armor->UpdatePictures(GData,
                          ZERO_V2,
                          SpecialFlags|Armor->GetSpecialFlags(),
                          GetMaxAlpha(),
                          GR_HUMANOID,
                          static_cast<bposretriever>(Retriever));
    Armor->CheckFluidGearPictures((Armor->*Retriever)(0), SpecialFlags, BodyArmor);
  }
  else
    GData.Retire();
}

truth playerkindhead::UpdateArmorPictures()
{
  UpdateHeadArmorPictures(HelmetGraphicData);
  return true;
}

truth playerkindtorso::UpdateArmorPictures()
{
  UpdateTorsoArmorPictures(TorsoArmorGraphicData,
                           CloakGraphicData,
                           BeltGraphicData);
  return true;
}

truth playerkindrightarm::UpdateArmorPictures()
{
  UpdateArmArmorPictures(ArmArmorGraphicData,
                         GauntletGraphicData,
                         ST_RIGHT_ARM);
  return true;
}

truth playerkindleftarm::UpdateArmorPictures()
{
  UpdateArmArmorPictures(ArmArmorGraphicData,
                         GauntletGraphicData,
                         ST_LEFT_ARM);
  return true;
}

truth playerkindgroin::UpdateArmorPictures()
{
  UpdateGroinArmorPictures(GroinArmorGraphicData);
  return true;
}

truth playerkindrightleg::UpdateArmorPictures()
{
  UpdateLegArmorPictures(LegArmorGraphicData,
                         BootGraphicData,
                         ST_RIGHT_LEG);
  return true;
}

truth playerkindleftleg::UpdateArmorPictures()
{
  UpdateLegArmorPictures(LegArmorGraphicData,
                         BootGraphicData,
                         ST_LEFT_LEG);
  return true;
}

void head::UpdateHeadArmorPictures(graphicdata& HelmetGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    UpdateArmorPicture(HelmetGraphicData,
                       GetHelmet(),
                       ST_OTHER_BODYPART,
                       &item::GetHelmetBitmapPos);
  }
}

void humanoidtorso::UpdateTorsoArmorPictures(graphicdata& TorsoArmorGraphicData,
                                             graphicdata& CloakGraphicData,
                                             graphicdata& BeltGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    UpdateArmorPicture(TorsoArmorGraphicData,
                       GetBodyArmor(),
                       ST_OTHER_BODYPART,
                       &item::GetTorsoArmorBitmapPos,
                       true);
    UpdateArmorPicture(CloakGraphicData,
                       GetCloak(),
                       ST_OTHER_BODYPART,
                       &item::GetCloakBitmapPos);
    UpdateArmorPicture(BeltGraphicData,
                       GetBelt(),
                       ST_OTHER_BODYPART,
                       &item::GetBeltBitmapPos);
  }
}

void arm::UpdateArmArmorPictures(graphicdata& ArmArmorGraphicData,
                                 graphicdata& GauntletGraphicData,
                                 int SpecialFlags) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    UpdateArmorPicture(ArmArmorGraphicData,
                       Master ? GetExternalBodyArmor() : 0,
                       SpecialFlags,
                       GetAttribute(ARM_STRENGTH, false) >= 20 ?
                         &item::GetAthleteArmArmorBitmapPos : &item::GetArmArmorBitmapPos,
                       true);
    UpdateArmorPicture(GauntletGraphicData,
                       GetGauntlet(),
                       SpecialFlags,
                       &item::GetGauntletBitmapPos);
  }
}

void groin::UpdateGroinArmorPictures(graphicdata& GroinArmorGraphicData) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    UpdateArmorPicture(GroinArmorGraphicData,
                       Master ? GetExternalBodyArmor() : 0,
                       ST_GROIN,
                       &item::GetLegArmorBitmapPos,
                       true);
  }
}

void leg::UpdateLegArmorPictures(graphicdata& LegArmorGraphicData,
                                 graphicdata& BootGraphicData,
                                 int SpecialFlags) const
{
  if(!Master || !Master->PictureUpdatesAreForbidden())
  {
    UpdateArmorPicture(LegArmorGraphicData,
                       Master ? GetExternalBodyArmor() : 0,
                       SpecialFlags,
                       &item::GetLegArmorBitmapPos,
                       true);
    UpdateArmorPicture(BootGraphicData,
                       GetBoot(),
                       SpecialFlags,
                       &item::GetBootBitmapPos);
  }
}

void bodypart::DrawEquipment(const graphicdata& GraphicData, blitdata& BlitData) const
{
  int EAF = GraphicData.AnimationFrames;

  if(EAF)
  {
    int F = !(BlitData.CustomData & ALLOW_ANIMATE) || EAF == 1 ? 0 : GET_TICK() & (EAF - 1);
    GraphicData.Picture[F]->AlphaPriorityBlit(BlitData);
  }
}

void playerkindhead::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(HelmetGraphicData, BlitData);

  if(GetHelmet())
    GetHelmet()->DrawFluidGearPictures(BlitData);
}

void playerkindtorso::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(TorsoArmorGraphicData, BlitData);

  if(GetBodyArmor())
    GetBodyArmor()->DrawFluidGearPictures(BlitData);

  DrawEquipment(CloakGraphicData, BlitData);

  if(GetCloak())
    GetCloak()->DrawFluidGearPictures(BlitData);

  DrawEquipment(BeltGraphicData, BlitData);

  if(GetBelt())
    GetBelt()->DrawFluidGearPictures(BlitData);
}

void playerkindrightarm::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(ArmArmorGraphicData, BlitData);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(BlitData, ST_RIGHT_ARM);

  DrawEquipment(GauntletGraphicData, BlitData);

  if(GetGauntlet())
    GetGauntlet()->DrawFluidGearPictures(BlitData);
}

void playerkindleftarm::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(ArmArmorGraphicData, BlitData);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(BlitData, ST_LEFT_ARM);

  DrawEquipment(GauntletGraphicData, BlitData);

  if(GetGauntlet())
    GetGauntlet()->DrawFluidGearPictures(BlitData);
}

void playerkindgroin::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(GroinArmorGraphicData, BlitData);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(BlitData, ST_GROIN);
}

void playerkindrightleg::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(LegArmorGraphicData, BlitData);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(BlitData, ST_RIGHT_LEG);

  DrawEquipment(BootGraphicData, BlitData);

  if(GetBoot())
    GetBoot()->DrawFluidGearPictures(BlitData);
}

void playerkindleftleg::DrawArmor(blitdata& BlitData) const
{
  DrawEquipment(LegArmorGraphicData, BlitData);

  if(Master && GetExternalBodyArmor())
    GetExternalBodyArmor()->DrawFluidBodyArmorPictures(BlitData, ST_LEFT_LEG);

  DrawEquipment(BootGraphicData, BlitData);

  if(GetBoot())
    GetBoot()->DrawFluidGearPictures(BlitData);
}

void playerkindhead::Save(outputfile& SaveFile) const
{
  head::Save(SaveFile);
  SaveFile << HelmetGraphicData;
}

void playerkindhead::Load(inputfile& SaveFile)
{
  head::Load(SaveFile);
  SaveFile >> HelmetGraphicData;
}

void playerkindtorso::Save(outputfile& SaveFile) const
{
  humanoidtorso::Save(SaveFile);
  SaveFile << TorsoArmorGraphicData << CloakGraphicData << BeltGraphicData;
}

void playerkindtorso::Load(inputfile& SaveFile)
{
  humanoidtorso::Load(SaveFile);
  SaveFile >> TorsoArmorGraphicData >> CloakGraphicData >> BeltGraphicData;
}

void playerkindrightarm::Save(outputfile& SaveFile) const
{
  rightarm::Save(SaveFile);
  SaveFile << ArmArmorGraphicData << GauntletGraphicData;
}

void playerkindrightarm::Load(inputfile& SaveFile)
{
  rightarm::Load(SaveFile);
  SaveFile >> ArmArmorGraphicData >> GauntletGraphicData;
}

void playerkindleftarm::Save(outputfile& SaveFile) const
{
  leftarm::Save(SaveFile);
  SaveFile << ArmArmorGraphicData << GauntletGraphicData;
}

void playerkindleftarm::Load(inputfile& SaveFile)
{
  leftarm::Load(SaveFile);
  SaveFile >> ArmArmorGraphicData >> GauntletGraphicData;
}

void playerkindgroin::Save(outputfile& SaveFile) const
{
  groin::Save(SaveFile);
  SaveFile << GroinArmorGraphicData;
}

void playerkindgroin::Load(inputfile& SaveFile)
{
  groin::Load(SaveFile);
  SaveFile >> GroinArmorGraphicData;
}

void playerkindrightleg::Save(outputfile& SaveFile) const
{
  rightleg::Save(SaveFile);
  SaveFile << LegArmorGraphicData << BootGraphicData;
}

void playerkindrightleg::Load(inputfile& SaveFile)
{
  rightleg::Load(SaveFile);
  SaveFile >> LegArmorGraphicData >> BootGraphicData;
}

void playerkindleftleg::Save(outputfile& SaveFile) const
{
  leftleg::Save(SaveFile);
  SaveFile << LegArmorGraphicData << BootGraphicData;
}

void playerkindleftleg::Load(inputfile& SaveFile)
{
  leftleg::Load(SaveFile);
  SaveFile >> LegArmorGraphicData >> BootGraphicData;
}

truth bodypart::MasterIsAnimated() const
{
  return Master && !Master->IsInitializing() && Master->IsAnimated();
}

void bodypart::UpdatePictures()
{
  truth WasAnimated = MasterIsAnimated();

  item::UpdatePictures();
  UpdateArmorPictures();

  if(!WasAnimated != !MasterIsAnimated())
    SignalAnimationStateChange(WasAnimated);
}

void playerkindtorso::SignalVolumeAndWeightChange()
{
  humanoidtorso::SignalVolumeAndWeightChange();

  if(Master && !Master->IsInitializing())
    Master->UpdatePictures();
}

void bodypart::ReceiveAcid(material* Material, cfestring& LocationName, long Modifier)
{
  if(Master && MainMaterial->GetInteractionFlags() & CAN_DISSOLVE)
  {
    long Tries = Modifier / 1000;
    Modifier -= Tries * 1000; //opt%?
    int Damage = 0;

    for(long c = 0; c < Tries; ++c)
      if(!(RAND() % 100))
        ++Damage;

    if(Modifier && !(RAND() % 100000 / Modifier))
      ++Damage;

    if(Damage)
    {
      ulong Minute = game::GetTotalMinutes();
      character* Master = this->Master;

      if(Master->GetLastAcidMsgMin() != Minute && (Master->CanBeSeenByPlayer() || Master->IsPlayer()))
      {
        Master->SetLastAcidMsgMin(Minute);
        cchar* MName = Material->GetName(false, false).CStr();

        if(Master->IsPlayer())
        {
          cchar* TName = LocationName.IsEmpty() ? GetBodyPartName().CStr() : LocationName.CStr();
          ADD_MESSAGE("Acidous %s dissolves your %s.", MName, TName);
        }
        else
          ADD_MESSAGE("Acidous %s dissolves %s.", MName, Master->CHAR_NAME(DEFINITE));
      }

      Master->ReceiveBodyPartDamage(0, Damage, ACID, GetBodyPartIndex(), YOURSELF, false, false, false);
      ulong DeathFlags = Material->IsStuckTo(Master) ? IGNORE_TRAPS : 0;
      Master->CheckDeath(CONST_S("dissolved by ") + Material->GetName(), 0, DeathFlags);
    }
  }
}

void bodypart::TryToRust(long LiquidModifier)
{
  if(MainMaterial->TryToRust(LiquidModifier << 4))
  {
    cchar* MoreMsg = MainMaterial->GetRustLevel() == NOT_RUSTED ? "" : " more";

    if(Master)
    {
      if(Master->IsPlayer())
        ADD_MESSAGE("Your %s rusts%s.", CHAR_NAME(UNARTICLED), MoreMsg);
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("The %s of %s rusts%s.", CHAR_NAME(UNARTICLED), Master->CHAR_NAME(DEFINITE), MoreMsg);
    }
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s rusts%s.", CHAR_NAME(DEFINITE), MoreMsg);

    MainMaterial->SetRustLevel(MainMaterial->GetRustLevel() + 1);
  }
}

material* corpse::GetConsumeMaterial(ccharacter* Consumer, materialpredicate Predicate) const
{
  for(int c = GetDeceased()->GetBodyParts() - 1; c; --c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart)
    {
      material* CM = BodyPart->GetConsumeMaterial(Consumer, Predicate);

      if(CM)
        return CM;
    }
  }

  return GetDeceased()->GetTorso()->GetConsumeMaterial(Consumer, Predicate);
}

void corpse::Cannibalize()
{
  item::Cannibalize();

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart)
      BodyPart->Cannibalize();
  }
}

material* bodypart::RemoveMaterial(material* Material)
{
  if(Master && GetBodyPartIndex() == TORSO_INDEX)
    return Master->GetMotherEntity()->RemoveMaterial(Material); // gum
  else
    return item::RemoveMaterial(Material);
}

void arm::CopyAttributes(const bodypart* BodyPart)
{
  const arm* Arm = static_cast<const arm*>(BodyPart);
  StrengthExperience = Arm->StrengthExperience;
  DexterityExperience = Arm->DexterityExperience;
}

void leg::CopyAttributes(const bodypart* BodyPart)
{
  const leg* Leg = static_cast<const leg*>(BodyPart);
  StrengthExperience = Leg->StrengthExperience;
  AgilityExperience = Leg->AgilityExperience;
}

truth corpse::DetectMaterial(const material* Material) const
{
  return GetDeceased()->DetectMaterial(Material);
}

void bodypart::DestroyBodyPart(stack* Stack)
{
  int Lumps = 1 + RAND() % 3;
  long LumpVolume = Volume / Lumps >> 2;

  if(LumpVolume >= 10)
    for(int c = 0; c < Lumps; ++c)
    {
      item* Lump = GetMainMaterial()->CreateNaturalForm(LumpVolume + RAND() % LumpVolume);
      Stack->AddItem(Lump);
    }

  SendToHell();
}

v2 magicmushroomtorso::GetBitmapPos(int Frame) const
{
  v2 BasePos = torso::GetBitmapPos(Frame);
  Frame &= 0x3F;

  if(!(Frame & 0x30))
  {
    if(Frame <= 8)
      return v2(BasePos.X + 64 - (abs(Frame - 4) << 4), BasePos.Y);
    else
      return v2(BasePos.X + 64 - (abs(Frame - 12) << 4), BasePos.Y + 16);
  }
  else
    return BasePos;
}

v2 dogtorso::GetBitmapPos(int Frame) const
{
  v2 BasePos = torso::GetBitmapPos(Frame);

  if(Frame >= GraphicData.AnimationFrames >> 1)
    BasePos.X += 32;

  return v2(BasePos.X + ((Frame & 4) << 2), BasePos.Y);
}

void dogtorso::Draw(blitdata& BlitData) const
{
  cint AF = GraphicData.AnimationFrames >> 1;
  int Index = !(BlitData.CustomData & ALLOW_ANIMATE) || AF == 1 ? 0 : GET_TICK() & (AF - 1);

  if(GetHP() << 1 <= GetMaxHP())
    Index += AF;

  cbitmap* P = GraphicData.Picture[Index];

  if(BlitData.CustomData & ALLOW_ALPHA)
    P->AlphaPriorityBlit(BlitData);
  else
    P->MaskedPriorityBlit(BlitData);
}

void corpse::SetLifeExpectancy(int Base, int RandPlus)
{
  Deceased->SetLifeExpectancy(Base, RandPlus);
}

void corpse::Be()
{
  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = Deceased->GetBodyPart(c);

    if(BodyPart)
      BodyPart->Be();
  }
}

void bodypart::SetLifeExpectancy(int Base, int RandPlus)
{
  LifeExpectancy = RandPlus > 1 ? Base + RAND_N(RandPlus) : Base;

  if(!Master)
    Enable();
}

void bodypart::SpecialEatEffect(character* Eater, int Amount)
{
  Amount >>= 6;

  if(Amount
     && (!Master || Master->SpillsBlood())
     && (IsAlive() || MainMaterial->IsLiquid())
     && !game::IsInWilderness())
  {
    if(Eater->GetVirtualHead())
      Eater->GetVirtualHead()->SpillFluid(Eater, CreateBlood(Amount));

    Eater->GetTorso()->SpillFluid(Eater, CreateBlood(Amount));
  }
}

truth corpse::IsValuable() const
{
  for(int c = 0; c < Deceased->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = Deceased->GetBodyPart(c);

    if(BodyPart && BodyPart->IsValuable())
      return true;
  }

  return false;
}

truth corpse::Necromancy(character* Necromancer)
{
  if(Necromancer && Necromancer->IsPlayer())
    game::DoEvilDeed(50);

  character* Zombie = GetDeceased()->CreateZombie();

  if(Zombie)
  {
    Zombie->ChangeTeam(Necromancer ? Necromancer->GetTeam() : game::GetTeam(MONSTER_TEAM));
    Zombie->PutToOrNear(GetPos());
    RemoveFromSlot();
    SendToHell();

    if(Zombie->CanBeSeenByPlayer())
      ADD_MESSAGE("%s rises back to cursed undead life.", Zombie->CHAR_DESCRIPTION(INDEFINITE));

    Zombie->SignalStepFrom(0);
    return true;
  }
  else
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s vibrates for some time.", CHAR_NAME(DEFINITE));

    return false;
  }
}

alpha mysticfrogtorso::GetOutlineAlpha(int Frame) const
{
  Frame &= 31;
  return Frame * (31 - Frame) >> 1;
}

col16 mysticfrogtorso::GetOutlineColor(int Frame) const
{
  switch((Frame&127) >> 5)
  {
   case 0: return BLUE;
   case 1: return GREEN;
   case 2: return RED;
   case 3: return YELLOW;
  }

  return TRANSPARENT_COLOR;
}

void bodypart::UpdateFlags()
{
  if((HP << 1) + HP < MaxHP || (HP == 1 && MaxHP != 1))
    Flags |= BADLY_HURT;
  else
    Flags &= ~BADLY_HURT;

  if(Master->BodyPartIsStuck(GetBodyPartIndex()))
    Flags |= STUCK;
  else
    Flags &= ~STUCK;
}

void head::SignalPossibleUsabilityChange()
{
  ulong OldFlags = Flags;
  UpdateFlags();

  if(!Master->IsInitializing() && HP > 0
     && Flags & BADLY_HURT && !(OldFlags & BADLY_HURT))
    switch(RAND_N(8))
    {
     case 0:
     case 1:
     case 2: Master->LoseConsciousness(50 + RAND_N(50)); break;
     case 3:
     case 4:
     case 5: Master->BeginTemporaryState(CONFUSED, 500 + RAND_N(500)); break;
     case 6:
      if(Master->IsPlayer() && !RAND_N(3))
      {
        if(RAND_N(5))
        {
          ADD_MESSAGE("Your memory becomes blurred.");
          GetLevel()->Amnesia(25 + RAND_N(50));
          Master->EditExperience(INTELLIGENCE, -80, 1 << 13);
          game::SendLOSUpdateRequest();
        }
        else
        {
          ADD_MESSAGE("A terrible concussion garbles your consciousness.");
          Master->BeginTemporaryState(CONFUSED, 5000 + RAND_N(5000));
          Master->EditExperience(INTELLIGENCE, -100, 1 << 14);
          GetLevel()->BlurMemory();
          game::SendLOSUpdateRequest();
        }
      }
      else
        Master->EditExperience(INTELLIGENCE, -60, 1 << 12);

      break;
     case 7:
      Master->ForgetRandomThing();
    }
}

void arm::SignalPossibleUsabilityChange()
{
  ulong OldFlags = Flags;
  UpdateFlags();

  if(Flags != OldFlags && !Master->IsInitializing())
    Master->CalculateBattleInfo();
}

void leg::SignalPossibleUsabilityChange()
{
  ulong OldFlags = Flags;
  UpdateFlags();

  if(Flags != OldFlags && !Master->IsInitializing())
    Master->CalculateBattleInfo();
}

void bodypart::IncreaseHP()
{
  ++HP;
  RemoveDamageIDs(1);
  SignalPossibleUsabilityChange();
}

void bodypart::FastRestoreHP()
{
  HP = MaxHP;
  DamageID.clear();
  SignalPossibleUsabilityChange();
}

void bodypart::RestoreHP()
{
  HP = MaxHP;
  DamageID.clear();
  SignalPossibleUsabilityChange();
  Master->CalculateHP();
}

void bodypart::SetIsUnique(truth What)
{
  if(What)
    Flags |= UNIQUE;
  else
    Flags &= ~UNIQUE;
}

void bodypart::SetIsInfectedByLeprosy(truth What)
{
  MainMaterial->SetIsInfectedByLeprosy(What);
}

void bodypart::SetSparkleFlags(int What)
{
  cint S = SPARKLING_B|SPARKLING_C|SPARKLING_D;
  Flags = (Flags & ~(S << BODYPART_SPARKLE_SHIFT)) | ((What & S) << BODYPART_SPARKLE_SHIFT);
}

truth arm::IsAnimated() const
{
  return (WieldedGraphicData.AnimationFrames > 1) || IsBurning();
}

void bodypart::SignalAnimationStateChange(truth WasAnimated)
{
  if(WasAnimated)
  {
    for(int c = 0; c < GetSquaresUnder(); ++c)
    {
      square* Square = GetSquareUnder(c);

      if(Square)
        Square->DecAnimatedEntities();
    }
  }
  else
  {
    for(int c = 0; c < GetSquaresUnder(); ++c)
    {
      square* Square = GetSquareUnder(c);

      if(Square)
        Square->IncAnimatedEntities();
    }
  }
}

truth bodypart::MaterialIsChangeable(ccharacter*) const
{
  return !Master || !Master->BodyPartIsVital(GetBodyPartIndex()) || UseMaterialAttributes();
}

truth bodypart::AddAdjective(festring& String, truth Articled) const
{
  if(!Master)
  {
    if(Articled)
      String << "a ";

    String << "severed ";
    return true;
  }
  else
    return false;
}

void bodypart::RemoveRust()
{
  item::RemoveRust();
  RestoreHP();
}

void bodypart::RemoveBurns()
{
  item::RemoveBurns();
  RestoreHP();
}

long bodypart::GetFixPrice() const
{
  return GetMaxHP() - GetHP() + GetMainMaterial()->GetRustLevel() * 25 + GetMainMaterial()->GetBurnLevel() * 25;
}

truth bodypart::IsFixableBySmith(ccharacter*) const
{
  return (GetMainMaterial()->GetCategoryFlags() & IS_METAL
          && (GetHP() < GetMaxHP() || IsRusted()));
}

truth bodypart::IsFixableByTailor(ccharacter*) const
{
  return (GetMainMaterial()->GetCategoryFlags() & CAN_BE_TAILORED
          && GetHP() < GetMaxHP());
}

item* bodypart::Fix()
{
  RestoreHP();
  return this;
}

void bodypart::SignalMaterialChange()
{
  if(Master)
    RestoreHP();
}

truth bodypart::ShowMaterial() const
{
  return MainMaterial->GetConfig() != NormalMaterial;
}

col16 lobhsetorso::GetMaterialColorD(int Frame) const
{
  Frame &= 31;
  int Modifier = Frame * (31 - Frame);
  return MakeRGB16(220 - (Modifier >> 2), 220 - (Modifier >> 1), 0);
}

truth bodypart::IsDestroyable(ccharacter*) const
{
  return !Master || !Master->BodyPartIsVital(GetBodyPartIndex());
}

truth bodypart::DamageTypeCanScar(int Type)
{
  return !(Type == POISON || Type == DRAIN || Type == PSI);
}

void bodypart::GenerateScar(int Damage, int Type)
{
  Scar.push_back(scar());
  scar& NewScar = Scar.back();
  NewScar.Severity = 1 + RAND_N(1 + 5 * Damage / GetMaxHP());

  if(GetMaster()->IsPlayer())
  {
    int ScarColor = MakeShadeColor(GetMainMaterial()->GetColor());
    NewScar.PanelBitmap = igraph::GenerateScarBitmap(GetBodyPartIndex(),
                                                     NewScar.Severity,
                                                     ScarColor);
    ADD_MESSAGE("Your %s is scarred.", CHAR_NAME(UNARTICLED));
  }
  else
    NewScar.PanelBitmap = 0;

  CalculateMaxHP();
  GetMaster()->CalculateMaxHP();
  GetMaster()->CalculateAttributeBonuses();
  CalculateAttackInfo();
}

void bodypart::DrawScars(cblitdata& B) const
{
  for(size_t c = 0; c < Scar.size(); ++c)
  {
    if(!Scar[c].PanelBitmap)
    {
      int ScarColor = MakeShadeColor(GetMainMaterial()->GetColor());
      Scar[c].PanelBitmap = igraph::GenerateScarBitmap(GetBodyPartIndex(),
                                                       Scar[c].Severity,
                                                       ScarColor);
    }

    Scar[c].PanelBitmap->NormalMaskedBlit(B);
  }
}

outputfile& operator<<(outputfile& SaveFile, const scar& Scar)
{
  SaveFile << Scar.Severity << Scar.PanelBitmap;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, scar& Scar)
{
  SaveFile >> Scar.Severity >> Scar.PanelBitmap;
  return SaveFile;
}

int bodypart::CalculateScarAttributePenalty(int Attribute) const
{
  double DoubleAttribute = Attribute;

  for(size_t c = 0; c < Scar.size(); ++c)
    DoubleAttribute *= (100. - Scar[c].Severity * 4) / 100;

  return Min(Attribute - int(DoubleAttribute), Attribute - 1);
}

int bodypart::CalculateBurnAttributePenalty(int Attribute) const
{
  int BurnLevel = 0;
  double DoubleAttribute = Attribute;

  if(MainMaterial)
  {
    BurnLevel = MainMaterial->GetBurnLevel();
    DoubleAttribute *= (1. - BurnLevel * 0.05);
  }

  return BurnLevel ? Min(Attribute - int(DoubleAttribute), Attribute - 1) : 0;
}

bodypart::~bodypart()
{
  for(size_t c = 0; c < Scar.size(); ++c)
    delete Scar[c].PanelBitmap;
}

bodypart::bodypart(const bodypart& B)
: mybase(B), OwnerDescription(B.OwnerDescription), Master(B.Master),
  CarriedWeight(B.CarriedWeight), BodyPartVolume(B.BodyPartVolume),
  BitmapPos(B.BitmapPos), ColorB(B.ColorB), ColorC(B.ColorC), ColorD(B.ColorD),
  SpecialFlags(B.SpecialFlags), HP(B.HP), MaxHP(B.MaxHP),
  BloodMaterial(B.BloodMaterial), NormalMaterial(B.NormalMaterial),
  SpillBloodCounter(B.SpillBloodCounter), WobbleData(B.WobbleData), Scar(B.Scar)
{
  for(size_t c = 0; c < Scar.size(); ++c)
    if(Scar[c].PanelBitmap)
      Scar[c].PanelBitmap = new bitmap(Scar[c].PanelBitmap);
}

/* Amount should be > 0 */

void bodypart::RemoveDamageIDs(int Amount)
{
  /*while(Amount)
  {
    damageid& D = DamageID.front();
    int CurrentAmount = D.Amount;

    if(Amount < CurrentAmount)
    {
      D.Amount -= Amount;
      Amount = 0;
    }
    else
    {
      DamageID.pop_front();
      Amount -= CurrentAmount;
    }
  }*/
}

/* Amount should be > 0 */

void bodypart::AddDamageID(int SrcID, int Amount)
{
  /*damageid D = { SrcID, Amount };
  DamageID.push_back(D);*/
}

int arm::GetCurrentSWeaponSkillBonus() const
{
  return (*GetCurrentSWeaponSkill()
      ? (*GetCurrentSWeaponSkill())->GetBonus() : 1);
}

v2 battorso::GetBitmapPos(int Frame) const
{
  v2 BasePos = torso::GetBitmapPos(Frame);
  Frame &= 0xF;
  return v2(BasePos.X + ((Frame &~ 3) << 2), BasePos.Y);
}

v2 spidertorso::GetBitmapPos(int Frame) const
{
  v2 BasePos = torso::GetBitmapPos(Frame);
  Frame &= 0xF;
  return v2(BasePos.X + ((Frame &~ 7) << 1), BasePos.Y);
}

truth arm::HasSadistWeapon() const
{
  item* Wielded = GetWielded();
  return Wielded && Wielded->IsSadistWeapon();
}

truth corpse::AddStateDescription(festring& Name, truth Articled) const
{
  if(!Spoils())
    return false;

  truth Hasted = true, Slowed = true;

  for(int c = 0; c < GetDeceased()->GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetDeceased()->GetBodyPart(c);

    if(BodyPart)
    {
      if(!(BodyPart->ItemFlags & HASTE))
        Hasted = false;

      if(!(BodyPart->ItemFlags & SLOW))
        Slowed = false;
    }
  }

  if((Hasted | Slowed) && Articled)
    Name << "a ";

  if(Hasted)
    Name << "hasted ";

  if(Slowed)
    Name << "slowed ";

  return true;
}
