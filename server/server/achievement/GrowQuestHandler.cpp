#include "GrowQuestHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CGrowQuestHandler::CGrowQuestHandler( )
{
}

CGrowQuestHandler::~CGrowQuestHandler()
{
}

unsigned int CGrowQuestHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryGrowQuestCompletedCount;
}

IRequirementHandler *CGrowQuestHandler::Create()
{
	return new CGrowQuestHandler();
}

unsigned int CGrowQuestHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetRoleQuest()->GetCompletedQuestCountByType(Quest_Grow);
	}

	return 0;
}

// end of file

