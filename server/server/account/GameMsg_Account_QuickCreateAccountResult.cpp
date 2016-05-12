#include "GameMsg_Account_QuickCreateAccountResult.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"
#include "../../socket/Utility.h"

GameMsg_Account_QuickCreateAccountResult::GameMsg_Account_QuickCreateAccountResult():GameMsg_Base(MSG_ACCOUNT_QuickCreateAccooutResult)
{
	m_nGateSession = 0;
	memset(szName, 0, MAX_NAME_LEN);
	memset(szPass, 0, MAX_PASS_LEN);
	m_strResultMsg = "";
}
GameMsg_Account_QuickCreateAccountResult::~GameMsg_Account_QuickCreateAccountResult()
{
}
bool GameMsg_Account_QuickCreateAccountResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddInt64(m_nGateSession);
	IOBuff.AddUChar(nResult);
	IOBuff.AddStringW(m_strResultMsg.c_str());
	IOBuff.AddStringW(szName);
	IOBuff.AddStringW(szPass);

	return true;
}


//end file


