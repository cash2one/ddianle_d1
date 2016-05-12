type socket\Socket.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > socket\Socket.vcxproj

type datainfor\DataInfor.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > datainfor\DataInfor.vcxproj

type libserverframe\LibServerFrame.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > libserverframe\LibServerFrame.vcxproj

type server\Agent.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > server\Agent.vcxproj

type server\GMServer.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > server\GMServer.vcxproj

type server\AccountServer.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > server\AccountServer.vcxproj

type server\GameServer.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > server\GameServer.vcxproj

type server\GroupServer.vcxproj | sed -e "s/ToolsVersion=\"12.0\"/ToolsVersion=\"4.0\"/g" > sed.out.txt
type sed.out.txt | sed -e "/PlatformToolset/d" > server\GroupServer.vcxproj

type hotdance.sln | sed -e "s/Visual Studio 2013/Visual Studio 2010/g" | sed -e "s/Format Version 12/Format Version 11/g" > sed.out.txt
type sed.out.txt | sed -e "/VisualStudioVersion/d" > hotdance.sln

del/F/Q sed.out.txt