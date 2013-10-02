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

void meleeweapon::SetSecondaryMaterial(material* What, int SpecialFlags) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void meleeweapon::ChangeSecondaryMaterial(material* What, int SpecialFlags) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void meleeweapon::InitMaterials(material* M1, material* M2, truth CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), CUP); }
double meleeweapon::GetTHVBonus() const { return Enchantment * .5; }
double meleeweapon::GetDamageBonus() const { return Enchantment; }
col16 meleeweapon::GetDripColor() const { return Fluid[0]->GetLiquid()->GetColor(); }
truth meleeweapon::IsDippable(ccharacter*) const { return !Fluid; }
truth meleeweapon::AllowRegularColors() const { return SecondaryMaterial->GetVolume(); }
v2 meleeweapon::GetWieldedBitmapPos(int I) const { return SecondaryMaterial->GetVolume() ? item::GetWieldedBitmapPos(I) : v2(160, 128); }
void meleeweapon::InitMaterials(const materialscript* M, const materialscript* S, truth CUP) { InitMaterials(M->Instantiate(), S->Instantiate(), CUP); }

col16 justifier::GetOutlineColor(int) const { return MakeRGB16(0, 255, 0); }

col16 neercseulb::GetOutlineColor(int) const { return MakeRGB16(255, 0, 0); }

int flamingsword::GetSpecialFlags() const { return meleeweapon::GetSpecialFlags()|ST_FLAME_1; }

col16 gorovitsweapon::GetOutlineColor(int) const { return MakeRGB16(255, 0, 0); }

int thunderhammer::GetSpecialFlags() const { return !IsBroken() ? meleeweapon::GetSpecialFlags()|ST_LIGHTNING : meleeweapon::GetSpecialFlags(); }

int armor::GetCarryingBonus() const { return Enchantment << 1; }
double armor::GetTHVBonus() const { return Enchantment * .5; }
double armor::GetDamageBonus() const { return Enchantment; }

long bodyarmor::GetPrice() const { return (armor::GetPrice() << 3) + GetEnchantedPrice(Enchantment); }
truth bodyarmor::IsInCorrectSlot(int I) const { return I == BODY_ARMOR_INDEX; }
cfestring& bodyarmor::GetNameSingular() const { return GetMainMaterial()->GetFlexibility() >= 5 ? item::GetFlexibleNameSingular() : item::GetNameSingular(); }
cchar* bodyarmor::GetBreakVerb() const { return GetMainMaterial()->GetFlexibility() >= 5 ? "is torn apart" : "breaks"; }

col16 goldeneagleshirt::GetOutlineColor(int) const { return MakeRGB16(0, 255, 255); }

long cloak::GetPrice() const { return armor::GetPrice() * 10 + GetEnchantedPrice(Enchantment); }
truth cloak::IsInCorrectSlot(int I) const { return I == CLOAK_INDEX; }
col16 cloak::GetMaterialColorB(int) const { return MakeRGB16(111, 64, 37); }
cchar* cloak::GetBreakVerb() const { return GetMainMaterial()->GetFlexibility() >= 5 ? "is torn apart" : "breaks"; }
truth cloak::ReceiveDamage(character* Damager, int Damage,  int Type, int Dir) { return armor::ReceiveDamage(Damager, Damage >> 1, Type, Dir); }
int cloak::GetSpecialFlags() const { return ST_CLOAK; }

long boot::GetPrice() const { return armor::GetPrice() / 5 + GetEnchantedPrice(Enchantment); }
truth boot::IsInCorrectSlot(int I) const { return I == RIGHT_BOOT_INDEX || I == LEFT_BOOT_INDEX; }

long gauntlet::GetPrice() const { return armor::GetPrice() / 3 + GetEnchantedPrice(Enchantment); }
truth gauntlet::IsInCorrectSlot(int I) const { return I == RIGHT_GAUNTLET_INDEX || I == LEFT_GAUNTLET_INDEX; }

