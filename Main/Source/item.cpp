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

/* Compiled through itemset.cpp */

#include "dbgmsgproj.h"

cchar* ToHitValueDescription[] =
{
  "unbelievably inaccurate",
  "extremely inaccurate",
  "inaccurate",
  "decently accurate",
  "accurate",
  "highly accurate",
  "extremely accurate",
  "unbelievably accurate"
};
cchar* StrengthValueDescription[] =
{
  "fragile",
  "rather sturdy",
  "sturdy",
  "strong",
  "very strong",
  "extremely strong",
  "almost unbreakable"
};

itemprototype::itemprototype(const itemprototype* Base, itemspawner Spawner, itemcloner Cloner, cchar* ClassID)
: Base(Base), Spawner(Spawner), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<item>::Add(this); }

truth itemdatabase::AllowRandomInstantiation() const { return !(Config & S_LOCK_ID); }

item::item() : Slot(0), CloneMotherID(0), Fluid(0), LifeExpectancy(0), ItemFlags(0), iRotateFlyingThrownStep(0) { }
truth item::IsOnGround() const { return Slot[0]->IsOnGround(); }
truth item::IsSimilarTo(item* Item) const { return Item->GetType() == GetType() && Item->GetConfig() == GetConfig(); }
double item::GetBaseDamage() const { return Max(0., sqrt(5e-5 * GetWeaponStrength()) + GetDamageBonus()); }
int item::GetBaseMinDamage() const { return int(GetBaseDamage() * 0.75); }
int item::GetBaseMaxDamage() const { return int(GetBaseDamage() * 1.25) + 1; }
int item::GetBaseToHitValue() const { return int(10000. / (1000 + GetWeight()) + GetTHVBonus()); }
int item::GetBaseBlockValue() const { return int((10000. / (1000 + GetWeight()) + GetTHVBonus()) * GetBlockModifier() / 10000.); }
truth item::IsInCorrectSlot(int I) const { return I == RIGHT_WIELDED_INDEX || I == LEFT_WIELDED_INDEX; }
truth item::IsInCorrectSlot() const { return IsInCorrectSlot(static_cast<gearslot*>(*Slot)->GetEquipmentIndex()); }
int item::GetEquipmentIndex() const { return static_cast<gearslot*>(*Slot)->GetEquipmentIndex(); }
int item::GetGraphicsContainerIndex() const { return GR_ITEM; }
truth item::IsBroken() const { return GetConfig() & BROKEN; }
truth item::IsFood() const { return DataBase->Category & FOOD; }
cchar* item::GetBreakVerb() const { return "breaks"; }
square* item::GetSquareUnderEntity(int I) const { return GetSquareUnder(I); }
square* item::GetSquareUnder(int I) const { return Slot[I] ? Slot[I]->GetSquareUnder() : 0; }
lsquare* item::GetLSquareUnder(int I) const { return static_cast<lsquare*>(Slot[I]->GetSquareUnder()); }
void item::SignalStackAdd(stackslot* StackSlot, void (stack::*)(item*, truth)) { Slot[0] = StackSlot; }
truth item::IsAnimated() const { return GraphicData.AnimationFrames > 1 || (Fluid && ShowFluids()) || (IsBurning()); }
truth item::IsRusted() const { return MainMaterial->GetRustLevel() != NOT_RUSTED; }
truth item::IsBurnt() const { return MainMaterial->GetBurnLevel() != NOT_BURNT; }
truth item::IsEatable(ccharacter* Eater) const { return GetConsumeMaterial(Eater, &material::IsSolid) && IsConsumable() && !IsBurning(); }
truth item::IsDrinkable(ccharacter* Eater) const { return GetConsumeMaterial(Eater, &material::IsLiquid) && IsConsumable() && !IsBurning(); }
truth item::IsValidRecipeIngredient(ccharacter*) const { return ValidRecipeIngredient; }
pixelpredicate item::GetFluidPixelAllowedPredicate() const { return &rawbitmap::IsTransparent; }
void item::Cannibalize() { Flags |= CANNIBALIZED; }
material* item::SetMainMaterial(material* NewMaterial, int SpecialFlags)
{ return SetMaterial(MainMaterial, NewMaterial, GetDefaultMainVolume(), SpecialFlags); }
void item::InitMaterials(const materialscript* M, const materialscript*, truth CUP)
{ InitMaterials(M->Instantiate(), CUP); }
int item::GetMainMaterialRustLevel() const { return MainMaterial->GetRustLevel(); }

item::item(citem& Item)
: object(Item), Slot(0), Size(Item.Size), DataBase(Item.DataBase), Volume(Item.Volume), Weight(Item.Weight),
  iRotateFlyingThrownStep(Item.iRotateFlyingThrownStep),
  Fluid(0), SquaresUnder(Item.SquaresUnder), LifeExpectancy(Item.LifeExpectancy), ItemFlags(Item.ItemFlags)
{
  Flags &= ENTITY_FLAGS|SQUARE_POSITION_BITS;
  ID = game::CreateNewItemID(this);

  CloneMotherID = new idholder(Item.ID);
  idholder* TI = CloneMotherID;
  for(idholder* II = Item.CloneMotherID; II; II = II->Next)
    TI = TI->Next = new idholder(II->ID);
  TI->Next = 0;

  Slot = new slot*[SquaresUnder];
  for(int c = 0; c < SquaresUnder; ++c)
    Slot[c] = 0;
}

item::~item()
{
  delete [] Slot;
  game::RemoveItemID(ID);

  fluid** FP = Fluid;

  if(FP)
  {
    for(int c = 0; c < SquaresUnder; ++c)
      for(fluid* F = FP[c]; F;)
      {
        fluid* ToDel = F;
        F = F->Next;
        delete ToDel;
      }

    delete [] FP;
  }

  for(idholder* I = CloneMotherID; I;)
  {
    idholder* ToDel = I;
    I = I->Next;
    delete ToDel;
  }
}

void item::Fly(character* Thrower, int Direction, int Force, bool bTryStartThrownRotation)
{
  if(ivanconfig::GetRotateTimesPerSquare() > 0)
  {
    iRotateFlyingThrownStep=0; //simple granted reset
  }
  lsquare* LandingSquare=NULL;

  int Range = Force * 25 / Max(long(sqrt(GetWeight())), 1L);

  lsquare* LSquareUnder = GetLSquareUnder();
  RemoveFromSlot();
  LSquareUnder->GetStack()->AddItem(this, false);
  LandingSquare=LSquareUnder;

  if(!Range || GetSquaresUnder() != 1)
  {
    if(GetLSquareUnder()->GetRoom())
      GetLSquareUnder()->GetRoom()->AddItemEffect(this);

    return;
  }

  if(Direction == RANDOM_DIR)
    Direction = RAND() & 7;

  v2 StartingPos = GetPos();
  v2 Pos = StartingPos;
  v2 DirVector = game::GetMoveVector(Direction);
  truth Breaks = false;
  double BaseDamage, BaseToHitValue;

  /*** check ***/
  int iRotateTimes = ivanconfig::GetRotateTimesPerSquare();

  if(Thrower)
  {
    int Bonus = Thrower->IsHumanoid() ? Thrower->GetCWeaponSkill(GetWeaponCategory())->GetBonus() : 1000;
    BaseDamage = sqrt(5e-12 * GetWeaponStrength() * Force / Range) * Bonus;
    BaseToHitValue = 10 * Bonus * Thrower->GetMoveEase()
                     / (500 + GetWeight()) * Thrower->GetAttribute(DEXTERITY)
                     * sqrt(2.5e-8 * Thrower->GetAttribute(PERCEPTION)) / Range;

    if(bTryStartThrownRotation && ivanconfig::GetRotateTimesPerSquare()>0)
      iRotateFlyingThrownStep = (clock()%2)==0 ? 1 : -1; //init rotation

    if(!Thrower->IsPlayer() && iRotateTimes>1)
      iRotateTimes=1; //disable "dramatic" rotations from NPCs but still keep minimum if enabled
  }
  else
  {
    BaseDamage = sqrt(5e-6 * GetWeaponStrength() * Force / Range);
    BaseToHitValue = 10 * 100 / (500 + GetWeight()) / Range;
    iRotateTimes=0;
  }

  int RangeLeft;

  truth Draw=false;
  float fFlyDelay = 0.03;
  bool bLowerRotationsPerSqr = iRotateTimes==5;
  for(RangeLeft = Range; RangeLeft; --RangeLeft)
  {
    if(!GetLevel()->IsValidPos(Pos + DirVector))
      break;

    lsquare* JustHit = GetNearLSquare(Pos + DirVector);

    if(!JustHit->IsFlyable())
    {
      Breaks = true;
      JustHit->GetOLTerrain()->HasBeenHitByItem(Thrower, this, int(BaseDamage * sqrt(RangeLeft)));
      break;
    }
    else
    {
      clock_t StartTime = clock();
      Pos += DirVector;
      RemoveFromSlot();
      JustHit->GetStack()->AddItem(this, false);
      LandingSquare=JustHit;
      Draw = game::OnScreen(JustHit->GetPos()) && JustHit->CanBeSeenByPlayer();

      if(Draw)
        game::DrawEverything();

      if(JustHit->GetCharacter())
      {
        int Damage = int(BaseDamage * sqrt(RangeLeft));
        double ToHitValue = BaseToHitValue * RangeLeft;
        int Returned = HitCharacter(Thrower, JustHit->GetCharacter(), Damage, ToHitValue, Direction);

        if(Returned == HIT)
          Breaks = true;

        if(Returned != MISSED)
          break;
      }

      if(Draw)
        while(clock() - StartTime < fFlyDelay * CLOCKS_PER_SEC);

      if(ivanconfig::GetRotateTimesPerSquare()>0 && iRotateFlyingThrownStep!=0){
        if(iRotateTimes==1){
          iRotateFlyingThrownStep += iRotateFlyingThrownStep>0 ? 1 : -1; //next rotation step on next square
        }else{ //if rotation steps is >= 2 rotate at least one more time on the same square
          for(int i=0;i<(iRotateTimes-1);i++){
            iRotateFlyingThrownStep += iRotateFlyingThrownStep>0 ? 1 : -1;
            if(Draw){
              StartTime = clock();
              RemoveFromSlot();JustHit->GetStack()->AddItem(this, false); //TODO find a better way then remove and re-add to same square to redraw...
              game::DrawEverything();
              if(bLowerRotationsPerSqr){
                iRotateTimes--;
                if(iRotateTimes<1)iRotateTimes=1;
              }
              //while(clock() - StartTime < fFlyDelay * CLOCKS_PER_SEC);
            }
          }
        }
      }

    }

  }

  if(ivanconfig::GetRotateTimesPerSquare()>0 && iRotateFlyingThrownStep!=0){ //must be disabled before exiting Fly()
    iRotateFlyingThrownStep=4; //default rotation is w/o the rotation flags at the switch(){}
    //force redraw at default rotation to avoid another spin when player moves TODO how to let it stay in the last rotation?
    RemoveFromSlot();LandingSquare->GetStack()->AddItem(this, false); //TODO find a better way then remove and re-add to same square...
    game::DrawEverything();
    iRotateFlyingThrownStep=0; //disables rotation
  }

  if(Breaks)
    ReceiveDamage(Thrower, int(sqrt(GetWeight() * RangeLeft) / 10), THROW|PHYSICAL_DAMAGE, Direction);

  if(Exists() && GetLSquareUnder()->GetRoom())
    GetLSquareUnder()->GetRoom()->AddItemEffect(this);
}

