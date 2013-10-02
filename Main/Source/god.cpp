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

/* Compiled through godset.cpp */

godprototype::godprototype(godspawner Spawner, cchar* ClassID) : Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<god>::Add(this); }

god::god() : Relation(0), Timer(0), Known(false), LastPray(-1) { }
int god::GetBasicAlignment() const { return NEUTRAL; }

void god::Pray()
{
  LastPray = 0;
  if(!Timer)
    if(Relation >= -RAND_N(500))
    {
      ADD_MESSAGE("You feel %s is pleased.", GetName());

      if(!TryToAttachBodyPart(PLAYER) && !TryToHardenBodyPart(PLAYER))
	PrayGoodEffect();

      AdjustTimer(5000);
      AdjustRelation(50);
      game::ApplyDivineAlignmentBonuses(this, 10, true);
      PLAYER->EditExperience(WISDOM, 200, 1 << 10);

      if(Relation > 250 && !(RAND() % 20))
      {
	character* Angel = CreateAngel(PLAYER->GetTeam(), 10000);

	if(Angel)
	  ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_DESCRIPTION(DEFINITE));
      }
      else if(Relation > 100 && !(RAND() % 20))
      {
	long Category = RAND() & ANY_CATEGORY;

	if(!Category)
	  Category = ANY_CATEGORY;

	item* Gift = protosystem::BalancedCreateItem(Relation / 2, Relation * 2, Category, 0, 0, GetType());

	if(Gift)
	{
	  Gift->CalculateEnchantment();
	  PLAYER->GetStack()->AddItem(Gift);

	  if(Gift->IsBroken())
	    Gift = Gift->Fix();

	  if(Relation == 1000)
	    Gift->EditEnchantment(3);
	  else if(Relation >= 500)
	    Gift->EditEnchantment(2);
	  else
	    Gift->EditEnchantment(1);

	  ADD_MESSAGE("You notice %s in your inventory which you don't recall picking up anywhere.", Gift->CHAR_NAME(INDEFINITE));
	}
      }
    }
    else
    {
      ADD_MESSAGE("You feel %s is displeased today.", GetName());
      PrayBadEffect();
      AdjustTimer(10000);
      AdjustRelation(-50);
      game::ApplyDivineAlignmentBonuses(this, 10, false);
      PLAYER->EditExperience(WISDOM, -50, 1 << 10);
    }
  else
    if(Relation > RAND_N(500) && Timer < RAND_N(500000))
    {
      ADD_MESSAGE("You feel %s is displeased, but tries to help you anyway.", GetName());

      if(!TryToAttachBodyPart(PLAYER) && !TryToHardenBodyPart(PLAYER))
	PrayGoodEffect();

      AdjustTimer(25000);
      AdjustRelation(-75);
      game::ApplyDivineAlignmentBonuses(this, 15, false);
      PLAYER->EditExperience(WISDOM, -50, 1 << 10);
    }
    else
    {
      ADD_MESSAGE("You feel %s is angry.", GetName());
      PrayBadEffect();
      AdjustTimer(50000);
      AdjustRelation(-100);
      game::ApplyDivineAlignmentBonuses(this, 20, false);
      PLAYER->EditExperience(WISDOM, -100, 1 << 11);

      if(Relation < -250 && !(RAND() % 5))
      {
	character* Angel = CreateAngel(game::GetTeam(4), 10000);

	if(Angel)
	  ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_DESCRIPTION(DEFINITE));
      }
    }
}


festring god::GetCompleteDescription() const
{
  festring Desc(game::GetAlignment(GetAlignment()));
  Desc.Resize(4);
  Desc << GetName();
  Desc.Resize(20);

  if(game::WizardModeIsActive()) {
    Desc << "Timer: " << Timer << " Relation: " << Relation;
    return Desc;
  }
  else
    Desc << "You have ";
  if (LastPray>-1)
  {
    int Hour = LastPray / 2000;
    int Day = Hour / 24;
    Hour %= 24;
    int Min = LastPray % 2000 * 60 / 2000;
    Desc << "last prayed ";
    if (Day>=7)
      Desc << "over a week ago.";
    else
    {
      if (Day>1)
	Desc << Day << " days, ";
      else if (Day)
	Desc << "one day, ";
      if (Hour>1)
	Desc << Hour << " hours, ";
      else if (Hour)
	Desc << "one hour, ";
      if (Day || Hour)
	Desc << "and " << Min << " minutes ago.";
      else
	Desc << Min << " minutes ago.";
    }
  }
  else
    Desc << "never prayed to this god.";
  return Desc;
}


