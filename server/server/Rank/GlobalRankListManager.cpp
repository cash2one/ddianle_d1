#include "GlobalRankListManager.h"
#include "RankDataMgr.h"

#include "../group/Global.h"
#include "../group/GroupSystemSetting.h"

#include "../util/CommonFunc.h"
#include "../share/ServerMsgDef.h"
#include "../libServerFrame/QueryManager.h"
#include "GameMsg_Rank_SG.h"

#include "../../socket/Log.h"
#include "../../socket/GameMsg_Map.h"

typedef std::map<unsigned short, SharedPtr<RankDataMgr> >      Map_RankData;

#define DAY_SECONDES        (24 * 3600)
#define ACTIVE_DAYS         90

#define SAVE_GROUP_INTERVAL         (60 * 10)   // seconds
#define SAVE_INTIMACY_INTERVAL      (60 * 10)   // seconds
#define SAVE_GROUP_MAX_COUNT        2000
#define SAVE_INTIMACY_MAX_COUNT     2000

extern CQueryManager* g_pQueryMgr;


CGlobalRankListManager::CGlobalRankListManager(void)
    : GameMsg_Processor(true, true)
    , m_nStartWeekDay(0)
    , m_nStartHour(0)
    , m_nMaxOfflineTime(0)
    , m_nLastHour(-1)
    , m_nLastSaveGroupTime(0)
    , m_nLastSaveIntimacyTime(0)
    , m_bOpening(false)
    , m_bCallUpdate(true)
    , m_bSetClosed(false)
    , m_bShowGay(false)
{
    m_pMailInfo = new CRankMailInfo;
}

CGlobalRankListManager::~CGlobalRankListManager(void)
{
    SAFE_DELETE( m_pMailInfo );
}

CGlobalRankListManager& CGlobalRankListManager::Instance()
{
    static CGlobalRankListManager s_RankingListProcessor;

    return s_RankingListProcessor;
}

void CGlobalRankListManager::Initialize()
{
    m_bOpening = CGroupSystemSetting::Instance().IsFunctionOpen((unsigned short)EFunctionSwitch_Rank);
    m_bShowGay = CGroupSystemSetting::Instance().IsFunctionOpen((unsigned short)EFunctionSwitch_RankShowGay);

    if (!m_bOpening)
        return;

    RegisterMessage();

    unsigned int nNow = (unsigned int)time(NULL) + 1;
    unsigned int nCurrentStartTime = CalcCurrentStartTime();
    unsigned int nLastStartTime = CalcLastStartTime(nCurrentStartTime);
    unsigned int nActiveStartTime = 0;
    unsigned int nActiveEndTime = 0;
    SharedPtr<RankDataMgr> sharedMgr;

    CalcActiveTime(nActiveStartTime, nActiveEndTime);

    for ( Map_RankData::const_iterator indexItr = m_mapIndex2Data.begin(); indexItr != m_mapIndex2Data.end(); ++indexItr )
    {
        sharedMgr = indexItr->second;

        if ( !sharedMgr.IsValid() )
        {
            continue;
        }

        unsigned short nType = sharedMgr->GetDataType();
        unsigned int nCurrentLoadTime = CalcCurrentLoadTime( nType, nCurrentStartTime );

        QueryRankDatas( nType, ERankDetailType_Active, nActiveStartTime, nActiveEndTime,
            sharedMgr->GetMaxLoadCount( ERankDetailType_Active ) );
        QueryRankDatas( nType, ERankDetailType_LastWeek, nLastStartTime, nCurrentStartTime,
            sharedMgr->GetMaxLoadCount( ERankDetailType_LastWeek ) );
        QueryRankDatas( nType, ERankDetailType_CurrentWeek, nCurrentLoadTime, nNow,
            sharedMgr->GetMaxLoadCount( ERankDetailType_CurrentWeek ) );
    }
}

