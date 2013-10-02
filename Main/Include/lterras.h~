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

#ifndef __LTERRAS_H__
#define __LTERRAS_H__

#include "lterra.h"

class ghost;

OLTERRAIN(wall, olterrain)
{
};

OLTERRAIN(decoration, olterrain)
{
};

GLTERRAIN(solidterrain, glterrain)
{
};

OLTERRAIN(door, olterrain)
{
 public:
  virtual truth Open(character*);
  virtual truth Close(character*);
  virtual truth CanBeOpened() const { return !Opened; }
  virtual void BeKicked(character*, int, int);
  virtual void SetIsOpened(truth What) { Opened = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth IsDoor() const { return true; }
  virtual void SetIsLocked(truth What) { Locked = What; }
  virtual truth IsLocked() const { return Locked; }
  virtual truth CanBeOpenedByAI();
  virtual void ReceiveDamage(character*, int, int);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual truth TryKey(item*, character*);
  virtual void SetParameters(int);
  virtual void Lock() { SetIsLocked(true); }
  virtual void HasBeenHitByItem(character*, item*, int);
  virtual truth IsTransparent() const;
  virtual int GetWalkability() const;
  virtual int GetTheoreticalWalkability() const;
  virtual void BeDestroyed();
 protected:
  virtual void PostConstruct();
  virtual truth AddAdjective(festring&, truth) const;
  virtual void Break();
  virtual v2 GetBitmapPos(int) const;
  virtual void MakeWalkable();
  virtual void MakeNotWalkable();
  virtual int GetBoobyTrap() { return BoobyTrap; }
  virtual void SetBoobyTrap(int What) { BoobyTrap = What; }
  truth Opened;
  truth Locked;
  int BoobyTrap;
};

OLTERRAIN(stairs, olterrain)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth Enter(truth) const;
  virtual void StepOn(character*);
  virtual void SetAttachedArea(int What) { AttachedArea = What; }
  virtual void SetAttachedEntry(int What) { AttachedEntry = What; }
  int GetAttachedArea() const { return AttachedArea; }
  int GetAttachedEntry() const { return AttachedEntry; }
  virtual void AddSpecialCursors();
 protected:
  virtual void PostConstruct();
  int AttachedArea;
  int AttachedEntry;
};

OLTERRAIN(portal, stairs)
{
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual v2 GetBitmapPos(int) const;
};

OLTERRAIN(altar, olterrain)
{
 public:
  virtual truth AcceptsOffers() const { return true; }
  virtual void StepOn(character*);
  virtual void BeKicked(character*, int, int);
  virtual truth ReceiveVomit(character*, liquid*);
  virtual truth Polymorph(character*);
  virtual truth SitOn(character*);
  virtual void Draw(blitdata&) const;
  virtual truth VomitingIsDangerous(ccharacter*) const;
};

OLTERRAIN(throne, decoration)
{
 public:
  virtual truth SitOn(character*);
};

OLTERRAIN(olterraincontainer, olterrain)
{
 public:
  olterraincontainer();
  virtual ~olterraincontainer();
  virtual truth Open(character*);
  virtual truth CanBeOpened() const { return true; }
  virtual stack* GetContained() const { return Contained; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int);
  virtual void Break();
  virtual truth AllowContentEmitation() const { return false; }
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone();
 protected:
  stack* Contained;
};

OLTERRAIN(fountain, olterrain)
{
 public:
  virtual ~fountain();
  virtual truth SitOn(character*);
  virtual truth Drink(character*);
  virtual truth HasDrinkEffect() const { return true; }
  virtual void DryOut();
  virtual truth DipInto(item*, character*);
  virtual truth IsDipDestination() const;
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, truth = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual material* GetMaterial(int) const;
  virtual void InitMaterials(const materialscript*, const materialscript*, truth);
  virtual truth IsFountainWithWater() const;
  virtual int GetSparkleFlags() const;
 protected:
  virtual void GenerateMaterials();
  virtual col16 GetMaterialColorB(int) const;
  virtual void AddPostFix(festring& String, int) const { AddContainerPostFix(String); }
  virtual truth AddAdjective(festring&, truth) const;
  virtual v2 GetBitmapPos(int) const;
  material* SecondaryMaterial;
};

OLTERRAIN(brokendoor, door)
{
 public:
  virtual void BeKicked(character*, int, int);
  virtual void ReceiveDamage(character*, int, int);
  virtual void HasBeenHitByItem(character*, item*, int);
  virtual void Break() { olterrain::Break(); }
};

GLTERRAIN(liquidterrain, glterrain)
{
 public:
  virtual cchar* SurviveMessage() const;
  virtual cchar* MonsterSurviveMessage() const;
  virtual cchar* DeathMessage() const;
  virtual cchar* MonsterDeathVerb() const;
  virtual cchar* ScoreEntry() const;
  virtual truth IsFatalToStay() const { return true; }
  virtual truth DipInto(item*, character*);
  virtual truth IsDipDestination() const { return true; }
  virtual void SurviveEffect(character*);
  virtual void AddLocationDescription(festring&) const;
 protected:
  virtual void AddPostFix(festring& String, int) const { AddLumpyPostFix(String); }
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual v2 GetBitmapPos(int) const;
};

OLTERRAIN(boulder, olterrain)
{
};

OLTERRAIN(sign, olterrain)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetText(cfestring& What) { Text = What; }
  virtual festring GetText() const;
  virtual void AddPostFix(festring&, int) const;
  virtual void StepOn(character*);
 protected:
  festring Text;
};

OLTERRAIN(earth, olterrain)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void PostConstruct();
  virtual v2 GetBitmapPos(int) const;
  int PictureIndex;
};

OLTERRAIN(monsterportal, olterrain)
{
 public:
  monsterportal();
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual v2 GetBitmapPos(int) const;
};

OLTERRAIN(coffin, olterraincontainer)
{
 public:
  virtual truth Open(character*);
  virtual void Break();
 protected:
  virtual void GenerateGhost(lsquare*);
};

OLTERRAIN(barwall, olterrain)
{
 public:
  void Break();
};

OLTERRAIN(ironmaiden, olterrain)
{
 public:
  ironmaiden() : Opened(false) {}
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual truth Open(character*);
  virtual truth CanBeOpened() const { return !Opened; }
  virtual truth Close(character*);
 protected:
  virtual v2 GetBitmapPos(int) const;
  truth Opened;
};

#endif
