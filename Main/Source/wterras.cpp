#include <materias.h>
#include <confdef.h>

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

v2 ocean::GetBitmapPos(int Frame) const
{ return v2(48 + ((Frame << 3)&~8), 48); }
cchar* ocean::SurviveMessage() const
{ return "you manage to reach the shore"; }
cchar* ocean::MonsterSurviveMessage() const
{ return "manages to reach the shore"; }
cchar* ocean::DeathMessage() const { return "you drown"; }
cchar* ocean::MonsterDeathVerb() const { return "drowns"; }
cchar* ocean::ScoreEntry() const { return "drowned"; }

int ocean::GetWalkability() const { return ANY_MOVE&~WALK; }

truth ocean::DipInto(item* ToBeDipped, character* Who)
{
  auto brine = liquid::Spawn(BRINE);
  ToBeDipped->DipInto(brine->SpawnMoreLiquid(ToBeDipped->GetDefaultSecondaryVolume()), Who);
  delete brine;
  return true;
}
