#include "EntityAchievementComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../datastructure/ItemErrorInfo.h"
#include "../../socket/GameMsg_Map.h"
#include "../util/CommonFunc.h"
//#include "../../liblog/LogType.h"

#include "../../datastructure/datainfor/ConfigManager.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "RequirementHandlerManager.h"

#include "GameMsg_C2S_GetAchievementList.h"
#include "GameMsg_C2S_AchievementGetReward.h"

#include "GameMsg_S2C_AchievementCompleted.h"
#include "GameMsg_S2C_GetAchievementList.h"
#include "GameMsg_S2C_AchievementGetRewardResult.h"

#include <algorithm>


CEntityAchievementComponent::CEntityAchievementComponent():
CEntityComponent(true, true)
{
	m_pRoleAttr = NULL;
	m_pRoleNet = NULL;
	m_pRoleItem = NULL;
	m_pRoleChat = NULL;
	m_pRoleQuest = NULL;
	m_pRoleCheck = NULL;

	m_nLastUpdateTime = 0;
	m_nLastSpecialCount = 0;
}

CEntityAchievementComponent::~CEntityAchievementComponent()
{
}

void CEntityAchievementComponent::OnEvent(CComponentEvent& refEvent )
{
	unsigned int nNow = (unsigned int)time(NULL);
	if (refEvent.nID == eComponentEvent_TodayFirstLogin)	{// 记录一周内的事件
		if (GetWeekDay() == MONDAY || GetLocalDays(m_BigSpeakersUseCountWeekly.m_nLastEventTime) / WEEKDAYS != GetLocalDays(nNow) / WEEKDAYS ) {
			m_BigSpeakersUseCountWeekly.Clear();	
		}
		if (GetWeekDay() == MONDAY || GetLocalDays(m_SmallSpeakersUseCountWeekly.m_nLastEventTime) / WEEKDAYS != GetLocalDays(nNow) / WEEKDAYS) {
			m_SmallSpeakersUseCountWeekly.Clear();
		}
		if (GetWeekDay() == MONDAY || GetLocalDays(m_SendFlowersCountWeekly.m_nLastEventTime) / WEEKDAYS != GetLocalDays(nNow) / WEEKDAYS) {
			m_SendFlowersCountWeekly.Clear();
		}
	}
}

void CEntityAchievementComponent::Start()
{
	m_pRoleAttr = GetComponent<CRoleAttribute>();
	m_pRoleNet  = GetComponent<CRoleNet>();
	m_pRoleItem = GetComponent<CRoleItem>();
	m_pRoleChat = GetComponent<CRoleChat>();
	m_pRoleQuest = GetComponent<CRoleQuest>();
	m_pRoleCheck = GetComponent<CRoleCheckIn>();
}

void CEntityAchievementComponent::OnLogin()	
{
}

void CEntityAchievementComponent::OnLogout()	
{
}

void CEntityAchievementComponent::SerializeComponent(CParamPool &IOBuff)
{
	unsigned int nScore = 0;	
	unsigned short nSize= (unsigned short)m_listAchievement.size();
	IOBuff.AddUShort(nSize);
	if (nSize > 0) {
		for (list<CAchievementInfo>::iterator itr = m_listAchievement.begin(); itr != m_listAchievement.end(); itr++) {
			IOBuff.AddUShort(itr->m_nAchievementID);	
			IOBuff.AddUInt(itr->m_nCompletedTime);
			IOBuff.AddUChar(itr->m_nState);
			if (itr->GetAchievementDoc() != NULL) {
				nScore += itr->GetAchievementDoc()->GetScore();
			}
		}
	}

	IOBuff.AddUInt(nScore);
}

void CEntityAchievementComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
	time_t nNow = time(NULL);
	if (nNow > (time_t)m_nLastUpdateTime && GetLocalHourOfDay(nNow) == 0 && GetLocalHourOfDay(m_nLastUpdateTime) == 23) {//每天凌晨检测一次
		if (GetWeekDay() == MONDAY) {
			m_BigSpeakersUseCountWeekly.Clear();
			m_SmallSpeakersUseCountWeekly.Clear();
			m_SendFlowersCountWeekly.Clear();
		}
		m_nLastUpdateTime = (unsigned int)nNow;
	}
}

