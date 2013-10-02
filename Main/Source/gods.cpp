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

#define LAWFUL_BASIC_COLOR MakeRGB16(160, 160, 0)
#define LAWFUL_ELITE_COLOR MakeRGB16(220, 220, 220)

#define NEUTRAL_BASIC_COLOR MakeRGB16(20, 120, 200)
#define NEUTRAL_ELITE_COLOR MakeRGB16(120, 120, 120)

#define CHAOS_BASIC_COLOR MakeRGB16(200, 0, 0)
#define CHAOS_ELITE_COLOR MakeRGB16(40, 40, 40)

cchar* valpurus::GetName() const { return "Valpurus"; }
cchar* valpurus::GetDescription() const { return "King of Gods"; }
int valpurus::GetAlignment() const { return ALPP; }
int valpurus::GetBasicAlignment() const { return GOOD; }
col16 valpurus::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 valpurus::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* legifer::GetName() const { return "Legifer"; }
cchar* legifer::GetDescription() const { return "god of law and order"; }
int legifer::GetAlignment() const { return ALP; }
int legifer::GetBasicAlignment() const { return GOOD; }
col16 legifer::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 legifer::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* atavus::GetName() const { return "Atavus"; }
cchar* atavus::GetDescription() const { return "god of charity and munificence"; }
int atavus::GetAlignment() const { return ALP; }
int atavus::GetBasicAlignment() const { return GOOD; }
col16 atavus::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 atavus::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* dulcis::GetName() const { return "Dulcis"; }
cchar* dulcis::GetDescription() const { return "goddess of love and art"; }
int dulcis::GetAlignment() const { return AL; }
int dulcis::GetBasicAlignment() const { return GOOD; }
col16 dulcis::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 dulcis::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* seges::GetName() const { return "Seges"; }
cchar* seges::GetDescription() const { return "goddess of health and nutrition"; }
int seges::GetAlignment() const { return AL; }
int seges::GetBasicAlignment() const { return GOOD; }
col16 seges::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 seges::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* sophos::GetName() const { return "Sophos"; }
cchar* sophos::GetDescription() const { return "god of knowledge, magic and handicrafts"; }
int sophos::GetAlignment() const { return ALM; }
int sophos::GetBasicAlignment() const { return GOOD; }
col16 sophos::GetColor() const { return LAWFUL_BASIC_COLOR; }
col16 sophos::GetEliteColor() const { return LAWFUL_ELITE_COLOR; }

cchar* silva::GetName() const { return "Silva"; }
cchar* silva::GetDescription() const { return "goddess of nature"; }
int silva::GetAlignment() const { return ANP; }
int silva::GetBasicAlignment() const { return NEUTRAL; }
col16 silva::GetColor() const { return NEUTRAL_BASIC_COLOR; }
col16 silva::GetEliteColor() const { return NEUTRAL_ELITE_COLOR; }

cchar* loricatus::GetName() const { return "Loricatus"; }
cchar* loricatus::GetDescription() const { return "god of fire, machines and weaponry"; }
int loricatus::GetAlignment() const { return AN; }
int loricatus::GetBasicAlignment() const { return NEUTRAL; }
col16 loricatus::GetColor() const { return NEUTRAL_BASIC_COLOR; }
col16 loricatus::GetEliteColor() const { return NEUTRAL_ELITE_COLOR; }

cchar* mellis::GetName() const { return "Mellis"; }
cchar* mellis::GetDescription() const { return "god of money, trade and politics"; }
int mellis::GetAlignment() const { return ANM; }
int mellis::GetBasicAlignment() const { return NEUTRAL; }
col16 mellis::GetColor() const { return NEUTRAL_BASIC_COLOR; }
col16 mellis::GetEliteColor() const { return NEUTRAL_ELITE_COLOR; }

cchar* cleptia::GetName() const { return "Cleptia"; }
cchar* cleptia::GetDescription() const { return "goddess of assassins and thieves"; }
int cleptia::GetAlignment() const { return ACP; }
int cleptia::GetBasicAlignment() const { return EVIL; }
col16 cleptia::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 cleptia::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

cchar* nefas::GetName() const { return "Nefas"; }
cchar* nefas::GetDescription() const { return "goddess of forbidden pleasures"; }
int nefas::GetAlignment() const { return AC; }
int nefas::GetBasicAlignment() const { return EVIL; }
col16 nefas::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 nefas::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

cchar* scabies::GetName() const { return "Scabies"; }
cchar* scabies::GetDescription() const { return "goddess of mutations, disease and famine"; }
int scabies::GetAlignment() const { return AC; }
int scabies::GetBasicAlignment() const { return EVIL; }
col16 scabies::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 scabies::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