void god::AdjustRelation(god* Competitor, int Multiplier, truth Good)
{
  int Adjustment = (Multiplier << 1) - abs(GetAlignment() - Competitor->GetAlignment()) * Multiplier;

  if(!Good && Adjustment > 0)
    Adjustment = -Adjustment;

  AdjustRelation(Adjustment);
}

void god::AdjustRelation(int Amount)
{
  if(Amount < 0)
    Amount = Amount * 100 / (100 + PLAYER->GetAttribute(WISDOM));
  else
    Amount = Amount * (100 + PLAYER->GetAttribute(WISDOM)) / 100;

  Relation += Amount;

  if(Relation < -1000)
    Relation = -1000;

  if(Relation > 1000)
    Relation = 1000;
}

void god::AdjustTimer(long Amount)
{
  Timer += Amount;

  if(Timer < 0)
    Timer = 0;

  if(Timer > 1000000000)
    Timer = 1000000000;
}

truth god::PlayerVomitedOnAltar(liquid* Liquid)
{
  if(PLAYER->GetVirtualHead())
  {
    ADD_MESSAGE("The vomit drops on the altar, but then suddenly gravity changes its direction. The vomit lands on your face.");
    PLAYER->GetVirtualHead()->SpillFluid(0, Liquid);
    PLAYER->ReceiveDamage(0, 1 + (RAND() & 1), ACID, HEAD);
  }
  else
  {
    ADD_MESSAGE("The vomit drops on the altar, but then suddenly gravity changes its direction. The vomit lands all over you.");
    PLAYER->SpillFluid(0, Liquid);
    PLAYER->ReceiveDamage(0, 1 + (RAND() & 1), ACID);
  }

  AdjustRelation(-200);
  PLAYER->CheckDeath(CONST_S("killed by a flying lump of vomit"), 0);

  if(!(RAND() % 5))
  {
    character* Angel = CreateAngel(game::GetTeam(4), 10000);

    if(Angel)
      ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_DESCRIPTION(DEFINITE));
  }

  return true;
}

character* god::CreateAngel(team* Team, int LifeBase)
{
  v2 TryToCreate;

  for(int c = 0; c < 100; ++c)
  {
    TryToCreate = PLAYER->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

    if(game::GetCurrentArea()->IsValidPos(TryToCreate))
    {
      angel* Angel;

      if(LifeBase && GetType() != VALPURUS
	 && (!(RAND() % 5) || abs(Relation) == 1000))
      {
	Angel = archangel::Spawn(GetType());
	Angel->SetLifeExpectancy(LifeBase / 10, 0);
      }
      else
      {
	Angel = angel::Spawn(GetType());

	if(LifeBase)
	  Angel->SetLifeExpectancy(LifeBase, 0);
      }

      lsquare* Square = game::GetCurrentLevel()->GetLSquare(TryToCreate);

      if(Angel->CanMoveOn(Square) && Angel->IsFreeForMe(Square))
      {
	Angel->SetTeam(Team);
	Angel->SetGenerationDanger(ANGEL_GENERATION_DANGER);
	Angel->PutTo(TryToCreate);
	ADD_MESSAGE("Suddenly %s appears!", Angel->CHAR_DESCRIPTION(INDEFINITE));
	return Angel;
      }
      else
	Angel->SendToHell();
    }
  }

  return 0;
}

void god::PrintRelation() const
{
  cchar* VerbalRelation;

  if(GetRelation() == 1000)
    VerbalRelation = "greets you as a Champion of the Cause!";
  else if(GetRelation() > 750)
    VerbalRelation = "is extremely pleased.";
  else if(GetRelation() > 250)
    VerbalRelation = "is very pleased.";
  else if(GetRelation() > 50)
    VerbalRelation = "is pleased.";
  else if(GetRelation() > -50)
    VerbalRelation = "is content.";
  else if(GetRelation() > -250)
    VerbalRelation = "is angry.";
  else if(GetRelation() > -750)
    VerbalRelation = "is very angry.";
  else if(GetRelation() > -1000)
    VerbalRelation = "is extremely angry.";
  else VerbalRelation = "hates you more than any other mortal.";

  ADD_MESSAGE("%s %s", GetName(), VerbalRelation);
}

