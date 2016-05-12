#ifndef __ITEM_BASE_H__
#define __ITEM_BASE_H__

#include "datainfor/ItemConfig.h"
#include "DataStruct_Base.h"
#include "Macro_Define.h"

class CParamPool;
class ItemFactory;

/************************************************************************/
/* ItemBase: 玩家的物品实例                                               */
/* 该类的构造函数和析构函数都是受保护方法，只能使用ItemFactory实例化和销毁，      */
/* 而ItemFactory也只能在物品模块内部使用，这样可以更好地控制物品对象的生命期，     */
/* 降低物品内存管理复杂度，减少物品内存泄漏。                                  */
/************************************************************************/

class ItemBase
{
protected:
    // 物品
    ItemBase();
    ItemBase(const ItemConfig *pItemConfig);
    virtual ~ItemBase();

    friend class ItemFactory;

public:
    // 物品实例初始化接口（使用DBItem对象初始化）
    virtual void InitFromDBItem(const DBItem &dbItem);
    virtual void SaveToDBItem( DBItem &dbItem ) const;

    // 物品使用接口
    virtual int OnUse();
    virtual int OnBatchUse(int nCount);

    // 是否是有效物品
    virtual bool IsValid() const;

    // 物品序列化
    virtual void Encode(CParamPool *pool) const;
    virtual void Decode(CParamPool * pPool);

    // 获取和设置物品ID
    itemid_t GetItemID() const;
    void SetItemID(itemid_t nItemID);

    // 栏位和格子
    int GetColumn() const;
    int GetGrid() const;
    void SetGrid(int nColumn, int nGrid);

    // 物品数量
    unsigned short GetCount() const;
    void SetCount(unsigned short nCount, bool bCheckUsetimes = false);
    void ModifyCount(short nCount);

    // 使用是否删除
    bool IsDeleteByUsed(int nCount);

    // 物品使用次数
    short GetLeftUseTimes() const;
    void SetLeftUseTimes(short nUseTimes);

    // 是否过期
    bool IsOverDue(int nCurTime) const;
    int GetDueTime() const;
    void SetDueTime(int nDueTime);
    void AddDuration(int nAddDueTime);

    // 动态礼盒关联值
    int GetDynamicBoxRelatedValue() const;
    void SetDynamicBoxRelatedValue(int nRelatedValue);

    // 获取还可以叠加的物品数量
    unsigned int GetLeftStackCount() const;

    //////////////////////////////////////////////////////////////////////////
    // 物品配置接口
    //////////////////////////////////////////////////////////////////////////
    // 获取物品类型
    itemtype_t GetType() const;
    unsigned char GetAnType(int i) const;

    // 是否物品
    bool IsExpandable() const;

    // 是否装备
    bool IsEquip() const;

    // 是否衣服
    bool IsCloth()const;

    // 是否是坐骑
    bool IsHorse() const ;

    // 是否是结婚戒指
    bool IsWeddingRing() const;

    // 是否是徽章
    bool IsBadge()const;

    // 是否是套装
    bool IsItemCloth_Type_Suit() const;

    // 是否是礼包、宝箱、动态宝箱
    bool IsPackage() const;

    // 礼包类道具
    bool IsPacketExpandable() const;

    // 宝箱类道具
    bool IsBoxExpandable() const;

    // 动态宝箱
    bool IsDynamicBoxExpandable() const;

    // 宠物蛋
    bool IsPetEgg() const;

    // 是否是变身物品
    bool IsTransform();

    // 获取物品名字
    const char* GetName() const;

    // 物品最大堆叠数目
    unsigned int GetMaxStackCount() const;

    //时间单位是秒
    int GetCoolDownTime() const;
    unsigned short GetCoolDownTag() const;

    // 获取VIP等级
    unsigned short GetVipLevelConfig() const;

    // 获取使用次数配置
    int GetUseTimesConfig() const;

    // 获取过期时间配置
    int GetDueTimeConfig() const;

    // 获取基因配置
    void GetGene(std::list<GeneInfo*> &listGene) const;

    // 获取性别配置
    ESexType GetSexConfig() const;

    // 等级要求
    int GetLevelConfig() const;

    // 是可以装特效服饰
    bool IsCanAddEffectCloth();

    // 获取特效配置
    unsigned int GetEffectIDConfig() const;

protected:
    itemid_t        m_nItemID;                  // 物品唯一ID，DB存储id
    int             m_nColumn;                  // 栏位
    int             m_nGrid;                    // 格子编号
    int             m_nDueTime;                 // >0表示到期时间点，<=0表示永久有效的物品
    unsigned short  m_nCount;                   // 物品数量
    short           m_nLeftUseTimes;            // 剩余使用次数，-1表示可以无限使用
    int             m_nDynamicBoxRelatedValue;  // 动态礼盒关联值

protected:
    const ItemConfig *m_pItemConfig;            // 物品静态数据
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

	// 设置穿戴标示
	virtual void OnEquip();
	virtual void UnEquip();

	// 是否装备
	bool IsEquiped() const;

	// 获取配置穿戴位置
	unsigned char GetEquipConfigPos() const;

	// 是否需要合法性检查
	bool RequireIntegrityCheck() const;

	// 获取/设置特效ID
	void SetEffectID(unsigned int nEffectID);
	unsigned int GetEffectID() const;

	// 是否是旧特效服饰
	bool IsOldEffectCloth();

    // 是否状态发生变化
    bool IsStautsChange();

    // 是否是永久的
    bool IsPerpetual();

protected:
	bool		        m_bEquiped;     // 是否已经穿在身上，当物品时衣服时别看是unsigned char 其实是当bool大家小心,当物品时徽章时,存的是当前位置+1.
    bool                m_bInitStauts;
	unsigned int        m_nEffectID;    // 特效ID
};



#endif //__ITEM_BASE_H__

