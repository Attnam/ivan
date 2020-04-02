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

#include "area.h"
#include "audio.h"
#include "bitmap.h"
#include "command.h"
#include "feio.h"
#include "felist.h"
#include "game.h"
#include "graphics.h"
#include "iconf.h"
#include "igraph.h"
#include "message.h"
#include "save.h"
#include "stack.h"
#include "whandler.h"
#include "bugworkaround.h"

stringoption ivanconfig::DefaultName(     "DefaultName",
                                          "Player's default name",
                                          "Pick a character name for all your future games. When left empty, the game will generate a random name if a random name generator pattern is configured, or otherwise ask you to pick a name whenever you start a new game.",
                                          "",
                                          &configsystem::NormalStringDisplayer,
                                          &DefaultNameChangeInterface);
stringoption ivanconfig::FantasyNamePattern("FantasyNamePattern",
                                          "Random name generator pattern",
                                          "Set a pattern for the random fantasy name generator. See MANUAL for further information and pattern examples.", // TODO: this above
                                          "!ss !sV",
                                          &configsystem::NormalStringDisplayer,
                                          &FantasyNameChangeInterface);
stringoption ivanconfig::DefaultPetName(  "DefaultPetName",
                                          "Starting pet's default name",
                                          "Choose a name for your puppy.",
                                          CONST_S("Kenny"),
                                          &configsystem::NormalStringDisplayer,
                                          &DefaultPetNameChangeInterface);
stringoption ivanconfig::SelectedBkgColor("SelectedBkgColor",
                                          "Highlight color of selected entry",
                                          "Choose the RGB code of a color used to highlight the selected entry in a menu.",
                                          "8,8,8",
                                          &configsystem::NormalStringDisplayer,
                                          &SelectedBkgColorChangeInterface,
                                          &SelectedBkgColorChanger);
stringoption ivanconfig::AutoPickUpMatching("AutoPickUpMatching",
                                          "Auto pick up regex",
                                          "Automatically pick up items according to a regular expression. To disable something, you can invalidate it with '_' without removing it from the expression (eg. '_dagger'). To disable everything at once, begin this config option with '!'. Due to current constraints on length of options, editing is easier to do externally for now.",  //TODO if multiline text editing is implemented, remove the last help statement.
                                          "!((book|can|dagger|grenade|horn of|kiwi|key|ring|scroll|wand|whistle)|^(?:(?!(broken|empty)).)*(bottle|vial)|sol stone)",
                                          &configsystem::NormalStringDisplayer,
                                          &AutoPickUpMatchingChangeInterface,
                                          &AutoPickUpMatchingChanger);
numberoption ivanconfig::AutoSaveInterval("AutoSaveInterval",
                                          "Autosave interval",
                                          "Automatically backs up your game in case of a crash.",
                                          100,
                                          &AutoSaveIntervalDisplayer,
                                          &AutoSaveIntervalChangeInterface,
                                          &AutoSaveIntervalChanger);
scrollbaroption ivanconfig::Contrast(     "Contrast",
                                          "Contrast",
                                          "",
                                          100,
                                          &ContrastDisplayer,
                                          &ContrastChangeInterface,
                                          &ContrastChanger,
                                          &ContrastHandler);
cycleoption ivanconfig::HitIndicator(     "HitIndicator",
                                          "Show successful hit",
                                          "",
                                          0, 5,
                                          &HitIndicatorDisplayer);
cycleoption ivanconfig::HoldPosMaxDist(   "HoldPosMaxDist",
                                          "Pets stay put when not following",
                                          "If pets are set not to follow the player, they will move around from their spot no further than the maximum specified distance. If a pet set to be following loses track of the player, it will stay near the last position where it was still able to follow the player.",
                                          0, 7,
                                          &HoldPosMaxDistDisplayer);
cycleoption ivanconfig::ShowItemsAtPlayerSquare("ShowItemsAtPlayerSquare",
                                          "Show items at player's square",
                                          "",
                                          0, 12,
                                          &ShowItemsAtPlayerSquareDisplayer,
                                          &configsystem::NormalCycleChangeInterface,
                                          &ShowItemsAtPlayerSquareChanger);
cycleoption ivanconfig::ShowMap(          "ShowMap",
                                          "Map preferences",
                                          "",
                                          0, 5,
                                          &ShowMapDisplayer);
cycleoption ivanconfig::RotateTimesPerSquare("RotateTimesPerSquare",
                                          "Thrown weapons rotate",
                                          "Select how many times per square should the animation rotate a thrown weapon.",
                                          0, 6,
                                          &RotateTimesPerSquareDisplayer);
