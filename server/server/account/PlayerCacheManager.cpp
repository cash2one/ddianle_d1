#include "PlayerCacheManager.h"
#include "PlayerCacheData.h"
#include "../../socket/Log.h"
#include <ctime>

#define CHECK_PLAYER_INTERVAL       300000  // ms
#define CHECK_RECEIVING_INTERVAL    5000    // ms
#define MAX_RECEIVING_TIME          20      // s // 最大接收时间
#define REMOVE_COUNT_PRE_TIME       10
#define MAX_INVALID_CACHE_TIME      30      // s    // 无效Cache静默时间

typedef std::map<unsigned int, PlayerCacheData *>   PlayerCacheMap;
typedef PlayerCacheMap::iterator                    PlayerCacheMapItr;

PlayerCacheManager::PlayerCacheManager()
    : m_nMaxCount( 500 )
    , m_nValidTime( 1800 )
    , m_nCheckPlayerElapse( 0 )
    , m_nCheckReceivingElapse( 0 )
{
}

PlayerCacheManager::~PlayerCacheManager()
{
    PlayerCacheMapItr itr;

    for ( itr = m_mapPlayerCache.begin(); itr != m_mapPlayerCache.end(); ++itr )
        delete itr->second, itr->second = NULL;

    for ( itr = m_mapReceivingCache.begin(); itr != m_mapReceivingCache.end(); ++itr )
        delete itr->second, itr->second = NULL;

    m_mapPlayerCache.clear();
    m_mapReceivingCache.clear();
    m_playerCacheQueue.clear();
    m_receivingCacheQueue.clear();
}

void PlayerCacheManager::Init( unsigned int nMaxCount, unsigned int nValidTime )
{
    m_nMaxCount = nMaxCount;
    m_nValidTime = nValidTime;
}

void PlayerCacheManager::OnUpdate( int nTimeElapse )
{
    m_nCheckPlayerElapse += nTimeElapse;
    m_nCheckReceivingElapse += nTimeElapse;

    if ( m_nCheckPlayerElapse >= CHECK_PLAYER_INTERVAL )
    {
        m_nCheckPlayerElapse -= CHECK_PLAYER_INTERVAL;

        RemoveExpiredCache( m_playerCacheQueue, m_mapPlayerCache, REMOVE_COUNT_PRE_TIME );
    }

    if ( m_nCheckReceivingElapse >= CHECK_RECEIVING_INTERVAL )
    {
        m_nCheckReceivingElapse -= CHECK_RECEIVING_INTERVAL;

        RemoveExpiredCache( m_receivingCacheQueue, m_mapReceivingCache, REMOVE_COUNT_PRE_TIME );
    }
}

void PlayerCacheManager::ReceiveCacheDataBegin( unsigned int nAccountID )
{
    if ( nAccountID == 0 )
        return;
    else if ( CheckCached( nAccountID ) )
        return;
    else if ( CheckInReceiving( nAccountID ) )
        return;

    unsigned int nExpirationTime = (unsigned int)time( NULL ) + MAX_RECEIVING_TIME;
    m_mapReceivingCache[nAccountID] = new PlayerCacheData( nAccountID, nExpirationTime );
    m_receivingCacheQueue.push_back( nAccountID );
}

void PlayerCacheManager::ReceiveCacheData( unsigned int nAccountID, unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data )
{
    if ( CheckCached( nAccountID ) )
        return;

    PlayerCacheMapItr itr = m_mapReceivingCache.find( nAccountID );
    if ( itr == m_mapReceivingCache.end() || itr->second == NULL )
    {
        AddErrorCache( nAccountID );

        if ( itr != m_mapReceivingCache.end() )
            m_mapReceivingCache.erase( itr );

        return;
    }

    itr->second->AddCacheData( nKey, nMaxSize, nSize, data );
}

void PlayerCacheManager::ReceiveCacheDataEnd( unsigned int nAccountID )
{
    if ( nAccountID == 0 )
        return;
    else if ( CheckCached( nAccountID ) )
        return;

    PlayerCacheMapItr itr = m_mapReceivingCache.find( nAccountID );
    if ( itr == m_mapReceivingCache.end() || itr->second == NULL )
    {
        AddErrorCache( nAccountID );

        if ( itr != m_mapReceivingCache.end() )
            m_mapReceivingCache.erase( itr );

        return;
    }

    PlayerCacheData *pData = itr->second;
    pData->AddCacheDataEnd();

    unsigned int nExpirationTime = (unsigned int)time( NULL ) + m_nValidTime;
    pData->SetExpirationTime( nExpirationTime );
    m_mapPlayerCache[nAccountID] = pData;
    m_playerCacheQueue.push_back( nAccountID );

    while ( m_mapPlayerCache.size() > m_nMaxCount )
        RemoveExpiredCache( m_playerCacheQueue, m_mapPlayerCache, 1, true );

    m_mapReceivingCache.erase( itr );
}

