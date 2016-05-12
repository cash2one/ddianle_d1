#include "CheckInHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../CheckIn/EntityCheckInComponent.h"

CCheckInHandler::CCheckInHandler()
{
}

CCheckInHandler::~CCheckInHandler()
{
}

unsigned int CCheckInHandler::GetRequirementHandlerID() const
{
	return Achievement_HistoryContinuousCheckInCount;
}

IRequirementHandler *CCheckInHandler::Create()
{
	return new CCheckInHandler();
}

unsigned int CCheckInHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement) const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetRoleCheck()->GetCurrentCheckCount();
	}

	return 0;
}


// end of file


