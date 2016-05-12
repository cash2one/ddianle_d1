#include "GameMsg_S2G_Logout.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_Logout::GameMsg_S2G_Logout(unsigned int nRoleID, int nLine):
GameMsg_Base(MSG_S2G_Logout)
{
	m_nRoleID = nRoleID;
	m_nLine = nLine;
}

GameMsg_S2G_Logout::~GameMsg_S2G_Logout()
{
}

bool GameMsg_S2G_Logout::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddInt(m_nLine);

	return true;
}

bool GameMsg_S2G_Logout::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nLine = IOBuff.GetInt();

	return true;
}


