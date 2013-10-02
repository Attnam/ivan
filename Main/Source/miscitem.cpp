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

void materialcontainer::SetSecondaryMaterial(material* What, int SpecialFlags) { SetMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void materialcontainer::ChangeSecondaryMaterial(material* What, int SpecialFlags) { ChangeMaterial(SecondaryMaterial, What, GetDefaultSecondaryVolume(), SpecialFlags); }
void materialcontainer::InitMaterials(material* M1, material* M2, truth CUP) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), SecondaryMaterial, M2, GetDefaultSecondaryVolume(), CUP); }
void materialcontainer::InitMaterials(const materialscript* M, const materialscript* C, truth CUP) { InitMaterials(M->Instantiate(), C->Instantiate(), CUP); }

int holybanana::GetSpecialFlags() const { return ST_FLAME_1; }

col16 lantern::GetMaterialColorA(int) const { return MakeRGB16(255, 255, 240); }
col16 lantern::GetMaterialColorB(int) const { return MakeRGB16(255, 255, 100); }
col16 lantern::GetMaterialColorC(int) const { return MakeRGB16(255, 255, 100); }
col16 lantern::GetMaterialColorD(int) const { return MakeRGB16(255, 255, 100); }

truth can::AddAdjective(festring& String, truth Articled) const { return AddEmptyAdjective(String, Articled); }
v2 can::GetBitmapPos(int) const { return v2(16, SecondaryMaterial ? 288 : 304); }
truth can::IsDipDestination(ccharacter*) const { return SecondaryMaterial && SecondaryMaterial->IsLiquid(); }

truth potion::IsExplosive() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive(); }
truth potion::AddAdjective(festring& String, truth Articled) const { return AddEmptyAdjective(String, Articled); }
truth potion::EffectIsGood() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->GetInteractionFlags() & EFFECT_IS_GOOD; }
truth potion::IsDipDestination(ccharacter*) const { return SecondaryMaterial && SecondaryMaterial->IsLiquid(); }

truth bananapeels::IsDangerous(ccharacter* Stepper) const { return Stepper->HasALeg(); }

truth brokenbottle::IsDangerous(ccharacter* Stepper) const { return Stepper->HasALeg(); }

long wand::GetPrice() const { return Charges > TimesUsed ? item::GetPrice() : 0; }

truth backpack::IsExplosive() const { return GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive(); }
long backpack::GetTotalExplosivePower() const { return GetSecondaryMaterial() ? GetSecondaryMaterial()->GetTotalExplosivePower() : 0; }

long stone::GetTruePrice() const { return item::GetTruePrice() << 1; }

col16 whistle::GetMaterialColorB(int) const { return MakeRGB16(80, 32, 16); }

col16 itemcontainer::GetMaterialColorB(int) const { return MakeRGB16(80, 80, 80); }

truth mine::AddAdjective(festring& String, truth Articled) const { return IsActive() && AddActiveAdjective(String, Articled); }

truth beartrap::AddAdjective(festring& String, truth Articled) const { return (IsActive() && AddActiveAdjective(String, Articled)) || (!IsActive() && item::AddAdjective(String, Articled)); }

col16 carrot::GetMaterialColorB(int) const { return MakeRGB16(80, 100, 16); }

col16 charmlyre::GetMaterialColorB(int) const { return MakeRGB16(150, 130, 110); }

truth scroll::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheatMode();
}

void scrollofteleportation::FinishReading(character* Reader)
{
  if(!Reader->IsPlayer() && Reader->CanBeSeenByPlayer())
    ADD_MESSAGE("%s disappears!", Reader->CHAR_NAME(DEFINITE));

  Reader->TeleportRandomly(true);

  if(Reader->IsPlayer())
    ADD_MESSAGE("Suddenly you realize you have teleported.");

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 150, 1 << 12);
}

truth wand::Apply(character* Terrorist)
{
  if(Terrorist->IsPlayer() && !game::TruthQuestion(CONST_S("Are you sure you want to break ") + GetName(DEFINITE) + "? [y/N]"))
    return false;

  if(Terrorist->IsPlayer())
    ADD_MESSAGE("You bend %s with all your strength.", CHAR_NAME(DEFINITE));
  else if(Terrorist->CanBeSeenByPlayer())
    ADD_MESSAGE("%s bends %s with all %s strength.", Terrorist->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE), Terrorist->CHAR_POSSESSIVE_PRONOUN);

  if(Terrorist->IsPlayer() || Terrorist->CanBeSeenByPlayer())
    ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), GetBreakMsg().CStr());

  BreakEffect(Terrorist, CONST_S("killed by ") + GetName(INDEFINITE) + " broken @bk");
  Terrorist->DexterityAction(5);
  return true;
}

void wand::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << TimesUsed << Charges;
}

void wand::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> TimesUsed >> Charges;
}

void scrollofwishing::FinishReading(character* Reader)
{
  game::Wish(Reader,
	     "%s appears from nothing and the scroll burns!",
	     "Two %s appear from nothing and the scroll burns!");

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 600, 1 << 12);
}

void scrollofchangematerial::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("What item do you wish to change?"), NO_MULTI_SELECT|SELECT_PAIR, &item::MaterialIsChangeable))
    {
      ADD_MESSAGE("You notice you haven't got anything to alter.");
      return;
    }

    if(Item.empty())
      if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
	return;
      else
	continue;

    if(!Item[0]->IsMaterialChangeable())
    {
      ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
      break;
    }

    if(Item[0]->HandleInPairs() && Item.size() == 1)
    {
      ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

      if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	continue;
    }

    festring Temp = game::DefaultQuestion(CONST_S("What material do you want to wish for?"),
					  game::GetDefaultChangeMaterial());
    material* TempMaterial = protosystem::CreateMaterial(Temp);

    if(!TempMaterial)
    {
      game::DrawEverythingNoBlit();
      continue;
    }

    msgsystem::EnterBigMessageMode();

    if(Item.size() == 1)
      ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames.", Item[0]->CHAR_NAME(UNARTICLED));
    else
      ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames.", Item[0]->CHAR_NAME(PLURAL));

    if(TempMaterial->GetIntelligenceRequirement() + 5 > Reader->GetAttribute(INTELLIGENCE) && !game::WizardModeIsActive())
    {
      ADD_MESSAGE("But your mind is not yet strong enough to summon enough %s for the change.", TempMaterial->GetName(false, false).CStr());
      delete TempMaterial;
      msgsystem::LeaveBigMessageMode();
      continue;
    }

    material* MainMaterial = Item[0]->GetMainMaterial();
    material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

    if(Item.size() == 1)
    {
      if(!MainMaterial->IsSameAs(TempMaterial))
      {
	ADD_MESSAGE("As the fire dies out it looks greatly altered.");

	if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	  Item[0]->ChangeSecondaryMaterial(TempMaterial->SpawnMore());

	Item[0]->ChangeMainMaterial(TempMaterial);
      }
      else
	ADD_MESSAGE("As the fire dies out it looks unchanged.");
    }
    else
    {
      if(!MainMaterial->IsSameAs(TempMaterial))
      {
	ADD_MESSAGE("As the fire dies out they look greatly altered.");

	if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	  for(uint c = 0; c < Item.size(); ++c)
	    Item[c]->ChangeSecondaryMaterial(TempMaterial->SpawnMore());

	Item[0]->ChangeMainMaterial(TempMaterial);

	for(uint c = 1; c < Item.size(); ++c)
	  Item[c]->ChangeMainMaterial(TempMaterial->SpawnMore());
      }
      else
	ADD_MESSAGE("As the fire dies out they look unchanged.");
    }

    msgsystem::LeaveBigMessageMode();
    break;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 500, 1 << 12);
}

item* brokenbottle::BetterVersion() const
{
  return potion::Spawn();
}

void brokenbottle::StepOnEffect(character* Stepper)
{
  if(Stepper->HasALeg() && !(RAND() % 5))
  {
    if(Stepper->IsPlayer())
      ADD_MESSAGE("Ouch. You step on sharp glass splinters.");
    else if(Stepper->CanBeSeenByPlayer())
      ADD_MESSAGE("%s steps on sharp glass splinters.", Stepper->CHAR_NAME(DEFINITE));

    Stepper->ReceiveDamage(0, 1 + RAND() % 3, PHYSICAL_DAMAGE, LEGS);
    Stepper->CheckDeath(CONST_S("stepped on a broken bottle"), 0);
  }
}

liquid* can::CreateDipLiquid()
{
  return static_cast<liquid*>(GetSecondaryMaterial()->TakeDipVolumeAway());
}

liquid* potion::CreateDipLiquid()
{
  return static_cast<liquid*>(GetSecondaryMaterial()->TakeDipVolumeAway());
}

void potion::DipInto(liquid* Liquid, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  ChangeSecondaryMaterial(Liquid);
  Dipper->DexterityAction(10);
}

void lantern::SignalSquarePositionChange(int SquarePosition)
{
  item::SignalSquarePositionChange(SquarePosition);
  UpdatePictures();
}

item* potion::BetterVersion() const
{
  if(!GetSecondaryMaterial())
    return potion::Spawn();
  else
    return 0;
}

item* can::BetterVersion() const
{
  if(!GetSecondaryMaterial())
    return can::Spawn();
  else
    return 0;
}

truth backpack::Apply(character* Terrorist)
{
  if(IsExplosive())
  {
    if(Terrorist->IsPlayer())
      ADD_MESSAGE("You light your %s. It explodes!", CHAR_NAME(UNARTICLED));
    else if(Terrorist->CanBeSeenByPlayer())
      ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
    else if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("Something explodes!");

    RemoveFromSlot();
    SendToHell();
    Terrorist->DexterityAction(5);
    Terrorist->GetLevel()->Explosion(Terrorist, CONST_S("kamikazed @k"), Terrorist->GetLSquareUnder()->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
    return true;
  }
  else if(Terrorist->IsPlayer())
    ADD_MESSAGE("You are not able to explode yourself with this crummy %s!", CHAR_NAME(UNARTICLED));

  return false;
}

truth holybook::CanBeRead(character* Reader) const
{
  return Reader->CanRead() || game::GetSeeWholeMapCheatMode();
}

void holybook::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
  {
    PLAYER->EditExperience(INTELLIGENCE, 75, 1 << 12);
    PLAYER->EditExperience(WISDOM, 150, 1 << 12);

    if(GetMasterGod()->IsKnown())
    {
      ADD_MESSAGE("The book reveals many divine secrets of %s to you.", GetMasterGod()->GetName());
      GetMasterGod()->AdjustRelation(75);
      game::ApplyDivineAlignmentBonuses(GetMasterGod(), 15, true);

      if(!(RAND() % 3))
      {
	ADD_MESSAGE("But then it disappears.");
	RemoveFromSlot();
	SendToHell();
      }
    }
    else
    {
      game::LearnAbout(GetMasterGod());
      game::LearnAbout(GetMasterGod());
      ADD_MESSAGE("You feel you master the magical rites of %s.", GetMasterGod()->GetName());
    }
  }
}

