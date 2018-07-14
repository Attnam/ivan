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
{
  action::Save(SaveFile);
  SaveFile << rpd.iBaseTurnsToFinish << ToolRequired << itWhatID << rpd.itSpawnTot << rpd.otSpawn << rpd.v2PlaceAt
           << MoveCraftTool << RightBackupID << LeftBackupID << rpd.ingredientsIDs;
}

void craft::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> rpd.iBaseTurnsToFinish >> ToolRequired >> itWhatID >> rpd.itSpawnTot >> rpd.otSpawn >> rpd.v2PlaceAt
           >> MoveCraftTool >> RightBackupID >> LeftBackupID >> rpd.ingredientsIDs;
}

void craft::Handle()
{DBGLN;
  if(itWhatID!=0 && rpd.itSpawn==NULL)
    rpd.itSpawn = game::SearchItem(itWhatID); // do this here to work correctly at ~craft

  character* Actor = GetActor();
  item* Tool = Actor->GetMainWielded();DBGLN;

  if(ToolRequired && Tool==NULL)
  {DBGLN;
    ADD_MESSAGE("You have not the required tool to craft this."); //TODO like in case the tool is destroyed by sulf. acid?
    Terminate(false);
    return;
  }

  DBGSV2(rpd.v2PlaceAt);
  lsquare* lsqrWhere = Actor->GetNearLSquare(rpd.v2PlaceAt);
  olterrain* oltExisting = lsqrWhere->GetOLTerrain();
  if(rpd.otSpawn!=NULL && oltExisting!=NULL){DBGLN;
    ADD_MESSAGE("%s cannot be placed there.", rpd.otSpawn->GetName(DEFINITE).CStr()); //TODO like in case something is placed there before ending the construction?
    Terminate(false);
    return;
  }

//  int Damage = Actor->GetAttribute(ARM_STRENGTH) * Tool->GetMainMaterial()->GetStrengthValue() / 500;
  rpd.iBaseTurnsToFinish--; //TODO is this way correct? as long one Handle() call per turn will work.

  for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
    item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
    if(it==NULL){ //ABORT("ingredient id %d not found",rpd.ingredientsIDs[i]);
      /**
       * ex.: if something catches fire and is destroyed before the crafting ends.
       */
      ADD_MESSAGE("a required ingredient was destroyed...");DBG1(rpd.ingredientsIDs[i]);
      Terminate(false); //TODO a crash happens in this line, how? memory corruption? if the tiny explosions trigger things on the floor like wands
      return;
    }

    // a magpie or siren may have taken it
    if(it->GetSquareUnder()!=Actor->GetSquareUnder()){
      ADD_MESSAGE("%s ingredient went missing...",it->GetName(DEFINITE).CStr());
      Terminate(false);
      return;
    }

    if(!rpd.v2ForgeLocation.Is0()){
      int xplodXtra=0;
      if(!craftcore::canBeCrafted(it)){ //basically contains some kind of magic
        //TODO once: apply wands, release rings/ammys effects, xplod str 5+ if enchanteds +1 +2 etc
        xplodXtra=clock()%10;
      }

      int xplodStr=0;
      int iFumblePerc=clock()%100;
      float fSkill = ((Actor->GetAttribute(DEXTERITY)+Actor->GetAttribute(WISDOM))/2.0)/20.0;
      int iFumbleBase=20/fSkill;
      int iDiv=0;
      iDiv=1;if(iFumbleBase>iDiv && iFumblePerc<=iFumbleBase/iDiv)xplodStr++;
      iDiv=2;if(iFumbleBase>iDiv && iFumblePerc<=iFumbleBase/iDiv)xplodStr++;
      iDiv=4;if(iFumbleBase>iDiv && iFumblePerc<=iFumbleBase/iDiv)xplodStr++;
      if(iFumblePerc<=1)xplodStr++; //always have 1% weakest xplod chance
      if(xplodStr>0){
        xplodStr+=clock()%5+xplodXtra; //reference: weak lantern xplod str is 5
        lsqrWhere->GetLevel()->Explosion(Actor, CONST_S("killed by the forge heat"), rpd.v2ForgeLocation, xplodStr, false, true);
        ADD_MESSAGE("The forge sparks explode lightly."); //this will let sfx play TODO better message? the idea is to make forging a bit hazardous,
      }
    }

  }

  truth finished = rpd.iBaseTurnsToFinish==0;
  festring fsCreated;
  festring fsMsg("");
  int Case = INDEFINITE;
  if(finished)
  {DBGLN;
    if(rpd.itSpawn!=NULL){DBGLN;
      item* itChkAgain = game::SearchItem(itWhatID);
      if(itChkAgain!=rpd.itSpawn)
        ABORT("spawning item ID changed or vanished %d %d",itWhatID,itChkAgain!=NULL?itChkAgain->GetID():0); //could be a duplicate issue? like item::Fix(), //could be something near that exploded and destroyed it?

      item* itWhatTmp=rpd.itSpawn;
      rpd.itSpawn=NULL; //see ~craft
      itWhatID=0;

      if(rpd.itSpawnTot > 1){DBGLN;
        fsCreated << rpd.itSpawnTot << " " << itWhatTmp->GetNamePlural();DBGLN;
        for(int i=0;i<rpd.itSpawnTot-1;i++){ //-1 as the last one will be the original
          /**
           * IMPORTANT!!!
           * the duplicator will vanish with the item ID that is being duplicated
           */
          itWhatTmp->DuplicateToStack(Actor->GetStack());
        }
      }else{DBGLN;
        fsCreated << itWhatTmp->GetName(Case);
      }

      itWhatTmp->MoveTo(Actor->GetStack());DBGLN;

      fsMsg << "You prepared "<< fsCreated.CStr();
    }

    int iWallMaterialConfig=-1;
    if(rpd.otSpawn!=NULL){DBGLN;
      lsqrWhere->ChangeOLTerrainAndUpdateLights(rpd.otSpawn);
      if(dynamic_cast<wall*>(rpd.otSpawn)!=NULL)
        iWallMaterialConfig = rpd.otSpawn->GetMainMaterial()->GetConfig();

//      if(lsqrWhere->CanBeSeenByPlayer())
      fsCreated << rpd.otSpawn->GetName(Case);
      fsMsg << "You built " << fsCreated.CStr();

      rpd.otSpawn=NULL; //see ~craft
    }

    festring fsIng,fsIngP;
    festring fsIngPrev,fsIngPPrev;
    int iCountEqual=1;
    festring fsIngMsg("");
    for(int i=0;i<rpd.ingredientsIDs.size();i++){DBG1(rpd.ingredientsIDs[i]);
      item* it=game::SearchItem(rpd.ingredientsIDs[i]);DBGLN;
      if(it==NULL)ABORT("ingredient id %d not found",rpd.ingredientsIDs[i]);
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
        it->MoveTo(lsqrWhere->GetStack());
      }DBGLN;

      fsIng.Empty();fsIng << it->GetName(Case);DBGLN;
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
          fsIngMsg << iCountEqual << " " << fsIngPPrev;
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

    /* If the door was boobytrapped etc. and the character is dead, Action has already been deleted */

    if(!Actor->IsEnabled())
      return;
  }

  /*******************
   * ATTENTION!!! Save these here because the EditNP call below can cause 'this' to be terminated
   * and DELETED!!!!!!. if the player decides to stop crafting because of becoming hungry.
   *******************/
  truth MoveCraftTool = this->MoveCraftTool;DBGLN;
  ulong RightBackupID = this->RightBackupID;
  ulong LeftBackupID = this->LeftBackupID;

  Actor->EditExperience(DEXTERITY, 200, 1 << 5);DBGLN; //TODO are these values good for crafting?
  Actor->EditAP(-200000 / APBonus(Actor->GetAttribute(DEXTERITY)));
  Actor->EditNP(-500);
  Actor->EditStamina(-1000 / Actor->GetAttribute(ARM_STRENGTH), false);

  truth AlreadyTerminated = Actor->GetAction() != this;DBGLN;
  truth Stopped = finished || AlreadyTerminated;

  if(finished && !AlreadyTerminated)
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

  if(!finished)
    game::DrawEverything();
}

void craft::Terminate(truth Finished)
{DBGLN;
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(!Finished)
  {DBGLN;
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop crafting.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops crafting.", GetActor()->CHAR_NAME(DEFINITE));
  }

  action::Terminate(Finished);DBGLN;
}

craft::~craft(){DBGLN; // called from Terminate()
  // cleanups if not finished

  if(rpd.itSpawn && rpd.itSpawn->Exists()){DBGLN;
    rpd.itSpawn->RemoveFromSlot(); //just in case it is required one day, this prevents a lot of trouble...
    rpd.itSpawn->SendToHell();
  }

  if(rpd.otSpawn && rpd.otSpawn->Exists()){DBGLN;
    rpd.otSpawn->SendToHell();
  }
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
