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

#include <ctime>
#include <cctype>

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#ifdef WIN32
#define stat _stat
#include <io.h>
#endif

#ifdef UNIX
#include <dirent.h>
#include <stddef.h>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#endif

#ifdef __DJGPP__
#include <dir.h>
#endif

#include "bitmap.h"
#include "error.h"
#include "feio.h"
#include "felist.h"
#include "festring.h"
#include "graphics.h"
#include "rawbit.h"
#include "save.h"
#include "whandler.h"

#include "dbgmsgproj.h"

#define PENT_WIDTH 70

/* Prints screen full of Text in color Color. If GKey is true function
   waits for keypress. BitmapEditor is a pointer to function that is
   called during every fade tick. */

bool bInUse=false;
bool iosystem::IsInUse()
{
  return bInUse;
}

void iosystem::TextScreen(cfestring& Text, v2 Disp,
                          col16 Color, truth GKey, truth Fade,
                          bitmapeditor BitmapEditor)
{
  bInUse=true;

  bitmap Buffer(RES, 0);
  Buffer.ActivateFastFlag();
  festring::sizetype c;
  int LineNumber = 0;

  for(c = 0; c < Text.GetSize(); ++c)
    if(Text[c] == '\n')
      ++LineNumber;

  LineNumber >>= 1;
  char Line[200];
  int Lines = 0, LastBeginningOfLine = 0;

  for(c = 0; c < Text.GetSize(); ++c)
    if(Text[c] == '\n')
    {
      Line[c - LastBeginningOfLine] = 0;
      v2 PrintPos((RES.X >> 1) - (strlen(Line) << 2) + Disp.X,
                  (RES.Y << 1) / 5 - (LineNumber - Lines) * 15 + Disp.Y);
      FONT->Printf(&Buffer, PrintPos, Color, "%s", Line);
      ++Lines;
      LastBeginningOfLine = c + 1;
    }
    else
      Line[c - LastBeginningOfLine] = Text[c];

  Line[c - LastBeginningOfLine] = 0;
  v2 PrintPos((RES.X >> 1) - (strlen(Line) << 2) + Disp.X,
              (RES.Y << 1) / 5 - (LineNumber - Lines) * 15 + Disp.Y);
  FONT->Printf(&Buffer, PrintPos, Color, "%s", Line);

  if(Fade)
    Buffer.FadeToScreen(BitmapEditor);
  else
  {
    BitmapEditor(&Buffer, true);
    Buffer.FastBlit(DOUBLE_BUFFER);
    graphics::BlitDBToScreen();
  }

  if(GKey)
  {
    if(BitmapEditor)
      while(!READ_KEY())
        BitmapEditor(DOUBLE_BUFFER, false);
    else
      GET_KEY();
  }

  bInUse=false;
}

/* Returns amount of chars cSF in string sSH */

int CountChars(char cSF, cfestring& sSH)
{
  int iReturnCounter = 0;

  for(festring::sizetype i = 0; i < sSH.GetSize(); ++i)
    if(sSH[i] == cSF)
      ++iReturnCounter;

  return iReturnCounter;
}

truth bMenuIsActive=false;

truth iosystem::IsOnMenu(){
  return bMenuIsActive;
}

/* Draws a menu on bitmap BackGround to position Pos. festring Topic
   is the text that is shown before the choices '\r' is a line-ending
   character. Topic must end with a '\r'. sMS is a list of choices
   separated by '\r'. sMS must end with '\r'.

   Color is the col of font of sMS, SmallText1 and SmallText2. SmallText1
   is printed to the lower-left corner and SmallText2 is printed to the
   lower-right. They both can have line-ending characters ('\r') and must
   also always end with one. */

/* Warning: This function is utter garbage that just happens to work.
   If you need to use this function use the comments. Don't try to
   understand it. It is impossible. */

