#include "EntityQuestComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "../../socket/GameMsg_Map.h"
#include "QuestListEntry.h"
#include "../util/CommonFunc.h"
#include "QuestDataManager.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../achievement/EntityAchievementComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../logic/EntityTimerComponent.h"
#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../vip/EntityVIPComponent.h"

#include "../mail/MailMgr.h"
#include "../item/ItemProcess.h"

#include "../../datastructure/ItemErrorInfo.h"
//#include "../../liblog/LogType.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/Localization.h"
#include "SpecailQuestMgr.h"

#include "GameMsg_C2S_GetQuestReward.h"
#include "GameMsg_C2S_GetQuestList.h"
#include "GameMsg_C2S_QuestRead.h"
#include "GameMsg_C2S_AcceptQuest.h"

#include "GameMsg_S2C_QuestStateChange.h"
#include "GameMsg_S2C_GetQuestRewardResult.h"
#include "GameMsg_S2C_GetQuestListResult.h"
#include "GameMsg_S2C_AcceptQuestResult.h"
#include "GameMsg_S2C_Quest.h"



CEntityQuestComponent::CEntityQuestComponent():
	CEntityComponent(true, true)
{
	 m_pNet = NULL;
	 m_pItem = NULL;
	 m_pChat = NULL;
	 m_pAttr = NULL;
	 m_pFriend = NULL;
	 m_pTimer = NULL;
	 m_pGroup = NULL;
	 m_pCouple = NULL;
	 m_nNewQuestCount = 0;
	 m_nNewCompleteQuestCount = 0;
	 m_nLastStop = 0;
	 m_nLastAutoPushTime = 0;
	 m_nFresherDay = 0;
	 m_nMallBuyCount = 0;
	 m_nFresherBestMusicGrade = 0;
     m_nFresherLastRewardQuest = 0;
}

CEntityQuestComponent::~CEntityQuestComponent()
{

}

void CEntityQuestComponent::Start()
{
	m_pAttr   = GetComponent<CRoleAttribute>();
	m_pNet    = GetComponent<CRoleNet>();
	m_pItem   = GetComponent<CRoleItem>();
	m_pChat   = GetComponent<CRoleChat>();
	m_pAch    = GetComponent<CRoleAchievement>();
	m_pFriend = GetComponent<CRoleFriend>();
	m_pTimer  = GetComponent<CRoleTimer>();
	m_pGroup  = GetComponent<CRoleDanceGroup>();
	m_pCouple = GetComponent<CRoleCouple>();
    m_pVip    = GetComponent<CRoleVIP>();
}

void CEntityQuestComponent::RegComponentNetMsgMap()
{
	//GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestReward);
	//GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestList);
	//GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuestRead);
	//GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AcceptQuest);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFresherQuestReward);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFresherQuestList);
	//
	//RegMsgProcessor(MSG_C2S_GetQuestReward, &CEntityQuestComponent::OnGetQuestReward);	
	//RegMsgProcessor(MSG_C2S_GetQuestList, &CEntityQuestComponent::OnGetQuestList);
	//RegMsgProcessor(MSG_C2S_QuestRead, &CEntityQuestComponent::OnQuestRead);
	//RegMsgProcessor(MSG_C2S_AcceptQuest, &CEntityQuestComponent::OnAcceptQuest);
	RegMsgProcessor(MSG_C2S_GetFresherQuestList, &CEntityQuestComponent::OnGetFresherQuestList);
	RegMsgProcessor(MSG_C2S_GetFresherQuestReward, &CEntityQuestComponent::OnGetFresherQuestReward);

	// DB
	RegQueryProcessor(QUERY_AddRoleQuest, &CEntityQuestComponent::_AddQuestToDBCallBack);
	RegQueryProcessor(QUERY_RemoveRoleQuest, &CEntityQuestComponent::_RemoveQuestToDBCallBack);
	RegQueryProcessor(QUERY_AddRoleQuestHistory, &CEntityQuestComponent::_AddQuestHistoryToDBCallBack);
	RegQueryProcessor(QUERY_UpdateQuestInfo, &CEntityQuestComponent::_UpdateQuestInfoCallBack);
}

void CEntityQuestComponent::OnEvent(CComponentEvent& refEvent )
{
	//if (refEvent.nID == eQuestAcceptTimer || refEvent.nID == eQuestExpiredTimer)
	//{
	//	unsigned short * pQuestID = (unsigned short *)refEvent.m_pData;// 上层没有new，仅仅是传入局部变量地址
	//	unsigned short nQuestID = *(pQuestID);
	//	CQuestDoc * pQuestDoc = CDataManager::Instance().GetQuestManager().GetQuestByID(nQuestID);

	//	if (refEvent.nID == eQuestAcceptTimer){
	//		_ProcessNewQuest(pQuestDoc);
	//	} else {
	//		QUESTINFO * pQuestInfo = _GetPlayerQuest(nQuestID);
	//		if (pQuestInfo != NULL && _CanCompleteQuest(pQuestInfo)){
	//			//SetQuestState(pQuestInfo, eQuestState_Completed);
	//			//NotifyPlayerQuestStateChange(pQuestInfo->m_nQuestID, eQuestState_Completed);
	//		} else if (pQuestInfo != NULL && !_CanCompleteQuest(pQuestInfo)){
	//			//不提示玩家任务到期
	//			_RemoveQuest(nQuestID);
	//		}
	//	}
	//}
}

void CEntityQuestComponent::OnLogin()	
{
	//_RemoveExpiredQuest();
	//PushPlayerQuest(true);
	//_SendQuestStaticBinData();
	if (CFresherQuestDataManager::Instance().IsOpen() 
		&& m_nFresherDay > 0
		&& m_nFresherDay <= CFresherQuestDataManager::Instance().m_nQuestDuration + 1)
		// 7天过后，如果玩家有未领取的奖励，还需要自动发送给玩家，所以这里fresherDay判断时多了一天
	{
		_SendFresherQuestAndReward();
	}
}
void CEntityQuestComponent::OnLogout()
{
}


void CEntityQuestComponent::SendExtraStaticData()
{
	_SendQuestStaticBinData();
}

void CEntityQuestComponent::PushPlayerQuest(bool bPushTimer)
{
	unsigned int now = (unsigned int)time(NULL);
	const map<unsigned short, CQuestDoc *> & aAllQuest = ConfigManager::Instance().GetQuestManager().GetAllQuest();
	map<unsigned short, CQuestDoc *>::const_iterator it = aAllQuest.begin();
	while (it != aAllQuest.end()) {
		CQuestDoc * pQuestDoc = (CQuestDoc *)(it->second);
		if (bPushTimer && m_pTimer != NULL && pQuestDoc != NULL && pQuestDoc->GetStartTime() != 0 && now < pQuestDoc->GetStartTime()){
			m_pTimer->AddTimer(pQuestDoc->GetStartTime(),eComponentEvent_QuestAcceptTimer,pQuestDoc->GetID());
		}
		_ProcessNewQuest(pQuestDoc);
		++it;
	}

	//if (m_nNewQuestCount > 0)
	//{
	//	GameMsg_S2C_AutoPushQuest msg;
	//	msg.m_nNewQuestCount = m_nNewQuestCount;
	//	m_pNet->SendPlayerMsg(&msg);
	//	m_nNewQuestCount = 0;
	//}

	//if (m_nNewCompleteQuestCount > 0)
	//{
	//	GameMsg_S2C_UnRewardQuest msg;
	//	msg.m_nQuestCount = m_nNewCompleteQuestCount;
	//	m_pNet->SendPlayerMsg(&msg);
	//	m_nNewCompleteQuestCount = 0;
	//}
}

void CEntityQuestComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityQuestComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
	unsigned int nNow = (unsigned int)time(NULL);		// 第二天零时将清除前一天的非整周的日常任务
	//if (GetLocalHourOfDay(nNow) == 0 && GetLocalHourOfDay(m_nLastStop) == 23) {
	//	list<QUESTINFO>::iterator itr =	m_listQuest.begin();					
	//	while(itr != m_listQuest.end()) {
	//		if (itr->IsValid() && itr->m_pQuest->IsRepeatedQuest()) {
	//			itr = m_listQuest.erase(itr);
	//		} else {
	//			++itr;
	//		}
	//	}
	//}

	m_nFresherDay = DiffDays(nNow, m_pAttr->m_nRoleCreateTime) + 1;

	static const unsigned int kAutoPushQuestInterval = 5;
	if (nNow - m_nLastAutoPushTime > kAutoPushQuestInterval)
	{
		//PushPlayerQuest(true);
		m_nLastAutoPushTime = nNow;

		_SendFresherUnReceivedReward();
	}

	m_nLastStop = nNow;	
}

QUESTINFO *CEntityQuestComponent::_GetPlayerQuest(unsigned short nQuestID)
{
	list<QUESTINFO>::iterator it;
	it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);
	if (it != m_listQuest.end()){
		return &(*it);
	}

	return NULL;
}

//QUESTHISTORYINFO * CEntityQuestComponent::GetPlayerHistoryQuest(unsigned short nQuestID)
//{
//	list<QUESTHISTORYINFO>::iterator it;
//	it = find(m_listQuestHistory.begin(), m_listQuestHistory.end(), nQuestID);
//	if (it != m_listQuestHistory.end()){
//		return &(*it);
//	}
//	return NULL;
//}

