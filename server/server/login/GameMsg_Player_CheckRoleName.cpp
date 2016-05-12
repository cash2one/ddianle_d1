#include "GameMsg_Player_CheckRoleName.h"
#include "../../socket/ParamPool.h"
#include "../share/ServerMsgDef.h"
#include "../../datastructure/datainfor/ItemType.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_Player_CheckRoleName::GameMsg_Player_CheckRoleName():GameMsg_Base(MSG_Role_CheckRoleName)
{
	memset(m_szRoleName, 0, MAX_NAME_LEN);
}

GameMsg_Player_CheckRoleName::~GameMsg_Player_CheckRoleName()
{
}

GameMsg_Player_CheckRoleName::GameMsg_Player_CheckRoleName(const GameMsg_Player_CheckRoleName &rhs)
    : GameMsg_Base(MSG_Role_CheckRoleName)
{
	memcpy(m_szRoleName, rhs.m_szRoleName, g_ServerConfig.m_MaxNameLimit-1);
}

bool GameMsg_Player_CheckRoleName::doDecode(CParamPool &IOBuff)
{	
	string szTemp;
	IOBuff.GetStringW(szTemp);
	CopyString2Buffer(szTemp,m_szRoleName, g_ServerConfig.m_MaxNameLimit - 1);

	return true;	
}

//file end

