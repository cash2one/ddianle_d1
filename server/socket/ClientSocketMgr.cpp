// ClientSocketMgr.cpp: implementation of the CClientSocketMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ClientSocketMgr.h"
#include "Utility.h"
#include "GameMsg_System_Connect.h"
#include "GameMsg_System_Slot.h"
#include "GameMsg_System_Ping.h"
#include "GameMsg_System_Packet.h"
#include "GameMsg_System_Disconnect.h"
#include "Formatter.h"

#include <time.h>
#ifdef _GAMEMSG_MEMPOOL
#include "MemPool.h"
#endif

#ifndef _TEST_NETWORK
#define _TEST_NETWORK  0
#endif


#ifdef WIN32
#define WaitThread(x)	{WaitForSingleObject(x, INFINITE);CloseHandle(x);}
#define Notify()		{if(m_hWndNotify)PostMessage(m_hWndNotify, UM_GAMEMSG, 0, 0);if(m_hEventNotify)SetEvent(m_hEventNotify);}
#define DoSend()		SetEvent(m_hEventSend)
#else
#define WaitThread(x)	pthread_join(x, NULL)
#define Notify()		if(m_pMutexNotify){pthread_mutex_lock(m_pMutexNotify);pthread_cond_signal(m_pCondNotify);pthread_mutex_unlock(m_pMutexNotify);}
#define DoSend()		
#endif

using namespace ZeroSocket;

CClientSocketMgr::CClientSocketMgr()
{
	m_nState = NETWORK_STATE_DISCONNECTED;
	m_bRunning = 0;
	m_pFuncEncrypt = NULL;
	m_pFuncDecrypt = NULL;

	m_szServerName[0] = 0;
	m_szServerAddr[0] = 0;
	m_nServerPort = 0;

	m_slot.SetSlotIndex(0);
	m_groupSend = NULL;
	m_groupRecv = NULL;
	m_nMsgIndex = m_nMsgCount = 0;

#ifdef WIN32
	m_hWndNotify = NULL;
	std::string strName;
	Formatter(strName) << "Socket" << (unsigned long)this << (unsigned int)time(NULL);
	m_hEventSocket = CreateEvent(NULL, FALSE, FALSE, strName.c_str());
#else
	m_pCondNotify = NULL;
	m_pMutexNotify = NULL;
#endif

}

CClientSocketMgr::~CClientSocketMgr()
{
	Disconnect();
#ifdef WIN32
	CloseHandle(m_hEventSocket);
#endif

	delete m_groupSend;
	delete m_groupRecv;
	while(m_nMsgIndex < m_nMsgCount)
	{
		msgBuff[m_nMsgIndex++].clear();
	}
}

