#ifndef	__GAMEMSG_G2S_ADDMCOIN_H__
#define __GAMEMSG_G2S_ADDMCOIN_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>

class CParamPool;

class GameMsg_G2S_AddMCoinByGm:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_G2S_AddMCoinByGm);
	GameMsg_G2S_AddMCoinByGm();
	~GameMsg_G2S_AddMCoinByGm();

public:
	bool doEncode(CParamPool &IOBuff);
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nPid;
	std::string m_strPUID;
	unsigned int m_nAmount;
	unsigned int m_nBindAmount;
	std::string m_strTransID;
	unsigned int m_nCurrency;
};

#endif
// end of file

