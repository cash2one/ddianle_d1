#include "EntityDanceGroupComponent.h"
#include "DanceGroupChallengeManager.h"

#include "../EntityNetComponent/EntityNetComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "../PhotoRoom/EntityPhotoRoomComponent.h"
#include "../AmuseRoom/AmuseRoomComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../quest/EntityQuestComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../logic/EntityTimerComponent.h"
#include "../activeness/ActivenessComponent.h"
#include "../Medal/EntityMedalComponent.h"

#include "../logic/EntityComponentEventID.h"
#include "../item/ItemProcess.h"
#include "../util/Split.h"

#include "../vip/VIPDataMgr.h"

#include "DanceGroupMsgDef.h"
#include "GameMsg_C2S_DanceGroup.h"
#include "GameMsg_S2C_DanceGroup.h"
#include "GameMsg_G2S_DanceGroup.h"
#include "GameMsg_S2G_DanceGroup.h"
#include "GameMsg_C2S_DanceGroupChallenge.h"
#include "GameMsg_S2C_DanceGroupChallenge.h"
#include "GameMsg_S2G_DanceGroupChallenge.h"

#include "../room/RoomComponent.h"
#include "../room/Room.h"
#include "../room/DanceRoom.h"
#include "../room/DanceGroupArcadeRoom.h"
#include "../room/BigMamaRoom.h"
#include "../room/NewRoomManager.h"
#include "../Couple/WeddingRoom.h"
#include "../PhotoRoom/PhotoRoom.h"
#include "../AmuseRoom/AmuseRoom.h"

#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "../../datastructure/Localization.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../../datastructure/datainfor/EffectConfig.h"

#include "../CeremonyRoom/GameMsg_CeremonyRoom.h"
#include "../CeremonyRoom/ceremonyRoom.h"
#include "../CeremonyRoom/ceremonyRoomManager.h"
#include "../logic/LogicCirculator.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "../util/NameDeny.h"
#include "../chat/EntityChatComponent.h"
#include "GameMsg_BigMama.h"
#include "GameMsg_BigMamaServer.h"
#include "../roomLib/GameMsgRoomBroadcast.h"


//fairyland 
#include "../DanceGroupFairyLand/GameMsg_FairyLand.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandCfgMgr.h"
//end

CEntityDanceGroupComponent::CEntityDanceGroupComponent(void)
    : CEntityComponent(true, true)
    , m_pNetComponent(NULL)
    , m_pRoleAttr(NULL)
    , m_pRoleItem(NULL)
    , m_pRoleRoom(NULL)
    , m_pRoleCouple(NULL)
    , m_pRolePhotoRoom(NULL)
    , m_pRoleAmuseRoom(NULL)
    , m_pRoleQuest(NULL)
    , m_pRoleQuestNew(NULL)
    , m_pRoleTimer( NULL )
    , m_pRoleVIP( NULL )
    , m_nGroupID(0)
    , m_pGroupInfo(NULL)
    , m_bTodayFirstLogin(false)
    , m_bNormalReward(false)
    , m_nLastGetRewardTime(0)
    , m_bChallengeOpening( false )
    , m_nChallengeChance( 0 )
    , m_nChallengeTotalChance( 0 )
    , m_nChallengeChanceResetTime( 0 )
    , m_nChallengeBuyChance( 0 )
    , m_nChallengeBuyChanceResetTime( 0 )
    , m_nChallengeCurStorey( MIN_CHALLENGE_STOREY )
    , m_nChallengeCurStoreyState( EDanceGroupChallengeStoreyState_NotStart )
    , m_nChallengeScene( RAN_SCENE_ID )
    , m_nChallengeMusic( RAN_MUSIC_ID )
    , m_nChallengeMusicMode( MusicMode_None )
    , m_nChallengeMusicLevel( MusicLevel_None )
    , m_nChallengeSaveStorey( 0 )
    , m_nChallengeSaveStartTime( 0 )
    , m_nChallengeStartTime( 0 )
    , m_nChallengeRoomID( INVALID_ROOM_ID )
    , m_nCeremonyRoomLastPraise(0)
    , m_entityBigMama(this)
{
    m_bChallengeOpening = CSystemSettingMgr::Instance().IsFunctionOpen( EFunctionSwitch_DanceGroupChallenge );
    m_nChallengeMyInspire = DanceGroupChallengeManager::Instance().GetInspireMaxCount();
}

CEntityDanceGroupComponent::~CEntityDanceGroupComponent(void)
{
    delete m_pGroupInfo;
    m_pGroupInfo = NULL;
}

void CEntityDanceGroupComponent::Start()
{
    m_pNetComponent = GetComponent<CRoleNet>();
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleRoom = GetComponent<CRoleRoom>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRolePhotoRoom = GetComponent<CRolePhotoRoom>();
    m_pRoleAmuseRoom = GetComponent<CRoleAmuseRoom>();
    m_pRoleQuest = GetComponent<CRoleQuest>();
    m_pRoleQuestNew = GetComponent<CRoleQuestNew>();
    m_pRoleTimer = GetComponent<CRoleTimer>();
    m_pRoleVIP = GetComponent<CRoleVIP>();

    m_pRoleRoom->AddSubscriber( this );
    m_pRoleVIP->AddSubscriber( this );
    GetComponent<CRoleActiveness>()->AddSubscriber( this );
    m_entityBigMama.Init(m_pRoleAttr, (CRoleEntity*)Entity());

    return;
}

void CEntityDanceGroupComponent::OnEvent(CComponentEvent &refEvent)
{
    if (eComponentEvent_TodayFirstLogin == refEvent.nID)
    {
        m_bTodayFirstLogin = true;
    }
    else if ( refEvent.nID == eComponentEvent_DanceGroupChallengeResetChance )
    {
        ResetChallengeChance();
    }
    else if ( refEvent.nID == eComponentEvent_DanceGroupChallengeResetBuyChance )
    {
        ResetChallengeBuyChance();
    }
    else if ( refEvent.nID == eComponentEvent_DanceGroupChallengeResetRanking )
    {
        if ( m_nGroupID != 0 )
        {
            GameMsg_S2C_ChallengeClosedNotice closeMsg;
            SendPlayerMsg( closeMsg );
        }

        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();

        UpdateChallengeRankingState( m_mapRanking, m_nChallengeStartTime, EDanceGroupChallengeRankingState_Invalid );
        m_nChallengeStartTime = mgr.GetNextStartTime( m_nChallengeStartTime );
        m_nChallengeCurStorey = MIN_CHALLENGE_STOREY;
        m_nChallengeCurStoreyState = EDanceGroupChallengeStoreyState_NotStart;
        m_nChallengeMyInspire = 0;

        if ( m_pRoleTimer != NULL )
        {
            unsigned int nNextStartTime = mgr.GetNextStartTime( m_nChallengeStartTime );
            unsigned int nCloseTime = mgr.GetCloseTime( nNextStartTime );
            unsigned int nAnnouncementTime = mgr.GetAnnouncementTime( nNextStartTime );
            m_pRoleTimer->AddTimer( nCloseTime, eComponentEvent_DanceGroupChallengeResetRanking, 0 );
            m_pRoleTimer->AddTimer( nAnnouncementTime, eComponentEvent_DanceGroupChallengeCloseAnnounce, 0 );
        }
    }
    else if ( refEvent.nID == eComponentEvent_DanceGroupChallengeCloseAnnounce )
    {
        std::string rText;
        SafeFormat( rText, CLocalization::Instance().GetString( "DanceGroup_Announce" ), 
            DanceGroupChallengeManager::Instance().GetAnnouncementSeconds() / 60 );

        GetComponent<CRoleChat>()->SendPlayerDanceGroupAnnouncement( rText );
    }
}

void CEntityDanceGroupComponent::OnLogin()
{
    GetMyDanceGroupInfo(DANCE_GROUP_CLICKED_PLACE_NONE);
    LoadChallengeRanking();

    // 推送仪式举办信息：
    GameMsg_S2C_SyncCeremonyConfig syncConfig;

    syncConfig.m_nFreeApplyTimes = CDanceGroupDataMgr::Instance().m_nCeremonyFreeApplyTimes;
    syncConfig.m_nApplyMoneyType = CDanceGroupDataMgr::Instance().m_nCeremonyApplyCostType;
    syncConfig.m_nApplyMoneyCount = CDanceGroupDataMgr::Instance().m_nCeremonyApplyCostNum;

    syncConfig.m_nFreePraiseTimes = CDanceGroupDataMgr::Instance().m_nCeremonyFreePraiseTimes;
    syncConfig.m_nPraiseMoneyType = CDanceGroupDataMgr::Instance().m_nCeremonyPraiseCostType;
    syncConfig.m_nPraiseMoneyCount = CDanceGroupDataMgr::Instance().m_nCeremonyPraiseCostNum;

    syncConfig.m_fStartDuration = (float)CDanceGroupDataMgr::Instance().m_nCeremonyStartDuration;

    m_entityBigMama.OnLogin();

    SendPlayerMsg(syncConfig);

}

void CEntityDanceGroupComponent::OnLogout()
{
    CeremonyRoomManager::Instance().OnLogout(m_pRoleAttr->GetRoleID());
    UpdateMemberInfo(false);
    m_entityBigMama.OnLogout();
}

bool CEntityDanceGroupComponent::CreateFromDB(ROLEINFO_DB *pRoleInforDB)
{
    m_nGroupID = pRoleInforDB->m_nDanceGroupID;
    m_bNormalReward = pRoleInforDB->m_bDanceGroupFirstAddPowerReward;
    m_nLastGetRewardTime = pRoleInforDB->m_nDanceGroupFirstAddPowerRewardTime;
    m_nChallengeChance = pRoleInforDB->m_nDanceGroupChallengeChance;
    m_nChallengeChanceResetTime = pRoleInforDB->m_nDanceGroupChallengeChanceResetTime;
    m_nChallengeBuyChance = pRoleInforDB->m_nDanceGroupChallengeBuyChance;
    m_nChallengeBuyChanceResetTime = pRoleInforDB->m_nDanceGroupChallengeBuyChanceResetTime;

    ResetChallengeChance();
    ResetChallengeBuyChance();

    m_entityBigMama.CreateFromDB(pRoleInforDB);


    return true;
}

bool CEntityDanceGroupComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
    pRoleInforDB->m_bDanceGroupFirstAddPowerReward = m_bNormalReward;

    return true;
}

bool CEntityDanceGroupComponent::PacketToCache(ROLEINFO_DB *pRoleInfoCache) const
{
    m_entityBigMama.PacketToCache(pRoleInfoCache);

    return true;
}


void CEntityDanceGroupComponent::OnNotify( IParam &param )
{
    int nActive = 0;
    int nAddChance = 0;
    if ( param.GetParam( "dance_group_active", nActive ) && nActive > 0 )
    {
        do 
        {
            BREAK_IF( m_nGroupID == 0 );
            BREAK_IF( !m_bChallengeOpening );

            GameMsg_S2G_AddDanceGroupChallengeActive s2gMsg;
            s2gMsg.m_nGroupID = m_nGroupID;
            s2gMsg.m_nAddValue = nActive;
            Send2GroupMsg( s2gMsg );
        } while (0);
    }
    else if ( param.GetParam( "add_challenge_chance", nAddChance ) && nAddChance > 0 )
    {
        CalcChallengeTotalChance();
        m_nChallengeChance += (unsigned short)nAddChance;

        UpdateChallengeChance();

        GameMsg_S2C_RefreshChallengeChanceNotice noticeMsg;
        noticeMsg.m_nMaxChance = m_nChallengeTotalChance;
        noticeMsg.m_nCurChance = m_nChallengeChance;
        SendPlayerMsg( noticeMsg );
    }
    else if (param.ExistParam("room_ready"))
    {
        if (INVALID_ROOM_ID != m_entityBigMama.BigMamaRoomID())
        {
            m_entityBigMama.OnReadyRoom();
            param.RemoveParam("room_ready");
        }
    }
    else if (param.ExistParam("room_loadingprogress"))
    {
        if (INVALID_ROOM_ID != m_entityBigMama.BigMamaRoomID())
        {
            int nRate = 0;
            param.GetParam("room_loadingprogress_rate", nRate);
            BigMamaRoomManager::Instance().OnLoadingProgress(((CRoleEntity*)Entity())->GetRoleID(), m_entityBigMama.BigMamaRoomID(), (unsigned char)nRate);
            param.RemoveParam("room_loadingprogress");
        }
    }
    else if (param.ExistParam("room_end"))
    {
        if (EndRoom())
            param.RemoveParam( "room_end" );
        if (INVALID_ROOM_ID != m_entityBigMama.BigMamaRoomID())
        {
            m_entityBigMama.EndRoom();
            param.RemoveParam("room_end");
        }
    }
    else if ( param.ExistParam( "report_round_mark" ) )
    {
        std::string strMD5Code, strRoundRank;
        int nMark = 0, nKeyRank = 0, nRound = 0;
        std::vector<char> vectRoundRank;

        param.GetParam( "MD5_code", strMD5Code );
        param.GetParam( "mark", nMark );
        param.GetParam( "key_rank", nKeyRank );
        param.GetParam( "round", nRound );
        param.GetParam( "round_rank", strRoundRank );

        Split( strRoundRank, "|", vectRoundRank );

        if (m_pRoleAttr != NULL )
        {
            if (m_nChallengeRoomID != INVALID_ROOM_ID)
                DanceGroupArcadeRoomManager::Instance().ReportRoundMark( m_nChallengeRoomID, m_pRoleAttr->GetRoleID(), 
                    strMD5Code, (unsigned int)nMark, (char)nKeyRank, (unsigned char)nRound, vectRoundRank );
            else if (INVALID_ROOM_ID != m_entityBigMama.BigMamaRoomID())
            {
                BigMamaRoomManager::Instance().ReportRoundMark(m_entityBigMama.BigMamaRoomID(), m_pRoleAttr->GetRoleID(),
                    strMD5Code, (unsigned int)nMark, (char)nKeyRank, (unsigned char)nRound, vectRoundRank);
            }
        }

        param.RemoveParam( "report_round_mark" );
    }
}

void CEntityDanceGroupComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestMyDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDanceGroupInfoList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CreateDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DismissDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelDismissDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelRequestEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReplyRequestEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ReplyRequestListEnterDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupName);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupColor);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupBadge);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupProfile);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupAnnouncement);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupTitle);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupTitleName);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupLeader);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDanceGroupBadgeInfos);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_UnlockDanceGroupBadge);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ChangeDanceGroupBadgeOrEffect);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DonateDanceGroupResource);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_UpgradeDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetDanceGroupRecords);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExitDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_KickOutDanceGroupMember);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupActivityGetStarInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupActivityAddStarPower);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupActivityGetStarPowerInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupActivityAddStarPowerPoint2Money);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupActivityOpenStarPacket);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupShopGetInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_DanceGroupShopExchange);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_InviteDanceGroupMember);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_AcceptDanceGroupMemberInvitation);

    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_SendSimpleDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GetDanceGroupInfoResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_SendDanceGroupInfoList);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DismissDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_CancelDismissDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_RequestEnterDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_CancelRequestEnterDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ReplyRequestEnterDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ReplyRequestListEnterDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupNameResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupColorResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupBadgeResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupProfileResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupAnnouncementResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupTitleResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupTitleNameResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ChangeDanceGroupLeaderResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_UnlockDanceGroupBadgeResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_UpgradeDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_GetDanceGroupRecrodsResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_ExitDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_KickedOutDanceGroup);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_KickOutDanceGroupMemberResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupActivityGetStarInfoResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupActivityAddStarPowerResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupActivityOpenStarPacketResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupShopExchangeResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_DanceGroupRemoved);
    GAMEMSG_REGISTERCREATOR(GameMsg_S2C_BeFefuseRefresh);

    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetMyDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_CreateDanceGroupResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AddRequestDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_RemoveRequestDanceGroupInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AddDanceGroupMemberInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_EnterDanceGroupSuccess);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_UpdateDanceGroupBaseInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_UpdateDanceGroupMemberInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_DonateDanceGroupResourceResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_RemoveDanceGroupMemberInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_UpdateDanceGroupActivityStarState);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_DanceGroupShopGetInfoResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_AcceptDanceGroupMemberInvitationResult);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_CanBeInvitedCheck);
    

// 入团仪式：
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestApplyCeremony);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestEnterCeremony);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestQuitCeremony);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestRefreshCeremony);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestStartCeremony);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetCeremonyRookie);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_CancelCeremonyRookie);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_KickCeremonyPlayer);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SaveCeremonyIntruduction);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestCeremonyRecord);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestCeremonyPraise);
	GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestPraiseRookie);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_CeremonyRoomStatics);




    RegMsgProcessor(MSG_C2S_RequestMyDanceGroupInfo, &CEntityDanceGroupComponent::OnRequestMyDanceGroupInfo);
    RegMsgProcessor(MSG_C2S_GetDanceGroupInfo, &CEntityDanceGroupComponent::OnGetDanceGroupInfo);
    RegMsgProcessor(MSG_C2S_GetDanceGroupInfoList, &CEntityDanceGroupComponent::OnGetDanceGroupInfoList);
    RegMsgProcessor(MSG_C2S_CreateDanceGroup, &CEntityDanceGroupComponent::OnCreateDanceGroup);
    RegMsgProcessor(MSG_C2S_DismissDanceGroup, &CEntityDanceGroupComponent::OnDismissDanceGroup);
    RegMsgProcessor(MSG_C2S_CancelDismissDanceGroup, &CEntityDanceGroupComponent::OnCancelDismissDanceGroup);
    RegMsgProcessor(MSG_C2S_RequestEnterDanceGroup, &CEntityDanceGroupComponent::OnRequestEnterDanceGroup);
    RegMsgProcessor(MSG_C2S_CancelRequestEnterDanceGroup, &CEntityDanceGroupComponent::OnCancelRequestEnterDanceGroup);
    RegMsgProcessor(MSG_C2S_ReplyRequestEnterDanceGroup, &CEntityDanceGroupComponent::OnReplyRequestEnterDanceGroup);
    RegMsgProcessor(MSG_C2S_ReplyRequestListEnterDanceGroup, &CEntityDanceGroupComponent::OnReplyRequestListEnterDanceGroup);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupName, &CEntityDanceGroupComponent::OnChangeDanceGroupName);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupColor, &CEntityDanceGroupComponent::OnChangeDanceGroupColor);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupBadge, &CEntityDanceGroupComponent::OnChangeDanceGroupBadge);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupProfile, &CEntityDanceGroupComponent::OnChangeDanceGroupProfile);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupAnnouncement, &CEntityDanceGroupComponent::OnChangeDanceGroupAnnouncement);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupTitle, &CEntityDanceGroupComponent::OnChangeDanceGroupTitle);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupTitleName, &CEntityDanceGroupComponent::OnChangeDanceGroupTitleName);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupLeader, &CEntityDanceGroupComponent::OnChangeDanceGroupLeader);
    RegMsgProcessor(MSG_C2S_GetDanceGroupBadgeInfos, &CEntityDanceGroupComponent::OnGetDanceGroupBadgeInfos);
    RegMsgProcessor(MSG_C2S_UnlockDanceGroupBadge, &CEntityDanceGroupComponent::OnUnlockDanceGroupBadge);
    RegMsgProcessor(MSG_C2S_ChangeDanceGroupBadgeOrEffect, &CEntityDanceGroupComponent::OnChangeDanceGroupBadgeOrEffect);
    RegMsgProcessor(MSG_C2S_DonateDanceGroupResource, &CEntityDanceGroupComponent::OnDonateDanceGroupResource);
    RegMsgProcessor(MSG_C2S_UpgradeDanceGroup, &CEntityDanceGroupComponent::OnUpgradeDanceGroup);
    RegMsgProcessor(MSG_C2S_GetDanceGroupRecords, &CEntityDanceGroupComponent::OnGetDanceGroupRecords);
    RegMsgProcessor(MSG_C2S_ExitDanceGroup, &CEntityDanceGroupComponent::OnExitDanceGroup);
    RegMsgProcessor(MSG_C2S_KickOutDanceGroupMember, &CEntityDanceGroupComponent::OnKickOutDanceGroupMember);
    RegMsgProcessor(MSG_C2S_DanceGroupActivityGetStarInfo, &CEntityDanceGroupComponent::OnDanceGroupActivityGetStarInfo);
    RegMsgProcessor(MSG_C2S_DanceGroupActivityAddStarPower, &CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPower);
    RegMsgProcessor(MSG_C2S_DanceGroupActivityGetStarPowerInfo, &CEntityDanceGroupComponent::OnDanceGroupActivityGetStarPowerInfo);
    RegMsgProcessor(MSG_C2S_DanceGroupActivityAddStarPowerPoint2Money, &CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPowerPoint2Money);
    RegMsgProcessor(MSG_C2S_DanceGroupActivityOpenStarPacket, &CEntityDanceGroupComponent::OnDanceGroupActivityOpenStarPacket);
    RegMsgProcessor(MSG_C2S_DanceGroupShopGetInfo, &CEntityDanceGroupComponent::OnDanceGroupShopGetInfo);
    RegMsgProcessor(MSG_C2S_DanceGroupShopExchange, &CEntityDanceGroupComponent::OnDanceGroupShopExchange);
    RegMsgProcessor(MSG_C2S_InviteDanceGroupMember, &CEntityDanceGroupComponent::OnInviteDanceGroupMember);
    RegMsgProcessor(MSG_C2S_AcceptDanceGroupMemberInvitation, &CEntityDanceGroupComponent::OnAcceptDanceGroupMemberInvitation);

    RegMsgProcessor(MSG_S2C_SendSimpleDanceGroupInfo, &CEntityDanceGroupComponent::OnSendSimpleDanceGroupInfo);;
    RegMsgProcessor(MSG_S2C_GetDanceGroupInfoResult, &CEntityDanceGroupComponent::OnGetDanceGroupInfoResult);
    RegMsgProcessor(MSG_S2C_SendDanceGroupInfoList, &CEntityDanceGroupComponent::OnSendDanceGroupInfoList);
    RegMsgProcessor(MSG_S2C_DismissDanceGroupResult, &CEntityDanceGroupComponent::OnDismissDanceGroupResult);
    RegMsgProcessor(MSG_S2C_CancelDismissDanceGroupResult, &CEntityDanceGroupComponent::OnCancelDismissDanceGroupResult);
    RegMsgProcessor(MSG_S2C_RequestEnterDanceGroupResult, &CEntityDanceGroupComponent::OnRequestEnterDanceGroupResult);
    RegMsgProcessor(MSG_S2C_CancelRequestEnterDanceGroupResult, &CEntityDanceGroupComponent::OnCancelRequestEnterDanceGroupResult);
    RegMsgProcessor(MSG_S2C_ReplyRequestEnterDanceGroupResult, &CEntityDanceGroupComponent::OnReplyRequestEnterDanceGroupResult);
    RegMsgProcessor(MSG_S2C_ReplyRequestListEnterDanceGroupResult, &CEntityDanceGroupComponent::OnReplyRequestListEnterDanceGroupResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupNameResult, &CEntityDanceGroupComponent::OnChangeDanceGroupNameResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupColorResult, &CEntityDanceGroupComponent::OnChangeDanceGroupColorResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupBadgeResult, &CEntityDanceGroupComponent::OnChangeDanceGroupBadgeResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupProfileResult, &CEntityDanceGroupComponent::OnChangeDanceGroupProfileResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupAnnouncementResult, &CEntityDanceGroupComponent::OnChangeDanceGroupAnnouncementResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupTitleResult, &CEntityDanceGroupComponent::OnChangeDanceGroupTitleResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupTitleNameResult, &CEntityDanceGroupComponent::OnChangeDanceGroupTitleNameResult);
    RegMsgProcessor(MSG_S2C_ChangeDanceGroupLeaderResult, &CEntityDanceGroupComponent::OnChangeDanceGroupLeaderResult);
    RegMsgProcessor(MSG_S2C_UnlockDanceGroupBadgeResult, &CEntityDanceGroupComponent::OnUnlockDanceGroupBadgeResult);
    RegMsgProcessor(MSG_G2S_ChangeDanceGroupBadgeOrEffectResult, &CEntityDanceGroupComponent::OnChangeDanceGroupBadgeOrEffectResult);
    RegMsgProcessor(MSG_S2C_UpgradeDanceGroupResult, &CEntityDanceGroupComponent::OnUpgradeDanceGroupResult);
    RegMsgProcessor(MSG_S2C_GetDanceGroupRecrodsResult, &CEntityDanceGroupComponent::OnGetDanceGroupRecrodsResult);
    RegMsgProcessor(MSG_S2C_ExitDanceGroupResult, &CEntityDanceGroupComponent::OnExitDanceGroupResult);
    RegMsgProcessor(MSG_S2C_KickedOutDanceGroup, &CEntityDanceGroupComponent::OnKickedOutDanceGroup);
    RegMsgProcessor(MSG_S2C_KickOutDanceGroupMemberResult, &CEntityDanceGroupComponent::OnKickOutDanceGroupMemberResult);
    RegMsgProcessor(MSG_S2C_DanceGroupActivityGetStarInfoResult, &CEntityDanceGroupComponent::OnDanceGroupActivityGetStarInfoResult);
    RegMsgProcessor(MSG_S2C_DanceGroupActivityAddStarPowerResult, &CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPowerResult);
    RegMsgProcessor(MSG_S2C_DanceGroupActivityGetStarPowerInfoResult, &CEntityDanceGroupComponent::OnDanceGroupActivityGetStarPowerInfoResult);
    RegMsgProcessor(MSG_S2C_DanceGroupActivityOpenStarPacketResult, &CEntityDanceGroupComponent::OnDanceGroupActivityOpenStarPacketResult);