numberoption ivanconfig::WindowWidth(     "WindowWidth",
                                          "* Window width in pixels",
                                          "Choose the width of the game window, with a minimum of 640 pixels. Requires restarting the game to take effect.",
                                          800, //default will be vanilla, but mininum still can be 640
                                          &WindowWidthDisplayer,
                                          &WindowWidthChangeInterface,
                                          &WindowWidthChanger);
numberoption ivanconfig::WindowHeight(    "WindowHeight",
                                          "* Window height in pixels",
                                          "Choose the height of the game window, with a minimum of 480 pixels. Requires restarting the game to take effect.",
                                          600, //default will be vanilla, but mininum still can be 480
                                          &WindowHeightDisplayer,
                                          &WindowHeightChangeInterface,
                                          &WindowHeightChanger);
numberoption ivanconfig::StackListPageLength("StackListPageLength",
                                          "Page length in entries for non-selectable menus",
                                          "Choose how many entries will be displayed per page for non-selectable menus, such as when viewing your inventory. Note that selecting too many entries for the height of your screen may result in the menu being drawn partially off-screen.",
                                          stack::GetDefaultPageLength(),
                                          &StackListPageLengthDisplayer,
                                          &StackListPageLengthChangeInterface,
                                          &StackListPageLengthChanger);
cycleoption ivanconfig::GoOnStopMode(     "GoOnStopMode",
                                          "Fastwalk stop mode",
                                          "",
                                          0, 4,
                                          &GoOnStopModeDisplayer);
numberoption ivanconfig::FrameSkip(       "FrameSkip",
                                          "Frame skip (assures responsiveness)",
                                          "",
                                          0,
                                          &FrameSkipDisplayer,
                                          &FrameSkipChangeInterface,
                                          &FrameSkipChanger);
truthoption ivanconfig::AllowMouseOnFelist("AllowMouseOnFelist",
                                          "Enable mouse cursor on lists",
                                          "",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &AllowMouseOnFelistChanger);
truthoption ivanconfig::ShowMapAtDetectMaterial("ShowMapAtDetectMaterial",
                                          "Show map while detecting material",
                                          "",
                                          false);
truthoption ivanconfig::AutoPickupThrownItems("AutoPickupThrownItems",
                                          "Auto pick up thrown weapons",
                                          "Automatically annotate any thrown weapon and pick it up without loosing a turn when you step on its square.",
                                          true);
truthoption ivanconfig::TransparentMapLM( "TransparentMapLM",
                                          "Show transparent map in look mode",
                                          "Show transparent map of the whole level when in look mode.",
                                          true);
truthoption ivanconfig::AllowImportOldSavegame("AllowImportOldSavegame",
                                          "Import old savegames (v131 up, experimental)",
                                          "",
                                          false);
truthoption ivanconfig::WaitNeutralsMoveAway("WaitNeutralsMoveAway",
                                          "Wait until neutral NPCs move from your path",
                                          "When you try to move in a direction that is blocked by a neutral NPC, skip turns until the path is clear. Will not skip turns if the NPC doesn't move from their square, or if there are hostiles nearby.",
                                          false);
truthoption ivanconfig::AllWeightIsRelevant("AllWeightIsRelevant",
                                          "Only pile items with equal weight on lists", //clutter are useful now for crafting so their weight matters...
                                          "",
                                          false);
truthoption ivanconfig::DropBeforeOffering("DropBeforeOffering",
                                          "Drop the item on altar before offering it.",
                                          "Beware it may be owned floor!",
                                          false);
truthoption ivanconfig::ShowVolume(       "ShowVolume",
                                          "Show item volume in cm3",
                                          "",
                                          false);
truthoption ivanconfig::EnhancedLights(   "EnhancedLights",
                                          "Allow distant lights to be seen",
                                          "",
                                          true);
truthoption ivanconfig::HideWeirdHitAnimationsThatLookLikeMiss("HideWeirdHitAnimationsThatLookLikeMiss",
                                          "Hide hit animations that look like miss",
                                          "",
                                          true);
truthoption ivanconfig::UseLightEmiterBasedOnVolume("UseLightEmiterBasedOnVolume",
                                          "Small crystal rocks etc. will emit less light.",
                                          "This experimental feature still has bugs that happen when splitting rocks etc. Most are fixed after restarting the game.",
                                          false);
truthoption ivanconfig::ShowFullDungeonName("ShowFullDungeonName",
                                          "Show full name of current dungeon",
                                          "",
                                          false);
truthoption ivanconfig::ShowGodInfo(      "ShowGodInfo",
                                          "Show extra info about gods when praying",
                                          "Remember the last response to a prayer for each god.",
                                          false);
