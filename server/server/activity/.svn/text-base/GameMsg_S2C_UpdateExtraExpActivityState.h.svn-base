#ifndef	__GAMEMSG_S2C_UPDATE_EXTRA_EXP_ACTIVITY_STATE_H__
#define	__GAMEMSG_S2C_UPDATE_EXTRA_EXP_ACTIVITY_STATE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_UpdateExtraExpActivityState : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateExtraExpActivityState(int nActivityID, bool bOpening, unsigned char nStartHour, unsigned char nStartMinute,
		unsigned char nStopHour, unsigned char nStopMinute, unsigned short nRewardExpRate);
	~GameMsg_S2C_UpdateExtraExpActivityState();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	int					m_nActivityID;		//活动编号
	bool				m_bOpening;			//是否开启
	unsigned char		m_nStartHour;		//活动运营开始时间－小时
	unsigned char		m_nStartMinute;		//活动运营开始时间－分钟
	unsigned char		m_nStopHour;		//活动运营结束时间－小时
	unsigned char		m_nStopMinute;		//活动运营结束时间－分钟
	unsigned short		m_nRewardExpRate;	//经验奖励百分比
};

#endif
// end of file


