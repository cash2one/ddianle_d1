/*
 * TimeRanges.h
 * Description: 时间区间类
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-11-19 17:17:07
 */

#ifndef __TIMERANGES_H__
#define __TIMERANGES_H__

#include <list>
#include <vector>
#include <string>
#include <time.h>


typedef struct
{
    time_t  m_tBegin;
    time_t  m_tEnd;
} TimeRange;

//////////////////////////////////////////////////////////////////////////

enum OpenTimeType
{
    OpenTimeType_None = 0,
    OpenTimeType_Month,         // 以月为周期
    OpenTimeType_Week,          // 以周为周期
    OpenTimeType_Day,           // 以日为周期
};

class COpenTime
{
public:
    COpenTime();
    virtual ~COpenTime() {}
    bool Init(OpenTimeType type, const std::list<unsigned int> &rDate, std::string &beginHour, std::string &endHour);

public:
    bool AddHourRange(std::string &beginHour, std::string &endHour);

    bool IsInOpenTime(time_t tTime) const;

    bool IsInSameHourRange(time_t tTime1, time_t tTime2) const;

protected:
    bool IsOpenToday(const struct tm &tmTime) const;
    bool CheckMonthEnd(const struct tm &tmTime) const;

    int FindHourRange(time_t tTime) const;

private:
    OpenTimeType            m_nType;
    unsigned char           m_bufDate[32];
    std::vector<TimeRange>  m_vectHourRanges;
};

//////////////////////////////////////////////////////////////////////////

/**
* TimeRanges:
* 
*/

class CTimeRanges
{
public:
    CTimeRanges();
    virtual ~CTimeRanges() {}

public:
    void AddRangeInfo(time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour);
    void AddRangeInfo(unsigned int nIdx, std::string &beginHour, std::string &endHour);

    bool IsInTimeRange(time_t tTime) const;

    int FindRange(time_t begin, time_t end) const;

    bool IsInSameOpenRange(time_t tTime1, time_t tTime2) const;

private:
    std::vector<TimeRange>  m_vectTimes;
    std::vector<COpenTime>  m_vectOpenTimes;
};

#endif // __TIMERANGES_H__

