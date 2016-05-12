#ifndef		__GAMEMSG_S2C_ACHIEVEMENTCOMPLETED_H__
#define		__GAMEMSG_S2C_ACHIEVEMENTCOMPLETED_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2C_AchievementCompleted:
	public GameMsg_Base
{
public:
	GameMsg_S2C_AchievementCompleted(unsigned short nAchievementID = 0, unsigned int nCompletedTime = 0);
	~GameMsg_S2C_AchievementCompleted();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nAchievementID;
	unsigned int m_nCompletedTime;
};

#endif
// end of file

