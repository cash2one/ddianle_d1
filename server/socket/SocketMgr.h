//////////////////////////////////////////////////////////////////////////
// File		:	SocketMgr.h
// Date		:	[2001-12-25]
// Author	:	Li Hongtao
// Desc		:	������Ϸ�������׽��ֹ���
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
        //һЩ��ʼ������

        //��ʼ���׽��ֹ�������ָ�������Ķ˿ڣ��Լ�����������������Լ��ͻ��ķ���
        bool Init(unsigned short nPort, unsigned short nMaxClientCount, unsigned int nSendGroupCount = 1, unsigned int nRecvGroupCount = 1);

        //�����յ�����ʱ��֪ͨ�¼�
#ifdef WIN32
        void SetNotifyEvent(HANDLE hEvent);
#else
        void SetNotifyEvent(pthread_cond_t* Cond, pthread_mutex_t* Mutex);
#endif//WIN32

        //���ü��ܽ��ܺ���
        void SetEncryptFunction(LPENCRYPTPROC pEncryptProc, LPDECRYPTPROC pDecryptProc){
            m_pFuncEncrypt = pEncryptProc; m_pFuncDecrypt = pDecryptProc;
        };

        //���õ�ַ������
        void SetAddrMgr(CAddrMgr* pAddrMgr){
            m_pAddMgr = pAddrMgr;
        };

        //////////////////////////////////////////////////////////////////////////
        //��Ҫ�Ĺ��ܺ���,���ͻ������������ݺʹӽ��ջ������������
        //����Ϣ���뷢�ͻ�������pGameMsgΪҪ���͵���Ϣ��nSlotΪҪ��Ϣ���͵�Ŀ��ͻ���ռ��slot
        bool SendGameMsg(LPGameMsg_Base pGameMsg, unsigned short nSlot, bool bCopy = true);
        //�㲥��Ϣ�����пͻ�
        bool BroadcastMsg(LPGameMsg_Base pGameMsg);

        //������Ϣ��pGameMsgΪȡ�õ���Ϣ��nSlotΪ��Ҫ������Ϣ�����
        bool GetGameMsg(LPGameMsg_Base &pGameMsg, unsigned short& nSlot);
        //ȡ��һ����Ϣ,����ȡ����Ϣ������
        int GetGameMsg(IN_ELEMENT* pBuff, int nBuffSize);

        //�Ͽ��ͻ�
        void DisconnectClient(unsigned short nSlot, EDisconnectReason eReason);
        void DisconnectAllClient();
        //ֹͣ�����������ٽ�������
        void StopListen();


        //////////////////////////////////////////////////////////////////////////
        //һЩ���Եķ��ʺ���

        //�õ������˿�
        unsigned short GetListenPort(){
            return m_nPort;
        }
        //�õ����������
        unsigned int GetMaxClientCount(){
            return m_nMaxClientCount;
        };
        //�õ���ǰ������
        unsigned int GetCurClientCount();
        //�õ��ͻ�Slotָ��,ֻ�����ڶ�ȡ���ݲ������ڸ�д���е�����
        SLOT* GetSlot(unsigned short nClient){
            return (m_aSlot && nClient < m_nMaxClientCount) ? m_aSlot + nClient : 0;
        };
        //�õ��ͻ�������ָ�룬ֻ�����ڶ�ȡ���ݲ������ڸ�д���е�����
        FLOWMETER* GetFlux(unsigned short nClient){
            return (m_aSlot && nClient < m_nMaxClientCount) ? m_aSlot[nClient].GetFlux() : 0;
        };

    protected:
        //�O ���̵ĺ���
        static thread_return_type ListenThreadProc(LPVOID pParam);

        //�ͻ��˷Ƿ���
        void OnInvalidPacket(unsigned short nClientIndex);
        //��ȡ��������
        void ReceivePacket(unsigned short nClientIndex);
        //��������
        void AcceptConnect();

        //���ÿͻ����鷽��,�����ֱ��Ƿ���������������ͻ���������������������ͻ���
        //���Ȱ���ָ����ÿ��ͻ������飬�����Ա������ʣ�µĿͻ����������һ�飬���������пͻ�û���飬��������һ��
        void SetGroup(unsigned int nSendGroupCount, unsigned int nRecvGroupCount);

        //Ԥ������
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

        unsigned int m_nMaxClientCount;	//�������������
        unsigned int m_nCurClientCount; //��ǰ�����ӵĿͻ���
        //�����Ķ˿�
        unsigned short m_nPort;

        //�����׽���
        int m_hSocket;

        friend class SlotSendGroup;
        friend class SlotRecvGroup;
        //��־�Ƿ�����
        int m_bRunning;

        //���ջ���
        IN_ELEMENT m_msgBuffer[1024];
        int m_nMsgCount;
        int m_nMsgIndex;

        CCryptKey* m_aKeyEncrypt;
        CCryptKey* m_aKeyDecrypt;
        LPSLOT m_aSlot;//���ӿͻ��˵��׽���
        list< SlotSendGroup* > m_aSendGroup;
        list< SlotRecvGroup* > m_aRecvGroup;
        int m_nFindSlotStart;
    };
}// end of namespace

#endif//__SOCKETMGR_H__

