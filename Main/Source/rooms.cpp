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

/* Compiled through roomset.cpp */

void shop::Enter(character* Customer)
{
  if(Customer->IsPlayer())
    if(MasterIsActive())
    {
      if(GetMaster()->GetRelation(Customer) != HOSTILE
	 && Customer->CanBeSeenBy(GetMaster()))
	if(GetMaster()->CanBeSeenByPlayer())
	  ADD_MESSAGE("%s welcomes you warmly to the shop.",
		      GetMaster()->CHAR_NAME(DEFINITE));
	else
	  ADD_MESSAGE("Something welcomes you warmly to the shop.");
    }
    else
      ADD_MESSAGE("The shop appears to be deserted.");
}

/* item* ForSale can also be in chest or other container, so don't assume
   anything else in this function */

truth shop::PickupItem(character* Customer, item* ForSale, int Amount)
{
  if(!MasterIsActive() || Customer == GetMaster()
     || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  if(ForSale->IsLanternOnWall())
  {
    ADD_MESSAGE("\"I'd appreciate it if you left my "
		"light sources alone, thank you!\"");
    return false;
  }

  long Price = ForSale->GetTruePrice();

  if(Price)
  {
    Price = Amount * (Price * 100
		      / (100 + Customer->GetAttribute(CHARISMA)) + 1);

    if(GetMaster()->GetConfig() == NEW_ATTNAM)
      if(ForSale->IsBanana())
	Price = (Price >> 2) + 1;
      else if(ForSale->IsEatable(GetMaster()))
	Price <<= 2;
      else
	Price = 0;
  }

  if(!Customer->IsPlayer())
    if(Customer->CanBeSeenByPlayer() && Customer->GetMoney() >= Price)
    {
      if(Price)
      {
	ADD_MESSAGE("%s buys %s.", Customer->CHAR_NAME(DEFINITE),
		    ForSale->GetName(INDEFINITE, Amount).CStr());
	Customer->EditMoney(-Price);
	GetMaster()->EditMoney(Price);
	Customer->EditDealExperience(Price);
      }

      return true;
    }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
  {
    if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt()
       || ForSale->IsPetrussNut() || ForSale->IsTheAvatar()
       || ForSale->IsEncryptedScroll())
    {
      ADD_MESSAGE("\"I think it is yours. Take it.\"");
      return true;
    }

    if(!Price)
    {
      ADD_MESSAGE("\"Thank you for cleaning that junk out of my floor.\"");
      return true;
    }

    if(Customer->GetMoney() >= Price)
    {
      if(Amount == 1)
	ADD_MESSAGE("\"Ah! That %s costs %ld gold pieces. "
		    "No haggling, please.\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"Ah! Those %d %s cost %ld gold pieces. "
		    "No haggling, please.\"",
		    Amount, ForSale->CHAR_NAME(PLURAL), Price);

      if(game::TruthQuestion(CONST_S("Do you accept this deal? [y/N]")))
      {
	Customer->EditMoney(-Price);
	GetMaster()->EditMoney(+Price);
	Customer->EditDealExperience(Price);
	return true;
      }
      else
	return false;
    }
    else
    {
      if(Amount == 1)
	ADD_MESSAGE("\"Don't touch that %s, beggar! "
		    "It is worth %ld gold pieces!\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"Don't touch those %s, beggar! "
		    "They are worth %ld gold pieces!\"",
		    ForSale->CHAR_NAME(PLURAL), Price);

      return false;
    }
  }
  else
    if(game::TruthQuestion(CONST_S("Are you sure you want to "
				   "commit this thievery? [y/N]")))
    {
      Customer->Hostility(GetMaster());
      return true;
    }
    else
      return false;
}

