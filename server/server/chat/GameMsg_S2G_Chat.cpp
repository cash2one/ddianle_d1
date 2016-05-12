#include "GameMsg_S2G_Chat.h"
#include "ChatMsgDef.h"

GameMsg_S2G_Chat::GameMsg_S2G_Chat(void)
    : GameMsg_Base(MSG_S2G_Chat)
{
    m_nPhase = 0;
    m_nResult = eChat_Success;
    m_nChannel = 0;                     //频道
    m_nColor = CHAT_DEFAULT_TEXT_COLOR;
    m_nSenderRoleID = 0;                //如果是私聊，接受者的roleid
    m_nReceiverRoleID = 0;
    m_strSenderName = "";
    m_strReceiverName = "";
    m_strText = "";                     //聊天的文字
    m_nClickSex = 0;
    m_nExtraType = EChatExtraType_Default;
    m_bIsVIP = false;
    m_nVIPLevel = 0;
    m_bVoiceChat = false;
    m_nWorldChatFlag = 0;
    m_bAutoChat = false;
}

GameMsg_S2G_Chat::GameMsg_S2G_Chat(const GameMsg_S2G_Chat &ref)
    : GameMsg_Base(MSG_S2G_Chat)
{
    m_nPhase = ref.m_nPhase;
    m_nResult = ref.m_nResult;
    m_nChannel = ref.m_nChannel;
    m_nColor = ref.m_nColor;
    m_nSenderRoleID = ref.m_nSenderRoleID;
    m_nReceiverRoleID = ref.m_nReceiverRoleID;
    m_strSenderName = ref.m_strSenderName;
    m_strReceiverName = ref.m_strReceiverName;
    m_strText = ref.m_strText;
    m_nClickSex = ref.m_nClickSex;
    m_nExtraType = ref.m_nExtraType;
    m_bIsVIP = ref.m_bIsVIP;
    m_nVIPLevel = ref.m_nVIPLevel;
    m_bVoiceChat = ref.m_bVoiceChat;
    m_nWorldChatFlag = ref.m_nWorldChatFlag;
    m_bAutoChat = ref.m_bAutoChat;
}

GameMsg_S2G_Chat::GameMsg_S2G_Chat(unsigned char nPhrase, unsigned char nResult, unsigned char nChannel, unsigned char nColor, 
    unsigned int SenderRoleID, unsigned int nReceiverRoleID, const char *szSenderName, const char *szReceiverName, 
    const char *szText, unsigned char nClickSex, EChatExtraType nExtraType, bool bIsVIP, 
    unsigned short nVIPLevel, bool bVoiceChat, unsigned char nWorldChatFlag, bool bAutoChat /* = false */
)
    : GameMsg_Base(MSG_S2G_Chat)
    , m_nPhase(nPhrase)
    , m_nResult(nResult)
    , m_nChannel(nChannel)
    , m_nColor(nColor)
    , m_nSenderRoleID(SenderRoleID)
    , m_nReceiverRoleID(nReceiverRoleID)
    , m_nClickSex(nClickSex)
    , m_nExtraType(nExtraType)
    , m_bIsVIP(bIsVIP)
    , m_nVIPLevel(nVIPLevel)
    , m_bVoiceChat(bVoiceChat)
    , m_nWorldChatFlag(nWorldChatFlag)
    , m_bAutoChat(bAutoChat)
{
    if (szSenderName != NULL)
        m_strSenderName = szSenderName;

    if (szReceiverName != NULL)
        m_strReceiverName = szReceiverName;

    if (szText != NULL)
        m_strText = szText;
}

GameMsg_S2G_Chat::~GameMsg_S2G_Chat(void)
{
}

bool GameMsg_S2G_Chat::doEncode( CParamPool& IOBuff )
{
    IOBuff.AddUChar(m_nPhase);
    IOBuff.AddUChar(m_nResult);
    IOBuff.AddUChar(m_nChannel);
    IOBuff.AddUChar(m_nColor);
    IOBuff.AddUInt(m_nSenderRoleID);
    IOBuff.AddUInt(m_nReceiverRoleID);
    IOBuff.AddString(m_strSenderName.c_str() );
    IOBuff.AddString(m_strReceiverName.c_str() );
    IOBuff.AddString(m_strText.c_str() );
    IOBuff.AddUChar(m_nClickSex);
    IOBuff.AddChar((char)m_nExtraType);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddBool(m_bVoiceChat);
    IOBuff.AddUChar(m_nWorldChatFlag);
    IOBuff.AddBool(m_bAutoChat);

    return true;
}

