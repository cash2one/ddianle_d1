#include "GameMsg_Account_BillingState.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"

GameMsg_Account_BillingState::GameMsg_Account_BillingState(void)
:_baseClass(MSG_ACCOUNT_BillingInfo)
{
	nPt = 0;
	nChange = 0;

}

GameMsg_Account_BillingState::~GameMsg_Account_BillingState(void)
{
}

bool GameMsg_Account_BillingState::doEncode( CParamPool& IoBuff )
{
	IoBuff.AddInt(nPt);
	IoBuff.AddInt(nChange);
	return true;

}


//end file


