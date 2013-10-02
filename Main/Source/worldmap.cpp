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

#define MAX_TEMPERATURE	  27		//increase for a warmer world
#define LATITUDE_EFFECT	  40		//increase for more effect
#define ALTITUDE_EFFECT	  0.02

#define COLD		  10
#define MEDIUM		  12
#define WARM		  17
#define HOT		  19

int DirX[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int DirY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

worldmap::worldmap() { }
continent* worldmap::GetContinentUnder(v2 Pos) const
{ return Continent[ContinentBuffer[Pos.X][Pos.Y]]; }
v2 worldmap::GetEntryPos(ccharacter*, int I) const
{ return EntryMap.find(I)->second; }
continent* worldmap::GetContinent(int I) const { return Continent[I]; }
int worldmap::GetAltitude(v2 Pos) { return AltitudeBuffer[Pos.X][Pos.Y]; }
charactervector& worldmap::GetPlayerGroup() { return PlayerGroup; }
character* worldmap::GetPlayerGroupMember(int c) { return PlayerGroup[c]; }

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
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
}

worldmap::~worldmap()
{
  delete [] TypeBuffer;
  delete [] AltitudeBuffer;
  delete [] ContinentBuffer;

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
  SaveFile.Read(reinterpret_cast<char*>(TypeBuffer[0]),
		XSizeTimesYSize * sizeof(uchar));
  SaveFile.Read(reinterpret_cast<char*>(AltitudeBuffer[0]),
		XSizeTimesYSize * sizeof(short));
  SaveFile.Read(reinterpret_cast<char*>(ContinentBuffer[0]),
		XSizeTimesYSize * sizeof(uchar));
  continent::TypeBuffer = TypeBuffer;
  continent::AltitudeBuffer = AltitudeBuffer;
  continent::ContinentBuffer = ContinentBuffer;
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

  for(;;)
  {
    RandomizeAltitude();
    SmoothAltitude();
    GenerateClimate();
    SmoothClimate();
    CalculateContinents();
    std::vector<continent*> PerfectForAttnam, PerfectForNewAttnam;

    for(uint c = 1; c < Continent.size(); ++c)
      if(Continent[c]->GetSize() > 25 && Continent[c]->GetSize() < 1000
	 && Continent[c]->GetGTerrainAmount(EGForestType)
	 && Continent[c]->GetGTerrainAmount(SnowType))
	PerfectForAttnam.push_back(Continent[c]);

    if(!PerfectForAttnam.size())
      continue;

    v2 AttnamPos, ElpuriCavePos, NewAttnamPos, TunnelEntry, TunnelExit;
    truth Correct = false;
    continent* PetrusLikes;

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
		  if(Continent[ThisCont]->GetSize()
		     < Continent[NearCont]->GetSize())
		    Continent[ThisCont]->AttachTo(Continent[NearCont]);
		  else
		    Continent[NearCont]->AttachTo(Continent[ThisCont]);
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
      if(long(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	Map[x][y]->SignalSeen();
}
