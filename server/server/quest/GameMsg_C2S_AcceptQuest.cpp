#include "GameMsg_C2S_AcceptQuest.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_AcceptQuest::GameMsg_C2S_AcceptQuest():
GameMsg_Base(MSG_C2S_AcceptQuest)
{
}
GameMsg_C2S_AcceptQuest::~GameMsg_C2S_AcceptQuest()
{
}
bool GameMsg_C2S_AcceptQuest::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();

	return true;
}

GameMsg_C2S_ClientCompleteQuest::GameMsg_C2S_ClientCompleteQuest():
GameMsg_Base(MSG_C2S_ClientCompleteQuest)
{
	m_nQuestID = 0;
	m_nCompleteType = EQuest_Complete_None;
	m_nParam1 = 0;
	m_nParam2 = 0;
}

GameMsg_C2S_ClientCompleteQuest::~GameMsg_C2S_ClientCompleteQuest()
{

}

bool GameMsg_C2S_ClientCompleteQuest::doDecode(CParamPool &IOBuff)
{
	m_nQuestID = IOBuff.GetUShort();
	m_nCompleteType = (EQuest_Complete_Type)IOBuff.GetUShort();
	m_nParam1 = (int)IOBuff.GetUInt();
	m_nParam2 = IOBuff.GetUShort();

	return true;
}

GameMsg_C2S_GetQuestState::GameMsg_C2S_GetQuestState()
	: GameMsg_Base(MSG_C2S_GetQuestState)
{
}

GameMsg_C2S_GetQuestState::~GameMsg_C2S_GetQuestState()
{

}

bool GameMsg_C2S_GetQuestState::doDecode(CParamPool &IOBuff)
{
	unsigned short nCount = IOBuff.GetUShort();
	for (unsigned short i = 0 ; i < nCount ; i++)
	{
		unsigned short questID = IOBuff.GetUShort();
		m_nQuestIDList.push_back(questID);
	}
	return true;
}

GameMsg_S2C_GetQuestStateRes::GameMsg_S2C_GetQuestStateRes()
	: GameMsg_Base(MSG_S2C_GetQuestStateRes)
{
}

GameMsg_S2C_GetQuestStateRes::~GameMsg_S2C_GetQuestStateRes()
{

}

bool GameMsg_S2C_GetQuestStateRes::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_QuestStateMap.size());
	std::map<unsigned short,unsigned short>::iterator it = m_QuestStateMap.begin();
	for (;it != m_QuestStateMap.end() ; it++)
	{
		IOBuff.AddUShort(it->first);
		IOBuff.AddUShort(it->second);
	}
	return true;
}

void GameMsg_S2C_GetQuestStateRes::AddQuestState(unsigned short nQuestID,unsigned short nQuestState)
{
	m_QuestStateMap.insert(make_pair(nQuestID,nQuestState));
}

// end of file


