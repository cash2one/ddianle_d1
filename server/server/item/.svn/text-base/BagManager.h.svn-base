#ifndef __BAGMANAGER_H__
#define __BAGMANAGER_H__

#include <map>
#include <vector>
#include <set>

#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/ItemBase.h"
#include "ItemManager.h"
#include "../util/CommonFunc.h"

class Bag 
{
public:
    Bag(EBagColumn eColumn);
    virtual ~Bag();

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
	virtual int AddItem(int nSlot, itemid_t nItemID);
    virtual int AddItem(itemid_t nItemID, int & nSlot);

    // 获取物品的个数
    unsigned int GetItemCount();

    // 获取物品ID
    itemid_t GetItemID(int nSlot);

    // 是否合法格子序列
    bool IsValidSlot(int nSlot, bool bWriteLog = true);

    // 获取最大开放位置
    int GetMaxOpenIndex() const;

    // 移除物品
    int RemoveItem(ItemBase * pItemBase);

    // 是否存在物品
    bool ExistItem(int nSlot, bool bWriteLog = true);

    // 清理数据
    void Clear(); 

	// 查找第一空闲位置
	virtual int SearchFirstEmptySlot();

    // 获取所有物品ID
    void GetAllItemID(std::list<itemid_t> & listItemID);

private:
    // 增加/减少空格子数目
    void AddEmptySlotCount();
    void DeleteEmptySlotCount();

    // 设置物品
    void UpdateSlotItemID(itemid_t nItemID, int nSlot);

    // 更新空闲位置
    void UpdateFirstEmptySlot(int nNewEmptySlot);

public:
    EBagColumn              m_eColumn;
    std::vector<itemid_t>   m_vecItemID;          // 格子容器 物品ID
    int                     m_nMaxOpenSlot;       // 最大开放格子
    int                     m_nEmptySlotCount;    // 空闲格子数目
    int                     m_nFirstEmptySlot;    // 第一个空闲位子
};

class EquipBag : public Bag
{
public:
    EquipBag();
    virtual ~EquipBag();

public:
    virtual int RemoveItem(ItemBase * pItemBase);
    virtual void Initialize();
};

class ConsumableBag : public Bag
{
public:
    ConsumableBag();
    virtual ~ConsumableBag();

public:
    virtual void Initialize();
};

class BadgeBag : public Bag
{
public:
    BadgeBag();
    virtual ~BadgeBag();

public:
    virtual void Initialize();
	virtual void SerializeComponent(CParamPool & IOBuff);
};

class StorageBag : public Bag
{
public:
    StorageBag();
    virtual ~StorageBag();

public:
    virtual void Initialize(int nOpenSlot);
    void AddCapacity(int nAddCapacity);
    unsigned int CanAddCapacity();
};

class BagManager
{
public:
    BagManager();
    virtual ~BagManager();

public:
	// 初始化背包大小和ItemManager信息
    void Initalize(int nStorageCapacity);

	// 销毁
	void Destory();

	// 添加物品
    int InitalizeDBData(EBagColumn eColumn, int nSlot, itemid_t nItemID);
    int AddItem(ItemBase * pItemBase);
    int AddItem(EBagColumn eColumn, ItemBase * pItemBase);

	// 同步物品变化到收集列表
	void SyncItemChange(ItemBase * pItemBase);

	// 移除物品
	int RemoveItem(ItemBase * pItemBase);

    // 获取栏位所有ID
    void GetAllItemID(EBagColumn eColumn, std::list<itemid_t> & listItemID);

	// 获取物品ID
	itemid_t GetItemID(EBagColumn eColumn, int nSlot);

	// 获取物品能添加数目
	unsigned int GetColumnPutInMaxCount(EBagColumn eColumn, itemtype_t nItemType, const std::list<itemid_t> & listItemID);

	// 转换成背包枚举
	EBagColumn GetBagColumn(itemtype_t nItemType);
	EBagColumn GetBagColumn(EItemColumn eItemColumn);

	// 获取背包空格子数目
	unsigned int GetColumnEmptyCount(EBagColumn eColumn);
   
	// 开始/结束收集背包变化
	void BeginCollectBagChange();
    void EndCollectBagChange(std::list<itemgrid_t> & listGrid);

    // 增加仓库容量
    void AddStorageCapacity(int nAddCapacity);

    // 仓库能够增加的数目
    unsigned int GetStorageCanAddCapacity();

    // 仓库容量
    unsigned int GetStoragesCapacity();

private:
	// 栏位合法
	bool IsColumnValid(EBagColumn eColumn);

	// 获取背包
	Bag * GetBag(EBagColumn eColumn);

public:
	std::map<EBagColumn, Bag*>			m_mapBag;
	std::set<itemgrid_t, classCompPair<int> >	m_setBagChange;
    std::list<itemgrid_t>               m_listBagChange;
	bool								m_bCollecting;
};



#endif
