#ifndef __DYNAMICBOXINFOMANAGER_H_
#define __DYNAMICBOXINFOMANAGER_H_

#include "../item/ItemProcess.h"
#include <map>


class CDynamicBoxInfo
{
public:
	CDynamicBoxInfo(itemtype_t nBoxID);
	~CDynamicBoxInfo();

public:
	void AddRelatedGroup( unsigned int nItemID, unsigned int nRelatedRate, unsigned int nItemRated );
	void AddTheGroup(int nGroupID, int nGroupRate, bool bMale);
	void AddTheItem(int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale);

	void GetRandomItem(int nRelatedValue, bool bIsMale, std::list<CItem>& rItemList) const;
	bool CheckBoxInfo() const;

private:
	void _RandomItem(int nRelatedValue, std::list<CItem>& rItemList, const std::map<int, int>& rBoxGroup, const std::map<int, CItemRandCollect>& rBoxItem) const;

private:
	itemtype_t m_nBoxID;

	std::map<int, int> m_MaleBoxGroup;
	std::map<int, CItemRandCollect> m_MaleBoxItem;

	std::map<int, int> m_FemaleBoxGroup;
	std::map<int, CItemRandCollect> m_FemaleBoxItem;
	unsigned int	m_nRelatedItemID;
	unsigned int	m_nRelatedRate;
	unsigned int	m_nItemRate;
};


class CDynamicBoxInfoManager
{
public:
	CDynamicBoxInfoManager();
	~CDynamicBoxInfoManager();

public:
	static CDynamicBoxInfoManager& Instance();

	void AddDynamicBoxRelatedGroup(itemtype_t nBoxID, unsigned int nItemID, unsigned int nRelatedRate, unsigned int nItemRated);
	void AddDynamicBoxGroup(itemtype_t nBoxID, int nGroupID, int nGroupRate, bool bMale);
	void AddDynamicBoxItem(itemtype_t nBoxID, int nGroupID, const char* pItemInfo,  bool bAnnounce, int nItemRate, bool bMale);

	CDynamicBoxInfo* GetDynamicBoxInfoByID(itemtype_t nBoxID) const;
	bool CheckAllBox() const;

private:
	std::map<itemtype_t, CDynamicBoxInfo*> m_BoxMap;
};


#endif


