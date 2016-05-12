#ifndef __EQUIPMANAGER_H__
#define __EQUIPMANAGER_H__

#include <map>
#include <vector>

#include "../../datastructure/DataStruct_Base.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/ItemBase.h"
#include "../../socket/GameMsg_Base.h"

class Equip
{
public:
    Equip(EEquipColumn eColumn);
    virtual ~Equip();

public:
    virtual void Initialize(int nMaxSlot);
    int Initialize(int nSlot, itemid_t nItemID);
    virtual int CanEquip(ItemEquip * pEquip);
    virtual int OnEquip(ItemEquip * pEquip);
    virtual int UnEquip(ItemEquip * pEquip);
    virtual void UnEquipAll(std::list<itemid_t> & listItemID){};
    bool HasEquip(int nPos, itemid_t nItemID);
    bool HasEquip(itemid_t nItemID);
    itemid_t GetItemID(int nSlot);
    void GetAllItemID(std::list<itemid_t> & listItemID);
    unsigned int GetItemCount();
    bool IsExist(int nSlot);
    void AddSlotCount(int nAddCount);

protected:
    bool IsSlotValid(int nSlot);

public:
    EEquipColumn                m_eColumn;
    std::vector<itemid_t>       m_vecItemID;
    unsigned int                m_nRoleID;
};

class BadgeEquip : public Equip
{
public:
    BadgeEquip();
    virtual ~BadgeEquip();

public:
    virtual int CanEquip(ItemEquip * pEquip);
    virtual int OnEquip(ItemEquip * pEquip);
    virtual int UnEquip(ItemEquip * pEquip);

private:
    int GetFirstFreePos();
};

class WeddingRingEquip : public Equip
{
public:
    WeddingRingEquip();
    virtual ~WeddingRingEquip();

public:
    virtual int CanEquip(ItemEquip * pEquip);
    virtual int OnEquip(ItemEquip * pEquip);
    virtual int UnEquip(ItemEquip * pEquip);
};

class ClothEquip : public Equip
{
public:
    ClothEquip();
    virtual ~ClothEquip();

public:
    virtual int CanEquip(ItemEquip * pEquip);
    virtual int OnEquip(ItemEquip * pEquip);
    virtual int UnEquip(ItemEquip * pEquip);
    virtual void UnEquipAll(std::list<itemid_t> & listItemID);
};

class TransformEquip : public Equip
{
public:
    TransformEquip();
    virtual ~TransformEquip();

public:
    virtual int CanEquip(ItemEquip * pEquip);
    virtual int OnEquip(ItemEquip * pEquip);
    virtual int UnEquip(ItemEquip * pEquip);

private:

};

class EquipManager
{
public:
    EquipManager();
    virtual ~EquipManager();

public:
    // 初始化
    void Initialize(int nBadgeSlotCount);

    // 销毁
    void Destory();

    // 初始化DB装备
    int InitializeDBData(EEquipColumn eColumn, int nSlot, itemid_t nItemID);

    // 穿装备
    int OnEquip(ItemEquip * pEquip);
    int OnEquip(EEquipColumn eColumn, ItemEquip * pEquip);

    // 脱装备
    int UnEquip(ItemEquip * pEquip);
    int UnEquip(EEquipColumn eColumn, ItemEquip * pEquip);
    int UnEquipAll(EEquipColumn eColumn, std::list<itemid_t> & listItemID);

    // 获取物品ID
    itemid_t GetItemID(EEquipColumn eColumn, int nSlot);
    itemid_t GetClothItemID(int nSlot);

    // 获取穿在身上的衣服ID
    void GetEquipedCloth(std::list<itemid_t> & listItemID);
    itemid_t GetEquipedCloth(EItemClothType ePos);

    // 获取栏位物品个数
    unsigned int GetColumnItemCount(EEquipColumn eColumn);

    // 是否装备
    bool IsEquip(ItemEquip * pEquip);
    int CanEquip(ItemEquip * pEquip);

    // 获取装备的栏位
    EEquipColumn GetEquipColumn(ItemEquip * pEquip);

    // 获取栏位格子具体信息
    void GetAllItemID(EEquipColumn eColumn, std::list<itemid_t> & listItemID);

    // 增加栏位格子
    void AddSlotCount(EEquipColumn eColumn, int nAddCount);

private:
    // 获取装备栏位
    Equip * GetEquip(EEquipColumn eColumn);

public:
    std::map<EEquipColumn, Equip*> m_mapEquip;
};

#endif
