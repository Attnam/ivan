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

/* Compiled through materset.cpp */

/* Used to determine how pixels are distributed when fluid over bodyarmors
   is shown */

const long fluid::BodyArmorPartPixels[] = { 34, 7, 7, 8, 6, 6 };

fluid::fluid() : entity(HAS_BE), Next(0), MotherItem(0), GearImage(0) { }

fluid::fluid(liquid* Liquid, lsquare* LSquareUnder) : entity(HAS_BE), Next(0), Liquid(Liquid), LSquareUnder(LSquareUnder), MotherItem(0), Image(false), GearImage(0), Flags(0)
{
  TrapData.TrapID = game::CreateNewTrapID(this);
  TrapData.VictimID = 0;
  Image.ShadowPos = ZERO_V2;
  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  AddLiquid(Liquid->GetVolume());
}

fluid::fluid(liquid* Liquid, item* MotherItem, cfestring& LocationName, truth IsInside) : entity(HAS_BE), Next(0), Liquid(Liquid), LSquareUnder(0), MotherItem(MotherItem), Image(false), GearImage(0), Flags(0), LocationName(LocationName)
{
  TrapData.TrapID = 0;

  if(UseImage())
  {
    Image.Picture->InitRandMap();
    Image.Picture->InitPriorityMap(AVERAGE_PRIORITY);
    Image.ShadowPos = MotherItem->GetBitmapPos(0);
    Image.SpecialFlags = MotherItem->GetSpecialFlags();
  }

  if(IsInside)
    Flags |= FLUID_INSIDE;

  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  AddLiquid(Liquid->GetVolume());
}

fluid::~fluid()
{
  game::RemoveTrapID(TrapData.TrapID);
  delete Liquid;
  delete [] GearImage;
}

void fluid::AddLiquid(long Volume)
{
  long Pixels = Volume >> 2;

  if(Pixels && UseImage())
  {
    col16 Col = Liquid->GetColor();

    if(MotherItem)
    {
      pixelpredicate Pred = MotherItem->GetFluidPixelAllowedPredicate();
      Image.AddLiquidToPicture(MotherItem->GetRawPicture(), Pixels, 225, Col, Pred);

      if(GearImage)
	if(Flags & HAS_BODY_ARMOR_PICTURES)
	  for(int c = 0; c < BODY_ARMOR_PARTS; ++c)
	    GearImage[c].AddLiquidToPicture(igraph::GetHumanoidRawGraphic(), Pixels * BodyArmorPartPixels[c] / HUMAN_BODY_ARMOR_PIXELS, Image.AlphaAverage, Col, Pred);
	else
	  GearImage->AddLiquidToPicture(igraph::GetHumanoidRawGraphic(), Pixels, Image.AlphaAverage, Col, Pred);
    }
    else
      Image.AddLiquidToPicture(0, Pixels, 225, Col, 0);
  }
}

void fluid::AddLiquidAndVolume(long Volume)
{
  AddLiquid(Volume);
  Liquid->SetVolumeNoSignals(Liquid->GetVolume() + Volume);
}

void fluid::Be()
{
  long Rand = RAND();

  if(!(Rand & 7))
    if(MotherItem)
    {
      if(MotherItem->Exists() && MotherItem->AllowFluidBe())
	Liquid->TouchEffect(MotherItem, LocationName);
    }
    else
    {
      Liquid->TouchEffect(LSquareUnder->GetGLTerrain());

      if(LSquareUnder->GetOLTerrain())
	Liquid->TouchEffect(LSquareUnder->GetOLTerrain());

      if(LSquareUnder->GetCharacter())
	LSquareUnder->GetCharacter()->StayOn(Liquid);
    }

  if(MotherItem ? !(Rand & 15) && MotherItem->Exists() && MotherItem->AllowFluidBe() : !(Rand & 63))
  {
    long OldVolume = Liquid->GetVolume();
    long NewVolume = ((OldVolume << 6) - OldVolume) >> 6;
    Liquid->SetVolumeNoSignals(NewVolume);

    if(UseImage())
      while(NewVolume < Image.AlphaSum >> 6 && FadePictures());

    if(!MotherItem)
    {
      LSquareUnder->SendNewDrawRequest();
      LSquareUnder->SendMemorizedUpdateRequest();
    }

    if(!Liquid->GetVolume())
      Destroy();
  }
}

