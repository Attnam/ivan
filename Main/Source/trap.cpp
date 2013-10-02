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

/* Compiled through trapset.cpp */

trapprototype::trapprototype(trapspawner Spawner, cchar* ClassID) : Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<trap>::Add(this); }

trap::trap() : entity(HAS_BE), Next(0) { }
square* trap::GetSquareUnderEntity(int) const { return LSquareUnder; }

trap::~trap()
{
}

void trap::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
}

void trap::Load(inputfile&)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
}

outputfile& operator<<(outputfile& SaveFile, const trapdata* Data)
{
  SaveFile << *Data;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata*& Data)
{
  Data = new trapdata;
  SaveFile >> *Data;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const trapdata& Data)
{
  SaveFile << Data.TrapID << Data.VictimID << Data.BodyParts;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata& Data)
{
  SaveFile >> Data.TrapID >> Data.VictimID >> Data.BodyParts;
  return SaveFile;
}

trap* trapprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  trap* Trap = Spawner();
  Trap->Load(SaveFile);
  return Trap;
}

void itemtrapbase::Load(inputfile& SaveFile)
{
  SaveFile >> Active >> Team >> DiscoveredByTeam;
}

void itemtrapbase::Save(outputfile& SaveFile) const
{
  SaveFile << Active << Team << DiscoveredByTeam;
}

truth itemtrapbase::CanBeSeenBy(ccharacter* Viewer) const
{
  int ViewerTeam = Viewer->GetTeam()->GetID();
  return !Active || ViewerTeam == Team || DiscoveredByTeam.find(ViewerTeam) != DiscoveredByTeam.end();
}

void itemtrapbase::Search(ccharacter* Char, int Perception)
{
  int ViewerTeam = Char->GetTeam()->GetID();

  if(Active && ViewerTeam != Team && DiscoveredByTeam.find(ViewerTeam) == DiscoveredByTeam.end() && !RAND_N(200 / Perception))
  {
    DiscoveredByTeam.insert(ViewerTeam);
    SendNewDrawAndMemorizedUpdateRequest();

    if(Char->IsPlayer())
    {
      game::AskForKeyPress(CONST_S("Trap found! [press any key to continue]"));
      ADD_MESSAGE("You find %s.", CHAR_NAME(INDEFINITE));
    }
  }
}

void itemtrapbase::SetIsActive(truth What)
{
  Active = What;
  UpdatePictures();
  DiscoveredByTeam.clear();
}

void itemtrapbase::FinalProcessForBone()
{
  if(Team == PLAYER_TEAM)
    Team = MONSTER_TEAM;

  std::set<int>::iterator i = DiscoveredByTeam.find(PLAYER_TEAM);

  if(i != DiscoveredByTeam.end())
    DiscoveredByTeam.erase(i);
}

void itemtrapbase::TeleportRandomly()
{
  Team = NO_TEAM;
  DiscoveredByTeam.clear();
}
