#ifndef __BAG_H__
#define __BAG_H__

#include <map>
#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include "../../datastructure/ItemBase.h"
#include "../../datastructure/EquipItem.h"

class ItemColumn
{
public:
    ItemColumn();
    virtual ~ItemColumn();

public:
    // 初始化
    void Initialize(int nMaxSlotCount, int nMaxOpenSlot);

    // 是否满的
    bool IsFull();

    // 获取第一个空格子
    int GetFirstEmptySlot();

    // 设置/获取空格子数目
    int GetEmptySlotCount() const;

    // 开通格子
    void AddMaxOpenSlot(int nAddCount);

    // 添加物品
    int AddItem(int nSlot, itemid_t nItemID);

    // 获取物品的个数
    unsigned int GetItemCount();

    // 获取物品ID
    itemid_t GetItemID(int nSlot);

    // 是否合法格子序列
    bool IsValidSlot(int nSlot);//valid

    // 获取最大开放位置
    int GetMaxOpenIndex() const;

    // 移除物品
    void RemoveItem(int nSlot);

    // 是否存在物品
    bool ExistItem(int nSlot);

    // 清理数据
    void Clear(); 

    // 交换物品
    bool ExchangeItem(int nFirstSlot, int nSecondSlot);

private:
     // 增加/减少空格子数目
     void AddEmptySlotCount();
     void DeleteEmptySlotCount();

     // 设置物品
     void UpdateSlotItemID(itemid_t nItemID, int nSlot);

     // 更新空闲位置
     void UpdateFirstEmptySlot(int nNewEmptySlot);

     // 查找第一空闲位置
     int SearchFirstEmptySlot();
    
public:
    std::vector<itemid_t>   m_vecItemID;          // 格子容器 物品ID
    int                     m_nEmptySlotCount;    // 空闲格子数目
    int                     m_nMaxOpenSlot;       // 最大开放格子
    int                     m_nFirstEmptySlot;    // 第一个空闲位子
};

class ItemBag
{
public:
    ItemBag();
    virtual ~ItemBag();

public :
    // 初始化
    void InitializeBag(EItemColumn eColumn, int nMaxSolotCount, int nMaxOpenSlot);

    //  更新过期物品
    void CheckItemDueTime(std::list<ItemBase*> & listDueTimeItem);

    // 下发物品信息
    void SerializeComponent(CParamPool &IOBuff);

    // 根据DB数目初始化背包
    bool CreateFromDB(const std::list<DBItem> & listItem, const std::list<CoolDownData> & listCooling, std::list<itemtype_t> & listRepeatItem);
    void PacketToDB(std::list<DBItem> & listItem, std::list<CoolDownData> & listCooling) const;

    // 清理所有数据
    void Destroy();

    // 移除物品
    void RemoveItem(EItemColumn eColumn, int nSlot);
    void RemoveItem(itemid_t nItemID);
       
    // 物品是否存在
    // nItemType物品类型 nCount物品数目
    bool IsExist(itemtype_t nItemType, unsigned int nCount);

    // 检查栏位和格子
    bool IsValidGrid(EItemColumn eColumn, int nSlot);

    // 背包是否满了
    bool IsColumnFull(EItemColumn eColumn);

    // 物品是否在冷却
    bool IsInCooling(unsigned short nCoolingFlag);

    // 查找特定栏位物品格子
    ItemBase * GetItemBySlot(EItemColumn eColumn, int nSlot);
    ItemBase * GetItemByType(EItemColumn eColumn, itemtype_t nItemType);

    // 根据物品ID查找
    ItemBase * GetItemByID(itemid_t nItemID);

    // 搜索物品
    void SearchItemByType(itemtype_t nItemType, std::list<itemid_t> & listItemID , EItemColumn eExcludeColumn = EItemColumn_Invalid);
    void SearchItemByType(EItemColumn eColumn, itemtype_t nItemType, std::list<itemid_t> & listItemID);
    void SearchItem(EItemColumn eColumn, std::list<itemid_t> & listItemID);

    // 移动物品位置
    // nFromColumn旧栏位 nFromIndex旧格子序列 nToColumn新栏位 nToIndex新格子序列
    void MoveItem(EItemColumn eFromColumn, int nFromSlot, EItemColumn eToColumn, int nToSlot);

    // 交换物品
    void ExchangeItem(ItemBase * pFirstItem, ItemBase * pSecondItem);

    //  获取栏位格子数目
    unsigned int GetColumnSlotNum(EItemColumn eColumn);