long belt::GetPrice() const { return armor::GetPrice() * 5 + GetEnchantedPrice(Enchantment); }
truth belt::IsInCorrectSlot(int I) const { return I == BELT_INDEX; }

truth ring::IsInCorrectSlot(int I) const { return I == RIGHT_RING_INDEX || I == LEFT_RING_INDEX; }
col16 ring::GetMaterialColorB(int) const { return MakeRGB16(200, 200, 200); }

truth amulet::IsInCorrectSlot(int I) const { return I == AMULET_INDEX; }
col16 amulet::GetMaterialColorB(int) const { return MakeRGB16(111, 64, 37); }

truth helmet::IsGorovitsFamilyRelic() const { return GetConfig() == GOROVITS_FAMILY_GAS_MASK; }
long helmet::GetPrice() const { return armor::GetPrice() + GetEnchantedPrice(Enchantment); }
truth helmet::IsInCorrectSlot(int I) const { return I == HELMET_INDEX; }
col16 helmet::GetMaterialColorB(int) const { return GetConfig() != GOROVITS_FAMILY_GAS_MASK ? (GetConfig() & ~BROKEN) ? MakeRGB16(140, 70, 70) : MakeRGB16(111, 64, 37) : MakeRGB16(0, 40, 0); }
col16 helmet::GetMaterialColorC(int) const { return MakeRGB16(180, 200, 180); }

int wondersmellstaff::GetClassAnimationFrames() const { return !IsBroken() ? 128 : 1; }

truth meleeweapon::HitEffect(character* Enemy, character*, v2, int BodyPartIndex, int, truth BlockedByArmour)
{
  if(!BlockedByArmour && Fluid)
  {
    truth Success = false;
    fluidvector FluidVector;
    FillFluidVector(FluidVector);

    for(uint c = 0; c < FluidVector.size(); ++c)
      if(FluidVector[c]->Exists()
	 && FluidVector[c]->GetLiquid()->HitEffect(Enemy, Enemy->GetBodyPart(BodyPartIndex)))
	Success = true;

    return Success;
  }
  else
    return false;
}

void meleeweapon::DipInto(liquid* Liquid, character* Dipper)
{
  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now covered with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  SpillFluid(Dipper, Liquid);
  Dipper->DexterityAction(10);
}

truth pickaxe::Apply(character* User)
{
  if(IsBroken())
  {
    ADD_MESSAGE("%s is totally broken.",CHAR_NAME(DEFINITE));
    return false;
  }

  int Dir = game::DirectionQuestion(CONST_S("What direction do you want to dig? [press a direction key]"), false);

  v2 Temp = game::GetMoveVector(Dir);
  v2 ToBeDug = User->GetPos() + Temp;
  if(Dir == DIR_ERROR || !GetArea()->IsValidPos(ToBeDug))
    return false;

  lsquare* Square = GetNearLSquare(ToBeDug);
  olterrain* Terrain = Square->GetOLTerrain();

  if(!Terrain)
  {
    ADD_MESSAGE("Nothing to dig there!");
    return false;
  }

  if(Square->CanBeDug())
  {
    if(Terrain->CanBeDestroyed())
      if(Terrain->GetMainMaterial()->CanBeDug(GetMainMaterial()))
      {
	int RoomNumber = Square->GetRoomIndex();

	if(!RoomNumber || Square->GetLevel()->GetRoom(RoomNumber)->CheckDestroyTerrain(User))
	{
	  User->SwitchToDig(this, ToBeDug);
	  User->DexterityAction(5);
	  return true;
	}
	else
	  return false;
      }
      else
	ADD_MESSAGE("%s is too hard to dig with %s.", Square->GetOLTerrain()->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
    else
      ADD_MESSAGE(Terrain->GetDigMessage().CStr());
  }

  return false;
}

long meleeweapon::GetPrice() const
{
  double WeaponStrengthModifier = GetFormModifier() * GetMainMaterial()->GetStrengthValue();
  WeaponStrengthModifier *= WeaponStrengthModifier;
  WeaponStrengthModifier *= GetMainMaterial()->GetWeight();
  WeaponStrengthModifier *= Max((10 + Enchantment) * 0.1, 0.1);
  return long(WeaponStrengthModifier / (20000000.0 * sqrt(GetWeight())))
    + GetEnchantedPrice(Enchantment);
}

int whip::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

truth pickaxe::IsAppliable(ccharacter* Who) const
{
  return Who->CanWield();
}

void meleeweapon::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Enchantment;
  SaveFile << SecondaryMaterial;
}

