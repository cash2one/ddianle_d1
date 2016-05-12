#include "RankComponent.h"
#include "RankConfigData.h"

#include "../EntityNetComponent/EntityNetComponent.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../Medal/EntityMedalComponent.h"

#include "GameMsg_Rank_CS.h"
#include "GameMsg_Rank_SG.h"

#include "../util/CommonFunc.h"
#include "../SystemSetting/SystemSettingMgr.h"

#include "../../socket/GameMsg_Map.h"
#include "../../datastructure/LogStructDef.h"
#include "../../datastructure/DataStruct_Rank.h"

#define GET_HALL_INTERVAL   200 // ms
#define GET_DATAS_INTERVAL  100 // ms
#define SAVE_DATA_INTERVAL  (60 * 5)    // seconds

#define RankConfigIns   CRankConfigData::Instance()


CRankComponent::CRankComponent(void)
    : CEntityComponent(true, true)
    , m_pNetComponent(NULL)
    , m_pAttrComponent(NULL)
    , m_pCoupleComponent(NULL)
    , m_pDanceGroupComponent(NULL)
    , m_pItemComponent(NULL)
    , m_pMedalComponent(NULL)
    , m_nLastGetHallTime(0)
    , m_nLastGetDatasTime(0)
    , m_nLastSaveDataTime(0)
    , m_nLastHour(-1)
{
}

CRankComponent::~CRankComponent(void)
{
}

void CRankComponent::Start()
{
    m_pNetComponent = GetComponent<CRoleNet>();
    m_pAttrComponent = GetComponent<CRoleAttribute>();
    m_pCoupleComponent = GetComponent<CRoleCouple>();
    m_pDanceGroupComponent = GetComponent<CRoleDanceGroup>();
    m_pItemComponent = GetComponent<CRoleItem>();
    m_pMedalComponent = GetComponent<CRoleMedal>();
}

void CRankComponent::OnEvent(CComponentEvent &refEvent)
{
}

void CRankComponent::OnLogin()
{
    if ( !RankConfigIns.IsOpening() )
        return;
    else if ( m_pAttrComponent == NULL )
        return;

    unsigned int nStartTime = CalcCurrentStartTime();
    if ( nStartTime == 0 )
        return;

    std::list<unsigned short> listTypes;
    RankConfigIns.GetAllTypes( listTypes );

    for ( std::list<unsigned short>::const_iterator itr = listTypes.begin();
        itr != listTypes.end(); ++itr )
    {
        unsigned short nType = *itr;
        unsigned int nKey = 0;

        if ( !GetKeyByDataType( nType, nKey, false ) || nKey == 0 )
        {
            m_mapMyRankData[nType] = 0;
            continue;
        }

        RankGetRoleDataDB *pRoleDataDB = RankGetRoleDataDBFactory::Create( nType );
        if ( pRoleDataDB == NULL )
        {
            continue;
        }

        pRoleDataDB->m_nKey = nKey;
        pRoleDataDB->m_nStartTime = nStartTime;

        CDelHelper delHelper( pRoleDataDB );
        AddQuery( QUERY_Rank_GetRoleData, 0, pRoleDataDB, 0, NULL, &delHelper );
    }
}

void CRankComponent::OnLogout()
{
    SaveUnsaveData( time( NULL ) );
}

bool CRankComponent::CreateFromDB(ROLEINFO_DB *pRoleInforDB)
{
    return true;
}

bool CRankComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
    return true;
}

void CRankComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
    time_t nNow = time(NULL);
    struct tm tTime;
    LocalTime(&nNow, &tTime);

    int nStartWeekDay = 0;
    int nStartHour = 0;

    RankConfigIns.GetStartTime( nStartWeekDay, nStartHour );

    if (23 == m_nLastHour && 0 == tTime.tm_hour)
    {
        time_t nTodayStartTime = GetTodayStartTime(nNow);

        SaveUnsaveData(nTodayStartTime - 10);
        m_nLastSaveDataTime = (unsigned int)nNow;
    }
    else if (tTime.tm_wday == nStartWeekDay && 
        m_nLastHour != nStartHour && 
        tTime.tm_hour == nStartHour && 
        -1 != m_nLastHour)
    {
        time_t nTodayStartTime = GetTodayStartTime(nNow);

        SaveUnsaveData(nTodayStartTime);
        m_nLastSaveDataTime = (unsigned int)nNow;

        for ( std::map<unsigned short, unsigned int>::iterator itr = m_mapMyRankData.begin();
            itr != m_mapMyRankData.end(); ++itr )
        {
            itr->second = 0;
        }
    }
    else if ( ((unsigned int)nNow - m_nLastSaveDataTime) >= SAVE_DATA_INTERVAL )
    {
        SaveUnsaveData( nNow );
        m_nLastSaveDataTime = (unsigned int)nNow;
    }

    m_nLastHour = tTime.tm_hour;
}

void CRankComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRankHall);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRankDatas);

    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRankHallResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRankDatasResult);

    RegMsgProcessor(MSG_C2S_Rank_GetHall, &CRankComponent::OnGetRankHall);
    RegMsgProcessor(MSG_C2S_Rank_GetDatas, &CRankComponent::OnGetRankDatas);
    RegMsgCreatorAndProcessor( GameMsg_C2S_GetRankRewardRule, &CRankComponent::OnGetRewardRule );

    RegMsgProcessor(MSG_G2S_Rank_GetHallResult, &CRankComponent::OnGetRankHallResult);
    RegMsgProcessor(MSG_G2S_Rank_GetDatasResult, &CRankComponent::OnGetRankDatasResult);

    RegQueryProcessor( QUERY_Rank_GetRoleData, &CRankComponent::OnQueryRoleData );
}

void CRankComponent::SubmitRankValue(ERankDataType eDataType, int nValue)
{
    if ( nValue == 0 )
        return;

    ForceSubmit( eDataType, nValue );
}

void CRankComponent::OnGetRankHall(GameMsg_Base &msg,CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetRankHall &rGetHallMsg = dynamic_cast<GameMsg_C2S_GetRankHall &>(msg);

    if ( RankConfigIns.IsOpening() && IsGetHallTimeOK() )
    {
        GameMsg_S2G_GetRankHall s2gGetHallMsg;

        s2gGetHallMsg.m_nCurrentPage = rGetHallMsg.m_nCurrentPage;
        s2gGetHallMsg.m_nCount = rGetHallMsg.m_nCount;

        SendGroupMsg(s2gGetHallMsg);
    }
}

void CRankComponent::OnGetRankDatas(GameMsg_Base &msg,CSlotPeer &slotPeer)
{
    if ( !(RankConfigIns.IsOpening() && IsGetDatasTimeOK()) )
        return;

    GameMsg_C2S_GetRankDatas &rGetDatasMsg = dynamic_cast<GameMsg_C2S_GetRankDatas &>(msg);

    unsigned short nDataType = (unsigned short)rGetDatasMsg.m_nType;
    unsigned int nKey = 0;

    if (!GetKeyByDataType(nDataType, nKey, true) )
    {
        GameMsg_S2C_GetRankDatasResult datasResultMsg;
        datasResultMsg.m_nMyRank = 0;
        datasResultMsg.m_nCurrentPage = rGetDatasMsg.m_nCurrentPage;
        datasResultMsg.m_nTotal = 0;

        SendPlayerMsg(datasResultMsg);

        return;
    }

    GameMsg_S2G_GetRankDatas s2gGetDatasMsg;
    s2gGetDatasMsg.m_nType = rGetDatasMsg.m_nType;
    s2gGetDatasMsg.m_nDetailType = rGetDatasMsg.m_nDetailType;
    s2gGetDatasMsg.m_nCurrentPage = rGetDatasMsg.m_nCurrentPage;
    s2gGetDatasMsg.m_nCount = rGetDatasMsg.m_nCount;
    s2gGetDatasMsg.m_nKey = nKey;

    SendGroupMsg(s2gGetDatasMsg);
}

