#ifndef	__ENTITYACTIVITYCOMPONENT_H__
#define __ENTITYACTIVITYCOMPONENT_H__

// 累计在线时长奖励的数据库只存储最新的数据，不会删除或者清零。
// 数据是否有效只在DB存储出来后做由程序做处理。
#include <list>
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"
#include "ActivityDataStruct.h"


#define INVITATION_DB_QUERY_MIN_INTERVAL 5

using namespace std;

class CEntityNetComponent;
class CEntityTimerComponent;
class CEntityAttributeComponent;
class CEntityItemComponent;
class CEntityMailComponent;
class CEntityFriendComponent;
class CEntityChatComponent;
class CCummulativeRechareActivityInfo;
class CCummulativeSpendActivityInfo;
class CExchangeItemActivityInfo;
class CShowInTimeOnlineActivity;
class CPlainTextActivityInfo;
class CGiftCodeActivityInfo;
class COnlineRewardActivity;
class CumulativeSpendGotMedalActivityData;
class BuyItemGotMedalActivityData;
class CEntityCoupleComponent;
class CProprietaryActivityInfo;
class CEntityQuestComponent;
class CEntityVIPComponent;
class CEntityQuestNewComponent;
class CEntityMedalComponent;
class CEntityDanceGroupComponent;

class CEntityActivityComponent : public CEntityComponent, public CommonSubscriber
{
public:
    CEntityActivityComponent();
    ~CEntityActivityComponent();

public:
    virtual void OnEvent(CComponentEvent& refEvent);
    virtual void Start();
    virtual void OnLogin();
    virtual void OnLogout();
    virtual void OnUpdate(const unsigned long &nTimeElapsed);
    virtual void SerializeComponent(CParamPool &IOBuff);
    virtual const char* SerializeName(){
        return "Activity";
    };
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual bool PacketToCache( ROLEINFO_DB* pRoleInforDB ) const;
    virtual void SendExtraStaticData(){};
    virtual void PacketBuff(std::list<BuffData>& listBuff);
    virtual void OnNotify(IParam &param);
    void RegComponentNetMsgMap();
    void SendAllActivitiesInfo();

public:
    void OnOnlineRewardActivityStart();
    void OnOnlineRewardActivityEnd();

    void OnCumulativeRechargeActivityStart();
    void OnCumulativeRechargeActivityEnd();
    void UpdateOnlineRewardInfoToClient();
    void OnPlayerRecharge(int nCharge);
    void LogMeetAction(int nAction, unsigned int nCoupleRoleID = 0, int nAmuseID = 0);

    void OnTestActivity(int nPara1, int nPara2, int nPara3);

    bool IsSharedToSocial(EUISocialShare ui);
    void SetSharedToSocial(EUISocialShare ui, bool b, bool bToDB = true);
    time_t GetFirstSharedTime(EUISocialShare ui);
    bool CanGetPhotoShareVIPReward() const {
        return m_bNotGetPhotoShareVIPReward;
    }
    void SetNotGetPhotoShareVIPReward(bool bNoGetVIPReward) {
        m_bNotGetPhotoShareVIPReward = bNoGetVIPReward;
    }

    void OnActivityChange(unsigned char nActivityID, bool bOpen);

    EUISocialShare GetEUISocialShareByActivityID(EActivity eActivity);
    EActivity GetActivityIDByEUISocialShare(EUISocialShare eUI);