void meleeweapon::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Enchantment;
  LoadMaterial(SaveFile, SecondaryMaterial);
}

material* meleeweapon::GetMaterial(int I) const
{
  return !I ? MainMaterial : SecondaryMaterial;
}

col16 meleeweapon::GetMaterialColorB(int) const
{
  return SecondaryMaterial->GetVolume() ? SecondaryMaterial->GetColor() : TRANSPARENT_COLOR;
}

alpha meleeweapon::GetAlphaB(int) const
{
  return SecondaryMaterial->GetAlpha();
}

truth flamingsword::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && RAND() & 1)
  {
    if(Hitter)
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s sword burns %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));
    }
    else
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("The sword burns %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    }

    return Enemy->ReceiveBodyPartDamage(Hitter, 3 + (RAND() & 3), FIRE, BodyPartIndex, Direction) || BaseSuccess;
  }
  else
    return BaseSuccess;
}

truth mjolak::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 3))
  {
    if(Hitter)
    {
      if(Hitter->IsPlayer())
	game::DoEvilDeed(10);

      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("A burst of %s Mjolak's unholy energy fries %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));
    }
    else
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("A burst of Mjolak's unholy energy fries %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    }

    return Enemy->ReceiveBodyPartDamage(Hitter, 5 + (RAND() % 6), ENERGY, BodyPartIndex, Direction) || BaseSuccess;
  }
  else
    return BaseSuccess;
}

truth vermis::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
  {
    if(Hitter)
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("%s Vermis sends %s on a sudden journey.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));
    }
    else
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("Vermis sends %s on a sudden journey.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    }

    Enemy->TeleportRandomly();
    return true;
  }
  else
    return BaseSuccess;
}

truth turox::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
  {
    if(Hitter)
    {
      if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
	ADD_MESSAGE("%s smash%s %s with the full force of Turox.", Hitter->CHAR_PERSONAL_PRONOUN, Hitter->IsPlayer() ? "" : "es", Enemy->CHAR_DESCRIPTION(DEFINITE));
    }
    else
    {
      if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
	ADD_MESSAGE("Turox is smashed against %s with full force.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    }

    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("A magical explosion is triggered!");

    Enemy->GetLevel()->Explosion(Hitter, CONST_S("burned @bkp Turox's explosion"), HitPos, 10 + RAND() % 100);
    return true;
  }
  else
    return BaseSuccess;
}

truth whipofthievery::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && Hitter && CleptiaHelps(Enemy, Hitter))
  {
    if(Hitter->IsPlayer())
    {
      game::DoEvilDeed(10);
      game::GetGod(CLEPTIA)->AdjustRelation(10);
    }

    if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s whip asks for the help of Cleptia as it steals %s %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_POSSESSIVE_PRONOUN, Enemy->GetMainWielded()->CHAR_NAME(UNARTICLED));

    Enemy->GetMainWielded()->MoveTo(GetLSquareUnder()->GetStack());
    return true;
  }
  else
    return BaseSuccess;
}

meleeweapon::~meleeweapon()
{
  delete SecondaryMaterial;
}

meleeweapon::meleeweapon(const meleeweapon& MW) : mybase(MW), Enchantment(MW.Enchantment)
{
  CopyMaterial(MW.SecondaryMaterial, SecondaryMaterial);
}

