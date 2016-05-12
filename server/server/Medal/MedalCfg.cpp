#include "MedalCfg.h"

#include "../../datastructure/datainfor/FileStream.h"

CMedalCfg::CMedalCfg()
    : m_nId( 0 )
    , m_bIsForever( false )
    , m_nDuration( 0 )
    , m_nType( 0 )
    , m_nNeedMedalMoney( 0 )
    , m_nSortKey( 0 )
    , m_strName( "" )
    , m_strTypeDescribe( "" )
    , m_strDescribe( "" )
    , m_strIcon( "" )
{
}

CMedalCfg::~CMedalCfg()
{
}

bool CMedalCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
        return false;

    rFileStream.ReadInt( m_nId );
    rFileStream.ReadInt( m_nDuration );
    rFileStream.ReadBool( m_bIsForever );
    rFileStream.ReadInt( m_nType );
    rFileStream.ReadString2( m_strTypeDescribe );
    rFileStream.ReadString2( m_strName );
    rFileStream.ReadString2( m_strDescribe );
    rFileStream.ReadString2( m_strIcon );
    rFileStream.ReadInt( m_nNeedMedalMoney );
    rFileStream.ReadInt( m_nSortKey );
    rFileStream.ReadInt(m_nMedalScore);
    return true;
}

//////////////////////////////////////////////////////////////////////////

CMedalProgressCfg::CMedalProgressCfg()
    : m_nId( 0 )
    , m_nTotalProgress( 0 )
    , m_nType( 0 )
    , m_nMedalID( 0 )
    , m_nSortKey( 0 )
    , m_nVisibility( 0 )
    , m_nAllowGetStartTime( 0 )
    , m_nAllowGetEndTime( 0 )
    , m_strDescribe( "" )
{
    m_listSpecialParams.clear();
}

CMedalProgressCfg::~CMedalProgressCfg()
{
}

bool CMedalProgressCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
        return false;

    rFileStream.ReadInt( m_nId );
    rFileStream.ReadString2( m_strDescribe );
    rFileStream.ReadInt( m_nTotalProgress );

    unsigned int nParams = 0;
    unsigned short nCount = 0;
    rFileStream.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        rFileStream.ReadUInt( nParams );

        m_listSpecialParams.push_back( nParams );
    }

    rFileStream.ReadInt( m_nType );
    rFileStream.ReadUInt( m_nAllowGetStartTime );
    rFileStream.ReadUInt( m_nAllowGetEndTime );
    rFileStream.ReadInt( m_nMedalID );
    rFileStream.ReadInt( m_nSortKey );
    rFileStream.ReadInt( m_nVisibility );

    return true;
}
 