void CEntityAchievementComponent::RegComponentNetMsgMap()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetAchievementList);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AchievementGetReward);

	RegMsgProcessor(MSG_C2S_GetAchievementList, &CEntityAchievementComponent::OnGetAchievementList);	
	RegMsgProcessor(MSG_C2S_AchievementGetReward, &CEntityAchievementComponent::OnGetAchievementReward);	
}

bool CEntityAchievementComponent::CreateFromDB( ROLEINFO_DB* pRoleInfoDB)
{
	// 成就数据相关的数据
	m_nHistoryClothNum = pRoleInfoDB->nHistoryClothNum;
	m_nHistoryWishItemNum = pRoleInfoDB->nHistoryWishItemNum;
	m_nGainWishItemNum = pRoleInfoDB->nGainWishItemNum;
	m_nHistoryBadageNum = pRoleInfoDB->nHistoryBadageNum;
	m_nHistoryUseBouquetNum = pRoleInfoDB->nHistoryUseBouquetNum;

	m_BigSpeakersUseCountWeekly = pRoleInfoDB->bigSpeakersUseNumWeekly;
	m_SmallSpeakersUseCountWeekly = pRoleInfoDB->smallSpeakersUseNumWeekly;
	m_SendFlowersCountWeekly = pRoleInfoDB->sendFlowersNumInSevenWeekly;

	// 比赛成绩相关
	//m_nHistoryTotalScore = pRoleInfoDB->nHistoryTotalScore;
	m_nHistoryTaikoComboMusicNum = pRoleInfoDB->nHistoryTaikoComboMusicNum;
	m_nAutoMatchWithOppositeSexNum = pRoleInfoDB->nAutoMatchWithOppositeSexNum;
	m_mapFightResult = pRoleInfoDB->mapFightResult;
	// 称号
	m_listTitle = pRoleInfoDB->listTitle;
	// 成就
	m_listAchievement = pRoleInfoDB->listAchievement;
	for (list<CAchievementInfo>::iterator itr = m_listAchievement.begin(); itr != m_listAchievement.end();) {
		itr->SetAchievementDoc(ConfigManager::Instance().GetAchievementManager().GetByID((unsigned short)itr->m_nAchievementID));

		if (itr->GetAchievementDoc() == NULL) {
			itr = m_listAchievement.erase(itr);
		}
		else {
			m_nScore += itr->GetAchievementDoc()->GetScore();
			itr++;
		}
	}

	return true;
}

bool CEntityAchievementComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
	if (pRoleInforDB != NULL) {
		pRoleInforDB->nHistoryClothNum = m_nHistoryClothNum;
		pRoleInforDB->nHistoryWishItemNum = m_nHistoryWishItemNum;
		pRoleInforDB->nGainWishItemNum = m_nGainWishItemNum;
		pRoleInforDB->nHistoryBadageNum = m_nHistoryBadageNum;
		pRoleInforDB->nHistoryUseBouquetNum = m_nHistoryUseBouquetNum;

		pRoleInforDB->bigSpeakersUseNumWeekly = m_BigSpeakersUseCountWeekly;
		pRoleInforDB->smallSpeakersUseNumWeekly = m_SmallSpeakersUseCountWeekly;
		pRoleInforDB->sendFlowersNumInSevenWeekly = m_SendFlowersCountWeekly;

		pRoleInforDB->nHistoryTaikoComboMusicNum = m_nHistoryTaikoComboMusicNum;
		pRoleInforDB->nAutoMatchWithOppositeSexNum = m_nAutoMatchWithOppositeSexNum;

		pRoleInforDB->mapFightResult = m_mapFightResult;

		// title
		pRoleInforDB->listTitle = m_listTitle;
		// achievement
		pRoleInforDB->listAchievement = m_listAchievement;


		return true;
	}

	return false;
}

bool CEntityAchievementComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    // 其他数据大部分是存储在event表中，event会固定从数据库中加载
    pRoleInfoCache->mapFightResult = m_mapFightResult;
    pRoleInfoCache->listTitle = m_listTitle;
    pRoleInfoCache->listAchievement = m_listAchievement;

    return true;
}


