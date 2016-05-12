#include "PushRatingDataManager.h"

CPushRatingDataManager & CPushRatingDataManager::Instance()
{
	static CPushRatingDataManager s_instance;
	return s_instance;
}

CPushRatingDataManager::CPushRatingDataManager()
: m_bEnable(false)
, m_strPushMessage("")
, m_nPushInterval(0)
, m_nMaxCount(0)
, m_nResetWeekday(0)
, m_nOnlineTime(0)
, m_bEnableOnlineTimePush(false)
, m_bEnableFisrtDailyQuest(false)
, m_bEnableFisrtDailyShoping(false)
{

}
CPushRatingDataManager::~CPushRatingDataManager()
{

}
bool CPushRatingDataManager::IsEnabled()
{
	return m_bEnable;
}

int CPushRatingDataManager::Interval()
{
	return m_nPushInterval;
}

std::string & CPushRatingDataManager::Message()
{
	return m_strPushMessage;
}

void CPushRatingDataManager::SetPushRatingSystem(const char *pszPushMessage, int nInterval, bool enable, int nMaxCount, int nResetWeekday)
{
	m_bEnable = enable;
	m_strPushMessage = pszPushMessage;
	m_nPushInterval = nInterval;
	m_nMaxCount = nMaxCount;
	m_nResetWeekday = nResetWeekday;
}


bool CPushRatingDataManager::CanPushOnLevel(int nLevel) const
{
	return m_bEnable && m_levelSet.count(nLevel) > 0;
}

bool CPushRatingDataManager::CanPushOnlineTime(int nOnlineTime) const
{
	return m_bEnable && m_bEnableOnlineTimePush && nOnlineTime >= m_nOnlineTime;
}

bool CPushRatingDataManager::CanPushFirstDailyQuest() const
{
	return m_bEnable && m_bEnableFisrtDailyQuest;
}

bool CPushRatingDataManager::CanPushFirstDailyShoping() const
{
	return m_bEnable && m_bEnableFisrtDailyShoping;
}

bool CPushRatingDataManager::ExceedMaxCount(int nCount) const
{
	return !m_bEnable || nCount >= m_nMaxCount;
}

int CPushRatingDataManager::ResetWeekday() const
{
	return m_nResetWeekday;
}

void CPushRatingDataManager::AddPushLevel(int nLevel)
{
	m_levelSet.insert(nLevel);
}

void CPushRatingDataManager::SetPushOnlineTime(int nOnlineTime, bool enable)
{
	m_bEnableOnlineTimePush = enable;
	m_nOnlineTime = nOnlineTime;
}

void CPushRatingDataManager::SetPushFinishFirstDailyQuest(bool enable)
{
	m_bEnableFisrtDailyQuest = enable;
}

void CPushRatingDataManager::SetPushFinishFirstDailyShoping(bool enable)
{
	m_bEnableFisrtDailyShoping = enable;
}

