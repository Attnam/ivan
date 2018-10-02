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

#include <cstdio>
#include <ctime>
#include <ratio>
#include <chrono>

#include "bitmap.h"
#include "error.h"
#include "graphics.h"
#include "festring.h"
#include "rawbit.h"
#include "whandler.h"

#include "dbgmsgproj.h"

#if SDL_MAJOR_VERSION == 1
/* redefine SDL2 to SDL1 */
#define SDL_WINDOWEVENT SDL_VIDEOEXPOSE
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#endif

truth (*globalwindowhandler::ControlLoop[MAX_CONTROLS])();
int globalwindowhandler::Controls = 0;
ulong globalwindowhandler::Tick;
truth globalwindowhandler::ControlLoopsEnabled = true;
truth globalwindowhandler::playInBackground=false;
festring globalwindowhandler::ScrshotDirectoryName = "";
truth bLastSDLkeyEventIsKeyUp=false;

void globalwindowhandler::InstallControlLoop(truth (*What)())
{
  if(Controls == MAX_CONTROLS)
    ABORT("Animation control frenzy!");

  ControlLoop[Controls++] = What;
}

void globalwindowhandler::DeInstallControlLoop(truth (*What)())
{
  int c;

  for(c = 0; c < Controls; ++c)
    if(ControlLoop[c] == What)
      break;

  if(c != Controls)
  {
    --Controls;

    for(; c < Controls; ++c)
      ControlLoop[c] = ControlLoop[c + 1];
  }
}

bool globalwindowhandler::IsKeyPressed(int iSDLScanCode)
{
  return SDL_GetKeyboardState(NULL)[iSDLScanCode];
}


#ifdef __DJGPP__

#include <pc.h>
#include <keys.h>

int globalwindowhandler::GetKey(truth EmptyBuffer)
{
  if(EmptyBuffer)
    while(kbhit())
      getkey();

  int Key = 0;

  while(!Key)
  {
    while(!kbhit())
      if(Controls && ControlLoopsEnabled)
      {
        static ulong LastTick = 0;
        UpdateTick();

        if(LastTick != Tick)
        {
          LastTick = Tick;
          truth Draw = false;

          for(int c = 0; c < Controls; ++c)
            if(ControlLoop[c]())
              Draw = true;

          if(Draw)
            graphics::BlitDBToScreen();
        }
      }

    Key = getkey();

    if(Key == K_Control_Print && !ScrshotDirectoryName.IsEmpty())
    {
      mkdir(ScrshotDirectoryName.CStr(), S_IRUSR|S_IWUSR);
      DOUBLE_BUFFER->Save(ScrshotNameHandler());
      Key = 0;
    }
  }

  return Key;
}

int globalwindowhandler::ReadKey()
{
  return kbhit() ? getkey() : 0;
}

#endif

#ifdef USE_SDL

#include <algorithm>

std::vector<int> globalwindowhandler::KeyBuffer;
truth (*globalwindowhandler::QuitMessageHandler)() = 0;
bool (*globalwindowhandler::FunctionKeyHandler)(SDL_Keycode) = 0;
bool (*globalwindowhandler::ControlKeyHandler)(SDL_Keycode) = 0;

void globalwindowhandler::Init()
{
#if SDL_MAJOR_VERSION == 1
  SDL_EnableUNICODE(1);
  SDL_EnableKeyRepeat(500, 30);
#else
  //FIXSDL2 SDL_EnableKeyRepeat(500, 30);
#endif
}

int iCountFPS=0;
int iLastSecondFPS=0;
std::chrono::high_resolution_clock::time_point tpLastSecondFPS;
int MeasureLastSecondRealFPS(){ //call this every new frame request
  iCountFPS++;

  using namespace std::chrono;
  high_resolution_clock::time_point tpNow = high_resolution_clock::now();
  duration<double, std::milli> delay = tpNow - tpLastSecondFPS;
  if(delay.count() > 1000){ //1s
    iLastSecondFPS=iCountFPS;
    iCountFPS=0; //reset
    tpLastSecondFPS = tpNow; //reset
  }

  return iLastSecondFPS;
}

