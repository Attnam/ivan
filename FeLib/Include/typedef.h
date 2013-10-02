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

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include "pragmas.h"

class bitmap;
class festring;
struct blitdata;
struct v2;

typedef long truth;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int col16;
typedef ushort packcol16;
typedef ulong col24;
typedef int alpha;
typedef uchar packalpha;
typedef int priority;
typedef uchar packpriority;
typedef uchar paletteindex;

typedef const char cchar;
typedef const int cint;
typedef const long ctruth;
typedef const unsigned char cuchar;
typedef const unsigned short cushort;
typedef const unsigned int cuint;
typedef const unsigned long culong;
typedef const int ccol16;
typedef const ushort cpackcol16;
typedef const ulong ccol24;
typedef const int calpha;
typedef const uchar cpackalpha;
typedef const int cpriority;
typedef const uchar cpackpriority;
typedef const uchar cpaletteindex;

typedef const bitmap cbitmap;
typedef const blitdata cblitdata;
typedef const festring cfestring;
typedef const v2 cv2;

#endif
