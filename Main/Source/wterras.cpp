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
v2 glacier::GetBitmapPos(int) const { return v2(16, 16); }

cchar* desert::GetNameStem() const { return "desert"; }
v2 desert::GetBitmapPos(int) const { return v2(64, 16); }

cchar* snow::GetNameStem() const { return "tundra"; }
v2 snow::GetBitmapPos(int) const { return v2(112, 16); }

cchar* jungle::GetNameStem() const { return "jungle"; }
v2 jungle::GetBitmapPos(int) const { return v2(208, 16); }

cchar* leafyforest::GetNameStem() const { return "leafy forest"; }
v2 leafyforest::GetBitmapPos(int) const { return v2(304, 16); }

cchar* evergreenforest::GetNameStem() const
{ return "evergreen forest"; }
v2 evergreenforest::GetBitmapPos(int) const { return v2(352, 16); }

cchar* steppe::GetNameStem() const { return "steppe"; }
v2 steppe::GetBitmapPos(int) const { return v2(160, 16); }

cchar* attnam::GetNameStem() const
{ return "migthy cathedral reaching the clouds"; }
v2 attnam::GetBitmapPos(int) const { return v2(0, 48); }
int attnam::GetAttachedDungeon() const { return ATTNAM; }

cchar* elpuricave::GetNameStem() const
{ return "hideous cave entry radiating pure evil"; }
v2 elpuricave::GetBitmapPos(int) const { return v2(16, 48); }
int elpuricave::GetAttachedDungeon() const { return ELPURI_CAVE; }

cchar* newattnam::GetNameStem() const { return "primitive village"; }
v2 newattnam::GetBitmapPos(int) const { return v2(16, 64); }
int newattnam::GetAttachedDungeon() const { return NEW_ATTNAM; }

cchar* underwatertunnel::GetNameStem() const
{ return "entrance to an underwater tunnel"; }
v2 underwatertunnel::GetBitmapPos(int) const { return v2(32, 64); }
int underwatertunnel::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

cchar* underwatertunnelexit::GetNameStem() const
{ return "exit from an underwater tunnel"; }
v2 underwatertunnelexit::GetBitmapPos(int) const { return v2(32, 64); }
int underwatertunnelexit::GetAttachedDungeon() const
{ return UNDER_WATER_TUNNEL; }

int ocean::GetWalkability() const { return ANY_MOVE&~WALK; }
