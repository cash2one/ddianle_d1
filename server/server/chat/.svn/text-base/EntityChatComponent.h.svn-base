/*
*2011-11-9
*by wangshijun
*把entity组件化。
*/

#ifndef __CHATCOMPONENT_COMPONENT_H__
#define __CHATCOMPONENT_COMPONENT_H__

#include "../logic/RoleComponentImpl.h"
#include "../logic/EntityComponent.h"

class IChat;
class CEntityAttributeComponent;
class CEntityFriendComponent;
class CEntityNetComponent;
class CEntityItemComponent;
class CRoomComponent;
class CEntityAchievementComponent;
class CEntityQuestComponent;
class CEntityDanceGroupComponent;
class CEntityVIPComponent;
class CEntityActivityComponent;
class CEntityPhotoRoomComponent;
class CBetComponent;
class CAmuseRoomComponent;
class CEntityQuestNewComponent;
class CEntityDungeonComponent;
class CRankComponent;
class CEntityCheckInComponent;
class CeremonyRoom;
class CEntityMedalComponent;

class CEntityChatComponent : public CEntityComponent
{
public:
    CEntityChatComponent(void);
    ~CEntityChatComponent(void);

public:
    virtual void Start();
    void OnEvent(CComponentEvent &refEvent);
    virtual void OnLogin();
    virtual const char* SerializeName(){ return "Chat"; }

public:
    void SysChat(const char *szFormat, ...);

    void SendPlayerDanceGroupAnnouncement( const std::string &rText );

    void SetCurFontColor(int nColor);
    int GetCurFontColor();

    void SetLobbyChatTime(time_t tTime);
    time_t LastLobbyChatTime();

public:
    CEntityNetComponent* GetRoleNet() { return m_pRoleNet; }
    CEntityFriendComponent* GetRoleFriend() { return m_pRoleFriend; }
    CEntityAttributeComponent* GetRoleAttr() { return m_pRoleAttr; }
    CEntityItemComponent* GetRoleItem() { return m_pRoleItem; }
    CEntityQuestComponent* GetRoleQuest() { return m_pRoleQuest; }
    CRoomComponent* GetRoleRoom() { return m_pRoleRoom; }
    CEntityDanceGroupComponent* GetRoleDanceGroup() { return m_pRoleDanceGroup; }
    CEntityCoupleComponent* GetRoleCouple() { return m_pRoleCouple; }
    CEntityVIPComponent* GetRoleVIP() { return m_pRoleVIP; }
    CEntityActivityComponent* GetRoleActivity() { return m_pRoleActivity; }
    CEntityPhotoRoomComponent* GetRolePhotoRoom() { return m_pRolePhotoRoom; }
    CBetComponent* GetRoleBet() { return m_pRoleBet; }
    CAmuseRoomComponent* GetRoleAmuseRoom() { return m_pRoleAmuseRoom; }
    CEntityQuestNewComponent* GetRoleQuestNew() { return m_pRoleQuestNew; }
    CEntityDungeonComponent* GetRoleDungeon() { return m_pRoleDungeon; }
    CRankComponent* GetRankComponent() { return m_pRankComponent; }
    CEntityCheckInComponent* GetRoleCheckIn() { return m_pRoleCheckIn; }
    CeremonyRoom* GetCeremony();
    CEntityMedalComponent* GetRoleMedal() { return m_pRoleMedal; }

protected:
    void RegComponentNetMsgMap();

    void OnChat(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGroupChatMessage(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGroupChatNeedHistoryMsg(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGroupChatRecvHistoryMsg(GameMsg_Base &msg, CSlotPeer &slotPeer);

private:
    void _LogChat(unsigned int nRoleID, eChatChannel eChannel, const char* szText, bool bVoiceChat);

    bool CanAutoChat();

    void SyncAutoChatInfo();

private:
    int             m_nFontColor;
    time_t          m_tLastLobbyChatTime;
    unsigned int    m_nLastAutoChatTime;    // 上次自动发言时间

private:
    CEntityNetComponent         *m_pRoleNet;
    CEntityFriendComponent      *m_pRoleFriend;
    CEntityAttributeComponent   *m_pRoleAttr;
    CEntityItemComponent        *m_pRoleItem;
    CEntityQuestComponent       *m_pRoleQuest;
    CRoomComponent              *m_pRoleRoom;
    CEntityDanceGroupComponent  *m_pRoleDanceGroup;
    CEntityCoupleComponent      *m_pRoleCouple;
    CEntityVIPComponent         *m_pRoleVIP;
    CEntityActivityComponent    *m_pRoleActivity;
    CEntityPhotoRoomComponent   *m_pRolePhotoRoom;
    CBetComponent               *m_pRoleBet;
    CAmuseRoomComponent         *m_pRoleAmuseRoom;
    CEntityQuestNewComponent    *m_pRoleQuestNew;
    CEntityDungeonComponent     *m_pRoleDungeon;
    CRankComponent              *m_pRankComponent;
    CEntityCheckInComponent     *m_pRoleCheckIn;
    CEntityMedalComponent       *m_pRoleMedal;
};

typedef CRoleComponentImpl<CEntityChatComponent,CGetRoleProcessorObj,CGetRoleProcessorObj> CRoleChat;

#endif

//end file


