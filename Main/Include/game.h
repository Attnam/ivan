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

#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <vector>
#include <ctime>

#include "femath.h"
#include "festring.h"
#include "ivandef.h"

#ifndef LIGHT_BORDER
#define LIGHT_BORDER 80
#endif

#define PLAYER game::GetPlayer()

class area;
class level;
class dungeon;
class felist;
class team;
class character;
class gamescript;
class item;
class outputfile;
class inputfile;
class worldmap;
class god;
class square;
class wsquare;
class lsquare;
class bitmap;
class festring;
class rain;
class liquid;
class entity;
class olterrain;
struct explosion;

typedef std::map<festring, long> valuemap;
typedef truth (*stringkeyhandler)(int, festring&);
typedef v2 (*positionkeyhandler)(v2, int);
typedef void (*positionhandler)(v2);
typedef void (*bitmapeditor)(bitmap*, truth);

struct homedata
{
  v2 Pos;
  int Dungeon;
  int Level;
  int Room;
};

outputfile& operator<<(outputfile&, const homedata*);
inputfile& operator>>(inputfile&, homedata*&);

#ifdef VC
#pragma pack(1)
#endif

struct configid
{
  configid() { }
  configid(int Type, int Config) : Type(Type), Config(Config) { }
  bool operator<(const configid& CI) const { return memcmp(this, &CI, sizeof(configid)) < 0; }
  int Type NO_ALIGNMENT;
  int Config NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

outputfile& operator<<(outputfile&, const configid&);
inputfile& operator>>(inputfile&, configid&);

struct dangerid
{
  dangerid() { }
  dangerid(double NakedDanger, double EquippedDanger) : NakedDanger(NakedDanger), EquippedDanger(EquippedDanger) { }
  double NakedDanger;
  double EquippedDanger;
};

outputfile& operator<<(outputfile&, const dangerid&);
inputfile& operator>>(inputfile&, dangerid&);

struct ivantime
{
  int Day;
  int Hour;
  int Min;
};

struct massacreid
{
  massacreid() { }
  massacreid(int Type, int Config, cfestring& Name)
  : Type(Type), Config(Config), Name(Name) { }
  bool operator<(const massacreid&) const;
  int Type;
  int Config;
  festring Name;
};

inline bool massacreid::operator<(const massacreid& MI) const
{
  if(Type != MI.Type)
    return Type < MI.Type;

  if(Config != MI.Config)
    return Config < MI.Config;

  return Name < MI.Name;
}

outputfile& operator<<(outputfile&, const massacreid&);
inputfile& operator>>(inputfile&, massacreid&);

struct killreason
{
  killreason() { }
  killreason(cfestring& String, int Amount) : String(String), Amount(Amount) { }
  festring String;
  int Amount;
};

outputfile& operator<<(outputfile&, const killreason&);
inputfile& operator>>(inputfile&, killreason&);

struct killdata
{
  killdata(int Amount = 0, double DangerSum = 0) : Amount(Amount), DangerSum(DangerSum) { }
  int Amount;
  double DangerSum;
  std::vector<killreason> Reason;
};

outputfile& operator<<(outputfile&, const killdata&);
inputfile& operator>>(inputfile&, killdata&);

typedef std::map<configid, dangerid> dangermap;
typedef std::map<ulong, character*> characteridmap;
typedef std::map<ulong, item*> itemidmap;
typedef std::map<ulong, entity*> trapidmap;
typedef std::map<massacreid, killdata> massacremap;
typedef std::map<ulong, ulong> boneidmap;
typedef std::vector<item*> itemvector;
typedef std::vector<itemvector> itemvectorvector;
typedef std::vector<character*> charactervector;

class quitrequest { };
class areachangerequest { };

class game
{
 public:
  static truth Init(cfestring& = CONST_S(""));
  static void DeInit();
  static void Run();
  static int GetMoveCommandKey(int);
  static cv2 GetMoveVector(int I) { return MoveVector[I]; }
  static cv2 GetRelativeMoveVector(int I) { return RelativeMoveVector[I]; }
  static cv2 GetBasicMoveVector(int I) { return BasicMoveVector[I]; }
  static cv2 GetLargeMoveVector(int I) { return LargeMoveVector[I]; }
  static area* GetCurrentArea() { return CurrentArea; }
  static level* GetCurrentLevel() { return CurrentLevel; }
  static uchar*** GetLuxTable() { return LuxTable; }
  static character* GetPlayer() { return Player; }
  static void SetPlayer(character*);
  static v2 GetCamera() { return Camera; }
  static void UpdateCameraX();
  static void UpdateCameraY();
  static truth IsLoading() { return Loading; }
  static void SetIsLoading(truth What) { Loading = What; }
  static truth ForceJumpToPlayerBe() { return JumpToPlayerBe; }
  static void SetForceJumpToPlayerBe(truth What) { JumpToPlayerBe = What; }
  static level* GetLevel(int);
  static void InitLuxTable();
  static void DeInitLuxTable();
  static cchar* Insult();
  static truth TruthQuestion(cfestring&, int = 0, int = 0);
  static void DrawEverything();
  static truth Save(cfestring& = SaveName(""));
  static int Load(cfestring& = SaveName(""));
  static truth IsRunning() { return Running; }
  static void SetIsRunning(truth What) { Running = What; }
  static void UpdateCameraX(int);
  static void UpdateCameraY(int);
  static int GetCurrentLevelIndex() { return CurrentLevelIndex; }
  static int GetMoveCommandKeyBetweenPoints(v2, v2);
  static void DrawEverythingNoBlit(truth = false);
  static god* GetGod(int I) { return God[I]; }
  static cchar* GetAlignment(int I) { return Alignment[I]; }
  static void ApplyDivineTick();
  static void ApplyDivineAlignmentBonuses(god*, int, truth);
  static v2 GetDirectionVectorForKey(int);
  static festring SaveName(cfestring& = CONST_S(""));
  static void ShowLevelMessage();
  static double GetMinDifficulty();
  static void TriggerQuestForGoldenEagleShirt();
  static void CalculateGodNumber();
  static void IncreaseTick() { ++Tick; }
  static ulong GetTick() { return Tick; }
  static festring GetAutoSaveFileName() { return AutoSaveFileName; }
  static int DirectionQuestion(cfestring&, truth = true, truth = false);
  static void RemoveSaves(truth = true);
  static truth IsInWilderness() { return InWilderness; }
  static void SetIsInWilderness(truth What) { InWilderness = What; }
  static worldmap* GetWorldMap() { return WorldMap; }
  static void SetAreaInLoad(area* What) { AreaInLoad = What; }
  static void SetSquareInLoad(square* What) { SquareInLoad = What; }
  static area* GetAreaInLoad() { return AreaInLoad; }
  static square* GetSquareInLoad() { return SquareInLoad; }
  static int GetLevels();
  static dungeon* GetCurrentDungeon() { return Dungeon[CurrentDungeonIndex]; }
  static dungeon* GetDungeon(int I) { return Dungeon[I]; }
  static int GetCurrentDungeonIndex() { return CurrentDungeonIndex; }
  static void InitDungeons();
  static truth OnScreen(v2);
  static void DoEvilDeed(int);
  static void SaveWorldMap(cfestring& = SaveName(""), truth = true);
  static worldmap* LoadWorldMap(cfestring& = SaveName(""));
  static void UpdateCamera();
  static ulong CreateNewCharacterID(character*);
  static ulong CreateNewItemID(item*);
  static ulong CreateNewTrapID(entity*);
  static team* GetTeam(int I) { return Team[I]; }
  static int GetTeams() { return Teams; }
  static void Hostility(team*, team*);
  static void CreateTeams();
  static festring StringQuestion(cfestring&, col16, festring::sizetype, festring::sizetype, truth, stringkeyhandler = 0);
  static long NumberQuestion(cfestring&, int, truth = false);
  static ulong IncreaseLOSTick();
  static ulong GetLOSTick() { return LOSTick; }
  static void SendLOSUpdateRequest() { LOSUpdateRequested = true; }
  static void RemoveLOSUpdateRequest() { LOSUpdateRequested = false; }
  static character* GetPetrus() { return Petrus; }
  static void SetPetrus(character* What) { Petrus = What; }
  static truth HandleQuitMessage();
  static int GetDirectionForVector(v2);
  static cchar* GetVerbalPlayerAlignment();
  static void CreateGods();
  static int GetScreenXSize() { return 42; }
  static int GetScreenYSize() { return 26; }
  static v2 CalculateScreenCoordinates(v2);
  static void BusyAnimation();
  static void BusyAnimation(bitmap*, truth);
  static v2 PositionQuestion(cfestring&, v2, positionhandler = 0, positionkeyhandler = 0, truth = true);
  static void LookHandler(v2);
  static int AskForKeyPress(cfestring&);
  static truth AnimationController();
  static gamescript* GetGameScript() { return GameScript; }
  static void InitScript();
  static valuemap& GetGlobalValueMap() { return GlobalValueMap; }
  static void InitGlobalValueMap();
  static void TextScreen(cfestring&, v2 = ZERO_V2, col16 = 0xFFFF, truth = true, truth = true, bitmapeditor = 0);
  static void SetCursorPos(v2 What) { CursorPos = What; }
  static truth DoZoom() { return Zoom; }
  static void SetDoZoom(truth What) { Zoom = What; }
  static int KeyQuestion(cfestring&, int, int, ...);
  static v2 LookKeyHandler(v2, int);
  static v2 NameKeyHandler(v2, int);
  static void End(festring, truth = true, truth = true);
  static int CalculateRoughDirection(v2);
  static long ScrollBarQuestion(cfestring&, long, long, long, long, long, col16, col16, col16, void (*)(long) = 0);
  static truth IsGenerating() { return Generating; }
  static void SetIsGenerating(truth What) { Generating = What; }
  static void CalculateNextDanger();
  static int Menu(bitmap*, v2, cfestring&, cfestring&, col16, cfestring& = "", cfestring& = "");
  static void InitDangerMap();
  static const dangermap& GetDangerMap();
  static truth TryTravel(int, int, int, truth = false, truth = true);
  static truth LeaveArea(charactervector&, truth, truth);
  static void EnterArea(charactervector&, int, int);
  static int CompareLights(col24, col24);
  static int CompareLightToInt(col24, col24);
  static void CombineLights(col24&, col24);
  static col24 CombineConstLights(col24, col24);
  static truth IsDark(col24);
  static void SetStandardListAttributes(felist&);
  static double GetAveragePlayerArmStrengthExperience() { return AveragePlayerArmStrengthExperience; }
  static double GetAveragePlayerLegStrengthExperience() { return AveragePlayerLegStrengthExperience; }
  static double GetAveragePlayerDexterityExperience() { return AveragePlayerDexterityExperience; }
  static double GetAveragePlayerAgilityExperience() { return AveragePlayerAgilityExperience; }
  static void InitPlayerAttributeAverage();
  static void UpdatePlayerAttributeAverage();
  static void CallForAttention(v2, int);
  static character* SearchCharacter(ulong);
  static item* SearchItem(ulong);
  static entity* SearchTrap(ulong);
  static void AddCharacterID(character*, ulong);
  static void RemoveCharacterID(ulong);
  static void AddItemID(item*, ulong);
  static void RemoveItemID(ulong);
  static void UpdateItemID(item*, ulong);
  static void AddTrapID(entity*, ulong);
  static void RemoveTrapID(ulong);
  static void UpdateTrapID(entity*, ulong);
  static int GetStoryState() { return StoryState; }
  static void SetStoryState(int What) { StoryState = What; }
  static void SetIsInGetCommand(truth What) { InGetCommand = What; }
  static truth IsInGetCommand() { return InGetCommand; }
  static festring GetHomeDir();
  static festring GetSaveDir();
  static festring GetGameDir();
  static festring GetBoneDir();
  static truth PlayerWasHurtByExplosion() { return PlayerHurtByExplosion; }
  static void SetPlayerWasHurtByExplosion(truth What) { PlayerHurtByExplosion = What; }
  static void SetCurrentArea(area* What) { CurrentArea = What; }
  static void SetCurrentLevel(level* What) { CurrentLevel = What; }
  static void SetCurrentWSquareMap(wsquare*** What) { CurrentWSquareMap = What; }
  static void SetCurrentLSquareMap(lsquare*** What) { CurrentLSquareMap = What; }
  static festring& GetDefaultPolymorphTo() { return DefaultPolymorphTo; }
  static festring& GetDefaultSummonMonster() { return DefaultSummonMonster; }
  static festring& GetDefaultChangeMaterial() { return DefaultChangeMaterial; }
  static festring& GetDefaultDetectMaterial() { return DefaultDetectMaterial; }
  static void SignalDeath(ccharacter*, ccharacter*, festring);
  static void DisplayMassacreLists();
  static void DisplayMassacreList(const massacremap&, cchar*, long);
  static truth MassacreListsEmpty();
#ifdef WIZARD
  static void ActivateWizardMode() { WizardMode = true; }
  static truth WizardModeIsActive() { return WizardMode; }
  static void SeeWholeMap();
  static int GetSeeWholeMapCheatMode() { return SeeWholeMapCheatMode; }
  static truth GoThroughWallsCheatIsActive() { return GoThroughWallsCheat; }
  static void GoThroughWalls() { GoThroughWallsCheat = !GoThroughWallsCheat; }
#else
  static truth WizardModeIsActive() { return false; }
  static int GetSeeWholeMapCheatMode() { return 0; }
  static truth GoThroughWallsCheatIsActive() { return false; }
#endif
  static truth WizardModeIsReallyActive() { return WizardMode; }
  static void CreateBone();
  static int GetQuestMonstersFound() { return QuestMonstersFound; }
  static void SignalQuestMonsterFound() { ++QuestMonstersFound; }
  static void SetQuestMonstersFound(int What) { QuestMonstersFound = What; }
  static truth PrepareRandomBone(int);
  static boneidmap& GetBoneItemIDMap() { return BoneItemIDMap; }
  static boneidmap& GetBoneCharacterIDMap() { return BoneCharacterIDMap; }
  static double CalculateAverageDanger(const charactervector&, character*);
  static double CalculateAverageDangerOfAllNormalEnemies();
  static character* CreateGhost();
  static truth TooGreatDangerFound() { return TooGreatDangerFoundTruth; }
  static void SetTooGreatDangerFound(truth What) { TooGreatDangerFoundTruth = What; }
  static void CreateBusyAnimationCache();
  static long GetScore();
  static truth TweraifIsFree();
  static truth IsXMas();
  static int AddToItemDrawVector(const itemvector&);
  static void ClearItemDrawVector();
  static void ItemEntryDrawer(bitmap*, v2, uint);
  static int AddToCharacterDrawVector(character*);
  static void ClearCharacterDrawVector();
  static void CharacterEntryDrawer(bitmap*, v2, uint);
  static void GodEntryDrawer(bitmap*, v2, uint);
  static itemvectorvector& GetItemDrawVector() { return ItemDrawVector; }
  static charactervector& GetCharacterDrawVector() { return CharacterDrawVector; }
  static truth IsSumoWrestling() { return SumoWrestling; }
  static void SetIsSumoWrestling(truth What) { SumoWrestling = What; }
  static truth AllowHostilities() { return !SumoWrestling; }
  static truth AllBodyPartsVanish() { return SumoWrestling; }
  static truth TryToEnterSumoArena();
  static truth TryToExitSumoArena();
  static truth EndSumoWrestling(int);
  static character* GetSumo();
  static cfestring& GetPlayerName() { return PlayerName; }
  static rain* ConstructGlobalRain();
  static void SetGlobalRainLiquid(liquid* What) { GlobalRainLiquid = What; }
  static void SetGlobalRainSpeed(v2 What) { GlobalRainSpeed = What; }
  static truth PlayerIsSumoChampion() { return PlayerSumoChampion; }
  static v2 GetSunLightDirectionVector();
  static int CalculateMinimumEmitationRadius(col24);
  static ulong IncreaseSquarePartEmitationTicks();
  static cint GetLargeMoveDirection(int I) { return LargeMoveDirection[I]; }
  static void Wish(character*, cchar*, cchar*);
  static festring DefaultQuestion(festring, festring&, stringkeyhandler = 0);
  static void GetTime(ivantime&);
  static long GetTurn() { return Turn; }
  static void IncreaseTurn() { ++Turn; }
  static int GetTotalMinutes() { return Tick * 60 / 2000; }
  static truth PolymorphControlKeyHandler(int, festring&);
  static ulong* GetEquipmentMemory() { return EquipmentMemory; }
  static truth PlayerIsRunning();
  static void SetPlayerIsRunning(truth What) { PlayerRunning = What; }
  static truth FillPetVector(cchar*);
  static truth CommandQuestion();
  static void NameQuestion();
  static v2 CommandKeyHandler(v2, int);
  static void CommandScreen(cfestring&, ulong, ulong, ulong&, ulong&);
  static truth CommandAll();
  static double GetDangerFound() { return DangerFound; }
  static void SetDangerFound(double What) { DangerFound = What; }
  static col16 GetAttributeColor(int);
  static void UpdateAttributeMemory();
  static void InitAttributeMemory();
  static void TeleportHandler(v2);
  static void PetHandler(v2);
  static truth SelectPet(int);
  static double GetGameSituationDanger();
  static olterrain* GetMonsterPortal() { return MonsterPortal; }
  static void SetMonsterPortal(olterrain* What) { MonsterPortal = What; }
  static truth GetCausePanicFlag() { return CausePanicFlag; }
  static void SetCausePanicFlag(truth What) { CausePanicFlag = What; }
  static long GetTimeSpent();
  static void AddSpecialCursor(v2, int);
  static void RemoveSpecialCursors();
  static void LearnAbout(god*);
  static truth PlayerKnowsAllGods();
  static void AdjustRelationsToAllGods(int);
  static void ShowDeathSmiley(bitmap*, truth);
  static void SetEnterImage(cbitmap* What) { EnterImage = What; }
  static void SetEnterTextDisplacement(v2 What)
  {
    EnterTextDisplacement = What;
  }
 private:
  static void UpdateCameraCoordinate(int&, int, int, int);
  static cchar* const Alignment[];
  static god** God;
  static int CurrentLevelIndex;
  static int CurrentDungeonIndex;
  static cint MoveNormalCommandKey[];
  static cint MoveAbnormalCommandKey[];
  static cv2 MoveVector[];
  static cv2 RelativeMoveVector[];
  static cv2 BasicMoveVector[];
  static cv2 LargeMoveVector[];
  static uchar*** LuxTable;
  static truth Running;
  static character* Player;
  static v2 Camera;
  static ulong Tick;
  static festring AutoSaveFileName;
  static truth InWilderness;
  static worldmap* WorldMap;
  static area* AreaInLoad;
  static square* SquareInLoad;
  static dungeon** Dungeon;
  static ulong NextCharacterID;
  static ulong NextItemID;
  static ulong NextTrapID;
  static team** Team;
  static ulong LOSTick;
  static truth LOSUpdateRequested;
  static character* Petrus;
  static truth Loading;
  static truth JumpToPlayerBe;
  static gamescript* GameScript;
  static valuemap GlobalValueMap;
  static v2 CursorPos;
  static truth Zoom;
  static truth Generating;
  static dangermap DangerMap;
  static int NextDangerIDType;
  static int NextDangerIDConfigIndex;
  static double AveragePlayerArmStrengthExperience;
  static double AveragePlayerLegStrengthExperience;
  static double AveragePlayerDexterityExperience;
  static double AveragePlayerAgilityExperience;
  static characteridmap CharacterIDMap;
  static itemidmap ItemIDMap;
  static trapidmap TrapIDMap;
  static int Teams;
  static int Dungeons;
  static int StoryState;
  static truth InGetCommand;
  static truth PlayerHurtByExplosion;
  static area* CurrentArea;
  static level* CurrentLevel;
  static wsquare*** CurrentWSquareMap;
  static lsquare*** CurrentLSquareMap;
  static festring DefaultPolymorphTo;
  static festring DefaultSummonMonster;
  static festring DefaultWish;
  static festring DefaultChangeMaterial;
  static festring DefaultDetectMaterial;
  static massacremap PlayerMassacreMap;
  static massacremap PetMassacreMap;
  static massacremap MiscMassacreMap;
  static long PlayerMassacreAmount;
  static long PetMassacreAmount;
  static long MiscMassacreAmount;
  static truth WizardMode;
  static int SeeWholeMapCheatMode;
  static truth GoThroughWallsCheat;
  static int QuestMonstersFound;
  static boneidmap BoneItemIDMap;
  static boneidmap BoneCharacterIDMap;
  static truth TooGreatDangerFoundTruth;
  static bitmap* BusyAnimationCache[32];
  static itemvectorvector ItemDrawVector;
  static charactervector CharacterDrawVector;
  static truth SumoWrestling;
  static festring PlayerName;
  static liquid* GlobalRainLiquid;
  static v2 GlobalRainSpeed;
  static long GlobalRainTimeModifier;
  static truth PlayerSumoChampion;
  static ulong SquarePartEmitationTick;
  static cint LargeMoveDirection[];
  static long Turn;
  static ulong EquipmentMemory[MAX_EQUIPMENT_SLOTS];
  static truth PlayerRunning;
  static character* LastPetUnderCursor;
  static charactervector PetVector;
  static double DangerFound;
  static int OldAttribute[ATTRIBUTES];
  static int NewAttribute[ATTRIBUTES];
  static int LastAttributeChangeTick[ATTRIBUTES];
  static int NecroCounter;
  static int CursorData;
  static olterrain* MonsterPortal;
  static truth CausePanicFlag;
  static time_t TimePlayedBeforeLastLoad;
  static time_t LastLoad;
  static time_t GameBegan;
  static std::vector<v2> SpecialCursorPos;
  static std::vector<int> SpecialCursorData;
  static truth PlayerHasReceivedAllGodsKnownBonus;
  static cbitmap* EnterImage;
  static v2 EnterTextDisplacement;
};

inline void game::CombineLights(col24& L1, col24 L2)
{
  if(L2)
  {
    if(L1)
    {
      long Red1 = L1 & 0xFF0000, Red2 = L2 & 0xFF0000;
      long New = Red1 >= Red2 ? Red1 : Red2;
      long Green1 = L1 & 0xFF00, Green2 = L2 & 0xFF00;
      New |= Green1 >= Green2 ? Green1 : Green2;
      long Blue1 = L1 & 0xFF, Blue2 = L2 & 0xFF;
      L1 = Blue1 >= Blue2 ? New | Blue1 : New | Blue2;
    }
    else
      L1 = L2;
  }
}

inline col24 game::CombineConstLights(col24 L1, col24 L2)
{
  CombineLights(L1, L2);
  return L1;
}

inline truth game::IsDark(col24 Light)
{
  return !Light
    || ((Light & 0xFF0000) < (LIGHT_BORDER << 16)
	&& (Light & 0x00FF00) < (LIGHT_BORDER << 8)
	&& (Light & 0x0000FF) < LIGHT_BORDER);
}

inline int game::CompareLights(col24 L1, col24 L2)
{
  if(L1)
  {
    if((L1 & 0xFF0000) > (L2 & 0xFF0000)
       || (L1 & 0x00FF00) > (L2 & 0x00FF00)
       || (L1 & 0x0000FF) > (L2 & 0x0000FF))
      return 1;
    else if((L1 & 0xFF0000) == (L2 & 0xFF0000)
	    || (L1 & 0x00FF00) == (L2 & 0x00FF00)
	    || (L1 & 0x0000FF) == (L2 & 0x0000FF))
      return 0;
    else
      return -1;
  }
  else
    return -int(!!L2);
}

inline v2 game::CalculateScreenCoordinates(v2 Pos)
{
  return v2((Pos.X - Camera.X + 1) << 4, (Pos.Y - Camera.Y + 2) << 4);
}

#endif
