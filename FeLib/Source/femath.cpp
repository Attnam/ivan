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

#include <cmath>

#include "femath.h"
#include "error.h"
#include "save.h"

cint basequadricontroller::OrigoDeltaX[4] = { 0, 1, 0, 1 };
cint basequadricontroller::OrigoDeltaY[4] = { 0, 0, 1, 1 };
int basequadricontroller::OrigoX, basequadricontroller::OrigoY;
int basequadricontroller::StartX, basequadricontroller::StartY;
int basequadricontroller::XSize, basequadricontroller::YSize;
int basequadricontroller::RadiusSquare;
truth basequadricontroller::SectorCompletelyClear;

/* A C-program for MT19937: Integer     version                   */
/*  genrand() generates one pseudorandom unsigned integer (32bit) */
/* which is uniformly distributed among 0 to 2^32-1  for each     */
/* call. sgenrand(seed) set initial values to the working area    */
/* of 624 words. Before genrand(), sgenrand(seed) must be         */
/* called once. (seed is any 32-bit integer except for 0).        */
/*   Coded by Takuji Nishimura, considering the suggestions by    */
/* Topher Cooper and Marc Rieffel in July-Aug. 1997.              */

/* This library is free software; you can redistribute it and/or   */
/* modify it under the terms of the GNU Library General Public     */
/* License as published by the Free Software Foundation; either    */
/* version 2 of the License, or (at your option) any later         */
/* version.                                                        */
/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            */
/* See the GNU Library General Public License for more details.    */
/* You should have received a copy of the GNU Library General      */
/* Public License along with this library; if not, write to the    */
/* Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   */
/* 02111-1307  USA                                                 */

/* Copyright (C) 1997 Makoto Matsumoto and Takuji Nishimura.       */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

/* Period parameters */
#define N1 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

ulong femath::mt[N1]; /* the array for the state vector  */
long femath::mti = N1+1; /* mti==N+1 means mt[N] is not initialized */

/* backups */

ulong femath::mtb[N1];
long femath::mtib;

void femath::SetSeed(ulong Seed)
{
  /* setting initial seeds to mt[N] using         */
  /* the generator Line 25 of Table 1 in          */
  /* [KNUTH 1981, The Art of Computer Programming */
  /*    Vol. 2 (2nd Ed.), pp102]                  */

  mt[0] = Seed & 0xffffffff;

  for (mti=1; mti<N1; mti++)
    mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}

