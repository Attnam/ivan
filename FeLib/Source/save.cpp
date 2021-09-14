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

#ifdef WIN32
#include <windows.h>
#include <direct.h> // for _mkdir
#else
#include <dirent.h>
#include <sys/stat.h> // for mkdir
#endif

#include "save.h"
#include "femath.h"

#include "dbgmsgproj.h"

truth outputfile::bakcupBeforeSaving = true;
truth outputfile::saveOnNewFileAlways = true;

void outputfile::Close() { DBG3(FileNameNewTmp.CStr(),FileName.CStr(),saveOnNewFileAlways);
  File.close();

  if(saveOnNewFileAlways){ // This moves the new .tmp file to the correct filename, before removing it.
    if(FileNameNewTmp.IsEmpty())
      ABORT("new tmp filename is empty, 'save on new file always' option was properly set before this output file '%s' ?",FileName.CStr());

    std::ifstream newTmpFile(FileNameNewTmp.CStr(), std::ios::binary);
    if(newTmpFile.good()){
      std::remove(FileName.CStr()); //just to not let existing one create any doubts if some crash happens here

      std::ofstream final(FileName.CStr(), std::ios::binary);
      final << newTmpFile.rdbuf();

      final.close();
      newTmpFile.close();

      std::remove(FileNameNewTmp.CStr()); //last thing
    }else{
      ABORT("the new tmp file '%s' should exist!",FileNameNewTmp.CStr());
    }
  }
}

outputfile::outputfile(cfestring& fileName, truth AbortOnErr)
: FileName(fileName)
{
  if(FileName.IsEmpty())
    ABORT("empty output filename");

  // If FileName contains a directory, make sure the directory exists first.
  festring::csizetype LastPathSeparatorPos = FileName.FindLast('/');
  if(LastPathSeparatorPos != festring::NPos)
  {
    festring DirectoryPath = FileName;
    DirectoryPath.Resize(LastPathSeparatorPos);
    MakePath(DirectoryPath);
  }

  if(bakcupBeforeSaving){ // this is not that useful, the tmp files are better as they prevent overwriting the final files in case the game crashes
    std::ifstream orig(FileName.CStr(), std::ios::binary);
    if(orig.good()){
      festring fsBkp;fsBkp << FileName << ".bkp";
      std::remove(fsBkp.CStr()); //just to have a granted clean new backup

      std::ofstream bkp(fsBkp.CStr(), std::ios::binary);
      bkp << orig.rdbuf();

      orig.close();
      bkp.close();

      // backups are kept until leaving the game
    }
  }

  festring FileNameNew(FileName);
  if(saveOnNewFileAlways){
    FileNameNew<<".tmp";
    FileNameNewTmp<<FileNameNew;

    /**
     * if happened a crash and a tmp file is still there,
     * it is most probably invalid and incomplete,
     * so just remove it.
     */
    std::remove(FileNameNew.CStr());
  }

  File.open(FileNameNew.CStr(), std::ios::binary);

  if(AbortOnErr && !IsOpen())
    ABORT("Can't open %s for output!", FileNameNew.CStr());
}

inputfile::inputfile(cfestring& FileName,
                     const valuemap* ValueMap,
                     truth AbortOnErr)
: File(FileName.CStr(), std::ios::binary),
  FileName(FileName),
  ValueMap(ValueMap),
  lastWordWasString(false)
{
  if(AbortOnErr && !IsOpen())
    ABORT("File %s not found!", FileName.CStr());
}