truth whipofthievery::CleptiaHelps(ccharacter* Enemy, ccharacter* Hitter) const
{
  if(Enemy->IsImmuneToWhipOfThievery() || !Enemy->GetMainWielded() || GetMainMaterial()->GetFlexibility() <= 5)
    return false;

  if(Hitter->IsPlayer())
  {
    if(game::GetGod(CLEPTIA)->GetRelation() < 0)
      return false;
    else
      return !RAND_N(10 - game::GetGod(CLEPTIA)->GetRelation() / 200);
  }
  else
    return !RAND_N(10);
}

void meleeweapon::AddInventoryEntry(ccharacter* Viewer, festring& Entry, int, truth ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
  {
    Entry << " [" << GetWeight() << "g, DAM " << GetBaseMinDamage() << '-' << GetBaseMaxDamage();
    Entry << ", " << GetBaseToHitValueDescription();

    if(!IsBroken() && !IsWhip())
      Entry << ", " << GetStrengthValueDescription();

    int CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
    int SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

    if(CWeaponSkillLevel || SWeaponSkillLevel)
      Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

    Entry << ']';
  }
}

void meleeweapon::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == MainMaterial)
  {
    if(CanBeSeenByPlayer())
      if(SecondaryMaterial->GetVolume())
	ADD_MESSAGE("The edge of %s spoils.", GetExtendedDescription().CStr());
      else
	ADD_MESSAGE("%s spoils.", GetExtendedDescription().CStr());

    RemoveMainMaterial();
  }
  else
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("The handle of %s spoils", GetExtendedDescription().CStr());

    delete RemoveSecondaryMaterial();
  }
}

void meleeweapon::AddPostFix(festring& String, int Case) const
{
  item::AddPostFix(String, Case);

  if(Fluid)
  {
    String << " covered with ";
    fluid::AddFluidInfo(Fluid[0], String);
  }

  if(Enchantment > 0)
    String << " +" << Enchantment;
  else if(Enchantment < 0)
    String << ' ' << Enchantment;
}

void meleeweapon::Be()
{
  item::Be();

  if(Exists() && SecondaryMaterial->GetVolume())
    SecondaryMaterial->Be(ItemFlags);
}

long whipofthievery::GetPrice() const
{
  /* If intact but not flexible enough to work, special thievery bonus must be removed */

  return GetMainMaterial()->GetFlexibility() > 5 || IsBroken() ? whip::GetPrice() : whip::GetPrice() - item::GetPrice();
}

int meleeweapon::GetSparkleFlags() const
{
  return (MainMaterial->IsSparkling() ? SPARKLING_A|(SecondaryMaterial->GetVolume() ? SPARKLING_C : 0) : 0)
    | (SecondaryMaterial->IsSparkling() ? SPARKLING_B : 0);
}

void meleeweapon::SetEnchantment(int Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void meleeweapon::EditEnchantment(int Amount)
{
  Enchantment += Amount;
  SignalEnchantmentChange();
}

int meleeweapon::GetStrengthValue() const
{
  return Max<int>(long(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000 + Enchantment, 0);
}

void meleeweapon::PostConstruct()
{
  Enchantment = GetBaseEnchantment();
}

int meleeweapon::GetSpoilLevel() const
{
  int MainSpoilLevel = MainMaterial->GetSpoilLevel();

  if(SecondaryMaterial->GetVolume())
    return Max(MainSpoilLevel, SecondaryMaterial->GetSpoilLevel());
  else
    return MainSpoilLevel;
}

truth neercseulb::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
  {
    if(Hitter->IsPlayer())
      game::DoEvilDeed(10);

    if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s Neerc Se-ulb's life-draining energies swallow %s!", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

    return Enemy->ReceiveBodyPartDamage(Hitter, 10 + (RAND() % 11), DRAIN, BodyPartIndex, Direction) || BaseSuccess;
  }
  else
    return BaseSuccess;
}

truth thunderhammer::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
  {
    if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s hammer shoots a lightning bolt at %s!", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

    beamdata Beam
      (
	Hitter,
	CONST_S("electrocuted @bkp thunder hammer"),
	Hitter->GetPos(),
	WHITE,
	BEAM_LIGHTNING,
	Direction,
	4,
	0
      );

    GetLevel()->LightningBeam(Beam);
    return true;
  }
  else
    return BaseSuccess;
}

