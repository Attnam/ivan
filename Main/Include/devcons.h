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

typedef void (*callcmd)(std::string);

class devcons{
    static callcmd Find(std::string strCmd);
    static void Help(std::string strFilter);
    static void runCommand(festring fsCmd);
  public:
    static void Command();
    static void AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp=festring());
};

#endif /* MAIN_INCLUDE_DEVCONS_H_ */
