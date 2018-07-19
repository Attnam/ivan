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

#include "confdef.h"

#include "dbgmsgproj.h"

cchar* unconsciousness::GetDeathExplanation() const { return " unconscious"; }
cchar* unconsciousness::GetDescription() const { return "unconscious"; }
cchar* consume::GetDescription() const { return Description.CStr(); }
void consume::SetDescription(cfestring& What) { Description = What; }
cchar* rest::GetDescription() const { return "resting"; }
cchar* craft::GetDescription() const { return "crafting"; }
cchar* dig::GetDescription() const { return "digging"; }
cchar* go::GetDescription() const { return "going"; }
cchar* study::GetDescription() const { return "reading"; }

void unconsciousness::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Counter;
}

void unconsciousness::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Counter;
}

void unconsciousness::Handle()
{
  if(!--Counter)
    Terminate(true);
  else
  {
    Actor->EditExperience(ARM_STRENGTH, -50, 1 << 2);
    Actor->EditExperience(LEG_STRENGTH, -50, 1 << 2);
  }
}

void unconsciousness::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(GetActor()->IsPlayer())
    ADD_MESSAGE("You wake up.");
  else if(GetActor()->CanBeSeenByPlayer())
    ADD_MESSAGE("%s wakes up.", GetActor()->CHAR_NAME(DEFINITE));

  action::Terminate(Finished);
}

void consume::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << ConsumingID << Description;
}

void consume::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> ConsumingID >> Description;
}

void consume::Handle()
{
  item* Consuming = game::SearchItem(ConsumingID);

  if(!Consuming || !Consuming->Exists() || !Actor->IsOver(Consuming))
  {
    Terminate(false);
    return;
  }

  character* Actor = GetActor();

  if(!InDNDMode() && Actor->GetHungerState() >= BLOATED)
  {
    if(Actor->IsPlayer())
    {
      ADD_MESSAGE("You have a really hard time getting all this down your throat.");

      if(game::TruthQuestion(CONST_S("Continue ") + GetDescription() + "? [y/N]"))
        ActivateInDNDMode();
      else
      {
        Terminate(false);
        return;
      }
    }
    else
    {
      Terminate(false);
      return;
    }
  }

  if(!Actor->IsPlayer() && !Consuming->CanBeEatenByAI(Actor)) // item may be spoiled after action was started
  {
    Terminate(false);
    return;
  }

  /* Note: if backupped Actor has died of food effect,
     Action is deleted automatically, so we mustn't Terminate it */

  if(Consuming->Consume(Actor, 100) && Actor->GetAction() == this && Actor->IsEnabled())
    Terminate(true);
  else if(Actor->GetHungerState() == OVER_FED)
  {
    Actor->DeActivateVoluntaryAction(CONST_S("You are about to choke on this stuff."));
    Actor->Vomit(Actor->GetPos(), 500 + RAND() % 500);
  }
  else if(Actor->GetAction() == this && IsNibbling())
    Terminate(true);
}

void consume::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;
  item* Consuming = game::SearchItem(ConsumingID);
  character* Actor = GetActor();

  if(Actor->IsPlayer())
    ADD_MESSAGE("You %s %s.", Finished ? "finish" : "stop", Description.CStr());
  else if(Actor->CanBeSeenByPlayer())
    ADD_MESSAGE("%s %s %s.", Actor->CHAR_NAME(DEFINITE), Finished ? "finishes" : "stops", Description.CStr());

  if(Finished)
  {
    if(Consuming->Exists() && !game::IsInWilderness() && (!Actor->IsPlayer() || ivanconfig::GetAutoDropLeftOvers()))
    {
      Consuming->RemoveFromSlot();
      Actor->GetStackUnder()->AddItem(Consuming);
      Actor->DexterityAction(2);
    }
  }
  else if(Consuming && Consuming->Exists())
  {
    material* ConsumeMaterial = Consuming->GetConsumeMaterial(Actor);

    if(ConsumeMaterial)
      ConsumeMaterial->FinishConsuming(Actor);
  }

  action::Terminate(Finished);
}

void rest::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << GoalHP << MinToStop;
}

void rest::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> GoalHP >> MinToStop;
}

