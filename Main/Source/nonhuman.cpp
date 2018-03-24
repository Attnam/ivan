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

/* Compiled through charsset.cpp */

int nonhumanoid::GetUnarmedMinDamage() const { return int(UnarmedDamage * 0.75); }
int nonhumanoid::GetUnarmedMaxDamage() const { return int(UnarmedDamage * 1.25 + 1); }
int nonhumanoid::GetKickMinDamage() const { return int(KickDamage * 0.75); }
int nonhumanoid::GetKickMaxDamage() const { return int(KickDamage * 1.25 + 1); }
int nonhumanoid::GetBiteMinDamage() const { return int(BiteDamage * 0.75); }
int nonhumanoid::GetBiteMaxDamage() const { return int(BiteDamage * 1.25 + 1); }
int nonhumanoid::GetCarryingStrength() const { return (Max(GetAttribute(LEG_STRENGTH), 1) << 1) + CarryingBonus; }
truth nonhumanoid::UseMaterialAttributes() const { return GetTorso()->UseMaterialAttributes(); }

truth elpuri::SpecialEnemySightedReaction(character*) { return !(Active = true); }

cchar* billswill::FirstPersonBiteVerb() const { return "emit psi waves at"; }
cchar* billswill::FirstPersonCriticalBiteVerb() const { return "emit powerful psi waves at"; }
cchar* billswill::ThirdPersonBiteVerb() const { return "emits psi waves at"; }
cchar* billswill::ThirdPersonCriticalBiteVerb() const { return "emits powerful psi waves at"; }
int billswill::GetBodyPartWobbleData(int) const { return WOBBLE_HORIZONTALLY|(2 << WOBBLE_FREQ_SHIFT); }

int mommo::GetBodyPartWobbleData(int) const
{ return (GetConfig() == CONICAL ? WOBBLE_HORIZONTALLY : WOBBLE_VERTICALLY)|(2 << WOBBLE_FREQ_SHIFT); }

bodypart* dog::MakeBodyPart(int) const { return dogtorso::Spawn(0, NO_MATERIALS); }

bodypart* spider::MakeBodyPart(int) const { return spidertorso::Spawn(0, NO_MATERIALS); }

int dolphin::GetSpecialBodyPartFlags(int) const { return RAND() & (MIRROR|ROTATE); }

bodypart* bat::MakeBodyPart(int) const { return battorso::Spawn(0, NO_MATERIALS); }

col16 chameleon::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

void floatingeye::SetWayPoints(const fearray<packv2>& What) { ArrayToVector(What, WayPoints); }

bodypart* eddy::MakeBodyPart(int) const { return eddytorso::Spawn(0, NO_MATERIALS); }
int eddy::GetBodyPartWobbleData(int) const { return WOBBLE_VERTICALLY|(2 << WOBBLE_FREQ_SHIFT); }

bodypart* magicmushroom::MakeBodyPart(int) const { return magicmushroomtorso::Spawn(0, NO_MATERIALS); }

cchar* magpie::FirstPersonBiteVerb() const { return "peck"; }
cchar* magpie::FirstPersonCriticalBiteVerb() const { return "critically peck"; }
cchar* magpie::ThirdPersonBiteVerb() const { return "pecks"; }
cchar* magpie::ThirdPersonCriticalBiteVerb() const { return "critically pecks"; }

bodypart* largecreature::MakeBodyPart(int) const { return largetorso::Spawn(0, NO_MATERIALS); }
lsquare* largecreature::GetNeighbourLSquare(int I) const { return static_cast<lsquare*>(GetNeighbourSquare(I)); }
wsquare* largecreature::GetNeighbourWSquare(int I) const { return static_cast<wsquare*>(GetNeighbourSquare(I)); }

int hattifattener::GetSpecialBodyPartFlags(int) const { return ST_LIGHTNING; }
int hattifattener::GetBodyPartWobbleData(int) const
{ return WOBBLE_HORIZONTALLY|(1 << WOBBLE_SPEED_SHIFT)|(1 << WOBBLE_FREQ_SHIFT); }

col16 vladimir::GetSkinColor() const { return MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190); }

bodypart* blinkdog::MakeBodyPart(int) const { return blinkdogtorso::Spawn(0, NO_MATERIALS); }

int mysticfrog::GetBodyPartWobbleData(int) const
{ return WOBBLE_HORIZONTALLY|(1 << WOBBLE_SPEED_SHIFT)|(3 << WOBBLE_FREQ_SHIFT); }
bodypart* mysticfrog::MakeBodyPart(int) const { return mysticfrogtorso::Spawn(0, NO_MATERIALS); }

bodypart* lobhse::MakeBodyPart(int) const { return lobhsetorso::Spawn(0, NO_MATERIALS); }

truth elpuri::Hit(character* Enemy, v2, int, int Flags)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  character* EnemyHit[MAX_NEIGHBOUR_SQUARES];
  int EnemiesHit = 0;

  for(int d = 0; d < GetExtendedNeighbourSquares(); ++d)
    if(IsEnabled())
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
      {
        character* ByStander = Square->GetCharacter();

        if(ByStander && (ByStander == Enemy || GetRelation(ByStander) == HOSTILE))
        {
          truth Abort = false;

          for(int c = 0; c < EnemiesHit; ++c)
            if(EnemyHit[c] == ByStander)
              Abort = true;

          if(!Abort)
          {
            nonhumanoid::Hit(ByStander, Square->GetPos(), YOURSELF, Flags);
            ByStander->DamageAllItems(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE);
            EnemyHit[EnemiesHit++] = ByStander;
          }
        }

        Square->GetStack()->ReceiveDamage(this, RAND() % 36 + RAND() % 36, PHYSICAL_DAMAGE,
                                          game::GetLargeMoveDirection(d));
      }
    }

  EditAP(-500);
  return true;
}

truth dog::Catches(item* Thingy)
{
  if(Thingy->DogWillCatchAndConsume(this))
  {
    if(ConsumeItem(Thingy, CONST_S("eating")))
    {
      if(IsPlayer())
        ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
      else
      {
        if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

        ChangeTeam(PLAYER->GetTeam());
      }
    }
    else if(IsPlayer())
      ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

    return true;
  }
  else
    return false;
}

truth unicorn::SpecialEnemySightedReaction(character*)
{
  if(!(RAND() & 15))
  {
    MonsterTeleport("neighs happily");
    return true;
  }

  if(StateIsActivated(PANIC) || (RAND() & 1 && IsInBadCondition()))
  {
    MonsterTeleport("neighs");
    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void nonhumanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << StrengthExperience << AgilityExperience;
}

void nonhumanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> StrengthExperience >> AgilityExperience;
}

void nonhumanoid::CalculateUnarmedDamage()
{
  UnarmedDamage = sqrt(5e-12 * GetAttribute(ARM_STRENGTH))
                  * GetBaseUnarmedStrength() * GetCWeaponSkill(UNARMED)->GetBonus();
}

void nonhumanoid::CalculateUnarmedToHitValue()
{
  UnarmedToHitValue = GetAttribute(DEXTERITY) * sqrt(2.5 * GetAttribute(PERCEPTION))
                      * GetCWeaponSkill(UNARMED)->GetBonus() * GetMoveEase() / 500000;
}

void nonhumanoid::CalculateUnarmedAPCost()
{
  UnarmedAPCost = Max(long(10000000000. / (APBonus(GetAttribute(DEXTERITY)) * GetMoveEase()
                                           * GetCWeaponSkill(UNARMED)->GetBonus())), 100L);
}

void nonhumanoid::CalculateKickDamage()
{
  KickDamage = sqrt(5e-12 * GetAttribute(LEG_STRENGTH))
               * GetBaseKickStrength() * GetCWeaponSkill(KICK)->GetBonus();
}

void nonhumanoid::CalculateKickToHitValue()
{
  KickToHitValue = GetAttribute(AGILITY) * sqrt(2.5 * GetAttribute(PERCEPTION))
                   * GetCWeaponSkill(KICK)->GetBonus() * GetMoveEase() / 1000000;
}

void nonhumanoid::CalculateKickAPCost()
{
  KickAPCost = Max(long(20000000000. / (APBonus(GetAttribute(AGILITY))
                                        * GetMoveEase() * GetCWeaponSkill(KICK)->GetBonus())), 1000L);
}

void nonhumanoid::CalculateBiteDamage()
{
  BiteDamage = sqrt(5e-12 * GetAttribute(ARM_STRENGTH))
               * GetBaseBiteStrength() * GetCWeaponSkill(BITE)->GetBonus();
}

void nonhumanoid::CalculateBiteToHitValue()
{
  BiteToHitValue = GetAttribute(AGILITY) * sqrt(2.5 * GetAttribute(PERCEPTION))
                   * GetCWeaponSkill(BITE)->GetBonus() * GetMoveEase() / 1000000;
}