truth shop::DropItem(character* Customer, item* ForSale, int Amount)
{
  if(!MasterIsActive() || Customer == GetMaster()
     || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  if(GetMaster()->GetConfig() == NEW_ATTNAM)
  {
    ADD_MESSAGE("\"Sorry, I'm only allowed to buy from "
		"Decos Bananas Co. if I wish to stay here.\"");
    return false;
  }

  long Price = ForSale->GetTruePrice() * Amount
	       * (100 + Customer->GetAttribute(CHARISMA)) / 400;

  if(!Customer->IsPlayer())
    if(Price && Customer->CanBeSeenByPlayer()
       && GetMaster()->GetMoney() >= Price)
    {
      ADD_MESSAGE("%s sells %s.", Customer->CHAR_NAME(DEFINITE),
		  ForSale->GetName(INDEFINITE, Amount).CStr());
      Customer->EditMoney(Price);
      GetMaster()->EditMoney(-Price);
      Customer->EditDealExperience(Price);
      return true;
    }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
  {
    if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt()
       || ForSale->IsPetrussNut() || ForSale->IsTheAvatar()
       || ForSale->IsEncryptedScroll())
    {
      ADD_MESSAGE("\"Oh no! You need it far more than I!\"");
      return false;
    }
    
    if(ForSale->WillExplodeSoon())
    {
      ADD_MESSAGE("\"Hey that %s is primed! Take it out! OUT, I SAY!\"", 
		  ForSale->CHAR_NAME(UNARTICLED));
      return false;
    }

    if(!Price)
    {
      ADD_MESSAGE("\"Hah! I wouldn't take %s even "
		  "if you paid me for it!\"",
		  Amount == 1 ? "that" : "those");
      return false;
    }

    if(GetMaster()->GetMoney())
    {
      if(GetMaster()->GetMoney() < Price)
	Price = GetMaster()->GetMoney();

      if(Amount == 1)
	ADD_MESSAGE("\"What a fine %s. I'll pay "
		    "%ld gold pieces for it.\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"What a fine pile of %d %s. I'll "
		    "pay %ld gold pieces for them.\"",
		    Amount, ForSale->CHAR_NAME(PLURAL), Price);

      if(game::TruthQuestion(CONST_S("Do you accept this deal? [y/N]")))
      {
	Customer->SetMoney(Customer->GetMoney() + Price);
	GetMaster()->SetMoney(GetMaster()->GetMoney() - Price);
	Customer->EditDealExperience(Price);
	return true;
      }
      else
	return false;
    }
    else
    {
      ADD_MESSAGE("\"I would pay you %ld gold pieces "
		  "for %s, but I'm temporarily "
		  "short of cash. Sorry.\"",
		  Price, Amount == 1 ? "it" : "them");
      return false;
    }
  }
  else
  {
    ADD_MESSAGE("The shopkeeper doesn't see you, "
		"so you cannot trade with him.");
    return game::TruthQuestion(CONST_S("Still drop ")
			       + (Amount == 1 ? "this item" : "these items")
			       + "? [y/N]");
  }
}

void shop::KickSquare(character* Infidel, lsquare* Square)
{
  if(!AllowKick(Infidel, Square))
  {
    ADD_MESSAGE("\"You infidel!\"");
    Infidel->Hostility(GetMaster());
  }
}

