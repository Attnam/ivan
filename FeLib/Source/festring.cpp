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

#include <cctype>
#include <cstdlib>
#include <cstring>
#include "festring.h"
#include "allocate.h"
#include "error.h"

#include "dbgmsgproj.h"

char** festring::IntegerMap = 0;
cchar* const festring::EmptyString = "";
festring::csizetype festring::NPos = festring::sizetype(-1);

char Capitalize(char Char)
{
  return (Char >= 'a' && Char <= 'z') ? (Char ^ 0x20) : Char;
}

void bugTrackInvalidChars(cchar* c,int size){
  if(!c || !size)return;

  //symbols and characters that are not "usually" expected
  for(int i=0;i<size;i++){
    static char C;C=c[i];
    if(
        (C < 0x20 || C > 0x7E)
        && C!='\r' && C!='\n' && C!='\t'
        && C!=0
//        && C!='+' // o.O
    ){
      /**
       * after strenghtened festring, this now ends up being more useful to catch wrong usage of the permissive ADD_MESSAGE() passing a festring w/o .CStr()
       */
      ABORT("string contains \"probably invalid\" chars, possibly a bug: \"%s\", [%d], 0x%X /0%o %d '%c'",c, i, C,C,C,C); //actual char '?' must be last as may break the string!
    }
  }
}

/* All operations that may change the underlying data will avoid touching the
   memory allocator and deallocator as much as possible. They try very hard to
   save memory and go for speed. */

festring& festring::operator=(cchar* CStr)
{DBGEXEC(bugTrackInvalidChars(CStr,strlen(CStr)));
  CheckNull(CStr);

  sizetype NewSize = strlen(CStr);

  if(OwnsData)
  {
    if(!REFS(Data) && NewSize <= Reserved)
    {
      memcpy(Data, CStr, NewSize);
      Size = NewSize;
      return *this;
    }

    if(!REFS(Data)--)
      delete [] &REFS(Data);
  }

  if(NewSize)
    CreateOwnData(CStr, NewSize);
  else
    Empty();

  return *this;
}

festring& festring::operator=(staticstring SStr)
{DBGEXEC(bugTrackInvalidChars(SStr.Data,SStr.Size));
  CheckNull(SStr.Data);

  cchar* CStr = SStr.Data;
  sizetype NewSize = SStr.Size;

  if(OwnsData)
  {
    if(!REFS(Data) && NewSize <= Reserved)
    {
      memcpy(Data, CStr, NewSize);
      Size = NewSize;
      return *this;
    }

    if(!REFS(Data)--)
      delete [] &REFS(Data);
  }

  Size = NewSize;
  Data = const_cast<char*>(CStr);
  OwnsData = false;
  return *this;
}

festring& festring::operator=(cfestring& Str)
{DBGEXEC(bugTrackInvalidChars(Str.Data,Str.Size));
  char* CStr = Str.Data;
  sizetype NewSize = Str.Size;

  if(OwnsData)
  {
    if(!REFS(Data) && NewSize <= Reserved)
    {
      memcpy(Data, CStr, NewSize);
      Size = NewSize;
      return *this;
    }

    if(!REFS(Data)--)
      delete [] &REFS(Data);
  }

  Size = NewSize;
  Data = CStr;
  OwnsData = Str.OwnsData;

  if(OwnsData)
  {
    if(REFS(Data) < FESTRING_REF_MAX)
    {
      ++REFS(Data);
      Reserved = Str.Reserved;
    }
    else
      CreateOwnData(CStr, NewSize);
  }

  return *this;
}

/// Capitalize the first letter.
festring& festring::Capitalize()
{
  if(!Size)
    return *this;

  if(Data[0] >= 'a' && Data[0] <= 'z')
  {
    EnsureOwnsData(true);
    Data[0] = ::Capitalize(Data[0]);
  }

  return *this;
}

festring& festring::Append(char Char)
{
  if(OwnsData && !REFS(Data) && Size < Reserved)
    Data[Size++] = Char;
  else
    SlowAppend(&Char, 1);

  return *this;
}

festring& festring::Append(cchar* CStr, sizetype N)
{
  if(!N)
    return *this;

  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;

  if(OwnsData && !REFS(Data) && NewSize <= Reserved)
  {
    memcpy(Data + OldSize, CStr, N);
    Size = NewSize;
  }
  else
    SlowAppend(CStr, N);

  return *this;
}

festring& festring::Append(cfestring& Str)
{
  if(!Str.Size)
    return *this;

  sizetype N = Str.Size;
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;

  if(OwnsData && !REFS(Data) && NewSize <= Reserved)
  {
    memcpy(Data + OldSize, Str.Data, N);
    Size = NewSize;
  }
  else
    SlowAppend(Str.Data, N);

  return *this;
}

