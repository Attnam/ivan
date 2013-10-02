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

/* Compiled through slotset.cpp */

/* If REMEMBER_SELECTED flag is used, DrawContents() will use this to determine
   the initial selected item */

int stack::Selected;

stack::stack(square* MotherSquare, entity* MotherEntity, ulong Flags)
: Bottom(0), Top(0), MotherSquare(MotherSquare), MotherEntity(MotherEntity),
  Volume(0), Weight(0), Emitation(0), Flags(Flags), Items(0) { }
stack::~stack() { Clean(true); }
square* stack::GetSquareUnder() const
{ return !MotherEntity ? MotherSquare : MotherEntity->GetSquareUnderEntity(); }

/* Modifies the square index bits of BlitData.CustomData */

void stack::Draw(ccharacter* Viewer, blitdata& BlitData,
		 int RequiredSquarePosition) const
{
  if(!Items)
    return;

  int VisibleItems = 0;
  v2 StackPos = GetPos();

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == RequiredSquarePosition
       && (i->CanBeSeenBy(Viewer) || game::GetSeeWholeMapCheatMode()))
    {
      BlitData.CustomData |= i->GetSquareIndex(StackPos);
      i->Draw(BlitData);
      BlitData.CustomData &= ~SQUARE_INDEX_MASK;
      ++VisibleItems;
    }

  if(RequiredSquarePosition == CENTER)
  {
    truth PlusSymbol = VisibleItems > 1, Dangerous = NeedDangerSymbol(Viewer);

    if(PlusSymbol || Dangerous)
    {
      col24 L = BlitData.Luminance;
      BlitData.Luminance = ivanconfig::GetContrastLuminance();
      BlitData.Src.Y = 16;

      if(PlusSymbol)
	igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);

      if(Dangerous)
      {
	BlitData.Src.X = 160;
	igraph::GetSymbolGraphic()->LuminanceMaskedBlit(BlitData);
      }

      BlitData.Src.X = BlitData.Src.Y = 0; /// check
      BlitData.Luminance = L;
    }
  }
}

void stack::AddItem(item* ToBeAdded, truth RunRoomEffects)
{
  if(!ToBeAdded)
    return;

  AddElement(ToBeAdded);

  if(Flags & HIDDEN)
    return;

  lsquare* SquareUnder = GetLSquareTrulyUnder(ToBeAdded->GetSquarePosition());

  if(!SquareUnder)
    return;

  if(ToBeAdded->IsAnimated())
    SquareUnder->IncStaticAnimatedEntities();

  if(!game::IsGenerating())
  {
    if(RunRoomEffects && GetLSquareUnder()->GetRoom())
      GetLSquareUnder()->GetRoom()->AddItemEffect(ToBeAdded);

    SquareUnder->SendNewDrawRequest();
    SquareUnder->SendMemorizedUpdateRequest();
  }
}

void stack::RemoveItem(stackslot* Slot)
{
  item* Item = Slot->GetItem();
  truth WasAnimated = Item->IsAnimated();
  col24 Emit = Item->GetEmitation();
  RemoveElement(Slot);
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Item->GetSquarePosition(), Emit);

  if(Flags & HIDDEN)
    return;

  lsquare* SquareUnder = GetLSquareTrulyUnder(Item->GetSquarePosition());

  if(Item->GetSquarePosition() != CENTER)
    Item->SignalSquarePositionChange(CENTER);

  if(!SquareUnder)
    return;

  if(WasAnimated)
    SquareUnder->DecStaticAnimatedEntities();

  if(!game::IsGenerating())
  {
    SquareUnder->SendNewDrawRequest();
    SquareUnder->SendMemorizedUpdateRequest();
  }
}

/* Removes all items. LastClean should be true only if the stack is being
   deleted (the default is false) */

void stack::Clean(truth LastClean)
{
  if(!Items)
    return;

  stackslot* Slot = Bottom;

  if(!LastClean)
  {
    Bottom = Top = 0;
    Volume = Weight = Items = 0;
    SignalVolumeAndWeightChange();
  }

  while(Slot)
  {
    item* Item = Slot->GetItem();

    if(!(Flags & HIDDEN) && Item->IsAnimated() && !LastClean)
    {
      lsquare* Square = GetLSquareTrulyUnder(Item->GetSquarePosition());

      if(Square)
	Square->DecStaticAnimatedEntities();
    }

    if(LastClean && Item->GetSquaresUnder() == 1)
      delete Item;
    else
      Item->SendToHell();

    stackslot* Rubbish = Slot;
    Slot = Slot->Next;
    delete Rubbish;

    if(!LastClean)
      SignalEmitationDecrease(Item->GetSquarePosition(), Item->GetEmitation());
  }
}

