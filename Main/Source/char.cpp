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

/* Compiled through charset.cpp */

/* These statedata structs contain functions and values used for handling
 * states. Remember to update them. All normal states must have
 * PrintBeginMessage and PrintEndMessage functions and a Description string.
 * BeginHandler, EndHandler, Handler (called each tick) and IsAllowed are
 * optional, enter zero if the state doesn't need one. If the SECRET flag
 * is set, Description is not shown in the panel without magical means.
 * You can also set some source (SRC_*) and duration (DUR_*) flags, which
 * control whether the state can be randomly activated in certain situations.
 * These flags can be found in ivandef.h. RANDOMIZABLE sets all source
 * & duration flags at once. */

#include "hiteffect.h" //TODO move to charsset.cpp?

#define DBGMSG_V2
#include "dbgmsgproj.h"

struct statedata
{
  cchar* Description;
  int Flags;
  void (character::*PrintBeginMessage)() const;
  void (character::*PrintEndMessage)() const;
  void (character::*BeginHandler)();
  void (character::*EndHandler)();
  void (character::*Handler)();
  truth (character::*IsAllowed)() const;
  void (character::*SituationDangerModifier)(double&) const;
};

statedata StateData[STATES] =
{
  {
    "Polymorphed",
    NO_FLAGS,
    0,
    0,
    0,
    &character::EndPolymorph,
    0,
    0,
    0
  }, {
    "Hasted",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginHasteMessage,
    &character::PrintEndHasteMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Slowed",
    RANDOMIZABLE&~SRC_GOOD,
    &character::PrintBeginSlowMessage,
    &character::PrintEndSlowMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "PolyControl",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL|SRC_GOOD),
    &character::PrintBeginPolymorphControlMessage,
    &character::PrintEndPolymorphControlMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Life Saved",
    SECRET,
    &character::PrintBeginLifeSaveMessage,
    &character::PrintEndLifeSaveMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Lycanthropy",
    SECRET|SRC_FOUNTAIN|SRC_CONFUSE_READ|DUR_FLAGS,
    &character::PrintBeginLycanthropyMessage,
    &character::PrintEndLycanthropyMessage,
    0,
    0,
    &character::LycanthropyHandler,
    0,
    &character::LycanthropySituationDangerModifier
  }, {
    "Invisible",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginInvisibilityMessage,
    &character::PrintEndInvisibilityMessage,
    &character::BeginInvisibility, &character::EndInvisibility,
    0,
    0,
    0
  }, {
    "Infravision",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginInfraVisionMessage,
    &character::PrintEndInfraVisionMessage,
    &character::BeginInfraVision,
    &character::EndInfraVision,
    0,
    0,
    0
  }, {
    "ESP",
    RANDOMIZABLE&~SRC_EVIL,
    &character::PrintBeginESPMessage,
    &character::PrintEndESPMessage,
    &character::BeginESP,
    &character::EndESP,
    0,
    0,
    0
  }, {
    "Poisoned",
    DUR_TEMPORARY,
    &character::PrintBeginPoisonedMessage,
    &character::PrintEndPoisonedMessage,
    0,
    0,
    &character::PoisonedHandler,
    &character::CanBePoisoned,
    &character::PoisonedSituationDangerModifier
  }, {
    "Teleporting",
    SECRET|(RANDOMIZABLE&~(SRC_MUSHROOM|SRC_GOOD)),
    &character::PrintBeginTeleportMessage,
    &character::PrintEndTeleportMessage,
    0,
    0,
    &character::TeleportHandler,
    0,
    0
  }, {
    "Polymorphing",
    SECRET|(RANDOMIZABLE&~(SRC_MUSHROOM|SRC_GOOD)),
    &character::PrintBeginPolymorphMessage,
    &character::PrintEndPolymorphMessage,
    0,
    0,
    &character::PolymorphHandler,
    0,
    &character::PolymorphingSituationDangerModifier
  }, {
    "TeleControl",
    RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL),
    &character::PrintBeginTeleportControlMessage,
    &character::PrintEndTeleportControlMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Panicked",
    NO_FLAGS,
    &character::PrintBeginPanicMessage,
    &character::PrintEndPanicMessage,
    &character::BeginPanic,
    &character::EndPanic,
    0,
    &character::CanPanic,
    &character::PanicSituationDangerModifier
  }, {
    "Confused",
    SECRET|(RANDOMIZABLE&~(DUR_PERMANENT|SRC_GOOD)),
    &character::PrintBeginConfuseMessage,
    &character::PrintEndConfuseMessage,
    0,
    0,
    0,
    &character::CanBeConfused,
    &character::ConfusedSituationDangerModifier
  }, {
    "Parasite (tapeworm)",
    SECRET|(RANDOMIZABLE&~DUR_TEMPORARY),
    &character::PrintBeginParasitizedMessage,
    &character::PrintEndParasitizedMessage,
    0,
    0,
    &character::ParasitizedHandler,
    &character::CanBeParasitized,
    &character::ParasitizedSituationDangerModifier
  }, {
    "Searching",
    NO_FLAGS,
    &character::PrintBeginSearchingMessage,
    &character::PrintEndSearchingMessage,
    0,
    0,
    &character::SearchingHandler,
    0,
    0
  }, {
    "Gas Immunity",
    SECRET|(RANDOMIZABLE&~(SRC_GOOD|SRC_EVIL)),
    &character::PrintBeginGasImmunityMessage,
    &character::PrintEndGasImmunityMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Levitating",
    RANDOMIZABLE&~SRC_EVIL,
    &character::PrintBeginLevitationMessage,
    &character::PrintEndLevitationMessage,
    0,
    &character::EndLevitation,
    0,
    0,
    0
  }, {
    "Leprosy",
    SECRET|(RANDOMIZABLE&~DUR_TEMPORARY),
    &character::PrintBeginLeprosyMessage,
    &character::PrintEndLeprosyMessage,
    &character::BeginLeprosy,
    &character::EndLeprosy,
    &character::LeprosyHandler,
    0,
    &character::LeprosySituationDangerModifier
  }, {
    "Hiccups",
    SRC_FOUNTAIN|SRC_CONFUSE_READ|DUR_FLAGS,
    &character::PrintBeginHiccupsMessage,
    &character::PrintEndHiccupsMessage,
    0,
    0,
    &character::HiccupsHandler,
    0,
    &character::HiccupsSituationDangerModifier
  }, {
    "Ethereal",
    NO_FLAGS,
    &character::PrintBeginEtherealityMessage,
    &character::PrintEndEtherealityMessage,
    &character::BeginEthereality, &character::EndEthereality,
    0,
    0,
    0
  }, {
    "Vampirism",
    DUR_FLAGS,
    &character::PrintBeginVampirismMessage,
    &character::PrintEndVampirismMessage,
    0,
    0,
    &character::VampirismHandler,
    0,
    0
  }, {
    "Swimming",
    SECRET|(RANDOMIZABLE&~SRC_EVIL),
    &character::PrintBeginSwimmingMessage,
    &character::PrintEndSwimmingMessage,
    &character::BeginSwimming, &character::EndSwimming,
    0,
    0,
    0
  }, {
    "Detecting",
    SECRET|(RANDOMIZABLE&~(SRC_MUSHROOM|SRC_EVIL)),
    &character::PrintBeginDetectMessage,
    &character::PrintEndDetectMessage,
    0,
    0,
    &character::DetectHandler,
    0,
    0
  }, {
    "Polymorph Locked",
    SECRET|(RANDOMIZABLE&~SRC_EVIL),
    &character::PrintBeginPolymorphLockMessage,
    &character::PrintEndPolymorphLockMessage,
    0,
    0,
    &character::PolymorphLockHandler,
    0,
    0
  }, {
    "Regenerating",
    SECRET|(RANDOMIZABLE&~SRC_EVIL),
    &character::PrintBeginRegenerationMessage,
    &character::PrintEndRegenerationMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Disease Immunity",
    SECRET|(RANDOMIZABLE&~SRC_EVIL),
    &character::PrintBeginDiseaseImmunityMessage,
    &character::PrintEndDiseaseImmunityMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Teleport Locked",
    SECRET,
    &character::PrintBeginTeleportLockMessage,
    &character::PrintEndTeleportLockMessage,
    0,
    0,
    &character::TeleportLockHandler,
    0,
    0
  }, {
    "Fearless",
    RANDOMIZABLE&~SRC_EVIL,
    &character::PrintBeginFearlessMessage,
    &character::PrintEndFearlessMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Fasting",
    SECRET|(RANDOMIZABLE&~SRC_EVIL),
    &character::PrintBeginFastingMessage,
    &character::PrintEndFastingMessage,
    0,
    0,
    0,
    0,
    0
  }, {
    "Parasite (mindworm)",
    SECRET|(RANDOMIZABLE&~DUR_TEMPORARY),
    &character::PrintBeginMindwormedMessage,
    &character::PrintEndMindwormedMessage,
    0,
    0,
    &character::MindwormedHandler,
    &character::CanBeParasitized, // We are using TorsoIsAlive right now, but I think it's OK,
                                  // because with unliving torso, your head will not be much better for mind worms.
    &character::ParasitizedSituationDangerModifier
  }
};

characterprototype::characterprototype(const characterprototype* Base,
                                       characterspawner Spawner,
                                       charactercloner Cloner,
                                       cchar* ClassID)
: Base(Base), Spawner(Spawner), Cloner(Cloner), ClassID(ClassID)
{ Index = protocontainer<character>::Add(this); }
std::list<character*>::iterator character::GetTeamIterator()
{ return TeamIterator; }
void character::SetTeamIterator(std::list<character*>::iterator What)
{ TeamIterator = What; }
void character::CreateInitialEquipment(int SpecialFlags)
{ AddToInventory(DataBase->Inventory, SpecialFlags); }
void character::EditAP(long What)
{ AP = Limit<long>(AP + What, -12000, 1200); }
int character::GetRandomStepperBodyPart() const { return TORSO_INDEX; }
void character::GainIntrinsic(long What)
{ BeginTemporaryState(What, PERMANENT); }
truth character::IsUsingArms() const { return GetAttackStyle() & USE_ARMS; }
truth character::IsUsingLegs() const { return GetAttackStyle() & USE_LEGS; }
truth character::IsUsingHead() const { return GetAttackStyle() & USE_HEAD; }
void character::CalculateAllowedWeaponSkillCategories()
{ AllowedWeaponSkillCategories = MARTIAL_SKILL_CATEGORIES; }
festring character::GetBeVerb() const
{ return IsPlayer() ? CONST_S("are") : CONST_S("is"); }
void character::SetEndurance(int What)
{ BaseExperience[ENDURANCE] = What * EXP_MULTIPLIER; }
void character::SetPerception(int What)
{ BaseExperience[PERCEPTION] = What * EXP_MULTIPLIER; }
void character::SetIntelligence(int What)
{ BaseExperience[INTELLIGENCE] = What * EXP_MULTIPLIER; }
void character::SetWisdom(int What)
{ BaseExperience[WISDOM] = What * EXP_MULTIPLIER; }
void character::SetWillPower(int What)
{ BaseExperience[WILL_POWER] = What * EXP_MULTIPLIER; }
void character::SetCharisma(int What)
{ BaseExperience[CHARISMA] = What * EXP_MULTIPLIER; }
void character::SetMana(int What)
{ BaseExperience[MANA] = What * EXP_MULTIPLIER; }
truth character::IsOnGround() const
{ return MotherEntity && MotherEntity->IsOnGround(); }
truth character::LeftOversAreUnique() const
{ return GetArticleMode() || AssignedName.GetSize(); }
truth character::HomeDataIsValid() const
{ return (HomeData && HomeData->Level == GetLSquareUnder()->GetLevelIndex()
          && HomeData->Dungeon == GetLSquareUnder()->GetDungeonIndex()); }
void character::SetHomePos(v2 Pos) { HomeData->Pos = Pos; }
cchar* character::FirstPersonUnarmedHitVerb() const { return "hit"; }
cchar* character::FirstPersonCriticalUnarmedHitVerb() const
{ return "critically hit"; }
cchar* character::ThirdPersonUnarmedHitVerb() const { return "hits"; }
cchar* character::ThirdPersonCriticalUnarmedHitVerb() const
{ return "critically hits"; }
cchar* character::FirstPersonKickVerb() const { return "kick"; }
cchar* character::FirstPersonCriticalKickVerb() const
{ return "critically kick"; }
cchar* character::ThirdPersonKickVerb() const { return "kicks"; }
cchar* character::ThirdPersonCriticalKickVerb() const
{ return "critically kicks"; }
cchar* character::FirstPersonBiteVerb() const { return "bite"; }
cchar* character::FirstPersonCriticalBiteVerb() const
{ return "critically bite"; }
cchar* character::ThirdPersonBiteVerb() const { return "bites"; }
cchar* character::ThirdPersonCriticalBiteVerb() const
{ return "critically bites"; }
cchar* character::UnarmedHitNoun() const { return "attack"; }
cchar* character::KickNoun() const { return "kick"; }
cchar* character::BiteNoun() const { return "attack"; }
cchar* character::GetEquipmentName(int) const { return ""; }
const std::list<ulong>& character::GetOriginalBodyPartID(int I) const
{ return OriginalBodyPartID[I]; }
square* character::GetNeighbourSquare(int I) const
{ return GetSquareUnder()->GetNeighbourSquare(I); }
lsquare* character::GetNeighbourLSquare(int I) const
{ return static_cast<lsquare*>(GetSquareUnder())->GetNeighbourLSquare(I); }
wsquare* character::GetNeighbourWSquare(int I) const
{ return static_cast<wsquare*>(GetSquareUnder())->GetNeighbourWSquare(I); }
god* character::GetMasterGod() const { return game::GetGod(GetConfig()); }
col16 character::GetBodyPartColorA(int, truth) const
{ return GetSkinColor(); }
col16 character::GetBodyPartColorB(int, truth) const
{ return GetTorsoMainColor(); }
col16 character::GetBodyPartColorC(int, truth) const
{ return GetBeltColor(); } // sorry...
col16 character::GetBodyPartColorD(int, truth) const
{ return GetTorsoSpecialColor(); }
int character::GetRandomApplyBodyPart() const { return TORSO_INDEX; }
truth character::MustBeRemovedFromBone() const
{ return IsUnique() && !CanBeGenerated(); }
truth character::IsPet() const { return GetTeam()->GetID() == PLAYER_TEAM; }
character* character::GetLeader() const { return GetTeam()->GetLeader(); }
int character::GetMoveType() const
{ return ((!StateIsActivated(LEVITATION)
          ? DataBase->MoveType
          : DataBase->MoveType | FLY) |
          (!StateIsActivated(ETHEREAL_MOVING)
          ? DataBase->MoveType
          : DataBase->MoveType | ETHEREAL) |
          (!StateIsActivated(SWIMMING)
          ? DataBase->MoveType
          : DataBase->MoveType | WALK|SWIM)); }
festring character::GetZombieDescription() const
{ return " of " + GetName(INDEFINITE); }
truth character::BodyPartCanBeSevered(int I) const { return I; }
truth character::HasBeenSeen() const
{ return DataBase->Flags & HAS_BEEN_SEEN; }
truth character::IsTemporary() const
{ return GetTorso()->GetLifeExpectancy(); }
cchar* character::GetNormalDeathMessage() const { return "killed @k"; }
festring character::GetGhostDescription() const
{ return " of " + GetName(INDEFINITE); }

int characterdatabase::* ExpPtr[ATTRIBUTES] =
{
  &characterdatabase::DefaultEndurance,
  &characterdatabase::DefaultPerception,
  &characterdatabase::DefaultIntelligence,
  &characterdatabase::DefaultWisdom,
  &characterdatabase::DefaultWillPower,
  &characterdatabase::DefaultCharisma,
  &characterdatabase::DefaultMana,
  &characterdatabase::DefaultArmStrength,
  &characterdatabase::DefaultLegStrength,
  &characterdatabase::DefaultDexterity,
  &characterdatabase::DefaultAgility
};

contentscript<item> characterdatabase::* EquipmentDataPtr[EQUIPMENT_DATAS] =
{
  &characterdatabase::Helmet,
  &characterdatabase::Amulet,
  &characterdatabase::Cloak,
  &characterdatabase::BodyArmor,
  &characterdatabase::Belt,
  &characterdatabase::RightWielded,
  &characterdatabase::LeftWielded,
  &characterdatabase::RightRing,
  &characterdatabase::LeftRing,
  &characterdatabase::RightGauntlet,
  &characterdatabase::LeftGauntlet,
  &characterdatabase::RightBoot,
  &characterdatabase::LeftBoot
};

character::character(ccharacter& Char)
: entity(Char), id(Char), NP(Char.NP), AP(Char.AP),
  TemporaryState(Char.TemporaryState&~POLYMORPHED),
  Team(Char.Team), GoingTo(ERROR_V2),
  RandomMoveDir(femath::RandReal(8)),
  Money(0), AssignedName(Char.AssignedName), Action(0),
  DataBase(Char.DataBase), MotherEntity(0),
  PolymorphBackup(0), EquipmentState(0), SquareUnder(0),
  AllowedWeaponSkillCategories(Char.AllowedWeaponSkillCategories),
  BodyParts(Char.BodyParts),
  RegenerationCounter(Char.RegenerationCounter),
  SquaresUnder(Char.SquaresUnder), LastAcidMsgMin(0),
  Stamina(Char.Stamina), MaxStamina(Char.MaxStamina),
  BlocksSinceLastTurn(0), GenerationDanger(Char.GenerationDanger),
  CommandFlags(Char.CommandFlags), WarnFlags(0),
  ScienceTalks(Char.ScienceTalks), TrapData(0), CounterToMindWormHatch(0)
{
  Flags &= ~C_PLAYER;
  Flags |= C_INITIALIZING|C_IN_NO_MSG_MODE;
  Stack = new stack(0, this, HIDDEN);

  int c;

  for(c = 0; c < MAX_EQUIPMENT_SLOTS; c++)
    MemorizedEquippedItemIDs[c] = Char.MemorizedEquippedItemIDs[c];

  v2HoldPos=Char.v2HoldPos;

  for(c = 0; c < STATES; ++c)
    TemporaryStateCounter[c] = Char.TemporaryStateCounter[c];

  if(Team)
    TeamIterator = Team->Add(this);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    BaseExperience[c] = Char.BaseExperience[c];

  BodyPartSlot = new bodypartslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill[AllowedWeaponSkillCategories];
  SquareUnder = new square*[SquaresUnder];

  if(SquaresUnder == 1)
    *SquareUnder = 0;
  else
    memset(SquareUnder, 0, SquaresUnder * sizeof(square*));

  for(c = 0; c < BodyParts; ++c)
  {
    BodyPartSlot[c].SetMaster(this);
    bodypart* CharBodyPart = Char.GetBodyPart(c);
    OriginalBodyPartID[c] = Char.OriginalBodyPartID[c];

    if(CharBodyPart)
    {
      bodypart* BodyPart = static_cast<bodypart*>(CharBodyPart->Duplicate());
      SetBodyPart(c, BodyPart);
      BodyPart->CalculateEmitation();
    }
  }

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    CWeaponSkill[c] = Char.CWeaponSkill[c];

  HomeData = Char.HomeData ? new homedata(*Char.HomeData) : 0;
  ID = game::CreateNewCharacterID(this);
}

character::character()
: entity(HAS_BE), NP(50000), AP(0), TemporaryState(0), Team(0),
  GoingTo(ERROR_V2), RandomMoveDir(femath::RandReal(8)),
  Money(0), Action(0), MotherEntity(0), PolymorphBackup(0), EquipmentState(0),
  SquareUnder(0), RegenerationCounter(0), HomeData(0), LastAcidMsgMin(0),
  BlocksSinceLastTurn(0), GenerationDanger(DEFAULT_GENERATION_DANGER),
  WarnFlags(0), ScienceTalks(0), TrapData(0), CounterToMindWormHatch(0)
{
  Stack = new stack(0, this, HIDDEN);

  int c;

  for(c = 0; c < MAX_EQUIPMENT_SLOTS; c++)
    MemorizedEquippedItemIDs[c]=0;

  v2HoldPos=v2(0,0);
}

character::~character()
{
  if(Action)
    delete Action;

  if(Team)
    Team->Remove(GetTeamIterator());

  delete Stack;
  int c;

  for(c = 0; c < BodyParts; ++c)
    delete GetBodyPart(c);

  delete [] BodyPartSlot;
  delete [] OriginalBodyPartID;
  delete PolymorphBackup;
  delete [] SquareUnder;
  delete [] CWeaponSkill;
  delete HomeData;

  for(trapdata* T = TrapData; T;)
  {
    trapdata* ToDel = T;
    T = T->Next;
    delete ToDel;
  }

  game::RemoveCharacterID(ID);
}

void character::Hunger()
{
  switch(GetBurdenState())
  {
   case OVER_LOADED:
   case STRESSED:
    EditNP(-8);
    EditExperience(LEG_STRENGTH, 150, 1 << 2);
    EditExperience(AGILITY, -50, 1 << 2);
    break;
   case BURDENED:
    EditNP(-2);
    EditExperience(LEG_STRENGTH, 75, 1 << 1);
    EditExperience(AGILITY, -25, 1 << 1);
    break;
   case UNBURDENED:
    EditNP(-1);
    break;
  }

  switch(GetHungerState())
  {
   case STARVING:
    EditExperience(ARM_STRENGTH, -75, 1 << 3);
    EditExperience(LEG_STRENGTH, -75, 1 << 3);
    break;
   case VERY_HUNGRY:
    EditExperience(ARM_STRENGTH, -50, 1 << 2);
    EditExperience(LEG_STRENGTH, -50, 1 << 2);
    break;
   case HUNGRY:
    EditExperience(ARM_STRENGTH, -25, 1 << 1);
    EditExperience(LEG_STRENGTH, -25, 1 << 1);
    break;
   case SATIATED:
    EditExperience(AGILITY, -25, 1 << 1);
    break;
   case BLOATED:
    EditExperience(AGILITY, -50, 1 << 2);
    break;
   case OVER_FED:
    EditExperience(AGILITY, -75, 1 << 3);
    break;
  }

  CheckStarvationDeath(CONST_S("starved to death"));
}

int character::TakeHit(character* Enemy, item* Weapon,
                       bodypart* EnemyBodyPart, v2 HitPos,
                       double Damage, double ToHitValue,
                       int Success, int Type, int GivenDir,
                       truth Critical, truth ForceHit)
{
  hiteffectSetup* pHitEff=NULL;DBGLN;
  bool bShowHitEffect = false;
  static bool bHardestMode = false; //TODO make these an user hardcore combat option?
  if(!bHardestMode){
    //w/o ESP/infravision and if the square is visible even if both fighting are invisible
    static bool bPlayerCanHearWhereTheFightIsHappening = true; //TODO this feels like cheating? making things easier? if so, set it to false
    static bool bPlayerCanSensePetFighting = true; //TODO this feels like cheating? making things easier? if so, set it to false

    if(bPlayerCanHearWhereTheFightIsHappening) //TODO should then also show at non directly visible squares?
      if(GetLSquareUnder()->CanBeSeenByPlayer() || Enemy->GetLSquareUnder()->CanBeSeenByPlayer())bShowHitEffect = true;

    if(bPlayerCanSensePetFighting && IsPet())bShowHitEffect=true; // override for team awareness
  }
  if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())bShowHitEffect=true; //throwing hits in the air is valid (seen) if the other one is invisible
  if(IsPlayer())bShowHitEffect=true; //override
  if(bShowHitEffect){DBGLN;
    pHitEff=new hiteffectSetup();
    pHitEff->Critical=Critical;
    pHitEff->GivenDir=GivenDir;
    pHitEff->Type=Type;
    pHitEff->WhoHits=Enemy; DBGLN;DBG2(Enemy,"WhoHits"); DBGSV2(Enemy->GetPos()); DBG1(Enemy->GetName(DEFINITE).CStr());
    pHitEff->WhoIsHit=this;
    pHitEff->itemEffectReference = Weapon;
    if(pHitEff->itemEffectReference==NULL)pHitEff->itemEffectReference=EnemyBodyPart;
  }

  int Dir = Type == BITE_ATTACK ? YOURSELF : GivenDir;
  double DodgeValue = GetDodgeValue();

  if(!Enemy->IsPlayer() && GetAttackWisdomLimit() != NO_LIMIT)
    Enemy->EditExperience(WISDOM, 75, 1 << 13);

  if(!Enemy->CanBeSeenBy(this))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy))
    DodgeValue *= 2;

  if(Enemy->StateIsActivated(CONFUSED))
    ToHitValue *= 0.75;

  switch(Enemy->GetTirednessState())
  {
   case FAINTING:
    ToHitValue *= 0.50;
   case EXHAUSTED:
    ToHitValue *= 0.75;
  }

  switch(GetTirednessState())
  {
   case FAINTING:
    DodgeValue *= 0.50;
   case EXHAUSTED:
    DodgeValue *= 0.75;
  }

  if(!ForceHit)
  {
    if(!IsRetreating())
      SetGoingTo(Enemy->GetPos());
    else
      SetGoingTo(GetPos() - ((Enemy->GetPos() - GetPos()) << 4));

    if(!Enemy->IsRetreating())
      Enemy->SetGoingTo(GetPos());
    else
      Enemy->SetGoingTo(Enemy->GetPos()
                        - ((GetPos() - Enemy->GetPos()) << 4));
  }

  /* Effectively, the average chance to hit is 100% / (DV/THV + 1). */

  if(RAND() % int(100 + ToHitValue / DodgeValue * (100 + Success)) < 100
     && !Critical && !ForceHit)
  {
    Enemy->AddMissMessage(this);
    EditExperience(AGILITY, 150, 1 << 7);
    EditExperience(PERCEPTION, 75, 1 << 7);

    if(Enemy->CanBeSeenByPlayer())
      DeActivateVoluntaryAction(CONST_S("The attack of ")
                                + Enemy->GetName(DEFINITE)
                                + CONST_S(" interrupts you."));
    else
      DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

//    if(hitEff!=NULL)hitEff->End();
    return HAS_DODGED;
  }

  int TrueDamage = int(Damage * (100 + Success) / 100)
                   + (RAND() % 3 ? 1 : 0);

  if(Critical)
  {
    TrueDamage += TrueDamage >> 1;
    ++TrueDamage;
  }

  int BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);

  if(Critical)
  {
    switch(Type)
    {
     case UNARMED_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonCriticalUnarmedHitVerb(),
                                    Enemy->ThirdPersonCriticalUnarmedHitVerb(),
                                    BodyPart);
      break;
     case WEAPON_ATTACK:
      Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, true);
      break;
     case KICK_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonCriticalKickVerb(),
                                    Enemy->ThirdPersonCriticalKickVerb(),
                                    BodyPart);
      break;
     case BITE_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonCriticalBiteVerb(),
                                    Enemy->ThirdPersonCriticalBiteVerb(),
                                    BodyPart);
      break;
    }
  }
  else
  {
    switch(Type)
    {
     case UNARMED_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonUnarmedHitVerb(),
                                    Enemy->ThirdPersonUnarmedHitVerb(),
                                    BodyPart);
      break;
     case WEAPON_ATTACK:
      Enemy->AddWeaponHitMessage(this, Weapon, BodyPart, false);
      break;
     case KICK_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonKickVerb(),
                                    Enemy->ThirdPersonKickVerb(),
                                    BodyPart);
      break;
     case BITE_ATTACK:
      Enemy->AddPrimitiveHitMessage(this,
                                    Enemy->FirstPersonBiteVerb(),
                                    Enemy->ThirdPersonBiteVerb(),
                                    BodyPart);
      break;
    }
  }

  if(!Critical && TrueDamage && Enemy->AttackIsBlockable(Type))
  {
    TrueDamage = CheckForBlock(Enemy, Weapon, ToHitValue,
                               TrueDamage, Success, Type);

    if(!TrueDamage || (Weapon && !Weapon->Exists()))
    {
      if(Enemy->CanBeSeenByPlayer())
        DeActivateVoluntaryAction(CONST_S("The attack of ")
                                  + Enemy->GetName(DEFINITE)
                                  + CONST_S(" interrupts you."));
      else
        DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

//      if(hitEff!=NULL)hitEff->End();
      return HAS_BLOCKED;
    }
  }

  int WeaponSkillHits = CalculateWeaponSkillHits(Enemy);
  int DoneDamage = ReceiveBodyPartDamage(Enemy, TrueDamage,
                                         PHYSICAL_DAMAGE, BodyPart,
                                         Dir, false, Critical, true,
                                         Type == BITE_ATTACK
                                         && Enemy->BiteCapturesBodyPart());
  truth Succeeded = (GetBodyPart(BodyPart)
                     && HitEffect(Enemy, Weapon, HitPos, Type,
                                  BodyPart, Dir, !DoneDamage, Critical, DoneDamage))
                    || DoneDamage;

  if(Succeeded)
    Enemy->WeaponSkillHit(Weapon, Type, WeaponSkillHits);

  if(Weapon)
  {
//    if(CanBeSeenByPlayer())
//      GetLSquareUnder()->AddHitEffect(Weapon,this,Enemy);

    if(Weapon->Exists() && DoneDamage < TrueDamage)
      Weapon->ReceiveDamage(Enemy, TrueDamage - DoneDamage, PHYSICAL_DAMAGE);

    if(Weapon->Exists() && DoneDamage
       && SpillsBlood() && GetBodyPart(BodyPart)
       && (GetBodyPart(BodyPart)->IsAlive()
           || GetBodyPart(BodyPart)->GetMainMaterial()->IsLiquid()))
      Weapon->SpillFluid(0, CreateBlood(15 + RAND() % 15));
  }

  if(Enemy->AttackIsBlockable(Type))
    SpecialBodyDefenceEffect(Enemy, EnemyBodyPart, Type);

  if(!Succeeded)
  {
    if(Enemy->CanBeSeenByPlayer())
      DeActivateVoluntaryAction(CONST_S("The attack of ")
                                + Enemy->GetName(DEFINITE)
                                + CONST_S(" interrupts you."));
    else
      DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

//    if(hitEff!=NULL)hitEff->End();
    return DID_NO_DAMAGE;
  }

  if(pHitEff!=NULL){DBGLN;
    GetLSquareUnder()->AddHitEffect(*pHitEff); //after all returns of failure and before any other returns
    delete pHitEff; //already copied
  }

  if(CheckDeath(GetNormalDeathMessage(), Enemy,
                Enemy->IsPlayer() ? FORCE_MSG : 0))
    return HAS_DIED;

  if(Enemy->CanBeSeenByPlayer())
    DeActivateVoluntaryAction(CONST_S("The attack of ")
                              + Enemy->GetName(DEFINITE)
                              + CONST_S(" interrupts you."));
  else
    DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));

  return HAS_HIT;
}

struct svpriorityelement
{
  svpriorityelement(int BodyPart, int StrengthValue)
  : BodyPart(BodyPart), StrengthValue(StrengthValue) { }
  bool operator<(const svpriorityelement& AnotherPair) const
  { return StrengthValue > AnotherPair.StrengthValue; }
  int BodyPart;
  int StrengthValue;
};

int character::ChooseBodyPartToReceiveHit(double ToHitValue,
                                          double DodgeValue)
{
  if(BodyParts == 1)
    return 0;

  std::priority_queue<svpriorityelement> SVQueue;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && (BodyPart->GetHP() != 1 || BodyPart->CanBeSevered(PHYSICAL_DAMAGE)))
      SVQueue.push(svpriorityelement(c, ModifyBodyPartHitPreference(c, BodyPart->GetStrengthValue()
                                                                       + BodyPart->GetHP())));
  }

  while(SVQueue.size())
  {
    svpriorityelement E = SVQueue.top();
    int ToHitPercentage = int(GLOBAL_WEAK_BODYPART_HIT_MODIFIER
                              * ToHitValue
                              * GetBodyPart(E.BodyPart)->GetBodyPartVolume()
                              / (DodgeValue * GetBodyVolume()));
    ToHitPercentage = ModifyBodyPartToHitChance(E.BodyPart, ToHitPercentage);

    if(ToHitPercentage < 1)
      ToHitPercentage = 1;
    else if(ToHitPercentage > 95)
      ToHitPercentage = 95;

    if(ToHitPercentage > RAND() % 100)
      return E.BodyPart;

    SVQueue.pop();
  }

  return 0;
}

#include "audio.h"

void character::Be()
{
  if(game::ForceJumpToPlayerBe())
  {
    if(!IsPlayer())
      return;
    else
      game::SetForceJumpToPlayerBe(false);
  }
  else
  {
    truth ForceBe = HP != MaxHP || AllowSpoil();

    for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart && (ForceBe || BodyPart->NeedsBe()))
        BodyPart->Be();
    }

    HandleStates();

    if(!IsEnabled())
      return;

    if(GetTeam() == PLAYER->GetTeam())
    {
      for(int c = 0; c < AllowedWeaponSkillCategories; ++c)
        if(CWeaponSkill[c].Tick() && IsPlayer())
          CWeaponSkill[c].AddLevelDownMessage(c);

      SWeaponSkillTick();
    }

    if(IsPlayer())
    {
      if(GetHungerState() == STARVING && !(RAND() % 50))
        LoseConsciousness(250 + RAND_N(250), true);

      if(!Action || Action->AllowFoodConsumption())
        Hunger();


      int MinHPPercent = 128;
      for(int c = 0; c < BodyParts; ++c)
      {
         int tempHpPercent;
        bodypart* BodyPart = GetBodyPart(c);

        if(BodyPart)
        {
           tempHpPercent = (BodyPart->GetHP() * audio::MAX_INTENSITY_VOLUME) / BodyPart->GetMaxHP();
           if(tempHpPercent < MinHPPercent )
           {
              MinHPPercent = tempHpPercent;
           }


        }
      }
      audio::IntensityLevel( audio::MAX_INTENSITY_VOLUME - MinHPPercent );
    }

    if(Stamina != MaxStamina)
      RegenerateStamina();

    if(HP != MaxHP || StateIsActivated(REGENERATION))
      Regenerate();

    if(Action && AP >= 1000)
      ActionAutoTermination();

    if(Action && AP >= 1000)
    {
      Action->Handle();

      if(!IsEnabled())
        return;
    }
    else
      EditAP(GetStateAPGain(100));
  }

  if(AP >= 1000)
  {
    SpecialTurnHandler();
    BlocksSinceLastTurn = 0;

    if(IsPlayer())
    {
      static int Timer = 0;

      if(ivanconfig::GetAutoSaveInterval() && !GetAction()
         && ++Timer >= ivanconfig::GetAutoSaveInterval())
      {
        game::Save(game::GetAutoSaveFileName());
        Timer = 0;
      }

      game::CalculateNextDanger();

      if(!StateIsActivated(POLYMORPHED))
        game::UpdatePlayerAttributeAverage();

      if(!game::IsInWilderness())
        Search(GetAttribute(PERCEPTION));

      if(!Action)
      {
        GetPlayerCommand();
      }
      else
      {
        if(Action->ShowEnvironment())
        {
          static int Counter = 0;

          if(++Counter == 10)
          {
            game::DrawEverything();
            Counter = 0;
          }
        }

        msgsystem::ThyMessagesAreNowOld();

        if(Action->IsVoluntary() && WAIT_FOR_KEY_DOWN())
        {
          READ_KEY();
          Action->Terminate(false);
        }
      }
    }
    else
    {
      if(!Action && !game::IsInWilderness())
        GetAICommand();
    }
  }
}

void character::Move(v2 MoveTo, truth TeleportMove, truth Run)
{
  if(!IsEnabled())
    return;

  /* Test whether the player is stuck to something */

  if(!TeleportMove && !TryToUnStickTraps(MoveTo - GetPos()))
    return;

  if(Run && !IsPlayer() && TorsoIsAlive()
     && (Stamina <= 10000 / Max(GetAttribute(LEG_STRENGTH), 1)
         || (!StateIsActivated(PANIC) && Stamina < MaxStamina >> 2)))
    Run = false;

  RemoveTraps();

  if(GetBurdenState() != OVER_LOADED || TeleportMove)
  {
    lsquare* OldSquareUnder[MAX_SQUARES_UNDER];

    if(!game::IsInWilderness())
      for(int c = 0; c < GetSquaresUnder(); ++c)
        OldSquareUnder[c] = GetLSquareUnder(c);

    Remove();
    PutTo(MoveTo);

    if(!TeleportMove)
    {
      /* Multitiled creatures should behave differently, maybe? */

      if(Run)
      {
        int ED = GetSquareUnder()->GetEntryDifficulty();
        EditAP(-GetMoveAPRequirement(ED) >> 1);
        EditNP(-24 * ED);
        EditExperience(AGILITY, 125, ED << 7);
        int Base = 10000;

        if(IsPlayer())
          switch(GetHungerState())
          {
           case SATIATED:
            Base = 11000;
            break;
           case BLOATED:
            Base = 12500;
            break;
           case OVER_FED:
            Base = 15000;
            break;
          }

        EditStamina(-Base / Max(GetAttribute(LEG_STRENGTH), 1), true);
      }
      else
      {
        int ED = GetSquareUnder()->GetEntryDifficulty();
        EditAP(-GetMoveAPRequirement(ED));
        EditNP(-12 * ED);
        EditExperience(AGILITY, 75, ED << 7);
      }
    }

    if(IsPlayer())
      ShowNewPosInfo();

    if(!game::IsInWilderness())
      SignalStepFrom(OldSquareUnder);
  }
  else
  {
    if(IsPlayer())
    {
      cchar* CrawlVerb = StateIsActivated(LEVITATION) ? "float" : "crawl";
      ADD_MESSAGE("You try very hard to %s forward. But your load is too heavy.", CrawlVerb);
    }

    EditAP(-1000);
  }
}

void character::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(CheckSadism())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

truth character::MoveTowardsTarget(truth Run)
{
  v2 Pos = GetPos();
  v2 TPos;

  if(!Route.empty())
  {
    TPos = Route.back();
    Route.pop_back();
  }
  else
    TPos = GoingTo;

  v2 MoveTo[3];

  if(TPos.X < Pos.X)
  {
    if(TPos.Y < Pos.Y)
    {
      MoveTo[0] = v2(-1, -1);
      MoveTo[1] = v2(-1,  0);
      MoveTo[2] = v2( 0, -1);
    }

    if(TPos.Y == Pos.Y)
    {
      MoveTo[0] = v2(-1,  0);
      MoveTo[1] = v2(-1, -1);
      MoveTo[2] = v2(-1,  1);
    }

    if(TPos.Y > Pos.Y)
    {
      MoveTo[0] = v2(-1, 1);
      MoveTo[1] = v2(-1, 0);
      MoveTo[2] = v2( 0, 1);
    }
  }

  if(TPos.X == Pos.X)
  {
    if(TPos.Y < Pos.Y)
    {
      MoveTo[0] = v2( 0, -1);
      MoveTo[1] = v2(-1, -1);
      MoveTo[2] = v2( 1, -1);
    }

    if(TPos.Y == Pos.Y)
    {
      TerminateGoingTo();
      return false;
    }

    if(TPos.Y > Pos.Y)
    {
      MoveTo[0] = v2( 0, 1);
      MoveTo[1] = v2(-1, 1);
      MoveTo[2] = v2( 1, 1);
    }
  }

  if(TPos.X > Pos.X)
  {
    if(TPos.Y < Pos.Y)
    {
      MoveTo[0] = v2(1, -1);
      MoveTo[1] = v2(1,  0);
      MoveTo[2] = v2(0, -1);
    }

    if(TPos.Y == Pos.Y)
    {
      MoveTo[0] = v2(1,  0);
      MoveTo[1] = v2(1, -1);
      MoveTo[2] = v2(1,  1);
    }

    if(TPos.Y > Pos.Y)
    {
      MoveTo[0] = v2(1, 1);
      MoveTo[1] = v2(1, 0);
      MoveTo[2] = v2(0, 1);
    }
  }

  v2 ModifiedMoveTo = ApplyStateModification(MoveTo[0]);

  if(TryMove(ModifiedMoveTo, true, Run))
  {
    RandomMoveDir = femath::RandReal(8);
    return true;
  }

  int L = (Pos - TPos).GetManhattanLength();

  if(RAND() & 1)
    Swap(MoveTo[1], MoveTo[2]);

  if(Pos.IsAdjacent(TPos))
  {
    TerminateGoingTo();
    return false;
  }

  if((Pos + MoveTo[1] - TPos).GetManhattanLength() <= L
     && TryMove(ApplyStateModification(MoveTo[1]), true, Run))
    return true;

  if((Pos + MoveTo[2] - TPos).GetManhattanLength() <= L
     && TryMove(ApplyStateModification(MoveTo[2]), true, Run))
    return true;

  Illegal.insert(Pos + ModifiedMoveTo);

  if(CreateRoute())
    return true;

  return false;
}

int character::CalculateNewSquaresUnder(lsquare** NewSquare, v2 Pos) const
{
  if(GetLevel()->IsValidPos(Pos))
  {
    *NewSquare = GetNearLSquare(Pos);
    return 1;
  }
  else
    return 0;
}

truth character::TryMove(v2 MoveVector, truth Important, truth Run, truth* pbWaitNeutralMove)
{
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  character* Pet[MAX_SQUARES_UNDER];
  character* Neutral[MAX_SQUARES_UNDER];
  character* Hostile[MAX_SQUARES_UNDER];
  v2 PetPos[MAX_SQUARES_UNDER];
  v2 NeutralPos[MAX_SQUARES_UNDER];
  v2 HostilePos[MAX_SQUARES_UNDER];
  v2 MoveTo = GetPos() + MoveVector;
  int Direction = game::GetDirectionForVector(MoveVector);

  if(Direction == DIR_ERROR)
    ABORT("Direction fault.");

  if(!game::IsInWilderness())
  {
    int Squares = CalculateNewSquaresUnder(MoveToSquare, MoveTo);

    if(Squares)
    {
      int Pets = 0;
      int Neutrals = 0;
      int Hostiles = 0;

      for(int c = 0; c < Squares; ++c)
      {
        character* Char = MoveToSquare[c]->GetCharacter();

        if(Char && Char != this)
        {
          v2 Pos = MoveToSquare[c]->GetPos();

          if(IsAlly(Char))
          {
            Pet[Pets] = Char;
            PetPos[Pets++] = Pos;
          }
          else if(Char->GetRelation(this) != HOSTILE)
          {
            Neutral[Neutrals] = Char;
            NeutralPos[Neutrals++] = Pos;
          }
          else
          {
            Hostile[Hostiles] = Char;
            HostilePos[Hostiles++] = Pos;
          }
        }
      }

      if(Hostiles == 1)
        return Hit(Hostile[0], HostilePos[0], Direction);
      else if(Hostiles)
      {
        int Index = RAND() % Hostiles;
        return Hit(Hostile[Index], HostilePos[Index], Direction);
      }

      if(Neutrals>0 && ivanconfig::IsWaitNeutralsMoveAway() && pbWaitNeutralMove!=NULL){
        (*pbWaitNeutralMove)=true;
        return false;
      }else{
        if(Neutrals == 1)
        {
          if(!IsPlayer() && !Pets && Important && CanMoveOn(MoveToSquare[0]))
            return HandleCharacterBlockingTheWay(Neutral[0], NeutralPos[0], Direction);
          else
            return IsPlayer() && Hit(Neutral[0], NeutralPos[0], Direction);
        }
        else if(Neutrals)
        {
          if(IsPlayer())
          {
            int Index = RAND() % Neutrals;
            return Hit(Neutral[Index], NeutralPos[Index], Direction);
          }
          else
            return false;
        }
      }

      if(!IsPlayer())
        for(int c = 0; c < Squares; ++c)
          if(MoveToSquare[c]->IsScary(this))
            return false;

      if(Pets == 1)
      {
        if(IsPlayer() && !ivanconfig::GetBeNice()
           && Pet[0]->IsMasochist() && HasSadistAttackMode()
           && game::TruthQuestion("Do you want to punish " + Pet[0]->GetObjectPronoun() + "? [y/N]"))
          return Hit(Pet[0], PetPos[0], Direction, SADIST_HIT);
        else
          return (Important
                  && (CanMoveOn(MoveToSquare[0])
                      || (IsPlayer()
                          && game::GoThroughWallsCheatIsActive()))
                  && Displace(Pet[0]));
    }
    else if(Pets)
      return false;

    if((CanMove() && CanMoveOn(MoveToSquare[0]))
       || (game::GoThroughWallsCheatIsActive() && IsPlayer()))
    {
      Move(MoveTo, false, Run);

      if(IsEnabled() && GetPos() == GoingTo)
        TerminateGoingTo();

      return true;
    }
    else
      for(int c = 0; c < Squares; ++c)
      {
        olterrain* Terrain = MoveToSquare[c]->GetOLTerrain();

        if(Terrain && Terrain->CanBeOpened())
        {
          if(CanOpen())
          {
            if(Terrain->IsLocked())
            {
              if(IsPlayer())
              {
                /* not sure if this is better than "the door is locked", but I guess it _might_ be slightly better */
                ADD_MESSAGE("The %s is locked.", Terrain->GetNameSingular().CStr());
                if(!IsPlayerAutoPlay())return false;
              }

              if(Important && CheckKick())
              {
                room* Room = MoveToSquare[c]->GetRoom();

                if(!Room || Room->AllowKick(this, MoveToSquare[c]))
                {
                  int HP = Terrain->GetHP();

                  if(CanBeSeenByPlayer())
                    ADD_MESSAGE("%s kicks %s.", CHAR_NAME(DEFINITE), Terrain->CHAR_NAME(DEFINITE));

                  Kick(MoveToSquare[c], Direction);
                  olterrain* NewTerrain = MoveToSquare[c]->GetOLTerrain();

                  if(NewTerrain == Terrain && Terrain->GetHP() == HP) // BUG!
                  {
                    Illegal.insert(MoveTo);
                    CreateRoute();
                  }

                  return true;
                }
              }
            }
            else
              return MoveToSquare[c]->Open(this);
          }
          else
          {
            if(IsPlayer())
            {
              ADD_MESSAGE("This monster type cannot open doors.");
              return false;
            }
            else if(Important)
            {
              Illegal.insert(MoveTo);
              return CreateRoute();
            }
          }
        }
      }

    return false;
  }
  else
  {
    if(IsPlayer() && !IsStuck() && GetLevel()->IsOnGround()
       && game::TruthQuestion(CONST_S("Do you want to leave ")
                              + game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex())
                              + "? [y/N]"))
    {
      if(HasPetrussNut() && !HasGoldenEagleShirt())
      {
        game::PlayVictoryMusic();
        game::TextScreen(CONST_S("An undead and sinister voice greets you as you leave the city behind:\n\n"
                                 "\"MoRtAl! ThOu HaSt SlAuGhTeReD pEtRuS aNd PlEaSeD mE!\nfRoM tHiS dAy On, "
                                 "ThOu ArT tHe DeArEsT sErVaNt Of AlL eViL!\"\n\nYou are victorious!"));
        game::GetCurrentArea()->SendNewDrawRequest();
        game::DrawEverything();
        ShowAdventureInfo();
        festring Msg = CONST_S("killed Petrus and became the Avatar of Chaos");
        PLAYER->AddScoreEntry(Msg, 3, false);
        game::End(Msg);
        return true;
      }

      if(game::TryTravel(WORLD_MAP, WORLD_MAP, game::GetCurrentDungeonIndex()))
        return true;
    }

    return false;
  }
}
  else
  {
    /** No multitile support */

    if(CanMove()
       && GetArea()->IsValidPos(MoveTo)
       && (CanMoveOn(GetNearWSquare(MoveTo))
           || game::GoThroughWallsCheatIsActive()))
    {
      if(!game::GoThroughWallsCheatIsActive())
      {
        charactervector& V = game::GetWorldMap()->GetPlayerGroup();
        truth Discard = false;

        for(uint c = 0; c < V.size(); ++c)
          if(!V[c]->CanMoveOn(GetNearWSquare(MoveTo)))
          {
            if(!Discard)
            {
              ADD_MESSAGE("One or more of your team members cannot cross this terrain.");

              if(!game::TruthQuestion("Discard them? [y/N]"))
                return false;

              Discard = true;
            }

            if(Discard)
              delete V[c];

            V.erase(V.begin() + c--);
          }
      }

      Move(MoveTo, false);
      return true;
    }
    else
      return false;
  }
}

void character::CreateCorpse(lsquare* Square)
{
  if(!BodyPartsDisappearWhenSevered() && !game::AllBodyPartsVanish())
  {
    corpse* Corpse = corpse::Spawn(0, NO_MATERIALS);
    Corpse->SetDeceased(this);
    Square->AddItem(Corpse);
    Disable();
  }
  else
    SendToHell();
}

void character::AutoPlayAITeleport(bool bDeathCountBased)
{
  bool bTeleportNow=false;

  if(bDeathCountBased){ // this is good to prevent autoplay AI getting stuck endless dieing
    static int iDieMax=10;
    static int iDieTeleportCountDown=iDieMax;
    if(iDieTeleportCountDown==0){ //this helps on defeating not so strong enemies in spot
      if(IsPlayerAutoPlay())
        bTeleportNow=true;
      iDieTeleportCountDown=iDieMax;
    }else{
      static v2 v2DiePos(0,0);
      if(v2DiePos==GetPos()){
        iDieTeleportCountDown--;
      }else{
        v2DiePos=GetPos();
        iDieTeleportCountDown=iDieMax;
      }
    }
  }

  if(bTeleportNow)
    Move(GetLevel()->GetRandomSquare(this), true); //not using teleport function to avoid prompts, but this code is from there TODO and should be in sync! create TeleportRandomDirectly() ?
}

void character::Die(ccharacter* Killer, cfestring& Msg, ulong DeathFlags)
{
  /* Note: This function musn't delete any objects, since one of these may be
     the one currently processed by pool::Be()! */

  if(!IsEnabled())
    return;

  RemoveTraps();

  if(IsPlayer())
  {
    ADD_MESSAGE("You die.");

    if(game::WizardModeIsActive())
    {
      game::DrawEverything();

      bool bInstaResurrect=false;
      if(!bInstaResurrect && IsPlayerAutoPlay())bInstaResurrect=true;
      if(!bInstaResurrect && !game::TruthQuestion(CONST_S("Do you want to do this, cheater? [y/n]"), REQUIRES_ANSWER))bInstaResurrect=true;
      if(bInstaResurrect)
      {
        RestoreBodyParts();
        ResetSpoiling();
        if(IsBurning())
        {
          doforbodypartswithparam<truth>()(this, &bodypart::Extinguish, false);
          doforbodyparts()(this, &bodypart::ResetThermalEnergies);
          doforbodyparts()(this, &bodypart::ResetBurning);
        }
        RestoreHP();
        RestoreStamina();
        ResetStates();
        SetNP(SATIATED_LEVEL);
        SendNewDrawRequest();
        if(IsPlayerAutoPlay())AutoPlayAITeleport(true);
        return;
      }
    }
  }
  else if(CanBeSeenByPlayer() && !(DeathFlags & DISALLOW_MSG))
    ProcessAndAddMessage(GetDeathMessage());
  else if(DeathFlags & FORCE_MSG)
    ADD_MESSAGE("You sense the death of something.");

  if(IsBurning()) // do this anyway, it stops the corpse from emitating and continuing to propagate weirdness
  {
    doforbodypartswithparam<truth>()(this, &bodypart::Extinguish, false);
    doforbodyparts()(this, &bodypart::ResetThermalEnergies);
    doforbodyparts()(this, &bodypart::ResetBurning);
  }

  if(!(DeathFlags & FORBID_REINCARNATION))
  {
    if(StateIsActivated(LIFE_SAVED)
       && CanMoveOn(!game::IsInWilderness() ? GetSquareUnder() : PLAYER->GetSquareUnder()))
    {
      SaveLife();
      return;
    }

    if(SpecialSaveLife())
      return;
  }
  else if(StateIsActivated(LIFE_SAVED))
    RemoveLifeSavers();

  Flags |= C_IN_NO_MSG_MODE;
  bonesghost* Ghost = 0;

  if(IsPlayer())
  {
    game::RemoveSaves();

    if(!game::IsInWilderness())
    {
      Ghost = game::CreateGhost();
      Ghost->Disable();
    }
  }

  square* SquareUnder[MAX_SQUARES_UNDER];
  lsquare** LSquareUnder = reinterpret_cast<lsquare**>(SquareUnder);
  memset(SquareUnder, 0, sizeof(SquareUnder));
  Disable();

  if(IsPlayer() || !game::IsInWilderness())
  {
    for(int c = 0; c < SquaresUnder; ++c)
      SquareUnder[c] = GetSquareUnder(c);

    Remove();
  }
  else
  {
    charactervector& V = game::GetWorldMap()->GetPlayerGroup();
    V.erase(std::find(V.begin(), V.end(), this));
  }

  if(!game::IsInWilderness())
  {
    if(!StateIsActivated(POLYMORPHED))
    {
      if(!IsPlayer() && !IsTemporary() && !Msg.IsEmpty())
        game::SignalDeath(this, Killer, Msg);

      if(!(DeathFlags & DISALLOW_CORPSE))
        CreateCorpse(LSquareUnder[0]);
      else
        SendToHell();
    }
    else
    {
      if(!IsPlayer() && !IsTemporary() && !Msg.IsEmpty())
        game::SignalDeath(GetPolymorphBackup(), Killer, Msg);

      GetPolymorphBackup()->CreateCorpse(LSquareUnder[0]);
      GetPolymorphBackup()->Flags &= ~C_POLYMORPHED;
      SetPolymorphBackup(0);
      SendToHell();
    }
  }
  else
  {
    if(!IsPlayer() && !IsTemporary() && !Msg.IsEmpty())
      game::SignalDeath(this, Killer, Msg);

    SendToHell();
  }

  if(IsPlayer())
  {
    if(!game::IsInWilderness())
      for(int c = 0; c < GetSquaresUnder(); ++c)
        LSquareUnder[c]->SetTemporaryEmitation(GetEmitation());

    game::SRegionAroundDisable();
    game::PlayDefeatMusic();
    ShowAdventureInfo();

    if(!game::IsInWilderness())
      for(int c = 0; c < GetSquaresUnder(); ++c)
        LSquareUnder[c]->SetTemporaryEmitation(0);
  }

  if(!game::IsInWilderness())
  {
    if(GetSquaresUnder() == 1)
    {
      stack* StackUnder = LSquareUnder[0]->GetStack();
      GetStack()->MoveItemsTo(StackUnder);
      doforbodypartswithparam<stack*>()(this, &bodypart::DropEquipment, StackUnder);
    }
    else
    {
      while(GetStack()->GetItems())
        GetStack()->GetBottom()->MoveTo(LSquareUnder[RAND_N(GetSquaresUnder())]->GetStack());

      for(int c = 0; c < BodyParts; ++c)
      {
        bodypart* BodyPart = GetBodyPart(c);

        if(BodyPart)
          BodyPart->DropEquipment(LSquareUnder[RAND_N(GetSquaresUnder())]->GetStack());
      }
    }
  }

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(0);

  Flags &= ~C_IN_NO_MSG_MODE;

  if(IsPlayer())
  {
    if(game::GetXinrochTombStoryState() == 2)
    {
      festring MsgBut = CONST_S("delivered the Shadow Veil to the Necromancer and continued to further adventures, but was ");
      festring NewMsg = MsgBut << Msg;
      AddScoreEntry(NewMsg, 2, true);
    }
    else
      AddScoreEntry(Msg);

    if(!game::IsInWilderness())
    {
      Ghost->PutTo(LSquareUnder[0]->GetPos());
      Ghost->Enable();
      game::CreateBone();
    }
    game::TextScreen(CONST_S("Unfortunately you died."), ZERO_V2, WHITE, true, true, &game::ShowDeathSmiley);
    game::End(Msg);
  }
}

void character::AddMissMessage(ccharacter* Enemy) const
{
  festring Msg;

  if(Enemy->IsPlayer())
    Msg = GetDescription(DEFINITE) + " misses you!";
  else if(IsPlayer())
    Msg = CONST_S("You miss ") + Enemy->GetDescription(DEFINITE) + '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg = GetDescription(DEFINITE) + " misses " + Enemy->GetDescription(DEFINITE) + '!';
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

void character::AddBlockMessage(ccharacter* Enemy, citem* Blocker, cfestring& HitNoun, truth Partial) const
{
  festring Msg;
  festring BlockVerb = (Partial ? " to partially block the " : " to block the ") + HitNoun;

  if(IsPlayer())
    Msg << "You manage" << BlockVerb << " with your " << Blocker->GetName(UNARTICLED) << '!';
  else if(Enemy->IsPlayer() || Enemy->CanBeSeenByPlayer())
  {
    if(CanBeSeenByPlayer())
      Msg << GetName(DEFINITE) << " manages" << BlockVerb << " with "
          << GetPossessivePronoun() << ' ' << Blocker->GetName(UNARTICLED) << '!';
    else
      Msg << "Something manages" << BlockVerb << " with something!";
  }
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

void character::AddPrimitiveHitMessage(ccharacter* Enemy, cfestring& FirstPersonHitVerb,
                                       cfestring& ThirdPersonHitVerb, int BodyPart) const
{
  festring Msg;
  festring BodyPartDescription;

  if(BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()))
    BodyPartDescription << " in the " << Enemy->GetBodyPartName(BodyPart);

  if(Enemy->IsPlayer())
    Msg << GetDescription(DEFINITE) << ' ' << ThirdPersonHitVerb << " you" << BodyPartDescription << '!';
  else if(IsPlayer())
    Msg << "You " << FirstPersonHitVerb << ' ' << Enemy->GetDescription(DEFINITE) << BodyPartDescription << '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    Msg << GetDescription(DEFINITE) << ' ' << ThirdPersonHitVerb << ' '
        << Enemy->GetDescription(DEFINITE) + BodyPartDescription << '!';
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

cchar*const HitVerb[] = { "strike", "slash", "stab" };
cchar*const HitVerb3rdPersonEnd[] = { "s", "es", "s" };

void character::AddWeaponHitMessage(ccharacter* Enemy, citem* Weapon, int BodyPart, truth Critical) const
{
  festring Msg;
  festring BodyPartDescription;

  if(BodyPart && (Enemy->CanBeSeenByPlayer() || Enemy->IsPlayer()))
    BodyPartDescription << " in the " << Enemy->GetBodyPartName(BodyPart);

  int FittingTypes = 0;
  int DamageFlags = Weapon->GetDamageFlags();
  int DamageType = 0;

  for(int c = 0; c < DAMAGE_TYPES; ++c)
    if(1 << c & DamageFlags)
    {
      if(!FittingTypes || !RAND_N(FittingTypes + 1))
        DamageType = c;

      ++FittingTypes;
    }

  if(!FittingTypes)
    ABORT("No damage flags specified for %s!", Weapon->CHAR_NAME(UNARTICLED));

  festring NewHitVerb = Critical ? " critically " : " ";
  NewHitVerb << HitVerb[DamageType];
  cchar*const E = HitVerb3rdPersonEnd[DamageType];

  if(Enemy->IsPlayer())
  {
    Msg << GetDescription(DEFINITE) << NewHitVerb << E << " you" << BodyPartDescription;

    if(CanBeSeenByPlayer())
      Msg << " with " << GetPossessivePronoun() << ' ' << Weapon->GetName(UNARTICLED);

    Msg << '!';
  }
  else if(IsPlayer())
    Msg << "You" << NewHitVerb << ' ' << Enemy->GetDescription(DEFINITE) << BodyPartDescription << '!';
  else if(CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
  {
    Msg << GetDescription(DEFINITE) << NewHitVerb << E << ' '
        << Enemy->GetDescription(DEFINITE) << BodyPartDescription;

    if(CanBeSeenByPlayer())
      Msg << " with " << GetPossessivePronoun() << ' ' << Weapon->GetName(UNARTICLED);

    Msg << '!';
  }
  else
    return;

  ADD_MESSAGE("%s", Msg.CStr());
}

truth character::HasHeadOfElpuri() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsHeadOfElpuri())
      return true;

  return combineequipmentpredicates()(this, &item::IsHeadOfElpuri, 1);
}

truth character::HasPetrussNut() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsPetrussNut())
      return true;

  return combineequipmentpredicates()(this, &item::IsPetrussNut, 1);
}

truth character::HasGoldenEagleShirt() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsGoldenEagleShirt())
      return true;

  return combineequipmentpredicates()(this, &item::IsGoldenEagleShirt, 1);
}

truth character::HasEncryptedScroll() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsEncryptedScroll())
      return true;

  return combineequipmentpredicates()(this, &item::IsEncryptedScroll, 1);
}

truth character::HasShadowVeil() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsShadowVeil())
      return true;

  return combineequipmentpredicates()(this, &item::IsShadowVeil, 1);
}

truth character::HasLostRubyFlamingSword() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsLostRubyFlamingSword())
      return true;

  return combineequipmentpredicates()(this, &item::IsLostRubyFlamingSword, 1);
}

truth character::RemoveEncryptedScroll()
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsEncryptedScroll())
    {
      item* Item = *i;
      Item->RemoveFromSlot();
      Item->SendToHell();
      return true;
    }

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Item = GetEquipment(c);

    if(Item && Item->IsEncryptedScroll())
    {
      Item->RemoveFromSlot();
      Item->SendToHell();
      return true;
    }
  }

  return false;
}

truth character::RemoveShadowVeil()
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsShadowVeil())
    {
      item* Item = *i;
      Item->RemoveFromSlot();
      Item->SendToHell();
      return true;
    }

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Item = GetEquipment(c);

    if(Item && Item->IsShadowVeil())
    {
      Item->RemoveFromSlot();
      Item->SendToHell();
      return true;
    }
  }

  return false;
}

truth character::ReadItem(item* ToBeRead)
{
  if(ToBeRead->CanBeRead(this))
  {
    int LocalReadDifficulty = ToBeRead->GetBurnLevel() * sqrt(abs(ToBeRead->GetReadDifficulty())) / 2;
    if(GetAttribute(INTELLIGENCE) < LocalReadDifficulty)
    {
      ADD_MESSAGE("%s is completely unreadable.", ToBeRead->CHAR_NAME(DEFINITE));
      return false;
    }

    if(!GetLSquareUnder()->IsDark() || game::GetSeeWholeMapCheatMode())
    {
      if(StateIsActivated(CONFUSED) && !(RAND() & 7))
      {
        if(!ToBeRead->IsDestroyable(this))
          ADD_MESSAGE("You read some words of %s and understand exactly nothing.", ToBeRead->CHAR_NAME(DEFINITE));
        else
        {
          ADD_MESSAGE("%s is very confusing. Or perhaps you are just too confused?", ToBeRead->CHAR_NAME(DEFINITE));
          ActivateRandomState(SRC_CONFUSE_READ, 1000 + RAND() % 1500);
          ToBeRead->RemoveFromSlot();
          ToBeRead->SendToHell();
        }

        EditAP(-1000);
        return true;
      }

      if(ToBeRead->Read(this))
      {
        if(!game::WizardModeIsActive())
        {
          /* This AP is used to take the stuff out of backpack */
          DexterityAction(5);
        }

        return true;
      }
      else
        return false;
    }
    else
    {
      if(IsPlayer())
        ADD_MESSAGE("It's too dark here to read.");

      return false;
    }
  }
  else
  {
    if(IsPlayer())
      ADD_MESSAGE("You can't read this.");

    return false;
  }
}

void character::CalculateBurdenState()
{
  int OldBurdenState = BurdenState;
  long SumOfMasses = GetCarriedWeight();
  long CarryingStrengthUnits = long(GetCarryingStrength()) * 2500;

  if(SumOfMasses > (CarryingStrengthUnits << 1) + CarryingStrengthUnits)
    BurdenState = OVER_LOADED;
  else if(SumOfMasses > CarryingStrengthUnits << 1)
    BurdenState = STRESSED;
  else if(SumOfMasses > CarryingStrengthUnits)
    BurdenState = BURDENED;
  else
    BurdenState = UNBURDENED;

  if(!IsInitializing() && BurdenState != OldBurdenState)
    CalculateBattleInfo();
}

void character::Save(outputfile& SaveFile) const
{
  SaveFile << static_cast<ushort>(GetType());
  Stack->Save(SaveFile);
  SaveFile << ID;
  int c;

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile << BaseExperience[c];

  SaveFile << ExpModifierMap;
  SaveFile << NP << AP << Stamina << GenerationDanger << ScienceTalks
           << CounterToMindWormHatch;
  SaveFile << TemporaryState << EquipmentState << Money << GoingTo << RegenerationCounter << Route << Illegal;
  SaveFile.Put(!!IsEnabled());
  SaveFile << HomeData << BlocksSinceLastTurn << CommandFlags;
  SaveFile << WarnFlags << static_cast<ushort>(Flags);

  for(c = 0; c < BodyParts; ++c)
    SaveFile << BodyPartSlot[c] << OriginalBodyPartID[c];

  SaveLinkedList(SaveFile, TrapData);
  SaveFile << Action;

  for(c = 0; c < STATES; ++c)
    SaveFile << TemporaryStateCounter[c];

  if(GetTeam())
  {
    SaveFile.Put(true);
    SaveFile << Team->GetID();
  }
  else
    SaveFile.Put(false);

  if(GetTeam() && GetTeam()->GetLeader() == this)
    SaveFile.Put(true);
  else
    SaveFile.Put(false);

  SaveFile << AssignedName << PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile << CWeaponSkill[c];

  SaveFile << static_cast<ushort>(GetConfig());

  for(c = 0; c < MAX_EQUIPMENT_SLOTS; c++)
    SaveFile << MemorizedEquippedItemIDs[c];
}

void character::Load(inputfile& SaveFile)
{
  LoadSquaresUnder();
  int c;
  Stack->Load(SaveFile);
  SaveFile >> ID;
  game::AddCharacterID(this, ID);

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    SaveFile >> BaseExperience[c];

  SaveFile >> ExpModifierMap;
  SaveFile >> NP >> AP >> Stamina >> GenerationDanger >> ScienceTalks
           >> CounterToMindWormHatch;
  SaveFile >> TemporaryState >> EquipmentState >> Money >> GoingTo >> RegenerationCounter >> Route >> Illegal;

  if(!SaveFile.Get())
    Disable();

  SaveFile >> HomeData >> BlocksSinceLastTurn >> CommandFlags;
  SaveFile >> WarnFlags;
  WarnFlags &= ~WARNED;
  Flags |= ReadType<ushort>(SaveFile) & ~ENTITY_FLAGS;

  for(c = 0; c < BodyParts; ++c)
  {
    SaveFile >> BodyPartSlot[c] >> OriginalBodyPartID[c];

    item* BodyPart = *BodyPartSlot[c];

    if(BodyPart)
      BodyPart->Disable();
  }

  LoadLinkedList(SaveFile, TrapData);
  SaveFile >> Action;

  if(Action)
    Action->SetActor(this);

  for(c = 0; c < STATES; ++c)
    SaveFile >> TemporaryStateCounter[c];

  if(SaveFile.Get())
    SetTeam(game::GetTeam(ReadType<ulong>(SaveFile)));

  if(SaveFile.Get())
    GetTeam()->SetLeader(this);

  SaveFile >> AssignedName >> PolymorphBackup;

  for(c = 0; c < AllowedWeaponSkillCategories; ++c)
    SaveFile >> CWeaponSkill[c];

  databasecreator<character>::InstallDataBase(this, ReadType<ushort>(SaveFile));

  if(game::GetCurrentSavefileVersion()>=132){
    for(c = 0; c < MAX_EQUIPMENT_SLOTS; c++)
      SaveFile >> MemorizedEquippedItemIDs[c];
  }

  /////////////// loading ended /////////////////////////

  if(IsEnabled() && !game::IsInWilderness())
    for(c = 1; c < GetSquaresUnder(); ++c)
      GetSquareUnder(c)->SetCharacter(this);
}

truth character::Engrave(cfestring& What)
{
  GetLSquareUnder()->Engrave(What);
  return true;
}

truth character::MoveRandomly()
{
  if(!IsEnabled())
    return false;

  double DirChange = femath::NormalDistributedRand(0.03);
  RandomMoveDir += DirChange;
  WrapFRef(RandomMoveDir, 0., 8.);

  int Unit = Sign(DirChange);
  if(!Unit) Unit = RAND_2 ? 1 : -1;

  int Aim;
  if(femath::RandReal(1) < RandomMoveDir - floor(RandomMoveDir))
    Aim = floor(RandomMoveDir);
  else
    Aim = Wrap(static_cast<int>(ceil(RandomMoveDir)), 0, 8);

  for(int Delta = 0; abs(Delta) <= 4;)
  {
    int Dir = Wrap(Aim + Delta, 0, 8);
    v2 ToTry = game::GetClockwiseMoveVector(Dir);

    if(GetLevel()->IsValidPos(GetPos() + ToTry))
    {
      lsquare* Square = GetNearLSquare(GetPos() + ToTry);

      if(!Square->IsDangerous(this)
         && !Square->IsScary(this)
         && TryMove(ToTry, false, false))
      {
        if(Delta)
          RandomMoveDir = Dir;

        return true;
      }
    }

    Delta = -Delta;

    if(!Sign(Delta) || Sign(Delta) == Unit)
      Delta += Unit;
  }

  RandomMoveDir = femath::RandReal(8);
  return false;
}

truth character::TestForPickup(item* ToBeTested) const
{
  if(MakesBurdened(ToBeTested->GetWeight() + GetCarriedWeight()))
    return false;

  return true;
}

void character::AddScoreEntry(cfestring& Description, double Multiplier, truth AddEndLevel) const
{
  if(!game::WizardModeIsReallyActive())
  {
    highscore HScore(game::GetStateDir() + HIGH_SCORE_FILENAME);

    if(!HScore.CheckVersion())
    {
      if(game::Menu(0, v2(RES.X >> 1, RES.Y >> 1),
                    CONST_S("The highscore version doesn't match.\rDo you want to erase "
                            "previous records and start a new file?\rNote, if you answer "
                            "no, the score of your current game will be lost!\r"),
                    CONST_S("Yes\rNo\r"), LIGHT_GRAY))
        return;

      HScore.Clear();
    }

    festring Desc = game::GetPlayerName();
    Desc << ", " << Description;

    if(AddEndLevel)
    {
      if(game::IsInWilderness())
        Desc << " in the world map";
      else
        Desc << " in " << game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex());
    }

    HScore.Add(long(game::GetScore() * Multiplier), Desc);
    HScore.Save();
  }
}

truth character::CheckDeath(cfestring& Msg, ccharacter* Murderer, ulong DeathFlags)
{
  if(!IsEnabled())
    return true;

  if(game::IsSumoWrestling() && IsDead())
  {
    game::EndSumoWrestling(!!IsPlayer());
    return true;
  }

  if(DeathFlags & FORCE_DEATH || IsDead())
  {
    if(Murderer && Murderer->IsPlayer() && GetTeam()->GetKillEvilness())
      game::DoEvilDeed(GetTeam()->GetKillEvilness());

    festring SpecifierMsg;
    int SpecifierParts = 0;

    if(GetPolymorphBackup())
    {
      SpecifierMsg << " polymorphed into ";
      id::AddName(SpecifierMsg, INDEFINITE);
      ++SpecifierParts;
    }

    if(!(DeathFlags & IGNORE_TRAPS) && IsStuck())
    {
      if(SpecifierParts++)
        SpecifierMsg << " and";

      SpecifierMsg << " caught in " << GetTrapDescription();
    }

    if(GetAction()
       && !(DeathFlags & IGNORE_UNCONSCIOUSNESS
            && GetAction()->IsUnconsciousness()))
    {
      festring ActionMsg = GetAction()->GetDeathExplanation();

      if(!ActionMsg.IsEmpty())
      {
        if(SpecifierParts > 1)
          SpecifierMsg = ActionMsg << ',' << SpecifierMsg;
        else
        {
          if(SpecifierParts)
            SpecifierMsg << " and";

          SpecifierMsg << ActionMsg;
        }

        ++SpecifierParts;
      }
    }

    festring NewMsg = Msg;

    if(Murderer == this)
    {
      SEARCH_N_REPLACE(NewMsg, "@bkp", CONST_S("by ") + GetPossessivePronoun(false) + " own");
      SEARCH_N_REPLACE(NewMsg, "@bk", CONST_S("by ") + GetObjectPronoun(false) + "self");
      SEARCH_N_REPLACE(NewMsg, "@k", GetObjectPronoun(false) + "self");
    }
    else
    {
      SEARCH_N_REPLACE(NewMsg, "@bkp", CONST_S("by ") + Murderer->GetName(INDEFINITE) + "'s");
      SEARCH_N_REPLACE(NewMsg, "@bk", CONST_S("by ") + Murderer->GetName(INDEFINITE));
      SEARCH_N_REPLACE(NewMsg, "@k", CONST_S("by ") + Murderer->GetName(INDEFINITE));
    }

    if(SpecifierParts)
      NewMsg << " while" << SpecifierMsg;

    if(IsPlayer() && game::WizardModeIsActive())
      ADD_MESSAGE("Death message: %s. Score: %ld.", NewMsg.CStr(), game::GetScore());

    Die(Murderer, NewMsg, DeathFlags);
    return true;
  }
  else
    return false;
}

truth character::CheckStarvationDeath(cfestring& Msg)
{
  if(GetNP() < 1 && UsesNutrition() && !(StateIsActivated(FASTING)))
    return CheckDeath(Msg, 0, FORCE_DEATH);
  else
    return false;
}

void character::ThrowItem(int Direction, item* ToBeThrown)
{
  if(Direction > 7)
    ABORT("Throw in TOO odd direction...");

  ToBeThrown->Fly(this, Direction, GetAttribute(ARM_STRENGTH),
    ToBeThrown->IsWeapon(this) && !ToBeThrown->IsBroken());
}

void character::HasBeenHitByItem(character* Thrower, item* Thingy, int Damage, double ToHitValue, int Direction)
{
  if(IsPlayer())
    ADD_MESSAGE("%s hits you.", Thingy->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s hits %s.", Thingy->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));

  int BodyPart = ChooseBodyPartToReceiveHit(ToHitValue, DodgeValue);
  int WeaponSkillHits = Thrower ? CalculateWeaponSkillHits(Thrower) : 0;
  int DoneDamage = ReceiveBodyPartDamage(Thrower, Damage, PHYSICAL_DAMAGE, BodyPart, Direction);
  truth Succeeded = (GetBodyPart(BodyPart) && HitEffect(Thrower, Thingy, Thingy->GetPos(), THROW_ATTACK,
                                                        BodyPart, Direction, !DoneDamage, false, DoneDamage)) || DoneDamage;

  if(Succeeded && Thrower)
    Thrower->WeaponSkillHit(Thingy, THROW_ATTACK, WeaponSkillHits);

  festring DeathMsg = CONST_S("killed by a flying ") + Thingy->GetName(UNARTICLED);

  if(CheckDeath(DeathMsg, Thrower))
    return;

  if(Thrower)
  {
    if(Thrower->CanBeSeenByPlayer())
      DeActivateVoluntaryAction(CONST_S("The attack of ") + Thrower->GetName(DEFINITE) + CONST_S(" interrupts you."));
    else
      DeActivateVoluntaryAction(CONST_S("The attack interrupts you."));
  }
  else
    DeActivateVoluntaryAction(CONST_S("The hit interrupts you."));
}

truth character::DodgesFlyingItem(item* Item, double ToHitValue)
{
  return !Item->EffectIsGood() && RAND() % int(100 + ToHitValue / DodgeValue * 100) < 100;
}

character* AutoPlayLastChar=NULL;
const int iMaxWanderTurns=20;
const int iMinWanderTurns=3;

/**
 * 5 seems good, broken cheap weapons, stones, very cheap weapons non broken etc
 * btw, lantern price is currently 10.
 */
static int iMaxValueless = 5;

v2 v2KeepGoingTo=v2(0,0);
v2 v2TravelingToAnotherDungeon=v2(0,0);
int iWanderTurns=iMinWanderTurns;
bool bAutoPlayUseRandomNavTargetOnce=false;
std::vector<v2> vv2DebugDrawSqrPrevious;
v2 v2LastDropPlayerWasAt=v2(0,0);
std::vector<v2> vv2FailTravelToTargets;
std::vector<v2> vv2WrongGoingTo;

void character::AutoPlayAIReset(bool bFailedToo)
{ DBG7(bFailedToo,iWanderTurns,DBGAV2(v2KeepGoingTo),DBGAV2(v2TravelingToAnotherDungeon),DBGAV2(v2LastDropPlayerWasAt),vv2FailTravelToTargets.size(),vv2DebugDrawSqrPrevious.size());
  v2KeepGoingTo=v2(0,0); //will retry
  v2TravelingToAnotherDungeon=v2(0,0);
  iWanderTurns=0; // warning: this other code was messing the logic ---> if(iWanderTurns<iMinWanderTurns)iWanderTurns=iMinWanderTurns; //to wander just a bit looking for random spot from where Route may work
  bAutoPlayUseRandomNavTargetOnce=false;
  v2LastDropPlayerWasAt=v2(0,0);
  vv2DebugDrawSqrPrevious.clear();

  PLAYER->TerminateGoingTo();

  if(bFailedToo){
    vv2FailTravelToTargets.clear();
    vv2WrongGoingTo.clear();
  }
}
truth character::AutoPlayAISetAndValidateKeepGoingTo(v2 v2KGTo)
{
  v2KeepGoingTo=v2KGTo;
  SetGoingTo(v2KeepGoingTo); DBG3(DBGAV2(GetPos()),DBGAV2(GoingTo),DBGAV2(v2KeepGoingTo));
  CreateRoute();
  if(!Route.empty())
    return true;

  DBG1("RouteCreationFailed");
  TerminateGoingTo(); //redundant?
  vv2FailTravelToTargets.push_back(v2KeepGoingTo); DBG3("BlockGoToDestination",DBGAV2(v2KeepGoingTo),vv2FailTravelToTargets.size());
  bAutoPlayUseRandomNavTargetOnce=true;

  AutoPlayAIReset(false);

  return false;
}

void character::AutoPlayAIDebugDrawSquareRect(v2 v2SqrPos, col16 color, int iPrintIndex, bool bWide, bool bKeepColor)
{
  static v2 v2ScrPos=v2(0,0); //static to avoid instancing
  static int iAddPos;iAddPos=bWide?2:1;
  static int iSubBorder;iSubBorder=bWide?3:2;
  if(game::OnScreen(v2SqrPos)){
    v2ScrPos=game::CalculateScreenCoordinates(v2SqrPos);

    DOUBLE_BUFFER->DrawRectangle(
        v2ScrPos.X+iAddPos, v2ScrPos.Y+iAddPos,
        v2ScrPos.X+TILE_SIZE-iSubBorder, v2ScrPos.Y+TILE_SIZE-iSubBorder,
        color, bWide);

    if(iPrintIndex>-1)
      FONT->Printf(DOUBLE_BUFFER, v2(v2ScrPos.X+1,v2ScrPos.Y+5), DARK_GRAY, "%d", iPrintIndex);

    if(!bKeepColor)
      vv2DebugDrawSqrPrevious.push_back(v2SqrPos);
  }
}

const int iVisitAgainMax=10;
int iVisitAgainCount=iVisitAgainMax;
std::vector<lsquare*> vv2AllDungeonSquares;
bool character::AutoPlayAICheckAreaLevelChangedAndReset()
{
  static area* areaPrevious=NULL;
  area* Area = game::GetCurrentArea();
  if(Area != areaPrevious){
    areaPrevious=Area;

    iVisitAgainCount=iVisitAgainMax;

    vv2DebugDrawSqrPrevious.clear();

    vv2AllDungeonSquares.clear();
    if(!game::IsInWilderness())
      for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){ for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
        vv2AllDungeonSquares.push_back(game::GetCurrentLevel()->GetLSquare(iX, iY));
      }}

    return true;
  }

  return false;
}

void character::AutoPlayAIDebugDrawOverlay()
{
  if(!game::WizardModeIsActive())return;

  AutoPlayAICheckAreaLevelChangedAndReset();

  // redraw previous to clean them
  area* Area = game::GetCurrentArea(); //got the Area to draw in the wilderness too and TODO navigate there one day
  std::vector<v2> vv2DebugDrawSqrPreviousCopy(vv2DebugDrawSqrPrevious);
  for(int i=0;i<vv2DebugDrawSqrPreviousCopy.size();i++){
//    Area->GetSquare(vv2DebugDrawSqrPrevious[i])->SendNewDrawRequest();
//    square* sqr = Area->GetSquare(vv2DebugDrawSqrPrevious[i]);
//    if(sqr)sqr->SendStrongNewDrawRequest(); //TODO sqr NULL?
    AutoPlayAIDebugDrawSquareRect(vv2DebugDrawSqrPreviousCopy[i],DARK_GRAY);
  }

  // draw new ones
  vv2DebugDrawSqrPrevious.clear(); //empty before fillup below

  for(int i=0;i<vv2FailTravelToTargets.size();i++)
    AutoPlayAIDebugDrawSquareRect(vv2FailTravelToTargets[i],RED,i==(vv2FailTravelToTargets.size()-1),i,true);

  if(!PLAYER->Route.empty())
    for(int i=0;i<PLAYER->Route.size();i++)
      AutoPlayAIDebugDrawSquareRect(PLAYER->Route[i],GREEN);

  if(!v2KeepGoingTo.Is0())
    AutoPlayAIDebugDrawSquareRect(v2KeepGoingTo,BLUE,PLAYER->Route.size(),true);
  else if(iWanderTurns>0)
    AutoPlayAIDebugDrawSquareRect(PLAYER->GetPos(),YELLOW,iWanderTurns);

  for(int i=0;i<vv2WrongGoingTo.size();i++)
    AutoPlayAIDebugDrawSquareRect(vv2WrongGoingTo[i],BLUE,i,false,true);
}

truth character::AutoPlayAIDropThings()
{
//  level* lvl = game::GetCurrentLevel(); DBG1(lvl);
//  area* Area = game::GetCurrentArea();

  /**
   *  unburden
   */
  bool bDropSomething = false;
  static item* eqDropChk=NULL;
  item* eqBroken=NULL;
  for(int i=0;i<GetEquipments();i++){
    eqDropChk=GetEquipment(i);
    if(eqDropChk!=NULL && eqDropChk->IsBroken()){ DBG2("chkDropBroken",eqDropChk);
      eqBroken=eqDropChk;
      bDropSomething=true;
      break;
    }
  }

  if(!bDropSomething && GetBurdenState() == STRESSED){
    if(clock()%100<5){ //5% chance to drop something weighty randomly every turn
      bDropSomething=true; DBGLN;
    }
  }

  if(!bDropSomething && GetBurdenState() == OVER_LOADED){
    bDropSomething=true;
  }

  if(bDropSomething){ DBG1("DropSomething");
    item* dropMe=NULL;
    if(eqBroken!=NULL)dropMe=eqBroken;

    item* heaviest=NULL;
    item* cheapest=NULL;

//    bool bFound=false;
//    for(int k=0;k<2;k++){
//      if(dropMe!=NULL)break;
//    static item* eqDropChk=NULL;
//    for(int i=0;i<GetEquipments();i++){
//      eqDropChk=GetEquipment(i);
//      if(eqDropChk!=NULL && eqDropChk->IsBroken()){
//        dropMe=eqDropChk;
//        break;
//      }
//    }

    if(dropMe==NULL){
      static itemvector vit;vit.clear();GetStack()->FillItemVector(vit);
      for(int i=0;i<vit.size();i++){ DBG4("CurrentChkToDrop",vit[i]->GetName(DEFINITE).CStr(),vit[i]->GetTruePrice(),vit[i]->GetWeight());
        if(vit[i]->IsEncryptedScroll())continue;
//        if(!bPlayerHasLantern && dynamic_cast<lantern*>(vit[i])!=NULL){
//          bPlayerHasLantern=true; //will keep only the 1st lantern
//          continue;
//        }

        if(vit[i]->IsBroken()){ //TODO use repair scroll?
          dropMe=vit[i];
          break;
        }

        if(heaviest==NULL)heaviest=vit[i];
        if(cheapest==NULL)cheapest=vit[i];

//        switch(k){
//        case 0: //better not implement this as a user function as that will remove the doubt about items values what is another fun challenge :)
          if(vit[i]->GetTruePrice() < cheapest->GetTruePrice()) //cheapest
            cheapest=vit[i];
//          break;
//        case 1: //this could be added as user function to avoid browsing the drop list, but may not be that good...
          if(vit[i]->GetWeight() > heaviest->GetWeight()) //heaviest
            heaviest=vit[i];
//          break;
//        }
      }
    }

    if(heaviest!=NULL && cheapest!=NULL){
      if(dropMe==NULL && heaviest==cheapest)
        dropMe=heaviest;

      if(dropMe==NULL && cheapest->GetTruePrice()<=iMaxValueless){ DBG2("DropValueless",cheapest->GetName(DEFINITE).CStr());
        dropMe=cheapest;
      }

      if(dropMe==NULL){
        // the worst price VS weight will be dropped
        float fC = cheapest ->GetTruePrice()/(float)cheapest ->GetWeight();
        float fW = heaviest->GetTruePrice()/(float)heaviest->GetWeight(); DBG3("PriceVsWeightRatio",fC,fW);
        if(fC < fW){
          dropMe = cheapest;
        }else{
          dropMe = heaviest;
        }
      }

      if(dropMe==NULL)
        dropMe = clock()%2==0 ? heaviest : cheapest;
    }

    // chose a throw direction
    if(dropMe!=NULL){
      static std::vector<int> vv2DirBase;static bool bDummyInit = [](){for(int i=0;i<8;i++)vv2DirBase.push_back(i);return true;}();
      std::vector<int> vv2Dir(vv2DirBase);
      int iDirOk=-1;
      v2 v2DropAt(0,0);
      lsquare* lsqrDropAt=NULL;
      for(int i=0;i<8;i++){
        int k = clock()%vv2Dir.size(); //random chose from remaining TODO could be where there is NPC foe
        int iDir = vv2Dir[k]; //collect direction value
        vv2Dir.erase(vv2Dir.begin() + k); //remove using the chosen index to prepare next random choice

        v2 v2Dir = game::GetMoveVector(iDir);
        v2 v2Chk = GetPos() + v2Dir;
        if(game::GetCurrentLevel()->IsValidPos(v2Chk)){
          lsquare* lsqrChk=game::GetCurrentLevel()->GetLSquare(v2Chk);
          if(lsqrChk->IsFlyable()){
            iDirOk = iDir;
            v2DropAt = v2Chk;
            lsqrDropAt=lsqrChk;
            break;
          }
        }
      };DBGLN;

      if(iDirOk==-1){iDirOk=clock()%8;DBG2("RandomDir",iDirOk);}DBGLN; //TODO should just drop may be? unless hitting w/e is there could help

      if(iDirOk>-1){DBG2("KickOrThrow",iDirOk);
        static itemcontainer* itc;itc = dynamic_cast<itemcontainer*>(dropMe);DBGLN;
        static humanoid* h;h = dynamic_cast<humanoid*>(this);DBGLN;
        DBG8("CanKickLockedChest",lsqrDropAt,itc,itc?itc->IsLocked():-1,CanKick(),h,h?h->GetLeftLeg():0,h?h->GetRightLeg():0);
        if(lsqrDropAt && itc && itc->IsLocked() && CanKick() && h && h->GetLeftLeg() && h->GetRightLeg()){DBGLN;
          dropMe->MoveTo(lsqrDropAt->GetStack());DBGLN; //drop in front..
          Kick(lsqrDropAt,iDirOk,true);DBGLN; // ..to kick it
        }else{DBGLN;
          ThrowItem(iDirOk, dropMe); DBG5("DropThrow",iDirOk,dropMe->GetName(DEFINITE).CStr(),dropMe->GetTruePrice(),dropMe->GetWeight());
        }
      }else{DBGLN;
        dropMe->MoveTo(GetLSquareUnder()->GetStack());DBGLN; //just drop
      }

      v2LastDropPlayerWasAt=GetPos();DBGSV2(v2LastDropPlayerWasAt);

      return true;
    }

    DBG1("AutoPlayNeedsImprovement:DropItem");
    ADD_MESSAGE("%s says \"I need more intelligence to drop trash...\"", CHAR_NAME(DEFINITE)); // improve the dropping AI
    //TODO stop autoplay mode? if not, something random may happen some time and wont reach here ex.: spoil, fire, etc..
  }

  return false;
}

truth character::AutoPlayAIEquipAndPickup(bool bPlayerHasLantern)
{
  static humanoid* h;h = dynamic_cast<humanoid*>(this);
  if(h==NULL)return false;

  if(h->AutoPlayAIequip())
    return true;

  if(GetBurdenState()!=OVER_LOADED){ //DBG2("",CommandFlags&DONT_CHANGE_EQUIPMENT);
    if(v2LastDropPlayerWasAt!=GetPos()){
      static bool bHoarder=true; //TODO wizard autoplay AI config exclusive felist

      if(CheckForUsefulItemsOnGround(false))
        if(!bHoarder)
          return true;

      //just pick up any useful stuff
      static itemvector vit;vit.clear();GetStackUnder()->FillItemVector(vit);
      for(uint c = 0; c < vit.size(); ++c){
        if(!vit[c]->CanBeSeenBy(this))continue;
        if(!vit[c]->IsPickable(this))continue;
        if(vit[c]->GetSquaresUnder()!=1)continue; //avoid big corpses 2x2

        if(!bPlayerHasLantern && vit[c]->IsOnFire(this)){
          //ok
        }else{
          if(vit[c]->IsBroken())continue;
          if(vit[c]->GetTruePrice()<=iMaxValueless)continue; //mainly to avoid all rocks from broken walls
          if(vit[c]->GetSpoilLevel()>0)continue;
        }

        static itemcontainer* itc;itc = dynamic_cast<itemcontainer*>(vit[c]);
        if(itc && !itc->IsLocked()){
          static itemvector vitItc;vitItc.clear();itc->GetContained()->FillItemVector(vitItc);
          for(uint d = 0; d < vitItc.size(); ++d)
            vitItc[d]->MoveTo(itc->GetLSquareUnder()->GetStack());
          continue;
        }

        vit[c]->MoveTo(GetStack()); DBG2("pickup",vit[c]->GetNameSingular().CStr());
//          if(GetBurdenState()==OVER_LOADED)ThrowItem(clock()%8,ItemVector[c]);
//          return true;
        if(!bHoarder)
          return true;
      }
    }
  }

  return false;
}

static const int iMoreThanMaxDist=10000000; //TODO should be max integer but this will do for now in 2018 :)
truth character::AutoPlayAITestValidPathTo(v2 v2To)
{
  return AutoPlayAIFindWalkDist(v2To) < iMoreThanMaxDist;
}

int character::AutoPlayAIFindWalkDist(v2 v2To)
{
  static bool bUseSimpleDirectDist=false; //very bad navigation this is
  if(bUseSimpleDirectDist)return (v2To - GetPos()).GetLengthSquare();

  static v2 GoingToBkp;GoingToBkp = GoingTo; //IsGoingSomeWhere() ? GoingTo : v2(0,0);

  SetGoingTo(v2To);
  CreateRoute();
  static int iDist;iDist=Route.size();
  TerminateGoingTo();

  if(GoingToBkp!=ERROR_V2){ DBG2("Warning:WrongUsage:ShouldBeGoingNoWhere",DBGAV2(GoingToBkp));
    SetGoingTo(GoingToBkp);
    CreateRoute();
  }

  return iDist>0?iDist:iMoreThanMaxDist;
}

truth character::AutoPlayAINavigateDungeon(bool bPlayerHasLantern)
{
  /**
   * navigate the unknown dungeon
   */
  std::vector<v2> v2Exits;
  if(v2KeepGoingTo.Is0()){ DBG1("TryNewMoveTarget");
    // target undiscovered squares to explore
    v2 v2PreferedTarget(0,0);

    int iNearestLanterOnFloorDist = iMoreThanMaxDist;
    v2 v2PreferedLanternOnFloorTarget(0,0);

    v2 v2NearestUndiscovered(0,0);
    int iNearestUndiscoveredDist=iMoreThanMaxDist;
    std::vector<v2> vv2UndiscoveredValidPathSquares;

    lsquare* lsqrNearestSquareWithWallLantern=NULL;
    lsquare* lsqrNearestDropWallLanternAt=NULL;
    stack* stkNearestDropWallLanternAt = NULL;
    int iNearestSquareWithWallLanternDist=iMoreThanMaxDist;
    item* itNearestWallLantern=NULL;

    /***************************************************************
     * scan whole dungeon squares
     */
    for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){ for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
      lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(iX,iY);

      olterrain* olt = lsqr->GetOLTerrain();
      if(olt && (olt->GetConfig() == STAIRS_UP || olt->GetConfig() == STAIRS_DOWN)){
        v2Exits.push_back(v2(lsqr->GetPos())); DBGSV2(v2Exits[v2Exits.size()-1]);
      }

      stack* stkSqr = lsqr->GetStack();
      static itemvector vit;vit.clear();stkSqr->FillItemVector(vit);
      bool bAddValidTargetSquare=true;

      // find nearest wall lantern
      if(!bPlayerHasLantern && !CanMoveOn(lsqr)){ //probably a wall
        for(int n=0;n<vit.size();n++){
          if(vit[n]->IsLanternOnWall() && !vit[n]->IsBroken()){
            static stack* stkDropWallLanternAt;stkDropWallLanternAt = lsqr->GetStackOfAdjacentSquare(vit[n]->GetSquarePosition());
            static lsquare* lsqrDropWallLanternAt;lsqrDropWallLanternAt =
              stkDropWallLanternAt?stkDropWallLanternAt->GetLSquareUnder():NULL;

            if(stkDropWallLanternAt && lsqrDropWallLanternAt && CanMoveOn(lsqrDropWallLanternAt)){
              int iDist = AutoPlayAIFindWalkDist(lsqrDropWallLanternAt->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();
              if(lsqrNearestSquareWithWallLantern==NULL || iDist<iNearestSquareWithWallLanternDist){
                iNearestSquareWithWallLanternDist=iDist;

                lsqrNearestSquareWithWallLantern=lsqr;
                itNearestWallLantern=vit[n]; DBG3(iNearestSquareWithWallLanternDist,DBGAV2(lsqr->GetPos()),DBGAV2(GetPos()));
                lsqrNearestDropWallLanternAt=lsqrDropWallLanternAt;
                stkNearestDropWallLanternAt=stkDropWallLanternAt;
              }
            }

            break;
          }
        }
      }

      if(bAddValidTargetSquare && !CanMoveOn(lsqr))
        bAddValidTargetSquare=false;

      bool bIsFailToTravelSquare=false;
      if(bAddValidTargetSquare){
        for(int j=0;j<vv2FailTravelToTargets.size();j++)
          if(vv2FailTravelToTargets[j]==lsqr->GetPos()){
            bAddValidTargetSquare=false;
            bIsFailToTravelSquare=true;
            break;
          }
      }

      if(!bIsFailToTravelSquare){

//          if(bAddValidTargetSquare && v2PreferedTarget.Is0() && (lsqr->HasBeenSeen() || !bPlayerHasLantern)){
        if(bAddValidTargetSquare && (lsqr->HasBeenSeen() || !bPlayerHasLantern)){
          bool bVisitAgain=false;
          if(iVisitAgainCount>0 || !bPlayerHasLantern){
            if(stkSqr!=NULL && stkSqr->GetItems()>0){
              for(int n=0;n<vit.size();n++){ DBG1(vit[n]);DBG1(vit[n]->GetID());DBG1(vit[n]->GetType());DBG3("VisitAgain:ChkItem",vit[n]->GetNameSingular().CStr(),vit.size());
                if(vit[n]->IsBroken())continue; DBGLN;

                static bool bIsLanternOnFloor;bIsLanternOnFloor = dynamic_cast<lantern*>(vit[n])!=NULL;// || vit[n]->IsOnFire(this); DBGLN;

                if( // if is useful to the AutoPlay AI endless tests
                    vit[n]->IsShield  (this) ||
                    vit[n]->IsWeapon  (this) ||
                    vit[n]->IsArmor   (this) ||
                    vit[n]->IsAmulet  (this) ||
                    vit[n]->IsZappable(this) ||
                    vit[n]->IsRing    (this) ||
                    bIsLanternOnFloor
                ){
                  bVisitAgain=true;

                  if(bIsLanternOnFloor && !bPlayerHasLantern){
                    static int iDist;iDist = AutoPlayAIFindWalkDist(lsqr->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();
                    if(iDist<iNearestLanterOnFloorDist){
                      iNearestLanterOnFloorDist=iDist;
                      v2PreferedLanternOnFloorTarget = lsqr->GetPos(); DBG2("PreferLanternAt",DBGAV2(lsqr->GetPos()))
                    }
                  }else{
                    iVisitAgainCount--;
                  }

                  DBG4(bVisitAgain,DBGAV2(lsqr->GetPos()),iVisitAgainCount,bIsLanternOnFloor);
                  break;
                }
              }
            }
          }

          if(!bVisitAgain)bAddValidTargetSquare=false;
        }

      }

      if(bAddValidTargetSquare){ DBG2("addValidSqr",DBGAV2(lsqr->GetPos()));
        static int iDist;iDist=AutoPlayAIFindWalkDist(lsqr->GetPos()); //(lsqr->GetPos() - GetPos()).GetLengthSquare();

        if(iDist<iMoreThanMaxDist) //add only valid paths
          vv2UndiscoveredValidPathSquares.push_back(lsqr->GetPos());

        if(iDist<iNearestUndiscoveredDist){
          iNearestUndiscoveredDist=iDist;
          v2NearestUndiscovered=lsqr->GetPos(); DBG3(iNearestUndiscoveredDist,DBGAV2(lsqr->GetPos()),DBGAV2(GetPos()));
        }
      }
    }} DBG2(DBGAV2(v2PreferedTarget),vv2UndiscoveredValidPathSquares.size());

    /***************************************************************
     * define prefered navigation target
     */
    if(!bPlayerHasLantern && v2PreferedTarget.Is0()){
      bool bUseWallLantern=false;
      if(!v2PreferedLanternOnFloorTarget.Is0() && lsqrNearestSquareWithWallLantern!=NULL){
        if(iNearestLanterOnFloorDist <= iNearestSquareWithWallLanternDist){
          v2PreferedTarget=v2PreferedLanternOnFloorTarget;
        }else{
          bUseWallLantern=true;
        }
      }else if(!v2PreferedLanternOnFloorTarget.Is0()){
        v2PreferedTarget=v2PreferedLanternOnFloorTarget;
      }else if(lsqrNearestSquareWithWallLantern!=NULL){
        bUseWallLantern=true;
      }

      if(bUseWallLantern){
        /**
         * target to nearest wall lantern
         * check for lanterns on walls of adjacent squares if none found on floors
         */
        itNearestWallLantern->MoveTo(stkNearestDropWallLanternAt); // the AI is prepared to get things from the floor only so "magically" drop it :)
        v2PreferedTarget = lsqrNearestDropWallLanternAt->GetPos(); DBG2("PreferWallLanternAt",DBGAV2(lsqrNearestDropWallLanternAt->GetPos()))
      }

    }

    /***************************************************************
     * validate and set new navigation target
     */
//    DBG9("AllNavigatePossibilities",DBGAV2(v2PreferedTarget),DBGAV2(v2PreferedLanternOnFloorTarget),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2(),DBGAV2());
    v2 v2NewKGTo=v2(0,0);

    if(v2NewKGTo.Is0()){
      //TODO if(!v2PreferedTarget.Is0){ // how can this not be compiled? error: cannot convert ‘v2::Is0’ from type ‘truth (v2::)() const {aka bool (v2::)() const}’ to type ‘bool’
      if(v2PreferedTarget.GetLengthSquare()>0)
        if(AutoPlayAITestValidPathTo(v2PreferedTarget))
          v2NewKGTo=v2PreferedTarget; DBGSV2(v2PreferedTarget);
    }

    if(v2NewKGTo.Is0()){
      if(bAutoPlayUseRandomNavTargetOnce){ //these targets were already path validated and are safe to use!
        v2NewKGTo=vv2UndiscoveredValidPathSquares[clock()%vv2UndiscoveredValidPathSquares.size()]; DBG2("RandomTarget",DBGAV2(v2NewKGTo));
        bAutoPlayUseRandomNavTargetOnce=false;
      }else{    //find nearest
        if(!v2NearestUndiscovered.Is0()){
          v2NewKGTo=v2NearestUndiscovered; DBGSV2(v2NearestUndiscovered);
        }
      }
    }

    if(v2NewKGTo.Is0()){ //no new destination: fully explored
      if(v2Exits.size()>0){
        if(game::GetCurrentDungeonTurnsCount()==0){ DBG1("Dungeon:FullyExplored:FirstTurn");
          iWanderTurns=100+clock()%300; DBG2("WanderALotOnFullyExploredLevel",iWanderTurns); //just move around a lot, some NPC may spawn
        }else{
          // travel between dungeons if current fully explored
          v2 v2Try = v2Exits[clock()%v2Exits.size()];
          if(AutoPlayAITestValidPathTo(v2Try))
            v2NewKGTo = v2TravelingToAnotherDungeon = v2Try; DBGSV2(v2TravelingToAnotherDungeon);
        }
      }else{
        DBG1("AutoPlayNeedsImprovement:Navigation")
        ADD_MESSAGE("%s says \"I need more intelligence to move around...\"", CHAR_NAME(DEFINITE)); // improve the dropping AI
        //TODO stop autoplay mode?
      }
    }

    if(v2NewKGTo.Is0()){ DBG1("Desperately:TryAnyRandomTargetNavWithValidPath");
      std::vector<lsquare*> vlsqrChk(vv2AllDungeonSquares);

      while(vlsqrChk.size()>0){
        static int i;i=clock()%vlsqrChk.size();
        static v2 v2Chk; v2Chk = vlsqrChk[i]->GetPos();

        if(!AutoPlayAITestValidPathTo(v2Chk)){
          vlsqrChk.erase(vlsqrChk.begin()+i);
        }else{
          v2NewKGTo=v2Chk;
          break;
        }
      }
    }

    if(!v2NewKGTo.Is0()){
      AutoPlayAISetAndValidateKeepGoingTo(v2NewKGTo);
    }

    DBG1("TODO:too complex paths are failing... improve create path method?");
  }

  if(!v2KeepGoingTo.Is0()){
    if(v2KeepGoingTo==GetPos()){ DBG3("ReachedDestination",DBGAV2(v2KeepGoingTo),DBGAV2(GoingTo));
      //wander a bit before following new target destination
      iWanderTurns=(clock()%iMaxWanderTurns)+iMinWanderTurns; DBG2("WanderAroundAtReachedDestination",iWanderTurns);

//      v2KeepGoingTo=v2(0,0);
//      TerminateGoingTo();
      AutoPlayAIReset(false);
      return true;
    }

//    CheckForUsefulItemsOnGround(false); DBGSV2(GoingTo);
//    CheckForEnemies(false, true, false, false); DBGSV2(GoingTo);

//    if(!IsGoingSomeWhere() || v2KeepGoingTo!=GoingTo){ DBG3("ForceKeepGoingTo",DBGAV2(v2KeepGoingTo),DBGAV2(GoingTo));
//      SetGoingTo(v2KeepGoingTo);
//    }
    static int iForceGoingToCountDown=10;
    static v2 v2GoingToBkp;v2GoingToBkp=GoingTo;
    if(!v2KeepGoingTo.IsAdjacent(GoingTo)){
      if(iForceGoingToCountDown==0){
        DBG4("ForceKeepGoingTo",DBGAV2(v2KeepGoingTo),DBGAV2(GoingTo),DBGAV2(GetPos()));

        if(!AutoPlayAISetAndValidateKeepGoingTo(v2KeepGoingTo)){
          static int iSetFailTeleportCountDown=10;
          iSetFailTeleportCountDown--;
          vv2WrongGoingTo.push_back(v2GoingToBkp);
          if(iSetFailTeleportCountDown==0){
            AutoPlayAITeleport(false);
            AutoPlayAIReset(true); //refresh to test/try it all again
            iSetFailTeleportCountDown=10;
          }
        }
        DBGSV2(GoingTo);
        return true;
      }else{
        iForceGoingToCountDown--;
      }
    }else{
      iForceGoingToCountDown=10;
    }

    /**
     * Determinedly blindly moves towards target, the goal is to Navigate!
     *
     * this has several possible status if returning false...
     * so better do not decide anything based on it?
     */
    MoveTowardsTarget(false);

//    if(!MoveTowardsTarget(false)){ DBG3("OrFailedGoingTo,OrReachedDestination...",DBGAV2(GoingTo),DBGAV2(GetPos())); // MoveTowardsTarget may break the GoingTo EVEN if it succeeds?????
//      TerminateGoingTo();
//      v2KeepGoingTo=v2(0,0); //reset only this one to try again
//      GetAICommand(); //wander once for randomicity
//    }

    return true;
  }

  return false;
}

truth character::AutoPlayAICommand(int& rKey)
{ DBGSV2(GetPos());
  if(AutoPlayLastChar!=this){
    AutoPlayAIReset(true);
    AutoPlayLastChar=this;
  }

  if(AutoPlayAICheckAreaLevelChangedAndReset()){
    AutoPlayAIReset(true);
  }

  static bool bDummy_initDbg = [](){game::AddDebugDrawOverlayFunction(&AutoPlayAIDebugDrawOverlay);return true;}();

  truth bPlayerHasLantern=false;
  static itemvector vit;vit.clear();GetStack()->FillItemVector(vit);
  for(int i=0;i<vit.size();i++){
    if(dynamic_cast<lantern*>(vit[i])!=NULL || vit[i]->IsOnFire(this)){
      bPlayerHasLantern=true; //will keep only the 1st lantern
      break;
    }
  }

  if(StateIsActivated(PANIC)){ DBG1("Wandering:InPanic");
    for(int c = 1; c <= GODS; ++c)
      if(game::GetGod(c)->IsKnown())
        if(clock()%10==0){
          game::GetGod(c)->Pray(); DBG2("PrayingTo",game::GetGod(c)->GetName());
          break;
        }

    iWanderTurns=1; // to regain control as soon it is a ghost anymore as it can break navigation when inside walls
  }

  //TODO this doesnt work??? -> if(IsPolymorphed()){ //to avoid some issues TODO but could just check if is a ghost
//  if(dynamic_cast<humanoid*>(this) == NULL){ //this avoid some issues TODO but could just check if is a ghost
//  if(StateIsActivated(ETHEREAL_MOVING)){ //this avoid many issues
  if(dynamic_cast<ghost*>(this) != NULL){ DBG1("Wandering:Ghost"); //this avoid many issues
    iWanderTurns=1; // to regain control as soon it is a ghost anymore as it can break navigation when inside walls
  }

  if(AutoPlayAIDropThings())
    return true;

  if(AutoPlayAIEquipAndPickup(bPlayerHasLantern))
    return true;

  if(iWanderTurns>0){
    GetAICommand(); DBG2("Wandering",iWanderTurns); //fallback to default TODO never reached?
    iWanderTurns--;
    return true;
  }

  /***************************************************************************************************
   * WANDER above here
   * NAVIGATE below here
   ***************************************************************************************************/

  /**
   * travel between dungeons
   */
  if(!v2TravelingToAnotherDungeon.Is0() && GetPos() == v2TravelingToAnotherDungeon){
    bool bTravel=false;
    lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(v2TravelingToAnotherDungeon);
//    square* sqr = Area->GetSquare(v2TravelingToAnotherDungeon);
    olterrain* ot = lsqr->GetOLTerrain();
//    oterrain* ot = sqr->GetOTerrain();
    if(ot){
      if(ot->GetConfig() == STAIRS_UP){
        rKey='<';
        bTravel=true;
      }

      if(ot->GetConfig() == STAIRS_DOWN){
        rKey='>';
        bTravel=true;
      }
    }

    if(bTravel){ DBG3("travel",DBGAV2(v2TravelingToAnotherDungeon),rKey);
      AutoPlayAIReset(true);
      return false; //so the new/changed key will be used as command, otherwise it would be ignored
    }
  }

  if(AutoPlayAINavigateDungeon(bPlayerHasLantern))
    return true;

  /****************************************
   * Twighlight zone
   */

  ADD_MESSAGE("%s says \"I need more intelligence to do things by myself...\"", CHAR_NAME(DEFINITE)); DBG1("TODO: AI needs improvement");

  static int iDesperateResetCountDown=10;
  if(iDesperateResetCountDown==0){
    iDesperateResetCountDown=10;

    AutoPlayAIReset(true);

    // AFTER THE RESET!!!
    iWanderTurns=iMaxWanderTurns; DBG2("DesperateResetToSeeIfAIWorksAgain",iWanderTurns);
  }else{
    GetAICommand(); DBG2("WanderingDesperatelyNotKnowingWhatToDo",iDesperateResetCountDown); // :)
    iDesperateResetCountDown--;
  }

  return true;
}

void character::GetPlayerCommand()
{
  truth HasActed = false;

  while(!HasActed)
  {
    graphics::SetAllowStretchedBlit(); //overall great/single location to re-enable stretched blit!

    game::DrawEverything();

    if(!StateIsActivated(FEARLESS) && game::GetDangerFound())
    {
      if(game::GetDangerFound() > 500.)
      {
        if(game::GetCausePanicFlag())
        {
          game::SetCausePanicFlag(false);
          BeginTemporaryState(PANIC, 500 + RAND_N(500));
        }

        game::AskForKeyPress(CONST_S("You are horrified by your situation! [press any key to continue]"));
      }
      else if(ivanconfig::GetWarnAboutDanger())
      {
        if(game::GetDangerFound() > 50.)
          game::AskForKeyPress(CONST_S("You sense great danger! [press any key to continue]"));
        else
          game::AskForKeyPress(CONST_S("You sense danger! [press any key to continue]"));
      }

      game::SetDangerFound(0);
    }

    game::SetIsInGetCommand(true);
    int Key = GET_KEY();
    game::SetIsInGetCommand(false);

    if(Key != '+' && Key != '-' && Key != 'M') // gum (these are the messages keys M=ShowHistory +-=ScrollHistUpDown)
      msgsystem::ThyMessagesAreNowOld();

    truth ValidKeyPressed = false;
    int c;

#ifdef WIZARD
    if(IsPlayerAutoPlay()){
      if(Key=='.'){ // pressed or simulated
        if(game::IsInWilderness()){
          Key='>'; //blindly tries to go back to the dungeon safety :) TODO target and move to other dungeons/towns in the wilderness
        }else{
          HasActed = AutoPlayAICommand(Key); DBG2("Simulated",Key);
          if(HasActed)ValidKeyPressed = true; //valid simulated action
        }
      }else{
        /**
         * if the user hits any key during the autoplay mode that runs by itself, it will be disabled.
         * at non auto mode, can be moved around but cannot rest or will move by itself
         */
        if(game::GetAutoPlayMode()>=2 && Key!='~'){
          game::DisableAutoPlayMode();
          AutoPlayAIReset(true); // this will help on re-randomizing things, mainly paths
        }
      }
    }
#endif

    if(!HasActed){

      for(c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
        if(Key == game::GetMoveCommandKey(c))
        {
          bool bWaitNeutralMove=false;
          HasActed = TryMove(ApplyStateModification(game::GetMoveVector(c)), true, game::PlayerIsRunning(), &bWaitNeutralMove);
          if(!HasActed && bWaitNeutralMove){
            //cant access.. HasActed = commandsystem::NOP(this);
            Key = '.'; //TODO request NOP()'s key instead of this '.' hardcoded here. how?
          }
          ValidKeyPressed = true;
        }

      for(c = 1; commandsystem::GetCommand(c); ++c)
        if(Key == commandsystem::GetCommand(c)->GetKey())
        {
          if(game::IsInWilderness() && !commandsystem::GetCommand(c)->IsUsableInWilderness())
            ADD_MESSAGE("This function cannot be used while in wilderness.");
          else
            if(!game::WizardModeIsActive() && commandsystem::GetCommand(c)->IsWizardModeFunction())
              ADD_MESSAGE("Activate wizardmode to use this function.");
            else{
              game::RegionListItemEnable(commandsystem::IsForRegionListItem(c));
              game::RegionSilhouetteEnable(commandsystem::IsForRegionSilhouette(c));
              HasActed = commandsystem::GetCommand(c)->GetLinkedFunction()(this);
              game::RegionListItemEnable(false);
              game::RegionSilhouetteEnable(false);
            }

          ValidKeyPressed = true;
          break;
        }

    }

    if(!ValidKeyPressed)
      ADD_MESSAGE("Unknown key. Press '?' for a list of commands.");
  }

  game::IncreaseTurn();
}

void character::Vomit(v2 Pos, int Amount, truth ShowMsg)
{
  if(!CanVomit())
    return;

  if(ShowMsg)
  {
    if(IsPlayer())
      ADD_MESSAGE("You vomit.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s vomits.", CHAR_NAME(DEFINITE));
  }

  if(VomittingIsUnhealthy())
  {
    EditExperience(ARM_STRENGTH, -75, 1 << 9);
    EditExperience(LEG_STRENGTH, -75, 1 << 9);
  }

  if(IsPlayer())
  {
    EditNP(-2500 - RAND() % 2501);
    CheckStarvationDeath(CONST_S("vomited himself to death"));
  }

  if(StateIsActivated(PARASITE_TAPE_WORM) && !(RAND() & 7))
  {
    if(IsPlayer())
      ADD_MESSAGE("You notice a dead broad tapeworm among your former stomach contents.");

    DeActivateTemporaryState(PARASITE_TAPE_WORM);
  }

  if(!game::IsInWilderness())
    GetNearLSquare(Pos)->ReceiveVomit(this,
                                      liquid::Spawn(GetVomitMaterial(), long(sqrt(GetBodyVolume()) * Amount / 1000)));
}

truth character::Polymorph(character* NewForm, int Counter)
{
  if(!IsPolymorphable() || (!IsPlayer() && game::IsInWilderness()))
  {
    delete NewForm;
    return false;
  }

  RemoveTraps();

  if(GetAction())
    GetAction()->Terminate(false);

  NewForm->SetAssignedName("");

  if(IsPlayer())
    ADD_MESSAGE("Your body glows in a crimson light. You transform into %s!", NewForm->CHAR_NAME(INDEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s glows in a crimson light and %s transforms into %s!",
                CHAR_NAME(DEFINITE), GetPersonalPronoun().CStr(), NewForm->CHAR_NAME(INDEFINITE));

  Flags |= C_IN_NO_MSG_MODE;
  NewForm->Flags |= C_IN_NO_MSG_MODE;
  NewForm->ChangeTeam(GetTeam());
  NewForm->GenerationDanger = GenerationDanger;

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(NewForm);

  v2 Pos = GetPos();
  Remove();
  NewForm->PutToOrNear(Pos);
  NewForm->SetAssignedName(GetAssignedName());
  NewForm->ActivateTemporaryState(POLYMORPHED);
  NewForm->SetTemporaryStateCounter(POLYMORPHED, Counter);

  if(TemporaryStateIsActivated(POLYMORPHED))
  {
    NewForm->SetPolymorphBackup(GetPolymorphBackup());
    SetPolymorphBackup(0);
    SendToHell();
  }
  else
  {
    NewForm->SetPolymorphBackup(this);
    Flags |= C_POLYMORPHED;
    Disable();
  }

  GetStack()->MoveItemsTo(NewForm->GetStack());
  NewForm->SetMoney(GetMoney());
  DonateEquipmentTo(NewForm);
  Flags &= ~C_IN_NO_MSG_MODE;
  NewForm->Flags &= ~C_IN_NO_MSG_MODE;
  NewForm->CalculateAll();

  if(IsPlayer())
  {
    Flags &= ~C_PLAYER;
    game::SetPlayer(NewForm);
    game::SendLOSUpdateRequest();
    UpdateESPLOS();
  }

  NewForm->TestWalkability();
  return true;
}

void character::BeKicked(character* Kicker, item* Boot, bodypart* Leg, v2 HitPos, double KickDamage,
                         double ToHitValue, int Success, int Direction, truth Critical, truth ForceHit)
{
  switch(TakeHit(Kicker, Boot, Leg, HitPos, KickDamage, ToHitValue,
                 Success, KICK_ATTACK, Direction, Critical, ForceHit))
  {
   case HAS_HIT:
   case HAS_BLOCKED:
   case DID_NO_DAMAGE:
    if(IsEnabled() && !CheckBalance(KickDamage))
    {
      if(IsPlayer())
        ADD_MESSAGE("The kick throws you off balance.");
      else if(Kicker->IsPlayer())
        ADD_MESSAGE("The kick throws %s off balance.", CHAR_DESCRIPTION(DEFINITE));

      v2 FallToPos = GetPos() + game::GetMoveVector(Direction);
      FallTo(Kicker, FallToPos);
    }
  }
}

/* Return true if still in balance */

truth character::CheckBalance(double KickDamage)
{
  return !CanMove()
    || IsStuck()
    || !KickDamage
    || (!IsFlying()
        && KickDamage * 5 < RAND() % GetSize());
}

void character::FallTo(character* GuiltyGuy, v2 Where)
{
  EditAP(-500);
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  int Squares = CalculateNewSquaresUnder(MoveToSquare, Where);

  if(Squares)
  {
    truth NoRoom = false;

    for(int c = 0; c < Squares; ++c)
    {
      olterrain* Terrain = MoveToSquare[c]->GetOLTerrain();

      if(Terrain && !CanMoveOn(Terrain))
      {
        NoRoom = true;
        break;
      }
    }

    if(NoRoom)
    {
      if(HasHead())
      {
        if(IsPlayer())
          ADD_MESSAGE("You hit your head on the wall.");
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s hits %s head on the wall.", CHAR_NAME(DEFINITE), GetPossessivePronoun().CStr());
      }

      ReceiveDamage(GuiltyGuy, 1 + RAND() % 5, PHYSICAL_DAMAGE, HEAD);
      CheckDeath(CONST_S("killed by hitting a wall due to being kicked @bk"), GuiltyGuy);
    }
    else
    {
      if(IsFreeForMe(MoveToSquare[0]))
        Move(Where, true);

      // Place code that handles characters bouncing to each other here
    }
  }
}

truth character::CheckCannibalism(cmaterial* What) const
{
  return GetTorso()->GetMainMaterial()->IsSameAs(What);
}

void character::StandIdleAI()
{
  SeekLeader(GetLeader());

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForDoors())
    return;

  if(MoveTowardsHomePos())
    return;

  if(CheckSadism())
    return;

  EditAP(-1000);
}

truth character::LoseConsciousness(int Counter, truth HungerFaint)
{
  if(!AllowUnconsciousness())
    return false;

  action* Action = GetAction();

  if(Action)
  {
    if(HungerFaint && !Action->AllowUnconsciousness())
      return false;

    if(Action->IsUnconsciousness())
    {
      static_cast<unconsciousness*>(Action)->RaiseCounterTo(Counter);
      return true;
    }
    else
      Action->Terminate(false);
  }

  if(IsPlayer())
    ADD_MESSAGE("You lose consciousness.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s loses consciousness.", CHAR_NAME(DEFINITE));

  unconsciousness* Unconsciousness = unconsciousness::Spawn(this);
  Unconsciousness->SetCounter(Counter);
  SetAction(Unconsciousness);
  return true;
}

void character::DeActivateTemporaryState(long What)
{
  if(PolymorphBackup)
    PolymorphBackup->TemporaryState &= ~What;

  TemporaryState &= ~What;
}

void character::DeActivateVoluntaryAction(cfestring& Reason)
{
  if(GetAction() && GetAction()->IsVoluntary())
  {
    if(IsPlayer())
    {
      if(Reason.GetSize())
        ADD_MESSAGE("%s", Reason.CStr());

      if(game::TruthQuestion(CONST_S("Continue ") + GetAction()->GetDescription() + "? [y/N]"))
        GetAction()->ActivateInDNDMode();
      else
        GetAction()->Terminate(false);
    }
    else
      GetAction()->Terminate(false);
  }
}

void character::ActionAutoTermination()
{
  if(!GetAction() || !GetAction()->IsVoluntary() || GetAction()->InDNDMode())
    return;

  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled()
           && p->CanBeSeenBy(this, false, true)
           && (p->CanMove() || p->GetPos().IsAdjacent(Pos))
           && p->CanAttack())
        {
          if(IsPlayer())
          {
            ADD_MESSAGE("%s seems to be hostile.", p->CHAR_NAME(DEFINITE));

            if(game::TruthQuestion(CONST_S("Continue ") + GetAction()->GetDescription() + "? [y/N]"))
              GetAction()->ActivateInDNDMode();
            else
              GetAction()->Terminate(false);
          }
          else
            GetAction()->Terminate(false);

          return;
        }
}

truth character::CheckForEnemies(truth CheckDoors, truth CheckGround, truth MayMoveRandomly, truth RunTowardsTarget)
{
  if(!IsEnabled())
    return false;

  truth HostileCharsNear = false;
  character* NearestChar = 0;
  long NearestDistance = 0x7FFFFFFF;
  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled() && GetAttribute(WISDOM) < p->GetAttackWisdomLimit())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));

          if(ThisDistance <= GetLOSRangeSquare())
            HostileCharsNear = true;

          if((ThisDistance < NearestDistance
              || (ThisDistance == NearestDistance && !(RAND() % 3)))
             && p->CanBeSeenBy(this, false, IsGoingSomeWhere())
             && (!IsGoingSomeWhere() || HasClearRouteTo(p->GetPos())))
          {
            NearestChar = p;
            NearestDistance = ThisDistance;
          }
        }

  if(NearestChar)
  {
    if(GetAttribute(INTELLIGENCE) >= 10 || IsSpy())
      game::CallForAttention(GetPos(), 100);

    if(SpecialEnemySightedReaction(NearestChar))
      return true;

    if(IsExtraCoward() && !StateIsActivated(PANIC) && NearestChar->GetRelativeDanger(this) >= 0.5 && !StateIsActivated(FEARLESS))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s sees %s.", CHAR_NAME(DEFINITE), NearestChar->CHAR_DESCRIPTION(DEFINITE));

      BeginTemporaryState(PANIC, 500 + RAND() % 500);
    }

    if(!IsRetreating())
    {
      if(CheckGround && NearestDistance > 2 && CheckForUsefulItemsOnGround(false))
        return true;

      SetGoingTo(NearestChar->GetPos());
    }
    else
      SetGoingTo(Pos - ((NearestChar->GetPos() - Pos) << 4));

    return MoveTowardsTarget(true);
  }
  else
  {
    character* Leader = GetLeader();

    if(Leader == this)
      Leader = 0;

    if(!Leader && IsGoingSomeWhere())
    {
      if(!MoveTowardsTarget(RunTowardsTarget))
      {
        TerminateGoingTo();
        return false;
      }
      else
      {
        if(!IsEnabled())
          return true;

        if(GetPos() == GoingTo)
          TerminateGoingTo();

        return true;
      }
    }
    else
    {
      if((!Leader || (Leader && !IsGoingSomeWhere())) && HostileCharsNear)
      {
        if(CheckDoors && CheckForDoors())
          return true;

        if(CheckGround && CheckForUsefulItemsOnGround())
          return true;

        if(MayMoveRandomly && MoveRandomly()) // one has heard that an enemy is near but doesn't know where
          return true;
      }

      return false;
    }
  }
}

truth character::CheckForDoors()
{
  if(!CanOpen() || !IsEnabled())
    return false;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    lsquare* Square = GetNeighbourLSquare(d);

    if(Square && Square->GetOLTerrain() && Square->GetOLTerrain()->Open(this))
      return true;
  }

  return false;
}

truth character::CheckForUsefulItemsOnGround(truth CheckFood)
{
  if(StateIsActivated(PANIC) || !IsEnabled())
    return false;

  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->CanBeSeenBy(this) && ItemVector[c]->IsPickable(this))
    {
      if(!(CommandFlags & DONT_CHANGE_EQUIPMENT)
         && TryToEquip(ItemVector[c]))
        return true;

      if(CheckFood && UsesNutrition() && !CheckIfSatiated()
         && TryToConsume(ItemVector[c]))
        return true;
    }

  return false;
}

truth character::FollowLeader(character* Leader)
{
  if(!Leader || Leader == this || !IsEnabled())
    return false;

  if(CommandFlags & FOLLOW_LEADER && Leader->CanBeSeenBy(this) && Leader->SquareUnderCanBeSeenBy(this, true)){
    v2HoldPos = GoingTo; //will keep the last reference position possible

    v2 Distance = GetPos() - GoingTo; //set by SeekLeader()
    if(abs(Distance.X) <= 2 && abs(Distance.Y) <= 2)
      return false;
    else
      return MoveTowardsTarget(false);
  }

  if(IsGoingSomeWhere()){
    if(!MoveTowardsTarget(true)){
      TerminateGoingTo();
      return false;
    }else{
      return true;
    }
  }else{
    // just hold near position
    if(v2HoldPos.Is0())
      v2HoldPos=GetPos(); //when the game is loaded keep current pos TODO could be savegamed tho
    if(ivanconfig::GetHoldPosMaxDist()>0){
      v2 v2HoldDist = GetPos() - v2HoldPos;
      if(abs(v2HoldDist.X) < ivanconfig::GetHoldPosMaxDist() && abs(v2HoldDist.Y) < ivanconfig::GetHoldPosMaxDist()){
        // will do other things
        return false;
      }else{
        SetGoingTo(v2HoldPos); DBG4(GetNameSingular().CStr(),DBGAV2(v2HoldPos),DBGAV2(Leader->GetPos()),DBGAV2(v2HoldDist));
        return true;
      }
    }
  }

  return false;
}

void character::SeekLeader(ccharacter* Leader)
{
  if(Leader && Leader != this)
  {
    if(Leader->CanBeSeenBy(this) && (Leader->SquareUnderCanBeSeenBy(this, true) || !IsGoingSomeWhere()))
    {
      if(CommandFlags & FOLLOW_LEADER)
        SetGoingTo(Leader->GetPos());
    }
    else if(!IsGoingSomeWhere())
    {
      team* Team = GetTeam();

      for(character* p : Team->GetMember()){
        if(p->IsEnabled()
           && p->GetID() != GetID()
           && (CommandFlags & FOLLOW_LEADER)
           == (p->CommandFlags & FOLLOW_LEADER)
           && p->CanBeSeenBy(this))
        {
          v2 Pos = p->GetPos();
          v2 Distance = GetPos() - Pos;

          if(abs(Distance.X) > 2 && abs(Distance.Y) > 2)
          {
            SetGoingTo(Pos);
            break;
          }
        }
      }

//      if(!IsGoingSomeWhere()){ // couldnt follow leader neather team mate
//      }
    }
  }
}

int character::GetMoveEase() const
{
  switch(BurdenState)
  {
   case OVER_LOADED:
   case STRESSED: return 50;
   case BURDENED: return 75;
   case UNBURDENED: return 100;
  }

  return 666;
}

int character::GetLOSRange() const
{
  if(!game::IsInWilderness())
    return GetAttribute(PERCEPTION) * GetLevel()->GetLOSModifier() / 48;
  else
    return 3;
}

truth character::Displace(character* Who, truth Forced)
{
  if(GetBurdenState() == OVER_LOADED)
  {
    if(IsPlayer())
    {
      cchar* CrawlVerb = StateIsActivated(LEVITATION) ? "float" : "crawl";
      ADD_MESSAGE("You try very hard to %s forward. But your load is too heavy.", CrawlVerb);
      EditAP(-1000);
      return true;
    }
    else
      return false;
  }

  double Danger = GetRelativeDanger(Who);
  int PriorityDifference = Limit(GetDisplacePriority() - Who->GetDisplacePriority(), -31, 31);

  if(IsPlayer())
    ++PriorityDifference;
  else if(Who->IsPlayer())
    --PriorityDifference;

  if(PriorityDifference >= 0)
    Danger *= 1 << PriorityDifference;
  else
    Danger /= 1 << -PriorityDifference;

  if(IsSmall() && Who->IsSmall()
     && (Forced || Danger > 1. || !(Who->IsPlayer() || Who->IsBadPath(GetPos())))
     && !IsStuck() && !Who->IsStuck()
     && (!Who->GetAction() || Who->GetAction()->TryDisplace())
     && CanMove() && Who->CanMove() && Who->CanMoveOn(GetLSquareUnder()))
  {
    if(IsPlayer())
      ADD_MESSAGE("You displace %s!", Who->CHAR_DESCRIPTION(DEFINITE));
    else if(Who->IsPlayer())
      ADD_MESSAGE("%s displaces you!", CHAR_DESCRIPTION(DEFINITE));
    else if(CanBeSeenByPlayer() || Who->CanBeSeenByPlayer())
      ADD_MESSAGE("%s displaces %s!", CHAR_DESCRIPTION(DEFINITE), Who->CHAR_DESCRIPTION(DEFINITE));

    lsquare* OldSquareUnder1[MAX_SQUARES_UNDER];
    lsquare* OldSquareUnder2[MAX_SQUARES_UNDER];
    int c;

    for(c = 0; c < GetSquaresUnder(); ++c)
      OldSquareUnder1[c] = GetLSquareUnder(c);

    for(c = 0; c < Who->GetSquaresUnder(); ++c)
      OldSquareUnder2[c] = Who->GetLSquareUnder(c);

    v2 Pos = GetPos();
    v2 WhoPos = Who->GetPos();
    Remove();
    Who->Remove();
    PutTo(WhoPos);
    Who->PutTo(Pos);
    EditAP(-GetMoveAPRequirement(GetSquareUnder()->GetEntryDifficulty()) - 500);
    EditNP(-12 * GetSquareUnder()->GetEntryDifficulty());
    EditExperience(AGILITY, 75, GetSquareUnder()->GetEntryDifficulty() << 7);

    if(IsPlayer())
      ShowNewPosInfo();

    if(Who->IsPlayer())
      Who->ShowNewPosInfo();

    SignalStepFrom(OldSquareUnder1);
    Who->SignalStepFrom(OldSquareUnder2);
    return true;
  }
  else
  {
    if(IsPlayer())
    {
      ADD_MESSAGE("%s resists!", Who->CHAR_DESCRIPTION(DEFINITE));
      EditAP(-1000);
      return true;
    }
    else
      return false;
  }
}

void character::SetNP(long What)
{
  int OldState = GetHungerState();
  NP = What;

  if(IsPlayer())
  {
    int NewState = GetHungerState();

    if(NewState == STARVING && OldState > STARVING)
      DeActivateVoluntaryAction(CONST_S("You are getting really hungry."));
    else if(NewState == VERY_HUNGRY && OldState > VERY_HUNGRY)
      DeActivateVoluntaryAction(CONST_S("You are getting very hungry."));
    else if(NewState == HUNGRY && OldState > HUNGRY)
      DeActivateVoluntaryAction(CONST_S("You are getting hungry."));
  }
}

void character::ShowNewPosInfo() const
{
  msgsystem::EnterBigMessageMode();
  v2 Pos = GetPos();

  if(Pos.X < game::GetCamera().X + 3 || Pos.X >= game::GetCamera().X + game::GetScreenXSize() - 3)
    game::UpdateCameraX();

  if(Pos.Y < game::GetCamera().Y + 3 || Pos.Y >= game::GetCamera().Y + game::GetScreenYSize() - 3)
    game::UpdateCameraY();

  game::SendLOSUpdateRequest();
  game::DrawEverythingNoBlit();
  UpdateESPLOS();

  if(!game::IsInWilderness())
  {
    if(GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
      ADD_MESSAGE("It's dark in here!");

    GetLSquareUnder()->ShowSmokeMessage();
    itemvectorvector PileVector;
    GetStackUnder()->Pile(PileVector, this, CENTER);

    if(PileVector.size())
    {
      truth Feel = !GetLSquareUnder()->IsTransparent() || GetLSquareUnder()->IsDark();

      if(PileVector.size() == 1)
      {
        if(Feel)
          ADD_MESSAGE("You feel %s lying here.",
                      PileVector[0][0]->GetName(INDEFINITE, PileVector[0].size()).CStr());
        else
          ADD_MESSAGE("%s %s lying here.",
                      PileVector[0][0]->GetName(INDEFINITE, PileVector[0].size()).CStr(),
                      PileVector[0].size() == 1 ? "is" : "are");
      }
      else
      {
        int Items = 0;

        for(uint c = 0; c < PileVector.size(); ++c)
          if((Items += PileVector[c].size()) > 3)
            break;

        if(Items > 3)
        {
          if(Feel)
            ADD_MESSAGE("You feel several items lying here.");
          else
            ADD_MESSAGE("Several items are lying here.");
        }
        else if(Items)
        {
          if(Feel)
            ADD_MESSAGE("You feel a few items lying here.");
          else
            ADD_MESSAGE("A few items are lying here.");
        }
      }
    }

    festring SideItems;
    GetLSquareUnder()->GetSideItemDescription(SideItems);

    if(!SideItems.IsEmpty())
      ADD_MESSAGE("There is %s.", SideItems.CStr());

    if(GetLSquareUnder()->HasEngravings())
    {
      if(CanRead())
        ADD_MESSAGE("Something has been engraved here: \"%s\"", GetLSquareUnder()->GetEngraved());
      else
        ADD_MESSAGE("Something has been engraved here.");
    }
  }

  msgsystem::LeaveBigMessageMode();
}

void character::Hostility(character* Enemy)
{
  if(Enemy == this || !Enemy || !Team || !Enemy->Team)
    return;

  if(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND)
    return;

  if(!IsAlly(Enemy))
    GetTeam()->Hostility(Enemy->GetTeam());
  else if(IsPlayer() && !Enemy->IsPlayer()) // I believe both may be players due to polymorph feature...
  {
    if(Enemy->CanBeSeenByPlayer())
      ADD_MESSAGE("%s becomes enraged.", Enemy->CHAR_NAME(DEFINITE));

    Enemy->ChangeTeam(game::GetTeam(BETRAYED_TEAM));
  }
}

stack* character::GetGiftStack() const
{
  if(GetLSquareUnder()->GetRoomIndex() && !GetLSquareUnder()->GetRoom()->AllowDropGifts())
    return GetStack();
  else
    return GetStackUnder();
}

truth character::MoveRandomlyInRoom()
{
  for(int c = 0; c < 10; ++c)
  {
    v2 ToTry = game::GetMoveVector(RAND() & 7);

    if(GetLevel()->IsValidPos(GetPos() + ToTry))
    {
      lsquare* Square = GetNearLSquare(GetPos() + ToTry);

      if(!Square->IsDangerous(this)
         && !Square->IsScary(this)
         && (!Square->GetOLTerrain()
             || !Square->GetOLTerrain()->IsDoor())
         && TryMove(ToTry, false, false))
        return true;
    }
  }

  return false;
}

void character::GoOn(go* Go, truth FirstStep)
{
  v2 MoveVector = ApplyStateModification(game::GetMoveVector(Go->GetDirection()));
  lsquare* MoveToSquare[MAX_SQUARES_UNDER];
  int Squares = CalculateNewSquaresUnder(MoveToSquare, GetPos() + MoveVector);

  if(!Squares || !CanMoveOn(MoveToSquare[0]))
  {
    Go->Terminate(false);
    return;
  }

  uint OldRoomIndex = GetLSquareUnder()->GetRoomIndex();
  uint CurrentRoomIndex = MoveToSquare[0]->GetRoomIndex();

  if((OldRoomIndex && (CurrentRoomIndex != OldRoomIndex)) && !FirstStep)
  {
    Go->Terminate(false);
    return;
  }

  for(int c = 0; c < Squares; ++c)
    if((MoveToSquare[c]->GetCharacter() && GetTeam() != MoveToSquare[c]->GetCharacter()->GetTeam())
       || MoveToSquare[c]->IsDangerous(this))
    {
      Go->Terminate(false);
      return;
    }

  int OKDirectionsCounter = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    lsquare* Square = GetNeighbourLSquare(d);

    if(Square && CanMoveOn(Square))
      ++OKDirectionsCounter;
  }

  if(!Go->IsWalkingInOpen())
  {
    if(OKDirectionsCounter > 2)
    {
      Go->Terminate(false);
      return;
    }
  }
  else
    if(OKDirectionsCounter <= 2)
      Go->SetIsWalkingInOpen(false);

  square* BeginSquare = GetSquareUnder();

  if(!TryMove(MoveVector, true, game::PlayerIsRunning())
     || BeginSquare == GetSquareUnder()
     || (CurrentRoomIndex && (OldRoomIndex != CurrentRoomIndex)))
  {
    Go->Terminate(false);
    return;
  }

  if(GetStackUnder()->GetVisibleItems(this))
  {
    bool bUseless=false,bTooCheap=false,bEncumbering=false;

    switch(ivanconfig::GetGoOnStopMode()){
    case 0: Go->Terminate(false); return;
    case 1:bUseless=true;break;
    case 2:bTooCheap=true;break;
    case 3:bEncumbering=true;break;
    default:
      ABORT("unsupported go on stop mode %d",ivanconfig::GetGoOnStopMode());
      break;
    }

    itemvector vit;
    GetStackUnder()->FillItemVector(vit);
    for(int i=0;i<vit.size();i++){
      DBG9(vit[i]->GetNameSingular().CStr(), vit[i]->AllowEquip(), vit[i]->IsAppliable(this), vit[i]->IsConsumable(),
        vit[i]->IsEatable(this), vit[i]->IsDrinkable(this), vit[i]->IsOpenable(this), vit[i]->IsReadable(this), vit[i]->IsZappable(this) );
      DBG8(vit[i]->GetNameSingular().CStr(),vit[i]->IsWeapon(this),vit[i]->IsArmor(this),vit[i]->IsBodyArmor(this),vit[i]->IsHelmet(this),
        vit[i]->IsGauntlet(this),vit[i]->IsBoot(this),vit[i]->IsBelt(this) );
      if( //TODO ? vit[i]->GetSpoilLevel()==0
          vit[i]->IsOpenable(this) //doors are not items, but works for chests too
          ||
          (bUseless &&
            (
              vit[i]->IsAppliable(this) ||
              vit[i]->IsZappable(this)  ||

              // bad!              vit[i]->IsConsumable() ||
              vit[i]->IsEatable(this) ||
              vit[i]->IsDrinkable(this) ||

              // bad!              vit[i]->AllowEquip() ||
              vit[i]->IsWeapon(this) ||
              vit[i]->IsArmor(this) || //all armor slots
//              vit[i]->IsBodyArmor(this) ||
//              vit[i]->IsHelmet(this) ||
//              vit[i]->IsGauntlet(this) ||
//              vit[i]->IsBoot(this) ||
//              vit[i]->IsBelt(this) ||

              vit[i]->IsAmulet(this) ||
              vit[i]->IsRing(this) ||

              vit[i]->IsReadable(this)
            )
          ) ||
          (bTooCheap &&
            (vit[i]->GetTruePrice() > iMaxValueless)
          ) ||
          (bEncumbering && //calc in float price vs weight
            (vit[i]->GetTruePrice()/(vit[i]->GetWeight()/1000.0)) > (iMaxValueless*2)
          )
      ){
        Go->Terminate(false);
        return;
      }
    }
  }

  game::DrawEverything();
}

void character::SetTeam(team* What)
{
  Team = What;
  SetTeamIterator(What->Add(this));
}

void character::ChangeTeam(team* What)
{
  if(Team)
    Team->Remove(GetTeamIterator());

  Team = What;
  SendNewDrawRequest();

  if(Team)
    SetTeamIterator(Team->Add(this));
}

truth character::ChangeRandomAttribute(int HowMuch)
{
  for(int c = 0; c < 50; ++c)
  {
    int AttribID = RAND() % ATTRIBUTES;

    if(EditAttribute(AttribID, HowMuch))
      return true;
  }

  return false;
}

int character::RandomizeReply(long& Said, int Replies)
{
  truth NotSaid = false;

  for(int c = 0; c < Replies; ++c)
    if(!(Said & (1 << c)))
    {
      NotSaid = true;
      break;
    }

  if(!NotSaid)
    Said = 0;

  long ToSay;
  while(Said & 1 << (ToSay = RAND() % Replies));
  Said |= 1 << ToSay;
  return ToSay;
}

void character::DisplayInfo(festring& Msg)
{
  if(IsPlayer())
  {
    Msg << " You are " << GetStandVerb() << " here.";
    if(IsBurning())
      Msg << " You are on fire.";
  }
  else
  {
    Msg << ' ' << GetName(INDEFINITE).CapitalizeCopy() << " is "
        << GetStandVerb() << " here. " << GetPersonalPronoun().CapitalizeCopy();
    cchar* Separator1 = GetAction() ? "," : " and";
    cchar* Separator2 = " and";

    if(GetTeam() == PLAYER->GetTeam())
      Msg << " is tame";
    else
    {
      int Relation = GetRelation(PLAYER);

      if(Relation == HOSTILE)
        Msg << " is hostile";
      else if(Relation == UNCARING)
      {
        Msg << " does not care about you";
        Separator1 = Separator2 = " and is";
      }
      else
        Msg << " is friendly";
    }

    if(StateIsActivated(PANIC))
    {
      Msg << Separator1 << " panicked";
      Separator2 = " and";
    }

    if(IsBurning())
    {
      Msg << Separator1 << " is on fire";
      Separator2 = " and";
    }

    if(GetAction())
      Msg << Separator2 << ' ' << GetAction()->GetDescription();

    Msg << '.';
  }
}

void character::TestWalkability()
{
  if(!IsEnabled())
    return;

  square* SquareUnder = !game::IsInWilderness()
                        ? GetSquareUnder() : PLAYER->GetSquareUnder();

  if(SquareUnder->IsFatalToStay() && !CanMoveOn(SquareUnder))
  {
    truth Alive = false;

    if(!game::IsInWilderness() || IsPlayer())
      for(int d = 0; d < GetNeighbourSquares(); ++d)
      {
        square* Square = GetNeighbourSquare(d);

        if(Square && CanMoveOn(Square) && IsFreeForMe(Square))
        {
          if(IsPlayer())
            ADD_MESSAGE("%s.", SquareUnder->SurviveMessage(this));
          else if(CanBeSeenByPlayer())
            ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), SquareUnder->MonsterSurviveMessage(this));

          Move(Square->GetPos(), true); // actually, this shouldn't be a teleport move
          SquareUnder->SurviveEffect(this);
          Alive = true;
          break;
        }
      }

    if(!Alive)
    {
      if(IsPlayer())
      {
        Remove();
        SendToHell();
        festring DeathMsg = festring(SquareUnder->DeathMessage(this));
        game::AskForKeyPress(DeathMsg + ". [press any key to continue]");
        festring Msg = SquareUnder->ScoreEntry(this);
        PLAYER->AddScoreEntry(Msg);
        game::End(Msg);
      }
      else
      {
        if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s %s.", CHAR_NAME(DEFINITE), SquareUnder->MonsterDeathVerb(this));

        Die(0, SquareUnder->ScoreEntry(this), DISALLOW_MSG);
      }
    }
  }
}

int character::GetSize() const
{
  return GetTorso()->GetSize();
}

void character::SetMainMaterial(material* NewMaterial, int SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->SetMainMaterial(NewMaterial, SpecialFlags);

  for(int c = 1; c < BodyParts; ++c)
  {
    NewMaterial = NewMaterial->SpawnMore(GetBodyPart(c)->GetMainMaterial()->GetVolume());
    GetBodyPart(c)->SetMainMaterial(NewMaterial, SpecialFlags);
  }
}

void character::ChangeMainMaterial(material* NewMaterial, int SpecialFlags)
{
  NewMaterial->SetVolume(GetBodyPart(0)->GetMainMaterial()->GetVolume());
  GetBodyPart(0)->ChangeMainMaterial(NewMaterial, SpecialFlags);

  for(int c = 1; c < BodyParts; ++c)
  {
    NewMaterial = NewMaterial->SpawnMore(GetBodyPart(c)->GetMainMaterial()->GetVolume());
    GetBodyPart(c)->ChangeMainMaterial(NewMaterial, SpecialFlags);
  }
}

void character::SetSecondaryMaterial(material*, int)
{
  ABORT("Illegal character::SetSecondaryMaterial call!");
}

void character::ChangeSecondaryMaterial(material*, int)
{
  ABORT("Illegal character::ChangeSecondaryMaterial call!");
}

void character::TeleportRandomly(truth Intentional)
{
  v2 TelePos = ERROR_V2;

  if(StateIsActivated(TELEPORT_LOCK))
  {
    ADD_MESSAGE("You flicker for a second.");
    return;
  }

  if(StateIsActivated(TELEPORT_CONTROL))
  {
    if(IsPlayer())
    {
      v2 Input = game::PositionQuestion(CONST_S("Where do you wish to teleport? "
                                                "[direction keys move cursor, space accepts]"),
                                        GetPos(), &game::TeleportHandler, 0, false);

      if(Input == ERROR_V2) // esc pressed
        Input = GetPos();

      lsquare* Square = GetNearLSquare(Input);

      if(CanMoveOn(Square) || game::GoThroughWallsCheatIsActive())
      {
        if(Square->GetPos() == GetPos())
        {
          ADD_MESSAGE("You disappear and reappear.");
          return;
        }

        if(IsFreeForMe(Square))
        {
          if((Input - GetPos()).GetLengthSquare() <= GetTeleportRangeSquare())
          {
            EditExperience(INTELLIGENCE, 100, 1 << 10);
            TelePos = Input;
          }
          else
            ADD_MESSAGE("You cannot concentrate yourself enough to control a teleport that far.");
        }
        else
        {
          character* C = Square->GetCharacter();

          if(C)
            ADD_MESSAGE("For a moment you feel very much like %s.", C->CHAR_NAME(INDEFINITE));
          else
            ADD_MESSAGE("You feel that something weird has happened, but can't really tell what exactly.");
        }
      }
      else
        ADD_MESSAGE("You feel like having been hit by something really hard from the inside.");
    }
    else if(!Intentional)
    {
      if(IsGoingSomeWhere() && GetLevel()->IsValidPos(GoingTo))
      {
        v2 Where = GetLevel()->GetNearestFreeSquare(this, GoingTo);

        if(Where != ERROR_V2 && (Where - GetPos()).GetLengthSquare() <= GetTeleportRangeSquare())
        {
          EditExperience(INTELLIGENCE, 100, 1 << 10);
          Where = TelePos;
        }
      }
    }
  }

  if(IsPlayer())
    ADD_MESSAGE("A rainbow-colored whirlpool twists the existence around you. "
                "You are sucked through a tunnel piercing a myriad of surreal "
                "universes. Luckily you return to this dimension in one piece.");

  if(TelePos != ERROR_V2)
    Move(TelePos, true);
  else
    Move(GetLevel()->GetRandomSquare(this), true);

  if(!IsPlayer() && CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears.", CHAR_NAME(INDEFINITE));

  if(GetAction() && GetAction()->IsVoluntary())
    GetAction()->Terminate(false);

  if(IsPlayerAutoPlay())
    AutoPlayAIReset(true);
}

truth character::IsPlayerAutoPlay()
{
  return IsPlayer() && game::GetAutoPlayMode()>0;
}

void character::DoDetecting()
{
  if(IsPlayerAutoPlay())return;

  material* TempMaterial;

  for(;;)
  {
    festring Temp;

    if(game::DefaultQuestion(Temp, CONST_S("What material do you want to detect?"),
                             game::GetDefaultDetectMaterial(), true) == ABORTED)
    {
      if(game::TruthQuestion(CONST_S("Really cancel? [y/N]")))
        return;
      else
        continue;
    }

    TempMaterial = protosystem::CreateMaterial(Temp);

    if(TempMaterial)
      break;
    else
      game::DrawEverythingNoBlit();
  }

  level* Level = GetLevel();
  int Squares = Level->DetectMaterial(TempMaterial);

  if(Squares > GetAttribute(INTELLIGENCE) * (25 + RAND() % 51))
  {
    ADD_MESSAGE("An enormous burst of geographical information overwhelms your consciousness. Your mind cannot cope with it and your memories blur.");
    Level->BlurMemory();
    BeginTemporaryState(CONFUSED, 1000 + RAND() % 1000);
    EditExperience(INTELLIGENCE, -100, 1 << 12);
  }
  else if(!Squares)
  {
    ADD_MESSAGE("You feel a sudden urge to imagine the dark void of a starless night sky.");
    EditExperience(INTELLIGENCE, 20, 1 << 12);
  }
  else
  {
    ADD_MESSAGE("You feel attracted to all things made of %s.", TempMaterial->GetName(false, false).CStr());
    game::SetDrawMapOverlay(ivanconfig::IsShowMapAtDetectMaterial());
    game::PositionQuestion(CONST_S("Detecting material [direction keys move cursor, space exits]"), GetPos(), 0, 0, false);
    game::SetDrawMapOverlay(false);
    EditExperience(INTELLIGENCE, 30, 1 << 12);
  }

  delete TempMaterial;
  Level->CalculateLuminances();
  game::SendLOSUpdateRequest();
}

void character::RestoreHP()
{
  doforbodyparts()(this, &bodypart::FastRestoreHP);
  HP = MaxHP;
}

void character::RestoreLivingHP()
{
  HP = 0;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->CanRegenerate())
    {
      BodyPart->FastRestoreHP();
      HP += BodyPart->GetHP();
    }
  }
}

truth character::IsBurnt() const
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->IsBurnt())
    {
      return true;
    }
  }
  return false;
}

truth character::AllowDamageTypeBloodSpill(int Type)
{
  switch(Type&0xFFF)
  {
   case PHYSICAL_DAMAGE:
   case SOUND:
   case ENERGY:
    return true;
   case ACID:
   case FIRE:
   case DRAIN:
   case POISON:
   case ELECTRICITY:
   case MUSTARD_GAS_DAMAGE:
   case PSI:
    return false;
  }

  ABORT("Unknown blood effect destroyed the dungeon!");
  return false;
}

v2 character::GetPosSafely() const
{
  square* sqr = GetSquareUnderSafely();
  if(sqr!=NULL)return sqr->GetPos();
  return v2();
}

square* character::GetSquareUnderSafely() const
{ //prevents crash if polymorphed (here at least)
  if(SquareUnder[0]!=NULL){DBGLN;
    return SquareUnder[0];
  }DBGLN;

  if(IsPolymorphed()){DBGLN;
    character* pb = GetPolymorphBackup();
    if(pb!=NULL && pb->SquareUnder[0]!=NULL){ DBG1(pb->GetNameSingular().CStr()); //TODO to use square under index here may cause inconsistencies?
      return pb->SquareUnder[0];
    }
  }DBGLN;

  return NULL;
}

stack* character::GetStackUnderSafely() const
{
  square* sqr = GetSquareUnderSafely();
  lsquare* lsqr = dynamic_cast<lsquare*>(sqr);
  if(lsqr!=NULL)return lsqr->GetStack();
  return NULL;
}


/* Returns truly done damage */

int character::ReceiveBodyPartDamage(character* Damager, int Damage, int Type, int BodyPartIndex,
                                     int Direction, truth PenetrateResistance, truth Critical,
                                     truth ShowNoDamageMsg, truth CaptureBodyPart)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(!Damager || Damager->AttackMayDamageArmor())
    BodyPart->DamageArmor(Damager, Damage, Type);

  if(!PenetrateResistance)
    Damage -= (BodyPart->GetTotalResistance(Type) >> 1) + RAND() % ((BodyPart->GetTotalResistance(Type) >> 1) + 1);

  if(Damage < 1)
  {
    if(Critical)
      Damage = 1;
    else
    {
      if(ShowNoDamageMsg)
      {
        if(IsPlayer())
          ADD_MESSAGE("You are not hurt.");
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().CStr());
      }

      return 0;
    }
  }

  if(BodyPart->GetMainMaterial())
  {
    if(BodyPart->CanBeBurned()
       && (BodyPart->GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
       && !BodyPart->IsBurning()
       && Type & FIRE)
    {
      BodyPart->TestActivationEnergy(Damage);
    }
    else if(BodyPart->IsBurning() && Type & FIRE)
      BodyPart->GetMainMaterial()->AddToThermalEnergy(Damage);
  }

  if(Critical && AllowDamageTypeBloodSpill(Type) && !game::IsInWilderness())
  {
    BodyPart->SpillBlood(2 + (RAND() & 1));

    for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->IsFlyable())
        BodyPart->SpillBlood(1, Square->GetPos());
    }
  }

  if(BodyPart->ReceiveDamage(Damager, Damage, Type, Direction)
     && BodyPartCanBeSevered(BodyPartIndex))
  {
    if(DamageTypeDestroysBodyPart(Type))
    {
      if(IsPlayer())
        ADD_MESSAGE("Your %s is destroyed!", BodyPart->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s %s is destroyed!", GetPossessivePronoun().CStr(), BodyPart->GetBodyPartName().CStr());

      GetBodyPart(BodyPartIndex)->DropEquipment();
      item* Severed = SevereBodyPart(BodyPartIndex);

      if(Severed)
        Severed->DestroyBodyPart(!game::IsInWilderness() ? GetStackUnder() : GetStack());

      SendNewDrawRequest();

      if(IsPlayer())
        game::AskForKeyPress(CONST_S("Bodypart destroyed! [press any key to continue]"));
    }
    else
    {
      if(IsPlayer())
        ADD_MESSAGE("Your %s is severed off!", BodyPart->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s %s is severed off!", GetPossessivePronoun().CStr(), BodyPart->GetBodyPartName().CStr());

      item* Severed = SevereBodyPart(BodyPartIndex);
      SendNewDrawRequest();

      if(Severed)
      {
        if(CaptureBodyPart)
          Damager->GetLSquareUnder()->AddItem(Severed);
        else if(!game::IsInWilderness())
        {
          /** No multi-tile humanoid support! */

          stack* su = GetStackUnderSafely();
          if(su){
            su->AddItem(Severed);

            if(Direction != YOURSELF)
              Severed->Fly(0, Direction, Damage);
          }else{
            /**
             * this may happen when polymorphing (tests were made as a snake) during an explosion that severe body parts
             */
            GetStack()->AddItem(Severed); DBGLN;
          }
        }
        else
          GetStack()->AddItem(Severed);

        Severed->DropEquipment();
      }
      else if(IsPlayer() || CanBeSeenByPlayer())
        ADD_MESSAGE("It vanishes.");

      if(IsPlayer())
        game::AskForKeyPress(CONST_S("Bodypart severed! [press any key to continue]"));
    }

    if(CanPanicFromSeveredBodyPart()
       && RAND() % 100 < GetPanicLevel()
       && !StateIsActivated(PANIC)
       && !StateIsActivated(FEARLESS)
       && !IsDead())
      BeginTemporaryState(PANIC, 1000 + RAND() % 1001);

    SpecialBodyPartSeverReaction();
  }

  if(!IsDead())
    CheckPanic(500);

  return Damage;
}

/* Returns 0 if bodypart disappears */

item* character::SevereBodyPart(int BodyPartIndex, truth ForceDisappearance, stack* EquipmentDropStack)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(StateIsActivated(LEPROSY))
    BodyPart->GetMainMaterial()->SetIsInfectedByLeprosy(true);

  if(BodyPartIndex == HEAD_INDEX && StateIsActivated(PARASITE_MIND_WORM))
    DeActivateTemporaryState(PARASITE_MIND_WORM);

  if(ForceDisappearance
     || BodyPartsDisappearWhenSevered()
     || StateIsActivated(POLYMORPHED)
     || game::AllBodyPartsVanish())
  {
    BodyPart->DropEquipment(EquipmentDropStack);
    BodyPart->RemoveFromSlot();
    CalculateAttributeBonuses();
    CalculateBattleInfo();
    BodyPart->SendToHell();
    SignalPossibleTransparencyChange();
    RemoveTraps(BodyPartIndex);
    return 0;
  }
  else
  {
    BodyPart->SetOwnerDescription("of " + GetName(INDEFINITE));
    BodyPart->SetIsUnique(LeftOversAreUnique());
    UpdateBodyPartPicture(BodyPartIndex, true);
    BodyPart->RemoveFromSlot();
    BodyPart->RandomizePosition();
    CalculateAttributeBonuses();
    CalculateBattleInfo();
    BodyPart->Enable();
    SignalPossibleTransparencyChange();
    RemoveTraps(BodyPartIndex);
    return BodyPart;
  }
}

void character::IgniteBodyPart(int BodyPartIndex, int Damage)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);
  BodyPart->TestActivationEnergy(Damage);
}

/* The second int is actually TargetFlags, which is not used here, but seems to be used
   in humanoid::ReceiveDamage. Returns true if the character really receives damage */

truth character::ReceiveDamage(character* Damager, int Damage, int Type, int, int Direction,
                               truth, truth PenetrateArmor, truth Critical, truth ShowMsg)
{
  truth Affected = ReceiveBodyPartDamage(Damager, Damage, Type, 0, Direction, PenetrateArmor, Critical, ShowMsg);

  if(DamageTypeAffectsInventory(Type))
  {
    for(int c = 0; c < GetEquipments(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
        Equipment->ReceiveDamage(Damager, Damage, Type);
    }

    GetStack()->ReceiveDamage(Damager, Damage, Type);
  }

  return Affected;
}

festring character::GetDescription(int Case) const
{
  if(IsPlayer())
    return CONST_S("you");
  else if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return CONST_S("something");
}

festring character::GetPersonalPronoun(truth PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("you");
  else if(GetSex() == UNDEFINED
          || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("it");
  else if(GetSex() == MALE)
    return CONST_S("he");
  else
    return CONST_S("she");
}

festring character::GetPossessivePronoun(truth PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("your");
  else if(GetSex() == UNDEFINED
          || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("its");
  else if(GetSex() == MALE)
    return CONST_S("his");
  else
    return CONST_S("her");
}

festring character::GetObjectPronoun(truth PlayersView) const
{
  if(IsPlayer() && PlayersView)
    return CONST_S("you");
  else if(GetSex() == UNDEFINED
          || (PlayersView && !CanBeSeenByPlayer() && !game::GetSeeWholeMapCheatMode()))
    return CONST_S("it");
  else if(GetSex() == MALE)
    return CONST_S("him");
  else
    return CONST_S("her");
}

void character::AddName(festring& String, int Case) const
{
  if(AssignedName.IsEmpty())
    id::AddName(String, Case);
  else if(!(Case & PLURAL))
  {
    if(!ShowClassDescription())
      String << AssignedName;
    else
    {
      String << AssignedName << ' ';
      id::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
    }
  }
  else
  {
    id::AddName(String, Case);
    String << " named " << AssignedName;
  }
}

int character::GetHungerState() const
{
  if(!UsesNutrition())
    return NOT_HUNGRY;
  else if(GetNP() > OVER_FED_LEVEL)
    return OVER_FED;
  else if(GetNP() > BLOATED_LEVEL)
    return BLOATED;
  else if(GetNP() > SATIATED_LEVEL)
    return SATIATED;
  else if(GetNP() > NOT_HUNGER_LEVEL)
    return NOT_HUNGRY;
  else if(GetNP() > HUNGER_LEVEL)
    return HUNGRY;
  else if(GetNP() > VERY_HUNGER_LEVEL)
    return VERY_HUNGRY;
  else
    return STARVING;
}

truth character::CanConsume(material* Material) const
{
  return GetConsumeFlags() & Material->GetConsumeType();
}

void character::SetTemporaryStateCounter(long State, int What)
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] = What;
}

void character::EditTemporaryStateCounter(long State, int What)
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      TemporaryStateCounter[c] += What;
}

int character::GetTemporaryStateCounter(long State) const
{
  for(int c = 0; c < STATES; ++c)
    if((1 << c) & State)
      return TemporaryStateCounter[c];

  ABORT("Illegal GetTemporaryStateCounter request!");
  return 0;
}

truth character::CheckKick() const
{
  if(!CanKick())
  {
    if(IsPlayer())
      ADD_MESSAGE("This race can't kick.");

    return false;
  }
  else
    return true;
}

int character::GetResistance(int Type) const
{
  switch(Type&0xFFF)
  {
   case PHYSICAL_DAMAGE:
   case DRAIN:
   case MUSTARD_GAS_DAMAGE:
   case PSI:
    return 0;
   case ENERGY: return GetEnergyResistance();
   case FIRE: return GetFireResistance();
   case POISON: return GetPoisonResistance();
   case ELECTRICITY: return GetElectricityResistance();
   case ACID: return GetAcidResistance();
   case SOUND: return GetSoundResistance();
  }

  ABORT("Resistance lack detected!");
  return 0;
}

void character::Regenerate()
{
  if(StateIsActivated(REGENERATION) && !(RAND() % 3000))
  {
    bodypart* NewBodyPart = GenerateRandomBodyPart();

    if(NewBodyPart)
    {
      NewBodyPart->SetHP(1);

      if(IsPlayer())
        ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), NewBodyPart->GetBodyPartName().CStr());
    }
  }

  if(HP == MaxHP)
    return;

  long RegenerationBonus = 0;
  truth NoHealableBodyParts = true;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->CanRegenerate())
    {
      RegenerationBonus += BodyPart->GetMaxHP();

      if(NoHealableBodyParts && BodyPart->GetHP() < BodyPart->GetMaxHP())
        NoHealableBodyParts = false;
    }
  }

  if(!RegenerationBonus || NoHealableBodyParts)
    return;

  RegenerationBonus *= (50 + GetAttribute(ENDURANCE));

  if(Action && Action->IsRest())
  {
    if(SquaresUnder == 1)
      RegenerationBonus *= GetSquareUnder()->GetRestModifier() << 1;
    else
    {
      int Lowest = GetSquareUnder(0)->GetRestModifier();

      for(int c = 1; c < GetSquaresUnder(); ++c)
      {
        int Mod = GetSquareUnder(c)->GetRestModifier();

        if(Mod < Lowest)
          Lowest = Mod;
      }

      RegenerationBonus *= Lowest << 1;
    }
  }

  RegenerationCounter += RegenerationBonus;

  while(RegenerationCounter > 1250000)
  {
    bodypart* BodyPart = HealHitPoint();

    if(!BodyPart)
      break;

    EditNP(-Max(7500 / MaxHP, 1));
    RegenerationCounter -= 1250000;
    int HP = BodyPart->GetHP();
    EditExperience(ENDURANCE, Min(1000 * BodyPart->GetMaxHP() / (HP * HP), 300), 1000);
  }
}

void character::PrintInfo() const
{
  felist Info(CONST_S("Information about ") + GetName(DEFINITE));

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if((EquipmentEasilyRecognized(c) || game::WizardModeIsActive()) && Equipment)
    {
      int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
      Info.AddEntry(festring(GetEquipmentName(c)) + ": " + Equipment->GetName(INDEFINITE),
                    LIGHT_GRAY, 0, ImageKey, true);
    }
  }

  if(Info.IsEmpty())
    ADD_MESSAGE("There's nothing special to tell about %s.", CHAR_NAME(DEFINITE));
  else
  {
    game::SetStandardListAttributes(Info);
    Info.SetEntryDrawer(game::ItemEntryDrawer);
    Info.Draw();
  }

  game::ClearItemDrawVector();
}

truth character::TryToRiseFromTheDead()
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
    {
      BodyPart->ResetSpoiling();

      if(BodyPart->CanRegenerate() || BodyPart->GetHP() < 1)
        BodyPart->SetHP(1);
    }
  }

  ResetStates();
  return true;
}

truth character::RaiseTheDead(character*)
{
  truth Useful = false;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(!BodyPart && CanCreateBodyPart(c))
    {
      CreateBodyPart(c)->SetHP(1);

      if(IsPlayer())
        ADD_MESSAGE("Suddenly you grow a new %s.", GetBodyPartName(c).CStr());
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), GetBodyPartName(c).CStr());

      Useful = true;
    }
    else if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < 1)
      BodyPart->SetHP(1);
  }

  if(!Useful)
  {
    if(IsPlayer())
      ADD_MESSAGE("You shudder.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s shudders.", CHAR_NAME(DEFINITE));
  }

  return Useful;
}

void character::SetSize(int Size)
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
      BodyPart->SetSize(GetBodyPartSize(c, Size));
  }
}

long character::GetBodyPartSize(int I, int TotalSize) const
{
  if(I == TORSO_INDEX)
    return TotalSize;
  else
  {
    ABORT("Weird bodypart size request for a character!");
    return 0;
  }
}

long character::GetBodyPartVolume(int I) const
{
  if(I == TORSO_INDEX)
    return GetTotalVolume();
  else
  {
    ABORT("Weird bodypart volume request for a character!");
    return 0;
  }
}

void character::CreateBodyParts(int SpecialFlags)
{
  for(int c = 0; c < BodyParts; ++c)
    if(CanCreateBodyPart(c))
      CreateBodyPart(c, SpecialFlags);
}

void character::RestoreBodyParts()
{
  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      CreateBodyPart(c);
}

void character::UpdatePictures()
{
  if(!PictureUpdatesAreForbidden())
    for(int c = 0; c < BodyParts; ++c)
      UpdateBodyPartPicture(c, false);
}

bodypart* character::MakeBodyPart(int I) const
{
  if(I == TORSO_INDEX)
    return normaltorso::Spawn(0, NO_MATERIALS);
  else
  {
    ABORT("Weird bodypart to make for a character!");
    return 0;
  }
}

bodypart* character::CreateBodyPart(int I, int SpecialFlags)
{
  bodypart* BodyPart = MakeBodyPart(I);
  material* Material = CreateBodyPartMaterial(I, GetBodyPartVolume(I));
  BodyPart->InitMaterials(Material, false);
  BodyPart->SetSize(GetBodyPartSize(I, GetTotalSize()));
  BodyPart->SetBloodMaterial(GetBloodMaterial());
  BodyPart->SetNormalMaterial(Material->GetConfig());
  SetBodyPart(I, BodyPart);
  BodyPart->InitSpecialAttributes();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdateBodyPartPicture(I, false);

  if(!IsInitializing())
  {
    CalculateBattleInfo();
    SendNewDrawRequest();
    SignalPossibleTransparencyChange();
  }

  return BodyPart;
}

v2 character::GetBodyPartBitmapPos(int I, truth) const
{
  if(I == TORSO_INDEX)
    return GetTorsoBitmapPos();
  else
  {
    ABORT("Weird bodypart BitmapPos request for a character!");
    return v2();
  }
}

void character::UpdateBodyPartPicture(int I, truth Severed)
{
  bodypart* BP = GetBodyPart(I);

  if(BP)
  {
    BP->SetBitmapPos(GetBodyPartBitmapPos(I, Severed));
    BP->GetMainMaterial()->SetSkinColor(GetBodyPartColorA(I, Severed));
    BP->GetMainMaterial()->SetSkinColorIsSparkling(GetBodyPartSparkleFlags(I) & SPARKLING_A);
    BP->SetMaterialColorB(GetBodyPartColorB(I, Severed));
    BP->SetMaterialColorC(GetBodyPartColorC(I, Severed));
    BP->SetMaterialColorD(GetBodyPartColorD(I, Severed));
    BP->SetSparkleFlags(GetBodyPartSparkleFlags(I));
    BP->SetSpecialFlags(GetSpecialBodyPartFlags(I));
    BP->SetWobbleData(GetBodyPartWobbleData(I));
    BP->UpdatePictures();
  }
}

void character::LoadDataBaseStats()
{
  for(int c = 0; c < BASE_ATTRIBUTES; ++c)
  {
    BaseExperience[c] = DataBase->NaturalExperience[c];

    if(BaseExperience[c])
      LimitRef(BaseExperience[c], MIN_EXP, MAX_EXP);
  }

  SetMoney(GetDefaultMoney());
  const fearray<long>& Skills = GetKnownCWeaponSkills();

  if(Skills.Size)
  {
    const fearray<long>& Hits = GetCWeaponSkillHits();

    if(Hits.Size == 1)
    {
      for(uint c = 0; c < Skills.Size; ++c)
        if(Skills[c] < AllowedWeaponSkillCategories)
          CWeaponSkill[Skills[c]].AddHit(Hits[0] * 100);
    }
    else if(Hits.Size == Skills.Size)
    {
      for(uint c = 0; c < Skills.Size; ++c)
        if(Skills[c] < AllowedWeaponSkillCategories)
          CWeaponSkill[Skills[c]].AddHit(Hits[c] * 100);
    }
    else
      ABORT("Illegal weapon skill hit array size detected!");
  }
}

character* characterprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  character* Char = Spawner(0, LOAD);
  Char->Load(SaveFile);
  Char->CalculateAll();
  return Char;
}

void character::Initialize(int NewConfig, int SpecialFlags)
{
  Flags |= C_INITIALIZING|C_IN_NO_MSG_MODE;
  CalculateBodyParts();
  CalculateAllowedWeaponSkillCategories();
  CalculateSquaresUnder();
  BodyPartSlot = new bodypartslot[BodyParts];
  OriginalBodyPartID = new std::list<ulong>[BodyParts];
  CWeaponSkill = new cweaponskill[AllowedWeaponSkillCategories];
  SquareUnder = new square*[SquaresUnder];

  if(SquaresUnder == 1)
    *SquareUnder = 0;
  else
    memset(SquareUnder, 0, SquaresUnder * sizeof(square*));

  int c;

  for(c = 0; c < BodyParts; ++c)
    BodyPartSlot[c].SetMaster(this);

  if(!(SpecialFlags & LOAD))
  {
    ID = game::CreateNewCharacterID(this);
    databasecreator<character>::InstallDataBase(this, NewConfig);
    LoadDataBaseStats();
    TemporaryState |= GetClassStates();

    if(TemporaryState)
      for(c = 0; c < STATES; ++c)
        if(TemporaryState & (1 << c))
          TemporaryStateCounter[c] = PERMANENT;

    CreateBodyParts(SpecialFlags | NO_PIC_UPDATE);
    InitSpecialAttributes();
    CommandFlags = GetDefaultCommandFlags();

    if(GetAttribute(INTELLIGENCE, false) < 8) // gum
      CommandFlags &= ~DONT_CONSUME_ANYTHING_VALUABLE;

    if(!GetDefaultName().IsEmpty())
      SetAssignedName(GetDefaultName());
  }

  if(!(SpecialFlags & LOAD))
    PostConstruct();

  if(!(SpecialFlags & LOAD))
  {
    if(!(SpecialFlags & NO_EQUIPMENT))
      CreateInitialEquipment((SpecialFlags & NO_EQUIPMENT_PIC_UPDATE) >> 1);

    if(!(SpecialFlags & NO_PIC_UPDATE))
      UpdatePictures();

    CalculateAll();
    RestoreHP();
    RestoreStamina();
  }

  Flags &= ~(C_INITIALIZING|C_IN_NO_MSG_MODE);
}

truth character::TeleportNear(character* Caller)
{
  v2 Where = GetLevel()->GetNearestFreeSquare(this, Caller->GetPos());

  if(Where == ERROR_V2)
    return false;

  Move(Where, true);
  return true;
}

void character::ReceiveHeal(long Amount)
{
  int c;

  if(IsBurnt())
    HealBurntBodyParts(Amount);

  for(c = 0; c < Amount / 10; ++c)
    if(!HealHitPoint())
      break;

  Amount -= c * 10;

  if(RAND() % 10 < Amount)
    HealHitPoint();

  if(Amount >= 1000 || RAND() % 1000 < Amount)
  {
    bodypart* NewBodyPart = GenerateRandomBodyPart();

    if(!NewBodyPart)
      return;

    NewBodyPart->SetHP(1);

    if(IsPlayer())
      ADD_MESSAGE("You grow a new %s.", NewBodyPart->GetBodyPartName().CStr());
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s grows a new %s.", CHAR_NAME(DEFINITE), NewBodyPart->GetBodyPartName().CStr());
  }
}

void character::AddHealingLiquidConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks healthier.", CHAR_NAME(DEFINITE));
}

void character::ReceiveSchoolFood(long SizeOfEffect)
{
  SizeOfEffect += RAND() % SizeOfEffect;

  if(SizeOfEffect >= 250)
    VomitAtRandomDirection(SizeOfEffect);

  if(!(RAND() % 3) && SizeOfEffect >= 500
     && EditAttribute(ENDURANCE, SizeOfEffect / 500))
  {
    if(IsPlayer())
      ADD_MESSAGE("You gain a little bit of toughness for surviving this stuff.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s looks tougher.", CHAR_NAME(DEFINITE));
  }

  BeginTemporaryState(POISONED, (SizeOfEffect >> 1));
}

void character::AddSchoolFoodConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff tasted like vomit and old mousepads.");
}

void character::AddSchoolFoodHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! This stuff feels like vomit and old mousepads.");
}

void character::ReceiveNutrition(long SizeOfEffect)
{
  EditNP(SizeOfEffect);
}

void character::ReceiveOmmelBlood(long Amount)
{
  EditExperience(WILL_POWER, 500, Amount << 4);
  EditExperience(MANA, 500, Amount << 4);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelUrine(long Amount)
{
  EditExperience(ARM_STRENGTH, 500, Amount << 4);
  EditExperience(LEG_STRENGTH, 500, Amount << 4);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelCerumen(long Amount)
{
  EditExperience(INTELLIGENCE, 500, Amount << 5);
  EditExperience(WISDOM, 500, Amount << 5);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelSweat(long Amount)
{
  EditExperience(AGILITY, 500, Amount << 4);
  EditExperience(DEXTERITY, 500, Amount << 4);
  RestoreStamina();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelTears(long Amount)
{
  EditExperience(PERCEPTION, 500, Amount << 4);
  EditExperience(CHARISMA, 500, Amount << 4);

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelSnot(long Amount)
{
  EditExperience(ENDURANCE, 500, Amount << 5);
  RestoreLivingHP();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::ReceiveOmmelBone(long Amount)
{
  EditExperience(ARM_STRENGTH, 500, Amount << 6);
  EditExperience(LEG_STRENGTH, 500, Amount << 6);
  EditExperience(DEXTERITY, 500, Amount << 6);
  EditExperience(AGILITY, 500, Amount << 6);
  EditExperience(ENDURANCE, 500, Amount << 6);
  EditExperience(PERCEPTION, 500, Amount << 6);
  EditExperience(INTELLIGENCE, 500, Amount << 6);
  EditExperience(WISDOM, 500, Amount << 6);
  EditExperience(CHARISMA, 500, Amount << 6);
  RestoreLivingHP();
  RestoreStamina();

  if(IsPlayer())
    game::DoEvilDeed(Amount / 25);
}

void character::AddOmmelConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a primitive force coursing through your veins.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks more powerful.", CHAR_NAME(DEFINITE));
}

void character::ReceivePepsi(long Amount)
{
  ReceiveDamage(0, Amount / 100, POISON, TORSO);
  EditExperience(PERCEPTION, Amount, 1 << 14);

  if(CheckDeath(CONST_S("was poisoned by pepsi"), 0))
    return;

  if(IsPlayer())
    game::DoEvilDeed(Amount / 10);
}

void character::AddPepsiConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Urgh. You feel your guruism fading away.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very lame.", CHAR_NAME(DEFINITE));
}

void character::AddCocaColaConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your guruism rising!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks awesome.", CHAR_NAME(DEFINITE));
}

void character::ReceiveDarkness(long Amount)
{
  EditExperience(INTELLIGENCE, -Amount / 5, 1 << 13);
  EditExperience(WISDOM, -Amount / 5, 1 << 13);
  EditExperience(CHARISMA, -Amount / 5, 1 << 13);

  if(IsPlayer())
    game::DoEvilDeed(int(Amount / 50));
}

void character::AddFrogFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Arg. You feel the fate of a navastater placed upon you...");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks like a navastater.", CHAR_NAME(DEFINITE));
}

void character::ReceiveKoboldFlesh(long)
{
  /* As it is commonly known, the possibility of fainting per 500 cubic
     centimeters of kobold flesh is exactly 5%. */

  if(!(RAND() % 20))
  {
    if(IsPlayer())
      ADD_MESSAGE("You lose control of your legs and fall down.");

    LoseConsciousness(250 + RAND_N(250));
  }
}

void character::AddKoboldFleshConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff tasted really funny.");
}

void character::AddKoboldFleshHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very funny.");
}

void character::AddBoneConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel like a hippie.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s barks happily.", CHAR_NAME(DEFINITE)); // this suspects that nobody except dogs can eat bones
}

truth character::RawEditAttribute(double& Experience, int Amount) const
{
  /* Check if the attribute is disabled for creature */

  if(!Experience)
    return false;

  if((Amount < 0 && Experience < 2 * EXP_MULTIPLIER)
     || (Amount > 0 && Experience > 999 * EXP_MULTIPLIER))
    return false;

  Experience += Amount * EXP_MULTIPLIER;
  LimitRef<double>(Experience, MIN_EXP, MAX_EXP);
  return true;
}

void character::DrawPanel(truth AnimationDraw) const
{
  if(AnimationDraw)
  {
    DrawStats(true);
    return;
  }

  igraph::BlitBackGround(v2(19 + (game::GetMaxScreenXSize() << 4), 0),
                         v2(RES.X - 19 - (game::GetMaxScreenXSize() << 4), RES.Y));
  igraph::BlitBackGround(v2(16, 45 + (game::GetMaxScreenYSize() << 4)),
                         v2(game::GetMaxScreenXSize() << 4, 9));
  FONT->Printf(DOUBLE_BUFFER, v2(16, 45 + (game::GetMaxScreenYSize() << 4)), WHITE, "%s", GetPanelName().CStr());
  game::UpdateAttributeMemory();
  int PanelPosX = RES.X - 96;
  int PanelPosY = DrawStats(false);
  PrintAttribute("End", ENDURANCE, PanelPosX, PanelPosY++);
  PrintAttribute("Per", PERCEPTION, PanelPosX, PanelPosY++);
  PrintAttribute("Int", INTELLIGENCE, PanelPosX, PanelPosY++);
  PrintAttribute("Wis", WISDOM, PanelPosX, PanelPosY++);
  PrintAttribute("Will", WILL_POWER, PanelPosX, PanelPosY++);
  PrintAttribute("Cha", CHARISMA, PanelPosX, PanelPosY++);
  PrintAttribute("Mana", MANA, PanelPosX, PanelPosY++);
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Ht   %d cm", GetSize());
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Wt   %d kg", GetTotalCharacterWeight());
  ++PanelPosY;
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10),
               IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  ++PanelPosY;
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Gold: %ld", GetMoney());
  ++PanelPosY;

  if(game::IsInWilderness())
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Worldmap");
  else
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "%s",
                 game::GetCurrentDungeon()->GetShortLevelDescription(game::GetCurrentLevelIndex()).CapitalizeCopy().CStr());

  ivantime Time;
  game::GetTime(Time);
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Day %d", Time.Day);
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Time %d:%s%d", Time.Hour,
                                                                                      Time.Min < 10 ? "0" : "",
                                                                                      Time.Min);
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "Turn %ld", game::GetTurn());

  ++PanelPosY;

  if(GetAction())
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, "%s",
                 festring(GetAction()->GetDescription()).CapitalizeCopy().CStr());

  for(int c = 0; c < STATES; ++c)
    if(!(StateData[c].Flags & SECRET)
       && StateIsActivated(1 << c)
       && (1 << c != HASTE || !StateIsActivated(SLOW))
       && (1 << c != SLOW || !StateIsActivated(HASTE)))
      FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10),
                   (1 << c) & EquipmentState || TemporaryStateCounter[c] >= PERMANENT ? BLUE : WHITE,
                   "%s", StateData[c].Description);

  static cchar* HungerStateStrings[] = { "Starving!", "Very hungry", "Hungry", "", "Satiated", "Bloated", "Overfed!" };
  static cpackcol16 HungerStateColors[] = { RED, RED, YELLOW, 0, WHITE, WHITE, YELLOW };
  int HungerState = GetHungerState();
  if(HungerState != NOT_HUNGRY)
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10),
                 HungerStateColors[HungerState], HungerStateStrings[HungerState]);

  static cchar* BurdenStateStrings[] = { "Overload!", "Stressed", "Burdened" };
  static cpackcol16 BurdenStateColors[] = { RED, RED, YELLOW };
  int BurdenState = GetBurdenState();
  if(BurdenState != UNBURDENED)
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10),
                 BurdenStateColors[BurdenState], BurdenStateStrings[BurdenState]);

  static cchar* TirednessStateStrings[] = { "Fainting!", "Exhausted" };
  static cpackcol16 TirednessStateColors[] = { RED, YELLOW };
  int TirednessState = GetTirednessState();
  if(TirednessState != UNTIRED)
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10),
                 TirednessStateColors[TirednessState], TirednessStateStrings[TirednessState]);

  if(game::PlayerIsRunning())
  {
    FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, GetRunDescriptionLine(0));
    cchar* SecondLine = GetRunDescriptionLine(1);

    if(SecondLine[0] != '\0')
      FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY++ * 10), WHITE, SecondLine);
  }
}

void character::CalculateDodgeValue()
{
  DodgeValue = 0.05 * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(IsFlying())
    DodgeValue *= 2;

  if(DodgeValue < 1)
    DodgeValue = 1;
}

truth character::DamageTypeAffectsInventory(int Type)
{
  switch(Type&0xFFF)
  {
   case SOUND:
   case ENERGY:
   case ACID:
   case FIRE:
   case ELECTRICITY:
    return true;
   case PHYSICAL_DAMAGE:
   case POISON:
   case DRAIN:
   case MUSTARD_GAS_DAMAGE:
   case PSI:
    return false;
  }

  ABORT("Unknown reaping effect destroyed dungeon!");
  return false;
}

int character::CheckForBlockWithArm(character* Enemy, item* Weapon, arm* Arm,
                                    double WeaponToHitValue, int Damage, int Success, int Type)
{
  int BlockStrength = Arm->GetBlockCapability();
  double BlockValue = Arm->GetBlockValue();

  if(BlockStrength && BlockValue)
  {
    item* Blocker = Arm->GetWielded();

    if(RAND() % int(100 + WeaponToHitValue / BlockValue / (1 << BlocksSinceLastTurn) * (100 + Success)) < 100)
    {
      int NewDamage = BlockStrength < Damage ? Damage - BlockStrength : 0;

      switch(Type)
      {
       case UNARMED_ATTACK:
        AddBlockMessage(Enemy, Blocker, Enemy->UnarmedHitNoun(), NewDamage);
        break;
       case WEAPON_ATTACK:
        AddBlockMessage(Enemy, Blocker, "attack", NewDamage);
        break;
       case KICK_ATTACK:
        AddBlockMessage(Enemy, Blocker, Enemy->KickNoun(), NewDamage);
        break;
       case BITE_ATTACK:
        AddBlockMessage(Enemy, Blocker, Enemy->BiteNoun(), NewDamage);
        break;
      }

      long Weight = Blocker->GetWeight();
      long StrExp = Limit(15 * Weight / 200L, 75L, 300L);
      long DexExp = Weight ? Limit(75000L / Weight, 75L, 300L) : 300;
      Arm->EditExperience(ARM_STRENGTH, StrExp, 1 << 8);
      Arm->EditExperience(DEXTERITY, DexExp, 1 << 8);
      EditStamina(-10000 / GetAttribute(ARM_STRENGTH), false);

      if(Arm->TwoHandWieldIsActive())
      {
        arm* PairArm = Arm->GetPairArm();
        PairArm->EditExperience(ARM_STRENGTH, StrExp, 1 << 8);
        PairArm->EditExperience(DEXTERITY, DexExp, 1 << 8);
      }

      Blocker->WeaponSkillHit(Enemy->CalculateWeaponSkillHits(this));
      Blocker->ReceiveDamage(this, Damage, PHYSICAL_DAMAGE);

      Blocker->BlockEffect(this, Enemy, Weapon, Type);

      if(Weapon)
        Weapon->ReceiveDamage(Enemy, Damage - NewDamage, PHYSICAL_DAMAGE);

      if(BlocksSinceLastTurn < 16)
        ++BlocksSinceLastTurn;

      return NewDamage;
    }
  }

  return Damage;
}

long character::GetStateAPGain(long BaseAPGain) const
{
  if(!StateIsActivated(HASTE) == !StateIsActivated(SLOW))
    return BaseAPGain;
  else if(StateIsActivated(HASTE))
    return (BaseAPGain * 5) >> 2;
  else
    return (BaseAPGain << 2) / 5;
}

void character::SignalEquipmentAdd(int EquipmentIndex)
{
  item* Equipment = GetEquipment(EquipmentIndex);

  if(Equipment->IsInCorrectSlot(EquipmentIndex))
  {
    long AddedStates = Equipment->GetGearStates();

    if(AddedStates)
      for(int c = 0; c < STATES; ++c)
        if(AddedStates & (1 << c))
        {
          if(!StateIsActivated(1 << c))
          {
            if(!IsInNoMsgMode())
              (this->*StateData[c].PrintBeginMessage)();

            EquipmentState |= 1 << c;

            if(StateData[c].BeginHandler)
              (this->*StateData[c].BeginHandler)();
          }
          else
            EquipmentState |= 1 << c;
        }
  }

  if(!IsInitializing() && Equipment->IsInCorrectSlot(EquipmentIndex))
    ApplyEquipmentAttributeBonuses(Equipment);
}

void character::SignalEquipmentRemoval(int, citem* Item)
{
  CalculateEquipmentState();

  if(CalculateAttributeBonuses())
    CheckDeath(festring("lost ") + GetPossessivePronoun(false) + " vital " + Item->GetName(INDEFINITE));
}

void character::CalculateEquipmentState()
{
  int c;
  long Back = EquipmentState;
  EquipmentState = 0;

  for(c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && Equipment->IsInCorrectSlot(c))
      EquipmentState |= Equipment->GetGearStates();
  }

  for(c = 0; c < STATES; ++c)
    if(Back & (1 << c) && !StateIsActivated(1 << c))
    {
      if(StateData[c].EndHandler)
      {
        (this->*StateData[c].EndHandler)();

        if(!IsEnabled())
          return;
      }

      if(!IsInNoMsgMode())
        (this->*StateData[c].PrintEndMessage)();
    }
}

/* Counter = duration in ticks */

void character::BeginTemporaryState(long State, int Counter)
{
  if(!Counter)
    return;

  int Index;

  if(State == POLYMORPHED)
    ABORT("No Polymorphing with BeginTemporaryState!");

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == State)
      break;

  if(Index == STATES)
    ABORT("BeginTemporaryState works only when State == 2 ^ n!");

  if(TemporaryStateIsActivated(State))
  {
    int OldCounter = GetTemporaryStateCounter(State);

    if(OldCounter != PERMANENT)
      EditTemporaryStateCounter(State, Max(Counter, 50 - OldCounter));
  }
  else if(StateData[Index].IsAllowed == 0 || (this->*StateData[Index].IsAllowed)())
  {
    SetTemporaryStateCounter(State, Max(Counter, 50));

    if(!EquipmentStateIsActivated(State))
    {
      if(!IsInNoMsgMode())
        (this->*StateData[Index].PrintBeginMessage)();

      ActivateTemporaryState(State);

      if(StateData[Index].BeginHandler)
        (this->*StateData[Index].BeginHandler)();
    }
    else
      ActivateTemporaryState(State);
  }
}

void character::HandleStates()
{
  if(!TemporaryState && !EquipmentState)
    return;

  for(int c = 0; c < STATES; ++c)
  {
    if(TemporaryState & (1 << c) && TemporaryStateCounter[c] != PERMANENT)
    {
      if(!--TemporaryStateCounter[c])
      {
        TemporaryState &= ~(1 << c);

        if(!(EquipmentState & (1 << c)))
        {
          if(StateData[c].EndHandler)
          {
            (this->*StateData[c].EndHandler)();

            if(!IsEnabled())
              return;
          }

          if(!TemporaryStateCounter[c])
            (this->*StateData[c].PrintEndMessage)();
        }
      }
    }

    if(StateIsActivated(1 << c))
      if(StateData[c].Handler)
        (this->*StateData[c].Handler)();

    if(!IsEnabled())
      return;
  }
}

void character::PrintBeginPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your mind has total control over your body.");
}

void character::PrintEndPolymorphControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are somehow uncertain of your willpower.");
}

void character::PrintBeginLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You hear Hell's gates being locked just now.");
}

void character::PrintEndLifeSaveMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the Afterlife is welcoming you once again.");
}

void character::PrintBeginLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly notice you've always loved full moons.");
}

void character::PrintEndLycanthropyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the wolf inside you has had enough of your bad habits.");
}

void character::PrintBeginVampirismMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly decide you have always hated garlic.");
}

void character::PrintEndVampirismMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You recall your delight of the morning sunshine back in New Attnam.");
}

void character::PrintBeginFearlessMessage () const
{
  if(IsPlayer())
    ADD_MESSAGE("You shout: \"I am invincible!\"");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very calm.", CHAR_NAME(DEFINITE));
}

void character::PrintEndFearlessMessage () const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything looks much more dangerous now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s seems to have lost all confidence.", CHAR_NAME(DEFINITE));
}

void character::PrintBeginFastingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your life-force invigorating your entire body.");
}

void character::PrintEndFastingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your stomach growls in discontent.");
}

void character::PrintBeginInvisibilityMessage() const
{
  if((PLAYER->StateIsActivated(INFRA_VISION) && IsWarm())
     || (PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
  {
    if(IsPlayer())
      ADD_MESSAGE("You seem somehow transparent.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s seems somehow transparent.", CHAR_NAME(DEFINITE));
  }
  else
  {
    if(IsPlayer())
      ADD_MESSAGE("You fade away.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears!", CHAR_NAME(DEFINITE));
  }
}

void character::PrintEndInvisibilityMessage() const
{
  if((PLAYER->StateIsActivated(INFRA_VISION) && IsWarm())
     || (PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
  {
    if(IsPlayer())
      ADD_MESSAGE("Your notice your transparency has ended.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("The appearance of %s seems far more solid now.", CHAR_NAME(INDEFINITE));
  }
  else
  {
    if(IsPlayer())
      ADD_MESSAGE("You reappear.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s appears from nowhere!", CHAR_NAME(INDEFINITE));
  }
}

void character::PrintBeginEtherealityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel like many miscible droplets of ether.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s melds into the surroundings.", CHAR_NAME(DEFINITE));
}

void character::PrintEndEtherealityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You drop out of the firmament, feeling suddenly quite dense.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s displaces the air with a puff.", CHAR_NAME(INDEFINITE));
}

void character::PrintBeginSwimmingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You fondly remember the sound of ocean waves.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks wet.", CHAR_NAME(DEFINITE));
}

void character::PrintEndSwimmingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly remember how you nearly drowned as a child.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s looks less wet.", CHAR_NAME(INDEFINITE));
}

void character::PrintBeginInfraVisionMessage() const
{
  if(IsPlayer())
  {
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You reappear.");
    else
      ADD_MESSAGE("You feel your perception being magically altered.");
  }
}

void character::PrintEndInfraVisionMessage() const
{
  if(IsPlayer())
  {
    if(StateIsActivated(INVISIBLE) && IsWarm() && !(StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5))
      ADD_MESSAGE("You disappear.");
    else
      ADD_MESSAGE("You feel your perception returning to normal.");
  }
}

void character::PrintBeginESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly feel like being only a tiny part of a great network of intelligent minds.");
}

void character::PrintEndESPMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You are filled with desire to be just yourself from now on.");
}

void character::PrintBeginHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::PrintEndHasteMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintBeginSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Everything seems to move much faster now.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks slower!", CHAR_NAME(DEFINITE));
}

void character::PrintEndSlowMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Time slows down to a crawl.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks faster!", CHAR_NAME(DEFINITE));
}

void character::EndPolymorph()
{
  ForceEndPolymorph();
}

character* character::ForceEndPolymorph()
{
  if(IsPlayer())
    ADD_MESSAGE("You return to your true form.");
  else if(game::IsInWilderness())
  {
    ActivateTemporaryState(POLYMORPHED);
    SetTemporaryStateCounter(POLYMORPHED, 10);
    return this; // fast gum solution, state ends when the player enters a dungeon
  }
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s returns to %s true form.", CHAR_NAME(DEFINITE), GetPossessivePronoun().CStr());

  RemoveTraps();

  if(GetAction())
    GetAction()->Terminate(false);

  v2 Pos = GetPos();
  SendToHell();
  Remove();
  character* Char = GetPolymorphBackup();
  Flags |= C_IN_NO_MSG_MODE;
  Char->Flags |= C_IN_NO_MSG_MODE;
  Char->ChangeTeam(GetTeam());

  if(GetTeam()->GetLeader() == this)
    GetTeam()->SetLeader(Char);

  SetPolymorphBackup(0);
  Char->PutToOrNear(Pos);
  Char->Enable();
  Char->Flags &= ~C_POLYMORPHED;
  GetStack()->MoveItemsTo(Char->GetStack());
  DonateEquipmentTo(Char);
  Char->SetMoney(GetMoney());
  Flags &= ~C_IN_NO_MSG_MODE;
  Char->Flags &= ~C_IN_NO_MSG_MODE;
  Char->CalculateAll();
  Char->SetAssignedName(GetAssignedName());

  if(IsPlayer())
  {
    Flags &= ~C_PLAYER;
    game::SetPlayer(Char);
    game::SendLOSUpdateRequest();
    UpdateESPLOS();
  }

  Char->TestWalkability();
  return Char;
}

void character::LycanthropyHandler()
{
  if(StateIsActivated(POLYMORPH_LOCK))
    return;

  if(GetType() == werewolfwolf::ProtoType.GetIndex())
    return;

  if(!(RAND() % 2000))
  {
    if(StateIsActivated(POLYMORPH_CONTROL)
       && (IsPlayer() ? !game::TruthQuestion(CONST_S("Do you wish to change into a werewolf? [y/N]")) : false))
      return;

    Polymorph(werewolfwolf::Spawn(), 1000 + RAND() % 2000);
  }
}

void character::SaveLife()
{
  if(TemporaryStateIsActivated(LIFE_SAVED))
  {
    if(IsPlayer())
      ADD_MESSAGE("But wait! You glow briefly red and seem to be in a better shape!");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("But wait, suddenly %s glows briefly red and seems to be in a better shape!",
                  GetPersonalPronoun().CStr());

    DeActivateTemporaryState(LIFE_SAVED);
  }
  else
  {
    item* LifeSaver = 0;

    for(int c = 0; c < GetEquipments(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && Equipment->IsInCorrectSlot(c) && Equipment->GetGearStates() & LIFE_SAVED)
        LifeSaver = Equipment;
    }

    if(!LifeSaver)
      ABORT("The Universe can only kill you once!");

    if(IsPlayer())
      ADD_MESSAGE("But wait! Your %s glows briefly red and disappears and you seem to be in a better shape!",
                  LifeSaver->CHAR_NAME(UNARTICLED));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("But wait, suddenly %s %s glows briefly red and disappears and %s seems to be in a better shape!",
                  GetPossessivePronoun().CStr(), LifeSaver->CHAR_NAME(UNARTICLED), GetPersonalPronoun().CStr());

    LifeSaver->RemoveFromSlot();
    LifeSaver->SendToHell();
  }

  if(IsPlayer())
    game::AskForKeyPress(CONST_S("Life saved! [press any key to continue]"));

  RestoreBodyParts();
  ResetSpoiling();
  RestoreHP();
  RestoreStamina();
  ResetStates();

  if(GetNP() < SATIATED_LEVEL)
    SetNP(SATIATED_LEVEL);

  SendNewDrawRequest();

  if(GetAction())
    GetAction()->Terminate(false);
}

character* character::PolymorphRandomly(int MinDanger, int MaxDanger, int Time)
{
  character* NewForm = 0;

  if(StateIsActivated(POLYMORPH_LOCK))
  {
    ADD_MESSAGE("You feel uncertain about your body for a moment.");
    return NewForm;
  }
  if(StateIsActivated(POLYMORPH_CONTROL))
  {
    if(IsPlayer() && !IsPlayerAutoPlay())
    {
      if(!GetNewFormForPolymorphWithControl(NewForm))
        return NewForm;
    }
    else
      NewForm = protosystem::CreateMonster(MinDanger * 10, MaxDanger * 10, NO_EQUIPMENT);
  }
  else
    NewForm = protosystem::CreateMonster(MinDanger, MaxDanger, NO_EQUIPMENT);

  Polymorph(NewForm, Time);
  return NewForm;
}

/* In reality, the reading takes Time / (Intelligence * 10) turns */

void character::StartReading(item* Item, long Time)
{
  study* Read = study::Spawn(this);
  Read->SetLiteratureID(Item->GetID());

  if(game::WizardModeIsActive())
    Time = 1;

  Read->SetCounter(Time);
  SetAction(Read);

  if(IsPlayer())
    ADD_MESSAGE("You start reading %s.", Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s starts reading %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
}

/* Call when one makes something with his/her/its hands.
 * Difficulty of 5 takes about one turn, so it's the most common to use. */

void character::DexterityAction(int Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(DEXTERITY)));
  EditExperience(DEXTERITY, Difficulty * 15, 1 << 7);
}

/* If Theoretically != false, range is not a factor. */

truth character::CanBeSeenByPlayer(truth Theoretically, truth IgnoreESP) const
{
  if(IsEnabled() && !game::IsGenerating() && (Theoretically || GetSquareUnder()))
  {
    truth MayBeESPSeen = PLAYER->IsEnabled() && !IgnoreESP && PLAYER->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5 && !IsESPBlockedByEquipment();
    truth MayBeInfraSeen = PLAYER->IsEnabled() && PLAYER->StateIsActivated(INFRA_VISION) && IsWarm();
    truth Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

    if(game::IsInWilderness())
      return Visible;

    if(MayBeESPSeen
       && (Theoretically
           || GetDistanceSquareFrom(PLAYER) <= PLAYER->GetESPRangeSquare()))
      return true;
    else if(!Visible)
      return false;
    else
      return Theoretically || SquareUnderCanBeSeenByPlayer(MayBeInfraSeen);
  }
  else
    return false;
}

truth character::CanBeSeenBy(ccharacter* Who, truth Theoretically, truth IgnoreESP) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer(Theoretically, IgnoreESP);
  else
  {
    if(IsEnabled() && !game::IsGenerating() && (Theoretically || GetSquareUnder()))
    {
      truth MayBeESPSeen = Who->IsEnabled() && !IgnoreESP && Who->StateIsActivated(ESP) && GetAttribute(INTELLIGENCE) >= 5;
      truth MayBeInfraSeen = Who->IsEnabled() && Who->StateIsActivated(INFRA_VISION) && IsWarm();
      truth Visible = !StateIsActivated(INVISIBLE) || MayBeESPSeen || MayBeInfraSeen;

      if(game::IsInWilderness())
        return Visible;

      if(MayBeESPSeen
         && (Theoretically
             || GetDistanceSquareFrom(Who) <= Who->GetESPRangeSquare()))
        return true;
      else if(!Visible)
        return false;
      else
        return Theoretically || SquareUnderCanBeSeenBy(Who, MayBeInfraSeen);
    }
    else
      return false;
  }
}

truth character::SquareUnderCanBeSeenByPlayer(truth IgnoreDarkness) const
{
  if(!GetSquareUnder())
    return false;

  int S1 = SquaresUnder, S2 = PLAYER->SquaresUnder;

  if(S1 == 1 && S2 == 1)
  {
    if(GetSquareUnder()->CanBeSeenByPlayer(IgnoreDarkness))
      return true;

    if(IgnoreDarkness)
    {
      int LOSRangeSquare = PLAYER->GetLOSRangeSquare();

      if((GetPos() - PLAYER->GetPos()).GetLengthSquare() <= LOSRangeSquare)
      {
        eyecontroller::Map = GetLevel()->GetMap();
        return mapmath<eyecontroller>::DoLine(PLAYER->GetPos().X, PLAYER->GetPos().Y,
                                              GetPos().X, GetPos().Y, SKIP_FIRST);
      }
    }

    return false;
  }
  else
  {
    for(int c1 = 0; c1 < S1; ++c1)
    {
      lsquare* Square = GetLSquareUnder(c1);

      if(Square->CanBeSeenByPlayer(IgnoreDarkness))
        return true;
      else if(IgnoreDarkness)
      {
        v2 Pos = Square->GetPos();
        int LOSRangeSquare = PLAYER->GetLOSRangeSquare();

        for(int c2 = 0; c2 < S2; ++c2)
        {
          v2 PlayerPos = PLAYER->GetPos(c2);

          if((Pos - PlayerPos).GetLengthSquare() <= LOSRangeSquare)
          {
            eyecontroller::Map = GetLevel()->GetMap();

            if(mapmath<eyecontroller>::DoLine(PlayerPos.X, PlayerPos.Y, Pos.X, Pos.Y, SKIP_FIRST))
              return true;
          }
        }
      }
    }

    return false;
  }
}

truth character::SquareUnderCanBeSeenBy(ccharacter* Who, truth IgnoreDarkness) const
{
  int S1 = SquaresUnder, S2 = Who->SquaresUnder;
  int LOSRangeSquare = Who->GetLOSRangeSquare();

  if(S1 == 1 && S2 == 1)
    return GetSquareUnder()->CanBeSeenFrom(Who->GetPos(), LOSRangeSquare, IgnoreDarkness);
  else
  {
    for(int c1 = 0; c1 < S1; ++c1)
    {
      lsquare* Square = GetLSquareUnder(c1);

      for(int c2 = 0; c2 < S2; ++c2)
        if(Square->CanBeSeenFrom(Who->GetPos(c2), LOSRangeSquare, IgnoreDarkness))
          return true;
    }

    return false;
  }
}

int character::GetDistanceSquareFrom(ccharacter* Who) const
{
  int S1 = SquaresUnder, S2 = Who->SquaresUnder;

  if(S1 == 1 && S2 == 1)
    return (GetPos() - Who->GetPos()).GetLengthSquare();
  else
  {
    v2 MinDist(0x7FFF, 0x7FFF);
    int MinLength = 0xFFFF;

    for(int c1 = 0; c1 < S1; ++c1)
      for(int c2 = 0; c2 < S2; ++c2)
      {
        v2 Dist = GetPos(c1) - Who->GetPos(c2);

        if(Dist.X < 0)
          Dist.X = -Dist.X;

        if(Dist.Y < 0)
          Dist.Y = -Dist.Y;

        if(Dist.X <= MinDist.X && Dist.Y <= MinDist.Y)
        {
          MinDist = Dist;
          MinLength = Dist.GetLengthSquare();
        }
        else if(Dist.X < MinDist.X || Dist.Y < MinDist.Y)
        {
          int Length = Dist.GetLengthSquare();

          if(Length < MinLength)
          {
            MinDist = Dist;
            MinLength = Length;
          }
        }
      }

    return MinLength;
  }
}

void character::AttachBodyPart(bodypart* BodyPart)
{
  SetBodyPart(BodyPart->GetBodyPartIndex(), BodyPart);

  if(!AllowSpoil())
    BodyPart->ResetSpoiling();

  BodyPart->ResetPosition();
  BodyPart->UpdatePictures();
  CalculateAttributeBonuses();
  CalculateBattleInfo();
  SendNewDrawRequest();
  SignalPossibleTransparencyChange();
}

/* Returns true if the character has all bodyparts, false if not. */

truth character::HasAllBodyParts() const
{
  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      return false;

  return true;
}

truth character::HasFire() const
{
  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsBurning())
      return true;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);
    if(BodyPart)
      if(BodyPart->IsBurning())
        return true;
  }

  return false;
}

truth character::IsBurning() const
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);
    if(BodyPart)
      if(BodyPart->IsBurning())
        return true;
  }
  return false;
}

bodypart* character::GenerateRandomBodyPart()
{
  int NeededBodyPart[MAX_BODYPARTS];
  int Index = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c) && CanCreateBodyPart(c))
      NeededBodyPart[Index++] = c;

  return Index ? CreateBodyPart(NeededBodyPart[RAND() % Index]) : 0;
}

/* Searches the character's Stack and if it find some bodyparts there that are
   the character's old bodyparts returns a stackiterator to one of them (choosen
   in random). If no fitting bodyparts are found the function returns 0 */

bodypart* character::FindRandomOwnBodyPart(truth AllowNonLiving) const
{
  itemvector LostAndFound;

  for(int c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c))
      for(ulong ID : OriginalBodyPartID[c])
      {
        bodypart* Found = static_cast<bodypart*>(SearchForItem(ID));

        if(Found && (AllowNonLiving || Found->CanRegenerate()))
          LostAndFound.push_back(Found);
      }

  if(LostAndFound.empty())
    return 0;
  else
    return static_cast<bodypart*>(LostAndFound[RAND() % LostAndFound.size()]);
}

void character::PrintBeginPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You seem to be very ill.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks very ill.", CHAR_NAME(DEFINITE));
}

void character::PrintEndPoisonedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel better again.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks better.", CHAR_NAME(DEFINITE));
}

void character::PoisonedHandler()
{
  if(!(RAND() % 100))
    VomitAtRandomDirection(500 + RAND_N(250));

  int Damage = 0;

  for(int Used = 0; Used < GetTemporaryStateCounter(POISONED); Used += 100)
    if(!(RAND() % 100))
      ++Damage;

  if(Damage)
  {
    ReceiveDamage(0, Damage, POISON, ALL, 8, false, false, false, false);
    CheckDeath(CONST_S("died of acute poisoning"), 0);
  }
}

truth character::IsWarm() const
{
  return combinebodypartpredicates()(this, &bodypart::IsWarm, 1);
}

truth character::IsWarmBlooded() const
{
  return combinebodypartpredicates()(this, &bodypart::IsWarmBlooded, 1);
}

void character::BeginInvisibility()
{
  UpdatePictures();
  SendNewDrawRequest();
  SignalPossibleTransparencyChange();
}

void character::BeginInfraVision()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::BeginESP()
{
  if(IsPlayer())
    GetArea()->SendNewDrawRequest();
}

void character::BeginEthereality()
{
}

void character::BeginSwimming()
{
}

void character::EndInvisibility()
{
  UpdatePictures();
  SendNewDrawRequest();
  SignalPossibleTransparencyChange();
}

void character::EndInfraVision()
{
  if(IsPlayer() && IsEnabled())
    GetArea()->SendNewDrawRequest();
}

void character::EndESP()
{
  if(IsPlayer() && IsEnabled())
    GetArea()->SendNewDrawRequest();
}

void character::EndEthereality()
{
}

void character::EndSwimming()
{
}

void character::Draw(blitdata& BlitData) const
{
  col24 L = BlitData.Luminance;

  if(PLAYER->IsEnabled()
     && ((PLAYER->StateIsActivated(ESP)
          && GetAttribute(INTELLIGENCE) >= 5
          && (PLAYER->GetPos() - GetPos()).GetLengthSquare() <= PLAYER->GetESPRangeSquare())
         || (PLAYER->StateIsActivated(INFRA_VISION) && IsWarm())))
    BlitData.Luminance = ivanconfig::GetContrastLuminance();

  DrawBodyParts(BlitData);
  BlitData.Luminance = ivanconfig::GetContrastLuminance();
  BlitData.Src.Y = 16;
  cint SquareIndex = BlitData.CustomData & SQUARE_INDEX_MASK;

  if(GetTeam() == PLAYER->GetTeam() && !IsPlayer()
     && SquareIndex == GetTameSymbolSquareIndex())
  {
    BlitData.Src.X = 32;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);
  }

  if(IsFlying() && SquareIndex == GetFlySymbolSquareIndex())
  {
    BlitData.Src.X = 128;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);
  }

  if(IsSwimming() && SquareIndex == GetSwimmingSymbolSquareIndex())
  {
    BlitData.Src.X = 240;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);
  }

  if(GetAction() && GetAction()->IsUnconsciousness()
     && SquareIndex == GetUnconsciousSymbolSquareIndex())
  {
    BlitData.Src.X = 224;
    igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);
  }

  BlitData.Src.X = BlitData.Src.Y = 0;
  BlitData.Luminance = L;
}

void character::DrawBodyParts(blitdata& BlitData) const
{
  GetTorso()->Draw(BlitData);
}

void character::PrintBeginTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel jumpy.");
}

void character::PrintEndTeleportMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You suddenly realize you've always preferred walking to jumping.");
}

void character::PrintBeginDetectMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel curious about your surroundings.");
}

void character::PrintEndDetectMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You decide to rely on your own sight from now on.");
}

void character::TeleportHandler()
{
  if(!(RAND() % 1500) && !game::IsInWilderness())
  {
    if(IsPlayer())
      ADD_MESSAGE("You feel an urgent spatial relocation is now appropriate.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    TeleportRandomly();
  }
}

void character::DetectHandler()
{
  if(IsPlayer()) //the AI can't be asked position questions! So only the player can have this state really.
  {
    if(!(RAND() % 3000) && !game::IsInWilderness())
    {
      ADD_MESSAGE("Your mind wanders in search of something.");
      DoDetecting();
    }
  }
  else
    return;
}

void character::PrintBeginPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("An uncomfortable uncertainty of who you really are overwhelms you.");
}

void character::PrintEndPolymorphMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you are you and no one else.");
}

void character::PolymorphHandler()
{
  if(!(RAND() % 1500))
    PolymorphRandomly(1, 999999, 200 + RAND() % 800);
}

void character::PrintBeginPolymorphLockMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel incredibly stubborn about who you are.");
}

void character::PrintEndPolymorphLockMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel more open to new ideas.");
}

void character::PolymorphLockHandler()
{
  if (TemporaryStateIsActivated(POLYMORPHED))
  {
      EditTemporaryStateCounter(POLYMORPHED, 1);
      if (GetTemporaryStateCounter(POLYMORPHED) < 1000)
        EditTemporaryStateCounter(POLYMORPHED, 1);
  }
}

void character::PrintBeginTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel very controlled.");
}

void character::PrintEndTeleportControlMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your control slipping.");
}

void character::PrintBeginRegenerationMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your heart races.");
}

void character::PrintEndRegenerationMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your rapid heartbeat calms down.");
}

void character::PrintBeginDiseaseImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel especially healthy.");
}

void character::PrintEndDiseaseImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You develop a sudden fear of germs.");
}

void character::PrintBeginTeleportLockMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel firmly planted in reality.");
}

void character::PrintEndTeleportLockMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your mind soars far and wide.");
}

void character::TeleportLockHandler()
{
  if (StateIsActivated(TELEPORT_LOCK))
  {
    EditTemporaryStateCounter(TELEPORT_LOCK, 1);
    if (GetTemporaryStateCounter(TELEPORT_LOCK) < 1000)
      EditTemporaryStateCounter(TELEPORT_LOCK, 1);
  }
}

void character::DisplayStethoscopeInfo(character*) const
{
  game::RegionListItemEnable(false);
  game::RegionSilhouetteEnable(false);
  felist Info(CONST_S("Information about ") + GetDescription(DEFINITE));
  AddSpecialStethoscopeInfo(Info);
  Info.AddEntry(CONST_S("Endurance: ") + GetAttribute(ENDURANCE), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Perception: ") + GetAttribute(PERCEPTION), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Intelligence: ") + GetAttribute(INTELLIGENCE), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Wisdom: ") + GetAttribute(WISDOM), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Willpower: ") + GetAttribute(WILL_POWER), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Charisma: ") + GetAttribute(CHARISMA), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Mana: ") + GetAttribute(MANA), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Height: ") + GetSize() + " cm", LIGHT_GRAY);
  Info.AddEntry(CONST_S("Weight: ") + GetTotalCharacterWeight() + " kg", LIGHT_GRAY);
  Info.AddEntry(CONST_S("HP: ") + GetHP() + "/" + GetMaxHP(), IsInBadCondition() ? RED : LIGHT_GRAY);

  if(GetAction())
    Info.AddEntry(festring(GetAction()->GetDescription()).CapitalizeCopy(), LIGHT_GRAY);

  for(int c = 0; c < STATES; ++c)
    if(StateIsActivated(1 << c)
       && (1 << c != HASTE || !StateIsActivated(SLOW))
       && (1 << c != SLOW || !StateIsActivated(HASTE)))
      Info.AddEntry(StateData[c].Description, LIGHT_GRAY);

  switch(GetTirednessState())
  {
   case FAINTING:
    Info.AddEntry("Fainting", RED);
    break;
   case EXHAUSTED:
    Info.AddEntry("Exhausted", LIGHT_GRAY);
    break;
  }

  game::SetStandardListAttributes(Info);
  Info.Draw();
}

truth character::CanUseStethoscope(truth PrintReason) const
{
  if(PrintReason)
    ADD_MESSAGE("This type of monster can't use a stethoscope.");

  return false;
}

/* Effect used by at least Sophos.
 * NOTICE: Doesn't check for death! */

void character::TeleportSomePartsAway(int NumberToTeleport)
{
  if(StateIsActivated(TELEPORT_LOCK))
  {
    if(IsPlayer())
      ADD_MESSAGE("You feel very itchy for a moment.");

    return;
  }

  for(int c = 0; c < NumberToTeleport; ++c)
  {
    int RandomBodyPart = GetRandomNonVitalBodyPart();

    if(RandomBodyPart == NONE_INDEX)
    {
      for(; c < NumberToTeleport; ++c)
      {
        GetTorso()->SetHP((GetTorso()->GetHP() << 2) / 5);
        long TorsosVolume = GetTorso()->GetMainMaterial()->GetVolume() / 10;

        if(!TorsosVolume)
          break;

        long Amount = (RAND() % TorsosVolume) + 1;
        item* Lump = GetTorso()->GetMainMaterial()->CreateNaturalForm(Amount);
        GetTorso()->GetMainMaterial()->EditVolume(-Amount);
        Lump->MoveTo(GetNearLSquare(GetLevel()->GetRandomSquare())->GetStack());

        if(IsPlayer())
          ADD_MESSAGE("Parts of you teleport away.");
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("Parts of %s teleport away.", CHAR_NAME(DEFINITE));
      }
    }
    else
    {
      item* SeveredBodyPart = SevereBodyPart(RandomBodyPart);

      if(SeveredBodyPart)
      {
        GetNearLSquare(GetLevel()->GetRandomSquare())->AddItem(SeveredBodyPart);
        SeveredBodyPart->DropEquipment();

        if(IsPlayer())
          ADD_MESSAGE("Your %s teleports away.", GetBodyPartName(RandomBodyPart).CStr());
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s %s teleports away.", GetPossessivePronoun().CStr(), GetBodyPartName(RandomBodyPart).CStr());
      }
      else
      {
        if(IsPlayer())
          ADD_MESSAGE("Your %s disappears.", GetBodyPartName(RandomBodyPart).CStr());
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s %s disappears.", GetPossessivePronoun().CStr(), GetBodyPartName(RandomBodyPart).CStr());
      }
    }
  }
}

/* Returns an index of a random bodypart that is not vital. If no non-vital bodypart is found returns NONE_INDEX */

int character::GetRandomNonVitalBodyPart() const
{
  int OKBodyPart[MAX_BODYPARTS];
  int OKBodyParts = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c) && !BodyPartIsVital(c))
      OKBodyPart[OKBodyParts++] = c;

  return OKBodyParts ? OKBodyPart[RAND() % OKBodyParts] : NONE_INDEX;
}

int character::GetTotalCharacterWeight() const
{
  int CharacterWeight = 0;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
    {
      CharacterWeight += BodyPart->GetWeight();
    }
  }

  return (floor(CharacterWeight / 1000));
}

void character::CalculateVolumeAndWeight()
{
  Volume = Stack->GetVolume();
  Weight = Stack->GetWeight();
  BodyVolume = 0;
  CarriedWeight = Weight;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
    {
      BodyVolume += BodyPart->GetBodyPartVolume();
      Volume += BodyPart->GetVolume();
      CarriedWeight += BodyPart->GetCarriedWeight();
      Weight += BodyPart->GetWeight();
    }
  }
}

void character::SignalVolumeAndWeightChange()
{
  if(!IsInitializing())
  {
    CalculateVolumeAndWeight();

    if(IsEnabled())
      CalculateBurdenState();

    if(MotherEntity)
      MotherEntity->SignalVolumeAndWeightChange();
  }
}

void character::SignalEmitationIncrease(col24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
  {
    game::CombineLights(Emitation, EmitationUpdate);

    if(MotherEntity)
      MotherEntity->SignalEmitationIncrease(EmitationUpdate);
    else if(SquareUnder[0] && !game::IsInWilderness())
      for(int c = 0; c < GetSquaresUnder(); ++c)
        GetLSquareUnder()->SignalEmitationIncrease(EmitationUpdate);
  }
}

void character::SignalEmitationDecrease(col24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
  {
    col24 Backup = Emitation;
    CalculateEmitation();

    if(Backup != Emitation)
    {
      if(MotherEntity)
        MotherEntity->SignalEmitationDecrease(EmitationUpdate);
      else if(SquareUnder[0] && !game::IsInWilderness())
        for(int c = 0; c < GetSquaresUnder(); ++c)
          GetLSquareUnder(c)->SignalEmitationDecrease(EmitationUpdate);
    }
  }
}

void character::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
      game::CombineLights(Emitation, BodyPart->GetEmitation());
  }

  game::CombineLights(Emitation, Stack->GetEmitation());
}

void character::CalculateAll()
{
  Flags |= C_INITIALIZING;
  CalculateAttributeBonuses();
  CalculateVolumeAndWeight();
  CalculateEmitation();
  CalculateBodyPartMaxHPs(0);
  CalculateMaxStamina();
  CalculateBurdenState();
  CalculateBattleInfo();
  Flags &= ~C_INITIALIZING;
}

void character::CalculateHP()
{
  HP = sumbodypartproperties()(this, &bodypart::GetHP);
}

void character::CalculateMaxHP()
{
  MaxHP = sumbodypartproperties()(this, &bodypart::GetMaxHP);
}

void character::CalculateBodyPartMaxHPs(ulong Flags)
{
  doforbodypartswithparam<ulong>()(this, &bodypart::CalculateMaxHP, Flags);
  CalculateMaxHP();
  CalculateHP();
}

truth character::EditAttribute(int Identifier, int Value)
{
  if(Identifier == ENDURANCE && UseMaterialAttributes())
    return false;

  if(RawEditAttribute(BaseExperience[Identifier], Value))
  {
    if(!IsInitializing())
    {
      if(Identifier == LEG_STRENGTH)
        CalculateBurdenState();
      else if(Identifier == ENDURANCE)
        CalculateBodyPartMaxHPs();
      else if(IsPlayer() && Identifier == PERCEPTION)
        game::SendLOSUpdateRequest();
      else if(IsPlayerKind() && (Identifier == INTELLIGENCE || Identifier == WISDOM || Identifier == CHARISMA))
        UpdatePictures();

      CalculateBattleInfo();
    }

    return true;
  }
  else
    return false;
}

truth character::ActivateRandomState(int Flags, int Time, long Seed)
{
  femath::SaveSeed();

  if(Seed)
    femath::SetSeed(Seed);

  long ToBeActivated = GetRandomState(Flags|DUR_TEMPORARY);
  femath::LoadSeed();

  if(!ToBeActivated)
    return false;

  BeginTemporaryState(ToBeActivated, Time);
  return true;
}

truth character::GainRandomIntrinsic(int Flags)
{
  long ToBeActivated = GetRandomState(Flags|DUR_PERMANENT);

  if(!ToBeActivated)
    return false;

  GainIntrinsic(ToBeActivated);
  return true;
}

/* Returns 0 if state not found */

long character::GetRandomState(int Flags) const
{
  long OKStates[STATES];
  int NumberOfOKStates = 0;

  for(int c = 0; c < STATES; ++c)
    if(StateData[c].Flags & Flags & DUR_FLAGS && StateData[c].Flags & Flags & SRC_FLAGS)
      OKStates[NumberOfOKStates++] = 1 << c;

  return NumberOfOKStates ? OKStates[RAND() % NumberOfOKStates] : 0;
}

int characterprototype::CreateSpecialConfigurations(characterdatabase** TempConfig, int Configs, int Level)
{
  if(Level == 0 && TempConfig[0]->CreateDivineConfigurations)
    Configs = databasecreator<character>::CreateDivineConfigurations(this, TempConfig, Configs);

  if(Level == 1 && TempConfig[0]->CreateUndeadConfigurations)
    for(int c = 1; c < protocontainer<character>::GetSize(); ++c)
    {
      const character::prototype* Proto = protocontainer<character>::GetProto(c);
      const character::database*const* CharacterConfigData = Proto->GetConfigData();
      const character::database*const* End = CharacterConfigData + Proto->GetConfigSize();

      for(++CharacterConfigData; CharacterConfigData != End; ++CharacterConfigData)
      {
        const character::database* CharacterDataBase = *CharacterConfigData;

        if(CharacterDataBase->UndeadVersions)
        {
          character::database* ConfigDataBase = new character::database(**TempConfig);
          ConfigDataBase->InitDefaults(this, (c << 8) | CharacterDataBase->Config);
          ConfigDataBase->PostFix << "of ";

          if(CharacterDataBase->Adjective.GetSize())
          {
            if(CharacterDataBase->UsesLongAdjectiveArticle)
              ConfigDataBase->PostFix << "an ";
            else
              ConfigDataBase->PostFix << "a ";

            ConfigDataBase->PostFix << CharacterDataBase->Adjective << ' ';
          }
          else
          {
            if(CharacterDataBase->UsesLongArticle)
              ConfigDataBase->PostFix << "an ";
            else
              ConfigDataBase->PostFix << "a ";
          }

          ConfigDataBase->PostFix << CharacterDataBase->NameSingular;

          if(CharacterDataBase->PostFix.GetSize())
            ConfigDataBase->PostFix << ' ' << CharacterDataBase->PostFix;

          int P1 = TempConfig[0]->UndeadAttributeModifier;
          int P2 = TempConfig[0]->UndeadVolumeModifier;
          int c2;

          for(c2 = 0; c2 < ATTRIBUTES; ++c2)
              ConfigDataBase->*ExpPtr[c2] = CharacterDataBase->*ExpPtr[c2] * P1 / 100;

          for(c2 = 0; c2 < EQUIPMENT_DATAS; ++c2)
              ConfigDataBase->*EquipmentDataPtr[c2] = contentscript<item>();

          ConfigDataBase->DefaultIntelligence = 5;
          ConfigDataBase->DefaultWisdom = 5;
          ConfigDataBase->DefaultCharisma = 5;
          ConfigDataBase->TotalSize = CharacterDataBase->TotalSize;
          ConfigDataBase->Sex = CharacterDataBase->Sex;
          ConfigDataBase->AttributeBonus = CharacterDataBase->AttributeBonus;
          ConfigDataBase->TotalVolume = CharacterDataBase->TotalVolume * P2 / 100;

          if(TempConfig[0]->UndeadCopyMaterials)
          {
            ConfigDataBase->HeadBitmapPos = CharacterDataBase->HeadBitmapPos;
            ConfigDataBase->HairColor = CharacterDataBase->HairColor;
            ConfigDataBase->EyeColor = CharacterDataBase->EyeColor;
            ConfigDataBase->CapColor = CharacterDataBase->CapColor;
            ConfigDataBase->FleshMaterial = CharacterDataBase->FleshMaterial;
            ConfigDataBase->BloodMaterial = CharacterDataBase->BloodMaterial;
            ConfigDataBase->VomitMaterial = CharacterDataBase->VomitMaterial;
            ConfigDataBase->SweatMaterial = CharacterDataBase->SweatMaterial;
          }

          if(TempConfig[0]->GhostCopyMaterials)
          {
            ConfigDataBase->HeadBitmapPos = CharacterDataBase->HeadBitmapPos;
            ConfigDataBase->RightArmBitmapPos = CharacterDataBase->RightArmBitmapPos;
            ConfigDataBase->LeftArmBitmapPos = CharacterDataBase->LeftArmBitmapPos;
            ConfigDataBase->TorsoBitmapPos = CharacterDataBase->TorsoBitmapPos;
            ConfigDataBase->RightLegBitmapPos = CharacterDataBase->RightLegBitmapPos;
            ConfigDataBase->LeftLegBitmapPos = CharacterDataBase->LeftLegBitmapPos;
            ConfigDataBase->HairColor = CharacterDataBase->HairColor;
            ConfigDataBase->EyeColor = CharacterDataBase->EyeColor;
          }

          ConfigDataBase->KnownCWeaponSkills = CharacterDataBase->KnownCWeaponSkills;
          ConfigDataBase->CWeaponSkillHits = CharacterDataBase->CWeaponSkillHits;
          ConfigDataBase->PostProcess();
          TempConfig[Configs++] = ConfigDataBase;
        }
      }
    }

  if(Level == 0 && TempConfig[0]->CreateGolemMaterialConfigurations)
    for(int c = 1; c < protocontainer<material>::GetSize(); ++c)
    {
      const material::prototype* Proto = protocontainer<material>::GetProto(c);
      const material::database*const* MaterialConfigData = Proto->GetConfigData();
      const material::database*const* End = MaterialConfigData + Proto->GetConfigSize();

      for(++MaterialConfigData; MaterialConfigData != End; ++MaterialConfigData)
      {
        const material::database* MaterialDataBase = *MaterialConfigData;

        if(MaterialDataBase->CategoryFlags & IS_GOLEM_MATERIAL)
        {
          character::database* ConfigDataBase = new character::database(**TempConfig);
          ConfigDataBase->InitDefaults(this, MaterialDataBase->Config);
          ConfigDataBase->Adjective = MaterialDataBase->NameStem;
          ConfigDataBase->UsesLongAdjectiveArticle = MaterialDataBase->NameFlags & USE_AN;
          ConfigDataBase->AttachedGod = MaterialDataBase->AttachedGod;
          TempConfig[Configs++] = ConfigDataBase;
        }
      }
    }

  return Configs;
}

double character::GetTimeToDie(ccharacter* Enemy, int Damage, double ToHitValue,
                               truth AttackIsBlockable, truth UseMaxHP) const
{
  double DodgeValue = GetDodgeValue();

  if(!Enemy->CanBeSeenBy(this, true))
    ToHitValue *= 2;

  if(!CanBeSeenBy(Enemy, true))
    DodgeValue *= 2;

  double MinHits = 1000;
  truth First = true;

  for(int c = 0; c < BodyParts; ++c)
    if(BodyPartIsVital(c) && GetBodyPart(c))
    {
      double Hits = GetBodyPart(c)->GetTimeToDie(Damage, ToHitValue, DodgeValue, AttackIsBlockable, UseMaxHP);

      if(First)
      {
        MinHits = Hits;
        First = false;
      }
      else
        MinHits = 1 / (1 / MinHits + 1 / Hits);
    }

  return MinHits;
}

double character::GetRelativeDanger(ccharacter* Enemy, truth UseMaxHP) const
{
  double Danger = Enemy->GetTimeToKill(this, UseMaxHP) / GetTimeToKill(Enemy, UseMaxHP);
  int EnemyAP = Enemy->GetMoveAPRequirement(1);
  int ThisAP = GetMoveAPRequirement(1);

  if(EnemyAP > ThisAP)
    Danger *= 1.25;
  else if(ThisAP > EnemyAP)
    Danger *= 0.80;

  if(!Enemy->CanBeSeenBy(this, true))
    Danger *= Enemy->IsPlayer() ? 0.2 : 0.5;

  if(!CanBeSeenBy(Enemy, true))
    Danger *= IsPlayer() ? 5. : 2.;

  if(GetAttribute(INTELLIGENCE) < 10 && !IsPlayer())
    Danger *= 0.80;

  if(Enemy->GetAttribute(INTELLIGENCE) < 10 && !Enemy->IsPlayer())
    Danger *= 1.25;

  return Limit(Danger, 0.001, 1000.);
}

festring character::GetBodyPartName(int I, truth Articled) const
{
  if(I == TORSO_INDEX)
    return Articled ? CONST_S("a torso") : CONST_S("torso");
  else
  {
    ABORT("Illegal character bodypart name request!");
    return "";
  }
}

item* character::SearchForItem(ulong ID) const
{
  item* Equipment = findequipment<ulong>()(this, &item::HasID, ID);

  if(Equipment)
    return Equipment;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->GetID() == ID)
      return *i;

  return 0;
}

truth character::ContentsCanBeSeenBy(ccharacter* Viewer) const
{
  return Viewer == this;
}

truth character::HitEffect(character* Enemy, item* Weapon, v2 HitPos, int Type,
                           int BodyPartIndex, int Direction, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(Weapon)
    return Weapon->HitEffect(this, Enemy, HitPos, BodyPartIndex, Direction, BlockedByArmour);

  switch(Type)
  {
   case UNARMED_ATTACK:
    return Enemy->SpecialUnarmedEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour);
   case KICK_ATTACK:
    return Enemy->SpecialKickEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour);
   case BITE_ATTACK:
    return Enemy->SpecialBiteEffect(this, HitPos, BodyPartIndex, Direction, BlockedByArmour, Critical, DoneDamage);
  }

  return false;
}

void character::WeaponSkillHit(item* Weapon, int Type, int Hits)
{
  int Category;

  switch(Type)
  {
   case UNARMED_ATTACK:
    Category = UNARMED;
    break;
   case WEAPON_ATTACK:
    Weapon->WeaponSkillHit(Hits);
    return;
   case KICK_ATTACK:
    Category = KICK;
    break;
   case BITE_ATTACK:
    Category = BITE;
    break;
   case THROW_ATTACK:
    if(!IsHumanoid()) return;
    Category = Weapon->GetWeaponCategory();
    break;
   default:
    ABORT("Illegal Type %d passed to character::WeaponSkillHit()!", Type);
    return;
  }

  if(GetCWeaponSkill(Category)->AddHit(Hits))
  {
    CalculateBattleInfo();

    if(IsPlayer())
      GetCWeaponSkill(Category)->AddLevelUpMessage(Category);
  }
}

/* Returns 0 if character cannot be duplicated */

character* character::Duplicate(ulong Flags)
{
  if(!(Flags & IGNORE_PROHIBITIONS) && !CanBeCloned())
    return 0;

  character* Char = GetProtoType()->Clone(this);

  if(Flags & MIRROR_IMAGE)
  {
    DuplicateEquipment(Char, Flags & ~IGNORE_PROHIBITIONS);
    Char->SetLifeExpectancy(Flags >> LE_BASE_SHIFT & LE_BASE_RANGE,
                            Flags >> LE_RAND_SHIFT & LE_RAND_RANGE);
  }

  Char->CalculateAll();
  Char->CalculateEmitation();
  Char->UpdatePictures();
  Char->Flags &= ~(C_INITIALIZING|C_IN_NO_MSG_MODE);
  return Char;
}

truth character::TryToEquip(item* Item, truth onlyIfEmpty, int onlyAt)
{
  if(!Item->AllowEquip()
     || !CanUseEquipment()
     || GetAttribute(WISDOM) >= Item->GetWearWisdomLimit()
     || Item->GetSquaresUnder() != 1)
    return false;

  for(int e = 0; e < GetEquipments(); ++e){
    if(onlyAt>-1 && e!=onlyAt)continue;

    if(GetBodyPartOfEquipment(e) && EquipmentIsAllowed(e))
    {
      sorter Sorter = EquipmentSorter(e);

      if((Sorter == 0 || (Item->*Sorter)(this))
         && ((e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX)
             || Item->IsWeapon(this)
             || Item->IsShield(this))
         && AllowEquipment(Item, e))
      {
        item* OldEquipment = GetEquipment(e);
        if(onlyIfEmpty && OldEquipment!=NULL)continue;

        if(BoundToUse(OldEquipment, e))
          continue;

        lsquare* LSquareUnder = GetLSquareUnder();
        stack* StackUnder = LSquareUnder->GetStack();
        msgsystem::DisableMessages();
        Flags |= C_PICTURE_UPDATES_FORBIDDEN;
        LSquareUnder->Freeze();
        StackUnder->Freeze();
        double Danger = GetRelativeDanger(PLAYER);

        if(OldEquipment)
          OldEquipment->RemoveFromSlot();

        Item->RemoveFromSlot();
        SetEquipment(e, Item);
        double NewDanger = GetRelativeDanger(PLAYER);
        Item->RemoveFromSlot();
        StackUnder->AddItem(Item);

        if(OldEquipment)
          SetEquipment(e, OldEquipment);

        msgsystem::EnableMessages();
        Flags &= ~C_PICTURE_UPDATES_FORBIDDEN;
        LSquareUnder->UnFreeze();
        StackUnder->UnFreeze();

        if(OldEquipment)
        {
          if(NewDanger > Danger || BoundToUse(Item, e))
          {
            room* Room = GetRoom();

            if(!Room || Room->PickupItem(this, Item, 1))
            {
              if(CanBeSeenByPlayer())
                ADD_MESSAGE("%s drops %s %s and equips %s instead.",
                            CHAR_NAME(DEFINITE), CHAR_POSSESSIVE_PRONOUN,
                            OldEquipment->CHAR_NAME(UNARTICLED), Item->CHAR_NAME(INDEFINITE));

              if(Room)
                Room->DropItem(this, OldEquipment, 1);

              OldEquipment->MoveTo(StackUnder);
              Item->RemoveFromSlot();
              SetEquipment(e, Item);
              DexterityAction(5);
              return true;
            }
          }
        }
        else
        {
          if(NewDanger > Danger
             || (NewDanger == Danger
                 && e != RIGHT_WIELDED_INDEX && e != LEFT_WIELDED_INDEX)
             || BoundToUse(Item, e))
          {
            room* Room = GetRoom();

            if(!Room || Room->PickupItem(this, Item, 1))
            {
              if(CanBeSeenByPlayer())
                ADD_MESSAGE("%s picks up and equips %s.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(INDEFINITE));

              Item->RemoveFromSlot();
              SetEquipment(e, Item);
              DexterityAction(5);
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

truth character::TryToConsume(item* Item)
{
  return Item->CanBeEatenByAI(this) && ConsumeItem(Item, Item->GetConsumeMaterial(this)->GetConsumeVerb());
}

void character::UpdateESPLOS() const
{
  if(StateIsActivated(ESP) && !game::IsInWilderness())
    for(int c = 0; c < game::GetTeams(); ++c)
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
          p->SendNewDrawRequest();
}

int character::GetCWeaponSkillLevel(citem* Item) const
{
  if(Item->GetWeaponCategory() < GetAllowedWeaponSkillCategories())
    return GetCWeaponSkill(Item->GetWeaponCategory())->GetLevel();
  else
    return 0;
}

void character::PrintBeginPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You panic!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s panics.", CHAR_NAME(DEFINITE));
}

void character::PrintEndPanicMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You finally calm down.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s calms down.", CHAR_NAME(DEFINITE));
}

void character::CheckPanic(int Ticks)
{
  if(GetPanicLevel() > 1 && !StateIsActivated(PANIC)
     && GetHP() * 100 < RAND() % (GetPanicLevel() * GetMaxHP() << 1) && !StateIsActivated(FEARLESS))
    BeginTemporaryState(PANIC, ((Ticks * 3) >> 2) + RAND() % ((Ticks >> 1) + 1)); // 25% randomness to ticks...
}

/* returns 0 if fails else the newly created character */

character* character::DuplicateToNearestSquare(character* Cloner, ulong Flags)
{
  character* NewlyCreated = Duplicate(Flags);

  if(!NewlyCreated)
    return 0;

  if(Flags & CHANGE_TEAM && Cloner)
    NewlyCreated->ChangeTeam(Cloner->GetTeam());

  NewlyCreated->PutNear(GetPos());
  return NewlyCreated;
}

void character::SignalSpoil()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoil(0);
  else
    Disappear(0, "spoil", &item::IsVeryCloseToSpoiling);
}
// never call this function!
void character::SignalBurn()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalBurn(0);
  else
    Disappear(0, "burn", &item::IsVeryCloseToBurning);
}

void character::Extinguish(truth SendMessages)
{
  doforbodypartswithparam<truth>()(this, &bodypart::Extinguish, SendMessages);
}

truth character::CanHeal() const
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < BodyPart->GetMaxHP())
      return true;
  }

  return false;
}

int character::GetRelation(ccharacter* Who) const
{
  return GetTeam()->GetRelation(Who->GetTeam());
}

truth (item::*AffectTest[BASE_ATTRIBUTES])() const =
{
  &item::AffectsEndurance,
  &item::AffectsPerception,
  &item::AffectsIntelligence,
  &item::AffectsWisdom,
  &item::AffectsWillPower,
  &item::AffectsCharisma,
  &item::AffectsMana
};

/* Returns nonzero if endurance has decreased and death may occur */

truth character::CalculateAttributeBonuses()
{
  doforbodyparts()(this, &bodypart::CalculateAttributeBonuses);
  int BackupBonus[BASE_ATTRIBUTES];
  int BackupCarryingBonus = CarryingBonus;
  CarryingBonus = 0;
  int c1;

  for(c1 = 0; c1 < BASE_ATTRIBUTES; ++c1)
  {
    BackupBonus[c1] = AttributeBonus[c1];
    AttributeBonus[c1] = 0;
  }

  for(c1 = 0; c1 < GetEquipments(); ++c1)
  {
    item* Equipment = GetEquipment(c1);

    if(!Equipment || !Equipment->IsInCorrectSlot(c1))
      continue;

    for(int c2 = 0; c2 < BASE_ATTRIBUTES; ++c2)
      if((Equipment->*AffectTest[c2])())
        AttributeBonus[c2] += Equipment->GetEnchantment();

    if(Equipment->AffectsCarryingCapacity())
      CarryingBonus += Equipment->GetCarryingBonus();
  }

  ApplySpecialAttributeBonuses();

  if(IsPlayer() && !IsInitializing() && AttributeBonus[PERCEPTION] != BackupBonus[PERCEPTION])
    game::SendLOSUpdateRequest();

  if(IsPlayer() && !IsInitializing() && AttributeBonus[INTELLIGENCE] != BackupBonus[INTELLIGENCE])
    UpdateESPLOS();

  if(!IsInitializing() && CarryingBonus != BackupCarryingBonus)
    CalculateBurdenState();

  if(!IsInitializing() && AttributeBonus[ENDURANCE] != BackupBonus[ENDURANCE])
  {
    CalculateBodyPartMaxHPs();
    CalculateMaxStamina();
    return AttributeBonus[ENDURANCE] < BackupBonus[ENDURANCE];
  }

  return false;
}

void character::ApplyEquipmentAttributeBonuses(item* Equipment)
{
  if(Equipment->AffectsEndurance())
  {
    AttributeBonus[ENDURANCE] += Equipment->GetEnchantment();
    CalculateBodyPartMaxHPs();
    CalculateMaxStamina();
  }

  if(Equipment->AffectsPerception())
  {
    AttributeBonus[PERCEPTION] += Equipment->GetEnchantment();

    if(IsPlayer())
      game::SendLOSUpdateRequest();
  }

  if(Equipment->AffectsIntelligence())
  {
    AttributeBonus[INTELLIGENCE] += Equipment->GetEnchantment();

    if(IsPlayer())
      UpdateESPLOS();
  }

  if(Equipment->AffectsWisdom())
    AttributeBonus[WISDOM] += Equipment->GetEnchantment();

  if(Equipment->AffectsWillPower())
    AttributeBonus[WILL_POWER] += Equipment->GetEnchantment();

  if(Equipment->AffectsCharisma())
    AttributeBonus[CHARISMA] += Equipment->GetEnchantment();

  if(Equipment->AffectsMana())
    AttributeBonus[MANA] += Equipment->GetEnchantment();

  if(Equipment->AffectsCarryingCapacity())
  {
    CarryingBonus += Equipment->GetCarryingBonus();
    CalculateBurdenState();
  }
}

void character::ReceiveAntidote(long Amount)
{
  if(StateIsActivated(POISONED))
  {
    if(GetTemporaryStateCounter(POISONED) > Amount)
    {
      EditTemporaryStateCounter(POISONED, -Amount);
      Amount = 0;
    }
    else
    {
      if(IsPlayer())
        ADD_MESSAGE("Aaaah... You feel much better.");

      Amount -= GetTemporaryStateCounter(POISONED);
      DeActivateTemporaryState(POISONED);
    }
  }

  if((Amount > 500 || RAND_N(1000) < Amount) && StateIsActivated(PARASITE_TAPE_WORM))
  {
    if(IsPlayer())
      ADD_MESSAGE("Something in your belly didn't seem to like this stuff.");

    DeActivateTemporaryState(PARASITE_TAPE_WORM);
    Amount -= Min(100L, Amount);
  }

  if((Amount > 500 || RAND_N(1000) < Amount) && StateIsActivated(PARASITE_MIND_WORM))
  {
    if(IsPlayer())
      ADD_MESSAGE("Something in your head screeches in pain.");

    DeActivateTemporaryState(PARASITE_MIND_WORM);
    Amount -= Min(100L, Amount);
  }

  if((Amount > 500 || RAND_N(1000) < Amount) && StateIsActivated(LEPROSY))
  {
    if(IsPlayer())
      ADD_MESSAGE("You are not falling to pieces anymore.");

    DeActivateTemporaryState(LEPROSY);
    Amount -= Min(100L, Amount);
  }
}

void character::AddAntidoteConsumeEndMessage() const
{
  if(StateIsActivated(POISONED)) // true only if the antidote didn't cure the poison completely
  {
    if(IsPlayer())
      ADD_MESSAGE("Your body processes the poison in your veins with rapid speed.");
  }
}

truth character::IsDead() const
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPartIsVital(c) && (!BodyPart || BodyPart->GetHP() < 1))
      return true;
  }

  return false;
}

void character::SignalSpoilLevelChange()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalSpoilLevelChange(0);
  else
    UpdatePictures();
}

void character::SignalBurnLevelChange()
{
//  if(GetMotherEntity())
//    GetMotherEntity()->SignalBurnLevelChange();
//  else
    UpdatePictures();
}

void character::AddOriginalBodyPartID(int I, ulong What)
{
  if(std::find(OriginalBodyPartID[I].begin(), OriginalBodyPartID[I].end(), What) == OriginalBodyPartID[I].end())
  {
    OriginalBodyPartID[I].push_back(What);

    if(OriginalBodyPartID[I].size() > 100)
      OriginalBodyPartID[I].erase(OriginalBodyPartID[I].begin());
  }
}

void character::AddToInventory(const fearray<contentscript<item>>& ItemArray, int SpecialFlags)
{
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
          Stack->AddItem(Item);
          Item->SpecialGenerationHandler();
        }
      }
    }
}

truth character::HasHadBodyPart(citem* Item) const
{
  for(int c = 0; c < BodyParts; ++c)
    if(std::find(OriginalBodyPartID[c].begin(),
                 OriginalBodyPartID[c].end(),
                 Item->GetID()) != OriginalBodyPartID[c].end())
      return true;

  return GetPolymorphBackup() && GetPolymorphBackup()->HasHadBodyPart(Item);
}

festring& character::ProcessMessage(festring& Msg) const
{
  SEARCH_N_REPLACE(Msg, "@nu", GetName(UNARTICLED));
  SEARCH_N_REPLACE(Msg, "@ni", GetName(INDEFINITE));
  SEARCH_N_REPLACE(Msg, "@nd", GetName(DEFINITE));
  SEARCH_N_REPLACE(Msg, "@du", GetDescription(UNARTICLED));
  SEARCH_N_REPLACE(Msg, "@di", GetDescription(INDEFINITE));
  SEARCH_N_REPLACE(Msg, "@dd", GetDescription(DEFINITE));
  SEARCH_N_REPLACE(Msg, "@pp", GetPersonalPronoun());
  SEARCH_N_REPLACE(Msg, "@sp", GetPossessivePronoun());
  SEARCH_N_REPLACE(Msg, "@op", GetObjectPronoun());
  SEARCH_N_REPLACE(Msg, "@Nu", GetName(UNARTICLED).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Ni", GetName(INDEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Nd", GetName(DEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Du", GetDescription(UNARTICLED).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Di", GetDescription(INDEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Dd", GetDescription(DEFINITE).CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Pp", GetPersonalPronoun().CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Sp", GetPossessivePronoun().CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Op", GetObjectPronoun().CapitalizeCopy());
  SEARCH_N_REPLACE(Msg, "@Gd", GetMasterGod()->GetName());
  return Msg;
}

void character::ProcessAndAddMessage(festring Msg) const
{
  ADD_MESSAGE("%s", ProcessMessage(Msg).CStr());
}

void character::BeTalkedTo()
{
  static long Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size)]);
}

truth character::CheckZap()
{
  if(!CanZap())
  {
    ADD_MESSAGE("This monster type can't zap.");
    return false;
  }

  return true;
}

void character::DamageAllItems(character* Damager, int Damage, int Type)
{
  GetStack()->ReceiveDamage(Damager, Damage, Type);

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
      Equipment->ReceiveDamage(Damager, Damage, Type);
  }
}

truth character::Equips(citem* Item) const
{
  return combineequipmentpredicateswithparam<ulong>()(this, &item::HasID, Item->GetID(), 1);
}

void character::PrintBeginConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel quite happy.");
}

void character::PrintEndConfuseMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("The world is boring again.");
}

v2 character::ApplyStateModification(v2 TryDirection) const
{
  if(!StateIsActivated(CONFUSED) || RAND() & 15 || game::IsInWilderness())
    return TryDirection;
  else
  {
    v2 To = GetLevel()->GetFreeAdjacentSquare(this, GetPos(), true);

    if(To == ERROR_V2)
      return TryDirection;
    else
    {
      To -= GetPos();

      if(To != TryDirection && IsPlayer())
        ADD_MESSAGE("Whoa! You somehow don't manage to walk straight.");

      return To;
    }
  }
}

void character::AddConfuseHitMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("This stuff is confusing.");
}

item* character::SelectFromPossessions(cfestring& Topic, sorter Sorter)
{
  itemvector ReturnVector;
  SelectFromPossessions(ReturnVector, Topic, NO_MULTI_SELECT, Sorter);
  return !ReturnVector.empty() ? ReturnVector[0] : 0;
}

truth character::SelectFromPossessions(itemvector& ReturnVector, cfestring& Topic, int Flags, sorter Sorter)
{
  felist List(Topic);
  truth InventoryPossible = GetStack()->SortedItems(this, Sorter);

  if(InventoryPossible)
    List.AddEntry(CONST_S("choose from inventory"), LIGHT_GRAY, 20, game::AddToItemDrawVector(itemvector()));

  truth Any = false;
  itemvector Item;
  festring Entry;
  int c;

  for(c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && (Sorter == 0 || (BodyPart->*Sorter)(this)))
    {
      Item.push_back(BodyPart);
      Entry.Empty();
      BodyPart->AddName(Entry, UNARTICLED);
      int ImageKey = game::AddToItemDrawVector(itemvector(1, BodyPart));
      List.AddEntry(Entry, LIGHT_GRAY, 20, ImageKey, true);
      Any = true;
    }
  }

  for(c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && (Sorter == 0 || (Equipment->*Sorter)(this)))
    {
      Item.push_back(Equipment);
      Entry = GetEquipmentName(c);
      Entry << ':';
      Entry.Resize(20);
      Equipment->AddInventoryEntry(this, Entry, 1, true);
      AddSpecialEquipmentInfo(Entry, c);
      int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
      List.AddEntry(Entry, LIGHT_GRAY, 20, ImageKey, true);
      Any = true;
    }
  }

  if(Any)
  {
    game::SetStandardListAttributes(List);
    List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
    List.SetEntryDrawer(game::ItemEntryDrawer);
    game::DrawEverythingNoBlit();
    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true);
    int Chosen = List.Draw();
    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);
    game::ClearItemDrawVector();

    if(Chosen != ESCAPED)
    {
      if((InventoryPossible && !Chosen) || Chosen & FELIST_ERROR_BIT)
        GetStack()->DrawContents(ReturnVector, this, Topic, Flags, Sorter);
      else
      {
        ReturnVector.push_back(Item[InventoryPossible ? Chosen - 1 : Chosen]);

        if(Flags & SELECT_PAIR && ReturnVector[0]->HandleInPairs())
        {
          item* PairEquipment = GetPairEquipment(ReturnVector[0]->GetEquipmentIndex());

          if(PairEquipment && PairEquipment->CanBePiledWith(ReturnVector[0], this))
            ReturnVector.push_back(PairEquipment);
        }
      }
    }
  }
  else
  {
    if(!GetStack()->SortedItems(this, Sorter))
      return false;

    game::ClearItemDrawVector();
    GetStack()->DrawContents(ReturnVector, this, Topic, Flags, Sorter);
  }

  return true;
}

truth character::EquipsSomething(sorter Sorter)
{
  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && (Sorter == 0 || (Equipment->*Sorter)(this)))
      return true;
  }

  return false;
}

material* character::CreateBodyPartMaterial(int, long Volume) const
{
  return MAKE_MATERIAL(GetFleshMaterial(), Volume);
}

truth character::CheckTalk()
{
  if(!CanTalk())
  {
    ADD_MESSAGE("This monster does not know the art of talking.");
    return false;
  }

  return true;
}

truth character::MoveTowardsHomePos()
{
  if(HomeDataIsValid() && IsEnabled())
  {
    SetGoingTo(HomeData->Pos);
    return MoveTowardsTarget(false)
      || (!GetPos().IsAdjacent(HomeData->Pos) && MoveRandomly());
  }
  else
    return false;
}

truth character::TryToChangeEquipment(stack* MainStack, stack* SecStack, int Chosen)
{
  if(!GetBodyPartOfEquipment(Chosen))
  {
    ADD_MESSAGE("Bodypart missing!");
    return false;
  }

  item* OldEquipment = GetEquipment(Chosen);

  if(!IsPlayer() && BoundToUse(OldEquipment, Chosen))
  {
    ADD_MESSAGE("%s refuses to unequip %s.", CHAR_DESCRIPTION(DEFINITE), OldEquipment->CHAR_NAME(DEFINITE));
    return false;
  }

  if(OldEquipment)
    OldEquipment->MoveTo(MainStack);

  sorter Sorter = EquipmentSorter(Chosen);

  if(!MainStack->SortedItems(this, Sorter)
     && (!SecStack || !SecStack->SortedItems(this, Sorter)))
  {
    ADD_MESSAGE("You haven't got any item that could be used for this purpose.");
    return false;
  }
  else
  {
    game::DrawEverythingNoBlit();
    itemvector ItemVector;
    int Return = MainStack->DrawContents(ItemVector,
                                         SecStack,
                                         this,
                                         CONST_S("Choose ") + GetEquipmentName(Chosen) + ':',
                                         SecStack ? CONST_S("Items in your inventory") : CONST_S(""),
                                         SecStack ? festring(CONST_S("Items in ") + GetPossessivePronoun()
                                                             + " inventory") : CONST_S(""),
                                         SecStack ? festring(GetDescription(DEFINITE) + " is "
                                                             + GetVerbalBurdenState()) : CONST_S(""),
                                         GetVerbalBurdenStateColor(),
                                         NONE_AS_CHOICE|NO_MULTI_SELECT,
                                         Sorter);

    if(Return == ESCAPED)
    {
      if(OldEquipment)
      {
        OldEquipment->RemoveFromSlot();
        SetEquipment(Chosen, OldEquipment);
      }

      return false;
    }

    item* Item = ItemVector.empty() ? 0 : ItemVector[0];

    if(Item)
    {
      if(!IsPlayer() && !AllowEquipment(Item, Chosen))
      {
        ADD_MESSAGE("%s refuses to equip %s.", CHAR_DESCRIPTION(DEFINITE), Item->CHAR_NAME(DEFINITE));
        return false;
      }

      Item->RemoveFromSlot();
      SetEquipment(Chosen, Item);

      if(CheckIfEquipmentIsNotUsable(Chosen))
        Item->MoveTo(MainStack); // small bug?
    }

    return Item != OldEquipment;
  }
}

void character::PrintBeginParasitizedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you are no longer alone.");
}

void character::PrintEndParasitizedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("A feeling of long welcome emptiness overwhelms you.");
}

void character::ParasitizedHandler()
{
  EditNP(-10);

  if(!(RAND() % 250))
  {
    if(IsPlayer())
      ADD_MESSAGE("Ugh. You feel something violently carving its way through your intestines.");

    ReceiveDamage(0, 1, DRAIN, TORSO, 8, false, false, false, false); // Use DRAIN here so that resistances do not apply.
    CheckDeath(CONST_S("killed by a vile parasite"), 0);
  }
}

truth character::CanFollow() const
{
  return CanMove() && !StateIsActivated(PANIC) && !IsStuck();
}

festring character::GetKillName() const
{
  if(!GetPolymorphBackup())
    return GetName(INDEFINITE);
  else
  {
    festring KillName;
    GetPolymorphBackup()->AddName(KillName, INDEFINITE);
    KillName << " polymorphed into ";
    id::AddName(KillName, INDEFINITE);
    return KillName;
  }
}

festring character::GetPanelName() const
{
  festring Name;
  Name << AssignedName << " the " << game::GetVerbalPlayerAlignment() << ' ';
  id::AddName(Name, UNARTICLED);

  festring PanelName;
  if(!game::IsInWilderness()){
    PanelName << Name;
    if(ivanconfig::IsShowFullDungeonName()){
      PanelName << " (at " << game::GetCurrentDungeon()->GetLevelDescription(game::GetCurrentLevelIndex(), true) << ')';
    }
  }

  return PanelName;
}

long character::GetMoveAPRequirement(int Difficulty) const
{
  return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty
         / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
}

bodypart* character::HealHitPoint()
{
  int NeedHeal = 0, NeedHealIndex[MAX_BODYPARTS];

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetHP() < BodyPart->GetMaxHP())
      NeedHealIndex[NeedHeal++] = c;
  }

  if(NeedHeal)
  {
    bodypart* BodyPart = GetBodyPart(NeedHealIndex[RAND() % NeedHeal]);
    BodyPart->IncreaseHP();
    ++HP;
    return BodyPart;
  }
  else
    return 0;
}

void character::HealBurntBodyParts(long Amount)
{
  if(!Amount)
    return;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(Amount > 0)
    {
      if(BodyPart && BodyPart->CanRegenerate() && BodyPart->IsBurnt())
      {
        if(IsPlayer())
          ADD_MESSAGE("Your %s is healed of its burns.", BodyPart->GetBodyPartName().CStr());
        else if(CanBeSeenByPlayer())
          ADD_MESSAGE("The %s of %s is healed of its burns.", BodyPart->GetBodyPartName().CStr(),
                      CHAR_NAME(DEFINITE));

        BodyPart->GetMainMaterial()->ResetBurning();
        Amount = Amount - 250;
      }
    }
    else
      break;
  }
  return;
}

void character::CreateHomeData()
{
  HomeData = new homedata;
  lsquare* Square = GetLSquareUnder();
  HomeData->Pos = Square->GetPos();
  HomeData->Dungeon = Square->GetDungeonIndex();
  HomeData->Level = Square->GetLevelIndex();
  HomeData->Room = Square->GetRoomIndex();
}

room* character::GetHomeRoom() const
{
  if(HomeDataIsValid() && HomeData->Room)
    return GetLevel()->GetRoom(HomeData->Room);
  else
    return 0;
}

void character::RemoveHomeData()
{
  delete HomeData;
  HomeData = 0;
}

void character::AddESPConsumeMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a strange mental activity.");
}

void character::SetBodyPart(int I, bodypart* What)
{
  BodyPartSlot[I].PutInItem(What);

  if(What)
  {
    What->SignalPossibleUsabilityChange();
    What->Disable();
    AddOriginalBodyPartID(I, What->GetID());

    if(What->GetMainMaterial()->IsInfectedByLeprosy())
      GainIntrinsic(LEPROSY);
    else if(StateIsActivated(LEPROSY))
      What->GetMainMaterial()->SetIsInfectedByLeprosy(true);
  }
}

truth character::ConsumeItem(item* Item, cfestring& ConsumeVerb, truth nibbling)
{
  if(IsPlayer()
     && HasHadBodyPart(Item)
     && !game::TruthQuestion(CONST_S("Are you sure? You may be able to put it back... [y/N]")))
    return false;

  if(Item->IsOnGround() && GetRoom() && !GetRoom()->ConsumeItem(this, Item, 1))
    return false;

  if(IsPlayer())
    ADD_MESSAGE("You begin %s %s.", ConsumeVerb.CStr(), Item->CHAR_NAME(DEFINITE));
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s begins %s %s.", CHAR_NAME(DEFINITE), ConsumeVerb.CStr(), Item->CHAR_NAME(DEFINITE));

  consume* Consume = consume::Spawn(this);
  Consume->SetDescription(ConsumeVerb);
  Consume->SetConsumingID(Item->GetID());
  Consume->SetNibbling(nibbling);
  SetAction(Consume);
  DexterityAction(5);
  return true;
}

truth character::CheckThrow() const
{
  if(!CanThrow())
  {
    ADD_MESSAGE("This monster type cannot throw.");
    return false;
  }

  return true;
}

void character::GetHitByExplosion(const explosion* Explosion, int Damage)
{
  int DamageDirection = GetPos() == Explosion->Pos ?
                        RANDOM_DIR : game::CalculateRoughDirection(GetPos() - Explosion->Pos);

  if(!IsPet() && Explosion->Terrorist && Explosion->Terrorist->IsPet())
    Explosion->Terrorist->Hostility(this);

  if(!Explosion->FireOnly)
    GetTorso()->SpillBlood((8 - Explosion->Size + RAND() % (8 - Explosion->Size)) >> 1);

  if(DamageDirection == RANDOM_DIR)
    DamageDirection = RAND() & 7;

  v2 SpillPos = GetPos() + game::GetMoveVector(DamageDirection);

  truth WasUnconscious = GetAction() && GetAction()->IsUnconsciousness();
  truth Burned = ReceiveDamage(Explosion->Terrorist, Explosion->FireOnly ? Damage : (Damage >> 1),
                               FIRE, ALL, DamageDirection, true, false, false, false);
  truth Pummeled = ReceiveDamage(Explosion->Terrorist, Explosion->FireOnly ? 0 : (Damage >> 1),
                                 PHYSICAL_DAMAGE, ALL, DamageDirection, true, false, false, false);

  // The ReceiveDamage calls above might cause 'this' to be polymorphed, in which case
  // SquareUnder[0] is null and calling GetArea or SpillBlood will crash.
  // See https://github.com/Attnam/ivan/issues/237 for details.
  if(SquareUnder[0] && Pummeled && GetArea()->IsValidPos(SpillPos))
    GetTorso()->SpillBlood((8 - Explosion->Size + RAND() % (8 - Explosion->Size)) >> 1, SpillPos);

  festring Msg;
  if(IsPlayer())
    Msg << "You are ";
  else
    Msg << CHAR_NAME(DEFINITE) << " is ";

  if (Burned && Pummeled)
    Msg << "blasted ";
  else if (Burned)
    Msg << "burned ";
  else if (Pummeled)
    Msg << "pummeled ";
  else
    Msg << "unharmed ";

  Msg << "by the " << (Explosion->FireOnly ? "fireball" : "explosion") << "!";

  if(IsPlayer() || CanBeSeenByPlayer())
    ADD_MESSAGE(Msg.CStr());

  if(IsEnabled())
    CheckDeath(Explosion->DeathMsg, Explosion->Terrorist, !WasUnconscious ? IGNORE_UNCONSCIOUSNESS : 0);
}

void character::SortAllItems(const sortdata& SortData)
{
  GetStack()->SortAllItems(SortData);
  doforequipmentswithparam<const sortdata&>()(this, &item::SortAllItems, SortData);
}

void character::PrintBeginSearchingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you can now notice even the very smallest details around you.");
}

void character::PrintEndSearchingMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel less perceptive.");
}

void character::SearchingHandler()
{
  if(!game::IsInWilderness())
    Search(15);
}

void character::Search(int Perception)
{
  for(int d = 0; d < GetExtendedNeighbourSquares(); ++d)
  {
    lsquare* LSquare = GetNeighbourLSquare(d);

    if(LSquare)
      LSquare->GetStack()->Search(this, Min(Perception, 200));
  }
}

// surprisingly returns 0 if fails

character* character::GetRandomNeighbour(int RelationFlags) const
{
  character* Chars[MAX_NEIGHBOUR_SQUARES];
  int Index = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    lsquare* LSquare = GetNeighbourLSquare(d);

    if(LSquare)
    {
      character* Char = LSquare->GetCharacter();

      if(Char && (GetRelation(Char) & RelationFlags))
        Chars[Index++] = Char;
    }
  }

  return Index ? Chars[RAND() % Index] : 0;
}

void character::ResetStates()
{
  for(int c = 0; c < STATES; ++c)
    if(1 << c != POLYMORPHED && TemporaryStateIsActivated(1 << c) && TemporaryStateCounter[c] != PERMANENT)
    {
      TemporaryState &= ~(1 << c);

      if(StateData[c].EndHandler)
      {
        (this->*StateData[c].EndHandler)();

        if(!IsEnabled())
          return;
      }
    }
}

void characterdatabase::InitDefaults(const characterprototype* NewProtoType, int NewConfig)
{
  IsAbstract = false;
  ProtoType = NewProtoType;
  Config = NewConfig;
  Alias.Clear();
}

void character::PrintBeginGasImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("All smells fade away.");
}

void character::PrintEndGasImmunityMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Yuck! The world smells bad again.");
}

void inventoryInfo(const character* pC){
  game::RegionListItemEnable(true);
  game::RegionSilhouetteEnable(true);

  pC->GetStack()->DrawContents(pC, CONST_S("Your inventory"), REMEMBER_SELECTED);

  for(stackiterator i = pC->GetStack()->GetBottom(); i.HasItem(); ++i)
    i->DrawContents(pC);

  doforequipmentswithparam<ccharacter*>()(pC, &item::DrawContents, pC);

  game::RegionListItemEnable(false);
  game::RegionSilhouetteEnable(false);
}

void character::ShowAdventureInfo() const
{
  graphics::SetAllowStretchedBlit();

  if(ivanconfig::IsAltAdentureInfo())
  {
    ShowAdventureInfoAlt();
  }
  else
  {
    if(GetStack()->GetItems()
       && game::TruthQuestion(CONST_S("Do you want to see your inventory? [y/n]"), REQUIRES_ANSWER))
    {
      inventoryInfo(this);
    }

    if(game::TruthQuestion(CONST_S("Do you want to see your message history? [y/n]"), REQUIRES_ANSWER))
      msgsystem::DrawMessageHistory();

    if(!game::MassacreListsEmpty()
       && game::TruthQuestion(CONST_S("Do you want to see your massacre history? [y/n]"), REQUIRES_ANSWER))
    {
      game::DisplayMassacreLists();
    }
  }

  graphics::SetDenyStretchedBlit(); //back to menu
}

void character::ShowAdventureInfoAlt() const
{
  while(true) {
#ifdef WIZARD
    int Answer =
     game::KeyQuestion(
       CONST_S("See (i)nventory, (m)essage history, (k)ill list, (l)ook, (x) cheat look or [ESC]/(n)othing?"),
         'z', 13, 'i','I', 'm','M', 'k','K', 'l','L', 'x','X', 'N','n', KEY_ESC); //default answer 'z' is ignored
#else
    int Answer =
     game::KeyQuestion(
       CONST_S("See (i)nventory, (m)essage history, (k)ill list, (l)ook or [ESC]/(n)othing?"),
         'z', 11, 'i','I', 'm','M', 'k','K', 'l','L', 'n','N', KEY_ESC); //default answer 'z' is ignored
#endif

    if(Answer == 'i' || Answer == 'I'){
      inventoryInfo(this);
    }else if(Answer == 'm' || Answer == 'M'){
      msgsystem::DrawMessageHistory();
    }else if(Answer == 'k' || Answer == 'K'){
      game::DisplayMassacreLists();
#ifdef WIZARD
    }else if(Answer == 'l' || Answer == 'L' || Answer == 'x' || Answer == 'X'){
      commandsystem::PlayerDiedLookMode(Answer == 'x' || Answer == 'X');
#else
    }else if(Answer == 'l' || Answer == 'L'){
      commandsystem::PlayerDiedLookMode();
#endif
    }else if(Answer == 'n' || Answer == 'N' || Answer == KEY_ESC){
      return;
    }
  }
}


truth character::EditAllAttributes(int Amount)
{
  if(!Amount)
    return true;

  int c;
  truth MayEditMore = false;

  for(c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->EditAllAttributes(Amount))
      MayEditMore = true;
  }

  for(c = 0; c < BASE_ATTRIBUTES; ++c)
    if(BaseExperience[c])
    {
      BaseExperience[c] += Amount * EXP_MULTIPLIER;
      LimitRef(BaseExperience[c], MIN_EXP, MAX_EXP);

      if((Amount < 0 && BaseExperience[c] != MIN_EXP)
         || (Amount > 0 && BaseExperience[c] != MAX_EXP))
        MayEditMore = true;
    }

  CalculateAll();
  RestoreHP();
  RestoreStamina();

  if(IsPlayer())
  {
    game::SendLOSUpdateRequest();
    UpdateESPLOS();
  }

  if(IsPlayerKind())
    UpdatePictures();

  return MayEditMore;
}

#ifdef WIZARD

void character::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(57);
  Entry << GetAttribute(ENDURANCE);
  Entry.Resize(60);
  Entry << GetAttribute(PERCEPTION);
  Entry.Resize(63);
  Entry << GetAttribute(INTELLIGENCE);
  Entry.Resize(66);
  Entry << GetAttribute(WISDOM);
  Entry.Resize(69);
  Entry << GetAttribute(CHARISMA);
  Entry.Resize(72);
  Entry << GetAttribute(MANA);
}

void character::AddDefenceInfo(felist& List) const
{
  festring Entry;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
    {
      Entry = CONST_S("   ");
      BodyPart->AddName(Entry, UNARTICLED);
      Entry.Resize(60);
      Entry << BodyPart->GetMaxHP();
      Entry.Resize(70);
      Entry << BodyPart->GetTotalResistance(PHYSICAL_DAMAGE);
      List.AddEntry(Entry, LIGHT_GRAY);
    }
  }
}

void character::DetachBodyPart()
{
  ADD_MESSAGE("You haven't got any extra bodyparts.");
}

void character::SetFireToBodyPart()
{
  ADD_MESSAGE("You haven't got any extra bodyparts.");
}

#endif

void character::ReceiveHolyBanana(long Amount)
{
  Amount <<= 1;
  EditExperience(ARM_STRENGTH, Amount, 1 << 13);
  EditExperience(LEG_STRENGTH, Amount, 1 << 13);
  EditExperience(DEXTERITY, Amount, 1 << 13);
  EditExperience(AGILITY, Amount, 1 << 13);
  EditExperience(ENDURANCE, Amount, 1 << 13);
  EditExperience(PERCEPTION, Amount, 1 << 13);
  EditExperience(INTELLIGENCE, Amount, 1 << 13);
  EditExperience(WISDOM, Amount, 1 << 13);
  EditExperience(CHARISMA, Amount, 1 << 13);
  RestoreLivingHP();
}

void character::AddHolyBananaConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel a mysterious strengthening fire coursing through your body.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("For a moment %s is surrounded by a swirling fire aura.", CHAR_NAME(DEFINITE));
}

truth character::PreProcessForBone()
{
  if(IsPet() && IsEnabled())
  {
    Die(0, CONST_S(""), FORBID_REINCARNATION);
    return true;
  }

  if(GetAction())
    GetAction()->Terminate(false);

  if(TemporaryStateIsActivated(POLYMORPHED))
  {
    character* PolymorphBackup = GetPolymorphBackup();
    EndPolymorph();
    PolymorphBackup->PreProcessForBone();
    return true;
  }

  if(MustBeRemovedFromBone())
    return false;
  else if(IsUnique() && !CanBeGenerated())
    game::SignalQuestMonsterFound();

  RestoreLivingHP();
  ResetStates();
  RemoveTraps();
  GetStack()->PreProcessForBone();
  doforequipments()(this, &item::PreProcessForBone);
  doforbodyparts()(this, &bodypart::PreProcessForBone);
  game::RemoveCharacterID(ID);
  ID = -ID;
  game::AddCharacterID(this, ID);
  return true;
}

truth character::PostProcessForBone(double& DangerSum, int& Enemies)
{
  if(PostProcessForBone())
  {
    if(GetRelation(PLAYER) == HOSTILE)
    {
      double Danger = GetRelativeDanger(PLAYER, true);

      if(Danger > 99.)
        game::SetTooGreatDangerFound(true);
      else if(!IsUnique() && !IgnoreDanger())
      {
        DangerSum += Danger;
        ++Enemies;
      }
    }

    return true;
  }
  else
    return false;
}

truth character::PostProcessForBone()
{
  ulong NewID = game::CreateNewCharacterID(this);
  game::GetBoneCharacterIDMap().insert(std::make_pair(-ID, NewID));
  game::RemoveCharacterID(ID);
  ID = NewID;

  if(IsUnique() && CanBeGenerated())
  {
    if(DataBase->Flags & HAS_BEEN_GENERATED)
      return false;
    else
      SignalGeneration();
  }

  GetStack()->PostProcessForBone();
  doforequipments()(this, &item::PostProcessForBone);
  doforbodyparts()(this, &bodypart::PostProcessForBone);
  return true;
}

void character::FinalProcessForBone()
{
  Flags &= ~C_PLAYER;
  GetStack()->FinalProcessForBone();
  doforequipments()(this, &item::FinalProcessForBone);
  int c;

  for(c = 0; c < BodyParts; ++c)
  {
    for(std::list<ulong>::iterator i = OriginalBodyPartID[c].begin(); i != OriginalBodyPartID[c].end();)
    {
      boneidmap::iterator BI = game::GetBoneItemIDMap().find(*i);

      if(BI == game::GetBoneItemIDMap().end())
      {
        std::list<ulong>::iterator Dirt = i++;
        OriginalBodyPartID[c].erase(Dirt);
      }
      else
      {
        *i = BI->second;
        ++i;
      }
    }
  }
}

void character::SetSoulID(ulong What)
{
  if(GetPolymorphBackup())
    GetPolymorphBackup()->SetSoulID(What);
}

truth character::SearchForItem(citem* Item) const
{
  if(combineequipmentpredicateswithparam<ulong>()(this, &item::HasID, Item->GetID(), 1))
    return true;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(*i == Item)
      return true;

  return false;
}

item* character::SearchForItem(const sweaponskill* SWeaponSkill) const
{
  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && SWeaponSkill->IsSkillOf(Equipment))
      return Equipment;
  }

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(SWeaponSkill->IsSkillOf(*i))
      return *i;

  return 0;
}

void character::PutNear(v2 Pos)
{
  v2 NewPos = game::GetCurrentLevel()->GetNearestFreeSquare(this, Pos, false);

  if(NewPos == ERROR_V2)
    do
    {
      NewPos = game::GetCurrentLevel()->GetRandomSquare(this);
    }
    while(NewPos == Pos);

  PutTo(NewPos);
}

void character::PutToOrNear(v2 Pos)
{
  if(game::IsInWilderness() || (CanMoveOn(game::GetCurrentLevel()->GetLSquare(Pos))
                                && IsFreeForMe(game::GetCurrentLevel()->GetLSquare(Pos))))
    PutTo(Pos);
  else
    PutNear(Pos);
}

void character::PutTo(v2 Pos)
{
  SquareUnder[0] = game::GetCurrentArea()->GetSquare(Pos);
  SquareUnder[0]->AddCharacter(this);
}

void character::Remove()
{
  SquareUnder[0]->RemoveCharacter();
  SquareUnder[0] = 0;
}

void character::SendNewDrawRequest() const
{
  for(int c = 0; c < SquaresUnder; ++c)
  {
    square* Square = GetSquareUnder(c);

    if(Square)
      Square->SendNewDrawRequest();
  }
}

truth character::IsOver(v2 Pos) const
{
  for(int c = 0; c < SquaresUnder; ++c)
  {
    square* Square = GetSquareUnder(c);

    if(Square && Square->GetPos() == Pos)
      return true;
  }

  return false;
}

truth character::CanTheoreticallyMoveOn(const lsquare* LSquare) const
{
  return GetMoveType() & LSquare->GetTheoreticalWalkability();
}

truth character::CanMoveOn(const lsquare* LSquare) const
{
  return GetMoveType() & LSquare->GetWalkability();
}

truth character::CanMoveOn(const square* Square) const
{
  return GetMoveType() & Square->GetSquareWalkability();
}

truth character::CanMoveOn(const olterrain* OLTerrain) const
{
  return GetMoveType() & OLTerrain->GetWalkability();
}

truth character::CanMoveOn(const oterrain* OTerrain) const
{
  return GetMoveType() & OTerrain->GetWalkability();
}

truth character::IsFreeForMe(square* Square) const
{
  return !Square->GetCharacter() || Square->GetCharacter() == this;
}

void character::LoadSquaresUnder()
{
  SquareUnder[0] = game::GetSquareInLoad();
}

truth character::AttackAdjacentEnemyAI()
{
  if(!IsEnabled())
    return false;

  character* Char[MAX_NEIGHBOUR_SQUARES];
  v2 Pos[MAX_NEIGHBOUR_SQUARES];
  int Dir[MAX_NEIGHBOUR_SQUARES];
  int Index = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    square* Square = GetNeighbourSquare(d);

    if(Square)
    {
      character* Enemy = Square->GetCharacter();

      if(Enemy && (GetRelation(Enemy) == HOSTILE || StateIsActivated(CONFUSED)))
      {
        Dir[Index] = d;
        Pos[Index] = Square->GetPos();
        Char[Index++] = Enemy;
      }
    }
  }

  if(Index)
  {
    int ChosenIndex = RAND() % Index;
    Hit(Char[ChosenIndex], Pos[ChosenIndex], Dir[ChosenIndex]);
    return true;
  }

  return false;
}

void character::SignalStepFrom(lsquare** OldSquareUnder)
{
  int c;
  lsquare* NewSquareUnder[MAX_SQUARES_UNDER];

  for(c = 0; c < GetSquaresUnder(); ++c)
    NewSquareUnder[c] = GetLSquareUnder(c);

  for(c = 0; c < GetSquaresUnder(); ++c)
    if(IsEnabled() && GetLSquareUnder(c) == NewSquareUnder[c])
      NewSquareUnder[c]->StepOn(this, OldSquareUnder);
}

int character::GetSumOfAttributes() const
{
  return GetAttribute(ENDURANCE)
       + GetAttribute(PERCEPTION)
       + GetAttribute(INTELLIGENCE)
       + GetAttribute(WISDOM)
       + GetAttribute(CHARISMA)
       + GetAttribute(ARM_STRENGTH)
       + GetAttribute(AGILITY);
}

void character::IntelligenceAction(int Difficulty)
{
  EditAP(-20000 * Difficulty / APBonus(GetAttribute(INTELLIGENCE)));
  EditExperience(INTELLIGENCE, Difficulty * 15, 1 << 7);
}

struct walkabilitycontroller
{
  static truth Handler(int x, int y)
  {
    return x >= 0 && y >= 0 && x < LevelXSize && y < LevelYSize
                && Map[x][y]->GetTheoreticalWalkability() & MoveType;
  }
  static lsquare*** Map;
  static int LevelXSize, LevelYSize;
  static int MoveType;
};

lsquare*** walkabilitycontroller::Map;
int walkabilitycontroller::LevelXSize, walkabilitycontroller::LevelYSize;
int walkabilitycontroller::MoveType;

truth character::CreateRoute()
{
  Route.clear();

  if(GetAttribute(INTELLIGENCE) >= 10 && !StateIsActivated(CONFUSED))
  {
    v2 Pos = GetPos();
    walkabilitycontroller::Map = GetLevel()->GetMap();
    walkabilitycontroller::LevelXSize = GetLevel()->GetXSize();
    walkabilitycontroller::LevelYSize = GetLevel()->GetYSize();
    walkabilitycontroller::MoveType = GetMoveType();
    node* Node;

    for(int c = 0; c < game::GetTeams(); ++c)
      for(character* Char : game::GetTeam(c)->GetMember())
      {
        if(Char->IsEnabled()
           && !Char->Route.empty()
           && (Char->GetMoveType() & GetMoveType()) == Char->GetMoveType())
        {
          v2 CharGoingTo = Char->Route[0];
          v2 iPos = Char->Route.back();

          if((GoingTo - CharGoingTo).GetLengthSquare() <= 100
             && (Pos - iPos).GetLengthSquare() <= 100
             && mapmath<walkabilitycontroller>::DoLine(CharGoingTo.X, CharGoingTo.Y, GoingTo.X, GoingTo.Y, SKIP_FIRST)
             && mapmath<walkabilitycontroller>::DoLine(Pos.X, Pos.Y, iPos.X, iPos.Y, SKIP_FIRST))
          {
            if(!Illegal.empty() && Illegal.find(Char->Route.back()) != Illegal.end())
              continue;

            Node = GetLevel()->FindRoute(CharGoingTo, GoingTo, Illegal, GetMoveType());

            if(Node)
              while(Node->Last)
              {
                Route.push_back(Node->Pos);
                Node = Node->Last;
              }
            else
            {
              Route.clear();
              continue;
            }

            Route.insert(Route.end(), Char->Route.begin(), Char->Route.end());
            Node = GetLevel()->FindRoute(Pos, iPos, Illegal, GetMoveType());

            if(Node)
              while(Node->Last)
              {
                Route.push_back(Node->Pos);
                Node = Node->Last;
              }
            else
            {
              Route.clear();
              continue;
            }

            IntelligenceAction(1);
            return true;
          }
        }
      }

    Node = GetLevel()->FindRoute(Pos, GoingTo, Illegal, GetMoveType());

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

void character::SetGoingTo(v2 What)
{
  if(GoingTo != What)
  {
    GoingTo = What;
    Route.clear();
    Illegal.clear();
  }
}

void character::TerminateGoingTo()
{
  GoingTo = ERROR_V2;
  Route.clear();
  Illegal.clear();
}

truth character::CheckForFood(int Radius)
{
  if(StateIsActivated(PANIC) || !UsesNutrition() || !IsEnabled())
    return false;

  v2 Pos = GetPos();
  int x, y;

  for(int r = 1; r <= Radius; ++r)
  {
    x = Pos.X - r;

    if(x >= 0)
      for(y = Pos.Y - r; y <= Pos.Y + r; ++y)
        if(CheckForFoodInSquare(v2(x, y)))
          return true;

    x = Pos.X + r;

    if(x < GetLevel()->GetXSize())
      for(y = Pos.Y - r; y <= Pos.Y + r; ++y)
        if(CheckForFoodInSquare(v2(x, y)))
          return true;

    y = Pos.Y - r;

    if(y >= 0)
      for(x = Pos.X - r; x <= Pos.X + r; ++x)
        if(CheckForFoodInSquare(v2(x, y)))
          return true;

    y = Pos.Y + r;

    if(y < GetLevel()->GetYSize())
      for(x = Pos.X - r; x <= Pos.X + r; ++x)
        if(CheckForFoodInSquare(v2(x, y)))
          return true;
  }

  return false;
}

truth character::CheckForFoodInSquare(v2 Pos)
{
  level* Level = GetLevel();

  if(Level->IsValidPos(Pos))
  {
    lsquare* Square = Level->GetLSquare(Pos);
    stack* Stack = Square->GetStack();

    if(Stack->GetItems())
      for(stackiterator i = Stack->GetBottom(); i.HasItem(); ++i)
        if(i->IsPickable(this)
           && i->CanBeSeenBy(this)
           && i->CanBeEatenByAI(this)
           && (!Square->GetRoomIndex()
               || Square->GetRoom()->AllowFoodSearch()))
        {
          SetGoingTo(Pos);
          return MoveTowardsTarget(false);
        }
  }

  return false;
}

void character::SetConfig(int NewConfig, int SpecialFlags)
{
  databasecreator<character>::InstallDataBase(this, NewConfig);
  CalculateAll();
  CheckIfSeen();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();
}

truth character::IsOver(citem* Item) const
{
  for(int c1 = 0; c1 < Item->GetSquaresUnder(); ++c1)
    for(int c2 = 0; c2 < SquaresUnder; ++c2)
      if(Item->GetPos(c1) == GetPos(c2))
        return true;

  return false;
}

truth character::CheckConsume(cfestring& Verb) const
{
  if(!UsesNutrition())
  {
    if(IsPlayer())
      ADD_MESSAGE("In this form you can't and don't need to %s.", Verb.CStr());

    return false;
  }

  return true;
}

void character::PutTo(lsquare* To)
{
  PutTo(To->GetPos());
}

double character::RandomizeBabyExperience(double SumE)
{
  if(!SumE)
    return 0;

  double E = (SumE / 4) - (SumE / 32) + (double(RAND()) / MAX_RAND) * (SumE / 16 + 1);
  return Limit(E, MIN_EXP, MAX_EXP);
}

liquid* character::CreateBlood(long Volume) const
{
  return liquid::Spawn(GetBloodMaterial(), Volume);
}

void character::SpillFluid(character* Spiller, liquid* Liquid, int SquareIndex)
{
  long ReserveVolume = Liquid->GetVolume() >> 1;
  Liquid->EditVolume(-ReserveVolume);
  GetStack()->SpillFluid(Spiller, Liquid,
                         long(Liquid->GetVolume() * sqrt(double(GetStack()->GetVolume()) / GetVolume())));
  Liquid->EditVolume(ReserveVolume);
  int c;
  long Modifier[MAX_BODYPARTS], ModifierSum = 0;

  for(c = 0; c < BodyParts; ++c)
    if(GetBodyPart(c))
    {
      Modifier[c] = long(sqrt(GetBodyPart(c)->GetVolume()));

      if(Modifier[c])
        Modifier[c] *= 1 + (RAND() & 3);

      ModifierSum += Modifier[c];
    }
    else
      Modifier[c] = 0;

  for(c = 1; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && IsEnabled())
      GetBodyPart(c)->SpillFluid(Spiller,
                                 Liquid->SpawnMoreLiquid(Liquid->GetVolume() * Modifier[c] / ModifierSum),
                                 SquareIndex);

  if(IsEnabled())
  {
    Liquid->SetVolume(Liquid->GetVolume() * Modifier[TORSO_INDEX] / ModifierSum);
    GetTorso()->SpillFluid(Spiller, Liquid, SquareIndex);
  }
}

void character::StayOn(liquid* Liquid)
{
  Liquid->TouchEffect(this, TORSO_INDEX);
}

truth character::IsAlly(ccharacter* Char) const
{
  return Char->GetTeam()->GetID() == GetTeam()->GetID();
}

void character::ResetSpoiling()
{
  doforbodyparts()(this, &bodypart::ResetSpoiling);
}

void character::ResetBurning()
{
  doforbodyparts()(this, &bodypart::ResetBurning);
}

void character::ResetLivingBurning()
{
  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->CanRegenerate() && BodyPart->GetMainMaterial())
    {
      BodyPart->GetMainMaterial()->ResetBurning();
    }
  }
}

void character::RemoveBurns()
{
  doforbodyparts()(this, &bodypart::RemoveBurns);
}

void character::ResetThermalEnergies()
{
  doforbodyparts()(this, &bodypart::ResetThermalEnergies);
}

item* character::SearchForItem(ccharacter* Char, sorter Sorter) const
{
  item* Equipment = findequipment<ccharacter*>()(this, Sorter, Char);

  if(Equipment)
    return Equipment;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if(((*i)->*Sorter)(Char))
      return *i;

  return 0;
}

truth character::DetectMaterial(cmaterial* Material) const
{
  return GetStack()->DetectMaterial(Material)
    || combinebodypartpredicateswithparam<cmaterial*>()(this, &bodypart::DetectMaterial, Material, 1)
    || combineequipmentpredicateswithparam<cmaterial*>()(this, &item::DetectMaterial, Material, 1);
}

truth character::DamageTypeDestroysBodyPart(int Type)
{
  return (Type&0xFFF) != PHYSICAL_DAMAGE;
}

truth character::CheckIfTooScaredToHit(ccharacter* Enemy) const
{
  if(IsPlayer() && StateIsActivated(PANIC))
    for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
      {
        if(CanMoveOn(Square)
           && (!Square->GetCharacter()
               || Square->GetCharacter()->IsPet()))
        {
          ADD_MESSAGE("You are too scared to attack %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
          return true;
        }
      }
    }

  return false;
}

void character::PrintBeginLevitationMessage() const
{
  if(!IsFlying())
  {
    if(IsPlayer())
      ADD_MESSAGE("You rise into the air like a small hot-air balloon.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s begins to float.", CHAR_NAME(DEFINITE));
  }
}

void character::PrintEndLevitationMessage() const
{
  if(!IsFlying())
  {
    if(IsPlayer())
      ADD_MESSAGE("You descend gently onto the ground.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s drops onto the ground.", CHAR_NAME(DEFINITE));
  }
}

truth character::IsLimbIndex(int I)
{
  switch(I)
  {
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX:
    return true;
  }

  return false;
}

void character::EditExperience(int Identifier, double Value, double Speed)
{
  if(!AllowExperience()
     || (Identifier == ENDURANCE && UseMaterialAttributes()))
    return;

  int Change = RawEditExperience(BaseExperience[Identifier],
                                 GetNaturalExperience(Identifier),
                                 Value, Speed);

  if(!Change)
    return;

  cchar* PlayerMsg = 0, * NPCMsg = 0;

  switch(Identifier)
  {
   case ENDURANCE:
    if(Change > 0)
    {
      PlayerMsg = "You feel tougher than anything!";

      if(IsPet())
        NPCMsg = "Suddenly %s looks tougher.";
    }
    else
    {
      PlayerMsg = "You feel less healthy.";

      if(IsPet())
        NPCMsg = "Suddenly %s looks less healthy.";
    }

    CalculateBodyPartMaxHPs();
    CalculateMaxStamina();
    break;
   case PERCEPTION:
    if(IsPlayer())
    {
      if(Change > 0)
        PlayerMsg = "You now see the world in much better detail than before.";
      else
      {
        PlayerMsg = "You feel very guru.";
        game::GetGod(VALPURUS)->AdjustRelation(100);
      }

      game::SendLOSUpdateRequest();
    }
    break;
   case INTELLIGENCE:
    if(IsPlayer())
    {
      if(Change > 0)
        PlayerMsg = "Suddenly the inner structure of the Multiverse around you looks quite simple.";
      else
        PlayerMsg = "It surely is hard to think today.";

      UpdateESPLOS();
    }

    if(IsPlayerKind())
      UpdatePictures();

    break;
   case WISDOM:
    if(IsPlayer())
    {
      if(Change > 0)
        PlayerMsg = "You feel your life experience increasing all the time.";
      else
        PlayerMsg = "You feel like having done something unwise.";
    }

    if(IsPlayerKind())
      UpdatePictures();

    break;
   case WILL_POWER:
    if(Change > 0)
    {
      PlayerMsg = "You feel incredibly stubborn.";
      NPCMsg = "You notice %s looks much more determined.";
    }
    else
    {
      PlayerMsg = "You loose your determination.";
      NPCMsg = "You notice how wimpy %s looks.";
    }
    break;
   case CHARISMA:
    if(Change > 0)
    {
      PlayerMsg = "You feel very confident of your social skills.";

      if(IsPet())
      {
        if(GetAttribute(CHARISMA) <= 15)
          NPCMsg = "%s looks less ugly.";
        else
          NPCMsg = "%s looks more attractive.";
      }
    }
    else
    {
      PlayerMsg = "You feel somehow disliked.";

      if(IsPet())
      {
        if(GetAttribute(CHARISMA) < 15)
          NPCMsg = "%s looks more ugly.";
        else
          NPCMsg = "%s looks less attractive.";
      }
    }

    if(IsPlayerKind())
      UpdatePictures();

    break;
   case MANA:
    if(Change > 0)
    {
      PlayerMsg = "You feel magical forces coursing through your body!";
      NPCMsg = "You notice an odd glow around %s.";
    }
    else
    {
      PlayerMsg = "You feel your magical abilities withering slowly.";
      NPCMsg = "You notice strange vibrations in the air around %s. But they disappear rapidly.";
    }
    break;
   default:
    if(Change > 0)
    {
      PlayerMsg = "You feel more awesome!";
      NPCMsg = "You notice how cool %s looks.";
    }
    else
    {
      PlayerMsg = "You feel a strange sensation, but then it passes.";
      NPCMsg = "You notice there is something different about %s.";
    }
  }

  if(IsPlayer())
    ADD_MESSAGE(PlayerMsg);
  else if(NPCMsg && CanBeSeenByPlayer())
    ADD_MESSAGE(NPCMsg, CHAR_NAME(DEFINITE));

  CalculateBattleInfo();
}

int character::RawEditExperience(double& Exp, double NaturalExp, double Value, double Speed) const
{
  double OldExp = Exp;

  if(Speed < 0)
  {
    Speed = -Speed;
    Value = -Value;
  }

  if(!OldExp || !Value
     || (Value > 0 && OldExp >= NaturalExp * (100 + Value) / 100)
     || (Value < 0 && OldExp <= NaturalExp * (100 + Value) / 100))
    return 0;

  if(!IsPlayer())
    Speed *= 1.5;

  Exp += (NaturalExp * (100 + Value) - 100 * OldExp) * Speed * EXP_DIVISOR;
  LimitRef(Exp, MIN_EXP, MAX_EXP);
  int NewA = int(Exp * EXP_DIVISOR);
  int OldA = int(OldExp * EXP_DIVISOR);
  int Delta = NewA - OldA;

  if(Delta > 0)
    Exp = Max(Exp, (NewA + 0.05) * EXP_MULTIPLIER);
  else if(Delta < 0)
    Exp = Min(Exp, (NewA + 0.95) * EXP_MULTIPLIER);

  LimitRef(Exp, MIN_EXP, MAX_EXP);
  return Delta;
}

int character::GetAttribute(int Identifier, truth AllowBonus) const
{
  int A = int(BaseExperience[Identifier] * EXP_DIVISOR);

  if(AllowBonus && Identifier == INTELLIGENCE && BrainsHurt())
    return Max((A + AttributeBonus[INTELLIGENCE]) / 3, 1);

  if(AllowBonus && Identifier == PERCEPTION && IsHeadless())
    return 0;

  return A && AllowBonus ? Max(A + AttributeBonus[Identifier], 1) : A;
}

void characterdatabase::PostProcess()
{
  double AM = (100 + AttributeBonus) * EXP_MULTIPLIER / 100;

  for(int c = 0; c < ATTRIBUTES; ++c)
    NaturalExperience[c] = this->*ExpPtr[c] * AM;
}

void character::EditDealExperience(long Price)
{
  EditExperience(CHARISMA, sqrt(Price) * 10, 15000);
}

void character::PrintBeginLeprosyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel you're falling in pieces.");
}

void character::PrintEndLeprosyMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your limbs are stuck in place tightly."); // CHANGE OR DIE
}

void character::TryToInfectWithLeprosy(ccharacter* Infector)
{
  if(!IsImmuneToLeprosy()
     && ((GetRelation(Infector) == HOSTILE
          && !RAND_N(50 * GetAttribute(ENDURANCE)))
         || !RAND_N(500 * GetAttribute(ENDURANCE))))
    GainIntrinsic(LEPROSY);
}

void character::SignalGeneration()
{
  const_cast<database*>(DataBase)->Flags |= HAS_BEEN_GENERATED;
}

void character::CheckIfSeen()
{
  if(IsPlayer() || CanBeSeenByPlayer())
    SignalSeen();
}

void character::SignalSeen()
{
  if(!(WarnFlags & WARNED)
     && GetRelation(PLAYER) == HOSTILE
     && !StateIsActivated(FEARLESS))
  {
    double Danger = GetRelativeDanger(PLAYER);

    if(Danger > 5.)
    {
      game::SetDangerFound(Max(game::GetDangerFound(), Danger));

      if(Danger > 500. && !(WarnFlags & HAS_CAUSED_PANIC))
      {
        WarnFlags |= HAS_CAUSED_PANIC;
        game::SetCausePanicFlag(true);
      }

      WarnFlags |= WARNED;
    }
  }

  const_cast<database*>(DataBase)->Flags |= HAS_BEEN_SEEN;
}

int character::GetPolymorphIntelligenceRequirement() const
{
  if(DataBase->PolymorphIntelligenceRequirement == DEPENDS_ON_ATTRIBUTES)
    return Max(GetAttributeAverage() - 5, 0);
  else
    return DataBase->PolymorphIntelligenceRequirement;
}

void character::RemoveAllItems()
{
  GetStack()->Clean();

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
    {
      Equipment->RemoveFromSlot();
      Equipment->SendToHell();
    }
  }
}

int character::CalculateWeaponSkillHits(ccharacter* Enemy) const
{
  if(Enemy->IsPlayer())
  {
    configid ConfigID(GetType(), GetConfig());
    const dangerid& DangerID = game::GetDangerMap().find(ConfigID)->second;
    return Min(int(DangerID.EquippedDanger * 2000), 1000);
  }
  else
    return Min(int(GetRelativeDanger(Enemy, true) * 2000), 1000);
}

truth character::CanUseEquipment(int I) const
{
  return CanUseEquipment() && I < GetEquipments() && GetBodyPartOfEquipment(I) && EquipmentIsAllowed(I);
}

void character::MemorizeEquipedItems(){DBGCHAR(this,"");
  int iEqCount=0;
  for(int c = 0; c < MAX_EQUIPMENT_SLOTS; ++c)
  {
    item* Item = NULL;
    if(CanUseEquipment(c))Item=GetEquipment(c);

    if(Item!=NULL){
      MemorizedEquippedItemIDs[c]=Item->GetID(); DBGSI(MemorizedEquippedItemIDs[c]);
      iEqCount++;
    }else{
      MemorizedEquippedItemIDs[c]=0;
    }
  }
}

void addPolymorphBkpToVector(character* CharIni,std::vector<character*>* pvCharMemory){
  character* CharMemory = CharIni;

  while(CharMemory!=NULL){
    if(std::find(pvCharMemory->begin(), pvCharMemory->end(), CharMemory) == pvCharMemory->end()){ //if not already in vector
      pvCharMemory->push_back(CharMemory); DBGCHAR(CharMemory,"");
    }

    CharMemory=CharMemory->GetPolymorphBackup();

    if(CharMemory==CharIni){DBG1("weirdRecurrentPolymorphBkp?");break;} //TODO keep this? just in case some weird thing happens out of here? it probably would mean inconsistency?
  }
}
/* Target mustn't have any equipment */
void character::DonateEquipmentTo(character* Character)
{
  if(IsPlayer())
  {
    ulong* EquipmentMemory = game::GetEquipmentMemory();

    for(int c = 0; c < MAX_EQUIPMENT_SLOTS; ++c)
    {
      item* Item = GetEquipment(c);

      if(Item)
      {
        if(Character->CanUseEquipment(c))
        {
          Item->RemoveFromSlot();
          Character->SetEquipment(c, Item);
        }
        else
        {
          EquipmentMemory[c] = Item->GetID();
          Item->MoveTo(Character->GetStack());
        }
      }
      else if(CanUseEquipment(c))
        EquipmentMemory[c] = 0;
      else if(EquipmentMemory[c]
              && Character->CanUseEquipment(c))
      {
        for(stackiterator i = Character->GetStack()->GetBottom(); i.HasItem(); ++i)
          if(i->GetID() == EquipmentMemory[c])
          {
            item* Item = *i;
            Item->RemoveFromSlot();
            Character->SetEquipment(c, Item);
            break;
          }

        EquipmentMemory[c] = 0;
      }
    }
  }
  else
  {
    bool bImprovedEquipping =
       ivanconfig::GetMemorizeEquipmentMode()==2 ||
      (ivanconfig::GetMemorizeEquipmentMode()==1 && IsPet());

    if(bImprovedEquipping)MemorizeEquipedItems();

    for(int c = 0; c < GetEquipments(); ++c)
    {
      item* Item = GetEquipment(c);

      if(bImprovedEquipping && Item==NULL){
        /**
         *
         * Looks as much far as possible for Equipped memories.
         */
        std::vector<character*> vCharMemory;
        addPolymorphBkpToVector(Character,&vCharMemory);
        addPolymorphBkpToVector(this,&vCharMemory); DBGSI(vCharMemory.size());

        /* The target has all items on it's inventory now, so search at it only. */
        DBGCHAR(Character,""); DBGCHAR(this,"");
        for(int i=0;i<vCharMemory.size();i++){ character* CharMemory=vCharMemory[i]; DBGCHAR(CharMemory,"");
          Item=Character->SearchForItem(CharMemory->MemorizedEquippedItemIDs[c]);
          if(Item!=NULL){
            DBG5("FoundMemEqAtInv",DBGI(Item->GetID()),DBGI(CharMemory->GetID()),DBGI(this->GetID()),DBGI(Character->GetID()));
            break;
          }
        }
      }

      if(Item)
      {
        if(Character->CanUseEquipment(c))
        {
          Item->RemoveFromSlot();
          Character->SetEquipment(c, Item);
        }
        else
        {
          if(bImprovedEquipping){
            Item->MoveTo(Character->GetStack());
          }else{
            Item->MoveTo(Character->GetStackUnder());
          }
        }
      }
    }
  }
}

void character::ReceivePeaSoup(long)
{                                            /* don't spawn fart smoke on the world map (smoke objects only
                                                have functions for lsquares, not wsquares) */
  if(!game::IsInWilderness() || !IsPlayer()) /* not sure if the AI eats while the player's on the world map,
                                                but smoke should still spawn in the dungeon if they do */
  {
    lsquare* Square = GetLSquareUnder();

    if(Square->IsFlyable())
      Square->AddSmoke(gas::Spawn(FART, 250));
  }
}

void character::AddPeaSoupConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Mmmh! The soup is very tasty.%s", CanHear() ? " You hear a small puff." : "");
  else if(CanBeSeenByPlayer() && PLAYER->CanHear()) // change someday
    ADD_MESSAGE("You hear a small puff.");
}

void character::CalculateMaxStamina()
{
  MaxStamina = TorsoIsAlive() ? GetAttribute(ENDURANCE) * 10000 : 0;
}

void character::EditStamina(int Amount, truth CanCauseUnconsciousness)
{
  if(!TorsoIsAlive())
    return;

  int UnconsciousnessStamina = MaxStamina >> 3;

  if(!CanCauseUnconsciousness && Amount < 0)
  {
    if(Stamina > UnconsciousnessStamina)
    {
      Stamina += Amount;

      if(Stamina < UnconsciousnessStamina)
        Stamina = UnconsciousnessStamina;
    }

    return;
  }

  int OldStamina = Stamina;
  Stamina += Amount;

  if(Stamina > MaxStamina)
    Stamina = MaxStamina;
  else if(Stamina < 0)
  {
    Stamina = 0;
    LoseConsciousness(250 + RAND_N(250));
  }
  else if(IsPlayer())
  {
    if(OldStamina >= MaxStamina >> 2 && Stamina < MaxStamina >> 2)
      ADD_MESSAGE("You are getting a little tired.");
    else if(OldStamina >= UnconsciousnessStamina
            && Stamina < UnconsciousnessStamina)
    {
      ADD_MESSAGE("You are seriously out of breath!");
      game::SetPlayerIsRunning(false);
    }
  }

  if(IsPlayer() && StateIsActivated(PANIC)
     && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);
}

void character::RegenerateStamina()
{
  if(GetTirednessState() != UNTIRED)
  {
    EditExperience(ENDURANCE, 50, 1);

    if(Sweats() && TorsoIsAlive() && !RAND_N(30) && !game::IsInWilderness())
    {
      long Volume = long(.05 * sqrt(GetBodyVolume()));

      if(GetTirednessState() == FAINTING)
        Volume <<= 1;

      for(int c = 0; c < SquaresUnder; ++c)
        GetLSquareUnder(c)->SpillFluid(0, CreateSweat(Volume), false, false);
    }
  }

  int Bonus = 1;

  if(Action)
  {
    if(Action->IsRest())
    {
      if(SquaresUnder == 1)
        Bonus = GetSquareUnder()->GetRestModifier() << 1;
      else
      {
        int Lowest = GetSquareUnder(0)->GetRestModifier();

        for(int c = 1; c < GetSquaresUnder(); ++c)
        {
          int Mod = GetSquareUnder(c)->GetRestModifier();

          if(Mod < Lowest)
            Lowest = Mod;
        }

        Bonus = Lowest << 1;
      }
    }
    else if(Action->IsUnconsciousness())
      Bonus = 2;
  }

  int Plus1 = 100;

  switch(GetBurdenState())
  {
   case OVER_LOADED:
    Plus1 = 25;
    break;
   case STRESSED:
    Plus1 = 50;
    break;
   case BURDENED:
    Plus1 = 75;
    break;
  }

  int Plus2 = 100;

  if(IsPlayer())
    switch(GetHungerState())
    {
     case STARVING:
      Plus2 = 25;
      break;
     case VERY_HUNGRY:
      Plus2 = 50;
      break;
     case HUNGRY:
      Plus2 = 75;
      break;
    }

  Stamina += Plus1 * Plus2 * Bonus / 1000;

  if(Stamina > MaxStamina)
    Stamina = MaxStamina;

  if(IsPlayer() && StateIsActivated(PANIC)
     && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);
}

void character::BeginPanic()
{
  if(IsPlayer() && GetTirednessState() != FAINTING)
    game::SetPlayerIsRunning(true);

  DeActivateVoluntaryAction();
}

void character::EndPanic()
{
  if(IsPlayer())
    game::SetPlayerIsRunning(false);
}

int character::GetTirednessState() const
{
  if(Stamina >= MaxStamina >> 2)
    return UNTIRED;
  else if(Stamina >= MaxStamina >> 3)
    return EXHAUSTED;
  else
    return FAINTING;
}

void character::ReceiveBlackUnicorn(long Amount)
{
  if(!(RAND() % 160))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);

  for(int c = 0; c < STATES; ++c)
    if(StateData[c].Flags & DUR_TEMPORARY)
    {
      BeginTemporaryState(1 << c, Amount / 100);

      if(!IsEnabled())
        return;
    }
    else if(StateData[c].Flags & DUR_PERMANENT)
    {
      GainIntrinsic(1 << c);

      if(!IsEnabled())
        return;
    }
}

void character::ReceiveGrayUnicorn(long Amount)
{
  if(!(RAND() % 80))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);

  for(int c = 0; c < STATES; ++c)
    if(1 << c != TELEPORT)
    {
      DecreaseStateCounter(1 << c, -Amount / 100);

      if(!IsEnabled())
        return;
    }
}

void character::ReceiveWhiteUnicorn(long Amount)
{
  if(!(RAND() % 40))
    game::DoEvilDeed(Amount / 50);

  BeginTemporaryState(TELEPORT, Amount / 100);
  DecreaseStateCounter(LYCANTHROPY, -Amount / 100);
  DecreaseStateCounter(POISONED, -Amount / 100);
  DecreaseStateCounter(PARASITE_TAPE_WORM, -Amount / 100);
  DecreaseStateCounter(LEPROSY, -Amount / 100);
  DecreaseStateCounter(VAMPIRISM, -Amount / 100);
}

/* Counter should be negative. Removes intrinsics. */

void character::DecreaseStateCounter(long State, int Counter)
{
  int Index;

  for(Index = 0; Index < STATES; ++Index)
    if(1 << Index == State)
      break;

  if(Index == STATES)
    ABORT("DecreaseTemporaryStateCounter works only when State == 2 ^ n!");

  if(TemporaryState & State)
  {
    if(TemporaryStateCounter[Index] == PERMANENT
       || (TemporaryStateCounter[Index] += Counter) <= 0)
    {
      TemporaryState &= ~State;

      if(!(EquipmentState & State))
      {
        if(StateData[Index].EndHandler)
        {
          (this->*StateData[Index].EndHandler)();

          if(!IsEnabled())
            return;
        }

        (this->*StateData[Index].PrintEndMessage)();
      }
    }
  }
}

truth character::IsImmuneToLeprosy() const
{
  return DataBase->IsImmuneToLeprosy || UseMaterialAttributes() || StateIsActivated(DISEASE_IMMUNITY);
}

void character::LeprosyHandler()
{
  if(!(RAND() % 1000) && IsPlayer())
    ADD_MESSAGE("You notice you're covered in sores!");
  EditExperience(ARM_STRENGTH, -25, 1 << 1);
  EditExperience(LEG_STRENGTH, -25, 1 << 1);
  EditExperience(DEXTERITY, -25, 1 << 1);
  EditExperience(AGILITY, -25, 1 << 1);
  EditExperience(ENDURANCE, -25, 1 << 1);
  EditExperience(CHARISMA, -25, 1 << 1);
  CheckDeath(CONST_S("killed by leprosy"));
}

void character::VampirismHandler()
{
  EditExperience(WISDOM, -25, 1 << 1);
  CheckDeath(CONST_S("killed by vampirism"));
}

bodypart* character::SearchForOriginalBodyPart(int I) const
{
  for(stackiterator i = GetStackUnder()->GetBottom(); i.HasItem(); ++i)
  {
    for(ulong ID : OriginalBodyPartID[I])
      if(i->GetID() == ID)
        return static_cast<bodypart*>(*i);
  }

  return 0;
}

void character::SetLifeExpectancy(int Base, int RandPlus)
{
  int c;

  for(c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
      BodyPart->SetLifeExpectancy(Base, RandPlus);
  }

  for(c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
      Equipment->SetLifeExpectancy(Base, RandPlus);
  }
}

/* Receiver should be a fresh duplicate of this */

void character::DuplicateEquipment(character* Receiver, ulong Flags)
{
  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
    {
      item* Duplicate = Equipment->Duplicate(Flags);
      Receiver->SetEquipment(c, Duplicate);
    }
  }
}

void character::Disappear(corpse* Corpse, cchar* Verb, truth (item::*ClosePredicate)() const)
{
  truth TorsoDisappeared = false;
  truth CanBeSeen = Corpse ? Corpse->CanBeSeenByPlayer() : IsPlayer() || CanBeSeenByPlayer();
  int c;

  if((GetTorso()->*ClosePredicate)())
  {
    if(CanBeSeen)
    {
      if(Corpse)
        ADD_MESSAGE("%s %ss.", Corpse->CHAR_NAME(DEFINITE), Verb);
      else if(IsPlayer())
        ADD_MESSAGE("You %s.", Verb);
      else
        ADD_MESSAGE("%s %ss.", CHAR_NAME(DEFINITE), Verb);
    }

    TorsoDisappeared = true;

    for(c = 0; c < GetEquipments(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment && (Equipment->*ClosePredicate)())
      {
        Equipment->RemoveFromSlot();
        Equipment->SendToHell();
      }
    }

    itemvector ItemVector;
    GetStack()->FillItemVector(ItemVector);

    for(uint c = 0; c < ItemVector.size(); ++c)
      if(ItemVector[c] && (ItemVector[c]->*ClosePredicate)())
      {
        ItemVector[c]->RemoveFromSlot();
        ItemVector[c]->SendToHell();
      }
  }

  for(c = 1; c < GetBodyParts(); ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart)
    {
      if((BodyPart->*ClosePredicate)())
      {
        if(!TorsoDisappeared && CanBeSeen)
        {
          if(IsPlayer())
            ADD_MESSAGE("Your %s %ss.", GetBodyPartName(c).CStr(), Verb);
          else
            ADD_MESSAGE("The %s of %s %ss.", GetBodyPartName(c).CStr(), CHAR_NAME(DEFINITE), Verb);
        }

        BodyPart->DropEquipment();
        item* BodyPart = SevereBodyPart(c);

        if(BodyPart)
          BodyPart->SendToHell();
      }
      else if(TorsoDisappeared)
      {
        BodyPart->DropEquipment();
        item* BodyPart = SevereBodyPart(c);

        if(BodyPart)
        {
          if(Corpse)
            Corpse->GetSlot()->AddFriendItem(BodyPart);
          else if(!game::IsInWilderness())
            GetStackUnder()->AddItem(BodyPart);
          else
            BodyPart->SendToHell();
        }
      }
    }
  }

  if(TorsoDisappeared)
  {
    if(Corpse)
    {
      Corpse->RemoveFromSlot();
      Corpse->SendToHell();
    }
    else
      CheckDeath(festring(Verb) + "ed", 0, FORCE_DEATH|DISALLOW_CORPSE|DISALLOW_MSG);
  }
  else
    CheckDeath(festring(Verb) + "ed", 0, DISALLOW_MSG);
}

void character::SignalDisappearance()
{
  if(GetMotherEntity())
    GetMotherEntity()->SignalDisappearance();
  else
    Disappear(0, "disappear", &item::IsVeryCloseToDisappearance);
}

truth character::HornOfFearWorks() const
{
  return CanHear() && GetPanicLevel() > RAND() % 33 && !StateIsActivated(FEARLESS);
}

void character::BeginLeprosy()
{
  doforbodypartswithparam<truth>()(this, &bodypart::SetIsInfectedByLeprosy, true);
}

void character::EndLeprosy()
{
  doforbodypartswithparam<truth>()(this, &bodypart::SetIsInfectedByLeprosy, false);
}

truth character::IsSameAs(ccharacter* What) const
{
  return What->GetType() == GetType()
    && What->GetConfig() == GetConfig();
}

ulong character::GetCommandFlags() const
{
  return !StateIsActivated(PANIC)
    ? CommandFlags
    : CommandFlags|FLEE_FROM_ENEMIES;
}

ulong character::GetConstantCommandFlags() const
{
  return !StateIsActivated(PANIC)
    ? DataBase->ConstantCommandFlags
    : DataBase->ConstantCommandFlags|FLEE_FROM_ENEMIES;
}

ulong character::GetPossibleCommandFlags() const
{
  int Int = GetAttribute(INTELLIGENCE);
  ulong Flags = ALL_COMMAND_FLAGS;

  if(!CanMove() || Int < 4)
    Flags &= ~FOLLOW_LEADER;

  if(!CanMove() || Int < 6)
    Flags &= ~FLEE_FROM_ENEMIES;

  if(!CanUseEquipment() || Int < 8)
    Flags &= ~DONT_CHANGE_EQUIPMENT;

  if(!UsesNutrition() || Int < 8)
    Flags &= ~DONT_CONSUME_ANYTHING_VALUABLE;

  return Flags;
}

truth character::IsRetreating() const
{
  return StateIsActivated(PANIC)
    || (CommandFlags & FLEE_FROM_ENEMIES && IsPet());
}

truth character::ChatMenu()
{
  if(GetAction() && !GetAction()->CanBeTalkedTo())
  {
    ADD_MESSAGE("%s is silent.", CHAR_DESCRIPTION(DEFINITE));
    PLAYER->EditAP(-200);
    return true;
  }

  ulong ManagementFlags = GetManagementFlags();

  if(ManagementFlags == CHAT_IDLY || !IsPet())
    return ChatIdly();

  static cchar*const ChatMenuEntry[CHAT_MENU_ENTRIES] =
    {
      "Change equipment",
      "Take items",
      "Give items",
      "Issue commands",
      "Chat idly",
    };

  static const petmanagementfunction PMF[CHAT_MENU_ENTRIES] =
    {
      &character::ChangePetEquipment,
      &character::TakePetItems,
      &character::GivePetItems,
      &character::IssuePetCommands,
      &character::ChatIdly
    };

  felist List(CONST_S("Choose action:"));
  game::SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  int c, i;

  for(c = 0; c < CHAT_MENU_ENTRIES; ++c)
    if(1 << c & ManagementFlags)
      List.AddEntry(ChatMenuEntry[c], LIGHT_GRAY);

  int Chosen = List.Draw();

  if(Chosen & FELIST_ERROR_BIT)
    return false;

  for(c = 0, i = 0; c < CHAT_MENU_ENTRIES; ++c)
    if(1 << c & ManagementFlags && i++ == Chosen)
      return (this->*PMF[c])();

  return false; // dummy
}

truth character::ChangePetEquipment()
{
  game::RegionListItemEnable(true);
  game::RegionSilhouetteEnable(true);
  bool b = EquipmentScreen(PLAYER->GetStack(), GetStack());
  game::RegionListItemEnable(false);
  game::RegionSilhouetteEnable(false);

  if(b)
  {
    DexterityAction(3);
    return true;
  }

  return false;
}

truth character::TakePetItems()
{
  truth Success = false;
  stack::SetSelected(0);

  for(;;)
  {
    itemvector ToTake;
    game::DrawEverythingNoBlit();

    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true);
    GetStack()->DrawContents(ToTake,
                             0,
                             PLAYER,
                             CONST_S("What do you want to take from ") + CHAR_DESCRIPTION(DEFINITE) + '?',
                             CONST_S(""),
                             CONST_S(""),
                             GetDescription(DEFINITE) + " is " + GetVerbalBurdenState(),
                             GetVerbalBurdenStateColor(),
                             REMEMBER_SELECTED);
    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);

    if(ToTake.empty())
      break;

    for(uint c = 0; c < ToTake.size(); ++c)
      ToTake[c]->MoveTo(PLAYER->GetStack());

    ADD_MESSAGE("You take %s.", ToTake[0]->GetName(DEFINITE, ToTake.size()).CStr());
    Success = true;
  }

  if(Success)
  {
    DexterityAction(2);
    PLAYER->DexterityAction(2);
  }

  return Success;
}

truth character::GivePetItems()
{
  truth Success = false;
  stack::SetSelected(0);

  for(;;)
  {
    itemvector ToGive;
    game::DrawEverythingNoBlit();

    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true);
    PLAYER->GetStack()->DrawContents(ToGive,
                                     0,
                                     this,
                                     CONST_S("What do you want to give to ") + CHAR_DESCRIPTION(DEFINITE) + '?',
                                     CONST_S(""),
                                     CONST_S(""),
                                     GetDescription(DEFINITE) + " is " + GetVerbalBurdenState(),
                                     GetVerbalBurdenStateColor(),
                                     REMEMBER_SELECTED);
    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);

    if(ToGive.empty())
      break;

    for(uint c = 0; c < ToGive.size(); ++c)
      ToGive[c]->MoveTo(GetStack());

    ADD_MESSAGE("You give %s to %s.", ToGive[0]->GetName(DEFINITE, ToGive.size()).CStr(), CHAR_DESCRIPTION(DEFINITE));
    Success = true;
  }

  if(Success)
  {
    DexterityAction(2);
    PLAYER->DexterityAction(2);
  }

  return Success;
}

truth character::IssuePetCommands()
{
  if(!IsConscious())
  {
    ADD_MESSAGE("%s is unconscious.", CHAR_DESCRIPTION(DEFINITE));
    return false;
  }

  ulong PossibleC = GetPossibleCommandFlags();

  if(!PossibleC)
  {
    ADD_MESSAGE("%s cannot be commanded.", CHAR_DESCRIPTION(DEFINITE));
    return false;
  }

  ulong OldC = GetCommandFlags();
  ulong NewC = OldC, VaryFlags = 0;
  game::CommandScreen(CONST_S("Issue commands to ") + GetDescription(DEFINITE),
                      PossibleC, GetConstantCommandFlags(), VaryFlags, NewC);

  if(NewC == OldC)
    return false;

  SetCommandFlags(NewC);
  PLAYER->EditAP(-500);
  PLAYER->EditExperience(CHARISMA, 25, 1 << 7);
  return true;
}

truth character::ChatIdly()
{
  if(!TryToTalkAboutScience())
  {
    BeTalkedTo();
    PLAYER->EditExperience(CHARISMA, 75, 1 << 7);
  }

  PLAYER->EditAP(-1000);
  return true;
}

truth character::EquipmentScreen(stack* MainStack, stack* SecStack)
{
  if(!CanUseEquipment())
  {
    ADD_MESSAGE("%s cannot use equipment.", CHAR_DESCRIPTION(DEFINITE));
    return false;
  }

  int Chosen = 0;
  truth EquipmentChanged = false;
  felist List(CONST_S("Equipment menu [ESC exits]"));
  festring Entry;
  long TotalEquippedWeight;

  for(;;)
  {
    List.Empty();
        List.EmptyDescription();

        TotalEquippedWeight = 0;

        for(int c = 0; c < GetEquipments(); ++c) // if equipment exists, add to TotalEquippedWeight
        {
                item* Equipment = GetEquipment(c);
                TotalEquippedWeight += (Equipment) ? Equipment->GetWeight() : 0;
        }

        if(IsPlayer())
        {
                festring Total("Total weight: ");
                Total << TotalEquippedWeight;
                Total << "g";

                List.AddDescription(CONST_S(""));
                List.AddDescription(Total);
        }

    if(!IsPlayer())
    {
      List.AddDescription(CONST_S(""));
      List.AddDescription(festring(GetDescription(DEFINITE) + " is " + GetVerbalBurdenState()).CapitalizeCopy(),
                          GetVerbalBurdenStateColor());
    }

    for(int c = 0; c < GetEquipments(); ++c)
    {
      Entry = GetEquipmentName(c);
      Entry << ':';
      Entry.Resize(20);
      item* Equipment = GetEquipment(c);

      if(Equipment)
      {
        Equipment->AddInventoryEntry(this, Entry, 1, true);
        AddSpecialEquipmentInfo(Entry, c);
        int ImageKey = game::AddToItemDrawVector(itemvector(1, Equipment));
        List.AddEntry(Entry, LIGHT_GRAY, 20, ImageKey, true);
      }
      else
      {
        Entry << (GetBodyPartOfEquipment(c) ? "-" : "can't use");
        List.AddEntry(Entry, LIGHT_GRAY, 20, game::AddToItemDrawVector(itemvector()));
      }
    }

    game::DrawEverythingNoBlit();
    game::SetStandardListAttributes(List);
    List.SetFlags(SELECTABLE|DRAW_BACKGROUND_AFTERWARDS);
    List.SetEntryDrawer(game::ItemEntryDrawer);
    Chosen = List.Draw();
    game::ClearItemDrawVector();

    if(Chosen >= GetEquipments())
      break;

    EquipmentChanged = TryToChangeEquipment(MainStack, SecStack, Chosen);
  }

  if(EquipmentChanged)
    DexterityAction(5);

  return EquipmentChanged;
}

ulong character::GetManagementFlags() const
{
  ulong Flags = ALL_MANAGEMENT_FLAGS;

  if(!CanUseEquipment() || !AllowPlayerToChangeEquipment())
    Flags &= ~CHANGE_EQUIPMENT;

  if(!GetStack()->GetItems())
    Flags &= ~TAKE_ITEMS;

  if(!WillCarryItems())
    Flags &= ~GIVE_ITEMS;

  if(!GetPossibleCommandFlags())
    Flags &= ~ISSUE_COMMANDS;

  return Flags;
}

cchar* VerbalBurdenState[] = { "overloaded", "stressed", "burdened", "unburdened" };
col16 VerbalBurdenStateColor[] = { RED, BLUE, BLUE, WHITE };

cchar* character::GetVerbalBurdenState() const
{
  return VerbalBurdenState[BurdenState];
}

col16 character::GetVerbalBurdenStateColor() const
{
  return VerbalBurdenStateColor[BurdenState];
}

int character::GetAttributeAverage() const
{
  return GetSumOfAttributes() / 7;
}

cfestring& character::GetStandVerb() const
{
  if(ForceCustomStandVerb())
    return DataBase->StandVerb;

  static festring Hovering = "hovering";
  static festring Swimming = "swimming";

  if(StateIsActivated(LEVITATION))
    return Hovering;

  if(IsSwimming())
    return Swimming;

  return DataBase->StandVerb;
}

truth character::CheckApply() const
{
  if(!CanApply())
  {
    ADD_MESSAGE("This monster type cannot apply.");
    return false;
  }

  return true;
}

void character::EndLevitation()
{
  if(!IsFlying() && GetSquareUnder())
  {
    if(!game::IsInWilderness())
      SignalStepFrom(0);

    if(game::IsInWilderness() || !GetLSquareUnder()->IsFreezed())
      TestWalkability();
  }
}

truth character::CanMove() const
{
  return !IsRooted() || StateIsActivated(LEVITATION);
}

void character::CalculateEnchantments()
{
  doforequipments()(this, &item::CalculateEnchantment);
  GetStack()->CalculateEnchantments();
}

truth character::GetNewFormForPolymorphWithControl(character*& NewForm)
{
  festring Topic, Temp;
  NewForm = 0;

  if(StateIsActivated(POLYMORPH_LOCK))
  {
    ADD_MESSAGE("You feel uncertain about your body for a moment.");
    return false;
  }

  while(!NewForm)
  {
    festring Temp;
    game::RegionListItemEnable(true);
    game::RegionSilhouetteEnable(true); //polymorphing may unequip items, so taking a look at them is good
    int Return = game::DefaultQuestion(Temp, CONST_S("What do you want to become? [press '?' for a list]"),
                                       game::GetDefaultPolymorphTo(), true,
                                       &game::PolymorphControlKeyHandler);
    game::RegionListItemEnable(false);
    game::RegionSilhouetteEnable(false);
    if(Return == ABORTED)
    {
      ADD_MESSAGE("You choose not to polymorph.");
      NewForm = this;
      return false;
    }

    NewForm = protosystem::CreateMonster(Temp);

    if(NewForm)
    {
      if(NewForm->IsSameAs(this))
      {
        delete NewForm;
        ADD_MESSAGE("You choose not to polymorph.");
        NewForm = this;
        return false;
      }

      if(PolymorphBackup && NewForm->IsSameAs(PolymorphBackup))
      {
        delete NewForm;
        NewForm = ForceEndPolymorph();
        return false;
      }

      if(NewForm->GetPolymorphIntelligenceRequirement()
         > GetAttribute(INTELLIGENCE)
         && !game::WizardModeIsActive())
      {
        ADD_MESSAGE("You feel your mind isn't yet powerful enough to call forth the form of %s.",
                    NewForm->CHAR_NAME(INDEFINITE));
        delete NewForm;
        NewForm = 0;
      }
      else
        NewForm->RemoveAllItems();
    }
  }

  return true;
}

liquid* character::CreateSweat(long Volume) const
{
  return liquid::Spawn(GetSweatMaterial(), Volume);
}

truth character::TeleportRandomItem(truth TryToHinderVisibility)
{
  if(IsImmuneToItemTeleport() || StateIsActivated(TELEPORT_LOCK))
    return false;

  itemvector ItemVector;
  std::vector<long> PossibilityVector;
  int TotalPossibility = 0;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
  {
    ItemVector.push_back(*i);
    int Possibility = i->GetTeleportPriority();

    if(TryToHinderVisibility)
      Possibility += i->GetHinderVisibilityBonus(this);

    PossibilityVector.push_back(Possibility);
    TotalPossibility += Possibility;
  }

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment)
    {
      ItemVector.push_back(Equipment);
      int Possibility = Equipment->GetTeleportPriority();

      if(TryToHinderVisibility)
        Possibility += Equipment->GetHinderVisibilityBonus(this);

      PossibilityVector.push_back(Possibility <<= 1);
      TotalPossibility += Possibility;
    }
  }

  if(!TotalPossibility)
    return false;

  int Chosen = femath::WeightedRand(PossibilityVector, TotalPossibility);
  item* Item = ItemVector[Chosen];
  truth Equipped = PLAYER->Equips(Item);
  truth Seen = Item->CanBeSeenByPlayer();
  Item->RemoveFromSlot();

  if(Seen)
    ADD_MESSAGE("%s disappears.", Item->CHAR_NAME(DEFINITE));

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment lost! [press any key to continue]"));

  v2 Pos = GetPos();
  int Range = Item->GetEmitation() && TryToHinderVisibility ? 25 : 5;
  rect Border(Pos + v2(-Range, -Range), Pos + v2(Range, Range));
  Pos = GetLevel()->GetRandomSquare(this, 0, &Border);

  if(Pos == ERROR_V2)
    Pos = GetLevel()->GetRandomSquare();

  GetNearLSquare(Pos)->GetStack()->AddItem(Item);

  if(Item->CanBeSeenByPlayer())
    ADD_MESSAGE("%s appears.", Item->CHAR_NAME(INDEFINITE));

  return true;
}

truth character::HasClearRouteTo(v2 Pos) const
{
  pathcontroller::Map = GetLevel()->GetMap();
  pathcontroller::Character = this;
  v2 ThisPos = GetPos();
  return mapmath<pathcontroller>::DoLine(ThisPos.X, ThisPos.Y, Pos.X, Pos.Y, SKIP_FIRST);
}

truth character::IsTransparent() const
{
  return !IsEnormous() || GetTorso()->GetMainMaterial()->IsTransparent() || StateIsActivated(INVISIBLE);
}

void character::SignalPossibleTransparencyChange()
{
  if(!game::IsInWilderness())
    for(int c = 0; c < SquaresUnder; ++c)
    {
      lsquare* Square = GetLSquareUnder(c);

      if(Square)
        Square->SignalPossibleTransparencyChange();
    }
}

int character::GetCursorData() const
{
  int Bad = 0;
  int Color = game::PlayerIsRunning() ? BLUE_CURSOR : DARK_CURSOR;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPart && BodyPart->IsUsable())
    {
      int ConditionColorIndex = BodyPart->GetConditionColorIndex();

      if((BodyPartIsVital(c) && !ConditionColorIndex)
         || (ConditionColorIndex <= 1 && ++Bad == 2))
        return Color|CURSOR_FLASH;
    }
    else if(++Bad == 2)
      return Color|CURSOR_FLASH;
  }

  Color = game::PlayerIsRunning() ? YELLOW_CURSOR : RED_CURSOR;
  return Bad ? Color|CURSOR_FLASH : Color;
}

void character::TryToName()
{
  if(!IsPet())
    ADD_MESSAGE("%s refuses to let YOU decide what %s's called.", CHAR_NAME(DEFINITE), CHAR_PERSONAL_PRONOUN);
  else if(IsPlayer())
    ADD_MESSAGE("You can't rename yourself.");
  else if(!IsNameable())
    ADD_MESSAGE("%s refuses to be called anything else but %s.", CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
  else
  {
    festring Topic = CONST_S("What name will you give to ") + GetName(DEFINITE) + '?';
    festring Name;

    if(game::StringQuestion(Name, Topic, WHITE, 0, 80, true) == NORMAL_EXIT && Name.GetSize())
      SetAssignedName(Name);
  }
}

double character::GetSituationDanger(ccharacter* Enemy, v2 ThisPos, v2 EnemyPos, truth SeesEnemy) const
{
  double Danger;

  if(IgnoreDanger() && !IsPlayer())
  {
    if(Enemy->IgnoreDanger() && !Enemy->IsPlayer())
    {
      Danger = double(GetHP()) * GetHPRequirementForGeneration()
               / (Enemy->GetHP() * Enemy->GetHPRequirementForGeneration());
    }
    else
      Danger = .25 * GetHPRequirementForGeneration() / Enemy->GetHP();
  }
  else if(Enemy->IgnoreDanger() && !Enemy->IsPlayer())
    Danger = 4. * GetHP() / Enemy->GetHPRequirementForGeneration();
  else
    Danger = GetRelativeDanger(Enemy);

  Danger *= 3. / ((EnemyPos - ThisPos).GetManhattanLength() + 2);

  if(!SeesEnemy)
    Danger *= .2;

  if(StateIsActivated(PANIC))
    Danger *= .2;

  Danger *= double(GetHP()) * Enemy->GetMaxHP() / (Enemy->GetHP() * GetMaxHP());
  return Danger;
}

void character::ModifySituationDanger(double& Danger) const
{
  switch(GetTirednessState())
  {
   case FAINTING:
    Danger *= 1.5;
   case EXHAUSTED:
    Danger *= 1.25;
  }

  for(int c = 0; c < STATES; ++c)
    if(StateIsActivated(1 << c) && StateData[c].SituationDangerModifier != 0)
      (this->*StateData[c].SituationDangerModifier)(Danger);
}

void character::LycanthropySituationDangerModifier(double& Danger) const
{
  character* Wolf = werewolfwolf::Spawn();
  double DangerToWolf = GetRelativeDanger(Wolf);
  Danger *= pow(DangerToWolf, 0.1);
  delete Wolf;
}

void character::PoisonedSituationDangerModifier(double& Danger) const
{
  int C = GetTemporaryStateCounter(POISONED);
  Danger *= (1 + (C * C) / (GetHP() * 10000. * (GetGlobalResistance(POISON) + 1)));
}

void character::PolymorphingSituationDangerModifier(double& Danger) const
{
  if((!StateIsActivated(POLYMORPH_CONTROL)) && (!StateIsActivated(POLYMORPH_LOCK)))
    Danger *= 1.5;
}

void character::PanicSituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::ConfusedSituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::ParasitizedSituationDangerModifier(double& Danger) const
{
  Danger *= 1.25;
}

void character::LeprosySituationDangerModifier(double& Danger) const
{
  Danger *= 1.5;
}

void character::AddRandomScienceName(festring& String) const
{
  festring Science = GetScienceTalkName().GetRandomElement().CStr();

  if(Science[0] == '!')
  {
    String << Science.CStr() + 1;
    return;
  }

  festring Attribute = GetScienceTalkAdjectiveAttribute().GetRandomElement();
  festring Prefix;
  truth NoAttrib = Attribute.IsEmpty(), NoSecondAdjective = false;

  if(!Attribute.IsEmpty() && Attribute[0] == '!')
  {
    NoSecondAdjective = true;
    Attribute.Erase(0, 1);
  }

  if(!Science.Find("the "))
  {
    Science.Erase(0, 4);

    if(!Attribute.Find("the ", 0, 4))
      Attribute << " the";
    else
      Attribute.Insert(0, "the ", 4);
  }

  if(islower(Science[0])
     && Science.Find(' ') == festring::NPos
     && Science.Find('-') == festring::NPos
     && Science.Find("phobia") == festring::NPos)
  {
    Prefix = GetScienceTalkPrefix().GetRandomElement();

    if(!Prefix.IsEmpty() && Science.Find(Prefix) != festring::NPos)
      Prefix.Empty();
  }

  if(!NoAttrib && !NoSecondAdjective == !RAND_GOOD(3))
  {
    int S1 = NoSecondAdjective ? 0 : GetScienceTalkAdjectiveAttribute().Size;
    int S2 = GetScienceTalkSubstantiveAttribute().Size;
    festring OtherAttribute;
    int Chosen = RAND_GOOD(S1 + S2);

    if(Chosen < S1)
      OtherAttribute = GetScienceTalkAdjectiveAttribute()[Chosen];
    else
      OtherAttribute = GetScienceTalkSubstantiveAttribute()[Chosen - S1];

    if(!OtherAttribute.IsEmpty() && OtherAttribute.Find("the ", 0, 4)
       && Attribute.Find(OtherAttribute) == festring::NPos)
    {
      String << Attribute << ' ' << OtherAttribute << ' ' << Prefix << Science;
      return;
    }
  }

  String << Attribute;

  if(!NoAttrib)
    String << ' ';

  String << Prefix << Science;
}

truth character::TryToTalkAboutScience()
{
  if(GetRelation(PLAYER) == HOSTILE
     || GetScienceTalkPossibility() <= RAND_GOOD(100)
     || PLAYER->GetAttribute(INTELLIGENCE) < GetScienceTalkIntelligenceRequirement()
     || PLAYER->GetAttribute(WISDOM) < GetScienceTalkWisdomRequirement()
     || PLAYER->GetAttribute(CHARISMA) < GetScienceTalkCharismaRequirement())
    return false;

  festring Science;

  if(RAND_GOOD(3))
    AddRandomScienceName(Science);
  else
  {
    festring S1, S2;
    AddRandomScienceName(S1);
    AddRandomScienceName(S2);

    if(S1.Find(S2) == festring::NPos && S2.Find(S1) == festring::NPos)
    {
      switch(RAND_GOOD(3))
      {
       case 0: Science = "the relation of "; break;
       case 1: Science = "the differences of "; break;
       case 2: Science = "the similarities of "; break;
      }

      Science << S1 << " and " << S2;
    }
    else
      AddRandomScienceName(Science);
  }

  switch((RAND() + GET_TICK()) % 10)
  {
   case 0:
    ADD_MESSAGE("You have a rather pleasant chat about %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 1:
    ADD_MESSAGE("%s explains a few of %s opinions regarding %s to you.",
                CHAR_DESCRIPTION(DEFINITE), CHAR_POSSESSIVE_PRONOUN, Science.CStr());
    break;
   case 2:
    ADD_MESSAGE("%s reveals a number of %s insightful views of %s to you.",
                CHAR_DESCRIPTION(DEFINITE), CHAR_POSSESSIVE_PRONOUN, Science.CStr());
    break;
   case 3:
    ADD_MESSAGE("You exchange some information pertaining to %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 4:
    ADD_MESSAGE("You engage in a pretty intriguing conversation about %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 5:
    ADD_MESSAGE("You discuss at length about %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 6:
    ADD_MESSAGE("You have a somewhat boring talk concerning %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 7:
    ADD_MESSAGE("You are drawn into a heated argument regarding %s with %s.",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
   case 8:
    ADD_MESSAGE("%s delivers a long monologue concerning eg. %s.",
                CHAR_DESCRIPTION(DEFINITE), Science.CStr());
    break;
   case 9:
    ADD_MESSAGE("You dive into a brief but thought-provoking debate over %s with %s",
                Science.CStr(), CHAR_DESCRIPTION(DEFINITE));
    break;
  }

  PLAYER->EditExperience(INTELLIGENCE, 1000, 50. * GetScienceTalkIntelligenceModifier() / ++ScienceTalks);
  PLAYER->EditExperience(WISDOM, 1000, 50. * GetScienceTalkWisdomModifier() / ++ScienceTalks);
  PLAYER->EditExperience(CHARISMA, 1000, 50. * GetScienceTalkCharismaModifier() / ++ScienceTalks);
  return true;
}

truth character::IsUsingWeaponOfCategory(int Category) const
{
  return ((GetMainWielded() && GetMainWielded()->GetWeaponCategory() == Category)
          || (GetSecondaryWielded() && GetSecondaryWielded()->GetWeaponCategory() == Category));
}

truth character::TryToUnStickTraps(v2 Dir)
{
  if(!TrapData)
    return true;

  std::vector<trapdata> TrapVector;

  for(const trapdata* T = TrapData; T; T = T->Next)
    TrapVector.push_back(*TrapData);

  for(uint c = 0; c < TrapVector.size(); ++c)
    if(IsEnabled())
    {
      entity* Trap = game::SearchTrap(TrapVector[c].TrapID);

      if(Trap->GetVictimID() == GetID() && Trap->TryToUnStick(this, Dir))
        break;
    }

  return !TrapData && IsEnabled();
}

struct trapidcomparer
{
  trapidcomparer(ulong ID) : ID(ID) { }
  truth operator()(const trapdata* T) const { return T->TrapID == ID; }
  ulong ID;
};

void character::RemoveTrap(ulong ID)
{
  trapdata*& T = ListFind(TrapData, trapidcomparer(ID));
  trapdata* ToDel = T;
  T = T->Next;
  delete ToDel;
  doforbodyparts()(this, &bodypart::SignalPossibleUsabilityChange);
}

void character::AddTrap(ulong ID, ulong BodyParts)
{
  trapdata*& T = ListFind(TrapData, trapidcomparer(ID));

  if(T)
    T->BodyParts |= BodyParts;
  else
    T = new trapdata(ID, GetID(), BodyParts);

  doforbodyparts()(this, &bodypart::SignalPossibleUsabilityChange);
}

truth character::IsStuckToTrap(ulong ID) const
{
  for(const trapdata* T = TrapData; T; T = T->Next)
    if(T->TrapID == ID)
      return true;

  return false;
}

void character::RemoveTraps()
{
  for(trapdata* T = TrapData; T;)
  {
    entity* Trap = game::SearchTrap(T->TrapID);

    if(Trap)
      Trap->UnStick();

    trapdata* ToDel = T;
    T = T->Next;
    delete ToDel;
  }

  TrapData = 0;
  doforbodyparts()(this, &bodypart::SignalPossibleUsabilityChange);
}

void character::RemoveTraps(int BodyPartIndex)
{
  ulong Flag = 1 << BodyPartIndex;

  for(trapdata** T = &TrapData; *T;)
    if((*T)->BodyParts & Flag)
    {
      entity* Trap = game::SearchTrap((*T)->TrapID);

      if(!((*T)->BodyParts &= ~Flag))
      {
        if(Trap)
          Trap->UnStick();

        trapdata* ToDel = *T;
        *T = (*T)->Next;
        delete ToDel;
      }
      else
      {
        if(Trap)
          Trap->UnStick(BodyPartIndex);

        T = &(*T)->Next;
      }
    }
    else
      T = &(*T)->Next;

  if(GetBodyPart(BodyPartIndex))
    GetBodyPart(BodyPartIndex)->SignalPossibleUsabilityChange();
}

festring character::GetTrapDescription() const
{
  festring Desc;
  std::pair<entity*, int> TrapStack[3];
  int Index = 0;

  for(const trapdata* T = TrapData; T; T = T->Next)
  {
    if(Index < 3)
    {
      entity* Trap = game::SearchTrap(T->TrapID);

      if(Trap)
      {
        int c;

        for(c = 0; c < Index; ++c)
          if(TrapStack[c].first->GetTrapType() == Trap->GetTrapType())
            ++TrapStack[c].second;

        if(c == Index)
          TrapStack[Index++] = std::make_pair(Trap, 1);
      }
    }
    else
    {
      ++Index;
      break;
    }
  }

  if(Index <= 3)
  {
    TrapStack[0].first->AddTrapName(Desc, TrapStack[0].second);

    if(Index == 2)
    {
      Desc << " and ";
      TrapStack[1].first->AddTrapName(Desc, TrapStack[1].second);
    }
    else if(Index == 3)
    {
      Desc << ", ";
      TrapStack[1].first->AddTrapName(Desc, TrapStack[1].second);
      Desc << " and ";
      TrapStack[2].first->AddTrapName(Desc, TrapStack[2].second);
    }
  }
  else
    Desc << "lots of traps";

  return Desc;
}

int character::RandomizeHurtBodyPart(ulong BodyParts) const
{
  int BodyPartIndex[MAX_BODYPARTS];
  int Index = 0;

  for(int c = 0; c < GetBodyParts(); ++c)
    if(1 << c & BodyParts)
      BodyPartIndex[Index++] = c;

  return BodyPartIndex[RAND_N(Index)];
}

truth character::BodyPartIsStuck(int I) const
{
  for(const trapdata* T = TrapData; T; T = T->Next)
    if(1 << I & T->BodyParts)
      return true;

  return false;
}

void character::PrintAttribute(cchar* Desc, int I, int PanelPosX, int PanelPosY) const
{
  int Attribute = GetAttribute(I);
  int NoBonusAttribute = GetAttribute(I, false);
  col16 C = game::GetAttributeColor(I);
  festring String = Desc;
  String.Resize(5);
  String << Attribute;
  String.Resize(8);
  FONT->Printf(DOUBLE_BUFFER, v2(PanelPosX, PanelPosY * 10), C, String.CStr());

  if(Attribute != NoBonusAttribute)
  {
    int Where = PanelPosX + ((String.GetSize() + 1) << 3);
    FONT->Printf(DOUBLE_BUFFER, v2(Where, PanelPosY * 10), LIGHT_GRAY,
                 "%d", NoBonusAttribute);
  }
}

truth character::AllowUnconsciousness() const
{
  return DataBase->AllowUnconsciousness && TorsoIsAlive();
}

truth character::CanPanic() const
{
  return !Action || !Action->IsUnconsciousness() || !StateIsActivated(FEARLESS);
}

int character::GetRandomBodyPart(ulong Possible) const
{
  int OKBodyPart[MAX_BODYPARTS];
  int OKBodyParts = 0;

  for(int c = 0; c < BodyParts; ++c)
    if(1 << c & Possible && GetBodyPart(c))
      OKBodyPart[OKBodyParts++] = c;

  return OKBodyParts ? OKBodyPart[RAND_N(OKBodyParts)] : NONE_INDEX;
}

void character::EditNP(long What)
{
  int OldState = GetHungerState();
  NP += What;
  int NewState = GetHungerState();

  if(OldState > VERY_HUNGRY && NewState == VERY_HUNGRY)
    DeActivateVoluntaryAction(CONST_S("You are getting really hungry."));

  if(OldState > STARVING && NewState == STARVING)
    DeActivateVoluntaryAction(CONST_S("You are getting extremely hungry."));
}

truth character::IsSwimming() const
{
  return !IsFlying() && GetSquareUnder()
    && GetSquareUnder()->GetSquareWalkability() & SWIM;
}

void character::AddBlackUnicornConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel dirty and loathsome.");
}

void character::AddGrayUnicornConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel neutralized.");
}

void character::AddWhiteUnicornConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel purified.");
}

void character::AddOmmelBoneConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel the power of all your canine ancestors combining in your body.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("For a moment %s looks extremely ferocious. You shudder.", CHAR_NAME(DEFINITE));
}

void character::AddLiquidHorrorConsumeEndMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Untold horrors flash before your eyes. The melancholy of the world is on your shoulders!");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s looks as if the true terror of existence flashed before %s eyes!.", CHAR_NAME(DEFINITE), GetPossessivePronoun().CStr());
}

int character::GetBodyPartSparkleFlags(int) const
{
  return ((GetNaturalSparkleFlags() & SKIN_COLOR ? SPARKLING_A : 0)
          | (GetNaturalSparkleFlags() & TORSO_MAIN_COLOR ? SPARKLING_B : 0)
          | (GetNaturalSparkleFlags() & TORSO_SPECIAL_COLOR ? SPARKLING_D : 0));
}

truth character::IsAnimated() const
{
  return combinebodypartpredicates()(this, &bodypart::IsAnimated, 1);
}

double character::GetNaturalExperience(int Identifier) const
{
  return DataBase->NaturalExperience[Identifier];
}

truth character::HasBodyPart(sorter Sorter) const
{
  if(Sorter == 0)
    return true;

  return combinebodypartpredicateswithparam<ccharacter*>()(this, Sorter, this, 1);
}

truth character::PossessesItem(sorter Sorter) const
{
  if(Sorter == 0)
    return true;

  return (GetStack()->SortedItems(this, Sorter)
          || combinebodypartpredicateswithparam<ccharacter*>()(this, Sorter, this, 1)
          || combineequipmentpredicateswithparam<ccharacter*>()(this, Sorter, this, 1));
}

/* 0 <= I <= 1 */

cchar* character::GetRunDescriptionLine(int I) const
{
  if(!GetRunDescriptionLineOne().IsEmpty())
    return !I ? GetRunDescriptionLineOne().CStr() : GetRunDescriptionLineTwo().CStr();

  if(IsFlying())
    return !I ? "Flying" : "very fast";

  if(IsSwimming())
    return !I ? "Swimming" : "very fast";

  return !I ? "Running" : "";
}

void character::VomitAtRandomDirection(int Amount)
{
  if(game::IsInWilderness())
    return;

  /* Lacks support of multitile monsters */

  v2 Possible[9];
  int Index = 0;

  for(int d = 0; d < 9; ++d)
  {
    lsquare* Square = GetLSquareUnder()->GetNeighbourLSquare(d);

    if(Square && !Square->VomitingIsDangerous(this))
      Possible[Index++] = Square->GetPos();
  }

  if(Index)
    Vomit(Possible[RAND_N(Index)], Amount);
  else
    Vomit(GetPos(), Amount);
}

void character::RemoveLifeSavers()
{
  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && Equipment->IsInCorrectSlot(c) && Equipment->GetGearStates() & LIFE_SAVED)
    {
      Equipment->SendToHell();
      Equipment->RemoveFromSlot();
    }
  }
}

ccharacter* character::FindCarrier() const
{
  return this; //check
}

void character::PrintBeginHiccupsMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your diaphragm is spasming vehemently.");
}

void character::PrintEndHiccupsMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You feel your annoying hiccoughs have finally subsided.");
}

void character::HiccupsHandler()
{
  /*if(!(RAND() % 2000))
  {
    if(IsPlayer())
      ADD_MESSAGE("");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("");
    else if((PLAYER->GetPos() - GetPos()).GetLengthSquare() <= 400)
      ADD_MESSAGE("");

    game::CallForAttention(GetPos(), 400);
  }*/
}

void character::HiccupsSituationDangerModifier(double& Danger) const
{
  Danger *= 1.25;
}

bool character::IsConscious() const
{
  return !Action || !Action->IsUnconsciousness();
}

wsquare* character::GetNearWSquare(v2 Pos) const
{
  return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(Pos));
}

wsquare* character::GetNearWSquare(int x, int y) const
{
  return static_cast<wsquare*>(GetSquareUnder()->GetArea()->GetSquare(x, y));
}

void character::ForcePutNear(v2 Pos)
{
  /* GUM SOLUTION!!! */

  v2 NewPos = game::GetCurrentLevel()->GetNearestFreeSquare(PLAYER, Pos, false);

  if(NewPos == ERROR_V2)
    do
    {
      NewPos = game::GetCurrentLevel()->GetRandomSquare(this);
    }
    while(NewPos == Pos);

  PutTo(NewPos);
}

void character::ReceiveMustardGas(int BodyPart, long Volume)
{
  if(Volume)
    GetBodyPart(BodyPart)->AddFluid(liquid::Spawn(MUSTARD_GAS_LIQUID, Volume), CONST_S("skin"), 0, true);
}

void character::ReceiveMustardGasLiquid(int BodyPartIndex, long Modifier)
{
  bodypart* BodyPart = GetBodyPart(BodyPartIndex);

  if(BodyPart->GetMainMaterial()->GetInteractionFlags() & IS_AFFECTED_BY_MUSTARD_GAS)
  {
    long Tries = Modifier;
    Modifier -= Tries; //opt%?
    int Damage = 0;

    for(long c = 0; c < Tries; ++c)
      if(!(RAND() % 100))
        ++Damage;

    if(Modifier && !(RAND() % 1000 / Modifier))
      ++Damage;

    if(Damage)
    {
      ulong Minute = game::GetTotalMinutes();

      if(GetLastAcidMsgMin() != Minute && (CanBeSeenByPlayer() || IsPlayer()))
      {
        SetLastAcidMsgMin(Minute);

        if(IsPlayer())
          ADD_MESSAGE("Mustard gas dissolves the skin of your %s.",
                      BodyPart->GetBodyPartName().CStr());
        else
          ADD_MESSAGE("Mustard gas dissolves %s.", CHAR_NAME(DEFINITE));
      }

      ReceiveBodyPartDamage(0, Damage, MUSTARD_GAS_DAMAGE,
                            BodyPartIndex, YOURSELF, false, false, false);
      CheckDeath(CONST_S("killed by a fatal exposure to mustard gas"));
    }
  }
}

truth character::IsBadPath(v2 Pos) const
{
  if(!IsGoingSomeWhere())
    return false;

  v2 TPos = !Route.empty() ? Route.back() : GoingTo;

  return ((TPos - Pos).GetManhattanLength()
          > (TPos - GetPos()).GetManhattanLength());
}

double& character::GetExpModifierRef(expid E)
{
  return ExpModifierMap.insert(std::make_pair(E, 1.)).first->second;
}

/* Should probably do more. Now only makes Player forget gods */
truth character::ForgetRandomThing()
{
  if(IsPlayer())
  {
    /* hopefully this code isn't some where else */
    std::vector<god*> Known;

    for(int c = 1; c <= GODS; ++c)
      if(game::GetGod(c)->IsKnown())
        Known.push_back(game::GetGod(c));

    if(Known.empty())
      return false;

    int RandomGod = RAND_N(Known.size());
    Known.at(RAND_N(Known.size()))->SetIsKnown(false);
    ADD_MESSAGE("You forget how to pray to %s.",
                Known.at(RandomGod)->GetName());
    return true;
  }
  return false;
}

int character::CheckForBlock(character* Enemy, item* Weapon,
                             double ToHitValue, int Damage,
                             int Success, int Type)
{
  return Damage;
}

void character::ApplyAllGodsKnownBonus()
{
  stack* AddPlace = GetStackUnder();

  if(game::IsInWilderness())
    AddPlace = GetStack();
  else
    AddPlace = GetStackUnder();

  pantheonbook* NewBook = pantheonbook::Spawn();
  AddPlace->AddItem(NewBook);

  ADD_MESSAGE("\"MORTAL! BEHOLD THE HOLY SAGA!\"");
  ADD_MESSAGE("%s materializes near your feet.",
              NewBook->CHAR_NAME(INDEFINITE));
}

truth character::ReceiveSirenSong(character* Siren)
{
  if(Siren->GetRelation(this) != HOSTILE)
    return false;

  if(!RAND_N(4))
  {
    if(IsPlayer())
      ADD_MESSAGE("The beautiful melody of %s makes you feel sleepy.", Siren->CHAR_NAME(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("The beautiful melody of %s makes %s look sleepy.", Siren->CHAR_NAME(DEFINITE), CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("You hear a beautiful song.");

    Stamina -= (1 + RAND_N(4)) * 10000;
    return true;
  }

  if(!IsPlayer() && IsCharmable() && CanTameWithDulcis(Siren) && !RAND_N(5))
  {
    ChangeTeam(Siren->GetTeam());

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s seems to be totally brainwashed by %s melodies.", CHAR_NAME(DEFINITE), Siren->CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("You hear a beautiful song.");

    return true;
  }

  if(!IsImmuneToWhipOfThievery() && !RAND_N(4))
  {
    item* What = GiveMostExpensiveItem(Siren);

    if(What)
    {
      if(IsPlayer())
        ADD_MESSAGE("%s song persuades you to give %s to %s as a present.",
                    Siren->CHAR_NAME(DEFINITE), What->CHAR_NAME(DEFINITE), Siren->CHAR_OBJECT_PRONOUN);
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s is persuaded to give %s to %s because of %s beautiful singing.",
                    CHAR_NAME(DEFINITE), What->CHAR_NAME(INDEFINITE), Siren->CHAR_NAME(DEFINITE), Siren->CHAR_OBJECT_PRONOUN);
      else
        ADD_MESSAGE("You hear a beautiful song.");
    }
    else
    {
      if(IsPlayer())
        ADD_MESSAGE("You would like to give something to %s.", Siren->CHAR_NAME(DEFINITE));
      else
        ADD_MESSAGE("You hear a beautiful song.");
    }
    return true;
  }
  return false;
}

// return 0, if no item found

item* character::FindMostExpensiveItem() const
{
  int MaxPrice = -1;
  item* MostExpensive = 0;

  for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
    if((*i)->GetPrice() > MaxPrice)
    {
      MaxPrice = (*i)->GetPrice();
      MostExpensive = (*i);
    }

  for(int c = 0; c < GetEquipments(); ++c)
  {
    item* Equipment = GetEquipment(c);

    if(Equipment && Equipment->GetPrice() > MaxPrice)
    {
      MaxPrice = Equipment->GetPrice();
      MostExpensive = Equipment;
    }
  }

  return MostExpensive;
}

// returns 0 if no items available

item* character::GiveMostExpensiveItem(character* ToWhom)
{
  item* ToGive = FindMostExpensiveItem();

  if(!ToGive)
    return 0;

  truth Equipped = PLAYER->Equips(ToGive);
  ToGive->RemoveFromSlot();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment lost! [press any key to continue]"));

  ToWhom->ReceiveItemAsPresent(ToGive);
  EditAP(-1000);
  return ToGive;
}

void character::ReceiveItemAsPresent(item* Present)
{
  if(TestForPickup(Present))
    GetStack()->AddItem(Present);
  else
    GetStackUnder()->AddItem(Present);
}

/* returns 0 if no enemies in sight */

character* character::GetNearestEnemy() const
{
  character* NearestEnemy = 0;
  long NearestEnemyDistance = 0x7FFFFFFF;
  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X),
                                        abs(p->GetPos().Y - Pos.Y));

          if((ThisDistance < NearestEnemyDistance
              || (ThisDistance == NearestEnemyDistance && !(RAND() % 3)))
             && p->CanBeSeenBy(this))
          {
            NearestEnemy = p;
            NearestEnemyDistance = ThisDistance;
          }
        }
    }

  return NearestEnemy;
}

void character::PrintBeginMindwormedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("You have a killer headache.");
}

void character::PrintEndMindwormedMessage() const
{
  if(IsPlayer())
    ADD_MESSAGE("Your headache finally subsides.");
}

truth character::MindWormCanPenetrateSkull(mindworm*) const
{
  if(!(RAND() % 2))
    return true;
  else
    return false;
}

void character::MindwormedHandler()
{
  if(!(RAND() % 200))
  {
    if(IsPlayer())
      ADD_MESSAGE("Your brain hurts!");

    BeginTemporaryState(CONFUSED, 100 + RAND_N(100));
    return;
  }

  // Multiple mind worm hatchlings can hatch, because multiple eggs could have been implanted.
  if(!game::IsInWilderness() && !(RAND() % 500))
  {
    character* Spawned = mindworm::Spawn(HATCHLING);
    v2 Pos = game::GetCurrentLevel()->GetNearestFreeSquare(Spawned, GetPos());

    if(Pos == ERROR_V2)
    {
      delete Spawned;
      return;
    }

    Spawned->SetTeam(game::GetTeam(MONSTER_TEAM));
    Spawned->PutTo(Pos);

    if(IsPlayer())
    {
      ADD_MESSAGE("%s suddenly digs out of your skull.", Spawned->CHAR_NAME(INDEFINITE));
    }
    else if(CanBeSeenByPlayer())
    {
      ADD_MESSAGE("%s suddenly digs out of %s's skull.", Spawned->CHAR_NAME(INDEFINITE), CHAR_NAME(DEFINITE));
    }

    ReceiveDamage(0, 1, DRAIN, HEAD, 8, false, false, false, false); // Use DRAIN here so that AV does not apply.
    CheckDeath(CONST_S("killed by giving birth to ") + Spawned->GetName(INDEFINITE));
  }
}

truth character::CanTameWithDulcis(const character* Tamer) const
{
  int TamingDifficulty = GetTamingDifficulty();

  if(TamingDifficulty == NO_TAMING)
    return false;

  if(GetAttachedGod() == DULCIS)
    return true;

  int Modifier = Tamer->GetAttribute(WISDOM) + Tamer->GetAttribute(CHARISMA);

  if(Tamer->IsPlayer())
    Modifier += game::GetGod(DULCIS)->GetRelation() / 20;
  else if(Tamer->GetAttachedGod() == DULCIS)
    Modifier += 50;

  if(TamingDifficulty == 0)
  {
    if(!IgnoreDanger())
      TamingDifficulty = int(10 * GetRelativeDanger(Tamer));
    else
      TamingDifficulty = 10 * GetHPRequirementForGeneration()
                         / Max(Tamer->GetHP(), 1);
  }

  return Modifier >= TamingDifficulty * 3;
}

truth character::CanTameWithLyre(const character* Tamer) const
{
  int TamingDifficulty = GetTamingDifficulty();

  if(TamingDifficulty == NO_TAMING)
    return false;

  if(TamingDifficulty == 0)
  {
    if(!IgnoreDanger())
      TamingDifficulty = int(10 * GetRelativeDanger(Tamer));
    else
      TamingDifficulty = 10 * GetHPRequirementForGeneration()
                         / Max(Tamer->GetHP(), 1);
  }

  return Tamer->GetAttribute(CHARISMA) >= TamingDifficulty;
}

truth character::CanTameWithScroll(const character* Tamer) const
{
  int TamingDifficulty = GetTamingDifficulty();
  return (TamingDifficulty != NO_TAMING
          && (TamingDifficulty == 0
              || Tamer->GetAttribute(INTELLIGENCE) * 4
              + Tamer->GetAttribute(CHARISMA)
              >= TamingDifficulty * 5));
}

truth character::CanTameWithResurrection(const character* Tamer) const
{
        int TamingDifficulty = GetTamingDifficulty();

        if (TamingDifficulty == NO_TAMING)
                return false;
        if (TamingDifficulty == 0)
                return true;

        return (Tamer->GetAttribute(CHARISMA) >= TamingDifficulty / 2);
        //      || Tamer->GetAttribute(CHARISMA) + WisIntAvg >= (2 * TamingDifficulty) / 3);
                // Alternate formula 2/3 * TamingDifficulty <= CHA + (WIS + INT) / 2
}

truth character::CheckSadism()
{
  if(!IsSadist() || !HasSadistAttackMode() || !IsSmall()) // gum
    return false;

  if(!RAND_N(10))
  {
    for(int d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
      {
        character* Char = Square->GetCharacter();

        if(Char && Char->IsMasochist() && GetRelation(Char) == FRIEND
           && Char->GetHP() * 3 >= Char->GetMaxHP() * 2
           && Hit(Char, Square->GetPos(), d, SADIST_HIT))
        {
          TerminateGoingTo();
          return true;
        }
      }
    }
  }

  return false;
}

truth character::CheckForBeverage()
{
  if(StateIsActivated(PANIC) || !IsEnabled() || !UsesNutrition() || CheckIfSatiated())
    return false;

  itemvector ItemVector;
  GetStack()->FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->IsBeverage(this) && TryToConsume(ItemVector[c]))
      return true;

  return false;
}

void character::Haste()
{
  doforbodyparts()(this, &bodypart::Haste);
  doforequipments()(this, &item::Haste);
  BeginTemporaryState(HASTE, 500 + RAND() % 1000);
}

void character::Slow()
{
  doforbodyparts()(this, &bodypart::Slow);
  doforequipments()(this, &item::Slow);
  BeginTemporaryState(SLOW, 500 + RAND() % 1000);
}

truth character::IsESPBlockedByEquipment() const
{
  for(int c = 0; c < GetEquipments(); ++c)
  {
    item *Item = GetEquipment(c);

    if (Item && Item->IsHelmet(this) &&
        ((Item->GetMainMaterial() && Item->GetMainMaterial()->BlockESP()) ||
         (Item->GetSecondaryMaterial() && Item->GetSecondaryMaterial()->BlockESP())))
      return !(Item->IsBroken());
  }
  return false;
}

truth character::TemporaryStateIsActivated (long What) const
{
  if((What&PANIC) && (TemporaryState&PANIC) && StateIsActivated(FEARLESS))
  {
    return ((TemporaryState&What) & (~PANIC));
  }
  if((What&ESP) && (TemporaryState&ESP) && IsESPBlockedByEquipment())
  {
    return ((TemporaryState&What) & (~ESP));
  }
  return (TemporaryState & What);
}

truth character::StateIsActivated (long What) const
{
  if ((What & PANIC) && ((TemporaryState|EquipmentState) & PANIC) && StateIsActivated(FEARLESS))
  {
    return ((TemporaryState & What) & (~PANIC)) || ((EquipmentState & What) & (~PANIC));
  }
  if ((What & ESP) && ((TemporaryState|EquipmentState) & ESP) && IsESPBlockedByEquipment())
  {
    return ((TemporaryState & What) & (~ESP)) || ((EquipmentState & What) & (~ESP));
  }
  return (TemporaryState & What) || (EquipmentState & What);
}

truth character::CheckAIZapOpportunity()
{
  if(!CanZap() || !IsHumanoid() || !IsEnabled())
    return false;

  // Check visible area for hostiles:
  v2 Pos = GetPos();
  v2 TestPos;
  int SensibleRange = 5;
  int RangeMax = GetLOSRange();

  if(RangeMax < SensibleRange)
    SensibleRange = RangeMax;

  int CandidateDirections[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int HostileFound = 0;

  for(int r = 2; r <= SensibleRange; ++r)
  {
    for(int dir = 0; dir < 8; ++dir)
    {
      switch(dir)
      {
       case 0:
        TestPos = v2(Pos.X - r, Pos.Y - r);
        break;
       case 1:
        TestPos = v2(Pos.X, Pos.Y - r);
        break;
       case 2:
        TestPos = v2(Pos.X + r, Pos.Y - r);
        break;
       case 3:
        TestPos = v2(Pos.X - r, Pos.Y);
        break;
       case 4:
        TestPos = v2(Pos.X + r, Pos.Y);
        break;
       case 5:
        TestPos = v2(Pos.X - r, Pos.Y + r);
        break;
       case 6:
        TestPos = v2(Pos.X, Pos.Y + r);
        break;
       case 7:
        TestPos = v2(Pos.X + r, Pos.Y + r);
        break;
      }

      level* Level = GetLevel();

      if(Level->IsValidPos(TestPos))
      {
        square* TestSquare = GetNearSquare(TestPos);
        character* Dude = TestSquare->GetCharacter();

        if((Dude && Dude->IsEnabled() && (GetRelation(Dude) != HOSTILE)
           && Dude->SquareUnderCanBeSeenBy(this, false)))
        {
          CandidateDirections[dir] = BLOCKED;
        }

        if(Dude && Dude->IsEnabled() && (GetRelation(Dude) == HOSTILE)
           && Dude->SquareUnderCanBeSeenBy(this, false)
           && (CandidateDirections[dir] != BLOCKED))
        {
          CandidateDirections[dir] = SUCCESS;
          HostileFound = 1;
        }
      }
    }
  }

  int ZapDirection = 0;
  int TargetFound = 0;

  if(HostileFound)
  {
    for(uint dir = 0; dir < 8; ++dir)
    {
      if((CandidateDirections[dir] == SUCCESS) && !TargetFound)
      {
        ZapDirection = dir;
        TargetFound = 1;
      }
    }
    if(!TargetFound)
    {
      return false;
    }
  }
  else
    return false;


  // Check inventory for zappable item.
  itemvector ItemVector;
  GetStack()->FillItemVector(ItemVector);
  item* ToBeZapped = 0;

  for(uint c = 0; c < ItemVector.size(); ++c)
    if((ItemVector[c]->GetMinCharges() > 0) && ItemVector[c]->GetPrice()) // Empty wands have zero price!
    {
      ToBeZapped = ItemVector[c];

      if(!(RAND() % 3)) // Do not always pick the first available wand to zap.
        break;
    }

  if(!ToBeZapped)
    return false;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s zaps %s.", CHAR_NAME(DEFINITE), ToBeZapped->CHAR_NAME(INDEFINITE));

  if(ToBeZapped->Zap(this, GetPos(), ZapDirection))
  {
    EditAP(-100000 / APBonus(GetAttribute(PERCEPTION)));
    return true;
  }
  else
    return false;

  TerminateGoingTo(); // Is this useful here? I don't think the code will ever
  return true;        // get down here.

  // Steps:
  // (1) - Acquire target as nearest enemy.
  // (2) - Check that this enemy is in range, and is in appropriate direction.
  //       No friendly fire!
  // (3) - Check inventory for zappable item.
  // (4) - Zap item in direction where the enemy is.
}
