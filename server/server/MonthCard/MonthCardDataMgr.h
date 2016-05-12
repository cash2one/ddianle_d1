#ifndef		__MonthCardDataMGR_H__
#define		__MonthCardDataMGR_H__

#include <list>
#include <map>
#include "../../datastructure/DataStruct_Base.h"


using namespace std;

enum EMonthCardType
{
	EMonthCardType_None, // 
	EMonthCardType_Month, // ����Ϊ��λ
};

enum EMonthCardMsgError
{
	EMonthCardMsgError_Success,
	EMonthCardMsgError_ConfigNot,			// ���ò�����
	EMonthCardMsgError_HaveOpen,			// �Ѿ���ͨ��
	EMonthCardMsgError_NotOpen,				// û�п�ͨ
	EMonthCardMsgError_MCoinNotEnough,		// M�Ҳ���
	EMonthCardMsgError_HaveGetReward,		// �Ѿ���ȡ
};

struct MonthCardInfo
{
	EMonthCardType	nType;								// ������
	int	nPrice;											// �۸�M�ң�
	unsigned int nOpenDays;								// ��ͨ����(��)
	unsigned int nRenewDays;							// �����������룩
	int	nOpenMBillReward;								// ��ͨM�ҽ���
	std::list<CItem> listOpenMaleItemReward;			// ��ͨ��Ʒ����
	std::list<CItem> listOpenFemaleItemReward;			// ��ͨ��Ʒ����
	int	nOpenMoneyReward;								// ��ͨ��ȯ����
	int	nEveryDayMBillReward;							// ÿ��M�ҽ���
	std::list<CItem> listEveryDayMaleItemReward;		// ÿ����Ʒ����
	std::list<CItem> listEveryDayFemaleItemReward;		// ÿ����Ʒ����
	int	nEveryDayMoneyReward;							// ÿ�ս�ȯ����
	bool bIsExclusive;									// �Ƿ�����
	std::string strDesc;								// ˵��

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




