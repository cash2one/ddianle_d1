#ifndef __GAMEMSG_S2G_ADDMUTELIST_H__
#define __GAMEMSG_S2G_ADDMUTELIST_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_AddMuteList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_AddMuteList);
	GameMsg_S2G_AddMuteList();
	GameMsg_S2G_AddMuteList(unsigned int nRoleID, unsigned int nMuteID);
	~GameMsg_S2G_AddMuteList();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nMuteID;
};

#endif
// end of file

