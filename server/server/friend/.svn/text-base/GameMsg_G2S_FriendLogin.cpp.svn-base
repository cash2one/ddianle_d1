#include "GameMsg_G2S_FriendLogin.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_FriendLogin::GameMsg_G2S_FriendLogin(unsigned int nLoginID, const char *szName):
GameMsg_Base(MSG_G2S_FriendLogin)
{
}

GameMsg_G2S_FriendLogin::~GameMsg_G2S_FriendLogin()
{
}

bool GameMsg_G2S_FriendLogin::doEncode(CParamPool &IOBuff)
{
	m_FriendOnlineInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_G2S_FriendLogin::doDecode(CParamPool &IOBuff)
{
	m_FriendOnlineInfo.doDecode(IOBuff);

	return true;
}


// end of file