void fluid::Save(outputfile& SaveFile) const
{
  SaveFile << TrapData << Liquid;
  SaveFile << LocationName << Flags;
  Image.Save(SaveFile);

  if(GearImage)
  {
    SaveFile.Put(true);
    int Images = Flags & HAS_BODY_ARMOR_PICTURES ? BODY_ARMOR_PARTS : 1;

    for(int c = 0; c < Images; ++c)
      GearImage[c].Save(SaveFile);
  }
  else
    SaveFile.Put(false);
}

void fluid::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  SaveFile >> TrapData;
  game::AddTrapID(this, TrapData.TrapID);
  Liquid = static_cast<liquid*>(ReadType<material*>(SaveFile));
  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  SaveFile >> LocationName >> Flags;
  Image.Load(SaveFile);

  if(SaveFile.Get())
  {
    int Images = Flags & HAS_BODY_ARMOR_PICTURES ? BODY_ARMOR_PARTS : 1;
    GearImage = new imagedata[Images];

    for(int c = 0; c < Images; ++c)
    {
      GearImage[c].Load(SaveFile);
      GearImage[c].Picture->InitRandMap();
      GearImage[c].Picture->CalculateRandMap();
    }
  }
}

void fluid::SimpleDraw(blitdata& BlitData) const
{
  Image.Picture->AlphaLuminanceBlit(BlitData);
}

void fluid::Draw(blitdata& BlitData) const
{
  if(!UseImage())
    return;

  bitmap* Picture = Image.Picture;
  int SpecialFlags = MotherItem ? MotherItem->GetSpecialFlags() : 0;

  if(SpecialFlags & 0x7)
  {
    /* Priority Bug!!! */
    Picture->BlitAndCopyAlpha(igraph::GetFlagBuffer(), SpecialFlags);
    igraph::GetFlagBuffer()->AlphaLuminanceBlit(BlitData);
  }
  else
    Picture->AlphaPriorityBlit(BlitData);

  if(MotherItem && BlitData.CustomData & ALLOW_ANIMATE)
    Image.Animate(BlitData, SpecialFlags);
}

outputfile& operator<<(outputfile& SaveFile, const fluid* Fluid)
{
  Fluid->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, fluid*& Fluid)
{
  Fluid = new fluid;
  Fluid->Load(SaveFile);
  return SaveFile;
}

/* If fluid has decreased, fade, otherwise add new pixels */

void fluid::SignalVolumeAndWeightChange()
{
  long Volume = Liquid->GetVolume();

  if(UseImage())
    if(Volume < Image.AlphaSum >> 6)
      while(FadePictures() && Volume < Image.AlphaSum >> 6);
    else
      AddLiquid(Volume - (Image.AlphaSum >> 6));
}

truth fluid::FadePictures()
{
  truth Change = Image.Fade();

  if(GearImage)
  {
    int Images = Flags & HAS_BODY_ARMOR_PICTURES ? BODY_ARMOR_PARTS : 1;

    for(int c = 0; c < Images; ++c)
      GearImage[c].Fade();
  }

  return Change;
}

/* Used by lookmode, item descriptions etc. If there are 1-3 members in
   Fluid, list them all, otherwise say "a lot of liquids". If there are
   several types of blood in the list, they are counted as one. */

