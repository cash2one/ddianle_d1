#ifndef		__FESTIVALBLESSACTIVITYDATAMGR_H__
#define		__FESTIVALBLESSACTIVITYDATAMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"
#include "ActivityDataStruct.h"


//////////////////////////////////////////////////
class CFestivalBlessActivityDataMgr
{
private:
	CFestivalBlessActivityDataMgr();
	~CFestivalBlessActivityDataMgr();
public:
	static CFestivalBlessActivityDataMgr & Instance();
public:
	bool Init();
	void AddBlessActivityInfo(unsigned int nShowBeginTime, unsigned int nShowEndTime, 
		                      unsigned int nBlessBeginTime, unsigned int nBlessEndTime,
							  itemtype_t nBlessRequireItemType);

	void AddBlessWords(CFestivalBlessWords *pBlessWords);
	
	void AddBlessRewards(CFestivalBlessedReward *pBlessReward);

	CFestivalSendBlessReward & SendBlessReward();
	void AddSendBlessReward(CFestivalSendBlessReward & rSendBlessReward);


	bool IsShowOpening();
	void SetIsShowOpening(bool bIsShowingOpening);

	bool IsBlessOpening();
	void SetIsBlessOpening(bool bIsBlessOpening);

	unsigned int BlessBeginTime();
	unsigned int BlessEndTime();

	itemtype_t BlessRequireItem();

	CFestivalBlessWords * GetFestivalBlessWord(int nIndex);

	CFestivalBlessedReward * GetFestivalBlessReward(int nLevel);

	const std::map<int, CFestivalBlessedReward*> & FestivalBlessRewardMap();
	const std::map<int, CFestivalBlessWords*> & FestivalBlessWordsMap();
public:
	bool			m_bIsShowOpening;
	bool			m_bIsBlessOpening;	
	unsigned int	m_nShowActivityBeginTime;
	unsigned int	m_nShowActivityEndTime;
	unsigned int	m_nBlessBeginTime;
	unsigned int	m_nBlessEndTime;
	itemtype_t	m_nBlessRequireItemType;

	CFestivalSendBlessReward m_SendBlessReward;
	std::map<int, CFestivalBlessWords*>	m_mapFestivalBlessWords;
	std::map<int, CFestivalBlessedReward*>	m_mapFestivalBlessedReward;
};






#endif




