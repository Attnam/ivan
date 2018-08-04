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

#include <iostream>

#ifdef __DJGPP__
#include <go32.h>
#include <sys/farptr.h>
#endif

#ifndef WIN32
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <execinfo.h>
#include <unistd.h>
#endif

#include "game.h"
#include "database.h"
#include "definesvalidator.h"
#include "devcons.h"
#include "feio.h"
#include "igraph.h"
#include "iconf.h"
#include "whandler.h"
#include "hscore.h"
#include "graphics.h"
#include "script.h"
#include "message.h"
#include "proto.h"
#include "audio.h"

#include "dbgmsgproj.h"

#include "bugworkaround.h"

#ifndef WIN32
void CrashHandler(int Signal)
{
  globalerrorhandler::DumpStackTraceToStdErr(Signal);
  exit(1);
}
#endif

void SkipGameScript(inputfile* pSaveFile){
  gamescript* gs=0;
  (*pSaveFile) >> gs; //dummy just to "seek" for next binary data TODO if dungeon and level was saved before it, this would not be necessary... re-structuring the savegame file (one day) would be good.
}

int main(int argc, char** argv)
{
#ifndef WIN32
  signal(SIGABRT, CrashHandler);
  signal(SIGBUS, CrashHandler);
  signal(SIGFPE, CrashHandler);
  signal(SIGILL, CrashHandler);
  signal(SIGINT, CrashHandler);
  signal(SIGSEGV, CrashHandler);
  signal(SIGSYS, CrashHandler);
  signal(SIGTERM, CrashHandler);
  signal(SIGTRAP, CrashHandler);
  signal(SIGQUIT, CrashHandler);
#endif

  game::GetHomeDir(); //just to properly initialize as soon as possible DBGMSG correct path b4 everywhere it may be used.

  if(argc > 1 && festring(argv[1]) == "--version")
  {
    std::cout << "Iter Vehemens ad Necem version " << IVAN_VERSION << std::endl;
    return 0;
  }

#ifdef __DJGPP__

  /* Saves numlock state and toggles it off */

  char ShiftByteState = _farpeekb(_dos_ds, 0x417);
  _farpokeb(_dos_ds, 0x417, 0);

#endif /* __DJGPP__ */

  audio::Init(game::GetMusicDir());

  femath::SetSeed(time(0));
  game::InitGlobalValueMap();
  scriptsystem::Initialize();
  databasesystem::Initialize();
  game::InitLuxTable();
  ivanconfig::Initialize();
  igraph::Init();
  game::CreateBusyAnimationCache();
  globalwindowhandler::SetQuitMessageHandler(game::HandleQuitMessage);
  globalwindowhandler::SetScrshotDirectory(game::GetScrshotDir());
  globalwindowhandler::SetDeveloperConsoleHandler(devcons::Command);
  globalwindowhandler::SetFilterHandler(game::FilterListQuestion);
  bugfixdp::init();
  definesvalidator::init();
  msgsystem::Init();
  protosystem::Initialize();
  igraph::LoadMenu();
  game::PrepareStretchRegionsLazy();

  /* Set off the main menu music */
  audio::SetPlaybackStatus(0);
  audio::ClearMIDIPlaylist();
  audio::LoadMIDIFile("mainmenu.mid", 0, 100);
  audio::SetPlaybackStatus(audio::PLAYING);

  for(int running = 1; running;)
  {
    int Select = iosystem::Menu(igraph::GetMenuGraphic(),
                                v2(RES.X / 2, RES.Y / 2 - 20),
                                CONST_S("\r"),
                                CONST_S("Start Game\r"
                                        "Continue Game\r"
                                        "Configuration\r"
                                        "Highscores\r"
                                        "Quit\r"),
                                LIGHT_GRAY,
                                CONST_S("Released under the GNU\r"
                                        "General Public License\r"
                                        "More info: see COPYING\r"),
                                CONST_S("IVAN v" IVAN_VERSION "\r"));

    switch(Select)
    {
     case 0:
      if(game::Init())
      {
        igraph::UnLoadMenu();

        game::Run();
        game::DeInit();
        igraph::LoadMenu();
      }

      break;
     case 1:
      {
        iosystem::SetSkipSeekSave(&SkipGameScript);
        festring LoadName = iosystem::ContinueMenu(WHITE, LIGHT_GRAY, game::GetSaveDir(), game::GetSaveFileVersion(), ivanconfig::IsAllowImportOldSavegame());

        if(LoadName.GetSize())
        {
          LoadName.Resize(LoadName.GetSize() - 4); // - ".sav"

          if(game::Init(LoadName))
          {
            igraph::UnLoadMenu();
            game::Run();
            game::DeInit();
            igraph::LoadMenu();
          }
        }

        break;
      }
     case 2:
      ivanconfig::Show();
      break;
     case 3:
      {
        highscore HScore(game::GetStateDir() + HIGH_SCORE_FILENAME);
        HScore.Draw();
        break;
      }
     case 4:

#ifdef __DJGPP__

      /* Loads numlock state */

      _farpokeb(_dos_ds, 0x417, ShiftByteState);

#endif

      running = 0;
      break;
    }
  }

  msgsystem::DeInit();

  return 0;
}
