//////////////////////////////////////////////////////////////////////////
//mempool.cpp
#include "StdAfx.h"
#include "MemPool.h"
#include "Utility.h"

#ifdef WIN32
#define Lock(x)			EnterCriticalSection(x)
#define Unlock(x)		LeaveCriticalSection(x)
#else
#define Lock(x)			pthread_mutex_lock(x)
#define Unlock(x)		pthread_mutex_unlock(x)
#endif

#ifdef SERVER_USE_MEMORY_POOL

CMemPoolMgr::CMemPool::CMemPool(unsigned short nNodeSize, unsigned short nNodeCount /* = 256 */)
{
	m_pHead = NULL;
	m_pTemp = NULL;
	m_nNodeSize = nNodeSize;
	m_nNodeCount = nNodeCount;
	//申请内存
	Resize();
}

CMemPoolMgr::CMemPool::~CMemPool()
{
	//释放内存
	while (m_listBlock.size()) 
	{
		delete m_listBlock.back();
		m_listBlock.pop_back();
	}
}

void* CMemPoolMgr::CMemPool::Alloc()
{
	if ( m_pHead == NULL )
	{
		Resize();
	}
	m_pTemp = m_pHead;
	if (m_pHead) 
	{
		m_pHead = (int*)(*m_pHead);
	}
//	TRACE("alloc(%d) return %d\n", m_nNodeSize, m_pTemp);
	return m_pTemp;
}

void CMemPoolMgr::CMemPool::Free(void * &ptr)
{
#ifdef _DEBUG
	//判断指针是否在此mempool内
	int value = (int)ptr;
	list< int* >::iterator it;
	for(it = m_listBlock.begin(); it != m_listBlock.end(); it++)
	{
		int nPtr = (int)(*it);
		unsigned short i = 0;
		for(i = 0; i < m_nNodeCount; i++)
		{
			if(value == nPtr)break;
			nPtr += m_nNodeSize;
		}
		if(i < m_nNodeCount)break;
	}
	if(it != m_listBlock.end())
	{//确实在此mempool内
	}
	else
	{
		TRACE("Invalid call function: CMemPool::Free()\n");
		return ;
	}
#endif
	m_pTemp = m_pHead;
	m_pHead = (int*)ptr;
	*m_pHead = (int)m_pTemp;
//	TRACE("free(%d) %d\n", m_nNodeSize, ptr);
};

bool CMemPoolMgr::CMemPool::PointerInPool(void* ptr)
{
	int *pInt = (int*)ptr;
	list< int* >::iterator it;
	for(it = m_listBlock.begin(); it != m_listBlock.end(); it++)
	{
		if(pInt-(*it) >= 0 && pInt-(*it) <= m_nNodeSize*m_nNodeCount)
		{//指针在池内
			if((pInt-(*it))%m_nNodeSize == 0)
			{//正确的指针值
				return true;
			}
			else
			{//错误的指针值
				TRACE("Invalid pointer!\n");
				break;
			}
		}
	}

	return false;
}

bool CMemPoolMgr::CMemPool::Resize()
{
	//分配内存
	TRACE("alloc NodeSize=%d, NodeCount=%d, Total=%d\n", m_nNodeSize, m_nNodeCount, m_nNodeSize*m_nNodeCount);
	m_pTemp = new int[ m_nNodeSize*m_nNodeCount >> 2];
	if (m_pTemp == NULL) return false;
	m_listBlock.push_back( m_pTemp );

	int *ptr = m_pTemp;
	int value = (int)m_pTemp;
	for(unsigned short i = 1; i < m_nNodeCount; i++)
	{
		value+=m_nNodeSize;
		*ptr = value;
		ptr += (m_nNodeSize>>2);
	}
	
	value = (int)m_pHead;
	*ptr = value;
	m_pHead = m_pTemp;

	
	return true;
}


CMemPoolMgr::CMemPoolMgr()
{
#ifdef WIN32
	InitializeCriticalSection(&m_lockMemPool);
#else
	pthread_mutex_init(&m_lockMemPool, NULL);
#endif
	m_bThreadSafe = true;
}

