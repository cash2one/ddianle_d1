#ifndef __GAMEMSG_S2G_CHAT_H__
#define __GAMEMSG_S2G_CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_Base.h"
#include <string>

//客户端发往服务器的除私聊以外的聊天消息
class CParamPool;

class GameMsg_S2G_Chat : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_Chat);

public:
    GameMsg_S2G_Chat(const GameMsg_S2G_Chat &ref);
    GameMsg_S2G_Chat(unsigned char nPhrase, unsigned char nResult, unsigned char nChannel, unsigned char nColor, 
        unsigned int SenderRoleID, unsigned int nReceiverRoleID, const char *szSenderName, const char *szReceiverName, 
        const char *szText, unsigned char nClickSex, EChatExtraType nExtraType,bool bIsVIP, 
        unsigned short nVIPLevel, bool bVoiceChat, unsigned char nWorldChatFlag, bool bAutoChat = false);

public :
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned char   m_nPhase;           // 发送过程的状态
    unsigned char   m_nResult;          // 涉及跨服交互的聊天需填充
    unsigned char   m_nChannel;         // 频道
    unsigned char   m_nColor;
    unsigned int    m_nSenderRoleID;
    unsigned int    m_nReceiverRoleID;  // 如果是私聊，接收者的roleid
    std::string     m_strSenderName;
    std::string     m_strReceiverName;
    std::string     m_strText;          // 聊天的文字
    unsigned char   m_nClickSex;
    EChatExtraType  m_nExtraType;
    bool            m_bIsVIP;
    unsigned short  m_nVIPLevel;
    bool            m_bVoiceChat;
    unsigned char   m_nWorldChatFlag;
    bool            m_bAutoChat;        // 自动发言
};

class GameMsg_S2G_GroupDanceChatHistoryRecord : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_GroupDanceChatHistoryRecord);
    GameMsg_S2G_GroupDanceChatHistoryRecord(const GameMsg_S2G_GroupDanceChatHistoryRecord &ref);
    GameMsg_S2G_GroupDanceChatHistoryRecord(int nGroupId ,const char* szSenderName, unsigned int nSenderRoleID, unsigned char nChannel,
        unsigned char nColor, const char* szText, unsigned char nClickSex, EChatExtraType eExtraType, bool bIsVIP,
        unsigned short nVIPLevel, bool bVoiceChat, unsigned char cWorldChatFlag);
    
public:
    virtual bool doEncode(CParamPool &IOBuff);
    virtual bool doDecode(CParamPool &IOBuff);

public:
    int m_nGroupId;
    unsigned int m_nSenderRoleID;
    unsigned char m_nColor;
    unsigned char m_nChannel;
    std::string m_strSenderName;
    std::string m_strText;
    unsigned char m_nClickSex;
    int m_nExtraType;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    bool m_bVoiceChat;
    unsigned char m_WorldChatFlag;
};

class GameMsg_S2G_NeedGroupChatHistory : public GameMsg_Base
{
public:
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_NeedGroupChatHistory);
    virtual bool doEncode(CParamPool &IOBuff)
    { 
        IOBuff.AddInt(m_nGroupId);
        IOBuff.AddInt(m_nRoleId);
        return true; 
    }
    virtual bool doDecode(CParamPool &IOBuff)
    { 
        m_nGroupId = IOBuff.GetInt();
        m_nRoleId = IOBuff.GetInt();
        return true; 
    }

    int m_nGroupId;
    int m_nRoleId;
};

class GameMsg_S2G_Announce : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2G_Announce);

public:
    virtual bool doEncode( CParamPool &IOBuff );
    virtual bool doDecode( CParamPool &IOBuff );

public:
    CAnnounceBase m_announce;
};

#endif

//file end

