#ifndef __GAMEMSG_S2G_ADDFRIEND_H__
#define __GAMEMSG_S2G_ADDFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_AddBlacListFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddBlacListFriend);
	GameMsg_S2G_AddBlacListFriend();
	~GameMsg_S2G_AddBlacListFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	FriendInfo m_nFriendInfo;
};

#endif
// end of file

