#include "GameMsg_S2C_Chat_Priv.h"
#include "../server/ManagerMsgDef.h"

GameMsg_S2C_Chat_Priv::GameMsg_S2C_Chat_Priv(void):_baseClass(MSG_S2C_CHAT_PRIV)
{
	m_bVoiceChat = false;
}

GameMsg_S2C_Chat_Priv::~GameMsg_S2C_Chat_Priv(void)
{
}
GameMsg_S2C_Chat_Priv* GameMsg_S2C_Chat_Priv::Create(const char* szSenderName,unsigned int _nEntityID,const char* szContext , unsigned short dwColor1 ,unsigned short dwColor2, bool bVoiceChat  )
{
	GameMsg_S2C_Chat_Priv* pMsg = new GameMsg_S2C_Chat_Priv;
	pMsg->nMsg = MSG_S2C_CHAT_PRIV;
	pMsg->nEntityID = _nEntityID;

	if(szSenderName)
	{
		szSenderName = szSenderName;
	}

	pMsg->strContext = szContext;
	pMsg->dwColor[0] = dwColor1;
	pMsg->dwColor[1] = dwColor2;
	pMsg->m_bVoiceChat = bVoiceChat;
	return pMsg;
}

bool GameMsg_S2C_Chat_Priv::doEncode( CParamPool& IoBuff )
{
	return false;
}
bool GameMsg_S2C_Chat_Priv::doDecode( CParamPool& IoBuff )
{
	return false;
}

