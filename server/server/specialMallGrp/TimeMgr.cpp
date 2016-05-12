#include "TimeMgr.h"
#include "../../socket/Log.h"

CTimeMgr::CTimeMgr(void)
{
    m_bIsOpening = false;
    m_uRange = 1;
    m_vectTimes.clear();
}

CTimeMgr::~CTimeMgr(void)
{
}

void CTimeMgr::AddTimeRangeInfo(time_t begin, time_t end, std::string &rExtend)
{
    if (begin >= end)
    {
        LOG_ERROR << "Time error: begin time: " 
                  << (unsigned int)begin 
                  << ", end time: " 
                  << (unsigned int)end 
                  << std::endl;

        return;
    }

    TimeRange range;
    range.m_tBegin = begin;
    range.m_tEnd = end;
    range.m_strExtend = rExtend;

    if (0 == m_vectTimes.size())
    {
        m_vectTimes.push_back(range);

        m_uRange = CheckCurRange(time(NULL)) + 1;

        return;
    }

    for (unsigned int i = 0, size = m_vectTimes.size(); i < size; ++i)
    {
        if (range.m_tEnd < m_vectTimes[i].m_tBegin)
        {
            if (0 == i)
                m_vectTimes.insert(m_vectTimes.begin(), range);
            else if (range.m_tBegin > m_vectTimes[i - 1].m_tEnd)
                m_vectTimes.insert(m_vectTimes.begin() + i, range);
            else
                LOG_ERROR << "Time error: begin time: " 
                          << (unsigned int)begin 
                          << ", end time: " 
                          << (unsigned int)end 
                          << std::endl;

            m_uRange = CheckCurRange(time(NULL)) + 1;

            return;
        }
    }

    if (range.m_tBegin > m_vectTimes.rbegin()->m_tEnd)
        m_vectTimes.push_back(range);
    else
        LOG_ERROR << "Time error: begin time: " 
                  << (unsigned int)begin 
                  << ", end time: " 
                  << (unsigned int)end 
                  << std::endl;

    m_uRange = CheckCurRange(time(NULL)) + 1;
}

void CTimeMgr::OnUpdate(time_t tTime)
{
    if (m_bIsOpening != IsInTimeRange(tTime))
        SetOpening(!m_bIsOpening);
}

bool CTimeMgr::CanParticipateAgain() const
{
    if (m_uRange > m_vectTimes.size())
        return false;

    time_t tTime = time(NULL);

    if (tTime > m_vectTimes.rbegin()->m_tEnd)
        return false;

    return true;
}

float CTimeMgr::CalcCD() const
{
    unsigned i = m_uRange - 1;

    if (i >= m_vectTimes.size())
    {
        LOG_ERROR << "Range(" << m_uRange 
                  << ") can not bigger than vector's size(" 
                  << (int)m_vectTimes.size() 
                  << ")" << std::endl;

        return 0;
    }

    time_t tTime = time(NULL);
    time_t tNextTime = tTime;

    if (tTime < m_vectTimes[i].m_tBegin)
        tNextTime = m_vectTimes[i].m_tBegin;
    else if (tTime <= m_vectTimes[i].m_tEnd)
        tNextTime = m_vectTimes[i].m_tEnd;

    return (float)difftime(tNextTime, tTime);
}

time_t CTimeMgr::GetCurRangeBeginTime() const
{
    unsigned int i = m_uRange - 1;

    if (i >= m_vectTimes.size())
    {
        LOG_ERROR << "Range(" << m_uRange 
                  << ") can not bigger than vector's size(" 
                  << (int)m_vectTimes.size() 
                  << ")" << std::endl;

        return 0;
    }

    return m_vectTimes[i].m_tBegin;
}

time_t CTimeMgr::GetCurRangeEndTime() const
{
    unsigned int i = m_uRange - 1;

    if (i >= m_vectTimes.size())
    {
        LOG_ERROR << "Range(" << m_uRange 
                  << ") can not bigger than vector's size(" 
                  << (int)m_vectTimes.size() 
                  << ")" << std::endl;

        return 0;
    }

    return m_vectTimes[i].m_tEnd;
}

time_t CTimeMgr::GetLastRangeEndTime() const
{
    unsigned int i = m_uRange - 2;

    if (i >= m_vectTimes.size())
    {
        LOG_ERROR << "Range(" << m_uRange 
                  << ") can not bigger than vector's size(" 
                  << (int)m_vectTimes.size() 
                  << ")" << std::endl;

        return 0;
    }

    return m_vectTimes[i].m_tEnd;
}

std::string CTimeMgr::GetCurRangeExtend() const
{
    unsigned int i = m_uRange - 1;

    if (i >= m_vectTimes.size())
        return "";

    return m_vectTimes[i].m_strExtend;
}

void CTimeMgr::SetOpening(bool bOpening)
{
    LOG_DEBUG << "CTimeMgr::SetOpening(). Old:("
              << (m_bIsOpening ? "true" : "false")
              << "), New:("
              << (bOpening ? "true" : "false")
              << ")" << std::endl;

    m_bIsOpening = bOpening;

    if (!m_bIsOpening)
        ++m_uRange;
}

bool CTimeMgr::IsInTimeRange(time_t tTime)
{
    unsigned int i = m_uRange - 1;

    if (i >= m_vectTimes.size())
        return false;

    if (m_vectTimes[i].m_tBegin <= tTime && tTime <= m_vectTimes[i].m_tEnd)
        return true;

    return false;
}

unsigned short CTimeMgr::CheckCurRange(time_t tTime) const
{
    unsigned short range = (unsigned short)m_vectTimes.size();

    for (unsigned int i = m_vectTimes.size(); i > 0; --i)
    {
        if (tTime >= m_vectTimes[i - 1].m_tEnd)
            break;

        --range;
    }

    return range;
}