void CGlobalRankListManager::OnUpdate(time_t tNow)
{
    if (!m_bCallUpdate)
        return;

    struct tm tTime;
    memset(&tTime, 0, sizeof(tTime));
    LocalTime(&tNow, &tTime);

    bool bSaveGroup = false;
    bool bSaveIntimacy = false;
    bool bChangeToNextRange = false;
    unsigned int nSaveTime = 0;
    unsigned int nMaxGroupCount = SAVE_GROUP_MAX_COUNT;
    unsigned int nMaxIntimacyCount = SAVE_INTIMACY_MAX_COUNT;

    if (-1 == m_nLastHour)
    {
        m_nLastSaveGroupTime = (unsigned int)tNow;
        m_nLastSaveIntimacyTime = (unsigned int)tNow;
    }
    else if (23 == m_nLastHour && 0 == tTime.tm_hour)
    {
        time_t nTodayStartTime = GetTodayStartTime(tNow);
        nSaveTime = CalcSaveDataTime(nTodayStartTime - 10);

        nMaxGroupCount = m_mapUnsaveGroupHonor.size();
        nMaxIntimacyCount = m_mapUnsaveIntimacy.size();

        bSaveGroup = true;
        bSaveIntimacy = true;

        m_nLastSaveGroupTime = (unsigned int)tNow;
        m_nLastSaveIntimacyTime = (unsigned int)tNow;
    }
    else if (tTime.tm_wday == m_nStartWeekDay && 
        m_nLastHour != m_nStartHour && 
        tTime.tm_hour == m_nStartHour)
    {
        time_t nTodayStartTime = GetTodayStartTime(tNow);
        nSaveTime = CalcSaveDataTime(nTodayStartTime);

        nMaxGroupCount = m_mapUnsaveGroupHonor.size();
        nMaxIntimacyCount = m_mapUnsaveIntimacy.size();

        bSaveGroup = true;
        bSaveIntimacy = true;

        m_nLastSaveGroupTime = (unsigned int)tNow;
        m_nLastSaveIntimacyTime = (unsigned int)tNow;

        if ( m_bOpening )
            bChangeToNextRange = true;
    }
    else
    {
        nSaveTime = CalcSaveDataTime(tNow);

        if (((unsigned int)tNow - m_nLastSaveGroupTime) >= SAVE_GROUP_INTERVAL)
        {
            bSaveGroup = true;

            m_nLastSaveGroupTime = (unsigned int)tNow;
        }

        if (((unsigned int)tNow - m_nLastSaveIntimacyTime) >= SAVE_INTIMACY_INTERVAL)
        {
            bSaveIntimacy = true;

            m_nLastSaveIntimacyTime = (unsigned int)tNow;
        }
    }

    if (m_bSetClosed)
    {
        m_bCallUpdate = false;
        bSaveGroup = true;
        bSaveIntimacy = true;
    }

    if (bSaveGroup)
        SaveUnsaveDatas((unsigned short)ERankDataType_Group_Honor, m_mapUnsaveGroupHonor, nMaxGroupCount, nSaveTime);

    if (bSaveIntimacy)
        SaveUnsaveDatas((unsigned short)ERankDataType_Intimacy, m_mapUnsaveIntimacy, nMaxIntimacyCount, nSaveTime);

    if ( bChangeToNextRange )
        ChangeToNextRange();

    m_nLastHour = tTime.tm_hour;

    for ( std::multimap<unsigned int, unsigned short>::iterator bRemoveItr = m_mapUnRemoveKey.begin();
        bRemoveItr != m_mapUnRemoveKey.end(); )
    {
        SharedPtr<RankDataMgr> sharedMgr = GetDataByType( bRemoveItr->second );

        if ( !sharedMgr.IsValid() )
        {
            m_mapUnRemoveKey.erase( bRemoveItr++ );
        }
        else if ( sharedMgr->IsInitialized() )
        {
            sharedMgr->RemoveRankInfo( bRemoveItr->first );
            m_mapUnRemoveKey.erase( bRemoveItr++ );
        }
        else
        {
            ++bRemoveItr;
        }
    }
}

void CGlobalRankListManager::SetClosed()
{
    m_bSetClosed = true;
}

void CGlobalRankListManager::LoadStartTime(unsigned int nStartWeekDay, unsigned int nStartHour)
{
    if ( nStartWeekDay > 6 )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank start week day(%u) is wrong.", nStartWeekDay );
    }
    else if ( nStartHour > 23 )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank start hour(%u) is wrong.", nStartHour );
    }
    else
    {
        m_nStartWeekDay = (int)nStartWeekDay;
        m_nStartHour = (int)nStartHour;
    }
}

void CGlobalRankListManager::LoadMaxOfflineTime( int nMaxOfflineTime )
{
    if ( m_nMaxOfflineTime < nMaxOfflineTime )
        m_nMaxOfflineTime = nMaxOfflineTime;
}

