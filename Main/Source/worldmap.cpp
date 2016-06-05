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

#define MAX_TEMPERATURE   27            // increase for a warmer world
#define LATITUDE_EFFECT   40            // increase for more effect
#define ALTITUDE_EFFECT   0.02

#define COLD              10
#define MEDIUM            12
#define WARM              17
#define HOT               19

int DirX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int DirY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

continent* worldmap::GetContinentUnder(v2 Pos) const
{ return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
v2 worldmap::GetEntryPos(ccharacter*, int I) const
{ return EntryMap.find(I)->second; }
continent* worldmap::GetContinent(int I) const { return Continent[I]; }
int worldmap::GetAltitude(v2 Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
charactervector& worldmap::GetPlayerGroup() { return PlayerGroup; }
character* worldmap::GetPlayerGroupMember(int c) { return PlayerGroup[c]; }

struct location
{
  v2 Position;
  int GTerrainType;
  int ContinentIndex;
  int DistanceToAttnam;

  location(v2 p, int t, int i, int d) : Position(p), GTerrainType(t), ContinentIndex(i), DistanceToAttnam(d){}
};

struct distancetoattnam
{
    inline bool operator() (const location& loc1, const location& loc2)
    {
        return (loc1.DistanceToAttnam < loc2.DistanceToAttnam);
    }
};

struct place
{
  int Type;
  int Config;
  int NativeGTerrainType;
  truth HasBeenPlaced;

  place(int t, int c, int n, truth p) : Type(t), Config(c), NativeGTerrainType(n), HasBeenPlaced(p){}
};

worldmap::worldmap(int XSize, int YSize) : area(XSize, YSize)
{
  Map = reinterpret_cast<wsquare***>(area::Map);

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
    {
      Map[x][y] = new wsquare(this, v2(x, y));
      Map[x][y]->SetGWTerrain(ocean::Spawn());
    }

  Alloc2D(TypeBuffer, XSize, YSize);
  Alloc2D(AltitudeBuffer, XSize, YSize);
  Alloc2D(ContinentBuffer, XSize, YSize);
  Alloc2D(PossibleLocationBuffer, XSize, YSize);
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
  continent::PossibleLocationBuffer = PossibleLocationBuffer;
}

worldmap::~worldmap()
{
  delete [] TypeBuffer;
  delete [] AltitudeBuffer;
  delete [] ContinentBuffer;
  delete [] PossibleLocationBuffer;

  uint c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  for(c = 0; c < PlayerGroup.size(); ++c)
    delete PlayerGroup[c];
}

void worldmap::Save(outputfile& SaveFile) const
{
  area::Save(SaveFile);
  SaveFile.Write(reinterpret_cast<char*>(TypeBuffer[0]),
                 XSizeTimesYSize * sizeof(uchar));
  SaveFile.Write(reinterpret_cast<char*>(AltitudeBuffer[0]),
                 XSizeTimesYSize * sizeof(short));
  SaveFile.Write(reinterpret_cast<char*>(ContinentBuffer[0]),
                 XSizeTimesYSize * sizeof(uchar));
  SaveFile.Write(reinterpret_cast<char*>(PossibleLocationBuffer[0]),
                 XSizeTimesYSize * sizeof(uchar));

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->Save(SaveFile);

  SaveFile << Continent << PlayerGroup;
}

void worldmap::Load(inputfile& SaveFile)
{
  area::Load(SaveFile);
  Map = reinterpret_cast<wsquare***>(area::Map);
  Alloc2D(TypeBuffer, XSize, YSize);
  Alloc2D(AltitudeBuffer, XSize, YSize);
  Alloc2D(ContinentBuffer, XSize, YSize);
  Alloc2D(PossibleLocationBuffer, XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(TypeBuffer[0]),
                XSizeTimesYSize * sizeof(uchar));
  SaveFile.Read(reinterpret_cast<char*>(AltitudeBuffer[0]),
                XSizeTimesYSize * sizeof(short));
  SaveFile.Read(reinterpret_cast<char*>(ContinentBuffer[0]),
                XSizeTimesYSize * sizeof(uchar));
  SaveFile.Read(reinterpret_cast<char*>(PossibleLocationBuffer[0]),
                XSizeTimesYSize * sizeof(uchar));
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
  continent::PossibleLocationBuffer = PossibleLocationBuffer;
  int x, y;

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
      Map[x][y] = new wsquare(this, v2(x, y));

  for(x = 0; x < XSize; ++x)
    for(y = 0; y < YSize; ++y)
    {
      game::SetSquareInLoad(Map[x][y]);
      Map[x][y]->Load(SaveFile);
    }

  CalculateNeighbourBitmapPoses();
  SaveFile >> Continent >> PlayerGroup;
}

void worldmap::Generate()
{
  Alloc2D(OldAltitudeBuffer, XSize, YSize);
  Alloc2D(OldTypeBuffer, XSize, YSize);
  Alloc2D(NoIslandAltitudeBuffer, XSize, YSize);

  for(;;)
  {
    RandomizeAltitude();
    SmoothAltitude();
    GenerateClimate();
    SmoothClimate();
    CalculateContinents();
    std::vector<continent*> PerfectForAttnam, PerfectForNewAttnam;

    for(uint c = 1; c < Continent.size(); ++c)
      if(Continent[c]->GetSize() > 200 && Continent[c]->GetSize() < 800
         && Continent[c]->GetGTerrainAmount(EGForestType)
         && Continent[c]->GetGTerrainAmount(SnowType))
        PerfectForAttnam.push_back(Continent[c]);

    if(!PerfectForAttnam.size())
      continue;

    v2 AttnamPos, ElpuriCavePos, NewAttnamPos, TunnelEntry, TunnelExit;
    truth Correct = false;
    continent* PetrusLikes;

    // Store this before we start making islands which have no continent number.
    NoIslandAltitudeBuffer = AltitudeBuffer;

    for(int c1 = 0; c1 < 25; ++c1)
    {
      game::BusyAnimation();
      PetrusLikes = PerfectForAttnam[RAND() % PerfectForAttnam.size()];
      AttnamPos = PetrusLikes->GetRandomMember(EGForestType);
      ElpuriCavePos = PetrusLikes->GetRandomMember(SnowType);

      for(int c2 = 1; c2 < 50; ++c2)
      {
        TunnelExit = PetrusLikes->GetMember(RAND() % PetrusLikes->GetSize());

        if(AttnamPos != TunnelExit && ElpuriCavePos != TunnelExit)
        {
          for(int d1 = 0; d1 < 8; ++d1)
          {
            v2 Pos = TunnelExit + game::GetMoveVector(d1);

            if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
            {
              int Distance = 3 + (RAND() & 3);
              truth Error = false;
              TunnelEntry = Pos;

              for(int c2 = 0; c2 < Distance; ++c2)
              {
                TunnelEntry += game::GetMoveVector(d1);

                if(!IsValidPos(TunnelEntry)
                   || AltitudeBuffer[TunnelEntry.X][TunnelEntry.Y] > 0)
                {
                  Error = true;
                  break;
                }
              }

              if(Error)
                continue;

              int x, y;
              int Counter = 0;

              for(x = TunnelEntry.X - 3; x <= TunnelEntry.X + 3; ++x)
              {
                for(y = TunnelEntry.Y - 3; y <= TunnelEntry.Y + 3;
                    ++y, ++Counter)
                  if(Counter != 0 && Counter != 6
                     && Counter != 42 && Counter != 48
                     && (!IsValidPos(x, y)
                         || AltitudeBuffer[x][y] > 0
                         || AltitudeBuffer[x][y] < -350))
                  {
                    Error = true;
                    break;
                  }

                if(Error)
                  break;
              }

              if(Error)
                continue;

              Error = true;

              for(x = 0; x < XSize; ++x)
                if(TypeBuffer[x][TunnelEntry.Y] == JungleType)
                {
                  Error = false;
                  break;
                }

              if(Error)
                continue;

              Counter = 0;

              for(x = TunnelEntry.X - 2; x <= TunnelEntry.X + 2; ++x)
                for(y = TunnelEntry.Y - 2; y <= TunnelEntry.Y + 2;
                    ++y, ++Counter)
                  if(Counter != 0 && Counter != 4
                     && Counter != 20 && Counter != 24)
                    AltitudeBuffer[x][y] /= 2;

              AltitudeBuffer[TunnelEntry.X][TunnelEntry.Y] = 1 + RAND() % 50;
              TypeBuffer[TunnelEntry.X][TunnelEntry.Y] = JungleType;
              GetWSquare(TunnelEntry)->ChangeGWTerrain(jungle::Spawn());
              int NewAttnamIndex;

              for(NewAttnamIndex = RAND() & 7;
                  NewAttnamIndex == 7 - d1;
                  NewAttnamIndex = RAND() & 7);

              NewAttnamPos = TunnelEntry
                             + game::GetMoveVector(NewAttnamIndex);
              static int DiagonalDir[4] = { 0, 2, 5, 7 };
              static int NotDiagonalDir[4] = { 1, 3, 4, 6 };
              static int AdjacentDir[4][2] = { { 0, 1 }, { 0, 2 },
                                               { 1, 3 }, { 2, 3 } };
              truth Raised[] = { false, false, false, false };
              int d2;

              for(d2 = 0; d2 < 4; ++d2)
                if(NotDiagonalDir[d2] != 7 - d1
                   && (NotDiagonalDir[d2] == NewAttnamIndex
                       || !(RAND() & 2)))
                {
                  v2 Pos = TunnelEntry
                           + game::GetMoveVector(NotDiagonalDir[d2]);
                  AltitudeBuffer[Pos.X][Pos.Y] = 1 + RAND() % 50;
                  TypeBuffer[Pos.X][Pos.Y] = JungleType;
                  GetWSquare(Pos)->ChangeGWTerrain(jungle::Spawn());
                  Raised[d2] = true;
                }

              for(d2 = 0; d2 < 4; ++d2)
                if(DiagonalDir[d2] != 7 - d1
                   && (DiagonalDir[d2] == NewAttnamIndex
                       || (Raised[AdjacentDir[d2][0]]
                           && Raised[AdjacentDir[d2][1]] && !(RAND() & 2))))
                {
                  v2 Pos = TunnelEntry
                           + game::GetMoveVector(DiagonalDir[d2]);
                  AltitudeBuffer[Pos.X][Pos.Y] = 1 + RAND() % 50;
                  TypeBuffer[Pos.X][Pos.Y] = JungleType;
                  GetWSquare(Pos)->ChangeGWTerrain(jungle::Spawn());
                }

              Correct = true;
              break;
            }
          }

          if(Correct)
            break;
        }
      }

      if(Correct)
        break;
    }

    if(!Correct)
      continue;

    AllocateGlobalPossibleLocations(XSize, YSize, 6, 10);

    // Create a vector of location data structures from the available locations
    std::vector <location> AvailableLocations;

    // Pick out all the locations above ground as valid candidate locations
    for(int x1 = 0; x1 < XSize; ++x1)
      for(int y1 = 0; y1 < YSize; ++y1)
        if((PossibleLocationBuffer[x1][y1] == true) && (NoIslandAltitudeBuffer[x1][y1] > 0))
        {
          AvailableLocations.push_back(location(v2(x1, y1), TypeBuffer[x1][y1], GetContinentUnder(v2(x1, y1))->GetIndex(), (AttnamPos - v2(x1, y1)).GetManhattanLength(), false));
          GetWSquare(v2(x1, y1))->ChangeOWTerrain(newattnam::Spawn());
        }

    ADD_MESSAGE("AvailableLocations was %d long", AvailableLocations.size());

    // Remove those positions that have already been taken up by core places, plus the origin. Theoretically, New Attnam and Tunnel Entry need not be checked.
    std::vector<v2> ForbiddenPositions = {v2(0, 0), NewAttnamPos, TunnelEntry, TunnelExit, AttnamPos, ElpuriCavePos};
    for(int i = 0; i < ForbiddenPositions.size(); i++)
    {
      AvailableLocations.erase(
        std::remove_if(
          AvailableLocations.begin(),
          AvailableLocations.end(),
          [ForbiddenPositions, i](location vec) -> truth {return vec.Position == ForbiddenPositions[i];}),
        AvailableLocations.end());
    }

    // Sort the vector of global available positions according to distance to attnam. Closest places are first.
    std::sort(AvailableLocations.begin(), AvailableLocations.end(), distancetoattnam());
    
    ADD_MESSAGE("AvailableLocations are %d long", AvailableLocations.size());

    // Make up a vector of locations from the script that need to be placed
    std::vector<place> ToBePlaced;
    // Pick out only the ones that can be generated, and get their native ground terrain type
    for(int Type = 1; Type < protocontainer<owterrain>::GetSize(); ++Type)
    {
      const owterrain::prototype* Proto = protocontainer<owterrain>::GetProto(Type);
      const owterrain::database*const* ConfigData = Proto->GetConfigData();
      int ConfigSize = Proto->GetConfigSize();

      for(int c = 0; c < ConfigSize; ++c)
      {
        const owterrain::database* DataBase = ConfigData[c];

        if(!DataBase->IsAbstract && DataBase->CanBeGenerated)
        {
          place ConfigID(Type, DataBase->Config, DataBase->NativeGTerrainType, false);
          ToBePlaced.push_back(ConfigID);
        }
      }
    }

    // Terrain type analytics:
    ADD_MESSAGE("ToBePlaced is %d long", ToBePlaced.size());
    ADD_MESSAGE("AvailableLocations is %d long", AvailableLocations.size());
    // Collect available terrain statistics
    int t1 = 0, s1 = 0, d1 = 0, e1 = 0, l1 = 0, g1 = 0, j1 = 0;
    for(int i = 0; i < AvailableLocations.size(); i++)
    {
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(TUNDRA))
        t1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(STEPPE))
        s1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(DESERT))
        d1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(EVERGREEN_FOREST))
        e1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(LEAFY_FOREST))
        l1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(GLACIER))
        g1++;
      if(AvailableLocations[i].GTerrainType == GetTypeOfNativeGTerrainType(JUNGLE))
        j1++;
    }
    ADD_MESSAGE("Total snow tiles is %d", t1);
    ADD_MESSAGE("Total steppe tiles is %d", s1);
    ADD_MESSAGE("Total desert tiles is %d", d1);
    ADD_MESSAGE("Total evergreen forest tiles is %d", e1);
    ADD_MESSAGE("Total leafy forest tiles is %d", l1);
    ADD_MESSAGE("Total glacier tiles is %d", g1);
    ADD_MESSAGE("Total jungle tiles is %d", j1);
    
    // Collect ToBePlaced terrain statistics
    int t2 = 0, s2 = 0, d2 = 0, e2 = 0, l2 = 0, g2 = 0, j2 = 0;
    for(int i = 0; i < ToBePlaced.size(); i++)
    {
      if(ToBePlaced[i].NativeGTerrainType == TUNDRA)
        t2++;
      if(ToBePlaced[i].NativeGTerrainType == STEPPE)
        s2++;
      if(ToBePlaced[i].NativeGTerrainType == DESERT)
        d2++;
      if(ToBePlaced[i].NativeGTerrainType == EVERGREEN_FOREST)
        e2++;
      if(ToBePlaced[i].NativeGTerrainType == LEAFY_FOREST)
        l2++;
      if(ToBePlaced[i].NativeGTerrainType == GLACIER)
        g2++;
      if(ToBePlaced[i].NativeGTerrainType == JUNGLE)
        j2++;
    }

    if((t1 < t2) || (s1 < s2) || (d1 < d2) || (e1 < e2) || (l1 < l2) || (g1 < g2) || (j1 < j2))
    {
      ADD_MESSAGE("Warning: Too few available terrains for locations to be placed...");
    }

    // Re-order the places so they appear in random order:
    std::random_shuffle(ToBePlaced.begin(), ToBePlaced.end());

    // Do this for as many times as there are number of continents.
    for(uint c = 1; c < Continent.size(); ++c)
    {
      // Get the next nearest continent index by looking at the top of the available positions.
      int ThisContinent = AvailableLocations[0].ContinentIndex;
      
      // Get each available position on the first continent.
      std::vector<location> AvailableLocationsOnThisContinent;
      
      // Collect all remaining available positions on this continent.
      for(int i = 0; i < AvailableLocations.size(); i++)
      {
        if(AvailableLocations[i].ContinentIndex == ThisContinent)
        {
          AvailableLocationsOnThisContinent.push_back(AvailableLocations[i]);
        }
      }
      // Go through all the locations on the continent. These are always in order of distance to Attnam, closest at the top.
      for(int i = 0; i < AvailableLocationsOnThisContinent.size(); i++)
      {
        // Go through all remaining places. These are always in a random order :)
        for(int j = 0; j < ToBePlaced.size(); j++)
        {
          // If the terrain type of the available location matches that of the place, then put the place there.
          if(AvailableLocationsOnThisContinent[i].GTerrainType == GetTypeOfNativeGTerrainType(ToBePlaced[j].NativeGTerrainType))
          {
            owterrain* NewPlace = protocontainer<owterrain>::GetProto(ToBePlaced[j].Type)->Spawn();
            v2 NewPos = AvailableLocationsOnThisContinent[i].Position;
            GetWSquare(NewPos)->ChangeOWTerrain(NewPlace);
            SetEntryPos(NewPlace->GetAttachedDungeon(), NewPos);
            ToBePlaced[j].HasBeenPlaced = true;
            RevealEnvironment(NewPos, 1);
            break;
          }
        }
        // Remove any places that have been placed, so we don't try to place them again.
        ToBePlaced.erase(
          std::remove_if(
            ToBePlaced.begin(),
            ToBePlaced.end(),
            [](place vec) -> truth {return vec.HasBeenPlaced;}),
          ToBePlaced.end());
      }
      // Remove the positions on the continent we have just examined. Whether or not they populate with places, they will not be re-visited.
      AvailableLocations.erase(
        std::remove_if(
          AvailableLocations.begin(),
          AvailableLocations.end(),
          [ThisContinent](location vec) -> truth {return vec.ContinentIndex == ThisContinent;}),
        AvailableLocations.end());

      AvailableLocationsOnThisContinent.clear();

      // Two early stopping criteria. In the default case we just run out of continents to step through.
      if(AvailableLocations.empty())
        break;

      if(ToBePlaced.empty())
        break;
    }

    if(!ToBePlaced.empty())
      ADD_MESSAGE("There were %d leftover places", ToBePlaced.size());// Too many places (Overflow)
    if(!AvailableLocations.empty())
      ADD_MESSAGE("There were %d leftover positions", AvailableLocations.size());// Too many locations (Underflow - not a critical case)

    GetWSquare(AttnamPos)->ChangeOWTerrain(attnam::Spawn());
    SetEntryPos(ATTNAM, AttnamPos);
    RevealEnvironment(AttnamPos, 1);
    GetWSquare(NewAttnamPos)->ChangeOWTerrain(newattnam::Spawn());
    SetEntryPos(NEW_ATTNAM, NewAttnamPos);
    SetEntryPos(ELPURI_CAVE, ElpuriCavePos);
    GetWSquare(TunnelEntry)->ChangeOWTerrain(underwatertunnel::Spawn());
    SetEntryPos(UNDER_WATER_TUNNEL, TunnelEntry);
    GetWSquare(TunnelExit)->ChangeOWTerrain(underwatertunnelexit::Spawn());
    SetEntryPos(UNDER_WATER_TUNNEL_EXIT, TunnelExit);
    PLAYER->PutTo(NewAttnamPos);
    CalculateLuminances();
    CalculateNeighbourBitmapPoses();
    break;
  }

  delete [] OldAltitudeBuffer;
  delete [] OldTypeBuffer;
}

