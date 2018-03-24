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
truthoption ivanconfig::WarnAboutDanger(  "WarnAboutVeryDangerousMonsters",
                                          "Warn about very dangerous monsters",
                                          true);
truthoption ivanconfig::AutoDropLeftOvers("AutoDropLeftOvers",
                                          "drop food leftovers automatically",
                                          true);
truthoption ivanconfig::LookZoom(         "LookZoom",
                                          "zoom feature in look mode",
                                          false);
truthoption ivanconfig::XBRZScale(        "XBRZScale",
                                          "use XBRZScale to stretch graphics",
                                          false);
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
scrollbaroption ivanconfig::Volume(     "Volume",
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
                                          "select graphics scale factor",
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
                                          "Outlined graphics (applied on next run)",
                                          false);

v2 ivanconfig::GetQuestionPos() { return game::IsRunning() ? v2(16, 6) : v2(30, 30); }
void ivanconfig::BackGroundDrawer() { game::DrawEverythingNoBlit(); }

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

truth ivanconfig::DefaultNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::DefaultPetNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name for the starting pet (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::AutoSaveIntervalChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new autosave interval (1-50000 turns, 0 for never):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::ContrastChangeInterface(numberoption* O)
{
  iosystem::ScrollBarQuestion(CONST_S("Set new contrast value (0-200, '<' and '>' move the slider):"),
                              GetQuestionPos(), O->Value, 5, 0, 200, O->Value, WHITE, LIGHT_GRAY, DARK_GRAY,
                              game::GetMoveCommandKey(KEY_LEFT_INDEX), game::GetMoveCommandKey(KEY_RIGHT_INDEX),
                              !game::IsRunning(), static_cast<scrollbaroption*>(O)->BarHandler);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::VolumeChangeInterface(numberoption* O)
{
  iosystem::ScrollBarQuestion(CONST_S("Set new volume value (0-127, '<' and '>' move the slider):"),
                              GetQuestionPos(), O->Value, 5, 0, 127, O->Value, WHITE, LIGHT_GRAY, DARK_GRAY,
                              game::GetMoveCommandKey(KEY_LEFT_INDEX), game::GetMoveCommandKey(KEY_RIGHT_INDEX),
                              !game::IsRunning(), static_cast<scrollbaroption*>(O)->BarHandler);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
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

truth ivanconfig::GraphicsScaleChangeInterface(cycleoption* O)
{
  O->ChangeValue(O->Value % O->CycleCount + 1);
  return true;
}

void ivanconfig::GraphicsScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
  graphics::SetScale(What);
}

void ivanconfig::FullScreenModeChanger(truthoption*, truth)
{
  graphics::SwitchMode();
}

#endif

void ivanconfig::Show()
{
  configsystem::Show(&BackGroundDrawer, &game::SetStandardListAttributes, game::IsRunning());
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

void ivanconfig::Initialize()
{
  configsystem::AddOption(&DefaultName);
  configsystem::AddOption(&DefaultPetName);
  configsystem::AddOption(&AutoSaveInterval);
  configsystem::AddOption(&Contrast);
  configsystem::AddOption(&WarnAboutDanger);
  configsystem::AddOption(&AutoDropLeftOvers);
  configsystem::AddOption(&LookZoom);
  configsystem::AddOption(&XBRZScale);
  configsystem::AddOption(&DirectionKeyMap);
  configsystem::AddOption(&SmartOpenCloseApply);
  configsystem::AddOption(&BeNice);
  configsystem::AddOption(&ShowTurn);
  configsystem::AddOption(&OutlinedGfx);
  configsystem::AddOption(&PlaySounds);
  configsystem::AddOption(&Volume);

  std::vector<std::string> DeviceNames;
  int NumDevices = audio::GetMIDIOutputDevices(DeviceNames);
  MIDIOutputDevice.Value = 0;
  if( NumDevices )
  {
     MIDIOutputDevice.Value = 1;
  }
  MIDIOutputDevice.CycleCount = NumDevices+1;

  configsystem::AddOption(&MIDIOutputDevice);
#ifndef __DJGPP__
  configsystem::AddOption(&GraphicsScale);
  configsystem::AddOption(&FullScreenMode);
#endif
#if defined(WIN32) || defined(__DJGPP__)
  configsystem::SetConfigFileName(game::GetHomeDir() + "ivan.cfg");
#else
  configsystem::SetConfigFileName(game::GetHomeDir() + "ivan.conf");
#endif
  configsystem::Load();
  CalculateContrastLuminance();
  audio::ChangeMIDIOutputDevice(MIDIOutputDevice.Value);
  audio::SetVolumeLevel(Volume.Value);

}
