#include "EntityQuestNewComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../../nsytworld/QuestCondition.h"
#include "../../nsytworld/QuestComplete.h"
#include "../../nsytworld/QuestMan.h"
#include "../friend/EntityFriendComponent.h"
#include "../logic/EntityTimerComponent.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../logic/EntityComponentEventID.h"
#include "../util/CommonFunc.h"
#include "GameMsg_C2S_GetQuestList.h"
#include "GameMsg_C2S_GetQuestReward.h"
#include "GameMsg_C2S_QuestRead.h"
#include "GameMsg_C2S_AcceptQuest.h"
#include "GameMsg_S2C_GetQuestListResult.h"
#include "GameMsg_S2C_GetQuestRewardResult.h"
#include "../../socket/GameMsg_Base.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../../socket/Utility.h"
#include "QuestListEntry.h"
#include "QuestTargetSend.h"
#include "GameMsg_S2C_Quest.h"
#include "GameMsg_S2C_QuestStateChange.h"
#include "GameMsg_S2C_AcceptQuestResult.h"
#include "../item/ItemProcess.h"
#include "../CheckIn/EntityCheckInComponent.h"
#include "../Dungeon/EntityDungeonComponent.h"

#define QUESTLOG "QUESTLOG:"

#ifdef WIN32
#pragma warning(disable:4061)
#endif

template <typename TParamKey, typename TParamV>
std::string EncodeQuestParam(const std::map<unsigned short, std::map<TParamKey, TParamV> > &data, unsigned short nQuestID)
{
    typename std::map<unsigned short, std::map<TParamKey, TParamV> >::const_iterator it = data.find(nQuestID);
    if (it == data.end())
    {
        return "";
    }
    std::string retString;
    typename std::map<TParamKey, TParamV>::const_iterator its = it->second.begin();
    for ( ; its != it->second.end() ; ++its )
    {
        std::string strBuff;
        Formatter(strBuff) << its->first << ',' << its->second << '|';
        retString += strBuff;
    }

    return retString;
}



CEntityQuestNewComponent::CEntityQuestNewComponent()
  : CEntityComponent(true, true)
  , m_nDirtyTime(0)
{
	m_pAttr = NULL;
	m_pItem = NULL;
	m_pNet = NULL;
	m_pRoleVIP = NULL;
	m_pCouple = NULL;
	m_pDanceGroup = NULL;
	m_pFriend = NULL;
	m_pTimer = NULL;
    m_pCheckIn = NULL;
    m_pDungeon = NULL;
	//m_GuideQuestID = 0;
	m_bSendQuestDataToClient = false;
    m_UnRewardVipQuest = 0;
}

CEntityQuestNewComponent::~CEntityQuestNewComponent()
{

}

void CEntityQuestNewComponent::Start()
{
    m_pAttr = GetComponent<CRoleAttribute>();
    m_pItem = GetComponent<CRoleItem>();
    m_pNet = GetComponent<CRoleNet>();
    m_pRoleVIP = GetComponent<CRoleVIP>();
    m_pCouple = GetComponent<CRoleCouple>();
    m_pDanceGroup = GetComponent<CRoleDanceGroup>();
    m_pFriend = GetComponent<CRoleFriend>();
    m_pTimer = GetComponent<CRoleTimer>();
    m_pCheckIn = GetComponent<CRoleCheckIn>();
    m_pDungeon = GetComponent<CRoleDungeon>();
}

void CEntityQuestNewComponent::RegComponentNetMsgMap()
{

	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestReward);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestList);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_QuestRead);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AcceptQuest);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ClientCompleteQuest);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetQuestState);

	RegMsgProcessor(MSG_C2S_GetQuestReward, &CEntityQuestNewComponent::OnGetQuestReward);	
	RegMsgProcessor(MSG_C2S_GetQuestList, &CEntityQuestNewComponent::OnGetQuestList);
	RegMsgProcessor(MSG_C2S_QuestRead, &CEntityQuestNewComponent::OnQuestRead);
	RegMsgProcessor(MSG_C2S_AcceptQuest, &CEntityQuestNewComponent::OnAcceptQuest);
	RegMsgProcessor(MSG_C2S_ClientCompleteQuest,&CEntityQuestNewComponent::OnClientCompleteQuest);
	RegMsgProcessor(MSG_C2S_GetQuestState,&CEntityQuestNewComponent::OnGetQuestState);

	//数据库
	//RegQueryProcessor(QUERY_RemoveRoleQuest, &CEntityQuestNewComponent::_RemoveQuestToDBCallBack);
	//RegQueryProcessor(QUERY_AddRoleQuestHistory, &CEntityQuestNewComponent::_AddQuestHistoryToDBCallBack);
	//RegQueryProcessor(QUERY_UpdateQuestInfo, &CEntityQuestNewComponent::_UpdateQuestInfoCallBack);
}

void CEntityQuestNewComponent::SerializeComponent(CParamPool &IOBuff)
{

}

bool CEntityQuestNewComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
	std::list<QUESTNEWINFO>::iterator it = pRoleInfoDB->listQuestNew.begin();
	for ( ; it != pRoleInfoDB->listQuestNew.end() ; it++)
	{
		if (it->m_DestroyType == eQuestDestroy_None)
		{
			switch(it->m_State)
			{
			case eQuestState_None:
				{
					CQuestDocNew* pDoc = _GetQuestDoc(it->m_QuestID);
					if (pDoc != NULL)
					{
						m_UnAcceptQuest.insert(it->m_QuestID);
					}
				}
				break;
			case eQuestState_Accepted:
			case eQuestState_Completed:
				{
					//已经接受了任务，但是没有完成或没有领奖
					_AddAcceptedQuest(&(*it));
				}
				break;
			case eQuestState_HasReward:
            case eQuestState_Destroy:
				{ // 奖励已经领取
					//已经完成了任务
//					_AddCompletedQuest(&(*it));
                    _AddHistoryQuest(it->m_QuestID);
				}
				break;
			}
		}
		else
		{
 			_AddHistoryQuest(it->m_QuestID);
		}
		m_QuestDetailInfo.insert(make_pair(it->m_QuestID,*it));
	}

	return true;
}

bool CEntityQuestNewComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
	return false;
}

bool CEntityQuestNewComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    pRoleInfoCache->listQuestNew.clear();
    // 把数据缓存到roleInfoDb中
    std::map<unsigned short, QUESTNEWINFO>::const_iterator iter = m_QuestDetailInfo.begin();
    for (; iter != m_QuestDetailInfo.end(); ++iter)
    {
        pRoleInfoCache->listQuestNew.push_back(iter->second);
    }
    
    return true;
}


void CEntityQuestNewComponent::OnEvent(CComponentEvent& refEvent )
{
	if (refEvent.nID == eComponentEvent_DailyQuestUpdateTimer)
	{
        _CleanOverdueRepeatQuest();
        CheckCurrentDaily();
        _CheckCanAcceptQuest(0, false);
        _CheckCanCompleteQuest();

		//重新更新一下玩家任务列表
		_SendQuestList();

		m_pTimer->AddTimer(GetNextDayStartTime(time(NULL)),eComponentEvent_DailyQuestUpdateTimer,0);
	}
	if (refEvent.nID == eComponentEvent_QuestExpiredTimer)
	{
		unsigned short * pQuestID = (unsigned short *)refEvent.m_pData;

        if (m_HistroyQuest.find(*pQuestID) == m_HistroyQuest.end())
        {
            ENSURE_WITH_LOG_CMD(NULL != _GetQuestDoc(*pQuestID), ,"role[%u] quest[%u] miss config..", m_pAttr->GetRoleID(), *pQuestID);
            m_CurrentQuest.erase(*pQuestID);
            m_UnAcceptQuest.erase(*pQuestID);
            m_HistroyQuest.insert(*pQuestID);
            _UpdateQuestState(*pQuestID, eQuestState_Destroy, eQuestDestroy_DateTime);
        }
	}
    if (refEvent.nID == eComponentEvent_QuestAcceptTimer)
    {
        unsigned short * pQuestID = (unsigned short *)refEvent.m_pData;

        if (m_UnAcceptQuest.find(*pQuestID) != m_UnAcceptQuest.end()) 
        {
            CQuestDocNew *pQuestDoc = _GetQuestDoc(*pQuestID);
            ENSURE_WITH_LOG_CMD(NULL != pQuestDoc, return, "role[%u] quest[%u] miss config..", m_pAttr->GetRoleID(), *pQuestID);
            if (_CanAcceptQuest(pQuestDoc))
            {
                std::vector<unsigned short> idList;
                idList.push_back(*pQuestID);
                _AcceptQuestList(idList, true); // 通知到客户端
            }
        }
    }
}

void CEntityQuestNewComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
    unsigned int kDirtyQuestWriteInterval = 1000 * Random(600, 1800); // 10-30分钟检测脏数据
    m_nDirtyTime += nTimeElapsed;
    if (m_nDirtyTime > kDirtyQuestWriteInterval)
    {
        WriteDirtyQuest();
        m_nDirtyTime = 0;
    }
}

void CEntityQuestNewComponent::WriteDirtyQuest()
{
    if (m_DirtyQuest.empty())
        return;

    for (std::map<unsigned short, QUESTNEWINFO>::iterator it = m_DirtyQuest.begin(); it != m_DirtyQuest.end(); ++it)
    {
        WriteDirtyQuest(it->second);
    }
    m_DirtyQuest.clear();
}

void CEntityQuestNewComponent::WriteDirtyQuest(const QUESTNEWINFO &questInfo)
{
    QUESTNEWINFO* pNewQuestInfo = new QUESTNEWINFO(questInfo);
    AddQuery(QUERY_Update_Quest, 0, pNewQuestInfo, 0, NULL);
}

void CEntityQuestNewComponent::OnLogin()
{
    _CleanOverdueRepeatQuest();
	// /删除过期的任务
    CheckOverdueQuest();
    CheckCurrentDaily();
    CheckRealHistroy();
    //重新检测可接任务
    _CheckCanAcceptQuest(0,false);
    _CheckCanCompleteQuest();

    
    for (QuestIDSet::iterator iter = m_CurrentQuest.begin(); iter != m_CurrentQuest.end(); ++iter)
    {
        _AddDestroyQuestTimer(_GetQuestDoc(*iter));
    }


	SendQuestData();
	m_bSendQuestDataToClient = true;

	if (m_pTimer != NULL)
	{
		m_pTimer->AddTimer(GetNextDayStartTime(time(NULL)),eComponentEvent_DailyQuestUpdateTimer,0);
	}
    AddTimerStartQuest(); // 处理完毕

	_SendQuestList();
	_PushCanCompleteQuest();
}

void CEntityQuestNewComponent::OnLogout()
{
    WriteDirtyQuest();
}

void CEntityQuestNewComponent::SendExtraStaticData()
{
}

bool CEntityQuestNewComponent::_CanAcceptQuest(CQuestDocNew* pDoc,int nParam)
{
	if (m_pAttr == NULL)
	{
		return false;
	}
	
	//当前任务如果已经接过或完成过，则不能再次接取，（可重复任务则按次数计算是否可以继续接）
	if (!_CanAcceptQuestRepeat(pDoc))
	{
		return false;
	}

	//任务必须在可接区间
	unsigned int nNow = (unsigned int )time(NULL);
	if ((pDoc->m_BaseInfo.StartTime != 0 && nNow < pDoc->m_BaseInfo.StartTime) || 
        (pDoc->m_BaseInfo.EndTime != 0 && nNow > pDoc->m_BaseInfo.EndTime))
	{
		return false;
	}

	std::map<EQuest_Condition_Type,IQuestCondition*>::iterator it = pDoc->m_Condition.begin();
	for (; it != pDoc->m_Condition.end() ; it++)
	{
		if (!_CheckCondition(it->second,nParam))
		{
			return false;
		}
	}
	return true;
}

bool CEntityQuestNewComponent::_CanCompleteQuest(CQuestDocNew* pDoc,int nParam)
{
	if (m_CurrentQuest.find(pDoc->m_BaseInfo.ID) == m_CurrentQuest.end())
	{
		return false;
	}
	bool bChecked = true;
	map<EQuest_Complete_Type,IQuestComplete*>::iterator it = pDoc->m_Complete.begin();
	for ( ; it != pDoc->m_Complete.end() ; it++)
	{
		if (!_CheckComplete(it->second,nParam))
		{
			bChecked = false;
			break;
		}
	}
	return bChecked;
}

bool CEntityQuestNewComponent::_CheckCondition(IQuestCondition* pCond,int nParam)
{
    if (m_pAttr == NULL || m_pItem == NULL || m_pRoleVIP == NULL || m_pNet == NULL)
    {
        return false;
    }

    switch(pCond->GetConditionType() )
    {
    case EQuest_Condition_LevelMin:
        return pCond->CheckCondition(m_pAttr->m_nLevel);
    case EQuest_Condition_VipLevel:
        return pCond->CheckCondition(m_pRoleVIP->m_VIPInfo.m_nLevel);
    case EQuest_Condition_Sex:
        return pCond->CheckCondition(m_pAttr->m_nSex);
    case EQuest_Condition_XingZuo:
        return pCond->CheckCondition(m_pAttr->m_nConstellation);
    case EQuest_Condition_HasLover:
        return pCond->CheckCondition(_HasLover() ? 1 : 0);
    case EQuest_Condition_HasMarried:
        return pCond->CheckCondition(_HasMarried() ? 1 : 0);
    case EQuest_Condition_Intimacy:
        return pCond->CheckCondition(m_pCouple->m_coupleInfo.GetIntimacy() );
    case EQuest_Condition_PreQuest:
        return _CheckConditionPreQuest(pCond);
    case EQuest_Condition_ObtainItem:
        return _CheckConditionHasItem(pCond);
    case EQuest_Condition_UseItem:
        return _CheckConditionUseItem(pCond, (unsigned short)nParam);
    case EQuest_Condition_HasDanceGroup:
        return _HasDanceGroup();
    case EQuest_Condition_DanceGroupContribution:
        return pCond->CheckCondition(_GetDanceGroupContribution() );
    case EQuest_Condition_DanceGroupLevel:
        return pCond->CheckCondition(_GetDanceGroupLevel() );
    case EQuest_Condition_Item:
        return _CheckConditionItem(pCond);
    case EQuest_Condition_ChargeVersion:
        return pCond->CheckCondition(m_pNet->GetVerType() );
    default:
        WriteLog(LOGLEVEL_WARNING, "%s Not Implemented Condition : %d .\n", QUESTLOG,(int)pCond->GetConditionType() );
        return false;
    }
}

