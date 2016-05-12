#ifndef	__GAMEMSG_S2G_GETFRIENDONLINEINFO_H__
#define	__GAMEMSG_S2G_GETFRIENDONLINEINFO_H__ 

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <list>

using namespace std;

class CParamPool;

class GameMsg_S2G_GetFriendOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetFriendOnlineInfo);
	GameMsg_S2G_GetFriendOnlineInfo(unsigned int nRoleID = 0);
	~GameMsg_S2G_GetFriendOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

#endif
// end of file

