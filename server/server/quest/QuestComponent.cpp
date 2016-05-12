#include "EntityQuestComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../socket/GameMsg_Map.h"
#include "../QuestListEntry.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"

#include "../../datastructure/ItemErrorInfo.h"
//#include "../../liblog/LogType.h"
#include "../../nsytworld/IQuestMan.h"
#include "../../datastructure/datainfor/DataManager.h"

#include "../GameMsg_C2S_GetQuestReward.h"
#include "../GameMsg_C2S_GetQuestList.h"
#include "../GameMsg_C2S_QuestRead.h"
#include "../GameMsg_C2S_AcceptQuest.h"

#include "../GameMsg_S2C_QuestStateChange.h"
#include "../GameMsg_S2C_GetQuestRewardResult.h"
#include "../GameMsg_S2C_GetQuestListResult.h"
#include "../GameMsg_S2C_AcceptQuestResult.h"


CEnityQuestComponent::CEnityQuestComponent():
	CEntityComponent(true, true)
{
	 m_pNet = NULL;
	 m_pItem = NULL;
	 m_pChat = NULL;
	 m_pAttr = NULL;
}

CEnityQuestComponent::~CEnityQuestComponent()
{

}

void CEnityQuestComponent::Start()
{
	m_pAttr = GetComponent<CRoleAttribute>();
	m_pNet  = GetComponent<CRoleNet>();
	m_pItem = GetComponent<CRoleItem>();
	m_pChat = GetComponent<CRoleChat>();
}

void CEnityQuestComponent::Register()
{
	m_pQuestMan = (IQuestMan *)&(CDataManager::GetInstance()->m_QuestMan);
	if (m_pQuestMan != NULL && m_pAttr != NULL)
	{
		m_pQuestMan->Register(m_pAttr->GetRoleID(), this);
	}
}

void CEnityQuestComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestReward);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestList);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuestRead);
	
	RegMsgProcessor(MSG_C2S_GetQuestReward, &CEnityQuestComponent::OnGetQuestReward);	
	RegMsgProcessor(MSG_C2S_GetQuestList, &CEnityQuestComponent::OnGetQuestList);
	RegMsgProcessor(MSG_C2S_QuestRead, &CEnityQuestComponent::OnQuestRead);

	// DB
	RegQueryProcessor(QUERY_QusetStateChange,&CEnityQuestComponent::UpdateQuestStateDB);
	RegQueryProcessor(QUERY_RemoveRoleQuest, &CEnityQuestComponent::RemoveQuestToDBCallBack);
	RegQueryProcessor(QUERY_AddRoleQuestHistory, &CEnityQuestComponent::AddQuestHistoryToDBCallBack);
	RegQueryProcessor(QUERY_UpdateQuestInfo, &CEnityQuestComponent::UpdateQuestInfoCallBack);
}

int CEnityQuestComponent::GetWeekDay()
{
	struct tm  *weekDay ;
	time_t now;
	time(&now);
	weekDay = localtime(&now);
	return weekDay->tm_wday;

}
void CEnityQuestComponent::OnLogin()
{
	RemoveExpiredQuest();
	PushPlayerQuest();
}
void CEnityQuestComponent::PushPlayerQuest()
{
	m_pQuestMan->PushPlayerQuest(this);
}

void CEnityQuestComponent::OnLogout()
{
	if (m_pAttr == NULL){
		return;
	}
	m_pQuestMan->Remove(m_pAttr->GetRoleID());
}

void CEnityQuestComponent::OnUpdate(const unsigned long &nTimeElapsed)
{

}

void CEnityQuestComponent::SetQuestMeetRequirement(QUESTINFO *pQuestInfo)
{
	if (m_pAttr == NULL){
		return;
	}
	SetQuestState(pQuestInfo, eQuestState_MeetRequirement);
	AddQuery(QUERY_QusetStateChange, m_pAttr->GetRoleID(), pQuestInfo, 0, NULL);
}

