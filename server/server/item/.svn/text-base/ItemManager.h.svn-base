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
    // 初始化
    void InitializeCoolDown(const std::list<CoolDownData> & listCooling);

    //  更新过期物品
    void CheckItemDueTime(std::list<ItemBase*> & listDueTimeItem);

    // 下发物品信息
    void SerializeToBuff( std::string &rBuff ) const;

    // 根据DB数目初始化背包
    bool CreateFromDB(const std::list<DBItem> & listItem, const std::list<CoolDownData> & listCooling, std::list<itemtype_t> & listRepeatItem);
    void PacketToDB(std::list<DBItem> & listItem) const;
    void PacketToCache( std::list<DBItem> &rItem, std::map<itemid_t, unsigned int> &rClothEffect, std::list<CoolDownData> &rCoolDown ) const;

    // 清理所有数据
    void Destroy();

    // 移除物品
    void RemoveItem(itemid_t nItemID);

    // 物品是否在冷却
    bool IsInCooling(unsigned short nCoolingFlag);

    // 查找特定栏位物品格子
    ItemBase * GetItemByType(itemtype_t nItemType);

    // 根据物品ID查找
    ItemBase * GetItemByID(itemid_t nItemID) const;

    // 搜索物品
	void SearchItemByType(itemtype_t nItemType, std::list<itemid_t> & listItemID);

    // 添加物品到新格子
    ItemBase * AddItem(ItemConfig * pItemConfig, int nCount, int nDueTime = 0, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

    // 添加物品冷却
    void UpdateCoolingCD(unsigned short nCoolingFlag, unsigned int nCoolingTime);

    // 获取CD数据
    void GetItemCoolingData(std::list<CoolDownData> & coolingData);

    // 清空CD记录
    void ClearAllInCoolingItem();

    // 更新物品的过期时间
    void UpdateItemDueTime(int nOldDueTime, int nNewDueTime, itemid_t nItemID);

    // 更新栏位物品个数
    void UpdateItemCount(ItemBase * pItemBase, int nItemCount);
    int BatchUpdateItemCount(ItemBase * pItemBase, int nItemCount);

	// 加载DB数据
	bool LoadItemData(const DBItem & dbItemData);

private:
    // 添加物品到背包
    void InternalAddItem(ItemBase * pItemBase);

    // 添加物品到物品管理器
    void InsertInItemManager(ItemBase* pItemBase);
    void RemoveFromItemManager(itemid_t nItemID);

    // 添加/移除类型管理器
    void InsertInTypeManager(itemtype_t nItemType, itemid_t nItemID);
    void RemoveFromTypeManager(itemtype_t nItemType, itemid_t nItemID);

    // 添加/管理过期管理器
    void InsertInDueTimeManager(int nDueTime, itemid_t nItemID);
    void RemoveFromDueTimeManager(unsigned int nDueTime, itemid_t nItemID);

    // 移除过期物品
    void GetDueTimeItem(int nDueTime, std::list<ItemBase*> & listDueTimeItem);

    // 检查唯一物品是否不唯一
    bool CheckUniqueItem(EItemColumn eColumn, const DBItem & dbItemData);

    // 处理错误DB数据
    void ProcessWrongDBData(const std::list<DBItem> & listWrongData, std::list<itemtype_t> & listRepeatItem);

    // 创建物品
    ItemBase * CreateItem(ItemConfig * pItemConfig, const DBItem & dbItemData);
    ItemBase * CreateItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);
    ItemBase * CreateItem1(ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

private:
    std::map<itemid_t,ItemBase*>            m_mapItem;                             // 物品管理 key--物品ID、value--物品指针
    std::multimap<itemtype_t,itemid_t>      m_mapItemType;                         // 物品类型对应存放背包位置管理 key--物品类型、value--物品ID
    std::multimap<unsigned int,itemid_t>    m_mapDueTimeItem;                      // 过期物品时间管理 key--过期时间、value--物品ID
    std::map<unsigned short,unsigned int>   m_mapItemCooling;                      // 物品冷却时间管理 kye--冷却标示、value--要冷却的时间
};

#endif