//     RegMsgProcessor(MSG_S2C_DanceGroupShopExchangeResult, &CEntityDanceGroupComponent::OnDanceGroupShopExchangeResult);
    RegMsgProcessor(MSG_S2C_DanceGroupRemoved, &CEntityDanceGroupComponent::OnDanceGroupRemoved);
    RegMsgProcessor(MSG_S2C_DanceGroupBeRefuseRefresh, &CEntityDanceGroupComponent::OnDanceGroupBeRefuseRefresh);
    
    RegMsgProcessor(MSG_G2S_GetMyDanceGroupInfo, &CEntityDanceGroupComponent::OnGetMyDanceGroupInfo);
    RegMsgProcessor(MSG_G2S_CreateDanceGroupResult, &CEntityDanceGroupComponent::OnCreateDanceGroupResult);
    RegMsgProcessor(MSG_G2S_AddRequestDanceGroupInfo, &CEntityDanceGroupComponent::OnAddRequestDanceGroupInfo);
    RegMsgProcessor(MSG_G2S_RemoveRequestDanceGroupInfo, &CEntityDanceGroupComponent::OnRemoveRequestDanceGroupInfo);
    RegMsgProcessor(MSG_G2S_AddDanceGroupMemberInfo, &CEntityDanceGroupComponent::OnAddDanceGroupMemberInfo);
    RegMsgProcessor(MSG_G2S_EnterDanceGroupSuccess, &CEntityDanceGroupComponent::OnEnterDanceGroupSuccess);
    RegMsgProcessor(MSG_G2S_UpdateDanceGroupBaseInfo, &CEntityDanceGroupComponent::OnUpdateDanceGroupBaseInfo);
    RegMsgProcessor(MSG_G2S_UpdateDanceGroupMemberInfo, &CEntityDanceGroupComponent::OnUpdateDanceGroupMemberInfo);
    RegMsgProcessor(MSG_G2S_DonateDanceGroupResourceResult, &CEntityDanceGroupComponent::OnDonateDanceGroupResourceResult);
    RegMsgProcessor(MSG_G2S_RemoveDanceGroupMemberInfo, &CEntityDanceGroupComponent::OnRemoveDanceGroupMemberInfo);
    RegMsgProcessor(MSG_G2S_UpdateDanceGroupActivityStarState, &CEntityDanceGroupComponent::OnUpdateDanceGroupActivityStarState);
//    RegMsgProcessor(MSG_G2S_DanceGroupShopGetInfoResult, &CEntityDanceGroupComponent::OnDanceGroupShopGetInfoResult);
    RegMsgProcessor(MSG_G2S_AcceptDanceGroupMemberInvitationResult, &CEntityDanceGroupComponent::OnAcceptDanceGroupMemberInvitationResult);
    RegMsgProcessor(MSG_G2S_CanBeInviteCheck, &CEntityDanceGroupComponent::OnCanBeInviteCheck);


// 入团仪式请求函数：
	RegMsgProcessor(MSG_C2S_RequestApplyCeremony, &CEntityDanceGroupComponent::OnRequestApplyCeremony);
	RegMsgProcessor(MSG_C2S_RequestEnterCeremony, &CEntityDanceGroupComponent::OnRequestEnterCeremony);
	RegMsgProcessor(MSG_C2S_RequestQuitCeremony, &CEntityDanceGroupComponent::OnRequestQuitCeremony);
	RegMsgProcessor(MSG_C2S_RequestRefreshCeremony, &CEntityDanceGroupComponent::OnRequestRefreshCeremony);
	RegMsgProcessor(MSG_C2S_RequestStartCeremony, &CEntityDanceGroupComponent::OnRequestStartCeremony);
	RegMsgProcessor(MSG_C2S_SetCeremonyRookie, &CEntityDanceGroupComponent::OnSetCeremonyRookie);
	RegMsgProcessor(MSG_C2S_CancelCeremonyRookie, &CEntityDanceGroupComponent::OnCancelCeremonyRookie);
	RegMsgProcessor(MSG_C2S_KickCeremonyPlayer, &CEntityDanceGroupComponent::OnKickCeremonyPlayer);
	RegMsgProcessor(MSG_C2S_SaveCeremonyIntruduction, &CEntityDanceGroupComponent::OnSaveCeremonyIntruduction);
	RegMsgProcessor(MSG_C2S_RequestCeremonyRecord, &CEntityDanceGroupComponent::OnRequestCeremonyRecord);
	RegMsgProcessor(MSG_C2S_RequestCeremonyPraise, &CEntityDanceGroupComponent::OnRequestCeremonyPraise);
    RegMsgProcessor(MSG_C2S_RequestPraiseRookie, &CEntityDanceGroupComponent::OnRequestPraiseRookie);
    RegMsgProcessor(MSG_G2S_CeremonyRoomStatics, &CEntityDanceGroupComponent::OnCeremonyRoomStatics);
    RegMsgCreatorAndProcessor(GameMsg_G2S_UpdateDanceGroupChallengeActive, &CEntityDanceGroupComponent::OnUpdateChallengeActive);

    RegChallengeNetMsgMap();

    RegBigMamaMsg();
}

void CEntityDanceGroupComponent::RegBigMamaMsg()
{
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaInfoNotifyOnLogin, &CEntityDanceGroupComponent::OnG2SBigMamaInfoNotifyOnLogin);
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaInfoNotifyOnJoinDanceGroup, &CEntityDanceGroupComponent::OnG2SBigMamaInfoNotifyOnJoinDanceGroup);
    //    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaStateNotify, &CEntityDanceGroupComponent::OnG2SNoticeBigMamaState);
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaSyncStageInfo, &CEntityDanceGroupComponent::OnG2SBroadBigMamaInfo);
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaStageInfoNotify, &CEntityDanceGroupComponent::OnG2SBigMamaInfoNotify);
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaFightResult, &CEntityDanceGroupComponent::OnG2SBigMamaFightResult);
    RegMsgCreatorAndProcessor(GameMsg_G2S_BigMamaGetRewardResult, &CEntityDanceGroupComponent::OnG2SBigMamaGetRewardResult);

    if (CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_BigMama))
    {
        RegMsgCreatorAndProcessor(GameMsg_C2S_RequestBigMamaStageInfo, &CEntityDanceGroupComponent::OnRequestBigMamaStageInfo);
        RegMsgCreatorAndProcessor(GameMsg_C2S_RequestFightBigMama, &CEntityDanceGroupComponent::OnRequestFightBigMama);
        RegMsgCreatorAndProcessor(GameMsg_C2S_RequestGetBigMamaReward, &CEntityDanceGroupComponent::OnRequestGetBigMamaReward);
        RegMsgCreatorAndProcessor(GameMsg_C2S_CompleteBigMamaGuide, &CEntityDanceGroupComponent::OnCompleteBigMamaGuide);
        RegMsgCreatorAndProcessor(GameMsg_C2S_RequestBigMamaGuideReward, &CEntityDanceGroupComponent::OnRequestBigMamaGuideReward);
    }
    //舞团秘境
    RegFairyLandMsg();
    return;
}

void CEntityDanceGroupComponent::DanceGroupTest(int nPara1, int nPara2, int nPara3)
{
    //int nIndex = nPara1;
    int nIndex = -1;
    CSlotPeer slotPeer;

    switch (nIndex)
    {
    case 0: // 捐赠资源
        {
            GameMsg_C2S_DonateDanceGroupResource msgDonate;
            msgDonate.m_nResA = nPara2;
            msgDonate.m_nResB = nPara2;
            msgDonate.m_nResC = nPara3;

            OnDonateDanceGroupResource(msgDonate, slotPeer);

            LOG_DEBUG << "Test player donate resources" << std::endl;
        }
        break;

    case 1: // 升级
        {
            GameMsg_C2S_UpgradeDanceGroup msgUpgrade;

            OnUpgradeDanceGroup(msgUpgrade, slotPeer);

            LOG_DEBUG << "Test upgrade dance group" << std::endl;
        }
        break;

    case 2: // 修改舞团名称
        {
            GameMsg_C2S_ChangeDanceGroupName msgChangeGroupName;
            Formatter(msgChangeGroupName.m_strNewGroupName) << "hello" << nPara2;

            OnChangeDanceGroupName(msgChangeGroupName, slotPeer);

            LOG_DEBUG << "Test change dance group name" << std::endl;
        }
        break;

    case 3: // 修改舞团荣誉
        {
            ChangeDanceGroupHonor(nPara2, EChangeDanceGroupHonorCause_MemberLogin, nPara3);

            LOG_DEBUG << "Test change dance group honor" << std::endl;
        }
        break;

    default:
        break;
    }
}

CDanceGroupInfo* CEntityDanceGroupComponent::GetDanceGroupInfo()
{
    return m_pGroupInfo;
}

const std::map<unsigned int, CDanceGroupMember *>& CEntityDanceGroupComponent::GetDanceGroupMemberMap()
{
    static std::map<unsigned int, CDanceGroupMember *> emptyMap;

    if (NULL != m_pGroupInfo)
        return m_pGroupInfo->m_MemberMap;

    return emptyMap;
}

unsigned int CEntityDanceGroupComponent::GetDanceGroupTotalHonor()
{
    if (NULL != m_pGroupInfo)
        return m_pGroupInfo->m_DGBaseInfo.m_nHonor;

    return 0;
}

unsigned int CEntityDanceGroupComponent::GetDanceGroupTotalContribution()
{
    unsigned int nContribution = 0;
    const std::map<unsigned int, CDanceGroupMember *> &rMember = GetDanceGroupMemberMap();

    for (std::map<unsigned int, CDanceGroupMember *>::const_iterator bItr = rMember.begin(), 
         eItr = rMember.end(); bItr != eItr; ++bItr)
    {
        nContribution += bItr->second->m_nContribution;
    }

    return nContribution;
}

void CEntityDanceGroupComponent::ChangeDanceGroupHonor(int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara)
{
	ChangeDanceGroupHonor(m_nGroupID,nChangeValue,eCause,nPara);
}

void CEntityDanceGroupComponent::ChangeDanceGroupHonor(unsigned int nGroupID ,int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara)
{
	if (0 != nGroupID && 0 != nChangeValue)
	{
		GameMsg_S2G_ChangeDanceGroupHonor changeHonorMsg;
		changeHonorMsg.m_nGroupID = nGroupID;
		changeHonorMsg.m_nChangeHonor = nChangeValue;
		changeHonorMsg.m_nCause = eCause;
		changeHonorMsg.m_nPara = nPara;

		Send2GroupMsg(changeHonorMsg);
	}
}

void CEntityDanceGroupComponent::AddDanceGroupByName(const std::string strDanceGroupName)
{
    GameMsg_S2G_RequestEnterDanceGroupByName s2g;
    s2g.m_nMyGroupID = m_nGroupID;
    s2g.m_strRequestDanceGroupName = strDanceGroupName;
    Send2GroupMsg(s2g);

    return ;
}

void CEntityDanceGroupComponent::ChangeDanceGroupContribution(unsigned int nChangeValue)
{
    if (0 != m_nGroupID && 0 != nChangeValue)
    {
        GameMsg_S2G_AddDanceGroupContribution addContributionMsg;
        addContributionMsg.m_nGroupID = m_nGroupID;
        addContributionMsg.m_nChangeValue = nChangeValue;

        Send2GroupMsg(addContributionMsg);
    }
}

CDanceGroupMember* CEntityDanceGroupComponent::GetSelfMemberInfo()
{
    if (NULL == m_pRoleAttr)
        return NULL;
    if (NULL == m_pGroupInfo)
        return NULL;

    return m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID()); // 
}

CeremonyRoom* CEntityDanceGroupComponent::GetCeremonyRoom()
{
    ENSURE_WITH_LOG_CMD(NULL != m_pRoleAttr, return NULL, "error to get role attr entity");
    CeremonyRoom* room = CeremonyRoomManager::Instance().GetCeremonyRoom(GetDanceGroupID());
    if (NULL == room)
        return NULL; // 舞团没有房间

    if (room->HasMember(m_pRoleAttr->GetRoleID()))
        return room;

    return NULL; // 
}

void CEntityDanceGroupComponent::GmGetCeremonyRoomNum(int queryType)
{
    if (queryType == 0)
    { // group 数据
        GameMsg_S2G_CeremonyRoomStatics msg;
        Send2GroupMsg(msg); 
    }
    else
    { // 本线数据：
        CRoleChat *pChat = GetComponent<CRoleChat>();
        if (NULL != pChat)
            pChat->SysChat("current room num: %d", CeremonyRoomManager::Instance().CeremonyRoomNum());
    }

    return ;
}

bool CEntityDanceGroupComponent::IsInChallengeRoom() const
{
    return m_nChallengeRoomID != INVALID_ROOM_ID;
}

bool CEntityDanceGroupComponent::IsInBigMamaRoom() const
{
    return m_entityBigMama.BigMamaRoomID() != INVALID_ROOM_ID;
}


void CEntityDanceGroupComponent::OnLevelUp()
{
    UpdateMemberInfo(true);
}

void CEntityDanceGroupComponent::OnRoleNameChanged()
{
    UpdateMemberInfo(true);
}

void CEntityDanceGroupComponent::OnVIPChange()
{
    UpdateMemberInfo(true);
}

void CEntityDanceGroupComponent::OnRequestMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestMyDanceGroupInfo &rMsg = dynamic_cast<GameMsg_C2S_RequestMyDanceGroupInfo &>(msg);

    GetMyDanceGroupInfo(rMsg.m_nClickedPlace);
}

void CEntityDanceGroupComponent::OnGetDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetDanceGroupInfo &rMsg = dynamic_cast<GameMsg_C2S_GetDanceGroupInfo &>(msg);

    GameMsg_S2G_GetDanceGroupInfo getInfoMsg;
    getInfoMsg.m_nGroupID = rMsg.m_nGroupID;

    if (0 == getInfoMsg.m_nGroupID)
        getInfoMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(getInfoMsg);
}

void CEntityDanceGroupComponent::OnGetDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    Send2GroupMsg(msg);
}

void CEntityDanceGroupComponent::OnCreateDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_CreateDanceGroup &rMsg = dynamic_cast<GameMsg_C2S_CreateDanceGroup &>(msg);

    if (NULL != m_pRoleAttr && NULL != m_pRoleItem)
    {
        GameMsg_S2C_CreateDanceGroupResult resMsg;
        resMsg.m_nResult = ECreateDanceGroupResult_Success;

        if (0 != m_nGroupID)
        {
            resMsg.m_nResult = ECreateDanceGroupResult_HaveAGroup;
        }
        else if (m_pRoleAttr->GetLevel() < cMIN_LEVEL_FOR_CREATE_DANCE_GROUP)
        {
            resMsg.m_nResult = ECreateDanceGroupResult_LevelLimit;
        }
        else if (0 == m_pRoleItem->GetItemCount(cITEM_TYPE_FOR_CREATE_DANCE_GROUP, false, false))
        {
            resMsg.m_nResult = ECreateDanceGroupResult_NoCreateItem;
        }
        else if ( !NameDeny::Instance().IsValidText( rMsg.m_strGroupName ) )
        {
            resMsg.m_nResult = ECreateDanceGroupResult_InvalidName;
        }

        if (ECreateDanceGroupResult_Success != resMsg.m_nResult)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_CreateDanceGroup createMsg;
        createMsg.m_strGroupName = rMsg.m_strGroupName;
        createMsg.m_nBadge = rMsg.m_nBadge;
        createMsg.m_strProfile = rMsg.m_strProfile;
        createMsg.m_nLastOffLineTime = m_pRoleAttr->GetLastLogoutTime();

        NameDeny::Instance().FilterInvalidText( createMsg.m_strProfile );

        Send2GroupMsg(createMsg);
    }
}

void CEntityDanceGroupComponent::OnDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_DismissDanceGroupResult resMsg;
        resMsg.m_nResult = EDismissDanceGroupResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_DismissDanceGroup dismissMsg;
    dismissMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(dismissMsg);
}

void CEntityDanceGroupComponent::OnCancelDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_CancelDismissDanceGroupResult resMsg;
        resMsg.m_nResult = ECancelDismissDanceGroupResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_CancelDismissDanceGroup cancelDismissMsg;
    cancelDismissMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(cancelDismissMsg);
}

void CEntityDanceGroupComponent::OnRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestEnterDanceGroup &rMsg = dynamic_cast<GameMsg_C2S_RequestEnterDanceGroup &>(msg);

    if (0 != m_nGroupID)
    {
        GameMsg_S2C_RequestEnterDanceGroupResult resMsg;
        resMsg.m_nResult = ERequestEnterDanceGroupResult_HaveAGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_RequestEnterDanceGroup requestMsg;
    requestMsg.m_nMyGroupID = m_nGroupID;
    requestMsg.m_nRequestGroupID = rMsg.m_nGroupID;

    Send2GroupMsg(requestMsg);
}

void CEntityDanceGroupComponent::OnCancelRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    Send2GroupMsg(msg);
}

void CEntityDanceGroupComponent::OnReplyRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ReplyRequestEnterDanceGroup &rMsg = dynamic_cast<GameMsg_C2S_ReplyRequestEnterDanceGroup &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ReplyRequestEnterDanceGroupResult resMsg;
        resMsg.m_nRequestRoleID = rMsg.m_nRequestRoleID;
        resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ReplyRequestEnterDanceGroup replyMsg;
    replyMsg.m_nGroupID = m_nGroupID;
    replyMsg.m_nRequestRoleID = rMsg.m_nRequestRoleID;
    replyMsg.m_bAllow = rMsg.m_bAllow;

    Send2GroupMsg(replyMsg);
}

void CEntityDanceGroupComponent::OnReplyRequestListEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ReplyRequestListEnterDanceGroup &rMsg = dynamic_cast<GameMsg_C2S_ReplyRequestListEnterDanceGroup &>(msg);

    if (0 == m_nGroupID)
    {
        if (NULL != m_pRoleAttr)
        {
            GameMsg_S2C_ReplyRequestListEnterDanceGroupResult resMsg;
            resMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
            resMsg.m_nAllowCount = 0;
            resMsg.m_nResult = EReplyRequestEnterDanceGroupResult_NotInGroup;
            resMsg.m_bAllow = rMsg.m_bAllow;

            SendPlayerMsg(resMsg);
        }

        return;
    }

    GameMsg_S2G_ReplyRequestListEnterDanceGroup replyListMsg;
    replyListMsg.m_bAllow = rMsg.m_bAllow;
    replyListMsg.m_nGroupID = m_nGroupID;
    replyListMsg.m_listRoleID = rMsg.m_listRoleID;

    Send2GroupMsg(replyListMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupName &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupName &>(msg);

    if (NULL != m_pRoleItem)
    {
        GameMsg_S2C_ChangeDanceGroupNameResult resMsg;
        resMsg.m_strNewGroupName = rMsg.m_strNewGroupName;
        resMsg.m_nResult = EChangeDanceGroupNameResult_Success;

        if (0 == m_nGroupID)
        {
            resMsg.m_nResult = EChangeDanceGroupNameResult_NoInGroup;
        }
        else if (0 == m_pRoleItem->GetItemCount(cITEM_CHANGE_DANCE_GOURP_NAME_CARD, false, false))
        {
            resMsg.m_nResult = EChangeDanceGroupNameResult_NoRequiredItem;
        }
        else if ( !NameDeny::Instance().IsValidText( rMsg.m_strNewGroupName ) )
        {
            resMsg.m_nResult = EChangeDanceGroupNameResult_InvalidName;
        }

        if (EChangeDanceGroupNameResult_Success != resMsg.m_nResult)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_ChangeDanceGroupName changeNameMsg;
        changeNameMsg.m_nGroupID = m_nGroupID;
        changeNameMsg.m_strNewGroupName = rMsg.m_strNewGroupName;

        Send2GroupMsg(changeNameMsg);
    }
}

