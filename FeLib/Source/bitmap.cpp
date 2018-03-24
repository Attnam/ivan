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

#include <cmath>
#include <ctime>
#include <iostream>

#include "bitmap.h"
#include "graphics.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"
#include "rawbit.h"
#include "libxbrzscale.h"

/*
 * Blitting must be as fast as possible, even if no optimizations are used;
 * therefore we can't use inline functions inside loops, since they may be
 * left unexpanded. These macros will do the job efficiently, even if they
 * are rather ugly
 */

#define LOAD_SRC() int SrcCol = *SrcPtr;
#define LOAD_DEST() int DestCol = *DestPtr;
#define LOAD_ALPHA() int Alpha = *AlphaPtr;

#define STORE_COLOR() *DestPtr = Red | Green | Blue;

#define NEW_LUMINATE_RED()\
int Red = (SrcCol & 0xF800) + NewRedLuminance;\
\
if(Red >= 0)\
{\
  if(Red > 0xF800)\
    Red = 0xF800;\
}\
else\
  Red = 0;

#define NEW_LUMINATE_GREEN()\
int Green = (SrcCol & 0x7E0) + NewGreenLuminance;\
\
if(Green >= 0)\
{\
  if(Green > 0x7E0)\
    Green = 0x7E0;\
}\
else\
  Green = 0;

#define NEW_LUMINATE_BLUE()\
int Blue = (SrcCol & 0x1F) + NewBlueLuminance;\
\
if(Blue >= 0)\
{\
  if(Blue > 0x1F)\
    Blue = 0x1F;\
}\
else\
  Blue = 0;

#define NEW_APPLY_ALPHA_RED()\
{\
  int DestRed = (DestCol & 0xF800);\
  Red = (((Red - DestRed) * Alpha >> 8) + DestRed) & 0xF800;\
}

#define NEW_APPLY_ALPHA_GREEN()\
{\
  int DestGreen = (DestCol & 0x7E0);\
  Green = (((Green - DestGreen) * Alpha >> 8) + DestGreen) & 0x7E0;\
}

#define NEW_APPLY_ALPHA_BLUE()\
{\
  int DestBlue = (DestCol & 0x1F);\
  Blue = ((Blue - DestBlue) * Alpha >> 8) + DestBlue;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_RED()\
int Red;\
{\
  int DestRed = DestCol & 0xF800;\
  Red = ((((SrcCol & 0xF800) - DestRed) * Alpha >> 8) + DestRed) & 0xF800;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_GREEN()\
int Green;\
{\
  int DestGreen = DestCol & 0x7E0;\
  Green = ((((SrcCol & 0x7E0) - DestGreen) * Alpha >> 8) + DestGreen)\
          & 0x7E0;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_BLUE()\
int Blue;\
{\
  int DestBlue = DestCol & 0x1F;\
  Blue = (((SrcCol & 0x1F) - DestBlue) * Alpha >> 8) + DestBlue;\
}

bitmap::bitmap(cfestring& FileName)
: FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0), img(0), imgStretched(0)
{
  rawbitmap Temp(FileName);
  Size = Temp.Size;
  XSizeTimesYSize = Size.X * Size.Y;
  Alloc2D(Image, Size.Y, Size.X);
  packcol16* Buffer = Image[0];
  paletteindex* TempBuffer = Temp.PaletteBuffer[0];

  for(int y = 0; y < Size.Y; ++y)
    for(int x = 0; x < Size.X; ++x)
    {
      int Char1 = *TempBuffer++;
      int Char3 = Char1 + (Char1 << 1);
      *Buffer++ = int(Temp.Palette[Char3] >> 3) << 11
                | int(Temp.Palette[Char3 + 1] >> 2) << 5
                | int(Temp.Palette[Char3 + 2] >> 3);
    }
}

bitmap::bitmap(cbitmap* Bitmap, int Flags, truth CopyAlpha)
: bitmap(Bitmap->Size)
{
  if(CopyAlpha && Bitmap->AlphaMap)
  {
    Alloc2D(AlphaMap, Size.Y, Size.X);
    Bitmap->BlitAndCopyAlpha(this, Flags);
  }
  else
  {
    if(!Flags)
      Bitmap->FastBlit(this);
    else
      Bitmap->NormalBlit(this, Flags);
  }
}

bitmap::bitmap(v2 Size)
: Size(Size), XSizeTimesYSize(Size.X * Size.Y),
  FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0), img(0), imgStretched(0)
{
  Alloc2D(Image, Size.Y, Size.X);
}

bitmap::bitmap(v2 Size, col16 Color)
: bitmap(Size)
{
  ClearToColor(Color);
}

bitmap::~bitmap()
{
  delete [] Image;
  delete [] AlphaMap;
  delete [] PriorityMap;
  delete [] RandMap;
}

void bitmap::Save(outputfile& SaveFile) const
{
  SaveFile.Write(reinterpret_cast<char*>(Image[0]),
                 XSizeTimesYSize * sizeof(packcol16));

  if(AlphaMap)
  {
    SaveFile.Put(true);
    SaveFile.Write(reinterpret_cast<char*>(AlphaMap[0]),
                   XSizeTimesYSize * sizeof(packalpha));
  }
  else
    SaveFile.Put(false);

  if(PriorityMap)
  {
    SaveFile.Put(true);
    SaveFile.Write(reinterpret_cast<char*>(PriorityMap[0]),
                   XSizeTimesYSize * sizeof(packpriority));
  }
  else
    SaveFile.Put(false);

  SaveFile << uchar(FastFlag);
}

void bitmap::Load(inputfile& SaveFile)
{
  SaveFile.Read(reinterpret_cast<char*>(Image[0]),
                XSizeTimesYSize * sizeof(packcol16));

  if(SaveFile.Get())
  {
    Alloc2D(AlphaMap, Size.Y, Size.X);
    SaveFile.Read(reinterpret_cast<char*>(AlphaMap[0]),
                  XSizeTimesYSize * sizeof(packalpha));
  }

  if(SaveFile.Get())
  {
    Alloc2D(PriorityMap, Size.Y, Size.X);
    SaveFile.Read(reinterpret_cast<char*>(PriorityMap[0]),
                  XSizeTimesYSize * sizeof(packpriority));
  }

  FastFlag = ReadType<uchar>(SaveFile);
}

void bitmap::Save(cfestring& FileName) const
{
  static char BMPHeader[] =
  {
    char(0x42), char(0x4D), char(0xB6), char(0x4F), char(0x12), char(0x00),
    char(0x00), char(0x00), char(0x00), char(0x00), char(0x36), char(0x00),
    char(0x00), char(0x00), char(0x28), char(0x00), char(0x00), char(0x00),
    char(0x20), char(0x03), char(0x00), char(0x00), char(0xF4), char(0x01),
    char(0x00), char(0x00), char(0x01), char(0x00), char(0x18), char(0x00),
    char(0x00), char(0x00), char(0x00), char(0x00), char(0x80), char(0x4F),
    char(0x12), char(0x00), char(0x33), char(0x0B), char(0x00), char(0x00),
    char(0x33), char(0x0B), char(0x00), char(0x00), char(0x00), char(0x00),
    char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00)
  };

  outputfile SaveFile(FileName);
  BMPHeader[0x12] =  Size.X       & 0xFF;
  BMPHeader[0x13] = (Size.X >> 8) & 0xFF;
  BMPHeader[0x16] =  Size.Y       & 0xFF;
  BMPHeader[0x17] = (Size.Y >> 8) & 0xFF;
  SaveFile.Write(BMPHeader, 0x36);

  for(int y = Size.Y - 1; y >= 0; --y)
    for(int x = 0; x < Size.X; ++x)
    {
      col16 Pixel = GetPixel(x, y);
      SaveFile << char(Pixel << 3)
               << char((Pixel >> 5) << 2)
               << char((Pixel >> 11) << 3);
    }
}

