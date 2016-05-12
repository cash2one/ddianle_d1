#include "HistoryTotalScoreHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryTotalScoreHandler::CHistoryTotalScoreHandler( )
{
}

CHistoryTotalScoreHandler::~CHistoryTotalScoreHandler()
{
}

unsigned int CHistoryTotalScoreHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryTotalScore;
}

IRequirementHandler *CHistoryTotalScoreHandler::Create()
{
	return new CHistoryTotalScoreHandler();
}

unsigned int CHistoryTotalScoreHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		//WriteLog("history score %d\n", pRoleAchievement->GetTotalScore());

		return pRoleAchievement->GetTotalScore();
	}

	return 0;
}




// end o file

