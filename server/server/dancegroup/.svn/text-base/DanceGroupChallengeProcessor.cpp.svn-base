#include "DanceGroupChallengeProcessor.h"
#include "DanceGroupProcessor.h"
#include "GameMsg_S2G_DanceGroupChallenge.h"
#include "GameMsg_G2S_DanceGroupChallenge.h"

#include "../chatGrp/WChat.h"
#include "../util/CommonFunc.h"
#include "../group/Global.h"
#include "../group/GroupSystemSetting.h"
#include "../libServerFrame/QueryManager.h"

#include "../../socket/Formatter.h"
#include "../../socket/GameMsg_Map.h"

#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/DataStruct_DanceGroup.h"

extern CQueryManager* g_pQueryMgr;
typedef std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> > RoleInfoMultimap;

DanceGroupChallengeProcessor::DanceGroupChallengeProcessor(void)
    : GameMsg_Processor( true, true )
    , m_bIsOpening( false )
    , m_bLoadComplete( false )
    , m_nInspirePercentForAll( 0 )
    , m_nInspirePercentForSelf( 0 )
    , m_nInspireMaxCountForAll( 0 )
    , m_nInspireMaxCountForSelf( 0 )
    , m_nInspireRewardGroupContribution( 0 )
    , m_nResetWeekday( 0 )
    , m_nResetHour( 0 )
    , m_nStartTime( 0 )
    , m_nNextStartTime( 0 )
{
}

DanceGroupChallengeProcessor::~DanceGroupChallengeProcessor(void)
{
    Clear();
}

void DanceGroupChallengeProcessor::Initialize()
{
    m_bIsOpening = CGroupSystemSetting::Instance().IsFunctionOpen( EFunctionSwitch_DanceGroupChallenge );

    if ( !m_bIsOpening )
        return;

    RegisterMessage();
    ResetStartTime();

    g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_LoadAllInfo, m_nStartTime, &m_mapAllInfo );
}

void DanceGroupChallengeProcessor::OnUpdate( time_t tNow )
{
    if ( !m_bLoadComplete || !m_bIsOpening )
        return;

    if ( m_nNextStartTime <= (unsigned int)tNow )
    {
        Clear();
        ResetStartTime();
    }
}

void DanceGroupChallengeProcessor::SetInspireInfos( unsigned int nPercentForAll, unsigned int nPercentForSelf, 
    unsigned int nMaxCountForAll, unsigned int nMaxCountForSelf, int nRewardContribution )
{
    m_nInspirePercentForAll = nPercentForAll;
    m_nInspirePercentForSelf = nPercentForSelf;
    m_nInspireMaxCountForAll = nMaxCountForAll;
    m_nInspireMaxCountForSelf = nMaxCountForSelf;
    m_nInspireRewardGroupContribution = nRewardContribution;
}

void DanceGroupChallengeProcessor::SetResetInfos( int nWeekday, int nHour )
{
    m_nResetWeekday = nWeekday;
    m_nResetHour = nHour;
}

void DanceGroupChallengeProcessor::RegisterMessage()
{
    RegMsgCreatorAndProcessor(GameMsg_S2G_EnterChallenge, &DanceGroupChallengeProcessor::OnEnterChallenge);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GetChallengeStoreyInfo, &DanceGroupChallengeProcessor::OnGetChallengeStoreyInfo);
    RegMsgCreatorAndProcessor(GameMsg_S2G_GetChallengeRankList, &DanceGroupChallengeProcessor::OnGetChallengeRankList);
    RegMsgCreatorAndProcessor(GameMsg_S2G_ChallengeInspire, &DanceGroupChallengeProcessor::OnChallengeInspire);
    RegMsgCreatorAndProcessor(GameMsg_S2G_OpenChallengeStorey, &DanceGroupChallengeProcessor::OnOpenChallengeStorey);
    RegMsgCreatorAndProcessor(GameMsg_S2G_PrepareChallenge, &DanceGroupChallengeProcessor::OnPrepareChallenge);
    RegMsgCreatorAndProcessor(GameMsg_S2G_ReportChallengeScore, &DanceGroupChallengeProcessor::OnReportChallengeScore);

    RegQueryProcessor(QUERY_DanceGroupChallenge_LoadAllInfo, &DanceGroupChallengeProcessor::OnLoadAllInfo);
    RegQueryProcessor(QUERY_DanceGroupChallenge_RecordRanking, &DanceGroupChallengeProcessor::OnRecordRanking);
}