truthoption ivanconfig::CenterOnPlayerAfterLook("CenterOnPlayerAfterLook",
                                          "Center camera on player after exiting look mode",
                                          "Always center the displayed region of the dungeon back on player after exiting look mode.",
                                          false);
truthoption ivanconfig::WarnAboutDanger(  "WarnAboutVeryDangerousMonsters",
                                          "Warn about very dangerous monsters",
                                          "Display a warning prompt when you encounter an unusually dangerous monster.",
                                          true);
truthoption ivanconfig::AutoDropLeftOvers("AutoDropLeftOvers",
                                          "Drop food leftovers automatically",
                                          "",
                                          true);
truthoption ivanconfig::LookZoom(         "LookZoom",
                                          "Zoom in look mode",
                                          "",
                                          false);
truthoption ivanconfig::AltAdentureInfo(  "AltAdentureInfo",
                                          "Enhanced message review mode after death",
                                          "",
                                          false);
truthoption ivanconfig::DescriptiveHP(    "DescriptiveHP",
                                          "Use health level descriptions",
                                          "Display description of your relative health rather than numeric value of your hit points.",
                                          false);
truthoption ivanconfig::StartWithNoPet(   "StartWithNoPet",
                                          "Start with no pet",
                                          "Do not start the game with a puppy.",
                                          false);
cycleoption ivanconfig::MemorizeEquipmentMode("MemorizeEquipmentMode",
                                          "NPCs restore equipped items after polymorph",
                                          "",
                                          2, 3,
                                          &MemorizeEquipmentModeDisplayer);
truthoption ivanconfig::XBRZScale(        "XBRZScale",
                                          "Use xBRZScale to stretch graphics",
                                          "",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &XBRZScaleChanger);
numberoption ivanconfig::XBRZSquaresAroundPlayer("XBRZSquaresAroundPlayer",
                                          "Stretch squares around player with xBRZ",
                                          "",
                                          3,
                                          &XBRZSquaresAroundPlayerDisplayer,
                                          &XBRZSquaresAroundPlayerChangeInterface,
                                          &XBRZSquaresAroundPlayerChanger);
cycleoption ivanconfig::DungeonGfxScale(  "DungeonGfxScale",
                                          "* Select graphics scaling factor",
                                          "",
                                          1, 6, //from 1 to 6 (max xbrz) where 1 is no scale
                                          &DungeonGfxScaleDisplayer,
                                          &DungeonGfxScaleChangeInterface,
                                          &DungeonGfxScaleChanger);
cycleoption ivanconfig::FontGfx(          "FontGfx",
                                          "* Select font",
                                          "Select your favorite from the available fonts.",
                                          1, 3, //from 1 to 3 (three options available)
                                          &FontGfxDisplayer,
                                          &FontGfxChangeInterface,
                                          &FontGfxChanger);
cycleoption ivanconfig::DistLimitMagicMushrooms("DistLimitMagicMushrooms",
                                          "Breeders' active range",
                                          "Select the maximum distance where breeding monsters will spawn more of their own. This option can be used to prevent lag from high number of creatures on slow computers, but may impact the intended game balance negatively.",
                                          0, 5,
                                          &DistLimitMagicMushroomsDisplayer);
cycleoption ivanconfig::SaveGameSortMode( "SaveGameSortMode",
                                          "Sort savegame files by dungeon IDs",
                                          "Savegame selection menu will be sorted according to the chosen criterion.",
                                          0, 4,
                                          &SaveGameSortModeDisplayer,
                                          &configsystem::NormalCycleChangeInterface,
                                          &SaveGameSortModeChanger);
cycleoption ivanconfig::SilhouetteScale(  "SilhouetteScale",
                                          "Silhouette scaling factor",
                                          "Select scaling factor for silhouette, 1x to disable (no scaling).",
                                          1, 6, //from 1 to 6 (max xbrz) where 1 is no scale
                                          &SilhouetteScaleDisplayer,
                                          &SilhouetteScaleChangeInterface,
                                          &SilhouetteScaleChanger);
cycleoption ivanconfig::AltSilhouette(    "AltSilhouette",
                                          "Alternative silhouette mode",
                                          "",
                                          0, 7,
                                          &AltSilhouetteDisplayer);
cycleoption ivanconfig::AltSilhouettePreventColorGlitch("AltSilhouettePreventColorGlitch",
                                          "Alternative silhouette background",
                                          "",
                                          2, 3,
                                          &AltSilhouettePreventColorGlitchDisplayer);
