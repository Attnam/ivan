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

  private:
    static character* BugWorkaroundDupPlayer();
    static void DevConsCmd(std::string strCmdParams);

    static void GatherAllItemInLevel();
    static void ItemWork(character* Char, item* it, bool bFix, const char* cInfo, std::vector<item*>* pvItem,bool bSendToHell);
    static void FixPlayerDupInv(character* CharChk);

    static void CharEquipmentsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharInventoryWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharBodypartsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharAllItemsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharAllItemsInfo(character* CharAsked);
    static void CharAllItemsCollect(character* CharAsked,std::vector<item*>* pvItem);

    static character* FindValidPlayer(v2 ReqPosL,bool bAndFixIt);
    static std::vector<character*> FindPlayersOnLevel();
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
};


#endif /* MAIN_INCLUDE_BUGWORKAROUND_H_ */