cchar* infuscor::GetName() const { return "Infuscor"; }
cchar* infuscor::GetDescription() const { return "goddess of wrong knowledge and vile magic"; }
int infuscor::GetAlignment() const { return ACM; }
int infuscor::GetBasicAlignment() const { return EVIL; }
col16 infuscor::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 infuscor::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

cchar* cruentus::GetName() const { return "Cruentus"; }
cchar* cruentus::GetDescription() const { return "god of war and blood"; }
int cruentus::GetAlignment() const { return ACM; }
int cruentus::GetBasicAlignment() const { return EVIL; }
col16 cruentus::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 cruentus::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

cchar* mortifer::GetName() const { return "Mortifer"; }
cchar* mortifer::GetDescription() const { return "Destroyer of Worlds"; }
int mortifer::GetAlignment() const { return ACMM; }
int mortifer::GetBasicAlignment() const { return EVIL; }
col16 mortifer::GetColor() const { return CHAOS_BASIC_COLOR; }
col16 mortifer::GetEliteColor() const { return CHAOS_ELITE_COLOR; }

void sophos::PrayGoodEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement! You teleport away!");
  PLAYER->Move(game::GetCurrentLevel()->GetRandomSquare(PLAYER), true);
}

void sophos::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
  PLAYER->TeleportSomePartsAway(1 + (RAND() & 1));
  PLAYER->CheckDeath(CONST_S("shattered to pieces by the wrath of ") + GetName(), 0);
}

void valpurus::PrayGoodEffect()
{
  if(RAND() & 1)
  {
    ADD_MESSAGE("You hear booming voice: \"THIS WILL PROTECT THEE FROM MORTIFER, MY PALADIN!\" A shield glittering with holy might appears from nothing.");
    shield* Shield = shield::Spawn();
    Shield->InitMaterials(MAKE_MATERIAL(VALPURIUM));
    PLAYER->GetGiftStack()->AddItem(Shield);
  }
  else
  {
    ADD_MESSAGE("You hear booming voice: \"DEFEAT MORTIFER WITH THIS, MY PALADIN!\" A sword glittering with holy might appears from nothing.");
    meleeweapon* Weapon = meleeweapon::Spawn(TWO_HANDED_SWORD);
    Weapon->InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), true);
    PLAYER->GetGiftStack()->AddItem(Weapon);
  }
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  PLAYER->ReceiveDamage(0, 10, PHYSICAL_DAMAGE, HEAD, RAND() & 7);
  PLAYER->CheckDeath(CONST_S("faced the hammer of Justice from the hand of ") + GetName(), 0);
}

void legifer::PrayGoodEffect()
{
  ADD_MESSAGE("A booming voice echoes: \"Inlux! Inlux! Save us!\" A huge firestorm engulfs everything around you.");
  game::GetCurrentLevel()->Explosion(PLAYER, CONST_S("killed by the holy fire of ") + GetName(), PLAYER->GetPos(), (300 + Max(GetRelation(), 0)) >> 3, false);
}

void legifer::PrayBadEffect()
{
  ADD_MESSAGE("%s casts a beam of horrible, yet righteous, fire on you.", GetName());
  PLAYER->ReceiveDamage(0, 50 + RAND() % 50, FIRE, ALL);
  PLAYER->CheckDeath(CONST_S("burned to death by the wrath of ") + GetName(), 0);
}

void dulcis::PrayGoodEffect()
{
  ADD_MESSAGE("A beautiful melody echoes around you.");

  for(int d = 0; d < PLAYER->GetNeighbourSquares(); ++d)
  {
    square* Square = PLAYER->GetNeighbourSquare(d);

    if(Square)
    {
      character* Char = Square->GetCharacter();

      if(Char)
	if(Char->CanHear())
	  if(Char->CanTameWithDulcis(PLAYER))
	  {
	    if(Char->GetTeam() == PLAYER->GetTeam())
	      ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_DESCRIPTION(DEFINITE));
	    else if(Char->GetRelation(PLAYER) == HOSTILE)
	      ADD_MESSAGE("%s stops fighting.", Char->CHAR_DESCRIPTION(DEFINITE));
	    else
	      ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_DESCRIPTION(DEFINITE));

	    Char->ChangeTeam(PLAYER->GetTeam());
	  }
	  else
	    ADD_MESSAGE("%s resists its charming call.", Char->CHAR_DESCRIPTION(DEFINITE));
	else
	  ADD_MESSAGE("%s seems not affected.", Char->CHAR_DESCRIPTION(DEFINITE));
    }
  }
}

void dulcis::PrayBadEffect()
{
  ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GetName());
  PLAYER->ReceiveDamage(0, 1 + RAND() % 9, SOUND, HEAD);
  PLAYER->CheckDeath(CONST_S("became insane by listening ") + GetName() + " too much", 0);
}

