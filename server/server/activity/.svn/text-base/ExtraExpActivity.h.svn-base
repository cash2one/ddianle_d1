#ifndef	__EXTRA_EXP_ACTIVITY_H__
#define __EXTRA_EXP_ACTIVITY_H__

#include "TimerActivity.h"

class CExtraExpActivity : public CDayTimeBlockActivity
{
public:
	CExtraExpActivity();
	virtual ~CExtraExpActivity();

	//初始化
	void Init(int nActivityID, const char * szBeginTime, const char * szEndTime,
		const char * szStartTime, const char * szStopTime, unsigned short nReewardExpRate);
	//活动开始运营
	virtual void OnStart();
	//活动结束运营
	virtual void OnStop();
	//发送活动状态到客户端
	virtual void UpdateStateToClient(CEntityNetComponent & rNetCom);
private:
	int						m_nActivityID;			//活动编号
	unsigned short			m_nRewardExpRate;		//经验奖励百分比
};
#endif