void CEntityDanceGroupComponent::OnChangeDanceGroupColor(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupColor &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupColor &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupColorResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupColorResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupColor changeColorMsg;
    changeColorMsg.m_nGroupID = m_nGroupID;
    changeColorMsg.m_nColor = rMsg.m_nColor;

    Send2GroupMsg(changeColorMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupBadge &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupBadge &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupBadgeResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupBadge changeBadgeMsg;
    changeBadgeMsg.m_nGroupID = m_nGroupID;
    changeBadgeMsg.m_nBadge = rMsg.m_nBadge;

    Send2GroupMsg(changeBadgeMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupProfile(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupProfile &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupProfile &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupProfileResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupProfileResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupProfile changeProfileMsg;
    changeProfileMsg.m_nGroupID = m_nGroupID;
    changeProfileMsg.m_strProfile = rMsg.m_strProfile;

    NameDeny::Instance().FilterInvalidText( changeProfileMsg.m_strProfile );

    Send2GroupMsg(changeProfileMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupAnnouncement &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupAnnouncement &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupAnnouncementResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupAnnouncementResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupAnnouncement changeAnnouncementMsg;
    changeAnnouncementMsg.m_nGroupID = m_nGroupID;
    changeAnnouncementMsg.m_strAnnouncement = rMsg.m_strAnnouncement;

    NameDeny::Instance().FilterInvalidText( changeAnnouncementMsg.m_strAnnouncement );

    Send2GroupMsg(changeAnnouncementMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupTitle(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupTitle &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupTitle &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupTitleResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupTitleResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupTitle changeTitleMsg;
    changeTitleMsg.m_nGroupID = m_nGroupID;
    changeTitleMsg.m_nTargetRoleID = rMsg.m_nTargetRoleID;
    changeTitleMsg.m_nNewTitle = (unsigned char)rMsg.m_nNewTitle;

    Send2GroupMsg(changeTitleMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupTitleName(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupTitleName &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupTitleName &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupTitleNameResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupTitleNameResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupTitleName changeTitleNameMsg;
    changeTitleNameMsg.m_nGroupID = m_nGroupID;
    changeTitleNameMsg.m_nTitle = (unsigned char)rMsg.m_nTitle;
    changeTitleNameMsg.m_strTitleName = rMsg.m_strTitleName;

    NameDeny::Instance().FilterInvalidText( changeTitleNameMsg.m_strTitleName );

    Send2GroupMsg(changeTitleNameMsg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupLeader(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupLeader &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupLeader &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ChangeDanceGroupLeaderResult resMsg;
        resMsg.m_nResult = EChangeDanceGroupLeaderResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ChangeDanceGroupLeader changeLeaderMsg;
    changeLeaderMsg.m_nGroupID = m_nGroupID;
    changeLeaderMsg.m_nNewLeaderID = rMsg.m_nNewLeaderID;

    Send2GroupMsg(changeLeaderMsg);
}

void CEntityDanceGroupComponent::OnGetDanceGroupBadgeInfos(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_GetDanceGroupBadgeInfosResult resMsg;

        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupBadgeCond();

        if (EDanceGroupBadgeResult_Success == resMsg.m_nErrorCode)
        {
            bool bLocked = true;
            std::set<int> unlockSet;
            std::set<int>::iterator unlockItr;
            SplitString(m_pGroupInfo->m_DGBaseInfo.m_strBadgeEffectLock, ",", unlockSet);

            std::map<unsigned short, CDanceGroupBadgeEffectConfig *> &rEffectMap = ConfigManager::Instance().GetEffectConfigMgr().GetDanceGroupBadgeEffectList();
            for (std::map<unsigned short, CDanceGroupBadgeEffectConfig *>::const_iterator bIt = rEffectMap.begin(), 
                 eIt = rEffectMap.end(); bIt != eIt; ++bIt)
            {
                resMsg.m_vectBadgeEffect.push_back(bIt->first);

                bLocked = true;

                if (bIt->second->nParam1 > 0 || 
                    bIt->second->nParam2 > 0 || 
                    bIt->second->nParam3 > 0)
                {
                    unlockItr = unlockSet.find(bIt->second->nId);

                    if (unlockSet.end() != unlockItr)
                    {
                        bLocked = false;

                        unlockSet.erase(unlockItr);
                    }
                }
                else
                    bLocked = false;

                resMsg.m_vectEffectState.push_back(bLocked);
            }
        }

        SendPlayerMsg(resMsg);
    }
}

void CEntityDanceGroupComponent::OnUnlockDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_UnlockDanceGroupBadge &rMsg = dynamic_cast<GameMsg_C2S_UnlockDanceGroupBadge &>(msg);

    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_UnlockDanceGroupBadgeResult resMsg;

        resMsg.m_nType = rMsg.m_nType;
        resMsg.m_nUnlockID = rMsg.m_nUnlockID;
        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupBadgeCond();

        if (EDanceGroupBadgeResult_Success == resMsg.m_nErrorCode)
        {
            if (EUnlockDanceGroupBadgeType_Badge == rMsg.m_nType)
            {
                CDanceGroupBadgeConfig *pBadgeInfo = ConfigManager::Instance().GetEffectConfigMgr().GetDanceGroupBadgeByID(rMsg.m_nUnlockID);

                if (NULL != pBadgeInfo)
                {
                    SendPlayerMsg(resMsg);

                    return;
                }
            }
            else if (EUnlockDanceGroupBadgeType_Effect == rMsg.m_nType)
            {
                CDanceGroupBadgeEffectConfig *pBadgeEffectInfo = ConfigManager::Instance().GetEffectConfigMgr().GetDanceGroupBadgeEffectByID(rMsg.m_nUnlockID);

                if (NULL != pBadgeEffectInfo)
                {
                    GameMsg_S2G_UnlockDanceGroupBadge unlockBadgeMsg;
                    unlockBadgeMsg.m_nGoupID = m_nGroupID;
                    unlockBadgeMsg.m_nType = rMsg.m_nType;
                    unlockBadgeMsg.m_nUnlockID = rMsg.m_nUnlockID;
                    unlockBadgeMsg.m_nResA = pBadgeEffectInfo->nParam1;
                    unlockBadgeMsg.m_nResB = pBadgeEffectInfo->nParam2;
                    unlockBadgeMsg.m_nResC = pBadgeEffectInfo->nParam3;

                    Send2GroupMsg(unlockBadgeMsg);

                    return;
                }
            }

            resMsg.m_nErrorCode = EDanceGroupBadgeResult_NotExistBadgeOrEffect;
        }

        SendPlayerMsg(resMsg);
    }
}

void CEntityDanceGroupComponent::OnChangeDanceGroupBadgeOrEffect(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ChangeDanceGroupBadgeOrEffect &rMsg = dynamic_cast<GameMsg_C2S_ChangeDanceGroupBadgeOrEffect &>(msg);

    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew resMsg;

        resMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        resMsg.m_nBadge = rMsg.m_nBadgeID;
        resMsg.m_nEffect = rMsg.m_nEffectID;
        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupBadgeCond();

        if (EDanceGroupBadgeResult_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }
        else if ((0 == rMsg.m_nBadgeID || rMsg.m_nBadgeID == m_pGroupInfo->m_DGBaseInfo.m_nBadge) && 
            (0 == rMsg.m_nEffectID || rMsg.m_nEffectID == m_pGroupInfo->m_DGBaseInfo.m_nEffect))
        {
            SendPlayerMsg(resMsg);

            return;
        }

        unsigned int nNeedResA = 0;
        unsigned int nNeedResB = 0;
        unsigned int nNeedResC = 0;

        if (rMsg.m_nBadgeID > 0 && rMsg.m_nBadgeID != m_pGroupInfo->m_DGBaseInfo.m_nBadge)
        {
            CDanceGroupBadgeConfig *pBadgeInfo = ConfigManager::Instance().GetEffectConfigMgr().GetDanceGroupBadgeByID(rMsg.m_nBadgeID);

            if (NULL != pBadgeInfo)
            {
                if (pBadgeInfo->m_nUnlockLevel > m_pGroupInfo->m_DGBaseInfo.m_nLevel)
                {
                    resMsg.m_nErrorCode = EDanceGroupBadgeResult_BadgeLock;
                }
                else
                {
                    nNeedResA = pBadgeInfo->nParam4;
                    nNeedResB = pBadgeInfo->nParam5;
                    nNeedResC = pBadgeInfo->nParam6;
                }
            }
            else
                resMsg.m_nErrorCode = EDanceGroupBadgeResult_NotExistBadgeOrEffect;
        }

        if (EDanceGroupBadgeResult_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        if (rMsg.m_nEffectID > 0 && rMsg.m_nEffectID != m_pGroupInfo->m_DGBaseInfo.m_nEffect)
        {
            CDanceGroupBadgeEffectConfig *pEffectInfo = ConfigManager::Instance().GetEffectConfigMgr().GetDanceGroupBadgeEffectByID(rMsg.m_nEffectID);

            if (NULL != pEffectInfo)
            {
                if (pEffectInfo->nParam1 > 0 || 
                    pEffectInfo->nParam2 > 0 || 
                    pEffectInfo->nParam3 > 0)
                {
                    std::set<int> unlockSet;
                    SplitString(m_pGroupInfo->m_DGBaseInfo.m_strBadgeEffectLock, ",", unlockSet);

                    if (unlockSet.end() == unlockSet.find(pEffectInfo->nId))
                        resMsg.m_nErrorCode = EDanceGroupBadgeResult_EffectLock;
                }

                nNeedResA += pEffectInfo->nParam4;
                nNeedResB += pEffectInfo->nParam5;
                nNeedResC += pEffectInfo->nParam6;
            }
            else
                resMsg.m_nErrorCode = EDanceGroupBadgeResult_NotExistBadgeOrEffect;
        }

        if (EDanceGroupBadgeResult_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_ChangeDanceGroupBadgeOrEffect changeBadgeOrEffectMsg;
        changeBadgeOrEffectMsg.m_nGroupID = m_nGroupID;
        changeBadgeOrEffectMsg.m_nBadgeID = rMsg.m_nBadgeID;
        changeBadgeOrEffectMsg.m_nEffectID = rMsg.m_nEffectID;
        changeBadgeOrEffectMsg.m_nResA = nNeedResA;
        changeBadgeOrEffectMsg.m_nResB = nNeedResB;
        changeBadgeOrEffectMsg.m_nResC = nNeedResC;

        Send2GroupMsg(changeBadgeOrEffectMsg);
    }
}

void CEntityDanceGroupComponent::OnDonateDanceGroupResource(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DonateDanceGroupResource &rMsg = dynamic_cast<GameMsg_C2S_DonateDanceGroupResource &>(msg);

    if (0 == rMsg.m_nResA && 
        0 == rMsg.m_nResB && 
        0 == rMsg.m_nResC)
    {
        WriteLog(LOGLEVEL_ERROR, "[[DANCE GROUP] donate resource: total resource is 0]");
        return;
    }
    if (m_nGroupID == 0)
    {
        WriteLog(LOGLEVEL_ERROR, "[DANCE GROUP] donate resource: not in dance group, roleid=%u",
            m_pRoleAttr->GetRoleID());
        return;
    }

    if (NULL != m_pRoleItem)
    {
        unsigned int nResA = m_pRoleItem->GetItemCount(cITEM_TYPE_RES_A, false, false);
        unsigned int nResB = m_pRoleItem->GetItemCount(cITEM_TYPE_RES_B, false, false);
        unsigned int nResC = m_pRoleItem->GetItemCount(cITEM_TYPE_RES_C, false, false);

        if (rMsg.m_nResA > nResA || rMsg.m_nResB > nResB || rMsg.m_nResC > nResC)
        {
            GameMsg_S2C_DonateDanceGroupResourceResult resMsg;
            resMsg.m_nResult = EDonateDanceGroupResourceResult_NotEnoughResource;
            SendPlayerMsg(resMsg);
            return;
        }

        m_pRoleItem->RemoveItemByType(cITEM_TYPE_RES_A, rMsg.m_nResA, EItemAction_Del_DonateDanceGroupRes);
        m_pRoleItem->RemoveItemByType(cITEM_TYPE_RES_B, rMsg.m_nResB, EItemAction_Del_DonateDanceGroupRes);
        m_pRoleItem->RemoveItemByType(cITEM_TYPE_RES_C, rMsg.m_nResC, EItemAction_Del_DonateDanceGroupRes);

        GameMsg_S2G_DonateDanceGroupResource donateMsg;
        donateMsg.m_nGroupID = m_nGroupID;
        donateMsg.m_nResA = rMsg.m_nResA;
        donateMsg.m_nResB = rMsg.m_nResB;
        donateMsg.m_nResC = rMsg.m_nResC;

        Send2GroupMsg(donateMsg);
    }
}

void CEntityDanceGroupComponent::OnUpgradeDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_UpgradeDanceGroupResult resMsg;
        resMsg.m_nResult = EUpgradeGroupDanceResult_NoInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_UpgradeDanceGroup upgradeMsg;
    upgradeMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(upgradeMsg);
}

void CEntityDanceGroupComponent::OnGetDanceGroupRecords(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_GetDanceGroupRecrodsResult resMsg;
        resMsg.m_nErrorCode = EDanceGroupRecrodsResult_NoInDG;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_GetDanceGroupRecrods getRecordsMsg;
    getRecordsMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(getRecordsMsg);
}

void CEntityDanceGroupComponent::OnExitDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_ExitDanceGroupResult resMsg;
        resMsg.m_nResult = EExitDanceGroupResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_ExitDanceGroup exitMsg;
    exitMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(exitMsg);
}

void CEntityDanceGroupComponent::OnKickOutDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_KickOutDanceGroupMember &rMsg = dynamic_cast<GameMsg_C2S_KickOutDanceGroupMember &>(msg);

    if (0 == m_nGroupID)
    {
        GameMsg_S2C_KickOutDanceGroupMemberResult resMsg;
        resMsg.m_nResult = EKickOutDanceGroupMemberResult_NotInGroup;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_KickOutDanceGroupMember kickMemberMsg;
    kickMemberMsg.m_nGroupID = m_nGroupID;
    kickMemberMsg.m_nTargetRoleID = rMsg.m_nTargetRoleID;

    Send2GroupMsg(kickMemberMsg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityGetStarInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_DanceGroupActivityGetStarInfoResult resMsg;
        resMsg.m_nErrorCode = EDanceGroupActivityStar_NoInDG;

        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_DanceGroupActivityGetStarInfo getStarMsg;
    getStarMsg.m_nGroupID = m_nGroupID;

    Send2GroupMsg(getStarMsg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPower(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DanceGroupActivityAddStarPower &rMsg = dynamic_cast<GameMsg_C2S_DanceGroupActivityAddStarPower &>(msg);

    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_DanceGroupActivityAddStarPowerResult resMsg;
        resMsg.m_nType = rMsg.m_nType;
        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupActivityAddStarPowerCond(rMsg.m_nType);

        if (EDanceGroupActivityStar_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_DanceGroupActivityAddStarPower addStarPowerMsg;
        addStarPowerMsg.m_nGroupID = m_nGroupID;
        addStarPowerMsg.m_nType = rMsg.m_nType;

        Send2GroupMsg(addStarPowerMsg);
    }
}

void CEntityDanceGroupComponent::OnDanceGroupActivityGetStarPowerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (0 == m_nGroupID)
    {
        GameMsg_S2C_DanceGroupActivityGetStarPowerInfoResult resMsg;
        resMsg.m_nErrorCode = EDanceGroupActivityStar_NoInDG;

        SendPlayerMsg(resMsg);
    }
    else
    {
        GameMsg_S2G_DanceGroupActivityGetStarPowerInfo getPowerInfoMsg;
        getPowerInfoMsg.m_nGroupID = m_nGroupID;

        Send2GroupMsg(getPowerInfoMsg);
    }
}

void CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPowerPoint2Money(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_DanceGroupActivityAddStarPowerResult resMsg;
        resMsg.m_nType = EDanceGroupActivityStarType_Point2Money;
        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupActivityAddStarPowerCond(resMsg.m_nType);

        if (EDanceGroupActivityStar_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_DanceGroupActivityAddStarPower addStarPowerMsg;
        addStarPowerMsg.m_nGroupID = m_nGroupID;
        addStarPowerMsg.m_nType = resMsg.m_nType;

        Send2GroupMsg(addStarPowerMsg);
    }
}

void CEntityDanceGroupComponent::OnDanceGroupActivityOpenStarPacket(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_DanceGroupActivityOpenStarPacket &rMsg = dynamic_cast<GameMsg_C2S_DanceGroupActivityOpenStarPacket &>(msg);

    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_DanceGroupActivityOpenStarPacketResult resMsg;
        resMsg.m_nStarLevel = rMsg.m_nStarLevel;
        resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupActivityStarCond();

        if (EDanceGroupActivityStar_Success != resMsg.m_nErrorCode)
        {
            SendPlayerMsg(resMsg);

            return;
        }

        GameMsg_S2G_DanceGroupActivityOpenStarPacket openPacketMsg;
        openPacketMsg.m_nGroupID = m_nGroupID;
        openPacketMsg.m_nStarLevel = rMsg.m_nStarLevel;

        Send2GroupMsg(openPacketMsg);
    }
}


void CEntityDanceGroupComponent::OnDanceGroupShopGetInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_DanceGroupShopGetInfoResult resMsg;

    std::vector<BigMamaExchange>& products = g_BigMamaConfigManager.GetExchangeList((ESexType)m_pRoleAttr->GetSex());

    resMsg.m_nErrorCode = 0;
    for (size_t i = 0; i < products.size(); ++i)
    {
        DanceGroupShopExchangeInfo prod;
        prod.m_nIndex = i;
        prod.m_ExchangeItem = CItem(products[i].m_nTargetItemID, (unsigned short)products[i].m_nTargetCount, products[i].m_nTargetMatune, false);
        if (0 != products[i].m_nM1ItemID)
            prod.m_ListRequestItem.push_back(CItem(products[i].m_nM1ItemID, (unsigned short)products[i].m_nM1Count, products[i].m_nM1Matune, false));
        if (0 != products[i].m_nM2ItemID)
            prod.m_ListRequestItem.push_back(CItem(products[i].m_nM2ItemID, (unsigned short)products[i].m_nM2Count, products[i].m_nM2Matune, false));
        if (0 != products[i].m_nM3ItemID)
            prod.m_ListRequestItem.push_back(CItem(products[i].m_nM3ItemID, (unsigned short)products[i].m_nM3Count, products[i].m_nM3Matune, false));
        if (products[i].m_nMoneyCount != 0)
        {
            prod.m_nRequestMoneyType = EMallCurrencyType_Money;
            prod.m_nRequestMoneyCount = products[i].m_nMoneyCount;
        }
        if (products[i].m_nBindMCoinCount != 0)
        {
            prod.m_nRequestMoneyType = EMallCurrencyType_BindMCoin;
            prod.m_nRequestMoneyCount = products[i].m_nBindMCoinCount;
        }
        if (products[i].m_nMCoinCount != 0)
        {
            prod.m_nRequestMoneyType = EMallCurrencyType_MCoin;
            prod.m_nRequestMoneyCount = products[i].m_nMCoinCount;
        }

        prod.m_nRequestContribution = products[i].m_nDanceGroupContibute;

        resMsg.m_vecExchange.push_back(prod);
    }


    SendPlayerMsg(resMsg); // 回复这个消息

    return;
}

void CEntityDanceGroupComponent::OnDanceGroupShopExchange(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    ENSURE_WITH_LOG_CMD(NULL != m_pRoleAttr, return, "role attr should not null in dance component");
    GameMsg_C2S_DanceGroupShopExchange &rMsg = dynamic_cast<GameMsg_C2S_DanceGroupShopExchange &>(msg);
    
    BigMamaExchange *pExchange = g_BigMamaConfigManager.GetExchange((ESexType)m_pRoleAttr->GetSex(), (unsigned char)rMsg.m_nIndex);
    ENSURE_WITH_LOG_CMD(pExchange != NULL, return, "role[%u] error to get DanceGroupShop item[%u]",
        m_pRoleAttr->GetRoleID(), rMsg.m_nIndex);

    GameMsg_S2C_DanceGroupShopExchangeResult resMsg;
    resMsg.m_nIndex = rMsg.m_nIndex;
    resMsg.m_nErrorCode = (unsigned short)CheckDanceGroupShopCond();

    do 
    {
        if (EDanceGroupShopResult_Success != resMsg.m_nErrorCode)
        {
            break;
        }

        ENSURE_WITH_LOG_CMD(NULL != m_pGroupInfo, return, "dance group info should not null");
        CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID());
        ENSURE_WITH_LOG_CMD(NULL != pMember, return, "dance group member should not null");

        if (pMember->m_nContribution < pExchange->m_nDanceGroupContibute)
        {
            resMsg.m_nErrorCode = EDanceGroupShopResult_NotEnoughContribution;
            break;
        }

        // 检测钱
        if (0 != pExchange->m_nMoneyCount || 0 != pExchange->m_nBindMCoinCount || 0 != pExchange->m_nMCoinCount)
        { // 如果有一种不为0
            BUILD_CURRENCY_MAP(currencyMap, pExchange->m_nMCoinCount, pExchange->m_nBindMCoinCount, pExchange->m_nMoneyCount);
            if (!m_pRoleAttr->CanBill(currencyMap))
            {
                resMsg.m_nErrorCode = EDanceGroupShopResult_MoneyNotEnough;
                break;
            }
        }

        // 检测物品：
        if ((pExchange->m_nM1Count != 0 && m_pRoleItem->GetItemCount(pExchange->m_nM1ItemID, false, false) < pExchange->m_nM1Count)
            || (pExchange->m_nM2Count != 0 && m_pRoleItem->GetItemCount(pExchange->m_nM2ItemID, false, false) < pExchange->m_nM2Count)
            || (pExchange->m_nM3Count != 0 && m_pRoleItem->GetItemCount(pExchange->m_nM3ItemID, false, false) < pExchange->m_nM3Count))
        {
            resMsg.m_nErrorCode = EDanceGroupShopResult_ItemNotEnough;
            break;
        }

        // 条件充足可以兑换：
        // 先扣除，然后在发送奖励
        if (pExchange->m_nDanceGroupContibute != 0)
        {
            GameMsg_S2G_ReduceDanceGroupContribution msg;
            msg.m_nGroupID = m_nGroupID;
            msg.m_nChangeValue = pExchange->m_nDanceGroupContibute;
            
            Send2GroupMsg(msg);
        }

        // 扣钱：
        if (0 != pExchange->m_nMoneyCount || 0 != pExchange->m_nBindMCoinCount || 0 != pExchange->m_nMCoinCount)
        { // 如果有一种不为0
            BUILD_CURRENCY_MAP(currencyMap, pExchange->m_nMCoinCount, pExchange->m_nBindMCoinCount, pExchange->m_nMoneyCount);
            m_pRoleAttr->Bill(currencyMap, EChangeBillCause_Del_DanceGroupShop, EChangeBindBillCause_Del_DanceGroupShop, EChangeMoneyCause_ApplyCeremonyRoom);
        }

        // 扣除物品：
        if (pExchange->m_nM1Count != 0)
        {
            m_pRoleItem->RemoveItemByType(pExchange->m_nM1ItemID, pExchange->m_nM1Count, EItemAction_Del_DanceGroupShop);
        }
        if (pExchange->m_nM2Count != 0)
        {
            m_pRoleItem->RemoveItemByType(pExchange->m_nM2ItemID, pExchange->m_nM2Count, EItemAction_Del_DanceGroupShop);
        }
        if (pExchange->m_nM2Count != 0)
        {
            m_pRoleItem->RemoveItemByType(pExchange->m_nM3ItemID, pExchange->m_nM3Count, EItemAction_Del_DanceGroupShop);
        }

        // 发放奖励:
        std::list<CItem> lstItem;
        lstItem.push_back(CItem(pExchange->m_nTargetItemID, (unsigned short)pExchange->m_nTargetCount, pExchange->m_nTargetMatune, true));
        CItemProcess::AddItems(*(CRoleEntity*)Entity(), lstItem, EItemAction_Add_DanceGroupShop, 0, true);

    } while (0);

    SendPlayerMsg(resMsg);
}

void CEntityDanceGroupComponent::OnInviteDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_InviteDanceGroupMember &rInviteMsg = dynamic_cast<GameMsg_C2S_InviteDanceGroupMember &>(msg);

    if (rInviteMsg.m_nInviteeRoleID != 0 && m_pRoleAttr != NULL)
    {
        GameMsg_S2G_CanBeInvitedCheck sendmsg;
        sendmsg.m_nInvitedRole = rInviteMsg.m_nInviteeRoleID;
        sendmsg.m_nToGroupID = m_nGroupID;
        Send2GroupMsg(sendmsg);
    }

    return ;
}

void CEntityDanceGroupComponent::OnCanBeInviteCheck(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_CanBeInvitedCheck &rmsg = (GameMsg_G2S_CanBeInvitedCheck &)msg;

    ENSURE_WITH_LOG_CMD(m_pRoleAttr != NULL, return, "rolt attr should not null");

    GameMsg_S2C_InviteDanceGroupMemberResult inviteResMsg;
    do 
    {
        if (rmsg.m_nRet != EInviteDanceGroupMemberResult_Success)
        {
            inviteResMsg.m_nResult = rmsg.m_nRet;
            break;
        }

        CRoleEntity *pInviteeRoleEntity = CPlayerManager::Instance().GetEntityByRoleID(rmsg.m_nInvitedRole);
        inviteResMsg.m_nResult = (unsigned short)CheckInviteDanceGroupMemberCond(m_pRoleAttr->GetRoleID(), pInviteeRoleEntity);

        if (inviteResMsg.m_nResult != EInviteDanceGroupMemberResult_Success)
            break;

        // 通知被邀请人
        GameMsg_S2C_BeInvitedAsDanceGroupMember beInvitedMsg;
        beInvitedMsg.m_strInviterRoleName = m_pRoleAttr->GetRoleName();
        beInvitedMsg.m_nDanceGroupID = m_nGroupID;
        beInvitedMsg.m_strDanceGroupName = m_pGroupInfo->m_DGBaseInfo.m_strGroupName;

        pInviteeRoleEntity->SendPlayerMsg(&beInvitedMsg);
    } while (0);

    SendPlayerMsg(inviteResMsg);

    return ;
}


void CEntityDanceGroupComponent::OnAcceptDanceGroupMemberInvitation(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_AcceptDanceGroupMemberInvitation &rAcceptMsg = dynamic_cast<GameMsg_C2S_AcceptDanceGroupMemberInvitation &>(msg);

    GameMsg_S2C_AcceptDanceGroupMemberInvitationResult resMsg;
    resMsg.m_nResult = EAcceptDanceGroupMemberInvitationResult_Success;

    if (m_nGroupID != 0)
        resMsg.m_nResult = EAcceptDanceGroupMemberInvitationResult_HasGroup;
    else if (rAcceptMsg.m_nDanceGroupID == 0)
        resMsg.m_nResult = EAcceptDanceGroupMemberInvitationResult_GroupNotExist;

    if (resMsg.m_nResult != EAcceptDanceGroupMemberInvitationResult_Success)
    {
        SendPlayerMsg(resMsg);

        return;
    }

    GameMsg_S2G_AcceptDanceGroupMemberInvitation acceptInvitationMsg;
    acceptInvitationMsg.m_nDanceGroupID = rAcceptMsg.m_nDanceGroupID;
    acceptInvitationMsg.m_strInviterRoleName = rAcceptMsg.m_strInviterRoleName;

    Send2GroupMsg(acceptInvitationMsg);
}

void CEntityDanceGroupComponent::OnSendSimpleDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnGetDanceGroupInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnSendDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnCancelDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnCancelRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnReplyRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnReplyRequestListEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_ChangeDanceGroupNameResult &rMsg = dynamic_cast<GameMsg_S2C_ChangeDanceGroupNameResult &>(msg);

    if (EChangeDanceGroupNameResult_Success == rMsg.m_nResult)
    {
        UpdateDanceGroupName(rMsg.m_strNewGroupName);

        if (NULL != m_pGroupInfo)
        {
            m_pGroupInfo->m_DGBaseInfo.m_strGroupName = rMsg.m_strNewGroupName;

            if (NULL != m_pRoleAttr && m_pRoleAttr->GetRoleID() == m_pGroupInfo->m_DGBaseInfo.m_nLeaderRoleID)
            {
                if (NULL != m_pRoleItem)
                    m_pRoleItem->RemoveItemByType(cITEM_CHANGE_DANCE_GOURP_NAME_CARD, 1, EItemAction_Del_ChangeDanceGroupName);
            }
        }
    }

    if (NULL != m_pRoleAttr && m_pRoleAttr->GetRoleID() == m_pGroupInfo->m_DGBaseInfo.m_nLeaderRoleID)
        SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupColorResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupProfileResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupAnnouncementResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupTitleResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupTitleNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupLeaderResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnUnlockDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnChangeDanceGroupBadgeOrEffectResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult &rMsg = dynamic_cast<GameMsg_G2S_ChangeDanceGroupBadgeOrEffectResult &>(msg);
    
    if (EDanceGroupBadgeResult_Success == rMsg.m_nErrorCode)
        UpdateDanceGroupBadgeOrEffect(rMsg.m_nBadge, rMsg.m_nEffect);

    if (NULL != m_pRoleAttr && m_pRoleAttr->GetRoleID() == rMsg.m_nRoleID)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew retMsg;
        retMsg.m_nErrorCode = rMsg.m_nErrorCode;
        retMsg.m_nBadge = rMsg.m_nBadge;
        retMsg.m_nRoleID = rMsg.m_nRoleID;
        retMsg.m_nEffect = rMsg.m_nEffect;
        SendPlayerMsg(msg);
    }
}

void CEntityDanceGroupComponent::OnUpgradeDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnGetDanceGroupRecrodsResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnExitDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_ExitDanceGroupResult &rMsg = dynamic_cast<GameMsg_S2C_ExitDanceGroupResult &>(msg);

    if (EExitDanceGroupResult_Success == rMsg.m_nResult)
        ClearDanceGroupInfo();

    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnKickedOutDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    ClearDanceGroupInfo();

    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnKickOutDanceGroupMemberResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityGetStarInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityAddStarPowerResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_DanceGroupActivityAddStarPowerResult &rMsg = dynamic_cast<GameMsg_S2C_DanceGroupActivityAddStarPowerResult &>(msg);

    if (EDanceGroupActivityStar_Success == rMsg.m_nErrorCode)
    {
        rMsg.m_bReward = ProcessFirstReward();
        int nValue = 0;

        if (NULL == m_pRoleAttr)
            return;

        switch (rMsg.m_nType)
        {
        case EDanceGroupActivityStarType_Money:
            {
                nValue = CDanceGroupDataMgr::Instance().m_nActivityStarMoneyNum;

                m_pRoleAttr->ChangeMoney(-abs(nValue), EChangeMoneyCause_Del_DGActivityStar, 0);
            }
            break;

        case EDanceGroupActivityStarType_Point:
            {
                nValue = CDanceGroupDataMgr::Instance().m_nActivityStarPointNum;

                m_pRoleAttr->ChangeBill(-abs(nValue), EChangeBillCause_Del_DGActivityStar);
            }
            break;

        case EDanceGroupActivityStarType_Point2Money:
            {
                nValue = m_pRoleAttr->GetMoney();

                m_pRoleAttr->ChangeMoney(-nValue, EChangeMoneyCause_Del_DGActivityStar, 0);

                nValue = CDanceGroupDataMgr::Instance().m_nActivityStarMoneyNum - nValue;

                if (nValue > 0)
                    m_pRoleAttr->ChangeBill(-nValue, EChangeBillCause_Del_DGActivityStar);
            }
        }
    }

    SendPlayerMsg(rMsg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityGetStarPowerInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnDanceGroupActivityOpenStarPacketResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_DanceGroupActivityOpenStarPacketResult &rMsg = dynamic_cast<GameMsg_S2C_DanceGroupActivityOpenStarPacketResult &>(msg);

    if (EDanceGroupActivityStar_Success == rMsg.m_nErrorCode)
    {
        rMsg.m_nErrorCode = EDanceGroupActivityStar_Failed;

        std::map<unsigned short, std::list<CItem> > listPacketItem;

        if (CDanceGroupDataMgr::Instance().GetPacketListByDay(listPacketItem, m_pRoleAttr->GetSex(), (unsigned short)GetWeekDay() ) )
        {
            std::map<unsigned short, std::list<CItem> >::const_iterator itr = listPacketItem.find(rMsg.m_nStarLevel);

            if (listPacketItem.end() != itr)
            {
                CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), itr->second, 
                    EItemAction_DanceGroup_Activity, 0, 
                    true, EMailType_DanceGroupActivity, "", "");

                rMsg.m_nErrorCode = EDanceGroupActivityStar_Success;
            }
        }
    }

    SendPlayerMsg(rMsg);
}
// 
// void CEntityDanceGroupComponent::OnDanceGroupShopExchangeResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
// {
//     GameMsg_S2C_DanceGroupShopExchangeResult &rMsg = dynamic_cast<GameMsg_S2C_DanceGroupShopExchangeResult &>(msg);
// 
//     if (EDanceGroupShopResult_Success == rMsg.m_nErrorCode)
//     {
//         const CDGShopInfo &rDayShopInfo = CDanceGroupDataMgr::Instance().GetDayShopInfo();
//         std::map<unsigned short, CDGShopItemInfo>::const_iterator itr = rDayShopInfo.m_dayItemList.find(rMsg.m_nIndex);
// 
//         if (rDayShopInfo.m_dayItemList.end() != itr)
//         {
//             if (NULL != m_pRoleAttr && NULL != m_pRoleItem)
//             {
//                 const CItem *pItem = &(itr->second.m_maleItem);
//                 if (ESexType_Male != m_pRoleAttr->GetSex())
//                     pItem = &(itr->second.m_femaleItem);
// 
//                 m_pRoleItem->AddItem(pItem->m_nItemType, pItem->m_nItemCount, pItem->m_nValidTime, 
//                     EItemAction_Add_DanceGroupShop, 0, true, false);
//             }
//         }
//     }
// 
//     SendPlayerMsg(rMsg);
// }