void bitmap::Fill(v2 TopLeft, int Width, int Height, col16 Color)
{ Fill(TopLeft.X, TopLeft.Y, Width, Height, Color); }
void bitmap::Fill(int X, int Y, v2 FillSize, col16 Color)
{ Fill(X, Y, FillSize.X, FillSize.Y, Color); }
void bitmap::Fill(v2 TopLeft, v2 FillSize, col16 Color)
{ Fill(TopLeft.X, TopLeft.Y, FillSize.X, FillSize.Y, Color); }

void bitmap::Fill(int X, int Y, int Width, int Height, col16 Color)
{
  if(X >= Size.X || Y >= Size.Y)
    return;

  if(X + Width > Size.X)
    Width = Size.X - X;

  if(Y + Height > Size.Y)
    Height = Size.Y - Y;

  if(Color >> 8 == (Color & 0xFF))
  {
    Width <<= 1;

    for(int y = 0; y < Height; ++y)
      memset(&Image[Y + y][X], Color, Width);
  }
  else
    for(int y = 0; y < Height; ++y)
    {
      packcol16* Ptr = &Image[Y + y][X];
      cpackcol16*const EndPtr = Ptr + Width;

      while(Ptr != EndPtr)
        *Ptr++ = Color;
    }
}

void bitmap::ClearToColor(col16 Color)
{
  packcol16* Ptr = Image[0];

  if(Color >> 8 == (Color & 0xFF))
    memset(Ptr, Color, XSizeTimesYSize * sizeof(packcol16));
  else
  {
    cpackcol16*const EndPtr = Ptr + XSizeTimesYSize;

    while(Ptr != EndPtr)
      *Ptr++ = Color;
  }
}

void bitmap::NormalBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap blit attempt detected!");

    if(B.Flags & ROTATE && B.Border.X != B.Border.Y)
      ABORT("Blit error: FeLib supports only square rotating!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packcol16** DestImage = B.Bitmap->Image;

  switch(B.Flags & 7)
  {
   case NONE:
    {
      if(!B.Src.X && !B.Src.Y && !B.Dest.X && !B.Dest.Y
         && B.Border.X == Size.X && B.Border.Y == Size.Y
         && B.Border.X == B.Bitmap->Size.X && B.Border.Y == B.Bitmap->Size.Y)
        memcpy(DestImage[0], SrcImage[0], XSizeTimesYSize * sizeof(packcol16));
      else
      {
        cint Bytes = B.Border.X * sizeof(packcol16);

        for(int y = 0; y < B.Border.Y; ++y)
          memcpy(&DestImage[B.Dest.Y + y][B.Dest.X], &SrcImage[B.Src.Y + y][B.Src.X], Bytes);
      }

      break;
    }

   case MIRROR:
    {
      B.Dest.X += B.Border.X - 1;

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
          *DestPtr = *SrcPtr;
      }

      break;
    }

   case FLIP:
    {
      B.Dest.Y += B.Border.Y - 1;
      cint Bytes = B.Border.X * sizeof(packcol16);

      for(int y = 0; y < B.Border.Y; ++y)
        memcpy(&DestImage[B.Dest.Y - y][B.Dest.X], &SrcImage[B.Src.Y + y][B.Src.X], Bytes);

      break;
    }

   case (MIRROR | FLIP):
    {
      B.Dest.X += B.Border.X - 1;
      B.Dest.Y += B.Border.Y - 1;

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y - y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
          *DestPtr = *SrcPtr;
      }

      break;
    }

   case ROTATE:
    {
      B.Dest.X += B.Border.X - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
          *DestPtr = *SrcPtr;
      }

      break;
    }

   case (MIRROR | ROTATE):
    {
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
          *DestPtr = *SrcPtr;
      }

      break;
    }

   case (FLIP | ROTATE):
    {
      B.Dest.X += B.Border.X - 1;
      B.Dest.Y += B.Border.Y - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
          *DestPtr = *SrcPtr;
      }

      break;
    }

   case (MIRROR | FLIP | ROTATE):
    {
      B.Dest.Y += B.Border.Y - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
          *DestPtr = *SrcPtr;
      }

      break;
    }
  }
}

void bitmap::LuminanceBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(B.Luminance == NORMAL_LUMINANCE)
  {
    B.Flags = 0;
    NormalBlit(B);
    return;
  }

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packcol16** DestImage = B.Bitmap->Image;
  int NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
    {
      LOAD_SRC();
      NEW_LUMINATE_RED();
      NEW_LUMINATE_GREEN();
      NEW_LUMINATE_BLUE();
      STORE_COLOR();
    }
  }
}

