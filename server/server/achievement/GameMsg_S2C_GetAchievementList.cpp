#include "GameMsg_S2C_GetAchievementList.h"
#include "../../datastructure/datainfor/AchievementConfig.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "EntityAchievementComponent.h"
#include "RequirementHandlerManager.h"
#include "../share/ServerMsgDef.h"

#include <algorithm>


CAchievementRewardItem::CAchievementRewardItem(itemtype_t nItemID, unsigned short nCount)
{
	m_nItemID = nItemID;		
	m_nCount = nCount;
}
CAchievementRewardItem::~CAchievementRewardItem()
{
}

void CAchievementlistEntry::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nAchievementID);
	IOBuff.AddUInt(m_nCompletedTime);
	IOBuff.AddUInt(m_nCurrentValue);
	IOBuff.AddUChar(m_nState);
}

CAchievementlistEntry::~CAchievementlistEntry()
{
}

CAchievementlistEntry::CAchievementlistEntry(unsigned short nAchievementID, unsigned short nSeriesID, unsigned int nCurrentValue, unsigned int nTime, unsigned char nState):
m_nAchievementID(nAchievementID),
m_nSeriesID(nSeriesID),
m_nCompletedTime(nTime), 
m_nCurrentValue(nCurrentValue), 
m_nState(nState)
{
}

bool CAchievementlistEntry::operator==(const CAchievementlistEntry & rhs)
{
	if (this == &rhs)
	{
		return true;
	}

	return m_nSeriesID == rhs.m_nSeriesID;
}


GameMsg_S2C_GetAchievementList::GameMsg_S2C_GetAchievementList(const CEntityAchievementComponent *pAchievement):
GameMsg_Base(MSG_S2C_GetAchievementList)
{
	if (pAchievement != NULL)
	{
		const list<CAchievementInfo> & HistoryAchList = pAchievement->m_listAchievement;
		map<unsigned short, list<AchievementConfig *> >::const_iterator mapItr = ConfigManager::Instance().GetAchievementManager().m_mapAchievementBySeriesID.begin();	
		for (; mapItr != ConfigManager::Instance().GetAchievementManager().m_mapAchievementBySeriesID.end(); ++mapItr) {
			list<AchievementConfig *>::const_iterator listItr = mapItr->second.begin();
			for (; listItr != mapItr->second.end(); ++listItr) {
				if (*listItr != NULL/* && bEnter*/) {
					const IRequirementHandler *pHandler = CRequirementHandlerManager::Instance().GetHandler( (*listItr)->GetHanlerID());
					if (pHandler != NULL && pHandler->GetCurrentValue(pAchievement) > 0) {
						list<CAchievementInfo>::const_iterator listHistoryItr = find(HistoryAchList.begin(), HistoryAchList.end(), (*listItr)->GetAchievementID());
						unsigned char nState = listHistoryItr != HistoryAchList.end() ? listHistoryItr->m_nState : (unsigned char)AchievementProcessing;
						unsigned int nTime = listHistoryItr != HistoryAchList.end() ? (unsigned int)listHistoryItr->GetCompletedTime() : 0;
						unsigned int nCount = MIN(pHandler->GetCurrentValue(pAchievement), (*listItr)->GetRequireValue());
						CAchievementlistEntry entry = CAchievementlistEntry((*listItr)->GetAchievementID(), (*listItr)->GetSeriesID(),nCount, nTime, nState);
						m_listAchievement.push_back(entry);
						if (pHandler->GetCurrentValue(pAchievement) < (*listItr)->GetRequireValue()) {
							break;
						}
					}

				}
			}
		}

	//	const list<CAchievementInfo> & HistoryAchList = pAchievement->m_listAchievement;
	//	map<unsigned short, CAchievementDoc *>::iterator itr =	 CDataManager::Instance().GetAchievementManager().m_mapAchievement.begin();	
	//	for (; itr !=  CDataManager::Instance().GetAchievementManager().m_mapAchievement.end(); itr++)
	//	{
	//		if (itr->second != NULL)
	//		{
	//			const IRequirementHandler *pHandler = CRequirementHandlerManager::Instance().GetHandler( itr->second->GetHanlerID());
	//			if (pHandler != NULL && pHandler->GetCurrentValue(pAchievement) > 0)
	//			{
	//				list<CAchievementInfo>::const_iterator listItr = find(HistoryAchList.begin(), HistoryAchList.end(), itr->second->GetAchievementID());
	//				unsigned char nState = listItr != HistoryAchList.end() ? listItr->m_nState : AchievementProcessing;
	//				unsigned int nTime = listItr != HistoryAchList.end() ? (unsigned int)listItr->GetCompletedTime() : 0;	
	//				unsigned int nCount = MIN(pHandler->GetCurrentValue(pAchievement), itr->second->GetRequireValue());
	//				CAchievementlistEntry entry = CAchievementlistEntry(itr->second->GetAchievementID(), itr->second->GetSeriesID(),nCount, nTime, nState);
	//				//if (m_listAchievement.end() == find(m_listAchievement.begin(), m_listAchievement.end(), entry) )
	//				//{
	//					m_listAchievement.push_back(entry);
	//				//}
	//			}
	//		}
	//	}

	}
}

GameMsg_S2C_GetAchievementList::GameMsg_S2C_GetAchievementList():GameMsg_Base(MSG_S2C_GetAchievementList)
{
}

GameMsg_S2C_GetAchievementList::~GameMsg_S2C_GetAchievementList()
{
}

bool GameMsg_S2C_GetAchievementList::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_listAchievement.size());
	if (m_listAchievement.size() > 0)
	{
		for (list<CAchievementlistEntry>::iterator itr = m_listAchievement.begin(); itr != m_listAchievement.end(); itr++) 
		{
			itr->doEncode(IOBuff);
		}
	}

	return true;
}


//end file


