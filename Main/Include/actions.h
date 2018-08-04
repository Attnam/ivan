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

#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "action.h"
#include "festring.h"
#include "v2.h"

ACTION(unconsciousness, action)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  void SetCounter(int What) { Counter = What; }
  virtual truth IsVoluntary() const { return false; }
  virtual void Terminate(truth);
  virtual truth AllowUnconsciousness() const { return false; }
  virtual cchar* GetDescription() const;
  virtual cchar* GetDeathExplanation() const;
  virtual truth CanBeTalkedTo() const { return false; }
  virtual truth IsUnconsciousness() const { return true; }
  void RaiseCounterTo(int);
 protected:
  int Counter;
};

ACTION(consume, action)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual void Terminate(truth);
  void SetConsumingID(ulong What) { ConsumingID = What; }
  virtual truth AllowUnconsciousness() const { return false; }
  virtual truth AllowFoodConsumption() const { return false; }
  virtual cchar* GetDescription() const;
  virtual void SetDescription(cfestring&);
  virtual void SetNibbling(truth What) { nibbling = What; }
  virtual truth IsNibbling() { return nibbling; }
 protected:
  festring Description;
  ulong ConsumingID;
  truth nibbling = false;
};

ACTION(rest, action)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  void SetGoalHP(int What) { GoalHP = What; }
  virtual void Terminate(truth);
  virtual truth IsRest() const { return true; }
  virtual cchar* GetDescription() const;
  void SetMinToStop(int What) { MinToStop = What; }
 protected:
  int GoalHP;
  int MinToStop;
};

ACTION(dig, action)
{
 public:
  dig() : RightBackupID(0), LeftBackupID(0) { }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  void SetSquareDug(v2 What) { SquareDug = What; }
  virtual void Terminate(truth);
  void SetRightBackupID(ulong What) { RightBackupID = What; }
  void SetLeftBackupID(ulong What) { LeftBackupID = What; }
  virtual truth TryDisplace() { return false; }
  virtual cchar* GetDescription() const;
  virtual truth ShowEnvironment() const { return false; }
  void SetMoveDigger(truth What) { MoveDigger = What; }
 protected:
  ulong RightBackupID;
  ulong LeftBackupID;
  v2 SquareDug;
  truth MoveDigger;
};

ACTION(go, action)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  int GetDirection() const { return Direction; }
  void SetDirection(int What) { Direction = What; }
  truth IsWalkingInOpen() const { return WalkingInOpen; }
  void SetIsWalkingInOpen(truth What) { WalkingInOpen = What; }
  virtual truth TryDisplace();
  virtual cchar* GetDescription() const;
  virtual truth ShowEnvironment() const { return false; }
 protected:
  int Direction;
  truth WalkingInOpen;
};

ACTION(study, action)
{
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual void Terminate(truth);
  void SetLiteratureID(ulong What) { LiteratureID = What; }
  virtual cchar* GetDescription() const;
  void SetCounter(int What) { Counter = What; }
 protected:
  ulong LiteratureID;
  int Counter;
};

#endif
