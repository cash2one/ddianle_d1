#include "GameMsg_G2S_NotifyGameRemoveFriend.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_NotifyGameRemoveFriend::GameMsg_G2S_NotifyGameRemoveFriend():
GameMsg_Base(MSG_G2S_NotifyGameRemoveFriend)
{
	m_nRemovedRoleID = 0;
}


GameMsg_G2S_NotifyGameRemoveFriend::GameMsg_G2S_NotifyGameRemoveFriend(unsigned int nRemovedRoleID):
GameMsg_Base(MSG_G2S_NotifyGameRemoveFriend)
{
	m_nRemovedRoleID = nRemovedRoleID;
}

GameMsg_G2S_NotifyGameRemoveFriend::~GameMsg_G2S_NotifyGameRemoveFriend()
{
}

bool GameMsg_G2S_NotifyGameRemoveFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRemovedRoleID);

	return true;
}

bool GameMsg_G2S_NotifyGameRemoveFriend::doDecode(CParamPool &IOBuff)
{
	m_nRemovedRoleID = IOBuff.GetUInt();

	return true;
}


// end of file