void festring::SlowAppend(char Char)
{
  SlowAppend(&Char, 1);
}

/// Always allocate new memory before append data.
void festring::SlowAppend(cchar* CStr, sizetype N)
{
  if(!N)
    return;

  char* OldData = Data;
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;
  decltype(&REFS(Data)) FreePtr = 0;

  if(OwnsData && !REFS(OldData)--)
    FreePtr = &REFS(OldData);

  CreateNewData(NewSize);
  memcpy(Data, OldData, OldSize);
  memcpy(Data + OldSize, CStr, N);
  Size = NewSize;

  if(FreePtr)
    delete [] FreePtr;
}

/// Set the data to a string of the same char.
void festring::Assign(sizetype NewSize, char Char)
{
  if(!NewSize)
  {
    Empty();
    return;
  }

  if(OwnsData)
  {
    if(!REFS(Data) && NewSize <= Reserved)
    {
      memset(Data, Char, NewSize);
      Size = NewSize;
      return;
    }

    if(!REFS(Data)--)
      delete [] &REFS(Data);
  }

  CreateNewData(NewSize);
  memset(Data, Char, NewSize);
  Size = NewSize;
}

/// Resize the string and use a char for padding.
void festring::Resize(sizetype NewSize, char Char)
{
  if(Size > NewSize)
  {
    if(OwnsData)
    {
      if(!REFS(Data))
      {
        Size = NewSize;
        return;
      }

      --REFS(Data);
    }

    CreateOwnData(Data, NewSize);
    return;
  }
  else if(Size < NewSize)
  {
    char* OldData = Data;
    sizetype OldSize = Size;
    decltype(&REFS(Data)) FreePtr = 0;

    if(OwnsData)
    {
      if(!REFS(OldData))
      {
        if(NewSize <= Reserved)
        {
          memset(OldData + OldSize, Char, NewSize - OldSize);
          Size = NewSize;
          return;
        }
        else
          FreePtr = &REFS(OldData);
      }
      else
        --REFS(OldData);
    }

    CreateNewData(NewSize);
    memcpy(Data, OldData, OldSize);
    memset(Data + OldSize, Char, NewSize - OldSize);
    Size = NewSize;

    if(FreePtr)
      delete [] FreePtr;
  }
  else
    return;
}

/// Shrink the length of every string of spaces to one. Useful for item search.
void festring::ShrinkWhitespace()
{
  if(Find("  ") == NPos)
    return;

  EnsureOwnsData(true);

  truth KeepSpace = Size > 0 && Data[Size-1] == ' ';
  sizetype slow = 0, fast = 2;

  while(fast < Size)
  {
    if(Data[slow] != ' ' || Data[slow+1] != ' ')
    {
      ++slow;
      ++fast;
    }
    else if(Data[fast] != ' ')
    {
      while(fast < Size && Data[slow+1] == ' ' && Data[fast] != ' ')
      {
        Data[++slow] = Data[fast++];
        Data[1+slow] = ' ';
      }
      Data[2+slow] = ' ';
    }
    else
      ++fast;
  }
  while(slow < Size && Data[slow] != ' ')
    ++slow;

  Size = slow + KeepSpace;
  Data[Size] = 0;
}

/// Find a char not before the specified position.
festring::sizetype festring::Find(char Char, sizetype Pos) const
{
  if(Size <= Pos)
    return NPos;

  char* Result = static_cast<char*>(memchr(Data + Pos, Char, Size - Pos));

  return Result ? (Result - Data) : NPos;
}

/// Find a string not before the specified position.
festring::sizetype festring::Find(cchar* CStr,
                                  sizetype Pos,
                                  sizetype N) const
{
  if(Size <= Pos)
    return NPos;

  if(!N)
    return Pos;

  for(sizetype i = Pos; i + N <= Size; ++i)
    if(Data[i] == CStr[0] && !strncmp(Data + i, CStr, N))
      return i;

  return NPos;
}

/// Find the last occurrence of a char not after the specified position.
festring::sizetype festring::FindLast(char Char, sizetype Pos) const
{
  if(Pos >= Size)
    Pos = Size - 1;  // sizetype(-1) == NPos

  while(Pos != NPos && Data[Pos] != Char)
    --Pos;

  return Pos;
}

