#!/bin/bash

#~ ls -l config.txt
#~ echo "newline, word, character, byte, maximum line length"
#~ wc -cmlwL config.txt
#~ echo

#~ cat config.txt |tr -d '\r' >config.NoCR.txt
#~ ls -l config.NoCR.txt
#~ wc -cmlwL config.NoCR.txt
#~ echo

##############
#### sed commands also restores CR for windows users
##############
sedMiddleCRNL='s@[\]n@\r\n@'
sedB4EndCR='s@.*@&\r@'

IFS=$'\n' read -d '' -r -a astrList < <(cat config.txt |tr -d '\r'); #removes CR too
#IFS=$'\n' read -d '' -r -a astrList < <(cat config.NoCR.txt); #removes CR too
str2="";
for((i=0;i<${#astrList[@]};i++));do 
	strLine="${astrList[i]}";
	
	str2+="$strLine";
	if((i%2==0));then 
		str2+="\n"; # add NL readable marker
	else
		echo "$str2";
		str2="";
	fi;
done \
	|sort \
	|sed -r \
		-e "$sedB4EndCR" \
		-e "$sedMiddleCRNL" \
	>config.SORTED.txt
	
#~ ls -l config.SORTED.txt
#~ echo "newline, word, character, byte, maximum line length"
#~ wc -cmlwL config.SORTED.txt
#~ echo

#~ if read -p "all seems ok? (hit ctrl+c to stop)" -n 1;then
	#~ mv -v config.SORTED.txt config.txt
	#~ ls -l config*
	#~ echo "newline, word, character, byte, maximum line length"
	#~ wc -cmlwL config.txt
#~ fi

if((`cat config.txt |wc -l`==`cat config.SORTED.txt |wc -l`));then
	mv -v config.SORTED.txt config.txt
fi
