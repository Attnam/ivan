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
#include <bitset>

#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "rawbit.h"
#include "save.h"
#include "festring.h"
#include "dbgmsgproj.h"

const felist* FelistCurrentlyDrawn = 0;

col16 felist::colSelectedBkg = MakeRGB16(8,8,8);//TRANSPARENT_COLOR;
v2 felist::v2SelectedPos = v2(0,0);
v2 felist::v2DefaultEntryImageSize=v2(0,0);

truth felist::isAnyFelistCurrentlyDrawn(){
  return FelistCurrentlyDrawn!=NULL;
}

bool felist::PrepareListItemAltPosBackground(blitdata& rB,bool bAltPosFullBkg){
  if(FelistCurrentlyDrawn==NULL)return false;

  rB.Src = felist::GetCurrentListSelectedItemPos();

//  v2 v2ItemFinalSize(rB.Border.X*rB.Stretch, rB.Border.Y*rB.Stretch);
  v2 v2ItemFinalSize(rB.Border*rB.Stretch);

  int iExtraH = v2ItemFinalSize.Y/4;

  // scaled up item pos
  rB.Dest.X=FelistCurrentlyDrawn->v2OriginalPos.X;//B.Dest.X=5;
  rB.Dest.Y=rB.Src.Y - v2ItemFinalSize.Y/2;
  if(rB.Dest.Y<0)rB.Dest.Y=0;

  // full background where all items will be drawn above it
  if(bAltPosFullBkg){
    v2 v2TopLeft;
    v2TopLeft.X = FelistCurrentlyDrawn->v2OriginalPos.X;
    v2TopLeft.Y = FelistCurrentlyDrawn->Pos.Y - iExtraH;

    v2 v2Border;
    v2Border.X = v2ItemFinalSize.X;
    v2Border.Y = FelistCurrentlyDrawn->v2FinalPageSize.Y + iExtraH*2;

    rB.Bitmap->Fill(v2TopLeft, v2Border, BLACK);

    // full background outline
    graphics::DrawRectangleOutlineAround(rB.Bitmap, v2TopLeft, v2Border, DARK_GRAY, true);
  }

  return true;
}

truth FelistDrawController()
{
  FelistCurrentlyDrawn->DrawPage(DOUBLE_BUFFER,NULL);
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
  felistdescription() = default;
  felistdescription(cfestring& String, col16 Color)
  : String(String), Color(Color) { }
  festring String;
  col16 Color;
};

felist::felist(cfestring& Topic, col16 TopicColor, uint Maximum)
: Maximum(Maximum), Selected(0), Pos(10, 10), Width(780),
  PageLength(30), BackColor(0), Flags(SELECTABLE|FADE), FirstDrawNoFade(false),
  UpKey(KEY_UP), DownKey(KEY_DOWN), EntryDrawer(0), v2FinalPageSize(0,0)
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

uint felist::GetMouseSelectedEntry(v2 v2MousePos)
{
  for(int i=0;i<vEntryRect.size();i++){
    const EntryRect& er = vEntryRect[i];
    if(
        v2MousePos.X > er.v2TopLeft.X     && v2MousePos.Y > er.v2TopLeft.Y &&
        v2MousePos.X < er.v2BottomRight.X && v2MousePos.Y < er.v2BottomRight.Y
    ){
      Selected = er.iSelectableIndex; DBG2("insideRectangle",er.iSelectableIndex);
      return er.iSelectableIndex;
    }
  }

  return -1; //not -1 actually as uint but works.. :/ just dont MmAeTsHs with it..
}

void felist::SetFirstDrawNoFade(bool b)
{
  FirstDrawNoFade=b;
}

bool bAllowMouseSelect=false;
void felist::SetAllowMouse(bool b)
{
  bAllowMouseSelect=b;
}

