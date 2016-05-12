#ifndef  _DANCEGROUP_TIMER_ACTIVITY_H__
#define _DANCEGROUP_TIMER_ACTIVITY_H__
#pragma once

#include "TimerActivity.h"


//����ÿ������
class CDanceGroupActivity : public CDayActivity
{
public:
	CDanceGroupActivity();
	virtual ~CDanceGroupActivity();

public:
	//��ʼ��
	void Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime );
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�����ʱ
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);

private:
	//�����Ӧ������
	void _OnTime();

private:

};


class CDanceGroupRecord : public CDayActivity
{
public:
	CDanceGroupRecord();
	virtual ~CDanceGroupRecord();

public:
	//��ʼ��
	void Init( const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime );
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�����ʱ
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);

private:
	//�����Ӧ������
	void _OnTime();

private:
};

#endif //_DANCEGROUP_TIMER_ACTIVITY_H__