void DanceGroupChallengeProcessor::Clear()
{
    for ( std::map<unsigned int, DanceGroupChallengeInfo *>::iterator itr = m_mapAllInfo.begin(); 
        itr != m_mapAllInfo.end(); ++itr )
    {
        SAFE_DELETE( itr->second );
    }

    m_mapAllInfo.clear();
}

void DanceGroupChallengeProcessor::ResetStartTime()
{
    m_nStartTime = PrevTimeStamp( time( NULL ), m_nResetWeekday, m_nResetHour );
    m_nNextStartTime = m_nStartTime + 7 * DAY_PRE_SECONDS;
}

unsigned int DanceGroupChallengeProcessor::GetAllInspire( const DanceGroupChallengeContribution *pContribution ) const
{
    if ( pContribution == NULL )
        return 0;

    unsigned int nAllInspire = pContribution->GetTotalInspire();

    if ( nAllInspire <= m_nInspireMaxCountForAll )
        return nAllInspire;

    return m_nInspireMaxCountForAll;
}

unsigned int DanceGroupChallengeProcessor::CalcScoreAdditional( unsigned int nMyInspire, unsigned int nAllInspire ) const
{
    return ( nMyInspire * m_nInspirePercentForSelf + nAllInspire * m_nInspirePercentForAll );
}

