#include "GameMsg_S2G_GetBlackListOnlineInfo.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_GetBlackListOnlineInfo::GameMsg_S2G_GetBlackListOnlineInfo()
:GameMsg_Base(MSG_S2G_GetBlackListOnlineInfo)
,m_nRoleID(0)
{

}

GameMsg_S2G_GetBlackListOnlineInfo::~GameMsg_S2G_GetBlackListOnlineInfo()
{

}

bool GameMsg_S2G_GetBlackListOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

bool GameMsg_S2G_GetBlackListOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}



