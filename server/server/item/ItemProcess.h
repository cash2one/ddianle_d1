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
	//����������Ʒ 
	static bool AddItems(CRoleEntity & rRoleEntity, const std::list< CItem > & rItemList, EItemAction eItemAction, __int64 nSrcData, bool bNoticeClient);
    //�ȸ���ҷ�����Ʒ�����ʧ�ܣ����ʼ� 
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
	std::vector<CItem>	m_RandItemVec;		//�����Ʒ
	std::vector<int>	m_RandRateVec;		//��Ʒ����
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
	std::vector<CItem>	m_BetItemVec;		//�����Ʒ
	std::vector<char>	m_BetModeVec;		//���ģʽ
	std::vector<int>	m_FreeRateVec;		//��Ѹ���
	std::vector<int>	m_ChargeRateVec;	//�շѸ���
};


#endif //__ITEM_PROCESS_H__

