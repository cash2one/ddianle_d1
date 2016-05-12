#ifndef _H_GAMEMGS_G2S_NEW_COMPENSATION_H
#define _H_GAMEMGS_G2S_NEW_COMPENSATION_H

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>
#include <map>

class CParamPool;


class GameMsg_G2S_SyncCompensation : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_SyncCompensation);
	GameMsg_G2S_SyncCompensation();
	~GameMsg_G2S_SyncCompensation();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	map<int,CompensationInfo> m_CompensationMap;
};


class GameMsg_G2S_NewCompensation : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_NewCompensation);
	GameMsg_G2S_NewCompensation();
	~GameMsg_G2S_NewCompensation();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	CompensationInfo m_CompensationInfo;
};

#endif
