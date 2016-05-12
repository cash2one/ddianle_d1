#include "GameMsg_Account_QuickCreateAccount.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"

GameMsg_Account_QuickCreateAccount::GameMsg_Account_QuickCreateAccount(__int64 nSessionID):
GameMsg_Base(MSG_ACCOUNT_QuickCreateAccoout),
m_nGateSessionID(nSessionID)
{
}

GameMsg_Account_QuickCreateAccount::~GameMsg_Account_QuickCreateAccount()
{
}

bool GameMsg_Account_QuickCreateAccount::doDecode(CParamPool &IoBuff)
{
	m_nGateSessionID = IoBuff.GetInt64();

	return true;
}



//end file






