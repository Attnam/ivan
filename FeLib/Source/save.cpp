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

#include "save.h"
#include "femath.h"

outputfile::outputfile(cfestring& FileName, truth AbortOnErr)
: Buffer(fopen(FileName.CStr(), "wb")), FileName(FileName)
{
  if(AbortOnErr && !IsOpen())
    ABORT("Can't open %s for output!", FileName.CStr());
}

outputfile::~outputfile()
{
  if(Buffer)
    fclose(Buffer);
}

void outputfile::ReOpen()
{
  fclose(Buffer);
  Buffer = fopen(FileName.CStr(), "ab");
}

inputfile::inputfile(cfestring& FileName,
		     const valuemap* ValueMap,
		     truth AbortOnErr)
: Buffer(fopen(FileName.CStr(), "rb")),
  FileName(FileName), ValueMap(ValueMap)
{
  if(AbortOnErr && !IsOpen())
    ABORT("File %s not found!", FileName.CStr());
}

inputfile::~inputfile()
{
  if(Buffer)
    fclose(Buffer);
}

festring inputfile::ReadWord(truth AbortOnEOF)
{
  static festring ToReturn;
  ReadWord(ToReturn, AbortOnEOF);
  return ToReturn;
}

#define MODE_WORD 1
#define MODE_NUMBER 2

#define PUNCT_RETURN 0
#define PUNCT_CONTINUE 1

int inputfile::HandlePunct(festring& String, int Char, int Mode)
{
  if(Char == '/')
  {
    if(!feof(Buffer))
    {
      Char = fgetc(Buffer);

      if(Char == '*')
      {
	long StartPos = TellPos();
	int OldChar = 0, CommentLevel = 1;

	for(;;)
	{
	  if(feof(Buffer))
	    ABORT("Unterminated comment in file %s, beginning at line %ld!",
		  FileName.CStr(), TellLineOfPos(StartPos));

	  Char = fgetc(Buffer);

	  if(OldChar != '*' || Char != '/')
	  {
	    if(OldChar != '/' || Char != '*')
	      OldChar = Char;
	    else
	    {
	      ++CommentLevel;
	      OldChar = 0;
	    }
	  }
	  else
	  {
	    if(!--CommentLevel)
	      break;
	    else
	      OldChar = 0;
	  }
	}

	return PUNCT_CONTINUE;
      }
      else
      {
	ungetc(Char, Buffer);
	clearerr(Buffer);
      }
    }

    if(Mode)
      ungetc('/', Buffer);
    else
      String << '/';

    return PUNCT_RETURN;
  }

  if(Mode)
  {
    ungetc(Char, Buffer);
    return PUNCT_RETURN;
  }

  if(Char == '"')
  {
    long StartPos = TellPos();
    int OldChar = 0;

    for(;;)
    {
      if(feof(Buffer))
	ABORT("Unterminated string in file %s, beginning at line %ld!",
	      FileName.CStr(), TellLineOfPos(StartPos));

      Char = fgetc(Buffer);

      if(Char != '"')
      {
	String << char(Char);
	OldChar = Char;
      }
      else if(OldChar == '\\')
      {
	String[String.GetSize() - 1] = '"';
	OldChar = 0;
      }
      else
	return PUNCT_RETURN;
    }
  }

  String << char(Char);
  return PUNCT_RETURN;
}

void inputfile::ReadWord(festring& String, truth AbortOnEOF)
{
  int Mode = 0;
  String.Empty();

  for(int Char = fgetc(Buffer); !feof(Buffer); Char = fgetc(Buffer))
  {
    if(isalpha(Char) || Char == '_')
    {
      if(!Mode)
	Mode = MODE_WORD;
      else if(Mode == MODE_NUMBER)
      {
	ungetc(Char, Buffer);
	return;
      }

      String << char(Char);
      continue;
    }

    if(isdigit(Char))
    {
      if(!Mode)
	Mode = MODE_NUMBER;
      else if(Mode == MODE_WORD)
      {
	ungetc(Char, Buffer);
	return;
      }

      String << char(Char);
      continue;
    }

    if((Char == ' ' || Char == '\n') && Mode)
      return;

    if(ispunct(Char) && HandlePunct(String, Char, Mode) == PUNCT_RETURN)
      return;
  }

  if(AbortOnEOF)
    ABORT("Unexpected end of file %s!", FileName.CStr());

  if(Mode)
    clearerr(Buffer);
}

