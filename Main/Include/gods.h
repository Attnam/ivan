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

#ifndef __GODS_H__
#define __GODS_H__

#include "god.h"

#define FAVOURDEBIT_AUTO -1
#define FAVOURDEBIT_AUTOHALF -2
#define FAVOURDEBIT_AUTODOUBLE -3

GOD(valpurus, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(legifer, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(atavus, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth LikesMaterial(const materialdatabase*, ccharacter*) const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(dulcis, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(seges, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth ForceGiveBodyPart() const { return true; }
  virtual truth HealRegeneratingBodyParts() const { return true; }
  virtual truth LikesMaterial(const materialdatabase*, ccharacter*) const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(sophos, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(silva, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(loricatus, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(mellis, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(cleptia, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(nefas, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(scabies, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual truth PlayerVomitedOnAltar(liquid*);
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual truth LikesMaterial(const materialdatabase*, ccharacter*) const;
  virtual truth MutatesBodyParts() const { return true; }
  virtual int GetSex() const { return FEMALE; }
  virtual truth LikesVomit() const { return true; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(infuscor, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return FEMALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(cruentus, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

GOD(mortifer, god)
{
 public:
  virtual cchar* GetName() const;
  virtual cchar* GetDescription() const;
  virtual int GetAlignment() const;
  virtual void Pray();
  virtual int GetBasicAlignment() const;
  virtual col16 GetColor() const;
  virtual col16 GetEliteColor() const;
  virtual int GetSex() const { return MALE; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
  virtual bool Favour(cfestring fsWhat, int iDebit=0);
};

#endif
