#ifndef __GAMEMSG_C2S_CHAT_H__
#define __GAMEMSG_C2S_CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include <string>

//�ͻ��˷����������ĳ�˽�������������Ϣ
class CParamPool;

class GameMsg_C2S_Chat : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_Chat);

public :
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int    m_nSenderRoleID;    // �����ߵ�ID��˽�ı�����
    unsigned int    m_nReceiverRoleID;  // �����˽�ģ������ߵ�roleid
    unsigned char   m_nColor;
    unsigned char   m_nChannel;         // Ƶ��
    std::string     m_strText;          // ���������
    bool            m_bVoiceChat;       // �Ƿ���������
    unsigned char   m_WorldChatFlag;    // ���������ʾ��������ʾ��ͨ���ֺ��Ų�����
    bool            m_bAutoChat;        // �Ƿ��Զ�����
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