char inputfile::ReadLetter(truth AbortOnEOF)
{
  for(int Char = fgetc(Buffer); !feof(Buffer); Char = fgetc(Buffer))
  {
    if(isalpha(Char) || isdigit(Char))
      return Char;

    if(ispunct(Char))
    {
      if(Char == '/')
      {
	if(!feof(Buffer))
	{
	  Char = fgetc(Buffer);

	  if(Char == '*')
	  {
	    long StartPos = TellPos();
	    int OldChar = 0, CommentLevel = 1;

	    for(;;)
	    {
	      if(feof(Buffer))
		ABORT("Unterminated comment in file %s, "
		      "beginning at line %ld!",
		      FileName.CStr(), TellLineOfPos(StartPos));

	      Char = fgetc(Buffer);

	      if(OldChar != '*' || Char != '/')
	      {
		if(OldChar != '/' || Char != '*')
		  OldChar = Char;
		else
		{
		  ++CommentLevel;
		  OldChar = 0;
		}
	      }
	      else
	      {
		if(!--CommentLevel)
		  break;
		else
		  OldChar = 0;
	      }
	    }

	    continue;
	  }
	  else
	    ungetc(Char, Buffer);
	}

	return '/';
      }

      return Char;
    }
  }

  if(AbortOnEOF)
    ABORT("Unexpected end of file %s!", FileName.CStr());

  return 0;
}

/* Reads a number or a formula from inputfile. Valid values could be for
   instance "3", "5 * 4+5", "2+Variable%4" etc. */

long inputfile::ReadNumber(int CallLevel, truth PreserveTerminator)
{
  long Value = 0;
  static festring Word;
  truth NumberCorrect = false;

  for(;;)
  {
    ReadWord(Word);
    char First = Word[0];

    if(isdigit(First))
    {
      Value = atoi(Word.CStr());
      NumberCorrect = true;
      continue;
    }

    if(Word.GetSize() == 1)
    {
      if(First == ';' || First == ',' || First == ':')
      {
	if(CallLevel != HIGHEST || PreserveTerminator)
	  ungetc(First, Buffer);

	return Value;
      }

      if(First == ')')
      {
	if((CallLevel != HIGHEST && CallLevel != 4) || PreserveTerminator)
	  ungetc(')', Buffer);

	return Value;
      }

      if(First == '~')
      {
	Value = ~ReadNumber(4);
	NumberCorrect = true;
	continue;
      }

      /* Convert this into an inline function! */

#define CHECK_OP(op, cl)\
	  if(First == #op[0])\
	    if(cl < CallLevel)\
	      {\
		Value op##= ReadNumber(cl);\
		NumberCorrect = true;\
		continue;\
	      }\
	    else\
	      {\
		ungetc(#op[0], Buffer);\
		return Value;\
	      }

      CHECK_OP(&, 1); CHECK_OP(|, 1); CHECK_OP(^, 1);
      CHECK_OP(*, 2); CHECK_OP(/, 2); CHECK_OP(%, 2);
      CHECK_OP(+, 3); CHECK_OP(-, 3);

      if(First == '<')
      {
	char Next = Get();

	if(Next == '<')
	  if(1 < CallLevel)
	  {
	    Value <<= ReadNumber(1);
	    NumberCorrect = true;
	    continue;
	  }
	  else
	  {
	    ungetc('<', Buffer);
	    ungetc('<', Buffer);
	    return Value;
	  }
	else
	  ungetc(Next, Buffer);
      }

      if(First == '>')
      {
	char Next = Get();

	if(Next == '>')
	  if(1 < CallLevel)
	  {
	    Value >>= ReadNumber(1);
	    NumberCorrect = true;
	    continue;
	  }
	  else
	  {
	    ungetc('>', Buffer);
	    ungetc('>', Buffer);
	    return Value;
	  }
	else
	  ungetc(Next, Buffer);
      }

      if(First == '(')
	if(NumberCorrect)
	{
	  ungetc('(', Buffer);
	  return Value;
	}
	else
	{
	  Value = ReadNumber(4);
	  NumberCorrect = false;
	  continue;
	}

      if(First == '=' && CallLevel == HIGHEST)
	continue;

      if(First == '#') // for #defines
      {
	ungetc('#', Buffer);
	return Value;
      }
    }

    if(Word == "rgb")
    {
      int Bits = ReadNumber();

      if(Bits == 16)
      {
	int Red = ReadNumber();
	int Green = ReadNumber();
	int Blue = ReadNumber();
	Value = MakeRGB16(Red, Green, Blue);
      }
      else if(Bits == 24)
      {
	int Red = ReadNumber();
	int Green = ReadNumber();
	int Blue = ReadNumber();
	Value = MakeRGB24(Red, Green, Blue);
      }
      else
	ABORT("Illegal RGB bit size %d in file %s, line %ld!",
	      Bits, FileName.CStr(), TellLine());

      NumberCorrect = true;
      continue;
    }

    if(Word == "true")
    {
      Value = 1;
      NumberCorrect = true;
      continue;
    }

    if(Word == "false")
    {
      Value = 0;
      NumberCorrect = true;
      continue;
    }

    if(ValueMap)
    {
      valuemap::const_iterator Iterator = ValueMap->find(Word);

      if(Iterator != ValueMap->end())
      {
	Value = Iterator->second;
	NumberCorrect = true;
	continue;
      }
    }

    ABORT("Odd numeric value \"%s\" encountered in file %s, line %ld!",
	  Word.CStr(), FileName.CStr(), TellLine());
  }
}

v2 inputfile::ReadVector2d()
{
  v2 Vector;
  Vector.X = ReadNumber();
  Vector.Y = ReadNumber();
  return Vector;
}

rect inputfile::ReadRect()
{
  rect Rect;
  Rect.X1 = ReadNumber();
  Rect.Y1 = ReadNumber();
  Rect.X2 = ReadNumber();
  Rect.Y2 = ReadNumber();
  return Rect;
}

outputfile& operator<<(outputfile& SaveFile, cfestring& String)
{
  ushort Length = String.GetSize();
  SaveFile << Length;

  if(Length)
    SaveFile.Write(String.CStr(), Length);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, festring& String)
{
  char* RealBuffer, StackBuffer[1024];
  ushort Length;
  SaveFile >> Length;
  RealBuffer = Length < 1024 ? StackBuffer : new char[Length + 1];
  String.Empty();

  if(Length)
  {
    SaveFile.Read(RealBuffer, Length);
    RealBuffer[Length] = 0;
    String << RealBuffer;
  }

  if(Length >= 1024)
    delete [] RealBuffer;

  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, cchar* String)
{
  ushort Length = String ? strlen(String) : 0;
  SaveFile << Length;

  if(Length)
    SaveFile.Write(String, Length);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, char*& String)
{
  ushort Length;
  SaveFile >> Length;

  if(Length)
  {
    String = new char[Length + 1];
    SaveFile.Read(String, Length);
    String[Length] = 0;
  }
  else
    String = 0;

  return SaveFile;
}

void ReadData(festring& String, inputfile& SaveFile)
{
  SaveFile.ReadWord(String);

  if(String == "=")
    SaveFile.ReadWord(String);

  SaveFile.ReadWord();
}

void ReadData(fearray<long>& Array, inputfile& SaveFile)
{
  Array.Clear();
  static festring Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
  {
    Array.Allocate(1);
    Array.Data[0] = SaveFile.ReadNumber();
    return;
  }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());

  fearray<long>::sizetype Size = SaveFile.ReadNumber();
  Array.Allocate(Size);

  for(fearray<long>::sizetype c = 0; c < Size; ++c)
    Array.Data[c] = SaveFile.ReadNumber();

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" "
	  "encountered in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(),
	  SaveFile.TellLine());
}

