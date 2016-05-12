#ifdef WIN32
#include <process.h>
#else

#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "CirculatorBase.h"
#include "../../socket/inc/SocketLib.h"
#include "../../socket/Formatter.h"
#include "Main.h"
#include "../share/ServerDataStruct.h"
#include "../share/GameMsg_Cmd.h"
#include "../share/GameMsg_CmdRes.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_System_Slot.h"
#include "../../socket/GameMsg_System_Ping.h"
#include "../../socket/GameMsg_System_Packet.h"
#include "../../socket/GameMsg_System_Invalid.h"
#include "../../socket/GameMsg_System_Connect.h"
#include "../../socket/GameMsg_System_Disconnect.h"
#include <string>
#include <vector>
using namespace std;

#ifdef _GameMsg_MEMPOOL
#include "MemPool.h"
extern CMemPoolMgr * g_pMemPoolMgrGameMsg;
#endif


CSocketMgr*     g_pSocketMgr = NULL;


#ifdef WIN32
DWORD CCirculatorBase::ThreadProc(void* pParam)
{
    CCirculatorBase* pCirculator = (CCirculatorBase*)pParam;
    pCirculator->m_dwTickLast = GetTickCount();

    while (pCirculator->m_bRunning) 
    {
        WaitForSingleObject(pCirculator->m_hEventNotify, pCirculator->m_nElapseHeartBeat);
        if(!pCirculator->m_bRunning)break;

        pCirculator->PumpProc();
    }

    TRACE("Game thread exited!\n");
    return 0;
}
#else
void* CCirculatorBase::ThreadProc(void* pParam)
{
    CCirculatorBase* pCirculator = (CCirculatorBase*)pParam;

    timeval now;
    struct timezone tz;
    timespec timeout;
    gettimeofday(&pCirculator->m_timeLast, &tz);
    while (pCirculator->m_bRunning) 
    {
        gettimeofday(&now, &tz);
        timeout.tv_sec = now.tv_sec;
        timeout.tv_nsec = (now.tv_usec + 1000) << 10;

        pthread_mutex_lock(&pCirculator->m_mutexNotify);
        pthread_cond_timedwait(&pCirculator->m_condNotify, &pCirculator->m_mutexNotify, &timeout);
        pthread_mutex_unlock(&pCirculator->m_mutexNotify);

        pCirculator->PumpProc();

    }
    WriteLog(LOGLEVEL_DEBUG, "Game thread exited!\n");
    return NULL;
}
#endif

int g_LastDataTick = 0;
int g_nFrame = 0;
int g_nFPS = 0;

void CCirculatorBase::PumpProc()
{
    LPGameMsg_Base pMsg = NULL;
    unsigned short nSlot;
    int nCount = 0;

    //process game messages from clients
    if (m_pSocketMgr)
    {
#ifndef WIN32
        unsigned int nBegin = GetTickCount();
#endif
        nCount = 0;
        while (m_pSocketMgr->GetGameMsg(pMsg, nSlot))
        {
            m_SlotPeer->SetSlot(nSlot);
            GameMsg_Map::Instantce().ProcessMsg(*pMsg, *m_SlotPeer);
            delete pMsg;
            nCount++;
        }
#ifndef WIN32
        unsigned int nEnd = GetTickCount();
        ASSERT(nEnd - nBegin < 100);
        ASSERT(nEnd - nBegin < 500);
        if( nEnd - nBegin > 500)
        {
            TRACE("MsgCount=%d, Process time=%d\n", nCount, nEnd-nBegin);
        }
#endif
    }

#ifdef WIN32
    DWORD dwTickNow = GetTickCount();
    int timeElapse = dwTickNow - m_dwTickLast;
    if(timeElapse >= m_nElapseHeartBeat)
    {
        ProcessHeartBeat(timeElapse);
        m_dwTickLast = dwTickNow;
    }
#else
    // process timer(heart beat)
    timeval now;
    struct timezone tz;
    gettimeofday(&now, &tz);
    int timeElapse = (now.tv_sec - m_timeLast.tv_sec) * 1000 +  (now.tv_usec - m_timeLast.tv_usec) / 1000 ;
    //补偿微秒取毫秒导致的累计误差
    static int usecLast = 0;

    if (timeElapse >= m_nElapseHeartBeat)
    {
        usecLast += (now.tv_usec - m_timeLast.tv_usec)%1000;
        while(usecLast >= 1000)
        {
            usecLast -= 1000;
            ++timeElapse;
        }
        ProcessHeartBeat(timeElapse);
        m_timeLast = now;
    }
#endif
}

CCirculatorBase::CCirculatorBase() : GameMsg_Processor(true,true)
{
    m_nElapseHeartBeat = 25;
    m_nHeartBeatCount = -1;
    m_pSocketMgr = NULL;

    m_bRunning = false;

    m_SlotPeer = NULL;
#ifdef WIN32
    std::string strEventName;
    Formatter(strEventName) << "CirculatorEvent" << (int)this << GetTickCount();
    m_hEventNotify = CreateEvent(NULL, FALSE, FALSE, strEventName.c_str());
#else
    pthread_mutex_init(&m_mutexNotify, NULL);
    pthread_cond_init(&m_condNotify, NULL);
#endif
}

