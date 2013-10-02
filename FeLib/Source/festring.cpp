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
#include "festring.h"
#include "allocate.h"
#include "error.h"

char** festring::IntegerMap = 0;
char* festring::EmptyString = "";
festring::csizetype festring::NPos = festring::sizetype(-1);

festring& festring::Append(cchar* CStr, sizetype N)
{
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;
  char* OldPtr = Data;

  if(OwnsData && OldPtr && !REFS(OldPtr) && NewSize <= Reserved)
  {
    memcpy(OldPtr + OldSize, CStr, N);
    Size = NewSize;
  }
  else
    SlowAppend(CStr, N);

  return *this;
}

festring& festring::operator=(cchar* CStr)
{
  sizetype NewSize = strlen(CStr);
  Size = NewSize;
  char* Ptr = Data;

  if(Ptr && OwnsData)
  {
    if(!REFS(Ptr) && NewSize <= Reserved)
    {
      memcpy(Ptr, CStr, NewSize);
      return *this;
    }

    if(!REFS(Ptr)--)
      delete [] &REFS(Ptr);
  }

  Data = const_cast<char*>(CStr);
  OwnsData = false;
  return *this;
}

festring& festring::operator=(cfestring& Str)
{
  sizetype NewSize = Str.Size;
  Size = NewSize;
  char* Ptr = Data;
  char* StrPtr = Str.Data;

  if(Ptr && OwnsData)
  {
    if(!REFS(Ptr) && NewSize <= Reserved)
    {
      if(StrPtr)
	memcpy(Ptr, StrPtr, NewSize);

      return *this;
    }

    if(!REFS(Ptr)--)
      delete [] &REFS(Ptr);
  }

  if((Data = StrPtr) && (OwnsData = Str.OwnsData))
  {
    ++REFS(StrPtr);
    Reserved = Str.Reserved;
  }

  return *this;
}

/* Size must be > 0 */

festring& festring::Capitalize()
{
  char* OldPtr = Data;

  if(*OldPtr > 0x60 && *OldPtr < 0x7B)
  {
    if(!OwnsData)
      CreateOwnData(OldPtr, Size);
    else if(REFS(OldPtr))
    {
      --REFS(OldPtr);
      CreateOwnData(OldPtr, Size);
    }

    *Data ^= 0x20;
  }

  return *this;
}

void festring::CreateOwnData(cchar* CStr, sizetype N)
{
  Size = N;
  Reserved = N|FESTRING_PAGE;
  char* Ptr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
  REFS(Ptr) = 0;
  Data = Ptr;
  memcpy(Ptr, CStr, N);
  OwnsData = true;
}

void festring::SlowAppend(char Char)
{
  char* OldPtr = Data;

  if(OldPtr)
  {
    sizetype OldSize = Size++;
    sizetype NewSize = OldSize + 1;
    ulong* DeletePtr = 0;

    if(OwnsData && !REFS(OldPtr)--)
      DeletePtr = &REFS(OldPtr);

    Reserved = NewSize|FESTRING_PAGE;
    char* NewPtr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
    REFS(NewPtr) = 0;
    Data = NewPtr;
    memcpy(NewPtr, OldPtr, OldSize);
    NewPtr[OldSize] = Char;

    if(DeletePtr)
      delete [] DeletePtr;
  }
  else
  {
    Size = 1;
    Reserved = FESTRING_PAGE;
    char* Ptr = sizeof(int*) + new char[FESTRING_PAGE + sizeof(int*)+1];
    REFS(Ptr) = 0;
    Ptr[0] = Char;
    Data = Ptr;
  }

  OwnsData = true;
}

void festring::SlowAppend(cchar* CStr, sizetype N)
{
  char* OldPtr = Data;

  if(OldPtr)
  {
    sizetype OldSize = Size;
    sizetype NewSize = OldSize + N;
    Size = NewSize;
    ulong* DeletePtr = 0;

    if(OwnsData && !REFS(OldPtr)--)
      DeletePtr = &REFS(OldPtr);

    Reserved = NewSize|FESTRING_PAGE;
    char* NewPtr = sizeof(int*) + new char[Reserved +  sizeof(int*)+1];
    REFS(NewPtr) = 0;
    Data = NewPtr;
    memcpy(NewPtr, OldPtr, OldSize);
    memcpy(NewPtr + OldSize, CStr, N);
    OwnsData = true;

    if(DeletePtr)
      delete [] DeletePtr;
  }
  else
    CreateOwnData(CStr, N);
}