void bitmap::NormalMaskedBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap masked blit attempt detected!");

    if(B.Flags & ROTATE && B.Border.X != B.Border.Y)
      ABORT("MaskedBlit error: FeLib supports only square rotating!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packcol16** DestImage = B.Bitmap->Image;
  packcol16 PackedMaskColor = B.MaskColor;

  switch(B.Flags & 7)
  {
   case NONE:
    {
      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case MIRROR:
    {
      B.Dest.X += B.Border.X - 1;

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case FLIP:
    {
      B.Dest.Y += B.Border.Y - 1;

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y - y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case (MIRROR | FLIP):
    {
      B.Dest.X += B.Border.X - 1;
      B.Dest.Y += B.Border.Y - 1;

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = &DestImage[B.Dest.Y - y][B.Dest.X];

        for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case ROTATE:
    {
      B.Dest.X += B.Border.X - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case (MIRROR | ROTATE):
    {
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case (FLIP | ROTATE):
    {
      B.Dest.X += B.Border.X - 1;
      B.Dest.Y += B.Border.Y - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }

   case (MIRROR | FLIP | ROTATE):
    {
      B.Dest.Y += B.Border.Y - 1;
      int TrueDestXMove = B.Bitmap->Size.X;
      packcol16* DestBase = &DestImage[B.Dest.Y][B.Dest.X];

      for(int y = 0; y < B.Border.Y; ++y)
      {
        cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
        cpackcol16* EndPtr = SrcPtr + B.Border.X;
        packcol16* DestPtr = DestBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
          if(*SrcPtr != PackedMaskColor)
            *DestPtr = *SrcPtr;
      }

      break;
    }
  }
}

void bitmap::LuminanceMaskedBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(B.Luminance == NORMAL_LUMINANCE)
  {
    B.Flags = 0;
    NormalMaskedBlit(B);
    return;
  }

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap masked blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packcol16** DestImage = B.Bitmap->Image;
  int NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
    {
      LOAD_SRC();

      if(SrcCol != B.MaskColor)
      {
        NEW_LUMINATE_RED();
        NEW_LUMINATE_GREEN();
        NEW_LUMINATE_BLUE();
        STORE_COLOR();
      }
    }
  }
}

void bitmap::SimpleAlphaBlit(bitmap* Bitmap, alpha Alpha, col16 MaskColor) const
{
  if(Alpha == 255)
  {
    blitdata B = { Bitmap,
                   { 0, 0 },
                   { 0, 0 },
                   { Size.X, Size.Y },
                   { 0 },
                   MaskColor,
                   0 };

    NormalMaskedBlit(B);
    return;
  }

  if(!FastFlag && (Size.X != Bitmap->Size.X || Size.Y != Bitmap->Size.Y))
    ABORT("Fast simple alpha blit attempt of noncongruent bitmaps detected!");

  cpackcol16* SrcPtr = Image[0];
  cpackcol16* EndPtr = SrcPtr + XSizeTimesYSize;
  packcol16* DestPtr = Bitmap->Image[0];

  for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
  {
    LOAD_SRC();

    if(SrcCol != MaskColor)
    {
      LOAD_DEST();
      NEW_LOAD_AND_APPLY_ALPHA_RED();
      NEW_LOAD_AND_APPLY_ALPHA_GREEN();
      NEW_LOAD_AND_APPLY_ALPHA_BLUE();
      STORE_COLOR();
    }
  }
}

void bitmap::AlphaMaskedBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(!AlphaMap)
  {
    B.Flags = 0;
    NormalMaskedBlit(B);
    return;
  }

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap alpha blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packalpha** SrcAlphaMap = AlphaMap;
  packcol16** DestImage = B.Bitmap->Image;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackalpha* AlphaPtr = &SrcAlphaMap[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr)
    {
      LOAD_SRC();

      if(SrcCol != B.MaskColor)
      {
        LOAD_DEST();
        LOAD_ALPHA();
        NEW_LOAD_AND_APPLY_ALPHA_RED();
        NEW_LOAD_AND_APPLY_ALPHA_GREEN();
        NEW_LOAD_AND_APPLY_ALPHA_BLUE();
        STORE_COLOR();
      }
    }
  }
}

void bitmap::DrawLine(v2 From, int ToX, int ToY, col16 Color, truth Wide)
{ DrawLine(From.X, From.Y, ToX, ToY, Color, Wide); }
void bitmap::DrawLine(int FromX, int FromY, v2 To, col16 Color, truth Wide)
{ DrawLine(FromX, FromY, To.X, To.Y, Color, Wide); }
void bitmap::DrawLine(v2 From, v2 To, col16 Color, truth Wide)
{ DrawLine(From.X, From.Y, To.X, To.Y, Color, Wide); }

void bitmap::DrawLine(int OrigFromX, int OrigFromY, int OrigToX, int OrigToY, col16 Color, truth Wide)
{
  if(OrigFromY == OrigToY)
  {
    DrawHorizontalLine(OrigFromX, OrigToX, OrigFromY, Color, Wide);
    return;
  }

  if(OrigFromX == OrigToX)
  {
    DrawVerticalLine(OrigFromX, OrigFromY, OrigToY, Color, Wide);
    return;
  }

  static cint PointX[] = { 0, 0, -1, 1, 0 };
  static cint PointY[] = { 0, -1, 0, 0, 1 };
  cint Times = Wide ? 5 : 1;

  for(int c1 = 0; c1 < Times; ++c1)
  {
    cint X1 = OrigFromX + PointX[c1];
    cint Y1 = OrigFromY + PointY[c1];
    cint X2 = OrigToX + PointX[c1];
    cint Y2 = OrigToY + PointY[c1];
    cint DeltaX = abs(X2 - X1);
    cint DeltaY = abs(Y2 - Y1);
    int x, c2;
    int XChange, PtrXChange, PtrYChange;
    int DoubleDeltaX, DoubleDeltaY, End;

    if(DeltaX >= DeltaY)
    {
      x = X1;
      c2 = DeltaX;
      PtrXChange = XChange = X1 < X2 ? 1 : -1;
      PtrYChange = Y1 < Y2 ? Size.X : -Size.X;
      DoubleDeltaX = DeltaX << 1;
      DoubleDeltaY = DeltaY << 1;
      End = X2;
    }
    else
    {
      x = Y1;
      c2 = DeltaY;
      XChange = Y1 < Y2 ? 1 : -1;
      PtrXChange = Y1 < Y2 ? Size.X : -Size.X;
      PtrYChange = X1 < X2 ? 1 : -1;
      DoubleDeltaX = DeltaY << 1;
      DoubleDeltaY = DeltaX << 1;
      End = Y2;
    }

    packcol16* Ptr = &Image[Y1][X1];
    *Ptr = Color;

    while(x != End)
    {
      x += XChange;
      Ptr += PtrXChange;
      c2 += DoubleDeltaY;

      if(c2 >= DoubleDeltaX)
      {
        c2 -= DoubleDeltaX;
        Ptr += PtrYChange;
      }

      *Ptr = Color;
    }
  }
}

void bitmap::DrawVerticalLine(int OrigX, int OrigFromY, int OrigToY, col16 Color, truth Wide)
{
  static cint PointX[] = { 0, -1, 1 };
  cint Times = Wide ? 3 : 1;

  for(int c = 0; c < Times; ++c)
  {
    int X = OrigX + PointX[c];
    int FromY = OrigFromY;
    int ToY = OrigToY;

    if(FromY > ToY)
      Swap(FromY, ToY);

    if(Wide && !c)
    {
      --FromY;
      ++ToY;
    }

    if(X < 0 || X >= Size.X || ToY < 0 || FromY >= Size.Y)
      continue;

    FromY = Max(FromY, 0);
    ToY = Min(ToY, Size.Y-1);
    packcol16* Ptr = &Image[FromY][X];

    for(int y = FromY; y <= ToY; ++y, Ptr += Size.X)
      *Ptr = Color;
  }
}

void bitmap::DrawHorizontalLine(int OrigFromX, int OrigToX, int OrigY, col16 Color, truth Wide)
{
  static cint PointY[] = { 0, -1, 1 };
  cint Times = Wide ? 3 : 1;

  for(int c = 0; c < Times; ++c)
  {
    int Y = OrigY + PointY[c];
    int FromX = OrigFromX;
    int ToX = OrigToX;

    if(FromX > ToX)
      Swap(FromX, ToX);

    if(Wide && !c)
    {
      --FromX;
      ++ToX;
    }

    if(Y < 0 || Y >= Size.Y || ToX < 0 || FromX >= Size.X)
      continue;

    FromX = Max(FromX, 0);
    ToX = Min(ToX, Size.X-1);
    packcol16* Ptr = &Image[Y][FromX];

    for(int x = FromX; x <= ToX; ++x, ++Ptr)
      *Ptr = Color;
  }
}

void bitmap::DrawPolygon(int CenterX, int CenterY, int Radius, int NumberOfSides,
                         col16 Color, truth DrawSides, truth DrawDiameters, double Rotation)
{
  if(!DrawSides && !DrawDiameters)
    return;

  v2* Point = new v2[NumberOfSides];
  double AngleDelta = 2 * FPI / NumberOfSides;
  int c;

  for(c = 0; c < NumberOfSides; ++c)
  {
    Point[c].X = CenterX + int(sin(AngleDelta * c + Rotation) * Radius);
    Point[c].Y = CenterY + int(cos(AngleDelta * c + Rotation) * Radius);
  }

  if(DrawDiameters)
  {
    if(DrawSides)
    {
      for(c = 0; c < NumberOfSides; ++c)
        for(int a = 0; a < NumberOfSides; ++a)
          if(c != a)
            DrawLine(Point[c].X, Point[c].Y, Point[a].X, Point[a].Y, Color, true);
    }
    else
    {
      for(c = 0; c < NumberOfSides; ++c)
        for(int a = 0; a < NumberOfSides; ++a)
          if((c - a > 1 || a - c > 1) && (a || c != NumberOfSides - 1) && (c || a != NumberOfSides - 1))
            DrawLine(Point[c].X, Point[c].Y, Point[a].X, Point[a].Y, Color, true);
    }
  }
  else
  {
    for(c = 0; c < NumberOfSides - 1; ++c)
      DrawLine(Point[c].X, Point[c].Y, Point[c + 1].X, Point[c + 1].Y, Color, true);

    DrawLine(Point[NumberOfSides - 1].X, Point[NumberOfSides - 1].Y, Point[0].X, Point[0].Y, Color, true);
  }

  delete [] Point;
}

void bitmap::CreateAlphaMap(alpha InitialValue)
{
  if(AlphaMap)
    ABORT("Alpha leak detected!");

  Alloc2D(AlphaMap, Size.Y, Size.X);
  memset(AlphaMap[0], InitialValue, XSizeTimesYSize);
}

truth bitmap::Fade(long& AlphaSum, packalpha& AlphaAverage, int Amount)
{
  if(!AlphaMap)
    ABORT("No alpha map to fade.");

  truth Changes = false;
  long Alphas = 0;
  long NewAlphaSum = 0;
  long Size = XSizeTimesYSize;

  for(long c = 0; c < Size; ++c)
  {
    packalpha* AlphaPtr = &AlphaMap[0][c];

    if(*AlphaPtr)
    {
      if(*AlphaPtr > Amount)
      {
        *AlphaPtr -= Amount;
        NewAlphaSum += *AlphaPtr;
        ++Alphas;
        Changes = true;
      }
      else
      {
        *AlphaPtr = 0;
        Changes = true;

        if(RandMap)
          UpdateRandMap(c, false);
      }
    }
  }

  AlphaSum = NewAlphaSum;
  AlphaAverage = Alphas ? NewAlphaSum / Alphas : 0;
  return Changes;
}

void bitmap::Outline(col16 Color, alpha Alpha, priority Priority)
{
  if(!AlphaMap)
    CreateAlphaMap(255);

  col16 LastColor, NextColor;
  int XMax = Size.X;
  int YMax = Size.Y - 1;

  for(int x = 0; x < XMax; ++x)
  {
    packcol16* Buffer = &Image[0][x];
    LastColor = *Buffer;

    for(int y = 0; y < YMax; ++y)
    {
      NextColor = *(Buffer + XMax);

      if((LastColor == TRANSPARENT_COLOR || !y) && NextColor != TRANSPARENT_COLOR)
      {
        *Buffer = Color;
        SetAlpha(x, y, Alpha);
        SafeSetPriority(x, y, Priority);
      }

      Buffer += XMax;

      if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || y == YMax - 1))
      {
        *Buffer = Color;
        SetAlpha(x, y + 1, Alpha);
        SafeSetPriority(x, y + 1, Priority);
      }

      LastColor = NextColor;
    }
  }

  --XMax;
  ++YMax;

  for(int y = 0; y < YMax; ++y)
  {
    packcol16* Buffer = Image[y];
    LastColor = *Buffer;

    for(int x = 0; x < XMax; ++x)
    {
      NextColor = *(Buffer + 1);

      if((LastColor == TRANSPARENT_COLOR || !x) && NextColor != TRANSPARENT_COLOR)
      {
        *Buffer = Color;
        SetAlpha(x, y, Alpha);
        SafeSetPriority(x, y, Priority);
      }

      ++Buffer;

      if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || x == XMax - 1))
      {
        *Buffer = Color;
        SetAlpha(x + 1, y, Alpha);
        SafeSetPriority(x + 1, y, Priority);
      }

      LastColor = NextColor;
    }
  }
}

