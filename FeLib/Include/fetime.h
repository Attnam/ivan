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

#ifndef __FETIME_H__
#define __FETIME_H__

#include <ctime>

#include "festring.h"

class time
{
 public:
  static time_t GetZeroTime();
  static time_t TimeAdd(time_t,time_t);
  static time_t TimeDifference(time_t,time_t);
  static festring VerbalizeAsTimeSpent(time_t);
  static festring VerbalizeAsCalenderTime(time_t);
};

#endif
