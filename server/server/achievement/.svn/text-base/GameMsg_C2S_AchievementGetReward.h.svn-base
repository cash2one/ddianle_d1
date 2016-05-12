#ifndef		__GAMEMSGC2SACHIEVEMENTGETREWARD_H__
#define		__GAMEMSGC2SACHIEVEMENTGETREWARD_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_AchievementGetReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AchievementGetReward);
	GameMsg_C2S_AchievementGetReward();
	~GameMsg_C2S_AchievementGetReward();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned short m_nAchievementID;
};

#endif
// end of file