festring inputfile::ReadWord(truth AbortOnEOF)
{
  /*static*/ festring ToReturn;
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
    if(!Eof())
    {
      Char = Get();

      if(Char == '*')
      {
        long StartPos = TellPos();
        int OldChar = 0, CommentLevel = 1;

        for(;;)
        {
          if(Eof())
            ABORT("Unterminated comment in file %s, beginning at line %ld!",
                  FileName.CStr(), TellLineOfPos(StartPos));

          Char = Get();

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
        File.putback(Char);
        File.clear();
      }
    }

    if(Mode)
      File.putback('/');
    else
      String << '/';

    return PUNCT_RETURN;
  }

  if(Mode)
  {
    File.putback(Char);
    return PUNCT_RETURN;
  }

  if(Char == '"')
  {
    lastWordWasString = true;
    long StartPos = TellPos();
    int OldChar = 0;

    for(;;)
    {
      if(Eof())
        ABORT("Unterminated string in file %s, beginning at line %ld!",
              FileName.CStr(), TellLineOfPos(StartPos));

      Char = Get();

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
  lastWordWasString = false;

  for(int Char = Get(); !Eof(); Char = Get())
  {
    if(isalpha(Char) || Char == '_')
    {
      if(!Mode)
        Mode = MODE_WORD;
      else if(Mode == MODE_NUMBER)
      {
        File.putback(Char);
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
        File.putback(Char);
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
    File.clear();
}

char inputfile::ReadLetter(truth AbortOnEOF)
{
  for(int Char = Get(); !Eof(); Char = Get())
  {
    if(isalpha(Char) || isdigit(Char))
      return Char;

    if(ispunct(Char))
    {
      if(Char == '/')
      {
        if(!Eof())
        {
          Char = Get();

          if(Char == '*')
          {
            long StartPos = TellPos();
            int OldChar = 0, CommentLevel = 1;

            for(;;)
            {
              if(Eof())
                ABORT("Unterminated comment in file %s, "
                      "beginning at line %ld!",
                      FileName.CStr(), TellLineOfPos(StartPos));

              Char = Get();

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
            File.putback(Char);
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

festring inputfile::ReadNumberIntr(int CallLevel, long *num, truth *isString, truth allowStr, truth PreserveTerminator)
{
  long Value = 0;
  /*static*/ festring Word;
  truth NumberCorrect = false;
  truth firstWord = true;
  *isString = false;
  *num = 0;
  festring res;

  for(;;)
  {
    ReadWord(Word);
    if(firstWord)
    {
      if(allowStr && lastWordWasString)
      {
        *isString = true;
        return Word;
      }
      firstWord = false;
    }
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
          File.putback(First);

        *num = Value;
        return res;
      }

      if(First == ')')
      {
        if((CallLevel != HIGHEST && CallLevel != 4) || PreserveTerminator)
          File.putback(')');

        *num = Value;
        return res;
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
          {\
            if(cl < CallLevel)\
              {\
                Value op##= ReadNumber(cl);\
                NumberCorrect = true;\
                continue;\
              }\
            else\
              {\
                File.putback(#op[0]);\
                *num = Value;\
                return res;\
              }\
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
            File.putback('<');
            File.putback('<');
            *num = Value;
            return res;
          }
        else
          File.putback(Next);
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
            File.putback('>');
            File.putback('>');
            *num = Value;
            return res;
          }
        else
          File.putback(Next);
      }

      if(First == '(')
      {
        if(NumberCorrect)
        {
          File.putback('(');
          *num = Value;
          return res;
        }
        else
        {
          Value = ReadNumber(4);
          NumberCorrect = false;
          continue;
        }
      }

      if(First == '=' && CallLevel == HIGHEST)
        continue;

      if(First == '#') // for #defines
      {
        File.putback('#');
        *num = Value;
        return res;
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

long inputfile::ReadNumber(int CallLevel, truth PreserveTerminator)
{
  long num = 0;
  truth isString = false;
  ReadNumberIntr(CallLevel, &num, &isString, false, PreserveTerminator);
  return num;
}

festring inputfile::ReadStringOrNumber(long *num, truth *isString, truth PreserveTerminator)
{
  return ReadNumberIntr(0xFF, num, isString, true, PreserveTerminator);
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
  /*static*/ festring Word;
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
  /*static*/ festring Word;
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
    if(Get() == '\n')
      ++Line;

  if(TellPos() != BackupPos)
    SeekPosBegin(BackupPos);

  return Line;
}

/* Returns a vector of absolute file paths referring to files inside Directory
 * with the given Extension. */

std::vector<festring> ListFiles(festring Directory, cfestring& Extension)
{
  std::vector<festring> Files;
  if(Directory[Directory.GetSize() - 1] != '/')
    Directory << '/';

#ifdef WIN32
  festring SearchPath = Directory + "*" + Extension;
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(SearchPath.CStr(), &fd);
  if(hFind == INVALID_HANDLE_VALUE)
    ABORT("Cannot open directory %s", Directory.CStr());

  do
  {
    if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      Files.push_back(Directory + fd.cFileName);
  }
  while(::FindNextFile(hFind, &fd));
  ::FindClose(hFind);
#else
#ifdef __DJGPP__
#error "ListFiles() not implemented on DOS!"
#else
  DIR* dirp = opendir(Directory.CStr());
  if(!dirp)
    ABORT("Cannot open directory %s", Directory.CStr());

  dirent* dp;
  while((dp = readdir(dirp)) != nullptr)
  {
    cint ExtIndex = strlen(dp->d_name) - Extension.GetSize();
    if(ExtIndex < 0) continue;
    if(strcmp(dp->d_name + ExtIndex, Extension.CStr()) == 0)
      Files.push_back(Directory + dp->d_name);
  }
  closedir(dirp);
#endif
#endif
  return Files;
}

/* Postcondition: Path exists and is a directory. */

void MakePath(cfestring& Path)
{
  for(festring::sizetype Pos = 0; Pos != Path.GetSize();)
  {
    Pos = Path.Find('/', Pos + 1);
    if (Pos == festring::NPos) Pos = Path.GetSize();
    cfestring DirectoryPath = festring(Path.CStr(), Pos) + '\0';
#ifdef WIN32
    _mkdir(DirectoryPath.CStr());
#else
    mkdir(DirectoryPath.CStr(), S_IRWXU|S_IRWXG);
#endif
  }
}

festring GetUserDataDir()
{
#ifdef UNIX
  festring Dir;
#ifdef MAC_APP
  Dir << getenv("HOME") << "/Library/Application Support/IVAN/";
#else
  char *xdg_home = getenv("XDG_DATA_HOME"); // check if XDG_DATA_HOME is set
  if (xdg_home != NULL) { // if it is, use that directory
          Dir << xdg_home << "/ivan/";
  } else { // otherwise, default to home directory
          Dir << getenv("HOME") << "/.ivan/";
  }
#endif /* MAC_APP */
#ifdef DBGMSG
  dbgmsg::SetDebugLogPath(Dir.CStr());
#endif /* DBGMSG */
  return Dir;
#endif /* UNIX */

#if defined(WIN32) || defined(__DJGPP__)
  return "./";
#endif
}
