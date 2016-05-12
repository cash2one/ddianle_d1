#include "HistoryTaikoComboCountHandler.h"

#include "../attribute/EntityAttributeComponent.h"
#include "../EntityNetComponent/EntityNetComponent.h"
#include "../item/EntityItemComponent.h"
#include "../chat/EntityChatComponent.h"
#include "../quest/EntityQuestComponent.h"

CHistoryTaikoComboCountHandler::CHistoryTaikoComboCountHandler( )
{
}

CHistoryTaikoComboCountHandler::~CHistoryTaikoComboCountHandler()
{
}

unsigned int CHistoryTaikoComboCountHandler::GetRequirementHandlerID()const
{
	return Achievement_HistoryTaikoComboCount;
}

IRequirementHandler *CHistoryTaikoComboCountHandler::Create()
{
	return new CHistoryTaikoComboCountHandler();
}

unsigned int CHistoryTaikoComboCountHandler::GetCurrentValue(const IEntityAchievementComponent	*pRoleAchievement)const
{
	unsigned int nCount = 0;
	if (pRoleAchievement != NULL)
	{
		map<int, map<int, CFightResult > >::const_iterator itMapResult = pRoleAchievement->m_mapFightResult.find(MusicMode_Taiko);

		if (itMapResult != pRoleAchievement->m_mapFightResult.end() ) {
			for ( map<int, CFightResult >::const_iterator itr = itMapResult->second.begin(); itr != itMapResult->second.end(); ++itr) {
				nCount += itr->second.m_nConPerfectNum;
			}
		}
	}
	//WriteLog("Achievement_HistoryTaikoComboCount VALUE  %d\n", nCount);

	return nCount;
}


// end of file