truth wand::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (FIRE|ENERGY|PHYSICAL_DAMAGE) && Damage && (Damage > 125 || !(RAND() % (250 / Damage))))
  {
    festring DeathMsg = CONST_S("killed by an explosion of ");
    AddName(DeathMsg, INDEFINITE);

    if(Damager)
      DeathMsg << " caused @bk";

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s %s.", GetExtendedDescription().CStr(), GetBreakMsg().CStr());

    BreakEffect(Damager, DeathMsg);
    return true;
  }

  return false;
}

truth backpack::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (FIRE|ENERGY) && Damage && IsExplosive() && (Damage > 25 || !(RAND() % (50 / Damage))))
  {
    festring DeathMsg = CONST_S("killed by an explosion of ");
    AddName(DeathMsg, INDEFINITE);

    if(Damager)
      DeathMsg << " caused @bk";

    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    lsquare* Square = GetLSquareUnder();
    RemoveFromSlot();
    SendToHell();
    Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
    return true;
  }

  return false;
}

truth scroll::ReceiveDamage(character*, int Damage, int Type, int)
{
  if(Type & FIRE && Damage
     && GetMainMaterial()->GetInteractionFlags() & CAN_BURN
     && (Damage > 125 || !(RAND() % (250 / Damage))))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s catches fire!", GetExtendedDescription().CStr());

    RemoveFromSlot();
    SendToHell();
    return true;
  }

  return false;
}

truth holybook::ReceiveDamage(character*, int Damage, int Type, int)
{
  if(Type & FIRE && Damage
     && GetMainMaterial()->GetInteractionFlags() & CAN_BURN
     && (Damage > 125 || !(RAND() % (250 / Damage))))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s catches fire!", GetExtendedDescription().CStr());

    RemoveFromSlot();
    SendToHell();
    return true;
  }

  return false;
}

truth oillamp::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You rub %s.", CHAR_NAME(DEFINITE));

  if(GetInhabitedByGenie())
  {
    genie* Genie = genie::Spawn();
    v2 TryToCreate;
    truth FoundPlace = false;

    if(RAND_N(5))
      Genie->SetTeam(Applier->GetTeam());
    else
      Genie->SetTeam(game::GetTeam(MONSTER_TEAM));

    /* First try to create a genie nearby (10 tries - if all of them fail then stop trying) */

    for(int c = 0; c < 10 && !FoundPlace; ++c)
    {
      TryToCreate = Applier->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

      if(GetArea()->IsValidPos(TryToCreate) && Genie->CanMoveOn(GetNearLSquare(TryToCreate)) && Genie->IsFreeForMe(GetNearLSquare(TryToCreate)))
      {
	Genie->PutTo(TryToCreate);
	FoundPlace = true;
	SetInhabitedByGenie(false);
	break;
      }
    }

    if(FoundPlace)
    {
      Genie->GetLSquareUnder()->AddSmoke(gas::Spawn(SMOKE, 1000));

      if(!Genie->IsPet())
	ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! As a reward, I will kill you.\"", Genie->CHAR_NAME(INDEFINITE));
      else
      {
	if(Applier->IsPlayer())
	{
	  ADD_MESSAGE("You see a puff of smoke, and %s appears. \"For centuries I have been imprisoned in this lamp. But at last you have freed me! I am deeply grateful. You deserve a generous reward. I may serve you for 1001 nights or grant you a wish. It's your choice.\"", Genie->CHAR_NAME(INDEFINITE));

	  if(game::TruthQuestion(CONST_S("Do you want to wish? [Y/n]"), YES))
	  {
	    ADD_MESSAGE("You may wish for an item.");
	    game::Wish(Applier,
		       "%s appears from nothing and the genie flies happily away!",
		       "Two %s appear from nothing and the genie flies happily away!");

	    Genie->Remove();
	    delete Genie;
	    Applier->EditAP(-1000);
	    return true;
	  }
	}
      }

      meleeweapon* Weapon = meleeweapon::Spawn(TWO_HANDED_SCIMITAR, NO_MATERIALS);
      Weapon->InitMaterials(MAKE_MATERIAL(ARCANITE), MAKE_MATERIAL(ARCANITE), true);
      Genie->SetRightWielded(Weapon);
      ADD_MESSAGE("%s wishes for a two-handed scimitar. Suddenly %s appears from nothing and %s wields it.", Genie->CHAR_NAME(DEFINITE), Genie->GetMainWielded()->CHAR_NAME(INDEFINITE), Genie->CHAR_NAME(DEFINITE));
    }
    else
    {
      if(Applier->IsPlayer())
	ADD_MESSAGE("You feel that it is warmer.");

      delete Genie;
    }
  }
  else
    if(Applier->IsPlayer())
      ADD_MESSAGE("Nothing happens.");

  Applier->EditAP(-1000);
  return true;
}

void oillamp::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << InhabitedByGenie;
}

void oillamp::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> InhabitedByGenie;
}

col16 holybook::GetMaterialColorA(int) const
{
  return GetMasterGod()->GetColor();
}

void scrollofcharging::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to charge?"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsChargeable))
    {
      ADD_MESSAGE("You notice you haven't got anything to charge.");
      return;
    }

    if(!Item.empty())
    {
      if(Item[0]->HandleInPairs() && Item.size() == 1)
      {
	ADD_MESSAGE("Only one %s will be charged.", Item[0]->CHAR_NAME(UNARTICLED));

	if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	  continue;
      }

      for(uint c = 0; c < Item.size(); ++c)
	Item[c]->ChargeFully(Reader);

      ADD_MESSAGE("You charge %s and the scroll burns.", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
      break;
    }
    else if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
      return;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

void bananapeels::StepOnEffect(character* Stepper)
{
  if(Stepper->HasALeg() && !(RAND() % 5))
  {
    if(Stepper->IsPlayer())
      ADD_MESSAGE("Ouch. Your feet slip on %s and you fall down.", CHAR_NAME(INDEFINITE));
    else if(Stepper->CanBeSeenByPlayer())
      ADD_MESSAGE("%s steps on %s and falls down.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

    /* Do damage against any random bodypart except legs */

    Stepper->ReceiveDamage(0, 1 + (RAND() & 1), PHYSICAL_DAMAGE, ALL&~LEGS);
    Stepper->CheckDeath(CONST_S("slipped on a banana peel"), 0);
    Stepper->EditAP(-500);
  }
}

void scrolloftaming::FinishReading(character* Reader)
{
  // First find all tameable characters in the squares around Reader

  character* CharacterNear[8];
  int Index = 0;

  for(int c = 0; c < 8; ++c)
  {
    v2 Test = Reader->GetPos() + game::GetMoveVector(c);

    if(GetArea()->IsValidPos(Test))
    {
      character* Char = GetNearSquare(Test)->GetCharacter();

      if(Char && Char->CanTameWithScroll(Reader) && Char->GetTeam() != Reader->GetTeam())
	CharacterNear[Index++] = Char;
    }
  }

  // Then pick one of the characters and set it to the same team as Reader

  if(!Index)
  {
    if(Reader->IsPlayer() || Reader->CanBeSeenByPlayer())
      ADD_MESSAGE("The scroll burns, but nothing happens.");
  }
  else
  {
    character* ToBeTamed = CharacterNear[RAND() % Index];
    ToBeTamed->ChangeTeam(Reader->GetTeam());

    if(Reader->IsPlayer())
    {
      if(ToBeTamed->CanBeSeenByPlayer())
	ADD_MESSAGE("%s looks much friendlier.", ToBeTamed->CHAR_NAME(INDEFINITE));
      else
	ADD_MESSAGE("You notice no effect.");
    }
    else if(Reader->CanBeSeenByPlayer())
    {
      if(ToBeTamed->CanBeSeenByPlayer())
	ADD_MESSAGE("%s seems to like %s far more.", ToBeTamed->CHAR_NAME(INDEFINITE), Reader->CHAR_NAME(DEFINITE));
    }
    else if(ToBeTamed->CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s looks like having fallen in love with something nearby.", ToBeTamed->CHAR_NAME(INDEFINITE));
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

truth mine::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if((Type & (FIRE|ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage)))) || (Type & (PHYSICAL_DAMAGE|SOUND) && WillExplode(0)))
  {
    festring DeathMsg = CONST_S("killed by an explosion of ");
    AddName(DeathMsg, INDEFINITE);

    if(Damager)
      DeathMsg << " caused @bk";

    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    lsquare* Square = GetLSquareUnder();
    RemoveFromSlot();
    SendToHell();
    Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
    return true;
  }

  return false;
}

void mine::StepOnEffect(character* Stepper)
{
  if(!WillExplode(Stepper))
    return;

  if(Stepper->IsPlayer())
  {
    cchar* SenseVerb = Stepper->CanHear() ? "hear" : "sense";

    if(GetLSquareUnder()->IsDark())
      ADD_MESSAGE("You %s a faint thump. You try to look down, but it is too dark to see anything.", SenseVerb);
    else
      ADD_MESSAGE("You %s a faint thump. You look down. You see %s.", SenseVerb, CHAR_NAME(INDEFINITE));
  }
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s steps on %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));
  else if(GetSquareUnder()->CanBeSeenByPlayer(true))
    ADD_MESSAGE("Something explodes!");

  SetIsActive(false);
  SendNewDrawAndMemorizedUpdateRequest();

  if(Stepper->IsPlayer())
    game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevel()->Explosion(0, "killed by a land mine", Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
}

truth key::Apply(character* User)
{
  if(User->IsPlayer())
  {
    if(!User->CanOpen())
    {
      ADD_MESSAGE("This monster type cannot use keys.");
      return false;
    }
    int Key;
    truth OpenableItems = User->GetStack()->SortedItems(User, &item::HasLock);

    if(OpenableItems)
      Key = game::AskForKeyPress(CONST_S("What do you wish to lock or unlock? [press a direction key, space or i]"));
    else
      Key = game::AskForKeyPress(CONST_S("What do you wish to lock or unlock? [press a direction key or space]"));

    if(Key == 'i' && OpenableItems)
    {
      item* Item = User->GetStack()->DrawContents(User, CONST_S("What do you want to lock or unlock?"), 0, &item::IsOpenable);
      return Item && Item->TryKey(this, User);
    }

    v2 DirVect = game::GetDirectionVectorForKey(Key);

    if(DirVect != ERROR_V2 && User->GetArea()->IsValidPos(User->GetPos() + DirVect))
      return GetLevel()->GetLSquare(User->GetPos() + DirVect)->TryKey(this, User);
  }
  return true;
}

void materialcontainer::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << SecondaryMaterial;
}

