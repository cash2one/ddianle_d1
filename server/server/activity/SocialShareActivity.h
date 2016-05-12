/** MallShareActivity.h
* Description: 商城分享活动，又名灰姑娘活动
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-11-4 21:10:35
*/
#ifndef __SOCIAL_SHARE_ACTIVITY_H__
#define __SOCIAL_SHARE_ACTIVITY_H__

#include "TimerActivity.h"
#include "../../datastructure/Macro_Define.h"

class CSocialShareActivityShow: 
	public CCrontabActivity
{
public:
	CSocialShareActivityShow(EActivity eActivity);
	~CSocialShareActivityShow();
public:
	void Init(const char* strBeginTimeShow, const char* strEndTimeShow);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
	virtual void UpdateStateToClient(CEntityNetComponent & rNetCom);
};

//////////////////////////////////////////////////////////////////////////

class CParamPool;
class CRoleEntity;
class CRewardTitleInfo;
class CSocialShareActivityData;
class CEntityActivityComponent;
class GameMsg_C2S_ActivitySocialShare;

class CSocialShareActivity : public CMonthWeekDayActivity
{
public:
    CSocialShareActivity(EActivity eActivity);
    virtual ~CSocialShareActivity() {}

public:
    virtual void Init(time_t beginShowTime, time_t endShowTime);

    virtual void SetRewardItemInfo(const CRewardTitleInfo *pItemInfo);

    // 当活动开启
    virtual void OnActivityBegin();
    // 当活动结束
    virtual void OnActivityEnd();

    const CSocialShareActivityData* GetActivityData();

    bool doEncode(CParamPool &IOBuff, bool bFemale, bool bReward);

    virtual void ProcessActivityMsg(CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg);

    virtual void ResendVIPExtraReward(unsigned char nOpenVIPFromType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent);

protected:
    virtual void OnActivityChange(bool bBegin);

protected:
    CSocialShareActivityData    *m_pData;
    time_t                      m_tLastUpdateTime;
};

//////////////////////////////////////////////////////////////////////////

class CMallSocialShareActivityData;

class CMallSocialShareActivity : public CSocialShareActivity
{
public:
    CMallSocialShareActivity(const CMallSocialShareActivityData &rData, time_t beginShowTime, time_t endShowTime);
    CMallSocialShareActivity(const CMallSocialShareActivity &ref);
    ~CMallSocialShareActivity();

public:
    virtual void SetRewardItemInfo(const CRewardTitleInfo *pItemInfo);

    void ProcessActivityMsg(CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg);
};

//////////////////////////////////////////////////////////////////////////

class CPhotoSocialShareActivityData;

class CPhotoSocialShareActivity : public CSocialShareActivity
{
public:
    CPhotoSocialShareActivity(const CPhotoSocialShareActivityData &rData, time_t beginShowTime, time_t endShowTime);
    CPhotoSocialShareActivity(const CPhotoSocialShareActivity &ref);
    ~CPhotoSocialShareActivity();

public:
    virtual void SetRewardItemInfo(const CRewardTitleInfo *pItemInfo);

    void ProcessActivityMsg(CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent, GameMsg_C2S_ActivitySocialShare &msg);

    void ResendVIPExtraReward(unsigned char nOpenVIPFromType, CRoleEntity *pRole, CEntityActivityComponent *pActivityComponent);
};

#endif

