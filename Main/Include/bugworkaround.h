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

#ifndef MAIN_INCLUDE_BUGWORKAROUND_H_
#define MAIN_INCLUDE_BUGWORKAROUND_H_


struct bugWorkaroundDupPlayerCharItem{
  character* Char;
  item* it;
};
class bugfixdp{
  public:
    static void init();
    static character* ValidatePlayerAt(square* sqr);
    static bool IsFixing();

  private:
    static character* BugWorkaroundDupPlayer();
    static void DevConsCmd(festring fsCmdParams);

    static void GatherAllItemInLevel();
    static bool ItemWork(character* Char, item* it, bool bFix, const char* cInfo, std::vector<item*>* pvItem,bool bSendToHell);
    static void FixPlayerDupInv(character* CharChk);

    static int TrapsWork();
    static int CharEquipmentsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static int CharInventoryWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static int CharBodypartsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static int CharAllItemsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharAllItemsInfo(character* CharAsked);
    static void CharAllItemsCollect(character* CharAsked,std::vector<item*>* pvItem);

    static character* FindByPlayerID1(v2 ReqPosL,bool bAndFixIt);
    static std::vector<character*> FindByPlayerFlag();
    static std::vector<character*> FindCharactersOnLevel(bool bOnlyPlayers=false);
    static bool ScanLevelForCharactersAndItemsWork(item*, bool, bool, std::vector<bugWorkaroundDupPlayerCharItem>*);
    static void CollectAllItemsOnLevel(std::vector<item*>* pvAllItemsOnLevel);
    static void CollectAllCharactersOnLevel(std::vector<character*>* pvCharsOnLevel);
    static void CollectAllCharactersAndItemsOnLevel(std::vector<bugWorkaroundDupPlayerCharItem>* pvAllCharAndOrItemsInLevel);
    static bool FindDupItemOnLevel(item* itWork, bool bIgnoreBodyParts, bool bAbortOnMultiples);
    static void ValidateFullLevel();
    static void DupPlayerFix(character*);

    static void DrawAlertConfirmFix(bitmap* Buffer);
    static bool AlertConfirmFixMsg(const char* cMsg);
    static bool IsAlertConfirmFixMsgDraw();
};


#endif /* MAIN_INCLUDE_BUGWORKAROUND_H_ */
