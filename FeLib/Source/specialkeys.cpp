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
#include <algorithm>

#include "bitmap.h"
#include "feio.h"
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

cfestring specialkeys::FilterListQuestion()
{
  static festring What;
  int R = iosystem::StringQuestion(What, CONST_S("Type this list filter:"), v2(16, 6), WHITE, 0, 30, false /*TODO !bGameIsRunning*/, true, NULL);
  if(R == NORMAL_EXIT){ DBG1(What.CStr());
    return What;
  }

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

int specialkeys::Request=-1;
bool specialkeys::ConsumeEvent(SKEvent e,festring& fsInOut){DBGLN;
  if(!IsRequestedEvent(e))
    return false;

  switch(e){
    case Filter:{DBGLN;
      globalwindowhandler::SuspendKeyTimeout();
      fsInOut=FilterListQuestion();
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

  return true;
}

void specialkeys::init()
{
  globalwindowhandler::SetFunctionKeyHandler(specialkeys::FunctionKeyHandler);
  globalwindowhandler::SetControlKeyHandler(specialkeys::ControlKeyHandler);

  graphics::AddDrawAboveAll(&DrawHelpDialog,90000,"HelpDialog");
}

bool specialkeys::FunctionKeyHandler(SDL_Keycode key)
{DBGLN;
  switch(key){ //TODO how to not use SDLK_ keys here??? shouldnt anyway????
  case SDLK_F1:DBGLN;
    Request=FocusedElementHelp;
    return true;
  }
  return false;
}

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
  }
  return false;
}

