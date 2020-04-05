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

#include <ctime>
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
#define CIT_NONE 0 //TODO use, save and load enums?
#define CIT_STONE 1
#define CIT_PROTOTYPE 2
#define CIT_POTION 3
#define CIT_LUMP 4
#define CIT_STICK 5

//enum craftTerrainType{
//  CTT_NONE=0,
//  CTT_WALL=1,
//  CTT_DOOR=2,
//  CTT_FURNITURE=3,
//};
#define CTT_NONE 0 //TODO use, save and load enums?
#define CTT_WALL 1
#define CTT_DOOR 2
#define CTT_FURNITURE 3

class recipecore {
  private:
    clock_t initKey;

    humanoid* h;

    bool bCanBeSuspended;
    int iDungeonLevelID;

  public:
    recipecore(humanoid* H,uint sel);

    void integrityCheck(character* Char=NULL) const;

    void SetHumanoid(character* C);
    humanoid* H(){return h;}

    void SetCannotBeSuspended(){bCanBeSuspended=false;}
    bool IsCanBeSuspended() const {return bCanBeSuspended;}

    int GetDungeonLevelID() const {return iDungeonLevelID;}

    void Save(outputfile& SaveFile) const;
    void Load(inputfile& SaveFile);

    void ClearRefs();
};

//class recipedatacore;
//typedef recipedatacore* (*recipespawner)(character*);
//class recipeprototype
//{
// public:
//  recipeprototype(recipespawner, cchar*);
//  recipedatacore* SpawnAndLoad(inputfile&) const;
//  cchar* GetClassID() const { return ClassID; }
//  int GetIndex() const { return Index; }
// private:
//  int Index;
//  recipespawner Spawner;
//  cchar* ClassID;
//};
//class recipedatacore {
//  public:
//    typedef recipeprototype prototype;
//    virtual const prototype* GetProtoType() const = 0;
//    int GetType() const { return GetProtoType()->GetIndex(); }
//    virtual ~recipedatacore() = default;
//};
//class recipedata : public recipecore {

/**/
class recipedata {
  /**
   * tip: for clarity group fields by max of 5 no matter group context (despite in context would be better)
   */

  friend class craft;
  friend class craftcore;
  friend class crafthandle;

  friend struct recipe;
  friend struct srpFluidsBASE;
  friend struct srpOltBASE;

  friend struct srpChair;
  friend struct srpDoor;
  friend struct srpWall2;
  friend struct srpPoison;
  friend struct srpAcid;

  friend struct srpDismantle;
  friend struct srpSplitLump;
  friend struct srpMelt;
  friend struct srpForgeItem;
  friend struct srpAnvil;

  friend struct srpForge;
  friend struct srpJoinLumps;
  friend struct srpWorkBench;
  friend struct srpTWorkBench;
  friend struct srpResistanceVS;
  friend struct srpCutWeb;
  
  friend struct srpInspect;

  protected:
    recipecore rc;

    //TODO use std::bitset<32> instead of booleans?

    // references
    item* itTool;
    item* itTool2;
    lsquare* lsqrPlaceAt;
    lsquare* lsqrCharPos;
    item* itWeakestIngredient;
    lsquare* lsqrActor;

    // no need to save
    uint SelectedRecipe;
    bool bSpendCurrentTurn;
    bool bAlreadyExplained;
    bool bHasAllIngredients;
    bool bCanStart;

    bool bCanBePlaced;
    int xplodStr;
    int iStrongerXplod;
    v2 v2XplodAt;
    bool bOnlyXplodIfCriticalFumble;

    bool bSpecialExtendedTurns;
    int iMinTurns;
    bool bFailedTerminateCancel;
    bool bFailedSuspend;

    /*******************************************
     * save REQUIRED fields!!!
     * if re-organized, do also at constructor initializer please!
     * but save and load will make existing saved games with suspended crafting incompatible then,
     * so better avoid doing it.
     */
    std::vector<ulong> ingredientsIDs;
    int iAddDexterity;
    int iBaseTurnsToFinish;
    int itSpawnTot;
    v2 v2ForgeLocation;

    v2 v2PlaceAt;
    bool bSuccesfullyCompleted;
    v2 v2AnvilLocation;
    v2 v2PlayerCraftingAt;
    festring fsCraftInfo;

    ulong itToolID;
    ulong itTool2ID;
    ulong itSpawnType;
    festring fsItemSpawnSearchPrototype;

    ulong itSpawnCfg;

    //TODO if duplicating the material works with spoil, rust etc, save THE MATERIAL and load it to dup from. and remove all these vars about'em
    ulong itSpawnMatMainCfg;
    ulong itSpawnMatMainVol;
//    ulong itSpawnMatMainSpoilLevel;

