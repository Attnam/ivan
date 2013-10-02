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

smoke::smoke() : entity(HAS_BE), Next(0) { }
square* smoke::GetSquareUnderEntity(int) const { return LSquareUnder; }

smoke::smoke(gas* Gas, lsquare* LSquareUnder) : entity(HAS_BE), Next(0), Gas(Gas), LSquareUnder(LSquareUnder), Alpha(Gas->GetAlpha())
{
  Gas->SetMotherEntity(this);
  Picture.resize(16);
  packcol16 Color = Gas->GetColor();
  bitmap Temp(TILE_V2, TRANSPARENT_COLOR);
  Temp.ActivateFastFlag();
  int Frame[16];
  int Flags[16];

  for(int c = 0; c < 16; ++c)
  {
    Picture[c] = new bitmap(TILE_V2, TRANSPARENT_COLOR);
    Picture[c]->ActivateFastFlag();
    Picture[c]->CreateAlphaMap(Alpha);
    truth Correct = false;

    while(!Correct)
    {
      Frame[c] = RAND() & 3;
      Flags[c] = RAND() & 7;
      Correct = true;

      for(int i = 0; i < c; ++i)
	if(Frame[c] == Frame[i] && Flags[c] == Flags[i])
	{
	  Correct = false;
	  break;
	}
    }

    igraph::GetRawGraphic(GR_EFFECT)->MaskedBlit(&Temp, v2(Frame[c] << 4, 32), ZERO_V2, TILE_V2, &Color);
    Temp.NormalBlit(Picture[c], Flags[c]);
  }

  LSquareUnder->SignalSmokeAlphaChange(Alpha);
}

smoke::~smoke()
{
  for(uint c = 0; c < Picture.size(); ++c)
    delete Picture[c];

  delete Gas;
}

void smoke::Be()
{
  if(!(RAND() & 7))
  {
    LSquareUnder->SendNewDrawRequest();
    LSquareUnder->SignalSmokeAlphaChange(-1);

    if(!--Alpha)
    {
      LSquareUnder->RemoveSmoke(this);
      SendToHell();
      return;
    }

    for(int c = 0; c < 16; ++c)
      Picture[c]->FillAlpha(Alpha);

    Gas->SetVolume(Gas->GetVolume() - Gas->GetVolume() / 50);
  }

  character* Char = LSquareUnder->GetCharacter();

  if(Char && !Char->StateIsActivated(GAS_IMMUNITY))
    Gas->BreatheEffect(Char);
}

void smoke::Draw(blitdata& BlitData) const
{
  Picture[(GET_TICK() >> 1) & 3]->AlphaLuminanceBlit(BlitData);
}

void smoke::Save(outputfile& SaveFile) const
{
  SaveFile << Picture << Gas << Alpha;
}

void smoke::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  SaveFile >> Picture >> Gas >> Alpha;
  Gas->SetMotherEntity(this);
}

outputfile& operator<<(outputfile& SaveFile, const smoke* Smoke)
{
  Smoke->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, smoke*& Smoke)
{
  Smoke = new smoke;
  Smoke->Load(SaveFile);
  return SaveFile;
}

void smoke::AddBreatheMessage() const
{
  if(Gas->GetBreatheMessage().GetSize())
    ADD_MESSAGE("%s", Gas->GetBreatheMessage().CStr());
}

void smoke::Merge(gas* OtherGas)
{
  Gas->EditVolume(OtherGas->GetVolume());
  LSquareUnder->SignalSmokeAlphaChange(OtherGas->GetAlpha() - Alpha);
  Alpha = OtherGas->GetAlpha();

  for(int c = 0; c < 16; ++c)
    Picture[c]->FillAlpha(Alpha);

  delete OtherGas;
}

truth smoke::IsDangerousToBreathe(ccharacter* Who) const
{
  return (!Who->StateIsActivated(GAS_IMMUNITY)
	  && Who->GetAttribute(WISDOM) >= Gas->GetStepInWisdomLimit());
}

truth smoke::IsScaryToBreathe(ccharacter* Who) const
{
  return (!Who->StateIsActivated(GAS_IMMUNITY)
	  && Gas->GetCategoryFlags() & IS_SCARY);
}
