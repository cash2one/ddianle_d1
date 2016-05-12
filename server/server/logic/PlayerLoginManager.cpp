#include "PlayerLoginManager.h"
#include "../../datastructure/RoleEntity.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../socket/Log.h"
#include <ctime>

enum ELogicSlotState
{
    ELogicSlotState_Invalid = 0,        // 初始状态
    ELogicSlotState_EnteredLoginQueue,  // 进入登录队列
    ELogicSlotState_EnteredDBQueue,     // 进入DB加载队列
    ELogicSlotState_WaitingCache,       // 等待Cache
    ELogicSlotState_LoadingFromCache,   // Cache加载中
    ELogicSlotState_InGame              // 游戏中
};

LogicSlot::LogicSlot()
    : m_nState( ELogicSlotState_Invalid )
    , m_bSwitchLine( false )
{
    m_tag.Clear();
}

SharedPtr<LogicSlot> LogicSlot::Create()
{
    return SharedPtr<LogicSlot>( new LogicSlot );
}

void LogicSlot::SetInGame()
{
    m_nState = ELogicSlotState_InGame;
}

bool LogicSlot::IsInGame() const
{
    return m_nState == ELogicSlotState_InGame;
}

LoginElement::LoginElement( unsigned int nExpirationTime, SharedPtr<LogicSlot> sharedSlot )
    : m_nExpirationTime( nExpirationTime )
    , m_sharedSlot( sharedSlot )
{
}

LoginElement::LoginElement( const LoginElement &ele )
    : m_nExpirationTime( ele.m_nExpirationTime )
    , m_sharedSlot( ele.m_sharedSlot )
{
}

LoginElement& LoginElement::operator= (const LoginElement &ele)
{
    m_nExpirationTime = ele.m_nExpirationTime;
    m_sharedSlot = ele.m_sharedSlot;

    return *this;
}

LoadingCacheData::LoadingCacheData()
    : m_nAccountID( 0 )
    , m_nKey( ERoleCacheInfoEnumMax )
    , m_nMaxSize( 0 )
    , m_nCurrentSize( 0 )
{
}

void LoadingCacheData::Reset()
{
    m_nAccountID = 0;
    m_nKey = ERoleCacheInfoEnumMax;
    m_nMaxSize = 0;
    m_nCurrentSize = 0;
    m_buff.clear();
}

//////////////////////////////////////////////////////////////////////////

#define CHECK_WAITING_INTERVAL  2000    // ms
#define MAX_WAITING_TIME        10      // s
typedef std::map<unsigned int, SharedPtr<LogicSlot> >   LogicSlotMap;

PlayerLoginManager::PlayerLoginManager()
    : m_nLastTimeElapse( 0 )
{
}

PlayerLoginManager::~PlayerLoginManager()
{
    m_mapSlot.clear();
    m_listLoginQueue.clear();
    m_listDBQueue.clear();
    m_loadingCacheData.Reset();
}

void PlayerLoginManager::OnUpdate( int nTimeElapse )
{
    m_nLastTimeElapse += nTimeElapse;

    if ( m_nLastTimeElapse >= CHECK_WAITING_INTERVAL )
    {
        m_nLastTimeElapse -= CHECK_WAITING_INTERVAL;

        unsigned int nNow = (unsigned int)time( NULL );
        while ( !m_listLoginQueue.empty() )
        {
            std::list<LoginElement>::iterator itr = m_listLoginQueue.begin();
            if ( itr->m_nExpirationTime > nNow )
                break;

            m_mapSlot.erase( itr->m_sharedSlot->m_tag.m_nAccount );
            m_listLoginQueue.erase( itr );
        }
    }
}

SharedPtr<LogicSlot> PlayerLoginManager::GetPlayerInfo( unsigned int nAccountID ) const
{
    LogicSlotMap::const_iterator itr = m_mapSlot.find( nAccountID );

    if ( itr != m_mapSlot.end() )
        return itr->second;

    return SharedPtr<LogicSlot>();
}

SharedPtr<LogicSlot> PlayerLoginManager::GetPlayerInfoAndRemove( unsigned int nAccountID )
{
    LogicSlotMap::iterator itr = m_mapSlot.find( nAccountID );
    SharedPtr<LogicSlot> res;

    if ( itr != m_mapSlot.end() )
    {
        res = itr->second;
        m_mapSlot.erase( itr );
    }

    return res;
}

SharedPtr<LogicSlot> PlayerLoginManager::GetNextQueryPlayer()
{
    while ( !m_listDBQueue.empty() )
    {
        SharedPtr<LogicSlot> res = m_listDBQueue.front();
        m_listDBQueue.pop_front();

        if ( IsPlayerLoginInfoSame( res ) )
        {
            if ( !res->m_sharedRole.IsValid() )
            {
                res->m_sharedRole = SharedPtr<ROLEINFO_DB>( new ROLEINFO_DB[MAX_ROLE_IN_ACCOUNT], ArrayPtrDtor() );
            }

            if ( res->m_sharedRole.IsValid() )
            {
                res->m_sharedRole->nAccount = res->m_tag.m_nAccount;
                res->m_sharedRole->m_nVersionType = res->m_tag.m_nVersionType;

                return res;
            }
        }
    }

    return SharedPtr<LogicSlot>();
}