truth thunderhammer::ReceiveDamage(character* Damager, int Damage, int Type, int Dir)
{
  return Type & ELECTRICITY ? false : meleeweapon::ReceiveDamage(Damager, Damage, Type, Dir);
}

long armor::GetPrice() const
{
  double StrengthValue = GetStrengthValue();
  return long(StrengthValue * StrengthValue * StrengthValue * 20 / sqrt(GetWeight()));
}

int belt::GetFormModifier() const
{
  return item::GetFormModifier() * GetMainMaterial()->GetFlexibility();
}

void armor::AddInventoryEntry(ccharacter*, festring& Entry, int Amount, truth ShowSpecialInfo) const
{
  if(Amount == 1)
    AddName(Entry, INDEFINITE);
  else
  {
    Entry << Amount << ' ';
    AddName(Entry, PLURAL);
  }

  if(ShowSpecialInfo)
    Entry << " [" << GetWeight() * Amount << "g, AV " << GetStrengthValue() << ']';
}

void shield::AddInventoryEntry(ccharacter* Viewer, festring& Entry, int, truth ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
  {
    Entry << " [" << GetWeight() << "g, "  << GetBaseBlockValueDescription();

    if(!IsBroken())
      Entry << ", " << GetStrengthValueDescription();

    int CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
    int SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

    if(CWeaponSkillLevel || SWeaponSkillLevel)
      Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

    Entry << ']';
  }
}

truth armor::CanBePiledWith(citem* Item, ccharacter* Viewer) const
{
  return item::CanBePiledWith(Item, Viewer) && Enchantment == static_cast<const armor*>(Item)->Enchantment;
}

long shield::GetPrice() const /* temporary... */
{
  double StrengthValue = GetStrengthValue();
  return long(sqrt(GetBaseBlockValue()) * StrengthValue * StrengthValue) + item::GetPrice();
}

void armor::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << Enchantment;
}

void armor::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> Enchantment;
}

void armor::AddPostFix(festring& String, int Case) const
{
  item::AddPostFix(String, Case);

  if(Fluid)
  {
    String << " covered with ";
    fluid::AddFluidInfo(Fluid[0], String);
  }

  if(Enchantment > 0)
    String << " +" << Enchantment;
  else if(Enchantment < 0)
    String << ' ' << Enchantment;
}

void armor::SetEnchantment(int Amount)
{
  Enchantment = Amount;
  SignalEnchantmentChange();
}

void armor::EditEnchantment(int Amount)
{
  Enchantment += Amount;
  SignalEnchantmentChange();
}

int armor::GetStrengthValue() const
{
  return Max<long>(long(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000 + Enchantment, 0);
}

void armor::PostConstruct()
{
  Enchantment = GetBaseEnchantment();
}

int armor::GetInElasticityPenalty(int Attribute) const
{
  return Attribute * GetInElasticityPenaltyModifier() / (GetMainMaterial()->GetFlexibility() * 100);
}

void meleeweapon::GenerateMaterials()
{
  int Chosen = RandomizeMaterialConfiguration();
  const fearray<long>& MMC = GetMainMaterialConfig();
  InitMaterial(MainMaterial,
	       MAKE_MATERIAL(MMC.Data[MMC.Size == 1 ? 0 : Chosen]),
	       GetDefaultMainVolume());
  const fearray<long>& SMC = GetSecondaryMaterialConfig();
  InitMaterial(SecondaryMaterial,
	       MAKE_MATERIAL(SMC.Data[SMC.Size == 1 ? 0 : Chosen]),
	       GetDefaultSecondaryVolume());
}

truth chameleonwhip::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && ScabiesHelps(Enemy, Hitter))
  {
    if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s whip asks for the help of Scabies as it polymorphs %s.", Hitter->CHAR_PERSONAL_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

    if(Hitter->IsPlayer())
    {
      game::DoEvilDeed(20);
      game::GetGod(SCABIES)->AdjustRelation(10);
    }

    int CurrentDanger = int(Enemy->GetRelativeDanger(PLAYER) * 1000);
    Enemy->PolymorphRandomly(CurrentDanger / 4, Min(CurrentDanger, 999999), 100 + RAND() % 400);
    return true;
  }
  else
    return BaseSuccess;
}