void stack::Save(outputfile& SaveFile) const
{
  if(!Items)
  {
    SaveFile << ushort(0);
    return;
  }

  int SavedItems = 0;

  for(stackiterator i1 = GetBottom(); i1.HasItem(); ++i1)
    if(i1->IsMainSlot(&i1.GetSlot()))
      ++SavedItems;

  SaveFile << (ushort)SavedItems;

  /* Save multitiled items only to one stack */

  for(stackiterator i2 = GetBottom(); i2.HasItem(); ++i2)
    if(i2->IsMainSlot(&i2.GetSlot()))
      SaveFile << i2.GetSlot();
}

void stack::Load(inputfile& SaveFile)
{
  int SavedItems = 0;
  SaveFile >> (ushort&)SavedItems;

  for(int c = 0; c < SavedItems; ++c)
  {
    if(!c && !Items)
      Bottom = Top = new stackslot(this, 0);
    else
      Top = Top->Next = new stackslot(this, Top);

    SaveFile >> *Top;
    Volume += (*Top)->GetVolume();
    Weight += (*Top)->GetWeight();

    if((*Top)->GetSquarePosition() == CENTER)
      Emitation = game::CombineConstLights(Emitation, (*Top)->GetEmitation());
  }

  Items += SavedItems;
}

v2 stack::GetPos() const
{
  return GetSquareUnder()->GetPos();
}

/* Returns whether there are any items satisfying the sorter or any visible
   items if it is zero */

truth stack::SortedItems(ccharacter* Viewer, sorter SorterFunction) const
{
  if(Items)
    for(stackiterator i = GetBottom(); i.HasItem(); ++i)
      if((SorterFunction == 0 || ((*i)->*SorterFunction)(Viewer))
	 && ((Flags & HIDDEN) || i->CanBeSeenBy(Viewer)))
	return true;

  return false;
}

void stack::BeKicked(character* Kicker, int KickDamage, int Direction)
{
  if(KickDamage)
  {
    ReceiveDamage(Kicker, KickDamage, PHYSICAL_DAMAGE, Direction);

    if(GetItems() && GetLSquareUnder()->IsFlyable())///&& SquarePosition == CENTER)
    {
      item* Item1 = *GetTop();
      item* Item2 = RAND() & 1 && GetItems() > 1 ? *--GetTop() : 0;
      Item1->Fly(Kicker, Direction, KickDamage * 3);

      if(Item2)
      {
	if(!Item2->Exists() || Item2->GetPos() != GetPos())
	  int esko = esko = 2;

	Item2->Fly(Kicker, Direction, KickDamage * 3);
      }
    }
  }
  else if(Kicker->IsPlayer() && GetNativeVisibleItems(Kicker))
    ADD_MESSAGE("Your weak kick has no effect.");
}

void stack::Polymorph(character* Polymorpher)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);
  int p = 0;

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists()
       && ItemVector[c]->Polymorph(Polymorpher, this)
       && ++p == 5)
      break;
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists())
    {
      ItemVector[c]->StepOnEffect(Stepper);

      if(!Stepper->IsEnabled())
	return;
    }
}

lsquare* stack::GetLSquareTrulyUnder(int SquarePosition) const
{
  switch(SquarePosition)
  {
   case DOWN:
    if(GetArea()->IsValidPos(GetPos() + v2(0, 1)))
      return GetNearLSquare(GetPos() + v2(0, 1));
    else
      return 0;
   case LEFT:
    if(GetArea()->IsValidPos(GetPos() + v2(-1, 0)))
      return GetNearLSquare(GetPos() + v2(-1, 0));
    else
      return 0;
   case UP:
    if(GetArea()->IsValidPos(GetPos() + v2(0, -1)))
      return GetNearLSquare(GetPos() + v2(0, -1));
    else
      return 0;
   case RIGHT:
    if(GetArea()->IsValidPos(GetPos() + v2(1, 0)))
      return GetNearLSquare(GetPos() + v2(1, 0));
    else
      return 0;
  }

  return GetLSquareUnder();
}

void stack::ReceiveDamage(character* Damager, int Damage,
			  int Type, int Direction)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists()
       && AllowDamage(Direction, ItemVector[c]->GetSquarePosition()))
      ItemVector[c]->ReceiveDamage(Damager, Damage, Type);
}

