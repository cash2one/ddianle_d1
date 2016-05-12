#ifndef		__HISTORYWISHITEMCOUNTHANDLER_H__
#define		__HISTORYWISHITEMCOUNTHANDLER_H__ 

#include "IRequirementHandler.h"

class	CHistoryWishItemCountHandler :
	public IRequirementHandler
{
public:
	CHistoryWishItemCountHandler();
	~CHistoryWishItemCountHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


