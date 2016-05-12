// SocketMgr.cpp: implementation of the CSocketMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "SocketMgr.h"
#include "Windefine.h"
#include "Utility.h"
#include "GameMsg_System_Packet.h"
#include "GameMsg_System_Slot.h"
#include "GameMsg_System_Invalid.h"
#include "GameMsg_System_Connect.h"
#include "Log.h"

#include <algorithm>
#ifdef _GAMEMSG_MEMPOOL
#include "MemPool.h"
#endif

// socket 底层的bug。直接屏蔽。
#ifdef WIN32
#pragma warning(disable:4389 4548 4296)
#endif

#ifdef WIN32
#else
#include <cassert>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#endif


#define MAX_SPARE_FD  32

#ifndef _TEST_NETWORK
#define _TEST_NETWORK   0
#endif

#ifdef WIN32
#define WaitThread(x)	{WaitForSingleObject(x, INFINITE);CloseHandle(x);}
#define Notify()		{SetEvent(m_hEventNotify);}
#define DoSend()		{SetEvent(m_hEventSend);}
#else
#define WaitThread(x)	pthread_join(x, NULL)
#define Notify()		{pthread_mutex_lock(m_pMutexNotify);pthread_cond_signal(m_pCondNotify);pthread_mutex_unlock(m_pMutexNotify);}
#define DoSend()		{/*pthread_mutex_lock(&m_lockSend); */pthread_cond_signal(&m_condSend);/*pthread_mutex_unlock(&m_lockSend);*/}
#endif

using namespace ZeroSocket;

CSocketMgr::CSocketMgr()
{

    m_pFuncEncrypt = NULL;
    m_pFuncDecrypt = NULL;

    m_pAddMgr = NULL;

    m_nPort = 0;
    m_nMaxClientCount = 0;
    m_nCurClientCount = 0;
    m_hSocket = INVALID_SOCKET;
    m_aSlot = NULL;
    m_aKeyEncrypt = NULL;
    m_aKeyDecrypt = NULL;

    m_bRunning = false;

#ifdef WIN32
    m_hEventNotify = NULL;
#else
    m_pCondNotify = NULL;
    m_pMutexNotify = NULL;
#endif

    m_nMsgCount = 0;
    m_nMsgIndex = 0;
}

CSocketMgr::~CSocketMgr()
{
    m_bRunning = false;
    if (m_hSocket != INVALID_SOCKET)
    {
        closesocket(m_hSocket);
    }

    m_threadListen.Wait();

    while (m_aSendGroup.size())
    {
        delete m_aSendGroup.front();
        m_aSendGroup.pop_front();
    }
    while (m_aRecvGroup.size())
    {
        delete m_aRecvGroup.front();
        m_aRecvGroup.pop_front();
    }

    if (m_aSlot != NULL)
    {
        delete[] m_aSlot;
        m_aSlot = NULL;
    }

    if (m_aKeyEncrypt != NULL)
    {
        delete[] m_aKeyEncrypt;
        m_aKeyEncrypt = NULL;
    }
    if (m_aKeyDecrypt != NULL)
    {
        delete[] m_aKeyDecrypt;
        m_aKeyDecrypt = NULL;
    }

    if (m_nMsgCount > m_nMsgIndex)
    {
        for (; m_nMsgIndex < m_nMsgCount; m_nMsgIndex++)
        {
            m_msgBuffer[m_nMsgIndex].clear();
        }
    }
}

#ifdef WIN32
void CSocketMgr::SetNotifyEvent(HANDLE hEventNotify)
{
    m_hEventNotify = hEventNotify;
}
#else
void CSocketMgr::SetNotifyEvent(pthread_cond_t* Cond, pthread_mutex_t* Mutex)
{
    m_pCondNotify = Cond;
    m_pMutexNotify = Mutex;
}
#endif

