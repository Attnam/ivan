#!/bin/bash

#TODO these filters should be simpler to be addressed like properties on .dat files or classes...

astrValidToCraft=(
  beartrap
  whistle
)

echo "HELP: update the array astrValidToCraft with allowed to craft classes"
declare -p astrValidToCraft |tr '[' '\n'

if ! ls Main/Source/game.cpp;then
  echo "PROBLEM!!! cd to the root ivan dev path" >&2
  exit 1
fi

astrDenyBaseTypes=( #base types ending with
  holybook
  scroll
  bodypart
  torso
  groin
  head
  arm
  leg
)
strDenyBaseTypes="`echo "${astrDenyBaseTypes[@]}" |tr ' ' '|'`"

astrValidToCraft=(
  armor
  belt
  beartrap
  bodyarmor
  boot
  can
  cauldron
  cloak
  gauntlet
  helmet
  itemcontainer # chests/strongbox
  key
  lump # base crafting material
  materialcontainer # potions
  meleeweapon
  pickaxe
  potion # from other recipe
  shield
  stethoscope
  stick # base crafting material
  stone # base crafting material (as ingots)
  whip
  whistle
)
strValidToCraft="`echo "${astrValidToCraft[@]}" |tr ' ' '|'`"

#strOutFile="`mktemp`"
strOutFile="Main/Source/cmdcraftfilters.cpp"

echo "wait a bit, this may take some time..."

sedAutoCodeReplacer="  if(dynamic_cast<\1*>(it)!=NULL)return false;\n"
sedAutoCode="s'ITEM\(([^,]*),.*'${sedAutoCodeReplacer}'" #the filters

######################################## FILL  THE FILE

echo >"$strOutFile" #cleanup
echo "/* compiled thru cmdcraft.cpp */" >>"$strOutFile"
echo "// you can modify this file directly or preferably update it's generator (linux) script `basename "$0"` TODO convert the generator to c++ code?" >>"$strOutFile"
echo "bool craftcore::MoreCraftDeniedFilters(item* it){" >>"$strOutFile"

echo "  // BASE TYPES"  >>"$strOutFile"
for strDenyBaseType in "${astrDenyBaseTypes[@]}";do
  echo "$strDenyBaseType" |sed -r "s'(.*)'${sedAutoCodeReplacer}'" >>"$strOutFile"
done

echo "  // SPECIFIC TYPES" >>"$strOutFile"
egrep "^ITEM" -wrhI * \
  |egrep -v "${strDenyBaseTypes}\)" \
  |sort \
  |sed -r "$sedAutoCode" \
  |egrep -v "\<(${strValidToCraft})" \
  >>"$strOutFile"
  
echo "  return true;" >>"$strOutFile"
echo "}" >>"$strOutFile"

######################################### OK

#echo "result stored at temporary file:  '$strOutFile'"
cat "$strOutFile"
echo "// THE ABOVE RESULT REPLACED FILE AT '$strOutFile'" >&2
