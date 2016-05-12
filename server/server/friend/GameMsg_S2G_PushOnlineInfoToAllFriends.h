#ifndef __GAMEMSG_S2G_PUSHONLINEINFOTOALLFRIENDS_H__
#define __GAMEMSG_S2G_PUSHONLINEINFOTOALLFRIENDS_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_PushOnlineInfoToAllFriends:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_PushOnlineInfoToAllFriends);
	GameMsg_S2G_PushOnlineInfoToAllFriends();
	~GameMsg_S2G_PushOnlineInfoToAllFriends();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

#endif


