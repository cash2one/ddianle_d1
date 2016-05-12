#ifndef		__XMASMAKEWISHDATAMGR_H__
#define		__XMASMAKEWISHDATAMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"


class CFestivalWishRequireData_Raw
{
public:
	CFestivalWishRequireData_Raw();
	~CFestivalWishRequireData_Raw();
public:
	int m_nWishStartTimes;
	int m_nWishEndTimes;
	itemtype_t m_nItemType;		//许愿所需物品
	unsigned short m_nItemCount;	//许愿所需物品数量
};

class CFestivalWishRequireData
{
public:
	CFestivalWishRequireData();
	~CFestivalWishRequireData();
public:
	int m_nWishTimes;
	itemtype_t m_nItemType;
	unsigned short m_nIteCount;
};


class CFestivalWishRewardData
{
public:
	CFestivalWishRewardData();
	~CFestivalWishRewardData();
public:
	int					m_nTimes;
	std::list<CItem>	m_listMaleItems;
	std::list<CItem>	m_listFemaleItems;
	int					m_nMoney;
};

class CFestivalWishDataMgr
{
private:
	CFestivalWishDataMgr();
	~CFestivalWishDataMgr();
public:
	static CFestivalWishDataMgr & Instance();

	bool Init();
	bool IsOpening();
	void SetIsOpening(bool bIsOpening);
	unsigned int ActivityStartTime();
	unsigned int ActivityEndTime();
	int MaxWishTimes();
	int GetWishNeedItemCount(int nWishTimes);
	CFestivalWishRequireData * GetWishRequirement(int nWishTimes);
	CFestivalWishRewardData * GetWishReward(int nWishTimes);

	void AddActivityData(unsigned int nActivityStartTime, unsigned int nActivityEndTime);
	void AddMakeWishRewardData_Raw(CFestivalWishRequireData_Raw *pRawData);
	void AddRewardData(CFestivalWishRewardData *pRewardData);
	void AddMakeWishRewardData(CFestivalWishRewardData *pRewardInfo);
private:
	void _AddWishRequireData(CFestivalWishRequireData_Raw *pRawData);
public:
	bool m_bIsOpening;
	unsigned int m_nActivityStartTime;
	unsigned int m_nActivityEndTime;
	int m_nMaxWishTimes;
	std::map<int, CFestivalWishRewardData *>	m_mapFestivalWishReward;
	std::map<int, CFestivalWishRequireData *>	m_mapFestivalWishRequireInfo;
	std::list<CFestivalWishRequireData_Raw *>	m_listRawFestivalWishInfo;
};




#endif