void stack::TeleportRandomly(uint Amount)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size() && c < Amount; ++c)
    if(ItemVector[c]->Exists())
    {
      if(ItemVector[c]->CanBeSeenByPlayer())
	ADD_MESSAGE("%s disappears!",
		    ItemVector[c]->GetExtendedDescription().CStr());

      ItemVector[c]->TeleportRandomly();
    }
}

/* ItemVector receives all items in the stack */

void stack::FillItemVector(itemvector& ItemVector) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    ItemVector.push_back(*i);
}

/* Don't use; this function is only for gum solutions */

item* stack::GetItem(int I) const
{
  int c = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i, ++c)
    if(c == I)
      return *i;

  return 0;
}

/* Don't use; this function is only for gum solutions */

int stack::SearchItem(item* ToBeSearched) const
{
  int c = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i, ++c)
    if(*i == ToBeSearched)
      return c;

  return -1;
}

/* Flags for all DrawContents functions can be found in ivandef.h.
   Those returning int return 0 on success and a felist error
   otherwise (see felibdef.h) */

item* stack::DrawContents(ccharacter* Viewer, cfestring& Topic,
			  int Flags, sorter SorterFunction) const
{
  itemvector ReturnVector;
  DrawContents(ReturnVector, 0, Viewer, Topic, CONST_S(""), CONST_S(""),
	       CONST_S(""), 0, Flags|NO_MULTI_SELECT, SorterFunction);
  return ReturnVector.empty() ? 0 : ReturnVector[0];
}

int stack::DrawContents(itemvector& ReturnVector,
			ccharacter* Viewer,
			cfestring& Topic, int Flags,
			sorter SorterFunction) const
{
  return DrawContents(ReturnVector, 0, Viewer, Topic, CONST_S(""),
		      CONST_S(""), CONST_S(""), 0, Flags, SorterFunction);
}

/* MergeStack is used for showing two stacks together. Like when eating when
   there are items on the ground and in the character's stack */

int stack::DrawContents(itemvector& ReturnVector, stack* MergeStack,
			ccharacter* Viewer, cfestring& Topic,
			cfestring& ThisDesc, cfestring& ThatDesc,
			cfestring& SpecialDesc, col16 SpecialDescColor,
			int Flags, sorter SorterFunction) const
{
  felist Contents(Topic);
  lsquare* Square = GetLSquareUnder();
  stack* AdjacentStack[4] = { 0, 0, 0, 0 };
  int c;

  if(!(this->Flags & HIDDEN))
    for(c = 0; c < 4; ++c)
      AdjacentStack[c] = Square->GetStackOfAdjacentSquare(c);

  if(!SpecialDesc.IsEmpty())
  {
    Contents.AddDescription(CONST_S(""));
    Contents.AddDescription(SpecialDesc.CapitalizeCopy(), SpecialDescColor);
  }

  /*if(!(Flags & NO_SPECIAL_INFO))
    {
    Contents.AddDescription(CONST_S(""));
    long Weight = GetWeight(Viewer, CENTER);

    if(MergeStack)
    Weight += MergeStack->GetWeight(Viewer, CENTER);

    for(c = 0; c < 4; ++c)
    if(AdjacentStack[c])
    Weight += AdjacentStack[c]->GetWeight(Viewer, 3 - c);

    Contents.AddDescription(CONST_S("Overall weight: ") + Weight + " grams");
    }*/

  if(Flags & NONE_AS_CHOICE)
  {
    int ImageKey = game::AddToItemDrawVector(itemvector());
    Contents.AddEntry(CONST_S("none"), LIGHT_GRAY, 0, ImageKey);
  }

  if(MergeStack)
    MergeStack->AddContentsToList(Contents, Viewer, ThatDesc,
				  Flags, CENTER, SorterFunction);

  AddContentsToList(Contents, Viewer, ThisDesc, Flags, CENTER, SorterFunction);
  static cchar* WallDescription[] = { "western", "southern",
					   "nothern", "eastern" };

  for(c = 0; c < 4; ++c)
    if(AdjacentStack[c])
      AdjacentStack[c]->AddContentsToList(Contents, Viewer,
					  CONST_S("Items on the ")
					  + WallDescription[c] + " wall:",
					  Flags, 3 - c, SorterFunction);

  game::SetStandardListAttributes(Contents);
  Contents.SetPageLength(12);
  Contents.RemoveFlags(BLIT_AFTERWARDS);
  Contents.SetEntryDrawer(game::ItemEntryDrawer);

  if(!(Flags & NO_SELECT))
    Contents.AddFlags(SELECTABLE);

  if(Flags & REMEMBER_SELECTED)
    Contents.SetSelected(GetSelected());

  game::DrawEverythingNoBlit(); //doesn't prevent mirage puppies
  int Chosen = Contents.Draw();
  game::ClearItemDrawVector();

  if(Chosen & FELIST_ERROR_BIT)
  {
    Selected = 0;
    return Chosen;
  }
  else
    Selected = Chosen;

  int Pos = 0;

  if(Flags & NONE_AS_CHOICE)
    if(!Selected)
      return 0;
    else
      ++Pos;

  if(MergeStack)
  {
    Pos = MergeStack->SearchChosen(ReturnVector, Viewer, Pos, Selected,
				   Flags, CENTER, SorterFunction);

    if(!ReturnVector.empty())
      return 0;
  }

  Pos = SearchChosen(ReturnVector, Viewer, Pos, Selected,
		     Flags, CENTER, SorterFunction);

  if(!ReturnVector.empty())
    return 0;

  for(c = 0; c < 4; ++c)
    if(AdjacentStack[c])
    {
      AdjacentStack[c]->SearchChosen(ReturnVector, Viewer, Pos, Selected,
				     Flags, 3 - c, SorterFunction);

      if(!ReturnVector.empty())
	break;
    }

  return 0;
}