void materialcontainer::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  LoadMaterial(SaveFile, SecondaryMaterial);
}

material* materialcontainer::GetMaterial(int I) const
{
  return !I ? MainMaterial : SecondaryMaterial;
}

col16 materialcontainer::GetMaterialColorB(int Frame) const
{
  if(GetSecondaryMaterial())
    return GetSecondaryMaterial()->GetColor();
  else
    return GetMaterialColorA(Frame);
}

alpha materialcontainer::GetAlphaB(int Frame) const
{
  if(GetSecondaryMaterial() && GetSecondaryMaterial()->GetAlpha() > GetAlphaA(Frame))
    return GetSecondaryMaterial()->GetAlpha();
  else
    return GetAlphaA(Frame);
}

void wand::PostConstruct()
{
  Charges = GetMinCharges() + RAND() % (GetMaxCharges() - GetMinCharges() + 1);
  TimesUsed = 0;
}

oillamp::oillamp()
{
  if(!game::IsLoading())
    InhabitedByGenie = RAND_2;
}

truth whistle::Apply(character* Whistler)
{
  if(!Whistler->HasHead())
  {
    ADD_MESSAGE("You must have a head to do this.");
    return false;
  }

  BlowEffect(Whistler);
  Whistler->EditAP(-1000);
  return true;
}

void whistle::BlowEffect(character* Whistler)
{
  if(Whistler->IsPlayer())
  {
    if(Whistler->CanHear())
      ADD_MESSAGE("You produce an interesting sound.");
    else
      ADD_MESSAGE("You blow %s", CHAR_NAME(DEFINITE));
  }
  else if(Whistler->CanBeSeenByPlayer())
  {
    if(PLAYER->CanHear())
      ADD_MESSAGE("%s blows %s and produces an interesting sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("%s blows %s.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  }
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear a whistle playing.");

  game::CallForAttention(GetPos(), 400);
}

struct distancepair
{
  distancepair(long Distance, character* Char) : Distance(Distance), Char(Char) { }
  bool operator<(const distancepair& D) const { return Distance > D.Distance; }
  long Distance;
  character* Char;
};

void magicalwhistle::BlowEffect(character* Whistler)
{
  if(LastUsed && game::GetTick() - LastUsed < 2000)
  {
    whistle::BlowEffect(Whistler);
    return;
  }
  else
    LastUsed = game::GetTick();

  if(Whistler->IsPlayer())
  {
    if(Whistler->CanHear())
      ADD_MESSAGE("You produce a peculiar sound.");
    else
      ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
  }
  else if(Whistler->CanBeSeenByPlayer())
  {
    if(PLAYER->CanHear())
      ADD_MESSAGE("%s blows %s and produces a peculiar sound.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("%s blows %s.", Whistler->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  }
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear a strange tune playing.");

  const std::list<character*>& Member = Whistler->GetTeam()->GetMember();
  std::vector<distancepair> ToSort;
  v2 Pos = Whistler->GetPos();

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled() && Whistler != *i)
      ToSort.push_back(distancepair((Pos - (*i)->GetPos()).GetLengthSquare(), *i));

  if(ToSort.size() > 5)
    std::sort(ToSort.begin(), ToSort.end());

  for(uint c = 0; c < 5 && c < ToSort.size(); ++c)
    ToSort[c].Char->TeleportNear(Whistler);

  game::CallForAttention(GetPos(), 400);
}

itemcontainer::itemcontainer()
{
  Contained = new stack(0, this, HIDDEN);
}

void itemcontainer::PostConstruct()
{
  lockableitem::PostConstruct();
  SetIsLocked(RAND_N(3));
  long ItemNumber = RAND() % (GetMaxGeneratedContainedItems() + 1);

  for(int c = 0; c < ItemNumber; ++c)
  {
    item* NewItem = protosystem::BalancedCreateItem();
    long Volume = NewItem->GetVolume();

    if(NewItem->HandleInPairs())
      Volume <<= 1;

    if(NewItem->CanBeGeneratedInContainer()
       && (GetStorageVolume() - GetContained()->GetVolume()) >= Volume)
    {
      GetContained()->AddItem(NewItem);
      NewItem->SpecialGenerationHandler();
    }
    else
      delete NewItem;
  }
}

void materialcontainer::GenerateMaterials()
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

/* Returns true if container opens fine else false */

truth itemcontainer::Open(character* Opener)
{
  if(IsLocked())
  {
    ADD_MESSAGE("%s seems to be locked.", CHAR_NAME(DEFINITE));
    return false;
  }

  festring Question = CONST_S("Do you want to (t)ake something from or (p)ut something in this container? [t,p]");
  truth Success;

  switch(game::KeyQuestion(Question, KEY_ESC, 3, 't', 'p', KEY_ESC))
  {
   case 't':
   case 'T':
    Success = GetContained()->TakeSomethingFrom(Opener, GetName(DEFINITE));
    break;
   case 'p':
   case 'P':
    Success = GetContained()->PutSomethingIn(Opener, GetName(DEFINITE), GetStorageVolume(), GetID());
    break;
   default:
    return false;
  }

  if(Success)
    Opener->DexterityAction(Opener->OpenMultiplier() * 5);

  return Success;
}

void itemcontainer::Save(outputfile& SaveFile) const
{
  lockableitem::Save(SaveFile);
  Contained->Save(SaveFile);}

void itemcontainer::Load(inputfile& SaveFile)
{
  lockableitem::Load(SaveFile);
  Contained->Load(SaveFile);
}

truth itemcontainer::Polymorph(character* Polymorpher, stack* CurrentStack)
{
  GetContained()->MoveItemsTo(CurrentStack);
  item::Polymorph(Polymorpher, CurrentStack);
  return true;
}

itemcontainer::~itemcontainer()
{
  delete Contained;
}

beartrap::beartrap(const beartrap& Trap) : mybase(Trap)
{
  TrapData.TrapID = game::CreateNewTrapID(this);
  TrapData.VictimID = 0;
}

truth beartrap::TryToUnStick(character* Victim, v2)
{
  ulong TrapID = GetTrapID();
  int Modifier = GetBaseTrapDamage() * 40 / Max(Victim->GetAttribute(DEXTERITY) + Victim->GetAttribute(ARM_STRENGTH), 1);

  if(!RAND_N(Max(Modifier, 2)))
  {
    Victim->RemoveTrap(TrapID);
    TrapData.VictimID = 0;

    if(Victim->IsPlayer())
      ADD_MESSAGE("You manage to free yourself from %s.", CHAR_NAME(DEFINITE));
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s manages to free %sself from %s.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

    Victim->EditAP(-500);
    return true;
  }

  if(!RAND_N(Max(Modifier << 1, 2)))
  {
    Victim->RemoveTrap(TrapID);
    TrapData.VictimID = 0;
    Break(Victim);

    if(Victim->IsPlayer())
      ADD_MESSAGE("You are freed.");
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s is freed.", Victim->CHAR_NAME(DEFINITE));

    Victim->EditAP(-500);
    return true;
  }

  Modifier = Victim->GetAttribute(DEXTERITY) + Victim->GetAttribute(ARM_STRENGTH) * 3 / 20;

  if(!RAND_N(Max(Modifier, 2)))
  {
    int BodyPart = Victim->RandomizeHurtBodyPart(TrapData.BodyParts);

    if(Victim->IsPlayer())
      ADD_MESSAGE("You manage to hurt your %s even more.", Victim->GetBodyPartName(BodyPart).CStr());
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s hurts %s %s more with %s.", Victim->CHAR_NAME(DEFINITE), Victim->GetPossessivePronoun().CStr(), Victim->GetBodyPartName(BodyPart).CStr(), CHAR_NAME(DEFINITE));

    Victim->ReceiveBodyPartDamage(0, GetBaseTrapDamage(), PHYSICAL_DAMAGE, BodyPart, YOURSELF, false, false, false);
    Victim->CheckDeath(CONST_S("died while trying to escape from ") + GetName(INDEFINITE), 0, IGNORE_TRAPS);
    Victim->EditAP(-1000);
    return false;
  }

  if(!RAND_N(Max(Modifier << 1, 2)))
  {
    int VictimBodyPart = Victim->RandomizeTryToUnStickBodyPart(ALL_BODYPART_FLAGS&~TrapData.BodyParts);

    if(VictimBodyPart != NONE_INDEX)
    {
      TrapData.BodyParts |= 1 << VictimBodyPart;
      Victim->AddTrap(GetTrapID(), 1 << VictimBodyPart);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You fail to free yourself from %s and your %s is stuck in it in the attempt.", CHAR_NAME(DEFINITE), Victim->GetBodyPartName(VictimBodyPart).CStr());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to free %sself from %s but is stuck more tightly in it in the attempt.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

      Victim->ReceiveBodyPartDamage(0, GetBaseTrapDamage() << 1, PHYSICAL_DAMAGE, VictimBodyPart, YOURSELF, false, false, false);
      Victim->CheckDeath(CONST_S("died while trying to escape from ") + GetName(INDEFINITE), 0, IGNORE_TRAPS);
      Victim->EditAP(-1000);
      return true;
    }
  }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from %s.", CHAR_NAME(DEFINITE));

  Victim->EditAP(-1000);
  return false;
}

void beartrap::Load(inputfile& SaveFile)
{
  mybase::Load(SaveFile);
  SaveFile >> TrapData;
  game::AddTrapID(this, TrapData.TrapID);
}

void beartrap::Save(outputfile& SaveFile) const
{
  mybase::Save(SaveFile);
  SaveFile << TrapData;
}

beartrap::beartrap()
{
  if(!game::IsLoading())
  {
    TrapData.TrapID = game::CreateNewTrapID(this);
    TrapData.VictimID = 0;
  }
}

beartrap::~beartrap()
{
  game::RemoveTrapID(TrapData.TrapID);
}

void beartrap::StepOnEffect(character* Stepper)
{
  if(IsActive() && !IsBroken())
  {
    int StepperBodyPart = Stepper->GetRandomStepperBodyPart();

    if(StepperBodyPart == NONE_INDEX)
      return;

    TrapData.VictimID = Stepper->GetID();
    TrapData.BodyParts = 1 << StepperBodyPart;
    Stepper->AddTrap(GetTrapID(), 1 << StepperBodyPart);

    if(Stepper->IsPlayer())
      ADD_MESSAGE("You step in %s and it traps your %s.", CHAR_NAME(INDEFINITE), Stepper->GetBodyPartName(StepperBodyPart).CStr());
    else if(Stepper->CanBeSeenByPlayer())
      ADD_MESSAGE("%s is trapped in %s.", Stepper->CHAR_NAME(DEFINITE), CHAR_NAME(INDEFINITE));

    SetIsActive(false);
    SendNewDrawAndMemorizedUpdateRequest();

    if(Stepper->IsPlayer())
      game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

    Stepper->ReceiveBodyPartDamage(0, GetBaseTrapDamage() << 1, PHYSICAL_DAMAGE, StepperBodyPart, YOURSELF, false, false, false);
    Stepper->CheckDeath(CONST_S("died by stepping to ") + GetName(INDEFINITE), 0, IGNORE_TRAPS);
  }
}

truth beartrap::CheckPickUpEffect(character* Picker)
{
  if(Picker->IsStuckToTrap(GetTrapID()))
  {
    if(Picker->IsPlayer())
      ADD_MESSAGE("You are tightly stuck in %s.", CHAR_NAME(DEFINITE));

    return false;
  }

  SetIsActive(false);
  return true;
}

int lantern::GetSpecialFlags() const
{
  switch(GetSquarePosition())
  {
   case LEFT: return ROTATE|MIRROR;
   case DOWN: return FLIP;
   case UP: return 0;
   case RIGHT: return ROTATE;
  }

  return 0;
}

truth stethoscope::Apply(character* Doctor)
{
  if(!Doctor->CanUseStethoscope(true))
    return false;

  if(!Doctor->IsPlayer())
    ABORT("Doctor is not here, man, but these pills taste just as good anyway.");

  int Dir = game::DirectionQuestion(CONST_S("What do you want to inspect? [press a direction key]"), false,true);

  if(Dir == DIR_ERROR)
    return false;

  Doctor->DexterityAction(2);
  character* Char = GetNearSquare(GetPos() + game::GetMoveVector(Dir))->GetCharacter();

  if(!Char)
  {
    ADD_MESSAGE("There's no-one here.");
    return false;
  }

  Char->DisplayStethoscopeInfo(Doctor);
  return true;
}

void itemcontainer::CalculateVolumeAndWeight()
{
  item::CalculateVolumeAndWeight();
  Volume += Contained->GetVolume();
  Weight += Contained->GetWeight();
}

materialcontainer::~materialcontainer()
{
  delete SecondaryMaterial;
}

truth itemcontainer::ContentsCanBeSeenBy(ccharacter* Viewer) const
{
  return GetMainMaterial()->IsTransparent() && CanBeSeenBy(Viewer);
}

truth mine::Apply(character* User)
{
  if(User->IsPlayer() && !game::TruthQuestion(CONST_S("Are you sure you want to plant ") + GetName(DEFINITE) + "? [y/N]"))
    return false;

  room* Room = GetRoom();

  if(Room)
    Room->HostileAction(User);

  if(User->IsPlayer())
    ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

  SetIsActive(!IsActive());
  User->DexterityAction(10);

  if(IsActive())
  {
    Team = User->GetTeam()->GetID();
    RemoveFromSlot();
    User->GetStackUnder()->AddItem(this);
  }

  return true;
}

truth beartrap::Apply(character* User)
{
  if(IsBroken())
  {
    if(User->IsPlayer())
      ADD_MESSAGE("%s is useless.", CHAR_NAME(DEFINITE));

    return false;
  }

  if(User->IsPlayer()
     && !game::TruthQuestion(CONST_S("Are you sure you want to plant ") + GetName(DEFINITE) + "? [y/N]"))
    return false;

  room* Room = GetRoom();

  if(Room)
    Room->HostileAction(User);

  if(User->GetAttribute(DEXTERITY) < femath::LoopRoll(90, 1000))
  {
    int UserBodyPart = User->GetRandomApplyBodyPart();

    if(User->IsPlayer())
      ADD_MESSAGE("Somehow you manage to trap your %s in %s.", User->GetBodyPartName(UserBodyPart).CStr(), CHAR_NAME(DEFINITE));
    else if(User->CanBeSeenByPlayer())
      ADD_MESSAGE("%s somehow traps %sself in %s.", User->CHAR_NAME(DEFINITE), User->CHAR_OBJECT_PRONOUN, CHAR_NAME(DEFINITE));

    RemoveFromSlot();
    User->GetStackUnder()->AddItem(this);
    TrapData.VictimID = User->GetID();
    TrapData.BodyParts = 1 << UserBodyPart;
    User->AddTrap(GetTrapID(), 1 << UserBodyPart);
    SendNewDrawAndMemorizedUpdateRequest();

    if(User->IsPlayer())
      game::AskForKeyPress(CONST_S("Trap activated! [press any key to continue]"));

    User->ReceiveBodyPartDamage(0, 1 + (RAND() & 1), PHYSICAL_DAMAGE, UserBodyPart, YOURSELF, false, false, false);
    User->CheckDeath(CONST_S("died failing to set ") + GetName(INDEFINITE), 0, IGNORE_TRAPS);
  }
  else
  {
    if(User->IsPlayer())
      ADD_MESSAGE("%s is now %sactive.", CHAR_NAME(DEFINITE), IsActive() ? "in" : "");

    SetIsActive(!IsActive());
    User->DexterityAction(10);

    if(IsActive())
    {
      Team = User->GetTeam()->GetID();
      RemoveFromSlot();
      User->GetStackUnder()->AddItem(this);
    }
  }

  return true;
}

v2 beartrap::GetBitmapPos(int Frame) const
{
  if(!IsBroken())
    return IsActive() ? v2(32, 304) : v2(32, 320);
  else
    return item::GetBitmapPos(Frame);
}

truth mine::WillExplode(ccharacter* Stepper) const
{
  return IsActive() && GetSecondaryMaterial() && GetSecondaryMaterial()->IsExplosive() && (!Stepper || Stepper->GetWeight() > 5000);
}

materialcontainer::materialcontainer(const materialcontainer& MC) : mybase(MC)
{
  CopyMaterial(MC.SecondaryMaterial, SecondaryMaterial);
}

itemcontainer::itemcontainer(const itemcontainer& Container) : mybase(Container)
{
  Contained = new stack(0, this, HIDDEN);
  CalculateAll();
}

oillamp::oillamp(const oillamp& Lamp) : mybase(Lamp), InhabitedByGenie(false)
{
}

truth wand::Zap(character* Zapper, v2, int Direction)
{
  if(Charges <= TimesUsed)
  {
    ADD_MESSAGE("Nothing happens.");
    return true;
  }

  Zapper->EditExperience(PERCEPTION, 150, 1 << 10);

  beamdata Beam
    (
      Zapper,
      CONST_S("killed by ") + GetName(INDEFINITE) + " zapped @bk",
      Zapper->GetPos(),
      GetBeamColor(),
      GetBeamEffect(),
      Direction,
      GetBeamRange(),
      GetSpecialParameters()
    );

  (GetLevel()->*level::GetBeam(GetBeamStyle()))(Beam);
  ++TimesUsed;
  return true;
}

void wand::AddInventoryEntry(ccharacter*, festring& Entry, int, truth ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
  {
    Entry << " [" << GetWeight();

    if(TimesUsed == 1)
      Entry << "g, used 1 time]";
    else if(TimesUsed)
      Entry << "g, used " << TimesUsed << " times]";
    else
      Entry << "g]";
  }
}

void materialcontainer::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == MainMaterial)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s becomes so spoiled that it cannot hold its contents anymore.", CHAR_NAME(DEFINITE));

    RemoveMainMaterial();
  }
  else
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("The contents of %s spoil completely.", CHAR_NAME(DEFINITE));

    delete RemoveSecondaryMaterial();
  }
}

truth materialcontainer::CanBePiledWith(citem* Item, ccharacter* Viewer) const
{
  if(!item::CanBePiledWith(Item, Viewer))
    return false;

  const materialcontainer* Weapon = static_cast<const materialcontainer*>(Item);

  if(!SecondaryMaterial && !Weapon->SecondaryMaterial)
    return true;

  return SecondaryMaterial
    && Weapon->SecondaryMaterial
    && SecondaryMaterial->IsSameAs(Weapon->SecondaryMaterial)
    && SecondaryMaterial->GetSpoilLevel() == Weapon->SecondaryMaterial->GetSpoilLevel();
}

long itemcontainer::GetTruePrice() const
{
  return GetContained()->GetTruePrice() + item::GetTruePrice();
}

void potion::Break(character* Breaker, int Dir)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shatters to pieces.", GetExtendedDescription().CStr());
  else if(PLAYER->CanHear())
    ADD_MESSAGE("You hear something shattering.");

  if(Breaker && IsOnGround())
  {
    room* Room = GetRoom();

    if(Room)
      Room->HostileAction(Breaker);
  }

  item* Remains = brokenbottle::Spawn(0, NO_MATERIALS);
  Remains->InitMaterials(GetMainMaterial()->SpawnMore());
  DonateFluidsTo(Remains);
  DonateIDTo(Remains);
  DonateSlotTo(Remains);
  SendToHell();

  if(GetSecondaryMaterial() && GetSecondaryMaterial()->IsLiquid())
  {
    liquid* Liquid = static_cast<liquid*>(GetSecondaryMaterial());

    if(Dir != YOURSELF)
    {
      v2 Pos = Remains->GetPos() + game::GetMoveVector(Dir);

      if(Remains->GetLevel()->IsValidPos(Pos))
      {
	long HalfVolume = GetSecondaryMaterial()->GetVolume() >> 1;
	Liquid->EditVolume(-HalfVolume);
	Remains->GetNearLSquare(Pos)->SpillFluid(Breaker, Liquid->SpawnMoreLiquid(HalfVolume));
      }
    }

    if(Remains->Exists())
      Remains->GetLSquareUnder()->SpillFluid(Breaker, Liquid->SpawnMoreLiquid(Liquid->GetVolume()));
  }

  if(PLAYER->Equips(Remains))
    game::AskForKeyPress(CONST_S("Equipment broken! [press any key to continue]"));
}