void CEnityQuestComponent::OnGetQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_GetQuestReward *pRewardMsg = (GameMsg_C2S_GetQuestReward *)&msg;

	if (pRewardMsg == NULL)
	{
		return;
	}
	if (!IsInQuestList(pRewardMsg->m_nQuestID))
	{
	 	m_pChat->SysChat2("您没有接受该任务，无法领取奖品。");
		return;
	}
	if (IsInHistoryList(pRewardMsg->m_nQuestID))
	{
		m_pChat->SysChat2("您已经领取了该任务奖励，无法再领取。");
		return;
	}
	QUESTINFO *pQuestInfo = GetQuestInfoBy(pRewardMsg->m_nQuestID);
	if (pQuestInfo != NULL)
	{
		if (!pQuestInfo->IsAcceptedState())
		{
			m_pChat->SysChat2("您没有接受该任务，无法获取奖品。");
			return;
		}
		if (!pQuestInfo->CanCompleteQuest())
		{
			m_pChat->SysChat2("您还无法完成该任务。");
			return;
		}
	}
	if (!CanAddItemPlus(pQuestInfo->m_pQuest->m_nRewrad.m_mapRewardItemPlus)
		|| !CanAddItemchoice(pQuestInfo->m_pQuest->m_nRewrad.m_map_RewardItemChoice, pRewardMsg->m_ItemChoiceID))
	{
		m_pChat->SysChat2("无法加入获奖物品， 请检查物品栏和物品。");
		return;
	}
	RemoveCollectionItem(pQuestInfo->m_nQuestID, pQuestInfo->m_pQuest->m_mapItemCollection);	
	GetReward(pRewardMsg->m_nQuestID, pRewardMsg->m_ItemChoiceID, &(pQuestInfo->m_pQuest->m_nRewrad));
	RemoveQuest(pQuestInfo->m_nQuestID);
	RemoveQuestToDB(pQuestInfo->m_nQuestID);
	AddQuestHistory(pQuestInfo->m_nQuestID);	
	AddQuestHistoryToDB(pQuestInfo->m_nQuestID);

	GameMsg_S2C_GetQuestRewardResult rewardResultMsg = GameMsg_S2C_GetQuestRewardResult(eGetQuestReward_Success);
	SendPlayerMsg(&rewardResultMsg);

	PushPlayerQuest();
}

void CEnityQuestComponent::OnGetQuestList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2C_GetQuestListResult GetListResultMsg;

	list<QUESTINFO>::iterator it = m_listQuest.begin();
	for (; it != m_listQuest.end(); it++)
	{
		CQuestListEntry listEntry(&(*it));	
		GetListResultMsg.m_listQuestInfo.push_back(listEntry);
	}

	SendPlayerMsg(&GetListResultMsg);
}

void CEnityQuestComponent::OnQuestRead(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	if (m_pAttr == NULL){
		return;
	}
	GameMsg_C2S_QuestRead *pQuestReadMsg = (GameMsg_C2S_QuestRead *)&msg;

	QUESTINFO *pQuestInfo = FindQuestInfoByID(pQuestReadMsg->m_nQuestID);
	if (pQuestInfo != NULL)
	{
		QUESTINFO *pQueryQuestInfo = new QUESTINFO;
		if (pQuestInfo != NULL)
		{
			*pQueryQuestInfo = *pQuestInfo;
			pQueryQuestInfo->SetState(eQuestState_HaveRead);
			AddQuery(QUERY_UpdateQuestInfo, m_pAttr->GetRoleID(), pQueryQuestInfo);
		}
	}
}

void CEnityQuestComponent::OnAcceptQuest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_AcceptQuest	*pAcceptMsg = (GameMsg_C2S_AcceptQuest *)&msg;
	unsigned short nQuestID = pAcceptMsg->m_nQuestID;

	QUESTINFO *pQuestInfo = FindQuestInfoByID(nQuestID);
	if (pQuestInfo != NULL && pQuestInfo->GetState() > eQuestState_Accepted)
	{
		m_pChat->SysChat2("您已经接受了该任务。");
		return;
	}
	if (!pQuestInfo->IsRepeatedQuest() && IsInHistoryList(nQuestID))
	{
		m_pChat->SysChat2("你已经完成了该任务，无法再接该任务。");
		return;
	}
	QUESTINFO *pQueryQuestInfo = new QUESTINFO;
	if (pQueryQuestInfo != NULL)
	{
		*pQueryQuestInfo = *pQuestInfo;
		SetQuestState(pQuestInfo, eQuestState_Accepted);
		AddQuery(QUERY_UpdateQuestInfo, m_pAttr->GetRoleID(), pQuestInfo);
		GameMsg_S2C_AcceptQuestResult resultMsg;
		SendPlayerMsg(&resultMsg);
	}
}

