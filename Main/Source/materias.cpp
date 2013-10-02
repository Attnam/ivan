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

/* Compiled through materset.cpp */

void organic::ResetSpoiling() { SpoilCounter = SpoilLevel = 0; }

cchar* liquid::GetConsumeVerb() const { return "drinking"; }

truth powder::IsExplosive() const { return !Wetness && material::IsExplosive(); }

truth ironalloy::IsSparkling() const { return material::IsSparkling() && GetRustLevel() == NOT_RUSTED; }

void organic::Be(ulong Flags)
{
  if(SpoilCheckCounter++ >= 50)
  {
    if(MotherEntity->AllowSpoil())
    {
      if(Flags & HASTE)
	SpoilCounter += 125;
      else if(Flags & SLOW)
	SpoilCounter += 5;
      else
	SpoilCounter += 25;

      if(SpoilCounter < GetSpoilModifier())
      {
	if(SpoilCounter << 1 >= GetSpoilModifier())
	{
	  int NewSpoilLevel = ((SpoilCounter << 4) / GetSpoilModifier()) - 7;

	  if(NewSpoilLevel != SpoilLevel)
	  {
	    SpoilLevel = NewSpoilLevel;
	    MotherEntity->SignalSpoilLevelChange(this);
	  }
	}
      }
      else
      {
	SpoilLevel = 8;
	MotherEntity->SignalSpoil(this);
      }
    }

    SpoilCheckCounter = 0;
  }
}

void organic::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << SpoilCounter << SpoilCheckCounter << SpoilLevel;
}

void organic::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> SpoilCounter >> SpoilCheckCounter >> SpoilLevel;
}

void organic::PostConstruct()
{
  SpoilLevel = SpoilCheckCounter = 0;
  SpoilCounter = (RAND() % GetSpoilModifier()) >> 5;
}

void flesh::PostConstruct()
{
  organic::PostConstruct();
  SkinColorSparkling = InfectedByLeprosy = false;
  SkinColor = GetColor();
}

void flesh::Save(outputfile& SaveFile) const
{
  organic::Save(SaveFile);
  SaveFile << SkinColor << SkinColorSparkling << InfectedByLeprosy;
}

void flesh::Load(inputfile& SaveFile)
{
  organic::Load(SaveFile);
  SaveFile >> SkinColor >> SkinColorSparkling >> InfectedByLeprosy;
}

void powder::Be(ulong)
{
  if(Wetness > 0)
    --Wetness;
}

void powder::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << Wetness;
}

void powder::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> Wetness;
}

material* organic::EatEffect(character* Eater, long Amount)
{
  Amount = Volume > Amount ? Amount : Volume;
  GetMotherEntity()->SpecialEatEffect(Eater, Amount);
  Effect(Eater, TORSO_INDEX, Amount);
  Eater->ReceiveNutrition(GetNutritionValue() * Amount * 20 / (1000 * (GetSpoilLevel() + 1)));

  if(IsInfectedByLeprosy() && Amount && !RAND_N(25000 / Amount))
    Eater->GainIntrinsic(LEPROSY);

  if(GetSpoilLevel() > 0)
  {
    Eater->BeginTemporaryState(CONFUSED, int(Amount * GetSpoilLevel() * sqrt(GetNutritionValue()) / 1000));

    if(GetBodyFlags() & CAN_HAVE_PARASITE
       && !(RAND() % (250 / GetSpoilLevel())))
      Eater->GainIntrinsic(PARASITIZED);
  }

  if(GetSpoilLevel() > 4)
    Eater->BeginTemporaryState(POISONED, int(Amount * (GetSpoilLevel() - 4) * sqrt(GetNutritionValue()) / 1000));

  if(Volume != Amount)
  {
    EditVolume(-Amount);
    return 0;
  }
  else
    return MotherEntity->RemoveMaterial(this);
}