bool CEntityQuestComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
	//m_listQuest = pRoleInfoDB->listQuest;
	//m_mapQuestHistory = pRoleInfoDB->mapQuestHistory;
	//m_listRepeatedQuest = pRoleInfoDB->listRepeatedQuest;
	
	//for (list<QUESTINFO>::iterator it = m_listQuest.begin(); it != m_listQuest.end(); ){
	//	it->SetQuestDocData(CDataManager::Instance().GetQuestManager().GetQuestByID((unsigned short)it->m_nQuestID));
	//	if (it->GetQuest() == NULL){
	//		m_listQuest.erase(it++);
	//	} else {
	//		_AddQuestTimer(&(*it));//添加计时器
	//		if (it->GetState() == eQuestState_Completed) {
	//			m_nNewCompleteQuestCount++;
	//		}
	//		it++;
	//	}
	//}

	m_setFresherQuestReward = pRoleInfoDB->m_setFresherQuestReward;
	
	m_nFresherDay = DiffDays(time(NULL), m_pAttr->m_nRoleCreateTime) + 1;
	m_nFresherBestMusicGrade = pRoleInfoDB->m_nFresherBestMusicGrade;
	m_nMallBuyCount = pRoleInfoDB->m_nMallBuyCount;
	return true;
}

bool CEntityQuestComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const 
{
	if (pRoleInforDB != NULL) {
		//pRoleInforDB->listQuest = m_listQuest;
		//pRoleInforDB->mapQuestHistory = m_mapQuestHistory;
		//pRoleInforDB->listRepeatedQuest = m_listRepeatedQuest;

		return true;
	}

	return false;
}

void CEntityQuestComponent::_SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pNet != NULL && pMsg != NULL){
		m_pNet->SendPlayerMsg(pMsg);
	}
}

bool CEntityQuestComponent::_IsInQuestList(unsigned short nQuestID)
{
	list<QUESTINFO>::iterator it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);

	return it != m_listQuest.end();
}


void CEntityQuestComponent::OnGetQuestList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_S2C_GetQuestListResult GetListResultMsg;
	for (list<QUESTINFO>::iterator it = m_listQuest.begin(); it != m_listQuest.end(); it++) {
		_UpdateCollectionTarget(*it);
		CQuestListEntry listEntry(&(*it), (CRoleEntity *)Entity());	
		GetListResultMsg.m_listQuestInfo.push_back(listEntry);
	}
	_SendPlayerMsg(&GetListResultMsg);
	if (m_pAttr != NULL) {
		WriteLog(LOGLEVEL_DEBUG, "[%d]已经完成的任务数为%d\n", m_pAttr->GetRoleID(),GetCompletedAllQuestCount());
	}
}

void CEntityQuestComponent::OnQuestRead(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	if (m_pAttr != NULL){
		GameMsg_C2S_QuestRead *pQuestReadMsg = (GameMsg_C2S_QuestRead *)&msg;
		QUESTINFO *pQuestInfo = _GetPlayerQuest(pQuestReadMsg->m_nQuestID);
		if (pQuestInfo != NULL && pQuestInfo->IsCanAccept()){
			_SetQuestState(pQuestInfo,eQuestState_HaveRead);
		}
	}
}

void CEntityQuestComponent::OnAcceptQuest(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_AcceptQuest	*pAcceptMsg = (GameMsg_C2S_AcceptQuest *)&msg;
	unsigned short nQuestID = pAcceptMsg->m_nQuestID;

	_AcceptQuest(nQuestID, true);
}

void CEntityQuestComponent::OnGetQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
    GameMsg_C2S_GetQuestReward *pRewardMsg = (GameMsg_C2S_GetQuestReward *)&msg;

    if (!_HasAccept(pRewardMsg->m_nQuestID))
    {
        return;
    }
    if (!_HasComplete(pRewardMsg->m_nQuestID))
    {
        return;
    }

    QUESTINFO *pQuestInfo = _GetPlayerQuest(pRewardMsg->m_nQuestID);
    if (pQuestInfo != NULL && m_pAttr != NULL &&  pQuestInfo->IsValid())
    {
        QUESTINFO questInfo(*pQuestInfo);
        map<itemtype_t, CQuestItem *> *pMapRewardItem = NULL;
        if (m_pAttr->GetSex() == ESexType_Male)
        {
            pMapRewardItem = &(questInfo.m_pQuest->m_nRewrad.m_mapRewardItemMale);
        } 
        else if (m_pAttr->GetSex() == ESexType_Female)
        {
            pMapRewardItem = &(questInfo.m_pQuest->m_nRewrad.m_mapRewardItemFemale);
        } 
        else 
        {
            WriteLog(LOGLEVEL_ERROR, "OnGetQuestReward::玩家性别错误\n");
            return;
        }
        if (pMapRewardItem->size() > 0 && (!_CanAddItem(*pMapRewardItem)))
        {
            //m_pChat->SysChat("无法加入获奖物品， 请检查物品栏和物品");
            m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind11").c_str());
            return;
        }

        if (!_HasDone(pRewardMsg->m_nQuestID) ) 
        {	// 做过的日常任务也在此记录
            __int64 nRecord = _GetHistoryQuestBlob(pRewardMsg->m_nQuestID);
            _AddQuestHistory(pRewardMsg->m_nQuestID);

            if (nRecord == 0) 
            {
                _AddQuestHistoryToDB(pRewardMsg->m_nQuestID);
            } 
            else 
            {
                _UpdateQuestHistoryToDB(pRewardMsg->m_nQuestID);
            }
        }
        // 成就
        m_pAch->UpdateAchievement(AchievementType_HistoryAllQuestCompletedCount);	//全部任务的成就
        if (questInfo.m_pQuest->IsGrowQuest()) 
        {
            m_pAch->UpdateAchievement(AchievementType_HistoryGrowQuestCompletedCount);	//成长任务的成就
        }

        if (questInfo.m_pQuest->IsRepeatedQuest()) 
        {
            CRepeatedHistoryQuest *pInfo = _GetRepeatedQuestDoneBy(pRewardMsg->m_nQuestID);	
            if (pInfo != NULL) 
            {
                pInfo->m_nTime = (unsigned int)time(NULL);
                pInfo->m_nCount++;
                _UpdateRepeatedQuestToDB(*pInfo);
            } 
            else 
            {
                CRepeatedHistoryQuest info(pRewardMsg->m_nQuestID, (unsigned int)time(NULL), 1);
                _AddRepeatedQuestDoneToDB(info);
                _AddRepeatedQuestDone(info);
            }
        }
        _RemoveQuest(pRewardMsg->m_nQuestID);
        _RemoveQuestToDB(pRewardMsg->m_nQuestID);
        _RemoveCollectionItem(questInfo.m_nQuestID, questInfo.m_pQuest->m_mapItemCollection);	
        _GetReward(questInfo.m_nQuestID, questInfo.m_pQuest->m_nRewrad.m_nRewardMoney, questInfo.m_pQuest->m_nRewrad.m_nRewardExp, *pMapRewardItem);
        _LogRoleQuest(m_pAttr->GetRoleID(), pRewardMsg->m_nQuestID);
        _UpdateQuestTotalCount();	// 总数加1
        PushPlayerQuest();

        GameMsg_S2C_GetQuestRewardResult rewardResultMsg = GameMsg_S2C_GetQuestRewardResult(pRewardMsg->m_nQuestID , eGetQuestReward_Success);
        _SendPlayerMsg(&rewardResultMsg);
    }
}

