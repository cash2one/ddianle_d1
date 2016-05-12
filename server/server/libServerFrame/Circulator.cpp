#ifdef WIN32
#include <process.h>
#else

#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <string>
#include <vector>
using namespace std;


#include "Circulator.h"
#include "QueryManager.h"
#include "../share/ServerDataStruct.h"
#include "Main.h"
#include "../../socket/DBQuery_Map.h"
#include "../agent/AgentMsg_Stat.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_Map.h"
#ifdef _GameMsg_MEMPOOL
#include "MemPool.h"
extern CMemPoolMgr * g_pMemPoolMgrGameMsg;
#endif

CQueryManager*	g_pQueryMgr = NULL;

bool g_bTermByKill = false;
void sig_kill(int signum)
{
    g_bTermByKill = true;
}

void CCirculator::PumpProc()
{
    CCirculatorBase::PumpProc();

    if (g_bTermByKill)
    {
        WriteLog(LOGLEVEL_SYSTEM, "Terminate by kill!\n");
        EnterExitProcess();
    }

    LPGameMsg_Base pMsg = NULL;
    if (m_pAgent)
    {
        while ((pMsg = m_pAgent->GetGameMsg()) != NULL)
        {
            if (!ProcessAgentMsg(pMsg))
            {
                GameMsg_Map::Instantce().ProcessMsg(*pMsg, *m_AgnetSlotPeer);
            }

            delete pMsg;
        }
    }

    //process query finished from database engine
    if (m_pQueryMgr)
    {
        QueryBase* pQueryFinished = NULL;
        while ((pQueryFinished = m_pQueryMgr->GetQueryFinished()) != NULL)
        {
            ProcessQueryFinished(pQueryFinished);

            delete pQueryFinished;
        }
    }

    if (m_bExiting)//如果正在退出流程
    {
        if ((m_pQueryMgr && m_pQueryMgr->IsQueueEmpty()) || m_pQueryMgr == NULL)//查询都处理完成
        {
            //再等待3秒
            millisecondSleep(3000);
            m_bRunning = false;
        }
    }
}

CCirculator::CCirculator()
{
    m_pQueryMgr = NULL;
    m_pAgent = NULL;
    m_stat.nStat = 1;
    m_stat.nType = SVRTYPE_UNKNOWN;
    m_nIntervalReportStat = 3000 / m_nElapseHeartBeat;
    m_nIntervalReconnectAgent = 6000 / m_nElapseHeartBeat;

    m_bExiting = false;
    m_bShutdownInTime = false;
    m_timeShutdown = 0;
    m_AgnetSlotPeer = NULL;


    //注册消息响应:
    //RegMsgProcessor()
}

CCirculator::~CCirculator()
{
    if (m_bRunning)
        Stop();

    delete m_pQueryMgr;
    m_pQueryMgr = NULL;

    delete m_pAgent;
    m_pAgent = NULL;

}

bool CCirculator::Initialize()
{
#ifdef WIN32
#else
    signal(SIGTERM, sig_kill);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
#endif

    bool bRes = CCirculatorBase::Initialize();
    if (bRes && (m_stat.nType == SVRTYPE_ACCOUNT || m_stat.nType == SVRTYPE_GAME || m_stat.nType == SVRTYPE_GROUP))
    {
        m_pQueryMgr = new CQueryManager;
        if (m_pQueryMgr != NULL &&
#ifdef WIN32
            m_pQueryMgr->Initialize(m_hEventNotify)
#else
            m_pQueryMgr->Initialize(&m_condNotify, &m_mutexNotify)
#endif
            && m_pQueryMgr->OpenDatabase(
            m_stat.nType == SVRTYPE_ACCOUNT || m_stat.nType == SVRTYPE_GAME || m_stat.nType == SVRTYPE_GROUP,
            m_stat.nType == SVRTYPE_ACCOUNT || m_stat.nType == SVRTYPE_GAME || m_stat.nType == SVRTYPE_GROUP,
            m_stat.nType == SVRTYPE_GAME || m_stat.nType == SVRTYPE_GROUP,
            m_stat.nType == SVRTYPE_GAME || m_stat.nType == SVRTYPE_GROUP)
            )
        {
            WriteLog(LOGLEVEL_SYSTEM, "Connect to database successully");
            g_pQueryMgr = m_pQueryMgr;
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "Connect to database failed!");
            bRes = false;
        }
    }

    extern CIni g_Config;

    m_stat.nVersion = g_Config.GetInt(g_sSection, "ver", 0);

    m_nIntervalReportStat = g_Config.GetInt("interval", "reportstat", 3000) / m_nElapseHeartBeat;
    m_nIntervalReconnectAgent = g_Config.GetInt("interval", "reconnectagent", 6000) / m_nElapseHeartBeat;

