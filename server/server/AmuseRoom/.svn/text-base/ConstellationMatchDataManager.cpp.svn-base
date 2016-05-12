#include "ConstellationMatchDataManager.h"

#include "../util/CommonFunc.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include <stdlib.h>

ConstellationMatchDataManager::ConstellationMatchDataManager(void)
{
	memset(m_szMatchRate, 0, sizeof(m_szMatchRate));

	m_nRoleLevelRequired = 0;
	m_nPriorityCardCount = 0;
	m_nPriorityCardCoolDown = 0;
	m_nPriorityCardMaxUseTimes = 0;
	m_nRoomCloseTime = 0;
	m_nMatchInterval = 0;
}

ConstellationMatchDataManager::~ConstellationMatchDataManager(void)
{
}

ConstellationMatchDataManager & ConstellationMatchDataManager::Instance(void)
{
	static ConstellationMatchDataManager s_instance;
	return s_instance;
}

bool ConstellationMatchDataManager::IsOpen() const
{
	return LeftOpenTime() > 0;
}

int ConstellationMatchDataManager::LeftOpenTime() const
{
	time_t nNow = time(NULL);

	return _LeftOpenTime((unsigned int)nNow);
}

int ConstellationMatchDataManager::_LeftOpenTime(unsigned int nTime) const
{
	struct tm tmTime;
	time_t tTime = (time_t)nTime;
	LocalTime(&tTime, &tmTime);

	int nDayTime = tmTime.tm_hour * 3600 + tmTime.tm_min * 60 + tmTime.tm_sec;
	for (size_t i = 0; i < m_vecOpenTime.size(); i++)
	{
		if (m_vecOpenTime[i].m_nWeekday == tmTime.tm_wday
			&& nDayTime >= m_vecOpenTime[i].m_nStartTime
			&& nDayTime <= m_vecOpenTime[i].m_nEndTime)
		{
			return (m_vecOpenTime[i].m_nEndTime - nDayTime);
		}
	}
	return 0;
}

int ConstellationMatchDataManager::WillOpenTime() const
{
	struct tm tmTime;
	time_t tTime = time(NULL);
	LocalTime(&tTime, &tmTime);
	int nDayTime = tmTime.tm_hour * 3600 + tmTime.tm_min * 60 + tmTime.tm_sec;

	size_t i = 0;
	for (; i < m_vecOpenTime.size(); i++)
	{
		if (m_vecOpenTime[i].m_nWeekday == tmTime.tm_wday)
		{
			if (nDayTime >= m_vecOpenTime[i].m_nStartTime
				&& nDayTime <= m_vecOpenTime[i].m_nEndTime)
			{
				return 0;
			}
			else if (nDayTime < m_vecOpenTime[i].m_nStartTime
				&& nDayTime < m_vecOpenTime[i].m_nEndTime)
			{
				return (m_vecOpenTime[i].m_nStartTime - nDayTime);
			}
		}
	}

	int nTomorrowWeekday = (tmTime.tm_wday + 1) % 7;
	for (i = 0; i < m_vecOpenTime.size(); i++)
	{
		if (m_vecOpenTime[i].m_nWeekday == nTomorrowWeekday)
		{
			return 24 * 3600 - nDayTime + m_vecOpenTime[i].m_nStartTime;
		}
	}

	return -1;
}

bool  ConstellationMatchDataManager::IsInCurrentOpenTime(unsigned int nTime, unsigned int nNow) const
{
	if (abs((int)(nNow - nTime)) >= 3600 * 24) // not same day
	{
		return false;
	}

	time_t tTime = nTime, tNow = nNow;
	struct tm tmTime, tmNow;
	LocalTime(&tTime, &tmTime);
	LocalTime(&tNow, &tmNow);

	int nDayTime = tmTime.tm_hour * 3600 + tmTime.tm_min * 60 + tmTime.tm_sec;
	int nNowDayTime = tmNow.tm_hour * 3600 + tmNow.tm_min * 60 + tmNow.tm_sec;

	for (size_t i = 0; i < m_vecOpenTime.size(); i++)
	{
		if (m_vecOpenTime[i].m_nWeekday == tmTime.tm_wday
			&& m_vecOpenTime[i].m_nWeekday == tmNow.tm_wday
			&& nDayTime >= m_vecOpenTime[i].m_nStartTime
			&& nDayTime <= m_vecOpenTime[i].m_nEndTime
			&& nNowDayTime >= m_vecOpenTime[i].m_nStartTime
			&& nNowDayTime <= m_vecOpenTime[i].m_nEndTime)
		{
			return true;
		}
	}
	return false;
}

const std::vector<int> *ConstellationMatchDataManager::GetMatchConstellation(int nConstellation) const
{
	if (nConstellation >= 0 && nConstellation < MAX_CONSTELLATION_COUNT)
	{
		return &m_vecMatchTable[nConstellation];
	}

	return NULL;
}

