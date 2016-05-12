#ifndef	__GAMEMSG_G2S_FINDPLAYERFROM_H__
#define __GAMEMSG_G2S_FINDPLAYERFROM_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

class CParamPool;

class GameMsg_G2S_FindPlayerResult:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_FindPlayerResult);
	GameMsg_G2S_FindPlayerResult();
	~GameMsg_G2S_FindPlayerResult();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	std::string m_strError;
	std::list<CFindPlayerInfo> m_PlayerList;
};

#endif
// end of file

