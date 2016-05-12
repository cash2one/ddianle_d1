#include "PlayerCacheData.h"
#include "../../socket/Log.h"

class PlayerCacheSegment
{
public:
    PlayerCacheSegment( unsigned int nMaxSize, unsigned short nKey );
    ~PlayerCacheSegment();

public:
    unsigned int GetMaxSize() const { return m_nMaxSize; }
    unsigned short GetKey() const { return m_nKey; }

    void AddData( unsigned int nSize, const char *data );
    void PopData( std::string &rData );

    bool IsSizeCorrect() const { return m_nMaxSize == m_nCurrentSize; }
    bool IsEmpty() const { return m_data.empty(); }

private:
    unsigned int            m_nMaxSize;
    unsigned int            m_nCurrentSize;
    unsigned short          m_nKey;
    std::list<std::string>  m_data;
};

PlayerCacheSegment::PlayerCacheSegment( unsigned int nMaxSize, unsigned short nKey )
    : m_nMaxSize( nMaxSize )
    , m_nCurrentSize( 0 )
    , m_nKey( nKey )
{
}

PlayerCacheSegment::~PlayerCacheSegment()
{
    m_data.clear();
}

void PlayerCacheSegment::AddData( unsigned int nSize, const char *data )
{
    if ( m_nMaxSize < m_nCurrentSize || data == NULL )
        return;

    m_data.push_back( std::string( data, nSize ) );
    m_nCurrentSize += nSize;
}

void PlayerCacheSegment::PopData( std::string &rData )
{
    if ( m_data.empty() )
        return;

    rData.swap( m_data.front() );
    m_data.pop_front();
}

//////////////////////////////////////////////////////////////////////////

PlayerCacheData::PlayerCacheData( unsigned int nAccountID, unsigned int nExpirationTime )
    : m_nAccountID( nAccountID )
    , m_nExpirationTime( nExpirationTime )
    , m_nState( EPlayerCacheState_None )
{
}

PlayerCacheData::~PlayerCacheData()
{
    Clear();
}

void PlayerCacheData::AddCacheData( unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data )
{
    if ( nMaxSize == 0 )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache data(%u) max size equal 0.", nKey );
        SetState( EPlayerCacheState_Error );
    }
    else if ( nSize == 0 )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache data(%u) size equal 0.", nKey );
        SetState( EPlayerCacheState_Error );
    }
    else if ( data == NULL )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache data(%u) is NULL.", nKey );
        SetState( EPlayerCacheState_Error );
    }

    if ( m_nState == EPlayerCacheState_Error )
        return;

    if ( m_listSegments.empty() )
    {
        CreateCacheSegment( nKey, nMaxSize, nSize, data );
        return;
    }

    PlayerCacheSegment *segment = m_listSegments.back();

    if ( segment == NULL )
    {
        m_listSegments.pop_back();

        CreateCacheSegment( nKey, nMaxSize, nSize, data );
    }
    else if ( segment->GetKey() != nKey )
    {
        if ( !segment->IsSizeCorrect() ) // 上个Key数据异常
        {
            WriteLog( LOGLEVEL_ERROR, "[Cache]Last key(%u) size is not correct.", segment->GetKey() );
            SetState( EPlayerCacheState_Error );
        }
        else
        {
            CreateCacheSegment( nKey, nMaxSize, nSize, data );
        }
    }
    else
    {
        segment->AddData( nSize, data );
    }
}

void PlayerCacheData::AddCacheDataEnd()
{
    if ( m_nState == EPlayerCacheState_Error )
    {
        return;
    }
    else if ( m_listSegments.empty() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache segment is empty." );
        SetState( EPlayerCacheState_Error );
        return;
    }

    PlayerCacheSegment *segment = m_listSegments.back();
    if ( segment == NULL || !segment->IsSizeCorrect() )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Last key size is not correct in end." );
        SetState( EPlayerCacheState_Error );
        return;
    }

    WriteLog( LOGLEVEL_DEBUG, "[Cache]Cache success." );
    SetState( EPlayerCacheState_Success );
}

bool PlayerCacheData::PopCacheData( unsigned short &rKey, unsigned int &rMaxSize, std::string &rData )
{
    if ( m_nState == EPlayerCacheState_Error )
        return false;
    else if ( m_listSegments.empty() )
        return false;

    PlayerCacheSegment *segment = m_listSegments.front();
    if ( segment == NULL )
    {
        m_listSegments.pop_front();
        return false;
    }

    rKey = segment->GetKey();
    rMaxSize = segment->GetMaxSize();
    rData.clear();
    segment->PopData( rData );

    if ( segment->IsEmpty() )
    {
        delete segment, segment = NULL;
        m_listSegments.pop_front();
    }

    return true;
}

bool PlayerCacheData::IsCacheSuccess() const
{
    return m_nState == EPlayerCacheState_Success;
}

bool PlayerCacheData::HasData() const
{
    return !m_listSegments.empty();
}

void PlayerCacheData::SetState( unsigned char nState )
{
    m_nState = nState;

    if ( m_nState == EPlayerCacheState_Error )
        Clear();
}

void PlayerCacheData::CreateCacheSegment( unsigned short nKey, unsigned int nMaxSize, unsigned int nSize, const char *data )
{
    if ( data == NULL )
    {
        WriteLog( LOGLEVEL_ERROR, "[Cache]Cache data(%u) is NULL.", nKey );
        SetState( EPlayerCacheState_Error );
        return;
    }

    PlayerCacheSegment *segment = new PlayerCacheSegment( nMaxSize, nKey );
    segment->AddData( nSize, data );
    m_listSegments.push_back( segment );
}

void PlayerCacheData::Clear()
{
    while ( !m_listSegments.empty() )
    {
        delete m_listSegments.front();
        m_listSegments.pop_front();
    }
}

