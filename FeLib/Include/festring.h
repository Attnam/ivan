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

#ifndef __FESTRING_H__
#define __FESTRING_H__

#include <climits>
#include <vector>

#include "felibdef.h"

#define FESTRING_PAGE 0x7F
#define FESTRING_REF_MAX ULONG_MAX

class festring
{
 public:
  typedef ulong sizetype;
  typedef const ulong csizetype;

  class staticstring
  {
    friend class festring;

    public:
      template<sizetype N>
      constexpr staticstring(cchar (&s)[N]): Data(s), Size(strlen(s)) {}
    private:
      cchar* const Data;
      sizetype const Size;
  };

  /* It can be proven that the code works even if OwnsData is left
     uninitialized. However, Valgrind reports this as a possible error
     which is annoying */
  festring()
  : Data(const_cast<char*>(EmptyString)), Size(0), OwnsData(false) { }
  festring(cfestring&);
  festring(sizetype, char);
  festring(cchar* CStr) : festring(CStr, strlen(CStr)) { }
  festring(cchar* CStr, sizetype N) : festring()
  { CheckNull(CStr); if(N) CreateOwnData(CStr, N); }
  festring(staticstring SStr)
  : Data(const_cast<char*>(SStr.Data)), Size(SStr.Size), OwnsData(false)
  { CheckNull(Data); }
  ~festring();
  festring& Capitalize();
  festring CapitalizeCopy() const { return festring(*this).Capitalize(); }
  char operator[](sizetype N) const { return Data[N]; }
  char& operator[](sizetype N) { EnsureOwnsData(true); return Data[N]; }
  festring& operator=(cchar*);
  festring& operator=(staticstring);
  festring& operator=(cfestring&);
  festring& operator<<(char);
  festring& operator<<(cchar*);
  festring& operator<<(cfestring&);
  festring& operator<<(short Int) { return AppendInt(Int); }
  festring& operator<<(ushort Int) { return AppendInt(Int); }
  festring& operator<<(int Int) { return AppendInt(Int); }
  festring& operator<<(uint Int) { return AppendInt(Int); }
  festring& operator<<(long Int) { return AppendInt(Int); }
  festring& operator<<(ulong Int) { return AppendInt(Int); }
  bool operator<(cfestring&) const;
  truth operator==(cfestring&) const;
  truth operator!=(cfestring&) const;
  truth operator==(cchar*) const;
  truth operator!=(cchar*) const;
  int Compare(cfestring&) const;
  cchar* CStr() const;
  sizetype GetSize() const { return Size; }
  void Empty();
  void Assign(sizetype, char);
  void Resize(sizetype, char = ' ');
  sizetype Find(char, sizetype = 0) const;
  sizetype Find(cchar* CStr, sizetype Pos = 0) const
  { return Find(CStr, Pos, strlen(CStr)); }
  sizetype Find(cchar*, sizetype, sizetype) const;
  sizetype Find(cfestring& S, sizetype Pos = 0) const
  { return Find(S.Data, Pos, S.Size); }
  sizetype FindLast(char, sizetype = NPos) const;
  sizetype FindLast(cchar* CStr, sizetype Pos = NPos) const
  { return FindLast(CStr, Pos, strlen(CStr)); }
  sizetype FindLast(cchar*, sizetype, sizetype) const;
  sizetype FindLast(cfestring& S, sizetype Pos = NPos) const
  { return FindLast(S.Data, Pos, S.Size); }
  void Erase(sizetype, sizetype);
  void Insert(sizetype Pos, char Char)
  { Insert(Pos, &Char, 1); }
  void Insert(sizetype Pos, cchar* CStr)
  { Insert(Pos, CStr, strlen(CStr)); }
  void Insert(sizetype, cchar*, sizetype);
  void Insert(sizetype Pos, cfestring& S)
  { Insert(Pos, S.Data, S.Size); }
  festring& Append(cfestring& Str, sizetype N)
  { return Append(Str.Data, N); }
  static csizetype NPos;
  static void SplitString(festring&, festring&, sizetype);
  static int SplitString(cfestring&, std::vector<festring>&,
                         sizetype, sizetype = 0);
  static sizetype IgnoreCaseFind(cfestring&,
                                 cfestring&, sizetype = 0);
  static void SearchAndReplace(festring&, cfestring&,
                               cfestring&, sizetype = 0);
  static bool IgnoreCaseCompare(cfestring&, cfestring&);
  truth IsEmpty() const { return !Size; }
  void PreProcessForFebot();
  void PostProcessForFebot();
  void SwapData(festring&);
  void ExtractWord(festring&);
  long GetCheckSum() const;
  void EnsureOwnsData(bool = false);
 private:
  static void InstallIntegerMap();
  static void DeInstallIntegerMap();
  static void CheckNull(cchar*);
  void CreateNewData(sizetype);
  void CreateOwnData(cchar*, sizetype);
  festring& AppendInt(long);
  festring& Append(char);
  festring& Append(cchar*, sizetype);
  festring& Append(cfestring&);
  void SlowAppend(char);
  void SlowAppend(cchar*, sizetype);
  static char** IntegerMap;
  static cchar* const EmptyString;
  char* Data;
  sizetype Size;
  sizetype OwnsData : 1;
  sizetype Reserved : 31;
};

