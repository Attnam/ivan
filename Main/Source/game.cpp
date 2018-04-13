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
#include <iostream>
#include <vector>

#if defined(UNIX) || defined(__DJGPP__)
#include <sys/stat.h>
#endif

#ifdef WIN32
#include <direct.h>
#endif

#include "allocate.h"
#include "area.h"
#include "audio.h"
#include "balance.h"
#include "bitmap.h"
#include "confdef.h"
#include "feio.h"
#include "felist.h"
#include "fetime.h"
#include "game.h"
#include "gear.h"
#include "god.h"
#include "graphics.h"
#include "hscore.h"
#include "human.h"
#include "iconf.h"
#include "materias.h"
#include "message.h"
#include "miscitem.h"
#include "nonhuman.h"
#include "pool.h"
#include "proto.h"
#include "rain.h"
#include "rawbit.h"
#include "room.h"
#include "save.h"
#include "stack.h"
#include "team.h"
#include "whandler.h"
#include "wsquare.h"

#define DBGMSG_BLITDATA
#include "dbgmsgproj.h"

#define SAVE_FILE_VERSION 131 // Increment this if changes make savefiles incompatible
#define BONE_FILE_VERSION 117 // Increment this if changes make bonefiles incompatible

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
int game::XinrochTombStoryState;
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
truth game::TouristHasSpider;
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
//int game::iListWidth = 652;

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

cint game::MoveNormalCommandKey[] =
{ KEY_HOME, KEY_UP, KEY_PAGE_UP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, '.' };
cint game::MoveAbnormalCommandKey[] = { '7', '8', '9', 'u', 'o', 'j', 'k', 'l', '.' };
cint game::MoveNetHackCommandKey[] = { 'y', 'k', 'u', 'h', 'l', 'b', 'j', 'n', '.' };

cv2 game::MoveVector[] =
{ v2(-1, -1), v2(0, -1), v2(1, -1), v2(-1, 0), v2(1, 0), v2(-1, 1), v2(0, 1), v2(1, 1), v2(0, 0) };
cv2 game::ClockwiseMoveVector[] =
{ v2(-1, -1), v2(0, -1), v2(1, -1), v2(1, 0), v2(1, 1), v2(0, 1), v2(-1, 1), v2(-1, 0), v2(0, 0) };
cv2 game::RelativeMoveVector[] =
{ v2(-1, -1), v2(1, 0), v2(1, 0), v2(-2, 1), v2(2, 0), v2(-2, 1), v2(1, 0), v2(1, 0), v2(-1, -1) };
cv2 game::BasicMoveVector[] = { v2(-1, 0), v2(1, 0), v2(0, -1), v2(0, 1) };
cv2 game::LargeMoveVector[] =
{ v2(-1, -1), v2(0, -1), v2(1, -1), v2(2, -1), v2(-1, 0), v2(2, 0), v2(-1, 1), v2(2, 1),
  v2(-1, 2), v2(0, 2), v2(1, 2), v2(2, 2), v2(0, 0), v2(1, 0), v2(0, 1), v2(1, 1) };
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

int iMaxXSize=0;
int iMaxYSize=0;
int iXSize=0;
int iYSize=0;

int iRegionAroundXBRZ = -1;
int iRegionSilhouette = -1;
int iRegionIndexDungeon = -1;
int iRegionListItem = -1;
int iRegionItemsUnder = -1;

blitdata game::bldAroundOnScreenTMP = DEFAULT_BLITDATA;
blitdata bldFullDungeonTMP = DEFAULT_BLITDATA;
blitdata bldSilhouetteTMP = DEFAULT_BLITDATA;
blitdata bldListItemTMP = DEFAULT_BLITDATA;
//blitdata bldShowItemsAtPlayerSquareTMP = DEFAULT_BLITDATA;

int iItemsUnderStretch = 2;

int iZoomFactor=6;
v2 ZoomPos = {0,0};
v2 silhouettePos = {0,0};

bool bPositionQuestionMode=false;

void game::SetIsRunning(truth What) { Running = What; }

int game::GetMaxScreenXSize() { //this generally should not be used when the campera position is part of the calculations
  if(iMaxXSize==0){
    // 800 provides 42. 800/16=50. 42=50-8. 8 magic number for whatever is drawn to the right of the dungeon.
    iMaxXSize = ivanconfig::GetStartingWindowWidth()/TILE_SIZE;
    iMaxXSize-=8;
  }
  return iMaxXSize;
}

int game::GetMaxScreenYSize() { //this generally should not be used when the campera position is part of the calculations
  if(iMaxYSize==0){
    // 600 provides 26. 600/16=37. 26=37-11. 11 magic number for whatever is drawn below of the dungeon.
    iMaxYSize = ivanconfig::GetStartingWindowHeight()/TILE_SIZE;
    iMaxYSize-=11;
  }
  return iMaxYSize;
}

int game::GetScreenXSize() { //actually dugeon visible width in tiles count
  if(iXSize==0){
    iXSize=GetMaxScreenXSize();
    iXSize/=ivanconfig::GetStartingDungeonGfxScale();DBG2("VisibleDungeonColumns",iXSize); //yes, may lose some columns, no way to fit as scaler is integer and not float
  }
  return iXSize;
}

int game::GetScreenYSize() {  //actually dugeon visible height in tiles count
  if(iYSize==0){
    iYSize=GetMaxScreenYSize();
    iYSize/=ivanconfig::GetStartingDungeonGfxScale();DBG2("VisibleDungeonLines",iYSize); //yes, may lose some lines, no way to fit as scaler is integer and not float
  }
  return iYSize;
}

void game::AddCharacterID(character* Char, ulong ID)
{
  CharacterIDMap.insert(std::make_pair(ID, Char));
}
void game::RemoveCharacterID(ulong ID)
{
  CharacterIDMap.erase(CharacterIDMap.find(ID));
}
void game::AddItemID(item* Item, ulong ID)
{
  ItemIDMap.insert(std::make_pair(ID, Item));
}
void game::RemoveItemID(ulong ID)
{
  if(ID) ItemIDMap.erase(ItemIDMap.find(ID));
}
void game::UpdateItemID(item* Item, ulong ID)
{
  ItemIDMap.find(ID)->second = Item;
}
void game::AddTrapID(entity* Trap, ulong ID)
{
  if(ID) TrapIDMap.insert(std::make_pair(ID, Trap));
}
void game::RemoveTrapID(ulong ID)
{
  if(ID) TrapIDMap.erase(TrapIDMap.find(ID));
}
void game::UpdateTrapID(entity* Trap, ulong ID)
{
  TrapIDMap.find(ID)->second = Trap;
}
const dangermap& game::GetDangerMap() { return DangerMap; }
void game::ClearItemDrawVector() { ItemDrawVector.clear(); }
void game::ClearCharacterDrawVector() { CharacterDrawVector.clear(); }

void game::InitScript()
{
  inputfile ScriptFile(GetDataDir() + "Script/dungeon.dat", &GlobalValueMap);
  GameScript = new gamescript;
  GameScript->ReadFrom(ScriptFile);
  {
    for (int f = 0; f <= 99; f++) //additional dungeon files
    {
      char bnum[32];
      sprintf(bnum, "Script/dungeon_%02d.dat", f);
      inputfile ifl(game::GetDataDir()+bnum, &game::GetGlobalValueMap(), false);
      if (ifl.IsOpen())
      {
        GameScript->ReadFrom(ifl);
        ifl.Close();
      }
    }
  }
  GameScript->RandomizeLevels();
}

