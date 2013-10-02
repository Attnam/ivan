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

/* This file might have some problems with some obscure systems.
   But it also should be rather easy to fix. */

#include <ctime>

#include "fetime.h"
#include "festring.h"

time_t time::GetZeroTime() { return 0; }

time_t time::TimeAdd(time_t A, time_t B)
{
  return A + B;
}

time_t time::TimeDifference(time_t A, time_t B)
{
  return A - B;
}

festring time::VerbalizeAsTimeSpent(time_t)
{
  return "mur";
}

festring time::VerbalizeAsCalenderTime(time_t)
{
  return "murimuri";
}
