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

#ifndef __MATERIAS_H__
#define __MATERIAS_H__

#include "materia.h"

class lterrain;

MATERIAL(solid, material)
{
 public:
  solid() : BurnData(NOT_BURNT), TransientThermalEnergy(0), SteadyStateThermalEnergy(0), BurnCounter(0), BurnCheckCounter(0) { }
  virtual void SetBurnLevel(int, truth);
  virtual int GetStrengthValue() const;
  virtual int GetBurnModifier() const;
  virtual void ResetThermalEnergies();
  virtual void Be(ulong);
  virtual truth HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsSparkling() const;
  virtual int GetBurnData() const { return BurnData; }
  virtual int GetTransientThermalEnergy() const { return TransientThermalEnergy; }
  virtual truth AddBurnLevelDescription(festring&, truth) const;
  virtual int GetBurnLevel() const { return BurnData & 3; }
  virtual void ResetBurning();
  virtual truth IsVeryCloseToBurning() const { return (BurnData & 3) == HEAVILY_BURNT; }
  virtual void AddToThermalEnergy(int);
  virtual void AddToSteadyStateThermalEnergy(int);
  virtual void AddToTransientThermalEnergy(int);
  virtual void RemoveFromThermalEnergy(int);
 protected:
  virtual void PostConstruct();
  int BurnData;
  int TransientThermalEnergy; // Transient is for storing the energy in an explosion so that
                              // mutliple explosions accumulate, SteadyState is to store the
                              // energy to be decremented only by the application of liquids
  int SteadyStateThermalEnergy;
  ushort BurnCounter;
  uchar BurnCheckCounter;
};

MATERIAL(organic, solid)
{
 public:
  virtual void Be(ulong);
  virtual truth HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsVeryCloseToSpoiling() const { return SpoilLevel == 8; }
  virtual int GetSpoilLevel() const { return SpoilLevel; }
  virtual void ResetSpoiling();
  virtual material* EatEffect(character*, long);
  virtual void AddConsumeEndMessage(character*) const;
  virtual void SetSpoilCounter(int);
  virtual truth CanSpoil() const { return true; }
  virtual int GetSpoilPercentage() const;
  virtual truth Spoils() const { return true; }
 protected:
  virtual void PostConstruct();
  ushort SpoilCounter;
  uchar SpoilCheckCounter;
  uchar SpoilLevel;
};

MATERIAL(gas, material)
{
  virtual int IsBurning() const { return 0; }
};

MATERIAL(liquid, material)
{
 public:
  virtual cchar* GetConsumeVerb() const;
  virtual truth IsLiquid() const { return true; }
  void TouchEffect(item*, cfestring&);
  void TouchEffect(character*, int);
  void TouchEffect(lterrain*);
  liquid* SpawnMoreLiquid(long Volume) const { return static_cast<liquid*>(SpawnMore(Volume)); }
  virtual int IsBurning() const { return 0; }
};

MATERIAL(flesh, organic)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual col16 GetSkinColor() const { return SkinColor; }
  virtual void SetSkinColor(int What) { SkinColor = What; }
  virtual truth SkinColorIsSparkling() const { return SkinColorSparkling; }
  virtual void SetSkinColorIsSparkling(truth What) { SkinColorSparkling = What; }
  virtual truth IsFlesh() const { return true; }
  virtual void SetIsInfectedByLeprosy(truth What) { InfectedByLeprosy = What; }
  virtual truth IsInfectedByLeprosy() const { return InfectedByLeprosy; }
 protected:
  virtual void PostConstruct();
  col16 SkinColor;
  truth SkinColorSparkling;
  truth InfectedByLeprosy;
};

MATERIAL(powder, liquid)
{
 public:
  powder() : Wetness(0) { }
  virtual truth IsPowder() const { return true; }
  virtual truth IsExplosive() const;
  virtual void AddWetness(long What) { Wetness += What; }
  virtual void Be(ulong);
  virtual truth HasBe() const { return true; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  long Wetness;
};

/* Materials that can rust */

MATERIAL(ironalloy, solid)
{
 public:
  ironalloy() : RustData(NOT_RUSTED) { }
  virtual void SetRustLevel(int);
  virtual int GetStrengthValue() const;
  virtual int GetRustLevel() const { return RustData & 3; }
  virtual truth IsSparkling() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetRustData() const { return RustData; }
  virtual truth TryToRust(long, long = 0);
  virtual truth AddRustLevelDescription(festring&, truth) const;
 protected:
  int RustData;
};

#endif
