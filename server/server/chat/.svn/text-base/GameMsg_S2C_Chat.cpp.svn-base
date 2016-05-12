#include "GameMsg_S2C_Chat.h"
#include "ChatMsgDef.h"

GameMsg_S2C_Chat::GameMsg_S2C_Chat(void)
    : GameMsg_Base(MSG_S2C_CHAT)
    , m_strSenderName("")
    , m_nSenderRoleID(0)
    , m_nChannel(0)
    , m_nColor(CHAT_DEFAULT_TEXT_COLOR)
    , m_strText("")
    , m_nExtraType(-1)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_bVoiceChat(false)
    , m_WorldChatFlag(0)
{
}

GameMsg_S2C_Chat::GameMsg_S2C_Chat(const char* szSenderName, unsigned int nSenderRoleID, unsigned char nChannel, 
    unsigned char nColor, const char* szText, unsigned char nClickSex, EChatExtraType eExtraType, bool bIsVIP, 
    unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag)
    : GameMsg_Base(MSG_S2C_CHAT)
    , m_nSenderRoleID(nSenderRoleID)
    , m_nChannel(nChannel)
    , m_nColor(nColor)
    , m_nClickSex(nClickSex)
    , m_nExtraType((char)eExtraType)
    , m_bIsVIP(bIsVIP)
    , m_nVIPLevel(nVIPLevel)
    , m_bVoiceChat(bVoiceChat)
    , m_WorldChatFlag(cWorldChatFlag)
{
    if (szSenderName != NULL)
        m_strSenderName = szSenderName;

    if (szText != NULL)
        m_strText = szText;
}

GameMsg_S2C_Chat::~GameMsg_S2C_Chat(void)
{
}

GameMsg_S2C_Chat* GameMsg_S2C_Chat::Create(const char* szSenderName, unsigned int nSenderRoleID, unsigned char nChannel, 
    const char* szText , unsigned char nColor, unsigned char nClickSex, EChatExtraType eExtraType, bool bIsVIP, 
    unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag)
{
    GameMsg_S2C_Chat* pMsg = new GameMsg_S2C_Chat;

    if (pMsg != NULL)
    {
        pMsg->m_nSenderRoleID = nSenderRoleID;
        pMsg->m_nColor = nColor;
        pMsg->m_nChannel = (unsigned char)nChannel;
        if(szSenderName != NULL)
        {
            pMsg->m_strSenderName = szSenderName;
        }
        if (szText != NULL)
        {
            pMsg->m_strText =  szText;
        }
        pMsg->m_nClickSex = nClickSex;
        pMsg->m_nExtraType = (char)eExtraType;
        pMsg->m_bIsVIP = bIsVIP;
        pMsg->m_nVIPLevel = nVIPLevel;
        pMsg->m_bVoiceChat = bVoiceChat;
        pMsg->m_WorldChatFlag = cWorldChatFlag;
    }

    return pMsg;
}

bool GameMsg_S2C_Chat::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nSenderRoleID);//∑¢ÀÕ’ﬂid
    IOBuff.AddUChar(m_nColor);
    IOBuff.AddUChar(m_nChannel);
    IOBuff.AddStringW(m_strSenderName.c_str() );
    IOBuff.AddStringW(m_strText.c_str() );
    IOBuff.AddUChar(m_nClickSex);
    IOBuff.AddChar(m_nExtraType);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddBool(m_bVoiceChat);
    IOBuff.AddUChar(m_WorldChatFlag);

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_SyncAutoChatInfo::GameMsg_S2C_SyncAutoChatInfo()
    : GameMsg_Base(MSG_S2C_SyncAutoChatInfo)
    , m_fInterval(3)
    , m_fMaxPauseTime(60)
{
}

GameMsg_S2C_SyncAutoChatInfo::~GameMsg_S2C_SyncAutoChatInfo()
{
}

bool GameMsg_S2C_SyncAutoChatInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_fInterval << m_fMaxPauseTime;

    return true;
}

//////////////////////////////////////////////////////////////////////////

GameMsg_S2C_AutoChatResponse::GameMsg_S2C_AutoChatResponse()
    : GameMsg_Base(MSG_S2C_AutoChatResponse)
    , m_bChatSuccess(false)
{
}

GameMsg_S2C_AutoChatResponse::~GameMsg_S2C_AutoChatResponse()
{
}

bool GameMsg_S2C_AutoChatResponse::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_bChatSuccess;

    return true;
}

//end file


