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

class recipedata {
  private:
    bool bCanBeSuspended;

  public:
    bool IsCanBeSuspended(){return bCanBeSuspended;}
    void SetCanBeSuspended(){bCanBeSuspended=true;}

    //TODO only methods should be public, it is currently like that to speed up development ONLY!!! but is a sure source of future problems if kept like that!!!
    //TODO protect: none of these fields should be modified outside this class and every change should be dbgmsg logged.

    // tip: for clarity group by max of 5 no matter group context, but if re-organized, do also at constructor initializer please! (but save and load will make existing saved games with suspended crafting incompatible)
    humanoid* h; //TODO protect: set only once
    int Selected; //TODO protect: set only once
    std::vector<ulong> ingredientsIDs;
    int iAddDexterity;

    int iBaseTurnsToFinish;
    bool bSpendCurrentTurn;
    bool bAlreadyExplained;
    int itSpawnTot;
    v2 v2ForgeLocation;

    item* itTool;
    item* itSpawn;
    olterrain* otSpawn;
    lsquare* lsqrWhere;
    lsquare* lsqrCharPos;

    v2 v2PlaceAt;
    bool bHasAllIngredients;
    bool bCanStart;
    bool bCanBePlaced;

    bool bSuccesfullyCompleted;
    v2 v2AnvilLocation;
    bool bFailed;
    v2 v2PlayerCraftingAt;
    ulong itSpawnID;

    ulong itToolID;
    v2 v2BuildWhere;

  public:
    recipedata(humanoid* H);
    cfestring info();
    void Save(outputfile& SaveFile) const;
    void Load(inputfile& SaveFile);
    void ClearRefs();
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
