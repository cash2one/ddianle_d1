#ifndef		__HISTORYSENDFLOWERSCOUNTINSEVENDAYSHANDLER_H__
#define		__HISTORYSENDFLOWERSCOUNTINSEVENDAYSHANDLER_H__ 

#include "IRequirementHandler.h"

class	CHistorySendFlowersCountHandler:
	public IRequirementHandler
{
public:
	CHistorySendFlowersCountHandler();
	~CHistorySendFlowersCountHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