void worldmap::RandomizeAltitude()
{
  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      AltitudeBuffer[x][y] = 4000 - RAND() % 8000;
}

void worldmap::SmoothAltitude()
{
  for(int c = 0; c < 10; ++c)
  {
    game::BusyAnimation();
    int x, y;

    for(y = 0; y < YSize; ++y)
      SafeSmooth(0, y);

    for(x = 1; x < XSize - 1; ++x)
    {
      SafeSmooth(x, 0);

      for(y = 1; y < YSize - 1; ++y)
        FastSmooth(x, y);

      SafeSmooth(x, YSize - 1);
    }

    for(y = 0; y < YSize; ++y)
      SafeSmooth(XSize - 1, y);
  }
}

void worldmap::FastSmooth(int x, int y)
{
  long HeightNear = 0;
  int d;

  for(d = 0; d < 4; ++d)
    HeightNear += OldAltitudeBuffer[x + DirX[d]][y + DirY[d]];

  for(d = 4; d < 8; ++d)
    HeightNear += AltitudeBuffer[x + DirX[d]][y + DirY[d]];

  OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
  AltitudeBuffer[x][y] = HeightNear >> 3;
}

void worldmap::SafeSmooth(int x, int y)
{
  long HeightNear = 0;
  int d, SquaresNear = 0;

  for(d = 0; d < 4; ++d)
  {
    int X = x + DirX[d];
    int Y = y + DirY[d];

    if(IsValidPos(X, Y))
    {
      HeightNear += OldAltitudeBuffer[X][Y];
      ++SquaresNear;
    }
  }

  for(d = 4; d < 8; ++d)
  {
    int X = x + DirX[d];
    int Y = y + DirY[d];

    if(IsValidPos(X, Y))
    {
      HeightNear += AltitudeBuffer[X][Y];
      ++SquaresNear;
    }
  }

  OldAltitudeBuffer[x][y] = AltitudeBuffer[x][y];
  AltitudeBuffer[x][y] = HeightNear / SquaresNear;
}