void PlayerLoginManager::GetAllPlayerAccountIDBySlot( unsigned short nSlot, std::list<unsigned int> &rAllPlayerIDs ) const
{
    for ( LogicSlotMap::const_iterator itr = m_mapSlot.begin(); itr != m_mapSlot.end(); ++itr )
    {
        if ( itr->second.IsValid() && itr->second->m_tag.m_nSlot == nSlot )
        {
            rAllPlayerIDs.push_back( itr->second->m_tag.m_nAccount );
        }
    }
}

void PlayerLoginManager::EnteringLoginQueue( unsigned int nAccountID, SharedPtr<LogicSlot> slot, bool bFromCache /* = true */ )
{
    if ( nAccountID == 0 || !slot.IsValid() )
        return;

     std::pair<LogicSlotMap::iterator, bool> res = m_mapSlot.insert( std::make_pair( nAccountID, slot ) );
     if ( res.second )
     {
         if ( bFromCache )
         {
             unsigned int nNow = (unsigned int)time( NULL ) + MAX_WAITING_TIME;
             m_listLoginQueue.push_back( LoginElement( nNow, slot ) );
             slot->m_nState = ELogicSlotState_EnteredLoginQueue;
         }
         else
         {
             m_listDBQueue.push_back( slot );
             slot->m_nState = ELogicSlotState_EnteredDBQueue;
         }
     }
}

void PlayerLoginManager::RemovePlayer( unsigned int nAccountID )
{
    m_mapSlot.erase( nAccountID );
}

int PlayerLoginManager::RequestPlayerCacheRes( unsigned int nAccountID, __int64 nSessionID, bool bCacheSuccess, bool bHasCache )
{
    if ( nAccountID == 0 || m_listLoginQueue.empty() || m_mapSlot.empty() )
        return ERequestCacheRes_NotFound;

    LogicSlotMap::const_iterator slotItr = m_mapSlot.find( nAccountID );
    if ( slotItr == m_mapSlot.end() || !slotItr->second.IsValid() )
    {
        return ERequestCacheRes_InvalidSlot; // 没有找到相应的Slot数据，表示该玩家已退出
    }
    else if ( slotItr->second->m_tag.m_nSessionID != nSessionID )
    {
        return ERequestCacheRes_InvalidSessionID; // 与Slot数据不匹配，表示该玩家重新登录，此时数据为第一次请求数据
    }
    else if ( slotItr->second->m_nState != ELogicSlotState_EnteredLoginQueue )
    {
        return ERequestCacheRes_InvalidState; // 状态不正确，表示玩家已进入DB队列或收到异常数据
    }
    else if ( !bCacheSuccess )
    {
        return ERequestCacheRes_NotCacheSuccess;
    }

    while ( !m_listLoginQueue.empty() )
    {
        LoginElement &ele = m_listLoginQueue.front();

        if ( ele.m_sharedSlot == slotItr->second )
            break;

        m_mapSlot.erase( ele.m_sharedSlot->m_tag.m_nAccount );
        m_listLoginQueue.pop_front();
    }

    if ( bHasCache )
    {
        slotItr->second->m_nState = ELogicSlotState_WaitingCache;
        m_loadingCacheData.m_nAccountID = nAccountID;
    }
    else
    {
        MoveFromLoginToDB( m_listLoginQueue.begin() );
    }

    return ERequestCacheRes_Success;
}

void PlayerLoginManager::ReceiveCacheDataBegin( unsigned int nAccountID )
{
    if ( nAccountID == 0 || m_loadingCacheData.m_nAccountID != nAccountID )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u)", nAccountID );
        return;
    }

    LogicSlotMap::iterator slotItr = m_mapSlot.find( nAccountID );
    if ( slotItr == m_mapSlot.end() || !slotItr->second.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u) in mapSlot.", nAccountID );
        return;
    }
    else if ( slotItr->second->m_nState != ELogicSlotState_WaitingCache )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache state is not ELogicSlotState_WaitingCache." );
        return;
    }

    slotItr->second->m_nState = ELogicSlotState_LoadingFromCache;
    slotItr->second->m_sharedRole = SharedPtr<ROLEINFO_DB>( new ROLEINFO_DB[MAX_ROLE_IN_ACCOUNT], ArrayPtrDtor() );
}

