#ifndef __GAMEMSG_ACCOUNT__GETGAMESERVERSINFO_H__
#define __GAMEMSG_ACCOUNT__GETGAMESERVERSINFO_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <list>

using namespace std;


class CParamPool;

class GameMsg_Account_C2S_GetGameServersInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_C2S_GetGameServersInfo);
	GameMsg_Account_C2S_GetGameServersInfo();
	~GameMsg_Account_C2S_GetGameServersInfo();
public:
	virtual bool doDecode( CParamPool& IoBuff );
public:
};

class GameMsg_Account_S2C_GetGameServersInfoRes:
	public GameMsg_Base
{
public:			
	GameMsg_Account_S2C_GetGameServersInfoRes();
	~GameMsg_Account_S2C_GetGameServersInfoRes();
public:
	virtual bool doEncode(CParamPool& IoBuff );
public:
	int m_nCurLine;
	list<CGameServerInfo>	m_listGameServersInfo;
};



#endif

//end file


