#include "GameMsg_S2G_BecomeFriends.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_BecomeFriends::GameMsg_S2G_BecomeFriends():
GameMsg_Base(MSG_S2G_BecomeFriends)
{
	m_nRoleID = 0;
}

GameMsg_S2G_BecomeFriends::~GameMsg_S2G_BecomeFriends()
{
}

bool GameMsg_S2G_BecomeFriends::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	m_friendInfo.doEncode(IOBuff);
	
	return true;
}

bool GameMsg_S2G_BecomeFriends::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetInt();
	m_friendInfo.doDecode(IOBuff);

	return true;
}

// end of file