truth shop::ConsumeItem(character* Customer, item*, int)
{
  if(!MasterIsActive() || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  if(!Customer->IsPlayer())
    return false;

  if(Customer->CanBeSeenBy(GetMaster()))
  {
    ADD_MESSAGE("\"Buy that first, please.\"");
    return false;
  }
  else
    if(game::TruthQuestion(CONST_S("It's illegal to eat property "
				   "of others. Are you sure you sure? [y/N]")))
    {
      Customer->Hostility(GetMaster());
      return true;
    }
    else
      return false;
}

void cathedral::Enter(character* Visitor)
{
  if(Visitor->IsPlayer() && !Entered)
  {
    ADD_MESSAGE("The majestetic Cathedral of Valpurus looms "
		"before you. You watch it with utter respect.");
    Entered = true;
  }
}

/* Item can also be in a chest, so don't assume anything else... */

truth cathedral::PickupItem(character* Visitor, item* Item, int)
{
  if(game::GetStoryState() == 2
     || game::GetTeam(ATTNAM_TEAM)->GetRelation(Visitor->GetTeam()) == HOSTILE)
    return true;

  if(Visitor->IsPlayer())
  {
    if(Item->IsHeadOfElpuri() || Item->IsGoldenEagleShirt()
       || Item->IsPetrussNut() || !Item->GetTruePrice()
       || Item->IsEncryptedScroll())
      return true;

    ADD_MESSAGE("Picking up property of the Cathedral is prohibited.");

    if(game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
    {
      Visitor->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
      return true;
    }
  }

  return false;
}

truth cathedral::DropItem(character* Visitor, item* Item, int)
{
  if(game::GetStoryState() == 2
     || game::GetTeam(ATTNAM_TEAM)->GetRelation(Visitor->GetTeam()) == HOSTILE)
    return true;

  if(Visitor->IsPlayer())
  {
    if(Item->IsHeadOfElpuri() || Item->IsGoldenEagleShirt()
       || Item->IsPetrussNut() || Item->IsTheAvatar()
       || Item->IsEncryptedScroll())
    {
      ADD_MESSAGE("Donating this to the Cathedral wouldn't "
		  "be wise. You may still need it.");
      return false;
    }

    if(game::TruthQuestion(CONST_S("Do you wish to donate this "
				   "item to the Cathedral? [y/N]")))
      return true;
  }

  return false;
}

void cathedral::KickSquare(character* Kicker, lsquare* Square)
{
  if(!AllowKick(Kicker, Square)
     && Kicker->IsPlayer() && game::GetStoryState() != 2
     && game::GetTeam(ATTNAM_TEAM)->GetRelation(Kicker->GetTeam()) != HOSTILE)
  {
    ADD_MESSAGE("You have harmed the property of the Cathedral!");
    Kicker->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
  }
}

truth cathedral::ConsumeItem(character* HungryMan, item*, int)
{
  if(game::GetStoryState() == 2
     || (game::GetTeam(ATTNAM_TEAM)->GetRelation(HungryMan->GetTeam())
	 == HOSTILE))
    return true;

  if(HungryMan->IsPlayer())
  {
    ADD_MESSAGE("Eating the property of the Cathedral is forbidden.");

    if(game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
    {
      HungryMan->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
      return true;
    }
  }

  return false;
}

void cathedral::Save(outputfile& SaveFile) const
{
  room::Save(SaveFile);
  SaveFile << Entered;
}

void cathedral::Load(inputfile& SaveFile)
{
  room::Load(SaveFile);
  SaveFile >> Entered;
}

truth cathedral::Drink(character* Thirsty) const
{
  if(game::GetStoryState() == 2
     || game::GetTeam(ATTNAM_TEAM)->GetRelation(Thirsty->GetTeam()) == HOSTILE)
    return game::TruthQuestion(CONST_S("Do you want to drink? [y/N]"));

  if(Thirsty->IsPlayer())
  {
    ADD_MESSAGE("Drinking property of the Cathedral is prohibited.");

    if(game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
    {
      Thirsty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
      return true;
    }
  }

  return false;
}

void shop::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(Square->GetStack()->GetItems() && MasterIsActive()
     && Infidel && Infidel != GetMaster()
     && GetMaster()->GetRelation(Infidel) != HOSTILE
     && Square->CanBeSeenBy(GetMaster()))
  {
    ADD_MESSAGE("\"You infidel!\"");
    Infidel->Hostility(GetMaster());
  }
}

void cathedral::TeleportSquare(character* Teleporter, lsquare* Square)
{
  if(game::GetStoryState() == 2 || !Teleporter
     || (game::GetTeam(ATTNAM_TEAM)->GetRelation(Teleporter->GetTeam())
	 == HOSTILE))
    return;

  if(Teleporter->IsPlayer() && Square->GetStack()->GetItems())
  {
    ADD_MESSAGE("You have done unnatural things to "
		"the property of the Cathedral!");
    Teleporter->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
  }
}

truth cathedral::Dip(character* Thirsty) const
{
  if(game::GetStoryState() == 2
     || game::GetTeam(ATTNAM_TEAM)->GetRelation(Thirsty->GetTeam()) == HOSTILE)
    return true;

  if(Thirsty->IsPlayer())
  {
    /* What if it's not water? */

    ADD_MESSAGE("Stealing the precious water of the Cathedral is prohibited.");

    if(game::TruthQuestion(CONST_S("Are you sure you want to dip? [y/N]")))
    {
      Thirsty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
      return true;
    }
  }

  return false;
}

cathedral::cathedral()
{
  SetEntered(false);
}

void library::Enter(character* Customer)
{
  if(Customer->IsPlayer())
    if(MasterIsActive())
    {
      if(GetMaster()->GetRelation(Customer) != HOSTILE
	 && Customer->CanBeSeenBy(GetMaster()))
	if(GetMaster()->CanBeSeenByPlayer())
	  ADD_MESSAGE("%s looks at you suspiciously. "
		      "\"Feel free to open the shelves, "
		      "but be quiet in the library!\" %s whispers.",
		      GetMaster()->CHAR_NAME(DEFINITE),
		      GetMaster()->GetPersonalPronoun().CStr());
	else
	  ADD_MESSAGE("You feel somebody staring at you.");
    }
    else
      ADD_MESSAGE("The library appears to be deserted.");
}

truth library::PickupItem(character* Customer, item* ForSale, int Amount)
{
  if(!MasterIsActive() || Customer == GetMaster()
     || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  if(ForSale->IsLanternOnWall())
  {
    ADD_MESSAGE("\"I'd appreciate it if you left my "
		"light sources alone, thank you!\"");
    return false;
  }

  long Price = ForSale->GetTruePrice() * Amount
	       * 100 / (100 + Customer->GetAttribute(CHARISMA));

  if(!Customer->IsPlayer())
  {
    if(Customer->CanBeSeenByPlayer() && Customer->GetMoney() >= Price)
    {
      ADD_MESSAGE("%s buys %s.", Customer->CHAR_NAME(DEFINITE),
		  ForSale->GetName(INDEFINITE, Amount).CStr());
      Customer->EditMoney(-Price);
      GetMaster()->EditMoney(Price);
      Customer->EditDealExperience(Price);
      return true;
    }
    else
      return false;
  }

  if(Customer->CanBeSeenBy(GetMaster()))
  {
    if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt()
       || ForSale->IsPetrussNut() || ForSale->IsTheAvatar()
       || ForSale->IsEncryptedScroll())
    {
      ADD_MESSAGE("\"I think it is yours. Take it.\"");
      return true;
    }

    if(!Price || !ForSale->CanBeSoldInLibrary(GetMaster()))
    {
      ADD_MESSAGE("\"Thank you for cleaning that junk out of my floor.\"");
      return true;
    }

    if(Customer->GetMoney() >= Price)
    {
      if(Amount == 1)
	ADD_MESSAGE("\"Ah! That %s costs %ld gold "
		    "pieces. No haggling, please.\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"Ah! Those %d %s cost %ld gold "
		    "pieces. No haggling, please.\"",
		    Amount, ForSale->CHAR_NAME(PLURAL), Price);

      if(game::TruthQuestion(CONST_S("Do you accept this deal? [y/N]")))
      {
	Customer->EditMoney(-Price);
	GetMaster()->EditMoney(Price);
	Customer->EditDealExperience(Price);
	return true;
      }
      else
	return false;
    }
    else
    {
      if(Amount == 1)
	ADD_MESSAGE("\"Don't touch that %s, beggar! "
		    "It is worth %ld gold pieces!\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"Don't touch those %s, beggar! "
		    "They are worth %ld gold pieces!\"",
		    ForSale->CHAR_NAME(PLURAL), Price);

      return false;
    }
  }
  else
    if(game::TruthQuestion(CONST_S("Are you sure you want to "
				   "commit this thievery? [y/N]")))
    {
      Customer->Hostility(GetMaster());
      return true;
    }
    else
      return false;
}

truth library::DropItem(character* Customer, item* ForSale, int Amount)
{
  if(!MasterIsActive() || Customer == GetMaster()
     || GetMaster()->GetRelation(Customer) == HOSTILE)
    return true;

  long Price = ForSale->GetTruePrice() * Amount
	       * (100 + Customer->GetAttribute(CHARISMA)) / 400;

  if(!Customer->IsPlayer())
    if(Price && Customer->CanBeSeenByPlayer()
       && GetMaster()->GetMoney() >= Price)
    {
      ADD_MESSAGE("%s sells %s.", Customer->CHAR_NAME(DEFINITE),
		  ForSale->GetName(INDEFINITE, Amount).CStr());
      Customer->SetMoney(Customer->GetMoney() + Price);
      GetMaster()->SetMoney(GetMaster()->GetMoney() - Price);
      Customer->EditDealExperience(Price);
      return true;
    }
    else
      return false;

  if(Customer->CanBeSeenBy(GetMaster()))
  {
    if(ForSale->IsHeadOfElpuri() || ForSale->IsGoldenEagleShirt()
       || ForSale->IsPetrussNut() || ForSale->IsTheAvatar()
       || ForSale->IsEncryptedScroll())
    {
      ADD_MESSAGE("\"Oh no! You need it far more than I!\"");
      return false;
    }

    if(!Price || !ForSale->CanBeSoldInLibrary(GetMaster()))
    {
      ADD_MESSAGE("\"Sorry, but I don't think %s into my collection.\"",
		  Amount == 1 ? "that fits" : "those fit");
      return false;
    }

    if(GetMaster()->GetMoney())
    {
      if(GetMaster()->GetMoney() < Price)
	Price = GetMaster()->GetMoney();

      if(Amount == 1)
	ADD_MESSAGE("\"What an interesting %s. I'll "
		    "pay %ld gold pieces for it.\"",
		    ForSale->CHAR_NAME(UNARTICLED), Price);
      else
	ADD_MESSAGE("\"What an interesting collection of %d "
		    "%s. I'll pay %ld gold pieces for it.\"",
		    Amount, ForSale->CHAR_NAME(PLURAL), Price);

      if(game::TruthQuestion(CONST_S("Do you want to sell ")
			     + (Amount == 1 ? "this item" : "these items")
			     + "? [y/N]"))
      {
	Customer->EditMoney(Price);
	GetMaster()->EditMoney(-Price);
	Customer->EditDealExperience(Price);
	return true;
      }
      else
	return false;
    }
    else
    {
      ADD_MESSAGE("\"I would pay you %ld gold pieces for %s, "
		  "but I'm temporarily short of cash. Sorry.\"",
		  Price, Amount == 1 ? "it" : "them");
      return false;
    }
  }
  else
  {
    ADD_MESSAGE("The librarian doesn't see you, "
		"so you cannot trade with him.");
    return game::TruthQuestion(CONST_S("Still drop ")
			       + (Amount == 1 ? "this item" : "these items")
			       + "? [y/N]");
  }
}

void library::KickSquare(character* Infidel, lsquare* Square)
{
  if(!AllowKick(Infidel, Square))
  {
    ADD_MESSAGE("\"You book vandal!\"");
    Infidel->Hostility(GetMaster());
  }
}

truth library::ConsumeItem(character*, item*, int)
{
  return true;
}

void library::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(Square->GetStack()->GetItems() && MasterIsActive()
     && Infidel && Infidel != GetMaster()
     && GetMaster()->GetRelation(Infidel) != HOSTILE
     && Square->CanBeSeenBy(GetMaster()))
  {
    ADD_MESSAGE("\"You book hater!\"");
    Infidel->Hostility(GetMaster());
  }
}

truth bananadroparea::PickupItem(character* Hungry, item* Item, int)
{
  if(game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Hungry->GetTeam()) == HOSTILE)
    return true;

  if(Hungry->IsPlayer())
  {
    if(!Item->IsBanana() && !Item->IsLanternOnWall())
      return true;

    ADD_MESSAGE("That would be stealing.");

    if(game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
    {
      Hungry->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
      return true;
    }
  }

  return false;
}

truth bananadroparea::DropItem(character* Dropper, item* Item, int)
{
  return (game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Dropper->GetTeam())
	  == HOSTILE
	  || (Dropper->IsPlayer()
	      && ((!Item->IsBanana() && !Item->IsLanternOnWall())
		  || game::TruthQuestion(CONST_S("Do you wish to "
						 "donate this item "
						 "to the town? [y/N]")))));
}

void bananadroparea::KickSquare(character* Kicker, lsquare* Square)
{
  if(AllowKick(Kicker, Square) && Kicker->IsPlayer()
     && game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Kicker->GetTeam())
     != HOSTILE)
  {
    for(stackiterator i = Square->GetStack()->GetBottom(); i.HasItem(); ++i)
      if(i->IsBanana() || i->IsLanternOnWall())
      {
	ADD_MESSAGE("You have harmed the property of the town!");
	Kicker->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
	return;
      }
  }
}