void CRankComponent::OnGetRewardRule( GameMsg_Base &msg, CSlotPeer &slotPeer )
{
    if ( !RankConfigIns.IsOpening() )
        return;

    GameMsg_C2S_GetRankRewardRule *pGetRewards = dynamic_cast<GameMsg_C2S_GetRankRewardRule *>(&msg);
    if ( pGetRewards == NULL )
        return;

    GameMsg_S2C_GetRankRewardRuleResult ruleRes;
    ruleRes.m_strRewardRule = RankConfigIns.GetRewardRule();
    ruleRes.m_strScoreRule = RankConfigIns.GetScoreRule();

    std::list<CRankReward> listRewards;
    RankConfigIns.GetAllRewards( pGetRewards->m_nDataType, listRewards );

    unsigned short nRangeBegin = 1;
    for ( std::list<CRankReward>::const_iterator itr = listRewards.begin();
        itr != listRewards.end(); ++itr )
    {
        RankRewardsInfo info;
        info.m_nBindMCoin = itr->m_nBindMCoin;
        info.m_nMoney = itr->m_nMoney;
        info.m_nRangeBegin = nRangeBegin;
        info.m_nRangeEnd = itr->m_nSecond;
        info.m_listItems = itr->m_listItems;

        ruleRes.m_listRewards.push_back( info );
        nRangeBegin = info.m_nRangeEnd + 1;
    }

    SendPlayerMsg( ruleRes );
}

void CRankComponent::OnGetRankHallResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetRankHallResult &rHallResultMsg = dynamic_cast<GameMsg_G2S_GetRankHallResult &>(msg);
    GameMsg_S2C_GetRankHallResult s2cHallMsg;

    s2cHallMsg.m_nCurrentPage = rHallResultMsg.m_nCurrentPage;
    s2cHallMsg.m_nTotal = rHallResultMsg.m_nTotal;
    s2cHallMsg.m_listTopOneInfo = rHallResultMsg.m_listTopOneInfo;

    SendPlayerMsg(s2cHallMsg);

    // save visit log
    if (s2cHallMsg.m_nCurrentPage == 0 && m_setLogDetailType.size() == 0)
        LogRoleVisitRank(CRoleVisitRankLog::EVisitRankAction_Hall, (unsigned char)ERankDetailType_Max, 0);

    m_setLogDetailType.clear();
}