/* Internal function to fill Contents list */

void stack::AddContentsToList(felist& Contents, ccharacter* Viewer,
			      cfestring& Desc, int Flags,
			      int RequiredSquarePosition,
			      sorter SorterFunction) const
{
  itemvectorvector PileVector;
  Pile(PileVector, Viewer, RequiredSquarePosition, SorterFunction);

  truth DrawDesc = Desc.GetSize();
  long LastCategory = 0;
  festring Entry;

  for(uint p = 0; p < PileVector.size(); ++p)
  {
    if(DrawDesc)
    {
      if(!Contents.IsEmpty())
	Contents.AddEntry(CONST_S(""), WHITE, 0, NO_IMAGE, false);

      Contents.AddEntry(Desc, WHITE, 0, NO_IMAGE, false);
      Contents.AddEntry(CONST_S(""), WHITE, 0, NO_IMAGE, false);
      DrawDesc = false;
    }

    item* Item = PileVector[p].back();

    if(Item->GetCategory() != LastCategory)
    {
      LastCategory = Item->GetCategory();
      Contents.AddEntry(item::GetItemCategoryName(LastCategory),
			LIGHT_GRAY, 0, NO_IMAGE, false);
    }

    Entry.Empty();
    Item->AddInventoryEntry(Viewer, Entry, PileVector[p].size(),
			    !(Flags & NO_SPECIAL_INFO));
    int ImageKey = game::AddToItemDrawVector(PileVector[p]);
    Contents.AddEntry(Entry, LIGHT_GRAY, 0, ImageKey);
  }
}

/* Internal function which fills ReturnVector according to Chosen,
   which is given by felist::Draw, and possibly the user's additional
   input about item amount. */

int stack::SearchChosen(itemvector& ReturnVector,
			ccharacter* Viewer,
			int Pos, int Chosen, int Flags,
			int RequiredSquarePosition,
			sorter SorterFunction) const
{
  /* Not really efficient... :( */

  itemvectorvector PileVector;
  Pile(PileVector, Viewer, RequiredSquarePosition, SorterFunction);

  for(uint p = 0; p < PileVector.size(); ++p)
    if(Pos++ == Chosen)
      if(Flags & NO_MULTI_SELECT)
      {
	int Amount = (Flags & SELECT_PAIR
		      && PileVector[p][0]->HandleInPairs()
		      && PileVector[p].size() >= 2
		      ? 2 : 1);
	ReturnVector.assign(PileVector[p].end() - Amount, PileVector[p].end());
	return -1;
      }
      else
      {
	int Amount = PileVector[p].size();

	if(Amount > 1)
	  Amount = game::ScrollBarQuestion(CONST_S("How many ")
					   + PileVector[p][0]->GetName(PLURAL)
					   + '?',
					   Amount, 1, 0, Amount, 0, WHITE,
					   LIGHT_GRAY, DARK_GRAY);

	ReturnVector.assign(PileVector[p].end() - Amount, PileVector[p].end());
	return -1;
      }

  return Pos;
}

truth stack::RaiseTheDead(character* Summoner)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->RaiseTheDead(Summoner))
      return true;

  return false;
}