int iosystem::Menu(cbitmap* BackGround, v2 Pos,
                   cfestring& Topic, cfestring& sMS,
                   col16 Color, cfestring& SmallText1,
                   cfestring& SmallText2)
{
  if(CountChars('\r', sMS) < 1)
    return (-1);

  bMenuIsActive=true;
  truth bReady = false;
  int iSelected = 0;
  bitmap Backup(DOUBLE_BUFFER);
  Backup.ActivateFastFlag();
  bitmap Buffer(RES);
  Buffer.ActivateFastFlag();
  int c = 0;

  if(BackGround){
    if( (RES.X!=BackGround->GetSize().X) || (RES.Y!=BackGround->GetSize().Y) ){
      blitdata B = DEFAULT_BLITDATA;
      B.Bitmap = &Buffer;

      B.Src.X = (RES.X - BackGround->GetSize().X)/2;
      if(B.Src.X>0)B.Src.X=0;
      if(B.Src.X<0)B.Src.X*=-1;
      B.Src.Y = (RES.Y - BackGround->GetSize().Y)/2;
      if(B.Src.Y>0)B.Src.Y=0;
      if(B.Src.Y<0)B.Src.Y*=-1;

      B.Dest.X = (RES.X - BackGround->GetSize().X)/2;
      if(B.Dest.X<0)B.Dest.X=0;
      B.Dest.Y = (RES.Y - BackGround->GetSize().Y)/2;
      if(B.Dest.Y<0)B.Dest.Y=0;

      B.Border = BackGround->GetSize() - v2();

      Buffer.ClearToColor(0);
      BackGround->NormalBlit(B);
    }else{
      BackGround->FastBlit(&Buffer); //vanilla was 800x600 as the background menu image
    }
  }else
    Buffer.ClearToColor(0);

  festring sCopyOfMS;
  festring VeryUnGuruPrintf;

  while(!bReady)
  {
    clock_t StartTime = clock();
    sCopyOfMS = Topic;
    int i;

    for(i = 0; i < CountChars('\r', Topic); ++i)
    {
      festring::sizetype RPos = sCopyOfMS.Find('\r');
      VeryUnGuruPrintf = sCopyOfMS;
      VeryUnGuruPrintf.Resize(RPos);
      sCopyOfMS.Erase(0, RPos + 1);
      v2 PrintPos(Pos.X - (VeryUnGuruPrintf.GetSize() << 2),
                  Pos.Y - 30 - (CountChars('\r', Topic)
                                + CountChars('\r', sMS)) * 25 + i * 25);
      FONT->Printf(&Buffer, PrintPos, RED, "%s", VeryUnGuruPrintf.CStr());
    }

    sCopyOfMS = sMS;

    for(i = 0; i < CountChars('\r', sMS); ++i)
    {
      festring::sizetype RPos = sCopyOfMS.Find('\r');
      VeryUnGuruPrintf = sCopyOfMS;
      VeryUnGuruPrintf.Resize(RPos);
      sCopyOfMS.Erase(0, RPos + 1);
      int XPos = Pos.X - ((VeryUnGuruPrintf.GetSize() + 3) << 2);
      int YPos = Pos.Y - CountChars('\r', sMS) * 25 + i * 50;
      Buffer.Fill(XPos, YPos, ((VeryUnGuruPrintf.GetSize() + 3) << 3), 9, 0);

      if(i == iSelected)
        FONT->PrintfUnshaded(&Buffer, v2(XPos + 1, YPos + 1), WHITE,
                             "%d. %s", i + 1, VeryUnGuruPrintf.CStr());
      else
        FONT->Printf(&Buffer, v2(XPos, YPos), Color, "%d. %s",
                     i + 1, VeryUnGuruPrintf.CStr());

    }

    sCopyOfMS = SmallText1;

    for(i = 0; i < CountChars('\r', SmallText1); ++i)
    {
      festring::sizetype RPos = sCopyOfMS.Find('\r');
      VeryUnGuruPrintf = sCopyOfMS;
      VeryUnGuruPrintf.Resize(RPos);
      sCopyOfMS.Erase(0, RPos + 1);
      v2 PrintPos(3, RES.Y - CountChars('\r', SmallText1) * 10 + i * 10);
      FONT->Printf(&Buffer, PrintPos, Color, "%s", VeryUnGuruPrintf.CStr());

    }

    sCopyOfMS = SmallText2;

    for(i = 0; i < CountChars('\r', SmallText2); ++i)
    {
      festring::sizetype RPos = sCopyOfMS.Find('\r');
      VeryUnGuruPrintf = sCopyOfMS;
      VeryUnGuruPrintf.Resize(RPos);
      sCopyOfMS.Erase(0, RPos + 1);
      v2 PrintPos(RES.X - (VeryUnGuruPrintf.GetSize() << 3) - 2,
                  RES.Y - CountChars('\r', SmallText2) * 10 + i * 10);
      FONT->Printf(&Buffer, PrintPos, Color, "%s", VeryUnGuruPrintf.CStr());
    }

    int k;

    if(c < 5)
    {
      int Element = 127 - c * 25;
      blitdata BlitData = { DOUBLE_BUFFER,
                            { 0, 0 },
                            { 0, 0 },
                            { RES.X, RES.Y },
                            { MakeRGB24(Element, Element, Element) },
                            0,
                            0 };
      Backup.LuminanceMaskedBlit(BlitData);
      Buffer.SimpleAlphaBlit(DOUBLE_BUFFER, c++ * 50, 0);
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.05 * CLOCKS_PER_SEC);
      k = READ_KEY();
    }
    else
    {
                //FONT->Printf(&Buffer, v2(100, 100), Color, "%s", "NUKES IS HERE!");
      Buffer.FastBlit(DOUBLE_BUFFER);
      graphics::BlitDBToScreen();
      k = GET_KEY(false);
    }

    switch(k)
    {
     case KEY_UP:
      if(iSelected > 0)
        --iSelected;
      else
        iSelected = (CountChars('\r', sMS)-1);
      break;

     case KEY_DOWN:
      if(iSelected < (CountChars('\r', sMS)-1))
        ++iSelected;
      else
        iSelected = 0;
      break;

     case 0x00D:
      bReady = true;
      break;

     default:
      if(k > 0x30 && k < 0x31 + CountChars('\r', sMS)){
        bMenuIsActive=false;
        return k - 0x31;
      }
    }
  }

  bMenuIsActive=false;
  return iSelected;
}

/**
 * this prevents all possibly troublesome characters in all OSs
 */
void iosystem::fixChars(festring& fs)
{
  for(festring::sizetype i = 0; i < fs.GetSize(); ++i)
  {
    if(fs[i]>='A' && fs[i]<='Z')continue;
    if(fs[i]>='a' && fs[i]<='z')continue;
    if(fs[i]>='0' && fs[i]<='9')continue;

    fs[i] = '_';
  }
}

/* Asks the user a question requiring a string answer. The answer is saved
   to Input. Input can also already have a default something pretyped for
   the user. Topic is the question or other topic for the question. Pos is the
   coordinates of where the question is printed on the screen. Color is the
   col of all the fonts in this function. Enter is only accepted when the
   answer's length is between MinLetters and MaxLetters. If Fade is true the
   question is asked on a black background and the transition to that is a
   fade. If AllowExit is true the user can abort with the esc-key.

   The function returns ABORTED (when user aborts with esc) or
   NORMAL_EXIT. */