void CEnityQuestComponent::OnPlayerExitRoom()
{
	list<QUESTINFO>::iterator itr = m_listQuest.begin();	
	// each quest
	for (; itr != m_listQuest.end(); itr++)
	{
		list<TargetInfo > *pListMusicTarget = itr->GetTargetListBy(eTargetMusic);
		if (pListMusicTarget != NULL)
		{
			// each MusicTarget
			list<TargetInfo>::iterator itEntry = pListMusicTarget->begin();
			for (; itEntry != pListMusicTarget->end(); itEntry++)
			{
				if (itEntry->m_bContinued)
				{
					itEntry->m_bContinued = false;
				}
			}
		}
	}
}

void CEnityQuestComponent::RemoveCollectionItem(unsigned int nQuestID, map<unsigned short, CQuestItem *> &mapItemCollection)
{
	map<unsigned short, CQuestItem *>::iterator it = mapItemCollection.begin();
	for (; it != mapItemCollection.end(); it++)
	{
		if (it->second != NULL)
		{
			CQuestItem *pItemCollection = it->second;
			m_pItem->RemoveItemByType(pItemCollection->m_nItemID, pItemCollection->m_nItemCount, LOGTYPE_ITEM_RemoveByQuest,  nQuestID, false);
		}
	}
}

void CEnityQuestComponent::GetReward(unsigned int nQuestID, itemtype_t nItemChoice, CQuestReward *pReward)
{
	if (pReward == NULL)
	{
		return;
	}

	m_pAttr->AddExp(pReward->m_nRewardExp, 0, nQuestID);
	m_pAttr->ModifyMoney(pReward->m_nRewardMoney, 0, nQuestID);
	AddRewardItemPlus(pReward->m_mapRewardItemPlus);
	AddRewardItemChoice(pReward->m_map_RewardItemChoice, nItemChoice);
}

void CEnityQuestComponent::AddRewardItemPlus(map<unsigned short, CQuestItem *> &mapItemPlus)
{
	map<unsigned short, CQuestItem *>::const_iterator it = mapItemPlus.begin();

	for (; it != mapItemPlus.end(); it++ )
	{
		if (it->second != NULL)
		{
			CQuestItem *pItemPlus = it->second;
			m_pItem->AddItem(pItemPlus->m_nItemID, pItemPlus->m_nItemID, false, 0, LOGTYPE_ITEM_GetByQuest, false, false);
		}
	}
}

void CEnityQuestComponent::AddRewardItemChoice(map<itemtype_t, CQuestItem *> &mapItemChoice, itemtype_t nItemID)
{
	CQuestItem *pItemChoice = GetItemChoice(mapItemChoice, nItemID);

	if (pItemChoice != NULL)
	{
		m_pItem->AddItem(pItemChoice->m_nItemID, pItemChoice->m_nItemID, false, 0, LOGTYPE_ITEM_GetByQuest, false, false);
	}
}

void CEnityQuestComponent::UpdateQuestStateDB(CQuery &query)
{
}

void CEnityQuestComponent:: ItemUseChange(unsigned short nItemUseID, unsigned short nCount)
{
	if (nCount > 0)
	{
		UpdateItemUseTarget(nItemUseID, nCount);
		UpdateItemCollection(nItemUseID, -nCount);
		UpdateAllQuests();
	}
	else
	{
		WriteLog("ItemUse Error, nCount is a NEGATIVE number");
	}
}

void CEnityQuestComponent::ItemCollectionChange(unsigned short nItemCollectionID, unsigned short nCount)	// maybe可能任务物品被消耗，所以可能是负的
{
	UpdateItemCollection(nItemCollectionID, nCount);
	UpdateAllQuests();
}

