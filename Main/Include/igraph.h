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

#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#include <map>

#include "ivandef.h"
#include "femath.h"

class bitmap;
class rawbitmap;
class outputfile;
class inputfile;
class festring;

/* memcmp doesn't like alignment of structure members */

#ifdef VC
#pragma pack(1)
#endif

struct graphicid
{
  graphicid() { }
  bool operator<(const graphicid&) const;
  ushort BitmapPosX NO_ALIGNMENT;
  ushort BitmapPosY NO_ALIGNMENT;
  packcol16 Color[4] NO_ALIGNMENT;
  uchar Frame NO_ALIGNMENT;
  uchar FileIndex NO_ALIGNMENT;
  ushort SpecialFlags NO_ALIGNMENT;
  packalpha Alpha[4] NO_ALIGNMENT;
  packalpha BaseAlpha NO_ALIGNMENT;
  uchar SparkleFrame NO_ALIGNMENT;
  uchar SparklePosX NO_ALIGNMENT;
  uchar SparklePosY NO_ALIGNMENT;
  packcol16 OutlineColor NO_ALIGNMENT;
  packalpha OutlineAlpha NO_ALIGNMENT;
  uchar FlyAmount NO_ALIGNMENT;
  v2 Position NO_ALIGNMENT;
  uchar RustData[4] NO_ALIGNMENT;
  ushort Seed NO_ALIGNMENT;
  uchar WobbleData NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

inline bool graphicid::operator<(const graphicid& GI) const
{
  return memcmp(this, &GI, sizeof(graphicid)) < 0;
}

outputfile& operator<<(outputfile&, const graphicid&);
inputfile& operator>>(inputfile&, graphicid&);

struct tile
{
  tile() { }
  tile(bitmap* Bitmap) : Bitmap(Bitmap), Users(1) { }
  bitmap* Bitmap;
  long Users;
};

typedef std::map<graphicid, tile> tilemap;

struct graphicdata
{
  graphicdata() : AnimationFrames(0) { }
  ~graphicdata();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Retire();
  int AnimationFrames;
  bitmap** Picture;
  tilemap::iterator* GraphicIterator;
};

outputfile& operator<<(outputfile&, const graphicdata&);
inputfile& operator>>(inputfile&, graphicdata&);

class igraph
{
 public:
  static void Init();
  static void DeInit();
  static cbitmap* GetWTerrainGraphic() { return Graphic[GR_WTERRAIN]; }
  static cbitmap* GetFOWGraphic() { return Graphic[GR_FOW]; }
  static const rawbitmap* GetCursorRawGraphic() { return RawGraphic[GR_CURSOR]; }
  static cbitmap* GetSymbolGraphic() { return Graphic[GR_SYMBOL]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(v2, int, int = 0);
  static tilemap::iterator AddUser(const graphicid&);
  static void RemoveUser(tilemap::iterator);
  static const rawbitmap* GetHumanoidRawGraphic() { return RawGraphic[GR_HUMANOID]; }
  static const rawbitmap* GetCharacterRawGraphic() { return RawGraphic[GR_CHARACTER]; }
  static const rawbitmap* GetEffectRawGraphic() { return RawGraphic[GR_EFFECT]; }
  static const rawbitmap* GetRawGraphic(int I) { return RawGraphic[I]; }
  static cint* GetBodyBitmapValidityMap(int);
  static bitmap* GetFlagBuffer() { return FlagBuffer; }
  static cbitmap* GetMenuGraphic() { return Menu; }
  static void LoadMenu();
  static void UnLoadMenu();
  static bitmap* GetSilhouetteCache(int I1, int I2, int I3) { return SilhouetteCache[I1][I2][I3]; }
  static cbitmap* GetBackGround() { return BackGround; }
  static void BlitBackGround(v2, v2);
  static void CreateBackGround(int);
  static bitmap* GenerateScarBitmap(int, int, int);
  static cbitmap* GetSmileyGraphic() { return Graphic[GR_SMILEY]; }
 private:
  static void EditBodyPartTile(rawbitmap*, rawbitmap*, v2, int);
  static v2 RotateTile(rawbitmap*, rawbitmap*, v2, v2, int);
  static void CreateBodyBitmapValidityMaps();
  static void CreateSilhouetteCaches();
  static col16 GetBackGroundColor(int);
  static rawbitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES];
  static bitmap* TileBuffer;
  static cchar* RawGraphicFileName[];
  static cchar* GraphicFileName[];
  static tilemap TileMap;
  static uchar RollBuffer[256];
  static bitmap* FlagBuffer;
  static int** BodyBitmapValidityMap;
  static bitmap* Menu;
  static bitmap* SilhouetteCache[HUMANOID_BODYPARTS][CONDITION_COLORS][SILHOUETTE_TYPES];
  static rawbitmap* ColorizeBuffer[2];
  static bitmap* Cursor[CURSOR_TYPES];
  static bitmap* BigCursor[CURSOR_TYPES];
  static col16 CursorColor[CURSOR_TYPES];
  static bitmap* BackGround;
  static int CurrentColorType;
};

#endif