void DanceGroupChallengeProcessor::OnEnterChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_EnterChallenge &rMsg = (GameMsg_S2G_EnterChallenge &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_EnterChallengeRes resMsg;

    do 
    {
        if ( !m_bLoadComplete )
        {
            resMsg.m_nResult = EEnterChallengeRes_Initializing;
            break;
        }

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        if ( pDGInfo == NULL )
        {
            resMsg.m_nResult = EEnterChallengeRes_GroupNotExist;
            break;
        }

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        if ( pInfo == NULL )
        {
            pInfo = new DanceGroupChallengeInfo( rMsg.m_nGroupID, m_nStartTime );
            m_mapAllInfo[rMsg.m_nGroupID] = pInfo;
        }

        resMsg.m_nCurStorey = pInfo->GetCurrentStorey( &resMsg.m_nCurStoreyState );
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnGetChallengeStoreyInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetChallengeStoreyInfo &rMsg = (GameMsg_S2G_GetChallengeStoreyInfo &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_GetChallengeStoreyInfoRes resMsg;
    resMsg.m_nStorey = rMsg.m_nRequestStorey;
    resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_NotStart;

    do 
    {
        BREAK_IF( !m_bLoadComplete );

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        BREAK_IF( pDGInfo == NULL );

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        BREAK_IF( pInfo == NULL );

        const DanceGroupChallengeOpendStoreyInfo *pStorey = pInfo->GetOpendStoreyInfo( rMsg.m_nRequestStorey );
        BREAK_IF( pStorey == NULL );

        if ( pStorey->IsFinished() )
        {
            resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_Finished;
        }
        else
        {
            resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_Challenging;
        }
        resMsg.m_nScene = pStorey->GetScene();
        resMsg.m_nMusic = pStorey->GetMusic();
        resMsg.m_nMusicMode = pStorey->GetMusicMode();
        resMsg.m_nMusicLevel = pStorey->GetMusicLevel();
        resMsg.m_nTargetScore = pStorey->GetTargetScore();
        resMsg.m_nCurrentScore = pStorey->GetCurrentScore();

        const DanceGroupChallengeContribution *pContribution = pInfo->GetStoreyContribution( rMsg.m_nRequestStorey );
        if ( pContribution != NULL )
        {
            unsigned int nMyInspire = pContribution->GetRoleInspire( pRoleEntity->GetRoleID() );
            unsigned int nAllInspire = GetAllInspire( pContribution );

            resMsg.m_nMyInspire = (unsigned short)nMyInspire;
            resMsg.m_nAdditional = (unsigned short)CalcScoreAdditional( nMyInspire, nAllInspire );
        }
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnGetChallengeRankList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetChallengeRankList &rMsg = (GameMsg_S2G_GetChallengeRankList &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_GetChallengeRankListRes resMsg;
    resMsg.m_nRankType = rMsg.m_nRankType;
    resMsg.m_nStorey = rMsg.m_nRequestStorey;
    resMsg.m_nCurPage = rMsg.m_nRequestPage;

    do 
    {
        BREAK_IF( !m_bLoadComplete );

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        BREAK_IF( pDGInfo == NULL );

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        BREAK_IF( pInfo == NULL );

        const DanceGroupChallengeContribution *pContribution = pInfo->GetStoreyContribution( rMsg.m_nRequestStorey );
        BREAK_IF( pContribution == NULL );

        const RoleInfoMultimap *pAllInfos = &pContribution->GetAllScoreInfos();
        if ( rMsg.m_nRankType != EDanceGroupChallengeRankType_ScoreRank )
        {
            pAllInfos = &pContribution->GetAllInspireInfos();
        }
        BREAK_IF( pAllInfos == NULL );

        int nStartPos = rMsg.m_nRequestPage * rMsg.m_nRequestCount;
        resMsg.m_nTotalCount = pAllInfos->size();
        if ( nStartPos >= resMsg.m_nTotalCount )
        {
            resMsg.m_nCurPage = 0;
            nStartPos = 0;
        }

        RoleInfoMultimap::const_iterator itr = pAllInfos->begin();
        if ( nStartPos > 0 )
        {
            std::advance( itr, nStartPos );
        }

        for ( int nCount = 1; itr != pAllInfos->end() && nCount <= rMsg.m_nRequestCount; ++nCount, ++itr )
        {
            if ( itr->second == NULL )
                continue;

            DanceGroupChallengeRankInfo rankInfo;
            rankInfo.m_nRankType = resMsg.m_nRankType;
            rankInfo.m_nRoleID = itr->second->GetRoleID();
            rankInfo.m_strRoleName = itr->second->GetRoleName();
            rankInfo.m_nRank = (unsigned short)(nStartPos + nCount);
            rankInfo.m_nScore = itr->second->GetScore();
            rankInfo.m_nMyInspire = (unsigned short)itr->second->GetInspire();

            resMsg.m_listInfo.push_back( rankInfo );
        }
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnChallengeInspire(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ChallengeInspire &rMsg = (GameMsg_S2G_ChallengeInspire &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    unsigned int nRoleID = pRoleEntity->GetRoleID();
    GameMsg_G2S_ChallengeInspireRes resMsg;
    resMsg.m_nStorey = rMsg.m_nRequestStorey;

    do 
    {
        if ( !m_bLoadComplete )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[Initializing].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_Initializing;
            break;
        }

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        if ( pDGInfo == NULL )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[GroupNotExist].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_GroupNotExist;
            break;
        }

        const CDanceGroupMember *pMember = pDGInfo->GetGroupMember( nRoleID );
        if ( pMember == NULL )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[NotInGroup].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_NotInGroup;
            break;
        }

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        if ( pInfo == NULL )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[ChallengeNotExists].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_ChallengeNotExists;
            break;
        }
        else if ( !pInfo->IsStoreyOpened( rMsg.m_nRequestStorey ) )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[StoreyNotStart].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_StoreyNotStart;
            break;
        }

        DanceGroupChallengeContribution *pContribution = pInfo->GetStoreyContribution( rMsg.m_nRequestStorey );
        if ( pContribution == NULL )
        {
            pContribution = new DanceGroupChallengeContribution( rMsg.m_nRequestStorey );
            pInfo->AddStoreyContribution( pContribution );
        }

        unsigned int nMyInspire = pContribution->GetRoleInspire( nRoleID );
        unsigned int nAllInspire = GetAllInspire( pContribution );

        if ( nMyInspire >= m_nInspireMaxCountForSelf )
        {
            WriteLog( LOGLEVEL_ERROR, "Dance group challenge inspire failed[MaxInspireCount].RoleID[%u].", nRoleID );
            resMsg.m_nResult = EChallengeInspireRes_MaxInspireCount;
            resMsg.m_nAdditional = (unsigned short)CalcScoreAdditional( nMyInspire, nAllInspire );
            break;
        }

        ++nMyInspire;
        if ( nAllInspire < m_nInspireMaxCountForAll )
        {
            ++nAllInspire;
        }
        pContribution->AddRoleInspire( nRoleID, pRoleEntity->GetName(), 1 );

        DanceGroupChallengeUpdateRoleContribution *pUpdate = new DanceGroupChallengeUpdateRoleContribution( rMsg.m_nGroupID, m_nStartTime, nRoleID );
        pUpdate->SetType( EDanceGroupChallengeUpdateRoleContributionType_Inspire );
        pUpdate->SetStorey( rMsg.m_nRequestStorey );
        pUpdate->SetInspire( nMyInspire );
        g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_UpdateContribution, SharedPtr<DanceGroupChallengeUpdateRoleContribution>(pUpdate) );

        if ( m_nInspireRewardGroupContribution > 0 )
        {
            CDanceGroupProcessor::Instance().UpdateDanceGroupContribution( rMsg.m_nGroupID, 
                nRoleID, m_nInspireRewardGroupContribution, "Challenge" );
        }

        resMsg.m_nMyInspire = (unsigned short)nMyInspire;
        resMsg.m_nAdditional = (unsigned short)CalcScoreAdditional( nMyInspire, nAllInspire );
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnOpenChallengeStorey(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_OpenChallengeStorey &rMsg = (GameMsg_S2G_OpenChallengeStorey &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_OpenChallengeStoreyRes resMsg;

    do 
    {
        if ( !m_bLoadComplete )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_Initializing;
            break;
        }

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        if ( pDGInfo == NULL )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_GroupNotExist;
            break;
        }

        const CDanceGroupMember *pMember = pDGInfo->GetGroupMember( pRoleEntity->GetRoleID() );
        if ( pMember == NULL )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotInGroup;
            break;
        }
        else if ( !CanOpenChallenge( pMember->m_nTitle ) )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NoAuthority;
            break;
        }
        else if ( pDGInfo->m_DGBaseInfo.m_nChallengeActive < rMsg.m_nActive )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotEnoughActive;
            break;
        }

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        if ( pInfo == NULL )
        {
            pInfo = new DanceGroupChallengeInfo( rMsg.m_nGroupID, m_nStartTime );
            m_mapAllInfo[rMsg.m_nGroupID] = pInfo;
        }
        if ( pInfo->IsStoreyOpened( rMsg.m_nRequestStorey ) )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_StoreyOpend;
            break;
        }
        else if ( !pInfo->IsPrevStoreyFinished( rMsg.m_nRequestStorey ) )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_PrevNotFinished;
            break;
        }

        DanceGroupChallengeOpendStoreyInfo *pOpendInfo = new DanceGroupChallengeOpendStoreyInfo;
        pOpendInfo->SetStorey( rMsg.m_nRequestStorey );
        pOpendInfo->SetScene( rMsg.m_nScene );
        pOpendInfo->SetMusic( rMsg.m_nMusic );
        pOpendInfo->SetMusicMode( rMsg.m_nMusicMode );
        pOpendInfo->SetMusicLevel( rMsg.m_nMusicLevel );
        pOpendInfo->SetTargetScore( rMsg.m_nTarget );

        pInfo->AddOpendStoreyInfo( pOpendInfo );
        g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_OpenStorey, rMsg.m_nGroupID, pOpendInfo, m_nStartTime );

        std::string strContent;
        SafeFormat( strContent, CLocalization::Instance().GetString( "DanceGroup_Record_OpenStorey" ), 
            pRoleEntity->GetName(), rMsg.m_nRequestStorey );

        CDanceGroupProcessor::Instance().OpenChallengeStoreyAndRecord( rMsg.m_nGroupID, 
            pMember, rMsg.m_nRequestStorey, rMsg.m_nActive, strContent );
        CWChatMgr::Instance().SendDanceGroupAnnouncement( rMsg.m_nGroupID, strContent );

        resMsg.m_nStorey = rMsg.m_nRequestStorey;
        resMsg.m_nScene = rMsg.m_nScene;
        resMsg.m_nMusic = rMsg.m_nMusic;
        resMsg.m_nMusicMode = rMsg.m_nMusicMode;
        resMsg.m_nMusicLevel = rMsg.m_nMusicLevel;
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnPrepareChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_PrepareChallenge &rMsg = (GameMsg_S2G_PrepareChallenge &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_PrepareChallengeRes resMsg;
    resMsg.m_nStorey = rMsg.m_nRequestStorey;
    resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_NotStart;

    do 
    {
        if ( !m_bLoadComplete )
        {
            resMsg.m_nResult = EPrepareChallengeRes_Initializing;
            break;
        }

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        if ( pDGInfo == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_GroupNotExist;
            break;
        }

        const CDanceGroupMember *pMember = pDGInfo->GetGroupMember( pRoleEntity->GetRoleID() );
        if ( pMember == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_NotInGroup;
            break;
        }

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        if ( pInfo == NULL )
        {
            pInfo = new DanceGroupChallengeInfo( rMsg.m_nGroupID, m_nStartTime );
            m_mapAllInfo[rMsg.m_nGroupID] = pInfo;
        }

        const DanceGroupChallengeOpendStoreyInfo *pStorey = pInfo->GetOpendStoreyInfo( rMsg.m_nRequestStorey );
        if ( pStorey == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_StoreyNotStart;
            break;
        }
        else if ( pStorey->IsFinished() )
        {
            resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_Finished;
        }
        else
        {
            resMsg.m_nStoreyState = EDanceGroupChallengeStoreyState_Challenging;
        }
        resMsg.m_nScene = pStorey->GetScene();
        resMsg.m_nMusic = pStorey->GetMusic();
        resMsg.m_nMusicMode = pStorey->GetMusicMode();
        resMsg.m_nMusicLevel = pStorey->GetMusicLevel();
        resMsg.m_nTargetScore = pStorey->GetTargetScore();
        resMsg.m_nCurrentScore = pStorey->GetCurrentScore();

        const DanceGroupChallengeContribution *pContribution = pInfo->GetStoreyContribution( rMsg.m_nRequestStorey );
        if ( pContribution != NULL )
        {
            unsigned int nMyInspire = pContribution->GetRoleInspire( pRoleEntity->GetRoleID() );
            unsigned int nAllInspire = GetAllInspire( pContribution );

            resMsg.m_nAdditional = (unsigned short)CalcScoreAdditional( nMyInspire, nAllInspire );
        }
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnReportChallengeScore(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_ReportChallengeScore &rMsg = (GameMsg_S2G_ReportChallengeScore &)msg;
    CGroupRoleEntity *pRoleEntity = CGroupPlayerManager::Instance().SearchRoleByAccountID(msg.nAccount);

    if ( pRoleEntity == NULL )
        return;

    GameMsg_G2S_ReportChallengeScoreRes resMsg;
    resMsg.m_scoreInfo = rMsg.m_scoreInfo;

    do 
    {
        if ( !m_bLoadComplete || m_nStartTime != rMsg.m_nStartTime )
        {
            resMsg.m_nResult = EChallengeEndRes_Closed;
            break;
        }

        const CDanceGroupInfo *pDGInfo = CDanceGroupProcessor::Instance().GetDanceGroupInfo( rMsg.m_nGroupID );
        if ( pDGInfo == NULL )
        {
            resMsg.m_nResult = EChallengeEndRes_GroupDismiss;
            break;
        }

        unsigned int nRoleID = pRoleEntity->GetRoleID();
        const CDanceGroupMember *pMember = pDGInfo->GetGroupMember( nRoleID );
        if ( pMember == NULL )
        {
            resMsg.m_nResult = EChallengeEndRes_KickOut;
            break;
        }

        DanceGroupChallengeInfo *pInfo = m_mapAllInfo[rMsg.m_nGroupID];
        BREAK_IF( pInfo == NULL );

        DanceGroupChallengeOpendStoreyInfo *pStorey = pInfo->GetOpendStoreyInfo( rMsg.m_nRequestStorey );
        BREAK_IF( pStorey == NULL );

        DanceGroupChallengeContribution *pContribution = pInfo->GetStoreyContribution( rMsg.m_nRequestStorey );
        if ( pContribution == NULL )
        {
            pContribution = new DanceGroupChallengeContribution( rMsg.m_nRequestStorey );
            pInfo->AddStoreyContribution( pContribution );
        }

        unsigned int nMyInspire = pContribution->GetRoleInspire( nRoleID );
        unsigned int nAllInspire = GetAllInspire( pContribution );
        unsigned int nAdditional = CalcScoreAdditional( nMyInspire, nAllInspire );
        unsigned int nScore = rMsg.m_scoreInfo.m_nMyTargetScore;
        nScore += nScore * nAdditional / 100;
        bool bFinished = pStorey->IsFinished();

        resMsg.m_scoreInfo.m_nTargetScore = pStorey->GetTargetScore();
        resMsg.m_scoreInfo.m_nCurScore = pStorey->GetCurrentScore() + nScore;
        resMsg.m_scoreInfo.m_nAdditional = (unsigned short)nAdditional;
        resMsg.m_scoreInfo.m_nMyTargetScore = nScore;

        pStorey->AddScore( nScore );
        pContribution->AddRoleScore( nRoleID, pRoleEntity->GetName(), nScore );

        g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_UpdateStoreyScore, rMsg.m_nGroupID, pStorey, m_nStartTime );

        DanceGroupChallengeUpdateRoleContribution *pUpdate = new DanceGroupChallengeUpdateRoleContribution( rMsg.m_nGroupID, m_nStartTime, nRoleID );
        pUpdate->SetType( EDanceGroupChallengeUpdateRoleContributionType_Score );
        pUpdate->SetStorey( rMsg.m_nRequestStorey );
        pUpdate->SetScore( pContribution->GetRoleScore( nRoleID ) );
        g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_UpdateContribution, SharedPtr<DanceGroupChallengeUpdateRoleContribution>(pUpdate) );

        BREAK_IF( bFinished ); // 之前已经完成，不需要同步
        BREAK_IF( !pStorey->IsFinished() );

        GameMsg_G2S_ChallengeStoreyFinishedNotice noticeMsg;
        noticeMsg.m_nNextStorey = rMsg.m_nRequestStorey + 1;
        CDanceGroupProcessor::Instance().SendMsg2DanceGroupMember( rMsg.m_nGroupID, 0, noticeMsg );

        const RoleInfoMultimap& rAllScoreInfos = pContribution->GetAllScoreInfos();
        BREAK_IF( rAllScoreInfos.empty() );

        DanceGroupChallengeRecordRankingInfo *pRecord = new DanceGroupChallengeRecordRankingInfo( m_nStartTime, rMsg.m_nRequestStorey );
        SharedPtr<DanceGroupChallengeRecordRankingInfo> sharePtr( pRecord );

        std::list<unsigned int> listRoleID;
        for ( RoleInfoMultimap::const_iterator itr = rAllScoreInfos.begin(); 
            itr != rAllScoreInfos.end(); ++itr )
        {
            if ( itr->second == NULL )
                continue;

            listRoleID.push_back( itr->second->GetRoleID() );
        }

        pRecord->SetAllRoleID( listRoleID );
        g_pQueryMgr->AddQuery( QUERY_DanceGroupChallenge_RecordRanking, sharePtr );
    } while (0);

    pRoleEntity->SendPlayerMsg( &resMsg );
}

