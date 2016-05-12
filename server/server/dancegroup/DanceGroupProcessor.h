/*
 * DanceGroupProcessor.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-1-6 10:11:03
 */

#ifndef __DANCEGROUPPROCESSOR_H__
#define __DANCEGROUPPROCESSOR_H__

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/LogStructDef.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "DanceGroupRankMgr.h"

class CDanceGroupInfo;
class CDanceGroupMember;
class CRequestDanceGroupInfo;

/**
* DanceGroupProcessor:
* 
*/

class CDanceGroupProcessor : public GameMsg_Processor
{
protected:
    CDanceGroupProcessor();
    virtual ~CDanceGroupProcessor();

public:
    static CDanceGroupProcessor& Instance();

    // 初始化
    bool Init();

    // 更新
    void OnUpdate(time_t tNow);

    // 是否启动加载完成
    bool IsLoadComplete();

    const CDanceGroupInfo* GetDanceGroupInfo(unsigned int nGroupID);

    // 重置每日举办入团仪式次数
    void ZeroResetCermonyApplyTimes(time_t now);

    // 更新
    void ChangeMemberProperty(unsigned int nRoleID, unsigned int nDanceGroupID);
    std::string MemberPropertyString(EMemberProperty prop);

    // 添加舞团的举办次数,如果老的次数和新的次数不匹配，则认为group已经发生了切天操作。不再执行次数添加
    void AddGroupCeremonyTime(unsigned int nDanceGroupID, bool bIsFreeTime);