std::chrono::high_resolution_clock::time_point tpPreviousFrameTime;
double InstaCalcFPS(){ //call this every new frame request
  using namespace std::chrono;
  high_resolution_clock::time_point tpNow = high_resolution_clock::now();
  duration<double, std::milli> delay = tpNow - tpPreviousFrameTime;
  tpPreviousFrameTime = tpNow;
  double d = delay.count();
  if(d==0)d=1; //TODO this may never happen?
  return 1000/d;
}

bool bAllowFrameSkip=true;
int iFrameSkip=0;
std::chrono::high_resolution_clock::time_point tpBefore;
std::chrono::duration<double, std::milli> delay;
int iLastSecCountFPS;
float fInstaFPS;
double dLastFrameTimeMS;
int iDefaultDelayMS=10;
int iAddFrameSkip=0;

void globalwindowhandler::SetAddFrameSkip(int i){
  iAddFrameSkip=i;
  bAllowFrameSkip = iAddFrameSkip!=0;
}

/**
 *  to let user input be more responsive as full dungeon xBRZ may be too heavy
 *  return SDL delay in ms
 */
int FrameSkipOrDraw(){ //TODO could this be simplified?
  bool bWaitAKeyPress=false;
  bool bDoAutoFrameSkip=false;
  bool bDrawFrame=false;
  bool bDelay1MS=false;

  if(iFrameSkip<0)ABORT("iFrameSkip is %d < 0",iFrameSkip);

  if(iAddFrameSkip==-2){
    if(dLastFrameTimeMS>142){ //if it is too slow <= 7 fps
      bWaitAKeyPress=true;
    }else{
      bDoAutoFrameSkip=true;
    }
  }else
  if(iAddFrameSkip==-1){
    bDoAutoFrameSkip=true;
  }

  if(!bWaitAKeyPress){ // if waiting a key press, there will have no stand-by animation at all...
    if(iFrameSkip==0){
      bDrawFrame=true;

      // setup next
      if(bDoAutoFrameSkip){ //TODO improve this automatic mode?
        if(dLastFrameTimeMS>250){ //4fps
          iFrameSkip=10;
        }else
        if(dLastFrameTimeMS>200){ //5fps
          iFrameSkip=5;
        }else
        if(dLastFrameTimeMS>150){
          iFrameSkip=3;
        }else
        if(dLastFrameTimeMS>100){ //10fps
          iFrameSkip=1;
        }

        bDelay1MS=true;
      }else{
        if(iAddFrameSkip==0){ //vanilla  (wont be reached tho cuz of bAllowFrameSkip=false), kept for completeness (in case it changes before calling this method)
          bDrawFrame=true;
        }else
        if(iAddFrameSkip>0){ //fixed
          iFrameSkip=iAddFrameSkip;

          bDelay1MS=true;
        }
      }
    }
  }

  if(bDrawFrame){
    tpBefore = std::chrono::high_resolution_clock::now();
    graphics::BlitDBToScreen();
    delay = std::chrono::high_resolution_clock::now() - tpBefore;
    dLastFrameTimeMS = delay.count();

    //call these ONLY when the frame is actually DRAWN!!! (despite not being actually used yet)
    iLastSecCountFPS = MeasureLastSecondRealFPS();
    fInstaFPS = InstaCalcFPS();
    //DBG5(DBGF(fInstaFPS),DBGI(iLastSecCountFPS),DBGF(dLastFrameTimeMS),DBGI(iAddFrameSkip),DBGI(iFrameSkip));
  }else{
    if(iFrameSkip>0)iFrameSkip--;
    //DBGSI(iFrameSkip);
  }

  if(bDelay1MS){
    return 1;
  }else{
    return iDefaultDelayMS;
  }
}

const int globalwindowhandler::iRestWaitKey = '.';

int iTimeoutDelay=0; // must init with 0
int iTimeoutDefaultKey = globalwindowhandler::iRestWaitKey;
long keyTimeoutRequestedAt;
/**
 * This is intended to remain active ONLY until the user hits any key.
 * iTimeoutMillis can be 0 or >=10
 */