truth bananadroparea::ConsumeItem(character* HungryMan, item* Item, int)
{
  if(game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(HungryMan->GetTeam())
     == HOSTILE)
    return true;

  if(HungryMan->IsPlayer())
  {
    if(!Item->IsBanana() && !Item->IsLanternOnWall())
      return true;

    ADD_MESSAGE("Eating this is forbidden.");

    if(game::TruthQuestion(CONST_S("Do you still want to do this? [y/N]")))
    {
      HungryMan->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
      return true;
    }
  }

  return HungryMan->IsSumoWrestler();
}

void bananadroparea::TeleportSquare(character* Infidel, lsquare* Square)
{
  if(!Infidel
     || game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Infidel->GetTeam())
     == HOSTILE)
    return;

  for(stackiterator i = Square->GetStack()->GetBottom(); i.HasItem(); ++i)
    if(i->IsBanana() || i->IsLanternOnWall())
    {
      Infidel->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
      return;
    }
}

truth shop::AllowSpoil(citem* Item) const
{
  character* Master = GetMaster();
  return !Master || !Master->IsEnabled() || !Item->HasPrice();
}

/* Gum solution */

truth shop::AllowKick(ccharacter* Char, const lsquare* LSquare) const
{
  return (!LSquare->GetStack()->GetItems() || !MasterIsActive()
	  || Char == GetMaster() || GetMaster()->GetRelation(Char) == HOSTILE
	  || !LSquare->CanBeSeenBy(GetMaster()));
}

