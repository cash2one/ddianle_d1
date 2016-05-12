#ifndef __GAMEMSG_ACCOUNT_QUICKCREATEACCOUNTRESULT_H__
#define __GAMEMSG_ACCOUNT_QUICKCREATEACCOUNTRESULT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

using namespace std;
class CParamPool;

enum
{
	QuickCreateResult_FAIL,	
	QuickCreateResult_SUCCESS,
};
class GameMsg_Account_QuickCreateAccountResult:
	public GameMsg_Base
{
public:
	__int64 m_nGateSession;
	unsigned char nResult;
	string m_strResultMsg;
	char szName[MAX_NAME_LEN];
	char szPass[MAX_PASS_LEN];
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_QuickCreateAccountResult);
	GameMsg_Account_QuickCreateAccountResult();
	~GameMsg_Account_QuickCreateAccountResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);
};

#endif


//end file


