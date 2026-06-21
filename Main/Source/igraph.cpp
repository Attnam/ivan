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

#include "igraph.h"
#include "felist.h"
#include "bitmap.h"
#include "graphics.h"
#include "iconf.h"
#include "rawbit.h"
#include "game.h"
#include "save.h"
#include "object.h"
#include "allocate.h"
#include "whandler.h"

rawbitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
bitmap* igraph::FlagBuffer;
cchar* igraph::RawGraphicFileName[] =
{
  "Graphics/GLTerra.png",
  "Graphics/OLTerra.png",
  "Graphics/Item.png",
  "Graphics/Char.png",
  "Graphics/Humanoid.png",
  "Graphics/Effect.png",
  "Graphics/Cursor.png"
};
cchar* igraph::GraphicFileName[] =
{
  "Graphics/WTerra.png",
  "Graphics/FOW.png",
  "Graphics/Symbol.png",
  "Graphics/Smiley.png"
};
tilemap igraph::TileMap;
uchar igraph::RollBuffer[256];
int** igraph::BodyBitmapValidityMap;
std::vector<bitmap*> igraph::vMenu;
bitmap* igraph::SilhouetteCache[HUMANOID_BODYPARTS][CONDITION_COLORS][SILHOUETTE_TYPES];
rawbitmap* igraph::ColorizeBuffer[2] = { new rawbitmap(TILE_V2), new rawbitmap(TILE_V2) };
bitmap* igraph::Cursor[CURSOR_TYPES];
bitmap* igraph::BigCursor[CURSOR_TYPES];
col16 igraph::CursorColor[CURSOR_TYPES] = { MakeRGB16(40, 40, 40),
                                            MakeRGB16(255, 0, 0),
                                            MakeRGB16(100, 100, 255),
                                            MakeRGB16(200, 200, 0) };
bitmap* igraph::BackGround;
int igraph::CurrentColorType = -1;

void igraph::Init()
{
  static truth AlreadyInstalled = false;

  if(!AlreadyInstalled)
  {
    AlreadyInstalled = true;
    graphics::Init();
    graphics::SetMode("IVAN " IVAN_VERSION,
#ifndef MAC_APP
                      festring(game::GetDataDir() + "Graphics/Icon.bmp").CStr(),
#else
                      NULL,
#endif
                      v2(ivanconfig::GetStartingWindowWidth(), ivanconfig::GetStartingWindowHeight()),
#ifndef __DJGPP__
                      ivanconfig::GetGraphicsScale(),
                      ivanconfig::GetScalingQuality(),
#endif
                      ivanconfig::GetFullScreenMode());
    DOUBLE_BUFFER->ClearToColor(0);
    graphics::BlitDBToScreen();
#ifndef __DJGPP__
    graphics::SetSwitchModeHandler(ivanconfig::SwitchModeHandler);
#endif
      if(ivanconfig::GetStartingFontGfx()==1){
    graphics::LoadDefaultFont(game::GetDataDir() + "Graphics/Font.png");
    }
      if(ivanconfig::GetStartingFontGfx()==2){
    graphics::LoadDefaultFont(game::GetDataDir() + "Graphics/Font2.png");
    }
      if(ivanconfig::GetStartingFontGfx()==3){
    graphics::LoadDefaultFont(game::GetDataDir() + "Graphics/Font3.png");
    }
    FONT->CreateFontCache(WHITE);
    FONT->CreateFontCache(LIGHT_GRAY);
    felist::SetDefaultEntryImageSize(TILE_V2);
    felist::CreateQuickDrawFontCaches(FONT, WHITE, 8);
    felist::CreateQuickDrawFontCaches(FONT, LIGHT_GRAY, 8);
    object::InitSparkleValidityArrays();
    int c;

    for(c = 0; c < RAW_TYPES; ++c)
      RawGraphic[c] = new rawbitmap(game::GetDataDir() + RawGraphicFileName[c]);

    for(c = 0; c < GRAPHIC_TYPES; ++c)
    {
      Graphic[c] = new bitmap(game::GetDataDir() + GraphicFileName[c]);
      Graphic[c]->ActivateFastFlag();
    }

    ColorizeBuffer[0]->CopyPaletteFrom(RawGraphic[0]);
    ColorizeBuffer[1]->CopyPaletteFrom(RawGraphic[0]);
    TileBuffer = new bitmap(TILE_V2);
    TileBuffer->ActivateFastFlag();
    TileBuffer->InitPriorityMap(0);
    FlagBuffer = new bitmap(TILE_V2);
    FlagBuffer->ActivateFastFlag();
    FlagBuffer->CreateAlphaMap(0);
    Alloc2D(BodyBitmapValidityMap, 8, 16);
    CreateBodyBitmapValidityMaps();
    CreateSilhouetteCaches();

    for(c = 0; c < CURSOR_TYPES; ++c)
    {
      packcol16 Color = CursorColor[c];
      Cursor[c] = new bitmap(v2(48, 16), TRANSPARENT_COLOR);
      Cursor[c]->CreateAlphaMap(255);
      GetCursorRawGraphic()->MaskedBlit(Cursor[c], ZERO_V2, ZERO_V2, v2(48, 16), &Color);
      BigCursor[c] = new bitmap(v2(96, 32), TRANSPARENT_COLOR);
      BigCursor[c]->CreateAlphaMap(255);
      GetCursorRawGraphic()->MaskedBlit(BigCursor[c], v2(0, 16), ZERO_V2, v2(96, 32), &Color);
    }
  }
}

void igraph::DeInit()
{
  int c;

  for(c = 0; c < RAW_TYPES; ++c)
    delete RawGraphic[c];

  for(c = 0; c < GRAPHIC_TYPES; ++c)
    delete Graphic[c];

  delete TileBuffer;
  delete FlagBuffer;
  delete [] BodyBitmapValidityMap;
  delete BackGround;
}

