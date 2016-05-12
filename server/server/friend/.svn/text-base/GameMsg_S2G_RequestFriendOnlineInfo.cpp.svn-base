#include "GameMsg_S2G_RequestFriendOnlineInfo.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_RequestFriendOnlineInfo::GameMsg_S2G_RequestFriendOnlineInfo()
:GameMsg_Base(MSG_S2G_RequestFriendOnlineInfo)
,m_nSrcRoleID(0)
,m_nDstRoleID(0)
{

}

GameMsg_S2G_RequestFriendOnlineInfo::~GameMsg_S2G_RequestFriendOnlineInfo()
{

}

bool GameMsg_S2G_RequestFriendOnlineInfo::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcRoleID);
	IOBuff.AddUInt(m_nDstRoleID);
	m_FriendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_S2G_RequestFriendOnlineInfo::doDecode(CParamPool &IOBuff)
{
	m_nSrcRoleID = IOBuff.GetUInt();
	m_nDstRoleID = IOBuff.GetUInt();
	m_FriendInfo.doEncode(IOBuff);

	return true;
}