void game::PrepareToClearNonVisibleSquaresAround(v2 v2SqrPos) {
  int i=ivanconfig::GetXBRZSquaresAroundPlayer();
  if(i==0)return;
  if(DoZoom())return; //TODO should be able to clear in zoom mode too? the result is still messy, but... is it cool to xBRZ non visible squares in look mode?  if so, no need to clear them...

  /***
   * this will check the squares around player for visibility/CanFeel
   *
   * the problem is the dungeon corners, ex.:
   *  0       1       2
   *  ####### ######O ######O
   *  ####### ##P###O ##P###O
   *  WWW#### WWW###O WWW###O
   *  ##WP### ##W###O     ##O
   *  ##W#### ##W###O      #O
   *  ####### OOOOOOO OOOOOOO
   *  ####### OOOOOOO OOOOOOO
   *
   *  Using xBRZ around player by 3 squares.
   *  P = player
   *  # = within user requested "around" distance.
   *  O = out of user requested "around" distance (are ignored/not considered)
   *  W = wall
   *
   *  0 - player is far from visible dungeon corners (no complexity)
   *
   *  1 - player is near top left corner:
   *    The drawn dungeon visible area will be copied just 2 to the left and 1 to the top away from player position,
   *    this means the cached bitmap will be smaller than at (*0).
   *
   *  2 - The player surroundings will be checked for visibility/CanFeel, if neither, these squares (that I deleted
   *    on the example above) will be cleared from pixel colors to the mask/transparent color.
   *    So, the vanilla (non xBRZ) non-visible squares' representation will be kept.
   */

//  lsquare* plsq = Player->GetLSquareUnder();
//  v2 v2PlayerPos = plsq->GetPos(); DBG3("PlayerPos",v2PlayerPos.X,v2PlayerPos.Y);
  v2 v2MaxSqrUpperLeft (v2SqrPos.X-i,v2SqrPos.Y-i); DBGV2(v2MaxSqrUpperLeft);
  v2 v2MaxSqrLowerRight(v2SqrPos.X+i,v2SqrPos.Y+i); DBGV2(v2MaxSqrLowerRight);

  level* plv = Player->GetLevel();
  v2 v2ChkSqrPos;
  lsquare* plsqChk;
  std::vector<v2> vv2ToBeCleared;
  v2 v2CamSqPos = GetCamera();
  v2 v2DungeonSqSize = v2(GetScreenXSize(),GetScreenYSize());
  int iSqLeftSkipX=0;
  int iSqTopSkipY=0;
  v2 v2Invalid(-1,-1),v2TopLeft(v2Invalid),v2BottomRight(v2Invalid);
  // tips: OnScreen(v2Square)
  for(int iY=v2MaxSqrUpperLeft.Y;iY<=v2MaxSqrLowerRight.Y;iY++){
    if(iY<0                || iY<  v2CamSqPos.Y                   ){iSqTopSkipY++;continue;}
    if(iY>=plv->GetYSize() || iY>=(v2CamSqPos.Y+v2DungeonSqSize.Y))break;

    iSqLeftSkipX=0; //must be reset here
    for(int iX=v2MaxSqrUpperLeft.X;iX<=v2MaxSqrLowerRight.X;iX++){
      if(iX<0                || iX<  v2CamSqPos.X                   ){iSqLeftSkipX++;continue;}
      if(iX>=plv->GetXSize() || iX>=(v2CamSqPos.X+v2DungeonSqSize.X))break;

      v2ChkSqrPos={iX,iY};
      if(v2TopLeft==v2Invalid)v2TopLeft=v2ChkSqrPos; //first is top left
      v2BottomRight=v2ChkSqrPos; //it will keep updating bottom right while it can
      plsqChk = plv->GetLSquare(v2ChkSqrPos);

      if(plsqChk->CanBeSeenByPlayer())continue;DBGLN;
      if(!IsInWilderness() && plsqChk->CanBeFeltByPlayer())continue;DBGLN;

      /********************************************************************************************
       * Now the final thing is to setup the relative pixel position on the small blitdata->bitmap
       * (that is a copy of the player surroundings at dungeon area),
       * that will have the squares cleared after it is cached
       * and before it is stretched with xBRZ,
       * so that the non visible squares will be drawn equally to all other far away
       * non vivible squares.
       */
      vv2ToBeCleared.push_back(v2( //TODO CalculateScreenCoordinates(v2Square)
        (v2ChkSqrPos.X - v2MaxSqrUpperLeft.X - iSqLeftSkipX)*TILE_SIZE,
        (v2ChkSqrPos.Y - v2MaxSqrUpperLeft.Y - iSqTopSkipY )*TILE_SIZE
      )); DBGV2(vv2ToBeCleared[vv2ToBeCleared.size()-1]);
    }
  }

  DBGV2(v2TopLeft);DBGV2(v2BottomRight);

  graphics::SetSRegionClearSquaresAt(iRegionAroundXBRZ,TILE_V2,vv2ToBeCleared);
}

void game::SRegionAroundDisable(){
  graphics::SetSRegionEnabled(iRegionAroundXBRZ,false);
}

bool bRegionAroundXBRZAllowed=true;
void game::SRegionAroundAllow(){
  bRegionAroundXBRZAllowed=true;
}
void game::SRegionAroundDeny(){
  bRegionAroundXBRZAllowed=false;
}

void game::UpdatePosAroundForXBRZ(v2 v2SqrPos){ //TODO join this logic with PrepareToClearNonVisibleSquaresAroundPlayer() as they deal with the same thing.
  if(iRegionAroundXBRZ==-1)return;

  bool bOk=true;

  if(bOk && !bRegionAroundXBRZAllowed)bOk=false;

  int iSAP=ivanconfig::GetXBRZSquaresAroundPlayer();
  if(bOk && iSAP==0)bOk=false;

  if(bOk && Player->IsDead())bOk=false; // this may actually never happen...

  if(bOk && bPositionQuestionMode){
    if(!IsInWilderness()){ // always allowed in wilderness (as there is only fully dark squares, not partial as memories)
      bOk=false;
      /**
       * TODO ??? adapt the squares cleaners to work with bPositionQuestionMode in dungeons and towns too ONLY IF
       * one day, it is working faster, as for now it only makes that feature a slow thing...
       */
    }
  }

  if(!bOk){
    SRegionAroundDisable();
    return;
  }

  /////////////////// ok ///////////////////////

  v2 v2ScreenPos = CalculateScreenCoordinates(v2SqrPos);//DBGV2(v2ScreenPos);

  graphics::SetSRegionEnabled(iRegionAroundXBRZ,true);

  bldAroundOnScreenTMP.Src = v2ScreenPos;

//  v2 v2SqrPosPlayer = Player->GetPos();
  v2 v2SqrPosCam = GetCamera();
  v2 v2DeltaSqr = {v2SqrPos.X-v2SqrPosCam.X, v2SqrPos.Y-v2SqrPosCam.Y};

  v2 deltaSquaresForUpperLeft=v2DeltaSqr;
  deltaSquaresForUpperLeft.X-=iSAP;
  deltaSquaresForUpperLeft.Y-=iSAP;

  v2 v2SrcInSquares(iSAP,iSAP);

  if(deltaSquaresForUpperLeft.X<0)v2SrcInSquares.X+=deltaSquaresForUpperLeft.X;
  if(deltaSquaresForUpperLeft.Y<0)v2SrcInSquares.Y+=deltaSquaresForUpperLeft.Y;

  bldAroundOnScreenTMP.Src.X-=TILE_SIZE*v2SrcInSquares.X;
  bldAroundOnScreenTMP.Src.Y-=TILE_SIZE*v2SrcInSquares.Y;

//  bldPlayerOnScreen.Dest = bldPlayerOnScreen.Src;

  v2 v2BorderInSquares(iSAP*2,iSAP*2);

  if(deltaSquaresForUpperLeft.X<0)v2BorderInSquares.X+=deltaSquaresForUpperLeft.X;
  if(deltaSquaresForUpperLeft.Y<0)v2BorderInSquares.Y+=deltaSquaresForUpperLeft.Y;

  v2 deltaForLowerRight=v2DeltaSqr;
  deltaForLowerRight.X=GetScreenXSize()-deltaForLowerRight.X-iSAP-1;
  deltaForLowerRight.Y=GetScreenYSize()-deltaForLowerRight.Y-iSAP-1;

  if(deltaForLowerRight.X<0)v2BorderInSquares.X+=deltaForLowerRight.X;
  if(deltaForLowerRight.Y<0)v2BorderInSquares.Y+=deltaForLowerRight.Y;

  bldAroundOnScreenTMP.Border.X=TILE_SIZE+(TILE_SIZE*v2BorderInSquares.X);
  bldAroundOnScreenTMP.Border.Y=TILE_SIZE+(TILE_SIZE*v2BorderInSquares.Y);

  // this grants positioninig on the upper left player's square corner

  // relative to full dungeon in source image vanilla position
  v2 deltaForFullDungeonSrc = {bldAroundOnScreenTMP.Src.X-bldFullDungeonTMP.Src.X, bldAroundOnScreenTMP.Src.Y-bldFullDungeonTMP.Src.Y};

  // relative to full dungeon over it's stretched image position
  bldAroundOnScreenTMP.Dest.X=bldFullDungeonTMP.Dest.X+(deltaForFullDungeonSrc.X*ivanconfig::GetStartingDungeonGfxScale());
  bldAroundOnScreenTMP.Dest.Y=bldFullDungeonTMP.Dest.Y+(deltaForFullDungeonSrc.Y*ivanconfig::GetStartingDungeonGfxScale());

  graphics::SetSRegionBlitdata(iRegionAroundXBRZ,bldAroundOnScreenTMP); DBGBLD(bldAroundOnScreenTMP);

  PrepareToClearNonVisibleSquaresAround(v2SqrPos);
}