void CRankComponent::OnGetRankDatasResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetRankDatasResult &rDatasResultMsg = dynamic_cast<GameMsg_G2S_GetRankDatasResult &>(msg);
    GameMsg_S2C_GetRankDatasResult s2cDatasMsg;

    s2cDatasMsg.m_nMyRank = rDatasResultMsg.m_nMyRank;
    s2cDatasMsg.m_nCurrentPage = rDatasResultMsg.m_nCurrentPage;
    s2cDatasMsg.m_nTotal = rDatasResultMsg.m_nTotal;

    CRoleEntity *pRole = NULL;

    for (std::list<CRankOfflineInfo>::const_iterator rankOfflineitr = rDatasResultMsg.m_listOfflineInfo.begin(); 
         rankOfflineitr != rDatasResultMsg.m_listOfflineInfo.end(); ++rankOfflineitr)
    {
        CRankRowInfo rowInfo;
        rowInfo.m_nRank = rankOfflineitr->m_nRank;
        rowInfo.m_listOtherInfo = rankOfflineitr->m_listOtherInfo;
        rowInfo.m_nSmallRoleID = rankOfflineitr->m_nSmallRoleID;

        for (std::list<RoleImageOfflineData>::const_iterator offlineRoleItr = rankOfflineitr->m_listOfflineData.begin(); 
             offlineRoleItr != rankOfflineitr->m_listOfflineData.end(); ++offlineRoleItr)
        {
            CRankPlayerInfo playerInfo;

            pRole = CPlayerManager::Instance().GetEntityByRoleID(offlineRoleItr->m_nRoleID);
            if (NULL != pRole)
            {
                playerInfo.m_nTransformID = (unsigned short)pRole->TransformID();
                playerInfo.m_pRoleItem = pRole->GetItemComponent();
                playerInfo.m_pRoleGene = pRole->GetRoleGene();
                playerInfo.m_pRolePet = pRole->GetComponent<CRolePet>();
            }

            playerInfo.m_OfflineData = *offlineRoleItr;

            rowInfo.m_listPlayersInfo.push_back(playerInfo);
        }

        s2cDatasMsg.m_listRowInfo.push_back(rowInfo);
    }

    SendPlayerMsg(s2cDatasMsg);

    // save visit log
    if (m_setLogDetailType.find(rDatasResultMsg.m_nDetailType) == m_setLogDetailType.end() && 
        rDatasResultMsg.m_nDataType != ERankDataType_Max && 
        rDatasResultMsg.m_nCurrentPage == 0)
    {
        unsigned short eAction = rDatasResultMsg.m_nDataType - ERankDataType_MCoin + CRoleVisitRankLog::EVisitRankAction_MCoin;
        LogRoleVisitRank(eAction, rDatasResultMsg.m_nDetailType, rDatasResultMsg.m_nMyRank);

        m_setLogDetailType.insert(rDatasResultMsg.m_nDetailType);
    }
}

void CRankComponent::OnQueryRoleData( QueryBase &rQuery )
{
    RankGetRoleDataDB *pRoleDataDB = (RankGetRoleDataDB *)rQuery.m_pAttachment;

    if ( pRoleDataDB != NULL && rQuery.m_nResult == Query_Res_OK )
        UpdateRankData( pRoleDataDB->GetDataType(), pRoleDataDB->GetValue(), pRoleDataDB->m_nStartTime );

    delete pRoleDataDB;
}

bool CRankComponent::IsGetHallTimeOK()
{
    unsigned int nCurTick = GetTickCount();

    if (nCurTick - m_nLastGetHallTime >= GET_HALL_INTERVAL)
    {
        m_nLastGetHallTime = nCurTick;

        return true;
    }

    return false;
}

bool CRankComponent::IsGetDatasTimeOK()
{
    unsigned int nCurTick = GetTickCount();

    if (nCurTick - m_nLastGetDatasTime >= GET_DATAS_INTERVAL)
    {
        m_nLastGetDatasTime = nCurTick;

        return true;
    }

    return false;
}

unsigned int CRankComponent::CalcCurrentStartTime() const
{
    if ( !RankConfigIns.IsOpening() )
        return 0;

    int nStartWeekDay = 0, nStartHour = 0;
    RankConfigIns.GetStartTime( nStartWeekDay, nStartHour );

    return PrevTimeStamp( time( NULL ), nStartWeekDay, nStartHour );
}

unsigned int CRankComponent::CalcSaveDataTime(time_t nNow) const
{
    int nStartWeekDay = 0;
    int nStartHour = 0;

    RankConfigIns.GetStartTime( nStartWeekDay, nStartHour );

    struct tm tTime;
    memset(&tTime, 0, sizeof(tTime));

    LocalTime(&nNow, &tTime);

    tTime.tm_hour = nStartHour;
    tTime.tm_min = 0;
    tTime.tm_sec = 0;

    unsigned int nTodayStart = (unsigned int)mktime(&tTime);

#define MINUTE_SECONDS  60
#define OFFSET_MINUTES  10

    if (tTime.tm_wday != nStartWeekDay || tTime.tm_hour >= nStartHour)
    {
        nTodayStart += OFFSET_MINUTES * MINUTE_SECONDS;
    }
    else
    {
        nTodayStart -= OFFSET_MINUTES * MINUTE_SECONDS;
    }

    return nTodayStart;
}

