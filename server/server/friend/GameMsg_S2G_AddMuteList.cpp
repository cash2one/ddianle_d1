#include "GameMsg_S2G_AddMuteList.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_AddMuteList::GameMsg_S2G_AddMuteList():
GameMsg_Base(MSG_S2G_AddMuteList)
{
	m_nRoleID = 0;
}

GameMsg_S2G_AddMuteList::GameMsg_S2G_AddMuteList(unsigned int nRoleID, unsigned int nMuteID):
GameMsg_Base(MSG_S2G_AddMuteList)
{
	m_nRoleID = nRoleID;
	m_nMuteID = nMuteID;
}

GameMsg_S2G_AddMuteList::~GameMsg_S2G_AddMuteList()
{
}

bool GameMsg_S2G_AddMuteList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nMuteID);

	return true;
}

bool GameMsg_S2G_AddMuteList::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nMuteID = IOBuff.GetUInt();

	return true;
}

// end of file

