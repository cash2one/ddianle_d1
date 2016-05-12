#include "../../socket/GameMsg_Map.h"
#include "../../socket/Formatter.h"
#include "EntityActivityComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../logic/EntityComponentEventID.h"
#include "OnlineRewardInfoMgr.h"
#include "../logic/EntityTimerComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../item/EntityItemComponent.h"
#include "../Couple/EntityCoupleComponent.h"
#include "GameMsg_LevelUpReward.h"
#include "../mail/EntityMailComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../mail/MailMgr.h"
#include "LevelUpRewardInfoMgr.h"
#include "../../datastructure/LogicInitData.h"
#include "../util/CommonFunc.h"
#include "GameMsg_S2C_UpdateGiftInfo.h"
#include "GameMsg_C2S_GetGift.h"
#include "GameMsg_S2C_GetGiftResult.h"
#include "GameMsg_S2C_SendGift.h"
#include "../util/GameStringTipsDef.h"
#include "../item/ItemProcess.h"
#include "SystemActivityManager.h"
#include "../../datastructure/Localization.h"
#include "CumulativeRechargeMgr.h"
#include "ExchangeItemInfoMgr.h"
#include "GameMsg_Activity.h"
#include "InTimeOnlineInfoMgr.h"
#include "PlainTextActivityInfoMgr.h"
#include "GiftCodeInfoMgr.h"
#include "XmasFreeRewardDataMgr.h"
#include "FestivalWishDataMgr.h"
#include "FestivalBlessActivityDataMgr.h"
#include "PuzzleDataMgr.h"
#include "InvitationActivityMgr.h"
#include "GameMsg_S2S_Activities.h"
#include "../friend/EntityFriendComponent.h"
#include "../chat/ChatManager.h"
#include "../../datastructure/datainfor/ConfigManager.h"
#include "../SystemSetting/SystemSettingMgr.h"
#include "FresherActivityMgr.h"
#include "ExtraActivityInfoMgr.h"
#include "MeetActivityMgr.h"
#include "GlobalActivityData.h"
#include "../player/GameMsg_Buff.h"
#include "ProprietaryActivity.h"
#include "MallShareMgr.h"
#include "SocialShareMgr.h"
#include "CumulativeSpendMgr.h"
#include "BuyItemGotMedalMgr.h"
#include "../quest/EntityQuestComponent.h"
#include "../vip/EntityVIPComponent.h"
#include "../quest/EntityQuestNewComponent.h"
#include "../Medal/EntityMedalComponent.h"
#include "LongactingCumulativeRechareMgr.h"
#include "../../datastructure/LogStructDef.h"
#include "./RedEnvelope/GameMsg_CS_RedEnvelope.h"
#include "./RedEnvelope/RedEnvelopeActivityManager.h"
#include "./RedEnvelope/GameMsg_GS_RedEnvelope.h"
#include "../dancegroup/EntityDanceGroupComponent.h"
#include "RedEnvelope/RedEnvelopeConfigManager.h"
#include "EffectStoneExchange/EffectStoneExchangeManager.h"

#ifdef WIN32
#pragma warning(disable:4702)
#endif

void SendMsg2GroupServer(LPGameMsg_Base pMsg);

const unsigned long kUpdateInterval = 1000;
const unsigned long kUpdateKeepOnlineRewardDBInterval = 3 * 60 * 1000;

CEntityActivityComponent::CEntityActivityComponent()
: CEntityComponent(true, true)
, m_pRoleNet(NULL)
, m_pRoleTimer(NULL)
, m_pRoleAttr(NULL)
, m_pRoleItem(NULL)
, m_pRoleMail(NULL)
, m_pRoleFriend(NULL)
, m_pRoleChat(NULL)
, m_pRoleCouple(NULL)
, m_pRoleFresherQuest(NULL)
, m_pRoleVip(NULL)
, m_pRoleQuest(NULL)
, m_pRoleMedal(NULL)
, m_pRoleDanceGroup(NULL)
, m_nLastStop(0)
, m_nTimeElapsed(0)
, m_nUpdateDBTimeElapse(0)
, m_nCumulativeRechargeNum(0)
, m_bLoadCumulativeRechargeDB(false)
, m_nCumulativeBill(0)
, m_nCumulativeBindBill(0)
, m_bLoadCumulativeSpendSuc(false)
, m_nCumulativeBillForMedal(0)
, m_nCumulativeBindBillForMedal(0)
, m_bCheckPuzzleOpen(false)
, m_nInvitationBindCount(0)
, m_nInvitationQueryTime(0)
, m_nFresherDay(0)
, m_nCurrentFresherDay(0)
, m_bProprietaryReward(false)
, m_bNotGetPhotoShareVIPReward(false)
, m_nLongactingCumulativeRechargeIndex(0)
, m_bHaveExchangeItem(false)
{
    m_vecCumulativeRewards.assign(CUMULATIVE_REWARD_LEVEL, 0);

    m_mapSocialShareActivity2UI[EActivity_MallSocialShare] = EUISocialShare_Mall;
    m_mapSocialShareActivity2UI[EActivity_PhotoSocialShare] = EUISocialShare_Photo;
}

CEntityActivityComponent::~CEntityActivityComponent()
{
}

void CEntityActivityComponent::OnEvent(CComponentEvent& refEvent)
{
    time_t nNow = time(NULL);
    if (refEvent.nID == eComponentEvent_NormalLogin || refEvent.nID == eComponentEvent_FirstLogin || refEvent.nID == eComponentEvent_TodayFirstLogin) {
        if (m_pRoleTimer != NULL) {
            m_pRoleTimer->AddTimer((unsigned int)COnlineRewardInfoMgr::Instance().GetResetTime(nNow), eComponentEvent_KeepOnlineReward_Reset, 0);
        }
    }
    else if (refEvent.nID == eComponentEvent_KeepOnlineReward_Finished) {
        m_keepOnlineRewardInfo.m_nCumulativeTime = m_keepOnlineRewardInfo.m_nRequireTime;
        m_keepOnlineRewardInfo.m_bFinished = true;
        _UpdateOnlineRewardInfoDB();
    }
    else if (refEvent.nID == eComponentEvent_KeepOnlineReward_Reset) {
        _OnOnlineRewardActivityReset();
        m_pRoleTimer->AddTimer((unsigned int)COnlineRewardInfoMgr::Instance().GetResetTime(nNow), eComponentEvent_KeepOnlineReward_Reset, 0);
    }
}

void CEntityActivityComponent::Start()
{
    m_pRoleNet = GetComponent<CRoleNet>();
    m_pRoleTimer = GetComponent<CRoleTimer>();
    m_pRoleAttr = GetComponent<CRoleAttribute>();
    m_pRoleItem = GetComponent<CRoleItem>();
    m_pRoleMail = GetComponent<CRoleMail>();
    m_pRoleFriend = GetComponent<CRoleFriend>();
    m_pRoleChat = GetComponent<CRoleChat>();
    m_pRoleCouple = GetComponent<CRoleCouple>();
    m_pRoleFresherQuest = GetComponent<CRoleQuest>();
    m_pRoleVip = GetComponent<CRoleVIP>();
    m_pRoleQuest = GetComponent<CRoleQuestNew>();
    m_pRoleMedal = GetComponent<CRoleMedal>();
    m_pRoleDanceGroup = GetComponent<CRoleDanceGroup>();

    m_pRoleItem->AddSubscriber(this);
}

void CEntityActivityComponent::OnLogin()
{
    //礼品查询
    //std::map< int, GiftDB >* pGiftMap = new std::map< int, GiftDB >();
    //CDelHelper delHelper(pGiftMap);
    //AddQuery(QUERY_Activity_LoadGift, m_pRoleAttr->GetAccount(), pGiftMap, 0, NULL, &delHelper);

    if (CCumulativeRechargeMgr::Instance().IsOpening())
    {
        _LoadCumulativeRechargeInfo();
    }

    if (CXmasFreeRewardDataMgr::Instance().IsOpening())
    {
        _LoadXmasFreeRewardInfo();
    }

    if (CFestivalBlessActivityDataMgr::Instance().IsBlessOpening())
    {
        _LoadRoleBlessActivityInfo();
    }

    if (CFestivalWishDataMgr::Instance().IsOpening())
    {
        _LoadFestivalWishInfo();
    }

    //if (CPuzzleDataMgr::Instance().IsOpening() )
    //{
    //    _LoadRolePuzzleDB();
    //}
    //else
    //{
    //    m_bCheckPuzzleOpen = true;
    //}

    if (CCumulativeSpendMgr::Instance().IsOpening())
    {
        _LoadCumulativeSpendInfo();
   
    }

    if (CumulativeSpendGotMedalMgr::Instance().HasOpening())
    {
        _LoadCumulativeSpendInfoForMedal();
        LoadCumulativeSpendGotMedalReward();
    }

    if (BuyItemGotMedalMgr::Instance().HasOpening())
    {
        LoadBuyItemGotMedalRewardedInfo();
    }

    // initialize the fresher day
    if (NULL != m_pRoleAttr)
    {
        m_nFresherDay = DiffDays(time(NULL), m_pRoleAttr->m_nRoleCreateTime) + 1;

        if (m_nFresherDay <= 0)
        {
            WriteLog(LOGLEVEL_WARNING, "roleid=%u fresherDay=%d roleCreateTime=%u",
                m_pRoleAttr->GetRoleID(), m_nFresherDay, m_pRoleAttr->m_nRoleCreateTime);
            m_nFresherDay = 1;
        }
    }

    // send social share activities 'can first' to client
    SendAllShareActivitiesCanFirst();
    SyncPuzzleData();
    SyncGiftData();

    // effectstone exchange
    if (EffectStoneExchangeManager::Instance().IsActivityOpen())
    {
        ExchangeEffectStone();
    }
}

void CEntityActivityComponent::OnLogout()
{
    if (m_keepOnlineRewardInfo.m_bValid)
    {
        _UpdateOnlineRewardInfoDB();
    }

    // 邂逅
    CMeetActivityMgr::Instance().Logout(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex());
}

void CEntityActivityComponent::SerializeComponent(CParamPool &IOBuff)
{
}

void CEntityActivityComponent::OnUpdate(const unsigned long &nTimeElapsed)
{
    unsigned int nNow = (unsigned int)time(NULL);
    m_nTimeElapsed += nTimeElapsed;
    m_nUpdateDBTimeElapse += nTimeElapsed;

    if (m_nTimeElapsed > kUpdateInterval)
    {
        if (m_keepOnlineRewardInfo.m_bValid && !m_keepOnlineRewardInfo.m_bFinished
            && m_keepOnlineRewardInfo.m_nRequireTime - m_keepOnlineRewardInfo.m_nCumulativeTime > 0)
        {
            m_keepOnlineRewardInfo.m_nCumulativeTime += (m_nTimeElapsed / kUpdateInterval);
        }
        m_nTimeElapsed %= kUpdateInterval;
    }
    if (m_nUpdateDBTimeElapse != 0 && m_nUpdateDBTimeElapse > kUpdateKeepOnlineRewardDBInterval)
    {
        if (m_keepOnlineRewardInfo.m_bValid)
        {
            _UpdateOnlineRewardInfoDB();
        }
        m_nUpdateDBTimeElapse -= kUpdateKeepOnlineRewardDBInterval;
    }

    bool bCheckReset = (GetLocalHourOfDay(m_nLastStop) == 23 && GetLocalHourOfDay(nNow) == 0);
    if (bCheckReset && CXmasFreeRewardDataMgr::Instance().IsOpening())
    {
        m_roleXmasFreeRewardInfo.Reset();
        _SaveXmasFreeRewardInfo(0);
        _SendXmasFreeRewardActivityInfo();
    }

    if (bCheckReset && CFestivalWishDataMgr::Instance().IsOpening())
    {
        m_roleFestivalWishActivityInfo.Reset();
        _SaveFestivalWishInfo();
        _SendFestivalWishActivityInfo();
    }

    if (m_bCheckPuzzleOpen && CPuzzleDataMgr::Instance().IsOpening())
    {
        m_bCheckPuzzleOpen = false;
        _InitRolePuzzleInfo(NULL);
    }

    if (bCheckReset)
    {
        SendCanSocialShareFirst(EUISocialShare_Normal, true);
    }

    m_nLastStop = nNow;
}

void CEntityActivityComponent::RegComponentNetMsgMap()
{
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetOnlineReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetGift);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetAllActivitiesInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCDKeyInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetBindActivationInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCumulativeRechargeReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetProprietaryReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_ExchangeItem);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetXmasFreeRewardActivityInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetXmasFreeReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFestivalWishInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_FestivalWish);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFestivalBlessActivityInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BlessFriend);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFestivalBlessReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_FillPuzzle);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetInvitationInviterInfo);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_BindInvitationCode);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetInvitationInviterReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetFresherReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetMatch);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetCancelMatch);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetLeaveScene);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetCameraEvent);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetEnd);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_MeetEnterAmuseScene);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCumulativeSpendReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_RequestVIPExtraReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLongactingCumulativeRecharge);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLongactingCumulativeRechargeReward);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast);


    RegMsgProcessor(MSG_C2S_GetOnlineReward, &CEntityActivityComponent::_OnGetOnlineReward);
    RegMsgProcessor(MSG_C2S_GetGift, &CEntityActivityComponent::_OnGetGift);
    RegMsgProcessor(MSG_C2S_GetAllActivitiesInfo, &CEntityActivityComponent::_OnGetAllActivitiesInfo);
    RegMsgProcessor(MSG_C2S_GetCDKeyInfo, &CEntityActivityComponent::GetCDKeyInfo);
    RegMsgProcessor(MSG_C2S_GetBindActivationInfo, &CEntityActivityComponent::GetBindActivationInfo);
    RegMsgProcessor(MSG_C2S_GetCumulativeRechargeReward, &CEntityActivityComponent::_OnGetCumulativeRechargeReward);
    RegMsgProcessor(MSG_C2S_GetCumulativeSpendReward, &CEntityActivityComponent::OnGetCumulativeSpendReward);
    RegMsgProcessor(MSG_C2S_GetProprietaryReward, &CEntityActivityComponent::_OnGetProprietaryReward);
    RegMsgProcessor(MSG_C2S_ExchangeItem, &CEntityActivityComponent::_OnExchangeItem);
    RegMsgProcessor(MSG_C2S_GetXmasAcitivyInfo, &CEntityActivityComponent::_OnGetXmasActivityInfo);
    RegMsgProcessor(MSG_C2S_GetFestivalWishInfo, &CEntityActivityComponent::_OnGetFestivalWishActivityInfo);
    RegMsgProcessor(MSG_C2S_GetXmaxFreeReward, &CEntityActivityComponent::_OnGetXmasFreeReward);
    RegMsgProcessor(MSG_C2S_FestivalWish, &CEntityActivityComponent::_OnRoleFestivalWish);
    RegMsgProcessor(MSG_C2S_GetFestivalBlessActivityInfo, &CEntityActivityComponent::_OnGetFestivalBlessActivityInfo);
    RegMsgProcessor(MSG_C2S_BlessFriend, &CEntityActivityComponent::_BlessFriend);
    RegMsgProcessor(MSG_C2S_GetFestivalBlessReward, &CEntityActivityComponent::_GetFestivalBlessReward);
    RegMsgProcessor(MSG_C2S_FillPuzzle, &CEntityActivityComponent::_OnFillPuzzle);
    RegMsgProcessor(MSG_C2S_GetInvitationInviterInfo, &CEntityActivityComponent::_OnGetInvitationInviterInfo);
    RegMsgProcessor(MSG_C2S_BindInvitationCode, &CEntityActivityComponent::_OnBindInvitationCode);
    RegMsgProcessor(MSG_C2S_GetInvitationInviterReward, &CEntityActivityComponent::_OnGetInviterCumulativeReward);
    RegMsgProcessor(MSG_C2S_GetFresherReward, &CEntityActivityComponent::_OnGetFresherReward);
    RegMsgProcessor(MSG_C2S_MeetMatch, &CEntityActivityComponent::_OnMeetMatch);
    RegMsgProcessor(MSG_C2S_MeetCancelMatch, &CEntityActivityComponent::_OnMeetCancelMatch);
    RegMsgProcessor(MSG_C2S_MeetLeaveScene, &CEntityActivityComponent::_OnMeetLeaveScene);
    RegMsgProcessor(MSG_C2S_MeetCameraEvent, &CEntityActivityComponent::_OnMeetCameraEvent);
    RegMsgProcessor(MSG_C2S_MeetEnd, &CEntityActivityComponent::_OnMeetEnd);
    RegMsgProcessor(MSG_C2S_MeetEnterAmuseScene, &CEntityActivityComponent::_OnMeetEnterAmuseScene);
    RegMsgProcessor(MSG_C2S_RequestVipExtraReward, &CEntityActivityComponent::_OnRequestVipExtraReward);
    RegMsgProcessor(MSG_C2S_GetLongactingCumulativeRecharge, &CEntityActivityComponent::OnGetLongactingCumulativeRechargeInfo);
    RegMsgProcessor(MSG_C2S_GetLongactingCumulativeRechargeReward, &CEntityActivityComponent::OnGetLongactingCumulativeReward);
    RegMsgProcessor(MSG_C2S_GetLongactingCumulativeRechargeBroadcase, &CEntityActivityComponent::OnGetLongactingCumulativeBoardCast);

    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_BlessFriend);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetLongactingRechargeBoardCast);

    RegMsgProcessor(MSG_G2S_BlessFriend, &CEntityActivityComponent::_BlessFriend_G);
    RegMsgProcessor(MSG_G2S_GetLongactingRechargeBroadCase, &CEntityActivityComponent::_OnGetLongactingCumulativeBoardCast);

    RegMsgCreatorAndProcessor(GameMsg_C2S_GetExtraActivityInfo, &CEntityActivityComponent::_OnGetExtraActivityInfo);
    RegMsgCreatorAndProcessor(GameMsg_C2S_SocialShare, &CEntityActivityComponent::_OnSocialShare);
    RegMsgCreatorAndProcessor(GameMsg_C2S_ActivitySocialShare, &CEntityActivityComponent::_OnActivitySocialShare);

    RegMsgCreatorAndProcessor(GameMsg_C2S_GetCumulativeSpendGotMedalReward, &CEntityActivityComponent::OnGetCumulativeSpendGotMedalReward);
    RegMsgCreatorAndProcessor(GameMsg_C2S_RequestBuyItemGotMedalReward, &CEntityActivityComponent::OnRequestBuyItemGotMedalReward);

    RegQueryProcessor(QUERY_Activity_LoadGift, &CEntityActivityComponent::_OnLoadGift);
    RegQueryProcessor(QUERY_LoadCumulativeRechargeInfo, &CEntityActivityComponent::_OnLoadCumulativeRechargeInfo);
    RegQueryProcessor(QUERY_LoadCumulativeSpendInfo, &CEntityActivityComponent::_OnLoadCumulativeSpendInfo);
    RegQueryProcessor(QUERY_LoadCumulativeSpendInfoForMedal, &CEntityActivityComponent::_OnLoadCumulativeSpendInfoForMedal);
    RegQueryProcessor(QUERY_LoadCumulativeSpendGotMedalReward, &CEntityActivityComponent::OnQueryCumulativeSpendGotMedalReward);
    RegQueryProcessor(QUERY_LoadXmasFreeRewardInfo, &CEntityActivityComponent::_OnDBLoadXmasFreeRewardInfo);
    RegQueryProcessor(QUERY_LoadRoleBlessActivityInfo, &CEntityActivityComponent::_OnDBLoadRoleBlessActivityInfo);
    RegQueryProcessor(QUERY_LoadFestivalWishInfo, &CEntityActivityComponent::_OnDBLoadRoleFestivalWishActivityInfo);
    RegQueryProcessor(QUERY_Puzzle_LoadPicture, &CEntityActivityComponent::_OnDBLoadRolePuzzleActivityInfo);
    RegQueryProcessor(QUERY_Invitation_CheckInvitationCode, &CEntityActivityComponent::_OnDBCheckInvitationCode);
    RegQueryProcessor(QUERY_Invitation_GetInvitationInfo, &CEntityActivityComponent::_OnDBGetInvitationInfo);
    RegQueryProcessor(QUERY_LoadBuyItemGotMedalRewardedInfo, &CEntityActivityComponent::OnQueryBuyItemGotMedalRewardedInfo);

    // 舞团红包
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRedEnvelopeList);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetRedEnvelopeDetails);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_SetAnonymity);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_OpenRedEnvelope);
    GAMEMSG_REGISTERCREATOR(GameMsg_C2S_GetCanOpenRedEnvelopeCount);

    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRedEnvelopeList);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_NewRedEnvelope);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetRedEnvelopeDetails);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_OpenRedEnvelope);
    GAMEMSG_REGISTERCREATOR(GameMsg_G2S_GetCanOpenRedEnvelopeCount);

    RegMsgProcessor(MSG_C2S_GetRedEnvelopeList, &CEntityActivityComponent::OnGetRedEnvelopeList);
    RegMsgProcessor(MSG_C2S_GetRedEnvelopeDetailes, &CEntityActivityComponent::OnGetRedEnvelopeDetails);
    RegMsgProcessor(MSG_C2S_SetAnonymity, &CEntityActivityComponent::OnSetAnonymity);
    RegMsgProcessor(MSG_C2S_OpenRedEnvelope, &CEntityActivityComponent::OnOpenRedEnvelope);
    RegMsgProcessor(MSG_C2S_GetCanOpenRedEnevlopeCount, &CEntityActivityComponent::OnGetCanOpenRedEnvelopeCount);


    RegMsgProcessor(MSG_G2S_GetRedEnvelopeList, &CEntityActivityComponent::_OnGetRedEnvelopeList);
    RegMsgProcessor(MSG_G2S_GetRedEnvelopeDetails, &CEntityActivityComponent::_OnGetRedEnvelopeDetails);
    RegMsgProcessor(MSG_G2S_NewRedEnvelope, &CEntityActivityComponent::OnNewRedEnvelopeNotice);
    RegMsgProcessor(MSG_G2S_OpenRedEnvelope, &CEntityActivityComponent::_OnOpenRedEnvelope);
    RegMsgProcessor(MSG_G2S_GetCanOpenRedEnvelopeCount, &CEntityActivityComponent::_OnGetCanOpenRedEnvelopeCount);
}

