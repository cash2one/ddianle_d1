#include "GameMsg_S2C_GetCheckInRewardResult.h"
#include	"ServerMsgDef.h"

GameMsg_S2C_GetCheckInRewardResult::GameMsg_S2C_GetCheckInRewardResult(unsigned char nDay, unsigned char nResult):GameMsg_Base(MSG_S2C_GetCheckInRewardResult)
{
	m_nDay = nDay;
	m_nResult = nResult;
}

GameMsg_S2C_GetCheckInRewardResult::~GameMsg_S2C_GetCheckInRewardResult()
{
}

bool GameMsg_S2C_GetCheckInRewardResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nDay);
	IOBuff.AddUChar(m_nResult);

	return true;
}

// end of file