void materialcontainer::Be()
{
  item::Be();

  if(Exists() && SecondaryMaterial)
    SecondaryMaterial->Be(ItemFlags);
}

int materialcontainer::GetSparkleFlags() const
{
  return (MainMaterial->IsSparkling() ? SPARKLING_A : 0)
    | (SecondaryMaterial && SecondaryMaterial->IsSparkling() ? SPARKLING_B : 0);
}

void scrollofenchantweapon::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("Choose a weapon to enchant:"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsWeapon))
    {
      ADD_MESSAGE("You notice you haven't got anything to enchant.");
      return;
    }

    if(!Item.empty())
    {
      if(!Item[0]->CanBeEnchanted())
      {
	ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	break;
      }

      if(Item[0]->HandleInPairs() && Item.size() == 1)
      {
	ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

	if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	  continue;
      }

      if(Item[0]->GetEnchantment() >= 5 && RAND_GOOD(Item[0]->GetEnchantment() - 3))
      {
	if(Item.size() == 1)
	  ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
	else
	  ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

	break;
      }

      if(Item.size() == 1)
	ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
      else
	ADD_MESSAGE("Your %s glow briefly red. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

      for(uint c = 0; c < Item.size(); ++c)
	Item[c]->EditEnchantment(1);

      break;
    }
    else if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
      return;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

void scrollofenchantarmor::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("Choose an armor to enchant:"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsArmor))
    {
      ADD_MESSAGE("You notice you haven't got anything to enchant.");
      return;
    }

    if(!Item.empty())
    {
      if(!Item[0]->CanBeEnchanted())
      {
	ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
	break;
      }

      if(Item[0]->HandleInPairs() && Item.size() == 1)
      {
	ADD_MESSAGE("Only one %s will be enchanted.", Item[0]->CHAR_NAME(UNARTICLED));

	if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	  continue;
      }

      if(Item[0]->GetEnchantment() >= 5 && RAND_GOOD(Item[0]->GetEnchantment() - 3))
      {
	if(Item.size() == 1)
	  ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant it further!", Item[0]->CHAR_NAME(DEFINITE));
	else
	  ADD_MESSAGE("Magic energies swirl around %s, but they fail to enchant them further!", Item[0]->CHAR_NAME(DEFINITE|PLURAL));

	break;
      }

      if(Item.size() == 1)
	ADD_MESSAGE("Your %s glows briefly blue. It feels very warm now.", Item[0]->CHAR_NAME(UNARTICLED));
      else
	ADD_MESSAGE("Your %s glow briefly blue. They feel very warm now.", Item[0]->CHAR_NAME(PLURAL));

      for(uint c = 0; c < Item.size(); ++c)
	Item[c]->EditEnchantment(1);

      break;
    }
    else if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
      return;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

truth itemcontainer::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & (PHYSICAL_DAMAGE|SOUND|ENERGY))
  {
    Contained->ReceiveDamage(Damager, Damage / GetDamageDivider(), Type);
    int SV = Max(GetStrengthValue(), 1);

    if(IsLocked() && Damage > SV && RAND() % (100 * Damage / SV) >= 100)
    {
      SetIsLocked(false);
      SetConfig(GetConfig()&~LOCK_BITS|BROKEN_LOCK);

      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The %s's lock shatters to pieces.", GetNameSingular().CStr());

      return true;
    }
    else if(Damager && Damager->IsPlayer())
      ADD_MESSAGE("THUMP!");
  }

  return false;
}

void itemcontainer::DrawContents(ccharacter* Char)
{
  festring Topic = CONST_S("Contents of your ") + GetName(UNARTICLED);
  GetContained()->DrawContents(Char, Topic, NO_SELECT);

  for(stackiterator i = GetContained()->GetBottom(); i.HasItem(); ++i)
    i->DrawContents(Char);
}

void magicalwhistle::Save(outputfile& SaveFile) const
{
  whistle::Save(SaveFile);
  SaveFile << LastUsed;
}

void magicalwhistle::Load(inputfile& SaveFile)
{
  whistle::Load(SaveFile);
  SaveFile >> LastUsed;
}

int materialcontainer::GetSpoilLevel() const
{
  return Max(MainMaterial->GetSpoilLevel(), SecondaryMaterial ? SecondaryMaterial->GetSpoilLevel() : 0);
}

void itemcontainer::SetItemsInside(const fearray<contentscript<item> >& ItemArray, int SpecialFlags)
{
  GetContained()->Clean();

  for(uint c1 = 0; c1 < ItemArray.Size; ++c1)
    if(ItemArray[c1].IsValid())
    {
      const interval* TimesPtr = ItemArray[c1].GetTimes();
      int Times = TimesPtr ? TimesPtr->Randomize() : 1;

      for(int c2 = 0; c2 < Times; ++c2)
      {
	item* Item = ItemArray[c1].Instantiate(SpecialFlags);

	if(Item)
	{
	  Contained->AddItem(Item);
	  Item->SpecialGenerationHandler();
	}
      }
    }
}

truth mine::CheckPickUpEffect(character*)
{
  if(WillExplode(0))
  {
    lsquare* Square = GetLSquareUnder();

    if(Square->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    RemoveFromSlot();
    SendToHell();
    Square->GetLevel()->Explosion(0, "killed by a land mine", Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
    return false;
  }

  return true;
}

void scrollofrepair::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to repair?"), NO_MULTI_SELECT|SELECT_PAIR, &item::IsRepairable))
    {
      ADD_MESSAGE("You notice you haven't got anything to repair.");
      return;
    }

    if(!Item.empty())
    {
      if(Item[0]->HandleInPairs() && Item.size() == 1)
      {
	ADD_MESSAGE("Only one %s will be repaired.", Item[0]->CHAR_NAME(UNARTICLED));

	if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	  continue;
      }

      if(Item.size() == 1)
	ADD_MESSAGE("As you read the scroll, %s glows green and %s.", Item[0]->CHAR_NAME(DEFINITE), Item[0]->IsBroken() ? "fixes itself" : "its rust vanishes");
      else
	ADD_MESSAGE("As you read the scroll, %s glow green and %s.", Item[0]->CHAR_NAME(PLURAL), Item[0]->IsBroken() ? "fix themselves" : "their rust vanishes");

      for(uint c = 0; c < Item.size(); ++c)
      {
	Item[c]->RemoveRust();
	Item[c]->Fix();
      }

      break;
    }
    else if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
      return;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

item* brokenbottle::Fix()
{
  potion* Potion = potion::Spawn(0, NO_MATERIALS);
  Potion->InitMaterials(GetMainMaterial(), 0);
  DonateFluidsTo(Potion);
  DonateIDTo(Potion);
  DonateSlotTo(Potion);
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
  return Potion;
}

truth encryptedscroll::Read(character*)
{
  ADD_MESSAGE("You could never hope to decipher this top secret message. It is meant for Petrus's eyes only.");
  return false;
}

truth horn::Apply(character* Blower)
{
  if(!Blower->HasHead())
  {
    if(Blower->IsPlayer())
      ADD_MESSAGE("You need a head to do this.");

    return false;
  }

  if(!LastUsed || game::GetTick() - LastUsed >= 2500)
  {
    LastUsed = game::GetTick();
    cchar* SoundDescription = GetConfig() == BRAVERY ? "loud but calming" : "frightening, almost scream-like";

    if(Blower->IsPlayer())
    {
      if(Blower->CanHear())
	ADD_MESSAGE("You produce a %s sound.", SoundDescription);
      else
	ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
    }
    else if(Blower->CanBeSeenByPlayer())
    {
      if(PLAYER->CanHear())
	ADD_MESSAGE("%s blows %s and produces a %s sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE), SoundDescription);
      else
	ADD_MESSAGE("%s blows %s.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }
    else if(PLAYER->CanHear())
      ADD_MESSAGE("You hear a %s sound echoing everywhere.", SoundDescription);

    rect Rect;
    femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 10);

    for(int x = Rect.X1; x <= Rect.X2; ++x)
      for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	character* Audience = GetNearSquare(x, y)->GetCharacter();

	if(Audience)
	{
	  if(GetConfig() == BRAVERY && Audience->CanHear() && Audience->TemporaryStateIsActivated(PANIC)
	     && Blower->IsAlly(Audience))
	  {
	    if(Audience->IsPlayer())
	      ADD_MESSAGE("You calm down.");
	    else if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s calms down.", Audience->CHAR_NAME(DEFINITE));

	    Audience->DeActivateTemporaryState(PANIC);
	  }
	  else if(GetConfig() == FEAR && !Audience->TemporaryStateIsActivated(PANIC)
		  && Blower->GetRelation(Audience) == HOSTILE && Audience->HornOfFearWorks())
	    Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
	}
      }

  }
  else
  {
    if(Blower->IsPlayer())
    {
      if(Blower->CanHear())
	ADD_MESSAGE("You produce a mighty sound.");
      else
	ADD_MESSAGE("You blow %s.", CHAR_NAME(DEFINITE));
    }
    else if(Blower->CanBeSeenByPlayer())
    {
      if(PLAYER->CanHear())
	ADD_MESSAGE("%s blows %s and produces a mighty sound.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("%s blows %s.", Blower->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }
    else if(PLAYER->CanHear())
      ADD_MESSAGE("You hear a horn being blown.");
  }

  game::CallForAttention(GetPos(), 900);
  Blower->EditAP(-1000);
  return true;
}

void horn::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void horn::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

item* bananapeels::BetterVersion() const
{
  return banana::Spawn();
}

truth beartrap::IsPickable(character* Picker) const
{
  return !IsActive() && !Picker->IsStuckToTrap(GetTrapID());
}

void banana::Save(outputfile& SaveFile) const
{
  materialcontainer::Save(SaveFile);
  SaveFile << TimesUsed << Charges << Jammed;
}

void banana::Load(inputfile& SaveFile)
{
  materialcontainer::Load(SaveFile);
  SaveFile >> TimesUsed >> Charges >> Jammed;
}

truth banana::Zap(character*, v2, int)
{
  if(IsBroken())
  {
    ADD_MESSAGE("This banana seems to be somehow disfunctional.");
    return false;
  }

  if(Jammed)
  {
    ADD_MESSAGE("Unfortunately, your banana is jammed!");
    return false;
  }

  if(Charges > TimesUsed)
  {
    if(TimesUsed && !RAND_N(10))
    {
      ADD_MESSAGE("Oh no! Your banana jams in the middle of the firefight!");
      Jammed = true;
    }
    else
    {
      ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
      ++TimesUsed;
    }
  }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void banana::SignalSpoil(material* Material)
{
  if(!Exists())
    return;

  if(Material == GetSecondaryMaterial() && !GetMainMaterial()->IsVeryCloseToSpoiling())
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("The inside of %s spoils completely.", GetExtendedDescription().CStr());

    delete RemoveSecondaryMaterial();
  }
  else
    item::SignalSpoil(Material);
}

truth bone::DogWillCatchAndConsume(ccharacter* Doggie) const
{
  return GetConsumeMaterial(Doggie)->GetConfig() == BONE
						  && !GetConsumeMaterial(Doggie)->GetSpoilLevel();
}

int itemcontainer::GetOfferValue(int Receiver) const
{
  int Sum = 0;

  for(int c = 0; c < GetContained()->GetItems(); ++c)
    Sum += GetContained()->GetItem(c)->GetOfferValue(Receiver);

  return item::GetOfferValue(Receiver) + Sum;
}

truth itemcontainer::IsDestroyable(ccharacter* Char) const
{
  for(int c = 0; c < GetContained()->GetItems(); ++c)
    if(!GetContained()->GetItem(c)->IsDestroyable(Char))
      return false;

  return true;
}

alpha lantern::GetAlphaB(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 1);
}

alpha lantern::GetAlphaC(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 2);
}

alpha lantern::GetAlphaD(int Frame) const
{
  Frame &= 31;
  return (Frame * (31 - Frame) >> 3);
}

void itemcontainer::SortAllItems(const sortdata& SortData) const
{
  item::SortAllItems(SortData);

  if(SortData.Recurse)
    GetContained()->SortAllItems(SortData);
}

int materialcontainer::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : SecondaryMaterial ? SecondaryMaterial->GetAttachedGod() : MainMaterial->GetAttachedGod();
}

void wand::BreakEffect(character* Terrorist, cfestring& DeathMsg)
{
  v2 Pos = GetPos();
  level* Level = GetLevel();
  RemoveFromSlot();
  ulong StackSize = Level->AddRadiusToSquareStack(Pos, GetBreakEffectRangeSquare());
  lsquare** SquareStack = Level->GetSquareStack();
  ulong c;

  for(c = 0; c < StackSize; ++c)
    SquareStack[c]->RemoveFlags(IN_SQUARE_STACK);

  fearray<lsquare*> Stack(SquareStack, StackSize);
  (Level->*level::GetBeamEffectVisualizer(GetBeamStyle()))(Stack, GetBeamColor());

  beamdata Beam
    (
      Terrorist,
      DeathMsg,
      YOURSELF,
      GetSpecialParameters()
    );

  for(c = 0; c < Stack.Size; ++c)
    (Stack[c]->*lsquare::GetBeamEffect(GetBeamEffect()))(Beam);

  SendToHell();
}

truth beartrap::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(!IsBroken() && Type & PHYSICAL_DAMAGE && Damage)
  {
    if(Damage > 125 || !(RAND() % (250 / Damage)))
    {
      SetIsActive(false);
      Break(Damager);
      return true;
    }
    else
    {
      if(IsActive())
      {
	if(CanBeSeenByPlayer())
	  ADD_MESSAGE("%s snaps shut.", CHAR_NAME(DEFINITE));

	SetIsActive(false);
	SendNewDrawAndMemorizedUpdateRequest();
	return true;
      }
    }
  }

  return false;
}

truth potion::ReceiveDamage(character* Damager, int Damage, int Type, int Dir)
{
  if(Type & FIRE && Damage && IsExplosive() && (Damage > 50 || !(RAND() % (100 / Damage))))
  {
    festring DeathMsg = CONST_S("killed by an explosion of ");
    AddName(DeathMsg, INDEFINITE);

    if(Damager)
      DeathMsg << " caused @bk";

    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    lsquare* Square = GetLSquareUnder();
    RemoveFromSlot();
    SendToHell();
    Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), GetSecondaryMaterial()->GetTotalExplosivePower());
    return true;
  }

  if(Type & THROW)
  {
    int StrengthValue = GetStrengthValue();

    if(!StrengthValue)
      StrengthValue = 1;

    if(Damage > StrengthValue << 2 && RAND() % (50 * Damage / StrengthValue) >= 100)
    {
      Break(Damager, Dir);
      return true;
    }
  }

  return item::ReceiveDamage(Damager, Damage, Type, Dir);
}

