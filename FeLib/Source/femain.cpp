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

#ifdef __DJGPP__
#include <conio.h>
#include "graphics.h"
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <iostream>
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

#include <cstdlib>

#include "error.h"

int Main(int, char**);

int main(int argc, char* argv[])
{
  /*try
  {*/
    return Main(argc, argv);
  /*}
  catch(...)
  {
    cchar* Msg = "Fatal Error: Unknown exception thrown.";
#ifdef WIN32
    ShowWindow(GetActiveWindow(), SW_HIDE);
    char Buffer[256];
    strcpy(Buffer, Msg);
    strcat(Buffer, globalerrorhandler::GetBugMsg());
    MessageBox(NULL, Buffer, "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#endif
#ifdef LINUX
    std::cout << Msg << globalerrorhandler::GetBugMsg() << std::endl;
#endif
#ifdef __DJGPP__
    graphics::DeInit();
    std::cout << Msg << globalerrorhandler::GetBugMsg() << std::endl;
#endif
    exit(3);
  }

  exit(0);*/
}
