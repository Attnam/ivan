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

#ifndef __ICONF_H__
#define __ICONF_H__

#include "config.h"
#include "v2.h"

class ivanconfig
{
 public:
  static cfestring& GetDefaultName() { return DefaultName.Value; }
  static cfestring& GetDefaultPetName() { return DefaultPetName.Value; }
  static long GetAutoSaveInterval() { return AutoSaveInterval.Value; }
  static long GetContrast() { return Contrast.Value; }
  static long GetStartingWindowWidth() { return iStartingWindowWidth; }
  static long GetStartingWindowHeight() { return iStartingWindowHeight; }
  static long GetFrameSkip() { return FrameSkip.Value; }
  static truth IsSavegameSafely(){ return SavegameSafely.Value; }
  static truth IsAllowImportOldSavegame(){ return AllowImportOldSavegame.Value; }
  static int GetMemorizeEquipmentMode() { return MemorizeEquipmentMode.Value; }
  static truth IsShowFullDungeonName() { return ShowFullDungeonName.Value; }
  static truth IsCenterOnPlayerAfterLook(){ return CenterOnPlayerAfterLook.Value; }
  static truth GetWarnAboutDanger() { return WarnAboutDanger.Value; }
  static truth GetAutoDropLeftOvers() { return AutoDropLeftOvers.Value; }
  static truth GetLookZoom() { return LookZoom.Value; }
  static truth IsXBRZScale() { return XBRZScale.Value; }
  static truth IsAltAdentureInfo() { return AltAdentureInfo.Value; }
  static int GetXBRZSquaresAroundPlayer() { return XBRZSquaresAroundPlayer.Value; }
  static int GetStartingDungeonGfxScale() { return iStartingDungeonGfxScale; }
  static int GetSilhouetteScale() { return SilhouetteScale.Value; }
  static long GetDirectionKeyMap() { return DirectionKeyMap.Value; }
  static int GetAltListItemWidth() { return AltListItemWidth.Value; }
  static int GetStackListPageLength() { return StackListPageLength.Value; }
  static truth GetSmartOpenCloseApply() { return SmartOpenCloseApply.Value; }
  static truth GetBeNice() { return BeNice.Value; }
  static int GetAltListItemPos() { return AltListItemPos.Value; }
  static truth GetPlaySounds() { return PlaySounds.Value; }
  static truth IsShowTurn() { return ShowTurn.Value; }
  static truth IsStartingOutlinedGfx() { return bStartingOutlinedGfx; }
  static long GetVolume() { return Volume.Value; }
  static long GetMIDIOutputDevice() { return MIDIOutputDevice.Value; }

#ifndef __DJGPP__
  static int GetGraphicsScale() { return GraphicsScale.Value; }
  static truth GetFullScreenMode() { return FullScreenMode.Value; }
  static void SwitchModeHandler();
#else
  static truth GetFullScreenMode() { return true; }
#endif

  static long ApplyContrastTo(long);
  static void Save() { configsystem::Save(); }
  static void Load() { configsystem::Load(); }
  static void CalculateContrastLuminance();
  static col24 GetContrastLuminance() { return ContrastLuminance; }
  static void Initialize();
  static void Show();

