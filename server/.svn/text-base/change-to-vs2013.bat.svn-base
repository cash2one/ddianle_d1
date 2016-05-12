type socket\Socket.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > socket\Socket.vcxproj

type datainfor\DataInfor.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > datainfor\DataInfor.vcxproj

type libserverframe\LibServerFrame.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > libserverframe\LibServerFrame.vcxproj

type server\Agent.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > server\Agent.vcxproj

type server\GMServer.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > server\GMServer.vcxproj

type server\AccountServer.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > server\AccountServer.vcxproj

type server\GameServer.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > server\GameServer.vcxproj

type server\GroupServer.vcxproj | sed -e "s/ToolsVersion=\"4.0\"/ToolsVersion=\"12.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/CharacterSet/a\ \ \ \ <PlatformToolset>v120</PlatformToolset>" > server\GroupServer.vcxproj

type hotdance.sln | sed -e "s/Visual Studio 2010/Visual Studio 2013/g" | sed -e "s/Format Version 11/Format Version 12/g" > sed.out.txt
type sed.out.txt | sed -e "4iVisualStudioVersion = 12.0.21005.1" | sed -e "5iMinimumVisualStudioVersion = 10.0.40219.1" > hotdance.sln

del/F/Q sed.out.txt