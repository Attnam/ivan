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

/* Compiled through actset.cpp */

actionprototype::actionprototype(actionspawner Spawner, cchar* ClassID)
: Spawner(Spawner), ClassID(ClassID) { Index = protocontainer<action>::Add(this); }

void action::Terminate(truth)
{
  if(GetActor()->IsPlayer() && ivanconfig::IsXBRZScale())
    game::UpdateSRegionsXBRZ(true); // to restore it, see character::Be() just b4 action Handle()

  GetActor()->SetAction(0);
  delete this;
}

void action::Save(outputfile& SaveFile) const
{
  SaveFile << static_cast<ushort>(GetType()) << Flags;
}

void action::Load(inputfile& SaveFile)
{
  SaveFile >> Flags;
}

action* actionprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  action* Action = Spawner(0);
  Action->Load(SaveFile);
  return Action;
}