uint felist::Draw()
{
  uint FlagsChk = Flags;

  if(Flags & SELECTABLE){
    if(PageLength > 26)PageLength=26; //constraint limit from aA to zZ as there is no coded support beyond these keys anyways...
  }else{
    for(int i=0;i<Entry.size();i++)
      if(Entry[i]->ImageKey != NO_IMAGE){
        /**
         * This allows much more visible entries when the list have no images.
         *
         * But this is still a dumb guesser, because it considers all entries will have images.
         *
         * The difficulty is because having a fixed page length, even if the contents of each page may differ,
         * we are unable to precisely calculate how many entries will fit on each page.
         *
         * So, opting for the worst case (all are images) is the safest option.
         */
        PageLength/=2;
        break;
      }
  }

  while(Entry.size() && Entry[GetLastEntryIndex()]->String.IsEmpty())
    Pop();

  if(Entry.empty())
    return LIST_WAS_EMPTY;

  FelistCurrentlyDrawn = this;DBGLN;
  globalwindowhandler::SetGetKeyTimeout(10,'.'); //using the min millis value grants mouse will be updated most often possible

  if(globalwindowhandler::ControlLoopsInstalled())
    globalwindowhandler::InstallControlLoop(FelistDrawController);

  bitmap BackGround(RES);
  BackGround.ActivateFastFlag();
  bitmap* Buffer;

  if(Flags & FADE)
  {DBGLN;
    Buffer = new bitmap(RES, 0);
    Buffer->ActivateFastFlag();
    BackGround.ClearToColor(0);
  }
  else
  {DBGLN;
    Buffer = DOUBLE_BUFFER;
    Buffer->FastBlit(&BackGround);
  }

  uint c;
  uint Return, Selectables = 0;
  truth JustSelectMoveOnce = false;

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

  bool bSafeScrollToEnd=false; //page per page
  bool bWaitKeyUp=false;
  bool bClearKeyBufferOnce=false;
  graphics::PrepareBeforeDrawingFelist();
  v2 v2MousePosPrevious=globalwindowhandler::GetMouseLocation();
  globalwindowhandler::ConsumeMouseEvent(); //this call is important to clear the last mouse action outside felist
  int iDrawCount=0;
  for(;;)
  {
    if(FlagsChk != Flags)ABORT("flags changed during felist draw %s %s",std::bitset<16>(FlagsChk).to_string().c_str(), std::bitset<16>(Flags).to_string().c_str());

    graphics::DrawAtDoubleBufferBeforeFelistPage(); // here prevents full dungeon blink
    truth AtTheEnd = DrawPage(Buffer,&v2FinalPageSize,&vEntryRect);DBGLN;

    if(FirstDrawNoFade && iDrawCount == 0){
      JustSelectMoveOnce=true;
    }
    iDrawCount++;

    if(Flags & FADE)
    {DBGLN;
      if(JustSelectMoveOnce)
      {DBGLN;
        Buffer->FastBlit(DOUBLE_BUFFER);
        graphics::BlitDBToScreen();
      }else{DBGLN;
        Buffer->FadeToScreen();
      }

      JustSelectMoveOnce = false;
    }else{DBGLN;
      if(Buffer != DOUBLE_BUFFER)ABORT("felist non-fade Buffer != DOUBLE_BUFFER");
      graphics::BlitDBToScreen();
      graphics::DrawAtDoubleBufferBeforeFelistPage(); //TODO confirm this (the above one doesnt suffice?): here helps on hiding unstretched static squares before the NEXT felist page be drawn
    }

    /**
     * '.' is a skipper to ignore actions,
     * this means if felist expects '.' to be pressed,
     * that will fail TODO so may be this should be configurable? just expose defaulting to '.'
     */
    static uint DefaultAnswer='.';
    uint Pressed=DefaultAnswer;
    bool bLeftMouseButtonClick=false;
    bool bMouseButtonClick=false;
    bool bJustRefreshOnce=false;
    if(bSafeScrollToEnd)
      Pressed=KEY_PAGE_DOWN;
    else{
      for(;;){
        /**
         * every section here may break the loop and they are prioritized
         */

        ////////////////////////////// scroll to end by-pass
        if(bSafeScrollToEnd){
//          JustSelectMoveOnce=true;
          break;
        }

        /////////////////////////////////////////// MOUSE ///////////////////////////////////////
        v2 v2MousePos = globalwindowhandler::GetMouseLocation();
        v2MousePos/=graphics::GetScale();
        mouseclick mc=globalwindowhandler::ConsumeMouseEvent();
        static v2 v2MousePosFix;
        ////////////////////////////// mouse click
        if(bAllowMouseSelect && mc.btn!=-1){ DBG1(mc.btn);
          switch(mc.btn){
          case 1:
            bLeftMouseButtonClick=true;
            bMouseButtonClick=true;
            break;
          }

          if(bMouseButtonClick){
            /**
             * when clicking the pos is correct/matches the visible cursor,
             * this problem actually happens in fullscreen mode only
             */
            if(v2MousePos != mc.pos){
              v2MousePosFix = v2MousePos - mc.pos;
            }else
              v2MousePosFix=v2();

            uint iSel = GetMouseSelectedEntry(mc.pos); //make sure selected is the one at mouse pos no matter the highlight
            if(iSel!=-1){
              Selected=iSel;
              break;
            }
          }
        }
        v2MousePos-=v2MousePosFix;

        ////////////////////////////// mouse wheel scroll
        if(bAllowMouseSelect && mc.wheelY!=0){
          Pressed = mc.wheelY < 0 ? KEY_PAGE_DOWN : KEY_PAGE_UP; //just to simplify it
          break;
        }

        ////////////////////////////// mouse move/hover (to not be hindered by getkey timeout
        static clock_t lastMouseMoveTime=clock(); //static as mouse is a global thing
        if(bAllowMouseSelect && v2MousePosPrevious!=v2MousePos){ //DBG2(DBGAV2(v2MousePosPrevious),DBGAV2(v2MousePos));
          bool bSelChanged = false;

          uint iSel = GetMouseSelectedEntry(v2MousePos);
          if(iSel!=-1)
            bSelChanged = Selected!=iSel;

          v2MousePosPrevious=v2MousePos; //reset
          lastMouseMoveTime=clock();

          if(bSelChanged){
            Selected = iSel; DBG1(iSel);
            bJustRefreshOnce=true;
            JustSelectMoveOnce=true;
            break;
          }
        }
//        if((Flags & ALLOW_MOUSE_SELECT) && (clock() - lastMouseMoveTime) < (0.5 * CLOCKS_PER_SEC)){
//        }

        ///////////////////////////////////////// KEYBOARD /////////////////////////////////////
        ////////////////////////////// normal key press
        bool bClearKeyBuffer=false;
        if(bClearKeyBufferOnce){
          bClearKeyBuffer=true;
          bClearKeyBufferOnce=false;
        }
        Pressed = GET_KEY(bClearKeyBuffer);DBG1(Pressed); //see iTimeoutMillis above
        if(Pressed!=DefaultAnswer)
          break;

      }
    }
    DBGLN;

//    if(bMouseHovering && !bMouseButtonClick)
    if(bJustRefreshOnce)
      continue;

    if((Flags & SELECTABLE) && Pressed > 64 // 65='A' 90='Z'
       && Pressed < 91 && Pressed - 65 < PageLength
       && Pressed - 65 + PageBegin < Selectables)
    {DBGLN;
      Return = Selected = Pressed - 65 + PageBegin;
      bWaitKeyUp=true;
      break;
    }

    if((Flags & SELECTABLE) && Pressed > 96 // 97='a' 122='z'
       && Pressed < 123 && Pressed - 97 < PageLength
       && Pressed - 97 + PageBegin < Selectables)
    {DBGLN;
      Return = Selected = Pressed - 97 + PageBegin;
      bWaitKeyUp=true;
      break;
    }

    if((Flags & SELECTABLE) && Pressed == UpKey)
    {DBGLN;
      if(Selected)
      {
        --Selected;

        if(Selected < PageBegin)
        {
          BackGround.FastBlit(Buffer);
          PageBegin -= PageLength;
        }
        else
          JustSelectMoveOnce = true;
      }
      else
      {
        for(c = 0, Selected = 0; c < Entry.size(); ++c)
          if(Entry[c]->Selectable)
            ++Selected;

        --Selected;

        if(PageBegin == Selected - Selected % PageLength)
          JustSelectMoveOnce = true;
        else
        {
          BackGround.FastBlit(Buffer);
          PageBegin = Selected - Selected % PageLength;
        }
      }

      if(globalwindowhandler::IsLastSDLkeyEventWasKeyUp())
        bClearKeyBufferOnce=true;

      continue;
    }

    if((Flags & SELECTABLE) && Pressed == DownKey)
    {DBGLN;
      if(!AtTheEnd || Selected != Selectables - 1)
      {
        ++Selected;

        if(Selected > PageBegin + PageLength - 1)
        {
          BackGround.FastBlit(Buffer);
          PageBegin += PageLength;
        }
        else
          JustSelectMoveOnce = true;
      }
      else
      {
        if(!PageBegin)
          JustSelectMoveOnce = true;
        else
          BackGround.FastBlit(Buffer);

        Selected = PageBegin = 0;
      }

      if(globalwindowhandler::IsLastSDLkeyEventWasKeyUp())
        bClearKeyBufferOnce=true;

      continue;
    }

    if((Flags & SELECTABLE) && (Pressed == KEY_ENTER || bLeftMouseButtonClick))
    {
      Return = Selected;
      if(!bLeftMouseButtonClick)
        bWaitKeyUp=true;
      break;
    }

    if(Pressed == KEY_ESC)
    {
      Return = ESCAPED;
      break;
    }

    /**
     * PAGE navigation
     */
    DBG8(Entry.size(),Selectables,Selectables/PageLength,KEY_SPACE,KEY_PAGE_UP,KEY_PAGE_DOWN,KEY_HOME,KEY_END);
    bool bNav=false; //navigating
    if(!bNav && Pressed == KEY_PAGE_DOWN)bNav=true;
    if(!bNav && Pressed == KEY_PAGE_UP)bNav=true;
    if(!bNav && Pressed == KEY_HOME)bNav=true;
    if(!bNav && Pressed == KEY_END)bNav=true; //TODO ? END key usage is getting complicated, disabled for now:
    if(!bNav && Pressed == KEY_SPACE && !AtTheEnd)bNav=true; //space works as PGDN
    if(Pressed == KEY_SPACE || bNav){DBGLN;
      int iDir = 1;
      if(Flags & INVERSE_MODE)
        iDir *= -1;
      if(Pressed == KEY_PAGE_UP){ //TODO confirm that this inverts the INVERSE_MODE behavior
        iDir *= -1;
      }

      int iCurrent = PageBegin + iDir*PageLength;
      if(iCurrent<0) //PageBegin is uint ...
        iCurrent=0;
    }

    if(!bNav) {
      if(Pressed == KEY_SPACE) //to work stictly as on the help info
        if( (AtTheEnd && !(Flags & INVERSE_MODE)) || (!PageBegin && (Flags & INVERSE_MODE)) )
        {
          Return = NOTHING_SELECTED;
          break;
        }
    } else {
      BackGround.FastBlit(Buffer);

      int iDir = 1;
      if(Flags & INVERSE_MODE)
        iDir *= -1;
      if(Pressed == KEY_PAGE_UP) //TODO confirm that this inverts the INVERSE_MODE behavior
        iDir *= -1;

      int iPB = PageBegin + iDir*PageLength;DBG1(iPB);
      if(iPB<0) //PageBegin is uint ...
        iPB=0;

      /**
       * overriders
       * obs.: pgdn and space are default "advance page" action
       */
      if(AtTheEnd && Pressed == KEY_PAGE_DOWN){
        bSafeScrollToEnd=false;
        Selected = Selectables-1;
        continue; //do nothing
      }

      if(Flags & INVERSE_MODE ? Pressed == KEY_END : Pressed == KEY_HOME) // go to first
        iPB=0;

      if(Flags & INVERSE_MODE ? Pressed == KEY_HOME : Pressed == KEY_END){ // go to last
        if(Entry.size()<=PageLength){ //only one page
          Selected = Selectables-1;
          continue; //do nothing
        }

        if(AtTheEnd){
          Selected = Selectables-1;
          continue; //do nothing
        }

        bSafeScrollToEnd=true; // will just page down once, as this is the default action, otherwise should `continue;`
      }

      // fail safe LAST check
      if(iPB >= Selectables){ DBG3("how it happened?",iPB,Selectables);
        continue; //do nothing
      }

      // apply
      PageBegin = iPB;

      DBG3(PageBegin,Pressed,iDir);

      if(Flags & SELECTABLE)
        Selected = PageBegin;
    }
  }

  if(!(Flags & FADE))
  {
    if(Flags & DRAW_BACKGROUND_AFTERWARDS){
      BackGround.FastBlit(DOUBLE_BUFFER);
    }

    if(Flags & BLIT_AFTERWARDS){
      graphics::BlitDBToScreen();
    }
  }
  else
    delete Buffer;

  globalwindowhandler::DeInstallControlLoop(FelistDrawController);

  FelistCurrentlyDrawn=NULL;DBGLN;

  #ifdef FELIST_WAITKEYUP
  if(bWaitKeyUp && !globalwindowhandler::IsLastSDLkeyEventWasKeyUp())
    for(;;)
      if(WAIT_FOR_KEY_UP()) //TODO it is NOT waiting, why? that's the reason of `for(;;)` above...
        break;
  #endif

  globalwindowhandler::ResetGetKeyTimeout();
  return Return;
}

