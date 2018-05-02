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

#include "iconf.h"
#include "game.h"
#include "feio.h"
#include "area.h"
#include "graphics.h"
#include "bitmap.h"
#include "igraph.h"
#include "audio.h"
#include "whandler.h"
#include "stack.h"

stringoption ivanconfig::DefaultName(     "DefaultName",
                                          "player's default name",
                                          "",
                                          &configsystem::NormalStringDisplayer,
                                          &DefaultNameChangeInterface);
stringoption ivanconfig::DefaultPetName(  "DefaultPetName",
                                          "starting pet's default name",
                                          CONST_S("Kenny"),
                                          &configsystem::NormalStringDisplayer,
                                          &DefaultPetNameChangeInterface);
numberoption ivanconfig::AutoSaveInterval("AutoSaveInterval",
                                          "autosave interval",
                                          100,
                                          &AutoSaveIntervalDisplayer,
                                          &AutoSaveIntervalChangeInterface,
                                          &AutoSaveIntervalChanger);
scrollbaroption ivanconfig::Contrast(     "Contrast",
                                          "contrast",
                                          100,
                                          &ContrastDisplayer,
                                          &ContrastChangeInterface,
                                          &ContrastChanger,
                                          &ContrastHandler);
cycleoption ivanconfig::HitIndicator(     "HitIndicator",
                                          "Show Hit",
                                          0, 5,
                                          &HitIndicatorDisplayer);
cycleoption ivanconfig::ShowItemsAtPlayerSquare("ShowItemsAtPlayerSquare",
                                          "Show items at player square",
                                          0, 12,
                                          &ShowItemsAtPlayerSquareDisplayer,
                                          &configsystem::NormalCycleChangeInterface,
                                          &ShowItemsAtPlayerSquareChanger);
cycleoption ivanconfig::RotateTimesPerSquare("RotateTimesPerSquare",
                                          "Thrown weapons rotate times per square",
                                          0, 6,
                                          &RotateTimesPerSquareDisplayer);
numberoption ivanconfig::WindowWidth(     "WindowWidth",
                                          "* window width in pixels, min 800",
                                          800,
                                          &WindowWidthDisplayer,
                                          &WindowWidthChangeInterface,
                                          &WindowWidthChanger);
numberoption ivanconfig::WindowHeight(    "WindowHeight",
                                          "* window height in pixels, min 600",
                                          600,
                                          &WindowHeightDisplayer,
                                          &WindowHeightChangeInterface,
                                          &WindowHeightChanger);
numberoption ivanconfig::StackListPageLength("StackListPageLength",
                                          "Page length in entries for non-selectable lists",
                                          stack::GetDefaultPageLength(),
                                          &StackListPageLengthDisplayer,
                                          &StackListPageLengthChangeInterface,
                                          &StackListPageLengthChanger);
numberoption ivanconfig::FrameSkip(       "FrameSkip",
                                          "FrameSkip to inc. input responsiveness",
                                          0,
                                          &FrameSkipDisplayer,
                                          &FrameSkipChangeInterface,
                                          &FrameSkipChanger);
truthoption ivanconfig::ShowFullDungeonName("ShowFullDungeonName",
                                          "Show current dungeon's full name",
                                          false);
truthoption ivanconfig::ShowGodInfo(      "ShowGodInfo",
                                          "Shows info about each God when praying",
                                          false);
truthoption ivanconfig::CenterOnPlayerAfterLook("CenterOnPlayerAfterLook",
                                          "Always center camera on player after look mode exits",
                                          false);
truthoption ivanconfig::WarnAboutDanger(  "WarnAboutVeryDangerousMonsters",
                                          "Warn about very dangerous monsters",
                                          true);
truthoption ivanconfig::AutoDropLeftOvers("AutoDropLeftOvers",
                                          "drop food leftovers automatically",
                                          true);
truthoption ivanconfig::LookZoom(         "LookZoom",
                                          "zoom feature in look mode",
                                          false);
