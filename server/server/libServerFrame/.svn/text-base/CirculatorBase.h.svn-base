#ifndef __CIRCULATOR_BASE_H__
#define __CIRCULATOR_BASE_H__

#include "../../socket/inc/SocketLib.h"
using namespace ZeroSocket;

#include "../share/ServerDataStruct.h"
#include "../../datastructure/DataStruct_Agent.h"
#include "../../socket/GameMsg_Processor.h"


class CCirculatorBase:public GameMsg_Processor
{
public:
    CCirculatorBase();
    virtual ~CCirculatorBase();
public:

    //Start logic circle
    bool Start(bool bCreateThread = true);
    //Start logic circle
    void Stop();

protected:
    // Initialize game logic, load game data...
    virtual bool Initialize();

    //release game logic	
    virtual void Release();

    //server step
    virtual void PumpProc(); 

    //process heart beat
    virtual void ProcessHeartBeat(int& timeElapse);

    bool InitSocketMgr(CSocketMgr* &pSockMgr, unsigned short nCount, short nPort );
    bool InitClientSock(CClientSocketMgr* &pClientSock);

    void CirculatorRegisterMsg();

#ifdef WIN32
    static DWORD WINAPI ThreadProc(void* pParam);
#else
    static void* ThreadProc(void* pParam);
#endif

protected:
    bool m_bRunning;
    int m_nElapseHeartBeat;
    int m_nHeartBeatCount;

#ifdef WIN32
    void* m_hThread;
    unsigned long m_dwTickLast;
    void* m_hEventNotify;
#else

    pthread_t m_thread;
    pthread_mutex_t m_mutexNotify;
    pthread_cond_t m_condNotify;
    //Last heart beat time point
    timeval m_timeLast;
#endif

public:
    CSocketMgr * m_pSocketMgr;
    CSlotPeer * m_SlotPeer;
};

extern CSocketMgr*      g_pSocketMgr;

#endif//__CIRCULATOR_BASE_H__

