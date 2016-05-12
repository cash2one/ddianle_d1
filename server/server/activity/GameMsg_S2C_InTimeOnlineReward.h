#ifndef	__GAMEMSG_S2C_INTIME_ONLINE_REWARD_H__
#define	__GAMEMSG_S2C_INTIME_ONLINE_REWARD_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_InTimeOnlineReward : public GameMsg_Base
{
public:
	GameMsg_S2C_InTimeOnlineReward(unsigned char nHour, unsigned char nMinute, std::list< CItem > & rRewardList, int nRewardMoney);
	~GameMsg_S2C_InTimeOnlineReward();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	unsigned char m_nRewardHour;		//奖励时间－小时
	unsigned char m_nRewardMinute;		//奖励时间－分钟
	std::list< CItem > m_RewardList;	//奖励物品
	int m_nRewardMoney;					//奖励金钱
};

#endif
// end of file