int item::HitCharacter(character* Thrower, character* Dude, int Damage, double ToHitValue, int Direction)
{
  if(Dude->Catches(this))
    return CATCHED;

  if(Thrower && !EffectIsGood())
    Thrower->Hostility(Dude);

  if(Dude->DodgesFlyingItem(this, ToHitValue))
  {
    if(Dude->IsPlayer())
      ADD_MESSAGE("%s misses you.", CHAR_NAME(DEFINITE));
    else if(Dude->CanBeSeenByPlayer())
      ADD_MESSAGE("%s misses %s.", CHAR_NAME(DEFINITE), Dude->CHAR_NAME(DEFINITE));

    return MISSED;
  }

  Dude->HasBeenHitByItem(Thrower, this, Damage, ToHitValue, Direction);
  return HIT;
}

double item::GetWeaponStrength() const
{
  return GetFormModifier() * GetMainMaterial()->GetStrengthValue() * sqrt(GetMainMaterial()->GetWeight());
}

int item::GetStrengthRequirement() const
{
  double WeightTimesSize = GetWeight() * GetSize();
  return int(1.25e-10 * WeightTimesSize * WeightTimesSize);
}

truth item::Apply(character* Applier)
{
  if(Applier->IsPlayer())
    ADD_MESSAGE("You can't apply this!");

  return false;
}

/* Returns truth that tells whether the Polymorph really happened */

truth item::Polymorph(character* Polymorpher, stack* CurrentStack)
{
  if(!IsPolymorphable())
    return false;
  else
  {
    if(Polymorpher && IsOnGround())
    {
      room* Room = GetRoom();

      if(Room)
        Room->HostileAction(Polymorpher);
    }

    if(GetSquarePosition() != CENTER)
    {
      stack* Stack = CurrentStack->GetLSquareUnder()->GetStackOfAdjacentSquare(GetSquarePosition());

      if(Stack)
        CurrentStack = Stack;
    }

    CurrentStack->AddItem(protosystem::BalancedCreateItem(0, MAX_PRICE, ANY_CATEGORY, 0, 0, 0, true));
    RemoveFromSlot();
    SendToHell();
    return true;
  }
}

