#include "TimerActivity.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"

const unsigned int c_DAY_SECONDS = 86400;

//----------------------------------- CTimeActivity
CTimeActivity::CTimeActivity()
{
	m_tBeginTime = 0;
	m_tEndTime = 0;
	m_bIsOpening = false;
	m_bValid = false;
}
CTimeActivity::~CTimeActivity()
{
}
//初始化，初始化后才是有效的
void CTimeActivity::Init(time_t tBeginTime, time_t tEndTime)
{
	m_bValid = true;
	time_t tNow = time(NULL);
	m_tBeginTime = tBeginTime;
	m_tEndTime = tEndTime;
	_SetOpening(tNow >= m_tBeginTime && tNow <= m_tEndTime);
}
void CTimeActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	//检查活动开启结束状态
	_CheckActivityState(tNow);
}
//当活动开启
void CTimeActivity::OnActivityBegin()
{
}
//当活动结束
void CTimeActivity::OnActivityEnd()
{
}
//检查活动状态
void CTimeActivity::_CheckActivityState(time_t tNow)
{
	if (m_bIsOpening != (tNow >= m_tBeginTime && tNow <= m_tEndTime))
	{
		_SetOpening(!m_bIsOpening);
	}
}
//设置活动开启状态
void CTimeActivity::_SetOpening(bool bIsOpening)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CTimeActivity::_SetOpening. Old:(%d), New:(%d)", m_bIsOpening ? 1 : 0, bIsOpening ? 1 : 0);
	m_bIsOpening = bIsOpening;
	if (m_bIsOpening)
	{
		OnActivityBegin();
		m_bBegun = true;
	}
	else
	{
		OnActivityEnd();
		m_bBegun = false;
	}
}
//----------------------------------- CDayActivity
CDayActivity::CDayActivity()
{
	m_nHour = 0;
	m_nMinute = 0;
	m_tLastRefreshTime = 0;
	m_tBeginTime = 0;
	m_tEndTime = 0;
	m_bIsOpening = false;
	m_bValid = false;
}
CDayActivity::~CDayActivity()
{
}
//初始化，初始化后才是有效的
void CDayActivity::Init(unsigned int nHour, unsigned int nMinute, time_t tBeginTime, time_t tEndTime)
{
	m_bValid = true;
	time_t tNow = time(NULL);
	m_nHour = (unsigned char)(nHour % 24);
	m_nMinute = (unsigned char)(nMinute % 60);
	_SetLastRefreshTime(tNow);
	m_tBeginTime = tBeginTime;
	m_tEndTime = tEndTime;
	_SetOpening(tNow >= m_tBeginTime && tNow <= m_tEndTime);
}
void CDayActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	//检查活动开启结束状态
	_CheckActivityState(tNow);
	if (m_bIsOpening)
	{
		//检查触发情况
		_CheckTriggering(lTimeElapsed, tNow);
	}
}
void CDayActivity::OnTime(const unsigned long &lTimeElapsed, time_t tNow)
{

}
//当活动开启
void CDayActivity::OnActivityBegin()
{
}
//当活动结束
void CDayActivity::OnActivityEnd()
{
}
//检查触发情况
void CDayActivity::_CheckTriggering(const unsigned long &lTimeElapsed, time_t tNow)
{
	if (tNow > (time_t)(m_tLastRefreshTime + c_DAY_SECONDS) || tNow < m_tLastRefreshTime)
	{
		_SetLastRefreshTime(tNow);
		OnTime(lTimeElapsed, tNow);
	}
}
//检查活动状态
void CDayActivity::_CheckActivityState(time_t tNow)
{
	if (m_bIsOpening != (tNow >= m_tBeginTime && tNow <= m_tEndTime))
	{
		_SetOpening(!m_bIsOpening);
		//临界点都刷新时间
		_SetLastRefreshTime(tNow);
	}
}
//根据当前时间设置刷新时间
void CDayActivity::_SetLastRefreshTime(time_t tNow)
{
	m_tLastRefreshTime = SetTime(GetYear(tNow), GetMonth(tNow), GetMonthDay(tNow), m_nHour, m_nMinute, 0);
	if (tNow < m_tLastRefreshTime)
	{
		m_tLastRefreshTime -= c_DAY_SECONDS;
	}
	WriteLog(LOGLEVEL_DEBUG, "Debug: CDayActivity::_SetLastRefreshTime. tNow=(%u), LastRefreshTime(%u), Hour(%d), Minute(%d)",
		(unsigned int)tNow, (unsigned int)m_tLastRefreshTime, (int)m_nHour, (int)m_nMinute);
}
//设置活动开启状态
void CDayActivity::_SetOpening(bool bIsOpening)
{
	WriteLog(LOGLEVEL_DEBUG, "Debug: CDayActivity::_SetOpening. Old:(%d), New:(%d)", m_bIsOpening ? 1 : 0, bIsOpening ? 1 : 0);
	m_bIsOpening = bIsOpening;
	if (m_bIsOpening)
	{
		OnActivityBegin();
		m_bBegun = true;
	}
	else
	{
		OnActivityEnd();
		m_bBegun = false;
	}
}

