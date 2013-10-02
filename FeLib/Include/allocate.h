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

#ifndef __ALLOCATE_H__
#define __ALLOCATE_H__

#include "typedef.h"

template <class type>
inline void Alloc2D(type**& Map, int XSize, int YSize)
{
  cint Size = XSize * (sizeof(type*) + YSize * sizeof(type));
  Map = reinterpret_cast<type**>(new char[Size]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
    Map[x] = XPointer;
}

template <class type>
inline void Alloc2D(type**& Map, int XSize, int YSize,
		    const type& Initializer)
{
  cint Size = XSize * (sizeof(type*) + YSize * sizeof(type));
  Map = reinterpret_cast<type**>(new char[Size]);
  type* XPointer = reinterpret_cast<type*>(Map + XSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
  {
    Map[x] = XPointer;

    for(int y = 0; y < YSize; ++y)
      Map[x][y] = Initializer;
  }
}

template <class type>
inline void Alloc3D(type***& Map, int XSize, int YSize, int ZSize)
{
  cint Size = XSize * (sizeof(type**)
		       + YSize * (sizeof(type*)
				  + ZSize * sizeof(type)));
  Map = reinterpret_cast<type***>(new char[Size]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
  {
    Map[x] = XPointer;

    for(int y = 0; y < YSize; ++y, YPointer += ZSize)
      Map[x][y] = YPointer;
  }
}

template <class type>
inline void Alloc3D(type***& Map, int XSize, int YSize,
		    int ZSize, const type& Initializer)
{
  cint Size = XSize * (sizeof(type**)
		       + YSize * (sizeof(type*)
				  + ZSize * sizeof(type)));
  Map = reinterpret_cast<type***>(new char[Size]);
  type** XPointer = reinterpret_cast<type**>(Map + XSize);
  type* YPointer = reinterpret_cast<type*>(XPointer + XSize * YSize);

  for(int x = 0; x < XSize; ++x, XPointer += YSize)
  {
    Map[x] = XPointer;

    for(int y = 0; y < YSize; ++y, YPointer += ZSize)
    {
      Map[x][y] = YPointer;

      for(int z = 0; z < ZSize; ++z)
	Map[x][y][z] = Initializer;
    }
  }
}

#endif
