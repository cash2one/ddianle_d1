#include "GameMsg_S2G_GameServerInfo.h"
#include "ServerMsgDef.h"

GameMsg_S2G_GameServerInfo::GameMsg_S2G_GameServerInfo():GameMsg_Base(MSG_S2G_GameServerInfo)
{
	m_nLine = -1;
}
GameMsg_S2G_GameServerInfo::~GameMsg_S2G_GameServerInfo()
{
}
bool GameMsg_S2G_GameServerInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nLine);
	
	return true;
}
bool GameMsg_S2G_GameServerInfo::doDecode(CParamPool &IOBuff)
{
	m_nLine = IOBuff.GetInt();
	
	return true;
}


//file end

