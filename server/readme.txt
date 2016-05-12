

要在每一个makefile中修改IntDir=Release

liblog.mak								PREDEF= -DNDEBUG -D_LIB -D_GAME_SERVER
accountserver.mak        	PREDEF= -DNDEBUG -D_CONSOLE
gameserver.mak						PREDEF= -DNDEBUG -D_CONSOLE -D_GAME_SERVER
groupserver.mak						PREDEF= -DNDEBUG -D_CONSOLE
agent.mak									PREDEF= -DNDEBUG -D_CONSOLE

*************************************************************************

LibServerFrame
在VCLibrarianTool增加如下OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="..\Server\LibServerFrame.lib"
/>
	
*************************************************************************		
	
DataInfor	
在VCLibrarianTool下面添加OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="../Server/DataInfor.lib"
	
*************************************************************************		
	
LibLog
在VCLibrarianTool下面添加OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="..\Server\LibLog.lib"
	
*************************************************************************

AccountServer
	
在VCLinkerTool下面添加OutputFile
<Tool
		Name="VCLinkerTool"
		OutputFile="./AccountServer.exe"

如果还不行的话，
在99		
<Configuration
			Name="Release|Win32"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="1"
			>

下面加
<Tool
	Name="VCLinkerTool"
	OutputFile="./AccountServer.exe"
/>


*************************************************************************		
	
GameServer
在VCLinkerTool下面添加OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="./GameServer.exe"
	

*************************************************************************		
	
Agent
在VCLinkerTool下面添加OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="$(OutDir)/Agent.exe"
	
*************************************************************************		
	
GroupServer	
在VCLinkerTool下面添加OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="./GroupServer.exe"

*************************************************************************		