//----------------------------------- CDayTimeBlockActivity
CDayTimeBlockActivity::CDayTimeBlockActivity()
{
	m_bIsOpening = false;
	m_bValid = false;
	m_bIsRunning = false;
	m_nStartHour = 0;
	m_nStartMinute = 0;
	m_nStopHour = 0;
	m_nStopMinute = 0;
	m_nStartMinutes = 0;
	m_nStopMinutes = 0;
	m_tBeginTime = 0;
	m_tEndTime = 0;
}
CDayTimeBlockActivity::~CDayTimeBlockActivity()
{
}
//初始化，初始化后才是有效的
void CDayTimeBlockActivity::Init(unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute, time_t tBeginTime, time_t tEndTime)
{
	m_bValid = true;
	time_t tNow = time(NULL);
	m_nStartHour = (unsigned char)(nStartHour % 24);
	m_nStartMinute = (unsigned char)(nStartMinute % 60);
	m_nStopHour = (unsigned char)(nStopHour % 24);
	m_nStopMinute = (unsigned char)(nStopMinute % 60);
	m_nStartMinutes = m_nStartHour * 60 + m_nStartMinute;
	m_nStopMinutes = m_nStopHour * 60 + m_nStopMinute;
	if (m_nStartMinutes > m_nStopMinutes)
	{
		m_nStopHour += 24;
		m_nStopMinutes += 1440;
	}
	m_tBeginTime = tBeginTime;
	m_tEndTime = tEndTime;
	_SetOpening(tNow >= m_tBeginTime && tNow <= m_tEndTime);
	int nNowMinutes = GetHour(tNow) * 60 + GetMinute(tNow);
	if (m_nStartMinutes > nNowMinutes)
	{
		nNowMinutes += 1440;
	}
	_SetRunning(m_bIsOpening && nNowMinutes >= m_nStartMinutes && nNowMinutes < m_nStopMinutes);
}
void CDayTimeBlockActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	//检查活动开启结束状态
	_CheckActivityState(tNow);
	if (m_bIsOpening)
	{
		//检查活动运营状态
		_CheckActivityRunningState(tNow);
	}
}
//当活动开启
void CDayTimeBlockActivity::OnActivityBegin()
{
}
//当活动结束
void CDayTimeBlockActivity::OnActivityEnd()
{
}
//活动开始运营
void CDayTimeBlockActivity::OnStart()
{
}
//活动结束运营
void CDayTimeBlockActivity::OnStop()
{
}
//检查活动运营状态
void CDayTimeBlockActivity::_CheckActivityRunningState(time_t tNow)
{
	int nNowMinutes = GetHour(tNow) * 60 + GetMinute(tNow);
	if (m_nStartMinutes > nNowMinutes)
	{
		nNowMinutes += 1440;
	}
	if (m_bIsRunning != (nNowMinutes >= m_nStartMinutes && nNowMinutes < m_nStopMinutes))
	{
		_SetRunning(!m_bIsRunning);
	}
}
//检查活动状态
void CDayTimeBlockActivity::_CheckActivityState(time_t tNow)
{
	if (m_bIsOpening != (tNow >= m_tBeginTime && tNow <= m_tEndTime))
	{
		_SetOpening(!m_bIsOpening);
		//临界点都设为false
		_SetRunning(false);
	}
}
//设置活动开启状态
void CDayTimeBlockActivity::_SetOpening(bool bIsOpening)
{
	if (m_bIsOpening != bIsOpening)
	{
		WriteLog(LOGLEVEL_DEBUG, "Debug: CDayTimeBlockActivity::_SetOpening. Old:(%d), New:(%d)", m_bIsOpening ? 1 : 0, bIsOpening ? 1 : 0);
		m_bIsOpening = bIsOpening;
		if (m_bIsOpening)
		{
			OnActivityBegin();
			m_bBegun = true;
		}
		else
		{
			OnActivityEnd();
			m_bBegun = false;
		}
	}
}
//设置活动运营状态
void CDayTimeBlockActivity::_SetRunning(bool bIsRunning)
{
	if (m_bIsRunning != bIsRunning)
	{
		WriteLog(LOGLEVEL_DEBUG, "Debug: CDayTimeBlockActivity::_SetRunning. Old:(%d), New:(%d)", m_bIsRunning ? 1 : 0, bIsRunning ? 1 : 0);
		m_bIsRunning = bIsRunning;
		if (m_bIsRunning)
		{
			OnStart();
		}
		else
		{
			OnStop();
		}
	}
}
CWeekDayTimeBlockActivity::CWeekDayTimeBlockActivity()
{

}
CWeekDayTimeBlockActivity::~CWeekDayTimeBlockActivity()
{

}