void festring::Assign(sizetype N, char C)
{
  Size = N;
  char* Ptr = Data;

  if(OwnsData && Ptr)
  {
    if(!REFS(Ptr) && N <= Reserved)
    {
      memset(Ptr, C, N);
      return;
    }
    else
      delete [] &REFS(Ptr);
  }

  Reserved = N|FESTRING_PAGE;
  Ptr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
  REFS(Ptr) = 0;
  Data = Ptr;
  memset(Ptr, C, N);
  OwnsData = true;
}

void festring::Resize(sizetype N, char C)
{
  sizetype OldSize = Size;
  char* OldPtr = Data;
  char* NewPtr;
  Size = N;

  if(OldSize < N)
  {
    ulong* DeletePtr = 0;

    if(OwnsData && OldPtr)
    {
      if(!REFS(OldPtr))
      {
	if(N <= Reserved)
	{
	  memset(OldPtr + OldSize, C, N - OldSize);
	  return;
	}
	else
	  DeletePtr = &REFS(OldPtr);
      }
      else
	--REFS(OldPtr);
    }

    Reserved = N|FESTRING_PAGE;
    NewPtr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
    REFS(NewPtr) = 0;
    Data = NewPtr;
    memcpy(NewPtr, OldPtr, OldSize);
    memset(NewPtr + OldSize, C, N - OldSize);
    OwnsData = true;

    if(DeletePtr)
      delete [] DeletePtr;
  }
  else
  {
    if(OwnsData && OldPtr)
      if(!REFS(OldPtr))
	return;
      else
	--REFS(OldPtr);

    Reserved = N|FESTRING_PAGE;
    NewPtr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
    REFS(NewPtr) = 0;
    Data = NewPtr;
    memcpy(NewPtr, OldPtr, N);
    OwnsData = true;
  }
}

festring::sizetype festring::Find(char Char, sizetype Pos) const
{
  char* Ptr = Data;

  if(Ptr)
  {
    char* Result = static_cast<char*>(memchr(Ptr + Pos, Char, Size - Pos));

    if(Result)
      return Result - Ptr;
  }

  return NPos;
}

festring::sizetype festring::Find(cchar* CStr,
				  sizetype Pos,
				  sizetype N) const
{
  if(N)
  {
    char* Ptr = Data;

    if(Ptr)
    {
      char Char = CStr[0];

      for(;;)
      {
	char* Result = static_cast<char*>(memchr(Ptr + Pos, Char,
						 Size - Pos));

	if(!Result)
	  return NPos;

	if(!memcmp(Result, CStr, N))
	  return Result - Ptr;
	else
	  Pos = Result - Ptr + 1;
      }
    }
  }

  return NPos;
}

festring::sizetype festring::FindLast(char Char, sizetype Pos) const
{
  char* Ptr = Data;

  if(Ptr)
  {
    if(Pos >= Size)
      Pos = Size - 1;

    sizetype c;
    for(c = Pos; c != NPos && Ptr[c] != Char; --c);
    return c;
  }
  else
    return NPos;
}

festring::sizetype festring::FindLast(const char* CStr,
				      sizetype Pos,
				      sizetype N) const
{
  if(N)
  {
    char* Ptr = Data;

    if(Ptr && Size >= N)
    {
      char Char = CStr[0];

      if(Pos > Size - N)
	Pos = Size - N;

      for(sizetype c = Pos; c != NPos; --c)
	if(Ptr[c] == Char && !memcmp(Ptr + c, CStr, N))
	  return c;

      return NPos;
    }
  }

  return NPos;
}

void festring::Erase(sizetype Pos, sizetype Length)
{
  char* OldPtr = Data;

  if(OldPtr && Length)
  {
    sizetype OldSize = Size;

    if(Pos < OldSize)
    {
      truth MoveReq = Length < OldSize - Pos;

      if(OwnsData)
      {
	if(!REFS(OldPtr))
	{
	  if(MoveReq)
	  {
	    sizetype End = Pos + Length;
	    memmove(OldPtr + Pos, OldPtr + End, OldSize - End);
	  }

	  Size -= Length;
	  return;
	}
	else
	  --REFS(OldPtr);
      }

      sizetype NewSize = MoveReq ? OldSize - Length : Pos;
      Size = NewSize;
      Reserved = NewSize|FESTRING_PAGE;
      char* Ptr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
      REFS(Ptr) = 0;
      Data = Ptr;
      OwnsData = true;

      if(Pos)
	memcpy(Ptr, OldPtr, Pos);

      if(MoveReq)
      {
	sizetype End = Pos + Length;
	memcpy(Ptr + Pos, OldPtr + End, OldSize - End);
      }
    }
  }
}