void globalwindowhandler::SetKeyTimeout(int iTimeoutMillis,int iDefaultReturnedKey)//,int iIgnoreKeyWhenDisabling)
{
  if(iTimeoutMillis<0)ABORT("invalid negative timeout %d",iTimeoutMillis);

  iTimeoutDelay = (iTimeoutMillis/1000.0) * CLOCKS_PER_SEC;
  if(iTimeoutDelay>0 && iTimeoutDelay<10)iTimeoutDelay=10; // we are unable to issue commands if it is too low TODO could be less than 10ms?

  iTimeoutDefaultKey=iDefaultReturnedKey;
}
truth globalwindowhandler::IsKeyTimeoutEnabled()
{
  return iTimeoutDelay>0;
}
void globalwindowhandler::CheckKeyTimeout()
{
  if(iTimeoutDelay>0){ // timeout mode is enalbed
    if(!KeyBuffer.empty()){ DBG2(KeyBuffer.size(),KeyBuffer[0]); // user pressed some key
      keyTimeoutRequestedAt=clock(); // resets reference time to wait from
    }else{ DBG2(keyTimeoutRequestedAt,iTimeoutDelay);
      if( clock() > (keyTimeoutRequestedAt+iTimeoutDelay) ) //wait for the timeout to...
        KeyBuffer.push_back(iTimeoutDefaultKey); //...simulate the keypress
    }
  }
}
int iTimeoutDelayBkp=0;
void globalwindowhandler::SuspendKeyTimeout()
{
  iTimeoutDelayBkp=iTimeoutDelay;
  iTimeoutDelay=0;
}
void globalwindowhandler::ResumeKeyTimeout()
{
  iTimeoutDelay=iTimeoutDelayBkp;
}

float globalwindowhandler::GetFPS(bool bInsta){
  if(bInsta)return fInstaFPS;
  return iLastSecCountFPS;
}

truth globalwindowhandler::HasKeysOnBuffer(){
  return KeyBuffer.size()>0;
}

void ShowFPS(){ //TODO still flickers sometimes cuz of silhouette?
  static long lTimePrevious=clock();
  static bool bShowFPS = [](){const char* c=std::getenv("IVAN_SHOWFPS");return c!=NULL && strcmp(c,"true")==0;}();
  if(bShowFPS){
//    if(clock()%(CLOCKS_PER_SEC*3)<CLOCKS_PER_SEC){
    long lTime=clock();
    if(clock()-lTimePrevious > CLOCKS_PER_SEC*1){
      static int iMargin=2;
      static v2 v2Margin(iMargin,iMargin);
      static char c[100];

      sprintf(c,"FPS:ls=%.1f,insta=%.1f",globalwindowhandler::GetFPS(false),globalwindowhandler::GetFPS(true));
      int iDistX = strlen(c)*8 + 10;
      v2 v2Pos = RES-v2(iDistX,RES.Y)+v2Margin;
      v2 v2Size(iDistX, 8+iMargin*2);

      DOUBLE_BUFFER->Fill(v2Pos,v2Size,BLACK);
      FONT->Printf(DOUBLE_BUFFER,v2Pos+v2Margin,WHITE,"%s",c);
      lTimePrevious=lTime;
    }
  }
}

int globalwindowhandler::GetKey(truth EmptyBuffer)
{
  SDL_Event Event;

  if(EmptyBuffer)
  {
    PollEvents(&Event);
    KeyBuffer.clear();
  }

  keyTimeoutRequestedAt=clock();
  int iDelayMS=iDefaultDelayMS;
  for(;;){
    CheckKeyTimeout();

    if(!KeyBuffer.empty())
    {
      int Key = KeyBuffer[0];
      KeyBuffer.erase(KeyBuffer.begin());

      if(Key > 0xE000)
        return Key - 0xE000;

      if(Key && Key < 0x81)
        return Key;
    }
    else
    {
      bool bHasFocus=false;
#if SDL_MAJOR_VERSION == 1
      bHasFocus = SDL_GetAppState() & SDL_APPACTIVE;
#else
      bHasFocus = SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
#endif

      bool bPlay=true;

      if(bPlay && !bHasFocus && !playInBackground)
        bPlay=false;

      if(bPlay && Controls==0)
        bPlay=false;

      if(bPlay && !ControlLoopsEnabled)
        bPlay=false;

      bool bHasEvents=PollEvents(&Event)>0;

      if(!bHasEvents)
      {
        if(bPlay)
        {
          static ulong LastTick = 0;
          UpdateTick();

          if(LastTick != Tick)
          {
            LastTick = Tick;
            truth Draw = false;

            for(int c = 0; c < Controls; ++c)
              if(ControlLoop[c]())
                Draw = true;

            /******************
             * the stand-by animation
             */
            ShowFPS();
            if(!bAllowFrameSkip){
              if(Draw)
                graphics::BlitDBToScreen();

              iDelayMS=iDefaultDelayMS;
            }else{
              iDelayMS = FrameSkipOrDraw();
            }

          }

          SDL_Delay(iDelayMS);
        }
        else
        {
          if(bHasFocus){
            iDelayMS=1000/30; //30 FPS on main menu just to not use too much CPU there. If one day it is animated, lower this properly.
            SDL_Delay(iDelayMS);
          }else{
            SDL_WaitEvent(&Event);DBGLN;
            ProcessMessage(&Event);DBGLN;
          }
        }
      }
    }

  }
}

