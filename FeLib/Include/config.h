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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <fstream>

#include "festring.h"

class inputfile;
class felist;
struct configoption;
struct stringoption;
struct numberoption;
struct truthoption;

class configsystem
{
 public:
  static truth Save();
  static truth Load();
  static void Show(void (*)() = 0, void (*)(felist&) = 0, truth = false);
  static void AddOption(configoption*);
  static void NormalStringDisplayer(const stringoption*, festring&);
  static void NormalNumberDisplayer(const numberoption*, festring&);
  static void NormalTruthDisplayer(const truthoption*, festring&);
  static truth NormalStringChangeInterface(stringoption*);
  static truth NormalNumberChangeInterface(numberoption*);
  static truth NormalTruthChangeInterface(truthoption*);
  static void NormalStringChanger(stringoption*, cfestring&);
  static void NormalNumberChanger(numberoption*, long);
  static void NormalTruthChanger(truthoption*, truth);
  static void SetConfigFileName(cfestring& What)
  { ConfigFileName = What; }
 private:
  static configoption* Option[MAX_CONFIG_OPTIONS];
  static festring ConfigFileName;
  static int Options;
};

/* Currently there's no human-readable output option in outputfile,
   so we're forced to use std::ofstream */

struct configoption
{
  configoption(cchar*, cchar*);
  virtual ~configoption() { }
  virtual void SaveValue(std::ofstream&) const = 0;
  virtual void LoadValue(inputfile&) = 0;
  virtual truth ActivateChangeInterface() = 0;
  virtual void DisplayeValue(festring&) const = 0;
  cchar* Name;
  cchar* Description;
};

struct stringoption : public configoption
{
  stringoption(cchar*, cchar*, cfestring&,
	       void (*)(const stringoption*, festring&)
	       = &configsystem::NormalStringDisplayer,
	       truth (*)(stringoption*)
	       = &configsystem::NormalStringChangeInterface,
	       void (*)(stringoption*, cfestring&)
	       = &configsystem::NormalStringChanger);
  virtual void SaveValue(std::ofstream&) const;
  virtual void LoadValue(inputfile&);
  virtual void DisplayeValue(festring& Entry) const
  { ValueDisplayer(this, Entry); }
  virtual truth ActivateChangeInterface() { return ChangeInterface(this); }
  void ChangeValue(cfestring& What) { ValueChanger(this, What); }
  festring Value;
  void (*ValueDisplayer)(const stringoption*, festring&);
  truth (*ChangeInterface)(stringoption*);
  void (*ValueChanger)(stringoption*, cfestring&);
};

struct numberoption : public configoption
{
  numberoption(cchar*, cchar*, long,
	       void (*)(const numberoption*, festring&)
	       = &configsystem::NormalNumberDisplayer,
	       truth (*)(numberoption*)
	       = &configsystem::NormalNumberChangeInterface,
	       void (*)(numberoption*, long)
	       = &configsystem::NormalNumberChanger);
  virtual void SaveValue(std::ofstream&) const;
  virtual void LoadValue(inputfile&);
  virtual void DisplayeValue(festring& Entry) const
  { ValueDisplayer(this, Entry); }
  virtual truth ActivateChangeInterface() { return ChangeInterface(this); }
  void ChangeValue(long What) { ValueChanger(this, What); }
  long Value;
  void (*ValueDisplayer)(const numberoption*, festring&);
  truth (*ChangeInterface)(numberoption*);
  void (*ValueChanger)(numberoption*, long);
};

struct scrollbaroption : public numberoption
{
  scrollbaroption(cchar*, cchar*, long,
		  void (*)(const numberoption*, festring&),
		  truth (*)(numberoption*),
		  void (*)(numberoption*, long)
		  = &configsystem::NormalNumberChanger,
		  void (*)(long) = 0);
  void (*BarHandler)(long);
};

struct truthoption : public configoption
{
  truthoption(cchar*, cchar*, truth,
	      void (*)(const truthoption*, festring&)
	      = &configsystem::NormalTruthDisplayer,
	      truth (*)(truthoption*)
	      = &configsystem::NormalTruthChangeInterface,
	      void (*)(truthoption*, truth)
	      = &configsystem::NormalTruthChanger);
  virtual void SaveValue(std::ofstream&) const;
  virtual void LoadValue(inputfile&);
  virtual void DisplayeValue(festring& Entry) const
  { ValueDisplayer(this, Entry); }
  virtual truth ActivateChangeInterface() { return ChangeInterface(this); }
  void ChangeValue(truth What) { ValueChanger(this, What); }
  truth Value;
  void (*ValueDisplayer)(const truthoption*, festring&);
  truth (*ChangeInterface)(truthoption*);
  void (*ValueChanger)(truthoption*, truth);
};

#endif
