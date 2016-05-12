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

    // 获取vip每日奖励
	void GetDailyExpBonus( int nLevel, int & nExp, int & nBindMBill, std::list<CItem> & listItem );

	int ConsumedPtExpBounsRate();

    // 获取商城赠送增加vip经验限额M币
    int GetSendAddVipExpMaxBill(){ return m_nSendAddVipExpMaxBill; }

	unsigned int GetExpLevelUpNeed(int nLevel);
	const CVIPLevelPrivInfo *GetVIPPrivInfo(int nLevel);
	void AddVIPPrivInfo(CVIPLevelPrivInfo *pLevelUpReward);
	void AddVIPExpInfo(CVIPExpInfo *pVIPExp);
	void AddVIPInfo(int nMaxLevel, int nConsumedPtExpBounsRate, int nSendAddVipExpMaxBill);
	const VIPLevelPrivInfoContainer & VIPLevelUpContainer();
	void Serialize(CParamPool & IOBuff, unsigned char nSex);

    // 获取vip魔法神灯购买体力优化次数
    int GetBuyVitCount(int nLevel);

    // 获取VIP对应的仓库容量
    int GetStorageCapacity(int nLevel);

    // 获取舞团秘境额外挑战次数
    int GetDanceGroupFairyLandExChance(int nLevel);

    // 获取舞团秘境Vipbuf
    int GetDanceGroupFairyLandVipBuf(int nLevel);

public:
	int m_nMaxLevel;
	int m_nConsumedPtExpBounsRate;
    int m_nSendAddVipExpMaxBill;
	VIPLevelPrivInfoContainer	m_mapVIPPrivInfo;
	std::map<int, CVIPExpInfo *> m_mapVIPNeedLevelUpExp;
};









#endif