/// Find the last occurrence of a string not after the specified position.
festring::sizetype festring::FindLast(cchar* CStr,
                                      sizetype Pos,
                                      sizetype N) const
{
  if(Size < N)
    return NPos;

  if(Pos > Size - N)
    Pos = Size - N;

  if(!N)
    return Pos;

  for(sizetype i = Pos; i != NPos; --i)
    if(Data[i] == CStr[0] && !strncmp(Data + i, CStr, N))
      return i;

  return NPos;
}

/// Erase the substring starting from the specified position.
void festring::Erase(sizetype Pos, sizetype Length)
{
  if(!Length || Size <= Pos)
    return;

  if(Pos == 0 && Pos + Length >= Size)
  {
    Empty();
    return;
  }

  char* OldData = Data;
  sizetype OldSize = Size;
  sizetype NewSize = (Pos + Length < Size) ? (Size - Length) : Pos;
  sizetype EraseEnd = (Pos + Length < Size) ? (Pos + Length) : OldSize;

  if(OwnsData)
  {
    if(!REFS(OldData))
    {
      memmove(OldData + Pos, OldData + EraseEnd, OldSize - EraseEnd);
      Size = NewSize;
      return;
    }
    else
      --REFS(OldData);
  }

  CreateNewData(NewSize);
  memcpy(Data, OldData, Pos);
  memcpy(Data + Pos, OldData + EraseEnd, OldSize - EraseEnd);
  Size = NewSize;
}

/// Insert a string at the specified position.
void festring::Insert(sizetype Pos, cchar* CStr, sizetype N)
{
  if(!N)
    return;

  if(Size < Pos)
    ABORT("Illegal festring insertion detected!");

  if(Size == Pos)
  {
    Append(CStr, N);
    return;
  }

  char* OldData = Data;
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;
  decltype(&REFS(Data)) FreePtr = 0;

  if(OwnsData)
  {
    if(!REFS(OldData))
    {
      if(NewSize <= Reserved)
      {
        memmove(OldData + Pos + N, OldData + Pos, OldSize - Pos);
        memcpy(OldData + Pos, CStr, N);
        Size = NewSize;
        return;
      }
      else
        FreePtr = &REFS(OldData);
    }
    else
      --REFS(OldData);
  }

  CreateNewData(NewSize);
  memcpy(Data, OldData, Pos);
  memcpy(Data + Pos, CStr, N);
  memcpy(Data + Pos + N, OldData + Pos, OldSize - Pos);
  Size = NewSize;

  if(FreePtr)
    delete [] FreePtr;
}

/* Creates map of char representations of numbers 0-999 used by
   festring::Append(long). Due to automatization, you don't need
   to explicitly call it. */

void festring::InstallIntegerMap()
{
  Alloc2D(IntegerMap, 1000, 3);
  char Ones = '0', Tens = '0', Hundreds = '0';

  for(int c = 0; c < 1000; ++c)
  {
    IntegerMap[c][0] = Hundreds;
    IntegerMap[c][1] = Tens;
    IntegerMap[c][2] = Ones;

    if(++Ones > '9')
    {
      Ones = '0';

      if(++Tens > '9')
      {
        Tens = '0';
        ++Hundreds;
      }
    }
  }

  atexit(DeInstallIntegerMap);
}

/* Deletes the integer map used by festring::Append(long).
   Due to automatization, you don't need to explicitly call it. */

void festring::DeInstallIntegerMap()
{
  delete [] IntegerMap;
  IntegerMap = 0;
}

void festring::CheckNull(cchar* Ptr)
{
  if(!Ptr)
    ABORT("Null pointer detected!");
}

/* Displays numbers in the range [-2147483647, 2147483647].
   Much faster than sprintf and (nonstandard) itoa. */
