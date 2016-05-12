/*
 * EntityDanceGroupComponent.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-1-6 13:57:33
 */

#ifndef __ENTITYDANCEGROUPCOMPONENT_H__
#define __ENTITYDANCEGROUPCOMPONENT_H__

#include "../logic/EntityComponent.h"
#include "../logic/RoleComponentImpl.h"
#include "EntityBigMama.h"

class CEntityNetComponent;
class CEntityAttributeComponent;
class CEntityItemComponent;
class CRoomComponent;
class CEntityCoupleComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class CEntityVIPComponent;
class CEntityQuestComponent;
class CEntityQuestNewComponent;
class CEntityTimerComponent;
class CeremonyRoom;
class DungeonPlayerInfo;
class BigMamaPlayerInfo;

/**
* EntityDanceGroupComponent:
* 
*/

class CEntityDanceGroupComponent : public CEntityComponent, public CommonSubscriber
{
public:
    CEntityDanceGroupComponent();
    virtual ~CEntityDanceGroupComponent();

public:
    virtual void Start();
    virtual void OnEvent(CComponentEvent &refEvent);
    virtual void OnLogin();
    virtual void OnLogout();
    virtual bool CreateFromDB(ROLEINFO_DB *pRoleInforDB);
    virtual bool PacketToDB(ROLEINFO_DB *pRoleInforDB) const;
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;
    virtual void OnUpdate(const unsigned long &nTimeElapsed) {}
    virtual void SendExtraStaticData() {}
    virtual void PacketBuff(std::list<BuffData> &listBuff) {}
    virtual void SerializeComponent(CParamPool &IOBuff) {}
    virtual const char* SerializeName() { return "DanceGroup"; }
    virtual void OnNotify( IParam &param );

public:
    void RegComponentNetMsgMap();

    void RegBigMamaMsg();
    void RegFairyLandMsg();

    // 通过Debug模拟消息测试
    void DanceGroupTest(int nPara1, int nPara2, int nPara3);
    void RookieAnyBody(unsigned int nRookieRole, int seq);

public:
    // 获取舞团信息
    CDanceGroupInfo* GetDanceGroupInfo();
    // 获取舞团成员列表
    const std::map<unsigned int, CDanceGroupMember *>& GetDanceGroupMemberMap();
    // 获取舞团ID
    unsigned int GetDanceGroupID() { return m_nGroupID; }
    // 获取舞团总荣誉值
    unsigned int GetDanceGroupTotalHonor();
    // 获取舞团总贡献值
    unsigned int GetDanceGroupTotalContribution();

    // 修改舞团荣誉值
    void ChangeDanceGroupHonor(int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara);
    // 修改舞团贡献值
    void ChangeDanceGroupContribution(unsigned int nChangeValue);

    void ChangeDanceGroupHonor(unsigned int nGroupID ,int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara);

    void AddDanceGroupByName(const std::string strDanceGroupName);

    CDanceGroupMember* GetSelfMemberInfo();
    CeremonyRoom* GetCeremonyRoom();

    void GmGetCeremonyRoomNum(int queryType);

