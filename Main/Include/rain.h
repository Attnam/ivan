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

#ifndef __RAIN_H__
#define __RAIN_H__

#include "lsquare.h"
#include "entity.h"

class rain : public entity
{
 public:
  /* Come To The Dark Side */
  rain* Next;
 public:
  rain() : entity(HAS_BE), Next(0), Drop(0), Drops(0), OwnLiquid(0) { }
  rain(liquid*, lsquare*, v2, int, truth);
  ~rain();
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(blitdata&) const;
  truth HasOwnLiquid() const { return OwnLiquid; }
  void RandomizeDropPos(int) const;
  liquid* GetLiquid() const { return Liquid; }
  virtual square* GetSquareUnderEntity(int = 0) const { return LSquareUnder; }
  square* GetSquareUnder() const { return LSquareUnder; }
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual truth IsOnGround() const { return true; }
  int GetTeam() const { return Team; }
 protected:
  mutable struct drop
  {
    packv2 StartPos;
    ushort StartTick;
    ushort MaxAge;
  }* Drop;
  liquid* Liquid;
  lsquare* LSquareUnder;
  v2 Speed;
  long SpeedAbs;
  mutable int Drops : 8;
  int BeCounter : 7;
  truth OwnLiquid : 1;
  int Team : 8;
};

outputfile& operator<<(outputfile&, const rain*);
inputfile& operator>>(inputfile&, rain*&);

#endif
