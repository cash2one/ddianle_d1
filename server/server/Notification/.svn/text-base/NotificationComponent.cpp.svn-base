#include "../util/CommonFunc.h"
#include "../../socket/GameMsg_Map.h"
#include "../libServerFrame/QueryManager.h"
#include "NotificationManager.h"

#include "NotificationComponent.h"
#include "PushRatingDataManager.h"
#include "GameMsg_Notification.h"

#ifdef WIN32
#pragma warning(disable:4702)
#endif

CNotificationComponent::CNotificationComponent()
: CEntityComponent(true, true)
{
	m_pAttr = NULL;
	m_pNet = NULL;

	m_nUpdateInterval = 0;

	m_nLoginTime = 0;
	m_nLastPushRatingTime = 0;

	m_nLastPushRatedLevel = 0;
	m_nNoPushRatingTime = 0;
	m_nPushRatedOnlineTime = 0;
	m_nPushRatedDailyQuest = 0;
	m_nPushRatedDailyShoping = 0;

	m_nPushRatedCount = 0;
	m_nPushRatedCountTime = 0;
}

CNotificationComponent::~CNotificationComponent()
{
}

void CNotificationComponent::RegComponentNetMsgMap()
{
	// 部分安卓和苹果
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InfoCenterGetIDRes);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InfoCenterMarkReaded);

	RegMsgProcessor(Msg_C2S_InfoCenterGetIDRes, &CNotificationComponent::OnInfoCenterGetIDRes);
	RegMsgProcessor(Msg_C2S_InfoCenterMarkReaded, &CNotificationComponent::InfoCenterMarkReaded);

#ifdef SERVER_IOS
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_NoPushRatingNotification);
    RegMsgProcessor(MSG_C2S_NoPushRatingNotification, &CNotificationComponent::OnNoPushRating);
#endif //SERVER_IOS
}


void CNotificationComponent::OnNoPushRating(GameMsg_Base &msg, CSlotPeer &slot)
{
	if (m_nNoPushRatingTime)
		return;

	m_nNoPushRatingTime = (unsigned int)time(NULL);
	AddQuery(QUERY_Rating_NoPushRating, m_pAttr->m_nRoleID, NULL, m_nNoPushRatingTime);
}

void CNotificationComponent::OnInfoCenterGetIDRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_InfoCenterGetIDRes &rmsg = (GameMsg_C2S_InfoCenterGetIDRes &)msg;
	
	GameMsg_S2C_InfoCenterGetIDRes sendMsg;
	sendMsg.m_nErros = EInfoCenter_MissID;

	do 
	{
		if (!NotificationManager::Instance().FillInfo(rmsg.m_ids, sendMsg.m_infos, m_pAttr->m_nPID))
		{
			break;
		}

		sendMsg.m_nErros = EInfoCenter_OK;
	} while (0);

	SendPlayerMsg(sendMsg);  // 给这个人发送信息

	return ;
}

void CNotificationComponent::InfoCenterMarkReaded(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
	GameMsg_C2S_InfoCenterMarkReaded &rmsg = (GameMsg_C2S_InfoCenterMarkReaded &)msg;

	unsigned int now = (unsigned int)time(NULL); // 当前时间

	for (size_t i = 0; i < rmsg.m_ids.size(); ++i)
	{
		MarkInfoCenterRead(rmsg.m_ids[i], now);
	}

	return ;
}



void CNotificationComponent::Start()
{
	m_pAttr = GetComponent<CRoleAttribute>();
	m_pNet = GetComponent<CRoleNet>();
}

void CNotificationComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
    const static int kUpdateInterval = 1000;

	m_nUpdateInterval += nTimeElapsed;
	if (m_nUpdateInterval >= kUpdateInterval)
    {
#ifdef SERVER_IOS
        UpdatePushRating();
#endif //SERVER_IOS

		m_nUpdateInterval -= kUpdateInterval;
	}

    OnLazyPost(nTimeElapsed);
}

