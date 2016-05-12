#include "GameMsg_Account_GetGameServersInfo.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"

GameMsg_Account_C2S_GetGameServersInfo::GameMsg_Account_C2S_GetGameServersInfo(void)
:GameMsg_Base(MSG_Account_C2S_GetGameServersInfo)
{
}

GameMsg_Account_C2S_GetGameServersInfo::~GameMsg_Account_C2S_GetGameServersInfo(void)
{
}

bool GameMsg_Account_C2S_GetGameServersInfo::doDecode( CParamPool& IoBuff )
{
	return true;
}

GameMsg_Account_S2C_GetGameServersInfoRes::GameMsg_Account_S2C_GetGameServersInfoRes():
GameMsg_Base(MSG_Account_S2C_GetGameServersInfoRes)
{
	m_nCurLine = 0;
}

GameMsg_Account_S2C_GetGameServersInfoRes::~GameMsg_Account_S2C_GetGameServersInfoRes()
{
}

bool GameMsg_Account_S2C_GetGameServersInfoRes::doEncode(CParamPool& IoBuff )
{
	IoBuff.AddInt(m_nCurLine);
	IoBuff.AddUShort((unsigned short)m_listGameServersInfo.size());

	if (m_listGameServersInfo.size() > 0) {
		list<CGameServerInfo>::iterator itr = m_listGameServersInfo.begin();
		for (; itr != m_listGameServersInfo.end(); ++itr) {
			itr->doEncode(IoBuff);
		}
	}

	return true;
}





