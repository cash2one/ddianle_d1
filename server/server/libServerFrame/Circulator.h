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

	virtual void EnterExitProcess();//进入退出流程,即停止接受连接,断开所有客户,保存数据,退出进程...	

public:
	bool m_bShutdownInTime;//一定时间内关闭
	time_t m_timeShutdown;//关闭的时间
	bool m_bExiting;//正在退出
	CQueryManager* m_pQueryMgr;
	CClientSocketMgr* m_pAgent;
	CSlotPeer *m_AgnetSlotPeer;

	STAT_BASE m_stat;
	unsigned int m_nIntervalReportStat;
	unsigned int m_nIntervalReconnectAgent;
};

#endif//__CIRCULATOR_H__


//file end