bool CEntityQuestNewComponent::_CheckConditionHasItem(IQuestCondition* pCond)
{
	if (pCond->GetConditionType() != EQuest_Condition_ObtainItem)
	{
		return false;
	}
	QuestConditionObtainItem* pQuest = (QuestConditionObtainItem*)pCond;
	std::vector<IQuestCondition* >::iterator it = pQuest->m_SubCondition.begin();
	for (;it != pQuest->m_SubCondition.end() ; it++)
	{
		if (!_CheckConditionItem(*it))
		{
			return false;
		}
	}
	return true;
}

bool CEntityQuestNewComponent::_CheckConditionItem(IQuestCondition* pCond)
{
    if (pCond->GetConditionType() != EQuest_Condition_Item || m_pItem == NULL)
        return false;

    QuestConditionItem *pQuest = (QuestConditionItem *)pCond;
    unsigned int nItemNumber = m_pItem->GetItemCount(pQuest->m_Item.m_nItemID, true, true);

    if (nItemNumber >= (unsigned int)pQuest->m_Item.m_nItemCount)
        return true;

    return false;
}

bool CEntityQuestNewComponent::_CheckConditionPreQuest(IQuestCondition* pCond)
{
	if (pCond->GetConditionType() != EQuest_Condition_PreQuest)
	{
		return false;
	}
	QuestConditionPreQuest* pQuest = (QuestConditionPreQuest*)pCond;
	return m_HistroyQuest.find(pQuest->m_nPreQuestID) != m_HistroyQuest.end() && _GetQuestCompleteDayTimes(pQuest->m_nPreQuestID) > 0;
}

//////////////////////////////////////////////////////////////////////////
//使用物品触发条件限制为只使用一个物品，并且限制次数为1次
//可以和其他触发条件叠加
bool CEntityQuestNewComponent::_CheckConditionUseItem(IQuestCondition* pCond,itemtype_t itemID)
{
	if (pCond->GetConditionType() != EQuest_Condition_UseItem)
	{
		return false;
	}
	QuestConditionUseItem* pQuest = (QuestConditionUseItem*) pCond;
	std::vector<IQuestCondition* >::iterator it = pQuest->m_SubCondition.begin();
	if (it != pQuest->m_SubCondition.end())
	{
		QuestConditionItem* pItemQuest = (QuestConditionItem*)(*it);
		return pItemQuest->m_Item.m_nItemID == itemID;
	}
	return false;
}


// bool CEntityQuestNewComponent::_CheckTimeType(char timeType)
// {
// 	int nDay = GetWeekDay();
// 	switch((EQuestTimeType)timeType)
// 	{
// 	case EQuestTimeType_Monday:
// 		return nDay == MONDAY;
// 	case EQuestTimeType_Tuesday:
// 		return nDay == TUESDAY;
// 	case EQuestTimeType_Wednesday:
// 		return nDay == WEDNESDAY;
// 	case EQuestTimeType_Thursday:
// 		return nDay == THURSDAY;
// 	case EQuestTimeType_Friday:
// 		return nDay == FRIDAY;
// 	case EQuestTimeType_Saturday:
// 		return nDay == SATURDAY;
// 	case EQuestTimeType_Sunday:
// 		return nDay == SUNDAY;
// 	}
// 	return true;
// }

bool CEntityQuestNewComponent::_CheckTimesLimit(QUESTNEWINFO* questNewInfo, CQuestDocNew* pDoc)
{
    ENSURE_WITH_LOG_CMD(NULL != questNewInfo && NULL != pDoc, return false, "error to get null QuestInfo or QuestDocInfo");
    
    if (pDoc->m_BaseInfo.Type == eQuestTypeNew_Daily)
    {// 每次次数具有有效性，无效的情况下，任务完成次数认为是0
     // 总次数不需要检测有效性。
        unsigned short dayTime = 0;
        if (DiffDays(questNewInfo->m_UpdateTime, time(NULL)) == 0)
        { // 检测有效性，
            dayTime = questNewInfo->m_DayTimes;
        }
        return pDoc->m_BaseInfo.DayTimes > dayTime &&
            pDoc->m_BaseInfo.CompleteTimes > questNewInfo->m_Times;
    }
    else if (pDoc->m_BaseInfo.Type == eQuestTypeNew_Activity)
        return pDoc->m_BaseInfo.CompleteTimes > questNewInfo->m_Times;

    // 其他类型不检测
    return false;
}

// ligh modify 20151109 ,进接受不检测是否可以接受
void CEntityQuestNewComponent::_AcceptQuest(CQuestDocNew* pDoc,/*bool bCleanDayTimes,*/bool bNotifyClient,bool bUpdateDB)
{
	//添加到已接任务中
	unsigned short id = pDoc->m_BaseInfo.ID;
	m_CurrentQuest.insert(id);

	std::map<unsigned short,QUESTNEWINFO>::iterator itDetail = m_QuestDetailInfo.find(id);
    if (itDetail != m_QuestDetailInfo.end())
	{
		//更新到数据库
		LOG_DEBUG<<"_AcceptQuest._UpdateQuestState , quest id = " << id  << " role name " << m_pAttr->m_szRoleName<< endl;
		_UpdateQuestState(id,eQuestState_Accepted, eQuestDestroy_None);
	}
	else
	{
		//插入到数据库中
		_InsertQuest(pDoc);
	}

	if(bNotifyClient)
	{
		//通知客户端接到新任务
		_NotifyAcceptQuest(pDoc);
	}

    _AddDestroyQuestTimer(pDoc);

    // 检测如果包含checkIn条件：
    do 
    {
        QuestCompleteUseFunction* p = (QuestCompleteUseFunction*)pDoc->GetComplete(EQuest_Complete_UseFunction);
        if (NULL == p)
            break;
        if (p->GetFunction() != EUF_RewardCheckIn)
            break; 

        if (!m_pCheckIn->IsTodayCheckIn())
            break;

        // 
        _AddUseFunctionCount(pDoc->m_BaseInfo.ID, p->GetFunction());

    } while (0);

	//if (pDoc->m_BaseInfo.Type == eQuestTypeNew_Guide)
	//{
	//	m_GuideQuestID = id;
	//	_PushGuideQuest();
	//}
}

bool CEntityQuestNewComponent::_CheckComplete(IQuestComplete* pComp,int nParam /* = 0 */)
{
	bool bChecked = false;
	switch(pComp->GetCompleteType())
	{
	case EQuest_Complete_Level:
		bChecked = m_pAttr != NULL && pComp->CheckCondition(m_pAttr->m_nLevel);
		break;
	case EQuest_Complete_FriendCount:
		bChecked = m_pFriend != NULL && pComp->CheckCondition(m_pFriend->m_mapFriend.size());
		break;
	case EQuest_Complete_VipLevel:
		bChecked = m_pRoleVIP != NULL && pComp->CheckCondition(m_pRoleVIP->m_VIPInfo.m_nLevel);
		break;
	case EQuest_Complete_HasLover:
		bChecked = m_pCouple != NULL && m_pCouple->m_coupleInfo.m_nCoupleDate > 0;
		break;
	case EQuest_Complete_HasMarried:
		bChecked = m_pCouple != NULL && m_pCouple->m_coupleInfo.m_nMarriageDate > 0;
		break;
	case EQuest_Complete_Intimacy:
		bChecked = m_pCouple != NULL && pComp->CheckCondition(m_pCouple->m_coupleInfo.GetIntimacy());
		break;
	case EQuest_Complete_HasDanceGroup:
		bChecked = _HasDanceGroup();
		break;
	case EQuest_Complete_DanceGroupContribution:
		bChecked = pComp->CheckCondition(_GetDanceGroupContribution());
		break;
	case EQuest_Complete_DanceGroupTotalContribution:
		bChecked = pComp->CheckCondition(_GetDanceGroupTotalContribution());
		break;
	case EQuest_Complete_DanceGroupTotalHonor:
		bChecked = pComp->CheckCondition(_GetDanceGroupTotalHonor());
		break;
	case EQuest_Complete_UseCloth:
		bChecked = _CheckUseCloth(pComp);
		break;
	case EQuest_Complete_UseItem:
		bChecked = _CheckUseItem(pComp);
		break;
	case EQuest_Complete_ObtainItem:
		bChecked = _CheckObtainItem(pComp);
		break;
	case EQuest_Complete_OpenWeb:
		break;
	case EQuest_Complete_UseFunction:
		{
			int nFunction = 0;
			int nTimes = _GetUseFunctionTimes(pComp->GetQuestID(),nFunction);
			bChecked = pComp->CheckCondition(nTimes,nFunction);
		}
		break;
	case  EQuest_Complete_UseMusic:
		bChecked = _CheckMusicCompleted(pComp);
		break;
	case EQuest_Complete_Item:
		bChecked = _CheckCompleteHasItem(pComp);
		break;
	case EQuest_Complete_OpenComponent:
		bChecked = _CheckCompleteOpenComponent(pComp);
		break;
	case EQuest_Complete_MatchMusicEnd:
		bChecked = pComp->CheckCondition(_GetMusicTimes(pComp->GetQuestID(),0));
		break;
    case EQuest_Complete_Magiclamp:
        {
            QuestCompleteMagiclamp *pMagicComp = (QuestCompleteMagiclamp *)pComp;
            bChecked = m_pDungeon->GetParcloseStarCount(EDungeonType_Mainline, 0, pMagicComp->m_nStageID) >= pMagicComp->m_nStar;
        }
        break;
    case EQuest_Complete_BuyItem:
        bChecked = _CheckBuyItem(pComp);
        break;

    case EQuest_Complete_ClotheEffectReplace:
        {
            QuestRecordClotheEffectReplace *pRec = m_recordManager.GetQuestRecord<QuestRecordClotheEffectReplace>(pComp->GetQuestID());
            bChecked = pComp->CheckCondition(NULL == pRec ? 0 : pRec->m_num);
        }
        break;

    case EQuest_Complete_ClotheEffectLevelUp:
    {
        QuestRecordClotheEffectLevelUp *pRec = m_recordManager.GetQuestRecord<QuestRecordClotheEffectLevelUp>(pComp->GetQuestID());
        bChecked = pComp->CheckCondition(NULL == pRec ? 0 : pRec->m_num);
    }
        break;
    case EQuest_Complete_MoneyMagicArray:
    {
        QuestRecordMoneyMagicArray *pRec = m_recordManager.GetQuestRecord<QuestRecordMoneyMagicArray>(pComp->GetQuestID());
        bChecked = pComp->CheckCondition(NULL == pRec ? 0 : pRec->m_num);
    }
        break;
    case EQuest_Complete_CoinMagicArray:
    {
        QuestRecordCoinMagicArray *pRec = m_recordManager.GetQuestRecord<QuestRecordCoinMagicArray>(pComp->GetQuestID());
        bChecked = pComp->CheckCondition(NULL == pRec ? 0 : pRec->m_num);
    }
        break;

	default:
		WriteLog(LOGLEVEL_WARNING, "%s Not Implemented Complete : %d .\n" , QUESTLOG,(int)pComp->GetCompleteType());
		break;
	}
	return bChecked;
}

bool CEntityQuestNewComponent::_CheckCompleteMusic(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo)
{
	bool bCheck = false;
	switch(pComp->GetCompleteType())
	{
	case EMusic_Complete_Scene:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nSceneID);
		break;
	case EMusic_Complete_Diff:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nMusicDifficulty);
		break;
	case EMusic_Complete_SceneHuman:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nPlayerNum);
		break;
	case EMusic_Complete_SceneFriend:
		bCheck = pComp->CheckCondition(pMusicInfo->m_FriendNum);
		break;
	case EMusic_Complete_HasLover:
		bCheck = pComp->CheckCondition(pMusicInfo->m_LoverNum);
		break;
	case EMusic_Complete_DanceGroup:
		bCheck = pComp->CheckCondition(pMusicInfo->m_GroupNum);
		break;
	case EMusic_Complete_Rank:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nRank);
		break;
	case EMusic_Complete_EvalLevel:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nGrade);
		break;
	case EMusic_Complete_Free:
		break;
	case EMusic_Complete_Team:
		break;
	case EMusic_Complete_Score:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nScore);
		break;
	case EMusic_Complete_Perfect:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nPerfectNum);
		break;
	case EMusic_Complete_ConPerfect:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nConPerfectNum);
		break;
	case EMusic_Complete_Combo:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nComboNum);
		break;
	case EMusic_Complete_MissMax:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nFailNum);
		break;
	case EMusic_Complete_Model:
		bCheck = pComp->CheckCondition(pMusicInfo->m_nModeID);
		break;
	default:
		WriteLog(LOGLEVEL_WARNING, "%s Not Implemented Complete : %d .\n" ,QUESTLOG ,(int)pComp->GetCompleteType());
		break;
	}
	return bCheck;
}

bool CEntityQuestNewComponent::_CheckMusicCompleted(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo)
{
	if (pComp->GetCompleteType() != EQuest_Complete_UseMusic)
	{
		return false;
	}
	QuestCompleteUseMusic* pQuest = (QuestCompleteUseMusic*)pComp;
	CQuestMusicNew* pMusic = pQuest->GetMusicComplete((unsigned short)pMusicInfo->m_nMusicID);
	if (pMusic == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "%s Invalid MusicID : %d .\n",QUESTLOG,(int)pMusicInfo->m_nMusicID);
		return false;
	}
	bool bChecked = true;
	std::map<EQuest_Complete_Type,IQuestComplete* >::iterator it = pMusic->m_MusicQuest.begin();
	for ( ; it != pMusic->m_MusicQuest.end() ; it++ )
	{
		bChecked = _CheckCompleteMusic(it->second,pMusicInfo);
		if (!bChecked)
		{
			break;
		}
	}

	return bChecked;
}

