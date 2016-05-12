#include "HistoryBigSpeakersUseInSevenDaysCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryBigSpeakersUseInSevenDaysCountHandler::CHistoryBigSpeakersUseInSevenDaysCountHandler()
{
}

CHistoryBigSpeakersUseInSevenDaysCountHandler::~CHistoryBigSpeakersUseInSevenDaysCountHandler()
{
}

unsigned int CHistoryBigSpeakersUseInSevenDaysCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryBigSpeakersUseInSevenDaysCount;
}

IRequirementHandler *CHistoryBigSpeakersUseInSevenDaysCountHandler::Create()
{
	return new CHistoryBigSpeakersUseInSevenDaysCountHandler();
}

unsigned int CHistoryBigSpeakersUseInSevenDaysCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetBigSpeakersUseCountInSevenDays();
	}

	return 0;
}



// end of file

