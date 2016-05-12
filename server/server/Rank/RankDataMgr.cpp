#include "RankDataMgr.h"
#include "RankMgr.h"

#include "../group/Global.h"
#include "../mail/WordMailManager.h"
#include "../dancegroup/DanceGroupProcessor.h"
#include "../libServerFrame/QueryManager.h"

#include "../../socket/Formatter.h"
#include "../../datastructure/Localization.h"

extern CQueryManager* g_pQueryMgr;

//////////////////////////////////////////////////////////////////////////

RankDataMgr::RankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount, bool bActiveOpend )
    : m_nDataType( nDataType )
    , m_nHeaderIndex( nHeaderIndex )
    , m_strTitle( rTitle )
    , m_nMaxShowCount( nMaxCount )
    , m_nCurrentWeekMaxLoadCount( nMaxCount )
    , m_bInitialized( false )
    , m_bActiveOpend( bActiveOpend )
    , m_pCurrentWeekMgr( NULL )
    , m_pLastWeekMgr( NULL )
    , m_pActiveMgr( NULL )
{
}

RankDataMgr::~RankDataMgr()
{
    delete m_pCurrentWeekMgr;
    delete m_pLastWeekMgr;
    delete m_pActiveMgr;
}

void RankDataMgr::Initialize()
{
    m_pCurrentWeekMgr   = new CRankMgr( m_nCurrentWeekMaxLoadCount, m_nCurrentWeekMaxLoadCount );
    m_pLastWeekMgr      = new CRankMgr( m_nMaxShowCount, m_nMaxShowCount );
    if ( m_bActiveOpend )
        m_pActiveMgr    = new CRankMgr( m_nMaxShowCount, m_nMaxShowCount );
}

void RankDataMgr::AddReward( const CRankReward &rReward )
{
    if ( m_setReward.count( rReward ) == 0 )
        m_setReward.insert( rReward );
}

void RankDataMgr::LoadRankInfo( unsigned char nDetailType, const std::list<CRankInfo *> &rInfoList )
{
    CRankMgr *pRankMgr = GetRankMgrByDetailType( nDetailType );
    if ( pRankMgr == NULL )
        return;

    std::set<unsigned int> setRoleID;
    for ( std::list<CRankInfo *>::const_iterator itr = rInfoList.begin(); itr != rInfoList.end(); ++itr )
    {
        if ( *itr == NULL )
            continue;

        AddNewRankInfo( pRankMgr, *itr, setRoleID );
    }

    CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( setRoleID );

    if ( nDetailType == ERankDetailType_CurrentWeek )
        m_bInitialized = true;
}

unsigned int RankDataMgr::GetMaxLoadCount( unsigned char nDetailType ) const
{
    switch ( nDetailType )
    {
    case ERankDetailType_CurrentWeek:
        return m_nCurrentWeekMaxLoadCount;
        break;

    case ERankDetailType_LastWeek:
        return m_nMaxShowCount;
        break;

    case ERankDetailType_Active:
    {
        if ( m_bActiveOpend )
            return m_nMaxShowCount;
        else
            return 0;
    }
        break;

    default:
        return 0;
        break;
    }
}

bool RankDataMgr::GetRoleRank( unsigned char nDetailType, unsigned int nKey, unsigned int *pRoleRank, unsigned int *pTotal ) const
{
    CRankMgr *pRankMgr = GetRankMgrByDetailType( nDetailType );
    if ( pRankMgr == NULL || pRankMgr->TotalCount() == 0 )
        return false;

    if ( pRoleRank != NULL )
    {
        *pRoleRank = pRankMgr->GetRankByKey( nKey );

        if ( *pRoleRank > m_nMaxShowCount )
            *pRoleRank = 0;
    }

    if ( pTotal != NULL )
    {
        *pTotal = pRankMgr->TotalCount();

        if ( *pTotal > m_nMaxShowCount )
            *pTotal = m_nMaxShowCount;
    }

    return true;
}

