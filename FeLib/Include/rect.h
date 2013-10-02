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

#ifndef __RECT_H__
#define __RECT_H__

#include "v2.h"

struct rect
{
  rect() { }
  rect(int X1, int Y1, int X2, int Y2) : X1(X1), Y1(Y1), X2(X2), Y2(Y2) { }
  rect(v2 V1, v2 V2) : X1(V1.X), Y1(V1.Y), X2(V2.X), Y2(V2.Y) { }
  rect operator+(v2 V) const
  { return rect(X1 + V.X, Y1 + V.Y, X2 + V.X, Y2 + V.Y); }
  int X1, Y1, X2, Y2;
};

#endif
