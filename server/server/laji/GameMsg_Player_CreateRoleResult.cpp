#include "GameMsg_Player_CreateRoleResult.h"
#include "../socket/ParamPool.h"
#include "ServerMsgDef.h"
#include "../datastructure/datainfor/ItemType.h"


GameMsg_Player_CreateRoleResult::GameMsg_Player_CreateRoleResult():GameMsg_Base(MSG_S2C_CreateRoleResult)
{
	m_nResult = CREATE_ROLE_RESULT_SUCCESS;
	m_strResultMsg = "";
}

GameMsg_Player_CreateRoleResult::~GameMsg_Player_CreateRoleResult()
{
}

bool GameMsg_Player_CreateRoleResult::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddStringW(m_strResultMsg.c_str());
	return true;
}

//file end

