#include "DanceGroupFairlyLandTimeManager.h"
#include "DanceGroupFairlyLandProcessor.h"

FairlyLandTimer::FairlyLandTimer()
    : m_nWaitStamp(0)
    , m_nDanceGroupID(0)
{

}
FairlyLandTimer::~FairlyLandTimer()
{

}

bool FairlyLandTimer::operator > (const FairlyLandTimer &ft) const
{
    return this->m_nWaitStamp > ft.m_nWaitStamp;
}


FairlyLandTimeManager::FairlyLandTimeManager()
{

}

FairlyLandTimeManager::~FairlyLandTimeManager()
{

}

FairlyLandTimeManager& FairlyLandTimeManager::Instance()
{
    static FairlyLandTimeManager s;
    return s;
}


void FairlyLandTimeManager::OnUpdate(time_t now)
{
    while (!m_timerQueue.empty() && m_timerQueue.top().m_nWaitStamp >= now)
    {
        OnTimer(now, m_timerQueue.top().m_nWaitStamp, m_timerQueue.top().m_nDanceGroupID);
        m_timerQueue.pop();
    }
}

void FairlyLandTimeManager::AddTimer(time_t waitTime, unsigned int nDanceGroupID)
{
    FairlyLandTimer f;
    f.m_nWaitStamp = waitTime;
    f.m_nDanceGroupID = nDanceGroupID;
    m_timerQueue.push(f);

    return;
}

void FairlyLandTimeManager::OnTimer(time_t now, time_t waitTime, unsigned int nDanceGroupID)
{
    CDanceGroupFairlyLandProcessor::Instance().CheckBlessStamp(now, nDanceGroupID);
}