void ConstellationMatchDataManager::RandomMatchConstellation(int nConstellation, std::vector<int> &vecMatchConstellation) const
{
	if (nConstellation < 0 || nConstellation >= MAX_CONSTELLATION_COUNT)
	{
		return;
	}

	vecMatchConstellation = m_vecMatchTable[nConstellation];
	while ((int)vecMatchConstellation.size() < m_nPriorityCardCount)
	{
		vecMatchConstellation.push_back(MAX_CONSTELLATION_COUNT);
	}
	while ((int)vecMatchConstellation.size() > m_nPriorityCardCount)
	{
		int nIndex = Random(0, (int)m_vecMatchTable[nConstellation].size() - 1);
		vecMatchConstellation.erase(vecMatchConstellation.begin() + nIndex);
	}
}

int ConstellationMatchDataManager::GetMatchRate(int nConstellation, int nOtherConstellation) const
{
	if (nConstellation >= 0 && nOtherConstellation >= 0
		&& nConstellation < MAX_CONSTELLATION_COUNT
		&& nOtherConstellation < MAX_CONSTELLATION_COUNT)
	{
		return m_szMatchRate[nConstellation][nOtherConstellation];
	}

	return 0;
}

const char *ConstellationMatchDataManager::RandomWishWords() const
{
	if (m_vecWishWords.empty())
	{
		return "Best Wishes";
	}

	return m_vecWishWords[Random(0, (int)m_vecWishWords.size() - 1)].c_str();
}

int ConstellationMatchDataManager::RandomAmuseScene() const
{
	if (m_vecSceneID.empty())
	{
		return ConfigManager::Instance().GetAmuseMgr().RandomAmuseScene();
	}

	int nSceneID = m_vecSceneID[Random(0, (int)m_vecSceneID.size() - 1)];
	if (ConfigManager::Instance().GetAmuseMgr().ExistAmuseScene(nSceneID))
	{
		return nSceneID;
	}

	return ConfigManager::Instance().GetAmuseMgr().RandomAmuseScene();
}

void ConstellationMatchDataManager::Init()
{
	memcpy(m_szMatchRate, ConfigManager::Instance().GetConstellationMgr().MatchRateList(), sizeof(m_szMatchRate));
	m_vecWishWords = ConfigManager::Instance().GetConstellationMgr().WishWordsList();
}

void ConstellationMatchDataManager::SetConstellationMatch(int nLevel, const char *strFisrtEnterTip, int nPriorityCardCount, int nPriorityCardCoolDown, int nMaxUseTimes, int nRoomCloseTime, int nMatchInterval, const char *strRule)
{
	m_nRoleLevelRequired = nLevel;
	m_strFirstEnterTip = strFisrtEnterTip;
	m_nPriorityCardCount = nPriorityCardCount;
	m_nPriorityCardCoolDown = nPriorityCardCoolDown;
	m_nPriorityCardMaxUseTimes = nMaxUseTimes;
	m_nRoomCloseTime = nRoomCloseTime;
	m_nMatchInterval = nMatchInterval;
	m_strRule = strRule;
}

void ConstellationMatchDataManager::AddOpenTime(int nWeekday, const char *strStartTime, const char *strEndTime)
{
	if (nWeekday >= 0 && nWeekday < 7)
	{
		std::string strTimeBuff;
		struct tm tmBuff;

		strTimeBuff.assign("2014-08-01 "), strTimeBuff.append(strStartTime);
		time_t nStartTime = SetTime(strTimeBuff.c_str());
		LocalTime(&nStartTime, &tmBuff);
		nStartTime = tmBuff.tm_hour * 3600 + tmBuff.tm_min * 60 + tmBuff.tm_sec;

		strTimeBuff.assign("2014-08-01 "), strTimeBuff.append(strEndTime);
		time_t nEndTime = SetTime(strTimeBuff.c_str());
		LocalTime(&nEndTime, &tmBuff);
		nEndTime = tmBuff.tm_hour * 3600 + tmBuff.tm_min * 60 + tmBuff.tm_sec;

		SOpenTimePair openTime(nWeekday, (int)nStartTime, (int)nEndTime);
		m_vecOpenTime.push_back(openTime);
	}
}


void ConstellationMatchDataManager::AddMatchTable(int nFisrtConstellation, int nSecondConstellation)
{
	if (nFisrtConstellation >= 0 && nSecondConstellation >= 0
		&& nFisrtConstellation < MAX_CONSTELLATION_COUNT
		&& nSecondConstellation < MAX_CONSTELLATION_COUNT)
	{
		m_vecMatchTable[nFisrtConstellation].push_back(nSecondConstellation);
	}
}

void ConstellationMatchDataManager::AddWishWords(const char *strWords)
{
	std::string words = strWords;
	m_vecWishWords.push_back(words);
}

void ConstellationMatchDataManager::AddMatchRate(int nFisrtConstellation, int nSecondConstellation, int nRate)
{
	if (nFisrtConstellation >= 0 && nSecondConstellation >= 0
		&& nFisrtConstellation < MAX_CONSTELLATION_COUNT
		&& nSecondConstellation < MAX_CONSTELLATION_COUNT)
	{
		m_szMatchRate[nFisrtConstellation][nSecondConstellation] = nRate;
	}
}

void ConstellationMatchDataManager::AddAmuseSceneID(int nSceneID)
{
	m_vecSceneID.push_back(nSceneID);
}