void CEntityAchievementComponent::SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pRoleNet != NULL && pMsg != NULL) {
		m_pRoleNet->SendPlayerMsg(pMsg);
	}
}

void CEntityAchievementComponent::OnGetAchievementList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_S2C_GetAchievementList listMsg = GameMsg_S2C_GetAchievementList (this);

	SendPlayerMsg(&listMsg);
}

void CEntityAchievementComponent::OnGetAchievementReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_AchievementGetReward *pClientMsg = (GameMsg_C2S_AchievementGetReward  *)&msg;
	CAchievementInfo *pInfo = GetAchievementInfoBy(pClientMsg->m_nAchievementID);
	AchievementConfig *pDoc = ConfigManager::Instance().GetAchievementManager().GetByID(pClientMsg->m_nAchievementID);
	if (pInfo != NULL && pDoc != NULL && m_pRoleAttr != NULL && m_pRoleItem != NULL && pInfo->m_nState != AchievementGotReward) {// 未领奖
		GameMsg_S2C_AchievementGetRewardResult sendMsg(pClientMsg->m_nAchievementID, 0);
		const IRequirementHandler *pHandler = CRequirementHandlerManager::Instance().GetHandler(pDoc->GetHanlerID());
		if (pHandler != NULL && pHandler->CanMeetRequirement(pClientMsg->m_nAchievementID, this)) {
			list<CItem> listItem;
			for (list<AchievementItemReward>::iterator itr = pDoc->m_listRewardItem.begin();  itr != pDoc->m_listRewardItem.end(); itr++) {
				CItem item(itr->m_nItemID, itr->m_nItemCount, 0, false);
				listItem.push_back(item);
			}
			if (m_pRoleItem->CanAddAllItem(listItem) == EItemError_Success) {
				for (list<AchievementItemReward>::iterator itr = pDoc->m_listRewardItem.begin();  itr != pDoc->m_listRewardItem.end(); itr++) 
				{
					GetRoleItem()->AddItem(itr->GetItemID(), itr->GetItemCount(), 0, EItemAction_Add_Achievement, pClientMsg->m_nAchievementID, true, false);
				}
				// 获得称号
				if (!IsInTitleList(pDoc->GetRewardTitleID())) {
					AddTitle(pDoc->GetRewardTitleID());
					AddTitleToDB(pDoc->GetRewardTitleID());
					UpdateAchievement(AchievementType_HistoryTitleCount);
				}
				pInfo->m_nState = AchievementGotReward;
				sendMsg.m_nResult = 1;
				UpdateAchievementStateToDB(GetRoleAttr()->GetRoleID(), pClientMsg->m_nAchievementID, AchievementGotReward);
				_LogRoleAchievement(m_pRoleAttr->GetRoleID(), CRoleAchievementLog::EAchievementAction_GetReward, pClientMsg->m_nAchievementID);
			}
		}
		SendPlayerMsg(&sendMsg);
	}
}

bool CEntityAchievementComponent::IsInAchievementList(unsigned int nAchievementID)
{
	return (m_listAchievement.end() != find(m_listAchievement.begin(), m_listAchievement.end(), nAchievementID));
}

bool CEntityAchievementComponent::IsInTitleList(unsigned short nTitleID)
{
	return (m_listTitle.end() != find(m_listTitle.begin(), m_listTitle.end(), nTitleID));
}

CAchievementInfo *CEntityAchievementComponent::GetAchievementInfoBy(unsigned int nAchievementID)
{
	list<CAchievementInfo>::iterator itr = find(m_listAchievement.begin(), m_listAchievement.end(), nAchievementID);

	if (itr != m_listAchievement.end()) {
		return &(*itr);
	}

	return NULL;
}

bool CEntityAchievementComponent::CanMeetAchivementCriteria(unsigned short nAchievementID)
{
	if (nAchievementID > 0)
	{
		AchievementConfig *pDoc = ConfigManager::Instance().GetAchievementManager().GetByID(nAchievementID);
		if (pDoc != NULL) {
			if (!IsInAchievementList(nAchievementID)) {
				IRequirementHandler *pHandler = 	CRequirementHandlerManager::Instance().m_ppRequirementHandler[pDoc->GetHanlerID()];
				if (pHandler != NULL) {
					return pHandler->CanMeetRequirement(nAchievementID, this);
				}
			}
		}
	}

	return false;
}


