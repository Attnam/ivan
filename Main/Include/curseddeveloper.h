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

class curseddeveloper {
#ifdef CURSEDDEVELOPER
  public:
    static bool IsCursedDeveloper(){return bCursedDeveloper;};
    static bool IsCursedDeveloperTeleport(){return bCursedDeveloperTeleport;}
    static bool LifeSaveJustABit(character* Killer);
    static bool BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev);
    static void RestoreLimbs(festring fsCmdParams);
    static bool HealTorso(bodypart* bp);
    static bool HealBP(int iIndex,int iMode,int iMinHpOK=0);
    static bool CreateBP(int iIndex);
  private:
    static bool bCursedDeveloper;
    static bool bCursedDeveloperTeleport;
#else
  public:
    static bool IsCursedDeveloper(){return false;}
    static bool IsCursedDeveloperTeleport(){return false;}
    static bool LifeSaveJustABit(character* Killer){return false;}
#endif
};

#endif //__CURSEDDEVELOPER_H__
