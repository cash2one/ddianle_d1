#ifndef		__HISTORYSMALLSPEAKERSUSEINSEVENDAYSCOUNTHANDLER_H__
#define		__HISTORYSMALLSPEAKERSUSEINSEVENDAYSCOUNTHANDLER_H__ 

#include "IRequirementHandler.h"

class	CHistorySmallSpeakersUseInSevenDaysCountHandler:
	public IRequirementHandler
{
public:
	CHistorySmallSpeakersUseInSevenDaysCountHandler();
	~CHistorySmallSpeakersUseInSevenDaysCountHandler();
public:
	unsigned int GetRequirementHandlerID()const;
	IRequirementHandler *Create();
	unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const;
};


#endif


//end file


