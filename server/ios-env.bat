
type libserverframe\LibServerFrame.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt libserverframe\LibServerFrame.vcxproj

type datainfor\DataInfor.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt datainfor\DataInfor.vcxproj

type server\GMServer.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt server\GMServer.vcxproj

type server\AccountServer.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt server\AccountServer.vcxproj

type server\GameServer.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt server\GameServer.vcxproj

type server\GroupServer.vcxproj | sed -e "s/%%(PreprocessorDefinitions)/SERVER_IOS;%%(PreprocessorDefinitions)/g" > sed.out.txt
move/Y sed.out.txt server\GroupServer.vcxproj


RMDIR/S/Q server\etc
XCOPY /E/Y/Q server\etc_ios server\etc\