void CEntityQuestComponent:: OnUseItem(itemtype_t nItemType, unsigned short nCount)
{
	//if (nItemType == 0 || nCount == 0){
	//	return;
	//}
	//list<QUESTINFO>::iterator itQuest = m_listQuest.begin();
	//for (; itQuest != m_listQuest.end(); itQuest++){
	//	QUESTINFO * pQuest = (QUESTINFO *)&(*itQuest);
	//	//只更新已接任务
	//	if (pQuest != NULL && pQuest->IsValid() && pQuest->IsAccepted()){
	//		list<TargetInfo> *pListItemUse = pQuest->GetTargetListBy(TARGET_ITEMUSE);		
	//		if (pListItemUse != NULL){
	//			list<TargetInfo>::iterator itTarget = pListItemUse->begin();
	//			for (; itTarget != pListItemUse->end(); itTarget++){
	//				if (nItemType == itTarget->m_nIndex){
	//					map<unsigned short, CQuestItem *>::const_iterator itrItem  = pQuest->GetQuest()->m_mapItemUse.find(itTarget->m_nIndex);
	//					if (itrItem != pQuest->GetQuest()->m_mapItemUse.end()){
	//						itTarget->m_nCount =itTarget->m_nCount + nCount;
	//						itTarget->m_nCount = itTarget->m_nCount >  itrItem->second->m_nItemCount ?  itrItem->second->m_nItemCount  : itTarget->m_nCount;
	//						_UpdateTargetToDB(pQuest->m_nQuestID,&(*itTarget));
	//						if (_CanCompleteQuest(pQuest)){
	//							_SetQuestState(pQuest,eQuestState_Completed);
	//							_NotifyPlayerQuestStateChange(pQuest->m_nQuestID, eQuestState_Completed);
	//						} 
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void CEntityQuestComponent::OnItemCountChange(itemtype_t nItemType, int nCount)
{
	//if (nItemType == 0 || nCount == 0 || m_pItem == NULL){
	//	return;
	//}
	//list<QUESTINFO>::iterator itQuest = m_listQuest.begin();
	//for (; itQuest != m_listQuest.end(); itQuest++){
	//	QUESTINFO * pQuest = (QUESTINFO *)&(*itQuest);
	//	//只更新已接任务或者处于完成状态任务
	//	if (pQuest != NULL && pQuest->IsValid() && (pQuest->IsAccepted() || pQuest->IsCompleted())){
	//		list<TargetInfo> *pListItemCollection = pQuest->GetTargetListBy(TARGET_ITEMCOLLECTION);		

	//		if (pListItemCollection != NULL){
	//			list<TargetInfo>::iterator itTarget = pListItemCollection->begin();
	//			for (; itTarget != pListItemCollection->end(); itTarget++){
	//				if (nItemType == itTarget->m_nIndex){
	//					//int nCountTemp = itTarget->m_nCount + nCount;
	//					//if (nCountTemp < 0){
	//					//	nCountTemp = 0;
	//					//}
	//					//Item_Column nColumnTypeOut;
	//					//int nIndexOut;
	//					int nCountTemp = m_pItem->GetItemNumber(nItemType, true, true);

	//					map<unsigned short, CQuestItem *>::const_iterator itrItem  = pQuest->GetQuest()->m_mapItemCollection.find(itTarget->m_nIndex);
	//					if (itrItem != pQuest->GetQuest()->m_mapItemCollection.end()){
	//						itTarget->m_nCount = (unsigned short)nCountTemp > itrItem->second->m_nItemCount ? itrItem->second->m_nItemCount : (unsigned short)nCountTemp;
	//						_UpdateTargetToDB(pQuest->m_nQuestID,&(*itTarget));
	//						if (_CanCompleteQuest(pQuest) && !pQuest->IsCompleted()){ 
	//							_SetQuestState(pQuest,eQuestState_Completed);
	//							_NotifyPlayerQuestStateChange(pQuest->m_nQuestID, eQuestState_Completed);
	//						} else if (!_CanCompleteQuest(pQuest) && pQuest->IsCompleted()){	// 进度回退
	//							_SetQuestState(pQuest, eQuestState_Accepted);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void CEntityQuestComponent::MusicMatchChange(QUESTUPDATEINFO *pQuestUpdateInfo)
{
	//if (pQuestUpdateInfo == NULL){
	//	return;
	//}
	//list<QUESTINFO>::iterator itQuest = m_listQuest.begin();
	//for (; itQuest != m_listQuest.end(); itQuest++){
	//	QUESTINFO * pQuest = (QUESTINFO *)&(*itQuest);
	//	if (pQuest != NULL && pQuest->IsValid() && pQuest->IsAccepted()){
	//		list<TargetInfo> *pListMusic = pQuest->GetTargetListBy(TARGET_MUSIC);
	//		if (pListMusic != NULL){
	//			list<TargetInfo>::iterator itTarget = pListMusic->begin();
	//			for (; itTarget != pListMusic->end(); itTarget++){
	//				CQuestDoc * pQuestDoc = pQuest->GetQuest();
	//				CQuestMusic * pQuestMusic = pQuestDoc->GetQuestMusic(itTarget->m_nIndex);
	//				if (_UpdateMusicTarget(pQuestDoc, pQuestMusic, pQuestUpdateInfo)){
	//					(itTarget->m_nCount)++;
	//					_UpdateTargetToDB(pQuest->m_nQuestID,&(*itTarget));
	//					if (_CanCompleteQuest(pQuest)){
	//						_SetQuestState(pQuest,eQuestState_Completed);
	//						_NotifyPlayerQuestStateChange(pQuest->m_nQuestID, eQuestState_Completed);
	//					}
	//				}
	//				else if(pQuestMusic != NULL && pQuestMusic->m_bConRequired && pQuestMusic->m_nRequiredMatchNum > 1){
	//					itTarget->m_nCount = 0;
	//					_UpdateTargetToDB(pQuest->m_nQuestID,&(*itTarget));
	//				}
	//			}
	//		}
	//	}
	//}
}

void CEntityQuestComponent::MusicGradeChange(int nGrade)
{
	if (m_nFresherBestMusicGrade < nGrade
		&& CFresherQuestDataManager::Instance().IsOpen()
		&& m_nFresherDay <= CFresherQuestDataManager::Instance().m_nQuestDuration)
	{
		m_nFresherBestMusicGrade = nGrade;
		AddQuery(QUERY_FresherQuest_BestMusicGrade, m_pAttr->m_nRoleID, NULL, nGrade, NULL);
	}
	PushFresherQuestStateChange();
}

void CEntityQuestComponent::OnRoleAttrChange()
{
	//list<QUESTINFO>::iterator itQuest = m_listQuest.begin();
	//for (; itQuest != m_listQuest.end(); itQuest++){
	//	QUESTINFO * pQuest = (QUESTINFO *)&(*itQuest);
	//	if (pQuest != NULL && pQuest->IsValid() && (pQuest->IsAccepted() || pQuest->IsCompleted())){
	//		//SetQuestState(pQuest,CanCompleteQuest(pQuest) ? (unsigned char)eQuestState_Completed:(unsigned char)eQuestState_Accepted);
	//		if (_CanCompleteQuest(pQuest)){
	//			_SetQuestState(pQuest,(unsigned char)eQuestState_Completed);
	//			_NotifyPlayerQuestStateChange(pQuest->m_nQuestID, eQuestState_Completed);
	//		} else if (pQuest->IsCompleted()){
	//			_SetQuestState(pQuest, (unsigned char)eQuestState_Accepted);
	//		}
	//	}
	//}

	PushFresherQuestStateChange();
}

void CEntityQuestComponent::MusicEnd(CRoomComponent& rRoleRoomCOM)
{
	QUESTUPDATEINFO questUpdateInfo;
	questUpdateInfo.m_nMusicID = rRoleRoomCOM.Room()->PlayMusicID();
	questUpdateInfo.m_nModeID = rRoleRoomCOM.Room()->PlayMusicMode();
	questUpdateInfo.m_nSceneID = rRoleRoomCOM.Room()->PlayScene();
	questUpdateInfo.m_nMusicDifficulty = rRoleRoomCOM.Room()->PlayMusicLevel();
	questUpdateInfo.m_nPlayerNum = rRoleRoomCOM.Room()->StartRoomDancersCount();
	questUpdateInfo.m_nRank = rRoleRoomCOM.RoleMarkInfo().Place();
	questUpdateInfo.m_nScore = rRoleRoomCOM.RoleMarkInfo().Mark();
	questUpdateInfo.m_nPerfectNum = rRoleRoomCOM.RoleMarkInfo().TotalRankCount(RoomRoundRank_Perfect);
	questUpdateInfo.m_nFailNum = rRoleRoomCOM.RoleMarkInfo().TotalRankCount(RoomRoundRank_Miss);
	questUpdateInfo.m_nConPerfectNum = rRoleRoomCOM.RoleMarkInfo().ContinuousRankCount(RoomRoundRank_Perfect);
	questUpdateInfo.m_nComboNum =  rRoleRoomCOM.RoleMarkInfo().MaxSpecialCount();
	questUpdateInfo.m_nGrade = rRoleRoomCOM.RoleMarkInfo().Grade();
	//MusicMatchChange(&questUpdateInfo);
	MusicGradeChange(questUpdateInfo.m_nGrade);
}


bool CEntityQuestComponent::_HasDone(unsigned short nQuestID)
{
	if (nQuestID > 0){
		list<CRepeatedHistoryQuest>::iterator it;
		map<short, __int64>::const_iterator itr = m_mapQuestHistory.find(nQuestID / (sizeof(__int64) * 8));

		if (itr != m_mapQuestHistory.end()){
			__int64 record = itr->second;
			return GetBit(record, nQuestID % (sizeof(__int64) * 8)) == 1;	
		}
	}

	return false;
}

bool CEntityQuestComponent::_HasAccept(unsigned short nQuestID)
{
	list<QUESTINFO>::iterator it;
	it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);
	if (it != m_listQuest.end()){
		return it->IsAccepted();
	}
	return false;
}

bool CEntityQuestComponent::_HasComplete(unsigned short nQuestID)
{
	list<QUESTINFO>::iterator it;
	it = find(m_listQuest.begin(), m_listQuest.end(), nQuestID);
	if (it != m_listQuest.end()){
		return it->IsCompleted();
	}
	return false;
}

bool CEntityQuestComponent::_CanAcceptQuest(CQuestDoc *pQuestDoc)
{
	if (pQuestDoc == NULL || m_pAttr == NULL){
		return false;
	}

	CQuestRequirement * pQuestRequirement = &pQuestDoc->m_CQusetRequirement;
	if (pQuestRequirement->m_nLevel != 0 && pQuestRequirement->m_nLevel > m_pAttr->GetLevel()){
		return false;
	}

	if (pQuestRequirement->m_nEndLevel != 0 && pQuestRequirement->m_nEndLevel < m_pAttr->GetLevel()){
		return false;
	}

	if (pQuestRequirement->m_nSex != 0 && pQuestRequirement->m_nSex != m_pAttr->GetSex()){
		return false;
	}
	if (pQuestRequirement->m_nHotValue != 0 && pQuestRequirement->m_nHotValue > m_pAttr->GetHot()){
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
	if (pQuestRequirement->m_nWeekDay != 0 && (pQuestRequirement->m_nWeekDay - 1) != GetWeekDay() )	{
		return false;
	}
	if (!pQuestDoc->IsValidTime()){
		return false;
	}
	
	if (pQuestRequirement->m_nPreviousQusetID != 0 && !_HasDone(pQuestRequirement->m_nPreviousQusetID)){
		return false;
	}
	if (_HasDone(pQuestRequirement->m_nExclusiveQustID) || _HasAccept(pQuestRequirement->m_nExclusiveQustID)){
		return false;
	}
	//map<unsigned short, CQuestItem *> m_mapRequredItem 物品要求
	if (!_GetEnoughItem(pQuestRequirement->m_mapRequredItem)){
		return false;
	}
	return true;
}

__int64 CEntityQuestComponent::_GetHistoryQuestBlob(unsigned short nQuestID)
{
	map<short, __int64>::const_iterator itr =  m_mapQuestHistory.find(nQuestID / (sizeof(__int64) * 8) );
	if (itr != m_mapQuestHistory.end()){
		return itr->second;
	}

	return 0;		
}

bool CEntityQuestComponent::_CanAddDailyQuest(CQuestDoc & questDoc)
{
	if (questDoc.IsDailyQuest()){
		list<CRepeatedHistoryQuest>::const_iterator itr = find(m_listRepeatedQuest.begin(), m_listRepeatedQuest.end(), CRepeatedHistoryQuest(questDoc.GetID()));
		if (itr != m_listRepeatedQuest.end()){		// 曾经完成过，是否是今天完成
			return !IsTimeIsToday(itr->m_nTime);
		}
		return true;
	}

	return false;
}


void CEntityQuestComponent::_ProcessNewQuest(CQuestDoc *pQuestDoc)
{
	if (pQuestDoc == NULL)
	{
		return;
	}
	
	if (_CanAcceptQuest(pQuestDoc))
	{
		if (_IsInQuestList(pQuestDoc->GetID()))
		{
			_AcceptQuest(pQuestDoc->GetID(), false);
			m_nNewQuestCount++;
		}
		else
		{
			if (!_HasDone(pQuestDoc->GetID()))
			{
				_AddQuest(pQuestDoc);
			}
			else if (pQuestDoc->IsRepeatedQuest())
			{
				if (pQuestDoc->IsDailyQuest() && _CanAddDailyQuest(*pQuestDoc))
				{
					_AddQuest(pQuestDoc);
				}
			}
		}
	}
}

bool CEntityQuestComponent::_AcceptQuest(unsigned short nQuestID, bool bSendMsg)
{
	QUESTINFO *pQuestInfo = _GetPlayerQuest(nQuestID);
	if (pQuestInfo != NULL && pQuestInfo->IsValid())
	{
		if (pQuestInfo->GetState() >= eQuestState_Accepted)
		{
			if (bSendMsg)
			{
				m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind12").c_str());
			}
			return false;
		}
		CQuestDoc * pQuestDoc = pQuestInfo->GetQuest();
		if ( _HasDone(nQuestID) && !pQuestDoc->IsRepeatedQuest())
		{
			if (bSendMsg)
			{
				m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind13").c_str());
			}
			return false;
		}

		pQuestInfo->m_nTime = (unsigned int)time(NULL);
		_SetQuestState(pQuestInfo,eQuestState_Accepted);

		if (pQuestInfo->GetTargetListBy(TARGET_ITEMCOLLECTION) != NULL)
		{
			_UpdateCollectionTarget(*pQuestInfo);//搜集物品的任务信息在接受的时候就需要更新；
		}

		if (_CanCompleteQuest(pQuestInfo))
		{
			_SetQuestState(pQuestInfo,eQuestState_Completed);
			//NotifyPlayerQuestStateChange(pQuestInfo->m_nQuestID, eQuestState_Completed);
		}

		_DelExclusiveQuest(nQuestID);

		if (Entity() != NULL && bSendMsg)
		{
			GameMsg_S2C_AcceptQuestResult resultMsg(pQuestInfo->m_nQuestID, 0, pQuestInfo->GetState(),*pQuestInfo, *(CRoleEntity *)Entity());
			_SendPlayerMsg(&resultMsg);
		}

		return true;
	}
	else
	{
		if (bSendMsg)
		{
			m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind14").c_str());
		}
	}

	return false;
}

void CEntityQuestComponent::FinQuest(unsigned short nQuestID)
{
	if (m_pChat == NULL) {
		return;
	}

	CQuestDoc *pQuest = ConfigManager::Instance().GetQuestManager().GetQuestByID(nQuestID);
	if (pQuest == NULL) {
		//m_pChat->SysChat("该任务不存在。");
		m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind14").c_str());
		return;
	}

	CQuestDoc * pQuestDoc = pQuest;
	if (_HasDone(nQuestID) && !pQuestDoc->IsRepeatedQuest()){
		//m_pChat->SysChat("您已经完成了该任务。");
		m_pChat->SysChat(CLocalization::Instance().GetString("Chat_System_Remind12").c_str());
		return;
	}
	if (!_HasDone(nQuestID) ) {	// 做过的日常任务也在此记录
		__int64 nRecord = _GetHistoryQuestBlob(nQuestID);
		_AddQuestHistory(nQuestID);

		if (nRecord == 0) {
			_AddQuestHistoryToDB(nQuestID);
		} else {
			_UpdateQuestHistoryToDB(nQuestID);
		}
	}

	if (pQuest->IsRepeatedQuest()) {
		CRepeatedHistoryQuest *pInfo = _GetRepeatedQuestDoneBy(nQuestID);	
		if (pInfo != NULL) {
			pInfo->m_nTime = (unsigned int)time(NULL);
			pInfo->m_nCount++;
			_UpdateRepeatedQuestToDB(*pInfo);
		} else {
			CRepeatedHistoryQuest info(nQuestID, (unsigned int)time(NULL), 1);
			_AddRepeatedQuestDoneToDB(info);
			_AddRepeatedQuestDone(info);
		}
	}

	QUESTINFO *pQuestInfo = _GetPlayerQuest(nQuestID);
	if (pQuestInfo != NULL) {	//已经被推送了该任务
		_RemoveQuest(nQuestID);
		_RemoveQuestToDB(nQuestID);
	}

	_LogRoleQuest(m_pAttr->GetRoleID(), nQuestID);
	_UpdateQuestTotalCount();
	PushPlayerQuest();
	// 成就
	m_pAch->UpdateAchievement(AchievementType_HistoryAllQuestCompletedCount);	//全部任务的成就
	if (pQuest->IsGrowQuest()) {
		m_pAch->UpdateAchievement(AchievementType_HistoryGrowQuestCompletedCount);	//成长任务的成就
	}
}


void CEntityQuestComponent::_AddQuest(CQuestDoc *pQuestDoc)
{
	if (pQuestDoc == NULL || m_pAttr == NULL){
		return;
	}
	QUESTINFO * pQuestInfo = new QUESTINFO;
	pQuestInfo->m_nQuestID = pQuestDoc->GetID();
	pQuestInfo->m_nTime = 0;
	pQuestInfo->SetState(eQuestState_CanAccept);
	pQuestInfo->SetQuestDocData(pQuestDoc);

	list<TargetInfo> listTarget; // 歌曲
	map<unsigned short, CQuestMusic *>::iterator itMapMusic = pQuestDoc->m_mapMusicTarget.begin();
	for (; itMapMusic != pQuestDoc->m_mapMusicTarget.end(); itMapMusic++)
	{
		TargetInfo target(TARGET_MUSIC, itMapMusic->second->m_nMusicID, 0);
		listTarget.push_back(target);
	}
	if (!listTarget.empty()) {
		pQuestInfo->m_mapTarget.insert(make_pair(TARGET_MUSIC, listTarget));
	}

	listTarget.clear();			// 物品使用
	map<itemtype_t, CQuestItem *>::iterator itMapItemUse = pQuestDoc->m_mapItemUse.begin();
	for (; itMapItemUse != pQuestDoc->m_mapItemUse.end(); itMapItemUse++)
	{
		TargetInfo target(TARGET_ITEMUSE, itMapItemUse->second->m_nItemID, 0);
		listTarget.push_back(target);
	}
	if (!listTarget.empty()) {
		pQuestInfo->m_mapTarget.insert(make_pair(TARGET_ITEMUSE, listTarget));
	}

	listTarget.clear();			// 物品消耗
	map<itemtype_t, CQuestItem *>::iterator itMapItemCollection = pQuestDoc->m_mapItemCollection.begin();
	for (; itMapItemCollection != pQuestDoc->m_mapItemCollection.end(); itMapItemCollection++)
	{
		TargetInfo target(TARGET_ITEMCOLLECTION, itMapItemCollection->second->m_nItemID, 0);
		listTarget.push_back(target);
	}
	if (!listTarget.empty()) {
		pQuestInfo->m_mapTarget.insert(make_pair(TARGET_ITEMCOLLECTION, listTarget));
	}

	m_listQuest.push_front(*pQuestInfo);

	CDelHelper delHelper(pQuestInfo);
	AddQuery(QUERY_AddRoleQuest, m_pAttr->GetRoleID(), pQuestInfo, 0, NULL, &delHelper);
}

void CEntityQuestComponent::_AddQuestToDBCallBack(QueryBase &query)
{
	QUESTINFO *pQuestInfo = (QUESTINFO *)query.m_pAttachment;

	if (pQuestInfo != NULL)
	{
		if (query.m_nResult == 0)
		{
			if (_AcceptQuest(pQuestInfo->m_nQuestID, false))
			{
				m_nNewQuestCount++;
			}

			//添加计时器
			_AddQuestTimer(pQuestInfo);

			WriteLog(LOGLEVEL_DEBUG, "Add Quest succeed. roleid=%u questid=%d.", m_pAttr->GetRoleID(), (int)pQuestInfo->m_nQuestID);
		}
		else
		{
			_RemoveQuest(pQuestInfo->m_nQuestID);
			WriteLog(LOGLEVEL_ERROR, "Add Quest failed. roleid=%u questid=%d.", m_pAttr->GetRoleID(), (int)pQuestInfo->m_nQuestID);
		}

		delete pQuestInfo,pQuestInfo = NULL;
	}
}

void CEntityQuestComponent::_AddQuestTimer(QUESTINFO *pQuestInfo)
{
	//添加计时器
	if (m_pTimer != NULL && pQuestInfo != NULL && pQuestInfo->IsValid()){
		unsigned int now = (unsigned int)time(NULL);
		CQuestDoc * pQuestDoc = pQuestInfo->GetQuest();
		if (pQuestDoc->GetEndTime() != 0 && pQuestDoc->GetEndTime() > now){	// 有截止时间
			m_pTimer->AddTimer(pQuestDoc->GetEndTime(),eComponentEvent_QuestExpiredTimer,pQuestDoc->GetID());
		}
		if (pQuestDoc->GetLimitedTime() != 0 && pQuestInfo->GetAcceptTime() + pQuestDoc->GetLimitedTime() > now){	//有时间限制
			m_pTimer->AddTimer(pQuestInfo->m_nTime + pQuestDoc->GetLimitedTime(),eComponentEvent_QuestExpiredTimer,pQuestDoc->GetID());
		}
	}
}
void CEntityQuestComponent::_DelExclusiveQuest(unsigned short nQuestID)
{
	list< QUESTINFO >::iterator itr = m_listQuest.begin();

	while (itr != m_listQuest.end()) {
		if (itr->GetQuestID() != nQuestID && itr->GetQuest() != NULL && itr->GetQuest()->GetExclusiveQuestID() == nQuestID ) {
			_RemoveQuestToDB(itr->GetQuestID());
			m_listQuest.erase(itr++);
		} else {
			++itr;
		}
	}

}

void CEntityQuestComponent::_AddQuestHistory(unsigned short nQuestID)	
{
	map<short, __int64>::iterator itr = m_mapQuestHistory.find(nQuestID / (sizeof(__int64) * 8));					
	if (itr != m_mapQuestHistory.end()){
		SetBit(itr->second, nQuestID % (sizeof(__int64) * 8), 1);
	} else {
		__int64 nRecord = 0;
		SetBit(nRecord, nQuestID % (sizeof(__int64) * 8), 1);
		m_mapQuestHistory.insert(make_pair((short)(nQuestID / (sizeof(__int64) * 8)), nRecord) );
	}
}

void CEntityQuestComponent::_AddQuestHistoryToDB(unsigned short nQuestID )
{
	if (m_pAttr != NULL){
			__int64 nRecord = 0;
			SetBit(nRecord, nQuestID % (sizeof(__int64) * 8), 1);
			__int64 *pRecord = new __int64(nRecord);
			if (pRecord != NULL){
				CDelHelper delHelper(pRecord);
				AddQuery(QUERY_AddRoleQuestHistory, m_pAttr->GetRoleID(), pRecord, nQuestID / (sizeof(__int64) * 8), NULL, &delHelper);
			}
	}
}

void CEntityQuestComponent::_UpdateQuestHistoryToDB(unsigned short nQuestID)
{
	if (m_pAttr != NULL){
		short nIndex = nQuestID / (sizeof(__int64) * 8);
		map<short, __int64>::iterator itr = m_mapQuestHistory.find(nIndex);					
		if (itr != m_mapQuestHistory.end()){
			__int64 *pRecord = new __int64(itr->second);
			if (pRecord != NULL) {
				CDelHelper delHelper(pRecord);
				AddQuery(QUERY_UpdateRoleQuestHistory, m_pAttr->GetRoleID(), pRecord, nIndex, NULL, &delHelper);
			}
		} 
	}
}

void CEntityQuestComponent::_AddRepeatedQuestDoneToDB(CRepeatedHistoryQuest &info)
{
	if (m_pAttr != NULL){
		CRepeatedHistoryQuest *pInfo = new CRepeatedHistoryQuest(info);
		CDelHelper delHelper(pInfo);
		AddQuery(QUERY_AddRepeatedQuestDone, m_pAttr->GetRoleID(), pInfo, 0, NULL, &delHelper);		
	}
}

void CEntityQuestComponent::_UpdateRepeatedQuestToDB(CRepeatedHistoryQuest & info)
{
	if (m_pAttr != NULL){
		CRepeatedHistoryQuest *pInfo = new CRepeatedHistoryQuest(info);
		CDelHelper delHelper(pInfo);
		AddQuery(QUERY_UpdateRepeatedQuestDone, m_pAttr->GetRoleID(), pInfo, 0, NULL, &delHelper);
	}
}

void CEntityQuestComponent::_AddQuestHistoryToDBCallBack(QueryBase &query)
{
	if (query.m_nResult != 0){
		WriteLog(LOGLEVEL_ERROR, "Add QuestHistory failed.");
	}
}

void CEntityQuestComponent::_SetQuestState(QUESTINFO *pQuestInfo,unsigned char nQuestState)
{
	if (pQuestInfo != NULL && pQuestInfo->IsValid() && m_pAttr != NULL){
		pQuestInfo->SetState(nQuestState);
		QUESTINFO *pQueryQuestInfo = new QUESTINFO(*pQuestInfo);
		if (pQueryQuestInfo != NULL){
			CDelHelper delHelper(pQueryQuestInfo);
			AddQuery(QUERY_UpdateQuestInfo, m_pAttr->GetRoleID(), pQueryQuestInfo, nQuestState, NULL, &delHelper);
		}
	}
}

void CEntityQuestComponent::_UpdateQuestInfoCallBack(QueryBase &query)
{
	if (query.m_nResult != 0)
	{
		WriteLog(LOGLEVEL_ERROR, "UpdateQuestInfo failed.");
	}
	else
	{
		unsigned char nQuestState = (unsigned char)query.m_nTag;
		if (nQuestState == eQuestState_Completed)
		{
			m_nNewCompleteQuestCount++;
		}
	}
}

void CEntityQuestComponent::_RemoveQuest(unsigned short nQuestID)
{
	list<QUESTINFO>::iterator itr = m_listQuest.begin();
	for (; itr != m_listQuest.end(); itr++){
		if (itr->m_nQuestID == nQuestID){
			m_listQuest.erase(itr);
			break;
		}
	}
}

void CEntityQuestComponent::_RemoveQuestToDB(unsigned short nQuestID)
{
	if (m_pAttr != NULL){
		AddQuery(QUERY_RemoveRoleQuest, m_pAttr->GetRoleID(), NULL, nQuestID, NULL);
	}
}

void CEntityQuestComponent::_RemoveQuestToDBCallBack(QueryBase &query)
{
	if (query.m_nResult != 0){
		WriteLog(LOGLEVEL_ERROR, "Remove Quest Targets failed.");
	}
}

void CEntityQuestComponent::_RemoveExpiredQuest()
{
	list<QUESTINFO>::iterator itr = m_listQuest.begin(); 
	for (; itr != m_listQuest.end(); ){
		bool bDelete = false;
		
		CQuestDoc* pQuest = itr->GetQuest();
		if (pQuest == NULL){
			bDelete = true;
		} else {
			if ( pQuest->IsDailyQuest() && pQuest->GetWeekDay() != 0 && (pQuest->GetWeekDay() - 1) != GetWeekDay()){	
				bDelete = true;
			} else if (!pQuest->IsValidTime()){
				bDelete = true;
			} else if (pQuest->GetLimitedTime() != 0 && (unsigned int)time(NULL) > itr->GetAcceptTime() + pQuest->GetLimitedTime()){
				bDelete = true;
			}
		}

		if (bDelete){
			_RemoveQuestToDB(itr->m_nQuestID);
			m_listQuest.erase(itr++);
		} else {
			++itr;
		}
	}
}

void CEntityQuestComponent::_UpdateCollectionTarget(QUESTINFO &questInfo)
{
	if (questInfo.IsValid() && questInfo.IsAccepted() && m_pItem != NULL){
		list<TargetInfo> *pListItemCollection = questInfo.GetTargetListBy(TARGET_ITEMCOLLECTION);
		if (pListItemCollection != NULL){
			map<itemtype_t, CQuestItem *>::iterator itrCriteriaCollecItem = questInfo.m_pQuest->m_mapItemCollection.begin();
			for (; itrCriteriaCollecItem != questInfo.m_pQuest->m_mapItemCollection.end(); ++itrCriteriaCollecItem){
				if (itrCriteriaCollecItem->second != NULL) {
					ItemBase* pItem = NULL;
					if ( (pItem = m_pItem->SearchItem(itrCriteriaCollecItem->second->m_nItemID, true) ) != NULL)
					{
						list<TargetInfo>::iterator itr = find(pListItemCollection->begin(), pListItemCollection->end(), TargetInfo(TARGET_ITEMCOLLECTION, itrCriteriaCollecItem->second->m_nItemID));
						if (itr != pListItemCollection->end() && itr->m_nCount < itrCriteriaCollecItem->second->m_nItemCount){
							unsigned int nCountItem = m_pItem->GetItemCount(itrCriteriaCollecItem->second->m_nItemID, true, true);
							itr->m_nCount = nCountItem > itrCriteriaCollecItem->second->m_nItemCount ? itrCriteriaCollecItem->second->m_nItemCount : (unsigned short)nCountItem;	
							_UpdateTargetToDB(questInfo.m_nQuestID, &(*itr));
						}
					}
				}
			}
		}
	}
}

bool CEntityQuestComponent::_UpdateMusicTarget(CQuestDoc * pQuestDoc, CQuestMusic * pQuestMusic,QUESTUPDATEINFO *pQuestUpdateInfo)
{
	if (pQuestMusic == NULL || pQuestUpdateInfo == NULL){
		return false;
	}

	if (pQuestMusic == NULL){
		return false;
	}

	if (pQuestMusic->m_nMusicID != 0 && pQuestMusic->m_nMusicID != pQuestUpdateInfo->m_nMusicID) {
		return false;
	}

	if (pQuestMusic->m_nModelID != 0 && pQuestMusic->m_nModelID != pQuestUpdateInfo->m_nModeID){
		return false;
	}
	if (pQuestMusic->m_nSceneID != 0 && pQuestMusic->m_nSceneID != pQuestUpdateInfo->m_nSceneID){
		return false;
	}
	if (pQuestMusic->m_nDifficulty != 0 && pQuestMusic->m_nDifficulty != pQuestUpdateInfo->m_nMusicDifficulty){
		return false;
	}
	if (pQuestMusic->m_nMinPlayerNum != 0 && pQuestMusic->m_nMinPlayerNum > pQuestUpdateInfo->m_nPlayerNum){
		return false;
	}
	if (pQuestMusic->m_nMinRank != 0 && pQuestMusic->m_nMinRank < pQuestUpdateInfo->m_nRank){
		return false;
	}
	if (pQuestMusic->m_nMinScore != 0 && pQuestMusic->m_nMinScore > pQuestUpdateInfo->m_nScore){
		return false;
	}
	if (pQuestMusic->m_nMinPerfect != 0 && pQuestMusic->m_nMinPerfect > pQuestUpdateInfo->m_nPerfectNum){
		return false;
	}
	if (pQuestMusic->m_nMinConPerfect != 0 && pQuestMusic->m_nMinConPerfect > pQuestUpdateInfo->m_nConPerfectNum){
		return false;
	}
	if (pQuestMusic->m_nMaxFail != 0 && pQuestMusic->m_nMaxFail < pQuestUpdateInfo->m_nFailNum){
		return false;
	}
	if (pQuestMusic->m_nMinCombo != 0 && pQuestMusic->m_nMinCombo > pQuestUpdateInfo->m_nComboNum){
		return false;
	}

	map<itemtype_t, CQuestItem *> & muusicCloth = pQuestMusic->m_mapCloth;
	if (!_GetEnoughRequiredEquipedCloth(muusicCloth)){
		return false;
	}
	return true;
}

void CEntityQuestComponent::_UpdateTargetToDB(unsigned short nQuestID,TargetInfo * pTarget)
{
	if (pTarget == NULL){
		WriteLog(LOGLEVEL_ERROR, "UpdateTargetToDB failed.");
		return;
	}
	if (m_pAttr != NULL)
	{
		TargetInfo *pQueryTargetInfo = new TargetInfo(*pTarget);
		if (pQueryTargetInfo != NULL){
			CDelHelper delHelper(pQueryTargetInfo);
			AddQuery(QUERY_UpdateTargetInfo,nQuestID,pQueryTargetInfo,m_pAttr->GetRoleID(),NULL,&delHelper);
		}
	}
}

bool CEntityQuestComponent::_CanCompleteQuest(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo == NULL || !pQuestInfo->IsValid()){
		return false;
	}

	CQuestDoc * pQuest = pQuestInfo->GetQuest();
	if (!pQuest->IsValidTime()){
		return false;
	}
	if (!_CanCompleteMusicTarget(pQuestInfo)){
		return false;
	}
	if (!_CanCompleteItemUseTarget(pQuestInfo)){
		return false;
	}
	if (!_CanCompleteItemCollectionTarget(pQuestInfo)){
		return false;
	}

	if (m_pAttr != NULL){
		if (pQuest->m_attrTarget.m_nTargetLevel > 0 && pQuest->m_attrTarget.m_nTargetLevel > m_pAttr->GetLevel()){
			return false;
		}
		if (pQuest->m_attrTarget.m_nTargetHotValue > 0 && pQuest->m_attrTarget.m_nTargetHotValue > m_pAttr->GetHot()){
			return false;
		}
	}
	if (m_pFriend != NULL){
		if (pQuest->m_attrTarget.m_nTargetFriendNum > 0 && pQuest->m_attrTarget.m_nTargetFriendNum > m_pFriend->FriendCount()){
			return false;
		}
	}
	return true;
}

bool CEntityQuestComponent::_CanCompleteMusicTarget(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo != NULL){
		list<TargetInfo> * pListMusicTarget = pQuestInfo->GetTargetListBy(TARGET_MUSIC);
		map<unsigned short, CQuestMusic *> & mapQuestMusic = pQuestInfo->GetQuest()->m_mapMusicTarget;

		if (pListMusicTarget != NULL && mapQuestMusic.size() > 0){
			map<unsigned short, CQuestMusic *>::const_iterator itQuest = mapQuestMusic.begin();
			for ( ; itQuest != mapQuestMusic.end(); itQuest++){
				list<TargetInfo>::const_iterator itCompleted  = find(pListMusicTarget->begin(), pListMusicTarget->end(), TargetInfo((unsigned char)TARGET_MUSIC, itQuest->second->m_nMusicID)); 
				if (itCompleted == pListMusicTarget->end()){
					return false;
				}
				if (itCompleted->m_nCount < itQuest->second->m_nRequiredMatchNum){
					return false;
				}
			}
		}
	}
	return true;
}

