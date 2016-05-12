/*
 * BuyItemGotMedalMgr.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-8-31 20:17:00
 */

#ifndef __BUYITEMGOTMEDALMGR_H__
#define __BUYITEMGOTMEDALMGR_H__

#include "../Macros/Macros.h"
#include <list>
#include <map>
#include <string>

class BuyItemGotMedalRewards
{
public:
    BuyItemGotMedalRewards() {}
    ~BuyItemGotMedalRewards() {}

    CLASS_PROPERTY(unsigned int, m_nItemType, ItemType)
    CLASS_PROPERTY(unsigned short, m_nItemCount, ItemCount)
    CLASS_PROPERTY_BY_REF(std::list<int>, m_listMaleReward, MaleReward)
    CLASS_PROPERTY_BY_REF(std::list<int>, m_listFemaleReward, FemaleReward)
};

/**
* BuyItemGotMedalMgr:
* 
*/

class BuyItemGotMedalMgr
{
private:
    BuyItemGotMedalMgr();
    virtual ~BuyItemGotMedalMgr() {}

public:
    CLASS_INSTANCE_FUNC(BuyItemGotMedalMgr)

    void Initialize();

    void OnUpdate( unsigned int nNow );

    bool IsValidIndex( unsigned char nIndex ) const;

    bool CanGetRewards( unsigned char nIndex, unsigned short nCurrentCount ) const;
    void GetRewards( unsigned char nIndex, unsigned char nSex, std::list<int> &rReward ) const;

    const std::map<unsigned char, BuyItemGotMedalRewards>& GetAllRewardsInfo() const { return m_mapRewardInfos; }

    void InitInfoByLua( bool bExhibit, int nWeight, unsigned int nBeginTime, unsigned int nEndTime, 
        unsigned int nOpenTime, unsigned int nCloseTime, const std::string &rTitle, const std::string &rContent, 
        const std::string &rSmallImage, const std::string &rBigImage );

    void AddRewardInfo( unsigned char nIndex, unsigned int nItemType, unsigned short nItemCount, 
        const std::list<int> &rMaleReward, const std::list<int> &rFemaleReward );

protected:
    void CheckValid( unsigned int nNow );

    CLASS_BOOL_PROPERTY_READONLY(m_bOpening, Opening)
    CLASS_BOOL_PROPERTY_READONLY(m_bValid, Valid)
    CLASS_BOOL_PROPERTY_READONLY(m_bExhibit, Exhibit)

    CLASS_PROPERTY_READONLY(int, m_nWeight, Weight)
    CLASS_PROPERTY_READONLY(unsigned int, m_nBeginTime, BeginTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nEndTime, EndTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nOpenTime, OpenTime)
    CLASS_PROPERTY_READONLY(unsigned int, m_nCloseTime, CloseTime)

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strTitle, Title)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strContent, Content)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strSmallImage, SmallImage)
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strBigImage, BigImage)

private:
    std::map<unsigned char, BuyItemGotMedalRewards> m_mapRewardInfos;
};

#endif // __BUYITEMGOTMEDALMGR_H__