void fluid::AddFluidInfo(const fluid* Fluid, festring& String)
{
  liquid* LiquidStack[4];
  liquid** Show = LiquidStack + 1;
  int Index = 0;
  truth Blood = false, OneBlood = true;

  for(; Fluid; Fluid = Fluid->Next)
  {
    liquid* Liquid = Fluid->GetLiquid();
    truth LiquidBlood = Liquid->GetCategoryFlags() & IS_BLOOD;

    if(!LiquidBlood || !Blood)
    {
      if(Index < 3)
      {
	if(LiquidBlood)
	{
	  --Show;
	  Show[0] = Liquid;
	  ++Index;
	}
	else
	  Show[Index++] = Liquid;
      }
      else
      {
	++Index;
	break;
      }
    }

    if(LiquidBlood)
    {
      if(Blood)
	OneBlood = false;
      else
	Blood = true;
    }
  }

  if(Index <= 3)
  {
    if(!Blood || OneBlood)
      String << Show[0]->GetName(false, false);
    else
      String << "different types of blood";

    if(Index == 2)
      String << " and " << Show[1]->GetName(false, false);
    else if(Index == 3)
      String << ", " << Show[1]->GetName(false, false) << " and " << Show[2]->GetName(false, false);
  }
  else
    String << "a lot of liquids";
}

/* Used only when loading fluids. Correcting RandMap here is somewhat a gum
   solution. */

void fluid::SetMotherItem(item* What)
{
  MotherItem = What;

  if(UseImage())
  {
    Image.Picture->InitRandMap();
    Image.Picture->CalculateRandMap();
  }
}

/* Ensures the gear pictures are correct after this. ShadowPos is the armor's
   or weapon's BitmapPos in humanoid.pcx. SpecialFlags should include drawing
   information of the bodypart in question (ST_RIGHT_ARM etc). BodyArmor should
   be true iff the picture is part of a body armor, for instance armor covering
   one's shoulder. */

void fluid::CheckGearPicture(v2 ShadowPos, int SpecialFlags, truth BodyArmor)
{
  if(!UseImage())
    return;

  if(BodyArmor && !(Flags & HAS_BODY_ARMOR_PICTURES))
  {
    Flags |= HAS_BODY_ARMOR_PICTURES;
    delete [] GearImage;
    GearImage = 0;
  }
  else if(!BodyArmor && Flags & HAS_BODY_ARMOR_PICTURES)
  {
    Flags &= ~HAS_BODY_ARMOR_PICTURES;
    delete [] GearImage;
    GearImage = 0;
  }

  imagedata* ImagePtr;
  long Pixels;

  if(BodyArmor)
  {
    int Index = (SpecialFlags & 0x38) >> 3;

    if(Index >= BODY_ARMOR_PARTS)
      Index = 0;

    if(GearImage)
      if(GearImage[Index].ShadowPos != ShadowPos)
	GearImage[Index].Clear(false);
      else
	return; // the picture already exists and is correct
    else
    {
      GearImage = new imagedata[BODY_ARMOR_PARTS];

      for(int c = 0; c < BODY_ARMOR_PARTS; ++c)
	new(&GearImage[c]) imagedata(false);
    }

    ImagePtr = &GearImage[Index];
    Pixels = (Image.AlphaSum * BodyArmorPartPixels[Index] / HUMAN_BODY_ARMOR_PIXELS) >> 10;
  }
  else
  {
    if(GearImage)
      if(GearImage->ShadowPos != ShadowPos)
	GearImage->Clear(false);
      else
	return; // the picture already exists and is correct
    else
    {
      GearImage = new imagedata[1];
      new(GearImage) imagedata(false);
    }

    ImagePtr = GearImage;
    Pixels = Image.AlphaSum >> 10;
  }

  ImagePtr->ShadowPos = ShadowPos;
  ImagePtr->SpecialFlags = SpecialFlags;
  ImagePtr->Picture->InitRandMap();
  ImagePtr->Picture->InitPriorityMap(AVERAGE_PRIORITY);

  if(Pixels)
    ImagePtr->AddLiquidToPicture(igraph::GetHumanoidRawGraphic(),
				 Pixels,
				 Image.AlphaAverage,
				 Liquid->GetColor(),
				 MotherItem->GetFluidPixelAllowedPredicate());
}

