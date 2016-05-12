/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\EffectConfigManager.h
    author:     

    purpose:    对特效配置数据进行管理
*********************************************************************/

#ifndef __EFFECTCONFIG_MANAGER_H__
#define __EFFECTCONFIG_MANAGER_H__

#include <string>
#include <list>
#include <vector>
#include <map>
#include "../DataStruct_Base.h"
#include "EffectConfig.h"

class CFileStream;

// 特效配置管理
class EffectManager
{
public:
    EffectManager();
    ~EffectManager();

public:
    itemtype_t GetHornItemBySpeckType(char type);
    bool Load(std::string& fileName);
    bool Save(std::string& fileName);
    void Initialize();

public:
    void Destroy();
    CDanceGroupBadgeConfig * GetDanceGroupBadgeByID(unsigned short unId);
    CDanceGroupBadgeEffectConfig* GetDanceGroupBadgeEffectByID(unsigned short unId);
    std::map<unsigned short, CDanceGroupBadgeConfig*> & GetDanceGroupBageList();
    std::map<unsigned short, CDanceGroupBadgeEffectConfig*> &GetDanceGroupBadgeEffectList();

    // 获取服饰特效
    ClothEffectConfig * GetClothEffectConfig(unsigned int nEffectID);

    // 获取下一级特效
    ClothEffectConfig * GetNextLevelEffectConfig(unsigned int nEffectID);

    // 获取衣服特效升级配置
    ClothEffectUpgrade * GetClothEffectUpgradeConfig(unsigned int nEffectID);

    // 获取服饰特效由特效石
    ClothEffectConfig * GetClothEffectConfigByItem(itemtype_t nItemType);
    unsigned int GetClothEffectIDByItem(itemtype_t nItemType);

    // 生成衣服部位对应
    void LoadClothEffectPos(ClothEffectConfig * pEffectCnfig);
    
    // 获取两个等级进特效ID
    void GetBetweenEffectID(unsigned short nSuitID, unsigned short nClothPos, unsigned int nLastEffectID, 
            unsigned int nNewEffectID, std::set<unsigned int> & setEffectID);

    // 获取最大等级的特效ID
    unsigned int GetMaxLevelEffectID(unsigned short nSuitID, unsigned short nClothPos);

    // 获取套装特效个数
    unsigned int GetSuitEffectNum(unsigned short nSuitID);
    unsigned int GetSuitEffectNum(unsigned short nSuitID, unsigned short nPos);

    // 获取套装部位
    void GetSuitClothPos(unsigned short nSuitID, std::set<EItemClothType> & setClothPos);

    // 获取服饰特效的积分
    unsigned int GetClothEffectScore(unsigned int nEffectID, EItemClothType eClothType);

private:
    std::map<char,HornEffectConfig>                            m_HornList;
    std::map<unsigned short,EffectBaseConfig>                  m_EffectInfoList;
    std::map<unsigned short,ChatPapawConfig>                   m_ChatPapawList;
    std::map<unsigned short,RoomBadgeConfig>                   m_RoomBadgeList;
    std::map<unsigned short, CDanceGroupBadgeConfig*>          m_mDGBadge;//舞团团徽徽章
    std::map<unsigned short, CDanceGroupBadgeEffectConfig*>    m_mDGEffect;//舞团团徽特效
    std::map<unsigned short, CWeddingRingEffectConfig*>        m_mWREffect;//婚姻戒指特效
    std::map<unsigned int,ClothEffectConfig*>                  m_mapClothEffect;        // 服饰特效
    std::map<unsigned int,ClothEffectUpgrade*>                 m_mapClothEffectUpgrade; // 服饰特效升级
    std::map<itemtype_t,unsigned int>                          m_mapClothEffectItem;    // 光效物品对应的光效ID
    std::map<unsigned short, ClothEffectPos*>                  m_mapClothEffectPos;     // 服饰部位对应的特效ID
    std::map<unsigned short, std::map<EItemClothType, int> >   m_mapClothEffectLevelScore;  // 服饰等级对应积分
};


#endif//__ITEMTYPE_H__

