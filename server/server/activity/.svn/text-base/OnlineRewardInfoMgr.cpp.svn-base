#include <list>
#include "OnlineRewardInfoMgr.h"
#include "SystemActivityManager.h"
#include "../../socket/Log.h"
#include "../../socket/Formatter.h"
#include "../item/ItemProcess.h"
#include "../logic/PlayerManager.h"
#include "EntityActivityComponent.h"
#include "../util/CommonFunc.h"


COnlineRewardAcitvity::COnlineRewardAcitvity()
{
	m_nActivityIdx = 0;
	SetActivityType(EActivity_OnlineReward);
}

COnlineRewardAcitvity::~COnlineRewardAcitvity()
{

}

void COnlineRewardAcitvity::Init(time_t tBeginTime, time_t tEndTime, unsigned short nActivityIdx)
{
    m_nActivityIdx = nActivityIdx;
    CTimeActivity::Init(tBeginTime, tEndTime);
    //COnlineRewardInfoMgr::Instance().SetCurActivityIdx(m_nActivityIdx);
}


void COnlineRewardAcitvity::OnActivityBegin()
{
	COnlineRewardInfoMgr::Instance().SetIsOpening(true);
	COnlineRewardInfoMgr::Instance().SetCurActivityIdx(m_nActivityIdx);

	std::list< CRoleEntity * > roleList;
	CPlayerManager::Instance().GetAllPlayers(roleList);
	std::list< CRoleEntity * >::iterator itList = roleList.begin();
	for (; itList != roleList.end(); ++itList)
	{
		CRoleEntity * pRoleEntity = *itList;
		if (pRoleEntity)
		{
			CRoleActivity *pRoleActivity = pRoleEntity->GetComponent< CRoleActivity >();
			if (pRoleActivity != NULL) {
				pRoleActivity->OnOnlineRewardActivityStart();
			}
		}
	}
}

void COnlineRewardAcitvity::OnActivityEnd()
{
	std::list< CRoleEntity * > roleList;
	CPlayerManager::Instance().GetAllPlayers(roleList);
	std::list< CRoleEntity * >::iterator itList = roleList.begin();
	for (; itList != roleList.end(); ++itList)
	{
		CRoleEntity * pRoleEntity = *itList;
		if (pRoleEntity)
		{
			CRoleActivity *pRoleActivity = pRoleEntity->GetComponent< CRoleActivity >();
			if (pRoleActivity != NULL) {
				pRoleActivity->OnOnlineRewardActivityEnd();
			}
		}
	}

	if ( m_bBegun )
	{
		COnlineRewardInfoMgr::Instance().SetIsOpening(false);
		COnlineRewardInfoMgr::Instance().SetCurActivityIdx(0);
	}
}

COnlineRewardInfo::COnlineRewardInfo()
:m_nActivityIndex(-1)
,m_nIndex(-1)
,m_nRequireTime(0)
,m_nMoney(0)
,m_tStartTime(0)
,m_tEndTime(0)
{

}

COnlineRewardInfo::~COnlineRewardInfo()
{

}


COnlineRewardInfoMgr::COnlineRewardInfoMgr()
:m_bIsOpening(false)
,m_bExhibit(false)
,m_nWeight(0)
,m_nShowBeginTime(0)
,m_nShowEndTime(0)
,m_nActivityBeginTime(0)
,m_nActivityEndTime(0)
,m_strRegularImageName("")
,m_strThumbnailName("")
,m_strActivityName("")
,m_strContentFormat("")
,m_nResetHour(0)
,m_nResetMin(0)
,m_nResetSec(0)
,m_nCurActivityIndex(0)
,m_nActivityShowIndex(1)
{
}

COnlineRewardInfoMgr::~COnlineRewardInfoMgr()
{
	for (std::map<unsigned short, COnlineRewardInfo *>::iterator itr = m_mapOnlineRewardInfo.begin(); itr != m_mapOnlineRewardInfo.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}
	m_mapOnlineRewardInfo.clear();
}

COnlineRewardInfoMgr & COnlineRewardInfoMgr::Instance()
{
	static COnlineRewardInfoMgr s_OnlineRewardInfoMgr;
	return s_OnlineRewardInfoMgr;
}

bool COnlineRewardInfoMgr::Init()
{
	if ( !IsTimeRangeApart(m_mapActivityTimeRange) )
	{
		return false;
	}
	if ( m_mapActivityTimeRange.size() != m_mapActivityTime2Idx.size() )
	{
		return false;
	}
	for (std::map<time_t, time_t>::iterator it = m_mapActivityTimeRange.begin();
		it != m_mapActivityTimeRange.end(); ++it)
	{
		if ( it->first == it->second )
		{
			return false;
		}
		COnlineRewardAcitvity * pActivity = new COnlineRewardAcitvity();
		pActivity->Init(it->first, it->second, m_mapActivityTime2Idx[it->first]);
		//m_bIsOpening = m_bIsOpening || pActivity->IsOpening();
		CSystemActivityManager::Instance().AddTimerActivity(pActivity);
	}
	return true;
}

