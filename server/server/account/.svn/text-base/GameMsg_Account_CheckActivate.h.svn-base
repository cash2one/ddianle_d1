#ifndef __GAMEMSG_ACCOUNT_CHECKACTIVATE_H__
#define __GAMEMSG_ACCOUNT_CHECKACTIVATE_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_Account_CheckActivate:
	public GameMsg_Base
{
public:
	char	m_szActivateCode[MAX_ACTIVATE_CODE];
	int		m_nLine;
	std::string m_strPUID;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_CheckActivate);
	GameMsg_Account_CheckActivate();
	~GameMsg_Account_CheckActivate();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

#endif




