#include "GameMsg_S2G_SwitchLineComplete.h"
#include "../share/ServerMsgDef.h"


GameMsg_S2G_SwitchLineComplete::GameMsg_S2G_SwitchLineComplete():
GameMsg_Base(MSG_PLAYER_S2C_LOGOUTFORSWITCHSERVERCOMPLETE),
m_nLine(0),
m_strErrorMsg("")
{
}

GameMsg_S2G_SwitchLineComplete::~GameMsg_S2G_SwitchLineComplete()
{
}

bool GameMsg_S2G_SwitchLineComplete::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nLine);
	IOBuff.AddStringW(m_strErrorMsg.c_str());

	return true;
}


// end of file


