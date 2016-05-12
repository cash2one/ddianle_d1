#include "FestivalWishDataMgr.h"
#include "FestivalWishActivity.h"
#include "SystemActivityManager.h"

//---------------------------------------------CFestivalWishRequireData_Raw
CFestivalWishRequireData_Raw::CFestivalWishRequireData_Raw()
:m_nWishStartTimes(0)
,m_nWishEndTimes(0)
,m_nItemType(0)
,m_nItemCount(0)
{

}

CFestivalWishRequireData_Raw::~CFestivalWishRequireData_Raw()
{

}

//---------------------------------------------CFestivalWishRequireData
CFestivalWishRequireData::CFestivalWishRequireData()
:m_nWishTimes(0)
,m_nItemType(0)
,m_nIteCount(0)
{

}

//--------------------------------------------CFestivalWishRequireData
CFestivalWishRequireData::~CFestivalWishRequireData()
{

}

CFestivalWishRewardData::CFestivalWishRewardData()
:m_nTimes(0)
,m_nMoney(0)
{

}

CFestivalWishRewardData::~CFestivalWishRewardData()
{

}

//--------------------------------------------CFestivalWishDataMgr
CFestivalWishDataMgr::CFestivalWishDataMgr()
:m_bIsOpening(false)
,m_nActivityStartTime(0)
,m_nActivityEndTime(0)
,m_nMaxWishTimes(0)
{

}

CFestivalWishDataMgr::~CFestivalWishDataMgr()
{
	for (std::map<int, CFestivalWishRewardData *>::iterator itr = m_mapFestivalWishReward.begin(); itr != m_mapFestivalWishReward.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}
	m_mapFestivalWishReward.clear();

	for (std::map<int, CFestivalWishRequireData *>::iterator itr = m_mapFestivalWishRequireInfo.begin(); itr != m_mapFestivalWishRequireInfo.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}

	for (std::list<CFestivalWishRequireData_Raw *>::iterator itr = m_listRawFestivalWishInfo.begin(); itr != m_listRawFestivalWishInfo.end(); ++itr) {
		CFestivalWishRequireData_Raw *pData_Raw = *itr;
		if (pData_Raw != NULL) {
			delete pData_Raw, pData_Raw = NULL;
		}
	}
	m_mapFestivalWishRequireInfo.clear();
}

CFestivalWishDataMgr & CFestivalWishDataMgr::Instance()
{
	static CFestivalWishDataMgr s_XmasMakeWishDataMgr;

	return s_XmasMakeWishDataMgr;
}

bool CFestivalWishDataMgr::Init()
{
	for (std::list<CFestivalWishRequireData_Raw *>::iterator itr = m_listRawFestivalWishInfo.begin(); itr != m_listRawFestivalWishInfo.end(); ++itr) {
		if (*itr != NULL) {
			if ((*itr)->m_nWishStartTimes > (*itr)->m_nWishEndTimes) {
				return false;
			}

			std::list<CFestivalWishRequireData_Raw *>::iterator itrNext = itr;
			++itrNext;
			for ( ; itrNext != m_listRawFestivalWishInfo.end(); ++itrNext) {
				if (*itrNext != NULL) {
					if ((*itr)->m_nWishStartTimes > (*itrNext)->m_nWishStartTimes && (*itr)->m_nWishStartTimes < (*itrNext)->m_nWishEndTimes) {
						return false;
					}

					if ((*itr)->m_nWishEndTimes > (*itrNext)->m_nWishStartTimes && (*itr)->m_nWishEndTimes < (*itrNext)->m_nWishEndTimes) {
						return false;
					}
				}
			}
			int nMaxTimes = MAX((*itr)->m_nWishStartTimes, (*itr)->m_nWishEndTimes);
			m_nMaxWishTimes = MAX(m_nMaxWishTimes, nMaxTimes);
			_AddWishRequireData(*itr);
		}
	}
	
	CFestivalWishActivity * pActivity = new CFestivalWishActivity();
	pActivity->Init(m_nActivityStartTime, m_nActivityEndTime);
	m_bIsOpening = pActivity->IsOpening();
	CSystemActivityManager::Instance().AddTimerActivity(pActivity);

	return true;
}

