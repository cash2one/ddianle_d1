#ifndef	__GAMEMSG_G2S_NOTIFYGAMEREMOVEFOLLOWEDFRIEND_H__
#define __GAMEMSG_G2S_NOTIFYGAMEREMOVEFOLLOWEDFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_G2S_NotifyGameRemoveFollowedFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyGameRemoveFollowedFriend);
	GameMsg_G2S_NotifyGameRemoveFollowedFriend();
	//GameMsg_G2S_NotifyGameRemoveFollowedFriend(unsigned int nDesRoleID, FriendInfo& friendInfo);
	GameMsg_G2S_NotifyGameRemoveFollowedFriend(unsigned int nRemovedRoleID);
	~GameMsg_G2S_NotifyGameRemoveFollowedFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	//unsigned int m_nDesRoleID;
	//FriendInfo m_friendInfo;
	unsigned int m_nRemovedRoleID;
};

#endif
// end of file