void organic::AddConsumeEndMessage(character* Eater) const
{
  if(Eater->IsPlayer())
  {
    if(GetSpoilLevel() > 0 && GetSpoilLevel() <= 4)
      ADD_MESSAGE("Ugh. This stuff was slightly spoiled.");
    else if(GetSpoilLevel() > 4)
      ADD_MESSAGE("Ugh. This stuff was terribly spoiled!");
  }

  material::AddConsumeEndMessage(Eater);
}

void organic::SetSpoilCounter(int What)
{
  SpoilCounter = What;

  if(SpoilCounter < GetSpoilModifier())
  {
    if(SpoilCounter << 1 >= GetSpoilModifier())
    {
      int NewSpoilLevel = ((SpoilCounter << 4) / GetSpoilModifier()) - 7;

      if(NewSpoilLevel != SpoilLevel)
      {
	SpoilLevel = NewSpoilLevel;
	MotherEntity->SignalSpoilLevelChange(this);
      }
    }
  }
  else
    MotherEntity->SignalSpoil(this);
}

void ironalloy::SetRustLevel(int What)
{
  if(GetRustLevel() != What)
  {
    if(!RustData)
      RustData = RAND() & 0xFC | What;
    else if(!What)
      RustData = 0;
    else
      RustData = RustData & 0xFC | What;

    if(MotherEntity)
      MotherEntity->SignalRustLevelChange();
  }
}

int ironalloy::GetStrengthValue() const
{
  int Base = material::GetStrengthValue();

  switch(GetRustLevel())
  {
   case NOT_RUSTED: return Base;
   case SLIGHTLY_RUSTED: return ((Base << 3) + Base) / 10;
   case RUSTED: return ((Base << 1) + Base) >> 2;
   case VERY_RUSTED: return Base >> 1;
  }

  return 0; /* not possible */
}

truth ironalloy::AddRustLevelDescription(festring& Name, truth Articled) const
{
  if(GetRustLevel() == NOT_RUSTED)
    return false;

  if(Articled)
    Name << "a ";

  switch(GetRustLevel())
  {
   case SLIGHTLY_RUSTED:
    Name << "slightly rusted ";
    break;
   case RUSTED:
    Name << "rusted ";
    break;
   case VERY_RUSTED:
    Name << "very rusted ";
    break;
  }

  return true;
}

void ironalloy::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << RustData;
}

void ironalloy::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> RustData;
}

void liquid::TouchEffect(item* Item, cfestring& LocationName)
{
  if(GetRustModifier())
    Item->TryToRust(GetRustModifier() * GetVolume());

  if(GetAcidicity())
    Item->ReceiveAcid(this, LocationName, Volume * GetAcidicity());

  character* Char = Item->GetBodyPartMaster();

  if(Char)
    Effect(Char, Item->GetBodyPartIndex(), Volume >> 6);
}

void liquid::TouchEffect(lterrain* Terrain)
{
  if(GetRustModifier())
    Terrain->TryToRust(GetRustModifier() * GetVolume());

  if(GetAcidicity())
    Terrain->ReceiveAcid(this, Volume * GetAcidicity());
}

void liquid::TouchEffect(character* Char, int BodyPartIndex)
{
  if(GetRustModifier())
    Char->GetBodyPart(BodyPartIndex)->TryToRust(GetRustModifier() * GetVolume());

  if(Char->IsEnabled() && GetAcidicity())
    Char->GetBodyPart(BodyPartIndex)->ReceiveAcid(this, CONST_S(""), Volume * GetAcidicity() >> 1);

  if(Char->IsEnabled())
    Effect(Char, BodyPartIndex, Volume >> 9);
}

/* Doesn't do the actual rusting, just returns whether it should happen */

truth ironalloy::TryToRust(long Modifier, long Volume)
{
  if(GetRustLevel() != VERY_RUSTED)
  {
    if(!Volume)
      Volume = GetVolume();

    long Chance = long(30000000. * sqrt(Volume) / (Modifier * GetRustModifier()));

    if(Chance <= 1 || !(RAND() % Chance))
      return true;
  }

  return false;
}

int organic::GetSpoilPercentage() const
{
  return 100 * SpoilCounter / GetSpoilModifier();
}
