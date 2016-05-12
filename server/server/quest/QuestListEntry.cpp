#include "QuestListEntry.h"
#include "../../nsytworld/QuestDoc.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/RoleEntity.h"

CQuestListEntry::CQuestListEntry()
{
	m_nQuestID = 0;
	m_nState = 0;
}

CQuestListEntry::CQuestListEntry(QUESTINFO *pQuestInfo, CRoleEntity* pRole)
{
	if (pQuestInfo != NULL && pRole != NULL && pQuestInfo->IsValid())
	{
		m_nQuestID = (unsigned short)pQuestInfo->m_nQuestID;
		m_nState = pQuestInfo->m_nState;

		CQuestTargetSend targetSend;
		map< unsigned short, CQuestMusic * >::iterator itrMusicMap = pQuestInfo->m_pQuest->m_mapMusicTarget.begin();
		for (; itrMusicMap != pQuestInfo->m_pQuest->m_mapMusicTarget.end(); itrMusicMap++)
		{
			targetSend.m_nType = TARGET_MUSIC;
			targetSend.m_nIndex = itrMusicMap->second->m_nMusicID;
			targetSend.m_nCompletedCount = pQuestInfo->GetTargetCompletedCountBy(TARGET_MUSIC, targetSend.m_nIndex);
			targetSend.m_nRequiredCount = itrMusicMap->second->m_nRequiredMatchNum;
			m_listTargetInfo.push_back(targetSend);
		}
		map <itemtype_t, CQuestItem *>::iterator itrItemUseMap = pQuestInfo->m_pQuest->m_mapItemUse.begin();
		for (; itrItemUseMap != pQuestInfo->m_pQuest->m_mapItemUse.end() ; itrItemUseMap++)
		{
			targetSend.m_nType = TARGET_ITEMUSE;
			targetSend.m_nIndex = itrItemUseMap->second->m_nItemID;
			targetSend.m_nCompletedCount = pQuestInfo->GetTargetCompletedCountBy(TARGET_ITEMUSE, targetSend.m_nIndex);
			targetSend.m_nRequiredCount = itrItemUseMap->second->m_nItemCount;				
			m_listTargetInfo.push_back(targetSend);
		}
		map <itemtype_t, CQuestItem *>::iterator itrItemCollectionMap = pQuestInfo->m_pQuest->m_mapItemCollection.begin();
		for (; itrItemCollectionMap != pQuestInfo->m_pQuest->m_mapItemCollection.end() ; itrItemCollectionMap++)
		{
			targetSend.m_nType = TARGET_ITEMCOLLECTION;
			targetSend.m_nIndex = itrItemCollectionMap->second->m_nItemID;
			targetSend.m_nCompletedCount = pQuestInfo->GetTargetCompletedCountBy(TARGET_ITEMCOLLECTION, targetSend.m_nIndex);
			targetSend.m_nRequiredCount = itrItemCollectionMap->second->m_nItemCount;				
			m_listTargetInfo.push_back(targetSend);
		}
		if (pQuestInfo->m_pQuest->m_attrTarget.m_nTargetLevel > 0){	// 等级
			targetSend.m_nType = TARGET_LEVEL;
			targetSend.m_nIndex = 0;
			targetSend.m_nCompletedCount = pQuestInfo->IsAccepted() ? MIN(pRole->GetLevel(), pQuestInfo->m_pQuest->m_attrTarget.m_nTargetLevel) : 0;
			targetSend.m_nRequiredCount = pQuestInfo->m_pQuest->m_attrTarget.m_nTargetLevel;
			m_listTargetInfo.push_back(targetSend);
		}
		if (pQuestInfo->m_pQuest->m_attrTarget.m_nTargetHotValue > 0){	// 人气
			targetSend.m_nType = TARGET_HOTVALUE;
			targetSend.m_nIndex = 0;
			targetSend.m_nCompletedCount = pQuestInfo->IsAccepted() ? MIN(pRole->GetHot(),pQuestInfo->m_pQuest->m_attrTarget.m_nTargetHotValue) : 0; 
			targetSend.m_nRequiredCount = pQuestInfo->m_pQuest->m_attrTarget.m_nTargetHotValue;
			m_listTargetInfo.push_back(targetSend);
		}
		if (pQuestInfo->m_pQuest->m_attrTarget.m_nTargetFriendNum > 0){	// 好友数
			targetSend.m_nType = TARGET_FRIENDCOUNT;
			targetSend.m_nIndex = 0;
			targetSend.m_nCompletedCount = pQuestInfo->IsAccepted() ? MIN(pRole->GetFriendCount(), pQuestInfo->m_pQuest->m_attrTarget.m_nTargetFriendNum) : 0;
			targetSend.m_nRequiredCount = pQuestInfo->m_pQuest->m_attrTarget.m_nTargetFriendNum;
			m_listTargetInfo.push_back(targetSend);
		}
	}
}

CQuestListEntry::~CQuestListEntry()
{
}

void CQuestListEntry::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nQuestID);	
	IOBuff.AddUChar(m_nState);
	IOBuff.AddUChar((unsigned char)m_listTargetInfo.size());
	if (m_listTargetInfo.size() > 0)
	{
		list<CQuestTargetSend>::iterator itr = m_listTargetInfo.begin();
		for (; itr != m_listTargetInfo.end(); itr++)
		{
			itr->doEncode(IOBuff);
		}
	}
    IOBuff.AddStringW(m_strJsonProgress);
}



// end of file


