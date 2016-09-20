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

#ifndef __FEARRAY_H__
#define __FEARRAY_H__

#include "femath.h"

template <class type> struct fearray
{
  typedef uint sizetype;
  fearray() : Data(0), Size(0) { }
  fearray(const fearray&);
  fearray(const type*, sizetype);
  ~fearray();
  fearray& operator=(const fearray&);
  type& operator[](sizetype I) { return Data[I]; }
  const type& operator[](sizetype I) const { return Data[I]; }
  void Allocate(sizetype);
  void Add(const type&);
  void Clear();
  const type& GetRandomElement() const { return Data[RAND_N(Size)]; }
  type* Data;
  sizetype Size;
};

template <class type>
inline fearray<type>::fearray(const fearray<type>& A)
: Data(A.Data), Size(A.Size)
{
  if(Data)
    ++REFS(Data);
}

template <class type>
inline fearray<type>::fearray(const type* Array, sizetype Size)
: Size(Size)
{
  char* Ptr = new char[Size * sizeof(type) + sizeof(int*)];
  *reinterpret_cast<ulong*>(Ptr) = 0;
  Data = reinterpret_cast<type*>(Ptr + sizeof(int*));

  for(sizetype c = 0; c < Size; ++c)
    new(&Data[c]) type(Array[c]);
}

template <class type> inline fearray<type>::~fearray<type>()
{
  type* Ptr = Data;

  if(Ptr && !REFS(Ptr)--)
  {
    type* TempPtr = Ptr, * EndPtr = Ptr + Size;

    for(; TempPtr != EndPtr; ++TempPtr)
      TempPtr->~type();

    delete [] &REFS(Ptr);
  }
}

template <class type>
inline fearray<type>& fearray<type>::operator=(const fearray<type>& A)
{
  if(A.Data)
    ++REFS(Data = A.Data);

  Size = A.Size;
  return *this;
}

template <class type> inline void fearray<type>::Clear()
{
  type* Ptr = Data;

  if(Ptr)
  {
    if(!REFS(Ptr)--)
    {
      for(sizetype c = 0; c < Size; ++c)
        Ptr[c].~type();

      delete [] &REFS(Ptr);
    }

    Data = 0;
    Size = 0;
  }
}

template <class type> inline void fearray<type>::Allocate(sizetype What)
{
  char* Ptr = new char[What * sizeof(type) + sizeof(ulong)];
  *reinterpret_cast<ulong*>(Ptr) = 0;
  Data = reinterpret_cast<type*>(Ptr + sizeof(ulong));
  Size = What;

  for(sizetype c = 0; c < What; ++c)
    new(&Data[c]) type;
}

/* Don't use unless necessary */

template <class type> inline void fearray<type>::Add(const type& Type)
{
  type* Ptr = Data;

  if(Ptr)
  {
    sizetype Size = this->Size++;
    char* NewPtr = new char[(Size + 1) * sizeof(type) + sizeof(ulong)];
    *reinterpret_cast<ulong*>(NewPtr) = 0;
    type* NewData = reinterpret_cast<type*>(NewPtr + sizeof(ulong));

    if(!REFS(Ptr)--)
    {
      for(sizetype c = 0; c < Size; ++c)
      {
        new(&NewData[c]) type(Ptr[c]);
        Ptr[c].~type();
      }

      delete [] &REFS(Ptr);
    }
    else
      for(sizetype c = 0; c < Size; ++c)
        new(&NewData[c]) type(Ptr[c]);

    Data = NewData;
    new(&NewData[Size]) type(Type);
  }
  else
  {
    char* NewPtr = new char[sizeof(type) + sizeof(ulong)];
    *reinterpret_cast<ulong*>(NewPtr) = 0;
    Data = reinterpret_cast<type*>(NewPtr + sizeof(ulong));
    Size = 1;
    new(Data) type(Type);
  }
}

template <class type1, class type2>
inline void ArrayToVector(const fearray<type1>& Array,
                          std::vector<type2>& Vect)
{
  Vect.resize(Array.Size, type2());

  for(typename fearray<type1>::sizetype c = 0; c < Array.Size; ++c)
    Vect[c] = Array.Data[c];
}

#endif
