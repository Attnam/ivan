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

#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

#include <vector>

#include "area.h"

class wsquare;
class continent;

typedef std::vector<character*> charactervector;

class worldmap : public area
{
 public:
  worldmap(int, int);
  worldmap();
  virtual ~worldmap();
  void Generate();
  void Save(outputfile&) const;
  void Load(inputfile&);
  wsquare* GetWSquare(v2 Pos) const { return Map[Pos.X][Pos.Y]; }
  wsquare* GetWSquare(int x, int y) const { return Map[x][y]; }
  void GenerateClimate();
  int WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(int, int);
  void CalculateContinents();
  void SmoothAltitude();
  void SmoothClimate();
  void RandomizeAltitude();
  continent* GetContinentUnder(v2) const;
  continent* GetContinent(int) const;
  void RemoveEmptyContinents();
  int GetAltitude(v2);
  charactervector& GetPlayerGroup();
  character* GetPlayerGroupMember(int);
  virtual void Draw(truth) const;
  void CalculateLuminances();
  void CalculateNeighbourBitmapPoses();
  wsquare* GetNeighbourWSquare(v2, int) const;
  v2 GetEntryPos(ccharacter*, int) const;
  void RevealEnvironment(v2, int);
  void SafeSmooth(int, int);
  void FastSmooth(int, int);
  wsquare*** GetMap() const { return Map; }
  void UpdateLOS();
 protected:
  wsquare*** Map;
  std::vector<continent*> Continent;
  uchar** TypeBuffer;
  uchar** OldTypeBuffer;
  short** AltitudeBuffer;
  short** OldAltitudeBuffer;
  uchar** ContinentBuffer;
  charactervector PlayerGroup;
};

outputfile& operator<<(outputfile&, const worldmap*);
inputfile& operator>>(inputfile&, worldmap*&);

#endif