void CGlobalRankListManager::SetMailInfo( const std::string &rSender, const std::string &rTitle, const std::string &rContent )
{
    if ( m_pMailInfo != NULL )
    {
        m_pMailInfo->m_strSender = rSender;
        m_pMailInfo->m_strTitle = rTitle;
        m_pMailInfo->m_strContent = rContent;
    }
}

void CGlobalRankListManager::AddHeaderName( unsigned short nHeadIndex, const std::list<std::string> &rNameList )
{
    if ( NotFoundHeaderName( nHeadIndex ) )
        m_mapHeaderName.insert( std::make_pair( nHeadIndex, rNameList ) );
}

void CGlobalRankListManager::AddRankHall( unsigned short nIndex, unsigned short nDataType,
    unsigned short nHeaderIndex, const std::string &rTitle, unsigned int nMaxCount )
{
    if ( nDataType >= ERankDataType_Max )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall data type(%u) is wrong.", nDataType );
        return;
    }
    else if ( nMaxCount == 0 )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall max count is equal to 0." );
        return;
    }
    else if ( NotFoundHeaderName( nHeaderIndex ) )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall header(%u) is wrong.", nHeaderIndex );
        return;
    }

    Map_RankData::const_iterator indexItr = m_mapIndex2Data.find( nIndex );
    Map_RankData::const_iterator typeItr = m_mapType2Data.find( nDataType );

    if ( indexItr != m_mapIndex2Data.end() )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall show index(%u) is duplicate.", nIndex );
        return;
    }
    else if ( typeItr != m_mapType2Data.end() )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall data type(%u) is duplicate.", nDataType );
        return;
    }

    RankDataMgr *pDataMgr = RankDataFactory::CreateRankData( nDataType, nHeaderIndex, rTitle, nMaxCount );
    if ( pDataMgr == NULL )
        return;

    SharedPtr<RankDataMgr> sharedMgr( pDataMgr );

    pDataMgr->Initialize();

    m_mapIndex2Data.insert( std::make_pair( nIndex, sharedMgr ) );
    m_mapType2Data.insert( std::make_pair( nDataType, sharedMgr ) );
}

void CGlobalRankListManager::AddRankReward( unsigned short nDataType, unsigned short nSecond, const std::list<CItem> &rItems, int nMoney, int nBindMCoin )
{
    if ( nDataType >= ERankDataType_Max )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall data type(%u) is wrong.", nDataType );
        return;
    }

    SharedPtr<RankDataMgr> sharedMgr = GetDataByType( nDataType );
    if ( sharedMgr.IsValid() )
    {
        sharedMgr->AddReward( CRankReward( nSecond, rItems, nMoney, nBindMCoin ) );
    }
}

void CGlobalRankListManager::RemoveFromRank(ERankDataType eDataType, unsigned int nKey)
{
    unsigned short nDataType = (unsigned short)eDataType;
    if ( nDataType >= ERankDataType_Max )
    {
        WriteLog( LOGLEVEL_ERROR, "Rank hall data type(%u) is wrong.", nDataType );
        return;
    }

    if ( g_pQueryMgr != NULL )
    {
        CRankUpdateDataStateDB *pUpdateStateDB = new CRankUpdateDataStateDB;
        pUpdateStateDB->m_nDataType = nDataType;
        pUpdateStateDB->m_nKey = nKey;
        pUpdateStateDB->m_nTime = CalcSaveDataTime( time( NULL ) );

        g_pQueryMgr->AddQuery( QUERY_Rank_UpdateDataState, 0, pUpdateStateDB );
    }

    if ( !m_bOpening )
        return;

    SharedPtr<RankDataMgr> sharedMgr = GetDataByType( nDataType );
    if ( !sharedMgr.IsValid() )
        return;
    else if ( sharedMgr->IsInitialized() )
        sharedMgr->RemoveRankInfo( nKey );
    else
        m_mapUnRemoveKey.insert( std::make_pair( nKey, nDataType ) );
}

