#ifndef __CHAT_H__
#define __CHAT_H__

#include "../../socket/GameMsg_Base.h"
#include <stdlib.h>

class CEntityChatComponent;
class CEntityAttributeComponent;
class CEntityNetComponent;
class CEntityFriendComponent;
class CRoomComponent;
class CEntityItemComponent;
class CEntityAchievementComponent;
class CEntityDanceGroupComponent;
class CEntityCoupleComponent;
class CEntityVIPComponent;
class CEntityPhotoRoomComponent;
class CAmuseRoomComponent;
class GameMsg_Base;
class CRankComponent;
class CeremonyRoom;

class IChat
{
public:
	IChat(CEntityChatComponent *pRoleChat = NULL):m_pRoleChat(pRoleChat)
	{
	}
	virtual ~IChat();
	virtual bool OnChat(GameMsg_Base *pMsg) = 0; //on net message
	virtual void OnGroupChatMessage(GameMsg_Base *pMsg) = 0; // on group message
	virtual bool CanTalk();
public:
	CEntityAttributeComponent *GetRoleAttr();
	CEntityFriendComponent *GetRoleFriend();
	CEntityNetComponent *GetRoleNet();
	CEntityItemComponent *GetRoleItem();
	CRoomComponent* GetRoleRoom();
	CEntityDanceGroupComponent * GetDanceGroup();
	CEntityCoupleComponent * GetRoleCouple();
	CEntityVIPComponent * GetRoleVIP();
	CEntityPhotoRoomComponent * GetRolePhotoRoom();
	CAmuseRoomComponent* GetRoleAmuseRoom();
    CRankComponent* GetRankComponent();
    CeremonyRoom* GetCeremonyRoom();
public:
	CEntityChatComponent *m_pRoleChat;
};

#endif


//end file


