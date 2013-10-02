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

#include <cstdarg>
#include <cctype>

#include "message.h"
#include "festring.h"
#include "felist.h"
#include "game.h"
#include "graphics.h"
#include "save.h"
#include "bitmap.h"
#include "igraph.h"

felist msgsystem::MessageHistory(CONST_S("Message history"), WHITE, 128);
festring msgsystem::LastMessage;
festring msgsystem::BigMessage;
int msgsystem::Times;
v2 msgsystem::Begin, msgsystem::End;
truth msgsystem::Enabled = true;
truth msgsystem::BigMessageMode = false;
truth msgsystem::MessagesChanged = true;
bitmap* msgsystem::QuickDrawCache = 0;
int msgsystem::LastMessageLines;

void msgsystem::AddMessage(cchar* Format, ...)
{
  if(!Enabled)
    return;

  if(BigMessageMode && BigMessage.GetSize() >= 512)
    LeaveBigMessageMode();

  char Message[1024];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Message, Format, AP);
  va_end(AP);

  festring Buffer(Message);

  if(!Buffer.GetSize())
    ABORT("Empty message request!");

  Buffer.Capitalize();

  /* Comment the first line and uncomment the second before the release! */

  if(isalpha(Buffer[Buffer.GetSize() - 1]))
    //Buffer << " (this sentence isn't terminated correctly because Hex doesn't know grammar rules)";
    Buffer << '.';

  if(BigMessageMode)
  {
    if(BigMessage.GetSize())
      BigMessage << ' ';

    BigMessage << Buffer;
    return;
  }

  ivantime Time;
  game::GetTime(Time);

  if(Buffer == LastMessage)
  {
    for(int c = 0; c < LastMessageLines; ++c)
      MessageHistory.Pop();

    ++Times;
    End = v2(Time.Hour, Time.Min);
  }
  else
  {
    Times = 1;
    Begin = End = v2(Time.Hour, Time.Min);
    LastMessage = Buffer;
    LastMessage.EnsureOwnsData();
  }

  festring Temp;
  Temp << Begin.X << ':';

  if(Begin.Y < 10)
    Temp << '0';

  Temp << Begin.Y;

  if(Begin != End)
  {
    Temp << '-' << End.X << ':';

    if(End.Y < 10)
      Temp << '0';

    Temp << End.Y;
  }

  if(Times != 1)
    Temp << " (" << Times << "x)";

  Temp << ' ';
  int Marginal = Temp.GetSize();
  Temp << Buffer;

  std::vector<festring> Chapter;
  festring::SplitString(Temp, Chapter, 78, Marginal);

  for(uint c = 0; c < Chapter.size(); ++c)
    MessageHistory.AddEntry(Chapter[c], WHITE);

  MessageHistory.SetSelected(MessageHistory.GetLastEntryIndex());
  LastMessageLines = Chapter.size();
  MessagesChanged = true;
}

void msgsystem::Draw()
{
  truth WasInBigMessageMode = BigMessageMode;
  LeaveBigMessageMode();

  if(MessagesChanged)
  {
    MessageHistory.QuickDraw(QuickDrawCache, 8);
    MessagesChanged = false;
  }

  v2 Size = QuickDrawCache->GetSize();
  int Y = RES.Y - 122;
  blitdata B = { DOUBLE_BUFFER,
		 { 0, 0 },
		 { 13, Y },
		 { Size.X, Size.Y },
		 { 0 },
		 0,
		 0 };

  QuickDrawCache->NormalBlit(B);
  igraph::BlitBackGround(v2(13, Y), v2(1, 1));
  igraph::BlitBackGround(v2(12 + Size.X, Y), v2(1, 1));
  igraph::BlitBackGround(v2(13, Y + Size.Y - 1), v2(1, 1));
  igraph::BlitBackGround(v2(12 + Size.X, Y + Size.Y - 1), v2(1, 1));

  if(WasInBigMessageMode)
    EnterBigMessageMode();
}

void msgsystem::DrawMessageHistory()
{
  MessageHistory.Draw();
}

void msgsystem::Format()
{
  MessageHistory.Empty();
  LastMessage.Empty();
  MessagesChanged = true;
  BigMessageMode = false;
}

void msgsystem::Save(outputfile& SaveFile)
{
  MessageHistory.Save(SaveFile);
  SaveFile << LastMessage << Times << Begin << End;
}

void msgsystem::Load(inputfile& SaveFile)
{
  MessageHistory.Load(SaveFile);
  SaveFile >> LastMessage >> Times >> Begin >> End;
}

void msgsystem::ScrollDown()
{
  if(MessageHistory.GetSelected() < MessageHistory.GetLastEntryIndex())
  {
    MessageHistory.EditSelected(1);
    MessagesChanged = true;
  }
}

void msgsystem::ScrollUp()
{
  if(MessageHistory.GetSelected())
  {
    MessageHistory.EditSelected(-1);
    MessagesChanged = true;
  }
}

void msgsystem::LeaveBigMessageMode()
{
  BigMessageMode = false;

  if(BigMessage.GetSize())
  {
    AddMessage("%s", BigMessage.CStr());
    BigMessage.Empty();
  }
}

void msgsystem::Init()
{
  QuickDrawCache = new bitmap(v2((game::GetScreenXSize() << 4) + 6, 106));
  QuickDrawCache->ActivateFastFlag();
  game::SetStandardListAttributes(MessageHistory);
  MessageHistory.AddFlags(INVERSE_MODE);
}

void msgsystem::ThyMessagesAreNowOld()
{
  if(MessageHistory.GetColor(MessageHistory.GetLastEntryIndex()) == WHITE)
    MessagesChanged = true;

  for(uint c = 0; c < MessageHistory.GetLength(); ++c)
    MessageHistory.SetColor(c, LIGHT_GRAY);
}
