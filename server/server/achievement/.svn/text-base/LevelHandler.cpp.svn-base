#include "LevelHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CLevelHandler::CLevelHandler( )
{
}

CLevelHandler::~CLevelHandler()
{
}

unsigned int CLevelHandler::GetRequirementHandlerID()const
{
	return Achievement_LevelHandler;
}

IRequirementHandler *CLevelHandler::Create()
{
	return new CLevelHandler();
}

unsigned int CLevelHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	if (pRoleAchievement != NULL)
	{
		return pRoleAchievement->GetRoleAttr()->GetLevel();
	}

	return 0;
}



// end of file

