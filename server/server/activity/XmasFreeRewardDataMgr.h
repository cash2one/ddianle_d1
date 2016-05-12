#ifndef		__XMASFREEREWARDDATAMGR_H__
#define		__XMASFREEREWARDDATAMGR_H__

#include <list>
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class CXmasFreeRewardInfo
{
public:
	CXmasFreeRewardInfo();
	~CXmasFreeRewardInfo();
public:
	void doEncode(CParamPool & IOBuff, unsigned char nSex);
public:
	int m_nLevel;	
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	int					m_nMoney;
};

class CXmasFreeRewardDataMgr
{
private:
	CXmasFreeRewardDataMgr();
	~CXmasFreeRewardDataMgr();
public:
	static CXmasFreeRewardDataMgr & Instance();
	bool IsOpening();
	void SetIsOpening(bool isOpening);

	void Init();
	void AddFreeRewardActivityInfo(unsigned int nBeginTime, unsigned int nEndTime);
	void AddFreeRewardInfo(CXmasFreeRewardInfo *pRewardInfo);
	CXmasFreeRewardInfo *GetXmasFreeRewardInfo(int nLevel);
	const std::map<int, CXmasFreeRewardInfo *> & FreeRewardMap();
	unsigned int ActivityStartTime();
	unsigned int ActivityEndTime();
	//void doEncode(CParamPool & IOBuff, unsigned char nSex);
public:
	bool m_bIsOpening;
	unsigned int m_nActivityStartTime;
	unsigned int m_nActivityEndTime;
	std::map<int, CXmasFreeRewardInfo *> m_mapFreeReward;
};







#endif






