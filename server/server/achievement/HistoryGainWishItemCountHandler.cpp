#include "HistoryGainWishItemCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryGainWishItemCountHandler::CHistoryGainWishItemCountHandler( )
{
}

CHistoryGainWishItemCountHandler::~CHistoryGainWishItemCountHandler()
{
}

unsigned int CHistoryGainWishItemCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryGainWishItemCount;
}

IRequirementHandler *CHistoryGainWishItemCountHandler::Create()
{
	return new CHistoryGainWishItemCountHandler();
}

unsigned int CHistoryGainWishItemCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nGainWishItemNum;
	}

	return 0;
}


// end of file

