#ifndef __ITEM_BASE_H__
#define __ITEM_BASE_H__

#include "datainfor/ItemConfig.h"
#include "DataStruct_Base.h"
#include "Macro_Define.h"

class CParamPool;
class ItemFactory;

/************************************************************************/
/* ItemBase: ��ҵ���Ʒʵ��                                               */
/* ����Ĺ��캯�����������������ܱ���������ֻ��ʹ��ItemFactoryʵ���������٣�      */
/* ��ItemFactoryҲֻ������Ʒģ���ڲ�ʹ�ã��������Ը��õؿ�����Ʒ����������ڣ�     */
/* ������Ʒ�ڴ�����Ӷȣ�������Ʒ�ڴ�й©��                                  */
/************************************************************************/

class ItemBase
{
protected:
    // ��Ʒ
    ItemBase();
    ItemBase(const ItemConfig *pItemConfig);
    virtual ~ItemBase();

    friend class ItemFactory;

public:
    // ��Ʒʵ����ʼ���ӿڣ�ʹ��DBItem�����ʼ����
    virtual void InitFromDBItem(const DBItem &dbItem);
    virtual void SaveToDBItem( DBItem &dbItem ) const;

    // ��Ʒʹ�ýӿ�
    virtual int OnUse();
    virtual int OnBatchUse(int nCount);

    // �Ƿ�����Ч��Ʒ
    virtual bool IsValid() const;

    // ��Ʒ���л�
    virtual void Encode(CParamPool *pool) const;
    virtual void Decode(CParamPool * pPool);

    // ��ȡ��������ƷID
    itemid_t GetItemID() const;
    void SetItemID(itemid_t nItemID);

    // ��λ�͸���
    int GetColumn() const;
    int GetGrid() const;
    void SetGrid(int nColumn, int nGrid);

    // ��Ʒ����
    unsigned short GetCount() const;
    void SetCount(unsigned short nCount, bool bCheckUsetimes = false);
    void ModifyCount(short nCount);

    // ʹ���Ƿ�ɾ��
    bool IsDeleteByUsed(int nCount);

    // ��Ʒʹ�ô���
    short GetLeftUseTimes() const;
    void SetLeftUseTimes(short nUseTimes);

    // �Ƿ����
    bool IsOverDue(int nCurTime) const;
    int GetDueTime() const;
    void SetDueTime(int nDueTime);
    void AddDuration(int nAddDueTime);

    // ��̬��й���ֵ
    int GetDynamicBoxRelatedValue() const;
    void SetDynamicBoxRelatedValue(int nRelatedValue);

    // ��ȡ�����Ե��ӵ���Ʒ����
    unsigned int GetLeftStackCount() const;

    //////////////////////////////////////////////////////////////////////////
    // ��Ʒ���ýӿ�
    //////////////////////////////////////////////////////////////////////////
    // ��ȡ��Ʒ����
    itemtype_t GetType() const;
    unsigned char GetAnType(int i) const;

    // �Ƿ���Ʒ
    bool IsExpandable() const;

    // �Ƿ�װ��
    bool IsEquip() const;

    // �Ƿ��·�
    bool IsCloth()const;

    // �Ƿ�������
    bool IsHorse() const ;

    // �Ƿ��ǽ���ָ
    bool IsWeddingRing() const;

    // �Ƿ��ǻ���
    bool IsBadge()const;

    // �Ƿ�����װ
    bool IsItemCloth_Type_Suit() const;

    // �Ƿ�����������䡢��̬����
    bool IsPackage() const;

    // ��������
    bool IsPacketExpandable() const;

    // ���������
    bool IsBoxExpandable() const;

    // ��̬����
    bool IsDynamicBoxExpandable() const;

    // ���ﵰ
    bool IsPetEgg() const;

    // �Ƿ��Ǳ�����Ʒ
    bool IsTransform();

    // ��ȡ��Ʒ����
    const char* GetName() const;

    // ��Ʒ���ѵ���Ŀ
    unsigned int GetMaxStackCount() const;

    //ʱ�䵥λ����
    int GetCoolDownTime() const;
    unsigned short GetCoolDownTag() const;

    // ��ȡVIP�ȼ�
    unsigned short GetVipLevelConfig() const;

    // ��ȡʹ�ô�������
    int GetUseTimesConfig() const;

    // ��ȡ����ʱ������
    int GetDueTimeConfig() const;

    // ��ȡ��������
    void GetGene(std::list<GeneInfo*> &listGene) const;

    // ��ȡ�Ա�����
    ESexType GetSexConfig() const;

    // �ȼ�Ҫ��
    int GetLevelConfig() const;

    // �ǿ���װ��Ч����
    bool IsCanAddEffectCloth();

    // ��ȡ��Ч����
    unsigned int GetEffectIDConfig() const;

protected:
    itemid_t        m_nItemID;                  // ��ƷΨһID��DB�洢id
    int             m_nColumn;                  // ��λ
    int             m_nGrid;                    // ���ӱ��
    int             m_nDueTime;                 // >0��ʾ����ʱ��㣬<=0��ʾ������Ч����Ʒ
    unsigned short  m_nCount;                   // ��Ʒ����
    short           m_nLeftUseTimes;            // ʣ��ʹ�ô�����-1��ʾ��������ʹ��
    int             m_nDynamicBoxRelatedValue;  // ��̬��й���ֵ

protected:
    const ItemConfig *m_pItemConfig;            // ��Ʒ��̬����
};


class ItemEquip : public ItemBase
{
public:
	ItemEquip(void);
	ItemEquip(const ItemConfig *pItemConfig);
	virtual ~ItemEquip(void);

public:
    virtual void InitFromDBItem( const DBItem & dbInfo );
    virtual void SaveToDBItem( DBItem &dbItem ) const;
    virtual void Encode(CParamPool * pPool) const;
    virtual void Decode(CParamPool * pPool);

	// ���ô�����ʾ
	virtual void OnEquip();
	virtual void UnEquip();

	// �Ƿ�װ��
	bool IsEquiped() const;

	// ��ȡ���ô���λ��
	unsigned char GetEquipConfigPos() const;

	// �Ƿ���Ҫ�Ϸ��Լ��
	bool RequireIntegrityCheck() const;

	// ��ȡ/������ЧID
	void SetEffectID(unsigned int nEffectID);
	unsigned int GetEffectID() const;

	// �Ƿ��Ǿ���Ч����
	bool IsOldEffectCloth();

    // �Ƿ�״̬�����仯
    bool IsStautsChange();

    // �Ƿ������õ�
    bool IsPerpetual();

protected:
	bool		        m_bEquiped;     // �Ƿ��Ѿ��������ϣ�����Ʒʱ�·�ʱ����unsigned char ��ʵ�ǵ�bool���С��,����Ʒʱ����ʱ,����ǵ�ǰλ��+1.
    bool                m_bInitStauts;
	unsigned int        m_nEffectID;    // ��ЧID
};



#endif //__ITEM_BASE_H__

