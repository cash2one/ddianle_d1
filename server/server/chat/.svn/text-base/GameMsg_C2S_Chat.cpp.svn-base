#include "GameMsg_C2S_Chat.h"
#include "../../datastructure/Macro_Define.h"

#include "ChatMsgDef.h"

GameMsg_C2S_Chat::GameMsg_C2S_Chat(void)
    : GameMsg_Base(MSG_C2S_CHAT)
{
    m_nSenderRoleID = 0;
    m_nReceiverRoleID = 0;      //如果是私聊，接受者的roleid
    m_nColor = CHAT_DEFAULT_TEXT_COLOR;
    m_nChannel = 0;             //频道
    m_bVoiceChat = false;
    m_bAutoChat = false;
}

GameMsg_C2S_Chat::~GameMsg_C2S_Chat(void)
{
}

bool GameMsg_C2S_Chat::doDecode(CParamPool &IOBuff)
{
    m_nSenderRoleID = IOBuff.GetUInt();
    m_nReceiverRoleID = IOBuff.GetUInt();
    m_nColor = IOBuff.GetUChar();
    m_nChannel = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strText);
    m_bVoiceChat = IOBuff.GetBool();
    m_WorldChatFlag = IOBuff.GetUChar();
    m_bAutoChat = IOBuff.GetBool();

    if (!m_bVoiceChat && m_strText.size() >= MAX_CHAT_TEXT_LEN)
    {
        m_strText.resize(MAX_CHAT_TEXT_LEN - 1);
    }

    return true;
}

GameMsg_C2S_NeedDanceGroupChatHistory::GameMsg_C2S_NeedDanceGroupChatHistory()
    :GameMsg_Base(MSG_C2S_GroupDanceChatHistory)
{}

GameMsg_C2S_NeedDanceGroupChatHistory::~GameMsg_C2S_NeedDanceGroupChatHistory()
{}
//end file


