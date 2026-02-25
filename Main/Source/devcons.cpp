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
#include "confdef.h"
#include "lterras.h"
#include "dbgmsgproj.h"

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

std::vector<character*> vCharLastSearch;
std::vector<item*>      vItemLastSearch;

#ifdef WIZARD
#ifdef DBGMSG
void DbgSetVar(festring fsParams){
  if(!fsParams.IsEmpty()){
    std::string part;
    std::stringstream iss(fsParams.CStr());
    
    iss >> part;
    std::string strId=part;
    
    iss >> part;
    std::string strValue=part;
    
    DBGSETV(strId,strValue);
    DEVCMDMSG2P("DBG ID='%s' Value='%s'",strId.c_str(),strValue.c_str());
  }
}
#endif //DBGMSG
truth IsValidChar(character* C){
  if(!C->Exists())
    return false;
  if(C->IsDead())
    return false;
  if(!C->GetLSquareUnder())
    return false;
  if(!C->IsEnabled())
    return false;
  return true;
}
void TeleToChar(festring fsFilter){
  if(!game::WizardModeIsReallyActive())return;
  
  characteridmap map = game::GetCharacterIDMapCopy();
  for(characteridmap::iterator itr = map.begin();itr!=map.end();itr++){
    character* C = itr->second;
    if(!IsValidChar(C))
      continue;
    
    if(C->GetName(DEFINITE).Find(fsFilter)!=festring::NPos){
      game::GetPlayer()->TeleportNear(C);
      return;
    }
  }
}
void TeleToMe(festring fsFilter){
  if(!game::WizardModeIsReallyActive())return;
  
  characteridmap map = game::GetCharacterIDMapCopy();
  for(characteridmap::iterator itr = map.begin();itr!=map.end();itr++){
    character* C = itr->second;
    if(!IsValidChar(C))
      continue;
    
    if(C->GetName(DEFINITE).Find(fsFilter)!=festring::NPos){
      C->TeleportNear(game::GetPlayer());
      DEVCMDMSG2P("%d %s",C,C->GetName(DEFINITE).CStr());
    }
  }
}
void FillWithWalls(festring fsFilter){
  if(!game::WizardModeIsReallyActive())return;
  
  int iAround=0;
  if(!fsFilter.IsEmpty())
    iAround=atoi(fsFilter.CStr());
  int iCount=0;
  ulong Tick = game::GetLOSTick();
  v2 v2PPos=game::GetPlayer()->GetPos();
  for(int iY=0;iY<game::GetCurrentLevel()->GetYSize();iY++){ for(int iX=0;iX<game::GetCurrentLevel()->GetXSize();iX++){
    lsquare* lsqr = game::GetCurrentLevel()->GetLSquare(iX,iY);

    if(lsqr->GetOLTerrain())continue;
    if(lsqr->GetCharacter())continue;
    if(!(lsqr->GetGLTerrain()->GetWalkability() & WALK))continue;
    if(iAround>0){
      if(lsqr->GetPos().X > v2PPos.X+iAround)continue;
      if(lsqr->GetPos().X < v2PPos.X-iAround)continue;
      if(lsqr->GetPos().Y > v2PPos.Y+iAround)continue;
      if(lsqr->GetPos().Y < v2PPos.Y-iAround)continue;
    }
    
    lsqr->ChangeOLTerrainAndUpdateLights(wall::Spawn(STONE_WALL));
    lsqr->Reveal(Tick);
    iCount++;
  }}
  DEVCMDMSG1P("new walls: %d",iCount);
}
void ListChars(festring fsFilter){
  if(!game::WizardModeIsReallyActive())return;
  
  ulong idFilter=0;
  if(!fsFilter.IsEmpty())
    idFilter=atoi(fsFilter.CStr());

  DEVCMDMSG2P("params: NameHas=\"%s\" or ID=%d",fsFilter.CStr(),idFilter);

//  std::vector<character*> vc = game::GetAllCharacters();
//  for(int i=0;i<vc.size();i++){
  characteridmap map = game::GetCharacterIDMapCopy();
  vCharLastSearch.clear();
  for(characteridmap::iterator itr = map.begin();itr!=map.end();itr++){
    character* C = itr->second;
    if(!IsValidChar(C))
      continue;
    
    if(idFilter!=0){
      if(idFilter!=C->GetID())
        continue;
    }else{
      if(C->GetName(DEFINITE).Find(fsFilter)==festring::NPos)
        continue;
    }

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
    DEVCMDMSG1P("%s",fsMsg.CStr());
    vCharLastSearch.push_back(C);
  }
  DEVCMDMSG1P("total:%d",vCharLastSearch.size());
}
void DelChars(festring fsParams){
  if(!game::WizardModeIsReallyActive())return;
  
  ulong count=0;
  if(!fsParams.IsEmpty())
    count=atoi(fsParams.CStr());
  
  if(count==0 || count>vCharLastSearch.size())
    count=vCharLastSearch.size();

  DEVCMDMSG1P("params: count=%d",count);
  
  int iRm=0;
  for(int i=count-1;i>=0;i--){
    character* C = vCharLastSearch[i];
    if(!C->Exists() || C->IsPlayer() || C->IsPet())continue;
    DEVCMDMSG2P("Go to hell! id=%d name=\"%s\"",C->GetID(),C->GetName(DEFINITE).CStr());
    C->Die();
    vCharLastSearch.pop_back();
    iRm++;
  }
  DEVCMDMSG2P("total=%d, remaining=%d",iRm,vCharLastSearch.size());
}
festring fsDummy;
entity* GetOwner(item* it,festring& rfsType = fsDummy){
  if(!it->GetLSquareUnder())
    return NULL;
  
  slot* Slot = it->GetSlot();
  if(!Slot->GetSquareUnder())
    return NULL;
  
  const entity* ent;
  if(dynamic_cast<gearslot*>(Slot)!=NULL){
    ent=((gearslot*)Slot)->FindCarrier();
    rfsType="gear";
  }else
  if(dynamic_cast<bodypartslot*>(Slot)!=NULL){
    ent=((bodypartslot*)Slot)->GetMaster();
    rfsType="bodypart";
  }else
  if(dynamic_cast<stackslot*>(Slot)!=NULL){
    stackslot* sl = ((stackslot*)Slot);
    if(sl->GetMotherStack()!=NULL)
      ent=sl->GetMotherStack()->GetMotherEntity();
    else
      ent=sl->FindCarrier();
    rfsType="stack";
  }else
    ent=NULL;
  
  return (entity*)ent;
}
character* GetOwnerChar(item* it,festring& rfsType = fsDummy){
  entity* ent = GetOwner(it,rfsType);
  if(dynamic_cast<character*>(ent)!=NULL)
    return (character*)ent;
  return NULL;
}
void DelItems(festring fsParams){
  if(!game::WizardModeIsReallyActive())return;
  
  ulong count=0;
  if(!fsParams.IsEmpty())
    count=atoi(fsParams.CStr());
  
  if(count==0 || count>vItemLastSearch.size())
    count=vItemLastSearch.size();

  DEVCMDMSG1P("params: count=%d",count);
  
  int iRm=0;
  item* it;
  character* C;
  for(int i=count-1;i>=0;i--){
    it = vItemLastSearch[i];
    if(!it->Exists())continue;
    
    C = GetOwnerChar(it);
    if(C && (C->IsPlayer() || C->IsPet()))continue;
    DEVCMDMSG2P("Go to hell! id=%d name=\"%s\"",it->GetID(),it->GetName(DEFINITE).CStr());
    it->RemoveFromSlot();
    it->SendToHell();
    vItemLastSearch.pop_back();
    iRm++;
  }
  DEVCMDMSG2P("total=%d, remaining=%d",iRm,vItemLastSearch.size());
}
void ListItems(festring fsParams){
  if(!game::WizardModeIsReallyActive())return;
  
  ulong idCharFilter=0;
  ulong idItemFilter=0;
  festring fsFilter;
  vCharLastSearch.clear();
  vItemLastSearch.clear();
  bool bItemMode=false;

  if(!fsParams.IsEmpty()){
    std::string part;
    std::stringstream iss(fsParams.CStr());
    if(iss >> part){
      if(part=="c"){
        if(iss >> part){
          fsFilter=part.c_str();
          idCharFilter=atoi(fsFilter.CStr());
        }
        bItemMode=false;
      }
      if(part=="i"){
        if(iss >> part){
          fsFilter=part.c_str();
          idItemFilter=atoi(fsFilter.CStr());
        }
        bItemMode=true;
      }
    }
  }

  DEVCMDMSG3P("params: ItemID=%d CharID=%d ItemOrCharNameHas=\"%s\"",idItemFilter,idCharFilter,fsFilter.CStr());

  itemidmap map = game::GetItemIDMapCopy();
  for(itemidmap::iterator itr = map.begin();itr!=map.end();itr++){
//  return Iterator != ItemIDMap.end() ? Iterator->second : 0;
//  std::vector<item*> vc = game::GetAllItems();
//  for(int i=0;i<vc.size();i++){
//    item* it = vc[i]; //helps on debugging too
    item* it = itr->second; //helps on debugging too

    //TODO could check app memory range if pointer is within limits...
    if( //TODO items could have a random key to detect if they were not deleted improperly or w/e, could still segfault when reading it tho...
      !it->Exists() ||
      it->GetVolume()==0 ||
      it->GetID()==0 ||
      it->GetLSquareUnder()==NULL ||
      it->GetSquaresUnder()==0 ||
      it->GetSquaresUnder()>100 || //something improbable, could be just 8 I guess...
      false //dummy
    ){
      DEVCMDMSG3P("item REFERENCE INVALID at consistent list ID=%d 0x%X \"%s\"",itr->first,it,it->GetName(DEFINITE).CStr()); //was the item deleted or what happened?
      continue;
    }

    if(bItemMode){
      if(idItemFilter!=0){
        if(idItemFilter!=it->GetID())
          continue;
      }else{
        if(it->GetName(DEFINITE).Find(fsFilter)==festring::NPos)
          continue;
      }
    }
      
    festring fsType;
    const entity* ent=GetOwner(it,fsType);

    festring fsDec;
    citem* ownerI=NULL;
    ccharacter* ownerC=NULL;
    if(dynamic_cast<citem*>(ent)){
      ownerI=(citem*)ent;
      ownerC=NULL;
      if(dynamic_cast<const corpse*>(ent)){
        const corpse* CP = (const corpse*)ent;
        ownerC = CP->GetDeceased();
        fsDec=",Dec";
      }
    }else
    if(dynamic_cast<ccharacter*>(ent)){
      ownerI=NULL;
      ownerC=(ccharacter*)ent;
    }else{
      ownerI=NULL;
      ownerC=NULL;
    }

    if(!bItemMode){
      if(idCharFilter!=0){
        if(ownerC==NULL || ownerC->GetID()!=idCharFilter)
          continue;
      }else{
        if(ownerC==NULL || ownerC->GetName(DEFINITE).Find(fsFilter)==festring::NPos)
          continue;
      }
    }

    bool bPlayerStuff = ownerC!=NULL && ownerC->IsPlayer();

    festring fsPB;
    if(ownerC!=NULL && ownerC->GetPolymorphBackup()!=NULL && ownerC->GetPolymorphBackup()->IsPlayer())
      fsPB=",PB";

    festring fsPos="NULL";
    if(it->GetSquareUnder()!=NULL){
      fsPos.Empty();
      fsPos<<it->GetPos().X<<","<<it->GetPos().Y;
    }

    DEVCMDMSG15P("%sid=%d (%s) '%s' owner '%d/%s' '%d/%s' (%s%s%s).",
      bPlayerStuff?"@":" ",
      it->GetID(),

      fsPos.CStr(),

      it->GetName(DEFINITE).CStr(),

      ownerC!=NULL ? ownerC->GetID() : 0,
      ownerC!=NULL ? ownerC->GetName(DEFINITE).CStr() : "NoEntC",

      ownerI!=NULL ? ownerI->GetID() : 0,
      ownerI!=NULL ? ownerI->GetName(DEFINITE).CStr() : "NoEntI",

      fsType.CStr(),
      fsPB.CStr(),
      fsDec.CStr(),
      
      0,0,0,0 //dummy
    );
    if(ownerC!=NULL){
      bool bSkip=false;
      for(auto pCchk = vCharLastSearch.begin(); pCchk != vCharLastSearch.end(); ++pCchk){
        bSkip = (*pCchk == ownerC);
        if(bSkip)break;
      }
      if(!bSkip)
        vCharLastSearch.push_back((character*)ownerC);
    }
    vItemLastSearch.push_back(it);
  }
  DEVCMDMSG2P("total: Chars=%d Items=%d",vCharLastSearch.size(),vItemLastSearch.size());
}
#endif //WIZARD

