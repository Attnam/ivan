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

class cursedDeveloper {
#ifdef CURSEDDEVELOPER
  public:
    static bool IsCursedDeveloper(){return bCursedDeveloper;};
    static bool LifeSaveJustABit(character* Killer);
    static bool BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev);
  private:
    static bool bCursedDeveloper;
#else
  public:
    static bool IsCursedDeveloper(){return false;}
    static bool LifeSaveJustABit(character* Killer){return false;}
#endif
};

#endif //__CURSEDDEVELOPER_H__
