#include "../share/StdAfx.h"
#include "../libServerFrame/Main.h"
#include "AgentCirculator.h"
#include "AgentMsg_Stat.h"
#include "AgentMsg_Close.h"
#include "AgentMsg_SysInfo.h"
#include "AgentMsg_Setting.h"
#include "AgentMsg_Execute.h"
#include "AgentMsg_Start.h"
#include "AgentMsg_Kill.h"
#include "AgentMsg_Exit.h"
#include "AgentMsg_Announce.h"
#include "AgentMsg_ShutdownInTime.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_System_Connect.h"
#include "../../socket/GameMsg_System_Disconnect.h"
#include "../../socket/MsgBaseStruct.h"
#include "../../socket/Formatter.h"

#include <stdlib.h>//for system()
#include <time.h>
#include <sstream>

CAgentCirculator* g_pCirculator = NULL;

CAgentCirculator::CAgentCirculator()
{
	g_pCirculator = this;
	m_pSockLocal = NULL;
	m_aStat = NULL;
	m_aPID = NULL;
	m_pLocalSlotPeer = NULL;
}

CAgentCirculator::~CAgentCirculator()
{
	Release();

	g_pCirculator = NULL;
	
	if(m_aStat != NULL)
		delete [] m_aStat;
	delete m_aPID;

	delete m_pSockLocal;
	m_pSockLocal = NULL;

	if(m_pLocalSlotPeer )
	{
		delete m_pLocalSlotPeer;
		m_pLocalSlotPeer= NULL;
	}
}

bool CAgentCirculator::Initialize()
{
    bool bRes = CCirculator::Initialize();
    if (bRes)
    {
        unsigned short nCount = (unsigned short)g_Config.GetInt(g_sSection, "internalcount", 6);
        unsigned short nPort = (unsigned short)g_Config.GetInt(g_sSection, "internalport", 7751);
        bRes = InitSocketMgr(m_pSockLocal, nCount, nPort);
        m_aStat = new STAT_BASE[nCount];
        m_aPID = new int[nCount];
        memset(m_aPID, 0, nCount * sizeof(int));

        m_pLocalSlotPeer = new CSlotPeer();
        m_pLocalSlotPeer->SetSocketMgr(m_pSocketMgr);
    }

    if (bRes)
    {
        LoadSetting();
    }

    std::string strSetting;
    SerializeSetting(strSetting);
    WriteLog(LOGLEVEL_SYSTEM, strSetting.c_str());

    RegisterMessage();
    RegisterMessageProcessor();

    return bRes;
}

void CAgentCirculator::LoadSetting()
{
	m_setting.nSysInfoInterval = g_Config.GetInt("interval", "reportsysinfo", 3000)/m_nElapseHeartBeat;
	m_setting.nCheckInterval = g_Config.GetInt("interval", "startserver", 30000)/m_nElapseHeartBeat;

	for(int i = SVRTYPE_AGENT+1; i < SVRTYPE_COUNT; i++)
	{
		std::string strKey("keep");
		strKey += s_szSvrType[i];
		m_setting.abKeepServer[i] = g_Config.GetInt(g_sSection, strKey.c_str(), 0) != 0;
	}
}

void CAgentCirculator::SerializeSetting(std::string &strDestBuffer) const
{
	std::stringstream ss;
	ss << "Agent setting: "
		<< "SysInfoInterval=[" << m_setting.nSysInfoInterval << "] "
		<< "CheckInterval=[" << m_setting.nCheckInterval << "] ";

	for (int i = SVRTYPE_AGENT+1; i < SVRTYPE_COUNT; i++)
	{
		ss << s_szSvrType[i] << "=[" << (m_setting.abKeepServer[i] ? "Open" : "Close") << "] ";
	}

	strDestBuffer = ss.str();
}

void CAgentCirculator::PumpProc()
{
	CCirculator::PumpProc();

	LPGameMsg_Base pMsg = NULL;
	unsigned short nSlot;
	while (m_pSockLocal->GetGameMsg(pMsg, nSlot))
	{
		if (pMsg != NULL)
		{
			m_pLocalSlotPeer->SetSlot(nSlot);
			ProcessLocalServerMsg(pMsg,*m_pLocalSlotPeer);
			delete pMsg;
		}
		pMsg = NULL;
	}
}

