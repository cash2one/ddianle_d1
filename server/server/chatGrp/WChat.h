/********************************************************************
created:	 2011/01/10
author:		 meijiuhua
filename: 	 server\group\grpchat\wchat.cpp
purpose:	 group聊天模块
*********************************************************************/
#ifndef __WCHAT_H__
#define __WCHAT_H__

#include "../../socket/GameMsg_Processor.h"
#include "../Macros/Macros.h"
#include <queue>
#include <string>

class IGroupChat;
class GameMsg_S2G_Chat;
class CAnnounceBase;
class CChatHistory;

class CWChatMgr : public GameMsg_Processor
{
protected:
    CWChatMgr();
    virtual ~CWChatMgr();

public:
    CLASS_INSTANCE_FUNC(CWChatMgr);

    void Initialize();

    void SetAutoChatSendInterval(unsigned int nInterval);

    void OnUpdate(int nTimeElapse);

    void SendDanceGroupAnnouncement(unsigned int nGroupID, const std::string &rText);
    void SendAllAnnouncement(CAnnounceBase &ann);
protected:
    void RegisterMessage();
    void RegisterMessageProcessor();

    void OnNetMessage(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnSendAnnounce(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnRecordChatHistory(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnNeedChatHistory(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnChat(GameMsg_S2G_Chat *pChatMsg);
    IGroupChat *CreateChatChannelBy(unsigned char nChannel);

    bool PreprocessMsg(GameMsg_S2G_Chat *pChatMsg);

    bool CanSendAutoChatMsg();

private:
    // 自动发言发送间隔时间(毫秒)
    CLASS_PROPERTY_READONLY(unsigned int, m_nAutoChatSendInterval, AutoChatSendInterval);
    unsigned int    m_nLastAutoChatSendTime;
    std::queue<GameMsg_S2G_Chat *>  m_queueAutoChatMsg;
};

#endif

