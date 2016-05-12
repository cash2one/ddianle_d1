#ifndef		__CUMMULATIVESPENDMGR_H__
#define		__CUMMULATIVESPENDMGR_H__

#include <list>
#include <map>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../Macros/Macros.h"

#define Cumulative_Spend_Reward_MaxCount 9

class CCumulativeSpendReward
{
public:
	CCumulativeSpendReward();
	~CCumulativeSpendReward();
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

typedef std::map< int,  CCumulativeSpendReward* > CCumulativeSpendReward_t;

class CCumulativeSpendMgr
{
private:
	CCumulativeSpendMgr();
	~CCumulativeSpendMgr();
public:
	static CCumulativeSpendMgr & Instance();
	bool Init();

	void AddCumulativeSpendInfo(bool bExhibit, int nCurrencyType, int m_nWeight, 
								std::string & rstrRegularImageName, std::string & rThumbnailsName, 
								std::string & rStrActivityName, std::string & rStrContent, 
								std::string & rStrActivityBeginTime, std::string & rStrActivityEndTime, 
								std::string & rRechargeStartTime, std::string & rStrRechargeStopTime,
								std::string & rStrRechargeStartAnnounce, std::string & rStrRechargeStopAnnounce);

	void AddCummulativeSpendRewardInfo(CCumulativeSpendReward & rewward);

public:
	bool IsOpening();
	void SetIsOpening(bool isOpening);
	bool Exhibit();
	int Weight();
	const std::string & RegularImageName();
	const std::string & ThumbnailsName();
	const std::map< int,  CCumulativeSpendReward* > & RewardMap();
	std::string & ActivityName();
	std::string & Detail();
	unsigned int ActivityBeginTime();
	unsigned int ActivityEndTime();
	unsigned int SpendStartTime();
	unsigned int SpendStopTime();
	bool IsValidLevel(int nLevel);
	bool IsValidSpendTime(time_t nTime);
	const CCumulativeSpendReward *GetRewardInfo(int nLevel);
	int GetCurrencyType(){ return m_nCurrencyType; }

private:
	bool m_bIsOpening;

	bool m_bExhibit;
	int  m_nCurrencyType;
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

	CCumulativeSpendReward_t m_mapRewards;	// index - CCumlativeReward
};

//////////////////////////////////////////////////////////////////////////

class CumulativeSpendGotMedalRewards
{
public:
    CumulativeSpendGotMedalRewards() {}
    ~CumulativeSpendGotMedalRewards() {}

    CLASS_PROPERTY(int, m_nRequiredNum, RequiredNum)
    CLASS_PROPERTY_BY_REF(std::list<int>, m_listMaleReward, MaleReward)
    CLASS_PROPERTY_BY_REF(std::list<int>, m_listFemaleReward, FemaleReward)
};

class CumulativeSpendGotMedalMgr
{
private:
    CumulativeSpendGotMedalMgr();
    ~CumulativeSpendGotMedalMgr() {}

public:
    CLASS_INSTANCE_FUNC(CumulativeSpendGotMedalMgr)

    void Initialize();

    void OnUpdate( unsigned int nNow );

    bool IsValidIndex( unsigned char nIndex ) const;
    int GetMaxLevel() const;

    const std::map<unsigned char, CumulativeSpendGotMedalRewards>& GetAllRewardsInfo() const { return m_mapRewardInfos; }

    int GetRequiredAmount( unsigned char nIndex ) const;

    void GetRewards( unsigned char nIndex, unsigned char nSex, std::list<int> &rReward ) const;

    void InitInfoByLua( bool bExhibit, int nWeight, unsigned int nBeginTime, unsigned int nEndTime, 
        unsigned int nOpenTime, unsigned int nCloseTime, const std::string &rTitle, const std::string &rContent, 
        const std::string &rSmallImage, const std::string &rBigImage, unsigned char nCumulativeType );

    void AddRewardInfo( unsigned char nIndex, int nRequiredNum, const std::list<int> &rMaleReward, const std::list<int> &rFemaleReward );

protected:
    void CheckValid( unsigned int nNow );

    CLASS_BOOL_PROPERTY_READONLY(m_bOpening, Opening)
    CLASS_BOOL_PROPERTY_READONLY(m_bValid, Valid)
    CLASS_BOOL_PROPERTY_READONLY(m_bExhibit, Exhibit)

    CLASS_PROPERTY_READONLY(int, m_nWeight, Weight)
    CLASS_PROPERTY_READONLY(unsigned int, m_nBeginTime, BeginTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nEndTime, EndTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nOpenTime, OpenTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nCloseTime, CloseTime)

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strTitle, Title)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strContent, Content)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strSmallImage, SmallImage)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strBigImage, BigImage)

    CLASS_PROPERTY_READONLY(unsigned char, m_nCumulativeType, CumulativeType)

private:
    std::map<unsigned char, CumulativeSpendGotMedalRewards> m_mapRewardInfos;
};

#endif




