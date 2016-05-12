#include "GameMsg_C2S_Check.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_Check::GameMsg_C2S_Check():
GameMsg_Base(MSG_C2S_Check)
,m_nDayIndex(0)
{
}

GameMsg_C2S_Check::~GameMsg_C2S_Check()
{
}

bool GameMsg_C2S_Check::doDecode(CParamPool &IOBuff)
{
	m_nDayIndex = IOBuff.GetInt();

	return true;
}



// end of file

