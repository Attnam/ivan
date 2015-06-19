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

#include "whandler.h"
#include "graphics.h"
#include "error.h"
#include "bitmap.h"
#include "festring.h"

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

    if(Key == K_Control_Print)
    {
      DOUBLE_BUFFER->Save(festring(ScrshotNameHandler()));
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

int globalwindowhandler::GetKey(truth EmptyBuffer)
{
  SDL_Event Event;

  if(EmptyBuffer)
  {
    while(SDL_PollEvent(&Event))
      ProcessMessage(&Event);

    KeyBuffer.clear();
  }

  for(;;)
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
      if(SDL_PollEvent(&Event))
        ProcessMessage(&Event);
      else
      {
#if SDL_MAJOR_VERSION == 1
        if(SDL_GetAppState() & SDL_APPACTIVE
#else
        if(SDL_GetWindowFlags(graphics::Window) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS)
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

            if(Draw)
              graphics::BlitDBToScreen();
          }

          SDL_Delay(10);
        }
        else
        {
          SDL_WaitEvent(&Event);
          ProcessMessage(&Event);
        }
      }
    }
}

int globalwindowhandler::ReadKey()
{
  SDL_Event Event;
  memset(&Event,0,sizeof(SDL_Event));
#if SDL_MAJOR_VERSION == 1
  if(SDL_GetAppState() & SDL_APPACTIVE)
#else
  if(SDL_GetWindowFlags(graphics::Window) & (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS))
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
   case SDL_KEYDOWN:
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

      DOUBLE_BUFFER->Save(festring(ScrshotNameHandler()));
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
    if(std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed)
       == KeyBuffer.end())
      KeyBuffer.push_back(KeyPressed);
    break;
#if SDL_MAJOR_VERSION == 2
   case SDL_TEXTINPUT:
     KeyPressed = Event->text.text[0];
     if(std::find(KeyBuffer.begin(), KeyBuffer.end(), KeyPressed)
        == KeyBuffer.end())
       KeyBuffer.push_back(KeyPressed);
#endif
  }
}

// returns true if shift is being pressed
// else false
truth globalwindowhandler::ShiftIsDown() {
  return false;

}

festring globalwindowhandler::ScrshotNameHandler() { // returns filename to be used for screenshot
	static int ScrshotCount = 0;

	festring ScrshotNum;
	if (ScrshotCount < 10) // prepend 0s so that files are properly sorted in browser (up to 999 at least).
		ScrshotNum << "00" << ScrshotCount;
	else if (ScrshotCount < 100)
		ScrshotNum << "0" << ScrshotCount;
	else
		ScrshotNum << ScrshotCount;
	
	festring ScrshotName;
	#ifdef WIN32
	ScrshotName << "Scrshot/Scrshot" << ScrshotNum << ".bmp";
	#else
	ScrshotName << festring(getenv("HOME")) << "/IvanScrshot/Scrshot" << ScrshotNum << ".bmp";
	#endif
	
	FILE* Scrshot = fopen(ScrshotName.CStr(), "r");
	if (Scrshot) {
    // file exists; close file and increment ScrshotCount
		fclose(Scrshot);
		++ScrshotCount;
		return ScrshotNameHandler();
	} 

    // if file doesn't exist; we can use this filename
	return ScrshotName;
}
#endif

