#ifndef	__GAMEMSG_S2C_UPDATE_GIFT_INFO_H__
#define	__GAMEMSG_S2C_UPDATE_GIFT_INFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

class CParamPool;

class GameMsg_S2C_UpdateGiftInfo : public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateGiftInfo(int nIndexID, int nGiftID, unsigned int nExpiredTime, std::list< CItem > & rRewardList, int nRewardMoney);
	virtual ~GameMsg_S2C_UpdateGiftInfo();
public:
	bool doEncode(CParamPool &IOBuff);
private:
	int m_nIndexID;						//编号
	int m_nGiftID;						//礼品编号
	unsigned int m_nExpiredTime;		//过期时间
	int m_nRewardMoney;					//奖励金钱
	std::list< CItem > m_RewardList;	//奖励物品
};

#endif
// end of file


