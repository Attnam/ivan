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

#ifdef USE_SDL
#include "SDL.h"
#endif

#ifdef __DJGPP__
#include <dpmi.h>
#include <conio.h>
#include <go32.h>
#endif

#include <cassert>
#include <iostream>
#include <sstream>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "error.h"
#include "rawbit.h"
#include "felist.h"
#include "feio.h"

void (*graphics::SwitchModeHandler)();

#ifdef USE_SDL
#if SDL_MAJOR_VERSION == 1
SDL_Surface* graphics::Screen;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
SDL_Surface* graphics::TempSurface;
#endif
#else
SDL_Window* graphics::Window;
SDL_Renderer* graphics::Renderer;
SDL_Texture* graphics::Texture;
#endif
#endif

#ifdef __DJGPP__
ulong graphics::BufferSize;
ushort graphics::ScreenSelector = 0;
graphics::vesainfo graphics::VesaInfo;
graphics::modeinfo graphics::ModeInfo;
#endif

struct stretchRegion //TODO all these booleans could be a single uint32? unnececessarily complicated?
{
  int iIndex;
  const char* strId;
  bool bEnabled;
  blitdata B;
  bool bForceXBRZ;
  bool bShowWithFelist;
  bool bSpecialListItem;
  bool bSpecialListItemAltPos;
};
const stretchRegion SRdefault = {-1,NULL,true,DEFAULT_BLITDATA,false,false,false,false};

std::vector<stretchRegion> vStretchRegion;
bitmap* graphics::DoubleBuffer=NULL;
bitmap* graphics::StretchedDB=NULL;
truth graphics::bUseXbrzScale=false;
truth graphics::bAllowStretchedBlit=false;
v2 graphics::Res;
int graphics::Scale;
int graphics::ColorDepth;
rawbitmap* graphics::DefaultFont = 0;

#ifdef DBGMSG
  #include "dbgmsg.h"

//#define DBGSRI(info) dbgSR(__FILENAME__,__LINE__,SR,info)
  #define DBGSRI(info) dbgSR(SR,info)
  #define DBGSR DBGSRI("")

  //void dbgSR(const char* fname, int iline, stretchRegion SR,const char* strInfo){
  void dbgSR(stretchRegion SR,const char* strInfo){
    blitdata Bto=SR.B;
  //  std::cerr<<fname<<":"<<iline<<":"<<strInfo
    stringstream ss;ss<<strInfo
      <<"["<<SR.iIndex<<"]SR@"
      <<"Src="<<Bto.Src.X<<","<<Bto.Src.Y<<"/"
      <<"Dest="<<Bto.Dest.X<<","<<Bto.Dest.Y<<"/"
      <<"Stretch="<<Bto.Stretch<<"/"
      <<"bForceXBRZ="<<SR.bForceXBRZ<<"/"
      <<"id="<<SR.strId<<"/"
      <<std::endl;
  }
#else
  #include "rmdbgmsg.h"

  #define DBGSR
  #define DBGSRI
#endif

void graphics::Init()
{
  static truth AlreadyInstalled = false;

  if(!AlreadyInstalled)
  {
    AlreadyInstalled = true;

#ifdef USE_SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE))
      ABORT("Can't initialize SDL.");
#if SDL_MAJOR_VERSION == 2
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
#endif
#endif

#ifdef __DJGPP__
    VesaInfo.Retrieve();
#endif

    atexit(graphics::DeInit);
  }
}

void graphics::DeInit()
{
  delete DefaultFont;
  DefaultFont = 0;

#ifdef USE_SDL
#if SDL_MAJOR_VERSION == 1
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  SDL_FreeSurface(TempSurface);
#endif
#else
  if(Texture)
    SDL_DestroyTexture(Texture);

  if(Renderer)
    SDL_DestroyRenderer(Renderer);

  if(Window)
    SDL_DestroyWindow(Window);
#endif
  SDL_Quit();
#endif

#ifdef __DJGPP__
  if(ScreenSelector)
  {
    __dpmi_free_ldt_descriptor(ScreenSelector);
    ScreenSelector = 0;
    textmode(0x3);
  }
#endif
}

#ifdef USE_SDL