void nonhumanoid::CalculateBiteAPCost()
{
  BiteAPCost = Max(long(10000000000. / (APBonus(GetAttribute(DEXTERITY))
                                        * GetMoveEase() * GetCWeaponSkill(BITE)->GetBonus())), 100L);
}

void nonhumanoid::InitSpecialAttributes()
{
  StrengthExperience = GetNaturalExperience(ARM_STRENGTH);
  AgilityExperience = GetNaturalExperience(AGILITY);
  LimitRef(StrengthExperience, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience, MIN_EXP, MAX_EXP);
}

void nonhumanoid::Bite(character* Enemy, v2 HitPos, int Direction, truth ForceHit)
{
  EditNP(-50);
  EditAP(-GetBiteAPCost());
  EditExperience(ARM_STRENGTH, 75, 1 << 8);
  EditExperience(AGILITY, 150, 1 << 8);
  EditStamina(-10000 / GetAttribute(ARM_STRENGTH), false);
  Enemy->TakeHit(this, 0, GetTorso(), HitPos, GetBiteDamage(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26,
                 BITE_ATTACK, Direction, !(RAND() % GetCriticalModifier()), ForceHit);
}

void nonhumanoid::Kick(lsquare* Square, int Direction, truth ForceHit)
{
  EditNP(-50);
  EditAP(-GetKickAPCost());
  EditStamina(-10000 / GetAttribute(ARM_STRENGTH), false);

  if(Square->BeKicked(this, 0, GetTorso(), GetKickDamage(), GetKickToHitValue(), RAND() % 26 - RAND() % 26,
                      Direction, !(RAND() % GetCriticalModifier()), ForceHit))
  {
    EditExperience(LEG_STRENGTH, 150, 1 << 8);
    EditExperience(AGILITY, 75, 1 << 8);
  }
}

truth nonhumanoid::Hit(character* Enemy, v2 HitPos, int Direction, int Flags)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(IsPlayer())
  {
    if(!(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND) && GetRelation(Enemy) != HOSTILE
       && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
  }
  else if(GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  if(GetBurdenState() == OVER_LOADED)
  {
    if(IsPlayer())
      ADD_MESSAGE("You cannot fight while carrying so much.");

    return false;
  }

  /* Behold this Terrible Father of Gum Solutions! */

  int AttackStyle = GetAttackStyle();

  if(AttackStyle & USE_LEGS)
  {
    room* Room = GetNearLSquare(HitPos)->GetRoom();

    if(Room && !Room->AllowKick(this, GetNearLSquare(HitPos)))
      AttackStyle &= ~USE_LEGS;
  }

  int c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(AttackStyle & (1 << c))
      ++AttackStyles;

  int Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(AttackStyle & (1 << c) && AttackStyles++ == Chosen)
    {
      Chosen = 1 << c;
      break;
    }

  switch(Chosen)
  {
   case USE_ARMS:
    msgsystem::EnterBigMessageMode();
    Hostility(Enemy);
    UnarmedHit(Enemy, HitPos, Direction, Flags & SADIST_HIT);
    msgsystem::LeaveBigMessageMode();
    return true;
   case USE_LEGS:
    msgsystem::EnterBigMessageMode();
    Hostility(Enemy);
    Kick(GetNearLSquare(HitPos), Direction, Flags & SADIST_HIT);
    msgsystem::LeaveBigMessageMode();
    return true;
   case USE_HEAD:
    msgsystem::EnterBigMessageMode();
    Hostility(Enemy);
    Bite(Enemy, HitPos, Direction, Flags & SADIST_HIT);
    msgsystem::LeaveBigMessageMode();
    return true;
   default:
    ABORT("Strange alien attack style requested!");
    return false;
  }
}

void nonhumanoid::UnarmedHit(character* Enemy, v2 HitPos, int Direction, truth ForceHit)
{
  EditNP(-50);
  EditAP(-GetUnarmedAPCost());
  EditStamina(-10000 / GetAttribute(ARM_STRENGTH), false);

  switch(Enemy->TakeHit(this, 0, GetTorso(), HitPos, GetUnarmedDamage(), GetUnarmedToHitValue(),
                        RAND() % 26 - RAND() % 26, UNARMED_ATTACK, Direction,
                        !(RAND() % GetCriticalModifier()), ForceHit))
  {
   case HAS_HIT:
   case HAS_BLOCKED:
   case HAS_DIED:
   case DID_NO_DAMAGE:
    EditExperience(ARM_STRENGTH, 150, 1 << 8);
   case HAS_DODGED:
    EditExperience(DEXTERITY, 75, 1 << 8);
  }
}

/* Returns the average number of APs required to kill Enemy */

double nonhumanoid::GetTimeToKill(ccharacter* Enemy, truth UseMaxHP) const
{
  double Effectivity = 0;
  int AttackStyles = 0;

  if(IsUsingArms())
  {
    Effectivity += 1 / (Enemy->GetTimeToDie(this, int(GetUnarmedDamage()) + 1, GetUnarmedToHitValue(),
                                            AttackIsBlockable(UNARMED_ATTACK), UseMaxHP) * GetUnarmedAPCost());
    ++AttackStyles;
  }

  if(IsUsingLegs())
  {
    Effectivity += 1 / (Enemy->GetTimeToDie(this, int(GetKickDamage()) + 1, GetKickToHitValue(),
                                            AttackIsBlockable(KICK_ATTACK), UseMaxHP) * GetKickAPCost());
    ++AttackStyles;
  }

  if(IsUsingHead())
  {
    Effectivity += 1 / (Enemy->GetTimeToDie(this, int(GetBiteDamage()) + 1, GetBiteToHitValue(),
                                            AttackIsBlockable(BITE_ATTACK), UseMaxHP) * GetBiteAPCost());
    ++AttackStyles;
  }

  if(StateIsActivated(HASTE))
    Effectivity *= 2;

  if(StateIsActivated(SLOW))
    Effectivity /= 2;

  return AttackStyles / Effectivity;
}

int nonhumanoid::GetAttribute(int Identifier, truth AllowBonus) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier, AllowBonus);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
  {
    if(!UseMaterialAttributes())
      return int(StrengthExperience * EXP_DIVISOR);
    else
      return GetTorso()->GetMainMaterial()->GetStrengthValue();
  }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
  {
    if(!UseMaterialAttributes())
      return int(AgilityExperience * EXP_DIVISOR);
    else
      return (GetTorso()->GetMainMaterial()->GetFlexibility() << 2);
  }
  else
  {
    ABORT("Illegal nonhumanoid attribute %d request!", Identifier);
    return 0xABBE;
  }
}

truth nonhumanoid::EditAttribute(int Identifier, int Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    return !UseMaterialAttributes() && RawEditAttribute(StrengthExperience, Value);
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    return !UseMaterialAttributes() && RawEditAttribute(AgilityExperience, Value);
  else
  {
    ABORT("Illegal nonhumanoid attribute %d edit request!", Identifier);
    return false;
  }
}

void nonhumanoid::EditExperience(int Identifier, double Value, double Speed)
{
  if(!AllowExperience())
    return;

  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value, Speed);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
  {
    if(!UseMaterialAttributes())
    {
      int Change = RawEditExperience(StrengthExperience,
                                     GetNaturalExperience(ARM_STRENGTH),
                                     Value, Speed / 2);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "stronger" : "weaker";

        if(IsPlayer())
          ADD_MESSAGE("Your feel %s!", Adj);
        else if(IsPet() && CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", CHAR_NAME(DEFINITE), Adj);

        CalculateBurdenState();
        CalculateBattleInfo();
      }
    }
  }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
  {
    if(!UseMaterialAttributes())
    {
      int Change = RawEditExperience(AgilityExperience,
                                     GetNaturalExperience(AGILITY),
                                     Value, Speed / 2);

      if(Change)
      {
        cchar* Adj = Change > 0 ? "very agile" : "sluggish";

        if(IsPlayer())
          ADD_MESSAGE("Your feel %s!", Adj);
        else if(IsPet() && CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s looks %s.", CHAR_NAME(DEFINITE), Adj);

        CalculateBattleInfo();
      }
    }
  }
  else
    ABORT("Illegal nonhumanoid attribute %d experience edit request!", Identifier);
}

int nonhumanoid::DrawStats(truth AnimationDraw) const
{
  if(AnimationDraw)
    return 3;

  int PanelPosX = RES.X - 96, PanelPosY = 3;
  PrintAttribute("Str", ARM_STRENGTH, PanelPosX, PanelPosY++);
  PrintAttribute("Agi", AGILITY, PanelPosX, PanelPosY++);
  return PanelPosY;
}

void nonhumanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();
  CalculateUnarmedAttackInfo();
  CalculateKickAttackInfo();
  CalculateBiteAttackInfo();
}

void nonhumanoid::CalculateUnarmedAttackInfo()
{
  CalculateUnarmedDamage();
  CalculateUnarmedToHitValue();
  CalculateUnarmedAPCost();
}

void nonhumanoid::CalculateKickAttackInfo()
{
  CalculateKickDamage();
  CalculateKickToHitValue();
  CalculateKickAPCost();
}

void nonhumanoid::CalculateBiteAttackInfo()
{
  CalculateBiteDamage();
  CalculateBiteToHitValue();
  CalculateBiteAPCost();
}

void dog::BeTalkedTo()
{
  if(RAND_N(5))
  {
    if(GetRelation(PLAYER) != HOSTILE)
    {
      static truth Last;
      cchar* Reply;

      if(GetHP() << 1 > GetMaxHP())
        Reply = Last ? "barks happily" : "wags its tail happily";
      else
        Reply = Last ? "yelps" : "howls";

      ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), Reply);
      Last = !Last;
    }
    else
      character::BeTalkedTo();
  }
  else if(RAND_N(5))
    ADD_MESSAGE("\"Can't you understand I can't speak?\"");
  else
    ADD_MESSAGE("\"Meow.\"");
}

void dog::CreateCorpse(lsquare* Square)
{
  if(GetConfig() == SKELETON_DOG)
  {
    Square->AddItem(skull::Spawn(PUPPY_SKULL));

    int Amount = 2 + (RAND() & 3);

    for(int c = 0; c < Amount; ++c)
      Square->AddItem(bone::Spawn());

    SendToHell();
  }
  else
    nonhumanoid::CreateCorpse(Square);
}

col16 wolf::GetSkinColor() const
{
  int Element = 40 + RAND() % 50;
  return MakeRGB16(Element, Element, Element);
}

void genetrixvesana::GetAICommand()
{
  ++TurnsExisted;

  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(!(RAND() % 60))
  {
    int NumberOfPlants = RAND() % 3 + RAND() % 3 + RAND() % 3 + RAND() % 3;

    for(int c1 = 0; c1 < 50 && NumberOfPlants; ++c1)
    {
      for(int c2 = 0; c2 < game::GetTeams() && NumberOfPlants; ++c2)
        if(GetTeam()->GetRelation(game::GetTeam(c2)) == HOSTILE)
          for(character* p : game::GetTeam(c2)->GetMember())
          {
            if(!NumberOfPlants)
              break;

            if(p->IsEnabled())
            {
              lsquare* LSquare = p->GetNeighbourLSquare(RAND() % GetNeighbourSquares());

              if(LSquare && (LSquare->GetWalkability() & WALK) && !LSquare->GetCharacter())
              {
                character* NewPlant;
                long RandomValue = RAND() % TurnsExisted;

                if(RandomValue < 250)
                  NewPlant = carnivorousplant::Spawn();
                else if(RandomValue < 1500)
                  NewPlant = carnivorousplant::Spawn(GREATER);
                else
                  NewPlant = carnivorousplant::Spawn(GIANT);

                for(int c = 3; c < TurnsExisted / 500; ++c)
                  NewPlant->EditAllAttributes(1);

                NewPlant->SetGenerationDanger(GetGenerationDanger());
                NewPlant->SetTeam(GetTeam());
                NewPlant->PutTo(LSquare->GetPos());
                --NumberOfPlants;

                if(NewPlant->CanBeSeenByPlayer())
                {
                  if(p->IsPlayer())
                    ADD_MESSAGE("%s sprouts from the ground near you.", NewPlant->CHAR_NAME(INDEFINITE));
                  else if(p->CanBeSeenByPlayer())
                    ADD_MESSAGE("%s sprouts from the ground near %s.", NewPlant->CHAR_NAME(INDEFINITE), p->CHAR_NAME(DEFINITE));
                  else
                    ADD_MESSAGE("%s sprouts from the ground.", NewPlant->CHAR_NAME(INDEFINITE));
                }
              }
            }
          }
    }

    EditAP(-2000);
    return;
  }

  if(AttackAdjacentEnemyAI())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

col16 carnivorousplant::GetTorsoSpecialColor() const // the flower
{
  if(!GetConfig())
    return MakeRGB16(RAND() % 100, 125 + RAND() % 125, RAND() % 100);
  else if(GetConfig() == GREATER)
    return MakeRGB16(RAND() % 100, RAND() % 100, 125 + RAND() % 125);
  else
    return MakeRGB16(125 + RAND() % 125, 125 + RAND() % 125, RAND() % 100);
}

void ostrich::GetAICommand()
{
  if(game::TweraifIsFree())
  {
    nonhumanoid::GetAICommand();
    return;
  }

  if(CheckForEnemies(false, false, true, true))
    return;

  if(!IsEnabled())
    return;

  if(GetPos() == v2(45, 45))
    HasDroppedBananas = true;

  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);
  int BananasPicked = 0;

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->IsBanana() && ItemVector[c]->CanBeSeenBy(this)
       && ItemVector[c]->IsPickable(this)
       && !MakesBurdened(GetCarriedWeight() + ItemVector[c]->GetWeight()))
    {
      ItemVector[c]->MoveTo(GetStack());
      ++BananasPicked;
    }

  if(BananasPicked)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s picks up %s.", CHAR_NAME(DEFINITE), BananasPicked == 1 ? "the banana" : "some bananas");

    return;
  }

  if(!HasDroppedBananas)
  {
    SetGoingTo(v2(45, 45));

    if(MoveTowardsTarget(true))
      return;
  }
  else if(GetPos().Y == 54)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s leaves the town.", CHAR_NAME(DEFINITE));

    itemvector ItemVector;
    GetStack()->FillItemVector(ItemVector);

    for(uint c = 0; c < ItemVector.size(); ++c)
    {
      ItemVector[c]->RemoveFromSlot();
      ItemVector[c]->SendToHell();
    }

    v2 Where = GetLevel()->GetNearestFreeSquare(this, v2(45, 0));

    if(Where == ERROR_V2)
      Where = GetLevel()->GetRandomSquare(this, NOT_IN_ROOM); // this is odd but at least it doesn't crash

    Move(Where, true);
    RestoreHP();
    RestoreStamina();
    ResetStates();
    TemporaryState = 0;
    GainIntrinsic(LEVITATION);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s enters the town.", CHAR_NAME(INDEFINITE));

    HasDroppedBananas = false;
  }
  else
  {
    SetGoingTo(v2(45, 54));

    if(MoveTowardsTarget(true))
      return;
  }

  EditAP(-1000);
}

void ostrich::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << HasDroppedBananas;
}

void ostrich::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> HasDroppedBananas;
}

truth ostrich::HandleCharacterBlockingTheWay(character* Char, v2 Pos, int Dir)
{
  return Char->GetPos() == v2(45, 45) && (Displace(Char, true) || Hit(Char, Pos, Dir));
}

void elpuri::Save(outputfile& SaveFile) const
{
  largecreature::Save(SaveFile);
  SaveFile << Active;
}

void elpuri::Load(inputfile& SaveFile)
{
  largecreature::Load(SaveFile);
  SaveFile >> Active;
}

void elpuri::GetAICommand()
{
  if(Active)
    character::GetAICommand();
  else
  {
    if(CheckForEnemies(false, false, false))
      return;

    EditAP(-1000);
  }
}

int elpuri::ReceiveBodyPartDamage(character* Damager, int Damage, int Type, int BodyPartIndex,
                                  int Direction, truth PenetrateResistance, truth Critical,
                                  truth ShowNoDamageMsg, truth CaptureBodyPart)
{
  Active = true;
  return character::ReceiveBodyPartDamage(Damager, Damage, Type, BodyPartIndex, Direction,
                                          PenetrateResistance, Critical, ShowNoDamageMsg, CaptureBodyPart);
}

void mommo::CreateCorpse(lsquare* Square)
{
  for(int d = 0; d < GetExtendedNeighbourSquares(); ++d)
  {
    lsquare* NeighbourSquare = Square->GetNeighbourLSquare(d);

    if(NeighbourSquare)
      NeighbourSquare->SpillFluid(0, static_cast<liquid*>(GetTorso()->GetMainMaterial()->SpawnMore(250 + RAND() % 250)));
  }

  SendToHell();
}

void carnivorousplant::CreateCorpse(lsquare* Square)
{
  int Amount = !GetConfig() ? (RAND() % 7 ? 0 : 1) : GetConfig() == GREATER ?
               (RAND() & 1 ? 0 : (RAND() % 5 ? 1 : (RAND() % 5 ? 2 : 3))) :
               (!(RAND() % 3) ? 0 : (RAND() % 3 ? 1 : (RAND() % 3 ? 2 : 3)));

  for(int c = 0; c < Amount; ++c)
    Square->AddItem(kiwi::Spawn());

  nonhumanoid::CreateCorpse(Square);
}