void CEntityAchievementComponent::AddNewAchievement(unsigned short nAchievementID, unsigned int nCompletedTime, AchievementConfig *pAchievementDoc)
{
	if (nAchievementID > 0 && pAchievementDoc != NULL) {
		AddAchivement(nAchievementID, nCompletedTime, pAchievementDoc, AchievementCompleted);
		AddAchievementToDB(nAchievementID, nCompletedTime);
		_LogRoleAchievement(m_pRoleAttr->GetRoleID(), CRoleAchievementLog::EAchievementAction_Complete, nAchievementID);
	}
}

void CEntityAchievementComponent::ItemUse(itemtype_t nItemType, unsigned short nCount)
{
	unsigned int nNow = (unsigned int)time(NULL);
	if (nItemType == ITEMID_BIGSPEAKER)	{// 一周内使用的大喇叭
		m_BigSpeakersUseCountWeekly.m_nCount = m_BigSpeakersUseCountWeekly.m_nCount + nCount;
		m_BigSpeakersUseCountWeekly.m_nLastEventTime = nNow;
		UpdateAchievement(AchievementType_HistoryBigSpeakersUseInSevenDaysCount);
	}
	if (nItemType == ITEMID_SMALLSPEAKER) {// 一周内使用的小喇叭
		m_SmallSpeakersUseCountWeekly.m_nCount = m_SmallSpeakersUseCountWeekly.m_nCount + nCount;
		m_SmallSpeakersUseCountWeekly.m_nLastEventTime = nNow;
		//UpdateAchievement(AchievementType_HistoryGainWishItemCount);
	}
	if (IsFlower(nItemType )) {	// 一周内献花数量 
		m_SendFlowersCountWeekly.m_nCount = m_SendFlowersCountWeekly.m_nCount + nCount;
		m_SendFlowersCountWeekly.m_nLastEventTime = nNow;
		UpdateAchievement(AchievementType_HistorySendFlowersCountInSevenDays);
	}
	if (nItemType == ITEMID_BOUQUET) {	// 累计使用的花束
		m_nHistoryUseBouquetNum = m_nHistoryUseBouquetNum + nCount;
		UpdateAchievement(AchievementType_HistoryUseBouquetCount);
	}
}

void CEntityAchievementComponent::UpdateAchievement(unsigned short nSeiriesID)
{
	map<unsigned short, list<AchievementConfig *> >::iterator mapItr = ConfigManager::Instance().GetAchievementManager().m_mapAchievementBySeriesID.find(nSeiriesID);

	if (mapItr != ConfigManager::Instance().GetAchievementManager().m_mapAchievementBySeriesID.end()) {
		for (list<AchievementConfig *>::iterator i = mapItr->second.begin(); i != mapItr->second.end(); ++i ) {
			if (IsInAchievementList((*i)->GetAchievementID())) {
				continue;
			} else if (CanMeetAchivementCriteria((*i)->GetAchievementID())) {
				unsigned int nCompletedTime = (unsigned int)time(NULL);
				AddNewAchievement((*i)->GetAchievementID(), nCompletedTime, *i);
				UpdateAchievement(AchievementType_HistoryCompletedAchievementCount);// 递归调用
				GameMsg_S2C_AchievementCompleted msg = GameMsg_S2C_AchievementCompleted((*i)->GetAchievementID(), nCompletedTime);
				SendPlayerMsg(&msg);
			} else {
				break;
			}
		}
	}
}

void CEntityAchievementComponent::AddAchivement(unsigned short nAchievementID, unsigned int nCompletedTime,  AchievementConfig *pDoc, unsigned char nState)
{
	if (nAchievementID > 0 && pDoc != NULL) {
		if (find(m_listAchievement.begin(), m_listAchievement.end(), nAchievementID) == m_listAchievement.end()) {
			CAchievementInfo achivement = CAchievementInfo(nAchievementID, nCompletedTime, pDoc, nState);
			m_listAchievement.push_back(achivement);
		}
	}
}