void graphics::SetMode(cchar* Title, cchar* IconName,
                       v2 NewRes, int NewScale, truth FullScreen)
{
#if SDL_MAJOR_VERSION == 1
  if(IconName)
  {
    SDL_Surface* Icon = SDL_LoadBMP(IconName);
    SDL_SetColorKey(Icon, SDL_SRCCOLORKEY,
                    SDL_MapRGB(Icon->format, 255, 255, 255));
    SDL_WM_SetIcon(Icon, NULL);
  }
#endif

  ulong Flags = SDL_SWSURFACE;

  if(FullScreen)
  {
    SDL_ShowCursor(SDL_DISABLE);
#if SDL_MAJOR_VERSION == 1
    Flags |= SDL_FULLSCREEN;
#else
    Flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
  }

#if SDL_MAJOR_VERSION == 1
  Screen = SDL_SetVideoMode(NewRes.X, NewRes.Y, 16, Flags);
  if(!Screen)
    ABORT("Couldn't set video mode.");

  SDL_WM_SetCaption(Title, 0);
#else
  Flags |= SDL_WINDOW_ALLOW_HIGHDPI;

  Window = SDL_CreateWindow(Title,
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            NewRes.X, NewRes.Y, Flags);

  if(!Window)
    ABORT("Couldn't set video mode.");

  if(IconName)
  {
    SDL_Surface* Icon = SDL_LoadBMP(IconName);
    SDL_SetColorKey(Icon, SDL_TRUE,
                    SDL_MapRGB(Icon->format, 255, 255, 255));
    SDL_SetWindowIcon(Window, Icon);
    SDL_FreeSurface(Icon);
  }

  Renderer = SDL_CreateRenderer(Window, -1, 0);
  if(!Renderer)
    ABORT("Couldn't set renderer mode.");

  SDL_RenderSetLogicalSize(Renderer, NewRes.X, NewRes.Y);

  /* The following code will determine whether to use nearest neighbor or
   * linear interpolation when scaling the game in fullscreen mode. */

  SDL_DisplayMode VirtualDisplayMode;
  if(SDL_GetDesktopDisplayMode(0, &VirtualDisplayMode) == 0)
  {
    v2 ActualWindowRes; // On high-DPI displays this is greater than NewRes.
    SDL_GL_GetDrawableSize(Window, &ActualWindowRes.X, &ActualWindowRes.Y);

    v2 ActualDisplayRes;
    if(SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
      ActualDisplayRes = ActualWindowRes;
    else
      ActualDisplayRes = v2(ActualWindowRes.X / NewRes.X * VirtualDisplayMode.w,
                            ActualWindowRes.Y / NewRes.Y * VirtualDisplayMode.h);

    if((ActualDisplayRes.Y % NewRes.Y == 0
       && ActualDisplayRes.X >= ActualDisplayRes.Y / NewRes.Y * NewRes.X)
       || (ActualDisplayRes.X % NewRes.X == 0
       && ActualDisplayRes.Y >= ActualDisplayRes.X / NewRes.X * NewRes.Y))
      /* In-game pixels can be safely mapped one-on-one to rectangular
       * units consisting of one or more on-screen pixels. */
      SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    else
      SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  }
  else
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  Texture = SDL_CreateTexture(Renderer,
                              SDL_PIXELFORMAT_RGB565,
                              SDL_TEXTUREACCESS_STREAMING,
                              NewRes.X, NewRes.Y);
#endif

  globalwindowhandler::Init();
  DoubleBuffer = new bitmap(NewRes);
  StretchedDB = new bitmap(NewRes);
  Res = NewRes;
  SetScale(NewScale);
  ColorDepth = 16;

#if SDL_MAJOR_VERSION == 1
#if SDL_BYTEORDER == SDL_BIG_ENDIAN

  Uint32 rmask, gmask, bmask;
  rmask = 0xF800;
  gmask = 0x7E0;
  bmask = 0x1F;

  TempSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, Res.X, Res.Y, 16,
                                     rmask, gmask, bmask, 0);

  if(!TempSurface)
    ABORT("CreateRGBSurface failed: %s\n", SDL_GetError());

#endif
#endif
}

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