void devcons::Init()
{
  specialkeys::AddCtrlOrFuncKeyHandler(SDLK_BACKQUOTE,&OpenCommandsConsole);
}

const int iVarTot=10;
float afVars[iVarTot];
void devcons::SetVar(festring fsParams)
{
  if(!game::WizardModeIsReallyActive())return;
  
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
    ADDCMD(FillWithWalls,"[distance:int] all empty (of OLTerrain) squares (optionally around player max distance).",true);
    ADDCMD(DelChars,"[count:int] delete characters (from the list filled on the previous command) up to count if set.",true);
    ADDCMD(DelItems,"[count:int] delete items (from the list filled on the previous command) up to count if set.",true);
    ADDCMD(ListChars,"[[filterCharID:ulong]|[strCharNamePart:string]] List characters on current dungeon level",true);
    ADDCMD(ListItems,"[[c|i] <<filterID:ulong>|<filterName:string>>] List items on current dungeon level, including on characters ('c' will filter by character ID or name) inventory and containers",true);
    ADDCMD(SetVar,festring()<<"<index> <floatValue> set a float variable index (max "<<(iVarTot-1)<<") to be used on debug",true);
#ifdef DBGMSG
    ADDCMD(DbgSetVar,"<VarID:string> <VarValue:any> sets a DBGMSG variable.",true);
#endif //DBGMSG
    ADDCMD(TeleToChar,"<filterName:string> teleports near 1st character matching filter.",true);
    ADDCMD(TeleToMe,"<filterName:string> teleports all NPCs matching filter to you.",true);
#endif //WIZARD
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
    if(game::WizardModeIsReallyActive())
      fsQ="Developer(WIZ) ";
    fsQ<<"Console Command(s) separated by ';' (try 'help' or '?'):";
    //TODO key up/down commands history and save/load to a txt file
    if(game::StringQuestion(fsFullCmd, fsQ, WHITE, 1, 255, true) == NORMAL_EXIT){
      festring fsCmd;
      DBG1(fsFullCmd.CStr());
      for(;;){
        fsCmd=fsFullCmd;
        
        if(fsCmd.IsEmpty())
          break;
        
        int iPos=fsFullCmd.Find(";",0);
        if(iPos!=festring::NPos){ //found it
          fsCmd.Resize(iPos); //erases from ';' inclusive
          DBG1(fsCmd.CStr());
          fsFullCmd.Erase(0,iPos+1); //erases til ';' inclusive
          DBG1(fsFullCmd.CStr());
        }
        
        runCommand(fsCmd);
        msgsystem::DrawMessageHistory();
        
        if(iPos==festring::NPos) //no more commands to be run
          break;
      }
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
    if(vCmd[j].bWizardModeOnly){
      fsWM="(WIZ) ";
    }else{
      fsWM="";
    }
    DEVCMDMSG3P("%s - %s%s",vCmd[j].fsCmd.CStr(),fsWM.CStr(),vCmd[j].fsHelp.CStr());
  }
//  ADD_MESSAGE("%sPs.: main commands are case insensitive.",cPrompt);
  DEVCMDMSG1P("%s","Ps.: main commands are case insensitive.");
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
  DEVCMDMSG1P("%s",DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG DEVCMDMSGTAG );
  DEVCMDMSG3P("Trying to run: %s ('%s' '%s')",
    festring(strFullCmd.c_str()).CStr(),
    festring(strCmd.c_str()).CStr(),
    festring(strParams.c_str()).CStr()
  );

  callcmd cc = Find(strCmd.c_str());
  if(cc){
    cc(strParams.c_str());
//    ADD_MESSAGE("%scommand %s completed",cPrompt,strCmd.c_str());
    DEVCMDMSG1P(" <<< command %s completed",strCmd.c_str());
  }else{
//    ADD_MESSAGE("%scommand %s not found",cPrompt,strCmd.c_str());
    DEVCMDMSG1P(" <<< command %s not found",strCmd.c_str());
  }
}
