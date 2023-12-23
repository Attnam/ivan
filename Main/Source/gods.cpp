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

#include <vector>


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

/**
 * changing the order of these enums will mess importing old savegames (but wont break them)
 * prefer sorting on the initialization of the strings FavourInit()
 */
enum eFavours {
  FAVOUR_CALLRAIN = 1,
  FAVOUR_CONFUSE,
  FAVOUR_CURELEPROSY,
  FAVOUR_CURELYCANTHROPY,
  FAVOUR_CUREMINDWORM,
  FAVOUR_CUREPOISON,
  FAVOUR_CURESLOWNESS,
  FAVOUR_CURETAPEWORM,
  FAVOUR_CUREWOUNDS,
  FAVOUR_DISEASEIMMUNITY,
  FAVOUR_EARTHQUAKE,
  FAVOUR_ENCHANT,
  FAVOUR_ETHEREALMOV,
  FAVOUR_EXTINGUISHFIRE,
  FAVOUR_FEED,
  FAVOUR_FIRESTORM,
  FAVOUR_FIXEQUIPMENT,
  FAVOUR_HEALBURNS,
  FAVOUR_HOLYGREN,
  FAVOUR_INFRAVISION,
  FAVOUR_INVIGORATE,
  FAVOUR_INVISIBILITY,
  FAVOUR_SHOPPING,
  FAVOUR_SPEEDUP,
  FAVOUR_STOPFIRE,
  FAVOUR_SUMMONWOLF,
  FAVOUR_TAME,
  FAVOUR_TELEPORT,
  FAVOUR_BURNENEMIES,
  FAVOUR_FEELENEMIES,
  FAVOUR_POLYCONTROL,
  FAVOUR_TELEPCONTROL,
  FAVOUR_ENRAGE,
  FAVOUR_CAUSEFEAR,
  FAVOUR_CUREVAMP,
};

void god::FavourInit() //this one is better on this file
{
  AddFavourID(FAVOUR_BURNENEMIES,"Immolation");
  AddFavourID(FAVOUR_CALLRAIN,"Call Rain");
  AddFavourID(FAVOUR_CAUSEFEAR,"Inspire Fear");
  AddFavourID(FAVOUR_CONFUSE,"Spread Confusion");
  AddFavourID(FAVOUR_CURELEPROSY,"Cure Leprosy");
  AddFavourID(FAVOUR_CURELYCANTHROPY,"Cure Lycanthropy");
  AddFavourID(FAVOUR_CUREMINDWORM,"Remove Brain Parasite");
  AddFavourID(FAVOUR_CUREPOISON,"Cure Poison");
  AddFavourID(FAVOUR_CURESLOWNESS,"Cure Slowness");
  AddFavourID(FAVOUR_CURETAPEWORM,"Remove Stomach Parasite");
  AddFavourID(FAVOUR_CUREVAMP,"Cure Vampirism");
  AddFavourID(FAVOUR_CUREWOUNDS,"Heal");
  AddFavourID(FAVOUR_DISEASEIMMUNITY,"Ward Off Disease");
  AddFavourID(FAVOUR_EARTHQUAKE,"Quake the Earth");
  AddFavourID(FAVOUR_ENCHANT,"Enchant Equipment");
  AddFavourID(FAVOUR_ENRAGE,"Second Wind");
  AddFavourID(FAVOUR_ETHEREALMOV,"Join the Shadows");
  AddFavourID(FAVOUR_EXTINGUISHFIRE,"Quench Flames"); //TODO: consider price vs FAVOUR_HEALBURNS
  AddFavourID(FAVOUR_FEED,"Calm Hunger");
  AddFavourID(FAVOUR_FEELENEMIES,"Sense Thy Foes");
  AddFavourID(FAVOUR_FIRESTORM,"Holy Flames");
  AddFavourID(FAVOUR_FIXEQUIPMENT,"Repair Item");
  AddFavourID(FAVOUR_HEALBURNS,"Remove Burns");
  AddFavourID(FAVOUR_HOLYGREN,"Paladin's Gift");
  AddFavourID(FAVOUR_INFRAVISION,"See Thy Foes");
  AddFavourID(FAVOUR_INVIGORATE,"Invigorate");
  AddFavourID(FAVOUR_INVISIBILITY,"Become Invisible");
  AddFavourID(FAVOUR_POLYCONTROL,"Control Shape");
  AddFavourID(FAVOUR_SHOPPING,"Bounty"); //"Black Friday"
  AddFavourID(FAVOUR_SPEEDUP,"Haste");
  AddFavourID(FAVOUR_STOPFIRE,"Repair Burns");
  AddFavourID(FAVOUR_SUMMONWOLF,"Summon Nature's Ally");
  AddFavourID(FAVOUR_TAME,"Song of Taming");
  AddFavourID(FAVOUR_TELEPCONTROL,"Control Warp");
  AddFavourID(FAVOUR_TELEPORT,"Teleport");
}

int god::CalcDebit(int iDebit,int iDefault){
  if(iDebit!=0){
    switch(iDebit){
      case FAVOURDEBIT_AUTO:       iDebit=iDefault;   break;
      case FAVOURDEBIT_AUTOHALF:   iDebit=iDefault/2; break;
      case FAVOURDEBIT_AUTODOUBLE: iDebit=iDefault*2; break;
    }

    // can ask more favours if very well aligned
    if(game::GetPlayerAlignment() == game::GetGodAlignmentVsPlayer(this))
      iDebit/=2;

    /**
     * if enough time has passed, a normal pray could provide the favour freely
     * and even with relation benefits, so make it cheaper too, but not costless.
     */
    if(Timer==0)
      iDebit/=2; // /=3 too cheap?

    // skilled in manipulative praying :)
    iDebit -= game::GetPlayer()->GetAttribute(WISDOM);

    /**
     * max of 20 vafours (50*20=1000) (too much?)
     * in the best case (master prayer) only
     */
    if(iDebit<50)
      iDebit=50;
  }
  return iDebit;
}

void AddKnownSpell(std::vector<int>& ks,int iNew)
{
  for(auto pfsSpell = ks.begin(); pfsSpell != ks.end(); pfsSpell++){
    if(*pfsSpell == iNew)return;
  }
  ks.push_back(iNew);
}

bool FavourTeleport(god* G)
{
  if(!PLAYER->StateIsActivated(TELEPORT_LOCK))
  {
    ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
    game::AskForKeyPress(CONST_S("You teleport! [press any key to continue]"));
    PLAYER->Move(game::GetCurrentLevel()->GetRandomSquare(PLAYER), true);
    return true;
  }
  return false;
}