/* Returns false if the Applier didn't try to use the key */

truth stack::TryKey(item* Key, character* Applier)
{
  if(!Applier->IsPlayer())
    return false;

  item* ToBeOpened = DrawContents(Applier,
				  CONST_S("Where do you wish to use the key?"),
				  0, &item::HasLock);

  if(!ToBeOpened)
    return false;

  return ToBeOpened->TryKey(Key, Applier);
}

/* Returns false if the Applier didn't try to open anything */

truth stack::Open(character* Opener)
{
  if(!Opener->IsPlayer())
    return false;

  item* ToBeOpened = DrawContents(Opener, CONST_S("What do you wish to open?"),
				  0, &item::IsOpenable);
  return ToBeOpened ? ToBeOpened->Open(Opener) : false;
}

int stack::GetSideItems(int RequiredSquarePosition) const
{
  int VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == RequiredSquarePosition)
      ++VisibleItems;

  return VisibleItems;
}

int stack::GetVisibleItems(ccharacter* Viewer) const
{
  int VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == CENTER && i->CanBeSeenBy(Viewer))
      ++VisibleItems;

  lsquare* Square = GetLSquareUnder();

  for(int c = 0; c < 4; ++c)
  {
    stack* Stack = Square->GetStackOfAdjacentSquare(c);

    if(Stack)
      VisibleItems += Stack->GetVisibleSideItems(Viewer, 3 - c);
  }

  return VisibleItems;
}

int stack::GetNativeVisibleItems(ccharacter* Viewer) const
{
  if(Flags & HIDDEN)
    return Items;

  int VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->CanBeSeenBy(Viewer))
      ++VisibleItems;

  return VisibleItems;
}

int stack::GetVisibleSideItems(ccharacter* Viewer,
			       int RequiredSquarePosition) const
{
  int VisibleItems = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == RequiredSquarePosition
       && i->CanBeSeenBy(Viewer))
      ++VisibleItems;

  return VisibleItems;
}

item* stack::GetBottomVisibleItem(ccharacter* Viewer) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if((Flags & HIDDEN) || i->CanBeSeenBy(Viewer))
      return *i;

  return 0;
}

void stack::SignalVolumeAndWeightChange()
{
  if(!(Flags & FREEZED))
  {
    CalculateVolumeAndWeight();

    if(MotherEntity)
      MotherEntity->SignalVolumeAndWeightChange();
  }
}

void stack::CalculateVolumeAndWeight()
{
  Volume = Weight = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
  {
    Volume += i->GetVolume();
    Weight += i->GetWeight();
  }
}

void stack::SignalEmitationIncrease(int ItemSquarePosition,
				    col24 EmitationUpdate)
{
  if(ItemSquarePosition < CENTER)
  {
    stack* Stack = GetLSquareUnder()
		   ->GetStackOfAdjacentSquare(ItemSquarePosition);

    if(Stack)
      Stack->SignalEmitationIncrease(CENTER, EmitationUpdate);

    return;
  }

  if(!(Flags & FREEZED) && game::CompareLights(EmitationUpdate, Emitation) > 0)
  {
    Emitation = game::CombineConstLights(Emitation, EmitationUpdate);

    if(MotherEntity)
    {
      if(MotherEntity->AllowContentEmitation())
	MotherEntity->SignalEmitationIncrease(EmitationUpdate);
    }
    else
      GetLSquareUnder()->SignalEmitationIncrease(EmitationUpdate);
  }
}

void stack::SignalEmitationDecrease(int ItemSquarePosition,
				    col24 EmitationUpdate)
{
  if(ItemSquarePosition < CENTER)
  {
    stack* Stack = GetLSquareUnder()
		   ->GetStackOfAdjacentSquare(ItemSquarePosition);

    if(Stack)
      Stack->SignalEmitationDecrease(CENTER, EmitationUpdate);

    return;
  }

  if(!(Flags & FREEZED) && Emitation
     && game::CompareLights(EmitationUpdate, Emitation) >= 0)
  {
    col24 Backup = Emitation;
    CalculateEmitation();

    if(Backup != Emitation)
    {
      if(MotherEntity)
      {
	if(MotherEntity->AllowContentEmitation())
	  MotherEntity->SignalEmitationDecrease(EmitationUpdate);
      }
      else
	GetLSquareUnder()->SignalEmitationDecrease(EmitationUpdate);
    }
  }
}

void stack::CalculateEmitation()
{
  Emitation = GetSideEmitation(CENTER);
}

