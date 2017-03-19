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

#ifndef __HSCORE_H__
#define __HSCORE_H__

#include <vector>
#include <ctime>

#include "festring.h"

#ifdef UNIX
#define HIGH_SCORE_FILENAME LOCAL_STATE_DIR "/ivan-highscore.scores"
#endif

#if defined(WIN32) || defined(__DJGPP__)
#define HIGH_SCORE_FILENAME CONST_S("HScore.dat")
#endif

class festring;

enum highscoreview
{
  LOCAL,
  GLOBAL
};

class highscore
{
 public:
  highscore(cfestring& = HIGH_SCORE_FILENAME);
  truth Add(long, cfestring&, cfestring&, cfestring&, cfestring&);
  void Draw(cfestring&);
  void Save(cfestring& = HIGH_SCORE_FILENAME) const;
  void Load(cfestring& = HIGH_SCORE_FILENAME);
  truth LastAddFailed() const;
  void AddToFile(highscore*) const;
  truth MergeToFile(highscore*) const;
  int Find(long, cfestring&, time_t, long);
  cfestring& GetEntry(int) const;
  long GetScore(int) const;
  long GetSize() const;
  ushort GetVersion() const { return Version; }
  void Clear();
  truth CheckVersion() const;
 private:
  truth Add(long, cfestring&, time_t, long, cfestring&, cfestring&, cfestring&);
  void ToggleBetweenLocalAndGlobalView() { View = static_cast<highscoreview>(!View); }
  std::vector<festring> Entry;
  std::vector<long> Score;
  std::vector<time_t> Time;
  std::vector<long> RandomID;
  std::vector<festring> GlobalEntry;
  std::vector<long> GlobalScore;
  std::vector<time_t> GlobalTime;
  int LastAdd;
  ushort Version;
  static highscoreview View;
};

#endif