void CEntityDanceGroupComponent::OnDanceGroupRemoved(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    ClearDanceGroupInfo();

    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnDanceGroupBeRefuseRefresh(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnGetMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetMyDanceGroupInfo &rGetInfoMsg = dynamic_cast<GameMsg_G2S_GetMyDanceGroupInfo &>(msg);

    if (NULL != rGetInfoMsg.m_pDGInfo)
    {
        SetDanceGroupInfo(rGetInfoMsg.m_pDGInfo);

        if (NULL != m_pRoleQuestNew)
            m_pRoleQuestNew->OnDanceGroupChange();

        UpdateMemberInfo(true);
    }
    else
    {
        WriteLog(LOGLEVEL_ERROR, "Dance group info is not load completed. Role id = %d, group id = %d.", 
            m_pRoleAttr->GetRoleID(), m_nGroupID);
    }

    PushDanceGroupInfoOnLogin(NULL != m_pGroupInfo, rGetInfoMsg.m_nClickedPlace);
}

void CEntityDanceGroupComponent::OnCreateDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_CreateDanceGroupResult &rMsg = dynamic_cast<GameMsg_G2S_CreateDanceGroupResult &>(msg);

    GameMsg_S2C_CreateDanceGroupResult resMsg;
    resMsg.m_nResult = rMsg.m_nResult;
    resMsg.m_nCreateCD = rMsg.m_nCreateCD;

    if (ECreateDanceGroupResult_Success == rMsg.m_nResult)
    {
        SetDanceGroupInfo(rMsg.m_pDGInfo);

        UpdateDanceGroupInfo();
        SyncChallengeConfig();

        if (NULL != m_pRoleItem)
            m_pRoleItem->RemoveItemByType(cITEM_TYPE_FOR_CREATE_DANCE_GROUP, 1, EItemAction_Del_CreateDanceGroup);

        if (m_pRoleAttr != NULL && m_pGroupInfo != NULL)
        {
            CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID() );

            if (pMember != NULL)
            {
                UpdateDanceGroupName(m_pGroupInfo->m_DGBaseInfo.m_strGroupName);
                UpdateDanceGroupTitle(pMember->m_nTitle);
                UpdateDanceGroupBadgeOrEffect(m_pGroupInfo->m_DGBaseInfo.m_nBadge, m_pGroupInfo->m_DGBaseInfo.m_nEffect);
            }
        }
    }

    SendPlayerMsg(resMsg);
}

void CEntityDanceGroupComponent::OnAddRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_AddRequestDanceGroupInfo &rMsg = dynamic_cast<GameMsg_G2S_AddRequestDanceGroupInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    GameMsg_S2C_NotifyRequestEnterDanceGroup notifyMsg;
    notifyMsg.m_RequestInfo = rMsg.m_RequestInfo;

    SendPlayerMsg(notifyMsg);
}

void CEntityDanceGroupComponent::OnRemoveRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_RemoveRequestDanceGroupInfo &rMsg = dynamic_cast<GameMsg_G2S_RemoveRequestDanceGroupInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    GameMsg_S2C_NotifyRemoveRequestEnter notifyMsg;
    notifyMsg.m_nRoleID = rMsg.m_nRoleID;

    SendPlayerMsg(notifyMsg);
}

void CEntityDanceGroupComponent::OnAddDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_AddDanceGroupMemberInfo &rMsg = dynamic_cast<GameMsg_G2S_AddDanceGroupMemberInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    if (NULL != m_pGroupInfo)
        m_pGroupInfo->AddMemberInfo(rMsg.m_MemberInfo);

    GameMsg_S2C_AddDanceGroupMember addMemberMsg;
    addMemberMsg.m_DGMember = rMsg.m_MemberInfo;

    SendPlayerMsg(addMemberMsg);
}

void CEntityDanceGroupComponent::OnEnterDanceGroupSuccess(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_EnterDanceGroupSuccess &rMsg = dynamic_cast<GameMsg_G2S_EnterDanceGroupSuccess &>(msg);

    if (NULL == rMsg.m_pDGInfo)
    {
        LOG_ERROR << "Dance group info is NULL in msg" << std::endl;

        return;
    }

    SetDanceGroupInfo(rMsg.m_pDGInfo);

    if (NULL == m_pGroupInfo)
        return;

    GameMsg_S2C_EnterDanceGroupSuccess resMsg;
    resMsg.m_nGroupID = rMsg.m_pDGInfo->m_DGBaseInfo.m_nGroupID;

    SendPlayerMsg(resMsg);

    UpdateDanceGroupInfo();
    UpdateMemberInfo(true);
    UpdateDanceGroupName(m_pGroupInfo->m_DGBaseInfo.m_strGroupName);
    SyncChallengeConfig();

    if (NULL != m_pRoleAttr)
    {
        CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID());

        if (NULL != pMember)
        {
            UpdateDanceGroupTitle(pMember->m_nTitle);
            UpdateDanceGroupBadgeOrEffect(m_pGroupInfo->m_DGBaseInfo.m_nBadge, m_pGroupInfo->m_DGBaseInfo.m_nEffect);
        }
    }

    if (NULL != m_pRoleQuest)
        m_pRoleQuest->PushFresherQuestStateChange();
    if (NULL != m_pRoleQuestNew)
        m_pRoleQuestNew->OnDanceGroupChange();
}

void CEntityDanceGroupComponent::OnUpdateDanceGroupBaseInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateDanceGroupBaseInfo &rMsg = dynamic_cast<GameMsg_G2S_UpdateDanceGroupBaseInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_DGBaseInfo.m_nGroupID))
        return;

    if (NULL != m_pGroupInfo)
        m_pGroupInfo->UpdateBaseInfo(rMsg.m_DGBaseInfo);

    switch (rMsg.m_nUpdateType)
    {
    case EDanceGroupBaseInfoUpdateType_Data:
        {
            GameMsg_S2C_UpdateDanceGroupBaseInfo resMsg;
            resMsg.m_DGBaseInfo = rMsg.m_DGBaseInfo;

            SendPlayerMsg(resMsg);
        }
        break;

    case EDanceGroupBaseInfoUpdateType_Announcement:
        {
            GameMsg_S2C_UpdateDanceGroupAnnounce resMsg;
            resMsg.m_strNewAnnounce = rMsg.m_DGBaseInfo.m_strAnnouncement;

            SendPlayerMsg(resMsg);
        }
        break;

    case EDanceGroupBaseInfoUpdateType_Profile:
        {
            GameMsg_S2C_UpdateDanceGroupProfile resMsg;
            resMsg.m_strNewProfile = rMsg.m_DGBaseInfo.m_strProfile;

            SendPlayerMsg(resMsg);
        }
        break;
    case EDanceGroupBaseInfoUpdateType_CermonyTimes:
        {
            GameMsg_S2C_SyncCermonyApplyTimes resResMsg;
            resResMsg.m_nUsedTimes = (unsigned char)rMsg.m_DGBaseInfo.m_nCeremonyTimes; //

            SendPlayerMsg(resResMsg);
        }
        break;
    default:
        LOG_ERROR << "Update dance group info type(" 
                  << rMsg.m_nUpdateType 
                  << ") is wrong." << std::endl;
        break;
    }
}

void CEntityDanceGroupComponent::OnUpdateDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateDanceGroupMemberInfo &rMsg = dynamic_cast<GameMsg_G2S_UpdateDanceGroupMemberInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    char nTitle = -1;
    unsigned int nRoleID = 0;

    if (NULL != m_pGroupInfo)
    {
        if (NULL != m_pRoleAttr)
        {
            nRoleID = m_pRoleAttr->GetRoleID();
            CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(nRoleID);

            if (NULL != pMember)
                nTitle = pMember->m_nTitle;
        }

        m_pGroupInfo->UpdateMemberInfo(rMsg.m_MemberInfo);
    }

    GameMsg_S2C_UpdateDanceGroupMemberInfo updateMemberInfo;
    updateMemberInfo.m_DGMember = rMsg.m_MemberInfo;

    SendPlayerMsg(updateMemberInfo);

    if (nRoleID == rMsg.m_MemberInfo.m_nRoleID && 
        nTitle != rMsg.m_MemberInfo.m_nTitle)
    {
        UpdateDanceGroupTitle(rMsg.m_MemberInfo.m_nTitle);
    }
}

void CEntityDanceGroupComponent::OnDonateDanceGroupResourceResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_DonateDanceGroupResourceResult &rMsg = dynamic_cast<GameMsg_G2S_DonateDanceGroupResourceResult &>(msg);

    GameMsg_S2C_DonateDanceGroupResourceResult resMsg;
    resMsg.m_nResult = rMsg.m_nResult;

    SendPlayerMsg(resMsg);
}

void CEntityDanceGroupComponent::OnRemoveDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_RemoveDanceGroupMemberInfo &rMsg = dynamic_cast<GameMsg_G2S_RemoveDanceGroupMemberInfo &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    if (NULL != m_pGroupInfo)
        m_pGroupInfo->RemoveMemberInfo(rMsg.m_nRoleID);

    GameMsg_S2C_RemoveDanceGroupMember removeMemberMsg;
    removeMemberMsg.m_nRoleID = rMsg.m_nRoleID;

    SendPlayerMsg(removeMemberMsg);
}

void CEntityDanceGroupComponent::OnUpdateDanceGroupActivityStarState(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateDanceGroupActivityStarState &rMsg = dynamic_cast<GameMsg_G2S_UpdateDanceGroupActivityStarState &>(msg);

    if (!IsDanceGroupIDSame(rMsg.m_nGroupID))
        return;

    if (NULL != m_pGroupInfo)
    {
        m_pGroupInfo->m_DGActivityInfo.m_nStarLevel = rMsg.m_nStarLevel;
        m_pGroupInfo->m_DGActivityInfo.m_nCurrentPowerValue = rMsg.m_nCurrentPower;
    }

    GameMsg_S2C_NotifyDanceGroupActivityStarState notifyMsg;
    notifyMsg.m_nOldStarLevel = rMsg.m_nStarLevel - 1;
    notifyMsg.m_nStarLevel = rMsg.m_nStarLevel;
    notifyMsg.m_nCurrentPower = rMsg.m_nCurrentPower;

    SendPlayerMsg(notifyMsg);
}

// void CEntityDanceGroupComponent::OnDanceGroupShopGetInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
// {
//     GameMsg_G2S_DanceGroupShopGetInfoResult &rMsg = dynamic_cast<GameMsg_G2S_DanceGroupShopGetInfoResult &>(msg);
// 
//     if (NULL != m_pRoleAttr)
//     {
//         GameMsg_S2C_DanceGroupShopGetInfoResult resMsg;
//         resMsg.m_nErrorCode = rMsg.m_nErrorCode;
//         resMsg.m_mapUseCount = rMsg.m_mapUseCount;
//         resMsg.m_nSex = m_pRoleAttr->GetSex();
// 
//         if (EDanceGroupShopResult_Success == resMsg.m_nErrorCode)
//             resMsg.m_cShopInfo = CDanceGroupDataMgr::Instance().GetDayShopInfo();
// 
//         SendPlayerMsg(resMsg);
//     }
// }

void CEntityDanceGroupComponent::OnAcceptDanceGroupMemberInvitationResult(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_AcceptDanceGroupMemberInvitationResult &rMsg = dynamic_cast<GameMsg_G2S_AcceptDanceGroupMemberInvitationResult &>(msg);
    GameMsg_S2C_AcceptDanceGroupMemberInvitationResult acceptResMsg;
    acceptResMsg.m_nResult = rMsg.m_nResult;

    SendPlayerMsg(acceptResMsg);
}

void CEntityDanceGroupComponent::OnUpdateChallengeActive(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_UpdateDanceGroupChallengeActive &rMsg = (GameMsg_G2S_UpdateDanceGroupChallengeActive &)msg;

    if ( !IsDanceGroupIDSame( rMsg.m_nGroupID ) )
        return;

    if ( m_pGroupInfo != NULL )
    {
        m_pGroupInfo->m_DGBaseInfo.m_nChallengeActive = rMsg.m_nActive;

        UpdateDanceGroupInfo();
    }
}

void CEntityDanceGroupComponent::OnRequestApplyCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
//	GameMsg_C2S_RequestApplyCeremony &rmsg = (GameMsg_C2S_RequestApplyCeremony &)msg;
    if ( !CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_DanceGroupCeremony) )
    {
        return;
    }

	CeremonyRoomManager::Instance().TryOpenCeremonyRoom(m_pRoleAttr->GetRoleID()); // 试图创建房间
	
	return ; 
}

