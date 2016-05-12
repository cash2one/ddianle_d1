#include	"GameMsg_C2S_Recheck.h"
#include	"ServerMsgDef.h"

GameMsg_C2S_ReCheck::GameMsg_C2S_ReCheck():
GameMsg_Base(MSG_C2S_Recheck)
{
}

GameMsg_C2S_ReCheck::~GameMsg_C2S_ReCheck()
{
}

bool GameMsg_C2S_ReCheck::doDecode(CParamPool &IOBuff)
{
	m_nWeekDay = IOBuff.GetInt();

	return true;
}


// end of file


