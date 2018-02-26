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

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "v2.h"

#define ADD_MESSAGE msgsystem::AddMessage

class felist;
class outputfile;
class inputfile;
class bitmap;
class festring;

class msgsystem
{
 public:
  static void LIKE_PRINTF(1, 2) AddMessage(cchar*, ...);
  static void Draw();
  static void DrawMessageHistory();
  static void Format();
  static void Save(outputfile&);
  static void Load(inputfile&);
  static void ScrollDown();
  static void ScrollUp();
  static void EnableMessages() { Enabled = true; }
  static void DisableMessages() { Enabled = false; }
  static void EnterBigMessageMode() { BigMessageMode = true; }
  static void LeaveBigMessageMode();
  static void Init();
  static void InitMessagesSinceLastKeyScan();
  static void ThyMessagesAreNowOld();
 private:
  static felist MessageHistory;
  static festring LastMessage;
  static festring BigMessage;
  static int Times;
  static v2 Begin, End;
  static truth Enabled;
  static truth BigMessageMode;
  static truth MessagesChanged;
  static bitmap* QuickDrawCache;
  static int LastMessageLines;
};

#ifndef NOSOUND
#include "SDL_mixer.h"
#include <vector>
#include "festring.h"

class soundsystem
{
 public:
  static void playSound(festring Buffer);
 
 private:
  static int SoundState;
  static void initSound();
  static std::vector<struct SoundFile> files;
  static std::vector<struct SoundInfo> patterns;
  static int addFile(festring filename);
  static truth matches(festring Pattern, festring Buffer);
  static struct SoundFile *findMatchingSound(festring Buffer);
};
#endif

#endif
