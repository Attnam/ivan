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

#include "object.h"
#include "materia.h"
#include "festring.h"
#include "whandler.h"
#include "rawbit.h"
#include "proto.h"
#include "game.h"
#include "bitmap.h"
#include "save.h"

v2 RightArmSparkleValidityArray[128];
v2 LeftArmSparkleValidityArray[128];
v2 GroinSparkleValidityArray[169];
v2 RightLegSparkleValidityArray[42];
v2 LeftLegSparkleValidityArray[45];
v2 NormalSparkleValidityArray[256];
v2 PossibleSparkleBuffer[256];

object::object() : entity(0), MainMaterial(0) { }
int object::GetSpecialFlags() const { return ST_NORMAL; }
col16 object::GetOutlineColor(int) const { return TRANSPARENT_COLOR; }
cbitmap*const* object::GetPicture() const { return GraphicData.Picture; }

object::object(const object& Object) : entity(Object), id(Object), VisualEffects(Object.VisualEffects)
{
  CopyMaterial(Object.MainMaterial, MainMaterial);
}

object::~object()
{
  delete MainMaterial;
}

void object::CopyMaterial(material* const& Source, material*& Dest)
{
  if(Source)
  {
    Dest = Source->Duplicate();
    Dest->SetMotherEntity(this);
  }
  else
    Dest = 0;
}

void object::Save(outputfile& SaveFile) const
{
  SaveFile << GraphicData << (int)VisualEffects;
  SaveFile << MainMaterial;
}

void object::Load(inputfile& SaveFile)
{
  SaveFile >> GraphicData >> (int&)VisualEffects;
  LoadMaterial(SaveFile, MainMaterial);
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, long FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, long SecondDefaultVolume, truth CallUpdatePictures)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::InitMaterial(material*& Material, material* NewMaterial, long DefaultVolume)
{
  Material = NewMaterial;

  if(Material)
  {
    if(Material->HasBe())
      Enable();

    if(DefaultVolume && !Material->GetVolume())
      Material->SetVolume(DefaultVolume);

    Material->SetMotherEntity(this);
    SignalEmitationIncrease(Material->GetEmitation());
  }
}

void object::ChangeMaterial(material*& Material, material* NewMaterial, long DefaultVolume, int SpecialFlags)
{
  delete SetMaterial(Material, NewMaterial, DefaultVolume, SpecialFlags);
}

material* object::SetMaterial(material*& Material, material* NewMaterial, long DefaultVolume, int SpecialFlags)
{
  material* OldMaterial = Material;
  Material = NewMaterial;

  if((!OldMaterial || !OldMaterial->HasBe())
     && NewMaterial && NewMaterial->HasBe())
    Enable();
  else if(OldMaterial && OldMaterial->HasBe()
	  && (!NewMaterial || !NewMaterial->HasBe())
	  && !CalculateHasBe())
    Disable();

  if(NewMaterial)
  {
    if(!NewMaterial->GetVolume())
    {
      if(OldMaterial)
	NewMaterial->SetVolume(OldMaterial->GetVolume());
      else if(DefaultVolume)
	NewMaterial->SetVolume(DefaultVolume);
      else
	ABORT("Singularity spawn detected!");
    }

    NewMaterial->SetMotherEntity(this);

    if(!(SpecialFlags & NO_SIGNALS))
      SignalEmitationIncrease(NewMaterial->GetEmitation());
  }

  if(!(SpecialFlags & NO_SIGNALS))
  {
    if(OldMaterial)
      SignalEmitationDecrease(OldMaterial->GetEmitation());

    SignalVolumeAndWeightChange();
    SignalMaterialChange();
  }

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();

  return OldMaterial;
}

void object::UpdatePictures()
{
  static cv2 ZeroPos(0, 0);
  UpdatePictures(GraphicData, ZeroPos, VisualEffects|GetSpecialFlags(), GetMaxAlpha(), GetGraphicsContainerIndex(), &object::GetBitmapPos);
}

