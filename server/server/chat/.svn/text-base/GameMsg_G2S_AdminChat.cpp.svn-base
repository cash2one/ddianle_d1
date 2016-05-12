#include "GameMsg_G2S_AdminChat.h"
#include "../../socket/ParamPool.h"
#include "ChatMsgDef.h"

GameMsg_G2S_AdminChat::GameMsg_G2S_AdminChat(unsigned int nSenderRoleID, const char *szSenderName, const char *szText):
GameMsg_Base(MSG_G2S_Admin),m_nSenderRoleID(nSenderRoleID),m_strSenderName(szSenderName),m_strText(szText)
{
}

GameMsg_G2S_AdminChat::~GameMsg_G2S_AdminChat()
{
}

bool GameMsg_G2S_AdminChat::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSenderRoleID);
	IOBuff.AddString(m_strSenderName.c_str());
	IOBuff.AddString(m_strText.c_str());

	return true;
}

bool GameMsg_G2S_AdminChat::doDecode(CParamPool &IOBuff)
{
	m_nSenderRoleID = IOBuff.GetUInt();
	m_strSenderName = IOBuff.GetString();
	m_strText = IOBuff.GetString();

	return true;
}





