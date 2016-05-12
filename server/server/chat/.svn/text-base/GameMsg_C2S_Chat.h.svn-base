#ifndef __GAMEMSG_C2S_CHAT_H__
#define __GAMEMSG_C2S_CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

//客户端发往服务器的除私聊以外的聊天消息
class CParamPool;

class GameMsg_C2S_Chat : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_Chat);

public :
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nSenderRoleID;    // 发送者的ID，私聊必须填
    unsigned int    m_nReceiverRoleID;  // 如果是私聊，接收者的roleid
    unsigned char   m_nColor;
    unsigned char   m_nChannel;         // 频道
    std::string     m_strText;          // 聊天的文字
    bool            m_bVoiceChat;       // 是否语音聊天
    unsigned char   m_WorldChatFlag;    // 喇叭聊天标示，用来标示普通文字和炫彩字体
    bool            m_bAutoChat;        // 是否自动发言
};

class GameMsg_C2S_NeedDanceGroupChatHistory : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_NeedDanceGroupChatHistory);

public:
    virtual bool doDecode(CParamPool &IOBuff)
    {
        return true;
    }
};


#endif

//end file


