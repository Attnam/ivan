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

cchar* ToHitValueDescription[] = { "unbelievably inaccurate", "extremely inaccurate", "inaccurate", "decently accurate", "accurate", "highly accurate", "extremely accurate", "unbelievably accurate" };
cchar* StrengthValueDescription[] = { "fragile", "rather sturdy", "sturdy", "strong", "very strong", "extremely strong", "almost unbreakable" };

itemprototype::itemprototype(const itemprototype* Base, itemspawner Spawner, itemcloner Cloner, cchar* ClassID) : Base(Base), Spawner(Spawner), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<item>::Add(this); }

truth itemdatabase::AllowRandomInstantiation() const { return !(Config & S_LOCK_ID); }

item::item() : Slot(0), CloneMotherID(0), Fluid(0), LifeExpectancy(0), ItemFlags(0) { }
truth item::IsOnGround() const { return Slot[0]->IsOnGround(); }
truth item::IsSimiliarTo(item* Item) const { return Item->GetType() == GetType() && Item->GetConfig() == GetConfig(); }
double item::GetBaseDamage() const { return sqrt(5e-5 * GetWeaponStrength()) + GetDamageBonus(); }
int item::GetBaseMinDamage() const { return int(GetBaseDamage() * 0.75); }
int item::GetBaseMaxDamage() const { return int(GetBaseDamage() * 1.25) + 1; }
int item::GetBaseToHitValue() const { return int(10000. / (1000 + GetWeight()) + GetTHVBonus()); }
int item::GetBaseBlockValue() const { return int((10000. / (1000 + GetWeight()) + GetTHVBonus()) * GetBlockModifier() / 10000.); }
truth item::IsInCorrectSlot(int I) const { return I == RIGHT_WIELDED_INDEX || I == LEFT_WIELDED_INDEX; }
truth item::IsInCorrectSlot() const { return IsInCorrectSlot(static_cast<gearslot*>(*Slot)->GetEquipmentIndex()); }
int item::GetEquipmentIndex() const { return static_cast<gearslot*>(*Slot)->GetEquipmentIndex(); }
int item::GetGraphicsContainerIndex() const { return GR_ITEM; }
truth item::IsBroken() const { return GetConfig() & BROKEN; }
cchar* item::GetBreakVerb() const { return "breaks"; }
square* item::GetSquareUnderEntity(int I) const { return GetSquareUnder(I); }
square* item::GetSquareUnder(int I) const { return Slot[I] ? Slot[I]->GetSquareUnder() : 0; }
lsquare* item::GetLSquareUnder(int I) const { return static_cast<lsquare*>(Slot[I]->GetSquareUnder()); }
void item::SignalStackAdd(stackslot* StackSlot, void (stack::*)(item*, truth)) { Slot[0] = StackSlot; }
truth item::IsAnimated() const { return GraphicData.AnimationFrames > 1 || (Fluid && ShowFluids()); }
truth item::IsRusted() const { return MainMaterial->GetRustLevel() != NOT_RUSTED; }
truth item::IsEatable(ccharacter* Eater) const { return GetConsumeMaterial(Eater, &material::IsSolid) && IsConsumable(); }
truth item::IsDrinkable(ccharacter* Eater) const { return GetConsumeMaterial(Eater, &material::IsLiquid) && IsConsumable(); }
pixelpredicate item::GetFluidPixelAllowedPredicate() const { return &rawbitmap::IsTransparent; }
void item::Cannibalize() { Flags |= CANNIBALIZED; }
void item::SetMainMaterial(material* NewMaterial, int SpecialFlags) { SetMaterial(MainMaterial, NewMaterial, GetDefaultMainVolume(), SpecialFlags); }
void item::ChangeMainMaterial(material* NewMaterial, int SpecialFlags) { ChangeMaterial(MainMaterial, NewMaterial, GetDefaultMainVolume(), SpecialFlags); }
void item::InitMaterials(const materialscript* M, const materialscript*, truth CUP) { InitMaterials(M->Instantiate(), CUP); }
int item::GetMainMaterialRustLevel() const { return MainMaterial->GetRustLevel(); }

item::item(citem& Item) : object(Item), Slot(0), Size(Item.Size), DataBase(Item.DataBase), Volume(Item.Volume), Weight(Item.Weight), Fluid(0), SquaresUnder(Item.SquaresUnder), LifeExpectancy(Item.LifeExpectancy), ItemFlags(Item.ItemFlags)
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
}

