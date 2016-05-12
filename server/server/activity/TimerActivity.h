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

	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow) = 0;
	//�Ƿ���Ч����Чʱϵͳ�����
	virtual bool IsValid() = 0;
	//���ͻ״̬���ͻ���
	virtual void UpdateStateToClient(CEntityNetComponent & rNetCom){}

public:
	int GetActivityType() const { return m_nActivityType; }
	void SetActivityType(int val) { m_nActivityType = val; }
protected:
	bool m_bBegun;
	int m_nActivityType;//��ӦEActivity
};

class CTimeActivity : public ITimerActivity
{
public:
	CTimeActivity();
	virtual ~CTimeActivity();
	//��ʼ������ʼ���������Ч��
	void Init( time_t tBeginTime, time_t tEndTime);
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�������
	virtual void OnActivityBegin();
	//�������
	virtual void OnActivityEnd();
	//��Ƿ���
	bool IsOpening() {return m_bIsOpening;}
	//�Ƿ���Ч
	virtual bool IsValid(){return m_bValid;}
private:
	//���״̬
	void _CheckActivityState(time_t tNow);
	//���û����״̬
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//�Ƿ���Ч
	bool m_bIsOpening;				//�Ƿ���
	time_t m_tBeginTime;			//�����ʱ��
	time_t m_tEndTime;				//�����ʱ��
};

//ÿ��׼�㴥���
class CDayActivity : public ITimerActivity
{
public:
	CDayActivity();
	virtual ~CDayActivity();
	//��ʼ������ʼ���������Ч��
	void Init(unsigned int nHour, unsigned int nMinute, time_t tBeginTime, time_t tEndTime);
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�����ʱ
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);
	//�������
	virtual void OnActivityBegin();
	//�������
	virtual void OnActivityEnd();
	//��Ƿ���
	bool IsOpening() {return m_bIsOpening;}
	//�Ƿ���Ч
	virtual bool IsValid(){return m_bValid;}
private:
	//��鴥�����
	void _CheckTriggering(const unsigned long &lTimeElapsed, time_t tNow);
	//���״̬
	void _CheckActivityState(time_t tNow);
	//���ݵ�ǰʱ������ˢ��ʱ��
	void _SetLastRefreshTime(time_t tNow);
	//���û����״̬
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//�Ƿ���Ч
	bool m_bIsOpening;				//�Ƿ���
	time_t m_tLastRefreshTime;		//����ˢ��ʱ��
	unsigned char m_nHour;			//����ʱ�䣭Сʱ
	unsigned char m_nMinute;		//����ʱ�䣭����
	time_t m_tBeginTime;			//�����ʱ��
	time_t m_tEndTime;				//�����ʱ��
};

//ÿ��ʱ��λ
class CDayTimeBlockActivity : public ITimerActivity
{
public:
	CDayTimeBlockActivity();
	virtual ~CDayTimeBlockActivity();
	//��ʼ������ʼ���������Ч��
	void Init(unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute, time_t tBeginTime, time_t tEndTime);
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�������
	virtual void OnActivityBegin();
	//�������
	virtual void OnActivityEnd();
	//��Ƿ���
	bool IsOpening() {return m_bIsOpening;}
	//���ʼ��Ӫ
	virtual void OnStart();
	//�������Ӫ
	virtual void OnStop();
	//��ǰ���Ӫ��
	bool IsRunning() {return m_bIsRunning;}
	//�Ƿ���Ч
	virtual bool IsValid(){return m_bValid;} 
protected:
	//�����Ӫ״̬
	virtual void _CheckActivityRunningState(time_t tNow);
	//���״̬
	void _CheckActivityState(time_t tNow);
	//���û����״̬
	void _SetOpening(bool bIsOpening);
	//���û��Ӫ״̬
	void _SetRunning(bool bIsRunning);
protected:
	bool m_bValid;					//�Ƿ���Ч
	bool m_bIsOpening;				//�Ƿ���
	bool m_bIsRunning;				//�Ƿ���Ӫ
	unsigned char m_nStartHour;		//���Ӫ��ʼʱ�䣭Сʱ
	unsigned char m_nStartMinute;	//���Ӫ��ʼʱ�䣭����
	unsigned char m_nStopHour;		//���Ӫ����ʱ�䣭Сʱ
	unsigned char m_nStopMinute;	//���Ӫ����ʱ�䣭����
	int m_nStartMinutes;			//����ʱ��ķ���ֵ
	int m_nStopMinutes;				//����ʱ��ķ���ֵ
	time_t m_tBeginTime;			//�����ʱ��
	time_t m_tEndTime;				//�����ʱ��
};
//ÿ����ʱ��λ
class CWeekDayTimeBlockActivity : public CDayTimeBlockActivity
{
public:
	CWeekDayTimeBlockActivity();
	virtual ~CWeekDayTimeBlockActivity();

	//��ʼ������ʼ���������Ч��
	void Init(unsigned char nWeekDay, unsigned int nStartHour, unsigned int nStartMinute, unsigned int nStopHour, unsigned int nStopMinute,
		time_t tBeginTime, time_t tEndTime);
private:
	//�����Ӫ״̬
	virtual void _CheckActivityRunningState(time_t tNow);
protected:
	unsigned char m_nWeekDay; // days since Sunday - [0,6]
};
//����ʱ���ʽ�Ļ
class CCrontabActivity : public ITimerActivity
{
public:
	CCrontabActivity();
	virtual ~CCrontabActivity();
	//��ʼ������ʼ���������Ч��
	void Init( const char* strBeginTime, const char* strEndTime );
	//����
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//�������
	virtual void OnActivityBegin();
	//�������
	virtual void OnActivityEnd();
	//��Ƿ���
	bool IsOpening() {return m_bIsOpening;}
	//�Ƿ���Ч
	virtual bool IsValid(){return m_bValid;}

	const CCrontab& GetBeginTime(){return m_beginTime;}
	const CCrontab& GetEndTime(){return m_endTime;}
private:
	//���״̬
	void _CheckActivityState(time_t tNow);
	//���û����״̬
	void _SetOpening(bool bIsOpening);
protected:
	bool m_bValid;					//�Ƿ���Ч
	bool m_bIsOpening;				//�Ƿ���
	CCrontab m_beginTime;			//����ʱ��
	CCrontab m_endTime;				//����ʱ��
};

//////////////////////////////////////////////////////////////////////////
// �¡��ܡ��ջ
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
    // �Ƿ���Ч����Чʱϵͳ���ջ�
    virtual bool IsValid() { return m_bValid; }

    // �������
    virtual void OnActivityBegin();

    // �������
    virtual void OnActivityEnd();

    // ��Ƿ���
    bool IsOpening() const { return m_bIsOpening; }

    // ��Ƿ���ʾ
    bool IsShowing() const { return m_bIsShowing; }

    void AddTimeRangeInfo(time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour);

    bool IsOpenNow();

    // �Ƿ����ٴβ���
    virtual bool CanParticipateAgain(time_t lastTime) const;

protected:
    void SetOpening(bool bOpening);

    bool IsInTimeRange(time_t tTime);

protected:
    bool m_bValid;                  // �Ƿ���Ч
    bool m_bIsOpening;              // �Ƿ���
    bool m_bIsShowing;              // �Ƿ���ʾ

    time_t m_nBeginTime;            // ���ʼʱ��
    time_t m_nEndTime;              // �����ʱ��
    time_t m_nBeginShowTime;        // ��ʼ��ʾʱ��
    time_t m_nEndShowTime;          // ������ʾʱ��

    CTimeRanges m_cTimeRanges;      // ����ʱ������
};

#endif