bool CEntityActivityComponent::CreateFromDB(ROLEINFO_DB* pRoleInfoDB)
{
    unsigned int nNow = (unsigned int)time(NULL);
    m_nLastStop = nNow;

    if (pRoleInfoDB != NULL)
    {
        m_FresherRewardDays = pRoleInfoDB->m_setFresherReward;
        m_keepOnlineRewardInfo = pRoleInfoDB->keepOnlineRewardInfo;
        m_bProprietaryReward = pRoleInfoDB->m_bPropretaryActivityReward;

        if (_OnlineRewardActivityActivated())
        {
            if (m_keepOnlineRewardInfo.m_nIndex > 0)
            {
                if (GetLocalDays(nNow) == GetLocalDays(m_keepOnlineRewardInfo.m_nTimingDay) &&
                    COnlineRewardInfoMgr::Instance().IsTimeInCurActivityTimeRange(m_keepOnlineRewardInfo.m_nTimingDay))
                {// the same day and must be in current ativity time range
                    if (!m_keepOnlineRewardInfo.m_bGotReward)
                    {
                        COnlineRewardInfo *pInfo = COnlineRewardInfoMgr::Instance().GetOnlineRewardInfo(m_keepOnlineRewardInfo.m_nIndex);
                        if (pInfo == NULL) 
                        {
                            _ResetKeepOnlineRewardInfo();
                        }
                        else
                        {	// the reward timer exists
                            if (m_pRoleTimer != NULL) 
                            {
                                m_keepOnlineRewardInfo.m_nRequireTime = pInfo->m_nRequireTime;
                                _HandleOnlineInfo(m_keepOnlineRewardInfo, nNow);
                            }
                        }
                    }
                    else
                    {	// got reward
                        m_keepOnlineRewardInfo.m_bValid = false;
                        _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo, false);
                        if (m_keepOnlineRewardInfo.m_bValid) 
                        {
                            _UpdateOnlineRewardInfoDB();
                        }
                    }
                }
                else
                {	// not the same day
                    _ResetKeepOnlineRewardInfo();
                    _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo);
                    if (m_keepOnlineRewardInfo.m_bValid)
                    {
                        _UpdateOnlineRewardInfoDB();
                    }
                }
            }
            else
            {	// does not have any recored
                _ResetKeepOnlineRewardInfo();
                _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo);
                if (m_keepOnlineRewardInfo.m_bValid) 
                {
                    _UpdateOnlineRewardInfoDB();
                }
            }
        }
        else
        {
            _ResetKeepOnlineRewardInfo();
        }
        //社交分享
        for (std::map<int, unsigned int>::iterator it = pRoleInfoDB->m_mapSocialShareFirstTime.begin(); it != pRoleInfoDB->m_mapSocialShareFirstTime.end(); ++it)
        {
            m_mapSocialShareRecord[(EUISocialShare)it->first] = (time_t)it->second;
        }

        // 长效累冲
        m_nLongactingCumulativeRechargeIndex = pRoleInfoDB->m_nLongactingCumulativeRechargeIndex;

        // 舞团红包
        RedEnvelopeActivityManager::Instance().SetAnonymity(pRoleInfoDB->m_bRedEnvelopeAnonymity);

        // 拼图
        _InitRolePuzzleInfo(pRoleInfoDB->m_listPuzzleActivityDB);

        // 礼品
        m_GiftMap.swap(pRoleInfoDB->m_mapGift);
    }

    return true;
}

bool CEntityActivityComponent::PacketToDB(ROLEINFO_DB *pRoleInforDB) const
{
    return true;
}

bool CEntityActivityComponent::PacketToCache( ROLEINFO_DB* pRoleInforDB ) const
{
    if ( pRoleInforDB == NULL )
        return false;

    pRoleInforDB->m_mapSocialShareFirstTime.clear();
    for ( std::map<EUISocialShare, time_t>::const_iterator shareItr = m_mapSocialShareRecord.begin();
        shareItr != m_mapSocialShareRecord.end(); ++shareItr )
    {
        pRoleInforDB->m_mapSocialShareFirstTime.insert( std::make_pair( (int)shareItr->first, (unsigned int)shareItr->second ) );
    }

    pRoleInforDB->m_listPuzzleActivityDB.clear();
    for ( std::map<int, CRolePuzzleActivityInfo>::const_iterator puzzleItr = m_rolePuzzleActivityInfo.begin();
        puzzleItr != m_rolePuzzleActivityInfo.end(); ++puzzleItr )
    {
        const CRolePuzzleActivityInfo &info = puzzleItr->second;
        if ( info.m_nUpdateTime == 0 )
            continue;

        CRolePuzzleActivityDB puzzleDB;
        puzzleDB.m_nPuzzleID = info.m_nPuzzleID;
        puzzleDB.m_nUpdateTime = info.m_nUpdateTime;
        puzzleDB.m_nFinishTime = info.m_nFinishTime;
        strcpy( puzzleDB.m_arFragments, info.m_arFragments );

        pRoleInforDB->m_listPuzzleActivityDB.push_back( puzzleDB );
    }

    pRoleInforDB->keepOnlineRewardInfo = m_keepOnlineRewardInfo;
    pRoleInforDB->m_mapGift = m_GiftMap;

    return true;
}

void CEntityActivityComponent::_OnGetOnlineReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    if (_OnlineRewardActivityActivated())
    {
        COnlineRewardInfo *pInfo = COnlineRewardInfoMgr::Instance().GetOnlineRewardInfo(m_keepOnlineRewardInfo.m_nIndex);
        if (m_pRoleAttr != NULL && m_keepOnlineRewardInfo.m_bValid && m_keepOnlineRewardInfo.m_bFinished && pInfo != NULL)
        {
            _SendPlayerReward(m_keepOnlineRewardInfo.m_nIndex);

            GameMsg_S2C_GetOnlineRewardSuc msgSuc;
            msgSuc.m_nCurIndex = (unsigned short)m_keepOnlineRewardInfo.m_nIndex;

            m_keepOnlineRewardInfo.m_bGotReward = true;
            _UpdateOnlineRewardInfoDB();

            m_keepOnlineRewardInfo.m_bValid = false;
            _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo, false);
            
            if (m_keepOnlineRewardInfo.m_bValid)
            {
                _UpdateOnlineRewardInfoDB();
            }
           
            if (!(COnlineRewardInfoMgr::Instance().IsEndActivityIndex(m_keepOnlineRewardInfo.m_nIndex) && m_keepOnlineRewardInfo.m_bGotReward))
            {
                msgSuc.m_NextRewardInfo.m_onlineRewardInfo = m_keepOnlineRewardInfo;
            }

            _SendPlayerMsg(&msgSuc);
        }
        else
        {
            GameMsg_S2C_GetOnlineRewardFail msgFail;
            msgFail.m_strError = "Get_Online_Reward_Fail";
            _SendPlayerMsg(&msgFail);
        }
    }
    else
    {
        GameMsg_S2C_GetOnlineRewardFail msgFail;
        msgFail.m_strError = "Get_Online_Reward_Fail_ActivityNotOpen";
        _SendPlayerMsg(&msgFail);
    }
}

//获取礼品
void CEntityActivityComponent::_OnGetGift(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetGift * pMsg = (GameMsg_C2S_GetGift *)&msg;
    GameMsg_S2C_GetGiftResult resultMsg(pMsg->m_nIndexID, EGetGiftResult_Success);
    if (m_pRoleAttr == NULL)
    {
        resultMsg.m_nResult = EGetGiftResult_Error;
        _SendPlayerMsg(&resultMsg);
        return;
    }
    std::map< int, GiftDB >::iterator it = m_GiftMap.find(pMsg->m_nIndexID);
    if (it == m_GiftMap.end())
    {
        resultMsg.m_nResult = EGetGiftResult_DoNotHave;
        _SendPlayerMsg(&resultMsg);
        return;
    }
    time_t tNow = time(NULL);
    GiftDB & rGiftDB = it->second;
    if (rGiftDB.m_nExpiredTime < (unsigned int)tNow)
    {
        resultMsg.m_nResult = EGetGiftResult_Expired;
        _SendPlayerMsg(&resultMsg);
        return;
    }
    CGiftInfo * pGiftInfo = CSystemActivityManager::Instance().GiftSystem().GetGiftInfo(rGiftDB.m_nGiftID);
    if (pGiftInfo == NULL)
    {
        resultMsg.m_nResult = EGetGiftResult_GiftEmpty;
        _SendPlayerMsg(&resultMsg);
        return;
    }
    pGiftInfo->SendGiftToRole(*(CRoleEntity *)Entity());
    AddQuery(QUERY_Activity_GetGiftReward, pMsg->m_nIndexID);
    m_GiftMap.erase(it);
    _SendPlayerMsg(&resultMsg);
}

void CEntityActivityComponent::SendAllActivitiesInfo()
{
    GameMsg_S2C_GetAllActivitiesInfoSuc msgSuc;
    std::string strError = "";
    if (CCumulativeRechargeMgr::Instance().IsOpening()) {
        _SetCumulativeRechargeActivity(msgSuc.m_CumulativeRechargeActivity);
    }

    if (CExchangeItemInfoMgr::Instance().IsOpening()) {
        _SetExchangeItemActivity(msgSuc.m_ExchangeItemActivity);
    }

    if (CInTimeOnlineInfoMgr::Instance().IsShowOpening()) {
        _SetInTimeOnlineActivity(msgSuc.m_ShowInTimeOnlineActivity);
    }

    if (CPlainTextActivityInfoMgr::Instance().IsOpening()) {
        _SetPlainTextActivity(msgSuc.m_plainTextActivity);
    }

    if (CGiftCodeInfoMgr::Instance().IsShowing()) {
        _SetGiftCodeActivity(msgSuc.m_GiftCodeActivity);
    }

    if (COnlineRewardInfoMgr::Instance().IsShowing()) {
        _SetOnlineRewardActivity(msgSuc.m_OnlineRewardActivity);
    }

    if (CInvitationActivityMgr::Instance().IsActivityOpening())
    {
        _SetInvitationActivity(msgSuc.m_InvitationActivity);
    }

    if (CCumulativeSpendMgr::Instance().IsOpening())
    {
        _SetCumulativeSpendActivity(msgSuc.m_CumulativeSpendActivity);
    }
    _SetFresherActivity(msgSuc.m_FresherActivity);

    if (CSocialShareMgr::Instance().IsShowNow(EActivity_MallSocialShare))
    {
        _SetMallSocialShareActivity(msgSuc.m_MallActivity);
    }
    if (CSocialShareMgr::Instance().IsShowNow(EActivity_PhotoSocialShare))
    {
        _SetPhotoSocialShareActivity(msgSuc.m_PhotoActivity);
    }
    msgSuc.m_bFemale = (ESexType_Female == m_pRoleAttr->GetSex());

    if (CProprietaryActivityInfoMgr::Instance().IsOpening())
    {
        _SetProprietaryAcitivty(msgSuc.m_proprietaryActivity);
    }

    if (CumulativeSpendGotMedalMgr::Instance().HasOpening())
    {
        SetCumulativeSpendGotMedalActivity(msgSuc.m_cumulativeSpendGotMedalActivity);
    }

    if (BuyItemGotMedalMgr::Instance().HasOpening())
    {
        SetBuyItemGotMedalActivity(msgSuc.m_buyItemGotMedalActivity);
    }

    if (RedEnvelopeActivityManager::Instance().IsActivityOpen())
    {
        RedEnvelopeConfigManager::Instance().GetRedEnvelopeConfig(msgSuc.m_redenvelopeActivity);
    }

    if (m_bHaveExchangeItem)
    {
        EffectStoneExchangeManager::Instance().GetEffectStoneExchangeConfig(msgSuc.m_effectStoneExchangeActivity);
        msgSuc.m_effectStoneExchangeActivity.m_listExchangeItem = m_listExchangeItem;
        m_listExchangeItem.clear();
        m_bHaveExchangeItem = false;
    }

    if (!strError.empty())
    {
        GameMsg_S2C_GetAllActivitiesInfoFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);
    }
    else
    {
        _SendPlayerMsg(&msgSuc);
    }
}

void CEntityActivityComponent::_OnGetAllActivitiesInfo(GameMsg_Base & msg, CSlotPeer & slotPeer)
{
    SendAllActivitiesInfo();
}

void CEntityActivityComponent::GetBindActivationInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetBindActivationInfoSuc msgSuc;
    if (CInvitationActivityMgr::Instance().IsActivityOpening())
    {
        _SetInvitationActivity(msgSuc.m_InvitationActivity);
    }

    _SendPlayerMsg(&msgSuc);
}

void CEntityActivityComponent::GetCDKeyInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetCDKeyInfoSuc msgSuc;

    if (CGiftCodeInfoMgr::Instance().IsShowing())
    {
        _SetGiftCodeActivity(msgSuc.m_GiftCodeActivity);
    }

    _SendPlayerMsg(&msgSuc);
}

void CEntityActivityComponent::_OnGetCumulativeRechargeReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetCumulativeRechargeReward & msgGetReward = (GameMsg_C2S_GetCumulativeRechargeReward &)msg;

    std::string strError = "";
    if (!CCumulativeRechargeMgr::Instance().IsOpening())
    {
        strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String1");
    }
    else if (!CCumulativeRechargeMgr::Instance().IsValidLevel(msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String3");
    }
    else if (_HasReceivedCumulativeReward(msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String4");
    }
    else if (!_ReachCumulativeRechargeRequire(m_nCumulativeRechargeNum, msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String5");
    }
    else
    {
        if (m_pRoleAttr != NULL && m_pRoleItem != NULL)
        {
            std::list<CItem> listReward;
            const CCumulativeReward *pRechargeInfo = CCumulativeRechargeMgr::Instance().GetRewardInfo(msgGetReward.m_nLevel);

            if (pRechargeInfo != NULL)
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    listReward = pRechargeInfo->m_listMaleReward;
                }
                else if (m_pRoleAttr->GetSex() == ESexType_Female)
                {
                    listReward = pRechargeInfo->m_listFemaleReward;
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "Get RoleSex Error, roleid(%u)\n", m_pRoleAttr->GetRoleID());
                }

                if (CItemProcess::AddItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_CumulativeRecharge, 0, true))
                {
                    m_pRoleAttr->ChangeMoney(pRechargeInfo->m_nMoney, EChangeMoneyCause_Add_Cumulative_Rechrge, 0);
                    m_pRoleAttr->ChangeBindBill(pRechargeInfo->m_nBindMCoin, EChangeBindBillCause_Add_CumulativeRechargeReward);
                    m_vecCumulativeRewards[msgGetReward.m_nLevel - 1] = 1;

                    std::vector<int> * pVecReward = new std::vector<int>(m_vecCumulativeRewards);
                    CDelHelper delHelper(pVecReward);
                    AddQuery(QUERY_SaveCumulativeRecharge, m_pRoleAttr->GetRoleID(), pVecReward, time(NULL), NULL, &delHelper);
                    GameMsg_S2C_GetCumulativeRechargeRewardSuc msgSuc;
                    msgSuc.m_nLevel = msgGetReward.m_nLevel;
                    _SendPlayerMsg(&msgSuc);
                }
                else
                {
                    strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String6");
                }
            }
        }
    }

    if (strError != "")
    {
        GameMsg_S2C_GetCumulativeRechargeRewardFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::AddCumulativeSpendBill(int nValue)
{
    m_nCumulativeBill += nValue;
    m_nCumulativeBillForMedal += nValue;

    int nSpendType = CCumulativeSpendMgr::Instance().GetCurrencyType();
    if (nSpendType == 1)
    {
        GameMsg_S2C_UpdateCumulativeSpendNum updatemsg;
        updatemsg.m_nSpendNum = m_nCumulativeBill;
        _SendPlayerMsg(&updatemsg);
    }
    else
    {
        GameMsg_S2C_UpdateCumulativeSpendNum updatemsg;
        updatemsg.m_nSpendNum = m_nCumulativeBill + m_nCumulativeBindBill;
        _SendPlayerMsg(&updatemsg);
    }
}

void CEntityActivityComponent::AddCumulativeSpendBindBill(int nValue)
{
    m_nCumulativeBindBill += nValue;
    m_nCumulativeBindBillForMedal += nValue;

    int nSpendType = CCumulativeSpendMgr::Instance().GetCurrencyType();
    if (nSpendType == 2)
    {
        GameMsg_S2C_UpdateCumulativeSpendNum updatemsg;
        updatemsg.m_nSpendNum = m_nCumulativeBill + m_nCumulativeBindBill;
        _SendPlayerMsg(&updatemsg);
    }
}

void CEntityActivityComponent::OnGetCumulativeSpendReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetCumulativeSpendReward & msgGetReward = (GameMsg_C2S_GetCumulativeSpendReward &)msg;

    std::string strError = "";
    if (!CCumulativeSpendMgr::Instance().IsOpening())
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String1");
    }
    else if (!CCumulativeSpendMgr::Instance().IsValidLevel(msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String3");
    }
    else if (_HasReceivedCumulativeSpendReward(msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String4");
    }
    else if (!_ReachCumulativeSpendRequire(GetCumulativeSpendValue(CCumulativeSpendMgr::Instance().GetCurrencyType()), msgGetReward.m_nLevel))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String5");
    }
    else
    {
        if (m_pRoleAttr != NULL && m_pRoleItem != NULL)
        {
            std::list<CItem> listReward;
            const CCumulativeSpendReward *pRechargeInfo = CCumulativeSpendMgr::Instance().GetRewardInfo(msgGetReward.m_nLevel);

            if (pRechargeInfo != NULL)
            {
                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    listReward = pRechargeInfo->m_listMaleReward;
                }
                else if (m_pRoleAttr->GetSex() == ESexType_Female)
                {
                    listReward = pRechargeInfo->m_listFemaleReward;
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "Get RoleSex Error, roleid(%u)\n", m_pRoleAttr->GetRoleID());
                }

                if (CItemProcess::AddItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_CumulativeSpend, 0, true))
                {
                    m_pRoleAttr->ChangeMoney(pRechargeInfo->m_nMoney, EChangeMoneyCause_Add_CumulativeSpend, 0);
                    m_pRoleAttr->ChangeBindBill(pRechargeInfo->m_nBindMCoin, EChangeBindBillCause_Add_CumulativeSpendReward);


                    if (IsSpendRewardEmpty())
                    {
                        AddQuery(QUERY_Insert_CumulativeReward, m_pRoleAttr->GetRoleID(), NULL, 0);
                    }

                    m_CumulativeSpendReward[msgGetReward.m_nLevel - 1] = (unsigned int)time(NULL);

                    AddQuery(QUERY_Update_CumulativeReward, m_pRoleAttr->GetRoleID(), NULL, msgGetReward.m_nLevel);

                    GameMsg_S2C_GetCumulativeSpendRewardSuc msgSuc;
                    msgSuc.m_nLevel = msgGetReward.m_nLevel;
                    _SendPlayerMsg(&msgSuc);
                }
                else
                {
                    strError = CLocalization::Instance().GetString("CumulativeRechargeActivity_String6");
                }
            }
        }
    }

    if (strError != "")
    {
        GameMsg_S2C_GetCumulativeSpendRewardFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);

    }
}

void CEntityActivityComponent::_OnGetProprietaryReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    /*
    if (m_pRoleAttr != NULL && m_pRoleNet != NULL)
    {
        Version_Type eType = (Version_Type)m_pRoleNet->GetVerType();
        if (eType == Version_Charge)
        {
            //尊享版本领取奖励操作
            std::string strError = "";
            if (!CProprietaryActivityInfoMgr::Instance().IsOpening())
            {
                strError = "ProprietaryRewardActivity_Closed";
            }
            //判断是否领取过该奖励
            if (m_bProprietaryReward)
            {
                strError = "ProprietaryRewardActivity_Reward";
            }
            else
            {
                m_bProprietaryReward = true;
                //用Event事件来标识
                AddQuery(QUERY_AddProprietaryActivityReward, m_pRoleAttr->GetRoleID());

                //获取奖励物品
                CEntityItemComponent *pItemCom = GetComponent<CRoleItem>();
                if (pItemCom != NULL)
                {
                    if (m_pRoleAttr->GetSex() == ESexType_Male)
                    {
                        CItemProcess::AddOrMailItems(*(CRoleEntity*)Entity(),
                            CProprietaryActivityInfoMgr::Instance().Rewards().m_listMaleReward,
                            EItemAction_DanceGroup_Activity, 0, true, EMailType_DanceGroupActivity, "", "");
                    }
                    else
                    {
                        CItemProcess::AddOrMailItems(*(CRoleEntity*)Entity(),
                            CProprietaryActivityInfoMgr::Instance().Rewards().m_listFemaleReward,
                            EItemAction_DanceGroup_Activity, 0, true, EMailType_DanceGroupActivity, "", "");
                    }
                }
            }
            GameMsg_S2C_GetProprietaryRewardResult msgResult;
            msgResult.m_strError = strError;
            _SendPlayerMsg(&msgResult);
        }//end if
    }
    */
}

