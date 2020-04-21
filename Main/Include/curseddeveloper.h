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
  public:
    static bool BuffAndDebuffPlayerKiller(character* Killer,int& riBuff,int& riDebuff,bool& rbRev);
    static bool LifeSaveJustABit(character* Killer);
    static bool IsCursedDeveloper(){return bCursedDeveloper;};
  private:
    static bool bCursedDeveloper;
};

#endif //__CURSEDDEVELOPER_H__
