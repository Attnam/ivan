#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "feio.h"
#include "rawbit.h"
#include "felist.h"

#define STRETCH 5
#define TILE_SIZE 16
cv2 TILE_V2(TILE_SIZE, TILE_SIZE);

int Main(int, char**)
{
  festring OldDirectory;
  std::ifstream IConfigFile("igor.cfg");

  if(IConfigFile.is_open())
    {
      char ch;

      while(IConfigFile.get(ch))
	OldDirectory << ch;
    }

  IConfigFile.close();
  std::cout << "Where is the graphics directory? ";

  if(OldDirectory.GetSize())
    std::cout << '[' << OldDirectory.CStr() << "] ";

  festring Directory;
  char ch;

  while((ch = getchar()) != '\n')
    Directory << ch;

  if(Directory.IsEmpty())
    Directory = OldDirectory;

  if(!Directory.IsEmpty() && Directory[Directory.GetSize() - 1] != '/')
    Directory << '/';

  std::ofstream OConfigFile("igor.cfg");
  OConfigFile << Directory.CStr();
  OConfigFile.close();

  graphics::Init();
  graphics::SetMode("IGOR 1.203", 0, v2(800, 600), false);
  graphics::LoadDefaultFont(Directory + "Font.pcx");
  DOUBLE_BUFFER->ClearToColor(0);

  rawbitmap* CBitmap;
  felist List(CONST_S("Choose file to edit:"));
  List.AddEntry(CONST_S("Char.pcx"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Humanoid.pcx"), LIGHT_GRAY);
  List.AddEntry(CONST_S("Item.pcx"), LIGHT_GRAY);
  List.AddEntry(CONST_S("GLTerra.pcx"), LIGHT_GRAY);
  List.AddEntry(CONST_S("OLTerra.pcx"), LIGHT_GRAY);
  ushort Selected;
  festring FileName;
  List.SetPos(v2(300, 250));
  List.SetWidth(200);

  while((Selected = List.Draw()) & FELIST_ERROR_BIT)
    if(Selected == ESCAPED)
      return 0;

  switch(Selected)
    {
    case 0: FileName = CONST_S("Char.pcx"); break;
    case 1: FileName = CONST_S("Humanoid.pcx"); break;
    case 2: FileName = CONST_S("Item.pcx"); break;
    case 3: FileName = CONST_S("GLTerra.pcx"); break;
    case 4: FileName = CONST_S("OLTerra.pcx"); break;
    }

  CBitmap = new rawbitmap(Directory + FileName);
  bitmap CursorBitmap(Directory + "Cursor.pcx");
  CursorBitmap.ActivateFastFlag();
  v2 Cursor(0, 0);
  int k = 0;
  Selected = 0;
  packcol16 Color[4] = { MakeRGB16(47, 131, 95), MakeRGB16(123, 0, 127), MakeRGB16(0, 131, 131), MakeRGB16(175, 131, 0) };
  std::vector<v2> DrawQueue;
  uchar TempBuffer[256];
  blitdata B1 = { DOUBLE_BUFFER,
		  { 0, 0 },
		  { RES.X - STRETCH * 16 - 10, RES.Y - STRETCH * 16 - 10 },
		  { TILE_SIZE, TILE_SIZE },
		  { STRETCH } };
  blitdata B2 = { DOUBLE_BUFFER,
		  { 0, 0 },
		  { RES.X - STRETCH * 32 - 20, RES.Y - STRETCH * 16 - 10 },
		  { TILE_SIZE, TILE_SIZE },
		  { STRETCH } };
  blitdata B3 = { DOUBLE_BUFFER,
		  { 0, 0 },
		  { 0, 0 },
		  { TILE_SIZE, TILE_SIZE },
		  { 0 },
		  TRANSPARENT_COLOR };

  for(;;)
    {
      static v2 MoveVector[] = { v2(0, -16), v2(-16, 0), v2(0, 16), v2(16, 0) };
      static int Key[] = { 'w', 'a', 's', 'd' };

      int c;

      for(c = 0; c < 4; ++c)
	{
	  if(Key[c] == k)
	    {
	      v2 NewPos = Cursor + MoveVector[c];

	      if(NewPos.X >= 0 && NewPos.X <= CBitmap->GetSize().X - 16 && NewPos.Y >= 0 && NewPos.Y <= CBitmap->GetSize().Y - 16)
		Cursor = NewPos;

	      break;
	    }

	  if((Key[c]&~0x20) == k)
	    {
	      v2 NewPos = Cursor + (MoveVector[c] << 2);

	      if(NewPos.X >= 0 && NewPos.X <= CBitmap->GetSize().X - 16 && NewPos.Y >= 0 && NewPos.Y <= CBitmap->GetSize().Y - 16)
		Cursor = NewPos;

	      break;
	    }
	}

      if(k >= 0x31 && k <= 0x34)
	Selected = k - 0x31;
      else if(k == '+')
	CBitmap->AlterGradient(Cursor, TILE_V2, Selected, 1, false);
      else if(k == '-')
	CBitmap->AlterGradient(Cursor, TILE_V2, Selected, -1, false);
      else if(k == '>')
	CBitmap->AlterGradient(Cursor, TILE_V2, Selected, 1, true);
      else if(k == '<')
	CBitmap->AlterGradient(Cursor, TILE_V2, Selected, -1, true);
      else if(k == KEY_UP)
	CBitmap->Roll(Cursor, TILE_V2, v2(0, -1), TempBuffer);
      else if(k == KEY_DOWN)
	CBitmap->Roll(Cursor, TILE_V2, v2(0, 1), TempBuffer);
      else if(k == KEY_RIGHT)
	CBitmap->Roll(Cursor, TILE_V2, v2(1, 0), TempBuffer);
      else if(k == KEY_LEFT)
	CBitmap->Roll(Cursor, TILE_V2, v2(-1, 0), TempBuffer);
      else if(k == '=')
	{
	  FONT->Printf(DOUBLE_BUFFER, v2(10, 460), RED, "Select col to swap with [1-4/ESC]");
	  graphics::BlitDBToScreen();

	  for(k = GET_KEY(); k != 0x1B; k = GET_KEY())
	    if(k >= 0x31 && k <= 0x34)
	      {
		CBitmap->SwapColors(Cursor, TILE_V2, Selected, k - 0x31);
		break;
	      }
	}
      else if(k == 0x1B)
	{
	  FONT->Printf(DOUBLE_BUFFER, v2(10, 460), RED, "Save? [y/n/c]");
	  graphics::BlitDBToScreen();

	  for(;;)
	    {
	      k = GET_KEY();

	      if(k == 'y' || k == 'Y')
		{
		  CBitmap->Save(Directory + FileName);
		  delete CBitmap;
		  return 0;
		}

	      if(k == 'n' || k == 'N')
		{
		  delete CBitmap;
		  return 0;
		}

	      if(k == 'c' || k == 'C')
		break;
	    }
	}
      else if(k == 'p')
	{
	  std::vector<v2>::iterator i = std::find(DrawQueue.begin(), DrawQueue.end(), Cursor);

	  if(i == DrawQueue.end())
	    DrawQueue.push_back(Cursor);
	  else
	    DrawQueue.erase(i);
	}
      else if(k == 'c')
	DrawQueue.clear();

      DOUBLE_BUFFER->ClearToColor(0);
      DOUBLE_BUFFER->Fill(0, 0, CBitmap->GetSize(), 0xF81F);
      CBitmap->MaskedBlit(DOUBLE_BUFFER, v2(0, 0), v2(0, 0), CBitmap->GetSize(), Color);
      DOUBLE_BUFFER->DrawRectangle(RES.X - STRETCH * 16 - 12, RES.Y - STRETCH * 16 - 12, RES.X - 9, RES.Y - 9, DARK_GRAY, true);
      DOUBLE_BUFFER->DrawRectangle(RES.X - STRETCH * 32 - 22, RES.Y - STRETCH * 16 - 12, RES.X - STRETCH * 16 - 19, RES.Y - 9, DARK_GRAY, true);
      B1.Src = Cursor;
      DOUBLE_BUFFER->StretchBlit(B1);
      FONT->Printf(DOUBLE_BUFFER, v2(10, 480), WHITE, "Control cursor: wasd and WASD");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 490), WHITE, "Select m-col: 1-4");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 500), WHITE, "Safely alter gradient: ±");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 510), WHITE, "Power alter gradient: <>");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 520), WHITE, "Swap m-cols: =");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 530), WHITE, "Push to / pop from draw queue: p");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 540), WHITE, "Clear draw queue: c");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 550), WHITE, "Roll picture: arrow keys");
      FONT->Printf(DOUBLE_BUFFER, v2(10, 570), WHITE, "MColor selected: %d", Selected + 1);
      FONT->Printf(DOUBLE_BUFFER, v2(10, 580), WHITE, "Current position: (%d, %d)", Cursor.X, Cursor.Y);

      for(c = 0; c < DrawQueue.size(); ++c)
	{
	  B2.Src = DrawQueue[c];
	  DOUBLE_BUFFER->StretchBlit(B2);
	  B3.Dest = DrawQueue[c];
	  CursorBitmap.NormalMaskedBlit(B3);
	}

      B3.Dest = Cursor;
      CursorBitmap.NormalMaskedBlit(B3);
      graphics::BlitDBToScreen();
      k = GET_KEY();
    }

  return 1;
}

