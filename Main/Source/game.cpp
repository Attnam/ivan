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

#include <algorithm>
#include <cstdarg>

#if defined(LINUX) || defined(__DJGPP__)
#include <sys/stat.h>
#endif

#ifdef WIN32
#include <direct.h>
#endif

#include "whandler.h"
#include "hscore.h"
#include "rawbit.h"
#include "message.h"
#include "feio.h"
#include "team.h"
#include "iconf.h"
#include "allocate.h"
#include "pool.h"
#include "god.h"
#include "proto.h"
#include "stack.h"
#include "felist.h"
#include "human.h"
#include "nonhuman.h"
#include "wsquare.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "save.h"
#include "miscitem.h"
#include "room.h"
#include "materias.h"
#include "rain.h"
#include "gear.h"
#include "fetime.h"
#include "balance.h"
#include "confdef.h"

#define SAVE_FILE_VERSION 119 // Increment this if changes make savefiles incompatible
#define BONE_FILE_VERSION 106 // Increment this if changes make bonefiles incompatible

#define LOADED 0
#define NEW_GAME 1
#define BACK 2

int game::CurrentLevelIndex;
truth game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
dungeon** game::Dungeon;
int game::CurrentDungeonIndex;
ulong game::NextCharacterID = 1;
ulong game::NextItemID = 1;
ulong game::NextTrapID = 1;
team** game::Team;
ulong game::LOSTick;
v2 game::CursorPos(-1, -1);
truth game::Zoom;
truth game::Generating = false;
double game::AveragePlayerArmStrengthExperience;
double game::AveragePlayerLegStrengthExperience;
double game::AveragePlayerDexterityExperience;
double game::AveragePlayerAgilityExperience;
int game::Teams;
int game::Dungeons;
int game::StoryState;
massacremap game::PlayerMassacreMap;
massacremap game::PetMassacreMap;
massacremap game::MiscMassacreMap;
long game::PlayerMassacreAmount = 0;
long game::PetMassacreAmount = 0;
long game::MiscMassacreAmount = 0;
boneidmap game::BoneItemIDMap;
boneidmap game::BoneCharacterIDMap;
truth game::TooGreatDangerFoundTruth;
itemvectorvector game::ItemDrawVector;
charactervector game::CharacterDrawVector;
truth game::SumoWrestling;
liquid* game::GlobalRainLiquid;
v2 game::GlobalRainSpeed;
long game::GlobalRainTimeModifier;
truth game::PlayerSumoChampion;
ulong game::SquarePartEmitationTick = 0;
long game::Turn;
truth game::PlayerRunning;
character* game::LastPetUnderCursor;
charactervector game::PetVector;
double game::DangerFound;
int game::OldAttribute[ATTRIBUTES];
int game::NewAttribute[ATTRIBUTES];
int game::LastAttributeChangeTick[ATTRIBUTES];
int game::NecroCounter;
int game::CursorData;
truth game::CausePanicFlag;

truth game::Loading = false;
truth game::JumpToPlayerBe = false;
truth game::InGetCommand = false;
character* game::Petrus = 0;
time_t game::TimePlayedBeforeLastLoad;
time_t game::LastLoad;
time_t game::GameBegan;
truth game::PlayerHasReceivedAllGodsKnownBonus;

festring game::AutoSaveFileName = game::GetSaveDir() + "AutoSave";
cchar* const game::Alignment[] = { "L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--" };
god** game::God;

cint game::MoveNormalCommandKey[] = { KEY_HOME, KEY_UP, KEY_PAGE_UP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, '.' };
cint game::MoveAbnormalCommandKey[] = { '7','8','9','u','o','j','k','l','.' };

cv2 game::MoveVector[] = { v2(-1, -1), v2(0, -1), v2(1, -1), v2(-1, 0), v2(1, 0), v2(-1, 1), v2(0, 1), v2(1, 1), v2(0, 0) };
cv2 game::RelativeMoveVector[] = { v2(-1, -1), v2(1, 0), v2(1, 0), v2(-2, 1), v2(2, 0), v2(-2, 1), v2(1, 0), v2(1, 0), v2(-1, -1) };
cv2 game::BasicMoveVector[] = { v2(-1, 0), v2(1, 0), v2(0, -1), v2(0, 1) };
cv2 game::LargeMoveVector[] = { v2(-1, -1), v2(0, -1), v2(1, -1), v2(2, -1), v2(-1, 0), v2(2, 0), v2(-1, 1), v2(2, 1), v2(-1, 2), v2(0, 2), v2(1, 2), v2(2, 2), v2(0, 0), v2(1, 0), v2(0, 1), v2(1, 1) };
cint game::LargeMoveDirection[] = { 0, 1, 1, 2, 3, 4, 3, 4, 5, 6, 6, 7, 8, 8, 8, 8 };

truth game::LOSUpdateRequested = false;
uchar*** game::LuxTable = 0;
truth game::Running;
character* game::Player;
v2 game::Camera(0, 0);
ulong game::Tick;
gamescript* game::GameScript = 0;
valuemap game::GlobalValueMap;
dangermap game::DangerMap;
int game::NextDangerIDType;
int game::NextDangerIDConfigIndex;
characteridmap game::CharacterIDMap;
itemidmap game::ItemIDMap;
trapidmap game::TrapIDMap;
truth game::PlayerHurtByExplosion;
area* game::CurrentArea;
level* game::CurrentLevel;
wsquare*** game::CurrentWSquareMap;
lsquare*** game::CurrentLSquareMap;
festring game::DefaultPolymorphTo;
festring game::DefaultSummonMonster;
festring game::DefaultWish;
festring game::DefaultChangeMaterial;
festring game::DefaultDetectMaterial;
truth game::WizardMode;
int game::SeeWholeMapCheatMode;
truth game::GoThroughWallsCheat;
int game::QuestMonstersFound;
bitmap* game::BusyAnimationCache[32];
festring game::PlayerName;
ulong game::EquipmentMemory[MAX_EQUIPMENT_SLOTS];
olterrain* game::MonsterPortal;
std::vector<v2> game::SpecialCursorPos;
std::vector<int> game::SpecialCursorData;
cbitmap* game::EnterImage;
v2 game::EnterTextDisplacement;

void game::AddCharacterID(character* Char, ulong ID) {
  if(CharacterIDMap.find(ID) != CharacterIDMap.end())
    int esko = esko = 2;
  CharacterIDMap.insert(std::make_pair(ID, Char));
}
void game::RemoveCharacterID(ulong ID) {
  if(CharacterIDMap.find(ID) == CharacterIDMap.end())
    int esko = esko = 2;
  CharacterIDMap.erase(CharacterIDMap.find(ID));
}
void game::AddItemID(item* Item, ulong ID) {
  if(ItemIDMap.find(ID) != ItemIDMap.end())
    int esko = esko = 2;
  ItemIDMap.insert(std::make_pair(ID, Item));
}
void game::RemoveItemID(ulong ID)
{
  if(ID && ItemIDMap.find(ID) == ItemIDMap.end())
    int esko = esko = 2;

  if(ID) ItemIDMap.erase(ItemIDMap.find(ID));
}
void game::UpdateItemID(item* Item, ulong ID) {
  if(ItemIDMap.find(ID) == ItemIDMap.end())
    int esko = esko = 2;
  ItemIDMap.find(ID)->second = Item;
}
void game::AddTrapID(entity* Trap, ulong ID) {
  if(TrapIDMap.find(ID) != TrapIDMap.end())
    int esko = esko = 2;

  if(ID)
    TrapIDMap.insert(std::make_pair(ID, Trap));
}
void game::RemoveTrapID(ulong ID)
{
  if(ID && TrapIDMap.find(ID) == TrapIDMap.end())
    int esko = esko = 2;

  if(ID) TrapIDMap.erase(TrapIDMap.find(ID));
}
void game::UpdateTrapID(entity* Trap, ulong ID) {
  if(TrapIDMap.find(ID) == TrapIDMap.end())
    int esko = esko = 2;
  TrapIDMap.find(ID)->second = Trap;
}
const dangermap& game::GetDangerMap() { return DangerMap; }
void game::ClearItemDrawVector() { ItemDrawVector.clear(); }
void game::ClearCharacterDrawVector() { CharacterDrawVector.clear(); }

void game::InitScript()
{
  inputfile ScriptFile(GetGameDir() + "Script/dungeon.dat", &GlobalValueMap);
  GameScript = new gamescript;
  GameScript->ReadFrom(ScriptFile);
  GameScript->RandomizeLevels();
}

truth game::Init(cfestring& Name)
{
  if(Name.IsEmpty())
    if(ivanconfig::GetDefaultName().IsEmpty())
    {
      PlayerName.Empty();

      if(iosystem::StringQuestion(PlayerName, CONST_S("What is your name? (1-20 letters)"), v2(30, 46), WHITE, 1, 20, true, true) == ABORTED || PlayerName.IsEmpty())
	return false;
    }
    else
      PlayerName = ivanconfig::GetDefaultName();
  else
    PlayerName = Name;

#ifdef WIN32
  _mkdir("Save");
  _mkdir("Bones");
#endif

#ifdef __DJGPP__
  mkdir("Save", S_IWUSR);
  mkdir("Bones", S_IWUSR);
#endif

#ifdef LINUX
  mkdir(GetSaveDir().CStr(), S_IRWXU|S_IRWXG);
#endif

  LOSTick = 2;
  DangerFound = 0;
  CausePanicFlag = false;

  switch(Load(SaveName(PlayerName)))
  {
   case LOADED:
    {
      globalwindowhandler::InstallControlLoop(AnimationController);
      SetIsRunning(true);
      SetForceJumpToPlayerBe(true);
      GetCurrentArea()->SendNewDrawRequest();
      SendLOSUpdateRequest();
      ADD_MESSAGE("Game loaded successfully.");
      return true;
    }
   case NEW_GAME:
    {
      iosystem::TextScreen(CONST_S( "You couldn't possibly have guessed this day would differ from any other.\n"
				    "It began just as always. You woke up at dawn and drove off the giant spider\n"
				    "resting on your face. On your way to work you had serious trouble avoiding\n"
				    "the lions and pythons roaming wild around the village. After getting kicked\n"
				    "by colony masters for being late you performed your twelve-hour routine of\n"
				    "climbing trees, gathering bananas, climbing trees, gathering bananas, chasing\n"
				    "monkeys that stole the first gathered bananas, carrying bananas to the village\n"
				    "and trying to look happy when real food was distributed.\n\n"
				    "Finally you were about to enjoy your free time by taking a quick dip in the\n"
				    "nearby crocodile bay. However, at this point something unusual happened.\n"
				    "You were summoned to the mansion of Richel Decos, the viceroy of the\n"
				    "colony, and were led directly to him."));

      iosystem::TextScreen(CONST_S( "\"I have a task for you, citizen\", said the viceroy picking his golden\n"
				    "teeth, \"The market price of bananas has taken a deep dive and yet the\n"
				    "central government is about to raise taxes. I have sent appeals to high\n"
				    "priest Petrus but received no response. I fear my enemies in Attnam are\n"
				    "plotting against me and intercepting my messages before they reach him!\"\n\n"
				    "\"That is why you must travel to Attnam with a letter I'll give you and\n"
				    "deliver it to Petrus directly. Alas, you somehow have to cross the sea\n"
				    "between. Because it's winter, all Attnamese ships are trapped by ice and\n"
				    "I have none. Therefore you must venture through the small underwater tunnel\n"
				    "connecting our islands. It is infested with monsters, but since you have\n"
				    "stayed alive here so long, the trip will surely cause you no trouble.\"\n\n"
				    "You have never been so happy! According to the mansion's traveling\n"
				    "brochures, Attnam is a peaceful but bustling world city on a beautiful\n"
				    "snowy fell surrounded by frozen lakes glittering in the arctic sun just\n"
				    "like the diamonds of the imperial treasury. Not that you would believe a\n"
				    "word. The point is that tomorrow you can finally forget your home and\n"
				    "face the untold adventures ahead."));

      globalwindowhandler::InstallControlLoop(AnimationController);
      SetIsRunning(true);
      InWilderness = true;
      iosystem::TextScreen(CONST_S("Generating game...\n\nThis may take some time, please wait."), ZERO_V2, WHITE, false, true, &BusyAnimation);
      igraph::CreateBackGround(GRAY_FRACTAL);
      NextCharacterID = 1;
      NextItemID = 1;
      NextTrapID = 1;
      InitScript();
      CreateTeams();
      CreateGods();
      SetPlayer(playerkind::Spawn());
      Player->SetAssignedName(PlayerName);
      Player->SetTeam(GetTeam(0));
      Player->SetNP(SATIATED_LEVEL);

      for(int c = 0; c < ATTRIBUTES; ++c)
      {
	if(c != ENDURANCE)
	  Player->EditAttribute(c, (RAND() & 1) - (RAND() & 1));

	Player->EditExperience(c, 500, 1 << 11);
      }

      Player->SetMoney(Player->GetMoney() + RAND() % 11);
      GetTeam(0)->SetLeader(Player);
      InitDangerMap();
      Petrus = 0;
      InitDungeons();
      SetCurrentArea(WorldMap = new worldmap(128, 128));
      CurrentWSquareMap = WorldMap->GetMap();
      WorldMap->Generate();
      UpdateCamera();
      SendLOSUpdateRequest();
      Tick = 0;
      Turn = 0;
      InitPlayerAttributeAverage();
      StoryState = 0;
      PlayerMassacreMap.clear();
      PetMassacreMap.clear();
      MiscMassacreMap.clear();
      PlayerMassacreAmount = PetMassacreAmount = MiscMassacreAmount = 0;
      DefaultPolymorphTo.Empty();
      DefaultSummonMonster.Empty();
      DefaultWish.Empty();
      DefaultChangeMaterial.Empty();
      DefaultDetectMaterial.Empty();
      Player->GetStack()->AddItem(encryptedscroll::Spawn());
      character* Doggie = dog::Spawn();
      Doggie->SetTeam(GetTeam(0));
      GetWorldMap()->GetPlayerGroup().push_back(Doggie);
      Doggie->SetAssignedName(ivanconfig::GetDefaultPetName());
      WizardMode = false;
      SeeWholeMapCheatMode = MAP_HIDDEN;
      GoThroughWallsCheat = false;
      SumoWrestling = false;
      GlobalRainTimeModifier = 2048 - (RAND() & 4095);
      PlayerSumoChampion = false;
      protosystem::InitCharacterDataBaseFlags();
      memset(EquipmentMemory, 0, sizeof(EquipmentMemory));
      PlayerRunning = false;
      InitAttributeMemory();
      NecroCounter = 0;
      GameBegan = time(0);
      LastLoad = time(0);
      TimePlayedBeforeLastLoad = time::GetZeroTime();
      bool PlayerHasReceivedAllGodsKnownBonus = false;
      ADD_MESSAGE("You commence your journey to Attnam. Use direction keys to move, '>' to enter an area and '?' to view other commands.");

      if(IsXMas())
      {
	item* Present = banana::Spawn();
	Player->GetStack()->AddItem(Present);
	ADD_MESSAGE("Atavus is happy today! He gives you %s.", Present->CHAR_NAME(INDEFINITE));
      }

      return true;
    }
   default:
    return false;
  }
}