#include <cstdio>
festring& festring::AppendInt(long Integer)
{
  if(!IntegerMap)
    InstallIntegerMap();
  char IntegerBuffer[12];
//   snprintf(IntegerBuffer, 12, "%l", &Integer);
//   return Append(IntegerBuffer, strlen(IntegerBuffer));
  char* BufferPtr = IntegerBuffer;
  truth Negative = false;

  if(Integer < 0)
  {
    if(Integer < -2147483647)
      return Append("-Inf", 4);

    *BufferPtr++ = '0';
    Integer = -Integer;
    Negative = true;
  }
  else if(Integer > 2147483647)
    return Append("Inf", 3);

  truth ForceZeros = false;

  if(Integer >= 2000000000)
  {
    *BufferPtr++ = '2';
    Integer -= 2000000000;
    ForceZeros = true;
  }
  else if(Integer >= 1000000000)
  {
    *BufferPtr++ = '1';
    Integer -= 1000000000;
    ForceZeros = true;
  }

  if(ForceZeros || Integer >= 1000000)
  {
    int Temp = Integer / 1000000;
    *BufferPtr++ = IntegerMap[Temp][0];
    *BufferPtr++ = IntegerMap[Temp][1];
    *BufferPtr++ = IntegerMap[Temp][2];
    Integer -= Temp * 1000000;
    ForceZeros = true;
  }

  if(ForceZeros || Integer >= 1000)
  {
    int Temp = Integer / 1000;
    *BufferPtr++ = IntegerMap[Temp][0];
    *BufferPtr++ = IntegerMap[Temp][1];
    *BufferPtr++ = IntegerMap[Temp][2];
    Integer -= Temp * 1000;
  }

  *BufferPtr++ = IntegerMap[Integer][0];
  *BufferPtr++ = IntegerMap[Integer][1];
  *BufferPtr++ = IntegerMap[Integer][2];
  *BufferPtr = 0;

  for(BufferPtr = IntegerBuffer; *BufferPtr == '0'; ++BufferPtr);

  if(Negative)
    *--BufferPtr = '-';
  else if(!*BufferPtr) // check if the original Integer was zero
    --BufferPtr;

  return Append(BufferPtr, strlen(BufferPtr));
}

/* The Result string receives up to Length characters from source,
   but words are left uncut if possible. */

void festring::SplitString(festring& Source,
                           festring& Result,
                           sizetype Length)
{
  if(Source.GetSize() <= Length)
  {
    Result << Source;
    Source.Empty();
    return;
  }

  sizetype Pos = Source.FindLast(' ', Length);

  if(Pos != NPos)
  {
    Result.Append(Source, Pos);
    Source.Erase(0, Pos + 1);
  }
  else
  {
    Result.Append(Source, Length);
    Source.Erase(0, Length);
  }
}

/* Divides Source into lines of size up to Length without cutting words
   and stores them one by one to StringVector. You can also specify a
   Marginal, in which case a number of spaces is inserted in the
   beginning of each line except the first. It returns the number of
   created lines. */

int festring::SplitString(cfestring& Source,
                          std::vector<festring>& StringVector,
                          sizetype Length, sizetype Marginal)
{
  if(!Length)
    ABORT("Illegal Length 0 passed to festring::SplitString()!");

  if(Marginal >= Length)
    ABORT("Illegal festring::SplitString() call:"
          "Marginal must be less than Length!");

  festring CopyOfSource(Source);

  if(StringVector.empty())
    StringVector.push_back(festring());
  else
    StringVector[0].Empty();

  SplitString(CopyOfSource, StringVector[0], Length);
  sizetype Result = 1;

  while(!CopyOfSource.IsEmpty())
  {
    if(StringVector.size() <= Result)
      StringVector.push_back(festring());

    festring& String = StringVector[Result++];
    String.Assign(Marginal, ' ');
    SplitString(CopyOfSource, String, Length - Marginal);
  }

  return Result;
}

/* Returns the position of the first occurance of What in Where
   starting at Begin or after it, ignoring the case of letters.
   If the search fails, festring::NPos is returned instead. */

festring::sizetype festring::IgnoreCaseFind(cfestring& Where,
                                            cfestring& What,
                                            sizetype Begin)
{
  if(What.IsEmpty())
    return Begin;

  for(sizetype i = Begin; Where.GetSize() >= What.GetSize() + i; ++i)
    if(::Capitalize(Where[i]) == ::Capitalize(What[0]))
    {
      truth Equal = true;

      for(sizetype j = 1; j < What.GetSize(); ++j)
        if(::Capitalize(Where[i + j]) != ::Capitalize(What[j]))
        {
          Equal = false;
          break;
        }

      if(Equal)
        return i;
    }

  return NPos;
}

/* Replaces all occurances of What in Where after Begin with With */

void festring::SearchAndReplace(festring& Where, cfestring& What,
                                cfestring& With, sizetype Begin)
{
  if(What.IsEmpty())
    ABORT("Infinite loops in SearchAndReplace detected!");

  for(sizetype Pos = Where.Find(What, Begin);
      Pos != NPos; Pos = Where.Find(What, Pos))
  {
    Where.Erase(Pos, What.GetSize());
    Where.Insert(Pos, With);
    Pos += With.GetSize();
  }
}

/* Returns whether First is behind Second in alphabetical order,
   ignoring case */

bool festring::IgnoreCaseCompare(cfestring& First,
                                 cfestring& Second)
{
  for(sizetype Pos = 0; Pos < First.GetSize() && Pos < Second.GetSize(); ++Pos)
  {
    char Char1 = ::Capitalize(First[Pos]);
    char Char2 = ::Capitalize(Second[Pos]);

    if(Char1 != Char2)
      return Char1 < Char2;
  }

  return First.GetSize() < Second.GetSize();
}

