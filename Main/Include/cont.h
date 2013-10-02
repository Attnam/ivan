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

#ifndef __CONT_H__
#define __CONT_H__

#include <vector>

#include "v2.h"
#include "festring.h"

class outputfile;
class inputfile;

class continent
{
 public:
  friend class worldmap;
  continent();
  continent(int);
  void AttachTo(continent*);
  void Add(v2);
  void Save(outputfile&) const;
  void Load(inputfile&);
  long GetSize() const;
  int GetIndex() const { return Index; }
  void GenerateInfo();
  festring GetName() const { return Name; }
  int GetGTerrainAmount(int) const;
  v2 GetRandomMember(int);
  v2 GetMember(int) const;
 private:
  static uchar** TypeBuffer;
  static short** AltitudeBuffer;
  static uchar** ContinentBuffer;
  festring Name;
  std::vector<v2> Member;
  std::vector<long> GTerrainAmount;
  int Index;
};

outputfile& operator<<(outputfile&, const continent*);
inputfile& operator>>(inputfile&, continent*&);

#endif