void game::DeInit()
{
  delete WorldMap;
  WorldMap = 0;
  int c;

  for(c = 1; c < Dungeons; ++c)
    delete Dungeon[c];

  delete [] Dungeon;

  for(c = 1; c <= GODS; ++c)
    delete God[c]; // sorry, Valpuri!

  delete [] God;
  pool::BurnHell();

  for(c = 0; c < Teams; ++c)
    delete Team[c];

  delete [] Team;
  delete GameScript;
  msgsystem::Format();
  DangerMap.clear();
}

void game::Run()
{
  for(;;)
  {
    if(!InWilderness)
    {
      /* Temporary places */
      static int Counter = 0;

      if(++Counter == 10)
      {
	CurrentLevel->GenerateMonsters();
	Counter = 0;
      }

      if(CurrentDungeonIndex == ELPURI_CAVE
	 && CurrentLevelIndex == ZOMBIE_LEVEL
	 && !RAND_N(1000 + NecroCounter))
      {
	character* Char = necromancer::Spawn(RAND_N(4) ? APPRENTICE_NECROMANCER : MASTER_NECROMANCER);
	v2 Pos;

	for(int c2 = 0; c2 < 30; ++c2)
	{
	  Pos = GetCurrentLevel()->GetRandomSquare(Char);

	  if(abs(int(Pos.X) - Player->GetPos().X) > 20
	     || abs(int(Pos.Y) - Player->GetPos().Y) > 20)
	    break;
	}

	if(Pos != ERROR_V2)
	{
	  Char->SetTeam(GetTeam(MONSTER_TEAM));
	  Char->PutTo(Pos);
	  Char->SetGenerationDanger(GetCurrentLevel()->GetDifficulty());
	  Char->SignalGeneration();
	  Char->SignalNaturalGeneration();
	  ivantime Time;
	  GetTime(Time);
	  int Modifier = Time.Day - EDIT_ATTRIBUTE_DAY_MIN;

	  if(Modifier > 0)
	    Char->EditAllAttributes(Modifier >> EDIT_ATTRIBUTE_DAY_SHIFT);

	  NecroCounter += 50;
	}
	else
	  delete Char;
      }

      if(!(GetTick() % 1000))
	CurrentLevel->CheckSunLight();

      if((CurrentDungeonIndex == NEW_ATTNAM
	  || CurrentDungeonIndex == ATTNAM)
	 && CurrentLevelIndex == 0)
      {
	long OldVolume = GlobalRainLiquid->GetVolume();
	long NewVolume = Max(long(sin((Tick + GlobalRainTimeModifier) * 0.0003) * 300 - 150), 0L);

	if(NewVolume && !OldVolume)
	  CurrentLevel->EnableGlobalRain();
	else if(!NewVolume && OldVolume)
	  CurrentLevel->DisableGlobalRain();

	GlobalRainLiquid->SetVolumeNoSignals(NewVolume);

	/*{
	  item* Item;

	  if(!RAND_N(2))
	  Item = wand::Spawn(1 + RAND_N(12));
	  else if(!RAND_N(2))
	  {
	  Item = beartrap::Spawn();
	  Item->SetIsActive(true);
	  Item->SetTeam(MONSTER_TEAM);
	  }
	  else if(!RAND_N(2))
	  {
	  Item = mine::Spawn();
	  Item->SetIsActive(true);
	  Item->SetTeam(MONSTER_TEAM);
	  }
	  else
	  Item = holybanana::Spawn();

	  CurrentLevel->GetLSquare(CurrentLevel->GetRandomSquare())->AddItem(Item);
	  }

	  if(!RAND_N(10))
	  {
	  character* Char = protosystem::CreateMonster(0, 1000000);
	  Char->ChangeTeam(GetTeam(RAND() % Teams));
	  Char->PutTo(CurrentLevel->GetRandomSquare(Char));
	  }

	  if(!RAND_N(5))
	  {
	  character* Char;
	  if(!RAND_N(5))
	  Char = spider::Spawn(GIANT);
	  else if(!RAND_N(5))
	  Char = darkmage::Spawn(1 + RAND_N(4));
	  else if(!RAND_N(5))
	  Char = necromancer::Spawn(1 + RAND_N(2));
	  else if(!RAND_N(5))
	  Char = chameleon::Spawn();
	  else if(!RAND_N(5))
	  Char = kamikazedwarf::Spawn(1 + RAND_N(GODS));
	  else if(!RAND_N(5))
	  Char = mommo::Spawn(1 + RAND_N(2));
	  else if(!RAND_N(3))
	  Char = bunny::Spawn(RAND_2 ? ADULT_MALE : ADULT_FEMALE);
	  else if(!RAND_N(3))
	  Char = eddy::Spawn();
	  else if(!RAND_N(3))
	  Char = magicmushroom::Spawn();
	  else if(!RAND_N(5))
	  Char = mushroom::Spawn();
	  else if(!RAND_N(3))
	  Char = blinkdog::Spawn();
	  else if(!RAND_N(5))
	  Char = tourist::Spawn(1 + RAND_N(3));
	  else if(!RAND_N(5))
	  Char = hattifattener::Spawn();
	  else if(!RAND_N(5))
	  Char = genetrixvesana::Spawn();
	  else if(!RAND_N(5))
	  Char = skunk::Spawn();
	  else if(!RAND_N(5))
	  Char = ennerbeast::Spawn();
	  else if(!RAND_N(5))
	  Char = werewolfhuman::Spawn();
	  else if(!RAND_N(5))
	  Char = unicorn::Spawn(1 + RAND_N(3));
	  else if(!RAND_N(5))
	  Char = floatingeye::Spawn();
	  else if(!RAND_N(5))
	  Char = zombie::Spawn();
	  else if(!RAND_N(5))
	  Char = magpie::Spawn();
	  else if(!RAND_N(5))
	  Char = elpuri::Spawn();
	  else if(!RAND_N(5))
	  Char = vladimir::Spawn();
	  else if(!RAND_N(5))
	  Char = billswill::Spawn();
	  else if(!RAND_N(5))
	  Char = ghost::Spawn();
	  else if(!RAND_N(5))
	  Char = dolphin::Spawn();
	  else if(!RAND_N(5))
	  Char = cossack::Spawn();
	  else
	  Char = invisiblestalker::Spawn();

	  Char->SetTeam(GetTeam(RAND() % Teams));
	  Char->PutTo(CurrentLevel->GetRandomSquare(Char));
	  }*/
      }
    }

    try
    {
      pool::Be();
      pool::BurnHell();
      IncreaseTick();
      ApplyDivineTick();
    }
    catch(quitrequest)
    {
      break;
    }
    catch(areachangerequest)
    {
    }
  }
}

void game::InitLuxTable()
{
  if(!LuxTable)
  {
    Alloc3D(LuxTable, 256, 33, 33);

    for(int c = 0; c < 0x100; ++c)
      for(int x = 0; x < 33; ++x)
	for(int y = 0; y < 33; ++y)
	{
	  int X = x - 16, Y = y - 16;
	  LuxTable[c][x][y] = int(c / (double(X * X + Y * Y) / 128 + 1));
	}

    atexit(DeInitLuxTable);
  }
}

void game::DeInitLuxTable()
{
  delete [] LuxTable;
  LuxTable = 0;
}

void game::UpdateCameraX()
{
  UpdateCameraX(Player->GetPos().X);
}

void game::UpdateCameraY()
{
  UpdateCameraY(Player->GetPos().Y);
}

void game::UpdateCameraX(int X)
{
  UpdateCameraCoordinate(Camera.X, X, GetCurrentArea()->GetXSize(), GetScreenXSize());
}

void game::UpdateCameraY(int Y)
{
  UpdateCameraCoordinate(Camera.Y, Y, GetCurrentArea()->GetYSize(), GetScreenYSize());
}

void game::UpdateCameraCoordinate(int& Coordinate, int Center, int Size, int ScreenSize)
{
  int OldCoordinate = Coordinate;

  if(Size < ScreenSize)
    Coordinate = (Size - ScreenSize) >> 1;
  else if(Center < ScreenSize >> 1)
    Coordinate = 0;
  else if(Center > Size - (ScreenSize >> 1))
    Coordinate = Size - ScreenSize;
  else
    Coordinate = Center - (ScreenSize >> 1);

  if(Coordinate != OldCoordinate)
    GetCurrentArea()->SendNewDrawRequest();
}

cchar* game::Insult() // convert to array
{
  switch(RAND_N(18))
  {
   case 0  : return "moron";
   case 1  : return "silly";
   case 2  : return "idiot";
   case 3  : return "airhead";
   case 4  : return "jerk";
   case 5  : return "dork";
   case 6  : return "Mr. Mole";
   case 7  : return "navastater";
   case 8  : return "potatoes-for-eyes";
   case 9  : return "lamer";
   case 10 : return "mommo-for-brains";
   case 11 : return "pinhead";
   case 12 : return "stupid-headed person";
   case 13 : return "software abuser";
   case 14 : return "loser";
   case 15 : return "peaballs";
   case 16 : return "person-with-problems";
   case 17 : return "unimportant user";
   default : return "hugger-mugger";
  }
}

/* DefaultAnswer = REQUIRES_ANSWER the question requires an answer */

truth game::TruthQuestion(cfestring& String, int DefaultAnswer, int OtherKeyForTrue)
{
  if(DefaultAnswer == NO)
    DefaultAnswer = 'n';
  else if(DefaultAnswer == YES)
    DefaultAnswer = 'y';
  else if(DefaultAnswer != REQUIRES_ANSWER)
    ABORT("Illegal TruthQuestion DefaultAnswer send!");

  int FromKeyQuestion = KeyQuestion(String, DefaultAnswer, 5, 'y', 'Y', 'n', 'N', OtherKeyForTrue);
  return FromKeyQuestion == 'y' || FromKeyQuestion == 'Y' || FromKeyQuestion == OtherKeyForTrue;
}

void game::DrawEverything()
{
  DrawEverythingNoBlit();
  graphics::BlitDBToScreen();
}

truth game::OnScreen(v2 Pos)
{
  return Pos.X >= 0 && Pos.Y >= 0 && Pos.X >= Camera.X && Pos.Y >= Camera.Y && Pos.X < GetCamera().X + GetScreenXSize() && Pos.Y < GetCamera().Y + GetScreenYSize();
}

void game::DrawEverythingNoBlit(truth AnimationDraw)
{
  if(LOSUpdateRequested && Player->IsEnabled())
    if(!IsInWilderness())
      GetCurrentLevel()->UpdateLOS();
    else
      GetWorldMap()->UpdateLOS();

  if(OnScreen(CursorPos))
    if(!IsInWilderness() || CurrentWSquareMap[CursorPos.X][CursorPos.Y]->GetLastSeen() || GetSeeWholeMapCheatMode())
      CurrentArea->GetSquare(CursorPos)->SendStrongNewDrawRequest();
    else
      DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), TILE_V2, 0);

  int c;

  for(c = 0; c < SpecialCursorPos.size(); ++c)
    if(OnScreen(SpecialCursorPos[c]))
      CurrentArea->GetSquare(SpecialCursorPos[c])->SendStrongNewDrawRequest();

  globalwindowhandler::UpdateTick();
  GetCurrentArea()->Draw(AnimationDraw);
  Player->DrawPanel(AnimationDraw);

  if(!AnimationDraw)
    msgsystem::Draw();

  if(OnScreen(CursorPos))
  {
    v2 ScreenCoord = CalculateScreenCoordinates(CursorPos);
    blitdata B = { DOUBLE_BUFFER,
		   { 0, 0 },
		   { ScreenCoord.X, ScreenCoord.Y },
		   { TILE_SIZE, TILE_SIZE },
		   { 0 },
		   TRANSPARENT_COLOR,
		   ALLOW_ANIMATE|ALLOW_ALPHA };

    if(!IsInWilderness() && !GetSeeWholeMapCheatMode())
    {
      lsquare* Square = CurrentLSquareMap[CursorPos.X][CursorPos.Y];

      if(Square->GetLastSeen() != GetLOSTick())
	Square->DrawMemorized(B);
    }

    if(DoZoom())
    {
      B.Src = B.Dest;
      B.Dest.X = RES.X - 96;
      B.Dest.Y = RES.Y - 96;
      B.Stretch = 5;
      DOUBLE_BUFFER->StretchBlit(B);
    }

    igraph::DrawCursor(ScreenCoord, CursorData);
  }

  if(Player->IsEnabled())
    if(Player->IsSmall())
    {
      v2 Pos = Player->GetPos();

      if(OnScreen(Pos))
      {
	v2 ScreenCoord = CalculateScreenCoordinates(Pos);
	igraph::DrawCursor(ScreenCoord, Player->GetCursorData());
      }
    }
    else
    {
      for(c = 0; c < Player->GetSquaresUnder(); ++c)
      {
	v2 Pos = Player->GetPos(c);

	if(OnScreen(Pos))
	{
	  v2 ScreenCoord = CalculateScreenCoordinates(Pos);
	  igraph::DrawCursor(ScreenCoord, Player->GetCursorData()|CURSOR_BIG, c);
	}
      }
    }

  for(c = 0; c < SpecialCursorPos.size(); ++c)
    if(OnScreen(SpecialCursorPos[c]))
    {
      v2 ScreenCoord = CalculateScreenCoordinates(SpecialCursorPos[c]);
      igraph::DrawCursor(ScreenCoord, SpecialCursorData[c]);
      GetCurrentArea()->GetSquare(SpecialCursorPos[c])->SendStrongNewDrawRequest();
    }
}

truth game::Save(cfestring& SaveName)
{
  outputfile SaveFile(SaveName + ".sav");
  SaveFile << int(SAVE_FILE_VERSION);
  SaveFile << GameScript << CurrentDungeonIndex << CurrentLevelIndex << Camera;
  SaveFile << WizardMode << SeeWholeMapCheatMode << GoThroughWallsCheat;
  SaveFile << Tick << Turn << InWilderness << NextCharacterID << NextItemID << NextTrapID << NecroCounter;
  SaveFile << SumoWrestling << PlayerSumoChampion << GlobalRainTimeModifier;
  long Seed = RAND();
  femath::SetSeed(Seed);
  SaveFile << Seed;
  SaveFile << AveragePlayerArmStrengthExperience;
  SaveFile << AveragePlayerLegStrengthExperience;
  SaveFile << AveragePlayerDexterityExperience;
  SaveFile << AveragePlayerAgilityExperience;
  SaveFile << Teams << Dungeons << StoryState << PlayerRunning;
  SaveFile << PlayerMassacreMap << PetMassacreMap << MiscMassacreMap;
  SaveFile << PlayerMassacreAmount << PetMassacreAmount << MiscMassacreAmount;
  SaveArray(SaveFile, EquipmentMemory, MAX_EQUIPMENT_SLOTS);
  int c;

  for(c = 0; c < ATTRIBUTES; ++c)
    SaveFile << OldAttribute[c] << NewAttribute[c] << LastAttributeChangeTick[c];

  for(c = 1; c < Dungeons; ++c)
    SaveFile << Dungeon[c];

  for(c = 1; c <= GODS; ++c)
    SaveFile << God[c];

  for(c = 0; c < Teams; ++c)
    SaveFile << Team[c];

  if(InWilderness)
    SaveWorldMap(SaveName, false);
  else
    GetCurrentDungeon()->SaveLevel(SaveName, CurrentLevelIndex, false);

  SaveFile << Player->GetPos() << PlayerName;
  msgsystem::Save(SaveFile);
  SaveFile << DangerMap << NextDangerIDType << NextDangerIDConfigIndex;
  SaveFile << DefaultPolymorphTo << DefaultSummonMonster;
  SaveFile << DefaultWish << DefaultChangeMaterial << DefaultDetectMaterial;
  SaveFile << GetTimeSpent();
  /* or in more readable format: time() - LastLoad + TimeAtLastLoad */

  SaveFile << PlayerHasReceivedAllGodsKnownBonus;
  protosystem::SaveCharacterDataBaseFlags(SaveFile);
  return true;
}

