#ifndef __GAMEMSG_S2G_GAMESERVERINFO_H__
#define __GAMEMSG_S2G_GAMESERVERINFO_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;


class GameMsg_S2G_GameServerInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GameServerInfo);
	GameMsg_S2G_GameServerInfo();
	~GameMsg_S2G_GameServerInfo();
private:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nLine;
};

#endif


//file end

