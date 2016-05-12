#ifndef __GAMEMSG_S2G_NOTIFYGAMEHASBEENDELETEDFRIEND_H__
#define __GAMEMSG_S2G_NOTIFYGAMEHASBEENDELETEDFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_S2G_NotifyGameHasBeenDeletedFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_NotifyGameHasBeenDeletedFriend);
	GameMsg_S2G_NotifyGameHasBeenDeletedFriend(unsigned int nRoleID = 0, unsigned int nRemovedID = 0);
	~GameMsg_S2G_NotifyGameHasBeenDeletedFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nRemovedID;
};

#endif
// end of file

