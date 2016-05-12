#include "ContinuousWinIn4PersonsHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CContinuousWinIn4PersonsHandler::CContinuousWinIn4PersonsHandler()
{
}

CContinuousWinIn4PersonsHandler::~CContinuousWinIn4PersonsHandler()
{
}

unsigned int CContinuousWinIn4PersonsHandler::GetRequirementHandlerID()const
{
	return Achievement_WinIn4Persons;
}

IRequirementHandler *CContinuousWinIn4PersonsHandler::Create()
{
	return new CContinuousWinIn4PersonsHandler();
}

unsigned int CContinuousWinIn4PersonsHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nNumWinIn4Persons;
	}

	return 0;
}

// end of file