void game::RegionListItemEnable(bool b){
  game::PrepareStretchRegionsLazy();
  if(iRegionListItem==-1)return;

  // src pos is set at felist
  graphics::SetSRegionBlitdata(iRegionListItem, bldListItemTMP);
  graphics::SetSRegionEnabled(iRegionListItem, b);
}

void game::RegionSilhouetteEnable(bool b){
  game::PrepareStretchRegionsLazy();
  if(iRegionSilhouette==-1)return;

  if( b && ivanconfig::GetSilhouetteScale()>1 ){
    bldSilhouetteTMP.Stretch = ivanconfig::GetSilhouetteScale();

    bldSilhouetteTMP.Dest = {
        silhouettePos.X -(bldSilhouetteTMP.Border.X*ivanconfig::GetSilhouetteScale()) -3,
        silhouettePos.Y};

    graphics::SetSRegionBlitdata(iRegionSilhouette, bldSilhouetteTMP);
    graphics::SetSRegionEnabled(iRegionSilhouette, true);
  }else{
    graphics::SetSRegionEnabled(iRegionSilhouette, false);
  }

}

void game::UpdateSRegionsXBRZ(){
  for(int i=0;i<graphics::GetTotSRegions();i++){
    if(i==iRegionIndexDungeon){
      graphics::SetSRegionDrawBeforeFelistPage(iRegionIndexDungeon,true,ivanconfig::IsXBRZScale());

      if(ivanconfig::GetXBRZSquaresAroundPlayer()>0){
        graphics::SetSRegionUseXBRZ(iRegionIndexDungeon,false);
        continue;
      }
    }

    graphics::SetSRegionUseXBRZ(i,ivanconfig::IsXBRZScale());
  }
}

void game::PrepareStretchRegionsLazy(){ // the ADD order IS important IF they overlap
  if(iRegionIndexDungeon==-1){
    if(ivanconfig::GetStartingDungeonGfxScale()>1){
      /**
       * dungeon visible area (Bitmap must be NULL)
       * workaround: only one line of the border will be stretched, hence src -1 and border +2
       */
      v2 topleft = area::getTopLeftCorner();
      bldFullDungeonTMP.Src = {topleft.X-1,topleft.Y-1}; //the top left corner of the dungeon drawn area INSIDE the dungeon are grey ouline
      bldFullDungeonTMP.Dest = {topleft.X - area::getOutlineThickness() -1, topleft.Y - area::getOutlineThickness() -1}; //the top left corner of the grey ouline to cover it TODO a new one should be drawn one day
      bldFullDungeonTMP.Border = {GetScreenXSize()*TILE_SIZE+2, game::GetScreenYSize()*TILE_SIZE+2};
      bldFullDungeonTMP.Stretch = ivanconfig::GetStartingDungeonGfxScale();
      iRegionIndexDungeon = graphics::AddStretchRegion(bldFullDungeonTMP,"FullDungeon");

      /***********************************
       * AROUND: player or look zoom pos *
       ***********************************/
      // (will be above dungeon) around player on screen
      bldAroundOnScreenTMP.Stretch = ivanconfig::GetStartingDungeonGfxScale();
      iRegionAroundXBRZ = graphics::AddStretchRegion(bldAroundOnScreenTMP,"AroundXBRZ");
    }
  }

  //TODO player stats etc, text log? at most x2?, set thru one user option bool for all (fast blit only?)

  if(iRegionSilhouette==-1){     // equiped items and humanoid silhouette region
    silhouettePos = humanoid::GetSilhouetteWhere();
    if(silhouettePos.X>0 && silhouettePos.Y>0){
      silhouettePos.X -= 15; silhouettePos.Y -= 23; //exact top left corner of all equipped items countour
      silhouettePos-=v2(1,1); //1 dot b4
      bldSilhouetteTMP.Src = {silhouettePos.X, silhouettePos.Y};
      bldSilhouetteTMP.Border = {94,110}; //SILHOUETTE_SIZE + equipped items around
      bldSilhouetteTMP.Border+=v2(2,2); //compensate for pos-1 and add +1 after border
      bldSilhouetteTMP.Stretch = 2; // minimum to allow setup
      iRegionSilhouette = graphics::AddStretchRegion(bldSilhouetteTMP,"Silhouette");
      graphics::SetSRegionDrawAfterFelist(iRegionSilhouette,true);
      graphics::SetSRegionDrawRectangleOutline(iRegionSilhouette,true);
    }
  }

  if(iRegionListItem==-1){
    bldListItemTMP.Dest = ZoomPos;
    bldListItemTMP.Border = TILE_V2;
    bldListItemTMP.Stretch = iZoomFactor;
    iRegionListItem = graphics::AddStretchRegion(bldListItemTMP,"ListItem");
    graphics::SetSRegionListItem(iRegionListItem);
    graphics::SetSRegionDrawRectangleOutline(iRegionListItem,true);
  }

  if(iRegionItemsUnder==-1){
    blitdata B = DEFAULT_BLITDATA;
    B.Stretch=iItemsUnderStretch;
    iRegionItemsUnder = graphics::AddStretchRegion(B,"ItemsUnderPosShowAboveHead");
  }

  UpdateSRegionsXBRZ();
}

