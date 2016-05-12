#include "GameMsg_S2C_AcceptQuestResult.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Windefine.h"

GameMsg_S2C_AcceptQuestResult::GameMsg_S2C_AcceptQuestResult():
GameMsg_Base(MSG_S2C_AcceptQuestResult),
m_nQuestID(0),
m_nResult(0)
{
}

// 这个函数新的任务不要再调用
GameMsg_S2C_AcceptQuestResult::GameMsg_S2C_AcceptQuestResult(unsigned short nQuestID, unsigned char nResult, unsigned char nState, QUESTINFO &questInfo, CRoleEntity &role):
GameMsg_Base(MSG_S2C_AcceptQuestResult),
m_nQuestID(nQuestID),
m_nResult(nResult),
m_nState(nState)
{
	CQuestTargetSend targetSend;
	map <itemtype_t, CQuestItem *>::iterator itrItemCollectionMap = questInfo.m_pQuest->m_mapItemCollection.begin();
	for (; itrItemCollectionMap != questInfo.m_pQuest->m_mapItemCollection.end() ; itrItemCollectionMap++)
	{
		targetSend.m_nType = TARGET_ITEMCOLLECTION;
		targetSend.m_nIndex = itrItemCollectionMap->second->m_nItemID;
		targetSend.m_nCompletedCount = questInfo.GetTargetCompletedCountBy(TARGET_ITEMCOLLECTION, targetSend.m_nIndex);
		targetSend.m_nRequiredCount = itrItemCollectionMap->second->m_nItemCount;				
		if (targetSend.m_nCompletedCount > 0){
			m_listTargetInfo.push_back(targetSend);
		}
	}
	if (questInfo.m_pQuest->m_attrTarget.m_nTargetLevel > 0){	// 等级
		targetSend.m_nType = TARGET_LEVEL;
		targetSend.m_nIndex = 0;
		targetSend.m_nCompletedCount = MIN(role.GetLevel(), questInfo.m_pQuest->m_attrTarget.m_nTargetLevel);
		targetSend.m_nRequiredCount = questInfo.m_pQuest->m_attrTarget.m_nTargetLevel;
		if (targetSend.m_nCompletedCount > 0){
			m_listTargetInfo.push_back(targetSend);
		}
	}
	if (questInfo.m_pQuest->m_attrTarget.m_nTargetHotValue > 0){	// 人气
		targetSend.m_nType = TARGET_HOTVALUE;
		targetSend.m_nIndex = 0;
		targetSend.m_nCompletedCount = MIN(role.GetHot(), questInfo.m_pQuest->m_attrTarget.m_nTargetHotValue);
		targetSend.m_nRequiredCount = questInfo.m_pQuest->m_attrTarget.m_nTargetHotValue;
		if (targetSend.m_nCompletedCount > 0){
			m_listTargetInfo.push_back(targetSend);
		}
	}
	if (questInfo.m_pQuest->m_attrTarget.m_nTargetFriendNum > 0){	// 好友数
		targetSend.m_nType = TARGET_FRIENDCOUNT;
		targetSend.m_nIndex = 0;
		targetSend.m_nCompletedCount = MIN(role.GetFriendCount(), questInfo.m_pQuest->m_attrTarget.m_nTargetFriendNum);
		targetSend.m_nRequiredCount = questInfo.m_pQuest->m_attrTarget.m_nTargetFriendNum;
		if (targetSend.m_nCompletedCount > 0){
			m_listTargetInfo.push_back(targetSend);
		}
	}
}

GameMsg_S2C_AcceptQuestResult::~GameMsg_S2C_AcceptQuestResult()
{
}

bool GameMsg_S2C_AcceptQuestResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddUChar(m_nState);
	IOBuff.AddUShort((unsigned short)m_listTargetInfo.size());
	if (m_listTargetInfo.size() > 0){
		for (list<CQuestTargetSend>::iterator itr = m_listTargetInfo.begin(); itr != m_listTargetInfo.end(); ++itr){
			(*itr).doEncode(IOBuff);
		}
	}
    IOBuff.AddStringW(m_strJsonProgress);

	return true;
}

void GameMsg_S2C_AcceptQuestResult::SetListEntry(CQuestListEntry questEntry)
{
	m_nResult = 0;
	m_nState = questEntry.m_nState;
	m_nQuestID = questEntry.m_nQuestID;
	list<CQuestTargetSend>::iterator it = questEntry.m_listTargetInfo.begin();
	for (;it!=questEntry.m_listTargetInfo.end();it++)
	{
		m_listTargetInfo.push_back(*it);
	}
    m_strJsonProgress = questEntry.m_strJsonProgress;
}

// end of file


