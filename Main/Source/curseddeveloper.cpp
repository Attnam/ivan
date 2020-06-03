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
#include "devcons.h"

#include <bitset>
#include <cmath>

/**
 * This is a developer environment variable to test the game without wizard mode.
 */

festring fsKCPrefix="CursedDeveloperKC=";
character* GetPlayerCharWithTorsoForKC()
{
  character* P=game::GetPlayer();
  
  while(P->GetTorso()->GetLabel().Find(fsKCPrefix)==festring::NPos){
    if(!P->GetPolymorphBackup())
      break;
    DBG2(P->GetID(),P->GetNameSingular().CStr());
    P=P->GetPolymorphBackup();
    DBGEXEC(if(P){DBG2(P->GetID(),P->GetNameSingular().CStr());});
  }
  
  return P;
}

bool curseddeveloper::IsCursedDeveloper()
{
#ifdef CURSEDDEVELOPER
  if(bCursedDeveloper)return true; //this is for the real compiled mode
#endif
  
  static character* Pprevious=NULL;
  static int iCursedDeveloperDoubleCheck=0;
  
  if(game::GetPlayer()!=Pprevious){
    iCursedDeveloperDoubleCheck=0; // to let it be checked again, mainly when coming back from main menu
    Pprevious=game::GetPlayer();
  }
  
  if(iCursedDeveloperDoubleCheck!=0)
    return iCursedDeveloperDoubleCheck==1;
    
  /**
   * deep check/validation against a savegame that could have been used initially
   * on the cursed developer mode and after on a normal gameplay.
   */
  character* P = GetPlayerCharWithTorsoForKC();
  if(P->GetTorso()->GetLabel().Find(fsKCPrefix)!=festring::NPos)
    iCursedDeveloperDoubleCheck = 1; //cursed
  else
    iCursedDeveloperDoubleCheck = 2; //normal
  
  return iCursedDeveloperDoubleCheck==1;
}

#ifdef CURSEDDEVELOPER

bool curseddeveloper::bCursedDeveloper = [](){char* pc=getenv("IVAN_CURSEDDEVELOPER");return strcmp(pc?pc:"","true")==0;}();
bool curseddeveloper::bCursedDeveloperTeleport = false;
long curseddeveloper::lKillCredit=0;
bool curseddeveloper::bNightmareWakeUp=false;
bool curseddeveloper::bResurrect=false;
character* curseddeveloper::Killer=NULL;
bool bAlwaysTryToWakeup=false;

#define HEAL_1     1
#define HEAL_MINOK 2
#define HEAL_MAX   3

/**
 *  is some special/named/important character
 */
bool IsSpecialCharacter(character* C){return C && !C->CanBeCloned();} 

void curseddeveloper::NightmareWakeUp(character* P)
{
  bNightmareWakeUp=false;
  
  ADD_MESSAGE("You had a nightmare! And... for some reason, you feel stronger...");
  P->GetLSquareUnder()->SpillFluid(P, liquid::Spawn(SWEAT, 5 * P->GetAttribute(ENDURANCE)));

  if(RAND()%3){
    P->GetLSquareUnder()->SpillFluid(P, liquid::Spawn(OMMEL_URINE, 5 * P->GetAttribute(ENDURANCE))); //ugh.. not ommel's tho.. TODO will this buff the player?
    ADD_MESSAGE("You need a bath now...");
  }

  if(lKillCredit!=0)
    ResetKillCredit();
}

void curseddeveloper::ResetKillCredit(festring fsCmdParams)
{
  ModKillCredit(lKillCredit * -1);
}

long curseddeveloper::UpdateKillCredit(character* Victim,int iMod)
{
  character* P=game::GetPlayer();
  if(!P)return lKillCredit;
  
  P=GetPlayerCharWithTorsoForKC();
  festring fsKillCredit = P->GetTorso()->GetLabel();
  fsKillCredit.Erase(0,fsKCPrefix.GetSize());
  DBG1(fsKillCredit.CStr());
  long lKCStored=0;
  if(!fsKillCredit.IsEmpty())
    lKCStored = atol(fsKillCredit.CStr());
  DBG1(lKCStored);
  
  if(Victim){
    int i = Victim->GetRelativeDanger(P)*10;
    DBG1(i);
    if(i<1)i=1;
    lKCStored+=i;
  }
  
  lKCStored+=iMod;
  
  if(bNightmareWakeUp)
    NightmareWakeUp(P);
  
  DBG1(lKCStored);
  P->GetTorso()->SetLabel(festring()<<fsKCPrefix<<lKCStored); // using label as custom data storage
  lKillCredit = lKCStored;
  
  return lKillCredit;
}