int game::Load(cfestring& SaveName)
{
  inputfile SaveFile(SaveName + ".sav", 0, false);

  if(!SaveFile.IsOpen())
    return NEW_GAME;

  int Version;
  SaveFile >> Version;

  if(Version != SAVE_FILE_VERSION)
  {
    if(!iosystem::Menu(0, v2(RES.X >> 1, RES.Y >> 1), CONST_S("Sorry, this save is incompatible with the new version.\rStart new game?\r"), CONST_S("Yes\rNo\r"), LIGHT_GRAY))
      return NEW_GAME;
    else
      return BACK;
  }

  SaveFile >> GameScript >> CurrentDungeonIndex >> CurrentLevelIndex >> Camera;
  SaveFile >> WizardMode >> SeeWholeMapCheatMode >> GoThroughWallsCheat;
  SaveFile >> Tick >> Turn >> InWilderness >> NextCharacterID >> NextItemID >> NextTrapID >> NecroCounter;
  SaveFile >> SumoWrestling >> PlayerSumoChampion >> GlobalRainTimeModifier;
  femath::SetSeed(ReadType<long>(SaveFile));
  SaveFile >> AveragePlayerArmStrengthExperience;
  SaveFile >> AveragePlayerLegStrengthExperience;
  SaveFile >> AveragePlayerDexterityExperience;
  SaveFile >> AveragePlayerAgilityExperience;
  SaveFile >> Teams >> Dungeons >> StoryState >> PlayerRunning;
  SaveFile >> PlayerMassacreMap >> PetMassacreMap >> MiscMassacreMap;
  SaveFile >> PlayerMassacreAmount >> PetMassacreAmount >> MiscMassacreAmount;
  LoadArray(SaveFile, EquipmentMemory, MAX_EQUIPMENT_SLOTS);
  int c;

  for(c = 0; c < ATTRIBUTES; ++c)
    SaveFile >> OldAttribute[c] >> NewAttribute[c] >> LastAttributeChangeTick[c];

  Dungeon = new dungeon*[Dungeons];
  Dungeon[0] = 0;

  for(c = 1; c < Dungeons; ++c)
    SaveFile >> Dungeon[c];

  God = new god*[GODS + 1];
  God[0] = 0;

  for(c = 1; c <= GODS; ++c)
    SaveFile >> God[c];

  Team = new team*[Teams];

  for(c = 0; c < Teams; ++c)
    SaveFile >> Team[c];

  if(InWilderness)
  {
    SetCurrentArea(LoadWorldMap(SaveName));
    CurrentWSquareMap = WorldMap->GetMap();
    igraph::CreateBackGround(GRAY_FRACTAL);
  }
  else
  {
    SetCurrentArea(CurrentLevel = GetCurrentDungeon()->LoadLevel(SaveName, CurrentLevelIndex));
    CurrentLSquareMap = CurrentLevel->GetMap();
    igraph::CreateBackGround(*CurrentLevel->GetLevelScript()->GetBackGroundType());
  }

  v2 Pos;
  SaveFile >> Pos >> PlayerName;
  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());
  msgsystem::Load(SaveFile);
  SaveFile >> DangerMap >> NextDangerIDType >> NextDangerIDConfigIndex;
  SaveFile >> DefaultPolymorphTo >> DefaultSummonMonster;
  SaveFile >> DefaultWish >> DefaultChangeMaterial >> DefaultDetectMaterial;
  SaveFile >> TimePlayedBeforeLastLoad;
  SaveFile >> PlayerHasReceivedAllGodsKnownBonus;
  LastLoad = time(0);
  protosystem::LoadCharacterDataBaseFlags(SaveFile);
  return LOADED;
}

festring game::SaveName(cfestring& Base)
{
  festring SaveName = GetSaveDir();

  if(!Base.GetSize())
    SaveName << PlayerName;
  else
    SaveName << Base;

  for(festring::sizetype c = 0; c < SaveName.GetSize(); ++c)
    if(SaveName[c] == ' ')
      SaveName[c] = '_';

#if defined(WIN32) || defined(__DJGPP__)
  if(SaveName.GetSize() > 13)
    SaveName.Resize(13);
#endif

  return SaveName;
}

int game::GetMoveCommandKeyBetweenPoints(v2 A, v2 B)
{
  for(int c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if((A + GetMoveVector(c)) == B)
      return GetMoveCommandKey(c);

  return DIR_ERROR;
}

void game::ApplyDivineTick()
{
  for(int c = 1; c <= GODS; ++c)
    GetGod(c)->ApplyDivineTick();
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, int Multiplier, truth Good)
{
  for(int c = 1; c <= GODS; ++c)
    if(GetGod(c) != CompareTarget)
      GetGod(c)->AdjustRelation(CompareTarget, Multiplier, Good);
}

v2 game::GetDirectionVectorForKey(int Key)
{
  if(Key == KEY_NUMPAD_5)
    return ZERO_V2;

  for(int c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if(Key == GetMoveCommandKey(c))
      return GetMoveVector(c);

  return ERROR_V2;
}

double game::GetMinDifficulty()
{
  double Base = CurrentLevel->GetDifficulty() * 0.2;
  long MultiplierExponent = 0;
  ivantime Time;
  GetTime(Time);
  int Modifier = Time.Day - DANGER_PLUS_DAY_MIN;

  if(Modifier > 0)
    Base += DANGER_PLUS_MULTIPLIER * Modifier;

  for(;;)
  {
    int Dice = RAND() % 25;

    if(Dice < 5 && MultiplierExponent > -3)
    {
      Base /= 3;
      --MultiplierExponent;
      continue;
    }

    if(Dice >= 20 && MultiplierExponent < 3)
    {
      Base *= 3;
      ++MultiplierExponent;
      continue;
    }

    return Base;
  }
}

void game::ShowLevelMessage()
{
  if(CurrentLevel->GetLevelMessage().GetSize())
    ADD_MESSAGE(CurrentLevel->GetLevelMessage().CStr());

  CurrentLevel->SetLevelMessage("");
}

int game::DirectionQuestion(cfestring& Topic, truth RequireAnswer, truth AcceptYourself)
{
  for(;;)
  {
    int Key = AskForKeyPress(Topic);

    if(AcceptYourself && Key == '.')
      return YOURSELF;

    for(int c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
      if(Key == GetMoveCommandKey(c))
	return c;

    if(!RequireAnswer)
      return DIR_ERROR;
  }
}

void game::RemoveSaves(truth RealSavesAlso)
{
  if(RealSavesAlso)
  {
    remove(festring(SaveName() + ".sav").CStr());
    remove(festring(SaveName() + ".wm").CStr());
  }

  remove(festring(AutoSaveFileName + ".sav").CStr());
  remove(festring(AutoSaveFileName + ".wm").CStr());
  festring File;

  for(int i = 1; i < Dungeons; ++i)
    for(int c = 0; c < GetDungeon(i)->GetLevels(); ++c)
    {
      /* This looks very odd. And it is very odd.
       * Indeed, gcc is very odd to not compile this correctly with -O3
       * if it is written in a less odd way. */

      File = SaveName() + '.' + i;
      File << c;

      if(RealSavesAlso)
	remove(File.CStr());

      File = AutoSaveFileName + '.' + i;
      File << c;

      remove(File.CStr());
    }
}

void game::SetPlayer(character* NP)
{
  Player = NP;

  if(Player)
    Player->AddFlags(C_PLAYER);
}

void game::InitDungeons()
{
  Dungeons = *GetGameScript()->GetDungeons() + 1;
  Dungeon = new dungeon*[Dungeons];
  Dungeon[0] = 0;

  for(int c = 1; c < Dungeons; ++c)
  {
    Dungeon[c] = new dungeon(c);
    Dungeon[c]->SetIndex(c);
  }
}

void game::DoEvilDeed(int Amount)
{
  if(!Amount)
    return;

  for(int c = 1; c <= GODS; ++c)
  {
    int Change = Amount - Amount * GetGod(c)->GetAlignment() / 5;

    if(!IsInWilderness() && Player->GetLSquareUnder()->GetDivineMaster() == c)
      if(GetGod(c)->GetRelation() - (Change << 1) < -750)
      {
	if(GetGod(c)->GetRelation() > -750)
	  GetGod(c)->SetRelation(-750);
      }
      else if(GetGod(c)->GetRelation() - (Change << 1) > 750)
      {
	if(GetGod(c)->GetRelation() < 750)
	  GetGod(c)->SetRelation(750);
      }
      else
	GetGod(c)->SetRelation(GetGod(c)->GetRelation() - (Change << 1));
    else
      if(GetGod(c)->GetRelation() - Change < -500)
      {
	if(GetGod(c)->GetRelation() > -500)
	  GetGod(c)->SetRelation(-500);
      }
      else if(GetGod(c)->GetRelation() - Change > 500)
      {
	if(GetGod(c)->GetRelation() < 500)
	  GetGod(c)->SetRelation(500);
      }
      else
	GetGod(c)->SetRelation(GetGod(c)->GetRelation() - Change);
  }
}

void game::SaveWorldMap(cfestring& SaveName, truth DeleteAfterwards)
{
  outputfile SaveFile(SaveName + ".wm");
  SaveFile << WorldMap;

  if(DeleteAfterwards)
  {
    delete WorldMap;
    WorldMap = 0;
  }
}

worldmap* game::LoadWorldMap(cfestring& SaveName)
{
  inputfile SaveFile(SaveName + ".wm");
  SaveFile >> WorldMap;
  return WorldMap;
}

void game::Hostility(team* Attacker, team* Defender)
{
  for(int c = 0; c < Teams; ++c)
    if(GetTeam(c) != Attacker && GetTeam(c) != Defender
       && GetTeam(c)->GetRelation(Defender) == FRIEND
       && c != NEW_ATTNAM_TEAM
       && c != TOURIST_GUIDE_TEAM) // gum solution
      GetTeam(c)->SetRelation(Attacker, HOSTILE);
}

void game::CreateTeams()
{
  Teams = *GetGameScript()->GetTeams();
  Team = new team*[Teams];
  int c;

  for(c = 0; c < Teams; ++c)
  {
    Team[c] = new team(c);

    for(int i = 0; i < c; ++i)
      Team[i]->SetRelation(Team[c], UNCARING);
  }

  for(c = 0; c < Teams; ++c)
    if(c != 1)
      Team[1]->SetRelation(Team[c], HOSTILE);

  const std::list<std::pair<int, teamscript> >& TeamScript = GetGameScript()->GetTeam();

  for(std::list<std::pair<int, teamscript> >::const_iterator i = TeamScript.begin(); i != TeamScript.end(); ++i)
  {
    for(uint c = 0; c < i->second.GetRelation().size(); ++c)
      GetTeam(i->second.GetRelation()[c].first)->SetRelation(GetTeam(i->first), i->second.GetRelation()[c].second);

    cint* KillEvilness = i->second.GetKillEvilness();

    if(KillEvilness)
      GetTeam(i->first)->SetKillEvilness(*KillEvilness);
  }
}

/* v2 Pos should be removed from xxxQuestion()s? */

festring game::StringQuestion(cfestring& Topic, col16 Color, festring::sizetype MinLetters, festring::sizetype MaxLetters, truth AllowExit, stringkeyhandler KeyHandler)
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23)); // pos may be incorrect!
  festring Return;
  iosystem::StringQuestion(Return, Topic, v2(16, 6), Color, MinLetters, MaxLetters, false, AllowExit, KeyHandler);
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  return Return;
}

long game::NumberQuestion(cfestring& Topic, col16 Color, truth ReturnZeroOnEsc)
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  long Return = iosystem::NumberQuestion(Topic, v2(16, 6), Color, false, ReturnZeroOnEsc);
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  return Return;
}

long game::ScrollBarQuestion(cfestring& Topic, long BeginValue, long Step, long Min, long Max, long AbortValue, col16 TopicColor, col16 Color1, col16 Color2, void (*Handler)(long))
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  long Return = iosystem::ScrollBarQuestion(Topic, v2(16, 6), BeginValue, Step, Min, Max, AbortValue, TopicColor, Color1, Color2, GetMoveCommandKey(KEY_LEFT_INDEX), GetMoveCommandKey(KEY_RIGHT_INDEX), false, Handler);
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  return Return;
}

ulong game::IncreaseLOSTick()
{
  if(LOSTick != 0xFE)
    return LOSTick += 2;
  else
  {
    CurrentLevel->InitLastSeen();
    return LOSTick = 4;
  }
}

void game::UpdateCamera()
{
  UpdateCameraX();
  UpdateCameraY();
}

truth game::HandleQuitMessage()
{
#ifdef USE_SDL

  if(IsRunning())
  {
    if(IsInGetCommand())
    {
      switch(Menu(0, v2(RES.X >> 1, RES.Y >> 1), CONST_S("Do you want to save your game before quitting?\r"), CONST_S("Yes\rNo\rCancel\r"), LIGHT_GRAY))
      {
       case 0:
	Save();
	RemoveSaves(false);
	break;
       case 2:
	GetCurrentArea()->SendNewDrawRequest();
	DrawEverything();
	return false;
       default:
	festring Msg = CONST_S("cowardly quit the game");
	Player->AddScoreEntry(Msg, 0.75);
	End(Msg, true, false);
	break;
      }
    }
    else
      if(!Menu(0, v2(RES.X >> 1, RES.Y >> 1), CONST_S("You can't save at this point. Are you sure you still want to do this?\r"), CONST_S("Yes\rNo\r"), LIGHT_GRAY))
	RemoveSaves();
      else
      {
	GetCurrentArea()->SendNewDrawRequest();
	DrawEverything();
	return false;
      }
  }

#endif /* USE_SDL */

  return true;
}

