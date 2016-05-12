#include "LinkingInfos.h"
#include "GameServerLinkInfo.h"


LinkingInfos::LinkingInfos(void)
    : m_nMaxCount( 0 )
{
}

LinkingInfos::~LinkingInfos(void)
{
    Clear();
}

void LinkingInfos::Initialize( unsigned short nMaxCount )
{
    if ( m_nMaxCount != 0 )
        return;

    m_nMaxCount = nMaxCount;
    m_vectSlot.resize( nMaxCount, nMaxCount );
    m_vectLinkInfo.resize( nMaxCount );
}

void LinkingInfos::Clear()
{
    std::fill( m_vectSlot.begin(), m_vectSlot.end(), m_nMaxCount );

    for ( std::vector<GameServerLinkInfo *>::iterator itr = m_vectLinkInfo.begin(); 
        itr != m_vectLinkInfo.end(); ++itr )
    {
        SAFE_DELETE( *itr );
    }
}

bool LinkingInfos::AddServerInfo( const GameServerLinkInfo &rInfo )
{
    do 
    {
        int nLine = rInfo.GetLine();
        unsigned short nSlot = rInfo.GetSlot();

        BREAK_IF( !IsValidLine( nLine ) );
        BREAK_IF( nSlot >= m_vectLinkInfo.size() );

        unsigned short nOldSlot = m_vectSlot[nLine];
        BREAK_IF( nOldSlot != m_nMaxCount || nOldSlot == nSlot );

        GameServerLinkInfo *pInfo = new GameServerLinkInfo( rInfo );
        m_vectSlot[nLine] = nSlot;
        m_vectLinkInfo[nSlot] = pInfo;

        return true;
    } while (0);

    return false;
}

int LinkingInfos::RemoveServerInfoBySlot( unsigned short nSlot )
{
    if ( nSlot >= m_vectLinkInfo.size() || m_vectLinkInfo[nSlot] == NULL )
        return INVALID_LINE;

    int nLine = m_vectLinkInfo[nSlot]->GetLine();

    SAFE_DELETE( m_vectLinkInfo[nSlot] );

    if ( IsValidLine( nLine ) )
        m_vectSlot[nLine] = m_nMaxCount;

    return nLine;
}

void LinkingInfos::SetClientBySlot( unsigned short nSlot, unsigned short nClient )
{
    GameServerLinkInfo *pInfo = FindLinkInfoBySlot( nSlot );
    if ( pInfo != NULL )
        pInfo->SetClient( nClient );
}

int LinkingInfos::GetAvailableLine() const
{
    for ( std::vector<GameServerLinkInfo *>::const_iterator itr = m_vectLinkInfo.begin(); 
        itr != m_vectLinkInfo.end(); ++itr )
    {
        if ( *itr != NULL && (*itr)->GetLine() != INVALID_LINE )
        {
            return (*itr)->GetLine();
        }
    }

    return INVALID_LINE;
}

int LinkingInfos::GetLineBySlot( unsigned short nSlot ) const
{
    const GameServerLinkInfo *pInfo = FindLinkInfoBySlot( nSlot );

    if ( pInfo == NULL )
        return INVALID_LINE;

    return pInfo->GetLine();
}

unsigned short LinkingInfos::GetSlotByLine( int nLine ) const
{
    if ( IsValidLine( nLine ) )
        return m_vectSlot[nLine];

    return m_nMaxCount;
}

const GameServerLinkInfo* LinkingInfos::GetLinkInfoByLine( int nLine ) const
{
    return FindLinkInfoBySlot( GetSlotByLine( nLine ) );
}

const GameServerLinkInfo* LinkingInfos::GetLinkInfoBySlot( unsigned short nSlot ) const
{
    return FindLinkInfoBySlot( nSlot );
}

GameServerLinkInfo* LinkingInfos::FindLinkInfoBySlot( unsigned short nSlot ) const
{
    if ( nSlot < m_vectLinkInfo.size() )
        return m_vectLinkInfo[nSlot];

    return NULL;
}

bool LinkingInfos::IsValidLine( int nLine ) const
{
    if ( 0 <= nLine && nLine < (int)m_vectSlot.size() )
        return true;

    return false;
}