bool RankDataMgr::GetRankDatas( unsigned char nDetailType, unsigned short nPage, unsigned short nCount, std::list<CRankOfflineInfo> &rOfflineInfos ) const
{
    CRankMgr *pRankMgr = GetRankMgrByDetailType( nDetailType );
    if ( pRankMgr == NULL || pRankMgr->TotalCount() == 0 )
        return false;

    unsigned int nBegin = nPage * nCount + 1;
    unsigned int nEnd = nBegin + nCount - 1;
    std::list<CRankInfo *> listRankInfos;

    if ( nEnd > m_nMaxShowCount )
        nEnd = m_nMaxShowCount;

    pRankMgr->GetRange( nBegin, nEnd, listRankInfos );

    for ( std::list<CRankInfo *>::const_iterator itr = listRankInfos.begin();
        itr != listRankInfos.end(); ++itr )
    {
        const CRankInfo *pRankInfo = *itr;
        if ( pRankInfo == NULL )
        {
            continue;
        }

        CRankOfflineInfo rankOfflineInfo;
        rankOfflineInfo.m_nRank = (unsigned short)nBegin;

        if ( GetRankOfflineData( pRankInfo, rankOfflineInfo ) )
        {
            rOfflineInfos.push_back( rankOfflineInfo );
            ++nBegin;
        }
    }

    return true;
}

bool RankDataMgr::IsInCurrentWeek( unsigned int nKey ) const
{
    if ( nKey != 0 )
        return m_pCurrentWeekMgr->IsInRank( nKey );

    return false;
}

unsigned int RankDataMgr::GetCurrentWeekMinValue() const
{
    if ( m_pCurrentWeekMgr->TotalCount() < m_nMaxShowCount )
        return 0;

    return m_pCurrentWeekMgr->GetMinValue();
}

void RankDataMgr::RemoveRankInfo( unsigned int nKey )
{
    if ( nKey == 0 )
        return;

    m_pCurrentWeekMgr->RemoveRankInfo( nKey );
    m_pLastWeekMgr->RemoveRankInfo( nKey );
    if ( m_bActiveOpend )
        m_pActiveMgr->RemoveRankInfo( nKey );
}

void RankDataMgr::ChangeToNextRange( const CRankMailInfo *pMailInfo )
{
    if ( !m_bInitialized )
        return;

    m_pLastWeekMgr->Reset();

    unsigned int nRank = 1;
    std::list<CRankInfo *> listInfos;
    std::set<CRankReward>::const_iterator rewardItr = m_setReward.begin();

    m_pCurrentWeekMgr->GetRange( nRank, m_nMaxShowCount, listInfos );

    for ( std::list<CRankInfo *>::iterator infoItr = listInfos.begin();
        infoItr != listInfos.end(); ++infoItr )
    {
        CRankInfo *pRankInfo = *infoItr;
        if ( pRankInfo == NULL )
            continue;

        if ( rewardItr != m_setReward.end() && nRank > rewardItr->m_nSecond )
            ++rewardItr;

        if ( rewardItr != m_setReward.end() )
            SendReward( pRankInfo, &(*rewardItr), pMailInfo );

        MoveToLastWeek( pRankInfo );

        ++nRank;
    }

    ResetCurrentWeek();
}