void CGlobalRankListManager::ChangeDanceGroupRankValue( unsigned int nKey, unsigned int nValue )
{
    std::map<unsigned int, int>::iterator itr = m_mapUnsaveGroupHonor.find( nKey );

    if ( itr != m_mapUnsaveGroupHonor.end() )
        itr->second += (int)nValue;
    else
        m_mapUnsaveGroupHonor.insert( std::make_pair( nKey, nValue ) );

    if ( m_bOpening )
    {
        SharedPtr<RankDataMgr> sharedMgr = GetDataByType( ERankDataType_Group_Honor );
        if ( sharedMgr.IsValid() && sharedMgr->IsInitialized() )
        {
            CRankInfo info( nKey, nValue );
            sharedMgr->ChangeCurrentWeekValue( &info );
        }
    }
}

void CGlobalRankListManager::ChangeCoupleRankValue( unsigned int nKey, unsigned int nValue, unsigned int nMaleRoleID, unsigned int nFemaleRoleID, unsigned int nSmallRoleID )
{
    std::map<unsigned int, int>::iterator itr = m_mapUnsaveIntimacy.find( nKey );

    if ( itr != m_mapUnsaveIntimacy.end() )
        itr->second += (int)nValue;
    else
        m_mapUnsaveIntimacy.insert( std::make_pair( nKey, nValue ) );

    if ( m_bOpening )
    {
        SharedPtr<RankDataMgr> sharedMgr = GetDataByType( ERankDataType_Intimacy );
        if ( sharedMgr.IsValid() && sharedMgr->IsInitialized() )
        {
            CRankCoupleValue coupleValue;
            coupleValue.m_nPairID = nKey;
            coupleValue.m_nValue = nValue;
            coupleValue.m_nMaleRoleID = nMaleRoleID;
            coupleValue.m_nFemaleRoleID = nFemaleRoleID;
            coupleValue.m_nSamllRoleID = nSmallRoleID;

            CRankCoupleInfo info( coupleValue );
            sharedMgr->ChangeCurrentWeekValue( &info );
        }
    }
}

void CGlobalRankListManager::RegisterMessage()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRankHall);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRankDatas);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_ChangeRankValue);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRankAllDataForMedal);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2G_GetRankAllData);

    RegMsgProcessor(MSG_S2G_Rank_GetHall, &CGlobalRankListManager::OnGetRankHall);
    RegMsgProcessor(MSG_S2G_Rank_GetDatas, &CGlobalRankListManager::OnGetRankDatas);
    RegMsgProcessor(MSG_S2G_Rank_ChangeValue, &CGlobalRankListManager::OnChangeRankValue);
    RegMsgProcessor(MSG_S2G_Rank_GetAllRankDataForMedal, &CGlobalRankListManager::OnGetAllRankDatasForMedal);
    RegMsgProcessor(MSG_S2G_Rank_GetAllRankData, &CGlobalRankListManager::OnGetAllRankDatas);

    RegQueryProcessor(QUERY_Rank_GetAllDatas, &CGlobalRankListManager::OnQueryGetAllDatas);
}

void CGlobalRankListManager::QueryRankDatas(unsigned short nDataType, unsigned char nDetailType, unsigned int nStartTime, unsigned int nEndTime, unsigned int nMaxCount)
{
    if (nDataType >= ERankDataType_Max || nDetailType >= ERankDetailType_Max || nMaxCount == 0)
        return;

    RankGetAllDatasDB *pRankAllDatasDB = RankGetAllDatasDBFactory::Create( nDataType );

    if (NULL != pRankAllDatasDB)
    {
        pRankAllDatasDB->m_nDetailType = nDetailType;
        pRankAllDatasDB->m_nStartTime = nStartTime;
        pRankAllDatasDB->m_nEndTime = nEndTime;
        pRankAllDatasDB->m_nMaxCount = nMaxCount;
        pRankAllDatasDB->m_bShowGay = m_bShowGay;

        CDelHelper delHelper(pRankAllDatasDB);
        g_pQueryMgr->AddQuery(QUERY_Rank_GetAllDatas, 0, pRankAllDatasDB, 0, NULL, &delHelper);
    }
}

unsigned int CGlobalRankListManager::CalcCurrentStartTime() const
{
    return PrevTimeStamp( time( NULL ), m_nStartWeekDay, m_nStartHour );
}

unsigned int CGlobalRankListManager::CalcCurrentLoadTime( unsigned short nDataType, unsigned int nCurrentStartTime ) const
{
    switch ( nDataType )
    {
    case ERankDataType_Cloths:
    case ERankDataType_ClothEffect:
    case ERankDataType_Medal:
        return nCurrentStartTime - m_nMaxOfflineTime;
        break;

    default:
        return nCurrentStartTime;
        break;
    }
}