uint globalwindowhandler::PollEvents(SDL_Event* pEvent)
{
  if(pEvent==NULL)
    pEvent=new SDL_Event();

  uint i=0;
  while(SDL_PollEvent(pEvent)){
    ProcessMessage(pEvent);
    i++;
  }

  return i;
}

v2 v2MousePos;
uint globalwindowhandler::UpdateMouse()
{
  /**
   * global didnt fix the wrong mouse position relatively to the visible cursor...
  if(SDL_GetWindowFlags(graphics::GetWindow()) & SDL_WINDOW_FULLSCREEN_DESKTOP)
    return SDL_GetGlobalMouseState(&v2MousePos.X,&v2MousePos.Y);
  else
   */
    return SDL_GetMouseState(&v2MousePos.X,&v2MousePos.Y);
}

int globalwindowhandler::ReadKey()
{
  SDL_Event Event;

#if SDL_MAJOR_VERSION == 1
  if(SDL_GetAppState() & SDL_APPACTIVE)
#else
  if( playInBackground || (SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS)) )
#endif
  {
    PollEvents(&Event);
  }
  else
  {
    SDL_WaitEvent(&Event);
    ProcessMessage(&Event);
  }

  return KeyBuffer.size() ? GetKey(false) : 0;
}

truth globalwindowhandler::WaitForKeyEvent(uint Key)
{
  SDL_Event Event;

#if SDL_MAJOR_VERSION == 1
  if(SDL_GetAppState() & SDL_APPACTIVE)
#else
  if( playInBackground || (SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS)) )
#endif
  {
#if SDL_MAJOR_VERSION == 2
    while(SDL_PollEvent(&Event))
      if(Event.type == Key)
        return true;
#else
    while(SDL_PollEvent(&Event))
      if(Event.active.type == Key)
        return true;
#endif
  }
  else
    SDL_WaitEvent(&Event);

  return false;
}

truth globalwindowhandler::IsLastSDLkeyEventWasKeyUp()
{
  return bLastSDLkeyEventIsKeyUp;
}

v2 globalwindowhandler::GetMouseLocation()
{
  UpdateMouse();
  return v2MousePos;
}

bool globalwindowhandler::IsMouseAtRect(v2 v2TopLeft, v2 v2BorderOrBottomRigh, bool b2ndParmIsBorder, v2 v2MousePosOverride)
{
  v2 v2MP = v2MousePosOverride;
  if(v2MousePosOverride.Is0()){
    UpdateMouse();
    v2MP=v2MousePos;
  }

  v2 v2BottomRight = v2BorderOrBottomRigh;
  if(b2ndParmIsBorder)
    v2BottomRight += v2TopLeft;

  return
    v2MP.X > v2TopLeft.X     &&
    v2MP.Y > v2TopLeft.Y     &&
    v2MP.X < v2BottomRight.X &&
    v2MP.Y < v2BottomRight.Y    ;
}

mouseclick mc;
mouseclick globalwindowhandler::ConsumeMouseEvent() //TODO buffer it?
{
  mouseclick mcR;
  if(mc.btn!=-1 || mc.wheelY!=0)
    mcR=mc;

  mc.btn=-1;
  mc.pos=v2();
  mc.wheelY=0;

  return mcR;
}