long femath::Rand()
{
  ulong y;
  static ulong mag01[2]={0x0, MATRIX_A};

  /* mag01[x] = x * MATRIX_A  for x=0,1 */

  if (mti >= N1) { /* generate N words at one time */
    int kk;

    if (mti == N1+1) /* if sgenrand() has not been called, */
      SetSeed(4357); /* a default initial seed is used   */

    for (kk=0;kk<N1-M;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    for (;kk<N1-1;kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+(M-N1)] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    y = (mt[N1-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
    mt[N1-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

    mti = 0;
  }

  y = mt[mti++];
  y ^= TEMPERING_SHIFT_U(y);
  y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
  y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
  y ^= TEMPERING_SHIFT_L(y);

  return y & 0x7FFFFFFF;
}

int femath::WeightedRand(long* Possibility, long TotalPossibility)
{
  long Rand = RAND() % TotalPossibility, PartialSum = 0;

  for(int c = 0;; ++c)
  {
    PartialSum += Possibility[c];

    if(PartialSum > Rand)
      return c;
  }
}


int femath::WeightedRand(const std::vector<long>& Possibility,
			 long TotalPossibility)
{
  long Rand = RAND() % TotalPossibility, PartialSum = 0;

  for(int c = 0;; ++c)
  {
    PartialSum += Possibility[c];

    if(PartialSum > Rand)
      return c;
  }
}

double femath::CalculateAngle(v2 Direction)
{
  if(Direction.X < 0)
    return atan(double(Direction.Y) / Direction.X) + FPI;
  else if(Direction.X > 0)
  {
    if(Direction.Y < 0)
      return atan(double(Direction.Y) / Direction.X) + 2 * FPI;
    else
      return atan(double(Direction.Y) / Direction.X);
  }
  else
  {
    if(Direction.Y < 0)
      return 3 * FPI / 2;
    else if(Direction.Y > 0)
      return FPI / 2;
    else
    {
      ABORT("Illegal direction (0, 0) passed to femath::CalculateAngle()!");
      return 0;
    }
  }
}

void femath::CalculateEnvironmentRectangle(rect& Rect,
					   const rect& MotherRect,
					   v2 Origo, int Radius)
{
  Rect.X1 = Origo.X - Radius;
  Rect.Y1 = Origo.Y - Radius;
  Rect.X2 = Origo.X + Radius;
  Rect.Y2 = Origo.Y + Radius;

  if(Rect.X1 < MotherRect.X1)
    Rect.X1 = MotherRect.X1;

  if(Rect.Y1 < MotherRect.Y1)
    Rect.Y1 = MotherRect.Y1;

  if(Rect.X2 > MotherRect.X2)
    Rect.X2 = MotherRect.X2;

  if(Rect.Y2 > MotherRect.Y2)
    Rect.Y2 = MotherRect.Y2;
}

truth femath::Clip(int& SourceX, int& SourceY, int& DestX, int& DestY, int& Width, int& Height, int XSize, int YSize, int DestXSize, int DestYSize)
{
  /* This sentence is usually true */

  if(SourceX >= 0
     && SourceY >= 0
     && DestX >= 0
     && DestY >= 0
     && SourceX + Width <= XSize
     && SourceY + Height <= YSize
     && DestX + Width <= DestXSize
     && DestY + Height <= DestYSize)
    return true;

  if(SourceX < 0)
  {
    Width += SourceX;
    DestX -= SourceX;
    SourceX = 0;
  }

  if(SourceY < 0)
  {
    Height += SourceY;
    DestY -= SourceY;
    SourceY = 0;
  }

  if(DestX < 0)
  {
    Width += DestX;
    SourceX -= DestX;
    DestX = 0;
  }

  if(DestY < 0)
  {
    Height += DestY;
    SourceY -= DestY;
    DestY = 0;
  }

  if(SourceX + Width > XSize)
    Width = XSize - SourceX;

  if(SourceY + Height > YSize)
    Height = YSize - SourceY;

  if(DestX + Width > DestXSize)
    Width = DestXSize - DestX;

  if(DestY + Height > DestYSize)
    Height = DestYSize - DestY;

  return Width > 0 && Height > 0;
}

void femath::SaveSeed()
{
  mtib = mti;

  for(int c = 0; c < N1; ++c)
    mtb[c] = mt[c];
}

void femath::LoadSeed()
{
  mti = mtib;

  for(int c = 0; c < N1; ++c)
    mt[c] = mtb[c];
}

void ReadData(interval& I, inputfile& SaveFile)
{
  I.Min = SaveFile.ReadNumber(HIGHEST, true);
  festring Word;
  SaveFile.ReadWord(Word);

  if(Word == ";" || Word == ",")
    I.Max = I.Min;
  else if(Word == ":")
    I.Max = Max(SaveFile.ReadNumber(), I.Min);
  else
    ABORT("Odd interval terminator %s detected, file %s line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());
}

void ReadData(region& R, inputfile& SaveFile)
{
  ReadData(R.X, SaveFile);
  ReadData(R.Y, SaveFile);
}

outputfile& operator<<(outputfile& SaveFile, const interval& I)
{
  SaveFile.Write(reinterpret_cast<cchar*>(&I), sizeof(I));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, interval& I)
{
  SaveFile.Read(reinterpret_cast<char*>(&I), sizeof(I));
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const region& R)
{
  SaveFile.Write(reinterpret_cast<cchar*>(&R), sizeof(R));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, region& R)
{
  SaveFile.Read(reinterpret_cast<char*>(&R), sizeof(R));
  return SaveFile;
}

long femath::SumArray(const fearray<long>& Vector)
{
  long Sum = 0;

  for(uint c = 0; c < Vector.Size; ++c)
    Sum += Vector.Data[c];

  return Sum;
}

void femath::GenerateFractalMap(int** Map, int Side,
				int StartStep,
				int Randomness)
{
  cint Limit = Side - 1;
  Map[0][0] = 0;
  Map[0][Limit] = 0;
  Map[Limit][0] = 0;
  Map[Limit][Limit] = 0;

  for(int Step = StartStep, HalfStep = Step >> 1;
      HalfStep;
      Step = HalfStep, HalfStep >>= 1,
    Randomness = ((Randomness << 3) - Randomness) >> 3)
  {
    int x, y, RandMod = (Randomness << 1) + 1;

    for(x = HalfStep; x < Side; x += Step)
      for(y = HalfStep; y < Side; y += Step)
	Map[x][y] = ((Map[x - HalfStep][y - HalfStep]
		      + Map[x - HalfStep][y + HalfStep]
		      + Map[x + HalfStep][y - HalfStep]
		      + Map[x + HalfStep][y + HalfStep])
		     >> 2) - Randomness + RAND() % RandMod;

    for(x = HalfStep; x < Side; x += Step)
      for(y = 0; y < Side; y += Step)
      {
	int HeightSum = Map[x - HalfStep][y] + Map[x + HalfStep][y];
	int Neighbours = 2;

	if(y)
	{
	  HeightSum += Map[x][y - HalfStep];
	  ++Neighbours;
	}

	if(y != Limit)
	{
	  HeightSum += Map[x][y + HalfStep];
	  ++Neighbours;
	}

	if(Neighbours == 4)
	  HeightSum >>= 2;
	else
	  HeightSum /= Neighbours;

	Map[x][y] = HeightSum - Randomness + RAND() % RandMod;
      }

    for(x = 0; x < Side; x += Step)
      for(y = HalfStep; y < Side; y += Step)
      {
	int HeightSum = Map[x][y - HalfStep] + Map[x][y + HalfStep];
	int Neighbours = 2;

	if(x)
	{
	  HeightSum += Map[x - HalfStep][y];
	  ++Neighbours;
	}

	if(x != Limit)
	{
	  HeightSum += Map[x + HalfStep][y];
	  ++Neighbours;
	}

	if(Neighbours == 4)
	  HeightSum >>= 2;
	else
	  HeightSum /= Neighbours;

	Map[x][y] = HeightSum - Randomness + RAND() % RandMod;
      }
  }
}