unsigned int CGlobalRankListManager::CalcLastStartTime(unsigned int nCurrentStartTime) const
{
    unsigned int offsetSec = DAY_SECONDES * 7;

    if (nCurrentStartTime > offsetSec)
        return (nCurrentStartTime - offsetSec);

    return 0;
}

void CGlobalRankListManager::CalcActiveTime(unsigned int &nStartTime, unsigned int &nEndTime) const
{
    unsigned int nTodayStart = (unsigned int)GetTodayStartTime( time( NULL ) );
    nStartTime = (unsigned int)(nTodayStart - DAY_SECONDES * ACTIVE_DAYS);
    nEndTime = (unsigned int)nTodayStart;
}

unsigned int CGlobalRankListManager::CalcSaveDataTime(time_t nNow) const
{
    struct tm tTime;
    memset(&tTime, 0, sizeof(tTime));
    LocalTime(&nNow, &tTime);

    tTime.tm_hour = m_nStartHour;
    tTime.tm_min = 0;
    tTime.tm_sec = 0;

    unsigned int nTodayStart = (unsigned int)mktime(&tTime);

#define MINUTE_SECONDS  60
#define OFFSET_MINUTES  10

    if (tTime.tm_wday != m_nStartWeekDay || tTime.tm_hour >= m_nStartHour)
    {
        nTodayStart += OFFSET_MINUTES * MINUTE_SECONDS;
    }
    else
    {
        nTodayStart -= OFFSET_MINUTES * MINUTE_SECONDS;
    }

    return nTodayStart;
}

void CGlobalRankListManager::ChangeToNextRange()
{
    for ( Map_RankData::iterator indexItr = m_mapIndex2Data.begin();
        indexItr != m_mapIndex2Data.end(); ++indexItr )
    {
        SharedPtr<RankDataMgr> sharedMgr = indexItr->second;
        if ( !sharedMgr.IsValid() || !sharedMgr->IsInitialized() )
        {
            continue;
        }

        sharedMgr->ChangeToNextRange( m_pMailInfo );

        RoleCumulativeRankDataMgr *pCumulativeMgr = dynamic_cast<RoleCumulativeRankDataMgr *>(sharedMgr.Get());
        if ( pCumulativeMgr != NULL )
            pCumulativeMgr->RemoveCurrentRoleByOfflineTime( m_nMaxOfflineTime );

        GameMsg_G2S_UpdateRankMinValue updateMinValue;
        updateMinValue.m_nDataType = sharedMgr->GetDataType();
        updateMinValue.m_nValue = sharedMgr->GetCurrentWeekMinValue();
        BroadcastMsg2GameServer( &updateMinValue );
    }
}

bool CGlobalRankListManager::SaveOneDataPreDay(unsigned short nDataType, unsigned int nKey, int nValue, unsigned int nSaveTime)
{
    if (nDataType >= ERankDataType_Max || 0 == nKey)
        return true;

    RankSaveDataDB *pSaveDataDB = RankSaveDataDBFactory::Create( nDataType, nKey, nValue );
    if ( NULL != pSaveDataDB )
    {
        pSaveDataDB->m_nTime = nSaveTime;

        g_pQueryMgr->AddQuery( QUERY_Rank_SaveData, 0, pSaveDataDB );

        return true;
    }

    return false;
}

void CGlobalRankListManager::SaveUnsaveDatas(unsigned short nDataType, std::map<unsigned int, int> &rUnsaveMap, unsigned int nMaxCount, unsigned int nSaveTime)
{
    unsigned int count = 0;
    std::map<unsigned int, int>::iterator itr;

    for (count = 0, itr = rUnsaveMap.begin(); 
        count < nMaxCount && itr != rUnsaveMap.end(); )
    {
        if (SaveOneDataPreDay(nDataType, itr->first, itr->second, nSaveTime) )
            rUnsaveMap.erase(itr++);
        else
            ++itr;
    }
}

bool CGlobalRankListManager::NotFoundHeaderName( unsigned short nHeadIndex ) const
{
    std::map<unsigned short, std::list<std::string> >::const_iterator itr = m_mapHeaderName.find( nHeadIndex );

    return itr == m_mapHeaderName.end();
}

