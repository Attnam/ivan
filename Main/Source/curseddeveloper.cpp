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

#include "dbgmsgproj.h"

#include <bitset>
#include <cmath>

/**
 * This is a developer environment variable to test the game without wizard mode.
 */
#ifdef CURSEDDEVELOPER
bool cursedDeveloper::bCursedDeveloper = [](){char* pc=getenv("IVAN_CURSEDDEVELOPER");return strcmp(pc?pc:"","true")==0;}();
#else
bool cursedDeveloper::bCursedDeveloper = false;
#endif

#ifndef CURSEDDEVELOPER
bool cursedDeveloper::LifeSaveJustABit(character* Killer){return false;}
#else
bool cursedDeveloper::LifeSaveJustABit(character* Killer)
{
  if(!bCursedDeveloper)
    return false;
  
  character* P = game::GetPlayer();
  game::DrawEverything();

  int iBuff=0,iDebuff=0;
  bool bRev;
  bool bStay = BuffAndDebuffPlayerKiller(Killer,iBuff,iDebuff,bRev); //to spice it up
  if(!bStay)
    Killer->SetAssignedName(festring()+"[B"+iBuff+"D"+iDebuff+(bRev?"R":"")+"]"); //player killed count

  // save life but just a little bit
  for(int c = 0; c < P->BodyParts; ++c){ //only enough to continue testing normal gameplay
    bodypart* bp = P->GetBodyPart(c);
    if(!bp && P->CanCreateBodyPart(c))
      bp=P->CreateBodyPart(c);
    if(!bp)continue;
    
    if(bp->GetHP() > bp->GetMaxHP()){ //TODO how does this happens???
      DBG4(c,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
      bp->SetHP(-1);
    }
    
    /**
     * How to prevent endless die loop?
     * Clear the bad effects? better not, let them continue working.
     * A bit more of HP to the core body parts may suffice (funny head is not one lol).
     */
    static int iTorsoHpMinOk=10; //this is to fight mustard gas
    if(P->GetBodyPart(TORSO_INDEX)==bp && bp->GetHP() < iTorsoHpMinOk){
      bp->SetHP(P->GetMaxHP()>iTorsoHpMinOk ? iTorsoHpMinOk : P->GetMaxHP());
      DBG4(c,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
      continue;
    }    
    
    int iHpMinUsable = bp->GetMaxHP()/3 + (bp->GetMaxHP()%3>0 ? 1 : 0); //ceil
    if(bp->GetHP() < iHpMinUsable){
      DBG4(c,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
      bp->SetHP(iHpMinUsable);
      bp->SignalPossibleUsabilityChange();
    }
    
    DBG5(c,iHpMinUsable,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
  }
  P->CalculateBodyPartMaxHPs(0); //this also calculates the overall current HP
  DBG2(P->HP,P->MaxHP);
  if(P->HP>P->MaxHP) // it MUST be ok here!!!
    ABORT("HP>MaxHP %d>%d",P->HP,P->MaxHP);

  if(P->GetNP() < HUNGER_LEVEL)
    P->SetNP(HUNGER_LEVEL); //to avoid endless sleeping

  if(P->HasStateFlag(PANIC))
    P->DeActivateTemporaryState(PANIC); //to be able to do something

  if(P->GetAction())
    P->GetAction()->Terminate(false); //just to avoid messing any action

  if(!bStay && Killer && !game::IsInWilderness()){
    game::SetMapNote(P->GetLSquareUnder(),"Your cursed life was saved here.");
    P->Move(P->GetLevel()->GetRandomSquare(P), true); //teleport is required to prevent death loop: killer keeps killing the player forever on every turn
  }

  ADD_MESSAGE("But wait... you are cursed, therefore forbidden to R.I.P... and your doings will be forever forgotten...");
  return true;
}

/**
 * this will make the NPC that kills the player more challenging for every kill
 * TODO could these NPC permanent upgrades be part of the normal gameplay in some way? May be, the life saving ammulet could let these buffs also be applied?
 * @return if player should stay (true) or teleport (false)
 */
bool cursedDeveloper::BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev)
{
  if(!bCursedDeveloper)return true;
  if(!Killer)return true;
  
  riBuff=1;
  riDebuff=0;
  rbRev=false;
  
  // BUFFs, every death makes it harder to player:
  if(!Killer->HasStateFlag(ESP)){Killer->GainIntrinsic(ESP);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(INFRA_VISION)){Killer->GainIntrinsic(INFRA_VISION);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(VAMPIRISM)){Killer->GainIntrinsic(VAMPIRISM);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(PANIC))
    if(!Killer->HasStateFlag(FEARLESS)){Killer->GainIntrinsic(FEARLESS);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(SLOW))
    if(!Killer->HasStateFlag(HASTE)){Killer->GainIntrinsic(HASTE);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(HICCUPS))
    if(!Killer->HasStateFlag(INVISIBLE)){Killer->GainIntrinsic(INVISIBLE);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(SWIMMING)){Killer->GainIntrinsic(SWIMMING);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(ETHEREAL_MOVING)){Killer->GainIntrinsic(ETHEREAL_MOVING);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(REGENERATION)){Killer->GainIntrinsic(REGENERATION);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(LEVITATION)){Killer->GainIntrinsic(LEVITATION);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(GAS_IMMUNITY)){Killer->GainIntrinsic(GAS_IMMUNITY);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(TELEPORT_LOCK)){Killer->GainIntrinsic(TELEPORT_LOCK);return false;}
  riBuff++;
  if(!Killer->HasStateFlag(POLYMORPH_LOCK)){Killer->GainIntrinsic(POLYMORPH_LOCK);return false;}
  
  // DEBUFFs, after player has taken too much it is time to make it stop, but slowly:
  riDebuff=1;
  if(!Killer->HasStateFlag(HICCUPS)){
    Killer->DeActivateTemporaryState(INVISIBLE);
    Killer->GainIntrinsic(HICCUPS);
    return false;
  }
  riDebuff++;
  
  if(!Killer->HasStateFlag(SLOW)){
    Killer->DeActivateTemporaryState(HASTE);
    Killer->GainIntrinsic(SLOW);
    return false;
  }
  riDebuff++;
  
  if(!Killer->HasStateFlag(PARASITE_TAPE_WORM)){Killer->GainIntrinsic(PARASITE_TAPE_WORM);return false;}
  riDebuff++;
  if(!Killer->HasStateFlag(CONFUSED)){Killer->GainIntrinsic(CONFUSED);return false;}
  riDebuff++;
  if(!Killer->HasStateFlag(LEPROSY)){Killer->GainIntrinsic(LEPROSY);return false;}
  riDebuff++;
// this is too much as adds worm mobs on the dungeon...
//  if(!Killer->HasStateFlag(PARASITE_MIND_WORM)){Killer->GainIntrinsic(PARASITE_MIND_WORM);return false;}
//  riDebuff++;
  if(!Killer->HasStateFlag(POISONED)){Killer->GainIntrinsic(POISONED);return false;}
  riDebuff++;
  if(!Killer->HasStateFlag(PANIC)){
    Killer->DeActivateTemporaryState(FEARLESS);
    Killer->GainIntrinsic(PANIC);
    return true;
  }
  
  // Revenge, grant it will stop:
  game::GetCurrentLevel()->Explosion(
    game::GetPlayer(), CONST_S("Killed by cursed fire!"), Killer->GetPos(), 9/*1 square size*/, false, true);
  
  ADD_MESSAGE("Cursed acid hits %s!", Killer->GetName(DEFINITE).CStr());
  Killer->GetLSquareUnder()->SpillFluid(PLAYER, liquid::Spawn(SULPHURIC_ACID, 30 * PLAYER->GetAttribute(WISDOM)));
  
  rbRev=true;
  
  return true;
}
#endif //CURSEDDEVELOPER