void igraph::DrawCursor(v2 Pos, int CursorData, int Index)
{
  /* Inefficient gum solution */

  blitdata BlitData = { DOUBLE_BUFFER,
                        { 0, 0 },
                        { Pos.X, Pos.Y },
                        { TILE_SIZE, TILE_SIZE },
                        { ivanconfig::GetContrastLuminance() },
                        TRANSPARENT_COLOR,
                        0 };

  bitmap* CursorBitmap;
  int SrcX = 0;

  if(ivanconfig::GetMode3() && !game::IsInWilderness())
    BlitData.CustomData |= DO_BLIT3;

  if(CursorData & CURSOR_BIG)
  {
    CursorBitmap = BigCursor[CursorData&~CURSOR_FLAGS];
    BlitData.Src.X = SrcX = (Index & 1) << 4;
    BlitData.Src.Y = (Index & 2) << 3;
  }
  else
    CursorBitmap = Cursor[CursorData&~CURSOR_FLAGS];

  if(!(CursorData & (CURSOR_FLASH|CURSOR_TARGET)))
  {
    Blit3(CursorBitmap, BlitData, MF_BLIT_LUMINANCE_MASKED | MF_WALL | MF_FLOOR | MF_CEIL);
    return;
  }

  if(!(CursorData & CURSOR_TARGET))
  {
    int Tick = GET_TICK() & 31;
    CursorBitmap->FillAlpha(95 + 10 * abs(Tick - 16));
    Blit3(CursorBitmap, BlitData, MF_BLIT_ALPHA_LUMINANCE | MF_WALL | MF_FLOOR | MF_CEIL);
    return;
  }

  int Tick = (GET_TICK() << 2) / 3;
  int Frame = (Tick >> 4) % 3;
  int Base = Frame << 4;
  BlitData.Src.X = SrcX + (CursorData & CURSOR_BIG ? Base << 1 : Base);
  CursorBitmap->FillAlpha(255 - (Tick & 0xF) * 16);
  Blit3(CursorBitmap, BlitData, MF_BLIT_ALPHA_LUMINANCE | MF_WALL | MF_FLOOR | MF_CEIL);
  Base = ((Frame + 1) % 3) << 4;
  BlitData.Src.X = SrcX + (CursorData & CURSOR_BIG ? Base << 1 : Base);
  CursorBitmap->FillAlpha((Tick & 0xF) * 16);
  Blit3(CursorBitmap, BlitData, MF_BLIT_ALPHA_LUMINANCE | MF_WALL | MF_FLOOR | MF_CEIL);
}

tilemap::iterator igraph::AddUser(const graphicid& GI)
{
  tilemap::iterator Iterator = TileMap.find(GI);

  if(Iterator != TileMap.end())
  {
    tile& Tile = Iterator->second;
    ++Tile.Users;
    return Iterator;
  }
  else
  {
    cint SpecialFlags = GI.SpecialFlags;
    cint BodyPartFlags = SpecialFlags & 0x78;
    cint RotateFlags = SpecialFlags & 0x7;
    cint Frame = GI.Frame;
    v2 SparklePos = v2(GI.SparklePosX, GI.SparklePosY);
    rawbitmap* RawBitmap = RawGraphic[GI.FileIndex];
    v2 RawPos = v2(GI.BitmapPosX, GI.BitmapPosY);

    if(BodyPartFlags && BodyPartFlags < ST_OTHER_BODYPART)
    {
      ColorizeBuffer[0]->Clear();
      EditBodyPartTile(RawBitmap, ColorizeBuffer[0], RawPos, BodyPartFlags);
      RawBitmap = ColorizeBuffer[0];
      RawPos.X = RawPos.Y = 0;

      if(RotateFlags)
      {
        ColorizeBuffer[1]->Clear();
        SparklePos = RotateTile(RawBitmap, ColorizeBuffer[1], RawPos, SparklePos, RotateFlags);
        RawBitmap = ColorizeBuffer[1];
      }
    }
    else if(RotateFlags)
    {
      ColorizeBuffer[0]->Clear();
      SparklePos = RotateTile(RawBitmap, ColorizeBuffer[0], RawPos, SparklePos, RotateFlags);
      RawBitmap = ColorizeBuffer[0];
      RawPos.X = RawPos.Y = 0;
    }

    bitmap* Bitmap = RawBitmap->Colorize(RawPos, TILE_V2, GI.Position, GI.Color, GI.BaseAlpha, GI.Alpha,
                                         GI.RustData, GI.BurnData, !(GI.SpecialFlags & ST_DISALLOW_R_COLORS));
    Bitmap->ActivateFastFlag();

    if(BodyPartFlags)
      Bitmap->InitPriorityMap(SpecialFlags & ST_CLOAK ? CLOAK_PRIORITY : AVERAGE_PRIORITY);

    if(GI.OutlineColor != TRANSPARENT_COLOR)
      Bitmap->Outline(GI.OutlineColor, GI.OutlineAlpha,
                      BodyPartFlags != ST_WIELDED ? ARMOR_OUTLINE_PRIORITY : AVERAGE_PRIORITY);

    if(SparklePos.X != SPARKLE_POS_X_ERROR)
      Bitmap->CreateSparkle(SparklePos + GI.Position, GI.SparkleFrame);

    if(GI.FlyAmount)
      Bitmap->CreateFlies(GI.Seed, Frame, GI.FlyAmount);

    cint WobbleData = GI.WobbleData;

    if(WobbleData & WOBBLE)
    {
      int Speed = (WobbleData & WOBBLE_SPEED_RANGE) >> WOBBLE_SPEED_SHIFT;
      int Freq = (WobbleData & WOBBLE_FREQ_RANGE) >> WOBBLE_FREQ_SHIFT;
      int WobbleMask = 7 >> Freq << (6 - Speed);

      if(!(Frame & WobbleMask))
        Bitmap->Wobble(Frame & ((1 << (6 - Speed)) - 1), Speed, WobbleData & WOBBLE_HORIZONTALLY_BIT);
    }

    if(SpecialFlags & ST_FLAMES)
    {
      ulong SeedNFlags = (SpecialFlags >> ST_FLAME_SHIFT & 3) | GI.Seed << 4;
      Bitmap->CreateFlames(RawBitmap, RawPos - GI.Position, SeedNFlags, Frame);
    }

    if(SpecialFlags & ST_LIGHTNING && !((Frame + 1) & 7))
      Bitmap->CreateLightning(GI.Seed + Frame, WHITE);

    return TileMap.insert(std::make_pair(GI, tile(Bitmap))).first;
  }
}

