#include "DanceGroupChallengeManager.h"

#include "../util/CommonFunc.h"
#include "../../socket/Windefine.h"
#include "../../datastructure/datainfor/FileStream.h"
#include "../../datastructure/datainfor/ConfigManager.h"

extern std::string g_sPath;

#define CHALLENGE_FILE_NAME "data/bytes/DanceGroupChallenge.bytes"


DanceGroupChallengeStoreyInfo::DanceGroupChallengeStoreyInfo()
    : m_nStorey( 0 )
    , m_nSceneID( 0 )
    , m_nMusicID( 0 )
    , m_nMusicMode( 0 )
    , m_nMusicLevel( 0 )
    , m_nTargetType( EDanceGroupChallengeTargetType_Score )
    , m_nTargetScore( 0 )
    , m_nBossID( 0 )
    , m_nActiveValue( 0 )
{
}

DanceGroupChallengeStoreyInfo::~DanceGroupChallengeStoreyInfo()
{
}

void DanceGroupChallengeStoreyInfo::LoadConfigFromFile( CFileStream& file )
{
    if ( !file.IsOpen() )
        return;

    file.ReadInt( m_nStorey );
    file.ReadString2(m_strAtlas);
    file.ReadString2( m_strIcon );
    file.ReadInt( m_nSceneID );
    file.ReadInt( m_nMusicID );
    file.ReadInt( m_nMusicMode );
    file.ReadInt( m_nMusicLevel );
    file.ReadUChar( m_nTargetType );
    file.ReadUInt( m_nTargetScore );
    file.ReadInt( m_nBossID );
    file.ReadUInt( m_nActiveValue );

    ASSERT( m_nStorey >= MIN_CHALLENGE_STOREY );
    ASSERT( m_nSceneID >= 0 );
    ASSERT( m_nMusicID >= 0 );
    ASSERT( m_nMusicMode >= 0 );
    ASSERT( m_nMusicLevel >= 0 );
    ASSERT( m_nTargetType < EDanceGroupChallengeTargetType_Max );
    ASSERT( m_nTargetScore > 0 );
    ASSERT( m_nActiveValue > 0 );
}

//////////////////////////////////////////////////////////////////////////

DanceGroupChallengeReward::DanceGroupChallengeReward()
    : m_nStorey( 0 )
{
}

DanceGroupChallengeReward::~DanceGroupChallengeReward()
{
}

void DanceGroupChallengeReward::LoadConfigFromFile( CFileStream &file )
{
    if ( !file.IsOpen() )
        return;

    file.ReadInt( m_nStorey );
    ReadItemInfos( file, m_vectMaleItem, true );
    ReadItemInfos( file, m_vectMaleRandItem, true );
    ReadItemInfos( file, m_vectFemaleItem, false );
    ReadItemInfos( file, m_vectFemaleRandItem, false );
}

void DanceGroupChallengeReward::GetAllRewardInfos( bool bMale, std::list<CItem> &rList ) const
{
    if ( bMale )
    {
        rList.insert( rList.end(), m_vectMaleItem.begin(), m_vectMaleItem.end() );
        rList.insert( rList.end(), m_vectMaleRandItem.begin(), m_vectMaleRandItem.end() );
    }
    else
    {
        rList.insert( rList.end(), m_vectFemaleItem.begin(), m_vectFemaleItem.end() );
        rList.insert( rList.end(), m_vectFemaleRandItem.begin(), m_vectFemaleRandItem.end() );
    }
}

void DanceGroupChallengeReward::GetRewardInfos( bool bMale, unsigned char nRandCount, std::list<CItem> &rList ) const
{
    const std::vector<CItem> *pItem = &m_vectMaleItem;
    const std::vector<CItem> *pRandItem = &m_vectMaleRandItem;

    if ( !bMale )
    {
        pItem = &m_vectFemaleItem;
        pRandItem = &m_vectFemaleRandItem;
    }

    if ( pItem != NULL && !pItem->empty() )
        rList.insert( rList.end(), pItem->begin(), pItem->end() );

    if ( pRandItem != NULL && !pRandItem->empty() )
    {
        unsigned int size = pRandItem->size();
        unsigned int nIndex = 0;
        int nEnd = (int)size - 1;

        while ( nRandCount > 0 )
        {
            nIndex = (unsigned int)Random( 0, nEnd );//[0, size - 1]

            if ( nIndex < size )
            {
                rList.push_back( pRandItem->at( nIndex ) );
            }

            --nRandCount;
        }
    }
}