truth game::Init(cfestring& Name)
{
//  graphics::SetStretchMode(ivanconfig::IsXBRZScale());
////  PrepareStretchRegions();

  if(Name.IsEmpty())
  {
    if(ivanconfig::GetDefaultName().IsEmpty())
    {
      PlayerName.Empty();

      if(iosystem::StringQuestion(PlayerName, CONST_S("What is your name? (1-20 letters)"),
                                  v2(30, 46), WHITE, 1, 20, true, true) == ABORTED
         || PlayerName.IsEmpty())
        return false;
    }
    else
      PlayerName = ivanconfig::GetDefaultName();
  }
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

#ifdef UNIX
  mkdir(GetHomeDir().CStr(), S_IRWXU|S_IRWXG);
  mkdir(GetSaveDir().CStr(), S_IRWXU|S_IRWXG);
  mkdir(GetBoneDir().CStr(), S_IRWXU|S_IRWXG);
#endif

  LOSTick = 2;
  DangerFound = 0;
  CausePanicFlag = false;

  bool bSuccess=false;
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
      bSuccess=true;
      break;
    }
   case NEW_GAME:
    {
      /* New game music */
      audio::SetPlaybackStatus(0);
      audio::ClearMIDIPlaylist();
      audio::LoadMIDIFile("newgame.mid", 0, 100);
      audio::SetPlaybackStatus(audio::PLAYING);

      iosystem::TextScreen(CONST_S("You couldn't possibly have guessed this day would differ from any other.\n"
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

      iosystem::TextScreen(CONST_S("\"I have a task for you, citizen\", said the viceroy picking his golden\n"
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
      iosystem::TextScreen(CONST_S("Generating game...\n\nThis may take some time, please wait."),
                           ZERO_V2, WHITE, false, true, &BusyAnimation);
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
      XinrochTombStoryState = 0;
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
      TouristHasSpider = false;
      protosystem::InitCharacterDataBaseFlags();
      memset(EquipmentMemory, 0, sizeof(EquipmentMemory));
      PlayerRunning = false;
      InitAttributeMemory();
      NecroCounter = 0;
      GameBegan = time(0);
      LastLoad = time(0);
      TimePlayedBeforeLastLoad = time::GetZeroTime();
      bool PlayerHasReceivedAllGodsKnownBonus = false;
      ADD_MESSAGE("You commence your journey to Attnam. Use direction keys to "
                  "move, '>' to enter an area and '?' to view other commands.");

      if(IsXMas())
      {
        item* Present = banana::Spawn();
        Player->GetStack()->AddItem(Present);
        ADD_MESSAGE("Atavus is happy today! He gives you %s.", Present->CHAR_NAME(INDEFINITE));
      }

      /* Set off the worldmap music */
      audio::SetPlaybackStatus(0);
      audio::ClearMIDIPlaylist();
      audio::LoadMIDIFile("world.mid", 0, 100);
      audio::SetPlaybackStatus(audio::PLAYING);

      bSuccess=true;
      break;
    }
   default:
    {
      bSuccess=false;
      break;
    }
  }

  if(bSuccess){ // for loaded or new game
    ZoomPos = {RES.X - 104, RES.Y - 112};
    PrepareStretchRegionsLazy();
  }

  return bSuccess;
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
  PrepareStretchRegionsLazy();

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

          if(abs(Pos.X - Player->GetPos().X) > 20
             || abs(Pos.Y - Player->GetPos().Y) > 20)
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
      graphics::SetDenyStretchedBlit();
      break;
    }
    catch(areachangerequest)
    {
    }

//    PrepareStretchRegions();
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

cchar* game::Insult()
{
  static cchar*const Insult[] =
  {
    "moron",
    "silly",
    "idiot",
    "airhead",
    "jerk",
    "dork",
    "Mr. Mole",
    "navastater",
    "potatoes-for-eyes",
    "lamer",
    "mommo-for-brains",
    "pinhead",
    "stupid-headed person",
    "software abuser",
    "loser",
    "peaballs",
    "person-with-problems",
    "unimportant user",
    "hugger-mugger"
  };

  return Insult[RAND_N(sizeof(Insult) / sizeof(Insult[0]))];
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
  return Pos.X >= 0 && Pos.Y >= 0 && Pos.X >= Camera.X && Pos.Y >= Camera.Y
      && Pos.X < GetCamera().X + GetScreenXSize() && Pos.Y < GetCamera().Y + GetScreenYSize();
}

void game::DrawEverythingNoBlit(truth AnimationDraw)
{
  bool bXBRZandFelist = ivanconfig::IsXBRZScale() && felist::isAnyFelistCurrentlyDrawn();

  if(LOSUpdateRequested && Player->IsEnabled())
  {
    if(!IsInWilderness())
      GetCurrentLevel()->UpdateLOS();
    else
      GetWorldMap()->UpdateLOS();
  }

  if(OnScreen(CursorPos))
  {
    if(!IsInWilderness() || CurrentWSquareMap[CursorPos.X][CursorPos.Y]->GetLastSeen() || GetSeeWholeMapCheatMode())
      CurrentArea->GetSquare(CursorPos)->SendStrongNewDrawRequest();
    else
      DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), TILE_V2, 0);
  }

  if(!bXBRZandFelist)
    for(size_t c = 0; c < SpecialCursorPos.size(); ++c)
      if(OnScreen(SpecialCursorPos[c]))
        CurrentArea->GetSquare(SpecialCursorPos[c])->SendStrongNewDrawRequest();

  globalwindowhandler::UpdateTick();
  if(!bXBRZandFelist)GetCurrentArea()->Draw(AnimationDraw);
  Player->DrawPanel(AnimationDraw);

  if(!AnimationDraw)
    msgsystem::Draw();

  if(!bXBRZandFelist && OnScreen(CursorPos))
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
    { //TODO could this zoom feature simply be another stretchregion?
      B.Src = B.Dest;
      B.Dest = ZoomPos;
      B.Stretch = iZoomFactor;
      graphics::DrawRectangleOutlineAround(DOUBLE_BUFFER, ZoomPos, TILE_V2*iZoomFactor, DARK_GRAY, true);
      graphics::Stretch(ivanconfig::IsXBRZScale(),DOUBLE_BUFFER,B,false);
    }

    igraph::DrawCursor(ScreenCoord, CursorData);
  }

  if(!bXBRZandFelist && Player->IsEnabled())
  {
    if(Player->IsSmall())
    {
      v2 Pos = Player->GetPos();

      if(OnScreen(Pos))
      {
        v2 ScreenCoord = CalculateScreenCoordinates(Pos);
        igraph::DrawCursor(ScreenCoord, Player->GetCursorData());
        if(!DoZoom())UpdatePosAroundForXBRZ(Pos);
      }
    }
    else
    {
      for(int c = 0; c < Player->GetSquaresUnder(); ++c)
      {
        v2 Pos = Player->GetPos(c);

        if(OnScreen(Pos))
        {
          v2 ScreenCoord = CalculateScreenCoordinates(Pos);
          igraph::DrawCursor(ScreenCoord, Player->GetCursorData()|CURSOR_BIG, c);
          if(!DoZoom())UpdatePosAroundForXBRZ(Pos);
        }
      }
    }
  }

  if(!bXBRZandFelist){
    for(size_t c = 0; c < SpecialCursorPos.size(); ++c){
      if(OnScreen(SpecialCursorPos[c]))
      {
        v2 ScreenCoord = CalculateScreenCoordinates(SpecialCursorPos[c]);
        igraph::DrawCursor(ScreenCoord, SpecialCursorData[c]);
        GetCurrentArea()->GetSquare(SpecialCursorPos[c])->SendStrongNewDrawRequest();
      }
    }
  }

  UpdateShowItemsAtLevelSquarePos(!bXBRZandFelist);

}

int game::IntemUnderCode(int iCycleValue){
  switch(iCycleValue){
    case 0:return 0; //disabled

    case 1:return 1; //above head

    // corners and Horizontal/Vertical
    case 2:return  10;
    case 3:return  11;

    case 4:return 110;
    case 5:return 111;

    case 6:return 210;
    case 7:return 211;

    case 8:return 310;
    case 9:return 311;

    default:ABORT("invalid IntemUnder cycle value %d",iCycleValue);
  }

  return -1; //dummy (never happens, just to cpp do not bother..)
}
int game::ItemUnderCorner(int val){
  return val/100;
}
int game::ItemUnderZoom(int val){
  return (val%100)/10;
}
bool game::ItemUnderHV(int val){
  return val%2==0; //odd is vertical
}

