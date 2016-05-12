#include "IChat.h"
#include "../chat/EntityChatComponent.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../friend/EntityFriendComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../achievement/EntityAchievementComponent.h"

IChat::~IChat()
{
	m_pRoleChat = NULL;
}


bool IChat::CanTalk()
{
	return true;
}

CEntityAttributeComponent *IChat::GetRoleAttr()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleAttr();
	}

	return NULL;
}

CEntityFriendComponent *IChat::GetRoleFriend()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleFriend();
	}

	return NULL;
}

CEntityNetComponent *IChat::GetRoleNet()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleNet();
	}

	return NULL;
}

CRoomComponent* IChat::GetRoleRoom()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleRoom();
	}

	return NULL;
}

CEntityItemComponent * IChat::GetRoleItem()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleItem();
	}

	return NULL;
}


CEntityDanceGroupComponent * IChat::GetDanceGroup()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleDanceGroup();
	}
	return NULL;
}

CEntityCoupleComponent * IChat::GetRoleCouple()
{
	if (m_pRoleChat != NULL)
	{
		return m_pRoleChat->GetRoleCouple();
	}

	return NULL;
}

CEntityVIPComponent * IChat::GetRoleVIP()
{
	if (m_pRoleChat != NULL) {
		return m_pRoleChat->GetRoleVIP();
	}

	return NULL;
}

CEntityPhotoRoomComponent * IChat::GetRolePhotoRoom()
{
	if (m_pRoleChat != NULL) {
		return m_pRoleChat->GetRolePhotoRoom();
	}

	return NULL;
}

CAmuseRoomComponent* IChat::GetRoleAmuseRoom()
{
	if (m_pRoleChat != NULL) {
		return m_pRoleChat->GetRoleAmuseRoom();
	}

	return NULL;
}

CRankComponent* IChat::GetRankComponent()
{
    if (m_pRoleChat != NULL)
    {
        return m_pRoleChat->GetRankComponent();
    }

    return NULL;
}

CeremonyRoom* IChat::GetCeremonyRoom()
{
    if (m_pRoleChat != NULL)
    {
        return m_pRoleChat->GetCeremony();
    }

    return NULL;
}