unsigned short CEntityQuestNewComponent::_GetDanceGroupLevel()
{
	if (m_pDanceGroup == NULL)
	{
		return 0;
	}
	CDanceGroupInfo* pDanceInfo = m_pDanceGroup->GetDanceGroupInfo();
	if (pDanceInfo == NULL)
	{
		return 0;
	}
	return pDanceInfo->m_DGBaseInfo.m_nLevel;
}

int CEntityQuestNewComponent::_GetDanceGroupContribution()
{
	if (m_pDanceGroup == NULL || m_pAttr == NULL)
	{
		return 0;
	}
	CDanceGroupInfo* pDanceInfo = m_pDanceGroup->GetDanceGroupInfo();
	if (pDanceInfo == NULL)
	{
		return 0;
	}
	return pDanceInfo->GetGroupMember(m_pAttr->m_nRoleID)->m_nContribution;
}

bool CEntityQuestNewComponent::_HasDanceGroup()
{
	if (m_pDanceGroup == NULL || m_pAttr == NULL)
	{
		return false;
	}
	CDanceGroupInfo* pDanceInfo = m_pDanceGroup->GetDanceGroupInfo();
	return pDanceInfo != NULL;
}

bool CEntityQuestNewComponent::_HasLover()
{
	if (m_pAttr == NULL || m_pCouple == NULL)
	{
		return false;
	}
	return !m_pCouple->IsSingle();
}

bool CEntityQuestNewComponent::_HasMarried()
{
	if (m_pAttr == NULL || m_pCouple == NULL)
	{
		return false;
	}
	return m_pCouple->IsMarried();
}

int CEntityQuestNewComponent::_GetUseFunctionTimes(unsigned short nQuestID,int & nParam)
{
	std::map<unsigned short,FunctionParamMap >::iterator itq = m_UseFunctionCount.find(nQuestID);
	if (itq == m_UseFunctionCount.end())
	{
		return 0;
	}
	if (itq->second.size() == 0)
	{
		return 0;
	}
	else
	{
		FunctionParamMap::iterator it = itq->second.begin();
		nParam = it->first;
		return it->second;
	}
}

int CEntityQuestNewComponent::_GetDanceGroupTotalContribution()
{
	if (m_pAttr == NULL || m_pDanceGroup == NULL)
	{
		return 0;
	}
	return m_pDanceGroup->GetDanceGroupTotalContribution();
}

int CEntityQuestNewComponent::_GetDanceGroupTotalHonor()
{
	if (m_pAttr == NULL || m_pDanceGroup == NULL)
	{
		return 0;
	}
	return m_pDanceGroup->GetDanceGroupTotalHonor();
}

bool CEntityQuestNewComponent::_CheckMusicCompleted(IQuestComplete* pComp)
{
	if (pComp->GetCompleteType() != EQuest_Complete_UseMusic)
	{
		WriteLog(LOGLEVEL_ERROR, "%s _CheckMusicCompleted : invalid type = %d .\n" ,QUESTLOG , (int)pComp->GetCompleteType());
		return false;
	}
	QuestCompleteUseMusic* pMusicQuest = (QuestCompleteUseMusic*)pComp;
	std::vector<CQuestMusicNew*>::iterator it = pMusicQuest->m_MusicList.begin();
	for (; it != pMusicQuest->m_MusicList.end() ; it++)
	{
		CQuestMusicNew* pMusic = *it;

		if (pMusic->m_Count > _GetMusicTimes(pComp->GetQuestID(), pMusic->m_MusicID))
		{
			//次数不够
			return false;
		}
	}
	//全部检测通过
	return true;
}

void CEntityQuestNewComponent::_AddMusicTimes(unsigned short nQuestID,unsigned short nMusicID)
{
	std::map<unsigned short,MusicParamMap >::iterator itq = m_MusicCount.find(nQuestID);
	if (itq == m_MusicCount.end())
	{
		//完全没有这个任务的数据
		MusicParamMap musicInfo;
		musicInfo.insert(make_pair(nMusicID,(unsigned short)1));
		m_MusicCount.insert(make_pair(nQuestID,musicInfo));
	}
	else
	{
		MusicParamMap::iterator itFind = itq->second.find(nMusicID);
		if (itFind == itq->second.end())
		{
            itq->second.insert(make_pair(nMusicID, (unsigned short)1));
		}
		else
		{
			itFind->second++;
		}
	}
	_UpdateQuest2DB(nQuestID);
}

bool CEntityQuestNewComponent::_CheckUseCloth(IQuestComplete* pComp)
{
	//检测身上所有的物品，看是不是够
	if (pComp->GetCompleteType() != EQuest_Complete_UseCloth || m_pItem == NULL)
	{
		WriteLog(LOGLEVEL_ERROR, "%s _CheckUseCloth : invalid type = %d != Quest_Complete_UseCloth.\n" ,QUESTLOG , pComp->GetCompleteType());
		return false;
	}

	QuestCompleteUseCloth* pQuest = (QuestCompleteUseCloth*)pComp;
	std::vector<IQuestComplete* >::iterator it = pQuest->m_SubComplete.begin();
	for (;it != pQuest->m_SubComplete.end() ; it++)
	{
		IQuestComplete* pSubQuest = (*it);
		if (pSubQuest->GetCompleteType() != EQuest_Complete_Item)
		{
			WriteLog(LOGLEVEL_ERROR, "%s _CheckUseCloth : invalid type = %d !=  Quest_Complete_Item .\n" ,QUESTLOG , pSubQuest->GetCompleteType());
			return false;
		}
		QuestCompleteItem* pItemQuest = (QuestCompleteItem*)pSubQuest;
		//由于需要在服饰里面搜索，所以不能直接调用CheckCondition了。
        if (!m_pItem->IsEquiped(pItemQuest->m_ItemID))
		{
			return false;
		}
	}

	return true;
}

bool CEntityQuestNewComponent::_CheckUseItem(IQuestComplete* pComp)
{
	if (pComp->GetCompleteType() != EQuest_Complete_UseItem)
	{
		WriteLog(LOGLEVEL_ERROR, "%s _CheckUseItem : invalid type = %d != Quest_Complete_UseItem.\n" ,QUESTLOG , pComp->GetCompleteType());
		return false;
	}
	QuestCompleteUseItem* pQuest = (QuestCompleteUseItem*)pComp;
	std::vector<IQuestComplete* >::iterator it = pQuest->m_SubComplete.begin();
	for (;it != pQuest->m_SubComplete.end() ; it++)
	{
		IQuestComplete* pSub = *it;
		if (pSub->GetCompleteType() != EQuest_Complete_Item)
		{
			WriteLog(LOGLEVEL_ERROR, "%s _CheckUseItem : invalid type = %d !=  Quest_Complete_Item .\n" ,QUESTLOG , pSub->GetCompleteType());
			return false;
		}
		QuestCompleteItem* pItemQuest = (QuestCompleteItem*)pSub;
		std::map<unsigned short,UseItemParamMap >::iterator itq = m_ItemUseCount.find(pComp->GetQuestID());
		if (itq == m_ItemUseCount.end())
		{
			return false;
		}
		UseItemParamMap::iterator itFind = itq->second.find(pItemQuest->m_ItemID);
		if (itFind == itq->second.end())
		{
			return false;
		}
		if (itFind->second < pItemQuest->m_Count)
		{
			return false;
		}
	}
	return true;
}

bool CEntityQuestNewComponent::_CheckObtainItem(IQuestComplete* pComp)
{
	if (pComp->GetCompleteType() != EQuest_Complete_ObtainItem)
	{
		WriteLog(LOGLEVEL_ERROR, "%s _CheckObtainItem : invalid type = %d != Quest_Complete_ObtainItem.\n" ,QUESTLOG , pComp->GetCompleteType());
		return false;
	}
	QuestCompleteObtainItem* pQuest = (QuestCompleteObtainItem*)pComp;
	std::vector<IQuestComplete* >::iterator it = pQuest->m_SubComplete.begin();
	for (; it != pQuest->m_SubComplete.end() ; it++)
	{
		IQuestComplete* pSub = *it;
		if (!_CheckCompleteHasItem(pSub))
		{
			return false;
		}
	}
	return true;
}

bool CEntityQuestNewComponent::_CheckBuyItem(IQuestComplete* pComp)
{
    ENSURE_WITH_LOG_CMD(pComp->GetCompleteType() == EQuest_Complete_BuyItem, return false, 
        "%s _CheckBuyItem : invalid type = %d\n", QUESTLOG, pComp->GetCompleteType());

    QuestRecordMallBuy *buyRec = m_recordManager.GetQuestRecord<QuestRecordMallBuy>(pComp->GetQuestID());
    if (NULL == buyRec)
        return false; // 还没有购买

    QuestCompleteBuyItem *pQuest = (QuestCompleteBuyItem *)pComp;
    std::vector<QuestCompleteItem* >::iterator it = pQuest->m_SubComplete.begin();
    for (; it != pQuest->m_SubComplete.end() ; it++)
    {
        QuestCompleteItem* pSub = *it;
        if (pSub->m_Count > buyRec->GetItemNum(pSub->m_ItemID))
            return false; // 没有完成
    }

    return true;
}


bool CEntityQuestNewComponent::_CheckCompleteHasItem(IQuestComplete* pComp)
{
    if ( pComp->GetCompleteType() != EQuest_Complete_Item )
    {
        WriteLog( LOGLEVEL_ERROR, "%s _CheckCompleteHasItem : invalid type = %d !=  Quest_Complete_Item .\n", 
            QUESTLOG, pComp->GetCompleteType() );
        return false;
    }

    QuestCompleteItem* pQuest = (QuestCompleteItem *)pComp;
    return m_pItem->GetItemCount( pQuest->m_ItemID, false, false ) >= pQuest->m_Count;
}

void CEntityQuestNewComponent::OnMusicEnd(QUESTUPDATEINFO *pQuestUpdateInfo)
{
	//音乐只能完成任务，所以没有任务可以触发
	std::vector<unsigned short> questidList;
	unsigned short nCount = _AddMusicTimes(pQuestUpdateInfo,questidList);
	if (nCount>0)
	{
		std::vector<unsigned short>::iterator it = questidList.begin();
		for ( ; it != questidList.end() ; it++)
		{
			QuestIDSet::iterator itFind = m_CurrentQuest.find(*it);
			if (itFind != m_CurrentQuest.end())
			{
                CQuestDocNew *pDoc = _GetQuestDoc(*itFind);
                ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config.", *itFind);
				if (_CanCompleteQuest(pDoc))
				{
					_CompleteQuest(pDoc);
				}
				else
				{
					_NotifyChangeQuestState(*itFind,eQuestState_Accepted);
				}
			}
		}
	}
}

void CEntityQuestNewComponent::_AcceptQuestList(std::vector<unsigned short>& idList,bool bNotifyClient)
{
	std::vector<unsigned short>::iterator itID = idList.begin();
	for (;itID != idList.end() ; itID++)
	{
        m_UnAcceptQuest.erase(*itID);
		CQuestDocNew* pDoc = _GetQuestDoc(*itID);
		if (pDoc != NULL)
		{
			_AcceptQuest(pDoc,/*false,*/bNotifyClient,true);
			//有些任务可以自动完成
			if (_CanCompleteQuest(pDoc))
			{
				_CompleteQuest(pDoc,bNotifyClient);
			}
		}
	}
}

void CEntityQuestNewComponent::OnAttributeChange()
{
	//遍历未接受的任务，看是否有任务可以触发
	std::vector<unsigned short> canAcceptQuestID;
	QuestIDSet::iterator it = m_UnAcceptQuest.begin();
	for ( ; it != m_UnAcceptQuest.end()  ; it ++)
	{
		CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		if (_CanAcceptQuest(pDoc,0))
		{
			canAcceptQuestID.push_back(*it);
			//_AcceptQuest(pDoc,false);
			//it = m_UnAcceptQuest.erase(it);
		}
	}
	_AcceptQuestList(canAcceptQuestID,true);
	//遍历所有已接任务，看是否有任务可以完成
	_CheckCanCompleteQuest();
}

void CEntityQuestNewComponent::OnItemChange()
{
	std::vector<unsigned short> canAcceptQuestID;
	QuestIDSet::iterator it = m_UnAcceptQuest.begin();
	for ( ; it != m_UnAcceptQuest.end()  ; ++it)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		if (_CanAcceptQuest(pDoc,0))
		{
			canAcceptQuestID.push_back(*it);
			//_AcceptQuest(pDoc,false);
			//it = m_UnAcceptQuest.erase(it);
		}
	}
	_AcceptQuestList(canAcceptQuestID,true);

	it = m_CurrentQuest.begin();
	for (;it != m_CurrentQuest.end() ; ++it)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		if (pDoc->GetComplete(EQuest_Complete_ObtainItem) == NULL)
		{
			continue;
		}
		std::map<unsigned short,QUESTNEWINFO>::iterator itInfo = m_QuestDetailInfo.find(*it);
		if (itInfo != m_QuestDetailInfo.end() && itInfo->second.m_State == eQuestState_Accepted && _CanCompleteQuest(pDoc,0))
		{
			_CompleteQuest(pDoc);
		}
		//else
		//{
		//	//物品减少或不足以完成的时候修改状态为不可完成状态
		//	_NotifyChangeQuestState(it->first,eQuestState_Accepted);
		//}
	}
}

void CEntityQuestNewComponent::OnMagiclampComplete()
{
    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (;it != m_CurrentQuest.end() ; it++)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);

        if (pDoc->GetComplete(EQuest_Complete_Magiclamp) == NULL)
        {
            continue;
        }
        std::map<unsigned short,QUESTNEWINFO>::iterator itInfo = m_QuestDetailInfo.find(*it);
        if (itInfo != m_QuestDetailInfo.end() && itInfo->second.m_State == eQuestState_Accepted && _CanCompleteQuest(pDoc,0))
        {
            _CompleteQuest(pDoc);
        }
    }
}

