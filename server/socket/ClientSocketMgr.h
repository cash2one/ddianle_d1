// ClientSocketMgr.h: interface for the CClientSocketMgr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CLIENT_SOCKET_MGR_H__
#define __CLIENT_SOCKET_MGR_H__

#include "SocketDef.h"
#include "SocketLibCommonDef.h"
#include "Slot.h"
#include "MsgBaseStruct.h"
#include "Crypt.h"
#include <list>
using namespace std;

namespace ZeroSocket
{

class CClientSocketMgr  
{
public:
	CClientSocketMgr();
	virtual ~CClientSocketMgr();
public:

//////////////////////////////////////////////////////////////////////////
//��ʼ������

	//��ʼ����������Ϣ����ʱ��Ҫ֪ͨ�Ĵ���hWndNotify��
	//��������ã��������ѭ���е���GetGameMsg()���ж��Ƿ�����Ϣ��
	//ʹ��connect֮ǰ�����ȵ���Initialize
	#ifdef WIN32
	bool Init(HWND hWndNotify = NULL, HANDLE hEventNotify = NULL);
	#else
	bool Init(pthread_cond_t* Cond = NULL, pthread_mutex_t* Mutex = NULL);
	#endif

	//���ý��ܽ��ܺ���
	void SetEncryptFunction(LPENCRYPTPROC pEncryptProc, LPDECRYPTPROC pDecryptProc){m_pFuncEncrypt = pEncryptProc; m_pFuncDecrypt = pDecryptProc;};

//////////////////////////////////////////////////////////////////////////
//��Ҫ���ܺ���

	//���ӷ�������֮ǰ�������Initialize
	bool Connect(LPCSTR lpszServerName, LPCSTR lpszServerIP, unsigned short nPort);
	bool Reconnect();//�����ӣ����ϴ����ӵ�server��port

	//�Ͽ�����
	void Disconnect();

	//����Ϣ������ȡ��һ����Ϣ��ʹ������delete����Ϣ�����������û����Ϣ�򷵻�NULL
	LPGameMsg_Base GetGameMsg();

	//����һ����Ϣ��bToSelf == TRUE ��ͨ�������ֱ�ӷ����Լ���Ĭ����ͨ�����紫��
	INLINE bool SendGameMsg(LPGameMsg_Base pGameMsg, bool bToSelf = false);

//////////////////////////////////////////////////////////////////////////
//�õ����Ժ�ͳ����Ϣ�Ľӿ�

	int GetState(){return m_nState;};

	//�õ������Ƶ�ָ��
	FLOWMETER* GetFlowmeter(){return m_slot.GetFlux();};


protected:

	//������Ϣ��ThreadProc
	static thread_return_type SendThreadProc(LPVOID pParam);
	//������Ϣ��ThreadProc
	static thread_return_type RecvThreadProc(LPVOID pParam);

	//Ԥ����
	INLINE void Pretreat(LPGameMsg_Base &pMsg);
protected:

#ifdef WIN32
	HWND m_hWndNotify;
	HANDLE m_hEventNotify;

	WSAEVENT m_hEventSocket;//Socket�¼�
#else
	pthread_cond_t* m_pCondNotify;//����֪ͨ����������
	pthread_mutex_t* m_pMutexNotify;
#endif

	//�������Ʒ��ͺͽ��ջ�������дͬ���Ļ������
	ThreadLock m_lockSend, m_lockRecv;

	Thread 	m_threadSend;
	Thread	m_threadRecv;

	int   m_nState;	//��ǰ������״̬
	int   m_bRunning;		//��־�Ƿ�������

	char m_szServerName[MAX_PATH];
	char m_szServerAddr[MAX_PATH];
	unsigned short m_nServerPort;
	SOCKET m_hSocket;

	LPENCRYPTPROC m_pFuncEncrypt;
	LPDECRYPTPROC m_pFuncDecrypt;
	CCryptKey m_keyEncrypt;//
	CCryptKey m_keyDecrypt;

	//CMsgBuffer m_msgBuffer;
#define MSGBUFF_SIZE	128
	IN_ELEMENT msgBuff[MSGBUFF_SIZE];
	OUT_ELEMENT msgBuffOut[MSGBUFF_SIZE];
	int m_nMsgCount;
	int m_nMsgIndex;

	SLOT m_slot;
	SlotSendGroup *m_groupSend;
	SlotRecvGroup *m_groupRecv;
};

}//end of namespace ZeroSocket

#endif // __CLIENT_SOCKET_MGR_H__


//file end