void genetrixvesana::CreateCorpse(lsquare* Square)
{
  for(int c = 0; c < 3; ++c)
    Square->AddItem(pineapple::Spawn());

  largecreature::CreateCorpse(Square);
}

void nonhumanoid::AddSpecialStethoscopeInfo(felist& Info) const
{
  Info.AddEntry(CONST_S("Strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Agility: ") + GetAttribute(AGILITY), LIGHT_GRAY);
}

void floatingeye::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << WayPoints << NextWayPoint;
}

void floatingeye::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> WayPoints >> NextWayPoint;
}

void floatingeye::GetAICommand()
{
  if(WayPoints.size() && !IsGoingSomeWhere())
  {
    if(GetPos() == WayPoints[NextWayPoint])
    {
      if(NextWayPoint < WayPoints.size() - 1)
        ++NextWayPoint;
      else
        NextWayPoint = 0;
    }

    GoingTo = WayPoints[NextWayPoint];
  }

  SeekLeader(GetLeader());

  if(CheckForEnemies(false, false, true))
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

truth floatingeye::Hit(character* Enemy, v2, int, int)
{
  if(IsPlayer())
    ADD_MESSAGE("You stare at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s stares at you.", CHAR_NAME(DEFINITE));

  EditAP(-1000);
  return true;
}

int floatingeye::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart,
                         v2 HitPos, double Damage, double ToHitValue, int Success,
                         int Type, int Direction, truth Critical, truth ForceHit)
{
  if(CanBeSeenBy(Enemy) && Enemy->HasEyes() && RAND() % 3
     && Enemy->LoseConsciousness(150 + RAND_N(150))) /* Changes for fainting 2 out of 3 */
  {
    if(!Enemy->IsPlayer())
      Enemy->EditExperience(WISDOM, 75, 1 << 13);

    return HAS_FAILED;
  }
  else
    return nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue,
                                Success, Type, Direction, Critical, ForceHit);
}

void elpuri::CreateCorpse(lsquare* Square)
{
  largecreature::CreateCorpse(Square);
  Square->AddItem(headofelpuri::Spawn());
}

truth snake::SpecialBiteEffect(character* Char, v2, int, int, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour)
  {
    Char->BeginTemporaryState(POISONED, 400 + RAND_N(200));
    return true;
  }
  else
    return false;
}

truth spider::SpecialBiteEffect(character* Char, v2, int, int, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour)
  {
    Char->BeginTemporaryState(POISONED, GetConfig() == LARGE ? 80 + RAND_N(40) : 400 + RAND_N(200));
    return true;
  }
  else
    return false;
}

truth vampirebat::SpecialBiteEffect(character* Victim, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour && Victim->IsWarmBlooded() && (!(RAND() % 3) || Critical) && !Victim->AllowSpoil())
  {
    if(IsPlayer())
      ADD_MESSAGE("You drain some precious lifeblood from %s!", Victim->CHAR_DESCRIPTION(DEFINITE));
    else if(Victim->IsPlayer() || Victim->CanBeSeenByPlayer() || CanBeSeenByPlayer())
      ADD_MESSAGE("%s drains some precious lifeblood from %s!", CHAR_DESCRIPTION(DEFINITE), Victim->CHAR_DESCRIPTION(DEFINITE));

    if(Victim->IsHumanoid() && !Victim->StateIsActivated(LYCANTHROPY) && !Victim->StateIsActivated(DISEASE_IMMUNITY))
      Victim->BeginTemporaryState(VAMPIRISM, 500 + RAND_N(250));

      // HP recieved is about half the damage done; against werewolves this is full
      int DrainDamage = (DoneDamage >> 1) + 1;
      if(Victim->StateIsActivated(LYCANTHROPY))
        DrainDamage = DoneDamage + 1;

    return Victim->ReceiveBodyPartDamage(this, DrainDamage, DRAIN, BodyPartIndex, Direction);
  }
  else
    return false;
}

truth chameleon::SpecialEnemySightedReaction(character*)
{
  if(HP != MaxHP || !(RAND() % 3))
  {
    character* NewForm = PolymorphRandomly(100, 1000, 500 + RAND() % 500);
    NewForm->GainIntrinsic(POLYMORPH);
    return true;
  }

  return false;
}

int chameleon::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, v2 HitPos, double Damage,
                       double ToHitValue, int Success, int Type, int Direction, truth Critical, truth ForceHit)
{
  int Return = nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue,
                                    Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DIED)
  {
    character* NewForm = PolymorphRandomly(100, 1000, 500 + RAND() % 500);
    NewForm->GainIntrinsic(POLYMORPH);
  }

  return Return;
}

truth eddy::Hit(character* Enemy, v2, int, int)
{
  if(IsPlayer())
  {
    if(!(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND) && GetRelation(Enemy) != HOSTILE
       && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
  }

  Hostility(Enemy);

  if(RAND() & 1)
  {
    if(IsPlayer())
      ADD_MESSAGE("You engulf %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
      ADD_MESSAGE("%s engulfs %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

    Enemy->TeleportRandomly();
  }
  else if(IsPlayer())
    ADD_MESSAGE("You miss %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));

  EditAP(-500);
  return true;
}

void mushroom::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << Species;
}

void mushroom::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> Species;
}

void mushroom::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  lsquare* CradleSquare = GetNeighbourLSquare(RAND() % 8);

  if(CradleSquare && !CradleSquare->GetCharacter()
     && (CradleSquare->GetWalkability() & WALK))
  {
    int SpoiledItems = 0;
    int MushroomsNear = 0;

    for(int d = 0; d < 8; ++d)
    {
      lsquare* Square = CradleSquare->GetNeighbourLSquare(d);

      if(Square)
      {
        character* Char = Square->GetCharacter();

        if(Char && Char->IsMushroom())
          ++MushroomsNear;

        SpoiledItems += Square->GetSpoiledItems();
      }
    }

    if((SpoiledItems && MushroomsNear < 5 && !RAND_N(50))
       || (MushroomsNear < 3 && !RAND_N((1 + MushroomsNear) * 100)))
    {
      mushroom* Child = static_cast<mushroom*>(GetProtoType()->Spawn(GetConfig()));
      Child->SetSpecies(Species);
      Child->SetTeam(GetTeam());
      Child->SetGenerationDanger(GetGenerationDanger());
      Child->PutTo(CradleSquare->GetPos());

      for(int c = 0; c < BASE_ATTRIBUTES; ++c)
        Child->BaseExperience[c] = RandomizeBabyExperience(BaseExperience[c] * 4);

      if(Child->CanBeSeenByPlayer())
        ADD_MESSAGE("%s pops out from the ground.", Child->CHAR_NAME(INDEFINITE));
    }
  }

  if(AttackAdjacentEnemyAI())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void mushroom::PostConstruct()
{
  switch(RAND() % 3)
  {
   case 0: SetSpecies(MakeRGB16(125 + RAND() % 125, RAND() % 100, RAND() % 100)); break;
   case 1: SetSpecies(MakeRGB16(RAND() % 100, 125 + RAND() % 125, RAND() % 100)); break;
   case 2: SetSpecies(MakeRGB16(RAND() % 100, RAND() % 100, 125 + RAND() % 125)); break;
  }
}

void magicmushroom::GetAICommand()
{
  if(!(RAND() % 750))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    TeleportRandomly(true);
    EditAP(-1000);
  }
  else if(!(RAND() % 50))
  {
    lsquare* Square = GetNeighbourLSquare(RAND() % 8);

    if(Square && Square->IsFlyable())
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s releases odd-looking gas.", CHAR_NAME(DEFINITE));

      Square->AddSmoke(gas::Spawn(MAGIC_VAPOUR, 1000));
      EditAP(-1000);
    }
  }
  else
    mushroom::GetAICommand();
}

void mushroom::SetSpecies(int What)
{
  Species = What;
  UpdatePictures();
}