void RankDataMgr::ChangeCurrentWeekValue( const CRankInfo *pRankInfo )
{
    if ( !m_bInitialized || pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return;

    if ( m_pCurrentWeekMgr->IsInRank( pRankInfo->GetKey() ) )
    {
        unsigned int nSrcRank = 0;
        unsigned int nDesRank = 0;
        m_pCurrentWeekMgr->ChangeRankInfo( pRankInfo->GetKey(), (int)pRankInfo->GetValue(), nSrcRank, nDesRank );
    }
    else
    {
        std::set<unsigned int> setRoleID;

        AddNewRankInfo( m_pCurrentWeekMgr, pRankInfo, setRoleID );
        CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( setRoleID );
    }
}

CRankMgr* RankDataMgr::GetRankMgrByDetailType( unsigned char nDetailType ) const
{
    switch ( nDetailType )
    {
    case ERankDetailType_CurrentWeek:
        return m_pCurrentWeekMgr;
        break;

    case ERankDetailType_LastWeek:
        return m_pLastWeekMgr;
        break;

    case ERankDetailType_Active:
        return m_pActiveMgr;
        break;

    default:
        return NULL;
        break;
    }
}

std::string RankDataMgr::GetRoleName( unsigned int nRoleID ) const
{
    if ( nRoleID != 0 )
    {
        CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( nRoleID );
        if ( pRoleEntity != NULL )
        {
            return pRoleEntity->m_szRoleName;
        }

        CGroupOffLineRoleEntity *pOffLineEntity = CGroupOffLinePlayerManager::Instance().GetOfflineRoleImageData( nRoleID );
        if ( pOffLineEntity != NULL )
        {
            RoleImageOfflineData roleImage;
            pOffLineEntity->GetOfflineRoleImageData( roleImage );

            return roleImage.m_strRoleName;
        }
    }

    return "";
}

void RankDataMgr::GetOfflineRoleImage( unsigned int nRoleID, RoleImageOfflineData &rRoleImage ) const
{
    if ( nRoleID == 0 )
        return;

    rRoleImage.m_nRoleID = nRoleID;

    CGroupOffLineRoleEntity *pOffLineEntity = CGroupOffLinePlayerManager::Instance().GetOfflineRoleImageData( nRoleID );
    if ( pOffLineEntity != NULL )
        pOffLineEntity->GetOfflineRoleImageData( rRoleImage );

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByID( nRoleID );
    if ( pRoleEntity != NULL )
    {
        rRoleImage.m_strRoleName = pRoleEntity->m_szRoleName;
        rRoleImage.m_nLevel = pRoleEntity->m_nLevel;
    }
}

void RankDataMgr::SendMail( unsigned int nRoleID, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const
{
    if ( nRoleID == 0 || pReward == NULL || pMailInfo == NULL )
        return;

    if ( pReward->m_nMoney != 0 || pReward->m_nBindMCoin != 0 )
    {
        CWMailManager::Instance().SendMailToPlayer( nRoleID, EMailType_Rank, pMailInfo->m_strSender,
            pMailInfo->m_strTitle, pMailInfo->m_strContent, pReward->m_nMoney, pReward->m_nBindMCoin );
    }

    if ( !pReward->m_listItems.empty() )
    {
        CWMailManager::Instance().SendMailToPlayer( nRoleID, EMailType_Rank, pMailInfo->m_strSender,
            pMailInfo->m_strTitle, pMailInfo->m_strContent, pReward->m_listItems );
    }
}

bool RankDataMgr::GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 )
{
    rInfo1.clear();
    rInfo2.clear();

    if ( m_bInitialized && m_pCurrentWeekMgr != NULL &&
        m_pCurrentWeekMgr->Front() != NULL )
    {
        return true;
    }

    return false;
}

bool RankDataMgr::MoveToLastWeek( const CRankInfo *pRankInfo )
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    std::set<unsigned int> setRoleID;

    bool bRes = AddNewRankInfo( m_pLastWeekMgr, pRankInfo, setRoleID );
    if ( bRes )
        CGroupOffLinePlayerManager::Instance().LoadOfflineRoleImageData( setRoleID );

    return bRes;
}

void RankDataMgr::ResetCurrentWeek()
{
    m_pCurrentWeekMgr->Reset();
}

//////////////////////////////////////////////////////////////////////////

#define RANK_MAX_LOAD_COUNT 500

RoleRankDataMgr::RoleRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
    : RankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount )
{
}

RoleRankDataMgr::~RoleRankDataMgr()
{
}

bool RoleRankDataMgr::GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 )
{
    if ( !RankDataMgr::GetTopOneInfo( rInfo1, rInfo2 ) )
        return false;

    const CRankInfo *pInfo = m_pCurrentWeekMgr->Front();
    rInfo1 = GetRoleName( pInfo->GetKey() );

    return true;
}

bool RoleRankDataMgr::AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID )
{
    if ( pRankMgr == NULL || pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    CRankInfo *pNewInfo = new CRankInfo( *pRankInfo );
    if ( pRankMgr->AddNewRankInfo( pNewInfo ) > 0 )
    {
        rRoleID.insert( pNewInfo->GetKey() );

        return true;
    }

    delete pNewInfo;

    return false;
}

bool RoleRankDataMgr::GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    RoleImageOfflineData roleImage;
    GetOfflineRoleImage( pRankInfo->GetKey(), roleImage );

    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << roleImage.m_nLevel );
    rOfflineInfo.m_listOtherInfo.push_back( roleImage.m_strRoleName );
    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << pRankInfo->GetValue() );
    rOfflineInfo.m_listOfflineData.push_back( roleImage );

    return true;
}