truth cathedral::AllowKick(ccharacter* Char, const lsquare* LSquare) const
{
  return (game::GetTeam(ATTNAM_TEAM)->GetRelation(Char->GetTeam()) == HOSTILE
	  || !LSquare->GetStack()->GetItems());
}

truth library::AllowKick(ccharacter* Char, const lsquare* LSquare) const
{
  return (!LSquare->GetStack()->GetItems()
	  || !MasterIsActive() || Char == GetMaster()
	  || GetMaster()->GetRelation(Char) == HOSTILE
	  || LSquare->CanBeSeenBy(GetMaster()));
}

truth bananadroparea::AllowKick(ccharacter* Char, const lsquare*) const
{
  return (!Char->IsPlayer()
	  || (game::GetTeam(NEW_ATTNAM_TEAM)->GetRelation(Char->GetTeam())
	      == HOSTILE));
}

void shop::HostileAction(character* Guilty) const
{
  if(MasterIsActive() && Guilty && Guilty != GetMaster()
     && GetMaster()->GetRelation(Guilty) != HOSTILE
     && Guilty->CanBeSeenBy(GetMaster()))
  {
    ADD_MESSAGE("\"You infidel!\"");
    Guilty->Hostility(GetMaster());
  }
}

void cathedral::HostileAction(character* Guilty) const
{
  if(game::GetStoryState() != 2 && Guilty)
    Guilty->GetTeam()->Hostility(game::GetTeam(ATTNAM_TEAM));
}