#ifdef WIN32
bool CClientSocketMgr::Init(HWND hWndNotify /* = NULL */, HANDLE hEventNotify)
{
	m_hWndNotify = hWndNotify;
	m_hEventNotify = hEventNotify;
	WSADATA WSAData;
	if ( WSAStartup(WINSOCK_VERSION,&WSAData) ) 
	{
		return false;
	}
#else
bool CClientSocketMgr::Init(pthread_cond_t* Cond, pthread_mutex_t* Mutex)
{
	m_pCondNotify = Cond;
	m_pMutexNotify = Mutex;
#endif
#ifdef _GAMEMSG_MEMPOOL
	extern CMemPoolMgr* g_pMemPoolMgrGameMsg;
	if(g_pMemPoolMgrGameMsg == NULL)
	{
		g_pMemPoolMgrGameMsg = new CMemPoolMgr;
		g_pMemPoolMgrGameMsg->Initialize();
	}
#endif
	//m_bRunning = true;
	return true;
}

bool CClientSocketMgr::Connect(LPCSTR lpszServerName, LPCSTR lpszServerIP, unsigned short nPort)
{
	if(lpszServerName)strcpy(m_szServerName, lpszServerName);
	else m_szServerName[0] = 0;
	if(lpszServerIP)strcpy(m_szServerAddr, lpszServerIP);
	else m_szServerAddr[0] = 0;
	m_nServerPort = nPort;

	return Reconnect();
}

bool CClientSocketMgr::Reconnect()
{
	if(m_bRunning && m_nState > NETWORK_STATE_DISCONNECTED && m_slot.sock != INVALID_SOCKET)
	{
		return true;
	}
	else 
	{
		m_nState = NETWORK_STATE_DISCONNECTED;
		m_bRunning = false;
	}

#define m_hSocket m_slot.sock

	m_slot.SLOT_BASE::Clear();
	m_slot.Clear();

	delete m_groupSend; m_groupSend = NULL;
	delete m_groupRecv; m_groupRecv = NULL;

	DWORD dwIP = INADDR_NONE;

	if( m_szServerName[0] == 0 && m_szServerAddr[0] == 0 )
	{
		#if _TEST_NETWORK > 0
		TRACE(("__ClientSockMgr No server selected! Local machine replaced!\n"));
		#endif
		gethostname(m_szServerName, MAX_PATH);
	}
	else
	{
		dwIP = inet_addr(m_szServerAddr);
	}

	sockaddr_in sockAddr;

	m_hSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	if (m_hSocket == INVALID_SOCKET) 
	{
		#if _TEST_NETWORK > 0
		TRACE(("__ClientSockMgr socket failed!\n"));
		#endif
		return false;
	}

	//以IP地址为主
	if( dwIP == INADDR_NONE || dwIP == INADDR_ANY)
	{
		hostent* lpHostent = gethostbyname( m_szServerName );
		if( lpHostent != NULL )
		{
			sockAddr.sin_addr.s_addr = ((in_addr*)lpHostent->h_addr)->s_addr; 
		}
	}
	else
	{
		sockAddr.sin_addr.s_addr = dwIP;
	}


	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons( m_nServerPort);
	m_bRunning = true;

#ifdef WIN32
	//设置事件选择模式，该模式下套接字自动为非阻塞
	int nErrorCode = WSAEventSelect(m_hSocket, m_hEventSocket, FD_CONNECT);
	if (nErrorCode == SOCKET_ERROR) 
	{
		#if _TEST_NETWORK > 0
		TRACE(("__ClientSockMgr WSAEventSelect failed!\n"));
		#endif
		return false;
	}

	int nConnect = connect(m_hSocket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
	if (nConnect == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) 
	{
		#if _TEST_NETWORK > 0
		TRACE(( "__ClientSockMgr Socket Connect Error!\n"));
		#endif
		return false;
	}
	else
	{
		//等待连接返回，timeout时间
#define CONNECT_TIMEOUT		5000

		DWORD dwWaitRes = WaitForSingleObject(m_hEventSocket, CONNECT_TIMEOUT);
		if( dwWaitRes != WAIT_OBJECT_0 )
		{
			#if _TEST_NETWORK > 0
			TRACE(("__ClientSockMgr Connect failed!\n"));
			#endif
			Disconnect();
			return false;
		}

		WSANETWORKEVENTS event;
		int nRes = WSAEnumNetworkEvents(m_hSocket, m_hEventSocket, &event);
		if ( nRes == 0) 
		{
			if (event.lNetworkEvents & FD_CONNECT) 
			{
				m_nState = NETWORK_STATE_CONNECTED;
				if( 0 != event.iErrorCode[FD_CONNECT_BIT])
				{
					m_nState = NETWORK_STATE_DISCONNECTED;
					#if _TEST_NETWORK > 0
					TRACE(("__ClientSockMgr Connect failed\n"));
					#endif
				}
				else
				{
					#if _TEST_NETWORK > 0
					TRACE(("__ClientSockMgr Connect successfully\n"));
					#endif
				}

				m_slot.OnConnect();

				GameMsg_System_Connect *msgConn = new GameMsg_System_Connect;
				msgConn->nConnCode = event.iErrorCode[FD_CONNECT_BIT];
				SendGameMsg(msgConn, true);
			}
		}
		else
		{
			#if _TEST_NETWORK > 0
			TRACE("WSAEnumNetworkEvents failed!\n");
			#endif
			return false;
		}

		//设置事件选择模式，该模式下套接字自动为非阻塞
		nErrorCode = WSAEventSelect(m_hSocket, m_hEventSocket, FD_READ|FD_CLOSE);
		if (nErrorCode == SOCKET_ERROR) 
		{
			TRACE(("__ClientSockMgr WSAEventSelect failed!\n"));
			return false;
		}

		if( !m_threadSend.Create(SendThreadProc, this))
		{
			TRACE(("__ClientSockMgr CreateSendThread failed!\n"));
			Disconnect();
			return false;
		}
		if(!m_threadRecv.Create(RecvThreadProc, this))
		{
			TRACE(("__ClientSockMgr CreateRecvThread failed!\n"));
			Disconnect();
			return false;
		}
	}
#else
	int nConnect = connect(m_hSocket, (sockaddr*)&sockAddr, sizeof(sockAddr));
	if (nConnect < 0) 
	{
		TRACE(( "__ClientSockMgr Socket Connect Error!\n"));
		m_nState = NETWORK_STATE_DISCONNECTED;
		return false;
	}
	else
	{
		m_nState = NETWORK_STATE_CONNECTED;

		m_groupSend = new SlotSendGroup(&m_slot, 1, &m_bRunning);
		m_groupRecv = new SlotRecvGroup(&m_slot, 1, &m_bRunning);

		m_slot.OnConnect();

		GameMsg_System_Connect *msgConn = new GameMsg_System_Connect;
		msgConn->nConnCode = 0;
		SendGameMsg(msgConn, true);
	}
#endif
	return m_bRunning && m_nState == NETWORK_STATE_CONNECTED && m_hSocket != INVALID_SOCKET;
}

void CClientSocketMgr::Disconnect()
{
	
	m_bRunning = false;

	m_threadSend.Wait();
	m_threadRecv.Wait();

	m_slot.SLOT_BASE::Clear();

	m_nState = NETWORK_STATE_DISCONNECTED;
}

LPGameMsg_Base CClientSocketMgr::GetGameMsg()
{
	IN_ELEMENT *pBuff = msgBuff;
	LPGameMsg_Base pMsg = NULL;
	if(m_nMsgCount <= m_nMsgIndex)
	{
		m_nMsgCount = m_slot.In(pBuff, 64);
		m_nMsgIndex = 0;
	}
	if(	m_nMsgCount > m_nMsgIndex)
	{
		pMsg = msgBuff[m_nMsgIndex++].pMsg;
		m_slot.GetFlux()->Process();

		Pretreat(pMsg);
	}
	return pMsg;
}

INLINE void CClientSocketMgr::Pretreat(LPGameMsg_Base &pGameMsg)
{
	//TRACE("ClientSocketMgr Pretreat 0x%04X,%d \n", pGameMsg->nMsg, pGameMsg->nSize);

	bool bValid = true;
	//pretreat game messages
	switch(pGameMsg->nMsg) 
	{
	case MSG_SYSTEM_Connect:
		if(((GameMsg_System_Connect*)pGameMsg)->nConnCode == 0)
		{
			GameMsg_System_Slot msgSlot;
			msgSlot.nSlot = time(NULL);
			msgSlot.nRandom = (unsigned short)rand();
			SendGameMsg(&msgSlot);
			m_keyEncrypt.SetKey((char*)&msgSlot, sizeof(GameMsg_System_Slot));
		}
		break;
	case MSG_SYSTEM_Disconnect:
		m_nState = NETWORK_STATE_DISCONNECTED;
		break;
	case MSG_SYSTEM_Slot:
		{
			//握手封包只能是第一个收到的封包
//			bValid = pGameMsg->nSize == sizeof(GameMsg_System_Slot);
			//if(bValid)bValid = m_slot.GetFlux()->nPacketProcessed == 1;
			if(bValid)
			{
				LPGameMsg_System_Slot pSlot = (LPGameMsg_System_Slot)pGameMsg;
				//回复握手信息
				//GameMsg_System_Slot msgSlot;
				//msgSlot.nSlot = pSlot->nSlot;
				//msgSlot.nRandom = rand();
				//SendGameMsg(&msgSlot);
				//用握手封包中的信息计算key
				//m_keyEncrypt.SetKey((char*)&msgSlot, sizeof(GameMsg_System_Slot));
				m_keyDecrypt.SetKey((char*)pSlot, sizeof(GameMsg_System_Slot));
				
			}
			
			delete pGameMsg;
			pGameMsg = NULL;
		}
		break;
	case MSG_SYSTEM_Ping:
		{
			LPGameMsg_System_Ping pPing = (LPGameMsg_System_Ping)pGameMsg;
			if(pPing->nDirection == 0)//服务器发过来的ping封包
			{
				SendGameMsg(pGameMsg);
				delete pGameMsg;
				pGameMsg = NULL;
			}
			else if(pPing->nDirection == 1)//服务器回过来的ping封包
			{
			}
		}
		break;
	//case MSG_SYSTEM_Packet:
	//	if (m_pFuncDecrypt) 
	//	{
	//		char *pData = (char*)pGameMsg;
//			m_pFuncDecrypt(pData+sizeof(GameMsg_Base), pGameMsg->nSize - sizeof(GameMsg_Base), m_keyDecrypt.GetKey(), CryptKeyLen);
//			LPGameMsg_System_Packet pPacket = (LPGameMsg_System_Packet)pData;
			//bValid = pPacket->nSize == pPacket->msg.nSize + sizeof(GameMsg_System_Packet) - sizeof(GameMsg_Base);
			//if(bValid)
			//	bValid = pPacket->nCheckSum == pPacket->msg.CheckSum();
			//if(bValid)
			//{
			//	//wsj
			//	//pGameMsg = new(pPacket->msg.nSize)GameMsg_Base;
			//	//memcpy(pGameMsg, &pPacket->msg, pPacket->msg.nSize);
			//	delete pPacket;
			//	m_keyDecrypt.Update();
			//}
//		}
//		break;
	default:
		break;
	}
}

//void CClientSocketMgr::AddGameMsgToReceiveBuffer(LPGameMsg_Base pMsg)
//{
	//m_flux.In(pMsg->nSize);

//}

INLINE bool CClientSocketMgr::SendGameMsg(LPGameMsg_Base pGameMsg, bool bToSelf /* = false */)
{
	//if (pGameMsg->nSize < sizeof(GameMsg_Base) )
	//{
	//	TRACE("__ClientSockMgr Invalid GameMsg \n");
	//	return false;
	//}

	if (bToSelf) //Add game msg to gamemsg list
	{
		if(m_nMsgCount < MSGBUFF_SIZE)
		{
			msgBuff[m_nMsgCount++] = pGameMsg;
		}
		//m_msgBuffer.add(pGameMsg, 0);
	}
	else
	{
		//wsj
/*		LPGameMsg_Base pMsg = NULL;
		if ((pGameMsg->nMsg & 0xFF00) != MSG_TYPE_SYSTEM
#ifndef _INLINE_ENCRYPT
			&& m_pFuncEncrypt != NULL
#endif
			)//系统消息不加密
		{
			unsigned short nSize = pGameMsg->nSize+sizeof(GameMsg_System_Packet)-sizeof(GameMsg_Base);
			LPGameMsg_System_Packet pPacket = new (nSize)GameMsg_System_Packet;
			pPacket->nMsg = MSG_SYSTEM_Packet;
			pPacket->nSize = nSize;
			pPacket->nCheckSum = pGameMsg->CheckSum();
			memcpy(&pPacket->msg, pGameMsg, pGameMsg->nSize);
#ifndef _INLINE_ENCRYPT
			m_pFuncEncrypt((char*)pPacket + sizeof(GameMsg_Base), pPacket->nSize-sizeof(GameMsg_Base), m_keyEncrypt.GetKey(), CryptKeyLen);
#else
			DES_Encrypt((char*)pPacket + sizeof(GameMsg_Base), pPacket->nSize-sizeof(GameMsg_Base), m_keyEncrypt.GetKey(), CryptKeyLen);
#endif
			m_keyEncrypt.Update();
			pMsg = pPacket;
		}
		else
		{
			pMsg = new (pGameMsg->nSize) GameMsg_Base;
			memcpy(pMsg, pGameMsg, pGameMsg->nSize);
		}*/

		m_slot.Out(pGameMsg);
	}
	return true;
}

#ifdef WIN32
DWORD CClientSocketMgr::RecvThreadProc(LPVOID pParam)
{
	CClientSocketMgr *pCSM = (CClientSocketMgr *)pParam;
	WSANETWORKEVENTS event;
	while(pCSM->m_bRunning)
	{
		DWORD dwWaitRes = WaitForSingleObject(pCSM->m_hEventSocket, 10);
		switch(dwWaitRes) 
		{
		case WAIT_OBJECT_0:
			{
				//TRACE("__ClientSockMgr Event Signaled!\n");
				int nRes = WSAEnumNetworkEvents(pCSM->m_slot.sock, pCSM->m_hEventSocket, &event);
				if ( nRes == 0) 
				{
					if(event.lNetworkEvents & FD_READ)
					{
						pCSM->m_slot.Recv();
					}
					if(event.lNetworkEvents & FD_CLOSE)
					{
						#if _TEST_NETWORK > 0
						TRACE(("__ClientSockMgr Socket closed!\n"));
						#endif
						GameMsg_System_Disconnect* pmsgDisconnect = new GameMsg_System_Disconnect;
						pCSM->m_slot.OutToSelf(pmsgDisconnect);
						pCSM->m_bRunning = false;
					}
					if (event.lNetworkEvents & FD_CONNECT) 
					{
						//GameMsg_System_Connect *pmsgConn = new GameMsg_System_Connect;
						//pmsgConn->nConnCode = event.iErrorCode[FD_CONNECT_BIT];
						//pCSM->m_slot.OutToSelf(pmsgConn);
						//if( 0 == event.iErrorCode[FD_CONNECT_BIT])
						//{
						//	pCSM->m_nState = NETWORK_STATE_CONNECTED;
						//}
						//else
						//{
						//	pCSM->m_bRunning = false;
						//}
						//TRACE(("__ClientSockMgr Connect %s\n"), bLoop ? ("successfully") : ("failed"));
					}
				}
			}
			break;
		default:
			break;
		}
	}
	pCSM->m_nState = NETWORK_STATE_DISCONNECTED;
	#if _TEST_NETWORK > 0
	TRACE(("__ClientSockMgr Receive thread exit normally!\n"));
	#endif

	return 0;
}

DWORD CClientSocketMgr::SendThreadProc(LPVOID pParam)
{
	CClientSocketMgr *pCSM = (CClientSocketMgr *)pParam;
	while(pCSM->m_bRunning)
	{
		pCSM->m_slot.Send();
		millisecondSleep(10);

		//DWORD dwWaitRes = WaitForSingleObject(pCSM->m_hEventSend, 10);

		//switch(dwWaitRes) 
		//{
		//case WAIT_OBJECT_0:
		//	//      if( !bLoop )TRACE(_T("__ClientSockMgr Send packet false\n"));
		//	break;
		//}
	}
	pCSM->m_nState = NETWORK_STATE_DISCONNECTED;
	#if _TEST_NETWORK > 0
	TRACE(("__ClientSockMgr Send thread exit normally!\n"));
	#endif
	return 0;

}

#endif // win32


//file end

