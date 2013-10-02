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

#include "febot.h"
#include "femath.h"
#include "save.h"

/* The fword associated with the empty string is called ControlFWord and is always
   the first in FWordSet (since S1 < S2 for every string S2 if S1 is empty).
   Fwords to which ControlFWord is linked can begin a reply and those fwords linked
   to ControlFWord can end it. */

void febot::Initialize(ushort NewChainLength)
{
  ChainLength = NewChainLength;
  WordChainSet.clear();
  WordChainSet.insert(wordchain(ChainLength));
}

febot::wordchain* febot::GetControlWordChain() const
{ return const_cast<wordchain*>(&*WordChainSet.begin()); }

febot::wordchain* febot::wordchain::GetRandomLink() const
{ return Link[RAND() % Link.size()]; }

febot::wordchain::wordchain(const febot::wordchain* WordChain, cfestring& NewWord) : String(++WordChain->String.begin(), WordChain->String.end())
{
  String.push_back(NewWord);
}

/* Searches for an fword associated with String in FWordSet and returns it
   or creates a new one if needed */

febot::wordchain* febot::CreateWordChain(const febot::wordchain* WordChain, cfestring& NewWord)
{
  return const_cast<wordchain*>(&*WordChainSet.insert(wordchain(WordChain, NewWord)).first);
}

void febot::Save(outputfile& SaveFile) const
{


  /*SaveFile << ulong(WordChainSet.size());
    fwordset::const_iterator i1;*/

  /* Speeds up saving tremendously for large files */

  /*std::map<const fword*, ulong> FWordIndexMap;
    ulong c;

    for(i1 = FWordSet.begin(), c = 0; i1 != FWordSet.end(); ++i1, ++c)
    {
    SaveFile << i1->String;
    FWordIndexMap[&*i1] = c;
    }

    for(i1 = FWordSet.begin(); i1 != FWordSet.end(); ++i1)
    {
    SaveFile << ulong(i1->Link.size());

    for(c = 0; c < i1->Link.size(); ++c)
    SaveFile << FWordIndexMap.find(i1->Link[c])->second;
    }*/
}

void febot::Load(inputfile& SaveFile)
{
  wordchain* Chain = CreateWordChain(GetControlWordChain(), "Jaska");
  Chain = CreateWordChain(Chain, "on");
  /*FWordSet.clear();
    ulong MapSize;
    SaveFile >> MapSize;*/

  /* Speeds up loading tremendously for large files */

  /*std::map<ulong, fword*> FWordPtrMap;

  for(ulong c = 0; c < MapSize; ++c)
  FWordPtrMap[c] = const_cast<fword*>(&*FWordSet.insert(FWordSet.end(), fword(ReadType<festring>(SaveFile))));

  for(fwordset::const_iterator i1 = FWordSet.begin(); i1 != FWordSet.end(); ++i1)
  {
  ulong LinkSize;
  SaveFile >> LinkSize;
  fword* FWord = const_cast<fword*>(&*i1);
  FWord->Link.resize(LinkSize);

  for(ulong c = 0; c < LinkSize; ++c)
  FWord->Link[c] = FWordPtrMap.find(ReadType<ulong>(SaveFile))->second;
  }*/
}

void febot::BeTalkedTo(festring String)
{
  String.PreProcessForFebot();

  if(!String.IsEmpty())
  {
    festring Word;
    wordchain* OldChain = GetControlWordChain();

    for(String.ExtractWord(Word); !Word.IsEmpty(); String.ExtractWord(Word))
    {
      wordchain* NewChain = CreateWordChain(OldChain, Word);
      OldChain->Link.push_back(NewChain);
      OldChain = NewChain;
    }

    OldChain->Link.push_back(GetControlWordChain());
  }
}

void febot::Reply(festring& String) const
{
  String.Empty();

  if(GetControlWordChain()->Link.empty())
    return;

  wordchain* WordChain = GetControlWordChain()->GetRandomLink();
  String << WordChain->String.back();

  for(WordChain = WordChain->GetRandomLink(); !WordChain->String.back().IsEmpty(); WordChain = WordChain->GetRandomLink())
    String << ' ' << WordChain->String.back();

  String.PostProcessForFebot();
}

bool febot::wordchain::operator<(const febot::wordchain& W) const
{
  for(std::list<festring>::const_iterator i1 = String.begin(), i2 = W.String.begin(); i1 != String.end(); ++i1, ++i2)
  {
    char Comp = i1->Compare(*i2);

    if(Comp)
      return Comp < 0;
  }

  return false;
}
