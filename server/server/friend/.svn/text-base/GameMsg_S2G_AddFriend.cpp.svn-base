#include "GameMsg_S2G_AddFriend.h"
#include "share/ServerMsgDef.h"

GameMsg_S2G_AddBlacListFriend::GameMsg_S2G_AddBlacListFriend()
: GameMsg_Base(MSG_S2G_AddBlackListFriend)
{
}

GameMsg_S2G_AddBlacListFriend::~GameMsg_S2G_AddBlacListFriend()
{
}

bool GameMsg_S2G_AddBlacListFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	m_nFriendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_S2G_AddBlacListFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nFriendInfo.doDecode(IOBuff);

	return true;
}

// end of file