void CEntityDanceGroupComponent::OnRequestEnterCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
//	GameMsg_C2S_RequestEnterCeremony &rmsg = (GameMsg_C2S_RequestEnterCeremony &)msg;

	GameMsg_S2C_EnterCeremonyRes enterResMsg;
	enterResMsg.m_nErrorType = ECeremonyErrType_OK;
	
	GameMsg_S2C_OtherPlayerEnterCeremony toOtherMsg;

	do 
	{
        CRoleEntity *pRoleEnt = (CRoleEntity*)Entity();
        if (pRoleEnt->InSomeRoom())
        {
            enterResMsg.m_nErrorType = ECeremonyErrType_InOtherRoom;
            break;
        }

		CeremonyRoom* pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); // 取得房间
		
		if (NULL == pRoom)
		{
			enterResMsg.m_nErrorType = ECeremonyErrType_NotOpen;
			break;
		}
		
        CPlayerPosition pos;
        pos.Reset();
        pos.m_ESceneID = ESceneMove_DanceGroupCeremonyRoom;

		int ret = pRoom->EnterRoom(m_pRoleAttr->GetRoleID(), enterResMsg.m_CeremonyRoomInfo, toOtherMsg.m_PlayerInfo); // 进来了。
		if (ECeremonyErrType_OK != ret)
		{
			enterResMsg.m_nErrorType = (unsigned char)ret;
			break;
		}

		pRoom->RoomBroadcastExcept(m_pRoleAttr->GetRoleID(), toOtherMsg); // 广播给房间的其他人
	} while (0);

	SendPlayerMsg(enterResMsg);
	
	return;
}

void CEntityDanceGroupComponent::OnRequestQuitCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
//	GameMsg_C2S_RequestQuitCeremony &rmsg = (GameMsg_C2S_RequestQuitCeremony &)msg;
    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); //
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "error to get role[%u], danceGroup[%u] room", m_pRoleAttr->GetRoleID(), m_nGroupID);

    GameMsg_C2S_RequestQuitCeremony & requestmsg = (GameMsg_C2S_RequestQuitCeremony&)msg;

    GameMsg_S2C_QuitCeremonyRes retMsg; // 告知所有人退出的消息
    retMsg.m_nErrorType = (unsigned char)pRoom->LeaveRoom(m_pRoleAttr->GetRoleID());
    retMsg.m_nQuitType = ECeremonyRoomQuitType_Quit;
    retMsg.m_nKickerID = 0; // 没人踢
    retMsg.m_nQuitTo = requestmsg.m_nQuitTo;
    
    if (ECeremonyErrType_OK == retMsg.m_nErrorType)
    {
        GameMsg_S2C_OtherPlayerQuitCeremony bmsg;
        bmsg.m_nPlayerID = m_pRoleAttr->GetRoleID();
        pRoom->RoomBroadcast(bmsg); // 退出广播
    }
    
    SendPlayerMsg(retMsg); //

    return ;
}

void CEntityDanceGroupComponent::OnRequestRefreshCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
//   GameMsg_C2S_RequestRefreshCeremony &rmsg = (GameMsg_C2S_RequestRefreshCeremony &)msg;

    GameMsg_S2C_RefreshCeremonyRes enterResMsg;
    enterResMsg.m_nErrorType = ECeremonyErrType_OK;

    GameMsg_S2C_OtherPlayerEnterCeremony toOtherMsg;

    do 
    {
        CRoleEntity *pRoleEnt = (CRoleEntity*)Entity();
        if (pRoleEnt->InSomeRoom())
        {
            enterResMsg.m_nErrorType = ECeremonyErrType_InOtherRoom;
            break;
        }

        CeremonyRoom* pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); // 取得房间

        if (NULL == pRoom)
        {
            enterResMsg.m_nErrorType = ECeremonyErrType_NotOpen;
            break;
        }


        int ret = pRoom->EnterRoom(m_pRoleAttr->GetRoleID(), enterResMsg.m_CeremonyRoomInfo, toOtherMsg.m_PlayerInfo); // 进来了。
        if (ECeremonyErrType_OK != ret)
        {
            enterResMsg.m_nErrorType = (unsigned char)ret;
            break;
        }

        pRoom->RoomBroadcastExcept(m_pRoleAttr->GetRoleID(), toOtherMsg); // 广播给房间的其他人
    } while (0);

    SendPlayerMsg(enterResMsg);

    return ;
}

void CEntityDanceGroupComponent::OnRequestStartCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
//    GameMsg_C2S_RequestStartCeremony &rmsg = (GameMsg_C2S_RequestStartCeremony &)msg;

    GameMsg_S2C_StartCeremonyRes retMsg; // 成功则广播所有人，失败则只回发送者
    retMsg.m_nErrorType = ECeremonyErrType_OK;
    CeremonyRoom* pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); // 取得房间

    do 
    {
        if (NULL == pRoom)
        {
            retMsg.m_nErrorType = ECeremonyErrType_NotOpen;
            break;
        }

        if (pRoom->GetCreator() != m_pRoleAttr->GetRoleID())
        {
            retMsg.m_nErrorType = ECeremonyErrType_RightNotEnough;
            break;
        }

        retMsg.m_nErrorType = (unsigned char)pRoom->Play(retMsg.m_vecRookieInfos); // 举行仪式

    } while (0);

    if (ECeremonyErrType_OK == retMsg.m_nErrorType)
    {
        pRoom->RoomBroadcast(retMsg);
    }
    else
    {
        SendPlayerMsg(retMsg); // 
    }

    return ;
}

void CEntityDanceGroupComponent::RookieAnyBody(unsigned int nRookieRole, int seq)
{
    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID);

    unsigned char ret = 0;
    do 
    {
        if (NULL == pRoom)
        {
            ret = ECeremonyErrType_NotOpen;
            break;
        }

        ret = (unsigned char)pRoom->SetNewMember(nRookieRole, seq, false); //设置顺序
    } while (0);

    if (ECeremonyErrType_OK == ret)
    {
        GameMsg_S2C_SyncCeremonyRookie brodMsg;
        brodMsg.m_nRoleId = nRookieRole;
        brodMsg.m_bIsRookie = true; //
        pRoom->GetRoomPlayerShowInfo(nRookieRole, brodMsg.m_PlayerInfo);
        pRoom->RoomBroadcast(brodMsg); // 广播所有人新人同步消息
    }
}

void CEntityDanceGroupComponent::OnSetCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_SetCeremonyRookie &rmsg = (GameMsg_C2S_SetCeremonyRookie &)msg;

    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID);
    GameMsg_S2C_SetCeremonyRookieRes retMsg;
    retMsg.m_nErrorType = ECeremonyErrType_OK;

    do 
    {
        if (NULL == pRoom)
        {
            retMsg.m_nErrorType = ECeremonyErrType_NotOpen;
            break;
        }

        if (pRoom->GetCreator() != m_pRoleAttr->GetRoleID())
        {
            retMsg.m_nErrorType = ECeremonyErrType_RightNotEnough;
            break;
        }

        retMsg.m_nErrorType = (unsigned char)pRoom->SetNewMember(rmsg.m_nRoleId, rmsg.m_nRookieIndex, true); //设置顺序
    } while (0);


    if (ECeremonyErrType_OK == retMsg.m_nErrorType)
    {
        GameMsg_S2C_SyncCeremonyRookie brodMsg;
        brodMsg.m_nRoleId = rmsg.m_nRoleId;
        brodMsg.m_bIsRookie = true; //
        pRoom->GetRoomPlayerShowInfo(rmsg.m_nRoleId, brodMsg.m_PlayerInfo);
        pRoom->RoomBroadcast(brodMsg); // 广播所有人新人同步消息
    }
    SendPlayerMsg(retMsg);

    return ;
}

void CEntityDanceGroupComponent::OnCancelCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_CancelCeremonyRookie &rmsg = (GameMsg_C2S_CancelCeremonyRookie &)msg;

    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID);
    GameMsg_S2C_CancelCeremonyRookieRes retMsg;
    retMsg.m_nErrorType = ECeremonyErrType_OK;

    do 
    {
        if (NULL == pRoom)
        {
            retMsg.m_nErrorType = ECeremonyErrType_NotOpen;
            break;
        }

        if (pRoom->GetCreator() != m_pRoleAttr->GetRoleID())
        {
            retMsg.m_nErrorType = ECeremonyErrType_RightNotEnough;
            break;
        }

        retMsg.m_nErrorType = (unsigned char)pRoom->CancelNewMember(rmsg.m_nRoleId); //设置顺序
    } while (0);

    if (ECeremonyErrType_OK == retMsg.m_nErrorType)
    {
        GameMsg_S2C_SyncCeremonyRookie brodMsg;
        brodMsg.m_nRoleId = rmsg.m_nRoleId;
        brodMsg.m_bIsRookie = false; //
        pRoom->RoomBroadcast(brodMsg); // 广播所有人新人同步消息
    }
    SendPlayerMsg(retMsg);

    return;
}

void CEntityDanceGroupComponent::OnKickCeremonyPlayer(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_KickCeremonyPlayer &rmsg = (GameMsg_C2S_KickCeremonyPlayer &)msg;
    
    GameMsg_S2C_KickCeremonyPlayerRes retCreator;
    retCreator.m_nErrorType = ECeremonyErrType_OK;
    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID);

    do 
    {
        if (NULL == pRoom)
        {
            retCreator.m_nErrorType = ECeremonyErrType_NotOpen;
            break;
        }

        if (pRoom->GetCreator() != m_pRoleAttr->GetRoleID())
        {
            retCreator.m_nErrorType = ECeremonyErrType_RightNotEnough; //
            break;
        }

        retCreator.m_nErrorType = (unsigned char)pRoom->KickPerson(rmsg.m_nRoleId);
    } while (0);
    
    if (ECeremonyErrType_OK == retCreator.m_nErrorType)
    {
        GameMsg_S2C_OtherPlayerQuitCeremony brodcastQuit;
        brodcastQuit.m_nPlayerID = rmsg.m_nRoleId;
        pRoom->RoomBroadcast(brodcastQuit);

        // 通知被踢的人
        GameMsg_S2C_QuitCeremonyRes kickedNotify; // 
        kickedNotify.m_nErrorType = ECeremonyErrType_OK;
        kickedNotify.m_nQuitType = ECeremonyRoomQuitType_Kicked;
        kickedNotify.m_nKickerID = m_pRoleAttr->GetRoleID(); // 
        kickedNotify.m_nQuitTo = (char)ECeremonyRoomQuitTo_World;

        CRoleEntity *pRole = CPlayerManager::Instance().GetEntityByRoleID(rmsg.m_nRoleId);
        if (NULL != pRole)
            pRole->SendPlayerMsg(&kickedNotify);
    }

    // 回复创建者
    SendPlayerMsg(retCreator); // 踢人者
}

void CEntityDanceGroupComponent::OnSaveCeremonyIntruduction(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_SaveCeremonyIntruduction &rmsg = (GameMsg_C2S_SaveCeremonyIntruduction &)msg;

    CeremonyRoom *pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); //
    ENSURE_WITH_LOG_CMD(NULL != pRoom, return, "role[%u] dancegroup[%u] miss ceremony room of", m_pRoleAttr->GetRoleID(), m_nGroupID);

    GameMsg_S2C_SaveCeremonyIntruductionRes retMsg;
    retMsg.m_nErrorType = (unsigned char)pRoom->SaveIntroduce(m_pRoleAttr->GetRoleID(), rmsg.m_strIntruduction); // 设置介绍
    
    SendPlayerMsg(retMsg);

    return ;
}

void CEntityDanceGroupComponent::OnRequestCeremonyRecord(GameMsg_Base &msg, CSlotPeer &slotPeer)
{ // 日志请求：
    Send2GroupMsg(msg);
}

void CEntityDanceGroupComponent::OnRequestCeremonyPraise(GameMsg_Base &msg, CSlotPeer &slotPeer)
{ // 请求点赞信息
    Send2GroupMsg(msg);
}

void CEntityDanceGroupComponent::OnRequestPraiseRookie(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestPraiseRookie &rmsg = (GameMsg_C2S_RequestPraiseRookie &)msg;
    CeremonyRoom* pRoom = CeremonyRoomManager::Instance().GetCeremonyRoom(m_nGroupID); // 取得房间

    GameMsg_S2C_RequestPraiseRookieRes retMsg;
    retMsg.m_nErrorType = ECeremonyErrType_OK; // 
    // 被投票的人，投票之后的赞的数量
    int praiseNewCount = 0;

    do 
    {
        if (time(NULL) - m_nCeremonyRoomLastPraise < CDanceGroupDataMgr::Instance().m_nCeremonyPraiseCD)
        {
            retMsg.m_nErrorType = ECeremonyErrType_PraiseCD;
            break; 
        }
        m_nCeremonyRoomLastPraise = time(NULL);

        if (NULL == pRoom)
        {
            retMsg.m_nErrorType = ECeremonyErrType_NotOpen; // 房间不在
            break;
        }

        retMsg.m_nErrorType = (unsigned char)pRoom->APraiseB(m_pRoleAttr->GetRoleID(), rmsg.m_nRoleID, 
            (unsigned char)rmsg.m_nRookieIndex, rmsg.m_nCount, praiseNewCount);
    } while (0);

    SendPlayerMsg(retMsg);
    if (ECeremonyErrType_OK == retMsg.m_nErrorType)
    { // 广播新人获得赞的数量
        GameMsg_S2C_SyncPraiseInfo bmsg;
        bmsg.m_praiseNum = praiseNewCount;
        bmsg.m_roleID = rmsg.m_nRoleID; // 新人啊

        pRoom->RoomBroadcast(bmsg); // 广播新人获得的赞的数量
    }

}

void CEntityDanceGroupComponent::OnCeremonyRoomStatics(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_CeremonyRoomStatics &rmsg = (GameMsg_G2S_CeremonyRoomStatics &)msg;

    CRoleChat *pChat = GetComponent<CRoleChat>();
    if (NULL != pChat)
    {
        pChat->SysChat(rmsg.m_strStatics.c_str());
    }

    return ;
}

void CEntityDanceGroupComponent::OnRequestEnterChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_RequestEnterChallengeRes resMsg;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        resMsg.m_nOpenTime = mgr.GetOpenTime( m_nChallengeStartTime );

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EEnterChallengeRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EEnterChallengeRes_NotInGroup;
            break;
        }
        else if ( m_pGroupInfo == NULL )
        {
            resMsg.m_nResult = EEnterChallengeRes_Initializing;
            break;
        }
        else if ( m_pGroupInfo->m_DGBaseInfo.m_nLevel < mgr.GetOpenLevel() )
        {
            resMsg.m_nResult = EEnterChallengeRes_NotEnoughGroupLevel;
            break;
        }

        const CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember( m_pRoleAttr->GetRoleID() );

        if ( pMember == NULL )
        {
            resMsg.m_nResult = EEnterChallengeRes_NotInGroup;
            break;
        }
        else if ( IsTimeIsToday( pMember->m_nInTime ) )
        {
            resMsg.m_nResult = EEnterChallengeRes_NewMember;
            break;
        }

        GameMsg_S2G_EnterChallenge s2gMsg;
        s2gMsg.m_nGroupID = m_nGroupID;
        Send2GroupMsg( s2gMsg );
        return;
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnGetCurrentStoreyRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_EnterChallengeRes &rMsg = (GameMsg_G2S_EnterChallengeRes &)msg;
    GameMsg_S2C_RequestEnterChallengeRes resMsg;
    resMsg.m_nResult = rMsg.m_nResult;

    if ( resMsg.m_nResult == EEnterChallengeRes_Success )
    {
        m_nChallengeCurStorey = rMsg.m_nCurStorey;
        m_nChallengeCurStoreyState = rMsg.m_nCurStoreyState;
        m_nChallengeMyInspire = rMsg.m_nMyInspire;

        if ( DanceGroupChallengeManager::Instance().GetTotalStorey() < m_nChallengeCurStorey )
        {
            m_nChallengeCurStorey = DanceGroupChallengeManager::Instance().GetTotalStorey();
            m_nChallengeCurStoreyState = EDanceGroupChallengeStoreyState_Finished;
        }

        Currency nBuyChanceCost;
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        mgr.GetNextBuyChanceInfo( m_nChallengeBuyChance, nBuyChanceCost );

        resMsg.m_nOpenTime = mgr.GetOpenTime( m_nChallengeStartTime );
        resMsg.m_nCurrentStorey = m_nChallengeCurStorey;
        resMsg.m_nChanceCostType = nBuyChanceCost.GetType();
        resMsg.m_nChanceCostAmount = nBuyChanceCost.GetAmount();
    }

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestChallengeStoreyList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestChallengeStoreyList &rMsg = (GameMsg_C2S_RequestChallengeStoreyList &)msg;
    const std::map<int, DanceGroupChallengeStoreyInfo>& rStorey = DanceGroupChallengeManager::Instance().GetStoreyInfo();
    int nStartPos = rMsg.m_nRequestPage * rMsg.m_nRequestCount;

    GameMsg_S2C_RequestChallengeStoreyListRes resMsg;
    resMsg.m_nCurPage = rMsg.m_nRequestPage;
    resMsg.m_nTotalCount = rStorey.size();

    if ( nStartPos >= resMsg.m_nTotalCount )
    {
        resMsg.m_nCurPage = 0;
        nStartPos = 0;
    }

    std::map<int, DanceGroupChallengeStoreyInfo>::const_iterator itr = rStorey.begin();

    if ( nStartPos > 0 )
        std::advance( itr, nStartPos );

    for ( int nCount = 0; itr != rStorey.end() && nCount < rMsg.m_nRequestCount; ++nCount, ++itr )
    {
        DanceGroupChallengeStoreySimpleInfo simple;
        simple.m_nStorey = itr->second.GetStorey();
        simple.m_strAtlas = itr->second.GetAtlas();
        simple.m_strIcon = itr->second.GetIcon();

        if ( simple.m_nStorey < m_nChallengeCurStorey )
        {
            simple.m_nStoreyState = EDanceGroupChallengeStoreyState_Finished;
        }
        else if ( simple.m_nStorey > m_nChallengeCurStorey )
        {
            simple.m_nStoreyState = EDanceGroupChallengeStoreyState_NotStart;
        }
        else
        {
            simple.m_nStoreyState = m_nChallengeCurStoreyState;
        }

        resMsg.m_listInfo.push_back( simple );
    }

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestChallengeStoreyInfo(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestChallengeStoreyInfo &rMsg = (GameMsg_C2S_RequestChallengeStoreyInfo &)msg;
    const DanceGroupChallengeStoreyInfo *pStoreyInfo = DanceGroupChallengeManager::Instance().GetStoreyInfo( rMsg.m_nRequestStorey );

    if ( pStoreyInfo == NULL || m_nGroupID == 0 )
    {
        GameMsg_S2C_RequestChallengeStoreyInfoRes resMsg;
        resMsg.m_wholeInfo.m_nStorey = rMsg.m_nRequestStorey;
        SendPlayerMsg( resMsg );
        return;
    }

    GameMsg_S2G_GetChallengeStoreyInfo s2gMsg;
    s2gMsg.m_nGroupID = m_nGroupID;
    s2gMsg.m_nRequestStorey = rMsg.m_nRequestStorey;
    Send2GroupMsg( s2gMsg );
}

void CEntityDanceGroupComponent::OnGetChallengeStoreyInfoRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetChallengeStoreyInfoRes &rMsg = (GameMsg_G2S_GetChallengeStoreyInfoRes &)msg;
    DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
    const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nStorey );

    GameMsg_S2C_RequestChallengeStoreyInfoRes resMsg;
    resMsg.m_wholeInfo.m_nStorey = rMsg.m_nStorey;
    resMsg.m_wholeInfo.m_nStoreyState = rMsg.m_nStoreyState;
    resMsg.m_wholeInfo.m_nMaxChance = m_nChallengeTotalChance;
    resMsg.m_wholeInfo.m_nCurChance = m_nChallengeChance;

    if ( m_pRoleAttr != NULL )
    {
        mgr.GetAllRewardInfos( rMsg.m_nStorey, m_pRoleAttr->GetSex() == ESexType_Male, 
            resMsg.m_wholeInfo.m_listReward );
    }

    if ( pStoreyInfo != NULL )
    {
        resMsg.m_wholeInfo.m_nActive = pStoreyInfo->GetActiveValue();
        resMsg.m_wholeInfo.m_nTargetType = pStoreyInfo->GetTargetType();

        if ( rMsg.m_nStoreyState == EDanceGroupChallengeStoreyState_NotStart )
        {
            resMsg.m_wholeInfo.m_nMusic = (unsigned short)pStoreyInfo->GetMusic();
            resMsg.m_wholeInfo.m_nMusicMode = (unsigned char)pStoreyInfo->GetMusicMode();
            resMsg.m_wholeInfo.m_nMusicLevel = (unsigned char)pStoreyInfo->GetMusicLevel();
            resMsg.m_wholeInfo.m_nTargetScore = pStoreyInfo->GetTargetScore();
        }
        else
        {
            resMsg.m_wholeInfo.m_nMusic = (unsigned short)rMsg.m_nMusic;
            resMsg.m_wholeInfo.m_nMusicMode = (unsigned char)rMsg.m_nMusicMode;
            resMsg.m_wholeInfo.m_nMusicLevel = (unsigned char)rMsg.m_nMusicLevel;
            resMsg.m_wholeInfo.m_nTargetScore = rMsg.m_nTargetScore;
            resMsg.m_wholeInfo.m_nCurrentScore = rMsg.m_nCurrentScore;
            resMsg.m_wholeInfo.m_nAdditional = rMsg.m_nAdditional;

            if ( m_nChallengeCurStorey == rMsg.m_nStorey )
            {
                m_nChallengeScene = rMsg.m_nScene;
                m_nChallengeMusic = rMsg.m_nMusic;
                m_nChallengeMusicMode = rMsg.m_nMusicMode;
                m_nChallengeMusicLevel = rMsg.m_nMusicLevel;
            }
        }

        if ( m_nChallengeCurStorey == rMsg.m_nStorey )
        {
            m_nChallengeCurStoreyState = rMsg.m_nStoreyState;
            m_nChallengeMyInspire = rMsg.m_nMyInspire;
        }
    }

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestChallengeRankList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestChallengeRankList &rMsg = (GameMsg_C2S_RequestChallengeRankList &)msg;
    const DanceGroupChallengeStoreyInfo *pStoreyInfo = DanceGroupChallengeManager::Instance().GetStoreyInfo( rMsg.m_nRequestStorey );

    if ( pStoreyInfo == NULL || m_nGroupID == 0 )
    {
        GameMsg_S2C_RequestChallengeRankListRes resMsg;
        resMsg.m_nRankType = rMsg.m_nRankType;
        resMsg.m_nStorey = rMsg.m_nRequestStorey;
        resMsg.m_nCurPage = rMsg.m_nRequestPage;
        SendPlayerMsg( resMsg );
        return;
    }

    GameMsg_S2G_GetChallengeRankList s2gMsg;
    s2gMsg.m_nGroupID = m_nGroupID;
    s2gMsg.m_nRankType = rMsg.m_nRankType;
    s2gMsg.m_nRequestStorey = rMsg.m_nRequestStorey;
    s2gMsg.m_nRequestPage = rMsg.m_nRequestPage;
    s2gMsg.m_nRequestCount = rMsg.m_nRequestCount;
    Send2GroupMsg( s2gMsg );
}

