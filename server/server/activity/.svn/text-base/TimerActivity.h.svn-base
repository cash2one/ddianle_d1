#ifndef	__TIMER_ACTIVITY_H__
#define __TIMER_ACTIVITY_H__

#include <time.h>
#include "../util/Crontab.h"
#include "TimeRanges.h"
#ifdef WIN32
#pragma warning(disable:4100)
#endif

class CEntityNetComponent;

class ITimerActivity
{
public:
	ITimerActivity():m_bBegun(false), m_nActivityType(0){}
	virtual ~ITimerActivity(){}

	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow) = 0;
	//是否有效，无效时系统会回收
	virtual bool IsValid() = 0;
	//发送活动状态到客户端
	virtual void UpdateStateToClient(CEntityNetComponent & rNetCom){}

public:
	int GetActivityType() const { return m_nActivityType; }
	void SetActivityType(int val) { m_nActivityType = val; }
protected:
	bool m_bBegun;
	int m_nActivityType;//对应EActivity
};

class CTimeActivity : public ITimerActivity
{
public:
	CTimeActivity();
	virtual ~CTimeActivity();
	//初始化，初始化后才是有效的
	void Init( time_t tBeginTime, time_t tEndTime);
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//当活动开启
	virtual void OnActivityBegin();
	//当活动结束
	virtual void OnActivityEnd();
	//活动是否开启
	bool IsOpening() {return m_bIsOpening;}
	//是否有效
	virtual bool IsValid(){return m_bValid;}
private:
	//检查活动状态
	void _CheckActivityState(time_t tNow);
	//设置活动开启状态
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//是否有效
	bool m_bIsOpening;				//是否开启
	time_t m_tBeginTime;			//活动开启时间
	time_t m_tEndTime;				//活动结束时间
};

//每天准点触发活动
class CDayActivity : public ITimerActivity
{
public:
	CDayActivity();
	virtual ~CDayActivity();
	//初始化，初始化后才是有效的
	void Init(unsigned int nHour, unsigned int nMinute, time_t tBeginTime, time_t tEndTime);
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//活动触发时
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);
	//当活动开启
	virtual void OnActivityBegin();
	//当活动结束
	virtual void OnActivityEnd();
	//活动是否开启
	bool IsOpening() {return m_bIsOpening;}
	//是否有效
	virtual bool IsValid(){return m_bValid;}
private:
	//检查触发情况
	void _CheckTriggering(const unsigned long &lTimeElapsed, time_t tNow);
	//检查活动状态
	void _CheckActivityState(time_t tNow);
	//根据当前时间设置刷新时间
	void _SetLastRefreshTime(time_t tNow);
	//设置活动开启状态
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//是否有效
	bool m_bIsOpening;				//是否开启
	time_t m_tLastRefreshTime;		//最后的刷新时间
	unsigned char m_nHour;			//触发时间－小时
	unsigned char m_nMinute;		//触发时间－分钟
	time_t m_tBeginTime;			//活动开启时间
	time_t m_tEndTime;				//活动结束时间
};