bool god::CallFavour(CallFavourType call, int iCallFavour, int iWhat, int iDebit, int iDbtDefault)
{
  if(iCallFavour!=iWhat)
    return false;

  if(iDebit==0) //came thru normal praying
    AddKnownSpell(knownSpellsID,iCallFavour);

  iDebit = CalcDebit(iDebit,iDbtDefault);

  if(iDebit>0)
    if(!god::Favour(iWhat,iDebit))
      return false;

  bool bWorked = false;
  if((*call)(this)){
    if(iDebit>0){ //was a favour
      int iTm = 10000 - Relation*10; //by reaching here, Relation is always > 0
      if(iTm<1000)iTm=1000;
      AdjustTimer(iTm); // this is a kind of debit too (counts against next safe pray time)

      LastPray=0; // to make it count as a pray too

      Relation-=iDebit;
    }
    bWorked = true;
  }

  fsLastKnownRelation = PrintRelation();
  return bWorked;
}

/**
 *
 * @param fsWhat
 * @param iDebit if -1 will be automatic
 * @return
 */
bool sophos::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourTeleport,FAVOUR_TELEPORT,iWhat,iDebit,100))
    return true;
  return false;
}

void sophos::PrayGoodEffect()
{
  truth DidHelp = false;

  DidHelp = Favour(FAVOUR_TELEPORT);

  // Give a little attribute experience (Cha already given by Dulcis and not Wis,
  // as we want to check Wis to give the experience).
  if(PLAYER->GetAttribute(WISDOM) > RAND_128)
  {
    cchar* SecretType;
    int Experience = Min(200, Max(50, GetRelation() / 4));

    switch(RAND() % 3)
    {
      case 0:
       SecretType = "an ancient";
       PLAYER->EditExperience(INTELLIGENCE, Experience, 1 << 10);
       break;
      case 1:
       SecretType = "a terrible";
       PLAYER->EditExperience(WILL_POWER, Experience, 1 << 10);
       break;
      case 2:
       SecretType = "a profound";
       PLAYER->EditExperience(MANA, Experience, 1 << 10);
       break;
      default:
       SecretType = "a weird and disturbing";
       break;
    }

    ADD_MESSAGE("%s whispers %s secret to you.", GetName(), SecretType);
    DidHelp = true;
  }

  //TODO: If still didn't help, reveal a bit of the level? Or detect material?

  if(!DidHelp)
    ADD_MESSAGE("You hear a booming voice: \"Alas, I cannot help thee, mortal.\"");

  return;
}

void sophos::PrayBadEffect()
{
  ADD_MESSAGE("Suddenly, the fabric of space experiences an unnaturally powerful quantum displacement!");
  PLAYER->TeleportSomePartsAway(1 + (RAND() & 1));
  PLAYER->CheckDeath(CONST_S("shattered to pieces by the wrath of ") + GetName(), 0);
}

bool FavourHolyGrenade(god* G)
{
    ADD_MESSAGE("You hear a booming voice: \"I GRANT THEE THIS HOLY HAND GRENADE "
                "THAT WITH IT THOU MAYEST BLOW THY ENEMIES TO TINY BITS, MY PALADIN!\"");
    PLAYER->GetGiftStack()->AddItem(holyhandgrenade::Spawn());
    return true;
}

bool valpurus::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourHolyGrenade,FAVOUR_HOLYGREN,iWhat,iDebit,300))return true;
  return false;
}

void valpurus::PrayGoodEffect()
{
  if(!game::PlayerIsGodChampion())
  {
    ADD_MESSAGE("You hear a booming voice: \"I RECOGNIZETH THEE AS MINE OWN CHAMPION! "
                "JOURNEY FORTH WITH THESE ARMAMENTS TO DEFEAT MORTIFER AND ALL "
                "THE CHAOS HE HADST SOWN!\" A set of holy arms appear from nothing.");

    meleeweapon* Weapon = meleeweapon::Spawn(TWO_HANDED_SWORD);
    Weapon->InitMaterials(MAKE_MATERIAL(VALPURIUM), MAKE_MATERIAL(VALPURIUM), true);
    PLAYER->GetGiftStack()->AddItem(Weapon);

    shield* Shield = shield::Spawn();
    Shield->InitMaterials(MAKE_MATERIAL(VALPURIUM));
    PLAYER->GetGiftStack()->AddItem(Shield);

    game::MakePlayerGodChampion();
  }
  else // Player already received championship gift, give holy handgrenade instead.
  {
    Favour(FAVOUR_HOLYGREN);
  }
}

void valpurus::PrayBadEffect()
{
  ADD_MESSAGE("Valpurus smites you with a small hammer.");
  PLAYER->ReceiveDamage(0, 10, PHYSICAL_DAMAGE, HEAD, RAND() & 7);
  PLAYER->CheckDeath(CONST_S("faced the hammer of Justice from the hand of ") + GetName(), 0);
}

bool FavourFirestorm(god* G)
{
  // I think this is a remnant of past development that you call upon Inlux rather than Legifer. --red_kangaroo
  // No, my bad. Inlux is an anagram of Linux, which will hopefully save us from the horrid Bill. ;)
  ADD_MESSAGE("A booming voice echoes: \"Inlux! Inlux! Save us!\" A huge firestorm engulfs everything around you.");
  //ADD_MESSAGE("You are surrounded by the righteous flames of %s.", GetName());
  game::GetCurrentLevel()->Explosion(PLAYER, CONST_S("killed by the holy flames of ") + G->GetName(), PLAYER->GetPos(),
                                     (Max(20 * PLAYER->GetAttribute(WISDOM), 1) + Max(G->GetRelation(), 0)) >> 3, false);
  return true;
}

bool legifer::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourFirestorm,FAVOUR_FIRESTORM,iWhat,iDebit,200))return true;
  return false;
}

void legifer::PrayGoodEffect()
{
  Favour(FAVOUR_FIRESTORM);
}

void legifer::PrayBadEffect()
{
  ADD_MESSAGE("%s casts horrible yet righteous flames upon you.", GetName());
  PLAYER->ReceiveDamage(0, 50 + RAND() % 50, FIRE, ALL);
  PLAYER->CheckDeath(CONST_S("burned to death by the holy flames of ") + GetName(), 0);
}

bool FavourExtinguishFire(god* G)
{
  PLAYER->Extinguish(true);
  return true;
}