 private:
  static v2 GetQuestionPos();
  static void AutoSaveIntervalDisplayer(const numberoption*, festring&);
  static void XBRZSquaresAroundPlayerDisplayer(const numberoption* O, festring& Entry);
  static void WindowWidthDisplayer(const numberoption* O, festring& Entry);
  static void WindowHeightDisplayer(const numberoption* O, festring& Entry);
  static void StackListPageLengthDisplayer(const numberoption* O, festring& Entry);
  static void FrameSkipDisplayer(const numberoption* O, festring& Entry);
  static void AltListItemWidthDisplayer(const numberoption* O, festring& Entry);
  static void ContrastDisplayer(const numberoption*, festring&);
  static void DirectionKeyMapDisplayer(const cycleoption*, festring&);
  static truth DefaultNameChangeInterface(stringoption*);
  static truth DefaultPetNameChangeInterface(stringoption*);
  static truth AutoSaveIntervalChangeInterface(numberoption*);
  static truth XBRZSquaresAroundPlayerChangeInterface(numberoption* O);
  static truth WindowWidthChangeInterface(numberoption* O);
  static truth WindowHeightChangeInterface(numberoption* O);
  static truth StackListPageLengthChangeInterface(numberoption* O);
  static truth FrameSkipChangeInterface(numberoption* O);
  static truth AltListItemWidthChangeInterface(numberoption* O);
  static truth ContrastChangeInterface(numberoption*);
  static void AutoSaveIntervalChanger(numberoption*, long);
  static void XBRZSquaresAroundPlayerChanger(numberoption* O, long What);
  static void WindowWidthChanger(numberoption* O, long What);
  static void WindowHeightChanger(numberoption* O, long What);
  static void StackListPageLengthChanger(numberoption* O, long What);
  static void FrameSkipChanger(numberoption* O, long What);
  static void AltListItemWidthChanger(numberoption* O, long What);
  static void ContrastChanger(numberoption*, long);
  static void MemorizeEquipmentModeDisplayer(const cycleoption* O, festring& Entry);
  static void MIDIOutputDeviceDisplayer(const cycleoption*, festring&);
  static void VolumeDisplayer(const numberoption*, festring&);
  static truth VolumeChangeInterface(numberoption*);
  static void VolumeChanger(numberoption*, long);

#ifndef __DJGPP__
  static void GraphicsScaleDisplayer(const cycleoption*, festring&);
  static truth GraphicsScaleChangeInterface(cycleoption*);
  static void GraphicsScaleChanger(cycleoption*, long);
  static void FullScreenModeChanger(truthoption*, truth);
#endif

  static void DungeonGfxScaleDisplayer(const cycleoption*, festring&);
  static void SilhouetteScaleDisplayer(const cycleoption* O, festring& Entry);
  static void AltListItemPosDisplayer(const cycleoption* O, festring& Entry);
  static void SaveGameSortModeDisplayer(const cycleoption* O, festring& Entry);
  static truth DungeonGfxScaleChangeInterface(cycleoption*);
  static truth SilhouetteScaleChangeInterface(cycleoption*);
  static void DungeonGfxScaleChanger(cycleoption*, long);
  static void SilhouetteScaleChanger(cycleoption*, long);
  static void SaveGameSortModeChanger(cycleoption* O, long What);
  static void FullScreenModeChanger(truthoption*, truth);
  static void XBRZScaleChanger(truthoption*, truth);
  static void SavegameSafelyChanger(truthoption* O, truth What);
  static void ContrastHandler(long);
  static void VolumeHandler(long);
  static void BackGroundDrawer();

  static stringoption DefaultName;
  static stringoption DefaultPetName;
  static numberoption AutoSaveInterval;
  static truthoption AltAdentureInfo;
  static truthoption CenterOnPlayerAfterLook;
  static scrollbaroption Contrast;

  static numberoption WindowWidth;
  static int iStartingWindowWidth;

  static numberoption WindowHeight;
  static int iStartingWindowHeight;

  static numberoption FrameSkip;
  static truthoption ShowFullDungeonName;
  static truthoption AllowImportOldSavegame;
  static truthoption SavegameSafely;

  static cycleoption MemorizeEquipmentMode;

  static truthoption WarnAboutDanger;
  static truthoption AutoDropLeftOvers;
  static truthoption LookZoom;
  static truthoption XBRZScale;

  static cycleoption  SaveGameSortMode;

  static cycleoption  DungeonGfxScale;
  static int iStartingDungeonGfxScale;

  static truthoption   OutlinedGfx;
  static bool bStartingOutlinedGfx;

  static numberoption XBRZSquaresAroundPlayer;
  static cycleoption DirectionKeyMap;
  static cycleoption SilhouetteScale;
  static cycleoption AltListItemPos;
  static numberoption AltListItemWidth;
  static numberoption StackListPageLength;

  static truthoption SmartOpenCloseApply;
  static truthoption BeNice;
  static scrollbaroption Volume;
  static cycleoption MIDIOutputDevice;

#ifndef __DJGPP__
  static cycleoption GraphicsScale;
  static truthoption FullScreenMode;
#endif

  static col24 ContrastLuminance;
  static truthoption PlaySounds;
  static truthoption ShowTurn;
};

inline long ivanconfig::ApplyContrastTo(long L)
{
  long C = Contrast.Value;

  if(C == 100)
    return L;
  else
    return MakeRGB24(41 * GetRed24(L) * C >> 12,
                     41 * GetGreen24(L) * C >> 12,
                     41 * GetBlue24(L) * C >> 12);
}

#endif