truth god::ReceiveOffer(item* Sacrifice)
{
  if(Sacrifice->SpecialOfferEffect(GetType()))
  {
    return true;
  }

  int OfferValue = Sacrifice->GetOfferValue(GetType());

  if(OfferValue)
  {
    if(!Sacrifice->IsDestroyable(PLAYER))
    {
      ADD_MESSAGE("%s is too important for you to be sacrificed.", Sacrifice->CHAR_NAME(DEFINITE));
      return false;
    }

    AdjustRelation(OfferValue);

    if(OfferValue > 0)
      game::ApplyDivineAlignmentBonuses(this, OfferValue / 5, true);
    else
      game::ApplyDivineAlignmentBonuses(this, -OfferValue / 5, false);

    if(OfferValue > 0)
      PLAYER->EditExperience(WISDOM, 150, 1 << 7);
    else
      PLAYER->EditExperience(WISDOM, -100, 1 << 7);

    if(OfferValue > 0)
    {
      if(Sacrifice->GetAttachedGod() == GetType())
	ADD_MESSAGE("%s appreciates your generous offer truly.", GetName());
      else
	ADD_MESSAGE("%s thanks you for your gift.", GetName());
    }
    else
      ADD_MESSAGE("%s seems not to appreciate your gift at all.", GetName());

    PrintRelation();
    int RandModifier = Sacrifice->GetAttachedGod() == GetType() ? 50 : 100;

    if(OfferValue > 0 && Relation > 250 && !(RAND() % RandModifier))
    {
      character* Angel = CreateAngel(PLAYER->GetTeam());

      if(Angel)
	ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_DESCRIPTION(DEFINITE));
    }

    return true;
  }
  else
  {
    ADD_MESSAGE("Nothing happens.");
    return false;
  }
}


god* godprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  god* God = Spawner();
  God->Load(SaveFile);
  return God;
}

truth god::LikesMaterial(const materialdatabase* MDB, ccharacter*) const
{
  return MDB->AttachedGod == GetType();
}

struct materialsorter
{
  materialsorter(citem* Item) : Item(Item) { }
  truth operator()(cmaterial* M1, cmaterial* M2) const
  {
    return M1->GetHardenModifier(Item) > M2->GetHardenModifier(Item);
  }
  citem* Item;
};

truth god::TryToAttachBodyPart(character* Char)
{
  msgsystem::EnterBigMessageMode();

  if(!Char->HasAllBodyParts())
  {
    bodypart* BodyPart = Char->FindRandomOwnBodyPart(true);

    if(BodyPart && LikesMaterial(BodyPart->GetMainMaterial()->GetDataBase(), Char))
    {
      BodyPart->RemoveFromSlot();
      Char->AttachBodyPart(BodyPart);
      ADD_MESSAGE("%s attaches your old %s back.", GetName(), BodyPart->GetBodyPartName().CStr());
    }
    else
    {
      BodyPart = 0;
      materialvector MaterialVector;
      protosystem::CreateEveryMaterial(MaterialVector, this, Char);
      std::sort(MaterialVector.begin(), MaterialVector.end(), materialsorter(0));
      uint c;

      for(c = 0; c < MaterialVector.size(); ++c)
	if(ForceGiveBodyPart()
	   || (MaterialVector[c]->GetCommonFlags() & CAN_BE_WISHED
	       && !RAND_N(6000 / (GetRelation() + 2000))
	       && !RAND_N(Max(MaterialVector[c]->GetIntelligenceRequirement() - 10, 1))))
	{
	  BodyPart = Char->GenerateRandomBodyPart();
	  BodyPart->ChangeMainMaterial(MaterialVector[c]->SpawnMore());
	  Char->UpdatePictures();
	  festring MadeOf;

	  if(!MaterialVector[c]->IsSameAs(Char->GetTorso()->GetMainMaterial()))
	  {
	    MadeOf << " made of ";
	    MaterialVector[c]->AddName(MadeOf, false, false);
	  }

	  ADD_MESSAGE("%s gives you a new %s%s.", GetName(), BodyPart->GetBodyPartName().CStr(), MadeOf.CStr());
	  break;
	}

      for(c = 0; c < MaterialVector.size(); ++c)
	delete MaterialVector[c];
    }

    if(BodyPart)
    {
      if(MutatesBodyParts())
      {
	BodyPart->Mutate();
	ADD_MESSAGE("It seems somehow different.");
      }

      truth Heal = !BodyPart->CanRegenerate() || HealRegeneratingBodyParts();
      BodyPart->SetHP(Heal ? BodyPart->GetMaxHP() : 1);
      msgsystem::LeaveBigMessageMode();
      return true;
    }
  }

  msgsystem::LeaveBigMessageMode();
  return false;
}