truthoption ivanconfig::AltAdentureInfo(  "AltAdentureInfo",
                                          "on death, enhanced messages review mode",
                                          false);
truthoption ivanconfig::XBRZScale(        "XBRZScale",
                                          "use xBRZScale to stretch graphics",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &XBRZScaleChanger);
numberoption ivanconfig::XBRZSquaresAroundPlayer("XBRZSquaresAroundPlayer",
                                          "Stretch squares around player with xBRZ (performance)",
                                          3,
                                          &XBRZSquaresAroundPlayerDisplayer,
                                          &XBRZSquaresAroundPlayerChangeInterface,
                                          &XBRZSquaresAroundPlayerChanger);
cycleoption ivanconfig::DungeonGfxScale(  "DungeonGfxScale",
                                          "* Select dungeon scale factor",
                                          1, 6, //from 1 to 6 (max xbrz) where 1 is no scale
                                          &DungeonGfxScaleDisplayer,
                                          &DungeonGfxScaleChangeInterface,
                                          &DungeonGfxScaleChanger);
cycleoption ivanconfig::SilhouetteScale(  "SilhouetteScale",
                                          "Silhouette scale factor (1 to disable)",
                                          1, 6, //from 1 to 6 (max xbrz) where 1 is no scale
                                          &SilhouetteScaleDisplayer,
                                          &SilhouetteScaleChangeInterface,
                                          &SilhouetteScaleChanger);
cycleoption ivanconfig::AltSilhouette(    "AltSilhouette",
                                          "Use alternative scaled silhouette.",
                                          0, 7,
                                          &AltSilhouetteDisplayer);
cycleoption ivanconfig::DirectionKeyMap(  "DirectionKeyMap",
                                          "Movement control scheme",
                                          DIR_NORM, 3, // {default value, number of options to cycle through}
                                          &DirectionKeyMapDisplayer);
truthoption ivanconfig::SmartOpenCloseApply("SmartOpenCloseApply",
                                          "smart open/close/apply behavior",
                                          true);
truthoption ivanconfig::BeNice(           "BeNice",
                                          "be nice to pets",
                                          true);
cycleoption ivanconfig::AltListItemPos(   "AltListItemPos",
                                          "Stretched list item alternative position",
                                          0, 3,
                                          &AltListItemPosDisplayer);
numberoption ivanconfig::AltListItemWidth("AltListItemWidth",
                                          "List width for 'item alternative position'",
                                          game::getDefaultItemsListWidth(),
                                          &AltListItemWidthDisplayer,
                                          &AltListItemWidthChangeInterface,
                                          &AltListItemWidthChanger);
scrollbaroption ivanconfig::Volume(       "Volume",
                                          "volume",
                                          127,
                                          &VolumeDisplayer,
                                          &VolumeChangeInterface,
                                          &VolumeChanger,
                                          &VolumeHandler);
cycleoption ivanconfig::MIDIOutputDevice(  "MIDIOutputDevice",
                                          "select MIDI output device",
                                          0, 0, // {default value, number of options to cycle through}
                                          &MIDIOutputDeviceDisplayer);
#ifndef __DJGPP__
cycleoption ivanconfig::GraphicsScale(    "GraphicsScale",
                                          "select window scale factor",
                                          1, 2,
                                          &GraphicsScaleDisplayer,
                                          &GraphicsScaleChangeInterface,
                                          &GraphicsScaleChanger);
truthoption ivanconfig::FullScreenMode(   "FullScreenMode",
                                          "run the game in full screen mode",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &FullScreenModeChanger);
#endif
col24 ivanconfig::ContrastLuminance = NORMAL_LUMINANCE;
truthoption ivanconfig::PlaySounds(       "PlaySounds",
                                          "use sounds",
                                          true);
truthoption ivanconfig::ShowTurn(         "ShowTurn",
                                          "show the turn on log messages",
                                          false);
