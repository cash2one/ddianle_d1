#ifndef		__CHECKINHANDLER_H__	 
#define		__CHECKINHANDLER_H__	


#include "IRequirementHandler.h"


class	CCheckInHandler :
	public IRequirementHandler
{
public:
	CCheckInHandler();
	~CCheckInHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


