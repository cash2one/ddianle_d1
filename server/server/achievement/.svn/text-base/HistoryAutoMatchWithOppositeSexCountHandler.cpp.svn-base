#include "HistoryAutoMatchWithOppositeSexCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryAutoMatchWithOppositeSexCountHandler::CHistoryAutoMatchWithOppositeSexCountHandler()
{
}

CHistoryAutoMatchWithOppositeSexCountHandler::~CHistoryAutoMatchWithOppositeSexCountHandler()
{
}

unsigned int CHistoryAutoMatchWithOppositeSexCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryAutoMatchWithOppositeSexCount;
}

IRequirementHandler *CHistoryAutoMatchWithOppositeSexCountHandler::Create()
{
	return new CHistoryAutoMatchWithOppositeSexCountHandler();
}

unsigned int CHistoryAutoMatchWithOppositeSexCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nAutoMatchWithOppositeSexNum;
	}

	return 0;
}


// end of file

