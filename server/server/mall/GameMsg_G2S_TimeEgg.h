#ifndef __GAMEMSG_G2S_TIMEEGG_H__
#define __GAMEMSG_G2S_TIMEEGG_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_G2S_OpenTimeEgg:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_OpenTimeEgg);
	GameMsg_G2S_OpenTimeEgg();
	~GameMsg_G2S_OpenTimeEgg();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nEggID;
};
#endif