void CAgentCirculator::ProcessHeartBeat(int& timeElapse)
{
	//TRACE("HeartBeat\n");

	CCirculator::ProcessHeartBeat(timeElapse);

	if(m_setting.nSysInfoInterval != 0 && m_nHeartBeatCount % m_setting.nSysInfoInterval == 0)
	{
		ReportSysInfo();
	}

	if (m_setting.nCheckInterval != 0 && m_nHeartBeatCount % m_setting.nCheckInterval == 0)
	{
		//TRACE("Check Server running!\n");
		int i;
		bool bLaunch[SVRTYPE_COUNT] = {false, true,true,true,true,true,true};
		//已经在运行的Server不要再开启
		int nCount = m_pSockLocal->GetMaxClientCount();
		for(i = 0; i < nCount; i++)
		{
			//等于0表示关着，才可能需要开
			if(m_aStat[i].nStat != 0)
			{
				//不等于0表示已经开着了，不需要再开
				bLaunch[m_aStat[i].nType] = false;
			}
		}

		//不需要保持开启的Server也不要开启
		for(i = SVRTYPE_AGENT+1; i < SVRTYPE_COUNT; i++)
		{
			if(!m_setting.abKeepServer[i])
				bLaunch[i] = false;
		}

		//开启相关的server
		for(i = SVRTYPE_AGENT+1; i < SVRTYPE_COUNT; i++)
		{
			if(bLaunch[i])LaunchServer(i);
		}
	}
}

void CAgentCirculator::RegisterMessage()
{
	GAMEMSG_REGISTERCREATOR(GameMsg_System_Connect);
//	GAMEMSG_REGISTERCREATOR(GameMsg_System_Disconnect);
	
	GAMEMSG_REGISTERCREATOR(AgentMsg_Stat);
	GAMEMSG_REGISTERCREATOR(AgentMsg_SysInfo);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Execute);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Close);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Start);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Setting);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Announce);
	GAMEMSG_REGISTERCREATOR(AgentMsg_ShutdownInTime);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Kill);
	GAMEMSG_REGISTERCREATOR(AgentMsg_Exit);
}

void CAgentCirculator::RegisterMessageProcessor()
{
	RegMsgProcessor(MSG_SYSTEM_Connect,&CAgentCirculator::OnMonitorConnect);
	RegMsgProcessor(MSG_SYSTEM_Disconnect,&CAgentCirculator::OnMonitorDisConnect);

	RegMsgProcessor(MSG_AGENT_Stat,&CAgentCirculator::OnAgentStat);
	RegMsgProcessor(MSG_AGENT_SysInfo,&CAgentCirculator::OnMonitorSysInfo);
	RegMsgProcessor(MSG_AGENT_Execute,&CAgentCirculator::OnMonitorExecute);
	RegMsgProcessor(MSG_AGENT_Close,&CAgentCirculator::OnMonitorCloseServer);
	RegMsgProcessor(MSG_AGENT_Start,&CAgentCirculator::OnMonitorStartServer);
	RegMsgProcessor(MSG_AGENT_Setting,&CAgentCirculator::OnMonitorSetting);
	RegMsgProcessor(MSG_AGENT_Announce,&CAgentCirculator::OnMonitorAnnounce);
	RegMsgProcessor(MSG_AGENT_ShutdownInTime,&CAgentCirculator::OnMonitorShutdownInTime);
	RegMsgProcessor(MSG_AGENT_Kill,&CAgentCirculator::OnMonitorKillServer);
	RegMsgProcessor(MSG_AGENT_Exit,&CAgentCirculator::OnMonitorExit);
}


void CAgentCirculator::ProcessQueryFinished(QueryBase* pQuery)
{
	TRACE("ProcessQueryFinished!\n");
}