truth chameleonwhip::ScabiesHelps(ccharacter* Enemy, ccharacter* Hitter) const
{
  if(!Enemy->IsPolymorphable())
    return false;

  if(Hitter->IsPlayer())
  {
    if(game::GetGod(SCABIES)->GetRelation() < 0)
      return false;
    else
      return !(RAND() % (20 - game::GetGod(SCABIES)->GetRelation() / 150));
  }
  else
    return !(RAND() % 20);
}

alpha justifier::GetOutlineAlpha(int Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

alpha neercseulb::GetOutlineAlpha(int Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

alpha gorovitsweapon::GetOutlineAlpha(int Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

alpha goldeneagleshirt::GetOutlineAlpha(int Frame) const
{
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

alpha wondersmellstaff::GetOutlineAlpha(int Frame) const
{
  if(!IsBroken())
  {
    Frame &= 31;
    return Frame * (31 - Frame) >> 1;
  }
  else
    return 255;
}

col16 wondersmellstaff::GetOutlineColor(int Frame) const
{
  if(!IsBroken())
    switch((Frame&127) >> 5)
    {
     case 0: return BLUE;
     case 1: return GREEN;
     case 2: return RED;
     case 3: return YELLOW;
    }

  return TRANSPARENT_COLOR;
}

truth wondersmellstaff::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(!IsBroken() && Enemy->IsEnabled() && !(RAND() % 5))
  {
    if(RAND() & 3)
    {
      truth Seen = false;
      int Amount = 250 / Enemy->GetSquaresUnder();

      for(int c = 0; c < Enemy->GetSquaresUnder(); ++c)
      {
	lsquare* Square = Enemy->GetLSquareUnder(c);

	if(Square->IsFlyable())
	{
	  Square->AddSmoke(gas::Spawn(EVIL_WONDER_STAFF_VAPOUR, Amount));

	  if(!Seen && Square->CanBeSeenByPlayer())
	    Seen = true;
	}
      }

      if(Seen)
	ADD_MESSAGE("Strange red smoke billows out of %s staff.", Hitter->CHAR_POSSESSIVE_PRONOUN);
    }
    else
    {
      /* Can a multitiled creature ever be the hitter? */

      lsquare* Square = Hitter->GetLSquareUnder();

      if(Square->IsFlyable())
      {
	if(Square->CanBeSeenByPlayer())
	  ADD_MESSAGE("Strange blue smoke billows out of %s staff.", Hitter->CHAR_POSSESSIVE_PRONOUN);

	Square->AddSmoke(gas::Spawn(GOOD_WONDER_STAFF_VAPOUR, 100));
      }
    }

    return true;
  }
  else
    return BaseSuccess;
}

truth bodyarmor::AddAdjective(festring& String, truth Articled) const
{
  if(IsBroken())
  {
    if(Articled)
      String << "a ";

    if(GetMainMaterial()->GetFlexibility() >= 5)
      String << "torn";
    else
      String << "broken";

    String << ' ';
    return true;
  }
  else
    return false;
}

truth cloak::AddAdjective(festring& String, truth Articled) const
{
  if(IsBroken())
  {
    if(Articled)
      String << "a ";

    if(GetMainMaterial()->GetFlexibility() >= 5)
      String << "torn";
    else
      String << "broken";

    String << ' ';
    return true;
  }
  else
    return false;
}

int meleeweapon::GetRustDataB() const
{
  return SecondaryMaterial->GetRustData();
}

void meleeweapon::TryToRust(long LiquidModifier)
{
  item::TryToRust(LiquidModifier);

  if(SecondaryMaterial->GetVolume() && SecondaryMaterial->TryToRust(LiquidModifier))
    SecondaryMaterial->SetRustLevel(SecondaryMaterial->GetRustLevel() + 1);
}

material* meleeweapon::GetConsumeMaterial(ccharacter* Consumer, materialpredicate Predicate) const
{
  if((SecondaryMaterial->*Predicate)()
     && SecondaryMaterial->GetVolume()
     && Consumer->CanConsume(SecondaryMaterial))
    return SecondaryMaterial;
  else
    return item::GetConsumeMaterial(Consumer, Predicate);
}

material* meleeweapon::RemoveMaterial(material* Material)
{
  if(Material == MainMaterial)
    return RemoveMainMaterial();
  else
    return RemoveSecondaryMaterial();
}

material* meleeweapon::RemoveMainMaterial()
{
  truth Equipped = PLAYER->Equips(this);

  if(SecondaryMaterial->GetVolume())
  {
    item* Lump = SecondaryMaterial->CreateNaturalForm(SecondaryMaterial->GetVolume());
    DonateFluidsTo(Lump);
    DonateIDTo(Lump);
    DonateSlotTo(Lump);
  }
  else
    RemoveFromSlot();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));

  SendToHell();
  return 0;
}

material* meleeweapon::RemoveSecondaryMaterial()
{
  SecondaryMaterial->SetVolume(0);

  if(!IsBroken())
    Break(0);
  else
  {
    RedistributeFluids();
    UpdatePictures();
    SendNewDrawAndMemorizedUpdateRequest();
  }

  return 0;
}

pixelpredicate meleeweapon::GetFluidPixelAllowedPredicate() const
{
  if(SecondaryMaterial->GetVolume())
    return &rawbitmap::IsTransparent;
  else
    return &rawbitmap::IsMaterialColor1;
}

void meleeweapon::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  if(MainMaterial)
    game::CombineLights(Emitation, MainMaterial->GetEmitation());

  if(SecondaryMaterial->GetVolume())
    game::CombineLights(Emitation, SecondaryMaterial->GetEmitation());
}