void graphics::BlitDBToScreen()
{
#if SDL_MAJOR_VERSION == 1
  SDL_LockSurface(TempSurface);
  packcol16* SrcPtr = DoubleBuffer->GetImage()[0];
  packcol16* DestPtr = static_cast<packcol16*>(TempSurface->pixels);
  ulong ScreenYMove = (TempSurface->pitch >> 1);
  ulong LineSize = Res.X << 1;

  for(int y = 0; y < Res.Y; ++y, SrcPtr += Res.X, DestPtr += ScreenYMove)
    memcpy(DestPtr, SrcPtr, LineSize);

  SDL_UnlockSurface(TempSurface);
  SDL_Surface* S = SDL_DisplayFormat(TempSurface);
  SDL_BlitSurface(S, NULL, Screen, NULL);
  SDL_FreeSurface(S);
  SDL_UpdateRect(Screen, 0, 0, Res.X, Res.Y);
#else
  SDL_UpdateTexture(sdlTexture, NULL, myPixels, 640 * sizeof (Uint32));
#endif
}

#else

void graphics::Stretch(bitmap* bmpFrom, blitdata Bto){
  Stretch(bUseXbrzScale,bmpFrom,Bto);
}
void graphics::Stretch(bool bXbrzMode, bitmap* bmpFrom, blitdata Bto){
  if(bXbrzMode){
    bmpFrom->StretchBlitXbrz(Bto);
  }else{
    bmpFrom->StretchBlit(Bto);
  }
}

void graphics::SetStretchMode(truth isXbrz){
  bUseXbrzScale = isXbrz;
}

void graphics::SetAllowStretchedBlit(truth b){
  bAllowStretchedBlit=b;
}

bool graphics::IsSRegionEnabled(int iIndex){
  if(iIndex>=vStretchRegion.size())return false; //not ready yet
  return vStretchRegion[iIndex].bEnabled;
}
void graphics::SetSRegionEnabled(int iIndex, bool b){
  vStretchRegion[iIndex].bEnabled=b;
}
void graphics::SetSRegionForceXBRZ(int iIndex, bool b){
  vStretchRegion[iIndex].bForceXBRZ=b;
}
void graphics::SetSRegionShowWithFelist(int iIndex, bool b){
  vStretchRegion[iIndex].bShowWithFelist=b;
}
/**
 * there can only be one set at a time
 */
void graphics::SetSRegionListItem(int iIndex, bool bUseAlternateListItemPos){
  if(vStretchRegion[iIndex].bSpecialListItem)return; //permissive on redundant setup

  for(int i=0;i<vStretchRegion.size();i++){
    stretchRegion SR=vStretchRegion[i];
    assert(!SR.bSpecialListItem); //some other was set already
  }

  vStretchRegion[iIndex].bSpecialListItem=true;
  vStretchRegion[iIndex].bSpecialListItemAltPos=bUseAlternateListItemPos;
  vStretchRegion[iIndex].bShowWithFelist=true;
  vStretchRegion[iIndex].bEnabled=false;
}
int graphics::SetSRegionBlitdata(int iIndex, blitdata B){
  assert(B.Stretch>1); // some actual scaling is required
  assert(B.Bitmap==NULL); // see below

  B.Bitmap = StretchedDB;
  if(iIndex==-1){ //add
    stretchRegion SRcp = SRdefault;
    stretchRegion& SR = SRcp;
    SR.B=B;
    iIndex = SR.iIndex = vStretchRegion.size();
    vStretchRegion.push_back(SR);
    //DBGSRI("Add");
  }else{ //update
    stretchRegion& SR = vStretchRegion[iIndex];
    //DBG2(SR.iIndex,iIndex);assert(SR.iIndex==iIndex);
    SR.B=B;
    //DBGSRI("Update");
  }

  return iIndex;
}

int graphics::AddStretchRegion(blitdata B,const char* strId){
  int i = SetSRegionBlitdata(-1, B);
  vStretchRegion[i].strId=strId;
  return i;
}

