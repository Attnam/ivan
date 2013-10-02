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

#ifndef __V2_H__
#define __V2_H__

#include <cstdlib>

#include "felibdef.h"

struct v2;

struct packv2
{
  operator v2() const;
  short X, Y;
};

struct podv2
{
  operator v2() const;
  int X, Y;
};

/* Standard structure for representing positions */

struct v2
{
  v2() { }
  v2(int X, int Y) : X(X), Y(Y) { }
  v2 operator+(v2 V) const { return v2(X + V.X, Y + V.Y); }
  v2& operator+=(v2 V) { X += V.X; Y += V.Y; return *this; }
  v2 operator-(v2 V) const { return v2(X - V.X, Y - V.Y); }
  v2& operator-=(v2 V) { X -= V.X; Y -= V.Y; return *this; }
  v2 operator-()const { return v2(-X, -Y); }
  v2 operator*(int I) const { return v2(X * I, Y * I); }
  v2& operator*=(int I) { X *= I; Y *= I; return *this; }
  v2 operator/(int I) const { return v2(X / I, Y / I); }
  v2& operator/=(int I) { X /= I; Y /= I; return *this; }
  v2 operator*(double D) const
  { return v2(int(X * D), int(Y * D)); }
  v2& operator*=(double D)
  { X = int(X * D); Y = int(Y * D); return *this; }
  v2 operator/(double D) const
  { return v2(int(X / D), int(Y / D)); }
  v2& operator/=(double D)
  { X = int(X / D); Y = int(Y / D); return *this; }
  truth operator==(v2 V) const { return X == V.X && Y == V.Y; }
  truth operator!=(v2 V) const { return X != V.X || Y != V.Y; }
  v2 operator<<(int S) const { return v2(X << S, Y << S); }
  v2& operator<<=(int S) { X <<= S; Y <<= S; return *this; }
  v2 operator>>(int S) const { return v2(X >> S, Y >> S); }
  v2& operator>>=(int S) { X >>= S; Y >>= S; return *this; }
  bool operator<(v2 V) const
  { return X < V.X || (X == V.X && Y < V.Y); }
  int GetLengthSquare() const { return X * X + Y * Y; }
  /* Also returns true if V == *this */
  truth IsAdjacent(v2 V) const
  { return V.X >= X - 1 && V.X <= X + 1 && V.Y <= Y + 1 && V.Y >= Y - 1; }
  int GetManhattanLength() const { return Max(abs(X), abs(Y)); }
  truth Is0() const { return X == 0 && Y == 0; }
  operator packv2() const
  {
    packv2 V = { X, Y };
    return V;
  }
  operator podv2() const
  {
    podv2 V = { X, Y };
    return V;
  }

  //  v2 Randomize() const; Would be a good idea.
  int X, Y;
};

inline packv2::operator v2() const { return v2(X, Y); }
inline podv2::operator v2() const { return v2(X, Y); }

/*
 * Rotates a position Vect of a square map of size
 * Size x Size according to Flags (see felibdef.h)
 */

inline void Rotate(v2& Vect, int Size, int Flags)
{
  cint Limit = Size - 1;

  if(Flags & ROTATE)
  {
    cint T = Vect.X;
    Vect.X = Limit - Vect.Y;
    Vect.Y = T;
  }

  if(Flags & MIRROR)
    Vect.X = Limit - Vect.X;

  if(Flags & FLIP)
    Vect.Y = Limit - Vect.Y;
}

cv2 ZERO_V2(0, 0);
cv2 ERROR_V2(-0x8000, -0x8000);
cv2 ABORT_V2(-0x7FFF, -0x7FFF);

#endif
