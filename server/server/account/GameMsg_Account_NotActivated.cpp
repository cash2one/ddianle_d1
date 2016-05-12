#include "../../socket/Utility.h"
#include "GameMsg_Account_NotActivated.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Base.h"
#include "AccountMsgDef.h"


GameMsg_Account_NotActivated::GameMsg_Account_NotActivated():
GameMsg_Base(MSG_ACCOUNT_NotActivated)
{
}

GameMsg_Account_NotActivated::~GameMsg_Account_NotActivated()
{
}

bool GameMsg_Account_NotActivated::doEncode(CParamPool &IOBuff)
{
	return true;
}


//end file