void CEntityQuestNewComponent::OnMallBuyItem(itemtype_t itemID, int validTime,int nCount)
{ 
    // AddBuyItem times;
    std::vector<unsigned short> questidList;

    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (; it != m_CurrentQuest.end() ; ++it)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
        std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
        for (;itComp != pDoc->m_Complete.end(); itComp++)
        {
            if (itComp->first != EQuest_Complete_BuyItem)
                continue;
            
            // 检测物品和期限是否相符，如果相符的话，则更新记录.
            QuestCompleteBuyItem *pComp = (QuestCompleteBuyItem*)(itComp->second);
            if (!pComp->ValidItem(itemID, validTime))
                continue;

            QuestRecordFactory* pFactory = m_recordManager.EnsureGetFactory(*it);
            QuestRecordMallBuy *mallBuy = pFactory->EnsureGetReocord<QuestRecordMallBuy>();
            mallBuy->AddItemNum(itemID, nCount);

            //受到影响的任务都添加到返回列表中
            questidList.push_back(pDoc->m_BaseInfo.ID);
            _UpdateQuest2DB(pDoc->m_BaseInfo.ID);
            break;
        }
    }

    for (size_t i = 0; i < questidList.size(); ++i)
    {
        QuestIDSet::iterator itFind = m_CurrentQuest.find(questidList[i]);
        if (itFind != m_CurrentQuest.end())
        {
            CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
            ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);

            if (_CanCompleteQuest(pDoc))
            {
                _CompleteQuest(pDoc);
            }
            else
            {
                _NotifyChangeQuestState(*itFind, eQuestState_Accepted);
            }
        }
    }
}

void CEntityQuestNewComponent::OnClotheEffectReplace(unsigned int count)
{
    // AddBuyItem times;
    std::vector<unsigned short> questidList;

    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (; it != m_CurrentQuest.end() ; ++it)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
        std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
        for (;itComp != pDoc->m_Complete.end(); itComp++)
        {
            if (itComp->first != EQuest_Complete_ClotheEffectReplace)
                continue;

            // 检测物品和期限是否相符，如果相符的话，则更新记录.
            QuestRecordFactory* pFactory = m_recordManager.EnsureGetFactory(*it);
            QuestRecordClotheEffectReplace *pRec = pFactory->EnsureGetReocord<QuestRecordClotheEffectReplace>();
            pRec->m_num += count;

            //受到影响的任务都添加到返回列表中
            questidList.push_back(pDoc->m_BaseInfo.ID);
            _UpdateQuest2DB(pDoc->m_BaseInfo.ID);
            break;
        }
    }

    for (size_t i = 0; i < questidList.size(); ++i)
    {
        QuestIDSet::iterator itFind = m_CurrentQuest.find(questidList[i]);
        if (itFind != m_CurrentQuest.end())
        {
            CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
            ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);

            if (_CanCompleteQuest(pDoc))
            {
                _CompleteQuest(pDoc);
            }
            else
            {
                _NotifyChangeQuestState(*itFind, eQuestState_Accepted);
            }
        }
    }
}
void CEntityQuestNewComponent::OnClotheEffectLevelUp(unsigned int count)
{
    // AddBuyItem times;
    std::vector<unsigned short> questidList;

    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (; it != m_CurrentQuest.end() ; ++it)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
        std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
        for (;itComp != pDoc->m_Complete.end(); itComp++)
        {
            if (itComp->first != EQuest_Complete_ClotheEffectLevelUp)
                continue;

            // 检测物品和期限是否相符，如果相符的话，则更新记录.
            QuestRecordFactory* pFactory = m_recordManager.EnsureGetFactory(*it);
            QuestRecordClotheEffectLevelUp *pRec = pFactory->EnsureGetReocord<QuestRecordClotheEffectLevelUp>();
            pRec->m_num += count;

            //受到影响的任务都添加到返回列表中
            questidList.push_back(pDoc->m_BaseInfo.ID);
            _UpdateQuest2DB(pDoc->m_BaseInfo.ID);
            break;
        }
    }

    for (size_t i = 0; i < questidList.size(); ++i)
    {
        QuestIDSet::iterator itFind = m_CurrentQuest.find(questidList[i]);
        if (itFind != m_CurrentQuest.end())
        {
            CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
            ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);
            if (_CanCompleteQuest(pDoc))
            {
                _CompleteQuest(pDoc);
            }
            else
            {
                _NotifyChangeQuestState(*itFind, eQuestState_Accepted);
            }
        }
    }
}

void CEntityQuestNewComponent::OnUseMoneyMagicArray(unsigned int count)
{
    std::vector<unsigned short> questidList;

    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (; it != m_CurrentQuest.end(); ++it)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
        std::map<EQuest_Complete_Type, IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
        for (; itComp != pDoc->m_Complete.end(); itComp++)
        {
            if (itComp->first != EQuest_Complete_MoneyMagicArray)
                continue;

            QuestRecordFactory* pFactory = m_recordManager.EnsureGetFactory(*it);
            QuestRecordMoneyMagicArray *pRec = pFactory->EnsureGetReocord<QuestRecordMoneyMagicArray>();
            pRec->m_num += count;

            //受到影响的任务都添加到返回列表中
            questidList.push_back(pDoc->m_BaseInfo.ID);
            _UpdateQuest2DB(pDoc->m_BaseInfo.ID);
            break;
        }
    }

    for (size_t i = 0; i < questidList.size(); ++i)
    {
        QuestIDSet::iterator itFind = m_CurrentQuest.find(questidList[i]);
        if (itFind != m_CurrentQuest.end())
        {
            CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
            ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);
            if (_CanCompleteQuest(pDoc))
            {
                _CompleteQuest(pDoc);
            }
            else
            {
                _NotifyChangeQuestState(*itFind, eQuestState_Accepted);
            }
        }
    }
}
void CEntityQuestNewComponent::OnUseCoinMagicArray(unsigned int count)
{
    std::vector<unsigned short> questidList;

    QuestIDSet::iterator it = m_CurrentQuest.begin();
    for (; it != m_CurrentQuest.end(); ++it)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
        std::map<EQuest_Complete_Type, IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
        for (; itComp != pDoc->m_Complete.end(); itComp++)
        {
            if (itComp->first != EQuest_Complete_CoinMagicArray)
                continue;

            QuestRecordFactory* pFactory = m_recordManager.EnsureGetFactory(*it);
            QuestRecordCoinMagicArray *pRec = pFactory->EnsureGetReocord<QuestRecordCoinMagicArray>();
            pRec->m_num += count;

            //受到影响的任务都添加到返回列表中
            questidList.push_back(pDoc->m_BaseInfo.ID);
            _UpdateQuest2DB(pDoc->m_BaseInfo.ID);
            break;
        }
    }

    for (size_t i = 0; i < questidList.size(); ++i)
    {
        QuestIDSet::iterator itFind = m_CurrentQuest.find(questidList[i]);
        if (itFind != m_CurrentQuest.end())
        {
            CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
            ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);
            if (_CanCompleteQuest(pDoc))
            {
                _CompleteQuest(pDoc);
            }
            else
            {
                _NotifyChangeQuestState(*itFind, eQuestState_Accepted);
            }
        }
    }
}



void CEntityQuestNewComponent::OnDanceGroupChange()
{
    std::vector<unsigned short> canAcceptQuestID;

    for (QuestIDSet::iterator bIt = m_UnAcceptQuest.begin(), eIt = m_UnAcceptQuest.end(); 
         bIt != eIt; ++bIt)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*bIt);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *bIt);

        if (_CanAcceptQuest(pDoc, 0))
            canAcceptQuestID.push_back(*bIt);
    }

    _AcceptQuestList(canAcceptQuestID, true);
    _CheckCanCompleteQuest();
}

void CEntityQuestNewComponent::AddTimerStartQuest()
{
    time_t now = time(NULL);
    if (NULL == m_pTimer)
        return;

    for (QuestIDSet::iterator iter = m_UnAcceptQuest.begin(); iter != m_UnAcceptQuest.end(); ++iter)
    {
        CQuestDocNew *pDoc = _GetQuestDoc(*iter);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "miss quest[%d] config.", (int)*iter);

        if (pDoc->m_BaseInfo.StartTime != 0 && pDoc->m_BaseInfo.StartTime >= (unsigned int)now)
        {
            m_pTimer->AddTimer(pDoc->m_BaseInfo.StartTime, eComponentEvent_QuestAcceptTimer, *iter);
        }
    }
}


void CEntityQuestNewComponent::OnUseItem(itemtype_t itemID,int nCount)
{
	//检查可接任务
	std::vector<unsigned short> canAcceptQuestID;
	QuestIDSet::iterator it = m_UnAcceptQuest.begin();
	for ( ; it != m_UnAcceptQuest.end()  ; ++it)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		if (_CanAcceptQuest(pDoc,itemID))
		{
			canAcceptQuestID.push_back(*it);
		}
	}

	_AcceptQuestList(canAcceptQuestID,true);
	//增加完成任务计数
	std::vector<unsigned short> questlist;
	int nQuestCount = _AddUseItemTimes(itemID,nCount,questlist);
	//检查可完成任务
	if (nQuestCount > 0)
	{
		std::vector<unsigned short>::iterator it = questlist.begin();
		for (;it != questlist.end() ; it++)
		{
			QuestIDSet::iterator itFind = m_CurrentQuest.find(*it);
			if (itFind != m_CurrentQuest.end())
			{
                CQuestDocNew* pDoc = _GetQuestDoc(*itFind);
                ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *itFind);

				if (_CanCompleteQuest(pDoc))
				{
					_CompleteQuest(pDoc);
				}
                else
                {
                    _NotifyChangeQuestState(pDoc->QuestID(), eQuestState_Accepted);
                }
			}
		}
	}
}

void CEntityQuestNewComponent::OnUseItem(itemtype_t itemID)
{
	OnUseItem(itemID,1);
}

unsigned short  CEntityQuestNewComponent::_AddUseItemTimes(itemtype_t itemID,int nCount,std::vector<unsigned short>& questidList)
{
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	for (; it != m_CurrentQuest.end() ; ++it)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
		for (;itComp != pDoc->m_Complete.end(); itComp++)
		{
			if (itComp->first == EQuest_Complete_UseItem)
			{
				unsigned short nQuestID = _AddUseItemTimes(itComp->second,(unsigned short)nCount,itemID);
				if (nQuestID != 0)
				{
					//受到影响的任务都添加到返回列表中
					questidList.push_back(nQuestID);
				}
			}
		}
	}
	return (unsigned short)questidList.size();
}

unsigned short  CEntityQuestNewComponent::_AddUseItemTimes(IQuestComplete* pComp,unsigned short nCount,itemtype_t itemID)
{
	if (pComp->GetCompleteType() != EQuest_Complete_UseItem)
	{
		return 0;
	}

	unsigned short nQuestID = pComp->GetQuestID();
	QuestCompleteUseItem* pQuest = (QuestCompleteUseItem*)pComp;
	std::vector<IQuestComplete* >::iterator it = pQuest->m_SubComplete.begin();
	for (;it != pQuest->m_SubComplete.end() ; it++)
	{
		QuestCompleteItem* pSubQuest = (QuestCompleteItem*)(*it);
		if (pSubQuest->m_ItemID == itemID)
		{
			//添加计数
			std::map<unsigned short, UseItemParamMap >::iterator itFind = m_ItemUseCount.find(nQuestID);
			if (itFind == m_ItemUseCount.end())
			{
				UseItemParamMap newItemCount;
				newItemCount.insert(make_pair(itemID,nCount));
				m_ItemUseCount.insert(make_pair(nQuestID,newItemCount));
			}
			else
			{
				UseItemParamMap::iterator itItem = itFind->second.find(itemID);
				if (itItem == itFind->second.end())
				{
					itFind->second.insert(make_pair(itemID,nCount));
				}
				else
				{
					itItem->second += nCount;
				}
			}
			_UpdateQuest2DB(pSubQuest->GetQuestID());
			//添加成功后直接返回，不允许在任务条件中有两条一样的
			return pSubQuest->GetQuestID();
		}
	}
	//全部遍历完成都还没有找到，返回0
	return 0;
}

bool CEntityQuestNewComponent::_ExchangeQuestItem(CQuestDocNew* pDoc)
{
	//如果任务是兑换任务，那么从身上的物品中扣除
	if (pDoc->IsExchangeQuest() && m_pItem != NULL)
	{
		if (!_CanCompleteQuest(pDoc,0))
		{
			//这里会检测身上物品
			return false;
		}
		std::vector<CQuestItem> questItems;
		pDoc->GetExchangeItem(questItems);
		std::vector<CQuestItem>::iterator it = questItems.begin();
		for (;it != questItems.end() ; it++)
		{
            if (m_pItem->GetItemCount(it->m_nItemID, false, false) < (unsigned int)it->m_nItemCount)
            {
                //数量不够，理论上不会执行到这里
                WriteLog(LOGLEVEL_WARNING, "%s _CompleteQuest error,player %s has not enough item id = %d , count = %d .\n", QUESTLOG, m_pAttr->m_szRoleName, it->m_nItemID, it->m_nItemCount);
                return false;
            }
			
			bool bRet = m_pItem->RemoveItemByType(it->m_nItemID,it->m_nItemCount,EItemAction_Del_ExchangeItem);
			if (!bRet)
			{
				WriteLog(LOGLEVEL_ERROR, "%s _CompleteQuest error,player %s remove item error , item id = %d , count = %d .\n ",QUESTLOG,m_pAttr->m_szRoleName,it->m_nItemID,it->m_nItemCount);
				return false;
			}
		}
	}
	return true;
}

void CEntityQuestNewComponent::_CompleteQuest(CQuestDocNew* pDoc,bool bNotifyClient)
{
	//更新状态
	_UpdateQuestState(pDoc->m_BaseInfo.ID,eQuestState_Completed);

	if (bNotifyClient)
	{
		//可完成任务，通知客户端
		_NotifyCompleteQuest(pDoc);
	}
}