void seges::PrayGoodEffect()
{
  if(PLAYER->IsInBadCondition())
  {
    ADD_MESSAGE("%s cures your wounds.", GetName());
    PLAYER->RestoreLivingHP();
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(POISONED))
  {
    ADD_MESSAGE("%s removes the foul liquid in your veins.", GetName());
    PLAYER->DeActivateTemporaryState(POISONED);
    return;
  }

  if(PLAYER->StateIsActivated(LEPROSY))
  {
    ADD_MESSAGE("%s cures your leprosy.", GetName());
    PLAYER->DeActivateTemporaryState(LEPROSY);
    return;
  }

  if(PLAYER->GetNP() < SATIATED_LEVEL)
  {
    ADD_MESSAGE("Your stomach feels full again.");
    PLAYER->SetNP(BLOATED_LEVEL);
    return;
  }

  if(PLAYER->GetStamina() < PLAYER->GetMaxStamina() >> 1)
  {
    ADD_MESSAGE("You don't feel a bit tired anymore.");
    PLAYER->RestoreStamina();
    return;
  }
}

void seges::PrayBadEffect()
{
  if(PLAYER->UsesNutrition())
  {
    ADD_MESSAGE("You feel Seges altering the contents of your stomach in an eerie way.");
    PLAYER->EditNP(-10000);
    PLAYER->CheckStarvationDeath(CONST_S("starved by ") + GetName());
  }
  else
    ADD_MESSAGE("Seges tries to alter the contents of your stomach, but fails.");
}

void atavus::PrayGoodEffect()
{
  if(!Timer && Relation > 500 + RAND_N(500))
  {
    item* Reward = bodyarmor::Spawn(PLATE_MAIL, NO_MATERIALS);
    Reward->InitMaterials(MAKE_MATERIAL(ARCANITE));
    ADD_MESSAGE("%s materializes before you.", Reward->CHAR_NAME(INDEFINITE));
    PLAYER->GetGiftStack()->AddItem(Reward);
    AdjustTimer(45000);
    AdjustRelation(-300);
  }
  else
    ADD_MESSAGE("Nothing happens.");
}

void atavus::PrayBadEffect()
{
  ADD_MESSAGE("You have not been good the whole year.");

  if(PLAYER->GetStack()->GetItems())
  {
    int ToBeDeleted = RAND() % PLAYER->GetStack()->GetItems();
    item* Disappearing = PLAYER->GetStack()->GetItem(ToBeDeleted);

    if(Disappearing->IsDestroyable(0))
    {
      ADD_MESSAGE("Your %s disappears.", Disappearing->CHAR_NAME(UNARTICLED));
      Disappearing->RemoveFromSlot();
      Disappearing->SendToHell();
    }
    else
    {
      ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.", GetName(), Disappearing->CHAR_NAME(UNARTICLED));
      PLAYER->EditAttribute(AGILITY, -1);
      PLAYER->EditAttribute(ARM_STRENGTH, -1);
      PLAYER->EditAttribute(ENDURANCE, -1);
    }
  }
  else
  {
    ADD_MESSAGE("You feel you are not so gifted anymore.");
    PLAYER->EditAttribute(AGILITY, -1);
    PLAYER->EditAttribute(ARM_STRENGTH, -1);
    PLAYER->EditAttribute(ENDURANCE, -1);
  }

  PLAYER->CheckDeath(CONST_S("killed by Atavus's humour"));
}

