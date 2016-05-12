#include "GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist.h"
#include "../share/ServerMsgDef.h"

GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist::GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist()
:GameMsg_Base(MSG_G2S_NotifyGameHasBeenDeletedBlacklist)
,m_nSrcRoleID(0)
{
}

GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist::GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist(unsigned int nSrcRoleID)
:GameMsg_Base(MSG_G2S_NotifyGameHasBeenDeletedBlacklist)
,m_nSrcRoleID(nSrcRoleID)
{
}

GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist::~GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist()
{

}

bool GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSrcRoleID);

	return true;
}

bool GameMsg_G2S_NotifyGameHasBeenDeletedBlacklist::doDecode(CParamPool &IOBuff)
{
	m_nSrcRoleID = IOBuff.GetUInt();

	return true;
}



