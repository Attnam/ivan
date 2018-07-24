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

#ifndef __WHANDLER_H__
#define __WHANDLER_H__

#ifdef USE_SDL
#include <vector>
#include "SDL.h"
#endif

#ifdef __DJGPP__
#include <ctime>
#endif

#include "felibdef.h"
#include "festring.h"

#define GET_KEY globalwindowhandler::GetKey
#define READ_KEY globalwindowhandler::ReadKey
#define GET_TICK globalwindowhandler::GetTick
#define WAIT_FOR_KEY_DOWN globalwindowhandler::WaitForKeyDown
#define WAIT_FOR_KEY_UP globalwindowhandler::WaitForKeyUp

struct mouseclick{
 int btn=-1;
 v2 pos;
 int wheelY=0;
};

class globalwindowhandler
{
 public:
  static bool IsKeyPressed(int iSDLScanCode);
  static void ResetKeyTimeout(){SetKeyTimeout(0,'.');}
  static void CheckKeyTimeout();
  static truth IsKeyTimeoutEnabled();
  static void SetKeyTimeout(int iTimeoutMillis,int iDefaultReturnedKey);
  static mouseclick ConsumeMouseEvent();
  static void SetPlayInBackground(truth b){playInBackground=b;}
  static float GetFPS(bool bInsta);
  static truth HasKeysOnBuffer();
  static uint PollEvents(SDL_Event* pEvent = NULL);
  static uint UpdateMouse();
  static int GetKey(truth = true);
  static int ReadKey();
  static truth WaitForKeyEvent(uint Key);
  static truth WaitForKeyDown(){return WaitForKeyEvent(SDL_KEYDOWN);}
  static truth WaitForKeyUp  (){return WaitForKeyEvent(SDL_KEYUP  );}
  static v2 GetMouseLocation();
  static bool IsMouseAtRect(v2, v2, bool = true, v2 = v2());
  static truth IsLastSDLkeyEventWasKeyUp();
  static void InstallControlLoop(truth (*)());
  static void DeInstallControlLoop(truth (*)());
  static ulong GetTick() { return Tick; }
  static truth ControlLoopsInstalled() { return Controls; }
  static void EnableControlLoops() { ControlLoopsEnabled = true; }
  static void DisableControlLoops() { ControlLoopsEnabled = false; }
  static truth ShiftIsDown();
  static void SetScrshotDirectory(cfestring& DirectoryName){ ScrshotDirectoryName = DirectoryName; }
  static festring ScrshotNameHandler(); // Number successive screenshots based on existing filenames
  static void SetAddFrameSkip(int i);
#ifdef USE_SDL
  static void Init();
  static void SetQuitMessageHandler(truth (*What)()){ QuitMessageHandler = What; }
  static ulong UpdateTick() { return Tick = SDL_GetTicks() / 40; }
#endif
  static void SetDeveloperConsoleHandler(void (*What)()){ CmdDevConsHandler = What; }
#ifdef __DJGPP__
  static void Init() { }
  static void SetQuitMessageHandler(truth (*)()) { }
  static ulong UpdateTick() { return Tick = uclock() * 25 / UCLOCKS_PER_SEC; }
#endif
 private:
#ifdef USE_SDL
  static void ProcessMessage(SDL_Event*);
  static std::vector<int> KeyBuffer;
  static truth (*QuitMessageHandler)();
#endif
  static truth (*ControlLoop[MAX_CONTROLS])();
  static int Controls;
  static ulong Tick;
  static truth ControlLoopsEnabled;
  static truth playInBackground;
  static festring ScrshotDirectoryName;
  static void (*CmdDevConsHandler)();
};

#endif