void fluid::DrawGearPicture(blitdata& BlitData, int SpecialFlags) const
{
  if(!UseImage())
    return;

  bitmap* Picture = GearImage->Picture;

  if(SpecialFlags & 0x7)
  {
    Picture->BlitAndCopyAlpha(igraph::GetFlagBuffer(), SpecialFlags);
    igraph::GetFlagBuffer()->AlphaPriorityBlit(BlitData);
  }
  else
    GearImage->Picture->AlphaPriorityBlit(BlitData);

  if(BlitData.CustomData & ALLOW_ANIMATE)
    GearImage->Animate(BlitData, SpecialFlags);
}

void fluid::DrawBodyArmorPicture(blitdata& BlitData, int SpecialFlags) const
{
  if(!UseImage())
    return;

  /* We suppose body armor pictures are never rotated */

  int Index = (SpecialFlags & 0x38) >> 3;

  if(Index >= BODY_ARMOR_PARTS)
    Index = 0;

  GearImage[Index].Picture->AlphaPriorityBlit(BlitData);

  if(BlitData.CustomData & ALLOW_ANIMATE)
    GearImage[Index].Animate(BlitData, 0);
}

truth fluid::imagedata::Fade()
{
  return ShadowPos != ERROR_V2 ? Picture->Fade(AlphaSum, AlphaAverage, 1) : false;
}

/* Let two pixels fall every now and then over the picture. CurrentFlags
   should include rotation info. */

void fluid::imagedata::Animate(blitdata& BlitData, int CurrentFlags) const
{
  if(!AlphaSum)
    return;

  if(!DripTimer)
  {
    DripPos = Picture->RandomizePixel();

    /* DripPos != ERROR_V2 since AlphaSum != 0 */

    if(DripPos.Y <= 12)
    {
      DripColor = Picture->GetPixel(DripPos);
      DripAlpha = Picture->GetAlpha(DripPos);
    }
    else
      ++DripTimer;
  }

  if(DripTimer <= 0)
  {
    v2 TrueDripPos = DripPos;
    Rotate(TrueDripPos, 16, CurrentFlags);
    TrueDripPos.Y -= DripTimer;

    if(TrueDripPos.Y < 16)
      BlitData.Bitmap->AlphaPutPixel(TrueDripPos + BlitData.Dest, DripColor, BlitData.Luminance, DripAlpha);

    if(TrueDripPos.Y < 15)
    {
      ++TrueDripPos.Y;
      BlitData.Bitmap->AlphaPutPixel(TrueDripPos + BlitData.Dest, DripColor, BlitData.Luminance, DripAlpha);
    }
    else
      DripTimer = Min<long>(RAND() % (500000 / AlphaSum), 25000);
  }

  --DripTimer;
}

fluid::imagedata::imagedata(truth Load) : Picture(0), DripTimer(0), AlphaSum(0), ShadowPos(ERROR_V2)
{
  if(!Load)
  {
    Picture = new bitmap(TILE_V2, TRANSPARENT_COLOR);
    Picture->ActivateFastFlag();
    Picture->CreateAlphaMap(0);
  }
}

fluid::imagedata::~imagedata()
{
  delete Picture;
}

void fluid::imagedata::Save(outputfile& SaveFile) const
{
  SaveFile << Picture << AlphaSum << ShadowPos << (int)SpecialFlags;
}

void fluid::imagedata::Load(inputfile& SaveFile)
{
  SaveFile >> Picture >> AlphaSum >> ShadowPos >> (int&)SpecialFlags;
}

/* Shadow and this->ShadowPos specify the location of the raw image of
   MotherItem of which shape the pixels are bound. If the image is meant
   to be drawn on the ground, Shadow should be zero. The alphas of the
   pixels will be on average AlphaSuggestion. PixelPredicate is used
   to determine whether pixels of the Shadow are allowed to be covered
   by the fluid. It is not used if Shadow == 0. */

