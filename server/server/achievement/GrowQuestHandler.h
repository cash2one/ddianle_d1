#ifndef		 __ACHIEVEMENTGROWQUEST_HANDLER_H__
#define		 __ACHIEVEMENTGROWQUEST_HANDLER_H__


#include "IRequirementHandler.h"


class	CGrowQuestHandler :
	public IRequirementHandler
{
public:
	CGrowQuestHandler();
	~CGrowQuestHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


