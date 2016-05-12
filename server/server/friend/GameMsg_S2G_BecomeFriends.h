#ifndef __GAMEMSG_S2G_BECOMEFRIENDS_H__
#define __GAMEMSG_S2G_BECOMEFRIENDS_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_BecomeFriends:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_BecomeFriends);
	GameMsg_S2G_BecomeFriends();
	~GameMsg_S2G_BecomeFriends();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	FriendInfo m_friendInfo;
};

#endif
// end of file






