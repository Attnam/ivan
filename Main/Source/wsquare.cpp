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

wsquare::wsquare(worldmap* WorldMapUnder, v2 Pos)
: square(WorldMapUnder, Pos), GWTerrain(0), OWTerrain(0) { }

wsquare::~wsquare()
{
  delete GWTerrain;
  delete OWTerrain;
}

void wsquare::Save(outputfile& SaveFile) const
{
  square::Save(SaveFile);
  SaveFile << GWTerrain << OWTerrain;
  SaveFile.Put(!!LastSeen);
}

void wsquare::Load(inputfile& SaveFile)
{
  square::Load(SaveFile);
  SaveFile >> GWTerrain >> OWTerrain;
  LastSeen = SaveFile.Get();
  CalculateLuminance();
}

void wsquare::Draw(blitdata& BlitData)
{
  if(Flags & NEW_DRAW_REQUEST || AnimatedEntities)
  {
    BlitData.Luminance = ivanconfig::ApplyContrastTo(Luminance);
    GWTerrain->Draw(BlitData);

    if(OWTerrain)
      OWTerrain->Draw(BlitData);

    if(Character && Character->CanBeSeenByPlayer())
    {
      BlitData.Luminance = ivanconfig::GetContrastLuminance();
      BlitData.CustomData = Character->GetSquareIndex(Pos)
			    |ALLOW_ANIMATE|ALLOW_ALPHA;
      Character->Draw(BlitData);
    }

    Flags &= ~STRONG_NEW_DRAW_REQUEST;
  }
}

void wsquare::ChangeWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  ChangeGWTerrain(NewGround);
  ChangeOWTerrain(NewOver);
}

void wsquare::ChangeGWTerrain(gwterrain* NewGround)
{
  if(GWTerrain->IsAnimated())
    DecStaticAnimatedEntities();

  delete GWTerrain;
  SetGWTerrain(NewGround);
  Flags |= DESCRIPTION_CHANGE|NEW_DRAW_REQUEST;
}

void wsquare::ChangeOWTerrain(owterrain* NewOver)
{
  if(OWTerrain && OWTerrain->IsAnimated())
    DecStaticAnimatedEntities();

  delete OWTerrain;
  SetOWTerrain(NewOver);
  Flags |= DESCRIPTION_CHANGE|NEW_DRAW_REQUEST;
}

void wsquare::SetWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  SetGWTerrain(NewGround);
  SetOWTerrain(NewOver);
}

void wsquare::SetGWTerrain(gwterrain* What)
{
  GWTerrain = What;

  if(What)
  {
    What->SetWSquareUnder(this);

    if(What->IsAnimated())
      IncStaticAnimatedEntities();
  }
}

void wsquare::SetOWTerrain(owterrain* What)
{
  OWTerrain = What;

  if(What)
  {
    What->SetWSquareUnder(this);

    if(What->IsAnimated())
      IncStaticAnimatedEntities();
  }
}

void wsquare::UpdateMemorizedDescription(truth Cheat)
{
  if(Flags & DESCRIPTION_CHANGE || Cheat)
  {
    MemorizedDescription.Empty();

    if(OWTerrain)
    {
      OWTerrain->AddName(MemorizedDescription, INDEFINITE);
      MemorizedDescription << " surrounded by ";
      GWTerrain->AddName(MemorizedDescription, UNARTICLED);
    }
    else
      GWTerrain->AddName(MemorizedDescription, UNARTICLED);

    if(Cheat)
    {
      festring Continent;

      if(GetWorldMap()->GetContinentUnder(Pos))
	Continent << ", continent "
		  << GetWorldMap()->GetContinentUnder(Pos)->GetName();

      MemorizedDescription << " (pos " << Pos.X << ':' << Pos.Y
			   << Continent << ", height "
			   << GetWorldMap()->GetAltitude(Pos) << " m)";
    }

    Flags &= ~DESCRIPTION_CHANGE;
  }
}

gterrain* wsquare::GetGTerrain() const
{
  return GWTerrain;
}

oterrain* wsquare::GetOTerrain() const
{
  return OWTerrain;
}

truth wsquare::SignalSeen()
{
  UpdateMemorizedDescription();
  LastSeen = 1;
  return true;
}

void wsquare::CalculateLuminance()
{
  double T = log(1. + fabs(GetWorldMap()->GetAltitude(Pos)) / 500.);
  int Element = Min((128 - int(37.5 * T)), 255);
  Luminance = MakeRGB24(Element, Element, Element);
}

int wsquare::GetWalkability() const
{
  return (OWTerrain
	  ? OWTerrain->GetWalkability() & GWTerrain->GetWalkability()
	  : GWTerrain->GetWalkability());
}

truth wsquare::CanBeSeenByPlayer(truth) const
{
  return LastSeen;
}

truth wsquare::CanBeSeenFrom(v2 FromPos, long MaxDistance, truth) const
{
  return (Pos - FromPos).GetLengthSquare() <= MaxDistance;
}