bool FavourTame(god* G)
{
  bool HasHelped = false;

  for(int d = 0; d < PLAYER->GetNeighbourSquares(); ++d)
  {
    square* Square = PLAYER->GetNeighbourSquare(d);

    if(Square)
    {
      character* Char = Square->GetCharacter();

      if(Char)
      {
        if(Char->CanHear())
          if(Char->CanTameWithDulcis(PLAYER))
          {
            if(Char->GetTeam() == PLAYER->GetTeam())
              ADD_MESSAGE("%s seems to be very happy.", Char->CHAR_DESCRIPTION(DEFINITE));
            else if(Char->GetRelation(PLAYER) == HOSTILE)
            {
              ADD_MESSAGE("%s stops fighting.", Char->CHAR_DESCRIPTION(DEFINITE));
              HasHelped = true;
            }
            else
              ADD_MESSAGE("%s seems to be very friendly towards you.", Char->CHAR_DESCRIPTION(DEFINITE));

            Char->ChangeTeam(PLAYER->GetTeam());
          }
          else
          {
            ADD_MESSAGE("%s resists its charming call.", Char->CHAR_DESCRIPTION(DEFINITE));
            if(Char->GetRelation(PLAYER) == HOSTILE)
              HasHelped = true;
          }
        else
        {
          ADD_MESSAGE("%s seems not affected.", Char->CHAR_DESCRIPTION(DEFINITE));
          if(Char->GetRelation(PLAYER) == HOSTILE)
              HasHelped = true;
        }
      }
    }
  }

  return HasHelped;
}

bool dulcis::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourExtinguishFire,FAVOUR_EXTINGUISHFIRE,iWhat,iDebit,50))return true;
  if(CallFavour(&FavourTame,FAVOUR_TAME,iWhat,iDebit,250))return true;
  return false;
}

void dulcis::PrayGoodEffect()
{
  truth HasHelped = false;

  for(int d = 0; d < PLAYER->GetNeighbourSquares(); ++d)
  {
    square* Square = PLAYER->GetNeighbourSquare(d);

    if(Square)
    {
      character* Char = Square->GetCharacter();

      if(Char)
        if(Char->IsBurning())
          if(Char->GetTeam() == PLAYER->GetTeam())
          {
            Char->Extinguish(true);
            HasHelped = true;
          }
    }
  }
  if(PLAYER->IsBurning())
  {
    Favour(FAVOUR_EXTINGUISHFIRE);
    if(HasHelped)
      ADD_MESSAGE("Dulcis helps you and your companions to put out the flames.");
    else
      ADD_MESSAGE("Dulcis helps you to put out the flames.");

    HasHelped = true;
  }
  else if(HasHelped)
    ADD_MESSAGE("Dulcis helps your companions to put out the flames.");
  if(HasHelped)
    return;
  else
    ADD_MESSAGE("A beautiful melody echoes around you.");

  HasHelped = Favour(FAVOUR_TAME);
  if(HasHelped)
    return;

  if (GetRelation() >= 50)
  {
     ADD_MESSAGE("You feel the music resonate within you.", GetName());
     int Experience = Min(200, Max(75, GetRelation()/4));
     PLAYER->EditExperience(CHARISMA, Experience, 33335);
  }
}

void dulcis::PrayBadEffect()
{
  ADD_MESSAGE("%s plays a horrible tune that rots your brain.", GetName());
  PLAYER->ReceiveDamage(0, 1 + RAND() % 9, SOUND, HEAD);
  PLAYER->CheckDeath(CONST_S("became insane by listening ") + GetName() + " too much", 0);
}

bool FavourCureWounds(god* G)
{
    ADD_MESSAGE("%s cures your wounds.", G->GetName());
    PLAYER->RestoreLivingHP();
    return true;
}
bool FavourCurePoison(god* G)
{
    ADD_MESSAGE("%s removes the foul liquid in your veins.", G->GetName());
    PLAYER->DeActivateTemporaryState(POISONED);
    return true;
}
bool FavourCureLeprosy(god* G)
{
    ADD_MESSAGE("%s cures your leprosy.", G->GetName());
    PLAYER->DeActivateTemporaryState(LEPROSY);
    return true;
}
bool FavourCureLycanthropy(god* G)
{
    ADD_MESSAGE("%s cures your animalistic urges.", G->GetName());
    PLAYER->DeActivateTemporaryState(LYCANTHROPY);
    return true;
}
bool FavourCureTapeworm(god* G)
{
    ADD_MESSAGE("%s removes the evil hidden in your guts.", G->GetName());
    PLAYER->DeActivateTemporaryState(PARASITE_TAPE_WORM);
    return true;
}
bool FavourCureMindworm(god* G)
{
    ADD_MESSAGE("%s removes the evil hidden in your brain.", G->GetName());
    PLAYER->DeActivateTemporaryState(PARASITE_MIND_WORM);
    return true;
}
bool FavourHealBurns(god* G)
{
    ADD_MESSAGE("%s heals your burns.", G->GetName());
    //PLAYER->RemoveBurns(); // removes the burns and restores HP
    if(!PLAYER->IsBurning()) // the player would do well to put the flames out himself first
      PLAYER->ResetThermalEnergies();
    PLAYER->ResetLivingBurning(); // In keeping with Seges' au natural theme. Does roughly the same as RemoveBurns(),
                                  // only without the message(?) and it resets the burn level counter
    return true;
}
bool FavourInvigorate(god* G)
{
    ADD_MESSAGE("You don't feel a bit tired anymore.");
    PLAYER->RestoreStamina();
    return true;
}
bool FavourFeed(god* G)
{
    if(dynamic_cast<seges*>(G)){
      ADD_MESSAGE("Your stomach feels full again.");
      PLAYER->SetNP(BLOATED_LEVEL);
    }else{
      if(dynamic_cast<silva*>(G))
        ADD_MESSAGE("%s feeds you fruits and wild berries.", G->GetName());

      if(dynamic_cast<nefas*>(G))
        ADD_MESSAGE("%s breast-feeds you.", G->GetName());

      PLAYER->SetNP(SATIATED_LEVEL);
    }

    return true;
}
bool FavourCureVampirism(god* G)
{
    ADD_MESSAGE("%s cures your bloodlust.", G->GetName());
    PLAYER->DeActivateTemporaryState(VAMPIRISM);
    return true;
}

bool seges::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourCureWounds,FAVOUR_CUREWOUNDS,iWhat,iDebit,150))return true;
  if(CallFavour(&FavourCurePoison,FAVOUR_CUREPOISON,iWhat,iDebit,200))return true;
  if(CallFavour(&FavourCureLeprosy,FAVOUR_CURELEPROSY,iWhat,iDebit,250))return true;
  if(CallFavour(&FavourCureLycanthropy,FAVOUR_CURELYCANTHROPY,iWhat,iDebit,300))return true;
  if(CallFavour(&FavourCureVampirism,FAVOUR_CUREVAMP,iWhat,iDebit,100))return true;
  if(CallFavour(&FavourCureTapeworm,FAVOUR_CURETAPEWORM,iWhat,iDebit,250))return true;
  if(CallFavour(&FavourCureMindworm,FAVOUR_CUREMINDWORM,iWhat,iDebit,500))return true;
  if(CallFavour(&FavourFeed,FAVOUR_FEED,iWhat,iDebit,300))return true; //bloats
  if(CallFavour(&FavourHealBurns,FAVOUR_HEALBURNS,iWhat,iDebit,50))return true;
  if(CallFavour(&FavourInvigorate,FAVOUR_INVIGORATE,iWhat,iDebit,250))return true;
  return false;
}