    // 获取栏位空格子数目
    unsigned int GetColumnEmptyNum(EItemColumn eColumn);

    // 获取物品能添加数目
    unsigned int GetColumnPutInMaxCount(EItemColumn eColumn, itemtype_t nItemType);

    // 添加物品到新格子
    ItemBase * AddItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime = 0, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

    // 获取栏位物品数目
    unsigned int GetColumnItemCount(EItemColumn eColumn);

    // 获取栏位第一个空格子
    int GetColumnFirstEmptySlot(EItemColumn eColumn);

    // 添加物品冷却
    void UpdateCoolingCD(unsigned short nCoolingFlag, unsigned int nCoolingTime);

    // 获取CD数据
    void GetItemCoolingData(std::list<CoolDownData> & coolingData);

    // 添加栏位开发格子数
    void AddColumnMaxOpenSlot(EItemColumn eColumn, int nAddSlot);

    // 清空CD记录
    void ClearAllInCoolingItem();

    // 更新物品的过期时间
    void UpdateItemDueTime(int nOldDueTime, int nNewDueTime, itemid_t nItemID);

    // 更新栏位物品个数
    void UpdateItemCount(ItemBase * pItemBase, int nItemCount);
    int BatchUpdateItemCount(ItemBase * pItemBase, int nItemCount);

    // 开始收集变化格子
    void BeginCollectChangeGrid();
    void EndCollectChangeGrid(std::list<itemgrid_t> & listGridChange);
    void AddChangeGrid(EItemColumn eColumn, int nSlot);

private:
    // 检查栏位
    bool IsValidColumn(EItemColumn eColumn);

    // 栏位格子是否有物品
    bool ExistItem(EItemColumn eColumn, int nSlot);

    // 添加物品到背包
    void InternalAddItem(EItemColumn eColumn, int nSlot, ItemBase * pItemBase);

    // 添加物品到物品管理器
    void InsertInItemManager(ItemBase* pItemBase);
    void RemoveFromItemManager(itemid_t nItemID);

    // 添加/移除背包管理器
    void InsertInBagManager(EItemColumn eColumn, int nSlot, itemid_t nItemID);
    void RemoveFromBagManager(EItemColumn eColumn, int nSlot);

    // 添加/移除类型管理器
    void InsertInTypeManager(itemtype_t nItemType, itemid_t nItemID);
    void RemoveFromTypeManager(itemtype_t nItemType, itemid_t nItemID);

    // 添加/管理过期管理器
    void InsertInDueTimeManager(unsigned int nDueTime, itemid_t nItemID);
    void RemoveFromDueTimeManager(unsigned int nDueTime, itemid_t nItemID);

    // 移除过期物品
    void GetDueTimeItem(int nDueTime, std::list<ItemBase*> & listDueTimeItem);

    // 加载DB数据
    bool LoadItemData(const DBItem & dbItemData);

    // 检查唯一物品是否不唯一
    bool CheckUniqueItem(EItemColumn eColumn, const DBItem & dbItemData);

    // 处理错误DB数据
    void ProcessWrongDBData(const std::list<DBItem> & listWrongData, std::list<itemtype_t> & listRepeatItem);

    // 创建物品
    ItemBase * CreateItem(ItemConfig * pItemConfig, const DBItem & dbItemData);
    ItemBase * CreateItem(EItemColumn eColumn, ItemConfig * pItemConfig, int nCount, int nDueTime, itemid_t nItemID = 0, int nDynamicBoxRelatedValue = 0);

private:
    std::map<itemid_t,ItemBase*>            m_mapItem;                             // 物品管理 key--物品ID、value--物品指针
    ItemColumn                              m_ItemBag[EItemColumn_Max];            // 背包管理 一栏位-背包的对应关系
    std::multimap<itemtype_t,itemid_t>      m_mapItemType;                         // 物品类型对应存放背包位置管理 key--物品类型、value--物品ID
    std::multimap<unsigned int,itemid_t>    m_mapDueTimeItem;                      // 过期物品时间管理 key--过期时间、value--物品ID
    std::map<unsigned short,unsigned int>   m_mapItemCooling;                      // 物品冷却时间管理 kye--冷却标示、value--要冷却的时间
    std::list<itemgrid_t>                   m_listChangeGrid;                      // 改变的栏位格子
    bool                                    m_bCollecttingGridChange;              // 是否在收集栏位变化
};

#endif
