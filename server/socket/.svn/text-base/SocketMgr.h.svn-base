//////////////////////////////////////////////////////////////////////////
// File		:	SocketMgr.h
// Date		:	[2001-12-25]
// Author	:	Li Hongtao
// Desc		:	网络游戏服务器套接字管理
//////////////////////////////////////////////////////////////////////////

#ifndef __SOCKETMGR_H__
#define __SOCKETMGR_H__

#include "SocketDef.h"
#include "Slot.h"
#include "Crypt.h"
#include "AddrMgr.h"
#include <list>
using namespace std;

#ifdef _GAMEMSG_MEMPOOL
#include "MemPool.h"
extern CMemPoolMgr* g_pMemPoolMgrGameMsg;
#endif



namespace ZeroSocket
{
    class CSocketMgr
    {
    public:
        CSocketMgr();
        virtual ~CSocketMgr();
    public:

        //////////////////////////////////////////////////////////////////////////
        //一些初始化函数

        //初始化套接字管理器，指定监听的端口，以及允许的最大玩家数，以及客户的分组
        bool Init(unsigned short nPort, unsigned short nMaxClientCount, unsigned int nSendGroupCount = 1, unsigned int nRecvGroupCount = 1);

        //设置收到数据时的通知事件
#ifdef WIN32
        void SetNotifyEvent(HANDLE hEvent);
#else
        void SetNotifyEvent(pthread_cond_t* Cond, pthread_mutex_t* Mutex);
#endif//WIN32

        //设置加密解密函数
        void SetEncryptFunction(LPENCRYPTPROC pEncryptProc, LPDECRYPTPROC pDecryptProc){
            m_pFuncEncrypt = pEncryptProc; m_pFuncDecrypt = pDecryptProc;
        };

        //设置地址管理器
        void SetAddrMgr(CAddrMgr* pAddrMgr){
            m_pAddMgr = pAddrMgr;
        };

        //////////////////////////////////////////////////////////////////////////
        //主要的功能函数,向发送缓冲区发送数据和从接收缓冲区获得数据
        //将消息放入发送缓冲区，pGameMsg为要发送的消息，nSlot为要消息发送的目标客户所占用slot
        bool SendGameMsg(LPGameMsg_Base pGameMsg, unsigned short nSlot, bool bCopy = true);
        //广播消息给所有客户
        bool BroadcastMsg(LPGameMsg_Base pGameMsg);

        //接收消息，pGameMsg为取得的消息，nSlot为需要处理消息的玩家
        bool GetGameMsg(LPGameMsg_Base &pGameMsg, unsigned short& nSlot);
        //取回一批消息,返回取回消息的数量
        int GetGameMsg(IN_ELEMENT* pBuff, int nBuffSize);

        //断开客户
        void DisconnectClient(unsigned short nSlot, EDisconnectReason eReason);
        void DisconnectAllClient();
        //停止监听，即不再接受连接
        void StopListen();


        //////////////////////////////////////////////////////////////////////////
        //一些属性的访问函数

        //得到监听端口
        unsigned short GetListenPort(){
            return m_nPort;
        }
        //得到玩家数上限
        unsigned int GetMaxClientCount(){
            return m_nMaxClientCount;
        };
        //得到当前连接数
        unsigned int GetCurClientCount();
        //得到客户Slot指针,只可用于读取数据不能用于改写其中的数据
        SLOT* GetSlot(unsigned short nClient){
            return (m_aSlot && nClient < m_nMaxClientCount) ? m_aSlot + nClient : 0;
        };
        //得到客户的流量指针，只可用于读取数据不能用于改写其中的数据
        FLOWMETER* GetFlux(unsigned short nClient){
            return (m_aSlot && nClient < m_nMaxClientCount) ? m_aSlot[nClient].GetFlux() : 0;
        };

    protected:
        //監聽線程的函數
        static thread_return_type ListenThreadProc(LPVOID pParam);

        //客户端非法包
        void OnInvalidPacket(unsigned short nClientIndex);
        //读取网络数据
        void ReceivePacket(unsigned short nClientIndex);
        //接受连接
        void AcceptConnect();

        //设置客户分组方案,参数分别是发送组数、发送组客户数、接收组数、接收组客户数
        //首先按照指定的每组客户数分组，如果成员数大于剩下的客户数，则归于一组，如果分完后还有客户没分组，则归入最后一组
        void SetGroup(unsigned int nSendGroupCount, unsigned int nRecvGroupCount);

        //预处理封包
        void Pretreat(LPGameMsg_Base &pMsg, unsigned short &nSlot);

    protected:
#ifdef WIN32
        HANDLE m_hEventNotify;
#else
        pthread_cond_t* m_pCondNotify;
        pthread_mutex_t* m_pMutexNotify;
#endif//WIN32

        Thread m_threadListen;

        LPENCRYPTPROC m_pFuncEncrypt;
        LPDECRYPTPROC m_pFuncDecrypt;

        CAddrMgr* m_pAddMgr;

        unsigned int m_nMaxClientCount;	//允许的最大玩家数
        unsigned int m_nCurClientCount; //当前的连接的客户数
        //侦听的端口
        unsigned short m_nPort;

        //监听套接字
        int m_hSocket;

        friend class SlotSendGroup;
        friend class SlotRecvGroup;
        //标志是否运行
        int m_bRunning;

        //接收缓冲
        IN_ELEMENT m_msgBuffer[1024];
        int m_nMsgCount;
        int m_nMsgIndex;

        CCryptKey* m_aKeyEncrypt;
        CCryptKey* m_aKeyDecrypt;
        LPSLOT m_aSlot;//连接客户端的套接字
        list< SlotSendGroup* > m_aSendGroup;
        list< SlotRecvGroup* > m_aRecvGroup;
        int m_nFindSlotStart;
    };
}// end of namespace

#endif//__SOCKETMGR_H__

