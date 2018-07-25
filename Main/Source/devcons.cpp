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
#include <sstream>

#include "char.h"
#include "devcons.h"
#include "error.h"
#include "game.h"
#include "message.h"
#include "stack.h"

/**
 * ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!!
 * ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!!
 * ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!! ATTENTION!!!
 *
 * Non-wizard commands are intended ONLY to easify fixing the game,
 * help the player get un-stuck if ever happens,
 * workaround some glitch like a blocked path that shouldnt be,
 * etc.
 *
 * Any information/functionality that provides an advantage must be considered a cheat, therefore WIZARD MODE!!!
 */

#ifdef WIZARD
void ListChars(std::string strFilter){
  ulong idFilter=0;
  if(!strFilter.empty())atoi(strFilter.c_str());

  DEVCMDMSG("params: %d",idFilter);

  std::vector<character*> vc = game::GetAllCharacters();
  for(int i=0;i<vc.size();i++){
    if(idFilter!=0 && idFilter!=vc[i]->GetID())continue;

    festring fsPos="NULL";
    if(vc[i]->GetSquareUnder()!=NULL){
      fsPos.Empty();
      fsPos<<vc[i]->GetPos().X<<","<<vc[i]->GetPos().Y;
    }
    DEVCMDMSG("%sid=%d (%s) '%s'.",
//    ADD_MESSAGE("%sid=%d (%d,%d) '%s'.",
      vc[i]->IsPlayer()?"@":" ",
      vc[i]->GetID(),
      fsPos.CStr(),
      vc[i]->GetName(DEFINITE).CStr()
    );
  }
}
void ListItems(std::string strParams){
  ulong idCharFilter=0;
  ulong idFilter=0;

  if(!strParams.empty()){
    std::string part;
    std::stringstream iss(strParams);
    if(iss >> part){
      if(part=="c"){
        if(iss >> part)
          idCharFilter=atoi(part.c_str());
      }
      if(part=="i"){
        if(iss >> part)
          idFilter=atoi(part.c_str());
      }
    }
  }

  DEVCMDMSG("params: %d %d",idFilter,idCharFilter);

  std::vector<item*> vc = game::GetAllItems();
  for(int i=0;i<vc.size();i++){
    if(idFilter!=0 && idFilter!=vc[i]->GetID())
      continue;

    slot* Slot = vc[i]->GetSlot();

    ccharacter* CC;
    entity* ent=NULL;
    if(dynamic_cast<gearslot*>(Slot)!=NULL)
      CC=((gearslot*)Slot)->FindCarrier();
    else
    if(dynamic_cast<bodypart*>(Slot)!=NULL)
      CC=((bodypart*)Slot)->FindCarrier();
    else
    if(dynamic_cast<stackslot*>(Slot)!=NULL){
      CC=((stackslot*)Slot)->FindCarrier();
      stackslot* sl = ((stackslot*)Slot);
      if(sl->GetMotherStack()!=NULL)
        ent=sl->GetMotherStack()->GetMotherEntity();
    }else
      CC=NULL;

    if(CC!=NULL && idCharFilter!=0 && CC->GetID()!=idCharFilter)
      continue;

    character* entC=NULL;
    item* entI=NULL;
    if(dynamic_cast<item*>(ent))
      entI=(item*)ent;
    if(dynamic_cast<character*>(ent))
      entC=(character*)ent;

    bool bPlayerStuff = CC==PLAYER;

    festring fsPos="NULL";
    if(vc[i]->GetSquareUnder()!=NULL){
      fsPos.Empty();
      fsPos<<vc[i]->GetPos().X<<","<<vc[i]->GetPos().Y;
    }

    DEVCMDMSG("%sid=%d (%s) '%s' owner '%s' '%s'.",
      bPlayerStuff?"@":" ",
      vc[i]->GetID(),
      fsPos.CStr(),
      vc[i]->GetName(DEFINITE).CStr(),
      entC!=NULL ? entC->GetName(DEFINITE).CStr() : "NoEntC",
      entI!=NULL ? entI->GetName(DEFINITE).CStr() : "NoEntI"
    );
  }
}
#endif

