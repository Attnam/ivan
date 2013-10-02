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

stringoption ivanconfig::DefaultName(	  "DefaultName",
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
scrollbaroption ivanconfig::Contrast(	  "Contrast",
					  "contrast",
					  100,
					  &ContrastDisplayer,
					  &ContrastChangeInterface,
					  &ContrastChanger,
					  &ContrastHandler);
truthoption ivanconfig::WarnAboutDanger(	  "WarnAboutVeryDangerousMonsters",
						  "Warn about very dangerous monsters",
						  true);
truthoption ivanconfig::AutoDropLeftOvers( "AutoDropLeftOvers",
					   "drop food leftovers automatically",
					   true);
truthoption ivanconfig::LookZoom(	  "LookZoom",
					  "zoom feature in look mode",
					  false);
truthoption ivanconfig::UseAlternativeKeys("UseAlternativeKeys",
					   "use alternative direction keys",
					   false);
truthoption ivanconfig::BeNice(		  "BeNice",
					  "be nice to pets",
					  true);
#ifndef __DJGPP__
truthoption ivanconfig::FullScreenMode(	  "FullScreenMode",
					  "run the game in full screen mode",
					  false,
					  &configsystem::NormalTruthDisplayer,
					  &configsystem::NormalTruthChangeInterface,
					  &FullScreenModeChanger);
#endif
col24 ivanconfig::ContrastLuminance = NORMAL_LUMINANCE;

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

truth ivanconfig::DefaultNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name (1-20 letters):"), GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::DefaultPetNameChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name for the starting pet (1-20 letters):"), GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::AutoSaveIntervalChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new autosave interval (1-50000 turns, 0 for never):"), GetQuestionPos(), WHITE, !game::IsRunning()));

  if(game::IsRunning())
    igraph::BlitBackGround(v2(16, 6), v2(game::GetScreenXSize() << 4, 23));

  return false;
}

truth ivanconfig::ContrastChangeInterface(numberoption* O)
{
  iosystem::ScrollBarQuestion(CONST_S("Set new contrast value (0-200, '<' and '>' move the slider):"), GetQuestionPos(), O->Value, 5, 0, 200, O->Value, WHITE, LIGHT_GRAY, DARK_GRAY, game::GetMoveCommandKey(KEY_LEFT_INDEX), game::GetMoveCommandKey(KEY_RIGHT_INDEX), !game::IsRunning(), static_cast<scrollbaroption*>(O)->BarHandler);

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

#ifndef __DJGPP__

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
  configsystem::AddOption(&UseAlternativeKeys);
  configsystem::AddOption(&BeNice);
#ifndef __DJGPP__
  configsystem::AddOption(&FullScreenMode);
#endif
#if defined(WIN32) || defined(__DJGPP__)
  configsystem::SetConfigFileName("ivan.cfg");
#else
  configsystem::SetConfigFileName(festring(getenv("HOME")) + "/.ivan.conf");
#endif
  configsystem::Load();
  CalculateContrastLuminance();
}