void rest::Handle()
{
  if((GoalHP && (GetActor()->GetHP() >= GoalHP
                 || GetActor()->GetHP() == GetActor()->GetMaxHP()
                 || !GetActor()->CanHeal()))
     || (MinToStop && game::GetTotalMinutes() >= MinToStop))
    Terminate(true);
  else
  {
    GetActor()->EditExperience(DEXTERITY, -25, 1 << 1);
    GetActor()->EditExperience(AGILITY, -25, 1 << 1);
  }
}

void rest::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(Finished)
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You finish resting.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s finishes resting.", GetActor()->CHAR_NAME(DEFINITE));
  }
  else
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop resting.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops resting.", GetActor()->CHAR_NAME(DEFINITE));
  }

  action::Terminate(Finished);
}

void craft::Save(outputfile& SaveFile) const
{DBGLN;DBGSTK;
  action::Save(SaveFile);
  rpd.Save(SaveFile);
  SaveFile << MoveCraftTool << RightBackupID << LeftBackupID;
}

void craft::Load(inputfile& SaveFile)
{DBGLN;
  action::Load(SaveFile);
  rpd.Load(SaveFile);
  SaveFile >> MoveCraftTool >> RightBackupID >> LeftBackupID;

  if(rpd.rc.IsCanBeSuspended())
    craftcore::SetSuspended(&rpd);
}

//cfestring craft::info(){
//  return rpd.dbgInfo();
//}

//bool craft::IsSuspendedAction() {
//  if(rpd.bSuccesfullyCompleted)
//    return false;
//
//  if(rpd.bCanBeSuspended)
//    return true;
//
//  return false;
//}

void craft::Handle()
{DBGLN;
  rpd.rc.integrityCheck();

  character* Actor = GetActor();
  if(rpd.rc.H()==NULL)
    rpd.rc.SetHumanoid(Actor);

//  if(rpd.itSpawnCfg==0 && rpd.otSpawnCfg==0)
  if(rpd.otSpawnType==CTT_NONE && rpd.itSpawnType==CIT_NONE)
    ABORT("craft:Handle nothing? %s",rpd.dbgInfo().CStr());

//  if(rpd.itToolID!=0 && rpd.itTool==NULL)
  if(rpd.itToolID!=0){
    rpd.itTool = game::SearchItem(rpd.itToolID); //must keep searching it as it may have been destroyed.
    //TODO if a tool was broken and gets fixed, it's old ID will vanish!!! how to handle it!??!?!
    if(rpd.itTool==NULL){
      ADD_MESSAGE("The unmodified tool to craft this is missing.",rpd.itTool->GetName(DEFINITE).CStr());
      rpd.bFailed=true;
    }
    DBGEXEC(if(rpd.itTool!=NULL)DBGSV2(rpd.itTool->GetLSquareUnder()->GetPos()));
  }
  DBG6(rpd.itToolID,rpd.itTool,rpd.itSpawnCfg,rpd.otSpawnCfg,rpd.itSpawnType,rpd.otSpawnType);

  lsquare* lsqrActor = Actor->GetLSquareUnder(); DBGSV2(lsqrActor->GetPos());
  level* lvl = lsqrActor->GetLevel();

  if(rpd.itTool!=NULL && rpd.itTool->GetLSquareUnder()!=lsqrActor)//rpd.itTool!=Actor->GetMainWielded())
  {DBGLN; //TODO re-mainWield it
    ADD_MESSAGE("%s went missing.",rpd.itTool->GetName(DEFINITE).CStr());
    Terminate(false); //may suspend
    return;
  }

  if(Actor->GetPos()!=rpd.v2PlayerCraftingAt){ //in case player is teleported
    ADD_MESSAGE("You need to move back to where you started crafting.");
    Terminate(false); //may suspend
    return;
  }

  rpd.rc.integrityCheck();
  DBGSV2(rpd.v2PlaceAt);
  rpd.lsqrWhere = Actor->GetNearLSquare(rpd.v2PlaceAt); //near? it is abs map pos...
  olterrain* oltExisting = rpd.lsqrWhere->GetOLTerrain();
  if(rpd.otSpawnType!=CTT_NONE && oltExisting!=NULL){DBGLN;
//  ADD_MESSAGE("%s cannot be placed there.", rpd.otSpawn->GetName(DEFINITE).CStr()); //TODO like in case something is placed there before ending the construction?
    ADD_MESSAGE("Unable to place it there."); //TODO like in case something is placed there before ending the construction? but what and how?
    Terminate(false); //may suspend
    return;
  }

//  int Damage = Actor->GetAttribute(ARM_STRENGTH) * Tool->GetMainMaterial()->GetStrengthValue() / 500;
  rpd.iBaseTurnsToFinish--; //TODO is this way correct? as long one Handle() call per turn will work.

  rpd.rc.integrityCheck();
  int iStrongerXplod=0;
  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL){ //ABORT("ingredient id %d not found",rpd.ingredientsIDs[i]);
      /**
       * ex.: if something catches fire and is destroyed before the crafting ends.
       */
      ADD_MESSAGE("a required ingredient was destroyed...");DBG1(rpd.ingredientsIDs[i]);
      rpd.bFailed=true; //TODO a crash happens in this line, how? memory corruption? if the tiny explosions trigger things on the floor like wands
    }

    // a magpie or siren may have taken it
    if(it->GetSquareUnder()!=lsqrActor){
      ADD_MESSAGE("%s ingredient went missing...",it->GetName(DEFINITE).CStr());
      Terminate(false); //may suspend
      return;
    }

    //TODO once: apply wands, release rings/ammys effects, xplod str 5+ if enchanteds +1 +2 etc
    if(!craftcore::canBeCrafted(it)){ //basically contains some kind of magic
      if(it->GetEnchantment()!=0)
        iStrongerXplod+=abs(it->GetEnchantment());
      else
        iStrongerXplod++; //TODO could add based on how hazardous the magic type is ex. fireball wand would be +100 or something like that..
    }
  }

  v2 v2XplodAt;

  rpd.rc.integrityCheck();
  if(!rpd.v2AnvilLocation.Is0()){
    olterrain* ot = lvl->GetLSquare(rpd.v2AnvilLocation)->GetOLTerrain();
    if(ot==NULL || ot->GetConfig()!=ANVIL){
      ADD_MESSAGE("The anvil was destroyed!");
      rpd.bFailed=true;
    }

    v2XplodAt=rpd.v2AnvilLocation;
  }else
  if(!rpd.v2ForgeLocation.Is0()){
    olterrain* otForge = lvl->GetLSquare(rpd.v2ForgeLocation)->GetOLTerrain();
    if(otForge==NULL || otForge->GetConfig()!=FORGE){
      ADD_MESSAGE("The forge was destroyed!");
      rpd.bFailed=true;
    }

    v2XplodAt=rpd.v2ForgeLocation;
  }

  rpd.rc.integrityCheck();
  int xplodStr = 0;