void devcons::Command()
{
  static bool bDummyInit = [](){
    #define ADDCMD(cm,sh,bw) AddDevCmd(#cm,cm,sh,bw);
    ADDCMD(Help,"show this help",false);
    AddDevCmd("?",Help,"show this help",false);
#ifdef WIZARD
    ADDCMD(ListChars,"[filterCharID:ulong] list all characters on current dungeon level",true);
    ADDCMD(ListItems,"[[c|i] <filterID:ulong>] filter by char or item ID. List all items on current dungeon level, including on characters inventory and containers",true);
#endif
    return true;
  }();

  for(;;){
    static festring fsFullCmd;
    festring fsQ;
    if(game::WizardModeIsActive())
      fsQ="Developer(WIZ) ";
    fsQ<<"Console Command (try 'help' or '?'):";
    //TODO key up/down commands history and save/load to a txt file
    if(game::StringQuestion(fsFullCmd, fsQ, WHITE, 1, 255, true) == NORMAL_EXIT){
      runCommand(fsFullCmd);
      msgsystem::DrawMessageHistory();
    }else
      break;
  }
}

typedef void (*callcmd)(std::string);

struct DevCmd{
  std::string strCmd="";
  std::string strCmdLowerCase="";
  std::string strHelp="";
  callcmd Call=NULL;
  bool bWizardModeOnly=false;
};

std::vector<DevCmd> vCmd;

void devcons::AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp,bool bWizardModeOnly)
{
  callcmd cc = Find(fsCmd.CStr());
  if(cc!=NULL) //TODO ignore if equal?
    ABORT("command %s already set %x %x",fsCmd.CStr(),cc,Call);

  DevCmd dc;
  dc.strCmd=fsCmd.CStr();
  dc.strCmdLowerCase=fsCmd.CStr();
  std::transform(dc.strCmdLowerCase.begin(), dc.strCmdLowerCase.end(), dc.strCmdLowerCase.begin(), ::tolower);
  dc.strHelp=fsHelp.CStr();
  dc.Call = Call;
  dc.bWizardModeOnly=bWizardModeOnly;

  int i=dc.strCmd.find(" ");
  if(i!=std::string::npos)
    ABORT("command must not contain spaces '%s'",dc.strCmd.c_str());

  vCmd.push_back(dc);
}

void devcons::Help(std::string strFilter)
{
  for(int j=0;j<vCmd.size();j++){
    if(!vCmd[j].bWizardModeOnly || game::WizardModeIsActive())
      DEVCMDMSG("%s - %s%s",
//      ADD_MESSAGE("%s%s - %s%s",cPrompt,
        vCmd[j].strCmd.c_str(),
        vCmd[j].bWizardModeOnly?"(WIZ) ":"",
        vCmd[j].strHelp.c_str());
  }
//  ADD_MESSAGE("%sPs.: main commands are case insensitive.",cPrompt);
  DEVCMDMSG("%s","Ps.: main commands are case insensitive.");
}

callcmd devcons::Find(std::string strCmd)
{
  std::transform(strCmd.begin(), strCmd.end(), strCmd.begin(), ::tolower);
  for(int j=0;j<vCmd.size();j++){
    if(vCmd[j].strCmdLowerCase==strCmd)
      return vCmd[j].Call;
  }
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
    strParams = strFullCmd.substr(i+1);
  }

//  ADD_MESSAGE("%sTrying to run: %s ('%s' '%s')",cPrompt,strFullCmd.c_str(),strCmd.c_str(),strParams.c_str());
  DEVCMDMSG("Trying to run: %s ('%s' '%s')",strFullCmd.c_str(),strCmd.c_str(),strParams.c_str());

  callcmd cc = Find(strCmd);
  if(cc){
    cc(strParams);
//    ADD_MESSAGE("%scommand %s completed",cPrompt,strCmd.c_str());
    DEVCMDMSG("command %s completed",strCmd.c_str());
  }else{
//    ADD_MESSAGE("%scommand %s not found",cPrompt,strCmd.c_str());
    DEVCMDMSG("command %s not found",strCmd.c_str());
  }
}
