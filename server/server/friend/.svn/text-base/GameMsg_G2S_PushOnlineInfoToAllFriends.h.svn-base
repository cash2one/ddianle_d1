#ifndef	__GAMEMSG_G2S_PUSHONLINEINFOTOALLFRIENDS_H__
#define __GAMEMSG_G2S_PUSHONLINEINFOTOALLFRIENDS_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_G2S_PushOnlineInfoToAllFriends:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_PushOnlineInfoToAllFriends);
	GameMsg_G2S_PushOnlineInfoToAllFriends();
	~GameMsg_G2S_PushOnlineInfoToAllFriends();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	CFriendOnlineInfo	m_roleOnlineInfo;
};

#endif
// end of file