bool CEntityQuestComponent::_CanCompleteItemUseTarget(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo != NULL){
		list<TargetInfo> * pListItemUseTarget = pQuestInfo->GetTargetListBy(TARGET_ITEMUSE);
		map<itemtype_t, CQuestItem *>& mapQuestItemUse = pQuestInfo->GetQuest()->m_mapItemUse;
		if (pListItemUseTarget != NULL && mapQuestItemUse.size() > 0){
			map<itemtype_t, CQuestItem *>::const_iterator itItem = mapQuestItemUse.begin();
			for (; itItem != mapQuestItemUse.end(); itItem++){
				list<TargetInfo>::const_iterator itCompleted = find(pListItemUseTarget->begin(), pListItemUseTarget->end(), TargetInfo(TARGET_ITEMUSE, itItem->second->m_nItemID));
				if (itCompleted == pListItemUseTarget->end()){
					return false;
				}
				if (itCompleted->m_nCount < itItem->second->m_nItemCount){
					return false;
				}
			}
		}
	}
	return true;
}

bool CEntityQuestComponent::_CanCompleteItemCollectionTarget(QUESTINFO *pQuestInfo)
{
	if (pQuestInfo != NULL){
		list<TargetInfo> * pListItemCollectionTarget = pQuestInfo->GetTargetListBy(TARGET_ITEMCOLLECTION);
		map<itemtype_t, CQuestItem *> & mapQuestItemCollection = pQuestInfo->GetQuest()->m_mapItemCollection;
		if (pListItemCollectionTarget != NULL && mapQuestItemCollection.size() > 0){
			map<itemtype_t, CQuestItem *>::const_iterator itItem = mapQuestItemCollection.begin();
			for (; itItem != mapQuestItemCollection.end(); itItem++){
				list<TargetInfo>::const_iterator itCompleted = find(pListItemCollectionTarget->begin(), pListItemCollectionTarget->end(), TargetInfo(TARGET_ITEMCOLLECTION, itItem->second->m_nItemID));
				if (itCompleted == pListItemCollectionTarget->end()){
					return false;
				}
				if (itCompleted->m_nCount < itItem->second->m_nItemCount){
					return false;
				}
			}
		}
	}
	return true;
}

