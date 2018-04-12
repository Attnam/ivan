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

//TODO create a utility `class sregion{}` to set it's values outside here w/o using graphics::...
struct stretchRegion //TODO all these booleans could be a single uint32? unnececessarily complicated?
{
  int iIndex;
  const char* strId;
  bool bEnabled;
  blitdata B;

  bool bUseXBRZ;
  bool bUseXBRZDrawBeforeFelistPage;
  bool bDrawBeforeFelistPage;
  bool bDrawAfterFelist;
  bool bSpecialListItem;
  bool bDrawRectangleOutline;
  bool bAllowTransparency; //mask

  v2 v2SquareSize; //to clear or to inc/dec zoom

  std::vector<v2> vv2ClearSquaresAt; //these are the relative top left square's pixels at BClearSquares.Bitmap
  blitdata BClearSquares;

  bitmap* CacheBitmap;
};
#define DBGMSG_STRETCHREGION
#include "dbgmsgproj.h"

const stretchRegion SRdefault = {
  -1,"READABLE ID NOT SET!!!",true,DEFAULT_BLITDATA,
  false,false,false,false,false,false,false,
  v2(),
  std::vector<v2>(), DEFAULT_BLITDATA,
  NULL
};
bool graphics::bSpecialListItemAltPos=false;
bool bPrepareCacheBitmapsBeforeFelist=false;
bool bDrawCacheBitmapsBeforeFelist=false;
std::vector<stretchRegion> vStretchRegion;
truth graphics::bAllowStretchedRegionsBlit=false;

bitmap* graphics::DoubleBuffer=NULL;
bitmap* graphics::StretchedDB=NULL;
v2 graphics::Res;
int graphics::Scale;
int graphics::ColorDepth;
rawbitmap* graphics::DefaultFont = 0;

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

void graphics::Stretch(bool bXbrzMode, bitmap* pBmpFrom, blitdata& rBto, bool bAllowTransparency){
  if(bXbrzMode){
    pBmpFrom->StretchBlitXbrz(rBto,bAllowTransparency);
  }else{
    pBmpFrom->StretchBlit(rBto);
  }
}

bool graphics::IsSRegionEnabled(int iIndex){
  if(iIndex>=vStretchRegion.size())return false; //not ready yet
  return vStretchRegion[iIndex].bEnabled;
}
void graphics::SetSRegionEnabled(int iIndex, bool b){
  vStretchRegion[iIndex].bEnabled=b;
  if(b){stretchRegion& rSR = vStretchRegion[iIndex];DBGSR;}
}
void graphics::SetSRegionUseXBRZ(int iIndex, bool b){
  vStretchRegion[iIndex].bUseXBRZ=b;
}
void graphics::SetSRegionDrawBeforeFelistPage(int iIndex, bool bDrawBeforeFelistPage, bool bUseXBRZDrawBeforeFelistPage){
  vStretchRegion[iIndex].bDrawBeforeFelistPage=bDrawBeforeFelistPage;
  vStretchRegion[iIndex].bUseXBRZDrawBeforeFelistPage=bUseXBRZDrawBeforeFelistPage;
}
void graphics::SetSRegionDrawAfterFelist(int iIndex, bool b){
  vStretchRegion[iIndex].bDrawAfterFelist=b;
}
void graphics::SetSRegionDrawRectangleOutline(int iIndex, bool b){
  vStretchRegion[iIndex].bDrawRectangleOutline=b;
}
void graphics::SetSRegionClearSquaresAt(int iIndex, v2 v2Size, std::vector<v2> vv2){
  vStretchRegion[iIndex].vv2ClearSquaresAt=vv2;
  vStretchRegion[iIndex].v2SquareSize=v2Size;
  vStretchRegion[iIndex].bAllowTransparency=true;
}
/**
 * there can only be one set at a time
 */
void graphics::SetSRegionListItem(int iIndex){
  if(vStretchRegion[iIndex].bSpecialListItem)return; //permissive on redundant setup

  for(int i=0;i<vStretchRegion.size();i++){
    stretchRegion SR=vStretchRegion[i];
    if(SR.bSpecialListItem)ABORT("some other SRegion is already bSpecialListItem");
  }

  vStretchRegion[iIndex].bSpecialListItem=true;
  vStretchRegion[iIndex].bDrawAfterFelist=true;
  vStretchRegion[iIndex].bEnabled=false;
}
/**
 * it actually copies the blitdata
 */