truthoption ivanconfig::OutlinedGfx(      "OutlinedGfx",
                                          "* Outlined graphics",
                                          false);

v2 ivanconfig::GetQuestionPos() { return game::IsRunning() ? v2(16, 6) : v2(30, 30); }
void ivanconfig::BackGroundDrawer() { game::DrawEverythingNoBlit(); }

void ivanconfig::XBRZSquaresAroundPlayerDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " squares";
}

void ivanconfig::AltListItemWidthDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " pixels";
}

void ivanconfig::FrameSkipDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value;
  if(O->Value==-2)Entry  << " = wait"  ;
  if(O->Value==-1)Entry  << " = auto"  ;
  if(O->Value>= 0)Entry  <<   " frames";
}

void ivanconfig::StackListPageLengthDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " entries";
}

void ivanconfig::WindowHeightDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " pixels";
}

void ivanconfig::RotateTimesPerSquareDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "disabled";break;
  case 1: Entry << "x1";break;
  case 2: Entry << "x2";break;
  case 3: Entry << "x3";break;
  case 4: Entry << "x4";break;
  case 5: Entry << "dynamic";break;
  }
}

void ivanconfig::HitIndicatorDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "disabled";break;
  case 1: Entry << "immersive";break;
  case 2: Entry << "indicator";break;
  case 3: Entry << "ind+color";break;
  case 4: Entry << "dynamic";break;
  }
}

void ivanconfig::ShowItemsAtPlayerSquareDisplayer(const cycleoption* O, festring& Entry)
{
  if(O->Value>=10){
    Entry << "dynamic";
    if(O->Value==11)Entry << "+items";
    return;
  }

  int iCode = game::IntemUnderCode(O->Value);

  if(iCode==0){
    Entry << "disabled";
  }else
  if(iCode==1){
    Entry << "above head";
  }else{
    switch(game::ItemUnderCorner(iCode)){
      case 0:Entry << "UL";break;
      case 1:Entry << "UR";break;
      case 2:Entry << "LL";break;
      case 3:Entry << "LR";break;
    }

//    Entry << ",";
//    Entry << "x" << game::ItemUnderZoom(O->Value);

    Entry << ",";
    Entry << (game::ItemUnderHV(iCode) ? "H" : "V");
  }
}

void ivanconfig::WindowWidthDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " pixels";
}

void ivanconfig::AutoSaveIntervalDisplayer(const numberoption* O, festring& Entry)
{
  if(O->Value)
  {
    Entry << O->Value << " turn";

    if(O->Value != 1)
      Entry << 's';
  }
  else
    Entry << "disabled";
}

void ivanconfig::ContrastDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << "/100";
}

void ivanconfig::VolumeDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << "/127";
}

void ivanconfig::AltSilhouetteDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
    case 0: Entry << "no"        ; break;
    case 1: Entry << "short"     ; break;
    case 2: Entry << "tall"      ; break;
    case 3: Entry << "breathing" ; break;
    case 4: Entry << "breathing2"; break;
    case 5: Entry << "breathing3"; break;
    case 6: Entry << "breathing4"; break;
  }
}

void ivanconfig::DirectionKeyMapDisplayer(const cycleoption* O, festring& Entry)
{
        switch(O->Value)
        {
          case DIR_NORM:
                Entry << CONST_S("Normal");
                break;
          case DIR_ALT:
                Entry << CONST_S("Alternative");
                break;
          case DIR_HACK:
                Entry << CONST_S("NetHack");
                break;
        }
}

void ivanconfig::MIDIOutputDeviceDisplayer(const cycleoption* O, festring& Entry)
{
  std::vector<std::string> devicenames;
  int NumDevices = audio::GetMIDIOutputDevices(devicenames);
  MIDIOutputDevice.CycleCount = NumDevices+1;

  if(O->Value && O->Value <= devicenames.size())
  {
     const char *cstr = devicenames[O->Value - 1].c_str();
     Entry << cstr;

     audio::ChangeMIDIOutputDevice(O->Value);
     VolumeChanger(&Volume, GetVolume());
  }
  else
  {
     audio::ChangeMIDIOutputDevice(0);
     Entry << CONST_S("No output device");
  }
}