void curseddeveloper::RestoreLimbs(festring fsCmdParams)
{
  int iMode = fsCmdParams.IsEmpty() ? 0 : atoi(fsCmdParams.CStr());
  
  character* P = game::GetPlayer();
  
  // save life but just a little bit
  for(int c = 0; c < P->BodyParts; ++c){ //only enough to continue testing normal gameplay
    if(!CreateBP(c))continue;
    
    HealBP(c,iMode);
  }
  P->CalculateBodyPartMaxHPs(0); //this also calculates the overall current HP
  DBG2(P->HP,P->MaxHP);
  if(P->HP > P->MaxHP) // it MUST be ok here!!!
    ABORT("HP>MaxHP %d>%d",P->HP,P->MaxHP);
}

bool curseddeveloper::CreateBP(int iIndex)
{
  character* P = game::GetPlayer();
  
  if(dynamic_cast<humanoid*>(P))
  {
    //ok
  }
  else if(iIndex!=TORSO_INDEX) //can be polymorphed into non humanoid
    return false;
  
  bodypart* bp = P->GetBodyPart(iIndex);
  
  if(!bp){
    int iMod=1;
    for(ulong iOBpID : P->GetOriginalBodyPartID(iIndex)){
      bp = dynamic_cast<bodypart*>(game::SearchItem(iOBpID));
      if(bp){
        if(iIndex == HEAD_INDEX){
          /**
           * when a kamikaze dwarf explodes and player's head flies away,
           * bringing it back apparently causes SEGFAULT at: 
           * game::run() > pool::be() > character::be() at `BodyPart->Be();`
           * there is no null pointer, the `BodyPart` is valid and can be debugged,
           * but when BodyPart->Be() is called it SEGFAULTs...
           */
          bp->RemoveFromSlot();
          bp->SendToHell(); //so lets just destroy it to let a new one be created
          bp=NULL;
        }
        break;
      }
    }

    if(bp){
      bp->RemoveFromSlot();
      P->AttachBodyPart(bp);
      ADD_MESSAGE("Your creepy %s comes back to you.",bp->GetName(UNARTICLED).CStr());
      ModKillCredit(-1);
      iMod=5;
    }else{
      if(P->CanCreateBodyPart(iIndex)){
        bp=P->CreateBodyPart(iIndex);
        if(bp){
          ADD_MESSAGE("A new cursed %s vibrates and grows on you.",bp->GetName(UNARTICLED).CStr());
          ModKillCredit(-2);
          iMod=10;
        }
      }
    }
    
    if(bp){
      bp->SpillFluid(P,liquid::Spawn(LIQUID_DARKNESS,iMod*2));
      if(iIndex == HEAD_INDEX){
        int iTm=iMod*10;
        P->BeginTemporaryState(CONFUSED, iTm + RAND()%iTm);
      }
    }
  }
  
  if(bp && bp->GetHP() > bp->GetMaxHP()){ //TODO how does this happens???
    DBG4(iIndex,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
    bp->SetHP(-1); //to allow properly fixing
  }
  
  return bp!=NULL;
}

bool curseddeveloper::HealBP(int iIndex,int iMode,int iResHPoverride)
{
  if(!CreateBP(iIndex))return false;
  
  character* P = game::GetPlayer();
  
  /**
   * How to prevent endless die loop?
   * Clear the bad effects? better not, let them continue working.
   * A bit more of HP to the core body parts may suffice (funny head is not one lol).
   */
  bodypart* bp = P->GetBodyPart(iIndex);
  if(bp && bp->GetHP() < bp->GetMaxHP()){
    int iDiv=3;
    int iHpMinUsable = bp->GetMaxHP()/iDiv + (bp->GetMaxHP()%iDiv>0 ? 1 : 0); //ceil
    
    int iHpRestore = 0;
    switch(iMode){
      case HEAL_1: 
        iHpRestore = 1; 
        break;
      case HEAL_MINOK: 
        iHpRestore = iHpMinUsable;
        break;
      case HEAL_MAX: 
        iHpRestore = bp->GetMaxHP(); 
        break;
    }
    
    if(iResHPoverride>0)iHpRestore=iResHPoverride;
    
    if(bp->GetHP() < iHpRestore){
      DBG4(iIndex,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
      bp->SetHP(iHpRestore);
      bp->SignalPossibleUsabilityChange();
      switch(iMode){
        case HEAL_1: 
          break;
        case HEAL_MINOK: 
          ModKillCredit(-1); 
          break;
        case HEAL_MAX: 
          ModKillCredit(-iDiv); 
          break;
      }
    }
    
    DBG5(iIndex,iHpMinUsable,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
    
//    bp->SetHP(P->GetMaxHP()>iMinHpOK ? iMinHpOK : P->GetMaxHP());
//    DBG4(c,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
    return true;
  }    
  return false;
}
//bool cursedDeveloper::HealTorso(bodypart* bp)
//{
//  character* P = game::GetPlayer();
//  
//  /**
//   * How to prevent endless die loop?
//   * Clear the bad effects? better not, let them continue working.
//   * A bit more of HP to the core body parts may suffice (funny head is not one lol).
//   */
//  static int iTorsoHpMinOk=10; //this is to fight mustard gas
//  if(P->GetBodyPart(TORSO_INDEX)==bp && bp->GetHP() < iTorsoHpMinOk){
//    bp->SetHP(P->GetMaxHP()>iTorsoHpMinOk ? iTorsoHpMinOk : P->GetMaxHP());
//    DBG4(c,bp->GetHP(),bp->GetMaxHP(),bp->GetBodyPartName().CStr());
//    return true;
//  }    
//  return false;
//}

//curseddeveloper::curseddeveloper()
//{
//  devcons::AddDevCmd("RestoreLimbs",curseddeveloper::RestoreLimbs,
//    "[1|2|3] 1=1HP 2=minUsableHP 3=maxHP. Restore missing limbs to the cursed developer, better use only if the game becomes unplayable.");
//}
bool bAllowWakeUp=true;
void SetAllowWakeUp(festring fs)
{
  bAllowWakeUp=true;
  if(fs=="no")
    bAllowWakeUp=false;
  
  if(bAllowWakeUp){
    ADD_MESSAGE("You may wakeup...");
  }else{
    ADD_MESSAGE("You won't wakeup!");
  }
}
void SetAlwaysTryToWakeup(festring fs)
{
  bAlwaysTryToWakeup=false;
  if(fs=="yes")
    bAlwaysTryToWakeup=true;
  
  if(bAlwaysTryToWakeup){
    ADD_MESSAGE("You will wakeup...");
  }else{
    ADD_MESSAGE("You won't wakeup!");
  }
}
void curseddeveloper::Init(){
  devcons::AddDevCmd("RestoreLimbs",curseddeveloper::RestoreLimbs,
    "[1|2|3] 1=1HP 2=minUsableHP 3=maxHP. Restore missing limbs, better use only if the game becomes unplayable (cursed immortal).");
  devcons::AddDevCmd("ResetKC",curseddeveloper::ResetKillCredit,
    "to help make tests related to KillCredit's negative value (cursed immortal).");
  devcons::AddDevCmd("AllowWakeup",SetAllowWakeUp,
    "[no] to help on making tests ignoring KC negative value (cursed immortal).");
  devcons::AddDevCmd("AlwaysTryToWakeup",SetAlwaysTryToWakeup,
    "[yes] to help on making tests (cursed immortal).");
}

bool curseddeveloper::LifeSaveJustABitIfRequested()
{
  if(bResurrect){
    bool b=LifeSaveJustABit(Killer);
    Killer=NULL;
    bResurrect=false;
    return b;
  }
  return false;
}

bool curseddeveloper::LifeSaveJustABit(character* Killer)
{
  if(!bCursedDeveloper)
    return false;
  
  if(Killer && Killer->IsPlayer())
    ABORT("What? player killer is player? name='%s' id='%d'",Killer->GetNameSingular().CStr(),Killer->GetID());
  
  character* P = game::GetPlayer();
  game::DrawEverything();

  int iKillerBuff=0,iKillerDebuff=0;
  bool bRev;
  bool bStay = BuffAndDebuffPlayerKiller(Killer,iKillerBuff,iKillerDebuff,bRev); //to spice it up
  if(Killer){
    festring fsAN = Killer->GetAssignedName();
    festring fsToken=" <[B";
    ulong pos = fsAN.Find(fsToken);
    if(pos!=festring::NPos)
      fsAN.Erase(pos,fsAN.GetSize()-pos);
    fsAN<<fsToken<<iKillerBuff<<"D"<<iKillerDebuff<<(bRev?"{R}":"")<<"]>";
    Killer->SetAssignedName(fsAN);
  }

  // automatic minimal to save life
//  HealTorso(P->GetTorso());
  HealBP(HEAD_INDEX,HEAL_1);
  HealBP(TORSO_INDEX,0,10);//10hp is min to fight mustard gas
  HealBP(GROIN_INDEX,HEAL_1);
  
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
    //teleport is required to prevent death loop: killer keeps killing the player forever on every turn
    if(Killer->GetSquaresUnder()>1){ //huge foes
      bCursedDeveloperTeleport=true;
      if(!P->GetLSquareUnder()->GetEngraved())
        game::SetMapNote(P->GetLSquareUnder(),festring("@Your cursed life was saved here at ")<<game::GetTurn()); //this helps to find lost items
      P->TeleportRandomly(true);
      ADD_MESSAGE("You see a flash of dark light and teleport away from the killing blow!");
      bCursedDeveloperTeleport=false;
    }else{
      if(IsSpecialCharacter(Killer)){
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
  }
  
  // at resurrect spot
  if(iKillerDebuff>0) // if enemy got too powerful, buff the player randomly
    if(!game::IsInWilderness())
      P->GetLSquareUnder()->SpillFluid(NULL, liquid::Spawn(MAGIC_LIQUID, 30 * P->GetAttribute(WISDOM)));
  
//  if(bRev || IsSpecialCharacter(Killer)){
  character* M = P->DuplicateToNearestSquare(P, MIRROR_IMAGE|IGNORE_PROHIBITIONS|CHANGE_TEAM);
  if(M){
    int x=1;
    if(GetKillCredit()>x && P->GetBodyPart(RIGHT_ARM_INDEX))x++;
    if(GetKillCredit()>x && P->GetBodyPart(LEFT_ARM_INDEX ))x++;
    if(GetKillCredit()>x && P->GetBodyPart(RIGHT_LEG_INDEX))x++;
    if(GetKillCredit()>x && P->GetBodyPart(LEFT_LEG_INDEX ))x++;

    static int i1Min=33; //33 is 1 min or 1 turn right? see: game::GetTime() (any relation with 30 frames per second? 30 ticks?)
    int iXtra = GetKillCredit()<0 ? 10 : 1;
    int iLE = i1Min*5*x*iXtra;
    M->SetLifeExpectancy(iLE, iLE*2);
    ModKillCredit(-1*x);
  }
//  }
  
  ADD_MESSAGE("Your curse forbids you to rest and be remembered...");
  
  UpdateKillCredit();
        
  game::DrawEverything();
  
  int iDung=3,iLvl=0; //tweiraith island
  bool bIsAtHomeIsland = P->GetDungeon()->GetIndex()==iDung && P->GetLevel()->GetIndex()==iLvl;
  DBG6(P->GetDungeon()->GetIndex(),iDung,P->GetLevel()->GetIndex(),iLvl,lKillCredit,bIsAtHomeIsland);
  if(lKillCredit<0 && bIsAtHomeIsland){
    ResetKillCredit(); //to prevent pointless too negative value at home town
  }else
  if(bAllowWakeUp && lKillCredit<0 && !game::IsInWilderness() && !bIsAtHomeIsland && Killer){
    if(RAND()%10==0 || bAlwaysTryToWakeup){
      for(int i=0;i<10;i++){
        ADD_MESSAGE("You try to wakeup...");
        if(game::TryTravel(iDung, iLvl, DOUBLE_BED, false, true)){ //TODO should be the small bed at the small house
          bNightmareWakeUp=true;
          ADD_MESSAGE("You finally wakeup.");
          UpdateKillCredit(); //to call nightmare wakeup
          return true; // after TryTravel() avoid most code...
        }
        P->TeleportRandomly(true); //try to move away from foes to be able to travel
        ADD_MESSAGE("You feel haunted!");
      }
    }else{
      ADD_MESSAGE("You feel unconfortable...");
    }
  }
  
  return true;
}

bool AddState(character* Killer,long Flag,cchar* FlagName,long FlagD,cchar* FlagNameD,int& iB)
{
  if(FlagD && Killer->HasStateFlag(FlagD)){
    DBG5("DEACTIVATING",Killer->GetName(DEFINITE).CStr(),FlagD,FlagNameD,iB);
    Killer->DeActivateTemporaryState(FlagD);
  }
  
  if(Flag){
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
  }
  
  return false;
}

/**
 * This will make the Special NPC that kills the player more challenging for every kill.
 * Non special NPCs will fall faster.
 * TODO could these NPC permanent upgrades be part of the normal gameplay in some way? May be, the life saving ammulet could let these buffs also be applied?
 * @return if player should stay (true) or teleport (false)
 */
bool curseddeveloper::BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev)
{
  if(!bCursedDeveloper)return true;
  if(!Killer)return true;
  if(game::IsInWilderness())return true;
  
  riDebuff=0;
  rbRev=false;

  // BUFFs, every death makes it harder to player:
  riBuff=1;
#define ASRET(e,b) if(AddState(Killer,e,#e,0,NULL,b) && IsSpecialCharacter(Killer))return false;
//#define RMS(d,b)   if(AddState(Killer,0,NULL,d,#d,b) && IsSpecialCharacter(Killer))return false;
#define RMS(d,b)   AddState(Killer,0,NULL,d,#d,b);
  bool bAlreadyRev = Killer->GetAssignedName().Find("{R}")!=festring::NPos;
  if(IsSpecialCharacter(Killer)){
    if(!bAlreadyRev){
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
    }else{
      RMS(ESP,riBuff);
      RMS(INFRA_VISION,riBuff);
      RMS(HASTE,riBuff);
      RMS(SWIMMING,riBuff);
      RMS(ETHEREAL_MOVING,riBuff);
      RMS(REGENERATION,riBuff);
      RMS(LEVITATION,riBuff);
      RMS(GAS_IMMUNITY,riBuff);
      RMS(TELEPORT_LOCK,riBuff);
      RMS(POLYMORPH_LOCK,riBuff);
    }
  }
  
  /*************************
   *  DEBUFFs, after player has taken too much it is time to make it stop, but slowly:
   */
  riDebuff=1;
  ASRET(HICCUPS,riDebuff);
//  ASRETD(SLOW,HASTE,riDebuff);
  ASRET(SLOW,riDebuff);
//no, adds more mobs...  ASRET(PARASITE_TAPE_WORM,riDebuff);
  ASRET(CONFUSED,riDebuff);
  ASRET(POLYMORPH,riDebuff); //this may be the only way to defeat some special mob
//no, may mess the player...  ASRET(LEPROSY,riDebuff);
  if(RAND()%10==0){
    ASRET(POISONED,riDebuff);
    ModKillCredit(-2);
  }
  
  // Revenge, grant it will stop:
  if(bAlreadyRev || RAND()%5==0){
    for(int i=0; i < ( RAND() % (IsSpecialCharacter(Killer)?5:2) + 1 ) ;i++){
      if(Killer->IsDead())break;
      game::GetCurrentLevel()->Explosion(
        NULL, CONST_S("Killed by cursed fire!"), Killer->GetPos(), 9/*1 square size*/, false, true);
      ModKillCredit(-1);
    }
  }

  if(bAlreadyRev || RAND()%10==0){
    bodypart* bpHit=NULL;
    if(dynamic_cast<humanoid*>(Killer)){
      switch(RAND()%(10+5)){
        case 0:
          bpHit = Killer->GetBodyPart(HEAD_INDEX);
          if(bpHit)break;
        case 1:
          bpHit = Killer->GetBodyPart(GROIN_INDEX);
          if(bpHit)break;
        case 2: case 3:
          bpHit = Killer->GetBodyPart(LEFT_ARM_INDEX);
          if(bpHit)break;
        case 4: case 5:
          bpHit = Killer->GetBodyPart(RIGHT_ARM_INDEX);
          if(bpHit)break;
        case 6: case 7:
          bpHit = Killer->GetBodyPart(LEFT_LEG_INDEX);
          if(bpHit)break;
        case 8: case 9:
          bpHit = Killer->GetBodyPart(RIGHT_LEG_INDEX);
          if(bpHit)break;
        default: break;
      }
    }
    if(!bpHit)
      bpHit=Killer->GetTorso();
    bpHit->SpillFluid(NULL, liquid::Spawn(SULPHURIC_ACID, 5 * game::GetPlayer()->GetAttribute(WISDOM)));
    ADD_MESSAGE("Cursed acid hits %s!", Killer->GetName(DEFINITE).CStr());
    ModKillCredit(-3);
  }
    
  rbRev=true;
  
  UpdateKillCredit();
  
  return true;
}

#endif //CURSEDDEVELOPER
