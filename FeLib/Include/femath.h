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

#ifndef __FEMATH_H__
#define __FEMATH_H__

#include <vector>
#include <cmath>

#include "v2.h"
#include "rect.h"

#define RAND femath::Rand
#define RAND_N femath::RandN
#define RAND_2 (femath::Rand() & 1)
#define RAND_4 (femath::Rand() & 3)
#define RAND_8 (femath::Rand() & 7)
#define RAND_16 (femath::Rand() & 15)
#define RAND_32 (femath::Rand() & 31)
#define RAND_64 (femath::Rand() & 63)
#define RAND_128 (femath::Rand() & 127)
#define RAND_256 (femath::Rand() & 255)
#define RAND_GOOD femath::RandGood

class outputfile;
class inputfile;
template <class type> struct fearray;

class femath
{
 public:
  static long Rand();
  static void SetSeed(ulong);
  static long RandN(long N) { return long(double(N) * Rand() / 0x80000000); }
  static long RandGood(long N)
  { return long(double(N) * Rand() / 0x80000000); }
  static int WeightedRand(long*, long);
  static int WeightedRand(const std::vector<long>&, long);
  static double CalculateAngle(v2);
  static void CalculateEnvironmentRectangle(rect&, const rect&, v2, int);
  static truth Clip(int&, int&, int&, int&, int&, int&, int, int, int, int);
  static void SaveSeed();
  static void LoadSeed();
  static long SumArray(const fearray<long>&);
  static int LoopRoll(int, int);
  static void GenerateFractalMap(int**, int, int, int);
 protected:
  static ulong mt[];
  static long mti;
  static ulong mtb[];
  static long mtib;
};

struct interval
{
  long Randomize() const
  { return Min < Max ? Min + RAND() % (Max - Min + 1) : Min; }
  long Min;
  long Max;
};

struct region
{
  v2 Randomize() const { return v2(X.Randomize(), Y.Randomize()); }
  interval X;
  interval Y;
};

void ReadData(interval&, inputfile&);
void ReadData(region&, inputfile&);

outputfile& operator<<(outputfile&, const interval&);
inputfile& operator>>(inputfile&, interval&);
outputfile& operator<<(outputfile&, const region&);
inputfile& operator>>(inputfile&, region&);

template <class controller> class mapmath
{
 public:
  static truth DoLine(int, int, int, int, int = 0);
  static void DoArea();
  static void DoQuadriArea(int, int, int, int, int);
};

template <class controller>
inline truth mapmath<controller>::DoLine(int X1, int Y1,
					 int X2, int Y2, int Flags)
{
  if(!(Flags & SKIP_FIRST))
    controller::Handler(X1, Y1);

  cint DeltaX = abs(X2 - X1);
  cint DeltaY = abs(Y2 - Y1);
  cint DoubleDeltaX = DeltaX << 1;
  cint DoubleDeltaY = DeltaY << 1;
  cint XChange = X1 < X2 ? 1 : -1;
  cint YChange = Y1 < Y2 ? 1 : -1;
  int x = X1, y = Y1;

  if(DeltaX >= DeltaY)
  {
    int c = DeltaX;
    cint End = X2;

    while(x != End)
    {
      x += XChange;
      c += DoubleDeltaY;

      if(c >= DoubleDeltaX)
      {
	c -= DoubleDeltaX;
	y += YChange;
      }

      if(!controller::Handler(x, y))
	return x == End && !(Flags & ALLOW_END_FAILURE);
    }
  }
  else
  {
    int c = DeltaY;
    cint End = Y2;

    while(y != End)
    {
      y += YChange;
      c += DoubleDeltaX;

      if(c >= DoubleDeltaY)
      {
	c -= DoubleDeltaY;
	x += XChange;
      }

      if(!controller::Handler(x, y))
	return y == End && !(Flags & ALLOW_END_FAILURE);
    }
  }

  return true;
}

struct basequadricontroller
{
  static cint OrigoDeltaX[4];
  static cint OrigoDeltaY[4];
  static int OrigoX, OrigoY;
  static int StartX, StartY;
  static int XSize, YSize;
  static int RadiusSquare;
  static truth SectorCompletelyClear;
};

template <class controller>
struct quadricontroller : public basequadricontroller
{
  static truth Handler(int, int);
  static int GetStartX(int I)
  {
    SectorCompletelyClear = true;
    return StartX = (OrigoX << 1) + OrigoDeltaX[I];
  }
  static int GetStartY(int I)
  {
    return StartY = (OrigoY << 1) + OrigoDeltaY[I];
  }
};

