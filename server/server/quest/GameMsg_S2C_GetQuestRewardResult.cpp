#include "GameMsg_S2C_GetQuestRewardResult.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_GetQuestRewardResult::GameMsg_S2C_GetQuestRewardResult(unsigned short nQuestID, unsigned char nResult, string strResult):
GameMsg_Base(MSG_S2C_GetQuestRewardResult),
m_nQuestID(nQuestID),
m_nReuslt(nResult),
m_strResult(strResult)
{
}

GameMsg_S2C_GetQuestRewardResult::~GameMsg_S2C_GetQuestRewardResult()
{
}
bool GameMsg_S2C_GetQuestRewardResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddUChar(m_nReuslt);
	IOBuff.AddStringW(m_strResult.c_str());

	return true;
}


GameMsg_S2C_GetFresherQuestRewardSuc::GameMsg_S2C_GetFresherQuestRewardSuc()
	: GameMsg_Base(MSG_S2C_GetFresherQuestRewardSuc)
{
	m_nQuestID = 0;
}

GameMsg_S2C_GetFresherQuestRewardSuc::~GameMsg_S2C_GetFresherQuestRewardSuc()
{
}
bool GameMsg_S2C_GetFresherQuestRewardSuc::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);

	return true;
}


GameMsg_S2C_GetFresherQuestRewardFail::GameMsg_S2C_GetFresherQuestRewardFail()
: GameMsg_Base(MSG_S2C_GetFresherQuestRewardFail)
{
}

GameMsg_S2C_GetFresherQuestRewardFail::~GameMsg_S2C_GetFresherQuestRewardFail()
{
}
bool GameMsg_S2C_GetFresherQuestRewardFail::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

// end of file


