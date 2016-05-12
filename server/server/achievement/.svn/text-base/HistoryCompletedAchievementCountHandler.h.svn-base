#ifndef			 __ACHIEVEMENTHISTORYCOMPLETEDACHIEVEMENTCOUNT_H__
#define			__ACHIEVEMENTHISTORYCOMPLETEDACHIEVEMENTCOUNT_H__ 

#include "IRequirementHandler.h"


class	CCompletedAchievementCountHandler :
	public IRequirementHandler
{
public:
	CCompletedAchievementCountHandler();
	~CCompletedAchievementCountHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif




