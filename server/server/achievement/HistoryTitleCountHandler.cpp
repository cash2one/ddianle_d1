#include "HistoryTitleCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryTitleCountHandler::CHistoryTitleCountHandler( )
{
}

CHistoryTitleCountHandler::~CHistoryTitleCountHandler()
{
}

unsigned int CHistoryTitleCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryTitleCount;
}

IRequirementHandler *CHistoryTitleCountHandler::Create()
{
	return new CHistoryTitleCountHandler();
}

unsigned int CHistoryTitleCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetHistoryTitleCount();
	}

	return 0;
}


// end of file

