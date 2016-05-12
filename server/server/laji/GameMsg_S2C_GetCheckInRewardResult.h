#ifndef		__GAMEMSG_S2C_GETCHECKINREWARD_H__
#define		__GAMEMSG_S2C_GETCHECKINREWARD_H__

#include "../socket/GameMsg_Base.h"

class CParamPool;

enum
{
	GetVIPPacketItem_OK = 0,
	GetVIPPacketItem_Fail = 1
};

class GameMsg_S2C_GetCheckInRewardResult:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCheckInRewardResult(unsigned char nDay = 0, unsigned char nResult = 0);
	~GameMsg_S2C_GetCheckInRewardResult();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nDay;
	unsigned char m_nResult;		// 0-³É¹¦ 1-Ê§°Ü
};

#endif
// end of file