void graphics::BlitDBToScreen(){
#if SDL_MAJOR_VERSION == 1
  if(SDL_MUSTLOCK(Screen) && SDL_LockSurface(Screen) < 0)
    ABORT("Can't lock screen");

  packcol16* SrcPtr = DoubleBuffer->GetImage()[0];

  packcol16* DestPtr = static_cast<packcol16*>(Screen->pixels);
  ulong ScreenYMove = (Screen->pitch >> 1);
  ulong LineSize = Res.X << 1;

  for(int y = 0; y < Res.Y; ++y, SrcPtr += Res.X, DestPtr += ScreenYMove)
    memcpy(DestPtr, SrcPtr, LineSize);

  if(SDL_MUSTLOCK(Screen))
    SDL_UnlockSurface(Screen);

  SDL_UpdateRect(Screen, 0, 0, Res.X, Res.Y);
#else
  bitmap* DB = DoubleBuffer;

  if(
      bAllowStretchedBlit // basically, if the game is running. TODO so, not at loading animations, did it work?
      &&
      !iosystem::IsOnMenu() //main menu
      &&
      vStretchRegion.size()>0
  ){
    //DBG2("StretchedBlitsTot=",vStretchRegion.size());
    bool bDidStretch=false;
    bool bOk=true;
    for(int i=0;i<vStretchRegion.size();i++){
      stretchRegion SR=vStretchRegion[i];
      blitdata& B=SR.B;//DBGSR;

      bOk=true; // try to disable below, is more clear to read long lists

      if(!SR.bEnabled)bOk=false;//DBGOK;

      if(felist::isAnyFelistCurrentlyDrawn() && !SR.bShowWithFelist)bOk=false;//DBGOK;

      if(SR.bSpecialListItem && !felist::isAnyFelistCurrentlyDrawn())bOk=false;//DBGOK;

      if(B.Stretch<2)bOk=false;//DBGOK;

      assert(B.Border.X>=0 && B.Border.Y>=0); // only negatives are critical
      if(B.Border.X==0 || B.Border.Y==0){//DBGOK;
        if(B.Border.Is0()){//DBGOK; //being 0,0 means it is not ready yet.
          bOk=false;
        }else{//DBGOK;
          assert(B.Border.X>0 && B.Border.Y>0); //minimum (if not 0,0) is 1,1
        }
      }

      assert(B.Dest.X>=0 && B.Dest.Y>=0);//DBGOK;

      if(bOk){
        if(!bDidStretch){
          // first time, if there is at least one stretching, prepare "background/base" on the stretched
          DoubleBuffer->FastBlit(StretchedDB);
          DB = StretchedDB; //and set stretched as the final source
        }

        if(SR.bSpecialListItem){
          B.Src = felist::GetSelectedPos();
          if(SR.bSpecialListItemAltPos){
            B.Dest.X=0;//B.Dest.X=5;
            B.Dest.Y=B.Src.Y - (B.Border.Y*B.Stretch/2);
            if(B.Dest.Y<0)B.Dest.Y=0;
          }
          //DBGSRI("ListItem");
        }

        //DBG2("Blitting",i);
        if(SR.bForceXBRZ){
          Stretch(true,DoubleBuffer,B);
        }else{
          Stretch(DoubleBuffer,B);
        }
        //TODO should outline only the requested rectangle //Bto.Bitmap->Outline(LIGHT_GRAY, 0xff, AVERAGE_PRIORITY);

        bDidStretch=true;
      }
    }
  }

  packcol16* SrcPtr = DB->GetImage()[0];
  void* DestPtr;
  int Pitch;

  if (SDL_LockTexture(Texture, NULL, &DestPtr, &Pitch) == 0)
  {
    memcpy(DestPtr, SrcPtr, Res.Y * Pitch);
    SDL_UnlockTexture(Texture);
  }
  else
  {
    // Try to use the slower SDL_UpdateTexture() as a fallback if SDL_LockTexture() fails.
    SDL_UpdateTexture(Texture, NULL, SrcPtr, Res.X * sizeof(packcol16));
  }

  SDL_RenderClear(Renderer);
  SDL_RenderCopy(Renderer, Texture, NULL, NULL);
  SDL_RenderPresent(Renderer);
#endif
}

#endif

void graphics::SetScale(int NewScale)
{
  Scale = NewScale;
#if SDL_MAJOR_VERSION == 1
#warning Graphics scaling not implemented for SDL v1
#else
  // Scale the window, maintaining its center position.
  v2 WindowPos, OldSize, NewSize = Res * NewScale;
  SDL_GetWindowPosition(Window, &WindowPos.X, &WindowPos.Y);
  SDL_GetWindowSize(Window, &OldSize.X, &OldSize.Y);
  WindowPos += (OldSize - NewSize) / 2;
  SDL_SetWindowPosition(Window, WindowPos.X, WindowPos.Y);
  SDL_SetWindowSize(Window, Res.X * NewScale, Res.Y * NewScale);
  SDL_RenderSetScale(Renderer, NewScale, NewScale);
#endif
}

