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
#include <cstdlib>

#ifdef __DJGPP__
#include <go32.h>
#include <sys/farptr.h>
#endif

#ifdef BACKTRACE
#include <execinfo.h>
#endif

#ifndef WIN32
#include <csignal>
#include <cstring>
#include <cstdlib>
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
#include "specialkeys.h"
#include "message.h"
#include "proto.h"
#include "audio.h"

#include "dbgmsgproj.h"

#include "bugworkaround.h"

#ifdef BACKTRACE
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
#ifdef BACKTRACE
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

  GetUserDataDir(); //just to properly initialize as soon as possible DBGMSG correct path b4 everywhere it may be used.

  if(argc > 1 && festring(argv[1]) == "--version")
  {
    std::cout << "Iter Vehemens ad Necem version " << IVAN_VERSION << std::endl;
    return 0;
  }

  if(argc > 1 && festring(argv[1]) == "--defgen")
  {
    std::cout << "Generate defines validator file. " << std::endl;
    game::InitGlobalValueMap();
    std::cout << "DONE: InitGlobalValueMap()" << std::endl;
    definesvalidator::GenerateDefinesValidator("generate");
    std::cout << "Finished: Generate DefinesValidator" << std::endl;
    return 0;
  }

  if(argc > 1 && festring(argv[1]) == "--defval")
  {
    std::cout << "Validate defines. " << std::endl;
    game::InitGlobalValueMap();
    std::cout << "DONE: InitGlobalValueMap()" << std::endl;
    definesvalidator::GenerateDefinesValidator("validate");
    std::cout << "Finished: Validate defines" << std::endl;
    return 0;
  }

//  if(argc > 1 && festring(argv[1]) == "--genmvkeys")
//  {
//    festring fsFl = game::GetUserDataDir() + CUSTOM_KEYS_FILENAME;
//    FILE *fl = fopen(fsFl.CStr(), "wt"); //"a");
//    for(int i=0;i<=8;i++){
//      switch(i){
//        case 0: std::cout << "Upper Left" << std::endl; break;
//        case 1: std::cout << "Up" << std::endl; break;
//        case 2: std::cout << "Upper Right" << std::endl; break;
//        case 3: std::cout << "Left" << std::endl; break;
//        case 4: std::cout << "Right" << std::endl; break;
//        case 5: std::cout << "Lower Left" << std::endl; break;
//        case 6: std::cout << "Down" << std::endl; break;
//        case 7: std::cout << "Lower Right" << std::endl; break;
//        case 8: std::cout << "Stop" << std::endl; break;
//      }
//      //fprintf(fl, "%04d\n", globalwindowhandler::GetKey());
//      fprintf(fl, "%04X\n", globalwindowhandler::ReadKey());
//    }
//    fclose(fl);
//  }
  
  if(argc > 1 && festring(argv[1]) == "--help")
  {
    std::cout << "Command line options:" << std::endl;
    std::cout << "--defgen Generate defines validator source file. " << std::endl;
    std::cout << "--defval Validate defines. " << std::endl;
//    std::cout << "--genmvkeys Generate custom move keys cfg file. " << std::endl;
    std::cout << "--version Show current game version. " << std::endl;
    std::cout << std::endl;
    std::cout << "Environment Variables:" << std::endl;
    std::cout << "IVAN_SHOWFPS=[true] # show FPS at top right" << std::endl;
    std::cout << "IVAN_DebugShowTinyDungeon=[true] #DEBUG always show tiny dungeon above stretched one" << std::endl;
    std::cout << "IVAN_LISTDRAWABOVE=[true] #DEBUG output the draw above priority list to text console terminal" << std::endl;
    std::cout << "IVAN_DebugGenDungeonLevelLoopID=[DungeonLevelIndex] #DEBUG DungeonLevelIndex must be an integer matching a some dungeon level" << std::endl;
    std::cout << "IVAN_DebugGenDungeonLevelLoopMax=[integer] #DEBUG generate the dungeon level how many times" << std::endl;
#ifdef WIZARD    
    std::cout << "IVAN_DebugStayOnDungeonLevel=[DungeonLevelIndex] #DEBUG wizard auto play AI will not leave that Dungeon Level after entering it" << std::endl;
#endif
#ifdef CURSEDDEVELOPER
    std::cout << "IVAN_CURSEDDEVELOPER=[true] #DEBUG (only for developer tests) this will prevent player's death (and scoring) in normal (non wizard) gameplay" << std::endl;
#endif
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
  specialkeys::init();
  bugfixdp::init();
  devcons::Init();
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
        festring LoadName = iosystem::ContinueMenu(WHITE, LIGHT_GRAY, game::GetSaveDir(), game::GetSaveFileVersionHardcoded(), ivanconfig::IsAllowImportOldSavegame());

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
        highscore HScore(GetUserDataDir() + HIGH_SCORE_FILENAME);
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