void silva::PrayGoodEffect()
{
  if(PLAYER->GetNP() < HUNGER_LEVEL)
  {
    ADD_MESSAGE("Your stomach feels full again.");
    PLAYER->SetNP(SATIATED_LEVEL);
  }

  if(!game::GetCurrentLevel()->IsOnGround())
  {
    ADD_MESSAGE("Suddenly a horrible earthquake shakes the level.");
    int c, Tunnels = 2 + RAND() % 3;

    for(c = 0; c < Tunnels; ++c)
      game::GetCurrentLevel()->AttachPos(game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE|ATTACHABLE));

    int ToEmpty = 10 + RAND() % 11;

    for(c = 0; c < ToEmpty; ++c)
      for(int i = 0; i < 50; ++i)
      {
	v2 Pos = game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE);
	truth Correct = false;

	for(int d = 0; d < 8; ++d)
	{
	  lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos)->GetNeighbourLSquare(d);

	  if(Square && Square->IsFlyable())
	  {
	    Correct = true;
	    break;
	  }
	}

	if(Correct)
	{
	  game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(0);
	  break;
	}
      }

    int ToGround = 20 + RAND() % 21;

    for(c = 0; c < ToGround; ++c)
      for(int i = 0; i < 50; ++i)
      {
	v2 Pos = game::GetCurrentLevel()->GetRandomSquare(0, RAND() & 1 ? 0 : HAS_CHARACTER);

	if(Pos == ERROR_V2)
	  continue;

	lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
	character* Char = Square->GetCharacter();

	if(Square->GetOLTerrain() || (Char && (Char->IsPlayer() || PLAYER->GetRelation(Char) != HOSTILE)))
	  continue;

	int Walkables = 0;

	for(int d = 0; d < 8; ++d)
	{
	  lsquare* NearSquare = game::GetCurrentLevel()->GetLSquare(Pos)->GetNeighbourLSquare(d);

	  if(NearSquare && NearSquare->IsFlyable())
	    ++Walkables;
	}

	if(Walkables > 6)
	{
	  Square->ChangeOLTerrainAndUpdateLights(earth::Spawn());

	  if(Char)
	  {
	    if(Char->CanBeSeenByPlayer())
	      ADD_MESSAGE("%s is hit by a brick of earth falling from the roof!", Char->CHAR_NAME(DEFINITE));

	    Char->ReceiveDamage(0, 20 + RAND() % 21, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);
	    Char->CheckDeath(CONST_S("killed by an earthquake"), 0);
	  }

	  Square->KickAnyoneStandingHereAway();
	  Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 41, PHYSICAL_DAMAGE);
	  break;
	}
      }

    // Generate a few boulders in the level

    int BoulderNumber = 10 + RAND() % 10;

    for(c = 0; c < BoulderNumber; ++c)
    {
      v2 Pos = game::GetCurrentLevel()->GetRandomSquare();
      lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
      character* MonsterHere = Square->GetCharacter();

      if(!Square->GetOLTerrain() && (!MonsterHere || MonsterHere->GetRelation(PLAYER) == HOSTILE))
      {
	Square->ChangeOLTerrainAndUpdateLights(boulder::Spawn(1 + (RAND() & 1)));

	if(MonsterHere)
	  MonsterHere->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);

	Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE);
      }
    }

    // Damage to items in the level

    for(int x = 0; x < game::GetCurrentLevel()->GetXSize(); ++x)
      for(int y = 0; y < game::GetCurrentLevel()->GetYSize(); ++y)
	game::GetCurrentLevel()->GetLSquare(x, y)->ReceiveEarthQuakeDamage();
  }
  else
  {
    int TryToCreate = 1 + RAND() % 7;
    int Created = 0;

    for(; Created < TryToCreate; ++Created)
    {
      wolf* Wolf = wolf::Spawn();
      v2 Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Wolf, PLAYER->GetPos());

      if(Pos == ERROR_V2)
      {
	delete Wolf;
	break;
      }

      Wolf->SetTeam(PLAYER->GetTeam());
      Wolf->PutTo(Pos);
    }

    if(!Created && PLAYER->CanHear())
      ADD_MESSAGE("You hear a sad howling of a wolf imprisoned in the earth.");

    if(Created == 1)
      ADD_MESSAGE("Suddenly a tame wolf materializes beside you.");

    if(Created > 1)
      ADD_MESSAGE("Suddenly some tame wolves materialize around you.");
  }
}

void silva::PrayBadEffect()
{
  switch(RAND() % 3)
  {
   case 0:
    PLAYER->Polymorph(largerat::Spawn(), 1000 + RAND() % 1000);
    break;
   case 1:
    PLAYER->Polymorph(ass::Spawn(), 1000 + RAND() % 1000);
    break;
   case 2:
    PLAYER->Polymorph(jackal::Spawn(), 1000 + RAND() % 1000);
    break;
  }
}