void RoleRankDataMgr::SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const
{
    if ( pRankInfo == NULL || pReward == NULL || pMailInfo == NULL )
        return;

    SendMail( pRankInfo->GetKey(), pReward, pMailInfo );
}

//////////////////////////////////////////////////////////////////////////

GroupRankDataMgr::GroupRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
    : RankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount )
{
    m_nCurrentWeekMaxLoadCount = RANK_MAX_LOAD_COUNT;
}

GroupRankDataMgr::~GroupRankDataMgr()
{
}

bool GroupRankDataMgr::GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 )
{
    if ( !RankDataMgr::GetTopOneInfo( rInfo1, rInfo2 ) )
        return false;

    const CRankInfo *pInfo = m_pCurrentWeekMgr->Front();
    const CDanceGroupInfo *pGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( pInfo->GetKey() );
    if ( pGroupInfo != NULL )
    {
        rInfo1 = pGroupInfo->m_DGBaseInfo.m_strGroupName;
        rInfo2 = CLocalization::Instance().GetString( "RankList_Dance_Group_ID" ) + " ";
        rInfo2 += (Formatter() << pInfo->GetKey());
    }

    return true;
}

bool GroupRankDataMgr::AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID )
{
    if ( pRankMgr == NULL || pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    const CDanceGroupInfo *pGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( pRankInfo->GetKey() );
    if ( pGroupInfo == NULL )
        return false;

    CRankInfo *pNewInfo = new CRankInfo( *pRankInfo );
    if ( pRankMgr->AddNewRankInfo( pNewInfo ) > 0 )
    {
        rRoleID.insert( pGroupInfo->m_DGBaseInfo.m_nLeaderRoleID );

        return true;
    }

    delete pNewInfo;

    return false;
}

bool GroupRankDataMgr::GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    const CDanceGroupInfo *pGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( pRankInfo->GetKey() );
    if ( pGroupInfo == NULL )
        return false;

    RoleImageOfflineData roleImage;
    GetOfflineRoleImage( pGroupInfo->m_DGBaseInfo.m_nLeaderRoleID, roleImage );

    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << pRankInfo->GetKey() );
    rOfflineInfo.m_listOtherInfo.push_back( pGroupInfo->m_DGBaseInfo.m_strGroupName );
    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << pRankInfo->GetValue() );
    rOfflineInfo.m_listOfflineData.push_back( roleImage );

    return true;
}

void GroupRankDataMgr::SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const
{
    if ( pRankInfo == NULL || pReward == NULL || pMailInfo == NULL )
        return;

    const CDanceGroupInfo *pGroupInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( pRankInfo->GetKey() );
    if ( pGroupInfo == NULL )
        return;

    const std::map<unsigned int, CDanceGroupMember *> &rAllMember = pGroupInfo->GetAllMember();
    for ( std::map<unsigned int, CDanceGroupMember *>::const_iterator itr = rAllMember.begin();
        itr != rAllMember.end(); ++itr )
    {
        CDanceGroupMember *pMember = itr->second;
        if ( pMember == NULL )
            continue;

        SendMail( pMember->m_nRoleID, pReward, pMailInfo );
    }
}

//////////////////////////////////////////////////////////////////////////

CoupleRankDataMgr::CoupleRankDataMgr( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
    : RankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount )
{
    m_nCurrentWeekMaxLoadCount = RANK_MAX_LOAD_COUNT;
}

CoupleRankDataMgr::~CoupleRankDataMgr()
{
}

bool CoupleRankDataMgr::GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 )
{
    if ( !RankDataMgr::GetTopOneInfo( rInfo1, rInfo2 ) )
        return false;

    const CRankCoupleInfo *pCoupleInfo = dynamic_cast<const CRankCoupleInfo *>(m_pCurrentWeekMgr->Front());
    if ( pCoupleInfo != NULL )
    {
        rInfo1 = GetRoleName( pCoupleInfo->m_RankValue.m_nMaleRoleID );
        rInfo2 = GetRoleName( pCoupleInfo->m_RankValue.m_nFemaleRoleID );
    }

    return true;
}