bool GameMsg_S2G_Chat::doDecode( CParamPool& IOBuff )
{
    m_nPhase = IOBuff.GetUChar();
    m_nResult = IOBuff.GetUChar();
    m_nChannel = IOBuff.GetUChar();
    m_nColor = IOBuff.GetUChar();
    m_nSenderRoleID = IOBuff.GetUInt();
    m_nReceiverRoleID = IOBuff.GetUInt();
    m_strSenderName = IOBuff.GetString();
    m_strReceiverName = IOBuff.GetString();
    m_strText = IOBuff.GetString();
    m_nClickSex = IOBuff.GetUChar();
    m_nExtraType = (EChatExtraType)IOBuff.GetUChar();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_bVoiceChat = IOBuff.GetBool();
    m_nWorldChatFlag = IOBuff.GetUChar();
    m_bAutoChat = IOBuff.GetBool();

    return true;
}
//////////////////////////////////////////////////////////////////////////

GameMsg_S2G_GroupDanceChatHistoryRecord::GameMsg_S2G_GroupDanceChatHistoryRecord( )
    : GameMsg_Base(MSG_S2G_GroupDanceChatHistoryRecord)
    , m_nGroupId(0)
    , m_nSenderRoleID(0)
    , m_nColor(CHAT_DEFAULT_TEXT_COLOR)
    , m_nChannel(0)
    , m_strSenderName("")
    , m_strText("")
    , m_nExtraType(-1)
    , m_bIsVIP(false)
    , m_nVIPLevel(0)
    , m_bVoiceChat(false)
    , m_WorldChatFlag(0)
{
}

GameMsg_S2G_GroupDanceChatHistoryRecord::GameMsg_S2G_GroupDanceChatHistoryRecord(int nGroupId ,const char* szSenderName, unsigned int nSenderRoleID, unsigned char nChannel,
    unsigned char nColor, const char* szText, unsigned char nClickSex, EChatExtraType eExtraType, bool bIsVIP,
    unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag)
    : GameMsg_Base(MSG_S2G_GroupDanceChatHistoryRecord)
    , m_nGroupId(nGroupId)
    , m_nSenderRoleID(nSenderRoleID)
    , m_nColor(nColor)
    , m_nChannel(nChannel)
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

GameMsg_S2G_GroupDanceChatHistoryRecord::~GameMsg_S2G_GroupDanceChatHistoryRecord(void)
{
}

bool GameMsg_S2G_GroupDanceChatHistoryRecord::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt(m_nGroupId);
    IOBuff.AddUInt(m_nSenderRoleID);//发送者id
    IOBuff.AddUChar(m_nColor);
    IOBuff.AddUChar(m_nChannel);
    IOBuff.AddStringW(m_strSenderName.c_str());
    IOBuff.AddStringW(m_strText.c_str());
    IOBuff.AddUChar(m_nClickSex);
    IOBuff.AddChar((char)m_nExtraType);
    IOBuff.AddBool(m_bIsVIP);
    IOBuff.AddUShort(m_nVIPLevel);
    IOBuff.AddBool(m_bVoiceChat);
    IOBuff.AddUChar(m_WorldChatFlag);

    return true;
}

bool GameMsg_S2G_GroupDanceChatHistoryRecord::doDecode(CParamPool &IOBuff)
{
    m_nGroupId = IOBuff.GetInt();
    m_nSenderRoleID = IOBuff.GetUInt(); //发送者id
    m_nColor = IOBuff.GetUChar();
    m_nChannel = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strSenderName);
    IOBuff.GetStringW(m_strText);
    m_nClickSex = IOBuff.GetUChar();
    m_nExtraType = IOBuff.GetUChar();
    m_bIsVIP = IOBuff.GetBool();
    m_nVIPLevel = IOBuff.GetUShort();
    m_bVoiceChat = IOBuff.GetBool();
    m_WorldChatFlag = IOBuff.GetUChar();
    return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2G_NeedGroupChatHistory::GameMsg_S2G_NeedGroupChatHistory()
    :GameMsg_Base(MSG_S2G_GroupDanceChatHistory)
    , m_nGroupId(0)
    , m_nRoleId(0)
{}

GameMsg_S2G_NeedGroupChatHistory::~GameMsg_S2G_NeedGroupChatHistory()
{}

///////////////////////////////////////////////////
GameMsg_S2G_Announce::GameMsg_S2G_Announce()
    :GameMsg_Base(MSG_S2G_Announce)
{
}

GameMsg_S2G_Announce::~GameMsg_S2G_Announce()
{
}

bool GameMsg_S2G_Announce::doEncode( CParamPool& IoBuff )
{
    m_announce.doEncode(IoBuff);

    return true;
}

bool GameMsg_S2G_Announce::doDecode( CParamPool& IoBuff )
{
    m_announce.doDecode(IoBuff);

    return true;
}