//每天时间段活动
class CDayTimeBlockActivity : public ITimerActivity
{
public:
	CDayTimeBlockActivity();
	virtual ~CDayTimeBlockActivity();
	//初始化，初始化后才是有效的
	void Init(unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute, time_t tBeginTime, time_t tEndTime);
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//当活动开启
	virtual void OnActivityBegin();
	//当活动结束
	virtual void OnActivityEnd();
	//活动是否开启
	bool IsOpening() {return m_bIsOpening;}
	//活动开始运营
	virtual void OnStart();
	//活动结束运营
	virtual void OnStop();
	//当前活动运营中
	bool IsRunning() {return m_bIsRunning;}
	//是否有效
	virtual bool IsValid(){return m_bValid;} 
protected:
	//检查活动运营状态
	virtual void _CheckActivityRunningState(time_t tNow);
	//检查活动状态
	void _CheckActivityState(time_t tNow);
	//设置活动开启状态
	void _SetOpening(bool bIsOpening);
	//设置活动运营状态
	void _SetRunning(bool bIsRunning);
protected:
	bool m_bValid;					//是否有效
	bool m_bIsOpening;				//是否开启
	bool m_bIsRunning;				//是否运营
	unsigned char m_nStartHour;		//活动运营开始时间－小时
	unsigned char m_nStartMinute;	//活动运营开始时间－分钟
	unsigned char m_nStopHour;		//活动运营结束时间－小时
	unsigned char m_nStopMinute;	//活动运营结束时间－分钟
	int m_nStartMinutes;			//开启时间的分钟值
	int m_nStopMinutes;				//结束时间的分钟值
	time_t m_tBeginTime;			//活动开启时间
	time_t m_tEndTime;				//活动结束时间
};
//每周天时间段活动
class CWeekDayTimeBlockActivity : public CDayTimeBlockActivity
{
public:
	CWeekDayTimeBlockActivity();
	virtual ~CWeekDayTimeBlockActivity();

	//初始化，初始化后才是有效的
	void Init(unsigned char nWeekDay, unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute,
		time_t tBeginTime, time_t tEndTime);
private:
	//检查活动运营状态
	virtual void _CheckActivityRunningState(time_t tNow);
protected:
	unsigned char m_nWeekDay; // days since Sunday - [0,6]
};
//万能时间格式的活动
class CCrontabActivity : public ITimerActivity
{
public:
	CCrontabActivity();
	virtual ~CCrontabActivity();
	//初始化，初始化后才是有效的
	void Init( const char* strBeginTime, const char* strEndTime );
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//当活动开启
	virtual void OnActivityBegin();
	//当活动结束
	virtual void OnActivityEnd();
	//活动是否开启
	bool IsOpening() {return m_bIsOpening;}
	//是否有效
	virtual bool IsValid(){return m_bValid;}

	const CCrontab& GetBeginTime(){return m_beginTime;}
	const CCrontab& GetEndTime(){return m_endTime;}
private:
	//检查活动状态
	void _CheckActivityState(time_t tNow);
	//设置活动开启状态
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//是否有效
	bool m_bIsOpening;				//是否开启
	CCrontab m_beginTime;			//开启时间
	CCrontab m_endTime;				//结束时间
};

//////////////////////////////////////////////////////////////////////////
// 月、周、日活动
//////////////////////////////////////////////////////////////////////////

class CMonthWeekDayActivity : public ITimerActivity
{
public:
    CMonthWeekDayActivity();
    virtual ~CMonthWeekDayActivity();
    void Init(time_t beginTime, time_t endTime, time_t beginShowTime, time_t endShowTime);

public:
    // heart beat
    virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
    // 是否有效，无效时系统会收回
    virtual bool IsValid() { return m_bValid; }

    // 当活动开启
    virtual void OnActivityBegin();

    // 当活动结束
    virtual void OnActivityEnd();

    // 活动是否开启
    bool IsOpening() const { return m_bIsOpening; }

    // 活动是否显示
    bool IsShowing() const { return m_bIsShowing; }

    void AddTimeRangeInfo(time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour);

    bool IsOpenNow();

    // 是否能再次参与活动
    virtual bool CanParticipateAgain(time_t lastTime) const;

protected:
    void SetOpening(bool bOpening);

    bool IsInTimeRange(time_t tTime);

protected:
    bool m_bValid;                  // 是否有效
    bool m_bIsOpening;              // 是否开启
    bool m_bIsShowing;              // 是否显示

    time_t m_nBeginTime;            // 活动开始时间
    time_t m_nEndTime;              // 活动结束时间
    time_t m_nBeginShowTime;        // 开始显示时间
    time_t m_nEndShowTime;          // 结束显示时间

    CTimeRanges m_cTimeRanges;      // 开启时间区间
};

#endif