//  if(!v2XplodAt.Is0()){DBGSV2(v2XplodAt);
  if(rpd.fDifficulty>1.0){
    int xplodXtra=0;
    for(int i=0;i<iStrongerXplod;i++)
      xplodXtra+=clock()%5;

    /**
     * To fumble, base reference is 15% chance at a craft skill of 20.
     * ex.: Craft skill of 10 will have 30% fumble chance.
     */
    int iFumblePower=0;
    int iLuckPerc=clock()%100;
    static const float fBaseCraftSkillToNormalFumble=20.0*rpd.fDifficulty;
    static const int iBaseFumbleChancePerc=15;
    int iFumbleBase=iBaseFumbleChancePerc/(craftcore::CraftSkill(Actor)/fBaseCraftSkillToNormalFumble); //ex.: 30%
    if(iFumbleBase>99)iFumbleBase=99; //%1 granted luck
    int iDiv=0;
    iDiv=1;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <=30%
    iDiv=2;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <=15%
    iDiv=4;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <= 7%
    iDiv=8;if(iFumbleBase>iDiv && iLuckPerc<=iFumbleBase/iDiv)iFumblePower++; //ex.: <= 3%
    if(iLuckPerc<=1)iFumblePower++; //always have 1% weakest xplod chance
    if(clock()%100<=iFumblePower){
      if(!rpd.bSpawnBroken){
        rpd.iBaseTurnsToFinish/=2;
        if(!rpd.bCanBeBroken && rpd.iBaseTurnsToFinish>1) //to insta provide the obvious messy lump
          rpd.iBaseTurnsToFinish=1;
      }
      rpd.bSpawnBroken=true; //current max chance per round of spawning broken is 5%
    }
    xplodStr = iFumblePower;
    if(xplodStr>0){DBG2(xplodStr,rpd.dbgInfo().CStr());
      xplodStr+=clock()%5+xplodXtra; //reference: weak lantern xplod str is 5
      //TODO anvil should always be near the forge. Anvil have no sparks. Keeping messages like that til related code is improved
    }
  }

  rpd.rc.integrityCheck();
  if(rpd.bFailed){
    Terminate(false); //won't suspend!
    return;
  }

  rpd.bSuccesfullyCompleted = rpd.iBaseTurnsToFinish==0;
  festring fsCreated;
  festring fsMsg("");