truth twoheadedmoose::Hit(character* Enemy, v2 HitPos, int Direction, int Flags)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(IsPlayer())
  {
    if(!(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND) && GetRelation(Enemy) != HOSTILE
       && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
  }
  else if(GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  if(GetBurdenState() == OVER_LOADED)
  {
    if(IsPlayer())
      ADD_MESSAGE("You cannot fight while carrying so much.");

    return false;
  }

  Hostility(Enemy);
  msgsystem::EnterBigMessageMode();
  Bite(Enemy, HitPos, Direction, Flags & SADIST_HIT);
  v2 Pos[MAX_NEIGHBOUR_SQUARES];
  character* Char[MAX_NEIGHBOUR_SQUARES];
  int Index = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    lsquare* LSquare = GetNeighbourLSquare(d);

    if(LSquare)
    {
      character* Enemy = LSquare->GetCharacter();

      if(Enemy && GetRelation(Enemy) == HOSTILE && GetAttribute(WISDOM) < Enemy->GetAttackWisdomLimit())
      {
        Pos[Index] = LSquare->GetPos();
        Char[Index++] = Enemy;
      }
    }
  }

  if(Index)
  {
    int ChosenIndex = RAND() % Index;
    Bite(Char[ChosenIndex], Pos[ChosenIndex],
         game::GetDirectionForVector(Pos[ChosenIndex] - GetPos()), Flags & SADIST_HIT);
  }

  msgsystem::LeaveBigMessageMode();
  return true;
}

truth magpie::IsRetreating() const
{
  if(nonhumanoid::IsRetreating())
    return true;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if((*i)->GetSparkleFlags())
      return true;

  return false;
}

void magpie::GetAICommand()
{
  if(!IsRetreating())
  {
    character* Char = GetRandomNeighbour();

    if(Char)
    {
      itemvector Sparkling;

      for(stackiterator i = Char->GetStack()->GetBottom(); i.HasItem(); ++i)
      {
        if((*i)->GetSparkleFlags() && !MakesBurdened((*i)->GetWeight()))
          Sparkling.push_back(*i);
      }

      if(!Sparkling.empty())
      {
        item* ToSteal = Sparkling[RAND() % Sparkling.size()];
        ToSteal->RemoveFromSlot();
        GetStack()->AddItem(ToSteal);

        if(Char->IsPlayer())
          ADD_MESSAGE("%s steals your %s.", CHAR_NAME(DEFINITE), ToSteal->CHAR_NAME(UNARTICLED));

        EditAP(-500);
        return;
      }
    }
  }

  nonhumanoid::GetAICommand();
}

void eddy::GetAICommand()
{
  if(!GetLSquareUnder()->GetOLTerrain() && !(RAND() % 500))
  {
    decoration* Couch = decoration::Spawn(RAND_N(5) ? COUCH : DOUBLE_BED);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s spits out %s.", CHAR_NAME(DEFINITE), Couch->CHAR_NAME(INDEFINITE));

    GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(Couch);
    EditAP(-1000);
    return;
  }

  if(GetStackUnder()->GetItems() && !(RAND() % 10))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s engulfs something under it.", CHAR_NAME(DEFINITE));

    GetStackUnder()->TeleportRandomly(3);
    EditAP(-1000);
    return;
  }

  if(!(RAND() % 100))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s engulfs itself.", CHAR_NAME(DEFINITE));

    TeleportRandomly(true);
    EditAP(-1000);
    return;
  }

  nonhumanoid::GetAICommand();
}

void skunk::GetAICommand()
{
  if(!IsRetreating())
  {
    if(!RAND_N(4))
    {
      character* Char = GetRandomNeighbour(HOSTILE);

      if(Char)
      {
        int Amount = 500 / Char->GetSquaresUnder();
        truth Success = false;

        for(int c = 0; c < Char->GetSquaresUnder(); ++c)
          if(Char->GetLSquareUnder(c)->IsFlyable())
          {
            Success = true;
            Char->GetLSquareUnder(c)->AddSmoke(gas::Spawn(SKUNK_SMELL, Amount));
          }

        if(Success)
        {
          if(CanBeSeenByPlayer())
            ADD_MESSAGE("%s stinks.", CHAR_NAME(DEFINITE));

          EditAP(-1000);
          return;
        }
      }
    }
  }
  else if(RAND_N(2))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s stinks.", CHAR_NAME(DEFINITE));

    GetLSquareUnder()->AddSmoke(gas::Spawn(SKUNK_SMELL, 500));
  }

  nonhumanoid::GetAICommand();
}

truth elpuri::TryToRiseFromTheDead()
{
  character::TryToRiseFromTheDead();

  for(int c = 0; c < GetSquaresUnder(); ++c)
    for(stackiterator i = GetLSquareUnder(c)->GetStack()->GetBottom(); i.HasItem(); ++i)
      if(i->IsHeadOfElpuri())
      {
        i->SendToHell();
        i->RemoveFromSlot();
        return true;
      }

  if(CanBeSeenByPlayer())
  {
    ADD_MESSAGE("The headless body of %s vibrates violently.", CHAR_NAME(DEFINITE));
    ADD_MESSAGE("%s dies.", CHAR_NAME(DEFINITE));
  }

  return false;
}

truth nonhumanoid::EditAllAttributes(int Amount)
{
  if(!Amount)
    return true;

  LimitRef(StrengthExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  LimitRef(AgilityExperience += Amount * EXP_MULTIPLIER, MIN_EXP, MAX_EXP);
  return character::EditAllAttributes(Amount)
    || (Amount < 0
        && (StrengthExperience != MIN_EXP || AgilityExperience != MIN_EXP))
    || (Amount > 0
        && (StrengthExperience != MAX_EXP || AgilityExperience != MAX_EXP));
}

#ifdef WIZARD

void nonhumanoid::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(45);
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.Resize(48);
  Entry << "-  -  " << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  festring Entry;

  if(IsUsingArms())
  {
    Entry = CONST_S("   unarmed attack");
    Entry.Resize(50);
    Entry << GetUnarmedMinDamage() << '-' << GetUnarmedMaxDamage();
    Entry.Resize(60);
    Entry << int(GetUnarmedToHitValue());
    Entry.Resize(70);
    Entry << GetUnarmedAPCost();
    List.AddEntry(Entry, LIGHT_GRAY);
  }

  if(IsUsingLegs())
  {
    Entry = CONST_S("   kick attack");
    Entry.Resize(50);
    Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
    Entry.Resize(60);
    Entry << int(GetKickToHitValue());
    Entry.Resize(70);
    Entry << GetKickAPCost();
    List.AddEntry(Entry, LIGHT_GRAY);
  }

  if(IsUsingHead())
  {
    Entry = CONST_S("   bite attack");
    Entry.Resize(50);
    Entry << GetBiteMinDamage() << '-' << GetBiteMaxDamage();
    Entry.Resize(60);
    Entry << int(GetBiteToHitValue());
    Entry.Resize(70);
    Entry << GetBiteAPCost();
    List.AddEntry(Entry, LIGHT_GRAY);
  }
}

#else

void nonhumanoid::AddAttributeInfo(festring&) const { }
void nonhumanoid::AddAttackInfo(felist&) const { }

#endif

truth elpuri::MustBeRemovedFromBone() const
{
  return !IsEnabled()
         || GetTeam()->GetID() != MONSTER_TEAM
         || GetDungeon()->GetIndex() != ELPURI_CAVE
         || GetLevel()->GetIndex() != DARK_LEVEL;
}

truth genetrixvesana::MustBeRemovedFromBone() const
{
  return !IsEnabled()
         || GetTeam()->GetID() != MONSTER_TEAM
         || GetDungeon()->GetIndex() != UNDER_WATER_TUNNEL
         || GetLevel()->GetIndex() != VESANA_LEVEL;
}

int largecreature::GetSquareIndex(v2 Pos) const
{
  v2 RelativePos = Pos - GetPos();
  return RelativePos.X + (RelativePos.Y << 1);
}

square* largecreature::GetNeighbourSquare(int I) const
{
  square* SquareUnder = GetSquareUnder();
  area* Area = SquareUnder->GetArea();
  v2 Pos = SquareUnder->GetPos() + game::GetLargeMoveVector(I);
  return Area->IsValidPos(Pos) ? SquareUnder->GetArea()->GetSquare(Pos) : 0;
}

int largecreature::CalculateNewSquaresUnder(lsquare** NewSquare, v2 Pos) const
{
  level* Level = GetLevel();

  for(int c = 0; c < 4; ++c)
  {
    v2 SquarePos = Pos + game::GetLargeMoveVector(12 + c);

    if(Level->IsValidPos(SquarePos))
      NewSquare[c] = Level->GetLSquare(SquarePos);
    else
      return 0;
  }

  return 4;
}

truth largecreature::IsFreeForMe(square* Square) const
{
  v2 Pos = Square->GetPos();
  area* Area = Square->GetArea();

  for(int c = 0; c < 4; ++c)
  {
    v2 SquarePos = Pos + game::GetLargeMoveVector(12 + c);

    if(!Area->IsValidPos(SquarePos)
       || (Area->GetSquare(SquarePos)->GetCharacter()
           && Area->GetSquare(SquarePos)->GetCharacter() != static_cast<ccharacter*>(this)))
      return false;
  }

  return true;
}

