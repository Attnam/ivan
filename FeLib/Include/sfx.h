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

#ifndef __SFX_H__
#define __SFX_H__

#ifndef NOSOUND
#include "SDL_mixer.h"
#include <vector>
#include "festring.h"

class soundeffects
{
 friend class msgsystem;

 public:
  static void playSound(festring Buffer);
  
  static void SetEnableSfx(bool b){bIsEnabled=b;}
  static void SetSfxVolume(long l){lSfxVol=l;}
  static void SetDataDir(festring fs){fsDataDir=fs;}
 
 private:
  static void initSound();
  static void deInitSound();
  static int addFile(festring filename);
  static truth matches(festring Pattern, festring Buffer);
  static struct SoundFile *findMatchingSound(festring Buffer);
  
  static int SoundState;
  static std::vector<struct SoundFile> files;
  static std::vector<struct SoundInfo> patterns;
  static bool bIsEnabled;
  static long lSfxVol;
  static festring fsDataDir;
};
#endif

#endif