void igraph::EditBodyPartTile(rawbitmap* Source, rawbitmap* Dest, v2 Pos, int BodyPartFlags)
{
  if(BodyPartFlags == ST_RIGHT_ARM)
    Source->NormalBlit(Dest, Pos, ZERO_V2, v2(8, 16));
  else if(BodyPartFlags == ST_LEFT_ARM)
    Source->NormalBlit(Dest, v2(Pos.X + 8, Pos.Y), v2(8, 0), v2(8, 16));
  else if(BodyPartFlags == ST_GROIN)
  {
    Source->NormalBlit(Dest, v2(Pos.X, Pos.Y + 8), v2(0, 8), v2(16, 2));
    int i;
    v2 V;

    for(V.Y = 10, i = 0; V.Y < 13; ++V.Y)
      for(V.X = V.Y - 5; V.X < 20 - V.Y; ++V.X)
        Dest->PutPixel(V, Source->GetPixel(Pos + V));
  }
  else if(BodyPartFlags == ST_RIGHT_LEG)
  {
    /* Right leg from the character's, NOT the player's point of view */

    Source->NormalBlit(Dest, v2(Pos.X, Pos.Y + 10), v2(0, 10), v2(8, 6));
    Dest->PutPixel(v2(5, 10), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(6, 10), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(7, 10), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(6, 11), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(7, 11), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(7, 12), TRANSPARENT_PALETTE_INDEX);
  }
  else if(BodyPartFlags == ST_LEFT_LEG)
  {
    /* Left leg from the character's, NOT the player's point of view */

    Source->NormalBlit(Dest, v2(Pos.X + 8, Pos.Y + 10), v2(8, 10), v2(8, 6));
    Dest->PutPixel(v2(8, 10), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(9, 10), TRANSPARENT_PALETTE_INDEX);
    Dest->PutPixel(v2(8, 11), TRANSPARENT_PALETTE_INDEX);
  }
}

v2 igraph::RotateTile(rawbitmap* Source, rawbitmap* Dest, v2 Pos, v2 SparklePos, int RotateFlags)
{
  Source->NormalBlit(Dest, Pos, ZERO_V2, TILE_V2, RotateFlags);

  if(SparklePos.X != SPARKLE_POS_X_ERROR)
  {
    if(RotateFlags & ROTATE)
    {
      cint T = SparklePos.X;
      SparklePos.X = 15 - SparklePos.Y;
      SparklePos.Y = T;
    }

    if(RotateFlags & MIRROR)
      SparklePos.X = 15 - SparklePos.X;

    if(RotateFlags & FLIP)
      SparklePos.Y = 15 - SparklePos.Y;
  }

  return SparklePos;
}

void igraph::RemoveUser(tilemap::iterator Iterator)
{
  tile& Tile = Iterator->second;

  if(!--Tile.Users)
  {
    delete Tile.Bitmap;
    TileMap.erase(Iterator);
  }
}