truth item::Polymorph(character* Polymorpher, character* Wielder)
{
  if(!IsPolymorphable())
    return false;
  else if(!Wielder->Equips(this))
    return false;
  else
  {
    if(Polymorpher && Wielder)
    {
      Polymorpher->Hostility(Wielder);
    }

    item* NewItem = protosystem::BalancedCreateItem(0, MAX_PRICE, ANY_CATEGORY, 0, 0, 0, true);
    int EquipSlot = GetEquipmentIndex();

    if(Wielder->IsPlayer())
      ADD_MESSAGE("Your %s polymorphs into %s.", CHAR_NAME(UNARTICLED), NewItem->CHAR_NAME(INDEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s's %s polymorphs into %s.", Wielder->CHAR_NAME(DEFINITE), CHAR_NAME(UNARTICLED), NewItem->CHAR_NAME(INDEFINITE));

    RemoveFromSlot();
    SendToHell();

    switch (EquipSlot)
    {
      /*
      case HELMET_INDEX: Wielder->SetHelmet(NewItem); break;
      case AMULET_INDEX: Wielder->SetAmulet(NewItem); break;
      case CLOAK_INDEX: Wielder->SetCloak(NewItem); break;
      case BODY_ARMOR_INDEX: Wielder->SetBodyArmor(NewItem); break;
      case BELT_INDEX: Wielder->SetBelt(NewItem); break;
      */
      case RIGHT_WIELDED_INDEX: Wielder->SetRightWielded(NewItem); break;
      case LEFT_WIELDED_INDEX: Wielder->SetLeftWielded(NewItem); break;
      /*
      case RIGHT_RING_INDEX: Wielder->SetRightRing(NewItem); break;
      case LEFT_RING_INDEX: Wielder->SetLeftRing(NewItem); break;
      case RIGHT_GAUNTLET_INDEX: Wielder->SetRightGauntlet(NewItem); break;
      case LEFT_GAUNTLET_INDEX: Wielder->SetLeftGauntlet(NewItem); break;
      case RIGHT_BOOT_INDEX: Wielder->SetRightBoot(NewItem); break;
      case LEFT_BOOT_INDEX: Wielder->SetLeftBoot(NewItem); break;
      */
      default: Wielder->ReceiveItemAsPresent(NewItem); break;
    }

    if(Wielder->IsPlayer())
      game::AskForKeyPress(CONST_S("Equipment polymorphed! [press any key to continue]"));
    return true;
  }
}

/* Returns truth that tells whether the alchemical conversion really happened. */

truth item::Alchemize(character* Midas, stack* CurrentStack)
{
  if(IsQuestItem())
    return false;
  else
  {
    if(Midas && IsOnGround())
    {
      room* Room = GetRoom();

      if(Room)
        Room->HostileAction(Midas);
    }

    long Price = GetTruePrice();

    if(Price)
    {
      Price /= 4; /* slightly lower than with 10 Cha */
      ADD_MESSAGE("Gold pieces clatter on the floor.");
      Midas->SetMoney(Midas->GetMoney() + Price);
    }

    RemoveFromSlot();
    SendToHell();
    return true;
  }
}

truth item::SoftenMaterial()
{
  if(!IsMaterialChangeable() || !CanBeSoftened())
  {
    return false;
  }

  int Config = GetMainMaterial()->GetSoftenedMaterial(this);

  if(!Config)
  {
    /* Should not be possible. */
    return false;
  }

  msgsystem::EnterBigMessageMode();

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("Suddenly %s starts glowing dull yellow.", CHAR_NAME(INDEFINITE));

  material* TempMaterial = MAKE_MATERIAL(Config);
  material* MainMaterial = GetMainMaterial();
  material* SecondaryMaterial = GetSecondaryMaterial();

  if(SecondaryMaterial && SecondaryMaterial->IsSameAs(MainMaterial))
    delete SetSecondaryMaterial(TempMaterial->SpawnMore());

  delete SetMainMaterial(TempMaterial);

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("It softens into %s!", GetMainMaterial()->GetName(false, false).CStr());

  msgsystem::LeaveBigMessageMode();
  return true;
}

/* Returns whether the Eater must stop eating the item */

truth item::Consume(character* Eater, long Amount)
{
  material* ConsumeMaterial = GetConsumeMaterial(Eater);

  if(!ConsumeMaterial)
    return true;

  if(Eater->IsPlayer() && !(Flags & CANNIBALIZED) && Eater->CheckCannibalism(ConsumeMaterial))
  {
    game::DoEvilDeed(25);
    ADD_MESSAGE("You feel that this was an evil deed.");
    Cannibalize();
  }

  ulong ID = GetID();
  material* Garbage = ConsumeMaterial->EatEffect(Eater, Amount);
  item* NewConsuming = GetID() ? this : game::SearchItem(ID);
  material* NewConsumeMaterial = NewConsuming->GetConsumeMaterial(Eater);

  if(!NewConsuming->Exists()
     || !NewConsumeMaterial
     || !NewConsumeMaterial->IsSameAs(ConsumeMaterial))
    ConsumeMaterial->FinishConsuming(Eater);

  delete Garbage;
  return !NewConsuming->Exists() || !NewConsumeMaterial;
}

truth item::CanBeEatenByAI(ccharacter* Eater) const
{
  material* ConsumeMaterial = GetConsumeMaterial(Eater);

  return (!Eater->IsPet()
          || !(Eater->GetCommandFlags() & DONT_CONSUME_ANYTHING_VALUABLE)
          || !IsValuable())
    && IsConsumable()
    && ConsumeMaterial && ConsumeMaterial->CanBeEatenByAI(Eater);
}

bool item::HasTag(char tag)
{
  static char Tag[3]={'#',0,0};
  Tag[1]=tag;
  return label.Find(Tag,0) != festring::NPos;
}

/**
 * look for all usages to avoid tag clashes
 */
void item::SetTag(char tag)
{
  if(!HasTag(tag))
    label<<"#"<<tag;
}

void item::ClearTag(char tag)
{
  static char Tag[3]={'#',0,0};
  Tag[1]=tag;
  int pos = label.Find(Tag,0);
  if(pos != festring::NPos)
    label.Erase(pos,2);
}

void item::SetLabel(cfestring& What)
{
  label.Empty();
  if(!What.IsEmpty())
    label << What;
}

void item::AddName(festring& Name, int Case) const
{
  object::AddName(Name,Case);

  if(label.GetSize())
    Name << " inscribed " << label;
}

void item::Save(outputfile& SaveFile) const
{
  SaveFile << static_cast<ushort>(GetType());
  object::Save(SaveFile);
  SaveFile << static_cast<ushort>(GetConfig());
  SaveFile << static_cast<ushort>(Flags);
  SaveFile << Size << ID << LifeExpectancy << ItemFlags;
  SaveFile << label;
  SaveLinkedList(SaveFile, CloneMotherID);

  if(Fluid)
  {
    SaveFile.Put(true);

    for(int c = 0; c < SquaresUnder; ++c)
      SaveLinkedList(SaveFile, Fluid[c]);
  }
  else
    SaveFile.Put(false);
}

void item::Load(inputfile& SaveFile)
{
  object::Load(SaveFile);
  databasecreator<item>::InstallDataBase(this, ReadType<ushort>(SaveFile));
  Flags |= ReadType<ushort>(SaveFile) & ~ENTITY_FLAGS;
  SaveFile >> Size >> ID >> LifeExpectancy >> ItemFlags;
  if(game::GetCurrentSavefileVersion()>=132)
    SaveFile >> label;
  LoadLinkedList(SaveFile, CloneMotherID);

  if(LifeExpectancy)
    Enable();

  game::AddItemID(this, ID);

  if(SaveFile.Get())
  {
    Fluid = new fluid*[SquaresUnder];

    for(int c = 0; c < SquaresUnder; ++c)
    {
      LoadLinkedList(SaveFile, Fluid[c]);

      for(fluid* F = Fluid[c]; F; F = F->Next)
        F->SetMotherItem(this);
    }
  }
}

void item::TeleportRandomly()
{
  if(GetSquaresUnder() == 1) // gum solution
  {
    lsquare* Square = GetNearLSquare(GetLevel()->GetRandomSquare());
    MoveTo(Square->GetStack());

    if(Square->CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s appears!", CHAR_NAME(INDEFINITE));
  }
}

int item::GetStrengthValue() const
{
  return long(GetStrengthModifier()) * GetMainMaterial()->GetStrengthValue() / 2000;
}

void item::RemoveFromSlot()
{
  for(int c = 0; c < SquaresUnder; ++c)
    if(Slot[c])
    {
      try
      {
        Slot[c]->Empty();
      }
      catch(quitrequest)
      {
        SendToHell();
        throw;
      }

      Slot[c] = 0;
    }
}

void item::MoveTo(stack* Stack)
{
  RemoveFromSlot();
  Stack->AddItem(this);
}

cchar* item::GetItemCategoryName(long Category) // convert to array
{
  switch(Category)
  {
   case HELMET: return "Helmets";
   case AMULET: return "Amulets";
   case CLOAK: return "Cloaks";
   case BODY_ARMOR: return "Body armors";
   case WEAPON: return "Weapons";
   case SHIELD: return "Shields";
   case RING: return "Rings";
   case GAUNTLET: return "Gauntlets";
   case BELT: return "Belts";
   case BOOT: return "Boots";
   case FOOD: return "Food";
   case POTION: return "Potions";
   case SCROLL: return "Scrolls";
   case BOOK: return "Books";
   case WAND: return "Wands";
   case TOOL: return "Tools";
   case VALUABLE: return "Valuables";
   case MISC: return "Miscellaneous items";
  }

  return "Warezzz";
}

int item::GetResistance(int Type) const
{
  switch(Type&0xFFF)
  {
   case PHYSICAL_DAMAGE: return GetStrengthValue();
   case ENERGY: return GetEnergyResistance();
   case DRAIN:
   case PSI:
   case MUSTARD_GAS_DAMAGE:
    return 0;
   case FIRE: return GetFireResistance();
   case POISON: return GetPoisonResistance();
   case ELECTRICITY: return GetElectricityResistance();
   case ACID: return GetAcidResistance();
   case SOUND: return GetSoundResistance();
  }

  ABORT("Resistance lack detected!");
  return 0;
}

truth item::Open(character* Char)
{
  if(Char->IsPlayer())
    ADD_MESSAGE("You can't open %s.", CHAR_NAME(DEFINITE));

  return false;
}

item* itemprototype::SpawnAndLoad(inputfile& SaveFile) const
{
  item* Item = Spawner(0, LOAD);
  Item->Load(SaveFile);
  Item->CalculateAll();
  return Item;
}

void item::LoadDataBaseStats()
{
  SetSize(GetDefaultSize());
}

void item::Initialize(int NewConfig, int SpecialFlags)
{
  CalculateSquaresUnder();
  Slot = new slot*[SquaresUnder];

  for(int c = 0; c < SquaresUnder; ++c)
    Slot[c] = 0;

  if(!(SpecialFlags & LOAD))
  {
    ID = game::CreateNewItemID(this);
    databasecreator<item>::InstallDataBase(this, NewConfig);
    LoadDataBaseStats();
    RandomizeVisualEffects();
    Flags |= CENTER << SQUARE_POSITION_SHIFT;

    if(!(SpecialFlags & NO_MATERIALS))
      GenerateMaterials();
  }

  if(!(SpecialFlags & LOAD))
    PostConstruct();

  if(!(SpecialFlags & (LOAD|NO_MATERIALS)))
  {
    CalculateAll();

    if(!(SpecialFlags & NO_PIC_UPDATE))
      UpdatePictures();
  }
}

truth item::ShowMaterial() const
{
  if(GetMainMaterialConfig().Size == 1)
    return GetMainMaterial()->GetConfig() != GetMainMaterialConfig()[0];
  else
    return true;
}

long item::GetBlockModifier() const
{
  if(!IsShield(0))
    return GetSize() * GetRoundness() << 1;
  else
    return GetSize() * GetRoundness() << 2;
}

truth item::CanBeSeenByPlayer() const
{
  return CanBeSeenBy(PLAYER);
}

truth item::CanBeSeenBy(ccharacter* Who) const
{
  for(int c = 0; c < SquaresUnder; ++c)
    if(Slot[c] && Slot[c]->CanBeSeenBy(Who))
      return true;

  return Who->IsPlayer() && game::GetSeeWholeMapCheatMode();
}

festring item::GetDescription(int Case) const
{
  if(CanBeSeenByPlayer())
    return GetName(Case);
  else
    return CONST_S("something");
}

void item::SignalVolumeAndWeightChange()
{
  CalculateVolumeAndWeight();

  for(int c = 0; c < SquaresUnder; ++c)
    if(Slot[c])
      Slot[c]->SignalVolumeAndWeightChange();
}

void item::CalculateVolumeAndWeight()
{
  Volume = Weight = 0;

  for(int c = 0; c < GetMaterials(); ++c)
  {
    cmaterial* Material = GetMaterial(c);

    if(Material)
    {
      Volume += Material->GetVolume();
      Weight += Material->GetWeight();
    }
  }
}

void item::SignalEmitationIncrease(col24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
  {
    game::CombineLights(Emitation, EmitationUpdate);

    for(int c = 0; c < SquaresUnder; ++c)
      if(Slot[c])
        Slot[c]->SignalEmitationIncrease(EmitationUpdate);
  }
}

void item::SignalEmitationDecrease(col24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
  {
    col24 Backup = Emitation;
    CalculateEmitation();

    if(Backup != Emitation)
      for(int c = 0; c < SquaresUnder; ++c)
        if(Slot[c])
          Slot[c]->SignalEmitationDecrease(EmitationUpdate);
  }
}

void item::CalculateAll()
{
  CalculateVolumeAndWeight();
  CalculateEmitation();
}

/* Temporary and buggy. */

void item::WeaponSkillHit(int Hits)
{
  if(Slot[0] && Slot[0]->IsGearSlot())
    static_cast<arm*>(static_cast<gearslot*>(*Slot)->GetBodyPart())->WieldedSkillHit(Hits);
}

/* Returns 0 if item cannot be cloned */

item* item::Duplicate(ulong Flags)
{
  if(!(Flags & IGNORE_PROHIBITIONS)
     && ((!(Flags & MIRROR_IMAGE) && !CanBeCloned())
         || (Flags & MIRROR_IMAGE && (!CanBeMirrored()
                                      || (MainMaterial
                                          && !(MainMaterial->GetCommonFlags() & CAN_BE_MIRRORED))
                                      || (GetSecondaryMaterial()
                                          && !(GetSecondaryMaterial()->GetCommonFlags() & CAN_BE_MIRRORED))))))
    return 0;

  item* Clone = GetProtoType()->Clone(this);

  if(Flags & MIRROR_IMAGE)
    Clone->SetLifeExpectancy(Flags >> LE_BASE_SHIFT & LE_BASE_RANGE,
                             Flags >> LE_RAND_SHIFT & LE_RAND_RANGE);

  Clone->UpdatePictures();
  return Clone;
}

void item::AddInventoryEntry(ccharacter*, festring& Entry, int Amount, truth ShowSpecialInfo) const
{
  if(Amount == 1)
    AddName(Entry, INDEFINITE);
  else
  {
    Entry << Amount << ' ';
    AddName(Entry, PLURAL);
  }

  if(ShowSpecialInfo){
    //TODO if the 1st and 2nd of 3 items have 100g and the last has 2000g, the weight shown would be 300g ... now that lumps, stones and sticks are useful, this may not be that good...
    Entry << " [";
    Entry.PutWeight(GetWeight() * Amount);

    if(ivanconfig::IsShowVolume()) {
      Entry << " " << GetVolume() * Amount << "cm3"; //the item can be seen therefore it's volume guessed already
      if(GetSecondaryMaterial()==NULL){ //simple items like ingots sticks etc
        static char density[20];
        snprintf(density, sizeof(density), "%.1f", GetWeight()/(float)GetVolume());
        Entry << " " << density << "g/cm3"; //the item can be seen and weighted already so this just helps avoiding having to mentally calc density for every item
        if(game::WizardModeIsActive()) //TODO || Char-> possess item <materialmanual*>
          Entry << " " << GetStrengthValue() << "str"; //this is special info tho.
      }
    }
    Entry << "]";
  }
}

const itemdatabase* itemprototype::ChooseBaseForConfig(itemdatabase** TempConfig, int Configs, int ConfigNumber)
{
  if(!(ConfigNumber & BROKEN))
    return *TempConfig;
  else
  {
    ConfigNumber ^= BROKEN;

    for(int c = 0; c < Configs; ++c)
      if(TempConfig[c]->Config == ConfigNumber)
        return TempConfig[c];

    return *TempConfig;
  }
}

truth item::ReceiveDamage(character* Damager, int Damage, int Type, int Dir)
{
  if(MainMaterial && IsDestroyable(Damager))
  {
    if(CanBeBurned() && (MainMaterial->GetInteractionFlags() & CAN_BURN) && !IsBurning() && Type & FIRE)
    {
      TestActivationEnergy(Damage);
    }
    else if(IsBurning() && Type & FIRE)
      GetMainMaterial()->AddToThermalEnergy(Damage);
  }

  if(CanBeBroken() && !IsBroken() && Type & (PHYSICAL_DAMAGE|SOUND|ENERGY|ACID))
  {
    int StrengthValue = GetStrengthValue();

    if(!StrengthValue)
      StrengthValue = 1;

    if(Damage > StrengthValue << 2 && RAND() & 3 && RAND() % (25 * Damage / StrengthValue) >= 100)
    {
      Break(Damager, Dir);
      return true;
    }
  }

  if(Type & ACID && IsBroken() && IsDestroyable(Damager))
  {
    int StrengthValue = GetStrengthValue();

    if(!StrengthValue)
      StrengthValue = 1;

    if(Damage > StrengthValue << 4 && !(RAND() & 3) && RAND() % (100 * Damage / StrengthValue) >= 100)
    {
      Destroy(Damager, Dir);
      return true;
    }
  }

  return false;
}

void itemdatabase::InitDefaults(const itemprototype* NewProtoType, int NewConfig)
{
  IsAbstract = false;
  ProtoType = NewProtoType;
  Config = NewConfig;

  if(NewConfig & BROKEN)
  {
    if(Adjective.GetSize())
      Adjective.Insert(0, "broken ");
    else
      Adjective = CONST_S("broken");

    DefaultSize >>= 1;
    FormModifier >>= 2;
    StrengthModifier >>= 1;
  }
}

long item::GetNutritionValue() const
{
  long NV = 0;

  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      NV += GetMaterial(c)->GetTotalNutritionValue();

  return NV;
}

void item::SignalSpoil(material*)
{
  if(!Exists())
    return;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s spoils completely.", GetExtendedDescription().CStr());

  truth Equipped = PLAYER->Equips(this);
  Disappear();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));
}

void item::SignalBurn(material* Material)
{
  if(!Exists())
    return;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s burns away completely.", GetExtendedDescription().CStr());

  truth Equipped = PLAYER->Equips(this);
  Disappear();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));
}

