#ifndef		__VIPDATAMGR_H__
#define		__VIPDATAMGR_H__

#include <map>
#include "VIPStruct.h"
#include "VIPDefine.h"

typedef std::map<int, CVIPLevelPrivInfo *> VIPLevelPrivInfoContainer;

class CVIPDataMgr
{
private:
	CVIPDataMgr();
	~CVIPDataMgr();
public:
	static CVIPDataMgr & Instance();
	int GetMaxLevel();

    // ��ȡvipÿ�ս���
	void GetDailyExpBonus( int nLevel, int & nExp, int & nBindMBill, std::list<CItem> & listItem );

	int ConsumedPtExpBounsRate();

    // ��ȡ�̳���������vip�����޶�M��
    int GetSendAddVipExpMaxBill(){ return m_nSendAddVipExpMaxBill; }

	unsigned int GetExpLevelUpNeed(int nLevel);
	const CVIPLevelPrivInfo *GetVIPPrivInfo(int nLevel);
	void AddVIPPrivInfo(CVIPLevelPrivInfo *pLevelUpReward);
	void AddVIPExpInfo(CVIPExpInfo *pVIPExp);
	void AddVIPInfo(int nMaxLevel, int nConsumedPtExpBounsRate, int nSendAddVipExpMaxBill);
	const VIPLevelPrivInfoContainer & VIPLevelUpContainer();
	void Serialize(CParamPool & IOBuff, unsigned char nSex);

    // ��ȡvipħ����ƹ��������Ż�����
    int GetBuyVitCount(int nLevel);

    // ��ȡVIP��Ӧ�Ĳֿ�����
    int GetStorageCapacity(int nLevel);

    // ��ȡ�����ؾ�������ս����
    int GetDanceGroupFairyLandExChance(int nLevel);

    // ��ȡ�����ؾ�Vipbuf
    int GetDanceGroupFairyLandVipBuf(int nLevel);

public:
	int m_nMaxLevel;
	int m_nConsumedPtExpBounsRate;
    int m_nSendAddVipExpMaxBill;
	VIPLevelPrivInfoContainer	m_mapVIPPrivInfo;
	std::map<int, CVIPExpInfo *> m_mapVIPNeedLevelUpExp;
};









#endif