truth meleeweapon::CalculateHasBe() const
{
  return LifeExpectancy
    || (MainMaterial && MainMaterial->HasBe())
    || (SecondaryMaterial
	&& SecondaryMaterial->GetVolume()
	&& SecondaryMaterial->HasBe());
}

void decosadshirt::Be()
{
  if(PLAYER->Equips(this))
    ++EquippedTicks;

  bodyarmor::Be();
}

decosadshirt::decosadshirt() : EquippedTicks(0)
{
  Enable();
}

void decosadshirt::Save(outputfile& SaveFile) const
{
  bodyarmor::Save(SaveFile);
  SaveFile << EquippedTicks;
}

void decosadshirt::Load(inputfile& SaveFile)
{
  bodyarmor::Load(SaveFile);
  SaveFile >> EquippedTicks;
  Enable();
}

item* meleeweapon::Fix()
{
  SecondaryMaterial->SetVolumeNoSignals(GetDefaultSecondaryVolume());
  return item::Fix();
}

long meleeweapon::GetMaterialPrice() const
{
  return MainMaterial->GetRawPrice() + SecondaryMaterial->GetRawPrice();
}

void meleeweapon::CalculateEnchantment()
{
  Enchantment -= femath::LoopRoll(game::GetCurrentLevel()->GetEnchantmentMinusChance(), 5);
  Enchantment += femath::LoopRoll(game::GetCurrentLevel()->GetEnchantmentPlusChance(), 5);
  Enchantment -= femath::LoopRoll(GetEnchantmentMinusChance(), 5);
  Enchantment += femath::LoopRoll(GetEnchantmentPlusChance(), 5);
}