void CEntityAchievementComponent::AddAchievementToDB(unsigned short nAchievementID, unsigned int nCompletedTime)
{
	if (nAchievementID > 0) {
		CAchievementInfo *pInfo = new CAchievementInfo(nAchievementID, nCompletedTime, NULL, AchievementCompleted);
		if (pInfo != NULL && GetRoleAttr() != NULL) {
			CDelHelper delHelper(pInfo);
			AddQuery(QUERY_AddAchievement, GetRoleAttr()->GetRoleID(), pInfo, 0, NULL, &delHelper);
		}
	}
}

void CEntityAchievementComponent::AddTitle(unsigned short nTitleID)
{
	if (nTitleID > 0) {
		CTitleInfo title = CTitleInfo(nTitleID, (unsigned int)time(NULL));
		list<CTitleInfo>::iterator itr = find(m_listTitle.begin(), m_listTitle.end(), nTitleID);
		if (itr == m_listTitle.end()) {
			m_listTitle.push_back(title);
		}
	}
}

void CEntityAchievementComponent::AddTitleToDB(unsigned short nTitleID)
{
	if (nTitleID > 0) {
		CTitleInfo *pTitle = new CTitleInfo(nTitleID, (unsigned int)time(NULL));
		if (pTitle != NULL && GetRoleAttr() != NULL) {
			CDelHelper delHelper(pTitle);
			AddQuery(QUERY_AddTitle, GetRoleAttr()->GetRoleID(), pTitle, 0, NULL, &delHelper);
		}
	}
}

void CEntityAchievementComponent::UpdateAchievementStateToDB(unsigned int nRoleID, unsigned short nAchievementID, unsigned char nState)
{
	unsigned short *pAchievemenID = new unsigned short(nAchievementID);
	if (pAchievemenID != NULL) {
		CDelHelper delHelper(pAchievemenID);
		AddQuery(QUERY_UpdateAchievementState, nRoleID, pAchievemenID, nState, NULL, &delHelper);	
	}
}

bool CEntityAchievementComponent::IsFlower(itemtype_t nItemType)
{
	return nItemType == ITEMID_TULIP || nItemType == ITEMID_LILY || nItemType == ITEMID_ROSE;
}

void CEntityAchievementComponent::MusicEnd(CRoomComponent& rRoleRoomCOM)
{
	if (rRoleRoomCOM.RoleMarkInfo().Place() == 1) {
		if (rRoleRoomCOM.Room()->DancerCount() >= 2) {
			m_nNumWinBetween2To4Persons++;
			UpdateAchievement(AchievementType_WinTheComptetition);
		} else if (rRoleRoomCOM.Room()->DancerCount() == 4) {
			m_nNumWinIn4Persons++;
			UpdateAchievement(AchievementType_WinTheComptetition);
		}
	} else {
		m_nNumWinBetween2To4Persons = 0;
		m_nNumWinIn4Persons = 0;
	}
	if (rRoleRoomCOM.Room()->PlayMusicMode() == MusicMode_Taiko) {
		m_nTaikoComboNum = m_nTaikoComboNum + (unsigned short)rRoleRoomCOM.RoleMarkInfo().MaxSpecialCount();
		UpdateAchievement(AchievementType_HistoryTaikoComboCount);
	}

	if (rRoleRoomCOM.Room()->PlayMusicMode() ==  MusicMode_Taiko && rRoleRoomCOM.RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss) == 0) {
		m_nHistoryTaikoComboMusicNum++;
		UpdateAchievement(AchievementType_HistoryComboMusicCount);
	}

	m_nLastSpecialCount = 0;
}