/* Sorry for ugliness */

void festring::PreProcessForFebot()
{
  sizetype c, d, Length;

  for(c = 0, Length = 0;
      c < Size && (Data[c] == ' ' || Data[c] == '\t'); ++c)
    ++Length;

  Erase(0, Length);

  if(!Size)
    return;

  for(c = Size - 1, Length = 0;
      Data[c] == ' ' || Data[c] == '\t';
      --c)
    ++Length;

  Erase(c + 1, Length);

  for(c = 0; c < Size - 1; ++c)
  {
    char Char = Data[c + 1];

    if(Data[c] == '\t')
      (*this)[c] = ' ';  // Use (*this)[c] to modify Data[c]!
    else if(Data[c] == '\"' || Data[c] == '(' || Data[c] == ')')
    {
      Erase(c--, 1);
      continue;
    }

    if(Data[c] == ' ')
    {
      if(Char == ' ' || Char == '\t')
      {
        for(d = c + 2, Length = 1;
            d < Size && (Data[d] == ' ' || Data[d] == '\t'); ++d)
          ++Length;

        Erase(c + 1, Length);
      }
    }
    else if((Char == '.' || Char == '!' || Char == '?')
            && Data[c] != '.' && Data[c] != '!' && Data[c] != '?'
            && (c == Size - 2 || Data[c + 2] == ' ' || Data[c + 2] == '\t'))
      Insert(c+++1, " ", 1);
  }

  if(Data[c] == '\"' || Data[c] == '(' || Data[c] == ')')
    Erase(c--, 1);

  if(!ispunct(Data[c]))
    *this << ' ' << '.';
}

/* Sorry for ugliness */

void festring::PostProcessForFebot()
{
  Capitalize();
  truth CapitalizeNextChar = false;

  for(sizetype c = 0; c < Size - 1; ++c)
  {
    char Char1 = Data[c];

    if(Char1 == ' ')
    {
      char Char2 = Data[c + 1];

      if((Char2 == '.' || Char2 == '!' || Char2 == '?')
         && (c == Size - 2 || Data[c + 2] == ' ' || Data[c + 2] == '\t'))
      {
        Erase(c++, 1);
        CapitalizeNextChar = true;
      }
    }
    else if((Char1 == '.' || Char1 == '!' || Char1 == '?')
            && (c == Size - 2 || Data[c + 2] == ' ' || Data[c + 2] == '\t'))
      CapitalizeNextChar = true;
    else if(CapitalizeNextChar)
    {
      if(Char1 > 0x60 && Char1 < 0x7B)
        (*this)[c] &= ~0x20;  // Use (*this)[c] to modify Data[c]!

      CapitalizeNextChar = false;
    }
  }
}

/* Erases the first word of the sentence and places it to To.
   Should currently be used only by strings processed for Febot */

void festring::ExtractWord(festring& To)
{
  for(sizetype c = 0; c < Size; ++c)
    if(Data[c] == ' ')
    {
      To.Empty();
      To.Append(&Data[c + 1], Size - c - 1);
      Erase(c, Size - c);
      SwapData(To);
      return;
    }

  To = *this;
  Empty();
}

void festring::SwapData(festring& Str)
{
  char*const TData = Data;
  csizetype TSize = Size;
  csizetype TReserved = Reserved;
  ctruth TOwnsData = OwnsData;
  Data = Str.Data;
  Size = Str.Size;
  Reserved = Str.Reserved;
  OwnsData = Str.OwnsData;
  Str.Data = TData;
  Str.Size = TSize;
  Str.Reserved = TReserved;
  Str.OwnsData = TOwnsData;
}

long festring::GetCheckSum() const
{
  return Size > 0 ? Data[Size - 1] : 0;
}

void festring::CreateNewData(sizetype N)
{
  Reserved = N|FESTRING_PAGE;
  Data = sizeof(int*) + new char[Reserved + sizeof(int*) + 1];
  OwnsData = true;
  REFS(Data) = 0;
  Size = 0;
}

void festring::CreateOwnData(cchar* CStr, sizetype N)
{
  CreateNewData(N);
  memcpy(Data, CStr, N);
  Size = N;
}

void festring::EnsureOwnsData(bool Unique)
{
  if(!OwnsData)
    CreateOwnData(Data, Size);
  else if(Unique && REFS(Data))
  {
    --REFS(Data);
    CreateOwnData(Data, Size);
  }
}