outputfile& operator<<(outputfile& SaveFile, const graphicid& Value)
{
  SaveFile.Write(reinterpret_cast<cchar*>(&Value), sizeof(Value));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, graphicid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

graphicdata::~graphicdata()
{
  if(AnimationFrames)
  {
    for(int c = 0; c < AnimationFrames; ++c)
      igraph::RemoveUser(GraphicIterator[c]);

    delete [] Picture;
    delete [] GraphicIterator;
  }
}

void graphicdata::Save(outputfile& SaveFile) const
{
  SaveFile << AnimationFrames;

  for(int c = 0; c < AnimationFrames; ++c)
    SaveFile << GraphicIterator[c]->first;
}

void graphicdata::Load(inputfile& SaveFile)
{
  SaveFile >> AnimationFrames;

  if(AnimationFrames)
  {
    Picture = new bitmap*[AnimationFrames];
    GraphicIterator = new tilemap::iterator[AnimationFrames];
    graphicid GraphicID;

    for(int c = 0; c < AnimationFrames; ++c)
    {
      SaveFile >> GraphicID;
      Picture[c] = (GraphicIterator[c] = igraph::AddUser(GraphicID))->second.Bitmap;
    }
  }
}

outputfile& operator<<(outputfile& SaveFile, const graphicdata& Data)
{
  Data.Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, graphicdata& Data)
{
  Data.Load(SaveFile);
  return SaveFile;
}

void graphicdata::Retire()
{
  if(AnimationFrames)
  {
    for(int c = 0; c < AnimationFrames; ++c)
      igraph::RemoveUser(GraphicIterator[c]);

    AnimationFrames = 0;
    delete [] Picture;
    delete [] GraphicIterator;
  }
}

cint* igraph::GetBodyBitmapValidityMap(int SpecialFlags)
{
  return BodyBitmapValidityMap[(SpecialFlags & 0x38) >> 3];
}

void igraph::CreateBodyBitmapValidityMaps()
{
  memset(BodyBitmapValidityMap[0], 0xFF, 128 * sizeof(int));
  int* Map = BodyBitmapValidityMap[ST_RIGHT_ARM >> 3];
  int x, y;

  for(x = 8; x < 16; ++x)
    Map[x] = 0;

  Map = BodyBitmapValidityMap[ST_LEFT_ARM >> 3];

  for(x = 0; x < 8; ++x)
    Map[x] = 0;

  Map = BodyBitmapValidityMap[ST_GROIN >> 3];
  memset(Map, 0, 16 * sizeof(int));

  for(y = 10; y < 13; ++y)
    for(x = y - 5; x < 20 - y; ++x)
      Map[x] |= 1 << y;

  Map = BodyBitmapValidityMap[ST_RIGHT_LEG >> 3];

  for(x = 8; x < 16; ++x)
    Map[x] = 0;

  Map[5] &= ~(1 << 10);
  Map[6] &= ~(1 << 10);
  Map[7] &= ~(1 << 10);
  Map[6] &= ~(1 << 11);
  Map[7] &= ~(1 << 11);
  Map[7] &= ~(1 << 12);

  Map = BodyBitmapValidityMap[ST_LEFT_LEG >> 3];

  for(x = 0; x < 8; ++x)
    Map[x] = 0;

  Map[8] &= ~(1 << 10);
  Map[9] &= ~(1 << 10);
  Map[8] &= ~(1 << 11);
}

void igraph::LoadMenu()
{
  vMenu.push_back(new bitmap(game::GetDataDir() + "Graphics/Menu1.png"));
  vMenu.push_back(new bitmap(game::GetDataDir() + "Graphics/Menu2.png"));
  vMenu.push_back(new bitmap(game::GetDataDir() + "Graphics/Menu3.png"));
  vMenu.push_back(new bitmap(game::GetDataDir() + "Graphics/Menu4.png"));
  vMenu.push_back(new bitmap(game::GetDataDir() + "Graphics/Menu5.png"));
}

void igraph::UnLoadMenu()
{
  for(auto pbmp = vMenu.begin(); pbmp != vMenu.end(); ++pbmp){
    delete *pbmp;
  }
  vMenu.resize(0);
}

#ifdef IMPORT_EXPORT_GFX //INCOMPLETE WORK. for (one day) load each gfx from independent files.
bool isFileExist(const char *fileName) //TODO this should be more global
{
  DBG2("chkIfFileExist:",fileName);
  std::ifstream fl(fileName);
  bool b=fl.good(); //ifstream destructor will close the file
  DBG2("chkIfFileExist:",b);
  return b;
}
bool importGfx(festring fsFile,bitmap** ppbmpSC){
  DBG2("importing:",fsFile.CStr());
  (*ppbmpSC) = new bitmap(SILHOUETTE_SIZE, 0);
  inputfile flIn(fsFile);
  (*ppbmpSC)->Load(flIn);
  flIn.Close();
  DBG2("imported:",fsFile.CStr());

  return true; //TODO make it sure the file load woked, catching errors and so on...
}
bool bExportGfx=false; //if ran at a readonly location, true will fail.
void chkExportGfx(){
  //TODO inform this env var in some kind of developer documentation (preferable in an existing one)
  char* env=std::getenv("IVAN_EXPORTGFX"); DBG2("bExportGfx",env);

  if(env!=NULL){
    bExportGfx = strcmp(env,"true")==0; DBG2("bExportGfx",bExportGfx);
  }

}
festring prepareFileName(const char* strName){
  festring fs;
  return fs<<game::GetDataDir()<<"Graphics/HumanBodypartSilhouette/"<<strName<<".png";
}
#endif

void igraph::CreateSilhouetteCaches()
{
  int BodyPartSilhouetteMColorIndex[HUMANOID_BODYPARTS] = { 3, 0, 1, 2, 1, 2, 3 };
  col24 ConditionColor[CONDITION_COLORS] = { static_cast<col24>(MakeRGB16(48, 48, 48)),
                                             static_cast<col24>(MakeRGB16(120, 0, 0)),
                                             static_cast<col24>(MakeRGB16(180, 0, 0)),
                                             static_cast<col24>(MakeRGB16(180, 120, 120)),
                                             static_cast<col24>(MakeRGB16(180, 180, 180)) };
  v2 V(8, 64);

  for(int c1 = 0; c1 < HUMANOID_BODYPARTS; ++c1)
  {
    if(c1 == 4)
      V.X = 72;

    for(int c2 = 0; c2 < CONDITION_COLORS; ++c2)
    {
      packcol16 Color[4] = { 0, 0, 0, 0 };
      Color[BodyPartSilhouetteMColorIndex[c1]] = ConditionColor[c2];

      for(int c3 = 0; c3 < SILHOUETTE_TYPES; ++c3)
      {
        SilhouetteCache[c1][c2][c3] = new bitmap(SILHOUETTE_SIZE, 0);
        RawGraphic[GR_CHARACTER]->MaskedBlit(SilhouetteCache[c1][c2][c3],
                                             V, ZERO_V2,
                                             SILHOUETTE_SIZE, Color);
      }

      SilhouetteCache[c1][c2][SILHOUETTE_INTER_LACED]->InterLace();
    }
  }
}

//void igraph::CreateBackGround(int ColorType)
//{
//  if(CurrentColorType == ColorType)
//    return;
//
//  CurrentColorType = ColorType;
//  delete BackGround;
//  BackGround = new bitmap(RES);
//  int Side = 1025;
//  int** Map;
//  Alloc2D(Map, Side, Side);
//  femath::GenerateFractalMap(Map, Side, Side - 1, 800);
//
//  for(int x = 0; x < RES.X; ++x)
//    for(int y = 0; y < RES.Y; ++y)
//    {
//      int E = Limit<int>(abs(Map[1024 - x][1024 - (RES.Y - y)]) / 30, 0, 100);
//      BackGround->PutPixel(x, y, GetBackGroundColor(E));
//    }
//
//  delete [] Map;
//}
void igraph::CreateBackGround(int ColorType)
{
  if(CurrentColorType == ColorType)
    return;

  CurrentColorType = ColorType;
  delete BackGround;
  BackGround = new bitmap(RES);
  int base=1024; //TODO explain this: fractals require multiple of 1024 to work/workBetter?
  while(ivanconfig::GetStartingWindowWidth()>base)base+=1024;
  int Side = base+1;
  int** Map;
  Alloc2D(Map, Side, Side); //TODO confirm and explain this: it seems fractals work better on a squared img right?
  femath::GenerateFractalMap(Map, Side, Side - 1, ivanconfig::GetStartingWindowWidth());

  for(int x = 0; x < RES.X; ++x)
    for(int y = 0; y < RES.Y; ++y)
    {
      int E = Limit<int>(abs(Map[base - x][base - (RES.Y - y)]) / 30, 0, 100);
      BackGround->PutPixel(x, y, GetBackGroundColor(E));
    }

  delete [] Map;
}

col16 igraph::GetBackGroundColor(int Element)
{
  switch(CurrentColorType)
  {
   case GRAY_FRACTAL: return MakeRGB16(Element, Element, Element);
   case RED_FRACTAL: return MakeRGB16(Element + (Element >> 1), Element / 3, Element / 3);
   case GREEN_FRACTAL: return MakeRGB16(Element / 3, Element + (Element >> 2), Element / 3);
   case BLUE_FRACTAL: return MakeRGB16(Element / 3, Element / 3, Element + (Element >> 1));
   case YELLOW_FRACTAL: return MakeRGB16(Element + (Element >> 1), Element + (Element >> 1), Element / 3);
  }

  return 0;
}

void igraph::BlitBackGround(v2 Pos, v2 Border)
{
  BlitBackGround(DOUBLE_BUFFER, Pos, Border);
}

void igraph::BlitBackGround(bitmap* bmpAt, v2 Pos, v2 Border)
{
  blitdata B = { bmpAt,
                  { Pos.X, Pos.Y },
                  { Pos.X, Pos.Y },
                  { Border.X, Border.Y },
                  { 0 },
                  0,
                  0 };

  BackGround->NormalBlit(B);
}

bitmap* igraph::GenerateScarBitmap(int BodyPart, int Severity, int Color)
{
  bitmap* CacheBitmap = SilhouetteCache[BodyPart][0][SILHOUETTE_NORMAL];
  bitmap* Scar = new bitmap(SILHOUETTE_SIZE, 0);

  v2 StartPos;
  while(true)
  {
    StartPos = v2(RAND_N(SILHOUETTE_SIZE.X), RAND_N(SILHOUETTE_SIZE.Y));
    if(CacheBitmap->GetPixel(StartPos) != 0)
      break;
  }

  v2 EndPos;
  while(true)
  {
    double Angle = 2 * FPI * RAND_256 / 256;
    EndPos.X = int(StartPos.X + cos(Angle) * (Severity + 1));
    EndPos.Y = int(StartPos.Y + sin(Angle) * (Severity + 1));
    if(CacheBitmap->IsValidPos(EndPos) && CacheBitmap->GetPixel(EndPos) != 0)
      break;
  }
  Scar->DrawLine(StartPos, EndPos, Color);
  return Scar;
}

void igraph::AddOutlinesIfNeeded() {
  static bool CurrentlyOutlined = false;
  if(ivanconfig::IsOutlinedGfx() == CurrentlyOutlined) return;
  CurrentlyOutlined = ivanconfig::IsOutlinedGfx();

  for(auto& g: {GR_CHARACTER, GR_ITEM, GR_HUMANOID})
  {
    auto& img = RawGraphic[g];
    delete img;
    img = new rawbitmap(game::GetDataDir() + RawGraphicFileName[g]);
    if(!CurrentlyOutlined) continue;

    for(int y=0; y<img->GetSize().Y-15; y+=16)
    for(int x=0; x<img->GetSize().X-15; x+=16)
    {

      int S = 16;

      auto large_at = [&] (int x1, int y1)
      {
        if(x == x1 && y == y1) S = 32;
        else if((x == x1 || x == x1+16) && (y == y1 || y == y1+16)) S = 0;
      };

      int large_size = (x % 32 == 0 && y % 32 == 0) ? 32 : 0;

      if(g == GR_ITEM)
      {
        // do not add outlines to lights
        if(x == 0 && y == 192) S = 0;
        if(x == 0 && y == 256) S = 0;
        if(x == 0 && y == 112) S = 0; // protect the Holy Banana
        // large corpse
        large_at(48, 0);
      }

      if(g == GR_HUMANOID)
      {
        // do not add outlines to lights
        if(x == 160 && y == 160) S = 0;
      }


      if(g == GR_CHARACTER)
      {
        // paperdolls
        if(y >= 64 && x < 128) S = 0;
        // large creatures here
        if(y >= 64 && x >= 128 && x < 320) S = large_size;
        // except here
        if(y >= 96 && y < 128 && x >= 256 && x < 320) S = 16;
        // more large creatures here
        if(y >= 32 && y < 64 && x >= 256 && x < 320) S = large_size;
        if(y >= 32 && y < 64 && x >= 0 && x < 64) S = large_size;
        // great picture here
        if(y >= 32 && x >= 320) S = 0;
        large_at(560, 16);
      }

      auto xy = v2(x, y);
      for(int x=0; x<S; x++)
      for(int y=0; y<S; y++)
       {
        auto m = img->GetPixel(v2(xy.X+x, xy.Y+y));
        if(m != TRANSPARENT_PALETTE_INDEX && m != 0) {
          auto f = [&] (bool b, v2 adj)
          {
            if(b) img->PutPixel(v2(xy.X+x, xy.Y+y), 0);
            else if(img->GetPixel(adj) == TRANSPARENT_PALETTE_INDEX) img->PutPixel(adj, 0);
          };
          f(x == 0, v2(xy.X+x-1, xy.Y+y));
          f(y == 0, v2(xy.X+x, xy.Y+y-1));
          f(x == S-1, v2(xy.X+x+1, xy.Y+y));
          f(y == S-1, v2(xy.X+x, xy.Y+y+1));
        }
      }
    }
  }

  // uncomment if you want to debug this
  // RawGraphic[GR_ITEM]->Save("test-item.png");
  // RawGraphic[GR_HUMANOID]->Save("test-humanoid.png");
  // RawGraphic[GR_CHARACTER]->Save("test-character.png");
}

int NewRedLuminance, NewGreenLuminance, NewBlueLuminance;

void setLum(blitdata& B, int mode) {
  if(!(mode & MF_BLIT_LUMINANCE_MASKED))
    NewRedLuminance = NewGreenLuminance = NewBlueLuminance = 0;
  else {
    NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
    NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
    NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;
  }
}

struct fpoint4 {
  int x, y, z, ax;
  fpoint4() {}
  fpoint4(int X, int Y, int Z, int AX=0) {x=X; y=Y; z=Z; ax=AX;}
};

fpoint4 operator + (fpoint4& a, fpoint4& b) {
  return fpoint4(a.x+b.x, a.y+b.y, a.z+b.z, a.ax+b.ax);
}

struct fpoint {
  bool ok;
  float x;
  float y;
  fpoint() {ok = false;}
  fpoint(fpoint4 i);
};

static inline float cross(fpoint a, fpoint b, int x, int y) {
  return (x-a.x) * (b.y-a.y) - (b.x-a.x) * (y-a.y);
}

#include "char.h"

int currEye, cc, currAY;

fpoint::fpoint(fpoint4 i) {

  int sx = game::GetScreenXSize();
  int sy = game::GetScreenYSize();

  if(igraph::defy == -1) {
    y = (26 - i.x - i.y) / 2.0;
    if(y <= 0) {ok = false; return;}
    x = i.x - i.y;
    x = igraph::defx * (16+x);
    if(cc == 1) x -= 2;
    if(cc == 2) x += 2;
    x = sx*8 + sy*2*x/y;
    y = sy*8 + sy*32/y;
    x += sx * currEye / 10.0;
    ok = true;
    return;
  }

  float tx = i.x - PLAYER->GetPos().X * 64 - 32;
  float ty = i.y - PLAYER->GetPos().Y * 64 - 32;

  if(ivanconfig::Mode3FPP()) {

    int fac = game::GetFacing();

    float angle = fac * M_PI / 4;

    float S = -sin(angle);
    float C = cos(angle);

    x = tx * C - ty * S + i.ax;
    y = tx * S + ty * C + currAY;

    if(cc == 1) x += 2;
    if(cc == 2) x -= 2;

    ok = y < -1e-6;
    if(!ok) return;

    float eyelevel = (PLAYER->GetSize() * 64) / 300.0;
    if(eyelevel > 63) eyelevel = 63;

    if(PLAYER -> IsFlying())
      eyelevel = 48 + sin(GET_TICK()/16.0);

    eyelevel = i.z - eyelevel;

    if(PLAYER -> TemporaryStateIsActivated(CONFUSED)) {
      float angle = GET_TICK() * M_PI / 64;
      tx = x; ty = eyelevel;
      S = -sin(angle);
      C = cos(angle);
      x = C * tx + S * ty;
      eyelevel = -S * tx + C * ty;
      }

    x = sx * 8 - sy * 8 * x / y - 0.5;
    x += sx * currEye / 10.0;
    y = sy * 8 + sy * 8 * eyelevel / y - 0.5;
  }
  else {
    x = tx - ty; y = tx + ty;
    if(!ivanconfig::GetShowAllInIso()) x *= sqrt(3.0) / 2;
    x = sx * 8 + x/2.0 + i.ax/2.0 - 0.5;
    y = sy * 8 + y/4.0-i.z /2.0 - 0.5;
    ok = true;
  }
}

static inline col16 apply(col16 SrcCol, col16 DestCol, alpha Alpha) {
  int Red   = (SrcCol & 0xF800) + NewRedLuminance;
  int Green = (SrcCol & 0x7E0 ) + NewGreenLuminance;
  int Blue  = (SrcCol & 0x1F  ) + NewBlueLuminance;

  if(cc == 2)
  {
    Green += (Red >> 14) << 5;
    Blue  += (Red >> 15);
  }
  if(cc == 1)
    Red += ((Green >> 10) + (Blue >> 5)) << 11;

  if(Red   >= 0) {if(Red   > 0xF800) Red   = 0xF800;} else Red   = 0;
  if(Green >= 0) {if(Green > 0x7E0 ) Green = 0x7E0 ;} else Green = 0;
  if(Blue  >= 0) {if(Blue  > 0x1F  ) Blue  = 0x1F  ;} else Blue  = 0;

  int DestRed = (DestCol & 0xF800);
  int DestGreen = (DestCol & 0x7E0);
  int DestBlue = (DestCol & 0x1F);

  if(cc != 2)
    Red = (((Red - DestRed) * Alpha >> 8) + DestRed) & 0xF800;
  else
    Red = DestRed;
  if(cc != 1)
  {
    Green = (((Green - DestGreen) * Alpha >> 8) + DestGreen) & 0x7E0;
    Blue = ((Blue - DestBlue) * Alpha >> 8) + DestBlue;
  }
  else
  {
    Green = DestGreen;
    Blue = DestBlue;
  }
  return Red | Green | Blue;
}

struct boundbox {
  int xmin, ymin, xmax, ymax;
  boundbox(fpoint bp[4]) {
    int sx = game::GetScreenXSize();
    int sy = game::GetScreenYSize();

    xmin = sx*16; ymin = sy*16; xmax = 0; ymax = 0;

    for(int k=0; k<4; k++) {
      if(bp[k].x < xmin) xmin = int(floor(bp[k].x));
      if(bp[k].x > xmax) xmax = int(ceil (bp[k].x));
      if(bp[k].y < ymin) ymin = int(floor(bp[k].y));
      if(bp[k].y > ymax) ymax = int(ceil (bp[k].y));
    }

    if(xmin < 0) xmin = 0; if(xmax >= sx*16) xmax = sx*16-1;
    if(ymin < 0) ymin = 0; if(ymax >= sy*16) ymax = sy*16-1;
  }
};

void fDrawRect(fpoint a, fpoint b, fpoint c, fpoint d, col16 SrcCol, alpha Alpha) {

  fpoint p[4];
  p[0] = a; p[1] = b; p[2] = c; p[3] = d;
  if((!a.ok) || (!b.ok) || (!c.ok) || (!d.ok)) return;
  boundbox bb(p);

  for(int x=bb.xmin; x<=bb.xmax; x++) for(int y=bb.ymin; y<=bb.ymax; y++) {
    if(cross(a,b,x,y) > -1e-6 &&
       cross(b,c,x,y) > +1e-6 &&
       cross(c,d,x,y) > +1e-6 &&
       cross(d,a,x,y) > -1e-6
       ) {

      col16 DestCol = DOUBLE_BUFFER->GetPixel(16+x, 32+y);

      DOUBLE_BUFFER->PutPixel(16+x, 32+y, apply(SrcCol, DestCol, Alpha));
    }
  }
}

#define SetCA \
  col16 c = b->GetPixel(B.Src.X+px, B.Src.Y+py);    \
  if(c == mask) continue;                           \
  if(non != -1) { Alpha = (c == non) ? 192 : 256; } \
  if((mode & MF_BLIT_ALPHA) && b->GetAlphaMap())    \
    Alpha = b->GetAlpha(B.Src.X+px, B.Src.Y+py);    \

#define RF2(x) (RF(x+1)?x+1:x)
#define RF4(x) (RF(x+2)?RF2(x+2):RF2(x))
#define RF8(x) (RF(x+4)?RF4(x+4):RF4(x))
#define RF16(v) if(RF(8)) \
  {if(RF(16)) continue; v = RF8(8);} else {if(!RF(0)) continue; v = RF8(0);}

void tileRect(const bitmap *b, blitdata &B, int mode,
  fpoint4 corner, fpoint4 dx, fpoint4 dy, bool mirror, int non) {

  if(ivanconfig::GetAnaglyph() && (currEye == 0))
  {
    currEye = ivanconfig::GetAnaglyph(); cc = 1;
    tileRect(b,B,mode,corner,dx,dy,mirror,non);
    currEye = -ivanconfig::GetAnaglyph(); cc = 2;
    tileRect(b,B,mode,corner,dx,dy,mirror,non);
    currEye = 0; cc = 0;
    return;
  }
  if(igraph::defy != -1) {
    corner.x += igraph::defx * 64;
    corner.y += igraph::defy * 64;
  }
  fpoint4 p[32][32];
  fpoint pt[32][32];
  p[0][0] = corner;
  for(int py=0; py<16; py++) p[py+1][0] = p[py][0] + dy;
  for(int py=0; py<=16; py++) {
    for(int px=0; px<16; px++) p[py][px+1] = p[py][px] + dx;
  }
  for(int py=0; py<=16; py++) {
    pt[py][0]  = fpoint(p[py][0]);
    pt[py][16] = fpoint(p[py][16]);
    pt[0][py]  = fpoint(p[0][py]);
    pt[16][py] = fpoint(p[16][py]);
  }
  int Alpha = 256;
  int mask = B.MaskColor;
  if((!pt[0][0].ok) && (!pt[0][16].ok) && (!pt[16][0].ok) && (!pt[16][16].ok))
    return;
  if(pt[0][0].ok && pt[0][16].ok && pt[16][0].ok && pt[16][16].ok) {
    fpoint p[4];
    p[0] = pt[0][0]; p[1] = pt[0][16]; p[2] = pt[16][0]; p[3] = pt[16][16];
    boundbox bb(p);
    for(int x=bb.xmin; x<=bb.xmax; x++) for(int y=bb.ymin; y<=bb.ymax; y++) {
      int px, py;
      if(mirror) {
#define RF(c) (cross(pt[0][c], pt[16][c], x,y) > -1e-6)
        RF16(px);
#undef RF
#define RF(c) (cross(pt[c][0], pt[c][16], x,y) < -1e-6)
        RF16(py);
#undef RF
      }
      else {
#define RF(c) (cross(pt[0][c], pt[16][c], x,y) < -1e-6)
        RF16(px);
#undef RF
#define RF(c) (cross(pt[c][0], pt[c][16], x,y) > -1e-6)
        RF16(py);
#undef RF
      }
      SetCA;
      col16 DestCol = DOUBLE_BUFFER->GetPixel(16+x, 32+y);
      DOUBLE_BUFFER->PutPixel(16+x, 32+y, apply(c, DestCol, Alpha));
    }
  }
  else {
    for(int px=1; px<16; px++) for(int py=1; py<16; py++)
      pt[py][px] = fpoint(p[py][px]);
    for(int px=0; px<16; px++) for(int py=0; py<16; py++) {
      SetCA;
      if(mirror)
        fDrawRect(pt[py][px], pt[py+1][px], pt[py+1][px+1], pt[py][px+1], c,Alpha);
      else
        fDrawRect(pt[py][px], pt[py][px+1], pt[py+1][px+1], pt[py+1][px], c,Alpha);
    }
  }
}

void tileObject(const bitmap *b, blitdata &B, int mode);

void tileFloor(const bitmap *b, blitdata &B, int mode) {
  tileRect(b, B, mode,
    fpoint4(0,0,0), fpoint4(4,0,0), fpoint4(0,4,0), true, -1
  );
}

void tileTable(const bitmap *b, blitdata &B, int mode) {
  tileRect(b, B, mode,
    fpoint4(0,0,16), fpoint4(4,0,0), fpoint4(0,4,0), true, -1
  );
}

void tileCeil(const bitmap *b, blitdata &B, int mode) {
  tileRect(b, B, mode,
    fpoint4(0,0,64), fpoint4(4,0,0), fpoint4(0,4,0), false, -1
  );
}

void tileWalls(const bitmap *b, blitdata &B, int mode) {
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(64,0,64), fpoint4(-4,0,0), fpoint4(0,0,-4), true, -1
  );
  tileRect(b, B, mode,
    fpoint4(64,64,64), fpoint4(0,-4,0), fpoint4(0,0,-4), true, -1
  );
  tileRect(b, B, mode,
    fpoint4(0,64,64), fpoint4(4,0,0), fpoint4(0,0,-4), true, -1
  );
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(0,0,64), fpoint4(0,4,0), fpoint4(0,0,-4), true, -1
  );
  if(ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(0,0,64), fpoint4(4,0,0), fpoint4(0,4,0), true, -1
  );
}

