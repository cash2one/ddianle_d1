#include "GameMsg_S2C_UpdateExtraExpActivityState.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_UpdateExtraExpActivityState::GameMsg_S2C_UpdateExtraExpActivityState(int nActivityID, bool bOpening, unsigned char nStartHour, unsigned char nStartMinute,
										unsigned char nStopHour, unsigned char nStopMinute, unsigned short nRewardExpRate)
: GameMsg_Base(MSG_S2C_UpdateExtraExpActivityState)
{
	m_nActivityID = nActivityID;
	m_bOpening = bOpening;
	m_nStartHour = nStartHour;
	m_nStartMinute = nStartMinute;
	m_nStopHour = nStopHour;
	m_nStopMinute = nStopMinute;
	m_nRewardExpRate = nRewardExpRate;
}

GameMsg_S2C_UpdateExtraExpActivityState::~GameMsg_S2C_UpdateExtraExpActivityState()
{
}

bool GameMsg_S2C_UpdateExtraExpActivityState::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nActivityID);
	IOBuff.AddBool(m_bOpening);
	IOBuff.AddUChar(m_nStartHour);
	IOBuff.AddUChar(m_nStartMinute);
	IOBuff.AddUChar(m_nStopHour);
	IOBuff.AddUChar(m_nStopMinute);
	IOBuff.AddUShort(m_nRewardExpRate);
	return true;
}


