#!/bin/bash

if [[ ! -L Current ]];then echo "must be run at .devsPrefs folder where is the link 'Current'!";exit 1;fi

strUser="`readlink Current`"
declare -p strUser

cd ..
pwd

ls -l .gitignore .cproject .project

ln -vsf .devsPrefs/Current/.cproject .cproject
ln -vsf .devsPrefs/Current/.project .project
ln -vsf ".devsPrefs/Current/.gitignore.$strUser" .gitignore

ls -l .gitignore .cproject .project