truth god::TryToHardenBodyPart(character* Char)
{
  bodypart* PossibleBodyPart[MAX_BODYPARTS];
  uint c, Index = 0;

  for(c = 1; c < uint(Char->GetBodyParts()); ++c) // annoying :(
  {
    bodypart* BodyPart = Char->GetBodyPart(c);

    if(BodyPart && LikesMaterial(BodyPart->GetMainMaterial()->GetDataBase(), Char))
      PossibleBodyPart[Index++] = BodyPart;
  }

  if(!Index)
    return false;

  bodypart* BodyPart = PossibleBodyPart[RAND_N(Index)];
  material* OldMaterial = BodyPart->GetMainMaterial();
  int OldModifier = OldMaterial->GetHardenModifier(BodyPart);
  materialvector MaterialVector;
  protosystem::CreateEveryMaterial(MaterialVector, this, Char);
  std::sort(MaterialVector.begin(), MaterialVector.end(), materialsorter(BodyPart));
  truth Changed = false;

  for(c = 0; c < MaterialVector.size(); ++c)
    if(MaterialVector[c]->GetCommonFlags() & CAN_BE_WISHED)
    {
      material* Material = MaterialVector[c];

      if(Material->GetHardenModifier(BodyPart) > OldModifier
	 && !RAND_N(12000 / (GetRelation() + 2000))
	 && !RAND_N(Max(Material->GetIntelligenceRequirement() - 15, 1)))
      {
	BodyPart->ChangeMainMaterial(Material->SpawnMore());
	ADD_MESSAGE("%s changes your %s to %s.", GetName(), BodyPart->GetBodyPartName().CStr(), Material->GetName(false, false).CStr());
	Changed = true;
	break;
      }
    }

  for(c = 0; c < MaterialVector.size(); ++c)
    delete MaterialVector[c];

  return Changed;
}

cchar* god::GetPersonalPronoun() const
{
  return GetSex() == MALE ? "He" : "She";
}

cchar* god::GetObjectPronoun() const
{
  return GetSex() == MALE ? "Him" : "Her";
}

void god::SignalRandomAltarGeneration(const std::vector<v2>& RoomSquare)
{
  int Times = 1 + femath::LoopRoll(50, 4);

  for(int c = 0; c < Times; ++c)
  {
    long Category = RAND() & ANY_CATEGORY;

    if(!Category)
      Category = ANY_CATEGORY;

    long MaxPrice = 250 + femath::LoopRoll(95, 500) * 10;
    item* Item = protosystem::BalancedCreateItem(0, MaxPrice, Category, 0, 0, GetType());

    if(Item)
    {
      Item->CalculateEnchantment();
      game::GetCurrentLevel()->GetLSquare(RoomSquare[RAND_N(RoomSquare.size())])->AddItem(Item);
    }
  }
}

void god::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  SaveFile << Relation << Timer << Known << LastPray;
}

void god::Load(inputfile& SaveFile)
{
  SaveFile >> Relation >> Timer >> Known >> LastPray;
}


void god::ApplyDivineTick()
{
  if(Timer)
    --Timer;
  if(LastPray > -1 && LastPray < 336000)
    ++LastPray;
} 
