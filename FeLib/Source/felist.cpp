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

#include <fstream>

#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "rawbit.h"
#include "save.h"
#include "festring.h"

const felist* FelistCurrentlyDrawn = 0;

truth FelistDrawController()
{
  FelistCurrentlyDrawn->DrawPage(DOUBLE_BUFFER);
  return true;
}

struct felistentry
{
  felistentry() : ImageKey(NO_IMAGE) { }
  felistentry(cfestring&, col16, uint, uint, truth);
  festring String;
  col16 Color;
  uint Marginal;
  uint ImageKey;
  truth Selectable;
};

felistentry::felistentry(cfestring& String, col16 Color,
			 uint Marginal, uint ImageKey, truth Selectable)
: String(String), Color(Color), Marginal(Marginal),
  ImageKey(ImageKey), Selectable(Selectable)
{
}

outputfile& operator<<(outputfile& SaveFile, const felistentry* Entry)
{
  SaveFile << Entry->String << Entry->Color
	   << Entry->Marginal << Entry->Selectable;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistentry*& Entry)
{
  Entry = new felistentry;
  SaveFile >> Entry->String >> Entry->Color
	   >> Entry->Marginal >> Entry->Selectable;
  return SaveFile;
}

struct felistdescription
{
  felistdescription() { }
  felistdescription(cfestring& String, col16 Color)
  : String(String), Color(Color) { }
  festring String;
  col16 Color;
};

felist::felist(cfestring& Topic, col16 TopicColor, uint Maximum)
: Maximum(Maximum), Selected(0), Pos(10, 10), Width(780),
  PageLength(30), BackColor(0), Flags(SELECTABLE|FADE),
  UpKey(KEY_UP), DownKey(KEY_DOWN), EntryDrawer(0)
{
  AddDescription(Topic, TopicColor);
}

felist::~felist()
{
  Empty();

  for(uint c = 0; c < Description.size(); ++c)
    delete Description[c];
}

truth felist::IsEmpty() const
{ return Entry.empty(); }
uint felist::GetLength() const
{ return Entry.size(); }
uint felist::GetLastEntryIndex() const
{ return Entry.size() - 1; }
festring felist::GetEntry(uint I) const
{ return Entry[I]->String; }
col16 felist::GetColor(uint I) const
{ return Entry[I]->Color; }
void felist::SetColor(uint I, col16 What)
{ Entry[I]->Color = What; }
void felist::AddDescription(cfestring& Str, col16 Color)
{ Description.push_back(new felistdescription(Str, Color)); }

void felist::Pop()
{
  delete Entry[GetLastEntryIndex()];
  Entry.pop_back();
}

