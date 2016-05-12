/*
 * SocialShareMgr.h
 * Description: 分享活动管理类
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-11-21 10:59:43
 */

#ifndef __SOCIALSHAREMGR_H__
#define __SOCIALSHAREMGR_H__

#include <list>
#include <string>
#include <time.h>

class CParamPool;
class CRoleEntity;
class CRewardTitleInfo;
class CSocialShareActivity;
class CEntityActivityComponent;
class CSocialShareActivityData;
class GameMsg_C2S_ActivitySocialShare;

/**
* SocialShareMgr:
* 
*/

class CSocialShareMgr
{
public:
    CSocialShareMgr();
    virtual ~CSocialShareMgr();

    static CSocialShareMgr& Instance();

public:
    void Init();

    void AddSocialShareActivity(const CSocialShareActivity &rActivity);
    void AddRewardItemInfo(int nActivityType, const CRewardTitleInfo *pItemInfo);
    void AddTimeRangeInfo(int nActivityType, time_t begin, time_t end, unsigned int openType, const std::list<unsigned int> &rListDate, std::string &beginHour, std::string &endHour);

    bool IsShowNow(int nActivityType);
    bool IsOpenNow(int nActivityType);

    const CSocialShareActivityData* GetActivityData(int nActivityType) const;

    bool CanShareAgain(int nActivityType, time_t lastSharedTime) const;

    void OnHandleActivityMsg(int nActivityType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg);
    void OnHandleResendVIPExtraRewardMsg(int nActivityType, unsigned char nOpenVIPFromType, 
        CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent);

private:
    unsigned short Count() const;

    CSocialShareActivity* Find(int nActivityType) const;

private:
    std::list<CSocialShareActivity *> m_listActivities; // 分享活动列表
};

#endif // __SOCIALSHAREMGR_H__

