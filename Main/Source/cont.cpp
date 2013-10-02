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

/* Compiled through wmapset.cpp */

uchar** continent::TypeBuffer;
short** continent::AltitudeBuffer;
uchar** continent::ContinentBuffer;

continent::continent() { }
continent::continent(int Index) : Index(Index) { }
long continent::GetSize() const { return Member.size(); }
int continent::GetGTerrainAmount(int Type) const { return GTerrainAmount[Type]; }
v2 continent::GetMember(int I) const { return Member[I]; }

void continent::Save(outputfile& SaveFile) const
{
  SaveFile << Name << Member << Index;
}

void continent::Load(inputfile& SaveFile)
{
  SaveFile >> Name >> Member >> Index;
}

void continent::AttachTo(continent* Continent)
{
  for(ulong c = 0; c < Member.size(); ++c)
    ContinentBuffer[Member[c].X][Member[c].Y] = Continent->Index;

  if(!Continent->Member.size())
    Continent->Member.swap(Member);
  else
  {
    Continent->Member.insert(Continent->Member.end(), Member.begin(), Member.end());
    Member.clear();
  }
}

void continent::GenerateInfo()
{
  GTerrainAmount.resize(protocontainer<gwterrain>::GetSize() + 1);

  for(ulong c = 0; c < Member.size(); ++c)
    ++GTerrainAmount[TypeBuffer[Member[c].X][Member[c].Y]];

  Name = CONST_S("number ");
  Name << Index;
}

v2 continent::GetRandomMember(int Type)
{
  if(!GTerrainAmount[Type])
    ABORT("Shortage of terrain!");

  v2* TypeContainer = new v2[Member.size()];
  long Index = 0;

  for(ulong c = 0; c < Member.size(); ++c)
    if(TypeBuffer[Member[c].X][Member[c].Y] == Type)
    {
      TypeContainer[Index++] = Member[c];

      if(Index == GetGTerrainAmount(Type))
	break;
    }

  v2 Return = TypeContainer[RAND() % Index];
  delete [] TypeContainer;
  return Return;
}

outputfile& operator<<(outputfile& SaveFile, const continent* Continent)
{
  if(Continent)
  {
    SaveFile.Put(1);
    Continent->Save(SaveFile);
  }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, continent*& Continent)
{
  if(SaveFile.Get())
  {
    Continent = new continent;
    Continent->Load(SaveFile);
  }

  return SaveFile;
}

void continent::Add(v2 Pos)
{
  Member.push_back(Pos);
  ContinentBuffer[Pos.X][Pos.Y] = Index;
}