bitmap* bmpTgt = NULL;
bitmap* PrepareItemsUnder(bool bUseDB, stack* su, int iMax, v2 v2PosIni, int iDirX, int iDirY){
  int iTot = su->GetItems();
  if(iMax>-1)iTot = Min(iMax,iTot);
  if(iTot==0)return NULL;

  v2 v2Pos = v2PosIni;

  blitdata B = DEFAULT_BLITDATA;
  B.CustomData = ALLOW_ANIMATE;
  B.Stretch = 1; //TODO ignored?
  B.Border = { TILE_SIZE, TILE_SIZE };
  B.Luminance = ivanconfig::GetContrastLuminance();

  if(bUseDB){
    bmpTgt = DOUBLE_BUFFER;
  }else{
    v2 v2Size = v2(TILE_SIZE*iTot,TILE_SIZE);
    if(bmpTgt==NULL || bmpTgt->GetSize()!=v2Size){
      if(bmpTgt!=NULL)delete bmpTgt;
      bmpTgt = new bitmap(v2Size);
    }
  }

  B.Bitmap=bmpTgt;

  for(int i=0;i<iTot;i++){ // fully work on one square per time
    bmpTgt->Fill(v2Pos,B.Border,DARK_GRAY); //TODO could be? igraph::BlitBackGround(v2Pos,v2Size);
    graphics::DrawRectangleOutlineAround(bmpTgt, v2Pos+v2(1,1), B.Border-v2(2,2), LIGHT_GRAY, false);

    item* it = su->GetItem(i);
    B.Dest = v2Pos;
    it->Draw(B);

    v2Pos.X+=(TILE_SIZE*iDirX);
    v2Pos.Y+=(TILE_SIZE*iDirY);
  }

  return bmpTgt;
}

/**
 * The final screen coordinates are relative not to 0,0 but to the top left dungeon corner,
 * because the full dungeon stretching happens from that spot, therefore this does not work, ex.:
 *  CalculateScreenCoordinates(Player->GetPos())
 */
v2 game::CalculateStretchedBufferCoordinatesFromDungeonSquarePos(v2 v2SqrPos){
  int iStretchedTileSize = TILE_SIZE * ivanconfig::GetStartingDungeonGfxScale();
  v2 v2SqrRelativePosFromCam = v2SqrPos - GetCamera();
  v2 v2Min = area::getTopLeftCorner();;
  v2 v2StretchedBufferDest=v2Min;
  v2StretchedBufferDest+=(v2SqrRelativePosFromCam*iStretchedTileSize);
  return v2StretchedBufferDest;
}

void game::UpdateShowItemsAtLevelSquarePos(bool bAllowed){
  bool bOk=true;

  if(bOk && !bAllowed)bOk=false;

  if(bOk && !Player->IsEnabled())bOk=false;

  if(bOk && IsInWilderness())bOk=false;

  int iCode = IntemUnderCode(ivanconfig::GetShowItemsAtPlayerSquare());
  if(bOk && iCode==0)bOk=false;

  stack* su = Player->GetStackUnder(); //TODO should this work with look mode for visible squares too?
  if(bOk && su==NULL)bOk=false;
  if(bOk && su->GetItems()<2)bOk=false;

  if(!bOk){ // this is IMPORTANT as disabler
    graphics::SetSRegionEnabled(iRegionItemsUnder,false);
    return;
  }

  /////////////////////// ok ////////////////////////
  v2 v2AbsLevelSqrPos = Player->GetPos();

  int iNearEC=3; //near edges/corners to avoid hiding player/NPCs that can be in combat TODO use player view distance?
  if(
      v2AbsLevelSqrPos.X<=iNearEC
      ||
      v2AbsLevelSqrPos.Y<=iNearEC
      ||
      v2AbsLevelSqrPos.X >= (GetCurrentArea()->GetXSize() - iNearEC)
      ||
      v2AbsLevelSqrPos.Y >= (GetCurrentArea()->GetYSize() - iNearEC)
  ){
    iCode=1; //force above head
  }

  if(iCode==1){
    // TODO ? Some possible tips if look mode is used later: GetCurrentArea()->, Player->GetArea()->get, game::GetCurrentDungeon()->
    bitmap* bmp = PrepareItemsUnder(false, su, -1, v2(0,0), 1, 0);
    int iStretchedTileSize = TILE_SIZE * ivanconfig::GetStartingDungeonGfxScale();
    v2 v2PosFromCam = v2AbsLevelSqrPos - GetCamera();
    v2 v2Min = area::getTopLeftCorner();;
    v2 v2StretchedBufferDest=v2Min;
    v2StretchedBufferDest+=(v2PosFromCam*iStretchedTileSize);
    v2StretchedBufferDest.X+=iStretchedTileSize/2; //center of player's head
    v2StretchedBufferDest.X-=(bmp->GetSize().X*iItemsUnderStretch)/2;
    v2StretchedBufferDest.Y-= bmp->GetSize().Y*iItemsUnderStretch; // above player's head
    v2StretchedBufferDest.Y-=2; //just to look better
    if(v2StretchedBufferDest.X<v2Min.X)v2StretchedBufferDest.X=v2Min.X;
    if(v2StretchedBufferDest.Y<v2Min.Y)v2StretchedBufferDest.Y=v2Min.Y;
    graphics::SetSRegionSrcBitmapOverride(iRegionItemsUnder,bmp,v2StretchedBufferDest);
    graphics::SetSRegionEnabled(iRegionItemsUnder,true);
    return;
  }else{
    graphics::SetSRegionEnabled(iRegionItemsUnder,false);
  }

  int iCorner = ItemUnderCorner(iCode);
  bool bHorizontal = ItemUnderHV(iCode);
  int iStretch=ItemUnderZoom(iCode);

  switch(iStretch){
    case 1: { //this overwrites over dungeon squares pixels and is faster as it will go within the full dungeon stretch!
      int iDirX=0,iDirY=0;
      v2 v2SqrPos=Camera;
      v2 v2PosIni = area::getTopLeftCorner();
      int iScSX=game::GetScreenXSize()-1;
      int iScSY=game::GetScreenYSize()-1;
      switch(iCorner){
        case 0: iDirX=bHorizontal? 1:0; iDirY=bHorizontal?0: 1;
          break;
        case 1: iDirX=bHorizontal?-1:0; iDirY=bHorizontal?0: 1;
          v2SqrPos.X+=iScSX;
          v2PosIni.X+=iScSX*TILE_SIZE;
          break;
        case 2: iDirX=bHorizontal? 1:0; iDirY=bHorizontal?0:-1;
          v2SqrPos.Y+=iScSY;
          v2PosIni.Y+=iScSY*TILE_SIZE;
          break;
        case 3: iDirX=bHorizontal?-1:0; iDirY=bHorizontal?0:-1;
          v2SqrPos.X+=iScSX;
          v2SqrPos.Y+=iScSY;
          v2PosIni.X+=iScSX*TILE_SIZE;
          v2PosIni.Y+=iScSY*TILE_SIZE;
          break;
      }

      int iTot = su->GetItems();
      if(bHorizontal){
        if(iTot>game::GetScreenXSize())iTot=game::GetScreenXSize();
      }else{
        if(iTot>game::GetScreenYSize())iTot=game::GetScreenYSize();
      }

      PrepareItemsUnder(true,su,iTot,v2PosIni,iDirX,iDirY);

//        for(int i=0;i<iTot;i++){
//          //TODO could be? igraph::BlitBackGround(v2Pos, TILE_V2);
//          DOUBLE_BUFFER->Fill(v2Pos,v2Size,DARK_GRAY);
//          graphics::DrawRectangleOutlineAround(DOUBLE_BUFFER, v2Pos+v2(1,1), v2Size-v2(2,2), LIGHT_GRAY, false);
//
//          item* it = su->GetItem(i);
//          bldShowItemsAtPlayerSquareTMP.Dest = v2Pos;
//          it->Draw(bldShowItemsAtPlayerSquareTMP);
//
//          v2Pos.X+=(TILE_SIZE*iDirX);
//          v2Pos.Y+=(TILE_SIZE*iDirY);
//        }

      for(int i=0;i<iTot;i++){
        GetCurrentArea()->GetSquare(v2SqrPos)->SendStrongNewDrawRequest();
        v2SqrPos.X+=iDirX;
        v2SqrPos.Y+=iDirY;
      }

    }break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      //TODO xBRZ ?
      //TODO graphics::DrawRectangleOutlineAround(DOUBLE_BUFFER, area::getTopLeftCorner(), {TILE_SIZE*iTot,TILE_SIZE}, LIGHT_GRAY, false);
      break;
  }

}