int graphics::SetSRegionBlitdata(int iIndex, blitdata B){
  if(B.Stretch<=1)ABORT("SRegion stretch value invalid %d", B.Stretch); // some actual scaling is required
  if(B.Bitmap!=NULL)ABORT("SRegion bitmap should not be set."); // see below
  if(StretchedDB==NULL)ABORT("StretchedDB not set yet.");

  B.Bitmap = StretchedDB;
  if(iIndex==-1){ //add
    stretchRegion SRcp = SRdefault;
    stretchRegion& rSR = SRcp;
    rSR.B=B;
    iIndex = rSR.iIndex = vStretchRegion.size();
    vStretchRegion.push_back(rSR);DBGSRI("Add");
  }else{ //update
    stretchRegion& rSR = vStretchRegion[iIndex];
    DBG2(rSR.iIndex,iIndex);if(rSR.iIndex!=iIndex)ABORT("wrongly configured SRegion internal index %d, expecting %d",rSR.iIndex,iIndex);
    rSR.B=B;
    DBGSRI("Update");
  }

  return iIndex;
}

int graphics::AddStretchRegion(blitdata B,const char* strId){
  int i = SetSRegionBlitdata(-1, B);
  stretchRegion& rSR = vStretchRegion[i];
  rSR.strId=strId;DBGSRI("AddOk");
  return i;
}

bitmap* SRegionPrepareClearedSquares(bitmap* DoubleBuffer, stretchRegion& rSR){
  blitdata& rB = rSR.B;
  blitdata& rBC = rSR.BClearSquares;
  if(rBC.Bitmap==NULL || rBC.Bitmap->GetSize()!=rB.Border){
    if(rBC.Bitmap!=NULL)delete rBC.Bitmap;
    rBC.Bitmap = new bitmap(rB.Border);
  }

  rBC.Src = rB.Src;DBGLN;
  rBC.Dest = {0,0};DBGLN;
  rBC.Border = rB.Border;DBGLN;
  DBGBLD(rBC);
  DoubleBuffer->NormalBlit(rBC);DBGLN;

  for(int i=0;i<rSR.vv2ClearSquaresAt.size();i++){
    rBC.Bitmap->Fill(rSR.vv2ClearSquaresAt[i],rSR.v2SquareSize,TRANSPARENT_COLOR);
  }
  rSR.vv2ClearSquaresAt.clear();

  rB.Src=v2(); //as the blitdata for cleared squares will now be the source to be stretched from
  return rBC.Bitmap;
}

bool graphics::isStretchedRegionsAllowed(){
  return
    bAllowStretchedRegionsBlit
    &&
    !iosystem::IsOnMenu() //main menu
    &&
    vStretchRegion.size()>0
    ;
}

void graphics::PrepareBeforeDrawingFelist(){
  if(!isStretchedRegionsAllowed())return;

  for(int i=0;i<vStretchRegion.size();i++){
    stretchRegion& rSR=vStretchRegion[i];
    blitdata& rB=rSR.B;

    if(!rSR.bDrawBeforeFelistPage)continue;

    if(rSR.CacheBitmap==NULL || (rSR.CacheBitmap->GetSize() != rB.Border)){
      if(rSR.CacheBitmap!=NULL){
        delete rSR.CacheBitmap;
      }

      rSR.CacheBitmap = new bitmap(rB.Border * rB.Stretch);
    }

    blitdata B = rSR.B; //copy
    B.Bitmap = rSR.CacheBitmap;
    B.Dest=v2(0,0);

    Stretch(rSR.bUseXBRZ && rSR.bUseXBRZDrawBeforeFelistPage, DoubleBuffer, B, false);
  }
}

void graphics::DrawAtDoubleBufferBeforeFelistPage(){
  if(!isStretchedRegionsAllowed())return;

  for(int i=0;i<vStretchRegion.size();i++){
    stretchRegion& rSR=vStretchRegion[i];

    if(!rSR.bDrawBeforeFelistPage)continue;

    rSR.CacheBitmap->FastBlit(DoubleBuffer, rSR.B.Dest); // is a cache substitute to the region scaling
  }
}

