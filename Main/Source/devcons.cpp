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

#include "SDL.h"

//#include "bugworkaround.h"
#include "char.h"
#include "devcons.h"
#include "error.h"
#include "feio.h"
#include "felist.h"
#include "festring.h"
#include "game.h"
#include "message.h"
#include "stack.h"
#include "specialkeys.h"

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
void ListChars(festring fsFilter){
  ulong idFilter=0;
  if(!fsFilter.IsEmpty())
    idFilter=atoi(fsFilter.CStr());

  DEVCMDMSG("params: %d",idFilter);

//  std::vector<character*> vc = game::GetAllCharacters();
//  for(int i=0;i<vc.size();i++){
  characteridmap map = game::GetCharacterIDMapCopy();
  for(characteridmap::iterator itr = map.begin();itr!=map.end();itr++){
    character* C = itr->second;
    if(idFilter!=0 && idFilter!=C->GetID())continue;

    festring fsPos="NULL";
    if(C->GetSquareUnder()!=NULL){
      fsPos.Empty();
      fsPos<<C->GetPos().X<<","<<C->GetPos().Y;
    }

    festring fsMsg;
    fsMsg << (C->IsPlayer()?"@":" ") <<
        "id="<<C->GetID()<<"["<<itr->first<<"] "<<
        "("<<fsPos<<") "<<
        "'"<<C->GetName(DEFINITE)<<"'";
    character* PB = C->GetPolymorphBackup();
    if(PB!=NULL)
      fsMsg << " PB='"<<PB->GetID() <<"/"<< PB->GetName(DEFINITE)<<"'";
    fsMsg << ".";
    DEVCMDMSG("%s",fsMsg.CStr());
//    DEVCMDMSG("%sid=%d[%d] (%s) '%s'.",
////    ADD_MESSAGE("%sid=%d (%d,%d) '%s'.",
//      C->IsPlayer()?"@":" ",
//      C->GetID(),
//      itr->first,
//      fsPos.CStr(),
//      C->GetName(DEFINITE).CStr()
//    );
  }
}
void ListItems(festring fsParams){
  ulong idCharFilter=0;
  ulong idFilter=0;

  if(!fsParams.IsEmpty()){
    std::string part;
    std::stringstream iss(fsParams.CStr());
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

  itemidmap map = game::GetItemIDMapCopy();
  for(itemidmap::iterator itr = map.begin();itr!=map.end();itr++){
//  return Iterator != ItemIDMap.end() ? Iterator->second : 0;
//  std::vector<item*> vc = game::GetAllItems();
//  for(int i=0;i<vc.size();i++){
//    item* it = vc[i]; //helps on debugging too
    item* it = itr->second; //helps on debugging too

    //TODO could check app memory range if pointer is within limits...
    if( //TODO items could have a random key to detect if they were not deleted improperly or w/e, could still segfault when reading it tho...
      it->GetVolume()==0 ||
      it->GetID()==0 ||
      it->GetSquaresUnder()==0 ||
      it->GetSquaresUnder()>100 || //something improbable, could be just 8 I guess...
      false
    ){
      DEVCMDMSG("item REFERENCE INVALID at consistent list ID=%d 0x%X",itr->first,it); //was the item deleted or what happened?
    }

    if(idFilter!=0 && idFilter!=it->GetID())
      continue;

    slot* Slot = it->GetSlot();

    const entity* ent;
    festring fsType;
    if(dynamic_cast<gearslot*>(Slot)!=NULL){
      ent=((gearslot*)Slot)->FindCarrier();
      fsType="gear";
    }else
    if(dynamic_cast<bodypartslot*>(Slot)!=NULL){
      ent=((bodypartslot*)Slot)->GetMaster();
      fsType="bodypart";
    }else
    if(dynamic_cast<stackslot*>(Slot)!=NULL){
      stackslot* sl = ((stackslot*)Slot);
      ent=sl->FindCarrier();
      if(sl->GetMotherStack()!=NULL)
        ent=sl->GetMotherStack()->GetMotherEntity();
      fsType="stack";
    }else
      ent=NULL;

    festring fsDec;
    citem* entI;
    ccharacter* entC;
    if(dynamic_cast<citem*>(ent)){
      entI=(citem*)ent;
      entC=NULL;
      if(dynamic_cast<const corpse*>(ent)){
        const corpse* CP = (const corpse*)ent;
        entC = CP->GetDeceased();
        fsDec=",Dec";
      }
    }else
    if(dynamic_cast<ccharacter*>(ent)){
      entI=NULL;
      entC=(ccharacter*)ent;
    }else{
      entI=NULL;
      entC=NULL;
    }

    if(idCharFilter!=0)
      if(entC==NULL || entC->GetID()!=idCharFilter)
        continue;

    bool bPlayerStuff = entC!=NULL && entC->IsPlayer();

    festring fsPB;
    if(entC!=NULL && entC->GetPolymorphBackup()!=NULL && entC->GetPolymorphBackup()->IsPlayer())
      fsPB=",PB";

    festring fsPos="NULL";
    if(it->GetSquareUnder()!=NULL){
      fsPos.Empty();
      fsPos<<it->GetPos().X<<","<<it->GetPos().Y;
    }

    DEVCMDMSG("%sid=%d (%s) '%s' owner '%d/%s' '%d/%s' (%s%s%s).",
      bPlayerStuff?"@":" ",
      it->GetID(),

      fsPos.CStr(),

      it->GetName(DEFINITE).CStr(),

      entC!=NULL ? entC->GetID() : 0,
      entC!=NULL ? entC->GetName(DEFINITE).CStr() : "NoEntC",

      entI!=NULL ? entI->GetID() : 0,
      entI!=NULL ? entI->GetName(DEFINITE).CStr() : "NoEntI",

      fsType.CStr(),
      fsPB.CStr(),
      fsDec.CStr()
    );
  }
}
#endif

void devcons::Init()
{
  specialkeys::AddCtrlOrFuncKeyHandler(SDLK_BACKQUOTE,&OpenCommandsConsole);
}

const int iVarTot=10;
float afVars[iVarTot];
void devcons::SetVar(festring fsParams)
{
  if(!fsParams.IsEmpty()){
    std::string part;
    std::stringstream iss(fsParams.CStr());
    
    iss >> part;
    int index = atoi(part.c_str()); //TODO use string IDs instead of index and create a map
    if(index<0 || index>=iVarTot)
      ABORT("invalid var index %d",index);
    
    iss >> part;
    int value = atof(part.c_str());
    
    afVars[index]=value;
  }
}
float devcons::GetVar(int index,float fDefaultIf0) 
{
  static bool bDummyInit = [](){for(int i=0;i<iVarTot;i++)afVars[i]=0;return true;}();
  if(index<0 || index>=iVarTot)
    ABORT("invalid var index %d",index);
  return afVars[index] == 0 ? fDefaultIf0 : afVars[index];
}

bool bOpenCommandsConsole=false;
void devcons::OpenCommandsConsole()
{
  static bool bDummyInit = [](){
    #define ADDCMD(cm,sh,bw) AddDevCmd(#cm,cm,sh,bw);
    ADDCMD(Help,"show this help",false);
    AddDevCmd("?",Help,"show this help",false);
#ifdef WIZARD
    ADDCMD(SetVar,festring()<<"<index> <floatValue> set a float variable index (max "<<(iVarTot-1)<<") to be used on debug",true);
    ADDCMD(ListChars,"[filterCharID:ulong] list all characters on current dungeon level",true);
    ADDCMD(ListItems,"[[c|i] <filterID:ulong>] filter by char or item ID. List all items on current dungeon level, including on characters inventory and containers",true);
#endif
    return true;
  }();

  //  if(felist::isAnyFelistCurrentlyDrawn())    return;
  //  if(iosystem::IsInUse())    return;
  //  if(iosystem::IsOnMenu())    return;
  //  if(game::IsQuestionMode())    return;
  //  if(game::IsInGetCommand())    return;
  //  if(specialkeys::IsConsumingEvent())    return;
  //  if(bugfixdp::IsAlertConfirmFixMsgDraw) return;
  /**
   * can only be opened if nothing else is being done,
   * if waiting some global player command,
   * this is better than all other specific checks.
   */
  if(!game::IsInGetCommand())
    return;

  if(bOpenCommandsConsole)
    return;

  bOpenCommandsConsole=true;
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
  bOpenCommandsConsole=false;
}

//typedef void (*callcmd)(festring&);

struct DevCmd{
  festring fsCmd="";
  festring fsCmdLowerCase="";
  festring fsHelp="";
  callcmd Call=NULL;
  bool bWizardModeOnly=false;
};

std::vector<DevCmd> vCmd;

void devcons::AddDevCmd(festring fsCmd, callcmd Call, festring fsHelp,bool bWizardModeOnly)
{
  callcmd cc = Find(fsCmd.CStr());
  if(cc!=NULL) //TODO ignore if equal?
    ABORT("command %s already set %p %p",fsCmd.CStr(),cc,Call);

  DevCmd dc;
  dc.fsCmd=fsCmd.CStr();
  
  std::string strCmdLowerCase=fsCmd.CStr();
  std::transform(strCmdLowerCase.begin(), strCmdLowerCase.end(), strCmdLowerCase.begin(), ::tolower);
  dc.fsCmdLowerCase=strCmdLowerCase.c_str();
  
  dc.fsHelp=fsHelp.CStr();
  dc.Call = Call;
  dc.bWizardModeOnly=bWizardModeOnly;

  int i=std::string(dc.fsCmd.CStr()).find(" ");
  if(i!=std::string::npos)
    ABORT("command must not contain spaces '%s'",dc.fsCmd.CStr());

  vCmd.push_back(dc);
}

void devcons::Help(festring fsFilter)
{
  festring fsWM;
  for(int j=0;j<vCmd.size();j++){
    if(!vCmd[j].bWizardModeOnly || game::WizardModeIsActive()){
      if(vCmd[j].bWizardModeOnly){
        fsWM="(WIZ) ";
      }else{
        fsWM="";
      }
    }
      DEVCMDMSG("%s - %s%s",vCmd[j].fsCmd.CStr(),fsWM.CStr(),vCmd[j].fsHelp.CStr());
  }
//  ADD_MESSAGE("%sPs.: main commands are case insensitive.",cPrompt);
  DEVCMDMSG("%s","Ps.: main commands are case insensitive.");
}

callcmd devcons::Find(festring fsCmd)
{
  std::string strCmd = fsCmd.CStr();
  std::transform(strCmd.begin(), strCmd.end(), strCmd.begin(), ::tolower);
  for(int j=0;j<vCmd.size();j++){
    if(vCmd[j].fsCmdLowerCase==strCmd.c_str())
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
  DEVCMDMSG("Trying to run: %s ('%s' '%s')",
    festring(strFullCmd.c_str()).CStr(),
    festring(strCmd.c_str()).CStr(),
    festring(strParams.c_str()).CStr()
  );

  callcmd cc = Find(strCmd.c_str());
  if(cc){
    cc(strParams.c_str());
//    ADD_MESSAGE("%scommand %s completed",cPrompt,strCmd.c_str());
    DEVCMDMSG("command %s completed",strCmd.c_str());
  }else{
//    ADD_MESSAGE("%scommand %s not found",cPrompt,strCmd.c_str());
    DEVCMDMSG("command %s not found",strCmd.c_str());
  }
}
