#include "../../socket/Utility.h"
#include "GameMsg_Account_CheckActivateResult.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include "AccountMsgDef.h"

GameMsg_Account_CheckActivateResult::GameMsg_Account_CheckActivateResult():
GameMsg_Base(MSG_ACCOUNT_CheckActivateResult),
m_nResult(0)
{
}

GameMsg_Account_CheckActivateResult::~GameMsg_Account_CheckActivateResult()
{
}

bool GameMsg_Account_CheckActivateResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nResult);

	return true;
}




//end file


