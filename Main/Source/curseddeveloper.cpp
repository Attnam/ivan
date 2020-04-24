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
bool cursedDeveloper::bCursedDeveloperTeleport = false;

bool cursedDeveloper::LifeSaveJustABit(character* Killer)
{
  if(!bCursedDeveloper)
    return false;
  
  character* P = game::GetPlayer();
  game::DrawEverything();

  int iKillerBuff=0,iKillerDebuff=0;
  bool bRev;
  bool bStay = BuffAndDebuffPlayerKiller(Killer,iKillerBuff,iKillerDebuff,bRev); //to spice it up
  if(!bStay){
    festring fsAN = Killer->GetAssignedName();
    festring fsToken=" <[B";
    ulong pos = fsAN.Find(fsToken);
    if(pos!=festring::NPos)
      fsAN.Erase(pos,fsAN.GetSize()-pos);
    fsAN<<fsToken<<iKillerBuff<<"D"<<iKillerDebuff<<(bRev?"R":"")<<"]>";
    Killer->SetAssignedName(fsAN);
  }

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
  
  // at death spot
  if(!game::IsInWilderness())
    P->GetLSquareUnder()->SpillFluid(P, liquid::Spawn(BLOOD, 30 * P->GetAttribute(ENDURANCE)));
  
  if(!bStay && Killer && !game::IsInWilderness() && iKillerDebuff==0){
    if(!P->GetLSquareUnder()->GetEngraved())
      game::SetMapNote(P->GetLSquareUnder(),"Your cursed life was saved here@");
    
    //teleport is required to prevent death loop: killer keeps killing the player forever on every turn
    if(Killer->GetSquaresUnder()>1){
      bCursedDeveloperTeleport=true;
      P->TeleportRandomly(true);
      ADD_MESSAGE("You see a flash of dark light and teleport away from the killing blow!");
      bCursedDeveloperTeleport=false;
    }else{
      bool bRestoreTL=false;
      if(Killer->HasStateFlag(TELEPORT_LOCK)){
        Killer->DeActivateTemporaryState(TELEPORT_LOCK);
        bRestoreTL=true;
      }
      Killer->TeleportRandomly(true);
      if(bRestoreTL)
        Killer->GainIntrinsic(TELEPORT_LOCK);
    }
  }
  
  // at resurrect spot
  if(iKillerDebuff>0) // if enemy got too powerful, buff the player randomly
    if(!game::IsInWilderness())
      P->GetLSquareUnder()->SpillFluid(P, liquid::Spawn(MAGIC_LIQUID, 30 * P->GetAttribute(WISDOM)));
  
  ADD_MESSAGE("Your curse forbids you to rest and be remembered...");
  
  game::DrawEverything();
  
  return true;
}

bool AddState(character* Killer,long Flag,cchar* FlagName,long FlagD,cchar* FlagNameD,int& iB)
{
  if(FlagD && Killer->HasStateFlag(FlagD)){
    DBG5("DEACTIVATING",Killer->GetName(DEFINITE).CStr(),FlagD,FlagNameD,iB);
    Killer->DeActivateTemporaryState(FlagD);
  }
  
  DBG5("TRYADD",Killer->GetName(DEFINITE).CStr(),Flag,FlagName,iB);
  if(!Killer->HasStateFlag(Flag)){
    Killer->GainIntrinsic(Flag);
    if(Killer->HasStateFlag(Flag)){
      iB++;
      DBG2("SUCCEED TO ADD!!!",iB);
      return true;
    }else{
      DBG1("FAILED TO ADD");
    }
  }else{
    iB++;
    DBG1("HAS ALREADY");
  }
  
  return false;
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
  
  riDebuff=0;
  rbRev=false;

  // BUFFs, every death makes it harder to player:
  riBuff=1;
#define ASRET(e,b)    if(AddState(Killer,e,#e,0,NULL,b))return false;
#define ASRETD(e,d,b) if(AddState(Killer,e,#e,d,#d,b))return false;
  ASRET(ESP,riBuff);
  ASRET(INFRA_VISION,riBuff);
//  ASRETD(HASTE,SLOW,riBuff);
  ASRET(HASTE,riBuff);
  ASRET(SWIMMING,riBuff);
  ASRET(ETHEREAL_MOVING,riBuff);
  ASRET(REGENERATION,riBuff);
  ASRET(LEVITATION,riBuff);
  ASRET(GAS_IMMUNITY,riBuff);
  ASRET(TELEPORT_LOCK,riBuff);
  ASRET(POLYMORPH_LOCK,riBuff);
  
  /*************************
   *  DEBUFFs, after player has taken too much it is time to make it stop, but slowly:
   */
  riDebuff=1;
  ASRET(HICCUPS,riDebuff);
//  ASRETD(SLOW,HASTE,riDebuff);
  ASRET(SLOW,riDebuff);
//  ASRET(PARASITE_TAPE_WORM,riDebuff);
  ASRET(CONFUSED,riDebuff);
//  ASRET(LEPROSY,riDebuff);
  ASRET(POISONED,riDebuff);
  
  // Revenge, grant it will stop:
  if(!game::IsInWilderness() && game::GetCurrentLevel())
    game::GetCurrentLevel()->Explosion(
      game::GetPlayer(), CONST_S("Killed by cursed fire!"), Killer->GetPos(), 9/*1 square size*/, false, true);
  
  ADD_MESSAGE("Cursed acid hits %s!", Killer->GetName(DEFINITE).CStr());
  if(!game::IsInWilderness()){
    Killer->GetLSquareUnder()->SpillFluid(PLAYER, liquid::Spawn(SULPHURIC_ACID, 30 * PLAYER->GetAttribute(WISDOM)));
    Killer->GetLSquareUnder()->AddSmoke(gas::Spawn(EVIL_WONDER_STAFF_VAPOUR, 100));
  }
    
  rbRev=true;
  
  return true;
}

#endif //CURSEDDEVELOPER