cycleoption ivanconfig::DirectionKeyMap(  "DirectionKeyMap",
                                          "Movement control scheme",
                                          "Select a pre-defined keybinding scheme for the movement of your character. Normal scheme uses NumPad, or arrow keys along with Home, End, PgUp and PgDn for diagonal directions. Alternative scheme is better suited for laptops and uses number and letter keys on the main keyboard. NetHack scheme uses vi keys. After you select a movement control scheme, you may also check the in game keybindings help to see the currently active movement keys. Any other command keys may be auto changed also to not conflict with this movement keys choice.",
                                          DIR_NORM, 3, // {default value, number of options to cycle through}
                                          &DirectionKeyMapDisplayer);
truthoption ivanconfig::SetupCustomKeys(  "SetupCustomKeys",
                                          "Movement control custom keys", //TODO all keys one day, and let it work on main menu
                                          "Let you assign all 8 movement keys to any available key of your preference. All other command keys will remain as assigned by the scheme above. This global configuration won't work at main menu, load/start some game.",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &SetupCustomKeysChanger);
truthoption ivanconfig::SmartOpenCloseApply("SmartOpenCloseApply",
                                          "Smart open/close/apply behavior",
                                          "Automatically try to open doors when you walk into them, and don't ask for the target of close/apply actions when only one viable target is present.",
                                          true);
truthoption ivanconfig::BeNice(           "BeNice",
                                          "Be nice to pets",
                                          "Don't let your sadistic tendencies hurt your pets.",
                                          true);
cycleoption ivanconfig::AltListItemPos(   "AltListItemPos",
                                          "Use alternative position of stretched lists",
                                          "",
                                          0, 3,
                                          &AltListItemPosDisplayer);
numberoption ivanconfig::AltListItemWidth("AltListItemWidth",
                                          "List width for alternative stretched lists",
                                          "",
                                          game::getDefaultItemsListWidth(),
                                          &AltListItemWidthDisplayer,
                                          &AltListItemWidthChangeInterface,
                                          &AltListItemWidthChanger);
scrollbaroption ivanconfig::Volume(       "Volume",
                                          "Volume",
                                          "Select volume for sound effects and game music.",
                                          127,
                                          &VolumeDisplayer,
                                          &VolumeChangeInterface,
                                          &VolumeChanger,
                                          &VolumeHandler);
cycleoption ivanconfig::MIDIOutputDevice( "MIDIOutputDevice",
                                          "Use MIDI soundtrack",
                                          "Select an output device for the game music, or disable soundtrack.",
                                          0, 0, // {default value, number of options to cycle through}
                                          &MIDIOutputDeviceDisplayer);
#ifndef __DJGPP__
cycleoption ivanconfig::GraphicsScale(    "GraphicsScale",
                                          "Select window scaling factor",
                                          "",
                                          1, 2,
                                          &GraphicsScaleDisplayer,
                                          &GraphicsScaleChangeInterface,
                                          &GraphicsScaleChanger);
truthoption ivanconfig::FullScreenMode(   "FullScreenMode",
                                          "Full screen mode",
                                          "Display the game in full screen mode.",
                                          false,
                                          &configsystem::NormalTruthDisplayer,
                                          &configsystem::NormalTruthChangeInterface,
                                          &FullScreenModeChanger);
cycleoption ivanconfig::ScalingQuality(   "ScalingQuality",
                                          "* Scaling quality",
                                          "",
                                          0, 2,
                                          &ScalingQualityDisplayer);
#endif
col24 ivanconfig::ContrastLuminance = NORMAL_LUMINANCE;
truthoption ivanconfig::PlaySounds(       "PlaySounds",
                                          "Use sound effects",
                                          "Use sound effects for combat, explosions and more.",
                                          true);
truthoption ivanconfig::ShowTurn(         "ShowTurn",
                                          "Show game turn on message log",
                                          "Add a game turn number to each action described in the message log.",
                                          false);
truthoption ivanconfig::OutlinedGfx(      "OutlinedGfx",
                                          "* Use outlined graphics",
                                          "The game graphics will be outlined in black for better differentiation.",
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

void ivanconfig::DistLimitMagicMushroomsDisplayer(const cycleoption* O, festring& Entry)
{
  if(O->Value == 0)
    Entry << "everywhere";
  else if(O->Value == 1)
    Entry << "close";
  else if(O->Value == 2)
    Entry << "near";
  else if(O->Value == 3)
    Entry << "medium";
  else if(O->Value == 4)
    Entry << "far";
  else
    Entry << O->Value;
}

void ivanconfig::StackListPageLengthDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " entries";
}

void ivanconfig::WindowHeightDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value << " pixels";
}