void PlayerLoginManager::ReceiveCacheData( unsigned int nAccountID, unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *pData )
{
    if ( nAccountID == 0 || m_loadingCacheData.m_nAccountID != nAccountID )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u)", nAccountID );
        return;
    }
    else if ( nMaxSize == 0 || nSize == 0 )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache size error. Max size = %u, size = %u, key = %u", nMaxSize, nSize, nKey );
        return;
    }
    else if ( pData == NULL )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache data is NULL." );
        return;
    }

    LogicSlotMap::iterator slotItr = m_mapSlot.find( nAccountID );
    if ( slotItr == m_mapSlot.end() || !slotItr->second.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u) in mapSlot.", nAccountID );
        return;
    }
    else if ( !slotItr->second->m_sharedRole.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]role info is not valid" );
        return;
    }
    else if ( slotItr->second->m_nState != ELogicSlotState_LoadingFromCache )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache state is not ELogicSlotState_LoadingFromCache." );
        return;
    }

    if ( m_loadingCacheData.m_nKey == ERoleCacheInfoEnumMax )
    {
        m_loadingCacheData.m_nKey = nKey;
        m_loadingCacheData.m_nMaxSize = nMaxSize;
        m_loadingCacheData.m_nCurrentSize = nSize;
        m_loadingCacheData.m_buff.append( pData, nSize );
    }
    else if ( m_loadingCacheData.m_nKey == nKey )
    {
        if ( m_loadingCacheData.m_nCurrentSize <= m_loadingCacheData.m_nMaxSize )
        {
            m_loadingCacheData.m_nCurrentSize += nSize;
            m_loadingCacheData.m_buff.append( pData, nSize );
        }
    }
    else if ( m_loadingCacheData.m_nMaxSize == m_loadingCacheData.m_nCurrentSize )
    {
        CRoleEntity::CacheDecode( slotItr->second->m_sharedRole.Get(), m_loadingCacheData.m_nKey,
            const_cast<char *>( m_loadingCacheData.m_buff.c_str() ), (int)m_loadingCacheData.m_nMaxSize );

        m_loadingCacheData.m_nKey = nKey;
        m_loadingCacheData.m_nMaxSize = nMaxSize;
        m_loadingCacheData.m_nCurrentSize = nSize;
        m_loadingCacheData.m_buff.assign( pData, nSize );
    }
    else
    {
        m_loadingCacheData.m_nKey = ERoleCacheInfoEnumMax;
        m_loadingCacheData.m_buff.clear();
    }
}

void PlayerLoginManager::ReceiveCacheDataEnd( unsigned int nAccountID )
{
    if ( nAccountID == 0 || m_loadingCacheData.m_nAccountID != nAccountID )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u)", nAccountID );
        return;
    }

    LogicSlotMap::const_iterator slotItr = m_mapSlot.find( nAccountID );
    if ( slotItr == m_mapSlot.end() || !slotItr->second.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Can not found account(%u) in mapSlot.", nAccountID );
        return;
    }
    else if ( !slotItr->second->m_sharedRole.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]role info is not valid" );
        return;
    }
    else if ( slotItr->second->m_nState != ELogicSlotState_LoadingFromCache )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache state is not ELogicSlotState_LoadingFromCache." );
        return;
    }

    if ( m_loadingCacheData.m_nKey != ERoleCacheInfoEnumMax && m_loadingCacheData.m_nMaxSize == m_loadingCacheData.m_nCurrentSize )
    {
        CRoleEntity::CacheDecode( slotItr->second->m_sharedRole.Get(), m_loadingCacheData.m_nKey,
            const_cast<char *>( m_loadingCacheData.m_buff.c_str() ), (int)m_loadingCacheData.m_nMaxSize );
    }

    if ( !m_listLoginQueue.empty() )
        MoveFromLoginToDB( m_listLoginQueue.begin() );
}

bool PlayerLoginManager::IsPlayerLoginInfoSame( SharedPtr<LogicSlot> slot ) const
{
    if ( slot.IsValid() )
    {
        LogicSlotMap::const_iterator itr = m_mapSlot.find( slot->m_tag.m_nAccount );

        if ( itr != m_mapSlot.end() && itr->second.IsValid() && itr->second == slot )
        {
            return true;
        }
    }

    return false;
}

void PlayerLoginManager::MoveFromLoginToDB( std::list<LoginElement>::iterator itr )
{
    if ( itr == m_listLoginQueue.end() )
        return;

    SharedPtr<LogicSlot> sharedSlot = itr->m_sharedSlot;
    m_listLoginQueue.erase( itr );

    if ( m_loadingCacheData.m_nAccountID == sharedSlot->m_tag.m_nAccount )
        m_loadingCacheData.Reset();

    if ( !IsPlayerLoginInfoSame( sharedSlot ) )
        return;

    sharedSlot->m_nState = ELogicSlotState_EnteredDBQueue;
    m_listDBQueue.push_back( sharedSlot );
}

