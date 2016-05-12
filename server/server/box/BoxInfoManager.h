#ifndef __BOXINFOMANAGER_H_
#define __BOXINFOMANAGER_H_

#include "../item/ItemProcess.h"
#include <map>


class CBoxInfo
{
public:
	CBoxInfo(itemtype_t nBoxID);
	~CBoxInfo(void);

public:
	void AddTheGroup(int nGroupID, int nGroupRate, bool bMale);
	void AddTheItem(int nGroupID, const char* pItemInfo, bool bAnnounce, int nItemRate, bool bMale);

	void GetRandomItem(bool bIsMale, std::list<CItem>& rItemList) const;
	bool CheckBoxInfo() const;

private:
	void _RandomItem(std::list<CItem>& rItemList, const std::map<int, int>& rBoxGroup, const std::map<int, CItemRandCollect>& rBoxItem) const;

private:
	itemtype_t m_nBoxID;

	std::map<int, int> m_MaleBoxGroup;
	std::map<int, CItemRandCollect> m_MaleBoxItem;

	std::map<int, int> m_FemaleBoxGroup;
	std::map<int, CItemRandCollect> m_FemaleBoxItem;
};


class CBoxInfoManager
{
public:
	CBoxInfoManager(void);
	~CBoxInfoManager(void);

public:
	static CBoxInfoManager& Instance();

	void AddBoxGroup(itemtype_t nBoxID, int nGroupID, int nGroupRate, bool bMale);
	void AddBoxItem(itemtype_t nBoxID, int nGroupID, const char* pItemInfo,  bool bAnnounce, int nItemRate, bool bMale);

	CBoxInfo* GetBoxInfoByID(itemtype_t nBoxID) const;
	bool CheckAllBox() const;

private:
	std::map<itemtype_t, CBoxInfo*> m_BoxMap;
};


#endif