void CEnityQuestComponent::MusicMatchChange(QUESTUPDATEINFO *pQuestUpdateInfo)
{
	if (pQuestUpdateInfo == NULL)
	{
		return;
	}
	UpdateMusicTarget(pQuestUpdateInfo);
	UpdateAllQuests();
}

bool CEnityQuestComponent::GetEnoughRequiredEquipedCloth(map<unsigned short, CQuestItem *> *pMapQuestMusicCloth)const
{
	if (pMapQuestMusicCloth == NULL)
	{
		return false;
	}
	map<unsigned short, CQuestItem *>::const_iterator itr = pMapQuestMusicCloth->begin();
	for (; itr != pMapQuestMusicCloth->end(); itr++)
	{
		if (m_pItem->SearchCurEquipedCloth(itr->second->m_nItemID) == NULL)
		{
			return false;
		}
	}

	return true;
}


void CEnityQuestComponent::UpdateMusicTarget(QUESTUPDATEINFO *pQuestUpdateInfo)
{
	if (pQuestUpdateInfo == NULL)
	{
		return;
	}
	
	list<QUESTINFO>::iterator itQuest = m_listQuest.begin();
	for (; itQuest != m_listQuest.end(); itQuest++)		// each QUESTINFO
	{
		list<TargetInfo> *pListMusic = itQuest->GetTargetListBy(eTargetMusic);
		if (pListMusic != NULL)
		{
			list<TargetInfo>::iterator itTargetList = pListMusic->begin();
			for (; itTargetList != pListMusic->end(); itTargetList++)	// each TargetInfo
			{
				if (itQuest->GetQuest()->CanCompleteMusicTarget(itTargetList->m_nIndex, pQuestUpdateInfo))
				{
					CQuestMusic *pQuestMusic = itQuest->GetQuest()->GetQuestMusic(itTargetList->m_nIndex);	// music target
					if (pQuestMusic == NULL)
					{
						return;
					}
					map<unsigned short, CQuestItem *> *pMusicCloth = &(pQuestMusic->m_mapCloth);
					if (pMusicCloth != NULL)
					{
						if (GetEnoughRequiredEquipedCloth(pMusicCloth))
						{
							if (pQuestMusic->m_bConRequired)	// 需连续
							{
								if (itTargetList->m_bContinued)
								{
									itTargetList->m_nCount++;
									itTargetList->m_nCount = itTargetList->m_nCount > pQuestMusic->m_nRequiredMatchNum ? pQuestMusic->m_nRequiredMatchNum : itTargetList->m_nCount;
								}
								else
								{
									itTargetList->m_bContinued = true;
									itTargetList->m_nCount = 1;
								}
							}
							else	// 无需连续
							{
								itTargetList->m_nCount++;
								itTargetList->m_nCount = itTargetList->m_nCount > pQuestMusic->m_nRequiredMatchNum ? pQuestMusic->m_nRequiredMatchNum : itTargetList->m_nCount;
							}
						}
					}
				}
			}
		}
	}
}


void CEnityQuestComponent::UpdateItemCollection(unsigned short nItemCollectionID, unsigned short nCount) // 可能任务物品被消耗，所以可能是负的
{
	list<QUESTINFO>::iterator itQuest = m_listQuest.begin();

	for (; itQuest != m_listQuest.end(); itQuest++)
	{
		list<TargetInfo> *pListItemCollection = itQuest->GetTargetListBy(eTargetItemCollection);		
		if (pListItemCollection != NULL)
		{
			list<TargetInfo>::iterator itTargetList = pListItemCollection->begin();
			for (; itTargetList != pListItemCollection->end(); itTargetList++)
			{
				if (nItemCollectionID == itTargetList->m_nIndex)
				{
					itTargetList->m_nCount = itTargetList->m_nCount+ nCount;
					if (itTargetList->m_nCount <= 0)
					{
						itTargetList->m_nCount = 0;
					}
					UpdateTargetToDB(&(*itQuest));	
				}
			}
		}
	}
}