void DanceGroupChallengeReward::ReadItemInfos( CFileStream &file, std::vector<CItem> &rInfo, bool bMale ) const
{
    CItem item;
    unsigned short nCount = 0;
    const ItemConfig* pItemInfo = NULL;
    const ItemConfigManager& mgr = ConfigManager::Instance().GetItemConfigManager();

    file.ReadUShort( nCount );
    for ( unsigned short i = 0; i < nCount; ++i )
    {
        file.ReadUInt( item.m_nItemType );
        file.ReadUShort( item.m_nItemCount );
        file.ReadInt( item.m_nValidTime );

        pItemInfo = mgr.GetByID( item.m_nItemType );

        if ( pItemInfo == NULL || item.m_nItemCount == 0 )
            continue;
        else if ( bMale && pItemInfo->m_SexNeed == ESexType_Female )
            continue;
        else if ( !bMale && pItemInfo->m_SexNeed == ESexType_Male )
            continue;

        rInfo.push_back( item );
    }
}

//////////////////////////////////////////////////////////////////////////

DanceGroupChallengeManager::DanceGroupChallengeManager(void)
    : m_nFreeChance( 0 )
    , m_nInspireMaxCount( 0 )
    , m_nResetWeekday( 0 )
    , m_nResetHour( 0 )
    , m_nLockSeconds( 5 * 60 )
    , m_nAnnouncementSeconds( 5 * 60 )
{
}

DanceGroupChallengeManager::~DanceGroupChallengeManager(void)
{
}

bool DanceGroupChallengeManager::Initialize()
{
    LoadConfigFromFile();

    return CheckConfig();
}

int DanceGroupChallengeManager::GetTotalStorey() const
{
    if ( m_mapStoreyInfos.empty() )
        return 0;

    return m_mapStoreyInfos.rbegin()->first;
}

bool DanceGroupChallengeManager::IsOpenNow() const
{
    unsigned int nNow = (unsigned int)time( NULL );
    unsigned int nStartTime = PrevTimeStamp( nNow, m_nResetWeekday, m_nResetHour );
    unsigned int nNextStartTime = GetNextStartTime( nStartTime );

    if ( ( nStartTime + m_nLockSeconds ) < nNow && 
        ( nNow + m_nLockSeconds ) < nNextStartTime )
    {
        return true;
    }

    return false;
}

unsigned int DanceGroupChallengeManager::GetStartTime() const
{
    return PrevTimeStamp( time( NULL ), m_nResetWeekday, m_nResetHour );
}

unsigned int DanceGroupChallengeManager::GetNextStartTime( unsigned int nStartTime ) const
{
    return ( nStartTime + 7 * DAY_PRE_SECONDS );
}

unsigned int DanceGroupChallengeManager::GetOpenTime( unsigned int nStartTime ) const
{
    return ( nStartTime + m_nLockSeconds );
}

unsigned int DanceGroupChallengeManager::GetCloseTime( unsigned int nNextStartTime ) const
{
    unsigned int nSeconds = m_nLockSeconds;

    if ( nNextStartTime <= nSeconds )
        return 0;

    return ( nNextStartTime - nSeconds );
}

unsigned int DanceGroupChallengeManager::GetAnnouncementTime( unsigned int nNextStartTime ) const
{
    unsigned int nSeconds = m_nLockSeconds + m_nAnnouncementSeconds;

    if ( nNextStartTime <= nSeconds )
        return 0;

    return ( nNextStartTime - nSeconds );
}

const std::map<int, DanceGroupChallengeStoreyInfo>& DanceGroupChallengeManager::GetStoreyInfo() const
{
    return m_mapStoreyInfos;
}

const DanceGroupChallengeStoreyInfo* DanceGroupChallengeManager::GetStoreyInfo( int nStorey ) const
{
    std::map<int, DanceGroupChallengeStoreyInfo>::const_iterator itr = m_mapStoreyInfos.find( nStorey );
    if ( itr != m_mapStoreyInfos.end() )
        return &( itr->second );

    return NULL;
}

const CBossConfig* DanceGroupChallengeManager::GetBossInfo( int nStorey ) const
{
    std::map<int, CBossConfig>::const_iterator itr = m_mapBossInfos.find( nStorey );
    if ( itr != m_mapBossInfos.end() )
        return &( itr->second );

    return NULL;
}

void DanceGroupChallengeManager::GetAllRewardInfos( int nStorey, bool bMale, std::list<CItem> &rList ) const
{
    std::map<int, DanceGroupChallengeReward>::const_iterator itr = m_mapRewardInfos.find( nStorey );
    if ( itr != m_mapRewardInfos.end() )
        itr->second.GetAllRewardInfos( bMale, rList );
}

void DanceGroupChallengeManager::GetRewardInfos( int nStorey, short nRanking, bool bMale, std::list<CItem> &rList ) const
{
    std::map<int, DanceGroupChallengeReward>::const_iterator itr = m_mapRewardInfos.find( nStorey );
    if ( itr == m_mapRewardInfos.end() )
        return;

    unsigned char nCount = 0;
    std::map<short, unsigned char>::const_iterator randItr = m_mapRandItemCount.lower_bound( nRanking );
    if ( randItr != m_mapRandItemCount.end() )
        nCount = randItr->second;

    itr->second.GetRewardInfos( bMale, nCount, rList );
}

