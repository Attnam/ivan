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

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <vector>

#ifdef USE_SDL
#include "SDL.h"
#endif

#include "v2.h"

#define DOUBLE_BUFFER graphics::GetDoubleBuffer()
#define RES graphics::GetRes()
#define FONT graphics::GetDefaultFont()

class bitmap;
class rawbitmap;
class festring;

class graphics
{
 public:
  friend class bitmap;
  static void Init();
  static void DeInit();

#ifdef USE_SDL
  static void SetScale(int);
  static void SwitchMode();
#endif

#ifdef __DJGPP__
  static void SwitchMode() { }
#endif

  static void SetMode(cchar*, cchar*, v2, int, truth);
  static void Stretch(bool, bitmap*, blitdata&);
  static void DrawRectangleOutlineAround(bitmap* bmpAt, v2 v2TopLeft, v2 v2Border, col16 color, bool wide);
  static void BlitDBToScreen();

  static v2 GetRes() { return Res; }
  static bitmap* GetDoubleBuffer() { return DoubleBuffer; }
  static void LoadDefaultFont(cfestring&);
  static rawbitmap* GetDefaultFont() { return DefaultFont; }
  static void SetSwitchModeHandler(void (*What)()){ SwitchModeHandler = What; }

  static int AddStretchRegion(blitdata B,const char* strId);
  static void SetSpecialListItemAltPos(bool b){bSpecialListItemAltPos=b;}
  static void SetAllowStretchedBlit(){bAllowStretchedRegionsBlit=true;} //as the dungeon shows most of the time,
  static void SetDenyStretchedBlit(){bAllowStretchedRegionsBlit=false;} //it should be denied only during a few moments.
  static void PrepareBeforeDrawingFelist();
  static void DrawBeforeFelistPage();
  static bitmap* PrepareBuffer();

  //TODO utility class for sregion?
  static bool IsSRegionEnabled(int iIndex);
  static void SetSRegionEnabled(int iIndex, bool b);
  static void SetSRegionUseXBRZ(int iIndex, bool b);
  static void SetSRegionDrawAfterFelist(int iIndex, bool b);
  static void SetSRegionDrawBeforeFelistPage(int iIndex, bool, bool);
  static void SetSRegionDrawRectangleOutline(int iIndex, bool b);
  static void SetSRegionListItem(int iIndex);
  static int  SetSRegionBlitdata(int iIndex, blitdata B);
  static void ClearDoubleBufferSquareAt(v2 pos, v2 size);

#ifdef USE_SDL
  #if SDL_MAJOR_VERSION == 1
    static SDL_Surface* Screen;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      static SDL_Surface* TempSurface;
    #endif
  #else
    public:
      static SDL_Window* GetWindow(){return Window;};
    private:
      static SDL_Window* Window;
      static SDL_Renderer *Renderer;
      static SDL_Texture *Texture;
  #endif
#endif

 private:
  static void (*SwitchModeHandler)();
#ifdef __DJGPP__
  static ulong BufferSize;
  static ushort ScreenSelector;
  static struct vesainfo
  {
    void Retrieve();
    ulong Signature NO_ALIGNMENT;
    ushort Version NO_ALIGNMENT;
    ulong OEMString NO_ALIGNMENT;
    ulong Capabilities NO_ALIGNMENT;
    ulong ModeList NO_ALIGNMENT;
    ushort Memory NO_ALIGNMENT;
    uchar Shit[493] NO_ALIGNMENT;
  } VesaInfo;
  static struct modeinfo
  {
    void Retrieve(ushort);
    ushort Attribs1 NO_ALIGNMENT;
    uchar AWindowAttribs NO_ALIGNMENT;
    uchar BWindowAttribs NO_ALIGNMENT;
    ushort Granularity NO_ALIGNMENT;
    ushort WindowSize NO_ALIGNMENT;
    ushort WindowASegment NO_ALIGNMENT;
    ushort WindowBSegment NO_ALIGNMENT;
    ulong WindowMoveFunction NO_ALIGNMENT;
    ushort BytesPerLine NO_ALIGNMENT;
    ushort Width NO_ALIGNMENT;
    ushort Height NO_ALIGNMENT;
    uchar CharWidth NO_ALIGNMENT;
    uchar CharHeight NO_ALIGNMENT;
    uchar Planes NO_ALIGNMENT;
    uchar BitsPerPixel NO_ALIGNMENT;
    uchar Banks NO_ALIGNMENT;
    uchar MemoryModel NO_ALIGNMENT;
    uchar BankSize NO_ALIGNMENT;
    uchar ImagePages NO_ALIGNMENT;
    uchar Reserved1 NO_ALIGNMENT;
    uchar RedBits NO_ALIGNMENT;
    uchar RedShift NO_ALIGNMENT;
    uchar GreenBits NO_ALIGNMENT;
    uchar GreenShift NO_ALIGNMENT;
    uchar BlueBits NO_ALIGNMENT;
    uchar BlueShift NO_ALIGNMENT;
    uchar ResBits NO_ALIGNMENT;
    uchar ResShift NO_ALIGNMENT;
    uchar Attribs2 NO_ALIGNMENT;
    ulong PhysicalLFBAddress NO_ALIGNMENT;
    ulong OffScreenMem NO_ALIGNMENT;
    ushort OffScreenMemSize NO_ALIGNMENT;
    uchar Reserved2[206] NO_ALIGNMENT;
  } ModeInfo;
#endif
  static bitmap* DoubleBuffer;
  static bitmap* StretchedDB;
  static truth bAllowStretchedRegionsBlit;
  static truth bSpecialListItemAltPos;
  static v2 Res;
  static int Scale;
  static int ColorDepth;
  static rawbitmap* DefaultFont;
};

#endif
