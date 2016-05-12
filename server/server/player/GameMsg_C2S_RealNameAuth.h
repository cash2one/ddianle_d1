#ifndef __GAMEMSG_C2S_REALNAMEAUTH_H__
#define __GAMEMSG_C2S_REALNAMEAUTH_H__


#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_SynAdult:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SynAdult);
	GameMsg_C2S_SynAdult();
	~GameMsg_C2S_SynAdult();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nAdult;
};


#endif