void CEntityQuestComponent::_RemoveCollectionItem(unsigned short nQuestID, map<itemtype_t, CQuestItem *> &mapItemCollection)
{
	map<itemtype_t, CQuestItem *>::iterator it = mapItemCollection.begin();
	for (; it != mapItemCollection.end(); it++)
	{
		if (it->second != NULL)
		{
			CQuestItem *pItemCollection = it->second;
			m_pItem->RemoveItemByType(pItemCollection->m_nItemID, pItemCollection->m_nItemCount, EItemAction_Del_RemoveQuestItem);
		}
	}
}

void CEntityQuestComponent::_AddRewardItem(map<itemtype_t, CQuestItem *> &mapItem, unsigned short nQuestID)
{
	if (m_pItem != NULL){
		map<itemtype_t, CQuestItem *>::const_iterator it = mapItem.begin();
		for (; it != mapItem.end(); it++ ){
			if (it->second != NULL){
				m_pItem->AddItem(it->second->m_nItemID,it->second->m_nItemCount,it->second->m_nValidTime,EItemAction_Add_Quest,nQuestID,true,false);
			}
		}
	}
}

bool CEntityQuestComponent::_CanAddItem(map<itemtype_t, CQuestItem *> &mapItemPlus)
{
	list<CItem> listItem;
	for (map<itemtype_t, CQuestItem *>::iterator it = mapItemPlus.begin(); it != mapItemPlus.end(); it++){
		if (it->second != NULL){
			listItem.push_back(CItem(it->second->m_nItemID, it->second->m_nItemCount, 0, false));
		}
	}
	if (m_pItem != NULL){
		return (m_pItem->CanAddAllItem(listItem) == EItemError_Success ? true : false);
	}

	return false;
}