void bitmap::FadeToScreen(bitmapeditor BitmapEditor)
{
  bitmap Backup(DOUBLE_BUFFER);
  Backup.ActivateFastFlag();
  blitdata B = { DOUBLE_BUFFER,
                 { 0, 0 },
                 { 0, 0 },
                 { RES.X, RES.Y },
                 { 0 },
                 0,
                 0 };

  for(int c = 0; c <= 5; ++c)
  {
    clock_t StartTime = clock();
    int Element = 127 - c * 25;
    B.Luminance = MakeRGB24(Element, Element, Element);
    Backup.LuminanceMaskedBlit(B);

    if(BitmapEditor)
      BitmapEditor(this, true);

    SimpleAlphaBlit(DOUBLE_BUFFER, c * 50, 0);
    graphics::BlitDBToScreen();
    while(clock() - StartTime < 0.05 * CLOCKS_PER_SEC);
  }

  DOUBLE_BUFFER->ClearToColor(0);

  if(BitmapEditor)
    BitmapEditor(this, true);

  B.Flags = 0;
  NormalMaskedBlit(B);
  graphics::BlitDBToScreen();
}

/**
 * stretch from 2 to 6 only!
 */
void bitmap::StretchBlitXbrz(cblitdata& BlitData)
{
  libxbrzscale::setFreeInputSurfaceAfterScale(false); //TODO this config should be placed more globally...
  SDL_PixelFormat* fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

  blitdata B = BlitData;
  if(B.Stretch<2 || B.Stretch>6){std::cerr<<"invalid stretch value min=2, max=6, requested="<<B.Stretch<<std::endl;exit(1);}; //TODO should exit in a safer way? like saving the game? but this is a bug prevention anyway..

  Uint32 color32bit;
  packcol16 Pixel;
  unsigned char cr,cg,cb,ca;
  bool bFreeImg=false;

  if(img==NULL || img->w!=B.Border.X || img->h!=B.Border.Y || img->refcount==0){
    if(img!=NULL && img->refcount>0)SDL_FreeSurface(img); //previous one
    img = libxbrzscale::createARGBSurface(B.Border.X, B.Border.Y); //src img is always 16x16
  }
  // copy pixels to surface
  for(int x1 = 0; x1 < B.Border.X; x1++)
  {
    for(int y1 = 0; y1 < B.Border.Y; y1++)
    {
      Pixel = Image[B.Src.Y + y1][B.Src.X + x1];
      ca = Pixel == TRANSPARENT_COLOR ? 0 : 0xff; //TODO is 0xff correct for invisible alpha?
      color32bit = SDL_MapRGBA(
        fmt,
        (unsigned char)GetRed16(Pixel),
        (unsigned char)GetGreen16(Pixel),
        (unsigned char)GetBlue16(Pixel),
        ca
      );
      libxbrzscale::SDL_PutPixel(img, x1, y1, color32bit);
    }
  }

//  if(imgStretched==NULL || imgStretched->w!=B.Border.X || imgStretched->h!=B.Border.Y || imgStretched->refcount==0){
//    if(imgStretched!=NULL && imgStretched->refcount>0)SDL_FreeSurface(imgStretched); //previous one
//    imgStretched = libxbrzscale::scale(imgStretched,img,B.Stretch);
//  }
  imgStretched = libxbrzscale::scale(imgStretched,img,B.Stretch);
  // copy from surface the scaled image back to where it is expected TODO comment a more precise info...
  for(int x1 = 0; x1 < imgStretched->w; ++x1)
  {
    for(int y1 = 0; y1 < imgStretched->h; ++y1)
    {
      color32bit = libxbrzscale::SDL_GetPixel(imgStretched,x1,y1);
      SDL_GetRGBA(color32bit,fmt,&cr,&cg,&cb,&ca);
      if(ca!=0){
        B.Bitmap->Image[B.Dest.Y+y1][B.Dest.X+x1] = MakeRGB16(cr,cg,cb);
      }
    }
  }

//  SDL_FreeSurface(img);
//  SDL_FreeSurface(imgStretched);
}

