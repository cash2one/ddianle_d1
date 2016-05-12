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
//初始化函数

	//初始化，设置消息到来时需要通知的窗口hWndNotify，
	//如果不设置，则可以用循环中调用GetGameMsg()来判断是否有消息来
	//使用connect之前必须先调用Initialize
	#ifdef WIN32
	bool Init(HWND hWndNotify = NULL, HANDLE hEventNotify = NULL);
	#else
	bool Init(pthread_cond_t* Cond = NULL, pthread_mutex_t* Mutex = NULL);
	#endif

	//设置解密解密函数
	void SetEncryptFunction(LPENCRYPTPROC pEncryptProc, LPDECRYPTPROC pDecryptProc){m_pFuncEncrypt = pEncryptProc; m_pFuncDecrypt = pDecryptProc;};

//////////////////////////////////////////////////////////////////////////
//主要功能函数

	//连接服务器，之前必须调用Initialize
	bool Connect(LPCSTR lpszServerName, LPCSTR lpszServerIP, unsigned short nPort);
	bool Reconnect();//重连接，用上次连接的server和port

	//断开连接
	void Disconnect();

	//从消息缓冲区取出一个消息，使用後需delete该消息，如果缓冲区没有消息则返回NULL
	LPGameMsg_Base GetGameMsg();

	//发送一个消息，bToSelf == TRUE 则不通过网络而直接发给自己，默认是通过网络传送
	INLINE bool SendGameMsg(LPGameMsg_Base pGameMsg, bool bToSelf = false);

//////////////////////////////////////////////////////////////////////////
//得到属性和统计信息的接口

	int GetState(){return m_nState;};

	//得到流量计的指针
	FLOWMETER* GetFlowmeter(){return m_slot.GetFlux();};


protected:

	//发送信息的ThreadProc
	static thread_return_type SendThreadProc(LPVOID pParam);
	//接受信息的ThreadProc
	static thread_return_type RecvThreadProc(LPVOID pParam);

	//预处理
	INLINE void Pretreat(LPGameMsg_Base &pMsg);
protected:

#ifdef WIN32
	HWND m_hWndNotify;
	HANDLE m_hEventNotify;

	WSAEVENT m_hEventSocket;//Socket事件
#else
	pthread_cond_t* m_pCondNotify;//向外通知的条件变量
	pthread_mutex_t* m_pMutexNotify;
#endif

	//用来控制发送和接收缓冲区读写同步的互斥变量
	ThreadLock m_lockSend, m_lockRecv;

	Thread 	m_threadSend;
	Thread	m_threadRecv;

	int   m_nState;	//当前的连接状态
	int   m_bRunning;		//标志是否运行中

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