void CEntityQuestComponent::_GetReward(unsigned short nQuestID, int nMoney, int nExp, map<itemtype_t, CQuestItem *> &mapItem)
{
	m_pAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_Quest, nQuestID);
	m_pAttr->AddAbsExp(nExp, CRoleExpLog::EAddExpCause_Quest, nQuestID);
	_AddRewardItem(mapItem, nQuestID);
}

bool CEntityQuestComponent::_GetEnoughRequiredEquipedCloth(map<itemtype_t, CQuestItem *> & mapQuestMusicCloth)const
{
	map<itemtype_t, CQuestItem *>::const_iterator itr = mapQuestMusicCloth.begin();
	for (; itr != mapQuestMusicCloth.end(); itr++)
	{
		if (!m_pItem->IsEquiped(itr->second->m_nItemID))
		{
			return false;
		}
	}
	return true;
}

bool CEntityQuestComponent::_GetEnoughItem(map<itemtype_t, CQuestItem *>& mapItem)
{
	for (map<itemtype_t, CQuestItem *>::const_iterator it = mapItem.begin(); it !=mapItem.end(); it++){
		if (it->second != NULL && it->second->m_nItemCount != 0 && it->second->m_nItemCount > _GetItemCount(it->second->m_nItemID)){
			return false;
		}
	}
	return true;
}

unsigned int CEntityQuestComponent::_GetItemCount(itemtype_t nItemID)
{
	if (m_pItem != NULL){
		return m_pItem->GetItemCount(nItemID, true, true);
	}
	return 0;
}