//  int Case = INDEFINITE;
  if(rpd.bSuccesfullyCompleted)
  {DBGLN;
    if(rpd.itSpawnType!=CIT_NONE){DBGLN;
      fsMsg << "You crafted "<< rpd.SpawnItem();
    }

    int iWallMaterialConfig=-1;
//    if(rpd.otSpawn!=NULL){DBGLN;
    if(rpd.otSpawnType!=CTT_NONE){DBGLN;
      iWallMaterialConfig = rpd.otSpawnMatMainCfg;
      fsMsg << "You built " << rpd.SpawnTerrain();
    }

    festring fsIng,fsIngP;
    festring fsIngPrev,fsIngPPrev;
    int iCountEqual=1;
    festring fsIngMsg("");
    for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
      item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
      if(it==NULL)ABORT("ingredient id %d not found %s",rpd.ingredientsIDs[i],rpd.dbgInfo().CStr());
      it->RemoveFromSlot();DBGLN;

      bool bSendToHell=true;
      if(iWallMaterialConfig!=-1){DBGLN;
        if(it->GetMainMaterial()->GetConfig() != iWallMaterialConfig)
          bSendToHell=false;
      }

      if(bSendToHell){DBGLN;
        it->SendToHell();
      }else{DBGLN;
        //this way, the lower quality wall will still contain all stones in a non destructive way, is more fair
        //TODO what about amulet of phasing or ghost mode?
        it->MoveTo(rpd.lsqrWhere->GetStack());
      }DBGLN;

      fsIng.Empty();fsIng << it->GetName(INDEFINITE);DBGLN;
      fsIngP.Empty();fsIngP << it->GetName(PLURAL);DBGLN;
      if(fsCreated==fsIng)continue;

      bool bNewType = fsIngPrev!=fsIng;DBGLN;

      bool bDumpPrev = false;
      if(bNewType)
        bDumpPrev=true;
      if(i==rpd.ingredientsIDs.size()-1){DBGLN;
        bDumpPrev=true;
        fsIngPrev=fsIng;
      }

      if(bDumpPrev){DBGLN;
        if(fsIngMsg.GetSize()>0)
          fsIngMsg<<", ";

        if(iCountEqual>1){
          fsIngMsg << (iCountEqual+1) << " " << fsIngPPrev;
        }else{
          fsIngMsg << fsIngPrev;
        }

        iCountEqual=1;
      }else
        iCountEqual++;

      fsIngPrev.Empty();fsIngPrev<<fsIng;
      fsIngPPrev.Empty();fsIngPPrev<<fsIngP;
    }
    if(fsIngMsg.GetSize()>0) //TODO this needs improving, for plural etc, to look good
      fsMsg << " using " << fsIngMsg.CStr();
    fsMsg << ".";

    ADD_MESSAGE(fsMsg.CStr());

    Actor->DexterityAction(rpd.iAddDexterity);

    /* If the door was boobytrapped etc. and the character is dead, Action has already been deleted */

    if(!Actor->IsEnabled())
      return;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  //// ATTENTION!!! twiglight zone below here TODO name this more technically... ////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////////////

  /************************************************************************************************
   * ATTENTION!!! Save these here because the EditNP call below can cause 'this' to be terminated
   * and DELETED!!!!!!. if the player decides to stop crafting because of becoming hungry.
   *******************/
  rpd.rc.integrityCheck();

  truth MoveCraftTool = this->MoveCraftTool;DBGLN;
  ulong RightBackupID = this->RightBackupID;
  ulong LeftBackupID = this->LeftBackupID;
  bool bSuccesfullyCompleted = rpd.bSuccesfullyCompleted;
  lsquare* lsqrWhere = rpd.lsqrWhere;

  Actor->EditExperience(DEXTERITY, 200, 1 << 5);DBGLN; //TODO are these values good for crafting?
  Actor->EditAP(-200000 / APBonus(Actor->GetAttribute(DEXTERITY)));
  Actor->EditStamina(-1000 / Actor->GetAttribute(ARM_STRENGTH), false);
  Actor->EditNP(-500); ////////////////////////// CRITICAL BELOW HERE //////////////////////////////

  truth AlreadyTerminated = Actor->GetAction() != this;DBGLN;
  truth Stopped = bSuccesfullyCompleted || AlreadyTerminated;

  if(bSuccesfullyCompleted && !AlreadyTerminated)
    Terminate(true);

  if(Stopped)
  {DBGLN;
    if(MoveCraftTool && Actor->GetMainWielded())
      Actor->GetMainWielded()->MoveTo(Actor->GetStack());

    item* RightBackup = game::SearchItem(RightBackupID);

    if(RightBackup && RightBackup->Exists() && Actor->IsOver(RightBackup))
    {DBGLN;
      RightBackup->RemoveFromSlot();
      Actor->SetRightWielded(RightBackup);
    }

    item* LeftBackup = game::SearchItem(LeftBackupID);

    if(LeftBackup && LeftBackup->Exists() && Actor->IsOver(LeftBackup))
    {DBGLN;
      LeftBackup->RemoveFromSlot();
      Actor->SetLeftWielded(LeftBackup);
    }
  }

  /**
   * explosions may trigger something that apparently terminates the action and so also deletes it's recipedata
   * TODO what is being triggered?
   */
  if(!v2XplodAt.Is0() && xplodStr>0){
    lsqrWhere->GetLevel()->Explosion(Actor, CONST_S("killed by the forge heat"), v2XplodAt, xplodStr, false, false);
    ADD_MESSAGE("Forging sparks explode lightly."); //this will let sfx play TODO better message? the idea is to make forging a bit hazardous,
  }

  if(!bSuccesfullyCompleted)
    game::DrawEverything();
}