void CAgentCirculator::CloseServer(unsigned char bCloseSvr[SVRTYPE_COUNT])
{
	AgentMsg_Close msgClose;
	memcpy(msgClose.bCloseSvr, bCloseSvr, sizeof(msgClose.bCloseSvr));
	for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
	{
		if(m_aStat[i].nType != SVRTYPE_UNKNOWN )
		{
			if( bCloseSvr[0] || bCloseSvr[m_aStat[i].nType])
				m_pSockLocal->SendGameMsg(&msgClose, (unsigned short)i);
		}
	}
}

void CAgentCirculator::LaunchServer(int nSvrType)
{
	if(nSvrType >= SVRTYPE_COUNT)return ;

	std::string strKey(s_szSvrType[nSvrType]);
	strKey += "CmdLine";

	string strProgram;
	g_Config.GetString(g_sSection, strKey.c_str(), strProgram, "");
	if(strProgram.size() > 0)
	{
		if(nSvrType>=SVRTYPE_GAME2&&nSvrType<=SVRTYPE_GAME4)
		{
#ifndef WIN32
			string sGamePathBeg = "";
			string sGamePathEnd = "";
			string::size_type pos = g_sPath.find_last_of('/', g_sPath.size()-2);
			if( pos != string::npos)
			{
				sGamePathBeg = g_sPath.substr(0, pos+1);
				sGamePathEnd = g_sPath.substr(pos+1,g_sPath.size() - (pos+1) -1);
			}

			std::string strPathname;
			Formatter(strPathname) << sGamePathBeg << sGamePathEnd << (nSvrType-2) << "/";
			strProgram = strPathname + strProgram;
#endif
		}
		else
			strProgram = g_sPath + strProgram;
		WriteLog(LOGLEVEL_DEBUG, "Launch %s (%s)\n", s_szSvrType[nSvrType], strProgram.c_str());
		system(strProgram.c_str());
	}
}

void CAgentCirculator::KillServer(int nSvrType)
{
	if(nSvrType >= SVRTYPE_COUNT)return ;

	for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
	{
		if(m_aStat[i].nType == nSvrType)
		{
			if(m_aPID[i] != 0)
			{
				WriteLog(LOGLEVEL_DEBUG, "kill %s\n", s_szSvrType[nSvrType]);
				std::string strCmd;
				Formatter(strCmd) << "kill " << m_aPID[i];
				system(strCmd.c_str());
			}
		}
	}
}

void CAgentCirculator::ReportSysInfo()
{
#ifndef WIN32
	if( GetSysInfo(&m_statSys) )
#endif
	{
		//TRACE("CPU stats: %3.1f user, %3.1f system, %3.1f idle\n", m_statSys.nCPU[0]/10.0f, m_statSys.nCPU[1]/10.0f, m_statSys.nCPU[2]/10.0f);
		//TRACE("Mem: %3.1f \n", m_statSys.nMem/10.0f);
		AgentMsg_SysInfo msgSysInfo;
		msgSysInfo.stat = m_statSys;
		NotifyAllMonitor(&msgSysInfo);
	}
}

void CAgentCirculator::ProcessLocalServerMsg(LPGameMsg_Base pMsg,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	switch(pMsg->nMsg)
	{
	case MSG_SYSTEM_Connect:
		WriteLog(LOGLEVEL_SYSTEM, "Local server connected! [slot %d]\n",nSlot);
		break;
	case MSG_SYSTEM_Disconnect:
		{
			WriteLog(LOGLEVEL_SYSTEM, "Local server disconnected! [ %s on slot %d]\n", s_szSvrType[m_aStat[nSlot].nType], nSlot);
			if(m_aStat[nSlot].nType != SVRTYPE_UNKNOWN)
			{//如果曾经报告过状态则通知状态更新
				m_aPID[nSlot] = 0;
				m_aStat[nSlot].nStat = 0;
				m_aStat[nSlot].nCltCnt = 0;
				m_aStat[nSlot].nPort = 0;
				AgentMsg_Stat msgStat;
				msgStat.stat = m_aStat[nSlot];
				//注意这里发往monitor的消息中 m_aStat[nSlot].nType 存在SVRTYPE_GAME2 SVRTYPE_GAME3 SVRTYPE_GAME4  monotor那需要对应修改
				NotifyAllMonitor(&msgStat);
			}
		}
		break;
	default:
		GameMsg_Map::Instantce().ProcessMsg(*pMsg,slotPeer);
		break;
	}
}

