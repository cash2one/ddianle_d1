#include "VIPDataMgr.h"
#include <limits.h>

CVIPDataMgr::CVIPDataMgr()
:m_nMaxLevel(0)
,m_nConsumedPtExpBounsRate(0)
,m_nSendAddVipExpMaxBill(0)
{

}

CVIPDataMgr::~CVIPDataMgr()
{
	for (VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.begin(); itr != m_mapVIPPrivInfo.end(); ++itr) {
		if (itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}
	
	for (std::map<int, CVIPExpInfo *>::iterator itr = m_mapVIPNeedLevelUpExp.begin(); itr != m_mapVIPNeedLevelUpExp.end(); ++itr) {
		if (itr != m_mapVIPNeedLevelUpExp.end() && itr->second != NULL) {
			delete itr->second, itr->second = NULL;
		}
	}

	m_mapVIPPrivInfo.clear();
	m_mapVIPNeedLevelUpExp.clear();
}

CVIPDataMgr & CVIPDataMgr::Instance()
{
	static CVIPDataMgr s_VIPDataMgr;

	return s_VIPDataMgr;
}

void CVIPDataMgr::AddVIPPrivInfo( CVIPLevelPrivInfo *pLevelUpReward )
{
	if (pLevelUpReward != NULL) {
		m_mapVIPPrivInfo.insert(make_pair(pLevelUpReward->m_nLevel, pLevelUpReward));
	}
}

void CVIPDataMgr::AddVIPExpInfo(CVIPExpInfo *pVIPExp)
{
	if (pVIPExp != NULL) {
		m_mapVIPNeedLevelUpExp.insert(make_pair(pVIPExp->m_nCurLevel, pVIPExp));
	}
}

void CVIPDataMgr::AddVIPInfo(int nMaxLevel, int nConsumedPtExpBounsRate, int nSendAddVipExpMaxBill)
{
	m_nMaxLevel = nMaxLevel;
	m_nConsumedPtExpBounsRate = nConsumedPtExpBounsRate;
    m_nSendAddVipExpMaxBill = nSendAddVipExpMaxBill;
}

int CVIPDataMgr::GetMaxLevel()
{
	return m_nMaxLevel;
}

void CVIPDataMgr::GetDailyExpBonus( int nLevel, int & nExp, int & nBindMBill, std::list<CItem> & listItem )
{
	VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
	if (itr != m_mapVIPPrivInfo.end()) 
    {
	    nExp = itr->second->m_nDailyExpBonus;
        nBindMBill = itr->second->m_nDailyBindMBill;
        listItem = itr->second->m_listDailyItemReward;
	}
}

int CVIPDataMgr::ConsumedPtExpBounsRate()
{
	return m_nConsumedPtExpBounsRate;
}

unsigned int CVIPDataMgr::GetExpLevelUpNeed(int nLevel)
{
	std::map<int, CVIPExpInfo *>::iterator itr = m_mapVIPNeedLevelUpExp.find(nLevel);
	if (itr != m_mapVIPNeedLevelUpExp.end() && itr->second != NULL) {
		return itr->second->m_nExpLevepUpNeed;
	}

	return INT_MAX;
}

const CVIPLevelPrivInfo *CVIPDataMgr::GetVIPPrivInfo(int nLevel)
{
    VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
    if (itr != m_mapVIPPrivInfo.end() && itr->second != NULL) 
    {
        return itr->second;
    }

    return NULL;
}

const VIPLevelPrivInfoContainer & CVIPDataMgr:: VIPLevelUpContainer()
{
	return m_mapVIPPrivInfo;
}

void CVIPDataMgr::Serialize( CParamPool & IOBuff, unsigned char nSex)
{
	IOBuff.AddUShort((unsigned short)m_nMaxLevel);
	IOBuff.AddUShort((unsigned short) m_nConsumedPtExpBounsRate);
	IOBuff.AddUShort((unsigned short)m_mapVIPPrivInfo.size());
	for (VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.begin(); itr != m_mapVIPPrivInfo.end(); ++itr) {
		if (itr->second != NULL) {
			itr->second->Serialize(IOBuff, nSex);
		}
	}
}

int CVIPDataMgr::GetBuyVitCount(int nLevel)
{
    VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
    if (itr != m_mapVIPPrivInfo.end() && itr->second != NULL) 
    {
        return itr->second->m_nVipBuyVitCount;
    }

    return 0;
}

int CVIPDataMgr::GetStorageCapacity(int nLevel)
{
    VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
    if (itr != m_mapVIPPrivInfo.end() && itr->second != NULL)
    {
        return itr->second->m_nStorageCapacity;
    }

    return 0;
}


int CVIPDataMgr::GetDanceGroupFairyLandExChance(int nLevel)
{
    VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
    if (itr != m_mapVIPPrivInfo.end() && itr->second != NULL)
    {
        return itr->second->m_nDanceGroupFairyLandExChance;
    }

    return 1; // 没有vip信息就是一次
}

int CVIPDataMgr::GetDanceGroupFairyLandVipBuf(int nLevel)
{
    VIPLevelPrivInfoContainer::iterator itr = m_mapVIPPrivInfo.find(nLevel);
    if (itr != m_mapVIPPrivInfo.end() && itr->second != NULL)
    {
        return itr->second->m_nDanceGroupFairyLandVipBuf;
    }

    return 0;
}