void CNotificationComponent::UpdatePushRating()
{
	if (!CPushRatingDataManager::Instance().IsEnabled())
	{
		return;
	}

	// 是否设置今日不再提醒
	if (m_nNoPushRatingTime)
	{
		while (!m_ratingEventQueue.empty())
			m_ratingEventQueue.pop();
		return;
	}

	// 最小时间间隔判断
	unsigned int nNow = (unsigned int)time(NULL);
	if (nNow - m_nLastPushRatingTime < (unsigned int)CPushRatingDataManager::Instance().Interval())
	{
		return;
	}

	bool bCanPush = false;
	int nPushReason = 0;
	while (!m_ratingEventQueue.empty())
	{
		CNotificationEvent event = m_ratingEventQueue.front();
		nPushReason = event.m_nEventType;

		switch (event.m_nEventType)
		{
		case NOTIFICATION_LEVEL_UP://玩家升级
			if (m_nLastPushRatedLevel != m_pAttr->GetLevel()
				&& CPushRatingDataManager::Instance().CanPushOnLevel(m_pAttr->GetLevel()) )
			{
				bCanPush = true;
			}
			break;
		case NOTIFICATION_FINISH_QUEST://当天的第一个日常任务
			if ( !m_nPushRatedDailyQuest
				&& CPushRatingDataManager::Instance().CanPushFirstDailyQuest() )
			{
				bCanPush = true;
			}
			break;
		case NOTIFICATION_FINISH_SHOPING://当天的第一次商城购买(非新手引导购买)
			if ( !m_nPushRatedDailyShoping
				&& CPushRatingDataManager::Instance().CanPushFirstDailyShoping()
				&& m_pAttr->m_nCompleteProcGuide)	
			{
				bCanPush = true;
			}
			break;
		default:
			break;
		}

		m_ratingEventQueue.pop();
	}

	if ( !m_nPushRatedOnlineTime//在线时长达到特定时长
		&& CPushRatingDataManager::Instance().CanPushOnlineTime(nNow-m_nLoginTime))
	{
		nPushReason = NOTIFICATION_ONLINE_TIME;
		bCanPush = true;
	}

	if (CPushRatingDataManager::Instance().ExceedMaxCount(m_nPushRatedCount))
	{
		bCanPush = false;
	}

	if (bCanPush)
	{
		PushRatingMessage(nNow, nPushReason);
	}
}

void CNotificationComponent::PushRatingMessage(unsigned int nNow, int nPushReason)
{
	switch (nPushReason)
	{
	case NOTIFICATION_LEVEL_UP:
		AddQuery(QUERY_Rating_PushRatedLevelUp, m_pAttr->m_nRoleID, NULL, m_pAttr->m_nLevel);
		m_nLastPushRatedLevel = m_pAttr->m_nLevel;
		break;
	case NOTIFICATION_FINISH_SHOPING:
		AddQuery(QUERY_Rating_PushRatedDailyShoping, m_pAttr->m_nRoleID, NULL, nNow);
		m_nPushRatedDailyShoping = nNow;
		break;
	case NOTIFICATION_FINISH_QUEST:
		AddQuery(QUERY_Rating_PushRatedDailyQuest, m_pAttr->m_nRoleID, NULL, nNow);
		m_nPushRatedDailyQuest = nNow;
		break;
	case NOTIFICATION_ONLINE_TIME:
		AddQuery(QUERY_Rating_PushRatedOnlineTime, m_pAttr->m_nRoleID, NULL, nNow);
		m_nPushRatedOnlineTime = nNow;
		break;
	default:
		return;
	}
	m_nLastPushRatingTime = nNow;

	if (m_nPushRatedCount == 0) // 每周清0，重新记录推送时间
	{
		m_nPushRatedCountTime = nNow;
	}
	m_nPushRatedCount++;

	RolePushRatingDB *pRating = new RolePushRatingDB;
	pRating->nPushRatedCount = m_nPushRatedCount;
	pRating->nPushRatedCountTime = m_nPushRatedCountTime;
	AddQuery(QUERY_Rating_PushRatedCount, m_pAttr->m_nRoleID, pRating);

	GameMsg_S2C_PushRatingNotification msg;
	msg.m_strMessage = CPushRatingDataManager::Instance().Message();
	m_pNet->SendPlayerMsg(&msg);
}