col24 stack::GetSideEmitation(int RequiredSquarePosition)
{
  col24 Emitation = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == RequiredSquarePosition)
      game::CombineLights(Emitation, i->GetEmitation());

  return Emitation;
}

truth stack::CanBeSeenBy(ccharacter* Viewer, int SquarePosition) const
{
  if(MotherEntity)
    return MotherEntity->ContentsCanBeSeenBy(Viewer);
  else
  {
    lsquare* Square = GetLSquareTrulyUnder(SquarePosition);
    return Viewer->IsOver(Square->GetPos()) || Square->CanBeSeenBy(Viewer);
  }
}

truth stack::IsDangerous(ccharacter* Stepper) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->IsDangerous(Stepper) && i->CanBeSeenBy(Stepper))
      return true;

  return false;
}

/* Returns true if something was duplicated.
   Max is the cap of items to be affected */

truth stack::Duplicate(int Max, ulong Flags)
{
  if(!GetItems())
    return false;

  itemvector ItemVector;
  FillItemVector(ItemVector);
  int p = 0;

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Exists()
       && ItemVector[c]->DuplicateToStack(this, Flags)
       && ++p == Max)
      break;

  return p > 0;
}

/* Adds the item without any external update requests */

void stack::AddElement(item* Item, truth)
{
  ++Items;

  /* "I love writing illegible code." - Guy who wrote this */

  (Top = (Bottom ? Top->Next : Bottom) = new stackslot(this, Top))->PutInItem(Item);
}

/* Removes the slot without any external update requests */

void stack::RemoveElement(stackslot* Slot)
{
  --Items;
  (Slot->Last ? Slot->Last->Next : Bottom) = Slot->Next;
  (Slot->Next ? Slot->Next->Last : Top) = Slot->Last;
  delete Slot;
}

void stack::MoveItemsTo(stack* Stack)
{
  while(Items)
    GetBottom()->MoveTo(Stack);
}

void stack::MoveItemsTo(slot* Slot)
{
  while(Items)
    Slot->AddFriendItem(*GetBottom());
}

item* stack::GetBottomItem(ccharacter* Char,
			   truth ForceIgnoreVisibility) const
{
  if((Flags & HIDDEN) || ForceIgnoreVisibility)
    return Bottom ? **Bottom : 0;
  else
    return GetBottomVisibleItem(Char);
}

item* stack::GetBottomSideItem(ccharacter* Char,
			       int RequiredSquarePosition,
			       truth ForceIgnoreVisibility) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == RequiredSquarePosition
       && (Flags & HIDDEN) || ForceIgnoreVisibility || i->CanBeSeenBy(Char))
      return *i;

  return 0;
}

truth CategorySorter(const itemvector& V1, const itemvector& V2)
{
  return (*V1.begin())->GetCategory() < (*V2.begin())->GetCategory();
}

/* Slow function which sorts the stack's contents to a vector of piles
   (itemvectors) of which elements are similiar to each other, for instance
   4 bananas */

void stack::Pile(itemvectorvector& PileVector, ccharacter* Viewer,
		 int RequiredSquarePosition, sorter SorterFunction) const
{
  if(!Items)
    return;

  std::list<item*> List;

  for(stackiterator s = GetBottom(); s.HasItem(); ++s)
    if(s->GetSquarePosition() == RequiredSquarePosition
       && (SorterFunction == 0 || ((*s)->*SorterFunction)(Viewer))
       && ((Flags & HIDDEN) || s->CanBeSeenBy(Viewer)))
      List.push_back(*s);

  for(std::list<item*>::iterator i = List.begin(); i != List.end(); ++i)
  {
    PileVector.resize(PileVector.size() + 1);
    itemvector& Pile = PileVector.back();
    Pile.push_back(*i);

    if((*i)->CanBePiled())
    {
      std::list<item*>::iterator j = i;

      for(++j; j != List.end();)
	if((*j)->CanBePiled() && (*i)->CanBePiledWith(*j, Viewer))
	{
	  Pile.push_back(*j);
	  std::list<item*>::iterator Dirt = j++;
	  List.erase(Dirt);
	}
	else
	  ++j;
    }
  }

  std::stable_sort(PileVector.begin(), PileVector.end(), CategorySorter);
}

/* Total price of the stack */

long stack::GetTruePrice() const
{
  long Price = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Price += i->GetTruePrice();

  return Price;
}

/* GUI used for instance by chests and bookcases.
   Returns whether anything was done. */

