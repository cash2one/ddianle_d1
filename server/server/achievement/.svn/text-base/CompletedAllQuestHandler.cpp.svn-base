#include "CompletedAllQuestHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CCompletedAllQuestHandler::CCompletedAllQuestHandler()
{
}

CCompletedAllQuestHandler::~CCompletedAllQuestHandler()
{
}

unsigned int CCompletedAllQuestHandler::GetRequirementHandlerID() const
{
	return Achievement_HistoryAllQuestCompletedCount;
}

IRequirementHandler *CCompletedAllQuestHandler::Create()
{
	return new CCompletedAllQuestHandler();
}

unsigned int CCompletedAllQuestHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement) const
{
	if (pRoleAchievement != NULL)
	{
		//WriteLog("Has completed quest %d\n", pRoleAchievement->GetRoleQuest()->GetCompletedAllQuestCount());
		return pRoleAchievement->GetRoleQuest()->GetCompletedAllQuestCount();
	}

	return 0;
}


// end of file

