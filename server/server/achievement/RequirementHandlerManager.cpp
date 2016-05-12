#include "RequirementHandlerManager.h"

#include "CompletedAllQuestHandler.h"
#include "ContinuousWinBetween2To4PersonsHandler.h"
#include "ContinuousWinIn4PersonsHandler.h"
#include "GrowQuestHandler.h"
#include "HistoryAutoMatchWithOppositeSexCountHandler.h"
#include "HistoryBadgeCountHandler.h"
#include "HistoryBigSpeakersUseInSevenDaysCountHandler.h"
#include "HistoryComboMusicCountHandler.h"
#include "HistoryCompletedAchievementCountHandler.h"
#include "HistoryGainClothCountHandler.h"
#include "HistoryGainWishItemCountHandler.h"
#include "HistoryHotValueHandler.h"
#include "HistorySendFlowersCountInSevenDays.h"
#include "HistorySmallSpeakersUseInSevenDaysCountHandler.h"
#include "HistoryTaikoComboCountHandler.h"
#include "HistoryTitleCountHandler.h"
#include "HistoryTotalScoreHandler.h"
#include "HistoryUseBouquetCountHandler.h"
#include "HistoryWishItemCountHandler.h"
#include "LevelHandler.h"
#include "OnlineTimeHandler.h"
#include "CheckInHandler.h"


CRequirementHandlerManager& CRequirementHandlerManager::Instance()
{
	static CRequirementHandlerManager stcRHMgr;
	return	stcRHMgr;
}

CRequirementHandlerManager::CRequirementHandlerManager()
{
	m_ppRequirementHandler = new IRequirementHandler *[MAX_ACHIEVEMENT_HANDLER_COUNT];

	for (int i = 0; i < MAX_ACHIEVEMENT_HANDLER_COUNT; i++)
	{
		m_ppRequirementHandler[i] = NULL;
	}
}

CRequirementHandlerManager::~CRequirementHandlerManager()
{
	for ( int i = 0; i < MAX_ACHIEVEMENT_HANDLER_COUNT; i++)
	{
		if (m_ppRequirementHandler[i] != NULL)
		{
			delete m_ppRequirementHandler[i];
			m_ppRequirementHandler[i] = NULL;
		}
	}
	delete [] m_ppRequirementHandler;
}

void CRequirementHandlerManager::Init()
{

	Add(new CLevelHandler);										// 1
	Add(new CGrowQuestHandler);									// 2
	Add(new CCompletedAllQuestHandler);							// 3
	Add(new COnlineTimeHandler);								// 4
	Add(new CHistoryGainClothCountHandler);						// 5
	Add(new CHistoryTitleCountHandler);							// 6
	Add(new CHistoryBadgeHandler);								// 7
	Add(new CCompletedAchievementCountHandler);					// 8
	Add(new CHistoryTotalScoreHandler);							// 9
	Add(new CContinuousWinBetween2To4Handler);					// 10
	Add(new CContinuousWinIn4PersonsHandler );					// 11
	Add(new CHistoryTaikoComboCountHandler);					// 12
	Add(new CHistoryComboMusicCountHandler);					// 13
	Add(new CHistoryHotValueHandler);							// 14
	Add(new CHistoryBigSpeakersUseInSevenDaysCountHandler);		// 15
	Add(new CHistoryWishItemCountHandler);						// 16
	Add(new CHistoryGainWishItemCountHandler);					// 17
	Add(new CHistorySendFlowersCountHandler);					// 18
	Add(new CHistoryUseBouquetCountHandler);					// 19
	Add(new CCheckInHandler);									// 20
	Add(new CHistoryAutoMatchWithOppositeSexCountHandler);		// 21
	Add(new CHistorySmallSpeakersUseInSevenDaysCountHandler);	// 22
}

void CRequirementHandlerManager::Add(IRequirementHandler *pRequirementHandler)
{
	if (pRequirementHandler != NULL && pRequirementHandler->GetRequirementHandlerID() > 0 && pRequirementHandler->GetRequirementHandlerID() < MAX_ACHIEVEMENT_HANDLER_COUNT)
	{
		m_ppRequirementHandler[pRequirementHandler->GetRequirementHandlerID()] = pRequirementHandler;
	}
}

void CRequirementHandlerManager::Remove(unsigned int nRequirementHanderID)
{
	if (nRequirementHanderID < MAX_ACHIEVEMENT_HANDLER_COUNT)
	{
		delete m_ppRequirementHandler[nRequirementHanderID];	
		m_ppRequirementHandler[nRequirementHanderID] = NULL;
	}
}

const IRequirementHandler * CRequirementHandlerManager::GetHandler(unsigned short nHandlerID)
{
	if (/*nHandlerID >= 0 &&*/ nHandlerID < MAX_ACHIEVEMENT_HANDLER_COUNT)
	{
		return m_ppRequirementHandler[nHandlerID];	
	}

	return NULL;
}



//end file