SharedPtr<RankDataMgr> CGlobalRankListManager::GetDataByType( unsigned short nDataType ) const
{
    Map_RankData::const_iterator itr = m_mapType2Data.find( nDataType );

    if ( itr != m_mapType2Data.end() )
        return itr->second;

    return SharedPtr<RankDataMgr>();
}

void CGlobalRankListManager::GetRoleRank( unsigned short nDataType, unsigned char nDetailType, unsigned int nKey, unsigned int &rRoleRank ) const
{
    Map_RankData::const_iterator itr = m_mapType2Data.find( nDataType );

    if ( itr != m_mapType2Data.end() && itr->second.IsValid() )
        itr->second->GetRoleRank( nDetailType, nKey, &rRoleRank );
}

bool CGlobalRankListManager::CanChangeCurrentWeekValue( unsigned short nDataType, unsigned int nStartTime ) const
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
    case ERankDataType_Horn:
    case ERankDataType_Exp:
    case ERankDataType_Group_Honor:
    case ERankDataType_Intimacy:
    {
        unsigned int nCurrentStartTime = CalcCurrentStartTime();
        if ( nStartTime != nCurrentStartTime )
            return false;
    }
        break;

    case ERankDataType_Cloths:
    case ERankDataType_ClothEffect:
    case ERankDataType_Medal:
        break;

    default:
        return false;
        break;
    }

    return true;
}

void CGlobalRankListManager::OnGetRankHall(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetRankHall *pGetHallMsg = dynamic_cast<GameMsg_S2G_GetRankHall *>(&msg);
    if ( pGetHallMsg == NULL )
        return;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID( pGetHallMsg->nAccount );
    if ( pRoleEntity == NULL )
        return;

    unsigned int nTotal = m_mapIndex2Data.size();
    GameMsg_G2S_GetRankHallResult hallResultMsg;
    hallResultMsg.m_nCurrentPage = pGetHallMsg->m_nCurrentPage;
    hallResultMsg.m_nTotal = (unsigned short)nTotal;

    unsigned int nBeginIndex = pGetHallMsg->m_nCurrentPage * pGetHallMsg->m_nCount;
    if ( nBeginIndex < nTotal )
    {
        Map_RankData::const_iterator itr = m_mapIndex2Data.begin();
        std::advance( itr, nBeginIndex );

        SharedPtr<RankDataMgr> sharedMgr;
        for ( unsigned short nCount = 0, nMaxCount = pGetHallMsg->m_nCount;
            nCount < nMaxCount && itr != m_mapIndex2Data.end(); ++itr )
        {
            sharedMgr = itr->second;

            if ( !sharedMgr.IsValid() )
            {
                continue;
            }

            CRankTopOneInfo topOneInfo;
            topOneInfo.m_nType = (unsigned char)sharedMgr->GetDataType();
            topOneInfo.m_strTitle = sharedMgr->GetTitle();
            topOneInfo.m_bActiveOpend = sharedMgr->IsActiveOpend();
            topOneInfo.m_listHeaderName = m_mapHeaderName[sharedMgr->GetHeaderIndex()];
            topOneInfo.m_bInitialized = sharedMgr->IsInitialized();

            if ( !sharedMgr->GetTopOneInfo( topOneInfo.m_strDisplayInfo1, topOneInfo.m_strDisplayInfo2 ) )
            {
                topOneInfo.m_bInitialized = false;
            }

            hallResultMsg.m_listTopOneInfo.push_back( topOneInfo );
            ++nCount;
        }
    }

    pRoleEntity->SendPlayerMsg( &hallResultMsg );
}