void clearToBackgroundAfterChangeInterface(){
  if(game::IsRunning())igraph::BlitBackGround(v2(16, 6), v2(game::GetMaxScreenXSize() << 4, 23));
}

truth ivanconfig::GraphicsScaleChangeInterface(cycleoption* O)
{
  O->ChangeValue(O->Value % O->CycleCount + 1);
  clearToBackgroundAfterChangeInterface();
  return true;
}

truth ivanconfig::SilhouetteScaleChangeInterface(cycleoption* O)
{
  O->ChangeValue(O->Value % O->CycleCount + 1);
  clearToBackgroundAfterChangeInterface();
  return true;
}

truth ivanconfig::DungeonGfxScaleChangeInterface(cycleoption* O)
{
  O->ChangeValue(O->Value % O->CycleCount + 1);
  clearToBackgroundAfterChangeInterface();
  return true;
}

truth ivanconfig::DefaultNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::DefaultPetNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name for the starting pet (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::XBRZSquaresAroundPlayerChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set how many squares around player to xBRZ (0=FullDungeon):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::AltListItemWidthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new item's list width to fit well with other elements:"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::FrameSkipChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set frame skip to let controls/input work better (-2=wait,-1=auto,0to100):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::StackListPageLengthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new stack list page's length in entries:"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::WindowHeightChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new window height (from 600 to your monitor screen max width):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::WindowWidthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new window width (from 800 to your monitor screen max width):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::AutoSaveIntervalChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new autosave interval (1-50000 turns, 0 for never):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::ContrastChangeInterface(numberoption* O)
{
  iosystem::ScrollBarQuestion(CONST_S("Set new contrast value (0-200, '<' and '>' move the slider):"),
                              GetQuestionPos(), O->Value, 5, 0, 200, O->Value, WHITE, LIGHT_GRAY, DARK_GRAY,
                              game::GetMoveCommandKey(KEY_LEFT_INDEX), game::GetMoveCommandKey(KEY_RIGHT_INDEX),
                              !game::IsRunning(), static_cast<scrollbaroption*>(O)->BarHandler);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::VolumeChangeInterface(numberoption* O)
{
  iosystem::ScrollBarQuestion(CONST_S("Set new volume value (0-127, '<' and '>' move the slider):"),
                              GetQuestionPos(), O->Value, 5, 0, 127, O->Value, WHITE, LIGHT_GRAY, DARK_GRAY,
                              game::GetMoveCommandKey(KEY_LEFT_INDEX), game::GetMoveCommandKey(KEY_RIGHT_INDEX),
                              !game::IsRunning(), static_cast<scrollbaroption*>(O)->BarHandler);

  clearToBackgroundAfterChangeInterface();

  return false;
}

void ivanconfig::XBRZSquaresAroundPlayerChanger(numberoption* O, long What)
{
  if(What < 0) What = 0;
  O->Value = What;
}

void ivanconfig::AltListItemWidthChanger(numberoption* O, long What)
{
  if(What < 400) What = 400; //TODO find the best quality minimum value
  O->Value = What;
}

void ivanconfig::FrameSkipChanger(numberoption* O, long What)
{
  if(What <  -2) What =  -2;
  if(What > 100) What = 100;

  if(O!=NULL)O->Value = What;

  globalwindowhandler::SetAddFrameSkip(What);
}

void ivanconfig::StackListPageLengthChanger(numberoption* O, long What)
{
  if(What < stack::GetDefaultPageLength()) What = stack::GetDefaultPageLength();
  if(O!=NULL)O->Value = What;

  stack::SetStandardPageLength(What);
}

void ivanconfig::WindowHeightChanger(numberoption* O, long What)
{
  if(What < 600) What = 600;
  O->Value = What;
}

void ivanconfig::ShowItemsAtPlayerSquareChanger(cycleoption* O, long What)
{
//  if(What==1 && GetStartingDungeonGfxScale()<3){ //above head asked
//    What=2; //upgrade to 1st corner
//  }

  O->Value = What;
}

void ivanconfig::WindowWidthChanger(numberoption* O, long What)
{
  if(What < 800) What = 800;
  O->Value = What;
}

void ivanconfig::AutoSaveIntervalChanger(numberoption* O, long What)
{
  if(What < 0) What = 0;
  if(What > 50000) What = 50000;
  O->Value = What;
}

void ivanconfig::ContrastChanger(numberoption* O, long What)
{
  if(What < 0) What = 0;
  if(What > 200) What = 200;
  O->Value = What;

  CalculateContrastLuminance();
}

void ivanconfig::VolumeChanger(numberoption* O, long What)
{
  if(What < 0) What = 0;
  if(What > 127) What = 127;
  O->Value = What;

  audio::SetVolumeLevel(What);
}

#ifndef __DJGPP__

void ivanconfig::GraphicsScaleDisplayer(const cycleoption* O, festring& Entry)
{
  Entry << O->Value << 'x';
}

void ivanconfig::GraphicsScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
  graphics::SetScale(What);
}

void ivanconfig::SilhouetteScaleDisplayer(const cycleoption* O, festring& Entry)
{
  Entry << O->Value << 'x';
}

void ivanconfig::AltListItemPosDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0:Entry << "disabled";break; //do not show
  case 1:Entry << "no";break; //default pos
  case 2:Entry << "yes";break; //alt pos
  }
}

