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

#ifdef WIN32
#include <io.h>
#endif

#ifdef LINUX
#include <dirent.h>
#include <stddef.h>
#include <cstdio>
#include <sys/types.h>
#include <algorithm>
#endif

#ifdef __DJGPP__
#include <dir.h>
#endif

#include "graphics.h"
#include "feio.h"
#include "whandler.h"
#include "felist.h"
#include "rawbit.h"
#include "festring.h"
#include "bitmap.h"

#define PENT_WIDTH 70

/* Prints screen full of Text in color Color. If GKey is true function
   waits for keypress. BitmapEditor is a pointer to function that is
   called during every fade tick. */

void iosystem::TextScreen(cfestring& Text, v2 Disp,
			  col16 Color, truth GKey, truth Fade,
			  bitmapeditor BitmapEditor)
{
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
      FONT->Printf(&Buffer, PrintPos, Color, Line);
      ++Lines;
      LastBeginningOfLine = c + 1;
    }
    else
      Line[c - LastBeginningOfLine] = Text[c];

  Line[c - LastBeginningOfLine] = 0;
  v2 PrintPos((RES.X >> 1) - (strlen(Line) << 2) + Disp.X,
	      (RES.Y << 1) / 5 - (LineNumber - Lines) * 15 + Disp.Y);
  FONT->Printf(&Buffer, PrintPos, Color, Line);

  if(Fade)
    Buffer.FadeToScreen(BitmapEditor);
  else
  {
    BitmapEditor(&Buffer, true);
    Buffer.FastBlit(DOUBLE_BUFFER);
    graphics::BlitDBToScreen();
  }

  if(GKey)
    if(BitmapEditor)
      while(!READ_KEY())
	BitmapEditor(DOUBLE_BUFFER, false);
    else
      GET_KEY();
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
  if(CountChars('\r',sMS) < 1)
    return (-1);

  truth bReady = false;
  int iSelected = 0;
  bitmap Backup(DOUBLE_BUFFER);
  Backup.ActivateFastFlag();
  bitmap Buffer(RES);
  Buffer.ActivateFastFlag();
  int c = 0;

  if(BackGround)
    BackGround->FastBlit(&Buffer);
  else
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
      sCopyOfMS.Erase(0,RPos+1);
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
      sCopyOfMS.Erase(0,RPos+1);
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
      sCopyOfMS.Erase(0,RPos+1);
      v2 PrintPos(3, RES.Y - CountChars('\r', SmallText1) * 10 + i * 10);
      FONT->Printf(&Buffer, PrintPos, Color, "%s", VeryUnGuruPrintf.CStr());

    }

    sCopyOfMS = SmallText2;

    for(i = 0; i < CountChars('\r', SmallText2); ++i)
    {
      festring::sizetype RPos = sCopyOfMS.Find('\r');
      VeryUnGuruPrintf = sCopyOfMS;
      VeryUnGuruPrintf.Resize(RPos);
      sCopyOfMS.Erase(0,RPos+1);
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
		//FONT->Printf(&Buffer, v2(100,100), Color, "%s","NUKES IS HERE!");
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
	iSelected = (CountChars('\r',sMS)-1);
      break;

     case KEY_DOWN:
      if(iSelected < (CountChars('\r',sMS)-1))
	++iSelected;
      else
	iSelected = 0;
      break;

     case 0x00D:
      bReady = true;
      break;

     default:
      if(k > 0x30 && k < 0x31 + CountChars('\r',sMS))
	return k - 0x31;
    }
  }

  return iSelected;
}

/* Asks the user a question requiring a string answer. The answer is saved
   to Input. Input can also already have a default something retyped for
   the user. Topic is the question or other topic for the question. Pos the
   cordinates of where the question is printed on the screen. Color is the
   col of all the fonts in this function. Enter is only accepted when the
   answers length is between MinLetters and MaxLetters. If Fade is true the
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

    while(!(IsAcceptableForStringQuestion(LastKey)))
    {
      LastKey = GET_KEY(false);

      if(StringKeyHandler != 0 && StringKeyHandler(LastKey, Input))
      {
	LastKey = 0;
	break;
      }
    }

    if(!LastKey)
      continue;

    if(LastKey == KEY_ESC && AllowExit)
      return ABORTED;

    if(LastKey == KEY_BACK_SPACE)
    {
      if(CursorPos)
        Input.Erase(static_cast<festring::sizetype>(--CursorPos), 1);

      continue;
    }

    if(LastKey == KEY_ENTER)
      if(Input.GetSize() >= MinLetters)
	break;
      else
      {
	TooShort = true;
	continue;
      }

    if(LastKey == KEY_LEFT)
    {
      if(CursorPos)
        --CursorPos;

      continue;
    }

    if(LastKey == KEY_RIGHT)
    {
      if(CursorPos < static_cast<int>(Input.GetSize()))
        ++CursorPos;

      continue;
    }

    if(LastKey >= 0x20 && Input.GetSize() < MaxLetters
       && (LastKey != ' ' || !Input.IsEmpty())
       && LastKey != KEY_UP && LastKey != KEY_DOWN)
      Input.Insert(static_cast<festring::sizetype>(CursorPos++),
                   static_cast<char>(LastKey));
  }

  /* Delete all the trailing spaces */

  festring::sizetype LastAlpha = festring::NPos;

  for(festring::sizetype c = 0; c < Input.GetSize(); ++c)
    if(Input[c] != ' ')
      LastAlpha = c;

  /* note: festring::NPos + 1 == 0 */

  Input.Resize(LastAlpha + 1);

  return NORMAL_EXIT;
}