void seges::PrayGoodEffect()
{
  if(PLAYER->IsInBadCondition())
  {
    Favour(FAVOUR_CUREWOUNDS);
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(POISONED))
  {
    Favour(FAVOUR_CUREPOISON);
    return;
  }

  if(PLAYER->StateIsActivated(LEPROSY))
  {
    Favour(FAVOUR_CURELEPROSY);
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(LYCANTHROPY))
  {
    Favour(FAVOUR_CURELYCANTHROPY);
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(VAMPIRISM))
  {
    Favour(FAVOUR_CUREVAMP);
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(PARASITE_TAPE_WORM))
  {
    Favour(FAVOUR_CURETAPEWORM);
    return;
  }

  if(PLAYER->TemporaryStateIsActivated(PARASITE_MIND_WORM))
  {
    Favour(FAVOUR_CUREMINDWORM);
    return;
  }

  if(PLAYER->GetNP() < SATIATED_LEVEL)
  {
    Favour(FAVOUR_FEED);
    return;
  }

  if(PLAYER->IsBurnt())
  {
    Favour(FAVOUR_HEALBURNS);
    return;
  }

  // Always return at least some message.
  Favour(FAVOUR_INVIGORATE);
  return;
}

void seges::PrayBadEffect()
{
  if(PLAYER->UsesNutrition())
  {
    ADD_MESSAGE("You feel Seges altering the contents of your stomach in an eerie way.");
    PLAYER->EditNP(-100000 / Max(PLAYER->GetAttribute(WISDOM), 1));
    PLAYER->CheckStarvationDeath(CONST_S("starved by ") + GetName());
  }
  else
    ADD_MESSAGE("Seges tries to alter the contents of your stomach, but fails.");
}

bool FavourEnchantEquipment(god* G)
{
  item* Enchantable;
  item* PairEnchantable;
  int LowEnchant = 99;
  truth Pair = false;

  for(int c = 0; c < PLAYER->GetEquipments(); ++c)
  {
    item* Equipment = PLAYER->GetEquipment(c);

    if(Equipment && Equipment->CanBeEnchanted() && !Equipment->IsWeapon(PLAYER)
        && (Equipment->GetEnchantment() < LowEnchant))
    {
      Enchantable = Equipment;
      LowEnchant = Enchantable->GetEnchantment();
      Pair = false;
      continue;
    }

    if(Enchantable && Equipment && Equipment->HandleInPairs()
        && Equipment->CanBePiledWith(Enchantable, PLAYER))
    {
      Pair = true;
      PairEnchantable = Equipment;
    }
  }
  if(LowEnchant < 99)
  {
    int EnchDiff = ((Enchantable->GetEnchantment()+2)*250 - G->GetRelation()) / 50;
    if(EnchDiff <= 1 || !RAND_N(EnchDiff)) {
      if(Pair)
      {
        ADD_MESSAGE("Your %s glow briefly blue. They feel very warm now.", Enchantable->CHAR_NAME(PLURAL));
        Enchantable->EditEnchantment(1);
        PairEnchantable->EditEnchantment(1);
      }
      else
      {
        ADD_MESSAGE("Your %s glows briefly blue. It feels very warm now.", Enchantable->CHAR_NAME(UNARTICLED));
        Enchantable->EditEnchantment(1);
      }
      return true;
    }
  }

  return false;
}

bool atavus::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourEnchantEquipment,FAVOUR_ENCHANT,iWhat,iDebit,250))return true;
  return false;
}

void atavus::PrayGoodEffect()
{
  if(Favour(FAVOUR_ENCHANT))return;
  ADD_MESSAGE("You feel that %s is watching your actions closely.", GetName());
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
      ADD_MESSAGE("%s tries to remove your %s, but fails. You feel you are not so gifted anymore.",
                  GetName(), Disappearing->CHAR_NAME(UNARTICLED));
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

bool FavourCallRain(god* G)
{
    beamdata Beam
      (
        0,
        CONST_S("drowned by the tears of ") + G->GetName(),
        YOURSELF,
        0
      );

    lsquare* Square = PLAYER->GetLSquareUnder();
    PLAYER->SpillFluid(0, liquid::Spawn(WATER, 400 + RAND() % 800));
    Square->LiquidRain(Beam, WATER);

    ADD_MESSAGE("Silva allows a little spell of gentle rain to pour down from above.");

    return true;
}

bool FavourEarthQuake(god* G)
{
  scrollofearthquake::EarthQuakeMagic();
  return true;
}

bool FavourSummonWolf(god* G)
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

    return true;
}

bool silva::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourFeed,FAVOUR_FEED,iWhat,iDebit,200))return true; //satiated
  if(CallFavour(&FavourCallRain,FAVOUR_CALLRAIN,iWhat,iDebit,75))return true;
  if(CallFavour(&FavourEarthQuake,FAVOUR_EARTHQUAKE,iWhat,iDebit,500))return true;
  if(CallFavour(&FavourSummonWolf,FAVOUR_SUMMONWOLF,iWhat,iDebit,250))return true;

  return false;
}

void silva::PrayGoodEffect()
{
  if(PLAYER->GetNP() < HUNGER_LEVEL)
  {
    Favour(FAVOUR_FEED);
  }

  if(PLAYER->IsBurning() || PLAYER->PossessesItem(&item::IsOnFire))
  {
    Favour(FAVOUR_CALLRAIN);
  }
  else if(!game::GetCurrentLevel()->IsOnGround())
  {
    Favour(FAVOUR_EARTHQUAKE);
  }
  else
  {
    Favour(FAVOUR_SUMMONWOLF);
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

bool FavourFixEquipment(god* G)
{
    for(int c = 0; c < PLAYER->GetEquipments(); ++c)
    {
      item* Equipment = PLAYER->GetEquipment(c);

      if(Equipment && Equipment->IsBroken())
      {
        ADD_MESSAGE("%s fixes your %s.", G->GetName(), Equipment->CHAR_NAME(UNARTICLED));
        Equipment->Fix();
        break;
      }
    }

  return true;
}

bool FavourStopFire(god* G)
{
    for(int c = 0; c < PLAYER->GetEquipments(); ++c)
    {
      item* Equipment = PLAYER->GetEquipment(c);

      if(Equipment && Equipment->IsBurnt())
      {
        ADD_MESSAGE("%s repairs the burns on your %s.", G->GetName(), Equipment->CHAR_NAME(UNARTICLED));
        Equipment->RemoveBurns();
        if(!Equipment->IsBurning())
          Equipment->ResetThermalEnergies();
        Equipment->ResetBurning();
        break;
      }
    }

    return true;
}

bool loricatus::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourFixEquipment,FAVOUR_FIXEQUIPMENT,iWhat,iDebit,250))return true;
  if(CallFavour(&FavourStopFire,FAVOUR_STOPFIRE,iWhat,iDebit,50))return true;

  return false;
}

