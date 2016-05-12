#include "GameMsg_S2G_PushOnlineInfoToAllFriends.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_PushOnlineInfoToAllFriends::GameMsg_S2G_PushOnlineInfoToAllFriends()
:GameMsg_Base(MSG_S2G_PushOnlineInfoToAllFriends)
,m_nRoleID(0)
{
}

GameMsg_S2G_PushOnlineInfoToAllFriends::~GameMsg_S2G_PushOnlineInfoToAllFriends()
{
}

bool GameMsg_S2G_PushOnlineInfoToAllFriends::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	
	return true;
}

bool GameMsg_S2G_PushOnlineInfoToAllFriends::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();

	return true;
}


