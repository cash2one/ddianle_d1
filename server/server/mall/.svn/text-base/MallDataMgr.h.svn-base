#ifndef		__MALLDATAMGR_H__
#define		__MALLDATAMGR_H__

#include <vector>
#include <list>
#include <string>
#include "../../datastructure/DataStruct_Base.h"

class CParamPool;

class CRecommendEuipment
{
public:
	CRecommendEuipment();
	~CRecommendEuipment();
public:
	int							m_nIndex;
	std::string					m_strName;
	std::list<itemtype_t>       m_listEquipments;
public:
	void doEncode(CParamPool & IOBuff);
};


typedef std::vector<CRecommendEuipment *> RecommendEquiptment_t;

class CMallDataMgr
{
private:
	CMallDataMgr();
	~CMallDataMgr();
public:
	static CMallDataMgr & Instance();
	bool Init();

	const char * SerializeName();
	void SerializeRecommendEquipments(CParamPool & IOBuff);
public:
	void AddMaleRecommedEquipment(int nIndex, std::string strName, std::string strMaleEquipments);
	void AddFemaleRecommedEquipment(int nIndex, std::string strName, std::string strFemaleEquipments);
	void SetMallTip(const char *strTipTitle, const char *strTip);
	void SetExperienceCardProp(unsigned short timesPerDay, unsigned short dayCanExperienced, itemtype_t experienceCardItemType, std::string &experienceTypes);

private:
	void _TransferToItem(std::string strItems, std::list<unsigned short> & listItems);
    void _TransferToItem(std::string strItems, std::list<unsigned int> & listItems);
	bool _CheckEquipmentData(std::vector<CRecommendEuipment *> & vecRecommendEquipment);
public:
	std::vector<CRecommendEuipment *> m_vecMaleRecommendEquipment;
	std::vector<CRecommendEuipment *> m_vecFemaleRecommendEquipment;

	std::string m_strMallTipTitle;
	std::string m_strMallTip;

	unsigned short					m_experienceCardTimesPerDay; // 新手体验卡每日可以使用的最大次数
	unsigned short					m_dayCanExperienced; // 新手体验卡所购买到物品的天数
	itemtype_t						m_experienceCardItemType; // 体验卡物品ID
	std::vector<unsigned char>		m_vecExperienceType; // 可以用体验卡体验的类型
};




#endif




