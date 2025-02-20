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

#ifndef __CURSEDDEVELOPER_H__
#define __CURSEDDEVELOPER_H__

class bodypart;
class character;
class festring;

class curseddeveloper {
  public:
    static bool IsCursedDeveloper();
    
#ifdef CURSEDDEVELOPER
  public:
    static void Init();
    static bool IsCursedDeveloperTeleport(){return bCursedDeveloperTeleport;}
    static bool LifeSaveJustABitIfRequested();
    static bool LifeSaveJustABit(character* Killer);
    static long UpdateKillCredit(character* Victim=NULL,int iMod=0);
    static long ModKillCredit(int i){UpdateKillCredit(NULL,i);return lKillCredit;}
    static long GetKillCredit(){return lKillCredit;}
    static void RequestResurrect(character* C){if(!Killer)Killer=C;bResurrect=true;}
  protected:
    static bool BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev);
    static void RestoreLimbs(festring fsCmdParams = CONST_S(""));
    static bool HealTorso(bodypart* bp);
    static bool HealBP(int iIndex,int iMode,int iMinHpOK=0);
    static bool CreateBP(int iIndex);
    static void NightmareWakeUp(character* P);
    static void ResetKillCredit(festring fsCmdParams = CONST_S(""));
  private:
    static bool bCursedDeveloper;
    static bool bCursedDeveloperTeleport;
    static bool bInit;
    static long lKillCredit;
    static bool bNightmareWakeUp;
    static bool bResurrect;
    static character* Killer;
#else
  public:
    static bool IsCursedDeveloperTeleport(){return false;}
#endif
};

#endif //__CURSEDDEVELOPER_H__
