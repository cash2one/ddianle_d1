#include "GameMsg_C2S_ChangeRoleName.h"
#include "../../socket/ParamPool.h"
#include "../../server/share/ServerMsgDef.h"
#include "../../socket/Utility.h"
#include "../../datastructure/datainfor/ServerConfig.h"

GameMsg_C2S_ChangeRoleName::GameMsg_C2S_ChangeRoleName():
GameMsg_Base(MSG_C2S_ChangeRoleName)
{
	memset(m_szNewName, 0, MAX_NAME_LEN);
}

GameMsg_C2S_ChangeRoleName::~GameMsg_C2S_ChangeRoleName()
{
}

bool GameMsg_C2S_ChangeRoleName::doDecode(CParamPool &IOBuff)
{
	string temp;
	IOBuff.GetStringW(temp);
	safememcpy(m_szNewName, g_ServerConfig.m_MaxNameLimit - 1, temp.c_str(), (unsigned int)temp.length());
	
	return true;
}

//end

