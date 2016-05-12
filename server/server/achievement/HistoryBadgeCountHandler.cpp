#include "HistoryBadgeCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryBadgeHandler::CHistoryBadgeHandler()
{
}

CHistoryBadgeHandler::~CHistoryBadgeHandler()
{
}

unsigned int CHistoryBadgeHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryBadgeCount;
}

IRequirementHandler *CHistoryBadgeHandler::Create()
{
	return new CHistoryBadgeHandler();
}

unsigned int CHistoryBadgeHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nHistoryBadageNum;
	}

	return 0;
}


// end of file

