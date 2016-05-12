#ifndef		__ACHIEVEMENTHISTORYBADGECOUNT_H__	 
#define		__ACHIEVEMENTHISTORYBADGECOUNT_H__	  


#include "IRequirementHandler.h"

class	CHistoryBadgeHandler :
	public IRequirementHandler
{
public:
	CHistoryBadgeHandler();
	~CHistoryBadgeHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


