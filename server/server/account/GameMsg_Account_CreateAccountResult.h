#ifndef __GAMEMSG_ACCOUNT_CREATEACCOUNTRESULT_H__
#define __GAMEMSG_ACCOUNT_CREATEACCOUNTRESULT_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

using namespace std;
class CParamPool;

enum
{
	CreateAccountSuccess,
	CreateAccountfailed,
};

class GameMsg_Account_CreateAccountResult :
public GameMsg_Base
{
public:
	__int64 m_nGateSessionID;
	unsigned char m_nResult;
	string m_strResultMsg;
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_CreateAccountResult);
	GameMsg_Account_CreateAccountResult();
	~GameMsg_Account_CreateAccountResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);
};

#endif


//end file


