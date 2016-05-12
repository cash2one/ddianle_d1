#ifndef __ITEM_PROCESS_H__
#define __ITEM_PROCESS_H__

#include <list>
#include <string>
#include "../../datastructure/DataStruct_DB.h"
#include "../../socket/Windefine.h"


class CRoleEntity;

class CItemProcess
{
public:
	CItemProcess();
	~CItemProcess();

public:
	//给玩家添加物品 
	static bool AddItems(CRoleEntity & rRoleEntity, const std::list< CItem > & rItemList, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient);
    //先给玩家发送物品，如果失败，则发邮件 
    static void AddOrMailItems(CRoleEntity &rRoleEntity, const std::list<CItem> &rItemList, EItemAction eItemAction, __int64 nSrcData, 
        bool bNoticeClient, EMailType nMailType, const std::string &rMailTitle, const std::string &rMailContent);
    static void AddOrMailItems(CRoleEntity &rRoleEntity, const std::list<CItem> &rItemList, EItemAction eItemAction, __int64 nSrcData, 
        bool bNoticeClient, EMailType nMailType, const std::string &rMailSender, const std::string &rMailTitle, const std::string &rMailContent);
};

bool CheckItemVec(const std::vector<CItem>& itemVec);

class CItemRandCollect
{
public:
	CItemRandCollect();
	~CItemRandCollect();

public:
	void AddRandItem(CItem& rRandItem, int nRate);

	bool RandSingleItem(CItem& rItem) const;
	void RandMultiItem(std::list<CItem>& rItemList, int nCount) const;

	const std::vector<CItem>& RandItemCollect() const;
	bool Check() const;
	void GetItem( std::list<CItem> & itemlist ) const;

private:
	std::vector<CItem>	m_RandItemVec;		//随机物品
	std::vector<int>	m_RandRateVec;		//物品概率
};


class CItemBetCollect
{
public:
	CItemBetCollect();
	~CItemBetCollect();

public:
	void AddBetItem(CItem& rBetItem, bool bCanFree, int nFreeRate, int nChargeRate);

	int BetSingleItem(CItem& rItem, bool bIsFree) const;
	void BetMultiItem(std::list<CItem>& rItemList, int nCount, bool bIsFree) const;

	const std::vector<CItem>& BetItemCollect() const;
	const std::vector<char>& BetModeCollect() const;

	bool Check() const;

private:
	std::vector<CItem>	m_BetItemVec;		//随机物品
	std::vector<char>	m_BetModeVec;		//随机模式
	std::vector<int>	m_FreeRateVec;		//免费概率
	std::vector<int>	m_ChargeRateVec;	//收费概率
};


#endif //__ITEM_PROCESS_H__

