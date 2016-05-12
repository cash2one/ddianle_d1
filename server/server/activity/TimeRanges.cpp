#include "TimeRanges.h"
#include "../util/CommonFunc.h"
#include "../../socket/Log.h"

//////////////////////////////////////////////////////////////////////////

time_t Format(int hour, int minute, int second)
{
    if (hour < 0 || hour > 23 || 
        minute < 0 || minute > 59 || 
        second < 0 || second > 59)
        return 0;

    // 60 == 111100
    return (((hour << 6) + minute) << 6) + second;
}

time_t Format(std::string &strHour)
{
    int hour = 0, minute = 0, second = 0;

    sscanf(strHour.c_str(), "%2d:%2d:%2d", &hour, &minute, &second);

    return Format(hour, minute, second);
}

//////////////////////////////////////////////////////////////////////////

typedef std::list<unsigned int>::const_iterator   CItr;

COpenTime::COpenTime(void)
{
    m_nType = OpenTimeType_None;
    memset(m_bufDate, 0, sizeof(m_bufDate));
}

bool COpenTime::Init(OpenTimeType type, const std::list<unsigned int> &rDate, std::string &beginHour, std::string &endHour)
{
    unsigned int size = sizeof(m_bufDate) / sizeof(unsigned char);

    if (OpenTimeType_Month == type)
    {
        // do nothing
    }
    else if (OpenTimeType_Week == type)
    {
        size = 7;
    }
    else if (OpenTimeType_Day == type)
    {
        // do nothing
    }
    else
        type = OpenTimeType_None;

    m_nType = type;

    if (OpenTimeType_None != m_nType)
    {
        if (OpenTimeType_Day != m_nType)
        {
            bool bHasDate = false;
            for (CItr bItr = rDate.begin(), eItr = rDate.end(); bItr != eItr; ++bItr)
            {
                if ((*bItr <= size) && (0 == m_bufDate[*bItr - 1]))
                {
                    m_bufDate[*bItr - 1] = 1;
                    bHasDate = true;
                }
            }

            if (! bHasDate)
                m_nType = OpenTimeType_Day;
        }

        return AddHourRange(beginHour, endHour);
    }

    return false;
}

bool COpenTime::AddHourRange(std::string &beginHour, std::string &endHour)
{
    time_t nBeginHour = Format(beginHour);
    time_t nEndHour = Format(endHour);

    if (nEndHour <= nBeginHour)
    {
        LOG_ERROR << "COpenTime::AddHourRange() error: BeginHour: " 
                  << beginHour << ", EndHour: " << endHour << endl;

        return false;
    }

    TimeRange range;
    range.m_tBegin = nBeginHour;
    range.m_tEnd = nEndHour;

    if (0 == m_vectHourRanges.size())
    {
        m_vectHourRanges.push_back(range);

        return true;
    }

    for (unsigned int i = 0, size = m_vectHourRanges.size(); 
        i < size; ++i)
    {
        if (range.m_tEnd < m_vectHourRanges[i].m_tBegin)
        {
            if (0 == i)
            {
                m_vectHourRanges.insert(m_vectHourRanges.begin(), range);

                return true;
            }
            else if (range.m_tBegin > m_vectHourRanges[i - 1].m_tEnd)
            {
                m_vectHourRanges.insert(m_vectHourRanges.begin() + i, range);

                return true;
            }
            else
            {
                LOG_ERROR << "CRanges::AddHourRange() error: BeginHour: " 
                          << beginHour << ", EndHour: " << endHour << endl;

                return false;
            }
        }
    }

    if (range.m_tBegin > m_vectHourRanges.rbegin()->m_tEnd)
    {
        m_vectHourRanges.push_back(range);

        return true;
    }
    else
    {
        LOG_ERROR << "CRanges::AddHourRange() error: BeginHour: " 
                  << beginHour << ", EndHour: " << endHour << endl;

        return false;
    }
}

