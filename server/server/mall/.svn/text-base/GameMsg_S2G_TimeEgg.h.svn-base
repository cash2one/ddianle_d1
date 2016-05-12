#ifndef __GAMEMSG_S2G_TIMEEGG_H__
#define __GAMEMSG_S2G_TIMEEGG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class GameMsg_S2G_CreateTimeEgg:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_CreateTimeEgg);
	GameMsg_S2G_CreateTimeEgg();
	~GameMsg_S2G_CreateTimeEgg();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nEggID;
};
class GameMsg_S2G_OpenTimeEgg:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_OpenTimeEgg);
	GameMsg_S2G_OpenTimeEgg();
	~GameMsg_S2G_OpenTimeEgg();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned int m_nEggID;
};
class GameMsg_S2G_TimeEggSetting:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2G_TimeEggSetting);
	GameMsg_S2G_TimeEggSetting();
	~GameMsg_S2G_TimeEggSetting();
public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);
public:
	TimeEggSetting m_setting;
};
#endif