void fluid::imagedata::AddLiquidToPicture(const rawbitmap* Shadow, long Pixels, long AlphaSuggestion, col16 Color, pixelpredicate PixelPredicate)
{
  if(ShadowPos == ERROR_V2)
    return;

  DripTimer = 0;
  cint* ValidityMap = igraph::GetBodyBitmapValidityMap(SpecialFlags);
  v2 PixelAllowed[256];
  int PixelsAllowed = 0;

  if(Shadow)
  {
    for(int x = 1; x < 14; ++x)
      for(int y = 1; y < 14; ++y)
	if(ValidityMap[x] & (1 << y)
	   && !(Shadow->*PixelPredicate)(ShadowPos + v2(x, y)))
	  PixelAllowed[PixelsAllowed++] = v2(x, y);

    if(!PixelsAllowed)
      return;
  }

  long Lumps = Pixels - (Pixels << 3) / 9; // ceil[Pixels/9]
  long RoomForPixels = (Lumps << 3) + Lumps;
  int Red = GetRed16(Color);
  int Green = GetGreen16(Color);
  int Blue = GetBlue16(Color);

  if(AlphaSuggestion < 25)
    AlphaSuggestion = 25;

  for(long c = 0; c < Lumps; ++c)
  {
    v2 Cords;

    if(Shadow)
      Cords = PixelAllowed[RAND() % PixelsAllowed];
    else
      Cords = v2(1 + RAND() % 14, 1 + RAND() % 14);

    Picture->PutPixel(Cords, Color);
    long Alpha = Limit<long>(AlphaSuggestion - 25 + RAND() % 50, 0, 0xFF);
    AlphaSum += Alpha - Picture->GetAlpha(Cords);
    Picture->SetAlpha(Cords, Alpha);
    Picture->SafeUpdateRandMap(Cords, true);
    --Pixels;
    --RoomForPixels;

    for(int d = 0; d < 8; ++d)
    {
      if(Pixels > RAND() % RoomForPixels)
      {
	v2 Pos = Cords + game::GetMoveVector(d);

	if(!Shadow || (!(Shadow->*PixelPredicate)(ShadowPos + Pos)
		       && ValidityMap[Pos.X] & (1 << Pos.Y)))
	{
	  --Pixels;
	  Picture->PutPixel(Pos, MakeRGB16(Limit<int>(Red - 25 + RAND() % 51, 0, 0xFF),
					   Limit<int>(Green - 25 + RAND() % 51, 0, 0xFF),
					   Limit<int>(Blue - 25 + RAND() % 51, 0, 0xFF)));

	  long Alpha = Limit<long>(AlphaSuggestion - 25 + RAND() % 50, 0, 0xFF);
	  AlphaSum += Alpha - Picture->GetAlpha(Pos);
	  Picture->SetAlpha(Pos, Alpha);
	  Picture->SafeUpdateRandMap(Pos, true);

	  if(!Pixels) // implies c + 1 == Lumps
	    break;
	}
      }

      --RoomForPixels;
    }
  }

  AlphaAverage = Picture->CalculateAlphaAverage();
}

/* Remakes all images. Usually decreases, and never increases, the liquid's
   volume */

void fluid::Redistribute()
{
  if(!UseImage())
    return;

  truth InitRandMap = truth(MotherItem);
  Image.Clear(InitRandMap);

  if(GearImage)
    if(Flags & HAS_BODY_ARMOR_PICTURES)
      for(int c = 0; c < BODY_ARMOR_PARTS; ++c)
	GearImage[c].Clear(InitRandMap);
    else
      GearImage->Clear(InitRandMap);

  AddLiquid(Liquid->GetVolume());
}

void fluid::imagedata::Clear(truth InitRandMap)
{
  Picture->ClearToColor(TRANSPARENT_COLOR);
  Picture->FillAlpha(0);
  AlphaSum = 0;

  if(InitRandMap)
    Picture->InitRandMap();
}

material* fluid::RemoveMaterial(material*)
{
  Destroy();
  return 0;
}