truth largecreature::CanTheoreticallyMoveOn(const lsquare* LSquare) const
{
  v2 Pos = LSquare->GetPos();
  level* Level = LSquare->GetLevel();

  for(int c = 0; c < 4; ++c)
  {
    v2 SquarePos = Pos + game::GetLargeMoveVector(12 + c);

    if(!Level->IsValidPos(SquarePos) || !(GetMoveType() & Level->GetLSquare(SquarePos)->GetTheoreticalWalkability()))
      return false;
  }

  return true;
}

truth largecreature::CanMoveOn(const lsquare* LSquare) const
{
  v2 Pos = LSquare->GetPos();
  level* Level = LSquare->GetLevel();

  for(int c = 0; c < 4; ++c)
  {
    v2 SquarePos = Pos + game::GetLargeMoveVector(12 + c);

    if(!Level->IsValidPos(SquarePos) || !PartCanMoveOn(Level->GetLSquare(SquarePos)))
      return false;
  }

  return true;
}

truth largecreature::CanMoveOn(const square* Square) const
{
  v2 Pos = Square->GetPos();
  area* Area = Square->GetArea();

  for(int c = 0; c < 4; ++c)
  {
    v2 SquarePos = Pos + game::GetLargeMoveVector(12 + c);
    if(!Area->IsValidPos(SquarePos) || !(GetMoveType() & Area->GetSquare(SquarePos)->GetSquareWalkability()))
      return false;
  }

  return true;
}

void largecreature::PutTo(v2 Pos)
{
  for(int c = 0; c < 4; ++c)
  {
    SquareUnder[c] = game::GetCurrentArea()->GetSquare(Pos + game::GetLargeMoveVector(12 + c));
    SquareUnder[c]->AddCharacter(this);
  }
}

void largecreature::Remove()
{
  for(int c = 0; c < 4; ++c)
  {
    SquareUnder[c]->RemoveCharacter();
    SquareUnder[c] = 0;
  }
}

void largecreature::CreateCorpse(lsquare* Square)
{
  if(!BodyPartsDisappearWhenSevered() && !game::AllBodyPartsVanish())
  {
    corpse* Corpse = largecorpse::Spawn(0, NO_MATERIALS);
    Corpse->SetDeceased(this);
    Square->AddItem(Corpse);
    Disable();
  }
  else
    SendToHell();
}

void largecreature::LoadSquaresUnder()
{
  for(int c = 0; c < 4; ++c)
    SquareUnder[c] = game::GetSquareInLoad()->GetArea()->GetSquare(game::GetSquareInLoad()->GetPos()
                                                                   + game::GetLargeMoveVector(12 + c));
}

truth vladimir::MustBeRemovedFromBone() const
{
  return !IsEnabled()
         || GetTeam()->GetID() != IVAN_TEAM
         || GetDungeon()->GetIndex() != ELPURI_CAVE
         || GetLevel()->GetIndex() != IVAN_LEVEL;
}

void hattifattener::GetAICommand()
{
  if(!(RAND() % 7))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s emits a lightning bolt!", CHAR_DESCRIPTION(DEFINITE));

    beamdata Beam
      (
        this,
        "killed by a hattifattener's lightning",
        GetPos(),
        WHITE,
        BEAM_LIGHTNING,
        RAND() & 7,
        1 + (RAND() & 7),
        0
      );

    GetLevel()->LightningBeam(Beam);
    EditAP(-1000);
    return;
  }

  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void hattifattener::CreateCorpse(lsquare* Square)
{
  level* Level = Square->GetLevel();
  ulong StackSize = Level->AddRadiusToSquareStack(Square->GetPos(), 9);
  lsquare** SquareStack = Level->GetSquareStack();
  ulong c;

  for(c = 0; c < StackSize; ++c)
    SquareStack[c]->RemoveFlags(IN_SQUARE_STACK);

  fearray<lsquare*> Stack(SquareStack, StackSize);
  Level->LightningVisualizer(Stack, WHITE);

  for(c = 0; c < Stack.Size; ++c)
  {
    beamdata Beam
      (
        this,
        CONST_S("killed by electricity released by a dying hattifattener"),
        YOURSELF,
        0
      );

    Stack[c]->Lightning(Beam);
  }

  SendToHell();
}

void hedgehog::SpecialBodyDefenceEffect(character* Enemy, bodypart* BodyPart, int Type)
{
  if(Type != WEAPON_ATTACK && RAND() & 1)
  {
    if(Enemy->IsPlayer())
      ADD_MESSAGE("%s spines jab your %s!", CHAR_POSSESSIVE_PRONOUN, BodyPart->GetBodyPartName().CStr());
    else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
      ADD_MESSAGE("%s spines jab %s!", CHAR_POSSESSIVE_PRONOUN, Enemy->CHAR_NAME(DEFINITE));

    Enemy->ReceiveBodyPartDamage(this, 1 + (RAND() & 1), PHYSICAL_DAMAGE, BodyPart->GetBodyPartIndex(),
                                 YOURSELF, false, false, true, false);
    Enemy->CheckDeath(CONST_S("killed by the pointy spines of ") + GetName(INDEFINITE), this);
  }
}

void genetrixvesana::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << TurnsExisted;
}

void genetrixvesana::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> TurnsExisted;
}

truth largecreature::CreateRoute()
{
  Route.clear();

  if(GetAttribute(INTELLIGENCE) >= 10 && !StateIsActivated(CONFUSED))
  {
    node* Node = GetLevel()->FindRoute(GetPos(), GoingTo, Illegal, 0, this);

    if(Node)
      while(Node->Last)
      {
        Route.push_back(Node->Pos);
        Node = Node->Last;
      }
    else
      TerminateGoingTo();

    IntelligenceAction(5);
    return true;
  }
  else
    return false;
}

void bunny::GetAICommand()
{
  if(GetConfig() < 4 && GetNP() > (SATIATED_LEVEL + BLOATED_LEVEL) >> 1)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s looks more mature.", CHAR_NAME(DEFINITE));

    GetTorso()->SetSize(GetTorso()->GetSize() << 1);
    LimitRef(StrengthExperience *= 2, MIN_EXP, MAX_EXP);
    LimitRef(AgilityExperience *= 2, MIN_EXP, MAX_EXP);

    for(int c = 0; c < BASE_ATTRIBUTES; ++c)
      BaseExperience[c] = Limit(BaseExperience[c] * 2, MIN_EXP, MAX_EXP);

    GetTorso()->GetMainMaterial()->SetVolume(GetTorso()->GetMainMaterial()->GetVolume() << 1);
    SetConfig(GetConfig() + 2);
    RestoreHP();
    RestoreStamina();
  }

  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(CheckForFood(5))
    return;

  if(CheckForMatePartner())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void bunny::SignalNaturalGeneration()
{
  character* Partner = bunny::Spawn(GetConfig()^1);
  Partner->SetTeam(GetTeam());
  Partner->SetGenerationDanger(GetGenerationDanger());
  Partner->PutNear(GetPos());
}