int globalwindowhandler::ChkCtrlKey(SDL_Event* Event)
{
  if(Event->key.keysym.mod & KMOD_CTRL){ //if CTRL is pressed, user expects something else than the normal key, therefore not permissive
    if(ControlKeyHandler!=NULL)
      ControlKeyHandler(Event->key.keysym.sym);
    return iRestWaitKey; //gum TODO 0 should suffice one day...
  }DBGLN;

  return Event->key.keysym.sym;
}

void globalwindowhandler::ProcessKeyDownMessage(SDL_Event* Event)
{DBG4(Event->key.keysym.sym,Event->text.text[0],Event->key.keysym.mod & KMOD_ALT,Event->key.keysym.mod & KMOD_CTRL);

  bLastSDLkeyEventIsKeyUp=false;

  /**
   * Events are splitted between SDL_KEYDOWN and SDL_TEXTINPUT.
   *
   * All managed events must be explicited,
   * so, all keyDown events that will be modified must be handled here,
   * all other non modified keyDown events will be handled by SDL_TEXTINPUT event type outside here.
   *
   * More modifiers also means higher priority.
   *
   * if one or more modifiers are pressed,
   * user expects something else than the normal key,
   * therefore wont fill the key buffer
   *
   * Non handled ctrl+alt+... or ctrl+... or alt+... will be ignored.
   * Tho, they may be overriden by the OS and never reach here...
   */

  if((Event->key.keysym.mod & KMOD_CTRL) && (Event->key.keysym.mod & KMOD_ALT)){
    switch(Event->key.keysym.sym)
    {
    case SDLK_e:
      /**
       * TODO
       * exemplify where this is or can be ever used as tests provided no results on Linux,
       * is it the windows Explorer key? if so #ifdef WIN32 should be used...
       */
      AddKeyToBuffer('\177');
      break;
    }
    return;
  }

  if(Event->key.keysym.mod & KMOD_CTRL){
    if(ControlKeyHandler!=NULL) //this one was completely externalized
      ControlKeyHandler(Event->key.keysym.sym);
    return;
  }else
  if(Event->key.keysym.mod & KMOD_ALT){
    switch(Event->key.keysym.sym)
    {
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      graphics::SwitchMode();
      break;
    }
    return;
  }

  // other special non buffered keys
  switch(Event->key.keysym.sym)
  {
    case SDLK_F1:    case SDLK_F2:    case SDLK_F3:    case SDLK_F4:    case SDLK_F5:
    case SDLK_F6:    case SDLK_F7:    case SDLK_F8:    case SDLK_F9:    case SDLK_F10:
    case SDLK_F11:   case SDLK_F12:   case SDLK_F13:   case SDLK_F14:   case SDLK_F15:
    case SDLK_F16:   case SDLK_F17:   case SDLK_F18:   case SDLK_F19:   case SDLK_F20:
    case SDLK_F21:   case SDLK_F22:   case SDLK_F23:   case SDLK_F24:
      if(FunctionKeyHandler!=NULL)
        FunctionKeyHandler(Event->key.keysym.sym);
      return; //no buffer

    case SDLK_SYSREQ:
    case SDLK_PRINTSCREEN:
      if(!ScrshotDirectoryName.IsEmpty())
        DOUBLE_BUFFER->Save(ScrshotNameHandler());
      return; //no buffer
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////// MODIFIED KEY BUFFER ///////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  int KeyPressed = 0;
  switch(Event->key.keysym.sym)
  {
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      // both SDL keys are mixed into KEY_ENTER
      KeyPressed = KEY_ENTER; //TODO SDL1? old comment tip or deadCode: Event->key.keysym.unicode;
      break;

    case SDLK_DOWN:
    case SDLK_KP_2:
      KeyPressed = KEY_DOWN + 0xE000;
      break;

    case SDLK_UP:
    case SDLK_KP_8:
      KeyPressed = KEY_UP + 0xE000;
      break;

    case SDLK_RIGHT:
    case SDLK_KP_6:
      KeyPressed = KEY_RIGHT + 0xE000;
      break;

    case SDLK_LEFT:
    case SDLK_KP_4:
      KeyPressed = KEY_LEFT + 0xE000;
      break;

    case SDLK_HOME:
    case SDLK_KP_7:
      KeyPressed = KEY_HOME + 0xE000;
      break;

    case SDLK_END:
    case SDLK_KP_1:
      KeyPressed = KEY_END + 0xE000;
      break;
    case SDLK_PAGEUP:
    case SDLK_KP_9:
      KeyPressed = KEY_PAGE_UP + 0xE000;
      break;

    case SDLK_KP_3:
    case SDLK_PAGEDOWN:
      KeyPressed = KEY_PAGE_DOWN + 0xE000;
      break;

    case SDLK_KP_5:
      KeyPressed = iRestWaitKey;
      break;

#if SDL_MAJOR_VERSION == 2 //TODO there is no ESC on SDL1??? but does SDL1 still compiles? anyone uses it yet??? the same question about DJGPP...
   case SDLK_ESCAPE:
   case SDLK_BACKSPACE:
     KeyPressed = Event->key.keysym.sym;
     break;
#endif

#if SDL_MAJOR_VERSION == 1
   default:
    KeyPressed = Event->key.keysym.unicode;

    if(!KeyPressed)
      return;
#endif
  }
  AddKeyToBuffer(KeyPressed);
}

/**
 * buffer of yet non processed commands/textInput
 */
void globalwindowhandler::AddKeyToBuffer(int KeyPressed)
{ DBG1(KeyPressed);
  if(KeyPressed==0)return;

  if( std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed) == KeyBuffer.end() ) //prevent dups TODO because of fast key-repeat OS feature? should be the last key on buffer only then
    KeyBuffer.push_back(KeyPressed);
}

