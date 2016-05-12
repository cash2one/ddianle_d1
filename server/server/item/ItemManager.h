#ifndef __BAG_H__
#define __BAG_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/ItemBase.h"
#include "../../datastructure/EquipItem.h"

class ItemManager
{
public:
    ItemManager();
    virtual ~ItemManager();
	friend class Bag;

public :
    // ��ʼ��
    void InitializeCoolDown(const std::list<CoolDownData> & listCooling);

    //  ���¹�����Ʒ
    void CheckItemDueTime(std::list<ItemBase*> & listDueTimeItem);

    // �·���Ʒ��Ϣ
    void SerializeToBuff( std::string &rBuff ) const;

    // ����DB��Ŀ��ʼ������
    bool CreateFromDB(const std::list<DBItem> & listItem, const std::list<CoolDownData> & listCooling, std::list<itemtype_t> & listRepeatItem);
    void PacketToDB(std::list<DBItem> & listItem) const;
    void PacketToCache( std::list<DBItem> &rItem, std::map<itemid_t, unsigned int> &rClothEffect, std::list<CoolDownData> &rCoolDown ) const;

    // ������������
    void Destroy();

    // �Ƴ���Ʒ
    void RemoveItem(itemid_t nItemID);

    // ��Ʒ�Ƿ�����ȴ
    bool IsInCooling(unsigned short nCoolingFlag);

    // �����ض���λ��Ʒ����
    ItemBase * GetItemByType(itemtype_t nItemType);

    // ������ƷID����
    ItemBase * GetItemByID(itemid_t nItemID) const;

    // ������Ʒ
	void SearchItemByType(itemtype_t nItemType, std::list<itemid_t> & listItemID);

    // �����Ʒ���¸���
    ItemBase * AddItem(ItemConfig * pItemConfig, int nCount, int nDueTime = 0, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

    // �����Ʒ��ȴ
    void UpdateCoolingCD(unsigned short nCoolingFlag, unsigned int nCoolingTime);

    // ��ȡCD����
    void GetItemCoolingData(std::list<CoolDownData> & coolingData);

    // ���CD��¼
    void ClearAllInCoolingItem();

    // ������Ʒ�Ĺ���ʱ��
    void UpdateItemDueTime(int nOldDueTime, int nNewDueTime, itemid_t nItemID);

    // ������λ��Ʒ����
    void UpdateItemCount(ItemBase * pItemBase, int nItemCount);
    int BatchUpdateItemCount(ItemBase * pItemBase, int nItemCount);

	// ����DB����
	bool LoadItemData(const DBItem & dbItemData);

private:
    // �����Ʒ������
    void InternalAddItem(ItemBase * pItemBase);

    // �����Ʒ����Ʒ������
    void InsertInItemManager(ItemBase* pItemBase);
    void RemoveFromItemManager(itemid_t nItemID);

    // ���/�Ƴ����͹�����
    void InsertInTypeManager(itemtype_t nItemType, itemid_t nItemID);
    void RemoveFromTypeManager(itemtype_t nItemType, itemid_t nItemID);

    // ���/������ڹ�����
    void InsertInDueTimeManager(int nDueTime, itemid_t nItemID);
    void RemoveFromDueTimeManager(unsigned int nDueTime, itemid_t nItemID);

    // �Ƴ�������Ʒ
    void GetDueTimeItem(int nDueTime, std::list<ItemBase*> & listDueTimeItem);

    // ���Ψһ��Ʒ�Ƿ�Ψһ
    bool CheckUniqueItem(EItemColumn eColumn, const DBItem & dbItemData);

    // �������DB����
    void ProcessWrongDBData(const std::list<DBItem> & listWrongData, std::list<itemtype_t> & listRepeatItem);

    // ������Ʒ
    ItemBase * CreateItem(ItemConfig * pItemConfig, const DBItem & dbItemData);
    ItemBase * CreateItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);
    ItemBase * CreateItem1(ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

private:
    std::map<itemid_t,ItemBase*>            m_mapItem;                             // ��Ʒ���� key--��ƷID��value--��Ʒָ��
    std::multimap<itemtype_t,itemid_t>      m_mapItemType;                         // ��Ʒ���Ͷ�Ӧ��ű���λ�ù��� key--��Ʒ���͡�value--��ƷID
    std::multimap<unsigned int,itemid_t>    m_mapDueTimeItem;                      // ������Ʒʱ����� key--����ʱ�䡢value--��ƷID
    std::map<unsigned short,unsigned int>   m_mapItemCooling;                      // ��Ʒ��ȴʱ����� kye--��ȴ��ʾ��value--Ҫ��ȴ��ʱ��
};

#endif