truth object::RandomizeSparklePos(v2& SparklePos, v2 BPos, int& SparkleTime, ulong SeedBase, int SpecialFlags, int GraphicsContainerIndex) const
{
  static int SeedModifier = 1;
  femath::SaveSeed();
  femath::SetSeed(SeedBase + SeedModifier);

  if(++SeedModifier > 0x10)
    SeedModifier = 1;

  v2* ValidityArray;
  int ValidityArraySize;

  if((SpecialFlags & 0x38) == ST_RIGHT_ARM)
  {
    ValidityArray = RightArmSparkleValidityArray;
    ValidityArraySize = 128;
  }
  else if((SpecialFlags & 0x38) == ST_LEFT_ARM)
  {
    ValidityArray = LeftArmSparkleValidityArray;
    ValidityArraySize = 128;
  }
  else if((SpecialFlags & 0x38) == ST_GROIN)
  {
    ValidityArray = GroinSparkleValidityArray;
    ValidityArraySize = 169;
  }
  else if((SpecialFlags & 0x38) == ST_RIGHT_LEG)
  {
    ValidityArray = RightLegSparkleValidityArray;
    ValidityArraySize = 42;
  }
  else if((SpecialFlags & 0x38) == ST_LEFT_LEG)
  {
    ValidityArray = LeftLegSparkleValidityArray;
    ValidityArraySize = 45;
  }
  else
  {
    ValidityArray = NormalSparkleValidityArray;
    ValidityArraySize = 256;
  }

  SparklePos = igraph::GetRawGraphic(GraphicsContainerIndex)->RandomizeSparklePos(ValidityArray, PossibleSparkleBuffer, BPos, TILE_V2, ValidityArraySize, GetSparkleFlags());

  if(SparklePos != ERROR_V2)
  {
    SparkleTime = RAND() % 241;
    femath::LoadSeed();
    return true;
  }
  else
  {
    femath::LoadSeed();
    return false;
  }
}

