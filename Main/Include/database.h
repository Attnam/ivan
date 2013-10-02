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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <map>

#include "typedef.h"

class inputfile;
class festring;

template <class database> struct databasememberbase
{
  virtual void ReadData(database&, inputfile&) = 0;
};

template <class type> class databasecreator
{
 public:
  typedef typename type::database database;
  typedef typename type::prototype prototype;
  typedef std::map<festring, databasememberbase<database>*> databasemembermap;
  static void ReadFrom(inputfile&);
  static void FindDataBase(const database*&, const prototype*, int);
  static void InstallDataBase(type*, int);
  static void CreateDataBaseMemberMap();
  static int CreateDivineConfigurations(const prototype*, database**, int);
 private:
  static truth AnalyzeData(inputfile&, cfestring&, database&);
  static void CheckDefaults(cfestring&, database&);
  static void CreateLTerrainDataBaseMemberMap();
  static void SetBaseValue(cfestring&, databasememberbase<database>*, database&);
  static databasemembermap& GetDataBaseMemberMap();
};

class databasesystem
{
 public:
  static void Initialize();
};

#endif
