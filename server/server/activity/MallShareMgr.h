/** MallShareMgr.h
* Description: 商城分享活动数据管理器
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: fanliangyuan
* Create time: 2014-11-4 21:16:28
*/
#ifndef __MALL_SHARE_DATA_MGR_H__
#define __MALL_SHARE_DATA_MGR_H__

#include "SocialShareActivity.h"
#include "ActivityDataStruct.h"

class GameMsg_C2S_ActivitySocialShare;
class CEntityActivityComponent;
class CRoleEntity;

class CMallSocialShareMgr
{
public:
	CMallSocialShareMgr();
	~CMallSocialShareMgr();
public:
	static CMallSocialShareMgr& Instance();
public:
	void Init();
	const CMallShareActivityData& GetData() const { return m_data; }
	void SetData(CMallShareActivityData val) { m_data = val; }
	bool IsOpeningShow(){ return m_bIsOpeningShow; }
	void SetIsOpeningShow(bool val) { m_bIsOpeningShow = val; }
	bool IsActivityOpenInTime(time_t nTime);
	bool IsActivityOpenNow();
public:
	void OnHandleMsg(CRoleEntity* pRole, CEntityActivityComponent* pActivityComponent, GameMsg_C2S_ActivitySocialShare& msg);
private:
	CMallShareActivityData m_data;	//商城分享活动数据
	bool m_bIsOpeningShow;			//是否显示活动
	CCrontab m_beginTime;			//活动开启时间
	CCrontab m_endTime;				//活动结束时间
};

#endif