item* item::DuplicateToStack(stack* CurrentStack, ulong Flags)
{
  item* Duplicated = Duplicate(Flags);

  if(!Duplicated)
    return 0;

  CurrentStack->AddItem(Duplicated);
  return Duplicated;
}

truth item::CanBePiledWith(citem* Item, ccharacter* Viewer) const
{
  return (GetType() == Item->GetType()
          && GetConfig() == Item->GetConfig()
          && ItemFlags == Item->ItemFlags
          && ((!ivanconfig::IsAllWeightIsRelevant() && WeightIsIrrelevant()) || Weight == Item->Weight)
          && MainMaterial->IsSameAs(Item->MainMaterial)
          && MainMaterial->GetSpoilLevel() == Item->MainMaterial->GetSpoilLevel()
          && MainMaterial->GetRustLevel() == Item->MainMaterial->GetRustLevel()
          && MainMaterial->GetBurnLevel() == Item->MainMaterial->GetBurnLevel()
          && Viewer->GetCWeaponSkillLevel(this) == Viewer->GetCWeaponSkillLevel(Item)
          && Viewer->GetSWeaponSkillLevel(this) == Viewer->GetSWeaponSkillLevel(Item)
          && !Fluid && !Item->Fluid
          && !LifeExpectancy == !Item->LifeExpectancy
          && !IsBurning() == !Item->IsBurning()
          && label==Item->label );
}

void item::Break(character* Breaker, int)
{
  if(!CanBeBroken() || IsBroken())
    return;

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s %s.", GetExtendedDescription().CStr(), GetBreakVerb());

  if(Breaker && IsOnGround())
  {
    room* Room = GetRoom();

    if(Room)
      Room->HostileAction(Breaker);
  }

  item* Broken = GetProtoType()->Clone(this);
  Broken->SetConfig(GetConfig() | BROKEN);
  Broken->SetSize(Broken->GetSize() >> 1);
  DonateFluidsTo(Broken);
  DonateIDTo(Broken);
  DonateSlotTo(Broken);
  SendToHell();

  if(PLAYER->Equips(Broken))
    game::AskForKeyPress(CONST_S("Equipment broken! [press any key to continue]"));
}

void item::Be()
{
  MainMaterial->Be(ItemFlags);

  if(Exists() && LifeExpectancy)
  {
    if(LifeExpectancy == 1)
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s disappears.", GetExtendedDescription().CStr());

      truth Equipped = PLAYER->Equips(this);
      Disappear();

      if(Equipped)
        game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));
    }
    else
    {
      --LifeExpectancy;
      //TODO fluids emitation on weapons require updating in case it lowers with time, this didnt work: if(Fluid)CalculateEmitation();
    }
  }
}

int item::GetOfferValue(int Receiver) const
{
  /* Temporary */

  int OfferValue = int(sqrt(GetTruePrice()));

  if(Receiver == GetAttachedGod())
    OfferValue <<= 1;
  else
    OfferValue >>= 1;

  return OfferValue;
}

void item::SignalEnchantmentChange()
{
  for(int c = 0; c < SquaresUnder; ++c)
    if(Slot[c])
      Slot[c]->SignalEnchantmentChange();
}

long item::GetEnchantedPrice(int Enchantment) const
{
  if(!PriceIsProportionalToEnchantment())
    return item::GetPrice();
  else
    return Max<int>(item::GetPrice() * Enchantment * Enchantment, 0);
}

item* item::Fix()
{
  item* Fixed = this;

  if(IsBroken())
  {
    Fixed = GetProtoType()->Clone(this);
    Fixed->SetConfig(GetConfig() ^ BROKEN);
    Fixed->SetSize(Fixed->GetSize() << 1);
    DonateFluidsTo(Fixed);
    DonateIDTo(Fixed);
    DonateSlotTo(Fixed);
    SendToHell();
  }

  return Fixed;
}

void item::DonateSlotTo(item* Item)
{
  if(Slot[0])
  {
    Slot[0]->DonateTo(Item);
    Slot[0] = 0;

    for(int c = 1; c < SquaresUnder; ++c)
      if(Slot[c])
      {
        Slot[c]->Empty();
        Slot[c] = 0;
      }
  }
}

int item::GetSpoilLevel() const
{
  return MainMaterial->GetSpoilLevel();
}

int item::GetBurnLevel() const
{
  return MainMaterial->GetBurnLevel();
}

void item::SignalSpoilLevelChange(material*)
{
  if(!IsAnimated() && GetSpoilLevel() && Slot[0] && Slot[0]->IsVisible())
    for(int c = 0; c < SquaresUnder; ++c)
      GetSquareUnder(c)->IncStaticAnimatedEntities();

  SignalVolumeAndWeightChange(); // gum
  UpdatePictures();
}