void loricatus::PrayGoodEffect()
{
  item* MainWielded = PLAYER->GetMainWielded();

  if(MainWielded)
  {
    if(PLAYER->GetLSquareUnder()->IsDark()
       && (MainWielded->GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
       && MainWielded->CanBeBurned())
    {
      if(MainWielded->TestActivationEnergy(260 + GetRelation() / 2))
      {
        ADD_MESSAGE("\"Behold, a light in the dark places!\"");
        return;
      }
    }
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

          if(MainWielded->HandleInPairs()
             && (SecondaryWielded = PLAYER->GetSecondaryWielded())
             && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
          {
            MainWielded->AddName(Desc, PLURAL);
            Desc << " glow and sparkle like they were";

            if(SecondaryWielded->GetSecondaryMaterial()
               && SecondaryWielded->GetSecondaryMaterial()->IsSameAs(MainWielded->GetMainMaterial()))
              delete SecondaryWielded->SetSecondaryMaterial(MAKE_MATERIAL(Config));

            delete SecondaryWielded->SetMainMaterial(MAKE_MATERIAL(Config));
          }
          else
          {
            MainWielded->AddName(Desc, UNARTICLED);
            Desc << " glows and sparkles like it was";
          }

          if(MainWielded->GetSecondaryMaterial()
             && MainWielded->GetSecondaryMaterial()->IsSameAs(MainWielded->GetMainMaterial()))
            delete MainWielded->SetSecondaryMaterial(MAKE_MATERIAL(Config));

          delete MainWielded->SetMainMaterial(MAKE_MATERIAL(Config));
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

  if(Favour(FAVOUR_FIXEQUIPMENT))
    return;

  if(Favour(FAVOUR_STOPFIRE))
    return;

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

      if(MainWielded->HandleInPairs()
         && (SecondaryWielded = PLAYER->GetSecondaryWielded())
         && SecondaryWielded->CanBePiledWith(MainWielded, PLAYER))
      {
        MainWielded->AddName(Desc, PLURAL);
        Desc << " vibrate and soften";
        delete SecondaryWielded->SetMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
      }
      else
      {
        MainWielded->AddName(Desc, UNARTICLED);
        Desc << " vibrates and softens";
      }

      delete MainWielded->SetMainMaterial(MAKE_MATERIAL(BANANA_FLESH));
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

int CalcDuration(god* G)
{
  if(dynamic_cast<cleptia*>(G))
    return 200 * PLAYER->GetAttribute(WISDOM) + Max(G->GetRelation(), 0);

  if(dynamic_cast<scabies*>(G) || dynamic_cast<infuscor*>(G))
    return 300 * PLAYER->GetAttribute(WISDOM) + G->GetRelation() * 5;

  ABORT("duration calc for god %s is not available here!",G->GetName());
}
bool FavourCureSlowness(god* G)
{
    ADD_MESSAGE("%s restores the swiftness of your movement.", G->GetName());
    PLAYER->DeActivateTemporaryState(SLOW);
    return true;
}
bool FavourSpeedUp(god* G)
{
    int Duration = CalcDuration(G);
    ADD_MESSAGE("%s gives you the talent for speed.", G->GetName());
    PLAYER->BeginTemporaryState(HASTE, Duration);
    return true;
}
bool FavourInvisible(god* G)
{
    int Duration = CalcDuration(G);
    ADD_MESSAGE("%s hides you from your enemies.", G->GetName());
    PLAYER->BeginTemporaryState(INVISIBLE, Duration);
    return true;
}
bool FavourInfravision(god* G)
{
    int Duration = CalcDuration(G);
    ADD_MESSAGE("%s orders darkness to hinder you no more.", G->GetName());
    PLAYER->BeginTemporaryState(INFRA_VISION, Duration);
    return true;
}

bool cleptia::Favour(int iWhat, int iDebit)
{
  if(CallFavour(FavourCureSlowness,FAVOUR_CURESLOWNESS,iWhat,iDebit,100))return true;
  if(CallFavour(FavourSpeedUp,FAVOUR_SPEEDUP,iWhat,iDebit,150))return true;
  if(CallFavour(FavourInvisible,FAVOUR_INVISIBILITY,iWhat,iDebit,250))return true;
  if(CallFavour(FavourInfravision,FAVOUR_INFRAVISION,iWhat,iDebit,150))return true;
  return false;
}

void cleptia::PrayGoodEffect()
{
  PLAYER->RestoreStamina();

  if(PLAYER->StateIsActivated(SLOW))
  {
    Favour(FAVOUR_CURESLOWNESS);
    return;
  }

  if(!PLAYER->StateIsActivated(HASTE))
  {
    Favour(FAVOUR_SPEEDUP);
    return;
  }

  if(!PLAYER->StateIsActivated(INVISIBLE))
  {
    Favour(FAVOUR_INVISIBILITY);
    return;
  }

  if(!PLAYER->StateIsActivated(INFRA_VISION))
  {
    Favour(FAVOUR_INFRAVISION);
    return;
  }

  // Nothing else helped so far.
  int Experience = Min(200, Max(50, GetRelation() / 4));
  int WhichAttribute = RAND() & 1 ? DEXTERITY : AGILITY;
  PLAYER->EditExperience(WhichAttribute, Experience, 1 << 10);
  ADD_MESSAGE("%s helps you, but you really don't know how.", GetName());
  return;
}

void cleptia::PrayBadEffect()
{
  ADD_MESSAGE("%s slows you down.", GetName());
  PLAYER->BeginTemporaryState(SLOW, 250);
}

bool FavourEtherealMov(god* G)
{
    ADD_MESSAGE("The air suddenly feels much colder. A terrible undead voice shreds "
                "the silence: \"I aM PlEaSeD By tHy sQuIrMiNg, WoRm! WaLkEtH WiTh mE "
                "ThRoUgH ThE ShAdOwS As oNe oF ThE DeAd!\"");

    if(!PLAYER->StateIsActivated(ETHEREAL_MOVING))
      PLAYER->BeginTemporaryState(ETHEREAL_MOVING, PLAYER->GetAttribute(WISDOM) * 300);
    else
      PLAYER->EditTemporaryStateCounter(ETHEREAL_MOVING,
        PLAYER->GetTemporaryStateCounter(ETHEREAL_MOVING) + (PLAYER->GetAttribute(WISDOM) * 100));

    return true;
}

bool mortifer::Favour(int iWhat, int iDebit)
{
  if(CallFavour(FavourEtherealMov,FAVOUR_ETHEREALMOV,iWhat,iDebit,350))return true;
  return false;
}

void mortifer::PrayGoodEffect()
{
  if(!game::PlayerIsGodChampion())
  {
    ADD_MESSAGE("The air vibrates violently as a terrible undead voice echoes "
                "around you: \"SlAvE! ThOu HaSt PlAeSeD mE! lIfT tHy ReWaRd, "
                "ChAmPiOn!\" A heavy weapon of pure corruption materializes before you.");

    PLAYER->GetGiftStack()->AddItem(neercseulb::Spawn());
    game::MakePlayerGodChampion();
  }
  else
  {
    Favour(FAVOUR_ETHEREALMOV);
  }
}

void mortifer::PrayBadEffect()
{
  ADD_MESSAGE("A dark, booming voice shakes the area: \"PuNy MoRtAl! ThOu ArT nOt WoRtHy! "
              "I sHaLl dEsTrOy ThEe LiKe EvErYoNe ElSe!\" A bolt of black energy hits you.");

  PLAYER->ReceiveDamage(0, 1 + RAND() % 20, ENERGY, ALL);
  PLAYER->EditAttribute(AGILITY, -1);
  PLAYER->EditAttribute(ARM_STRENGTH, -1);
  PLAYER->EditAttribute(ENDURANCE, -1);
  PLAYER->CheckDeath(CONST_S("obliterated by the unholy power of ") + GetName(), 0);
}

bool FavourShopping(god* G)
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

  for(int c = 0; !OKItems.empty() && c < 4; ++c)
  {
    item* ToBeDeleted = OKItems[RAND() % OKItems.size()];
    NewVersion = ToBeDeleted->BetterVersion();
    ADD_MESSAGE("%s manages to trade %s into %s.", G->GetName(),
                ToBeDeleted->CHAR_NAME(DEFINITE), NewVersion->CHAR_NAME(INDEFINITE));
    PLAYER->GetStack()->AddItem(NewVersion);
    ToBeDeleted->RemoveFromSlot();
    ToBeDeleted->SendToHell();
    OKItems.erase(std::find(OKItems.begin(), OKItems.end(), ToBeDeleted));
  }

  return Success;
}
bool mellis::Favour(int iWhat, int iDebit)
{
  if(CallFavour(FavourShopping,FAVOUR_SHOPPING,iWhat,iDebit,250))return true;
  return false;
}

void mellis::PrayGoodEffect()
{
  truth Success = false;

  if(!RAND_2)
  {
    Success = Favour(FAVOUR_SHOPPING);
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
      ADD_MESSAGE("%s shares his knowledge of %s, the %s.", GetName(),
                  game::GetGod(NewKnownGod)->GetName(), game::GetGod(NewKnownGod)->GetDescription());
      return;
    }
  }

  if(!Success)
  {
    PLAYER->SetMoney(PLAYER->GetMoney() + (PLAYER->GetAttribute(WISDOM) * 2));
    ADD_MESSAGE("%s gives you some pocket money.", GetName());
  }
  return;
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
  LastPray = 0;
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
  LastPray = 0;
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
  truth Success = false;
  if(GetRelation() < -200)
  {
    uint c;

    for(c = 1; c < uint(PLAYER->GetBodyParts()); ++c) // annoying :(
    {
      bodypart* BodyPart = PLAYER->GetBodyPart(c);

      if(BodyPart && BodyPart->IsDestroyable(PLAYER))
        if(BodyPart->GetMainMaterial())
          if(BodyPart->CanBeBurned()
             && (BodyPart->GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
             && !BodyPart->IsBurning())
          {
            if(BodyPart->TestActivationEnergy(50))
            {
                Success = true;
            }
          }
    }
    if(Success)
      ADD_MESSAGE("\"I'm going to enjoy watching you burn, insolent mortal!\"");
  }

  ADD_MESSAGE("%s pumps vile and evil knowledge into your brain. It's too much for you to handle and you faint.", GetName());
  PLAYER->LoseConsciousness(1000 + RAND_N(1000));
}

bool FavourConfusion(god* G)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  truth AudiencePresent = false;

  for(int x = Rect.X1; x <= Rect.X2; ++x)
  {
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

      if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(CONFUSED)
         && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
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

        if(Audience && !Audience->TemporaryStateIsActivated(CONFUSED)
           && Audience->CanBeConfused() && PLAYER->GetRelation(Audience) == HOSTILE)
        {
          if(Audience->CanBeSeenByPlayer())
            ADD_MESSAGE("%s confuses %s with her sweet lies.", G->GetName(), Audience->CHAR_NAME(DEFINITE));

          Audience->BeginTemporaryState(CONFUSED, 30 * PLAYER->GetAttribute(WISDOM) + RAND() % 500);
        }
      }
  }

  return true;
}
bool nefas::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourFeed,FAVOUR_FEED,iWhat,iDebit,200))return true; //satiated
  if(CallFavour(&FavourConfusion,FAVOUR_CONFUSE,iWhat,iDebit,200))return true; //satiated
  return false;
}

