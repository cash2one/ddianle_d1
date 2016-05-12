#ifndef		__CUMMULATIVERECHARGEMGR_H__
#define		__CUMMULATIVERECHARGEMGR_H__

#include <list>
#include <map>
#include <string>
#include "../../datastructure/DataStruct_Base.h"



class CCumulativeReward
{
public:
	CCumulativeReward();
	~CCumulativeReward();

public:
	int m_nLevel;							//第几档次的奖励,对应index
	int m_nRequiredNum;						//需要达到的数值
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	int m_nMoney;
    int m_nBindMCoin;

public:
	void doEncode(CParamPool & IOBuff);
};

typedef std::map< int,  CCumulativeReward* > CumulativeReward_t;

class CCumulativeRechargeMgr
{
private:
	CCumulativeRechargeMgr();
	~CCumulativeRechargeMgr();
public:
	static CCumulativeRechargeMgr & Instance();
	bool Init();

	void AddCumulativeBriefInfo(bool bExhibit, int m_nWeight, 
								std::string & rstrRegularImageName, std::string & rThumbnailsName, 
								std::string & rStrActivityName, std::string & rStrContent, 
								std::string & rStrActivityBeginTime, std::string & rStrActivityEndTime, 
								std::string & rRechargeStartTime, std::string & rStrRechargeStopTime,
								std::string & rStrRechargeStartAnnounce, std::string & rStrRechargeStopAnnounce);
	void AddCummulativeRechargeInfo(CCumulativeReward & rewward);
public:
	bool IsOpening();
	void SetIsOpening(bool isOpening);
	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumbnailsName();
	const std::map< int,  CCumulativeReward* > & RewardMap();
	std::string & ActivityName();
	std::string & Detail();
	unsigned int ActivityBeginTime();
	unsigned int ActivityEndTime();
	unsigned int RechargeStartTime();
	unsigned int RechargeStopTime();
	bool IsValidLevel(int nLevel);
	bool IsValidRechargeTime(time_t nTime);
	const CCumulativeReward *GetRewardInfo(int nLevel);
private:
	bool m_bIsOpening;

	bool m_bExhibit;
	int m_nWeight;
	std::string m_strRegularImageName;
	std::string m_strThumblnailsName;
	std::string m_strActivityName;
	std::string m_strContent;
	unsigned int m_nActivityBeginTime;
	unsigned int m_nActivityEndTime;
	unsigned int m_nRechargeStartTime;
	unsigned int m_nRechargeStopTime;
	std::string m_strRechargeStartAnnouce;
	std::string m_strRechargeStopAnnouce;

	CumulativeReward_t m_mapRewards;	// index - CCumlativeReward
};


#endif