truth item::AllowSpoil() const
{
  DBG5(GetName(DEFINITE).CStr(),GetID(),GetSquareUnder(),GetWearer(),GetSlot());
  DBGEXEC( //this wont even compile if DBGMSG is not enabled
    /** crash helper
     * THE CAUSE SEEMS TO BE from crafting a new item, it being a chest, and cancelling. The code was not sending the canceled spawned chest to hell and it was not placed anywhere. Fixed that, this may not happen again.
     * TODO remove this debug code and the workaround below after sure wont need anymore. despite the debug code will only compile if using DBGMSG
     * seems to be about a buggy organic spawned "on floor" that has no square under...
    *  happens below at: if(IsOnGround())
   *   item.cpp:1048:AllowSpoil:{GetName(2).CStr()}="the loaf of dark bread";{GetID()}="92980";{GetSquareUnder()}="0";{GetWearer()}="0";{GetSlot()}="0x8272630";
 2018/07/17-15:36:08(1122986) item.cpp:1049:AllowSpoil:{GetName(2).CStr()}="the loaf of dark bread";{GetID()}="780622";{GetSquareUnder()}="0";{GetWearer()}="0";{GetSlot()}="0x38be0e0";
 2018/07/17-15:36:08(1122987) item.cpp:1091:AllowSpoil:{itSS}="0x3293700";{ss->GetSquareUnder()}="0";
 2018/07/17-15:36:08(1122988) item.cpp:1091:AllowSpoil:{itSS->GetID()}="780622";{itSS->GetNameSingular().CStr()}="loaf";
 2018/07/17-15:36:08(1122989) item.cpp:1091:AllowSpoil:{ss->GetMotherStack()->GetItems()}="3";{ent}="0x62a17d0";{ss->GetMotherStack()->GetSquareUnder()}="0";
 2018/07/17-15:36:08(1122990) item.cpp:1091:AllowSpoil:{ent->GetSquareUnderEntity()}="0";
 2018/07/17-15:36:08(1122991) item.cpp:1091:AllowSpoil:{itM->GetID()}="780621";{itM->GetNameSingular().CStr()}="chest";
   *     ./bin//ivan(_ZN6dbgmsg20getCurrentStackTraceEbRi+0xaa) [0x92f43e]
   *     ./bin//ivan(_ZN6dbgmsg22getCurrentStackTraceSSB5cxx11Ebb+0x54) [0x92f53c]
    *    ./bin//ivan(_ZN6dbgmsg8SigHndlrEi+0x1ae) [0x92fcdc]
     *   /lib/x86_64-linux-gnu/libc.so.6(+0x354b0) [0x7f6b145e14b0]
      *  ./bin//ivan(_ZNK4item10IsOnGroundEv+0x17) [0x8963c7]
      *  ./bin//ivan(_ZNK5stack10IsOnGroundEv+0x38) [0x8f12a8]
      *  ./bin//ivan(_ZNK9stackslot10IsOnGroundEv+0x20) [0x8ebb96]
     *   ./bin//ivan(_ZNK4item10IsOnGroundEv+0x31) [0x8963e1]
    *    ./bin//ivan(_ZNK4item10AllowSpoilEv+0x27f) [0x89b0ab]
   *     ./bin//ivan(_ZN7organic2BeEm+0x4f) [0x839d0d]
   *     ./bin//ivan(_ZN4item2BeEv+0x49) [0x89a819]
   *     ./bin//ivan(_ZN4pool2BeEv+0x3b) [0x69593b]
    *    ./bin//ivan(_ZN4game3RunEv+0x3da) [0x77ad5a]
     *   ./bin//ivan(main+0x4d7) [0x70d462]
      *  /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0) [0x7f6b145cc830]
      *  ./bin//ivan(_start+0x29) [0x68f999]
     */
    if(dynamic_cast<stackslot*>(GetSlot())!=NULL){
      stackslot* ss = (stackslot*)GetSlot();
      item* itSS = ss->GetItem();
      DBG2(itSS,ss->GetSquareUnder());
      if(itSS!=NULL)DBG2(itSS->GetID(),itSS->GetNameSingular().CStr());
      stack* stkM=ss->GetMotherStack();
      if(stkM!=NULL){
        entity* ent = stkM->GetMotherEntity();
        DBG3(ss->GetMotherStack()->GetItems(), ent, ss->GetMotherStack()->GetSquareUnder());
        if(ent!=NULL){
          DBG1(ent->GetSquareUnderEntity());
//          if(dynamic_cast<id*>(ent))DBG1(((id*)ent)->GetID());
          if(dynamic_cast<item*>(ent)){
            item* itM = (item*)ent;
            DBG2(itM->GetID(),itM->GetNameSingular().CStr());
          }
        }
      }
    }
  );
//TODO remove this. The workaround is NOT good as SquareUnder is essential everywhere!!!!
//  bool bIsOnGround=false;
//  lsquare* Square = GetLSquareUnder(); //TODO what could cause Square to be NULL ?????
//  static bool bAllowSpoilBugWorkaround=true;
//  if(bAllowSpoilBugWorkaround){ //See the above bug track code, the origin of the problem is still NOT solved/understood/discovered!!!!!!
//    if(Square!=NULL){
//      bIsOnGround=IsOnGround();
//      DBG1("WorkaroundForItemPlacedNoWhere");
//    }
//    /**
//     * what this means?
//     *
//     * not knowing where the item is, if on a room, the item will ALWAYS spoil,
//     * so if the buggy item "is" on a shop, it will spoil.
//     *
//     * and, on this stack/flow that game wont crash, but still may on other code flows!!!
//     */
//  }else{
//    bIsOnGround=IsOnGround();
//  }

  if(IsOnGround())
  {
    lsquare* Square = GetLSquareUnder();
    int RoomNumber = Square->GetRoomIndex();
    return !RoomNumber || Square->GetLevel()->GetRoom(RoomNumber)->AllowSpoil(this);
  }
  else
    return true;
}

void item::ResetSpoiling()
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->ResetSpoiling();
}

void item::ResetBurning()
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->ResetBurning();
  SignalEmitationDecrease(MakeRGB24(150, 120, 90));
}

/* be careful calling this function, it will also cause flames to extinguish */
void item::ResetThermalEnergies()
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->ResetThermalEnergies();
}

cchar* item::GetBaseToHitValueDescription() const
{
  if(GetBaseToHitValue() < 10)
    return ToHitValueDescription[Min(GetBaseToHitValue(), 6)];
  else
    return ToHitValueDescription[7];
}

cchar* item::GetBaseBlockValueDescription() const
{
  if(GetBaseBlockValue() < 20)
    return ToHitValueDescription[Min(GetBaseBlockValue() >> 1, 6)];
  else
    return ToHitValueDescription[7];
}

cchar* item::GetStrengthValueDescription() const
{
  int SV = GetStrengthValue();

  if(SV < 3)
    return StrengthValueDescription[0];
  else if(SV < 5)
    return StrengthValueDescription[1];
  else if(SV < 8)
    return StrengthValueDescription[2];
  else if(SV < 11)
    return StrengthValueDescription[3];
  else if(SV < 16)
    return StrengthValueDescription[4];
  else if(SV < 20)
    return StrengthValueDescription[5];
  else
    return StrengthValueDescription[6];
}

void item::SpecialGenerationHandler()
{
  if(HandleInPairs())
    Slot[0]->AddFriendItem(Duplicate());
}

void item::SortAllItems(const sortdata& SortData) const
{
  if(SortData.Sorter == 0 || (this->*SortData.Sorter)(SortData.Character))
    SortData.AllItems.push_back(const_cast<item*>(this));
}

int item::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

long item::GetMaterialPrice() const
{
  return MainMaterial->GetRawPrice();
}

long item::GetTruePrice() const
{
  if(LifeExpectancy)
    return 0;

  long Price = Max(GetPrice(), GetMaterialPrice());

  if(Spoils())
    Price = Price * (100 - GetMaxSpoilPercentage()) / 500;

  return Price;
}

#ifdef WIZARD

void item::AddAttackInfo(felist& List) const
{
  festring Entry(40, ' ');
  Entry << int(GetWeight());
  Entry.Resize(50);
  Entry << GetSize();
  Entry.Resize(60);
  Entry << GetStrengthRequirement();
  Entry.Resize(70);
  Entry << GetBaseMinDamage() << '-' << GetBaseMaxDamage();
  List.AddEntry(Entry, LIGHT_GRAY);
}

void item::AddMiscellaneousInfo(felist& List) const
{
  festring Entry(40, ' ');
  Entry << int(GetTruePrice());
  Entry.Resize(55);
  Entry << GetOfferValue(0);
  Entry.Resize(70);
  Entry << int(GetNutritionValue());
  List.AddEntry(Entry, LIGHT_GRAY);
}

#endif

void item::PreProcessForBone()
{
  if(IsQuestItem())
  {
    RemoveFromSlot();
    SendToHell();
  }
  else
  {
    game::RemoveItemID(ID);
    ID = -ID;
    game::AddItemID(this, ID);
  }
}

void item::_BugWorkaround_ItemDup(ulong key){ID=key;} //keep it simple!

void item::PostProcessForBone()
{
  boneidmap::iterator BI = game::GetBoneItemIDMap().find(-ID);
  game::RemoveItemID(ID);

  if(BI == game::GetBoneItemIDMap().end())
  {
    ulong NewID = game::CreateNewItemID(this);
    game::GetBoneItemIDMap().insert(std::make_pair(-ID, NewID));
    ID = NewID;
  }
  else
  {
    ID = BI->second;
    game::AddItemID(this, ID);
  }

  for(idholder* I = CloneMotherID; I; I = I->Next)
  {
    BI = game::GetBoneItemIDMap().find(I->ID);

    if(BI == game::GetBoneItemIDMap().end())
    {
      ulong NewCloneMotherID = game::CreateNewItemID(0);
      game::GetBoneItemIDMap().insert(std::make_pair(I->ID, NewCloneMotherID));
      I->ID = NewCloneMotherID;
    }
    else
      I->ID = BI->second;
  }
}