void library::HostileAction(character* Guilty) const
{
  if(MasterIsActive() && Guilty && Guilty != GetMaster()
     && GetMaster()->GetRelation(Guilty) != HOSTILE
     && Guilty->CanBeSeenBy(GetMaster()))
  {
    ADD_MESSAGE("\"You infidel!\"");
    Guilty->Hostility(GetMaster());
  }
}

void bananadroparea::HostileAction(character* Guilty) const
{
  if(Guilty)
    Guilty->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
}

void sumoarena::DestroyTerrain(character* Who)
{
  if(Who)
    Who->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
}

void sumoarena::HostileAction(character* Guilty) const
{
  if(Guilty)
    Guilty->GetTeam()->Hostility(game::GetTeam(NEW_ATTNAM_TEAM));
}

truth sumoarena::CheckDestroyTerrain(character* Infidel)
{
  if(Infidel->GetTeam()->GetRelation(game::GetTeam(NEW_ATTNAM_TEAM))
     == HOSTILE)
    return true;

  ADD_MESSAGE("The residents of New Attnam might not like this.");

  if(game::TruthQuestion(CONST_S("Are you sure you want to do this? [y/N]")))
  {
    DestroyTerrain(Infidel);
    return true;
  }
  else
    return false;
}

void shop::ReceiveVomit(character* Who)
{
  if(MasterIsActive()
     && Who->IsPlayer()
     && Who->GetRelation(GetMaster()) != HOSTILE
     && Who->CanBeSeenBy(GetMaster()))
    ADD_MESSAGE("\"Unfortunately I accept no returns.\"");
}