truth bunny::CheckForMatePartner()
{
  if(GetConfig() == ADULT_MALE)
  {
    character* BestPartner = 0;
    double BestPartnerDanger = 0;

    for(int c = 0; c < game::GetTeams(); ++c)
      if(GetTeam()->GetRelation(game::GetTeam(c)) != HOSTILE)
        for(character* p : game::GetTeam(c)->GetMember())
          if(p->IsEnabled() && p->IsBunny() && p->GetConfig() == ADULT_FEMALE && p->GetNP() > SATIATED_LEVEL)
          {
            double Danger = p->GetRelativeDanger(this, true);

            if(Danger > BestPartnerDanger)
            {
              BestPartner = p;
              BestPartnerDanger = Danger;
            }
          }

    if(BestPartner && !GetPos().IsAdjacent(BestPartner->GetPos()))
    {
      SetGoingTo(BestPartner->GetPos());
      MoveTowardsTarget(true);
      return true;
    }
  }

  if(GetConfig() == ADULT_FEMALE && GetNP() > NOT_HUNGER_LEVEL + 10000)
  {
    for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
      {
        character* Father = Square->GetCharacter();

        if(Father && Father->IsBunny() && Father->GetConfig() == ADULT_MALE && GetRelation(Father) != HOSTILE)
        {
          if(CanBeSeenByPlayer())
          {
            if(Father->IsPlayer())
              ADD_MESSAGE("You have much fun with %s.", CHAR_NAME(DEFINITE));
            else if(Father->CanBeSeenByPlayer())
              ADD_MESSAGE("%s and %s seem to have much fun together.",
                          Father->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
            else
              ADD_MESSAGE("%s seems to have much fun.", CHAR_NAME(DEFINITE));
          }
          else
          {
            if(Father->IsPlayer())
              ADD_MESSAGE("You have much fun with something.");
            else if(Father->CanBeSeenByPlayer())
              ADD_MESSAGE("%s seems to have much fun.", Father->CHAR_NAME(DEFINITE));
          }

          bunny* Baby = bunny::Spawn(BABY_MALE + (RAND() & 1));
          Baby->StrengthExperience = RandomizeBabyExperience(StrengthExperience + static_cast<bunny*>(Father)->StrengthExperience);
          Baby->AgilityExperience = RandomizeBabyExperience(AgilityExperience + static_cast<bunny*>(Father)->AgilityExperience);

          if(Baby->GetConfig() == BABY_MALE)
          {
            Baby->StrengthExperience *= 4;
            Baby->AgilityExperience *= 4;
          }
          else
          {
            Baby->StrengthExperience *= 2;
            Baby->AgilityExperience *= 6;
          }

          Baby->StrengthExperience /= 3;
          Baby->AgilityExperience /= 5;

          for(int c = 0; c < BASE_ATTRIBUTES; ++c)
            Baby->BaseExperience[c] = RandomizeBabyExperience(BaseExperience[c] + static_cast<bunny*>(Father)->BaseExperience[c]);

          Baby->CalculateAll();
          Baby->RestoreHP();
          Baby->RestoreStamina();
          Baby->SetTeam(GetTeam());
          Baby->SetGenerationDanger(GetGenerationDanger());
          Baby->PutNear(GetPos());

          if(Baby->CanBeSeenByPlayer())
            ADD_MESSAGE("%s is born.", Baby->CHAR_NAME(INDEFINITE));

          EditNP(-10000);
          Father->EditAP(-3000);
          EditAP(-5000);
          EditStamina(-GetMaxStamina() >> 1, true);
          Father->EditStamina(-(Father->GetMaxStamina() << 2) / 5, true);
          return true;
        }
      }
    }
  }

  return false;
}

truth bunny::Catches(item* Thingy)
{
  if(Thingy->BunnyWillCatchAndConsume(this))
  {
    if(ConsumeItem(Thingy, CONST_S("eating")))
    {
      if(IsPlayer())
        ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
      else
      {
        if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

        ChangeTeam(PLAYER->GetTeam());
      }
    }
    else if(IsPlayer())
      ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

    return true;
  }
  else
    return false;
}

truth largecreature::PlaceIsIllegal(v2 Pos, v2 Illegal) const
{
  for(int c = 0; c < 4; ++c)
    if(Pos + game::GetLargeMoveVector(12 + c) == Illegal)
      return true;

  return false;
}

truth mommo::Hit(character* Enemy, v2 Pos, int, int)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(IsPlayer())
  {
    if(!(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND) && GetRelation(Enemy) != HOSTILE
       && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
  }
  else if(GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  Hostility(Enemy);

  if(IsPlayer())
    ADD_MESSAGE("You spill acidous slime at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    ADD_MESSAGE("%s spills acidous slime at %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

  Vomit(Pos, 250 + RAND() % 250, false);
  EditAP(-1000);
  return true;
}

void mommo::GetAICommand()
{
  SeekLeader(GetLeader());

  if(CheckForEnemies(false, false, true))
    return;

  if(!(RAND() % 10))
  {
    VomitAtRandomDirection(350 + RAND() % 350);
    EditAP(-1000);
    return;
  }

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void dog::GetAICommand()
{
  if(!game::IsInWilderness() && !(RAND() & 7))
    GetLSquareUnder()->SpillFluid(this, liquid::Spawn(DOG_DROOL, 25 + RAND() % 50), false, false);

  character::GetAICommand();
}

truth blinkdog::SpecialEnemySightedReaction(character*)
{
  if(!(RAND() & 15) && SummonFriend())
    return true;

  if(!(RAND() & 31))
  {
    MonsterTeleport("a playful bark");
    return true;
  }

  if((!(RAND() & 3) && StateIsActivated(PANIC))
     || (!(RAND() & 7) && IsInBadCondition()))
  {
    MonsterTeleport("a frightened howl");
    return true;
  }

  return false;
}

void blinkdog::MonsterTeleport(cchar* BarkMsg)
{
  if(IsPlayer())
    ADD_MESSAGE("You vanish.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("You hear %s inside your head as %s vanishes!", BarkMsg, CHAR_NAME(DEFINITE));
  else
    ADD_MESSAGE("You hear %s inside your head.", BarkMsg);

  v2 Pos = GetPos();
  rect Border(Pos + v2(-5, -5), Pos + v2(5, 5));
  Pos = GetLevel()->GetRandomSquare(this, 0, &Border);

  if(Pos == ERROR_V2)
    Pos = GetLevel()->GetRandomSquare(this);

  Move(Pos, true);

  if(IsPlayer())
    ADD_MESSAGE("You materialize.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s materializes from nowhere!", CHAR_NAME(INDEFINITE));

  EditAP(-1000);
}

int unicorn::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, v2 HitPos, double Damage,
                     double ToHitValue, int Success, int Type, int Direction, truth Critical, truth ForceHit)
{
  int Return = nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue,
                                    Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DIED
     && (StateIsActivated(PANIC)
         || (RAND() & 1 && IsInBadCondition())
         || !(RAND() & 7)))
    MonsterTeleport(" in terror");

  return Return;
}

int blinkdog::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, v2 HitPos, double Damage,
                      double ToHitValue, int Success, int Type, int Direction, truth Critical, truth ForceHit)
{
  int Return = nonhumanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage, ToHitValue,
                                    Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DIED)
  {
    if(!(RAND() & 15) && SummonFriend())
      return Return;

    if((RAND() & 1 && StateIsActivated(PANIC))
       || (!(RAND() & 3) && IsInBadCondition())
       || !(RAND() & 15))
    MonsterTeleport("a terrified yelp");
  }

  return Return;
}

void unicorn::MonsterTeleport(cchar* NeighDescription)
{
  if(IsPlayer())
    ADD_MESSAGE("You neigh%s and disappear.", NeighDescription);
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s neighs%s and disappears!", CHAR_NAME(DEFINITE), NeighDescription);

  Move(GetLevel()->GetRandomSquare(this), true);

  if(IsPlayer())
    ADD_MESSAGE("You reappear.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s appears from nothing!", CHAR_NAME(INDEFINITE));

  EditAP(-1000);
}