void loricatus::PrayGoodEffect()
{
  item* MainWielded = PLAYER->GetMainWielded();

  if(MainWielded)
  {
    if(MainWielded->IsMaterialChangeable() && MainWielded->GetMainMaterial()->GetAttachedGod() == GetType())
    {
      int Config = MainWielded->GetMainMaterial()->GetHardenedMaterial(MainWielded);

      if(Config)
      {
	int IR = material::GetDataBase(Config)->IntelligenceRequirement - GetRelation() / 50;

	if(IR <= 1 || !RAND_N(IR))
	{
	  festring Desc;
	  item* SecondaryWielded;

	  if(MainWielded->HandleInPairs() && (SecondaryWielded = PLAYER->GetSecondaryWielded()) && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
	  {
	    MainWielded->AddName(Desc, PLURAL);
	    Desc << " glow and sparkle like they were";

	    if(SecondaryWielded->GetSecondaryMaterial() && SecondaryWielded->GetSecondaryMaterial()->IsSameAs(MainWielded->GetMainMaterial()))
	      SecondaryWielded->ChangeSecondaryMaterial(MAKE_MATERIAL(Config));

	    SecondaryWielded->ChangeMainMaterial(MAKE_MATERIAL(Config));
	  }
	  else
	  {
	    MainWielded->AddName(Desc, UNARTICLED);
	    Desc << " glows and sparkles like it was";
	  }

	  if(MainWielded->GetSecondaryMaterial() && MainWielded->GetSecondaryMaterial()->IsSameAs(MainWielded->GetMainMaterial()))
	    MainWielded->ChangeSecondaryMaterial(MAKE_MATERIAL(Config));

	  MainWielded->ChangeMainMaterial(MAKE_MATERIAL(Config));
	  ADD_MESSAGE("Your %s reforged by invisible hands.", Desc.CStr());
	  return;
	}
      }

      if(!(RAND() % 10))
      {
	item* Scroll = scrollofrepair::Spawn();
	ADD_MESSAGE("%s gives you %s.", GetName(), Scroll->CHAR_NAME(INDEFINITE));
	PLAYER->GetGiftStack()->AddItem(Scroll);
	return;
      }
      else
	ADD_MESSAGE("\"Mortal, thou art always my valiant knight!\"");
    }
  }

  for(int c = 0; c < PLAYER->GetEquipments(); ++c)
  {
    item* Equipment = PLAYER->GetEquipment(c);

    if(Equipment && Equipment->IsBroken())
    {
      ADD_MESSAGE("%s fixes your %s.", GetName(), Equipment->CHAR_NAME(UNARTICLED));
      Equipment->Fix();
      return;
    }
  }

  if(PLAYER->GetUsableArms())
    ADD_MESSAGE("You feel a slight tingling in your hands.");
  else
    ADD_MESSAGE("You feel a slight tingle.");
}

void loricatus::PrayBadEffect()
{
  item* MainWielded = PLAYER->GetMainWielded();

  if(MainWielded)
    if(MainWielded->IsMaterialChangeable())
    {
      festring Desc;
      item* SecondaryWielded;

      if(MainWielded->HandleInPairs() && (SecondaryWielded = PLAYER->GetSecondaryWielded()) && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
      {
	MainWielded->AddName(Desc, PLURAL);
	Desc << " vibrate and soften";
	SecondaryWielded->ChangeMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
      }
      else
      {
	MainWielded->AddName(Desc, UNARTICLED);
	Desc << " vibrates and softens";
      }

      MainWielded->ChangeMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
      ADD_MESSAGE("Your %s.", Desc.CStr());
    }
    else
      ADD_MESSAGE("%s emits strange light but remain unchanged.", MainWielded->CHAR_NAME(DEFINITE));
  else
  {
    if(PLAYER->GetUsableArms())
      ADD_MESSAGE("You feel a slight tingling in your hands.");
    else
      ADD_MESSAGE("You feel a slight tingle.");
  }
}

void cleptia::PrayGoodEffect()
{
  PLAYER->RestoreStamina();

  if(!PLAYER->StateIsActivated(HASTE))
  {
    ADD_MESSAGE("%s gives you the talent for speed.", GetName());
    PLAYER->BeginTemporaryState(HASTE, 2500);
    return;
  }

  if(!PLAYER->StateIsActivated(INVISIBLE))
  {
    ADD_MESSAGE("%s helps you to avoid your enemies by making you invisible.", GetName());
    PLAYER->BeginTemporaryState(INVISIBLE, 2500);
    return;
  }

  ADD_MESSAGE("Cleptia helps you, but you really don't know how.");
  int StateToActivate = RAND() & 1 ? HASTE : INVISIBLE;
  PLAYER->BeginTemporaryState(StateToActivate, 2500);
}

void cleptia::PrayBadEffect()
{
  ADD_MESSAGE("%s slows you down.", GetName());
  PLAYER->BeginTemporaryState(SLOW, 250);
}

void mortifer::PrayGoodEffect()
{
  ADD_MESSAGE("The air vibrates violently around you. A terrible undead voice echoes through the caverns: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHy ReWaRd, ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");
  PLAYER->GetGiftStack()->AddItem(neercseulb::Spawn());
}

void mortifer::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! ThOu ArT nOt WoRtHy! I sHaLl dEsTrOy ThEe LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");
  PLAYER->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);
  PLAYER->EditAttribute(AGILITY, -1);
  PLAYER->EditAttribute(ARM_STRENGTH, -1);
  PLAYER->EditAttribute(ENDURANCE, -1);
  PLAYER->CheckDeath(CONST_S("obliterated by the unholy power of ") + GetName(), 0);
}