bool CRankComponent::GetKeyByDataType(unsigned short nDataType, unsigned int &rKey, bool bFromClient) const
{
    switch (nDataType)
    {
    case ERankDataType_MCoin:
    case ERankDataType_Horn:
    case ERankDataType_Exp:
    case ERankDataType_Cloths:
    case ERankDataType_ClothEffect:
    case ERankDataType_Medal:
        {
            if (NULL != m_pAttrComponent)
            {
                rKey = m_pAttrComponent->GetRoleID();

                return true;
            }
        }
        break;

    case ERankDataType_Group_Honor:
        {
            if (NULL != m_pDanceGroupComponent && bFromClient)
            {
                rKey = m_pDanceGroupComponent->GetDanceGroupID();

                return true;
            }
        }
        break;

    case ERankDataType_Intimacy:
        {
            if (NULL != m_pCoupleComponent && bFromClient)
            {
                rKey = m_pCoupleComponent->PairID();

                return true;
            }
        }
        break;
    }

    return false;
}

void CRankComponent::SaveUnsaveData(time_t nNow)
{
    unsigned int nKey = 0;
    unsigned int nSaveTime = CalcSaveDataTime( nNow );

    for ( std::map<unsigned short, int>::iterator itr = m_mapUnsaveData.begin();
        itr != m_mapUnsaveData.end(); ++itr )
    {
        if ( !GetKeyByDataType( itr->first, nKey, false ) || nKey == 0 )
            continue;

        RankSaveDataDB *pSaveDataDB = RankSaveDataDBFactory::Create( itr->first, nKey, itr->second );
        if ( pSaveDataDB == NULL )
        {
            continue;
        }
        else if ( !pSaveDataDB->CanProcessValue() )
        {
            SAFE_DELETE( pSaveDataDB );
            continue;
        }

        pSaveDataDB->m_nTime = nSaveTime;

        AddQuery( QUERY_Rank_SaveData, 0, pSaveDataDB );

        itr->second = 0;
    }

    m_mapUnsaveData.clear();
}

void CRankComponent::SaveSubmitData( unsigned short nDataType, unsigned int nKey, int nValue )
{
    if ( nKey == 0 )
        return;

    switch ( nDataType )
    {
    case ERankDataType_MCoin:
    {
        if ( nValue <= 0 )
            break;

        RankSaveDataDB *pSaveDataDB = RankSaveDataDBFactory::Create( nDataType, nKey, nValue );
        if ( pSaveDataDB != NULL )
        {
            pSaveDataDB->m_nTime = (unsigned int)time( NULL );

            AddQuery( QUERY_Rank_SaveData, 0, pSaveDataDB );
        }
    }
        break;

    case ERankDataType_Horn:
    case ERankDataType_Exp:
    case ERankDataType_Cloths:
    case ERankDataType_ClothEffect:
    case ERankDataType_Medal:
    {
        m_mapUnsaveData[nDataType] += nValue;
    }
        break;

    default:
        break;
    }
}

void CRankComponent::LogRoleVisitRank(unsigned short eAction, unsigned char eDetailType, unsigned short nMyRank)
{
    if (m_pAttrComponent == NULL)
        return;

    CRoleVisitRankLog *pLog = new CRoleVisitRankLog;

    if (pLog != NULL)
    {
        pLog->m_nRoleID = m_pAttrComponent->GetRoleID();
        pLog->m_eAction = (CRoleVisitRankLog::EVisitRankAction)eAction;
        pLog->m_nDetailType = (unsigned short)eDetailType;
        pLog->m_nMyRank = nMyRank;

        AddQuery(QUERY_LogRoleVisitRank, 0, pLog);
    }
}

void CRankComponent::SendPlayerMsg(GameMsg_Base &msg)
{
    if (NULL != m_pNetComponent)
        m_pNetComponent->SendPlayerMsg(&msg);
}