uint felist::Draw()
{
  while(Entry.size() && Entry[GetLastEntryIndex()]->String.IsEmpty())
    Pop();

  if(Entry.empty())
    return LIST_WAS_EMPTY;

  FelistCurrentlyDrawn = this;

  if(globalwindowhandler::ControlLoopsInstalled())
    globalwindowhandler::InstallControlLoop(FelistDrawController);

  bitmap BackGround(RES);
  BackGround.ActivateFastFlag();
  bitmap* Buffer;

  if(Flags & FADE)
  {
    Buffer = new bitmap(RES, 0);
    Buffer->ActivateFastFlag();
    BackGround.ClearToColor(0);
  }
  else
  {
    Buffer = DOUBLE_BUFFER;
    Buffer->FastBlit(&BackGround);
  }

  uint c;
  uint Return, Selectables = 0;
  truth JustSelectMove = false;

  for(c = 0; c < Entry.size(); ++c)
    if(Entry[c]->Selectable)
      ++Selectables;

  if(Selected >= Selectables)
    Selected = Selectables - 1;

  if(Flags & SELECTABLE)
    PageBegin = Selected - Selected % PageLength;
  else if(Flags & INVERSE_MODE)
    PageBegin = GetLastEntryIndex() - GetLastEntryIndex() % PageLength;
  else
    PageBegin = 0;

  for(;;)
  {
    truth AtTheEnd = DrawPage(Buffer);

    if(Flags & FADE)
    {
      if(JustSelectMove)
      {
	Buffer->FastBlit(DOUBLE_BUFFER);
	graphics::BlitDBToScreen();
      }
      else
	Buffer->FadeToScreen();

      JustSelectMove = false;
    }
    else
      graphics::BlitDBToScreen();

    uint Pressed = GET_KEY(false);

    if(Flags & SELECTABLE && Pressed > 64
       && Pressed < 91 && Pressed - 65 < PageLength
       && Pressed - 65 + PageBegin < Selectables)
    {
      Return = Selected = Pressed - 65 + PageBegin;
      break;
    }

    if(Flags & SELECTABLE && Pressed > 96
       && Pressed < 123 && Pressed - 97 < PageLength
       && Pressed - 97 + PageBegin < Selectables)
    {
      Return = Selected = Pressed - 97 + PageBegin;
      break;
    }

    if(Flags & SELECTABLE && Pressed == UpKey)
    {
      if(Selected)
      {
	--Selected;

	if(Selected < PageBegin)
	{
	  BackGround.FastBlit(Buffer);
	  PageBegin -= PageLength;
	}
	else
	  JustSelectMove = true;
      }
      else
      {
	for(c = 0, Selected = 0; c < Entry.size(); ++c)
	  if(Entry[c]->Selectable)
	    ++Selected;

	--Selected;

	if(PageBegin == Selected - Selected % PageLength)
	  JustSelectMove = true;
	else
	{
	  BackGround.FastBlit(Buffer);
	  PageBegin = Selected - Selected % PageLength;
	}
      }

      continue;
    }

    if(Flags & SELECTABLE && Pressed == DownKey)
    {
      if(!AtTheEnd || Selected != Selectables - 1)
      {
	++Selected;

	if(Selected > PageBegin + PageLength - 1)
	{
	  BackGround.FastBlit(Buffer);
	  PageBegin += PageLength;
	}
	else
	  JustSelectMove = true;
      }
      else
      {
	if(!PageBegin)
	  JustSelectMove = true;
	else
	  BackGround.FastBlit(Buffer);

	Selected = PageBegin = 0;
      }

      continue;
    }

    if(Flags & SELECTABLE && Pressed == KEY_ENTER)
    {
      Return = Selected;
      break;
    }

    if(Pressed == KEY_ESC)
    {
      Return = ESCAPED;
      break;
    }

    if((AtTheEnd && !(Flags & INVERSE_MODE))
       || (!PageBegin && Flags & INVERSE_MODE))
    {
      Return = NOTHING_SELECTED;
      break;
    }
    else
    {
      BackGround.FastBlit(Buffer);

      if(Flags & INVERSE_MODE)
	PageBegin -= PageLength;
      else
	PageBegin += PageLength;

      if(Flags & SELECTABLE)
	Selected = PageBegin;
    }
  }

  if(!(Flags & FADE))
  {
    if(Flags & DRAW_BACKGROUND_AFTERWARDS)
      BackGround.FastBlit(DOUBLE_BUFFER);

    if(Flags & BLIT_AFTERWARDS)
      graphics::BlitDBToScreen();
  }
  else
    delete Buffer;

  globalwindowhandler::DeInstallControlLoop(FelistDrawController);
  return Return;
}

static festring Str;

