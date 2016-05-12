#include "GameMsg_S2C_Chat_Private2Self.h"
#include "../../datastructure/Macro_Define.h"
#include "ChatMsgDef.h"

GameMsg_S2C_Chat_Private2Self::GameMsg_S2C_Chat_Private2Self()
: GameMsg_Base(MSG_S2C_CHAT_PRIVTOSELF)
,m_nReceiverRoleID(0)
,m_strReceiverName("")
,m_nColor(CHAT_DEFAULT_TEXT_COLOR)
,m_strText("")
,m_nClickSex(0)
,m_bVoiceChat(false)
,m_WorldChatFlag(0)
{

}

GameMsg_S2C_Chat_Private2Self::GameMsg_S2C_Chat_Private2Self(unsigned int nReceiverRoleID, const char *szReceiverName, unsigned char nColor, const char *szText, unsigned char nClickSex, bool bVoiceChat,unsigned char cWorldChatFlag, bool bReceiverIsVIP, unsigned short nReceiverVIPLevel)
:GameMsg_Base(MSG_S2C_CHAT_PRIVTOSELF)
,m_nReceiverRoleID(nReceiverRoleID)
,m_strReceiverName(szReceiverName)
,m_nColor(nColor)
,m_strText(szText)
,m_nClickSex(nClickSex)
,m_bVoiceChat(bVoiceChat)
,m_WorldChatFlag(cWorldChatFlag)
,m_bReceiverIsVIP(bReceiverIsVIP)
,m_nReceiverVipLevel(nReceiverVIPLevel)
{
}

GameMsg_S2C_Chat_Private2Self::~GameMsg_S2C_Chat_Private2Self()
{
}

bool GameMsg_S2C_Chat_Private2Self::doEncode(CParamPool &IOBuff)
{
	IOBuff.AddUInt(m_nReceiverRoleID);
	IOBuff.AddStringW(m_strReceiverName.c_str());
	IOBuff.AddUChar(m_nColor);
	IOBuff.AddStringW(m_strText.c_str());
	IOBuff.AddUChar(m_nClickSex);
	IOBuff.AddBool(m_bVoiceChat);
	IOBuff.AddUChar(m_WorldChatFlag);
	IOBuff.AddBool(m_bReceiverIsVIP);
	IOBuff.AddUShort(m_nReceiverVipLevel);
	return true;
}


//end file