void CEntityActivityComponent::_OnExchangeItem(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_ExchangeItem msgExchange = (GameMsg_C2S_ExchangeItem &)msg;

    std::string strError = "";
    if (!CExchangeItemInfoMgr::Instance().IsOpening() || !CExchangeItemInfoMgr::Instance().IsExchangeItemOpen()) 
    {
        strError = CLocalization::Instance().GetString("ExchangeItems_String1");
    }
    else if (!CExchangeItemInfoMgr::Instance().IsValidLevel(msgExchange.m_nLevel)) 
    {
        strError = CLocalization::Instance().GetString("ExchangeItems_String2");
    }
    else if (!_ReachExchangeItemRequire(msgExchange.m_nLevel)) 
    {
        strError = CLocalization::Instance().GetString("ExchangeItems_String3");
    }
    else
    {
        std::list<CItem> listReward;
        const  CExchangeItemInfo* pExchangeInfo = CExchangeItemInfoMgr::Instance().GetExchangeItemInfo(msgExchange.m_nLevel);

        if (pExchangeInfo != NULL)
        {
            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                listReward = pExchangeInfo->m_listMaleReward;
            }
            else if (m_pRoleAttr->GetSex() == ESexType_Female)
            {
                listReward = pExchangeInfo->m_listFemaleReward;
            }
            else
            {
                WriteLog(LOGLEVEL_ERROR, "Get RoleSex Error, roleid(%u)\n", m_pRoleAttr->GetRoleID());
            }

            if (CItemProcess::AddItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_ExchangeItem, 0, true)) {
                m_pRoleItem->RemoveItemByType(CExchangeItemInfoMgr::Instance().RequiredItemType(), pExchangeInfo->m_nRequiredItemNum,EItemAction_Del_ExchangeItem);
                m_pRoleAttr->ChangeMoney(pExchangeInfo->m_nMoney, EChangeMoneyCause_Add_ExchangeItem, 0);

                GameMsg_S2C_ExchangeItemSuc msgSuc;
                msgSuc.m_nLevel = msgExchange.m_nLevel;
                _SendPlayerMsg(&msgSuc);
            }
            else
            {
                strError = CLocalization::Instance().GetString("ExchangeItems_String4");
            }
        }
    }

    if (strError != "")
    {
        GameMsg_S2C_ExchangeItemFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::_SaveXmasFreeRewardInfo(unsigned int nLastGetRewardTime)
{
    std::vector<int> * pVecReward = new std::vector<int>(m_roleXmasFreeRewardInfo.m_vecFreeReward);
    CDelHelper delHelper(pVecReward);
    AddQuery(QUERY_SaveXmasFreeRewardInfo, m_pRoleAttr->GetRoleID(), pVecReward, nLastGetRewardTime, NULL, &delHelper);
}

void CEntityActivityComponent::_LoadXmasFreeRewardInfo()
{
    CXmasFreeRewardDB *pXmasFreeRewardDB = new CXmasFreeRewardDB;
    if (pXmasFreeRewardDB != NULL) {
        pXmasFreeRewardDB->m_nRoleID = m_pRoleAttr->GetRoleID();
        pXmasFreeRewardDB->m_nActivityStartTime = CXmasFreeRewardDataMgr::Instance().ActivityStartTime();
        pXmasFreeRewardDB->m_nActivityEndTime = CXmasFreeRewardDataMgr::Instance().ActivityEndTime();
        CDelHelper delHelper(pXmasFreeRewardDB);
        AddQuery(QUERY_LoadXmasFreeRewardInfo, 0, pXmasFreeRewardDB, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_SendXmasFreeRewardActivityInfo()
{
    GameMsg_S2C_GetXmasActivityInfoSuc msgSuc;
    if (CXmasFreeRewardDataMgr::Instance().IsOpening()) {
        msgSuc.m_XmasFreeRewardActivity.m_nActivity = EActivity_XmaxFreeReward;
        const std::map<int, CXmasFreeRewardInfo *> & freeRewardMap = CXmasFreeRewardDataMgr::Instance().FreeRewardMap();
        for (std::map<int, CXmasFreeRewardInfo *>::const_iterator itr = freeRewardMap.begin(); itr != freeRewardMap.end(); ++itr) {
            if (itr->second != NULL) {
                if (itr->second->m_nLevel <= XMAS_FREEREWARD_LEVEL) {
                    CXmasRewardSend sendRewardInfo;
                    sendRewardInfo.m_nIndex = (unsigned char)itr->second->m_nLevel;
                    sendRewardInfo.m_nState = (unsigned char)m_roleXmasFreeRewardInfo.m_vecFreeReward[itr->second->m_nLevel - 1];
                    msgSuc.m_XmasFreeRewardActivity.m_listFreeRewardInfo.push_back(sendRewardInfo);
                }
            }
        }
    }
    _SendPlayerMsg(&msgSuc);
}

void CEntityActivityComponent::_OnGetXmasActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    _SendXmasFreeRewardActivityInfo();
}

void CEntityActivityComponent::_OnGetXmasFreeReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetXmasFreeReward &rMsgGetReward = (GameMsg_C2S_GetXmasFreeReward &)msg;

    unsigned char nResult = EGetXmasFreeRewardErrno_Success;
    if (!CXmasFreeRewardDataMgr::Instance().IsOpening()) {
        nResult = EGetXmasFreeRewardErrno_ActivityNotOpen;
    }


    CXmasFreeRewardInfo *pInfo = CXmasFreeRewardDataMgr::Instance().GetXmasFreeRewardInfo(rMsgGetReward.m_nLevel);
    if (pInfo == NULL || (rMsgGetReward.m_nLevel - 1) > XMAS_FREEREWARD_LEVEL) {
        nResult = EGetXmasFreeRewardErrno_NoSuchReward;
    }

    if (m_roleXmasFreeRewardInfo.m_vecFreeReward[rMsgGetReward.m_nLevel - 1] == 1) {
        nResult = EGetXmasFreeRewardErrno_HasGotTheReward;
    }

    if (nResult == EGetXmasFreeRewardErrno_Success) {
        m_pRoleAttr->ChangeMoney(pInfo->m_nMoney, EChangeMoneyCause_Add_XmasFreeReward, 0);
        std::list<CItem> listItems = m_pRoleAttr->GetSex() == ESexType_Male ? pInfo->m_listMaleReward : pInfo->m_listFemaleReward;
        std::string strMailSubject = CLocalization::Instance().GetString("Xmas_Free_Reward_Mail_Subject");
        std::string strMailBody = CLocalization::Instance().GetString("Xmas_Free_Reward_Mail_Body");
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listItems, EItemAction_Add_GetXmasFreeReward, 0, true, EMailType_GetXmaxFreeReward, strMailSubject, strMailBody);
        m_roleXmasFreeRewardInfo.m_vecFreeReward[rMsgGetReward.m_nLevel - 1] = 1;
        _SaveXmasFreeRewardInfo((unsigned int)time(NULL));

        GameMsg_S2C_GetXmasFreeRewardSuc msgSuc;
        msgSuc.m_nLevel = (unsigned char)pInfo->m_nLevel;
        msgSuc.m_listItems = listItems;
        msgSuc.m_nMoney = pInfo->m_nMoney;
        _SendPlayerMsg(&msgSuc);
    }
    else {
        GameMsg_S2C_GetXmasFreeRewardFail msgFail;
        msgFail.m_nError = nResult;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::_LoadFestivalWishInfo()
{
    CFestivalWishInfoDB *pWishInfoDB = new CFestivalWishInfoDB;
    if (pWishInfoDB != NULL) {
        pWishInfoDB->m_nRoleID = m_pRoleAttr->GetRoleID();
        pWishInfoDB->m_nActivityStartTime = CFestivalWishDataMgr::Instance().ActivityStartTime();
        pWishInfoDB->m_nActivityEndTime = CFestivalWishDataMgr::Instance().ActivityEndTime();
        CDelHelper delHelper(pWishInfoDB);
        AddQuery(QUERY_LoadFestivalWishInfo, 0, pWishInfoDB, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_SaveFestivalWishInfo()
{
    CFestivalWishInfoDB *pWishInfoDB = new CFestivalWishInfoDB;
    if (pWishInfoDB != NULL) {
        pWishInfoDB->m_nRoleID = m_pRoleAttr->GetRoleID();
        pWishInfoDB->m_nWishCount = m_roleFestivalWishActivityInfo.m_nWishedCount;
        pWishInfoDB->m_nLastGetRewardTime = m_roleFestivalWishActivityInfo.m_nLastGetRewardTime;
        CDelHelper delHelper(pWishInfoDB);
        AddQuery(QUERY_SaveFestivalWishInfo, 0, pWishInfoDB, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_SendFestivalWishActivityInfo()
{
    GameMsg_S2C_GetFestivalWishInfoSuc msgSuc;
    if (CFestivalWishDataMgr::Instance().IsOpening()) {
        msgSuc.m_FestivalWishInfo.m_nActivityID = EActivity_FestivalWish;
        msgSuc.m_FestivalWishInfo.m_nWishRemainTimes = (unsigned short)(CFestivalWishDataMgr::Instance().MaxWishTimes() - m_roleFestivalWishActivityInfo.m_nWishedCount);
        msgSuc.m_FestivalWishInfo.m_nWishItemCount = (unsigned short)CFestivalWishDataMgr::Instance().GetWishNeedItemCount(m_roleFestivalWishActivityInfo.m_nWishedCount + 1);
        CFestivalWishRewardData *pReward = CFestivalWishDataMgr::Instance().GetWishReward(m_roleFestivalWishActivityInfo.m_nWishedCount + 1);
        if (pReward != NULL) {
            msgSuc.m_FestivalWishInfo.m_listItems = m_pRoleAttr->GetSex() == ESexType_Male ? pReward->m_listMaleItems : pReward->m_listFemaleItems;
            msgSuc.m_FestivalWishInfo.m_nMoney = pReward->m_nMoney;
        }
    }
    _SendPlayerMsg(&msgSuc);
}

void CEntityActivityComponent::_OnGetFestivalWishActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    _SendFestivalWishActivityInfo();
}

void CEntityActivityComponent::_OnRoleFestivalWish(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    unsigned char nResut = EFestivalWishFailErrno_Success;

    if (!CFestivalWishDataMgr::Instance().IsOpening()) {
        nResut = EFestivalWishFailErrno_NotOpen;
    }

    if (m_roleFestivalWishActivityInfo.m_nWishedCount >= CFestivalWishDataMgr::Instance().MaxWishTimes()) {
        nResut = EFestivalWishFailErrno_ReachLimitTimes;
    }

    CFestivalWishRequireData* pWishRequirement = CFestivalWishDataMgr::Instance().GetWishRequirement(m_roleFestivalWishActivityInfo.m_nWishedCount + 1);
    if (pWishRequirement == NULL) {
        nResut = EFestivalWishFailErrno_Unkonw;
    }

    if (m_pRoleItem == NULL || m_pRoleItem->GetItemCount(pWishRequirement->m_nItemType, false, false) < pWishRequirement->m_nIteCount) {
        nResut = EFestivalWishFailErrno_NoEnoughItem;
    }

    if (nResut == EFestivalWishFailErrno_Success) {
        m_pRoleItem->RemoveItemByType(pWishRequirement->m_nItemType, pWishRequirement->m_nIteCount, EItemAction_Del_FestivalWish);
        ++m_roleFestivalWishActivityInfo.m_nWishedCount;
        m_roleFestivalWishActivityInfo.m_nLastGetRewardTime = (unsigned int)time(NULL);
        _SaveFestivalWishInfo();

        GameMsg_S2C_FestivalWishSuc msgSuc;
        CFestivalWishRewardData *pReward = CFestivalWishDataMgr::Instance().GetWishReward(m_roleFestivalWishActivityInfo.m_nWishedCount);
        if (pReward != NULL) {
            m_pRoleAttr->ChangeMoney(pReward->m_nMoney, EChangeMoneyCause_Add_FestivalWish, 0);
            msgSuc.m_listCurRewards = m_pRoleAttr->GetSex() == ESexType_Male ? pReward->m_listMaleItems : pReward->m_listFemaleItems;
            msgSuc.m_nCurMoney = pReward->m_nMoney;
            std::string strMailSubjet = CLocalization::Instance().GetString("Festival_Wish_Reward_Mail_Subject");
            std::string strMailBody = CLocalization::Instance().GetString("Festival_Wish_Reward_Mail_Body");
            CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), msgSuc.m_listCurRewards, EItemAction_Add_FestivalWish, 0, true, EMailType_FestivalWish, strMailSubjet, strMailBody);
        }
        CFestivalWishRewardData *pNextReward = CFestivalWishDataMgr::Instance().GetWishReward(m_roleFestivalWishActivityInfo.m_nWishedCount + 1);
        if (pNextReward != NULL) {
            msgSuc.m_listNextReward = m_pRoleAttr->GetSex() == ESexType_Male ? pNextReward->m_listMaleItems : pNextReward->m_listFemaleItems;
            msgSuc.m_nNextMoney = pNextReward->m_nMoney;
            msgSuc.m_nWishTimesRemain = (unsigned short)(CFestivalWishDataMgr::Instance().MaxWishTimes() - m_roleFestivalWishActivityInfo.m_nWishedCount);
            msgSuc.m_nNextWishItemCountNeed = (unsigned short)CFestivalWishDataMgr::Instance().GetWishNeedItemCount(m_roleFestivalWishActivityInfo.m_nWishedCount + 1);;
        }

        _SendPlayerMsg(&msgSuc);
    }
    else {
        GameMsg_S2C_FestivalWishFail msgFail;
        msgFail.m_nError = nResut;
        _SendPlayerMsg(&msgFail);
    }

}

void CEntityActivityComponent::_SaveBlessInfo(CFestivalBlessInfoDB *pBlessEntryDB)
{
    if (pBlessEntryDB != NULL) {
        CDelHelper delHelper(pBlessEntryDB);
        AddQuery(QUERY_SaveFestivalBlessEntry, 0, pBlessEntryDB, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_SaveRoleBlessActivityInfo(unsigned int nGetRewardTime)
{
    std::vector<int> *pVecReward = new std::vector<int>(m_roleBlessActivityInfo.m_vecRewards);
    CDelHelper delHelper(pVecReward);
    AddQuery(QUERY_SaveRoleFestivalBlessRewardInfo, m_pRoleAttr->GetRoleID(), pVecReward, nGetRewardTime, NULL, &delHelper);
}

void CEntityActivityComponent::_LoadRoleBlessActivityInfo()
{
    CRoleBlessActivityDB *pBlessInfo = new CRoleBlessActivityDB;
    if (pBlessInfo != NULL) {
        pBlessInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
        pBlessInfo->m_nBlessStartTime = CFestivalBlessActivityDataMgr::Instance().BlessBeginTime();
        pBlessInfo->m_nBlessEndTime = CFestivalBlessActivityDataMgr::Instance().BlessEndTime();

        CDelHelper delHelper(pBlessInfo);
        AddQuery(QUERY_LoadRoleBlessActivityInfo, 0, pBlessInfo, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_OnGetFestivalBlessActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetFestivalBlessActivityInfoSuc msgSuc;
    if (CFestivalBlessActivityDataMgr::Instance().IsShowOpening()) {
        msgSuc.m_nActivityID = EActivity_FestivalBless;
        msgSuc.m_nBeBlessedCount = m_roleBlessActivityInfo.m_nBeBlessedCount;
        msgSuc.m_BeBlessWords = m_roleBlessActivityInfo.m_listBlesses;
        const std::map<int, CFestivalBlessedReward*> & rRewardMap = CFestivalBlessActivityDataMgr::Instance().FestivalBlessRewardMap();
        for (std::map<int, CFestivalBlessedReward*>::const_iterator itr = rRewardMap.begin(); itr != rRewardMap.end(); ++itr) {
            if (itr->second != NULL) {
                CFestivalBlessRewardSend blessRewardSend;
                blessRewardSend.m_nLevel = (unsigned char)itr->second->m_nLevel;
                blessRewardSend.m_nBeBlessedCount = itr->second->m_nBeBlessedCount;
                blessRewardSend.m_listRewards = m_pRoleAttr->GetSex() == ESexType_Male ? itr->second->m_listMaleReward : itr->second->m_listFemaleReward;
                blessRewardSend.m_nMoney = itr->second->m_nMoney;
                blessRewardSend.m_bAlreadyGot = m_roleBlessActivityInfo.m_vecRewards[itr->second->m_nLevel - 1] > 0;
                msgSuc.m_listBlessRewardInfo.push_back(blessRewardSend);
            }
        }
        const std::map<int, CFestivalBlessWords*> & rWordsMap = CFestivalBlessActivityDataMgr::Instance().FestivalBlessWordsMap();
        for (std::map<int, CFestivalBlessWords*>::const_iterator itr = rWordsMap.begin(); itr != rWordsMap.end(); ++itr) {
            if (itr->second != NULL) {
                msgSuc.m_listBlessWordsData.push_back(*(itr->second));
            }
        }
    }

    m_roleBlessActivityInfo.m_bHasSentInfoToClient = true;
    _SendPlayerMsg(&msgSuc);
}

void CEntityActivityComponent::_BlessFriend(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_BlessFriend & rMsgBless = (GameMsg_C2S_BlessFriend &)msg;

    unsigned char nResult = EBlessFriendError_Success;
    if (!CFestivalBlessActivityDataMgr::Instance().IsBlessOpening()) {
        nResult = EBlessFriendError_NotOpen;
    }

    if (m_pRoleFriend == NULL || !m_pRoleFriend->IsFriend(rMsgBless.m_nFriendID)) {
        nResult = EBlessFriendError_NotFriend;
    }

    CFestivalBlessWords *pBlessWords = CFestivalBlessActivityDataMgr::Instance().GetFestivalBlessWord(rMsgBless.m_nWordsIndex);
    if (pBlessWords == NULL) {
        nResult = EBlessFriendError_WordsNotExists;
    }

    if (m_pRoleItem == NULL || m_pRoleItem->GetItemCount(CFestivalBlessActivityDataMgr::Instance().BlessRequireItem(), false, false) <= 0) {
        nResult = EBlessFriendError_NotEnoughItem;
    }

    if (nResult == EBlessFriendError_Success) {
        unsigned int nTimeNow = (unsigned int)time(NULL);
        CFestivalBlessInfoDB *pBlessInfoDB = new CFestivalBlessInfoDB;
        pBlessInfoDB->m_nSenderID = m_pRoleAttr->GetRoleID();
        pBlessInfoDB->m_nRcverID = rMsgBless.m_nFriendID;
        pBlessInfoDB->m_nWordsIndex = rMsgBless.m_nWordsIndex;
        pBlessInfoDB->m_nBlessTime = nTimeNow;
        _SaveBlessInfo(pBlessInfoDB);
        m_pRoleItem->RemoveItemByType(CFestivalBlessActivityDataMgr::Instance().BlessRequireItem(), 1, EItemAction_Del_BlessFriend);
        GameMsg_S2G_BlessFriend msgToGroup;
        msgToGroup.m_nSenderID = m_pRoleAttr->GetRoleID();
        msgToGroup.m_nRcverID = rMsgBless.m_nFriendID;
        msgToGroup.m_strSenderName = m_pRoleAttr->GetRoleName();
        msgToGroup.m_nWordsIndex = rMsgBless.m_nWordsIndex;
        msgToGroup.m_nBlessTime = nTimeNow;

        CFestivalSendBlessReward & sendBlessReward = CFestivalBlessActivityDataMgr::Instance().SendBlessReward();
        std::list<CItem> listItems = m_pRoleAttr->GetSex() == ESexType_Male ? sendBlessReward.m_listMaleRewards : sendBlessReward.m_listFemaleRewards;
        m_pRoleAttr->ChangeMoney(sendBlessReward.m_nMoney, EChangeMoneyCause_Add_BlessFriend, 0);
        if (!listItems.empty())
        {
            std::string strMailSubjet = CLocalization::Instance().GetString("Festival_Bless_Friend_Reward_Subject");
            std::string strMailBody = CLocalization::Instance().GetString("Festival_Bless_Friend_Reward_Body");
            CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listItems, EItemAction_Add_BlessFriendFriend, 0, true, EMailType_FestivaBlessFriend, strMailSubjet, strMailBody);
            if (m_pRoleChat != NULL)
            {
                std::list<CItem>::iterator itr = listItems.begin();
                ItemConfig * pItem = ConfigManager::Instance().GetItemConfigManager().GetByID(itr->m_nItemType);
                if (pItem != NULL) {
                    m_pRoleChat->SysChat(CLocalization::Instance().GetString("Festival_Bless_Friend_SysChat").c_str(), m_pRoleAttr->GetRoleName(), pItem->m_strName.c_str());
                }
            }
        }

        ::SendMsg2GroupServer(&msgToGroup);

        GameMsg_S2C_BlessFriendSuc msgSuc;
        msgSuc.m_listRewards = listItems;
        msgSuc.m_nMoney = sendBlessReward.m_nMoney;
        _SendPlayerMsg(&msgSuc);
    }
    else {
        GameMsg_S2C_BlessFriendFail msgFail;
        msgFail.m_nError = nResult;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::_BlessFriend_G(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_BlessFriend & rMsgBless = (GameMsg_G2S_BlessFriend &)msg;

    CFestivalBlessEntry blessEntry;
    blessEntry.m_nSenderID = rMsgBless.m_nSenderID;
    blessEntry.m_strSenderName = rMsgBless.m_strSenderName;
    blessEntry.m_nWordsIndex = rMsgBless.m_nWordsIndex;
    blessEntry.m_nBlessTime = rMsgBless.m_nBlessTime;
    m_roleBlessActivityInfo.m_listBlesses.push_front(blessEntry);
    ++m_roleBlessActivityInfo.m_nBeBlessedCount;

    if (m_roleBlessActivityInfo.m_bHasSentInfoToClient) {
        GameMsg_S2C_GetBlessFromFriend msgBlessTo;
        msgBlessTo.m_nWordsIndex = (unsigned char)blessEntry.m_nWordsIndex;
        msgBlessTo.m_strSenderName = blessEntry.m_strSenderName;

        _SendPlayerMsg(&msgBlessTo);
    }
}

void CEntityActivityComponent::_GetFestivalBlessReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetFestivalBlessReward & rMsgGetReward = (GameMsg_C2S_GetFestivalBlessReward &)msg;

    unsigned char nResult = EGetFestivalBlessRewarError_Success;
    if (!CFestivalBlessActivityDataMgr::Instance().IsBlessOpening()) {
        nResult = EGetFestivalBlessRewarError_NotOpen;
    }

    CFestivalBlessedReward *pReward = CFestivalBlessActivityDataMgr::Instance().GetFestivalBlessReward(rMsgGetReward.m_nLevel);
    if (pReward == NULL || rMsgGetReward.m_nLevel > BLESS_REWARD_LEVEL) {
        nResult = EGetFestivalBlessRewarError_NotSuchLevel;
    }

    if (m_roleBlessActivityInfo.m_nBeBlessedCount < pReward->m_nBeBlessedCount) {
        nResult = EGetFestivalBlessRewarError_NotReachRequirement;
    }

    if (m_roleBlessActivityInfo.m_vecRewards[rMsgGetReward.m_nLevel - 1] > 0) {
        nResult = EGetFestivalBlessRewarError_AlreadyGot;
    }

    if (nResult == EGetFestivalBlessRewarError_Success) {
        if (m_pRoleAttr != NULL) {
            m_pRoleAttr->ChangeMoney(pReward->m_nMoney, EChangeMoneyCause_Add_BeBlessed, 0);
            m_roleBlessActivityInfo.m_vecRewards[rMsgGetReward.m_nLevel - 1] = 1;
            _SaveRoleBlessActivityInfo((unsigned int)(time(NULL)));

            std::string strMailSubject = CLocalization::Instance().GetString("Festival_Bless_Reward_Mail_Subject");
            std::string strMailBody = CLocalization::Instance().GetString("Festival_Bless_Reward_Mail_Body");
            std::list<CItem> listReward = m_pRoleAttr->GetSex() == ESexType_Male ? pReward->m_listMaleReward : pReward->m_listFemaleReward;
            CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_BlessFriend, 0, true, EMailType_FestivalBless, strMailSubject, strMailBody);

            GameMsg_S2C_GetFestivalBlessRewardSuc msgSuc;
            msgSuc.m_nLevel = rMsgGetReward.m_nLevel;
            _SendPlayerMsg(&msgSuc);
        }
    }
    else {
        GameMsg_S2C_GetFestivalBlessRewardFail msgFail;
        msgFail.m_nError = nResult;
        _SendPlayerMsg(&msgFail);
    }
}

bool CEntityActivityComponent::_ReachCumulativeRechargeRequire(int nCurRechargeNum, int nLevel)
{
    const CCumulativeReward * pRewardInfo = CCumulativeRechargeMgr::Instance().GetRewardInfo(nLevel);
    if (pRewardInfo != NULL) {
        return nCurRechargeNum >= pRewardInfo->m_nRequiredNum;
    }

    return false;
}

bool CEntityActivityComponent::_ReachCumulativeSpendRequire(int nCurRechargeNum, int nLevel)
{
    const CCumulativeSpendReward * pRewardInfo = CCumulativeSpendMgr::Instance().GetRewardInfo(nLevel);
    if (pRewardInfo != NULL)
    {
        return nCurRechargeNum >= pRewardInfo->m_nRequiredNum;
    }

    return false;
}

int CEntityActivityComponent::GetCumulativeSpendValue(int nCurrencyType)
{
    switch (nCurrencyType)
    {
    case  1:return m_nCumulativeBill;
    case  2:return m_nCumulativeBill + m_nCumulativeBindBill;
    default: return 0;
    }
}

int CEntityActivityComponent::GetCumulativeSpendValueForMedal(int nCurrencyType)
{
    switch (nCurrencyType)
    {
    case  1:return m_nCumulativeBillForMedal;
    case  2:return m_nCumulativeBillForMedal + m_nCumulativeBindBillForMedal;
    default: return 0;
    }
}

void CEntityActivityComponent::_LoadCumulativeSpendInfo()
{
    std::vector<unsigned int> * pVecCumulatveSpend = new std::vector<unsigned int>(Cumulative_Spend_Reward_MaxCount);

    CCumulativeSpendDB * pInfo = new CCumulativeSpendDB;
    pInfo->m_nSpendStartTime = CCumulativeSpendMgr::Instance().SpendStartTime();
    pInfo->m_nSpendStopTime = CCumulativeSpendMgr::Instance().SpendStopTime();

    CDelHelper delHelper(pVecCumulatveSpend, pInfo);
    AddQuery(QUERY_LoadCumulativeSpendInfo, m_pRoleAttr->GetRoleID(), pVecCumulatveSpend, 0, pInfo, &delHelper);
}

void CEntityActivityComponent::_LoadCumulativeSpendInfoForMedal()
{
    std::vector<unsigned int> * pVecCumulatveSpend = new std::vector<unsigned int>(Cumulative_Spend_Reward_MaxCount);

    CCumulativeSpendDB * pInfo = new CCumulativeSpendDB;
    pInfo->m_nSpendStartTime = CumulativeSpendGotMedalMgr::Instance().GetBeginTime();
    pInfo->m_nSpendStopTime = CumulativeSpendGotMedalMgr::Instance().GetEndTime();

    CDelHelper delHelper(pVecCumulatveSpend, pInfo);
    AddQuery(QUERY_LoadCumulativeSpendInfoForMedal, m_pRoleAttr->GetRoleID(), pVecCumulatveSpend, 0, pInfo, &delHelper);
}

void CEntityActivityComponent::OnGetCumulativeSpendGotMedalReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetCumulativeSpendGotMedalReward &rGetMsg = (GameMsg_C2S_GetCumulativeSpendGotMedalReward &)msg;
    std::string strError("");

    if (m_pRoleAttr == NULL || m_pRoleMedal == NULL)
        return;

    CumulativeSpendGotMedalMgr &rMgr = CumulativeSpendGotMedalMgr::Instance();

    if (!rMgr.HasOpening())
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String1");
    }
    else if (!rMgr.IsValidIndex(rGetMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String3");
    }
    else if (HasReceivedCumulativeSpendGotMedalReward(rGetMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String4");
    }
    else if (!CanGetCumulativeSpendGotMedalRewards(rGetMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("CumulativeSpendActivity_String5");
    }
    else
    {
        std::list<int> listRewards;
        rMgr.GetRewards(rGetMsg.m_nIndex, m_pRoleAttr->GetSex(), listRewards);

        m_pRoleMedal->SetMedalActivited(listRewards, CMedalLog::EMedalActivityCumulativeSpend);

        m_vectCumulativeSpendGotMedalReward[rGetMsg.m_nIndex] = true;
        AddQuery(QUERY_InsertCumulativeSpendGotMedalReward, rGetMsg.m_nIndex, NULL, m_pRoleAttr->GetRoleID());

        GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc msgSuc;
        msgSuc.m_nIndex = rGetMsg.m_nIndex;
        _SendPlayerMsg(&msgSuc);
    }

    if (!strError.empty())
    {
        GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::LoadCumulativeSpendGotMedalReward()
{
    if (m_pRoleAttr == NULL)
        return;

    int nMaxSize = CumulativeSpendGotMedalMgr::Instance().GetMaxLevel();
    std::vector<bool> *pGotRewardInfo = new std::vector<bool>(nMaxSize, false);

    CumulativeSpendGotMedalDB *pInfo = new CumulativeSpendGotMedalDB;
    pInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
    pInfo->m_nOpenTime = CumulativeSpendGotMedalMgr::Instance().GetOpenTime();
    pInfo->m_nCloseTime = CumulativeSpendGotMedalMgr::Instance().GetCloseTime();

    CDelHelper delHelper(pGotRewardInfo);
    AddQuery(QUERY_LoadCumulativeSpendGotMedalReward, 0, pGotRewardInfo, 0, pInfo, &delHelper);
}

bool CEntityActivityComponent::CanGetCumulativeSpendGotMedalRewards(unsigned char nIndex)
{
    int nRequired = CumulativeSpendGotMedalMgr::Instance().GetRequiredAmount(nIndex);
    int nAmount = GetCumulativeSpendValueForMedal(CumulativeSpendGotMedalMgr::Instance().GetCumulativeType());

    if (0 < nRequired && nRequired <= nAmount)
        return true;

    return false;
}

void CEntityActivityComponent::_LoadCumulativeRechargeInfo()
{
    if (m_pRoleAttr != NULL)
    {
        std::vector<int> *pVecReward = new std::vector<int>(m_vecCumulativeRewards);

        CCumulativeRechargeDB *pInfo = new CCumulativeRechargeDB;
        pInfo->m_nActivityBeginTime = CCumulativeRechargeMgr::Instance().ActivityBeginTime();
        pInfo->m_nActivityEndTime = CCumulativeRechargeMgr::Instance().ActivityEndTime();
        pInfo->m_nRechargeStartTime = CCumulativeRechargeMgr::Instance().RechargeStartTime();
        pInfo->m_nRechargeStopTime = CCumulativeRechargeMgr::Instance().RechargeStopTime();
        CDelHelper delHelper(pVecReward, pInfo);
        AddQuery(QUERY_LoadCumulativeRechargeInfo, m_pRoleAttr->GetRoleID(), pVecReward, m_pRoleAttr->GetAccount(), pInfo, &delHelper);
    }
}

void CEntityActivityComponent::_LoadRolePuzzleDB()
{
    if (m_pRoleAttr != NULL)
    {
        CRoleAllPuzzleDB *pRolePuzzleDB = new CRoleAllPuzzleDB();
        pRolePuzzleDB->m_nRoleID = m_pRoleAttr->GetRoleID();
        pRolePuzzleDB->m_nPuzzleBeginTime = CPuzzleDataMgr::Instance().PuzzleBeginTime();
        pRolePuzzleDB->m_nPuzzleEndTime = CPuzzleDataMgr::Instance().PuzzleEndTime();
        CDelHelper delHelper(pRolePuzzleDB);
        AddQuery(QUERY_Puzzle_LoadPicture, 0, pRolePuzzleDB, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_UpdateRolePuzzleDB(int nOriTime, const CRolePuzzleActivityInfo & rPuzzleInfo)
{
    if (m_pRoleAttr != NULL)
    {
        CRolePuzzleActivityDB* pPuzzleInfoDB = new CRolePuzzleActivityDB();
        pPuzzleInfoDB->m_nPuzzleID = rPuzzleInfo.m_nPuzzleID;
        pPuzzleInfoDB->m_nUpdateTime = rPuzzleInfo.m_nUpdateTime;
        pPuzzleInfoDB->m_nFinishTime = rPuzzleInfo.m_nFinishTime;
        strcpy(pPuzzleInfoDB->m_arFragments, rPuzzleInfo.m_arFragments);

        CDelHelper delHelper(pPuzzleInfoDB);
        if (nOriTime > 0)
        {
            AddQuery(QUERY_Puzzle_UpdatePicture, m_pRoleAttr->GetRoleID(), pPuzzleInfoDB, nOriTime, NULL, &delHelper);
        }
        else
        {
            AddQuery(QUERY_Puzzle_InsertPicture, m_pRoleAttr->GetRoleID(), pPuzzleInfoDB, 0, NULL, &delHelper);
        }
    }
}

void CEntityActivityComponent::_InitRolePuzzleInfo(const std::list<CRolePuzzleActivityDB> &rlistPuzzle)
{
    if (!CPuzzleDataMgr::Instance().IsOpening())
    {
        m_bCheckPuzzleOpen = true;
        return;
    }

    std::map<int, const CRolePuzzleActivityDB *> mapValidData;
    int nBeginTime = CPuzzleDataMgr::Instance().m_nPuzzleBeginTime;
    int nEndTime = CPuzzleDataMgr::Instance().m_nPuzzleEndTime;

    for (std::list<CRolePuzzleActivityDB>::const_iterator itrData = rlistPuzzle.begin();
        itrData != rlistPuzzle.end(); ++itrData)
    {
        if (nBeginTime <= itrData->m_nUpdateTime && itrData->m_nUpdateTime <= nEndTime)
        {
            mapValidData.insert(std::make_pair(itrData->m_nPuzzleID, &(*itrData)));
        }
    }

    unsigned int nRoleID = m_pRoleAttr->GetRoleID();
    const std::map<int, CPuzzleInfo> &rAllConfig = CPuzzleDataMgr::Instance().m_mapAllPuzzle;
    for (std::map<int, CPuzzleInfo>::const_iterator itrConfig = rAllConfig.begin();
        itrConfig != rAllConfig.end(); ++itrConfig)
    {
        CRolePuzzleActivityInfo &rInfo = m_rolePuzzleActivityInfo[itrConfig->first];
        rInfo.m_nOwnerID = nRoleID;
        rInfo.m_nPuzzleID = itrConfig->first;

        std::map<int, const CRolePuzzleActivityDB *>::const_iterator itrValidData = mapValidData.find(itrConfig->first);
        if (itrValidData != mapValidData.end() && itrValidData->second != NULL)
        {
            rInfo.m_nUpdateTime = itrValidData->second->m_nUpdateTime;
            rInfo.m_nFinishTime = itrValidData->second->m_nFinishTime;
            memcpy(rInfo.m_arFragments, itrValidData->second->m_arFragments, PUZZLE_FRAGMENTS_COUNT + 1);
        }
    }
}

void CEntityActivityComponent::_InitRolePuzzleInfo(CRoleAllPuzzleDB *pPuzzleDB)
{
    GameMsg_S2C_SyncPuzzleData syncMsg;
    syncMsg.m_strPuzzleHelpTitle.assign(CPuzzleDataMgr::Instance().m_strPuzzleHelpTitle);
    syncMsg.m_strPuzzleHelpContent.assign(CPuzzleDataMgr::Instance().m_strPuzzleHelpContent);

    for (std::map<int, CPuzzleInfo>::iterator it_Data = CPuzzleDataMgr::Instance().m_mapAllPuzzle.begin(); it_Data != CPuzzleDataMgr::Instance().m_mapAllPuzzle.end(); ++it_Data)
    {
        CRolePuzzleActivityInfo puzzleInfo;
        puzzleInfo.m_nOwnerID = m_pRoleAttr->GetRoleID();
        puzzleInfo.m_nPuzzleID = it_Data->first;

        if (pPuzzleDB != NULL)
        {
            std::map<int, CRolePuzzleActivityDB>::iterator it_DB = pPuzzleDB->m_AllPuzzle.find(it_Data->first);
            if (it_DB != pPuzzleDB->m_AllPuzzle.end())
            {
                puzzleInfo.m_nUpdateTime = it_DB->second.m_nUpdateTime;
                puzzleInfo.m_nFinishTime = it_DB->second.m_nFinishTime;
                strcpy(puzzleInfo.m_arFragments, it_DB->second.m_arFragments);
            }
        }

        m_rolePuzzleActivityInfo.insert(std::make_pair(puzzleInfo.m_nPuzzleID, puzzleInfo));

        CRolePuzzleSync puzzleSync;
        puzzleSync.m_nPuzzleID = puzzleInfo.m_nPuzzleID;
        puzzleSync.m_strTitle.assign(it_Data->second.m_strTitle);
        puzzleSync.m_strTexture.assign(it_Data->second.m_strTexture);
        puzzleSync.m_NormalCost = it_Data->second.m_NormalCost;
        puzzleSync.m_SpecialCost = it_Data->second.m_SpecialCost;
        puzzleSync.m_nFinishTime = puzzleInfo.m_nFinishTime;
        strcpy(puzzleSync.m_arFragments, puzzleInfo.m_arFragments);
        puzzleSync.m_lstItemReward = (m_pRoleAttr->GetSex() == ESexType_Male ? it_Data->second.m_lstMaleReward : it_Data->second.m_lstFemaleReward);
        puzzleSync.m_nMoneyReward = it_Data->second.m_nMoneyReward;

        syncMsg.m_vecRolePuzzle.push_back(puzzleSync);
    }

    _SendPlayerMsg(&syncMsg);
}

void CEntityActivityComponent::_OnFillPuzzle(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    unsigned char nResult = EFillPuzzleError_Success;

    if (CPuzzleDataMgr::Instance().IsOpening())
    {
        GameMsg_C2S_FillPuzzle& rFillMsg = (GameMsg_C2S_FillPuzzle&)msg;

        std::map<int, CPuzzleInfo>::iterator it_Data = CPuzzleDataMgr::Instance().m_mapAllPuzzle.find(rFillMsg.m_nPuzzleID);
        if (it_Data != CPuzzleDataMgr::Instance().m_mapAllPuzzle.end())
        {
            std::map<int, CRolePuzzleActivityInfo>::iterator it_Info = m_rolePuzzleActivityInfo.find(rFillMsg.m_nPuzzleID);
            if (it_Info != m_rolePuzzleActivityInfo.end())
            {
                if (rFillMsg.m_nPieceIndex >= 0 && rFillMsg.m_nPieceIndex < PUZZLE_FRAGMENTS_COUNT)
                {
                    if (it_Info->second.m_arFragments[rFillMsg.m_nPieceIndex] == '0')
                    {
                        CItem& costItem = (rFillMsg.m_bUseSpecial ? it_Data->second.m_SpecialCost : it_Data->second.m_NormalCost);
                        if (costItem.m_nItemCount > 0)
                        {
                            int nExistItemCount = m_pRoleItem->GetItemCount(costItem.m_nItemType, false, false);
                            if (nExistItemCount >= costItem.m_nItemCount)
                            {
                                m_pRoleItem->RemoveItemByType(costItem.m_nItemType, costItem.m_nItemCount, EItemAction_Del_FillPuzzle);

                                int nOriTime = it_Info->second.m_nUpdateTime;
                                int nNowTime = (int)time(NULL);
                                it_Info->second.m_nUpdateTime = nNowTime;
                                it_Info->second.m_nFinishTime = nNowTime;
                                it_Info->second.m_arFragments[rFillMsg.m_nPieceIndex] = '1';

                                for (int i = 0; i < PUZZLE_FRAGMENTS_COUNT; ++i)
                                {
                                    if (it_Info->second.m_arFragments[i] == '0')
                                    {
                                        it_Info->second.m_nFinishTime = 0;
                                        break;
                                    }
                                }

                                if (it_Info->second.m_nFinishTime > 0)
                                {
                                    std::list<CItem>& listGiveItem = (m_pRoleAttr->GetSex() == ESexType_Male ? it_Data->second.m_lstMaleReward : it_Data->second.m_lstFemaleReward);
                                    string mailTitle = CLocalization::Instance().GetString("Puzzle_MailTitle");
                                    string mailContent = CLocalization::Instance().GetString("Puzzle_MailContent");
                                    CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listGiveItem, EItemAction_Add_FinishPuzzle, 0, true, EMailType_Puzzle, mailTitle, mailContent);

                                    if (it_Data->second.m_nMoneyReward > 0)
                                    {
                                        m_pRoleAttr->ChangeMoney(it_Data->second.m_nMoneyReward, EChangeMoneyCause_Add_FinishPuzzle, 0);
                                    }

                                    if (it_Data->second.m_bAnnounce) {
                                        std::string strContent;
                                        SafeFormat(strContent, CLocalization::Instance().GetString("Puzzle_Finish_Announce"),
                                            m_pRoleAttr->GetRoleName(), it_Data->second.m_strTitle.c_str());

                                        CChatManager::Instance().SendAnnounce(true, true, strContent);
                                    }
                                }

                                _UpdateRolePuzzleDB(nOriTime, it_Info->second);

                                GameMsg_S2C_FillPuzzleSuc sucMsg;
                                sucMsg.m_nPuzzleID = it_Info->second.m_nPuzzleID;
                                sucMsg.m_nPieceIndex = rFillMsg.m_nPieceIndex;
                                sucMsg.m_nFinishTime = it_Info->second.m_nFinishTime;
                                _SendPlayerMsg(&sucMsg);
                            }
                            else
                            {
                                nResult = EFillPuzzleError_NoItem;
                            }
                        }
                    }
                    else
                    {
                        nResult = EFillPuzzleError_HasFill;
                    }
                }
            }
        }
    }
    else
    {
        nResult = EFillPuzzleError_NotOpen;
    }

    if (nResult != EFillPuzzleError_Success) {
        GameMsg_S2C_FillPuzzleFail failMsg;
        failMsg.m_nError = nResult;
        _SendPlayerMsg(&failMsg);
    }
}

void CEntityActivityComponent::SyncPuzzleData()
{
    if (!CPuzzleDataMgr::Instance().IsOpening())
        return;

    GameMsg_S2C_SyncPuzzleData syncMsg;
    syncMsg.m_strPuzzleHelpTitle = CPuzzleDataMgr::Instance().m_strPuzzleHelpTitle;
    syncMsg.m_strPuzzleHelpContent = CPuzzleDataMgr::Instance().m_strPuzzleHelpContent;

    const std::map<int, CPuzzleInfo> &rAllConfig = CPuzzleDataMgr::Instance().m_mapAllPuzzle;
    for (std::map<int, CPuzzleInfo>::const_iterator itrConfig = rAllConfig.begin();
        itrConfig != rAllConfig.end(); ++itrConfig)
    {
        CRolePuzzleSync puzzleSync;
        puzzleSync.m_nPuzzleID = itrConfig->first;
        puzzleSync.m_strTitle = itrConfig->second.m_strTitle;
        puzzleSync.m_strTexture = itrConfig->second.m_strTexture;
        puzzleSync.m_NormalCost = itrConfig->second.m_NormalCost;
        puzzleSync.m_SpecialCost = itrConfig->second.m_SpecialCost;
        puzzleSync.m_lstItemReward = (m_pRoleAttr->GetSex() == ESexType_Male ?
            itrConfig->second.m_lstMaleReward : itrConfig->second.m_lstFemaleReward);
        puzzleSync.m_nMoneyReward = itrConfig->second.m_nMoneyReward;

        const CRolePuzzleActivityInfo &rInfo = m_rolePuzzleActivityInfo[itrConfig->first];
        puzzleSync.m_nFinishTime = rInfo.m_nFinishTime;
        memcpy(puzzleSync.m_arFragments, rInfo.m_arFragments, PUZZLE_FRAGMENTS_COUNT + 1);

        syncMsg.m_vecRolePuzzle.push_back(puzzleSync);
    }

    _SendPlayerMsg(&syncMsg);
}

void CEntityActivityComponent::SyncGiftData()
{
    if (!m_GiftMap.empty())
    {
        bool bMale = true;
        if (m_pRoleAttr)
        {
            bMale = m_pRoleAttr->GetSex() == ESexType_Male;
        }
        std::map< int, GiftDB >::iterator it = m_GiftMap.begin();
        for (; it != m_GiftMap.end();)
        {
            int nIndexID = it->first;
            GiftDB & rGiftDB = it->second;
            CGiftInfo * pGiftInfo = CSystemActivityManager::Instance().GiftSystem().GetGiftInfo(rGiftDB.m_nGiftID);
            if (pGiftInfo != NULL)
            {
                std::list< CItem > & rItemList = bMale ? pGiftInfo->m_MaleRewardItemList : pGiftInfo->m_FemaleRewardItemList;
                if (pGiftInfo->m_bSendOnLogin)
                {
                    pGiftInfo->SendGiftToRole(*(CRoleEntity *)Entity());
                    AddQuery(QUERY_Activity_GetGiftReward, nIndexID);
                    GameMsg_S2C_SendGift sendGiftMsg(pGiftInfo->m_nGiftID, rItemList, pGiftInfo->m_nMoneyReward, pGiftInfo->m_strMessage);
                    _SendPlayerMsg(&sendGiftMsg);
                    m_GiftMap.erase(it++);
                }
                else
                {
                    GameMsg_S2C_UpdateGiftInfo updateGiftInfoMsg(nIndexID, rGiftDB.m_nGiftID, rGiftDB.m_nExpiredTime,
                        rItemList, pGiftInfo->m_nMoneyReward);
                    _SendPlayerMsg(&updateGiftInfoMsg);
                    ++it;
                }
            }
            else
            {
                WriteLog(LOGLEVEL_ERROR, "Error: CEntityActivityComponent::_OnLoadGift. Can not find any gift info. GiftID=%d", rGiftDB.m_nGiftID);
                m_GiftMap.erase(it++);
            }
        }
    }
}

void CEntityActivityComponent::_OnLoadGift(QueryBase & rQuery)
{
    std::map< int, GiftDB >* pGiftMap = (std::map< int, GiftDB >*)rQuery.m_pAttachment;
    m_GiftMap = *pGiftMap;

    SyncGiftData();

    delete pGiftMap;
    pGiftMap = NULL;
}

void CEntityActivityComponent::_OnLoadCumulativeRechargeInfo(QueryBase & rQuery)
{
    std::vector<int> *pVecReward = (std::vector<int> *)rQuery.m_pAttachment;
    CCumulativeRechargeDB *pCumulativeRechargeInfo = (CCumulativeRechargeDB *)rQuery.m_pTag;
    if (pVecReward != NULL && pCumulativeRechargeInfo != NULL) {
        m_vecCumulativeRewards = *pVecReward;
        m_nCumulativeRechargeNum = pCumulativeRechargeInfo->m_nRechargeNum;
        m_bLoadCumulativeRechargeDB = true;
    }

    delete (std::vector<int> *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
    delete (CCumulativeRechargeDB *)rQuery.m_pTag, rQuery.m_pTag = NULL;
}

void CEntityActivityComponent::_OnLoadCumulativeSpendInfo(QueryBase & rQuery)
{
    std::vector<unsigned int> *pVecReward = (std::vector<unsigned int> *)rQuery.m_pAttachment;
    CCumulativeSpendDB *pSpendInfo = (CCumulativeSpendDB *)rQuery.m_pTag;
    if (pVecReward != NULL && pSpendInfo != NULL)
    {
        m_CumulativeSpendReward = *pVecReward;
        m_nCumulativeBill = pSpendInfo->m_nBillNum;
        m_nCumulativeBindBill = pSpendInfo->m_nBindBillNum;

        m_bLoadCumulativeSpendSuc = true;
    }

    delete (std::vector<unsigned int> *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
    delete (CCumulativeSpendDB *)rQuery.m_pTag, rQuery.m_pTag = NULL;
};

void CEntityActivityComponent::_OnLoadCumulativeSpendInfoForMedal(QueryBase& rQuery)
{
    std::vector<unsigned int> *pVecReward = (std::vector<unsigned int> *)rQuery.m_pAttachment;
    CCumulativeSpendDB *pSpendInfo = (CCumulativeSpendDB *)rQuery.m_pTag;
    if (pVecReward != NULL && pSpendInfo != NULL)
    {
        m_CumulativeSpendRewardForMedal = *pVecReward;
        m_nCumulativeBillForMedal = pSpendInfo->m_nBillNum;
        m_nCumulativeBindBillForMedal = pSpendInfo->m_nBindBillNum;
    }

    delete (std::vector<unsigned int> *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
    delete (CCumulativeSpendDB *)rQuery.m_pTag, rQuery.m_pTag = NULL;
}

void CEntityActivityComponent::_OnDBLoadXmasFreeRewardInfo(QueryBase & rQuery)
{
    CXmasFreeRewardDB *pXmasFreeRewardDB = (CXmasFreeRewardDB *)rQuery.m_pAttachment;
    if (pXmasFreeRewardDB != NULL) {
        if (rQuery.m_nResult == 0) {
            m_roleXmasFreeRewardInfo.m_nLastGetRewardTime = pXmasFreeRewardDB->m_nLastGetRewardTime;
            m_roleXmasFreeRewardInfo.m_vecFreeReward = pXmasFreeRewardDB->m_vecReward;

            if (m_roleXmasFreeRewardInfo.m_nLastGetRewardTime != 0 && m_roleXmasFreeRewardInfo.m_nLastGetRewardTime < (unsigned int)time(NULL) && !IsTimeIsToday(m_roleXmasFreeRewardInfo.m_nLastGetRewardTime)) {
                m_roleXmasFreeRewardInfo.Reset();
                _SaveXmasFreeRewardInfo(0);
            }
        }
    }

    delete (CXmasFreeRewardDB *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
}

void CEntityActivityComponent::_OnDBLoadRoleBlessActivityInfo(QueryBase & rQuery)
{
    CRoleBlessActivityDB *pBlessInfoDb = (CRoleBlessActivityDB *)rQuery.m_pAttachment;
    if (pBlessInfoDb != NULL) {
        if (rQuery.m_nResult == 0) {
            m_roleBlessActivityInfo.m_nBeBlessedCount = pBlessInfoDb->m_nBeBlessedCount;
            m_roleBlessActivityInfo.m_listBlesses = pBlessInfoDb->m_listBlesses;
            if (m_roleBlessActivityInfo.m_listBlesses.size() > BLESS_SHOW_COUNT) {
                m_roleBlessActivityInfo.m_listBlesses.resize(BLESS_SHOW_COUNT);
            }
            m_roleBlessActivityInfo.m_vecRewards = pBlessInfoDb->m_vecRewards;
        }
    }

    delete (CRoleBlessActivityDB *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
}

void CEntityActivityComponent::_OnDBLoadRoleFestivalWishActivityInfo(QueryBase & rQuery)
{
    CFestivalWishInfoDB *pWishInfoDB = (CFestivalWishInfoDB  *)rQuery.m_pAttachment;
    if (pWishInfoDB != NULL) {
        if (rQuery.m_nResult == 0) {
            m_roleFestivalWishActivityInfo.m_nWishedCount = pWishInfoDB->m_nWishCount;
            m_roleFestivalWishActivityInfo.m_nLastGetRewardTime = pWishInfoDB->m_nLastGetRewardTime;
        }

        if (m_roleFestivalWishActivityInfo.m_nLastGetRewardTime != 0 && m_roleFestivalWishActivityInfo.m_nLastGetRewardTime < (unsigned int)time(NULL) && !IsTimeIsToday(m_roleFestivalWishActivityInfo.m_nLastGetRewardTime)) {
            m_roleFestivalWishActivityInfo.Reset();
            _SaveFestivalWishInfo();
        }
    }

    delete (CFestivalWishInfoDB *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
}

void CEntityActivityComponent::_OnDBLoadRolePuzzleActivityInfo(QueryBase & rQuery)
{
    CRoleAllPuzzleDB *pPuzzleDB = (CRoleAllPuzzleDB *)rQuery.m_pAttachment;
    if (pPuzzleDB != NULL)
    {
        _InitRolePuzzleInfo(pPuzzleDB);
    }

    delete (CRoleAllPuzzleDB *)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
}

void CEntityActivityComponent::OnQueryCumulativeSpendGotMedalReward(QueryBase &rQuery)
{
    std::vector<bool> *pGotRewardInfo = (std::vector<bool> *)rQuery.m_pAttachment;

    if (pGotRewardInfo != NULL)
    {
        m_vectCumulativeSpendGotMedalReward.swap(*pGotRewardInfo);
    }

    delete pGotRewardInfo, rQuery.m_pAttachment = pGotRewardInfo = NULL;
}

void CEntityActivityComponent::OnQueryBuyItemGotMedalRewardedInfo(QueryBase &rQuery)
{
    BuyItemGotMedalRewardedInfoMap *pRewardedInfo = (BuyItemGotMedalRewardedInfoMap *)rQuery.m_pAttachment;

    if (pRewardedInfo != NULL && !pRewardedInfo->empty())
    {
        m_mapBuyItemGotMedalRewardedInfo.swap(*pRewardedInfo);
    }

    delete pRewardedInfo, rQuery.m_pAttachment = pRewardedInfo = NULL;
}

void CEntityActivityComponent::_SendPlayerMsg(GameMsg_Base *pMsg)
{
    if (m_pRoleNet != NULL && pMsg != NULL) {
        m_pRoleNet->SendPlayerMsg(pMsg);
    }
}

void CEntityActivityComponent::_SetNextKeepOnlineReward(int nIndex, CKeepOnlineReward & onlineInfo, bool bCanReset)
{
    if (nIndex > -1) {
        ++nIndex;
        COnlineRewardInfo *pInfo = COnlineRewardInfoMgr::Instance().GetOnlineRewardInfo(nIndex);
        if (pInfo != NULL)
        {
            onlineInfo.m_nIndex = pInfo->m_nIndex;
            onlineInfo.m_nRequireTime = pInfo->m_nRequireTime;
            onlineInfo.m_nCumulativeTime = 0;
            onlineInfo.m_nTimingDay = (unsigned int)time(NULL);
            onlineInfo.m_bFinished = false;
            onlineInfo.m_bValid = true;
            onlineInfo.m_bGotReward = false;
            _HandleOnlineInfo(onlineInfo, (unsigned int)time(NULL));
        }
        else if (bCanReset)
        {
            memset(&onlineInfo, 0, sizeof(onlineInfo));
        }
    }
}

void CEntityActivityComponent::_ResetKeepOnlineRewardInfo()
{
    m_keepOnlineRewardInfo.m_nIndex = 0;
    m_keepOnlineRewardInfo.m_nRequireTime = 0;
    m_keepOnlineRewardInfo.m_nCumulativeTime = 0;
    m_keepOnlineRewardInfo.m_nTimingDay = 0;
    m_keepOnlineRewardInfo.m_bFinished = false;
    m_keepOnlineRewardInfo.m_bValid = false;
}

void CEntityActivityComponent::_UpdateOnlineRewardInfoDB()
{
    if (m_pRoleAttr != NULL) {
        CKeepOnlineReward *pInfo = new CKeepOnlineReward(m_keepOnlineRewardInfo);
        if (pInfo != NULL) {
            CDelHelper delHelper(pInfo);
            AddQuery(QUERY_UpdateOnlineRewardInfo, m_pRoleAttr->GetRoleID(), pInfo, 0, NULL, &delHelper);
        }
    }
}

void CEntityActivityComponent::_HandleOnlineInfo(CKeepOnlineReward & onlineRewardInfo, unsigned int nNow)
{
    COnlineRewardInfo *pInfo = COnlineRewardInfoMgr::Instance().GetOnlineRewardInfo(m_keepOnlineRewardInfo.m_nIndex);

    int nRemainTime = onlineRewardInfo.m_nRequireTime - onlineRewardInfo.m_nCumulativeTime;
    if (nRemainTime > 0) {
        if (nRemainTime < (int)(::GetNextDayStartTime(nNow) - nNow)) { // reward required left time larger then the daytime left
            int nNotifyTime = nNow + nRemainTime;
            m_pRoleTimer->AddTimer(nNotifyTime, eComponentEvent_KeepOnlineReward_Finished, (unsigned short)pInfo->m_nIndex);
        }
    }
    else {
        m_keepOnlineRewardInfo.m_bFinished = true;
    }

}
void CEntityActivityComponent::OnOnlineRewardActivityStart()
{
    _ResetKeepOnlineRewardInfo();
    _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo);
    if (m_keepOnlineRewardInfo.m_bValid) {
        _UpdateOnlineRewardInfoDB();
        SendAllActivitiesInfo();
    }
    UpdateOnlineRewardInfoToClient();
}

void CEntityActivityComponent::OnOnlineRewardActivityEnd()
{
    _ResetKeepOnlineRewardInfo();
    m_keepOnlineRewardInfo.m_bValid = false;
    UpdateOnlineRewardInfoToClient();
}

void CEntityActivityComponent::_OnOnlineRewardActivityReset()
{
    if (_OnlineRewardActivityActivated()) {
        _ResetKeepOnlineRewardInfo();
        _SetNextKeepOnlineReward(m_keepOnlineRewardInfo.m_nIndex, m_keepOnlineRewardInfo);
        if (m_keepOnlineRewardInfo.m_bValid) {
            _UpdateOnlineRewardInfoDB();
            if (m_pRoleAttr != NULL) {
                UpdateOnlineRewardInfoToClient();
            }
        }
    }
}

void CEntityActivityComponent::OnCumulativeRechargeActivityStart()
{
    //GameMsg_S2C_UpdateCumulativeActivityState msgstate;
    //_SetCumulativeRechargeActivity(msgstate.m_CumulativeRechargeActivity);

    //_SendPlayerMsg(&msgstate);
}

void CEntityActivityComponent::OnCumulativeRechargeActivityEnd()
{

}

void CEntityActivityComponent::_SetCumulativeSpendActivity(CCummulativeSpendActivityInfo & activity)
{
    activity.m_nActivityID = EActivity_CumulativeSpend;
    activity.m_bExhibit = CCumulativeSpendMgr::Instance().Exhibit();
    activity.m_nWeight = CCumulativeSpendMgr::Instance().Weight();
    activity.m_nSpendType = (char)CCumulativeSpendMgr::Instance().GetCurrencyType();
    activity.m_strRegularImageName = CCumulativeSpendMgr::Instance().RegularImageName();
    activity.m_strThumbnailsName = CCumulativeSpendMgr::Instance().ThumbnailsName();
    activity.m_nSpendStartTime = CCumulativeSpendMgr::Instance().SpendStartTime();
    activity.m_nSpendStopTime = CCumulativeSpendMgr::Instance().SpendStopTime();
    activity.m_nCurRechargeNum = GetCumulativeSpendValue(CCumulativeSpendMgr::Instance().GetCurrencyType());
    activity.m_strActivityName = CCumulativeSpendMgr::Instance().ActivityName();
    activity.m_strDetail = CCumulativeSpendMgr::Instance().Detail();

    for (CCumulativeSpendReward_t::const_iterator itr = CCumulativeSpendMgr::Instance().RewardMap().begin(); itr != CCumulativeSpendMgr::Instance().RewardMap().end(); ++itr)
    {
        if (itr->second != NULL)
        {
            CCumulativeSpendRewardSend cumulativeReward;
            cumulativeReward.m_nLevel = itr->second->m_nLevel;
            cumulativeReward.m_nRequiredNum = itr->second->m_nRequiredNum;
            cumulativeReward.m_nMoney = itr->second->m_nMoney;
            cumulativeReward.m_nBindMCoin = itr->second->m_nBindMCoin;
            cumulativeReward.m_bHasReceived = _HasReceivedCumulativeSpendReward(itr->second->m_nLevel);

            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                cumulativeReward.m_listItem = itr->second->m_listMaleReward;
            }
            else if (m_pRoleAttr->GetSex() == ESexType_Female)
            {
                cumulativeReward.m_listItem = itr->second->m_listFemaleReward;
            }
            else
            {
                WriteLog(LOGLEVEL_ERROR, "_SetCumulativeRechargeActivity get player(roleid:%u) sex error\n", m_pRoleAttr->GetRoleID());
            }

            activity.m_listCumulativeReward.push_back(cumulativeReward);
        }
    }
}

void CEntityActivityComponent::_SetCumulativeRechargeActivity(CCummulativeRechareActivityInfo & activity)
{
    if (m_pRoleAttr != NULL)
    {
        activity.m_nActivityID = EActivity_CumulativeRecharge;
        activity.m_bExhibit = CCumulativeRechargeMgr::Instance().Exhibit();
        activity.m_nWeight = CCumulativeRechargeMgr::Instance().Weight();
        activity.m_strRegularImageName = CCumulativeRechargeMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CCumulativeRechargeMgr::Instance().ThumbnailsName();
        activity.m_nRechargeStartTime = CCumulativeRechargeMgr::Instance().RechargeStartTime();
        activity.m_nRechargeStopTime = CCumulativeRechargeMgr::Instance().RechargeStopTime();
        activity.m_nCurRechargeNum = m_nCumulativeRechargeNum;
        activity.m_strActivityName = CCumulativeRechargeMgr::Instance().ActivityName();
        activity.m_strDetail = CCumulativeRechargeMgr::Instance().Detail();
        for (CumulativeReward_t::const_iterator itr = CCumulativeRechargeMgr::Instance().RewardMap().begin(); itr != CCumulativeRechargeMgr::Instance().RewardMap().end(); ++itr)
        {
            if (itr->second != NULL)
            {
                CCumulativeRechargeRewardSend cumulativeReward;
                cumulativeReward.m_nLevel = itr->second->m_nLevel;
                cumulativeReward.m_nRequiredNum = itr->second->m_nRequiredNum;
                cumulativeReward.m_nMoney = itr->second->m_nMoney;
                cumulativeReward.m_nBindMCoin = itr->second->m_nBindMCoin;
                cumulativeReward.m_bHasReceived = _HasReceivedCumulativeReward(itr->second->m_nLevel);

                if (m_pRoleAttr->GetSex() == ESexType_Male)
                {
                    cumulativeReward.m_listItem = itr->second->m_listMaleReward;
                }
                else if (m_pRoleAttr->GetSex() == ESexType_Female)
                {
                    cumulativeReward.m_listItem = itr->second->m_listFemaleReward;
                }
                else
                {
                    WriteLog(LOGLEVEL_ERROR, "_SetCumulativeRechargeActivity get player(roleid:%u) sex error\n", m_pRoleAttr->GetRoleID());
                }

                activity.m_listCumulativeReward.push_back(cumulativeReward);
            }
        }
    }
}

bool CEntityActivityComponent::_HasReceivedCumulativeReward(int nLevel)
{
    if (nLevel >= 0 && nLevel - 1 < (int)m_vecCumulativeRewards.size())
    {
        return m_vecCumulativeRewards[nLevel - 1] == 1;
    }

    return true;
}

bool CEntityActivityComponent::_HasReceivedCumulativeSpendReward(int nLevel)
{
    if (nLevel >= 0 && nLevel <= (int)m_CumulativeSpendReward.size())
    {
        unsigned int nBeginTime = CCumulativeSpendMgr::Instance().ActivityBeginTime();
        unsigned int nEndTime = CCumulativeSpendMgr::Instance().ActivityEndTime();

        if (m_CumulativeSpendReward[nLevel - 1] < nBeginTime || m_CumulativeSpendReward[nLevel - 1] > nEndTime)
        {
            return false;
        }
    }

    return true;
}

bool CEntityActivityComponent::IsSpendRewardEmpty()
{
    for (size_t i = 0; i < m_CumulativeSpendReward.size(); ++i)
    {
        if (m_CumulativeSpendReward[i] > 0)
        {
            return false;
        }
    }

    return true;
}

void CEntityActivityComponent::SetCumulativeSpendGotMedalActivity(CumulativeSpendGotMedalActivityData &rActivity)
{
    if (m_pRoleAttr != NULL && m_pRoleMedal->IsOpen())
    {
        CumulativeSpendGotMedalMgr &rMgr = CumulativeSpendGotMedalMgr::Instance();

        rActivity.m_nActivityID = EActivity_CumulativeSpendGotMedal;
        rActivity.m_bExhibit = rMgr.HasExhibit();
        rActivity.m_nWeight = rMgr.GetWeight();
        rActivity.m_nOpenTime = rMgr.GetOpenTime();
        rActivity.m_nCloseTime = rMgr.GetCloseTime();
        rActivity.m_strTitle = rMgr.GetTitle();
        rActivity.m_strContent = rMgr.GetContent();
        rActivity.m_strSmallImage = rMgr.GetSmallImage();
        rActivity.m_strBigImage = rMgr.GetBigImage();
        rActivity.m_nCumulativeType = rMgr.GetCumulativeType();
        rActivity.m_nCumulativeAmount = GetCumulativeSpendValueForMedal(rActivity.m_nCumulativeType);

        const std::map<unsigned char, CumulativeSpendGotMedalRewards> &rRewards = rMgr.GetAllRewardsInfo();
        for (std::map<unsigned char, CumulativeSpendGotMedalRewards>::const_iterator itr = rRewards.begin();
            itr != rRewards.end(); ++itr)
        {
            CumulativeSpendGotMedalRewardData data;
            data.m_nIndex = itr->first;
            data.m_nRequiredNum = itr->second.GetRequiredNum();
            data.m_bHasReceived = HasReceivedCumulativeSpendGotMedalReward(itr->first);

            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                data.m_listReward = itr->second.GetMaleReward();
            }
            else
            {
                data.m_listReward = itr->second.GetFemaleReward();
            }

            rActivity.m_listReward.push_back(data);
        }
    }
}

bool CEntityActivityComponent::HasReceivedCumulativeSpendGotMedalReward(int nIndex)
{
    if (0 <= nIndex && nIndex < (int)m_vectCumulativeSpendGotMedalReward.size())
    {
        return m_vectCumulativeSpendGotMedalReward[nIndex];
    }

    return true;
}

void CEntityActivityComponent::OnRequestBuyItemGotMedalReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestBuyItemGotMedalReward &rRequestMsg = (GameMsg_C2S_RequestBuyItemGotMedalReward &)msg;
    std::string strError("");

    if (m_pRoleAttr == NULL || m_pRoleMedal == NULL)
        return;

    if (!m_pRoleMedal->IsOpen())
    {
        return;
    }

    BuyItemGotMedalMgr &rMgr = BuyItemGotMedalMgr::Instance();

    if (!rMgr.HasOpening())
    {
        strError = CLocalization::Instance().GetString("GetActivityRewardsError1");
    }
    else if (!rMgr.IsValidIndex(rRequestMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("GetActivityRewardsError2");
    }
    else if (HasReceivedBuyItemGotMedalReward(rRequestMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("GetActivityRewardsError3");
    }
    else if (!CanGetBuyItemGotMedalRewards(rRequestMsg.m_nIndex))
    {
        strError = CLocalization::Instance().GetString("GetActivityRewardsError4");
    }
    else
    {
        std::list<int> listRewards;
        rMgr.GetRewards(rRequestMsg.m_nIndex, m_pRoleAttr->GetSex(), listRewards);

        m_pRoleMedal->SetMedalActivited(listRewards, CMedalLog::EMedalActivityBuyItem);

        m_mapBuyItemGotMedalRewardedInfo[rRequestMsg.m_nIndex].m_bRewarded = true;

        BuyItemGotMedalDB *pInfo = new BuyItemGotMedalDB;
        pInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
        pInfo->m_nOpenTime = rMgr.GetOpenTime();
        pInfo->m_nCloseTime = rMgr.GetCloseTime();
        pInfo->m_nIndex = rRequestMsg.m_nIndex;
        AddQuery(QUERY_SetBuyItemGotMedalRewarded, 0, pInfo);

        GameMsg_S2C_RequestBuyItemGotMedalRewardSuc msgSuc;
        msgSuc.m_nIndex = rRequestMsg.m_nIndex;
        _SendPlayerMsg(&msgSuc);
    }

    if (!strError.empty())
    {
        GameMsg_S2C_RequestBuyItemGotMedalRewardFail msgFail;
        msgFail.m_strError = strError;
        _SendPlayerMsg(&msgFail);
    }
}

void CEntityActivityComponent::SetBuyItemGotMedalActivity(BuyItemGotMedalActivityData &rActivity) const
{
    if (m_pRoleAttr != NULL && m_pRoleMedal->IsOpen())
    {
        BuyItemGotMedalMgr &rMgr = BuyItemGotMedalMgr::Instance();

        rActivity.m_nActivityID = EActivity_BuyItemGotMedal;
        rActivity.m_bExhibit = rMgr.HasExhibit();
        rActivity.m_nWeight = rMgr.GetWeight();
        rActivity.m_nOpenTime = rMgr.GetOpenTime();
        rActivity.m_nCloseTime = rMgr.GetCloseTime();
        rActivity.m_strTitle = rMgr.GetTitle();
        rActivity.m_strContent = rMgr.GetContent();
        rActivity.m_strSmallImage = rMgr.GetSmallImage();
        rActivity.m_strBigImage = rMgr.GetBigImage();

        const std::map<unsigned char, BuyItemGotMedalRewards> &rRewards = rMgr.GetAllRewardsInfo();
        for (std::map<unsigned char, BuyItemGotMedalRewards>::const_iterator itr = rRewards.begin();
            itr != rRewards.end(); ++itr)
        {
            BuyItemGotMedalRewardData data;
            data.m_nIndex = itr->first;
            data.m_nItemType = itr->second.GetItemType();
            data.m_nItemCount = itr->second.GetItemCount();
            GetBuyItemGotMedalRewardedInfo(itr->first, &data.m_bHasReceived, &data.m_nCurrentCount);

            if (m_pRoleAttr->GetSex() == ESexType_Male)
            {
                data.m_listReward = itr->second.GetMaleReward();
            }
            else
            {
                data.m_listReward = itr->second.GetFemaleReward();
            }

            rActivity.m_listReward.push_back(data);
        }
    }
}

bool CEntityActivityComponent::HasReceivedBuyItemGotMedalReward(unsigned char nIndex) const
{
    bool bReceived = true;

    GetBuyItemGotMedalRewardedInfo(nIndex, &bReceived, NULL);

    return bReceived;
}

bool CEntityActivityComponent::CanGetBuyItemGotMedalRewards(unsigned char nIndex) const
{
    BuyItemGotMedalRewardedInfoMap::const_iterator itr = m_mapBuyItemGotMedalRewardedInfo.find(nIndex);

    if (itr == m_mapBuyItemGotMedalRewardedInfo.end())
        return false;

    return BuyItemGotMedalMgr::Instance().CanGetRewards(nIndex, itr->second.m_nBuyNum);
}

void CEntityActivityComponent::GetBuyItemGotMedalRewardedInfo(unsigned char nIndex, bool *rReceived, unsigned short *rCurrentCount) const
{
    bool bReceived = true;
    unsigned short nCurrentCount = 0;

    if (BuyItemGotMedalMgr::Instance().IsValidIndex(nIndex))
    {
        BuyItemGotMedalRewardedInfoMap::const_iterator itr = m_mapBuyItemGotMedalRewardedInfo.find(nIndex);

        if (itr != m_mapBuyItemGotMedalRewardedInfo.end())
        {
            bReceived = itr->second.m_bRewarded;
            nCurrentCount = itr->second.m_nBuyNum;
        }
        else
        {
            bReceived = false;
        }
    }

    if (rReceived != NULL)
        *rReceived = bReceived;
    if (rCurrentCount != NULL)
        *rCurrentCount = nCurrentCount;
}

void CEntityActivityComponent::LoadBuyItemGotMedalRewardedInfo()
{
    if (m_pRoleAttr == NULL)
        return;

    BuyItemGotMedalRewardedInfoMap *pRewardedInfo = new BuyItemGotMedalRewardedInfoMap;

    BuyItemGotMedalDB *pInfo = new BuyItemGotMedalDB;
    pInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
    pInfo->m_nOpenTime = BuyItemGotMedalMgr::Instance().GetOpenTime();
    pInfo->m_nCloseTime = BuyItemGotMedalMgr::Instance().GetCloseTime();

    CDelHelper delHelper(pRewardedInfo);
    AddQuery(QUERY_LoadBuyItemGotMedalRewardedInfo, 0, pRewardedInfo, 0, pInfo, &delHelper);
}

void CEntityActivityComponent::UpdateBuyItemGotMedalInfo(unsigned int nItemType, unsigned short nItemCount)
{
    if (nItemCount == 0 || m_pRoleAttr == NULL)
        return;

    BuyItemGotMedalMgr &rMgr = BuyItemGotMedalMgr::Instance();
    const std::map<unsigned char, BuyItemGotMedalRewards> &rRewards = rMgr.GetAllRewardsInfo();
    for (std::map<unsigned char, BuyItemGotMedalRewards>::const_iterator itr = rRewards.begin();
        itr != rRewards.end(); ++itr)
    {
        if (nItemType == itr->second.GetItemType())
        {
            m_mapBuyItemGotMedalRewardedInfo[itr->first].m_nBuyNum += nItemCount;

            BuyItemGotMedalDB *pInfo = new BuyItemGotMedalDB;
            pInfo->m_nRoleID = m_pRoleAttr->GetRoleID();
            pInfo->m_nOpenTime = rMgr.GetOpenTime();
            pInfo->m_nCloseTime = rMgr.GetCloseTime();
            pInfo->m_nIndex = itr->first;
            pInfo->m_nBuyNum = m_mapBuyItemGotMedalRewardedInfo[itr->first].m_nBuyNum;

            if (pInfo->m_nBuyNum == nItemCount)
            {
                AddQuery(QUERY_InsertBuyItemGotMedalInfo, 0, pInfo);
            }
            else
            {
                AddQuery(QUERY_UpdateBuyItemGotMedalInfo, 0, pInfo);
            }

            GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount msg;
            msg.m_nIndex = itr->first;
            msg.m_nCurrentCount = pInfo->m_nBuyNum;
            _SendPlayerMsg(&msg);
        }
    }
}

void CEntityActivityComponent::_SetExchangeItemActivity(CExchangeItemActivityInfo & activity)
{
    if (m_pRoleAttr != NULL && m_pRoleItem != NULL) {
        activity.m_nActivityID = EActivity_ExchangeItem;
        activity.m_bExhibit = CExchangeItemInfoMgr::Instance().Exhibit();
        activity.m_nWeight = CExchangeItemInfoMgr::Instance().Weight();
        activity.m_strRegularImageName = CExchangeItemInfoMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CExchangeItemInfoMgr::Instance().ThumbnailsName();
        activity.m_nActivityBeginTime = (unsigned int)CExchangeItemInfoMgr::Instance().ActivityStartTime();
        activity.m_nActivityEndTime = (unsigned int)CExchangeItemInfoMgr::Instance().ActivityEndTime();
        activity.m_strActivityName = CExchangeItemInfoMgr::Instance().ActivityName();
        activity.m_strActivityIntro = CExchangeItemInfoMgr::Instance().ActivityIntro();

        activity.m_nRequiredItemType = CExchangeItemInfoMgr::Instance().RequiredItemType();
        activity.m_nCurRequiredItemNum = m_pRoleItem->GetItemCount(CExchangeItemInfoMgr::Instance().RequiredItemType(), false, false);

        for (ExchangeItem_t::const_iterator itr = CExchangeItemInfoMgr::Instance().ExchangeInfoMap().begin(); itr != CExchangeItemInfoMgr::Instance().ExchangeInfoMap().end(); ++itr) {
            if (itr->second != NULL) {
                CExchangeItemRewardSend exchangeReward;
                exchangeReward.m_nLevel = itr->second->m_nLevel;
                exchangeReward.m_nRequiredItemType = CExchangeItemInfoMgr::Instance().RequiredItemType();
                exchangeReward.m_nRequiredItemNum = itr->second->m_nRequiredItemNum;
                exchangeReward.m_nMoney = itr->second->m_nMoney;
                if (m_pRoleAttr->GetSex() == ESexType_Male) {
                    exchangeReward.m_listItem = itr->second->m_listMaleReward;
                }
                else if (m_pRoleAttr->GetSex() == ESexType_Female) {
                    exchangeReward.m_listItem = itr->second->m_listFemaleReward;
                }
                else {
                    WriteLog(LOGLEVEL_ERROR, "_SetExchangeItemActivity get player(roleid:%u) sex error\n", m_pRoleAttr->GetRoleID());
                }

                activity.m_listExchangeItemReward.push_back(exchangeReward);
            }
        }
    }
}

bool CEntityActivityComponent::_ReachExchangeItemRequire(int nLevel)
{
    const CExchangeItemInfo * pExchangeInfo = CExchangeItemInfoMgr::Instance().GetExchangeItemInfo(nLevel);
    if (pExchangeInfo != NULL && m_pRoleItem != NULL) {
        int nCurItemCount = m_pRoleItem->GetItemCount(CExchangeItemInfoMgr::Instance().RequiredItemType(), false, false);
        return nCurItemCount >= pExchangeInfo->m_nRequiredItemNum;
    }

    return false;
}

void CEntityActivityComponent::_SetInTimeOnlineActivity(CShowInTimeOnlineActivity & activity)
{
    if (m_pRoleAttr != NULL && m_pRoleItem != NULL) {
        activity.m_nActivityID = EActivity_InTimeOnlineReward;
        activity.m_bExhibit = CInTimeOnlineInfoMgr::Instance().Exhibit();
        activity.m_nWeight = CInTimeOnlineInfoMgr::Instance().Weight();
        activity.m_strRegularImageName = CInTimeOnlineInfoMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CInTimeOnlineInfoMgr::Instance().ThumblnailsName();
        activity.m_strActivityName = CInTimeOnlineInfoMgr::Instance().ActivityName();
        activity.m_strActivityIntro = CInTimeOnlineInfoMgr::Instance().ActivityDetail();

        for (InTimeActivityInfo_t::const_iterator itr = CInTimeOnlineInfoMgr::Instance().Activities().begin(); itr != CInTimeOnlineInfoMgr::Instance().Activities().end(); ++itr) 
        {
            if (*itr != NULL) 
            {
                CInTimeOnlineActivitySend activityDetail;
                activityDetail.m_tActivityBeginTime = (*itr)->m_tActivityBeginTime;
                activityDetail.m_tActivityEndTime = (*itr)->m_tActivityEndTime;
                activityDetail.m_nTriggerHour = (*itr)->m_nTriggerHour;
                activityDetail.m_nTriggerMin = (*itr)->m_nTriggerMin;
                activityDetail.m_nMoney = (*itr)->m_nMoney;


                if (m_pRoleAttr->GetSex() == ESexType_Male) {
                    activityDetail.m_listRewards = (*itr)->m_listMaleItems;
                }
                else if (m_pRoleAttr->GetSex() == ESexType_Female) {
                    activityDetail.m_listRewards = (*itr)->m_listFemaleItems;
                }
                else {
                    WriteLog(LOGLEVEL_ERROR, "_SetInTimeOnlineActivity get player(roleid:%u) sex error\n", m_pRoleAttr->GetRoleID());
                }

                activity.m_listActivities.push_back(activityDetail);
            }
        }
    }
}

void CEntityActivityComponent::_SetPlainTextActivity(CPlainTextActivityInfo & activity)
{
    if (m_pRoleAttr != NULL) {
        activity.m_nActivityID = EActivity_PlainTextAcitivity;
        activity.m_bExhibit = CPlainTextActivityInfoMgr::Instance().Exhibit();
        activity.m_nWeight = CPlainTextActivityInfoMgr::Instance().Weight();
        activity.m_strRegularImageName = CPlainTextActivityInfoMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CPlainTextActivityInfoMgr::Instance().ThumbnailsName();
        activity.m_strActivityTitle = CPlainTextActivityInfoMgr::Instance().ActivityTitle();
        activity.m_strActivityContent = CPlainTextActivityInfoMgr::Instance().ActivityContent();

        for (PlainTextRewardContainer_t::const_iterator itr = CPlainTextActivityInfoMgr::Instance().Rewards().begin(); itr != CPlainTextActivityInfoMgr::Instance().Rewards().end(); ++itr) {
            CPlainTextActivityRewardSend plainTextReward;
            plainTextReward.m_strRewardName = itr->m_strRewardName;
            if (m_pRoleAttr->GetSex() == ESexType_Male) {
                plainTextReward.m_listItems = itr->m_listMaleReward;
            }
            else if (m_pRoleAttr->GetSex() == ESexType_Female) {
                plainTextReward.m_listItems = itr->m_listFemaleReward;
            }
            else {
                WriteLog(LOGLEVEL_ERROR, "_SetPlainTextActivity get player(roleid:%u) sex error\n", m_pRoleAttr->GetRoleID());
            }

            plainTextReward.m_nMoney = itr->m_nMoney;
            activity.m_listRewards.push_back(plainTextReward);
        }
    }
}

void CEntityActivityComponent::_SetGiftCodeActivity(CGiftCodeActivityInfo & activity)
{
    if (m_pRoleAttr != NULL) {
        activity.m_nActivityID = EActivity_GiftCodeAcitivity;
        activity.m_bExhibit = CGiftCodeInfoMgr::Instance().Exhibit();
        activity.m_nWeight = CGiftCodeInfoMgr::Instance().Weight();
        activity.m_strRegularImageName = CGiftCodeInfoMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CGiftCodeInfoMgr::Instance().ThumbnailsName();
        activity.m_strActivityTitle = CGiftCodeInfoMgr::Instance().ActivityTitle();
        activity.m_strActivityContent = CGiftCodeInfoMgr::Instance().ActivityContent();
    }
}

void CEntityActivityComponent::_SetOnlineRewardActivity(COnlineRewardActivity & activity)
{
    bool bHasAnyActivity = COnlineRewardInfoMgr::Instance().ActivityTimeToShow(activity.m_nActivityBeginTime, activity.m_nActivityEndTime, activity.m_strContent);
    if (!bHasAnyActivity)
    {
        return;
    }

    activity.m_nActivityID = EActivity_OnlineReward;
    activity.m_bExhibit = COnlineRewardInfoMgr::Instance().Exhibit();
    activity.m_nWeight = COnlineRewardInfoMgr::Instance().Weight();
    activity.m_strRegularImageName = COnlineRewardInfoMgr::Instance().RegularImageName();
    activity.m_strThumbnailName = COnlineRewardInfoMgr::Instance().ThumbnailName();
    activity.m_strActivityName = COnlineRewardInfoMgr::Instance().ActivityName();

    if (!(COnlineRewardInfoMgr::Instance().IsEndActivityIndex(m_keepOnlineRewardInfo.m_nIndex) && m_keepOnlineRewardInfo.m_bGotReward))
    {
        activity.m_nextRewardInfo.m_onlineRewardInfo = m_keepOnlineRewardInfo;
    }
   
    const std::map<unsigned short, COnlineRewardInfo *> & onlineRewardInfoMap = COnlineRewardInfoMgr::Instance().GetMapOnlineRewardInfoToShow();
    for (std::map<unsigned short, COnlineRewardInfo *>::const_iterator itr = onlineRewardInfoMap.begin(); itr != onlineRewardInfoMap.end(); ++itr)
    {
        if (itr->second != NULL)
        {
            COnlineRewardInfoSend rewardInfoSend;
            rewardInfoSend.m_nIndex = itr->second->m_nIndex;
            rewardInfoSend.m_nRequireTime = itr->second->m_nRequireTime;
            rewardInfoSend.m_listRewards = m_pRoleAttr->GetSex() == ESexType_Male ? itr->second->m_listMaleReward : itr->second->m_listFemaleReward;
            rewardInfoSend.m_nMoney = itr->second->m_nMoney;
            rewardInfoSend.m_bAlreadyGot = m_keepOnlineRewardInfo.m_bGotReward;
            activity.m_listOnlineRewardInfo.push_back(rewardInfoSend);
        }
    }
}

bool CEntityActivityComponent::_OnlineRewardActivityActivated()
{
    return COnlineRewardInfoMgr::Instance().IsOpening();
}

void CEntityActivityComponent::UpdateOnlineRewardInfoToClient()
{
    GameMsg_S2C_UpdateOnlineRewardInfo msgUpdate;
    msgUpdate.m_onlieReward.m_onlineRewardInfo = m_keepOnlineRewardInfo;
    _SendPlayerMsg(&msgUpdate);
}

void CEntityActivityComponent::OnPlayerRecharge(int nCharge)
{
    if (nCharge > 0)
    {
        m_nCumulativeRechargeNum += nCharge;
        GameMsg_S2C_UpdateCumulativeRechargeNum msgUpdate;
        msgUpdate.m_nRechargeNum = m_nCumulativeRechargeNum;
        _SendPlayerMsg(&msgUpdate);
    }
}

void CEntityActivityComponent::OnTestActivity(int nPara1, int nPara2, int nPara3)
{
    //int nIndex = nPara1;
    int nIndex = -1;
    CSlotPeer slotPeer;
    switch (nIndex)
    {
    case 0:
    {
              GameMsg_C2S_GetXmasFreeRewardActivityInfo msgGetInfo;
              _OnGetXmasActivityInfo(msgGetInfo, slotPeer);
    }
        break;
    case  1:
    {
               GameMsg_C2S_GetFestivalWishInfo msgGetInfo;
               _OnGetFestivalWishActivityInfo(msgGetInfo, slotPeer);
    }
        break;
    case 2:
    {
              GameMsg_C2S_GetFestivalBlessActivityInfo msgBless;
              _OnGetFestivalBlessActivityInfo(msgBless, slotPeer);
    }
        break;
    case 3:
    {
              GameMsg_C2S_GetXmasFreeReward msgGetReward;
              msgGetReward.m_nLevel = (unsigned char)nPara2;
              _OnGetXmasFreeReward(msgGetReward, slotPeer);
    }
        break;
    }
}

void CEntityActivityComponent::_SendPlayerReward(int nIndex)
{
    COnlineRewardInfo *pInfo = COnlineRewardInfoMgr::Instance().GetOnlineRewardInfo(nIndex);
    if (pInfo != NULL && m_pRoleAttr != NULL)
    {
        std::list<CItem> listReward;
        if (m_pRoleAttr->GetSex() == ESexType_Male)
        {
            listReward = pInfo->m_listMaleReward;
        }
        else if (m_pRoleAttr->GetSex() == ESexType_Female)
        {
            listReward = pInfo->m_listFemaleReward;
        }
        else
        {
            WriteLog(LOGLEVEL_ERROR, "GetRewardList error, sex type invalid\n");
        }

        std::string strTitle(CLocalization::Instance().GetString("Online_Reward_Mail_Title"));
        std::string strContent(CLocalization::Instance().GetString("Online_Reward_Mail_Content1"));

        int nHour = pInfo->m_nRequireTime / 3600;
        int nMinute = (pInfo->m_nRequireTime - nHour * 3600) / 60;
        if (nHour > 0)
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("Online_Reward_Mail_Content3"), nHour, nMinute);
        }
        else if (nMinute > 0)
        {
            SafeFormat(strContent, CLocalization::Instance().GetString("Online_Reward_Mail_Content2"), nMinute);
        }

        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), listReward, EItemAction_Add_KeepOnlineReward, 0, true, EMailType_Keep_Online_Reward, strTitle, strContent);
        m_pRoleAttr->ChangeMoney(pInfo->m_nMoney, EChangeMoneyCause_Add_OnlineReward, 0);
    }
}

void CEntityActivityComponent::_SetProprietaryAcitivty(CProprietaryActivityInfo& activity)
{
    if (m_pRoleAttr != NULL && m_pRoleNet != NULL)
    {
        activity.m_eActivityId = EActivity_ProprietaryAcitivty;
        activity.m_bExhibit = CProprietaryActivityInfoMgr::Instance().Exhibit();
        activity.m_nWeight = CProprietaryActivityInfoMgr::Instance().Weight();
        activity.m_strRegularImageName = CProprietaryActivityInfoMgr::Instance().RegularImageName();
        activity.m_strThumbnailsName = CProprietaryActivityInfoMgr::Instance().ThumbnailsName();
        activity.m_bHasReward = m_bProprietaryReward;
        activity.m_strActivityTitle = CProprietaryActivityInfoMgr::Instance().ActivityTitle();
        activity.m_strActivityContent = CProprietaryActivityInfoMgr::Instance().ActivityContent();
        activity.m_listReward = CProprietaryActivityInfoMgr::Instance().Rewards();


        if (m_pRoleNet->GetVerType() == Version_Free)
        {
            activity.m_bFreeOrCharge = false;
        }
        else
        {
            activity.m_bFreeOrCharge = true;
        }
        activity.m_nSex = m_pRoleAttr->GetSex();
    }
}

void CEntityActivityComponent::OnGetLongactingCumulativeRechargeInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetLongactingCumulativeRechargeResult resultmsg;
    resultmsg.m_nHaveRewardIndex = m_nLongactingCumulativeRechargeIndex;
    resultmsg.m_cSex = m_pRoleAttr->GetSex();
    LongactingCumulativeRechargeMgr::Instance().GetLongactingCumulativeRechargeConfig(resultmsg.m_listLongactingRechargeConfig);

    _SendPlayerMsg(&resultmsg);
}

void CEntityActivityComponent::OnGetLongactingCumulativeReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetLongactingCumulativeRechargeReward & c2smsg = (GameMsg_C2S_GetLongactingCumulativeRechargeReward&)msg;

    std::string strRoleName = m_pRoleAttr->GetRoleName();
    int nResult = LongactingCumulativeRechargeMgr::Instance().GetLongactingCumulativeRechargeReward(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex(),
        strRoleName, m_pRoleAttr->GetTotalPoint(), m_nLongactingCumulativeRechargeIndex, c2smsg.m_nRewardIndex);

    if (nResult != ELCRE_Scuess)
    {
        GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail failmsg;
        failmsg.m_nErrorFlag = nResult;
        _SendPlayerMsg(&failmsg);
    }
}

void CEntityActivityComponent::OnGetLongactingCumulativeBoardCast(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetLongactingRechargeBoardCast s2gmsg;
    s2gmsg.m_nRoleID = m_pRoleAttr->GetRoleID();

    ::SendMsg2GroupServer(&s2gmsg);
}

void CEntityActivityComponent::_OnGetLongactingCumulativeBoardCast(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetLongactingRechargeBoardCast & g2smsg = (GameMsg_G2S_GetLongactingRechargeBoardCast&)msg;

    GameMsg_S2C_GetLongactingCumulativeRechargeBCResult resultmsg;
    resultmsg.m_listLongactingBoardCast = g2smsg.m_listLongactingBoardCast;

    _SendPlayerMsg(&resultmsg);
}

void CEntityActivityComponent::UpdateLongactingRechargeRewardIndex(unsigned int nRewardIndex)
{
    m_nLongactingCumulativeRechargeIndex = nRewardIndex;
}

void CEntityActivityComponent::_SetMallSocialShareActivity(CMallSocialShareActivityData &activity)
{
    const CMallSocialShareActivityData *pData = dynamic_cast<const CMallSocialShareActivityData *>(CSocialShareMgr::Instance().GetActivityData(EActivity_MallSocialShare));

    if (NULL != pData)
    {
        activity.CopyForEncode(pData);
        activity.m_bIsReward = IsSharedToSocial(EUISocialShare_Mall);
    }
}

void CEntityActivityComponent::_SetPhotoSocialShareActivity(CPhotoSocialShareActivityData &activity)
{
    const CPhotoSocialShareActivityData *pData = dynamic_cast<const CPhotoSocialShareActivityData *>(CSocialShareMgr::Instance().GetActivityData(EActivity_PhotoSocialShare));

    if (NULL != pData)
    {
        activity.CopyForEncode(pData);
        activity.m_bIsReward = IsSharedToSocial(EUISocialShare_Photo);
    }
}

void CEntityActivityComponent::_OnGetInvitationInviterInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    if (CInvitationInviterConfigMgr::Instance().IsActivityOpen())
    {
        int nRoleID = m_pRoleAttr->m_nRoleID;
        if (strlen(m_pRoleAttr->GetInvitationCode()) == 0)
        {
            // 玩家还没有邀请码时，邀请的用户数一定为0，活动信息都是GameServer已经知道的，最后只需要将邀请码写入DB
            CInvitationInviterConfigMgr::MakeInvitationCode(nRoleID, m_pRoleAttr->m_szInvitationCode);

            GameMsg_S2C_GetInvitationInviterInfoSuc suc;
            suc.m_strInvitationCode = m_pRoleAttr->m_szInvitationCode;
            suc.m_nInvitationCount = 0;
            suc.m_strActivityName = CInvitationInviterConfigMgr::Instance().ActivityName();
            suc.m_strActivityContent = CInvitationInviterConfigMgr::Instance().ActivityName();
            suc.m_strShareAnimation = CInvitationInviterConfigMgr::Instance().RandomShareAnimation(m_pRoleAttr->m_nSex);

            const InvitationCumulativeReward& allLevelReward = CInvitationInviterConfigMgr::Instance().CumulativeReward(m_pRoleAttr->m_nSex);
            InvitationCumulativeReward::const_iterator it = allLevelReward.begin();
            for (; it != allLevelReward.end(); it++)
            {
                CInvitationInviterRewardInfo playerReward;
                playerReward.m_nLevel = it->second.m_nLevel;
                playerReward.m_nCumulation = it->second.m_nCumulation;
                playerReward.m_listItem = it->second.m_listItem;
                playerReward.m_nMoney = it->second.m_nMoney;
                playerReward.m_nReceivedFlag = INVITATION_REWARD_STATE_Not_Enough;
                suc.m_cumulativeReward[playerReward.m_nLevel] = playerReward;
            }

            m_pRoleNet->SendPlayerMsg(&suc);

            char* pInvitationCode = new char[MAX_INVITATION_CODE_LEN];
            strcpy(pInvitationCode, m_pRoleAttr->m_szInvitationCode);
            CDelHelper delHelper(pInvitationCode, MAX_INVITATION_CODE_LEN);
            AddQuery(QUERY_Invitation_UpdateInvitationCode, nRoleID, pInvitationCode, 0, NULL, &delHelper);
        }
        else
        {
            unsigned int nNow = (unsigned int)time(NULL);
            if (nNow - m_nInvitationQueryTime < INVITATION_DB_QUERY_MIN_INTERVAL)
            {
                GameMsg_S2C_GetInvitationInviterInfoFail fail;
                fail.m_strError = "Invitation_Query_Too_Often";
                m_pRoleNet->SendPlayerMsg(&fail);
            }
            else
            {
                m_nInvitationQueryTime = nNow;

                CRoleAllInvitationInfoDB* pRoleInvitationInfo = new CRoleAllInvitationInfoDB();
                pRoleInvitationInfo->m_nRoleID = m_pRoleAttr->m_nRoleID;
                pRoleInvitationInfo->m_strInvitationCode = m_pRoleAttr->GetInvitationCode();

                int nRequiredLevel = CInvitationInviterConfigMgr::Instance().AcceptorRequiredLevel();
                unsigned int* pEndTime = new unsigned int;
                *pEndTime = CInvitationInviterConfigMgr::Instance().ActivityEndTime();
                CDelHelper delHelper(pRoleInvitationInfo, pEndTime);
                AddQuery(QUERY_Invitation_GetInvitationInfo, nRequiredLevel, pRoleInvitationInfo,
                    CInvitationInviterConfigMgr::Instance().ActivityBeginTime(),
                    pEndTime, &delHelper);
            }
        }
    }
}

void CEntityActivityComponent::_OnDBGetInvitationInfo(QueryBase& rQuery)
{
    CRoleAllInvitationInfoDB* pRoleInvitationInfo = (CRoleAllInvitationInfoDB*)rQuery.m_pAttachment;
    if (pRoleInvitationInfo != NULL)
    {
        m_nInvitationBindCount = pRoleInvitationInfo->m_nInvitationBindCount;

        m_InvitationRewards.clear();
        m_InvitationRewards = pRoleInvitationInfo->m_InvitationRewards;

        GameMsg_S2C_GetInvitationInviterInfoSuc suc;
        suc.m_strInvitationCode = pRoleInvitationInfo->m_strInvitationCode;
        suc.m_nInvitationCount = pRoleInvitationInfo->m_nInvitationBindCount;
        suc.m_strActivityName = CInvitationInviterConfigMgr::Instance().ActivityName();
        suc.m_strActivityContent = CInvitationInviterConfigMgr::Instance().ActivityContent();
        suc.m_strShareAnimation = CInvitationInviterConfigMgr::Instance().RandomShareAnimation(m_pRoleAttr->m_nSex);

        const InvitationCumulativeReward& allLevelReward = CInvitationInviterConfigMgr::Instance().CumulativeReward(m_pRoleAttr->m_nSex);
        InvitationCumulativeReward::const_iterator it = allLevelReward.begin();
        for (; it != allLevelReward.end(); it++)
        {
            CInvitationInviterRewardInfo playerReward;
            playerReward.m_nLevel = it->second.m_nLevel;
            playerReward.m_nCumulation = it->second.m_nCumulation;
            playerReward.m_listItem = it->second.m_listItem;
            playerReward.m_nMoney = it->second.m_nMoney;
            if (m_InvitationRewards[(unsigned short)playerReward.m_nLevel].m_bReceived)
            {
                playerReward.m_nReceivedFlag = INVITATION_REWARD_STATE_Received;
            }
            else
            {
                if (m_nInvitationBindCount < it->second.m_nCumulation)
                    playerReward.m_nReceivedFlag = INVITATION_REWARD_STATE_Not_Enough;
                else
                    playerReward.m_nReceivedFlag = INVITATION_REWARD_STATE_Not_Received;
            }

            suc.m_cumulativeReward[playerReward.m_nLevel] = playerReward;
        }

        m_pRoleNet->SendPlayerMsg(&suc);
    }

    delete (CRoleAllInvitationInfoDB*)rQuery.m_pAttachment, rQuery.m_pAttachment = NULL;
    delete (unsigned int*)rQuery.m_pTag, rQuery.m_pTag = NULL;
}


void CEntityActivityComponent::_OnBindInvitationCode(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    if (CInvitationActivityMgr::Instance().IsActivityOpening())
    {
        GameMsg_C2S_BindInvitationCode* pMsg = (GameMsg_C2S_BindInvitationCode*)&msg;

        // 邀请码是否合法
        if (strlen(pMsg->m_szInvitationCode) != MAX_INVITATION_CODE_LEN - 1)
        {
            GameMsg_S2C_BindInvitationCodeFail fail;
            fail.m_strError = "Bind_Invitation_Code_Fail_Invalid_Code";
            m_pRoleNet->SendPlayerMsg(&fail);
            return;
        }

        // 验证是否已经绑接受邀请并领取了奖励
        if (m_pRoleAttr->m_bReceivedInvitationReward)
        {
            GameMsg_S2C_BindInvitationCodeFail fail;
            fail.m_strError = "Bind_Invitation_Code_Fail_Received_Reward";
            m_pRoleNet->SendPlayerMsg(&fail);
            return;
        }

        for (int i = 0; i < MAX_INVITATION_CODE_LEN - 1; i++)
            pMsg->m_szInvitationCode[i] = (char)toupper(pMsg->m_szInvitationCode[i]);

        // 验证是否是自己的邀请码
        if (strcmp(pMsg->m_szInvitationCode, m_pRoleAttr->m_szInvitationCode) == 0)
        {
            GameMsg_S2C_BindInvitationCodeFail fail;
            fail.m_strError = "Bind_Invitation_Code_Fail_Cannot_Bind_Self";
            m_pRoleNet->SendPlayerMsg(&fail);
            return;
        }

        // 验证等级是否已经超过活动等级
        if (m_pRoleAttr->m_nLevel > CInvitationInviterConfigMgr::Instance().AcceptorRequiredLevel())
        {
            GameMsg_S2C_BindInvitationCodeFail fail;
            fail.m_strError = "Bind_Invitation_Code_Fail_High_Level";
            m_pRoleNet->SendPlayerMsg(&fail);
            return;
        }

        // 查DB验证激活码合法性
        char* pBindInvitationCode = new char[MAX_INVITATION_CODE_LEN];
        strcpy(pBindInvitationCode, pMsg->m_szInvitationCode);
        CDelHelper delHelper(pBindInvitationCode, MAX_INVITATION_CODE_LEN);
        AddQuery(QUERY_Invitation_CheckInvitationCode, 0, pBindInvitationCode, 0, NULL, &delHelper);
    }
}

void CEntityActivityComponent::_OnDBCheckInvitationCode(QueryBase& rQuery)
{
    // 邀请码是否合法
    if (!rQuery.m_nAttachment)
    {
        GameMsg_S2C_BindInvitationCodeFail fail;
        fail.m_strError = "Bind_Invitation_Code_Fail_Invalid_Code";
        m_pRoleNet->SendPlayerMsg(&fail);

        delete (char*)(rQuery.m_pAttachment), rQuery.m_pAttachment = NULL;
    }
    else
    {
        // 修改内存已经领取奖励的状态
        m_pRoleAttr->m_bReceivedInvitationReward = true;

        // 记录邀请，领取奖励事件
        int nRoleID = m_pRoleAttr->m_nRoleID;
        unsigned int nNow = (unsigned int)time(NULL);
        char* pBindInvitationCode = (char*)rQuery.m_pAttachment;
        CDelHelper delHelper(pBindInvitationCode, MAX_INVITATION_CODE_LEN);
        AddQuery(QUERY_Invitation_AddInvitation, nRoleID, pBindInvitationCode, nNow, NULL, &delHelper);

        // 发送奖励
        const CInvitationAcceptorReward &reward = CInvitationActivityMgr::Instance().Reward(m_pRoleAttr->m_nSex);
        std::string& strMailTitle = CInvitationActivityMgr::Instance().MailTitle();
        std::string& strMailContent = CInvitationActivityMgr::Instance().MailContent();
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), reward.m_listItem, EItemAction_Add_AcceptInvitationReward, 0, true, EMailType_AcceptInvitationReward, strMailTitle, strMailContent);
        m_pRoleAttr->ChangeMoney(reward.m_nMoney, EChangeMoneyCause_Add_AcceptInvitationReward, 0);

        AddQuery(QUERY_Invitation_AddAcceptorReward, nRoleID);

        // 发送消息
        GameMsg_S2C_BindInvitationCodeSuc suc;
        m_pRoleNet->SendPlayerMsg(&suc);
    }
}

void CEntityActivityComponent::_OnMeetMatch(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    CMeetActivityMgr::Instance().OnMeetMatch(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex(), m_pRoleCouple->CoupleID());
}

void CEntityActivityComponent::_OnMeetCancelMatch(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    CMeetActivityMgr::Instance().OnMeetCancel(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex());
}

void CEntityActivityComponent::_OnMeetLeaveScene(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    CMeetActivityMgr::Instance().OnMeetLeaveScene(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex());
}

void CEntityActivityComponent::_OnMeetCameraEvent(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_MeetCameraEvent & c2sMsg = (GameMsg_C2S_MeetCameraEvent &)msg;
    unsigned int nErr = CMeetActivityMgr::Instance().OnMeetCameraEvent(m_pRoleAttr->GetRoleID(), c2sMsg.m_nIndex);
    if (nErr != EMeetErr_Success)
    {
        GameMsg_S2C_MeetPhotoFailed s2cFail;
        switch (nErr)
        {
        case EMeetErr_NotFindPlayer:
        {
                                       s2cFail.m_strError = "EMeetErr_NotFindPlayer";
        }
            break;
        case EMeetErr_PhotoNotEnoughItem:
        {
                                            s2cFail.m_strError = "EMeetErr_PhoteNotEnoughItem";
        }
            break;
        case EMeetErr_PhotoInIndexErr:
        {
                                         s2cFail.m_strError = "EMeetErr_PhoteInIndexErr";
        }
            break;
        case  EMeetErr_PhotoIndexHave:
        {
                                         s2cFail.m_strError = "EMeetErr_PhotoIndexHave";
        }
            break;
        case  EMeetErr_PlayerStatusErr:
        {
                                          s2cFail.m_strError = "EMeetErr_PlayerStatusErr";
        }
            break;
        }
        m_pRoleNet->SendPlayerMsg(&s2cFail);

        // log
        LogMeetAction(CMeetActivityLog::EMeetAction_Photo);
    }
    else
    {
        GameMsg_S2C_MeetPhotoSucceed s2cSucc;
        s2cSucc.m_nPhotoIndex = c2sMsg.m_nIndex;
        m_pRoleNet->SendPlayerMsg(&s2cSucc);
    }
}

void CEntityActivityComponent::_OnMeetEnd(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    CMeetActivityMgr::Instance().OnMeetEnd(m_pRoleAttr->GetRoleID(), m_pRoleAttr->GetSex());
}

void CEntityActivityComponent::_OnMeetEnterAmuseScene(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    CMeetActivityMgr::Instance().OnMeetEnterAmuseScene(m_pRoleAttr->GetRoleID());
}

void CEntityActivityComponent::LogMeetAction(int nAction, unsigned int nCoupleRoleID /* = 0 */, int nAmuseID /* = 0 */)
{
    CMeetActivityLog* pLog = new CMeetActivityLog();
    pLog->m_nRoleID = m_pRoleAttr->GetRoleID();
    pLog->m_eMeetAction = (CMeetActivityLog::EMeetAction)nAction;
    pLog->m_nCoupleRoleID = nCoupleRoleID;
    pLog->m_nAmuseID = nAmuseID;

    g_pQueryMgr->AddQuery(QUERY_LogRoleMeetActivity, 0, pLog);
}

void CEntityActivityComponent::_OnGetInviterCumulativeReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    if (CInvitationInviterConfigMgr::Instance().IsActivityOpen())
    {
        GameMsg_C2S_GetInvitationInviterReward* pMsg = (GameMsg_C2S_GetInvitationInviterReward*)&msg;
        unsigned short nType = pMsg->m_nType;
        unsigned short nLevel = pMsg->m_nLevel;

        // 验证是否已经发送过了
        CRoleInvitationRewardDBMap::iterator it = m_InvitationRewards.find(nLevel);
        if (it != m_InvitationRewards.end() && it->second.m_bReceived)
        {
            GameMsg_S2C_GetInvitationInviterRewardFail fail;
            fail.m_strError = "Invitation_Has_Recevied_Reward";
            m_pRoleNet->SendPlayerMsg(&fail);

            return;
        }

        // 查询奖励
        const CInvitationInviterReward* pReward = CInvitationInviterConfigMgr::Instance().GetCumulativeReward(nLevel, m_pRoleAttr->m_nSex);

        // 检查累计人数
        if (m_nInvitationBindCount < pReward->m_nCumulation)
        {
            GameMsg_S2C_GetInvitationInviterRewardFail fail;
            fail.m_strError = "Invitation_Not_Enough_Invitation_Count";

            m_pRoleNet->SendPlayerMsg(&fail);
            return;
        }

        // 发送奖励和消息
        std::string& strMailTitle = CInvitationInviterConfigMgr::Instance().MailTitle();
        std::string& strMailContent = CInvitationInviterConfigMgr::Instance().MailContent();
        CItemProcess::AddOrMailItems(*(CRoleEntity *)Entity(), pReward->m_listItem, EItemAction_Add_SentInvitationReward, 0, true, EMailType_SentInvitationReward, strMailTitle, strMailContent);
        m_pRoleAttr->ChangeMoney(pReward->m_nMoney, EChangeMoneyCause_Add_SentInvitationReward, 0);

        GameMsg_S2C_GetInvitationInviterRewardSuc suc;
        suc.m_nType = (unsigned char)nType;
        suc.m_nLevel = (unsigned char)nLevel;
        suc.m_nCumulation = pReward->m_nCumulation;
        m_pRoleNet->SendPlayerMsg(&suc);

        // 记录数据
        CRoleInvitationRewardDB curLevelReward;
        curLevelReward.m_nLevel = nLevel;
        curLevelReward.m_bReceived = true;
        m_InvitationRewards[nLevel] = curLevelReward;

        AddQuery(QUERY_Invitation_AddInviterReward, m_pRoleAttr->m_nRoleID, NULL, nLevel);
    }
}


void CEntityActivityComponent::_SetInvitationActivity(CInvitationActivityInfo &activity)
{
    if (m_pRoleAttr != NULL)
    {
        activity.m_nActivityID = EActivity_Invitation;
        activity.m_bIsOpen = CInvitationActivityMgr::Instance().IsActivityOpening();
        activity.m_bExhibit = CInvitationActivityMgr::Instance().Exhibit();
        activity.m_nWeight = CInvitationActivityMgr::Instance().Weight();
        activity.m_strRegularImageName = CInvitationActivityMgr::Instance().ActivityImageName();
        activity.m_strThumbnailImageName = CInvitationActivityMgr::Instance().ActivityThumbnailImageName();
        activity.m_nActivityBeginTime = (unsigned int)CInvitationActivityMgr::Instance().ActivityStartTime();
        activity.m_nActivityEndTime = (unsigned int)CInvitationActivityMgr::Instance().ActivityEndTime();
        activity.m_strActivityName = CInvitationActivityMgr::Instance().ActivityName();
        activity.m_strActivityContent = CInvitationActivityMgr::Instance().ActivityContent();
        activity.m_reward = CInvitationActivityMgr::Instance().Reward(m_pRoleAttr->m_nSex);

        if (m_pRoleAttr->m_bReceivedInvitationReward ||
            m_pRoleAttr->m_nLevel > CInvitationActivityMgr::Instance().VisibleMaxLevel())
        {
            // 已经领取奖励或者等级超过活动等级，就不再显示活动
            activity.m_nActivityID = EActivity_None;
        }
    }
}
void CEntityActivityComponent::_SetFresherActivity(CFresherActivityInfo &activity)
{
    activity.m_bIsOpen = CFresherActivityManager::Instance().IsOpen();
    if (m_nFresherDay > CFresherActivityManager::Instance().m_nDurationDays)
    {
        activity.m_bIsOpen = false;
    }

    if (activity.m_bIsOpen)
    {
        activity.m_nDay = (unsigned char)m_nFresherDay;
        activity.m_strActivityTitle = CFresherActivityManager::Instance().m_strActivityTitle;
        activity.m_strActivityContent = CFresherActivityManager::Instance().m_strActivityContent;
        activity.m_nRecvAgainMoney = CFresherActivityManager::Instance().m_nRecvAgainMoney;

        for (int i = 1; i <= CFresherActivityManager::Instance().m_nDurationDays; ++i)
        {
            const CFresherReward *pReward = CFresherActivityManager::Instance().GetFresherReward(i, m_pRoleAttr->m_nSex);
            CFresherReward reward;
            reward.m_nDay = i;
            if (pReward != NULL)
            {
                reward = *pReward;

                if (HaveGetFresherReward(i))
                {
                    reward.m_nRewardStatus = RewardStatus_Received;
                }
                else if (m_nFresherDay < i)
                {
                    reward.m_nRewardStatus = RewardStatus_CannotReceive;
                }
                else if (m_nFresherDay > i)
                {
                    reward.m_nRewardStatus = RewardStatus_ReceiveAgain;
                }
                else
                {
                    reward.m_nRewardStatus = RewardStatus_UnReceived;
                }
            }
            activity.m_vecReward.push_back(reward);
        }

        CFresherReward reward = CFresherActivityManager::Instance().GetCumulationReward(m_pRoleAttr->m_nSex);
        if (HaveGetFresherReward(0))
        {
            reward.m_nRewardStatus = RewardStatus_Received;
        }
        else
        {
            reward.m_nRewardStatus = RewardStatus_UnReceived;
        }
        activity.m_vecReward.push_back(reward);
    }
}

bool CEntityActivityComponent::_HasReceivedAllFresherReward()
{
    int nDurationDays = CFresherActivityManager::Instance().m_nDurationDays;
    for (int i = 1; i <= nDurationDays; ++i)
    {
        if (!HaveGetFresherReward(i))
        {
            return false;
        }
    }

    return true;
}

bool CEntityActivityComponent::HaveGetFresherReward(int nDay)
{
    std::set<int>::iterator it = m_FresherRewardDays.find(nDay);
    if (it != m_FresherRewardDays.end())
    {
        return true;
    }

    return false;
}

bool CEntityActivityComponent::ReissueVipReward(std::list<CReward> & viprewardlist)
{
    if (m_nCurrentFresherDay == 0)
    {
        return false;
    }

    if (!m_pRoleVip->IsVIP())
    {
        return false;
    }

    if (!HaveGetFresherReward(m_nCurrentFresherDay))
    {
        return false;
    }

    const CFresherReward *pReward = CFresherActivityManager::Instance().GetFresherReward(m_nCurrentFresherDay, m_pRoleAttr->m_nSex);
    if (pReward == NULL)
    {
        return false;
    }

    CReward vipreward;

    // vip item reward
    CItemProcess::AddItems(*(CRoleEntity *)Entity(), pReward->m_listVipItem, EItemAction_Add_FresherReward, 0, true);
    vipreward.m_listItems = pReward->m_listVipItem;

    // vip money
    m_pRoleAttr->ChangeMoney(pReward->m_nVipMoney, EChangeMoneyCause_Add_FresherReward, 0);
    vipreward.m_nMoney = pReward->m_nMoney;

    // vip bind coin
    m_pRoleAttr->ChangeBindBill(pReward->m_nVipBindCoin, EChangeBindBillCause_Add_FresherReward);
    vipreward.m_nBindMCoin = pReward->m_nVipBindCoin;

    viprewardlist.push_back(vipreward);

    if (_HasReceivedAllFresherReward())
    {
        const CFresherReward *pAllGetReward = &CFresherActivityManager::Instance().GetCumulationReward(m_pRoleAttr->m_nSex);
        // vip item reward
        CItemProcess::AddItems(*(CRoleEntity *)Entity(), pAllGetReward->m_listVipItem, EItemAction_Add_FresherReward, 0, true);
        vipreward.m_listItems.insert(vipreward.m_listItems.end(), pAllGetReward->m_listVipItem.begin(), pAllGetReward->m_listVipItem.end());

        // vip money
        m_pRoleAttr->ChangeMoney(pAllGetReward->m_nVipMoney, EChangeMoneyCause_Add_FresherReward, 0);
        vipreward.m_nMoney += pAllGetReward->m_nMoney;

        // vip bind coin
        m_pRoleAttr->ChangeBindBill(pAllGetReward->m_nVipBindCoin, EChangeBindBillCause_Add_FresherReward);
        vipreward.m_nBindMCoin += pAllGetReward->m_nVipBindCoin;
    }

    // reset
    m_nCurrentFresherDay = 0;

    return true;
}

void CEntityActivityComponent::_OnGetFresherReward(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetFresherReward *pMsg = (GameMsg_C2S_GetFresherReward *)&msg;
    if (m_nFresherDay <= 0)
    {
        WriteLog(LOGLEVEL_WARNING, "roleid=%u fresherDay=%d roleCreateTime=%u",
            m_pRoleAttr->GetRoleID(), m_nFresherDay, m_pRoleAttr->m_nRoleCreateTime);
        m_nFresherDay = 1;
    }

    std::string strError = "";
    if (pMsg->m_nDay <= 0 || pMsg->m_nDay > CFresherActivityManager::Instance().m_nDurationDays)
    {
        strError = "FresherActivity_InvalidParam";
    }
    else if (!CFresherActivityManager::Instance().IsOpen()
        || m_nFresherDay > CFresherActivityManager::Instance().m_nDurationDays)
    {
        strError = "FresherActivity_Closed";
    }
    else if (HaveGetFresherReward(pMsg->m_nDay))
    {
        strError = "FresherActivity_HasRecevied";
    }
    else if (m_nFresherDay < (int)pMsg->m_nDay)
    {
        WriteLog(LOGLEVEL_ERROR, "[ERROR] msg.day=%d fresher day=%d", (int)pMsg->m_nDay, m_nFresherDay);
        strError = "FresherActivity_CannotReceive_TooEarly";
    }
    else
    {
        if (m_nFresherDay > pMsg->m_nDay)
        {
            int nCostMoney = CFresherActivityManager::Instance().m_nRecvAgainMoney;
            if (m_pRoleAttr->m_nMoney < nCostMoney)
            {
                strError = "FresherActivity_CannotReceive_NotEnoughMoney";
            }
            else
            {
                m_pRoleAttr->ChangeMoney(-nCostMoney, EChangeMoneyCause_Del_FresherReward, 0);
            }
        }
    }

    if (strError.empty())
    {
        const CFresherReward *pReward = CFresherActivityManager::Instance().GetFresherReward(pMsg->m_nDay, m_pRoleAttr->m_nSex);
        if (pReward != NULL)
        {
            // item reward
            CItemProcess::AddItems(*(CRoleEntity *)Entity(), pReward->m_listItem, EItemAction_Add_FresherReward, 0, true);

            // money
            m_pRoleAttr->ChangeMoney(pReward->m_nMoney, EChangeMoneyCause_Add_FresherReward, 0);

            // bind coin
            m_pRoleAttr->ChangeBindBill(pReward->m_nBindCoin, EChangeBindBillCause_Add_FresherReward);

            if (m_pRoleVip->IsVIP())
            {
                // vip item reward
                CItemProcess::AddItems(*(CRoleEntity *)Entity(), pReward->m_listVipItem, EItemAction_Add_FresherReward, 0, true);

                // vip money
                m_pRoleAttr->ChangeMoney(pReward->m_nVipMoney, EChangeMoneyCause_Add_FresherReward, 0);

                // vip bind coin
                m_pRoleAttr->ChangeBindBill(pReward->m_nVipBindCoin, EChangeBindBillCause_Add_FresherReward);
            }

            AddQuery(QUERY_FresherActivity_AddReward, m_pRoleAttr->m_nRoleID, NULL, pMsg->m_nDay, NULL);
            m_FresherRewardDays.insert(pMsg->m_nDay);
            m_nCurrentFresherDay = pMsg->m_nDay;

            GameMsg_S2C_GetFresherRewardSuc suc;
            suc.m_nDay = pMsg->m_nDay;
            _SendPlayerMsg(&suc);

            if (_HasReceivedAllFresherReward())
            {
                const CFresherReward *pAllGetReward = &CFresherActivityManager::Instance().GetCumulationReward(m_pRoleAttr->m_nSex);

                // item reward
                CItemProcess::AddItems(*(CRoleEntity *)Entity(), pAllGetReward->m_listItem, EItemAction_Add_FresherReward, 0, true);

                // money
                m_pRoleAttr->ChangeMoney(pAllGetReward->m_nMoney, EChangeMoneyCause_Add_FresherReward, 0);

                // bind coin
                m_pRoleAttr->ChangeBindBill(pAllGetReward->m_nBindCoin, EChangeBindBillCause_Add_FresherReward);

                if (m_pRoleVip->IsVIP())
                {
                    // vip item reward
                    CItemProcess::AddItems(*(CRoleEntity *)Entity(), pAllGetReward->m_listVipItem, EItemAction_Add_FresherReward, 0, true);

                    // vip money
                    m_pRoleAttr->ChangeMoney(pAllGetReward->m_nVipMoney, EChangeMoneyCause_Add_FresherReward, 0);

                    // vip bind coin
                    m_pRoleAttr->ChangeBindBill(pAllGetReward->m_nVipBindCoin, EChangeBindBillCause_Add_FresherReward);
                }

                AddQuery(QUERY_FresherActivity_AddReward, m_pRoleAttr->m_nRoleID, NULL, 0, NULL);
                m_FresherRewardDays.insert(0);

                GameMsg_S2C_GetFresherAllReward all;
                _SendPlayerMsg(&all);
            }
        }
    }
    else
    {
        GameMsg_S2C_GetFresherRewardFail fail;
        fail.m_strError = strError;
        _SendPlayerMsg(&fail);
    }
}

void CEntityActivityComponent::_OnGetExtraActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer)
{
    GameMsg_S2C_GetExtraActivityInfoSuc m;
    m.m_nSceneID = (unsigned char)CExtraActivityInfoMgr::Instance().GetAmuseSceneID();
    _SendPlayerMsg(&m);
}

void CEntityActivityComponent::PacketBuff(std::list<BuffData>& listBuff)
{
    int nExpRate = CGlobalActivityData::Instance().GetExtraExpRate();
    if (nExpRate > 0)
    {
        BuffData buff;
        buff.m_nBuffType = EBuffType_Exp;
        buff.m_nBuffSrcType = EBuffSrcType_Activity;
        buff.m_nPercent = nExpRate;
        buff.m_nLeftTime = -1;
        listBuff.push_back(buff);
    }
}

void CEntityActivityComponent::OnNotify(IParam &param)
{
    int nItemType = 0;
    int nCount = 0;
    int nItemAction = 0;

    param.GetParam("item_type", nItemType);
    param.GetParam("item_count", nCount);
    param.GetParam("item_action", nItemAction);

    if (0 != nCount && nItemAction == EItemAction_Add_BuyInMall)
    {
        UpdateBuyItemGotMedalInfo((unsigned int)nItemType, (unsigned short)nCount);
    }
}

void CEntityActivityComponent::_SetMallShareActivity(CMallShareActivityData& activity)
{
    activity = CMallSocialShareMgr::Instance().GetData();
}

bool CEntityActivityComponent::IsSharedToSocial(EUISocialShare ui)
{
    std::map<EUISocialShare, time_t>::iterator it = m_mapSocialShareRecord.find(ui);
    if (it == m_mapSocialShareRecord.end())
    {
        return false;
    }

    if (EUISocialShare_Normal == ui)
        return IsSameDay(it->second, time(NULL));
    else
        return !CSocialShareMgr::Instance().CanShareAgain(GetActivityIDByEUISocialShare(ui), it->second);
}

time_t CEntityActivityComponent::GetFirstSharedTime(EUISocialShare ui)
{
    std::map<EUISocialShare, time_t>::iterator it = m_mapSocialShareRecord.find(ui);
    if (it == m_mapSocialShareRecord.end())
    {
        return 0;
    }
    return it->second;
}

void CEntityActivityComponent::SetSharedToSocial(EUISocialShare ui, bool b, bool bToDB)
{
    if (IsSharedToSocial(ui) != b && bToDB && b)
    {
        time_t now = time(NULL);

        m_mapSocialShareRecord[ui] = now;

        __int64 tag = ui;
        tag <<= 32;
        tag += (unsigned int)now;

        AddQuery(QUERY_SaveSocialShare, m_pRoleAttr->GetRoleID(), NULL, tag);
    }

    SendCanSocialShareFirst(ui, !b);
}

void CEntityActivityComponent::OnActivityChange(unsigned char nActivityID, bool bOpen)
{
    EUISocialShare eUI = GetEUISocialShareByActivityID((EActivity)nActivityID);

    SendCanSocialShareFirst(eUI, bOpen);
}

void CEntityActivityComponent::_OnSocialShare(GameMsg_Base &, CSlotPeer &)
{
    if (!CSystemSettingMgr::Instance().IsFunctionOpen(EFunctionSwitch_SocialShare))
    {
        return;
    }

    if (!IsSharedToSocial(EUISocialShare_Normal))
    {
        SetSharedToSocial(EUISocialShare_Normal, true);

        m_pRoleAttr->ChangeBindBill(CSystemSettingMgr::Instance().GetSocialShareRewardBindMCoin(),
            EChangeBindBillCause_Add_FirstSocialShare, true);

        GameMsg_S2C_SocialShare retMsg;
        SafeFormat(retMsg.m_strTip, CSystemSettingMgr::Instance().GetSocialShareRewardDesc(),
            CSystemSettingMgr::Instance().GetSocialShareRewardBindMCoin());
        _SendPlayerMsg(&retMsg);

        // 通知观察者
        CommonParam param;
        param.SetParam(std::string("activeness"), std::string("social_share"));
        NotifyAll(param);
    }
}

void CEntityActivityComponent::_OnActivitySocialShare(GameMsg_Base & refMsg, CSlotPeer &slotPeer)
{
    // check first
    _OnSocialShare(refMsg, slotPeer);

    GameMsg_C2S_ActivitySocialShare& msg = (GameMsg_C2S_ActivitySocialShare&)refMsg;

    switch (msg.m_nUI)
    {
    case EUISocialShare_Mall:
    case EUISocialShare_Photo:
        CSocialShareMgr::Instance().OnHandleActivityMsg(GetActivityIDByEUISocialShare((EUISocialShare)msg.m_nUI), (CRoleEntity *)Entity(), this, msg);
        break;

    default:
        LOG_WARNING << "CEntityActivityComponent::_OnActivitySocialShare() warning: msg.m_nUI: "
            << msg.m_nUI << "is valid" << endl;
        break;
    }
}

void CEntityActivityComponent::_OnRequestVipExtraReward(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_RequestVIPExtraReward &rRequestVIPReward = dynamic_cast<GameMsg_C2S_RequestVIPExtraReward &>(msg);

    switch (rRequestVIPReward.m_nOpenVipFromType)
    {
    case EOpenVIPFromType_PhotoShare:
    {
                                        CSocialShareMgr::Instance().OnHandleResendVIPExtraRewardMsg(EActivity_PhotoSocialShare,
                                            rRequestVIPReward.m_nOpenVipFromType, (CRoleEntity *)Entity(), this);
    }
        break;

    case EOpenVIPFromType_Fresher:
    {
                                     std::list<CReward> viprewardlist;
                                     bool bGetSucess = ReissueVipReward(viprewardlist);

                                     GameMsg_S2C_RequestVIPExtraRewardRes resmsg;
                                     resmsg.m_listRewards = viprewardlist;
                                     resmsg.m_nErrorType = (unsigned char)(bGetSucess ? ERequestVIPExtraRewardErrorType_Success : ERequestVIPExtraRewardErrorType_AlreadyGot);
                                     resmsg.m_nOpenVipFromType = rRequestVIPReward.m_nOpenVipFromType;

                                     _SendPlayerMsg(&resmsg);
    }
        break;

    case EOpenVIPFromType_FresherQuest:
    {
                                          //...
                                          if (m_pRoleFresherQuest != NULL)
                                          {
                                              m_pRoleFresherQuest->GetFresherVIPExtraReward();
                                          }
    }
        break;

    case EOpenVIPFromType_LevelUp:
    {
                                     if (m_pRoleAttr != NULL)
                                     {
                                         m_pRoleAttr->ResendLevelUpVIPExtraReward(rRequestVIPReward.m_nOpenVipFromType);
                                     }
    }
        break;

    case EOpenVipFromType_Quest:
    {
                                   if (m_pRoleQuest != NULL)
                                   {
                                       m_pRoleQuest->SendQuestVipReward();
                                   }
    }
        break;

    default:
    {
               WriteLog(LOGLEVEL_ERROR, "Request vip extra reward type(%u) is wrong.",
                   (unsigned int)rRequestVIPReward.m_nOpenVipFromType);
    }
        break;
    }
}

void CEntityActivityComponent::SendAllShareActivitiesCanFirst()
{
    for (std::map<EActivity, EUISocialShare>::iterator it = m_mapSocialShareActivity2UI.begin();
        it != m_mapSocialShareActivity2UI.end(); ++it)
    {
        bool bOpen = CSocialShareMgr::Instance().IsOpenNow(it->first);
        SendCanSocialShareFirst(it->second, bOpen && !IsSharedToSocial(it->second));
    }

    //每日分享
    SendCanSocialShareFirst(EUISocialShare_Normal, !IsSharedToSocial(EUISocialShare_Normal));
}

void CEntityActivityComponent::SendCanSocialShareFirst(EUISocialShare ui, bool bCanShare)
{
    GameMsg_S2C_CanSocialShareFirst msg;
    msg.m_nUI = (unsigned char)ui;
    msg.m_bCanDoFirstShare = bCanShare;

    if (ui == EUISocialShare_Mall)
    {
        CMallSocialShareActivityData mallActivity;
        _SetMallSocialShareActivity(mallActivity);
        if (m_pRoleAttr->IsMale())
        {
            msg.m_strActionName = mallActivity.m_strMaleActionName;
        }
        else
        {
            msg.m_strActionName = mallActivity.m_strFemaleActionName;
        }
    }

    _SendPlayerMsg(&msg);
}

EUISocialShare CEntityActivityComponent::GetEUISocialShareByActivityID(EActivity eActivity)
{
    std::map<EActivity, EUISocialShare>::iterator it = m_mapSocialShareActivity2UI.find(eActivity);
    if (it != m_mapSocialShareActivity2UI.end())
    {
        return it->second;
    }
    return EUISocialShare_Normal;
}

EActivity CEntityActivityComponent::GetActivityIDByEUISocialShare(EUISocialShare eUI)
{
    for (std::map<EActivity, EUISocialShare>::iterator it = m_mapSocialShareActivity2UI.begin();
        it != m_mapSocialShareActivity2UI.end(); ++it)
    {
        if (it->second == eUI)
        {
            return it->first;
        }
    }
    return EActivity_None;
}

void CEntityActivityComponent::OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetRedEnvelopeList g2smsg;
    g2smsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    g2smsg.m_nDanceGroupID = m_pRoleDanceGroup->GetDanceGroupID();
    SendMsg2GroupServer(&g2smsg);
}

void CEntityActivityComponent::_OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetRedEnvelopeList & g2smsg = (GameMsg_G2S_GetRedEnvelopeList&)msg;

    GameMsg_S2C_GetRedEnvelopeListResult resultmsg;
    resultmsg.m_listBriefRedEnvelope = g2smsg.m_listRedEnvelope;
    resultmsg.m_listGetRedEnvelopeID = g2smsg.m_listGetRedEnvelopeID;
    resultmsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    resultmsg.m_bAnonymity = RedEnvelopeActivityManager::Instance().IsAnonymity();
    resultmsg.m_bActivityShow = RedEnvelopeConfigManager::Instance().IsActivityShow((unsigned int)time(NULL));
    m_pRoleAttr->SendPlayerMsg(&resultmsg);
}

void CEntityActivityComponent::OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_GetRedEnvelopeDetails & c2smsg = (GameMsg_C2S_GetRedEnvelopeDetails&)msg;

    GameMsg_S2G_GetRedEnvelopeDetails detailsmsg;
    detailsmsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    detailsmsg.m_nRedEnvelopeID = c2smsg.m_nRedEnvelopeID;
    SendMsg2GroupServer(&detailsmsg);
}

void CEntityActivityComponent::OnSetAnonymity(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    bool bAnonymity = !RedEnvelopeActivityManager::Instance().IsAnonymity();
    RedEnvelopeActivityManager::Instance().SetAnonymity(bAnonymity);

    AddQuery(QUERY_RedEnvelope_UpdateAnonymityEvent, m_pRoleAttr->GetRoleID(), NULL, bAnonymity ? 1 : 0);

    GameMsg_S2C_UpdateAnonymity updatemsg;
    updatemsg.m_bAnonymity = bAnonymity;
    m_pRoleAttr->SendPlayerMsg(&updatemsg);
}

void CEntityActivityComponent::_OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetRedEnvelopeDetails & g2smsg = (GameMsg_G2S_GetRedEnvelopeDetails&)msg;

    if (g2smsg.m_RedEnvelope.m_nRedEnvelopeID == 0)
    {
        GameMsg_S2C_GetRedEnvelopeDetailsFail failmsg;
        failmsg.m_nError = 1;
        m_pRoleAttr->SendPlayerMsg(&failmsg);
    }
    else
    {
        GameMsg_S2C_GetRedEnvelopeDetailsResult resultmsg;
        resultmsg.m_RedEnvelope = g2smsg.m_RedEnvelope;
        m_pRoleAttr->SendPlayerMsg(&resultmsg);
    }
}

void CEntityActivityComponent::OnNewRedEnvelopeNotice(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_NewRedEnvelope & g2smsg = (GameMsg_G2S_NewRedEnvelope&)msg;

    GameMsg_S2C_NewRedEnvelopeNotice notice;
    notice.m_strSendRoleName = g2smsg.m_strRoleName;
    m_pRoleAttr->SendPlayerMsg(&notice);
}

void CEntityActivityComponent::OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_C2S_OpenRedEnvelope & c2smsg = (GameMsg_C2S_OpenRedEnvelope&)msg;

    if (m_pRoleDanceGroup->GetDanceGroupID() > 0)
    {
        //send to group
        GameMsg_S2G_OpenRedEnvelope getmsg;
        getmsg.m_nRoleID = m_pRoleAttr->GetRoleID();
        getmsg.m_nDanceGroupID = m_pRoleDanceGroup->GetDanceGroupID();
        getmsg.m_nRedEnvelopeID = c2smsg.m_nRedEnvelopeID;
        getmsg.m_strRoleName = m_pRoleAttr->GetRoleName();
        ::SendMsg2GroupServer(&getmsg);
    }
}

void CEntityActivityComponent::_OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_OpenRedEnvelope & g2smsg = (GameMsg_G2S_OpenRedEnvelope&)msg;

    if (g2smsg.m_nResultFlag == (unsigned int)ERedEnvelopeError_Scuess)
    {
        switch (g2smsg.m_nCurrencyType)
        {
        case EMallCurrencyType_MCoin:m_pRoleAttr->ChangeBill(g2smsg.m_nCurrencyValue, EChangeBillCause_Add_RedEnvelope); break;
        case EMallCurrencyType_Money:m_pRoleAttr->ChangeMoney(g2smsg.m_nCurrencyValue, EChangeMoneyCause_Add_RedEnvelope, 0); break;
        case EMallCurrencyType_BindMCoin:m_pRoleAttr->ChangeBindBill(g2smsg.m_nCurrencyValue, EChangeBindBillCause_Add_RedEnvelope); break;
        default:break;
        }
    }

    GameMsg_S2C_OpenRedEnvelopeResult resultmsg;
    resultmsg.m_nRedEnvelopeID = g2smsg.m_nRedEnvelopeID;
    resultmsg.m_nResult = g2smsg.m_nResultFlag;
    resultmsg.m_nCurrencyType = g2smsg.m_nCurrencyType;
    resultmsg.m_nCurrencyValue = g2smsg.m_nCurrencyValue;
    resultmsg.m_nOpenTime = g2smsg.m_nOpenTime;
    m_pRoleAttr->SendPlayerMsg(&resultmsg);
}

void CEntityActivityComponent::OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_S2G_GetCanOpenRedEnvelopeCount requiremsg;
    requiremsg.m_nRoleID = m_pRoleAttr->GetRoleID();
    requiremsg.m_nDanceGroupID = m_pRoleDanceGroup->GetDanceGroupID();
    ::SendMsg2GroupServer(&requiremsg);
}

void CEntityActivityComponent::_OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer)
{
    GameMsg_G2S_GetCanOpenRedEnvelopeCount & s2gmsg = (GameMsg_G2S_GetCanOpenRedEnvelopeCount&)msg;

    GameMsg_S2C_UpdateCanOpenRedEnvelopeCount updatemsg;
    updatemsg.m_nCanGetNum = s2gmsg.m_nCanOpenCount;
    m_pRoleAttr->SendPlayerMsg(&updatemsg);
}

void CEntityActivityComponent::ExchangeEffectStone()
{
    m_listExchangeItem.clear();
    const std::map<itemtype_t, CItem> & mapExchangeItem = EffectStoneExchangeManager::Instance().GetExchangeItem();
    for (std::map<itemtype_t, CItem>::const_iterator it = mapExchangeItem.begin(); it != mapExchangeItem.end(); ++it)
    {
        unsigned int nItemCount = m_pRoleItem->GetItemCount(it->first, false, false);
        if (nItemCount > 0)
        {
            m_pRoleItem->RemoveItemByType(it->first, nItemCount, EItemAction_Del_EffectStoneExchange);
            CItem item(it->second.m_nItemType, it->second.m_nItemCount*nItemCount, it->second.m_nValidTime, false);
            m_listExchangeItem.push_back(item);
        }
    }

    if (!m_listExchangeItem.empty())
    {
        m_pRoleItem->AddItem(m_listExchangeItem, EItemAction_Add_EffectStoneExchange, 0, true);
        m_bHaveExchangeItem = true;
    }
}