int iosystem::StringQuestion(festring& Input,
                             cfestring& Topic,
                             v2 Pos, col16 Color,
                             festring::sizetype MinLetters,
                             festring::sizetype MaxLetters,
                             truth Fade, truth AllowExit,
                             stringkeyhandler StringKeyHandler)
{
  bInUse=true;
  
  /**
   * History files are based on tokens to make them more shareable.
   * Questions (topic) must have '?' or ':' after the static string part and 
   * after that all can be dynamic.
   * So the files will be named up to these tokens only!
   * TODO This means some questions should be revised to share the history file for the very "same" base question...
   */
  festring fsFixTopicToFileName = Topic;
  ulong cutAt;
  cutAt = fsFixTopicToFileName.Find("?");
  if(cutAt!=festring::NPos)fsFixTopicToFileName.Resize(cutAt);
  cutAt = fsFixTopicToFileName.Find(":");
  if(cutAt!=festring::NPos)fsFixTopicToFileName.Resize(cutAt);
  fixChars(fsFixTopicToFileName);  
  festring fsHistFile = festring()+GetUserDataDir()+".QuestionHistory_"+fsFixTopicToFileName+".txt";
  DBG1(fsHistFile.CStr());
  FILE *fl = fopen(fsHistFile.CStr(), "a+");
  rewind(fl);
  festring Line;
  static const int iBuffSz=0xFF;
  char str[iBuffSz];
  std::vector<festring> vHist;
  while(fgets(str, iBuffSz, fl)){
    Line=str;
    Line.Resize(Line.GetSize()-1);
    vHist.push_back(Line); //removes trailing \n
  }
  
  v2 V(RES.X, 10); ///???????????
  bitmap BackUp(V, 0);
  blitdata B = { &BackUp,
                 { Pos.X, Pos.Y + 10 },
                 { 0, 0 },
                 { static_cast<int>((MaxLetters << 3) + 9), 10 },
                 { 0 },
                 0,
                 0 };

  if(Fade)
  {
    bitmap Buffer(RES, 0);
    Buffer.ActivateFastFlag();
    FONT->Printf(&Buffer, Pos, Color, "%s", Topic.CStr());
    FONT->Printf(&Buffer, v2(Pos.X, Pos.Y + 10), Color, "%s", Input.CStr());
    Buffer.FadeToScreen();
  }
  else
    DOUBLE_BUFFER->NormalBlit(B);

  truth TooShort = false;
  FONT->Printf(DOUBLE_BUFFER, Pos, Color, "%s", Topic.CStr());
  Swap(B.Src, B.Dest);

  bool bAbort = false;
  int iHistIndex = 0;
  if(vHist.size())iHistIndex=vHist.size()-1;
  for(int LastKey = 0, CursorPos = Input.GetSize();; LastKey = 0)
  {
    B.Bitmap = DOUBLE_BUFFER;
    BackUp.NormalBlit(B);
    FONT->Printf(DOUBLE_BUFFER, v2(Pos.X, Pos.Y + 10),
                 Color, "%s", Input.CStr());
    FONT->Printf(DOUBLE_BUFFER, v2(Pos.X, Pos.Y + 11),
                 Color, "%*c", CursorPos + 1, '_');

    if(TooShort)
    {
      FONT->Printf(DOUBLE_BUFFER, v2(Pos.X, Pos.Y + 30),
                   Color, "Too short!");
      TooShort = false;
    }

    graphics::BlitDBToScreen();

    if(TooShort)
      DOUBLE_BUFFER->Fill(Pos.X, Pos.Y + 30, 81, 9, 0);

    /* if LastKey is less than 20 it is a control
       character not available in the font */

    while(!(LastKey >= 0x20 && LastKey < 0x7F)
      && LastKey != KEY_BACK_SPACE
      && LastKey != KEY_DELETE
      && LastKey != KEY_DOWN
      && LastKey != KEY_END
      && LastKey != KEY_ENTER
      && LastKey != KEY_ESC
      && LastKey != KEY_HOME 
      && LastKey != KEY_LEFT 
      && LastKey != KEY_RIGHT
      && LastKey != KEY_UP
    ){
      LastKey = GET_KEY(false);

      if(StringKeyHandler != 0 && StringKeyHandler(LastKey, Input))
      {
        LastKey = 0; // to `continue;` below
        break;
      }
    }

    if(!LastKey)
      continue;

    if(LastKey == KEY_ESC && AllowExit){
      bInUse=false;
      bAbort=true;
      break;
    }

    if(LastKey == KEY_BACK_SPACE)
    {
      if(CursorPos > 0)
        Input.Erase(static_cast<festring::sizetype>(--CursorPos), 1);

      continue;
    }

    if(LastKey == KEY_DELETE)
    {
      if(CursorPos < Input.GetSize())
        Input.Erase(static_cast<festring::sizetype>(CursorPos), 1);

      continue;
    }

    if(LastKey == KEY_ENTER)
    {
      if(Input.GetSize() >= MinLetters)
        break;
      else
      {
        TooShort = true;
        continue;
      }
    }

    if(LastKey == KEY_HOME)
    {
      CursorPos = 0;

      continue;
    }

    if(LastKey == KEY_END)
    {
      CursorPos = static_cast<int>(Input.GetSize());

      continue;
    }

    if(LastKey == KEY_LEFT)
    {
      if(CursorPos > 0)
        --CursorPos;

      continue;
    }

    if(LastKey == KEY_RIGHT)
    {
      if(CursorPos < static_cast<int>(Input.GetSize()))
        ++CursorPos;

      continue;
    }

    if(LastKey == KEY_UP)
    {
      if(vHist.size()){
        if(iHistIndex == (vHist.size()-1))
          vHist.push_back(Input);

        --iHistIndex;
        if(iHistIndex<0)iHistIndex=0;
        Input=vHist[iHistIndex];
        if(CursorPos>Input.GetSize())
          CursorPos=Input.GetSize();
      }

      continue;
    }
    
    if(LastKey == KEY_DOWN)
    {
      if(vHist.size()){
        ++iHistIndex;
        if(iHistIndex>=vHist.size())iHistIndex=vHist.size()-1;
        Input=vHist[iHistIndex];
        if(CursorPos>Input.GetSize())
          CursorPos=Input.GetSize();
      }

      continue;
    }
    
    if(LastKey >= 0x20 && LastKey < 0x7F
       && (LastKey != ' ' || !Input.IsEmpty())
       && Input.GetSize() < MaxLetters)
      Input.Insert(static_cast<festring::sizetype>(CursorPos++),
                   static_cast<char>(LastKey));
  }
  
  if(bAbort)
    return ABORTED;

  /* Delete all the trailing spaces */

  festring::sizetype LastAlpha = festring::NPos;

  for(festring::sizetype c = 0; c < Input.GetSize(); ++c)
    if(Input[c] != ' ')
      LastAlpha = c;

  /* note: festring::NPos + 1 == 0 */

  Input.Resize(LastAlpha + 1);

  if(!vHist.size() || Input!=vHist[vHist.size()-1]){
    //vHist.push_back(Input);
    fprintf(fl, "%s\n", Input.CStr());
  }
  fclose(fl);
  
  bInUse=false;
  return NORMAL_EXIT;
}

/* Ask a question defined by Topic. This function only accepts numbers.
   The question is drawn to coordinates given by Pos. All fonts are Color
   colored. If Fade is true the question is asked on a black background
   and the transition to that is a fade. */