void worldmap::GenerateClimate()
{
  game::BusyAnimation();

  for(int y = 0; y < YSize; ++y)
  {
    double DistanceFromEquator = fabs(double(y) / YSize - 0.5);
    truth LatitudeRainy = DistanceFromEquator <= 0.05
                          || (DistanceFromEquator > 0.25
                              && DistanceFromEquator <= 0.45);

    for(int x = 0; x < XSize; ++x)
    {
      if(AltitudeBuffer[x][y] <= 0)
      {
        TypeBuffer[x][y] = OceanType;
        continue;
      }

      truth Rainy = LatitudeRainy;

      if(!Rainy)
        for(int d = 0; d < 8; ++d)
        {
          v2 Pos = v2(x, y) + game::GetMoveVector(d);

          if(IsValidPos(Pos) && AltitudeBuffer[Pos.X][Pos.Y] <= 0)
          {
            Rainy = true;
            break;
          }
        }

      int Temperature = int(MAX_TEMPERATURE
                            - DistanceFromEquator * LATITUDE_EFFECT
                            - AltitudeBuffer[x][y] * ALTITUDE_EFFECT);
      int Type = 0;

      if(Temperature <= COLD)
        Type = Rainy ? SnowType : GlacierType;
      else if(Temperature <= MEDIUM)
        Type = Rainy ? EGForestType : SnowType;
      else if(Temperature <= WARM)
        Type = Rainy ? LForestType : SteppeType;
      else if(Temperature <= HOT)
        Type = Rainy ? LForestType : DesertType;
      else
        Type = Rainy ? JungleType : DesertType;

      TypeBuffer[x][y] = Type;
    }
  }
}

