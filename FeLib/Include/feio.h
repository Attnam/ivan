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

#ifndef __FEIO_H__
#define __FEIO_H__

#include "v2.h"
#include "festring.h"

class bitmap;
class inputfile;

typedef truth (*stringkeyhandler)(int, festring&);
typedef void (*bitmapeditor)(bitmap*, truth);
typedef void (*skipseeksave)(inputfile* pSaveFile);

class iosystem
{
 public:
  static festring ContinueMenu(col16, col16, cfestring&, const int iSaveFileVersion, bool bAllowImportOldSavegame);
  static void SetSkipSeekSave(skipseeksave);
  static void SetSaveGameSortMode(int i);
  static void fixChars(festring& fs);
  static int StringQuestion(festring&, cfestring&, v2, col16,
                            festring::sizetype, festring::sizetype,
                            truth, truth, stringkeyhandler = 0);
  static long NumberQuestion(cfestring&, v2, col16,
                             truth, truth = false);
  static long ScrollBarQuestion(cfestring&, v2, long, long, long,
                                long, long, col16, col16, col16, int,
                                int, truth, void (*)(long) = 0);
  static int Menu(std::vector<bitmap*> vBackGround, v2, cfestring&,
                  cfestring&, col16,
                  cfestring& = CONST_S(""),
                  cfestring& = CONST_S(""),
                  truth = false);
  static void TextScreen(cfestring&, v2 Disp = ZERO_V2,
                         col16 = 0xFFFF, truth = true,
                         truth = true, bitmapeditor = 0);
  static truth IsOnMenu();
  static bool IsInUse();
  static bool AlertConfirmMsg(const char* cMsg,std::vector<festring> vfsCritMsgs = std::vector<festring>(),bool bConfirmMode=true);
  static void AlertConfirmMsgDraw(bitmap* Buffer);
};

#endif
