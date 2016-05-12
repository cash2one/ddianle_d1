#ifndef __EQUIP_ITEM_H__
#define __EQUIP_ITEM_H__

#include "ItemBase.h"
#include "Macro_Define.h"
#include "DataStruct_Base.h"

class EquipItem : public ItemBase
{
public:
    EquipItem(void);
    EquipItem(const ItemConfig *pItemConfig);
    virtual ~EquipItem(void);

public:
    virtual void Init(const DBItem & dbInfo);
    virtual void Encode(CParamPool * pPool) const;

    // 设置穿戴标示
    virtual void OnEquip(unsigned char nPos = 1);
    virtual void OnUnEquip();

    // 设置穿戴的位置
    void SetEquipedPos(unsigned char nEquipedPos);
    
    // 获取穿戴的位置
    unsigned char GetEquipedPos() const;

    // 是否装备
    bool HasEquiped() const;

    // 获取配置穿戴位置
    unsigned char GetEquipConfigPos() const;

    // 是否需要合法性检查
    bool RequireIntegrityCheck() const;

    // 获取/设置特效ID
    void SetEffectID(unsigned int nEffectID);
    unsigned int GetEffectID() const;

    // 是否是旧特效服饰
    bool IsOldEffectCloth();

    bool IsEquipStatusChanged() const;
    void ResetLoginEquipStatus();

protected:
    unsigned char       m_nIsEquiped;   // 是否已经穿在身上，当物品时衣服时别看是unsigned char 其实是当bool大家小心,当物品时徽章时,存的是当前位置+1.
    unsigned char       m_nLoginEquipedStatus;   // 登陆时装备状态
    unsigned int        m_nEffectID;    // 特效ID
};

#endif //__EQUIP_ITEM_H__