void item::SetConfig(int NewConfig, int SpecialFlags)
{
  databasecreator<item>::InstallDataBase(this, NewConfig);
  CalculateAll();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();
}

truth item::SetConfigIfPossible(int NewConfig, int SpecialFlags)
{
  if(databasecreator<item>::InstallDataBaseIfPossible(this, NewConfig, GetConfig())){
    CalculateAll();

    if(!(SpecialFlags & NO_PIC_UPDATE))
      UpdatePictures();
    
    return true;
  }
  
  return false;
}

god* item::GetMasterGod() const
{
  return game::GetGod(GetConfig());
}

int itemprototype::CreateSpecialConfigurations(itemdatabase** TempConfig, int Configs, int Level)
{
  if(Level)
    return Configs;

  if(TempConfig[0]->CreateDivineConfigurations)
    Configs = databasecreator<item>::CreateDivineConfigurations(this, TempConfig, Configs);

  /* Gum solution */

  if(TempConfig[0]->CreateLockConfigurations)
  {
    const item::database*const* KeyConfigData = key::ProtoType.GetConfigData();
    int KeyConfigSize = key::ProtoType.GetConfigSize();
    int OldConfigs = Configs;

    for(int c1 = 0; c1 < OldConfigs; ++c1)
      if(!TempConfig[c1]->IsAbstract)
      {
        int BaseConfig = TempConfig[c1]->Config;
        int NewConfig = BaseConfig | BROKEN_LOCK;
        itemdatabase* ConfigDataBase = new itemdatabase(*TempConfig[c1]);
        ConfigDataBase->InitDefaults(this, NewConfig);
        ConfigDataBase->PostFix << "with a broken lock";
        ConfigDataBase->Possibility = 0;
        TempConfig[Configs++] = ConfigDataBase;

        for(int c2 = 0; c2 < KeyConfigSize; ++c2)
        {
          NewConfig = BaseConfig | KeyConfigData[c2]->Config;
          ConfigDataBase = new itemdatabase(*TempConfig[c1]);
          ConfigDataBase->InitDefaults(this, NewConfig);
          ConfigDataBase->PostFix << "with ";

          if(KeyConfigData[c2]->UsesLongAdjectiveArticle)
            ConfigDataBase->PostFix << "an ";
          else
            ConfigDataBase->PostFix << "a ";

          ConfigDataBase->PostFix << KeyConfigData[c2]->Adjective << " lock";
          ConfigDataBase->Possibility = 0;
          TempConfig[Configs++] = ConfigDataBase;
        }
      }
  }

  return Configs;
}

void item::Draw(blitdata& BlitData) const
{
  cint AF = GraphicData.AnimationFrames;
  cint F = !(BlitData.CustomData & ALLOW_ANIMATE) || AF == 1 ? 0 : GET_TICK() & (AF - 1);

  bitmap* bmp = GraphicData.Picture[F];
  if(ivanconfig::GetRotateTimesPerSquare()>0 && iRotateFlyingThrownStep!=0){ // tests made using a single bladed (unbalanced) thrown axe where 0 degrees was: blade at topRight poiting downwards
    static blitdata B = [](){B=DEFAULT_BLITDATA; //to reuse tmp bitmap memory
      B.Bitmap = new bitmap(TILE_V2); //bmp->GetSize());
      B.Border = TILE_V2; return B; }();

    bitmap::ConfigureBlitdataRotation(B,iRotateFlyingThrownStep);

    bmp->NormalBlit(B); //or NormalMaskedBlit() only way to rotate...
    bmp = B.Bitmap;
  }
  cbitmap* P = bmp;

  if(BlitData.CustomData & ALLOW_ALPHA)
    P->AlphaLuminanceBlit(BlitData);
  else
    P->LuminanceMaskedBlit(BlitData);

  if(Fluid && ShowFluids())
    DrawFluids(BlitData);
}

v2 item::GetLargeBitmapPos(v2 BasePos, int I) const
{
  cint SquareIndex = I ? I / (GraphicData.AnimationFrames >> 2) : 0;
  return v2(SquareIndex & 1 ? BasePos.X + 16 : BasePos.X, SquareIndex & 2 ? BasePos.Y + 16 : BasePos.Y);
}

void item::LargeDraw(blitdata& BlitData) const
{
  cint TrueAF = GraphicData.AnimationFrames >> 2;
  cint SquareIndex = BlitData.CustomData & SQUARE_INDEX_MASK;
  cint F = !(BlitData.CustomData & ALLOW_ANIMATE) ? SquareIndex * TrueAF : SquareIndex * TrueAF + (GET_TICK() & (TrueAF - 1));
  cbitmap* P = GraphicData.Picture[F];

  if(BlitData.CustomData & ALLOW_ALPHA)
    P->AlphaLuminanceBlit(BlitData);
  else
    P->LuminanceMaskedBlit(BlitData);
}

void item::DonateIDTo(item* Item)
{
  game::RemoveItemID(Item->ID);
  game::UpdateItemID(Item, ID);
  Item->ID = ID;
  ID = 0;
}

void item::SignalRustLevelChange()
{
  SignalVolumeAndWeightChange();
  UpdatePictures();
  SendNewDrawAndMemorizedUpdateRequest();
}

void item::SignalBurnLevelTransitionMessage()
{
  if(CanBeSeenByPlayer())
  {
    if(MainMaterial->GetBurnLevel() == NOT_BURNT)
      ADD_MESSAGE("%s burns.", GetExtendedDescription().CStr());
    else
      ADD_MESSAGE("%s burns more.", GetExtendedDescription().CStr());
  }
}

void item::SignalBurnLevelChange()
{
  if(!IsAnimated() && GetBurnLevel() && Slot[0] && Slot[0]->IsVisible())
    for(int c = 0; c < SquaresUnder; ++c)
      GetSquareUnder(c)->IncStaticAnimatedEntities();

  SignalEmitationDecrease(MakeRGB24(150, 120, 90)); // completely remove previously applied emitation increases
  SignalEmitationIncrease(GetEmitationDueToBurnLevel()); // apply an emitation increase according to the current burn level

  SignalVolumeAndWeightChange();
  UpdatePictures();
  SendNewDrawAndMemorizedUpdateRequest();
}

/* emitation slowly ramps down with increasing item BurnLevel, in the beginning the
   light is bright, with light decreasing in intensity as the item gets more burnt */
col24 item::GetEmitationDueToBurnLevel()
{
  if(MainMaterial)
  {
    int CurrentBurnLevel = GetBurnLevel();

    int Red = 150 - 10 * CurrentBurnLevel;
    int Green = 120 - 8 * CurrentBurnLevel;
    int Blue = 90 - 6 * CurrentBurnLevel;
    //ADD_MESSAGE("Emitation due to BurnLevel: R%d G%d B%d", Red, Green, Blue); // por debug
    return MakeRGB24(Red, Green, Blue);
  }
  else
    return MakeRGB24(0, 0, 0);
}

const rawbitmap* item::GetRawPicture() const
{
  return igraph::GetRawGraphic(GetGraphicsContainerIndex());
}

void item::RemoveFluid(fluid* ToBeRemoved)
{
  truth WasAnimated = IsAnimated();
  truth HasFluids = false;

  for(int c = 0; c < SquaresUnder; ++c)
  {
    fluid* F = Fluid[c];

    if(F == ToBeRemoved)
      Fluid[c] = F->Next;
    else if(F)
    {
      fluid* LF = F;

      for(F = F->Next; F; LF = F, F = F->Next)
        if(F == ToBeRemoved)
        {
          LF->Next = F->Next;
          break;
        }
    }

    if(Fluid[c])
      HasFluids = true;
  }

  UpdatePictures();

  if(!HasFluids)
  {
    delete [] Fluid;
    Fluid = 0;

    if(!IsAnimated() != !WasAnimated && Slot[0]->IsVisible())
      GetSquareUnder()->DecStaticAnimatedEntities();
  }

  SignalEmitationDecrease(ToBeRemoved->GetEmitation());
  SignalVolumeAndWeightChange();
}

void item::AddFluid(liquid* ToBeAdded, festring LocationName, int SquareIndex, truth IsInside)
{
  truth WasAnimated = IsAnimated();

  if(Fluid)
  {
    fluid* F = Fluid[SquareIndex];

    if(!F)
      Fluid[SquareIndex] = new fluid(ToBeAdded, this, LocationName, IsInside);
    else
    {
      fluid* LF;

      do
      {
        if(ToBeAdded->IsSameAs(F->GetLiquid()))
        {
          F->AddLiquidAndVolume(ToBeAdded->GetVolume());
          delete ToBeAdded;
          return;
        }

        LF = F;
        F = F->Next;
      }
      while(F);

      LF->Next = new fluid(ToBeAdded, this, LocationName, IsInside);
    }
  }
  else
  {
    Fluid = new fluid*[SquaresUnder];
    memset(Fluid, 0, SquaresUnder * sizeof(fluid*));
    Fluid[SquareIndex] = new fluid(ToBeAdded, this, LocationName, IsInside);
  }

  UpdatePictures();
  SignalVolumeAndWeightChange();
  SignalEmitationIncrease(ToBeAdded->GetEmitation());

  if(Slot[0])
  {
    if(!IsAnimated() != !WasAnimated && Slot[0]->IsVisible())
      GetSquareUnder()->IncStaticAnimatedEntities();

    SendNewDrawAndMemorizedUpdateRequest();
  }
}

