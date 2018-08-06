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
#include <algorithm>
#include <sstream>

#include "feio.h"
#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "rawbit.h"
#include "save.h"
#include "specialkeys.h"
#include "festring.h"
#include "dbgmsgproj.h"

const felist* FelistCurrentlyDrawn = 0;

col16 felist::colSelectedBkg = MakeRGB16(8,8,8);//TRANSPARENT_COLOR;
v2 felist::v2SelectedPos = v2(0,0);
v2 felist::v2DefaultEntryImageSize=v2(0,0);

truth felist::isAnyFelistCurrentlyDrawn(){
  return FelistCurrentlyDrawn!=NULL;
}

int ListItemAltPosBackgroundMinY=0;
void felist::SetListItemAltPosMinY(int iY)
{
  ListItemAltPosBackgroundMinY=iY;
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
  if(rB.Dest.Y<ListItemAltPosBackgroundMinY)
    rB.Dest.Y=ListItemAltPosBackgroundMinY; //to not hide stuff above it

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
  festring Help;
};

felistentry::felistentry(cfestring& String, col16 Color,
                         uint Marginal, uint ImageKey, truth Selectable)
: String(String), Color(Color), Marginal(Marginal),
  ImageKey(ImageKey), Selectable(Selectable), Help("")
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
: Maximum(Maximum), Selected(0), bJustRestoreEntries(false),
  Pos(10, 32), //y=32 gum to let filter be nicely readable always
  Width(780), PageLength(30), BackColor(0), Flags(SELECTABLE|FADE), FirstDrawNoFade(false),
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

