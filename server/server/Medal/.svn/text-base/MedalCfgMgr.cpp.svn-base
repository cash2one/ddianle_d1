#include "MedalCfgMgr.h"

#include "../../socket/Log.h"

#include "../../datastructure/datainfor/FileStream.h"

extern std::string g_sPath;

#define MEDAL_FILE_NAME     "data/bytes/Medal.bytes"

CMedalCfgMgr::CMedalCfgMgr():m_nType(0),m_nMedalMoneyID(0)
{
}

CMedalCfgMgr::~CMedalCfgMgr()
{
}

int CMedalCfgMgr::GetVersionMedalType() const
{
    return m_nType;
}


void CMedalCfgMgr::SetVersionMedalType(int type)
{
    m_nType = type;
}

void CMedalCfgMgr::GetMedalScoreDescribe(std::string &strDescribe) const
{
    strDescribe = m_strMedalScoreDescribe;
}

void CMedalCfgMgr::SetMedalScoreDescribe(const std::string &strDescribe)
{
    m_strMedalScoreDescribe = strDescribe;
}


void CMedalCfgMgr::Initialize()
{
    LoadConfigFromFile();
}

int CMedalCfgMgr::GetMedalMoneyID() const
{
    return m_nMedalMoneyID;
}

void CMedalCfgMgr::SetMedalMoneyID(int medalmoneyid)
{
    m_nMedalMoneyID = medalmoneyid;
}

void CMedalCfgMgr::GetAllMedalIDList( std::list<int> &rListID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr;

    for ( itr = m_mapMedalCfg.begin(); itr != m_mapMedalCfg.end(); ++itr )
    {
        rListID.push_back( itr->first );
    }
}

bool CMedalCfgMgr::IsConfigContainMedal(int nMedalId) const
{
    if (m_mapMedalCfg.find(nMedalId) == m_mapMedalCfg.end() )
    {
        return false;
    }
    return true;
}

bool CMedalCfgMgr::IsMedalForevery( int nMedalID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        return itr->second.HasForever();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalID );

    return false;
}

int CMedalCfgMgr::GetMedalDuration( int nMedalID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        return itr->second.GetDuration();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalID );

    return 0;
}

int CMedalCfgMgr::GetMedalType( int nMedalID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        return itr->second.GetType();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalID );

    return 0;
}

int CMedalCfgMgr::GetNeedMedalMoney( int nMedalID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        return itr->second.GetNeedMedalMoney();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalID );

    return 0;
}

int CMedalCfgMgr::GetMedalSortPriority( int nMedalID ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        return itr->second.GetSortKey();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalID );

    return 0;
}

int CMedalCfgMgr::GetMedalScore(int nMedalId) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find(nMedalId);

    if (itr != m_mapMedalCfg.end())
        return itr->second.GetMedalScore();

    WriteLog(LOGLEVEL_ERROR, "Can not find Medal config info by id=%d", nMedalId);

    return 0;
}

void CMedalCfgMgr::GetMedalName( int nMedalID, std::string &rName ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        rName = itr->second.GetName();
}

void CMedalCfgMgr::GetMedalTypeDescribe( int nMedalID, std::string &rTypeDescribe ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        rTypeDescribe = itr->second.GetTypeDescribe();
}

void CMedalCfgMgr::GetMedalDescribe( int nMedalID, std::string &rDescribe ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        rDescribe = itr->second.GetDescribe();
}

void CMedalCfgMgr::GetMedalIcon( int nMedalID, std::string &rIcon ) const
{
    std::map<int, CMedalCfg>::const_iterator itr = m_mapMedalCfg.find( nMedalID );

    if ( itr != m_mapMedalCfg.end() )
        rIcon = itr->second.GetIcon();
}

void CMedalCfgMgr::GetAllProgressIDList( std::list<int> &rListID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr;

    for ( itr = m_mapMedalProgressCfg.begin(); itr != m_mapMedalProgressCfg.end(); ++itr )
    {
        rListID.push_back( itr->first );
    }
}

int CMedalCfgMgr::GetTotalProgress( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetTotalProgress();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

int CMedalCfgMgr::GetProgressType( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetType();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

int CMedalCfgMgr::GetRewardMedal( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetMedalID();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

int CMedalCfgMgr::GetProgressSortPriority( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetSortKey();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

int CMedalCfgMgr::GetVisibilityIndex(int nProgressID) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetVisibility();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}
unsigned int CMedalCfgMgr::GetAllowGetStartTime( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetAllowGetStartTime();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

unsigned int CMedalCfgMgr::GetAllowGetEndTime( int nProgressID ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        return itr->second.GetAllowGetEndTime();

    WriteLog( LOGLEVEL_ERROR, "Can not find Medal progress config info by id=%d", nProgressID );

    return 0;
}

void CMedalCfgMgr::GetProgressDescribe( int nProgressID, std::string &rDescribe ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        rDescribe = itr->second.GetDescribe();
}

void CMedalCfgMgr::GetSpecialParams( int nProgressID, std::list<unsigned int> &rParams ) const
{
    std::map<int, CMedalProgressCfg>::const_iterator itr = m_mapMedalProgressCfg.find( nProgressID );

    if ( itr != m_mapMedalProgressCfg.end() )
        rParams = itr->second.GetSpecialParams();
}

void CMedalCfgMgr::LoadConfigFromFile()
{
    std::string strFileName( g_sPath );
    strFileName.append( MEDAL_FILE_NAME );

    CFileStream file;
    file.open( strFileName.c_str(), "rb" );

    LoadMedalConfigFromFile( file );
    LoadProgressConfigFromFile( file );

    file.close();
}

void CMedalCfgMgr::LoadMedalConfigFromFile( CFileStream &rFileStream )
{
    if ( !rFileStream.IsOpen() )
        return;

    m_mapMedalCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        CMedalCfg medalInfo;

        if ( medalInfo.LoadInfoFromFile( rFileStream ) )
        {
            m_mapMedalCfg.insert( std::make_pair( medalInfo.GetId(), medalInfo ) );
        }
    }
}

void CMedalCfgMgr::LoadProgressConfigFromFile( CFileStream &rFileStream )
{
    if ( !rFileStream.IsOpen() )
        return;

    m_mapMedalProgressCfg.clear();

    unsigned short nCount = 0;
    rFileStream.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        CMedalProgressCfg processInfo;

        if ( processInfo.LoadInfoFromFile( rFileStream ) )
        {
            m_mapMedalProgressCfg.insert( std::make_pair( processInfo.GetId(), processInfo ) );
        }
    }
}