void mellis::PrayGoodEffect()
{
  truth Success = false;
  itemvector OKItems;

  for(stackiterator i = PLAYER->GetStack()->GetBottom(); i.HasItem(); ++i)
  {
    if(!i->HasBetterVersion())
      continue;

    OKItems.push_back(*i);
    Success = true;
  }

  item* NewVersion;

  for(int c = 0; !OKItems.empty() && c < 5; ++c)
  {
    item* ToBeDeleted = OKItems[RAND() % OKItems.size()];
    NewVersion = ToBeDeleted->BetterVersion();
    ADD_MESSAGE("%s manages to trade %s into %s.", GetName(), ToBeDeleted->CHAR_NAME(DEFINITE), NewVersion->CHAR_NAME(INDEFINITE));
    PLAYER->GetStack()->AddItem(NewVersion);
    ToBeDeleted->RemoveFromSlot();
    ToBeDeleted->SendToHell();
    OKItems.erase(std::find(OKItems.begin(), OKItems.end(), ToBeDeleted));
  }

  if((Success && !(RAND() % 5)) || (!Success && !(RAND() % 3)))
  {
    int Possible[GODS];
    int PossibleSize = 0;

    for(int c = 1; c <= GODS; ++c)
      if(!game::GetGod(c)->IsKnown())
	Possible[PossibleSize++] = c;

    if(PossibleSize)
    {
      int NewKnownGod = Possible[RAND() % PossibleSize];
      game::LearnAbout(game::GetGod(NewKnownGod));
      ADD_MESSAGE("%s shares his knowledge of %s, the %s.", GetName(), game::GetGod(NewKnownGod)->GetName(), game::GetGod(NewKnownGod)->GetDescription());
      return;
    }
  }

  if(!Success)
    ADD_MESSAGE("Nothing happens.");
}

void mellis::PrayBadEffect()
{
  for(int c = 1; c <= GODS; ++c)
    if(c != GetType())
      game::GetGod(c)->AdjustRelation(-100);

  ADD_MESSAGE("%s spreads bad rumours about you to other gods.", GetName());
}

void valpurus::Pray()
{
  if(!Timer && Relation == 1000)
  {
    ADD_MESSAGE("You feel %s is very pleased.", GetName());
    PrayGoodEffect();
    AdjustTimer(100000);
    AdjustRelation(-500);
    game::ApplyDivineAlignmentBonuses(this, 100, true);
    PLAYER->EditExperience(WISDOM, 400, 1 << 11);

    if(Relation > 250 && !(RAND() % 2))
    {
      character* Angel = CreateAngel(PLAYER->GetTeam());

      if(Angel)
	ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_DESCRIPTION(DEFINITE));
    }
  }
  else if(Relation < 0 || (!TryToAttachBodyPart(PLAYER) && !TryToHardenBodyPart(PLAYER)))
  {
    ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GetName());
    PrayBadEffect();
    AdjustTimer(50000);
    AdjustRelation(-100);
    game::ApplyDivineAlignmentBonuses(this, 20, false);
    PLAYER->EditExperience(WISDOM, -50, 1 << 10);

    if(Relation < -250 && !(RAND() % 3))
    {
      character* Angel = CreateAngel(game::GetTeam(4), 10000);

      if(Angel)
	ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_DESCRIPTION(DEFINITE));
    }
  }
}

void mortifer::Pray()
{
  if(!Timer && Relation == 1000)
  {
    ADD_MESSAGE("You feel %s is very pleased.", GetName());
    PrayGoodEffect();
    AdjustTimer(100000);
    AdjustRelation(-500);
    game::ApplyDivineAlignmentBonuses(this, 100, true);
    PLAYER->EditExperience(WISDOM, 400, 1 << 11);

    if(Relation > 250 && !(RAND() % 2))
    {
      character* Angel = CreateAngel(PLAYER->GetTeam());

      if(Angel)
	ADD_MESSAGE("%s seems to be very friendly towards you.", Angel->CHAR_DESCRIPTION(DEFINITE));
    }
  }
  else
  {
    ADD_MESSAGE("You feel you are not yet worthy enough for %s.", GetName());
    PrayBadEffect();
    AdjustTimer(50000);
    AdjustRelation(-100);
    game::ApplyDivineAlignmentBonuses(this, 20, false);
    PLAYER->EditExperience(WISDOM, -50, 1 << 10);

    if(Relation < -250 && !(RAND() % 3))
    {
      character* Angel = CreateAngel(game::GetTeam(4), 10000);

      if(Angel)
	ADD_MESSAGE("%s seems to be hostile.", Angel->CHAR_DESCRIPTION(DEFINITE));
    }
  }
}

void infuscor::PrayBadEffect()
{
  ADD_MESSAGE("Vile and evil knowledge pulps into your brain. It's too much for it to handle; you faint.");
  PLAYER->LoseConsciousness(1000 + RAND_N(1000));
}

