#include "GameMsg_G2S_BecomeFriends.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_BecomeFriends::GameMsg_G2S_BecomeFriends():
GameMsg_Base(MSG_G2S_BecomeFriends)
{
	m_nRoleID = 0;
}

GameMsg_G2S_BecomeFriends::~GameMsg_G2S_BecomeFriends()
{
}

bool GameMsg_G2S_BecomeFriends::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nRoleID);
	m_friendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_G2S_BecomeFriends::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetInt();
	m_friendInfo.doDecode(IOBuff);

	return true;
}

// end of file

