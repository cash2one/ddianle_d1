/************************************************************************
 * HandbookCfg.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-15 10:22:20
 ***********************************************************************/

#ifndef __HANDBOOK_CFG_H__
#define __HANDBOOK_CFG_H__

#include "../../server/Macros/Macros.h"
#include <list>
#include <map>
#include <string>
#include "../../datastructure/DataStruct_Base.h"

enum EClothEffectLevel
{
    EClothEffectLevel_1 = 1,    // 普通
    EClothEffectLevel_2,        // 星光
    EClothEffectLevel_3,        // 多彩
    EClothEffectLevel_4,        // 流光
    EClothEffectLevel_5,        // 绚丽
    EClothEffectLevel_6,        // 梦幻
};

class CFileStream;
class CHandboolCostumeCfg
{
public:
    CHandboolCostumeCfg();
    virtual ~CHandboolCostumeCfg();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_nItemId, ItemId);
    CLASS_PROPERTY_READONLY_BY_REF(std::list<unsigned int>, m_listTag, Tag);                     //标签
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strWayOfGain, WayOfGain);                      //获取途径
    CLASS_PROPERTY_READONLY_BY_REF(std::list<unsigned int>, m_listUsageSuggestion, UsageSuggestion); //完成特殊需求的参数，一般 填写 itemid
    CLASS_PROPERTY_READONLY(unsigned int, m_nEffectLevel, EffectLevel);

    unsigned int GetClothAttr(EClothAttrType eType);

public:
    std::map<EClothAttrType, unsigned int>          m_mapClothAttr;
};

class CHandbookCfgMgr
{
protected:
    CHandbookCfgMgr();

public:
    CLASS_INSTANCE_FUNC(CHandbookCfgMgr)
    virtual ~CHandbookCfgMgr();

    void Initialize();

    void GetAllCostumeIDList( std::list<int> &rListID ) const;
    void GetTagList(int nCostumeID, std::list<unsigned int> &rParams ) const;
    void GetWayOfGain(int nCostumeID, std::string &rStrWayOfGain) const;
    void GetUsageSuggestion(int nCostumeID, std::list<unsigned int> &rParams ) const;
    int  GetEffectLevel(int nCostumeID) const;
    unsigned int GetEffectLevelScore(itemtype_t nItemType, EItemClothType eClothType, bool bIsBadge) const;
    bool IsEffectCloth(itemtype_t nItemType) const;

    // 获取服饰基础属性
    unsigned int GetClothBaseAttr(itemtype_t nItemType, unsigned int nEffectID, EClothAttrType eType);

    // 获取光效石加成属性
    // unsigned int nGroupID 组别ID
    unsigned int GetClothStoneAttr(itemtype_t nItemType, unsigned int nGroupID);

    // 获取服饰属性的组别
    unsigned int GetClothAttrGroupID(EClothAttrType eType);

protected:
    void LoadConfigFromFile();
    void LoadCostumeConfigForm( CFileStream &rFileStream );
    void LoadAttrGroupIDConfig(CFileStream &rFileStream);
    void LoadEffectStoneConfig(CFileStream &rFileStream);
    unsigned int GetClothLevelScore(int nLevel, bool bIsSuit) const;
    unsigned int GetBadgeLevelScore(int nLevel) const;

    // 初始化光效积分配置
    void InitializeLevelScore();

private:
    std::map<int , CHandboolCostumeCfg> m_MapCostumeCfg;
    std::map<int, int>                  m_mapClothLevelScore;
    std::map<int, int>                  m_mapBadgeLevelScore;
    std::map<EClothAttrType, unsigned int>  m_mapAttrGroupID;
    std::map<itemtype_t, std::map<unsigned int, unsigned int> > m_mapEffectStoneAttr;
};

#endif  //__HANDBOOK_CFG_H__

