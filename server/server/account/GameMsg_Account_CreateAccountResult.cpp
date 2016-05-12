#include "GameMsg_Account_CreateAccountResult.h"
#include "../../socket/ParamPool.h"
#include "AccountMsgDef.h"

GameMsg_Account_CreateAccountResult::GameMsg_Account_CreateAccountResult():GameMsg_Base(MSG_ACCOUNT_CreateAccountResult)
{
	m_nGateSessionID = 0;
	m_nResult = 0 ;
	m_strResultMsg = "";
}

GameMsg_Account_CreateAccountResult::~GameMsg_Account_CreateAccountResult()
{
}
bool GameMsg_Account_CreateAccountResult::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddInt64(m_nGateSessionID);
	IOBuff.AddUChar(m_nResult);	
	IOBuff.AddStringW(m_strResultMsg.c_str());
	
	return true;
}


//end file