void item::SendNewDrawAndMemorizedUpdateRequest() const
{
  if(!game::IsInWilderness())
    for(int c = 0; c < SquaresUnder; ++c)
      if(Slot[c])
      {
        lsquare* Square = GetLSquareUnder(c);
        Square->SendNewDrawRequest();
        Square->SendMemorizedUpdateRequest();
      }
}

void item::CalculateEmitation()
{
  object::CalculateEmitation();

  if(Fluid){
    for(int c = 0; c < SquaresUnder; ++c){
      for(const fluid* F = Fluid[c]; F; F = F->Next){
        DBG2(F->GetEmitation(),F->GetLiquid()->GetVolume());
        game::CombineLights(Emitation,
          CalcEmitationBasedOnVolume( 0, F->GetEmitation(), F->GetLiquid()->GetVolume() ) );
      }
    }
  }
}

void item::FillFluidVector(fluidvector& Vector, int SquareIndex) const
{
  if(Fluid)
    for(fluid* F = Fluid[SquareIndex]; F; F = F->Next)
      Vector.push_back(F);
}

void item::SpillFluid(character*, liquid* Liquid, int SquareIndex)
{
  if(AllowFluids() && Liquid->GetVolume())
    AddFluid(Liquid, "", SquareIndex, false);
  else
    delete Liquid;
}

void item::TryToRust(long LiquidModifier)
{
  if(MainMaterial->TryToRust(LiquidModifier))
  {
    if(CanBeSeenByPlayer())
    {
      if(MainMaterial->GetRustLevel() == NOT_RUSTED)
        ADD_MESSAGE("%s rusts.", CHAR_NAME(DEFINITE));
      else
        ADD_MESSAGE("%s rusts more.", CHAR_NAME(DEFINITE));
    }

    MainMaterial->SetRustLevel(MainMaterial->GetRustLevel() + 1);
  }
}

truth item::TestActivationEnergy(int Damage)
{
  if(Damage <= 0)
      return false;

  truth Success = false;
//  if(MainMaterial)
//  {
//    int molamola = ((GetMainMaterial()->GetStrengthValue() >> 1) + 5 * MainMaterial->GetFireResistance() + GetResistance(FIRE));
//    ADD_MESSAGE("%s is being tested (Damage is %d, AE is %d)", CHAR_NAME(DEFINITE), Damage, molamola);
//  }

  if(MainMaterial)
  {
    int TestDamage = Damage + MainMaterial->GetTransientThermalEnergy();
    GetMainMaterial()->AddToTransientThermalEnergy(Damage);
    if(CanBeBurned() && GetMainMaterial()->GetInteractionFlags() & CAN_BURN
       && TestDamage >= ((GetMainMaterial()->GetStrengthValue() >> 1)
                         + 5 * MainMaterial->GetFireResistance() + GetResistance(FIRE)))
    {
      if(CanBeSeenByPlayer())
      {
        ADD_MESSAGE("%s catches fire!", CHAR_NAME(DEFINITE));
        //ADD_MESSAGE("%s catches fire! (TestDamage was %d)", CHAR_NAME(DEFINITE), TestDamage);
      }
      Ignite();
      GetMainMaterial()->AddToSteadyStateThermalEnergy(Damage);
      Success = true;
    }
  }
  return Success;
}

void item::Ignite(/*character* Arsonist*/)
{
  truth WasAnimated = IsAnimated();

  MainMaterial->SetIsBurning(true);
  SignalEmitationIncrease(GetEmitationDueToBurnLevel()); // kick this off by applying an emitation increase proportional to the burn level of the item
  UpdatePictures();
  //ADD_MESSAGE("The %s now burns brightly.", CHAR_NAME(DEFINITE));

  if(Slot[0])
  {
    if(!IsAnimated() != !WasAnimated && Slot[0]->IsVisible())
      GetSquareUnder()->IncStaticAnimatedEntities();

    SendNewDrawAndMemorizedUpdateRequest();
  }
}

/* This causes the main material to stop burning, resets the thermal energies
   and does a picture update on the level, as well as wielded pictures */
void item::Extinguish(/*character* FireFighter, */truth SendMessages)
{
  truth WasAnimated = IsAnimated();
  truth WasSeen = CanBeSeenByPlayer();
  truth WasBurning = IsBurning();

  MainMaterial->SetIsBurning(false);
  MainMaterial->ResetThermalEnergies();
  SignalEmitationDecrease(MakeRGB24(150, 120, 90));

  if(Slot[0])
  {
    if(!IsAnimated() != !WasAnimated && Slot[0]->IsVisible())
      GetSquareUnder()->DecStaticAnimatedEntities();
  }

  if(WasBurning && WasSeen && SendMessages) // by now it is dark...
    AddExtinguishMessage();

  SignalVolumeAndWeightChange();
  UpdatePictures();
  SendNewDrawAndMemorizedUpdateRequest();
}

void item::AddExtinguishMessage()
{
  ADD_MESSAGE("The flames on %s die away.", GetExtendedDescription().CStr());
}

// This is for anything made from phoenix feather
void item::AddSpecialExtinguishMessageForPF()
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s burns even more! But lo', even as it does so, the ashes "
                "peel away from it and it is made new by some innate virtue.", GetExtendedDescription().CStr());
}

void item::CheckFluidGearPictures(v2 ShadowPos, int SpecialFlags, truth BodyArmor)
{
  if(Fluid)
    for(fluid* F = Fluid[0]; F; F = F->Next)
      F->CheckGearPicture(ShadowPos, SpecialFlags, BodyArmor);
}

void item::DrawFluidGearPictures(blitdata& BlitData, int SpecialFlags) const
{
  if(Fluid)
    for(const fluid* F = Fluid[0]; F; F = F->Next)
      F->DrawGearPicture(BlitData, SpecialFlags);
}

void item::DrawFluidBodyArmorPictures(blitdata& BlitData, int SpecialFlags) const
{
  if(Fluid)
    for(const fluid* F = Fluid[0]; F; F = F->Next)
      F->DrawBodyArmorPicture(BlitData, SpecialFlags);
}

void item::DrawFluids(blitdata& BlitData) const
{
  cint SquareIndex = BlitData.CustomData & SQUARE_INDEX_MASK;

  for(const fluid* F = Fluid[SquareIndex]; F; F = F->Next)
    F->Draw(BlitData);
}

void item::ReceiveAcid(material*, cfestring&, long Modifier)
{
  if(GetMainMaterial()->GetInteractionFlags() & CAN_DISSOLVE)
  {
    int Damage = Modifier / 1000;

    if(Damage)
    {
      Damage += RAND() % Damage;
      ReceiveDamage(0, Damage, ACID);
    }
  }
}

void item::ReceiveHeat(material*, cfestring&, long Modifier)
{
  if(GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
  {
    int Damage = Modifier / 1000;

    if(Damage)
    {
      Damage += RAND() % Damage;
      ReceiveDamage(0, Damage, FIRE);
    }
  }
}

void item::FightFire(material*, cfestring&, long Volume)
{
  int Amount = sqrt(Volume);
  GetMainMaterial()->RemoveFromThermalEnergy(Amount);
}

void item::DonateFluidsTo(item* Item)
{
  if(Fluid)
    for(int c = 0; c < GetSquaresUnder(); ++c)
      for(fluid* F = Fluid[c]; F; F = F->Next)
      {
        liquid* Liquid = F->GetLiquid();
        Item->AddFluid(Liquid->SpawnMoreLiquid(Liquid->GetVolume()), F->GetLocationName(), c, F->IsInside());
      }
}

void item::Destroy(character* Destroyer, int)
{
  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s is destroyed.", GetExtendedDescription().CStr());

  if(Destroyer && IsOnGround())
  {
    room* Room = GetRoom();

    if(Room)
      Room->HostileAction(Destroyer);
  }

  truth Equipped = PLAYER->Equips(this);
  RemoveFromSlot();
  SendToHell();

  if(Equipped)
    game::AskForKeyPress(CONST_S("Equipment destroyed! [press any key to continue]"));
}

void item::RemoveRust()
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->SetRustLevel(NOT_RUSTED);
}

void item::RemoveBurns()
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      GetMaterial(c)->SetBurnLevel(NOT_BURNT, true);
}

void item::SetSpoilPercentage(int Value)
{
  for(int c = 0; c < GetMaterials(); ++c)
  {
    material* Material = GetMaterial(c);

    if(Material && Material->CanSpoil())
      Material->SetSpoilCounter(Material->GetSpoilModifier() * Value / 100);
  }
}

void item::RedistributeFluids()
{
  if(Fluid)
    for(int c = 0; c < GetSquaresUnder(); ++c)
      for(fluid* F = Fluid[c]; F; F = F->Next)
        F->Redistribute();
}

material* item::GetConsumeMaterial(ccharacter* Consumer, materialpredicate Predicate) const
{
  return (MainMaterial->*Predicate)() && Consumer->CanConsume(MainMaterial) ? MainMaterial : 0;
}

/* The parameter can only be MainMaterial */

material* item::RemoveMaterial(material*)
{
  RemoveFromSlot();
  SendToHell();
  return 0;
}

