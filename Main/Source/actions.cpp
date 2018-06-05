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

  if(Consuming->Consume(Actor, 500) && Actor->GetAction() == this && Actor->IsEnabled())
    Terminate(true);
  else if(Actor->GetHungerState() == OVER_FED)
  {
    Actor->DeActivateVoluntaryAction(CONST_S("You are about to choke on this stuff."));
    Actor->Vomit(Actor->GetPos(), 500 + RAND() % 500);
  }
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
  SaveFile << iTurnsToFinish << ToolRequired << itWhatID << otWhat << v2PlaceAt << MoveCraftTool << RightBackupID << LeftBackupID;
}

void craft::Load(inputfile& SaveFile)
{
  action::Load(SaveFile);
  SaveFile >> iTurnsToFinish >> ToolRequired >> itWhatID >> otWhat >> v2PlaceAt >> MoveCraftTool >> RightBackupID >> LeftBackupID;
}

void craft::Handle()
{
  character* Actor = GetActor();
  item* Tool = Actor->GetMainWielded();

  if(ToolRequired && Tool==NULL)
  {
    ADD_MESSAGE("You have not the required tool to craft this.");
    Terminate(false);
    return;
  }

  lsquare* lsqrWhere = Actor->GetNearLSquare(v2PlaceAt);
  olterrain* oltExisting = lsqrWhere->GetOLTerrain();
  if(otWhat!=NULL && oltExisting!=NULL){
    ADD_MESSAGE("%s cannot be placed there.", otWhat->GetName(DEFINITE).CStr());
    Terminate(false);
    return;
  }

//  int Damage = Actor->GetAttribute(ARM_STRENGTH) * Tool->GetMainMaterial()->GetStrengthValue() / 500;
  iTurnsToFinish--; //TODO is this way correct? as long one Handle() call per turn will work.

  /* Save these here because the EditNP call below can cause 'this' to be terminated
     and deleted, if the player decides to stop digging because of becoming hungry. */

  truth MoveDigger = this->MoveCraftTool;
  ulong RightBackupID = this->RightBackupID;
  ulong LeftBackupID = this->LeftBackupID;

  Actor->EditExperience(DEXTERITY, 200, 1 << 5); //TODO are these values good for crafting?
  Actor->EditAP(-200000 / APBonus(Actor->GetAttribute(DEXTERITY)));
  Actor->EditNP(-500);

  truth finished = iTurnsToFinish==0;
  truth AlreadyTerminated = Actor->GetAction() != this;
  truth Stopped = finished || AlreadyTerminated;

  festring fsCreated;
  festring fsMsg("");
  int Case = INDEFINITE;
  if(finished)
  {
    if(itWhatID!=0){
      if(itWhat==NULL)
        itWhat = game::SearchItem(itWhatID);

      if(itWhat!=NULL){
        itWhat->MoveTo(Actor->GetStack());
        fsCreated << itWhat->GetName(Case);
        fsMsg << "You prepared " << fsCreated.CStr();
        itWhat=NULL; //see destructor
      }
    }

    int iWallMaterialConfig=-1;
    if(otWhat!=NULL){
      lsqrWhere->ChangeOLTerrainAndUpdateLights(otWhat);
      if(dynamic_cast<wall*>(otWhat)!=NULL)
        iWallMaterialConfig = otWhat->GetMainMaterial()->GetConfig();

//      if(lsqrWhere->CanBeSeenByPlayer())
      fsCreated << otWhat->GetName(Case);
      fsMsg << "You built " << fsCreated.CStr();

      otWhat=NULL; //see destructor
    }

    festring fsIng,fsIngP;
    festring fsIngPrev,fsIngPPrev;
    int iCountEqual=1;
    festring fsIngMsg("");
    for(int i=0;i<Ingredients.size();i++){
      item* it=game::SearchItem(Ingredients[i]);
      it->RemoveFromSlot();

      bool bSendToHell=true;
      if(iWallMaterialConfig!=-1){
        if(it->GetMainMaterial()->GetConfig() != iWallMaterialConfig)
          bSendToHell=false;
      }

      if(bSendToHell){
        it->SendToHell();
      }else{
        //this way, the lower quality wall will still contain all stones in a non destructive way, is more fair
        //TODO what about amulet of phasing or ghost mode?
        it->MoveTo(lsqrWhere->GetStack());
      }

      fsIng.Empty();fsIng << it->GetName(Case);
      fsIngP.Empty();fsIngP << it->GetName(PLURAL);
      if(fsCreated==fsIng)continue;

      bool bNewType = fsIngPrev!=fsIng;

      bool bDumpPrev = false;
      if(bNewType)
        bDumpPrev=true;
      if(i==Ingredients.size()-1){
        bDumpPrev=true;
        fsIngPrev=fsIng;
      }

      if(bDumpPrev){
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

    if(!AlreadyTerminated)
      Terminate(true);
  }

  if(Stopped)
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

  if(!finished)
    game::DrawEverything();
}

void craft::Terminate(truth Finished)
{
  if(Flags & TERMINATING)
    return;

  Flags |= TERMINATING;

  if(!Finished)
  {
    if(GetActor()->IsPlayer())
      ADD_MESSAGE("You stop crafting.");
    else if(GetActor()->CanBeSeenByPlayer())
      ADD_MESSAGE("%s stops crafting.", GetActor()->CHAR_NAME(DEFINITE));
  }

  action::Terminate(Finished);
}

craft::~craft(){ // called from Terminate()
  // cleanups if not finished

  if(itWhat && itWhat->Exists()){
    itWhat->SendToHell();
  }

  if(otWhat && otWhat->Exists()){
    otWhat->SendToHell();
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