bool CoupleRankDataMgr::AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID )
{
    if ( pRankMgr == NULL || pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    const CRankCoupleInfo *pCoupleInfo = dynamic_cast<const CRankCoupleInfo *>(pRankInfo);
    if ( pCoupleInfo == NULL )
        return false;

    CRankInfo *pNewInfo = new CRankCoupleInfo( pCoupleInfo->m_RankValue );
    if ( pRankMgr->AddNewRankInfo( pNewInfo ) > 0 )
    {
        rRoleID.insert( pCoupleInfo->m_RankValue.m_nMaleRoleID );
        rRoleID.insert( pCoupleInfo->m_RankValue.m_nFemaleRoleID );

        return true;
    }

    delete pNewInfo;

    return false;
}

bool CoupleRankDataMgr::GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    const CRankCoupleInfo *pCoupleInfo = dynamic_cast<const CRankCoupleInfo *>(pRankInfo);
    if ( pCoupleInfo == NULL )
        return false;

    const CRankCoupleValue &rCoupleValue = pCoupleInfo->m_RankValue;
    rOfflineInfo.m_nSmallRoleID = rCoupleValue.m_nSamllRoleID;

    RoleImageOfflineData maleImage;
    GetOfflineRoleImage( rCoupleValue.m_nMaleRoleID, maleImage );

    rOfflineInfo.m_listOtherInfo.push_back( maleImage.m_strRoleName );
    rOfflineInfo.m_listOfflineData.push_back( maleImage );

    RoleImageOfflineData femaleImage;
    GetOfflineRoleImage( rCoupleValue.m_nFemaleRoleID, femaleImage );

    rOfflineInfo.m_listOtherInfo.push_back( femaleImage.m_strRoleName );
    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << pRankInfo->GetValue() );
    rOfflineInfo.m_listOfflineData.push_back( femaleImage );

    return true;
}

void CoupleRankDataMgr::SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const
{
    if ( pRankInfo == NULL || pReward == NULL || pMailInfo == NULL )
        return;

    const CRankCoupleInfo *pCoupleInfo = dynamic_cast<const CRankCoupleInfo *>(pRankInfo);
    if ( pCoupleInfo == NULL )
        return;

    SendMail( pCoupleInfo->m_RankValue.m_nMaleRoleID, pReward, pMailInfo );
    SendMail( pCoupleInfo->m_RankValue.m_nFemaleRoleID, pReward, pMailInfo );
}

//////////////////////////////////////////////////////////////////////////

RoleCumulativeRankDataMgr::RoleCumulativeRankDataMgr( unsigned short nDataType,
    unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
    : RankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount, false )
{
    m_nCurrentWeekMaxLoadCount = nMaxCount + RANK_MAX_LOAD_COUNT;
}

RoleCumulativeRankDataMgr::~RoleCumulativeRankDataMgr()
{
}

void RoleCumulativeRankDataMgr::RemoveCurrentRoleByOfflineTime( unsigned int nMaxOfflineTime )
{
    if ( m_pCurrentWeekMgr->TotalCount() == 0 )
        return;

    unsigned int nCheckTime = (unsigned int)time( NULL );
    if ( nCheckTime <= nMaxOfflineTime )
        return;

    nCheckTime -= nMaxOfflineTime;

    std::list<CRankInfo *> listInfos;
    m_pCurrentWeekMgr->GetRange( 1, m_pCurrentWeekMgr->TotalCount(), listInfos );

    for ( std::list<CRankInfo *>::iterator itr = listInfos.begin();
        itr != listInfos.end(); ++itr )
    {
        const CRankRoleCumulativeInfo *pCumulativeInfo = dynamic_cast<const CRankRoleCumulativeInfo *>(*itr);
        if ( pCumulativeInfo == NULL )
            continue;
        else if ( pCumulativeInfo->m_rankValue.m_nOfflineTime >= nCheckTime )
            continue;

        m_pCurrentWeekMgr->RemoveRankInfo( pCumulativeInfo->GetKey() );
        delete *itr, *itr = NULL;
    }
}

