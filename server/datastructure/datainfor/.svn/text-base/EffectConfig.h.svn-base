/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\EffectConfig.h
    author:     

    purpose:   GeneEffect.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef __EFFECTCONFIG_H__
#define __EFFECTCONFIG_H__

#include <string>
#include <list>
#include <vector>
#include <map>
#include "../DataStruct_Base.h"

class CFileStream;

//特效信息
class EffectBaseConfig
{
public :
    EffectBaseConfig(){};
    ~EffectBaseConfig(){};

public :
    void Save(CFileStream&file);
    void Load(CFileStream&file);

public :
    unsigned short  m_nEffectID ;
    std::string     m_EffectName ;
    unsigned short  m_nboneIndex ;
    int             m_nParam1 ;
    int             m_nParam2 ;
};

//聊天泡泡特效
class ChatPapawConfig
{
public:
    ChatPapawConfig(){};
    ~ChatPapawConfig(){};

public :
    void Save(CFileStream&file) ;
    void Load(CFileStream& file);

public:
    unsigned short m_PapawID;
    std::string m_BGAtlasName;
    std::string m_BGSpriteName;
    int m_ColorR;
    int m_ColorG;
    int m_ColorB;
    int m_ColorA;
};

//房间徽章特效
class RoomBadgeConfig
{
public:
    RoomBadgeConfig(){};
    ~RoomBadgeConfig(){};

public:
    unsigned short m_RoomBadgeID;
    std::string m_ForegroundAtalasName;
    std::string m_ForegroundSpriteName;
    std::string m_BackgroundAtalasName;
    std::string m_BackgroundSpriteName;
    std::string m_MiShiDiAtalasName;
    std::string m_MiShiDiSpriteName;
    std::string m_RoomBackgroundAtalasName;
    std::string m_RoomBackgroundSpriteName;

public :
    void Save(CFileStream&file);
    void Load(CFileStream&file);
};

//////////////////////////////////////////DanceGroup(舞团)
//舞团团徽Icon
class CDanceGroupBadgeConfig
{
public:
    CDanceGroupBadgeConfig(){}
    ~CDanceGroupBadgeConfig(){}

public:
    void Save( CFileStream &file );
    bool Load( CFileStream& file );

public:
    unsigned short nId;
    std::string m_tuanhuiBadgeName; //name
    std::string m_tuanhuiBadgeSpriteName;//sprite
    std::string m_tuanhuiBadgeAtlasName;//atlas
    unsigned short m_nUnlockLevel; //解锁等级

    //解锁消耗资源
    int nParam1;
    int nParam2;
    int nParam3;

    //使用消耗资源
    int nParam4;
    int nParam5;
    int nParam6;

    //每天消耗资源
    int nParam7;
    int nParam8;
    int nParam9;
};

//舞团团徽特效
class CDanceGroupBadgeEffectConfig
{
public:
    CDanceGroupBadgeEffectConfig(){}
    ~CDanceGroupBadgeEffectConfig(){}

public:
    void Save( CFileStream &file );
    bool Load( CFileStream& file );

public:
    unsigned short nId;
    std::string m_tuanhuiEffectName;
    std::string m_tuanhuiEffectSpriteName;
    std::string m_tuanhuiEffectAtlasName;

    //解锁消耗
    int nParam1;
    int nParam2;
    int nParam3;

    //使用消耗
    int nParam4; 
    int nParam5;
    int nParam6;

    //每天使用消耗
    int nParam7;
    int nParam8;
    int nParam9;
};

//婚姻戒指特效
class CWeddingRingEffectConfig
{
public:
    CWeddingRingEffectConfig();
    ~CWeddingRingEffectConfig();

public:
    unsigned int m_nEffectID;
    std::string m_strEffectName;

public:
    void Save(CFileStream& file);
    bool Load(CFileStream& file);
};

// 结婚场景特效
class WeddingSceneEffectConfig
{
public:
    WeddingSceneEffectConfig();
    virtual ~WeddingSceneEffectConfig();

public:
    void Save(CFileStream & file) ;
    void Load(CFileStream & file);

public:
    unsigned int        m_nEffectID;             // 特效ID
    std::string         m_strEffectName;        // 特效名字
    unsigned int        m_nBlessingValue;       // 祝福值
};