void nefas::PrayGoodEffect()
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  truth AudiencePresent = false;

  for(int x = Rect.X1; x <= Rect.X2; ++x)
  {
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

      if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(CONFUSED) && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
      {
	AudiencePresent = true;
	break;
      }
    }

    if(AudiencePresent)
      break;
  }

  if(AudiencePresent)
  {
    for(int x = Rect.X1; x <= Rect.X2; ++x)
      for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	if(Audience && !Audience->TemporaryStateIsActivated(CONFUSED) && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
	{
	  if(Audience->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s confuses %s with her sweet lies.", GetName(), Audience->CHAR_NAME(DEFINITE));

	  Audience->BeginTemporaryState(CONFUSED, 500 + RAND() % 500);
	}
      }
  }

  mistress* Mistress = mistress::Spawn(RAND() & 7 ? 0 : TORTURING_CHIEF);
  v2 Where = game::GetCurrentLevel()->GetNearestFreeSquare(Mistress, PLAYER->GetPos());

  if(Where == ERROR_V2)
  {
    if(PLAYER->CanHear())
      ADD_MESSAGE("You hear a strange scream from somewhere beneath.");
    else
      ADD_MESSAGE("You feel the air vibrating.");

    delete Mistress;
  }
  else
  {
    Mistress->SetTeam(PLAYER->GetTeam());
    Mistress->PutTo(Where);
    ADD_MESSAGE("You hear a sweet voice inside your head: \"Have fun, mortal!\"");
  }
}

void nefas::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  PLAYER->ReceiveDamage(0, 2 + RAND() % 7, PHYSICAL_DAMAGE, HEAD);
  PLAYER->GetStackUnder()->AddItem(brokenbottle::Spawn());
  PLAYER->CheckDeath(CONST_S("killed while enjoying the company of ") + GetName(), 0);
}

void scabies::PrayGoodEffect()
{
  if(!RAND_N(10))
  {
    for(int c = 0; c < game::GetTeams(); ++c)
      if(PLAYER->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
	for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	{
	  character* Char = *i;

	  if((*i)->IsEnabled() && !Char->IsImmuneToLeprosy())
	    Char->GainIntrinsic(LEPROSY);
	}

    ADD_MESSAGE("You feel a a horrible disease spreading.");
    return;
  }

  if(!(RAND() % 50))
  {
    ADD_MESSAGE("Five cans full of school food drop from somewhere above!");

    for(int c = 0; c < 5; ++c)
    {
      can* Reward = can::Spawn(0, NO_MATERIALS);
      Reward->InitMaterials(MAKE_MATERIAL(IRON), MAKE_MATERIAL(SCHOOL_FOOD));
      PLAYER->GetGiftStack()->AddItem(Reward);
    }

    return;
  }

  truth Success = false;

  for(int d = 0; d < PLAYER->GetNeighbourSquares(); ++d)
  {
    lsquare* Square = PLAYER->GetNeighbourLSquare(d);

    if(Square && Square->GetCharacter() && Square->GetCharacter()->GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("%s throws poison on %s!", GetName(), Square->GetCharacter()->CHAR_DESCRIPTION(DEFINITE));
      Square->SpillFluid(PLAYER, liquid::Spawn(POISON_LIQUID, 500));
      Success = true;
    }
  }

  if(!Success)
    PLAYER->PolymorphRandomly(2500, 10000, 1000 + RAND() % 1000);
}

void scabies::PrayBadEffect()
{
  if(!(RAND() % 50))
  {
    ADD_MESSAGE("%s makes you eat a LOT of school food.", GetName());
    material* SchoolFood = MAKE_MATERIAL(SCHOOL_FOOD, 2000);
    SchoolFood->EatEffect(PLAYER, 1000);
    delete SchoolFood;
    ADD_MESSAGE("You feel your muscles softening terribly...");
    PLAYER->EditAttribute(ARM_STRENGTH, -1);
    PLAYER->EditAttribute(DEXTERITY, -1);
  }
  else if(RAND_2)
  {
    ADD_MESSAGE("%s unleashes all her fury upon you!", GetName());
    PLAYER->BeginTemporaryState(POISONED, 600 + RAND() % 400);
  }
  else
  {
    ADD_MESSAGE("%s unleashes a horrible sickness upon you!", GetName());
    PLAYER->GainIntrinsic(LEPROSY);
  }
}

void infuscor::PrayGoodEffect()
{
  ADD_MESSAGE("%s helps you.", GetName());

  if(!PLAYER->StateIsActivated(ESP))
  {
    PLAYER->BeginTemporaryState(ESP, 10000 + RAND() % 10000);
    return;
  }

  if(!PLAYER->StateIsActivated(TELEPORT_CONTROL))
  {
    PLAYER->BeginTemporaryState(TELEPORT_CONTROL, 10000 + RAND() % 10000);
    return;
  }

  if(!PLAYER->StateIsActivated(POLYMORPH_CONTROL))
  {
    PLAYER->BeginTemporaryState(POLYMORPH_CONTROL, 10000 + RAND() % 10000);
    return;
  }

  ADD_MESSAGE("Suddenly three scrolls appear almost under your feet.");

  for(int c = 0; c < 3; ++c)
    PLAYER->GetGiftStack()->AddItem(scrollofteleportation::Spawn());
}

void cruentus::PrayGoodEffect()
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  truth AudiencePresent = false;

  for(int x = Rect.X1; x <= Rect.X2; ++x)
  {
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

      if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(PANIC) && PLAYER->GetRelation(Audience) == HOSTILE)
      {
	AudiencePresent = true;
	break;
      }
    }

    if(AudiencePresent)
      break;
  }

  if(AudiencePresent)
  {
    ADD_MESSAGE("The thundering voice of a godly battle drum shakes everything around you.");

    for(int x = Rect.X1; x <= Rect.X2; ++x)
      for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

	if(Audience
	   && !Audience->TemporaryStateIsActivated(PANIC)
	   && PLAYER->GetRelation(Audience) == HOSTILE
	   && Audience->GetPanicLevel() > RAND() % 33)
	  Audience->BeginTemporaryState(PANIC, 500 + RAND() % 500);
      }

    return;
  }

  item* Weapon = PLAYER->GetMainWielded();

  if(!Weapon || !Weapon->IsWeapon(PLAYER))
    Weapon = PLAYER->GetSecondaryWielded();

  if(Weapon && Weapon->IsWeapon(PLAYER) && Weapon->CanBeEnchanted()
     && Weapon->GetEnchantment() < 5 && !(RAND() % 10))
  {
    ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Weapon->CHAR_NAME(UNARTICLED));
    Weapon->EditEnchantment(1);
  }
  else if(RAND() & 3)
  {
    potion* Bottle = potion::Spawn(0, NO_MATERIALS);
    Bottle->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(TROLL_BLOOD));
    PLAYER->GetGiftStack()->AddItem(Bottle);
    ADD_MESSAGE("%s drops from nowhere.", Bottle->CHAR_DESCRIPTION(DEFINITE));
  }
  else
  {
    ADD_MESSAGE("Cruentus recommends you to his master, Mortifer.");
    game::GetGod(MORTIFER)->AdjustRelation(100);
  }
}