void tileWindow(const bitmap *b, blitdata &B, int mode) {
  int cw = b->GetPixel(8,8);
  if(ivanconfig::Mode3Iso()) cw = -1;
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(64,0,64), fpoint4(-4,0,0), fpoint4(0,0,-4), true, cw
  );
  tileRect(b, B, mode,
    fpoint4(64,64,64), fpoint4(0,-4,0), fpoint4(0,0,-4), true, cw
  );
  tileRect(b, B, mode,
    fpoint4(0,64,64), fpoint4(4,0,0), fpoint4(0,0,-4), true, cw
  );
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(0,0,64), fpoint4(0,4,0), fpoint4(0,0,-4), true, cw
  );
  if(ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(0,0,64), fpoint4(4,0,0), fpoint4(0,4,0), true, -1
  );
}

void tileLowWalls(const bitmap *b, blitdata &B, int mode) {
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(64,0,16), fpoint4(-4,0,0), fpoint4(0,0,-1), true, -1
  );
  tileRect(b, B, mode,
    fpoint4(64,64,16), fpoint4(0,-4,0), fpoint4(0,0,-1), true, -1
  );
  tileRect(b, B, mode,
    fpoint4(0,64,16), fpoint4(4,0,0), fpoint4(0,0,-1), true, -1
  );
  if(!ivanconfig::Mode3Iso())
  tileRect(b, B, mode,
    fpoint4(0,0,16), fpoint4(0,4,0), fpoint4(0,0,-1), true, -1
  );
}

