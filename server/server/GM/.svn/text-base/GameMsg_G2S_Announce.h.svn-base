#ifndef	__GAMEMSG_G2S_ANNOUNCE_H__
#define __GAMEMSG_G2S_ANNOUNCE_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_G2S_Announce:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_Announce);
	GameMsg_G2S_Announce();
	~GameMsg_G2S_Announce();
public:
	CAnnounceBase m_announce;
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
};

#endif
// end of file


