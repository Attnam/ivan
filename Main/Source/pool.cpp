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

/* Compiled through coreset.cpp */

entity* pool::FirstEntity = 0;
entity* pool::LastEntity = 0;
entity* pool::FirstDoomed = 0;
entity* pool::LastDoomed = 0;
entity* pool::CurrentEntity = 0;

/* Calls the Be() function of each self-changeable entity during each tick,
 * thus allowing acting characters, spoiling food etc.
 * Also handles removal of entities marked as dead by calling SendToHell(). */

void pool::Be()
{
  CurrentEntity = FirstEntity;

  while(CurrentEntity)
  {
    CurrentEntity->Be();

    if(CurrentEntity)
      CurrentEntity = CurrentEntity->Next;
  }
}

void pool::BurnHell()
{
  entity* Entity = FirstDoomed;

  while(Entity)
  {
    entity* Next = Entity->Next;
    delete Entity;
    Entity = Next;
  }

  FirstDoomed = 0;
}

void pool::Add(entity* Entity)
{
  Entity->Last = LastEntity;
  Entity->Next = 0;
  LastEntity = (FirstEntity ? LastEntity->Next : FirstEntity) = Entity;
}

void pool::Remove(entity* Entity)
{
  if(CurrentEntity == Entity)
    CurrentEntity = Entity->Next;

  (Entity->Last ? Entity->Last->Next : FirstEntity) = Entity->Next;
  (Entity->Next ? Entity->Next->Last : LastEntity) = Entity->Last;
}

void pool::AddToHell(entity* Entity)
{
  Entity->Next = 0;
  LastDoomed = (FirstDoomed ? LastDoomed->Next : FirstDoomed) = Entity;
}