long iosystem::NumberQuestion(cfestring& Topic, v2 Pos, col16 Color,
                              truth Fade, truth ReturnZeroOnEsc)
{
  bInUse=true;

  v2 V(RES.X, 10); ///???????????
  bitmap BackUp(V, 0);
  blitdata B = { &BackUp,
                 { Pos.X, Pos.Y + 10 },
                 { 0, 0 },
                 { 105, 10 },
                 { 0 },
                 0,
                 0 };

  if(Fade)
  {
    bitmap Buffer(RES, 0);
    Buffer.ActivateFastFlag();
    FONT->Printf(&Buffer, Pos, Color, "%s", Topic.CStr());
    FONT->Printf(&Buffer, v2(Pos.X, Pos.Y + 11), Color, "_");
    Buffer.FadeToScreen();
  }
  else
    DOUBLE_BUFFER->NormalBlit(B);

  festring Input;
  FONT->Printf(DOUBLE_BUFFER, Pos, Color, "%s", Topic.CStr());
  Swap(B.Src, B.Dest);

  for(int LastKey = 0, CursorPos = 0;; LastKey = 0)
  {
    B.Bitmap = DOUBLE_BUFFER;
    BackUp.NormalBlit(B);
    FONT->Printf(DOUBLE_BUFFER, v2(Pos.X, Pos.Y + 10),
                 Color, "%s", Input.CStr());
    FONT->Printf(DOUBLE_BUFFER, v2(Pos.X, Pos.Y + 11),
                 Color, "%*c", CursorPos + 1, '_');
    graphics::BlitDBToScreen();

    while(!isdigit(LastKey) && LastKey != KEY_BACK_SPACE
          && LastKey != KEY_ENTER && LastKey != KEY_ESC
          && LastKey != KEY_HOME && LastKey != KEY_END
          && LastKey != KEY_LEFT && LastKey != KEY_RIGHT
          && (LastKey != '-' || !Input.IsEmpty()))
      LastKey = GET_KEY(false);

    if(LastKey == KEY_BACK_SPACE)
    {
      if(CursorPos > 0)
        Input.Erase(static_cast<festring::sizetype>(--CursorPos), 1);

      continue;
    }

    if(LastKey == KEY_ENTER)
      break;

    if(LastKey == KEY_ESC)
    {
      if(ReturnZeroOnEsc){
        bInUse=false;
        return 0;
      }

      break;
    }

    if(LastKey == KEY_HOME)
    {
      CursorPos = 0;

      continue;
    }

    if(LastKey == KEY_END)
    {
      CursorPos = static_cast<int>(Input.GetSize());

      continue;
    }

    if(LastKey == KEY_LEFT)
    {
      if(CursorPos > 0)
        --CursorPos;

      continue;
    }

    if(LastKey == KEY_RIGHT)
    {
      if(CursorPos < static_cast<int>(Input.GetSize()))
        ++CursorPos;

      continue;
    }

    if(Input.GetSize() < 12)
      Input.Insert(static_cast<festring::sizetype>(CursorPos++),
                   static_cast<char>(LastKey));
  }

  bInUse=false;
  return atoi(Input.CStr());
}

/* Asks a question defined by Topic and the answer is numeric. The value is
   represented by a scroll bar. The topic is drawn to position Pos. Step is
   the step size. Min and Max are the minimum and maximum values. If the
   player aborts with the esc key AbortValue is returned. Color1 is the
   left portion controls the col of left portion of the scroll bar and
   Color2 the right portion. LeftKey and RightKey are the keys for changing
   the scrollbar. Although '<' and '>' also work always. If Fade is true
   the screen is faded to black before drawing th scrollbar. If Handler is
   set it is called always when the value of the scroll bar changes. */

