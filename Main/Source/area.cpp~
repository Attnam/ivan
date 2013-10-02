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

/* Compiled through areaset.cpp */

area::area() { }

area::area(int InitXSize, int InitYSize)
{
  Initialize(InitXSize, InitYSize);
}

void area::Initialize(int InitXSize, int InitYSize)
{
  XSize = InitXSize;
  YSize = InitYSize;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  memset(FlagMap[0], 0, XSizeTimesYSize * sizeof(uchar));
}

area::~area()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    delete Map[0][c];

  delete [] Map;
  delete [] FlagMap;
}

void area::Save(outputfile& SaveFile) const
{
  SaveFile << XSize << YSize << EntryMap;
  SaveFile.Write(reinterpret_cast<char*>(FlagMap[0]), XSizeTimesYSize * sizeof(uchar));
}

void area::Load(inputfile& SaveFile)
{
  game::SetAreaInLoad(this);
  SaveFile >> XSize >> YSize;

  if(SaveFile.Eof())
    ABORT("Area file %s corrupted!", SaveFile.GetFileName().CStr());

  SaveFile >> EntryMap;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(FlagMap[0]), XSizeTimesYSize * sizeof(uchar));
}

void area::SendNewDrawRequest()
{
  cint XMin = Max(game::GetCamera().X, 0);
  cint YMin = Max(game::GetCamera().Y, 0);
  cint XMax = Min(XSize, game::GetCamera().X + game::GetScreenXSize());
  cint YMax = Min(YSize, game::GetCamera().Y + game::GetScreenYSize());

  for(int x = XMin; x < XSize && x < XMax; ++x)
    for(int y = YMin; y < YMax; ++y)
      Map[x][y]->SendStrongNewDrawRequest();

  igraph::GetBackGround()->FastBlit(DOUBLE_BUFFER);
  DOUBLE_BUFFER->DrawRectangle(14, 30, 17 + (game::GetScreenXSize() << 4), 33 + (game::GetScreenYSize() << 4), DARK_GRAY, true);
  DOUBLE_BUFFER->Fill(16, 32, game::GetScreenXSize() << 4, game::GetScreenYSize() << 4, BLACK);
}

square* area::GetNeighbourSquare(v2 Pos, int I) const
{
  Pos += game::GetMoveVector(I);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void area::SetEntryPos(int I, v2 Pos)
{
  EntryMap.insert(std::make_pair(I, Pos));
}
