#ifndef __GAMEMSG_S2G_REMOVEMUTELIST_H__
#define __GAMEMSG_S2G_REMOVEMUTELIST_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_S2G_RemoveMuteList:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_RemoveMuteList);
	GameMsg_S2G_RemoveMuteList();
	GameMsg_S2G_RemoveMuteList(unsigned int nRoleID, unsigned int nRemoveID);
	~GameMsg_S2G_RemoveMuteList();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned int m_nRemoveID;
};

#endif
// end of file

