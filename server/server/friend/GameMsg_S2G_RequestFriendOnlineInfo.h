#ifndef __GAMEMSG_S2G_REQUESTROLEONLINEINFO_H__
#define __GAMEMSG_S2G_REQUESTROLEONLINEINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_RequestFriendOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RequestFriendOnlineInfo);
	GameMsg_S2G_RequestFriendOnlineInfo();
	~GameMsg_S2G_RequestFriendOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcRoleID;
	unsigned int m_nDstRoleID;
	CFriendOnlineInfo m_FriendInfo;
};

#endif