void fluid::Destroy()
{
  if(MotherItem)
  {
    if(!MotherItem->Exists())
      return;

    MotherItem->RemoveFluid(this);
  }
  else
  {
    character* Char = game::SearchCharacter(GetVictimID());

    if(Char)
      Char->RemoveTrap(GetTrapID());

    TrapData.VictimID = 0;
    LSquareUnder->RemoveFluid(this);
  }

  SendToHell();
}

truth fluid::UseImage() const
{
  return !(Flags & FLUID_INSIDE)
    && (!MotherItem || MotherItem->ShowFluids());
}

void fluid::AddTrapName(festring& String, int) const
{
  Liquid->AddName(String, false, false);
}

truth fluid::TryToUnStick(character* Victim, v2)
{
  ulong TrapID = GetTrapID();
  int Sum = Victim->GetAttribute(ARM_STRENGTH) + Victim->GetAttribute(LEG_STRENGTH) + Victim->GetAttribute(DEXTERITY) + Victim->GetAttribute(AGILITY);
  int Modifier = Liquid->GetStickiness() * Liquid->GetVolume() / (Max(Sum, 1) * 500);

  if(!RAND_N(Max(Modifier, 2)))
  {
    Victim->RemoveTrap(TrapID);
    TrapData.VictimID = 0;

    if(Victim->IsPlayer())
      ADD_MESSAGE("You manage to unstick yourself from the %s.", Liquid->GetName(false, false).CStr());
    else if(Victim->CanBeSeenByPlayer())
      ADD_MESSAGE("%s manages to unstick %sself from the %s.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, Liquid->GetName(false, false).CStr());

    Victim->EditAP(-250);
    return true;
  }

  Modifier = Sum * 10000 / (Liquid->GetStickiness() * Liquid->GetVolume());

  if(!RAND_N(Max(Modifier, 2)))
  {
    int VictimBodyPart = Victim->RandomizeTryToUnStickBodyPart(ALL_BODYPART_FLAGS&~TrapData.BodyParts);

    if(VictimBodyPart != NONE_INDEX)
    {
      TrapData.BodyParts |= 1 << VictimBodyPart;
      Victim->AddTrap(GetTrapID(), 1 << VictimBodyPart);

      if(Victim->IsPlayer())
	ADD_MESSAGE("You fail to free yourself from the %s and your %s is stuck in it in the attempt.", Liquid->GetName(false, false).CStr(), Victim->GetBodyPartName(VictimBodyPart).CStr());
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tries to free %sself from the %s but is stuck more tightly in it in the attempt.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN, Liquid->GetName(false, false).CStr());

      Victim->EditAP(-1000);
      return true;
    }
  }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to unstick yourself from %s.", Liquid->GetName(false, false).CStr());

  Victim->EditAP(-1000);
  return false;
}

void fluid::StepOnEffect(character* Stepper)
{
  if(!Liquid->GetStickiness() || Stepper->IsImmuneToStickiness())
    return;

  int StepperBodyPart = Stepper->GetRandomStepperBodyPart();

  if(StepperBodyPart == NONE_INDEX)
    return;

  TrapData.VictimID = Stepper->GetID();
  TrapData.BodyParts = 1 << StepperBodyPart;
  Stepper->AddTrap(GetTrapID(), 1 << StepperBodyPart);

  if(Stepper->IsPlayer())
    ADD_MESSAGE("Your %s is stuck to %s.", Stepper->GetBodyPartName(StepperBodyPart).CStr(), Liquid->GetName(false, false).CStr());
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s gets stuck to %s.", Stepper->CHAR_NAME(DEFINITE), Liquid->GetName(false, false).CStr());
}

void fluid::PreProcessForBone()
{
  game::RemoveTrapID(TrapData.TrapID);
  TrapData.TrapID = 0;
}

void fluid::PostProcessForBone()
{
  TrapData.TrapID = game::CreateNewTrapID(this);
}

truth fluid::IsStuckTo(ccharacter* Char) const
{
  return TrapData.VictimID == Char->GetID();
}

truth fluid::IsDangerous(ccharacter* Char) const
{
  return Char->GetAttribute(WISDOM) >= Liquid->GetStepInWisdomLimit();
}