void CEntityQuestNewComponent::_NotifyCompleteQuest(CQuestDocNew* pDoc)
{
	_PushCanCompleteQuest();

	_NotifyChangeQuestState(pDoc->m_BaseInfo.ID,eQuestState_Completed);
}

void CEntityQuestNewComponent::_NotifyChangeQuestState(unsigned short nQuestID,eQuestState state)
{
	if (m_bSendQuestDataToClient)
	{
		GameMsg_S2C_QuestStateChange msg;
		msg.m_nQuestID = nQuestID;
		msg.m_nState = state;

		CQuestListEntry listEntry;
		_MakeQuestListEntry(listEntry,nQuestID);
		msg.SetListEntry(listEntry);

		_SendPlayerMsg(&msg);	
	}
}

void CEntityQuestNewComponent::_NotifyAcceptQuest(CQuestDocNew* pDoc)
{
    if (m_bSendQuestDataToClient)
    {
        CQuestListEntry questEntry;
        _MakeQuestListEntry(questEntry,pDoc->m_BaseInfo.ID);
        GameMsg_S2C_AcceptQuestResult msg;
        msg.SetListEntry(questEntry);
        _SendPlayerMsg(&msg);
    }
}

unsigned short CEntityQuestNewComponent::_AddMusicTimes(QUESTUPDATEINFO* pMusicInfo,std::vector<unsigned short>& questidList)
{
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	for (;it != m_CurrentQuest.end() ; it++)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		std::map<EQuest_Complete_Type,IQuestComplete*>::iterator itComp = pDoc->m_Complete.begin();
		for (;itComp != pDoc->m_Complete.end();itComp++)
		{
			unsigned short nQuestID = _AddMusicTimes(itComp->second,pMusicInfo);
			if (nQuestID != 0)
			{
				questidList.push_back(nQuestID);
				//音乐条件都在一个里面，所以如果这里满足了，就不用继续检测其他条件了
				break;
			}
		}
	}
	return (unsigned short)questidList.size();
}

unsigned short CEntityQuestNewComponent::_AddMusicTimes(IQuestComplete* pComp,QUESTUPDATEINFO* pMusicInfo)
{
	if (pComp->GetCompleteType() == EQuest_Complete_UseMusic)
	{
		QuestCompleteUseMusic* pQuest = (QuestCompleteUseMusic*)pComp;
		std::vector<CQuestMusicNew*>::iterator it = pQuest->m_MusicList.begin();
		for (;it != pQuest->m_MusicList.end() ; it++)
		{
			CQuestMusicNew* pMusic = *it;

			if (pMusic->m_MusicID == 0 || pMusic->m_MusicID == pMusicInfo->m_nMusicID)
			{
				bool bChecked = true;
				std::map<EQuest_Complete_Type,IQuestComplete* >::iterator itQuest = pMusic->m_MusicQuest.begin();
				for ( ; itQuest != pMusic->m_MusicQuest.end() ; itQuest++ )
				{
					if (!_CheckCompleteMusic(itQuest->second,pMusicInfo))
					{
						bChecked = false;
						break;
					}
				}
				if (bChecked)
				{
					//条件完成，加入条件计数
					_AddMusicTimes(pQuest->GetQuestID(),(unsigned short)pMusic->m_MusicID);
					return pQuest->GetQuestID();
				}
			}
		}
	}
	else if (pComp->GetCompleteType() == EQuest_Complete_MatchMusicEnd)
	{
		QuestCompleteMatchMusicEnd* pQuest = (QuestCompleteMatchMusicEnd*)pComp;
		if (pMusicInfo->m_bIsMatch)
		{
			_AddMusicTimes(pQuest->GetQuestID(),0);
			return pQuest->GetQuestID();
		}
	}
	return 0;
}

CQuestDocNew* CEntityQuestNewComponent::_GetQuestDoc(unsigned short nQuestID)
{
	CQuestManNew &questMan = ConfigManager::Instance().GetQuestNewManager();
	return questMan.GetQuestByID(nQuestID);
}

void CEntityQuestNewComponent::_AddAcceptedQuest(QUESTNEWINFO* pQuestInfo)
{
	unsigned short nQuestID = pQuestInfo->m_QuestID;
	CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
	ENSURE_WITH_LOG_CMD (pDoc != NULL, return, "%s Invalid questid [%d] ." ,QUESTLOG ,nQuestID);
	ENSURE_WITH_LOG_CMD(m_CurrentQuest.find(nQuestID) == m_CurrentQuest.end(), return, "%s Quest repeated! id[%d] .\n",QUESTLOG,nQuestID);

	//添加需要完成次数的条件
	if (pQuestInfo->m_Musics != "")
	{
		_ParseAndInsertMusic(nQuestID,pQuestInfo->m_Musics);
	}
    if (pQuestInfo->m_UseItems != "")
    {
        _ParseAndInsertUseItem(nQuestID,pQuestInfo->m_UseItems);
    }
	if (pQuestInfo->m_Functions != "")
	{
		_ParseAndInsertFunctions(nQuestID,pQuestInfo->m_Functions);
	}
    if (!pQuestInfo->m_strJsonQuestParam.empty())
    { // 参数读取进来了。
        m_recordManager.Parse(nQuestID, pQuestInfo->m_strJsonQuestParam);
    }
	//添加任务
	m_CurrentQuest.insert(nQuestID);

	//if ( pDoc->m_BaseInfo.Type == eQuestTypeNew_Guide)
	//{
	//	m_GuideQuestID = nQuestID;
	//}

}

void CEntityQuestNewComponent::_ParseAndInsertFunctions(unsigned short nQuestID,std::string& funcInfo)
{
	std::map<unsigned short,FunctionParamMap >::iterator it = m_UseFunctionCount.find(nQuestID);
	if (it != m_UseFunctionCount.end())
	{
		WriteLog(LOGLEVEL_WARNING, "%s _ParseAndInsertFunctions Quest repeated! id = %d .\n",QUESTLOG,nQuestID);
		return;
	}
	std::vector<CompleteTargetInfo> parseResult;
	_ParseItem(funcInfo,parseResult);
	for (std::vector<CompleteTargetInfo>::iterator itc = parseResult.begin(); itc != parseResult.end() ; itc++)
	{
		if (it == m_UseFunctionCount.end())
		{
			FunctionParamMap funcCount;
			funcCount.insert(make_pair((int)itc->m_ID,itc->m_Count));
			m_UseFunctionCount.insert(make_pair(nQuestID,funcCount));
			it = m_UseFunctionCount.find(nQuestID);
		}
		else
		{
			it->second.insert(make_pair(itc->m_ID,itc->m_Count));
		}
	}
}

void CEntityQuestNewComponent::_ParseAndInsertMusic(unsigned short nQuestID,std::string& musicInfo)
{
	std::map<unsigned short, MusicParamMap >::iterator it = m_MusicCount.find(nQuestID);
	if (it != m_MusicCount.end())
	{
		WriteLog(LOGLEVEL_WARNING, "%s _ParseAndInsertMusic Quest repeated! id = %d .\n",QUESTLOG,nQuestID);
		return;
	}
	std::vector<CompleteTargetInfo> parseResult;
	_ParseItem(musicInfo,parseResult);
	for (std::vector<CompleteTargetInfo>::iterator itc = parseResult.begin(); itc != parseResult.end() ; itc++)
	{
		if (it == m_MusicCount.end())
		{
			MusicParamMap funcCount;
			funcCount.insert(make_pair((unsigned short)itc->m_ID,itc->m_Count));
			m_MusicCount.insert(make_pair(nQuestID,funcCount));
			it = m_MusicCount.find(nQuestID);
		}
		else
		{
            it->second.insert(make_pair((unsigned short)itc->m_ID, itc->m_Count));
		}
	}
}

void CEntityQuestNewComponent::_ParseAndInsertUseItem(unsigned short nQuestID,std::string& itemInfo)
{
	std::map<unsigned short,UseItemParamMap >::iterator it = m_ItemUseCount.find(nQuestID);
	if (it != m_ItemUseCount.end())
	{
		WriteLog(LOGLEVEL_WARNING, "%s _ParseAndInsertUseItem Quest repeated! id = %d .\n",QUESTLOG,nQuestID);
		return;
	}

	std::vector<CompleteTargetInfo> parseResult;
	_ParseItem(itemInfo,parseResult);
	for (std::vector<CompleteTargetInfo>::iterator itc = parseResult.begin(); itc != parseResult.end() ; itc++)
	{
		if (it == m_ItemUseCount.end())
		{
			UseItemParamMap funcCount;
			funcCount.insert(make_pair(itc->m_ID,itc->m_Count));
			m_ItemUseCount.insert(make_pair(nQuestID,funcCount));
			it = m_ItemUseCount.find(nQuestID);
		}
		else
		{
			it->second.insert(make_pair(itc->m_ID,itc->m_Count));
		}
	}
}

bool CEntityQuestNewComponent::_ParseItem(std::string& strInput,unsigned int& id,unsigned short& count)
{
	unsigned int nPos = (unsigned int)strInput.find(',');
	if (nPos == (unsigned int )std::string::npos)
	{
		return false;
	}
	id = atoi(strInput.substr(0,nPos).c_str());
	count = (unsigned short)atoi(strInput.substr(nPos + 1,strInput.size()-nPos-1).c_str());
	return true;
}

int CEntityQuestNewComponent::_ParseItem(std::string& strInput,std::vector<CompleteTargetInfo>& m_Result)
{
	std::string strBuff = strInput;
	std::string::size_type nPos = 0;

	for (std::string::size_type i = 0 ; i < strBuff.size() ; i++)
	{
		nPos =  strBuff.find('|',i);
		if (nPos != std::string::npos)
		{
			std::string s = strBuff.substr(i,nPos - i);
			unsigned int id = 0;
			unsigned short nCount = 0;
			bool bRes = _ParseItem(s,id,nCount);
			if(bRes)
			{
				CompleteTargetInfo cInfo;
				cInfo.m_ID = id;
				cInfo.m_Count = nCount;
				m_Result.push_back(cInfo);
			}
			i = nPos ;
		}
	}
	return m_Result.size();
}


void CEntityQuestNewComponent::_AddDestroyQuestTimer(CQuestDocNew *pDoc)
{
    ENSURE_WITH_LOG_CMD (NULL != pDoc, return, "quest[%u] should not null on add destroy timer");
	ENSURE_WITH_LOG_CMD (m_pTimer != NULL, return, "destroy timer null on add quest[%u] destroy", pDoc->QuestID());

    if (pDoc->m_BaseInfo.EndTime == 0 && pDoc->m_BaseInfo.LastTime == 0)
        return ;

    if (pDoc->m_BaseInfo.EndTime != 0)
    {
        m_pTimer->AddTimer(pDoc->m_BaseInfo.EndTime,eComponentEvent_QuestExpiredTimer,pDoc->QuestID());
    }
    else if (pDoc->m_BaseInfo.LastTime != 0)
    {
        std::map<unsigned short,QUESTNEWINFO>::iterator itd = m_QuestDetailInfo.find(pDoc->QuestID());
        if (itd == m_QuestDetailInfo.end())
            return;

        m_pTimer->AddTimer(itd->second.m_CreateTime + pDoc->m_BaseInfo.LastTime, eComponentEvent_QuestExpiredTimer, pDoc->QuestID());
    }
    
    return ;
}

void CEntityQuestNewComponent::_CheckCanCompleteQuest()
{
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	for (;it != m_CurrentQuest.end() ; it++)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config...", *it);
		std::map<unsigned short,QUESTNEWINFO>::iterator itInfo = m_QuestDetailInfo.find(*it);
		if (itInfo != m_QuestDetailInfo.end() && itInfo->second.m_State == eQuestState_Accepted && _CanCompleteQuest(pDoc,0))
		{
			_CompleteQuest(pDoc);
		}
	}
}

void CEntityQuestNewComponent::_CheckCanAcceptQuest(int nParam,bool bNotifyClient)
{
	CQuestManNew &questMan = ConfigManager::Instance().GetQuestNewManager();
	std::vector<unsigned short> canAcceptQuestList;
	std::map<unsigned short,CQuestDocNew*>::const_iterator it = questMan.GetAllQuestDoc()->begin();
	for (;it != questMan.GetAllQuestDoc()->end() ; ++it)
	{
		if (_IsQuestUnAccept(it->second))
		{
			//添加到未接任务列表中
			m_UnAcceptQuest.insert(it->second->m_BaseInfo.ID);
			if (_CanAcceptQuest(it->second,nParam))
			{
				//_AcceptQuest(it->second,false);
				canAcceptQuestList.push_back(it->first);
			}
		}
	}
	_AcceptQuestList(canAcceptQuestList,bNotifyClient);
}

bool CEntityQuestNewComponent::_CanAcceptQuestRepeat(CQuestDocNew* pDoc)
{
	//如果到达销毁等级，那么任务不可接，并且加入到历史任务中
	if (pDoc->CanDestroyByLevel(m_pAttr->m_nLevel))
	{
		return false;
	}

    // 对于不可重复接任务，如果已经完成了，就不能再接了。如果没完成就可以再接
    if (!pDoc->IsReAcceptType())
    { 
        return m_HistroyQuest.find(pDoc->m_BaseInfo.ID) == m_HistroyQuest.end();
    }
	
	//日常任务判断星期几
    if (!pDoc->CheckTimeType(GetWeekDay()))
//        if (!_CheckTimeType(pDoc->m_BaseInfo.TimeType))
	{
		return false;
	}

	//如果当前有这个任务，并且完成次数大于或等于可完成最大次数，那么不可接
	//-1的任务永远可以接（不限制完成次数）
//    if (pDoc->m_BaseInfo.CompleteTimes != 0xFFFFFFFF && m_CurrentQuest.find(pDoc->m_BaseInfo.ID) != m_CurrentQuest.end())
//    if (pDoc->m_BaseInfo.CompleteTimes != 0xFFFFFFFF)
	{
		std::map<unsigned short,QUESTNEWINFO>::iterator it = m_QuestDetailInfo.find(pDoc->m_BaseInfo.ID);
		if (it != m_QuestDetailInfo.end())
		{
			return _CheckTimesLimit(&(it->second), pDoc);
		}
	}
	return true;
}