void nefas::PrayGoodEffect()
{
  if(PLAYER->GetNP() < HUNGER_LEVEL)
  {
    Favour(FAVOUR_FEED);
    return;
  }

  Favour(FAVOUR_CONFUSE);

  if((GetRelation() > 200) && RAND_N(5)) {
    int Chief = 3000/GetRelation();

    mistress* Mistress = mistress::Spawn(RAND_N(Chief) ? 0 : TORTURING_CHIEF);
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
  else {
    ADD_MESSAGE("You hear a sweet voice inside your head: \"Enjoy, mortal!\".");
    potion* Bottle = potion::Spawn(0, NO_MATERIALS);
    Bottle->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(VODKA));
    PLAYER->GetGiftStack()->AddItem(Bottle);
    ADD_MESSAGE("%s drops from nowhere.", Bottle->CHAR_DESCRIPTION(INDEFINITE));
  }
}

void nefas::PrayBadEffect()
{
  ADD_MESSAGE("A potion drops on your head and shatters into small bits.");
  PLAYER->ReceiveDamage(0, 2 + RAND() % 7, PHYSICAL_DAMAGE, HEAD);
  PLAYER->GetStackUnder()->AddItem(brokenbottle::Spawn());
  PLAYER->CheckDeath(CONST_S("killed while enjoying the company of ") + GetName(), 0);
}

