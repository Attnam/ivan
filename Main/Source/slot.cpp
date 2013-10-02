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

void slot::Save(outputfile& SaveFile) const
{
  SaveFile << Item;
}

void slot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;

  if(Item)
    Item->SetMainSlot(this);
}

void stackslot::Load(inputfile& SaveFile)
{
  SaveFile >> Item;

  if(Item)
    Item->SignalStackAdd(this, &stack::AddElement);
}

void stackslot::Empty()
{
  GetMotherStack()->RemoveItem(this);
}

void bodypartslot::Empty()
{
  col24 Emitation = Item->GetEmitation();
  static_cast<bodypart*>(Item)->SetMaster(0);
  Item = 0;
  GetMaster()->CalculateEquipmentState();
  SignalVolumeAndWeightChange();
  SignalEmitationDecrease(Emitation);

  if(!GetMaster()->IsInitializing())
  {
    GetMaster()->CalculateHP();
    GetMaster()->CalculateMaxHP();
  }
}

void gearslot::Empty()
{
  citem* Old = Item;
  Item = 0;
  col24 Emitation = Old->GetEmitation();
  SignalVolumeAndWeightChange();
  GetBodyPart()->SignalEquipmentRemoval(this, Old);
  SignalEmitationDecrease(Emitation);
}

void gearslot::Init(bodypart* BodyPart, int I)
{
  SetBodyPart(BodyPart);
  SetEquipmentIndex(I);
}

void stackslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();
  GetMotherStack()->AddItem(Item);
}

void bodypartslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::IsInWilderness())
    GetMaster()->GetStackUnder()->AddItem(Item);
  else
    GetMaster()->GetStack()->AddItem(Item);
}

void gearslot::AddFriendItem(item* Item) const
{
  Item->RemoveFromSlot();

  if(!game::IsInWilderness())
    GetBodyPart()->GetLSquareUnder()->AddItem(Item);
  else
    GetBodyPart()->GetMaster()->GetStack()->AddItem(Item);
}

truth stackslot::IsOnGround() const
{
  return GetMotherStack()->IsOnGround();
}

void stackslot::SignalVolumeAndWeightChange()
{
  GetMotherStack()->SignalVolumeAndWeightChange();
}

void bodypartslot::SignalVolumeAndWeightChange()
{
  GetMaster()->SignalVolumeAndWeightChange();
  GetMaster()->SignalBodyPartVolumeAndWeightChange();
}

void gearslot::SignalVolumeAndWeightChange()
{
  GetBodyPart()->SignalVolumeAndWeightChange();
}

void stackslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
  {
    Item->SignalStackAdd(this, &stack::AddItem);
    SignalVolumeAndWeightChange();
    SignalEmitationIncrease(Item->GetEmitation());
  }
}

void bodypartslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
  {
    Item->SetMainSlot(this);
    static_cast<bodypart*>(Item)->SetMaster(GetMaster());

    if(!GetMaster()->IsInitializing())
    {
      SignalVolumeAndWeightChange();
      SignalEmitationIncrease(Item->GetEmitation());
      static_cast<bodypart*>(Item)->CalculateMaxHP(0);
      GetMaster()->CalculateHP();
      GetMaster()->CalculateMaxHP();
    }
  }
}

void gearslot::PutInItem(item* What)
{
  Item = What;

  if(Item)
  {
    Item->SetMainSlot(this);
    GetBodyPart()->SignalEquipmentAdd(this);
    SignalVolumeAndWeightChange();
    SignalEmitationIncrease(Item->GetEmitation());
  }
}

square* stackslot::GetSquareUnder(int) const
{
  return GetMotherStack()->GetSquareUnder();
}

square* bodypartslot::GetSquareUnder(int I) const
{
  return GetMaster()->GetSquareUnder(I);
}

square* gearslot::GetSquareUnder(int) const
{
  return GetBodyPart()->GetSquareUnder();
}

void stackslot::SignalEmitationIncrease(col24 Emitation)
{
  GetMotherStack()->SignalEmitationIncrease(Item->GetSquarePosition(), Emitation);
}

void bodypartslot::SignalEmitationIncrease(col24 Emitation)
{
  GetMaster()->SignalEmitationIncrease(Emitation);
}

void gearslot::SignalEmitationIncrease(col24 Emitation)
{
  GetBodyPart()->SignalEmitationIncrease(Emitation);
}

void stackslot::SignalEmitationDecrease(col24 Emitation)
{
  GetMotherStack()->SignalEmitationDecrease(Item->GetSquarePosition(), Emitation);
}

void bodypartslot::SignalEmitationDecrease(col24 Emitation)
{
  GetMaster()->SignalEmitationDecrease(Emitation);
}

void gearslot::SignalEmitationDecrease(col24 Emitation)
{
  GetBodyPart()->SignalEmitationDecrease(Emitation);
}

void bodypartslot::Load(inputfile& SaveFile)
{
  slot::Load(SaveFile);

  if(Item)
    static_cast<bodypart*>(Item)->SetMaster(GetMaster());
}

void slot::DonateTo(item* Item)
{
  Empty();
  PutInItem(Item);
}

void stackslot::DonateTo(item* Item) // could be optimized
{
  AddFriendItem(Item);
  Empty();
}

truth stackslot::CanBeSeenBy(ccharacter* Viewer) const
{
  return GetMotherStack()->CanBeSeenBy(Viewer, Item->GetSquarePosition());
}

truth bodypartslot::CanBeSeenBy(ccharacter* Viewer) const
{
  return GetMaster()->CanBeSeenBy(Viewer);
}

truth gearslot::CanBeSeenBy(ccharacter* Viewer) const
{
  return GetBodyPart()->CanBeSeenBy(Viewer);
}

void gearslot::SignalEnchantmentChange()
{
  GetBodyPart()->SignalEnchantmentChange();
}

truth stackslot::IsVisible() const
{
  return GetMotherStack()->IsVisible();
}

ccharacter* stackslot::FindCarrier() const
{
  return GetMotherStack()->FindCarrier();
}

ccharacter* gearslot::FindCarrier() const
{
  return GetBodyPart()->FindCarrier();
}
