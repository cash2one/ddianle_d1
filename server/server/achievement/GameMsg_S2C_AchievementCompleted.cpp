#include "GameMsg_S2C_AchievementCompleted.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_AchievementCompleted::GameMsg_S2C_AchievementCompleted(unsigned short nAchievementID,unsigned int nCompletedTime):
GameMsg_Base(MSG_S2C_AchievementCompleted)
{
	m_nAchievementID = nAchievementID;
	m_nCompletedTime = nCompletedTime;
}

GameMsg_S2C_AchievementCompleted::~GameMsg_S2C_AchievementCompleted()
{
}

bool GameMsg_S2C_AchievementCompleted::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nAchievementID);
	IOBuff.AddUInt(m_nCompletedTime);
	return true;
}


// end of file