// 大喇叭特效
class HornEffectConfig
{
public:
    HornEffectConfig(){};
    ~HornEffectConfig(){};

public :
    void Save(CFileStream&file) ;
    void Load(CFileStream& file);

public:
    char                m_HornType;
    itemtype_t          m_HornItemID;
    std::string         m_HornBtnName;
    std::string         m_HornEffectName;
};

// 服饰特效
class ClothEffectConfig
{
public:
    ClothEffectConfig();
    virtual ~ClothEffectConfig();

public:
    void Save(CFileStream & file);
    void Load(CFileStream & file);

public:
    unsigned int        m_nEffectID;                // 特效ID
    unsigned short      m_nSuitID;                  // 套装ID
    std::string         m_strEffectName;            // 特效名称
    std::string         m_strEffectResource;        // 特效资源
    unsigned short      m_nLevel;                   // 特效等级
    unsigned short      m_nClothPos;                // 对应衣服部位
    itemtype_t          m_nRelatedItem;             // 对应的光效石
    unsigned int        m_nCostCount;               // 对应光效石消耗数量
};

// 服饰特效升级配置
class ClothEffectUpgrade
{
public:
    ClothEffectUpgrade();
    virtual ~ClothEffectUpgrade();

public:
    void Save(CFileStream & file);
    void Load(CFileStream & file);

public:
    unsigned int        m_nEffectID;                // 特效ID
    unsigned int        m_nUpgradeID;               // 特效升级后ID
    std::list<CItem>    m_listCostItem;             // 特效升级消耗物品
    unsigned short      m_nCostMoneyType;           // 特效升级消耗金钱类型
    unsigned int        m_nCostMoneyNum;            // 特效升级消耗数目
    std::list<CItem>    m_listDiscountCostItem;     // 打折后特效升级消耗物品
    unsigned short      m_nDiscountCostMoneyType;   // 打折后特效升级消耗金钱类型
    unsigned int        m_nDiscountCostMoneyNum;    // 打折后特效升级消耗数目
};

// 服饰组合特效
class ClothCombinationalEffectConfig
{
public:
    ClothCombinationalEffectConfig();
    virtual ~ClothCombinationalEffectConfig();

public:
    void Save(CFileStream & file) ;
    void Load(CFileStream & file);

public:
    unsigned int                m_nCombinationID;           // 组合ID
    std::string                 m_strName;                  // 组合名称
    std::string                 m_strResourceName;          // 资源名称
    std::list<unsigned int>     m_listClothEffectID;        // 组合成员特效ID
};

// 服饰特效套装
class ClothEffectSuit
{
public:
    ClothEffectSuit();
    virtual ~ClothEffectSuit();

public:
    void Save(CFileStream & file) ;
    void Load(CFileStream & file);

public:
    unsigned short      m_nSuitID;
    std::string         m_strName;
    std::string         m_strDesc;
    std::string         m_strIcon;
    std::string         m_strAtlas;
};

// 服饰特效部位对应的特效ID
class ClothEffectPos
{
public:
    ClothEffectPos();
    virtual ~ClothEffectPos();

public:
    // 加载数据
    void LoadData(unsigned short nPos, unsigned int nEffectID);

    // 获取两特效之间的特效ID
    void GetBetweenEffectID(unsigned short nPos, unsigned int nLastEffectID, unsigned int nNewEffectID, std::set<unsigned int> & setEffectID);

    // 获取部位最大特下ID
    unsigned int GetMaxLevelEffectID(unsigned short nPos);

    // 获取套装总特效个数
    unsigned int GetEffectNum();
    unsigned int GetEffectNum(unsigned short nPos);

    // 获取衣服部位
    void GetClothPos(std::set<EItemClothType> & setClothPos);

public:
    std::map<unsigned short, std::set<unsigned int> >       m_mapPosEffectID;           // 部位对应的特效ID
};

#endif//__ITEMTYPE_H__