truth stack::TakeSomethingFrom(character* Opener,
			       cfestring& ContainerName)
{
  if(!GetItems())
  {
    ADD_MESSAGE("There is nothing in %s.", ContainerName.CStr());
    return false;
  }

  truth Success = false;
  room* Room = GetLSquareUnder()->GetRoom();
  SetSelected(0);

  for(;;)
  {
    itemvector ToTake;
    game::DrawEverythingNoBlit();
    DrawContents(ToTake, Opener,
		 CONST_S("What do you want to take from ")
		 + ContainerName + '?',
		 REMEMBER_SELECTED);

    if(ToTake.empty())
      break;

    if(!IsOnGround() || !Room
       || Room->PickupItem(Opener, ToTake[0], ToTake.size()))
    {
      for(uint c = 0; c < ToTake.size(); ++c)
	ToTake[c]->MoveTo(Opener->GetStack());

      ADD_MESSAGE("You take %s from %s.",
		  ToTake[0]->GetName(DEFINITE, ToTake.size()).CStr(),
		  ContainerName.CStr());
      Success = true;
    }
  }

  return Success;
}

/* GUI used for instance by chests and bookcases (use ContainerID == 0 if
   the container isn't an item). Returns whether anything was done. */

truth stack::PutSomethingIn(character* Opener, cfestring& ContainerName,
			    long StorageVolume, ulong ContainerID)
{
  if(!Opener->GetStack()->GetItems())
  {
    ADD_MESSAGE("You have nothing to put in %s.", ContainerName.CStr());
    return false;
  }

  truth Success = false;
  room* Room = GetLSquareUnder()->GetRoom();
  SetSelected(0);

  for(;;)
  {
    itemvector ToPut;
    game::DrawEverythingNoBlit();
    Opener->GetStack()->DrawContents(ToPut, Opener,
				     CONST_S("What do you want to put in ")
				     + ContainerName + '?',
				     REMEMBER_SELECTED);

    if(ToPut.empty())
      break;

    if(ToPut[0]->GetID() == ContainerID)
    {
      ADD_MESSAGE("You can't put %s inside itself!", ContainerName.CStr());
      continue;
    }

    uint Amount = Min<uint>((StorageVolume - GetVolume())
			    / ToPut[0]->GetVolume(),
			    ToPut.size());

    if(!Amount)
    {
      if(ToPut.size() == 1)
	ADD_MESSAGE("%s doesn't fit in %s.",
		    ToPut[0]->CHAR_NAME(DEFINITE),
		    ContainerName.CStr());
      else
	ADD_MESSAGE("None of the %d %s fit in %s.", int(ToPut.size()),
		    ToPut[0]->CHAR_NAME(PLURAL), ContainerName.CStr());

      continue;
    }

    if(Amount != ToPut.size())
      ADD_MESSAGE("Only %d of the %d %s fit%s in %s.", Amount,
		  int(ToPut.size()), ToPut[0]->CHAR_NAME(PLURAL),
		  Amount == 1 ? "s" : "", ContainerName.CStr());

    if(!IsOnGround() || !Room || Room->DropItem(Opener, ToPut[0], Amount))
    {
      for(uint c = 0; c < Amount; ++c)
	ToPut[c]->MoveTo(this);

      ADD_MESSAGE("You put %s in %s.",
		  ToPut[0]->GetName(DEFINITE, Amount).CStr(),
		  ContainerName.CStr());
      Success = true;
    }
  }

  return Success;
}

truth stack::IsOnGround() const
{
  return !MotherEntity || MotherEntity->IsOnGround();
}

int stack::GetSpoiledItems() const
{
  int Counter = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    Counter += (i->GetSpoilLevel() > 0); // even though this is pretty unclear, it isn't mine but Hex's

  return Counter;
}

/* Adds all items and recursively their contents
   which satisfy the sorter to ItemVector */

void stack::SortAllItems(const sortdata& SortData) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->SortAllItems(SortData);
}

/* Search for traps and other secret items */

void stack::Search(ccharacter* Char, int Perception)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->Search(Char, Perception);
}

/* Used to determine whether the danger symbol should be shown */

truth stack::NeedDangerSymbol(ccharacter* Viewer) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->NeedDangerSymbol() && i->CanBeSeenBy(Viewer))
      return true;

  return false;
}

void stack::PreProcessForBone()
{
  if(Items)
  {
    itemvector ItemVector;
    FillItemVector(ItemVector);

    for(uint c = 0; c < ItemVector.size(); ++c)
      ItemVector[c]->PreProcessForBone();
  }
}