void CAgentCirculator::OnAgentStat(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Stat* pMsg = (AgentMsg_Stat*)&msg;
	WriteLog(LOGLEVEL_SYSTEM, "Local server report stat! [slot %d]\n", nSlot);
	m_aStat[nSlot] = pMsg->stat;
	//因为GameServer启动时的类型都是SVRTYPE_GAME，所以通知到Agent时，需要调整过来，变成SVRTYPE_GAME2 SVRTYPE_GAME3 SVRTYPE_GAME4中的一个
	if(m_aStat[nSlot].nPort >= 7755 && m_aStat[nSlot].nPort <= 7757)
	{
		m_aStat[nSlot].nType = m_aStat[nSlot].nType + (m_aStat[nSlot].nPort - 7754);
		//发往monotor的消息也要改变类型值
		((AgentMsg_Stat*)pMsg)->stat.nType = m_aStat[nSlot].nType;
	}
	m_aPID[nSlot] = pMsg->pid;
	NotifyAllMonitor(pMsg);
}

void CAgentCirculator::OnMonitorConnect(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	WriteLog(LOGLEVEL_SYSTEM, "Monitor connected ! Slot %d, IP %s\n", nSlot, inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));
	AgentMsg_Setting msgSetting;
	msgSetting.setting = m_setting;
	msgSetting.setting.nCheckInterval *= m_nElapseHeartBeat;
	msgSetting.setting.nSysInfoInterval *= m_nElapseHeartBeat;
	m_pSocketMgr->SendGameMsg(&msgSetting, nSlot);

	for(int i = SVRTYPE_AGENT+1; i < SVRTYPE_COUNT; i++)
	{
		AgentMsg_Stat msgStat;
		std::string strKey("keep");
		strKey.append(s_szSvrType[i]);
		if( g_Config.GetInt(g_sSection, strKey.c_str(), 0) == 0)
			msgStat.stat.nStat = 2;//表示这个server不需要启动
		msgStat.stat.nType = i;
		m_pSocketMgr->SendGameMsg(&msgStat, nSlot);
	}
}

void CAgentCirculator::OnMonitorDisConnect(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	WriteLog(LOGLEVEL_SYSTEM, "Monitor Disconnect! Slot %d, IP %s\n", nSlot, inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));
}

void CAgentCirculator::OnMonitorSysInfo(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	ReportSysInfo();
}

void CAgentCirculator::OnMonitorExecute(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	AgentMsg_Execute *pExe = (AgentMsg_Execute *) &msg;
	WriteLog(LOGLEVEL_SYSTEM, "Monitor execute command %s\n", pExe->strCmdLine.c_str());
	if (strcmp(pExe->strCmdLine.c_str(), "#RestartAgent") == 0)
	{
		std::string strCmd(g_sPath);
		strCmd.append("bin/hotdance restart");
		system(strCmd.c_str());
		exit(0);
	}
	else{
		system( pExe->strCmdLine.c_str() );
	}
}

void CAgentCirculator::OnMonitorCloseServer(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Close from %s\n", inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));
	AgentMsg_Close *pMsgClose = (AgentMsg_Close*)&msg;

	std::string strBuffer;
	pMsgClose->Serialize(strBuffer);
	WriteLog(LOGLEVEL_SYSTEM, strBuffer.c_str());

	CloseServer(pMsgClose->bCloseSvr);
}

void CAgentCirculator::OnMonitorStartServer(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Start from %s\n", inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));
	AgentMsg_Start *pMsg = (AgentMsg_Start*)&msg;

	std::string strBuffer;
	pMsg->Serialize(strBuffer);
	WriteLog(LOGLEVEL_SYSTEM, strBuffer.c_str());

	for(int i = 1; i < SVRTYPE_COUNT; i++)
	{
		if (pMsg->bStartSvr[0] || pMsg->bStartSvr[i])
		{
			LaunchServer(i);
		}
	}
}