void globalwindowhandler::ProcessMessage(SDL_Event* Event)
{
  Uint32 type;
#if SDL_MAJOR_VERSION == 1
  type=(Event->active.type);
#else
  type=(Event->type);
#endif

  switch(type)
  {

#if SDL_MAJOR_VERSION == 1
   case SDL_VIDEOEXPOSE:
    graphics::BlitDBToScreen();
#else
   case SDL_WINDOWEVENT:
    switch(Event->window.event)
    {
     case SDL_WINDOWEVENT_SHOWN:
     case SDL_WINDOWEVENT_RESIZED:
     case SDL_WINDOWEVENT_RESTORED:
      graphics::BlitDBToScreen();
      break;
    }
#endif
    break;

   case SDL_QUIT:
    if(!QuitMessageHandler || QuitMessageHandler())
      exit(0);
    return;

   case SDL_MOUSEBUTTONUP:
     if(Event->button.button==1 && Event->button.clicks>0){
       mc.btn = 1;
       mc.pos.X=Event->button.x;
       mc.pos.Y=Event->button.y;
     }
     break;

   case SDL_MOUSEWHEEL:
     mc.wheelY = Event->wheel.y;
     break;

#if SDL_MAJOR_VERSION == 2 //BEFORE key up or down
   case SDL_TEXTINPUT: DBG2(Event->key.keysym.sym,Event->text.text[0]);
     AddKeyToBuffer(Event->text.text[0]);
     break;
#endif

   case SDL_KEYUP: DBGLN;
     bLastSDLkeyEventIsKeyUp=true;
     break;

   case SDL_KEYDOWN: DBGLN;
     ProcessKeyDownMessage(Event);
     break;
  }

}

// returns true if shift is being pressed
// else false
truth globalwindowhandler::ShiftIsDown()
{
  return false;
}

// returns filename to be used for screenshot
festring globalwindowhandler::ScrshotNameHandler()
{
  static int ScrshotCount = 0;

  festring ScrshotNum;
  if (ScrshotCount < 10) // prepend 0s so that files are properly sorted in browser (up to 999 at least).
    ScrshotNum << "00" << ScrshotCount;
  else if (ScrshotCount < 100)
    ScrshotNum << "0" << ScrshotCount;
  else
    ScrshotNum << ScrshotCount;

  festring ScrshotName;
  ScrshotName << ScrshotDirectoryName << "Scrshot" << ScrshotNum << ".bmp";

  FILE* Scrshot = fopen(ScrshotName.CStr(), "r");

  if (Scrshot)
  {
    // file exists; close file and increment ScrshotCount
    fclose(Scrshot);
    ++ScrshotCount;
    return ScrshotNameHandler();
  }

  // if file doesn't exist; we can use this filename
  return ScrshotName;
}

#endif /* USE_SDL */
