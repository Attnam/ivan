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

// this causes warnings with LGTM checks: #define DEVCMDMSG(fmt,x,...) ADD_MESSAGE(" > " fmt,x);
#define DEVCMDMSGTAG " > "
#define DEVCMDMSG1P(fmt,x) ADD_MESSAGE(DEVCMDMSGTAG fmt,x);
#define DEVCMDMSG2P(fmt,x,y) ADD_MESSAGE(DEVCMDMSGTAG fmt,x,y);
#define DEVCMDMSG3P(fmt,x,y,z) ADD_MESSAGE(DEVCMDMSGTAG fmt,x,y,z);
#define DEVCMDMSG4P(fmt,x,y,z,a) ADD_MESSAGE(DEVCMDMSGTAG fmt,x,y,z,a);
#define DEVCMDMSG15P(fmt,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) ADD_MESSAGE(DEVCMDMSGTAG fmt,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o);

typedef void (*callcmd)(festring);

class devcons{
    static callcmd Find(festring fsCmd);
    static void Help(festring fsFilter);
    static void runCommand(festring fsCmd);
  public:
    static void Init();
    static void OpenCommandsConsole();
    static void AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp=festring(), bool bWizardModeOnly=false);
    static void SetVar(festring fsParams);
    static float GetVar(int iIndex,float fDefaultIf0);
};

#endif /* MAIN_INCLUDE_DEVCONS_H_ */
