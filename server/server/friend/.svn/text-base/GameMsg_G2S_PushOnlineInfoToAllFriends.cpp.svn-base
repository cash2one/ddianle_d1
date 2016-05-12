#include "GameMsg_G2S_PushOnlineInfoToAllFriends.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_PushOnlineInfoToAllFriends::GameMsg_G2S_PushOnlineInfoToAllFriends():
GameMsg_Base(MSG_G2S_PushOnlineInfoToAllFriends)
{
}

GameMsg_G2S_PushOnlineInfoToAllFriends::~GameMsg_G2S_PushOnlineInfoToAllFriends()
{
}

bool GameMsg_G2S_PushOnlineInfoToAllFriends::doEncode(CParamPool &IOBuff)
{
	m_roleOnlineInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_G2S_PushOnlineInfoToAllFriends::doDecode(CParamPool &IOBuff)
{
	m_roleOnlineInfo.doDecode(IOBuff);

	return true;
}


// end of file