#ifndef WIN32
    if (bRes && m_stat.nType != SVRTYPE_AGENT) //不是Agent则连接Agent
    {
        unsigned short nPort = g_Config.GetInt(s_szSvrType[SVRTYPE_AGENT], "internalport", 7762);
        bRes = InitClientSock(m_pAgent);
        if (bRes)
        {
            m_pAgent->Connect(NULL, "127.0.0.1", nPort);
            //Debug模式运行时连接Agent失败可以继续运行
        }
    }

#endif

    m_AgnetSlotPeer = new CSlotPeer();
    m_AgnetSlotPeer->SetSocketMgr(m_pAgent);

    return bRes;
}

void CCirculator::Release()
{
    if (m_AgnetSlotPeer)
    {
        delete m_AgnetSlotPeer;
        m_AgnetSlotPeer = NULL;
    }
}

void CCirculator::EnterExitProcess()
{
    m_bExiting = true;//进入退出流程
    m_pSocketMgr->StopListen();//停止监听，不再接受客户端连接
    m_pSocketMgr->DisconnectAllClient();//断开所有当前的连接
}

bool CCirculator::ProcessAgentMsg(LPGameMsg_Base pMsg)
{
    bool bResult = true;
    switch (pMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        WriteLog(LOGLEVEL_DEBUG, "Agent Connected\n");
        ReportStat();
        break;
    case MSG_SYSTEM_Disconnect:
        WriteLog(LOGLEVEL_DEBUG, "Agent Disconnected\n");
        break;
    default:
        bResult = false;
        break;
    }
    return bResult;
}

void CCirculator::ProcessQueryFinished(QueryBase* pQuery)
{
    CDBQuery_Map::Instantce().ProcessMsg(*pQuery);
}

void CCirculator::ReportStat()
{
    //m_stat.nCltCnt = m_pSocketMgr->GetCurClientCount();
    m_stat.nPort = m_pSocketMgr->GetListenPort();

    AgentMsg_Stat msgStat;
    msgStat.stat = m_stat;
    msgStat.pid = getpid();
    m_pAgent->SendGameMsg(&msgStat);
}

void CCirculator::ProcessHeartBeat(int& timeElapse)
{
    CCirculatorBase::ProcessHeartBeat(timeElapse);

    if (m_pAgent != NULL
        && m_stat.nType != SVRTYPE_UNKNOWN
        && (m_nHeartBeatCount % m_nIntervalReportStat == 0))
    {
        ReportStat();
    }

    if (m_pAgent != NULL
        && m_pAgent->GetState() != NETWORK_STATE_CONNECTED
        && (m_nHeartBeatCount % m_nIntervalReconnectAgent == 0))
    {
        WriteLog(LOGLEVEL_DEBUG, "ReconnectAgent\n");
        m_pAgent->Reconnect();
    }

    if (m_bShutdownInTime && m_stat.nType != SVRTYPE_GAME)
    {
        time_t now = time(NULL);
        if (m_timeShutdown - now < 0)
        {
            EnterExitProcess();
        }
    }
}
