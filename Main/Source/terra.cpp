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

cchar* terrain::SurviveMessage() const { return "somehow you survive"; }
cchar* terrain::MonsterSurviveMessage() const { return "somehow survives"; }
cchar* terrain::DeathMessage() const { return "strangely enough, you die"; }
cchar* terrain::MonsterDeathVerb() const { return "dies"; }
cchar* terrain::ScoreEntry() const { return "died on unfriendly terrain"; }
