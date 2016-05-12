#include	"GAMEMSG_C2S_GetCheckInReward.h"
#include	"share/ServerMsgDef.h"

GameMsg_C2S_GetCheckInReward::GameMsg_C2S_GetCheckInReward():
GameMsg_Base(MSG_C2S_GetCheckInReward)
{
	m_nDayReward = 0;
}
GameMsg_C2S_GetCheckInReward::~GameMsg_C2S_GetCheckInReward()
{
}
bool GameMsg_C2S_GetCheckInReward::doDecode(CParamPool &IOBuff)
{
	m_nDayReward = IOBuff.GetUChar();

	return true;
}


// end of file

