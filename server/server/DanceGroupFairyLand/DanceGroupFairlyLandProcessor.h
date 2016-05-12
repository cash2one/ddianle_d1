/*
* DanceGroupFairlyLandProcessor.h
* Description: DanceGroupFairlyLand GroupSVR逻辑
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: lht
* Create time: 2015-1-6 10:11:03
*/

#ifndef __DANCEGROUPFAIRLYLANDPROCESSOR_H__
#define __DANCEGROUPFAIRLYLANDPROCESSOR_H__

#include "../Macros/Macros.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/DataStruct_DanceGroup.h"
#include "../DanceGroupFairyLand/DanceGroupFairyLandCfgMgr.h"
#include <map>

class CGroupRoleEntity;

class CDanceGroupFairlyLandProcessor : public GameMsg_Processor
{
    typedef std::map<int, DanceGroupFairlyLandGroupInfo> GroupInfoMap;                                      //map<group, groupinfo>
    typedef std::map<int, DanceGroupFairlyLandPersonalInfo> PersonalInfoMap;                                //map<role , personalinfo>
    typedef std::map<int, PersonalInfoMap > GroupPersonalMap;                                               //map<group ,map<role, personalinfo> >

    typedef GroupInfoMap::iterator GroupInfoMapIt;
    typedef PersonalInfoMap::iterator PersonalInfoMapIt;
    typedef GroupPersonalMap::iterator GroupPersonalMapIt;

private:
    CDanceGroupFairlyLandProcessor();
    virtual ~CDanceGroupFairlyLandProcessor();

public:
    CLASS_INSTANCE_FUNC(CDanceGroupFairlyLandProcessor)
    void Initialize();
    void OnUpdate(time_t tNow);
    //time process
    void NewDayProcess(time_t tNow);
    void ActiveTimeProcess(time_t tNow);
    void PassedRewardTimeProcess(time_t tNow);
    void AnnouncementTimeProcess(time_t tNow);
    void StorageBoxRewardTimeProcess(time_t tNow);

    time_t NextPassRewardTime(time_t tNow);
    time_t NextStorageBoxRewardTime(time_t tNow);
    time_t NextAnnouncementTime(time_t tNow);


    void RegisterMessage();
    void RegisterMessageDealy();

public:
    void OnRoleLogin(CGroupRoleEntity &groupRole);
    void OnRoleQuitGroup(unsigned int nDanceGroup, unsigned int nRoleID);
    void OnDanceGroupDisband(unsigned int nDanceGroup);

    void SendFairlyLandMsg(CGroupRoleEntity &groupRole);
    void CheckBlessStamp(time_t now, unsigned int nDanceGroupID);

    bool CheckAllowReadyInFairlyLandRoom(int nGroupId, int nRoleid);
    bool CheckAllowStart();
    bool CheckIsFinishedAllChapter(int nGroupId);

    int GetCurChapterLvl(int nGroupId);
    int GetCurMusic(int nGroupId);
    int GetCurScene(int nGroupId);
    int GetDifficultyLvl(int nGroupId);

    // 获取，如果没有则添加并且获取
    GroupInfoMapIt EnsureGetFairlyLandGroup(unsigned int nGroupID);
    void AddFairlyLandGroup(DanceGroupFairlyLandGroupInfo& Ginfo);
    void AddFairlyLandPersonal(int nGroupId, int nRoleId, DanceGroupFairlyLandPersonalInfo &Pinfo);
    void DelFairlyLandPersonal(int nGroupId, int nRoleId);
    void DelFairlyLandGroup(int nGroupId);

    void GetRankList(int nGroupId, int nType, std::list<FairlyLandRankCell> &rRank);

    // 给舞团加分
    void TryAddFairlyLandGroupScore(int nGroup, int nChapter, int nAddScore);
    // 舞团设置为下一关卡
    void GroupToNextChapterlvl(int nGroupid);
    // 记录过关标记
    void RecordPassedReward(int nChapter, DanceGroupFairlyLandPersonalInfo &personalInfo);
    // 记录参与标记
    void RecordChapterJoinFlag(int nChapter, int nGroupId, int nRoleId);
    // 舞蹈结束处理
    void DanceOverScoreProcess(int nGroup, int nChapter, std::list<SDancerScore>& listScore);

    void RankToYesterDay();

    void AddPersonalTotalScore(int nGroupId, int nRoleId, int nAddScore);
    void AddPersonalTotalBlessTimes(int nGroupId, int nRoleId);

    //奖励
    void NewDayClear();
    void SendJoinReward(int GId, int Rid, int chapter);
    void SendAllPassedReward();
    void SendAllStorageBoxReward();
    void AddStroageBox(int GID, int Rid, int chapter);

    /// <summary>
    /// MsgCB
    /// <summary>

    void OnCheckIsHaveYesterdayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnFairlyLandGroupInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnFairlyLandStorageBoxInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnFairlyLandRankInfo(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnFairlyLandSendYesterDayReward(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    void OnFairlyLandBless(GameMsg_Base &rMsgBase, CSlotPeer & SlotPeer);
    //DbCB
    void OnClearAllPassedRewardCB(QueryBase &rQuery);
    void OnClearAllStorageBoxRewardCB(QueryBase &rQuery);
    void OnLoadAllInfo(QueryBase &rQuery);
//    void OnLoadLastStartTime(QueryBase &rQuery);

protected:
    DanceGroupFairlyLandPersonalInfo *GetPersonalInfo(unsigned int nGroupID, unsigned int nRoleID);
    int GetDefaultChapter() const;
    void GetChapterPassReward(FairlyLandChapterReward &reward, int nChapter, unsigned char sex);

    bool IsFairlyLandOpening() const;

    // 最后一期的活动的相关奖励是否已经发放
    bool NeedDoLastDayClear(time_t nLastTime, time_t now);
    bool NeedDoLastPassRewardSend(time_t nLastTime, time_t now);
    bool NeedDoLastStorgeBoxSend(time_t nLastTime, time_t now);


    void CheckRoleGroup(unsigned int nRoleID, unsigned int nGroupID);
    void UpdateRoleGroupInfo(unsigned int nRoleID, unsigned int nGroupID);
    void RemoveRoleGroupInfo(unsigned int nRoleID);
    void ConstuctRoleGroupMap();
private:
    GroupInfoMap m_mapGroupInfo;
    GroupPersonalMap m_mapGroupIndexPcInfo;

    // <roleID, groupID>
    std::map<unsigned int, unsigned int> m_mapRoleGroup; // 匹配如果玩家舞团和记录不匹配的情况下，删除记录数据

    time_t m_nPrvCheckTime;

    bool    m_bIsFairlyLandActive;

    time_t m_nNextSendPassedRewardTime;
    time_t m_nNextSendStorageBoxTime;
    time_t m_nNextAnnouncementTime;


    bool m_bLoadComplete;
};


#endif