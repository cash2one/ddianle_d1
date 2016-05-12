#include "GameMsg_S2G_RemoveMuteList.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_RemoveMuteList::GameMsg_S2G_RemoveMuteList():GameMsg_Base(MSG_S2G_RemoveMuteList)
{
}

GameMsg_S2G_RemoveMuteList::GameMsg_S2G_RemoveMuteList(unsigned int nRoleID, unsigned int nRemoveID):
GameMsg_Base(MSG_S2G_RemoveMuteList)
{
	m_nRoleID = nRoleID;
	m_nRemoveID = nRemoveID;
}

GameMsg_S2G_RemoveMuteList::~GameMsg_S2G_RemoveMuteList()
{
}

bool GameMsg_S2G_RemoveMuteList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nRemoveID);

	return true;
}

bool GameMsg_S2G_RemoveMuteList::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nRemoveID = IOBuff.GetUInt();

	return true;
}

// end of file

