#ifndef __GAMEMSG_G2S_NOTIFYHASBEENACCEPTED_H__
#define __GAMEMSG_G2S_NOTIFYHASBEENACCEPTED_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>

using namespace std;

class CParamPool;

class GameMsg_G2S_NotifyHasBeenAccpted:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NotifyHasBeenAccpted);
	GameMsg_G2S_NotifyHasBeenAccpted();
	~GameMsg_G2S_NotifyHasBeenAccpted();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	FriendInfo m_friendInfo;
};

#endif
// end of file

