#include "GameMsg_C2S_GetQuestReward.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_GetQuestReward::GameMsg_C2S_GetQuestReward():
GameMsg_Base(MSG_C2S_GetQuestReward)
{
	m_nQuestID = 0;
}
GameMsg_C2S_GetQuestReward::~GameMsg_C2S_GetQuestReward()
{
}
bool GameMsg_C2S_GetQuestReward::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();	

	return true;
}


GameMsg_C2S_GetFresherQuestReward::GameMsg_C2S_GetFresherQuestReward():
GameMsg_Base(MSG_C2S_GetFresherQuestReward)
{
	m_nQuestID = 0;
}
GameMsg_C2S_GetFresherQuestReward::~GameMsg_C2S_GetFresherQuestReward()
{
}
bool GameMsg_C2S_GetFresherQuestReward::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();	

	return true;
}

// end of file