void DanceGroupChallengeManager::GetNextBuyChanceInfo( unsigned short nCurrentBuyCount, Currency &rCost ) const
{
    if ( m_mapBuyChanceInfos.empty() )
        return;

    ++nCurrentBuyCount;

    std::map<unsigned short, Currency>::const_iterator itr = m_mapBuyChanceInfos.find( nCurrentBuyCount );
    if ( itr != m_mapBuyChanceInfos.end() )
        rCost = itr->second;
    else
        rCost = m_mapBuyChanceInfos.rbegin()->second;
}

void DanceGroupChallengeManager::SetRandItemCount( short ranking, unsigned char nCount )
{
    if ( ranking > 0 && nCount != 0 )
        m_mapRandItemCount.insert( std::make_pair( ranking, nCount ) );
}

void DanceGroupChallengeManager::SetInspireInfos( Currency &rCost, unsigned short nMaxCount )
{
    m_inspireCost = rCost;
    m_nInspireMaxCount = nMaxCount;
}

void DanceGroupChallengeManager::SetResetInfos( int nWeekday, int nHour, unsigned int nLockTime, unsigned int nAnnouncementTime )
{
    m_nResetWeekday = nWeekday;
    m_nResetHour = nHour;

    if ( m_nLockSeconds < nLockTime )
        m_nLockSeconds = nLockTime;
    if ( m_nAnnouncementSeconds < nAnnouncementTime )
        m_nAnnouncementSeconds = nAnnouncementTime;
}

void DanceGroupChallengeManager::SetMailInfo( std::string &rMailSender, std::string &rMailTitle, std::string &rMailContent )
{
    m_strMailSender.swap( rMailSender );
    m_strMailTitle.swap( rMailTitle );
    m_strMailContent.swap( rMailContent );
}

void DanceGroupChallengeManager::LoadConfigFromFile()
{
    CFileStream file;
    std::string strFileName( g_sPath + CHALLENGE_FILE_NAME );

    if ( file.open( strFileName.c_str(), "rb" ) )
    {
        LoadLevelInfos( file );
        LoadRewardInfos( file );
        LoadBuyChanceInfos( file );
        LoadBossInfos( file );
    }

    file.close();
}

void DanceGroupChallengeManager::LoadLevelInfos( CFileStream &file )
{
    unsigned short nCount = 0;
    file.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        DanceGroupChallengeStoreyInfo level;
        level.LoadConfigFromFile( file );

        m_mapStoreyInfos.insert( std::make_pair( level.GetStorey(), level ) );
    }
}

void DanceGroupChallengeManager::LoadRewardInfos( CFileStream &file )
{
    unsigned short nCount = 0;
    file.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        DanceGroupChallengeReward reward;
        reward.LoadConfigFromFile( file );

        m_mapRewardInfos.insert( std::make_pair( reward.GetStorey(), reward ) );
    }
}

void DanceGroupChallengeManager::LoadBuyChanceInfos( CFileStream &file )
{
    unsigned short nCount = 0;
    file.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        unsigned short nBuyCount = 0;
        unsigned char nCurrencyType = 0;
        unsigned int nCurrencyAmount = 0;
        Currency currency;

        file.ReadUShort( nBuyCount );
        file.ReadUChar( nCurrencyType );
        file.ReadUInt( nCurrencyAmount );

        currency.SetType( nCurrencyType );
        currency.SetAmount( nCurrencyAmount );

        ASSERT( currency.IsValid() );

        if ( nBuyCount > 0 )
        {
            m_mapBuyChanceInfos.insert( std::make_pair( nBuyCount, currency ) );
        }
    }
}

void DanceGroupChallengeManager::LoadBossInfos( CFileStream &file )
{
    unsigned short nCount = 0;
    file.ReadUShort( nCount );

    for ( unsigned short i = 0; i < nCount; ++i )
    {
        CBossConfig boss;
        boss.Load( file );

        m_mapBossInfos.insert( std::make_pair( boss.m_nBossID, boss ) );
    }
}

bool DanceGroupChallengeManager::CheckConfig() const
{
    if ( m_mapStoreyInfos.empty() )
        return false;
    else if ( m_mapStoreyInfos.rbegin()->first != (int)m_mapStoreyInfos.size() )
        return false;
    else if ( m_mapBuyChanceInfos.empty() )
        return true;
    else if ( m_mapBuyChanceInfos.rbegin()->first != (unsigned short)m_mapBuyChanceInfos.size() )
        return false;
    else if ( !( 0 <= m_nResetWeekday && m_nResetWeekday <= 6 ) )
        return false;
    else if ( !( 0 <= m_nResetHour && m_nResetHour <= 23 ) )
        return false;

    return true;
}