void CEntityAchievementComponent::SetAchivementFightResult(CFightResult *pResult, CRoomComponent & roomComponent, int nScore, int nRank)
{
	if (pResult != NULL && roomComponent.Room() != NULL ) {
		pResult->m_nModeID = (unsigned char)roomComponent.Room()->PlayMusicMode();
		pResult->m_nDifficulty = (unsigned char)roomComponent.Room()->PlayMusicLevel();
		switch (nRank)
		{
		case RoomRoundRank_Perfect: { 
				pResult->m_nResult[0]++;
			}
			break;
		case RoomRoundRank_Good: {
				pResult->m_nResult[1]++;
			}
			break;
		case RoomRoundRank_Cool: {
				pResult->m_nResult[2]++;
			}
			break;
		case RoomRoundRank_Bad: {
				pResult->m_nResult[3]++;
			}
			break;
		case RoomRoundRank_Miss: {
				pResult->m_nResult[4]++;
			}
			break;
		default:
			;
		}
			
		pResult->m_nHistoryTotalScore += nScore;
		if (roomComponent.RoleMarkInfo().MaxSpecialCount() > m_nLastSpecialCount) {
			pResult->m_nConPerfectNum += (roomComponent.RoleMarkInfo().MaxSpecialCount() - m_nLastSpecialCount);
			//WriteLog("last combo count %d, add combo %d\n", pResult->m_nConPerfectNum, roomComponent.RoleMarkInfo().SpecialCount() - m_nLastSpecialCount);
			m_nLastSpecialCount = roomComponent.RoleMarkInfo().MaxSpecialCount() ;
		}
	}
}

void CEntityAchievementComponent::RoomStart(CRoomComponent & roomComponent)
{
	int nCount = 0;
	if (GetRoleAttr() != NULL && roomComponent.Room() != NULL) {
		if (roomComponent.Room()->IsMatch()) {
			if (GetRoleAttr()->GetSex() == ESexType_Female) {
				nCount = roomComponent.Room()->MaleDancerCount();
			} else {
				nCount = roomComponent.Room()->FemaleDancerCount();	
			}
		}
	}

	m_nAutoMatchWithOppositeSexNum += nCount;
	UpdateAchievement(AchievementType_HistoryAutoMatchWithOppositeSexCount);
}

void CEntityAchievementComponent::RoomPerRoundScoreReport(CRoomComponent & roomComponent, int nScore, int nRank)
{
	map<int, map<int, CFightResult > >::iterator itMapResult = m_mapFightResult.find(roomComponent.Room()->PlayMusicMode());

	if (itMapResult != m_mapFightResult.end()) {	// for mode
		map<int, CFightResult > *pTempMap = &(itMapResult->second);
		map<int, CFightResult >::iterator itResult = pTempMap->find(roomComponent.Room()->PlayMusicLevel());	// for level
		if (itResult != pTempMap->end()) {
			CFightResult *pResult = &itResult->second;
			SetAchivementFightResult(pResult, roomComponent, nScore, nRank);
		} else {
			CFightResult result;
			SetAchivementFightResult(&result, roomComponent, nScore, nRank);
			pTempMap->insert(make_pair(roomComponent.Room()->PlayMusicLevel(), result));
		}
	} else {
		map<int, CFightResult> mapTemp;
		CFightResult result;
		SetAchivementFightResult(&result, roomComponent, nScore, nRank);
		mapTemp.insert(make_pair(roomComponent.Room()->PlayMusicLevel(), result));
		m_mapFightResult.insert(make_pair(roomComponent.Room()->PlayMusicMode(), mapTemp));
	}
	UpdateAchievement(AchievementType_HistoryTotalScore);
}

unsigned int CEntityAchievementComponent::m_AchiScore()
{
	unsigned int nScore = 0;	
	unsigned short nSize= (unsigned short)m_listAchievement.size();
	if (nSize > 0) {
		for (list<CAchievementInfo>::iterator itr = m_listAchievement.begin(); itr != m_listAchievement.end(); itr++) {
			if (itr->GetAchievementDoc() != NULL) {
				nScore += itr->GetAchievementDoc()->GetScore();
			}
		}
	}

	return nScore;
}


//记录角色
void CEntityAchievementComponent::_LogRoleAchievement(unsigned int nRoleID, CRoleAchievementLog::EAchievementAction eAchievementAction, unsigned int nAchievementID)
{
	CRoleAchievementLog * pRoleAchievementLog = new CRoleAchievementLog();
	pRoleAchievementLog->m_nRoleID = nRoleID;
	pRoleAchievementLog->m_eAchievementAction = eAchievementAction;
	pRoleAchievementLog->m_nAchievementID = nAchievementID;
	AddQuery(QUERY_LogRoleAchievement, 0, pRoleAchievementLog);
}

// end of file