void ReadData(fearray<festring>& Array, inputfile& SaveFile)
{
  Array.Clear();
  static festring Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
  {
    Array.Allocate(1);
    SaveFile.ReadWord(Array.Data[0]);

    if(SaveFile.ReadWord() != ";")
      ABORT("Array syntax error \"%s\" found in file %s, line %ld!",
	    Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());

    return;
  }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());

  fearray<festring>::sizetype Size = SaveFile.ReadNumber();
  Array.Allocate(Size);

  for(fearray<festring>::sizetype c = 0; c < Size; ++c)
  {
    SaveFile.ReadWord(Array.Data[c]);
    SaveFile.ReadWord(Word);

    if(Word != "," && Word != ";")
      ABORT("Array syntax error \"%s\" found in file %s, line %ld!",
	    Word.CStr(), SaveFile.GetFileName().CStr(), SaveFile.TellLine());
  }

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" "
	  "encountered in file %s, line %ld!",
	  Word.CStr(), SaveFile.GetFileName().CStr(),
	  SaveFile.TellLine());
}

ulong inputfile::TellLineOfPos(long Pos)
{
  ulong Line = 1;
  long BackupPos = TellPos();
  SeekPosBegin(0);

  while(TellPos() != Pos)
    if(fgetc(Buffer) == '\n')
      ++Line;

  if(TellPos() != BackupPos)
    SeekPosBegin(BackupPos);

  return Line;
}
