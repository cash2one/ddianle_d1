#include "GameMsg_S2G_AddBlackFriend.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_AddBlackListFriend::GameMsg_S2G_AddBlackListFriend()
:GameMsg_Base(MSG_S2G_AddBlackListFriend)
{
}

GameMsg_S2G_AddBlackListFriend::~GameMsg_S2G_AddBlackListFriend()
{
}

bool GameMsg_S2G_AddBlackListFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	m_FriendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_S2G_AddBlackListFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_FriendInfo.doDecode(IOBuff);

	return true;
}

// end of file

