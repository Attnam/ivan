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

/* Compiled through charset.cpp */

team::team() : Leader(0) { }
team::team(ulong ID) : Leader(0), ID(ID), KillEvilness(0) { }
std::list<character*>::iterator team::Add(character* Char) { return Member.insert(Member.end(), Char); }

void team::SetRelation(team* AnotherTeam, int Relation)
{
  this->Relation[AnotherTeam->ID] = AnotherTeam->Relation[ID] = Relation;
}

int team::GetRelation(const team* AnotherTeam) const
{
  if(AnotherTeam != this)
  {
    std::map<ulong, int>::const_iterator Iterator = Relation.find(AnotherTeam->ID);

    if(Iterator != Relation.end())
      return Iterator->second;
    else
      ABORT("Team %lu dismissed!", AnotherTeam->ID);
  }

  return FRIEND;
}

void team::Hostility(team* Enemy)
{
  /* We're testing if the game works better this way... */

  if(ID != PLAYER_TEAM)
    return;

  if(this != Enemy && GetRelation(Enemy) != HOSTILE)
  {
    if(ID == PLAYER_TEAM && game::IsSumoWrestling())
      game::EndSumoWrestling(DISQUALIFIED);

    /* This is a gum solution. The behaviour should come from the script. */

    /*if(ID == COLONIST_TEAM && Enemy->ID == NEW_ATTNAM_TEAM)
      return;*/

    game::Hostility(this, Enemy);

    if(ID == PLAYER_TEAM)
    {
      if(Enemy->ID == ATTNAM_TEAM)
      {
	/* This is a gum solution. The message should come from the script. */
	if(PLAYER->CanHear())
	  ADD_MESSAGE("You hear an alarm ringing.");

	if(game::GetStoryState() != 2)
	{
	  v2 AngelPos = game::GetPetrus() ? game::GetPetrus()->GetPos() : v2(28, 20);
	  int Seen = 0;
	  angel* Angel;

	  for(int c = 0; c < 3; ++c)
	  {
	    if(!c)
	      Angel = archangel::Spawn(VALPURUS);
	    else
	      Angel = angel::Spawn(VALPURUS);

	    v2 Where = game::GetCurrentLevel()->GetNearestFreeSquare(Angel, AngelPos);

	    if(Where == ERROR_V2)
	      Where = game::GetCurrentLevel()->GetRandomSquare(Angel);

	    Angel->SetTeam(Enemy);
	    Angel->PutTo(Where);

	    if(Angel->CanBeSeenByPlayer())
	      ++Seen;
	  }

	  if(Seen == 1)
	    ADD_MESSAGE("%s materializes.", Angel->CHAR_NAME(INDEFINITE));
	  else if(Seen == 2)
	    ADD_MESSAGE("Two %s materialize.", Angel->CHAR_NAME(PLURAL));
	  else if(Seen == 3)
	    ADD_MESSAGE("Three %s materialize.", Angel->CHAR_NAME(PLURAL));

	  ADD_MESSAGE("\"We will defend the Holy Order!\"");
	}
      }

      ADD_MESSAGE("You have a feeling this wasn't a good idea...");
    }

    SetRelation(Enemy, HOSTILE);
  }
}

void team::Save(outputfile& SaveFile) const
{
  SaveFile << ID << Relation << KillEvilness;
}

void team::Load(inputfile& SaveFile)
{
  SaveFile >> ID >> Relation >> KillEvilness;
}

truth team::HasEnemy() const
{
  for(int c = 0; c < game::GetTeams(); ++c)
    if(!game::GetTeam(c)->GetMember().empty() && GetRelation(game::GetTeam(c)) == HOSTILE)
      return true;

  return false;
}

int team::GetEnabledMembers() const
{
  int Amount = 0;

  for(std::list<character*>::const_iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled())
      ++Amount;

  return Amount;
}

outputfile& operator<<(outputfile& SaveFile, const team* Team)
{
  Team->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, team*& Team)
{
  Team = new team;
  Team->Load(SaveFile);
  return SaveFile;
}

void team::MoveMembersTo(charactervector& CVector)
{
  for(std::list<character*>::iterator i = Member.begin(); i != Member.end(); ++i)
    if((*i)->IsEnabled())
    {
      if((*i)->GetAction() && (*i)->GetAction()->IsVoluntary())
	(*i)->GetAction()->Terminate(false);

      if(!(*i)->GetAction())
      {
	CVector.push_back(*i);
	(*i)->Remove();
      }
    }
}

void team::Remove(std::list<character*>::iterator Iterator)
{
  if(*Iterator == Leader)
    Leader = 0;

  Member.erase(Iterator);
}
