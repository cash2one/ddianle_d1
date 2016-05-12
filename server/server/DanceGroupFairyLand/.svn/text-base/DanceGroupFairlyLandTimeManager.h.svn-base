#ifndef __GROUPSERVER_DANCEGROUPFAIRLYLANDTIMEMANAGER_H__
#define __GROUPSERVER_DANCEGROUPFAIRLYLANDTIMEMANAGER_H__

#include <functional>
#include <queue>

class FairlyLandTimer
{
public:
    FairlyLandTimer();
    ~FairlyLandTimer();

    bool operator > (const FairlyLandTimer &ft) const;

    time_t m_nWaitStamp; // 超出这个时间就会触发
    unsigned int m_nDanceGroupID; // 舞团ID
};

class FairlyLandTimeManager
{
private:
    FairlyLandTimeManager();
public:
    ~FairlyLandTimeManager();

    static FairlyLandTimeManager& Instance();

    void OnUpdate(time_t now);

    void AddTimer(time_t waitTime, unsigned int nDanceGroupID);

    void OnTimer(time_t now, time_t waitTime, unsigned int nDanceGroupID);

    typedef std::priority_queue<FairlyLandTimer, std::vector<FairlyLandTimer>, std::greater<FairlyLandTimer> > TimeQueue;
    TimeQueue m_timerQueue;
};





#endif // __GROUPSERVER_DANCEGROUPFAIRLYLANDTIMEMANAGER_H__