    ulong itSpawnMatSecCfg;
    ulong itSpawnMatSecVol;
//    ulong itSpawnMatSecSpoilLevel;

    ulong otSpawnCfg;
    ulong otSpawnMatMainCfg;
    ulong otSpawnMatMainVol;
    ulong otSpawnMatSecCfg;
    ulong otSpawnMatSecVol;

    ulong otSpawnType;
    bool bSpawnBroken;
    double fDifficulty; //preferably a max of 5.0
    bool bCanBeBroken;
    bool bMeltable;

    v2 v2WorkbenchLocation;
    int iRemainingTurnsToFinish;
    bool bGradativeCraftOverride;
    bool bTailoringMode;
    v2 v2TailoringWorkbenchLocation;

  public:
    recipedata(humanoid* H=NULL,uint sel=FELIST_ERROR_BIT);
    cfestring dbgInfo() const;
    cfestring id() const;
//    void SendSpawnItemToHell();
//    void SendTerrainToHell();

    void Save(outputfile& SaveFile) const;
    void Load(inputfile& SaveFile);
    void CopySpawnItemCfgFrom(item* itCfg);
    void CopySpawnTerrainCfgFrom(olterrain* otCfg);

    void ClearRefs();
    item* GetTool(){return itTool;}
    item* GetTool2(){return itTool2;}

    bool IsFailedSuspendOrCancel(){return bFailedTerminateCancel || bFailedSuspend;}
};

class craftcore {
//  private:
//    static recipedata* prpdSuspended; //TODO suspendable action should be more global to be reused by other actions than crafting!

  public: //TODO review if some should be protected or private
    static void Save(outputfile& SaveFile);
    static void Load(inputfile& SaveFile);
    static truth Craft(character* Char);

    static bool canBeCrafted(item* it);

    static void SendToHellSafely(item* it);

    static int CurrentDungeonLevelID();

    static float CraftSkill(character* Char);
    static bool MoreCraftDeniedFilters(item* it);

    static bool EmptyContentsIfPossible(recipedata& rpd,item* itContainer,bool bMoveToInventory=false);

//    static material* CreateMaterial(bool bMain,recipedata* prpd,material* matOverride=NULL);
    static material* CreateMaterial(bool bMain,recipedata& rpd);
    static material* CreateMaterial(material* matCopyFrom);

    static truth CheckArms(humanoid* h);
    static truth CheckArms(humanoid* h,bool& bLOk,bool& bROk);
    static humanoid* CheckHumanoid(character* Char);

    static bool IsDegraded(item* it,bool bShowMsg=false);
    static truth IsMeltable(material* mat);
    static truth IsMeltable(material* matM,material* matS);
    static truth IsMeltable(item* it);
    static bool IsWooden(material* mat);
    static bool IsBone(material* mat);

    static item* PrepareRemains(recipedata&,material*,int ForceType=CIT_NONE, long volume = 0);
    static void FinishSpawning(recipedata& rpd,item* itSpawn);

    static void AddSuspended(const recipedata& rpd);
    static void RemoveIfSuspended(const recipedata&rpd);
    static bool ResumeSuspendedTo(character* Char,recipedata& rpd);
    static void ClearSuspendedList();
    static bool HasSuspended();

    static recipedata FindRecipedata(festring fsRpdId);
    
    static void CopyDegradation(material* matM,material* matSpM);
    static void CopyDegradation(item* itFrom,material* matTo);
    static void CopyDegradationIfPossible(recipedata& rpd, item* itTo);
    
    static int CitType(item* it);
    static bool CheckFumble(recipedata& rpd, bool& bCriticalFumble,int& iFumblePower);
    static void CraftSkillAdvance(recipedata&);
};

class crafthandle {
  private:
    static void CheckFumble(recipedata& rpd,bool bChangeTurns=true);
    static void CheckIngredients(recipedata& rpd);
    static void CheckFacilities(recipedata& rpd);
    static void CheckTools(recipedata& rpd);

    static void GradativeCraftOverride(recipedata& rpd);

    static item* CheckBreakItem(bool bAllowBreak, recipedata& rpd, item* itSpawn, festring& fsCreated);
    static cfestring DestroyIngredients(recipedata& rpd);

    static item* SpawnItem(recipedata& rpd, festring& fsCreated);
    static olterrain* SpawnTerrain(recipedata& rpd, festring& fsCreated);

  public:
    static void CheckEverything(recipedata& rpd, character* Char);
    static void CraftWorkTurn(recipedata& rpd);
};

#endif /* MAIN_INCLUDE_CRAFT_H_ */
