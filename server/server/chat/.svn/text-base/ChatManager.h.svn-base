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

    // sysChat - 红字公告
    // adminChat - 走马灯公告
    void SendAnnounce(bool bSysChat, bool bAdminChat, std::string &strContent);

private:
    void RegChatManagerNetMsgMap();

    bool CheckConfig() const;

    void OnGroupChat(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    // 自动发言间隔时间
    CLASS_PROPERTY_READONLY(unsigned int, m_nAutoChatInterval, AutoChatInterval);
    // 自动发言最长暂停时间
    CLASS_PROPERTY_READONLY(unsigned int, m_nAutoChatMaxPauseTime, AutoChatMaxPauseTime);
};

#endif


