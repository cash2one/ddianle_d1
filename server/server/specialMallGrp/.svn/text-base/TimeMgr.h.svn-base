/*
 * TimeMgr.h
 * Description: 时间管理类
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-16 16:30:38
 */

#ifndef __TIMEMGR_H__
#define __TIMEMGR_H__

#include <vector>
#include <string>

typedef struct
{
    time_t      m_tBegin;
    time_t      m_tEnd;
    std::string m_strExtend;
} TimeRange;

/**
* TimeMgr:
* 
*/

class CTimeMgr
{
public:
    CTimeMgr();
    virtual ~CTimeMgr();

public:
    void AddTimeRangeInfo(time_t begin, time_t end, std::string &rExtend);

    virtual void OnUpdate(time_t tTime);

    virtual bool CanParticipateAgain() const;

    float CalcCD() const;

    bool IsOpening() const { return m_bIsOpening; }

    unsigned short GetCurRange() const { return m_uRange; }

    time_t GetCurRangeBeginTime() const;
    time_t GetCurRangeEndTime() const;
    time_t GetLastRangeEndTime() const;

    std::string GetCurRangeExtend() const;

    unsigned int GetRangeSize() const { return m_vectTimes.size(); }

protected:
    virtual void SetOpening(bool bOpening);

    bool IsInTimeRange(time_t tTime);

    unsigned short CheckCurRange(time_t tTime) const;

private:
    bool                    m_bIsOpening;   // 是否开启

    unsigned short          m_uRange;       // 第几轮，从1开始

    std::vector<TimeRange>  m_vectTimes;    // 时间区间
};

#endif // __TIMEMGR_H__

