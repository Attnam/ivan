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

#ifndef __PRAGMAS_H__
#define __PRAGMAS_H__

#include <cstring>
#include <cmath>

#ifdef VC
#pragma warning(disable : 4786 4146)
//#pragma optimize("g", on)
#pragma intrinsic(abs, fabs, labs, memcmp, memcpy, memset)
#pragma intrinsic(strcat, strcmp, strcpy, strlen, pow)
#pragma intrinsic(exp, log, sin, cos, tan, atan, sqrt)
#endif

#endif