bool CEntityQuestNewComponent::_IsQuestUnAccept(CQuestDocNew* pDoc)
{
	return m_CurrentQuest.find(pDoc->m_BaseInfo.ID) == m_CurrentQuest.end() && m_HistroyQuest.find(pDoc->m_BaseInfo.ID) == m_HistroyQuest.end();
}

void CEntityQuestNewComponent::_InsertQuest(CQuestDocNew* pDoc)
{
    ENSURE_WITH_LOG_CMD(NULL != pDoc, return, "QuestDocNew Should not null");
	std::map<unsigned short,QUESTNEWINFO>::iterator it = EnsureGetDetailerInfo(pDoc);
	QUESTNEWINFO* pInfo = new QUESTNEWINFO(it->second);
	CDelHelper delHelper(pInfo);
	AddQuery(QUERY_Add_Quest,0,pInfo,0,NULL,&delHelper);
}

void CEntityQuestNewComponent::_InsertNewQuestData(CQuestDocNew* pDoc)
{
	QUESTNEWINFO info;
	info.m_CreateTime = (unsigned int)time(NULL);
	info.m_pQuestDoc = pDoc;
	info.m_QuestID = pDoc->m_BaseInfo.ID;
	info.m_RoleID = m_pAttr->m_nRoleID;
	info.m_State = eQuestState_Accepted;
	info.m_Times = 0;
	info.m_UpdateTime = (unsigned int)time(NULL);
	m_QuestDetailInfo.insert(make_pair(info.m_QuestID,info));
}

// 保证内存数据跟数据库数据的对应
void CEntityQuestNewComponent::_UpdateQuest2DB(QUESTNEWINFO &questInfo)
{
    // 每次更新，时间戳总是要加上的
    questInfo.m_UpdateTime = (unsigned int)time(NULL);

    questInfo.m_Musics = EncodeQuestParam(m_MusicCount, questInfo.m_QuestID);
    questInfo.m_UseItems = EncodeQuestParam(m_ItemUseCount, questInfo.m_QuestID);
    questInfo.m_Functions = EncodeQuestParam(m_UseFunctionCount, questInfo.m_QuestID);
    QuestRecordFactory *pFact = m_recordManager.GetFactory(questInfo.m_QuestID);
    questInfo.m_strJsonQuestParam = (NULL == pFact ? "" : pFact->Serial());

    m_DirtyQuest[questInfo.m_QuestID] = questInfo;

    WriteLog(LOGLEVEL_DEBUG, "role[%u] quest[%d] update, musics[%s], useItems[%s], function[%s], json[%s], time[%u]", 
        m_pAttr->GetRoleID(), (int)questInfo.m_QuestID, 
        questInfo.m_Musics.c_str(), questInfo.m_UseItems.c_str(), questInfo.m_Functions.c_str(), questInfo.m_strJsonQuestParam.c_str(), 
        (unsigned int)time(NULL));

    return;
}

void CEntityQuestNewComponent::_UpdateQuest2DB(unsigned short nQuestID)
{
    CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
    ENSURE_WITH_LOG_CMD (pDoc != NULL, return, "quest[%u] missed config.", nQuestID);

    std::map<unsigned short,QUESTNEWINFO>::iterator it = EnsureGetDetailerInfo(pDoc);
    _UpdateQuest2DB(it->second);    
}


void CEntityQuestNewComponent::_UpdateQuestState(unsigned short nQuestID,unsigned short newState)
{
	CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
    ENSURE_WITH_LOG_CMD (pDoc != NULL, return, "%s _UpdateQuestState Invalid id = %d .\n",QUESTLOG,(int)nQuestID);

    std::map<unsigned short,QUESTNEWINFO>::iterator it = EnsureGetDetailerInfo(pDoc);
	it->second.m_State = newState;
	_UpdateQuest2DB(it->second);
}

void CEntityQuestNewComponent::_UpdateQuestState(unsigned short nQuestID,unsigned short newState/*,bool addcompletetimes,bool adddaytimes*/,unsigned short destroytype)
{
    CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
    ENSURE_WITH_LOG_CMD (pDoc != NULL, return, "%s _UpdateQuestState Invalid id = %d .\n",QUESTLOG,(int)nQuestID);

    std::map<unsigned short,QUESTNEWINFO>::iterator it = EnsureGetDetailerInfo(pDoc);
    it->second.m_State = newState;
    // 	if(addcompletetimes) it->second.m_Times ++;
    // 	if(adddaytimes) it->second.m_DayTimes ++;
    it->second.m_DestroyType = destroytype;

    _UpdateQuest2DB(it->second);
}

void CEntityQuestNewComponent::_AddHistoryQuest(unsigned short questID)
{
	CQuestDocNew* pDoc = _GetQuestDoc(questID);
    ENSURE_WITH_LOG_CMD(NULL != pDoc, return, "%s _AddHistoryQuest invalid quest id = %d .\n",QUESTLOG,questID);
    m_HistroyQuest.insert(pDoc->m_BaseInfo.ID);
    return ;
}

void CEntityQuestNewComponent::_MakeQuestListEntry(CQuestListEntry& listEntry,unsigned short nQuestID)
{
	listEntry.m_nQuestID = nQuestID;
	_AddQuestMusicTarget(listEntry.m_listTargetInfo, m_MusicCount, nQuestID, TARGET_MUSIC);
	_AddQuestFunctionTarget(listEntry.m_listTargetInfo, m_UseFunctionCount, nQuestID, TARGET_USEFUNCTION);
	_AddQuestUseItemTarget(listEntry.m_listTargetInfo, m_ItemUseCount, nQuestID, TARGET_ITEMUSE);

    std::map<unsigned short,QUESTNEWINFO>::iterator itQuestInfo = m_QuestDetailInfo.find(nQuestID);
	if (itQuestInfo != m_QuestDetailInfo.end())
	{
		listEntry.m_nState = itQuestInfo->second.m_State;
        listEntry.m_strJsonProgress = itQuestInfo->second.m_strJsonQuestParam;
	}
	else
	{
		listEntry.m_nState = eQuestState_Accepted;
	}
}

void CEntityQuestNewComponent::_SendQuestList()
{
	if (m_bSendQuestDataToClient)
	{
		GameMsg_S2C_GetQuestListResult rMsg;
		QuestIDSet::iterator it = m_CurrentQuest.begin();
		for ( ; it != m_CurrentQuest.end() ; it++)
		{
			CQuestListEntry listEntry;
			_MakeQuestListEntry(listEntry, *it);
			rMsg.m_listQuestInfo.push_back(listEntry);
		}
        
		_SendPlayerMsg(&rMsg);
	}
}

void CEntityQuestNewComponent::OnGetQuestList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	_SendQuestList();
}

void CEntityQuestNewComponent::OnGetQuestReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_GetQuestReward *pRewardMsg = (GameMsg_C2S_GetQuestReward *)&msg;
	CQuestDocNew* pDoc = _GetQuestDoc(pRewardMsg->m_nQuestID);
	if (pDoc == NULL)
	{
		return;
	}
	std::map<unsigned short,QUESTNEWINFO>::iterator itFind = m_QuestDetailInfo.find(pDoc->m_BaseInfo.ID);
	if (itFind == m_QuestDetailInfo.end())
	{
		WriteLog(LOGLEVEL_WARNING, "%s OnGetQuestReward can't find quest data questid = %d .\n" ,QUESTLOG ,(int)pDoc->m_BaseInfo.ID);
		return;
	}
	if (itFind->second.m_State == eQuestState_Accepted && _CanCompleteQuest(pDoc))
	{
		_CompleteQuest(pDoc,false);
	}
	if (itFind->second.m_State != eQuestState_Completed)
	{
		return;
	}
	if (!_ExchangeQuestItem(pDoc))
	{
		//如果是兑换任务，并且扣除物品不成功，那么不能完成任务
		return;
	}
	//给玩家奖励
	_SendPlayerRewardAll(pDoc);
	
	//增加完成次数
	_AddCompleteTimes(&itFind->second);

    // 通知观察者
    if (pDoc->m_BaseInfo.Type == eQuestTypeNew_Daily)
    {
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("quest_daily"));
        NotifyAll(param);
    }

	if (AcceptAble(pDoc))
	{
        // 只要还能接受，都清理掉
        RetsetRepeatQuest(pDoc->m_BaseInfo.ID, false, false); 
        if (_CanAcceptQuest(pDoc))
        {
            _AcceptQuest(pDoc,/*false,*/false,true);
            if (_CanCompleteQuest(pDoc))
            {
                //重新接取后又可以完成
                _CompleteQuest(pDoc,false);
            }
        }
        else        
        {
            m_CurrentQuest.erase(pDoc->QuestID());
            m_UnAcceptQuest.insert(pDoc->QuestID());
            _UpdateQuestState(pDoc->QuestID(), eQuestState_None); // 可以再次接取
        }
	}
	else
	{
		//修改任务状态
		_UpdateQuestState(pDoc->m_BaseInfo.ID,eQuestState_HasReward/*,false,false*/,eQuestDestroy_Completed);
		//从当前任务中删除，添加到历史任务中
        m_CurrentQuest.erase(pDoc->m_BaseInfo.ID);
        _AddHistoryQuest(pDoc->m_BaseInfo.ID);
	}

	_OnQuestComplete(pDoc->m_BaseInfo.ID);
	_LogRoleQuest(m_pAttr->GetRoleID(),pDoc->m_BaseInfo.ID);

    WriteDirtyQuest(itFind->second);
    m_DirtyQuest.erase(pDoc->m_BaseInfo.ID); //如果已经完成立即回写数据库，此时就需要从dirty表中移除

	GameMsg_S2C_GetQuestRewardResult rewardResultMsg = GameMsg_S2C_GetQuestRewardResult(pRewardMsg->m_nQuestID , eGetQuestReward_Success);
	_SendPlayerMsg(&rewardResultMsg);
}

void CEntityQuestNewComponent::OnQuestRead(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	
}

void CEntityQuestNewComponent::OnAcceptQuest(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	
}

void CEntityQuestNewComponent::_AddQuestMusicTarget(list<CQuestTargetSend>& target,std::map<unsigned short,MusicParamMap>& source,unsigned short questid,eTargetType type)
{

	std::map<unsigned short,MusicParamMap>::iterator it = source.find(questid);
	if (it != source.end())
	{
		MusicParamMap::iterator itEntity = it->second.begin();
		for (;itEntity != it->second.end() ; itEntity ++ )
		{
			CQuestTargetSend targetSend;
			targetSend.m_nIndex = itEntity->first;
			targetSend.m_nCompletedCount = itEntity->second;
			targetSend.m_nType = type;
			targetSend.m_nRequiredCount = _GetTargetNeedTimes(type,questid,itEntity->first);

			target.push_back(targetSend);
		}
	}
}

void CEntityQuestNewComponent::_AddQuestFunctionTarget(list<CQuestTargetSend>& target,std::map<unsigned short,FunctionParamMap>& source,unsigned short questid,eTargetType type)
{

    std::map<unsigned short,FunctionParamMap>::iterator it = source.find(questid);
    if (it != source.end())
    {
        FunctionParamMap::iterator itEntity = it->second.begin();
        for (;itEntity != it->second.end() ; itEntity ++ )
        {
            CQuestTargetSend targetSend;
            targetSend.m_nIndex = itEntity->first;
            targetSend.m_nCompletedCount = itEntity->second;
            targetSend.m_nType = type;
            targetSend.m_nRequiredCount = _GetTargetNeedTimes(type,questid,itEntity->first);

            target.push_back(targetSend);
        }
    }
}

void CEntityQuestNewComponent::_AddQuestUseItemTarget(list<CQuestTargetSend>& target,std::map<unsigned short,UseItemParamMap >& source,unsigned short questid,eTargetType type)
{

    std::map<unsigned short,UseItemParamMap>::iterator it = source.find(questid);
    if (it != source.end())
    {
        UseItemParamMap::iterator itEntity = it->second.begin();
        for (;itEntity != it->second.end() ; itEntity ++ )
        {
            CQuestTargetSend targetSend;
            targetSend.m_nIndex = itEntity->first;
            targetSend.m_nCompletedCount = itEntity->second;
            targetSend.m_nType = type;
            targetSend.m_nRequiredCount = _GetTargetNeedTimes(type,questid,itEntity->first);

            target.push_back(targetSend);
        }
    }
}

unsigned short CEntityQuestNewComponent::_GetTargetNeedTimes(eTargetType type,unsigned short nQuestID,unsigned int nTargetID)
{
	unsigned short nCount = 0;
	switch(type)
	{
	case TARGET_MUSIC:
		{
			CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
            nCount = pDoc->GetMusicCount((unsigned short)nTargetID);
		}
		break;
	case TARGET_ITEMUSE:
		{
			CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
            nCount = pDoc->GetUseItemCount(nTargetID);
		}
		break;
	case TARGET_USEFUNCTION:
		{
			CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
            nCount = pDoc->GetUseFunctionCount((unsigned short)nTargetID);
		}
		break;
	default:
		break;
	}
	return nCount;
}

void CEntityQuestNewComponent::_SendPlayerMsg(GameMsg_Base *pMsg)
{
	if (m_pNet != NULL && pMsg != NULL){
		m_pNet->SendPlayerMsg(pMsg);
	}
}


void CEntityQuestNewComponent::_SendPlayerRewardAll(CQuestDocNew* pDoc)
{
	unsigned int nExp;
	unsigned int nMoney;
	unsigned int nContribution;
	unsigned int nHonor;
	unsigned int nVipValue;
	unsigned int nIntimacy;
	std::list< CItem > items;
	int nBindMCoin;
	bool bRet = pDoc->GetReward(m_pAttr->m_nSex, nExp, nMoney, nHonor, nContribution, nVipValue, nIntimacy, items, nBindMCoin);
	if (bRet)
	{
        _SendPlayerReward(pDoc, nExp, nMoney, nHonor, nContribution, nVipValue, nIntimacy, items, nBindMCoin);
        m_UnRewardVipQuest = pDoc->m_BaseInfo.ID;
	}
    
    if (m_pRoleVIP != NULL)
    {
        _SendPlayerVipReward(pDoc);
    }
}