void DanceGroupChallengeProcessor::OnLoadAllInfo(QueryBase &rQuery)
{
    m_bLoadComplete = true;

    WriteLog( LOGLEVEL_DEBUG, "Dance Group Challenge Info Load Complete." );
}

void DanceGroupChallengeProcessor::OnRecordRanking(QueryBase &rQuery)
{
    Query<DanceGroupChallengeRecordRankingInfo> *pTmp = (Query<DanceGroupChallengeRecordRankingInfo> *)&rQuery;

    if ( pTmp == NULL || !pTmp->IsValid() )
    {
        return;
    }

    if ( rQuery.m_nResult != Query_Res_OK )
    {
        WriteLog( LOGLEVEL_ERROR, "Dance group Challenge record ranking failed." );
        return;
    }

    WriteLog( LOGLEVEL_DEBUG, "Dance group Challenge record ranking success." );

    GameMsg_G2S_ChallengeRewardNotice noticeMsg;
    noticeMsg.m_nStorey = (*pTmp)->GetStorey();

    short nRanking = 0;
    const std::list<unsigned int> &rAllRoleID = (*pTmp)->GetAllRoleID();
    for ( std::list<unsigned int>::const_iterator itr = rAllRoleID.begin(); 
        itr != rAllRoleID.end(); ++itr )
    {
        noticeMsg.m_nRanking = ++nRanking;
        CDanceGroupProcessor::Instance().SendPlayerMsgByRoleID( *itr, noticeMsg );
    }
}