bool craft::IsSuspending(){
  return GetActor()->IsPlayer() && rpd.rc.IsCanBeSuspended() && !rpd.bFailed;
}

void craft::Terminate(truth Finished)
{DBGLN;
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(Finished){
    craftcore::SetSuspended(NULL);
  }else{
    if(IsSuspending()){
      ADD_MESSAGE("You suspend crafting (do not modify tools and ingredients)."); //TODO this message refers to a too technical subject: if a tool gets fixed, it's ID will vanish. Not sure if this message could be improved...
      craftcore::SetSuspended(&rpd);
    }else{
      if(GetActor()->IsPlayer())
        ADD_MESSAGE("You stop crafting.");
      else if(GetActor()->CanBeSeenByPlayer())
        ADD_MESSAGE("%s stops crafting.", GetActor()->CHAR_NAME(DEFINITE));

      craftcore::SetSuspended(NULL);
    }
  }

  action::Terminate(Finished);DBGLN;
}

craft::~craft(){DBGLN; // called from Terminate()
  // cleanups if not finished

//  // will be respawned
//  rpd.SendSpawnItemToHell();
//
//  if(!IsSuspending())
//    rpd.SendTerrainToHell();
}

void dig::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << SquareDug << MoveDigger << RightBackupID << LeftBackupID;
}

void dig::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> SquareDug >> MoveDigger >> RightBackupID >> LeftBackupID;
}