void CEntityDanceGroupComponent::OnGetChallengeRankListRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetChallengeRankListRes &rMsg = (GameMsg_G2S_GetChallengeRankListRes &)msg;
    GameMsg_S2C_RequestChallengeRankListRes resMsg;
    resMsg.m_nRankType = rMsg.m_nRankType;
    resMsg.m_nStorey = rMsg.m_nStorey;
    resMsg.m_nCurPage = rMsg.m_nCurPage;
    resMsg.m_nTotalCount = rMsg.m_nTotalCount;
    resMsg.m_listInfo.swap( rMsg.m_listInfo );

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestGetChallengeReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestGetChallengeReward &rMsg = (GameMsg_C2S_RequestGetChallengeReward &)msg;
    GameMsg_S2C_RequestGetChallengeRewardRes resMsg;
    resMsg.m_bGetAll = rMsg.m_bGetAll;
    resMsg.m_nRequestStorey = rMsg.m_nRequestStorey;

    if ( m_pRoleAttr == NULL )
        return;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EGetChallengeRewardRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EGetChallengeRewardRes_NotInGroup;
            break;
        }
        else if ( m_mapRanking.empty() )
        {
            resMsg.m_nResult = EGetChallengeRewardRes_NoRewardInfo;
            break;
        }

        std::map<int, DanceGroupChallengeRanking> mapRanking;
        std::map<int, DanceGroupChallengeRanking>::iterator itr;

        if ( rMsg.m_bGetAll )
        {
            mapRanking.swap( m_mapRanking );
        }
        else
        {
            itr = m_mapRanking.find( rMsg.m_nRequestStorey );

            if ( itr != m_mapRanking.end() )
            {
                mapRanking.insert( std::make_pair( itr->first, itr->second ) );
                m_mapRanking.erase( itr );
            }
        }

        bool bMale = ( m_pRoleAttr->GetSex() == ESexType_Male );
        for ( itr = mapRanking.begin(); itr != mapRanking.end(); )
        {
            if ( itr->second.GetState() != EDanceGroupChallengeRankingState_None )
            {
                mapRanking.erase( itr++ );
                continue;
            }

            mgr.GetRewardInfos( itr->second.GetStorey(), itr->second.GetRanking(), bMale, resMsg.m_listReward );

            ++itr;
        }

        if ( mapRanking.empty() )
        {
            resMsg.m_nResult = EGetChallengeRewardRes_NoRewardInfo;
            break;
        }

        UpdateChallengeRankingState( mapRanking, m_nChallengeStartTime, EDanceGroupChallengeRankingState_GotReward );

        CItemProcess::AddOrMailItems( *(CRoleEntity *)Entity(), resMsg.m_listReward, 
            EItemAction_Add_DanceGroupChallengeGetReward, 0, true, EMailType_DanceGroupChallenge, 
            mgr.GetMailSender(), mgr.GetMailTitle(), mgr.GetMailContent() );
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestChallengeInspire(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if ( m_pRoleAttr == NULL )
        return;

    GameMsg_C2S_RequestChallengeInspire &rMsg = (GameMsg_C2S_RequestChallengeInspire &)msg;
    GameMsg_S2C_RequestChallengeInspireRes resMsg;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nRequestStorey );

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EChallengeInspireRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EChallengeInspireRes_NotInGroup;
            break;
        }
        else if ( m_pGroupInfo == NULL )
        {
            resMsg.m_nResult = EChallengeInspireRes_Initializing;
            break;
        }
        else if ( pStoreyInfo == NULL )
        {
            resMsg.m_nResult = EChallengeInspireRes_StoreyNotExists;
            break;
        }
        else if ( m_nChallengeMyInspire >= mgr.GetInspireMaxCount() )
        {
            resMsg.m_nResult = EChallengeInspireRes_MaxInspireCount;
            break;
        }
        else if ( m_nChallengeCurStorey < rMsg.m_nRequestStorey )
        {
            resMsg.m_nResult = EChallengeInspireRes_StoreyNotStart;
            break;
        }
        else if ( rMsg.m_nRequestStorey < m_nChallengeCurStorey )
        {
            resMsg.m_nResult = EChallengeInspireRes_StoreyFinished;
            break;
        }
        else if ( m_nChallengeCurStoreyState == EDanceGroupChallengeStoreyState_Finished )
        {
            resMsg.m_nResult = EChallengeInspireRes_StoreyFinished;
            break;
        }
        else if ( m_nChallengeCurStoreyState == EDanceGroupChallengeStoreyState_NotStart )
        {
            resMsg.m_nResult = EChallengeInspireRes_StoreyNotStart;
            break;
        }

        CurrencyMap mapBills;
        mgr.GetInspireCost().AddIntoCurrencyMap( mapBills, true );
        if ( !m_pRoleAttr->CanBill( mapBills ) )
        {
            resMsg.m_nResult = EChallengeInspireRes_NotEnoughMoney;
            break;
        }

        m_pRoleAttr->Bill( mapBills, EChangeBillCause_Del_DanceGroupChallengeInspire, 
            EChangeBindBillCause_Del_DanceGroupChallengeInspire, EChangeMoneyCause_Del_DanceGroupChallengeInspire );

        GameMsg_S2G_ChallengeInspire s2gMsg;
        s2gMsg.m_nGroupID = m_nGroupID;
        s2gMsg.m_nRequestStorey = rMsg.m_nRequestStorey;
        Send2GroupMsg( s2gMsg );
        return;
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnChallengeInspireRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_ChallengeInspireRes &rMsg = (GameMsg_G2S_ChallengeInspireRes &)msg;
    GameMsg_S2C_RequestChallengeInspireRes resMsg;
    resMsg.m_nResult = rMsg.m_nResult;
    resMsg.m_nAdditional = rMsg.m_nAdditional;

    if ( rMsg.m_nResult == EChallengeInspireRes_Success && rMsg.m_nStorey == m_nChallengeCurStorey )
        m_nChallengeMyInspire = rMsg.m_nMyInspire;

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestAddChallengeChance(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if ( m_pRoleAttr == NULL )
        return;

    GameMsg_S2C_RequestAddChallengeChanceRes resMsg;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EAddChallengeChanceRes_NotOpen;
            break;
        }

        Currency cost;
        CurrencyMap mapBills;
        mgr.GetNextBuyChanceInfo( m_nChallengeBuyChance, cost );
        cost.AddIntoCurrencyMap( mapBills, true );

        if ( !m_pRoleAttr->CanBill( mapBills ) )
        {
            resMsg.m_nResult = EAddChallengeChanceRes_NotEnoughMoney;
            break;
        }

        m_pRoleAttr->Bill( mapBills, EChangeBillCause_Del_DanceGroupChallengeBuyChance, 
            EChangeBindBillCause_Del_DanceGroupChallengeBuyChance, EChangeMoneyCause_Del_DanceGroupChallengeBuyChance );

        ++m_nChallengeBuyChance;
        ++m_nChallengeChance;
        UpdateChallengeBuyChance();
        UpdateChallengeChance();

        mgr.GetNextBuyChanceInfo( m_nChallengeBuyChance, cost );

        resMsg.m_nMaxChance = m_nChallengeTotalChance;
        resMsg.m_nCurChance = m_nChallengeChance;
        resMsg.m_nNextCostType = cost.GetType();
        resMsg.m_nNextCostAmount = cost.GetAmount();
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestOpenChallengeStorey(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if ( m_pRoleAttr == NULL )
        return;

    GameMsg_C2S_RequestOpenChallengeStorey &rMsg = (GameMsg_C2S_RequestOpenChallengeStorey &)msg;
    GameMsg_S2C_RequestOpenChallengeStoreyRes resMsg;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nRequestStorey );

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotInGroup;
            break;
        }
        else if ( m_pGroupInfo == NULL )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_Initializing;
            break;
        }
        else if ( m_pGroupInfo->m_DGBaseInfo.m_nLevel < mgr.GetOpenLevel() )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotEnoughGroupLevel;
            break;
        }
        else if ( pStoreyInfo == NULL )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_StoreyNotExists;
            break;
        }

        const CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember( m_pRoleAttr->GetRoleID() );

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
        else if ( m_pGroupInfo->m_DGBaseInfo.m_nChallengeActive < pStoreyInfo->GetActiveValue() )
        {
            resMsg.m_nResult = EOpenChallengeStoreyRes_NotEnoughActive;
            break;
        }

        DanceBaseRoom room( eRoomType_DanceGroupChallenge, INVALID_ROOM_ID );
        room.SetScene( pStoreyInfo->GetScene() );
        room.ChangeChoosedMusicInfo( pStoreyInfo->GetMusic(), (unsigned char)pStoreyInfo->GetMusicMode(), 
            (unsigned char)pStoreyInfo->GetMusicLevel() );
        room.PrepareMusicAndScene();

        GameMsg_S2G_OpenChallengeStorey s2gMsg;
        s2gMsg.m_nGroupID = m_nGroupID;
        s2gMsg.m_nRequestStorey = rMsg.m_nRequestStorey;
        s2gMsg.m_nScene = room.GetPlayScene();
        s2gMsg.m_nMusic = room.GetPlayMusic();
        s2gMsg.m_nMusicMode = (int)room.GetPlayMusicMode();
        s2gMsg.m_nMusicLevel = (int)room.GetPlayMusicLevel();
        s2gMsg.m_nTarget = pStoreyInfo->GetTargetScore();
        s2gMsg.m_nActive = pStoreyInfo->GetActiveValue();
        Send2GroupMsg( s2gMsg );
        return;
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnOpenChallengeStoreyRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_OpenChallengeStoreyRes &rMsg = (GameMsg_G2S_OpenChallengeStoreyRes &)msg;
    GameMsg_S2C_RequestOpenChallengeStoreyRes resMsg;
    resMsg.m_nResult = rMsg.m_nResult;

    if ( resMsg.m_nResult == EOpenChallengeStoreyRes_Success )
    {
        resMsg.m_wholeInfo.m_nStorey = rMsg.m_nStorey;
        resMsg.m_wholeInfo.m_nStoreyState = EDanceGroupChallengeStoreyState_Challenging;
        resMsg.m_wholeInfo.m_nMusic = (unsigned short)rMsg.m_nMusic;
        resMsg.m_wholeInfo.m_nMusicMode = (unsigned char)rMsg.m_nMusicMode;
        resMsg.m_wholeInfo.m_nMusicLevel = (unsigned char)rMsg.m_nMusicLevel;
        resMsg.m_wholeInfo.m_nMaxChance = m_nChallengeTotalChance;
        resMsg.m_wholeInfo.m_nCurChance = m_nChallengeChance;

        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();

        const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nStorey );
        if ( pStoreyInfo != NULL )
        {
            resMsg.m_wholeInfo.m_nActive = pStoreyInfo->GetActiveValue();
            resMsg.m_wholeInfo.m_nTargetType = pStoreyInfo->GetTargetType();
            resMsg.m_wholeInfo.m_nTargetScore = pStoreyInfo->GetTargetScore();
        }

        if ( m_pRoleAttr != NULL )
        {
            mgr.GetAllRewardInfos( rMsg.m_nStorey, m_pRoleAttr->GetSex() == ESexType_Male, 
                resMsg.m_wholeInfo.m_listReward );
        }

        m_nChallengeCurStorey = rMsg.m_nStorey;
        m_nChallengeCurStoreyState = EDanceGroupChallengeStoreyState_Challenging;
        m_nChallengeScene = rMsg.m_nScene;
        m_nChallengeMusic = rMsg.m_nMusic;
        m_nChallengeMusicMode = rMsg.m_nMusicMode;
        m_nChallengeMusicLevel = rMsg.m_nMusicLevel;
    }

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestPrepareChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestPrepareChallenge &rMsg = (GameMsg_C2S_RequestPrepareChallenge &)msg;
    GameMsg_S2C_RequestPrepareChallengeRes resMsg;

    do 
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nRequestStorey );

        if ( !mgr.IsOpenNow() )
        {
            resMsg.m_nResult = EPrepareChallengeRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EPrepareChallengeRes_NotInGroup;
            break;
        }
        else if ( m_pGroupInfo == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_Initializing;
            break;
        }
        else if ( pStoreyInfo == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_StoreyNotExists;
            break;
        }
        else if ( rMsg.m_nRequestStorey < m_nChallengeCurStorey )
        {
            resMsg.m_nResult = EPrepareChallengeRes_StoreyFinished;
            break;
        }
        else if ( rMsg.m_nRequestStorey == m_nChallengeCurStorey && 
            m_nChallengeCurStoreyState == EDanceGroupChallengeStoreyState_Finished )
        {
            resMsg.m_nResult = EPrepareChallengeRes_StoreyFinished;
            break;
        }

        const CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember( m_pRoleAttr->GetRoleID() );

        if ( pMember == NULL )
        {
            resMsg.m_nResult = EPrepareChallengeRes_NotInGroup;
            break;
        }
        else if ( IsTimeIsToday( pMember->m_nInTime ) )
        {
            resMsg.m_nResult = EPrepareChallengeRes_NewMember;
            break;
        }

        GameMsg_S2G_PrepareChallenge s2gMsg;
        s2gMsg.m_nGroupID = m_nGroupID;
        s2gMsg.m_nRequestStorey = rMsg.m_nRequestStorey;
        Send2GroupMsg( s2gMsg );
        return;
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnPrepareChallengeRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_PrepareChallengeRes &rMsg = (GameMsg_G2S_PrepareChallengeRes &)msg;
    GameMsg_S2C_RequestPrepareChallengeRes resMsg;
    resMsg.m_nResult = rMsg.m_nResult;
    resMsg.m_wholeInfo.m_nStorey = rMsg.m_nStorey;
    resMsg.m_wholeInfo.m_nStoreyState = rMsg.m_nStoreyState;
    resMsg.m_wholeInfo.m_nMaxChance = m_nChallengeTotalChance;
    resMsg.m_wholeInfo.m_nCurChance = m_nChallengeChance;

    if ( resMsg.m_nResult == EPrepareChallengeRes_Success )
    {
        DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
        const DanceGroupChallengeStoreyInfo *pStoreyInfo = mgr.GetStoreyInfo( rMsg.m_nStorey );

        if ( m_pRoleAttr != NULL )
        {
            mgr.GetAllRewardInfos( rMsg.m_nStorey, m_pRoleAttr->GetSex() == ESexType_Male, 
                resMsg.m_wholeInfo.m_listReward );
        }

        if ( pStoreyInfo != NULL )
        {
            resMsg.m_wholeInfo.m_nActive = pStoreyInfo->GetActiveValue();
            resMsg.m_wholeInfo.m_nTargetType = pStoreyInfo->GetTargetType();

            if ( rMsg.m_nStoreyState == EDanceGroupChallengeStoreyState_NotStart )
            {
                resMsg.m_wholeInfo.m_nMusic = (unsigned short)pStoreyInfo->GetMusic();
                resMsg.m_wholeInfo.m_nMusicMode = (unsigned char)pStoreyInfo->GetMusicMode();
                resMsg.m_wholeInfo.m_nMusicLevel = (unsigned char)pStoreyInfo->GetMusicLevel();
                resMsg.m_wholeInfo.m_nTargetScore = pStoreyInfo->GetTargetScore();
            }
            else
            {
                resMsg.m_wholeInfo.m_nMusic = (unsigned short)rMsg.m_nMusic;
                resMsg.m_wholeInfo.m_nMusicMode = (unsigned char)rMsg.m_nMusicMode;
                resMsg.m_wholeInfo.m_nMusicLevel = (unsigned char)rMsg.m_nMusicLevel;
                resMsg.m_wholeInfo.m_nTargetScore = rMsg.m_nTargetScore;
                resMsg.m_wholeInfo.m_nCurrentScore = rMsg.m_nCurrentScore;
                resMsg.m_wholeInfo.m_nAdditional = rMsg.m_nAdditional;

                if ( m_nChallengeCurStorey == rMsg.m_nStorey )
                {
                    m_nChallengeScene = rMsg.m_nScene;
                    m_nChallengeMusic = rMsg.m_nMusic;
                    m_nChallengeMusicMode = rMsg.m_nMusicMode;
                    m_nChallengeMusicLevel = rMsg.m_nMusicLevel;
                }
            }

            if ( m_nChallengeCurStorey == rMsg.m_nStorey )
            {
                m_nChallengeCurStoreyState = rMsg.m_nStoreyState;
            }
        }
    }

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnRequestStartChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    if ( m_pRoleAttr == NULL )
        return;

    GameMsg_C2S_RequestStartChallenge &rMsg = (GameMsg_C2S_RequestStartChallenge &)msg;
    GameMsg_S2C_RequestStartChallengeRes resMsg;

    do 
    {
        if ( !DanceGroupChallengeManager::Instance().IsOpenNow() )
        {
            resMsg.m_nResult = EStartChallengeRes_NotOpen;
            break;
        }
        else if ( m_nGroupID == 0 )
        {
            resMsg.m_nResult = EStartChallengeRes_NotInGroup;
            break;
        }
        else if ( m_pGroupInfo == NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_Initializing;
            break;
        }
        else if ( m_pRoleRoom != NULL && m_pRoleRoom->Room() != NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_InRoom;
            break;
        }
        else if ( m_pRoleCouple != NULL && m_pRoleCouple->WeddingRoom() != NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_InCoupleRoom;
            break;
        }
        else if ( m_pRolePhotoRoom != NULL && m_pRolePhotoRoom->PhotoRoom() != NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_InPhotoRoom;
            break;
        }
        else if ( m_pRoleAmuseRoom != NULL && m_pRoleAmuseRoom->AmuseRoom() != NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_InAmuseRoom;
            break;
        }
        else if ( GetCeremonyRoom() != NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_InCeremonyRoom;
            break;
        }
        else if ( rMsg.m_nRequestStorey < m_nChallengeCurStorey )
        {
            resMsg.m_nResult = EStartChallengeRes_StoreyFinished;
            break;
        }
        else if ( m_nChallengeCurStorey < rMsg.m_nRequestStorey )
        {
            resMsg.m_nResult = EStartChallengeRes_StoreyNotStart;
            break;
        }
        else if ( m_nChallengeCurStoreyState == EDanceGroupChallengeStoreyState_NotStart )
        {
            resMsg.m_nResult = EStartChallengeRes_StoreyNotStart;
            break;
        }
        else if ( m_nChallengeCurStoreyState == EDanceGroupChallengeStoreyState_Finished )
        {
            resMsg.m_nResult = EStartChallengeRes_StoreyFinished;
            break;
        }
        else if ( m_nChallengeChance == 0 )
        {
            resMsg.m_nResult = EStartChallengeRes_NotEnoughChance;
            break;
        }
        else if ( m_nChallengeRoomID != INVALID_ROOM_ID )
        {
            resMsg.m_nResult = EStartChallengeRes_AlreadyStarted;
            break;
        }

        const CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember( m_pRoleAttr->GetRoleID() );

        if ( pMember == NULL )
        {
            resMsg.m_nResult = EStartChallengeRes_NotInGroup;
            break;
        }
        else if ( IsTimeIsToday( pMember->m_nInTime ) )
        {
            resMsg.m_nResult = EStartChallengeRes_NewMember;
            break;
        }

        std::string strRoomName;
        SafeFormat( strRoomName, CLocalization::Instance().GetString( "DanceGroup_Room_Name" ), m_nChallengeCurStorey );

        DanceGroupArcadeRoomManager &roomMgr = DanceGroupArcadeRoomManager::Instance();
        m_nChallengeRoomID = roomMgr.CreateAndEnterRoom( m_pRoleAttr->GetRoleID(), strRoomName, m_nChallengeScene, 
            m_nChallengeMusic, (unsigned char)m_nChallengeMusicMode, (unsigned char)m_nChallengeMusicLevel );

        if ( !roomMgr.Prepare( m_nChallengeRoomID, rMsg.m_nPhoneOS, resMsg.m_strCheckKey, resMsg.m_strStage ) )
        {
            resMsg.m_nResult = EStartChallengeRes_LoadStageFailed;
            m_nChallengeRoomID = INVALID_ROOM_ID;
            break;
        }

        resMsg.m_roomInfo.m_nScene = (unsigned char)m_nChallengeScene;
        resMsg.m_roomInfo.m_nMusic = (unsigned short)m_nChallengeMusic;
        resMsg.m_roomInfo.m_nMusicMode = (unsigned char)m_nChallengeMusicMode;
        resMsg.m_roomInfo.m_nMusicLevel = (unsigned char)m_nChallengeMusicLevel;

        DungeonPlayerInfo hostInfo;
        EncodePlayerInfo( hostInfo );
        resMsg.m_roomInfo.m_listDancer.push_back( hostInfo );

        DungeonPlayerInfo bossInfo;
        if ( EncodeBoosInfo( bossInfo, m_nChallengeCurStorey ) )
        {
            bossInfo.m_chRolePos = (char)resMsg.m_roomInfo.m_listDancer.size();
            resMsg.m_roomInfo.m_listDancer.push_back( bossInfo );
        }

        m_nChallengeSaveStorey = m_nChallengeCurStorey;
        m_nChallengeSaveStartTime = m_nChallengeStartTime;

        SendPlayerMsg( resMsg );

        roomMgr.Start( m_nChallengeRoomID );
        return;
    } while (0);

    SendPlayerMsg( resMsg );
}

void CEntityDanceGroupComponent::OnReportChallengeScoreRes(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_ReportChallengeScoreRes &rMsg = (GameMsg_G2S_ReportChallengeScoreRes &)msg;
    GameMsg_S2C_ChallengeStageEnd endMsg;
    endMsg.m_scoreInfo = rMsg.m_scoreInfo;

    SendPlayerMsg( endMsg );
}

void CEntityDanceGroupComponent::OnChallengeStoreyFinished(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_ChallengeStoreyFinishedNotice &rMsg = (GameMsg_G2S_ChallengeStoreyFinishedNotice &)msg;
    DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();

    if ( mgr.GetTotalStorey() < rMsg.m_nNextStorey )
    {
        m_nChallengeCurStorey = mgr.GetTotalStorey();
        m_nChallengeCurStoreyState = EDanceGroupChallengeStoreyState_Finished;
    }
    else
    {
        m_nChallengeCurStorey = rMsg.m_nNextStorey;
        m_nChallengeCurStoreyState = EDanceGroupChallengeStoreyState_NotStart;
        m_nChallengeMyInspire = 0;
    }

    if ( mgr.IsOpenNow() )
    {
        GameMsg_S2C_ChallengeStoreyFinishedNotice noticeMsg;
        noticeMsg.m_nStorey = rMsg.m_nNextStorey - 1;
        SendPlayerMsg( noticeMsg );
    }
}

void CEntityDanceGroupComponent::OnChallengeRewardNotice(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_ChallengeRewardNotice &rMsg = (GameMsg_G2S_ChallengeRewardNotice &)msg;

    DanceGroupChallengeRanking ranking( rMsg.m_nStorey, rMsg.m_nRanking );
    m_mapRanking.insert( std::make_pair( rMsg.m_nStorey, ranking ) );

    if ( DanceGroupChallengeManager::Instance().IsOpenNow() )
    {
        GameMsg_S2C_ChallengeRewardNotice noticeMsg;
        noticeMsg.m_nStorey = rMsg.m_nStorey;
        SendPlayerMsg( noticeMsg );
    }
}