truth game::Save(cfestring& SaveName)
{
  outputfile SaveFile(SaveName + ".sav");
  SaveFile << SAVE_FILE_VERSION;
  SaveFile << GameScript << CurrentDungeonIndex << CurrentLevelIndex << Camera;
  SaveFile << WizardMode << SeeWholeMapCheatMode << GoThroughWallsCheat;
  SaveFile << Tick << Turn << InWilderness << NextCharacterID << NextItemID << NextTrapID << NecroCounter;
  SaveFile << SumoWrestling << PlayerSumoChampion << TouristHasSpider << GlobalRainTimeModifier;
  long Seed = RAND();
  femath::SetSeed(Seed);
  SaveFile << Seed;
  SaveFile << AveragePlayerArmStrengthExperience;
  SaveFile << AveragePlayerLegStrengthExperience;
  SaveFile << AveragePlayerDexterityExperience;
  SaveFile << AveragePlayerAgilityExperience;
  SaveFile << Teams << Dungeons << StoryState << PlayerRunning << XinrochTombStoryState;
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
    if(!iosystem::Menu(0, v2(RES.X >> 1, RES.Y >> 1),
                       CONST_S("Sorry, this save is incompatible with the new version.\rStart new game?\r"),
                       CONST_S("Yes\rNo\r"), LIGHT_GRAY))
      return NEW_GAME;
    else
      return BACK;
  }

  SaveFile >> GameScript >> CurrentDungeonIndex >> CurrentLevelIndex >> Camera;
  SaveFile >> WizardMode >> SeeWholeMapCheatMode >> GoThroughWallsCheat;
  SaveFile >> Tick >> Turn >> InWilderness >> NextCharacterID >> NextItemID >> NextTrapID >> NecroCounter;
  SaveFile >> SumoWrestling >> PlayerSumoChampion >> TouristHasSpider >> GlobalRainTimeModifier;
  femath::SetSeed(ReadType<long>(SaveFile));
  SaveFile >> AveragePlayerArmStrengthExperience;
  SaveFile >> AveragePlayerLegStrengthExperience;
  SaveFile >> AveragePlayerDexterityExperience;
  SaveFile >> AveragePlayerAgilityExperience;
  SaveFile >> Teams >> Dungeons >> StoryState >> PlayerRunning >> XinrochTombStoryState;
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
  Dungeon = new dungeon *[Dungeons];
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

  const std::list<std::pair<int, teamscript>>& TeamScript = GetGameScript()->GetTeam();

  for(const std::pair<int, teamscript>& p : TeamScript)
  {
    for(uint c = 0; c < p.second.GetRelation().size(); ++c)
      GetTeam(p.second.GetRelation()[c].first)->SetRelation(GetTeam(p.first), p.second.GetRelation()[c].second);

    cint* KillEvilness = p.second.GetKillEvilness();

    if(KillEvilness)
      GetTeam(p.first)->SetKillEvilness(*KillEvilness);
  }
}

/* v2 Pos should be removed from xxxQuestion()s? */

/* If AllowExit is true the user can abort with the esc-key. The function returns ABORTED
   (when user aborts with esc) or NORMAL_EXIT. */

int game::StringQuestion(festring& Answer, cfestring& Topic, col16 Color,
                         festring::sizetype MinLetters, festring::sizetype MaxLetters,
                         truth AllowExit, stringkeyhandler KeyHandler)
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23)); // pos may be incorrect!
  SRegionAroundDeny();
  int Return = iosystem::StringQuestion(Answer, Topic, v2(16, 6), Color, MinLetters, MaxLetters, false, AllowExit, KeyHandler);
  SRegionAroundAllow();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  return Return;
}

long game::NumberQuestion(cfestring& Topic, col16 Color, truth ReturnZeroOnEsc)
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  SRegionAroundDeny();
  long Return = iosystem::NumberQuestion(Topic, v2(16, 6), Color, false, ReturnZeroOnEsc);
  SRegionAroundAllow();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  return Return;
}

long game::ScrollBarQuestion(cfestring& Topic, long BeginValue, long Step, long Min, long Max, long AbortValue,
                             col16 TopicColor, col16 Color1, col16 Color2, void (*Handler)(long))
{
  DrawEverythingNoBlit();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  SRegionAroundDeny();
  long Return = iosystem::ScrollBarQuestion(Topic, v2(16, 6), BeginValue, Step, Min, Max, AbortValue,
                                            TopicColor, Color1, Color2, GetMoveCommandKey(KEY_LEFT_INDEX),
                                            GetMoveCommandKey(KEY_RIGHT_INDEX), false, Handler);
  SRegionAroundAllow();
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
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
      switch(Menu(0, v2(RES.X >> 1, RES.Y >> 1),
                  CONST_S("Do you want to save your game before quitting?\r"),
                  CONST_S("Yes\rNo\rCancel\r"), LIGHT_GRAY))
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
      if(!Menu(0, v2(RES.X >> 1, RES.Y >> 1),
               CONST_S("You can't save at this point. Are you sure you still want to do this?\r"),
               CONST_S("Yes\rNo\r"), LIGHT_GRAY))
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
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  return Key;
}

/* Handler is called when the key has been identified as a movement key
 * KeyHandler is called when the key has NOT been identified as a movement key
 * Both can be deactivated by passing 0 as parameter */

v2 game::PositionQuestion(cfestring& Topic, v2 CursorPos, void (*Handler)(v2),
                          positionkeyhandler KeyHandler, truth Zoom)
{
  int Key = 0;
  SetDoZoom(Zoom);
  v2 Return;
  CursorData = RED_CURSOR;

  if(Handler)
    Handler(CursorPos);

  bPositionQuestionMode=true;
  for(;;)
  {
    square* Square = GetCurrentArea()->GetSquare(CursorPos);

    if(!Square->HasBeenSeen()
       && (!Square->GetCharacter() || !Square->GetCharacter()->CanBeSeenByPlayer())
       && !GetSeeWholeMapCheatMode())
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
    UpdatePosAroundForXBRZ(CursorPos);
    DrawEverything();
    Key = GET_KEY();
  }
  bPositionQuestionMode=false;

  // for text
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));

  // for zoom
  igraph::BlitBackGround(ZoomPos, TILE_V2*iZoomFactor);
  SetDoZoom(false);

  SetCursorPos(v2(-1, -1));

  if(ivanconfig::IsCenterOnPlayerAfterLook()){
    UpdateCamera();
  }

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
    if(!IsInWilderness()
       && !Square->CanBeSeenByPlayer()
       && GetCurrentLevel()->GetLSquare(CursorPos)->CanBeFeltByPlayer())
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
      {
        if(LSquare->EngravingsCanBeReadByPlayer() || GetSeeWholeMapCheatMode())
          LSquare->DisplayEngravedInfo(Msg);
        else
          Msg << " Something has been engraved here.";
      }
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
  inputfile SaveFile(GetDataDir() + "Script/define.dat", &GlobalValueMap);
  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
  {
    if(Word != "#" || SaveFile.ReadWord() != "define")
      ABORT("Illegal datafile define on line %ld!", SaveFile.TellLine());

    SaveFile.ReadWord(Word);
    GlobalValueMap.insert(std::make_pair(Word, SaveFile.ReadNumber()));
  }
}