void CGlobalRankListManager::OnGetRankDatas(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetRankDatas *pGetDatasMsg = dynamic_cast<GameMsg_S2G_GetRankDatas *>(&msg);
    if ( pGetDatasMsg == NULL )
        return;

    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID( pGetDatasMsg->nAccount );
    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_GetRankDatasResult datasResultMsg;
    datasResultMsg.m_nMyRank = 0;
    datasResultMsg.m_nCurrentPage = pGetDatasMsg->m_nCurrentPage;
    datasResultMsg.m_nTotal = 0;
    datasResultMsg.m_nDetailType = pGetDatasMsg->m_nDetailType;

    Map_RankData::const_iterator typeItr = m_mapType2Data.find( (unsigned short)pGetDatasMsg->m_nType );
    if ( typeItr != m_mapType2Data.end() && typeItr->second.IsValid() )
    {
        SharedPtr<RankDataMgr> sharedMgr = typeItr->second;
        unsigned int nMyRank = 0;
        unsigned int nTotal = 0;

        datasResultMsg.m_nDataType = sharedMgr->GetDataType();
        if ( sharedMgr->GetRoleRank( pGetDatasMsg->m_nDetailType, pGetDatasMsg->m_nKey, &nMyRank, &nTotal ) )
        {
            datasResultMsg.m_nMyRank = (unsigned short)nMyRank;
            datasResultMsg.m_nTotal = (unsigned short)nTotal;
            sharedMgr->GetRankDatas( pGetDatasMsg->m_nDetailType, pGetDatasMsg->m_nCurrentPage, pGetDatasMsg->m_nCount, datasResultMsg.m_listOfflineInfo );
        }
    }

    pRoleEntity->SendPlayerMsg( &datasResultMsg );
}

void CGlobalRankListManager::OnChangeRankValue(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChangeRankValue *pChangeValueMsg = dynamic_cast<GameMsg_S2G_ChangeRankValue *>(&msg);
    if ( pChangeValueMsg == NULL )
        return;

    if ( !CanChangeCurrentWeekValue( pChangeValueMsg->m_nDataType, pChangeValueMsg->m_nStartTime ) )
    {
        WriteLog( LOGLEVEL_ERROR, "Changing rank value failed, start time is not right. Type = %u, start time = %u.",
            pChangeValueMsg->m_nDataType, pChangeValueMsg->m_nStartTime );
        return;
    }

    SharedPtr<RankDataMgr> sharedMgr = GetDataByType( pChangeValueMsg->m_nDataType );
    if ( sharedMgr.IsValid() && sharedMgr->IsInitialized() )
    {
        RoleCumulativeRankDataMgr *pCumulativeMgr = dynamic_cast<RoleCumulativeRankDataMgr *>(sharedMgr.Get());
        if ( pCumulativeMgr != NULL )
        {
            CRankRoleCumulativeValue value;
            value.m_nRoleID = pChangeValueMsg->m_nKey;
            value.m_nValue = pChangeValueMsg->m_nChangeValue;
            value.m_nOfflineTime = (unsigned int)time( NULL );

            if ( !sharedMgr->IsInCurrentWeek( value.m_nRoleID ) )
                value.m_nValue = pChangeValueMsg->m_nTotalValue;

            CRankRoleCumulativeInfo info( value );
            sharedMgr->ChangeCurrentWeekValue( &info );
        }
        else
        {
            CRankInfo info( pChangeValueMsg->m_nKey, pChangeValueMsg->m_nChangeValue );
            if ( !sharedMgr->IsInCurrentWeek( info.GetKey() ) )
                info.SetValue( pChangeValueMsg->m_nTotalValue );

            sharedMgr->ChangeCurrentWeekValue( &info );
        }

        GameMsg_G2S_UpdateRankMinValue updateMinValueMsg;
        updateMinValueMsg.m_nDataType = pChangeValueMsg->m_nDataType;
        updateMinValueMsg.m_nValue = sharedMgr->GetCurrentWeekMinValue();
        BroadcastMsg2GameServer( &updateMinValueMsg );
    }
    else if ( !sharedMgr.IsValid() )
    {
        WriteLog( LOGLEVEL_ERROR, "Changing rank value failed, manager is not found. Type = %u.", pChangeValueMsg->m_nDataType );
    }
    else
    {
        WriteLog( LOGLEVEL_ERROR, "Changing rank value failed, manager is not initialized. Type = %u.", pChangeValueMsg->m_nDataType );
    }
}