bool COnlineRewardInfoMgr::IsShowing()
{
	unsigned int nTimeNow = (unsigned int)time(NULL);
	return  nTimeNow > m_nShowBeginTime && nTimeNow < m_nShowEndTime;
}

bool COnlineRewardInfoMgr::Exhibit()
{
	return m_bExhibit;
}

int COnlineRewardInfoMgr::Weight()
{
	return m_nWeight;
}

unsigned int COnlineRewardInfoMgr::ShowBeginTime()
{
	return m_nShowBeginTime;
}

unsigned int COnlineRewardInfoMgr::ShowEndTime()
{
	return m_nShowEndTime;
}

unsigned int COnlineRewardInfoMgr::ActivityBeginTime()
{
	return m_nActivityBeginTime;
}

unsigned int COnlineRewardInfoMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

bool COnlineRewardInfoMgr::ActivityTimeToShow( unsigned int& nBeginTime, unsigned int& nEndTime, std::string& strContent )
{
	time_t tNow = time(NULL);
	for (std::map<unsigned short, COnlineRewardInfo *>::iterator itr = m_mapOnlineRewardInfo.begin();
		itr != m_mapOnlineRewardInfo.end(); ++itr)
	{
		if ( (itr->second->m_tStartTime <= tNow && tNow <= itr->second->m_tEndTime) ||
			itr->second->m_tStartTime > tNow )
		{
			nBeginTime = (unsigned int)itr->second->m_tStartTime;
			nEndTime = (unsigned int)itr->second->m_tEndTime;

			tm tmStartTime, tmEndTime;
			tm* pStartTime = localtime(&itr->second->m_tStartTime);
			memcpy(&tmStartTime, pStartTime, sizeof(tm));
			tm* pEndTime = localtime(&itr->second->m_tEndTime);
			memcpy(&tmEndTime, pEndTime, sizeof(tm));

			SafeFormat(strContent, m_strContentFormat, 
				tmStartTime.tm_mon+1, tmStartTime.tm_mday, tmStartTime.tm_hour, tmStartTime.tm_min,
				tmEndTime.tm_mon+1, tmEndTime.tm_mday, tmEndTime.tm_hour, tmEndTime.tm_min);

			return true;
		}
	}
	return false;
}

std::string & COnlineRewardInfoMgr::RegularImageName()
{
	return m_strRegularImageName;
}

std::string & COnlineRewardInfoMgr::ThumbnailName()
{
	return m_strThumbnailName;
}

std::string & COnlineRewardInfoMgr::ActivityName()
{
	return m_strActivityName;
}

const std::map<unsigned short, COnlineRewardInfo *> & COnlineRewardInfoMgr::OnlineRewardInfoMap()
{
	return m_mapOnlineRewardInfo;
}

void COnlineRewardInfoMgr::ReadBriefInfo(int nWeight, bool bExhibit, 
										 unsigned int nShowBegintime, unsigned int nShowEndtime, 
										 std::string & rStrRegularImageName, std::string & rStrThumbnailName,
										 std::string & rStrActivityName, std::string & rStrContent,
										 int nResetHour, int nResetMin, int nResetSec)
{
	m_nWeight = nWeight;
	m_bExhibit = bExhibit;
	m_nShowBeginTime = nShowBegintime;
	m_nShowEndTime = nShowEndtime;
	m_strRegularImageName = rStrRegularImageName;
	m_strThumbnailName = rStrThumbnailName;
	m_strActivityName = rStrActivityName;
	m_strContentFormat = rStrContent;
	m_nResetHour = nResetHour;
	m_nResetMin = nResetMin;
	m_nResetSec = nResetSec;
}

void COnlineRewardInfoMgr::ReadOnlineRewardInfo(COnlineRewardInfo &onlineRewardInfo)
{
	COnlineRewardInfo *pInfo = new COnlineRewardInfo(onlineRewardInfo);

	if (pInfo != NULL) 
    {
		unsigned int idx = (pInfo->m_nActivityIndex << 8) + pInfo->m_nIndex;
		m_mapOnlineRewardInfo.insert(make_pair((unsigned short)idx, pInfo));
		m_mapActivityTimeRange.insert(make_pair(pInfo->m_tStartTime, pInfo->m_tEndTime));
        m_mapActivityTime2Idx.insert(make_pair(pInfo->m_tStartTime, (unsigned short)pInfo->m_nActivityIndex));
	}
}

