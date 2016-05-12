#include "GameMsg_S2C_ChangeRoleSignatureResult.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include "../../socket/Utility.h"

GameMsg_S2C_ChangeRoleSignatureResult::GameMsg_S2C_ChangeRoleSignatureResult(unsigned char nResult):
GameMsg_Base(MSG_S2C_ChangeRoleSignatureResult),
m_nResult(nResult)
{
	memset(m_szNewSignature, 0, MAX_SIGNATURE_LEN);
}

GameMsg_S2C_ChangeRoleSignatureResult::~GameMsg_S2C_ChangeRoleSignatureResult()
{
}

bool GameMsg_S2C_ChangeRoleSignatureResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddStringW(m_szNewSignature);

	return true;
}

//end

