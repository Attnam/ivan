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

int iTimeoutDelay=0; // must init with 0
int iTimeoutDefaultKey;
long keyTimeoutRequestedAt;
/**
 * This is intended to remain active ONLY until the user hits any key.
 */
void globalwindowhandler::SetKeyTimeout(int iTimeoutMillis,int iDefaultReturnedKey)//,int iIgnoreKeyWhenDisabling)
{
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
    if(!KeyBuffer.empty()){ // user pressed some key
      keyTimeoutRequestedAt=clock(); // resets reference time to wait from
    }else{
      if( clock() > (keyTimeoutRequestedAt+iTimeoutDelay) ) //wait for the timeout to...
        KeyBuffer.push_back(iTimeoutDefaultKey); //...simulate the keypress
    }
  }
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
    while(SDL_PollEvent(&Event))
      ProcessMessage(&Event);

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
      bool bHasEvents=false;
      while(SDL_PollEvent(&Event)){
        ProcessMessage(&Event);
        bHasEvents=true;
      }

      if(!bHasEvents)
      {
#if SDL_MAJOR_VERSION == 1
        if(SDL_GetAppState() & SDL_APPACTIVE
#else
        if( (playInBackground || (SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS)))
#endif
           && Controls && ControlLoopsEnabled)
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
          SDL_WaitEvent(&Event);
          ProcessMessage(&Event);
        }
      }
    }

  }
}

int globalwindowhandler::ReadKey()
{
  SDL_Event Event;

#if SDL_MAJOR_VERSION == 1
  if(SDL_GetAppState() & SDL_APPACTIVE)
#else
  if(SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS))
#endif
  {
    while(SDL_PollEvent(&Event))
      ProcessMessage(&Event);
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
  if(SDL_GetWindowFlags(graphics::GetWindow()) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS))
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

void globalwindowhandler::ProcessMessage(SDL_Event* Event)
{
  int KeyPressed = 0;

#if SDL_MAJOR_VERSION == 1
  switch(Event->active.type)
#else
  switch(Event->type)
#endif
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

   case SDL_KEYUP:
    bLastSDLkeyEventIsKeyUp=true;
    break;
   case SDL_KEYDOWN:
    bLastSDLkeyEventIsKeyUp=false;
    switch(Event->key.keysym.sym)
    {
     case SDLK_RETURN:
     case SDLK_KP_ENTER:
      if(Event->key.keysym.mod & KMOD_ALT)
      {
        graphics::SwitchMode();
        return;
      }
      else
        KeyPressed = KEY_ENTER; //Event->key.keysym.unicode;

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
      KeyPressed = '.';
      break;
     case SDLK_SYSREQ:
     case SDLK_PRINTSCREEN:
      if(!ScrshotDirectoryName.IsEmpty())
      {
        DOUBLE_BUFFER->Save(ScrshotNameHandler());
      }
      return;
#if SDL_MAJOR_VERSION == 2
     /* event are now splitted between SDL_KEYDOWN and SDL_TEXTINPUT,
        all managed events must be explicited */
     case SDLK_ESCAPE:
     case SDLK_BACKSPACE:
      KeyPressed = Event->key.keysym.sym;
      break;
#endif

     case SDLK_e:
      if(Event->key.keysym.mod & KMOD_ALT
         && (Event->key.keysym.mod & KMOD_LCTRL
             || Event->key.keysym.mod & KMOD_RCTRL))
      {
        KeyPressed = '\177';
        break;
      }
     default:
#if SDL_MAJOR_VERSION == 1
      KeyPressed = Event->key.keysym.unicode;
#endif

      if(!KeyPressed)
        return;
    }
    if( std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed) == KeyBuffer.end() )
      KeyBuffer.push_back(KeyPressed);
    break;
#if SDL_MAJOR_VERSION == 2
   case SDL_TEXTINPUT:
    KeyPressed = Event->text.text[0];
    if( std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed) == KeyBuffer.end() )
      KeyBuffer.push_back(KeyPressed);
#endif
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
