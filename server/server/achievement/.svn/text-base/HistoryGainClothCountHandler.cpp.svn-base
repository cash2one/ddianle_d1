#include "HistoryGainClothCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryGainClothCountHandler::CHistoryGainClothCountHandler( )
{
}

CHistoryGainClothCountHandler::~CHistoryGainClothCountHandler()
{
}

unsigned int CHistoryGainClothCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryAllClothCount;
}

IRequirementHandler *CHistoryGainClothCountHandler::Create()
{
	return new CHistoryGainClothCountHandler();
}

unsigned int CHistoryGainClothCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nHistoryClothNum;
	}

	return 0;
}


// end of file