void CRankComponent::SendGroupMsg(GameMsg_Base &msg)
{
    if (NULL != m_pNetComponent)
        m_pNetComponent->Send2GroupServer(&msg);
}

void CRankComponent::UpdateRankData( unsigned short nDataType, unsigned int nValue, unsigned int nLoadStartTime )
{
    switch ( nDataType )
    {
    case ERankDataType_MCoin:
    case ERankDataType_Horn:
    case ERankDataType_Exp:
    {
        unsigned int nNewValue = 0;
        unsigned int nStartTime = CalcCurrentStartTime();
        if ( nLoadStartTime == nStartTime )
            nNewValue = nValue;

        if ( nNewValue != 0 )
            m_mapMyRankData[nDataType] = nNewValue;
    }
        break;

    case ERankDataType_Cloths:
    {
        m_mapMyRankData[nDataType] = nValue;

        if ( nValue == 0 )
        {
            if ( m_pItemComponent != NULL )
                nValue = m_pItemComponent->GetAllPerpetualEquipCountNoRing();
            SubmitRankValue( (ERankDataType)nDataType, (int)nValue );
        }
        else if ( RankConfigIns.IsBeyondMaxOfflineTime( nLoadStartTime ) )
        {
            ForceSubmit( (ERankDataType)nDataType, 0 );
        }
    }
        break;

    case ERankDataType_ClothEffect:
    {
        m_mapMyRankData[nDataType] = nValue;

        if ( nValue == 0 )
        {
            if ( m_pItemComponent != NULL )
                nValue = m_pItemComponent->GetPerpetualEffectEquipScore();
            SubmitRankValue( (ERankDataType)nDataType, (int)nValue );
        }
        else if ( RankConfigIns.IsBeyondMaxOfflineTime( nLoadStartTime ) )
        {
            ForceSubmit( (ERankDataType)nDataType, 0 );
        }
    }
        break;

    case ERankDataType_Medal:
    {
        m_mapMyRankData[nDataType] = nValue;

        if ( nValue == 0 )
        {
            if ( m_pMedalComponent != NULL )
                nValue = (unsigned int)m_pMedalComponent->EncodePlayerMedalScore();
            SubmitRankValue( (ERankDataType)nDataType, (int)nValue );
        }
        else if ( RankConfigIns.IsBeyondMaxOfflineTime( nLoadStartTime ) )
        {
            ForceSubmit( (ERankDataType)nDataType, 0 );
        }
    }
        break;

    default:
        break;
    }
}

void CRankComponent::ForceSubmit( ERankDataType eDataType, int nValue )
{
    unsigned short nDataType = (unsigned short)eDataType;
    unsigned int nKey = 0;

    if ( !GetKeyByDataType( nDataType, nKey, false ) || nKey == 0 )
        return;

    SaveSubmitData( nDataType, nKey, nValue );

    if ( !RankConfigIns.IsOpening() )
        return;

    if ( nValue < 0 )
    {
        unsigned int nTmp = (unsigned int)(-nValue);
        if ( m_mapMyRankData[nDataType] <= nTmp )
            m_mapMyRankData[nDataType] = 0;
        else
            m_mapMyRankData[nDataType] -= nTmp;
    }
    else
    {
        m_mapMyRankData[nDataType] += (unsigned int)nValue;
    }

    if ( RankConfigIns.IsBiggerThanMinValue( nDataType, m_mapMyRankData[nDataType] ) )
    {
        GameMsg_S2G_ChangeRankValue updateValue;
        updateValue.m_nDataType = nDataType;
        updateValue.m_nKey = nKey;
        updateValue.m_nChangeValue = (unsigned int)nValue;
        updateValue.m_nTotalValue = m_mapMyRankData[nDataType];
        updateValue.m_nStartTime = CalcCurrentStartTime();

        SendGroupMsg( updateValue );
    }
}

