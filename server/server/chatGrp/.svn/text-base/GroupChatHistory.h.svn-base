/*
* DanceGroupFairyLandProcessor.h
* Description: GroupChatHistory
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: lht ∞Ù∞Ùﬂ’
* Create time: 2015-3-1 10:11:03
*/

#ifndef	__GROUPCHATHISTORY_H__
#define __GROUPCHATHISTORY_H__
#include "IGroupChat.h"
#include "../Macros/Macros.h"
#include "../../datastructure/Macro_Define.h"
#include "../chat/GameMsg_S2G_Chat.h"
#include "../chat/GameMsg_G2S_Chat.h"
#include <map>
#include <list>

class CChatHistory
{
public:
    CChatHistory();
    ~CChatHistory(){}
    CChatHistory(const CChatHistory& rOther);
    CChatHistory(const GameMsg_S2G_GroupDanceChatHistoryRecord& rRecord);

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

class CGroupChatHistory
{
public:
    CLASS_INSTANCE_FUNC(CGroupChatHistory);
    virtual ~CGroupChatHistory();
    void RecordHistory(int nGroupId, CChatHistory& rRecord);

    void DataCopyTo(GameMsg_S2G_GroupDanceChatHistoryRecord& data, CChatHistory & rRecord);
    void GetHistory(int nGroupId, std::list<CChatHistory> &rListHistory);
protected:
    CGroupChatHistory();

private:
    std::map<int, std::list<CChatHistory> > m_mapHistory;
};


#endif