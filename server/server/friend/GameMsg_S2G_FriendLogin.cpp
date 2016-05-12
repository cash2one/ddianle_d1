#include "GameMsg_S2G_FriendLogin.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_FriendLogin::GameMsg_S2G_FriendLogin(unsigned int nLoginID, unsigned int nNotifiedID):
GameMsg_Base(MSG_S2G_FriendLogin)
{
	m_nLoginID = nLoginID;
	//m_nNotifiedID = nNotifiedID;
}

GameMsg_S2G_FriendLogin::~GameMsg_S2G_FriendLogin()
{
}

bool GameMsg_S2G_FriendLogin::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nLoginID);
	//IOBuff.AddUInt(m_nNotifiedID);

	return true;
}

bool GameMsg_S2G_FriendLogin::doDecode(CParamPool &IOBuff)
{
	m_nLoginID = IOBuff.GetUInt();
	//m_nNotifiedID = IOBuff.GetUInt();

	return true;
}


// end of file

