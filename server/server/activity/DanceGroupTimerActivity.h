#ifndef  _DANCEGROUP_TIMER_ACTIVITY_H__
#define _DANCEGROUP_TIMER_ACTIVITY_H__
#pragma once

#include "TimerActivity.h"


//舞团每天消耗
class CDanceGroupActivity : public CDayActivity
{
public:
	CDanceGroupActivity();
	virtual ~CDanceGroupActivity();

public:
	//初始化
	void Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime );
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//活动触发时
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);

private:
	//处理对应的消耗
	void _OnTime();

private:

};


class CDanceGroupRecord : public CDayActivity
{
public:
	CDanceGroupRecord();
	virtual ~CDanceGroupRecord();

public:
	//初始化
	void Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime );
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//活动触发时
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);

private:
	//处理对应的消耗
	void _OnTime();

private:
};

#endif //_DANCEGROUP_TIMER_ACTIVITY_H__
