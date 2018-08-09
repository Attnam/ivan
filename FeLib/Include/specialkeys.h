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

#ifndef MAIN_INCLUDE_SPECIALKEYS_H_
#define MAIN_INCLUDE_SPECIALKEYS_H_

#include "SDL.h"

class festring;

typedef void (*specialkeyhandler)();

class specialkeys
{
 public:
  enum SKEvent{
    Filter,
    ClearStringInput,
    CopyToClipboard,
    PasteFromClipboard,
    FocusedElementHelp,
  };

  static void init();

  static bool FunctionKeyHandler(SDL_Keycode);
  static bool ControlKeyHandler(SDL_Keycode);
  static void AddCtrlOrFuncKeyHandler(SDL_Keycode key, specialkeyhandler Handler);

  static cfestring FilterListQuestion(cfestring What);
  static void ClearRequest(){Request=-1;}

  static bool ConsumeEvent(SKEvent k){festring fsDummy;return ConsumeEvent(k,fsDummy);}
  static bool ConsumeEvent(SKEvent,festring& fsInOut);
  static bool IsConsumingEvent();
  static bool IsRequestedEvent(SKEvent e);
  static bool HasEvent(){return Request>=0;}

 private:
  static int Request;
};


#endif /* MAIN_INCLUDE_SPECIALKEYS_H_ */
