#include "GameMsg_Player_CheckRoleNameResult.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"


GameMsg_Player_CheckRoleNameResult::GameMsg_Player_CheckRoleNameResult():GameMsg_Base(MSG_Role_CheckRoleNameResult)
{
	m_nResult = CheckRoleName_Fail;	
	m_strResult = "";
}

GameMsg_Player_CheckRoleNameResult::~GameMsg_Player_CheckRoleNameResult()
{
}

bool GameMsg_Player_CheckRoleNameResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddStringW(m_strResult.c_str());

	return true;	
}

//file end

