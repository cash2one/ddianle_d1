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

    // ���ô�����ʾ
    virtual void OnEquip(unsigned char nPos = 1);
    virtual void OnUnEquip();

    // ���ô�����λ��
    void SetEquipedPos(unsigned char nEquipedPos);
    
    // ��ȡ������λ��
    unsigned char GetEquipedPos() const;

    // �Ƿ�װ��
    bool HasEquiped() const;

    // ��ȡ���ô���λ��
    unsigned char GetEquipConfigPos() const;

    // �Ƿ���Ҫ�Ϸ��Լ��
    bool RequireIntegrityCheck() const;

    // ��ȡ/������ЧID
    void SetEffectID(unsigned int nEffectID);
    unsigned int GetEffectID() const;

    // �Ƿ��Ǿ���Ч����
    bool IsOldEffectCloth();

    bool IsEquipStatusChanged() const;
    void ResetLoginEquipStatus();

protected:
    unsigned char       m_nIsEquiped;   // �Ƿ��Ѿ��������ϣ�����Ʒʱ�·�ʱ����unsigned char ��ʵ�ǵ�bool���С��,����Ʒʱ����ʱ,����ǵ�ǰλ��+1.
    unsigned char       m_nLoginEquipedStatus;   // ��½ʱװ��״̬
    unsigned int        m_nEffectID;    // ��ЧID
};

#endif //__EQUIP_ITEM_H__

