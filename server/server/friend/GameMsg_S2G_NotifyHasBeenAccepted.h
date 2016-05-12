#ifndef __GAMEMSG_S2G_NOTIFYHASBEENACCEPTED_H__
#define __GAMEMSG_S2G_NOTIFYHASBEENACCEPTED_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_S2G_NotifyHasBeenAccpted:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_NotifyHasBeenAccpted);
	GameMsg_S2G_NotifyHasBeenAccpted();
	~GameMsg_S2G_NotifyHasBeenAccpted();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nRoleID;
	FriendInfo m_friendInfo;
};

#endif
// end of file

