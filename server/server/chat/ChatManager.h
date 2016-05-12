#ifndef __CHATMANAGER_H__
#define __CHATMANAGER_H__

#include "../../socket/GameMsg_Processor.h"

#include "../Macros/Macros.h"

#include <string>

class IChat;
class CEntityChatComponent;

class CChatManager : public GameMsg_Processor
{
protected:
    CChatManager();
    virtual ~CChatManager();

public:
    CLASS_INSTANCE_FUNC(CChatManager);
    bool Initialize();

    void SetAutoChatInfos(unsigned int nInterval, unsigned int nMaxPauseTime);

    IChat *CreateChatChannelBy(unsigned char nChannel, CEntityChatComponent *pRoleChat);

    void AdminChat(const char *szFormat, ...);

    // sysChat - ���ֹ���
    // adminChat - ����ƹ���
    void SendAnnounce(bool bSysChat, bool bAdminChat, std::string &strContent);

private:
    void RegChatManagerNetMsgMap();

    bool CheckConfig() const;

    void OnGroupChat(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // �Զ����Լ��ʱ��
    CLASS_PROPERTY_READONLY(unsigned int, m_nAutoChatInterval, AutoChatInterval);
    // �Զ��������ͣʱ��
    CLASS_PROPERTY_READONLY(unsigned int, m_nAutoChatMaxPauseTime, AutoChatMaxPauseTime);
};

#endif


