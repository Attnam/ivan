#!/bin/bash

if [[ ! -L Current ]];then echo "must be run at .devsPrefs folder where is the relative link to the curren usesr: 'Current'!";exit 1;fi

ls -l Current&&:
echo

strUser="`readlink Current`"
strUser="`basename "$strUser"`" #grant is is relativised
declare -p strUser
echo

cd ..
pwd
echo

echo "--------------"
ls -l .gitignore .cproject .project&&:
echo

echo ">>>>>>>>>>>>>>> .cproject"
trash -v .cproject&&:
ln -vsf .devsPrefs/Current/.cproject .cproject
echo

echo ">>>>>>>>>>>>>>> .project"
trash -v .project&&:
ln -vsf .devsPrefs/Current/.project .project
echo

echo ">>>>>>>>>>>>>>> .gitignore"
trash -v .gitignore&&:
ln -vsf ".devsPrefs/Current/.gitignore.$strUser" .gitignore
echo

echo "--------------"
ls -l .gitignore .cproject .project&&:
