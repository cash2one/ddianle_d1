#ifndef		__MonthCardDataMGR_H__
#define		__MonthCardDataMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"


using namespace std;

enum EMonthCardType
{
	EMonthCardType_None, // 
	EMonthCardType_Month, // 以月为单位
};

enum EMonthCardMsgError
{
	EMonthCardMsgError_Success,
	EMonthCardMsgError_ConfigNot,			// 配置不存在
	EMonthCardMsgError_HaveOpen,			// 已经开通了
	EMonthCardMsgError_NotOpen,				// 没有开通
	EMonthCardMsgError_MCoinNotEnough,		// M币不够
	EMonthCardMsgError_HaveGetReward,		// 已经领取
};

struct MonthCardInfo
{
	EMonthCardType	nType;								// 卡类型
	int	nPrice;											// 价格（M币）
	unsigned int nOpenDays;								// 开通天数(秒)
	unsigned int nRenewDays;							// 续费天数（秒）
	int	nOpenMBillReward;								// 开通M币奖励
	std::list<CItem> listOpenMaleItemReward;			// 开通物品奖励
	std::list<CItem> listOpenFemaleItemReward;			// 开通物品奖励
	int	nOpenMoneyReward;								// 开通金券奖励
	int	nEveryDayMBillReward;							// 每日M币奖励
	std::list<CItem> listEveryDayMaleItemReward;		// 每日物品奖励
	std::list<CItem> listEveryDayFemaleItemReward;		// 每日物品奖励
	int	nEveryDayMoneyReward;							// 每日金券奖励
	bool bIsExclusive;									// 是否排他
	std::string strDesc;								// 说明

	MonthCardInfo()
	{
		nType = EMonthCardType_Month;
		nPrice = 0;
		nOpenDays = 0;
		nRenewDays = 0;
		nOpenMBillReward = 0;
		nOpenMoneyReward = 0;
		nEveryDayMBillReward = 0;
		nEveryDayMoneyReward = 0;
		bIsExclusive = false;
		strDesc = "";
	}
};


class CMonthCardDataMgr
{
public:
	CMonthCardDataMgr();
	~CMonthCardDataMgr();

	static CMonthCardDataMgr & Instance();

public:
	void AddMonthCardConfig( const MonthCardInfo & info );

	const MonthCardInfo * GetMonthCardInfo( EMonthCardType etype );

public:
	std::map<unsigned int,MonthCardInfo>  m_MonthCardConfig;
};








#endif