int game::GetDirectionForVector(v2 Vector)
{
  for(int c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
    if(Vector == GetMoveVector(c))
      return c;

  return DIR_ERROR;
}

cchar* game::GetVerbalPlayerAlignment()
{
  long Sum = 0;

  for(int c = 1; c <= GODS; ++c)
  {
    if(GetGod(c)->GetRelation() > 0)
      Sum += GetGod(c)->GetRelation() * (5 - GetGod(c)->GetAlignment());
  }

  if(Sum > 15000)
    return "extremely lawful";
  if(Sum > 10000)
    return "very lawful";
  if(Sum > 5000)
    return "lawful";
  if(Sum > 1000)
    return "mildly lawful";
  if(Sum > -1000)
    return "neutral";
  if(Sum > -5000)
    return "mildly chaotic";
  if(Sum > -10000)
    return "chaotic";
  if(Sum > -15000)
    return "very chaotic";

  return "extremely chaotic";
}

void game::CreateGods()
{
  God = new god*[GODS + 1];
  God[0] = 0;

  for(int c = 1; c < protocontainer<god>::GetSize(); ++c)
    God[c] = protocontainer<god>::GetProto(c)->Spawn();
}

void game::BusyAnimation()
{
  BusyAnimation(DOUBLE_BUFFER, false);
}

void game::BusyAnimation(bitmap* Buffer, truth ForceDraw)
{
  static clock_t LastTime = 0;
  static int Frame = 0;
  static blitdata B1 = { 0,
			 { 0, 0 },
			 { 0, 0 },
			 { RES.X, RES.Y },
			 { 0 },
			 0,
			 0 };
  static blitdata B2 = { 0,
			 { 0, 0 },
			 { (RES.X >> 1) - 100, (RES.Y << 1) / 3 - 100 },
			 { 200, 200 },
			 { 0 },
			 0,
			 0 };

  if(ForceDraw || clock() - LastTime > CLOCKS_PER_SEC / 25)
  {
    B2.Bitmap = Buffer;
    B2.Dest.X = (RES.X >> 1) - 100 + EnterTextDisplacement.X;
    B2.Dest.Y = (RES.Y << 1) / 3 - 100 + EnterTextDisplacement.Y;

    if(EnterImage)
    {
      B1.Bitmap = Buffer;
      EnterImage->NormalMaskedBlit(B1);
    }

    BusyAnimationCache[Frame]->NormalBlit(B2);

    if(Buffer == DOUBLE_BUFFER)
      graphics::BlitDBToScreen();

    if(++Frame == 32)
      Frame = 0;

    LastTime = clock();
  }
}

void game::CreateBusyAnimationCache()
{
  bitmap Elpuri(TILE_V2, TRANSPARENT_COLOR);
  Elpuri.ActivateFastFlag();
  packcol16 Color = MakeRGB16(60, 60, 60);
  igraph::GetCharacterRawGraphic()->MaskedBlit(&Elpuri, v2(64, 0), ZERO_V2, TILE_V2, &Color);
  bitmap Circle(v2(200, 200), TRANSPARENT_COLOR);
  Circle.ActivateFastFlag();

  for(int x = 0; x < 4; ++x)
    Circle.DrawPolygon(100, 100, 95 + x, 50, MakeRGB16(255 - 12 * x, 0, 0));

  blitdata B1 = { 0,
		  { 0, 0 },
		  { 92, 92 },
		  { TILE_SIZE, TILE_SIZE },
		  { 0 },
		  TRANSPARENT_COLOR,
		  0 };

  blitdata B2 = { 0,
		  { 0, 0 },
		  { 0, 0 },
		  { 200, 200 },
		  { 0 },
		  TRANSPARENT_COLOR,
		  0 };

  for(int c = 0; c < 32; ++c)
  {
    B1.Bitmap = B2.Bitmap = BusyAnimationCache[c] = new bitmap(v2(200, 200), 0);
    B1.Bitmap->ActivateFastFlag();
    Elpuri.NormalMaskedBlit(B1);
    double Rotation = 0.3 + c * FPI / 80;

    for(int x = 0; x < 10; ++x)
      B1.Bitmap->DrawPolygon(100, 100, 95, 5, MakeRGB16(5 + 25 * x, 0, 0), false, true, Rotation + double(x) / 50);

    Circle.NormalMaskedBlit(B2);
  }
}

int game::AskForKeyPress(cfestring& Topic)
{
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, v2(16, 8), WHITE, "%s", Topic.CapitalizeCopy().CStr());
  graphics::BlitDBToScreen();
  int Key = GET_KEY();
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  return Key;
}

/* Handler is called when the key has been identified as a movement key
 * KeyHandler is called when the key has NOT been identified as a movement key
 * Both can be deactivated by passing 0 as parameter */

v2 game::PositionQuestion(cfestring& Topic, v2 CursorPos, void (*Handler)(v2), positionkeyhandler KeyHandler, truth Zoom)
{
  int Key = 0;
  SetDoZoom(Zoom);
  v2 Return;
  CursorData = RED_CURSOR;

  if(Handler)
    Handler(CursorPos);

  for(;;)
  {
    square* Square = GetCurrentArea()->GetSquare(CursorPos);

    if(!Square->HasBeenSeen() && (!Square->GetCharacter() || !Square->GetCharacter()->CanBeSeenByPlayer()) && !GetSeeWholeMapCheatMode())
      DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), TILE_V2, BLACK);
    else
      GetCurrentArea()->GetSquare(CursorPos)->SendStrongNewDrawRequest();

    if(Key == ' ' || Key == '.')
    {
      Return = CursorPos;
      break;
    }

    if(Key == KEY_ESC)
    {
      Return = ERROR_V2;
      break;
    }

    v2 DirectionVector = GetDirectionVectorForKey(Key);

    if(DirectionVector != ERROR_V2)
    {
      CursorPos += DirectionVector;

      if(CursorPos.X > GetCurrentArea()->GetXSize() - 1) CursorPos.X = 0;
      if(CursorPos.X < 0) CursorPos.X = GetCurrentArea()->GetXSize() - 1;
      if(CursorPos.Y > GetCurrentArea()->GetYSize() - 1) CursorPos.Y = 0;
      if(CursorPos.Y < 0) CursorPos.Y = GetCurrentArea()->GetYSize() - 1;

      if(Handler)
	Handler(CursorPos);
    }
    else if(KeyHandler)
    {
      CursorPos = KeyHandler(CursorPos, Key);

      if(CursorPos == ERROR_V2 || CursorPos == ABORT_V2)
      {
	Return = CursorPos;
	break;
      }
    }

    if(CursorPos.X < GetCamera().X + 3 || CursorPos.X >= GetCamera().X + GetScreenXSize() - 3)
      UpdateCameraX(CursorPos.X);

    if(CursorPos.Y < GetCamera().Y + 3 || CursorPos.Y >= GetCamera().Y + GetScreenYSize() - 3)
      UpdateCameraY(CursorPos.Y);

    FONT->Printf(DOUBLE_BUFFER, v2(16, 8), WHITE, "%s", Topic.CStr());
    SetCursorPos(CursorPos);
    DrawEverything();
    Key = GET_KEY();
  }

  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  igraph::BlitBackGround(v2(RES.X - 96, RES.Y - 96), v2(80, 80));
  SetDoZoom(false);
  SetCursorPos(v2(-1, -1));
  return Return;
}

