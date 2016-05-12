/*
**	从黑名单名单中加入好友
**
*/
#ifndef __GAMEMSG_S2G_ADDBLACKFRIEND_H__
#define __GAMEMSG_S2G_ADDBLACKFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_AddBlackListFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddBlackListFriend);
	GameMsg_S2G_AddBlackListFriend();
	~GameMsg_S2G_AddBlackListFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	FriendInfo m_FriendInfo;
};

#endif
// end of file