void object::UpdatePictures(graphicdata& GraphicData, v2 Position, int SpecialFlags, alpha MaxAlpha, int GraphicsContainerIndex, bposretriever BitmapPosRetriever) const
{
  int AnimationFrames = GetClassAnimationFrames();
  v2 SparklePos;
  int SparkleTime = 0;
  int Seed = 0;
  int FlyAmount = GetSpoilLevel();
  truth Sparkling = false, FrameNeeded = false, SeedNeeded = false;
  v2 BPos = (this->*BitmapPosRetriever)(0);
  alpha Alpha;

  if(!(SpecialFlags & (ST_FLAMES|ST_LIGHTNING)))
  {
    if(AllowSparkling())
    {
      int SparkleFlags = GetSparkleFlags();

      if(SparkleFlags
	 && RandomizeSparklePos(SparklePos, BPos, SparkleTime,
				BPos.X + BPos.Y + GetMaterialColorA(0),
				SpecialFlags, GraphicsContainerIndex))
      {
	Sparkling = true;

	if(AnimationFrames <= 256)
	  AnimationFrames = 256;
      }
    }

    if(FlyAmount)
    {
      SeedNeeded = true;
      FrameNeeded = true;

      if(AnimationFrames <= 32)
	AnimationFrames = 32;
    }
  }
  else if(SpecialFlags & ST_FLAMES)
  {
    SeedNeeded = true;
    FrameNeeded = true;

    if(AnimationFrames <= 16)
      AnimationFrames = 16;
  }
  else if(SpecialFlags & ST_LIGHTNING)
  {
    SeedNeeded = true;

    if(AnimationFrames <= 128)
      AnimationFrames = 128;
  }

  if(SeedNeeded)
  {
    static int SeedModifier = 1;
    Seed = BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier + 0x42;

    if(++SeedModifier > 0x10)
      SeedModifier = 1;
  }

  int WobbleMask = 0, WobbleData = GetWobbleData();

  if(WobbleData & WOBBLE)
  {
    int Speed = (WobbleData & WOBBLE_SPEED_RANGE) >> WOBBLE_SPEED_SHIFT;
    int Freq = (WobbleData & WOBBLE_FREQ_RANGE) >> WOBBLE_FREQ_SHIFT;
    int WobbleFrames = 512 >> (Freq + Speed);
    WobbleMask = 7 >> Freq << (6 - Speed);

    if(AnimationFrames <= WobbleFrames)
      AnimationFrames = WobbleFrames;
  }

  ModifyAnimationFrames(AnimationFrames);
  int c;
  int OldAnimationFrames = GraphicData.AnimationFrames;

  for(c = 0; c < OldAnimationFrames; ++c)
    igraph::RemoveUser(GraphicData.GraphicIterator[c]);

  if(OldAnimationFrames != AnimationFrames)
  {
    if(OldAnimationFrames)
    {
      delete [] GraphicData.Picture;
      delete [] GraphicData.GraphicIterator;
    }

    GraphicData.Picture = new bitmap*[AnimationFrames];
    GraphicData.GraphicIterator = new tilemap::iterator[AnimationFrames];
  }

  GraphicData.AnimationFrames = AnimationFrames;

  if(!AllowRegularColors())
    SpecialFlags |= ST_DISALLOW_R_COLORS;

  graphicid GI;
  GI.BaseAlpha = MaxAlpha;
  GI.FileIndex = GraphicsContainerIndex;
  GI.SpecialFlags = SpecialFlags;
  GI.Seed = Seed;
  GI.FlyAmount = FlyAmount;
  GI.Position = Position;
  GI.RustData[0] = GetRustDataA();
  GI.RustData[1] = GetRustDataB();
  GI.RustData[2] = GetRustDataC();
  GI.RustData[3] = GetRustDataD();
  GI.WobbleData = WobbleData;

  for(c = 0; c < AnimationFrames; ++c)
  {
    GI.Color[0] = GetMaterialColorA(c);
    GI.Color[1] = GetMaterialColorB(c);
    GI.Color[2] = GetMaterialColorC(c);
    GI.Color[3] = GetMaterialColorD(c);
    Alpha = GetAlphaA(c);
    GI.Alpha[0] = Alpha < MaxAlpha ? Alpha : MaxAlpha;
    Alpha = GetAlphaB(c);
    GI.Alpha[1] = Alpha < MaxAlpha ? Alpha : MaxAlpha;
    Alpha = GetAlphaC(c);
    GI.Alpha[2] = Alpha < MaxAlpha ? Alpha : MaxAlpha;
    Alpha = GetAlphaD(c);
    GI.Alpha[3] = Alpha < MaxAlpha ? Alpha : MaxAlpha;
    v2 BPos = (this->*BitmapPosRetriever)(c);
    GI.BitmapPosX = BPos.X;
    GI.BitmapPosY = BPos.Y;

    if(Sparkling && c > SparkleTime && c < SparkleTime + 16)
    {
      GI.SparklePosX = SparklePos.X;
      GI.SparklePosY = SparklePos.Y;
      GI.SparkleFrame = c - SparkleTime;
    }
    else
    {
      GI.SparklePosX = SPARKLE_POS_X_ERROR;
      GI.SparklePosY = 0;
      GI.SparkleFrame = 0;
    }

    GI.Frame = !c || FrameNeeded
	       || SpecialFlags & ST_LIGHTNING && !((c + 1) & 7)
	       || WobbleData & WOBBLE && !(c & WobbleMask)
	       ? c : 0;

    GI.OutlineColor = GetOutlineColor(c);
    GI.OutlineAlpha = GetOutlineAlpha(c);
    tilemap::iterator Iterator = igraph::AddUser(GI);
    GraphicData.GraphicIterator[c] = Iterator;
    GraphicData.Picture[c] = Iterator->second.Bitmap;
  }
}

