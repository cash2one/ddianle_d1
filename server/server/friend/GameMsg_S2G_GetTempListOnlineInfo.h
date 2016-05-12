#ifndef	__GAMEMSG_S2G_GETTEMPLISTONLINEINFO_H__
#define __GAMEMSG_S2G_GETTEMPLISTONLINEINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_S2G_GetTempListOnlineInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_GetTempListOnlineInfo);
	GameMsg_S2G_GetTempListOnlineInfo();
	~GameMsg_S2G_GetTempListOnlineInfo();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	int m_nRequestRoleID;
	unsigned char m_nListType;
	list<unsigned int>	m_listMate;
};

#endif