unsigned int CEntityQuestComponent::GetCompletedQuestCountByType(unsigned short nType)	
{
	unsigned int nCount = 0;

	if (!_IsRepeatedQuest(nType)) {	// 
		map<short, __int64>::const_iterator itr =  m_mapQuestHistory.begin();
		for (; itr != m_mapQuestHistory.end(); ++itr){
			for (int i = 0; i < (int)sizeof(__int64) * 8; ++i){
				if (GetBit(itr->second, i) == 1){
					unsigned short nQuestID = sizeof(__int64) * 8 * itr->first + i;
					CQuestDoc *pQuestDoc = ConfigManager::Instance().GetQuestManager().GetQuestByID(nQuestID);
					if (pQuestDoc != NULL && pQuestDoc->GetType() == nType){
						nCount++;
					}
				}
			}
		}
	} else {
		list< CRepeatedHistoryQuest >::const_iterator itr = m_listRepeatedQuest.begin();	
		for (; itr != m_listRepeatedQuest.end(); ++itr) {
			CQuestDoc *pQuestDoc = ConfigManager::Instance().GetQuestManager().GetQuestByID(itr->m_nQuestID);
			if (pQuestDoc != NULL && pQuestDoc->GetType() == nType){
				nCount += itr->m_nCount;
			}
		}
	}

	return nCount;
}

unsigned int CEntityQuestComponent::GetCompletedAllQuestCount()
{
	int nCount = 0;
	map<short, __int64>::const_iterator itr = m_mapQuestHistory.begin();	// 历史记录里面的任务，包含可重复的任务
	for (; itr != m_mapQuestHistory.end(); ++itr) {
		nCount += ::CountOne(itr->second);
	}

	list<CRepeatedHistoryQuest>::const_iterator itrRe = m_listRepeatedQuest.begin();
	for (; itrRe != m_listRepeatedQuest.end(); ++itrRe) {
		nCount += itrRe->m_nCount - 1;	// -1 是因为在历史记录已经记录
	}

	//WriteLog("completed quest %d\n", nCount);
	return nCount;
}

void CEntityQuestComponent::_NotifyPlayerQuestStateChange(unsigned short nQuestID, unsigned char nState)
{
	if (nQuestID > 0){
		GameMsg_S2C_QuestStateChange changeMsg(nQuestID, nState);
		_SendPlayerMsg(&changeMsg);
	}
}

void CEntityQuestComponent::_UpdateQuestTotalCount()
{
	if (m_pAttr != NULL){
		m_pAttr->m_nQuestDone++;
	}
}

CRepeatedHistoryQuest * CEntityQuestComponent::_GetRepeatedQuestDoneBy(unsigned short nQuestID)
{
	list< CRepeatedHistoryQuest >::iterator itr = find(m_listRepeatedQuest.begin(), m_listRepeatedQuest.end(), nQuestID);
	if (itr != m_listRepeatedQuest.end()) {
		return &(*itr);
	}

	return NULL;
}


void CEntityQuestComponent::_AddRepeatedQuestDone(CRepeatedHistoryQuest &info)
{
	list<CRepeatedHistoryQuest>::const_iterator itr =  find(m_listRepeatedQuest.begin(), m_listRepeatedQuest.end(), info);
	if (itr == m_listRepeatedQuest.end()){
		m_listRepeatedQuest.push_back(info);
	}
}

bool CEntityQuestComponent::_HasDoneRepeatedQuest(unsigned short nQuestID)
{
	list<CRepeatedHistoryQuest>::const_iterator itr =  find(m_listRepeatedQuest.begin(), m_listRepeatedQuest.end(), CRepeatedHistoryQuest(nQuestID));

	return itr != m_listRepeatedQuest.end();
}

bool CEntityQuestComponent::_HasDoneRepeatedQuestToday(unsigned short nQuestID)
{
	list<CRepeatedHistoryQuest>::const_iterator itr =  find(m_listRepeatedQuest.begin(), m_listRepeatedQuest.end(), CRepeatedHistoryQuest(nQuestID));

	if (itr != m_listRepeatedQuest.end())
	{
		return IsTimeIsToday(itr->m_nTime);
	}

	return true;
}

bool CEntityQuestComponent::_IsRepeatedQuest(unsigned short nType)
{
	return nType == Quest_Daily; 
}

bool CEntityQuestComponent::_CanReachSpecailCond(unsigned int nQuestID) const
{
	for (std::map<unsigned short, CPrivateQuest>::const_iterator itr = CSpecailQuestMgr::Instance().PrivateQuestMap().begin(); itr != CSpecailQuestMgr::Instance().PrivateQuestMap().end(); ++itr) {
		if (nQuestID == itr->second.m_nQuestID) {
			return m_pNet->GetVerType() == Version_Charge;
		}
	}

	return true;
}

//完成任务日志
void CEntityQuestComponent::_LogRoleQuest(unsigned int nRoleID, unsigned int nQuestID)
{
	CRoleQuestLog * pRoleQuestLog = new CRoleQuestLog();
	pRoleQuestLog->m_nRoleID = nRoleID;
	pRoleQuestLog->m_nQuestID = nQuestID;
	AddQuery(QUERY_LogRoleQuest, 0, pRoleQuestLog);
}

void CEntityQuestComponent::_SendQuestStaticBinData()
{
	//long nBinDataSize = CDataManager::Instance().GetQuestManager().QuestStaticBinDataSize();
	//char *szBinData = CDataManager::Instance().GetQuestManager().QuestStaticBinData();
	//CParamPool questPool(szBinData, nBinDataSize);
	//GameMsg_S2C_SendQuestStaticBinData msgSend;
	//msgSend.m_nBinDataSize = nBinDataSize;
	//questPool.GetMemory(msgSend.m_szBinData, msgSend.m_nBinDataSize);	
	//_SendPlayerMsg(&msgSend);
	//WriteLog("--send quest static data size[%d]\n", nBinDataSize);
}

void CEntityQuestComponent::_SendFresherUnReceivedReward()
{
	if (CFresherQuestDataManager::Instance().IsOpen() 
		&& m_pAttr->m_nRoleCreateTime >= CFresherQuestDataManager::Instance().m_nStartTime
		&& m_nFresherDay > CFresherQuestDataManager::Instance().m_nQuestDuration
		&& m_nFresherDay <= CFresherQuestDataManager::Instance().m_nQuestDuration + 1)
	{
		std::map<int, CFresherQuestDoc *>::iterator it = CFresherQuestDataManager::Instance().m_mapFresherQuest.begin();
		while (it != CFresherQuestDataManager::Instance().m_mapFresherQuest.end())
		{
			if (it->second != NULL && CanCompleteFresherQuest(it->second))
			{
                if (!HasReceivedFresherQuestReward(it->second->m_nQuestID))
                {
                    //普通奖励
                    SendPlayerFresherQuestReward(it->second, true, false);
                    if (m_pVip != NULL && m_pVip->IsVIP())
                    {
                        //Vip奖励
                        SendPlayerFresherQuestReward(it->second, true, true);
                    }
                }
			}
			++it;
		}
	}
}

void CEntityQuestComponent::_SendFresherQuestAndReward()
{
	_SendFresherQuest();
	_SendFresherUnReceivedReward();
}

bool CEntityQuestComponent::CanCompleteFresherQuest(CFresherQuestDoc *pDoc)
{
	switch (pDoc->m_nTargetType)
	{
	case TARGET_LEVEL:
		return (m_pAttr->GetLevel() >= pDoc->m_nTarget);
	case TARGET_FRIENDCOUNT:
		return (m_pFriend != NULL && m_pFriend->FriendCount() >= pDoc->m_nTarget);
	case TARGET_MUSICGRADE:
		return (m_nFresherBestMusicGrade >= pDoc->m_nTarget);
	case TARGET_BUYITEMCOUNT:
		return (m_nMallBuyCount >= pDoc->m_nTarget);
	case TARGET_JOINDANCEGROUP:
		return (m_pGroup->GetDanceGroupInfo() != NULL);
	case TARGET_HASLOVER:
		return (!m_pCouple->IsSingle());
	default:
		return true;
	}
}

unsigned char CEntityQuestComponent::FresherDayToEventID(int nDay)
{
    switch (nDay)
    {
    case 1: return eEvent_FresherQuestReward_Day1;
    case 2: return eEvent_FresherQuestReward_Day2;
    case 3: return eEvent_FresherQuestReward_Day3;
    case 4: return eEvent_FresherQuestReward_Day4;
    case 5: return eEvent_FresherQuestReward_Day5;
    case 6: return eEvent_FresherQuestReward_Day6;
    case 7: return eEvent_FresherQuestReward_Day7;
    default: WriteLog(LOGLEVEL_ERROR, "Error FresherQuest unexpected day: %d", nDay); return 0;
    }
}

