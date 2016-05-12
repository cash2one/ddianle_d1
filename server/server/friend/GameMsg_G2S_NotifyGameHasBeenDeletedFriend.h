#ifndef __GAMEMSG_G2S_NOTIFYGAMEHASBEENDELETEDFRIEND_H__
#define __GAMEMSG_G2S_NOTIFYGAMEHASBEENDELETEDFRIEND_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_G2S_NotifyGameHasBeenDeletedFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyGameHasBeenDeletedFriend);
	GameMsg_G2S_NotifyGameHasBeenDeletedFriend(unsigned int nRoleID = 0, unsigned int nRemovedID = 0);
	~GameMsg_G2S_NotifyGameHasBeenDeletedFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;	// who remove me
};

#endif
// end of file