class festringpile
{
 public:
  festringpile(cfestring& String) : String(String) { }
  template <class type>
  festringpile& operator+(type What) { String << What; return *this; }
  festringpile& operator+(cfestring& What)
  { String << What; return *this; }
  festringpile& operator+(const festringpile& What)
  { String << What.String; return *this; }
  operator festring() const { return String; }
 private:
  festring String;
};

template <class type>
inline festringpile operator+(cfestring& S, type What)
{ return festringpile(S) + What; }
inline festringpile operator+(cfestring& S, cfestring& What)
{ return festringpile(S) + What; }
inline festringpile operator+(cfestring& S, const festringpile& What)
{ return festringpile(S) + What; }

inline festring::festring(cfestring& Str)
: Data(Str.Data), Size(Str.Size),
  OwnsData(Str.OwnsData), Reserved(Str.Reserved)
{
  if(OwnsData)
  {
    if(REFS(Data) < FESTRING_REF_MAX)
      ++REFS(Data);
    else
      CreateOwnData(Str.Data, Str.Size);
  }
}

inline festring::festring(sizetype N, char Char)
{
  CreateNewData(N);
  memset(Data, Char, N);
  Size = N;
}

inline festring::~festring()
{
  if(OwnsData)
    if(!REFS(Data)--)
      delete [] &REFS(Data);
}

inline bool festring::operator<(cfestring& Str) const
{
  sizetype ThisSize = Size;
  sizetype StrSize = Str.Size;

  if(ThisSize && StrSize)
  {
    int Comp = memcmp(Data, Str.Data,
                      StrSize > ThisSize ? ThisSize : StrSize);
    return Comp < 0 || (!Comp && StrSize > ThisSize);
  }
  else
    return !ThisSize && StrSize;
}

inline truth festring::operator==(cfestring& Str) const
{
  sizetype StrSize = Str.Size;
  return Size == StrSize && (!StrSize || !memcmp(Data, Str.Data, StrSize));
}

inline truth festring::operator!=(cfestring& Str) const
{
  sizetype StrSize = Str.Size;
  return Size != StrSize || (StrSize && memcmp(Data, Str.Data, StrSize));
}

inline truth festring::operator==(cchar* CStr) const
{
  sizetype StrSize = strlen(CStr);
  return Size == StrSize && (!StrSize || !memcmp(Data, CStr, StrSize));
}

inline truth festring::operator!=(cchar* CStr) const
{
  sizetype StrSize = strlen(CStr);
  return Size != StrSize || (StrSize && memcmp(Data, CStr, StrSize));
}

/* Returns -1 if this is before Str in alphabetical order, zero
   if strings are identical, else 1 */

inline int festring::Compare(cfestring& Str) const
{
  sizetype ThisSize = Size;
  sizetype StrSize = Str.Size;

  if(ThisSize && StrSize)
  {
    int Comp = memcmp(Data, Str.Data,
                      StrSize > ThisSize ? ThisSize : StrSize);

    if(Comp)
      return Comp;
  }

  return ThisSize < StrSize ? -1 : ThisSize != StrSize;
}

inline cchar* festring::CStr() const
{
  if(OwnsData)
    Data[Size] = 0;

  return Data;
}

inline void festring::Empty()
{
  if(OwnsData)
  {
    if(REFS(Data))
    {
      --REFS(Data);
      Data = const_cast<char*>(EmptyString);
      OwnsData = false;
    }
  }
  else
  {
    Data = const_cast<char*>(EmptyString);
    OwnsData = false;
  }

  Size = 0;
}

inline festring& festring::operator<<(char Char)
{
  return Append(Char);
}

inline festring& festring::operator<<(cchar* CStr)
{
  return Append(CStr, strlen(CStr));
}

inline festring& festring::operator<<(cfestring& Str)
{
  return Append(Str);
}

struct charcomparer
{
  bool operator()(cchar* const& S1, cchar* const& S2) const
  { return strcmp(S1, S2) < 0; }
};

struct ignorecaseorderer
{
  bool operator()(cfestring& S1, cfestring& S2) const
  { return festring::IgnoreCaseCompare(S1, S2); }
};

#define CONST_S(str) festring::staticstring(str)

/*
 * This macro doesn't evaluate with if what
 * is not found so it's often faster
 */

#define SEARCH_N_REPLACE(where, what, with)\
if(where.Find(what) != festring::NPos)\
  festring::SearchAndReplace(where, what, with);

#endif
