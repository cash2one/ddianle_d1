#include "GameMsg_S2C_GetCheckInInfoResult.h"
#include "../share/ServerMsgDef.h"
#include "CheckInfoMgr.h"

GameMsg_S2C_GetCheckInInfoResult::GameMsg_S2C_GetCheckInInfoResult()
:GameMsg_Base(MSG_S2C_GetCheckInInfoResult)
{
}

GameMsg_S2C_GetCheckInInfoResult::~GameMsg_S2C_GetCheckInInfoResult()
{
}

bool GameMsg_S2C_GetCheckInInfoResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(CCheckInDataManager::Instance().GetDescribe());
	IOBuff.AddUInt( (unsigned int)time(NULL));
	IOBuff.AddUInt( m_CheckInInfo.m_nDayIndex );
	IOBuff.AddUInt( m_CheckInInfo.m_nCheckInTime );

	CCheckInDataManager::Instance().doEncode( IOBuff, m_cSex);
	return true;
}







// end of file