void CEnityQuestComponent::UpdateItemUseTarget(unsigned short nItemUseID, unsigned short nCount)
{
	list<QUESTINFO>::iterator itQuest = m_listQuest.begin();

	for (; itQuest != m_listQuest.end(); itQuest++)
	{
		list<TargetInfo> *pListItemUse = itQuest->GetTargetListBy(eTargetItemUse);		
		if (pListItemUse != NULL)
		{
			list<TargetInfo>::iterator itTargetList = pListItemUse->begin();
			for (; itTargetList != pListItemUse->end(); itTargetList++)
			{
				if (nItemUseID == itTargetList->m_nIndex)
				{
					itTargetList->m_nCount = itTargetList->m_nCount + nCount;
					UpdateTargetToDB(&(*itQuest));
				}
			}
		}
	}
}

void CEnityQuestComponent::UpdateTargetToDB(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo == NULL)
	{
		WriteLog("UpdateTargetToDB failed.");
		return;
	}
	if (m_pAttr == NULL){
		return;
	}
	QUESTINFO *pQueryQuestInfo = new QUESTINFO;
	*pQueryQuestInfo = *pQuestInfo;
	AddQuery(QUERY_UpdateTargetInfo, m_pAttr->GetRoleID(), pQueryQuestInfo, 0, NULL);
}

void CEnityQuestComponent::UpdateAllQuests()
{
	list<QUESTINFO>::iterator it = m_listQuest.begin();

	for (; it != m_listQuest.end(); it++)
	{
		if (it->CanCompleteQuest())
		{
			// 任务状态设置为"完成"，含数据库
			SetQuestMeetRequirement(&(*it));
			GameMsg_S2C_QuestStateChange msg;
			SendPlayerMsg(&msg);
		}
	}
}

bool	CEnityQuestComponent::GetEnoughItem(map<unsigned short, CQuestItem *>& mapItem)
{
	map<unsigned short, CQuestItem *>::const_iterator it;	
	it = mapItem.begin();
	for (; it !=mapItem.end(); it++)
	{
		if (it->second != NULL)
		{
			if (it->second->m_nItemCount != 0 && it->second->m_nItemCount > GetItemCount(it->second->m_nItemID))
			{
				return false;
			}
		}
	}

	return true;
}

bool CEnityQuestComponent::CanAddItemPlus(map<unsigned short, CQuestItem *> &mapItemPlus)
{
	map<unsigned short, CQuestItem *>::iterator it = mapItemPlus.begin();

	for (; it != mapItemPlus.end(); it++)
	{
		if ((m_pItem->CanAddItem(it->second->m_nItemID, it->second->m_nItemCount, false, false)) != ItemUse_Success)
		{
			return false;
		}
	}

	return true;
}

CQuestItem *CEnityQuestComponent::GetItemChoice(map<unsigned short, CQuestItem *> &mapQuestItem, unsigned short nItemID)
{
	map<unsigned short, CQuestItem *>::iterator it = mapQuestItem.find(nItemID);	
	if (it != mapQuestItem.end())
	{
		return it->second;
	}

	return NULL;
}

bool CEnityQuestComponent::CanAddItemchoice(map<unsigned short, CQuestItem *> &mapQuestItem, unsigned short nItemID)
{
	CQuestItem *pQuestItem = GetItemChoice(mapQuestItem, nItemID);

	if (pQuestItem != NULL)
	{
		if ( (m_pItem->CanAddItem(pQuestItem->m_nItemID, pQuestItem->m_nItemCount, false, false)) != ItemUse_Success)
		{
			return false;
		}
	}

	return true;
}

unsigned int CEnityQuestComponent::GetItemCount(itemtype_t nItemID)
{
	return m_pItem->GetItemNumber(nItemID, true, true);
}

void CEnityQuestComponent::ProcessNewQuestDoc(CQuestDoc *pQuestDoc)
{
	if (pQuestDoc != NULL)
	{
		if (CanAcceptQuest(&(pQuestDoc->m_CQusetRequirement)))
		{
			QUESTINFO questInfo = QUESTINFO(pQuestDoc);

			AddNewQuestInfo(&questInfo);
		}
	}
}

