#include "GameMsg_S2G_GetFriendOnlineInfo.h"
#include "../share/ServerMsgDef.h"



GameMsg_S2G_GetFriendOnlineInfo::GameMsg_S2G_GetFriendOnlineInfo(unsigned int nRoleID):
GameMsg_Base(MSG_S2G_GetFriendOnlineInfo),
m_nRoleID(nRoleID)
{
}

GameMsg_S2G_GetFriendOnlineInfo::~GameMsg_S2G_GetFriendOnlineInfo()
{
}

bool GameMsg_S2G_GetFriendOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}

bool GameMsg_S2G_GetFriendOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}



