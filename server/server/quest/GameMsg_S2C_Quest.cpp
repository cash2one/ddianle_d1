#include "GameMsg_S2C_Quest.h"
#include "../share/ServerMsgDef.h"
#include "QuestListEntry.h"
#include "QuestDataManager.h"

GameMsg_S2C_SendQuestStaticBinData::GameMsg_S2C_SendQuestStaticBinData()
:GameMsg_Base(MSG_S2C_SendQuestStaticBinData)
,m_nBinDataSize(0)
{
	memset(m_szBinData, 0, C_MAX_QUEST_DATA_SIZE);
}

GameMsg_S2C_SendQuestStaticBinData::~GameMsg_S2C_SendQuestStaticBinData()
{

}

bool GameMsg_S2C_SendQuestStaticBinData::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUShort(m_nBinDataSize);
	IOBuff.AddMemory(m_szBinData, m_nBinDataSize);

	return true;
}


GameMsg_S2C_AutoPushQuest::GameMsg_S2C_AutoPushQuest():
GameMsg_Base(MSG_S2C_AutoPushQuest)
{
	m_nNewQuestCount = 0;
}

GameMsg_S2C_AutoPushQuest::~GameMsg_S2C_AutoPushQuest()
{
}

bool GameMsg_S2C_AutoPushQuest::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nNewQuestCount);

	return true;
}


GameMsg_S2C_UnRewardQuest::GameMsg_S2C_UnRewardQuest():
GameMsg_Base(MSG_S2C_UnRewardQuest)
{
}

GameMsg_S2C_UnRewardQuest::~GameMsg_S2C_UnRewardQuest()
{
}

bool GameMsg_S2C_UnRewardQuest::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_IDs.size());
	
	std::vector<unsigned short>::iterator it = m_IDs.begin();
	for (;it != m_IDs.end() ; it++)
	{
		IOBuff.AddUShort(*it);
	}

	return true;
}

void GameMsg_S2C_UnRewardQuest::SetIDList(std::vector<unsigned short>& ids)
{
	for (int i = 0 ; i < (int)ids.size() ; i++)
	{
		m_IDs.push_back(ids[i]);
	}
}

GameMsg_C2S_GetFresherQuestList::GameMsg_C2S_GetFresherQuestList()
: GameMsg_Base(MSG_C2S_GetFresherQuestList)
{
}

GameMsg_C2S_GetFresherQuestList::~GameMsg_C2S_GetFresherQuestList()
{
}

bool GameMsg_C2S_GetFresherQuestList::doDecode(CParamPool &ioBuff)
{
	return true;
}

GameMsg_S2C_SendFresherQuest::GameMsg_S2C_SendFresherQuest()
:GameMsg_Base(MSG_S2C_SendFresherQuest)
,m_bIsOpen(false),m_nFresherQuestID(0), m_nSex(ESexType_Male)
{
}

GameMsg_S2C_SendFresherQuest::~GameMsg_S2C_SendFresherQuest()
{

}

bool GameMsg_S2C_SendFresherQuest::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddBool(m_bIsOpen);
	IOBuff.AddStringW(m_strError.c_str());
	if (m_bIsOpen)
	{
		IOBuff.AddUShort(m_nFresherQuestID);

		IOBuff.AddUShort((unsigned short)m_listQuestInfo.size());
		if (m_listQuestInfo.size() > 0)
		{
			for (std::list<CQuestListEntry>::iterator it = m_listQuestInfo.begin(); it != m_listQuestInfo.end(); it++)
			{
				it->doEncode(IOBuff);
			}
		}

		CFresherQuestDataManager::Instance().doEncode(IOBuff, m_nSex);
	}

	return true;
}

GameMsg_S2C_UnRewardFresherQuest::GameMsg_S2C_UnRewardFresherQuest():
GameMsg_Base(MSG_S2C_UnRewardFresherQuest)
{
	m_nQuestID = 0;
	m_nTargetValue = 0;
	m_nQuestState = eQuestState_None;
}

GameMsg_S2C_UnRewardFresherQuest::~GameMsg_S2C_UnRewardFresherQuest()
{
}

bool GameMsg_S2C_UnRewardFresherQuest::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddInt(m_nTargetValue);
	IOBuff.AddUChar(m_nQuestState);
	return true;
}

GameMsg_S2C_SendQuestNewStaticBinData::GameMsg_S2C_SendQuestNewStaticBinData()
: GameMsg_Base(MSG_S2C_SendQuestNewStaticBinData)
{
	m_nBinDataSize = 0;
	memset(m_szBinData,0,C_MAX_QUEST_DATA_SIZE);
}

GameMsg_S2C_SendQuestNewStaticBinData::GameMsg_S2C_SendQuestNewStaticBinData(bool lastPack, unsigned short binSize,const char* buff)
: GameMsg_Base(MSG_S2C_SendQuestNewStaticBinData)
{
    m_bLastPack = lastPack;
	m_nBinDataSize = binSize;
	m_szBinData = new char[m_nBinDataSize];
	if(m_szBinData != NULL)
		memcpy(m_szBinData,buff,binSize);
	else
	{
		m_nBinDataSize = 0;
	}
}

GameMsg_S2C_SendQuestNewStaticBinData::~GameMsg_S2C_SendQuestNewStaticBinData()
{
	if (m_szBinData != NULL)
	{
		delete[] m_szBinData;
		m_szBinData = NULL;
		m_nBinDataSize = 0;
	}
}

bool GameMsg_S2C_SendQuestNewStaticBinData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bLastPack);
	IOBuff.AddUShort(m_nBinDataSize);
	IOBuff.AddMemory(m_szBinData, m_nBinDataSize);
	return true;
}
