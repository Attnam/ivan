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

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "bitmap.h"
#include "feio.h"
#include "error.h"
#include "festring.h"
#include "graphics.h"
#include "rawbit.h"
#include "specialkeys.h"
#include "whandler.h"

#include "dbgmsgproj.h"

/**
 * They modify current behavior of anything that accepts such modifications,
 * or
 * they may override whatever is happening with something else.
 */

//#undef REQ
//#define REQ(name) \
//  bool b##name##Request=false; \
//  void specialkeys::Clear##name##Request(){b##name##Requested=false;}
//REQ(Filter);
//REQ(FocusedElementHelp);
//REQ(CopyToClipboard);
//REQ(PasteFromClipboard);

cfestring specialkeys::FilterListQuestion(cfestring what)
{
  festring What=what;

  v2 pos = v2(16, 6);
  static festring Topic = "Type this list filter:";

  int R = iosystem::StringQuestion(What, Topic, pos, WHITE, 0, 30, false /*TODO !bGameIsRunning*/, true, NULL);

  /**
   * TODO
   * clear the filter text using the background!
   * needed when pressing ESC or ENTER and not changing the filter!
   * tip: igraph::BlitBackGround(pos, v2(RES.X, 23));
   * also, could may be copy the background from doublebuffer and just paste it back after....
   */

  // cheap workaround to not look too bad (like input was not accepted) at least
  //  DOUBLE_BUFFER->Fill(pos,v2(RES.X, 23),BLACK);
  FONT->Printf(DOUBLE_BUFFER, pos, BLACK, "%s", Topic.CStr());
  FONT->Printf(DOUBLE_BUFFER, v2(pos.X, pos.Y + 10), BLACK, "%s_", What.CStr());

  if(R == NORMAL_EXIT){ DBG1(What.CStr());
    return What;
  }

  if(R == ABORTED)
    return what;

  return cfestring();
}

bool specialkeys::IsRequestedEvent(SKEvent e)
{
  return (e==Request);
}

std::vector<festring> afsHelpDialog;
void DrawHelpDialog(bitmap* Buffer) //TODO this kind'o message should be more global to be easier to be used...
{ //TODO create a buffer to not re-draw every loop... unless want to animate it...
  if(afsHelpDialog.size()==0)return;

  int iFontLetterWidth=8;
  int iLH=15;

  v2 v2Border;
  v2Border.Y=afsHelpDialog.size()*iLH+iLH*2;
  for(int i=0;i<afsHelpDialog.size();i++){
    int iW = afsHelpDialog[i].GetSize()*iFontLetterWidth;
    if(v2Border.X<iW)v2Border.X=iW;
  }

  v2Border.X+=iLH*2;

  v2 v2TL(RES.X/2-v2Border.X/2,RES.Y/2-v2Border.Y/2);

  Buffer->Fill(v2TL,v2Border,BLACK);
  graphics::DrawRectangleOutlineAround(Buffer, v2TL, v2Border, DARK_GRAY, true);

  v2TL+=v2(iLH,iLH);

  for(int i=0;i<afsHelpDialog.size();i++)
    FONT->Printf(Buffer, v2(v2TL.X,v2TL.Y+i*iLH), WHITE, "%s", afsHelpDialog[i].CStr());
}

bool bConsumingEvent=false;
bool specialkeys::IsConsumingEvent()
{
  return bConsumingEvent;
}

int specialkeys::Request=-1;
bool specialkeys::ConsumeEvent(SKEvent e,festring& fsInOut){DBGLN;
  if(!IsRequestedEvent(e))
    return false;

  bConsumingEvent=true;

  switch(e){
    case Filter:{DBGLN;
      globalwindowhandler::SuspendKeyTimeout();
      fsInOut=FilterListQuestion(fsInOut);
      globalwindowhandler::ResumeKeyTimeout();
  //    bFilterRequest=false;
      Request=-1;
    };break;

    case CopyToClipboard:{DBGLN;
      #ifdef UNIX //TODO works for MACOSX too?
        std::ostringstream osStkCmd;
        osStkCmd<<"echo -n '"<<fsInOut.CStr()<<"' |xclip -i";
        FILE* pipeFile = popen(osStkCmd.str().c_str(),"r"); //TODO exec pipelessly
        if(pipeFile!=NULL){
          pclose(pipeFile);
        }else{
          DBG2("unable to execute popen() with cmd: ",osStkCmd.str());
        }
      #endif
      //TODO windows (can be hardcoded)
    };break;

    case PasteFromClipboard:{DBGLN; //TODO iosystem::StringQuestion? globalwindowhandler::GetKey at keybuffer?
      #ifdef UNIX //TODO works for MACOSX too?
        std::ostringstream osStkCmd;
        osStkCmd<<"xclip -o";
        FILE* pipeFile = popen(osStkCmd.str().c_str(),"r");
        if(pipeFile!=NULL){
          static const int i=10*1024;
          char buf[i];
          if(fread(buf,1,i,pipeFile)>0)
            fsInOut = buf;
          pclose(pipeFile);
        }else{
          DBG2("unable to execute popen() with cmd: ",osStkCmd.str());
        }
      #endif
      //TODO windows (can be hardcoded)
    };break;

    case FocusedElementHelp:{DBGLN;
      globalwindowhandler::SuspendKeyTimeout();

      std::stringstream ss(fsInOut.CStr());
      std::string line;
      while(std::getline(ss,line,'\n'))
        afsHelpDialog.push_back(festring(line.c_str()));

      graphics::BlitDBToScreen();DBGLN;

      GET_KEY(true);DBGLN;

      afsHelpDialog.clear();

      globalwindowhandler::ResumeKeyTimeout();

      Request=-1; //it is com
    };break;
  }

  bConsumingEvent=false;

  return true;
}

void specialkeys::init()
{
  globalwindowhandler::SetFunctionKeyHandler(specialkeys::FunctionKeyHandler);
  globalwindowhandler::SetControlKeyHandler(specialkeys::ControlKeyHandler);

  graphics::AddDrawAboveAll(&DrawHelpDialog,90000,"HelpDialog");
}

typedef std::map<SDL_Keycode,specialkeyhandler> ckhmap;
ckhmap CkhMap;

bool specialkeys::FunctionKeyHandler(SDL_Keycode key)
{DBGLN;
  switch(key){ //TODO how to not use SDLK_ keys here??? shouldnt anyway????
  case SDLK_F1:DBGLN;
    Request=FocusedElementHelp;
    return true;
  default:
    ckhmap::iterator Iterator = CkhMap.find(key);
    if(Iterator != CkhMap.end()){
      Iterator->second();
      return true;
    }
    break;
  }
  return false;
}

/**
 * add Function or Ctrl+ key handler
 */
void specialkeys::AddCtrlOrFuncKeyHandler(SDL_Keycode key, specialkeyhandler Handler)
{
  ckhmap::iterator Iterator = CkhMap.find(key);
  if(Iterator != CkhMap.end())
    ABORT("control key handler already set for key %d",key);

  CkhMap.insert(std::make_pair(key,Handler));
}

/**
 * Ctrl+FunctionKey is here tho
 */
bool specialkeys::ControlKeyHandler(SDL_Keycode key)
{
  switch(key){ //TODO use SDLK_ keys?
  case 'f':
    Request=Filter;
    return true;
  case 'c':
    Request=CopyToClipboard;
    return true;
  case 'v':
    Request=PasteFromClipboard;
    return true;
  default:
    ckhmap::iterator Iterator = CkhMap.find(key);
    if(Iterator != CkhMap.end()){
      Iterator->second();
      return true;
    }
    break;
  }

  return false;
}