void bitmap::StretchBlit(cblitdata& BlitData) const
{
  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap stretch blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  if(B.Stretch > 1)
  {
    int tx = B.Dest.X;

    for(int x1 = B.Src.X; x1 < B.Src.X + B.Border.X; ++x1, tx += B.Stretch)
    {
      int ty = B.Dest.Y;

      for(int y1 = B.Src.Y; y1 < B.Src.Y + B.Border.Y; ++y1, ty += B.Stretch)
      {
        packcol16 Pixel = Image[y1][x1];

        if(Pixel != TRANSPARENT_COLOR)
          for(int x2 = tx; x2 < tx + B.Stretch; ++x2)
            for(int y2 = ty; y2 < ty + B.Stretch; ++y2)
              B.Bitmap->Image[y2][x2] = Pixel;
      }
    }

    return;
  }
  else if(B.Stretch < -1)
  {
    int tx = B.Dest.X;

    for(int x1 = B.Src.X; x1 < B.Src.X + B.Border.X; x1 -= B.Stretch, ++tx)
    {
      int ty = B.Dest.Y;

      for(int y1 = B.Src.Y; y1 < B.Src.Y + B.Border.Y; y1 -= B.Stretch, ++ty)
      {
        packcol16 Pixel = Image[y1][x1];

        if(Pixel != TRANSPARENT_COLOR)
          B.Bitmap->Image[ty][tx] = Pixel;
      }
    }

    return;
  }
  else
  {
    B.Flags = 0;
    NormalMaskedBlit(B);
    return;
  }
}

outputfile& operator<<(outputfile& SaveFile, cbitmap* Bitmap)
{
  if(Bitmap)
  {
    SaveFile.Put(1);
    SaveFile << Bitmap->GetSize();
    Bitmap->Save(SaveFile);
  }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, bitmap*& Bitmap)
{
  if(SaveFile.Get())
  {
    Bitmap = new bitmap(ReadType<v2>(SaveFile));
    Bitmap->Load(SaveFile);
  }
  else
    Bitmap = 0;

  return SaveFile;
}

void bitmap::DrawRectangle(v2 TopLeft, int Right, int Bottom, col16 Color, truth Wide)
{ DrawRectangle(TopLeft.X, TopLeft.Y, Right, Bottom, Color, Wide); }
void bitmap::DrawRectangle(int Left, int Top, v2 BottomRight, col16 Color, truth Wide)
{ DrawRectangle(Left, Top, BottomRight.X, BottomRight.Y, Color, Wide); }
void bitmap::DrawRectangle(v2 TopLeft, v2 BottomRight, col16 Color, truth Wide)
{ DrawRectangle(TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y, Color, Wide); }

void bitmap::DrawRectangle(int Left, int Top, int Right, int Bottom, col16 Color, truth Wide)
{
  DrawHorizontalLine(Left, Right, Top, Color, Wide);
  DrawHorizontalLine(Left, Right, Bottom, Color, Wide);
  DrawVerticalLine(Right, Top, Bottom, Color, Wide);
  DrawVerticalLine(Left, Top, Bottom, Color, Wide);
}

void bitmap::AlphaLuminanceBlit(cblitdata& BlitData) const
{
  if(BlitData.Luminance == NORMAL_LUMINANCE)
  {
    AlphaMaskedBlit(BlitData);
    return;
  }

  if(!AlphaMap)
  {
    LuminanceMaskedBlit(BlitData);
    return;
  }

  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap alpha blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packalpha** SrcAlphaMap = AlphaMap;
  packcol16** DestImage = B.Bitmap->Image;
  int NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackalpha* AlphaPtr = &SrcAlphaMap[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr)
    {
      LOAD_SRC();

      if(SrcCol != B.MaskColor)
      {
        LOAD_DEST();
        LOAD_ALPHA();
        NEW_LUMINATE_RED();
        NEW_APPLY_ALPHA_RED();
        NEW_LUMINATE_GREEN();
        NEW_APPLY_ALPHA_GREEN();
        NEW_LUMINATE_BLUE();
        NEW_APPLY_ALPHA_BLUE();
        STORE_COLOR();
      }
    }
  }
}

/* Only works for 16x16 pictures :( */

void bitmap::CreateFlames(rawbitmap* RawBitmap, v2 RawPos, ulong SeedNFlags, int Frame)
{
  femath::SaveSeed();
  femath::SetSeed(SeedNFlags);
  int FlameTop[16], FlameBottom[16], FlamePhase[16];
  int x, y;

  for(x = 0; x < 16; ++x)
  {
    FlameBottom[x] = NO_FLAME;

    for(y = 0; y < 16; ++y)
      if(GetPixel(x, y) != TRANSPARENT_COLOR)
      {
        if(1 << RawBitmap->GetMaterialColorIndex(RawPos.X + x, RawPos.Y + y) & SeedNFlags)
        {
          FlamePhase[x] = RAND_16;

          if(y > 1)
          {
            FlameBottom[x] = y - 1;

            if(y >= 5)
              FlameTop[x] = (y - (RAND_32 * y >> 5)) >> 1;
            else
              FlameTop[x] = 0;
          }
          else
          {
            FlameBottom[x] = 1;
            FlameTop[x] = 0;
          }
        }

        break;
      }
  }

  for(x = 0; x < 16; ++x)
  {
    if(FlameBottom[x] != NO_FLAME)
    {
      int Phase = (Frame + FlamePhase[x]) & 15;
      int Length = FlameBottom[x] - FlameTop[x];
      int Top = FlameBottom[x] - Length + Phase * (15 - Phase) * Length / 56;

      for(y = Top; y <= FlameBottom[x]; ++y)
      {
        int Pos = y - Top;
        PowerPutPixel(x, y, MakeRGB16(255, 255 - (Pos << 7) / Length, 0), 127 + (Pos << 6) / Length, AVERAGE_PRIORITY);
      }
    }
  }

  femath::LoadSeed();
}

void bitmap::CreateSparkle(v2 SparklePos, int Frame)
{
  if(Frame)
  {
    int Size = (Frame - 1) * (16 - Frame) / 10;
    PowerPutPixel(SparklePos.X, SparklePos.Y, WHITE, 255, SPARKLE_PRIORITY);

    for(int c = 1; c < Size; ++c)
    {
      int Lightness = 191 + ((Size - c) << 6) / Size;
      col16 RGB = MakeRGB16(Lightness, Lightness, Lightness);
      PowerPutPixel(SparklePos.X + c, SparklePos.Y, RGB, 255, SPARKLE_PRIORITY);
      PowerPutPixel(SparklePos.X - c, SparklePos.Y, RGB, 255, SPARKLE_PRIORITY);
      PowerPutPixel(SparklePos.X, SparklePos.Y + c, RGB, 255, SPARKLE_PRIORITY);
      PowerPutPixel(SparklePos.X, SparklePos.Y - c, RGB, 255, SPARKLE_PRIORITY);
    }
  }
}

void bitmap::CreateFlies(ulong Seed, int Frame, int FlyAmount)
{
  femath::SaveSeed();
  femath::SetSeed(Seed);

  for(int c = 0; c < FlyAmount; ++c)
  {
    double Constant = double(RAND() % 10000) / 10000 * FPI;
    v2 StartPos = v2(5 + RAND() % 6, 5 + RAND() % 6);
    double Temp = (double(16 - Frame) * FPI) / 16;

    if(RAND() & 1)
      Temp = -Temp;

    v2 Where;
    Where.X = int(StartPos.X + sin(Constant + Temp) * 3);
    Where.Y = int(StartPos.Y + sin(2 * (Constant + Temp)) * 3);
    PowerPutPixel(Where.X, Where.Y, MakeRGB16(40, 40, 60), 255, FLY_PRIORITY);
  }

  femath::LoadSeed();
}