void beartrap::Fly(character* Thrower, int Direction, int Force)
{
  if(!IsStuck())
    item::Fly(Thrower, Direction, Force);
}

void can::DipInto(liquid* Liquid, character* Dipper)
{
  /* Add alchemy */

  if(Dipper->IsPlayer())
    ADD_MESSAGE("%s is now filled with %s.", CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());

  ChangeSecondaryMaterial(Liquid);
  Dipper->DexterityAction(10);
}

truth holybanana::HitEffect(character* Enemy, character* Hitter, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour)
{
  truth BaseSuccess = banana::HitEffect(Enemy, Hitter, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  if(Enemy->IsEnabled() && RAND() & 1)
  {
    if(Enemy->IsPlayer() || Hitter->IsPlayer() || Enemy->CanBeSeenByPlayer() || Hitter->CanBeSeenByPlayer())
      ADD_MESSAGE("%s banana burns %s.", Hitter->CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_DESCRIPTION(DEFINITE));

    return Enemy->ReceiveBodyPartDamage(Hitter, 2 + (RAND() & 3), FIRE, BodyPartIndex, Direction) || BaseSuccess;
  }
  else
    return BaseSuccess;
}

truth holybanana::Zap(character* Zapper, v2, int Direction)
{
  if(Charges > TimesUsed)
  {
    ADD_MESSAGE("BANG! You zap %s!", CHAR_NAME(DEFINITE));
    Zapper->EditExperience(PERCEPTION, 150, 1 << 10);

    beamdata Beam
      (
	Zapper,
	CONST_S("killed by ") + GetName(INDEFINITE),
	Zapper->GetPos(),
	YELLOW,
	BEAM_FIRE_BALL,
	Direction,
	50,
	0
      );

    (GetLevel()->*level::GetBeam(PARTICLE_BEAM))(Beam);
    ++TimesUsed;
  }
  else
    ADD_MESSAGE("Click!");

  return true;
}

void holybanana::AddInventoryEntry(ccharacter* Viewer, festring& Entry, int, truth ShowSpecialInfo) const // never piled
{
  AddName(Entry, INDEFINITE);

  if(ShowSpecialInfo)
  {
    Entry << " [" << GetWeight() << "g, DAM " << GetBaseMinDamage() << '-' << GetBaseMaxDamage();
    Entry << ", " << GetBaseToHitValueDescription();

    if(!IsBroken())
      Entry << ", " << GetStrengthValueDescription();

    int CWeaponSkillLevel = Viewer->GetCWeaponSkillLevel(this);
    int SWeaponSkillLevel = Viewer->GetSWeaponSkillLevel(this);

    if(CWeaponSkillLevel || SWeaponSkillLevel)
      Entry << ", skill " << CWeaponSkillLevel << '/' << SWeaponSkillLevel;

    if(TimesUsed == 1)
      Entry << ", used 1 time";
    else if(TimesUsed)
      Entry << ", used " << TimesUsed << " times";

    Entry << ']';
  }
}

truth holybanana::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(TimesUsed != 6 && Type & (PHYSICAL_DAMAGE|FIRE|ENERGY) && Damage && (Damage > 50 || !(RAND() % (100 / Damage))))
  {
    festring DeathMsg = CONST_S("killed by an explosion of ");
    AddName(DeathMsg, INDEFINITE);

    if(Damager)
      DeathMsg << " caused @bk";

    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    lsquare* Square = GetLSquareUnder();
    RemoveFromSlot();
    SendToHell();
    Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), (6 - TimesUsed) * 50);
    return true;
  }

  return false;
}

void beartrap::PreProcessForBone()
{
  mybase::PreProcessForBone();
  game::RemoveTrapID(TrapData.TrapID);
  TrapData.TrapID = 0;
}

void beartrap::PostProcessForBone()
{
  mybase::PostProcessForBone();
  TrapData.TrapID = game::CreateNewTrapID(this);
}

void itemcontainer::PreProcessForBone()
{
  item::PreProcessForBone();
  Contained->PreProcessForBone();
}

void itemcontainer::PostProcessForBone()
{
  item::PostProcessForBone();
  Contained->PostProcessForBone();
}

void itemcontainer::FinalProcessForBone()
{
  item::FinalProcessForBone();
  Contained->FinalProcessForBone();
}

void magicalwhistle::FinalProcessForBone()
{
  whistle::FinalProcessForBone();
  LastUsed = 0;
}

void horn::FinalProcessForBone()
{
  item::FinalProcessForBone();
  LastUsed = 0;
}

truth charmlyre::Apply(character* Charmer)
{
  if(LastUsed && game::GetTick() - LastUsed < 10000)
  {
    if(Charmer->IsPlayer())
    {
      if(Charmer->CanHear())
	ADD_MESSAGE("You produce a highly alluring sound.");
      else
	ADD_MESSAGE("You try to play something with the %s, but it is hard when you can't hear.", CHAR_NAME(DEFINITE));
    }
    else if(Charmer->CanBeSeenByPlayer())
    {
      if(PLAYER->CanHear())
	ADD_MESSAGE("%s plays %s and produces a highly alluring sound.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("%s plays %s.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }
    else if(PLAYER->CanHear())
      ADD_MESSAGE("You hear a lyre playing.");
  }
  else
  {
    LastUsed = game::GetTick();
    if(Charmer->IsPlayer())
    {
      if(Charmer->CanHear())
	ADD_MESSAGE("You produce a mesmerizing sound.");
      else
	ADD_MESSAGE("You try to play something with the %s, but it is hard when you can't hear.", CHAR_NAME(DEFINITE));
    }
    else if(Charmer->CanBeSeenByPlayer())
    {
      if(PLAYER->CanHear())
	ADD_MESSAGE("%s plays %s and produces a mesmerizing sound.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("%s plays %s.", Charmer->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    }

    for(int d = 0; d < Charmer->GetNeighbourSquares(); ++d)
    {
      square* Square = Charmer->GetNeighbourSquare(d);

      if(Square)
      {
	character* Char = Square->GetCharacter();

	if(Char)
	  if(Char->CanHear())
	  {
	    if(Char->CanTameWithLyre(Charmer))
	    {
	      if(Char->GetTeam() == Charmer->GetTeam())
		ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_NAME(DEFINITE));
	      else if(Char->GetRelation(Charmer) == HOSTILE)
		ADD_MESSAGE("%s stops fighting.", Char->CHAR_NAME(DEFINITE));
	      else
		ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_NAME(DEFINITE));

	      Char->ChangeTeam(Charmer->GetTeam());
	    }
	    else
	      ADD_MESSAGE("%s resists its charming call.", Char->CHAR_NAME(DEFINITE));
	  }
	  else
	    ADD_MESSAGE("%s seems not affected.", Char->CHAR_NAME(DEFINITE));
      }
    }
  }

  Charmer->EditAP(-1000);
  game::CallForAttention(GetPos(), 100);
  return true;
}

void charmlyre::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << LastUsed;
}