void game::TextScreen(cfestring& Text, v2 Displacement, col16 Color,
                      truth GKey, truth Fade, bitmapeditor BitmapEditor)
{
  graphics::SetDenyStretchedBlit();
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
  igraph::BlitBackGround(v2(16, 6), v2(GetMaxScreenXSize() << 4, 23));
  return Return;
}

v2 game::LookKeyHandler(v2 CursorPos, int Key)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);

  switch(Key)
  {
   case 'i':
    if(!IsInWilderness())
    {
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
    }

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
  if(!Permanently)
    game::Save();

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

    /* Set off the main menu music */
    audio::SetPlaybackStatus(0);
    audio::ClearMIDIPlaylist();
    audio::LoadMIDIFile("mainmenu.mid", 0, 100);
    audio::SetPlaybackStatus(audio::PLAYING);

    throw quitrequest();
  }
}

void game::PlayVictoryMusic()
{
  audio::SetPlaybackStatus(0);
  audio::ClearMIDIPlaylist();
  audio::LoadMIDIFile("victory.mid", 0, 100);
  audio::SetPlaybackStatus(audio::PLAYING);
}

void game::PlayDefeatMusic()
{
  audio::SetPlaybackStatus(0);
  audio::ClearMIDIPlaylist();
  audio::LoadMIDIFile("defeat.mid", 0, 100);
  audio::SetPlaybackStatus(audio::PLAYING);
}

int game::CalculateRoughDirection(v2 Vector)
{
  if(!Vector.X && !Vector.Y)
    return YOURSELF;

  double Angle = femath::CalculateAngle(Vector);

  if(Angle < FPI / 8)
    return EAST;
  else if(Angle < 3 * FPI / 8)
    return SOUTHEAST;
  else if(Angle < 5 * FPI / 8)
    return SOUTH;
  else if(Angle < 7 * FPI / 8)
    return SOUTHWEST;
  else if(Angle < 9 * FPI / 8)
    return WEST;
  else if(Angle < 11 * FPI / 8)
    return NORTHWEST;
  else if(Angle < 13 * FPI / 8)
    return NORTH;
  else if(Angle < 15 * FPI / 8)
    return NORTHEAST;
  else
    return EAST;
}

int game::Menu(bitmap* BackGround, v2 Pos, cfestring& Topic, cfestring& sMS,
               col16 Color, cfestring& SmallText1, cfestring& SmallText2)
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

        character* Char = Proto->Spawn(Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE|NO_SEVERED_LIMBS);
        double NakedDanger = Char->GetRelativeDanger(Player, true);
        delete Char;
        Char = Proto->Spawn(Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE|NO_SEVERED_LIMBS);
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
    double DangerSum = Player->GetRelativeDanger(Char, true);

    for(character* p : Team->GetMember())
      if(p->IsEnabled() && !p->IsTemporary() && !RAND_N(10))
        DangerSum += p->GetRelativeDanger(Char, true) / 4;

    double CurrentDanger = 1 / DangerSum;
    double NakedDanger = DangerIterator->second.NakedDanger;
    delete Char;

    if(NakedDanger > CurrentDanger)
      DangerIterator->second.NakedDanger = (NakedDanger * 9 + CurrentDanger) / 10;

    Char = Proto->Spawn(DataBase->Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
    DangerSum = Player->GetRelativeDanger(Char, true);

    for(character* p : Team->GetMember())
      if(p->IsEnabled() && !p->IsTemporary() && !RAND_N(10))
        DangerSum += p->GetRelativeDanger(Char, true) / 4;

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
    GetCurrentDungeon()->PrepareMusic(Area);

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

    audio::SetPlaybackStatus(0);
    audio::ClearMIDIPlaylist();
    audio::LoadMIDIFile("world.mid", 0, 100);
    audio::SetPlaybackStatus(audio::PLAYING);

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

void prepareList(felist& rList, v2& v2TopLeft, int& iW){
  bool bAltItemPos = graphics::IsSRegionEnabled(iRegionListItem) && ivanconfig::IsAltListItemPos();

  rList.SetOriginalPos(v2TopLeft);

  int iX=v2TopLeft.X+10,iY=v2TopLeft.Y+10;
  if(ivanconfig::GetStartingDungeonGfxScale()>=2){
    //mainly to be drawn above the small dungeon (that gets scaled up)
    iX=v2TopLeft.X-3;
    iY=v2TopLeft.Y-3;
  }

  int iItemW=bldListItemTMP.Border.X*bldListItemTMP.Stretch;
  if(bAltItemPos){
    iX += area::getOutlineThickness()*2; //to leave some space to alt item outline
    iX += iItemW;
  }

  if(graphics::IsSRegionEnabled(iRegionSilhouette)){
    iW=ivanconfig::GetAltListItemWidth();
    //cant be so automatic... or user wants alt or default position... //if(bAltItemPos){iW+=iItemW;}
  }
  
  v2TopLeft={iX,iY};

  graphics::SetSpecialListItemAltPos(bAltItemPos);
}

int prepareListWidth(int iW){
  return iW;
}

void game::SetStandardListAttributes(felist& List)
{
  v2 v2TopLeft = area::getTopLeftCorner();
  int iW = iListWidth;

  prepareList(List, v2TopLeft, iW);

  List.SetPos(v2TopLeft);
  List.SetWidth(iW);
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
    AveragePlayerArmStrengthExperience = (49 * AveragePlayerArmStrengthExperience
                                          + PlayerArmStrengthExperience / Arms) / 50;
    AveragePlayerDexterityExperience = (49 * AveragePlayerDexterityExperience
                                        + PlayerDexterityExperience / Arms) / 50;
  }

  if(Legs)
  {
    AveragePlayerLegStrengthExperience = (49 * AveragePlayerLegStrengthExperience
                                          + PlayerLegStrengthExperience / Legs) / 50;
    AveragePlayerAgilityExperience = (49 * AveragePlayerAgilityExperience
                                      + PlayerAgilityExperience / Legs) / 50;
  }
}