void CEntityQuestNewComponent::_SendPlayerVipReward(CQuestDocNew* pDoc)
{
    unsigned int nExp;
    unsigned int nMoney;
    unsigned int nContribution;
    unsigned int nHonor;
    unsigned int nVipValue;
    unsigned int nIntimacy;
    std::list< CItem > items;
    int nBindMCoin;

    if (m_pRoleVIP != NULL && m_pRoleVIP->IsVIP())
    {
        bool bRet = pDoc->GetVipReward(m_pAttr->m_nSex, nExp, nMoney, nHonor, nContribution, nVipValue, nIntimacy, items, nBindMCoin);
        if (bRet)
        {
            _SendPlayerReward(pDoc, nExp, nMoney, nHonor, nContribution, nVipValue, nIntimacy, items, nBindMCoin);
        }
        m_UnRewardVipQuest = 0;
    }
}

void CEntityQuestNewComponent::_SendPlayerReward(CQuestDocNew* pDoc, unsigned int nExp, unsigned int nMoney, unsigned int nHonor,
    unsigned int nContribution, unsigned int nVipValue, unsigned int nIntimacy, std::list<CItem>& items , int nBindMCoin)
{
    if (m_pAttr != NULL)
    {
        if(nExp != 0)
        {
            m_pAttr->AddAbsExp(nExp,CRoleExpLog::EAddExpCause_Quest,0);
        }
        if(nMoney != 0)
        {
            m_pAttr->ChangeMoney((int)nMoney,EChangeMoneyCause_Add_Quest,0);
        }
        if (nBindMCoin != 0)
        {
            m_pAttr->ChangeBindBill(nBindMCoin,EChangeBindBillCause_Add_Quest);
        }
    }
    if (m_pDanceGroup != NULL && m_pAttr != NULL)
    {
        if(nHonor != 0)
        {
            m_pDanceGroup->ChangeDanceGroupHonor(nHonor,EChangeDanceGroupHonorCause_Quest,0);
        }
        if (nContribution != 0)
        {
            m_pDanceGroup->ChangeDanceGroupContribution(nContribution);
        }
    }
    if (m_pRoleVIP != NULL && m_pRoleVIP->IsVIP())
    {
        if(nVipValue != 0)m_pRoleVIP->ChangeVIPInfo(0, nVipValue, ERoleVipExpCause_AddByQuest);
    }
    if (m_pCouple != NULL && !m_pCouple->IsSingle())
    {
        if(nIntimacy != 0)m_pCouple->ChangeIntimacy((int)nIntimacy,EChangeIntimacyCause_Quest,0);
    }
    if (items.size() > 0)
    {
        CItemProcess::AddOrMailItems(*(CRoleEntity*)Entity(),items,EItemAction_Add_Quest,0,true,EMailType_Quest,pDoc->m_BaseInfo.Name,pDoc->m_BaseInfo.Description);
    }
}

void CEntityQuestNewComponent::_OnQuestComplete(unsigned short nQuestID)
{
	//一个任务被完成，检查这个任务可以触发的任务
	_CheckCanAcceptQuest(0,true); // param 不能乱用
}

// 仅需要进行内存操作。速度很快
void CEntityQuestNewComponent::_CleanOverdueRepeatQuest()
{
    time_t now = time(NULL);

    std::map<unsigned short, QUESTNEWINFO>::iterator iter = m_QuestDetailInfo.begin(); 
    for (; iter != m_QuestDetailInfo.end(); ++iter)
    {
        QUESTNEWINFO &qinfo = iter->second;

        CQuestDocNew* pDoc = _GetQuestDoc(qinfo.m_QuestID);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config.", qinfo.m_QuestID);
        if (pDoc->QuestType() != eQuestTypeNew_Daily)
        { // 只有每日任务才存在时间戳过期的问题
            continue;
        }

        if (DiffDays(now, qinfo.m_UpdateTime) == 0) 
            continue; // 有效时间

        qinfo.m_DayTimes = 0; // 

        m_MusicCount.erase(pDoc->QuestID());
        m_ItemUseCount.erase(pDoc->QuestID());
        m_UseFunctionCount.erase(pDoc->QuestID());
        m_recordManager.ReleaseQuestRecord(pDoc->QuestID());
    } // 注意，不用更新数据库，时间检测的操作，随时检测都是可行的。减少数据库的压力
}


void CEntityQuestNewComponent::RetsetRepeatQuest(unsigned short nQuestID,bool cleanDayTimes, bool toDB)
{
	std::map<unsigned short,QUESTNEWINFO>::iterator it = m_QuestDetailInfo.find(nQuestID);
	if (it != m_QuestDetailInfo.end())
	{
		if(cleanDayTimes) it->second.m_DayTimes = 0;
		it->second.m_DestroyType = eQuestDestroy_None;
		it->second.m_State = eQuestState_None;
	}

    m_MusicCount.erase(nQuestID);
    m_ItemUseCount.erase(nQuestID);
    m_UseFunctionCount.erase(nQuestID);
    m_recordManager.ReleaseQuestRecord(nQuestID);

    if (toDB && it != m_QuestDetailInfo.end())
    {
        _UpdateQuest2DB(it->second);
    }
}

void CEntityQuestNewComponent::_AddCompleteTimes(QUESTNEWINFO* pInfo)
{
	pInfo->m_DayTimes++;
	pInfo->m_Times++;
}
// 发送任务二进制配置文件
void CEntityQuestNewComponent::SendQuestData()
{
	CQuestManNew &  mgr = ConfigManager::Instance().GetQuestNewManager();
    
    static const int cStep = 1024*10; // 10K every time.
    for (int i = 0; i*cStep < mgr.GetBinDataSize(); i++)
    {
        bool bLast = false;
        if ((i+1)*cStep >= mgr.GetBinDataSize())
            bLast = true;

        GameMsg_S2C_SendQuestNewStaticBinData msg(bLast, 
            bLast ? mgr.GetBinDataSize()-i*cStep : cStep,
            mgr.GetBinData() + i*cStep);

        _SendPlayerMsg(&msg);
    }
}

void CEntityQuestNewComponent::_SendUnRewardQuest(std::vector<unsigned short>& ids)
{
	if (ids.size() != 0)
	{
		GameMsg_S2C_UnRewardQuest msg;
		msg.SetIDList(ids);
		m_pNet->SendPlayerMsg(&msg);
	}
}

void CEntityQuestNewComponent::_PushCanCompleteQuest()
{
	std::vector<unsigned short> ids;
	_GetCanRewardQuest(ids);
	_SendUnRewardQuest(ids);
}

int CEntityQuestNewComponent::_GetCanRewardQuest(std::vector<unsigned short>& ids)
{
	int nNum   =  0;
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	for (;it != m_CurrentQuest.end() ; it++)
	{
		std::map<unsigned short,QUESTNEWINFO>::iterator itq = m_QuestDetailInfo.find(*it);
		if (itq != m_QuestDetailInfo.end())
		{
			if (itq->second.m_State == eQuestState_Completed)
			{
				ids.push_back(itq->first);
				nNum++;
			}
		}
	}
	return nNum;
}

void CEntityQuestNewComponent::OnChangeLocation(int nCurrentLocation)
{
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	for ( ; it != m_CurrentQuest.end() ; it++)
	{
        CQuestDocNew* pDoc = _GetQuestDoc(*it);
        ENSURE_WITH_LOG_CMD (pDoc != NULL, continue, "quest[%u] missed config.", *it);
        IQuestComplete* pComp = pDoc->GetComplete(EQuest_Complete_OpenComponent);
		if (pComp != NULL)
		{
			QuestCompleteOpenComponent* qComp = (QuestCompleteOpenComponent*)pComp;
			if ((int)qComp->GetComponent() == nCurrentLocation)
			{
				std::vector<int>::iterator itFind = _FindOpenLocation(nCurrentLocation);
				if (itFind == m_QuestLocationList.end())
				{
					m_QuestLocationList.push_back(nCurrentLocation);
					if (_CanCompleteQuest(pDoc,0))
					{
						_CompleteQuest(pDoc);
					}
				}
			}
		}
	}
}

std::vector<int>::iterator CEntityQuestNewComponent::_FindOpenLocation(int nLoc)
{
	std::vector<int>::iterator it = m_QuestLocationList.begin();
	for (;it != m_QuestLocationList.end() ; it++)
	{
		if(*it == nLoc)
			return it;
	}
	return m_QuestLocationList.end();
}

bool CEntityQuestNewComponent::_CheckCompleteOpenComponent(IQuestComplete* pComp)
{
	QuestCompleteOpenComponent* qComp = (QuestCompleteOpenComponent*)pComp;
	return  _FindOpenLocation(qComp->GetComponent()) != m_QuestLocationList.end();
}

void CEntityQuestNewComponent::_AddUseFunctionCount(unsigned short nQuestID, int nFunctionID)
{
	std::map<unsigned short,FunctionParamMap>::iterator it = m_UseFunctionCount.find(nQuestID);
	if (it == m_UseFunctionCount.end())
	{
        FunctionParamMap functionList;
		functionList.insert(make_pair(nFunctionID,(unsigned short)1));
		m_UseFunctionCount.insert(make_pair(nQuestID,functionList));
	}
	else
	{
		FunctionParamMap::iterator itFunction = it->second.find(nFunctionID);
		if (itFunction == it->second.end())
		{
			it->second.insert(make_pair(nFunctionID,(unsigned short)1));
		}
		else
		{
			itFunction->second ++;
		}
	}
	//更新数据库
	_UpdateQuest2DB(nQuestID);
}

void CEntityQuestNewComponent::OnClientCompleteQuest(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ClientCompleteQuest& cMsg = (GameMsg_C2S_ClientCompleteQuest&)msg;
//     QuestDocPtrMap::iterator itQuest = m_CurrentQuest.find(cMsg.m_nQuestID);
    CQuestDocNew *pDoc = _GetQuestDoc(cMsg.m_nQuestID);
    if (NULL != pDoc)
    {
        WriteLog(LOGLEVEL_DEBUG, "%s Plyaer [%s] send Complete Quest CompleteType = %d , nParam = %d .\n",QUESTLOG,m_pAttr->m_szRoleName,(int)cMsg.m_nCompleteType,(int)cMsg.m_nParam1);

        switch(cMsg.m_nCompleteType)
        {
        case EQuest_Complete_UseFunction:
            {
                ECompleteUseFunction functionType = (ECompleteUseFunction)cMsg.m_nParam1;
                //可信任客户端的功能
                switch(functionType)
                {
                case EUF_ChangeName:
                case EUF_ChangeDescribe:
                case EUF_ChangeTitle:
                case EUF_ChangeSpecialTitle:
                case EUF_BuyItem:
                case EUF_UseLucky:
                case EUF_ChargeTurntable:
                case EUF_TenLucky:
                case EUF_SaveImage:
                case EUF_ReceiveVIP:
                case EUF_CreateRoom:
                case EUF_PhotographSingle:
                case EUF_PhotographLover:
                case EUF_PhotographMuti:
                case EUF_SharePhoto:
                case EUF_PhotographSuccess:
                case EUF_UploadPhotograph:
                case EUF_SendDanceRequire:
                case EUF_RewardSevenDay:
                case EUF_RewardCheckIn:
                case EUF_RewardReCheckIn:
                case EUF_OpenMall:
                case EUF_AddFriend:
                case EUF_WatchGuideTradition:
                case EUF_WatchGuideOsu:
                case EUF_WatchGuideRhythm:
                case EUF_WatchGuideADB:
                case EUF_WatchGuideRainBow:
                case EUF_WatchGuideSuperOsu:
                case EUF_ClotheEffectOpen:
                case EUF_ClotheEffectReplace:
                case EUF_ClotheEffectLevelUp:
                    _AddUseFunctionCount(cMsg.m_nQuestID,cMsg.m_nParam1);
                    break;
                default:
                    break;
                }
            }
            break;
        case EQuest_Complete_OpenComponent:
            {
                QuestCompleteOpenComponent* pComp = (QuestCompleteOpenComponent*)pDoc->GetComplete(EQuest_Complete_OpenComponent);
                if (pComp != NULL)
                {
                    if (pComp->GetComponent() == (unsigned int)cMsg.m_nParam1)
                    {
                        m_QuestLocationList.push_back(cMsg.m_nParam1);
                    }
                }
            }
            break;
        default:
            break;
        }

        //检测可完成的任务
        if (_CanCompleteQuest(pDoc,cMsg.m_nParam1))
        {
            _CompleteQuest(pDoc);
        }
    }
}

bool CEntityQuestNewComponent::AddQuestByCmd(unsigned short nQuestID)
{
	CQuestDocNew* pQuest = _GetQuestDoc(nQuestID);
	if (pQuest == NULL)
	{
		return false;
	}
	if (m_CurrentQuest.find(nQuestID) != m_CurrentQuest.end())
	{
		return true;
	}
    m_HistroyQuest.erase(nQuestID);
    m_UnAcceptQuest.erase(nQuestID);


    RetsetRepeatQuest(nQuestID, true, false);
	_UpdateQuestState(nQuestID,eQuestState_Accepted/*,false,false*/,eQuestDestroy_None);
    m_CurrentQuest.insert(nQuestID);
    _AddDestroyQuestTimer(pQuest);
	_NotifyAcceptQuest(pQuest);
	return true;
}

bool CEntityQuestNewComponent::AddQuestByCmdPersistent(unsigned short nQuestID)
{
    CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
    if (pDoc != NULL)
    {
        RetsetRepeatQuest(pDoc->m_BaseInfo.ID, false, false);
        _AcceptQuest(pDoc,/*false,*/true,true);

        return true;
    }
    
    return false;
}


