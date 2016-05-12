#ifndef		__GAMEMSGS2CACHIEVEMENTGETREWARDRESULT_H__
#define		__GAMEMSGS2CACHIEVEMENTGETREWARDRESULT_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2C_AchievementGetRewardResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AchievementGetRewardResult(unsigned short nAchievementID = 0, unsigned char nResult = 1);
	~GameMsg_S2C_AchievementGetRewardResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nAchievementID;
	unsigned char m_nResult;	// 0表示失败，1表示成功
};

#endif

// end of file









