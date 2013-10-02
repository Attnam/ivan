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

gwterrainprototype::gwterrainprototype(gwterrainspawner Spawner,
				       cchar* ClassID)
: Spawner(Spawner), ClassID(ClassID)
{ Index = protocontainer<gwterrain>::Add(this); }
owterrainprototype::owterrainprototype(owterrainspawner Spawner,
				       cchar* ClassID)
: Spawner(Spawner), ClassID(ClassID)
{ Index = protocontainer<owterrain>::Add(this); }

int gwterrain::GetWalkability() const { return ANY_MOVE&~SWIM; }
int owterrain::GetWalkability() const { return ANY_MOVE; }
int owterrain::GetAttachedEntry() const { return STAIRS_UP; }

void wterrain::AddName(festring& String, int Case) const
{
  if(!(Case & PLURAL))
    if(!(Case & ARTICLE_BIT))
      String << GetNameStem();
    else
      if(!(Case & INDEFINE_BIT))
	String << "the " << GetNameStem();
      else
	String << (UsesLongArticle() ? "an " : "a ") << GetNameStem();
  else
    if(!(Case & ARTICLE_BIT))
      String << GetNameStem() << " terrains";
    else
      if(!(Case & INDEFINE_BIT))
	String << "the " << GetNameStem() << " terrains";
      else
	String << GetNameStem() << " terrains";
}

festring wterrain::GetName(int Case) const
{
  static festring Name;
  Name.Empty();
  AddName(Name, Case);
  return Name;
}

void gwterrain::Draw(blitdata& BlitData) const
{
  cint AF = AnimationFrames;
  cint F = !(BlitData.CustomData & ALLOW_ANIMATE) || AF == 1
		? 0 : GET_TICK() & (AF - 1);
  BlitData.Src = GetBitmapPos(F);
  igraph::GetWTerrainGraphic()->LuminanceBlit(BlitData);

  for(int c = 0; c < 8 && Neighbour[c].second; ++c)
  {
    BlitData.Src = Neighbour[c].first;
    igraph::GetWTerrainGraphic()->LuminanceMaskedBlit(BlitData);
  }

  BlitData.Src.X = BlitData.Src.Y = 0;
}

void owterrain::Draw(blitdata& BlitData) const
{
  cint AF = AnimationFrames;
  cint F = !(BlitData.CustomData & ALLOW_ANIMATE) || AF == 1
		? 0 : GET_TICK() & (AF - 1);
  BlitData.Src = GetBitmapPos(F);
  igraph::GetWTerrainGraphic()->LuminanceMaskedBlit(BlitData);
  BlitData.Src.X = BlitData.Src.Y = 0;
}

void wterrain::Load(inputfile&)
{
  WSquareUnder = (wsquare*)game::GetSquareInLoad();
}

void gwterrain::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
}

void owterrain::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
}

gwterrain* gwterrainprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  gwterrain* Terrain = Spawner();
  Terrain->Load(SaveFile);
  return Terrain;
}

owterrain* owterrainprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  owterrain* Terrain = Spawner();
  Terrain->Load(SaveFile);
  return Terrain;
}

truth DrawOrderer(const std::pair<v2, int>& Pair1,
		  const std::pair<v2, int>& Pair2)
{
  return Pair1.second < Pair2.second;
}

void gwterrain::CalculateNeighbourBitmapPoses()
{
  int Index = 0;
  v2 Pos = GetPos();
  worldmap* WorldMap = GetWorldMap();
  int Priority = GetPriority();

  for(int d = 0; d < 8; ++d)
  {
    wsquare* NeighbourSquare = WorldMap->GetNeighbourWSquare(Pos, d);

    if(NeighbourSquare)
    {
      gwterrain* DoNeighbour = NeighbourSquare->GetGWTerrain();
      int NeighbourPriority = DoNeighbour->GetPriority();

      if(NeighbourPriority > Priority)
      {
	Neighbour[Index].first = DoNeighbour->GetBitmapPos(0)
				 - (game::GetMoveVector(d) << 4);
	Neighbour[Index].second = NeighbourPriority;
	++Index;
      }
    }
  }

  std::sort(Neighbour, Neighbour + Index, DrawOrderer);

  if(Index < 8)
    Neighbour[Index].second = 0;
}

truth owterrain::Enter(truth DirectionUp) const
{
  if(DirectionUp)
  {
    if(!PLAYER->IsFlying())
      ADD_MESSAGE("You jump into the air. For some "
		  "reason you don't get too far above.");
    else
      ADD_MESSAGE("You fly around for some time.");

    return false;
  }

  return game::TryTravel(GetAttachedDungeon(),
			 GetAttachedArea(),
			 GetAttachedEntry());
}
