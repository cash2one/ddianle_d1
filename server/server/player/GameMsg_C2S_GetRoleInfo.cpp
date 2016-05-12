#include "GameMsg_C2S_GetRoleInfo.h"
#include "../share/ServerMsgDef.h"

GameMsg_C2S_GetRoleInfo::GameMsg_C2S_GetRoleInfo(): GameMsg_Base(MSG_C2S_GetRoleInfo)
{
	m_nRoleID = 0;
}
GameMsg_C2S_GetRoleInfo::~GameMsg_C2S_GetRoleInfo()
{
}
bool GameMsg_C2S_GetRoleInfo::doDecode(CParamPool &IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	return true;
}



// end of file