truth blinkdog::SummonFriend()
{
  if(!SummonModifier)
    return false;

  --SummonModifier;
  blinkdog* Buddy = blinkdog::Spawn();
  Buddy->SummonModifier = SummonModifier;
  Buddy->SetTeam(GetTeam());
  Buddy->SetGenerationDanger(GetGenerationDanger());
  Buddy->PutNear(GetPos());

  if(CanBeSeenByPlayer())
  {
    ADD_MESSAGE("%s wags its tail in a mysterious pattern.", CHAR_NAME(DEFINITE));

    if(Buddy->CanBeSeenByPlayer())
      ADD_MESSAGE("Another of its kin appears!");
  }
  else if(Buddy->CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears!", Buddy->CHAR_NAME(INDEFINITE));

  EditAP(-1000);
  return true;
}

blinkdog::blinkdog()
{
  if(!game::IsLoading())
    SummonModifier = RAND_2 + RAND_2 + RAND_2 + RAND_2 + RAND_2 + RAND_2 + RAND_2;
}

void blinkdog::Save(outputfile& SaveFile) const
{
  dog::Save(SaveFile);
  SaveFile << SummonModifier;
}

void blinkdog::Load(inputfile& SaveFile)
{
  dog::Load(SaveFile);
  SaveFile >> SummonModifier;
}

void genetrixvesana::FinalProcessForBone()
{
  largecreature::FinalProcessForBone();
  TurnsExisted = 0;
}

void carnivorousplant::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(AttackAdjacentEnemyAI())
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void mysticfrog::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  character* NearestEnemy = 0;
  long NearestEnemyDistance = 0x7FFFFFFF;
  character* RandomFriend = 0;
  charactervector Friend;
  v2 Pos = GetPos();
  truth Enemies = false;

  for(int c = 0; c < game::GetTeams(); ++c)
  {
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          Enemies = true;
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));

          if((ThisDistance < NearestEnemyDistance || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && p->CanBeSeenBy(this))
          {
            NearestEnemy = p;
            NearestEnemyDistance = ThisDistance;
          }
        }
    }
    else if(GetTeam()->GetRelation(game::GetTeam(c)) == FRIEND)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled() && p->CanBeSeenBy(this))
          Friend.push_back(p);
    }
  }

  if(NearestEnemy && NearestEnemy->GetPos().IsAdjacent(Pos))
  {
    if(NearestEnemy->IsSmall()
       && GetAttribute(WISDOM) < NearestEnemy->GetAttackWisdomLimit()
       && !(RAND() % 5)
       && Hit(NearestEnemy, NearestEnemy->GetPos(), game::GetDirectionForVector(NearestEnemy->GetPos() - GetPos())))
      return;
    else if(!(RAND() & 3))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s invokes a spell and disappears.", CHAR_NAME(DEFINITE));

      TeleportRandomly(true);
      EditAP(-GetSpellAPCost());
      return;
    }
  }

  if(NearestEnemy && (NearestEnemyDistance < 10 || StateIsActivated(PANIC)) && RAND() & 3)
  {
    SetGoingTo((Pos << 1) - NearestEnemy->GetPos());

    if(MoveTowardsTarget(true))
      return;
  }

  if(Friend.size() && !(RAND() & 3))
  {
    RandomFriend = Friend[RAND() % Friend.size()];
    NearestEnemy = 0;
  }

  if(GetRelation(PLAYER) == HOSTILE && PLAYER->CanBeSeenBy(this) && !RAND_4)
    NearestEnemy = PLAYER;

  beamdata Beam
    (
      this,
      CONST_S("killed by the spells of ") + GetName(INDEFINITE),
      YOURSELF,
      0
    );

  if(NearestEnemy)
  {
    lsquare* Square = NearestEnemy->GetLSquareUnder();
    EditAP(-GetSpellAPCost());

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

    switch(RAND() % 20)
    {
     case 0:
     case 1:
     case 2:
     case 3:
     case 4:
     case 5: Square->DrawParticles(RED); if(NearestEnemy->TeleportRandomItem(GetConfig() == DARK)) break;
     case 6:
     case 7:
     case 8:
     case 9:
     case 10: Square->DrawParticles(RED); Square->Teleport(Beam); break;
     case 11:
     case 12:
     case 13:
     case 14: Square->DrawParticles(RED); Square->Slow(Beam); break;
     case 15: Square->DrawParticles(RED); Square->LowerEnchantment(Beam); break;
     default: Square->DrawLightning(v2(8, 8), WHITE, YOURSELF); Square->Lightning(Beam); break;
    }

    if(CanBeSeenByPlayer())
      NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell of ") + GetName(DEFINITE)
                                              + CONST_S(" interrupts you."));
    else
      NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell interrupts you."));

    return;
  }

  if(RandomFriend && Enemies)
  {
    lsquare* Square = RandomFriend->GetLSquareUnder();
    EditAP(-GetSpellAPCost());
    Square->DrawParticles(RED);

    if(RAND() & 1)
      Square->Invisibility(Beam);
    else
      Square->Haste(Beam);

    return;
  }

  StandIdleAI();
}

truth largecreature::PartCanMoveOn(const lsquare* LSquare) const
{
  int Walkability = LSquare->GetWalkability();

  if(GetMoveType() & Walkability)
    return true;

  if(DestroysWalls() && Walkability & ETHEREAL)
  {
    olterrain* Terrain = LSquare->GetOLTerrain();

    if(Terrain && Terrain->WillBeDestroyedBy(this))
    {
      room* Room = LSquare->GetRoom();

      if(!Room || Room->IsOKToDestroyWalls(this))
        return true;
    }
  }

  return false;
}

void spider::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  character* NearestChar = 0;
  long NearestDistance = 0x7FFFFFFF;
  v2 Pos = GetPos();
  int Hostiles = 0;

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled() && GetAttribute(WISDOM) < p->GetAttackWisdomLimit())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));
          ++Hostiles;

          if((ThisDistance < NearestDistance
              || (ThisDistance == NearestDistance && !(RAND() % 3)))
             && p->CanBeSeenBy(this, false, IsGoingSomeWhere())
             && (!IsGoingSomeWhere() || HasClearRouteTo(p->GetPos())))
          {
            NearestChar = p;
            NearestDistance = ThisDistance;
          }
        }

  if(Hostiles && !RAND_N(Max(80 / Hostiles, 8)))
  {
    web* Web = web::Spawn();
    Web->SetStrength(GetConfig() == LARGE ? 10 : 25);

    if(GetLSquareUnder()->AddTrap(Web))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s spins a web.", CHAR_NAME(DEFINITE));

      EditAP(-1000);
      return;
    }
  }

  if(NearestChar)
  {
    if(NearestChar->IsStuck())
      SetGoingTo(NearestChar->GetPos());
    else
      SetGoingTo((Pos << 1) - NearestChar->GetPos());

    if(MoveTowardsTarget(true))
      return;
  }

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void largecat::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << Lives;
}

void largecat::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> Lives;
}

truth largecat::SpecialSaveLife()
{
  if(--Lives <= 0 || game::IsInWilderness())
    return false;

  if(IsPlayer())
    ADD_MESSAGE("But wait! You seem to have miraculously avoided certain death!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("But wait, %s seems to have miraculously avoided certain death!", GetPersonalPronoun().CStr());

  v2 Pos = GetPos();
  rect Border(Pos + v2(-20, -20), Pos + v2(20, 20));
  Pos = GetLevel()->GetRandomSquare(this, 0, &Border);

  if(Pos == ERROR_V2)
    Pos = GetLevel()->GetRandomSquare(this);

  Move(Pos, true);

  if(!IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears!", CHAR_NAME(INDEFINITE));

  if(IsPlayer())
    game::AskForKeyPress(CONST_S("Life saved! [press any key to continue]"));

  RestoreBodyParts();
  ResetSpoiling();
  ResetBurning();
  RestoreHP();
  RestoreStamina();
  ResetStates();

  if(GetNP() < SATIATED_LEVEL)
    SetNP(SATIATED_LEVEL);

  SendNewDrawRequest();

  if(GetAction())
    GetAction()->Terminate(false);

  return true;
}

truth lobhse::MustBeRemovedFromBone() const
{
  return !IsEnabled()
         || GetTeam()->GetID() != MONSTER_TEAM
         || GetDungeon()->GetIndex() != UNDER_WATER_TUNNEL
         || GetLevel()->GetIndex() != SPIDER_LEVEL;
}

truth lobhse::SpecialBiteEffect(character* Char, v2, int, int, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour)
  {
    Char->BeginTemporaryState(POISONED, 80 + RAND() % 40);
    return true;
  }
  else
    return false;
}

void lobhse::GetAICommand()
{
  SeekLeader(GetLeader()); // will follow if tamed

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void lobhse::CreateCorpse(lsquare* Square)
{
  largecreature::CreateCorpse(Square);
}

void mindworm::GetAICommand()
{
  character* NeighbourEnemy = GetRandomNeighbour(HOSTILE);
  character* NearestEnemy = GetNearestEnemy();

  if(GetConfig() == BOIL && NeighbourEnemy)
  {
    if(NeighbourEnemy->HasHead() && !NeighbourEnemy->StateIsActivated(PARASITE_MIND_WORM))
    {
      if(TryToImplantLarvae(NeighbourEnemy))
        return;
    }
  }
  if(NearestEnemy && !NearestEnemy->IsESPBlockedByEquipment() && !StateIsActivated(CONFUSED) && !(RAND() & 2))
  {
    PsiAttack(NearestEnemy);
    return;
  }
  else
    nonhumanoid::GetAICommand();
}

truth mindworm::TryToImplantLarvae(character* Victim)
{
  if(Victim->MindWormCanPenetrateSkull(this) && Victim->CanBeParasitized())
  {
    if(Victim->IsPlayer())
    {
      ADD_MESSAGE("%s digs through your skull, lays %s eggs and jumps out.",
                  CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN);
    }
    else if(Victim->CanBeSeenByPlayer())
    {
      ADD_MESSAGE("%s digs through %s's skull, lays %s eggs and jumps out.",
                  CHAR_NAME(DEFINITE), Victim->CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN);
    }

    Victim->BeginTemporaryState(PARASITE_MIND_WORM, 400 + RAND_N(200));

    MoveRandomly();
    EditAP(-1000);
    return true;
  }
  else
    return false;
}

void mindworm::PsiAttack(character* Victim)
{
  if(Victim->IsPlayer())
  {
    ADD_MESSAGE("Your brain is on fire.");
  }
  else if(Victim->CanBeSeenByPlayer() && PLAYER->GetAttribute(PERCEPTION) > RAND_N(20))
  {
    ADD_MESSAGE("%s looks pained.", Victim->CHAR_NAME(DEFINITE));
  }

  Victim->ReceiveDamage(this, 1 + RAND_N(2), PSI, HEAD, YOURSELF, true);
  Victim->CheckDeath(CONST_S("killed by ") + GetName(INDEFINITE) + "'s psi attack", this);
  EditAP(-2000);
  EditStamina(-10000 / GetAttribute(INTELLIGENCE), false);
}