bool COpenTime::IsInOpenTime(time_t tTime) const
{
    bool bRet = false;

    struct tm tmTime;
    LocalTime(&tTime, &tmTime);

    bRet = IsOpenToday(tmTime);

    if (!bRet)
        return bRet;

    time_t hour = Format(tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

    for (unsigned int i = 0, size = m_vectHourRanges.size(); i < size; ++i)
    {
        if (hour < m_vectHourRanges[i].m_tBegin)
            return false;
        else if (hour < m_vectHourRanges[i].m_tEnd)
            return true;
    }

    return false;
}

bool COpenTime::IsInSameHourRange(time_t tTime1, time_t tTime2) const
{
    int nIdx1 = FindHourRange(tTime1);
    int nIdx2 = FindHourRange(tTime2);

    if (-1 == nIdx1 || -1 == nIdx2 || nIdx1 != nIdx2)
        return false;
    else
        return true;
}

bool COpenTime::IsOpenToday(const struct tm &tmTime) const
{
    bool bRet = false;

    if (OpenTimeType_Day == m_nType)
        bRet = true;
    else if (OpenTimeType_None != m_nType)
    {
        if (OpenTimeType_Month == m_nType)
        {
            int size = sizeof(m_bufDate) / sizeof(unsigned char);

            if (0 != m_bufDate[size - 1] && CheckMonthEnd(tmTime))
                bRet = true;
            else if (tmTime.tm_mday < size && 0 != m_bufDate[tmTime.tm_mday - 1])
                bRet = true;
        }
        else if (OpenTimeType_Week == m_nType)
        {
            int weekday = tmTime.tm_wday;

            if (0 == weekday)
                weekday = 7;

            if (0 != m_bufDate[weekday - 1])
                bRet = true;
        }
    }

    return bRet;
}

bool COpenTime::CheckMonthEnd(const struct tm &tmTime) const
{
    switch (tmTime.tm_mon)
    {
    case 1:// February
        {
            int nYear = tmTime.tm_year + 1900;

            if ((0 == nYear % 400) || 
                (0 == nYear % 4 && 0 != nYear % 100))
            {
                if (29 == tmTime.tm_mday)
                    return true;
            }
            else if (28 == tmTime.tm_mday)
                return true;
        }
        break;

    case  3:// April
    case  5:// June
    case  8:// September
    case 10:// November
        {
            if (30 == tmTime.tm_mday)
                return true;
        }
        break;

    default:
        {
            if (31 == tmTime.tm_mday)
                return true;
        }
        break;
    }

    return false;
}

int COpenTime::FindHourRange(time_t tTime) const
{
    struct tm tmTime;
    LocalTime(&tTime, &tmTime);

    time_t hour = Format(tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

    for (unsigned int i = 0, size = m_vectHourRanges.size(); i < size; ++i)
    {
        if (hour < m_vectHourRanges[i].m_tBegin)
            return -1;
        else if (hour < m_vectHourRanges[i].m_tEnd)
            return (int)i;
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////////

CTimeRanges::CTimeRanges()
{
    m_vectTimes.clear();
    m_vectOpenTimes.clear();
}

void CTimeRanges::AddRangeInfo(time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour)
{
    TimeRange range;
    COpenTime openTime;

    range.m_tBegin = begin;
    range.m_tEnd = end;

    if (openTime.Init((OpenTimeType)openType, rListDate, beginHour, endHour))
    {
        if (0 == m_vectTimes.size())
        {
            m_vectTimes.push_back(range);
            m_vectOpenTimes.push_back(openTime);

            return;
        }

        for (unsigned int i = 0, size = m_vectTimes.size(); 
             i < size; ++i)
        {
            if (range.m_tEnd < m_vectTimes[i].m_tBegin)
            {
                if (0 == i)
                {
                    m_vectTimes.insert(m_vectTimes.begin(), range);
                    m_vectOpenTimes.insert(m_vectOpenTimes.begin(), openTime);
                }
                else if (range.m_tBegin > m_vectTimes[i - 1].m_tEnd)
                {
                    m_vectTimes.insert(m_vectTimes.begin() + i, range);
                    m_vectOpenTimes.insert(m_vectOpenTimes.begin() + i, openTime);
                }
                else
                {
                    LOG_ERROR << "CTimeRanges::AddRangeInfo() error: BeginTime: " 
                              << (unsigned int)begin << ", EndTime: " << (unsigned int)end << endl;
                }

                return;
            }
        }

        if (range.m_tBegin > m_vectTimes.rbegin()->m_tEnd)
        {
            m_vectTimes.push_back(range);
            m_vectOpenTimes.push_back(openTime);
        }
        else
        {
            LOG_ERROR << "CTimeRanges::AddRangeInfo() error: BeginTime: " 
                      << (unsigned int)begin << ", EndTime: " << (unsigned int)end << endl;
        }
    }
}

void CTimeRanges::AddRangeInfo(unsigned int nIdx, std::string &beginHour, std::string &endHour)
{
    if (nIdx >= m_vectOpenTimes.size())
        return;

    m_vectOpenTimes[nIdx].AddHourRange(beginHour, endHour);
}

bool CTimeRanges::IsInTimeRange(time_t tTime) const
{
    unsigned int size = m_vectTimes.size();
    if (size > m_vectOpenTimes.size())
        size = m_vectOpenTimes.size();

    for (unsigned int i = 0; i < size; ++i)
    {
        if (m_vectTimes[i].m_tBegin <= tTime && tTime <= m_vectTimes[i].m_tEnd)
        {
            return m_vectOpenTimes[i].IsInOpenTime(tTime);
        }
    }

    return false;
}

int CTimeRanges::FindRange(time_t begin, time_t end) const
{
    int nIdx = -1;

    for (unsigned int i = 0, size = m_vectTimes.size(); i < size; ++i)
    {
        if (m_vectTimes[i].m_tBegin == begin && m_vectTimes[i].m_tEnd == end)
            return (int)i;
        else if (begin >= m_vectTimes[i].m_tEnd)
            break;
    }

    return nIdx;
}

bool CTimeRanges::IsInSameOpenRange(time_t tTime1, time_t tTime2) const
{
    if (!IsSameDay(tTime1, tTime2))
        return false;

    for (unsigned int i = 0, size = m_vectTimes.size(); i < size; ++i)
    {
        if (tTime1 < m_vectTimes[i].m_tBegin)
            break;
        else if (tTime1 <= m_vectTimes[i].m_tEnd)
        {
            return m_vectOpenTimes[i].IsInSameHourRange(tTime1, tTime2);
        }
    }

    return false;
}