    bool IsInChallengeRoom() const;
    bool IsInBigMamaRoom() const;

public:
    // 升级时调用
    void OnLevelUp();
    // 角色名改变时调用
    void OnRoleNameChanged();
    // 角色VIP信息变化时调用
    void OnVIPChange();

private:
    // 请求自己的舞团信息
    void OnRequestMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团信息
    void OnGetDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团信息列表
    void OnGetDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 创建舞团
    void OnCreateDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 解散舞团
    void OnDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消解散舞团
    void OnCancelDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 申请加入舞团
    void OnRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消加入舞团申请
    void OnCancelRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 批复舞团申请
    void OnReplyRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 批量批复舞团申请
    void OnReplyRequestListEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团名
    void OnChangeDanceGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团颜色
    void OnChangeDanceGroupColor(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团徽章
    void OnChangeDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团简介
    void OnChangeDanceGroupProfile(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团公告
    void OnChangeDanceGroupAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团职位
    void OnChangeDanceGroupTitle(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团职位名称
    void OnChangeDanceGroupTitleName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团传位
    void OnChangeDanceGroupLeader(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团徽章信息
    void OnGetDanceGroupBadgeInfos(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 解锁舞团徽章或效果
    void OnUnlockDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团徽章或效果
    void OnChangeDanceGroupBadgeOrEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 捐赠舞团资源
    void OnDonateDanceGroupResource(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团升级
    void OnUpgradeDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团记录
    void OnGetDanceGroupRecords(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 退出舞团
    void OnExitDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 开除舞团成员
    void OnKickOutDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团活动星星信息
    void OnDanceGroupActivityGetStarInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团活动摘星星
    void OnDanceGroupActivityAddStarPower(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团动力值
    void OnDanceGroupActivityGetStarPowerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团活动摘星星
    void OnDanceGroupActivityAddStarPowerPoint2Money(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 打开礼包
    void OnDanceGroupActivityOpenStarPacket(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团商店信息
    void OnDanceGroupShopGetInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团商店兑换
    void OnDanceGroupShopExchange(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 邀请舞团成员
    void OnInviteDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 接受舞团成员邀请
    void OnAcceptDanceGroupMemberInvitation(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // 发送舞团信息
    void OnSendSimpleDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团信息结果
    void OnGetDanceGroupInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 发送舞团信息列表
    void OnSendDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 解散舞团结果
    void OnDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消解散舞团结果
    void OnCancelDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 申请加入舞团结果
    void OnRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消加入舞团申请结果
    void OnCancelRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 批复舞团申请结果
    void OnReplyRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 批量批复舞团申请结果
    void OnReplyRequestListEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团名结果
    void OnChangeDanceGroupNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团颜色结果
    void OnChangeDanceGroupColorResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团徽章结果
    void OnChangeDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团简介结果
    void OnChangeDanceGroupProfileResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团公告结果
    void OnChangeDanceGroupAnnouncementResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团职位结果
    void OnChangeDanceGroupTitleResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团职位名称结果
    void OnChangeDanceGroupTitleNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 传位结果
    void OnChangeDanceGroupLeaderResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 解锁舞团徽章和效果结果
    void OnUnlockDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 修改舞团徽章或效果结果
    void OnChangeDanceGroupBadgeOrEffectResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团升级结果
    void OnUpgradeDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获得舞团记录结果
    void OnGetDanceGroupRecrodsResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 退出舞团结果
    void OnExitDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 被开除舞团信息
    void OnKickedOutDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 开除舞团成员结果
    void OnKickOutDanceGroupMemberResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团活动星星信息结果
    void OnDanceGroupActivityGetStarInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团活动摘星星结果
    void OnDanceGroupActivityAddStarPowerResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取动力值结果
    void OnDanceGroupActivityGetStarPowerInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 打开礼包结果
    void OnDanceGroupActivityOpenStarPacketResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团商店兑换结果
    void OnDanceGroupShopExchangeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团已解散
    void OnDanceGroupRemoved(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 刷新申请信息
    void OnDanceGroupBeRefuseRefresh(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // 获取舞团信息
    void OnGetMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 创建舞团结果
    void OnCreateDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团申请通知
    void OnAddRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团取消申请通知
    void OnRemoveRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团成员增加
    void OnAddDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 成功加入舞团
    void OnEnterDanceGroupSuccess(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新舞团基本信息
    void OnUpdateDanceGroupBaseInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新舞团成员信息
    void OnUpdateDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 捐赠舞团资源结果
    void OnDonateDanceGroupResourceResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 移除舞团成员信息
    void OnRemoveDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新星星状态
    void OnUpdateDanceGroupActivityStarState(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团商店信息结果
    void OnDanceGroupShopGetInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 接受舞团成员邀请结果
    void OnAcceptDanceGroupMemberInvitationResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新舞团试炼活力值
    void OnUpdateChallengeActive(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 检测玩家是否可以被邀请
    void OnCanBeInviteCheck(GameMsg_Base &msg, CSlotPeer &slotPeer);

	// 入团仪式：
	// 申请开启入团仪式
	void OnRequestApplyCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求进入入团仪式
	void OnRequestEnterCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求退出入团仪式
	void OnRequestQuitCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求刷新软团仪式信息
	void OnRequestRefreshCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求开启入团仪式
	void OnRequestStartCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 设置新人
	void OnSetCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 取消设置新人
	void OnCancelCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 踢出入团仪式的房间
	void OnKickCeremonyPlayer(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 保存自我介绍
	void OnSaveCeremonyIntruduction(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求仪式日志
	void OnRequestCeremonyRecord(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 点赞
	void OnRequestCeremonyPraise(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// 请求点赞日志
	void OnRequestPraiseRookie(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnCeremonyRoomStatics(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    void OnRequestEnterChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetCurrentStoreyRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestChallengeStoreyList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnRequestChallengeStoreyInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetChallengeStoreyInfoRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestChallengeRankList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetChallengeRankListRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestGetChallengeReward(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestChallengeInspire(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnChallengeInspireRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestAddChallengeChance(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestOpenChallengeStorey(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnOpenChallengeStoreyRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestPrepareChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnPrepareChallengeRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnRequestStartChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnReportChallengeScoreRes(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnChallengeStoreyFinished(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnChallengeRewardNotice(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnQueryChallengeRanking(QueryBase &rQuery);

// big mama.
public:
    void OnG2SNoticeBigMamaState(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 活动状态变更/（开启/关闭）

private:
    void OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 上线通知bigMama信息
    void OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 新加入舞团信息同步
    void OnG2SBroadBigMamaInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnG2SBigMamaInfoNotify(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    void OnG2SBigMamaFightResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnG2SBigMamaGetRewardResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //////////////////////////////////////////////////////////////////////////
    // from client.
    void OnRequestBigMamaStageInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 请求关卡信息
    void OnRequestFightBigMama(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 请求挑战

    void OnRequestGetBigMamaReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 请求领取奖励
    void OnCompleteBigMamaGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 完成大妈引导
    void OnRequestBigMamaGuideReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // 请求领取大妈引导奖励

//FairyLand
public:
    //发送秘境的奖励
    void OnG2SSendJoinRandomReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); 
//     公告秘境结束
//  void OnAnnouncementFairyLandEnd(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //获取舞团秘境信息
    void OnGetFairyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //引导
    void OnSetFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnCheckFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //获取排行榜规则
    void OnGetFairyLandRankRule(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //获取配置数据
    void OnGetFairyLandConfig(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //获取关卡数据
    void OnGetFairyLandChapterInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //获取秘境排行榜
    void OnGetFairyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //获取昨日排行榜奖励
    void OnGetYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //发送昨日排行榜奖励
    void OnSendYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    // 获取祝福信息
    void OnFairyLandOpenBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //祝福
    void OnFairyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

public:
    // 向Group Server发送舞团信息
    void Send2GroupMsg(GameMsg_Base &msg);
    // 向客户端发送舞团信息
    void SendPlayerMsg(GameMsg_Base &msg);
    // 向房间发送信息
    void SendMsgToRooms(GameMsg_Base &msg);

private:
    void GetMyDanceGroupInfo(unsigned short nClickedPlace);
    // 登录时推送舞团信息
    void PushDanceGroupInfoOnLogin(bool bValid, unsigned short nClickedPlace);
    // 更新舞团信息
    void UpdateDanceGroupInfo();
    // 更新舞团成员信息
    void UpdateMemberInfo(bool bOnline);
    // 更新舞团名
    void UpdateDanceGroupName(const std::string &rStrNewName);
    // 更新舞团职位信息
    void UpdateDanceGroupTitle(unsigned char nTitle);
    // 更新舞团徽章或效果信息
    void UpdateDanceGroupBadgeOrEffect(unsigned short nBadge, unsigned short nEffect);

    // 处理每日首次点击奖励
    bool ProcessFirstReward();

    // 检查舞团ID是否一致
    bool IsDanceGroupIDSame(unsigned int nGroupID);

    // 设置舞团信息
    void SetDanceGroupInfo(const CDanceGroupInfo *pDGInfo);
    // 清空舞团信息
    void ClearDanceGroupInfo();

    // 检查舞团徽章操作条件
    EDanceGroupBadgeResult CheckDanceGroupBadgeCond();
    // 检查舞团活动条件
    EDanceGroupActivityStarResult CheckDanceGroupActivityStarCond();
    // 检查舞团活动增加动力值条件
    EDanceGroupActivityStarResult CheckDanceGroupActivityAddStarPowerCond(unsigned short nType);
    // 检查舞团商店条件
    EDanceGroupShopResult CheckDanceGroupShopCond();
    // 检查邀请条件
    EInviteDanceGroupMemberResult CheckInviteDanceGroupMemberCond(unsigned int nRoleID, CRoleEntity *pInviteeRoleEntity);

    void RegChallengeNetMsgMap();

    void SyncChallengeConfig();

    void ResetChallengeChance();
    void UpdateChallengeChance();
    void CalcChallengeTotalChance();

    void ResetChallengeBuyChance();
    void UpdateChallengeBuyChance();

    void LoadChallengeRanking();
    void UpdateChallengeRankingState( std::map<int, DanceGroupChallengeRanking> &rRanking, unsigned int nStartTime, unsigned char nState );

    void EncodePlayerInfo(DungeonPlayerInfo &rInfo);
    bool EncodeBoosInfo(DungeonPlayerInfo &rInfo, int nStorey) const;

    bool EndRoom();

public:
    void EncodePlayerInfo(BigMamaPlayerInfo &rInfo);
    bool EncodeBossInfo(BigMamaPlayerInfo &rInfo, int bossID) const;


private:
    CEntityNetComponent         *m_pNetComponent;
    CEntityAttributeComponent   *m_pRoleAttr;
    CEntityItemComponent        *m_pRoleItem;
    CRoomComponent              *m_pRoleRoom;
    CEntityCoupleComponent      *m_pRoleCouple;
    CEntityPhotoRoomComponent   *m_pRolePhotoRoom;
    CAmuseRoomComponent         *m_pRoleAmuseRoom;
    CEntityQuestComponent       *m_pRoleQuest;
    CEntityQuestNewComponent    *m_pRoleQuestNew;
    CEntityTimerComponent       *m_pRoleTimer;
    CEntityVIPComponent         *m_pRoleVIP;

private:
    unsigned int                m_nGroupID;             // 当前舞团ID
    CDanceGroupInfo             *m_pGroupInfo;          // 当前舞团信息

    bool                        m_bTodayFirstLogin;     // 当天第一次登陆

    // 舞团活动
    bool                        m_bNormalReward;        // 每天第一次操作“点 一下”获得的礼物(在有舞团的情况下，如果获得过了就算重新建舞团也不会再获得礼物)
    unsigned int                m_nLastGetRewardTime;

    bool                        m_bChallengeOpening;
    unsigned short              m_nChallengeChance;             // 剩余试炼次数
    unsigned short              m_nChallengeTotalChance;        // 试炼总次数
    unsigned int                m_nChallengeChanceResetTime;    // 试炼次数重置时间

    unsigned short              m_nChallengeBuyChance;          // 购买次数
    unsigned int                m_nChallengeBuyChanceResetTime; // 购买重置时间

    int                         m_nChallengeCurStorey;          // 当前试炼塔层数
    unsigned char               m_nChallengeCurStoreyState;     // 当前试炼塔状态
    unsigned short              m_nChallengeMyInspire;          // 当前试炼塔自己鼓舞次数

    int                         m_nChallengeScene;              // 挑战场景
    int                         m_nChallengeMusic;              // 挑战歌曲
    int                         m_nChallengeMusicMode;          // 挑战模式
    int                         m_nChallengeMusicLevel;         // 挑战难度
    int                         m_nChallengeSaveStorey;         // 保存挑战的层数
    int                         m_nChallengeSaveStartTime;      // 保存开始时间

    unsigned int                m_nChallengeStartTime;          // 舞团试炼开始时间

    unsigned int                m_nChallengeRoomID;             // 舞团试炼房间ID

    std::map<int, DanceGroupChallengeRanking> m_mapRanking;     // 本周未领取奖励排名
	
    // 防止客户端连续点击多次，不需要计入数据库
    unsigned int                m_nCeremonyRoomLastPraise;      // 入团仪式上一次的冷却时间

    EntityBigMama               m_entityBigMama;                // 挑战大妈
};

typedef CRoleComponentImpl<CEntityDanceGroupComponent, CGetRoleProcessorObj, CGetRoleProcessorObj>  CRoleDanceGroup;

#endif // __ENTITYDANCEGROUPCOMPONENT_H__