void cruentus::PrayBadEffect()
{
  item* ToBe = PLAYER->GetMainWielded();

  if(ToBe)
  {
    if(!ToBe->IsDestroyable(0))
    {
      ToBe = PLAYER->GetSecondaryWielded();

      if(!ToBe || !ToBe->IsDestroyable(0))
	ADD_MESSAGE("%s tries to destroy your %s, but fails.", GetName(), PLAYER->GetMainWielded()->CHAR_NAME(UNARTICLED));
    }
  }
  else
  {
    ToBe = PLAYER->GetSecondaryWielded();

    if(ToBe && !ToBe->IsDestroyable(0))
      ADD_MESSAGE("%s tries to destroy your %s, but fails.", GetName(), ToBe->CHAR_NAME(UNARTICLED));
  }

  if(ToBe && ToBe->IsDestroyable(0))
  {
    ADD_MESSAGE("%s destroys your weapon.", GetName());
    ToBe->RemoveFromSlot();
    ToBe->SendToHell();
  }
  else
  {
    ADD_MESSAGE("%s gets mad and hits you!", GetName());
    PLAYER->ReceiveDamage(0, 1 + RAND() % 30, PHYSICAL_DAMAGE, ALL, RAND() & 7);
    PLAYER->CheckDeath(CONST_S("destroyed by ") + GetName(), 0);
  }
}

truth scabies::PlayerVomitedOnAltar(liquid*)
{
  ADD_MESSAGE("%s feels that you are indeed her follower.", GetName());
  AdjustRelation(1);
  return false;
}

truth atavus::LikesMaterial(const materialdatabase* MDB, ccharacter* Char) const
{
  return Char->GetTorso()->GetMainMaterial()->GetConfig() == MDB->Config;
}

truth seges::LikesMaterial(const materialdatabase* MDB, ccharacter* Char) const
{
  return MDB->BodyFlags & IS_ALIVE
    && Char->GetTorso()->GetMainMaterial()->GetConfig() == MDB->Config;
}

truth scabies::LikesMaterial(const materialdatabase* MDB, ccharacter* Char) const
{
  return MDB->BodyFlags & IS_ALIVE
    && Char->GetTorso()->GetMainMaterial()->GetConfig() == MDB->Config;
}