void ivanconfig::AltSilhouettePreventColorGlitchDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "transparent";break;
  case 1: Entry << "dark";break;
  case 2: Entry << "alignment";break;
  }
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

void ivanconfig::HoldPosMaxDistDisplayer(const cycleoption* O, festring& Entry)
{
  if(O->Value>0)
    Entry << O->Value << " squares";
  else
    Entry << "disabled";
}

void ivanconfig::HitIndicatorDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "disabled";break;
  case 1: Entry << "immersive";break;
  case 2: Entry << "indicator";break;
  case 3: Entry << "indicator + color";break;
  case 4: Entry << "dynamic";break;
  }
}

void ivanconfig::GoOnStopModeDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
    case 0:Entry << "everything";break;
    case 1:Entry << "ignore useless";break;
    case 2:Entry << "ignore cheap";break;
    case 3:Entry << "ignore encumbering";break;
  }
}

void ivanconfig::ShowMapDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
    case 0:Entry << "vanilla";break; //mmm... just not using xBRZ
    case 1:Entry << "xBRZ";break;
    case 2:Entry << "immersive 1";break;
    case 3:Entry << "immersive 2";break;
    case 4:Entry << "immersive 3";break;
  }
}

void ivanconfig::ShowItemsAtPlayerSquareDisplayer(const cycleoption* O, festring& Entry)
{
  if(O->Value>=10){
    Entry << "Use corners if NPC";
    if(O->Value==11)Entry << "+Items";
    Entry << " above";
    return;
  }

  int iCode = game::ItemUnderCode(O->Value);

  if(iCode==0){
    Entry << "disabled";
  }else
  if(iCode==1){
    Entry << "above head";
  }else{
    switch(game::ItemUnderCorner(iCode)){
      case 0:Entry << "upper left";break;
      case 1:Entry << "upper right";break;
      case 2:Entry << "lower left";break;
      case 3:Entry << "lower right";break;
    }

//    Entry << ",";
//    Entry << "x" << game::ItemUnderZoom(O->Value);

    Entry << ", ";
    Entry << (game::ItemUnderHV(iCode) ? "horizontal" : "vertical");
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
    case 4: Entry << "breathSlower"; break;
    case 5: Entry << "breathSlower+"; break;
    case 6: Entry << "breathSlower++"; break;
  }
}


truth ConfigureCustomKeys()
{
  festring fsFl = game::GetUserDataDir() + CUSTOM_KEYS_FILENAME;
  
  festring fsFlBkp=fsFl+".bkp";
  std::ifstream  src(fsFl.CStr()   , std::ios::binary);
  std::ofstream  dst(fsFlBkp.CStr(), std::ios::binary);
  dst << src.rdbuf();
  
  FILE *fl = fopen(fsFl.CStr(), "wt"); //"a");
  int iKey;
  festring fsAsk;
  bool bRet=true;
  int index=0;
  int aiKeyList[8]={0,0,0,0, 0,0,0,0};
  while(true){
    if(index>=8)break; //skip the last to keep as '.'
    
    bool bRetry=false;
    
    switch(index){
      case 0: fsAsk="Upper Left";break;
      case 1: fsAsk="Up"; break;
      case 2: fsAsk="Upper Right"; break;
      case 3: fsAsk="Left"; break;
      case 4: fsAsk="Right"; break;
      case 5: fsAsk="Lower Left"; break;
      case 6: fsAsk="Down"; break;
      case 7: fsAsk="Lower Right"; break;
      //case 8: fsAsk="Stop"; break; //skip the last to keep as '.'
    }
    iKey=game::AskForKeyPress(fsAsk);
    if(iKey==KEY_ESC){bRet=false;break;}
    
    for(int c = 1; commandsystem::GetCommand(c); ++c){
      if(iKey==commandsystem::GetCommand(c)->GetKey()){
        ADD_MESSAGE("SYSTEM: conflicting command key '%c'(code is %d or 0x%X), retry...",iKey,iKey,iKey); //TODO this messes the gameplay message log... but is better than a popup?
        bRetry=true;
        break;
      }
    }
    for(int c = 0; c<8; ++c){
      if(iKey==aiKeyList[c]){
        ADD_MESSAGE("SYSTEM: conflicting movement key '%c'(code is %d or 0x%X), retry...",iKey,iKey,iKey); //TODO this messes the gameplay message log... but is better than a popup?
        bRetry=true;
        break;
      }
    }
    if(bRetry)continue;
    
    fprintf(fl, "%04X\n", iKey);
    aiKeyList[index]=iKey;
    
    index++;
  }
  
  for(int c = 1; commandsystem::GetCommand(c); ++c){
    fprintf(fl, "\"%s\"=0x%04X='%c'\n", 
      commandsystem::GetCommand(c)->GetDescription(), 
      commandsystem::GetCommand(c)->GetKey(),
      commandsystem::GetCommand(c)->GetKey()
    );
  }
  
  fclose(fl);
  
  if(bRet)game::LoadCustomCommandKeys();
  return bRet;
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
     Entry << CONST_S("no");
  }
}

