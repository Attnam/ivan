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

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "v2.h"
#include "SDL.h"

class bitmap;
class rawbitmap;
class outputfile;
class inputfile;
class festring;

typedef void (*bitmapeditor)(bitmap*, truth);

struct blitdata
{
  bitmap* Bitmap;
  v2 Src;
  v2 Dest;
  v2 Border;
  union
  {
    col24 Luminance;
    int Flags, Stretch;
  };
  col16 MaskColor;
  ulong CustomData;
};

class bitmap
{
 public:
  friend class cachedfont;
  bitmap(cfestring&);
  bitmap(cbitmap*, int = 0, truth = true);
  bitmap(v2);
  bitmap(v2, col16);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(cfestring&) const;
  void PutPixel(int X, int Y, col16 Color) { Image[Y][X] = Color; }
  void PutPixel(v2 Pos, col16 Color) { Image[Pos.Y][Pos.X] = Color; }
  void PowerPutPixel(int, int, col16, alpha, priority);
  col16 GetPixel(int X, int Y) const { return Image[Y][X]; }
  col16 GetPixel(v2 Pos) const { return Image[Pos.Y][Pos.X]; }

  void Fill(int, int, int, int, col16);
  void Fill(v2, int, int, col16);
  void Fill(int, int, v2, col16);
  void Fill(v2, v2, col16);

  void ClearToColor(col16);
  void NormalBlit(cblitdata&) const;
  void NormalBlit(bitmap*, int = 0) const;
  void FastBlit(bitmap*) const;
  void FastBlit(bitmap*, v2) const;

  void LuminanceBlit(cblitdata&) const;
  void NormalMaskedBlit(cblitdata&) const;
  void LuminanceMaskedBlit(cblitdata&) const;
  void SimpleAlphaBlit(bitmap*, alpha, col16 = TRANSPARENT_COLOR) const;
  void AlphaMaskedBlit(cblitdata&) const;
  void AlphaLuminanceBlit(cblitdata&) const;

  void DrawLine(int, int, int, int, col16, truth = false);
  void DrawLine(v2, int, int, col16, truth = false);
  void DrawLine(int, int, v2, col16, truth = false);
  void DrawLine(v2, v2, col16, truth = false);

  void DrawVerticalLine(int, int, int, col16, truth = false);
  void DrawHorizontalLine(int, int, int, col16, truth = false);

  void StretchBlit(cblitdata&) const;
  void StretchBlitXbrz(cblitdata&);

  void DrawRectangle(int, int, int, int, col16, truth = false);
  void DrawRectangle(v2, int, int, col16, truth = false);
  void DrawRectangle(int, int, v2, col16, truth = false);
  void DrawRectangle(v2, v2, col16, truth = false);

  void BlitAndCopyAlpha(bitmap*, int = 0) const;
  void MaskedPriorityBlit(cblitdata&) const;
  void AlphaPriorityBlit(cblitdata&) const;
  void FastBlitAndCopyAlpha(bitmap*) const;
  v2 GetSize() const { return Size; }
  void DrawPolygon(int, int, int, int, col16, truth = true, truth = false, double = 0);
  void CreateAlphaMap(alpha);
  truth Fade(long&, packalpha&, int);
  void SetAlpha(int X, int Y, alpha Alpha) { AlphaMap[Y][X] = Alpha; }
  void SetAlpha(v2 Pos, alpha Alpha) { AlphaMap[Pos.Y][Pos.X] = Alpha; }
  alpha GetAlpha(int X, int Y) const { return AlphaMap[Y][X]; }
  alpha GetAlpha(v2 Pos) const { return AlphaMap[Pos.Y][Pos.X]; }
  void Outline(col16, alpha, priority);
  void FadeToScreen(bitmapeditor = 0);
  void CreateFlames(rawbitmap*, v2, ulong, int);
  truth IsValidPos(v2 What) const { return What.X >= 0 && What.Y >= 0 && What.X < Size.X && What.Y < Size.Y; }
  truth IsValidPos(int X, int Y) const { return X >= 0 && Y >= 0 && X < Size.X && Y < Size.Y; }
  void CreateSparkle(v2, int);
  void CreateFlies(ulong, int, int);
  void CreateLightning(ulong, col16);
  truth CreateLightning(v2, v2, int, col16);
  packcol16** GetImage() const { return Image; }
  packalpha** GetAlphaMap() const { return AlphaMap; }
  static truth PixelVectorHandler(long, long);
  void FillAlpha(alpha);
  void InitPriorityMap(priority);
  void FillPriority(priority);
  void SafeSetPriority(int, int, priority);
  void SafeSetPriority(v2 Pos, priority What) { SafeSetPriority(Pos.X, Pos.Y, What); }
  void SafeUpdateRandMap(v2, truth);
  void UpdateRandMap(long, truth);
  void InitRandMap();
  v2 RandomizePixel() const;
  void AlphaPutPixel(int, int, col16, col24, alpha);
  void AlphaPutPixel(v2 Pos, col16 Color, col24 Luminance, alpha Alpha) { AlphaPutPixel(Pos.X, Pos.Y, Color, Luminance, Alpha); }
  void CalculateRandMap();
  alpha CalculateAlphaAverage() const;
  void ActivateFastFlag() { FastFlag = 1; }
  void DeactivateFastFlag() { FastFlag = 0; }
  void Wobble(int, int, truth);
  void MoveLineVertically(int, int);
  void MoveLineHorizontally(int, int);
  void InterLace();
 protected:
  v2 Size;
  ulong XSizeTimesYSize : 31;
  ulong FastFlag : 1;
  packcol16** Image;
  packalpha** AlphaMap;
  packpriority** PriorityMap;
  truth* RandMap;
  SDL_Surface* img;
  SDL_Surface* imgStretched;
};

inline void bitmap::SafeUpdateRandMap(v2 Pos, truth What)
{
  if(RandMap)
    UpdateRandMap(Pos.Y * Size.X + Pos.X, What);
}

inline void bitmap::SafeSetPriority(int x, int y, priority What)
{
  if(PriorityMap)
    PriorityMap[y][x] = What;
}

inline void bitmap::FastBlit(bitmap* Bitmap) const
{
  memcpy(Bitmap->Image[0], Image[0], XSizeTimesYSize * sizeof(packcol16));
}

inline void bitmap::FastBlit(bitmap* Bitmap, v2 Pos) const
{
  packcol16** SrcImage = Image;
  packcol16** DestImage = Bitmap->Image;
  cint Bytes = Size.X * sizeof(packcol16);
  cint Height = Size.Y;

  for(int y = 0; y < Height; ++y)
    memcpy(&DestImage[Pos.Y + y][Pos.X], SrcImage[y], Bytes);
}

inline void bitmap::NormalBlit(bitmap* Bitmap, int Flags) const
{
  blitdata B = { Bitmap,
                 { 0, 0 },
                 { 0, 0 },
                 { Size.X, Size.Y },
                 { static_cast<col24>(Flags) }, // stupid union initialization rules...
                 0,
                 0 };
  NormalBlit(B);
}

outputfile& operator<<(outputfile&, cbitmap*);
inputfile& operator>>(inputfile&, bitmap*&);

class cachedfont : public bitmap
{
 public:
  cachedfont(v2);
  cachedfont(v2, col16);
  ~cachedfont() { delete [] MaskMap; }
  void PrintCharacter(cblitdata) const;
  void CreateMaskMap();
 private:
  packcol16** MaskMap;
};

#endif
