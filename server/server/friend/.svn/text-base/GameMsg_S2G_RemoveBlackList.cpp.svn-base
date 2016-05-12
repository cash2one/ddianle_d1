#include "GameMsg_S2G_RemoveBlackList.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_RemoveBlackList::GameMsg_S2G_RemoveBlackList(unsigned int nSrcRoleID, unsigned int nTargetID)
:GameMsg_Base(MSG_S2G_RemoveBlackList)
,m_nSrcRoleID(nSrcRoleID)
,m_nTargetID(nTargetID)
{
}

GameMsg_S2G_RemoveBlackList::~GameMsg_S2G_RemoveBlackList()
{
}

bool GameMsg_S2G_RemoveBlackList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcRoleID);
	IOBuff.AddUInt(m_nTargetID);

	return true;
}
bool GameMsg_S2G_RemoveBlackList::doDecode(CParamPool &IOBuff)
{
	m_nSrcRoleID = IOBuff.GetUInt();
	m_nTargetID = IOBuff.GetUInt();

	return true;
}