uint felist::GetMouseSelectedEntry(v2 v2MousePosOverride)
{
  for(int i=0;i<vEntryRect.size();i++){
    const EntryRect& er = vEntryRect[i];
    if(globalwindowhandler::IsMouseAtRect(er.v2TopLeft,er.v2BottomRight,false,v2MousePosOverride)){
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

uint felist::ScrollToLastPage(bool& JustRedrawEverythingOnce,bitmap& BackGround,bitmap* Buffer)
{
  Selected=0;

  for(uint c = 0; c < Entry.size(); ++c)
    if(Entry[c]->Selectable)
      ++Selected;

  --Selected;

  uint pb = Selected - Selected % PageLength;
  if(PageBegin == pb)
    JustRedrawEverythingOnce = true;
  else
    BackGround.FastBlit(Buffer);

  return pb;
}

felistentry* RetrieveSelectableEntry(std::vector<felistentry*> Entry,uint Selected){
  uint iSel=0;
  for(uint i=0;i<Entry.size();i++){
    if(!Entry[i]->Selectable)continue;

    if(iSel==Selected)
      return Entry[i];

    iSel++;
  }
  return NULL;
}

typedef std::map<festring,festring> filtermap;
filtermap FilterMap;

festring felist::GetFilter()
{
  festring key = Description[0]->String;
  filtermap::iterator Iterator = FilterMap.find(key);
  return Iterator != FilterMap.end() ? Iterator->second : "";
}

void felist::SetFilter(festring Filter)
{
  festring key = Description[0]->String;
  filtermap::iterator iter = FilterMap.find(key);
  if(iter!=FilterMap.end())
    FilterMap.erase(iter);
  FilterMap.insert(std::make_pair(key,Filter));
}

void felist::UpdateFilterDesc()
{
  festring Filter=GetFilter();

  static festring fsF="[Filter '";
  festring fsD = fsF+Filter+"']";
  bool bFound=false;
  for(int i=0;i<Description.size();i++){
    if(std::string(Description[i]->String.CStr()).substr(0,fsF.GetSize())==fsF.CStr()){
      if(Filter.IsEmpty())
        Description.erase(Description.begin()+i);
      else
        Description[i]->String = fsD;
      bFound=true;
      break;
    }
  }
  if(!bFound && !Filter.IsEmpty())
    AddDescription(fsD,WHITE);
}

void felist::ApplyFilter()
{
  festring Filter=GetFilter();

  UpdateFilterDesc();

  if(!Filter.IsEmpty()){
    Entry.clear();
    std::string sFilter=Filter.CStr();DBG1(sFilter);
    std::transform(sFilter.begin(), sFilter.end(), sFilter.begin(), ::tolower);
    std::string str;
    for(int i=0;i<EntryBkp.size();i++){ //case insensitive
      if(!EntryBkp[i]->Selectable)continue;

      str = EntryBkp[i]->String.CStr();
      std::transform(str.begin(), str.end(), str.begin(), ::tolower); DBG1(str);
      if(str.find(sFilter)!=std::string::npos)
        Entry.push_back(EntryBkp[i]);
    }
    DBG3(Filter.CStr(),EntryBkp.size(),Entry.size());
    if(Entry.empty()){ //filter was invalid
      Entry=EntryBkp;
      SetFilter("");
    }
  }else{
    if(EntryBkp.size()>0)
      Entry=EntryBkp;
  }
}

uint felist::Draw()
{
  EntryBkp=Entry;

  specialkeys::ClearRequest();

  ApplyFilter();

  for(;;){
    uint Return = DrawFiltered();

    if(Return == ESCAPED || Return == LIST_WAS_EMPTY) //TODO FELIST_ERROR_BIT?
      return Return;

    if(Return == NOTHING_SELECTED){ //special condition if has filter
      if(!GetFilter().IsEmpty()){
        ApplyFilter();
        continue;
      }else{
        if(bJustRestoreEntries){
          UpdateFilterDesc();
          Entry=EntryBkp;
          continue;
        }else{
          return NOTHING_SELECTED;
        }
      }
    }

    ////////////////////////// something was chosen ///////////////////////
    DBG3(Return,Entry.size(),EntryBkp.size());
    if(!GetFilter().IsEmpty()){
      /**
       * the filtered index differs from the original index...
       * the matching key will be the entry description/text
       */

      felistentry* fleR = Entry[Return];

      int iSelB=0;
      for(int i=0;i<EntryBkp.size();i++){ DBG3(i,iSelB,EntryBkp[i]->String.CStr());
        if(!EntryBkp[i]->Selectable)continue;

        if(EntryBkp[i]->String==fleR->String){ //TODO  there may have 2 items with identical descriptions tho... but user wouldnt be able to distinguish either right?
          Return = iSelB; DBG1(Return);
          break;
        }

        iSelB++;
      }
    }

    Entry=EntryBkp; //to be ready to proper felist::Empty() with deletion
    EntryBkp.clear();
    return Return;
  }

  return NOTHING_SELECTED; //never reached...
}

void felist::ClearFilter()
{
  SetFilter("");
  ApplyFilter();
}

uint felist::DrawFiltered()
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
         * we are unable to precisely calculate how many entries will fit on each page. TODO right?
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
  globalwindowhandler::SetKeyTimeout(10,'.'); //using the min millis value grants mouse will be updated most often possible

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
  truth JustRedrawEverythingOnce = false;

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

  bool bWaitKeyUp=false;
  bool bClearKeyBufferOnce=false;
  bool bInvM = Flags & INVERSE_MODE;
  graphics::PrepareBeforeDrawingFelist();
  v2 v2MousePosPrevious=globalwindowhandler::GetMouseLocation();
  globalwindowhandler::ConsumeMouseEvent(); //this call is important to clear the last mouse action outside felist
  int iDrawCount=0;
  festring Filter=GetFilter();
  festring fsFilterApplyNew=Filter;
  bool bApplyNewFilter=false;
  for(;;)
  {
    if(FlagsChk != Flags)ABORT("flags changed during felist draw %s %s",std::bitset<16>(FlagsChk).to_string().c_str(), std::bitset<16>(Flags).to_string().c_str());

    graphics::DrawAtDoubleBufferBeforeFelistPage(); // here prevents full dungeon blink
    truth LastEntryVisible = DrawPage(Buffer,&v2FinalPageSize,&vEntryRect);DBGLN;

    if(FirstDrawNoFade && iDrawCount == 0){
      JustRedrawEverythingOnce=true;
    }
    iDrawCount++;

    if(Flags & FADE)
    {DBGLN;
      if(JustRedrawEverythingOnce)
      {DBGLN;
        Buffer->FastBlit(DOUBLE_BUFFER);
        graphics::BlitDBToScreen();
      }else{DBGLN;
        Buffer->FadeToScreen();DBGLN;
      }

      JustRedrawEverythingOnce = false;
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

    for(;;){
      /**
       * every section here may break the loop and they are prioritized
       */

      if(specialkeys::ConsumeEvent(specialkeys::Filter,fsFilterApplyNew)){
        if(Filter != fsFilterApplyNew){DBGLN;
          if(fsFilterApplyNew.IsEmpty())
            bJustRestoreEntries=true;
          bApplyNewFilter=true;
        }else{
          bJustRefreshOnce=true;
        }
        break;
      }else
      if(specialkeys::IsRequestedEvent(specialkeys::CopyToClipboard)){
        if(Flags & SELECTABLE){
          felistentry* fle = RetrieveSelectableEntry(Entry,Selected);
          if(fle!=NULL)
            specialkeys::ConsumeEvent(specialkeys::CopyToClipboard,fle->String);
        }else{
          specialkeys::ClearRequest();
        }
        //TODO copy the entiry list if not selectable? nah...?
      }else
      if(specialkeys::IsRequestedEvent(specialkeys::FocusedElementHelp)){
        festring fs;
        felistentry* fle = RetrieveSelectableEntry(Entry,Selected);
        if(fle!=NULL){
          if(!fle->Help.IsEmpty())
            fs<<fle->Help<<"\n";
          else
            fs<<fle->String<<"\n";
          fs<<"\n";
        }
        fs<<
          "[List Help:]\n"
          " F1 - show this message\n"
          " Ctrl+f - filter entries\n"
          " Home/End/PageUp/PageDown - navigate thru pages\n"
          " ESC - exit the list\n"
          " SPACE - continue (next page or exit if at last one)\n";
        specialkeys::ConsumeEvent(specialkeys::FocusedElementHelp,fs);
        bJustRefreshOnce=true;
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
          JustRedrawEverythingOnce=true;
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
      }DBGLN;
      Pressed = GET_KEY(bClearKeyBuffer);DBG2(Pressed,DefaultAnswer); //see iTimeoutMillis above
//      if(specialkeys::HasEvent()){DBGLN;
//        bJustRefreshOnce=true;
//        break;
//      }
      if(Pressed!=DefaultAnswer)
        break;
    }
    DBGLN;

    if(Pressed == KEY_ESC) // this here grants will be preferred over everything else below
    {
      Return = ESCAPED;
      break;
    }

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
          JustRedrawEverythingOnce = true;
      }
      else
      {
        PageBegin = ScrollToLastPage(JustRedrawEverythingOnce, BackGround, Buffer);
      }

      if(globalwindowhandler::IsLastSDLkeyEventWasKeyUp())
        bClearKeyBufferOnce=true;

      continue;
    }

    if((Flags & SELECTABLE) && Pressed == DownKey)
    {DBGLN;
      if(!LastEntryVisible || Selected != Selectables - 1)
      {
        ++Selected;

        if(Selected > PageBegin + PageLength - 1)
        {
          BackGround.FastBlit(Buffer);
          PageBegin += PageLength;
        }
        else
          JustRedrawEverythingOnce = true;
      }
      else
      {
        if(!PageBegin)
          JustRedrawEverythingOnce = true;
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

    if(bApplyNewFilter){DBGLN;
      break;
    }

    if(Pressed == KEY_ESC) // this here grants will be preferred over everything else below
    {
      Return = ESCAPED;
      break;
    }

    /**
     * PAGE navigation
     */
    DBG8(Entry.size(),Selectables,Selectables/PageLength,KEY_SPACE,KEY_PAGE_UP,KEY_PAGE_DOWN,KEY_HOME,KEY_END);
    bool bNav=false; //navigating
    if(!bNav && Pressed == KEY_SPACE && !(bInvM ? PageBegin==0 : LastEntryVisible))bNav=true; //if at the end, will exit felist
    if(!bNav && Pressed == KEY_PAGE_DOWN)bNav=true;
    if(!bNav && Pressed == KEY_PAGE_UP)bNav=true;
    if(!bNav && Pressed == KEY_HOME)bNav=true;
    if(!bNav && Pressed == KEY_END)bNav=true; //TODO ? END key usage is getting complicated, disabled for now:

    if(!bNav) {DBGLN;
      if(Pressed == KEY_SPACE) //to work stictly as on the help info
        if(bInvM ? PageBegin==0 : LastEntryVisible){DBGLN;
          Return = NOTHING_SELECTED;
          break;
        }
    } else {DBGLN;
      BackGround.FastBlit(Buffer);

      int iDir = 1;
      if(bInvM)
        iDir *= -1;
      if(Pressed == KEY_PAGE_UP) //TODO confirm that this inverts the INVERSE_MODE behavior
        iDir *= -1;

      int iPB = PageBegin + iDir*PageLength;DBG1(iPB);
      if(iPB<0) //BEWARE!!! PageBegin is uint ...
        iPB=0;

      /**
       * overriders
       * obs.: pgdn and space are default "advance page" action
       */

      if(bInvM ? Pressed == KEY_END : Pressed == KEY_HOME) // go to first
        iPB=0;

      bool bSelLast=false;
      if(bInvM ? Pressed == KEY_HOME : Pressed == KEY_END){DBGLN; // go to last
        if(Entry.size()<=PageLength){DBGLN; //only one page
          Selected = Selectables-1;
          continue; //do nothing
        }

        if(LastEntryVisible){DBGLN;
          Selected = Selectables-1;
          continue; //do nothing
        }

        DBG6("Before",iPB,Selectables,Selected,PageLength,Entry.size());
        iPB = ScrollToLastPage(JustRedrawEverythingOnce, BackGround, Buffer); DBG6("After",iPB,Selectables,Selected,PageLength,Entry.size());
        bSelLast=true;
      }

      // fail safe LAST check
      if(iPB >= Selectables){ DBG6("HowItHappened?",iPB,Selectables,Selected,PageLength,Entry.size());
        continue; //do nothing
      }

      // apply
      PageBegin = iPB; DBG3(PageBegin,Pressed,iDir);

      if(!bSelLast)
        if(Flags & SELECTABLE)
          Selected = PageBegin;
    }
  };DBGLN;

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

  globalwindowhandler::ResetKeyTimeout();

  if(bApplyNewFilter){
    SetFilter(fsFilterApplyNew);
    return NOTHING_SELECTED;
  }

  return Return;
}

static festring Str;

bool felist::IsEntryDrawingAtValidPos(bitmap* Buffer,v2 pos){
  if(v2DefaultEntryImageSize.X==0 || v2DefaultEntryImageSize.Y==0)ABORT("v2DefaultEntryImageSize not set.");
  return Buffer->IsValidPos(pos.X+v2DefaultEntryImageSize.X, pos.Y+v2DefaultEntryImageSize.Y);
}

truth felist::DrawPage(bitmap* Buffer, v2* pv2FinalPageSize, std::vector<EntryRect>* pvEntryRect) const
{ DBGSV2(Pos);
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
      bool bShowHelp = false;
      if((!(Flags & INVERSE_MODE) && c != Entry.size() - 1)
         || ((Flags & INVERSE_MODE) && PageBegin))
      {
        bShowHelp=true;
      }
      static bool bShowAlways=true; //TODO user option?
      if(bShowAlways)bShowHelp=true;
      if(bShowHelp){
        Buffer->Fill(iTLX, LastFillBottom, iWidth, iHeight=30, BackColor);
        FONT->Printf(Buffer, v2(Pos.X + 13, LastFillBottom + 10), WHITE,
                     "- Press F1 to show help info -");
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

void felist::SetLastEntryHelp(cfestring Help)
{
  Entry[Entry.size()-1]->Help=Help;
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