void charmlyre::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> LastUsed;
}

charmlyre::charmlyre()
{
  LastUsed = 0;
}

void charmlyre::FinalProcessForBone()
{
  item::FinalProcessForBone();
  LastUsed = 0;
}

truth carrot::BunnyWillCatchAndConsume(ccharacter* Bunny) const
{
  return GetConsumeMaterial(Bunny)->GetConfig() == CARROT_FLESH
						 && !GetConsumeMaterial(Bunny)->GetSpoilLevel();
}

int materialcontainer::GetRustDataB() const
{
  return SecondaryMaterial ? SecondaryMaterial->GetRustData() : GetRustDataA();
}

void backpack::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  if(!Liquid->IsExplosive())
  {
    GetSecondaryMaterial()->AddWetness(Liquid->GetVolume() * 25);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s gets wet.", CHAR_NAME(DEFINITE));
  }

  item::SpillFluid(Spiller, Liquid, SquareIndex);
}

material* materialcontainer::GetConsumeMaterial(ccharacter* Consumer, materialpredicate Predicate) const
{
  if(SecondaryMaterial
     && (SecondaryMaterial->*Predicate)()
     && Consumer->CanConsume(SecondaryMaterial))
    return SecondaryMaterial;
  else
    return item::GetConsumeMaterial(Consumer, Predicate);
}

material* materialcontainer::RemoveMaterial(material* Material)
{
  if(Material == MainMaterial)
    return RemoveMainMaterial();
  else
    return RemoveSecondaryMaterial();
}

material* materialcontainer::RemoveMainMaterial()
{
  truth Equipped = PLAYER->Equips(this);

  if(!SecondaryMaterial)
    RemoveFromSlot();
  else if(SecondaryMaterial->IsLiquid())
  {
    if(!game::IsInWilderness())
    {
      lsquare* Square = GetLSquareUnder();
      RemoveFromSlot();
      Square->SpillFluid(0, static_cast<liquid*>(SecondaryMaterial));
      SetSecondaryMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
    }
    else
      RemoveFromSlot();
  }
  else
  {
    item* Lump = lump::Spawn(0, NO_MATERIALS);
    Lump->InitMaterials(SecondaryMaterial);
    DonateFluidsTo(Lump);
    DonateIDTo(Lump);
    DonateSlotTo(Lump);
    SetSecondaryMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  }

  SendToHell();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));

  return 0;
}

material* materialcontainer::RemoveSecondaryMaterial()
{
  material* Material = SecondaryMaterial;
  SetSecondaryMaterial(0);
  SendNewDrawAndMemorizedUpdateRequest();
  return Material;
}

material* banana::RemoveSecondaryMaterial()
{
  item* Peel = bananapeels::Spawn(0, NO_MATERIALS);
  Peel->InitMaterials(MainMaterial);
  DonateSlotTo(Peel);
  DonateIDTo(Peel);
  SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  SendToHell();
  return 0;
}

material* itemcontainer::RemoveMaterial(material* Material)
{
  Contained->MoveItemsTo(GetSlot());
  return item::RemoveMaterial(Material);
}

void materialcontainer::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  if(MainMaterial)
    game::CombineLights(Emitation, MainMaterial->GetEmitation());

  if(SecondaryMaterial)
    game::CombineLights(Emitation, SecondaryMaterial->GetEmitation());
}

truth materialcontainer::CalculateHasBe() const
{
  return LifeExpectancy
    || (MainMaterial && MainMaterial->HasBe())
    || (SecondaryMaterial && SecondaryMaterial->HasBe());
}

v2 lantern::GetBitmapPos(int Frame) const
{
  return GetSquarePosition() == CENTER
			      ? item::GetBitmapPos(Frame)
			      : item::GetWallBitmapPos(Frame);
}

long materialcontainer::GetMaterialPrice() const
{
  return MainMaterial->GetRawPrice()
    + (SecondaryMaterial ? SecondaryMaterial->GetRawPrice() : 0);
}

void scrollofdetectmaterial::FinishReading(character* Reader)
{
  material* TempMaterial;

  for(;;)
  {
    festring Temp = game::DefaultQuestion(CONST_S("What material do you want to detect?"),
					  game::GetDefaultDetectMaterial());
    TempMaterial = protosystem::CreateMaterial(Temp);

    if(TempMaterial)
      break;
    else
      game::DrawEverythingNoBlit();
  }

  level* Level = GetLevel();
  int Squares = Level->DetectMaterial(TempMaterial);
  RemoveFromSlot();
  SendToHell();

  if(Squares > Reader->GetAttribute(INTELLIGENCE) * (25 + RAND() % 51))
  {
    ADD_MESSAGE("An enormous burst of geographical information overwhelms your consciousness. Your mind cannot cope with it and your memories blur.");
    Level->BlurMemory();
    Reader->BeginTemporaryState(CONFUSED, 1000 + RAND() % 1000);
    Reader->EditExperience(INTELLIGENCE, -100, 1 << 12);
  }
  else if(!Squares)
  {
    ADD_MESSAGE("You feel a sudden urge to imagine the dark void of a starless night sky.");
    Reader->EditExperience(INTELLIGENCE, 200, 1 << 12);
  }
  else
  {
    ADD_MESSAGE("You feel attracted to all things made of %s.", TempMaterial->GetName(false, false).CStr());
    game::PositionQuestion(CONST_S("Detecting material [direction keys move cursor, space exits]"), Reader->GetPos(), 0, 0, false);
    Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
  }

  delete TempMaterial;
  Level->CalculateLuminances();
  game::SendLOSUpdateRequest();
}

int beartrap::GetBaseTrapDamage() const
{
  int Modifier = GetMainMaterial()->GetStrengthValue() / 50;
  Modifier *= Modifier;
  Modifier >>= 1;
  return Modifier ? Modifier + RAND_N(Modifier) : 1;
}

void scrollofhardenmaterial::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("What item do you wish to harden?"), NO_MULTI_SELECT|SELECT_PAIR, &item::CanBeHardened))
    {
      ADD_MESSAGE("You notice you haven't got anything to harden.");
      return;
    }

    if(Item.empty())
      if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
	return;
      else
	continue;

    if(!Item[0]->IsMaterialChangeable())
    {
      ADD_MESSAGE("You cast the spell, but the magic is not powerful enough to affect %s!", Item[0]->CHAR_NAME(DEFINITE|(Item.size() == 1 ? 0 : PLURAL)));
      break;
    }

    if(Item[0]->HandleInPairs() && Item.size() == 1)
    {
      ADD_MESSAGE("Only one %s will be altered.", Item[0]->CHAR_NAME(UNARTICLED));

      if(!game::TruthQuestion(CONST_S("Still continue? [y/N]")))
	continue;
    }

    msgsystem::EnterBigMessageMode();

    if(Item.size() == 1)
      ADD_MESSAGE("Suddenly your %s is consumed in roaring magical flames.", Item[0]->CHAR_NAME(UNARTICLED));
    else
      ADD_MESSAGE("Suddenly your %s are consumed in roaring magical flames.", Item[0]->CHAR_NAME(PLURAL));

    int Config = Item[0]->GetMainMaterial()->GetHardenedMaterial(Item[0]);

    if(!Config)
    {
      /* Should not be possible */

      if(Item.size() == 1)
	ADD_MESSAGE("But it is already as hard as it can get.");
      else
	ADD_MESSAGE("But they are already as hard as they can get.");

      msgsystem::LeaveBigMessageMode();
      break;
    }

    material* TempMaterial = MAKE_MATERIAL(Config);
    int Intelligence = Reader->GetAttribute(INTELLIGENCE);

    if(TempMaterial->GetIntelligenceRequirement() > Intelligence && !game::WizardModeIsActive())
    {
      delete TempMaterial;
      ADD_MESSAGE("But your mind is not yet strong enough to harden %s.", Item.size() == 1 ? "it" : "them");
      msgsystem::LeaveBigMessageMode();
      continue;
    }

    for(int NewConfig = TempMaterial->GetHardenedMaterial(Item[0]), c = 1;
	NewConfig;
	NewConfig = TempMaterial->GetHardenedMaterial(Item[0]), ++c)
    {
      material* NewMaterial = MAKE_MATERIAL(NewConfig);

      if(NewMaterial->GetIntelligenceRequirement()
	 <= Intelligence - c * 5)
      {
	delete TempMaterial;
	TempMaterial = NewMaterial;
      }
      else
	break;
    }

    material* MainMaterial = Item[0]->GetMainMaterial();
    material* SecondaryMaterial = Item[0]->GetSecondaryMaterial();

    if(Item.size() == 1)
    {
      ADD_MESSAGE("As the fire dies out it looks much harder.");

      if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	Item[0]->ChangeSecondaryMaterial(TempMaterial->SpawnMore());

      Item[0]->ChangeMainMaterial(TempMaterial);
    }
    else
    {
      ADD_MESSAGE("As the fire dies out they look much harder.");

      if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
	for(uint c = 0; c < Item.size(); ++c)
	  Item[c]->ChangeSecondaryMaterial(TempMaterial->SpawnMore());

      Item[0]->ChangeMainMaterial(TempMaterial);

      for(uint c = 1; c < Item.size(); ++c)
	Item[c]->ChangeMainMaterial(TempMaterial->SpawnMore());
    }

    msgsystem::LeaveBigMessageMode();
    break;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