bool CNotificationComponent::CreateFromDB(ROLEINFO_DB* pRoleInforDB)
{
	if (IsTimeIsToday(pRoleInforDB->sPushRatingDB.nNoPushRatingTime))
		m_nNoPushRatingTime = pRoleInforDB->sPushRatingDB.nNoPushRatingTime;

	m_nLastPushRatedLevel = pRoleInforDB->sPushRatingDB.nPushRatedLevel;
	if (IsTimeIsToday(pRoleInforDB->sPushRatingDB.nPushRatedOnlineTime))
		m_nPushRatedOnlineTime = pRoleInforDB->sPushRatingDB.nPushRatedOnlineTime;
	if (IsTimeIsToday(pRoleInforDB->sPushRatingDB.nPushRatedDailyQuest))
		m_nPushRatedDailyQuest = pRoleInforDB->sPushRatingDB.nPushRatedDailyQuest;
	if (IsTimeIsToday(pRoleInforDB->sPushRatingDB.nPushRatedDailyShoping))
		m_nPushRatedDailyShoping = pRoleInforDB->sPushRatingDB.nPushRatedDailyShoping;

	m_nPushRatedCount = pRoleInforDB->sPushRatingDB.nPushRatedCount;
	m_nPushRatedCountTime = pRoleInforDB->sPushRatingDB.nPushRatedCountTime;
	
	// 超过一周，或者到了周三，就重置推送次数
	const static int kDaySeconds = 24 * 3600;
	const static int kWeekSeconds = 7 * kDaySeconds;
	int nResetWeekday = CPushRatingDataManager::Instance().ResetWeekday();

	unsigned int nNow = (unsigned int)time(NULL);
	if ( (nNow >= m_nPushRatedCountTime + kWeekSeconds )
		|| (nNow > m_nPushRatedCountTime + kDaySeconds && GetWeekDay(nNow) == nResetWeekday))
	{
		m_nPushRatedCount = 0;
		m_nPushRatedCountTime = 0;
	}

	m_infoCenterReadFlag.clear();
	for (size_t i = 0; i < pRoleInforDB->m_vecInfoCenterReaded.size(); ++i)
	{ // 全部添加进来
		m_infoCenterReadFlag[pRoleInforDB->m_vecInfoCenterReaded[i].first] = pRoleInforDB->m_vecInfoCenterReaded[i].second;
	}

	
	return true;
}

bool CNotificationComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
	return true;
}

bool CNotificationComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    for (InfoCenterReadFlag::const_iterator citer = m_infoCenterReadFlag.begin(); citer != m_infoCenterReadFlag.end(); ++citer)
    {
        pRoleInfoCache->m_vecInfoCenterReaded.push_back(*citer);
    }
    return true;
}


void CNotificationComponent::OnLogin()
{
	m_nLoginTime = (unsigned int)time(NULL);
}

void CNotificationComponent::OnLogout()
{

}

void CNotificationComponent::SendExtraStaticData()
{
}


void CNotificationComponent::SerializeComponent(CParamPool &IOBuff)
{
	std::vector<InfoCenterItem*> infos;
	NotificationManager::Instance().FillAllInfo(infos, m_pAttr->PID());
	IOBuff.AddUShort((unsigned short)infos.size());
	for (std::vector<InfoCenterItem*>::iterator iter = infos.begin(); iter != infos.end(); ++iter)
	{
		InfoCenterItem* item = *iter;
		IOBuff.AddUInt(item->m_id);
		IOBuff.AddUInt(item->m_updateTime);
		IOBuff.AddUChar(item->m_group);
		IOBuff.AddUChar(item->m_type);
		IOBuff.AddBool(HasReadInfoCenterInfo(item->m_id, item->m_updateTime));
	}
}