void ivanconfig::DungeonGfxScaleDisplayer(const cycleoption* O, festring& Entry)
{
  Entry << O->Value << 'x';
}

void ivanconfig::SilhouetteScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
}

void ivanconfig::DungeonGfxScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
}

void ivanconfig::XBRZScaleChanger(truthoption* O, truth What)
{
  O->Value = What;

  game::UpdateSRegionsXBRZ();
}

void ivanconfig::FullScreenModeChanger(truthoption*, truth)
{
  graphics::SwitchMode();
}

#endif

void ivanconfig::Show()
{
  game::SRegionAroundDeny();
  configsystem::Show(&BackGroundDrawer, &game::SetStandardListAttributes, game::IsRunning());
  game::SRegionAroundAllow();
}

void ivanconfig::ContrastHandler(long Value)
{
  ContrastChanger(&Contrast, Value);

  if(game::IsRunning())
  {
    game::GetCurrentArea()->SendNewDrawRequest();
    game::DrawEverythingNoBlit();
  }
}

void ivanconfig::VolumeHandler(long Value)
{
  VolumeChanger(&Volume, Value);

  if(game::IsRunning())
  {
    game::GetCurrentArea()->SendNewDrawRequest();
    game::DrawEverythingNoBlit();
  }
}

#ifndef __DJGPP__

void ivanconfig::SwitchModeHandler()
{
  FullScreenMode.Value = !FullScreenMode.Value;
  Save();
}

#endif

void ivanconfig::CalculateContrastLuminance()
{
  int Element = Min<long>((GetContrast() << 7) / 100, 255);
  ContrastLuminance = MakeRGB24(Element, Element, Element);
}