void CAgentCirculator::OnMonitorKillServer(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Kill from %s\n", inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));
	AgentMsg_Kill *pMsg = (AgentMsg_Kill*)&msg;

	std::string strBuffer;
	pMsg->Serialize(strBuffer);
	WriteLog(LOGLEVEL_SYSTEM, strBuffer.c_str());

	for(int i = 1; i < SVRTYPE_COUNT; i++)
	{
		if (pMsg->bSvr[0] || pMsg->bSvr[i])
		{
			KillServer(i);
		}
	}
}

void CAgentCirculator::OnMonitorSetting(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Setting *pMsg = (AgentMsg_Setting*)&msg;

	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Setting from %s, ReLoadSetting=[%s]\n", 
		inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr), pMsg->bRestore ? "true" : "false");

	if (pMsg->bRestore)
	{
		LoadSetting();
	}
	else
	{
		m_setting = pMsg->setting;
		m_setting.nCheckInterval /= m_nElapseHeartBeat;
		m_setting.nSysInfoInterval /= m_nElapseHeartBeat;
	}

	std::string strSetting;
	SerializeSetting(strSetting);
	WriteLog(LOGLEVEL_SYSTEM, strSetting.c_str());

	AgentMsg_Setting msgSetting;
	msgSetting.setting = m_setting;
	msgSetting.setting.nCheckInterval *= m_nElapseHeartBeat;
	msgSetting.setting.nSysInfoInterval *= m_nElapseHeartBeat;
	m_pSocketMgr->SendGameMsg(&msgSetting, nSlot);
}

void CAgentCirculator::OnMonitorAnnounce(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Announce *pMsg = (AgentMsg_Announce*)&msg;

	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Announce from %s, Announce Content=[%s]\n", 
		inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr), pMsg->szContent.c_str());
	
	for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
	{
		if(m_aStat[i].nType >= SVRTYPE_GAME && m_aStat[i].nType <= SVRTYPE_GAME4 && m_aStat[i].nStat == 1)
		{
            m_pSockLocal->SendGameMsg(&msg, (unsigned short)i);
		}
	}
}

void CAgentCirculator::OnMonitorShutdownInTime(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_ShutdownInTime *pMsg = (AgentMsg_ShutdownInTime*)&msg;

	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_ShutdownInTime from %s, shutdown In [%d] seconds\n", 
		inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr), pMsg->nSeconds);

	for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
	{
		if(m_aStat[i].nType != SVRTYPE_UNKNOWN && m_aStat[i].nStat == 1)
		{
            m_pSockLocal->SendGameMsg(&msg, (unsigned short)i);
		}
	}
}

void CAgentCirculator::OnMonitorExit(GameMsg_Base & msg ,CSlotPeer & slotPeer)
{
	unsigned short nSlot = slotPeer.GetSlot();
	AgentMsg_Exit *pMsg = (AgentMsg_Exit*)&msg;

	WriteLog(LOGLEVEL_SYSTEM, "Recv AgentMsg_Exit from %s\n", 
		inet_ntoa(m_pSocketMgr->GetSlot(nSlot)->addr));

	std::string strBuffer;
	pMsg->Serialize(strBuffer);
	WriteLog(LOGLEVEL_SYSTEM, strBuffer.c_str());

	for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
	{
		if(m_aStat[i].nType == SVRTYPE_ACCOUNT && m_aStat[i].nStat == 1 && m_pSockLocal != NULL)
		{
            m_pSockLocal->SendGameMsg(&msg, (unsigned short)i);
		}
	}
}

//void CAgentCirculator::ProcessSocketMsg(LPGameMsg_Base pGameMsg, unsigned short &nSlot)
//{
//	case MSG_AGENT_Stat:
//		{
//			AgentMsg_Stat msgStat;
//			for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
//			{
//				if(m_aStat[i].nType != SVRTYPE_UNKNOWN)
//				{
//					msgStat.stat = m_aStat[i];
//					m_pSocketMgr->SendGameMsg(&msgStat, nSlot);
//				}
//			}
//		}
//	
//}

