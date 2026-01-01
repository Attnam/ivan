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

#ifndef __WIZAUTOPLAY_H__
#define __WIZAUTOPLAY_H__

#define AUTOPLAYMODE_DISABLED 0
#define AUTOPLAYMODE_NOTIMEOUT 1
#define AUTOPLAYMODE_SLOW 2
#define AUTOPLAYMODE_FAST 3
#define AUTOPLAYMODE_FRENZY 4

class wizautoplay
{
  public:
    static int GetMaxValueless(){return iMaxValueless;}
  private:
    static int iMaxValueless;
    
#ifdef WIZARD
  public:
    static void AutoPlayCommandKey(character* C,int& Key,truth& HasActed,truth& ValidKeyPressed);
    static truth AutoPlayAICommand(int&);
    static truth AutoPlayAIPray();
    static bool AutoPlayAIChkInconsistency();
    static void AutoPlayAIDebugDrawSquareRect(v2 v2SqrPos, col16 color, int iPrintIndex=-1, bool bWide=false, bool bKeepColor=false);
    static void AutoPlayAIDebugDrawOverlay();
    static bool AutoPlayAICheckAreaLevelChangedAndReset();
    static truth AutoPlayAIcanApply(item* it);
    static truth AutoPlayAIDropThings();
    static bool IsAutoplayAICanPickup(item* it,bool bPlayerHasLantern);
    static truth AutoPlayAIEquipAndPickup(bool bPlayerHasLantern);
    static int   AutoPlayAIFindWalkDist(v2 v2To);
    static truth AutoPlayAITestValidPathTo(v2 v2To);
    static truth AutoPlayAINavigateDungeon(bool bPlayerHasLantern);
    static truth AutoPlayAISetAndValidateKeepGoingTo(v2 v2KGTo);
    static void AutoPlayAITeleport(bool bDeathCountBased);
    static void AutoPlayAIReset(bool bFailedToo);
    static truth AutoPlayAIequipConsumeZapReadApply();
    static void IncAutoPlayMode();
    static void AutoPlayModeApply();
    static void DisableAutoPlayMode() {AutoPlayMode=AUTOPLAYMODE_DISABLED;AutoPlayModeApply();}
    
    static truth IsPlayerAutoPlay(character* C);
    static int GetAutoPlayMode() { return AutoPlayMode; }
  private:
    static truth IsPlayerAutoPlay(){return IsPlayerAutoPlay(P);};
    static character* P;
    static int AutoPlayMode;
#else
  public:
    static truth IsPlayerAutoPlay(character* C){return false;}
    static int GetAutoPlayMode() { return AUTOPLAYMODE_DISABLED; }
#endif
};

#endif //__WIZAUTOPLAY_H__
