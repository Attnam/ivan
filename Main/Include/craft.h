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
  public:
    //TODO only methods should be public, it is currently like that to speed up development ONLY!!! but is a sure source of future problems if kept like that!!!
    //TODO protect: none of these should be modified outside this class and every change should be dbgmsg logged.

    // tip: for clarity group by 5 no matter group context, but if re-organized, do also at constructor initializer please!!!
    humanoid* h; //TODO protect: set only once
    int Selected; //TODO protect: set only once
    std::vector<ulong> ingredientsIDs;
    bool bCanBeSuspended;
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
    object* craftWhat;

    bool bSuccesfullyCompleted;
    v2 v2AnvilLocation;
    bool bFailed;
    v2 v2PlayerCraftingAt;

    recipedata(humanoid* H);
    cfestring info();
};
class craftcore {
  private:
//    static character* player;
    static craft* craftAction;

  public: //TODO suspendable action should be more global to be reused for other actions than crafting!
//    static void reinitIfNeeded();
    static bool canBeCrafted(item* it);
    static void SetAction(craft* act);
    static bool HasSuspendedAction();
    static void TerminateSuspendedAction();
    static void SetSuspendedActionTo(character* Char);
    static cfestring SuspendedActionInfo();
};

#endif /* MAIN_INCLUDE_CRAFT_H_ */
