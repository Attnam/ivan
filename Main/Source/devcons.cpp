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

#include <algorithm>

#include "devcons.h"
#include "error.h"
#include "game.h"
#include "message.h"

/**
 * Attention!
 *
 * Non-wizard commands are intended ONLY to easify fixing the game,
 * help the player get un-stuck,
 * workaround some glitch like a blocked path that shouldnt be,
 * etc.
 *
 * Any information that provides an advantage must be considered WIZARD MODE!!!
 */

void devcons::Command()
{
  static bool bDummyInit = [](){
    AddDevCmd("help",Help);
    return true;
  }();

  for(;;){
    festring fsFullCmd;
    festring fsQ;
    if(game::WizardModeIsActive())
      fsQ="Developer(wizard) ";
    fsQ<<"Console Command: (type 'help' for options)";
    if(game::StringQuestion(fsFullCmd, fsQ, WHITE, 1, 255, true) == NORMAL_EXIT){
      runCommand(fsFullCmd);
    }else
      break;
  }
}

typedef void (*callcmd)(std::string);

struct DevCmd{
  std::string strCmd="";
  std::string strHelp="";
  callcmd Call=NULL;
};

std::vector<DevCmd> vCmd;

void devcons::AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp)
{
  callcmd cc = Find(fsCmd.CStr());
  if(cc!=NULL) //TODO ignore if equal?
    ABORT("command %s already set %x %x",fsCmd.CStr(),cc,Call);

  DevCmd dc;
  dc.strCmd=fsCmd.CStr();
  std::transform(dc.strCmd.begin(), dc.strCmd.end(), dc.strCmd.begin(), ::tolower);
  dc.strHelp=fsHelp.CStr();
  dc.Call = Call;

  int i=dc.strCmd.find(" ");
  if(i!=std::string::npos)
    ABORT("command must not contain spaces '%s'",dc.strCmd.c_str());

  vCmd.push_back(dc);
}

void devcons::Help(std::string strFilter)
{
  for(int j=0;j<vCmd.size();j++){
    ADD_MESSAGE("%s - %s",vCmd[j].strCmd.c_str(),vCmd[j].strHelp.c_str());
  }
  ADD_MESSAGE("Ps.: main commands are case insensitive.");
}

callcmd devcons::Find(std::string strCmd)
{
  std::transform(strCmd.begin(), strCmd.end(), strCmd.begin(), ::tolower);
  for(int j=0;j<vCmd.size();j++)
    if(vCmd[j].strCmd==strCmd)
      return vCmd[j].Call;
  return NULL;
}

void devcons::runCommand(festring fsFullCmd)
{
  std::string strFullCmd = fsFullCmd.CStr();
  if(strFullCmd.size()==0)
    return;

  std::string strCmd;
  std::string strParams;

  int i=strFullCmd.find(" ");
  if(i==std::string::npos)
    strCmd = strFullCmd;
  else{
    strCmd = strFullCmd.substr(0,i);
    std::string strParams = strFullCmd.substr(i+1);
  }

  callcmd cc = Find(strCmd);
  if(cc){
    cc(strParams);
    ADD_MESSAGE("command %s completed",strCmd.c_str());
  }else{
    ADD_MESSAGE("command %s not found",strCmd.c_str());
  }
}
