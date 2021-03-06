#ifndef	__GAMEMSG_S2G_NOTIFYGAMEREMOVEFRIEND_H__
#define __GAMEMSG_S2G_NOTIFYGAMEREMOVEFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_NotifyGameRemoveFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_NotifyGameRemoveFriend);
	GameMsg_S2G_NotifyGameRemoveFriend();
	GameMsg_S2G_NotifyGameRemoveFriend(unsigned int nDesRoleID, unsigned int nRemovedRoleID);
	~GameMsg_S2G_NotifyGameRemoveFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nDesRoleID;
	unsigned int m_nRemovedRoleID;
	//FriendInfo m_friendInfo;
};

#endif
// end of file