void dig::Handle()
{
  character* Actor = GetActor();
  item* Digger = Actor->GetMainWielded();

  if(!Digger)
  {
    Terminate(false);
    return;
  }

  lsquare* Square = Actor->GetNearLSquare(SquareDug);
  olterrain* Terrain = Square->GetOLTerrain();

  if(!Terrain || !Terrain->CanBeDestroyed() || !Terrain->GetMainMaterial()->CanBeDug(Digger->GetMainMaterial()))
  {
    Terminate(false);
    return;
  }

  int Damage = Actor->GetAttribute(ARM_STRENGTH) * Digger->GetMainMaterial()->GetStrengthValue() / 500;
  Terrain->EditHP(-Max(Damage, 1));

  /* Save these here because the EditNP call below can cause 'this' to be terminated
     and deleted, if the player decides to stop digging because of becoming hungry. */

  truth MoveDigger = this->MoveDigger;
  ulong RightBackupID = this->RightBackupID;
  ulong LeftBackupID = this->LeftBackupID;

  Actor->EditExperience(ARM_STRENGTH, 200, 1 << 5);
  Actor->EditAP(-200000 / APBonus(Actor->GetAttribute(DEXTERITY)));
  Actor->EditNP(-500);

  truth TerrainDestroyed = Terrain->GetHP() <= 0;
  truth AlreadyTerminated = Actor->GetAction() != this;
  truth StoppedDigging = TerrainDestroyed || AlreadyTerminated;

  if(TerrainDestroyed)
  {
    if(Square->CanBeSeenByPlayer())
      ADD_MESSAGE("%s", Terrain->GetDigMessage().CStr());

    Terrain->Break();

    /* If the door was boobytrapped etc. and the character is dead, Action has already been deleted */

    if(!Actor->IsEnabled())
      return;

    if(!AlreadyTerminated)
      Terminate(true);
  }

  if(StoppedDigging)
  {
    if(MoveDigger && Actor->GetMainWielded())
      Actor->GetMainWielded()->MoveTo(Actor->GetStack());

    item* RightBackup = game::SearchItem(RightBackupID);

    if(RightBackup && RightBackup->Exists() && Actor->IsOver(RightBackup))
    {
      RightBackup->RemoveFromSlot();
      Actor->SetRightWielded(RightBackup);
    }

    item* LeftBackup = game::SearchItem(LeftBackupID);

    if(LeftBackup && LeftBackup->Exists() && Actor->IsOver(LeftBackup))
    {
      LeftBackup->RemoveFromSlot();
      Actor->SetLeftWielded(LeftBackup);
    }
  }

  if(!TerrainDestroyed)
    game::DrawEverything();
}

void dig::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(!Finished)
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop digging.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops digging.", GetActor()->CHAR_NAME(DEFINITE));
  }

  action::Terminate(Finished);
}

void go::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Direction << WalkingInOpen;
}

void go::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Direction >> WalkingInOpen;
}

void go::Handle()
{
  GetActor()->EditAP(GetActor()->GetStateAPGain(100)); // gum solution
  GetActor()->GoOn(this);
}

void study::Handle()
{
  item* Literature = game::SearchItem(LiteratureID);

  if(!Literature || !Literature->Exists() || !Actor->IsOver(Literature))
  {
    Terminate(false);
    return;
  }

  if(GetActor()->GetLSquareUnder()->IsDark() && !game::GetSeeWholeMapCheatMode())
  {
    ADD_MESSAGE("It is too dark to read now.");
    Terminate(false);
    return;
  }

  if(game::CompareLightToInt(GetActor()->GetLSquareUnder()->GetLuminance(), 115) < 0)
    GetActor()->EditExperience(PERCEPTION, -50, 1 << 1);

  if(!Counter)
  {
    Terminate(true);
    return;
  }

  if(GetActor()->GetAttribute(INTELLIGENCE) >= Counter)
    Counter = 0;
  else
    Counter -= GetActor()->GetAttribute(INTELLIGENCE);
}

void study::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;
  item* Literature = game::SearchItem(LiteratureID);

  if(Finished)
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You finish reading %s.", Literature->CHAR_NAME(DEFINITE));
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s finishes reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));

    character* Actor = GetActor();
    Literature->FinishReading(Actor);

    if(!Actor->IsEnabled())
      return;
  }
  else if(Literature && Literature->Exists())
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop reading %s.", Literature->CHAR_NAME(DEFINITE));
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops reading %s.", GetActor()->CHAR_NAME(DEFINITE), Literature->CHAR_NAME(DEFINITE));
  }
  else
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop reading.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops reading.", GetActor()->CHAR_NAME(DEFINITE));
  }

  action::Terminate(Finished);
}

void study::Save(outputfile& SaveFile) const
{
  action::Save(SaveFile);
  SaveFile << Counter << LiteratureID;
}

void study::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> Counter >> LiteratureID;
}

truth go::TryDisplace()
{
  Terminate(false);
  return true;
}

void unconsciousness::RaiseCounterTo(int What)
{
  if(Counter < What)
    Counter = What;
}
