#ifndef		__IREQUIRMENTHANDLER_H__
#define		__IREQUIRMENTHANDLER_H__

#include <stdio.h>

#include "IEntityAchivementComponent.h"
#include <list>

class IRequirementHandler
{
public:
	IRequirementHandler();
	virtual ~IRequirementHandler();
public:
	virtual unsigned int GetRequirementHandlerID() const= 0;
	virtual IRequirementHandler *Create() = 0;
	virtual unsigned int GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement) const = 0;
	bool CanMeetRequirement(unsigned int nAchievementID, IEntityAchievementComponent	*pRoleAchievement)const;

	//void Change();
	//void Notify(IEntityAchievementComponent *pRoleAchievment);
//public:
	//std::list<unsigned int> m_listAchievement;
};



#endif



//end file