#ifndef WIN32
bool CAgentCirculator::GetSysInfo(STAT_SYS* pInfo)
{
#define PROC_BUFF_SIZE 256
	char szBuff[PROC_BUFF_SIZE];
	char *ptr;
	bool bRes = true;

	if (bRes)
	{
		//Get CPU stat
		FILE* fp = fopen("/proc/stat", "r");
		if(fp != NULL)
		{
			memset(szBuff, 0, PROC_BUFF_SIZE);
			fgets(szBuff, PROC_BUFF_SIZE, fp);
			fclose(fp);
			static __int64 s_nValueLast[4] = {0};
			__int64 nValue[4];
			__int64 nTotal = 0;
			int nCount = 0;
			for(ptr = szBuff+1; *ptr != 0; ptr++)
			{
				if( *(ptr-1) == ' ' && *ptr != ' ')
				{
					nValue[nCount] = strtoull(ptr, &ptr, 0) - s_nValueLast[nCount];
					s_nValueLast[nCount] += nValue[nCount];
					nTotal += nValue[nCount];
					nCount++;
					if(nCount == 4)break;
				}
			}
			if(nCount == 4)
			{
				pInfo->nCPU[0] = (unsigned short)((nValue[0] * 1000.0f) / nTotal);
				pInfo->nCPU[1] = (unsigned short)((nValue[2] * 1000.0f) / nTotal);
				pInfo->nCPU[2] = (unsigned short)((nValue[3] * 1000.0f) / nTotal);
			}
			else
			{
				bRes = false;
			}
		}
		else
		{
			bRes = false;
		}
	}

	if(bRes)
	{
		__int64 nSize = 0;
		for(unsigned int i = 0; i < m_pSockLocal->GetMaxClientCount(); i++)
		{
			if(m_aStat[i].nType != SVRTYPE_UNKNOWN)
			{
				nSize += GetProcessMem(m_aPID[i]);
			}
		}
		nSize += GetProcessMem(getpid());
		__int64 nTotal = 0;
		nTotal = GetSystemMem();

		pInfo->nMemUsed = (unsigned short)(nSize/1024);
		pInfo->nMemLeft = (unsigned short)(nTotal/1024);

	}

	return bRes;
}

__int64 GetProcessMem(int pid)
{
#ifndef PROC_BUFF_SIZE
#define PROC_BUFF_SIZE 256
#endif
	__int64 nSize = 0;
	std::string strBuff;
	Formatter::Format(strBuff, "/proc/%d/statm", pid);
	FILE* fp = fopen(strBuff.c_str(), "r");
	if(fp != NULL)
	{
		char szBuff[PROC_BUFF_SIZE];
		memset(szBuff, 0, PROC_BUFF_SIZE);
		fgets(szBuff, PROC_BUFF_SIZE, fp);
		fclose(fp);
		char* ptr = szBuff;
		nSize += strtoull(ptr, &ptr, 0);
		for(; *ptr != 0; ptr++)
		{
			if( *(ptr-1) == ' ' && *ptr != ' ')
			{
				nSize += strtoull(ptr, &ptr, 0);
			}
		}
	}

	return nSize;
}

__int64 GetSystemMem()
{
	__int64 nSize = 0;
	FILE* fp = fopen("/proc/meminfo", "r");
	if(fp != NULL)
	{
		char szBuff[PROC_BUFF_SIZE];
		memset(szBuff, 0, PROC_BUFF_SIZE);
		fgets(szBuff, PROC_BUFF_SIZE, fp);
		memset(szBuff, 0, PROC_BUFF_SIZE);
		fgets(szBuff, PROC_BUFF_SIZE, fp);
		fclose(fp);
		char *ptr;
		for(ptr = szBuff+1; *ptr != 0; ptr++)
		{
			if( *(ptr-1) == ' ' && *ptr != ' ')
			{
				nSize = strtoull(ptr, &ptr, 0);
				break;
			}
		}
	}

	return nSize;
}
#endif

void CAgentCirculator::NotifyAllMonitor(LPGameMsg_Base pMsg)
{
	for(unsigned int i = 0; i < m_pSocketMgr->GetMaxClientCount(); i++)
	{
        m_pSocketMgr->SendGameMsg(pMsg, (unsigned short)i);
	}
}

//end file


