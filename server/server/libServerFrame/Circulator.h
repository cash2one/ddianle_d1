#ifndef __CIRCULATOR_H__
#define __CIRCULATOR_H__

#include "CirculatorBase.h"

class CQueryManager;
class QueryBase;

class CCirculator:public CCirculatorBase
{
public:
	CCirculator();
	virtual ~CCirculator();
public:

protected:
	// Initialize game logic, load game data...
	virtual bool Initialize();

	//release game logic
	virtual void Release();

	//
	virtual void PumpProc(); 

	virtual bool ProcessAgentMsg(LPGameMsg_Base pAgentMsg);
	//process query finished 
	void ProcessQueryFinished(QueryBase* pQuery);
	//process heart beat
	virtual void ProcessHeartBeat(int& timeElapse);

	virtual void ReportStat();

	virtual void EnterExitProcess();//�����˳�����,��ֹͣ��������,�Ͽ����пͻ�,��������,�˳�����...	

public:
	bool m_bShutdownInTime;//һ��ʱ���ڹر�
	time_t m_timeShutdown;//�رյ�ʱ��
	bool m_bExiting;//�����˳�
	CQueryManager* m_pQueryMgr;
	CClientSocketMgr* m_pAgent;
	CSlotPeer *m_AgnetSlotPeer;

	STAT_BASE m_stat;
	unsigned int m_nIntervalReportStat;
	unsigned int m_nIntervalReconnectAgent;
};

#endif//__CIRCULATOR_H__


//file end

