#include "HistorySendFlowersCountInSevenDays.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistorySendFlowersCountHandler::CHistorySendFlowersCountHandler()
{
}

CHistorySendFlowersCountHandler::~CHistorySendFlowersCountHandler()
{
}

unsigned int CHistorySendFlowersCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistorySendFlowersCountInSevenDays;
}

IRequirementHandler *CHistorySendFlowersCountHandler::Create()
{
	return new CHistorySendFlowersCountHandler();
}

unsigned int CHistorySendFlowersCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetSendFlowersCountInSevenDays();
	}

	return 0;
}


// end of file

