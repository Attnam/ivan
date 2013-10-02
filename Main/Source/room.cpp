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

/* Compiled through roomset.cpp */

roomprototype::roomprototype(roomspawner Spawner, cchar* ClassID) : Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<room>::Add(this); }

void room::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  SaveFile << Pos << Size << Index << DivineMaster << MasterID;
}

void room::Load(inputfile& SaveFile)
{
  SaveFile >> Pos >> Size >> Index >> DivineMaster >> MasterID;
}

room* roomprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  room* Room = Spawner();
  Room->Load(SaveFile);
  return Room;
}

void room::DestroyTerrain(character* Who)
{
  if(Who && MasterIsActive())
    Who->Hostility(GetMaster());

  if(Who && Who->IsPlayer() && DivineMaster)
    game::GetGod(DivineMaster)->AdjustRelation(GetGodRelationAdjustment());
}

/* returns true if player agrees to continue */

truth room::CheckDestroyTerrain(character* Infidel)
{
  if(!MasterIsActive() || Infidel == GetMaster() || GetMaster()->GetRelation(Infidel) == HOSTILE)
    return true;

  ADD_MESSAGE("%s might not like this.", GetMaster()->CHAR_NAME(DEFINITE));

  if(game::TruthQuestion(CONST_S("Are you sure you want to do this? [y/N]")))
  {
    DestroyTerrain(Infidel);
    return true;
  }
  else
    return false;
}

truth room::MasterIsActive() const
{
  character* Master = GetMaster();
  return Master && Master->IsEnabled() && Master->IsConscious();
}

truth room::CheckKickSquare(ccharacter* Kicker, const lsquare* LSquare) const
{
  if(!AllowKick(Kicker, LSquare))
  {
    ADD_MESSAGE("That would be vandalism.");

    if(!game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
      return false;
  }
  return true;
}

character* room::GetMaster() const
{
  ulong Tick = game::GetTick();

  if(LastMasterSearchTick == Tick)
    return Master;
  else
  {
    LastMasterSearchTick = Tick;
    return Master = game::SearchCharacter(MasterID);
  }
}

truth room::IsOKToDestroyWalls(ccharacter* Infidel) const
{
  return !MasterIsActive() || Infidel == GetMaster() || GetMaster()->GetRelation(Infidel) == HOSTILE;
}

void room::FinalProcessForBone()
{
  if(MasterID)
  {
    boneidmap::iterator BI = game::GetBoneCharacterIDMap().find(MasterID);

    if(BI != game::GetBoneCharacterIDMap().end())
      MasterID = BI->second;
    else
      MasterID = 0;
  }
}
