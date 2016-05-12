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

    // ͨ��Debugģ����Ϣ����
    void DanceGroupTest(int nPara1, int nPara2, int nPara3);
    void RookieAnyBody(unsigned int nRookieRole, int seq);

public:
    // ��ȡ������Ϣ
    CDanceGroupInfo* GetDanceGroupInfo();
    // ��ȡ���ų�Ա�б�
    const std::map<unsigned int, CDanceGroupMember *>& GetDanceGroupMemberMap();
    // ��ȡ����ID
    unsigned int GetDanceGroupID() { return m_nGroupID; }
    // ��ȡ����������ֵ
    unsigned int GetDanceGroupTotalHonor();
    // ��ȡ�����ܹ���ֵ
    unsigned int GetDanceGroupTotalContribution();

    // �޸���������ֵ
    void ChangeDanceGroupHonor(int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara);
    // �޸����Ź���ֵ
    void ChangeDanceGroupContribution(unsigned int nChangeValue);

    void ChangeDanceGroupHonor(unsigned int nGroupID ,int nChangeValue, EChangeDanceGroupHonorCause eCause, int nPara);

    void AddDanceGroupByName(const std::string strDanceGroupName);

    CDanceGroupMember* GetSelfMemberInfo();
    CeremonyRoom* GetCeremonyRoom();

    void GmGetCeremonyRoomNum(int queryType);

    bool IsInChallengeRoom() const;
    bool IsInBigMamaRoom() const;

public:
    // ����ʱ����
    void OnLevelUp();
    // ��ɫ���ı�ʱ����
    void OnRoleNameChanged();
    // ��ɫVIP��Ϣ�仯ʱ����
    void OnVIPChange();

