
/* compiled thru cmdcraft.cpp */
// you can modify this file directly or preferably update it's generator (linux) script prepareCraftFilters.sh
bool craftcore::MoreCraftDeniedFilters(item* it){
  // BASE TYPES
  if(dynamic_cast<holybook*>(it)!=NULL)return false;

  if(dynamic_cast<scroll*>(it)!=NULL)return false;

  if(dynamic_cast<bodypart*>(it)!=NULL)return false;

  if(dynamic_cast<torso*>(it)!=NULL)return false;

  if(dynamic_cast<groin*>(it)!=NULL)return false;

  if(dynamic_cast<head*>(it)!=NULL)return false;

  if(dynamic_cast<arm*>(it)!=NULL)return false;

  if(dynamic_cast<leg*>(it)!=NULL)return false;

  // SPECIFIC TYPES
  if(dynamic_cast<acidshield*>(it)!=NULL)return false;

  if(dynamic_cast<amulet*>(it)!=NULL)return false;

  if(dynamic_cast<avatarofvalpurus*>(it)!=NULL)return false;

  if(dynamic_cast<backpack*>(it)!=NULL)return false;

  if(dynamic_cast<banana*>(it)!=NULL)return false;

  if(dynamic_cast<bananapeels*>(it)!=NULL)return false;

  if(dynamic_cast<bansheesickle*>(it)!=NULL)return false;


  if(dynamic_cast<bone*>(it)!=NULL)return false;

  if(dynamic_cast<brokenbottle*>(it)!=NULL)return false;


  if(dynamic_cast<carrot*>(it)!=NULL)return false;


  if(dynamic_cast<chameleonwhip*>(it)!=NULL)return false;

  if(dynamic_cast<chastitybelt*>(it)!=NULL)return false;

  if(dynamic_cast<copyofleftnutofpetrus*>(it)!=NULL)return false;

  if(dynamic_cast<corpse*>(it)!=NULL)return false;

  if(dynamic_cast<daggerofvenom*>(it)!=NULL)return false;

  if(dynamic_cast<darkaxe*>(it)!=NULL)return false;

  if(dynamic_cast<fiftymillionroubles*>(it)!=NULL)return false;

  if(dynamic_cast<firstbornchild*>(it)!=NULL)return false;

  if(dynamic_cast<flamingsword*>(it)!=NULL)return false;

  if(dynamic_cast<gasgrenade*>(it)!=NULL)return false;

  if(dynamic_cast<gastrap*>(it)!=NULL)return false;

  if(dynamic_cast<gorovitscopyoflenin*>(it)!=NULL)return false;

  if(dynamic_cast<gorovitsweapon*>(it)!=NULL)return false;

  if(dynamic_cast<holybanana*>(it)!=NULL)return false;

  if(dynamic_cast<holyhandgrenade*>(it)!=NULL)return false;

  if(dynamic_cast<horn*>(it)!=NULL)return false;


  if(dynamic_cast<justifier*>(it)!=NULL)return false;


  if(dynamic_cast<kiwi*>(it)!=NULL)return false;

  if(dynamic_cast<lantern*>(it)!=NULL)return false;

  if(dynamic_cast<largecorpse*>(it)!=NULL)return false;

  if(dynamic_cast<leftnutofpetrus*>(it)!=NULL)return false;

  if(dynamic_cast<loaf*>(it)!=NULL)return false;


  if(dynamic_cast<magicalwhistle*>(it)!=NULL)return false;

  if(dynamic_cast<mango*>(it)!=NULL)return false;



  if(dynamic_cast<mine*>(it)!=NULL)return false;

  if(dynamic_cast<mjolak*>(it)!=NULL)return false;

  if(dynamic_cast<neercseulb*>(it)!=NULL)return false;

  if(dynamic_cast<nut*>(it)!=NULL)return false;

  if(dynamic_cast<oillamp*>(it)!=NULL)return false;

  if(dynamic_cast<palmbranch*>(it)!=NULL)return false;


  if(dynamic_cast<pineapple*>(it)!=NULL)return false;


  if(dynamic_cast<ring*>(it)!=NULL)return false;

  if(dynamic_cast<rustscythe*>(it)!=NULL)return false;

  if(dynamic_cast<saalthul*>(it)!=NULL)return false;

  if(dynamic_cast<sausage*>(it)!=NULL)return false;

  if(dynamic_cast<sharpaxe*>(it)!=NULL)return false;

  if(dynamic_cast<skull*>(it)!=NULL)return false;

  if(dynamic_cast<skullofxinroch*>(it)!=NULL)return false;

  if(dynamic_cast<slowaxe*>(it)!=NULL)return false;




  if(dynamic_cast<taiaha*>(it)!=NULL)return false;

  if(dynamic_cast<terrorscythe*>(it)!=NULL)return false;

  if(dynamic_cast<thunderhammer*>(it)!=NULL)return false;

  if(dynamic_cast<trinket*>(it)!=NULL)return false;

  if(dynamic_cast<turox*>(it)!=NULL)return false;

  if(dynamic_cast<ullrbone*>(it)!=NULL)return false;

  if(dynamic_cast<vermis*>(it)!=NULL)return false;

  if(dynamic_cast<wand*>(it)!=NULL)return false;

  if(dynamic_cast<weepblade*>(it)!=NULL)return false;




  if(dynamic_cast<wondersmellstaff*>(it)!=NULL)return false;

  return true;
}
