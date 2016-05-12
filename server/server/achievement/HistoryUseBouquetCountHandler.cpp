#include "HistoryUseBouquetCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryUseBouquetCountHandler::CHistoryUseBouquetCountHandler( )
{
}

CHistoryUseBouquetCountHandler::~CHistoryUseBouquetCountHandler()
{
}

unsigned int CHistoryUseBouquetCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryUseBouquetCount;
}

IRequirementHandler *CHistoryUseBouquetCountHandler::Create()
{
	return new CHistoryUseBouquetCountHandler();
}

unsigned int CHistoryUseBouquetCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nHistoryUseBouquetNum;
	}

	return 0;
}


// end of file

