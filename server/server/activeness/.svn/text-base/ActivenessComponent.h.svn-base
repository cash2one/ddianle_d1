/**
* Filename: ActivenessComponent
* Description: 活跃度系统玩家组件
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 14:13:34
*/

#ifndef _ACTIVENESS_COMPONENT_H_
#define _ACTIVENESS_COMPONENT_H_

#include "ActivenessDataManager.h"
#include "../Pattern/Observer.h"
#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"

#include <set>

class CEntityAttributeComponent;
class CEntityNetComponent;

class ActivenessComponent : public CEntityComponent, public CommonSubscriber
{
public:
    ActivenessComponent();
    virtual ~ActivenessComponent();

public:
    virtual void Start();
    virtual bool CreateFromDB(ROLEINFO_DB* pRoleInfoDB);
    virtual bool PacketToCache(ROLEINFO_DB *pRoleInfoCache) const;

    virtual void RegComponentNetMsgMap();
    virtual void OnLogin();

public:
    virtual void OnNotify(IParam &param);

private:
    void SendReward(ActivenessRewardConfig &reward);
    void AddActiveness(int nAddScore, int nTargetType);
    void UpdateTarget(const ActivenessTarget & target);
    void UpdateDanceModeProgress(const ActivenessTarget & target);
    bool HasReceivedReward(int nRequireScore);
    void UpdateGetRewardRecord(int nRewardScore);
    int GetTargetProgress(int nTargetType);
    bool CanGetReward();
    void SendActivenessInfo();
    void SaveActivenessLog(CRoleActivenessLog::EActivenessAction eAction, int nTargetType, int nScore);

private:
    void OnGetActiveness(GameMsg_Base &msg, CSlotPeer &slot);
    void OnGetActivenessReward(GameMsg_Base &msg, CSlotPeer &slot);

    void ResetActivinessData(time_t nNow);
    void SaveActivinessDataToDb();
private:
    CEntityAttributeComponent *m_pRoleAttr;
    CEntityNetComponent       *m_pRoleNet;

    int m_nActivenessScore;             // 当前玩家活跃度积分
    unsigned int m_nUpdateTime;         // 活跃度积分时间
    std::set<int> m_setRewardLevelScore;// 领取奖励的积分列表(DB中记录的是领取时应该消耗的分数)
    /* 特别说明: 玩家进度只在退出登陆时保存, 包括目标进度和已完成跳舞模式set集合 */
    std::map<int, int> m_mapTargetProgress; // 目标进度, key: 目标类型TargetType, value: 目标进度Progress
    std::set<int> m_setFinishedDanceMode;//已完成跳舞模式
};

typedef CRoleComponentImpl<ActivenessComponent, CGetRoleProcessorObj, CGetRoleProcessorObj> CRoleActiveness;

#endif //_ACTIVENESS_COMPONENT_H_