template <class controller>
truth quadricontroller<controller>::Handler(int x, int y)
{
  cint HalfX = x >> 1, HalfY = y >> 1;

  if(HalfX >= 0 && HalfY >= 0 && HalfX < XSize && HalfY < YSize)
  {
    ulong& SquareTick = controller::GetTickReference(HalfX, HalfY);
    cint SquarePartIndex = (x & 1) + ((y & 1) << 1);
    culong Mask = SquarePartTickMask[SquarePartIndex];

    if((SquareTick & Mask) < controller::ShiftedTick[SquarePartIndex])
    {
      SquareTick = SquareTick & ~Mask
		   | controller::ShiftedQuadriTick[SquarePartIndex];
      int DeltaX = OrigoX - HalfX, DeltaY = OrigoY - HalfY;

      if(DeltaX * DeltaX + DeltaY * DeltaY <= RadiusSquare)
      {
	if(SectorCompletelyClear)
	{
	  if(controller::Handler(x, y))
	    return true;
	  else
	    SectorCompletelyClear = false;
	}
	else
	  return mapmath<controller>::DoLine(StartX, StartY,
					     x, y,
					     SKIP_FIRST
					     |ALLOW_END_FAILURE);
      }
    }
  }

  return false;
}

cint ChangeXArray[4][3] = { { -1,  0, -1 },
				 {  0,  1,  1 },
				 { -1, -1,  0 },
				 {  1,  0,  1 } };
cint ChangeYArray[4][3] = { { -1, -1,  0 },
				 { -1, -1,  0 },
				 {  0,  1,  1 },
				 {  0,  1,  1 } };

template <class controller>
inline void mapmath<controller>::DoArea()
{
  int Buffer[2][2048];
  int* OldStack = Buffer[0];
  int* NewStack = Buffer[1];

  for(int c1 = 0; c1 < 4; ++c1)
  {
    cint* ChangeX = ChangeXArray[c1], * ChangeY = ChangeYArray[c1];
    int OldStackPos = 0, NewStackPos = 0;
    int StartX = controller::GetStartX(c1);
    int StartY = controller::GetStartY(c1);

    for(int c2 = 0; c2 < 3; ++c2)
    {
      OldStack[OldStackPos] = StartX + ChangeX[c2];
      OldStack[OldStackPos + 1] = StartY + ChangeY[c2];
      OldStackPos += 2;
    }

    while(OldStackPos)
    {
      while(OldStackPos)
      {
	OldStackPos -= 2;
	cint X = OldStack[OldStackPos], Y = OldStack[OldStackPos + 1];

	if(controller::Handler(X, Y))
	  for(int c2 = 0; c2 < 3; ++c2)
	  {
	    NewStack[NewStackPos] = X + ChangeX[c2];
	    NewStack[NewStackPos + 1] = Y + ChangeY[c2];
	    NewStackPos += 2;
	  }
      }

      OldStackPos = NewStackPos;
      NewStackPos = 0;
      int* T = OldStack;
      OldStack = NewStack;
      NewStack = T;
    }
  }
}

template <class controller>
inline void mapmath<controller>::DoQuadriArea(int OrigoX, int OrigoY,
					      int RadiusSquare,
					      int XSize, int YSize)
{
  basequadricontroller::OrigoX = OrigoX;
  basequadricontroller::OrigoY = OrigoY;
  basequadricontroller::RadiusSquare = RadiusSquare;
  basequadricontroller::XSize = XSize;
  basequadricontroller::YSize = YSize;

  for(int c = 0; c < 4; ++c)
    controller::Handler((OrigoX << 1) + basequadricontroller::OrigoDeltaX[c],
			(OrigoY << 1) + basequadricontroller::OrigoDeltaY[c]);

  mapmath<quadricontroller<controller> >::DoArea();
}

/* Chance for n < Max to be returned is (1-CC)*CC^n,
   for n == Max chance is CC^n. */

inline int femath::LoopRoll(int ContinueChance, int Max)
{
  int R;
  for(R = 0; RAND_N(100) < ContinueChance && R < Max; ++R);
  return R;
}

template <class type, class predicate>
type*& ListFind(type*& Start, predicate Predicate)
{
  type** E;
  for(E = &Start; *E && !Predicate(*E); E = &(*E)->Next);
  return *E;
}

template <class type>
struct pointercomparer
{
  pointercomparer(const type* Element) : Element(Element) { }
  truth operator()(const type* E) const { return E == Element; }
  const type* Element;
};

#endif
