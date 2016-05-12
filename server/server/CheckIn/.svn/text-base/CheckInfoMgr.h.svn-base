#ifndef		__CHECKINFOMGR_H__
#define		__CHECKINFOMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"

class CRoleEntity;

using namespace std;

enum ECheckInErrorMsg
{
	ECheckInErrorMsg_Success,
	ECheckInErrorMsg_ConfigError,
	ECheckInErrorMsg_HaveCheckIn,
	ECheckInErrorMsg_DayIndexError,
};

class CCheckInConfig
{
public:
	CCheckInConfig();
	virtual ~CCheckInConfig(){};

public:
	void doDecode(CParamPool & IOBuff);
	void doEncode(CParamPool & IOBuff, char cSex);

public:
	unsigned int m_nDayIndex;
	CItem	m_maleItemReward;
	CItem	m_femaleItemReward;
	unsigned int m_nMoney;
	unsigned int m_nBindCoin;
	unsigned int m_nVipRewardLevel;
	unsigned int m_nVipRewardRate;
};

class CCheckInDataManager
{
public:
	CCheckInDataManager();
	virtual ~CCheckInDataManager(){};

public:
	static CCheckInDataManager & Instance();

    // 初始化
    void Init();

	// 添加配置
	void AddCheckInConfig( unsigned int nMonth, const CCheckInConfig & info );
	void AddCheckInDescConfig( std::string & strDesc );

	// 获取签到信息
	CCheckInConfig * GetCheckInConfig( unsigned int nDayIndex );

	// 获取下一个签到Index
	CCheckInConfig * GetNextCheckInConfig( unsigned int nDayIndex );

	// 获取上一个签到Index
	CCheckInConfig * GetLastCheckInConfig( unsigned int nDayIndex );

	// 获取已签到的次数
	unsigned int GetHaveCheckInCount( unsigned int nDayIndex );

	// 获取说明
	std::string GetDescribe(){ return m_strDescribe; }

	void doEncode(CParamPool & IOBuff,char cSex);
	void doDecode(CParamPool & IOBuff);

    void SetCurrentMonthConfig();
    void SetCurrentMonthConfig( unsigned int nCurrentMonth );
    

public:
	std::string		m_strDescribe;
    unsigned int    m_nCurrentMonth;
	std::map<unsigned int,CCheckInConfig>		m_CurrentMonthConfig;
    std::map<unsigned int, std::map<unsigned int, CCheckInConfig> > m_CheckInDataConfig;
};




#endif




