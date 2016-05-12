#ifndef __PAYMENTMINFOMGR_H__
#define __PAYMENTMINFOMGR_H__

#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/GameMsg_Map.h"
#include "../../socket/GameMsg_Processor.h"

class CRoleEntity;

class FirstPaymentInfo
{
public:
	std::list<CItem> m_listMaleReward;
	std::string	m_strMaleReward;
	std::list<CItem> m_listFemaleReward;
	std::string	m_strFemaleReward;
	int m_nChargeLevels;
	std::vector<CChargePlan> m_vecChargePlan;
	std::string m_strMailTitle;	
	std::string m_strMailContent;
	int m_StartTime;
	int m_EndTime;
};

// 充值奖励信息
class ReChargeInfo
{
public:
	int m_nChargeLevels;	// 充值等级数
	std::vector<CChargePlan> m_vecChargePlan;	//充值规则
	int m_StartTime;	// 活动 开始时间
	int m_EndTime;		// 活动 结束时间
	std::string m_strMailTitle;
	std::string m_strMailContent;
};

class PaymentInfoMgr
{
private:
	PaymentInfoMgr();
	~PaymentInfoMgr();
public:
	static PaymentInfoMgr & Instance();

public:
	//////////////////////////////////////////////////////////////////////////
	// 读取 首次充值奖励
	void ReadFirstPaymentReward( Version_Type verType, std::list<CItem>& listMaleReward, std::list<CItem>& listFemaleReward,
		int nChargeLevels, std::vector<CChargePlan>& vecChargePlan, string &strMailTitle, string & strMailContent,
		time_t startTime, time_t endTime, std::string& strMaleItem, std::string& strFemaleItem );

	// 判断 首充活动是否开启
	bool FirstChargeRewardIsOpen( Version_Type versionType, int nRechargeTime );

	// 获取 首充奖励信息
	FirstPaymentInfo* GetCurrentPaymentInfo( Version_Type versionType );
	FirstPaymentInfo* GetFirstPaymentInfo( Version_Type versionType, int nRechargeTime );

	// 计算首次充值金券\绑定Mcoin奖励
	bool GetFirstChargeReward( int &nMoneyReward, int &nMcoinReward, const int nChargeAmount, Version_Type verType, int nRechargeTime );

	// 客户端请求 首次充值金券\绑定Mcoin奖励 规则
	void GetFirstChargeRewardPlan( std::vector<CChargePlan>& vecChargePlan, Version_Type versionType, int nRechargeTime );

	int GetFirstChargeStartTime( Version_Type versionType, int nRechargeTime );

	std::string MailTitle( Version_Type versionType, int nRechargeTime );
	std::string MailContent( Version_Type versionType, int nRechargeTime );
	const std::list<CItem>& MaleRewards( Version_Type versionType, int nRechargeTime );
	const std::list<CItem>& FemaleRewards( Version_Type versionType, int nRechargeTime );

	//////////////////////////////////////////////////////////////////////////
	// 读取 日常充值奖励
	void ReadRechargeReward( Version_Type versionType, int nChargeLevels, std::vector<CChargePlan>& vecChargePlan, time_t startTime, time_t endTime,
		const std::string &strstrMailTitle, const std::string &strstrMailContent );
	
	// 获得符合开关条件的 日常充值规则
	ReChargeInfo* GetRechargeInfo( Version_Type versionType, int nRechargeTime );
	
	//日常充值奖励是否开启
	bool RechargeRewardIsOpen( Version_Type versionType, int nRechargeTime );
	
	//计算日常充值奖励
	bool GetRechargeReward( int &nMoneyReward, int &nMcoinReward, std::string &strMailTitle, std::string & strMailContent, 
		const int nChargeAmount, Version_Type verType, int nRechargeTime );
	
	// 客户端请求 日常充值金券\绑定Mcoin奖励
	void GetRechargeRewardPlan( std::vector<CChargePlan>& vecChargePlan, Version_Type versionType, int nRechargeTime );


	//////////////////////////////////////////////////////////////////////////
	// 固定金额首次充值奖励
	// 设置活动开启参数
//	void SetFixedChargeConfig(unsigned short isOpen, unsigned int uStartTime, unsigned int uEndTime);
    void AddFixedChargeConfig(unsigned int uStartTime, unsigned int uEndTime);
	// 添加需要绑定的金额
	void AddFixedChargeInfo(unsigned int mcoin, unsigned giftcoin, const std::string &strItem);

	// 取得赠送的金额
    unsigned FixedChargeGiftCoin(unsigned int mcoin);
    std::string FixedChargeGiftItem(unsigned int mcoin);
	// 固定充值活动当前是否生效
    bool FixedChargeIsRun(time_t nowTime, unsigned int *pStartTime, unsigned int *pEndTime);
    int FixedChargeIndex(time_t nowTime);
// 	// 固定充值活动现在或将来是否能生效
// 	bool CanOpenFirstCharge(time_t nowTime);

protected:
	// 根据(首次、日常)充值规则计算 金券|Mcoin 奖励
	void CalculateReward( int &nMoneyReward, int &nMcoinReward, int nChargeAmount, int nPlanSize, 
		const std::vector<CChargePlan> & vecChargePlan, Version_Type verType, int nRechargeTime );

public:

	std::vector<FirstPaymentInfo>	m_FreeVerFirstPaymentInfo;		// 免费 首次充值
	std::vector<FirstPaymentInfo>	m_ChargeVerFirstPaymentInfo;	// 收费

	std::vector<ReChargeInfo>	m_FreeVersionRechargeInfo;	// 免费 日常充值规则
	std::vector<ReChargeInfo>	m_ChargeVersionRechargeInfo;// 收费 

//	bool m_bOpenedFixedCharge; // 是否开启固定金额首次充值奖励
// 	unsigned int m_uStartTimeFixedCharge; // 开启时间
// 	unsigned int m_uEndTimeFixedCharge; // 结束时间
    typedef std::pair<unsigned int, unsigned int> TimeSpan;
    typedef std::vector<TimeSpan> TimeSpanVec;
    TimeSpanVec m_fixedChargeTimeSpanVec; // 定额首冲活动时间段

	typedef std::map<unsigned int, unsigned int> FixedChargeGiftMCoinMap;
	FixedChargeGiftMCoinMap m_mapGiftFixedCharge; // 固定金额首次充值奖励表
    typedef std::map<unsigned int, std::string> FixedGiftItemMap;
    FixedGiftItemMap m_fixedGiftItemMap;

};

#endif

