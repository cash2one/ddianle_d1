#include "HorseCfg.h"

#include "../../datastructure/datainfor/FileStream.h"

extern std::string g_sPath;

#define HORSE_FILE_NAME     "data/bytes/Vehicle.bytes"

CHorseCfg::CHorseCfg()
    : m_nHorseId(0)
    , m_nHorseItemType(0)
    , m_nHorseType(0)
    , m_nMaxPassager(0)
    , m_nSpeed(0)
{

}

CHorseCfg::~CHorseCfg()
{

}

bool CHorseCfg::LoadInfoFromFile(CFileStream &rFileStream)
{
    if ( !rFileStream.IsOpen() )
        return false;

    rFileStream.ReadUInt( m_nHorseId );
    rFileStream.ReadUInt( m_nHorseItemType );
    rFileStream.ReadUChar( m_nHorseType );
    rFileStream.ReadInt( m_nMaxPassager );
    rFileStream.ReadInt( m_nSpeed );
    rFileStream.ReadString2(m_strHorseName);
    rFileStream.ReadString2( m_strResourceName );
    rFileStream.ReadString2( m_strHorseAniName );
    rFileStream.ReadString2( m_strStandbyAniName );
    rFileStream.ReadString2( m_strMovingAniName );

    return true;
}

//////////////////////////////////////////////////////////////////////////

void CHorseCfgMgr::Initialize()
{
    std::string strFileName( g_sPath );
    strFileName.append( HORSE_FILE_NAME );

    CFileStream file;
    file.open( strFileName.c_str(), "rb" );

    if ( !file.IsOpen() )
    {
        return;
    }

    m_mapHorseCfg.clear();

    unsigned short nCount = 0;
    file.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        CHorseCfg horsecfg;

        if ( horsecfg.LoadInfoFromFile( file ) )
        {
            m_mapHorseCfg.insert( std::make_pair( horsecfg.GetHorseId(), horsecfg ) );
        }
    }

    file.close();
}

int CHorseCfgMgr::GetMaxPassager(int nHorseId)
{
    std::map<int, CHorseCfg>::iterator it = m_mapHorseCfg.find(nHorseId);
    if( it == m_mapHorseCfg.end() )
    {
        return 0;
    }
    return it->second.GetMaxPassager();
}

int CHorseCfgMgr::GetTypeId(int nHorseItemId)
{
    for (std::map<int, CHorseCfg>::iterator it = m_mapHorseCfg.begin();
        it != m_mapHorseCfg.end(); ++it)
    {
        if (it->second.GetHorseItemType() == (unsigned int)nHorseItemId)
        {
            return it->second.GetHorseId();
        }
    }
    return 0;
}

