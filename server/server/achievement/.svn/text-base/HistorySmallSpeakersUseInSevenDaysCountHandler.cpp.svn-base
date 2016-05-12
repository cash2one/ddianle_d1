#include "HistorySmallSpeakersUseInSevenDaysCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistorySmallSpeakersUseInSevenDaysCountHandler::CHistorySmallSpeakersUseInSevenDaysCountHandler()
{
}

CHistorySmallSpeakersUseInSevenDaysCountHandler::~CHistorySmallSpeakersUseInSevenDaysCountHandler()
{
}

unsigned int CHistorySmallSpeakersUseInSevenDaysCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistorySmallSpeakersUseInSevenDaysCount;
}

IRequirementHandler *CHistorySmallSpeakersUseInSevenDaysCountHandler::Create()
{
	return new CHistorySmallSpeakersUseInSevenDaysCountHandler();
}

unsigned int CHistorySmallSpeakersUseInSevenDaysCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetSmallSpeakersUseCountInSevenDays();
	}

	return 0;
}


// end of file