bool FavourDiseaseImmunity(god* G)
{
  int Duration = CalcDuration(G);
  PLAYER->BeginTemporaryState(DISEASE_IMMUNITY, Duration);
  ADD_MESSAGE("%s chuckles in your mind: \"No need to fall apart, my dear.\"", G->GetName());
  return true;
}
bool scabies::Favour(int iWhat, int iDebit)
{
  if(CallFavour(&FavourDiseaseImmunity,FAVOUR_DISEASEIMMUNITY,iWhat,iDebit,350))return true; //satiated
  return false;
}

void scabies::PrayGoodEffect()
{
  if(PLAYER->IsImmuneToLeprosy()) // Spread leprosy whenever you won't harm your followers.
  {
    for(int c = 0; c < game::GetTeams(); ++c)
      if(PLAYER->GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
        for(character* Char : game::GetTeam(c)->GetMember())
        {
          if(Char->IsEnabled() && !Char->IsImmuneToLeprosy())
            Char->GainIntrinsic(LEPROSY);
        }

    ADD_MESSAGE("You feel a horrible disease spreading.");
  }

  int Duration = CalcDuration(this);

  if((PLAYER->GetNP() < HUNGER_LEVEL) &&
     (!PLAYER->StateIsActivated(FASTING) || PLAYER->GetTemporaryStateCounter(FASTING) < Duration))
  {
    if(!PLAYER->StateIsActivated(FASTING))
      PLAYER->BeginTemporaryState(FASTING, Duration);
    else
      PLAYER->EditTemporaryStateCounter(FASTING, PLAYER->GetTemporaryStateCounter(FASTING) + Duration);
    ADD_MESSAGE("%s whispers in your mind: \"Famine culls the weak but purifies the strong, my child.\"", GetName());
    return;
  }

  // Scabies wants followers who can spread her word, not those who just lie on thr ground, missing limbs.
  if(PLAYER->StateIsActivated(LEPROSY) && !PLAYER->IsImmuneToLeprosy())
  {
    Favour(FAVOUR_DISEASEIMMUNITY);
    return;
  }

  truth Success = false;

  for(int d = 0; d < PLAYER->GetNeighbourSquares(); ++d)
  {
    lsquare* Square = PLAYER->GetNeighbourLSquare(d);

    if(Square && Square->GetCharacter() && Square->GetCharacter()->GetRelation(PLAYER) == HOSTILE)
    {
      if(!RAND_2)
      {
        ADD_MESSAGE("%s vomits poison on %s!", GetName(), Square->GetCharacter()->CHAR_DESCRIPTION(DEFINITE));
        Square->SpillFluid(PLAYER, liquid::Spawn(POISON_LIQUID, 30 * PLAYER->GetAttribute(WISDOM)));
        Success = true;
      }
      else
      {
        ADD_MESSAGE("%s vomits acid on %s!", GetName(), Square->GetCharacter()->CHAR_DESCRIPTION(DEFINITE));
        Square->SpillFluid(PLAYER, liquid::Spawn(SULPHURIC_ACID, 30 * PLAYER->GetAttribute(WISDOM)));
        Success = true;
      }
    }
  }

  // First try to deal with enemies, then give stuff.
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

  if(!Success)
    PLAYER->PolymorphRandomly(2500, 10000, 1000 + RAND() % 1000);
}

void scabies::PrayBadEffect()
{
  if(!RAND_2)
  {
    ADD_MESSAGE("%s makes you eat a LOT of school food.", GetName());
    material* SchoolFood = MAKE_MATERIAL(SCHOOL_FOOD, 2000);
    SchoolFood->EatEffect(PLAYER, 1000);
    delete SchoolFood;

    ADD_MESSAGE("You feel your muscles softening terribly...");
    PLAYER->EditAttribute(ARM_STRENGTH, -1);
    PLAYER->EditAttribute(DEXTERITY, -1);
  }
  else if(PLAYER->IsImmuneToLeprosy())
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

bool FavourBurnYourEnemies(god* G)
{
  truth Success = false;

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(),
      PLAYER->GetPos(), PLAYER->GetESPRange());

  for(int x = Rect.X1; x <= Rect.X2; ++x)
  {
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      character* Victim = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

      if(Victim && Victim->CanBeSeenByPlayer() && PLAYER->GetRelation(Victim) == HOSTILE)
      {
        uint c;
        truth Burned = false;

        for(c = 1; c < uint(Victim->GetBodyParts()); ++c) // annoying :(
        {
          bodypart* BodyPart = Victim->GetBodyPart(c);

          if(BodyPart && BodyPart->IsDestroyable(Victim)
              && BodyPart->GetMainMaterial() && BodyPart->CanBeBurned()
              && (BodyPart->GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
              && !BodyPart->IsBurning())
          {
            if(BodyPart->TestActivationEnergy(20 + G->GetRelation() / 10))
            {
              Success = true;
              Burned = true;
            }
          }
        }
        if(Burned)
          ADD_MESSAGE("%s savagely sets fire to %s!", G->GetName(), Victim->CHAR_DESCRIPTION(DEFINITE));
      }
    }
  }

  return Success;
}

int InfuscorFavourDuration = 0;
bool FavourFeelYourEnemies(god* G)
{
  if(!PLAYER->StateIsActivated(ESP))
    PLAYER->BeginTemporaryState(ESP, InfuscorFavourDuration);
  else
    PLAYER->EditTemporaryStateCounter(ESP, PLAYER->GetTemporaryStateCounter(ESP)+InfuscorFavourDuration);
  ADD_MESSAGE("You feel %s whisper in your mind.", G->GetName());
  return true;
}
bool FavourControlWhatYouAre(god* G)
{
  if(!PLAYER->StateIsActivated(POLYMORPH_CONTROL))
    PLAYER->BeginTemporaryState(POLYMORPH_CONTROL, InfuscorFavourDuration);
  else
    PLAYER->EditTemporaryStateCounter(POLYMORPH_CONTROL, PLAYER->GetTemporaryStateCounter(POLYMORPH_CONTROL)+InfuscorFavourDuration);
  ADD_MESSAGE("You feel %s gently touch your body.", G->GetName());
  return true;
}
bool FavourTeleportControl(god* G)
{
  if(!PLAYER->StateIsActivated(TELEPORT_CONTROL))
    PLAYER->BeginTemporaryState(TELEPORT_CONTROL, InfuscorFavourDuration);
  else
    PLAYER->EditTemporaryStateCounter(TELEPORT_CONTROL, PLAYER->GetTemporaryStateCounter(TELEPORT_CONTROL)+InfuscorFavourDuration);
  ADD_MESSAGE("You feel %s gently touch your soul.", G->GetName());
  return true;
}
bool infuscor::Favour(int iWhat, int iDebit)
{
  if(CallFavour(FavourBurnYourEnemies,FAVOUR_BURNENEMIES,iWhat,iDebit,200))return true;
  if(CallFavour(FavourFeelYourEnemies,FAVOUR_FEELENEMIES,iWhat,iDebit,250))return true;
  if(CallFavour(FavourControlWhatYouAre,FAVOUR_POLYCONTROL,iWhat,iDebit,300))return true;
  if(CallFavour(FavourTeleportControl,FAVOUR_TELEPCONTROL,iWhat,iDebit,300))return true;
  return false;
}

void infuscor::PrayGoodEffect()
{
  truth Success = Favour(FAVOUR_BURNENEMIES);

  if(!Success)
  {
    InfuscorFavourDuration = CalcDuration(this);

    if(!PLAYER->StateIsActivated(ESP) ||
        PLAYER->GetTemporaryStateCounter(ESP) < InfuscorFavourDuration)
    {
      Favour(FAVOUR_FEELENEMIES);
      return;
    }

    if(!PLAYER->StateIsActivated(POLYMORPH_CONTROL) ||
        PLAYER->GetTemporaryStateCounter(POLYMORPH_CONTROL) < InfuscorFavourDuration)
    {
      Favour(FAVOUR_POLYCONTROL);
      return;
    }

    if(!PLAYER->StateIsActivated(TELEPORT_CONTROL) ||
        PLAYER->GetTemporaryStateCounter(TELEPORT_CONTROL) < InfuscorFavourDuration)
    {
      Favour(FAVOUR_TELEPCONTROL);
      return;
    }

    ADD_MESSAGE("You feel %s smiling at you.", GetName());
  }

  return;
}

bool FavourEnrage(god* G)
{
  ADD_MESSAGE("%s snarls: \"Fight, you lousy coward!\"", G->GetName());
  PLAYER->DeActivateTemporaryState(PANIC);
  PLAYER->BeginTemporaryState(REGENERATION, 200 * PLAYER->GetAttribute(WISDOM) + G->GetRelation() * 3);
  PLAYER->BeginTemporaryState(FEARLESS, 200 * PLAYER->GetAttribute(WISDOM) + G->GetRelation() * 3);
  return true;
}
bool FavourCauseFear(god* G)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, game::GetCurrentLevel()->GetBorder(), PLAYER->GetPos(), 10);
  truth AudiencePresent = false;

  for(int x = Rect.X1; x <= Rect.X2; ++x)
  {
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      character* Audience = game::GetCurrentLevel()->GetSquare(x, y)->GetCharacter();

      if(Audience && Audience->CanBeSeenByPlayer() && !Audience->TemporaryStateIsActivated(PANIC)
          && !Audience->StateIsActivated(FEARLESS) && PLAYER->GetRelation(Audience) == HOSTILE)
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
           && Audience->GetPanicLevel() > (RAND() % (50 - Min(PLAYER->GetAttribute(WISDOM), 49))))
          Audience->BeginTemporaryState(PANIC, 30 * PLAYER->GetAttribute(WISDOM) + RAND() % 500);
      }

    return true;
  }

  return false;
}
bool cruentus::Favour(int iWhat, int iDebit)
{
  if(CallFavour(FavourEnrage,FAVOUR_ENRAGE,iWhat,iDebit,200))return true;
  if(CallFavour(FavourCauseFear,FAVOUR_CAUSEFEAR,iWhat,iDebit,500))return true;
  return false;
}

