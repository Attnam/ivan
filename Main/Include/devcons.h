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

#ifndef MAIN_INCLUDE_DEVCONS_H_
#define MAIN_INCLUDE_DEVCONS_H_

#include <string>

#include "festring.h"

#define DEVCMDMSG(fmt,x,...) ADD_MESSAGE(" > " fmt,x);

typedef void (*callcmd)(std::string);

class devcons{
    static callcmd Find(std::string strCmd);
    static void Help(std::string strFilter);
    static void runCommand(festring fsCmd);
  public:
    static void Init();
    static void OpenCommandsConsole();
    static void AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp=festring(), bool bWizardModeOnly=false);
    static void SetVar(std::string strParams);
    static float GetVar(int iIndex,float fDefaultIf0);
};

#endif /* MAIN_INCLUDE_DEVCONS_H_ */
