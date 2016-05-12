#include "GameMsg_C2S_ChangeRoleSignature.h"
#include "../../socket/ParamPool.h"
#include "../../server/share/ServerMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_C2S_ChangeRoleSignature::GameMsg_C2S_ChangeRoleSignature():
GameMsg_Base(MSG_C2S_ChangeRoleSignature)
{
	memset(m_szNewSignature, 0, MAX_SIGNATURE_LEN);
}

GameMsg_C2S_ChangeRoleSignature::~GameMsg_C2S_ChangeRoleSignature()
{
}

bool GameMsg_C2S_ChangeRoleSignature::doDecode(CParamPool &IOBuff)
{
	string temp;
	IOBuff.GetStringW(temp);
	safememcpy(m_szNewSignature, g_ServerConfig.m_SignatureLength - 1, temp.c_str(), (unsigned int)temp.length());

	return true;
}

//end