void bitmap::CreateLightning(ulong Seed, col16 Color)
{
  femath::SaveSeed();
  femath::SetSeed(Seed);
  v2 StartPos;
  v2 Direction(0, 0);

  do
  {
    do
    {
      if(RAND() & 1)
      {
        if(RAND() & 1)
        {
          StartPos.X = 0;
          Direction.X = 1;
        }
        else
        {
          StartPos.X = Size.X - 1;
          Direction.X = -1;
        }

        StartPos.Y = RAND() % Size.Y;
      }
      else
      {
        if(RAND() & 1)
        {
          StartPos.Y = 0;
          Direction.Y = 1;
        }
        else
        {
          StartPos.Y = Size.Y - 1;
          Direction.Y = -1;
        }

        StartPos.X = RAND() % Size.X;
      }
    }
    while(GetPixel(StartPos) != TRANSPARENT_COLOR);
  }
  while(!CreateLightning(StartPos, Direction, NO_LIMIT, Color));

  femath::LoadSeed();
}

struct pixelvectorcontroller
{
  static truth Handler(int x, int y)
  {
    if(CurrentSprite->GetPixel(x, y) == TRANSPARENT_COLOR)
    {
      PixelVector.push_back(v2(x, y));
      return true;
    }
    else
      return false;
  }
  static std::vector<v2> PixelVector;
  static bitmap* CurrentSprite;
};

std::vector<v2> pixelvectorcontroller::PixelVector;
bitmap* pixelvectorcontroller::CurrentSprite;

truth bitmap::CreateLightning(v2 StartPos, v2 Direction, int MaxLength, col16 Color)
{
  pixelvectorcontroller::CurrentSprite = this;
  std::vector<v2>& PixelVector = pixelvectorcontroller::PixelVector;
  PixelVector.clear();
  v2 LastMove(0, 0);
  int Counter = 0;

  for(;;)
  {
    v2 Move(1 + (RAND() & 3), 1 + (RAND() & 3));

    if(Direction.X < 0 || (!Direction.X && RAND() & 1))
      Move.X = -Move.X;

    if(Direction.Y < 0 || (!Direction.Y && RAND() & 1))
      Move.Y = -Move.Y;

    LimitRef(Move.X, -StartPos.X, Size.X - StartPos.X - 1);
    LimitRef(Move.Y, -StartPos.Y, Size.X - StartPos.Y - 1);

    if(Counter < 10 && ((!Move.Y && !LastMove.Y)
                        || (Move.Y && LastMove.Y && (Move.X << 10) / Move.Y == (LastMove.X << 10) / LastMove.Y)))
    {
      ++Counter;
      continue;
    }

    Counter = 0;

    if(!mapmath<pixelvectorcontroller>::DoLine(StartPos.X, StartPos.Y, StartPos.X + Move.X, StartPos.Y + Move.Y)
       || ulong(MaxLength) <= PixelVector.size())
    {
      int Limit = Min<int>(PixelVector.size(), MaxLength);

      for(int c = 0; c < Limit; ++c)
      {
        PutPixel(PixelVector[c], Color);
        SafeSetPriority(PixelVector[c], LIGHTNING_PRIORITY);
      }

      PixelVector.clear();
      return true;
    }

    StartPos += Move;
    LastMove = Move;

    if((Direction.X && (!StartPos.X || StartPos.X == Size.X - 1))
       || (Direction.Y && (!StartPos.Y || StartPos.Y == Size.X - 1)))
    {
      PixelVector.clear();
      return false;
    }
  }
}

void bitmap::BlitAndCopyAlpha(bitmap* Bitmap, int Flags) const
{
  if(!FastFlag)
  {
    if(!AlphaMap || !Bitmap->AlphaMap)
      ABORT("Attempt to blit and copy alpha without an alpha map detected!");

    if(Flags & ROTATE && Size.X != Size.Y)
      ABORT("Blit and copy alpha error: FeLib supports only square rotating!");

    if(Size.X != Bitmap->Size.X || Size.Y != Bitmap->Size.Y)
      ABORT("Blit and copy alpha attempt of noncongruent bitmaps detected!");
  }

  packcol16** SrcImage = Image;
  packalpha** SrcAlphaMap = AlphaMap;
  packcol16** DestImage = Bitmap->Image;
  packalpha** DestAlphaMap = Bitmap->AlphaMap;

  switch(Flags & 7)
  {
   case NONE:
    {
      memcpy(DestImage[0], SrcImage[0], XSizeTimesYSize * sizeof(packcol16));
      memcpy(DestAlphaMap[0], SrcAlphaMap[0], XSizeTimesYSize * sizeof(packalpha));
      break;
    }

   case MIRROR:
    {
      int Width = Size.X;
      int Height = Size.Y;
      int DestX = Width - 1;
      cpackcol16* SrcPtr = SrcImage[0];
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];

      for(int y = 0; y < Height; ++y)
      {
        cpackcol16* EndPtr = SrcPtr + Width;
        packcol16* DestPtr = &DestImage[y][DestX];
        packalpha* DestAlphaPtr = &DestAlphaMap[y][DestX];

        for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr, ++SrcAlphaPtr, --DestAlphaPtr)
        {
          *DestPtr = *SrcPtr;
          *DestAlphaPtr = *SrcAlphaPtr;
        }
      }

      break;
    }

   case FLIP:
    {
      int Height = Size.Y;
      int Width = Size.X;
      int DestY = Height - 1;

      for(int y = 0; y < Height; ++y)
      {
        memcpy(DestImage[DestY - y], SrcImage[y], Width * sizeof(packcol16));
        memcpy(DestAlphaMap[DestY - y], SrcAlphaMap[y], Width * sizeof(packalpha));
      }

      break;
    }

   case (MIRROR | FLIP):
    {
      cpackcol16* SrcPtr = SrcImage[0];
      cpackcol16* EndPtr = SrcPtr + XSizeTimesYSize;
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];
      packcol16* DestPtr = &DestImage[Size.Y - 1][Size.X - 1];
      packalpha* DestAlphaPtr = &DestAlphaMap[Size.Y - 1][Size.X - 1];

      for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr, ++SrcAlphaPtr, --DestAlphaPtr)
      {
        *DestPtr = *SrcPtr;
        *DestAlphaPtr = *SrcAlphaPtr;
      }

      break;
    }

   case ROTATE:
    {
      cint Width = Size.X;
      cpackcol16* SrcPtr = SrcImage[0];
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];
      packcol16* DestBase = &DestImage[0][Width - 1];
      packalpha* DestAlphaBase = &DestAlphaMap[0][Width - 1];

      for(int y = 0; y < Width; ++y)
      {
        cpackcol16* EndPtr = SrcPtr + Width;
        packcol16* DestPtr = DestBase - y;
        packalpha* DestAlphaPtr = DestAlphaBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += Width, ++SrcAlphaPtr, DestAlphaPtr += Width)
        {
          *DestPtr = *SrcPtr;
          *DestAlphaPtr = *SrcAlphaPtr;
        }
      }

      break;
    }

   case (MIRROR | ROTATE):
    {
      cint Width = Size.X;
      cpackcol16* SrcPtr = SrcImage[0];
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];
      packcol16* DestBase = DestImage[0];
      packalpha* DestAlphaBase = DestAlphaMap[0];

      for(int y = 0; y < Width; ++y)
      {
        cpackcol16* EndPtr = SrcPtr + Width;
        packcol16* DestPtr = DestBase + y;
        packalpha* DestAlphaPtr = DestAlphaBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += Width, ++SrcAlphaPtr, DestAlphaPtr += Width)
        {
          *DestPtr = *SrcPtr;
          *DestAlphaPtr = *SrcAlphaPtr;
        }
      }

      break;
    }

   case (FLIP | ROTATE):
    {
      cint Width = Size.X;
      cpackcol16* SrcPtr = SrcImage[0];
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];
      packcol16* DestBase = &DestImage[Width - 1][Width - 1];
      packalpha* DestAlphaBase = &DestAlphaMap[Width - 1][Width - 1];

      for(int y = 0; y < Width; ++y)
      {
        cpackcol16* EndPtr = SrcPtr + Width;
        packcol16* DestPtr = DestBase - y;
        packalpha* DestAlphaPtr = DestAlphaBase - y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= Width, ++SrcAlphaPtr, DestAlphaPtr -= Width)
        {
          *DestPtr = *SrcPtr;
          *DestAlphaPtr = *SrcAlphaPtr;
        }
      }

      break;
    }

   case (MIRROR | FLIP | ROTATE):
    {
      cint Width = Size.X;
      cpackcol16* SrcPtr = SrcImage[0];
      cpackalpha* SrcAlphaPtr = SrcAlphaMap[0];
      packcol16* DestBase = DestImage[Width - 1];
      packalpha* DestAlphaBase = DestAlphaMap[Width - 1];

      for(int y = 0; y < Width; ++y)
      {
        cpackcol16* EndPtr = SrcPtr + Width;
        packcol16* DestPtr = DestBase + y;
        packalpha* DestAlphaPtr = DestAlphaBase + y;

        for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= Width, ++SrcAlphaPtr, DestAlphaPtr -= Width)
        {
          *DestPtr = *SrcPtr;
          *DestAlphaPtr = *SrcAlphaPtr;
        }
      }

      break;
    }
  }
}

