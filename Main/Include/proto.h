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

#ifndef __PROTO_H__
#define __PROTO_H__

#include <map>
#include <vector>

#include "ivandef.h"
#include "save.h"

class character;
class item;
class material;
class god;
template <class type> class databasecreator;
struct itemdatabase;

typedef std::map<festring, long> valuemap;
typedef std::vector<item*> itemvector;
typedef std::vector<itemvector> itemvectorvector;
typedef std::vector<character*> charactervector;
typedef std::vector<material*> materialvector;

template <class type> class protocontainer
{
 public:
  friend class protosystem;
  friend class databasecreator<type>;
  typedef typename type::prototype prototype;
  static int Add(prototype*);
  static const prototype* GetProto(int I) { return GetProtoData()[I]; }
  static int SearchCodeName(cfestring&);
  static cchar* GetMainClassID() { return GetProtoData()[1]->GetClassID(); }
  static int GetSize() { return GetSizeRef(); }
 private:
  static prototype**& GetProtoData();
  static valuemap& GetCodeNameMap();
  static int& GetSizeRef();
};

template <class type>
inline int protocontainer<type>::Add(prototype* Proto)
{
  if(!GetSize())
    (GetProtoData() = new prototype*[1024])[GetSizeRef()++] = 0;

  int Index = GetSizeRef()++;
  GetProtoData()[Index] = Proto;
  std::pair<festring, long> Pair(Proto->GetClassID(), Index);
  GetCodeNameMap().insert(Pair);
  return Index;
}

template <class type>
inline int protocontainer<type>::SearchCodeName(cfestring& Name)
{
  valuemap::iterator I = GetCodeNameMap().find(Name);
  return I != GetCodeNameMap().end() ? I->second : 0;
}

class protosystem
{
 public:
  static character* BalancedCreateMonster();
  static item* BalancedCreateItem(long = 0, long = MAX_PRICE, long = ANY_CATEGORY, int = 0, int = 0, int = 0, truth = false);
  static character* CreateMonster(int = 1, int = 999999, int = 0);
  static character* CreateMonster(cfestring&, int = 0, truth = true);
  static item* CreateItem(cfestring&, truth = true);
  static material* CreateMaterial(cfestring&, long = 0, truth = true);
  static void CreateEveryNormalEnemy(charactervector&);
#ifdef WIZARD
  static void CreateEveryCharacter(charactervector&);
  static void CreateEveryItem(itemvectorvector&);
  static void CreateEveryMaterial(std::vector<material*>&);
#endif
  static void Initialize();
  static void InitCharacterDataBaseFlags();
  static void SaveCharacterDataBaseFlags(outputfile&);
  static void LoadCharacterDataBaseFlags(inputfile&);
  static void CreateEverySeenCharacter(charactervector&);
  static void CreateEveryMaterial(std::vector<material*>&, const god*, ccharacter*);
 private:
  static itemdatabase** ItemConfigData;
  static int ItemConfigDataSize;
  static itemdatabase** ItemCategoryData[ITEM_CATEGORIES];
  static int ItemCategorySize[ITEM_CATEGORIES];
  static long ItemCategoryPossibility[ITEM_CATEGORIES];
  static long TotalItemPossibility;
};

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const type* Class)
{
  if(Class)
    Class->Save(SaveFile);
  else
    SaveFile << ushort(0);

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, type*& Class)
{
  int Type = 0;
  SaveFile >> (ushort&)Type;
  Class = Type ? protocontainer<type>::GetProto(Type)->SpawnAndLoad(SaveFile) : 0;
  return SaveFile;
}

#endif
