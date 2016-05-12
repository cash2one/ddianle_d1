#ifndef		__HISTORYLEVELHANDLER_H__ 
#define		__HISTORYLEVELHANDLER_H__ 

#include "IRequirementHandler.h"

class	CLevelHandler :
		public IRequirementHandler
{
public:
	CLevelHandler();
	~CLevelHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