void tileObject(const bitmap *b, blitdata &B, int mode) {
  currAY = 24;
  tileRect(b, B, mode,
    fpoint4(32,32,64,-32), fpoint4(0,0,0,4), fpoint4(0,0,-4), true, -1
  );
  currAY = 0;
}

void tileLarge(const bitmap *b, blitdata &B, int mode) {
  int i = B.CustomData & SQUARE_INDEX_MASK;
  int cx = 1-i&1, cy = 1-(i>>1)&1;
  if(ivanconfig::Mode3Iso()) {
    tileRect(b, B, mode,
      fpoint4(cx*64,cy*64,64+cy*64,-cx*64), fpoint4(0,0,0,4), fpoint4(0,0,-4), true, -1
    );
    return;
  }
  currAY = 64;
  tileRect(b, B, mode,
    fpoint4(cx*64,cy*64,32+cy*32,-cx*48), fpoint4(0,0,0,3), fpoint4(0,0,-2), true, -1
  );
  currAY = 0;
}

void tileOnFloor(const bitmap *b, blitdata &B, int mode) {
  int m = B.CustomData & SQUARE_INDEX_MASK;
  if(m)
  {
  if(m == 1+DOWN)
  tileRect(b, B, mode,
    fpoint4(0,0,48), fpoint4(4,0,0), fpoint4(0,0,-4), true, -1
  );
  if(m == 1+LEFT)
  tileRect(b, B, mode,
    fpoint4(64,0,-16), fpoint4(0,0,4), fpoint4(0,4,0), true, -1
  );
  if(m == 1+UP)
  tileRect(b, B, mode,
    fpoint4(0,64,-16), fpoint4(4,0,0), fpoint4(0,0,4), true, -1
  );
  if(m == 1+RIGHT)
  tileRect(b, B, mode,
    fpoint4(0,0,48), fpoint4(0,0,-4), fpoint4(0,4,0), true, -1
  );
  return;
  }
  if(ivanconfig::Mode3Iso()) {
    tileObject(b, B, mode);
    return;
  }

  for(int y=15; y>=0; y--)
  for(int x=0; x<16; x++)
  if(b->GetPixel(x,y) != B.MaskColor)
  {
    currAY = 12;
    tileRect(b, B, mode,
      fpoint4(32,32,2*y+2,-16), fpoint4(0,0,0,2), fpoint4(0,0,-2), true, -1
    );
    currAY = 0;
    return;
  }
}