truth felist::DrawPage(bitmap* Buffer) const
{
  uint LastFillBottom = Pos.Y + 23 + Description.size() * 10;
  DrawDescription(Buffer);

  uint c, i; // c == entry index, i == selectable index

  for(c = 0, i = 0; i != PageBegin; ++c)
    if(Entry[c]->Selectable)
      ++i;

  while(!Entry[c]->Selectable && Entry[c]->String.IsEmpty()) ++c;
  std::vector<festring> Chapter;

  for(;;)
  {
    Str.Empty();
    uint Marginal = Entry[c]->Marginal;

    if(Flags & SELECTABLE && Entry[c]->Selectable)
    {
      Str << char('A' + (i - PageBegin)) << ": ";
      Marginal += 3;
    }

    Str << Entry[c]->String;

    if(Entry[c]->ImageKey != NO_IMAGE)
    {
      if(Str.GetSize() <= (Width - 50) >> 3)
      {
	Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 20, BackColor);

	if(EntryDrawer)
	  EntryDrawer(Buffer,
		      v2(Pos.X + 13, LastFillBottom),
		      Entry[c]->ImageKey);

	if(Flags & SELECTABLE && Entry[c]->Selectable && Selected == i)
	  FONT->PrintfUnshaded(Buffer, v2(Pos.X + 38, LastFillBottom + 5),
			       WHITE, "%s", Str.CStr());
	else
	  FONT->Printf(Buffer, v2(Pos.X + 37, LastFillBottom + 4),
		       Entry[c]->Color, "%s", Str.CStr());

	LastFillBottom += 20;
      }
      else
      {
	uint ChapterSize = festring::SplitString(Str, Chapter,
						 (Width - 50) >> 3,
						 Marginal);
	uint PictureTop = LastFillBottom + ChapterSize * 5 - 9;

	for(uint l = 0; l < ChapterSize; ++l)
	{
	  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);

	  if(Flags & SELECTABLE && Entry[c]->Selectable && Selected == i)
	    FONT->PrintfUnshaded(Buffer, v2(Pos.X + 38, LastFillBottom + 1),
				 WHITE, "%s", Chapter[l].CStr());
	  else
	    FONT->Printf(Buffer, v2(Pos.X + 37, LastFillBottom),
			 Entry[c]->Color, "%s", Chapter[l].CStr());

	  LastFillBottom += 10;
	}

	if(EntryDrawer)
	  EntryDrawer(Buffer,
		      v2(Pos.X + 13, PictureTop),
		      Entry[c]->ImageKey);
      }
    }
    else
    {
      uint ChapterSize = festring::SplitString(Str, Chapter,
					       (Width - 26) >> 3,
					       Marginal);

      for(uint l = 0; l < ChapterSize; ++l)
      {
	Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);

	if(Flags & SELECTABLE && Entry[c]->Selectable && Selected == i)
	  FONT->PrintfUnshaded(Buffer, v2(Pos.X + 14, LastFillBottom + 1),
			       WHITE, "%s", Chapter[l].CStr());
	else
	  FONT->Printf(Buffer, v2(Pos.X + 13, LastFillBottom),
		       Entry[c]->Color, "%s", Chapter[l].CStr());

	LastFillBottom += 10;
      }
    }

    if((i - PageBegin == PageLength - 1 && Entry[c]->Selectable)
       || c == Entry.size() - 1)
    {
      if((!(Flags & INVERSE_MODE) && c != Entry.size() - 1)
	 || (Flags & INVERSE_MODE && PageBegin))
      {
	Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 30, BackColor);
	FONT->Printf(Buffer, v2(Pos.X + 13, LastFillBottom + 10), WHITE,
		     "- Press SPACE to continue, ESC to exit -");
	LastFillBottom += 30;
      }
      else
      {
	Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);
	LastFillBottom += 10;
      }

      Buffer->DrawRectangle(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2,
			    LastFillBottom + 1, DARK_GRAY, true);
      break;
    }

    if(Entry[c++]->Selectable)
      ++i;
  }

  return c == Entry.size() - 1;
}

void felist::DrawDescription(bitmap* Buffer) const
{
  Buffer->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20, BackColor);

  for(uint c = 0; c < Description.size(); ++c)
  {
    Buffer->Fill(Pos.X + 3, Pos.Y + 13 + c * 10, Width - 6, 10, BackColor);
    FONT->Printf(Buffer, v2(Pos.X + 13, Pos.Y + 13 + c * 10),
		 Description[c]->Color, Description[c]->String.CStr());
  }

  Buffer->Fill(Pos.X + 3, Pos.Y + 13 + Description.size() * 10,
	       Width - 6, 10, BackColor);
}

