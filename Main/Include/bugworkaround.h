#ifndef MAIN_INCLUDE_BUGWORKAROUND_H_
#define MAIN_INCLUDE_BUGWORKAROUND_H_


struct bugWorkaroundDupPlayerCharItem{
  character* Char;
  item* it;
};
class bugWorkaroundDupPlayer{
  public:
    static character* BugWorkaroundDupPlayer(character* CharAsked, v2 v2AskedPos);
  private:
    static void GatherAllItemInLevel();
    static void ItemWork(character* Char, item* it, bool bFix, const char* cInfo, std::vector<item*>* pvItem,bool bSendToHell);
    static void FixPlayerDupInv(character* CharChk);

    static void CharEquipmentsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharInventoryWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharBodypartsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharAllItemsWork(character* CharAsked, bool bFix, bool bSendToHell, std::vector<item*>* pvItem=NULL);
    static void CharAllItemsInfo(character* CharAsked);
    static void CharAllItemsCollect(character* CharAsked,std::vector<item*>* pvItem);

    static bool ScanLevelForCharactersAndItemsWork(item*, bool, bool, std::vector<bugWorkaroundDupPlayerCharItem>*);
    static void CollectAllItemsOnLevel(std::vector<item*>* pvAllItemsOnLevel);
    static void CollectAllCharactersOnLevel(std::vector<character*>* pvCharsOnLevel);
    static void CollectAllCharactersAndItemsOnLevel(std::vector<bugWorkaroundDupPlayerCharItem>* pvAllCharAndOrItemsInLevel);
    static bool FindDupItemOnLevel(item* itWork, bool bIgnoreBodyParts, bool bAbortOnMultiples);

    static void AlertConfirmFixMsg(const char* cMsg, bool bAbortIfNot);
//    static bool isItemOnVector(std::vector<item*>* pv, item* e){return (std::find(pv->begin(), pv->end(), e) != pv->end());}
//    static bool isCharOnVector(std::vector<character*>* pv, character* e){return (std::find(pv->begin(), pv->end(), e) != pv->end());}
    static bool Accepted;
};


#endif /* MAIN_INCLUDE_BUGWORKAROUND_H_ */
