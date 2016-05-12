#include "GameMsg_Server_Connected.h"
#include "../../socket/ParamPool.h"
#include "ServerMsgDef.h"

GameMsg_Server_Connected::GameMsg_Server_Connected(void):_baseClass(MSG_SERVER_Connected)
{
	nCurPlayerCount = 0;
	nInstallerId = 0;
}

GameMsg_Server_Connected::~GameMsg_Server_Connected(void)
{
}
bool GameMsg_Server_Connected::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddUShort(nCurPlayerCount);
	IoBuff.AddUChar(nSvrType);
	IoBuff.AddInt(nLine);
	IoBuff.AddUChar(nInstallerId);
	IoBuff.AddString(strName.c_str());
	return true;
}
bool GameMsg_Server_Connected::doDecode( CParamPool& IoBuff )
{
	nCurPlayerCount = IoBuff.GetUShort();
	nSvrType = IoBuff.GetUChar();
	nLine = IoBuff.GetInt();
	nInstallerId = IoBuff.GetUChar();
	strName = IoBuff.GetString();
	return true;
}

//end

