#ifndef	__INTIME_ONLINE_ACTIVITY_H__
#define __INTIME_ONLINE_ACTIVITY_H__

#include "TimerActivity.h"
#include <list>
#include <string>

class CItem;
class CInTimeOnlineActivity : public CDayActivity
{
public:
	CInTimeOnlineActivity();
	virtual ~CInTimeOnlineActivity();

	//初始化
	void Init(int nActivityID, const char * szBeginTime, const char * szEndTime, const char * szTriggeringTime,
		const char * szMaleReward, const char * szFemaleReward, unsigned int nMoneyReward, const char * szTitle, const char * szContent);
	//心跳
	virtual void OnUpdate(const unsigned long &lTimeElapsed, time_t tNow);
	//活动触发时
	virtual void OnTime(const unsigned long &lTimeElapsed, time_t tNow);
private:
	//处理奖励
	void _DealReward(const unsigned long &lTimeElapsed);
private:
	int m_nActivityID;			//活动编号
	std::string m_strTitle;		//邮件标题
	std::string m_strContent;	//邮件内容
	std::list< CItem > m_MaleRewardItemList;	//男性奖励物品列表
	std::list< CItem > m_FemaleRewardItemList;	//女性奖励物品列表
	unsigned int m_nMoneyReward;				//金钱奖励

	std::list< unsigned int > m_RoleList;	//奖励人群
};

class CShowInTimeOnlineActivityInfo: 
	public CTimeActivity
{
public:
	CShowInTimeOnlineActivityInfo();
	~CShowInTimeOnlineActivityInfo();
public:

public:
	void Init(time_t tBeginTime, time_t tEndTime);
	virtual void OnActivityBegin();
	virtual void OnActivityEnd();
};

#endif