col16 object::GetMaterialColorA(int) const
{
  return MainMaterial->GetColor();
}

truth object::AddRustLevelDescription(festring& String, truth Articled) const
{
  return MainMaterial->AddRustLevelDescription(String, Articled);
}

truth object::AddMaterialDescription(festring& String, truth Articled) const
{
  MainMaterial->AddName(String, Articled);
  String << ' ';
  return true;
}

void object::AddContainerPostFix(festring& String) const
{
  if(GetSecondaryMaterial())
    GetSecondaryMaterial()->AddName(String << " full of ", false, false);
}

void object::AddLumpyPostFix(festring& String) const
{
  MainMaterial->AddName(String << " of ", false, false);
}

alpha object::GetAlphaA(int) const
{
  return MainMaterial->GetAlpha();
}

void object::RandomizeVisualEffects()
{
  int AcceptedFlags = GetOKVisualEffects();

  if(AcceptedFlags)
    SetVisualEffects(RAND() & 0x7 & AcceptedFlags | GetForcedVisualEffects());
  else
    SetVisualEffects(GetForcedVisualEffects());
}

void object::LoadMaterial(inputfile& SaveFile, material*& Material)
{
  SaveFile >> Material;

  if(Material)
  {
    if(Material->HasBe())
      Enable();

    Material->SetMotherEntity(this);
    game::CombineLights(Emitation, Material->GetEmitation());
  }
}

int object::RandomizeMaterialConfiguration()
{
  const fearray<long>& MCC = GetMaterialConfigChances();
  return MCC.Size > 1
    ? femath::WeightedRand(MCC.Data, GetMaterialConfigChanceSum())
    : 0;
}

truth object::AddEmptyAdjective(festring& String, truth Articled) const
{
  if(GetSecondaryMaterial())
    return false;
  else
  {
    String << (Articled ? "an empty " : "empty ");
    return true;
  }
}

void object::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  if(MainMaterial)
    game::CombineLights(Emitation, MainMaterial->GetEmitation());
}

truth object::CalculateHasBe() const
{
  return MainMaterial && MainMaterial->HasBe();
}

int object::GetSparkleFlags() const
{
  return MainMaterial->IsSparkling() ? SPARKLING_A : 0;
}

void object::InitSparkleValidityArrays()
{
  int y, x, Index = 0;

  for(y = 0; y < 16; ++y)
    for(x = 0; x < 8; ++x)
      RightArmSparkleValidityArray[Index++] = v2(x, y);

  Index = 0;

  for(y = 0; y < 16; ++y)
    for(x = 8; x < 16; ++x)
      LeftArmSparkleValidityArray[Index++] = v2(x, y);

  Index = 0;

  for(y = 0; y < 10; ++y)
    for(x = 0; x < 16; ++x)
      GroinSparkleValidityArray[Index++] = v2(x, y);

  for(y = 10; y < 13; ++y)
    for(x = y - 5; x < 20 - y; ++x)
      GroinSparkleValidityArray[Index++] = v2(x, y);

  Index = 0;

  for(y = 10; y < 16; ++y)
    for(x = 0; x < 8; ++x)
      if((y != 10 || x < 5) && (y != 11 || x < 6) && (y != 12 || x < 7))
	RightLegSparkleValidityArray[Index++] = v2(x, y);

  Index = 0;

  for(y = 10; y < 16; ++y)
    for(x = 8; x < 16; ++x)
      if((y != 10 || x > 9) && (y != 11 || x > 8))
	LeftLegSparkleValidityArray[Index++] = v2(x, y);

  Index = 0;

  for(y = 0; y < 16; ++y)
    for(x = 0; x < 16; ++x)
      NormalSparkleValidityArray[Index++] = v2(x, y);
}

int object::GetRustDataA() const
{
  return MainMaterial->GetRustData();
}

truth object::DetectMaterial(cmaterial* Material) const
{
  for(int c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && GetMaterial(c)->IsSameAs(Material))
      return true;

  return false;
}