void worldmap::SmoothClimate()
{
  for(int c = 0; c < 3; ++c)
  {
    game::BusyAnimation();

    for(int x = 0; x < XSize; ++x)
      for(int y = 0; y < YSize; ++y)
        if((OldTypeBuffer[x][y] = TypeBuffer[x][y]) != OceanType)
          TypeBuffer[x][y] = WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(x, y);
  }

  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      Map[x][y]->ChangeGWTerrain
        (protocontainer<gwterrain>::GetProto(TypeBuffer[x][y])->Spawn());
}

/* Evil... */

#define ANALYZE_TYPE(type)\
{\
  int T = type;\
  \
  for(c = 0; c < u; ++c)\
    if(T == UsedType[c])\
      {\
        ++TypeAmount[c];\
        break;\
      }\
  \
  if(c == u)\
    {\
      UsedType[u] = T;\
      TypeAmount[u++] = 1;\
    }\
}

int worldmap::WhatTerrainIsMostCommonAroundCurrentTerritorySquareIncludingTheSquareItself(int x, int y)
{
  int UsedType[9];
  int TypeAmount[9];
  int c, d, u = 1;

  UsedType[0] = TypeBuffer[x][y];
  TypeAmount[0] = 1;

  for(d = 0; d < 4; ++d)
  {
    int X = x + DirX[d];
    int Y = y + DirY[d];

    if(IsValidPos(X, Y))
      ANALYZE_TYPE(OldTypeBuffer[X][Y]);
  }

  for(d = 4; d < 8; ++d)
  {
    int X = x + DirX[d];
    int Y = y + DirY[d];

    if(IsValidPos(X, Y))
      ANALYZE_TYPE(TypeBuffer[X][Y]);
  }

  int MostCommon = 0;

  for(c = 1; c < u; ++c)
    if(TypeAmount[c] > TypeAmount[MostCommon] && UsedType[c] != OceanType)
      MostCommon = c;

  return UsedType[MostCommon];
}