void CEnityQuestComponent::SetQuestState(QUESTINFO *pQuestInfo, unsigned char nState)
{
	if (pQuestInfo != NULL)
	{
		pQuestInfo->SetState(nState);
	}
}

void CEnityQuestComponent::AddNewQuestInfo2DB(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo != NULL)
	{
		if (m_pAttr == NULL){
			return;
		}
		QUESTINFO* pQuestInfo = new	QUESTINFO;
		if (pQuestInfo != NULL)
		{
			*pQuestInfo = *pQuestInfo;
			AddQuery(QUERY_AddRoleQuest, m_pAttr->GetRoleID(), pQuestInfo, 0, NULL);
		}
	}
}

void CEnityQuestComponent::AddNewQuestInfo(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo != NULL)
	{
		list<QUESTINFO>::const_iterator it; 
		it = find(m_listQuest.begin(), m_listQuest.end(), *pQuestInfo);
		if (it == m_listQuest.end())
		{
			m_listQuest.push_back(*pQuestInfo);
			AddNewQuestInfo2DB(pQuestInfo);

			//if (pQuestInfo->CanCompleteQuest())	// 接收任务即完成
			//{
				// 任务状态设置为"完成"，含数据库
			//	SetQuestMeetRequirement(pQuestInfo);
			//}
			GameMsg_S2C_QuestStateChange msg;
			SendPlayerMsg(&msg);
		}
	}
}

bool CEnityQuestComponent::CanAcceptQuest(CQuestRequirement *pQuestRequirement)
{
	if (pQuestRequirement == NULL || m_pAttr == NULL)
	{
		return false;
	}
	if (pQuestRequirement->m_nLevel != 0 && pQuestRequirement->m_nLevel > m_pAttr->GetLevel())
	{
		return false;
	}
	if (pQuestRequirement->m_nSex != 0 && pQuestRequirement->m_nSex != m_pAttr->GetSex())
	{
		return false;
	}
	if (pQuestRequirement->m_nHotValue != 0 && pQuestRequirement->m_nHotValue > m_pAttr->GetHot())
	{
		return false;
	}
	//if (pQuestRequirement->m_nTitleID != 0 && pQuestRequirement->m_nTitleID != )
	//{
	//  return false;
	//}
	//if (pQuestRequirement->m_nPartyLevel != 0 && pQuestRequirement->m_nPartyLevel >)
	//{
	//	return false;
	//}
	// m_nGroupLevel	
	// m_nMarriageLevel
	if (pQuestRequirement->m_nWeekDay != 0 && (pQuestRequirement->m_nWeekDay - 1) != GetWeekDay() )	// 文本数据 0表示整个星期，1表示星期一，类推
	{
		return false;
	}
	//__int64			m_nBeginTime;
	//__int64			m_nEndTime;
	if (time(NULL) < pQuestRequirement->m_nBeginTime && pQuestRequirement->m_nEndTime < time(NULL) && pQuestRequirement->m_nEndTime != 0 )
	{
		return false;
	}
	//unsigned short	m_nPreviousQusetID;
	if (!IsInHistoryList(pQuestRequirement->m_nPreviousQusetID))
	{
		return false;
	}
	//unsigned short	m_nExclusiveQustID;	
	if (IsInHistoryList(pQuestRequirement->m_nExclusiveQustID))
	{
		return false;
	}
	//map<unsigned short, CQuestItem *> m_mapRequredItem 物品要求
	if (!GetEnoughItem(pQuestRequirement->m_mapRequredItem))
	{
		return false;
	}

	return true;
}

bool CEnityQuestComponent::IsInHistoryList(unsigned int nQuestID)
{
	list<QUESTHISTORYINFO>::iterator it;
	it  = find(m_listQuestHistory.begin(), m_listQuestHistory.end(), nQuestID);

	return it != m_listQuestHistory.end(); 
}

bool CEnityQuestComponent::IsInQuestList(unsigned int nQuestID)
{
	list<QUESTINFO>::iterator it;
	it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);

	return it != m_listQuest.end();
}

