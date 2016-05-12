#ifndef __GAMEMSG_ACCOUNT_QUICKCREATEACCOUNT_H__
#define __GAMEMSG_ACCOUNT_QUICKCREATEACCOUNT_H__
#include "../../socket/GameMsg_Base.h"

using namespace std;
class CParamPool;

class GameMsg_Account_QuickCreateAccount:
	public GameMsg_Base
{
private:
public:
	GAMEMSG_CREATEDECLARE(GameMsg_Account_QuickCreateAccount);
	GameMsg_Account_QuickCreateAccount(__int64 nSessionID = 0);
	~GameMsg_Account_QuickCreateAccount();
public:
	__int64 m_nGateSessionID;
	virtual bool doDecode(CParamPool &IoBuff );
};

#endif

//end file