void worldmap::CalculateContinents()
{
  uint c;

  for(c = 1; c < Continent.size(); ++c)
    delete Continent[c];

  Continent.resize(1, 0);
  memset(ContinentBuffer[0], 0, XSizeTimesYSize * sizeof(uchar));
  game::BusyAnimation();

  for(int x = 0; x < XSize; ++x)
    for(int y = 0; y < YSize; ++y)
      if(AltitudeBuffer[x][y] > 0)
      {
        truth Attached = false;

        for(int d = 0; d < 8; ++d)
        {
          v2 Pos = v2(x, y) + game::GetMoveVector(d);

          if(IsValidPos(Pos))
          {
            cint NearCont = ContinentBuffer[Pos.X][Pos.Y];

            if(NearCont)
            {
              cint ThisCont = ContinentBuffer[x][y];

              if(ThisCont)
              {
                if(ThisCont != NearCont)
                {
                  if(Continent[ThisCont]->GetSize()
                     < Continent[NearCont]->GetSize())
                    Continent[ThisCont]->AttachTo(Continent[NearCont]);
                  else
                    Continent[NearCont]->AttachTo(Continent[ThisCont]);
                }
              }
              else
                Continent[NearCont]->Add(v2(x, y));

              Attached = true;
            }
          }
        }

        if(!Attached)
        {
          if(Continent.size() == 255)
          {
            RemoveEmptyContinents();

            if(Continent.size() == 255)
              ABORT("Valpurus shall not carry more continents!");
          }

          continent* NewContinent = new continent(Continent.size());
          NewContinent->Add(v2(x, y));
          Continent.push_back(NewContinent);
        }
      }

  RemoveEmptyContinents();

  for(c = 1; c < Continent.size(); ++c)
    Continent[c]->GenerateInfo();
}