void CEntityDanceGroupComponent::OnQueryChallengeRanking(QueryBase &rQuery)
{
    Query<std::map<int, DanceGroupChallengeRanking> > *pTmp = (Query<std::map<int, DanceGroupChallengeRanking> > *)&rQuery;
    DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
    m_nChallengeStartTime = mgr.GetStartTime();

    if ( pTmp != NULL && pTmp->IsValid() && !(*pTmp)->empty() )
    {
        unsigned int nOldStartTime = (unsigned int)pTmp->m_nTag;

        if ( m_nGroupID == 0 || nOldStartTime != m_nChallengeStartTime )
        {
            UpdateChallengeRankingState( **pTmp, nOldStartTime, EDanceGroupChallengeRankingState_Invalid );
        }
        else
        {
            (*pTmp)->swap( m_mapRanking );
        }
    }

    if ( m_pRoleTimer != NULL )
    {
        unsigned int nNextStartTime = mgr.GetNextStartTime( m_nChallengeStartTime );
        unsigned int nCloseTime = mgr.GetCloseTime( nNextStartTime );
        unsigned int nAnnouncementTime = mgr.GetAnnouncementTime( nNextStartTime );
        m_pRoleTimer->AddTimer( nCloseTime, eComponentEvent_DanceGroupChallengeResetRanking, 0 );
        m_pRoleTimer->AddTimer( nAnnouncementTime, eComponentEvent_DanceGroupChallengeCloseAnnounce, 0 );
    }

    SyncChallengeConfig();
}

void CEntityDanceGroupComponent::OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBigMamaInfoNotifyOnLogin(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBigMamaInfoNotifyOnJoinDanceGroup(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SNoticeBigMamaState(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SNoticeBigMamaState(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SBroadBigMamaInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBroadBigMamaInfo(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SBigMamaInfoNotify(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBigMamaInfoNotify(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SBigMamaFightResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBigMamaFightResult(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnG2SBigMamaGetRewardResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnG2SBigMamaGetRewardResult(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnRequestBigMamaStageInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnRequestBigMamaStageInfo(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnRequestFightBigMama(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnRequestFightBigMama(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnRequestGetBigMamaReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnRequestGetBigMamaReward(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnCompleteBigMamaGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnCompleteBigMamaGuide(&rMsgBase);

    return;
}

void CEntityDanceGroupComponent::OnRequestBigMamaGuideReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    m_entityBigMama.OnRequestBigMamaGuideReward(&rMsgBase);

    return;
}



void CEntityDanceGroupComponent::Send2GroupMsg(GameMsg_Base &msg)
{
    if (NULL != m_pNetComponent)
        m_pNetComponent->Send2GroupServer(&msg);
}

void CEntityDanceGroupComponent::SendPlayerMsg(GameMsg_Base &msg)
{
    if (NULL != m_pNetComponent)
        m_pNetComponent->SendPlayerMsg(&msg);
}

void CEntityDanceGroupComponent::SendMsgToRooms(GameMsg_Base &msg)
{
    if (NULL != m_pRoleRoom && NULL != m_pRoleRoom->Room())
    {
        m_pRoleRoom->Room()->SendMsgToAll(&msg, m_pRoleRoom);
    }
    else if (NULL != m_pRoleCouple && NULL != m_pRoleCouple->WeddingRoom())
    {
        m_pRoleCouple->WeddingRoom()->SendMsgToAllBut(&msg, m_pRoleCouple);
    }
    else if (NULL != m_pRolePhotoRoom && NULL != m_pRolePhotoRoom->PhotoRoom())
    {
        m_pRolePhotoRoom->PhotoRoom()->SendMsgToAllBut(&msg, m_pRolePhotoRoom);
    }
    else if (NULL != m_pRoleAmuseRoom && NULL != m_pRoleAmuseRoom->AmuseRoom())
    {
        m_pRoleAmuseRoom->AmuseRoom()->SendMsgToAll(&msg, m_pRoleAmuseRoom);
    }
    else if (NULL != GetCeremonyRoom())
    {
        GetCeremonyRoom()->RoomBroadcast(msg);
    }
}

void CEntityDanceGroupComponent::GetMyDanceGroupInfo(unsigned short nClickedPlace)
{
    if (0 != m_nGroupID)
    {
        GameMsg_S2G_GetMyDanceGroupInfo getInfoMsg;
        getInfoMsg.m_nGroupID = m_nGroupID;
        getInfoMsg.m_nClickedPlace = nClickedPlace;

        Send2GroupMsg(getInfoMsg);
    }
    else
    {
        PushDanceGroupInfoOnLogin(true, nClickedPlace);
    }
}

void CEntityDanceGroupComponent::PushDanceGroupInfoOnLogin(bool bValid, unsigned short nClickedPlace)
{
    GameMsg_S2C_DanceGroupInfo infoMsg;
    infoMsg.m_bIsValid = bValid;
    infoMsg.m_nGroupID = m_nGroupID;
    infoMsg.m_nClickedPlace = nClickedPlace;

    if (NULL != m_pGroupInfo)
        infoMsg.m_pDGInfo = new CDanceGroupInfo(*m_pGroupInfo);

    SendPlayerMsg(infoMsg);

    if (m_bTodayFirstLogin && 0 != m_nGroupID && NULL != m_pRoleAttr && NULL != m_pGroupInfo && 
        m_pRoleAttr->GetLevel() >= CDanceGroupDataMgr::Instance().MemberLoginGroupHonorBonusLevelNeed())
    {
        ChangeDanceGroupHonor(CDanceGroupDataMgr::Instance().MemberLoginGropHonorBonus(), 
            EChangeDanceGroupHonorCause_MemberLogin, m_pRoleAttr->GetRoleID());
    }
}

void CEntityDanceGroupComponent::UpdateDanceGroupInfo()
{
    if (NULL != m_pGroupInfo)
    {
        GameMsg_S2C_SendDanceGroupInfo infoMsg;
        infoMsg.m_pDGInfo = new CDanceGroupInfo(*m_pGroupInfo);

        SendPlayerMsg(infoMsg);
    }
}

void CEntityDanceGroupComponent::UpdateMemberInfo(bool bOnline)
{
    if (NULL != m_pRoleAttr && 0 != m_nGroupID && NULL != m_pGroupInfo)
    {
        GameMsg_S2G_UpdateDanceGroupMemberInfo infoMsg;
        infoMsg.m_nGroupID = m_nGroupID;
        infoMsg.m_nState = (unsigned char)(bOnline ? EPlayerState_Idle : EPlayerState_Offline);
        infoMsg.m_nLastOffLineTime = (bOnline ? m_pRoleAttr->GetLastLogoutTime() : (unsigned int)time(NULL));

        Send2GroupMsg(infoMsg);
    }
}

void CEntityDanceGroupComponent::UpdateDanceGroupName(const std::string &rStrNewName)
{
    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_UpdateDanceGroupName updateMsg;
        updateMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        updateMsg.m_strNewName = rStrNewName;

        SendPlayerMsg(updateMsg);
        SendMsgToRooms(updateMsg);
    }
}

void CEntityDanceGroupComponent::UpdateDanceGroupTitle(unsigned char nTitle)
{
    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_UpdateDanceGroupTitle updateMsg;
        updateMsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        updateMsg.m_nTitle = (char)nTitle;

        SendPlayerMsg(updateMsg);
        SendMsgToRooms(updateMsg);
    }
}

void CEntityDanceGroupComponent::UpdateDanceGroupBadgeOrEffect(unsigned short nBadge, unsigned short nEffect)
{
    if (NULL != m_pRoleAttr)
    {
        GameMsg_S2C_ChangeDanceGroupBadgeOrEffectResultNew changeMsg;
        changeMsg.m_nErrorCode = EDanceGroupBadgeResult_Notify;
        changeMsg.m_nRoleID = m_pRoleAttr->m_nRoleID;
        changeMsg.m_nBadge = nBadge;
        changeMsg.m_nEffect = nEffect;

        SendPlayerMsg(changeMsg);
        SendMsgToRooms(changeMsg);
    }
}

bool CEntityDanceGroupComponent::ProcessFirstReward()
{
    if (0 == m_nGroupID || NULL == m_pRoleAttr)
        return false;

    if (!IsTimeIsToday(m_nLastGetRewardTime))
    {
        m_bNormalReward = false;
        m_nLastGetRewardTime = (unsigned int)time(NULL);

        AddQuery(QUERY_Add_DanceGroupReward, m_pRoleAttr->GetRoleID(), NULL, m_nLastGetRewardTime);
    }

    if (!m_bNormalReward)
    {
        m_bNormalReward = true;

        std::list<CItem> listPacketItem;

        if (ESexType_Male == m_pRoleAttr->GetSex())
            listPacketItem = CDanceGroupDataMgr::Instance().m_maleList;
        else
            listPacketItem = CDanceGroupDataMgr::Instance().m_femaleList;

        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listPacketItem, 
            EItemAction_DanceGroup_Activity, 0, 
            true, EMailType_DanceGroupActivity, "", "");

        return true;
    }

    return false;
}

bool CEntityDanceGroupComponent::IsDanceGroupIDSame(unsigned int nGroupID)
{
    if (m_nGroupID != nGroupID)
    {
        LOG_ERROR << "My dance group ID(" 
            << m_nGroupID 
            << ") is not equal to dance group ID(" 
            << nGroupID 
            << ") in msg" << std::endl;

        return false;
    }

    return true;
}

void CEntityDanceGroupComponent::SetDanceGroupInfo(const CDanceGroupInfo *pDGInfo)
{
    if (NULL != pDGInfo)
    {
        if (NULL != m_pGroupInfo)
            delete m_pGroupInfo;

        m_pGroupInfo = new CDanceGroupInfo(*pDGInfo);

        if (NULL != m_pGroupInfo)
        {
            m_nGroupID = m_pGroupInfo->m_DGBaseInfo.m_nGroupID;
        }
        else
        {
            LOG_ERROR << "Can not create CDanceGroupInfo's instance" << std::endl;
        }
    }
}

void CEntityDanceGroupComponent::ClearDanceGroupInfo()
{
    m_nGroupID = 0;

    if (NULL != m_pGroupInfo)
    {
        delete m_pGroupInfo;
        m_pGroupInfo = NULL;

        UpdateDanceGroupName("");
    }

    UpdateChallengeRankingState( m_mapRanking, m_nChallengeStartTime, EDanceGroupChallengeRankingState_Invalid );
}

EDanceGroupBadgeResult CEntityDanceGroupComponent::CheckDanceGroupBadgeCond()
{
    if (0 == m_nGroupID)
        return EDanceGroupBadgeResult_NoInDG;
    else if (NULL == m_pGroupInfo)
        return EDanceGroupBadgeResult_DGNotExist;
    else if (NULL == m_pRoleAttr || m_pRoleAttr->GetRoleID() != m_pGroupInfo->m_DGBaseInfo.m_nLeaderRoleID)
        return EDanceGroupBadgeResult_NoPrivilege;

    return EDanceGroupBadgeResult_Success;
}

EDanceGroupActivityStarResult CEntityDanceGroupComponent::CheckDanceGroupActivityStarCond()
{
    if (0 == m_nGroupID)
        return EDanceGroupActivityStar_NoInDG;
    else if (NULL == m_pGroupInfo)
        return EDanceGroupActivityStar_DGNotExist;
    else if (NULL == m_pRoleAttr)
        return EDanceGroupActivityStar_NotGroupMember;

    CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID());

    if (NULL == pMember)
        return EDanceGroupActivityStar_NotGroupMember;

    int nDiffDays = DiffDays(time(NULL), pMember->m_nInTime);

    if (0 == nDiffDays)
        return EDanceGroupActivityStar_JoinDGSameDay;

    return EDanceGroupActivityStar_Success;
}

EDanceGroupActivityStarResult CEntityDanceGroupComponent::CheckDanceGroupActivityAddStarPowerCond(unsigned short nType)
{
    EDanceGroupActivityStarResult result = CheckDanceGroupActivityStarCond();

    if (EDanceGroupActivityStar_Success != result)
        return result;

    switch (nType)
    {
    case EDanceGroupActivityStarType_Normal:
        break;

    case EDanceGroupActivityStarType_Money:
        if (m_pRoleAttr->GetMoney() < CDanceGroupDataMgr::Instance().m_nActivityStarMoneyNum)
            return EDanceGroupActivityStar_NoEnoughMoney;

        break;

    case EDanceGroupActivityStarType_Point:
        if (m_pRoleAttr->GetBillInfo().nPoint < (unsigned int)CDanceGroupDataMgr::Instance().m_nActivityStarPointNum)
            return EDanceGroupActivtyStar_NoEnoughMCoin;

        break;

    case EDanceGroupActivityStarType_Point2Money:
        if (m_pRoleAttr->GetBillInfo().nPoint < (unsigned int)CDanceGroupDataMgr::Instance().m_nActivityStarMoneyNum)
            return EDanceGroupActivtyStar_NoEnoughMCoin;

        break;

    default:
        return EDanceGroupActivityStar_Failed;
        break;
    }

    return EDanceGroupActivityStar_Success;
}

EDanceGroupShopResult CEntityDanceGroupComponent::CheckDanceGroupShopCond()
{
    if (0 == m_nGroupID)
        return EDanceGroupShopResult_NoInDG;
    else if (NULL == m_pGroupInfo)
        return EDanceGroupShopResult_DGNotExist;
    else if (NULL == m_pRoleAttr)
        return EDanceGroupShopResult_NotGroupMember;

    CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(m_pRoleAttr->GetRoleID());

    if (NULL == pMember)
        return EDanceGroupShopResult_NotGroupMember;

    return EDanceGroupShopResult_Success;
}

EInviteDanceGroupMemberResult CEntityDanceGroupComponent::CheckInviteDanceGroupMemberCond(unsigned int nRoleID, CRoleEntity *pInviteeRoleEntity)
{
    if (m_nGroupID == 0)
        return EInviteDanceGroupMemberResult_NotInGroup;
    else if (m_pGroupInfo == NULL)
        return EInviteDanceGroupMemberResult_Initializing;

    CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(nRoleID);

    if (pMember == NULL)
        return EInviteDanceGroupMemberResult_NotInGroup;
    else if (!HasAddMemberAuthority(pMember->m_nTitle) )
        return EInviteDanceGroupMemberResult_NoAuthority;
    else if (pInviteeRoleEntity == NULL)
        return EInviteDanceGroupMemberResult_NotInSameLine;
    else if (pInviteeRoleEntity->IsInBlackList(nRoleID) )
        return EInviteDanceGroupMemberResult_InBlackList;
    else if (pInviteeRoleEntity->GetDanceGroupID() != 0)
        return EInviteDanceGroupMemberResult_InviteeHasGroup;
    else if (m_pGroupInfo->GetMemberCount() >= GetDanceGroupMaxMemberCount(m_pGroupInfo->m_DGBaseInfo.m_nLevel) )
        return EInviteDanceGroupMemberResult_MaxCount;

    return EInviteDanceGroupMemberResult_Success;
}

void CEntityDanceGroupComponent::RegChallengeNetMsgMap()
{
    if ( !m_bChallengeOpening )
        return;

    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestEnterChallenge, &CEntityDanceGroupComponent::OnRequestEnterChallenge);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestChallengeStoreyList, &CEntityDanceGroupComponent::OnRequestChallengeStoreyList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestChallengeStoreyInfo, &CEntityDanceGroupComponent::OnRequestChallengeStoreyInfo);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestChallengeRankList, &CEntityDanceGroupComponent::OnRequestChallengeRankList);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestGetChallengeReward, &CEntityDanceGroupComponent::OnRequestGetChallengeReward);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestChallengeInspire, &CEntityDanceGroupComponent::OnRequestChallengeInspire);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestAddChallengeChance, &CEntityDanceGroupComponent::OnRequestAddChallengeChance);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestOpenChallengeStorey, &CEntityDanceGroupComponent::OnRequestOpenChallengeStorey);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestPrepareChallenge, &CEntityDanceGroupComponent::OnRequestPrepareChallenge);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestStartChallenge, &CEntityDanceGroupComponent::OnRequestStartChallenge);

    RegMsgCreatorAndProcessor(GameMsg_G2S_EnterChallengeRes, &CEntityDanceGroupComponent::OnGetCurrentStoreyRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_GetChallengeStoreyInfoRes, &CEntityDanceGroupComponent::OnGetChallengeStoreyInfoRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_GetChallengeRankListRes, &CEntityDanceGroupComponent::OnGetChallengeRankListRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_ChallengeInspireRes, &CEntityDanceGroupComponent::OnChallengeInspireRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_OpenChallengeStoreyRes, &CEntityDanceGroupComponent::OnOpenChallengeStoreyRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_PrepareChallengeRes, &CEntityDanceGroupComponent::OnPrepareChallengeRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_ReportChallengeScoreRes, &CEntityDanceGroupComponent::OnReportChallengeScoreRes);
    RegMsgCreatorAndProcessor(GameMsg_G2S_ChallengeStoreyFinishedNotice, &CEntityDanceGroupComponent::OnChallengeStoreyFinished);
    RegMsgCreatorAndProcessor(GameMsg_G2S_ChallengeRewardNotice, &CEntityDanceGroupComponent::OnChallengeRewardNotice);

    RegQueryProcessor(QUERY_DanceGroupChallenge_LoadRanking, &CEntityDanceGroupComponent::OnQueryChallengeRanking);
}

void CEntityDanceGroupComponent::SyncChallengeConfig()
{
    if ( !m_bChallengeOpening || m_nGroupID == 0 )
        return;

    DanceGroupChallengeManager& mgr = DanceGroupChallengeManager::Instance();

    GameMsg_S2C_SyncChallengeConfig syncConfigMsg;
    syncConfigMsg.m_nTotalStorey = mgr.GetTotalStorey();
    syncConfigMsg.m_nInspireCostType = mgr.GetInspireCost().GetType();
    syncConfigMsg.m_nInspireCostAmount = mgr.GetInspireCost().GetAmount();
    syncConfigMsg.m_nOpenLevel = mgr.GetOpenLevel();
    syncConfigMsg.m_strHelp = mgr.GetHelp();

    for ( std::map<int, DanceGroupChallengeRanking>::iterator itr = m_mapRanking.begin(); 
        itr != m_mapRanking.end(); ++itr )
    {
        syncConfigMsg.m_listRewardStorey.push_back( itr->first );
    }

    SendPlayerMsg( syncConfigMsg );
}

void CEntityDanceGroupComponent::ResetChallengeChance()
{
    if ( !m_bChallengeOpening )
        return;

    CalcChallengeTotalChance();
    unsigned int nNow = (unsigned int)time( NULL );

    if ( !IsSameDay( m_nChallengeChanceResetTime, nNow ) )
    {
        if ( m_nChallengeChance < m_nChallengeTotalChance )
        {
            m_nChallengeChance = m_nChallengeTotalChance;
        }

        m_nChallengeChanceResetTime = nNow;

        if ( m_pRoleAttr != NULL )
        {
            DanceGroupChallengeChanceEventDB *pReset = new DanceGroupChallengeChanceEventDB;
            pReset->SetRoleID( m_pRoleAttr->GetRoleID() );
            pReset->SetValue( m_nChallengeChance );
            pReset->SetResetTime( m_nChallengeChanceResetTime );

            AddQuery( QUERY_DanceGroupChallenge_ResetChance, SharedPtr<DanceGroupChallengeChanceEventDB>( pReset ) );
        }
    }

    if ( m_pRoleTimer != NULL )
    {
        unsigned int nNextResetTime = GetNextDayStartTime( m_nChallengeChanceResetTime );
        m_pRoleTimer->AddTimer( nNextResetTime, eComponentEvent_DanceGroupChallengeResetChance, 0 );
    }

    GameMsg_S2C_RefreshChallengeChanceNotice noticeMsg;
    noticeMsg.m_nMaxChance = m_nChallengeTotalChance;
    noticeMsg.m_nCurChance = m_nChallengeChance;
    SendPlayerMsg( noticeMsg );
}

void CEntityDanceGroupComponent::UpdateChallengeChance()
{
    if ( m_pRoleAttr == NULL )
        return;

    DanceGroupChallengeChanceEventDB *pUpdate = new DanceGroupChallengeChanceEventDB;
    pUpdate->SetRoleID( m_pRoleAttr->GetRoleID() );
    pUpdate->SetValue( m_nChallengeChance );

    AddQuery( QUERY_DanceGroupChallenge_UpdateChance, SharedPtr<DanceGroupChallengeChanceEventDB>( pUpdate ) );
}

void CEntityDanceGroupComponent::CalcChallengeTotalChance()
{
    m_nChallengeTotalChance = DanceGroupChallengeManager::Instance().GetFreeChance();

    if ( m_pRoleVIP != NULL )
        m_nChallengeTotalChance += m_pRoleVIP->GetDanceGroupChallengeExtraChance();
}

void CEntityDanceGroupComponent::ResetChallengeBuyChance()
{
    if ( !m_bChallengeOpening )
        return;

    unsigned int nNow = (unsigned int)time( NULL );

    if ( !IsSameDay( m_nChallengeBuyChanceResetTime, nNow ) )
    {
        m_nChallengeBuyChance = 0;
        m_nChallengeBuyChanceResetTime = nNow;

        if ( m_pRoleAttr != NULL )
        {
            DanceGroupChallengeChanceEventDB *pReset = new DanceGroupChallengeChanceEventDB;
            pReset->SetRoleID( m_pRoleAttr->GetRoleID() );
            pReset->SetValue( m_nChallengeBuyChance );
            pReset->SetResetTime( m_nChallengeBuyChanceResetTime );

            AddQuery( QUERY_DanceGroupChallenge_ResetBuyChance, SharedPtr<DanceGroupChallengeChanceEventDB>( pReset ) );
        }
    }

    if ( m_pRoleTimer != NULL )
    {
        unsigned int nNextResetTime = GetNextDayStartTime( m_nChallengeBuyChanceResetTime );
        m_pRoleTimer->AddTimer( nNextResetTime, eComponentEvent_DanceGroupChallengeResetBuyChance, 0 );
    }
}

void CEntityDanceGroupComponent::UpdateChallengeBuyChance()
{
    if ( m_pRoleAttr == NULL )
        return;

    DanceGroupChallengeChanceEventDB *pUpdate = new DanceGroupChallengeChanceEventDB;
    pUpdate->SetRoleID( m_pRoleAttr->GetRoleID() );
    pUpdate->SetValue( m_nChallengeBuyChance );

    AddQuery( QUERY_DanceGroupChallenge_UpdateBuyChance, SharedPtr<DanceGroupChallengeChanceEventDB>( pUpdate ) );
}

void CEntityDanceGroupComponent::LoadChallengeRanking()
{
    if ( !m_bChallengeOpening || m_pRoleAttr == NULL )
        return;

    m_nChallengeStartTime = DanceGroupChallengeManager::Instance().GetStartTime();
    std::map<int, DanceGroupChallengeRanking> *pRanking = new std::map<int, DanceGroupChallengeRanking>;

    AddQuery( QUERY_DanceGroupChallenge_LoadRanking, SharedPtr<std::map<int, DanceGroupChallengeRanking> >( pRanking ), 
        m_pRoleAttr->GetRoleID(), m_nChallengeStartTime );
}