void game::CallForAttention(v2 Pos, int RangeSquare)
{
  for(int c = 0; c < GetTeams(); ++c)
  {
    if(GetTeam(c)->HasEnemy())
      for(character* p : GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          long ThisDistance = HypotSquare(long(p->GetPos().X) - Pos.X, long(p->GetPos().Y) - Pos.Y);

          if(ThisDistance <= RangeSquare && !p->IsGoingSomeWhere())
            p->SetGoingTo(Pos);
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
  CharacterIDMap.insert(std::make_pair(ID, NewChar));
  return ID;
}

ulong game::CreateNewItemID(item* NewItem)
{
  ulong ID = NextItemID++;

  if(NewItem)
    ItemIDMap.insert(std::make_pair(ID, NewItem));

  return ID;
}

ulong game::CreateNewTrapID(entity* NewTrap)
{
  ulong ID = NextTrapID++;

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
#ifdef UNIX
  festring Dir;
  Dir << getenv("HOME") << "/.ivan/";
#ifdef DBGMSG
  dbgmsg::SetDebugLogPath(Dir.CStr());
#endif
  return Dir;
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetSaveDir()
{
  return GetHomeDir() + "Save/";
}

festring game::GetScrshotDir()
{
  return GetHomeDir() + "Scrshot/";
}

festring game::GetDataDir()
{
#ifdef UNIX
  return DATADIR "/ivan/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetBoneDir()
{
#ifdef UNIX
  return LOCAL_STATE_DIR "/Bones/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Bones/";
#endif
}

festring game::GetMusicDir()
{
#ifdef UNIX
  return GetDataDir() + "Music/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Music/";
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
  game::RegionListItemEnable(true);

  DisplayMassacreList(PlayerMassacreMap, "directly by you.", PlayerMassacreAmount);
  DisplayMassacreList(PetMassacreMap, "by your allies.", PetMassacreAmount);
  DisplayMassacreList(MiscMassacreMap, "by some other reason.", MiscMassacreAmount);

  game::RegionListItemEnable(false);
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

  for(const massacremap::value_type& p : MassacreMap)
  {
    character* Victim = protocontainer<character>::GetProto(p.first.Type)->Spawn(p.first.Config);
    Victim->SetAssignedName(p.first.Name);
    massacresetentry Entry;
    GraveYard.push_back(Victim);
    Entry.ImageKey = AddToCharacterDrawVector(Victim);

    if(p.second.Amount == 1)
    {
      Victim->AddName(Entry.Key, UNARTICLED);
      Victim->AddName(Entry.String, INDEFINITE);
    }
    else
    {
      Victim->AddName(Entry.Key, PLURAL);
      Entry.String << p.second.Amount << ' ' << Entry.Key;
    }

    if(First)
    {
      FirstPronoun = Victim->GetSex() == UNDEFINED ? "it" : Victim->GetSex() == MALE ? "he" : "she";
      First = false;
    }

    const std::vector<killreason>& Reason = p.second.Reason;
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

  for(const massacresetentry& MSE : MassacreSet)
    List.AddEntry(MSE.String, LIGHT_GRAY, 0, MSE.ImageKey);

  for(;;)
  {
    int Chosen = List.Draw();

    if(Chosen & FELIST_ERROR_BIT)
      break;

    felist SubList(CONST_S("Massacre details"));
    SetStandardListAttributes(SubList);
    SubList.SetPageLength(20);
    int Counter = 0;

    for(const massacresetentry& MSE : MassacreSet)
    {
      if(Counter == Chosen)
      {
        for(uint c = 0; c < MSE.Details.size(); ++c)
          SubList.AddEntry(MSE.Details[c], LIGHT_GRAY);

        break;
      }
      ++Counter;
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
      BoneFile << BONE_FILE_VERSION << PlayerName << CurrentLevel;
    }
  }
}

truth game::PrepareRandomBone(int LevelIndex)
{
  if(WizardModeIsActive()
     || GetCurrentDungeon()->IsGenerated(LevelIndex)
     || !*GetCurrentDungeon()->GetLevelScript(LevelIndex)->CanGenerateBone())
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
      else if(Player && Player->StateIsActivated(GAS_IMMUNITY))
        ADD_MESSAGE("You feel the cool breeze of death.");
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

bonesghost* game::CreateGhost()
{
  double AverageDanger = CalculateAverageDangerOfAllNormalEnemies();
  charactervector EnemyVector;
  protosystem::CreateEveryNormalEnemy(EnemyVector);
  bonesghost* Ghost = bonesghost::Spawn();
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
  switch(ivanconfig::GetDirectionKeyMap())
  {
  case DIR_NORM:
    return MoveNormalCommandKey[I];
  case DIR_ALT:
    return MoveAbnormalCommandKey[I];
  case DIR_HACK:
    return MoveNetHackCommandKey[I];
  default:
    ABORT("This is not Emacs!");
    return MoveNormalCommandKey[I];
  }
}

long game::GetScore()
{
  double Counter = 0;
  massacremap SumMap = PlayerMassacreMap;

  for(const massacremap::value_type& p : PetMassacreMap)
  {
    killdata& KillData = SumMap[p.first];
    KillData.Amount += p.second.Amount;
    KillData.DangerSum += p.second.DangerSum;
  }

  for(const massacremap::value_type& p : SumMap)
  {
    character* Char = protocontainer<character>::GetProto(p.first.Type)->Spawn(p.first.Config);
    int SumOfAttributes = Char->GetSumOfAttributes();
    Counter += sqrt(p.second.DangerSum / DEFAULT_GENERATION_DANGER) * SumOfAttributes * SumOfAttributes;
    delete Char;
  }

  return long(0.01 * Counter);
}

/* Only works if New Attnam is loaded */

truth game::TweraifIsFree()
{
  for(character* p : GetTeam(COLONIST_TEAM)->GetMember())
    if(p->IsEnabled())
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
                 { static_cast<int>(I << 4), 0 },
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

  ADD_MESSAGE("\"So you want to compete? Okay, I'll explain the rules. First, I'll make a mirror image out of us "
              "both. We'll enter the arena and fight till one is knocked out. Use of any equipment is not allowed. "
              "Note that we will not gain experience from fighting as a mirror image, but won't get really hurt, "
              "either. However, controlling the image is exhausting and you can get hungry very quickly.\"");

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
    Msg << "Here's a little something as a reward\", " << Sumo->GetPersonalPronoun()
        << " says and hands you a belt of levitation.\n\"";
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

int game::Wish(character* Wisher, cchar* MsgSingle, cchar* MsgPair, truth AllowExit)
{
  for(;;)
  {
    festring Temp;

    if(DefaultQuestion(Temp, CONST_S("What do you want to wish for?"), DefaultWish, AllowExit) == ABORTED)
      return ABORTED;

    item* TempItem = protosystem::CreateItem(Temp, Wisher->IsPlayer());

    if(TempItem)
    {
      Wisher->GetStack()->AddItem(TempItem);
      TempItem->SpecialGenerationHandler();

      if(TempItem->HandleInPairs())
        ADD_MESSAGE(MsgPair, TempItem->CHAR_NAME(PLURAL));
      else
        ADD_MESSAGE(MsgSingle, TempItem->CHAR_NAME(INDEFINITE));

      return NORMAL_EXIT;
    }
  }
}

int game::DefaultQuestion(festring& Answer, festring Topic, festring& Default,
                          truth AllowExit, stringkeyhandler KeyHandler)
{
  festring ShortDefault = Default;

  if(Default.GetSize() > 29)
  {
    ShortDefault.Resize(27);
    ShortDefault = ShortDefault << CONST_S("...");
  }

  if(!Default.IsEmpty())
    Topic << " [" << ShortDefault << ']';

  if(StringQuestion(Answer, Topic, WHITE, 0, 80, AllowExit, KeyHandler) == ABORTED)
    return ABORTED;

  if(Answer.IsEmpty())
    Answer = Default;
  else
    Default = Answer;

  return NORMAL_EXIT;
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

  for(character* p : Team->GetMember())
    if(p->IsEnabled() && !p->IsPlayer() && p->CanBeSeenByPlayer())
      PetVector.push_back(p);

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
    Pos = PositionQuestion(CONST_S("Whom do you wish to command? [direction keys/'+'/'-'/'a'll/space/esc]"),
                           Pos, &PetHandler, &CommandKeyHandler);

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
    PositionQuestion(CONST_S("Who do you want to name? [direction keys/'+'/'-'/'n'ame/esc]"),
                     PetVector[0]->GetPos(), &PetHandler, &NameKeyHandler);
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
    ulong ThisC = (NewFlags
                  & Char->GetPossibleCommandFlags()
                  & ~(ConstC|VaryFlags))
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
      for(character* Enemy : GetTeam(c1)->GetMember())
      {
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
              for(character* Friend : GetTeam(c2)->GetMember())
              {
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
  return time::TimeAdd(time::TimeDifference(time(0), LastLoad), TimePlayedBeforeLastLoad);
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

void game::SetRelationsToAllGods(int Amount)
{
  for(int c = 1; c <= GODS; ++c)
    GetGod(c)->SetRelation(Amount);
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
