#include "RoomBase.h"


RoomRoleBase::RoomRoleBase( unsigned int nRoleID )
    : m_nRoleID( nRoleID )
{
}

RoomRoleBase::~RoomRoleBase()
{
}

//////////////////////////////////////////////////////////////////////////

RoomBase::RoomBase( ERoomType eType, unsigned int nRoomID, const std::string &rName, const std::string &rPassword )
    : m_eType( eType )
    , m_nRoomID( nRoomID )
    , m_nScene( RAN_SCENE_ID )
    , m_strName( rName )
    , m_strPassword( rPassword )
{
}

RoomBase::~RoomBase()
{
    ClearPlayer();
}

void RoomBase::ChangeRoomInfo( const std::string &rName, const std::string &rPassword )
{
    m_strName = rName;
    m_strPassword = rPassword;
}

void RoomBase::SendMessage( GameMsg_Base &msg, unsigned int nExceptRoleID )
{
    for ( std::map<unsigned int, RoomRoleBase *>::iterator itr = m_mapPlayers.begin(); 
        itr != m_mapPlayers.end(); ++itr )
    {
        if ( itr->second == NULL || itr->second->GetRoleID() == nExceptRoleID )
            continue;

        itr->second->SendMessage( msg );
    }
}

bool RoomBase::PlayerIn( RoomRoleBase *pPlayer )
{
    if ( pPlayer != NULL && m_mapPlayers.find( pPlayer->GetRoleID() ) == m_mapPlayers.end() )
    {
        m_mapPlayers[pPlayer->GetRoleID()] = pPlayer;

        return true;
    }

    return false;
}

RoomRoleBase* RoomBase::PlayerOut( unsigned int nRoleID )
{
    RoomRoleBase *pRole = NULL;
    std::map<unsigned int, RoomRoleBase *>::iterator itr = m_mapPlayers.find( nRoleID );

    if ( itr != m_mapPlayers.end() )
    {
        pRole = itr->second;
        m_mapPlayers.erase( itr );
    }

    return pRole;
}

void RoomBase::RemovePlayer( unsigned int nRoleID )
{
    std::map<unsigned int, RoomRoleBase *>::iterator itr = m_mapPlayers.find( nRoleID );

    if ( itr != m_mapPlayers.end() )
    {
        SAFE_DELETE( itr->second );
        m_mapPlayers.erase( itr );
    }
}

void RoomBase::ClearPlayer()
{
    for ( std::map<unsigned int, RoomRoleBase *>::iterator itr = m_mapPlayers.begin(); 
        itr != m_mapPlayers.end(); ++itr )
    {
        SAFE_DELETE( itr->second );
    }

    m_mapPlayers.clear();
}

RoomRoleBase* RoomBase::FindPlayer( unsigned int nRoleID ) const
{
    std::map<unsigned int, RoomRoleBase *>::const_iterator itr = m_mapPlayers.find( nRoleID );

    if ( itr != m_mapPlayers.end() )
        return itr->second;

    return NULL;
}