void itemcontainer::SetLifeExpectancy(int Base, int RandPlus)
{
  LifeExpectancy = RandPlus > 1 ? Base + RAND_N(RandPlus) : Base;
  Enable();
  Contained->SetLifeExpectancy(Base, RandPlus);
}

ulong wand::GetSpecialParameters() const
{
  switch(GetConfig())
  {
   case WAND_OF_MIRRORING:
    return MIRROR_IMAGE|(1000 << LE_BASE_SHIFT)|(1000 << LE_RAND_SHIFT);
  }

  return 0;
}

void scrollofgolemcreation::FinishReading(character* Reader)
{
  for(;;)
  {
    itemvector Item;

    if(!Reader->SelectFromPossessions(Item, CONST_S("Which item do you wish to use for golem creation?"), NO_MULTI_SELECT, &item::IsDestroyable))
    {
      ADD_MESSAGE("You notice you haven't got anything to change into a golem.");
      return;
    }

    if(!Item.empty())
    {
      material* Main = Item[0]->GetMainMaterial();
      material* Sec = Item[0]->GetSecondaryMaterial();
      truth MainPossible = Main->GetCategoryFlags() & IS_GOLEM_MATERIAL;
      truth SecPossible = Sec && Sec->GetVolume()
			  && Sec->GetCategoryFlags() & IS_GOLEM_MATERIAL
			  && !Sec->IsSameAs(Main);

      if(!MainPossible && !SecPossible)
      {
	ADD_MESSAGE("You can't use that for golem creation.");
	continue;
      }

      if(MainPossible && SecPossible)
	if(game::TruthQuestion(CONST_S("Use main material? [Y/n]"), YES))
	  SecPossible = false;
	else
	  MainPossible = false;

      int MaterialConfig = MainPossible ? Main->GetConfig() : Sec->GetConfig();
      golem* Golem = golem::Spawn(MaterialConfig);
      long Volume = MainPossible ? Sec && Sec->IsSameAs(Main)
		    ? Main->GetVolume() + Sec->GetVolume()
		    : Main->GetVolume() : Sec->GetVolume();
      Golem->SetItemVolume(Volume);
      v2 Where = GetLevel()->GetNearestFreeSquare(Golem, Reader->GetPos());
      Item[0]->RemoveFromSlot();
      Item[0]->SendToHell();

      if(Where == ERROR_V2)
      {
	ADD_MESSAGE("You cast the spell and %s is sucked into a rainbow-coled magical vortex, but nothing happens.", Item[0]->CHAR_NAME(DEFINITE));
	delete Golem;
      }
      else
      {
	ADD_MESSAGE("You cast the spell and %s is sucked into a rainbow-coled magical vortex.", Item[0]->CHAR_NAME(DEFINITE));
	Golem->SetTeam(Reader->GetTeam());
	Golem->PutTo(Where);

	if(Golem->CanBeSeenByPlayer())
	  ADD_MESSAGE("Suddenly %s materializes!", Golem->CHAR_NAME(INDEFINITE));

	Golem->GetLSquareUnder()->DrawParticles(RED);
      }

      break;
    }
    else if(game::TruthQuestion(CONST_S("Really cancel read? [y/N]")))
      return;
  }

  RemoveFromSlot();
  SendToHell();
  Reader->EditExperience(INTELLIGENCE, 300, 1 << 12);
}

void itemcontainer::CalculateEnchantment()
{
  Contained->CalculateEnchantments();
}

int itemcontainer::GetTeleportPriority() const
{
  long Priority = item::GetTeleportPriority();

  for(stackiterator i = Contained->GetBottom(); i.HasItem(); ++i)
    Priority += i->GetTeleportPriority();

  return Priority;
}

void itemcontainer::SetParameters(int Param)
{
  SetIsLocked(Param & LOCKED);
}

truth bananapeels::RaiseTheDead(character*)
{
  GetSlot()->AddFriendItem(banana::Spawn());
  RemoveFromSlot();
  SendToHell();
  return true;
}

void beartrap::RemoveFromSlot()
{
  character* Char = game::SearchCharacter(GetVictimID());

  if(Char)
    Char->RemoveTrap(GetTrapID());

  TrapData.VictimID = 0;
  item::RemoveFromSlot();
}

void beartrap::DonateSlotTo(item* Item)
{
  character* Char = game::SearchCharacter(GetVictimID());

  if(Char)
    Char->RemoveTrap(GetTrapID());

  TrapData.VictimID = 0;
  item::DonateSlotTo(Item);
}

void itemcontainer::Disappear()
{
  Contained->MoveItemsTo(GetSlot());
  item::Disappear();
}

truth gasgrenade::ReceiveDamage(character* Damager, int Damage, int Type, int)
{
  if(Type & THROW || 
     (Type & (PHYSICAL_DAMAGE|FIRE|ENERGY) && Damage && (!(RAND_N(10 / Damage + 1)))))
  {
    if(GetSquareUnder()->CanBeSeenByPlayer(true))
      ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

    material* GasMaterial = GetSecondaryMaterial();
    GetLevel()->GasExplosion(static_cast<gas*>(GasMaterial), GetLSquareUnder());
    RemoveFromSlot();
    SendToHell();
    return true;
  }

  return false;
}

truth holyhandgrenade::Apply(character* Applier) 
{
  if(PinPulledTick)
  {
    ADD_MESSAGE("The pin of %s has already been removed.", CHAR_NAME(DEFINITE));
    return false;
  }

  if(Applier->IsPlayer()) {
    ADD_MESSAGE("You pull the pin off the grenade.");
  }

  PinPulledTick = game::GetTick();
  Count = 0;
  PinPullerID = Applier->GetID();
  Enable();
  UpdatePictures();
  return true;
}

truth holyhandgrenade::CalculateHasBe() const
{
  return PinPulledTick;
}

void holyhandgrenade::Be() {
  item::Be();
  if(3 * (game::GetTick() - PinPulledTick) > Count * 100) 
  {
    ++Count;
    festring Msg = "A voice loudly declares: \"";
    switch(Count)
    {
     case 1:
      Msg << "ONE";
      break;
     case 2:
      Msg << "TWO";
      break;
     case 3:
      Msg << "THREE";
      break;
    }
    Msg << "\".";
    ADD_MESSAGE(Msg.CStr());
    if(Count == 3) {
      Explode();
    }
  }
}

void holyhandgrenade::Explode()
{
  if(game::IsInWilderness()) {
    ADD_MESSAGE("You manage to dispose of %s.", CHAR_NAME(DEFINITE));
    RemoveFromSlot();
    SendToHell();
    return;
  }
  character* Damager = game::SearchCharacter(PinPullerID);
  festring DeathMsg = CONST_S("killed by an explosion of ");
  AddName(DeathMsg, INDEFINITE);

  if(Damager)
    DeathMsg << " caused @bk";

  if(GetSquareUnder()->CanBeSeenByPlayer(true))
    ADD_MESSAGE("%s explodes!", GetExtendedDescription().CStr());

  lsquare* Square = GetLSquareUnder();
  RemoveFromSlot();
  SendToHell();
  Square->GetLevel()->Explosion(Damager, DeathMsg, Square->GetPos(), 300);  
}

v2 holyhandgrenade::GetBitmapPos(int Frame) const
{
  return PinPulledTick ? v2(96, 64) : v2(96, 32);
}

int holyhandgrenade::GetClassAnimationFrames() const 
{ 
  return 32; 
}

alpha holyhandgrenade::GetOutlineAlpha(int Frame) const
{
  if(!PinPulledTick)
    return 0;
  Frame &= 31;
  return 50 + (Frame * (31 - Frame) >> 1);
}

col16 holyhandgrenade::GetOutlineColor(int) const
{ 
  return MakeRGB16(0, 255, 0); 
}

void holyhandgrenade::Save(outputfile& SaveFile) const
{
  item::Save(SaveFile);
  SaveFile << PinPulledTick << Count << PinPullerID;
}

void holyhandgrenade::Load(inputfile& SaveFile)
{
  item::Load(SaveFile);
  SaveFile >> PinPulledTick >> Count >> PinPullerID;
}

void holyhandgrenade::PreProcessForBone()
{
  if(PinPulledTick)
  {
    RemoveFromSlot();
    SendToHell();
  }
}

void holyhandgrenade::PostConstruct()
{
  PinPulledTick = 0;
  Count = 0;
  PinPullerID = 0;
}

col16 holyhandgrenade::GetMaterialColorB(int) const 
{ 
  return MakeRGB16(200, 10, 10); 
}

bool holyhandgrenade::WillExplodeSoon() const
{
  return PinPulledTick != 0;
}

void pantheonbook::FinishReading(character* Reader)
{
  if(Reader->IsPlayer())
  {
    PLAYER->EditExperience(INTELLIGENCE, 75, 1 << 12);
    PLAYER->EditExperience(WISDOM, 1000, 1 << 12);

    ADD_MESSAGE("The book reveals many divine secrets of the pantheon to you and disappears.");
    game::AdjustRelationsToAllGods(75);
    RemoveFromSlot();
    SendToHell();
  }
}

col16 pantheonbook::GetMaterialColorA(int) const
{
  return MakeRGB16(200, 200, 200); 
}

col16 gorovitscopyoflenin::GetMaterialColorB(int) const
{
  return MakeRGB16(30, 30, 30);
}

bool firstbornchild::SpecialOfferEffect(int GodNumber) {
  god* Receiver = game::GetGod(GodNumber);
  Receiver->AdjustRelation(2000); // to the max

  int AmountOfAngelAppears = false;
  for(int c = 0; c < 10; ++c) {
    character* Angel = Receiver->CreateAngel(PLAYER->GetTeam(), 1000); 
    if(Angel) 
    {
      ++AmountOfAngelAppears;
    }
  }

  if(AmountOfAngelAppears == 0) {
    ADD_MESSAGE("You sacrifice %s. %s is very pleased.", 
		CHAR_NAME(DEFINITE), Receiver->GetName());
  } 
  else if(AmountOfAngelAppears == 1) {
    ADD_MESSAGE("You sacrifice %s. %s is very pleased. An angel appears! ", 
		CHAR_NAME(DEFINITE), Receiver->GetName());
  } 
  else {
    ADD_MESSAGE("You sacrifice %s. %s is very pleased. An army of angels appears! ", 
		CHAR_NAME(DEFINITE), Receiver->GetName());
  }

  return true;
}

col16 firstbornchild::GetMaterialColorB(int) const { return MakeRGB16(160, 160, 160); }
