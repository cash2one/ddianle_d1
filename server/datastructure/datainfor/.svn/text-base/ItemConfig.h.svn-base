/********************************************************************
	created:	2010/12/06
	filename: 	DataStructure\uintinfo.h
	author:		meijiuhua
	
	purpose:	编辑器uint的数据结构
				这个数据同时放在客户端（暂定）
*********************************************************************/

#ifndef __ITEMCONFIG_H__
#define __ITEMCONFIG_H__
#ifdef WIN32
#pragma warning(disable:4018)
#pragma warning(disable:4100)
#endif

#include "ItemType.h"
#include "../Macro_Define.h"
#include <string>
#include <vector>
#include <list>
#include "../DataStruct_Base.h"

#ifndef WIN32
typedef long long         __int64;
#endif


class CFileStream;

class GeneInfo
{
public:
    GeneInfo(void);
    virtual ~GeneInfo();
    GeneInfo& operator =(const GeneInfo& rhs);

public:
    unsigned short      m_nGeneID;          //id
    int                 m_nParam1;          //参数
    int                 m_nParam2;          //参数
    std::string         m_strParam;         //参数
};

class ItemConfig
{
public:
    ItemConfig(void);
    ~ItemConfig(void);

public:
    itemtype_t          m_nType;	
    string              m_strName;
    unsigned char       m_anType[3];        //[主类型][子类型1][子类型2]
    ESexType            m_SexNeed;          //性别需求[无][男][女]
    int                 m_nDueTime;         //有效时间，有效的秒数 如一小时有效期，则该字段大小为60*60秒，无时效该字段为-1或者0
    short               m_nUseTimes;        //使用次数 -1表示无限使用 0不能使用
    unsigned short      m_nNeedLevel;       //人物级别需求，与角色属性中的级别对应
    bool                m_bUnique;          //是否只能拥有一个
    unsigned short      m_nMaxStackCount;   //放在一起叠加的最大数量
    unsigned short      m_nCoolDownTag;     //cooldown标志
    int                 m_nCooldownTime;    //cooldown时间
    std::string         m_strIcon;          //物品ICON
    std::string         m_strAtlas;         //物品Atlas
    std::string         m_strAnimation;     //动画名称
    unsigned int        m_nClothEffectID;   //特效ID
    unsigned char       m_nClothColor;      //衣服颜色
    unsigned short      m_nVIP;             // VIP标识，0表示无限制，1表示VIP1级
    int                 m_nIntimacy;        // 获取所需亲密度
    string              m_strIntro;         //tips
    bool                m_bIsFresher;       //是否是新手包道具
    list<GeneInfo *>    m_listGene;
    bool                m_bTransform;       //是否变身道具
    bool                m_bOldEffectCloth;  //是否是旧特效衣服

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    // 物品类型
    itemtype_t GetType() const;

    // 物品背包栏位
    EItemColumn GetItemColumnByType();

    // 是否是衣服
    bool IsEquip() const;

    // 是否是消耗品
    bool IsExpandable() const;

    //服饰
    bool IsCloth() const;

    //徽章
    bool IsBadge() const;

    //坐骑
    bool IsHorse() const;

    //功能类道具
    bool IsFunctionExpandable() const;

    //社交类道具
    bool IsSocialExpandable() const;

    //增益类道具
    bool IsAdditionExpandable() const;

    //是否是礼包、宝箱、动态宝箱
    bool IsPackage() const;

    //礼包类道具
    bool IsPacketExpandable() const;

    //宝箱类道具
    bool IsBoxExpandable() const;

    //任务类道具
    bool IsTransFormExpandable() const;

    //动态宝箱类道具
    bool IsDynamicBoxExpandable() const;

    //宠物蛋
    bool IsPetEgg()  const;

    //衣服穿戴位置
    unsigned char GetClothPos() const;

    //是否是婚戒
    bool IsBadge_Type_WeddingRing() const;

    //是否是皮肤
    bool IsItemCloth_Type_Skin() const;

    //是否是套装
    bool IsItemCloth_Type_Suit() const;

    //唯一物品
    bool IsUnique() const;

    // 是否是特效石
    bool IsEffectStone() const;
};

class CSkinInfo
{
public:
    CSkinInfo(void);
    ~CSkinInfo(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    itemtype_t		    m_nType;
    string				m_strName;
    unsigned char		m_cR;
    unsigned char		m_cG;
    unsigned char		m_cB;
    unsigned char		m_cA;
};

class CItemAniInfo
{
public:
    CItemAniInfo(void);
    ~CItemAniInfo(void);

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned short		m_nType;
    unsigned char		m_nMoveState;
    string				m_strPlayerIdleAni;
    string				m_strPlayerMoveAni;
    string				m_strWingMoveAni;
};

class CItemTransformInfo
{
public:
    CItemTransformInfo();
    ~CItemTransformInfo();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);

public:
    unsigned int m_nId;
    string m_strName;
    itemtype_t m_szItemCloth[eItemClothType_Max];
};

class CTransformAniInfo
{
public:
    CTransformAniInfo();
    ~CTransformAniInfo(){}

public:
    bool Save(CFileStream &file);

public:
    int m_nTransformId;				// 变身Id
    string m_strWomanAniName;		// 男动画名
    string m_strManAniName;			// 女动画名
};

#endif//__ITEMINFO_H__


//end file


