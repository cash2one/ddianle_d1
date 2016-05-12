#ifndef __GAMEMSG_C2S_GETROLEINFO_H__
#define __GAMEMSG_C2S_GETROLEINFO_H__

#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_C2S_GetRoleInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRoleInfo);
	GameMsg_C2S_GetRoleInfo();
	~GameMsg_C2S_GetRoleInfo();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nRoleID;
};

#endif
// end of file


