#ifndef	__GAMEMSG_S2G_GETBLACKLISTONLINEINFO_H__
#define __GAMEMSG_S2G_GETBLACKLISTONLINEINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_GetBlackListOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetBlackListOnlineInfo);
	GameMsg_S2G_GetBlackListOnlineInfo();
	~GameMsg_S2G_GetBlackListOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nRoleID;
};

#endif