void festring::Insert(sizetype Pos, cchar* CStr, sizetype N)
{
  if(N)
  {
    sizetype OldSize = Size;

    if(Pos < OldSize) // this implies Data != 0
    {
      char* OldPtr = Data;
      ulong* DeletePtr = 0;
      sizetype NewSize = OldSize + N;
      Size = NewSize;

      if(OwnsData)
      {
	if(!REFS(OldPtr))
	{
	  if(NewSize <= Reserved)
	  {
	    char* Ptr = OldPtr + Pos;
	    memmove(Ptr + N, Ptr, OldSize - Pos);
	    memcpy(Ptr, CStr, N);
	    return;
	  }
	  else
	    DeletePtr = &REFS(OldPtr);
	}
	else
	  --REFS(OldPtr);
      }

      Reserved = NewSize|FESTRING_PAGE;
      char* NewPtr = sizeof(int*) + new char[Reserved + sizeof(int*)+1];
      REFS(NewPtr) = 0;
      Data = NewPtr;
      memcpy(NewPtr, OldPtr, Pos);
      memcpy(NewPtr + Pos, CStr, N);
      memcpy(NewPtr + Pos + N, OldPtr + Pos, OldSize - Pos);
      OwnsData = true;

      if(DeletePtr)
	delete [] DeletePtr;
    }
    else if(Pos == OldSize)
      Append(CStr, N);
    else
      ABORT("Illegal festring insertion detected!");
  }
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

/* Displays numbers in the range [-2147483647, 2147483647].
   Much faster than sprintf and (nonstandard) itoa. */
#include <cstdio>
festring& festring::Append(long Integer)
{
  if(!IntegerMap)
    InstallIntegerMap();
  char IntegerBuffer[12];
//   snprintf(IntegerBuffer,12,"%l",&Integer);
//   return Append(IntegerBuffer,strlen(IntegerBuffer));
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
  char* EndPtr = BufferPtr;

  for(BufferPtr = IntegerBuffer; *BufferPtr == '0'; ++BufferPtr);

  if(Negative)
    *--BufferPtr = '-';
  else if(!*BufferPtr) // check if the original Integer was zero
    --BufferPtr;

  return Append(BufferPtr,strlen(BufferPtr));
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
   Marginal,  in which case a number of spaces is inserted in the
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
  sizetype Size = 1;

  while(!CopyOfSource.IsEmpty())
  {
    if(StringVector.size() <= Size)
      StringVector.push_back(festring());

    festring& String = StringVector[Size++];
    String.Assign(Marginal, ' ');
    SplitString(CopyOfSource, String, Length - Marginal);
  }

  return Size;
}

char Capitalize(char Char)
{ return Char > 0x60 && Char < 0x7B ? Char ^ 0x20 : Char; }

/* Returns the position of the first occurance of What in Where
   starting at Begin or after it, ignoring the case of letters.
   If the search fails, festring::NPos is returned instead. */

festring::sizetype festring::IgnoreCaseFind(cfestring& Where,
					    cfestring& What,
					    sizetype Begin)
{
  if(What.IsEmpty())
    return Begin;

  for(; Where.GetSize() >= What.GetSize() + Begin; ++Begin)
    if(::Capitalize(Where[Begin]) == ::Capitalize(What[0]))
    {
      truth Equal = true;

      for(sizetype c = 1; c < What.GetSize(); ++c)
	if(::Capitalize(Where[Begin + c]) != ::Capitalize(What[c]))
	{
	  Equal = false;
	  break;
	}

      if(Equal)
	return Begin;
    }

  return NPos;
}

/* Replaces all occurances of What in Where after Begin with With */

void festring::SearchAndReplace(festring& Where, cfestring& What,
				cfestring& With, sizetype Begin)
{
  for(sizetype Pos = Where.Find(What, Begin);
      Pos != NPos; Pos = Where.Find(What, Pos))
  {
    Where.Erase(Pos, What.GetSize());
    Where.Insert(Pos, With);
  }
}

/* Returns whether First is behind Second in alphabetical order,
   ignoring case */

bool festring::IgnoreCaseCompare(cfestring& First,
				 cfestring& Second)
{
  for(sizetype Pos = 0;
      Pos < First.GetSize() && Pos < Second.GetSize(); ++Pos)
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
      Data[c] = ' ';
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
    /* Erase() guarantees that OwnsData != false && REFS(Data) == 0 */
    else if(CapitalizeNextChar)
    {
      if(Char1 > 0x60 && Char1 < 0x7B)
	Data[c] &= ~0x20;

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
  long Counter = 0;

  for(ushort c = 0; c < GetSize(); ++c)
    Counter = long(Data[c]);

  return Counter;
}

void festring::EnsureOwnsData()
{
  if(!OwnsData)
    CreateOwnData(Data, Size);
}
