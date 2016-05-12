#include "GameMsg_S2C_QuestStateChange.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_QuestStateChange::GameMsg_S2C_QuestStateChange(unsigned short nQuestID, unsigned char nState):
GameMsg_Base(MSG_S2C_QuestStateChanged),
m_nQuestID(nQuestID),
m_nState(nState)
{
}

GameMsg_S2C_QuestStateChange::~GameMsg_S2C_QuestStateChange()
{
}

bool GameMsg_S2C_QuestStateChange::doEncode(CParamPool &IOBuff)
{ 
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddUChar(m_nState);

	IOBuff.AddUShort((unsigned short) m_QuestTargetSendList.size());
	list<CQuestTargetSend>::iterator it = m_QuestTargetSendList.begin();
	for ( ; it != m_QuestTargetSendList.end() ; it++)
	{
		it->doEncode(IOBuff);
	}
    IOBuff.AddStringW(m_strJsonProgress);

	return true;
}

void GameMsg_S2C_QuestStateChange::SetListEntry(CQuestListEntry& listEntry)
{
	list<CQuestTargetSend>::iterator it = listEntry.m_listTargetInfo.begin();
	for (;it != listEntry.m_listTargetInfo.end() ; it++)
	{
		m_QuestTargetSendList.push_back(*it);
	}
    m_strJsonProgress = listEntry.m_strJsonProgress;
}

// end of file


