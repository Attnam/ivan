/****
 * AUTO-GENERATED CODE FILE, DO NOT MODIFY as modifications will be overwritten !!!
 *
 * After it is generated, update the one at source code path with it and
 * recompile so the results on the abort message (if happens) will be updated !!!
 */

#ifndef _DEFINESVALIDATOR_H_
#define _DEFINESVALIDATOR_H_

#include <string>
#include <algorithm>
#include <cstdarg>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <bitset>
#include <ctime>

#include "error.h"

class definesvalidator{

 public:
  static void init();
  static void DevConsCmd(std::string);
  static void GenerateDefinesValidator(std::string);

 static void Validate() {
  std::stringstream ssErrors;
  std::bitset<32> bsA, bsB;

  
#ifdef AC // DO NOT MODIFY!
  bsA = 8;
  bsB = AC;
  if(bsA!=bsB)
    ssErrors << "Defined AC with value 8 from .dat file mismatches hardcoded c++ define value of " << AC << "!" << std::endl;
#endif 

  
#ifdef ACID // DO NOT MODIFY!
  bsA = 4;
  bsB = ACID;
  if(bsA!=bsB)
    ssErrors << "Defined ACID with value 4 from .dat file mismatches hardcoded c++ define value of " << ACID << "!" << std::endl;
#endif 

  
#ifdef ACIDOUS_BLOOD // DO NOT MODIFY!
  bsA = 16427;
  bsB = ACIDOUS_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined ACIDOUS_BLOOD with value 16427 from .dat file mismatches hardcoded c++ define value of " << ACIDOUS_BLOOD << "!" << std::endl;
#endif 

  
#ifdef ACM // DO NOT MODIFY!
  bsA = 9;
  bsB = ACM;
  if(bsA!=bsB)
    ssErrors << "Defined ACM with value 9 from .dat file mismatches hardcoded c++ define value of " << ACM << "!" << std::endl;
#endif 

  
#ifdef ACMM // DO NOT MODIFY!
  bsA = 10;
  bsB = ACMM;
  if(bsA!=bsB)
    ssErrors << "Defined ACMM with value 10 from .dat file mismatches hardcoded c++ define value of " << ACMM << "!" << std::endl;
#endif 

  
#ifdef ACP // DO NOT MODIFY!
  bsA = 7;
  bsB = ACP;
  if(bsA!=bsB)
    ssErrors << "Defined ACP with value 7 from .dat file mismatches hardcoded c++ define value of " << ACP << "!" << std::endl;
#endif 

  
#ifdef ADAMANT // DO NOT MODIFY!
  bsA = 28679;
  bsB = ADAMANT;
  if(bsA!=bsB)
    ssErrors << "Defined ADAMANT with value 28679 from .dat file mismatches hardcoded c++ define value of " << ADAMANT << "!" << std::endl;
#endif 

  
#ifdef ADULT_FEMALE // DO NOT MODIFY!
  bsA = 5;
  bsB = ADULT_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined ADULT_FEMALE with value 5 from .dat file mismatches hardcoded c++ define value of " << ADULT_FEMALE << "!" << std::endl;
#endif 

  
#ifdef ADULT_MALE // DO NOT MODIFY!
  bsA = 4;
  bsB = ADULT_MALE;
  if(bsA!=bsB)
    ssErrors << "Defined ADULT_MALE with value 4 from .dat file mismatches hardcoded c++ define value of " << ADULT_MALE << "!" << std::endl;
#endif 

  
#ifdef AEGIS_SHIELD // DO NOT MODIFY!
  bsA = 2;
  bsB = AEGIS_SHIELD;
  if(bsA!=bsB)
    ssErrors << "Defined AEGIS_SHIELD with value 2 from .dat file mismatches hardcoded c++ define value of " << AEGIS_SHIELD << "!" << std::endl;
#endif 

  
#ifdef AEGI_SALT // DO NOT MODIFY!
  bsA = 4322;
  bsB = AEGI_SALT;
  if(bsA!=bsB)
    ssErrors << "Defined AEGI_SALT with value 4322 from .dat file mismatches hardcoded c++ define value of " << AEGI_SALT << "!" << std::endl;
#endif 

  
#ifdef AFFECT_INSIDE // DO NOT MODIFY!
  bsA = 8;
  bsB = AFFECT_INSIDE;
  if(bsA!=bsB)
    ssErrors << "Defined AFFECT_INSIDE with value 8 from .dat file mismatches hardcoded c++ define value of " << AFFECT_INSIDE << "!" << std::endl;
#endif 

  
#ifdef AGATE // DO NOT MODIFY!
  bsA = 4268;
  bsB = AGATE;
  if(bsA!=bsB)
    ssErrors << "Defined AGATE with value 4268 from .dat file mismatches hardcoded c++ define value of " << AGATE << "!" << std::endl;
#endif 

  
#ifdef AGILITY // DO NOT MODIFY!
  bsA = 10;
  bsB = AGILITY;
  if(bsA!=bsB)
    ssErrors << "Defined AGILITY with value 10 from .dat file mismatches hardcoded c++ define value of " << AGILITY << "!" << std::endl;
#endif 

  
#ifdef AIR // DO NOT MODIFY!
  bsA = 12289;
  bsB = AIR;
  if(bsA!=bsB)
    ssErrors << "Defined AIR with value 12289 from .dat file mismatches hardcoded c++ define value of " << AIR << "!" << std::endl;
#endif 

  
#ifdef AL // DO NOT MODIFY!
  bsA = 2;
  bsB = AL;
  if(bsA!=bsB)
    ssErrors << "Defined AL with value 2 from .dat file mismatches hardcoded c++ define value of " << AL << "!" << std::endl;
#endif 

  
#ifdef ALABASTER // DO NOT MODIFY!
  bsA = 4323;
  bsB = ALABASTER;
  if(bsA!=bsB)
    ssErrors << "Defined ALABASTER with value 4323 from .dat file mismatches hardcoded c++ define value of " << ALABASTER << "!" << std::endl;
#endif 

  
#ifdef ALL // DO NOT MODIFY!
  bsA = 255;
  bsB = ALL;
  if(bsA!=bsB)
    ssErrors << "Defined ALL with value 255 from .dat file mismatches hardcoded c++ define value of " << ALL << "!" << std::endl;
#endif 

  
#ifdef ALL_DUNGEONS // DO NOT MODIFY!
  bsA = 32767;
  bsB = ALL_DUNGEONS;
  if(bsA!=bsB)
    ssErrors << "Defined ALL_DUNGEONS with value 32767 from .dat file mismatches hardcoded c++ define value of " << ALL_DUNGEONS << "!" << std::endl;
#endif 

  
#ifdef ALM // DO NOT MODIFY!
  bsA = 3;
  bsB = ALM;
  if(bsA!=bsB)
    ssErrors << "Defined ALM with value 3 from .dat file mismatches hardcoded c++ define value of " << ALM << "!" << std::endl;
#endif 

  
#ifdef ALP // DO NOT MODIFY!
  bsA = 1;
  bsB = ALP;
  if(bsA!=bsB)
    ssErrors << "Defined ALP with value 1 from .dat file mismatches hardcoded c++ define value of " << ALP << "!" << std::endl;
#endif 

  
#ifdef ALPP // DO NOT MODIFY!
  bsA = 0;
  bsB = ALPP;
  if(bsA!=bsB)
    ssErrors << "Defined ALPP with value 0 from .dat file mismatches hardcoded c++ define value of " << ALPP << "!" << std::endl;
#endif 

  
#ifdef ALUMINIUM // DO NOT MODIFY!
  bsA = 4120;
  bsB = ALUMINIUM;
  if(bsA!=bsB)
    ssErrors << "Defined ALUMINIUM with value 4120 from .dat file mismatches hardcoded c++ define value of " << ALUMINIUM << "!" << std::endl;
#endif 

  
#ifdef AMBASSADOR_SIREN // DO NOT MODIFY!
  bsA = 10;
  bsB = AMBASSADOR_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined AMBASSADOR_SIREN with value 10 from .dat file mismatches hardcoded c++ define value of " << AMBASSADOR_SIREN << "!" << std::endl;
#endif 

  
#ifdef AMBER // DO NOT MODIFY!
  bsA = 4280;
  bsB = AMBER;
  if(bsA!=bsB)
    ssErrors << "Defined AMBER with value 4280 from .dat file mismatches hardcoded c++ define value of " << AMBER << "!" << std::endl;
#endif 

  
#ifdef AMETHYST // DO NOT MODIFY!
  bsA = 4293;
  bsB = AMETHYST;
  if(bsA!=bsB)
    ssErrors << "Defined AMETHYST with value 4293 from .dat file mismatches hardcoded c++ define value of " << AMETHYST << "!" << std::endl;
#endif 

  
#ifdef AMULET // DO NOT MODIFY!
  bsA = 2;
  bsB = AMULET;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET with value 2 from .dat file mismatches hardcoded c++ define value of " << AMULET << "!" << std::endl;
#endif 

  
#ifdef AMULET_INDEX // DO NOT MODIFY!
  bsA = 1;
  bsB = AMULET_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_INDEX with value 1 from .dat file mismatches hardcoded c++ define value of " << AMULET_INDEX << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_DIMENSION_ANCHOR // DO NOT MODIFY!
  bsA = 8;
  bsB = AMULET_OF_DIMENSION_ANCHOR;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_DIMENSION_ANCHOR with value 8 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_DIMENSION_ANCHOR << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_DISEASE_IMMUNITY // DO NOT MODIFY!
  bsA = 7;
  bsB = AMULET_OF_DISEASE_IMMUNITY;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_DISEASE_IMMUNITY with value 7 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_DISEASE_IMMUNITY << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_ESP // DO NOT MODIFY!
  bsA = 2;
  bsB = AMULET_OF_ESP;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_ESP with value 2 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_ESP << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_FASTING // DO NOT MODIFY!
  bsA = 9;
  bsB = AMULET_OF_FASTING;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_FASTING with value 9 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_FASTING << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_LIFE_SAVING // DO NOT MODIFY!
  bsA = 1;
  bsB = AMULET_OF_LIFE_SAVING;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_LIFE_SAVING with value 1 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_LIFE_SAVING << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_PHASING // DO NOT MODIFY!
  bsA = 6;
  bsB = AMULET_OF_PHASING;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_PHASING with value 6 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_PHASING << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_SPEED // DO NOT MODIFY!
  bsA = 5;
  bsB = AMULET_OF_SPEED;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_SPEED with value 5 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_SPEED << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_UNBREATHING // DO NOT MODIFY!
  bsA = 4;
  bsB = AMULET_OF_UNBREATHING;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_UNBREATHING with value 4 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_UNBREATHING << "!" << std::endl;
#endif 

  
#ifdef AMULET_OF_WARDING // DO NOT MODIFY!
  bsA = 3;
  bsB = AMULET_OF_WARDING;
  if(bsA!=bsB)
    ssErrors << "Defined AMULET_OF_WARDING with value 3 from .dat file mismatches hardcoded c++ define value of " << AMULET_OF_WARDING << "!" << std::endl;
#endif 

  
#ifdef AN // DO NOT MODIFY!
  bsA = 5;
  bsB = AN;
  if(bsA!=bsB)
    ssErrors << "Defined AN with value 5 from .dat file mismatches hardcoded c++ define value of " << AN << "!" << std::endl;
#endif 

  
#ifdef ANGEL_FLESH // DO NOT MODIFY!
  bsA = 20506;
  bsB = ANGEL_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined ANGEL_FLESH with value 20506 from .dat file mismatches hardcoded c++ define value of " << ANGEL_FLESH << "!" << std::endl;
#endif 

  
#ifdef ANGEL_HAIR // DO NOT MODIFY!
  bsA = 4137;
  bsB = ANGEL_HAIR;
  if(bsA!=bsB)
    ssErrors << "Defined ANGEL_HAIR with value 4137 from .dat file mismatches hardcoded c++ define value of " << ANGEL_HAIR << "!" << std::endl;
#endif 

  
#ifdef ANM // DO NOT MODIFY!
  bsA = 6;
  bsB = ANM;
  if(bsA!=bsB)
    ssErrors << "Defined ANM with value 6 from .dat file mismatches hardcoded c++ define value of " << ANM << "!" << std::endl;
#endif 

  
#ifdef ANP // DO NOT MODIFY!
  bsA = 4;
  bsB = ANP;
  if(bsA!=bsB)
    ssErrors << "Defined ANP with value 4 from .dat file mismatches hardcoded c++ define value of " << ANP << "!" << std::endl;
#endif 

  
#ifdef ANTIDOTE_LIQUID // DO NOT MODIFY!
  bsA = 16385;
  bsB = ANTIDOTE_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined ANTIDOTE_LIQUID with value 16385 from .dat file mismatches hardcoded c++ define value of " << ANTIDOTE_LIQUID << "!" << std::endl;
#endif 

  
#ifdef ANVIL // DO NOT MODIFY!
  bsA = 18;
  bsB = ANVIL;
  if(bsA!=bsB)
    ssErrors << "Defined ANVIL with value 18 from .dat file mismatches hardcoded c++ define value of " << ANVIL << "!" << std::endl;
#endif 

  
#ifdef ANY_CATEGORY // DO NOT MODIFY!
  bsA = 2147483647;
  bsB = ANY_CATEGORY;
  if(bsA!=bsB)
    ssErrors << "Defined ANY_CATEGORY with value 2147483647 from .dat file mismatches hardcoded c++ define value of " << ANY_CATEGORY << "!" << std::endl;
#endif 

  
#ifdef ANY_MOVE // DO NOT MODIFY!
  bsA = 15;
  bsB = ANY_MOVE;
  if(bsA!=bsB)
    ssErrors << "Defined ANY_MOVE with value 15 from .dat file mismatches hardcoded c++ define value of " << ANY_MOVE << "!" << std::endl;
#endif 

  
#ifdef APPRENTICE // DO NOT MODIFY!
  bsA = 1;
  bsB = APPRENTICE;
  if(bsA!=bsB)
    ssErrors << "Defined APPRENTICE with value 1 from .dat file mismatches hardcoded c++ define value of " << APPRENTICE << "!" << std::endl;
#endif 

  
#ifdef APPRENTICE_NECROMANCER // DO NOT MODIFY!
  bsA = 1;
  bsB = APPRENTICE_NECROMANCER;
  if(bsA!=bsB)
    ssErrors << "Defined APPRENTICE_NECROMANCER with value 1 from .dat file mismatches hardcoded c++ define value of " << APPRENTICE_NECROMANCER << "!" << std::endl;
#endif 

  
#ifdef ARANEA // DO NOT MODIFY!
  bsA = 3;
  bsB = ARANEA;
  if(bsA!=bsB)
    ssErrors << "Defined ARANEA with value 3 from .dat file mismatches hardcoded c++ define value of " << ARANEA << "!" << std::endl;
#endif 

  
#ifdef ARCANITE // DO NOT MODIFY!
  bsA = 4202;
  bsB = ARCANITE;
  if(bsA!=bsB)
    ssErrors << "Defined ARCANITE with value 4202 from .dat file mismatches hardcoded c++ define value of " << ARCANITE << "!" << std::endl;
#endif 

  
#ifdef ARCH_MAGE // DO NOT MODIFY!
  bsA = 4;
  bsB = ARCH_MAGE;
  if(bsA!=bsB)
    ssErrors << "Defined ARCH_MAGE with value 4 from .dat file mismatches hardcoded c++ define value of " << ARCH_MAGE << "!" << std::endl;
#endif 

  
#ifdef ARMOR_OF_GREAT_HEALTH // DO NOT MODIFY!
  bsA = 3;
  bsB = ARMOR_OF_GREAT_HEALTH;
  if(bsA!=bsB)
    ssErrors << "Defined ARMOR_OF_GREAT_HEALTH with value 3 from .dat file mismatches hardcoded c++ define value of " << ARMOR_OF_GREAT_HEALTH << "!" << std::endl;
#endif 

  
#ifdef ARMS // DO NOT MODIFY!
  bsA = 12;
  bsB = ARMS;
  if(bsA!=bsB)
    ssErrors << "Defined ARMS with value 12 from .dat file mismatches hardcoded c++ define value of " << ARMS << "!" << std::endl;
#endif 

  
#ifdef ARM_CHAIR // DO NOT MODIFY!
  bsA = 7;
  bsB = ARM_CHAIR;
  if(bsA!=bsB)
    ssErrors << "Defined ARM_CHAIR with value 7 from .dat file mismatches hardcoded c++ define value of " << ARM_CHAIR << "!" << std::endl;
#endif 

  
#ifdef ARM_MAIN_COLOR // DO NOT MODIFY!
  bsA = 256;
  bsB = ARM_MAIN_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined ARM_MAIN_COLOR with value 256 from .dat file mismatches hardcoded c++ define value of " << ARM_MAIN_COLOR << "!" << std::endl;
#endif 

  
#ifdef ARM_SPECIAL_COLOR // DO NOT MODIFY!
  bsA = 1024;
  bsB = ARM_SPECIAL_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined ARM_SPECIAL_COLOR with value 1024 from .dat file mismatches hardcoded c++ define value of " << ARM_SPECIAL_COLOR << "!" << std::endl;
#endif 

  
#ifdef ARM_STRENGTH // DO NOT MODIFY!
  bsA = 7;
  bsB = ARM_STRENGTH;
  if(bsA!=bsB)
    ssErrors << "Defined ARM_STRENGTH with value 7 from .dat file mismatches hardcoded c++ define value of " << ARM_STRENGTH << "!" << std::endl;
#endif 

  
#ifdef ARTICLE_BIT // DO NOT MODIFY!
  bsA = 2;
  bsB = ARTICLE_BIT;
  if(bsA!=bsB)
    ssErrors << "Defined ARTICLE_BIT with value 2 from .dat file mismatches hardcoded c++ define value of " << ARTICLE_BIT << "!" << std::endl;
#endif 

  
#ifdef ASH // DO NOT MODIFY!
  bsA = 24585;
  bsB = ASH;
  if(bsA!=bsB)
    ssErrors << "Defined ASH with value 24585 from .dat file mismatches hardcoded c++ define value of " << ASH << "!" << std::endl;
#endif 

  
#ifdef ASH_WOOD // DO NOT MODIFY!
  bsA = 4230;
  bsB = ASH_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined ASH_WOOD with value 4230 from .dat file mismatches hardcoded c++ define value of " << ASH_WOOD << "!" << std::endl;
#endif 

  
#ifdef ASPHALT // DO NOT MODIFY!
  bsA = 16415;
  bsB = ASPHALT;
  if(bsA!=bsB)
    ssErrors << "Defined ASPHALT with value 16415 from .dat file mismatches hardcoded c++ define value of " << ASPHALT << "!" << std::endl;
#endif 

  
#ifdef ASSASSIN // DO NOT MODIFY!
  bsA = 4;
  bsB = ASSASSIN;
  if(bsA!=bsB)
    ssErrors << "Defined ASSASSIN with value 4 from .dat file mismatches hardcoded c++ define value of " << ASSASSIN << "!" << std::endl;
#endif 

  
#ifdef ATAVUS // DO NOT MODIFY!
  bsA = 3;
  bsB = ATAVUS;
  if(bsA!=bsB)
    ssErrors << "Defined ATAVUS with value 3 from .dat file mismatches hardcoded c++ define value of " << ATAVUS << "!" << std::endl;
#endif 

  
#ifdef ATHEIST // DO NOT MODIFY!
  bsA = 16;
  bsB = ATHEIST;
  if(bsA!=bsB)
    ssErrors << "Defined ATHEIST with value 16 from .dat file mismatches hardcoded c++ define value of " << ATHEIST << "!" << std::endl;
#endif 

  
#ifdef ATTACHABLE // DO NOT MODIFY!
  bsA = 24;
  bsB = ATTACHABLE;
  if(bsA!=bsB)
    ssErrors << "Defined ATTACHABLE with value 24 from .dat file mismatches hardcoded c++ define value of " << ATTACHABLE << "!" << std::endl;
#endif 

  
#ifdef ATTNAM // DO NOT MODIFY!
  bsA = 2;
  bsB = ATTNAM;
  if(bsA!=bsB)
    ssErrors << "Defined ATTNAM with value 2 from .dat file mismatches hardcoded c++ define value of " << ATTNAM << "!" << std::endl;
#endif 

  
#ifdef ATTNAM_TEAM // DO NOT MODIFY!
  bsA = 2;
  bsB = ATTNAM_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined ATTNAM_TEAM with value 2 from .dat file mismatches hardcoded c++ define value of " << ATTNAM_TEAM << "!" << std::endl;
#endif 

  
#ifdef ATTRIBUTES // DO NOT MODIFY!
  bsA = 11;
  bsB = ATTRIBUTES;
  if(bsA!=bsB)
    ssErrors << "Defined ATTRIBUTES with value 11 from .dat file mismatches hardcoded c++ define value of " << ATTRIBUTES << "!" << std::endl;
#endif 

  
#ifdef AXE // DO NOT MODIFY!
  bsA = 5;
  bsB = AXE;
  if(bsA!=bsB)
    ssErrors << "Defined AXE with value 5 from .dat file mismatches hardcoded c++ define value of " << AXE << "!" << std::endl;
#endif 

  
#ifdef AXES // DO NOT MODIFY!
  bsA = 7;
  bsB = AXES;
  if(bsA!=bsB)
    ssErrors << "Defined AXES with value 7 from .dat file mismatches hardcoded c++ define value of " << AXES << "!" << std::endl;
#endif 

  
#ifdef BABY_FEMALE // DO NOT MODIFY!
  bsA = 3;
  bsB = BABY_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined BABY_FEMALE with value 3 from .dat file mismatches hardcoded c++ define value of " << BABY_FEMALE << "!" << std::endl;
#endif 

  
#ifdef BABY_MALE // DO NOT MODIFY!
  bsA = 2;
  bsB = BABY_MALE;
  if(bsA!=bsB)
    ssErrors << "Defined BABY_MALE with value 2 from .dat file mismatches hardcoded c++ define value of " << BABY_MALE << "!" << std::endl;
#endif 

  
#ifdef BALSA_WOOD // DO NOT MODIFY!
  bsA = 4237;
  bsB = BALSA_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BALSA_WOOD with value 4237 from .dat file mismatches hardcoded c++ define value of " << BALSA_WOOD << "!" << std::endl;
#endif 

  
#ifdef BAMBOO_WOOD // DO NOT MODIFY!
  bsA = 4235;
  bsB = BAMBOO_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BAMBOO_WOOD with value 4235 from .dat file mismatches hardcoded c++ define value of " << BAMBOO_WOOD << "!" << std::endl;
#endif 

  
#ifdef BANANA_FLESH // DO NOT MODIFY!
  bsA = 8196;
  bsB = BANANA_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BANANA_FLESH with value 8196 from .dat file mismatches hardcoded c++ define value of " << BANANA_FLESH << "!" << std::endl;
#endif 

  
#ifdef BANANA_PEEL // DO NOT MODIFY!
  bsA = 8195;
  bsB = BANANA_PEEL;
  if(bsA!=bsB)
    ssErrors << "Defined BANANA_PEEL with value 8195 from .dat file mismatches hardcoded c++ define value of " << BANANA_PEEL << "!" << std::endl;
#endif 

  
#ifdef BANANA_STOLLEN // DO NOT MODIFY!
  bsA = 8207;
  bsB = BANANA_STOLLEN;
  if(bsA!=bsB)
    ssErrors << "Defined BANANA_STOLLEN with value 8207 from .dat file mismatches hardcoded c++ define value of " << BANANA_STOLLEN << "!" << std::endl;
#endif 

  
#ifdef BANANA_TREE // DO NOT MODIFY!
  bsA = 36;
  bsB = BANANA_TREE;
  if(bsA!=bsB)
    ssErrors << "Defined BANANA_TREE with value 36 from .dat file mismatches hardcoded c++ define value of " << BANANA_TREE << "!" << std::endl;
#endif 

  
#ifdef BARDOOR // DO NOT MODIFY!
  bsA = 1;
  bsB = BARDOOR;
  if(bsA!=bsB)
    ssErrors << "Defined BARDOOR with value 1 from .dat file mismatches hardcoded c++ define value of " << BARDOOR << "!" << std::endl;
#endif 

  
#ifdef BASALT // DO NOT MODIFY!
  bsA = 4264;
  bsB = BASALT;
  if(bsA!=bsB)
    ssErrors << "Defined BASALT with value 4264 from .dat file mismatches hardcoded c++ define value of " << BASALT << "!" << std::endl;
#endif 

  
#ifdef BASE_ATTRIBUTES // DO NOT MODIFY!
  bsA = 7;
  bsB = BASE_ATTRIBUTES;
  if(bsA!=bsB)
    ssErrors << "Defined BASE_ATTRIBUTES with value 7 from .dat file mismatches hardcoded c++ define value of " << BASE_ATTRIBUTES << "!" << std::endl;
#endif 

  
#ifdef BASILISK_SKIN // DO NOT MODIFY!
  bsA = 4146;
  bsB = BASILISK_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined BASILISK_SKIN with value 4146 from .dat file mismatches hardcoded c++ define value of " << BASILISK_SKIN << "!" << std::endl;
#endif 

  
#ifdef BASTARD_SWORD // DO NOT MODIFY!
  bsA = 12;
  bsB = BASTARD_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined BASTARD_SWORD with value 12 from .dat file mismatches hardcoded c++ define value of " << BASTARD_SWORD << "!" << std::endl;
#endif 

  
#ifdef BATTLE_AXE // DO NOT MODIFY!
  bsA = 13;
  bsB = BATTLE_AXE;
  if(bsA!=bsB)
    ssErrors << "Defined BATTLE_AXE with value 13 from .dat file mismatches hardcoded c++ define value of " << BATTLE_AXE << "!" << std::endl;
#endif 

  
#ifdef BATTLE_MAGE // DO NOT MODIFY!
  bsA = 2;
  bsB = BATTLE_MAGE;
  if(bsA!=bsB)
    ssErrors << "Defined BATTLE_MAGE with value 2 from .dat file mismatches hardcoded c++ define value of " << BATTLE_MAGE << "!" << std::endl;
#endif 

  
#ifdef BAT_FLESH // DO NOT MODIFY!
  bsA = 20500;
  bsB = BAT_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BAT_FLESH with value 20500 from .dat file mismatches hardcoded c++ define value of " << BAT_FLESH << "!" << std::endl;
#endif 

  
#ifdef BEAM_ACID_RAIN // DO NOT MODIFY!
  bsA = 11;
  bsB = BEAM_ACID_RAIN;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_ACID_RAIN with value 11 from .dat file mismatches hardcoded c++ define value of " << BEAM_ACID_RAIN << "!" << std::endl;
#endif 

  
#ifdef BEAM_ALCHEMY // DO NOT MODIFY!
  bsA = 14;
  bsB = BEAM_ALCHEMY;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_ALCHEMY with value 14 from .dat file mismatches hardcoded c++ define value of " << BEAM_ALCHEMY << "!" << std::endl;
#endif 

  
#ifdef BEAM_DOOR_CREATION // DO NOT MODIFY!
  bsA = 10;
  bsB = BEAM_DOOR_CREATION;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_DOOR_CREATION with value 10 from .dat file mismatches hardcoded c++ define value of " << BEAM_DOOR_CREATION << "!" << std::endl;
#endif 

  
#ifdef BEAM_DUPLICATE // DO NOT MODIFY!
  bsA = 8;
  bsB = BEAM_DUPLICATE;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_DUPLICATE with value 8 from .dat file mismatches hardcoded c++ define value of " << BEAM_DUPLICATE << "!" << std::endl;
#endif 

  
#ifdef BEAM_FIRE_BALL // DO NOT MODIFY!
  bsA = 2;
  bsB = BEAM_FIRE_BALL;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_FIRE_BALL with value 2 from .dat file mismatches hardcoded c++ define value of " << BEAM_FIRE_BALL << "!" << std::endl;
#endif 

  
#ifdef BEAM_HASTE // DO NOT MODIFY!
  bsA = 4;
  bsB = BEAM_HASTE;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_HASTE with value 4 from .dat file mismatches hardcoded c++ define value of " << BEAM_HASTE << "!" << std::endl;
#endif 

  
#ifdef BEAM_INVISIBILITY // DO NOT MODIFY!
  bsA = 7;
  bsB = BEAM_INVISIBILITY;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_INVISIBILITY with value 7 from .dat file mismatches hardcoded c++ define value of " << BEAM_INVISIBILITY << "!" << std::endl;
#endif 

  
#ifdef BEAM_LIGHTNING // DO NOT MODIFY!
  bsA = 9;
  bsB = BEAM_LIGHTNING;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_LIGHTNING with value 9 from .dat file mismatches hardcoded c++ define value of " << BEAM_LIGHTNING << "!" << std::endl;
#endif 

  
#ifdef BEAM_NECROMANCY // DO NOT MODIFY!
  bsA = 12;
  bsB = BEAM_NECROMANCY;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_NECROMANCY with value 12 from .dat file mismatches hardcoded c++ define value of " << BEAM_NECROMANCY << "!" << std::endl;
#endif 

  
#ifdef BEAM_POLYMORPH // DO NOT MODIFY!
  bsA = 0;
  bsB = BEAM_POLYMORPH;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_POLYMORPH with value 0 from .dat file mismatches hardcoded c++ define value of " << BEAM_POLYMORPH << "!" << std::endl;
#endif 

  
#ifdef BEAM_RESURRECT // DO NOT MODIFY!
  bsA = 6;
  bsB = BEAM_RESURRECT;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_RESURRECT with value 6 from .dat file mismatches hardcoded c++ define value of " << BEAM_RESURRECT << "!" << std::endl;
#endif 

  
#ifdef BEAM_SLOW // DO NOT MODIFY!
  bsA = 5;
  bsB = BEAM_SLOW;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_SLOW with value 5 from .dat file mismatches hardcoded c++ define value of " << BEAM_SLOW << "!" << std::endl;
#endif 

  
#ifdef BEAM_SOFTEN_MATERIAL // DO NOT MODIFY!
  bsA = 15;
  bsB = BEAM_SOFTEN_MATERIAL;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_SOFTEN_MATERIAL with value 15 from .dat file mismatches hardcoded c++ define value of " << BEAM_SOFTEN_MATERIAL << "!" << std::endl;
#endif 

  
#ifdef BEAM_STRIKE // DO NOT MODIFY!
  bsA = 1;
  bsB = BEAM_STRIKE;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_STRIKE with value 1 from .dat file mismatches hardcoded c++ define value of " << BEAM_STRIKE << "!" << std::endl;
#endif 

  
#ifdef BEAM_STYLES // DO NOT MODIFY!
  bsA = 3;
  bsB = BEAM_STYLES;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_STYLES with value 3 from .dat file mismatches hardcoded c++ define value of " << BEAM_STYLES << "!" << std::endl;
#endif 

  
#ifdef BEAM_TELEPORT // DO NOT MODIFY!
  bsA = 3;
  bsB = BEAM_TELEPORT;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_TELEPORT with value 3 from .dat file mismatches hardcoded c++ define value of " << BEAM_TELEPORT << "!" << std::endl;
#endif 

  
#ifdef BEAM_WALL_CREATION // DO NOT MODIFY!
  bsA = 16;
  bsB = BEAM_WALL_CREATION;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_WALL_CREATION with value 16 from .dat file mismatches hardcoded c++ define value of " << BEAM_WALL_CREATION << "!" << std::endl;
#endif 

  
#ifdef BEAM_WEBBING // DO NOT MODIFY!
  bsA = 13;
  bsB = BEAM_WEBBING;
  if(bsA!=bsB)
    ssErrors << "Defined BEAM_WEBBING with value 13 from .dat file mismatches hardcoded c++ define value of " << BEAM_WEBBING << "!" << std::endl;
#endif 

  
#ifdef BEAR_FLESH // DO NOT MODIFY!
  bsA = 20493;
  bsB = BEAR_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BEAR_FLESH with value 20493 from .dat file mismatches hardcoded c++ define value of " << BEAR_FLESH << "!" << std::endl;
#endif 

  
#ifdef BEAR_SKIN // DO NOT MODIFY!
  bsA = 4143;
  bsB = BEAR_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined BEAR_SKIN with value 4143 from .dat file mismatches hardcoded c++ define value of " << BEAR_SKIN << "!" << std::endl;
#endif 

  
#ifdef BEECH_WOOD // DO NOT MODIFY!
  bsA = 4234;
  bsB = BEECH_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BEECH_WOOD with value 4234 from .dat file mismatches hardcoded c++ define value of " << BEECH_WOOD << "!" << std::endl;
#endif 

  
#ifdef BEEF // DO NOT MODIFY!
  bsA = 20482;
  bsB = BEEF;
  if(bsA!=bsB)
    ssErrors << "Defined BEEF with value 20482 from .dat file mismatches hardcoded c++ define value of " << BEEF << "!" << std::endl;
#endif 

  
#ifdef BEER // DO NOT MODIFY!
  bsA = 16404;
  bsB = BEER;
  if(bsA!=bsB)
    ssErrors << "Defined BEER with value 16404 from .dat file mismatches hardcoded c++ define value of " << BEER << "!" << std::endl;
#endif 

  
#ifdef BELT // DO NOT MODIFY!
  bsA = 256;
  bsB = BELT;
  if(bsA!=bsB)
    ssErrors << "Defined BELT with value 256 from .dat file mismatches hardcoded c++ define value of " << BELT << "!" << std::endl;
#endif 

  
#ifdef BELT_COLOR // DO NOT MODIFY!
  bsA = 32;
  bsB = BELT_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_COLOR with value 32 from .dat file mismatches hardcoded c++ define value of " << BELT_COLOR << "!" << std::endl;
#endif 

  
#ifdef BELT_INDEX // DO NOT MODIFY!
  bsA = 4;
  bsB = BELT_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_INDEX with value 4 from .dat file mismatches hardcoded c++ define value of " << BELT_INDEX << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_CARRYING // DO NOT MODIFY!
  bsA = 1;
  bsB = BELT_OF_CARRYING;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_CARRYING with value 1 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_CARRYING << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_GIANT_STRENGTH // DO NOT MODIFY!
  bsA = 4;
  bsB = BELT_OF_GIANT_STRENGTH;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_GIANT_STRENGTH with value 4 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_GIANT_STRENGTH << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_LEVITATION // DO NOT MODIFY!
  bsA = 2;
  bsB = BELT_OF_LEVITATION;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_LEVITATION with value 2 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_LEVITATION << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_PROTECTION // DO NOT MODIFY!
  bsA = 3;
  bsB = BELT_OF_PROTECTION;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_PROTECTION with value 3 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_PROTECTION << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_REGENERATION // DO NOT MODIFY!
  bsA = 6;
  bsB = BELT_OF_REGENERATION;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_REGENERATION with value 6 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_REGENERATION << "!" << std::endl;
#endif 

  
#ifdef BELT_OF_THIEF // DO NOT MODIFY!
  bsA = 5;
  bsB = BELT_OF_THIEF;
  if(bsA!=bsB)
    ssErrors << "Defined BELT_OF_THIEF with value 5 from .dat file mismatches hardcoded c++ define value of " << BELT_OF_THIEF << "!" << std::endl;
#endif 

  
#ifdef BENCH // DO NOT MODIFY!
  bsA = 8;
  bsB = BENCH;
  if(bsA!=bsB)
    ssErrors << "Defined BENCH with value 8 from .dat file mismatches hardcoded c++ define value of " << BENCH << "!" << std::endl;
#endif 

  
#ifdef BERSERKER // DO NOT MODIFY!
  bsA = 1;
  bsB = BERSERKER;
  if(bsA!=bsB)
    ssErrors << "Defined BERSERKER with value 1 from .dat file mismatches hardcoded c++ define value of " << BERSERKER << "!" << std::endl;
#endif 

  
#ifdef BETRAYED_TEAM // DO NOT MODIFY!
  bsA = 11;
  bsB = BETRAYED_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined BETRAYED_TEAM with value 11 from .dat file mismatches hardcoded c++ define value of " << BETRAYED_TEAM << "!" << std::endl;
#endif 

  
#ifdef BIG_MINE // DO NOT MODIFY!
  bsA = 1;
  bsB = BIG_MINE;
  if(bsA!=bsB)
    ssErrors << "Defined BIG_MINE with value 1 from .dat file mismatches hardcoded c++ define value of " << BIG_MINE << "!" << std::endl;
#endif 

  
#ifdef BIRCH // DO NOT MODIFY!
  bsA = 22;
  bsB = BIRCH;
  if(bsA!=bsB)
    ssErrors << "Defined BIRCH with value 22 from .dat file mismatches hardcoded c++ define value of " << BIRCH << "!" << std::endl;
#endif 

  
#ifdef BIRCH_WOOD // DO NOT MODIFY!
  bsA = 4240;
  bsB = BIRCH_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BIRCH_WOOD with value 4240 from .dat file mismatches hardcoded c++ define value of " << BIRCH_WOOD << "!" << std::endl;
#endif 

  
#ifdef BITE // DO NOT MODIFY!
  bsA = 2;
  bsB = BITE;
  if(bsA!=bsB)
    ssErrors << "Defined BITE with value 2 from .dat file mismatches hardcoded c++ define value of " << BITE << "!" << std::endl;
#endif 

  
#ifdef BITE_ATTACK // DO NOT MODIFY!
  bsA = 3;
  bsB = BITE_ATTACK;
  if(bsA!=bsB)
    ssErrors << "Defined BITE_ATTACK with value 3 from .dat file mismatches hardcoded c++ define value of " << BITE_ATTACK << "!" << std::endl;
#endif 

  
#ifdef BLACK // DO NOT MODIFY!
  bsA = 0;
  bsB = BLACK;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK with value 0 from .dat file mismatches hardcoded c++ define value of " << BLACK << "!" << std::endl;
#endif 

  
#ifdef BLACK_BEAR // DO NOT MODIFY!
  bsA = 1;
  bsB = BLACK_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_BEAR with value 1 from .dat file mismatches hardcoded c++ define value of " << BLACK_BEAR << "!" << std::endl;
#endif 

  
#ifdef BLACK_DIAMOND // DO NOT MODIFY!
  bsA = 4299;
  bsB = BLACK_DIAMOND;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_DIAMOND with value 4299 from .dat file mismatches hardcoded c++ define value of " << BLACK_DIAMOND << "!" << std::endl;
#endif 

  
#ifdef BLACK_GRANITE // DO NOT MODIFY!
  bsA = 4275;
  bsB = BLACK_GRANITE;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_GRANITE with value 4275 from .dat file mismatches hardcoded c++ define value of " << BLACK_GRANITE << "!" << std::endl;
#endif 

  
#ifdef BLACK_IRON // DO NOT MODIFY!
  bsA = 28680;
  bsB = BLACK_IRON;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_IRON with value 28680 from .dat file mismatches hardcoded c++ define value of " << BLACK_IRON << "!" << std::endl;
#endif 

  
#ifdef BLACK_JADE // DO NOT MODIFY!
  bsA = 4303;
  bsB = BLACK_JADE;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_JADE with value 4303 from .dat file mismatches hardcoded c++ define value of " << BLACK_JADE << "!" << std::endl;
#endif 

  
#ifdef BLACK_LEATHER // DO NOT MODIFY!
  bsA = 4151;
  bsB = BLACK_LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_LEATHER with value 4151 from .dat file mismatches hardcoded c++ define value of " << BLACK_LEATHER << "!" << std::endl;
#endif 

  
#ifdef BLACK_SAND // DO NOT MODIFY!
  bsA = 24582;
  bsB = BLACK_SAND;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_SAND with value 24582 from .dat file mismatches hardcoded c++ define value of " << BLACK_SAND << "!" << std::endl;
#endif 

  
#ifdef BLACK_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 20510;
  bsB = BLACK_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BLACK_UNICORN_FLESH with value 20510 from .dat file mismatches hardcoded c++ define value of " << BLACK_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef BLINK_DOG_FLESH // DO NOT MODIFY!
  bsA = 20527;
  bsB = BLINK_DOG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BLINK_DOG_FLESH with value 20527 from .dat file mismatches hardcoded c++ define value of " << BLINK_DOG_FLESH << "!" << std::endl;
#endif 

  
#ifdef BLOOD // DO NOT MODIFY!
  bsA = 16419;
  bsB = BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BLOOD with value 16419 from .dat file mismatches hardcoded c++ define value of " << BLOOD << "!" << std::endl;
#endif 

  
#ifdef BLOOD_IRON // DO NOT MODIFY!
  bsA = 28682;
  bsB = BLOOD_IRON;
  if(bsA!=bsB)
    ssErrors << "Defined BLOOD_IRON with value 28682 from .dat file mismatches hardcoded c++ define value of " << BLOOD_IRON << "!" << std::endl;
#endif 

  
#ifdef BLOOD_STEEL // DO NOT MODIFY!
  bsA = 28683;
  bsB = BLOOD_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined BLOOD_STEEL with value 28683 from .dat file mismatches hardcoded c++ define value of " << BLOOD_STEEL << "!" << std::endl;
#endif 

  
#ifdef BLUE // DO NOT MODIFY!
  bsA = 10943;
  bsB = BLUE;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE with value 10943 from .dat file mismatches hardcoded c++ define value of " << BLUE << "!" << std::endl;
#endif 

  
#ifdef BLUE_BLOOD // DO NOT MODIFY!
  bsA = 16421;
  bsB = BLUE_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE_BLOOD with value 16421 from .dat file mismatches hardcoded c++ define value of " << BLUE_BLOOD << "!" << std::endl;
#endif 

  
#ifdef BLUE_CRYSTAL // DO NOT MODIFY!
  bsA = 4310;
  bsB = BLUE_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE_CRYSTAL with value 4310 from .dat file mismatches hardcoded c++ define value of " << BLUE_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef BLUE_FRACTAL // DO NOT MODIFY!
  bsA = 3;
  bsB = BLUE_FRACTAL;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE_FRACTAL with value 3 from .dat file mismatches hardcoded c++ define value of " << BLUE_FRACTAL << "!" << std::endl;
#endif 

  
#ifdef BLUE_JADE // DO NOT MODIFY!
  bsA = 4306;
  bsB = BLUE_JADE;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE_JADE with value 4306 from .dat file mismatches hardcoded c++ define value of " << BLUE_JADE << "!" << std::endl;
#endif 

  
#ifdef BLUE_SIREN // DO NOT MODIFY!
  bsA = 6;
  bsB = BLUE_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined BLUE_SIREN with value 6 from .dat file mismatches hardcoded c++ define value of " << BLUE_SIREN << "!" << std::endl;
#endif 

  
#ifdef BLUNT // DO NOT MODIFY!
  bsA = 1;
  bsB = BLUNT;
  if(bsA!=bsB)
    ssErrors << "Defined BLUNT with value 1 from .dat file mismatches hardcoded c++ define value of " << BLUNT << "!" << std::endl;
#endif 

  
#ifdef BLUNT_WEAPONS // DO NOT MODIFY!
  bsA = 6;
  bsB = BLUNT_WEAPONS;
  if(bsA!=bsB)
    ssErrors << "Defined BLUNT_WEAPONS with value 6 from .dat file mismatches hardcoded c++ define value of " << BLUNT_WEAPONS << "!" << std::endl;
#endif 

  
#ifdef BODY_ARMOR // DO NOT MODIFY!
  bsA = 8;
  bsB = BODY_ARMOR;
  if(bsA!=bsB)
    ssErrors << "Defined BODY_ARMOR with value 8 from .dat file mismatches hardcoded c++ define value of " << BODY_ARMOR << "!" << std::endl;
#endif 

  
#ifdef BODY_ARMOR_INDEX // DO NOT MODIFY!
  bsA = 3;
  bsB = BODY_ARMOR_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined BODY_ARMOR_INDEX with value 3 from .dat file mismatches hardcoded c++ define value of " << BODY_ARMOR_INDEX << "!" << std::endl;
#endif 

  
#ifdef BOIL // DO NOT MODIFY!
  bsA = 2;
  bsB = BOIL;
  if(bsA!=bsB)
    ssErrors << "Defined BOIL with value 2 from .dat file mismatches hardcoded c++ define value of " << BOIL << "!" << std::endl;
#endif 

  
#ifdef BOILED_LEATHER // DO NOT MODIFY!
  bsA = 4153;
  bsB = BOILED_LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined BOILED_LEATHER with value 4153 from .dat file mismatches hardcoded c++ define value of " << BOILED_LEATHER << "!" << std::endl;
#endif 

  
#ifdef BONE // DO NOT MODIFY!
  bsA = 8212;
  bsB = BONE;
  if(bsA!=bsB)
    ssErrors << "Defined BONE with value 8212 from .dat file mismatches hardcoded c++ define value of " << BONE << "!" << std::endl;
#endif 

  
#ifdef BONE_FISH // DO NOT MODIFY!
  bsA = 4;
  bsB = BONE_FISH;
  if(bsA!=bsB)
    ssErrors << "Defined BONE_FISH with value 4 from .dat file mismatches hardcoded c++ define value of " << BONE_FISH << "!" << std::endl;
#endif 

  
#ifdef BONUS_LIVES // DO NOT MODIFY!
  bsA = 0;
  bsB = BONUS_LIVES;
  if(bsA!=bsB)
    ssErrors << "Defined BONUS_LIVES with value 0 from .dat file mismatches hardcoded c++ define value of " << BONUS_LIVES << "!" << std::endl;
#endif 

  
#ifdef BOOK // DO NOT MODIFY!
  bsA = 8192;
  bsB = BOOK;
  if(bsA!=bsB)
    ssErrors << "Defined BOOK with value 8192 from .dat file mismatches hardcoded c++ define value of " << BOOK << "!" << std::endl;
#endif 

  
#ifdef BOOK_CASE // DO NOT MODIFY!
  bsA = 1;
  bsB = BOOK_CASE;
  if(bsA!=bsB)
    ssErrors << "Defined BOOK_CASE with value 1 from .dat file mismatches hardcoded c++ define value of " << BOOK_CASE << "!" << std::endl;
#endif 

  
#ifdef BOOT // DO NOT MODIFY!
  bsA = 512;
  bsB = BOOT;
  if(bsA!=bsB)
    ssErrors << "Defined BOOT with value 512 from .dat file mismatches hardcoded c++ define value of " << BOOT << "!" << std::endl;
#endif 

  
#ifdef BOOT_COLOR // DO NOT MODIFY!
  bsA = 64;
  bsB = BOOT_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined BOOT_COLOR with value 64 from .dat file mismatches hardcoded c++ define value of " << BOOT_COLOR << "!" << std::endl;
#endif 

  
#ifdef BOOT_OF_AGILITY // DO NOT MODIFY!
  bsA = 2;
  bsB = BOOT_OF_AGILITY;
  if(bsA!=bsB)
    ssErrors << "Defined BOOT_OF_AGILITY with value 2 from .dat file mismatches hardcoded c++ define value of " << BOOT_OF_AGILITY << "!" << std::endl;
#endif 

  
#ifdef BOOT_OF_KICKING // DO NOT MODIFY!
  bsA = 3;
  bsB = BOOT_OF_KICKING;
  if(bsA!=bsB)
    ssErrors << "Defined BOOT_OF_KICKING with value 3 from .dat file mismatches hardcoded c++ define value of " << BOOT_OF_KICKING << "!" << std::endl;
#endif 

  
#ifdef BOOT_OF_STRENGTH // DO NOT MODIFY!
  bsA = 1;
  bsB = BOOT_OF_STRENGTH;
  if(bsA!=bsB)
    ssErrors << "Defined BOOT_OF_STRENGTH with value 1 from .dat file mismatches hardcoded c++ define value of " << BOOT_OF_STRENGTH << "!" << std::endl;
#endif 

  
#ifdef BOY // DO NOT MODIFY!
  bsA = 1;
  bsB = BOY;
  if(bsA!=bsB)
    ssErrors << "Defined BOY with value 1 from .dat file mismatches hardcoded c++ define value of " << BOY << "!" << std::endl;
#endif 

  
#ifdef BRASS // DO NOT MODIFY!
  bsA = 4103;
  bsB = BRASS;
  if(bsA!=bsB)
    ssErrors << "Defined BRASS with value 4103 from .dat file mismatches hardcoded c++ define value of " << BRASS << "!" << std::endl;
#endif 

  
#ifdef BRAVERY // DO NOT MODIFY!
  bsA = 1;
  bsB = BRAVERY;
  if(bsA!=bsB)
    ssErrors << "Defined BRAVERY with value 1 from .dat file mismatches hardcoded c++ define value of " << BRAVERY << "!" << std::endl;
#endif 

  
#ifdef BRICK_FINE // DO NOT MODIFY!
  bsA = 1;
  bsB = BRICK_FINE;
  if(bsA!=bsB)
    ssErrors << "Defined BRICK_FINE with value 1 from .dat file mismatches hardcoded c++ define value of " << BRICK_FINE << "!" << std::endl;
#endif 

  
#ifdef BRICK_OLD // DO NOT MODIFY!
  bsA = 3;
  bsB = BRICK_OLD;
  if(bsA!=bsB)
    ssErrors << "Defined BRICK_OLD with value 3 from .dat file mismatches hardcoded c++ define value of " << BRICK_OLD << "!" << std::endl;
#endif 

  
#ifdef BRICK_PRIMITIVE // DO NOT MODIFY!
  bsA = 4;
  bsB = BRICK_PRIMITIVE;
  if(bsA!=bsB)
    ssErrors << "Defined BRICK_PRIMITIVE with value 4 from .dat file mismatches hardcoded c++ define value of " << BRICK_PRIMITIVE << "!" << std::endl;
#endif 

  
#ifdef BRICK_PRIMITIVE_PROPAGANDA // DO NOT MODIFY!
  bsA = 5;
  bsB = BRICK_PRIMITIVE_PROPAGANDA;
  if(bsA!=bsB)
    ssErrors << "Defined BRICK_PRIMITIVE_PROPAGANDA with value 5 from .dat file mismatches hardcoded c++ define value of " << BRICK_PRIMITIVE_PROPAGANDA << "!" << std::endl;
#endif 

  
#ifdef BRICK_PROPAGANDA // DO NOT MODIFY!
  bsA = 2;
  bsB = BRICK_PROPAGANDA;
  if(bsA!=bsB)
    ssErrors << "Defined BRICK_PROPAGANDA with value 2 from .dat file mismatches hardcoded c++ define value of " << BRICK_PROPAGANDA << "!" << std::endl;
#endif 

  
#ifdef BRIM_STONE // DO NOT MODIFY!
  bsA = 4317;
  bsB = BRIM_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined BRIM_STONE with value 4317 from .dat file mismatches hardcoded c++ define value of " << BRIM_STONE << "!" << std::endl;
#endif 

  
#ifdef BRINE // DO NOT MODIFY!
  bsA = 16397;
  bsB = BRINE;
  if(bsA!=bsB)
    ssErrors << "Defined BRINE with value 16397 from .dat file mismatches hardcoded c++ define value of " << BRINE << "!" << std::endl;
#endif 

  
#ifdef BROKEN // DO NOT MODIFY!
  bsA = 128;
  bsB = BROKEN;
  if(bsA!=bsB)
    ssErrors << "Defined BROKEN with value 128 from .dat file mismatches hardcoded c++ define value of " << BROKEN << "!" << std::endl;
#endif 

  
#ifdef BROKEN_BARWALL // DO NOT MODIFY!
  bsA = 1;
  bsB = BROKEN_BARWALL;
  if(bsA!=bsB)
    ssErrors << "Defined BROKEN_BARWALL with value 1 from .dat file mismatches hardcoded c++ define value of " << BROKEN_BARWALL << "!" << std::endl;
#endif 

  
#ifdef BROKEN_LOCK // DO NOT MODIFY!
  bsA = 32768;
  bsB = BROKEN_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined BROKEN_LOCK with value 32768 from .dat file mismatches hardcoded c++ define value of " << BROKEN_LOCK << "!" << std::endl;
#endif 

  
#ifdef BROKEN_WALL // DO NOT MODIFY!
  bsA = 8;
  bsB = BROKEN_WALL;
  if(bsA!=bsB)
    ssErrors << "Defined BROKEN_WALL with value 8 from .dat file mismatches hardcoded c++ define value of " << BROKEN_WALL << "!" << std::endl;
#endif 

  
#ifdef BRONZE // DO NOT MODIFY!
  bsA = 4100;
  bsB = BRONZE;
  if(bsA!=bsB)
    ssErrors << "Defined BRONZE with value 4100 from .dat file mismatches hardcoded c++ define value of " << BRONZE << "!" << std::endl;
#endif 

  
#ifdef BROWN_SLIME // DO NOT MODIFY!
  bsA = 16407;
  bsB = BROWN_SLIME;
  if(bsA!=bsB)
    ssErrors << "Defined BROWN_SLIME with value 16407 from .dat file mismatches hardcoded c++ define value of " << BROWN_SLIME << "!" << std::endl;
#endif 

  
#ifdef BUFFALO_FLESH // DO NOT MODIFY!
  bsA = 20514;
  bsB = BUFFALO_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined BUFFALO_FLESH with value 20514 from .dat file mismatches hardcoded c++ define value of " << BUFFALO_FLESH << "!" << std::endl;
#endif 

  
#ifdef BUFFALO_HORN // DO NOT MODIFY!
  bsA = 4250;
  bsB = BUFFALO_HORN;
  if(bsA!=bsB)
    ssErrors << "Defined BUFFALO_HORN with value 4250 from .dat file mismatches hardcoded c++ define value of " << BUFFALO_HORN << "!" << std::endl;
#endif 

  
#ifdef BUTCHER // DO NOT MODIFY!
  bsA = 2;
  bsB = BUTCHER;
  if(bsA!=bsB)
    ssErrors << "Defined BUTCHER with value 2 from .dat file mismatches hardcoded c++ define value of " << BUTCHER << "!" << std::endl;
#endif 

  
#ifdef Base // DO NOT MODIFY!
  bsA = 0;
  bsB = Base;
  if(bsA!=bsB)
    ssErrors << "Defined Base with value 0 from .dat file mismatches hardcoded c++ define value of " << Base << "!" << std::endl;
#endif 

  
#ifdef CACTUS // DO NOT MODIFY!
  bsA = 20;
  bsB = CACTUS;
  if(bsA!=bsB)
    ssErrors << "Defined CACTUS with value 20 from .dat file mismatches hardcoded c++ define value of " << CACTUS << "!" << std::endl;
#endif 

  
#ifdef CALCITE // DO NOT MODIFY!
  bsA = 4271;
  bsB = CALCITE;
  if(bsA!=bsB)
    ssErrors << "Defined CALCITE with value 4271 from .dat file mismatches hardcoded c++ define value of " << CALCITE << "!" << std::endl;
#endif 

  
#ifdef CAN_BE_DESTROYED // DO NOT MODIFY!
  bsA = 16;
  bsB = CAN_BE_DESTROYED;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BE_DESTROYED with value 16 from .dat file mismatches hardcoded c++ define value of " << CAN_BE_DESTROYED << "!" << std::endl;
#endif 

  
#ifdef CAN_BE_DETECTED // DO NOT MODIFY!
  bsA = 128;
  bsB = CAN_BE_DETECTED;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BE_DETECTED with value 128 from .dat file mismatches hardcoded c++ define value of " << CAN_BE_DETECTED << "!" << std::endl;
#endif 

  
#ifdef CAN_BE_MIRRORED // DO NOT MODIFY!
  bsA = 64;
  bsB = CAN_BE_MIRRORED;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BE_MIRRORED with value 64 from .dat file mismatches hardcoded c++ define value of " << CAN_BE_MIRRORED << "!" << std::endl;
#endif 

  
#ifdef CAN_BE_TAILORED // DO NOT MODIFY!
  bsA = 4;
  bsB = CAN_BE_TAILORED;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BE_TAILORED with value 4 from .dat file mismatches hardcoded c++ define value of " << CAN_BE_TAILORED << "!" << std::endl;
#endif 

  
#ifdef CAN_BE_WISHED // DO NOT MODIFY!
  bsA = 8;
  bsB = CAN_BE_WISHED;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BE_WISHED with value 8 from .dat file mismatches hardcoded c++ define value of " << CAN_BE_WISHED << "!" << std::endl;
#endif 

  
#ifdef CAN_BURN // DO NOT MODIFY!
  bsA = 1;
  bsB = CAN_BURN;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_BURN with value 1 from .dat file mismatches hardcoded c++ define value of " << CAN_BURN << "!" << std::endl;
#endif 

  
#ifdef CAN_DISSOLVE // DO NOT MODIFY!
  bsA = 4;
  bsB = CAN_DISSOLVE;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_DISSOLVE with value 4 from .dat file mismatches hardcoded c++ define value of " << CAN_DISSOLVE << "!" << std::endl;
#endif 

  
#ifdef CAN_EXPLODE // DO NOT MODIFY!
  bsA = 2;
  bsB = CAN_EXPLODE;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_EXPLODE with value 2 from .dat file mismatches hardcoded c++ define value of " << CAN_EXPLODE << "!" << std::endl;
#endif 

  
#ifdef CAN_HAVE_PARASITE // DO NOT MODIFY!
  bsA = 4;
  bsB = CAN_HAVE_PARASITE;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_HAVE_PARASITE with value 4 from .dat file mismatches hardcoded c++ define value of " << CAN_HAVE_PARASITE << "!" << std::endl;
#endif 

  
#ifdef CAN_REGENERATE // DO NOT MODIFY!
  bsA = 16;
  bsB = CAN_REGENERATE;
  if(bsA!=bsB)
    ssErrors << "Defined CAN_REGENERATE with value 16 from .dat file mismatches hardcoded c++ define value of " << CAN_REGENERATE << "!" << std::endl;
#endif 

  
#ifdef CAP_COLOR // DO NOT MODIFY!
  bsA = 2;
  bsB = CAP_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined CAP_COLOR with value 2 from .dat file mismatches hardcoded c++ define value of " << CAP_COLOR << "!" << std::endl;
#endif 

  
#ifdef CARBIDE // DO NOT MODIFY!
  bsA = 4195;
  bsB = CARBIDE;
  if(bsA!=bsB)
    ssErrors << "Defined CARBIDE with value 4195 from .dat file mismatches hardcoded c++ define value of " << CARBIDE << "!" << std::endl;
#endif 

  
#ifdef CARPET // DO NOT MODIFY!
  bsA = 4;
  bsB = CARPET;
  if(bsA!=bsB)
    ssErrors << "Defined CARPET with value 4 from .dat file mismatches hardcoded c++ define value of " << CARPET << "!" << std::endl;
#endif 

  
#ifdef CARROT_FLESH // DO NOT MODIFY!
  bsA = 8200;
  bsB = CARROT_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CARROT_FLESH with value 8200 from .dat file mismatches hardcoded c++ define value of " << CARROT_FLESH << "!" << std::endl;
#endif 

  
#ifdef CAT_FLESH // DO NOT MODIFY!
  bsA = 20504;
  bsB = CAT_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CAT_FLESH with value 20504 from .dat file mismatches hardcoded c++ define value of " << CAT_FLESH << "!" << std::endl;
#endif 

  
#ifdef CAUCASIAN_SIREN // DO NOT MODIFY!
  bsA = 3;
  bsB = CAUCASIAN_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined CAUCASIAN_SIREN with value 3 from .dat file mismatches hardcoded c++ define value of " << CAUCASIAN_SIREN << "!" << std::endl;
#endif 

  
#ifdef CAVE_BEAR // DO NOT MODIFY!
  bsA = 3;
  bsB = CAVE_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined CAVE_BEAR with value 3 from .dat file mismatches hardcoded c++ define value of " << CAVE_BEAR << "!" << std::endl;
#endif 

  
#ifdef CEM_ANTIDOTE // DO NOT MODIFY!
  bsA = 8;
  bsB = CEM_ANTIDOTE;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_ANTIDOTE with value 8 from .dat file mismatches hardcoded c++ define value of " << CEM_ANTIDOTE << "!" << std::endl;
#endif 

  
#ifdef CEM_BLACK_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 12;
  bsB = CEM_BLACK_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_BLACK_UNICORN_FLESH with value 12 from .dat file mismatches hardcoded c++ define value of " << CEM_BLACK_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef CEM_BONE // DO NOT MODIFY!
  bsA = 2;
  bsB = CEM_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_BONE with value 2 from .dat file mismatches hardcoded c++ define value of " << CEM_BONE << "!" << std::endl;
#endif 

  
#ifdef CEM_COCA_COLA // DO NOT MODIFY!
  bsA = 16;
  bsB = CEM_COCA_COLA;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_COCA_COLA with value 16 from .dat file mismatches hardcoded c++ define value of " << CEM_COCA_COLA << "!" << std::endl;
#endif 

  
#ifdef CEM_ESP // DO NOT MODIFY!
  bsA = 9;
  bsB = CEM_ESP;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_ESP with value 9 from .dat file mismatches hardcoded c++ define value of " << CEM_ESP << "!" << std::endl;
#endif 

  
#ifdef CEM_FROG_FLESH // DO NOT MODIFY!
  bsA = 3;
  bsB = CEM_FROG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_FROG_FLESH with value 3 from .dat file mismatches hardcoded c++ define value of " << CEM_FROG_FLESH << "!" << std::endl;
#endif 

  
#ifdef CEM_GRAY_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 13;
  bsB = CEM_GRAY_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_GRAY_UNICORN_FLESH with value 13 from .dat file mismatches hardcoded c++ define value of " << CEM_GRAY_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef CEM_HEALING_LIQUID // DO NOT MODIFY!
  bsA = 7;
  bsB = CEM_HEALING_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_HEALING_LIQUID with value 7 from .dat file mismatches hardcoded c++ define value of " << CEM_HEALING_LIQUID << "!" << std::endl;
#endif 

  
#ifdef CEM_HOLY_BANANA // DO NOT MODIFY!
  bsA = 10;
  bsB = CEM_HOLY_BANANA;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_HOLY_BANANA with value 10 from .dat file mismatches hardcoded c++ define value of " << CEM_HOLY_BANANA << "!" << std::endl;
#endif 

  
#ifdef CEM_KOBOLD_FLESH // DO NOT MODIFY!
  bsA = 6;
  bsB = CEM_KOBOLD_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_KOBOLD_FLESH with value 6 from .dat file mismatches hardcoded c++ define value of " << CEM_KOBOLD_FLESH << "!" << std::endl;
#endif 

  
#ifdef CEM_LIQUID_HORROR // DO NOT MODIFY!
  bsA = 17;
  bsB = CEM_LIQUID_HORROR;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_LIQUID_HORROR with value 17 from .dat file mismatches hardcoded c++ define value of " << CEM_LIQUID_HORROR << "!" << std::endl;
#endif 

  
#ifdef CEM_NOTHING // DO NOT MODIFY!
  bsA = 0;
  bsB = CEM_NOTHING;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_NOTHING with value 0 from .dat file mismatches hardcoded c++ define value of " << CEM_NOTHING << "!" << std::endl;
#endif 

  
#ifdef CEM_OMMEL // DO NOT MODIFY!
  bsA = 4;
  bsB = CEM_OMMEL;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_OMMEL with value 4 from .dat file mismatches hardcoded c++ define value of " << CEM_OMMEL << "!" << std::endl;
#endif 

  
#ifdef CEM_OMMEL_BONE // DO NOT MODIFY!
  bsA = 15;
  bsB = CEM_OMMEL_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_OMMEL_BONE with value 15 from .dat file mismatches hardcoded c++ define value of " << CEM_OMMEL_BONE << "!" << std::endl;
#endif 

  
#ifdef CEM_PEA_SOUP // DO NOT MODIFY!
  bsA = 11;
  bsB = CEM_PEA_SOUP;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_PEA_SOUP with value 11 from .dat file mismatches hardcoded c++ define value of " << CEM_PEA_SOUP << "!" << std::endl;
#endif 

  
#ifdef CEM_PEPSI // DO NOT MODIFY!
  bsA = 5;
  bsB = CEM_PEPSI;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_PEPSI with value 5 from .dat file mismatches hardcoded c++ define value of " << CEM_PEPSI << "!" << std::endl;
#endif 

  
#ifdef CEM_SCHOOL_FOOD // DO NOT MODIFY!
  bsA = 1;
  bsB = CEM_SCHOOL_FOOD;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_SCHOOL_FOOD with value 1 from .dat file mismatches hardcoded c++ define value of " << CEM_SCHOOL_FOOD << "!" << std::endl;
#endif 

  
#ifdef CEM_WHITE_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 14;
  bsB = CEM_WHITE_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CEM_WHITE_UNICORN_FLESH with value 14 from .dat file mismatches hardcoded c++ define value of " << CEM_WHITE_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef CENTER // DO NOT MODIFY!
  bsA = 4;
  bsB = CENTER;
  if(bsA!=bsB)
    ssErrors << "Defined CENTER with value 4 from .dat file mismatches hardcoded c++ define value of " << CENTER << "!" << std::endl;
#endif 

  
#ifdef CHAIN_MAIL // DO NOT MODIFY!
  bsA = 1;
  bsB = CHAIN_MAIL;
  if(bsA!=bsB)
    ssErrors << "Defined CHAIN_MAIL with value 1 from .dat file mismatches hardcoded c++ define value of " << CHAIN_MAIL << "!" << std::endl;
#endif 

  
#ifdef CHAIR // DO NOT MODIFY!
  bsA = 6;
  bsB = CHAIR;
  if(bsA!=bsB)
    ssErrors << "Defined CHAIR with value 6 from .dat file mismatches hardcoded c++ define value of " << CHAIR << "!" << std::endl;
#endif 

  
#ifdef CHALCEDONY // DO NOT MODIFY!
  bsA = 4270;
  bsB = CHALCEDONY;
  if(bsA!=bsB)
    ssErrors << "Defined CHALCEDONY with value 4270 from .dat file mismatches hardcoded c++ define value of " << CHALCEDONY << "!" << std::endl;
#endif 

  
#ifdef CHALK // DO NOT MODIFY!
  bsA = 4276;
  bsB = CHALK;
  if(bsA!=bsB)
    ssErrors << "Defined CHALK with value 4276 from .dat file mismatches hardcoded c++ define value of " << CHALK << "!" << std::endl;
#endif 

  
#ifdef CHAMELEON_FLESH // DO NOT MODIFY!
  bsA = 20518;
  bsB = CHAMELEON_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined CHAMELEON_FLESH with value 20518 from .dat file mismatches hardcoded c++ define value of " << CHAMELEON_FLESH << "!" << std::endl;
#endif 

  
#ifdef CHARISMA // DO NOT MODIFY!
  bsA = 5;
  bsB = CHARISMA;
  if(bsA!=bsB)
    ssErrors << "Defined CHARISMA with value 5 from .dat file mismatches hardcoded c++ define value of " << CHARISMA << "!" << std::endl;
#endif 

  
#ifdef CHAR_COAL // DO NOT MODIFY!
  bsA = 4192;
  bsB = CHAR_COAL;
  if(bsA!=bsB)
    ssErrors << "Defined CHAR_COAL with value 4192 from .dat file mismatches hardcoded c++ define value of " << CHAR_COAL << "!" << std::endl;
#endif 

  
#ifdef CHEAP // DO NOT MODIFY!
  bsA = 1;
  bsB = CHEAP;
  if(bsA!=bsB)
    ssErrors << "Defined CHEAP with value 1 from .dat file mismatches hardcoded c++ define value of " << CHEAP << "!" << std::endl;
#endif 

  
#ifdef CHEAP_BED // DO NOT MODIFY!
  bsA = 10;
  bsB = CHEAP_BED;
  if(bsA!=bsB)
    ssErrors << "Defined CHEAP_BED with value 10 from .dat file mismatches hardcoded c++ define value of " << CHEAP_BED << "!" << std::endl;
#endif 

  
#ifdef CHEESE // DO NOT MODIFY!
  bsA = 8208;
  bsB = CHEESE;
  if(bsA!=bsB)
    ssErrors << "Defined CHEESE with value 8208 from .dat file mismatches hardcoded c++ define value of " << CHEESE << "!" << std::endl;
#endif 

  
#ifdef CHEST // DO NOT MODIFY!
  bsA = 2;
  bsB = CHEST;
  if(bsA!=bsB)
    ssErrors << "Defined CHEST with value 2 from .dat file mismatches hardcoded c++ define value of " << CHEST << "!" << std::endl;
#endif 

  
#ifdef CHEST_OF_DRAWERS // DO NOT MODIFY!
  bsA = 2;
  bsB = CHEST_OF_DRAWERS;
  if(bsA!=bsB)
    ssErrors << "Defined CHEST_OF_DRAWERS with value 2 from .dat file mismatches hardcoded c++ define value of " << CHEST_OF_DRAWERS << "!" << std::endl;
#endif 

  
#ifdef CHICKEN_SOUP // DO NOT MODIFY!
  bsA = 16390;
  bsB = CHICKEN_SOUP;
  if(bsA!=bsB)
    ssErrors << "Defined CHICKEN_SOUP with value 16390 from .dat file mismatches hardcoded c++ define value of " << CHICKEN_SOUP << "!" << std::endl;
#endif 

  
#ifdef CHILD // DO NOT MODIFY!
  bsA = 3;
  bsB = CHILD;
  if(bsA!=bsB)
    ssErrors << "Defined CHILD with value 3 from .dat file mismatches hardcoded c++ define value of " << CHILD << "!" << std::endl;
#endif 

  
#ifdef CHITINE // DO NOT MODIFY!
  bsA = 4249;
  bsB = CHITINE;
  if(bsA!=bsB)
    ssErrors << "Defined CHITINE with value 4249 from .dat file mismatches hardcoded c++ define value of " << CHITINE << "!" << std::endl;
#endif 

  
#ifdef CHLOROPHYTE // DO NOT MODIFY!
  bsA = 4221;
  bsB = CHLOROPHYTE;
  if(bsA!=bsB)
    ssErrors << "Defined CHLOROPHYTE with value 4221 from .dat file mismatches hardcoded c++ define value of " << CHLOROPHYTE << "!" << std::endl;
#endif 

  
#ifdef CHOCOLATE // DO NOT MODIFY!
  bsA = 8209;
  bsB = CHOCOLATE;
  if(bsA!=bsB)
    ssErrors << "Defined CHOCOLATE with value 8209 from .dat file mismatches hardcoded c++ define value of " << CHOCOLATE << "!" << std::endl;
#endif 

  
#ifdef CHROME // DO NOT MODIFY!
  bsA = 4126;
  bsB = CHROME;
  if(bsA!=bsB)
    ssErrors << "Defined CHROME with value 4126 from .dat file mismatches hardcoded c++ define value of " << CHROME << "!" << std::endl;
#endif 

  
#ifdef CIDER // DO NOT MODIFY!
  bsA = 16401;
  bsB = CIDER;
  if(bsA!=bsB)
    ssErrors << "Defined CIDER with value 16401 from .dat file mismatches hardcoded c++ define value of " << CIDER << "!" << std::endl;
#endif 

  
#ifdef CITRINE // DO NOT MODIFY!
  bsA = 4278;
  bsB = CITRINE;
  if(bsA!=bsB)
    ssErrors << "Defined CITRINE with value 4278 from .dat file mismatches hardcoded c++ define value of " << CITRINE << "!" << std::endl;
#endif 

  
#ifdef CLAW // DO NOT MODIFY!
  bsA = 21;
  bsB = CLAW;
  if(bsA!=bsB)
    ssErrors << "Defined CLAW with value 21 from .dat file mismatches hardcoded c++ define value of " << CLAW << "!" << std::endl;
#endif 

  
#ifdef CLAY // DO NOT MODIFY!
  bsA = 4318;
  bsB = CLAY;
  if(bsA!=bsB)
    ssErrors << "Defined CLAY with value 4318 from .dat file mismatches hardcoded c++ define value of " << CLAY << "!" << std::endl;
#endif 

  
#ifdef CLEPTIA // DO NOT MODIFY!
  bsA = 10;
  bsB = CLEPTIA;
  if(bsA!=bsB)
    ssErrors << "Defined CLEPTIA with value 10 from .dat file mismatches hardcoded c++ define value of " << CLEPTIA << "!" << std::endl;
#endif 

  
#ifdef CLOAK // DO NOT MODIFY!
  bsA = 4;
  bsB = CLOAK;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK with value 4 from .dat file mismatches hardcoded c++ define value of " << CLOAK << "!" << std::endl;
#endif 

  
#ifdef CLOAK_INDEX // DO NOT MODIFY!
  bsA = 2;
  bsB = CLOAK_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_INDEX with value 2 from .dat file mismatches hardcoded c++ define value of " << CLOAK_INDEX << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_ACID_RESISTANCE // DO NOT MODIFY!
  bsA = 4;
  bsB = CLOAK_OF_ACID_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_ACID_RESISTANCE with value 4 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_ACID_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_ELECTRICITY_RESISTANCE // DO NOT MODIFY!
  bsA = 3;
  bsB = CLOAK_OF_ELECTRICITY_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_ELECTRICITY_RESISTANCE with value 3 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_ELECTRICITY_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_FIRE_RESISTANCE // DO NOT MODIFY!
  bsA = 2;
  bsB = CLOAK_OF_FIRE_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_FIRE_RESISTANCE with value 2 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_FIRE_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_FLYING // DO NOT MODIFY!
  bsA = 8;
  bsB = CLOAK_OF_FLYING;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_FLYING with value 8 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_FLYING << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_INVISIBILITY // DO NOT MODIFY!
  bsA = 1;
  bsB = CLOAK_OF_INVISIBILITY;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_INVISIBILITY with value 1 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_INVISIBILITY << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_ORCS // DO NOT MODIFY!
  bsA = 7;
  bsB = CLOAK_OF_ORCS;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_ORCS with value 7 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_ORCS << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_PROTECTION // DO NOT MODIFY!
  bsA = 10;
  bsB = CLOAK_OF_PROTECTION;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_PROTECTION with value 10 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_PROTECTION << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_QUICKNESS // DO NOT MODIFY!
  bsA = 9;
  bsB = CLOAK_OF_QUICKNESS;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_QUICKNESS with value 9 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_QUICKNESS << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_SHADOWS // DO NOT MODIFY!
  bsA = 5;
  bsB = CLOAK_OF_SHADOWS;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_SHADOWS with value 5 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_SHADOWS << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_VAMPIRE // DO NOT MODIFY!
  bsA = 11;
  bsB = CLOAK_OF_VAMPIRE;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_VAMPIRE with value 11 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_VAMPIRE << "!" << std::endl;
#endif 

  
#ifdef CLOAK_OF_WEREWOLF // DO NOT MODIFY!
  bsA = 6;
  bsB = CLOAK_OF_WEREWOLF;
  if(bsA!=bsB)
    ssErrors << "Defined CLOAK_OF_WEREWOLF with value 6 from .dat file mismatches hardcoded c++ define value of " << CLOAK_OF_WEREWOLF << "!" << std::endl;
#endif 

  
#ifdef CLOTH_COLOR // DO NOT MODIFY!
  bsA = 2834;
  bsB = CLOTH_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined CLOTH_COLOR with value 2834 from .dat file mismatches hardcoded c++ define value of " << CLOTH_COLOR << "!" << std::endl;
#endif 

  
#ifdef COAL // DO NOT MODIFY!
  bsA = 4193;
  bsB = COAL;
  if(bsA!=bsB)
    ssErrors << "Defined COAL with value 4193 from .dat file mismatches hardcoded c++ define value of " << COAL << "!" << std::endl;
#endif 

  
#ifdef COBALT // DO NOT MODIFY!
  bsA = 4122;
  bsB = COBALT;
  if(bsA!=bsB)
    ssErrors << "Defined COBALT with value 4122 from .dat file mismatches hardcoded c++ define value of " << COBALT << "!" << std::endl;
#endif 

  
#ifdef COBBLE_STONE // DO NOT MODIFY!
  bsA = 4259;
  bsB = COBBLE_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined COBBLE_STONE with value 4259 from .dat file mismatches hardcoded c++ define value of " << COBBLE_STONE << "!" << std::endl;
#endif 

  
#ifdef COCA_COLA // DO NOT MODIFY!
  bsA = 16395;
  bsB = COCA_COLA;
  if(bsA!=bsB)
    ssErrors << "Defined COCA_COLA with value 16395 from .dat file mismatches hardcoded c++ define value of " << COCA_COLA << "!" << std::endl;
#endif 

  
#ifdef COLONIST_TEAM // DO NOT MODIFY!
  bsA = 8;
  bsB = COLONIST_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined COLONIST_TEAM with value 8 from .dat file mismatches hardcoded c++ define value of " << COLONIST_TEAM << "!" << std::endl;
#endif 

  
#ifdef CONCRETE // DO NOT MODIFY!
  bsA = 4313;
  bsB = CONCRETE;
  if(bsA!=bsB)
    ssErrors << "Defined CONCRETE with value 4313 from .dat file mismatches hardcoded c++ define value of " << CONCRETE << "!" << std::endl;
#endif 

  
#ifdef CONFUSED // DO NOT MODIFY!
  bsA = 16384;
  bsB = CONFUSED;
  if(bsA!=bsB)
    ssErrors << "Defined CONFUSED with value 16384 from .dat file mismatches hardcoded c++ define value of " << CONFUSED << "!" << std::endl;
#endif 

  
#ifdef CONFUSION // DO NOT MODIFY!
  bsA = 3;
  bsB = CONFUSION;
  if(bsA!=bsB)
    ssErrors << "Defined CONFUSION with value 3 from .dat file mismatches hardcoded c++ define value of " << CONFUSION << "!" << std::endl;
#endif 

  
#ifdef CONICAL // DO NOT MODIFY!
  bsA = 1;
  bsB = CONICAL;
  if(bsA!=bsB)
    ssErrors << "Defined CONICAL with value 1 from .dat file mismatches hardcoded c++ define value of " << CONICAL << "!" << std::endl;
#endif 

  
#ifdef COPPER // DO NOT MODIFY!
  bsA = 4099;
  bsB = COPPER;
  if(bsA!=bsB)
    ssErrors << "Defined COPPER with value 4099 from .dat file mismatches hardcoded c++ define value of " << COPPER << "!" << std::endl;
#endif 

  
#ifdef CORAL // DO NOT MODIFY!
  bsA = 4286;
  bsB = CORAL;
  if(bsA!=bsB)
    ssErrors << "Defined CORAL with value 4286 from .dat file mismatches hardcoded c++ define value of " << CORAL << "!" << std::endl;
#endif 

  
#ifdef COUCH // DO NOT MODIFY!
  bsA = 5;
  bsB = COUCH;
  if(bsA!=bsB)
    ssErrors << "Defined COUCH with value 5 from .dat file mismatches hardcoded c++ define value of " << COUCH << "!" << std::endl;
#endif 

  
#ifdef CRAZED_FARMER // DO NOT MODIFY!
  bsA = 3;
  bsB = CRAZED_FARMER;
  if(bsA!=bsB)
    ssErrors << "Defined CRAZED_FARMER with value 3 from .dat file mismatches hardcoded c++ define value of " << CRAZED_FARMER << "!" << std::endl;
#endif 

  
#ifdef CREATE_DIVINE_CONFIGURATIONS // DO NOT MODIFY!
  bsA = 4;
  bsB = CREATE_DIVINE_CONFIGURATIONS;
  if(bsA!=bsB)
    ssErrors << "Defined CREATE_DIVINE_CONFIGURATIONS with value 4 from .dat file mismatches hardcoded c++ define value of " << CREATE_DIVINE_CONFIGURATIONS << "!" << std::endl;
#endif 

  
#ifdef CROCODILE_SKIN // DO NOT MODIFY!
  bsA = 4145;
  bsB = CROCODILE_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined CROCODILE_SKIN with value 4145 from .dat file mismatches hardcoded c++ define value of " << CROCODILE_SKIN << "!" << std::endl;
#endif 

  
#ifdef CRUENTUS // DO NOT MODIFY!
  bsA = 14;
  bsB = CRUENTUS;
  if(bsA!=bsB)
    ssErrors << "Defined CRUENTUS with value 14 from .dat file mismatches hardcoded c++ define value of " << CRUENTUS << "!" << std::endl;
#endif 

  
#ifdef CRYSTAL_LEVEL // DO NOT MODIFY!
  bsA = 3;
  bsB = CRYSTAL_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined CRYSTAL_LEVEL with value 3 from .dat file mismatches hardcoded c++ define value of " << CRYSTAL_LEVEL << "!" << std::endl;
#endif 

  
#ifdef CT_BONE // DO NOT MODIFY!
  bsA = 32;
  bsB = CT_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined CT_BONE with value 32 from .dat file mismatches hardcoded c++ define value of " << CT_BONE << "!" << std::endl;
#endif 

  
#ifdef CT_FRUIT // DO NOT MODIFY!
  bsA = 1;
  bsB = CT_FRUIT;
  if(bsA!=bsB)
    ssErrors << "Defined CT_FRUIT with value 1 from .dat file mismatches hardcoded c++ define value of " << CT_FRUIT << "!" << std::endl;
#endif 

  
#ifdef CT_GAS // DO NOT MODIFY!
  bsA = 512;
  bsB = CT_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined CT_GAS with value 512 from .dat file mismatches hardcoded c++ define value of " << CT_GAS << "!" << std::endl;
#endif 

  
#ifdef CT_LIQUID // DO NOT MODIFY!
  bsA = 16;
  bsB = CT_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined CT_LIQUID with value 16 from .dat file mismatches hardcoded c++ define value of " << CT_LIQUID << "!" << std::endl;
#endif 

  
#ifdef CT_MEAT // DO NOT MODIFY!
  bsA = 2;
  bsB = CT_MEAT;
  if(bsA!=bsB)
    ssErrors << "Defined CT_MEAT with value 2 from .dat file mismatches hardcoded c++ define value of " << CT_MEAT << "!" << std::endl;
#endif 

  
#ifdef CT_METAL // DO NOT MODIFY!
  bsA = 4;
  bsB = CT_METAL;
  if(bsA!=bsB)
    ssErrors << "Defined CT_METAL with value 4 from .dat file mismatches hardcoded c++ define value of " << CT_METAL << "!" << std::endl;
#endif 

  
#ifdef CT_MINERAL // DO NOT MODIFY!
  bsA = 8;
  bsB = CT_MINERAL;
  if(bsA!=bsB)
    ssErrors << "Defined CT_MINERAL with value 8 from .dat file mismatches hardcoded c++ define value of " << CT_MINERAL << "!" << std::endl;
#endif 

  
#ifdef CT_MISC_ORGANIC // DO NOT MODIFY!
  bsA = 128;
  bsB = CT_MISC_ORGANIC;
  if(bsA!=bsB)
    ssErrors << "Defined CT_MISC_ORGANIC with value 128 from .dat file mismatches hardcoded c++ define value of " << CT_MISC_ORGANIC << "!" << std::endl;
#endif 

  
#ifdef CT_PLASTIC // DO NOT MODIFY!
  bsA = 256;
  bsB = CT_PLASTIC;
  if(bsA!=bsB)
    ssErrors << "Defined CT_PLASTIC with value 256 from .dat file mismatches hardcoded c++ define value of " << CT_PLASTIC << "!" << std::endl;
#endif 

  
#ifdef CT_PROCESSED // DO NOT MODIFY!
  bsA = 64;
  bsB = CT_PROCESSED;
  if(bsA!=bsB)
    ssErrors << "Defined CT_PROCESSED with value 64 from .dat file mismatches hardcoded c++ define value of " << CT_PROCESSED << "!" << std::endl;
#endif 

  
#ifdef CULTIST // DO NOT MODIFY!
  bsA = 2;
  bsB = CULTIST;
  if(bsA!=bsB)
    ssErrors << "Defined CULTIST with value 2 from .dat file mismatches hardcoded c++ define value of " << CULTIST << "!" << std::endl;
#endif 

  
#ifdef CURE_ALL_LIQUID // DO NOT MODIFY!
  bsA = 16386;
  bsB = CURE_ALL_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined CURE_ALL_LIQUID with value 16386 from .dat file mismatches hardcoded c++ define value of " << CURE_ALL_LIQUID << "!" << std::endl;
#endif 

  
#ifdef CUSTARD // DO NOT MODIFY!
  bsA = 16392;
  bsB = CUSTARD;
  if(bsA!=bsB)
    ssErrors << "Defined CUSTARD with value 16392 from .dat file mismatches hardcoded c++ define value of " << CUSTARD << "!" << std::endl;
#endif 

  
#ifdef CYPRESS_WOOD // DO NOT MODIFY!
  bsA = 4231;
  bsB = CYPRESS_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined CYPRESS_WOOD with value 4231 from .dat file mismatches hardcoded c++ define value of " << CYPRESS_WOOD << "!" << std::endl;
#endif 

  
#ifdef DAEMON_FLESH // DO NOT MODIFY!
  bsA = 20508;
  bsB = DAEMON_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined DAEMON_FLESH with value 20508 from .dat file mismatches hardcoded c++ define value of " << DAEMON_FLESH << "!" << std::endl;
#endif 

  
#ifdef DAGGER // DO NOT MODIFY!
  bsA = 10;
  bsB = DAGGER;
  if(bsA!=bsB)
    ssErrors << "Defined DAGGER with value 10 from .dat file mismatches hardcoded c++ define value of " << DAGGER << "!" << std::endl;
#endif 

  
#ifdef DARK // DO NOT MODIFY!
  bsA = 1;
  bsB = DARK;
  if(bsA!=bsB)
    ssErrors << "Defined DARK with value 1 from .dat file mismatches hardcoded c++ define value of " << DARK << "!" << std::endl;
#endif 

  
#ifdef DARK_ASIAN_SIREN // DO NOT MODIFY!
  bsA = 2;
  bsB = DARK_ASIAN_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_ASIAN_SIREN with value 2 from .dat file mismatches hardcoded c++ define value of " << DARK_ASIAN_SIREN << "!" << std::endl;
#endif 

  
#ifdef DARK_BREAD // DO NOT MODIFY!
  bsA = 8204;
  bsB = DARK_BREAD;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_BREAD with value 8204 from .dat file mismatches hardcoded c++ define value of " << DARK_BREAD << "!" << std::endl;
#endif 

  
#ifdef DARK_FROG_BLOOD // DO NOT MODIFY!
  bsA = 16425;
  bsB = DARK_FROG_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_FROG_BLOOD with value 16425 from .dat file mismatches hardcoded c++ define value of " << DARK_FROG_BLOOD << "!" << std::endl;
#endif 

  
#ifdef DARK_GOLD // DO NOT MODIFY!
  bsA = 4116;
  bsB = DARK_GOLD;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_GOLD with value 4116 from .dat file mismatches hardcoded c++ define value of " << DARK_GOLD << "!" << std::endl;
#endif 

  
#ifdef DARK_GRASS // DO NOT MODIFY!
  bsA = 4214;
  bsB = DARK_GRASS;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_GRASS with value 4214 from .dat file mismatches hardcoded c++ define value of " << DARK_GRASS << "!" << std::endl;
#endif 

  
#ifdef DARK_GRASS_TERRAIN // DO NOT MODIFY!
  bsA = 7;
  bsB = DARK_GRASS_TERRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_GRASS_TERRAIN with value 7 from .dat file mismatches hardcoded c++ define value of " << DARK_GRASS_TERRAIN << "!" << std::endl;
#endif 

  
#ifdef DARK_GRAY // DO NOT MODIFY!
  bsA = 21130;
  bsB = DARK_GRAY;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_GRAY with value 21130 from .dat file mismatches hardcoded c++ define value of " << DARK_GRAY << "!" << std::endl;
#endif 

  
#ifdef DARK_LEVEL // DO NOT MODIFY!
  bsA = 8;
  bsB = DARK_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_LEVEL with value 8 from .dat file mismatches hardcoded c++ define value of " << DARK_LEVEL << "!" << std::endl;
#endif 

  
#ifdef DARK_MATTER // DO NOT MODIFY!
  bsA = 4117;
  bsB = DARK_MATTER;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_MATTER with value 4117 from .dat file mismatches hardcoded c++ define value of " << DARK_MATTER << "!" << std::endl;
#endif 

  
#ifdef DARK_PETAL // DO NOT MODIFY!
  bsA = 4219;
  bsB = DARK_PETAL;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_PETAL with value 4219 from .dat file mismatches hardcoded c++ define value of " << DARK_PETAL << "!" << std::endl;
#endif 

  
#ifdef DARK_SIREN // DO NOT MODIFY!
  bsA = 4;
  bsB = DARK_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined DARK_SIREN with value 4 from .dat file mismatches hardcoded c++ define value of " << DARK_SIREN << "!" << std::endl;
#endif 

  
#ifdef DEAD_FISH // DO NOT MODIFY!
  bsA = 3;
  bsB = DEAD_FISH;
  if(bsA!=bsB)
    ssErrors << "Defined DEAD_FISH with value 3 from .dat file mismatches hardcoded c++ define value of " << DEAD_FISH << "!" << std::endl;
#endif 

  
#ifdef DEAD_TREE // DO NOT MODIFY!
  bsA = 37;
  bsB = DEAD_TREE;
  if(bsA!=bsB)
    ssErrors << "Defined DEAD_TREE with value 37 from .dat file mismatches hardcoded c++ define value of " << DEAD_TREE << "!" << std::endl;
#endif 

  
#ifdef DEEP_BRONZE // DO NOT MODIFY!
  bsA = 4102;
  bsB = DEEP_BRONZE;
  if(bsA!=bsB)
    ssErrors << "Defined DEEP_BRONZE with value 4102 from .dat file mismatches hardcoded c++ define value of " << DEEP_BRONZE << "!" << std::endl;
#endif 

  
#ifdef DEEP_CORAL // DO NOT MODIFY!
  bsA = 4287;
  bsB = DEEP_CORAL;
  if(bsA!=bsB)
    ssErrors << "Defined DEEP_CORAL with value 4287 from .dat file mismatches hardcoded c++ define value of " << DEEP_CORAL << "!" << std::endl;
#endif 

  
#ifdef DEFAULT_TEAM // DO NOT MODIFY!
  bsA = 255;
  bsB = DEFAULT_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined DEFAULT_TEAM with value 255 from .dat file mismatches hardcoded c++ define value of " << DEFAULT_TEAM << "!" << std::endl;
#endif 

  
#ifdef DEFINITE // DO NOT MODIFY!
  bsA = 2;
  bsB = DEFINITE;
  if(bsA!=bsB)
    ssErrors << "Defined DEFINITE with value 2 from .dat file mismatches hardcoded c++ define value of " << DEFINITE << "!" << std::endl;
#endif 

  
#ifdef DEPENDS_ON_ATTRIBUTES // DO NOT MODIFY!
  bsA = 65535;
  bsB = DEPENDS_ON_ATTRIBUTES;
  if(bsA!=bsB)
    ssErrors << "Defined DEPENDS_ON_ATTRIBUTES with value 65535 from .dat file mismatches hardcoded c++ define value of " << DEPENDS_ON_ATTRIBUTES << "!" << std::endl;
#endif 

  
#ifdef DESERT // DO NOT MODIFY!
  bsA = 1;
  bsB = DESERT;
  if(bsA!=bsB)
    ssErrors << "Defined DESERT with value 1 from .dat file mismatches hardcoded c++ define value of " << DESERT << "!" << std::endl;
#endif 

  
#ifdef DESK // DO NOT MODIFY!
  bsA = 31;
  bsB = DESK;
  if(bsA!=bsB)
    ssErrors << "Defined DESK with value 31 from .dat file mismatches hardcoded c++ define value of " << DESK << "!" << std::endl;
#endif 

  
#ifdef DETECTING // DO NOT MODIFY!
  bsA = 16777216;
  bsB = DETECTING;
  if(bsA!=bsB)
    ssErrors << "Defined DETECTING with value 16777216 from .dat file mismatches hardcoded c++ define value of " << DETECTING << "!" << std::endl;
#endif 

  
#ifdef DEXTERITY // DO NOT MODIFY!
  bsA = 9;
  bsB = DEXTERITY;
  if(bsA!=bsB)
    ssErrors << "Defined DEXTERITY with value 9 from .dat file mismatches hardcoded c++ define value of " << DEXTERITY << "!" << std::endl;
#endif 

  
#ifdef DIAMOND // DO NOT MODIFY!
  bsA = 4298;
  bsB = DIAMOND;
  if(bsA!=bsB)
    ssErrors << "Defined DIAMOND with value 4298 from .dat file mismatches hardcoded c++ define value of " << DIAMOND << "!" << std::endl;
#endif 

  
#ifdef DIRT // DO NOT MODIFY!
  bsA = 24583;
  bsB = DIRT;
  if(bsA!=bsB)
    ssErrors << "Defined DIRT with value 24583 from .dat file mismatches hardcoded c++ define value of " << DIRT << "!" << std::endl;
#endif 

  
#ifdef DISEASE_IMMUNITY // DO NOT MODIFY!
  bsA = 134217728;
  bsB = DISEASE_IMMUNITY;
  if(bsA!=bsB)
    ssErrors << "Defined DISEASE_IMMUNITY with value 134217728 from .dat file mismatches hardcoded c++ define value of " << DISEASE_IMMUNITY << "!" << std::endl;
#endif 

  
#ifdef DOG_DROOL // DO NOT MODIFY!
  bsA = 16417;
  bsB = DOG_DROOL;
  if(bsA!=bsB)
    ssErrors << "Defined DOG_DROOL with value 16417 from .dat file mismatches hardcoded c++ define value of " << DOG_DROOL << "!" << std::endl;
#endif 

  
#ifdef DOG_FLESH // DO NOT MODIFY!
  bsA = 20495;
  bsB = DOG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined DOG_FLESH with value 20495 from .dat file mismatches hardcoded c++ define value of " << DOG_FLESH << "!" << std::endl;
#endif 

  
#ifdef DOLPHIN_FLESH // DO NOT MODIFY!
  bsA = 20492;
  bsB = DOLPHIN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined DOLPHIN_FLESH with value 20492 from .dat file mismatches hardcoded c++ define value of " << DOLPHIN_FLESH << "!" << std::endl;
#endif 

  
#ifdef DONT_CHANGE_EQUIPMENT // DO NOT MODIFY!
  bsA = 4;
  bsB = DONT_CHANGE_EQUIPMENT;
  if(bsA!=bsB)
    ssErrors << "Defined DONT_CHANGE_EQUIPMENT with value 4 from .dat file mismatches hardcoded c++ define value of " << DONT_CHANGE_EQUIPMENT << "!" << std::endl;
#endif 

  
#ifdef DONT_CONSUME_ANYTHING_VALUABLE // DO NOT MODIFY!
  bsA = 8;
  bsB = DONT_CONSUME_ANYTHING_VALUABLE;
  if(bsA!=bsB)
    ssErrors << "Defined DONT_CONSUME_ANYTHING_VALUABLE with value 8 from .dat file mismatches hardcoded c++ define value of " << DONT_CONSUME_ANYTHING_VALUABLE << "!" << std::endl;
#endif 

  
#ifdef DOUBLE_BED // DO NOT MODIFY!
  bsA = 9;
  bsB = DOUBLE_BED;
  if(bsA!=bsB)
    ssErrors << "Defined DOUBLE_BED with value 9 from .dat file mismatches hardcoded c++ define value of " << DOUBLE_BED << "!" << std::endl;
#endif 

  
#ifdef DOWN // DO NOT MODIFY!
  bsA = 1;
  bsB = DOWN;
  if(bsA!=bsB)
    ssErrors << "Defined DOWN with value 1 from .dat file mismatches hardcoded c++ define value of " << DOWN << "!" << std::endl;
#endif 

  
#ifdef DRAGON_BONE // DO NOT MODIFY!
  bsA = 4255;
  bsB = DRAGON_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined DRAGON_BONE with value 4255 from .dat file mismatches hardcoded c++ define value of " << DRAGON_BONE << "!" << std::endl;
#endif 

  
#ifdef DRAGON_HIDE // DO NOT MODIFY!
  bsA = 4158;
  bsB = DRAGON_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined DRAGON_HIDE with value 4158 from .dat file mismatches hardcoded c++ define value of " << DRAGON_HIDE << "!" << std::endl;
#endif 

  
#ifdef DRAIN // DO NOT MODIFY!
  bsA = 128;
  bsB = DRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined DRAIN with value 128 from .dat file mismatches hardcoded c++ define value of " << DRAIN << "!" << std::endl;
#endif 

  
#ifdef DREAM_CLOTH // DO NOT MODIFY!
  bsA = 4173;
  bsB = DREAM_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined DREAM_CLOTH with value 4173 from .dat file mismatches hardcoded c++ define value of " << DREAM_CLOTH << "!" << std::endl;
#endif 

  
#ifdef DRUID // DO NOT MODIFY!
  bsA = 1;
  bsB = DRUID;
  if(bsA!=bsB)
    ssErrors << "Defined DRUID with value 1 from .dat file mismatches hardcoded c++ define value of " << DRUID << "!" << std::endl;
#endif 

  
#ifdef DUAL_ENNER_BEAST_LEVEL // DO NOT MODIFY!
  bsA = 5;
  bsB = DUAL_ENNER_BEAST_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined DUAL_ENNER_BEAST_LEVEL with value 5 from .dat file mismatches hardcoded c++ define value of " << DUAL_ENNER_BEAST_LEVEL << "!" << std::endl;
#endif 

  
#ifdef DULCIS // DO NOT MODIFY!
  bsA = 4;
  bsB = DULCIS;
  if(bsA!=bsB)
    ssErrors << "Defined DULCIS with value 4 from .dat file mismatches hardcoded c++ define value of " << DULCIS << "!" << std::endl;
#endif 

  
#ifdef DURALLOY // DO NOT MODIFY!
  bsA = 4190;
  bsB = DURALLOY;
  if(bsA!=bsB)
    ssErrors << "Defined DURALLOY with value 4190 from .dat file mismatches hardcoded c++ define value of " << DURALLOY << "!" << std::endl;
#endif 

  
#ifdef DWARF_BEER // DO NOT MODIFY!
  bsA = 16406;
  bsB = DWARF_BEER;
  if(bsA!=bsB)
    ssErrors << "Defined DWARF_BEER with value 16406 from .dat file mismatches hardcoded c++ define value of " << DWARF_BEER << "!" << std::endl;
#endif 

  
#ifdef DWARF_BIRCH // DO NOT MODIFY!
  bsA = 24;
  bsB = DWARF_BIRCH;
  if(bsA!=bsB)
    ssErrors << "Defined DWARF_BIRCH with value 24 from .dat file mismatches hardcoded c++ define value of " << DWARF_BIRCH << "!" << std::endl;
#endif 

  
#ifdef DWARF_FLESH // DO NOT MODIFY!
  bsA = 20507;
  bsB = DWARF_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined DWARF_FLESH with value 20507 from .dat file mismatches hardcoded c++ define value of " << DWARF_FLESH << "!" << std::endl;
#endif 

  
#ifdef DWARF_STEEL // DO NOT MODIFY!
  bsA = 28676;
  bsB = DWARF_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined DWARF_STEEL with value 28676 from .dat file mismatches hardcoded c++ define value of " << DWARF_STEEL << "!" << std::endl;
#endif 

  
#ifdef EBONY_WOOD // DO NOT MODIFY!
  bsA = 4243;
  bsB = EBONY_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined EBONY_WOOD with value 4243 from .dat file mismatches hardcoded c++ define value of " << EBONY_WOOD << "!" << std::endl;
#endif 

  
#ifdef ECTOPLASM // DO NOT MODIFY!
  bsA = 12294;
  bsB = ECTOPLASM;
  if(bsA!=bsB)
    ssErrors << "Defined ECTOPLASM with value 12294 from .dat file mismatches hardcoded c++ define value of " << ECTOPLASM << "!" << std::endl;
#endif 

  
#ifdef EFFECT_ANTIDOTE // DO NOT MODIFY!
  bsA = 9;
  bsB = EFFECT_ANTIDOTE;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_ANTIDOTE with value 9 from .dat file mismatches hardcoded c++ define value of " << EFFECT_ANTIDOTE << "!" << std::endl;
#endif 

  
#ifdef EFFECT_BLACK_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 20;
  bsB = EFFECT_BLACK_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_BLACK_UNICORN_FLESH with value 20 from .dat file mismatches hardcoded c++ define value of " << EFFECT_BLACK_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef EFFECT_CONFUSE // DO NOT MODIFY!
  bsA = 10;
  bsB = EFFECT_CONFUSE;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_CONFUSE with value 10 from .dat file mismatches hardcoded c++ define value of " << EFFECT_CONFUSE << "!" << std::endl;
#endif 

  
#ifdef EFFECT_DARKNESS // DO NOT MODIFY!
  bsA = 2;
  bsB = EFFECT_DARKNESS;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_DARKNESS with value 2 from .dat file mismatches hardcoded c++ define value of " << EFFECT_DARKNESS << "!" << std::endl;
#endif 

  
#ifdef EFFECT_ESP // DO NOT MODIFY!
  bsA = 12;
  bsB = EFFECT_ESP;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_ESP with value 12 from .dat file mismatches hardcoded c++ define value of " << EFFECT_ESP << "!" << std::endl;
#endif 

  
#ifdef EFFECT_EVIL_WONDER_STAFF_VAPOUR // DO NOT MODIFY!
  bsA = 17;
  bsB = EFFECT_EVIL_WONDER_STAFF_VAPOUR;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_EVIL_WONDER_STAFF_VAPOUR with value 17 from .dat file mismatches hardcoded c++ define value of " << EFFECT_EVIL_WONDER_STAFF_VAPOUR << "!" << std::endl;
#endif 

  
#ifdef EFFECT_GOOD_WONDER_STAFF_VAPOUR // DO NOT MODIFY!
  bsA = 18;
  bsB = EFFECT_GOOD_WONDER_STAFF_VAPOUR;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_GOOD_WONDER_STAFF_VAPOUR with value 18 from .dat file mismatches hardcoded c++ define value of " << EFFECT_GOOD_WONDER_STAFF_VAPOUR << "!" << std::endl;
#endif 

  
#ifdef EFFECT_GRAY_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 21;
  bsB = EFFECT_GRAY_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_GRAY_UNICORN_FLESH with value 21 from .dat file mismatches hardcoded c++ define value of " << EFFECT_GRAY_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef EFFECT_HEAL // DO NOT MODIFY!
  bsA = 6;
  bsB = EFFECT_HEAL;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_HEAL with value 6 from .dat file mismatches hardcoded c++ define value of " << EFFECT_HEAL << "!" << std::endl;
#endif 

  
#ifdef EFFECT_HOLY_BANANA // DO NOT MODIFY!
  bsA = 16;
  bsB = EFFECT_HOLY_BANANA;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_HOLY_BANANA with value 16 from .dat file mismatches hardcoded c++ define value of " << EFFECT_HOLY_BANANA << "!" << std::endl;
#endif 

  
#ifdef EFFECT_IS_GOOD // DO NOT MODIFY!
  bsA = 16;
  bsB = EFFECT_IS_GOOD;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_IS_GOOD with value 16 from .dat file mismatches hardcoded c++ define value of " << EFFECT_IS_GOOD << "!" << std::endl;
#endif 

  
#ifdef EFFECT_KOBOLD_FLESH // DO NOT MODIFY!
  bsA = 5;
  bsB = EFFECT_KOBOLD_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_KOBOLD_FLESH with value 5 from .dat file mismatches hardcoded c++ define value of " << EFFECT_KOBOLD_FLESH << "!" << std::endl;
#endif 

  
#ifdef EFFECT_LYCANTHROPY // DO NOT MODIFY!
  bsA = 7;
  bsB = EFFECT_LYCANTHROPY;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_LYCANTHROPY with value 7 from .dat file mismatches hardcoded c++ define value of " << EFFECT_LYCANTHROPY << "!" << std::endl;
#endif 

  
#ifdef EFFECT_MAGIC_MUSHROOM // DO NOT MODIFY!
  bsA = 14;
  bsB = EFFECT_MAGIC_MUSHROOM;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_MAGIC_MUSHROOM with value 14 from .dat file mismatches hardcoded c++ define value of " << EFFECT_MAGIC_MUSHROOM << "!" << std::endl;
#endif 

  
#ifdef EFFECT_MUSHROOM // DO NOT MODIFY!
  bsA = 24;
  bsB = EFFECT_MUSHROOM;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_MUSHROOM with value 24 from .dat file mismatches hardcoded c++ define value of " << EFFECT_MUSHROOM << "!" << std::endl;
#endif 

  
#ifdef EFFECT_MUSTARD_GAS // DO NOT MODIFY!
  bsA = 30;
  bsB = EFFECT_MUSTARD_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_MUSTARD_GAS with value 30 from .dat file mismatches hardcoded c++ define value of " << EFFECT_MUSTARD_GAS << "!" << std::endl;
#endif 

  
#ifdef EFFECT_MUSTARD_GAS_LIQUID // DO NOT MODIFY!
  bsA = 31;
  bsB = EFFECT_MUSTARD_GAS_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_MUSTARD_GAS_LIQUID with value 31 from .dat file mismatches hardcoded c++ define value of " << EFFECT_MUSTARD_GAS_LIQUID << "!" << std::endl;
#endif 

  
#ifdef EFFECT_NOTHING // DO NOT MODIFY!
  bsA = 0;
  bsB = EFFECT_NOTHING;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_NOTHING with value 0 from .dat file mismatches hardcoded c++ define value of " << EFFECT_NOTHING << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_BLOOD // DO NOT MODIFY!
  bsA = 34;
  bsB = EFFECT_OMMEL_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_BLOOD with value 34 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_BLOOD << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_BONE // DO NOT MODIFY!
  bsA = 29;
  bsB = EFFECT_OMMEL_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_BONE with value 29 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_BONE << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_CERUMEN // DO NOT MODIFY!
  bsA = 25;
  bsB = EFFECT_OMMEL_CERUMEN;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_CERUMEN with value 25 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_CERUMEN << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_SNOT // DO NOT MODIFY!
  bsA = 28;
  bsB = EFFECT_OMMEL_SNOT;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_SNOT with value 28 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_SNOT << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_SWEAT // DO NOT MODIFY!
  bsA = 26;
  bsB = EFFECT_OMMEL_SWEAT;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_SWEAT with value 26 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_SWEAT << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_TEARS // DO NOT MODIFY!
  bsA = 27;
  bsB = EFFECT_OMMEL_TEARS;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_TEARS with value 27 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_TEARS << "!" << std::endl;
#endif 

  
#ifdef EFFECT_OMMEL_URINE // DO NOT MODIFY!
  bsA = 3;
  bsB = EFFECT_OMMEL_URINE;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_OMMEL_URINE with value 3 from .dat file mismatches hardcoded c++ define value of " << EFFECT_OMMEL_URINE << "!" << std::endl;
#endif 

  
#ifdef EFFECT_PANACEA // DO NOT MODIFY!
  bsA = 33;
  bsB = EFFECT_PANACEA;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_PANACEA with value 33 from .dat file mismatches hardcoded c++ define value of " << EFFECT_PANACEA << "!" << std::endl;
#endif 

  
#ifdef EFFECT_PANIC // DO NOT MODIFY!
  bsA = 35;
  bsB = EFFECT_PANIC;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_PANIC with value 35 from .dat file mismatches hardcoded c++ define value of " << EFFECT_PANIC << "!" << std::endl;
#endif 

  
#ifdef EFFECT_PEA_SOUP // DO NOT MODIFY!
  bsA = 19;
  bsB = EFFECT_PEA_SOUP;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_PEA_SOUP with value 19 from .dat file mismatches hardcoded c++ define value of " << EFFECT_PEA_SOUP << "!" << std::endl;
#endif 

  
#ifdef EFFECT_PEPSI // DO NOT MODIFY!
  bsA = 4;
  bsB = EFFECT_PEPSI;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_PEPSI with value 4 from .dat file mismatches hardcoded c++ define value of " << EFFECT_PEPSI << "!" << std::endl;
#endif 

  
#ifdef EFFECT_POISON // DO NOT MODIFY!
  bsA = 1;
  bsB = EFFECT_POISON;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_POISON with value 1 from .dat file mismatches hardcoded c++ define value of " << EFFECT_POISON << "!" << std::endl;
#endif 

  
#ifdef EFFECT_POLYMORPH // DO NOT MODIFY!
  bsA = 11;
  bsB = EFFECT_POLYMORPH;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_POLYMORPH with value 11 from .dat file mismatches hardcoded c++ define value of " << EFFECT_POLYMORPH << "!" << std::endl;
#endif 

  
#ifdef EFFECT_REGENERATION // DO NOT MODIFY!
  bsA = 37;
  bsB = EFFECT_REGENERATION;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_REGENERATION with value 37 from .dat file mismatches hardcoded c++ define value of " << EFFECT_REGENERATION << "!" << std::endl;
#endif 

  
#ifdef EFFECT_SCHOOL_FOOD // DO NOT MODIFY!
  bsA = 8;
  bsB = EFFECT_SCHOOL_FOOD;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_SCHOOL_FOOD with value 8 from .dat file mismatches hardcoded c++ define value of " << EFFECT_SCHOOL_FOOD << "!" << std::endl;
#endif 

  
#ifdef EFFECT_SKUNK_SMELL // DO NOT MODIFY!
  bsA = 13;
  bsB = EFFECT_SKUNK_SMELL;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_SKUNK_SMELL with value 13 from .dat file mismatches hardcoded c++ define value of " << EFFECT_SKUNK_SMELL << "!" << std::endl;
#endif 

  
#ifdef EFFECT_TELEPORTATION // DO NOT MODIFY!
  bsA = 38;
  bsB = EFFECT_TELEPORTATION;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_TELEPORTATION with value 38 from .dat file mismatches hardcoded c++ define value of " << EFFECT_TELEPORTATION << "!" << std::endl;
#endif 

  
#ifdef EFFECT_TELEPORT_CONTROL // DO NOT MODIFY!
  bsA = 23;
  bsB = EFFECT_TELEPORT_CONTROL;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_TELEPORT_CONTROL with value 23 from .dat file mismatches hardcoded c++ define value of " << EFFECT_TELEPORT_CONTROL << "!" << std::endl;
#endif 

  
#ifdef EFFECT_TRAIN_PERCEPTION // DO NOT MODIFY!
  bsA = 15;
  bsB = EFFECT_TRAIN_PERCEPTION;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_TRAIN_PERCEPTION with value 15 from .dat file mismatches hardcoded c++ define value of " << EFFECT_TRAIN_PERCEPTION << "!" << std::endl;
#endif 

  
#ifdef EFFECT_TRAIN_WISDOM // DO NOT MODIFY!
  bsA = 36;
  bsB = EFFECT_TRAIN_WISDOM;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_TRAIN_WISDOM with value 36 from .dat file mismatches hardcoded c++ define value of " << EFFECT_TRAIN_WISDOM << "!" << std::endl;
#endif 

  
#ifdef EFFECT_VAMPIRISM // DO NOT MODIFY!
  bsA = 32;
  bsB = EFFECT_VAMPIRISM;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_VAMPIRISM with value 32 from .dat file mismatches hardcoded c++ define value of " << EFFECT_VAMPIRISM << "!" << std::endl;
#endif 

  
#ifdef EFFECT_WHITE_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 22;
  bsB = EFFECT_WHITE_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined EFFECT_WHITE_UNICORN_FLESH with value 22 from .dat file mismatches hardcoded c++ define value of " << EFFECT_WHITE_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef ELASTI_CLOTH // DO NOT MODIFY!
  bsA = 4177;
  bsB = ELASTI_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined ELASTI_CLOTH with value 4177 from .dat file mismatches hardcoded c++ define value of " << ELASTI_CLOTH << "!" << std::endl;
#endif 

  
#ifdef ELDER // DO NOT MODIFY!
  bsA = 3;
  bsB = ELDER;
  if(bsA!=bsB)
    ssErrors << "Defined ELDER with value 3 from .dat file mismatches hardcoded c++ define value of " << ELDER << "!" << std::endl;
#endif 

  
#ifdef ELECTRICITY // DO NOT MODIFY!
  bsA = 16;
  bsB = ELECTRICITY;
  if(bsA!=bsB)
    ssErrors << "Defined ELECTRICITY with value 16 from .dat file mismatches hardcoded c++ define value of " << ELECTRICITY << "!" << std::endl;
#endif 

  
#ifdef ELECTRUM // DO NOT MODIFY!
  bsA = 4119;
  bsB = ELECTRUM;
  if(bsA!=bsB)
    ssErrors << "Defined ELECTRUM with value 4119 from .dat file mismatches hardcoded c++ define value of " << ELECTRUM << "!" << std::endl;
#endif 

  
#ifdef ELF_ALE // DO NOT MODIFY!
  bsA = 16405;
  bsB = ELF_ALE;
  if(bsA!=bsB)
    ssErrors << "Defined ELF_ALE with value 16405 from .dat file mismatches hardcoded c++ define value of " << ELF_ALE << "!" << std::endl;
#endif 

  
#ifdef ELF_CLOTH // DO NOT MODIFY!
  bsA = 4133;
  bsB = ELF_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined ELF_CLOTH with value 4133 from .dat file mismatches hardcoded c++ define value of " << ELF_CLOTH << "!" << std::endl;
#endif 

  
#ifdef ELF_FLESH // DO NOT MODIFY!
  bsA = 20532;
  bsB = ELF_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined ELF_FLESH with value 20532 from .dat file mismatches hardcoded c++ define value of " << ELF_FLESH << "!" << std::endl;
#endif 

  
#ifdef ELITE // DO NOT MODIFY!
  bsA = 6;
  bsB = ELITE;
  if(bsA!=bsB)
    ssErrors << "Defined ELITE with value 6 from .dat file mismatches hardcoded c++ define value of " << ELITE << "!" << std::endl;
#endif 

  
#ifdef ELITE_FEMALE // DO NOT MODIFY!
  bsA = 16;
  bsB = ELITE_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined ELITE_FEMALE with value 16 from .dat file mismatches hardcoded c++ define value of " << ELITE_FEMALE << "!" << std::endl;
#endif 

  
#ifdef ELM_WOOD // DO NOT MODIFY!
  bsA = 4229;
  bsB = ELM_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined ELM_WOOD with value 4229 from .dat file mismatches hardcoded c++ define value of " << ELM_WOOD << "!" << std::endl;
#endif 

  
#ifdef ELPURI_CAVE // DO NOT MODIFY!
  bsA = 1;
  bsB = ELPURI_CAVE;
  if(bsA!=bsB)
    ssErrors << "Defined ELPURI_CAVE with value 1 from .dat file mismatches hardcoded c++ define value of " << ELPURI_CAVE << "!" << std::endl;
#endif 

  
#ifdef ELPURI_FLESH // DO NOT MODIFY!
  bsA = 20490;
  bsB = ELPURI_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined ELPURI_FLESH with value 20490 from .dat file mismatches hardcoded c++ define value of " << ELPURI_FLESH << "!" << std::endl;
#endif 

  
#ifdef EMERALD // DO NOT MODIFY!
  bsA = 4296;
  bsB = EMERALD;
  if(bsA!=bsB)
    ssErrors << "Defined EMERALD with value 4296 from .dat file mismatches hardcoded c++ define value of " << EMERALD << "!" << std::endl;
#endif 

  
#ifdef EMISSARY // DO NOT MODIFY!
  bsA = 12;
  bsB = EMISSARY;
  if(bsA!=bsB)
    ssErrors << "Defined EMISSARY with value 12 from .dat file mismatches hardcoded c++ define value of " << EMISSARY << "!" << std::endl;
#endif 

  
#ifdef EMPTY_AREA // DO NOT MODIFY!
  bsA = 5;
  bsB = EMPTY_AREA;
  if(bsA!=bsB)
    ssErrors << "Defined EMPTY_AREA with value 5 from .dat file mismatches hardcoded c++ define value of " << EMPTY_AREA << "!" << std::endl;
#endif 

  
#ifdef ENDURANCE // DO NOT MODIFY!
  bsA = 0;
  bsB = ENDURANCE;
  if(bsA!=bsB)
    ssErrors << "Defined ENDURANCE with value 0 from .dat file mismatches hardcoded c++ define value of " << ENDURANCE << "!" << std::endl;
#endif 

  
#ifdef ENERGY // DO NOT MODIFY!
  bsA = 32;
  bsB = ENERGY;
  if(bsA!=bsB)
    ssErrors << "Defined ENERGY with value 32 from .dat file mismatches hardcoded c++ define value of " << ENERGY << "!" << std::endl;
#endif 

  
#ifdef ENNER_BEAST_FLESH // DO NOT MODIFY!
  bsA = 20496;
  bsB = ENNER_BEAST_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined ENNER_BEAST_FLESH with value 20496 from .dat file mismatches hardcoded c++ define value of " << ENNER_BEAST_FLESH << "!" << std::endl;
#endif 

  
#ifdef ENNER_BEAST_LEVEL // DO NOT MODIFY!
  bsA = 4;
  bsB = ENNER_BEAST_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined ENNER_BEAST_LEVEL with value 4 from .dat file mismatches hardcoded c++ define value of " << ENNER_BEAST_LEVEL << "!" << std::endl;
#endif 

  
#ifdef ESP // DO NOT MODIFY!
  bsA = 256;
  bsB = ESP;
  if(bsA!=bsB)
    ssErrors << "Defined ESP with value 256 from .dat file mismatches hardcoded c++ define value of " << ESP << "!" << std::endl;
#endif 

  
#ifdef ETEX_CLOTH // DO NOT MODIFY!
  bsA = 4178;
  bsB = ETEX_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined ETEX_CLOTH with value 4178 from .dat file mismatches hardcoded c++ define value of " << ETEX_CLOTH << "!" << std::endl;
#endif 

  
#ifdef ETHER // DO NOT MODIFY!
  bsA = 12291;
  bsB = ETHER;
  if(bsA!=bsB)
    ssErrors << "Defined ETHER with value 12291 from .dat file mismatches hardcoded c++ define value of " << ETHER << "!" << std::endl;
#endif 

  
#ifdef ETHEREAL // DO NOT MODIFY!
  bsA = 8;
  bsB = ETHEREAL;
  if(bsA!=bsB)
    ssErrors << "Defined ETHEREAL with value 8 from .dat file mismatches hardcoded c++ define value of " << ETHEREAL << "!" << std::endl;
#endif 

  
#ifdef ETHEREAL_MOVING // DO NOT MODIFY!
  bsA = 2097152;
  bsB = ETHEREAL_MOVING;
  if(bsA!=bsB)
    ssErrors << "Defined ETHEREAL_MOVING with value 2097152 from .dat file mismatches hardcoded c++ define value of " << ETHEREAL_MOVING << "!" << std::endl;
#endif 

  
#ifdef EUNUCH // DO NOT MODIFY!
  bsA = 3;
  bsB = EUNUCH;
  if(bsA!=bsB)
    ssErrors << "Defined EUNUCH with value 3 from .dat file mismatches hardcoded c++ define value of " << EUNUCH << "!" << std::endl;
#endif 

  
#ifdef EVERGREEN_FOREST // DO NOT MODIFY!
  bsA = 5;
  bsB = EVERGREEN_FOREST;
  if(bsA!=bsB)
    ssErrors << "Defined EVERGREEN_FOREST with value 5 from .dat file mismatches hardcoded c++ define value of " << EVERGREEN_FOREST << "!" << std::endl;
#endif 

  
#ifdef EVIL // DO NOT MODIFY!
  bsA = 3;
  bsB = EVIL;
  if(bsA!=bsB)
    ssErrors << "Defined EVIL with value 3 from .dat file mismatches hardcoded c++ define value of " << EVIL << "!" << std::endl;
#endif 

  
#ifdef EVIL_WONDER_STAFF_VAPOUR // DO NOT MODIFY!
  bsA = 12296;
  bsB = EVIL_WONDER_STAFF_VAPOUR;
  if(bsA!=bsB)
    ssErrors << "Defined EVIL_WONDER_STAFF_VAPOUR with value 12296 from .dat file mismatches hardcoded c++ define value of " << EVIL_WONDER_STAFF_VAPOUR << "!" << std::endl;
#endif 

  
#ifdef EXPENSIVE // DO NOT MODIFY!
  bsA = 2;
  bsB = EXPENSIVE;
  if(bsA!=bsB)
    ssErrors << "Defined EXPENSIVE with value 2 from .dat file mismatches hardcoded c++ define value of " << EXPENSIVE << "!" << std::endl;
#endif 

  
#ifdef EXPENSIVE_BED // DO NOT MODIFY!
  bsA = 12;
  bsB = EXPENSIVE_BED;
  if(bsA!=bsB)
    ssErrors << "Defined EXPENSIVE_BED with value 12 from .dat file mismatches hardcoded c++ define value of " << EXPENSIVE_BED << "!" << std::endl;
#endif 

  
#ifdef EYE_COLOR // DO NOT MODIFY!
  bsA = 8;
  bsB = EYE_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined EYE_COLOR with value 8 from .dat file mismatches hardcoded c++ define value of " << EYE_COLOR << "!" << std::endl;
#endif 

  
#ifdef FABRIC // DO NOT MODIFY!
  bsA = 4132;
  bsB = FABRIC;
  if(bsA!=bsB)
    ssErrors << "Defined FABRIC with value 4132 from .dat file mismatches hardcoded c++ define value of " << FABRIC << "!" << std::endl;
#endif 

  
#ifdef FAIRY_STEEL // DO NOT MODIFY!
  bsA = 4109;
  bsB = FAIRY_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined FAIRY_STEEL with value 4109 from .dat file mismatches hardcoded c++ define value of " << FAIRY_STEEL << "!" << std::endl;
#endif 

  
#ifdef FART // DO NOT MODIFY!
  bsA = 12298;
  bsB = FART;
  if(bsA!=bsB)
    ssErrors << "Defined FART with value 12298 from .dat file mismatches hardcoded c++ define value of " << FART << "!" << std::endl;
#endif 

  
#ifdef FASTING // DO NOT MODIFY!
  bsA = 1073741824;
  bsB = FASTING;
  if(bsA!=bsB)
    ssErrors << "Defined FASTING with value 1073741824 from .dat file mismatches hardcoded c++ define value of " << FASTING << "!" << std::endl;
#endif 

  
#ifdef FEAR // DO NOT MODIFY!
  bsA = 2;
  bsB = FEAR;
  if(bsA!=bsB)
    ssErrors << "Defined FEAR with value 2 from .dat file mismatches hardcoded c++ define value of " << FEAR << "!" << std::endl;
#endif 

  
#ifdef FEARLESS // DO NOT MODIFY!
  bsA = 536870912;
  bsB = FEARLESS;
  if(bsA!=bsB)
    ssErrors << "Defined FEARLESS with value 536870912 from .dat file mismatches hardcoded c++ define value of " << FEARLESS << "!" << std::endl;
#endif 

  
#ifdef FELT // DO NOT MODIFY!
  bsA = 4131;
  bsB = FELT;
  if(bsA!=bsB)
    ssErrors << "Defined FELT with value 4131 from .dat file mismatches hardcoded c++ define value of " << FELT << "!" << std::endl;
#endif 

  
#ifdef FEMALE // DO NOT MODIFY!
  bsA = 2;
  bsB = FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined FEMALE with value 2 from .dat file mismatches hardcoded c++ define value of " << FEMALE << "!" << std::endl;
#endif 

  
#ifdef FIR // DO NOT MODIFY!
  bsA = 2;
  bsB = FIR;
  if(bsA!=bsB)
    ssErrors << "Defined FIR with value 2 from .dat file mismatches hardcoded c++ define value of " << FIR << "!" << std::endl;
#endif 

  
#ifdef FIRE // DO NOT MODIFY!
  bsA = 8;
  bsB = FIRE;
  if(bsA!=bsB)
    ssErrors << "Defined FIRE with value 8 from .dat file mismatches hardcoded c++ define value of " << FIRE << "!" << std::endl;
#endif 

  
#ifdef FIR_WOOD // DO NOT MODIFY!
  bsA = 4239;
  bsB = FIR_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined FIR_WOOD with value 4239 from .dat file mismatches hardcoded c++ define value of " << FIR_WOOD << "!" << std::endl;
#endif 

  
#ifdef FISH_SCALE // DO NOT MODIFY!
  bsA = 4167;
  bsB = FISH_SCALE;
  if(bsA!=bsB)
    ssErrors << "Defined FISH_SCALE with value 4167 from .dat file mismatches hardcoded c++ define value of " << FISH_SCALE << "!" << std::endl;
#endif 

  
#ifdef FLAT // DO NOT MODIFY!
  bsA = 2;
  bsB = FLAT;
  if(bsA!=bsB)
    ssErrors << "Defined FLAT with value 2 from .dat file mismatches hardcoded c++ define value of " << FLAT << "!" << std::endl;
#endif 

  
#ifdef FLAT_BREAD // DO NOT MODIFY!
  bsA = 8205;
  bsB = FLAT_BREAD;
  if(bsA!=bsB)
    ssErrors << "Defined FLAT_BREAD with value 8205 from .dat file mismatches hardcoded c++ define value of " << FLAT_BREAD << "!" << std::endl;
#endif 

  
#ifdef FLEE_FROM_ENEMIES // DO NOT MODIFY!
  bsA = 2;
  bsB = FLEE_FROM_ENEMIES;
  if(bsA!=bsB)
    ssErrors << "Defined FLEE_FROM_ENEMIES with value 2 from .dat file mismatches hardcoded c++ define value of " << FLEE_FROM_ENEMIES << "!" << std::endl;
#endif 

  
#ifdef FLESH_ID // DO NOT MODIFY!
  bsA = 20480;
  bsB = FLESH_ID;
  if(bsA!=bsB)
    ssErrors << "Defined FLESH_ID with value 20480 from .dat file mismatches hardcoded c++ define value of " << FLESH_ID << "!" << std::endl;
#endif 

  
#ifdef FLEXI_CLOTH // DO NOT MODIFY!
  bsA = 4180;
  bsB = FLEXI_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined FLEXI_CLOTH with value 4180 from .dat file mismatches hardcoded c++ define value of " << FLEXI_CLOTH << "!" << std::endl;
#endif 

  
#ifdef FLINT // DO NOT MODIFY!
  bsA = 4277;
  bsB = FLINT;
  if(bsA!=bsB)
    ssErrors << "Defined FLINT with value 4277 from .dat file mismatches hardcoded c++ define value of " << FLINT << "!" << std::endl;
#endif 

  
#ifdef FLIP // DO NOT MODIFY!
  bsA = 2;
  bsB = FLIP;
  if(bsA!=bsB)
    ssErrors << "Defined FLIP with value 2 from .dat file mismatches hardcoded c++ define value of " << FLIP << "!" << std::endl;
#endif 

  
#ifdef FLOATING_EYE_FLESH // DO NOT MODIFY!
  bsA = 20519;
  bsB = FLOATING_EYE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined FLOATING_EYE_FLESH with value 20519 from .dat file mismatches hardcoded c++ define value of " << FLOATING_EYE_FLESH << "!" << std::endl;
#endif 

  
#ifdef FLOOR // DO NOT MODIFY!
  bsA = 2;
  bsB = FLOOR;
  if(bsA!=bsB)
    ssErrors << "Defined FLOOR with value 2 from .dat file mismatches hardcoded c++ define value of " << FLOOR << "!" << std::endl;
#endif 

  
#ifdef FLY // DO NOT MODIFY!
  bsA = 4;
  bsB = FLY;
  if(bsA!=bsB)
    ssErrors << "Defined FLY with value 4 from .dat file mismatches hardcoded c++ define value of " << FLY << "!" << std::endl;
#endif 

  
#ifdef FOLDED_CARBIDE // DO NOT MODIFY!
  bsA = 4196;
  bsB = FOLDED_CARBIDE;
  if(bsA!=bsB)
    ssErrors << "Defined FOLDED_CARBIDE with value 4196 from .dat file mismatches hardcoded c++ define value of " << FOLDED_CARBIDE << "!" << std::endl;
#endif 

  
#ifdef FOLLOW_PLAYER // DO NOT MODIFY!
  bsA = 1;
  bsB = FOLLOW_PLAYER;
  if(bsA!=bsB)
    ssErrors << "Defined FOLLOW_PLAYER with value 1 from .dat file mismatches hardcoded c++ define value of " << FOLLOW_PLAYER << "!" << std::endl;
#endif 

  
#ifdef FOOD // DO NOT MODIFY!
  bsA = 1024;
  bsB = FOOD;
  if(bsA!=bsB)
    ssErrors << "Defined FOOD with value 1024 from .dat file mismatches hardcoded c++ define value of " << FOOD << "!" << std::endl;
#endif 

  
#ifdef FORCE_THE // DO NOT MODIFY!
  bsA = 8;
  bsB = FORCE_THE;
  if(bsA!=bsB)
    ssErrors << "Defined FORCE_THE with value 8 from .dat file mismatches hardcoded c++ define value of " << FORCE_THE << "!" << std::endl;
#endif 

  
#ifdef FORGE // DO NOT MODIFY!
  bsA = 26;
  bsB = FORGE;
  if(bsA!=bsB)
    ssErrors << "Defined FORGE with value 26 from .dat file mismatches hardcoded c++ define value of " << FORGE << "!" << std::endl;
#endif 

  
#ifdef FOUNTAIN // DO NOT MODIFY!
  bsA = 65535;
  bsB = FOUNTAIN;
  if(bsA!=bsB)
    ssErrors << "Defined FOUNTAIN with value 65535 from .dat file mismatches hardcoded c++ define value of " << FOUNTAIN << "!" << std::endl;
#endif 

  
#ifdef FRIEND // DO NOT MODIFY!
  bsA = 4;
  bsB = FRIEND;
  if(bsA!=bsB)
    ssErrors << "Defined FRIEND with value 4 from .dat file mismatches hardcoded c++ define value of " << FRIEND << "!" << std::endl;
#endif 

  
#ifdef FROG_FLESH // DO NOT MODIFY!
  bsA = 20489;
  bsB = FROG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined FROG_FLESH with value 20489 from .dat file mismatches hardcoded c++ define value of " << FROG_FLESH << "!" << std::endl;
#endif 

  
#ifdef FRYING_PAN // DO NOT MODIFY!
  bsA = 20;
  bsB = FRYING_PAN;
  if(bsA!=bsB)
    ssErrors << "Defined FRYING_PAN with value 20 from .dat file mismatches hardcoded c++ define value of " << FRYING_PAN << "!" << std::endl;
#endif 

  
#ifdef FULLERITE // DO NOT MODIFY!
  bsA = 4197;
  bsB = FULLERITE;
  if(bsA!=bsB)
    ssErrors << "Defined FULLERITE with value 4197 from .dat file mismatches hardcoded c++ define value of " << FULLERITE << "!" << std::endl;
#endif 

  
#ifdef FULL_HELMET // DO NOT MODIFY!
  bsA = 1;
  bsB = FULL_HELMET;
  if(bsA!=bsB)
    ssErrors << "Defined FULL_HELMET with value 1 from .dat file mismatches hardcoded c++ define value of " << FULL_HELMET << "!" << std::endl;
#endif 

  
#ifdef FUNGI_WOOD // DO NOT MODIFY!
  bsA = 4226;
  bsB = FUNGI_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined FUNGI_WOOD with value 4226 from .dat file mismatches hardcoded c++ define value of " << FUNGI_WOOD << "!" << std::endl;
#endif 

  
#ifdef FURNACE // DO NOT MODIFY!
  bsA = 27;
  bsB = FURNACE;
  if(bsA!=bsB)
    ssErrors << "Defined FURNACE with value 27 from .dat file mismatches hardcoded c++ define value of " << FURNACE << "!" << std::endl;
#endif 

  
#ifdef GALVORN // DO NOT MODIFY!
  bsA = 4111;
  bsB = GALVORN;
  if(bsA!=bsB)
    ssErrors << "Defined GALVORN with value 4111 from .dat file mismatches hardcoded c++ define value of " << GALVORN << "!" << std::endl;
#endif 

  
#ifdef GAS_ID // DO NOT MODIFY!
  bsA = 12288;
  bsB = GAS_ID;
  if(bsA!=bsB)
    ssErrors << "Defined GAS_ID with value 12288 from .dat file mismatches hardcoded c++ define value of " << GAS_ID << "!" << std::endl;
#endif 

  
#ifdef GAS_IMMUNITY // DO NOT MODIFY!
  bsA = 131072;
  bsB = GAS_IMMUNITY;
  if(bsA!=bsB)
    ssErrors << "Defined GAS_IMMUNITY with value 131072 from .dat file mismatches hardcoded c++ define value of " << GAS_IMMUNITY << "!" << std::endl;
#endif 

  
#ifdef GAUNTLET // DO NOT MODIFY!
  bsA = 128;
  bsB = GAUNTLET;
  if(bsA!=bsB)
    ssErrors << "Defined GAUNTLET with value 128 from .dat file mismatches hardcoded c++ define value of " << GAUNTLET << "!" << std::endl;
#endif 

  
#ifdef GAUNTLET_COLOR // DO NOT MODIFY!
  bsA = 512;
  bsB = GAUNTLET_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined GAUNTLET_COLOR with value 512 from .dat file mismatches hardcoded c++ define value of " << GAUNTLET_COLOR << "!" << std::endl;
#endif 

  
#ifdef GAUNTLET_OF_DEXTERITY // DO NOT MODIFY!
  bsA = 2;
  bsB = GAUNTLET_OF_DEXTERITY;
  if(bsA!=bsB)
    ssErrors << "Defined GAUNTLET_OF_DEXTERITY with value 2 from .dat file mismatches hardcoded c++ define value of " << GAUNTLET_OF_DEXTERITY << "!" << std::endl;
#endif 

  
#ifdef GAUNTLET_OF_STRENGTH // DO NOT MODIFY!
  bsA = 1;
  bsB = GAUNTLET_OF_STRENGTH;
  if(bsA!=bsB)
    ssErrors << "Defined GAUNTLET_OF_STRENGTH with value 1 from .dat file mismatches hardcoded c++ define value of " << GAUNTLET_OF_STRENGTH << "!" << std::endl;
#endif 

  
#ifdef GENERAL // DO NOT MODIFY!
  bsA = 4;
  bsB = GENERAL;
  if(bsA!=bsB)
    ssErrors << "Defined GENERAL with value 4 from .dat file mismatches hardcoded c++ define value of " << GENERAL << "!" << std::endl;
#endif 

  
#ifdef GIANT // DO NOT MODIFY!
  bsA = 2;
  bsB = GIANT;
  if(bsA!=bsB)
    ssErrors << "Defined GIANT with value 2 from .dat file mismatches hardcoded c++ define value of " << GIANT << "!" << std::endl;
#endif 

  
#ifdef GIANT_DARK // DO NOT MODIFY!
  bsA = 3;
  bsB = GIANT_DARK;
  if(bsA!=bsB)
    ssErrors << "Defined GIANT_DARK with value 3 from .dat file mismatches hardcoded c++ define value of " << GIANT_DARK << "!" << std::endl;
#endif 

  
#ifdef GIANT_LIGHT // DO NOT MODIFY!
  bsA = 6;
  bsB = GIANT_LIGHT;
  if(bsA!=bsB)
    ssErrors << "Defined GIANT_LIGHT with value 6 from .dat file mismatches hardcoded c++ define value of " << GIANT_LIGHT << "!" << std::endl;
#endif 

  
#ifdef GIBBERLING_FLESH // DO NOT MODIFY!
  bsA = 20503;
  bsB = GIBBERLING_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined GIBBERLING_FLESH with value 20503 from .dat file mismatches hardcoded c++ define value of " << GIBBERLING_FLESH << "!" << std::endl;
#endif 

  
#ifdef GIRL // DO NOT MODIFY!
  bsA = 2;
  bsB = GIRL;
  if(bsA!=bsB)
    ssErrors << "Defined GIRL with value 2 from .dat file mismatches hardcoded c++ define value of " << GIRL << "!" << std::endl;
#endif 

  
#ifdef GLACIER // DO NOT MODIFY!
  bsA = 7;
  bsB = GLACIER;
  if(bsA!=bsB)
    ssErrors << "Defined GLACIER with value 7 from .dat file mismatches hardcoded c++ define value of " << GLACIER << "!" << std::endl;
#endif 

  
#ifdef GLASS // DO NOT MODIFY!
  bsA = 4206;
  bsB = GLASS;
  if(bsA!=bsB)
    ssErrors << "Defined GLASS with value 4206 from .dat file mismatches hardcoded c++ define value of " << GLASS << "!" << std::endl;
#endif 

  
#ifdef GLOWING_BLOOD // DO NOT MODIFY!
  bsA = 16423;
  bsB = GLOWING_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined GLOWING_BLOOD with value 16423 from .dat file mismatches hardcoded c++ define value of " << GLOWING_BLOOD << "!" << std::endl;
#endif 

  
#ifdef GNEISS // DO NOT MODIFY!
  bsA = 4272;
  bsB = GNEISS;
  if(bsA!=bsB)
    ssErrors << "Defined GNEISS with value 4272 from .dat file mismatches hardcoded c++ define value of " << GNEISS << "!" << std::endl;
#endif 

  
#ifdef GOBLINOID_FLESH // DO NOT MODIFY!
  bsA = 20488;
  bsB = GOBLINOID_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined GOBLINOID_FLESH with value 20488 from .dat file mismatches hardcoded c++ define value of " << GOBLINOID_FLESH << "!" << std::endl;
#endif 

  
#ifdef GOLD // DO NOT MODIFY!
  bsA = 4115;
  bsB = GOLD;
  if(bsA!=bsB)
    ssErrors << "Defined GOLD with value 4115 from .dat file mismatches hardcoded c++ define value of " << GOLD << "!" << std::endl;
#endif 

  
#ifdef GOLDEN_EAGLE_FEATHER // DO NOT MODIFY!
  bsA = 4165;
  bsB = GOLDEN_EAGLE_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined GOLDEN_EAGLE_FEATHER with value 4165 from .dat file mismatches hardcoded c++ define value of " << GOLDEN_EAGLE_FEATHER << "!" << std::endl;
#endif 

  
#ifdef GOOD // DO NOT MODIFY!
  bsA = 1;
  bsB = GOOD;
  if(bsA!=bsB)
    ssErrors << "Defined GOOD with value 1 from .dat file mismatches hardcoded c++ define value of " << GOOD << "!" << std::endl;
#endif 

  
#ifdef GOOD_WONDER_STAFF_VAPOUR // DO NOT MODIFY!
  bsA = 12297;
  bsB = GOOD_WONDER_STAFF_VAPOUR;
  if(bsA!=bsB)
    ssErrors << "Defined GOOD_WONDER_STAFF_VAPOUR with value 12297 from .dat file mismatches hardcoded c++ define value of " << GOOD_WONDER_STAFF_VAPOUR << "!" << std::endl;
#endif 

  
#ifdef GOROVITS_FAMILY_GAS_MASK // DO NOT MODIFY!
  bsA = 6;
  bsB = GOROVITS_FAMILY_GAS_MASK;
  if(bsA!=bsB)
    ssErrors << "Defined GOROVITS_FAMILY_GAS_MASK with value 6 from .dat file mismatches hardcoded c++ define value of " << GOROVITS_FAMILY_GAS_MASK << "!" << std::endl;
#endif 

  
#ifdef GOROVITS_HAMMER // DO NOT MODIFY!
  bsA = 1;
  bsB = GOROVITS_HAMMER;
  if(bsA!=bsB)
    ssErrors << "Defined GOROVITS_HAMMER with value 1 from .dat file mismatches hardcoded c++ define value of " << GOROVITS_HAMMER << "!" << std::endl;
#endif 

  
#ifdef GOROVITS_SCIMITAR // DO NOT MODIFY!
  bsA = 3;
  bsB = GOROVITS_SCIMITAR;
  if(bsA!=bsB)
    ssErrors << "Defined GOROVITS_SCIMITAR with value 3 from .dat file mismatches hardcoded c++ define value of " << GOROVITS_SCIMITAR << "!" << std::endl;
#endif 

  
#ifdef GOROVITS_SICKLE // DO NOT MODIFY!
  bsA = 2;
  bsB = GOROVITS_SICKLE;
  if(bsA!=bsB)
    ssErrors << "Defined GOROVITS_SICKLE with value 2 from .dat file mismatches hardcoded c++ define value of " << GOROVITS_SICKLE << "!" << std::endl;
#endif 

  
#ifdef GOSSAMER // DO NOT MODIFY!
  bsA = 4139;
  bsB = GOSSAMER;
  if(bsA!=bsB)
    ssErrors << "Defined GOSSAMER with value 4139 from .dat file mismatches hardcoded c++ define value of " << GOSSAMER << "!" << std::endl;
#endif 

  
#ifdef GRAND_MASTER // DO NOT MODIFY!
  bsA = 8;
  bsB = GRAND_MASTER;
  if(bsA!=bsB)
    ssErrors << "Defined GRAND_MASTER with value 8 from .dat file mismatches hardcoded c++ define value of " << GRAND_MASTER << "!" << std::endl;
#endif 

  
#ifdef GRAND_STOLLEN_KNIFE // DO NOT MODIFY!
  bsA = 17;
  bsB = GRAND_STOLLEN_KNIFE;
  if(bsA!=bsB)
    ssErrors << "Defined GRAND_STOLLEN_KNIFE with value 17 from .dat file mismatches hardcoded c++ define value of " << GRAND_STOLLEN_KNIFE << "!" << std::endl;
#endif 

  
#ifdef GRANITE // DO NOT MODIFY!
  bsA = 4274;
  bsB = GRANITE;
  if(bsA!=bsB)
    ssErrors << "Defined GRANITE with value 4274 from .dat file mismatches hardcoded c++ define value of " << GRANITE << "!" << std::endl;
#endif 

  
#ifdef GRASS // DO NOT MODIFY!
  bsA = 4213;
  bsB = GRASS;
  if(bsA!=bsB)
    ssErrors << "Defined GRASS with value 4213 from .dat file mismatches hardcoded c++ define value of " << GRASS << "!" << std::endl;
#endif 

  
#ifdef GRASS_TERRAIN // DO NOT MODIFY!
  bsA = 4;
  bsB = GRASS_TERRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined GRASS_TERRAIN with value 4 from .dat file mismatches hardcoded c++ define value of " << GRASS_TERRAIN << "!" << std::endl;
#endif 

  
#ifdef GRAVEL // DO NOT MODIFY!
  bsA = 4257;
  bsB = GRAVEL;
  if(bsA!=bsB)
    ssErrors << "Defined GRAVEL with value 4257 from .dat file mismatches hardcoded c++ define value of " << GRAVEL << "!" << std::endl;
#endif 

  
#ifdef GRAVE_KEEPER // DO NOT MODIFY!
  bsA = 5;
  bsB = GRAVE_KEEPER;
  if(bsA!=bsB)
    ssErrors << "Defined GRAVE_KEEPER with value 5 from .dat file mismatches hardcoded c++ define value of " << GRAVE_KEEPER << "!" << std::endl;
#endif 

  
#ifdef GRAY_FRACTAL // DO NOT MODIFY!
  bsA = 0;
  bsB = GRAY_FRACTAL;
  if(bsA!=bsB)
    ssErrors << "Defined GRAY_FRACTAL with value 0 from .dat file mismatches hardcoded c++ define value of " << GRAY_FRACTAL << "!" << std::endl;
#endif 

  
#ifdef GRAY_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 20511;
  bsB = GRAY_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined GRAY_UNICORN_FLESH with value 20511 from .dat file mismatches hardcoded c++ define value of " << GRAY_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef GREATER // DO NOT MODIFY!
  bsA = 1;
  bsB = GREATER;
  if(bsA!=bsB)
    ssErrors << "Defined GREATER with value 1 from .dat file mismatches hardcoded c++ define value of " << GREATER << "!" << std::endl;
#endif 

  
#ifdef GREATER_DARK // DO NOT MODIFY!
  bsA = 2;
  bsB = GREATER_DARK;
  if(bsA!=bsB)
    ssErrors << "Defined GREATER_DARK with value 2 from .dat file mismatches hardcoded c++ define value of " << GREATER_DARK << "!" << std::endl;
#endif 

  
#ifdef GREATER_LIGHT // DO NOT MODIFY!
  bsA = 5;
  bsB = GREATER_LIGHT;
  if(bsA!=bsB)
    ssErrors << "Defined GREATER_LIGHT with value 5 from .dat file mismatches hardcoded c++ define value of " << GREATER_LIGHT << "!" << std::endl;
#endif 

  
#ifdef GREEN // DO NOT MODIFY!
  bsA = 2016;
  bsB = GREEN;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN with value 2016 from .dat file mismatches hardcoded c++ define value of " << GREEN << "!" << std::endl;
#endif 

  
#ifdef GREEN_BLOOD // DO NOT MODIFY!
  bsA = 16420;
  bsB = GREEN_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_BLOOD with value 16420 from .dat file mismatches hardcoded c++ define value of " << GREEN_BLOOD << "!" << std::endl;
#endif 

  
#ifdef GREEN_CRYSTAL // DO NOT MODIFY!
  bsA = 4311;
  bsB = GREEN_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_CRYSTAL with value 4311 from .dat file mismatches hardcoded c++ define value of " << GREEN_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef GREEN_FRACTAL // DO NOT MODIFY!
  bsA = 2;
  bsB = GREEN_FRACTAL;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_FRACTAL with value 2 from .dat file mismatches hardcoded c++ define value of " << GREEN_FRACTAL << "!" << std::endl;
#endif 

  
#ifdef GREEN_JADE // DO NOT MODIFY!
  bsA = 4305;
  bsB = GREEN_JADE;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_JADE with value 4305 from .dat file mismatches hardcoded c++ define value of " << GREEN_JADE << "!" << std::endl;
#endif 

  
#ifdef GREEN_SIREN // DO NOT MODIFY!
  bsA = 5;
  bsB = GREEN_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_SIREN with value 5 from .dat file mismatches hardcoded c++ define value of " << GREEN_SIREN << "!" << std::endl;
#endif 

  
#ifdef GREEN_SLIME // DO NOT MODIFY!
  bsA = 16408;
  bsB = GREEN_SLIME;
  if(bsA!=bsB)
    ssErrors << "Defined GREEN_SLIME with value 16408 from .dat file mismatches hardcoded c++ define value of " << GREEN_SLIME << "!" << std::endl;
#endif 

  
#ifdef GRIFFON_FEATHER // DO NOT MODIFY!
  bsA = 4162;
  bsB = GRIFFON_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined GRIFFON_FEATHER with value 4162 from .dat file mismatches hardcoded c++ define value of " << GRIFFON_FEATHER << "!" << std::endl;
#endif 

  
#ifdef GRIZZLY_BEAR // DO NOT MODIFY!
  bsA = 2;
  bsB = GRIZZLY_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined GRIZZLY_BEAR with value 2 from .dat file mismatches hardcoded c++ define value of " << GRIZZLY_BEAR << "!" << std::endl;
#endif 

  
#ifdef GROIN // DO NOT MODIFY!
  bsA = 16;
  bsB = GROIN;
  if(bsA!=bsB)
    ssErrors << "Defined GROIN with value 16 from .dat file mismatches hardcoded c++ define value of " << GROIN << "!" << std::endl;
#endif 

  
#ifdef GROIN_INDEX // DO NOT MODIFY!
  bsA = 4;
  bsB = GROIN_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined GROIN_INDEX with value 4 from .dat file mismatches hardcoded c++ define value of " << GROIN_INDEX << "!" << std::endl;
#endif 

  
#ifdef GROUND // DO NOT MODIFY!
  bsA = 3;
  bsB = GROUND;
  if(bsA!=bsB)
    ssErrors << "Defined GROUND with value 3 from .dat file mismatches hardcoded c++ define value of " << GROUND << "!" << std::endl;
#endif 

  
#ifdef GUILD_TEAM // DO NOT MODIFY!
  bsA = 5;
  bsB = GUILD_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined GUILD_TEAM with value 5 from .dat file mismatches hardcoded c++ define value of " << GUILD_TEAM << "!" << std::endl;
#endif 

  
#ifdef GUN_POWDER // DO NOT MODIFY!
  bsA = 24577;
  bsB = GUN_POWDER;
  if(bsA!=bsB)
    ssErrors << "Defined GUN_POWDER with value 24577 from .dat file mismatches hardcoded c++ define value of " << GUN_POWDER << "!" << std::endl;
#endif 

  
#ifdef HAIR_COLOR // DO NOT MODIFY!
  bsA = 4;
  bsB = HAIR_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined HAIR_COLOR with value 4 from .dat file mismatches hardcoded c++ define value of " << HAIR_COLOR << "!" << std::endl;
#endif 

  
#ifdef HALBERD // DO NOT MODIFY!
  bsA = 6;
  bsB = HALBERD;
  if(bsA!=bsB)
    ssErrors << "Defined HALBERD with value 6 from .dat file mismatches hardcoded c++ define value of " << HALBERD << "!" << std::endl;
#endif 

  
#ifdef HALCYON // DO NOT MODIFY!
  bsA = 4288;
  bsB = HALCYON;
  if(bsA!=bsB)
    ssErrors << "Defined HALCYON with value 4288 from .dat file mismatches hardcoded c++ define value of " << HALCYON << "!" << std::endl;
#endif 

  
#ifdef HAMMER // DO NOT MODIFY!
  bsA = 16;
  bsB = HAMMER;
  if(bsA!=bsB)
    ssErrors << "Defined HAMMER with value 16 from .dat file mismatches hardcoded c++ define value of " << HAMMER << "!" << std::endl;
#endif 

  
#ifdef HARDENED_ASH // DO NOT MODIFY!
  bsA = 4326;
  bsB = HARDENED_ASH;
  if(bsA!=bsB)
    ssErrors << "Defined HARDENED_ASH with value 4326 from .dat file mismatches hardcoded c++ define value of " << HARDENED_ASH << "!" << std::endl;
#endif 

  
#ifdef HARDENED_CLAY // DO NOT MODIFY!
  bsA = 4319;
  bsB = HARDENED_CLAY;
  if(bsA!=bsB)
    ssErrors << "Defined HARDENED_CLAY with value 4319 from .dat file mismatches hardcoded c++ define value of " << HARDENED_CLAY << "!" << std::endl;
#endif 

  
#ifdef HARDENED_CONCRETE // DO NOT MODIFY!
  bsA = 4314;
  bsB = HARDENED_CONCRETE;
  if(bsA!=bsB)
    ssErrors << "Defined HARDENED_CONCRETE with value 4314 from .dat file mismatches hardcoded c++ define value of " << HARDENED_CONCRETE << "!" << std::endl;
#endif 

  
#ifdef HARDENED_LEATHER // DO NOT MODIFY!
  bsA = 4152;
  bsB = HARDENED_LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined HARDENED_LEATHER with value 4152 from .dat file mismatches hardcoded c++ define value of " << HARDENED_LEATHER << "!" << std::endl;
#endif 

  
#ifdef HARDENED_PLASTIC // DO NOT MODIFY!
  bsA = 4187;
  bsB = HARDENED_PLASTIC;
  if(bsA!=bsB)
    ssErrors << "Defined HARDENED_PLASTIC with value 4187 from .dat file mismatches hardcoded c++ define value of " << HARDENED_PLASTIC << "!" << std::endl;
#endif 

  
#ifdef HARPY_FEATHER // DO NOT MODIFY!
  bsA = 4161;
  bsB = HARPY_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined HARPY_FEATHER with value 4161 from .dat file mismatches hardcoded c++ define value of " << HARPY_FEATHER << "!" << std::endl;
#endif 

  
#ifdef HASTE // DO NOT MODIFY!
  bsA = 2;
  bsB = HASTE;
  if(bsA!=bsB)
    ssErrors << "Defined HASTE with value 2 from .dat file mismatches hardcoded c++ define value of " << HASTE << "!" << std::endl;
#endif 

  
#ifdef HAS_CHARACTER // DO NOT MODIFY!
  bsA = 2;
  bsB = HAS_CHARACTER;
  if(bsA!=bsB)
    ssErrors << "Defined HAS_CHARACTER with value 2 from .dat file mismatches hardcoded c++ define value of " << HAS_CHARACTER << "!" << std::endl;
#endif 

  
#ifdef HAS_NO_OTERRAIN // DO NOT MODIFY!
  bsA = 32;
  bsB = HAS_NO_OTERRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined HAS_NO_OTERRAIN with value 32 from .dat file mismatches hardcoded c++ define value of " << HAS_NO_OTERRAIN << "!" << std::endl;
#endif 

  
#ifdef HAS_SECONDARY_MATERIAL // DO NOT MODIFY!
  bsA = 2;
  bsB = HAS_SECONDARY_MATERIAL;
  if(bsA!=bsB)
    ssErrors << "Defined HAS_SECONDARY_MATERIAL with value 2 from .dat file mismatches hardcoded c++ define value of " << HAS_SECONDARY_MATERIAL << "!" << std::endl;
#endif 

  
#ifdef HATCHLING // DO NOT MODIFY!
  bsA = 1;
  bsB = HATCHLING;
  if(bsA!=bsB)
    ssErrors << "Defined HATCHLING with value 1 from .dat file mismatches hardcoded c++ define value of " << HATCHLING << "!" << std::endl;
#endif 

  
#ifdef HATTIFATTENER_FLESH // DO NOT MODIFY!
  bsA = 20526;
  bsB = HATTIFATTENER_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HATTIFATTENER_FLESH with value 20526 from .dat file mismatches hardcoded c++ define value of " << HATTIFATTENER_FLESH << "!" << std::endl;
#endif 

  
#ifdef HEAD // DO NOT MODIFY!
  bsA = 2;
  bsB = HEAD;
  if(bsA!=bsB)
    ssErrors << "Defined HEAD with value 2 from .dat file mismatches hardcoded c++ define value of " << HEAD << "!" << std::endl;
#endif 

  
#ifdef HEAD_INDEX // DO NOT MODIFY!
  bsA = 1;
  bsB = HEAD_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined HEAD_INDEX with value 1 from .dat file mismatches hardcoded c++ define value of " << HEAD_INDEX << "!" << std::endl;
#endif 

  
#ifdef HEALING // DO NOT MODIFY!
  bsA = 4;
  bsB = HEALING;
  if(bsA!=bsB)
    ssErrors << "Defined HEALING with value 4 from .dat file mismatches hardcoded c++ define value of " << HEALING << "!" << std::endl;
#endif 

  
#ifdef HEALING_LIQUID // DO NOT MODIFY!
  bsA = 16387;
  bsB = HEALING_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined HEALING_LIQUID with value 16387 from .dat file mismatches hardcoded c++ define value of " << HEALING_LIQUID << "!" << std::endl;
#endif 

  
#ifdef HEART_SHAPED_LOCK // DO NOT MODIFY!
  bsA = 19456;
  bsB = HEART_SHAPED_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined HEART_SHAPED_LOCK with value 19456 from .dat file mismatches hardcoded c++ define value of " << HEART_SHAPED_LOCK << "!" << std::endl;
#endif 

  
#ifdef HEAVILY_BURNT // DO NOT MODIFY!
  bsA = 3;
  bsB = HEAVILY_BURNT;
  if(bsA!=bsB)
    ssErrors << "Defined HEAVILY_BURNT with value 3 from .dat file mismatches hardcoded c++ define value of " << HEAVILY_BURNT << "!" << std::endl;
#endif 

  
#ifdef HEDGEHOG_FLESH // DO NOT MODIFY!
  bsA = 20524;
  bsB = HEDGEHOG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HEDGEHOG_FLESH with value 20524 from .dat file mismatches hardcoded c++ define value of " << HEDGEHOG_FLESH << "!" << std::endl;
#endif 

  
#ifdef HELMET // DO NOT MODIFY!
  bsA = 1;
  bsB = HELMET;
  if(bsA!=bsB)
    ssErrors << "Defined HELMET with value 1 from .dat file mismatches hardcoded c++ define value of " << HELMET << "!" << std::endl;
#endif 

  
#ifdef HELMET_INDEX // DO NOT MODIFY!
  bsA = 0;
  bsB = HELMET_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined HELMET_INDEX with value 0 from .dat file mismatches hardcoded c++ define value of " << HELMET_INDEX << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_ATTRACTIVITY // DO NOT MODIFY!
  bsA = 5;
  bsB = HELM_OF_ATTRACTIVITY;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_ATTRACTIVITY with value 5 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_ATTRACTIVITY << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_BRILLIANCE // DO NOT MODIFY!
  bsA = 4;
  bsB = HELM_OF_BRILLIANCE;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_BRILLIANCE with value 4 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_BRILLIANCE << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_ENLIGHTENMENT // DO NOT MODIFY!
  bsA = 10;
  bsB = HELM_OF_ENLIGHTENMENT;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_ENLIGHTENMENT with value 10 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_ENLIGHTENMENT << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_MANA // DO NOT MODIFY!
  bsA = 11;
  bsB = HELM_OF_MANA;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_MANA with value 11 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_MANA << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_PERCEPTION // DO NOT MODIFY!
  bsA = 2;
  bsB = HELM_OF_PERCEPTION;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_PERCEPTION with value 2 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_PERCEPTION << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_TELEPATHY // DO NOT MODIFY!
  bsA = 8;
  bsB = HELM_OF_TELEPATHY;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_TELEPATHY with value 8 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_TELEPATHY << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_TELEPORTATION // DO NOT MODIFY!
  bsA = 9;
  bsB = HELM_OF_TELEPORTATION;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_TELEPORTATION with value 9 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_TELEPORTATION << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_UNDERSTANDING // DO NOT MODIFY!
  bsA = 3;
  bsB = HELM_OF_UNDERSTANDING;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_UNDERSTANDING with value 3 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_UNDERSTANDING << "!" << std::endl;
#endif 

  
#ifdef HELM_OF_WILLPOWER // DO NOT MODIFY!
  bsA = 7;
  bsB = HELM_OF_WILLPOWER;
  if(bsA!=bsB)
    ssErrors << "Defined HELM_OF_WILLPOWER with value 7 from .dat file mismatches hardcoded c++ define value of " << HELM_OF_WILLPOWER << "!" << std::endl;
#endif 

  
#ifdef HEMATITE // DO NOT MODIFY!
  bsA = 4324;
  bsB = HEMATITE;
  if(bsA!=bsB)
    ssErrors << "Defined HEMATITE with value 4324 from .dat file mismatches hardcoded c++ define value of " << HEMATITE << "!" << std::endl;
#endif 

  
#ifdef HEMP_CLOTH // DO NOT MODIFY!
  bsA = 4129;
  bsB = HEMP_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined HEMP_CLOTH with value 4129 from .dat file mismatches hardcoded c++ define value of " << HEMP_CLOTH << "!" << std::endl;
#endif 

  
#ifdef HEPATIZON // DO NOT MODIFY!
  bsA = 4104;
  bsB = HEPATIZON;
  if(bsA!=bsB)
    ssErrors << "Defined HEPATIZON with value 4104 from .dat file mismatches hardcoded c++ define value of " << HEPATIZON << "!" << std::endl;
#endif 

  
#ifdef HEXAGONAL_LOCK // DO NOT MODIFY!
  bsA = 17408;
  bsB = HEXAGONAL_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined HEXAGONAL_LOCK with value 17408 from .dat file mismatches hardcoded c++ define value of " << HEXAGONAL_LOCK << "!" << std::endl;
#endif 

  
#ifdef HICCUPS // DO NOT MODIFY!
  bsA = 1048576;
  bsB = HICCUPS;
  if(bsA!=bsB)
    ssErrors << "Defined HICCUPS with value 1048576 from .dat file mismatches hardcoded c++ define value of " << HICCUPS << "!" << std::endl;
#endif 

  
#ifdef HISPANIC_SIREN // DO NOT MODIFY!
  bsA = 9;
  bsB = HISPANIC_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined HISPANIC_SIREN with value 9 from .dat file mismatches hardcoded c++ define value of " << HISPANIC_SIREN << "!" << std::endl;
#endif 

  
#ifdef HM_ANTIDOTE // DO NOT MODIFY!
  bsA = 7;
  bsB = HM_ANTIDOTE;
  if(bsA!=bsB)
    ssErrors << "Defined HM_ANTIDOTE with value 7 from .dat file mismatches hardcoded c++ define value of " << HM_ANTIDOTE << "!" << std::endl;
#endif 

  
#ifdef HM_CONFUSE // DO NOT MODIFY!
  bsA = 8;
  bsB = HM_CONFUSE;
  if(bsA!=bsB)
    ssErrors << "Defined HM_CONFUSE with value 8 from .dat file mismatches hardcoded c++ define value of " << HM_CONFUSE << "!" << std::endl;
#endif 

  
#ifdef HM_FROG_FLESH // DO NOT MODIFY!
  bsA = 2;
  bsB = HM_FROG_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HM_FROG_FLESH with value 2 from .dat file mismatches hardcoded c++ define value of " << HM_FROG_FLESH << "!" << std::endl;
#endif 

  
#ifdef HM_HEALING_LIQUID // DO NOT MODIFY!
  bsA = 6;
  bsB = HM_HEALING_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined HM_HEALING_LIQUID with value 6 from .dat file mismatches hardcoded c++ define value of " << HM_HEALING_LIQUID << "!" << std::endl;
#endif 

  
#ifdef HM_HOLY_BANANA // DO NOT MODIFY!
  bsA = 9;
  bsB = HM_HOLY_BANANA;
  if(bsA!=bsB)
    ssErrors << "Defined HM_HOLY_BANANA with value 9 from .dat file mismatches hardcoded c++ define value of " << HM_HOLY_BANANA << "!" << std::endl;
#endif 

  
#ifdef HM_KOBOLD_FLESH // DO NOT MODIFY!
  bsA = 5;
  bsB = HM_KOBOLD_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HM_KOBOLD_FLESH with value 5 from .dat file mismatches hardcoded c++ define value of " << HM_KOBOLD_FLESH << "!" << std::endl;
#endif 

  
#ifdef HM_NOTHING // DO NOT MODIFY!
  bsA = 0;
  bsB = HM_NOTHING;
  if(bsA!=bsB)
    ssErrors << "Defined HM_NOTHING with value 0 from .dat file mismatches hardcoded c++ define value of " << HM_NOTHING << "!" << std::endl;
#endif 

  
#ifdef HM_OMMEL // DO NOT MODIFY!
  bsA = 3;
  bsB = HM_OMMEL;
  if(bsA!=bsB)
    ssErrors << "Defined HM_OMMEL with value 3 from .dat file mismatches hardcoded c++ define value of " << HM_OMMEL << "!" << std::endl;
#endif 

  
#ifdef HM_PEPSI // DO NOT MODIFY!
  bsA = 4;
  bsB = HM_PEPSI;
  if(bsA!=bsB)
    ssErrors << "Defined HM_PEPSI with value 4 from .dat file mismatches hardcoded c++ define value of " << HM_PEPSI << "!" << std::endl;
#endif 

  
#ifdef HM_SCHOOL_FOOD // DO NOT MODIFY!
  bsA = 1;
  bsB = HM_SCHOOL_FOOD;
  if(bsA!=bsB)
    ssErrors << "Defined HM_SCHOOL_FOOD with value 1 from .dat file mismatches hardcoded c++ define value of " << HM_SCHOOL_FOOD << "!" << std::endl;
#endif 

  
#ifdef HOARD_MASTER // DO NOT MODIFY!
  bsA = 3;
  bsB = HOARD_MASTER;
  if(bsA!=bsB)
    ssErrors << "Defined HOARD_MASTER with value 3 from .dat file mismatches hardcoded c++ define value of " << HOARD_MASTER << "!" << std::endl;
#endif 

  
#ifdef HOLLY_WOOD // DO NOT MODIFY!
  bsA = 4232;
  bsB = HOLLY_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined HOLLY_WOOD with value 4232 from .dat file mismatches hardcoded c++ define value of " << HOLLY_WOOD << "!" << std::endl;
#endif 

  
#ifdef HOLY_BANANA_FLESH // DO NOT MODIFY!
  bsA = 8197;
  bsB = HOLY_BANANA_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HOLY_BANANA_FLESH with value 8197 from .dat file mismatches hardcoded c++ define value of " << HOLY_BANANA_FLESH << "!" << std::endl;
#endif 

  
#ifdef HOLY_TREE // DO NOT MODIFY!
  bsA = 3;
  bsB = HOLY_TREE;
  if(bsA!=bsB)
    ssErrors << "Defined HOLY_TREE with value 3 from .dat file mismatches hardcoded c++ define value of " << HOLY_TREE << "!" << std::endl;
#endif 

  
#ifdef HONEY // DO NOT MODIFY!
  bsA = 16394;
  bsB = HONEY;
  if(bsA!=bsB)
    ssErrors << "Defined HONEY with value 16394 from .dat file mismatches hardcoded c++ define value of " << HONEY << "!" << std::endl;
#endif 

  
#ifdef HONOR // DO NOT MODIFY!
  bsA = 11;
  bsB = HONOR;
  if(bsA!=bsB)
    ssErrors << "Defined HONOR with value 11 from .dat file mismatches hardcoded c++ define value of " << HONOR << "!" << std::endl;
#endif 

  
#ifdef HOSTILE // DO NOT MODIFY!
  bsA = 1;
  bsB = HOSTILE;
  if(bsA!=bsB)
    ssErrors << "Defined HOSTILE with value 1 from .dat file mismatches hardcoded c++ define value of " << HOSTILE << "!" << std::endl;
#endif 

  
#ifdef HUMAN_FLESH // DO NOT MODIFY!
  bsA = 20491;
  bsB = HUMAN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined HUMAN_FLESH with value 20491 from .dat file mismatches hardcoded c++ define value of " << HUMAN_FLESH << "!" << std::endl;
#endif 

  
#ifdef HUMAN_NAIL // DO NOT MODIFY!
  bsA = 4248;
  bsB = HUMAN_NAIL;
  if(bsA!=bsB)
    ssErrors << "Defined HUMAN_NAIL with value 4248 from .dat file mismatches hardcoded c++ define value of " << HUMAN_NAIL << "!" << std::endl;
#endif 

  
#ifdef HUMAN_SKIN // DO NOT MODIFY!
  bsA = 4141;
  bsB = HUMAN_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined HUMAN_SKIN with value 4141 from .dat file mismatches hardcoded c++ define value of " << HUMAN_SKIN << "!" << std::endl;
#endif 

  
#ifdef HUNTER // DO NOT MODIFY!
  bsA = 2;
  bsB = HUNTER;
  if(bsA!=bsB)
    ssErrors << "Defined HUNTER with value 2 from .dat file mismatches hardcoded c++ define value of " << HUNTER << "!" << std::endl;
#endif 

  
#ifdef HUSBAND // DO NOT MODIFY!
  bsA = 1;
  bsB = HUSBAND;
  if(bsA!=bsB)
    ssErrors << "Defined HUSBAND with value 1 from .dat file mismatches hardcoded c++ define value of " << HUSBAND << "!" << std::endl;
#endif 

  
#ifdef HYDRA_HIDE // DO NOT MODIFY!
  bsA = 4149;
  bsB = HYDRA_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined HYDRA_HIDE with value 4149 from .dat file mismatches hardcoded c++ define value of " << HYDRA_HIDE << "!" << std::endl;
#endif 

  
#ifdef ICE // DO NOT MODIFY!
  bsA = 4209;
  bsB = ICE;
  if(bsA!=bsB)
    ssErrors << "Defined ICE with value 4209 from .dat file mismatches hardcoded c++ define value of " << ICE << "!" << std::endl;
#endif 

  
#ifdef ICE_WALL // DO NOT MODIFY!
  bsA = 7;
  bsB = ICE_WALL;
  if(bsA!=bsB)
    ssErrors << "Defined ICE_WALL with value 7 from .dat file mismatches hardcoded c++ define value of " << ICE_WALL << "!" << std::endl;
#endif 

  
#ifdef IGNORE_BROKEN_PRICE // DO NOT MODIFY!
  bsA = 2;
  bsB = IGNORE_BROKEN_PRICE;
  if(bsA!=bsB)
    ssErrors << "Defined IGNORE_BROKEN_PRICE with value 2 from .dat file mismatches hardcoded c++ define value of " << IGNORE_BROKEN_PRICE << "!" << std::endl;
#endif 

  
#ifdef ILLITHIUM // DO NOT MODIFY!
  bsA = 4208;
  bsB = ILLITHIUM;
  if(bsA!=bsB)
    ssErrors << "Defined ILLITHIUM with value 4208 from .dat file mismatches hardcoded c++ define value of " << ILLITHIUM << "!" << std::endl;
#endif 

  
#ifdef IMPRISONED_FARMER // DO NOT MODIFY!
  bsA = 1;
  bsB = IMPRISONED_FARMER;
  if(bsA!=bsB)
    ssErrors << "Defined IMPRISONED_FARMER with value 1 from .dat file mismatches hardcoded c++ define value of " << IMPRISONED_FARMER << "!" << std::endl;
#endif 

  
#ifdef IMPRISONED_FEMALE // DO NOT MODIFY!
  bsA = 1;
  bsB = IMPRISONED_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined IMPRISONED_FEMALE with value 1 from .dat file mismatches hardcoded c++ define value of " << IMPRISONED_FEMALE << "!" << std::endl;
#endif 

  
#ifdef IMPRISONED_HUNTER // DO NOT MODIFY!
  bsA = 1;
  bsB = IMPRISONED_HUNTER;
  if(bsA!=bsB)
    ssErrors << "Defined IMPRISONED_HUNTER with value 1 from .dat file mismatches hardcoded c++ define value of " << IMPRISONED_HUNTER << "!" << std::endl;
#endif 

  
#ifdef IMPRISONED_NECROMANCER // DO NOT MODIFY!
  bsA = 9;
  bsB = IMPRISONED_NECROMANCER;
  if(bsA!=bsB)
    ssErrors << "Defined IMPRISONED_NECROMANCER with value 9 from .dat file mismatches hardcoded c++ define value of " << IMPRISONED_NECROMANCER << "!" << std::endl;
#endif 

  
#ifdef IMPRISONED_ZOMBIE // DO NOT MODIFY!
  bsA = 2;
  bsB = IMPRISONED_ZOMBIE;
  if(bsA!=bsB)
    ssErrors << "Defined IMPRISONED_ZOMBIE with value 2 from .dat file mismatches hardcoded c++ define value of " << IMPRISONED_ZOMBIE << "!" << std::endl;
#endif 

  
#ifdef IMP_HIDE // DO NOT MODIFY!
  bsA = 4157;
  bsB = IMP_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined IMP_HIDE with value 4157 from .dat file mismatches hardcoded c++ define value of " << IMP_HIDE << "!" << std::endl;
#endif 

  
#ifdef INDEFINE_BIT // DO NOT MODIFY!
  bsA = 4;
  bsB = INDEFINE_BIT;
  if(bsA!=bsB)
    ssErrors << "Defined INDEFINE_BIT with value 4 from .dat file mismatches hardcoded c++ define value of " << INDEFINE_BIT << "!" << std::endl;
#endif 

  
#ifdef INDEFINITE // DO NOT MODIFY!
  bsA = 6;
  bsB = INDEFINITE;
  if(bsA!=bsB)
    ssErrors << "Defined INDEFINITE with value 6 from .dat file mismatches hardcoded c++ define value of " << INDEFINITE << "!" << std::endl;
#endif 

  
#ifdef INFRA_VISION // DO NOT MODIFY!
  bsA = 128;
  bsB = INFRA_VISION;
  if(bsA!=bsB)
    ssErrors << "Defined INFRA_VISION with value 128 from .dat file mismatches hardcoded c++ define value of " << INFRA_VISION << "!" << std::endl;
#endif 

  
#ifdef INFUSCOR // DO NOT MODIFY!
  bsA = 13;
  bsB = INFUSCOR;
  if(bsA!=bsB)
    ssErrors << "Defined INFUSCOR with value 13 from .dat file mismatches hardcoded c++ define value of " << INFUSCOR << "!" << std::endl;
#endif 

  
#ifdef INGOT // DO NOT MODIFY!
  bsA = 2;
  bsB = INGOT;
  if(bsA!=bsB)
    ssErrors << "Defined INGOT with value 2 from .dat file mismatches hardcoded c++ define value of " << INGOT << "!" << std::endl;
#endif 

  
#ifdef INK // DO NOT MODIFY!
  bsA = 16396;
  bsB = INK;
  if(bsA!=bsB)
    ssErrors << "Defined INK with value 16396 from .dat file mismatches hardcoded c++ define value of " << INK << "!" << std::endl;
#endif 

  
#ifdef INTELLIGENCE // DO NOT MODIFY!
  bsA = 2;
  bsB = INTELLIGENCE;
  if(bsA!=bsB)
    ssErrors << "Defined INTELLIGENCE with value 2 from .dat file mismatches hardcoded c++ define value of " << INTELLIGENCE << "!" << std::endl;
#endif 

  
#ifdef INVISIBLE // DO NOT MODIFY!
  bsA = 64;
  bsB = INVISIBLE;
  if(bsA!=bsB)
    ssErrors << "Defined INVISIBLE with value 64 from .dat file mismatches hardcoded c++ define value of " << INVISIBLE << "!" << std::endl;
#endif 

  
#ifdef IN_ROOM // DO NOT MODIFY!
  bsA = 4;
  bsB = IN_ROOM;
  if(bsA!=bsB)
    ssErrors << "Defined IN_ROOM with value 4 from .dat file mismatches hardcoded c++ define value of " << IN_ROOM << "!" << std::endl;
#endif 

  
#ifdef IRIDIUM // DO NOT MODIFY!
  bsA = 4124;
  bsB = IRIDIUM;
  if(bsA!=bsB)
    ssErrors << "Defined IRIDIUM with value 4124 from .dat file mismatches hardcoded c++ define value of " << IRIDIUM << "!" << std::endl;
#endif 

  
#ifdef IRON // DO NOT MODIFY!
  bsA = 28674;
  bsB = IRON;
  if(bsA!=bsB)
    ssErrors << "Defined IRON with value 28674 from .dat file mismatches hardcoded c++ define value of " << IRON << "!" << std::endl;
#endif 

  
#ifdef IRON_ALLOY_ID // DO NOT MODIFY!
  bsA = 28672;
  bsB = IRON_ALLOY_ID;
  if(bsA!=bsB)
    ssErrors << "Defined IRON_ALLOY_ID with value 28672 from .dat file mismatches hardcoded c++ define value of " << IRON_ALLOY_ID << "!" << std::endl;
#endif 

  
#ifdef IS_ABSTRACT // DO NOT MODIFY!
  bsA = 1;
  bsB = IS_ABSTRACT;
  if(bsA!=bsB)
    ssErrors << "Defined IS_ABSTRACT with value 1 from .dat file mismatches hardcoded c++ define value of " << IS_ABSTRACT << "!" << std::endl;
#endif 

  
#ifdef IS_AFFECTED_BY_MUSTARD_GAS // DO NOT MODIFY!
  bsA = 32;
  bsB = IS_AFFECTED_BY_MUSTARD_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined IS_AFFECTED_BY_MUSTARD_GAS with value 32 from .dat file mismatches hardcoded c++ define value of " << IS_AFFECTED_BY_MUSTARD_GAS << "!" << std::endl;
#endif 

  
#ifdef IS_ALIVE // DO NOT MODIFY!
  bsA = 1;
  bsB = IS_ALIVE;
  if(bsA!=bsB)
    ssErrors << "Defined IS_ALIVE with value 1 from .dat file mismatches hardcoded c++ define value of " << IS_ALIVE << "!" << std::endl;
#endif 

  
#ifdef IS_BEVERAGE // DO NOT MODIFY!
  bsA = 64;
  bsB = IS_BEVERAGE;
  if(bsA!=bsB)
    ssErrors << "Defined IS_BEVERAGE with value 64 from .dat file mismatches hardcoded c++ define value of " << IS_BEVERAGE << "!" << std::endl;
#endif 

  
#ifdef IS_BLOOD // DO NOT MODIFY!
  bsA = 2;
  bsB = IS_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined IS_BLOOD with value 2 from .dat file mismatches hardcoded c++ define value of " << IS_BLOOD << "!" << std::endl;
#endif 

  
#ifdef IS_GOLEM_MATERIAL // DO NOT MODIFY!
  bsA = 32;
  bsB = IS_GOLEM_MATERIAL;
  if(bsA!=bsB)
    ssErrors << "Defined IS_GOLEM_MATERIAL with value 32 from .dat file mismatches hardcoded c++ define value of " << IS_GOLEM_MATERIAL << "!" << std::endl;
#endif 

  
#ifdef IS_LEADER // DO NOT MODIFY!
  bsA = 1;
  bsB = IS_LEADER;
  if(bsA!=bsB)
    ssErrors << "Defined IS_LEADER with value 1 from .dat file mismatches hardcoded c++ define value of " << IS_LEADER << "!" << std::endl;
#endif 

  
#ifdef IS_MASTER // DO NOT MODIFY!
  bsA = 2;
  bsB = IS_MASTER;
  if(bsA!=bsB)
    ssErrors << "Defined IS_MASTER with value 2 from .dat file mismatches hardcoded c++ define value of " << IS_MASTER << "!" << std::endl;
#endif 

  
#ifdef IS_METAL // DO NOT MODIFY!
  bsA = 1;
  bsB = IS_METAL;
  if(bsA!=bsB)
    ssErrors << "Defined IS_METAL with value 1 from .dat file mismatches hardcoded c++ define value of " << IS_METAL << "!" << std::endl;
#endif 

  
#ifdef IS_SCARY // DO NOT MODIFY!
  bsA = 16;
  bsB = IS_SCARY;
  if(bsA!=bsB)
    ssErrors << "Defined IS_SCARY with value 16 from .dat file mismatches hardcoded c++ define value of " << IS_SCARY << "!" << std::endl;
#endif 

  
#ifdef IS_SPARKLING // DO NOT MODIFY!
  bsA = 8;
  bsB = IS_SPARKLING;
  if(bsA!=bsB)
    ssErrors << "Defined IS_SPARKLING with value 8 from .dat file mismatches hardcoded c++ define value of " << IS_SPARKLING << "!" << std::endl;
#endif 

  
#ifdef IS_VALUABLE // DO NOT MODIFY!
  bsA = 32;
  bsB = IS_VALUABLE;
  if(bsA!=bsB)
    ssErrors << "Defined IS_VALUABLE with value 32 from .dat file mismatches hardcoded c++ define value of " << IS_VALUABLE << "!" << std::endl;
#endif 

  
#ifdef IS_WARM // DO NOT MODIFY!
  bsA = 2;
  bsB = IS_WARM;
  if(bsA!=bsB)
    ssErrors << "Defined IS_WARM with value 2 from .dat file mismatches hardcoded c++ define value of " << IS_WARM << "!" << std::endl;
#endif 

  
#ifdef IS_WARM_BLOODED // DO NOT MODIFY!
  bsA = 32;
  bsB = IS_WARM_BLOODED;
  if(bsA!=bsB)
    ssErrors << "Defined IS_WARM_BLOODED with value 32 from .dat file mismatches hardcoded c++ define value of " << IS_WARM_BLOODED << "!" << std::endl;
#endif 

  
#ifdef IVAN_LEVEL // DO NOT MODIFY!
  bsA = 7;
  bsB = IVAN_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined IVAN_LEVEL with value 7 from .dat file mismatches hardcoded c++ define value of " << IVAN_LEVEL << "!" << std::endl;
#endif 

  
#ifdef IVAN_TEAM // DO NOT MODIFY!
  bsA = 6;
  bsB = IVAN_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined IVAN_TEAM with value 6 from .dat file mismatches hardcoded c++ define value of " << IVAN_TEAM << "!" << std::endl;
#endif 

  
#ifdef JACINTH // DO NOT MODIFY!
  bsA = 4266;
  bsB = JACINTH;
  if(bsA!=bsB)
    ssErrors << "Defined JACINTH with value 4266 from .dat file mismatches hardcoded c++ define value of " << JACINTH << "!" << std::endl;
#endif 

  
#ifdef JACKAL_FLESH // DO NOT MODIFY!
  bsA = 20498;
  bsB = JACKAL_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined JACKAL_FLESH with value 20498 from .dat file mismatches hardcoded c++ define value of " << JACKAL_FLESH << "!" << std::endl;
#endif 

  
#ifdef JADEITE // DO NOT MODIFY!
  bsA = 4300;
  bsB = JADEITE;
  if(bsA!=bsB)
    ssErrors << "Defined JADEITE with value 4300 from .dat file mismatches hardcoded c++ define value of " << JADEITE << "!" << std::endl;
#endif 

  
#ifdef JASPER // DO NOT MODIFY!
  bsA = 4265;
  bsB = JASPER;
  if(bsA!=bsB)
    ssErrors << "Defined JASPER with value 4265 from .dat file mismatches hardcoded c++ define value of " << JASPER << "!" << std::endl;
#endif 

  
#ifdef JESTER // DO NOT MODIFY!
  bsA = 4;
  bsB = JESTER;
  if(bsA!=bsB)
    ssErrors << "Defined JESTER with value 4 from .dat file mismatches hardcoded c++ define value of " << JESTER << "!" << std::endl;
#endif 

  
#ifdef JET // DO NOT MODIFY!
  bsA = 4194;
  bsB = JET;
  if(bsA!=bsB)
    ssErrors << "Defined JET with value 4194 from .dat file mismatches hardcoded c++ define value of " << JET << "!" << std::endl;
#endif 

  
#ifdef JOTUN_ICE // DO NOT MODIFY!
  bsA = 4211;
  bsB = JOTUN_ICE;
  if(bsA!=bsB)
    ssErrors << "Defined JOTUN_ICE with value 4211 from .dat file mismatches hardcoded c++ define value of " << JOTUN_ICE << "!" << std::endl;
#endif 

  
#ifdef JUNGLE // DO NOT MODIFY!
  bsA = 2;
  bsB = JUNGLE;
  if(bsA!=bsB)
    ssErrors << "Defined JUNGLE with value 2 from .dat file mismatches hardcoded c++ define value of " << JUNGLE << "!" << std::endl;
#endif 

  
#ifdef KAURI_WOOD // DO NOT MODIFY!
  bsA = 4244;
  bsB = KAURI_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined KAURI_WOOD with value 4244 from .dat file mismatches hardcoded c++ define value of " << KAURI_WOOD << "!" << std::endl;
#endif 

  
#ifdef KELP // DO NOT MODIFY!
  bsA = 8202;
  bsB = KELP;
  if(bsA!=bsB)
    ssErrors << "Defined KELP with value 8202 from .dat file mismatches hardcoded c++ define value of " << KELP << "!" << std::endl;
#endif 

  
#ifdef KEVLAR // DO NOT MODIFY!
  bsA = 4181;
  bsB = KEVLAR;
  if(bsA!=bsB)
    ssErrors << "Defined KEVLAR with value 4181 from .dat file mismatches hardcoded c++ define value of " << KEVLAR << "!" << std::endl;
#endif 

  
#ifdef KICK // DO NOT MODIFY!
  bsA = 1;
  bsB = KICK;
  if(bsA!=bsB)
    ssErrors << "Defined KICK with value 1 from .dat file mismatches hardcoded c++ define value of " << KICK << "!" << std::endl;
#endif 

  
#ifdef KICK_ATTACK // DO NOT MODIFY!
  bsA = 2;
  bsB = KICK_ATTACK;
  if(bsA!=bsB)
    ssErrors << "Defined KICK_ATTACK with value 2 from .dat file mismatches hardcoded c++ define value of " << KICK_ATTACK << "!" << std::endl;
#endif 

  
#ifdef KING // DO NOT MODIFY!
  bsA = 4;
  bsB = KING;
  if(bsA!=bsB)
    ssErrors << "Defined KING with value 4 from .dat file mismatches hardcoded c++ define value of " << KING << "!" << std::endl;
#endif 

  
#ifdef KIWI_FLESH // DO NOT MODIFY!
  bsA = 8198;
  bsB = KIWI_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined KIWI_FLESH with value 8198 from .dat file mismatches hardcoded c++ define value of " << KIWI_FLESH << "!" << std::endl;
#endif 

  
#ifdef KOBOLD_FLESH // DO NOT MODIFY!
  bsA = 20502;
  bsB = KOBOLD_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined KOBOLD_FLESH with value 20502 from .dat file mismatches hardcoded c++ define value of " << KOBOLD_FLESH << "!" << std::endl;
#endif 

  
#ifdef KRAKEN_HIDE // DO NOT MODIFY!
  bsA = 4171;
  bsB = KRAKEN_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined KRAKEN_HIDE with value 4171 from .dat file mismatches hardcoded c++ define value of " << KRAKEN_HIDE << "!" << std::endl;
#endif 

  
#ifdef LANDING_SITE // DO NOT MODIFY!
  bsA = 5;
  bsB = LANDING_SITE;
  if(bsA!=bsB)
    ssErrors << "Defined LANDING_SITE with value 5 from .dat file mismatches hardcoded c++ define value of " << LANDING_SITE << "!" << std::endl;
#endif 

  
#ifdef LAPIS_LAZULI // DO NOT MODIFY!
  bsA = 4279;
  bsB = LAPIS_LAZULI;
  if(bsA!=bsB)
    ssErrors << "Defined LAPIS_LAZULI with value 4279 from .dat file mismatches hardcoded c++ define value of " << LAPIS_LAZULI << "!" << std::endl;
#endif 

  
#ifdef LARGE // DO NOT MODIFY!
  bsA = 1;
  bsB = LARGE;
  if(bsA!=bsB)
    ssErrors << "Defined LARGE with value 1 from .dat file mismatches hardcoded c++ define value of " << LARGE << "!" << std::endl;
#endif 

  
#ifdef LARGE_CHEST // DO NOT MODIFY!
  bsA = 3;
  bsB = LARGE_CHEST;
  if(bsA!=bsB)
    ssErrors << "Defined LARGE_CHEST with value 3 from .dat file mismatches hardcoded c++ define value of " << LARGE_CHEST << "!" << std::endl;
#endif 

  
#ifdef LARGE_CLOCK // DO NOT MODIFY!
  bsA = 6;
  bsB = LARGE_CLOCK;
  if(bsA!=bsB)
    ssErrors << "Defined LARGE_CLOCK with value 6 from .dat file mismatches hardcoded c++ define value of " << LARGE_CLOCK << "!" << std::endl;
#endif 

  
#ifdef LARGE_SWORDS // DO NOT MODIFY!
  bsA = 5;
  bsB = LARGE_SWORDS;
  if(bsA!=bsB)
    ssErrors << "Defined LARGE_SWORDS with value 5 from .dat file mismatches hardcoded c++ define value of " << LARGE_SWORDS << "!" << std::endl;
#endif 

  
#ifdef LATEX // DO NOT MODIFY!
  bsA = 4184;
  bsB = LATEX;
  if(bsA!=bsB)
    ssErrors << "Defined LATEX with value 4184 from .dat file mismatches hardcoded c++ define value of " << LATEX << "!" << std::endl;
#endif 

  
#ifdef LAW_STUDENT // DO NOT MODIFY!
  bsA = 1;
  bsB = LAW_STUDENT;
  if(bsA!=bsB)
    ssErrors << "Defined LAW_STUDENT with value 1 from .dat file mismatches hardcoded c++ define value of " << LAW_STUDENT << "!" << std::endl;
#endif 

  
#ifdef LEAD // DO NOT MODIFY!
  bsA = 4114;
  bsB = LEAD;
  if(bsA!=bsB)
    ssErrors << "Defined LEAD with value 4114 from .dat file mismatches hardcoded c++ define value of " << LEAD << "!" << std::endl;
#endif 

  
#ifdef LEAF // DO NOT MODIFY!
  bsA = 4216;
  bsB = LEAF;
  if(bsA!=bsB)
    ssErrors << "Defined LEAF with value 4216 from .dat file mismatches hardcoded c++ define value of " << LEAF << "!" << std::endl;
#endif 

  
#ifdef LEAFY_FOREST // DO NOT MODIFY!
  bsA = 4;
  bsB = LEAFY_FOREST;
  if(bsA!=bsB)
    ssErrors << "Defined LEAFY_FOREST with value 4 from .dat file mismatches hardcoded c++ define value of " << LEAFY_FOREST << "!" << std::endl;
#endif 

  
#ifdef LEATHER // DO NOT MODIFY!
  bsA = 4150;
  bsB = LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined LEATHER with value 4150 from .dat file mismatches hardcoded c++ define value of " << LEATHER << "!" << std::endl;
#endif 

  
#ifdef LEFT // DO NOT MODIFY!
  bsA = 0;
  bsB = LEFT;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT with value 0 from .dat file mismatches hardcoded c++ define value of " << LEFT << "!" << std::endl;
#endif 

  
#ifdef LEFT_ARM // DO NOT MODIFY!
  bsA = 8;
  bsB = LEFT_ARM;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_ARM with value 8 from .dat file mismatches hardcoded c++ define value of " << LEFT_ARM << "!" << std::endl;
#endif 

  
#ifdef LEFT_ARM_INDEX // DO NOT MODIFY!
  bsA = 3;
  bsB = LEFT_ARM_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_ARM_INDEX with value 3 from .dat file mismatches hardcoded c++ define value of " << LEFT_ARM_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEFT_BOOT_INDEX // DO NOT MODIFY!
  bsA = 12;
  bsB = LEFT_BOOT_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_BOOT_INDEX with value 12 from .dat file mismatches hardcoded c++ define value of " << LEFT_BOOT_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEFT_GAUNTLET_INDEX // DO NOT MODIFY!
  bsA = 10;
  bsB = LEFT_GAUNTLET_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_GAUNTLET_INDEX with value 10 from .dat file mismatches hardcoded c++ define value of " << LEFT_GAUNTLET_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEFT_LEG // DO NOT MODIFY!
  bsA = 64;
  bsB = LEFT_LEG;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_LEG with value 64 from .dat file mismatches hardcoded c++ define value of " << LEFT_LEG << "!" << std::endl;
#endif 

  
#ifdef LEFT_LEG_INDEX // DO NOT MODIFY!
  bsA = 6;
  bsB = LEFT_LEG_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_LEG_INDEX with value 6 from .dat file mismatches hardcoded c++ define value of " << LEFT_LEG_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEFT_RING_INDEX // DO NOT MODIFY!
  bsA = 8;
  bsB = LEFT_RING_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_RING_INDEX with value 8 from .dat file mismatches hardcoded c++ define value of " << LEFT_RING_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEFT_WIELDED_INDEX // DO NOT MODIFY!
  bsA = 6;
  bsB = LEFT_WIELDED_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined LEFT_WIELDED_INDEX with value 6 from .dat file mismatches hardcoded c++ define value of " << LEFT_WIELDED_INDEX << "!" << std::endl;
#endif 

  
#ifdef LEGIFER // DO NOT MODIFY!
  bsA = 2;
  bsB = LEGIFER;
  if(bsA!=bsB)
    ssErrors << "Defined LEGIFER with value 2 from .dat file mismatches hardcoded c++ define value of " << LEGIFER << "!" << std::endl;
#endif 

  
#ifdef LEGS // DO NOT MODIFY!
  bsA = 96;
  bsB = LEGS;
  if(bsA!=bsB)
    ssErrors << "Defined LEGS with value 96 from .dat file mismatches hardcoded c++ define value of " << LEGS << "!" << std::endl;
#endif 

  
#ifdef LEG_MAIN_COLOR // DO NOT MODIFY!
  bsA = 2048;
  bsB = LEG_MAIN_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined LEG_MAIN_COLOR with value 2048 from .dat file mismatches hardcoded c++ define value of " << LEG_MAIN_COLOR << "!" << std::endl;
#endif 

  
#ifdef LEG_SPECIAL_COLOR // DO NOT MODIFY!
  bsA = 4096;
  bsB = LEG_SPECIAL_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined LEG_SPECIAL_COLOR with value 4096 from .dat file mismatches hardcoded c++ define value of " << LEG_SPECIAL_COLOR << "!" << std::endl;
#endif 

  
#ifdef LEG_STRENGTH // DO NOT MODIFY!
  bsA = 8;
  bsB = LEG_STRENGTH;
  if(bsA!=bsB)
    ssErrors << "Defined LEG_STRENGTH with value 8 from .dat file mismatches hardcoded c++ define value of " << LEG_STRENGTH << "!" << std::endl;
#endif 

  
#ifdef LEPROSY // DO NOT MODIFY!
  bsA = 524288;
  bsB = LEPROSY;
  if(bsA!=bsB)
    ssErrors << "Defined LEPROSY with value 524288 from .dat file mismatches hardcoded c++ define value of " << LEPROSY << "!" << std::endl;
#endif 

  
#ifdef LEVIATHAN_HIDE // DO NOT MODIFY!
  bsA = 4172;
  bsB = LEVIATHAN_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined LEVIATHAN_HIDE with value 4172 from .dat file mismatches hardcoded c++ define value of " << LEVIATHAN_HIDE << "!" << std::endl;
#endif 

  
#ifdef LEVITATION // DO NOT MODIFY!
  bsA = 262144;
  bsB = LEVITATION;
  if(bsA!=bsB)
    ssErrors << "Defined LEVITATION with value 262144 from .dat file mismatches hardcoded c++ define value of " << LEVITATION << "!" << std::endl;
#endif 

  
#ifdef LIFE_SAVED // DO NOT MODIFY!
  bsA = 16;
  bsB = LIFE_SAVED;
  if(bsA!=bsB)
    ssErrors << "Defined LIFE_SAVED with value 16 from .dat file mismatches hardcoded c++ define value of " << LIFE_SAVED << "!" << std::endl;
#endif 

  
#ifdef LIGHT // DO NOT MODIFY!
  bsA = 4;
  bsB = LIGHT;
  if(bsA!=bsB)
    ssErrors << "Defined LIGHT with value 4 from .dat file mismatches hardcoded c++ define value of " << LIGHT << "!" << std::endl;
#endif 

  
#ifdef LIGHTNING_BEAM // DO NOT MODIFY!
  bsA = 1;
  bsB = LIGHTNING_BEAM;
  if(bsA!=bsB)
    ssErrors << "Defined LIGHTNING_BEAM with value 1 from .dat file mismatches hardcoded c++ define value of " << LIGHTNING_BEAM << "!" << std::endl;
#endif 

  
#ifdef LIGHT_ASIAN_SIREN // DO NOT MODIFY!
  bsA = 1;
  bsB = LIGHT_ASIAN_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined LIGHT_ASIAN_SIREN with value 1 from .dat file mismatches hardcoded c++ define value of " << LIGHT_ASIAN_SIREN << "!" << std::endl;
#endif 

  
#ifdef LIGHT_FROG_BLOOD // DO NOT MODIFY!
  bsA = 16429;
  bsB = LIGHT_FROG_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined LIGHT_FROG_BLOOD with value 16429 from .dat file mismatches hardcoded c++ define value of " << LIGHT_FROG_BLOOD << "!" << std::endl;
#endif 

  
#ifdef LIGHT_GRAY // DO NOT MODIFY!
  bsA = 38066;
  bsB = LIGHT_GRAY;
  if(bsA!=bsB)
    ssErrors << "Defined LIGHT_GRAY with value 38066 from .dat file mismatches hardcoded c++ define value of " << LIGHT_GRAY << "!" << std::endl;
#endif 

  
#ifdef LIME_STONE // DO NOT MODIFY!
  bsA = 4260;
  bsB = LIME_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined LIME_STONE with value 4260 from .dat file mismatches hardcoded c++ define value of " << LIME_STONE << "!" << std::endl;
#endif 

  
#ifdef LINEN_CLOTH // DO NOT MODIFY!
  bsA = 4128;
  bsB = LINEN_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined LINEN_CLOTH with value 4128 from .dat file mismatches hardcoded c++ define value of " << LINEN_CLOTH << "!" << std::endl;
#endif 

  
#ifdef LION_FLESH // DO NOT MODIFY!
  bsA = 20513;
  bsB = LION_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined LION_FLESH with value 20513 from .dat file mismatches hardcoded c++ define value of " << LION_FLESH << "!" << std::endl;
#endif 

  
#ifdef LIQUID_DARKNESS // DO NOT MODIFY!
  bsA = 16413;
  bsB = LIQUID_DARKNESS;
  if(bsA!=bsB)
    ssErrors << "Defined LIQUID_DARKNESS with value 16413 from .dat file mismatches hardcoded c++ define value of " << LIQUID_DARKNESS << "!" << std::endl;
#endif 

  
#ifdef LIQUID_FEAR // DO NOT MODIFY!
  bsA = 16414;
  bsB = LIQUID_FEAR;
  if(bsA!=bsB)
    ssErrors << "Defined LIQUID_FEAR with value 16414 from .dat file mismatches hardcoded c++ define value of " << LIQUID_FEAR << "!" << std::endl;
#endif 

  
#ifdef LIQUID_ID // DO NOT MODIFY!
  bsA = 16384;
  bsB = LIQUID_ID;
  if(bsA!=bsB)
    ssErrors << "Defined LIQUID_ID with value 16384 from .dat file mismatches hardcoded c++ define value of " << LIQUID_ID << "!" << std::endl;
#endif 

  
#ifdef LOCKED // DO NOT MODIFY!
  bsA = 1;
  bsB = LOCKED;
  if(bsA!=bsB)
    ssErrors << "Defined LOCKED with value 1 from .dat file mismatches hardcoded c++ define value of " << LOCKED << "!" << std::endl;
#endif 

  
#ifdef LOCK_DELTA // DO NOT MODIFY!
  bsA = 1024;
  bsB = LOCK_DELTA;
  if(bsA!=bsB)
    ssErrors << "Defined LOCK_DELTA with value 1024 from .dat file mismatches hardcoded c++ define value of " << LOCK_DELTA << "!" << std::endl;
#endif 

  
#ifdef LONG_SWORD // DO NOT MODIFY!
  bsA = 1;
  bsB = LONG_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined LONG_SWORD with value 1 from .dat file mismatches hardcoded c++ define value of " << LONG_SWORD << "!" << std::endl;
#endif 

  
#ifdef LORD // DO NOT MODIFY!
  bsA = 1;
  bsB = LORD;
  if(bsA!=bsB)
    ssErrors << "Defined LORD with value 1 from .dat file mismatches hardcoded c++ define value of " << LORD << "!" << std::endl;
#endif 

  
#ifdef LORICATUS // DO NOT MODIFY!
  bsA = 8;
  bsB = LORICATUS;
  if(bsA!=bsB)
    ssErrors << "Defined LORICATUS with value 8 from .dat file mismatches hardcoded c++ define value of " << LORICATUS << "!" << std::endl;
#endif 

  
#ifdef LOST_RUBY_FLAMING_SWORD // DO NOT MODIFY!
  bsA = 2;
  bsB = LOST_RUBY_FLAMING_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined LOST_RUBY_FLAMING_SWORD with value 2 from .dat file mismatches hardcoded c++ define value of " << LOST_RUBY_FLAMING_SWORD << "!" << std::endl;
#endif 

  
#ifdef LYCANTHROPY // DO NOT MODIFY!
  bsA = 32;
  bsB = LYCANTHROPY;
  if(bsA!=bsB)
    ssErrors << "Defined LYCANTHROPY with value 32 from .dat file mismatches hardcoded c++ define value of " << LYCANTHROPY << "!" << std::endl;
#endif 

  
#ifdef MACE // DO NOT MODIFY!
  bsA = 7;
  bsB = MACE;
  if(bsA!=bsB)
    ssErrors << "Defined MACE with value 7 from .dat file mismatches hardcoded c++ define value of " << MACE << "!" << std::endl;
#endif 

  
#ifdef MAGE_STAFF // DO NOT MODIFY!
  bsA = 18;
  bsB = MAGE_STAFF;
  if(bsA!=bsB)
    ssErrors << "Defined MAGE_STAFF with value 18 from .dat file mismatches hardcoded c++ define value of " << MAGE_STAFF << "!" << std::endl;
#endif 

  
#ifdef MAGIC_CRYSTAL // DO NOT MODIFY!
  bsA = 4201;
  bsB = MAGIC_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined MAGIC_CRYSTAL with value 4201 from .dat file mismatches hardcoded c++ define value of " << MAGIC_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef MAGIC_LIQUID // DO NOT MODIFY!
  bsA = 16388;
  bsB = MAGIC_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined MAGIC_LIQUID with value 16388 from .dat file mismatches hardcoded c++ define value of " << MAGIC_LIQUID << "!" << std::endl;
#endif 

  
#ifdef MAGIC_MUSHROOM_FLESH // DO NOT MODIFY!
  bsA = 20528;
  bsB = MAGIC_MUSHROOM_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MAGIC_MUSHROOM_FLESH with value 20528 from .dat file mismatches hardcoded c++ define value of " << MAGIC_MUSHROOM_FLESH << "!" << std::endl;
#endif 

  
#ifdef MAGIC_VAPOUR // DO NOT MODIFY!
  bsA = 12295;
  bsB = MAGIC_VAPOUR;
  if(bsA!=bsB)
    ssErrors << "Defined MAGIC_VAPOUR with value 12295 from .dat file mismatches hardcoded c++ define value of " << MAGIC_VAPOUR << "!" << std::endl;
#endif 

  
#ifdef MAGPIE_FLESH // DO NOT MODIFY!
  bsA = 20522;
  bsB = MAGPIE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MAGPIE_FLESH with value 20522 from .dat file mismatches hardcoded c++ define value of " << MAGPIE_FLESH << "!" << std::endl;
#endif 

  
#ifdef MAHOGANY_WOOD // DO NOT MODIFY!
  bsA = 4236;
  bsB = MAHOGANY_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined MAHOGANY_WOOD with value 4236 from .dat file mismatches hardcoded c++ define value of " << MAHOGANY_WOOD << "!" << std::endl;
#endif 

  
#ifdef MALACHITE // DO NOT MODIFY!
  bsA = 4301;
  bsB = MALACHITE;
  if(bsA!=bsB)
    ssErrors << "Defined MALACHITE with value 4301 from .dat file mismatches hardcoded c++ define value of " << MALACHITE << "!" << std::endl;
#endif 

  
#ifdef MALE // DO NOT MODIFY!
  bsA = 1;
  bsB = MALE;
  if(bsA!=bsB)
    ssErrors << "Defined MALE with value 1 from .dat file mismatches hardcoded c++ define value of " << MALE << "!" << std::endl;
#endif 

  
#ifdef MAMMOTH_FLESH // DO NOT MODIFY!
  bsA = 20509;
  bsB = MAMMOTH_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MAMMOTH_FLESH with value 20509 from .dat file mismatches hardcoded c++ define value of " << MAMMOTH_FLESH << "!" << std::endl;
#endif 

  
#ifdef MAMMOTH_TUSK // DO NOT MODIFY!
  bsA = 8218;
  bsB = MAMMOTH_TUSK;
  if(bsA!=bsB)
    ssErrors << "Defined MAMMOTH_TUSK with value 8218 from .dat file mismatches hardcoded c++ define value of " << MAMMOTH_TUSK << "!" << std::endl;
#endif 

  
#ifdef MANA // DO NOT MODIFY!
  bsA = 6;
  bsB = MANA;
  if(bsA!=bsB)
    ssErrors << "Defined MANA with value 6 from .dat file mismatches hardcoded c++ define value of " << MANA << "!" << std::endl;
#endif 

  
#ifdef MANGO_FLESH // DO NOT MODIFY!
  bsA = 8201;
  bsB = MANGO_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MANGO_FLESH with value 8201 from .dat file mismatches hardcoded c++ define value of " << MANGO_FLESH << "!" << std::endl;
#endif 

  
#ifdef MARBLE // DO NOT MODIFY!
  bsA = 4263;
  bsB = MARBLE;
  if(bsA!=bsB)
    ssErrors << "Defined MARBLE with value 4263 from .dat file mismatches hardcoded c++ define value of " << MARBLE << "!" << std::endl;
#endif 

  
#ifdef MARSHAL // DO NOT MODIFY!
  bsA = 5;
  bsB = MARSHAL;
  if(bsA!=bsB)
    ssErrors << "Defined MARSHAL with value 5 from .dat file mismatches hardcoded c++ define value of " << MARSHAL << "!" << std::endl;
#endif 

  
#ifdef MARTIAL_SKILL_CATEGORIES // DO NOT MODIFY!
  bsA = 3;
  bsB = MARTIAL_SKILL_CATEGORIES;
  if(bsA!=bsB)
    ssErrors << "Defined MARTIAL_SKILL_CATEGORIES with value 3 from .dat file mismatches hardcoded c++ define value of " << MARTIAL_SKILL_CATEGORIES << "!" << std::endl;
#endif 

  
#ifdef MASK // DO NOT MODIFY!
  bsA = 12;
  bsB = MASK;
  if(bsA!=bsB)
    ssErrors << "Defined MASK with value 12 from .dat file mismatches hardcoded c++ define value of " << MASK << "!" << std::endl;
#endif 

  
#ifdef MASTER // DO NOT MODIFY!
  bsA = 7;
  bsB = MASTER;
  if(bsA!=bsB)
    ssErrors << "Defined MASTER with value 7 from .dat file mismatches hardcoded c++ define value of " << MASTER << "!" << std::endl;
#endif 

  
#ifdef MASTER_ASSASSIN // DO NOT MODIFY!
  bsA = 5;
  bsB = MASTER_ASSASSIN;
  if(bsA!=bsB)
    ssErrors << "Defined MASTER_ASSASSIN with value 5 from .dat file mismatches hardcoded c++ define value of " << MASTER_ASSASSIN << "!" << std::endl;
#endif 

  
#ifdef MASTER_NECROMANCER // DO NOT MODIFY!
  bsA = 2;
  bsB = MASTER_NECROMANCER;
  if(bsA!=bsB)
    ssErrors << "Defined MASTER_NECROMANCER with value 2 from .dat file mismatches hardcoded c++ define value of " << MASTER_NECROMANCER << "!" << std::endl;
#endif 

  
#ifdef MASTER_TORTURER // DO NOT MODIFY!
  bsA = 2;
  bsB = MASTER_TORTURER;
  if(bsA!=bsB)
    ssErrors << "Defined MASTER_TORTURER with value 2 from .dat file mismatches hardcoded c++ define value of " << MASTER_TORTURER << "!" << std::endl;
#endif 

  
#ifdef MAX_PRICE // DO NOT MODIFY!
  bsA = 2147483647;
  bsB = MAX_PRICE;
  if(bsA!=bsB)
    ssErrors << "Defined MAX_PRICE with value 2147483647 from .dat file mismatches hardcoded c++ define value of " << MAX_PRICE << "!" << std::endl;
#endif 

  
#ifdef MAZE_ROOM // DO NOT MODIFY!
  bsA = 3;
  bsB = MAZE_ROOM;
  if(bsA!=bsB)
    ssErrors << "Defined MAZE_ROOM with value 3 from .dat file mismatches hardcoded c++ define value of " << MAZE_ROOM << "!" << std::endl;
#endif 

  
#ifdef MEAT_CLEAVER // DO NOT MODIFY!
  bsA = 22;
  bsB = MEAT_CLEAVER;
  if(bsA!=bsB)
    ssErrors << "Defined MEAT_CLEAVER with value 22 from .dat file mismatches hardcoded c++ define value of " << MEAT_CLEAVER << "!" << std::endl;
#endif 

  
#ifdef MELLIS // DO NOT MODIFY!
  bsA = 9;
  bsB = MELLIS;
  if(bsA!=bsB)
    ssErrors << "Defined MELLIS with value 9 from .dat file mismatches hardcoded c++ define value of " << MELLIS << "!" << std::endl;
#endif 

  
#ifdef MERCURY // DO NOT MODIFY!
  bsA = 16435;
  bsB = MERCURY;
  if(bsA!=bsB)
    ssErrors << "Defined MERCURY with value 16435 from .dat file mismatches hardcoded c++ define value of " << MERCURY << "!" << std::endl;
#endif 

  
#ifdef MERMAID_HAIR // DO NOT MODIFY!
  bsA = 4168;
  bsB = MERMAID_HAIR;
  if(bsA!=bsB)
    ssErrors << "Defined MERMAID_HAIR with value 4168 from .dat file mismatches hardcoded c++ define value of " << MERMAID_HAIR << "!" << std::endl;
#endif 

  
#ifdef METEORIC_STEEL // DO NOT MODIFY!
  bsA = 28677;
  bsB = METEORIC_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined METEORIC_STEEL with value 28677 from .dat file mismatches hardcoded c++ define value of " << METEORIC_STEEL << "!" << std::endl;
#endif 

  
#ifdef MILK // DO NOT MODIFY!
  bsA = 16393;
  bsB = MILK;
  if(bsA!=bsB)
    ssErrors << "Defined MILK with value 16393 from .dat file mismatches hardcoded c++ define value of " << MILK << "!" << std::endl;
#endif 

  
#ifdef MILKY_QUARTZ // DO NOT MODIFY!
  bsA = 4289;
  bsB = MILKY_QUARTZ;
  if(bsA!=bsB)
    ssErrors << "Defined MILKY_QUARTZ with value 4289 from .dat file mismatches hardcoded c++ define value of " << MILKY_QUARTZ << "!" << std::endl;
#endif 

  
#ifdef MIND_WORM_FLESH // DO NOT MODIFY!
  bsA = 20530;
  bsB = MIND_WORM_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MIND_WORM_FLESH with value 20530 from .dat file mismatches hardcoded c++ define value of " << MIND_WORM_FLESH << "!" << std::endl;
#endif 

  
#ifdef MIRROR // DO NOT MODIFY!
  bsA = 1;
  bsB = MIRROR;
  if(bsA!=bsB)
    ssErrors << "Defined MIRROR with value 1 from .dat file mismatches hardcoded c++ define value of " << MIRROR << "!" << std::endl;
#endif 

  
#ifdef MISC // DO NOT MODIFY!
  bsA = 131072;
  bsB = MISC;
  if(bsA!=bsB)
    ssErrors << "Defined MISC with value 131072 from .dat file mismatches hardcoded c++ define value of " << MISC << "!" << std::endl;
#endif 

  
#ifdef MITHRIL // DO NOT MODIFY!
  bsA = 4110;
  bsB = MITHRIL;
  if(bsA!=bsB)
    ssErrors << "Defined MITHRIL with value 4110 from .dat file mismatches hardcoded c++ define value of " << MITHRIL << "!" << std::endl;
#endif 

  
#ifdef MODERATELY_BURNT // DO NOT MODIFY!
  bsA = 2;
  bsB = MODERATELY_BURNT;
  if(bsA!=bsB)
    ssErrors << "Defined MODERATELY_BURNT with value 2 from .dat file mismatches hardcoded c++ define value of " << MODERATELY_BURNT << "!" << std::endl;
#endif 

  
#ifdef MONK // DO NOT MODIFY!
  bsA = 5;
  bsB = MONK;
  if(bsA!=bsB)
    ssErrors << "Defined MONK with value 5 from .dat file mismatches hardcoded c++ define value of " << MONK << "!" << std::endl;
#endif 

  
#ifdef MONSTER_TEAM // DO NOT MODIFY!
  bsA = 1;
  bsB = MONSTER_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined MONSTER_TEAM with value 1 from .dat file mismatches hardcoded c++ define value of " << MONSTER_TEAM << "!" << std::endl;
#endif 

  
#ifdef MOON_SILVER // DO NOT MODIFY!
  bsA = 4108;
  bsB = MOON_SILVER;
  if(bsA!=bsB)
    ssErrors << "Defined MOON_SILVER with value 4108 from .dat file mismatches hardcoded c++ define value of " << MOON_SILVER << "!" << std::endl;
#endif 

  
#ifdef MOOSE_ANTLER // DO NOT MODIFY!
  bsA = 4251;
  bsB = MOOSE_ANTLER;
  if(bsA!=bsB)
    ssErrors << "Defined MOOSE_ANTLER with value 4251 from .dat file mismatches hardcoded c++ define value of " << MOOSE_ANTLER << "!" << std::endl;
#endif 

  
#ifdef MOOSE_FLESH // DO NOT MODIFY!
  bsA = 20521;
  bsB = MOOSE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MOOSE_FLESH with value 20521 from .dat file mismatches hardcoded c++ define value of " << MOOSE_FLESH << "!" << std::endl;
#endif 

  
#ifdef MORAINE // DO NOT MODIFY!
  bsA = 4258;
  bsB = MORAINE;
  if(bsA!=bsB)
    ssErrors << "Defined MORAINE with value 4258 from .dat file mismatches hardcoded c++ define value of " << MORAINE << "!" << std::endl;
#endif 

  
#ifdef MORTIFER // DO NOT MODIFY!
  bsA = 15;
  bsB = MORTIFER;
  if(bsA!=bsB)
    ssErrors << "Defined MORTIFER with value 15 from .dat file mismatches hardcoded c++ define value of " << MORTIFER << "!" << std::endl;
#endif 

  
#ifdef MUD // DO NOT MODIFY!
  bsA = 16437;
  bsB = MUD;
  if(bsA!=bsB)
    ssErrors << "Defined MUD with value 16437 from .dat file mismatches hardcoded c++ define value of " << MUD << "!" << std::endl;
#endif 

  
#ifdef MUSHROOM_FLESH // DO NOT MODIFY!
  bsA = 20520;
  bsB = MUSHROOM_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MUSHROOM_FLESH with value 20520 from .dat file mismatches hardcoded c++ define value of " << MUSHROOM_FLESH << "!" << std::endl;
#endif 

  
#ifdef MUSTARD_GAS // DO NOT MODIFY!
  bsA = 12299;
  bsB = MUSTARD_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined MUSTARD_GAS with value 12299 from .dat file mismatches hardcoded c++ define value of " << MUSTARD_GAS << "!" << std::endl;
#endif 

  
#ifdef MUSTARD_GAS_DAMAGE // DO NOT MODIFY!
  bsA = 256;
  bsB = MUSTARD_GAS_DAMAGE;
  if(bsA!=bsB)
    ssErrors << "Defined MUSTARD_GAS_DAMAGE with value 256 from .dat file mismatches hardcoded c++ define value of " << MUSTARD_GAS_DAMAGE << "!" << std::endl;
#endif 

  
#ifdef MUSTARD_GAS_LIQUID // DO NOT MODIFY!
  bsA = 16412;
  bsB = MUSTARD_GAS_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined MUSTARD_GAS_LIQUID with value 16412 from .dat file mismatches hardcoded c++ define value of " << MUSTARD_GAS_LIQUID << "!" << std::endl;
#endif 

  
#ifdef MUTANT_ASS_FLESH // DO NOT MODIFY!
  bsA = 20499;
  bsB = MUTANT_ASS_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MUTANT_ASS_FLESH with value 20499 from .dat file mismatches hardcoded c++ define value of " << MUTANT_ASS_FLESH << "!" << std::endl;
#endif 

  
#ifdef MUTANT_BEAR // DO NOT MODIFY!
  bsA = 5;
  bsB = MUTANT_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined MUTANT_BEAR with value 5 from .dat file mismatches hardcoded c++ define value of " << MUTANT_BEAR << "!" << std::endl;
#endif 

  
#ifdef MUTANT_BUNNY_FLESH // DO NOT MODIFY!
  bsA = 20525;
  bsB = MUTANT_BUNNY_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined MUTANT_BUNNY_FLESH with value 20525 from .dat file mismatches hardcoded c++ define value of " << MUTANT_BUNNY_FLESH << "!" << std::endl;
#endif 

  
#ifdef MUTANT_PLANT_FIBER // DO NOT MODIFY!
  bsA = 8194;
  bsB = MUTANT_PLANT_FIBER;
  if(bsA!=bsB)
    ssErrors << "Defined MUTANT_PLANT_FIBER with value 8194 from .dat file mismatches hardcoded c++ define value of " << MUTANT_PLANT_FIBER << "!" << std::endl;
#endif 

  
#ifdef MYCELIUM // DO NOT MODIFY!
  bsA = 4215;
  bsB = MYCELIUM;
  if(bsA!=bsB)
    ssErrors << "Defined MYCELIUM with value 4215 from .dat file mismatches hardcoded c++ define value of " << MYCELIUM << "!" << std::endl;
#endif 

  
#ifdef MYSTERY_MEAT // DO NOT MODIFY!
  bsA = 20484;
  bsB = MYSTERY_MEAT;
  if(bsA!=bsB)
    ssErrors << "Defined MYSTERY_MEAT with value 20484 from .dat file mismatches hardcoded c++ define value of " << MYSTERY_MEAT << "!" << std::endl;
#endif 

  
#ifdef NACRE // DO NOT MODIFY!
  bsA = 4284;
  bsB = NACRE;
  if(bsA!=bsB)
    ssErrors << "Defined NACRE with value 4284 from .dat file mismatches hardcoded c++ define value of " << NACRE << "!" << std::endl;
#endif 

  
#ifdef NAGA_SKIN // DO NOT MODIFY!
  bsA = 4147;
  bsB = NAGA_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined NAGA_SKIN with value 4147 from .dat file mismatches hardcoded c++ define value of " << NAGA_SKIN << "!" << std::endl;
#endif 

  
#ifdef NANO_FIBER // DO NOT MODIFY!
  bsA = 4183;
  bsB = NANO_FIBER;
  if(bsA!=bsB)
    ssErrors << "Defined NANO_FIBER with value 4183 from .dat file mismatches hardcoded c++ define value of " << NANO_FIBER << "!" << std::endl;
#endif 

  
#ifdef NECRO_CHAMBER_LEVEL // DO NOT MODIFY!
  bsA = 6;
  bsB = NECRO_CHAMBER_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined NECRO_CHAMBER_LEVEL with value 6 from .dat file mismatches hardcoded c++ define value of " << NECRO_CHAMBER_LEVEL << "!" << std::endl;
#endif 

  
#ifdef NEFAS // DO NOT MODIFY!
  bsA = 11;
  bsB = NEFAS;
  if(bsA!=bsB)
    ssErrors << "Defined NEFAS with value 11 from .dat file mismatches hardcoded c++ define value of " << NEFAS << "!" << std::endl;
#endif 

  
#ifdef NEPHRITE // DO NOT MODIFY!
  bsA = 4302;
  bsB = NEPHRITE;
  if(bsA!=bsB)
    ssErrors << "Defined NEPHRITE with value 4302 from .dat file mismatches hardcoded c++ define value of " << NEPHRITE << "!" << std::endl;
#endif 

  
#ifdef NETHER_QUARTZ // DO NOT MODIFY!
  bsA = 4292;
  bsB = NETHER_QUARTZ;
  if(bsA!=bsB)
    ssErrors << "Defined NETHER_QUARTZ with value 4292 from .dat file mismatches hardcoded c++ define value of " << NETHER_QUARTZ << "!" << std::endl;
#endif 

  
#ifdef NEUTRAL // DO NOT MODIFY!
  bsA = 2;
  bsB = NEUTRAL;
  if(bsA!=bsB)
    ssErrors << "Defined NEUTRAL with value 2 from .dat file mismatches hardcoded c++ define value of " << NEUTRAL << "!" << std::endl;
#endif 

  
#ifdef NEUTRONIUM // DO NOT MODIFY!
  bsA = 4200;
  bsB = NEUTRONIUM;
  if(bsA!=bsB)
    ssErrors << "Defined NEUTRONIUM with value 4200 from .dat file mismatches hardcoded c++ define value of " << NEUTRONIUM << "!" << std::endl;
#endif 

  
#ifdef NEW_ATTNAM // DO NOT MODIFY!
  bsA = 3;
  bsB = NEW_ATTNAM;
  if(bsA!=bsB)
    ssErrors << "Defined NEW_ATTNAM with value 3 from .dat file mismatches hardcoded c++ define value of " << NEW_ATTNAM << "!" << std::endl;
#endif 

  
#ifdef NEW_ATTNAM_TEAM // DO NOT MODIFY!
  bsA = 7;
  bsB = NEW_ATTNAM_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined NEW_ATTNAM_TEAM with value 7 from .dat file mismatches hardcoded c++ define value of " << NEW_ATTNAM_TEAM << "!" << std::endl;
#endif 

  
#ifdef NICKEL // DO NOT MODIFY!
  bsA = 4121;
  bsB = NICKEL;
  if(bsA!=bsB)
    ssErrors << "Defined NICKEL with value 4121 from .dat file mismatches hardcoded c++ define value of " << NICKEL << "!" << std::endl;
#endif 

  
#ifdef NONE // DO NOT MODIFY!
  bsA = 0;
  bsB = NONE;
  if(bsA!=bsB)
    ssErrors << "Defined NONE with value 0 from .dat file mismatches hardcoded c++ define value of " << NONE << "!" << std::endl;
#endif 

  
#ifdef NOT_BURNT // DO NOT MODIFY!
  bsA = 0;
  bsB = NOT_BURNT;
  if(bsA!=bsB)
    ssErrors << "Defined NOT_BURNT with value 0 from .dat file mismatches hardcoded c++ define value of " << NOT_BURNT << "!" << std::endl;
#endif 

  
#ifdef NOT_IN_ROOM // DO NOT MODIFY!
  bsA = 8;
  bsB = NOT_IN_ROOM;
  if(bsA!=bsB)
    ssErrors << "Defined NOT_IN_ROOM with value 8 from .dat file mismatches hardcoded c++ define value of " << NOT_IN_ROOM << "!" << std::endl;
#endif 

  
#ifdef NOT_RUSTED // DO NOT MODIFY!
  bsA = 0;
  bsB = NOT_RUSTED;
  if(bsA!=bsB)
    ssErrors << "Defined NOT_RUSTED with value 0 from .dat file mismatches hardcoded c++ define value of " << NOT_RUSTED << "!" << std::endl;
#endif 

  
#ifdef NOT_WALKABLE // DO NOT MODIFY!
  bsA = 1;
  bsB = NOT_WALKABLE;
  if(bsA!=bsB)
    ssErrors << "Defined NOT_WALKABLE with value 1 from .dat file mismatches hardcoded c++ define value of " << NOT_WALKABLE << "!" << std::endl;
#endif 

  
#ifdef NO_ARTICLE // DO NOT MODIFY!
  bsA = 4;
  bsB = NO_ARTICLE;
  if(bsA!=bsB)
    ssErrors << "Defined NO_ARTICLE with value 4 from .dat file mismatches hardcoded c++ define value of " << NO_ARTICLE << "!" << std::endl;
#endif 

  
#ifdef NO_BROKEN // DO NOT MODIFY!
  bsA = 1;
  bsB = NO_BROKEN;
  if(bsA!=bsB)
    ssErrors << "Defined NO_BROKEN with value 1 from .dat file mismatches hardcoded c++ define value of " << NO_BROKEN << "!" << std::endl;
#endif 

  
#ifdef NO_LIMIT // DO NOT MODIFY!
  bsA = 65535;
  bsB = NO_LIMIT;
  if(bsA!=bsB)
    ssErrors << "Defined NO_LIMIT with value 65535 from .dat file mismatches hardcoded c++ define value of " << NO_LIMIT << "!" << std::endl;
#endif 

  
#ifdef NO_MONSTER_GENERATION // DO NOT MODIFY!
  bsA = 1;
  bsB = NO_MONSTER_GENERATION;
  if(bsA!=bsB)
    ssErrors << "Defined NO_MONSTER_GENERATION with value 1 from .dat file mismatches hardcoded c++ define value of " << NO_MONSTER_GENERATION << "!" << std::endl;
#endif 

  
#ifdef NO_MOVE // DO NOT MODIFY!
  bsA = 0;
  bsB = NO_MOVE;
  if(bsA!=bsB)
    ssErrors << "Defined NO_MOVE with value 0 from .dat file mismatches hardcoded c++ define value of " << NO_MOVE << "!" << std::endl;
#endif 

  
#ifdef NO_PARAMETERS // DO NOT MODIFY!
  bsA = 255;
  bsB = NO_PARAMETERS;
  if(bsA!=bsB)
    ssErrors << "Defined NO_PARAMETERS with value 255 from .dat file mismatches hardcoded c++ define value of " << NO_PARAMETERS << "!" << std::endl;
#endif 

  
#ifdef NO_TAMING // DO NOT MODIFY!
  bsA = -1;
  bsB = NO_TAMING;
  if(bsA!=bsB)
    ssErrors << "Defined NO_TAMING with value -1 from .dat file mismatches hardcoded c++ define value of " << NO_TAMING << "!" << std::endl;
#endif 

  
#ifdef NYLON // DO NOT MODIFY!
  bsA = 4176;
  bsB = NYLON;
  if(bsA!=bsB)
    ssErrors << "Defined NYLON with value 4176 from .dat file mismatches hardcoded c++ define value of " << NYLON << "!" << std::endl;
#endif 

  
#ifdef NYMPH_HAIR // DO NOT MODIFY!
  bsA = 4134;
  bsB = NYMPH_HAIR;
  if(bsA!=bsB)
    ssErrors << "Defined NYMPH_HAIR with value 4134 from .dat file mismatches hardcoded c++ define value of " << NYMPH_HAIR << "!" << std::endl;
#endif 

  
#ifdef N_LOCK_ID // DO NOT MODIFY!
  bsA = 1024;
  bsB = N_LOCK_ID;
  if(bsA!=bsB)
    ssErrors << "Defined N_LOCK_ID with value 1024 from .dat file mismatches hardcoded c++ define value of " << N_LOCK_ID << "!" << std::endl;
#endif 

  
#ifdef OAK // DO NOT MODIFY!
  bsA = 21;
  bsB = OAK;
  if(bsA!=bsB)
    ssErrors << "Defined OAK with value 21 from .dat file mismatches hardcoded c++ define value of " << OAK << "!" << std::endl;
#endif 

  
#ifdef OAK_WOOD // DO NOT MODIFY!
  bsA = 4241;
  bsB = OAK_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined OAK_WOOD with value 4241 from .dat file mismatches hardcoded c++ define value of " << OAK_WOOD << "!" << std::endl;
#endif 

  
#ifdef OBSIDIAN // DO NOT MODIFY!
  bsA = 4262;
  bsB = OBSIDIAN;
  if(bsA!=bsB)
    ssErrors << "Defined OBSIDIAN with value 4262 from .dat file mismatches hardcoded c++ define value of " << OBSIDIAN << "!" << std::endl;
#endif 

  
#ifdef OCCULTUM // DO NOT MODIFY!
  bsA = 4203;
  bsB = OCCULTUM;
  if(bsA!=bsB)
    ssErrors << "Defined OCCULTUM with value 4203 from .dat file mismatches hardcoded c++ define value of " << OCCULTUM << "!" << std::endl;
#endif 

  
#ifdef OCTAGONAL_LOCK // DO NOT MODIFY!
  bsA = 18432;
  bsB = OCTAGONAL_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined OCTAGONAL_LOCK with value 18432 from .dat file mismatches hardcoded c++ define value of " << OCTAGONAL_LOCK << "!" << std::endl;
#endif 

  
#ifdef OCTIRON // DO NOT MODIFY!
  bsA = 4105;
  bsB = OCTIRON;
  if(bsA!=bsB)
    ssErrors << "Defined OCTIRON with value 4105 from .dat file mismatches hardcoded c++ define value of " << OCTIRON << "!" << std::endl;
#endif 

  
#ifdef OFFICER // DO NOT MODIFY!
  bsA = 3;
  bsB = OFFICER;
  if(bsA!=bsB)
    ssErrors << "Defined OFFICER with value 3 from .dat file mismatches hardcoded c++ define value of " << OFFICER << "!" << std::endl;
#endif 

  
#ifdef OMMEL_BLOOD // DO NOT MODIFY!
  bsA = 16430;
  bsB = OMMEL_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_BLOOD with value 16430 from .dat file mismatches hardcoded c++ define value of " << OMMEL_BLOOD << "!" << std::endl;
#endif 

  
#ifdef OMMEL_BONE // DO NOT MODIFY!
  bsA = 8215;
  bsB = OMMEL_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_BONE with value 8215 from .dat file mismatches hardcoded c++ define value of " << OMMEL_BONE << "!" << std::endl;
#endif 

  
#ifdef OMMEL_CERUMEN // DO NOT MODIFY!
  bsA = 8211;
  bsB = OMMEL_CERUMEN;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_CERUMEN with value 8211 from .dat file mismatches hardcoded c++ define value of " << OMMEL_CERUMEN << "!" << std::endl;
#endif 

  
#ifdef OMMEL_CUIRASS // DO NOT MODIFY!
  bsA = 4;
  bsB = OMMEL_CUIRASS;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_CUIRASS with value 4 from .dat file mismatches hardcoded c++ define value of " << OMMEL_CUIRASS << "!" << std::endl;
#endif 

  
#ifdef OMMEL_HAIR // DO NOT MODIFY!
  bsA = 4135;
  bsB = OMMEL_HAIR;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_HAIR with value 4135 from .dat file mismatches hardcoded c++ define value of " << OMMEL_HAIR << "!" << std::endl;
#endif 

  
#ifdef OMMEL_SNOT // DO NOT MODIFY!
  bsA = 16434;
  bsB = OMMEL_SNOT;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_SNOT with value 16434 from .dat file mismatches hardcoded c++ define value of " << OMMEL_SNOT << "!" << std::endl;
#endif 

  
#ifdef OMMEL_SWEAT // DO NOT MODIFY!
  bsA = 16432;
  bsB = OMMEL_SWEAT;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_SWEAT with value 16432 from .dat file mismatches hardcoded c++ define value of " << OMMEL_SWEAT << "!" << std::endl;
#endif 

  
#ifdef OMMEL_TEARS // DO NOT MODIFY!
  bsA = 16433;
  bsB = OMMEL_TEARS;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_TEARS with value 16433 from .dat file mismatches hardcoded c++ define value of " << OMMEL_TEARS << "!" << std::endl;
#endif 

  
#ifdef OMMEL_TOOTH // DO NOT MODIFY!
  bsA = 8219;
  bsB = OMMEL_TOOTH;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_TOOTH with value 8219 from .dat file mismatches hardcoded c++ define value of " << OMMEL_TOOTH << "!" << std::endl;
#endif 

  
#ifdef OMMEL_URINE // DO NOT MODIFY!
  bsA = 16431;
  bsB = OMMEL_URINE;
  if(bsA!=bsB)
    ssErrors << "Defined OMMEL_URINE with value 16431 from .dat file mismatches hardcoded c++ define value of " << OMMEL_URINE << "!" << std::endl;
#endif 

  
#ifdef ONYX // DO NOT MODIFY!
  bsA = 4282;
  bsB = ONYX;
  if(bsA!=bsB)
    ssErrors << "Defined ONYX with value 4282 from .dat file mismatches hardcoded c++ define value of " << ONYX << "!" << std::endl;
#endif 

  
#ifdef OPAL // DO NOT MODIFY!
  bsA = 4281;
  bsB = OPAL;
  if(bsA!=bsB)
    ssErrors << "Defined OPAL with value 4281 from .dat file mismatches hardcoded c++ define value of " << OPAL << "!" << std::endl;
#endif 

  
#ifdef ORC_FLESH // DO NOT MODIFY!
  bsA = 20516;
  bsB = ORC_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined ORC_FLESH with value 20516 from .dat file mismatches hardcoded c++ define value of " << ORC_FLESH << "!" << std::endl;
#endif 

  
#ifdef OREE_LAIR // DO NOT MODIFY!
  bsA = 12;
  bsB = OREE_LAIR;
  if(bsA!=bsB)
    ssErrors << "Defined OREE_LAIR with value 12 from .dat file mismatches hardcoded c++ define value of " << OREE_LAIR << "!" << std::endl;
#endif 

  
#ifdef OREE_LAIR_ENTRY // DO NOT MODIFY!
  bsA = 300;
  bsB = OREE_LAIR_ENTRY;
  if(bsA!=bsB)
    ssErrors << "Defined OREE_LAIR_ENTRY with value 300 from .dat file mismatches hardcoded c++ define value of " << OREE_LAIR_ENTRY << "!" << std::endl;
#endif 

  
#ifdef OREE_LAIR_EXIT // DO NOT MODIFY!
  bsA = 400;
  bsB = OREE_LAIR_EXIT;
  if(bsA!=bsB)
    ssErrors << "Defined OREE_LAIR_EXIT with value 400 from .dat file mismatches hardcoded c++ define value of " << OREE_LAIR_EXIT << "!" << std::endl;
#endif 

  
#ifdef ORGANIC_ID // DO NOT MODIFY!
  bsA = 8192;
  bsB = ORGANIC_ID;
  if(bsA!=bsB)
    ssErrors << "Defined ORGANIC_ID with value 8192 from .dat file mismatches hardcoded c++ define value of " << ORGANIC_ID << "!" << std::endl;
#endif 

  
#ifdef ORICHALCUM // DO NOT MODIFY!
  bsA = 4106;
  bsB = ORICHALCUM;
  if(bsA!=bsB)
    ssErrors << "Defined ORICHALCUM with value 4106 from .dat file mismatches hardcoded c++ define value of " << ORICHALCUM << "!" << std::endl;
#endif 

  
#ifdef OSTRICH_FEATHER // DO NOT MODIFY!
  bsA = 4160;
  bsB = OSTRICH_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined OSTRICH_FEATHER with value 4160 from .dat file mismatches hardcoded c++ define value of " << OSTRICH_FEATHER << "!" << std::endl;
#endif 

  
#ifdef OSTRICH_FLESH // DO NOT MODIFY!
  bsA = 20517;
  bsB = OSTRICH_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined OSTRICH_FLESH with value 20517 from .dat file mismatches hardcoded c++ define value of " << OSTRICH_FLESH << "!" << std::endl;
#endif 

  
#ifdef OTHER // DO NOT MODIFY!
  bsA = 128;
  bsB = OTHER;
  if(bsA!=bsB)
    ssErrors << "Defined OTHER with value 128 from .dat file mismatches hardcoded c++ define value of " << OTHER << "!" << std::endl;
#endif 

  
#ifdef OUROBOROS_HIDE // DO NOT MODIFY!
  bsA = 4159;
  bsB = OUROBOROS_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined OUROBOROS_HIDE with value 4159 from .dat file mismatches hardcoded c++ define value of " << OUROBOROS_HIDE << "!" << std::endl;
#endif 

  
#ifdef OVEN // DO NOT MODIFY!
  bsA = 28;
  bsB = OVEN;
  if(bsA!=bsB)
    ssErrors << "Defined OVEN with value 28 from .dat file mismatches hardcoded c++ define value of " << OVEN << "!" << std::endl;
#endif 

  
#ifdef PALLADIUM // DO NOT MODIFY!
  bsA = 4125;
  bsB = PALLADIUM;
  if(bsA!=bsB)
    ssErrors << "Defined PALLADIUM with value 4125 from .dat file mismatches hardcoded c++ define value of " << PALLADIUM << "!" << std::endl;
#endif 

  
#ifdef PALM // DO NOT MODIFY!
  bsA = 15;
  bsB = PALM;
  if(bsA!=bsB)
    ssErrors << "Defined PALM with value 15 from .dat file mismatches hardcoded c++ define value of " << PALM << "!" << std::endl;
#endif 

  
#ifdef PALM_LEAF // DO NOT MODIFY!
  bsA = 4217;
  bsB = PALM_LEAF;
  if(bsA!=bsB)
    ssErrors << "Defined PALM_LEAF with value 4217 from .dat file mismatches hardcoded c++ define value of " << PALM_LEAF << "!" << std::endl;
#endif 

  
#ifdef PANDA_BEAR // DO NOT MODIFY!
  bsA = 6;
  bsB = PANDA_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined PANDA_BEAR with value 6 from .dat file mismatches hardcoded c++ define value of " << PANDA_BEAR << "!" << std::endl;
#endif 

  
#ifdef PANIC // DO NOT MODIFY!
  bsA = 8192;
  bsB = PANIC;
  if(bsA!=bsB)
    ssErrors << "Defined PANIC with value 8192 from .dat file mismatches hardcoded c++ define value of " << PANIC << "!" << std::endl;
#endif 

  
#ifdef PAPER_BOARD // DO NOT MODIFY!
  bsA = 4225;
  bsB = PAPER_BOARD;
  if(bsA!=bsB)
    ssErrors << "Defined PAPER_BOARD with value 4225 from .dat file mismatches hardcoded c++ define value of " << PAPER_BOARD << "!" << std::endl;
#endif 

  
#ifdef PAPYRUS // DO NOT MODIFY!
  bsA = 4222;
  bsB = PAPYRUS;
  if(bsA!=bsB)
    ssErrors << "Defined PAPYRUS with value 4222 from .dat file mismatches hardcoded c++ define value of " << PAPYRUS << "!" << std::endl;
#endif 

  
#ifdef PARASITE_MIND_WORM // DO NOT MODIFY!
  bsA = 2147483648;
  bsB = PARASITE_MIND_WORM;
  if(bsA!=bsB)
    ssErrors << "Defined PARASITE_MIND_WORM with value 2147483648 from .dat file mismatches hardcoded c++ define value of " << PARASITE_MIND_WORM << "!" << std::endl;
#endif 

  
#ifdef PARASITE_TAPE_WORM // DO NOT MODIFY!
  bsA = 32768;
  bsB = PARASITE_TAPE_WORM;
  if(bsA!=bsB)
    ssErrors << "Defined PARASITE_TAPE_WORM with value 32768 from .dat file mismatches hardcoded c++ define value of " << PARASITE_TAPE_WORM << "!" << std::endl;
#endif 

  
#ifdef PARCHMENT // DO NOT MODIFY!
  bsA = 4223;
  bsB = PARCHMENT;
  if(bsA!=bsB)
    ssErrors << "Defined PARCHMENT with value 4223 from .dat file mismatches hardcoded c++ define value of " << PARCHMENT << "!" << std::endl;
#endif 

  
#ifdef PARQUET // DO NOT MODIFY!
  bsA = 1;
  bsB = PARQUET;
  if(bsA!=bsB)
    ssErrors << "Defined PARQUET with value 1 from .dat file mismatches hardcoded c++ define value of " << PARQUET << "!" << std::endl;
#endif 

  
#ifdef PARTICLE_BEAM // DO NOT MODIFY!
  bsA = 0;
  bsB = PARTICLE_BEAM;
  if(bsA!=bsB)
    ssErrors << "Defined PARTICLE_BEAM with value 0 from .dat file mismatches hardcoded c++ define value of " << PARTICLE_BEAM << "!" << std::endl;
#endif 

  
#ifdef PATRIARCH // DO NOT MODIFY!
  bsA = 3;
  bsB = PATRIARCH;
  if(bsA!=bsB)
    ssErrors << "Defined PATRIARCH with value 3 from .dat file mismatches hardcoded c++ define value of " << PATRIARCH << "!" << std::endl;
#endif 

  
#ifdef PATROL // DO NOT MODIFY!
  bsA = 4;
  bsB = PATROL;
  if(bsA!=bsB)
    ssErrors << "Defined PATROL with value 4 from .dat file mismatches hardcoded c++ define value of " << PATROL << "!" << std::endl;
#endif 

  
#ifdef PEARL // DO NOT MODIFY!
  bsA = 4285;
  bsB = PEARL;
  if(bsA!=bsB)
    ssErrors << "Defined PEARL with value 4285 from .dat file mismatches hardcoded c++ define value of " << PEARL << "!" << std::endl;
#endif 

  
#ifdef PEARL_GLASS // DO NOT MODIFY!
  bsA = 4207;
  bsB = PEARL_GLASS;
  if(bsA!=bsB)
    ssErrors << "Defined PEARL_GLASS with value 4207 from .dat file mismatches hardcoded c++ define value of " << PEARL_GLASS << "!" << std::endl;
#endif 

  
#ifdef PEA_SOUP // DO NOT MODIFY!
  bsA = 16389;
  bsB = PEA_SOUP;
  if(bsA!=bsB)
    ssErrors << "Defined PEA_SOUP with value 16389 from .dat file mismatches hardcoded c++ define value of " << PEA_SOUP << "!" << std::endl;
#endif 

  
#ifdef PEDESTAL // DO NOT MODIFY!
  bsA = 25;
  bsB = PEDESTAL;
  if(bsA!=bsB)
    ssErrors << "Defined PEDESTAL with value 25 from .dat file mismatches hardcoded c++ define value of " << PEDESTAL << "!" << std::endl;
#endif 

  
#ifdef PEPSI // DO NOT MODIFY!
  bsA = 16410;
  bsB = PEPSI;
  if(bsA!=bsB)
    ssErrors << "Defined PEPSI with value 16410 from .dat file mismatches hardcoded c++ define value of " << PEPSI << "!" << std::endl;
#endif 

  
#ifdef PERCEPTION // DO NOT MODIFY!
  bsA = 1;
  bsB = PERCEPTION;
  if(bsA!=bsB)
    ssErrors << "Defined PERCEPTION with value 1 from .dat file mismatches hardcoded c++ define value of " << PERCEPTION << "!" << std::endl;
#endif 

  
#ifdef PETRIFIED_DARK // DO NOT MODIFY!
  bsA = 4118;
  bsB = PETRIFIED_DARK;
  if(bsA!=bsB)
    ssErrors << "Defined PETRIFIED_DARK with value 4118 from .dat file mismatches hardcoded c++ define value of " << PETRIFIED_DARK << "!" << std::endl;
#endif 

  
#ifdef PETRIFIED_WOOD // DO NOT MODIFY!
  bsA = 4247;
  bsB = PETRIFIED_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined PETRIFIED_WOOD with value 4247 from .dat file mismatches hardcoded c++ define value of " << PETRIFIED_WOOD << "!" << std::endl;
#endif 

  
#ifdef PHOENIX_FEATHER // DO NOT MODIFY!
  bsA = 4163;
  bsB = PHOENIX_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined PHOENIX_FEATHER with value 4163 from .dat file mismatches hardcoded c++ define value of " << PHOENIX_FEATHER << "!" << std::endl;
#endif 

  
#ifdef PHOENIX_SHIELD // DO NOT MODIFY!
  bsA = 1;
  bsB = PHOENIX_SHIELD;
  if(bsA!=bsB)
    ssErrors << "Defined PHOENIX_SHIELD with value 1 from .dat file mismatches hardcoded c++ define value of " << PHOENIX_SHIELD << "!" << std::endl;
#endif 

  
#ifdef PHYSICAL_DAMAGE // DO NOT MODIFY!
  bsA = 1;
  bsB = PHYSICAL_DAMAGE;
  if(bsA!=bsB)
    ssErrors << "Defined PHYSICAL_DAMAGE with value 1 from .dat file mismatches hardcoded c++ define value of " << PHYSICAL_DAMAGE << "!" << std::endl;
#endif 

  
#ifdef PIERCE // DO NOT MODIFY!
  bsA = 4;
  bsB = PIERCE;
  if(bsA!=bsB)
    ssErrors << "Defined PIERCE with value 4 from .dat file mismatches hardcoded c++ define value of " << PIERCE << "!" << std::endl;
#endif 

  
#ifdef PIG_IRON // DO NOT MODIFY!
  bsA = 28673;
  bsB = PIG_IRON;
  if(bsA!=bsB)
    ssErrors << "Defined PIG_IRON with value 28673 from .dat file mismatches hardcoded c++ define value of " << PIG_IRON << "!" << std::endl;
#endif 

  
#ifdef PINE // DO NOT MODIFY!
  bsA = 1;
  bsB = PINE;
  if(bsA!=bsB)
    ssErrors << "Defined PINE with value 1 from .dat file mismatches hardcoded c++ define value of " << PINE << "!" << std::endl;
#endif 

  
#ifdef PINEAPPLE_FLESH // DO NOT MODIFY!
  bsA = 8199;
  bsB = PINEAPPLE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined PINEAPPLE_FLESH with value 8199 from .dat file mismatches hardcoded c++ define value of " << PINEAPPLE_FLESH << "!" << std::endl;
#endif 

  
#ifdef PINE_WOOD // DO NOT MODIFY!
  bsA = 4238;
  bsB = PINE_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined PINE_WOOD with value 4238 from .dat file mismatches hardcoded c++ define value of " << PINE_WOOD << "!" << std::endl;
#endif 

  
#ifdef PINK // DO NOT MODIFY!
  bsA = 61470;
  bsB = PINK;
  if(bsA!=bsB)
    ssErrors << "Defined PINK with value 61470 from .dat file mismatches hardcoded c++ define value of " << PINK << "!" << std::endl;
#endif 

  
#ifdef PINK_SIREN // DO NOT MODIFY!
  bsA = 8;
  bsB = PINK_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined PINK_SIREN with value 8 from .dat file mismatches hardcoded c++ define value of " << PINK_SIREN << "!" << std::endl;
#endif 

  
#ifdef PLAIN_BED // DO NOT MODIFY!
  bsA = 11;
  bsB = PLAIN_BED;
  if(bsA!=bsB)
    ssErrors << "Defined PLAIN_BED with value 11 from .dat file mismatches hardcoded c++ define value of " << PLAIN_BED << "!" << std::endl;
#endif 

  
#ifdef PLANKTON // DO NOT MODIFY!
  bsA = 8203;
  bsB = PLANKTON;
  if(bsA!=bsB)
    ssErrors << "Defined PLANKTON with value 8203 from .dat file mismatches hardcoded c++ define value of " << PLANKTON << "!" << std::endl;
#endif 

  
#ifdef PLANT_FIBER // DO NOT MODIFY!
  bsA = 8193;
  bsB = PLANT_FIBER;
  if(bsA!=bsB)
    ssErrors << "Defined PLANT_FIBER with value 8193 from .dat file mismatches hardcoded c++ define value of " << PLANT_FIBER << "!" << std::endl;
#endif 

  
#ifdef PLANT_SAP // DO NOT MODIFY!
  bsA = 16422;
  bsB = PLANT_SAP;
  if(bsA!=bsB)
    ssErrors << "Defined PLANT_SAP with value 16422 from .dat file mismatches hardcoded c++ define value of " << PLANT_SAP << "!" << std::endl;
#endif 

  
#ifdef PLASTIC // DO NOT MODIFY!
  bsA = 4186;
  bsB = PLASTIC;
  if(bsA!=bsB)
    ssErrors << "Defined PLASTIC with value 4186 from .dat file mismatches hardcoded c++ define value of " << PLASTIC << "!" << std::endl;
#endif 

  
#ifdef PLASTIC_STEEL // DO NOT MODIFY!
  bsA = 4189;
  bsB = PLASTIC_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined PLASTIC_STEEL with value 4189 from .dat file mismatches hardcoded c++ define value of " << PLASTIC_STEEL << "!" << std::endl;
#endif 

  
#ifdef PLATE_MAIL // DO NOT MODIFY!
  bsA = 2;
  bsB = PLATE_MAIL;
  if(bsA!=bsB)
    ssErrors << "Defined PLATE_MAIL with value 2 from .dat file mismatches hardcoded c++ define value of " << PLATE_MAIL << "!" << std::endl;
#endif 

  
#ifdef PLATINUM // DO NOT MODIFY!
  bsA = 4113;
  bsB = PLATINUM;
  if(bsA!=bsB)
    ssErrors << "Defined PLATINUM with value 4113 from .dat file mismatches hardcoded c++ define value of " << PLATINUM << "!" << std::endl;
#endif 

  
#ifdef PLAYER_TEAM // DO NOT MODIFY!
  bsA = 0;
  bsB = PLAYER_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined PLAYER_TEAM with value 0 from .dat file mismatches hardcoded c++ define value of " << PLAYER_TEAM << "!" << std::endl;
#endif 

  
#ifdef PLENTY // DO NOT MODIFY!
  bsA = 5;
  bsB = PLENTY;
  if(bsA!=bsB)
    ssErrors << "Defined PLENTY with value 5 from .dat file mismatches hardcoded c++ define value of " << PLENTY << "!" << std::endl;
#endif 

  
#ifdef PLURAL // DO NOT MODIFY!
  bsA = 1;
  bsB = PLURAL;
  if(bsA!=bsB)
    ssErrors << "Defined PLURAL with value 1 from .dat file mismatches hardcoded c++ define value of " << PLURAL << "!" << std::endl;
#endif 

  
#ifdef POISON // DO NOT MODIFY!
  bsA = 64;
  bsB = POISON;
  if(bsA!=bsB)
    ssErrors << "Defined POISON with value 64 from .dat file mismatches hardcoded c++ define value of " << POISON << "!" << std::endl;
#endif 

  
#ifdef POISONED // DO NOT MODIFY!
  bsA = 512;
  bsB = POISONED;
  if(bsA!=bsB)
    ssErrors << "Defined POISONED with value 512 from .dat file mismatches hardcoded c++ define value of " << POISONED << "!" << std::endl;
#endif 

  
#ifdef POISON_LIQUID // DO NOT MODIFY!
  bsA = 16409;
  bsB = POISON_LIQUID;
  if(bsA!=bsB)
    ssErrors << "Defined POISON_LIQUID with value 16409 from .dat file mismatches hardcoded c++ define value of " << POISON_LIQUID << "!" << std::endl;
#endif 

  
#ifdef POLAR_BEAR // DO NOT MODIFY!
  bsA = 4;
  bsB = POLAR_BEAR;
  if(bsA!=bsB)
    ssErrors << "Defined POLAR_BEAR with value 4 from .dat file mismatches hardcoded c++ define value of " << POLAR_BEAR << "!" << std::endl;
#endif 

  
#ifdef POLE_ARMS // DO NOT MODIFY!
  bsA = 8;
  bsB = POLE_ARMS;
  if(bsA!=bsB)
    ssErrors << "Defined POLE_ARMS with value 8 from .dat file mismatches hardcoded c++ define value of " << POLE_ARMS << "!" << std::endl;
#endif 

  
#ifdef POLYMORPH // DO NOT MODIFY!
  bsA = 2048;
  bsB = POLYMORPH;
  if(bsA!=bsB)
    ssErrors << "Defined POLYMORPH with value 2048 from .dat file mismatches hardcoded c++ define value of " << POLYMORPH << "!" << std::endl;
#endif 

  
#ifdef POLYMORPHED // DO NOT MODIFY!
  bsA = 1;
  bsB = POLYMORPHED;
  if(bsA!=bsB)
    ssErrors << "Defined POLYMORPHED with value 1 from .dat file mismatches hardcoded c++ define value of " << POLYMORPHED << "!" << std::endl;
#endif 

  
#ifdef POLYMORPH_CONTROL // DO NOT MODIFY!
  bsA = 8;
  bsB = POLYMORPH_CONTROL;
  if(bsA!=bsB)
    ssErrors << "Defined POLYMORPH_CONTROL with value 8 from .dat file mismatches hardcoded c++ define value of " << POLYMORPH_CONTROL << "!" << std::endl;
#endif 

  
#ifdef POLYMORPH_LOCK // DO NOT MODIFY!
  bsA = 33554432;
  bsB = POLYMORPH_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined POLYMORPH_LOCK with value 33554432 from .dat file mismatches hardcoded c++ define value of " << POLYMORPH_LOCK << "!" << std::endl;
#endif 

  
#ifdef POOL // DO NOT MODIFY!
  bsA = 1;
  bsB = POOL;
  if(bsA!=bsB)
    ssErrors << "Defined POOL with value 1 from .dat file mismatches hardcoded c++ define value of " << POOL << "!" << std::endl;
#endif 

  
#ifdef POOL_BORDER // DO NOT MODIFY!
  bsA = 13;
  bsB = POOL_BORDER;
  if(bsA!=bsB)
    ssErrors << "Defined POOL_BORDER with value 13 from .dat file mismatches hardcoded c++ define value of " << POOL_BORDER << "!" << std::endl;
#endif 

  
#ifdef POOL_CORNER // DO NOT MODIFY!
  bsA = 14;
  bsB = POOL_CORNER;
  if(bsA!=bsB)
    ssErrors << "Defined POOL_CORNER with value 14 from .dat file mismatches hardcoded c++ define value of " << POOL_CORNER << "!" << std::endl;
#endif 

  
#ifdef PORCELAIN // DO NOT MODIFY!
  bsA = 4320;
  bsB = PORCELAIN;
  if(bsA!=bsB)
    ssErrors << "Defined PORCELAIN with value 4320 from .dat file mismatches hardcoded c++ define value of " << PORCELAIN << "!" << std::endl;
#endif 

  
#ifdef PORK // DO NOT MODIFY!
  bsA = 20481;
  bsB = PORK;
  if(bsA!=bsB)
    ssErrors << "Defined PORK with value 20481 from .dat file mismatches hardcoded c++ define value of " << PORK << "!" << std::endl;
#endif 

  
#ifdef PORRIDGE // DO NOT MODIFY!
  bsA = 16391;
  bsB = PORRIDGE;
  if(bsA!=bsB)
    ssErrors << "Defined PORRIDGE with value 16391 from .dat file mismatches hardcoded c++ define value of " << PORRIDGE << "!" << std::endl;
#endif 

  
#ifdef POTION // DO NOT MODIFY!
  bsA = 2048;
  bsB = POTION;
  if(bsA!=bsB)
    ssErrors << "Defined POTION with value 2048 from .dat file mismatches hardcoded c++ define value of " << POTION << "!" << std::endl;
#endif 

  
#ifdef POTTED_CACTUS // DO NOT MODIFY!
  bsA = 1;
  bsB = POTTED_CACTUS;
  if(bsA!=bsB)
    ssErrors << "Defined POTTED_CACTUS with value 1 from .dat file mismatches hardcoded c++ define value of " << POTTED_CACTUS << "!" << std::endl;
#endif 

  
#ifdef POTTED_PLANT // DO NOT MODIFY!
  bsA = 2;
  bsB = POTTED_PLANT;
  if(bsA!=bsB)
    ssErrors << "Defined POTTED_PLANT with value 2 from .dat file mismatches hardcoded c++ define value of " << POTTED_PLANT << "!" << std::endl;
#endif 

  
#ifdef POWDER_ID // DO NOT MODIFY!
  bsA = 24576;
  bsB = POWDER_ID;
  if(bsA!=bsB)
    ssErrors << "Defined POWDER_ID with value 24576 from .dat file mismatches hardcoded c++ define value of " << POWDER_ID << "!" << std::endl;
#endif 

  
#ifdef PRIMORDIAL_ICE // DO NOT MODIFY!
  bsA = 4212;
  bsB = PRIMORDIAL_ICE;
  if(bsA!=bsB)
    ssErrors << "Defined PRIMORDIAL_ICE with value 4212 from .dat file mismatches hardcoded c++ define value of " << PRIMORDIAL_ICE << "!" << std::endl;
#endif 

  
#ifdef PRINCE // DO NOT MODIFY!
  bsA = 3;
  bsB = PRINCE;
  if(bsA!=bsB)
    ssErrors << "Defined PRINCE with value 3 from .dat file mismatches hardcoded c++ define value of " << PRINCE << "!" << std::endl;
#endif 

  
#ifdef PRISONER_TEAM // DO NOT MODIFY!
  bsA = 15;
  bsB = PRISONER_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined PRISONER_TEAM with value 15 from .dat file mismatches hardcoded c++ define value of " << PRISONER_TEAM << "!" << std::endl;
#endif 

  
#ifdef PSYPHER // DO NOT MODIFY!
  bsA = 4327;
  bsB = PSYPHER;
  if(bsA!=bsB)
    ssErrors << "Defined PSYPHER with value 4327 from .dat file mismatches hardcoded c++ define value of " << PSYPHER << "!" << std::endl;
#endif 

  
#ifdef PUPPY_SKULL // DO NOT MODIFY!
  bsA = 1;
  bsB = PUPPY_SKULL;
  if(bsA!=bsB)
    ssErrors << "Defined PUPPY_SKULL with value 1 from .dat file mismatches hardcoded c++ define value of " << PUPPY_SKULL << "!" << std::endl;
#endif 

  
#ifdef PURPLE_CRYSTAL // DO NOT MODIFY!
  bsA = 4309;
  bsB = PURPLE_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined PURPLE_CRYSTAL with value 4309 from .dat file mismatches hardcoded c++ define value of " << PURPLE_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef PYRITE // DO NOT MODIFY!
  bsA = 4325;
  bsB = PYRITE;
  if(bsA!=bsB)
    ssErrors << "Defined PYRITE with value 4325 from .dat file mismatches hardcoded c++ define value of " << PYRITE << "!" << std::endl;
#endif 

  
#ifdef QUARTER_STAFF // DO NOT MODIFY!
  bsA = 15;
  bsB = QUARTER_STAFF;
  if(bsA!=bsB)
    ssErrors << "Defined QUARTER_STAFF with value 15 from .dat file mismatches hardcoded c++ define value of " << QUARTER_STAFF << "!" << std::endl;
#endif 

  
#ifdef QUARTZITE // DO NOT MODIFY!
  bsA = 4290;
  bsB = QUARTZITE;
  if(bsA!=bsB)
    ssErrors << "Defined QUARTZITE with value 4290 from .dat file mismatches hardcoded c++ define value of " << QUARTZITE << "!" << std::endl;
#endif 

  
#ifdef QUEEN // DO NOT MODIFY!
  bsA = 4;
  bsB = QUEEN;
  if(bsA!=bsB)
    ssErrors << "Defined QUEEN with value 4 from .dat file mismatches hardcoded c++ define value of " << QUEEN << "!" << std::endl;
#endif 

  
#ifdef QUICK_SAND // DO NOT MODIFY!
  bsA = 16438;
  bsB = QUICK_SAND;
  if(bsA!=bsB)
    ssErrors << "Defined QUICK_SAND with value 16438 from .dat file mismatches hardcoded c++ define value of " << QUICK_SAND << "!" << std::endl;
#endif 

  
#ifdef QUICK_SILVER // DO NOT MODIFY!
  bsA = 16436;
  bsB = QUICK_SILVER;
  if(bsA!=bsB)
    ssErrors << "Defined QUICK_SILVER with value 16436 from .dat file mismatches hardcoded c++ define value of " << QUICK_SILVER << "!" << std::endl;
#endif 

  
#ifdef QUILTED_LEATHER // DO NOT MODIFY!
  bsA = 4154;
  bsB = QUILTED_LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined QUILTED_LEATHER with value 4154 from .dat file mismatches hardcoded c++ define value of " << QUILTED_LEATHER << "!" << std::endl;
#endif 

  
#ifdef RAINBOW_CLOTH // DO NOT MODIFY!
  bsA = 4174;
  bsB = RAINBOW_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined RAINBOW_CLOTH with value 4174 from .dat file mismatches hardcoded c++ define value of " << RAINBOW_CLOTH << "!" << std::endl;
#endif 

  
#ifdef RANDOM // DO NOT MODIFY!
  bsA = 0;
  bsB = RANDOM;
  if(bsA!=bsB)
    ssErrors << "Defined RANDOM with value 0 from .dat file mismatches hardcoded c++ define value of " << RANDOM << "!" << std::endl;
#endif 

  
#ifdef RANDOM_COLOR // DO NOT MODIFY!
  bsA = 65536;
  bsB = RANDOM_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined RANDOM_COLOR with value 65536 from .dat file mismatches hardcoded c++ define value of " << RANDOM_COLOR << "!" << std::endl;
#endif 

  
#ifdef RATA_WOOD // DO NOT MODIFY!
  bsA = 4245;
  bsB = RATA_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined RATA_WOOD with value 4245 from .dat file mismatches hardcoded c++ define value of " << RATA_WOOD << "!" << std::endl;
#endif 

  
#ifdef RAT_FLESH // DO NOT MODIFY!
  bsA = 20505;
  bsB = RAT_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined RAT_FLESH with value 20505 from .dat file mismatches hardcoded c++ define value of " << RAT_FLESH << "!" << std::endl;
#endif 

  
#ifdef RECTANGLE // DO NOT MODIFY!
  bsA = 1;
  bsB = RECTANGLE;
  if(bsA!=bsB)
    ssErrors << "Defined RECTANGLE with value 1 from .dat file mismatches hardcoded c++ define value of " << RECTANGLE << "!" << std::endl;
#endif 

  
#ifdef RED // DO NOT MODIFY!
  bsA = 63488;
  bsB = RED;
  if(bsA!=bsB)
    ssErrors << "Defined RED with value 63488 from .dat file mismatches hardcoded c++ define value of " << RED << "!" << std::endl;
#endif 

  
#ifdef RED_FRACTAL // DO NOT MODIFY!
  bsA = 1;
  bsB = RED_FRACTAL;
  if(bsA!=bsB)
    ssErrors << "Defined RED_FRACTAL with value 1 from .dat file mismatches hardcoded c++ define value of " << RED_FRACTAL << "!" << std::endl;
#endif 

  
#ifdef RED_ICE // DO NOT MODIFY!
  bsA = 4210;
  bsB = RED_ICE;
  if(bsA!=bsB)
    ssErrors << "Defined RED_ICE with value 4210 from .dat file mismatches hardcoded c++ define value of " << RED_ICE << "!" << std::endl;
#endif 

  
#ifdef RED_JADE // DO NOT MODIFY!
  bsA = 4304;
  bsB = RED_JADE;
  if(bsA!=bsB)
    ssErrors << "Defined RED_JADE with value 4304 from .dat file mismatches hardcoded c++ define value of " << RED_JADE << "!" << std::endl;
#endif 

  
#ifdef RED_SAND // DO NOT MODIFY!
  bsA = 24581;
  bsB = RED_SAND;
  if(bsA!=bsB)
    ssErrors << "Defined RED_SAND with value 24581 from .dat file mismatches hardcoded c++ define value of " << RED_SAND << "!" << std::endl;
#endif 

  
#ifdef RED_SAND_STONE // DO NOT MODIFY!
  bsA = 4205;
  bsB = RED_SAND_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined RED_SAND_STONE with value 4205 from .dat file mismatches hardcoded c++ define value of " << RED_SAND_STONE << "!" << std::endl;
#endif 

  
#ifdef RED_SIREN // DO NOT MODIFY!
  bsA = 7;
  bsB = RED_SIREN;
  if(bsA!=bsB)
    ssErrors << "Defined RED_SIREN with value 7 from .dat file mismatches hardcoded c++ define value of " << RED_SIREN << "!" << std::endl;
#endif 

  
#ifdef RED_WINE // DO NOT MODIFY!
  bsA = 16403;
  bsB = RED_WINE;
  if(bsA!=bsB)
    ssErrors << "Defined RED_WINE with value 16403 from .dat file mismatches hardcoded c++ define value of " << RED_WINE << "!" << std::endl;
#endif 

  
#ifdef REGENERATION // DO NOT MODIFY!
  bsA = 67108864;
  bsB = REGENERATION;
  if(bsA!=bsB)
    ssErrors << "Defined REGENERATION with value 67108864 from .dat file mismatches hardcoded c++ define value of " << REGENERATION << "!" << std::endl;
#endif 

  
#ifdef REPRESENTATIVE // DO NOT MODIFY!
  bsA = 6;
  bsB = REPRESENTATIVE;
  if(bsA!=bsB)
    ssErrors << "Defined REPRESENTATIVE with value 6 from .dat file mismatches hardcoded c++ define value of " << REPRESENTATIVE << "!" << std::endl;
#endif 

  
#ifdef RESIDUUM // DO NOT MODIFY!
  bsA = 8220;
  bsB = RESIDUUM;
  if(bsA!=bsB)
    ssErrors << "Defined RESIDUUM with value 8220 from .dat file mismatches hardcoded c++ define value of " << RESIDUUM << "!" << std::endl;
#endif 

  
#ifdef RIGHT // DO NOT MODIFY!
  bsA = 3;
  bsB = RIGHT;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT with value 3 from .dat file mismatches hardcoded c++ define value of " << RIGHT << "!" << std::endl;
#endif 

  
#ifdef RIGHT_ARM // DO NOT MODIFY!
  bsA = 4;
  bsB = RIGHT_ARM;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_ARM with value 4 from .dat file mismatches hardcoded c++ define value of " << RIGHT_ARM << "!" << std::endl;
#endif 

  
#ifdef RIGHT_ARM_INDEX // DO NOT MODIFY!
  bsA = 2;
  bsB = RIGHT_ARM_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_ARM_INDEX with value 2 from .dat file mismatches hardcoded c++ define value of " << RIGHT_ARM_INDEX << "!" << std::endl;
#endif 

  
#ifdef RIGHT_BOOT_INDEX // DO NOT MODIFY!
  bsA = 11;
  bsB = RIGHT_BOOT_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_BOOT_INDEX with value 11 from .dat file mismatches hardcoded c++ define value of " << RIGHT_BOOT_INDEX << "!" << std::endl;
#endif 

  
#ifdef RIGHT_GAUNTLET_INDEX // DO NOT MODIFY!
  bsA = 9;
  bsB = RIGHT_GAUNTLET_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_GAUNTLET_INDEX with value 9 from .dat file mismatches hardcoded c++ define value of " << RIGHT_GAUNTLET_INDEX << "!" << std::endl;
#endif 

  
#ifdef RIGHT_LEG // DO NOT MODIFY!
  bsA = 32;
  bsB = RIGHT_LEG;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_LEG with value 32 from .dat file mismatches hardcoded c++ define value of " << RIGHT_LEG << "!" << std::endl;
#endif 

  
#ifdef RIGHT_LEG_INDEX // DO NOT MODIFY!
  bsA = 5;
  bsB = RIGHT_LEG_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_LEG_INDEX with value 5 from .dat file mismatches hardcoded c++ define value of " << RIGHT_LEG_INDEX << "!" << std::endl;
#endif 

  
#ifdef RIGHT_RING_INDEX // DO NOT MODIFY!
  bsA = 7;
  bsB = RIGHT_RING_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_RING_INDEX with value 7 from .dat file mismatches hardcoded c++ define value of " << RIGHT_RING_INDEX << "!" << std::endl;
#endif 

  
#ifdef RIGHT_WIELDED_INDEX // DO NOT MODIFY!
  bsA = 5;
  bsB = RIGHT_WIELDED_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined RIGHT_WIELDED_INDEX with value 5 from .dat file mismatches hardcoded c++ define value of " << RIGHT_WIELDED_INDEX << "!" << std::endl;
#endif 

  
#ifdef RING // DO NOT MODIFY!
  bsA = 64;
  bsB = RING;
  if(bsA!=bsB)
    ssErrors << "Defined RING with value 64 from .dat file mismatches hardcoded c++ define value of " << RING << "!" << std::endl;
#endif 

  
#ifdef RING_OF_ACID_RESISTANCE // DO NOT MODIFY!
  bsA = 11;
  bsB = RING_OF_ACID_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_ACID_RESISTANCE with value 11 from .dat file mismatches hardcoded c++ define value of " << RING_OF_ACID_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef RING_OF_BRAVERY // DO NOT MODIFY!
  bsA = 17;
  bsB = RING_OF_BRAVERY;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_BRAVERY with value 17 from .dat file mismatches hardcoded c++ define value of " << RING_OF_BRAVERY << "!" << std::endl;
#endif 

  
#ifdef RING_OF_DETECTION // DO NOT MODIFY!
  bsA = 14;
  bsB = RING_OF_DETECTION;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_DETECTION with value 14 from .dat file mismatches hardcoded c++ define value of " << RING_OF_DETECTION << "!" << std::endl;
#endif 

  
#ifdef RING_OF_ELECTRICITY_RESISTANCE // DO NOT MODIFY!
  bsA = 9;
  bsB = RING_OF_ELECTRICITY_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_ELECTRICITY_RESISTANCE with value 9 from .dat file mismatches hardcoded c++ define value of " << RING_OF_ELECTRICITY_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef RING_OF_FIRE_RESISTANCE // DO NOT MODIFY!
  bsA = 1;
  bsB = RING_OF_FIRE_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_FIRE_RESISTANCE with value 1 from .dat file mismatches hardcoded c++ define value of " << RING_OF_FIRE_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef RING_OF_INFRA_VISION // DO NOT MODIFY!
  bsA = 3;
  bsB = RING_OF_INFRA_VISION;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_INFRA_VISION with value 3 from .dat file mismatches hardcoded c++ define value of " << RING_OF_INFRA_VISION << "!" << std::endl;
#endif 

  
#ifdef RING_OF_INVISIBILITY // DO NOT MODIFY!
  bsA = 8;
  bsB = RING_OF_INVISIBILITY;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_INVISIBILITY with value 8 from .dat file mismatches hardcoded c++ define value of " << RING_OF_INVISIBILITY << "!" << std::endl;
#endif 

  
#ifdef RING_OF_LIGHT // DO NOT MODIFY!
  bsA = 12;
  bsB = RING_OF_LIGHT;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_LIGHT with value 12 from .dat file mismatches hardcoded c++ define value of " << RING_OF_LIGHT << "!" << std::endl;
#endif 

  
#ifdef RING_OF_MAGIC_RESISTANCE // DO NOT MODIFY!
  bsA = 13;
  bsB = RING_OF_MAGIC_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_MAGIC_RESISTANCE with value 13 from .dat file mismatches hardcoded c++ define value of " << RING_OF_MAGIC_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef RING_OF_POISON_RESISTANCE // DO NOT MODIFY!
  bsA = 7;
  bsB = RING_OF_POISON_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_POISON_RESISTANCE with value 7 from .dat file mismatches hardcoded c++ define value of " << RING_OF_POISON_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef RING_OF_POLYMORPH // DO NOT MODIFY!
  bsA = 6;
  bsB = RING_OF_POLYMORPH;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_POLYMORPH with value 6 from .dat file mismatches hardcoded c++ define value of " << RING_OF_POLYMORPH << "!" << std::endl;
#endif 

  
#ifdef RING_OF_POLYMORPH_CONTROL // DO NOT MODIFY!
  bsA = 2;
  bsB = RING_OF_POLYMORPH_CONTROL;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_POLYMORPH_CONTROL with value 2 from .dat file mismatches hardcoded c++ define value of " << RING_OF_POLYMORPH_CONTROL << "!" << std::endl;
#endif 

  
#ifdef RING_OF_POLYMORPH_LOCK // DO NOT MODIFY!
  bsA = 15;
  bsB = RING_OF_POLYMORPH_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_POLYMORPH_LOCK with value 15 from .dat file mismatches hardcoded c++ define value of " << RING_OF_POLYMORPH_LOCK << "!" << std::endl;
#endif 

  
#ifdef RING_OF_SEARCHING // DO NOT MODIFY!
  bsA = 10;
  bsB = RING_OF_SEARCHING;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_SEARCHING with value 10 from .dat file mismatches hardcoded c++ define value of " << RING_OF_SEARCHING << "!" << std::endl;
#endif 

  
#ifdef RING_OF_TELEPORTATION // DO NOT MODIFY!
  bsA = 4;
  bsB = RING_OF_TELEPORTATION;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_TELEPORTATION with value 4 from .dat file mismatches hardcoded c++ define value of " << RING_OF_TELEPORTATION << "!" << std::endl;
#endif 

  
#ifdef RING_OF_TELEPORT_CONTROL // DO NOT MODIFY!
  bsA = 5;
  bsB = RING_OF_TELEPORT_CONTROL;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_TELEPORT_CONTROL with value 5 from .dat file mismatches hardcoded c++ define value of " << RING_OF_TELEPORT_CONTROL << "!" << std::endl;
#endif 

  
#ifdef RING_OF_WORM // DO NOT MODIFY!
  bsA = 16;
  bsB = RING_OF_WORM;
  if(bsA!=bsB)
    ssErrors << "Defined RING_OF_WORM with value 16 from .dat file mismatches hardcoded c++ define value of " << RING_OF_WORM << "!" << std::endl;
#endif 

  
#ifdef RISES_FROM_ASHES // DO NOT MODIFY!
  bsA = 64;
  bsB = RISES_FROM_ASHES;
  if(bsA!=bsB)
    ssErrors << "Defined RISES_FROM_ASHES with value 64 from .dat file mismatches hardcoded c++ define value of " << RISES_FROM_ASHES << "!" << std::endl;
#endif 

  
#ifdef ROCK_CRYSTAL // DO NOT MODIFY!
  bsA = 4308;
  bsB = ROCK_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined ROCK_CRYSTAL with value 4308 from .dat file mismatches hardcoded c++ define value of " << ROCK_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef ROCK_SALT // DO NOT MODIFY!
  bsA = 4321;
  bsB = ROCK_SALT;
  if(bsA!=bsB)
    ssErrors << "Defined ROCK_SALT with value 4321 from .dat file mismatches hardcoded c++ define value of " << ROCK_SALT << "!" << std::endl;
#endif 

  
#ifdef ROLLING_PIN // DO NOT MODIFY!
  bsA = 19;
  bsB = ROLLING_PIN;
  if(bsA!=bsB)
    ssErrors << "Defined ROLLING_PIN with value 19 from .dat file mismatches hardcoded c++ define value of " << ROLLING_PIN << "!" << std::endl;
#endif 

  
#ifdef ROOKIE // DO NOT MODIFY!
  bsA = 1;
  bsB = ROOKIE;
  if(bsA!=bsB)
    ssErrors << "Defined ROOKIE with value 1 from .dat file mismatches hardcoded c++ define value of " << ROOKIE << "!" << std::endl;
#endif 

  
#ifdef ROOKIE_FEMALE // DO NOT MODIFY!
  bsA = 14;
  bsB = ROOKIE_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined ROOKIE_FEMALE with value 14 from .dat file mismatches hardcoded c++ define value of " << ROOKIE_FEMALE << "!" << std::endl;
#endif 

  
#ifdef ROOM_BANANA_DROP_AREA // DO NOT MODIFY!
  bsA = 5;
  bsB = ROOM_BANANA_DROP_AREA;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_BANANA_DROP_AREA with value 5 from .dat file mismatches hardcoded c++ define value of " << ROOM_BANANA_DROP_AREA << "!" << std::endl;
#endif 

  
#ifdef ROOM_CATHEDRAL // DO NOT MODIFY!
  bsA = 3;
  bsB = ROOM_CATHEDRAL;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_CATHEDRAL with value 3 from .dat file mismatches hardcoded c++ define value of " << ROOM_CATHEDRAL << "!" << std::endl;
#endif 

  
#ifdef ROOM_LIBRARY // DO NOT MODIFY!
  bsA = 4;
  bsB = ROOM_LIBRARY;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_LIBRARY with value 4 from .dat file mismatches hardcoded c++ define value of " << ROOM_LIBRARY << "!" << std::endl;
#endif 

  
#ifdef ROOM_NORMAL // DO NOT MODIFY!
  bsA = 1;
  bsB = ROOM_NORMAL;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_NORMAL with value 1 from .dat file mismatches hardcoded c++ define value of " << ROOM_NORMAL << "!" << std::endl;
#endif 

  
#ifdef ROOM_SHOP // DO NOT MODIFY!
  bsA = 2;
  bsB = ROOM_SHOP;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_SHOP with value 2 from .dat file mismatches hardcoded c++ define value of " << ROOM_SHOP << "!" << std::endl;
#endif 

  
#ifdef ROOM_SUMO_ARENA // DO NOT MODIFY!
  bsA = 6;
  bsB = ROOM_SUMO_ARENA;
  if(bsA!=bsB)
    ssErrors << "Defined ROOM_SUMO_ARENA with value 6 from .dat file mismatches hardcoded c++ define value of " << ROOM_SUMO_ARENA << "!" << std::endl;
#endif 

  
#ifdef ROSE_QUARTZ // DO NOT MODIFY!
  bsA = 4291;
  bsB = ROSE_QUARTZ;
  if(bsA!=bsB)
    ssErrors << "Defined ROSE_QUARTZ with value 4291 from .dat file mismatches hardcoded c++ define value of " << ROSE_QUARTZ << "!" << std::endl;
#endif 

  
#ifdef ROTATE // DO NOT MODIFY!
  bsA = 4;
  bsB = ROTATE;
  if(bsA!=bsB)
    ssErrors << "Defined ROTATE with value 4 from .dat file mismatches hardcoded c++ define value of " << ROTATE << "!" << std::endl;
#endif 

  
#ifdef ROUND_CORNERS // DO NOT MODIFY!
  bsA = 2;
  bsB = ROUND_CORNERS;
  if(bsA!=bsB)
    ssErrors << "Defined ROUND_CORNERS with value 2 from .dat file mismatches hardcoded c++ define value of " << ROUND_CORNERS << "!" << std::endl;
#endif 

  
#ifdef ROUND_LOCK // DO NOT MODIFY!
  bsA = 2048;
  bsB = ROUND_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined ROUND_LOCK with value 2048 from .dat file mismatches hardcoded c++ define value of " << ROUND_LOCK << "!" << std::endl;
#endif 

  
#ifdef RUBBER // DO NOT MODIFY!
  bsA = 4185;
  bsB = RUBBER;
  if(bsA!=bsB)
    ssErrors << "Defined RUBBER with value 4185 from .dat file mismatches hardcoded c++ define value of " << RUBBER << "!" << std::endl;
#endif 

  
#ifdef RUBY // DO NOT MODIFY!
  bsA = 4297;
  bsB = RUBY;
  if(bsA!=bsB)
    ssErrors << "Defined RUBY with value 4297 from .dat file mismatches hardcoded c++ define value of " << RUBY << "!" << std::endl;
#endif 

  
#ifdef RUNED_WHIP // DO NOT MODIFY!
  bsA = 1;
  bsB = RUNED_WHIP;
  if(bsA!=bsB)
    ssErrors << "Defined RUNED_WHIP with value 1 from .dat file mismatches hardcoded c++ define value of " << RUNED_WHIP << "!" << std::endl;
#endif 

  
#ifdef RUNE_SWORD // DO NOT MODIFY!
  bsA = 23;
  bsB = RUNE_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined RUNE_SWORD with value 23 from .dat file mismatches hardcoded c++ define value of " << RUNE_SWORD << "!" << std::endl;
#endif 

  
#ifdef RUSTED // DO NOT MODIFY!
  bsA = 2;
  bsB = RUSTED;
  if(bsA!=bsB)
    ssErrors << "Defined RUSTED with value 2 from .dat file mismatches hardcoded c++ define value of " << RUSTED << "!" << std::endl;
#endif 

  
#ifdef RYE_BREAD // DO NOT MODIFY!
  bsA = 8206;
  bsB = RYE_BREAD;
  if(bsA!=bsB)
    ssErrors << "Defined RYE_BREAD with value 8206 from .dat file mismatches hardcoded c++ define value of " << RYE_BREAD << "!" << std::endl;
#endif 

  
#ifdef SALT // DO NOT MODIFY!
  bsA = 24578;
  bsB = SALT;
  if(bsA!=bsB)
    ssErrors << "Defined SALT with value 24578 from .dat file mismatches hardcoded c++ define value of " << SALT << "!" << std::endl;
#endif 

  
#ifdef SAND // DO NOT MODIFY!
  bsA = 24580;
  bsB = SAND;
  if(bsA!=bsB)
    ssErrors << "Defined SAND with value 24580 from .dat file mismatches hardcoded c++ define value of " << SAND << "!" << std::endl;
#endif 

  
#ifdef SAND_STONE // DO NOT MODIFY!
  bsA = 4204;
  bsB = SAND_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined SAND_STONE with value 4204 from .dat file mismatches hardcoded c++ define value of " << SAND_STONE << "!" << std::endl;
#endif 

  
#ifdef SAND_TERRAIN // DO NOT MODIFY!
  bsA = 8;
  bsB = SAND_TERRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined SAND_TERRAIN with value 8 from .dat file mismatches hardcoded c++ define value of " << SAND_TERRAIN << "!" << std::endl;
#endif 

  
#ifdef SAPPHIRE // DO NOT MODIFY!
  bsA = 4295;
  bsB = SAPPHIRE;
  if(bsA!=bsB)
    ssErrors << "Defined SAPPHIRE with value 4295 from .dat file mismatches hardcoded c++ define value of " << SAPPHIRE << "!" << std::endl;
#endif 

  
#ifdef SARDINE // DO NOT MODIFY!
  bsA = 20487;
  bsB = SARDINE;
  if(bsA!=bsB)
    ssErrors << "Defined SARDINE with value 20487 from .dat file mismatches hardcoded c++ define value of " << SARDINE << "!" << std::endl;
#endif 

  
#ifdef SCABIES // DO NOT MODIFY!
  bsA = 12;
  bsB = SCABIES;
  if(bsA!=bsB)
    ssErrors << "Defined SCABIES with value 12 from .dat file mismatches hardcoded c++ define value of " << SCABIES << "!" << std::endl;
#endif 

  
#ifdef SCHOOL_FOOD // DO NOT MODIFY!
  bsA = 8210;
  bsB = SCHOOL_FOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SCHOOL_FOOD with value 8210 from .dat file mismatches hardcoded c++ define value of " << SCHOOL_FOOD << "!" << std::endl;
#endif 

  
#ifdef SCROLL // DO NOT MODIFY!
  bsA = 4096;
  bsB = SCROLL;
  if(bsA!=bsB)
    ssErrors << "Defined SCROLL with value 4096 from .dat file mismatches hardcoded c++ define value of " << SCROLL << "!" << std::endl;
#endif 

  
#ifdef SCYTHE // DO NOT MODIFY!
  bsA = 14;
  bsB = SCYTHE;
  if(bsA!=bsB)
    ssErrors << "Defined SCYTHE with value 14 from .dat file mismatches hardcoded c++ define value of " << SCYTHE << "!" << std::endl;
#endif 

  
#ifdef SEARCHING // DO NOT MODIFY!
  bsA = 65536;
  bsB = SEARCHING;
  if(bsA!=bsB)
    ssErrors << "Defined SEARCHING with value 65536 from .dat file mismatches hardcoded c++ define value of " << SEARCHING << "!" << std::endl;
#endif 

  
#ifdef SEA_SERPENT_SCALE // DO NOT MODIFY!
  bsA = 4170;
  bsB = SEA_SERPENT_SCALE;
  if(bsA!=bsB)
    ssErrors << "Defined SEA_SERPENT_SCALE with value 4170 from .dat file mismatches hardcoded c++ define value of " << SEA_SERPENT_SCALE << "!" << std::endl;
#endif 

  
#ifdef SEA_SHELL // DO NOT MODIFY!
  bsA = 4254;
  bsB = SEA_SHELL;
  if(bsA!=bsB)
    ssErrors << "Defined SEA_SHELL with value 4254 from .dat file mismatches hardcoded c++ define value of " << SEA_SHELL << "!" << std::endl;
#endif 

  
#ifdef SEA_WEED // DO NOT MODIFY!
  bsA = 4166;
  bsB = SEA_WEED;
  if(bsA!=bsB)
    ssErrors << "Defined SEA_WEED with value 4166 from .dat file mismatches hardcoded c++ define value of " << SEA_WEED << "!" << std::endl;
#endif 

  
#ifdef SECRET_DOOR // DO NOT MODIFY!
  bsA = 2;
  bsB = SECRET_DOOR;
  if(bsA!=bsB)
    ssErrors << "Defined SECRET_DOOR with value 2 from .dat file mismatches hardcoded c++ define value of " << SECRET_DOOR << "!" << std::endl;
#endif 

  
#ifdef SEGES // DO NOT MODIFY!
  bsA = 5;
  bsB = SEGES;
  if(bsA!=bsB)
    ssErrors << "Defined SEGES with value 5 from .dat file mismatches hardcoded c++ define value of " << SEGES << "!" << std::endl;
#endif 

  
#ifdef SELKIE_SKIN // DO NOT MODIFY!
  bsA = 4169;
  bsB = SELKIE_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined SELKIE_SKIN with value 4169 from .dat file mismatches hardcoded c++ define value of " << SELKIE_SKIN << "!" << std::endl;
#endif 

  
#ifdef SHACKLES // DO NOT MODIFY!
  bsA = 35;
  bsB = SHACKLES;
  if(bsA!=bsB)
    ssErrors << "Defined SHACKLES with value 35 from .dat file mismatches hardcoded c++ define value of " << SHACKLES << "!" << std::endl;
#endif 

  
#ifdef SHADOW_CLOTH // DO NOT MODIFY!
  bsA = 4175;
  bsB = SHADOW_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined SHADOW_CLOTH with value 4175 from .dat file mismatches hardcoded c++ define value of " << SHADOW_CLOTH << "!" << std::endl;
#endif 

  
#ifdef SHALE // DO NOT MODIFY!
  bsA = 4261;
  bsB = SHALE;
  if(bsA!=bsB)
    ssErrors << "Defined SHALE with value 4261 from .dat file mismatches hardcoded c++ define value of " << SHALE << "!" << std::endl;
#endif 

  
#ifdef SHARD // DO NOT MODIFY!
  bsA = 19;
  bsB = SHARD;
  if(bsA!=bsB)
    ssErrors << "Defined SHARD with value 19 from .dat file mismatches hardcoded c++ define value of " << SHARD << "!" << std::endl;
#endif 

  
#ifdef SHARK_TOOTH // DO NOT MODIFY!
  bsA = 8216;
  bsB = SHARK_TOOTH;
  if(bsA!=bsB)
    ssErrors << "Defined SHARK_TOOTH with value 8216 from .dat file mismatches hardcoded c++ define value of " << SHARK_TOOTH << "!" << std::endl;
#endif 

  
#ifdef SHELF // DO NOT MODIFY!
  bsA = 3;
  bsB = SHELF;
  if(bsA!=bsB)
    ssErrors << "Defined SHELF with value 3 from .dat file mismatches hardcoded c++ define value of " << SHELF << "!" << std::endl;
#endif 

  
#ifdef SHIELD // DO NOT MODIFY!
  bsA = 32;
  bsB = SHIELD;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELD with value 32 from .dat file mismatches hardcoded c++ define value of " << SHIELD << "!" << std::endl;
#endif 

  
#ifdef SHIELDS // DO NOT MODIFY!
  bsA = 10;
  bsB = SHIELDS;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELDS with value 10 from .dat file mismatches hardcoded c++ define value of " << SHIELDS << "!" << std::endl;
#endif 

  
#ifdef SHIELD_BEAM // DO NOT MODIFY!
  bsA = 2;
  bsB = SHIELD_BEAM;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELD_BEAM with value 2 from .dat file mismatches hardcoded c++ define value of " << SHIELD_BEAM << "!" << std::endl;
#endif 

  
#ifdef SHIELD_OF_ELECTRICITY_RESISTANCE // DO NOT MODIFY!
  bsA = 4;
  bsB = SHIELD_OF_ELECTRICITY_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELD_OF_ELECTRICITY_RESISTANCE with value 4 from .dat file mismatches hardcoded c++ define value of " << SHIELD_OF_ELECTRICITY_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef SHIELD_OF_FIRE_RESISTANCE // DO NOT MODIFY!
  bsA = 3;
  bsB = SHIELD_OF_FIRE_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELD_OF_FIRE_RESISTANCE with value 3 from .dat file mismatches hardcoded c++ define value of " << SHIELD_OF_FIRE_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef SHIELD_OF_MAGIC_RESISTANCE // DO NOT MODIFY!
  bsA = 5;
  bsB = SHIELD_OF_MAGIC_RESISTANCE;
  if(bsA!=bsB)
    ssErrors << "Defined SHIELD_OF_MAGIC_RESISTANCE with value 5 from .dat file mismatches hardcoded c++ define value of " << SHIELD_OF_MAGIC_RESISTANCE << "!" << std::endl;
#endif 

  
#ifdef SHOP // DO NOT MODIFY!
  bsA = 5;
  bsB = SHOP;
  if(bsA!=bsB)
    ssErrors << "Defined SHOP with value 5 from .dat file mismatches hardcoded c++ define value of " << SHOP << "!" << std::endl;
#endif 

  
#ifdef SHORT_SWORD // DO NOT MODIFY!
  bsA = 11;
  bsB = SHORT_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined SHORT_SWORD with value 11 from .dat file mismatches hardcoded c++ define value of " << SHORT_SWORD << "!" << std::endl;
#endif 

  
#ifdef SHOW_MATERIAL // DO NOT MODIFY!
  bsA = 16;
  bsB = SHOW_MATERIAL;
  if(bsA!=bsB)
    ssErrors << "Defined SHOW_MATERIAL with value 16 from .dat file mismatches hardcoded c++ define value of " << SHOW_MATERIAL << "!" << std::endl;
#endif 

  
#ifdef SICKLE // DO NOT MODIFY!
  bsA = 9;
  bsB = SICKLE;
  if(bsA!=bsB)
    ssErrors << "Defined SICKLE with value 9 from .dat file mismatches hardcoded c++ define value of " << SICKLE << "!" << std::endl;
#endif 

  
#ifdef SICK_BLOOD // DO NOT MODIFY!
  bsA = 16428;
  bsB = SICK_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SICK_BLOOD with value 16428 from .dat file mismatches hardcoded c++ define value of " << SICK_BLOOD << "!" << std::endl;
#endif 

  
#ifdef SICK_SPIDER_FLESH // DO NOT MODIFY!
  bsA = 20529;
  bsB = SICK_SPIDER_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined SICK_SPIDER_FLESH with value 20529 from .dat file mismatches hardcoded c++ define value of " << SICK_SPIDER_FLESH << "!" << std::endl;
#endif 

  
#ifdef SIDGURE_WOOD // DO NOT MODIFY!
  bsA = 4246;
  bsB = SIDGURE_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SIDGURE_WOOD with value 4246 from .dat file mismatches hardcoded c++ define value of " << SIDGURE_WOOD << "!" << std::endl;
#endif 

  
#ifdef SILVA // DO NOT MODIFY!
  bsA = 7;
  bsB = SILVA;
  if(bsA!=bsB)
    ssErrors << "Defined SILVA with value 7 from .dat file mismatches hardcoded c++ define value of " << SILVA << "!" << std::endl;
#endif 

  
#ifdef SILVER // DO NOT MODIFY!
  bsA = 4107;
  bsB = SILVER;
  if(bsA!=bsB)
    ssErrors << "Defined SILVER with value 4107 from .dat file mismatches hardcoded c++ define value of " << SILVER << "!" << std::endl;
#endif 

  
#ifdef SKELETON_DOG // DO NOT MODIFY!
  bsA = 1;
  bsB = SKELETON_DOG;
  if(bsA!=bsB)
    ssErrors << "Defined SKELETON_DOG with value 1 from .dat file mismatches hardcoded c++ define value of " << SKELETON_DOG << "!" << std::endl;
#endif 

  
#ifdef SKIN_COLOR // DO NOT MODIFY!
  bsA = 1;
  bsB = SKIN_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined SKIN_COLOR with value 1 from .dat file mismatches hardcoded c++ define value of " << SKIN_COLOR << "!" << std::endl;
#endif 

  
#ifdef SKUNK_FLESH // DO NOT MODIFY!
  bsA = 20523;
  bsB = SKUNK_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined SKUNK_FLESH with value 20523 from .dat file mismatches hardcoded c++ define value of " << SKUNK_FLESH << "!" << std::endl;
#endif 

  
#ifdef SKUNK_SMELL // DO NOT MODIFY!
  bsA = 12293;
  bsB = SKUNK_SMELL;
  if(bsA!=bsB)
    ssErrors << "Defined SKUNK_SMELL with value 12293 from .dat file mismatches hardcoded c++ define value of " << SKUNK_SMELL << "!" << std::endl;
#endif 

  
#ifdef SLADE // DO NOT MODIFY!
  bsA = 4283;
  bsB = SLADE;
  if(bsA!=bsB)
    ssErrors << "Defined SLADE with value 4283 from .dat file mismatches hardcoded c++ define value of " << SLADE << "!" << std::endl;
#endif 

  
#ifdef SLASH // DO NOT MODIFY!
  bsA = 2;
  bsB = SLASH;
  if(bsA!=bsB)
    ssErrors << "Defined SLASH with value 2 from .dat file mismatches hardcoded c++ define value of " << SLASH << "!" << std::endl;
#endif 

  
#ifdef SLATE // DO NOT MODIFY!
  bsA = 4273;
  bsB = SLATE;
  if(bsA!=bsB)
    ssErrors << "Defined SLATE with value 4273 from .dat file mismatches hardcoded c++ define value of " << SLATE << "!" << std::endl;
#endif 

  
#ifdef SLAUGHTERER // DO NOT MODIFY!
  bsA = 1;
  bsB = SLAUGHTERER;
  if(bsA!=bsB)
    ssErrors << "Defined SLAUGHTERER with value 1 from .dat file mismatches hardcoded c++ define value of " << SLAUGHTERER << "!" << std::endl;
#endif 

  
#ifdef SLAYER // DO NOT MODIFY!
  bsA = 1;
  bsB = SLAYER;
  if(bsA!=bsB)
    ssErrors << "Defined SLAYER with value 1 from .dat file mismatches hardcoded c++ define value of " << SLAYER << "!" << std::endl;
#endif 

  
#ifdef SLEEPING_GAS // DO NOT MODIFY!
  bsA = 12300;
  bsB = SLEEPING_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined SLEEPING_GAS with value 12300 from .dat file mismatches hardcoded c++ define value of " << SLEEPING_GAS << "!" << std::endl;
#endif 

  
#ifdef SLIGHTLY_BURNT // DO NOT MODIFY!
  bsA = 1;
  bsB = SLIGHTLY_BURNT;
  if(bsA!=bsB)
    ssErrors << "Defined SLIGHTLY_BURNT with value 1 from .dat file mismatches hardcoded c++ define value of " << SLIGHTLY_BURNT << "!" << std::endl;
#endif 

  
#ifdef SLIGHTLY_RUSTED // DO NOT MODIFY!
  bsA = 1;
  bsB = SLIGHTLY_RUSTED;
  if(bsA!=bsB)
    ssErrors << "Defined SLIGHTLY_RUSTED with value 1 from .dat file mismatches hardcoded c++ define value of " << SLIGHTLY_RUSTED << "!" << std::endl;
#endif 

  
#ifdef SLOW // DO NOT MODIFY!
  bsA = 4;
  bsB = SLOW;
  if(bsA!=bsB)
    ssErrors << "Defined SLOW with value 4 from .dat file mismatches hardcoded c++ define value of " << SLOW << "!" << std::endl;
#endif 

  
#ifdef SMALL_CHEST // DO NOT MODIFY!
  bsA = 1;
  bsB = SMALL_CHEST;
  if(bsA!=bsB)
    ssErrors << "Defined SMALL_CHEST with value 1 from .dat file mismatches hardcoded c++ define value of " << SMALL_CHEST << "!" << std::endl;
#endif 

  
#ifdef SMALL_CLOCK // DO NOT MODIFY!
  bsA = 5;
  bsB = SMALL_CLOCK;
  if(bsA!=bsB)
    ssErrors << "Defined SMALL_CLOCK with value 5 from .dat file mismatches hardcoded c++ define value of " << SMALL_CLOCK << "!" << std::endl;
#endif 

  
#ifdef SMALL_SWORDS // DO NOT MODIFY!
  bsA = 4;
  bsB = SMALL_SWORDS;
  if(bsA!=bsB)
    ssErrors << "Defined SMALL_SWORDS with value 4 from .dat file mismatches hardcoded c++ define value of " << SMALL_SWORDS << "!" << std::endl;
#endif 

  
#ifdef SMART_CLOTH // DO NOT MODIFY!
  bsA = 4179;
  bsB = SMART_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined SMART_CLOTH with value 4179 from .dat file mismatches hardcoded c++ define value of " << SMART_CLOTH << "!" << std::endl;
#endif 

  
#ifdef SMOKE // DO NOT MODIFY!
  bsA = 12292;
  bsB = SMOKE;
  if(bsA!=bsB)
    ssErrors << "Defined SMOKE with value 12292 from .dat file mismatches hardcoded c++ define value of " << SMOKE << "!" << std::endl;
#endif 

  
#ifdef SNAKE_FLESH // DO NOT MODIFY!
  bsA = 20515;
  bsB = SNAKE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined SNAKE_FLESH with value 20515 from .dat file mismatches hardcoded c++ define value of " << SNAKE_FLESH << "!" << std::endl;
#endif 

  
#ifdef SNAKE_JERKY // DO NOT MODIFY!
  bsA = 20485;
  bsB = SNAKE_JERKY;
  if(bsA!=bsB)
    ssErrors << "Defined SNAKE_JERKY with value 20485 from .dat file mismatches hardcoded c++ define value of " << SNAKE_JERKY << "!" << std::endl;
#endif 

  
#ifdef SNAKE_SKIN // DO NOT MODIFY!
  bsA = 4144;
  bsB = SNAKE_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined SNAKE_SKIN with value 4144 from .dat file mismatches hardcoded c++ define value of " << SNAKE_SKIN << "!" << std::endl;
#endif 

  
#ifdef SNOW // DO NOT MODIFY!
  bsA = 24579;
  bsB = SNOW;
  if(bsA!=bsB)
    ssErrors << "Defined SNOW with value 24579 from .dat file mismatches hardcoded c++ define value of " << SNOW << "!" << std::endl;
#endif 

  
#ifdef SNOW_BOULDER // DO NOT MODIFY!
  bsA = 4;
  bsB = SNOW_BOULDER;
  if(bsA!=bsB)
    ssErrors << "Defined SNOW_BOULDER with value 4 from .dat file mismatches hardcoded c++ define value of " << SNOW_BOULDER << "!" << std::endl;
#endif 

  
#ifdef SNOW_FIR // DO NOT MODIFY!
  bsA = 17;
  bsB = SNOW_FIR;
  if(bsA!=bsB)
    ssErrors << "Defined SNOW_FIR with value 17 from .dat file mismatches hardcoded c++ define value of " << SNOW_FIR << "!" << std::endl;
#endif 

  
#ifdef SNOW_PINE // DO NOT MODIFY!
  bsA = 16;
  bsB = SNOW_PINE;
  if(bsA!=bsB)
    ssErrors << "Defined SNOW_PINE with value 16 from .dat file mismatches hardcoded c++ define value of " << SNOW_PINE << "!" << std::endl;
#endif 

  
#ifdef SNOW_TERRAIN // DO NOT MODIFY!
  bsA = 6;
  bsB = SNOW_TERRAIN;
  if(bsA!=bsB)
    ssErrors << "Defined SNOW_TERRAIN with value 6 from .dat file mismatches hardcoded c++ define value of " << SNOW_TERRAIN << "!" << std::endl;
#endif 

  
#ifdef SOIL // DO NOT MODIFY!
  bsA = 24584;
  bsB = SOIL;
  if(bsA!=bsB)
    ssErrors << "Defined SOIL with value 24584 from .dat file mismatches hardcoded c++ define value of " << SOIL << "!" << std::endl;
#endif 

  
#ifdef SOLARIUM // DO NOT MODIFY!
  bsA = 4199;
  bsB = SOLARIUM;
  if(bsA!=bsB)
    ssErrors << "Defined SOLARIUM with value 4199 from .dat file mismatches hardcoded c++ define value of " << SOLARIUM << "!" << std::endl;
#endif 

  
#ifdef SOLID_ID // DO NOT MODIFY!
  bsA = 4096;
  bsB = SOLID_ID;
  if(bsA!=bsB)
    ssErrors << "Defined SOLID_ID with value 4096 from .dat file mismatches hardcoded c++ define value of " << SOLID_ID << "!" << std::endl;
#endif 

  
#ifdef SOL_STONE // DO NOT MODIFY!
  bsA = 1;
  bsB = SOL_STONE;
  if(bsA!=bsB)
    ssErrors << "Defined SOL_STONE with value 1 from .dat file mismatches hardcoded c++ define value of " << SOL_STONE << "!" << std::endl;
#endif 

  
#ifdef SOOT // DO NOT MODIFY!
  bsA = 24586;
  bsB = SOOT;
  if(bsA!=bsB)
    ssErrors << "Defined SOOT with value 24586 from .dat file mismatches hardcoded c++ define value of " << SOOT << "!" << std::endl;
#endif 

  
#ifdef SOPHOS // DO NOT MODIFY!
  bsA = 6;
  bsB = SOPHOS;
  if(bsA!=bsB)
    ssErrors << "Defined SOPHOS with value 6 from .dat file mismatches hardcoded c++ define value of " << SOPHOS << "!" << std::endl;
#endif 

  
#ifdef SOUL_STEEL // DO NOT MODIFY!
  bsA = 28681;
  bsB = SOUL_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined SOUL_STEEL with value 28681 from .dat file mismatches hardcoded c++ define value of " << SOUL_STEEL << "!" << std::endl;
#endif 

  
#ifdef SOUND // DO NOT MODIFY!
  bsA = 2;
  bsB = SOUND;
  if(bsA!=bsB)
    ssErrors << "Defined SOUND with value 2 from .dat file mismatches hardcoded c++ define value of " << SOUND << "!" << std::endl;
#endif 

  
#ifdef SPEAR // DO NOT MODIFY!
  bsA = 4;
  bsB = SPEAR;
  if(bsA!=bsB)
    ssErrors << "Defined SPEAR with value 4 from .dat file mismatches hardcoded c++ define value of " << SPEAR << "!" << std::endl;
#endif 

  
#ifdef SPIDER_BLOOD // DO NOT MODIFY!
  bsA = 16426;
  bsB = SPIDER_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SPIDER_BLOOD with value 16426 from .dat file mismatches hardcoded c++ define value of " << SPIDER_BLOOD << "!" << std::endl;
#endif 

  
#ifdef SPIDER_FLESH // DO NOT MODIFY!
  bsA = 20497;
  bsB = SPIDER_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined SPIDER_FLESH with value 20497 from .dat file mismatches hardcoded c++ define value of " << SPIDER_FLESH << "!" << std::endl;
#endif 

  
#ifdef SPIDER_LEVEL // DO NOT MODIFY!
  bsA = 4;
  bsB = SPIDER_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined SPIDER_LEVEL with value 4 from .dat file mismatches hardcoded c++ define value of " << SPIDER_LEVEL << "!" << std::endl;
#endif 

  
#ifdef SPIDER_SILK // DO NOT MODIFY!
  bsA = 4138;
  bsB = SPIDER_SILK;
  if(bsA!=bsB)
    ssErrors << "Defined SPIDER_SILK with value 4138 from .dat file mismatches hardcoded c++ define value of " << SPIDER_SILK << "!" << std::endl;
#endif 

  
#ifdef SPIRIT_CLOTH // DO NOT MODIFY!
  bsA = 4140;
  bsB = SPIRIT_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined SPIRIT_CLOTH with value 4140 from .dat file mismatches hardcoded c++ define value of " << SPIRIT_CLOTH << "!" << std::endl;
#endif 

  
#ifdef SPRUCE_WOOD // DO NOT MODIFY!
  bsA = 4227;
  bsB = SPRUCE_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SPRUCE_WOOD with value 4227 from .dat file mismatches hardcoded c++ define value of " << SPRUCE_WOOD << "!" << std::endl;
#endif 

  
#ifdef SQUAD_LEADER // DO NOT MODIFY!
  bsA = 2;
  bsB = SQUAD_LEADER;
  if(bsA!=bsB)
    ssErrors << "Defined SQUAD_LEADER with value 2 from .dat file mismatches hardcoded c++ define value of " << SQUAD_LEADER << "!" << std::endl;
#endif 

  
#ifdef SQUARE_LOCK // DO NOT MODIFY!
  bsA = 3072;
  bsB = SQUARE_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined SQUARE_LOCK with value 3072 from .dat file mismatches hardcoded c++ define value of " << SQUARE_LOCK << "!" << std::endl;
#endif 

  
#ifdef STAINLESS_STEEL // DO NOT MODIFY!
  bsA = 4188;
  bsB = STAINLESS_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined STAINLESS_STEEL with value 4188 from .dat file mismatches hardcoded c++ define value of " << STAINLESS_STEEL << "!" << std::endl;
#endif 

  
#ifdef STAIRS_DOWN // DO NOT MODIFY!
  bsA = 200;
  bsB = STAIRS_DOWN;
  if(bsA!=bsB)
    ssErrors << "Defined STAIRS_DOWN with value 200 from .dat file mismatches hardcoded c++ define value of " << STAIRS_DOWN << "!" << std::endl;
#endif 

  
#ifdef STAIRS_UP // DO NOT MODIFY!
  bsA = 100;
  bsB = STAIRS_UP;
  if(bsA!=bsB)
    ssErrors << "Defined STAIRS_UP with value 100 from .dat file mismatches hardcoded c++ define value of " << STAIRS_UP << "!" << std::endl;
#endif 

  
#ifdef STAR_METAL // DO NOT MODIFY!
  bsA = 4112;
  bsB = STAR_METAL;
  if(bsA!=bsB)
    ssErrors << "Defined STAR_METAL with value 4112 from .dat file mismatches hardcoded c++ define value of " << STAR_METAL << "!" << std::endl;
#endif 

  
#ifdef STEEL // DO NOT MODIFY!
  bsA = 28675;
  bsB = STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined STEEL with value 28675 from .dat file mismatches hardcoded c++ define value of " << STEEL << "!" << std::endl;
#endif 

  
#ifdef STEEL_LEAF // DO NOT MODIFY!
  bsA = 4220;
  bsB = STEEL_LEAF;
  if(bsA!=bsB)
    ssErrors << "Defined STEEL_LEAF with value 4220 from .dat file mismatches hardcoded c++ define value of " << STEEL_LEAF << "!" << std::endl;
#endif 

  
#ifdef STEPPE // DO NOT MODIFY!
  bsA = 3;
  bsB = STEPPE;
  if(bsA!=bsB)
    ssErrors << "Defined STEPPE with value 3 from .dat file mismatches hardcoded c++ define value of " << STEPPE << "!" << std::endl;
#endif 

  
#ifdef STONE_WALL // DO NOT MODIFY!
  bsA = 6;
  bsB = STONE_WALL;
  if(bsA!=bsB)
    ssErrors << "Defined STONE_WALL with value 6 from .dat file mismatches hardcoded c++ define value of " << STONE_WALL << "!" << std::endl;
#endif 

  
#ifdef STRANGE_TREE // DO NOT MODIFY!
  bsA = 38;
  bsB = STRANGE_TREE;
  if(bsA!=bsB)
    ssErrors << "Defined STRANGE_TREE with value 38 from .dat file mismatches hardcoded c++ define value of " << STRANGE_TREE << "!" << std::endl;
#endif 

  
#ifdef STRONG_BOX // DO NOT MODIFY!
  bsA = 4;
  bsB = STRONG_BOX;
  if(bsA!=bsB)
    ssErrors << "Defined STRONG_BOX with value 4 from .dat file mismatches hardcoded c++ define value of " << STRONG_BOX << "!" << std::endl;
#endif 

  
#ifdef STUDDED_LEATHER // DO NOT MODIFY!
  bsA = 4155;
  bsB = STUDDED_LEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined STUDDED_LEATHER with value 4155 from .dat file mismatches hardcoded c++ define value of " << STUDDED_LEATHER << "!" << std::endl;
#endif 

  
#ifdef STYMPHALIAN_FEATHER // DO NOT MODIFY!
  bsA = 4164;
  bsB = STYMPHALIAN_FEATHER;
  if(bsA!=bsB)
    ssErrors << "Defined STYMPHALIAN_FEATHER with value 4164 from .dat file mismatches hardcoded c++ define value of " << STYMPHALIAN_FEATHER << "!" << std::endl;
#endif 

  
#ifdef SULFUR // DO NOT MODIFY!
  bsA = 4316;
  bsB = SULFUR;
  if(bsA!=bsB)
    ssErrors << "Defined SULFUR with value 4316 from .dat file mismatches hardcoded c++ define value of " << SULFUR << "!" << std::endl;
#endif 

  
#ifdef SULPHURIC_ACID // DO NOT MODIFY!
  bsA = 16411;
  bsB = SULPHURIC_ACID;
  if(bsA!=bsB)
    ssErrors << "Defined SULPHURIC_ACID with value 16411 from .dat file mismatches hardcoded c++ define value of " << SULPHURIC_ACID << "!" << std::endl;
#endif 

  
#ifdef SUMO_ARENA_ENTRY // DO NOT MODIFY!
  bsA = 700;
  bsB = SUMO_ARENA_ENTRY;
  if(bsA!=bsB)
    ssErrors << "Defined SUMO_ARENA_ENTRY with value 700 from .dat file mismatches hardcoded c++ define value of " << SUMO_ARENA_ENTRY << "!" << std::endl;
#endif 

  
#ifdef SUMO_ARENA_EXIT // DO NOT MODIFY!
  bsA = 800;
  bsB = SUMO_ARENA_EXIT;
  if(bsA!=bsB)
    ssErrors << "Defined SUMO_ARENA_EXIT with value 800 from .dat file mismatches hardcoded c++ define value of " << SUMO_ARENA_EXIT << "!" << std::endl;
#endif 

  
#ifdef SUMO_TEAM // DO NOT MODIFY!
  bsA = 3;
  bsB = SUMO_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined SUMO_TEAM with value 3 from .dat file mismatches hardcoded c++ define value of " << SUMO_TEAM << "!" << std::endl;
#endif 

  
#ifdef SUN_CRYSTAL // DO NOT MODIFY!
  bsA = 4312;
  bsB = SUN_CRYSTAL;
  if(bsA!=bsB)
    ssErrors << "Defined SUN_CRYSTAL with value 4312 from .dat file mismatches hardcoded c++ define value of " << SUN_CRYSTAL << "!" << std::endl;
#endif 

  
#ifdef SUPER_CONCRETE // DO NOT MODIFY!
  bsA = 4315;
  bsB = SUPER_CONCRETE;
  if(bsA!=bsB)
    ssErrors << "Defined SUPER_CONCRETE with value 4315 from .dat file mismatches hardcoded c++ define value of " << SUPER_CONCRETE << "!" << std::endl;
#endif 

  
#ifdef SWEAT // DO NOT MODIFY!
  bsA = 16418;
  bsB = SWEAT;
  if(bsA!=bsB)
    ssErrors << "Defined SWEAT with value 16418 from .dat file mismatches hardcoded c++ define value of " << SWEAT << "!" << std::endl;
#endif 

  
#ifdef SWIM // DO NOT MODIFY!
  bsA = 2;
  bsB = SWIM;
  if(bsA!=bsB)
    ssErrors << "Defined SWIM with value 2 from .dat file mismatches hardcoded c++ define value of " << SWIM << "!" << std::endl;
#endif 

  
#ifdef SWIMMING // DO NOT MODIFY!
  bsA = 8388608;
  bsB = SWIMMING;
  if(bsA!=bsB)
    ssErrors << "Defined SWIMMING with value 8388608 from .dat file mismatches hardcoded c++ define value of " << SWIMMING << "!" << std::endl;
#endif 

  
#ifdef SYCAMORE_WOOD // DO NOT MODIFY!
  bsA = 4228;
  bsB = SYCAMORE_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined SYCAMORE_WOOD with value 4228 from .dat file mismatches hardcoded c++ define value of " << SYCAMORE_WOOD << "!" << std::endl;
#endif 

  
#ifdef SYNTH_CLOTH // DO NOT MODIFY!
  bsA = 4182;
  bsB = SYNTH_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined SYNTH_CLOTH with value 4182 from .dat file mismatches hardcoded c++ define value of " << SYNTH_CLOTH << "!" << std::endl;
#endif 

  
#ifdef S_LOCK_ID // DO NOT MODIFY!
  bsA = 16384;
  bsB = S_LOCK_ID;
  if(bsA!=bsB)
    ssErrors << "Defined S_LOCK_ID with value 16384 from .dat file mismatches hardcoded c++ define value of " << S_LOCK_ID << "!" << std::endl;
#endif 

  
#ifdef TABLE // DO NOT MODIFY!
  bsA = 33;
  bsB = TABLE;
  if(bsA!=bsB)
    ssErrors << "Defined TABLE with value 33 from .dat file mismatches hardcoded c++ define value of " << TABLE << "!" << std::endl;
#endif 

  
#ifdef TEAK // DO NOT MODIFY!
  bsA = 23;
  bsB = TEAK;
  if(bsA!=bsB)
    ssErrors << "Defined TEAK with value 23 from .dat file mismatches hardcoded c++ define value of " << TEAK << "!" << std::endl;
#endif 

  
#ifdef TEAK_WOOD // DO NOT MODIFY!
  bsA = 4242;
  bsB = TEAK_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined TEAK_WOOD with value 4242 from .dat file mismatches hardcoded c++ define value of " << TEAK_WOOD << "!" << std::endl;
#endif 

  
#ifdef TELEPORT // DO NOT MODIFY!
  bsA = 1024;
  bsB = TELEPORT;
  if(bsA!=bsB)
    ssErrors << "Defined TELEPORT with value 1024 from .dat file mismatches hardcoded c++ define value of " << TELEPORT << "!" << std::endl;
#endif 

  
#ifdef TELEPORT_CONTROL // DO NOT MODIFY!
  bsA = 4096;
  bsB = TELEPORT_CONTROL;
  if(bsA!=bsB)
    ssErrors << "Defined TELEPORT_CONTROL with value 4096 from .dat file mismatches hardcoded c++ define value of " << TELEPORT_CONTROL << "!" << std::endl;
#endif 

  
#ifdef TELEPORT_FLUID // DO NOT MODIFY!
  bsA = 16439;
  bsB = TELEPORT_FLUID;
  if(bsA!=bsB)
    ssErrors << "Defined TELEPORT_FLUID with value 16439 from .dat file mismatches hardcoded c++ define value of " << TELEPORT_FLUID << "!" << std::endl;
#endif 

  
#ifdef TELEPORT_GAS // DO NOT MODIFY!
  bsA = 12301;
  bsB = TELEPORT_GAS;
  if(bsA!=bsB)
    ssErrors << "Defined TELEPORT_GAS with value 12301 from .dat file mismatches hardcoded c++ define value of " << TELEPORT_GAS << "!" << std::endl;
#endif 

  
#ifdef TELEPORT_LOCK // DO NOT MODIFY!
  bsA = 268435456;
  bsB = TELEPORT_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined TELEPORT_LOCK with value 268435456 from .dat file mismatches hardcoded c++ define value of " << TELEPORT_LOCK << "!" << std::endl;
#endif 

  
#ifdef TEMPLAR // DO NOT MODIFY!
  bsA = 4;
  bsB = TEMPLAR;
  if(bsA!=bsB)
    ssErrors << "Defined TEMPLAR with value 4 from .dat file mismatches hardcoded c++ define value of " << TEMPLAR << "!" << std::endl;
#endif 

  
#ifdef TERRA // DO NOT MODIFY!
  bsA = 7;
  bsB = TERRA;
  if(bsA!=bsB)
    ssErrors << "Defined TERRA with value 7 from .dat file mismatches hardcoded c++ define value of " << TERRA << "!" << std::endl;
#endif 

  
#ifdef TERRA_TEAM // DO NOT MODIFY!
  bsA = 16;
  bsB = TERRA_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined TERRA_TEAM with value 16 from .dat file mismatches hardcoded c++ define value of " << TERRA_TEAM << "!" << std::endl;
#endif 

  
#ifdef THROW // DO NOT MODIFY!
  bsA = 32768;
  bsB = THROW;
  if(bsA!=bsB)
    ssErrors << "Defined THROW with value 32768 from .dat file mismatches hardcoded c++ define value of " << THROW << "!" << std::endl;
#endif 

  
#ifdef TIN // DO NOT MODIFY!
  bsA = 4098;
  bsB = TIN;
  if(bsA!=bsB)
    ssErrors << "Defined TIN with value 4098 from .dat file mismatches hardcoded c++ define value of " << TIN << "!" << std::endl;
#endif 

  
#ifdef TITANITE // DO NOT MODIFY!
  bsA = 4127;
  bsB = TITANITE;
  if(bsA!=bsB)
    ssErrors << "Defined TITANITE with value 4127 from .dat file mismatches hardcoded c++ define value of " << TITANITE << "!" << std::endl;
#endif 

  
#ifdef TOMB_ENTRY // DO NOT MODIFY!
  bsA = 9;
  bsB = TOMB_ENTRY;
  if(bsA!=bsB)
    ssErrors << "Defined TOMB_ENTRY with value 9 from .dat file mismatches hardcoded c++ define value of " << TOMB_ENTRY << "!" << std::endl;
#endif 

  
#ifdef TOMB_ENTRY_MASTER // DO NOT MODIFY!
  bsA = 10;
  bsB = TOMB_ENTRY_MASTER;
  if(bsA!=bsB)
    ssErrors << "Defined TOMB_ENTRY_MASTER with value 10 from .dat file mismatches hardcoded c++ define value of " << TOMB_ENTRY_MASTER << "!" << std::endl;
#endif 

  
#ifdef TOOL // DO NOT MODIFY!
  bsA = 32768;
  bsB = TOOL;
  if(bsA!=bsB)
    ssErrors << "Defined TOOL with value 32768 from .dat file mismatches hardcoded c++ define value of " << TOOL << "!" << std::endl;
#endif 

  
#ifdef TOPAZ // DO NOT MODIFY!
  bsA = 4294;
  bsB = TOPAZ;
  if(bsA!=bsB)
    ssErrors << "Defined TOPAZ with value 4294 from .dat file mismatches hardcoded c++ define value of " << TOPAZ << "!" << std::endl;
#endif 

  
#ifdef TORSO // DO NOT MODIFY!
  bsA = 1;
  bsB = TORSO;
  if(bsA!=bsB)
    ssErrors << "Defined TORSO with value 1 from .dat file mismatches hardcoded c++ define value of " << TORSO << "!" << std::endl;
#endif 

  
#ifdef TORSO_INDEX // DO NOT MODIFY!
  bsA = 0;
  bsB = TORSO_INDEX;
  if(bsA!=bsB)
    ssErrors << "Defined TORSO_INDEX with value 0 from .dat file mismatches hardcoded c++ define value of " << TORSO_INDEX << "!" << std::endl;
#endif 

  
#ifdef TORSO_MAIN_COLOR // DO NOT MODIFY!
  bsA = 16;
  bsB = TORSO_MAIN_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined TORSO_MAIN_COLOR with value 16 from .dat file mismatches hardcoded c++ define value of " << TORSO_MAIN_COLOR << "!" << std::endl;
#endif 

  
#ifdef TORSO_SPECIAL_COLOR // DO NOT MODIFY!
  bsA = 128;
  bsB = TORSO_SPECIAL_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined TORSO_SPECIAL_COLOR with value 128 from .dat file mismatches hardcoded c++ define value of " << TORSO_SPECIAL_COLOR << "!" << std::endl;
#endif 

  
#ifdef TORTURE_RACK // DO NOT MODIFY!
  bsA = 30;
  bsB = TORTURE_RACK;
  if(bsA!=bsB)
    ssErrors << "Defined TORTURE_RACK with value 30 from .dat file mismatches hardcoded c++ define value of " << TORTURE_RACK << "!" << std::endl;
#endif 

  
#ifdef TORTURING_CHIEF // DO NOT MODIFY!
  bsA = 1;
  bsB = TORTURING_CHIEF;
  if(bsA!=bsB)
    ssErrors << "Defined TORTURING_CHIEF with value 1 from .dat file mismatches hardcoded c++ define value of " << TORTURING_CHIEF << "!" << std::endl;
#endif 

  
#ifdef TOURIST_GUIDE_TEAM // DO NOT MODIFY!
  bsA = 9;
  bsB = TOURIST_GUIDE_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined TOURIST_GUIDE_TEAM with value 9 from .dat file mismatches hardcoded c++ define value of " << TOURIST_GUIDE_TEAM << "!" << std::endl;
#endif 

  
#ifdef TOURIST_TEAM // DO NOT MODIFY!
  bsA = 10;
  bsB = TOURIST_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined TOURIST_TEAM with value 10 from .dat file mismatches hardcoded c++ define value of " << TOURIST_TEAM << "!" << std::endl;
#endif 

  
#ifdef TOURMALINE // DO NOT MODIFY!
  bsA = 4269;
  bsB = TOURMALINE;
  if(bsA!=bsB)
    ssErrors << "Defined TOURMALINE with value 4269 from .dat file mismatches hardcoded c++ define value of " << TOURMALINE << "!" << std::endl;
#endif 

  
#ifdef TRAINEE // DO NOT MODIFY!
  bsA = 13;
  bsB = TRAINEE;
  if(bsA!=bsB)
    ssErrors << "Defined TRAINEE with value 13 from .dat file mismatches hardcoded c++ define value of " << TRAINEE << "!" << std::endl;
#endif 

  
#ifdef TRANSPARENT_COLOR // DO NOT MODIFY!
  bsA = 63519;
  bsB = TRANSPARENT_COLOR;
  if(bsA!=bsB)
    ssErrors << "Defined TRANSPARENT_COLOR with value 63519 from .dat file mismatches hardcoded c++ define value of " << TRANSPARENT_COLOR << "!" << std::endl;
#endif 

  
#ifdef TRANSSEXUAL // DO NOT MODIFY!
  bsA = 3;
  bsB = TRANSSEXUAL;
  if(bsA!=bsB)
    ssErrors << "Defined TRANSSEXUAL with value 3 from .dat file mismatches hardcoded c++ define value of " << TRANSSEXUAL << "!" << std::endl;
#endif 

  
#ifdef TRIANGULAR_LOCK // DO NOT MODIFY!
  bsA = 4096;
  bsB = TRIANGULAR_LOCK;
  if(bsA!=bsB)
    ssErrors << "Defined TRIANGULAR_LOCK with value 4096 from .dat file mismatches hardcoded c++ define value of " << TRIANGULAR_LOCK << "!" << std::endl;
#endif 

  
#ifdef TROLL_BLOOD // DO NOT MODIFY!
  bsA = 16424;
  bsB = TROLL_BLOOD;
  if(bsA!=bsB)
    ssErrors << "Defined TROLL_BLOOD with value 16424 from .dat file mismatches hardcoded c++ define value of " << TROLL_BLOOD << "!" << std::endl;
#endif 

  
#ifdef TROLL_HIDE // DO NOT MODIFY!
  bsA = 4156;
  bsB = TROLL_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined TROLL_HIDE with value 4156 from .dat file mismatches hardcoded c++ define value of " << TROLL_HIDE << "!" << std::endl;
#endif 

  
#ifdef TROLL_TUSK // DO NOT MODIFY!
  bsA = 8217;
  bsB = TROLL_TUSK;
  if(bsA!=bsB)
    ssErrors << "Defined TROLL_TUSK with value 8217 from .dat file mismatches hardcoded c++ define value of " << TROLL_TUSK << "!" << std::endl;
#endif 

  
#ifdef TROLL_WOOL // DO NOT MODIFY!
  bsA = 4136;
  bsB = TROLL_WOOL;
  if(bsA!=bsB)
    ssErrors << "Defined TROLL_WOOL with value 4136 from .dat file mismatches hardcoded c++ define value of " << TROLL_WOOL << "!" << std::endl;
#endif 

  
#ifdef TUNA // DO NOT MODIFY!
  bsA = 20486;
  bsB = TUNA;
  if(bsA!=bsB)
    ssErrors << "Defined TUNA with value 20486 from .dat file mismatches hardcoded c++ define value of " << TUNA << "!" << std::endl;
#endif 

  
#ifdef TUNDRA // DO NOT MODIFY!
  bsA = 6;
  bsB = TUNDRA;
  if(bsA!=bsB)
    ssErrors << "Defined TUNDRA with value 6 from .dat file mismatches hardcoded c++ define value of " << TUNDRA << "!" << std::endl;
#endif 

  
#ifdef TUNGSTEN // DO NOT MODIFY!
  bsA = 4123;
  bsB = TUNGSTEN;
  if(bsA!=bsB)
    ssErrors << "Defined TUNGSTEN with value 4123 from .dat file mismatches hardcoded c++ define value of " << TUNGSTEN << "!" << std::endl;
#endif 

  
#ifdef TURTLE_SHELL // DO NOT MODIFY!
  bsA = 4253;
  bsB = TURTLE_SHELL;
  if(bsA!=bsB)
    ssErrors << "Defined TURTLE_SHELL with value 4253 from .dat file mismatches hardcoded c++ define value of " << TURTLE_SHELL << "!" << std::endl;
#endif 

  
#ifdef TWO_HANDED_SCIMITAR // DO NOT MODIFY!
  bsA = 3;
  bsB = TWO_HANDED_SCIMITAR;
  if(bsA!=bsB)
    ssErrors << "Defined TWO_HANDED_SCIMITAR with value 3 from .dat file mismatches hardcoded c++ define value of " << TWO_HANDED_SCIMITAR << "!" << std::endl;
#endif 

  
#ifdef TWO_HANDED_SWORD // DO NOT MODIFY!
  bsA = 2;
  bsB = TWO_HANDED_SWORD;
  if(bsA!=bsB)
    ssErrors << "Defined TWO_HANDED_SWORD with value 2 from .dat file mismatches hardcoded c++ define value of " << TWO_HANDED_SWORD << "!" << std::endl;
#endif 

  
#ifdef UKKU_STEEL // DO NOT MODIFY!
  bsA = 28678;
  bsB = UKKU_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined UKKU_STEEL with value 28678 from .dat file mismatches hardcoded c++ define value of " << UKKU_STEEL << "!" << std::endl;
#endif 

  
#ifdef UNARMED // DO NOT MODIFY!
  bsA = 0;
  bsB = UNARMED;
  if(bsA!=bsB)
    ssErrors << "Defined UNARMED with value 0 from .dat file mismatches hardcoded c++ define value of " << UNARMED << "!" << std::endl;
#endif 

  
#ifdef UNARMED_ATTACK // DO NOT MODIFY!
  bsA = 0;
  bsB = UNARMED_ATTACK;
  if(bsA!=bsB)
    ssErrors << "Defined UNARMED_ATTACK with value 0 from .dat file mismatches hardcoded c++ define value of " << UNARMED_ATTACK << "!" << std::endl;
#endif 

  
#ifdef UNARTICLED // DO NOT MODIFY!
  bsA = 0;
  bsB = UNARTICLED;
  if(bsA!=bsB)
    ssErrors << "Defined UNARTICLED with value 0 from .dat file mismatches hardcoded c++ define value of " << UNARTICLED << "!" << std::endl;
#endif 

  
#ifdef UNCARING // DO NOT MODIFY!
  bsA = 2;
  bsB = UNCARING;
  if(bsA!=bsB)
    ssErrors << "Defined UNCARING with value 2 from .dat file mismatches hardcoded c++ define value of " << UNCARING << "!" << std::endl;
#endif 

  
#ifdef UNCATEGORIZED // DO NOT MODIFY!
  bsA = 3;
  bsB = UNCATEGORIZED;
  if(bsA!=bsB)
    ssErrors << "Defined UNCATEGORIZED with value 3 from .dat file mismatches hardcoded c++ define value of " << UNCATEGORIZED << "!" << std::endl;
#endif 

  
#ifdef UNDEFINED // DO NOT MODIFY!
  bsA = 0;
  bsB = UNDEFINED;
  if(bsA!=bsB)
    ssErrors << "Defined UNDEFINED with value 0 from .dat file mismatches hardcoded c++ define value of " << UNDEFINED << "!" << std::endl;
#endif 

  
#ifdef UNDERGROUND_LAKE // DO NOT MODIFY!
  bsA = 2;
  bsB = UNDERGROUND_LAKE;
  if(bsA!=bsB)
    ssErrors << "Defined UNDERGROUND_LAKE with value 2 from .dat file mismatches hardcoded c++ define value of " << UNDERGROUND_LAKE << "!" << std::endl;
#endif 

  
#ifdef UNDER_WATER_TUNNEL // DO NOT MODIFY!
  bsA = 4;
  bsB = UNDER_WATER_TUNNEL;
  if(bsA!=bsB)
    ssErrors << "Defined UNDER_WATER_TUNNEL with value 4 from .dat file mismatches hardcoded c++ define value of " << UNDER_WATER_TUNNEL << "!" << std::endl;
#endif 

  
#ifdef UNDER_WATER_TUNNEL_EXIT // DO NOT MODIFY!
  bsA = 128;
  bsB = UNDER_WATER_TUNNEL_EXIT;
  if(bsA!=bsB)
    ssErrors << "Defined UNDER_WATER_TUNNEL_EXIT with value 128 from .dat file mismatches hardcoded c++ define value of " << UNDER_WATER_TUNNEL_EXIT << "!" << std::endl;
#endif 

  
#ifdef UNICORN_HORN // DO NOT MODIFY!
  bsA = 4252;
  bsB = UNICORN_HORN;
  if(bsA!=bsB)
    ssErrors << "Defined UNICORN_HORN with value 4252 from .dat file mismatches hardcoded c++ define value of " << UNICORN_HORN << "!" << std::endl;
#endif 

  
#ifdef UP // DO NOT MODIFY!
  bsA = 2;
  bsB = UP;
  if(bsA!=bsB)
    ssErrors << "Defined UP with value 2 from .dat file mismatches hardcoded c++ define value of " << UP << "!" << std::endl;
#endif 

  
#ifdef URANIUM // DO NOT MODIFY!
  bsA = 4198;
  bsB = URANIUM;
  if(bsA!=bsB)
    ssErrors << "Defined URANIUM with value 4198 from .dat file mismatches hardcoded c++ define value of " << URANIUM << "!" << std::endl;
#endif 

  
#ifdef UR_STEEL // DO NOT MODIFY!
  bsA = 28685;
  bsB = UR_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined UR_STEEL with value 28685 from .dat file mismatches hardcoded c++ define value of " << UR_STEEL << "!" << std::endl;
#endif 

  
#ifdef USE_ADJECTIVE_AN // DO NOT MODIFY!
  bsA = 2;
  bsB = USE_ADJECTIVE_AN;
  if(bsA!=bsB)
    ssErrors << "Defined USE_ADJECTIVE_AN with value 2 from .dat file mismatches hardcoded c++ define value of " << USE_ADJECTIVE_AN << "!" << std::endl;
#endif 

  
#ifdef USE_AN // DO NOT MODIFY!
  bsA = 1;
  bsB = USE_AN;
  if(bsA!=bsB)
    ssErrors << "Defined USE_AN with value 1 from .dat file mismatches hardcoded c++ define value of " << USE_AN << "!" << std::endl;
#endif 

  
#ifdef USE_ARMS // DO NOT MODIFY!
  bsA = 1;
  bsB = USE_ARMS;
  if(bsA!=bsB)
    ssErrors << "Defined USE_ARMS with value 1 from .dat file mismatches hardcoded c++ define value of " << USE_ARMS << "!" << std::endl;
#endif 

  
#ifdef USE_HEAD // DO NOT MODIFY!
  bsA = 4;
  bsB = USE_HEAD;
  if(bsA!=bsB)
    ssErrors << "Defined USE_HEAD with value 4 from .dat file mismatches hardcoded c++ define value of " << USE_HEAD << "!" << std::endl;
#endif 

  
#ifdef USE_LEGS // DO NOT MODIFY!
  bsA = 2;
  bsB = USE_LEGS;
  if(bsA!=bsB)
    ssErrors << "Defined USE_LEGS with value 2 from .dat file mismatches hardcoded c++ define value of " << USE_LEGS << "!" << std::endl;
#endif 

  
#ifdef USE_MATERIAL_ATTRIBUTES // DO NOT MODIFY!
  bsA = 8;
  bsB = USE_MATERIAL_ATTRIBUTES;
  if(bsA!=bsB)
    ssErrors << "Defined USE_MATERIAL_ATTRIBUTES with value 8 from .dat file mismatches hardcoded c++ define value of " << USE_MATERIAL_ATTRIBUTES << "!" << std::endl;
#endif 

  
#ifdef VACUUM // DO NOT MODIFY!
  bsA = 12290;
  bsB = VACUUM;
  if(bsA!=bsB)
    ssErrors << "Defined VACUUM with value 12290 from .dat file mismatches hardcoded c++ define value of " << VACUUM << "!" << std::endl;
#endif 

  
#ifdef VALDEMAR // DO NOT MODIFY!
  bsA = 16399;
  bsB = VALDEMAR;
  if(bsA!=bsB)
    ssErrors << "Defined VALDEMAR with value 16399 from .dat file mismatches hardcoded c++ define value of " << VALDEMAR << "!" << std::endl;
#endif 

  
#ifdef VALPURIUM // DO NOT MODIFY!
  bsA = 4097;
  bsB = VALPURIUM;
  if(bsA!=bsB)
    ssErrors << "Defined VALPURIUM with value 4097 from .dat file mismatches hardcoded c++ define value of " << VALPURIUM << "!" << std::endl;
#endif 

  
#ifdef VALPURUS // DO NOT MODIFY!
  bsA = 1;
  bsB = VALPURUS;
  if(bsA!=bsB)
    ssErrors << "Defined VALPURUS with value 1 from .dat file mismatches hardcoded c++ define value of " << VALPURUS << "!" << std::endl;
#endif 

  
#ifdef VALUABLE // DO NOT MODIFY!
  bsA = 65536;
  bsB = VALUABLE;
  if(bsA!=bsB)
    ssErrors << "Defined VALUABLE with value 65536 from .dat file mismatches hardcoded c++ define value of " << VALUABLE << "!" << std::endl;
#endif 

  
#ifdef VAMPIRE_FLESH // DO NOT MODIFY!
  bsA = 20531;
  bsB = VAMPIRE_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined VAMPIRE_FLESH with value 20531 from .dat file mismatches hardcoded c++ define value of " << VAMPIRE_FLESH << "!" << std::endl;
#endif 

  
#ifdef VAMPIRISM // DO NOT MODIFY!
  bsA = 4194304;
  bsB = VAMPIRISM;
  if(bsA!=bsB)
    ssErrors << "Defined VAMPIRISM with value 4194304 from .dat file mismatches hardcoded c++ define value of " << VAMPIRISM << "!" << std::endl;
#endif 

  
#ifdef VELLUM // DO NOT MODIFY!
  bsA = 4224;
  bsB = VELLUM;
  if(bsA!=bsB)
    ssErrors << "Defined VELLUM with value 4224 from .dat file mismatches hardcoded c++ define value of " << VELLUM << "!" << std::endl;
#endif 

  
#ifdef VENISON // DO NOT MODIFY!
  bsA = 20483;
  bsB = VENISON;
  if(bsA!=bsB)
    ssErrors << "Defined VENISON with value 20483 from .dat file mismatches hardcoded c++ define value of " << VENISON << "!" << std::endl;
#endif 

  
#ifdef VERDIGRIS // DO NOT MODIFY!
  bsA = 4101;
  bsB = VERDIGRIS;
  if(bsA!=bsB)
    ssErrors << "Defined VERDIGRIS with value 4101 from .dat file mismatches hardcoded c++ define value of " << VERDIGRIS << "!" << std::endl;
#endif 

  
#ifdef VERY_RUSTED // DO NOT MODIFY!
  bsA = 3;
  bsB = VERY_RUSTED;
  if(bsA!=bsB)
    ssErrors << "Defined VERY_RUSTED with value 3 from .dat file mismatches hardcoded c++ define value of " << VERY_RUSTED << "!" << std::endl;
#endif 

  
#ifdef VESANA_LEVEL // DO NOT MODIFY!
  bsA = 2;
  bsB = VESANA_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined VESANA_LEVEL with value 2 from .dat file mismatches hardcoded c++ define value of " << VESANA_LEVEL << "!" << std::endl;
#endif 

  
#ifdef VETERAN // DO NOT MODIFY!
  bsA = 2;
  bsB = VETERAN;
  if(bsA!=bsB)
    ssErrors << "Defined VETERAN with value 2 from .dat file mismatches hardcoded c++ define value of " << VETERAN << "!" << std::endl;
#endif 

  
#ifdef VETERAN_FEMALE // DO NOT MODIFY!
  bsA = 15;
  bsB = VETERAN_FEMALE;
  if(bsA!=bsB)
    ssErrors << "Defined VETERAN_FEMALE with value 15 from .dat file mismatches hardcoded c++ define value of " << VETERAN_FEMALE << "!" << std::endl;
#endif 

  
#ifdef VIAL // DO NOT MODIFY!
  bsA = 1;
  bsB = VIAL;
  if(bsA!=bsB)
    ssErrors << "Defined VIAL with value 1 from .dat file mismatches hardcoded c++ define value of " << VIAL << "!" << std::endl;
#endif 

  
#ifdef VICE_ROY // DO NOT MODIFY!
  bsA = 1;
  bsB = VICE_ROY;
  if(bsA!=bsB)
    ssErrors << "Defined VICE_ROY with value 1 from .dat file mismatches hardcoded c++ define value of " << VICE_ROY << "!" << std::endl;
#endif 

  
#ifdef VINEGAR // DO NOT MODIFY!
  bsA = 16440;
  bsB = VINEGAR;
  if(bsA!=bsB)
    ssErrors << "Defined VINEGAR with value 16440 from .dat file mismatches hardcoded c++ define value of " << VINEGAR << "!" << std::endl;
#endif 

  
#ifdef VITRELLOY // DO NOT MODIFY!
  bsA = 4191;
  bsB = VITRELLOY;
  if(bsA!=bsB)
    ssErrors << "Defined VITRELLOY with value 4191 from .dat file mismatches hardcoded c++ define value of " << VITRELLOY << "!" << std::endl;
#endif 

  
#ifdef VODKA // DO NOT MODIFY!
  bsA = 16400;
  bsB = VODKA;
  if(bsA!=bsB)
    ssErrors << "Defined VODKA with value 16400 from .dat file mismatches hardcoded c++ define value of " << VODKA << "!" << std::endl;
#endif 

  
#ifdef VOMIT // DO NOT MODIFY!
  bsA = 16416;
  bsB = VOMIT;
  if(bsA!=bsB)
    ssErrors << "Defined VOMIT with value 16416 from .dat file mismatches hardcoded c++ define value of " << VOMIT << "!" << std::endl;
#endif 

  
#ifdef WALK // DO NOT MODIFY!
  bsA = 1;
  bsB = WALK;
  if(bsA!=bsB)
    ssErrors << "Defined WALK with value 1 from .dat file mismatches hardcoded c++ define value of " << WALK << "!" << std::endl;
#endif 

  
#ifdef WAND // DO NOT MODIFY!
  bsA = 16384;
  bsB = WAND;
  if(bsA!=bsB)
    ssErrors << "Defined WAND with value 16384 from .dat file mismatches hardcoded c++ define value of " << WAND << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_ACID_RAIN // DO NOT MODIFY!
  bsA = 12;
  bsB = WAND_OF_ACID_RAIN;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_ACID_RAIN with value 12 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_ACID_RAIN << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_ALCHEMY // DO NOT MODIFY!
  bsA = 16;
  bsB = WAND_OF_ALCHEMY;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_ALCHEMY with value 16 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_ALCHEMY << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_CLONING // DO NOT MODIFY!
  bsA = 10;
  bsB = WAND_OF_CLONING;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_CLONING with value 10 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_CLONING << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_DOOR_CREATION // DO NOT MODIFY!
  bsA = 8;
  bsB = WAND_OF_DOOR_CREATION;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_DOOR_CREATION with value 8 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_DOOR_CREATION << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_FIRE_BALLS // DO NOT MODIFY!
  bsA = 3;
  bsB = WAND_OF_FIRE_BALLS;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_FIRE_BALLS with value 3 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_FIRE_BALLS << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_HASTE // DO NOT MODIFY!
  bsA = 5;
  bsB = WAND_OF_HASTE;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_HASTE with value 5 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_HASTE << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_INVISIBILITY // DO NOT MODIFY!
  bsA = 9;
  bsB = WAND_OF_INVISIBILITY;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_INVISIBILITY with value 9 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_INVISIBILITY << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_LIGHTNING // DO NOT MODIFY!
  bsA = 11;
  bsB = WAND_OF_LIGHTNING;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_LIGHTNING with value 11 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_LIGHTNING << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_MIRRORING // DO NOT MODIFY!
  bsA = 13;
  bsB = WAND_OF_MIRRORING;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_MIRRORING with value 13 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_MIRRORING << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_NECROMANCY // DO NOT MODIFY!
  bsA = 14;
  bsB = WAND_OF_NECROMANCY;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_NECROMANCY with value 14 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_NECROMANCY << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_POLYMORPH // DO NOT MODIFY!
  bsA = 1;
  bsB = WAND_OF_POLYMORPH;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_POLYMORPH with value 1 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_POLYMORPH << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_RESURRECTION // DO NOT MODIFY!
  bsA = 7;
  bsB = WAND_OF_RESURRECTION;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_RESURRECTION with value 7 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_RESURRECTION << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_SLOW // DO NOT MODIFY!
  bsA = 6;
  bsB = WAND_OF_SLOW;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_SLOW with value 6 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_SLOW << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_SOFTEN_MATERIAL // DO NOT MODIFY!
  bsA = 17;
  bsB = WAND_OF_SOFTEN_MATERIAL;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_SOFTEN_MATERIAL with value 17 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_SOFTEN_MATERIAL << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_STRIKING // DO NOT MODIFY!
  bsA = 2;
  bsB = WAND_OF_STRIKING;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_STRIKING with value 2 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_STRIKING << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_TELEPORTATION // DO NOT MODIFY!
  bsA = 4;
  bsB = WAND_OF_TELEPORTATION;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_TELEPORTATION with value 4 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_TELEPORTATION << "!" << std::endl;
#endif 

  
#ifdef WAND_OF_WEBBING // DO NOT MODIFY!
  bsA = 15;
  bsB = WAND_OF_WEBBING;
  if(bsA!=bsB)
    ssErrors << "Defined WAND_OF_WEBBING with value 15 from .dat file mismatches hardcoded c++ define value of " << WAND_OF_WEBBING << "!" << std::endl;
#endif 

  
#ifdef WARLOCK // DO NOT MODIFY!
  bsA = 6;
  bsB = WARLOCK;
  if(bsA!=bsB)
    ssErrors << "Defined WARLOCK with value 6 from .dat file mismatches hardcoded c++ define value of " << WARLOCK << "!" << std::endl;
#endif 

  
#ifdef WARRIOR // DO NOT MODIFY!
  bsA = 1;
  bsB = WARRIOR;
  if(bsA!=bsB)
    ssErrors << "Defined WARRIOR with value 1 from .dat file mismatches hardcoded c++ define value of " << WARRIOR << "!" << std::endl;
#endif 

  
#ifdef WAR_HAMMER // DO NOT MODIFY!
  bsA = 8;
  bsB = WAR_HAMMER;
  if(bsA!=bsB)
    ssErrors << "Defined WAR_HAMMER with value 8 from .dat file mismatches hardcoded c++ define value of " << WAR_HAMMER << "!" << std::endl;
#endif 

  
#ifdef WAR_LADY // DO NOT MODIFY!
  bsA = 3;
  bsB = WAR_LADY;
  if(bsA!=bsB)
    ssErrors << "Defined WAR_LADY with value 3 from .dat file mismatches hardcoded c++ define value of " << WAR_LADY << "!" << std::endl;
#endif 

  
#ifdef WAR_LORD // DO NOT MODIFY!
  bsA = 2;
  bsB = WAR_LORD;
  if(bsA!=bsB)
    ssErrors << "Defined WAR_LORD with value 2 from .dat file mismatches hardcoded c++ define value of " << WAR_LORD << "!" << std::endl;
#endif 

  
#ifdef WAR_MASK // DO NOT MODIFY!
  bsA = 13;
  bsB = WAR_MASK;
  if(bsA!=bsB)
    ssErrors << "Defined WAR_MASK with value 13 from .dat file mismatches hardcoded c++ define value of " << WAR_MASK << "!" << std::endl;
#endif 

  
#ifdef WATER // DO NOT MODIFY!
  bsA = 16398;
  bsB = WATER;
  if(bsA!=bsB)
    ssErrors << "Defined WATER with value 16398 from .dat file mismatches hardcoded c++ define value of " << WATER << "!" << std::endl;
#endif 

  
#ifdef WAX // DO NOT MODIFY!
  bsA = 4256;
  bsB = WAX;
  if(bsA!=bsB)
    ssErrors << "Defined WAX with value 4256 from .dat file mismatches hardcoded c++ define value of " << WAX << "!" << std::endl;
#endif 

  
#ifdef WEAPON // DO NOT MODIFY!
  bsA = 16;
  bsB = WEAPON;
  if(bsA!=bsB)
    ssErrors << "Defined WEAPON with value 16 from .dat file mismatches hardcoded c++ define value of " << WEAPON << "!" << std::endl;
#endif 

  
#ifdef WEAPON_ATTACK // DO NOT MODIFY!
  bsA = 1;
  bsB = WEAPON_ATTACK;
  if(bsA!=bsB)
    ssErrors << "Defined WEAPON_ATTACK with value 1 from .dat file mismatches hardcoded c++ define value of " << WEAPON_ATTACK << "!" << std::endl;
#endif 

  
#ifdef WEAPON_SKILL_CATEGORIES // DO NOT MODIFY!
  bsA = 11;
  bsB = WEAPON_SKILL_CATEGORIES;
  if(bsA!=bsB)
    ssErrors << "Defined WEAPON_SKILL_CATEGORIES with value 11 from .dat file mismatches hardcoded c++ define value of " << WEAPON_SKILL_CATEGORIES << "!" << std::endl;
#endif 

  
#ifdef WELL // DO NOT MODIFY!
  bsA = 34;
  bsB = WELL;
  if(bsA!=bsB)
    ssErrors << "Defined WELL with value 34 from .dat file mismatches hardcoded c++ define value of " << WELL << "!" << std::endl;
#endif 

  
#ifdef WERE_WOLF_FLESH // DO NOT MODIFY!
  bsA = 20501;
  bsB = WERE_WOLF_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined WERE_WOLF_FLESH with value 20501 from .dat file mismatches hardcoded c++ define value of " << WERE_WOLF_FLESH << "!" << std::endl;
#endif 

  
#ifdef WHALE_BONE // DO NOT MODIFY!
  bsA = 8213;
  bsB = WHALE_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined WHALE_BONE with value 8213 from .dat file mismatches hardcoded c++ define value of " << WHALE_BONE << "!" << std::endl;
#endif 

  
#ifdef WHIPS // DO NOT MODIFY!
  bsA = 9;
  bsB = WHIPS;
  if(bsA!=bsB)
    ssErrors << "Defined WHIPS with value 9 from .dat file mismatches hardcoded c++ define value of " << WHIPS << "!" << std::endl;
#endif 

  
#ifdef WHIP_CHAMPION // DO NOT MODIFY!
  bsA = 2;
  bsB = WHIP_CHAMPION;
  if(bsA!=bsB)
    ssErrors << "Defined WHIP_CHAMPION with value 2 from .dat file mismatches hardcoded c++ define value of " << WHIP_CHAMPION << "!" << std::endl;
#endif 

  
#ifdef WHITE // DO NOT MODIFY!
  bsA = 65535;
  bsB = WHITE;
  if(bsA!=bsB)
    ssErrors << "Defined WHITE with value 65535 from .dat file mismatches hardcoded c++ define value of " << WHITE << "!" << std::endl;
#endif 

  
#ifdef WHITE_JADE // DO NOT MODIFY!
  bsA = 4307;
  bsB = WHITE_JADE;
  if(bsA!=bsB)
    ssErrors << "Defined WHITE_JADE with value 4307 from .dat file mismatches hardcoded c++ define value of " << WHITE_JADE << "!" << std::endl;
#endif 

  
#ifdef WHITE_STEEL // DO NOT MODIFY!
  bsA = 28684;
  bsB = WHITE_STEEL;
  if(bsA!=bsB)
    ssErrors << "Defined WHITE_STEEL with value 28684 from .dat file mismatches hardcoded c++ define value of " << WHITE_STEEL << "!" << std::endl;
#endif 

  
#ifdef WHITE_UNICORN_FLESH // DO NOT MODIFY!
  bsA = 20512;
  bsB = WHITE_UNICORN_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined WHITE_UNICORN_FLESH with value 20512 from .dat file mismatches hardcoded c++ define value of " << WHITE_UNICORN_FLESH << "!" << std::endl;
#endif 

  
#ifdef WHITE_WINE // DO NOT MODIFY!
  bsA = 16402;
  bsB = WHITE_WINE;
  if(bsA!=bsB)
    ssErrors << "Defined WHITE_WINE with value 16402 from .dat file mismatches hardcoded c++ define value of " << WHITE_WINE << "!" << std::endl;
#endif 

  
#ifdef WIFE // DO NOT MODIFY!
  bsA = 2;
  bsB = WIFE;
  if(bsA!=bsB)
    ssErrors << "Defined WIFE with value 2 from .dat file mismatches hardcoded c++ define value of " << WIFE << "!" << std::endl;
#endif 

  
#ifdef WILL_POWER // DO NOT MODIFY!
  bsA = 4;
  bsB = WILL_POWER;
  if(bsA!=bsB)
    ssErrors << "Defined WILL_POWER with value 4 from .dat file mismatches hardcoded c++ define value of " << WILL_POWER << "!" << std::endl;
#endif 

  
#ifdef WINDOW // DO NOT MODIFY!
  bsA = 1024;
  bsB = WINDOW;
  if(bsA!=bsB)
    ssErrors << "Defined WINDOW with value 1024 from .dat file mismatches hardcoded c++ define value of " << WINDOW << "!" << std::endl;
#endif 

  
#ifdef WISDOM // DO NOT MODIFY!
  bsA = 3;
  bsB = WISDOM;
  if(bsA!=bsB)
    ssErrors << "Defined WISDOM with value 3 from .dat file mismatches hardcoded c++ define value of " << WISDOM << "!" << std::endl;
#endif 

  
#ifdef WITCH_BARK // DO NOT MODIFY!
  bsA = 4218;
  bsB = WITCH_BARK;
  if(bsA!=bsB)
    ssErrors << "Defined WITCH_BARK with value 4218 from .dat file mismatches hardcoded c++ define value of " << WITCH_BARK << "!" << std::endl;
#endif 

  
#ifdef WOLF_FLESH // DO NOT MODIFY!
  bsA = 20494;
  bsB = WOLF_FLESH;
  if(bsA!=bsB)
    ssErrors << "Defined WOLF_FLESH with value 20494 from .dat file mismatches hardcoded c++ define value of " << WOLF_FLESH << "!" << std::endl;
#endif 

  
#ifdef WOLF_SKIN // DO NOT MODIFY!
  bsA = 4142;
  bsB = WOLF_SKIN;
  if(bsA!=bsB)
    ssErrors << "Defined WOLF_SKIN with value 4142 from .dat file mismatches hardcoded c++ define value of " << WOLF_SKIN << "!" << std::endl;
#endif 

  
#ifdef WOODEN_HORSE // DO NOT MODIFY!
  bsA = 29;
  bsB = WOODEN_HORSE;
  if(bsA!=bsB)
    ssErrors << "Defined WOODEN_HORSE with value 29 from .dat file mismatches hardcoded c++ define value of " << WOODEN_HORSE << "!" << std::endl;
#endif 

  
#ifdef WOOL_CLOTH // DO NOT MODIFY!
  bsA = 4130;
  bsB = WOOL_CLOTH;
  if(bsA!=bsB)
    ssErrors << "Defined WOOL_CLOTH with value 4130 from .dat file mismatches hardcoded c++ define value of " << WOOL_CLOTH << "!" << std::endl;
#endif 

  
#ifdef WORK_BENCH // DO NOT MODIFY!
  bsA = 32;
  bsB = WORK_BENCH;
  if(bsA!=bsB)
    ssErrors << "Defined WORK_BENCH with value 32 from .dat file mismatches hardcoded c++ define value of " << WORK_BENCH << "!" << std::endl;
#endif 

  
#ifdef WORLD_MAP // DO NOT MODIFY!
  bsA = 255;
  bsB = WORLD_MAP;
  if(bsA!=bsB)
    ssErrors << "Defined WORLD_MAP with value 255 from .dat file mismatches hardcoded c++ define value of " << WORLD_MAP << "!" << std::endl;
#endif 

  
#ifdef WRAITH_BONE // DO NOT MODIFY!
  bsA = 8214;
  bsB = WRAITH_BONE;
  if(bsA!=bsB)
    ssErrors << "Defined WRAITH_BONE with value 8214 from .dat file mismatches hardcoded c++ define value of " << WRAITH_BONE << "!" << std::endl;
#endif 

  
#ifdef WYVERN_HIDE // DO NOT MODIFY!
  bsA = 4148;
  bsB = WYVERN_HIDE;
  if(bsA!=bsB)
    ssErrors << "Defined WYVERN_HIDE with value 4148 from .dat file mismatches hardcoded c++ define value of " << WYVERN_HIDE << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB // DO NOT MODIFY!
  bsA = 6;
  bsB = XINROCH_TOMB;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB with value 6 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB_ENTRANCE // DO NOT MODIFY!
  bsA = 900;
  bsB = XINROCH_TOMB_ENTRANCE;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB_ENTRANCE with value 900 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB_ENTRANCE << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB_ENTRY_TEAM // DO NOT MODIFY!
  bsA = 12;
  bsB = XINROCH_TOMB_ENTRY_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB_ENTRY_TEAM with value 12 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB_ENTRY_TEAM << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB_EXIT // DO NOT MODIFY!
  bsA = 1000;
  bsB = XINROCH_TOMB_EXIT;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB_EXIT with value 1000 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB_EXIT << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB_KAMIKAZE_DWARF_TEAM // DO NOT MODIFY!
  bsA = 14;
  bsB = XINROCH_TOMB_KAMIKAZE_DWARF_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB_KAMIKAZE_DWARF_TEAM with value 14 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB_KAMIKAZE_DWARF_TEAM << "!" << std::endl;
#endif 

  
#ifdef XINROCH_TOMB_NECRO_TEAM // DO NOT MODIFY!
  bsA = 13;
  bsB = XINROCH_TOMB_NECRO_TEAM;
  if(bsA!=bsB)
    ssErrors << "Defined XINROCH_TOMB_NECRO_TEAM with value 13 from .dat file mismatches hardcoded c++ define value of " << XINROCH_TOMB_NECRO_TEAM << "!" << std::endl;
#endif 

  
#ifdef YELLOW // DO NOT MODIFY!
  bsA = 65504;
  bsB = YELLOW;
  if(bsA!=bsB)
    ssErrors << "Defined YELLOW with value 65504 from .dat file mismatches hardcoded c++ define value of " << YELLOW << "!" << std::endl;
#endif 

  
#ifdef YELLOW_FRACTAL // DO NOT MODIFY!
  bsA = 4;
  bsB = YELLOW_FRACTAL;
  if(bsA!=bsB)
    ssErrors << "Defined YELLOW_FRACTAL with value 4 from .dat file mismatches hardcoded c++ define value of " << YELLOW_FRACTAL << "!" << std::endl;
#endif 

  
#ifdef YEW_WOOD // DO NOT MODIFY!
  bsA = 4233;
  bsB = YEW_WOOD;
  if(bsA!=bsB)
    ssErrors << "Defined YEW_WOOD with value 4233 from .dat file mismatches hardcoded c++ define value of " << YEW_WOOD << "!" << std::endl;
#endif 

  
#ifdef ZOMBIE_LEVEL // DO NOT MODIFY!
  bsA = 5;
  bsB = ZOMBIE_LEVEL;
  if(bsA!=bsB)
    ssErrors << "Defined ZOMBIE_LEVEL with value 5 from .dat file mismatches hardcoded c++ define value of " << ZOMBIE_LEVEL << "!" << std::endl;
#endif 

  
#ifdef ZOMBIE_OF_KHAZ_ZADM // DO NOT MODIFY!
  bsA = 1;
  bsB = ZOMBIE_OF_KHAZ_ZADM;
  if(bsA!=bsB)
    ssErrors << "Defined ZOMBIE_OF_KHAZ_ZADM with value 1 from .dat file mismatches hardcoded c++ define value of " << ZOMBIE_OF_KHAZ_ZADM << "!" << std::endl;
#endif 


  if(ssErrors.str().length() > 0) ABORT(ssErrors.str().c_str());

}};

#endif // _DEFINESVALIDATOR_H_
