#include "GameMsg_S2G_RemoveFriend.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2G_RemoveFriend::GameMsg_S2G_RemoveFriend()
:GameMsg_Base(MSG_S2G_RemoveFriend)
,m_nRoleID(0)
,m_nTargetID(0)
{
}

GameMsg_S2G_RemoveFriend::~GameMsg_S2G_RemoveFriend()
{
}

bool GameMsg_S2G_RemoveFriend::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUInt(m_nTargetID);
	
	return true;
}

bool GameMsg_S2G_RemoveFriend::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nTargetID = IOBuff.GetUInt();

	return true;
}


// end of file

