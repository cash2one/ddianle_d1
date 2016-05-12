#ifndef __GROUPSERVER_DANCEGROUPFAIRYLANDTIMEMANAGER_H__
#define __GROUPSERVER_DANCEGROUPFAIRYLANDTIMEMANAGER_H__

#include <time.h>
#include <functional>
#include <queue>

class FairyLandTimer
{
public:
    FairyLandTimer();
    ~FairyLandTimer();

    bool operator > (const FairyLandTimer &ft) const;

    time_t m_nWaitStamp; // 超出这个时间就会触发
    unsigned int m_nDanceGroupID; // 舞团ID
};

class FairyLandTimeManager
{
private:
    FairyLandTimeManager();
public:
    ~FairyLandTimeManager();

    static FairyLandTimeManager& Instance();

    void OnUpdate(time_t now);

    void AddTimer(time_t waitTime, unsigned int nDanceGroupID);

    void OnTimer(time_t now, time_t waitTime, unsigned int nDanceGroupID);

    typedef std::priority_queue<FairyLandTimer, std::vector<FairyLandTimer>, std::greater<FairyLandTimer> > TimeQueue;
    TimeQueue m_timerQueue;
};





#endif // __GROUPSERVER_DANCEGROUPFAIRYLANDTIMEMANAGER_H__