#ifndef	__GAMEMSG_S2G_NOTIFYGAMEHASBEENDELETEDBLACKLIST_H__
#define __GAMEMSG_S2G_NOTIFYGAMEHASBEENDELETEDBLACKLIST_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist);
	GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist(unsigned int nDesRoleID = 0, unsigned int nRemovedRoleID =0);
	~GameMsg_S2G_NotifyGameHasBeenDeletedBlacklist();
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

