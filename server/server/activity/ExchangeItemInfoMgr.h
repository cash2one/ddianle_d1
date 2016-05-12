#ifndef		__EXCHANGEITEMINFOMGR_H__
#define		__EXCHANGEITEMINFOMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"

class CExchangeItemInfo
{
public:
	CExchangeItemInfo();
	~CExchangeItemInfo();
public:
	int					m_nLevel;
	int					m_nRequiredItemNum;
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	int					m_nMoney;
};

typedef std::map< int, CExchangeItemInfo * >	ExchangeItem_t;

class CExchangeItemInfoMgr
{
private:
	CExchangeItemInfoMgr();
	~CExchangeItemInfoMgr();
public:
	static CExchangeItemInfoMgr & Instance();
	void Init();
public:
	void AddActivityBriefInfo(bool bExhibit, int nWeight, 
							  std::string & rStrRegularImageName, std::string & rStrThumbnailsName,
							  itemtype_t nItemType,
							  std::string & rStrExhibitBeginTime, std::string & rExhibitEndTime,
							  std::string & rStrExchangeBeginTime, std::string & rStrExchangeEndTime,
							  std::string & rStrActivityName, std::string & rStrActivityIntro, 
							  std::string & rStrActivityStartAnnouce, std::string & rStrActivityEndAnnounce);

	void AddExchangeItemInfo(CExchangeItemInfo & exchangeItemInfo);

	void SetIsOpening(bool isExhibiting);
	bool IsOpening();

	bool IsExchangeItemOpen();

	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumbnailsName();
	itemtype_t RequiredItemType();
	time_t ExhibitBeginTime();
	time_t ExhibitEndTime();
	time_t ActivityStartTime();
	time_t ActivityEndTime();
	std::string & ActivityName();
	std::string & ActivityIntro();
	std::string & ActivityStartAnnouce();
	std::string & ActivityEndAnnouce();
	const ExchangeItem_t & ExchangeInfoMap();

	bool IsValidLevel(int nLevel);
	const CExchangeItemInfo * GetExchangeItemInfo(int nLevel);
public:
	bool m_bIsOpening;
	bool m_bExhibit;
	int m_nWeight;
	std::string	m_strRegularImageName;
	std::string m_strThumbnailsName;
	itemtype_t m_nRequiredItemType;
	time_t m_nExhibitBeginTime;
	time_t m_nExhibitEndTime;
	time_t m_nActivityBeginTime;
	time_t m_nActivityEndTime;
	std::string m_strActivityName;
	std::string m_strActivityIntro;
	std::string m_strActivityBeginAnnounce;
	std::string m_strActivityEndAnnouce;

	ExchangeItem_t	m_mapExchangeInfo;
};





#endif