void graphics::SwitchMode()
{
#if SDL_MAJOR_VERSION == 1
  ulong Flags;

  if(Screen->flags & SDL_FULLSCREEN)
  {
    SDL_ShowCursor(SDL_ENABLE);
    Flags = SDL_SWSURFACE;
  }
  else
  {
    SDL_ShowCursor(SDL_DISABLE);
    Flags = SDL_SWSURFACE|SDL_FULLSCREEN;
  }

  Screen = SDL_SetVideoMode(Res.X, Res.Y, ColorDepth, Flags);

  if(!Screen)
    ABORT("Couldn't toggle fullscreen mode.");

  BlitDBToScreen();
#else
  ulong Flags = SDL_GetWindowFlags(Window);
  if(Flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
  {
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetWindowFullscreen(Window, 0);
    SetScale(Scale);
  }
  else
  {
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  BlitDBToScreen();
#endif

  if(SwitchModeHandler)
    SwitchModeHandler();
}

#endif

void graphics::LoadDefaultFont(cfestring& FileName)
{
  DefaultFont = new rawbitmap(FileName);
}

#ifdef __DJGPP__

void graphics::SetMode(cchar*, cchar*, v2 NewRes, truth)
{
  ulong Mode;

  for(Mode = 0; Mode < 0x10000; ++Mode)
  {
    ModeInfo.Retrieve(Mode);

    if(ModeInfo.Attribs1 & 0x01
       && ModeInfo.Attribs1 & 0xFF
       && ModeInfo.Width == NewRes.X
       && ModeInfo.Height == NewRes.Y
       && ModeInfo.BitsPerPixel == 16)
      break;
  }

  if(Mode == 0x10000)
    ABORT("Resolution %dx%d not supported!", NewRes.X, NewRes.Y);

  __dpmi_regs Regs;
  Regs.x.ax = 0x4F02;
  Regs.x.bx = Mode | 0x4000;
  __dpmi_int(0x10, &Regs);
  Res.X = ModeInfo.Width;
  Res.Y = ModeInfo.Height;
  BufferSize = Res.Y * ModeInfo.BytesPerLine;
  delete DoubleBuffer;
  DoubleBuffer = new bitmap(Res);
  __dpmi_meminfo MemoryInfo;
  MemoryInfo.size = BufferSize;
  MemoryInfo.address = ModeInfo.PhysicalLFBAddress;
  __dpmi_physical_address_mapping(&MemoryInfo);
  __dpmi_lock_linear_region(&MemoryInfo);
  ScreenSelector = __dpmi_allocate_ldt_descriptors(1);
  __dpmi_set_segment_base_address(ScreenSelector, MemoryInfo.address);
  __dpmi_set_segment_limit(ScreenSelector, BufferSize - 1);
}

void graphics::BlitDBToScreen()
{
  movedata(_my_ds(), ulong(DoubleBuffer->GetImage()[0]),
           ScreenSelector, 0, BufferSize);
}

void graphics::vesainfo::Retrieve()
{
  Signature = 0x32454256;
  dosmemput(this, sizeof(vesainfo), __tb);
  __dpmi_regs Regs;
  Regs.x.ax = 0x4F00;
  Regs.x.di =  __tb       & 0x000F;
  Regs.x.es = (__tb >> 4) & 0xFFFF;
  __dpmi_int(0x10, &Regs);
  dosmemget(__tb, sizeof(vesainfo), this);
}

void graphics::modeinfo::Retrieve(ushort Mode)
{
  __dpmi_regs Regs;
  Regs.x.ax = 0x4F01;
  Regs.x.cx = Mode;
  Regs.x.di =  __tb       & 0x000F;
  Regs.x.es = (__tb >> 4) & 0xFFFF;
  dosmemput(this, sizeof(modeinfo), __tb);
  __dpmi_int(0x10, &Regs);
  dosmemget(__tb, sizeof(modeinfo), this);
}

#endif
