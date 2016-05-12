#ifndef		__WINBETWEEN2TO4PERSONSHANDLER_H__
#define		__WINBETWEEN2TO4PERSONSHANDLER_H__ 

#include "IRequirementHandler.h"

class	CContinuousWinBetween2To4Handler :
	public IRequirementHandler
{
public:
	CContinuousWinBetween2To4Handler();
	~CContinuousWinBetween2To4Handler();
public:
	unsigned int GetRequirementHandlerID() const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement) const;
};


#endif


//end file