void CEntityQuestComponent::SendPlayerFresherQuestReward(CFresherQuestDoc *pDoc, bool bSendMail, bool bIsVipReward)
{
	// update database and memory
	m_setFresherQuestReward.insert(pDoc->m_nQuestID);
	unsigned short *pQuestID = new unsigned short;
    *pQuestID = (unsigned short)pDoc->m_nQuestID;
	unsigned char nEventID = FresherDayToEventID(pDoc->m_nDay);
	CDelHelper delHelper(pQuestID);
	AddQuery(QUERY_FresherQuest_AddReward, m_pAttr->m_nRoleID, pQuestID, nEventID, NULL, &delHelper);

    std::list<CItem> *pListItem = NULL;
    int nMoney = 0;
    std::string strMailTitle;
    std::string strMailContent;
    if (!bIsVipReward)
    {
        // send item
        if (m_pAttr->m_nSex == ESexType_Male)
        {
            pListItem = &pDoc->m_MaleReward.m_listItems;
            nMoney = pDoc->m_MaleReward.m_nMoney;
        }
        else
        {
            pListItem = &pDoc->m_FemaleReward.m_listItems;
            nMoney = pDoc->m_FemaleReward.m_nMoney;
        }

        strMailTitle = CFresherQuestDataManager::Instance().m_strMailTitle;
        strMailContent = CFresherQuestDataManager::Instance().m_strMailContent;
    }
    else
    {
        if (m_pAttr->m_nSex == ESexType_Male)
        {
            pListItem = &pDoc->m_VipMaleReward.m_listItems;
            nMoney = pDoc->m_VipMaleReward.m_nMoney;
        }
        else
        {
            pListItem = &pDoc->m_VipFemaleReward.m_listItems;
            nMoney = pDoc->m_VipFemaleReward.m_nMoney;
        }

        strMailTitle = CFresherQuestDataManager::Instance().m_strVipRewardTitle;
        strMailContent = CFresherQuestDataManager::Instance().m_strVipRewardContent;
    }


    if (bSendMail)
    {
        CMailMgr::Instance().SendSystemMailToPlayer( m_pAttr->m_nRoleID, EMailType_FresherQuest, strMailTitle, strMailContent, *pListItem );
        if (nMoney > 0)
        {
            CMailMgr::Instance().SendSystemMailToPlayer( m_pAttr->m_nRoleID, EMailType_FresherQuest, strMailTitle, strMailContent, nMoney );
        }
    }
    else
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), *pListItem, EItemAction_Add_FresherQuest, 0, true, EMailType_FresherQuest, strMailTitle, strMailContent);
        m_pAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_FresherQuest, 0);
    }
}

void CEntityQuestComponent::_GetFresherQuestReward(int nQuestID, bool bIsExtraReward)
{
    int nDay = CFresherQuestDataManager::Instance().ToDayIndex(nQuestID);
    CFresherQuestDoc *pDoc = CFresherQuestDataManager::Instance().GetFresherQuest(nDay);
    std::string strError = "";

    if (nDay <= 0 || nDay > CFresherQuestDataManager::Instance().m_nQuestDuration || pDoc == NULL)
    {
        strError = "FresherQuest_InvalidQuestID";
    }
    else if (!CFresherQuestDataManager::Instance().IsOpen()
        || m_nFresherDay > CFresherQuestDataManager::Instance().m_nQuestDuration)
    {
        strError = "FresherQuest_Closed";
    }
    else if (HasReceivedFresherQuestReward(nQuestID))
    {
        if (!bIsExtraReward)
        {
            strError = "FresherQuest_HasReceived";
        }
    }
    else if (m_nFresherDay < nDay)
    {
        strError = "FresherQuest_TooEarly";
    }
    else if (!CanCompleteFresherQuest(pDoc))
    {
        strError = "FresherQuest_NotCompleted";
    }

    if (!strError.empty())
    {
        GameMsg_S2C_GetFresherQuestRewardFail fail;
        fail.m_strError = strError;
        m_pNet->SendPlayerMsg(&fail);
    }
    else
    {
        if (!bIsExtraReward)
        {
            //普通奖励
            SendPlayerFresherQuestReward(pDoc, false, false);
            //在普通奖励下设置最后领取任务
            m_nFresherLastRewardQuest = nQuestID;
            //Vip奖励
            if (m_pVip != NULL && m_pVip->IsVIP())
            {
                SendPlayerFresherQuestReward(pDoc, false, true);
                //领取VIP奖励后将该值清空
                m_nFresherLastRewardQuest = 0;
            }
        }
        else
        {
            //只有VIP奖励
            SendPlayerFresherQuestReward(pDoc,false,true);
            m_nFresherLastRewardQuest = 0;
        }

        // send message
        GameMsg_S2C_GetFresherQuestRewardSuc suc;
        suc.m_nQuestID = (unsigned short)nQuestID;
        m_pNet->SendPlayerMsg(&suc);
    }
}

void CEntityQuestComponent::OnGetFresherQuestReward(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	GameMsg_C2S_GetFresherQuestReward *pMsg = (GameMsg_C2S_GetFresherQuestReward *)&msg;

    _GetFresherQuestReward(pMsg->m_nQuestID,false);
}

void CEntityQuestComponent::PushFresherQuestStateChange()
{
	//取消实时推送
	if (CFresherQuestDataManager::Instance().IsOpen() 
		&& m_nFresherDay > 0
		&& m_nFresherDay <= CFresherQuestDataManager::Instance().m_nQuestDuration)
	{
		std::map<int, CFresherQuestDoc *>::iterator it = CFresherQuestDataManager::Instance().m_mapFresherQuest.begin();
		while (it != CFresherQuestDataManager::Instance().m_mapFresherQuest.end())
		{
			if (it->second != NULL && !HasReceivedFresherQuestReward(it->second->m_nQuestID) )
			{
				GameMsg_S2C_UnRewardFresherQuest msg;
                msg.m_nQuestID = (unsigned short)it->second->m_nQuestID;
				msg.m_nTargetValue = it->second->m_nTarget;
				if ( m_nFresherDay >= it->second->m_nDay && m_nFresherDay <= it->second->m_nDay + 1 && CanCompleteFresherQuest(it->second))
				{
					msg.m_nQuestState = eQuestState_Completed;
				}
				else
				{
					msg.m_nQuestState = eQuestState_Accepted;
				}
				_SendPlayerMsg(&msg);
			}
			++it;
		}	
	}
}

void CEntityQuestComponent::_SendFresherQuest()
{
	GameMsg_S2C_SendFresherQuest msgSend;
	if (CFresherQuestDataManager::Instance().IsOpen() 
		&& m_nFresherDay > 0
		&& m_nFresherDay <= CFresherQuestDataManager::Instance().m_nQuestDuration)
	{
		msgSend.m_bIsOpen = true;
		msgSend.m_nFresherQuestID = CFresherQuestDataManager::Instance().GetFresherQuestID(m_nFresherDay);
		
		std::map<int, CFresherQuestDoc *>::iterator it = CFresherQuestDataManager::Instance().m_mapFresherQuest.begin();
		while (it != CFresherQuestDataManager::Instance().m_mapFresherQuest.end())
		{
			CFresherQuestDoc *pDoc = it->second;
			if (pDoc != NULL)
			{
				CQuestListEntry questEntry;
                questEntry.m_nQuestID = (unsigned short)pDoc->m_nQuestID;
				questEntry.m_nState = ((m_nFresherDay >= pDoc->m_nDay && CanCompleteFresherQuest(pDoc)) ? eQuestState_Completed : eQuestState_Accepted);
				if (HasReceivedFresherQuestReward(pDoc->m_nQuestID))
				{
					questEntry.m_nState = eQuestState_HasReward;
				}
				CQuestTargetSend target;
                target.m_nType = (unsigned char)pDoc->m_nTargetType;
				target.m_nIndex = 0;
				target.m_nRequiredCount = pDoc->m_nTarget;
				if (questEntry.m_nState >= eQuestState_Completed)
				{
					target.m_nCompletedCount = target.m_nRequiredCount;
				}
				else
				{
					switch (pDoc->m_nTargetType)
					{
					case TARGET_LEVEL:
						target.m_nCompletedCount = m_pAttr->GetLevel();
						break;
					case TARGET_FRIENDCOUNT:
						if (m_pFriend != NULL)
						{
							target.m_nCompletedCount = m_pFriend->FriendCount();
						}
						else
						{
							target.m_nCompletedCount = 0;
						}
						break;
					case TARGET_MUSICGRADE:
						target.m_nCompletedCount = m_nFresherBestMusicGrade;
						break;
					case TARGET_BUYITEMCOUNT:
						target.m_nCompletedCount = m_nMallBuyCount;
						break;
					case TARGET_JOINDANCEGROUP:
						target.m_nCompletedCount = (m_pGroup->GetDanceGroupInfo() != NULL ? 1 : 0);
						break;
					case TARGET_HASLOVER:
						target.m_nCompletedCount = (m_pCouple->IsSingle() ? 0 : 1);
						break;
					default:
						break;
					}
				}

				questEntry.m_listTargetInfo.push_back(target);
				msgSend.m_listQuestInfo.push_back(questEntry);
				msgSend.m_nSex = (unsigned char)m_pAttr->GetSex();
			}
			++it;
		}
	}
	else
	{
		msgSend.m_bIsOpen = false;
		msgSend.m_strError = "FresherQuest_Timeout";
	}
	_SendPlayerMsg(&msgSend);
}

bool CEntityQuestComponent::HasReceivedFresherQuestReward(int nQuestID)
{
	bool bRes = (m_setFresherQuestReward.find(nQuestID) != m_setFresherQuestReward.end()) ;
    //if (bRes && m_pVip != NULL && m_pVip->IsVIP())
    //{
    //    //如果已经领取了奖励，那么判断是否VIP，并且在非VIP状态下完成了一次七日目标任务
    //    bRes = (m_nFresherLastRewardQuest == 0);
    //}
    return bRes;
}

void CEntityQuestComponent::OnBuyItem()
{
	m_nMallBuyCount++;
	AddQuery(QUERY_Mall_AddMallBuyCount, m_pAttr->m_nRoleID, NULL, m_nMallBuyCount, NULL);
	PushFresherQuestStateChange();
}

void CEntityQuestComponent::OnGetFresherQuestList(GameMsg_Base &msg, CSlotPeer & slotPeer)
{
	_SendFresherQuest();
}

void CEntityQuestComponent::GetFresherVIPExtraReward()
{
    if (m_nFresherLastRewardQuest != 0)
    {
        _GetFresherQuestReward(m_nFresherLastRewardQuest,true);
        //领取VIP奖励后将最后完成七日目标任务清0
        m_nFresherLastRewardQuest = 0;
    }
}