void CGlobalRankListManager::OnGetAllRankDatasForMedal(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID( msg.nAccount );
    if ( NULL == pRoleEntity )
        return;

    GameMsg_S2G_GetRankAllDataForMedal &pGameSvrMsg = dynamic_cast<GameMsg_S2G_GetRankAllDataForMedal &>(msg);

    GameMsg_G2S_GetRankAllDataForMedal resultMsg;
    resultMsg.m_RankMCoin = 0;
    resultMsg.m_RankHorn = 0;
    resultMsg.m_RankExp = 0;
    resultMsg.m_RankGroupHonor = 0;
    resultMsg.m_RankIntimacy = 0;
    resultMsg.m_RankMedalScore = 0;
    resultMsg.m_RankClothEffect = 0;
    resultMsg.m_RankEquipCount = 0;

    GetRoleRank( ERankDataType_MCoin,       ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankMCoin );
    GetRoleRank( ERankDataType_Horn,        ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankHorn );
    GetRoleRank( ERankDataType_Exp,         ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankExp );
    GetRoleRank( ERankDataType_Group_Honor, ERankDetailType_CurrentWeek, pGameSvrMsg.m_GroupId,     (unsigned int &)resultMsg.m_RankGroupHonor );
    GetRoleRank( ERankDataType_Intimacy,    ERankDetailType_CurrentWeek, pGameSvrMsg.m_IntimacyId,  (unsigned int &)resultMsg.m_RankIntimacy );
    GetRoleRank( ERankDataType_Medal,       ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankMedalScore );
    GetRoleRank( ERankDataType_ClothEffect, ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankClothEffect );
    GetRoleRank( ERankDataType_Cloths,      ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankEquipCount );

    pRoleEntity->SendPlayerMsg(&resultMsg);
}

//获取大部分排行榜数据
void CGlobalRankListManager::OnGetAllRankDatas(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID( msg.nAccount );
    if ( NULL == pRoleEntity )
        return;

    GameMsg_S2G_GetRankAllData &pGameSvrMsg = dynamic_cast<GameMsg_S2G_GetRankAllData &>(msg);

    GameMsg_G2S_GetRankAllData resultMsg;
    resultMsg.m_RankMCoin = 0;
    resultMsg.m_RankHorn = 0;
    resultMsg.m_RankExp = 0;
    resultMsg.m_RankGroupHonor = 0;
    resultMsg.m_RankIntimacy = 0;
    resultMsg.m_RankMedalScore = 0;
    resultMsg.m_RankClothEffect = 0;
    resultMsg.m_RankEquipCount = 0;

    GetRoleRank( ERankDataType_MCoin,       ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankMCoin );
    GetRoleRank( ERankDataType_Horn,        ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankHorn );
    GetRoleRank( ERankDataType_Exp,         ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankExp );
    GetRoleRank( ERankDataType_Group_Honor, ERankDetailType_CurrentWeek, pGameSvrMsg.m_GroupId,     (unsigned int &)resultMsg.m_RankGroupHonor );
    GetRoleRank( ERankDataType_Intimacy,    ERankDetailType_CurrentWeek, pGameSvrMsg.m_IntimacyId,  (unsigned int &)resultMsg.m_RankIntimacy );
    GetRoleRank(ERankDataType_Medal,        ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankMedalScore );
    GetRoleRank(ERankDataType_ClothEffect,  ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankClothEffect );
    GetRoleRank(ERankDataType_Cloths,       ERankDetailType_CurrentWeek, pGameSvrMsg.m_nRoleId,     (unsigned int &)resultMsg.m_RankEquipCount );

    pRoleEntity->SendPlayerMsg(&resultMsg);
}

void CGlobalRankListManager::OnQueryGetAllDatas(QueryBase &rQuery)
{
    RankGetAllDatasDB *pRankAllDatasDB = (RankGetAllDatasDB *)rQuery.m_pAttachment;

    if ( pRankAllDatasDB != NULL && rQuery.m_nResult == Query_Res_OK )
    {
        SharedPtr<RankDataMgr> sharedMgr = GetDataByType( pRankAllDatasDB->GetDataType() );
        if ( sharedMgr.IsValid() )
        {
            sharedMgr->LoadRankInfo( pRankAllDatasDB->m_nDetailType, pRankAllDatasDB->GetRankInfo() );

            if ( pRankAllDatasDB->m_nDetailType == ERankDetailType_CurrentWeek )
            {
                WriteLog( LOGLEVEL_DEBUG, "Rank info load complete and data type is %u", pRankAllDatasDB->GetDataType() );

                unsigned int nMinValue = sharedMgr->GetCurrentWeekMinValue();
                if ( nMinValue > 0 )
                {
                    GameMsg_G2S_UpdateRankMinValue updateMinValueMsg;
                    updateMinValueMsg.m_nDataType = pRankAllDatasDB->GetDataType();
                    updateMinValueMsg.m_nValue = nMinValue;
                    BroadcastMsg2GameServer( &updateMinValueMsg );
                }
            }
        }
    }

    delete pRankAllDatasDB;
}


