#include "DanceGroupFairyLandTimeManager.h"
#include "DanceGroupFairyLandProcessor.h"

FairyLandTimer::FairyLandTimer()
    : m_nWaitStamp(0)
    , m_nDanceGroupID(0)
{

}
FairyLandTimer::~FairyLandTimer()
{

}

bool FairyLandTimer::operator > (const FairyLandTimer &ft) const
{
    return this->m_nWaitStamp > ft.m_nWaitStamp;
}


FairyLandTimeManager::FairyLandTimeManager()
{

}

FairyLandTimeManager::~FairyLandTimeManager()
{

}

FairyLandTimeManager& FairyLandTimeManager::Instance()
{
    static FairyLandTimeManager s;
    return s;
}


void FairyLandTimeManager::OnUpdate(time_t now)
{
    while (!m_timerQueue.empty() && m_timerQueue.top().m_nWaitStamp >= now)
    {
        OnTimer(now, m_timerQueue.top().m_nWaitStamp, m_timerQueue.top().m_nDanceGroupID);
        m_timerQueue.pop();
    }
}

void FairyLandTimeManager::AddTimer(time_t waitTime, unsigned int nDanceGroupID)
{
    FairyLandTimer f;
    f.m_nWaitStamp = waitTime;
    f.m_nDanceGroupID = nDanceGroupID;
    m_timerQueue.push(f);

    return;
}

void FairyLandTimeManager::OnTimer(time_t now, time_t waitTime, unsigned int nDanceGroupID)
{
    CDanceGroupFairyLandProcessor::Instance().CheckBlessStamp(now, nDanceGroupID);
}
