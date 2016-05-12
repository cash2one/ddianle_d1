#include "GameMsg_G2S_NotifyGameRemoveFollowedFriend.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_NotifyGameRemoveFollowedFriend::GameMsg_G2S_NotifyGameRemoveFollowedFriend():
GameMsg_Base(MSG_G2S_NotifyGameRemoveFollowedFriend)
{
	m_nRemovedRoleID = 0;
}

//GameMsg_G2S_NotifyGameRemoveFollowedFriend::GameMsg_G2S_NotifyGameRemoveFollowedFriend(unsigned int nDesRoleID, FriendInfo &friendInfo):
//GameMsg_Base(MSG_G2S_NotifyGameAddFriend)
//{
//	m_nDesRoleID = nDesRoleID;
//	m_friendInfo = friendInfo;
//}

GameMsg_G2S_NotifyGameRemoveFollowedFriend::GameMsg_G2S_NotifyGameRemoveFollowedFriend(unsigned int nRemovedRoleID):
GameMsg_Base(MSG_G2S_NotifyGameRemoveFollowedFriend)
{
	m_nRemovedRoleID = nRemovedRoleID;
}

GameMsg_G2S_NotifyGameRemoveFollowedFriend::~GameMsg_G2S_NotifyGameRemoveFollowedFriend()
{
}

bool GameMsg_G2S_NotifyGameRemoveFollowedFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRemovedRoleID);
	//IOBuff.AddUInt(m_nDesRoleID);
	//IOBuff.AddUInt(m_friendInfo.nRoleID);
	//IOBuff.AddUChar(m_friendInfo.nSex);
	//IOBuff.AddString(m_friendInfo.szName);
	//IOBuff.AddUShort(m_friendInfo.nLevel);

	return true;
}

bool GameMsg_G2S_NotifyGameRemoveFollowedFriend::doDecode(CParamPool &IOBuff)
{
	m_nRemovedRoleID = IOBuff.GetUInt();
	//m_nDesRoleID = IOBuff.GetUInt();
	//m_friendInfo.nRoleID = IOBuff.GetUInt();
	//m_friendInfo.nSex = IOBuff.GetUChar();
	//string str = IOBuff.GetString();
	//CopyString2Buffer(str, m_friendInfo.szName, MAX_NAME_LEN - 1);
	//m_friendInfo.nLevel = IOBuff.GetUShort();

	return true;
}


// end of file