void item::Fly(character* Thrower, int Direction, int Force)
{
  int Range = Force * 25 / Max(long(sqrt(GetWeight())), 1L);

  lsquare* LSquareUnder = GetLSquareUnder();
  RemoveFromSlot();
  LSquareUnder->GetStack()->AddItem(this, false);

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

  if(Thrower)
  {
    int Bonus = Thrower->IsHumanoid() ? Thrower->GetCWeaponSkill(GetWeaponCategory())->GetBonus() : 1000;
    BaseDamage = sqrt(5e-12 * GetWeaponStrength() * Force / Range) * Bonus;
    BaseToHitValue = 10 * Bonus * Thrower->GetMoveEase() / (500 + GetWeight()) * Thrower->GetAttribute(DEXTERITY) * sqrt(2.5e-8 * Thrower->GetAttribute(PERCEPTION)) / Range;
  }
  else
  {
    BaseDamage = sqrt(5e-6 * GetWeaponStrength() * Force / Range);
    BaseToHitValue = 10 * 100 / (500 + GetWeight()) / Range;
  }

  int RangeLeft;

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
      truth Draw = game::OnScreen(JustHit->GetPos()) && JustHit->CanBeSeenByPlayer();

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
	while(clock() - StartTime < 0.03 * CLOCKS_PER_SEC);
    }
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

void item::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  object::Save(SaveFile);
  SaveFile << (ushort)GetConfig();
  SaveFile << (ushort)Flags;
  SaveFile << Size << ID << LifeExpectancy << ItemFlags;
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
   case SOUND:
   case ENERGY:
   case DRAIN:
   case MUSTARD_GAS_DAMAGE:
    return 0;
   case FIRE: return GetFireResistance();
   case POISON: return GetPoisonResistance();
   case ELECTRICITY: return GetElectricityResistance();
   case ACID: return GetAcidResistance();
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

  idholder* I = new idholder(ID);
  I->Next = CloneMotherID;
  CloneMotherID = I;
  game::RemoveItemID(ID);
  ID = game::CreateNewItemID(this);
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

  if(ShowSpecialInfo)
    Entry << " [" << GetWeight() * Amount << "g]";
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
	  && (WeightIsIrrelevant() || Weight == Item->Weight)
	  && MainMaterial->IsSameAs(Item->MainMaterial)
	  && MainMaterial->GetSpoilLevel() == Item->MainMaterial->GetSpoilLevel()
	  && MainMaterial->GetRustLevel() == Item->MainMaterial->GetRustLevel()
	  && Viewer->GetCWeaponSkillLevel(this) == Viewer->GetCWeaponSkillLevel(Item)
	  && Viewer->GetSWeaponSkillLevel(this) == Viewer->GetSWeaponSkillLevel(Item)
	  && !Fluid && !Item->Fluid
	  && !LifeExpectancy == !Item->LifeExpectancy);
}

void item::Break(character* Breaker, int)
{
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
      --LifeExpectancy;
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
  return !PriceIsProportionalToEnchantment() ? item::GetPrice() : Max<int>(item::GetPrice() * Enchantment * Enchantment, 0);
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
  Entry << int(GetSize());
  Entry.Resize(60);
  Entry << int(GetStrengthRequirement());
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
    if(game::SearchItem(BI->second))
      int esko = esko = 2;

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
  cbitmap* P = GraphicData.Picture[F];

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

  if(Fluid)
    for(int c = 0; c < SquaresUnder; ++c)
      for(const fluid* F = Fluid[c]; F; F = F->Next)
	game::CombineLights(Emitation, F->GetEmitation());
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
      if(MainMaterial->GetRustLevel() == NOT_RUSTED)
	ADD_MESSAGE("%s rusts.", CHAR_NAME(DEFINITE));
      else
	ADD_MESSAGE("%s rusts more.", CHAR_NAME(DEFINITE));

    MainMaterial->SetRustLevel(MainMaterial->GetRustLevel() + 1);
  }
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

    Locked = !Locked;
  }
  else
  {
    if(Applier->IsPlayer())
      ADD_MESSAGE("%s doesn't fit in the lock.", Key->CHAR_NAME(DEFINITE));
    else if(Applier->CanBeSeenByPlayer())
      ADD_MESSAGE("%s tries to fit %s in the lock, but fails.", Applier->CHAR_NAME(DEFINITE), Key->CHAR_NAME(DEFINITE));
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

truth item::AddStateDescription(festring& Name, truth Articled) const
{
  if(!Spoils())
    return false;

  if((ItemFlags & (HASTE|SLOW)) && Articled)
    Name << "a ";

  if(ItemFlags & HASTE)
    Name << "hasted ";

  if(ItemFlags & SLOW)
    Name << "slowed ";

  return true;
}