/* Ask a question defined by Topic. This function only accepts numbers.
   The question is drawn to cordinates given by Pos. All fonts are Color
   coled. If Fade is true the question is asked on a black background
   and the transition to that is a fade. */

long iosystem::NumberQuestion(cfestring& Topic, v2 Pos, col16 Color,
			      truth Fade, truth ReturnZeroOnEsc)
{
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
          && LastKey != KEY_LEFT && LastKey != KEY_RIGHT
	  && (LastKey != '-' || !Input.IsEmpty()))
      LastKey = GET_KEY(false);

    if(LastKey == KEY_BACK_SPACE)
    {
      if(CursorPos)
        Input.Erase(static_cast<festring::sizetype>(--CursorPos), 1);

      continue;
    }

    if(LastKey == KEY_ENTER)
      break;

    if(LastKey == KEY_ESC)
    {
      if(ReturnZeroOnEsc)
	return 0;

      break;
    }

    if(LastKey == KEY_LEFT)
    {
      if(CursorPos)
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

  return BarValue;
}

/* DirectoryName is the directory where the savefiles are located. Returns
   the selected file or "" if an error occures or if no files are found. */

festring iosystem::ContinueMenu(col16 TopicColor, col16 ListColor,
				cfestring& DirectoryName)
{
#ifdef WIN32
  struct _finddata_t Found;
  long hFile;
  int Check = 0;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);
  hFile = _findfirst(festring(DirectoryName + "*.sav").CStr(), &Found);

  /* No file found */
  if(hFile == -1L)
  {
    TextScreen(CONST_S("You don't have any previous saves."), ZERO_V2, TopicColor);
    return "";
  }

  while(!Check)
  {
    /* Copy all the filenames to Buffer */
    /* Buffer = Found.name; Doesn't work because of a festring bug */

    Buffer.Empty();
    Buffer << Found.name;
    List.AddEntry(Buffer, ListColor);
    Check = _findnext(hFile, &Found);
  }

  Check = List.Draw();

  /* an error has occured in felist */

  if(Check & FELIST_ERROR_BIT)
    return "";

  return List.GetEntry(Check);
#endif

#ifdef LINUX
  DIR* dp;
  struct dirent* ep;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);
  dp = opendir(DirectoryName.CStr());

  if(dp)
  {
    while((ep = readdir(dp)))
    {
      /* Buffer = ep->d_name; Doesn't work because of a festring bug */
      Buffer.Empty();
      Buffer << ep->d_name;
      /* Add to List all save files */
      if(Buffer.Find(".sav") != Buffer.NPos)
	List.AddEntry(Buffer, ListColor);
    }

    if(List.IsEmpty())
    {
      TextScreen(CONST_S("You don't have any previous saves."), ZERO_V2, TopicColor);
      return "";
    }
    else
    {
      int Check = List.Draw();

      if(Check & FELIST_ERROR_BIT)
	return "";

      return List.GetEntry(Check);
    }

  }

  return "";
#endif

#ifdef __DJGPP__
  struct ffblk Found;
  int Check = 0;
  festring Buffer;
  felist List(CONST_S("Choose a file and be sorry:"), TopicColor);

  /* get all filenames ending with .sav. Accepts all files even if they
     FA_HIDDEN or FA_ARCH flags are set (ie. they are hidden or archives */

  Check = findfirst(festring(DirectoryName + "*.sav").CStr(),
		    &Found, FA_HIDDEN | FA_ARCH);

  if(Check)
  {
    TextScreen(CONST_S("You don't have any previous saves."), ZERO_V2, TopicColor);
    return "";
  }

  while(!Check)
  {
    /* Buffer = Found.ff_name; Doesn't work because of a festring bug */
    Buffer.Empty();
    Buffer << Found.ff_name;
    List.AddEntry(Buffer, ListColor);
    Check = findnext(&Found);
  }

  Check = List.Draw();

  if(Check & FELIST_ERROR_BIT)
    return "";

  return List.GetEntry(Check);
#endif
}

truth iosystem::IsAcceptableForStringQuestion(char Key)
{
  if(Key == '|' || Key == '<' || Key == '>' || Key == '?' || Key == '*'
     || Key == '/' || Key == '\\' || Key == ':')
    return false;

  if(Key < 0x20
     && !(Key == KEY_BACK_SPACE || Key == KEY_ENTER || Key == KEY_ESC))
    return false;

  return true;
}
