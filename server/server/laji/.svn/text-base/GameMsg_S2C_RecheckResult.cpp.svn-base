#include	"GameMsg_S2C_RecheckResult.h"
#include	"ServerMsgDef.h"

GameMsg_S2C_RecheckResult::GameMsg_S2C_RecheckResult(int nWeekDay,unsigned char nResult)
:GameMsg_Base(MSG_S2C_RecheckResult)
,m_nWeekDay(nWeekDay)
,m_nResult(nResult)
{
	m_nWeekDay = nWeekDay;
	m_nResult = nResult;
}

GameMsg_S2C_RecheckResult::~GameMsg_S2C_RecheckResult()
{
}

bool GameMsg_S2C_RecheckResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nWeekDay);
	m_item.doEncode(IOBuff);
	IOBuff.AddUChar(m_nResult);

	return true;
}


// end of file