void cathedral::AddItemEffect(item* Dropped)
{

  truth SeenBeforeTeleport = Dropped->CanBeSeenByPlayer();
  character* KamikazeDwarf = FindRandomExplosiveReceiver();

  if(!Dropped->IsKamikazeWeapon(KamikazeDwarf))
    return;

  if(KamikazeDwarf)
  {
    Dropped->MoveTo(KamikazeDwarf->GetStack());

    if(KamikazeDwarf->CanBeSeenByPlayer())
    {
      if(SeenBeforeTeleport)
	ADD_MESSAGE("%s disappears and reappears in %s's inventory.",
		    Dropped->GetName(DEFINITE).CStr(),
		    KamikazeDwarf->GetName(DEFINITE).CStr());
      else
	ADD_MESSAGE("%s appears in %s's inventory.",
		    Dropped->GetName(DEFINITE).CStr(),
		    KamikazeDwarf->GetName(DEFINITE).CStr());
    }
    else if(SeenBeforeTeleport)
      ADD_MESSAGE("%s disappears.", Dropped->GetName(DEFINITE).CStr());
  }
  else
  {
    /* position is in kamikaze dwarf room */

    Dropped->RemoveFromSlot();
    game::GetCurrentLevel()->GetLSquare(18,21)
      ->GetStack()->AddItem(Dropped, false);

    if(Dropped->CanBeSeenByPlayer())
    {
      if(SeenBeforeTeleport)
	ADD_MESSAGE("%s disappears and reappears in the kamikaze dwarf room.",
		    Dropped->GetName(DEFINITE).CStr());
      else
	ADD_MESSAGE("%s appears in the kamikaze dwarf room.",
		    Dropped->GetName(DEFINITE).CStr());
    }
    else if(SeenBeforeTeleport)
      ADD_MESSAGE("%s disappears.", Dropped->GetNameSingular().CStr());
  }
}

character* cathedral::FindRandomExplosiveReceiver() const
{
  std::vector<character*> ListOfDwarfs;

  for(std::list<character*>::const_iterator i
	= game::GetTeam(ATTNAM_TEAM)->GetMember().begin();
      i != game::GetTeam(ATTNAM_TEAM)->GetMember().end(); ++i)
    if((*i)->IsEnabled() && (*i)->IsKamikazeDwarf())
      ListOfDwarfs.push_back(*i);

  if(ListOfDwarfs.empty())
    return 0;
  else
    return ListOfDwarfs[RAND_N(ListOfDwarfs.size())];
}
