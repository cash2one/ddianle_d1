// libServerFrame/QueryManager.h: interface for the CQueryManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __QUERYMANAGERBASE_H__
#define __QUERYMANAGERBASE_H__

#include <stdio.h>
#include <stdlib.h>
#include "Windefine.h"
#include "Log.h"

#ifdef WIN32
#else
#include <pthread.h>
#endif

#include <list>
#include <map>
using namespace std;

extern CLog g_DBLog;

class QueryBase;
class CDelHelper;

class CQueryManagerBase
{
public:
	CQueryManagerBase();
	virtual ~CQueryManagerBase();

	//初始化
#ifdef WIN32
	virtual bool Initialize(HANDLE hEventNotify);
#else
	virtual bool Initialize(pthread_cond_t* cond, pthread_mutex_t* mutex);
#endif
	//释放
	virtual void Release();

	virtual int ProcessQuery(QueryBase* &){return 0;};

	//添加一个查询
	//参数内存有通过new产生的情况则需要通过pDelHelper辅助释放（特别是回调方法没有调用到的情况），以防泄露
	virtual void AddQuery(int nQueryType, int nAttachment = 0, void* pAttachment = NULL, __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL);
	virtual void AddQuery(QueryBase *query);

	//得到已经完成的查询
	QueryBase* GetQueryFinished();

protected:
	//得到一个查询//
	QueryBase* GetQuery();

	//查询完成//
	void FinishQuery(QueryBase *query, int nRes);

#ifdef WIN32
	static DWORD WINAPI QueryThreadProc(void* pParam);
#else
	static void* QueryThreadProc(void* pParam);
#endif
	

protected:
	bool m_bRunning;
	list< QueryBase* >m_apQueryQueue;
	list< QueryBase* >m_apQueryFinished;

#ifdef WIN32
	HANDLE m_hThreadQuery;
	HANDLE m_hEventQuery;
	HANDLE m_hEventNotify;
	CRITICAL_SECTION m_lockQuery;
#else
	pthread_mutex_t m_lockQuery; 
	pthread_cond_t  m_condQuery;
	pthread_mutex_t *m_pMutexQuery; 
	pthread_cond_t  *m_pCondQuery;
	pthread_mutex_t* m_pMutexNotify;
	pthread_cond_t* m_pCondNotify;
	pthread_t       m_hThreadQuery;
#endif
};

#endif // __QUERYMANAGERBASE_H__

//file end