private:
    // �����Լ���������Ϣ
    void OnRequestMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ������Ϣ
    void OnGetDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ������Ϣ�б�
    void OnGetDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������
    void OnCreateDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ɢ����
    void OnDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ȡ����ɢ����
    void OnCancelDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����������
    void OnRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ȡ��������������
    void OnCancelRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ������������
    void OnReplyRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������������
    void OnReplyRequestListEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�������
    void OnChangeDanceGroupName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�������ɫ
    void OnChangeDanceGroupColor(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ż���
    void OnChangeDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����ż��
    void OnChangeDanceGroupProfile(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ź���
    void OnChangeDanceGroupAnnouncement(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�����ְλ
    void OnChangeDanceGroupTitle(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�����ְλ����
    void OnChangeDanceGroupTitleName(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ���Ŵ�λ
    void OnChangeDanceGroupLeader(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���Ż�����Ϣ
    void OnGetDanceGroupBadgeInfos(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������Ż��»�Ч��
    void OnUnlockDanceGroupBadge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ż��»�Ч��
    void OnChangeDanceGroupBadgeOrEffect(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������Դ
    void OnDonateDanceGroupResource(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������
    void OnUpgradeDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���ż�¼
    void OnGetDanceGroupRecords(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �˳�����
    void OnExitDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա
    void OnKickOutDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���Ż������Ϣ
    void OnDanceGroupActivityGetStarInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ���Żժ����
    void OnDanceGroupActivityAddStarPower(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���Ŷ���ֵ
    void OnDanceGroupActivityGetStarPowerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ���Żժ����
    void OnDanceGroupActivityAddStarPowerPoint2Money(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����
    void OnDanceGroupActivityOpenStarPacket(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ�����̵���Ϣ
    void OnDanceGroupShopGetInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����̵�һ�
    void OnDanceGroupShopExchange(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա
    void OnInviteDanceGroupMember(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա����
    void OnAcceptDanceGroupMemberInvitation(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // ����������Ϣ
    void OnSendSimpleDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ������Ϣ���
    void OnGetDanceGroupInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������Ϣ�б�
    void OnSendDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ɢ���Ž��
    void OnDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ȡ����ɢ���Ž��
    void OnCancelDismissDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������Ž��
    void OnRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ȡ����������������
    void OnCancelRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������������
    void OnReplyRequestEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ������������������
    void OnReplyRequestListEnterDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����������
    void OnChangeDanceGroupNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�������ɫ���
    void OnChangeDanceGroupColorResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ż��½��
    void OnChangeDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����ż����
    void OnChangeDanceGroupProfileResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ź�����
    void OnChangeDanceGroupAnnouncementResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�����ְλ���
    void OnChangeDanceGroupTitleResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸�����ְλ���ƽ��
    void OnChangeDanceGroupTitleNameResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��λ���
    void OnChangeDanceGroupLeaderResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������Ż��º�Ч�����
    void OnUnlockDanceGroupBadgeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �޸����Ż��»�Ч�����
    void OnChangeDanceGroupBadgeOrEffectResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����������
    void OnUpgradeDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ������ż�¼���
    void OnGetDanceGroupRecrodsResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �˳����Ž��
    void OnExitDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ������������Ϣ
    void OnKickedOutDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա���
    void OnKickOutDanceGroupMemberResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ���Ż������Ϣ���
    void OnDanceGroupActivityGetStarInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ���Żժ���ǽ��
    void OnDanceGroupActivityAddStarPowerResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ����ֵ���
    void OnDanceGroupActivityGetStarPowerInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������
    void OnDanceGroupActivityOpenStarPacketResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����̵�һ����
    void OnDanceGroupShopExchangeResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �����ѽ�ɢ
    void OnDanceGroupRemoved(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ˢ��������Ϣ
    void OnDanceGroupBeRefuseRefresh(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // ��ȡ������Ϣ
    void OnGetMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������Ž��
    void OnCreateDanceGroupResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������֪ͨ
    void OnAddRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����ȡ������֪ͨ
    void OnRemoveRequestDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ���ų�Ա����
    void OnAddDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �ɹ���������
    void OnEnterDanceGroupSuccess(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������Ż�����Ϣ
    void OnUpdateDanceGroupBaseInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա��Ϣ
    void OnUpdateDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������Դ���
    void OnDonateDanceGroupResourceResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �Ƴ����ų�Ա��Ϣ
    void OnRemoveDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��������״̬
    void OnUpdateDanceGroupActivityStarState(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ��ȡ�����̵���Ϣ���
    void OnDanceGroupShopGetInfoResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������ų�Ա������
    void OnAcceptDanceGroupMemberInvitationResult(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // ����������������ֵ
    void OnUpdateChallengeActive(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // �������Ƿ���Ա�����
    void OnCanBeInviteCheck(GameMsg_Base &msg, CSlotPeer &slotPeer);

	// ������ʽ��
	// ���뿪��������ʽ
	void OnRequestApplyCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// �������������ʽ
	void OnRequestEnterCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// �����˳�������ʽ
	void OnRequestQuitCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ����ˢ��������ʽ��Ϣ
	void OnRequestRefreshCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ������������ʽ
	void OnRequestStartCeremony(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ��������
	void OnSetCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ȡ����������
	void OnCancelCeremonyRookie(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// �߳�������ʽ�ķ���
	void OnKickCeremonyPlayer(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// �������ҽ���
	void OnSaveCeremonyIntruduction(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ������ʽ��־
	void OnRequestCeremonyRecord(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ����
	void OnRequestCeremonyPraise(GameMsg_Base &msg, CSlotPeer &slotPeer);
	// ���������־
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
    void OnG2SNoticeBigMamaState(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // �״̬���/������/�رգ�

private:
    void OnG2SBigMamaInfoNotifyOnLogin(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ����֪ͨbigMama��Ϣ
    void OnG2SBigMamaInfoNotifyOnJoinDanceGroup(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // �¼���������Ϣͬ��
    void OnG2SBroadBigMamaInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnG2SBigMamaInfoNotify(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    void OnG2SBigMamaFightResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnG2SBigMamaGetRewardResult(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //////////////////////////////////////////////////////////////////////////
    // from client.
    void OnRequestBigMamaStageInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ����ؿ���Ϣ
    void OnRequestFightBigMama(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ������ս

    void OnRequestGetBigMamaReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ������ȡ����
    void OnCompleteBigMamaGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ��ɴ�������
    void OnRequestBigMamaGuideReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); // ������ȡ������������

//FairyLand
public:
    //�����ؾ��Ľ���
    void OnG2SSendJoinRandomReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer); 
//     �����ؾ�����
//  void OnAnnouncementFairyLandEnd(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //��ȡ�����ؾ���Ϣ
    void OnGetFairyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //����
    void OnSetFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnCheckFairyLandGuide(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //��ȡ���а����
    void OnGetFairyLandRankRule(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //��ȡ��������
    void OnGetFairyLandConfig(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //��ȡ�ؿ�����
    void OnGetFairyLandChapterInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

    //��ȡ�ؾ����а�
    void OnGetFairyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //��ȡ�������а���
    void OnGetYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //�����������а���
    void OnSendYesterdayRankReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    // ��ȡף����Ϣ
    void OnFairyLandOpenBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //ף��
    void OnFairyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);

public:
    // ��Group Server����������Ϣ
    void Send2GroupMsg(GameMsg_Base &msg);
    // ��ͻ��˷���������Ϣ
    void SendPlayerMsg(GameMsg_Base &msg);
    // �򷿼䷢����Ϣ
    void SendMsgToRooms(GameMsg_Base &msg);

private:
    void GetMyDanceGroupInfo(unsigned short nClickedPlace);
    // ��¼ʱ����������Ϣ
    void PushDanceGroupInfoOnLogin(bool bValid, unsigned short nClickedPlace);
    // ����������Ϣ
    void UpdateDanceGroupInfo();
    // �������ų�Ա��Ϣ
    void UpdateMemberInfo(bool bOnline);
    // ����������
    void UpdateDanceGroupName(const std::string &rStrNewName);
    // ��������ְλ��Ϣ
    void UpdateDanceGroupTitle(unsigned char nTitle);
    // �������Ż��»�Ч����Ϣ
    void UpdateDanceGroupBadgeOrEffect(unsigned short nBadge, unsigned short nEffect);

    // ����ÿ���״ε������
    bool ProcessFirstReward();

    // �������ID�Ƿ�һ��
    bool IsDanceGroupIDSame(unsigned int nGroupID);

    // ����������Ϣ
    void SetDanceGroupInfo(const CDanceGroupInfo *pDGInfo);
    // ���������Ϣ
    void ClearDanceGroupInfo();

    // ������Ż��²�������
    EDanceGroupBadgeResult CheckDanceGroupBadgeCond();
    // ������Ż����
    EDanceGroupActivityStarResult CheckDanceGroupActivityStarCond();
    // ������Ż���Ӷ���ֵ����
    EDanceGroupActivityStarResult CheckDanceGroupActivityAddStarPowerCond(unsigned short nType);
    // ��������̵�����
    EDanceGroupShopResult CheckDanceGroupShopCond();
    // �����������
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
    unsigned int                m_nGroupID;             // ��ǰ����ID
    CDanceGroupInfo             *m_pGroupInfo;          // ��ǰ������Ϣ

    bool                        m_bTodayFirstLogin;     // �����һ�ε�½

    // ���Ż
    bool                        m_bNormalReward;        // ÿ���һ�β������� һ�¡���õ�����(�������ŵ�����£������ù��˾������½�����Ҳ�����ٻ������)
    unsigned int                m_nLastGetRewardTime;

    bool                        m_bChallengeOpening;
    unsigned short              m_nChallengeChance;             // ʣ����������
    unsigned short              m_nChallengeTotalChance;        // �����ܴ���
    unsigned int                m_nChallengeChanceResetTime;    // ������������ʱ��

    unsigned short              m_nChallengeBuyChance;          // �������
    unsigned int                m_nChallengeBuyChanceResetTime; // ��������ʱ��

    int                         m_nChallengeCurStorey;          // ��ǰ����������
    unsigned char               m_nChallengeCurStoreyState;     // ��ǰ������״̬
    unsigned short              m_nChallengeMyInspire;          // ��ǰ�������Լ��������

    int                         m_nChallengeScene;              // ��ս����
    int                         m_nChallengeMusic;              // ��ս����
    int                         m_nChallengeMusicMode;          // ��սģʽ
    int                         m_nChallengeMusicLevel;         // ��ս�Ѷ�
    int                         m_nChallengeSaveStorey;         // ������ս�Ĳ���
    int                         m_nChallengeSaveStartTime;      // ���濪ʼʱ��

    unsigned int                m_nChallengeStartTime;          // ����������ʼʱ��

    unsigned int                m_nChallengeRoomID;             // ������������ID

    std::map<int, DanceGroupChallengeRanking> m_mapRanking;     // ����δ��ȡ��������
	
    // ��ֹ�ͻ������������Σ�����Ҫ�������ݿ�
    unsigned int                m_nCeremonyRoomLastPraise;      // ������ʽ��һ�ε���ȴʱ��

    EntityBigMama               m_entityBigMama;                // ��ս����
};

typedef CRoleComponentImpl<CEntityDanceGroupComponent, CGetRoleProcessorObj, CGetRoleProcessorObj>  CRoleDanceGroup;

#endif // __ENTITYDANCEGROUPCOMPONENT_H__