void armor::CalculateEnchantment()
{
  Enchantment -= femath::LoopRoll(game::GetCurrentLevel()->GetEnchantmentMinusChance(), 5);
  Enchantment += femath::LoopRoll(game::GetCurrentLevel()->GetEnchantmentPlusChance(), 5);
  Enchantment -= femath::LoopRoll(GetEnchantmentMinusChance(), 5);
  Enchantment += femath::LoopRoll(GetEnchantmentPlusChance(), 5);
}

col16 meleeweapon::GetMaterialColorC(int Frame) const
{
  return SecondaryMaterial->GetVolume() ? GetMaterialColorA(Frame) : TRANSPARENT_COLOR;
}

void daggerofvenom::Be()
{
  meleeweapon::Be();

  if(Exists() && !IsBroken() && (*Slot)->IsGearSlot()  && !RAND_N(10))
  {
    fluidvector FluidVector;
    FillFluidVector(FluidVector);
    uint Volume = 0;

    for(uint c = 0; c < FluidVector.size(); ++c)
    {
      liquid* L = FluidVector[c]->GetLiquid();
      Volume += L->GetVolume();      //I imagine that there is a function I don't know to do this...
    }

    if(Volume < 90)
      SpillFluid(0, liquid::Spawn(POISON_LIQUID, 10));
  }
}

truth weepblade::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = meleeweapon::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && !(RAND_N(3)))
  {
    if(Enemy->IsPlayer() || Hitter->IsPlayer() 
       || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s weeping blade spills acid on %s.", 
		  Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));
    Enemy->SpillFluid(PLAYER, liquid::Spawn(SULPHURIC_ACID, 25+RAND()%25));
    return BaseSuccess;
  }
  else
    return BaseSuccess;
}

void acidshield::BlockEffect(character* Blocker, character* Attacker, item* Weapon, int Type)
{
   int CheckAttackType = 0;
   if(!IsBroken())
   {
      if(!RAND_N(400))
      {
	if(Weapon)
	{
	  Weapon->SpillFluid(Blocker, liquid::Spawn(SULPHURIC_ACID, 200 + RAND() % 51));
	  ADD_MESSAGE("%s is completely doused in sulpheric acid!", Attacker->CHAR_DESCRIPTION(DEFINITE));
	  return;
	}
      }

      if(RAND_2 && Weapon)
      {
	Weapon->SpillFluid(Blocker, liquid::Spawn(SULPHURIC_ACID, 20 + RAND() % 41));
	ADD_MESSAGE("%s weapon is splashed with acid from the shield!", Attacker->CHAR_POSSESSIVE_PRONOUN);
      }

      if(!RAND_N(5))
      {
	Attacker->SpillFluid(Blocker, liquid::Spawn(SULPHURIC_ACID, 5 + RAND() % 11));
	ADD_MESSAGE("%s is splashed with acid!", Attacker->CHAR_DESCRIPTION(DEFINITE));
	return;
      }

      if(RAND_2)
      {
	Attacker->SpillFluid(Blocker, liquid::Spawn(SULPHURIC_ACID, 25 + RAND() % 26));
	ADD_MESSAGE("%s is splashed with acid from the shield!", Attacker->CHAR_DESCRIPTION(DEFINITE));
      }
   }
} 


void wondersmellstaff::Break(character* Who, int Much)
{
    material* GasMaterial = GetSecondaryMaterial();
    GetLevel()->GasExplosion(gas::Spawn(GOOD_WONDER_STAFF_VAPOUR, 100), GetLSquareUnder());

    if(CanBeSeenByPlayer())
    {
      ADD_MESSAGE("%s unleashes a puff of a wonderous gas.", CHAR_NAME(DEFINITE));

    }
    meleeweapon::Break(Who,Much);
}
