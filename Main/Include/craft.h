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

#ifndef MAIN_INCLUDE_CRAFT_H_
#define MAIN_INCLUDE_CRAFT_H_

#include <vector>

class craft;
class humanoid;
class item;
class lsquare;
class object;
class olterrain;

struct v2;

//enum craftItemType{
//  CIT_NONE=0,
//  CIT_STONE=1,
//  CIT_PROTOTYPE=2,
//  CIT_POTION=3,
//};
#define  CIT_NONE 0
#define  CIT_STONE 1
#define  CIT_PROTOTYPE 2
#define  CIT_POTION 3

//enum craftTerrainType{
//  CTT_NONE=0,
//  CTT_WALL=1,
//  CTT_DOOR=2,
//  CTT_FURNITURE=3,
//};
#define  CTT_NONE 0
#define  CTT_WALL 1
#define  CTT_DOOR 2
#define  CTT_FURNITURE 3

struct recipework{
};

class recipedata { //TODO split what is savable from what is only work data prior to begin crafting, at recipework above
  private:
    bool bCanBeSuspended;

  public:
    bool IsCanBeSuspended(){return bCanBeSuspended;}
    void SetCanBeSuspended(){bCanBeSuspended=true;}

//    const recipework rpw;

    //TODO only methods should be public, it is currently like that to speed up development ONLY!!! but is a sure source of future problems if kept like that!!!
    //TODO protect: none of these fields should be modified outside this class and every change should be dbgmsg logged.

    // tip: for clarity group by max of 5 no matter group context, but if re-organized, do also at constructor initializer please! btw, save and load will make existing saved games with suspended crafting incompatible
    int Selected; //TODO protect: set only once
    std::vector<ulong> ingredientsIDs;
    int iAddDexterity;

    int iBaseTurnsToFinish;
    bool bSpendCurrentTurn;
    bool bAlreadyExplained;
    int itSpawnTot;
    v2 v2ForgeLocation;

    v2 v2PlaceAt;
    bool bHasAllIngredients;
    bool bCanStart;
    bool bCanBePlaced;

    bool bSuccesfullyCompleted;
    v2 v2AnvilLocation;
    bool bFailed;
    v2 v2PlayerCraftingAt;
//    ulong itSpawnID;

    ulong itToolID;
    v2 v2BuildWhere;
    ulong itSpawnType;
    festring fsItemSpawnSearchPrototype;

    ulong itSpawnCfg;
    ulong itSpawnMatMainCfg;
    ulong itSpawnMatMainVol;
    ulong itSpawnMatSecCfg;
    ulong itSpawnMatSecVol;

    ulong otSpawnCfg;
    ulong otSpawnMatMainCfg;
    ulong otSpawnMatMainVol;
    ulong otSpawnMatSecCfg;
    ulong otSpawnMatSecVol;

    ulong otSpawnType;

//    olterrain* otSpawn; //special save/load case as it can't be placed anywhere in the dungeon level

    item* itTool;
//    item* itSpawn;
    lsquare* lsqrWhere;
    lsquare* lsqrCharPos;

    humanoid* h; //TODO protect: set only once
    character* Actor; //same as humanoid above

  public:
    recipedata(humanoid* H);
    cfestring dbgInfo();
    cfestring id();
//    void SendSpawnItemToHell();
//    void SendTerrainToHell();
    void Save(outputfile& SaveFile) const;
    void Load(inputfile& SaveFile);
    void CopySpawnItemCfgFrom(item* itCfg);
    cfestring SpawnItem();
    void CopySpawnTerrainCfgFrom(olterrain* otCfg);
    cfestring SpawnTerrain();
//    void ClearRefs();
};
class craftcore {
  private:
    static recipedata* prpdSuspended;

  public: //TODO suspendable action should be more global to be reused for other actions than crafting!
    static bool canBeCrafted(item* it);

    static void SetSuspended(recipedata* prpd);
    static void ResetSuspended();
    static bool HasSuspended();
//    static void TerminateSuspended();
    static void ResumeSuspendedTo(character* Char);
};

#endif /* MAIN_INCLUDE_CRAFT_H_ */
