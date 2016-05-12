#include "ContinuousWinBetween2To4PersonsHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CContinuousWinBetween2To4Handler::CContinuousWinBetween2To4Handler( )
{
}

CContinuousWinBetween2To4Handler::~CContinuousWinBetween2To4Handler()
{
}

unsigned int CContinuousWinBetween2To4Handler::GetRequirementHandlerID()const
{
	return Achievement_WinBetween2To4Persons;
}

IRequirementHandler *CContinuousWinBetween2To4Handler::Create()
{
	return new CContinuousWinBetween2To4Handler();
}

unsigned int CContinuousWinBetween2To4Handler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement) const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nNumWinBetween2To4Persons;
	}

	return 0;
}


// end of file