bool RoleCumulativeRankDataMgr::GetTopOneInfo( std::string &rInfo1, std::string &rInfo2 )
{
    if ( !RankDataMgr::GetTopOneInfo( rInfo1, rInfo2 ) )
        return false;

    const CRankInfo *pInfo = m_pCurrentWeekMgr->Front();
    rInfo1 = GetRoleName( pInfo->GetKey() );

    return true;
}

bool RoleCumulativeRankDataMgr::AddNewRankInfo( CRankMgr *pRankMgr, const CRankInfo *pRankInfo, std::set<unsigned int> &rRoleID )
{
    if ( pRankMgr == NULL || pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    const CRankRoleCumulativeInfo *pCumulativeInfo = dynamic_cast<const CRankRoleCumulativeInfo *>(pRankInfo);
    if ( pCumulativeInfo == NULL )
        return false;

    CRankInfo *pNewInfo = new CRankRoleCumulativeInfo( pCumulativeInfo->m_rankValue );
    if ( pRankMgr->AddNewRankInfo( pNewInfo ) > 0 )
    {
        rRoleID.insert( pCumulativeInfo->GetKey() );

        return true;
    }

    delete pNewInfo;

    return false;
}

bool RoleCumulativeRankDataMgr::GetRankOfflineData( const CRankInfo *pRankInfo, CRankOfflineInfo &rOfflineInfo ) const
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    RoleImageOfflineData roleImage;
    GetOfflineRoleImage( pRankInfo->GetKey(), roleImage );

    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << roleImage.m_nLevel );
    rOfflineInfo.m_listOtherInfo.push_back( roleImage.m_strRoleName );
    rOfflineInfo.m_listOtherInfo.push_back( Formatter() << pRankInfo->GetValue() );
    rOfflineInfo.m_listOfflineData.push_back( roleImage );

    return true;
}

void RoleCumulativeRankDataMgr::SendReward( const CRankInfo *pRankInfo, const CRankReward *pReward, const CRankMailInfo *pMailInfo ) const
{
    if ( pRankInfo == NULL || pReward == NULL || pMailInfo == NULL )
        return;

    SendMail( pRankInfo->GetKey(), pReward, pMailInfo );
}

bool RoleCumulativeRankDataMgr::MoveToLastWeek( const CRankInfo *pRankInfo )
{
    if ( pRankInfo == NULL || pRankInfo->GetKey() == 0 )
        return false;

    bool bRes = RankDataMgr::MoveToLastWeek( pRankInfo );
    if ( bRes )
        m_mapLastWeekHistory.insert( std::make_pair( pRankInfo->GetKey(), pRankInfo->GetValue() ) );

    return bRes;
}

void RoleCumulativeRankDataMgr::ResetCurrentWeek()
{
    if ( !m_mapLastWeekHistory.empty() )
    {
        RankSaveHistoryDB *pSaveHistoryDB = RankSaveHistoryDBFactory::Create( m_nDataType );
        if ( pSaveHistoryDB != NULL )
        {
            pSaveHistoryDB->m_mapHistoryDatas = m_mapLastWeekHistory;

            g_pQueryMgr->AddQuery( QUERY_Rank_SaveHistory, 0, pSaveHistoryDB );
        }
    }

    m_mapLastWeekHistory.clear();
}

//////////////////////////////////////////////////////////////////////////

RankDataMgr* RankDataFactory::CreateRankData( unsigned short nDataType, unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
    case ERankDataType_Horn:
    case ERankDataType_Exp:
        return new RoleRankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount );
        break;

    case ERankDataType_Group_Honor:
        return new GroupRankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount );
        break;

    case ERankDataType_Intimacy:
        return new CoupleRankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount );
        break;

    case ERankDataType_Cloths:
    case ERankDataType_ClothEffect:
    case ERankDataType_Medal:
        return new RoleCumulativeRankDataMgr( nDataType, nHeaderIndex, rTitle, nMaxCount );
        break;

    default:
        return NULL;
        break;
    }
}