bool CEntityQuestNewComponent::CompleteQuestByCmd(unsigned short nQuestID)
{
	CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
	if (pDoc == NULL)
	{
		return false;
	}
	QuestIDSet::iterator itFind = m_CurrentQuest.find(nQuestID);
	if (itFind == m_CurrentQuest.end())
	{
		return false;
	}
	
	_UpdateQuestState(nQuestID,eQuestState_Completed);
	_NotifyChangeQuestState(nQuestID,eQuestState_Completed);

	return true;
}

int	CEntityQuestNewComponent::_GetQuestCompleteDayTimes(unsigned short nQuestID)
{
	std::map<unsigned short,QUESTNEWINFO>::iterator itFind = m_QuestDetailInfo.find(nQuestID);
	if (itFind != m_QuestDetailInfo.end())
	{
		return itFind->second.m_DayTimes;
	}
	return 0;
}
//完成任务日志
void CEntityQuestNewComponent::_LogRoleQuest(unsigned int nRoleID, unsigned int nQuestID)
{
	CRoleQuestLog * pRoleQuestLog = new CRoleQuestLog();
	pRoleQuestLog->m_nRoleID = nRoleID;
	pRoleQuestLog->m_nQuestID = nQuestID;
	AddQuery(QUERY_LogRoleQuest, 0, pRoleQuestLog);
}

// bool CEntityQuestNewComponent::IsReAcceptType(char nType)
// {
//     return (eQuestTypeNew_Activity == nType
//         || eQuestTypeNew_Daily == nType);
// }


std::map<unsigned short,QUESTNEWINFO>::iterator CEntityQuestNewComponent::EnsureGetDetailerInfo(CQuestDocNew *pDoc)
{
    std::map<unsigned short,QUESTNEWINFO>::iterator it = m_QuestDetailInfo.find(pDoc->m_BaseInfo.ID);
    if (it == m_QuestDetailInfo.end())
    {
        _InsertNewQuestData(pDoc);
        it = m_QuestDetailInfo.find(pDoc->m_BaseInfo.ID);
    }

    return it;
}


void CEntityQuestNewComponent::OnGetQuestState(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_GetQuestState& rMsg = (GameMsg_C2S_GetQuestState&)msg;
	GameMsg_S2C_GetQuestStateRes retMsg;

	std::list<unsigned short>::iterator it = rMsg.m_nQuestIDList.begin();
	for (;it != rMsg.m_nQuestIDList.end() ; it++ )
	{
		unsigned short nQuestID = *it;
		unsigned short nQuestState = eQuestState_None;
		CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
		if (pDoc != NULL)
		{
			std::map<unsigned short,QUESTNEWINFO>::iterator itFind = m_QuestDetailInfo.find(nQuestID);
			if (itFind != m_QuestDetailInfo.end())
			{
				if (_CanCompleteQuest(pDoc))
				{
					_CompleteQuest(pDoc,false);
				}
				else
				{
					_UpdateQuestState(nQuestID,eQuestState_Accepted);
				}
				nQuestState = itFind->second.m_State;
			}
		}
		retMsg.AddQuestState(nQuestID, nQuestState);
	}

	_SendPlayerMsg(&retMsg);
}

void CEntityQuestNewComponent::RemoveCoupleQuest()
{
	QuestIDSet::iterator it = m_CurrentQuest.begin();
	std::vector<unsigned short> destroyIDList;
	for ( ; it != m_CurrentQuest.end() ; ++it )
	{
        ENSURE_WITH_LOG_CMD(NULL != _GetQuestDoc(*it), continue, "quest[%u] miss config..", *it);
		if (_GetQuestDoc(*it)->IsDivorceDestroyQuest())
		{
			destroyIDList.push_back(*it);
		}
	}

	it = m_HistroyQuest.begin();
	for ( ; it != m_HistroyQuest.end() ; ++it )
	{
        ENSURE_WITH_LOG_CMD(NULL != _GetQuestDoc(*it), continue, "quest[%u] miss config..", *it);
		if (_GetQuestDoc(*it)->IsDivorceDestroyQuest())
		{
			destroyIDList.push_back(*it);
		}
	}

	std::vector<unsigned short>::iterator itQuest = destroyIDList.begin();
	for (;itQuest != destroyIDList.end(); itQuest++)
	{
		_DeleteQuest(*itQuest);
	}

	_SendQuestList();
}

void CEntityQuestNewComponent::_DeleteQuest(unsigned short nQuestID)
{
    m_CurrentQuest.erase(nQuestID);
    m_HistroyQuest.erase(nQuestID);
    CQuestDocNew* pDoc = _GetQuestDoc(nQuestID);
	if (pDoc != NULL)
	{
		m_UnAcceptQuest.insert(nQuestID);
	}
	_UpdateQuestState(nQuestID,eQuestState_None/*,false,false*/,eQuestDestroy_None);
}

void CEntityQuestNewComponent::RemoveDanceQuest()
{
    QuestIDSet::iterator it = m_CurrentQuest.begin();
    std::vector<unsigned short> destroyIDList;
    for ( ; it != m_CurrentQuest.end() ; it++ )
    {
        ENSURE_WITH_LOG_CMD(NULL != _GetQuestDoc(*it), continue, "quest[%u] miss config..", *it);

        if (_GetQuestDoc(*it)->IsDanceGroupDestroyQuest())
        {
            destroyIDList.push_back(*it);
        }
    }

    it = m_HistroyQuest.begin();
    for ( ; it != m_HistroyQuest.end() ; ++it )
    {
        ENSURE_WITH_LOG_CMD(NULL != _GetQuestDoc(*it), continue, "quest[%u] miss config..", *it);

        if (_GetQuestDoc(*it)->IsDanceGroupDestroyQuest())
        {
            destroyIDList.push_back(*it);
        }
    }

    std::vector<unsigned short>::iterator itQuest = destroyIDList.begin();
    for (;itQuest != destroyIDList.end(); itQuest++)
    {
        _DeleteQuest(*itQuest);
    }

    _SendQuestList();
}

unsigned int CEntityQuestNewComponent::_GetMusicTimes(unsigned short nQuestID,unsigned short nMusicID)
{
	std::map<unsigned short,MusicParamMap>::iterator itQuest = m_MusicCount.find(nQuestID);
	if (itQuest != m_MusicCount.end())
	{
		std::map<unsigned short,unsigned short >::iterator itMusic = itQuest->second.find(nMusicID);
		if (itMusic != itQuest->second.end())
		{
			return itMusic->second;
		}
	}
	return 0;
}

void CEntityQuestNewComponent::SendQuestVipReward()
{
    if (m_UnRewardVipQuest != 0)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(m_UnRewardVipQuest);
        if (pDoc != NULL)
        {
            _SendPlayerVipReward(pDoc);
            GameMsg_S2C_GetQuestRewardResult msg;
            msg.m_nQuestID = pDoc->m_BaseInfo.ID;
            msg.m_nReuslt = 0;
            _SendPlayerMsg(&msg);
        }
    }
}


void CEntityQuestNewComponent::CheckOverdueQuest()
{
    time_t nNow = time(NULL);
    for (std::map<unsigned short, QUESTNEWINFO>::iterator iter = m_QuestDetailInfo.begin(); iter != m_QuestDetailInfo.end(); ++iter)
    {
        if (m_HistroyQuest.find(iter->first) != m_HistroyQuest.end())
            continue;

        CQuestDocNew *pDoc = _GetQuestDoc(iter->first);
        ENSURE_WITH_LOG_CMD(NULL != pDoc, continue, "quest[%u] miss config.. ", iter->first);
        
        eQuestDestroy e = eQuestDestroy_None;
        if (!CheckQuestStartAndEndTime(pDoc, nNow))
        {
            e = eQuestDestroy_DateTime;
        }
        else if (!CheckQuestLastTime(pDoc, nNow))
        {
            e = eQuestDestroy_LastTime;
        }
        else
        {
            continue;
        }
        // 从当前任务中删除
        m_CurrentQuest.erase(iter->first);
        m_UnAcceptQuest.erase(iter->first);
        // 添加到历史任务
        m_HistroyQuest.insert(iter->first);
        // 更新任务状态：
        _UpdateQuestState(iter->first, eQuestState_Destroy, e); // 删除类型
    }
}

void CEntityQuestNewComponent::CheckCurrentDaily()
{
    time_t nNow = time(NULL);
    std::set<unsigned short> toUnAccept;
    std::set<unsigned short> toHistroy;
    // 超出了截止日期
    for (QuestIDSet::iterator citer = m_CurrentQuest.begin(); citer != m_CurrentQuest.end(); ++citer)
    {
        CQuestDocNew* pDoc = _GetQuestDoc(*citer);
        ENSURE_WITH_LOG_CMD (pDoc != NULL, continue, "quest[%u] missed config.", *citer);
        if (pDoc->QuestType() != eQuestTypeNew_Daily)
            continue; // 
        
        std::map<unsigned short,QUESTNEWINFO>::iterator itInfo = m_QuestDetailInfo.find(pDoc->QuestID());
        ENSURE_WITH_LOG_CMD(itInfo != m_QuestDetailInfo.end(), continue, "role[%u] quest[%u] miss detailer info...", m_pAttr->GetRoleID(), pDoc->QuestID());
        if (DiffDays(nNow, itInfo->second.m_UpdateTime) != 0)
        { // 超时间了。
            if (_CanAcceptQuest(pDoc))
            { // 还能接
                // do nothing.
                RetsetRepeatQuest(pDoc->QuestID(), true, false); // 清空记录
                _UpdateQuestState(pDoc->QuestID(), eQuestState_Accepted);
            }
            else if (AcceptAble(pDoc))
            {
                RetsetRepeatQuest(pDoc->QuestID(), true, true); // 清空记录
                toUnAccept.insert(pDoc->QuestID());
            }
            else
            { // 进历史了。
                RetsetRepeatQuest(pDoc->QuestID(), true, true); // 清空记录
                toHistroy.insert(pDoc->QuestID());
            }
        }
    }

    for (std::set<unsigned short>::iterator iter = toUnAccept.begin(); iter != toUnAccept.end(); ++iter)
    {
        m_CurrentQuest.erase(*iter);
        m_UnAcceptQuest.insert(*iter);
        _UpdateQuestState(*iter, eQuestState_None, eQuestDestroy_None);
    }

    for (std::set<unsigned short>::iterator iter = toHistroy.begin(); iter != toHistroy.end(); ++iter)
    {
        m_CurrentQuest.erase(*iter);
        m_HistroyQuest.insert(*iter);
        _UpdateQuestState(*iter, eQuestState_Destroy, eQuestDestroy_None); 
    }
}


void CEntityQuestNewComponent::CheckRealHistroy()
{
    std::set<unsigned short> accetpableSet;
    // 对于可重复接取的任务，如果还有次数，并且在有效时间段内，就拿出来，继续处理
    for (QuestIDSet::iterator hiter = m_HistroyQuest.begin(); hiter != m_HistroyQuest.end(); ++hiter)
    { 
        ENSURE_WITH_LOG_CMD(_GetQuestDoc(*hiter), continue, "quest[%u] miss configs.", *hiter);
        CQuestDocNew* pDoc = _GetQuestDoc(*hiter);

        if (pDoc->IsReAcceptType() && AcceptAble(pDoc))
//            if (pDoc->IsReAcceptType() && CheckQuestLastTime(pDoc, uNow) && CheckQuestStartAndEndTime(pDoc, uNow) && HasCompleteTimes(pDoc))
        {
            accetpableSet.insert(pDoc->QuestID()); // 这个还有被接的可能
        }
    }
    
    // 移动到未接中:
    for (std::set<unsigned short>::iterator iter = accetpableSet.begin(); iter != accetpableSet.end(); ++iter)
    {
        m_HistroyQuest.erase(*iter);
        m_UnAcceptQuest.insert(*iter);
        _UpdateQuestState(*iter, eQuestState_None, eQuestDestroy_None); // 可以再次接取
    }
}

bool CEntityQuestNewComponent::CheckQuestStartAndEndTime(CQuestDocNew* pDoc, time_t now)
{
    ENSURE_WITH_LOG_CMD(NULL != pDoc, return false, "QuestDocNew should not null");

    return !((pDoc->m_BaseInfo.StartTime != 0 && now < (time_t)pDoc->m_BaseInfo.StartTime) || 
        (pDoc->m_BaseInfo.EndTime != 0 && now > (time_t)pDoc->m_BaseInfo.EndTime));
}

bool CEntityQuestNewComponent::CheckQuestLastTime(CQuestDocNew* pDoc, time_t now)
{
    ENSURE_WITH_LOG_CMD(NULL != pDoc, return false, "QuestDocNew should not null");

    if (pDoc->m_BaseInfo.LastTime == 0)
        return true;
    std::map<unsigned short,QUESTNEWINFO>::iterator itd = m_QuestDetailInfo.find(pDoc->QuestID());
    if (itd == m_QuestDetailInfo.end())
        return true;
    return (now < (time_t)(itd->second.m_CreateTime + pDoc->m_BaseInfo.LastTime));
}

bool CEntityQuestNewComponent::HasCompleteTimes(CQuestDocNew* pDoc)
{
    ENSURE_WITH_LOG_CMD(NULL != pDoc, return false, "QuestDocNew should not null");

    if (pDoc->IsReAcceptType() && (pDoc->m_BaseInfo.CompleteTimes == 0 || pDoc->m_BaseInfo.CompleteTimes == 0xFFFFFFFF))  // 不限制次数
        return true;
    std::map<unsigned short,QUESTNEWINFO>::iterator itd = m_QuestDetailInfo.find(pDoc->QuestID());
    if (itd == m_QuestDetailInfo.end())
        return true;
    if (!pDoc->IsReAcceptType() && itd->second.m_Times >= 1)
        return false;
    return (itd->second.m_Times < pDoc->m_BaseInfo.CompleteTimes);
}

bool CEntityQuestNewComponent::AcceptAble(CQuestDocNew* pDoc)
{
    time_t now = time(NULL);
    return CheckQuestStartAndEndTime(pDoc, now) && CheckQuestLastTime(pDoc, now) && HasCompleteTimes(pDoc);
}

