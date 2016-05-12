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

    // ��ʼ��
    void Init();

	// �������
	void AddCheckInConfig( unsigned int nMonth, const CCheckInConfig & info );
	void AddCheckInDescConfig( std::string & strDesc );

	// ��ȡǩ����Ϣ
	CCheckInConfig * GetCheckInConfig( unsigned int nDayIndex );

	// ��ȡ��һ��ǩ��Index
	CCheckInConfig * GetNextCheckInConfig( unsigned int nDayIndex );

	// ��ȡ��һ��ǩ��Index
	CCheckInConfig * GetLastCheckInConfig( unsigned int nDayIndex );

	// ��ȡ��ǩ���Ĵ���
	unsigned int GetHaveCheckInCount( unsigned int nDayIndex );

	// ��ȡ˵��
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




