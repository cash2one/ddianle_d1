#include "GameMsg_C2S_QuestRead.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_QuestRead::GameMsg_C2S_QuestRead():
GameMsg_Base(MSG_C2S_QuestRead)
{
	m_nQuestID = 0;
}
GameMsg_C2S_QuestRead::~GameMsg_C2S_QuestRead()
{
}
bool GameMsg_C2S_QuestRead::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();

	return true;
}


// end of file


