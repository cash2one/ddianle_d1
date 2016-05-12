#ifndef __GAMEMSG_S2G_REMOVEBLACKLIST_H__
#define __GAMEMSG_S2G_REMOVEBLACKLIST_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_RemoveBlackList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RemoveBlackList);
	GameMsg_S2G_RemoveBlackList(unsigned int nRoleID = 0, unsigned int nRemoveID = 0);
	~GameMsg_S2G_RemoveBlackList();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nSrcRoleID;
	unsigned int m_nTargetID;
};

#endif

