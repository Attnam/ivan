#!/bin/bash
echo "//this script will prepare the full code to determine what commands will enable stretched/scaledup blitting regions"
echo "//just paste the output in the proper source file!"

cd ../../Main/Source

sedCollectDescription='s@[^"]*"([^"]*)".*@\1@'
sedPrepareCode='s@.*@if(strcmp(str,"&")==0)return true;@'

egrep "new command" command.cpp \
  |sed -r \
    -e "$sedCollectDescription" \
    -e "$sedPrepareCode" # already in alphabetic order for user  cmds
