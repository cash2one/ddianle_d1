#ifndef		__HISTORYBIGSPEAKERSUSEINSEVENDAYSCOUNTHANDLER_H__
#define		__HISTORYBIGSPEAKERSUSEINSEVENDAYSCOUNTHANDLER_H__ 

#include "IRequirementHandler.h"

class	CHistoryBigSpeakersUseInSevenDaysCountHandler:
	public IRequirementHandler
{
public:
	CHistoryBigSpeakersUseInSevenDaysCountHandler();
	~CHistoryBigSpeakersUseInSevenDaysCountHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