static festring Str;

bool felist::IsEntryDrawingAtValidPos(bitmap* Buffer,v2 pos){
  if(v2DefaultEntryImageSize.X==0 || v2DefaultEntryImageSize.Y==0)ABORT("v2DefaultEntryImageSize not set.");
  return Buffer->IsValidPos(pos.X+v2DefaultEntryImageSize.X, pos.Y+v2DefaultEntryImageSize.Y);
}

truth felist::DrawPage(bitmap* Buffer, v2* pv2FinalPageSize, std::vector<EntryRect>* pvEntryRect) const
{
  uint LastFillBottom = Pos.Y + 23 + Description.size() * 10;
  DrawDescription(Buffer);

  uint c, i; // c == entry index, i == selectable index

  for(c = 0, i = 0; i != PageBegin; ++c)
    if(Entry[c]->Selectable)
      ++i;

  while(!Entry[c]->Selectable && Entry[c]->String.IsEmpty()) ++c;
  std::vector<festring> Chapter;

  if(pvEntryRect!=NULL)
    (*pvEntryRect).clear();

  for(;;)
  {
    Str.Empty();
    uint Marginal = Entry[c]->Marginal;

    bool bIsSelectable = (Flags & SELECTABLE) && Entry[c]->Selectable;
    if(bIsSelectable){
      Str << char('A' + (i - PageBegin)) << ": ";
      Marginal += 3;
    }

    Str << Entry[c]->String;

    int iTLX=Pos.X + 3;
    int iTLY=LastFillBottom;
    int iWidth = Width - 6;
    int iHeight = -1;

    truth isTheSelectedOne = bIsSelectable && Selected == i;

    col16 colBkg = BackColor;
    if(isTheSelectedOne && colSelectedBkg!=TRANSPARENT_COLOR) //colBkg==BLACK
      colBkg=colSelectedBkg;

    if(Entry[c]->ImageKey != NO_IMAGE)
    {
      if(Str.GetSize() <= (Width - 50) >> 3)
      {
        Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=20, colBkg);

        v2 v2EntryPos = v2(Pos.X + 13, LastFillBottom);
        if(EntryDrawer && IsEntryDrawingAtValidPos(Buffer,v2EntryPos)){
          EntryDrawer(Buffer,
                      v2EntryPos,
                      Entry[c]->ImageKey);
        }

        if(isTheSelectedOne){
          FONT->PrintfUnshaded(Buffer, v2(Pos.X + 38, LastFillBottom + 5),
                               WHITE, "%s", Str.CStr());
          felist::v2SelectedPos = v2EntryPos;
        }else
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

        v2 v2EntryPos = v2(Pos.X + 13, PictureTop);

        for(uint l = 0; l < ChapterSize; ++l)
        {
          Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=10, colBkg);

          if(isTheSelectedOne) {
            FONT->PrintfUnshaded(Buffer, v2(Pos.X + 38, LastFillBottom + 1),
                                 WHITE, "%s", Chapter[l].CStr());
            felist::v2SelectedPos = v2EntryPos;
          } else
            FONT->Printf(Buffer, v2(Pos.X + 37, LastFillBottom),
                         Entry[c]->Color, "%s", Chapter[l].CStr());

          LastFillBottom += 10;
        }

        if(EntryDrawer && IsEntryDrawingAtValidPos(Buffer,v2EntryPos)){
          EntryDrawer(Buffer,
                      v2EntryPos,
                      Entry[c]->ImageKey);
        }
      }
    }
    else
    {
      uint ChapterSize = festring::SplitString(Str, Chapter,
                                               (Width - 26) >> 3,
                                               Marginal);

      for(uint l = 0; l < ChapterSize; ++l)
      {
        Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=10, colBkg);

        if(isTheSelectedOne){
          FONT->PrintfUnshaded(Buffer, v2(Pos.X + 14, LastFillBottom + 1),
                               WHITE, "%s", Chapter[l].CStr());
        }else
          FONT->Printf(Buffer, v2(Pos.X + 13, LastFillBottom),
                       Entry[c]->Color, "%s", Chapter[l].CStr());

        LastFillBottom += 10;
      }
    }

    bool bBreak=false;
    if(
        (i - PageBegin == PageLength - 1 && Entry[c]->Selectable)
        ||
        c == Entry.size() - 1
      )
    {
      if((!(Flags & INVERSE_MODE) && c != Entry.size() - 1)
         || ((Flags & INVERSE_MODE) && PageBegin))
      {
        Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=30, BackColor);
        FONT->Printf(Buffer, v2(Pos.X + 13, LastFillBottom + 10), WHITE,
                     "- Press PgUp/PgDn/Home/End or SPACE to continue, ESC to exit -");
        LastFillBottom += 30;
      }
      else
      {
        Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=10, BackColor);
        LastFillBottom += 10;
      }

      int iLeft   = Pos.X + 1,
          iTop    = Pos.Y + 1,
          iRight  = Pos.X + Width - 2,
          iBottom = LastFillBottom + 1;
      Buffer->DrawRectangle(iLeft,iTop,iRight,iBottom, DARK_GRAY, true);

      if(pv2FinalPageSize!=NULL){
        pv2FinalPageSize->X = iRight-iLeft;
        pv2FinalPageSize->Y = iBottom-iTop;
      }

      bBreak=true;
    }

    if(iHeight==-1)
      ABORT("list entry height was not set! %d %d %d %d",i,c,iTLX,iTLY);
    DBG7(bIsSelectable,i,c,iTLX,iTLY,iWidth,iHeight);
    if(pvEntryRect!=NULL && bIsSelectable){
      EntryRect er;
      er.iSelectableIndex=i;
      er.v2TopLeft=v2(iTLX, iTLY);
      er.v2BottomRight=(er.v2TopLeft+v2(iWidth,iHeight));
      (*pvEntryRect).push_back(er);
    }

    if(bBreak)
      break;

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
      Color = MakeRGB16(GetRed16(Color)   - ((GetRed16(Color) * 3
                                              * Index / PageLength) >> 2),
                        GetGreen16(Color) - ((GetGreen16(Color) * 3
                                              * Index / PageLength) >> 2),
                        GetBlue16(Color)  - ((GetBlue16(Color) * 3
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
{
  for(uint c = 1; c < Description.size(); ++c)
    delete Description[c];

  Description.resize(1);
}
