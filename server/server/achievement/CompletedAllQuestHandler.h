#ifndef		__REQUIREMENTCOMPLETEDALLQUESTHANDLER_H__	 
#define		__REQUIREMENTCOMPLETEDALLQUESTHANDLER_H__	  


#include "IRequirementHandler.h"


class	CCompletedAllQuestHandler :
	public IRequirementHandler
{
public:
	CCompletedAllQuestHandler();
	~CCompletedAllQuestHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


