#ifndef		__HISTORYHOTVALUEHANDLER_H__
#define		__HISTORYHOTVALUEHANDLER_H__ 

#include "IRequirementHandler.h"

class	CHistoryHotValueHandler :
	public IRequirementHandler
{
public:
	CHistoryHotValueHandler();
	~CHistoryHotValueHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


