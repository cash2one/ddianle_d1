#include "GameMsg_G2S_Chat.h"
#include "ChatMsgDef.h"
#include "../../datastructure/Macro_Define.h"

GameMsg_G2S_Chat::GameMsg_G2S_Chat(void)
    : GameMsg_Base( MSG_G2S_Chat )
    , m_nPhase( 0 )
    , m_nResult( 0 )
    , m_nChannel( 0 )
    , m_nColor( CHAT_DEFAULT_TEXT_COLOR )
    , m_nSenderRoleID( 0 )
    , m_nReceiverRoleID( 0 )
    , m_strSenderName( "" )
    , m_strReceiverName( "" )
    , m_strText( "" )
    , m_nClickSex( 0 )
    , m_nExtraType( EChatExtraType_Default )
    , m_bIsVIP( false )
    , m_nVIPLevel( 0 )
    , m_bVoiceChat( false )
    , m_WorldChatFlag( 0 )
{
}

GameMsg_G2S_Chat::GameMsg_G2S_Chat(unsigned char nPhrase, unsigned char nResult, unsigned char nChannel, unsigned char nColor, 
				 unsigned int SenderRoleID, unsigned int nReceiverRoleID, const char * szSenderName, const char* szReceiverName, 
				 const char *szText, unsigned char nClickSex, EChatExtraType nExtraType,
				 bool bIsVIP, unsigned short nVIPLevel, bool bVoiceChat,unsigned char cWorldChatFlag)
:GameMsg_Base(MSG_G2S_Chat)
,m_nPhase(nPhrase)
,m_nResult(nResult)
,m_nChannel(nChannel)
,m_nColor(CHAT_DEFAULT_TEXT_COLOR)
,m_nSenderRoleID(SenderRoleID)
,m_nReceiverRoleID(nReceiverRoleID)
,m_nClickSex(nClickSex)
,m_nExtraType(nExtraType)
,m_bIsVIP(bIsVIP)
,m_nVIPLevel(nVIPLevel)
,m_bVoiceChat(bVoiceChat)
,m_WorldChatFlag(cWorldChatFlag)
{
	if (szSenderName != NULL) {
		m_strSenderName = szSenderName;	
	}
	
	if (szReceiverName != NULL) {
		m_strReceiverName = szReceiverName;	
	}
	if (szText != NULL) {
		m_strText = szText;	
	}
}

GameMsg_G2S_Chat::~GameMsg_G2S_Chat(void)
{
}


bool GameMsg_G2S_Chat::doEncode( CParamPool& IOBuff )
{
	IOBuff.AddUChar(m_nPhase);
	IOBuff.AddUChar(m_nResult);
	IOBuff.AddUChar(m_nChannel) ;
	IOBuff.AddUChar(m_nColor) ;
	IOBuff.AddUInt(m_nSenderRoleID);
	IOBuff.AddUInt(m_nReceiverRoleID) ;
	IOBuff.AddStringW(m_strSenderName.c_str()) ;
	IOBuff.AddStringW(m_strReceiverName.c_str());
	IOBuff.AddStringW(m_strText.c_str()) ;
	IOBuff.AddUChar(m_nClickSex);
	IOBuff.AddChar((char)m_nExtraType);
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);
	IOBuff.AddBool(m_bVoiceChat);
	IOBuff.AddUChar(m_WorldChatFlag);
	
	return true;
}
bool GameMsg_G2S_Chat::doDecode( CParamPool& IOBuff )
{
	m_nPhase = IOBuff.GetUChar();
	m_nResult = IOBuff.GetUChar();
	m_nChannel = IOBuff.GetUChar();
	m_nColor = IOBuff.GetUChar();
	m_nSenderRoleID = IOBuff.GetUInt();
	m_nReceiverRoleID = IOBuff.GetUInt();
	IOBuff.GetStringW(m_strSenderName) ;
	IOBuff.GetStringW(m_strReceiverName);
	IOBuff.GetStringW(m_strText);
	m_nClickSex = IOBuff.GetUChar();
	m_nExtraType = (EChatExtraType)IOBuff.GetUChar();
	m_bIsVIP = IOBuff.GetBool();
	m_nVIPLevel = IOBuff.GetUShort();
	m_bVoiceChat = IOBuff.GetBool();
	m_WorldChatFlag = IOBuff.GetUChar();
	
	return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_G2S_GroupDanceChatHistory::GameMsg_G2S_GroupDanceChatHistory()
    :GameMsg_Base(MSG_G2S_GroupDanceChatHistory)
    , m_nSenderRoleID(0)
    , m_nColor(0)
    , m_nChannel(0)
    , m_strSenderName("")
    , m_strText("")
    , m_nClickSex(0)
    , m_nExtraType(0)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_bVoiceChat(false)
    , m_WorldChatFlag(0)
{

}

GameMsg_G2S_GroupDanceChatHistory::~GameMsg_G2S_GroupDanceChatHistory()
{
}

GameMsg_G2S_GroupDanceChatHistory::GameMsg_G2S_GroupDanceChatHistory(CChatHistory &rhistory)
    :GameMsg_Base(MSG_G2S_GroupDanceChatHistory)
{
    m_nSenderRoleID = rhistory.m_nSenderRoleID;
    m_nChannel = rhistory.m_nChannel;
    m_strSenderName = rhistory.m_strSenderName;
    m_strText = rhistory.m_strText;
    m_nClickSex = rhistory.m_nClickSex;
    m_nExtraType = rhistory.m_nExtraType;
    m_bIsVIP = rhistory.m_bIsVIP;
    m_nVIPLevel = rhistory.m_nVIPLevel;
    m_bVoiceChat = rhistory.m_bVoiceChat;
    m_WorldChatFlag = rhistory.m_WorldChatFlag;
}


bool GameMsg_G2S_GroupDanceChatHistory::doDecode(CParamPool &IOBuff)
{
    m_nSenderRoleID = IOBuff.GetUInt();
    m_nColor = IOBuff.GetUChar();
    m_nChannel = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strSenderName);
    IOBuff.GetStringW(m_strText);
    m_nClickSex = IOBuff.GetUChar();
    m_nExtraType = IOBuff.GetInt();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_bVoiceChat = IOBuff.GetBool();
    m_WorldChatFlag = IOBuff.GetUChar();

    return true;
}

bool GameMsg_G2S_GroupDanceChatHistory::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSenderRoleID);
    IOBuff.AddUChar(m_nColor);
    IOBuff.AddUChar(m_nChannel);
    IOBuff.AddStringW(m_strSenderName);
    IOBuff.AddStringW(m_strText);
    IOBuff.AddUChar(m_nClickSex);
    IOBuff.AddInt(m_nExtraType);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddBool(m_bVoiceChat);
    IOBuff.AddUChar(m_WorldChatFlag);
    return true;
}

//file end

