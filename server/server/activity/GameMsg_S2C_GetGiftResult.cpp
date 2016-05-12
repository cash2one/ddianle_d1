#include "GameMsg_S2C_GetGiftResult.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_GetGiftResult::GameMsg_S2C_GetGiftResult(int nIndexID, int nResult)
: GameMsg_Base(MSG_S2C_GetGiftResult)
{
	m_nIndexID = nIndexID;
	m_nResult = nResult;
}

GameMsg_S2C_GetGiftResult::~GameMsg_S2C_GetGiftResult()
{
}
bool GameMsg_S2C_GetGiftResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt(m_nIndexID);
	IOBuff.AddInt(m_nResult);
	return true;
}



