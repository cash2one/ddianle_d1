#ifndef __AGENT_CIRCULATOR_H__
#define __AGENT_CIRCULATOR_H__

#include "../libServerFrame/Circulator.h"
#include "../../socket/MsgBaseStruct.h"
#include "../libServerFrame/QueryManager.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

class CAgentCirculator: public CCirculator
{
public:
	CAgentCirculator();
	virtual ~CAgentCirculator();

	void CloseServer(unsigned char bCloseSvr[SVRTYPE_COUNT]);
protected:
	// Initialize game logic, load game data...
	virtual bool Initialize();

	void PumpProc();

	//process messages from local servers
	void ProcessLocalServerMsg(LPGameMsg_Base pMsg,CSlotPeer & slotPeer);
	//process socket messages from monitor
	//virtual void ProcessSocketMsg(LPGameMsg_Base pGameMsg, unsigned short &nSlot);
	//process query finished 
	virtual void ProcessQueryFinished(QueryBase* pQuery);
	//process heart beat
	virtual void ProcessHeartBeat(int& timeElapse);

	//网络消息和数据库处理
	void RegisterMessage();
	void RegisterMessageProcessor();

private:
	CSlotPeer *m_pLocalSlotPeer;
	void OnAgentStat(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorConnect(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorDisConnect(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorSysInfo(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorExecute(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorCloseServer(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorStartServer(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorKillServer(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorSetting(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorAnnounce(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorShutdownInTime(GameMsg_Base & msg ,CSlotPeer & slotPeer);
	void OnMonitorExit(GameMsg_Base & msg ,CSlotPeer & slotPeer);

	void SerializeSetting(std::string &strDestBuffer) const;

private:
	CSocketMgr* m_pSockLocal;//供本地服务器连接的SocketMgr
	STAT_SYS m_statSys;//系统状况信息
	LPSTAT_BASE m_aStat;//本地服务器状态
	int *m_aPID;//本地服务器的ｐｉｄ
	AGENT_SETTING m_setting;//
	void NotifyAllMonitor(LPGameMsg_Base pMsg);
#ifndef WIN32
	bool GetSysInfo(STAT_SYS* pInfo);
#endif
	void ReportSysInfo();
	void LoadSetting();
	void LaunchServer(int nSvrType);
	void KillServer(int nSvrType);
};

#ifndef WIN32
__int64 GetProcessMem(int pid);//获得进程所占用的ｍｅｍ
__int64 GetSystemMem();//获得系统的ｍｅｍ
#endif


#endif//__AGENT_CIRCULATOR_H__


//end file


