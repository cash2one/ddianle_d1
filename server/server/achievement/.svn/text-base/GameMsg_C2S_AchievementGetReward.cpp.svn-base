#include "GameMsg_C2S_AchievementGetReward.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_AchievementGetReward::GameMsg_C2S_AchievementGetReward():
GameMsg_Base(MSG_C2S_AchievementGetReward)
{
	m_nAchievementID = 0;
}

GameMsg_C2S_AchievementGetReward::~GameMsg_C2S_AchievementGetReward()
{
}

bool GameMsg_C2S_AchievementGetReward::doDecode(CParamPool &IOBuff)
{
	m_nAchievementID = IOBuff.GetUShort();
	return true;
}


// end of file