bitmap* graphics::PrepareBuffer(){
  bitmap* ReturnBuffer = DoubleBuffer;

//  if(felist::isAnyFelistCurrentlyDrawn())graphics::DrawAtDoubleBufferBeforeFelistPage();

  if(isStretchedRegionsAllowed()){ // !!!!!!!!!!!!!!!!!!!!!!!! DO NOT MODIFY DoubleBuffer HERE (chaotic recursive blitting problem) !!!!!!!!!!!!!!!!!!!!!!!!
    bool bDidStretch=false;
    bool bOk=true;

    for(int i=0;i<vStretchRegion.size();i++){
      stretchRegion& rSR=vStretchRegion[i];
      blitdata& rB=rSR.B;DBGSRI("tryBlit");

      if(rB.Bitmap!=StretchedDB)ABORT("SRegion bitmap is not pointing to StretchedDB.");

      // try to disable below, is easier to read long lists
      bOk=true;

      if(bOk && (!rSR.bEnabled))bOk=false;DBGSB(bOk);

      if(bOk && (rB.Stretch<2 ))bOk=false;DBGSB(bOk);

//      if(bOk && (rSR.bDrawBeforeFelistPage))bOk=false;DBGSB(bOk); //bDrawBeforeFelistPage is not meant to work here.

      if(felist::isAnyFelistCurrentlyDrawn()){
        if(bOk && (!rSR.bDrawAfterFelist))bOk=false;DBGSB(bOk);
      }else{
        if(bOk && ( rSR.bDrawAfterFelist))bOk=false;DBGSB(bOk);
      }

      if(!(rB.Border.X>=0 && rB.Border.Y>=0))ABORT("invalid SRegion border (negatives are critical) %d,%d",rB.Border.X,rB.Border.Y);
      if(bOk)if(rB.Border.X==0 || rB.Border.Y==0){DBGSB(bOk);
        if(rB.Border.Is0()){DBGSB(bOk); //being 0,0 may mean it is not ready yet (wouldnt be accepted to blit anyway).
          bOk=false;DBGSB(bOk);
        }else{DBGSB(bOk);
          if(!(rB.Border.X>0 && rB.Border.Y>0))ABORT("SRegion border: minimum (if not 0,0) is 1,1: %d,%d",rB.Border.X,rB.Border.Y);
        }
      }

      if(!(rB.Dest.X>=0 && rB.Dest.Y>=0))ABORT("invalid SRegion Dest (negatives are critical) %d,%d",rB.Dest.X,rB.Dest.Y);DBGSB(bOk);

      if(bOk){
        if(!bDidStretch){
          // first time, if there is at least one stretching, prepare "background/base" on the stretched
          DoubleBuffer->FastBlit(StretchedDB); //simple copy (like a 3rd buffer)
          ReturnBuffer = StretchedDB; //and set stretched as the final source
        }

        if(rSR.bSpecialListItem){ DBGSRI("ListItem");
          rB.Src = felist::GetCurrentListSelectedItemPos(); //the tiny one
        }

        bool bDrawSROutline=rSR.bDrawRectangleOutline;
        if(rSR.bSpecialListItem){
          if(bSpecialListItemAltPos){
            bool bAltPosFullBkg=false; //TODO user option ? doesnt look too good anyway..
            // let felist re-configure the blitdata before the stretching below
            felist::PrepareListItemAltPosBackground(rB,bAltPosFullBkg);
            if(bAltPosFullBkg)bDrawSROutline=false;
          }
        }

        if(bDrawSROutline){
          graphics::DrawRectangleOutlineAround(rB.Bitmap, rB.Dest, (rB.Border) * (rB.Stretch), DARK_GRAY, true);
        }

        bitmap* pbmpFrom = DoubleBuffer;
        if(rSR.vv2ClearSquaresAt.size()>0)pbmpFrom = SRegionPrepareClearedSquares(DoubleBuffer,rSR);

        DBGSRI("STRETCHING FROM DoubleBuffer TO StretchedDB");
        Stretch(rSR.bUseXBRZ, pbmpFrom, rB, rSR.bAllowTransparency);

        bDidStretch=true;
      }
    }
  }

  return ReturnBuffer;
}

void graphics::DrawRectangleOutlineAround(bitmap* bmpAt, v2 v2TopLeft, v2 v2Border, col16 color, bool wide){
  v2 v2BottomRight = v2TopLeft+v2Border;
  if(wide){ //is 3 thickness
    v2TopLeft -= v2(2,2);
    v2BottomRight += v2(1,1);
  }else{
    v2TopLeft -= v2(1,1);
  }

  bmpAt->DrawRectangle(
    v2TopLeft,
    v2BottomRight,
    color, wide
  );
}

int graphics::GetTotSRegions(){
  return vStretchRegion.size();
}

void graphics::BlitDBToScreen()
{
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
  packcol16* SrcPtr = PrepareBuffer()->GetImage()[0];
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