bool CSocketMgr::Init(unsigned short nPort, unsigned short nMaxClientCount, unsigned int nSendGroupCount /* = 1 */, unsigned int nRecvGroupCount /* = 1 */)
{
#ifndef WIN32
    signal(SIGPIPE, sig_empty);
#endif

    //初始化内存池
#ifdef _GAMEMSG_MEMPOOL
    extern CMemPoolMgr* g_pMemPoolMgrGameMsg;
    if (g_pMemPoolMgrGameMsg == NULL)
    {
        g_pMemPoolMgrGameMsg = new CMemPoolMgr;
        g_pMemPoolMgrGameMsg->Initialize();
    }
#endif

    int nRes;

    //生成客户数据
    m_nMaxClientCount = nMaxClientCount > FD_SETSIZE ? FD_SETSIZE : nMaxClientCount;

#ifdef WIN32
    WSADATA		wsaData;
    WSAStartup(WINSOCK_VERSION, &wsaData);
#else
    //设置文件描述符上限
    struct rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    if (m_nMaxClientCount + MAX_SPARE_FD > rlim.rlim_cur)rlim.rlim_cur = m_nMaxClientCount + MAX_SPARE_FD;
    if (m_nMaxClientCount + MAX_SPARE_FD > rlim.rlim_max)rlim.rlim_max = m_nMaxClientCount + MAX_SPARE_FD;
    setrlimit(RLIMIT_NOFILE, &rlim);
    getrlimit(RLIMIT_NOFILE, &rlim);

    if (m_nMaxClientCount > rlim.rlim_cur - MAX_SPARE_FD)
        m_nMaxClientCount = rlim.rlim_cur - MAX_SPARE_FD;
#endif//WIN32

    m_nPort = nPort;
    TRACE("Server service port %d\n", m_nPort);

    m_aSlot = new SLOT[m_nMaxClientCount];
    if (m_aSlot == NULL) return false;
    m_aKeyEncrypt = new CCryptKey[m_nMaxClientCount];
    m_aKeyDecrypt = new CCryptKey[m_nMaxClientCount];
    for (unsigned int i = 0; i < m_nMaxClientCount; i++)
    {
        m_aSlot[i].SetSlotIndex((unsigned short)i);
    }

    //监听套接字
    struct sockaddr_in saServer;
    m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_hSocket < 0)
    {
        TRACE("socket failed!\n");
        return false;
    }

    int optval = 1;
    setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(nPort);

    // Bind the server socket 
    nRes = bind(m_hSocket, (sockaddr*)&saServer, sizeof(saServer));
    if (nRes != 0)
    {
        TRACE("bind port %d failed!.\n", nPort);
        return false;
    }
    nRes = listen(m_hSocket, QUEUE_SIZE);
    if (nRes != 0)
    {
        TRACE("listen on port %d failed!\n", nPort);
        return false;
    }

#ifdef WIN32
    unsigned long flag = 1;
    ioctlsocket(m_hSocket, FIONBIO, &flag);
#else
    int flag = 1;
    flag = fcntl(m_hSocket, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(m_hSocket, F_SETFL, flag);
#endif

    m_bRunning = true;

    //将客户分组，分组方案
    SetGroup(nSendGroupCount, nRecvGroupCount);
    m_nFindSlotStart = 0;

    m_bRunning = m_threadListen.Create(ListenThreadProc, this);

    return m_bRunning != 0;
}

void CSocketMgr::SetGroup(unsigned int nSendGroupCount, unsigned int nRecvGroupCount)
{
    unsigned int nCountPerGroup = (m_nMaxClientCount + nSendGroupCount - 1) / nSendGroupCount;//每组的客户数
    unsigned int nRemain = m_nMaxClientCount;
    while (nRemain > 0)
    {
        unsigned int nSlotCount;
        if (nRemain >= nCountPerGroup)
            nSlotCount = nCountPerGroup;
        else
            nSlotCount = nRemain;

        SlotSendGroup *pGroup = new SlotSendGroup(m_aSlot + m_nMaxClientCount - nRemain, nSlotCount, &m_bRunning);
        m_aSendGroup.push_back(pGroup);
        nRemain -= nSlotCount;
    }

    nCountPerGroup = (m_nMaxClientCount + nRecvGroupCount - 1) / nRecvGroupCount;//每组的客户数
    nRemain = m_nMaxClientCount;
    while (nRemain > 0)
    {
        unsigned int nSlotCount;
        if (nRemain >= nCountPerGroup)
            nSlotCount = nCountPerGroup;
        else
            nSlotCount = nRemain;

        SlotRecvGroup *pGroup = new SlotRecvGroup(m_aSlot + m_nMaxClientCount - nRemain, nSlotCount, &m_bRunning);
        m_aRecvGroup.push_back(pGroup);
        nRemain -= nSlotCount;
    }
}

