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

#ifndef __ERROR_H__
#define __ERROR_H__

#include "felibdef.h"

#define ABORT globalerrorhandler::Abort

#ifdef __DJGPP__
#define SIGNALS 8
#endif

class undefinedConfigurationSoughtException 
{
  public:
    static truth IsGenNewLvl(){return bGeneratingNewDungeonLevel;}
    static truth ToggleGenNewLvl(){return bGeneratingNewDungeonLevel = !bGeneratingNewDungeonLevel;return bGeneratingNewDungeonLevel;}
  private:
    static truth bGeneratingNewDungeonLevel;
};

class globalerrorhandler
{
 public:
#ifndef WIN32
  static void DumpStackTraceToStdErr(int Signal = -1);
#endif
  static void Install();
  static void DeInstall();
  [[noreturn]] static void LIKE_PRINTF(1, 2) Abort(cchar*, ...);
  static cchar* GetBugMsg() { return BugMsg; }
 private:
  static cchar* BugMsg;
#ifdef VC
  static int NewHandler(size_t);
  static int (*OldNewHandler)(size_t);
#else
  static void NewHandler();
  static void (*OldNewHandler)();
#endif
#ifdef __DJGPP__
  static void SignalHandler(int);
  static void (*OldSignal[SIGNALS])(int);
  static int Signal[SIGNALS];
#endif
};

#endif