bool CNotificationComponent::HasReadInfoCenterInfo(unsigned int id, unsigned int lastUpdateTime)
{
	InfoCenterReadFlag::iterator iter = m_infoCenterReadFlag.find(id);
	if (iter == m_infoCenterReadFlag.end())
		return false; // 没读过

	return iter->second >= lastUpdateTime; 
}

// 记录已经读过，更新内存，和数据库
bool CNotificationComponent::MarkInfoCenterRead(unsigned int id, unsigned int now)
{
	InfoCenterReadFlag::iterator iter = m_infoCenterReadFlag.find(id);
	if (iter == m_infoCenterReadFlag.end())
	{
		m_infoCenterReadFlag.insert(std::make_pair(id, now));
	}
	else
	{
		iter->second = now;
	}
    
    std::pair<unsigned int, unsigned int> *p = new std::pair<unsigned int, unsigned int>();
    p->first = id;
    p->second = now;

	g_pQueryMgr->AddQuery(QUERY_InfoCenter_UpdateRoleReadInfo, m_pAttr->GetRoleID(), p, 0, NULL, NULL);

	return true;
}


void CNotificationComponent::SendPlayerMsg(GameMsg_Base &msg)
{
	ENSURE_WITH_LOG_CMD(NULL != m_pNet, return, "net component should not null");

	m_pNet->SendPlayerMsg(&msg);
}


void CNotificationComponent::AddEvent(CNotificationEvent &event)
{
	if (!CPushRatingDataManager::Instance().IsEnabled())
		return;

	if (m_nNoPushRatingTime)
		return;

	m_ratingEventQueue.push(event);
}

void CNotificationComponent::LazyPost(GameMsg_Base *msg, int nMilliseconds)
{
    m_listLazyMessage.push_back(LazyMessage(msg, nMilliseconds, m_pAttr->GetRoleID() % nMilliseconds));
}

void CNotificationComponent::LazyPost(SharedPtr<GameMsg_Base> &msg, int nMilliseconds)
{
    m_listLazyMessage.push_back(LazyMessage(msg, nMilliseconds, m_pAttr->GetRoleID() % nMilliseconds));
}

void CNotificationComponent::OnLazyPost(int nElapsedMilliseconds)
{
    if (m_listLazyMessage.empty())
    {
        return;
    }

    for (std::list<LazyMessage>::iterator it = m_listLazyMessage.begin();
        it != m_listLazyMessage.end(); )
    {
        LazyMessage &msg = *it;
        msg.Elapsed(nElapsedMilliseconds);
        if (msg.IsPostable())
        {
            SendPlayerMsg(*(msg.GetMsg()));
            m_listLazyMessage.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

LazyMessage::LazyMessage(GameMsg_Base *msg, int nPostTime, int nThreshold)
: m_msg(msg)
, m_nElapsedTime(0)
, m_nThresholdTime(nThreshold)
, m_nPostTime(nPostTime)
{
}

LazyMessage::LazyMessage(SharedPtr<GameMsg_Base> &msg, int nPostTime, int nThreshold)
: m_msg(msg)
, m_nElapsedTime(0)
, m_nThresholdTime(nThreshold)
, m_nPostTime(nPostTime)
{
}

LazyMessage::~LazyMessage()
{
}

GameMsg_Base *LazyMessage::GetMsg()
{
    if (m_msg.IsValid())
    {
        return m_msg.Get();
    }
    return NULL;
}

void LazyMessage::Elapsed(int nTime)
{
    m_nElapsedTime += nTime;
}

bool LazyMessage::IsPostable() const
{
    if (!m_msg.IsValid())
        return false;
    if (m_nElapsedTime >= m_nThresholdTime || m_nElapsedTime >= m_nPostTime)
        return true;
    return false;
}
