#!/bin/bash

##############
#### sed commands also restores CR for windows users
##############
sedB4EndCR='s@.*@&\r@'
sedGet1stFileName='s"([^.]*)[.]wav.*"\1"'

IFS=$'\n' read -d '' -r -a astrList < <(cat config.txt |tr -d '\r'); #removes CR too
for((i=0;i<${#astrList[@]};i++));do 
	strLine="${astrList[i]}";
	
	if((i%2==0));then 
    strRegex="$strLine";
	else
    strFiles="`echo "$strLine" |tr ' ' ',' |sed 's",", "g'`"
    strDesc="`echo "$strFiles" |sed -r "$sedGet1stFileName"`"
		echo "$strDesc; $strFiles;$strRegex"; # the sound files is the 2nd line
	fi;
done \
	|sort \
	|sed -r "$sedB4EndCR" \
	>SoundEffects.cfg

if((`cat config.txt |wc -l` != (`cat SoundEffects.cfg |wc -l`*2) ));then
	echo "SOME PROBLEM HAPPENED!"
fi

# prettyfiing readability
sedGetDesc='s"([^;]*);.*"\1"'
IFS=$'\n' read -d '' -r -a astrList < <(cat SoundEffects.cfg |tr -d '\r'); #removes CR too
for((i=0;i<${#astrList[@]};i++));do 
	strLine="${astrList[i]}";
  strDesc="`echo "$strLine" |sed -r "$sedGetDesc"`"
  
  if [[ "$strDescPrev" != "$strDesc" ]];then
    strFullDesc="### $strDesc ###"
    strDescOutline="`printf "%0${#strFullDesc}d\n" 0 |tr '0' '#'`"
    
    echo "$strDescOutline"
    echo "$strFullDesc"
    echo "$strDescOutline"
  fi
  
  echo "$strLine"
  echo
  
  strDescPrev="$strDesc"
done >SoundEffects.cfg
