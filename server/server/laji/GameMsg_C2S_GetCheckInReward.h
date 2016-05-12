#ifndef	__GAMEMSG_C2S_GETCHECKINREWARD_H__
#define __GAMEMSG_C2S_GETCHECKINREWARD_H__

#include "../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_GetCheckInReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCheckInReward);
	GameMsg_C2S_GetCheckInReward();
	~GameMsg_C2S_GetCheckInReward();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nDayReward;
};

#endif
// end of file


