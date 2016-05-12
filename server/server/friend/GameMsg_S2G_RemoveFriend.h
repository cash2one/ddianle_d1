#ifndef __GAMEMSG_S2G_REMOVEFRIEND_H__
#define __GAMEMSG_S2G_REMOVEFRIEND_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_RemoveFriend:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RemoveFriend);
	GameMsg_S2G_RemoveFriend();
	GameMsg_S2G_RemoveFriend(unsigned int nRoleID, unsigned int nRemoveID);
	~GameMsg_S2G_RemoveFriend();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nTargetID;
};

#endif
// end of file

