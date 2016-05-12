#ifndef		__GAMEMSG_S2C_NEW_COMPENSATION_H__
#define		__GAMEMSG_S2C_NEW_COMPENSATION_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <string>
#include <list>

class CParamPool;

class GameMsg_S2C_NewCompensation : public GameMsg_Base
{
public:
	GameMsg_S2C_NewCompensation();
	~GameMsg_S2C_NewCompensation();

public:
	virtual bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_GetCompensation 
	: public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCompensation);

	GameMsg_C2S_GetCompensation();
	~GameMsg_C2S_GetCompensation();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetCompensationSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetCompensationSuc();
	~GameMsg_S2C_GetCompensationSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	int m_nMoney;
	std::string m_strTitle;
	std::string m_strContent;
	std::list<CItem> m_Item;
};


class GameMsg_S2C_GetCompensationFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GetCompensationFail();
	~GameMsg_S2C_GetCompensationFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string m_strError;
};

#endif