void CEntityDanceGroupComponent::UpdateChallengeRankingState( std::map<int, DanceGroupChallengeRanking> &rRanking, 
    unsigned int nStartTime, unsigned char nState )
{
    if ( !m_bChallengeOpening || rRanking.empty() || m_pRoleAttr == NULL || nState >= EDanceGroupChallengeRankingState_Max )
        return;

    std::map<int, DanceGroupChallengeRanking> *pRanking = new std::map<int, DanceGroupChallengeRanking>;
    SharedPtr<std::map<int, DanceGroupChallengeRanking> > sharedPtr( pRanking );
    pRanking->swap( rRanking );

    if ( pRanking->empty() )
        return;

    pRanking->begin()->second.SetState( nState );

    AddQuery( QUERY_DanceGroupChallenge_UpdateRankingState, sharedPtr, m_pRoleAttr->GetRoleID(), nStartTime );
}

void CEntityDanceGroupComponent::EncodePlayerInfo( DungeonPlayerInfo &rInfo )
{
    if ( m_pRoleAttr == NULL )
        return;

    rInfo.m_nRoleID = m_pRoleAttr->GetRoleID();
    rInfo.m_strRoleName = m_pRoleAttr->GetRoleName();
    rInfo.m_bIsHost = true;
    rInfo.m_chRoleType = RoleRoomType_Dancer;
    rInfo.m_nIsBoss = false;
    rInfo.m_chRolePos = MIN_DANCE_ROOM_POS;
    rInfo.m_chRoleState = RoleRoomState_ToStart;
    rInfo.m_eColorTeam = EDanceColorTeam_None;
    rInfo.m_nRoleSex = (char)m_pRoleAttr->GetSex();
    rInfo.m_nRoleSkin = (char)m_pRoleAttr->GetColor();
    rInfo.m_nMoveType = 0;
    rInfo.m_nTransFormID = (unsigned short)m_pRoleAttr->GetTransformId();
    rInfo.m_bEncodeMedal = true;

    if ( m_pGroupInfo != NULL )
    {
        rInfo.m_strDanceGroupName = m_pGroupInfo->m_DGBaseInfo.m_strGroupName;
        rInfo.m_nDanceGroupBadge = m_pGroupInfo->m_DGBaseInfo.m_nBadge;
        rInfo.m_nDanceGroupEffect = m_pGroupInfo->m_DGBaseInfo.m_nEffect;

        CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember( rInfo.m_nRoleID );
        if ( pMember != NULL )
        {
            rInfo.m_nGroupTitle = (char)pMember->m_nTitle;
        }
    }

    if ( m_pRoleVIP != NULL )
    {
        rInfo.m_bIsVIP = m_pRoleVIP->IsVIP();
        rInfo.m_nVIPLevel = (unsigned short)m_pRoleVIP->VIPLevel();
    }

    rInfo.m_pItemCOM = m_pRoleItem;
    rInfo.m_pGeneCOM = GetComponent<CRoleGene>();
    rInfo.m_pPetCOM = GetComponent<CRolePet>();
    rInfo.m_pMedalCOM = GetComponent<CRoleMedal>();
}

bool CEntityDanceGroupComponent::EncodeBoosInfo( DungeonPlayerInfo &rInfo, int nStorey ) const
{
    const CBossConfig *pBossInfo = DanceGroupChallengeManager::Instance().GetBossInfo( nStorey );
    if ( pBossInfo == NULL )
        return false;

    rInfo.m_nRoleID = 0;
    rInfo.m_strRoleName = pBossInfo->m_strName;
    rInfo.m_nRoleSex = pBossInfo->m_cSex;
    rInfo.m_nRoleSkin = (char)pBossInfo->m_cSKinColor;
    rInfo.m_bIsHost = false;
    rInfo.m_chRoleType = RoleRoomType_Boss;
    rInfo.m_nIsBoss = true;
    rInfo.m_chRoleState = RoleRoomState_ToStart;
    rInfo.m_eColorTeam = EDanceColorTeam_None;
    rInfo.m_bEncodeMedal = true;
    rInfo.m_Equips = pBossInfo->m_Equips;

    return true;
}

bool CEntityDanceGroupComponent::EndRoom()
{
    if ( m_nChallengeRoomID == INVALID_ROOM_ID || m_pRoleAttr == NULL )
        return false;

    unsigned int score = 0;
    CRoomMarkInfo mark;
    bool bSuccess = DanceGroupArcadeRoomManager::Instance().End( m_nChallengeRoomID, m_pRoleAttr->GetRoleID(), score, mark );

    if ( bSuccess )
    {
        m_nChallengeRoomID = INVALID_ROOM_ID;

        --m_nChallengeChance;
        UpdateChallengeChance();

        GameMsg_S2C_ChallengeStageEnd resMsg;

        do 
        {
            DanceGroupChallengeManager &mgr = DanceGroupChallengeManager::Instance();
            const DanceGroupChallengeStoreyInfo *pStorey = mgr.GetStoreyInfo( m_nChallengeSaveStorey );

            if ( pStorey != NULL )
            {
                resMsg.m_scoreInfo.m_nTargetType = pStorey->GetTargetType();
                resMsg.m_scoreInfo.m_nTargetScore = pStorey->GetTargetScore();
            }
            resMsg.m_scoreInfo.m_nCurScore = score;
            resMsg.m_scoreInfo.m_nMyScore = score;
            resMsg.m_scoreInfo.m_nGrade = (unsigned char)mark.Grade();
            resMsg.m_scoreInfo.m_nPerfectCount = mark.TotalRankCount( RoomRoundRank_Perfect );
            resMsg.m_scoreInfo.m_nCoolCount = mark.TotalRankCount( RoomRoundRank_Cool );
            resMsg.m_scoreInfo.m_nGoodCount = mark.TotalRankCount( RoomRoundRank_Good );
            resMsg.m_scoreInfo.m_nBadCount = mark.TotalRankCount( RoomRoundRank_Bad );
            resMsg.m_scoreInfo.m_nMissCount = mark.TotalRankCount( RoomRoundRank_Miss );
            resMsg.m_scoreInfo.m_nSpecialCount = mark.MaxSpecialCount();
            resMsg.m_scoreInfo.m_nMyTargetScore = resMsg.m_scoreInfo.m_nMyScore;
            if ( resMsg.m_scoreInfo.m_nTargetType == EDanceGroupChallengeTargetType_Perfect )
            {
                resMsg.m_scoreInfo.m_nMyTargetScore = resMsg.m_scoreInfo.m_nPerfectCount;
            }

            if ( m_nGroupID == 0 || m_pGroupInfo == NULL )
            {
                resMsg.m_nResult = EChallengeEndRes_KickOut;
                break;
            }
            else if ( !mgr.IsOpenNow() )
            {
                resMsg.m_nResult = EChallengeEndRes_Closed;
                break;
            }
            else if ( score == 0 )
            {
                break;
            }

            GameMsg_S2G_ReportChallengeScore s2gMsg;
            s2gMsg.m_nGroupID = m_nGroupID;
            s2gMsg.m_nStartTime = m_nChallengeSaveStartTime;
            s2gMsg.m_nRequestStorey = m_nChallengeSaveStorey;
            s2gMsg.m_scoreInfo = resMsg.m_scoreInfo;

            m_nChallengeSaveStorey = 0;
            m_nChallengeSaveStartTime = 0;

            Send2GroupMsg( s2gMsg );
            return bSuccess;
        } while (0);

        m_nChallengeSaveStorey = 0;
        m_nChallengeSaveStartTime = 0;

        SendPlayerMsg( resMsg );
    }

    return bSuccess;
}


void CEntityDanceGroupComponent::EncodePlayerInfo(BigMamaPlayerInfo &rInfo)
{
    if (m_pRoleAttr == NULL)
        return;

    rInfo.m_nRoleID = m_pRoleAttr->GetRoleID();
    rInfo.m_strRoleName = m_pRoleAttr->GetRoleName();
    rInfo.m_bIsHost = true;
    rInfo.m_chRoleType = RoleRoomType_Dancer;
    rInfo.m_nIsBoss = false;
    rInfo.m_chRolePos = MIN_DANCE_ROOM_POS;
    rInfo.m_chRoleState = RoleRoomState_ToStart;
    rInfo.m_eColorTeam = EDanceColorTeam_None;
    rInfo.m_nRoleSex = (char)m_pRoleAttr->GetSex();
    rInfo.m_nRoleSkin = (char)m_pRoleAttr->GetColor();
    rInfo.m_nMoveType = 0;
    rInfo.m_nTransFormID = (unsigned short)m_pRoleAttr->GetTransformId();
    rInfo.m_bEncodeMedal = true;

    if (m_pGroupInfo != NULL)
    {
        rInfo.m_strDanceGroupName = m_pGroupInfo->m_DGBaseInfo.m_strGroupName;
        rInfo.m_nDanceGroupBadge = m_pGroupInfo->m_DGBaseInfo.m_nBadge;
        rInfo.m_nDanceGroupEffect = m_pGroupInfo->m_DGBaseInfo.m_nEffect;

        CDanceGroupMember *pMember = m_pGroupInfo->GetGroupMember(rInfo.m_nRoleID);
        if (pMember != NULL)
        {
            rInfo.m_nGroupTitle = (char)pMember->m_nTitle;
        }
    }

    if (m_pRoleVIP != NULL)
    {
        rInfo.m_bIsVIP = m_pRoleVIP->IsVIP();
        rInfo.m_nVIPLevel = (unsigned short)m_pRoleVIP->VIPLevel();
    }

    rInfo.m_pItemCOM = m_pRoleItem;
    rInfo.m_pGeneCOM = GetComponent<CRoleGene>();
    rInfo.m_pPetCOM = GetComponent<CRolePet>();
    rInfo.m_pMedalCOM = GetComponent<CRoleMedal>();
}

bool CEntityDanceGroupComponent::EncodeBossInfo(BigMamaPlayerInfo &rInfo, int bossID) const
{
    const BigMamaBossConfig *pBossInfo = g_BigMamaConfigManager.GetBigMamaBossConfig(bossID);
    if (pBossInfo == NULL)
        return false;

    rInfo.m_nRoleID = 0;
    rInfo.m_strRoleName = pBossInfo->m_strName;
    rInfo.m_nRoleSex = pBossInfo->m_cSex;
    rInfo.m_nRoleSkin = (char)pBossInfo->m_cSKinColor;
    rInfo.m_bIsHost = false;
    rInfo.m_chRoleType = RoleRoomType_Boss;
    rInfo.m_nIsBoss = true;
    rInfo.m_chRoleState = RoleRoomState_ToStart;
    rInfo.m_eColorTeam = EDanceColorTeam_None;
    rInfo.m_bEncodeMedal = true;
    rInfo.m_Equips = pBossInfo->m_Equips;

    return true;
}


//FairyLand
//msgreg
void CEntityDanceGroupComponent::RegFairyLandMsg()
{
    if (CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_DanceGroupFairyLand))
    {
        RegMsgCreatorAndProcessor(GameMsg_G2S_FairyLandJoinAndRandomReward, &CEntityDanceGroupComponent::OnG2SSendJoinRandomReward);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandCheckIsInYesterdatRank, &NewRoomManager::FromClientToGroup);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandStorageboxInfo, &NewRoomManager::FromClientToGroup);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandRankConfig, &CEntityDanceGroupComponent::OnGetFairyLandRankRule);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandConfig, &CEntityDanceGroupComponent::OnGetFairyLandConfig);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandChapterInfo, &CEntityDanceGroupComponent::OnGetFairyLandChapterInfo);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandRankInfo, &CEntityDanceGroupComponent::OnGetFairyLandRankInfo);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandGetYesterdayRankReward, &CEntityDanceGroupComponent::OnGetYesterdayRankReward);
        RegMsgCreatorAndProcessor(GameMsg_G2S_FairyLandGetYesterdayRankReward, &CEntityDanceGroupComponent::OnSendYesterdayRankReward);
        
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandOpenBless, &CEntityDanceGroupComponent::OnFairyLandOpenBless);
        RegMsgCreatorAndProcessor(GameMsg_C2S_FairyLandBless, &CEntityDanceGroupComponent::OnFairyLandBless);

        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandCheckIsInYesterdatRank, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandDanceEndInfo, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandRankInfo, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandBless, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandStorageboxInfo, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandOpenBless, &NewRoomManager::FromGroupToClient);

        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandChanllegeTimesNotify, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandBlessNotify, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandCurrentChapterNotify, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandChapterScoreNotify, &NewRoomManager::FromGroupToClient);
        RegMsgCreatorAndProcessor(GameMsg_S2C_FairyLandStateNotify, &NewRoomManager::FromGroupToClient);
        
    }
}

// void CEntityDanceGroupComponent::OnAnnouncementFairyLandEnd(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
// {
// 
// }

void CEntityDanceGroupComponent::OnG2SSendJoinRandomReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_G2S_FairyLandJoinAndRandomReward &rMsg = dynamic_cast<GameMsg_G2S_FairyLandJoinAndRandomReward &>(rMsgBase);

    if (rMsg.m_nIsSendJoin)
    {
        //发送参与奖励
        std::list<CItem> JoinRewardItems;
        int nBindMb = CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardBindMB(rMsg.m_nChapter);
        int nMoney = CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardMoney(rMsg.m_nChapter);
        CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardUnit(rMsg.m_nChapter, m_pRoleAttr->m_nSex, JoinRewardItems);

        m_pRoleAttr->ChangeBindBill(nBindMb,EChangeBindBillCause_Add_FairyLand);
        m_pRoleAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_FairyLand, 0);
        for (std::list<CItem>::iterator it = JoinRewardItems.begin(); it != JoinRewardItems.end(); ++it)
        {
            m_pRoleItem->AddItem(it->m_nItemType, it->m_nItemCount, it->m_nValidTime, EItemAction_Add_FairyLand, 0, true, false);
        }
        
        GameMsg_S2C_FairyLandReward rewardTips;
        rewardTips.m_nRewardType = GameMsg_S2C_FairyLandReward::JOIN_REWARD;
        rewardTips.m_nBindMb = nBindMb;
        rewardTips.m_nMoney = nMoney;
        rewardTips.m_listReward = JoinRewardItems;

        SendPlayerMsg(rewardTips);
    }

    //计算随机奖励
    int nDropId = CDanceGroupFairyLandCfgMgr::Instance().GetOverDropPoolId(rMsg.m_nChapter, m_pRoleAttr->m_nSex);
    CRewardInfo info;
    CDanceGroupFairyLandCfgMgr::Instance().GetDropPoolUnit(nDropId, info);

    GameMsg_S2C_FairyLandRandomReward randreward;
    switch (info.m_nType)
    {
    case eBindMBType:
        randreward.m_nBindMb = info.m_nBindMB;
        m_pRoleAttr->ChangeBindBill(info.m_nBindMB, EChangeBindBillCause_Add_FairyLand);
        break;
    case eMoneyType:
        randreward.m_nMoney = info.m_nMoney;
        m_pRoleAttr->ChangeMoney(info.m_nMoney, EChangeMoneyCause_Add_FairyLand, 0);
        break;
    case eItemType:
        randreward.m_item.Set(info.m_Item.m_nItemId, info.m_Item.m_nCount, info.m_Item.m_nTime);
        m_pRoleItem->AddItem(info.m_Item.m_nItemId, info.m_Item.m_nCount, info.m_Item.m_nTime, EItemAction_Add_FairyLand, 0, true, false);
        break;
    default:
        break;
    }

    SendPlayerMsg(randreward);
}


// void CEntityDanceGroupComponent::OnGetFairyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
// {
//     GameMsg_S2G_FairyLand_GroupInfo msg;
//     msg.m_nGroupId = m_nGroupID;
//     msg.m_nRoleId = m_pRoleAttr->GetRoleID();
//     Send2GroupMsg(msg);
// }

// void CEntityDanceGroupComponent::OnSetFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
// {
//     ENSURE_WITH_LOG_CMD(NULL != m_pRoleAttr, return, "fairyland miss attribute component ptr");
// 
//     if (!m_pRoleAttr->IsGuideFinish(EGuideBit_FairyLandComplete))
//     {
//         m_pRoleAttr->SetGuideFinish(EGuideBit_FairyLandComplete); // 设置完成，然后刷新数据库
//         m_pRoleAttr->FlushGuidData(); // 把引导结果写入
//     }
// }
// 
// void CEntityDanceGroupComponent::OnCheckFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
// {
//     GameMsg_S2C_FairyLandCheckGuide msg;
//     if (!m_pRoleAttr->IsGuideFinish(EGuideBit_FairyLandComplete))
//     {
//         msg.m_bIsComplete = false;
//     }
//     else
//     {
//         msg.m_bIsComplete = true;
//     }
//     SendPlayerMsg(msg);
// }

void CEntityDanceGroupComponent::OnGetFairyLandRankRule(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2C_FairyLandRankConfig msg;
    msg.m_strRankRule = CDanceGroupFairyLandCfgMgr::Instance().GetRankRule();

    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnGetFairyLandConfig(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2C_FairyLandConfig msg;
    msg.m_nTotalChatper = CDanceGroupFairyLandCfgMgr::Instance().GetMaxChapterLevel();
    msg.m_strStorageBoxRule = CDanceGroupFairyLandCfgMgr::Instance().GetStorageBoxRule();
    msg.m_nBlessCost = CDanceGroupFairyLandCfgMgr::Instance().GetBlessPrice();
    msg.m_strBlessRule = CDanceGroupFairyLandCfgMgr::Instance().GetBlessRule();
    msg.m_nDefaultChallengeCount = CVIPDataMgr::Instance().GetDanceGroupFairyLandExChance(0); // 0级 vip 的挑战次数

    SendPlayerMsg(msg);
}

void CEntityDanceGroupComponent::OnGetFairyLandChapterInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_FairyLandChapterInfo& msg = (GameMsg_C2S_FairyLandChapterInfo &)rMsgBase;
    int nChapter = msg.m_nChatper;

    GameMsg_S2C_FairyLandChapterInfo regmsg;
    regmsg.m_nChatper = msg.m_nChatper;
    regmsg.m_nDanceType = (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetChapterDanceType(nChapter);
    regmsg.m_nDifficultyLvl = (unsigned char)CDanceGroupFairyLandCfgMgr::Instance().GetChapterDifficultyLevel(nChapter);
    regmsg.m_nMusicId = (short)CDanceGroupFairyLandCfgMgr::Instance().GetChapterMusicId(nChapter);
    regmsg.m_nJoinMoney = CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardMoney(nChapter);
    regmsg.m_nJoinBindMB = CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardBindMB(nChapter);

    CDanceGroupFairyLandCfgMgr::Instance().GetChapterShowRewardItems(nChapter, m_pRoleAttr->m_nSex, regmsg.m_listShow);
    CDanceGroupFairyLandCfgMgr::Instance().GetJoinRewardUnit(nChapter, m_pRoleAttr->m_nSex, regmsg.m_listJoin);

    SendPlayerMsg(regmsg);
}

void CEntityDanceGroupComponent::OnGetFairyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_C2S_FairyLandRankInfo &msg = (GameMsg_C2S_FairyLandRankInfo &)rMsgBase;


    GameMsg_S2G_FairyLandRankInfo gmsg;
    gmsg.m_nType = msg.m_nType;
    gmsg.m_nRoleId = m_pRoleAttr->m_nRoleID;
    gmsg.m_nGroupId = m_nGroupID;

    ((CRoleEntity*)Entity())->SendMsg2GroupServer(&gmsg);

    return;
}

void CEntityDanceGroupComponent::OnGetYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
//     GameMsg_S2G_FairyLandGetYesterdayRankReward msg;
//     msg.m_nRoleId = m_pRoleAttr->m_nRoleID;
//     msg.m_nGroupId = m_nGroupID;
    Send2GroupMsg(rMsgBase);
}

void CEntityDanceGroupComponent::OnSendYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_G2S_FairyLandGetYesterdayRankReward &msg = (GameMsg_G2S_FairyLandGetYesterdayRankReward &)rMsgBase;
    int nRank = msg.m_nRewardId;

    std::list<CItem> listItems;

    int nBindMb = CDanceGroupFairyLandCfgMgr::Instance().GetRankBindMb(nRank);
    int nMoney = CDanceGroupFairyLandCfgMgr::Instance().GetRankBindMoney(nRank);
    CDanceGroupFairyLandCfgMgr::Instance().GetRankItems(nRank, m_pRoleAttr->m_nSex, listItems);

    GameMsg_S2C_FairyLandReward sendMsg;
    sendMsg.m_nRewardType = GameMsg_S2C_FairyLandReward::YESTERDAY_RANK_REWARD;
    sendMsg.m_nBindMb = nBindMb;
    sendMsg.m_nMoney = nMoney;
    sendMsg.m_listReward = listItems;
    SendPlayerMsg(sendMsg);

    m_pRoleAttr->ChangeBindBill(nBindMb, EChangeBindBillCause_Add_FairyLand);
    m_pRoleAttr->ChangeMoney(nMoney, EChangeMoneyCause_Add_FairyLand, 0);
    m_pRoleItem->AddItem(listItems, EItemAction_Add_FairyLand, 0, true);
}

void CEntityDanceGroupComponent::OnFairyLandOpenBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
//    GameMsg_C2S_FairyLandOpenBless msg;

    unsigned char retCode = GameMsg_S2C_FairyLandOpenBless::Sucess;

    do 
    {
        if (NULL == GetDanceGroupInfo()
            || 0 == GetDanceGroupInfo()->m_DGBaseInfo.m_nGroupID)
        {
            retCode = GameMsg_S2C_FairyLandOpenBless::Sucess;
            break;
        }

    } while (0);

    if (retCode != GameMsg_S2C_FairyLandOpenBless::Sucess)
    {
        GameMsg_S2C_FairyLandOpenBless retMsg;
        retMsg.m_nRetCode = retCode;
        ((CRoleEntity*)Entity())->SendPlayerMsg(&retMsg);
    }
    else
    {
        ((CRoleEntity*)Entity())->SendMsg2GroupServer(&rMsgBase);
    }
}

void CEntityDanceGroupComponent::OnFairyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer)
{
    GameMsg_S2C_FairyLandBless msg;
    int ret = GameMsg_S2C_FairyLandBless::eSuc;

    do 
    {
        if (0 == GetDanceGroupID())
        {
            ret = GameMsg_S2C_FairyLandBless::eNoGroup;
            break;
        }
        int nNeedBill = CDanceGroupFairyLandCfgMgr::Instance().GetBlessPrice();
        
        if (m_pRoleAttr->GetPoint() < (unsigned int)nNeedBill)
        {
            ret = GameMsg_S2C_FairyLandBless::eResourceNotEnough;
            break;
        }

        m_pRoleAttr->ChangeBill(-nNeedBill, EChangeBillCause_Del_FairyLandBless);
        Send2GroupMsg(rMsgBase);

    } while (0);

    if (GameMsg_S2C_FairyLandBless::eSuc != ret)
    {
        GameMsg_S2C_FairyLandBless msg;
        msg.m_nFlag = ret;
        SendPlayerMsg(msg);
    }
}