void game::LookHandler(v2 CursorPos)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);
  festring OldMemory;

  if(GetSeeWholeMapCheatMode())
  {
    OldMemory = Square->GetMemorizedDescription();

    if(IsInWilderness())
      GetWorldMap()->GetWSquare(CursorPos)->UpdateMemorizedDescription(true);
    else
      GetCurrentLevel()->GetLSquare(CursorPos)->UpdateMemorizedDescription(true);
  }

  festring Msg;

  if(Square->HasBeenSeen() || GetSeeWholeMapCheatMode())
  {
    if(!IsInWilderness() && !Square->CanBeSeenByPlayer() && GetCurrentLevel()->GetLSquare(CursorPos)->CanBeFeltByPlayer())
      Msg = CONST_S("You feel here ");
    else if(Square->CanBeSeenByPlayer(true) || GetSeeWholeMapCheatMode())
      Msg = CONST_S("You see here ");
    else
      Msg = CONST_S("You remember here ");

    Msg << Square->GetMemorizedDescription() << '.';

    if(!IsInWilderness() && (Square->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
    {
      lsquare* LSquare = GetCurrentLevel()->GetLSquare(CursorPos);
      LSquare->DisplaySmokeInfo(Msg);

      if(LSquare->HasEngravings() && LSquare->IsTransparent())
	if(LSquare->EngravingsCanBeReadByPlayer() || GetSeeWholeMapCheatMode())
	  LSquare->DisplayEngravedInfo(Msg);
	else
	  Msg << " Something has been engraved here.";
    }
  }
  else
    Msg = CONST_S("You have never been here.");

  character* Character = Square->GetCharacter();

  if(Character && (Character->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
    Character->DisplayInfo(Msg);

  if(!(RAND() % 10000) && (Square->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
    Msg << " You see here a frog eating a magnolia.";

  ADD_MESSAGE("%s", Msg.CStr());

  if(GetSeeWholeMapCheatMode())
    Square->SetMemorizedDescription(OldMemory);
}

truth game::AnimationController()
{
  DrawEverythingNoBlit(true);
  return true;
}

void game::InitGlobalValueMap()
{
  inputfile SaveFile(GetGameDir() + "Script/define.dat", &GlobalValueMap);
  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
  {
    if(Word != "#" || SaveFile.ReadWord() != "define")
      ABORT("Illegal datafile define on line %ld!", SaveFile.TellLine());

    SaveFile.ReadWord(Word);
    GlobalValueMap.insert(std::make_pair(Word, SaveFile.ReadNumber()));
  }
}

void game::TextScreen(cfestring& Text, v2 Displacement, col16 Color, truth GKey, truth Fade, bitmapeditor BitmapEditor)
{
  globalwindowhandler::DisableControlLoops();
  iosystem::TextScreen(Text, Displacement, Color, GKey, Fade, BitmapEditor);
  globalwindowhandler::EnableControlLoops();
}

/* ... all the keys that are acceptable
   DefaultAnswer = REQUIRES_ANSWER if this question requires an answer
   Not surprisingly KeyNumber is the number of keys at ...
*/

int game::KeyQuestion(cfestring& Message, int DefaultAnswer, int KeyNumber, ...)
{
  int* Key = new int[KeyNumber];
  va_list Arguments;
  va_start(Arguments, KeyNumber);

  for(int c = 0; c < KeyNumber; ++c)
    Key[c] = va_arg(Arguments, int);

  va_end(Arguments);
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, v2(16, 8), WHITE, "%s", Message.CStr());
  graphics::BlitDBToScreen();
  int Return = 0;

  while(!Return)
  {
    int k = GET_KEY();

    for(int c = 0; c < KeyNumber; ++c)
      if(Key[c] == k)
      {
	Return = k;
	break;
      }

    if(!Return && DefaultAnswer != REQUIRES_ANSWER)
      Return = DefaultAnswer;
  }

  delete [] Key;
  igraph::BlitBackGround(v2(16, 6), v2(GetScreenXSize() << 4, 23));
  return Return;
}

v2 game::LookKeyHandler(v2 CursorPos, int Key)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);

  switch(Key)
  {
   case 'i':
    if(!IsInWilderness())
      if(Square->CanBeSeenByPlayer() || CursorPos == Player->GetPos() || GetSeeWholeMapCheatMode())
      {
	lsquare* LSquare = GetCurrentLevel()->GetLSquare(CursorPos);
	stack* Stack = LSquare->GetStack();

	if(LSquare->IsTransparent() && Stack->GetVisibleItems(Player))
	  Stack->DrawContents(Player, "Items here", NO_SELECT|(GetSeeWholeMapCheatMode() ? 0 : NO_SPECIAL_INFO));
	else
	  ADD_MESSAGE("You see no items here.");
      }
      else
	ADD_MESSAGE("You should perhaps move a bit closer.");

    break;
   case 'c':
    if(Square->CanBeSeenByPlayer() || CursorPos == Player->GetPos() || GetSeeWholeMapCheatMode())
    {
      character* Char = Square->GetCharacter();

      if(Char && (Char->CanBeSeenByPlayer() || Char->IsPlayer() || GetSeeWholeMapCheatMode()))
	Char->PrintInfo();
      else
	ADD_MESSAGE("You see no one here.");
    }
    else
      ADD_MESSAGE("You should perhaps move a bit closer.");

    break;
  }

  return CursorPos;
}

v2 game::NameKeyHandler(v2 CursorPos, int Key)
{
  if(SelectPet(Key))
    return LastPetUnderCursor->GetPos();

  if(Key == 'n' || Key == 'N')
  {
    character* Char = GetCurrentArea()->GetSquare(CursorPos)->GetCharacter();

    if(Char && Char->CanBeSeenByPlayer())
      Char->TryToName();
    else
      ADD_MESSAGE("You don't see anyone here to name.");
  }

  return CursorPos;
}

void game::End(festring DeathMessage, truth Permanently, truth AndGoToMenu)
{
  globalwindowhandler::DeInstallControlLoop(AnimationController);
  SetIsRunning(false);

  if(Permanently || !WizardModeIsReallyActive())
    RemoveSaves(Permanently);

  if(Permanently && !WizardModeIsReallyActive())
  {
    highscore HScore;

    if(HScore.LastAddFailed())
    {
      iosystem::TextScreen(CONST_S("You didn't manage to get onto the high score list.\n\n\n\n")
			   + GetPlayerName() + ", " + DeathMessage + "\nRIP");
    }
    else
      HScore.Draw();
  }

  if(AndGoToMenu)
  {
    /* This prevents monster movement etc. after death. */

    throw quitrequest();
  }
}

int game::CalculateRoughDirection(v2 Vector)
{
  if(!Vector.X && !Vector.Y)
    return YOURSELF;

  double Angle = femath::CalculateAngle(Vector);

  if(Angle < FPI / 8)
    return 4;
  else if(Angle < 3 * FPI / 8)
    return 7;
  else if(Angle < 5 * FPI / 8)
    return 6;
  else if(Angle < 7 * FPI / 8)
    return 5;
  else if(Angle < 9 * FPI / 8)
    return 3;
  else if(Angle < 11 * FPI / 8)
    return 0;
  else if(Angle < 13 * FPI / 8)
    return 1;
  else if(Angle < 15 * FPI / 8)
    return 2;
  else
    return 4;
}

int game::Menu(bitmap* BackGround, v2 Pos, cfestring& Topic, cfestring& sMS, col16 Color, cfestring& SmallText1, cfestring& SmallText2)
{
  globalwindowhandler::DisableControlLoops();
  int Return = iosystem::Menu(BackGround, Pos, Topic, sMS, Color, SmallText1, SmallText2);
  globalwindowhandler::EnableControlLoops();
  return Return;
}

void game::InitDangerMap()
{
  truth First = true;

  for(int c1 = 1; c1 < protocontainer<character>::GetSize(); ++c1)
  {
    BusyAnimation();
    const character::prototype* Proto = protocontainer<character>::GetProto(c1);
    const character::database*const* ConfigData = Proto->GetConfigData();
    int ConfigSize = Proto->GetConfigSize();

    for(int c2 = 0; c2 < ConfigSize; ++c2)
      if(!ConfigData[c2]->IsAbstract)
      {
	int Config = ConfigData[c2]->Config;

	if(First)
	{
	  NextDangerIDType = c1;
	  NextDangerIDConfigIndex = c2;
	  First = false;
	}

	character* Char = Proto->Spawn(Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	double NakedDanger = Char->GetRelativeDanger(Player, true);
	delete Char;
	Char = Proto->Spawn(Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	double EquippedDanger = Char->GetRelativeDanger(Player, true);
	delete Char;
	DangerMap[configid(c1, Config)] = dangerid(NakedDanger, EquippedDanger);
      }
  }
}

void game::CalculateNextDanger()
{
  if(IsInWilderness() || !*CurrentLevel->GetLevelScript()->GenerateMonsters())
    return;

  const character::prototype* Proto = protocontainer<character>::GetProto(NextDangerIDType);
  const character::database*const* ConfigData = Proto->GetConfigData();
  const character::database* DataBase = ConfigData[NextDangerIDConfigIndex];
  dangermap::iterator DangerIterator = DangerMap.find(configid(NextDangerIDType, DataBase->Config));
  team* Team = GetTeam(PLAYER_TEAM);

  if(DataBase && DangerIterator != DangerMap.end())
  {
    character* Char = Proto->Spawn(DataBase->Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
    std::list<character*>::const_iterator i;
    double DangerSum = Player->GetRelativeDanger(Char, true);

    for(i = Team->GetMember().begin(); i != Team->GetMember().end(); ++i)
      if((*i)->IsEnabled() && !(*i)->IsTemporary() && !RAND_N(10))
	DangerSum += (*i)->GetRelativeDanger(Char, true) / 4;

    double CurrentDanger = 1 / DangerSum;
    double NakedDanger = DangerIterator->second.NakedDanger;
    delete Char;

    if(NakedDanger > CurrentDanger)
      DangerIterator->second.NakedDanger = (NakedDanger * 9 + CurrentDanger) / 10;

    Char = Proto->Spawn(DataBase->Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
    DangerSum = Player->GetRelativeDanger(Char, true);

    for(i = Team->GetMember().begin(); i != Team->GetMember().end(); ++i)
      if((*i)->IsEnabled() && !(*i)->IsTemporary() && !RAND_N(10))
	DangerSum += (*i)->GetRelativeDanger(Char, true) / 4;

    CurrentDanger = 1 / DangerSum;
    double EquippedDanger = DangerIterator->second.EquippedDanger;
    delete Char;

    if(EquippedDanger > CurrentDanger)
      DangerIterator->second.EquippedDanger = (EquippedDanger * 9 + CurrentDanger) / 10;

    if(++NextDangerIDConfigIndex < Proto->GetConfigSize())
      return;

    for(;;)
    {
      if(++NextDangerIDType >= protocontainer<character>::GetSize())
	NextDangerIDType = 1;

      Proto = protocontainer<character>::GetProto(NextDangerIDType);
      ConfigData = Proto->GetConfigData();
      int ConfigSize = Proto->GetConfigSize();

      for(int c = 0; c < ConfigSize; ++c)
	if(!ConfigData[c]->IsAbstract)
	{
	  NextDangerIDConfigIndex = c;
	  return;
	}
    }
  }
  else
    ABORT("It is dangerous to go ice fishing in the summer.");
}

truth game::TryTravel(int Dungeon, int Area, int EntryIndex, truth AllowHostiles, truth AlliesFollow)
{
  charactervector Group;

  if(LeaveArea(Group, AllowHostiles, AlliesFollow))
  {
    CurrentDungeonIndex = Dungeon;
    EnterArea(Group, Area, EntryIndex);
    return true;
  }
  else
    return false;
}

truth game::LeaveArea(charactervector& Group, truth AllowHostiles, truth AlliesFollow)
{
  if(!IsInWilderness())
  {
    if(AlliesFollow && !GetCurrentLevel()->CollectCreatures(Group, Player, AllowHostiles))
      return false;

    Player->Remove();
    GetCurrentDungeon()->SaveLevel(SaveName(), CurrentLevelIndex);
  }
  else
  {
    Player->Remove();
    GetWorldMap()->GetPlayerGroup().swap(Group);
    SaveWorldMap();
  }

  return true;
}

/* Used always when the player enters an area. */

void game::EnterArea(charactervector& Group, int Area, int EntryIndex)
{
  if(Area != WORLD_MAP)
  {
    Generating = true;
    SetIsInWilderness(false);
    CurrentLevelIndex = Area;
    truth New = !PrepareRandomBone(Area) && !GetCurrentDungeon()->PrepareLevel(Area);
    igraph::CreateBackGround(*CurrentLevel->GetLevelScript()->GetBackGroundType());
    GetCurrentArea()->SendNewDrawRequest();
    v2 Pos = GetCurrentLevel()->GetEntryPos(Player, EntryIndex);

    if(Player)
    {
      GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();
      Player->PutToOrNear(Pos);
    }
    else
      SetPlayer(GetCurrentLevel()->GetLSquare(Pos)->GetCharacter());

    uint c;

    for(c = 0; c < Group.size(); ++c)
    {
      v2 NPCPos = GetCurrentLevel()->GetNearestFreeSquare(Group[c], Pos);

      if(NPCPos == ERROR_V2)
	NPCPos = GetCurrentLevel()->GetRandomSquare(Group[c]);

      Group[c]->PutTo(NPCPos);
    }

    GetCurrentLevel()->FiatLux();
    ctruth* AutoReveal = GetCurrentLevel()->GetLevelScript()->AutoReveal();

    if(New && AutoReveal && *AutoReveal)
      GetCurrentLevel()->Reveal();

    ShowLevelMessage();
    SendLOSUpdateRequest();
    UpdateCamera();

    /* Gum solution! */

    if(New && CurrentDungeonIndex == ATTNAM && Area == 0)
    {
      GlobalRainLiquid = powder::Spawn(SNOW);
      GlobalRainSpeed = v2(-64, 128);
      CurrentLevel->CreateGlobalRain(GlobalRainLiquid, GlobalRainSpeed);
    }

    if(New && CurrentDungeonIndex == NEW_ATTNAM && Area == 0)
    {
      GlobalRainLiquid = liquid::Spawn(WATER);
      GlobalRainSpeed = v2(256, 512);
      CurrentLevel->CreateGlobalRain(GlobalRainLiquid, GlobalRainSpeed);
    }

    if(New && CurrentDungeonIndex == ELPURI_CAVE && Area == OREE_LAIR)
    {
      GlobalRainLiquid = liquid::Spawn(BLOOD);
      GlobalRainSpeed = v2(256, 512);
      CurrentLevel->CreateGlobalRain(GlobalRainLiquid, GlobalRainSpeed);
      GlobalRainLiquid->SetVolumeNoSignals(200);
      CurrentLevel->EnableGlobalRain();
    }

    Generating = false;
    GetCurrentLevel()->UpdateLOS();
    Player->SignalStepFrom(0);

    for(c = 0; c < Group.size(); ++c)
      Group[c]->SignalStepFrom(0);

    if(ivanconfig::GetAutoSaveInterval())
      Save(GetAutoSaveFileName().CStr());
  }
  else
  {
    igraph::CreateBackGround(GRAY_FRACTAL);
    SetIsInWilderness(true);
    LoadWorldMap();
    SetCurrentArea(WorldMap);
    CurrentWSquareMap = WorldMap->GetMap();
    GetWorldMap()->GetPlayerGroup().swap(Group);
    Player->PutTo(GetWorldMap()->GetEntryPos(Player, EntryIndex));
    SendLOSUpdateRequest();
    UpdateCamera();
    GetWorldMap()->UpdateLOS();

    if(ivanconfig::GetAutoSaveInterval())
      Save(GetAutoSaveFileName().CStr());
  }
}

int game::CompareLightToInt(col24 L, col24 Int)
{
  if((L & 0xFF0000) > Int || (L & 0xFF00) > Int || (L & 0xFF) > Int)
    return 1;
  else if((L & 0xFF0000) == Int || (L & 0xFF00) == Int || (L & 0xFF) == Int)
    return 0;
  else
    return -1;
}

void game::SetStandardListAttributes(felist& List)
{
  List.SetPos(v2(26, 42));
  List.SetWidth(652);
  List.SetFlags(DRAW_BACKGROUND_AFTERWARDS);
  List.SetUpKey(GetMoveCommandKey(KEY_UP_INDEX));
  List.SetDownKey(GetMoveCommandKey(KEY_DOWN_INDEX));
}

void game::InitPlayerAttributeAverage()
{
  AveragePlayerArmStrengthExperience
    = AveragePlayerLegStrengthExperience
    = AveragePlayerDexterityExperience
    = AveragePlayerAgilityExperience
    = 0;

  if(!Player->IsHumanoid())
    return;

  humanoid* Player = static_cast<humanoid*>(GetPlayer());
  int Arms = 0;
  int Legs = 0;
  arm* RightArm = Player->GetRightArm();

  if(RightArm && !RightArm->UseMaterialAttributes())
  {
    AveragePlayerArmStrengthExperience += RightArm->GetStrengthExperience();
    AveragePlayerDexterityExperience += RightArm->GetDexterityExperience();
    ++Arms;
  }

  arm* LeftArm = Player->GetLeftArm();

  if(LeftArm && !LeftArm->UseMaterialAttributes())
  {
    AveragePlayerArmStrengthExperience += LeftArm->GetStrengthExperience();
    AveragePlayerDexterityExperience += LeftArm->GetDexterityExperience();
    ++Arms;
  }

  leg* RightLeg = Player->GetRightLeg();

  if(RightLeg && !RightLeg->UseMaterialAttributes())
  {
    AveragePlayerLegStrengthExperience += RightLeg->GetStrengthExperience();
    AveragePlayerAgilityExperience += RightLeg->GetAgilityExperience();
    ++Legs;
  }

  leg* LeftLeg = Player->GetLeftLeg();

  if(LeftLeg && !LeftLeg->UseMaterialAttributes())
  {
    AveragePlayerLegStrengthExperience += LeftLeg->GetStrengthExperience();
    AveragePlayerAgilityExperience += LeftLeg->GetAgilityExperience();
    ++Legs;
  }

  if(Arms)
  {
    AveragePlayerArmStrengthExperience /= Arms;
    AveragePlayerDexterityExperience /= Arms;
  }

  if(Legs)
  {
    AveragePlayerLegStrengthExperience /= Legs;
    AveragePlayerAgilityExperience /= Legs;
  }
}

void game::UpdatePlayerAttributeAverage()
{
  if(!Player->IsHumanoid())
    return;

  humanoid* Player = static_cast<humanoid*>(GetPlayer());
  double PlayerArmStrengthExperience = 0;
  double PlayerLegStrengthExperience = 0;
  double PlayerDexterityExperience = 0;
  double PlayerAgilityExperience = 0;
  int Arms = 0;
  int Legs = 0;
  arm* RightArm = Player->GetRightArm();

  if(RightArm && !RightArm->UseMaterialAttributes())
  {
    PlayerArmStrengthExperience += RightArm->GetStrengthExperience();
    PlayerDexterityExperience += RightArm->GetDexterityExperience();
    ++Arms;
  }

  arm* LeftArm = Player->GetLeftArm();

  if(LeftArm && !LeftArm->UseMaterialAttributes())
  {
    PlayerArmStrengthExperience += LeftArm->GetStrengthExperience();
    PlayerDexterityExperience += LeftArm->GetDexterityExperience();
    ++Arms;
  }

  leg* RightLeg = Player->GetRightLeg();

  if(RightLeg && !RightLeg->UseMaterialAttributes())
  {
    PlayerLegStrengthExperience += RightLeg->GetStrengthExperience();
    PlayerAgilityExperience += RightLeg->GetAgilityExperience();
    ++Legs;
  }

  leg* LeftLeg = Player->GetLeftLeg();

  if(LeftLeg && !LeftLeg->UseMaterialAttributes())
  {
    PlayerLegStrengthExperience += LeftLeg->GetStrengthExperience();
    PlayerAgilityExperience += LeftLeg->GetAgilityExperience();
    ++Legs;
  }

  if(Arms)
  {
    AveragePlayerArmStrengthExperience = (49 * AveragePlayerArmStrengthExperience + PlayerArmStrengthExperience / Arms) / 50;
    AveragePlayerDexterityExperience = (49 * AveragePlayerDexterityExperience + PlayerDexterityExperience / Arms) / 50;
  }

  if(Legs)
  {
    AveragePlayerLegStrengthExperience = (49 * AveragePlayerLegStrengthExperience + PlayerLegStrengthExperience / Legs) / 50;
    AveragePlayerAgilityExperience = (49 * AveragePlayerAgilityExperience + PlayerAgilityExperience / Legs) / 50;
  }
}

void game::CallForAttention(v2 Pos, int RangeSquare)
{
  for(int c = 0; c < GetTeams(); ++c)
  {
    if(GetTeam(c)->HasEnemy())
      for(std::list<character*>::const_iterator i = GetTeam(c)->GetMember().begin(); i != GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	{
	  long ThisDistance = HypotSquare(long((*i)->GetPos().X) - Pos.X, long((*i)->GetPos().Y) - Pos.Y);

	  if(ThisDistance <= RangeSquare && !(*i)->IsGoingSomeWhere())
	    (*i)->SetGoingTo(Pos);
	}
  }
}

outputfile& operator<<(outputfile& SaveFile, const homedata* HomeData)
{
  if(HomeData)
  {
    SaveFile.Put(1);
    SaveFile << HomeData->Pos << HomeData->Dungeon << HomeData->Level << HomeData->Room;
  }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, homedata*& HomeData)
{
  if(SaveFile.Get())
  {
    HomeData = new homedata;
    SaveFile >> HomeData->Pos >> HomeData->Dungeon >> HomeData->Level >> HomeData->Room;
  }

  return SaveFile;
}

ulong game::CreateNewCharacterID(character* NewChar)
{
  ulong ID = NextCharacterID++;

  if(CharacterIDMap.find(ID) != CharacterIDMap.end())
    int esko = esko = 2;

  CharacterIDMap.insert(std::make_pair(ID, NewChar));
  return ID;
}

ulong game::CreateNewItemID(item* NewItem)
{
  ulong ID = NextItemID++;

  if(ItemIDMap.find(ID) != ItemIDMap.end())
    int esko = esko = 2;

  if(NewItem)
    ItemIDMap.insert(std::make_pair(ID, NewItem));

  return ID;
}

ulong game::CreateNewTrapID(entity* NewTrap)
{
  ulong ID = NextTrapID++;

  if(TrapIDMap.find(ID) != TrapIDMap.end())
    int esko = esko = 2;

  if(NewTrap)
    TrapIDMap.insert(std::make_pair(ID, NewTrap));

  return ID;
}

character* game::SearchCharacter(ulong ID)
{
  characteridmap::iterator Iterator = CharacterIDMap.find(ID);
  return Iterator != CharacterIDMap.end() ? Iterator->second : 0;
}

item* game::SearchItem(ulong ID)
{
  itemidmap::iterator Iterator = ItemIDMap.find(ID);
  return Iterator != ItemIDMap.end() ? Iterator->second : 0;
}

entity* game::SearchTrap(ulong ID)
{
  trapidmap::iterator Iterator = TrapIDMap.find(ID);
  return Iterator != TrapIDMap.end() ? Iterator->second : 0;
}

outputfile& operator<<(outputfile& SaveFile, const configid& Value)
{
  SaveFile.Write(reinterpret_cast<cchar*>(&Value), sizeof(Value));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, configid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const dangerid& Value)
{
  SaveFile << Value.NakedDanger << Value.EquippedDanger;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dangerid& Value)
{
  SaveFile >> Value.NakedDanger >> Value.EquippedDanger;
  return SaveFile;
}

/* The program can only create directories to the deepness of one, no more... */

festring game::GetHomeDir()
{
#ifdef LINUX
  festring Dir;
  Dir << getenv("HOME") << '/';
  return Dir;
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetSaveDir()
{
#ifdef LINUX
  festring Dir;
  Dir << getenv("HOME") << "/IvanSave/";
  return Dir;
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Save/";
#endif
}

festring game::GetGameDir()
{
#ifdef LINUX
  return DATADIR "/ivan/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetBoneDir()
{
#ifdef LINUX
  return LOCAL_STATE_DIR "/Bones/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Bones/";
#endif
}

level* game::GetLevel(int I)
{
  return GetCurrentDungeon()->GetLevel(I);
}

int game::GetLevels()
{
  return GetCurrentDungeon()->GetLevels();
}

void game::SignalDeath(ccharacter* Ghost, ccharacter* Murderer, festring DeathMsg)
{
  if(InWilderness)
    DeathMsg << " in the world map";
  else
    DeathMsg << " in " << GetCurrentDungeon()->GetLevelDescription(CurrentLevelIndex);

  massacremap* MassacreMap;

  if(!Murderer)
  {
    ++MiscMassacreAmount;
    MassacreMap = &MiscMassacreMap;
  }
  else if(Murderer->IsPlayer())
  {
    ++PlayerMassacreAmount;
    MassacreMap = &PlayerMassacreMap;
  }
  else if(Murderer->IsPet())
  {
    ++PetMassacreAmount;
    MassacreMap = &PetMassacreMap;
  }
  else
  {
    ++MiscMassacreAmount;
    MassacreMap = &MiscMassacreMap;
  }

  massacreid MI(Ghost->GetType(), Ghost->GetConfig(), Ghost->GetAssignedName());
  massacremap::iterator i = MassacreMap->find(MI);

  if(i == MassacreMap->end())
  {
    i = MassacreMap->insert(std::make_pair(MI, killdata(1, Ghost->GetGenerationDanger()))).first;
    i->second.Reason.push_back(killreason(DeathMsg, 1));
  }
  else
  {
    ++i->second.Amount;
    i->second.DangerSum += Ghost->GetGenerationDanger();
    std::vector<killreason>& Reason = i->second.Reason;
    uint c;

    for(c = 0; c < Reason.size(); ++c)
      if(Reason[c].String == DeathMsg)
      {
	++Reason[c].Amount;
	break;
      }

    if(c == Reason.size())
      Reason.push_back(killreason(DeathMsg, 1));
  }
}

void game::DisplayMassacreLists()
{
  DisplayMassacreList(PlayerMassacreMap, "directly by you.", PlayerMassacreAmount);
  DisplayMassacreList(PetMassacreMap, "by your allies.", PetMassacreAmount);
  DisplayMassacreList(MiscMassacreMap, "by some other reason.", MiscMassacreAmount);
}

struct massacresetentry
{
  bool operator<(const massacresetentry& MSE) const
  {
    return festring::IgnoreCaseCompare(Key, MSE.Key);
  }
  festring Key;
  festring String;
  std::vector<festring> Details;
  int ImageKey;
};

void game::DisplayMassacreList(const massacremap& MassacreMap, cchar* Reason, long Amount)
{
  std::set<massacresetentry> MassacreSet;
  festring FirstPronoun;
  truth First = true;
  charactervector GraveYard;

  for(massacremap::const_iterator i1 = MassacreMap.begin(); i1 != MassacreMap.end(); ++i1)
  {
    character* Victim = protocontainer<character>::GetProto(i1->first.Type)->Spawn(i1->first.Config);
    Victim->SetAssignedName(i1->first.Name);
    massacresetentry Entry;
    GraveYard.push_back(Victim);
    Entry.ImageKey = AddToCharacterDrawVector(Victim);

    if(i1->second.Amount == 1)
    {
      Victim->AddName(Entry.Key, UNARTICLED);
      Victim->AddName(Entry.String, INDEFINITE);
    }
    else
    {
      Victim->AddName(Entry.Key, PLURAL);
      Entry.String << i1->second.Amount << ' ' << Entry.Key;
    }

    if(First)
    {
      FirstPronoun = Victim->GetSex() == UNDEFINED ? "it" : Victim->GetSex() == MALE ? "he" : "she";
      First = false;
    }

    const std::vector<killreason>& Reason = i1->second.Reason;
    std::vector<festring>& Details = Entry.Details;

    if(Reason.size() == 1)
    {
      festring Begin;

      if(Reason[0].Amount == 1)
	Begin = "";
      else if(Reason[0].Amount == 2)
	Begin = "both ";
      else
	Begin = "all ";

      Details.push_back(Begin + Reason[0].String);
    }
    else
    {
      for(uint c = 0; c < Reason.size(); ++c)
	Details.push_back(CONST_S("") + Reason[c].Amount + ' ' + Reason[c].String);

      std::sort(Details.begin(), Details.end(), ignorecaseorderer());
    }

    MassacreSet.insert(Entry);
  }

  long Total = PlayerMassacreAmount + PetMassacreAmount + MiscMassacreAmount;
  festring MainTopic;

  if(Total == 1)
    MainTopic << "One creature perished during your adventure.";
  else
    MainTopic << Total << " creatures perished during your adventure.";

  felist List(MainTopic);
  SetStandardListAttributes(List);
  List.SetPageLength(15);
  List.AddFlags(SELECTABLE);
  List.SetEntryDrawer(CharacterEntryDrawer);
  List.AddDescription(CONST_S(""));
  festring SideTopic;

  if(Amount != Total)
  {
    SideTopic = CONST_S("The following ");

    if(Amount == 1)
      SideTopic << "one was killed " << Reason;
    else
      SideTopic << Amount << " were killed " << Reason;
  }
  else
  {
    if(Amount == 1)
    {
      FirstPronoun.Capitalize();
      SideTopic << FirstPronoun << " was killed " << Reason;
    }
    else
      SideTopic << "They were all killed " << Reason;
  }

  List.AddDescription(SideTopic);
  List.AddDescription(CONST_S(""));
  List.AddDescription("Choose a type of creatures to browse death details.");
  std::set<massacresetentry>::const_iterator i2;

  for(i2 = MassacreSet.begin(); i2 != MassacreSet.end(); ++i2)
    List.AddEntry(i2->String, LIGHT_GRAY, 0, i2->ImageKey);

  for(;;)
  {
    int Chosen = List.Draw();

    if(Chosen & FELIST_ERROR_BIT)
      break;

    felist SubList(CONST_S("Massacre details"));
    SetStandardListAttributes(SubList);
    SubList.SetPageLength(20);
    int Counter = 0;

    for(i2 = MassacreSet.begin(); i2 != MassacreSet.end(); ++i2, ++Counter)
      if(Counter == Chosen)
      {
	for(uint c = 0; c < i2->Details.size(); ++c)
	  SubList.AddEntry(i2->Details[c], LIGHT_GRAY);

	break;
      }

    SubList.Draw();
  }

  ClearCharacterDrawVector();

  for(uint c = 0; c < GraveYard.size(); ++c)
    delete GraveYard[c];
}

truth game::MassacreListsEmpty()
{
  return PlayerMassacreMap.empty() && PetMassacreMap.empty() && MiscMassacreMap.empty();
}

#ifdef WIZARD

void game::SeeWholeMap()
{
  if(SeeWholeMapCheatMode < 2)
    ++SeeWholeMapCheatMode;
  else
    SeeWholeMapCheatMode = 0;

  GetCurrentArea()->SendNewDrawRequest();
}

#endif

void game::CreateBone()
{
  if(!WizardModeIsActive() && !IsInWilderness() && RAND() & 3 && GetCurrentLevel()->PreProcessForBone())
  {
    int BoneIndex;
    festring BoneName;

    for(BoneIndex = 0; BoneIndex < 1000; ++BoneIndex)
    {
      BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + CurrentLevelIndex + BoneIndex;
      inputfile BoneFile(BoneName, 0, false);

      if(!BoneFile.IsOpen())
	break;
    }

    if(BoneIndex != 1000)
    {
      festring BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + CurrentLevelIndex + BoneIndex;
      outputfile BoneFile(BoneName);
      BoneFile << int(BONE_FILE_VERSION) << PlayerName << CurrentLevel;
    }
  }
}

truth game::PrepareRandomBone(int LevelIndex)
{
  if(WizardModeIsActive() || GetCurrentDungeon()->IsGenerated(LevelIndex) || !*GetCurrentDungeon()->GetLevelScript(LevelIndex)->CanGenerateBone())
    return false;

  int BoneIndex;
  festring BoneName;

  for(BoneIndex = 0; BoneIndex < 1000; ++BoneIndex)
  {
    BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + LevelIndex + BoneIndex;
    inputfile BoneFile(BoneName, 0, false);

    if(BoneFile.IsOpen() && !(RAND() & 7))
    {
      if(ReadType<int>(BoneFile) != BONE_FILE_VERSION)
      {
	BoneFile.Close();
	remove(BoneName.CStr());
	continue;
      }

      festring Name;
      BoneFile >> Name;
      level* NewLevel = GetCurrentDungeon()->LoadLevel(BoneFile, LevelIndex);

      if(!NewLevel->PostProcessForBone())
      {
	delete NewLevel;
	GetBoneItemIDMap().clear();
	GetBoneCharacterIDMap().clear();
	continue;
      }

      NewLevel->FinalProcessForBone();
      GetBoneItemIDMap().clear();
      GetBoneCharacterIDMap().clear();
      SetCurrentArea(NewLevel);
      CurrentLevel = NewLevel;
      CurrentLSquareMap = NewLevel->GetMap();
      GetCurrentDungeon()->SetIsGenerated(LevelIndex, true);

      if(Name == PlayerName)
	ADD_MESSAGE("This place is oddly familiar. Like you had been here in one of your past lives.");
      else
	ADD_MESSAGE("You smell the stench of death.");

      break;
    }
  }

  Generating = true;

  if(BoneIndex != 1000)
  {
    remove(BoneName.CStr());
    return true;
  }
  else
    return false;
}

double game::CalculateAverageDanger(const charactervector& EnemyVector, character* Char)
{
  double DangerSum = 0;
  int Enemies = 0;

  for(uint c = 0; c < EnemyVector.size(); ++c)
  {
    DangerSum += EnemyVector[c]->GetRelativeDanger(Char, true);
    ++Enemies;
  }

  return DangerSum / Enemies;
}

double game::CalculateAverageDangerOfAllNormalEnemies()
{
  double DangerSum = 0;
  int Enemies = 0;

  for(int c1 = 1; c1 < protocontainer<character>::GetSize(); ++c1)
  {
    const character::prototype* Proto = protocontainer<character>::GetProto(c1);
    const character::database*const* ConfigData = Proto->GetConfigData();
    int ConfigSize = Proto->GetConfigSize();

    for(int c2 = 0; c2 < ConfigSize; ++c2)
      if(!ConfigData[c2]->IsAbstract
	 && !ConfigData[c2]->IsUnique
	 && ConfigData[c2]->CanBeGenerated)
      {
	DangerSum += DangerMap.find(configid(c1, ConfigData[c2]->Config))->second.EquippedDanger;
	++Enemies;
      }
  }

  return DangerSum / Enemies;
}

character* game::CreateGhost()
{
  double AverageDanger = CalculateAverageDangerOfAllNormalEnemies();
  charactervector EnemyVector;
  protosystem::CreateEveryNormalEnemy(EnemyVector);
  ghost* Ghost = ghost::Spawn();
  Ghost->SetTeam(GetTeam(MONSTER_TEAM));
  Ghost->SetGenerationDanger(CurrentLevel->GetDifficulty());
  Ghost->SetOwnerSoul(PlayerName);
  Ghost->SetIsActive(false);
  Ghost->EditAllAttributes(-4);
  Player->SetSoulID(Ghost->GetID());
  while(CalculateAverageDanger(EnemyVector, Ghost) > AverageDanger && Ghost->EditAllAttributes(1));

  for(uint c = 0; c < EnemyVector.size(); ++c)
    delete EnemyVector[c];

  return Ghost;
}

int game::GetMoveCommandKey(int I)
{
  if(!ivanconfig::GetUseAlternativeKeys())
    return MoveNormalCommandKey[I];
  else
    return MoveAbnormalCommandKey[I];
}

long game::GetScore()
{
  double Counter = 0;
  massacremap::const_iterator i;
  massacremap SumMap = PlayerMassacreMap;

  for(i = PetMassacreMap.begin(); i != PetMassacreMap.end(); ++i)
  {
    killdata& KillData = SumMap[i->first];
    KillData.Amount += i->second.Amount;
    KillData.DangerSum += i->second.DangerSum;
  }

  for(i = SumMap.begin(); i != SumMap.end(); ++i)
  {
    character* Char = protocontainer<character>::GetProto(i->first.Type)->Spawn(i->first.Config);
    int SumOfAttributes = Char->GetSumOfAttributes();
    Counter += sqrt(i->second.DangerSum / DEFAULT_GENERATION_DANGER) * SumOfAttributes * SumOfAttributes;
    delete Char;
  }

  return long(0.01 * Counter);
}

/* Only works if New Attnam is loaded */

truth game::TweraifIsFree()
{
  for(std::list<character*>::const_iterator i = GetTeam(COLONIST_TEAM)->GetMember().begin(); i != GetTeam(COLONIST_TEAM)->GetMember().end(); ++i)
    if((*i)->IsEnabled())
      return false;

  return true;
}

truth game::IsXMas() // returns true if date is christmaseve or day
{
  time_t Time = time(0);
  struct tm* TM = localtime(&Time);
  return (TM->tm_mon == 11 && (TM->tm_mday == 24 || TM->tm_mday == 25));
}

int game::AddToItemDrawVector(const itemvector& What)
{
  ItemDrawVector.push_back(What);
  return ItemDrawVector.size() - 1;
}

v2 ItemDisplacement[3][3] =
{
  { v2(0, 0), ERROR_V2, ERROR_V2 },
  { v2(-2, -2), v2(2, 2), ERROR_V2 },
  { v2(-4, -4), v2(0, 0), v2(4, 4) }
};

void game::ItemEntryDrawer(bitmap* Bitmap, v2 Pos, uint I)
{
  blitdata B = { Bitmap,
		 { 0, 0 },
		 { 0, 0 },
		 { TILE_SIZE, TILE_SIZE },
		 { NORMAL_LUMINANCE },
		 TRANSPARENT_COLOR,
		 ALLOW_ANIMATE };

  itemvector ItemVector = ItemDrawVector[I];
  int Amount = Min<int>(ItemVector.size(), 3);

  for(int c = 0; c < Amount; ++c)
  {
    v2 Displacement = ItemDisplacement[Amount - 1][c];

    if(!ItemVector[0]->HasNormalPictureDirection())
      Displacement.X = -Displacement.X;

    B.Dest = Pos + Displacement;

    if(ItemVector[c]->AllowAlphaEverywhere())
      B.CustomData |= ALLOW_ALPHA;

    ItemVector[c]->Draw(B);
    B.CustomData &= ~ALLOW_ALPHA;
  }

  if(ItemVector.size() > 3)
  {
    B.Src.X = 0;
    B.Src.Y = 16;
    B.Dest = ItemVector[0]->HasNormalPictureDirection() ? Pos + v2(11, -2) : Pos + v2(-2, -2);
    B.Flags = 0;
    igraph::GetSymbolGraphic()->NormalMaskedBlit(B);
  }
}

int game::AddToCharacterDrawVector(character* What)
{
  CharacterDrawVector.push_back(What);
  return CharacterDrawVector.size() - 1;
}

void game::CharacterEntryDrawer(bitmap* Bitmap, v2 Pos, uint I)
{
  if(CharacterDrawVector[I])
  {
    blitdata B = { Bitmap,
		   { 0, 0 },
		   { Pos.X, Pos.Y },
		   { TILE_SIZE, TILE_SIZE },
		   { NORMAL_LUMINANCE },
		   TRANSPARENT_COLOR,
		   ALLOW_ANIMATE|ALLOW_ALPHA };

    CharacterDrawVector[I]->DrawBodyParts(B);
  }
}

void game::GodEntryDrawer(bitmap* Bitmap, v2 Pos, uint I)
{
  blitdata B = { Bitmap,
		 { I << 4, 0 },
		 { Pos.X, Pos.Y },
		 { TILE_SIZE, TILE_SIZE },
		 { 0 },
		 TRANSPARENT_COLOR,
		 0 };

  igraph::GetSymbolGraphic()->NormalMaskedBlit(B);
}

character* game::GetSumo()
{
  return GetCurrentLevel()->GetLSquare(SUMO_ROOM_POS)->GetRoom()->GetMaster();
}

truth game::TryToEnterSumoArena()
{
  character* Sumo = GetSumo();

  if(!Sumo || !Sumo->IsEnabled() || Sumo->GetRelation(Player) == HOSTILE || !Player->CanBeSeenBy(Sumo))
    return true;

  if(TweraifIsFree())
  {
    ADD_MESSAGE("\"You started this stupid revolution, after which I've been constantly hungry. Get lost!\"");
    return false;
  }

  if(PlayerIsSumoChampion())
  {
    ADD_MESSAGE("\"I don't really enjoy losing, especially many times to the same guy. Go away.\"");
    return false;
  }

  if(Player->IsPolymorphed())
  {
    ADD_MESSAGE("\"Don't try to cheat. Come back when you're normal again.\"");
    return false;
  }

  if(Player->GetHungerState() < SATIATED)
  {
    ADD_MESSAGE("\"Your figure is too slender for this sport. Eat a lot more and come back.\"");
    return false;
  }

  if(Player->GetHungerState() < BLOATED)
  {
    ADD_MESSAGE("\"You're still somewhat too thin. Eat some more and we'll compete.\"");
    return false;
  }

  ADD_MESSAGE("\"So you want to compete? Okay, I'll explain the rules. First, I'll make a mirror image out of us both. We'll enter the arena and fight till one is knocked out. Use of any equipment is not allowed. Note that we will not gain experience from fighting as a mirror image, but won't get really hurt, either. However, controlling the image is exhausting and you can get hungry very quickly.\"");

  if(!TruthQuestion("Do you want to challenge him? [y/N]"))
    return false;

  SumoWrestling = true;
  character* MirrorPlayer = Player->Duplicate(IGNORE_PROHIBITIONS);
  character* MirrorSumo = Sumo->Duplicate(IGNORE_PROHIBITIONS);
  SetPlayer(MirrorPlayer);
  charactervector Spectators;

  if(Player->GetTeam()->GetRelation(GetTeam(TOURIST_GUIDE_TEAM)) != HOSTILE
     && Player->GetTeam()->GetRelation(GetTeam(TOURIST_TEAM)) != HOSTILE)
  {
    GetTeam(TOURIST_GUIDE_TEAM)->MoveMembersTo(Spectators);
    GetTeam(TOURIST_TEAM)->MoveMembersTo(Spectators);
  }

  GetCurrentDungeon()->SaveLevel(SaveName(), 0);
  charactervector test;
  EnterArea(test, 1, STAIRS_UP);
  MirrorSumo->PutTo(SUMO_ARENA_POS + v2(6, 5));
  MirrorSumo->ChangeTeam(GetTeam(SUMO_TEAM));
  GetCurrentLevel()->GetLSquare(SUMO_ARENA_POS)->GetRoom()->SetMasterID(MirrorSumo->GetID());

  for(uint c = 0; c < Spectators.size(); ++c)
    Spectators[c]->PutToOrNear(SUMO_ARENA_POS + v2(6, 10));

  throw areachangerequest();
  return true;
}

truth game::TryToExitSumoArena()
{
  if(GetTeam(PLAYER_TEAM)->GetRelation(GetTeam(NEW_ATTNAM_TEAM)) == HOSTILE)
    return true;

  itemvector IVector;
  charactervector CVector;

  if(IsSumoWrestling())
  {
    if(TruthQuestion("Do you really wish to give up? [y/N]"))
      return EndSumoWrestling(LOST);
    else
      return false;
  }
  else
  {
    Player->Remove();
    GetCurrentLevel()->CollectEverything(IVector, CVector);
    GetCurrentDungeon()->SaveLevel(SaveName(), 1);
    std::vector<character*> test;
    EnterArea(test, 0, STAIRS_DOWN);
    Player->GetStackUnder()->AddItems(IVector);

    if(!IVector.empty())
    {
      character* Sumo = GetSumo();

      if(Sumo && Sumo->GetRelation(Player) != HOSTILE && Player->CanBeSeenBy(Sumo))
	ADD_MESSAGE("\"Don't leave anything there, please.\"");
    }

    v2 PlayerPos = Player->GetPos();

    for(uint c = 0; c < CVector.size(); ++c)
      CVector[c]->PutNear(PlayerPos);

    throw areachangerequest();
    return true;
  }
}

truth game::EndSumoWrestling(int Result)
{
  msgsystem::LeaveBigMessageMode();

  if(Result == LOST)
    AskForKeyPress("You lose. [press any key to continue]");
  else if(Result == WON)
    AskForKeyPress("You win! [press any key to continue]");
  else if(Result == DISQUALIFIED)
    AskForKeyPress("You are disqualified! [press any key to continue]");

  character* Sumo = GetCurrentLevel()->GetLSquare(SUMO_ARENA_POS)->GetRoom()->GetMaster();

  /* We'll make a throw soon so deletes are allowed */

  if(Sumo)
  {
    Sumo->Remove();
    delete Sumo;
  }

  Player->Remove();
  delete Player;
  SetPlayer(0);
  itemvector IVector;
  charactervector CVector;
  GetCurrentLevel()->CollectEverything(IVector, CVector);
  GetCurrentDungeon()->SaveLevel(SaveName(), 1);
  charactervector test;
  EnterArea(test, 0, STAIRS_DOWN);
  SumoWrestling = false;
  Player->GetStackUnder()->AddItems(IVector);
  v2 PlayerPos = Player->GetPos();

  for(uint c = 0; c < CVector.size(); ++c)
    CVector[c]->PutNear(PlayerPos);

  if(Result == LOST)
    ADD_MESSAGE("\"I hope you've learned your lesson now!\"");
  else if(Result == DISQUALIFIED)
    ADD_MESSAGE("\"Don't do that again or I'll be really angry!\"");
  else
  {
    PlayerSumoChampion = true;
    character* Sumo = GetSumo();
    festring Msg = Sumo->GetName(DEFINITE) + " seems humbler than before. \"Darn. You bested me.\n";
    Msg << "Here's a little something as a reward\", " << Sumo->GetPersonalPronoun() << " says and hands you a belt of levitation.\n\"";
    (belt::Spawn(BELT_OF_LEVITATION))->MoveTo(Player->GetStack());
    Msg << "Allow me to also teach you a few nasty martial art tricks the years have taught me.\"";
    Player->GetCWeaponSkill(UNARMED)->AddHit(100000);
    Player->GetCWeaponSkill(KICK)->AddHit(100000);
    character* Imperialist = GetCurrentLevel()->GetLSquare(5, 5)->GetRoom()->GetMaster();

    if(Imperialist && Imperialist->GetRelation(Player) != HOSTILE)
    {
      v2 Pos = Player->GetPos() + v2(0, 1);
      GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();
      Imperialist->Remove();
      Imperialist->PutTo(Pos);
      Msg << "\n\nSuddenly you notice " << Imperialist->GetName(DEFINITE) << " has also entered.\n"
	"\"I see we have a promising fighter among us. I had already heard of your\n"
	"adventures outside the village, but hardly could I believe that one day you\n"
	"would defeat even the mighty Huang Ming Pong! A hero such as you is bound\n"
	"to become world famous, and can earn a fortune if wealthy sponsors are behind\n"
	"him. May I therefore propose a mutually profitable contract: I'll give you this\n"
	"nice shirt with my company's ad, and you'll wear it as you journey bravely to\n"
	"the unknown and fight epic battles against the limitless minions of evil. I'll\n"
	"reward you well when you return, depending on how much you have used it.\"";
      Player->GetStack()->AddItem(decosadshirt::Spawn());
    }

    TextScreen(Msg);
    GetCurrentArea()->SendNewDrawRequest();
    DrawEverything();
  }

  Player->EditNP(-25000);
  Player->CheckStarvationDeath(CONST_S("exhausted after controlling a mirror image for too long"));
  throw areachangerequest();
  return true;
}

rain* game::ConstructGlobalRain()
{
  return new rain(GlobalRainLiquid, static_cast<lsquare*>(GetSquareInLoad()), GlobalRainSpeed, MONSTER_TEAM, false);
}

v2 game::GetSunLightDirectionVector()
{
  int Index = Tick % 48000 / 1000;

  /* Should have the same sign as sin(PI * Index / 24) and XTable[Index] /
     YTable[Index] should equal roughly -tan(PI * Index / 24). Also, vector
     (XTable[Index], YTable[Index]) + P should not be a valid position of
     any possible level L for any P belonging to L. */

  static int XTable[48] = {     0,  1000,  1000,  1000,  1000,  1000,
				1000,  1303,  1732,  2414,  3732,  7596,
				1000,  7596,  3732,  2414,  1732,  1303,
				1000,  1000,  1000,  1000,  1000,  1000,
			        0, -1000, -1000, -1000, -1000, -1000,
				-1000, -1303, -1732, -2414, -3732, -7596,
				-1000, -7596, -3732, -2414, -1732, -1303,
				-1000, -1000, -1000, -1000, -1000, -1000 };

  /* Should have the same sign as -cos(PI * Index / 24) */

  static int YTable[48] = { -1000, -7596, -3732, -2414, -1732, -1303,
			    -1000, -1000, -1000, -1000, -1000, -1000,
			    0,  1000,  1000,  1000,  1000,  1000,
			    1000,  1303,  1732,  2414,  3732,  7596,
			    1000,  7596,  3732,  2414,  1732,  1303,
			    1000,  1000,  1000,  1000,  1000,  1000,
			    0, -1000, -1000, -1000, -1000, -1000,
			    -1000, -1303, -1732, -2414, -3732, -7596 };

  return v2(XTable[Index], YTable[Index]);
}

int game::CalculateMinimumEmitationRadius(col24 E)
{
  int MaxElement = Max(GetRed24(E), GetGreen24(E), GetBlue24(E));
  return int(sqrt(double(MaxElement << 7) / LIGHT_BORDER - 120.));
}

ulong game::IncreaseSquarePartEmitationTicks()
{
  if((SquarePartEmitationTick += 2) == 0x100)
  {
    CurrentLevel->InitSquarePartEmitationTicks();
    SquarePartEmitationTick = 2;
  }

  return SquarePartEmitationTick;
}

void game::Wish(character* Wisher, cchar* MsgSingle, cchar* MsgPair)
{
  for(;;)
  {
    festring Temp = DefaultQuestion(CONST_S("What do you want to wish for?"),
				    DefaultWish);
    item* TempItem = protosystem::CreateItem(Temp, Wisher->IsPlayer());

    if(TempItem)
    {
      Wisher->GetStack()->AddItem(TempItem);
      TempItem->SpecialGenerationHandler();

      if(TempItem->HandleInPairs())
	ADD_MESSAGE(MsgPair, TempItem->CHAR_NAME(PLURAL));
      else
	ADD_MESSAGE(MsgSingle, TempItem->CHAR_NAME(INDEFINITE));

      return;
    }
  }
}

festring game::DefaultQuestion(festring Topic, festring& Default, stringkeyhandler KeyHandler)
{
  festring ShortDefault = Default;

  if(Default.GetSize() > 29)
  {
    ShortDefault.Resize(27);
    ShortDefault = ShortDefault << CONST_S("...");
  }

  if(!Default.IsEmpty())
    Topic << " [" << ShortDefault << ']';

  festring Answer = StringQuestion(Topic, WHITE, 0, 80, false, KeyHandler);

  if(Answer.IsEmpty())
    Answer = Default;

  return Default = Answer;
}

void game::GetTime(ivantime& Time)
{
  Time.Hour = 12 + Tick / 2000;
  Time.Day = Time.Hour / 24 + 1;
  Time.Hour %= 24;
  Time.Min = Tick % 2000 * 60 / 2000;
}

truth NameOrderer(character* C1, character* C2)
{
  return festring::IgnoreCaseCompare(C1->GetName(UNARTICLED), C2->GetName(UNARTICLED));
}

truth game::PolymorphControlKeyHandler(int Key, festring& String)
{
  if(Key == '?')
  {
    felist List(CONST_S("List of known creatures and their intelligence requirements"));
    SetStandardListAttributes(List);
    List.SetPageLength(15);
    List.AddFlags(SELECTABLE);
    protosystem::CreateEverySeenCharacter(CharacterDrawVector);
    std::sort(CharacterDrawVector.begin(), CharacterDrawVector.end(), NameOrderer);
    List.SetEntryDrawer(CharacterEntryDrawer);
    std::vector<festring> StringVector;
    uint c;

    for(c = 0; c < CharacterDrawVector.size(); ++c)
    {
      character* Char = CharacterDrawVector[c];

      if(Char->CanBeWished())
      {
	festring Entry;
	Char->AddName(Entry, UNARTICLED);
	StringVector.push_back(Entry);
	int Req = Char->GetPolymorphIntelligenceRequirement();

	if(Char->IsSameAs(Player)
	   || (Player->GetPolymorphBackup()
	       && Player->GetPolymorphBackup()->IsSameAs(Char)))
	  Req = 0;

	Entry << " (" << Req << ')';
	int Int = Player->GetAttribute(INTELLIGENCE);
	List.AddEntry(Entry, Req > Int ? RED : LIGHT_GRAY, 0, c);
      }
    }

    int Chosen = List.Draw();

    for(c = 0; c < CharacterDrawVector.size(); ++c)
      delete CharacterDrawVector[c];

    if(!(Chosen & FELIST_ERROR_BIT))
      String = StringVector[Chosen];

    CharacterDrawVector.clear();
    return true;
  }

  return false;
}

outputfile& operator<<(outputfile& SaveFile, const killdata& Value)
{
  SaveFile << Value.Amount << Value.DangerSum << Value.Reason;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, killdata& Value)
{
  SaveFile >> Value.Amount >> Value.DangerSum >> Value.Reason;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const killreason& Value)
{
  SaveFile << Value.Amount << Value.String;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, killreason& Value)
{
  SaveFile >> Value.Amount >> Value.String;
  return SaveFile;
}

truth DistanceOrderer(character* C1, character* C2)
{
  v2 PlayerPos = PLAYER->GetPos();
  v2 Pos1 = C1->GetPos();
  v2 Pos2 = C2->GetPos();
  int D1 = Max(abs(Pos1.X - PlayerPos.X), abs(Pos1.Y - PlayerPos.Y));
  int D2 = Max(abs(Pos2.X - PlayerPos.X), abs(Pos2.Y - PlayerPos.Y));

  if(D1 != D2)
    return D1 < D2;
  else if(Pos1.Y != Pos2.Y)
    return Pos1.Y < Pos2.Y;
  else
    return Pos1.X < Pos2.X;
}

truth game::FillPetVector(cchar* Verb)
{
  PetVector.clear();
  team* Team = GetTeam(PLAYER_TEAM);

  for(std::list<character*>::const_iterator i = Team->GetMember().begin();
      i != Team->GetMember().end(); ++i)
    if((*i)->IsEnabled() && !(*i)->IsPlayer() && (*i)->CanBeSeenByPlayer())
      PetVector.push_back(*i);

  if(PetVector.empty())
  {
    ADD_MESSAGE("You don't detect any friends to %s.", Verb);
    return false;
  }

  std::sort(PetVector.begin(), PetVector.end(), DistanceOrderer);
  LastPetUnderCursor = PetVector[0];
  return true;
}

truth game::CommandQuestion()
{
  if(!FillPetVector("command"))
    return false;

  character* Char;

  if(PetVector.size() == 1)
    Char = PetVector[0];
  else
  {
    v2 Pos = PetVector[0]->GetPos();
    Pos = PositionQuestion(CONST_S("Whom do you wish to command? [direction keys/'+'/'-'/'a'll/space/esc]"), Pos, &PetHandler, &CommandKeyHandler);

    if(Pos == ERROR_V2)
      return false;

    if(Pos == ABORT_V2)
      return true;

    Char = CurrentArea->GetSquare(Pos)->GetCharacter();

    if(!Char || !Char->CanBeSeenByPlayer())
    {
      ADD_MESSAGE("You don't see anyone here to command.");
      return false;
    }

    if(Char->IsPlayer())
    {
      ADD_MESSAGE("You do that all the time.");
      return false;
    }

    if(!Char->IsPet())
    {
      ADD_MESSAGE("%s refuses to be commanded by you.", Char->CHAR_NAME(DEFINITE));
      return false;
    }
  }

  return Char->IssuePetCommands();
}

void game::NameQuestion()
{
  if(!FillPetVector("name"))
    return;

  if(PetVector.size() == 1)
    PetVector[0]->TryToName();
  else
    PositionQuestion(CONST_S("Who do you want to name? [direction keys/'+'/'-'/'n'ame/esc]"), PetVector[0]->GetPos(), &PetHandler, &NameKeyHandler);
}

void game::PetHandler(v2 CursorPos)
{
  character* Char = CurrentArea->GetSquare(CursorPos)->GetCharacter();

  if(Char && Char->CanBeSeenByPlayer() && Char->IsPet() && !Char->IsPlayer())
    CursorData = RED_CURSOR|CURSOR_TARGET;
  else
    CursorData = RED_CURSOR;

  if(Char && !Char->IsPlayer() && Char->IsPet())
    LastPetUnderCursor = Char;
}

v2 game::CommandKeyHandler(v2 CursorPos, int Key)
{
  if(SelectPet(Key))
    return LastPetUnderCursor->GetPos();
  else if(Key == 'a' || Key == 'A')
    return CommandAll() ? ABORT_V2 : ERROR_V2;

  return CursorPos;
}

truth game::SelectPet(int Key)
{
  if(Key == '+')
  {
    for(uint c = 0; c < PetVector.size(); ++c)
      if(PetVector[c] == LastPetUnderCursor)
      {
	if(++c == PetVector.size())
	  c = 0;

	LastPetUnderCursor = PetVector[c];
	return true;
      }
  }
  else if(Key == '-')
  {
    for(uint c = 0; c < PetVector.size(); ++c)
      if(PetVector[c] == LastPetUnderCursor)
      {
	if(!c)
	  c = PetVector.size();

	LastPetUnderCursor = PetVector[--c];
	return true;
      }
  }

  return false;
}

void game::CommandScreen(cfestring& Topic, ulong PossibleFlags, ulong ConstantFlags, ulong& VaryFlags, ulong& Flags)
{
  static cchar* CommandDescription[COMMAND_FLAGS] =
    {
      "Follow me",
      "Flee from enemies",
      "Don't change your equipment",
      "Don't consume anything valuable"
    };

  felist List(Topic);
  SetStandardListAttributes(List);
  List.AddFlags(SELECTABLE);
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Command                                                        Active?"));

  for(;;)
  {
    int c, i;

    for(c = 0; c < COMMAND_FLAGS; ++c)
      if(1 << c & PossibleFlags)
      {
	truth Changeable = !(1 << c & ConstantFlags);
	festring Entry;

	if(Changeable)
	{
	  Entry = CommandDescription[c];
	  Entry.Resize(60);
	}
	else
	{
	  Entry << "   " << CommandDescription[c];
	  Entry.Resize(63);
	}

	if(1 << c & VaryFlags)
	  Entry << "varies";
	else
	  Entry << (1 << c & Flags ? "yes" : "no");

	List.AddEntry(Entry, Changeable ? LIGHT_GRAY : DARK_GRAY, 0, NO_IMAGE, Changeable);
      }

    int Chosen = List.Draw();

    if(Chosen & FELIST_ERROR_BIT)
      return;

    for(c = 0, i = 0; c < COMMAND_FLAGS; ++c)
      if(1 << c & PossibleFlags && !(1 << c & ConstantFlags) && i++ == Chosen)
      {
	if(1 << c & VaryFlags)
	{
	  VaryFlags &= ~(1 << c);
	  Flags |= 1 << c;
	}
	else
	  Flags ^= 1 << c;

	break;
      }

    List.Empty();
    DrawEverythingNoBlit();
  }
}

truth game::CommandAll()
{
  ulong PossibleFlags = 0, ConstantFlags = ALL_COMMAND_FLAGS, VaryFlags = 0, OldFlags = 0;
  uint c1, c2;

  for(c1 = 0; c1 < PetVector.size(); ++c1)
  {
    ConstantFlags &= PetVector[c1]->GetConstantCommandFlags();
    ulong C = PetVector[c1]->GetCommandFlags();
    ulong ThisPossible = PetVector[c1]->GetPossibleCommandFlags();

    for(c2 = 0; c2 < COMMAND_FLAGS; ++c2)
      if(1 << c2 & PossibleFlags & ThisPossible
	 && (1 << c2 & C) != (1 << c2 & OldFlags))
	VaryFlags |= 1 << c2;

    PossibleFlags |= ThisPossible;
    OldFlags |= C & ThisPossible;
  }

  if(!PossibleFlags)
  {
    ADD_MESSAGE("Not a single creature in your visible team can be commanded.");
    return false;
  }

  ulong NewFlags = OldFlags;
  CommandScreen(CONST_S("Issue commands to whole visible team"), PossibleFlags, ConstantFlags, VaryFlags, NewFlags);
  truth Change = false;

  for(c1 = 0; c1 < PetVector.size(); ++c1)
  {
    character* Char = PetVector[c1];

    if(!Char->IsConscious())
      continue;

    ulong OldC = Char->GetCommandFlags();
    ulong ConstC = Char->GetConstantCommandFlags();
    ulong ThisC = NewFlags
		  & Char->GetPossibleCommandFlags()
		  & ~(ConstC|VaryFlags)
		  | (OldC & (ConstC|VaryFlags));

    if(ThisC != OldC)
      Change = true;

    Char->SetCommandFlags(ThisC);
  }

  if(!Change)
    return false;

  Player->EditAP(-500);
  Player->EditExperience(CHARISMA, 50, 1 << 7);
  return true;
}

col16 game::GetAttributeColor(int I)
{
  int Delta = GetTick() - LastAttributeChangeTick[I];

  if(OldAttribute[I] == NewAttribute[I] || Delta >= 510)
    return WHITE;
  else if(OldAttribute[I] < NewAttribute[I])
    return MakeRGB16(255, 255, Delta >> 1);
  else
    return MakeRGB16(255, Delta >> 1, Delta >> 1);
}

void game::UpdateAttributeMemory()
{
  for(int c = 0; c < ATTRIBUTES; ++c)
  {
    int A = Player->GetAttribute(c);

    if(A != NewAttribute[c])
    {
      OldAttribute[c] = NewAttribute[c];
      NewAttribute[c] = A;
      LastAttributeChangeTick[c] = GetTick();
    }
  }
}

void game::InitAttributeMemory()
{
  for(int c = 0; c < ATTRIBUTES; ++c)
    OldAttribute[c] = NewAttribute[c] = Player->GetAttribute(c);
}

void game::TeleportHandler(v2 CursorPos)
{
  if((CursorPos - Player->GetPos()).GetLengthSquare() > Player->GetTeleportRangeSquare())
    CursorData = BLUE_CURSOR|CURSOR_TARGET;
  else
    CursorData = RED_CURSOR|CURSOR_TARGET;
}

double game::GetGameSituationDanger()
{
  double SituationDanger = 0;
  character* Player = GetPlayer();
  truth PlayerStuck = Player->IsStuck();
  v2 PlayerPos = Player->GetPos();
  character* TruePlayer = Player;

  if(PlayerStuck)
    (Player = Player->Duplicate(IGNORE_PROHIBITIONS))->ChangeTeam(0);

  for(int c1 = 0; c1 < GetTeams(); ++c1)
    if(GetTeam(c1)->GetRelation(GetTeam(PLAYER_TEAM)) == HOSTILE)
      for(std::list<character*>::const_iterator i1 = GetTeam(c1)->GetMember().begin();
	  i1 != GetTeam(c1)->GetMember().end(); ++i1)
      {
	character* Enemy = *i1;

	if(Enemy->IsEnabled() && Enemy->CanAttack()
	   && (Enemy->CanMove() || Enemy->GetPos().IsAdjacent(PlayerPos)))
	{
	  truth EnemyStuck = Enemy->IsStuck();
	  v2 EnemyPos = Enemy->GetPos();
	  truth Sees = TruePlayer->CanBeSeenBy(Enemy);
	  character* TrueEnemy = Enemy;

	  if(EnemyStuck)
	    Enemy = Enemy->Duplicate(IGNORE_PROHIBITIONS);

	  double PlayerTeamDanger = 1 / Enemy->GetSituationDanger(Player, EnemyPos, PlayerPos, Sees);

	  for(int c2 = 0; c2 < GetTeams(); ++c2)
	    if(GetTeam(c2)->GetRelation(GetTeam(c1)) == HOSTILE)
	      for(std::list<character*>::const_iterator i2 = GetTeam(c2)->GetMember().begin();
		  i2 != GetTeam(c2)->GetMember().end(); ++i2)
	      {
		character* Friend = *i2;

		if(Friend->IsEnabled() && !Friend->IsPlayer() && Friend->CanAttack()
		   && (Friend->CanMove() || Friend->GetPos().IsAdjacent(EnemyPos)))
		{
		  v2 FriendPos = Friend->GetPos();
		  truth Sees = TrueEnemy->CanBeSeenBy(Friend);

		  if(Friend->IsStuck())
		  {
		    Friend = Friend->Duplicate(IGNORE_PROHIBITIONS);
		    PlayerTeamDanger += Friend->GetSituationDanger(Enemy, FriendPos, EnemyPos, Sees) * .2;
		    delete Friend;
		  }
		  else
		    PlayerTeamDanger += Friend->GetSituationDanger(Enemy, FriendPos, EnemyPos, Sees);
		}
	      }

	  if(EnemyStuck)
	  {
	    PlayerTeamDanger *= 5;
	    delete Enemy;
	  }

	  SituationDanger += 1 / PlayerTeamDanger;
	}
      }

  Player->ModifySituationDanger(SituationDanger);

  if(PlayerStuck)
  {
    SituationDanger *= 2;
    delete Player;
  }

  return SituationDanger;
}

long game::GetTimeSpent()
{
  return time::TimeAdd(time::TimeDifference(time(0),LastLoad), TimePlayedBeforeLastLoad);
}

outputfile& operator<<(outputfile& SaveFile, const massacreid& MI)
{
  SaveFile << MI.Type << MI.Config << MI.Name;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, massacreid& MI)
{
  SaveFile >> MI.Type >> MI.Config >> MI.Name;
  return SaveFile;
}

truth game::PlayerIsRunning()
{
  return PlayerRunning && Player->CanMove();
}

void game::AddSpecialCursor(v2 Pos, int Data)
{
  SpecialCursorPos.push_back(Pos);
  SpecialCursorData.push_back(Data);
}

void game::RemoveSpecialCursors()
{
  SpecialCursorPos.clear();
  SpecialCursorData.clear();
}

void game::LearnAbout(god* Who)
{
  Who->SetIsKnown(true);

  /* slightly slow, but doesn't matter since
     this is run so rarely */

  if(PlayerKnowsAllGods() && !game::PlayerHasReceivedAllGodsKnownBonus)
  {
    GetPlayer()->ApplyAllGodsKnownBonus();
    game::PlayerHasReceivedAllGodsKnownBonus = true;
  }
}

truth game::PlayerKnowsAllGods()
{
  for(int c = 1; c <= GODS; ++c)
    if(!GetGod(c)->IsKnown())
      return false;

  return true;
}

void game::AdjustRelationsToAllGods(int Amount)
{
  for(int c = 1; c <= GODS; ++c)
    GetGod(c)->AdjustRelation(Amount);
}

void game::ShowDeathSmiley(bitmap* Buffer, truth)
{
  static blitdata B = { 0,
			{ 0, 0 },
			{ (RES.X >> 1) - 24, RES.Y * 4 / 7 - 24 },
			{ 48, 48 },
			{ 0 },
			TRANSPARENT_COLOR,
			0 };

  int Tick = globalwindowhandler::UpdateTick();  

  if(((Tick >> 1) & 31) == 1)
    B.Src.X = 48;
  else if(((Tick >> 1) & 31) == 2)
    B.Src.X = 96;
  else
    B.Src.X = 0;

  B.Bitmap = Buffer;
  igraph::GetSmileyGraphic()->NormalBlit(B);

  if(Buffer == DOUBLE_BUFFER)
    graphics::BlitDBToScreen();
}
