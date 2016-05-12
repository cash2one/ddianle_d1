#include	"GameMsg_S2C_AchievementGetRewardResult.h"
#include	"../share/ServerMsgDef.h"

GameMsg_S2C_AchievementGetRewardResult::GameMsg_S2C_AchievementGetRewardResult(unsigned short nAchievementID, unsigned char nResult):
GameMsg_Base(MSG_S2C_AchievementGetRewardResult)
{
	m_nAchievementID = nAchievementID;
	m_nResult = nResult;
}

GameMsg_S2C_AchievementGetRewardResult::~GameMsg_S2C_AchievementGetRewardResult()
{
}

bool GameMsg_S2C_AchievementGetRewardResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUShort(m_nAchievementID);
	IOBuff.AddUChar(m_nResult);
	return true;
}


// end of file