void stack::PostProcessForBone()
{
  if(Items)
  {
    itemvector ItemVector;
    FillItemVector(ItemVector);

    for(uint c = 0; c < ItemVector.size(); ++c)
      ItemVector[c]->PostProcessForBone();
  }
}

void stack::FinalProcessForBone()
{
  /* Items can't be removed during the final processing stage */

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->FinalProcessForBone();
}

/* VolumeModifier increases the spilled liquid's volume.
   Note that the original liquid isn't placed anywhere nor deleted,
   but its volume is decreased (possibly to zero). */

void stack::SpillFluid(character* Spiller, liquid* Liquid, long VolumeModifier)
{
  if(!Items)
    return;

  double ChanceMultiplier = 1. / (300 + sqrt(Volume));
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(int c = ItemVector.size() - 1; c >= 0; --c)
    if(ItemVector[c]->Exists() && ItemVector[c]->AllowFluids())
    {
      long ItemVolume = ItemVector[c]->GetVolume();
      double Root = sqrt(ItemVolume);

      if(Root > RAND() % 200 || Root > RAND() % 200)
      {
	long SpillVolume = long(VolumeModifier * Root * ChanceMultiplier);

	if(SpillVolume)
	{
	  Liquid->EditVolume(-Max(SpillVolume, Liquid->GetVolume()));
	  ItemVector[c]->SpillFluid(Spiller,
				    Liquid->SpawnMoreLiquid(SpillVolume),
				    ItemVector[c]->GetSquareIndex(GetPos()));

	  if(!Liquid->GetVolume())
	    return;
	}
      }
    }
}

void stack::AddItems(const itemvector& ItemVector)
{
  for(uint c = 0; c < ItemVector.size(); ++c)
    AddItem(ItemVector[c]);
}

void stack::MoveItemsTo(itemvector& ToVector, int RequiredSquarePosition)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetSquarePosition() == RequiredSquarePosition)
    {
      ItemVector[c]->RemoveFromSlot();
      ToVector.push_back(ItemVector[c]);
    }
}

void stack::DropSideItems()
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
  {
    int SquarePosition = i->GetSquarePosition();

    if(SquarePosition != CENTER)
    {
      if(i->IsAnimated())
      {
	lsquare* Square = GetLSquareTrulyUnder(SquarePosition);

	if(Square)
	  Square->DecStaticAnimatedEntities();

	GetLSquareUnder()->IncStaticAnimatedEntities();
      }

      i->SignalSquarePositionChange(CENTER);
      SignalEmitationDecrease(SquarePosition, i->GetEmitation());
      SignalEmitationIncrease(CENTER, i->GetEmitation());
    }
  }
}

truth stack::AllowDamage(int Direction, int SquarePosition)
{
  if(SquarePosition == CENTER)
    return true;

  switch(Direction)
  {
   case 0: return SquarePosition == DOWN || SquarePosition == RIGHT;
   case 1: return SquarePosition == DOWN;
   case 2: return SquarePosition == DOWN || SquarePosition == LEFT;
   case 3: return SquarePosition == RIGHT;
   case 4: return SquarePosition == LEFT;
   case 5: return SquarePosition == UP || SquarePosition == RIGHT;
   case 6: return SquarePosition == UP;
   case 7: return SquarePosition == UP || SquarePosition == LEFT;
  }

  return true;
}

long stack::GetWeight(ccharacter* Viewer, int SquarePosition) const
{
  long Weight = 0;

  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->GetSquarePosition() == SquarePosition
       && ((Flags & HIDDEN) || i->CanBeSeenBy(Viewer)))
      Weight += i->GetWeight();

  return Weight;
}

truth stack::DetectMaterial(cmaterial* Material) const
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    if(i->DetectMaterial(Material))
      return true;

  return false;
}

void stack::SetLifeExpectancy(int Base, int RandPlus)
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->SetLifeExpectancy(Base, RandPlus);
}

truth stack::Necromancy(character* Necromancer)
{
  itemvector ItemVector;
  FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->Necromancy(Necromancer))
      return true;

  return false;
}

void stack::CalculateEnchantments()
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->CalculateEnchantment();
}

ccharacter* stack::FindCarrier() const
{
  return MotherEntity ? MotherEntity->FindCarrier() : 0;
}

void stack::Haste()
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->Haste();
}

void stack::Slow()
{
  for(stackiterator i = GetBottom(); i.HasItem(); ++i)
    i->Slow();
}