long iosystem::ScrollBarQuestion(cfestring& Topic, v2 Pos,
                                 long StartValue, long Step,
                                 long Min, long Max, long AbortValue,
                                 col16 TopicColor, col16 Color1,
                                 col16 Color2, int LeftKey, int RightKey,
                                 truth Fade, void (*Handler)(long))
{
  bInUse=true;

  long BarValue = StartValue;
  festring Input;
  truth FirstTime = true;
  v2 V(RES.X, 20); ///???????????
  bitmap BackUp(V, 0);

  if(Fade)
  {
    bitmap Buffer(RES, 0);
    Buffer.ActivateFastFlag();
    FONT->Printf(&Buffer, Pos, TopicColor,
                 "%s %ld", Topic.CStr(), StartValue);
    FONT->Printf(&Buffer, v2(Pos.X + (Topic.GetSize() << 3) + 8, Pos.Y + 1),
                 TopicColor, "_");
    Buffer.DrawHorizontalLine(Pos.X + 1, Pos.X + 201,
                              Pos.Y + 15, Color2, false);
    Buffer.DrawVerticalLine(Pos.X + 201, Pos.Y + 12,
                            Pos.Y + 18, Color2, false);
    Buffer.DrawHorizontalLine(Pos.X + 1, Pos.X + 1
                              + (BarValue - Min) * 200 / (Max - Min),
                              Pos.Y + 15, Color1, true);
    Buffer.DrawVerticalLine(Pos.X + 1, Pos.Y + 12, Pos.Y + 18, Color1, true);
    Buffer.DrawVerticalLine(Pos.X + 1 + (BarValue - Min)
                            * 200 / (Max - Min), Pos.Y + 12,
                            Pos.Y + 18, Color1, true);
    Buffer.FadeToScreen();
  }
  else
  {
    blitdata B = { &BackUp,
                   { Pos.X, Pos.Y },
                   { 0, 0 },
                   { RES.X, 20 },
                   { 0 },
                   0,
                   0 };

    DOUBLE_BUFFER->NormalBlit(B);
  }

  blitdata B1 = { 0,
                  { 0, 0 },
                  { Pos.X, Pos.Y },
                  { static_cast<int>(((Topic.GetSize() + 14) << 3) + 1), 10 },
                  { 0 },
                  0,
                  0 };
  blitdata B2 = { 0,
                  { 0, 10 },
                  { Pos.X, Pos.Y + 10 },
                  { 203, 10 },
                  { 0 },
                  0,
                  0 };

  for(int LastKey = 0;; LastKey = 0)
  {
    if(!FirstTime)
      BarValue = Input.IsEmpty() ? Min : atoi(Input.CStr());

    if(BarValue < Min)
      BarValue = Min;

    if(BarValue > Max)
      BarValue = Max;

    if(Handler)
      Handler(BarValue);

    B1.Bitmap = B2.Bitmap = DOUBLE_BUFFER;
    BackUp.NormalBlit(B1);
    BackUp.NormalBlit(B2);

    if(FirstTime)
    {
      FONT->Printf(DOUBLE_BUFFER, Pos, TopicColor,
                   "%s %ld", Topic.CStr(), StartValue);
      FONT->Printf(DOUBLE_BUFFER,
                   v2(Pos.X + (Topic.GetSize() << 3) + 8, Pos.Y + 1),
                   TopicColor, "_");
      FirstTime = false;
    }
    else
    {
      FONT->Printf(DOUBLE_BUFFER, Pos, TopicColor,
                   "%s %s", Topic.CStr(), Input.CStr());
      FONT->Printf(DOUBLE_BUFFER,
                   v2(Pos.X + ((Topic.GetSize() + Input.GetSize()) << 3) + 8,
                   Pos.Y + 1), TopicColor, "_");
    }

    DOUBLE_BUFFER->DrawHorizontalLine(Pos.X + 1, Pos.X + 201,
                                      Pos.Y + 15, Color2, false);
    DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 201, Pos.Y + 12,
                                    Pos.Y + 18, Color2, false);
    DOUBLE_BUFFER->DrawHorizontalLine(Pos.X + 1, Pos.X + 1
                                      + (BarValue - Min) * 200 / (Max - Min),
                                      Pos.Y + 15, Color1, true);
    DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 1, Pos.Y + 12,
                                    Pos.Y + 18, Color1, true);
    DOUBLE_BUFFER->DrawVerticalLine(Pos.X + 1 + (BarValue - Min)
                                    * 200 / (Max - Min), Pos.Y + 12,
                                    Pos.Y + 18, Color1, true);
    graphics::BlitDBToScreen();

    while(!isdigit(LastKey) && LastKey != KEY_ESC
          && LastKey != KEY_BACK_SPACE && LastKey != KEY_ENTER
          && LastKey != KEY_SPACE && LastKey != '<' && LastKey != '>'
          && LastKey != KEY_HOME && LastKey != KEY_END
          && LastKey != RightKey && LastKey != LeftKey
          && LastKey != KEY_RIGHT && LastKey != KEY_LEFT)
      LastKey = GET_KEY(false);

    if(LastKey == KEY_ESC)
    {
      BarValue = AbortValue;
      break;
    }

    if(LastKey == KEY_BACK_SPACE)
    {
      if(!Input.IsEmpty())
        Input.Resize(Input.GetSize() - 1);

      continue;
    }

    if(LastKey == KEY_ENTER || LastKey == KEY_SPACE)
      break;

    if(LastKey == KEY_HOME)
    {
      BarValue = Min;

      Input.Empty();
      Input << BarValue;
      continue;
    }

    if(LastKey == KEY_END)
    {
      BarValue = Max;

      Input.Empty();
      Input << BarValue;
      continue;
    }

    if(LastKey == '<' || LastKey == LeftKey || LastKey == KEY_LEFT)
    {
      BarValue -= Step;

      if(BarValue < Min)
        BarValue = Min;

      Input.Empty();
      Input << BarValue;
      continue;
    }

    if(LastKey == '>' || LastKey == RightKey || LastKey == KEY_RIGHT)
    {
      BarValue += Step;

      if(BarValue > Max)
        BarValue = Max;

      Input.Empty();
      Input << BarValue;
      continue;
    }

    if(Input.GetSize() < 12)
      Input << char(LastKey);
  }

  bInUse=false;
  return BarValue;
}

struct sAlertConfirmMsg{
  bool bShow=false;
  const char* cMsg;
  std::vector<festring> vfsCritMsgs;
  bool bConfirmMode;
} sAlertConfirmMsgInst;
void iosystem::AlertConfirmMsgDraw(bitmap* Buffer)
{
  if(!sAlertConfirmMsgInst.bShow)return;
  
  //TODO calc all line withs to determine the full popup width to not look bad if overflow, see specialkeys help dialog creation
  int iLineHeight=20;
  v2 v2Border(700,100+(sAlertConfirmMsgInst.vfsCritMsgs.size()*iLineHeight));
  v2 v2TL(RES.X/2-v2Border.X/2,RES.Y/2-v2Border.Y/2);

  Buffer->Fill(v2TL,v2Border,DARK_GRAY);
  graphics::DrawRectangleOutlineAround(Buffer, v2TL, v2Border, YELLOW, true);

  v2TL+=v2(16,16);
  int y=v2TL.Y;
  #define ACMPRINT(S,C) {FONT->Printf(Buffer, v2(v2TL.X,y), C, "%s", S);y+=iLineHeight;}
  ACMPRINT(sAlertConfirmMsgInst.cMsg,YELLOW);
  if(sAlertConfirmMsgInst.bConfirmMode)ACMPRINT("(y)es, any other key to ignore this message.",WHITE);
  for(int i=0;i<sAlertConfirmMsgInst.vfsCritMsgs.size();i++){
    if(i==0)
      ACMPRINT("PROBLEMS:",RED);
    ACMPRINT(sAlertConfirmMsgInst.vfsCritMsgs[i].CStr(),WHITE);
  }
}
bool iosystem::AlertConfirmMsg(const char* cMsg,std::vector<festring> vfsCritMsgs,bool bConfirmMode)
{
  static bool bDummyInit = [](){graphics::AddDrawAboveAll(&AlertConfirmMsgDraw,100100,"iosystem::AlertConfirmMsgDraw"); return true;}();
    
  bInUse=true;

  sAlertConfirmMsgInst.bShow=true;
  sAlertConfirmMsgInst.cMsg=cMsg;
  sAlertConfirmMsgInst.vfsCritMsgs=vfsCritMsgs;
  sAlertConfirmMsgInst.bConfirmMode=bConfirmMode;

  AlertConfirmMsgDraw(DOUBLE_BUFFER);
  graphics::BlitDBToScreen(); //as the final blit may be from StretchedBuffer

  bool bAnswer=false;
  if(GET_KEY() == 'y')bAnswer=true;

  bInUse=false;
  sAlertConfirmMsgInst.bShow=false;
  return bAnswer;
}

