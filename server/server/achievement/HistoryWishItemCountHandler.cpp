#include "HistoryWishItemCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryWishItemCountHandler::CHistoryWishItemCountHandler( )
{
}

CHistoryWishItemCountHandler::~CHistoryWishItemCountHandler()
{
}

unsigned int CHistoryWishItemCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryWishItemCount;
}

IRequirementHandler *CHistoryWishItemCountHandler::Create()
{
	return new CHistoryWishItemCountHandler();
}

unsigned int CHistoryWishItemCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nHistoryWishItemNum;
	}

	return 0;
}


// end of file

