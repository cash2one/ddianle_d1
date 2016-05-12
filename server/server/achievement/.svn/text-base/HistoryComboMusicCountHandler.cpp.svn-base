#include "HistoryComboMusicCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryComboMusicCountHandler::CHistoryComboMusicCountHandler( )
{
}

CHistoryComboMusicCountHandler::~CHistoryComboMusicCountHandler()
{
}

unsigned int CHistoryComboMusicCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryComboMusicCount;
}

IRequirementHandler *CHistoryComboMusicCountHandler::Create()
{
	return new CHistoryComboMusicCountHandler();
}

unsigned int CHistoryComboMusicCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->m_nHistoryTaikoComboMusicNum;
	}

	return 0;
}


// end of file

