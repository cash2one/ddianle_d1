#ifndef __GAMEMSG_S2C_CHAT_H__
#define __GAMEMSG_S2C_CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
#include <string>

//服务器发往客户端的除私聊以外的聊天消息
class CParamPool;

class GameMsg_S2C_Chat : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_Chat);

public:
    GameMsg_S2C_Chat(const char* szSenderName, unsigned int m_nSenderRoleID, unsigned char nChannel, unsigned char nColor, const char* szText, 
        unsigned char nClickSex, EChatExtraType eExtraType, bool bIsVIP, unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag);

    static GameMsg_S2C_Chat* Create(const char* szSenderName, unsigned int nSenderRoleID, unsigned char nChannel, 
        const char* szText, unsigned char nColor, unsigned char nClickSex, EChatExtraType eExtraType, 
        bool bIsVIP, unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag);

public :
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::string     m_strSenderName;    //发送者名称
    unsigned int    m_nSenderRoleID;    //发送者id
    unsigned char   m_nChannel;         //频道
    unsigned char   m_nColor;
    string          m_strText;          //聊天的文字
    unsigned char   m_nClickSex;
    char            m_nExtraType;
    bool            m_bIsVIP;
    unsigned short  m_nVIPLevel;
    bool            m_bVoiceChat;       //是否语音聊天
    unsigned char   m_WorldChatFlag;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_SyncAutoChatInfo : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_SyncAutoChatInfo);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    float           m_fInterval;
    float           m_fMaxPauseTime;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_AutoChatResponse : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_AutoChatResponse);

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    bool            m_bChatSuccess;
};

#endif

//end file