void igraph::fDrawRainPixel(v2 p, col16 c) {
  NewRedLuminance = NewGreenLuminance = NewBlueLuminance = 0;
  fDrawRect(
    fpoint(fpoint4(defx*64+32, defy*64+32, 64-p.Y*4, p.X*4-32)),
    fpoint(fpoint4(defx*64+32, defy*64+32, 60-p.Y*4, p.X*4-32)),
    fpoint(fpoint4(defx*64+32, defy*64+32, 60-p.Y*4, p.X*4-28)),
    fpoint(fpoint4(defx*64+32, defy*64+32, 64-p.Y*4, p.X*4-28)),
    c,255
    );
  }

int igraph::defx;
int igraph::defy;
truth igraph::noCeiling;

#include <message.h>

void igraph::Blit3(const bitmap *b, blitdata &B, int mode) {

  if(!(B.CustomData & DO_BLIT3)) switch(mode & MF_BLIT_FLAGS) {
    case MF_BLIT_ALPHA_PRIORITY: b->AlphaPriorityBlit(B); return;
    case MF_BLIT_ALPHA_LUMINANCE: b->AlphaLuminanceBlit(B); return;
    case MF_BLIT_MASKED_PRIORITY: b->MaskedPriorityBlit(B); return;
    case MF_BLIT_LUMINANCE_MASKED: b->LuminanceMaskedBlit(B); return;
    default:
      ADD_MESSAGE("Unknown flag %d.\n", mode);
      ivanconfig::SwitchMode3();
    }
  setLum(B, mode);

  if(defy == -1)
  {
    truth tr = defx != -1;
    if(b->GetPixel(12,12) != B.MaskColor)
    tileRect(b, B, mode,
      fpoint4(0,0,0), fpoint4(1,0,0), fpoint4(0,1,0), tr, -1
    );
    else
    tileRect(b, B, mode,
      fpoint4(15,0,0), fpoint4(-1,0,0), fpoint4(0,1,0), !tr, -1
    );
    return;
  }

  currEye = 0;

  if(mode & MF_FLOOR)   tileFloor   (b, B, mode);
  if(mode & MF_CEIL)    tileCeil    (b, B, mode);
  if(mode & MF_OBJECT)  tileObject  (b, B, mode);
  if(mode & MF_STAIR)   if(ivanconfig::Mode3Iso()) tileObject  (b, B, mode);
  if(mode & MF_ONFLOOR) tileOnFloor (b, B, mode);
  if(mode & MF_WALL)    tileWalls   (b, B, mode);
  if(mode & MF_LOWWALL) tileLowWalls(b, B, mode);
  if(mode & MF_TABLE)   tileTable   (b, B, mode);
  if(mode & MF_WINDOW)  tileWindow  (b, B, mode);
  if(mode & MF_LARGE)   tileLarge   (b, B, mode);
  }