void clearToBackgroundAfterChangeInterface(){
  if(game::IsRunning())
    igraph::BlitBackGround(
      v2(16,6),
      v2(game::GetMaxScreenXSize() << 4, 23)
    );
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

truth ivanconfig::FontGfxChangeInterface(cycleoption* O)
{
  O->ChangeValue(O->Value % O->CycleCount + 1);
  clearToBackgroundAfterChangeInterface();
  return true;
}


truth ivanconfig::FantasyNameChangeInterface(stringoption* O)
{
  festring String;
  if(O)String<<O->Value;

  if(iosystem::StringQuestion(String, CONST_S("Set name generator pattern (recommended \"!ss !sV\"):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::DefaultNameChangeInterface(stringoption* O)
{
  festring String;
  if(O)String<<O->Value;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::SelectedBkgColorChangeInterface(stringoption* O)
{
  festring String;
  if(O)String<<O->Value;

  if(iosystem::StringQuestion(String, CONST_S("Set new RGB color (8 to 200 for each value, default \"8,8,8\"), or leave empty to disable:"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::AutoPickUpMatchingChangeInterface(stringoption* O)
{
  festring String;
  if(O)String<<O->Value;

  if(iosystem::StringQuestion(String, CONST_S("What items do you want to automatically pick up?"),
                              GetQuestionPos(), WHITE, 0, 200, !game::IsRunning(), true) == NORMAL_EXIT) //TODO should have no limit? but crashes if going beyond screen limit...
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::DefaultPetNameChangeInterface(stringoption* O)
{
  festring String;
  if(O)String<<O->Value;

  if(iosystem::StringQuestion(String, CONST_S("Set new default name for the starting pet (1-20 letters):"),
                              GetQuestionPos(), WHITE, 0, 20, !game::IsRunning(), true) == NORMAL_EXIT)
    O->ChangeValue(String);

  clearToBackgroundAfterChangeInterface();

  return false;
}

truth ivanconfig::XBRZSquaresAroundPlayerChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set how many squares around player to use xBRZ at (0 for whole dungeon):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::AltListItemWidthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new item list width:"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::FrameSkipChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set frame skip (-2 = wait, -1 = auto, or 0 to 100):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::StackListPageLengthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new length of list pages (in entries):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::WindowHeightChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new window height (from 480 to your monitor screen max height):"),
                                          GetQuestionPos(), WHITE, !game::IsRunning()));
  clearToBackgroundAfterChangeInterface();
  return false;
}

truth ivanconfig::WindowWidthChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new window width (from 640 to your monitor screen max width):"),
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
  if(What < 480) What = 480;
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
  if(What < 640) What = 640;
  O->Value = What;
}

void ivanconfig::SelectedBkgColorChanger(stringoption* O, cfestring& What)
{
  if(!What.IsEmpty()){
    int RGB[3]={1,1,1}, j=0;
    std::string sC;
    for(int i=0;i<What.GetSize();i++){
      if(j==3)return; //wrong usage detected

      if(What[i]>=0x30 && What[i]<=0x39) //0-9
        sC+=What[i];
      else{
        if(What[i]!=',') //wrong usage detected
          return;
      }

      if(What[i]==',' || i==What.GetSize()-1){
        RGB[j]=std::stol(sC);
        if(RGB[j]<8)return; //0,0,0 makes xBRZ not work well. 8,8,8 is min to have col16 not 0,0,0 (it is less bits than col24 per component)
        if(RGB[j]>200)return; //if all too high will prevent reading white text
        j++;
        sC="";
      }
    }

    if(j!=3)return; //wrong usage detected

    felist::SetSelectedBkgColor(MakeRGB16(RGB[0],RGB[1],RGB[2]));
  }else{
    felist::SetSelectedBkgColor(TRANSPARENT_COLOR);
  }

  if(O!=NULL){
    O->Value.Empty();
    O->Value<<What;
  }
}

void ivanconfig::AutoPickUpMatchingChanger(stringoption* O, cfestring& What)
{
  if(O!=NULL){
    O->Value.Empty();
    O->Value<<What;
    game::UpdateAutoPickUpMatching();
  }
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

void ivanconfig::SaveGameSortModeDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "newest first";break;
  case 1: Entry << "newest first + progress";break;
  case 2: Entry << "alphanumeric";break;
  case 3: Entry << "alphanumeric + progress";break;
  }
}

void ivanconfig::MemorizeEquipmentModeDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0:Entry << "disabled";break; //do not show
  case 1:Entry << "only pet";break; //default pos
  case 2:Entry << "everyone";break; //alt pos
  }
}

void ivanconfig::DungeonGfxScaleDisplayer(const cycleoption* O, festring& Entry)
{
  Entry << O->Value << 'x';
}

void ivanconfig::FontGfxDisplayer(const cycleoption* O, festring& Entry)
{
  Entry << O->Value;
}

void ivanconfig::SilhouetteScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
}

void ivanconfig::SaveGameSortModeChanger(cycleoption* O, long What)
{
  if(O!=NULL)O->Value = What;

  iosystem::SetSaveGameSortMode(What);
}

void ivanconfig::DungeonGfxScaleChanger(cycleoption* O, long What)
{
  O->Value = What;
}

void ivanconfig::FontGfxChanger(cycleoption* O, long What)
{
  O->Value = What;
}

void ivanconfig::SetupCustomKeysChanger(truthoption* O, truth What)
{
  if(game::IsRunning() || !What){
    O->Value = What;
    if(O->Value)
      ConfigureCustomKeys();
  }
}

void ivanconfig::XBRZScaleChanger(truthoption* O, truth What)
{
  O->Value = What;

  game::UpdateSRegionsXBRZ();
}

void ivanconfig::AllowMouseOnFelistChanger(truthoption* O, truth What)
{
  if(O!=NULL)O->Value = What;
  felist::SetAllowMouse(What);
  graphics::SetAllowMouseInFullScreen(What);
}

void ivanconfig::FullScreenModeChanger(truthoption*, truth)
{
  graphics::SwitchMode();
}

void ivanconfig::ScalingQualityDisplayer(const cycleoption* O, festring& Entry)
{
  switch(O->Value){
  case 0: Entry << "pixelated"; break;
  case 1: Entry << "smooth"; break;
  }
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
int  ivanconfig::iStartingFontGfx=-1;
bool ivanconfig::bStartingOutlinedGfx=false;
void ivanconfig::Initialize()
{
  festring fsCategory;

  fsCategory="General Setup";
  configsystem::AddOption(fsCategory,&DefaultName);
  configsystem::AddOption(fsCategory,&FantasyNamePattern);
  configsystem::AddOption(fsCategory,&DefaultPetName);
  configsystem::AddOption(fsCategory,&StartWithNoPet);
  configsystem::AddOption(fsCategory,&AutoSaveInterval);
  configsystem::AddOption(fsCategory,&AltAdentureInfo);

  fsCategory="Gameplay Options";
  configsystem::AddOption(fsCategory,&BeNice);
  configsystem::AddOption(fsCategory,&HoldPosMaxDist);
  //configsystem::AddOption(fsCategory,&MemorizeEquipmentMode); // Let everyone retore equipped items on unpolymorph.
  configsystem::AddOption(fsCategory,&WarnAboutDanger);
  configsystem::AddOption(fsCategory,&AutoDropLeftOvers);
  configsystem::AddOption(fsCategory,&SmartOpenCloseApply);
  configsystem::AddOption(fsCategory,&CenterOnPlayerAfterLook);
  configsystem::AddOption(fsCategory,&DescriptiveHP);
  configsystem::AddOption(fsCategory,&ShowGodInfo); //gameplay change in a sense that, to remember what each god is about may be a challenge on itself :)
  configsystem::AddOption(fsCategory,&ShowMapAtDetectMaterial);
  configsystem::AddOption(fsCategory,&GoOnStopMode);
  configsystem::AddOption(fsCategory,&WaitNeutralsMoveAway);
  configsystem::AddOption(fsCategory,&AllWeightIsRelevant);
  configsystem::AddOption(fsCategory,&ShowVolume);
  configsystem::AddOption(fsCategory,&EnhancedLights);
  configsystem::AddOption(fsCategory,&DistLimitMagicMushrooms);
  configsystem::AddOption(fsCategory,&AutoPickupThrownItems);
  configsystem::AddOption(fsCategory,&AutoPickUpMatching);
  configsystem::AddOption(fsCategory,&DropBeforeOffering);

  fsCategory="Game Window";
  configsystem::AddOption(fsCategory,&Contrast);
  configsystem::AddOption(fsCategory,&WindowWidth);
  configsystem::AddOption(fsCategory,&WindowHeight);
#ifndef __DJGPP__
  configsystem::AddOption(fsCategory,&GraphicsScale);
  configsystem::AddOption(fsCategory,&FullScreenMode);
#endif

  fsCategory="Graphics";
#ifndef __DJGPP__
  configsystem::AddOption(fsCategory,&ScalingQuality);
#endif
  configsystem::AddOption(fsCategory,&LookZoom);
  configsystem::AddOption(fsCategory,&XBRZScale);
  configsystem::AddOption(fsCategory,&XBRZSquaresAroundPlayer);
  configsystem::AddOption(fsCategory,&SilhouetteScale);
  configsystem::AddOption(fsCategory,&AltSilhouette);
  configsystem::AddOption(fsCategory,&AltSilhouettePreventColorGlitch);
  configsystem::AddOption(fsCategory,&AltListItemPos);
  configsystem::AddOption(fsCategory,&AltListItemWidth);
  configsystem::AddOption(fsCategory,&StackListPageLength);
  configsystem::AddOption(fsCategory,&DungeonGfxScale);
  configsystem::AddOption(fsCategory,&FontGfx);
  configsystem::AddOption(fsCategory,&OutlinedGfx);
  configsystem::AddOption(fsCategory,&FrameSkip);
  configsystem::AddOption(fsCategory,&ShowItemsAtPlayerSquare);
  configsystem::AddOption(fsCategory,&RotateTimesPerSquare);
  configsystem::AddOption(fsCategory,&HitIndicator);
  configsystem::AddOption(fsCategory,&ShowMap);
  configsystem::AddOption(fsCategory,&TransparentMapLM);

  fsCategory="Sounds";
  configsystem::AddOption(fsCategory,&PlaySounds);

  std::vector<std::string> DeviceNames;
  int NumDevices = audio::GetMIDIOutputDevices(DeviceNames);
  MIDIOutputDevice.Value = 0;
  if( NumDevices )
  {
     MIDIOutputDevice.Value = 1;
  }
  MIDIOutputDevice.CycleCount = NumDevices+1;

  configsystem::AddOption(fsCategory,&MIDIOutputDevice);
  configsystem::AddOption(fsCategory,&Volume);

  fsCategory="Input and Interface";
  configsystem::AddOption(fsCategory,&DirectionKeyMap);
  configsystem::AddOption(fsCategory,&SetupCustomKeys);
  configsystem::AddOption(fsCategory,&SaveGameSortMode);
  configsystem::AddOption(fsCategory,&ShowTurn);
  configsystem::AddOption(fsCategory,&ShowFullDungeonName);
  configsystem::AddOption(fsCategory,&SelectedBkgColor);
  configsystem::AddOption(fsCategory,&AllowMouseOnFelist);

  fsCategory="Advanced Options";
  configsystem::AddOption(fsCategory,&AllowImportOldSavegame);
  configsystem::AddOption(fsCategory,&HideWeirdHitAnimationsThatLookLikeMiss);
  configsystem::AddOption(fsCategory,&UseLightEmiterBasedOnVolume);

  /********************************
   * LOAD AND APPLY some SETTINGS *
   ********************************/
#if defined(WIN32) || defined(__DJGPP__)
  configsystem::SetConfigFileName(game::GetUserDataDir() + "ivan.cfg");
#else
  configsystem::SetConfigFileName(game::GetUserDataDir() + "ivan.conf");
#endif

  configsystem::Load();

  iStartingWindowWidth = WindowWidth.Value;
  iStartingWindowHeight = WindowHeight.Value;
  iStartingDungeonGfxScale = DungeonGfxScale.Value;
  iStartingFontGfx = FontGfx.Value;
  bStartingOutlinedGfx = OutlinedGfx.Value;

  CalculateContrastLuminance();
  audio::ChangeMIDIOutputDevice(MIDIOutputDevice.Value);
  audio::SetVolumeLevel(Volume.Value);
  
  if(ivanconfig::IsSetupCustomKeys())
    game::LoadCustomCommandKeys();

  //TODO re-use changer methods for above configs too to avoid duplicating the algo?
  FrameSkipChanger(NULL,FrameSkip.Value);
  StackListPageLengthChanger(NULL, StackListPageLength.Value);
  SaveGameSortModeChanger(NULL, SaveGameSortMode.Value);
  SelectedBkgColorChanger(NULL, SelectedBkgColor.Value);
  AutoPickUpMatchingChanger(NULL, AutoPickUpMatching.Value);
  AllowMouseOnFelistChanger(NULL, AllowMouseOnFelist.Value);
}