void bitmap::FillAlpha(alpha Alpha)
{
  memset(AlphaMap[0], Alpha, XSizeTimesYSize);
}

void bitmap::PowerPutPixel(int X, int Y, col16 Color, alpha Alpha, priority Priority)
{
  if(X >= 0 && Y >= 0 && X < Size.X && Y < Size.Y)
  {
    Image[Y][X] = Color;

    if(AlphaMap)
      AlphaMap[Y][X] = Alpha;
    else if(Alpha != 255)
    {
      CreateAlphaMap(255);
      AlphaMap[Y][X] = Alpha;
    }

    if(PriorityMap)
      PriorityMap[Y][X] = Priority;
  }
}

void bitmap::MaskedPriorityBlit(cblitdata& BlitData) const
{
  if(!PriorityMap || !BlitData.Bitmap->PriorityMap)
  {
    LuminanceMaskedBlit(BlitData);
    return;
  }

  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap masked priority blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y, Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packpriority** SrcPriorityMap = PriorityMap;
  packcol16** DestImage = B.Bitmap->Image;
  packpriority** DestPriorityMap = B.Bitmap->PriorityMap;
  int NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackpriority* SrcPriorityPtr = &SrcPriorityMap[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];
    packpriority* DestPriorityPtr = &DestPriorityMap[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++SrcPriorityPtr, ++DestPriorityPtr)
    {
      LOAD_SRC();

      if(SrcCol != B.MaskColor)
      {
        priority SrcPriority = *SrcPriorityPtr;
        priority DestPriority = *DestPriorityPtr;

        if((SrcPriority & 0xF) >= (DestPriority & 0xF) || (SrcPriority & 0xF0) >= (DestPriority & 0xF0))
        {
          NEW_LUMINATE_RED();
          NEW_LUMINATE_GREEN();
          NEW_LUMINATE_BLUE();
          STORE_COLOR();
          *DestPriorityPtr = SrcPriority;
        }
      }
    }
  }
}