bool CFestivalWishDataMgr::IsOpening()
{
	return m_bIsOpening;
}

void CFestivalWishDataMgr::SetIsOpening( bool bIsOpening )
{
	m_bIsOpening = bIsOpening;
}

unsigned int CFestivalWishDataMgr::ActivityStartTime()
{
	return m_nActivityStartTime;
}

unsigned int CFestivalWishDataMgr::ActivityEndTime()
{
	return m_nActivityEndTime;
}

int CFestivalWishDataMgr::MaxWishTimes()
{
	return m_nMaxWishTimes;
}

int CFestivalWishDataMgr::GetWishNeedItemCount(int nWishTimes)
{
	int nCount = 0;
	for (std::map<int, CFestivalWishRequireData *>::iterator itr = m_mapFestivalWishRequireInfo.begin(); itr != m_mapFestivalWishRequireInfo.end(); ++itr) {
		if (itr->second != NULL) {
			if (nWishTimes >= itr->second->m_nWishTimes) {
				nCount = itr->second->m_nIteCount;
			} else {
				break;
			}
		}
	}

	return nCount;
}

CFestivalWishRequireData * CFestivalWishDataMgr::GetWishRequirement(int nWishTimes)
{
	CFestivalWishRequireData * pRequirement = NULL;
	for (std::map<int, CFestivalWishRequireData *>::iterator itr = m_mapFestivalWishRequireInfo.begin(); itr != m_mapFestivalWishRequireInfo.end(); ++itr) {
		if (itr->second != NULL) {
			if (nWishTimes >= itr->second->m_nWishTimes) {
				pRequirement = itr->second;
			} else {
				break;
			}
		}
	}

	return pRequirement;
}

CFestivalWishRewardData * CFestivalWishDataMgr::GetWishReward(int nWishTimes)
{
	std::map<int, CFestivalWishRewardData *>::iterator itr = m_mapFestivalWishReward.find(nWishTimes);
	if (itr != m_mapFestivalWishReward.end() && itr->second != NULL) {
		return itr->second;
	}

	return NULL;
}

void CFestivalWishDataMgr::AddActivityData( unsigned int nActivityStartTime, unsigned int nActivityEndTime )
{
	m_nActivityStartTime = nActivityStartTime;
	m_nActivityEndTime = nActivityEndTime;
}


void CFestivalWishDataMgr::AddMakeWishRewardData_Raw( CFestivalWishRequireData_Raw *pRawData )
{
	if (pRawData != NULL) {
		m_listRawFestivalWishInfo.push_back(pRawData);
	}
}

void CFestivalWishDataMgr::AddRewardData(CFestivalWishRewardData *pRewardData)
{
	if (pRewardData != NULL) {
		m_mapFestivalWishReward.insert(make_pair(pRewardData->m_nTimes, pRewardData));
	}
}

void CFestivalWishDataMgr::AddMakeWishRewardData( CFestivalWishRewardData *pRewardInfo )
{
	if (pRewardInfo != NULL) {
		m_mapFestivalWishReward.insert(make_pair(pRewardInfo->m_nTimes, pRewardInfo));
	}
}

void CFestivalWishDataMgr::_AddWishRequireData( CFestivalWishRequireData_Raw *pRawData )
{
	CFestivalWishRequireData *pStartRequirData = new CFestivalWishRequireData;
	pStartRequirData->m_nWishTimes = pRawData->m_nWishStartTimes;
	pStartRequirData->m_nItemType = pRawData->m_nItemType;
	pStartRequirData->m_nIteCount = pRawData->m_nItemCount;
	m_mapFestivalWishRequireInfo.insert(make_pair(pStartRequirData->m_nWishTimes, pStartRequirData));

	CFestivalWishRequireData *pEndRequirData = new CFestivalWishRequireData;
	pEndRequirData->m_nWishTimes = pRawData->m_nWishStartTimes;
	pEndRequirData->m_nItemType = pRawData->m_nItemType;
	pEndRequirData->m_nIteCount = pRawData->m_nItemCount;
	m_mapFestivalWishRequireInfo.insert(make_pair(pEndRequirData->m_nWishTimes, pEndRequirData));
}



