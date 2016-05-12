#ifndef __GAMEMSG_ACCOUNT_CHECKACTIVATERESULT_H__
#define __GAMEMSG_ACCOUNT_CHECKACTIVATERESULT_H__

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"

class CParamPool;

class GameMsg_Account_CheckActivateResult:
	public GameMsg_Base
{
public:
	unsigned char m_nResult;	// 0表示成功，其余表示对应的错误码
public:
	GameMsg_Account_CheckActivateResult();
	~GameMsg_Account_CheckActivateResult();
public:
	virtual bool doEncode(CParamPool &IOBuff);
};

#endif


//end file