QUESTINFO *CEnityQuestComponent::GetQuestInfoBy(unsigned int nQuestID)
{
	list<QUESTINFO>::iterator it;

	it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);

	if (it != m_listQuest.end())
	{
		return &(*it);
	}

	return NULL;
}

void CEnityQuestComponent::OnEvent(CComponentEvent& refEvent )
{
	if (refEvent.nID == emNormalLogin)
	{
		OnLogin();
	}
}

bool CEnityQuestComponent::CreateFromDB(const ROLEINFO_DB* pRoleInfoDB)
{
	m_listQuest = pRoleInfoDB->listQuest;
	m_listQuestHistory = pRoleInfoDB->listQuestHistory;

	return true;
}

void CEnityQuestComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pNet != NULL && pMsg != NULL)
	{
		m_pNet->SendPlayerMsg(pMsg);
	}
}

void CEnityQuestComponent::RemoveQuest(unsigned int nQuestID)
{
	list<QUESTINFO>::iterator itr = m_listQuest.begin();

	for (; itr != m_listQuest.end(); itr++)
	{
		if (itr->m_nQuestID == nQuestID)
		{
			m_listQuest.erase(itr);
		}
	}
}

void CEnityQuestComponent::RemoveQuestToDB(unsigned int nQuestID)
{
	if (m_pAttr != NULL)
	{
		AddQuery(QUERY_RemoveRoleQuest, m_pAttr->GetRoleID(), NULL, nQuestID, NULL);
	}
}

void CEnityQuestComponent::RemoveQuestToDBCallBack(CQuery &query)
{
	if (query.m_nResult != 0)
	{
		WriteLog("Remove Quest Targets failed.");
	}
}

void CEnityQuestComponent::AddQuestHistory(unsigned int nQuestID)	
{
	QUESTHISTORYINFO questHistory =  QUESTHISTORYINFO(nQuestID, (unsigned int)time(NULL)); // time_t 强转
	m_listQuestHistory.push_back(questHistory);
}

void CEnityQuestComponent::AddQuestHistoryToDB(unsigned int nQuestID)
{
	if (m_pAttr != NULL)
	{

		list<QUESTHISTORYINFO>::iterator itr;
		itr = find(m_listQuestHistory.begin(), m_listQuestHistory.end(), nQuestID);

		if (itr != m_listQuestHistory.end())
		{
			QUESTHISTORYINFO *pHistoryQuest= new QUESTHISTORYINFO;
			*pHistoryQuest = *itr;

			if (pHistoryQuest != NULL)
			{
				AddQuery(QUERY_AddRoleQuestHistory, m_pAttr->GetRoleID(), pHistoryQuest);
			}
		}
	}
}

void CEnityQuestComponent::AddQuestHistoryToDBCallBack(CQuery &query)
{
	if (query.m_nResult != 0)
	{
		WriteLog("Add QuestHistory failed.");
	}
}

void CEnityQuestComponent::UpdateQuestInfoCallBack(CQuery &query)
{
	if (query.m_nResult != 0)
	{
		WriteLog("UpdateQuestInfo failed.");
	}
}

void CEnityQuestComponent::RemoveExpiredQuest()
{
	list<QUESTINFO>::iterator itr = m_listQuest.begin(); 

	for (; itr != m_listQuest.end(); )
	{
		CQuestDoc* pQuest = itr->GetQuest();
		if (pQuest == NULL)
		{
			itr++;
			continue;
		}
		if (pQuest->IsDailyQuest())
		{
			time_t  acceptTime = (time_t)itr->GetAcceptTime();

			if (!IsTimeIsToday(acceptTime))
			{
				itr = m_listQuest.erase(itr);
				RemoveQuestToDB(itr->m_nQuestID);
			}
			else
			{
				itr++;
			}
		}
	}
}

QUESTINFO *CEnityQuestComponent::FindQuestInfoByID(unsigned int nQuestID)
{
	list<QUESTINFO>::iterator itr = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);

	if (itr != m_listQuest.end())
	{
		return &(*itr);
	}

	return NULL;
}


// end of file

