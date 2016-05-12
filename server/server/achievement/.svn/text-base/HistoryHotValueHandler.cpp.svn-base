#include "HistoryHotValueHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryHotValueHandler::CHistoryHotValueHandler( )
{
}

CHistoryHotValueHandler::~CHistoryHotValueHandler()
{
}

unsigned int CHistoryHotValueHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryHotValue;
}

IRequirementHandler *CHistoryHotValueHandler::Create()
{
	return new CHistoryHotValueHandler();
}

unsigned int CHistoryHotValueHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetRoleAttr()->GetHot();
	}

	return 0;
}


// end of file

