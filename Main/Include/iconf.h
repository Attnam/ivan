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
  static truth GetWarnAboutDanger() { return WarnAboutDanger.Value; }
  static truth GetAutoDropLeftOvers() { return AutoDropLeftOvers.Value; }
  static truth GetLookZoom() { return LookZoom.Value; }
  static long GetDirectionKeyMap() { return DirectionKeyMap.Value; }
  static truth GetSmartOpenCloseApply() { return SmartOpenCloseApply.Value; }
  static truth GetBeNice() { return BeNice.Value; }
  static truth GetPlaySounds() { return PlaySounds.Value; }
  static truth GetShowTurn() { return ShowTurn.Value; }
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
  static void ContrastDisplayer(const numberoption*, festring&);
  static void DirectionKeyMapDisplayer(const cycleoption*, festring&);
  static truth DefaultNameChangeInterface(stringoption*);
  static truth DefaultPetNameChangeInterface(stringoption*);
  static truth AutoSaveIntervalChangeInterface(numberoption*);
  static truth ContrastChangeInterface(numberoption*);
  static void AutoSaveIntervalChanger(numberoption*, long);
  static void ContrastChanger(numberoption*, long);
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
  static void ContrastHandler(long);
  static void VolumeHandler(long);
  static void BackGroundDrawer();
  static stringoption DefaultName;
  static stringoption DefaultPetName;
  static numberoption AutoSaveInterval;
  static scrollbaroption Contrast;
  static truthoption WarnAboutDanger;
  static truthoption AutoDropLeftOvers;
  static truthoption LookZoom;
  static cycleoption DirectionKeyMap;
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