CCirculatorBase::~CCirculatorBase()
{
    if (m_bRunning)
        Stop();

    delete m_pSocketMgr;
    m_pSocketMgr = NULL;

    if (m_SlotPeer)
    {
        delete m_SlotPeer;
        m_SlotPeer = NULL;
    }

#ifdef WIN32
    CloseHandle(m_hEventNotify);
#else
    pthread_mutex_destroy(&m_mutexNotify);
    pthread_cond_destroy(&m_condNotify);
#endif

#ifdef _GameMsg_MEMPOOL
    delete g_pMemPoolMgrGameMsg;
    g_pMemPoolMgrGameMsg = NULL;
#endif

}

#define CLIENT_PER_SEND_THREAD	256
#define CLIENT_PRE_RECV_THREAD	512
bool CCirculatorBase::InitSocketMgr(CSocketMgr* &pSocketMgr, unsigned short nCount, short nPort )
{
    bool bRes = false;
    pSocketMgr = new CSocketMgr;
    int nSendGroup = (nCount + CLIENT_PER_SEND_THREAD - 1) / CLIENT_PER_SEND_THREAD;
    int nRecvGroup = (nCount + CLIENT_PRE_RECV_THREAD - 1) / CLIENT_PRE_RECV_THREAD;
#ifdef WIN32
    nSendGroup = 1;
    nRecvGroup = 1;
#endif
    if (pSocketMgr != NULL && pSocketMgr->Init(nPort, nCount, nSendGroup, nRecvGroup))
    {
        WriteLog(LOGLEVEL_SYSTEM, "Initialize network engine successfully.\n");
        bRes = true;
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Initialize network engine failed!!\n");
        delete pSocketMgr;
        pSocketMgr = NULL;
        bRes = false;
    }
    return bRes;
}

bool CCirculatorBase::InitClientSock(CClientSocketMgr* &pClientSock)
{
    bool bRes = false;
    pClientSock = new CClientSocketMgr;
    if (pClientSock != NULL &&
#ifdef WIN32
        pClientSock->Init(NULL, m_hEventNotify)
#else
        pClientSock->Init(&m_condNotify, &m_mutexNotify)
#endif
        )
    {
        bRes = true;
    }
    else
    {
        delete pClientSock;
        pClientSock = NULL;
    }

    return bRes;
}

bool CCirculatorBase::Initialize()
{
#ifdef _GameMsg_MEMPOOL
    g_pMemPoolMgrGameMsg = new CMemPoolMgr;
    g_pMemPoolMgrGameMsg->Initialize();
#endif

    extern CIni g_Config;
    m_nElapseHeartBeat = g_Config.GetInt("interval", "heartbeat", 25);

    // 这里开放的socket端口，为各服务器对外开放的不同服务，agent开放的是6313端口，供monitor工具连接，
    // AccountServer开放的是7753，供GateServer连接的端口，GameServer开放的是7754供GateServer连接的端口，
    // GroupServer开放的是7758，供GameServer连接
    extern int g_nPort;
    short nMaxClient = (short)g_Config.GetInt(g_sSection, "maxclientcount", 32);
    short nPort = (short)g_Config.GetInt(g_sSection, "serverport", 7754);

    //如果启动项参数指定了特定端口，则使用指定的端口，作用是在一台服务器上能够用不同端口启动GameServer
    if (g_nPort >= 0 && strcmp(g_sSection, "GameServer") == 0)
    {
        nPort = (short)g_nPort;
    }

    bool bRes = InitSocketMgr(m_pSocketMgr, nMaxClient, nPort);
    if (bRes)
    {
        g_pSocketMgr = m_pSocketMgr;
    }

    m_SlotPeer = new CSlotPeer();
    m_SlotPeer->SetSocketMgr(m_pSocketMgr);

    CirculatorRegisterMsg();

    return bRes;
}

void CCirculatorBase::Release()
{
}

void CCirculatorBase::CirculatorRegisterMsg()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Slot);
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Ping);
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Packet);
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Invalid);
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Connect);
    GAMEMSG_REGISTERCREATOR(GameMsg_System_Disconnect);
}

bool CCirculatorBase::Start(bool bCreateThread)
{
    WriteLog(LOGLEVEL_SYSTEM, "========== Server starting...==========");

    if (!Initialize())
    {
        WriteLog(LOGLEVEL_ERROR, "Circulator Initialize() failed!");
        return false;
    }

    m_bRunning = true;
    if (bCreateThread)
    {
#ifdef WIN32
        m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
        int nRes =  m_hThread == NULL ? 1 : 0;
#else
        int nRes = pthread_create(&m_thread, NULL, ThreadProc, this);
#endif
        if (nRes != 0)
        {
            WriteLog(LOGLEVEL_ERROR, "Create game thread failed!\n");
            m_bRunning = false;
        }
    }
    else
    {
        ThreadProc(this);
    }

    return m_bRunning;
}

void CCirculatorBase::Stop()
{
    if (m_bRunning)
    {
#ifdef WIN32
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
#else

        pthread_join(m_thread, NULL);
#endif

        m_bRunning = false;

#ifdef WIN32
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
#else

        pthread_join(m_thread, NULL);
#endif
        Release();
    }
}

void CCirculatorBase::ProcessHeartBeat(int& timeElapse)
{
    m_nHeartBeatCount++;
}

