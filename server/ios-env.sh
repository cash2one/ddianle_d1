#!/bin/bash

cat libserverframe/LibServerFrame.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt libserverframe/LibServerFrame.vcxproj

cat datainfor/DataInfor.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt datainfor/DataInfor.vcxproj

cat server/GMServer.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt server/GMServer.vcxproj

cat server/AccountServer.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt server/AccountServer.vcxproj

cat server/GameServer.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt server/GameServer.vcxproj

cat server/GroupServer.vcxproj | sed -e "s/%(PreprocessorDefinitions)/SERVER_IOS;%(PreprocessorDefinitions)/g" > sed.out.txt
mv -f sed.out.txt server/GroupServer.vcxproj


rm -rf server/etc
cp -rf server/etc_ios server/etc
