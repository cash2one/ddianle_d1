#ifndef __GAMEMSG_S2G_NOTIFYHASBEENDELETEDFRIEND_H__
#define __GAMEMSG_S2G_NOTIFYHASBEENDELETEDFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_S2G_NotifyHasBeenDeletedFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_NotifyHasBeenDeletedFriend);
	GameMsg_S2G_NotifyHasBeenDeletedFriend(unsigned int nRoleID = 0, unsigned int nRemovedID = 0);
	~GameMsg_S2G_NotifyHasBeenDeletedFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nRemovedID;
};

#endif
// end of file