COnlineRewardInfo * COnlineRewardInfoMgr::GetOnlineRewardInfo(int nIndex)
{
	COnlineRewardInfo *pInfo = NULL;
	unsigned short nIdx = (m_nCurActivityIndex << 8) + (unsigned short)nIndex;
	std::map<unsigned short, COnlineRewardInfo *>::iterator itr = m_mapOnlineRewardInfo.find(nIdx);
	if (itr != m_mapOnlineRewardInfo.end()) {
		return itr->second;
	}

	return pInfo;
}

bool COnlineRewardInfoMgr::IsEndActivityIndex(int nIndex)
{
    ++nIndex;
    COnlineRewardInfo * pInfo = GetOnlineRewardInfo(nIndex);
    if (pInfo == NULL)
    {
        return true;
    }

    return false;
}

void COnlineRewardInfoMgr::GetRewardList(int nIndex, std::list<CItem> & listReward, unsigned char nSex)
{
	COnlineRewardInfo * pInfo = GetOnlineRewardInfo(nIndex);
	if (pInfo != NULL) {
		if (nSex == ESexType_Male) {
			listReward = pInfo->m_listMaleReward;
		} else if (nSex == ESexType_Female){
			listReward = pInfo->m_listFemaleReward;
		} else {
			WriteLog(LOGLEVEL_ERROR, "GetRewardList error, sex type invalid\n");
		}
	}
}

int COnlineRewardInfoMgr::GetRewardMoney(int nIndex)
{
	COnlineRewardInfo * pInfo = GetOnlineRewardInfo(nIndex);
	if (pInfo != NULL) {
		return pInfo->m_nMoney;
	}

	return 0;
}

time_t COnlineRewardInfoMgr::GetResetTime(time_t nNow)
{
	if (nNow == 0) {
		nNow = time(NULL);
	}

	struct tm tTime;
	LocalTime(&nNow, &tTime);

	tTime.tm_hour = m_nResetHour;
	tTime.tm_min = m_nResetMin;
	tTime.tm_sec = m_nResetSec;

	time_t nTodayResetTime = mktime(&tTime);
	time_t nResetTime = nTodayResetTime;
	if (nNow >= nTodayResetTime) {
		nResetTime = nTodayResetTime + 24 * 60 * 60 ;
	} 

	return nResetTime;
}

void COnlineRewardInfoMgr::SetIsOpening( bool bOpening )
{
	m_bIsOpening = bOpening;
}

bool COnlineRewardInfoMgr::IsOpening()
{
	return m_bIsOpening;
}

void COnlineRewardInfoMgr::SetCurActivityIdx( unsigned short nActivityIndex )
{
    if ( m_nCurActivityIndex != nActivityIndex )
    {
        m_nCurActivityIndex = nActivityIndex;
        m_mapOnlineRewardInfoToShow.clear();

        bool bUpdateShow = false;
        if ( nActivityIndex == 0 )
        {
            unsigned short nIdx = (m_nActivityShowIndex << 8) + (unsigned short)1;
            std::map<unsigned short, COnlineRewardInfo *>::iterator itr = m_mapOnlineRewardInfo.find(nIdx);
            if (itr != m_mapOnlineRewardInfo.end()) 
            {
                time_t tNow = time(NULL);
                if (itr->second->m_tStartTime >= tNow && itr->second->m_tEndTime < tNow)
                {
                    ++m_nActivityShowIndex;
                    bUpdateShow = true;
                }
            }
        }
        else
        {
            m_nActivityShowIndex = nActivityIndex;
            bUpdateShow = true;
        }

        if ( bUpdateShow )
        {
            for (std::map<unsigned short, COnlineRewardInfo *>::iterator it = m_mapOnlineRewardInfo.begin();
                it != m_mapOnlineRewardInfo.end(); ++it)
            {
                if ( it->second->m_nActivityIndex == m_nActivityShowIndex )
                {
                    m_mapOnlineRewardInfoToShow.insert(std::make_pair((unsigned short)it->second->m_nIndex, it->second));
                }
            }
        }
    }
}

bool COnlineRewardInfoMgr::GetCurActivityTime(time_t& nBeginTime, time_t& nEndTime)
{
	COnlineRewardInfo* pInfo = GetOnlineRewardInfo(1);
	if ( pInfo )
	{
		nBeginTime = pInfo->m_tStartTime;
		nEndTime = pInfo->m_tEndTime;
		return true;
	}
	return false;
}

bool COnlineRewardInfoMgr::IsTimeInCurActivityTimeRange( time_t t )
{
	COnlineRewardInfo* pInfo = GetOnlineRewardInfo(1);
	if ( pInfo )
	{
		return (pInfo->m_tStartTime <= t && t < pInfo->m_tEndTime);
	}
	return false;
}






