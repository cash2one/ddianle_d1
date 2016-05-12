#include "GameMsg_S2G_NotifyHasBeenAccepted.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_NotifyHasBeenAccpted::GameMsg_S2G_NotifyHasBeenAccpted()
:GameMsg_Base(MSG_S2G_NotityHasBeenAccepted)
,m_nRoleID(0)
{
}

GameMsg_S2G_NotifyHasBeenAccpted::~GameMsg_S2G_NotifyHasBeenAccpted()
{
}

bool GameMsg_S2G_NotifyHasBeenAccpted::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nRoleID);
	m_friendInfo.doEncode(IOBuff);

	return true;
}

bool GameMsg_S2G_NotifyHasBeenAccpted::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_friendInfo.doDecode(IOBuff);

	return true;
}

// end of file

