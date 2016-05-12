#ifndef	__SYSTEM_ACTIVITY_MANAGER_H__
#define __SYSTEM_ACTIVITY_MANAGER_H__

#include <map>
#include <list>
#include "GiftSystem.h"

class ITimerActivity;
class CRoleEntity;

//整点在线奖励刷新器
class CSystemActivityManager
{
protected:
	CSystemActivityManager();
	virtual ~CSystemActivityManager();

public:
	static CSystemActivityManager & Instance();

	//添加时间活动
	void AddTimerActivity(ITimerActivity * pActivity);
	//心跳更新
	void OnUpdate(const unsigned long &lTimeElapsed);
	//角色登陆时，需要将一些活动的状态同步到客户端
	void OnRoleLogin(CRoleEntity & rRole);
public:
	CGiftSystem & GiftSystem(){return m_GiftSystem;}
private:
	std::list< ITimerActivity * > m_TimerActivityList;	//时间活动列表
	CGiftSystem m_GiftSystem;							//礼品系统
};

#endif