CMemPoolMgr::~CMemPoolMgr()
{
	Release();
#ifdef WIN32
	DeleteCriticalSection(&m_lockMemPool);
#else
	pthread_mutex_destroy(&m_lockMemPool);
#endif
}

bool CMemPoolMgr::Initialize(list< pair< unsigned short, unsigned short > > *plistPool /* = NULL */,bool bThreadSafe /* = true */)
{
	list< pair< unsigned short, unsigned short > > listNode;

	if(plistPool == NULL)
	{
#ifdef _DEBUG
		listNode.push_back(pair< unsigned short, unsigned short > ( 16, 256 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 32, 256 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 64, 256 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 128, 256 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 256, 256 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 512, 128) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 1024, 128) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 2048, 32) );
#else
		listNode.push_back(pair< unsigned short, unsigned short > ( 16, 1024 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 32, 1024 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 64, 1024 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 128, 512 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 256, 512 ) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 512, 512) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 1024, 512) );
		listNode.push_back(pair< unsigned short, unsigned short > ( 2048, 64) );
#endif

		plistPool = &listNode;

	}
	
	list< pair< unsigned short , unsigned short  > > ::iterator it;
	for(it = plistPool->begin(); it != plistPool->end(); it++)
	{
		unsigned short nNodeSize = (*it).first;
		nNodeSize = ((nNodeSize + 7)/8)*8;
		list< CMemPool* >::iterator itPool;
		for(itPool = m_listPool.begin(); itPool != m_listPool.end(); itPool++ )
		{
			if( (*itPool)->GetNodeSize() == nNodeSize )break;
		}
		if(itPool == m_listPool.end())//没有相同的内存池则创建之
		{
			for(itPool = m_listPool.begin(); itPool != m_listPool.end(); itPool++ )
			{
				if( (*itPool)->GetNodeSize() > nNodeSize )break;
			}
			CMemPool *pPool = new CMemPool( nNodeSize, (*it).second);
			m_listPool.insert( itPool, pPool );
		}
	}
	m_bThreadSafe = bThreadSafe;
	return true;
}

void CMemPoolMgr::Release()
{
	while(m_listPool.size())
	{
		delete m_listPool.back();
		m_listPool.pop_back();
	}
}

void *CMemPoolMgr::Alloc(unsigned short nSize)
{
	void *ptr = NULL;
	if(m_bThreadSafe)Lock(&m_lockMemPool);
	CMemPool *pPool = FindMemPool(nSize);

	if (pPool) 
	{
		ptr = pPool->Alloc();
	}
	else
	{
		ptr = (char*) new char[(nSize)] ;
	}
	if(m_bThreadSafe)Unlock(&m_lockMemPool);
	return ptr;
};

void CMemPoolMgr::Free(void* &ptr, unsigned short nSize)
{
	if(m_bThreadSafe)Lock(&m_lockMemPool);
	CMemPool *pPool = NULL;
	if(nSize == 0)pPool = FindMemPool(ptr);
	else pPool = FindMemPool(nSize);

	if (pPool) 
	{
		pPool->Free(ptr);
	}
	else
	{
		delete (int*)(ptr);
		ptr = NULL;
	}
	if(m_bThreadSafe)Unlock(&m_lockMemPool);
}

CMemPoolMgr::CMemPool* CMemPoolMgr::FindMemPool(unsigned short nSize)
{
	CMemPool *pPool = NULL;
	list< CMemPool*  >::reverse_iterator itPool;
	for(itPool = m_listPool.rbegin(); itPool != m_listPool.rend(); itPool++ )
	{
		if( (*itPool)->GetNodeSize() < nSize )break;
		pPool = (*itPool);
	}
	if( itPool == m_listPool.rend())
		pPool = NULL;

	return pPool;
}

CMemPoolMgr::CMemPool* CMemPoolMgr::FindMemPool(void* &ptr)
{
	CMemPool *pPool = NULL;
	list< CMemPool*  >::reverse_iterator itPool;
	for(itPool = m_listPool.rbegin(); itPool != m_listPool.rend(); itPool++ )
	{
		if( (*itPool)->PointerInPool(ptr) )
		{
			pPool = (*itPool);
			break;
		}
	}

	return pPool;
}

#endif //SERVER_USE_MEMORY_POOL
//End of file