bool CSocketMgr::SendGameMsg( LPGameMsg_Base pGameMsg, unsigned short nSlot, bool bCopy )
{
    if ( pGameMsg == NULL )
        return false;

    if ( nSlot >= m_nMaxClientCount )
    {
        WriteLog( LOGLEVEL_ERROR, "[SendGameMsg failed]Slot(%u) is wrong.", nSlot );
        return false;
    }
    else if ( m_aSlot[nSlot].sock <= 0 )
    {
        WriteLog( LOGLEVEL_ERROR, "[SendGameMsg failed]Slot(%u) is not in using.", nSlot );
        return false;
    }

    bool bRes = m_aSlot[nSlot].Out( pGameMsg );
    if ( !bRes )
    {
        WriteLog( LOGLEVEL_ERROR, "[SendGameMsg failed]Msg=%u, size=%u, slot=%u.",
            pGameMsg->nMsg, pGameMsg->nSize, nSlot );
    }

    return bRes;
}

bool CSocketMgr::BroadcastMsg(LPGameMsg_Base pGameMsg)
{
    for (unsigned short i = 0; i < m_nMaxClientCount; i++)
    {
        if (m_aSlot[i].sock >= 0)
            SendGameMsg(pGameMsg, i);
    }
    return true;
}

bool CSocketMgr::GetGameMsg(LPGameMsg_Base &pGameMsg, unsigned short &nSlot)
{
    if (m_nMsgCount <= m_nMsgIndex)
    {
        m_nMsgCount = GetGameMsg(m_msgBuffer, 1024);
        m_nMsgIndex = 0;
    }
    pGameMsg = NULL;
    if (m_nMsgCount > m_nMsgIndex)
    {
        pGameMsg = m_msgBuffer[m_nMsgIndex].pMsg;
        nSlot = m_msgBuffer[m_nMsgIndex].nSlot;
        m_nMsgIndex++;

        GetSlot(nSlot)->GetFlux()->Process();

        if (pGameMsg != NULL)
        {
            Pretreat(pGameMsg, nSlot);
        }
    }

    return pGameMsg != NULL;
}

int CSocketMgr::GetGameMsg(IN_ELEMENT* pBuff, int nBuffSize)
{
    IN_ELEMENT *ptr = pBuff;
    int nCount = 0;
    for (unsigned int i = 0; i < m_nMaxClientCount; i++)
    {
        nCount += m_aSlot[i].In(ptr, nBuffSize - nCount);
    }

    return nCount;
}

void CSocketMgr::Pretreat(LPGameMsg_Base &pGameMsg, unsigned short &nSlot)
{
    bool bValid = true;
    //检查消息的合法性
    switch (pGameMsg->nMsg)
    {
    case MSG_SYSTEM_Connect:
        //Connect封包的大小是固定的
        //		bValid = pGameMsg->nSize == sizeof(GameMsg_System_Connect);
        //Connect消息只能是系统自己产生的而不能是通过网络传送的，并且只能是最初始的封包
        if (bValid)
            bValid = GetSlot(nSlot)->GetFlux()->nPacketProcessed == 1;
        if (bValid)
        {//发送握手包
            GameMsg_System_Slot msgSlot;
            msgSlot.nSlot = nSlot;
            msgSlot.nRandom = (unsigned short)rand();
            SendGameMsg(&msgSlot, nSlot);
            m_aKeyEncrypt[nSlot].SetKey((char*)&msgSlot, sizeof(GameMsg_System_Slot));
            m_nCurClientCount++;
        }
        break;
    case MSG_SYSTEM_Disconnect:
        //Disconnect封包收到后不管如何断开连接
        m_aSlot[nSlot].SLOT_BASE::Clear();
        m_nCurClientCount--;
        break;
    case MSG_SYSTEM_Slot:
        //		bValid = pGameMsg->nSize == sizeof(GameMsg_System_Slot);
        //用握手封包中的信息计算key
        m_aKeyDecrypt[nSlot].SetKey((char*)pGameMsg, sizeof(GameMsg_System_Slot));
        //握手封包只能是第一个收到的封包
        if (bValid)
            bValid = GetSlot(nSlot)->GetFlux()->nPacketProcessed == 2;
        if (!bValid)
        {
            WriteLog(LOGLEVEL_WARNING, "Have received packet(s) before handshake!\n");
        }
        break;
    case MSG_SYSTEM_Packet:
        break;
    case MSG_SYSTEM_Ping:
        break;
    default:
        if (m_pFuncDecrypt != NULL)bValid = false;
        break;
    }
    if (!bValid)
    {
        delete pGameMsg;
        pGameMsg = new GameMsg_System_Invalid;
    }
}