bool PlayerCacheManager::IsInReceiving( unsigned int nAccountID )
{
    PlayerCacheMapItr itr = m_mapReceivingCache.find( nAccountID );

    if ( itr == m_mapReceivingCache.end() )
        return false;

    return true;
}

bool PlayerCacheManager::IsCacheSuccess( unsigned int nAccountID )
{
    if ( CheckInReceiving( nAccountID ) )
        return false;

    PlayerCacheMapItr itr = m_mapPlayerCache.find( nAccountID );
    if ( itr != m_mapPlayerCache.end() && itr->second != NULL )
        return itr->second->IsCacheSuccess();

    return true;
}

bool PlayerCacheManager::HasCached( unsigned int nAccountID )
{
    if ( CheckInReceiving( nAccountID ) )
        return true;

    PlayerCacheMapItr itr = m_mapPlayerCache.find( nAccountID );
    if ( itr != m_mapPlayerCache.end() && itr->second != NULL )
        return true;

    return false;
}

bool PlayerCacheManager::GetNextPlayerCacheData( unsigned int nAccountID, unsigned short &rKey, unsigned int &rMaxSize, std::string &rData )
{
    PlayerCacheMapItr itr = m_mapPlayerCache.find( nAccountID );
    if ( itr == m_mapPlayerCache.end() || itr->second == NULL || !itr->second->HasData() )
    {
        if ( itr != m_mapPlayerCache.end() )
        {
            delete itr->second, itr->second = NULL;
            m_mapPlayerCache.erase( itr );
        }

        return false;
    }

    return itr->second->PopCacheData( rKey, rMaxSize, rData );
}

void PlayerCacheManager::RemoveExpiredCache( std::list<unsigned int> &cacheQueue, std::map<unsigned int, PlayerCacheData *> &cache,
    unsigned int nCount, bool bForce /* = false */ )
{
    if ( cacheQueue.empty() )
        return;

    unsigned int nNow = (unsigned int)time( NULL );
    std::list<unsigned int>::iterator queItr = cacheQueue.begin();

    for ( unsigned int i = 0; i < nCount && queItr != cacheQueue.end(); ++queItr )
    {
        PlayerCacheMapItr cacheItr = cache.find( *queItr );

        if ( cacheItr == cache.end() )
        {
            continue;
        }
        else if ( cacheItr->second == NULL )
        {
            cache.erase( cacheItr );
            continue;
        }
        else if ( !bForce && cacheItr->second->GetExpirationTime() > nNow )
        {
            break;
        }

        delete cacheItr->second, cacheItr->second = NULL;
        cache.erase( cacheItr );

        ++i;
    }

    cacheQueue.erase( cacheQueue.begin(), queItr );
}

bool PlayerCacheManager::CheckCached( unsigned int nAccountID )
{
    PlayerCacheMapItr itr = m_mapPlayerCache.find( nAccountID );

    if ( itr == m_mapPlayerCache.end() )
        return false;

    WriteLog( LOGLEVEL_ERROR, "[Cache]Player's cache is cached." );

    if ( itr->second == NULL )
        m_mapPlayerCache.erase( itr );
    else
        itr->second->SetState( EPlayerCacheState_Error );

    return true;
}

bool PlayerCacheManager::CheckInReceiving( unsigned int nAccountID )
{
    if ( !IsInReceiving( nAccountID ) )
        return false;

    WriteLog( LOGLEVEL_ERROR, "[Cache]Cache is in receiving." );

    AddErrorCache( nAccountID );

    delete m_mapReceivingCache[nAccountID];
    m_mapReceivingCache.erase( nAccountID );

    return true;
}

void PlayerCacheManager::AddErrorCache( unsigned int nAccountID )
{
    unsigned int nExpirationTime = (unsigned int)time( NULL ) + MAX_INVALID_CACHE_TIME;
    m_mapPlayerCache[nAccountID] = new PlayerCacheData( nAccountID, nExpirationTime );
    m_mapPlayerCache[nAccountID]->SetState( EPlayerCacheState_Error );
    m_playerCacheQueue.push_back( nAccountID );
}