/* We suppose InverseMode != false here */

void felist::QuickDraw(bitmap* Bitmap, uint PageLength) const
{
  static std::vector<festring> Chapter;
  uint Width = Bitmap->GetSize().X;
  Bitmap->Fill(3, 3, Width - 6, 20 + PageLength * 10, 0);
  Bitmap->DrawRectangle(1, 1, Width - 2,
			24 + PageLength * 10,
			DARK_GRAY, true);
  uint LineSize = (Width - 26) >> 3;

  uint Index = 0;
  uint Bottom = PageLength * 10 + 3;

  for(uint c1 = 0; c1 <= Selected; ++c1)
  {
    const felistentry* CurrentEntry = Entry[Selected - c1];
    uint ChapterSize = festring::SplitString(CurrentEntry->String,
					     Chapter, LineSize,
					     CurrentEntry->Marginal);

    for(uint c2 = 0; c2 < ChapterSize; ++c2)
    {
      col16 Color = CurrentEntry->Color;
      Color = MakeRGB16(GetRed16(Color) - ((GetRed16(Color) * 3
					    * Index / PageLength) >> 2),
			GetGreen16(Color) - ((GetGreen16(Color) * 3
					      * Index / PageLength) >> 2),
			GetBlue16(Color) - ((GetBlue16(Color) * 3
					     * Index / PageLength) >> 2));
      FONT->Printf(Bitmap, v2(13, Bottom), Color, "%s",
		   Chapter[ChapterSize - c2 - 1].CStr());
      Bottom -= 10;

      if(++Index == PageLength)
	return;
    }
  }
}

void felist::CreateQuickDrawFontCaches(rawbitmap* Font,
				       col16 Color,
				       uint PageLength)
{
  if(PageLength < 2)
    return;

  for(uint c = 0; c < PageLength; ++c)
    Font->CreateFontCache(MakeRGB16(GetRed16(Color)
				    - ((GetRed16(Color) * 3
					* c / PageLength) >> 2),
				    GetGreen16(Color)
				    - ((GetGreen16(Color) * 3
					* c / PageLength) >> 2),
				    GetBlue16(Color)
				    - ((GetBlue16(Color) * 3
					* c / PageLength) >> 2)));
}

void felist::Empty()
{
  for(uint c = 0; c < Entry.size(); ++c)
    delete Entry[c];

  Entry.clear();
}

void felist::AddEntry(cfestring& Str, col16 Color,
		      uint Marginal, uint Key, truth Selectable)
{
  Entry.push_back(new felistentry(Str, Color, Marginal, Key, Selectable));

  if(Maximum && Entry.size() > ulong(Maximum))
  {
    delete Entry[0];
    Entry.erase(Entry.begin());
  }
}

void felist::Save(outputfile& SaveFile) const
{
  SaveFile << Entry << Maximum << Selected;
}

void felist::Load(inputfile& SaveFile)
{
  SaveFile >> Entry >> Maximum >> Selected;
}

void felist::PrintToFile(cfestring& FileName)
{
  std::ofstream SaveFile(FileName.CStr(), std::ios::out);

  if(!SaveFile.is_open())
    return;

  uint c;

  for(c = 0; c < Description.size(); ++c)
    SaveFile << Description[c]->String.CStr() << std::endl;

  SaveFile << std::endl;

  for(c = 0; c < Entry.size(); ++c)
  {
    if(Entry[c]->ImageKey != NO_IMAGE)
      SaveFile << "   ";

    SaveFile << Entry[c]->String.CStr() << std::endl;
  }
}

void felist::EmptyDescription()
{ Description.resize(1); }
