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

class humanoid;
class item;
class lsquare;
class object;
class olterrain;

struct v2;

class recipedata {
  public: //TODO only methods should be public, only like that to speed up dev
    humanoid* h; //TODO protect: set only once
    int Selected; //TODO protect: set only once

    //TODO protect: none of these should be modified outside this class and every change should be dbgmsg logged.
    int iBaseTurnsToFinish;
    item* itTool;
    lsquare* lsqrWhere;
    lsquare* lsqrCharPos;
    olterrain* otSpawn;
    bool bSpendCurrentTurn;
    bool bHasAllIngredients;
    bool bCanStart;
    bool bCanBePlaced;
    bool bAlreadyExplained;
    item* itSpawn;
    int itSpawnTot;
    object* craftWhat;
    v2 v2PlaceAt;
    std::vector<ulong> ingredientsIDs;
//    std::vector<ulong> ingMainIDs;
//    std::vector<ulong> ingSecondaryIDs;

    recipedata(humanoid* H);
};
class craftcore {
  public:
    static bool canBeCrafted(item* it);
};

#endif /* MAIN_INCLUDE_CRAFT_H_ */
