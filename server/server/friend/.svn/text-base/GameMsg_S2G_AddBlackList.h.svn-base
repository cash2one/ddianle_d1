#ifndef __GAMEMSG_S2G_ADDBLACKLIST_H__
#define __GAMEMSG_S2G_ADDBLACKLIST_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_AddBlackList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddBlackList);
	GameMsg_S2G_AddBlackList(unsigned int nRoleID = 0, unsigned int nBlackID = 0);
	~GameMsg_S2G_AddBlackList();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nBlackID;
};

#endif
// end of file

