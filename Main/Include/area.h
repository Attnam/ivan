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

#ifndef __AREA_H__
#define __AREA_H__

#include <map>

#include "rect.h"

class character;
class square;
class bitmap;
class outputfile;
class inputfile;

class area
{
 public:
  area();
  area(int, int);
  virtual ~area();
  virtual void Draw(truth) const = 0;
  void Save(outputfile&) const;
  void Load(inputfile&);
  int GetFlag(v2 Pos) const { return FlagMap[Pos.X][Pos.Y]; }
  void AddFlag(v2 Pos, int What) { FlagMap[Pos.X][Pos.Y] |= What; }
  square* GetSquare(v2 Pos) const { return Map[Pos.X][Pos.Y]; }
  square* GetSquare(int x, int y) const { return Map[x][y]; }
  int GetXSize() const { return XSize; }
  int GetYSize() const { return YSize; }
  void SendNewDrawRequest();
  void Initialize(int, int);
  square* GetNeighbourSquare(v2, int) const;
  truth IsValidPos(v2 Pos) const { return Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize; }
  truth IsValidPos(int X, int Y) const { return X >= 0 && Y >= 0 && X < XSize && Y < YSize; }
  const rect& GetBorder() const { return Border; }
  void SetEntryPos(int, v2);
 protected:
  square*** Map;
  uchar** FlagMap;
  int XSize, YSize;
  ulong XSizeTimesYSize;
  rect Border;
  std::map<int, v2> EntryMap;
};

#endif
