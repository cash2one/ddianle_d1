// QueryManager.cpp: implementation of the CQueryManagerBase class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "QueryManagerBase.h"
#include "Query.h"
#include "Utility.h"
#include "Formatter.h"

// #ifdef WIN32
// #pragma warning(disable:4311;disable:4996)
// #endif

#ifdef WIN32
#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#define WaitThread(x)	{WaitForSingleObject(x, INFINITE);CloseHandle(x);}
#define SignalEvent(x)	{SetEvent(m_hEvent##x);}
#else
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#define WaitThread(x)	pthread_join(x, NULL)
#define SignalEvent(x)	{pthread_mutex_lock(m_pMutex##x);pthread_cond_signal(m_pCond##x);pthread_mutex_unlock(m_pMutex##x);}
#endif


CQueryManagerBase::CQueryManagerBase()
{
	m_bRunning = false;
#ifdef WIN32
	InitializeCriticalSection(&m_lockQuery);
	std::string strName;
	Formatter(strName) << "EventQuery" << (unsigned int)this << GetTickCount();
	m_hEventQuery = CreateEvent(NULL, FALSE, FALSE, strName.c_str());;

	m_hEventNotify = NULL;
#else
	pthread_mutex_init(&m_lockQuery, NULL);
	pthread_cond_init(&m_condQuery, NULL);
	m_pMutexQuery = &m_lockQuery;
	m_pCondQuery = &m_condQuery;

	m_pCondNotify = NULL;
	m_pMutexNotify = NULL;

#endif

}

CQueryManagerBase::~CQueryManagerBase()
{
	Release();

#ifdef WIN32
	CloseHandle(m_hEventQuery);
	m_hEventQuery = NULL;
	DeleteCriticalSection(&m_lockQuery);
#else
	pthread_mutex_destroy(&m_lockQuery);
	pthread_cond_destroy(&m_condQuery);
#endif

	while (m_apQueryQueue.size() > 0) 
	{
		delete m_apQueryQueue.front();
		m_apQueryQueue.pop_front();
	}
	while (m_apQueryFinished.size() > 0) 
	{
		delete m_apQueryFinished.front();
		m_apQueryFinished.pop_front();
	}

}

#ifdef WIN32
bool CQueryManagerBase::Initialize(HANDLE hEventNotify)
{
	m_bRunning = true;
	m_hEventNotify = hEventNotify;
	m_hThreadQuery = CreateThread(NULL, 0, QueryThreadProc, this, 0, NULL);
	if(m_hThreadQuery == NULL)
	{
		TRACE("Create Query Thread failed!\n");
		return false;
	}

	return true;
}
#else

bool CQueryManagerBase::Initialize(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
	m_bRunning = true;
	m_pCondNotify = cond;
	m_pMutexNotify = mutex;

	int nResult;
	nResult = pthread_create(&m_hThreadQuery, NULL, QueryThreadProc, this);
	if (nResult != 0)
	{
		TRACE("Create Query Thread failed!\n");
		return false;
	}

	return true;
}
#endif

void CQueryManagerBase::Release()
{
	m_bRunning = false;
	SignalEvent(Query);
	WaitThread(m_hThreadQuery);
	m_hThreadQuery = 0;
}

void CQueryManagerBase::AddQuery(int nQueryType, int nAttachment, void* pAttachment, __int64 nTag, void* pTag, CDelHelper* pDelHelper)
{
	QueryBase *pQuery = new QueryBase(nQueryType, nAttachment, pAttachment, nTag, pTag, pDelHelper);
	Lock(&m_lockQuery);
	pQuery->m_nEnqueueTime = GetTickCount();
	m_apQueryQueue.push_back(pQuery);
	Unlock(&m_lockQuery);

	SignalEvent(Query);
}

void CQueryManagerBase::AddQuery(QueryBase *query)
{
	//CQuery *pQuery = new CQuery;
	//*pQuery = query;
	Lock(&m_lockQuery);
	query->m_nEnqueueTime = GetTickCount();
	m_apQueryQueue.push_back(query);
	Unlock(&m_lockQuery);

	SignalEvent(Query);
}

QueryBase* CQueryManagerBase::GetQuery()
{
	QueryBase *pQuery = NULL;

	Lock(&m_lockQuery);
	if( m_apQueryQueue.size() > 0)
	{
		pQuery = m_apQueryQueue.front();
		m_apQueryQueue.pop_front();
	}

	Unlock(&m_lockQuery);
	return pQuery;
}

void CQueryManagerBase::FinishQuery(QueryBase *pQuery, int nRes)
{
	pQuery->m_nResult = nRes;
	Lock(&m_lockQuery);
	m_apQueryFinished.push_back(pQuery);
	Unlock(&m_lockQuery);

	SignalEvent(Notify);
}

QueryBase* CQueryManagerBase::GetQueryFinished()
{
	QueryBase* pQuery = NULL;
	Lock(&m_lockQuery);
	if (m_apQueryFinished.size() > 0) 
	{
		pQuery = m_apQueryFinished.front();
		m_apQueryFinished.pop_front();
	}
	Unlock(&m_lockQuery);
	return pQuery;
}



#ifdef WIN32
unsigned long CQueryManagerBase::QueryThreadProc(void* pParam)
#else
#include <sys/time.h>
void* CQueryManagerBase::QueryThreadProc(void* pParam)
#endif
{
	QueryBase* pQuery;
	int nRes;
	CQueryManagerBase *pQM = (CQueryManagerBase *)pParam;
#ifdef WIN32
#else
	timeval now;
	struct timezone tz;
	timespec timeout;
	pthread_mutex_t lockNothing;
	pthread_mutex_init(&lockNothing, NULL);
#endif

	while(pQM->m_bRunning)
	{
		if((pQuery = pQM->GetQuery()) != NULL)
		{
			nRes = pQM->ProcessQuery(pQuery);
			if(pQuery)pQM->FinishQuery(pQuery, nRes);
		}
		else
		{
#ifdef WIN32
			WaitForSingleObject(pQM->m_hEventQuery, INFINITE);
#else
			gettimeofday(&now, &tz);
			timeout.tv_sec = now.tv_sec;
			timeout.tv_nsec = (now.tv_usec + 100000) << 10;

			pthread_mutex_lock(&lockNothing);
			pthread_cond_timedwait(&pQM->m_condQuery, &lockNothing, &timeout);
			pthread_mutex_unlock(&lockNothing);
#endif
		}
	}
#ifdef WIN32
#else
	pthread_mutex_destroy(&lockNothing);
#endif
	TRACE("QueryThreadProc return \n");
	return 0;
}


//file end

