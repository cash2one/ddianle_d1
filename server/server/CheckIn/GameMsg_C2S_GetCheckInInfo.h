//签到界面的信息

#ifndef		__GAMEMSG_C2S_GETCHECKININFO_H__
#define		__GAMEMSG_C2S_GETCHECKININFO_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_GetCheckInInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCheckInInfo);
	GameMsg_C2S_GetCheckInInfo();
	~GameMsg_C2S_GetCheckInInfo();
public:
	bool doDecode(CParamPool &IOBuff);
public:
};



#endif
// end of file