void bitmap::AlphaPriorityBlit(cblitdata& BlitData) const
{
  if(!AlphaMap)
  {
    MaskedPriorityBlit(BlitData);
    return;
  }

  if(!PriorityMap || !BlitData.Bitmap->PriorityMap)
  {
    AlphaLuminanceBlit(BlitData);
    return;
  }

  blitdata B = BlitData;

  if(!FastFlag)
  {
    if(!B.Border.X || !B.Border.Y)
      ABORT("Zero-sized bitmap alpha priority blit attempt detected!");

    if(!femath::Clip(B.Src.X, B.Src.Y, B.Dest.X, B.Dest.Y, B.Border.X, B.Border.Y,
                     Size.X, Size.Y, B.Bitmap->Size.X, B.Bitmap->Size.Y))
      return;
  }

  packcol16** SrcImage = Image;
  packalpha** SrcAlphaMap = AlphaMap;
  packpriority** SrcPriorityMap = PriorityMap;
  packcol16** DestImage = B.Bitmap->Image;
  packpriority** DestPriorityMap = B.Bitmap->PriorityMap;
  int NewRedLuminance = (B.Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (B.Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (B.Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < B.Border.Y; ++y)
  {
    cpackcol16* SrcPtr = &SrcImage[B.Src.Y + y][B.Src.X];
    cpackalpha* AlphaPtr = &SrcAlphaMap[B.Src.Y + y][B.Src.X];
    cpackpriority* SrcPriorityPtr = &SrcPriorityMap[B.Src.Y + y][B.Src.X];
    cpackcol16* EndPtr = SrcPtr + B.Border.X;
    packcol16* DestPtr = &DestImage[B.Dest.Y + y][B.Dest.X];
    packpriority* DestPriorityPtr = &DestPriorityMap[B.Dest.Y + y][B.Dest.X];

    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr, ++SrcPriorityPtr, ++DestPriorityPtr)
    {
      LOAD_SRC();

      if(SrcCol != B.MaskColor)
      {
        priority SrcPriority = *SrcPriorityPtr;
        priority DestPriority = *DestPriorityPtr;

        if((SrcPriority & 0xF) >= (DestPriority & 0xF)
           || (SrcPriority & 0xF0) >= (DestPriority & 0xF0))
        {
          LOAD_DEST();
          LOAD_ALPHA();
          NEW_LUMINATE_RED();
          NEW_APPLY_ALPHA_RED();
          NEW_LUMINATE_GREEN();
          NEW_APPLY_ALPHA_GREEN();
          NEW_LUMINATE_BLUE();
          NEW_APPLY_ALPHA_BLUE();
          STORE_COLOR();
          *DestPriorityPtr = SrcPriority;
        }
      }
    }
  }
}

void bitmap::InitPriorityMap(priority InitialValue)
{
  if(!PriorityMap)
    Alloc2D(PriorityMap, Size.Y, Size.X);

  memset(PriorityMap[0], InitialValue, XSizeTimesYSize);
}

void bitmap::FillPriority(priority Priority)
{
  memset(PriorityMap[0], Priority, XSizeTimesYSize);
}

void bitmap::FastBlitAndCopyAlpha(bitmap* Bitmap) const
{
  if(!FastFlag)
  {
    if(!AlphaMap || !Bitmap->AlphaMap)
      ABORT("Attempt to fast blit and copy alpha without an alpha map detected!");

    if(Size.X != Bitmap->Size.X || Size.Y != Bitmap->Size.Y)
      ABORT("Fast blit and copy alpha attempt of noncongruent bitmaps detected!");
  }

  memcpy(Bitmap->Image[0], Image[0], XSizeTimesYSize * sizeof(packcol16));
  memcpy(Bitmap->AlphaMap[0], AlphaMap[0], XSizeTimesYSize * sizeof(packalpha));
}

void bitmap::UpdateRandMap(long Index, truth Value)
{
  long c1 = XSizeTimesYSize + Index;
  RandMap[c1] = Value;

  for(long c2 = c1 >> 1; c2; c1 = c2, c2 >>= 1)
  {
    Value |= RandMap[c1 ^ 1];

    if(!RandMap[c2] != !Value)
      RandMap[c2] = Value;
    else
      return;
  }
}

void bitmap::InitRandMap()
{
  if(!RandMap)
    RandMap = new truth[XSizeTimesYSize << 1];

  memset(RandMap, 0, (XSizeTimesYSize << 1) * sizeof(truth));
}

v2 bitmap::RandomizePixel() const
{
  if(!RandMap[1])
    return ERROR_V2;

  long Rand = RAND();
  ulong c, RandMask = 1;
  ulong MapSize = XSizeTimesYSize << 1;

  for(c = 2; c < MapSize; c <<= 1)
    if(RandMap[c + 1] && (!RandMap[c] || Rand & (RandMask <<= 1)))
      ++c;

  c = (c - MapSize) >> 1;
  return v2(c % Size.X, c / Size.X);
}

void bitmap::CalculateRandMap()
{
  if(!AlphaMap)
    ABORT("Alpha map needed to calculate random map.");

  ulong Size = XSizeTimesYSize;

  for(ulong c = 0; c < Size; ++c)
    UpdateRandMap(c, AlphaMap[0][c]);
}

void bitmap::AlphaPutPixel(int x, int y, col16 SrcCol, col24 Luminance, alpha Alpha)
{
  int DestCol = Image[y][x];
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;
  NEW_LUMINATE_RED();
  NEW_APPLY_ALPHA_RED();
  NEW_LUMINATE_GREEN();
  NEW_APPLY_ALPHA_GREEN();
  NEW_LUMINATE_BLUE();
  NEW_APPLY_ALPHA_BLUE();
  Image[y][x] = Red|Green|Blue;

}

alpha bitmap::CalculateAlphaAverage() const
{
  if(!AlphaMap)
    ABORT("Alpha map needed to calculate alpha average!");

  long Alphas = 0;
  long AlphaSum = 0;
  ulong Size = XSizeTimesYSize;

  for(ulong c = 0; c < Size; ++c)
  {
    packalpha* AlphaPtr = &AlphaMap[0][c];

    if(*AlphaPtr)
    {
      AlphaSum += *AlphaPtr;
      ++Alphas;
    }
  }

  return Alphas ? AlphaSum / Alphas : 0;
}

cachedfont::cachedfont(v2 Size) : bitmap(Size)
{
  Alloc2D(MaskMap, Size.Y, Size.X);
}

cachedfont::cachedfont(v2 Size, col16 Color) : bitmap(Size, Color)
{
  Alloc2D(MaskMap, Size.Y, Size.X);
}

void cachedfont::PrintCharacter(cblitdata B) const
{
  if(B.Dest.X < 0 || B.Dest.Y < 0 || B.Dest.X + 10 >= B.Bitmap->Size.X || B.Dest.Y + 9 >= B.Bitmap->Size.Y)
  {
    NormalMaskedBlit(B);
    return;
  }

  packcol16** SrcLine = &Image[B.Src.Y];
  packcol16** EndLine = SrcLine + 9;
  packcol16** SrcMaskLine = &MaskMap[B.Src.Y];
  packcol16** DestLine = &B.Bitmap->Image[B.Dest.Y];

  for(; SrcLine != EndLine; ++SrcLine, ++SrcMaskLine, ++DestLine)
  {
    culong* FontPtr = reinterpret_cast<culong*>(*SrcLine + B.Src.X);
        // I don't know how correct this is, but longs are 64 bit on 64 bit.
    culong* EndPtr = FontPtr + (20 / sizeof(ulong));
    culong* MaskPtr = reinterpret_cast<culong*>(*SrcMaskLine + B.Src.X);
    ulong* DestPtr = reinterpret_cast<ulong*>(*DestLine + B.Dest.X);

    for(; FontPtr != EndPtr; ++DestPtr, ++MaskPtr, ++FontPtr)
      *DestPtr = (*DestPtr & *MaskPtr) | *FontPtr;
  }
}

void cachedfont::CreateMaskMap()
{
  packcol16* SrcPtr = Image[0];
  packcol16* EndPtr = SrcPtr + XSizeTimesYSize;
  packcol16* MaskPtr = MaskMap[0];

  for(; SrcPtr != EndPtr; ++SrcPtr, ++MaskPtr)
    if(*SrcPtr == TRANSPARENT_COLOR)
    {
      *SrcPtr = 0;
      *MaskPtr = 0xFFFF;
    }
    else
      *MaskPtr = 0;
}

cint WaveDelta[] = { 1, 2, 2, 2, 1, 0, -1, -2, -2, -2, -1 };

void bitmap::Wobble(int Frame, int SpeedShift, truth Horizontally)
{
  int WavePos = (Frame << SpeedShift >> 1) - 14;

  if(Horizontally)
  {
    for(int c = 0; c < 11; ++c)
      if(WavePos + c >= 0 && WavePos + c < Size.Y)
        MoveLineHorizontally(WavePos + c, WaveDelta[c]);
  }
  else
  {
    for(int c = 0; c < 11; ++c)
      if(WavePos + c >= 0 && WavePos + c < Size.X)
        MoveLineVertically(WavePos + c, WaveDelta[c]);
  }
}

void bitmap::MoveLineVertically(int X, int Delta)
{
  int y;

  if(Delta < 0)
  {
    for(y = 0; y < Size.Y + Delta; ++y)
      PowerPutPixel(X, y, GetPixel(X, y - Delta), AlphaMap ? GetAlpha(X, y - Delta) : 255, AVERAGE_PRIORITY);

    for(int y = -1; y >= Delta; --y)
      PowerPutPixel(X, Size.Y + y, TRANSPARENT_COLOR, 255, AVERAGE_PRIORITY);
  }
  else if(Delta > 0)
  {
    for(y = Size.Y - 1; y >= Delta; --y)
      PowerPutPixel(X, y, GetPixel(X, y - Delta), AlphaMap ? GetAlpha(X, y - Delta) : 255, AVERAGE_PRIORITY);

    for(y = 0; y < Delta; ++y)
      PowerPutPixel(X, y, TRANSPARENT_COLOR, 255, AVERAGE_PRIORITY);
  }
}

void bitmap::MoveLineHorizontally(int Y, int Delta)
{
  int x;

  if(Delta < 0)
  {
    for(x = 0; x < Size.X + Delta; ++x)
      PowerPutPixel(x, Y, GetPixel(x - Delta, Y), AlphaMap ? GetAlpha(x - Delta, Y) : 255, AVERAGE_PRIORITY);

    for(x = -1; x >= Delta; --x)
      PowerPutPixel(Size.X + x, Y, TRANSPARENT_COLOR, 255, AVERAGE_PRIORITY);
  }
  else if(Delta > 0)
  {
    for(x = Size.X - 1; x >= Delta; --x)
      PowerPutPixel(x, Y, GetPixel(x - Delta, Y), AlphaMap ? GetAlpha(x - Delta, Y) : 255, AVERAGE_PRIORITY);

    for(x = 0; x < Delta; ++x)
      PowerPutPixel(x, Y, TRANSPARENT_COLOR, 255, AVERAGE_PRIORITY);
  }
}

void bitmap::InterLace()
{
  for(int y = 0; y < Size.Y; ++y)
    if(!(y % 3))
      for(int x = 0; x < Size.X; ++x)
        if(Image[y][x] != 0)
          Image[y][x] = 1;
}