    // 开启试炼塔并记录
    void OpenChallengeStoreyAndRecord( unsigned int nGroupID, const CDanceGroupMember *pMember, 
        int nStorey, int nDecreaseActive, const std::string &rContent );
    // 增加成员贡献值
    void UpdateDanceGroupContribution( unsigned int nGroupID, unsigned int nRoleID, int nValue, const std::string& rType );

private:
    // 注册消息
    void RegisterMessageBeforeLoad();
    void RegisterMessageAfterLoaded();
    // 加载所有舞团数据
    bool LoadAllDanceGroup();

private:
    // 获取舞团信息列表
    void OnGetDanceGroupInfoList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消加入舞团
    void OnCancelRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 获取舞团信息
    void OnGetDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团信息
    void OnGetMyDanceGroupInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 更新舞团成员信息
    void OnUpdateDanceGroupMemberInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 创建舞团
    void OnCreateDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 解散舞团
    void OnDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 取消解散舞团
    void OnCancelDismissDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 申请加入舞团
    void OnRequestEnterDanceGroup(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 压力机器人专用消息：
    void OnRequestEnterDanceGroupByName(GameMsg_Base &msg, CSlotPeer &slotPeer);

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
    // 修改舞团荣誉值
    void OnChangeDanceGroupHonor(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 增加舞团贡献值
    void OnAddDanceGroupContribution(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 减少舞团贡献值
    void OnReduceDanceGroupContribution(GameMsg_Base &msg, CSlotPeer &slotPeer);
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
    // 获得舞团星星信息
    void OnDanceGroupActivityGetStarInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团活动摘星星
    void OnDanceGroupActivityAddStarPower(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取动力值
    void OnDanceGroupActivityGetStarPowerInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 打开礼包
    void OnDanceGroupActivityOpenStarPacket(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 获取舞团商店信息
    void OnDanceGroupShopGetInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 舞团商店兑换
    void OnDanceGroupShopExchange(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 接受舞团成员邀请
    void OnAcceptDanceGroupMemberInvitation(GameMsg_Base &msg, CSlotPeer &slotPeer);
    // 检测玩家是否可以加入舞团
    void OnCanBeInviteCheck(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 增加舞团试炼活力值
    void OnAddDanceGroupChallengeActive(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // 当加载完所有舞团信息
    void OnLoadAllDanceGroupInfo(QueryBase &rQuery);
    // 当数据库创建完舞团
    void OnDanceGroupCreated(QueryBase &rQuery);
    // 当数据库修改完舞团名
    void OnDanceGroupNameChanged(QueryBase &rQuery);

    // 记录申请加入舞团日志
    void SaveRequestEnterDanceGroupInfo(unsigned int nGroupID, const CRequestDanceGroupInfo &rRequestInfo);
    // 记录移除申请加入舞团日志
    void SaveRemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, const std::list<unsigned int> &rListRoleID);
    // 记录舞团记录日志
    void SaveDanceGroupRecordsInfo(CDanceGroupInfo *pDGInfo, unsigned int nRecordRoleID, unsigned int nRecordTime, const std::string &strContent);
    // 记录舞团日志
    void SaveDanceGroupLog(unsigned int nGroupID, const std::string &strGroupName, unsigned int nRoleID, const std::string &strRoleName, EDanceGroupLogAction eAction, ...);
    // 记录添加新成员日志
    void SaveAddDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo);
    // 记录舞团基本信息更新日志
    void SaveChangeDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo);
    // 记录修改舞团名日志
    void SaveChangeDanceGroupNameInfo(unsigned int nGroupID, unsigned int nRoleID, const std::string &strNewName, unsigned int nChangeTime);
    // 记录更新舞团成员信息
    void SaveUpdateDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo);
    // 记录移除舞团成员信息
    void SaveRemoveDanceGroupMemberInfo(unsigned int nGroupID, unsigned int nRoleID);
    // 记录更新舞团活动信息
    void SaveUpdateDanceGroupActivityInfo(unsigned int nGroupID, const CDanceGroupActivityInfo &rDGActivityInfo);
    // 记录更新舞团信息
    void SaveUpdateDanceGroupMemberWithTime(unsigned nGroupID, const CDanceGroupMember &rMemberInfo);

private:
    // 移除玩家所有申请记录
    void ClearAllRequestsInfoByRoleID(unsigned int nRoleID);
    // 移除舞团申请记录
    void RemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, unsigned int nRoleID, bool bNotifyRequestRole, bool bRemoveFromDG);
    void RemoveRequestEnterDanceGroupInfo(unsigned int nGroupID, const std::list<unsigned int> &rListRoleID, bool bNotifyRequestRole, bool bRemoveFromDG);

    // 处理舞团申请
    unsigned short ProcessRequestEnterDanceGroup(CDanceGroupInfo *pDGInfo, unsigned int nRoleID, const char *pRoleName, unsigned int nRequestRoleID, unsigned int nRecordTime);
    // 处理解散舞团
    void ProcessRemoveDanceGroup(unsigned int nGroudID);

    // 添加新成员
    void AddDanceGroupMemberInfo(unsigned int nGroupID, const CRequestDanceGroupInfo &rRequestInfo);
    // 更新成员
    void UpdateDanceGroupMemberInfo(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo);
    // 更新舞团成员信息
    void UpdateDanceGroupMemberWithTime(unsigned int nGroupID, const CDanceGroupMember &rMemberInfo);

    // 移除舞团成员
    void RemoveDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID);

    // 更新舞团记录时间
    void UpdateDanceGroupRecordsTime(CDanceGroupInfo *pDGInfo);

    // 重置舞团活动信息
    void ResetDanceGroupActivityData(CDanceGroupInfo *pDGInfo, bool bWriteDB);
    // 重置舞团成员活动信息
    void ResetDanceGroupMemberActivityData(unsigned int nGroupID, CDanceGroupMember *pMember, bool bWriteDB);

    // 更新舞团基本信息
    void UpdateDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo, EDanceGroupBaseInfoUpdateType eUpdateType);
    // 发送更新舞团基本信息
    void SendUpdateDanceGroupBaseInfo(const CDanceGroupBaseInfo &rBaseInfo, EDanceGroupBaseInfoUpdateType eUpdateType);

    // 发送更新舞团成员信息
    void SendUpdateDanceGroupMemberInfo(unsigned int nGroupID, unsigned int nExceptRoleID, const CDanceGroupMember &rMemberInfo);

    // 发送移除舞团成员信息
    void SendRemoveDanceGroupMemberInfo(unsigned nGroupID, unsigned int nRoleID);

    // 修改舞团荣誉值
    void ChangeDanceGroupHonor(CDanceGroupInfo *pDGInfo, int nChangeHonor, int nCause, int nPara, std::string &rStrType);

    // 记录玩家退出帮会时间点
    void SaveRoleQuitGroupInfo(unsigned int nRoleID, unsigned int nDanceGroupID, unsigned int stamp);

    // 修改试炼活力值
    bool ChangeChallengeActive( CDanceGroupInfo *pDGInfo, const CDanceGroupMember *pMember, unsigned int nValue, bool bAdd, const std::string &rType );

public: // 消息发送函数
    // 向舞团管理者发送消息
    void SendMsg2DanceGroupManager(unsigned int nGroupID, GameMsg_Base &msg);
    // 向舞团所有成员发送消息
    void SendMsg2DanceGroupMember(unsigned int nGroupID, unsigned int nExceptRoleID, GameMsg_Base &msg);
    // 向舞团管理者发送邮件
    void SendMailToDanceGroupManager( CDanceGroupInfo &rDGInfo, EMailType eMailType, const std::string &rTitle, const std::string &rContent );
    // 向舞团所有成员发送邮件
    void SendMailToDanceGroupMember( const CDanceGroupInfo &rDGInfo, EMailType eMailType, const std::string &rTitle, const std::string &rContent );

    // 向玩家发送消息
    void SendPlayerMsgByRoleID(unsigned int nRoleID, GameMsg_Base &msg);
    void SendPlayerMsgByAccountID(unsigned int nAccountID, GameMsg_Base &msg);

private:
    bool                m_bDanceGroupComplete;    // 是否加载完毕

    CDanceGroupManager  m_DGManager;        // 舞团数据管理器
    CDanceGroupRankMgr  m_DGRankManager;    // 舞团排行榜管理器

};

#endif // __DANCEGROUPPROCESSOR_H__

