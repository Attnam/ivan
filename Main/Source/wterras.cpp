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

cchar* ocean::GetNameStem() const { return "ocean"; }
v2 ocean::GetBitmapPos(int Frame) const
{ return v2(48 + ((Frame << 3)&~8), 48); }
cchar* ocean::SurviveMessage() const
{ return "you manage to reach the shore"; }
cchar* ocean::MonsterSurviveMessage() const
{ return "manages to reach the shore"; }
cchar* ocean::DeathMessage() const { return "you drown"; }
cchar* ocean::MonsterDeathVerb() const { return "drowns"; }
cchar* ocean::ScoreEntry() const { return "drowned"; }

cchar* glacier::GetNameStem() const { return "glacier"; }

cchar* desert::GetNameStem() const { return "desert"; }

cchar* snow::GetNameStem() const { return "tundra"; }

cchar* jungle::GetNameStem() const { return "jungle"; }

cchar* leafyforest::GetNameStem() const { return "leafy forest"; }

cchar* evergreenforest::GetNameStem() const
{ return "evergreen forest"; }

cchar* steppe::GetNameStem() const { return "steppe"; }

cchar* attnam::GetNameStem() const
{ return "migthy cathedral reaching the clouds"; }
int attnam::GetAttachedDungeon() const { return ATTNAM; }

cchar* elpuricave::GetNameStem() const
{ return "hideous cave entry radiating pure evil"; }
int elpuricave::GetAttachedDungeon() const { return ELPURI_CAVE; }

cchar* newattnam::GetNameStem() const { return "primitive village"; }
int newattnam::GetAttachedDungeon() const { return NEW_ATTNAM; }

cchar* underwatertunnel::GetNameStem() const
{ return "entrance to an underwater tunnel"; }
int underwatertunnel::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

cchar* underwatertunnelexit::GetNameStem() const
{ return "exit from an underwater tunnel"; }
int underwatertunnelexit::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

int ocean::GetWalkability() const { return ANY_MOVE&~WALK; }