void worldmap::RemoveEmptyContinents()
{
  for(uint c = 1; c < Continent.size(); ++c)
    if(!Continent[c]->GetSize())
    {
      for(uint i = Continent.size() - 1; i >= c; i--)
        if(Continent[i]->GetSize())
        {
          Continent[i]->AttachTo(Continent[c]);
          delete Continent[i];
          Continent.pop_back();
          break;
        }
        else
        {
          delete Continent[i];
          Continent.pop_back();
        }
    }
}

void worldmap::Draw(truth) const
{
  cint XMin = Max(game::GetCamera().X, 0);
  cint YMin = Max(game::GetCamera().Y, 0);
  cint XMax = Min(XSize, game::GetCamera().X + game::GetScreenXSize());
  cint YMax = Min(YSize, game::GetCamera().Y + game::GetScreenYSize());
  blitdata BlitData = { DOUBLE_BUFFER,
                        { 0, 0 },
                        { 0, 0 },
                        { TILE_SIZE, TILE_SIZE },
                        { 0 },
                        TRANSPARENT_COLOR,
                        ALLOW_ANIMATE|ALLOW_ALPHA };

  if(!game::GetSeeWholeMapCheatMode())
  {
    for(int x = XMin; x < XMax; ++x)
    {
      BlitData.Dest = game::CalculateScreenCoordinates(v2(x, YMin));
      wsquare** Square = &Map[x][YMin];

      for(int y = YMin; y < YMax;
          ++y, ++Square, BlitData.Dest.Y += TILE_SIZE)
        if((*Square)->LastSeen)
          (*Square)->Draw(BlitData);
    }
  }
  else
  {
    for(int x = XMin; x < XMax; ++x)
    {
      BlitData.Dest = game::CalculateScreenCoordinates(v2(x, YMin));
      wsquare** Square = &Map[x][YMin];

      for(int y = YMin; y < YMax;
          ++y, ++Square, BlitData.Dest.Y += TILE_SIZE)
        (*Square)->Draw(BlitData);
    }
  }
}

