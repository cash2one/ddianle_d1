#include "AntiAddictionDataMgr.h"
#include <algorithm>

CAntiAddictionDataMgr::CAntiAddictionDataMgr()
{
	m_nOfflineMinutes = 0;
}

CAntiAddictionDataMgr::~CAntiAddictionDataMgr()
{

}

CAntiAddictionDataMgr& CAntiAddictionDataMgr::Instance()
{
	static CAntiAddictionDataMgr inst;
	return inst;
}

//排序子，按时间排序
struct SortPred
{
	bool operator()(const AntiAddictionConfigItem& left, const AntiAddictionConfigItem& right) const
	{
		return left.timeRange.first + left.timeRange.second < right.timeRange.first + right.timeRange.second;
	}
};

void CAntiAddictionDataMgr::AddAntiAddictionConfig( int nID, AntiAddictionConfigItem* pConfig )
{
	m_mapAntiAddictionConfig[nID].push_back(*pConfig);
	SortPred _pred;
	std::sort(m_mapAntiAddictionConfig[nID].begin(), m_mapAntiAddictionConfig[nID].end(), _pred);
}

void CAntiAddictionDataMgr::AddPlatformConfig( int nPlatformID, int nConfigID )
{
	m_mapPlatformConfig[nPlatformID] = nConfigID;
}

AntiAddictionConfigItem* CAntiAddictionDataMgr::FindConfig( int nPlatformID, int nOnlineTime, int& nIndex )
{
	std::map<int, int>::iterator it = m_mapPlatformConfig.find(nPlatformID);
	if (it == m_mapPlatformConfig.end())
	{
		return NULL;
	}
	std::map<int, std::vector<AntiAddictionConfigItem> >::iterator cit = m_mapAntiAddictionConfig.find(it->second);
	if (cit == m_mapAntiAddictionConfig.end())
	{
		return NULL;
	}
	int i = 0;
	for (std::vector<AntiAddictionConfigItem>::iterator itemit = cit->second.begin(); itemit != cit->second.end(); ++itemit)
	{
		if (nOnlineTime >= itemit->timeRange.first && nOnlineTime < itemit->timeRange.second)
		{
			nIndex = i;
			return &(*itemit);
		}
		++i;
	}
	return NULL;
}

AntiAddictionConfigItem* CAntiAddictionDataMgr::FindConfig( int nPlatformID, int nIndex )
{
	std::map<int, int>::iterator it = m_mapPlatformConfig.find(nPlatformID);
	if (it == m_mapPlatformConfig.end())
	{
		return NULL;
	}

	std::map<int, std::vector<AntiAddictionConfigItem> >::iterator cit = m_mapAntiAddictionConfig.find(it->second);
	if (cit == m_mapAntiAddictionConfig.end())
	{
		return NULL;
	}

	if (nIndex < (int)cit->second.size())
	{
		return &(cit->second[nIndex]);
	}
	return NULL;
}

void CAntiAddictionDataMgr::SetOpenAntiaddiction( bool bOpenAntiaddiction )
{
	m_bOpenAntiaddiction = bOpenAntiaddiction;
}

bool CAntiAddictionDataMgr::IsOpenAntiaddiction()
{
	return m_bOpenAntiaddiction;
}

bool CAntiAddictionDataMgr::HasConfig( int nPlatformID )
{
	std::map<int, int>::iterator it = m_mapPlatformConfig.find(nPlatformID);
	if (it != m_mapPlatformConfig.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}


