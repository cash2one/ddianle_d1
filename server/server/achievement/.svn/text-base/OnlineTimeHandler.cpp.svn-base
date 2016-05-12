#include "OnlineTimeHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

COnlineTimeHandler::COnlineTimeHandler( )
{
}

COnlineTimeHandler::~COnlineTimeHandler()
{
}

unsigned int COnlineTimeHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryTimeOnline;
}

IRequirementHandler *COnlineTimeHandler::Create()
{
	return new COnlineTimeHandler();
}

unsigned int COnlineTimeHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return (unsigned int)pRoleAchievement->GetRoleAttr()->GetOnlineTime();
	}

	return 0;
}