void worldmap::CalculateLuminances()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->CalculateLuminance();
}

void worldmap::CalculateNeighbourBitmapPoses()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    Map[0][c]->GetGWTerrain()->CalculateNeighbourBitmapPoses();
}

wsquare* worldmap::GetNeighbourWSquare(v2 Pos, int I) const
{
  Pos += game::GetMoveVector(I);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void worldmap::RevealEnvironment(v2 Pos, int Radius)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      Map[x][y]->SignalSeen();
}

outputfile& operator<<(outputfile& SaveFile, const worldmap* WorldMap)
{
  WorldMap->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, worldmap*& WorldMap)
{
  WorldMap = new worldmap;
  WorldMap->Load(SaveFile);
  return SaveFile;
}

void worldmap::UpdateLOS()
{
  game::RemoveLOSUpdateRequest();
  int Radius = PLAYER->GetLOSRange();
  long RadiusSquare = Radius * Radius;
  v2 Pos = PLAYER->GetPos();
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
      if(HypotSquare(Pos.X - x, Pos.Y - y) <= RadiusSquare)
        Map[x][y]->SignalSeen();
}

void worldmap::AllocateGlobalPossibleLocations(int XSize, int YSize, int Radius, int TestPoints)
{
  //maybe make a struct called PoissonDiskSampler with all these initializations in it

  long RadiusSquared = Radius * Radius;
  long A = 3*RadiusSquared;
  double CellSize = Radius / sqrt(2);

  // Grid cell width and height
  int GridCellWidth = int(XSize / CellSize);
  int GridCellHeight = int(YSize / CellSize);
  
  bool CheckDistance = true;

  // Allocate a grid and a queue
  memset(PossibleLocationBuffer[0], 0, XSizeTimesYSize * sizeof(uchar)); // replace with a vector type in future?
  std::vector<v2> Grid(GridCellWidth*GridCellHeight, v2(0, 0));

  std::vector<v2> Queue;

  int QueueSize = 0;
  int SampleSize = 0;

  //RVS function
  //initial random point
  int XPos = RAND() % XSize;
  int YPos = RAND() % YSize;
  
  // Do the SetPoint function
  v2 Sample = v2(XPos, YPos);
  Queue.push_back(Sample);
  // Find where (x,y) sits in the grid
  int XIndex = int(XPos / CellSize);
  int YIndex = int(YPos / CellSize);
  int Step = GridCellWidth*YIndex + XIndex;

  Grid[Step] = Sample;
  QueueSize += 1;
  SampleSize += 1;

  while(QueueSize)
  {
    int XIdx = int((RAND() % 100 / 100.0) * QueueSize);
    Sample = Queue[XIdx];
    for(int YIdx = 0; YIdx < TestPoints; YIdx++)
    {
      double Angle = 2 * FPI * (RAND() % 100) / 100.0;
      double Hypotenuse = sqrt( A * (RAND() % 100) / 100.0 + RadiusSquared);
      int XPos = int(Sample.X + Hypotenuse*cos(Angle));
      int YPos = int(Sample.Y + Hypotenuse*sin(Angle));
      if((XPos >= 0) && (XPos < XSize))
      {
        if((YPos >= 0) && (YPos < YSize))
        {
          // Find where (x,y) sits in the grid
          int x_idx = int(XPos / CellSize);
          int y_idx = int(YPos / CellSize);
          // Determine a neighborhood of cells around (x,y)
          int x0 = Max(x_idx - 2, 0);
          int y0 = Max(y_idx - 2, 0);
          int x1 = Max(x_idx - 3, GridCellWidth);
          int y1 = Max(y_idx - 3, GridCellHeight);
          // Search around (x,y)
          for(int i = Min(y0, y1); i < Max(y0, y1); i++)
          {
            for(int j = Min(x0, x1); j < Max(x0, x1); j++)
            {
              int Step1 = i*GridCellWidth + j;
              // If the sample point exists on the grid
              if((Grid[Step1] != v2(0, 0)))
              {
                v2 Sample1 = Grid[Step1];
                int dx = (Sample1.X - XPos)*(Sample1.X - XPos);
                int dy = (Sample1.Y - YPos)*(Sample1.Y - YPos);
                // If the sample is too close 
                if((dx + dy) < RadiusSquared)
                {
                  CheckDistance = false;
                  goto here; // get rid of this goto!
                }
              }
            }
          }
          CheckDistance = true;
          here:
          if(CheckDistance)
          {
            // Do the SetPoint function
            v2 Sample2 = v2(XPos, YPos);
            Queue.push_back(Sample2);
            // find where (x,y) sits in the grid
            int XIndex2 = int(XPos / CellSize);
            int YIndex2 = int(YPos / CellSize);
            int Step2 = GridCellWidth*YIndex2 + XIndex2;
            Grid[Step2] = Sample2;
            QueueSize += 1;
            SampleSize += 1;
          }
        }
      }
    }
    Queue.erase(Queue.begin() + XIdx);
    //Queue.pop_back(); //was: delete self.queue[XIdx] //??
    //delete Queue[XIdx]; //?? does this work as expected?
    QueueSize -= 1;
    if(QueueSize > 1000)
      ABORT("Overfull QueueSize");
  }
  
  for(int c = 0; c < Grid.size(); ++c)
    PossibleLocationBuffer[Grid[c].X][Grid[c].Y] = true;
/*
  ADD_MESSAGE("Grid size is %d long", Grid.size());
  ADD_MESSAGE("Sample size is %d long", SampleSize);
*/
}

int worldmap::GetTypeOfNativeGTerrainType(int Type) const
{
  int Return;

  switch(Type)
  {
   case 0:
    Return = ocean::ProtoType.GetIndex();
    break;
   case DESERT:
    Return = desert::ProtoType.GetIndex();
    break;
   case JUNGLE:
    Return = jungle::ProtoType.GetIndex();
    break;
   case STEPPE:
    Return = steppe::ProtoType.GetIndex();
    break;
   case LEAFY_FOREST:
    Return = leafyforest::ProtoType.GetIndex();
    break;
   case EVERGREEN_FOREST:
    Return = evergreenforest::ProtoType.GetIndex();
    break;
   case TUNDRA:
    Return = snow::ProtoType.GetIndex();
    break;
   case GLACIER:
    Return = glacier::ProtoType.GetIndex();
    break;
   default:
    ABORT("You are a pest. Please stop creating wterrains that are stupid.");
  }

  return Return;
}