static bool bSaveGameSortModeByDtTm;
static bool bSaveGameSortModeProgress;
static bool bSaveGameSortModeReversed;
void iosystem::SetSaveGameSortMode(int i){
  bSaveGameSortModeByDtTm=false;
  bSaveGameSortModeProgress=false;
  bSaveGameSortModeReversed=false;

  switch(i){
  case 0:
    bSaveGameSortModeReversed=true;
    bSaveGameSortModeByDtTm=true;
    break;
  case 1:
    bSaveGameSortModeReversed=true;
    bSaveGameSortModeByDtTm=true;
    bSaveGameSortModeProgress=true;
    break;
  case 2: //alphanum is essential
    break;
  case 3:
    bSaveGameSortModeProgress=true;
    break;
  default:
    ABORT("unsupported savegame sort option %d",i);
  }
}
struct fileInfo{
  int Version = -1;
  char* GameScript = 0; //dummy
  int CurrentDungeonIndex = -1;
  int CurrentLevelIndex = -1;
  v2 Camera; //dummy
  truth WizardMode;
  festring fileName;
  festring absFileName;
  festring time;
  festring idOnList;
  festring dungeonID;
  festring fileNameAutoSave;
  std::vector<festring> vBackups;
  bool bIsBkp = false;
  struct stat attr;
  std::vector<festring> afsFullProblems;
};
std::vector<fileInfo> vFiles;
bool addFileInfo(const char* c){
  // skippers
  if(strcmp(c,".")==0)return false;
  if(strcmp(c,"..")==0)return false;

  // do add
  fileInfo fi;
  fi.fileName=c;
  vFiles.push_back(fi); //stores a copy

  return true; //added
}
skipseeksave skipSeek=NULL;
void iosystem::SetSkipSeekSave(skipseeksave sss){skipSeek = sss;}

/**
 * DirectoryName is the directory where the savefiles are located. Returns
 * the selected file or "" if an error occures or if no files are found.
 *
 * returns the savegame basename (w/o path)
 */
festring iosystem::ContinueMenu(col16 TopicColor, col16 ListColor,
                                cfestring& DirectoryName, const int iSaveFileVersion, bool bAllowImportOldSavegame)
{
  ///////////////////// prepare general base data ///////////////////
  vFiles.clear();
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);

  ////////////////////////// OS SPECIFIC!!! collect all files at save folder. //////////////////////////
#ifdef UNIX
  {
    DIR* dp;
    struct dirent* ep;
    dp = opendir(DirectoryName.CStr());
    if(dp!=NULL)
    {
      while( (ep = readdir(dp)) ) addFileInfo(ep->d_name);
      closedir(dp);
    }
  }
#endif

#ifdef WIN32
  struct _finddata_t Found;
  long hFile;
  int Check = 0;
  hFile = _findfirst(festring(DirectoryName + "*").CStr(), &Found);

  /* No file found */
  if(hFile != -1L)
  {
    while(!Check)
    {
      addFileInfo(Found.name);
      Check = _findnext(hFile, &Found);
    }
  }
#endif

#ifdef __DJGPP__
  struct ffblk Found;
  int Check = 0;

  /* get all filenames. Accepts all files even if they
     FA_HIDDEN or FA_ARCH flags are set (ie. they are hidden or archives */

  Check = findfirst(festring(DirectoryName + "*").CStr(),
                    &Found, FA_HIDDEN | FA_ARCH);

  if(!Check)
  {
    while(!Check){
      addFileInfo(Found.ff_name);
      Check = findnext(&Found);
    }
  }
#endif

  if(vFiles.size()==0){
    iosystem::TextScreen(CONST_S("You don't have any previous saves."), ZERO_V2, TopicColor);
    return "";
  }

  /////////////////////////////// sort work //////////////////////////////////////

