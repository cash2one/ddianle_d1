#include "GameMsg_S2G_AddBlackList.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_AddBlackList::GameMsg_S2G_AddBlackList(unsigned int nRoleID, unsigned int nBlackID)
:GameMsg_Base(MSG_S2G_AddBlackList)
,m_nRoleID(nRoleID)
,m_nBlackID(nBlackID)
{
}

GameMsg_S2G_AddBlackList::~GameMsg_S2G_AddBlackList()
{
}

bool GameMsg_S2G_AddBlackList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nBlackID);

	return true;
}

bool GameMsg_S2G_AddBlackList::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nBlackID = IOBuff.GetUInt();

	return true;
}


