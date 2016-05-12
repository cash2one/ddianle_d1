#include "Query.h"


QueryBase::QueryBase( int nQueryType, int nAttachment, void* pAttachment, __int64 nTag, void* pTag, CDelHelper* pDelHelper )
    : m_nSessionID( 0 )
    , m_nEntityIndex( 0 )
    , m_nRoleID( 0 )
    , m_nQueryType( nQueryType )
    , m_nAttachment( nAttachment )
    , m_pAttachment( pAttachment )
    , m_nTag( nTag )
    , m_pTag( pTag )
    , m_nResult( -1 )
    , m_nEnqueueTime( 0 )
{
    if ( pDelHelper != NULL )
        m_delHelper = *pDelHelper;
}

QueryBase::~QueryBase(void)
{
}

size_t QueryBase::ReleaseMemberMemory()
{
    size_t nSize = m_delHelper.Del();
    m_pAttachment = NULL;
    m_pTag = NULL;

    return nSize;
}

#ifdef _GAMEMSG_MEMPOOL
#include "MemPool.h"
extern CMemPoolMgr* g_pMemPoolMgrGameMsg;

void* QueryBase::operator new ( unsigned int nSize )
{
    return g_pMemPoolMgrGameMsg->Alloc( nSize );
}

void QueryBase::operator delete ( void* pvMem )
{
    if ( pvMem )
        g_pMemPoolMgrGameMsg->Free( pvMem );
}
#endif