void item::InitMaterials(material* FirstMaterial, truth CallUpdatePictures)
{
  InitMaterial(MainMaterial, FirstMaterial, GetDefaultMainVolume());
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void item::GenerateMaterials()
{
  int Chosen = RandomizeMaterialConfiguration();
  const fearray<long>& MMC = GetMainMaterialConfig();
  InitMaterial(MainMaterial,
               MAKE_MATERIAL(MMC.Data[MMC.Size == 1 ? 0 : Chosen]),
               GetDefaultMainVolume());
}

void item::SignalSquarePositionChange(int Position)
{
  Flags &= ~SQUARE_POSITION_BITS;
  Flags |= Position << SQUARE_POSITION_SHIFT;
}

truth item::Read(character* Reader)
{
  Reader->StartReading(this, GetReadDifficulty());
  return true;
}

truth item::CanBeHardened(ccharacter*) const
{
  return MainMaterial->GetHardenedMaterial(this) != NONE;
}

truth item::CanBeSoftened() const
{
  return MainMaterial->GetSoftenedMaterial(this) != NONE;
}

void item::SetLifeExpectancy(int Base, int RandPlus)
{
  LifeExpectancy = RandPlus > 1 ? Base + RAND_N(RandPlus) : Base;
  Enable();
}

truth item::IsVeryCloseToSpoiling() const
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && !GetMaterial(c)->IsVeryCloseToSpoiling())
      return false;

  return true;
}

truth item::IsVeryCloseToBurning() const
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && !GetMaterial(c)->IsVeryCloseToBurning())
      return false;

  return true;
}

truth item::IsValuable() const
{
  if(DataBase->IsValuable)
    return true;

  for(int c = 0; c < GetMaterials(); ++c)
  {
    material* M = GetMaterial(c);

    if(M && M->GetCommonFlags() & IS_VALUABLE)
      return true;
  }

  return false;
}

int item::GetHinderVisibilityBonus(ccharacter* Char) const
{
  int Bonus = 0;

  if(GetGearStates() & INFRA_VISION
     && !Char->TemporaryStateIsActivated(INFRA_VISION))
    Bonus += 20000;

  if(GetGearStates() & ESP
     && !Char->TemporaryStateIsActivated(ESP))
    Bonus += 20000;

  if(!game::IsDark(GetEmitation()))
    Bonus += 5000;

  return Bonus;
}

long item::GetFixPrice() const
{
  item* Clone = GetProtoType()->Clone(this);
  Clone = Clone->Fix();
  Clone->RemoveRust();
  Clone->RemoveBurns();
  long FixPrice = Clone->GetTruePrice();
  Clone->SendToHell();
  return Max(long(3.5 * sqrt(FixPrice)), 10L);
}

void item::AddTrapName(festring& String, int Amount) const
{
  if(Amount == 1)
    AddName(String, DEFINITE);
  else
  {
    String << Amount << ' ';
    AddName(String, PLURAL);
  }
}

truth item::Spoils() const
{
  for(int c = 0; c < GetMaterials(); ++c)
  {
    cmaterial* Material = GetMaterial(c);

    if(Material && Material->Spoils())
      return true;
  }

  return false;
}

int item::GetMaxSpoilPercentage() const
{
  int MaxPercentage = 0;

  for(int c = 0; c < GetMaterials(); ++c)
  {
    cmaterial* Material = GetMaterial(c);

    if(Material)
      MaxPercentage = Max(MaxPercentage, Material->GetSpoilPercentage());
  }

  return MaxPercentage;
}

truth item::HasPrice() const
{
  return GetPrice() || GetMaterialPrice();
}

void item::Disappear()
{
  RemoveFromSlot();
  SendToHell();
}

outputfile& operator<<(outputfile& SaveFile, const idholder* IdHolder)
{
  SaveFile << IdHolder->ID;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, idholder*& IdHolder)
{
  IdHolder = new idholder(ReadType<ulong>(SaveFile));
  return SaveFile;
}

festring item::GetExtendedDescription() const
{
  if(!CanBeSeenByPlayer())
    return CONST_S("something");

  festring Desc;
  ccharacter* Carrier = FindCarrier();

  if(Carrier)
  {
    if(Carrier->IsPlayer())
    {
      Desc << "your ";
      AddName(Desc, UNARTICLED);
      return Desc;
    }
    else if(Carrier->CanBeSeenByPlayer())
    {
      Carrier->AddName(Desc, DEFINITE);
      Desc << "'s ";
      AddName(Desc, UNARTICLED);
      return Desc;
    }
  }

  AddName(Desc, DEFINITE);

  if(IsOnGround())
    GetLSquareUnder()->AddLocationDescription(Desc);

  return Desc;
}

ccharacter* item::FindCarrier() const
{
  return Slot[0]->FindCarrier();
}

/* returns 0 if not worn or wielded else the wearer */

const character* item::GetWearer() const
{
  if(!GetSlot()->IsGearSlot())
    return 0;

  return FindCarrier();
}

void itemlock::PostConstruct()
{
  /* Terrible gum solution! */

  if(!(GetVirtualConfig() & LOCK_BITS))
  {
    int NormalLockTypes = 0;
    const itemdatabase*const* ConfigData = GetVirtualProtoType()->GetConfigData();
    int c, ConfigSize = GetVirtualProtoType()->GetConfigSize();

    for(c = 0; c < ConfigSize; ++c)
      if(ConfigData[c]->Config & LOCK_BITS
         && (ConfigData[c]->Config & ~LOCK_BITS) == GetVirtualConfig()
         && !(ConfigData[c]->Config & S_LOCK_ID))
        ++NormalLockTypes;

    int ChosenLock = RAND() % NormalLockTypes;

    for(c = 0; c < ConfigSize; ++c)
      if(ConfigData[c]->Config & LOCK_BITS
         && (ConfigData[c]->Config & ~LOCK_BITS) == GetVirtualConfig()
         && !(ConfigData[c]->Config & S_LOCK_ID)
         && !ChosenLock--)
      {
        SetVirtualConfig(ConfigData[c]->Config, NO_PIC_UPDATE);
        break;
      }
  }
}

truth itemlock::TryKey(item* Key, character* Applier)
{
  if(GetVirtualConfig() & BROKEN_LOCK)
  {
    ADD_MESSAGE("The lock is broken.");
    return true;
  }

  if(Key->CanOpenLockType(GetVirtualConfig()&LOCK_BITS))
  {
    if(Locked)
    {
      if(Applier->IsPlayer())
        ADD_MESSAGE("You unlock %s.", GetVirtualDescription(DEFINITE).CStr());
      else if(Applier->CanBeSeenByPlayer())
        ADD_MESSAGE("%s unlocks %s.", Applier->CHAR_NAME(DEFINITE), GetVirtualDescription(DEFINITE).CStr());
    }
    else
    {
      if(Applier->IsPlayer())
        ADD_MESSAGE("You lock %s.", GetVirtualDescription(DEFINITE).CStr());
      else if(Applier->CanBeSeenByPlayer())
        ADD_MESSAGE("%s locks %s.", Applier->CHAR_NAME(DEFINITE), GetVirtualDescription(DEFINITE).CStr());
    }

    // Add a tiny chance that any key you use breaks, so that there is some value in having
    // multiples of a key, and in keys of better materials.
    if(!RAND_N(Key->GetMainMaterial()->GetStrengthValue()))
    {
      Key->Break(Applier);
    }

    Locked = !Locked;
  }
  else
  {
    if(Applier->IsPlayer())
      ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHAR_NAME(DEFINITE));
    else if(Applier->CanBeSeenByPlayer())
      ADD_MESSAGE("%s tries to fit %s in the lock, but fails.",
                  Applier->CHAR_NAME(DEFINITE), Key->CHAR_NAME(DEFINITE));
  }

  return true;
}

void itemlock::Save(outputfile& SaveFile) const
{
  SaveFile << Locked;
}

void itemlock::Load(inputfile& SaveFile)
{
  SaveFile >> Locked;
}

truth item::IsBeverage(ccharacter*) const
{
  for(int c = 0; c < GetMaterials(); ++c)
  {
    cmaterial* Material = GetMaterial(c);

    if(Material && (Material->GetCategoryFlags() & IS_BEVERAGE))
      return true;
  }

  return false;
}

void item::Haste()
{
  ItemFlags |= HASTE;
  ItemFlags &= ~SLOW;
  SendMemorizedUpdateRequest();
}

void item::Slow()
{
  ItemFlags |= SLOW;
  ItemFlags &= ~HASTE;
  SendMemorizedUpdateRequest();
}

void item::SendMemorizedUpdateRequest() const
{
  if(!game::IsInWilderness())
    for(int c = 0; c < SquaresUnder; ++c)
      if(Slot[c])
      {
        lsquare* Square = GetLSquareUnder(c);
        Square->SendMemorizedUpdateRequest();
      }
}

void item::AddContainerPostFix(festring& String) const
{
  if(GetSecondaryMaterial()){
    float fRatio = GetSecondaryMaterial()->GetVolume()/(float)GetDefaultSecondaryVolume();
    const char* c="full of";
    if     (fRatio<=0.10)c="with just a little bit of";
    else if(fRatio<=0.25)c="with a bit of";
    else if(fRatio<=0.45)c="with some";
    else if(fRatio<=0.55)c="half full of";
    else if(fRatio<=0.75)c="well filled with"; //TODO any better phrasing for this?
    else if(fRatio<=0.93)c="almost full of"; //nice arguable arbitrary percent :)
    GetSecondaryMaterial()->AddName(String << " "<< c << " ", false, false);
  }
}

truth item::AddStateDescription(festring& Name, truth Articled) const
{
  if(!Spoils() || !(ItemFlags & (HASTE|SLOW)))
    return false;

  if(Articled)
    Name << "a ";

  if(ItemFlags & HASTE)
    Name << "hasted ";

  if(ItemFlags & SLOW)
    Name << "slowed ";

  return true;
}