    void AddCumulativeSpendBill(int nValue);
    void AddCumulativeSpendBindBill(int nValue);

private:
    void _OnGetAllActivitiesInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void GetCDKeyInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void GetBindActivationInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);

    // 获取在线奖励
    void _OnGetOnlineReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //获取礼品
    void _OnGetGift(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //累计充值
    void _OnGetCumulativeRechargeReward(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnExchangeItem(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //累计消费
    void OnGetCumulativeSpendReward(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _LoadCumulativeSpendInfo();
    void _LoadCumulativeSpendInfoForMedal();
    int GetCumulativeSpendValue(int nCurrencyType);
    int GetCumulativeSpendValueForMedal(int nCurType);

    // 累计消费获取勋章
    void OnGetCumulativeSpendGotMedalReward(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void LoadCumulativeSpendGotMedalReward();
    bool CanGetCumulativeSpendGotMedalRewards(unsigned char nIndex);

    //IOS尊享版奖励领取操作
    void _OnGetProprietaryReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //活动扩展数据
    void _OnGetExtraActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //圣诞免费礼物
    void _SaveXmasFreeRewardInfo(unsigned int nLastGetRewardTime);
    void _LoadXmasFreeRewardInfo();
    void _SendXmasFreeRewardActivityInfo();
    void _OnGetXmasActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnGetXmasFreeReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //许愿活动
    void _LoadFestivalWishInfo();
    void _SaveFestivalWishInfo();
    void _SendFestivalWishActivityInfo();
    void _OnGetFestivalWishActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnRoleFestivalWish(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //祝福活动
    void _SaveBlessInfo(CFestivalBlessInfoDB *pBlessEntryDB);
    void _SaveRoleBlessActivityInfo(unsigned int nGetRewardTime);
    void _LoadRoleBlessActivityInfo();
    void _OnGetFestivalBlessActivityInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _BlessFriend(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _BlessFriend_G(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _GetFestivalBlessReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    bool _ReachCumulativeRechargeRequire(int nCurRechargeNum, int nLevel);
    bool _ReachCumulativeSpendRequire(int nCurRechargeNum, int nLevel);
    void _LoadCumulativeRechargeInfo();

    //拼图活动
    void _LoadRolePuzzleDB();
    void _UpdateRolePuzzleDB(int nOriTime, const CRolePuzzleActivityInfo & rPuzzleInfo);
    void _InitRolePuzzleInfo(const std::list<CRolePuzzleActivityDB> &rlistPuzzle);
    void _InitRolePuzzleInfo(CRoleAllPuzzleDB *pPuzzleDB);
    void _OnFillPuzzle(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void SyncPuzzleData();

    //玩家招募活动
    void _SetInvitationActivity(CInvitationActivityInfo& activity);
    void _OnGetInvitationInviterInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnBindInvitationCode(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnGetInviterCumulativeReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    // 新手七天乐活动
    void _SetFresherActivity(CFresherActivityInfo &activity);
    void _OnGetFresherReward(GameMsg_Base & msg, CSlotPeer &slotPeer);
    bool _HasReceivedAllFresherReward();
    bool HaveGetFresherReward(int nDay);
    bool ReissueVipReward(std::list<CReward> & viprewardlist);

    //商城社交分享
    void _SetMallShareActivity(CMallShareActivityData& activity);

    // 数据库加载邀请人数和已经获得的奖励信息
    void _OnDBCheckInvitationCode(QueryBase& rQuery);
    void _OnDBGetInvitationInfo(QueryBase& rQuery);

    // 邂逅活动
    void _OnMeetMatch(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnMeetCancelMatch(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnMeetLeaveScene(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnMeetCameraEvent(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnMeetEnd(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnMeetEnterAmuseScene(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //普通社交分享功能
    void _OnSocialShare(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //社交分享活动
    void _OnActivitySocialShare(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnRequestVipExtraReward(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void SendAllShareActivitiesCanFirst();
    void SendCanSocialShareFirst(EUISocialShare ui, bool bCanShare);

    void SyncGiftData();

    // 光效石兑换
    void ExchangeEffectStone();

    //数据库加载礼品
    void _OnLoadGift(QueryBase & rQuery);
    void _OnLoadCumulativeRechargeInfo(QueryBase & rQuery);
    void _OnLoadCumulativeSpendInfo(QueryBase & rQuery);
    void _OnLoadCumulativeSpendInfoForMedal(QueryBase& rQuery);
    void _OnDBLoadXmasFreeRewardInfo(QueryBase & rQuery);
    void _OnDBLoadRoleBlessActivityInfo(QueryBase & rQuery);
    void _OnDBLoadRoleFestivalWishActivityInfo(QueryBase & rQuery);
    void _OnDBLoadRolePuzzleActivityInfo(QueryBase & rQuery);
    void OnQueryCumulativeSpendGotMedalReward(QueryBase &rQuery);
    void OnQueryBuyItemGotMedalRewardedInfo(QueryBase &rQuery);

    void _SendPlayerMsg(GameMsg_Base *pMsg);
    void _SetNextKeepOnlineReward(int nIndex, CKeepOnlineReward & onlineInfo, bool bCanReset = true);
    void _ResetKeepOnlineRewardInfo();
    void _UpdateOnlineRewardInfoDB();
    void _HandleOnlineInfo(CKeepOnlineReward & onlineRewardInfo, unsigned int nNow);
    void _OnOnlineRewardActivityReset();
    void _SetCumulativeRechargeActivity(CCummulativeRechareActivityInfo & activity);

    void _SetCumulativeSpendActivity(CCummulativeSpendActivityInfo & activity);
    bool _HasReceivedCumulativeReward(int nLevel);
    bool _HasReceivedCumulativeSpendReward(int nLevel);
    bool IsSpendRewardEmpty();

    void SetCumulativeSpendGotMedalActivity(CumulativeSpendGotMedalActivityData &rActivity);
    bool HasReceivedCumulativeSpendGotMedalReward(int nIndex);

    void OnRequestBuyItemGotMedalReward(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void SetBuyItemGotMedalActivity(BuyItemGotMedalActivityData &rActivity) const;
    bool HasReceivedBuyItemGotMedalReward(unsigned char nIndex) const;
    bool CanGetBuyItemGotMedalRewards(unsigned char nIndex) const;
    void GetBuyItemGotMedalRewardedInfo(unsigned char nIndex, bool *rReceived, unsigned short *rCurrentCount) const;
    void LoadBuyItemGotMedalRewardedInfo();
    void UpdateBuyItemGotMedalInfo(unsigned int nItemType, unsigned short nItemCount);

    void _SetExchangeItemActivity(CExchangeItemActivityInfo & activity);
    bool _ReachExchangeItemRequire(int nLevel);

    void _SetInTimeOnlineActivity(CShowInTimeOnlineActivity & activity);

    void _SetPlainTextActivity(CPlainTextActivityInfo & activity);

    void _SetGiftCodeActivity(CGiftCodeActivityInfo & activity);

    void _SetOnlineRewardActivity(COnlineRewardActivity & activit);

    bool _OnlineRewardActivityActivated();
    void _SendPlayerReward(int nIndex);

    void _SetMallSocialShareActivity(CMallSocialShareActivityData &activity);
    void _SetPhotoSocialShareActivity(CPhotoSocialShareActivityData &activity);

    void _SetProprietaryAcitivty(CProprietaryActivityInfo& activity);

    // 获取长效累冲信息
    void OnGetLongactingCumulativeRechargeInfo(GameMsg_Base & msg, CSlotPeer &slotPeer);

    // 领取长效累冲奖励
    void OnGetLongactingCumulativeReward(GameMsg_Base & msg, CSlotPeer &slotPeer);

    //  获取长效累冲广播
    void OnGetLongactingCumulativeBoardCast(GameMsg_Base & msg, CSlotPeer &slotPeer);
    void _OnGetLongactingCumulativeBoardCast(GameMsg_Base & msg, CSlotPeer &slotPeer);

public://...........舞团红包................
    // 获取红包列表
    void OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 获取红包详细信息
    void OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 新红包广播
    void OnNewRedEnvelopeNotice(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 设置匿名
    void OnSetAnonymity(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 开红包
    void OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer);

    //
    void OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void _OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer);

public:
    // 更新奖励序列
    void UpdateLongactingRechargeRewardIndex(unsigned int nRewardIndex);

private:
    CEntityNetComponent             * m_pRoleNet;
    CEntityTimerComponent           * m_pRoleTimer;
    CEntityAttributeComponent       * m_pRoleAttr;
    CEntityItemComponent            * m_pRoleItem;
    CEntityMailComponent            * m_pRoleMail;
    CEntityFriendComponent          * m_pRoleFriend;
    CEntityChatComponent            * m_pRoleChat;
    CEntityCoupleComponent          * m_pRoleCouple;
    CEntityQuestComponent           * m_pRoleFresherQuest;
    CEntityVIPComponent             * m_pRoleVip;
    CEntityQuestNewComponent        * m_pRoleQuest;
    CEntityMedalComponent           * m_pRoleMedal;
    CEntityDanceGroupComponent      * m_pRoleDanceGroup;

private:
    int                                     m_nLastStop;
    unsigned long                           m_nTimeElapsed;
    unsigned long                           m_nUpdateDBTimeElapse;
    std::map< int, GiftDB >                 m_GiftMap;                   //当前玩家的礼品列表

    // 累计充值
    int                                     m_nCumulativeRechargeNum;   //累积充值,表示当前开启的累计充值活动的数值
    bool                                    m_bLoadCumulativeRechargeDB;//是否已经加载了玩家累计充值的数据
    std::vector<int>                        m_vecCumulativeRewards;

    // 累计消费
    std::vector<unsigned int>               m_CumulativeSpendReward;

    unsigned int                            m_nCumulativeBill;
    unsigned int                            m_nCumulativeBindBill;
    bool                                    m_bLoadCumulativeSpendSuc;
    /// <summary>
    /// 记录勋章累计消费活动的值
    /// <summary
    unsigned int                            m_nCumulativeBillForMedal;
    unsigned int                            m_nCumulativeBindBillForMedal;
    std::vector<unsigned int>               m_CumulativeSpendRewardForMedal;
    std::vector<bool>                       m_vectCumulativeSpendGotMedalReward;

    // 购买物品获取勋章
    BuyItemGotMedalRewardedInfoMap          m_mapBuyItemGotMedalRewardedInfo;

    // 圣诞节免费领取
    CRoleXmasFreeRewardInfo                 m_roleXmasFreeRewardInfo;

    // 许愿活动
    CRoleFestivalWishActivityInfo           m_roleFestivalWishActivityInfo;

    // 祝福活动
    CRoleBlessActivityInfo                  m_roleBlessActivityInfo;

    // 拼图活动
    bool                                    m_bCheckPuzzleOpen;
    std::map<int, CRolePuzzleActivityInfo>  m_rolePuzzleActivityInfo;

    // 新玩家招募活动
    unsigned int                            m_nInvitationBindCount;
    CRoleInvitationRewardDBMap              m_InvitationRewards;
    unsigned int                            m_nInvitationQueryTime;

    // 新手七天乐活动
    std::set<int>                           m_FresherRewardDays;
    int                                     m_nFresherDay;
    int                                     m_nCurrentFresherDay;           //最后领取奖励的day

    bool                                    m_bProprietaryReward;       //是否已经领取过尊享版的奖励

    // 社交分享
    std::map<EUISocialShare, time_t>        m_mapSocialShareRecord;
    std::map<EActivity, EUISocialShare>     m_mapSocialShareActivity2UI;    //社交分享的活动id与界面id的对应关系
    bool                                    m_bNotGetPhotoShareVIPReward;   // 拍照分享没有获得VIP奖励

    // 在线奖励
    CKeepOnlineReward                       m_keepOnlineRewardInfo;

    // 长效累冲活动
    unsigned int                            m_nLongactingCumulativeRechargeIndex;         //奖励序列

    // 光效石兑换活动
    bool                                    m_bHaveExchangeItem;
    std::list<CItem>                        m_listExchangeItem;

};

typedef CRoleComponentImpl<CEntityActivityComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleActivity;

#endif

