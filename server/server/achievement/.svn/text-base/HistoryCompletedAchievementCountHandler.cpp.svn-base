#include "HistoryCompletedAchievementCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CCompletedAchievementCountHandler::CCompletedAchievementCountHandler()
{
}

CCompletedAchievementCountHandler::~CCompletedAchievementCountHandler()
{
}

unsigned int CCompletedAchievementCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryCompletedAchievementCount;
}

IRequirementHandler *CCompletedAchievementCountHandler::Create()
{
	return new CCompletedAchievementCountHandler();
}

unsigned int CCompletedAchievementCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetHistoryAchievementCount();
	}

	return 0;
}


// end of file

