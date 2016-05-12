#ifndef __GAMEMSG_S2A_ADDMCOINBYCMD_H__
#define __GAMEMSG_S2A_ADDMCOINBYCMD_H__

#include "../../socket/GameMsg_Base.h"
#include <string>
using namespace std;

class GameMsg_S2A_AddMCoinByCmd :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2A_AddMCoinByCmd);
	GameMsg_S2A_AddMCoinByCmd(void);
	~GameMsg_S2A_AddMCoinByCmd(void);

	unsigned int  nAccount;
	unsigned int  nAmount;
public:
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool& IoBuff);
};

class GameMsg_S2A_AddMCoinByGm :
	public GameMsg_Base
{
	typedef  GameMsg_Base _baseClass;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2A_AddMCoinByGm);
	GameMsg_S2A_AddMCoinByGm(void);
	~GameMsg_S2A_AddMCoinByGm(void);

	unsigned short nPid;
	std::string strPUID;
	unsigned int  nAmount;
	unsigned int  nBindAmount;
	std::string strTransID;
	unsigned int m_nCurrency;
public:
	bool doEncode(CParamPool & IOBuff);
	bool doDecode(CParamPool& IoBuff);
};

#endif

//file end