// TODO keep initializing with invalid values (where possible) so if they are used before cfg file loading it will show errors clearly?
int  ivanconfig::iStartingWindowWidth=-1;
int  ivanconfig::iStartingWindowHeight=-1;
int  ivanconfig::iStartingDungeonGfxScale=-1;
bool ivanconfig::bStartingOutlinedGfx=false;
void ivanconfig::Initialize()
{
  festring fsCategory;

  fsCategory="Core Game Setup";
  configsystem::AddOption(fsCategory,&DefaultName);
  configsystem::AddOption(fsCategory,&DefaultPetName);
  configsystem::AddOption(fsCategory,&AutoSaveInterval);
  configsystem::AddOption(fsCategory,&AltAdentureInfo);

  fsCategory="Gameplay Changes";
  configsystem::AddOption(fsCategory,&BeNice);
  configsystem::AddOption(fsCategory,&WarnAboutDanger);
  configsystem::AddOption(fsCategory,&AutoDropLeftOvers);
  configsystem::AddOption(fsCategory,&SmartOpenCloseApply);
  configsystem::AddOption(fsCategory,&CenterOnPlayerAfterLook);
  configsystem::AddOption(fsCategory,&ShowGodInfo); //gameplay change in a sense that, to remember what each god is about may be a challenge on itself :)

  fsCategory="Window";
  configsystem::AddOption(fsCategory,&Contrast);
  configsystem::AddOption(fsCategory,&WindowWidth);
  configsystem::AddOption(fsCategory,&WindowHeight);
#ifndef __DJGPP__
  configsystem::AddOption(fsCategory,&GraphicsScale);
  configsystem::AddOption(fsCategory,&FullScreenMode);
#endif

  fsCategory="Graphics";
  configsystem::AddOption(fsCategory,&LookZoom);
  configsystem::AddOption(fsCategory,&XBRZScale);
  configsystem::AddOption(fsCategory,&XBRZSquaresAroundPlayer);
  configsystem::AddOption(fsCategory,&SilhouetteScale);
  configsystem::AddOption(fsCategory,&AltSilhouette);
  configsystem::AddOption(fsCategory,&AltListItemPos);
  configsystem::AddOption(fsCategory,&AltListItemWidth);
  configsystem::AddOption(fsCategory,&StackListPageLength);
  configsystem::AddOption(fsCategory,&DungeonGfxScale);
  configsystem::AddOption(fsCategory,&OutlinedGfx);
  configsystem::AddOption(fsCategory,&FrameSkip);
  configsystem::AddOption(fsCategory,&ShowItemsAtPlayerSquare);
  configsystem::AddOption(fsCategory,&RotateTimesPerSquare);
  configsystem::AddOption(fsCategory,&HitIndicator);

  fsCategory="Sounds";
  configsystem::AddOption(fsCategory,&PlaySounds);
  configsystem::AddOption(fsCategory,&Volume);

  std::vector<std::string> DeviceNames;
  int NumDevices = audio::GetMIDIOutputDevices(DeviceNames);
  MIDIOutputDevice.Value = 0;
  if( NumDevices )
  {
     MIDIOutputDevice.Value = 1;
  }
  MIDIOutputDevice.CycleCount = NumDevices+1;

  configsystem::AddOption(fsCategory,&MIDIOutputDevice);

  fsCategory="System and user interface/input";
  configsystem::AddOption(fsCategory,&DirectionKeyMap);
  configsystem::AddOption(fsCategory,&ShowTurn);
  configsystem::AddOption(fsCategory,&ShowFullDungeonName);

  /********************************
   * LOAD AND APPLY some SETTINGS *
   ********************************/
#if defined(WIN32) || defined(__DJGPP__)
  configsystem::SetConfigFileName(game::GetHomeDir() + "ivan.cfg");
#else
  configsystem::SetConfigFileName(game::GetHomeDir() + "ivan.conf");
#endif

  configsystem::Load();

  iStartingWindowWidth=WindowWidth.Value;
  iStartingWindowHeight=WindowHeight.Value;
  iStartingDungeonGfxScale=DungeonGfxScale.Value;
  bStartingOutlinedGfx=OutlinedGfx.Value;

  CalculateContrastLuminance();
  audio::ChangeMIDIOutputDevice(MIDIOutputDevice.Value);
  audio::SetVolumeLevel(Volume.Value);

  FrameSkipChanger(NULL,FrameSkip.Value); //TODO re-use changer methods for above configs too?
  StackListPageLengthChanger(NULL, StackListPageLength.Value);
}