//初始化，初始化后才是有效的
void CWeekDayTimeBlockActivity::Init(unsigned char nWeekDay, unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute,
									 time_t tBeginTime, time_t tEndTime)
{
	m_nWeekDay = nWeekDay % 7;
	CDayTimeBlockActivity::Init(nStartHour, nStartMinute, nStopHour, nStopMinute, tBeginTime, tEndTime);
	time_t tNow = time(NULL);
	_SetRunning(m_bIsRunning && (m_nWeekDay == GetWeekDay(tNow)));
}
//检查活动运营状态
void CWeekDayTimeBlockActivity::_CheckActivityRunningState(time_t tNow)
{
	int nNowMinutes = GetHour(tNow) * 60 + GetMinute(tNow);
	if (m_nStartMinutes > nNowMinutes)
	{
		nNowMinutes += 1440;
	}
	if (m_bIsRunning != (nNowMinutes >= m_nStartMinutes && nNowMinutes < m_nStopMinutes && m_nWeekDay == GetWeekDay(tNow)))
	{
		_SetRunning(!m_bIsRunning);
	}
}
//----------------------------------- CCrontabActivity
CCrontabActivity::CCrontabActivity()
{
	m_bIsOpening = false;
	m_bValid = false;
}
CCrontabActivity::~CCrontabActivity()
{
}
//初始化，初始化后才是有效的
void CCrontabActivity::Init(const char* strBeginTime, const char* strEndTime)
{
	m_bValid = true;
	time_t tNow = time(NULL);
	m_beginTime.AssignFormat(strBeginTime, " |");
	m_endTime.AssignFormat(strEndTime, " |");
	_SetOpening(CCrontab::IsBetween(m_beginTime, m_endTime, tNow, false, false));
}
void CCrontabActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
	//检查活动开启结束状态
	_CheckActivityState(tNow);
}
//当活动开启
void CCrontabActivity::OnActivityBegin()
{
}
//当活动结束
void CCrontabActivity::OnActivityEnd()
{
}
//检查活动状态
void CCrontabActivity::_CheckActivityState(time_t tNow)
{
	if (m_bIsOpening != CCrontab::IsBetween(m_beginTime, m_endTime, tNow, false, false))
	{
		_SetOpening(!m_bIsOpening);
	}
}
//设置活动开启状态
void CCrontabActivity::_SetOpening(bool bIsOpening)
{
	WriteLog("Debug: CCrontabActivity::_SetOpening. Old:(%d), New:(%d)", m_bIsOpening ? 1 : 0, bIsOpening ? 1 : 0);
	m_bIsOpening = bIsOpening;
	if (m_bIsOpening)
	{
		OnActivityBegin();
		m_bBegun = true;
	}
	else
	{
		OnActivityEnd();
		m_bBegun = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 月、周、日活动
//////////////////////////////////////////////////////////////////////////
CMonthWeekDayActivity::CMonthWeekDayActivity()
{
    m_bValid = false;
    m_bIsOpening = false;
    m_bIsShowing = false;
    m_nBeginTime = 0;
    m_nEndTime = 0;
    m_nBeginShowTime = 0;
    m_nEndShowTime = 0;
}

CMonthWeekDayActivity::~CMonthWeekDayActivity()
{
}

void CMonthWeekDayActivity::Init(time_t beginTime, time_t endTime, time_t beginShowTime, time_t endShowTime)
{
    if (beginTime >= endTime)
    {
        LOG_ERROR << "CMonthWeekDayActivity::Init() error: BeginTime: "
                  << (unsigned int)beginTime << ", EndTime: " << (unsigned int)endTime << endl;

        return;
    }

    time_t now = time(NULL);

    if (beginShowTime <= 0)
        beginShowTime = now;

    if (endShowTime <= 0)
        endShowTime = endTime;

    if (beginShowTime > beginTime)
        beginShowTime = beginTime;

    if (endShowTime < endTime)
        endShowTime = endTime;

    m_bValid = true;
    m_nBeginTime = beginTime;
    m_nEndTime = endTime;
    m_nBeginShowTime = beginShowTime;
    m_nEndShowTime = endShowTime;

    SetOpening(IsInTimeRange(now));
}

void CMonthWeekDayActivity::OnUpdate(const unsigned long &lTimeElapsed, time_t tNow)
{
    if (m_bIsOpening != IsInTimeRange(tNow))
        SetOpening(!m_bIsOpening);
}

void CMonthWeekDayActivity::OnActivityBegin()
{
}

void CMonthWeekDayActivity::OnActivityEnd()
{
}

void CMonthWeekDayActivity::AddTimeRangeInfo(time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour)
{
    if (end <= begin)
        return;

    if (begin < m_nBeginTime)
        begin = m_nBeginTime;
    if (end > m_nEndTime)
        end = m_nEndTime;

    int idx = m_cTimeRanges.FindRange(begin, end);

    if (-1 == idx)
    {
        if (begin >= m_nEndTime)
            LOG_ERROR << "CMonthWeekDayActivity::AddTimeRangeInfo() error: BeginTime: "
                      << (unsigned int)begin << ", EndTime: " << (unsigned int)end 
                      << ", activityEndTime: " << (unsigned int)m_nEndTime << endl;
        else
            m_cTimeRanges.AddRangeInfo(begin, end, openType, rListDate, beginHour, endHour);
    }
    else if (idx >= 0)
        m_cTimeRanges.AddRangeInfo((unsigned int)idx, beginHour, endHour);
}

bool CMonthWeekDayActivity::IsOpenNow()
{
    return IsInTimeRange(time(NULL));
}

bool CMonthWeekDayActivity::CanParticipateAgain(time_t lastTime) const
{
    return !m_cTimeRanges.IsInSameOpenRange(lastTime, time(NULL));
}

void CMonthWeekDayActivity::SetOpening(bool bOpening)
{
    LOG_DEBUG << "CMonthWeekDayActivity::SetOpening(). Old:("
              << (m_bIsOpening ? "true" : "false")
              << "), New:("
              << (bOpening ? "true" : "false")
              << ")" << endl;

    m_bIsOpening = bOpening;

    if (m_bIsOpening)
    {
        OnActivityBegin();
        m_bBegun = true;
    }
    else
    {
        OnActivityEnd();
        m_bBegun = false;
    }
}

bool CMonthWeekDayActivity::IsInTimeRange(time_t tTime)
{
    if (m_nBeginShowTime <= tTime && tTime <= m_nEndShowTime)
        m_bIsShowing = true;
    else
        m_bIsShowing = false;

    if (m_nBeginTime <= tTime && tTime <= m_nEndTime)
        return m_cTimeRanges.IsInTimeRange(tTime);

    return false;
}


