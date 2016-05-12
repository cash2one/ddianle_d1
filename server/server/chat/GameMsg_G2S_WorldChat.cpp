#include "GameMsg_G2S_WorldChat.h"
#include "../../socket/ParamPool.h"
#include "ChatMsgDef.h"

GameMsg_G2S_WorldChat::GameMsg_G2S_WorldChat()
: GameMsg_Base(MSG_G2S_World)
,m_nSenderRoleID(0)
,m_strSenderName("")
,m_strText("")
,m_nClickSex(0)
{

}

GameMsg_G2S_WorldChat::GameMsg_G2S_WorldChat(unsigned int nSenderRoleID, const char *szSenderName, const char *szText, unsigned char nClicksex)
: GameMsg_Base(MSG_G2S_World)
,m_nSenderRoleID(nSenderRoleID)
,m_strSenderName(szSenderName)
,m_strText(szText)
,m_nClickSex(nClicksex)
{
}

GameMsg_G2S_WorldChat::~GameMsg_G2S_WorldChat()
{
}

bool GameMsg_G2S_WorldChat::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nSenderRoleID);
	IOBuff.AddStringW(m_strSenderName.c_str());
	IOBuff.AddStringW(m_strText.c_str());
	IOBuff.AddUChar(m_nClickSex);
	
	return true;
}

bool GameMsg_G2S_WorldChat::doDecode(CParamPool &IOBuff)
{
	m_nSenderRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strSenderName);
	IOBuff.GetStringW(m_strText);
	m_nClickSex = IOBuff.GetUChar();

	return true;
}