void cruentus::PrayGoodEffect()
{
  // Blood for the god of blood!
  if(!RAND_4 || Relation == 1000)
  {
    beamdata Beam
      (
        0,
        CONST_S("drowned by the blood of ") + GetName(),
        YOURSELF,
        0
      );
    lsquare* Square = PLAYER->GetLSquareUnder();
    Square->LiquidRain(Beam, BLOOD);

    if(PLAYER->HasHead())
      ADD_MESSAGE("A torrential rain of blood descends on your head.");
    else
      ADD_MESSAGE("A rain of blood drizzles all around you.");
  }

  // A little bit of healing, but only usable when panicked.
  if(PLAYER->StateIsActivated(PANIC))
  {
    Favour(FAVOUR_ENRAGE);
    return;
  }

  if(Favour(FAVOUR_CAUSEFEAR))
    return;

  if(!RAND_2)
  {
    item* Weapon = PLAYER->GetMainWielded();

    for(int i = 0; i < 2; i++)
    {
      if(Weapon && Weapon->IsWeapon(PLAYER) && Weapon->CanBeEnchanted())
      {
        int EnchDiff = (Weapon->GetEnchantment()*250 - GetRelation()) / 50;
        if (EnchDiff <= 1 || !RAND_N(EnchDiff))
        {
          ADD_MESSAGE("Your %s glows briefly red. It feels very warm now.", Weapon->CHAR_NAME(UNARTICLED));
          Weapon->EditEnchantment(1);
          return;
        }
      }
      Weapon = PLAYER->GetSecondaryWielded();
    }
  }

  if(RAND() & 3)
  {
    potion* Bottle = potion::Spawn(0, NO_MATERIALS);
    Bottle->InitMaterials(MAKE_MATERIAL(GLASS), MAKE_MATERIAL(TROLL_BLOOD));
    PLAYER->GetGiftStack()->AddItem(Bottle);
    ADD_MESSAGE("%s drops from nowhere.", Bottle->CHAR_DESCRIPTION(INDEFINITE));
  }
  else
  {
    ADD_MESSAGE("Cruentus recommends you to his master, Mortifer.");
    game::GetGod(MORTIFER)->AdjustRelation(100);
  }
  return;
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
        ADD_MESSAGE("%s tries to destroy your %s, but fails.",
                    GetName(), PLAYER->GetMainWielded()->CHAR_NAME(UNARTICLED));
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
  if(Relation == 1000)
  {
    ADD_MESSAGE("%s exclaims: \"You have pleased me greatly, darling!\"", GetName());

    if(PLAYER->GetMyVomitMaterial() != GREEN_SLIME)
    {
      PLAYER->SetNewVomitMaterial(GREEN_SLIME);
    }
  }
  else
  {
    ADD_MESSAGE("%s feels that you are indeed her follower.", GetName());
    AdjustRelation(1);
  }
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
