#include "IRequirementHandler.h"
#include "../../datastructure/datainfor/ConfigManager.h"


IRequirementHandler::IRequirementHandler()
{
}

IRequirementHandler::~IRequirementHandler()
{
}

bool IRequirementHandler::CanMeetRequirement(unsigned int nAchievementID, IEntityAchievementComponent	*pRoleAchievement)const
{
	if (nAchievementID > 0 && pRoleAchievement != NULL)
	{
		AchievementConfig *pAchievementDoc =  ConfigManager::Instance().GetAchievementManager().GetByID((unsigned short)nAchievementID);
		if (pAchievementDoc != NULL && pRoleAchievement != NULL)
		{
			return GetCurrentValue(pRoleAchievement) >= pAchievementDoc->m_nCount;
		}
	}

	return false;
}

//void IRequirementHandler::Register(unsigned int nAchievementID)
//{
//}




//end file