//  /**
//   * this is rerquired when there is only one savegame and it is just an ...AutoSave.sav
//   */
//  static festring fsLastChangeDetector="_Last_Change_Detector_.sav";
//  addFileInfo(fsLastChangeDetector.CStr());

  cint iTmSz=100;
  struct stat attr;
  for(int i=0;i<vFiles.size();i++)
  {
    vFiles[i].absFileName<<DirectoryName<<"/"<<vFiles[i].fileName;
    if(stat(vFiles[i].absFileName.CStr(), &attr)<0)ABORT("stat() failed: %s %s",vFiles[i].absFileName.CStr(),std::strerror(errno));

    char cTime[iTmSz];
    strftime(cTime,iTmSz,"%Y/%m/%d-%H:%M:%S",localtime(&(attr.st_mtime))); // this format is important for the sorting that is text based
    vFiles[i].time<<cTime;

    vFiles[i].attr=attr;

    DBG6(cTime,attr.st_mtime,attr.st_ctime,attr.st_size,vFiles[i].absFileName.CStr(),attr.st_ino);
  }

  // sort the vector BY NAME ONLY (to get the dungeon ids easily, numbers come before letters)
  std::sort( vFiles.begin(), vFiles.end(),
    [ ]( const fileInfo& l, const fileInfo& r ){ return l.fileName < r.fileName; }
  );DBGLN;

  /********************************************
   *  ATTENTION!!! some of these NEED to be reset down there!
   */
  std::vector<festring> vIds,vInvIds,vBackups;
  std::vector<fileInfo> vComponents;
  festring autoSaveFound;
  festring problems;
  cfestring cfsProblemToken="PROBLEMS:";
  std::vector<festring> afsFullProblems;
  int iPrepareSavFileIndex=-1;
  int iAutoSaveSavFileIndex=-1;
  std::string sPrettyNamePrevious="";
  for(int i=0;i<vFiles.size();) //no i++ here!!!
  {
    DBG2(vFiles[i].fileName.CStr(),sPrettyNamePrevious);

    /** caution with this about endless loop! */
    bool bNextFile=false;

    std::string sPrettyName=vFiles[i].fileName.CStr(); //as pretty as possible... TODO a simple text file containing the correct player name related to the current savegame file
    sPrettyName=sPrettyName.substr(0,sPrettyName.find("."));
    std::string sPrettyNameWork=sPrettyName;

//    if(!sPrettyNamePrevious.empty() && sPrettyNamePrevious!=sPrettyName){
    bool bPlayerFilesGroupChanged = !sPrettyNamePrevious.empty() && sPrettyNamePrevious!=sPrettyName;
    if(i==vFiles.size()-1 || bPlayerFilesGroupChanged){
      /**
       * the .sav main file for one player character was not found on the previous loop step
       * or it reached the last file w/o finding it
       */
      if(iAutoSaveSavFileIndex>-1){
        // but has .AutoSave.sav and will use it
        iPrepareSavFileIndex=iAutoSaveSavFileIndex;
        sPrettyNameWork=sPrettyNamePrevious;
        // bNextFile will remain false, so at next loop step it will use the current one again.
      }
    }

    if(iPrepareSavFileIndex==-1){
      bool bEmptyFileProblem=false;
      if(vFiles[i].attr.st_size==0){ //empty files will crash-abort with bad_alloc error, but only autosaves may be empty and the game can still be played
        if(problems.IsEmpty())
          problems<<cfsProblemToken;
        problems<<"EmptyFile,"; DBG2("PROBLEM:EmptyFile",vFiles[i].fileName.CStr());
        afsFullProblems.push_back(festring("EmptyFile:")+vFiles[i].fileName);
        bEmptyFileProblem=true;
      }

      bool bCriticalProblemFound=false;
      if(bEmptyFileProblem){
        bCriticalProblemFound=true;
      }

      if(bCriticalProblemFound){
        /**
         * will skip the broken/corrupted file, it will be ignored ONLY here. Trying to load may still consider it and crash.
         *
         * TODO
         *  show detailed info about the problems and say what user can do to try to fix it
         *
         *  ex.: like removing empty files, but if the empty file has no backup and is required by the .sav,
         *    the game will still crash and be lost.
         *    Generating a new level is probably bad as it will ignore whatever happened in that level and may create
         *    duplicated items (uniques) and characters (uniques that died) when they should not exist,
         *    may be only for fully random (with non unique things on it) levels such workaround could be used...
         * 
         * TODO for file extensions, find the type only at the end, use std::string if easier/clearer to implement
         */
      }else
      if(vFiles[i].fileName.Find(".wm" ) != vFiles[i].fileName.NPos){
        //skipped from the list
      }else
      if(vFiles[i].fileName.Find(".tmp" ) != vFiles[i].fileName.NPos){
        //just ignored
      }else
      if(vFiles[i].fileName.Find(".bkp") != vFiles[i].fileName.NPos){ //skipped from the list
        vBackups.push_back(vFiles[i].absFileName);
        vFiles[i].bIsBkp=true;
      }else
      if(vFiles[i].fileName.Find(".AutoSave.") != vFiles[i].fileName.NPos){ //skipped from the list
        if(vFiles[i].fileName.Find(".AutoSave.sav") != vFiles[i].fileName.NPos){ // the correct autosave to be returned
          autoSaveFound=vFiles[i].fileName;
          iAutoSaveSavFileIndex=i;
        }
      }else
      if(vFiles[i].fileName.Find(".sav") != vFiles[i].fileName.NPos){
        iPrepareSavFileIndex=i;
      }else{ //dungeon IDs TODO this will mess if player's name has dots '.', prevent it during filename creation as spaces are
        std::string s=vFiles[i].fileName.CStr();
        s=s.substr(s.find(".")+1);
        vFiles[i].dungeonID<<festring(s.c_str());
        vComponents.push_back(vFiles[i]);
        DBG3(vFiles[i].fileName.CStr(),s,vComponents.size());//,dungeonIds.CStr());
      }

      bNextFile=true;
    }

    if(iPrepareSavFileIndex>-1){
      fileInfo& rfi = vFiles[iPrepareSavFileIndex];

      festring id;

      // savegame version (save structure taken from game::Load())
      inputfile SaveFile(rfi.absFileName, 0, false);
      SaveFile >> rfi.Version; DBGSI(rfi.Version);
      if(skipSeek==NULL)ABORT("SkipSeek function not set");
      skipSeek(&SaveFile); //DUMMY (for here) binary data skipper
      //TODO the current dungeon and many other useful info could be stored in a simple text file just to be used during this game loading list!
      SaveFile >> rfi.CurrentDungeonIndex >> rfi.CurrentLevelIndex; DBG2(rfi.CurrentDungeonIndex,rfi.CurrentLevelIndex);
      SaveFile >> rfi.Camera; //skipper
      SaveFile >> rfi.WizardMode;
      SaveFile.Close();

      festring fsVer;
      if(rfi.Version != iSaveFileVersion)
        fsVer<<"(v"<<rfi.Version<<") ";

      if(bSaveGameSortModeByDtTm)
        id<<fsVer<<rfi.time<<" ";

      id<<(rfi.WizardMode?"[WIZ]":"")<<sPrettyNameWork.c_str()<<" ";

      if(!bSaveGameSortModeByDtTm)
        id<<fsVer<<" "; //after to not compromise the alphanumeric default sorting in case user want's to use it

      festring currentDungeonLevel;
      currentDungeonLevel << rfi.CurrentDungeonIndex << rfi.CurrentLevelIndex; DBG1(currentDungeonLevel.CStr());  //TODO tricky part if any dungeon or level goes beyond 9 ?
      if(bSaveGameSortModeProgress && !vComponents.empty()){
        for(int k=0;k<vComponents.size();k++){
          if(k>0)id<<" ";
          if(vComponents[k].dungeonID == currentDungeonLevel){
            id<<"@"<<currentDungeonLevel<<""; // the legendary player character indicator! :D
          }else{
            id<<vComponents[k].dungeonID;
          }
        }
      }

      if(!autoSaveFound.IsEmpty()){
        id<<" has AutoSave!";
        rfi.fileNameAutoSave=autoSaveFound;
      }

      if(vBackups.size()>0){
        id<<" has backup(s)!";
        rfi.vBackups=vBackups; //makes a copy
      }

      if(!problems.IsEmpty()){
        id<<" ["<<problems<<"]";
        rfi.afsFullProblems=afsFullProblems;
      }

      ///////////////// ID is ready /////////////////////

      rfi.idOnList<<id;

      bool bValid=false;
      static const int iOldSavegameVersionImportSince=131;
      if(!bValid && rfi.Version == iSaveFileVersion)
        bValid=true;
      if(!bValid && rfi.Version <  iSaveFileVersion)
        if(bAllowImportOldSavegame)
          if(rfi.Version >= iOldSavegameVersionImportSince)
            bValid=true;

      if(bValid){
        vIds.push_back(id);DBG2("ok",DBGC(id.CStr()));
      }else{
        vInvIds.push_back(id);DBG2("invalid",DBGC(id.CStr()));
      }

      //reset to for next .sav fillup
      vComponents.clear();
      vBackups.clear();
      autoSaveFound.Empty();
      problems.Empty();
      afsFullProblems.clear();
      iPrepareSavFileIndex=-1;
      iAutoSaveSavFileIndex=-1;
    }

    sPrettyNamePrevious=sPrettyName;

    if(bNextFile)i++;
  }

  // this will sort the ids alphabetically even if by time (so will sort the time as a string)
  std::sort( vIds   .begin(), vIds   .end(), [ ]( const festring& l, const festring& r ){return l < r;} );
  std::sort( vInvIds.begin(), vInvIds.end(), [ ]( const festring& l, const festring& r ){return l < r;} );

  // reversed or normal
  int iFirst,iFirstI,iStopAt,iStopAtI,iDir; //TODO implement something more readable, easier to maintain...
  if(bSaveGameSortModeReversed){
    iDir=-1; iFirst=vIds.size()-1; iStopAt=-1         ; iFirstI=vInvIds.size()-1; iStopAtI=-1            ; }else{
    iDir= 1; iFirst=0            ; iStopAt=vIds.size(); iFirstI=0               ; iStopAtI=vInvIds.size();
  }
  for(int i=iFirst ;i!=iStopAt ;i+=iDir){
    col16 lc = ListColor;
    if(vIds[i].Find(cfsProblemToken)!=festring::NPos) lc = RED;
    List.AddEntry(vIds   [i],lc       ,0,NO_IMAGE,true ); DBG2(DBGC(vIds[i].CStr())   ,"ok"     );
  }
  for(int i=iFirstI;i!=iStopAtI;i+=iDir){
    List.AddEntry(vInvIds[i],DARK_GRAY,0,NO_IMAGE,false); DBG2(DBGC(vInvIds[i].CStr()),"invalid");
  }

  if(List.IsEmpty() || vIds.empty())
  {
    iosystem::TextScreen(CONST_S("You don't have any valid previous saves."), ZERO_V2, TopicColor);
    return "";
  }
  else
  {
    int Check = List.Draw();

    /* an error has occured in felist */
    if(Check & FELIST_ERROR_BIT)
      return "";

    festring chosen;chosen<<List.GetEntry(Check);
    for(int i=0;i<vFiles.size();i++){
      if(chosen == vFiles[i].idOnList){
        bool bHasAutosave = !vFiles[i].fileNameAutoSave.IsEmpty();

        /**
         * Backups are a fallback mainly when a crash happens like this:
         *
         * - USER action: you try to enter a new dungeon (so it will be created now)
         *
         * - game action: saves a backup of the current dungeon last save at ".bkp"
         * - game action: saves the current dungeon w/o the player on it at ".tmp" (so if this saving crashes, the corrupted .tmp will just be ignored)
         * - game action: copies the sane ".tmp" to the final filename (and deletes the ".tmp" just after)
         *
         * - game bug: the new dungeon creation fails and crashes.
         *
         * This means: the only file remaining with player data (a player char at dungeon pos) is the .bkp one!!!
         */
        std::vector<festring>& rvBackups = vFiles[i].vBackups;
        bool bProblemsSeen=false;
        if(rvBackups.size()>0){
          bool bRestoreBkp = false;
          if(!vFiles[i].WizardMode){
            // normal savegame
            bRestoreBkp = AlertConfirmMsg("The backup may be unnecessary, restore it anyway?",vFiles[i].afsFullProblems);
            bProblemsSeen=true;
          }else{
            // wizard mode special case
            if(bHasAutosave){
              /**
               * TODO this is guessed, not 100% correct... the files' datetime should be compared instead...
               */
              bRestoreBkp = AlertConfirmMsg("(Wizard) The autosave is newer. Restore backup anyway?",vFiles[i].afsFullProblems);
              bProblemsSeen=true;
            }
          }
          if(bRestoreBkp){
            for(int b=0;b<rvBackups.size();b++){
              festring fsBkp(rvBackups[b]); DBG1(fsBkp.CStr());
              DBG2("RestoringBackupFrom",fsBkp.CStr());

              festring fsFinal(fsBkp);
              fsFinal.Resize(fsFinal.GetSize() -4); // - ".bkp"

              /**
               * Removes broken save file.
               * This action is destructive, preventing other possible options...
               * TODO instead, just rename to something like .OLD ?
               */
              std::remove(fsFinal.CStr());

              std::ifstream flBkp(fsBkp.CStr(), std::ios::binary);
              if(flBkp.good()){
                std::ofstream final(fsFinal.CStr(), std::ios::binary);
                final << flBkp.rdbuf();

                final.close();
                flBkp.close();

                // DO NOT Remove the backup here. If the game is playable, when returning to main menu, it will be done properly there!
              }else{
                ABORT("unable to access the backup file '%s'",fsBkp.CStr());
              }
            }
          }
        }

        if(vFiles[i].afsFullProblems.size()>0 && !bProblemsSeen)
          if(!AlertConfirmMsg("There are problems with this save, try to load anyway?",vFiles[i].afsFullProblems))
            return "";

        if(bHasAutosave){
          bool bUseAutosave=true; //normal game

          if(bUseAutosave && vFiles[i].WizardMode){
            /**
             * wizard mode always keep autosaves
             */
            if(rvBackups.size()==0){
              /**
               * Wizard mode without backups means it saved safely to main menu,
               * and also mean that the autosaves are OLDER than normal saves.
               */
              bUseAutosave=false; //this will let the NEWER wiz normal saves be loaded
            }else{
              /**
               * If the game crashes it will have backups.
               * And the autosaves will be NEWER than normal saves and backups.
               * UNLESS the backups were restored above losing the NEWER autosaves!!!
               */
            }
          }

          if(bUseAutosave){
            /**
             * autosaves are old and apparently a safe thing,
             * therefore will be preferred as restoration override.
             */
            DBG2("LoadingAutoSave",vFiles[i].fileNameAutoSave.CStr());
            return vFiles[i].fileNameAutoSave;
          }
        }

        DBG2("Loading",vFiles[i].fileName.CStr());
        return vFiles[i].fileName;
      }
    }

    ABORT("failed to match chosen file with id %s",chosen.CStr()); //shouldnt happen
  }

  return ""; //dummy just to gcc do not complain..
}