void CSocketMgr::AcceptConnect()
{
    unsigned int i = 0;

    //尽量让连进来的客户端均匀分布到各个线程中
    bool bFound = false;
    for (size_t k = 0; k < m_aSendGroup.size(); k++)
    {
        unsigned int nCountPerGroup = (m_nMaxClientCount + m_aSendGroup.size() - 1) / m_aSendGroup.size();
        int nGroupStart = m_nFindSlotStart * nCountPerGroup;
        if (m_nFindSlotStart == (int)m_aSendGroup.size() - 1)
            nCountPerGroup = m_nMaxClientCount - nCountPerGroup * m_nFindSlotStart;
        for (i = nGroupStart; i < nGroupStart + nCountPerGroup; i++)
        {
            if (m_aSlot[i].GetState() == SLOT_STATE_FREE && m_aSlot[i].sock == INVALID_SOCKET)
            {
                break;
            }
        }
        m_nFindSlotStart++;
        if (m_nFindSlotStart >= (int)m_aSendGroup.size())
            m_nFindSlotStart = 0;
        if (i < nGroupStart + nCountPerGroup)
        {
            bFound = true;
            break;
        }
    }

    sockaddr_in saClient;
    int nLen = sizeof(sockaddr);
    SOCKET sock = accept(m_hSocket, (sockaddr*)&saClient, (socklen_t*)&nLen);
    if (sock < 0)
    {
        WriteLog(LOGLEVEL_WARNING, "Failed to accept connection from %s\n", inet_ntoa(saClient.sin_addr));
    }
    else
    {
        if (!bFound)
        {
            closesocket(sock);
            WriteLog(LOGLEVEL_WARNING, "Server full!");
            return;
        }

        if (m_pAddMgr && m_pAddMgr->BeDenied(saClient.sin_addr.s_addr))
        {//连接来自一个被要求屏蔽的地址
            closesocket(sock);
            WriteLog(LOGLEVEL_DEBUG, "deny connection from %s!!!\n", inet_ntoa(saClient.sin_addr));
        }
        else
        {
            m_aSlot[i].OnAccept(sock, saClient);
        }
    }
}

unsigned int CSocketMgr::GetCurClientCount()
{
    unsigned int nCount = 0;
    for (unsigned int i = 0; i < m_nMaxClientCount; i++)
    {
        if (m_aSlot[i].sock != INVALID_SOCKET)
            nCount++;
    }
    return nCount;
}

void CSocketMgr::DisconnectClient(unsigned short nClientIndex, EDisconnectReason eReason)
{
    if (nClientIndex < m_nMaxClientCount)
    {
        m_aSlot[nClientIndex].Disconnect(eReason);
    }
}

void CSocketMgr::DisconnectAllClient()
{
    for (unsigned int i = 0; i < m_nMaxClientCount; i++)
    {
        DisconnectClient((unsigned short)i, EDisconnectReason_Normal);
    }
}

void CSocketMgr::StopListen()
{
    closesocket(m_hSocket);
}

thread_return_type CSocketMgr::ListenThreadProc(void* pParam)
{
    CSocketMgr*pMgr = (CSocketMgr*)pParam;
    timeval tv;
    int ret;
    fd_set set;

    while (pMgr->m_bRunning)
    {
        tv.tv_sec = 0;
        tv.tv_usec = 20000;
        FD_ZERO(&set);
        FD_SET(pMgr->m_hSocket, &set);
        ret = select(pMgr->m_hSocket + 1, &set, NULL, NULL, &tv);
        if (ret > 0)
        {
            if (FD_ISSET(pMgr->m_hSocket, &set))
            {
                pMgr->AcceptConnect();
            }
        }
        else if (ret == 0)//timeout
        {
        }
        else//failed
        {
            millisecondSleep(10);
        }
    }

    return 0;
}

