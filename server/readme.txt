

Ҫ��ÿһ��makefile���޸�IntDir=Release

liblog.mak								PREDEF= -DNDEBUG -D_LIB -D_GAME_SERVER
accountserver.mak        	PREDEF= -DNDEBUG -D_CONSOLE
gameserver.mak						PREDEF= -DNDEBUG -D_CONSOLE -D_GAME_SERVER
groupserver.mak						PREDEF= -DNDEBUG -D_CONSOLE
agent.mak									PREDEF= -DNDEBUG -D_CONSOLE

*************************************************************************

LibServerFrame
��VCLibrarianTool��������OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="..\Server\LibServerFrame.lib"
/>
	
*************************************************************************		
	
DataInfor	
��VCLibrarianTool�������OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="../Server/DataInfor.lib"
	
*************************************************************************		
	
LibLog
��VCLibrarianTool�������OutputFile
<Tool
	Name="VCLibrarianTool"
	OutputFile="..\Server\LibLog.lib"
	
*************************************************************************

AccountServer
	
��VCLinkerTool�������OutputFile
<Tool
		Name="VCLinkerTool"
		OutputFile="./AccountServer.exe"

��������еĻ���
��99		
<Configuration
			Name="Release|Win32"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="1"
			>

�����
<Tool
	Name="VCLinkerTool"
	OutputFile="./AccountServer.exe"
/>


*************************************************************************		
	
GameServer
��VCLinkerTool�������OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="./GameServer.exe"
	

*************************************************************************		
	
Agent
��VCLinkerTool�������OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="$(OutDir)/Agent.exe"
	
*************************************************************************		
	
GroupServer	
��VCLinkerTool�������OutputFile
<Tool
	Name="VCLinkerTool"
	OutputFile="./GroupServer.exe"

*************************************************************************		
