#ifndef __SOCKET_DEF_H__
#define __SOCKET_DEF_H__

#ifdef WIN32	//Windows

#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION			MAKEWORD(2,2)
#endif//WINSOCK_VERSION

#ifndef UM_GAMEMSG
#define UM_GAMEMSG    (WM_USER+1023)
#endif

#ifndef FD_SETSIZE
#define FD_SETSIZE      1024
#endif /* FD_SETSIZE */
#ifndef NTDDI_WIN7SP1
#define NTDDI_WIN7SP1 0x06010100
#endif
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4018 4313 4311 4291 4244)

#else	//Linux

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#endif// define WIN32

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "Windefine.h"

#define NETWORK_STATE_DISCONNECTED  0
#define NETWORK_STATE_CONNECTING    1
#define NETWORK_STATE_CONNECTED     2

//加密解密回调函数类型
typedef unsigned int ( *PENCRYPT_ROUTINE)(char* pData, unsigned int nSize, char* pKey, unsigned int nSizeKey);

//加密解密函数指针类型
typedef PENCRYPT_ROUTINE LPENCRYPTPROC;
typedef PENCRYPT_ROUTINE LPDECRYPTPROC;

namespace ZeroSocket
{


//////////////////////////////////////////////////////////////////////////
//关于SOCKET的掩藏差别的类型定义
#ifdef WIN32
#define socklen_t	int
#define iovec		WSABUF
#define iov_base	buf
#define iov_len		len
#else
typedef int			SOCKET;
#define closesocket(x)	::close(x)
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#define SD_RECEIVE		SHUT_RD
#define SD_SEND			SHUT_WR
#define SD_BOTH			SHUT_RDWR
#endif

#define QUEUE_SIZE	5		//同时处理的连接数
#define MAX_SPARE_FD		32//闲置的文件描述符个数

#ifndef MAX_PACKET_SIZE
//#define MAX_PACKET_SIZE			32768	//包的最大长度
#define MAX_PACKET_SIZE			65500	//包的最大长度
#endif

enum EDisconnectReason
{
	EDisconnectReason_Normal,
	EDisconnectReason_SendError,
	EDisconnectReason_RecvError,
};
//////////////////////////////////////////////////////////////////////////
//流量计
struct FLOWMETER
{
	FLOWMETER()
	{
		Clear();
	}
	void Out(unsigned int nBytes, unsigned int nPacketCount = 1)
	{
		nTotalPacketOut += nPacketCount;
		nPacketOut += nPacketCount;
		nTotalBytesOut += nBytes;
		nBytesOut += nBytes;
	};
	void In(unsigned int nBytes, unsigned int nPacketCount = 1)
	{
		nTotalPacketIn += nPacketCount;
		nPacketIn += nPacketCount;
		nTotalBytesIn += nBytes;
		nBytesIn += nBytes;
	};
	void Remeasure()
	{
		nPacketOut = 0;
		nPacketIn = 0;
		nBytesOut = 0;
		nBytesIn = 0;
	};
	void Process( unsigned int nPacketCount = 1 )
	{
		nPacketProcessed += nPacketCount;
		//nBytesProcessed += nBytes;
	}
	void Clear()
	{
		memset(this, 0, sizeof(FLOWMETER));
	};

	unsigned int nTotalPacketOut;//total packet count;
	unsigned int nTotalPacketIn;//total packet count;
	unsigned int nTotalBytesOut;
	unsigned int nTotalBytesIn;
	unsigned int nPacketOut;
	unsigned int nPacketIn;
	unsigned int nBytesOut;
	unsigned int nBytesIn;
	unsigned int nPacketProcessed;
	unsigned int nBytesProcessed;
};

//数据包加密的key
struct PacketEncryptKey
{
	unsigned short nRandom;
	unsigned short nCRC;
	unsigned short nSerial;
	unsigned short nSlot;

	void Clear(unsigned short slot = 0)
	{
		nSerial = 0;
		nCRC = 0xFFFF;
		nSlot = slot;
		nRandom = 0;
	};
};



//////////////////////////////////////////////////////////////////////////
//关于线程的掩藏差别的一些类型定义

#ifdef WIN32
#define thread_return_type	DWORD __stdcall
#define millisecondSleep(milliseconds)	Sleep(milliseconds)
#define threadHandle		HANDLE
#else
#include <pthread.h>
#define thread_return_type	void*
#include <unistd.h>
#define millisecondSleep(milliseconds)	usleep(1000*(milliseconds))
#define threadHandle		pthread_t
typedef void* ( *PTHREAD_START_ROUTINE)(  void* lpThreadParameter   );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
#endif

class Thread
{
public:
	bool Create( LPTHREAD_START_ROUTINE start_routine, void* arg)
	{
		#ifdef WIN32
		thread = CreateThread(NULL, 0, start_routine, arg, 0, &dwThreadID);
		return thread != NULL;
		#else
		int	nRes = pthread_create(&thread, 0, start_routine, arg);
		return nRes == 0;
		#endif
	};
	void Wait()
	{
		if (thread != 0)
		{
			#ifdef WIN32
			WaitForSingleObject(thread, INFINITE);
			#else
			void* thread_return;
			pthread_join(thread, &thread_return);
			#endif
		}
	}
	Thread()
	{
		thread = 0;
	}
protected:
	threadHandle thread;
#ifdef WIN32
	DWORD dwThreadID;
#endif
private:
};


class ThreadLock
{
public:
	ThreadLock()
	{
#ifdef WIN32
		InitializeCriticalSection(&_lock);
#else
		pthread_mutex_init(&_lock, 0);
#endif
	}
	~ThreadLock()
	{
#ifdef WIN32
		DeleteCriticalSection(&_lock);
#else
		pthread_mutex_destroy(&_lock);
#endif
	}
	void Lock()
	{
#ifdef WIN32
		EnterCriticalSection(&_lock);
#else
		pthread_mutex_lock(&_lock);
#endif
	}
	void Unlock()
	{
#ifdef WIN32
		LeaveCriticalSection(&_lock);
#else
		pthread_mutex_unlock(&_lock);
#endif
	}
protected:
#ifdef WIN32
	CRITICAL_SECTION _lock;
#else
	pthread_mutex_t _lock;
#endif
};

}//end of namespace

#endif//__SOCKET_DEF_H__

//file end

