/** DanceGroupFairyLandCfg.h
* Description:
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: lht
* Create time: 2016-02-18 11:48:00
*/


#ifndef __DANCEGROUP_FAIRYLADN_CFG_H__
#define __DANCEGROUP_FAIRYLADN_CFG_H__


#include "../../server/Macros/Macros.h"

#include <list>
#include <string>

class CFileStream;

class SDancerScore
{
public:
    SDancerScore();
    ~SDancerScore();
public:
    int m_nRoleId;
    int m_nScore;
    int m_nVipBuf;
    int m_nGrade;           // 评级
};

class CDropPoolCfg
{
public:
    CDropPoolCfg();
    virtual ~CDropPoolCfg();

public:
    bool loadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_DropId, nDropId);

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem1, strItme_1);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem2, strItme_2);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem3, strItme_3);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem4, strItme_4);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem5, strItme_5);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem6, strItme_6);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem7, strItme_7);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem8, strItme_8);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem9, strItme_9);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strItem10, strItme_10);
};

class CDanceGroupFairyLandCfg
{
public:
    CDanceGroupFairyLandCfg();
    virtual ~CDanceGroupFairyLandCfg();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_nChapterLevel, nChapterLevel);
    CLASS_PROPERTY_READONLY(int, m_nPassScore, nPassScore);
    CLASS_PROPERTY_READONLY(int, m_nMusicId, nMusicId);
    CLASS_PROPERTY_READONLY(int, m_nDifficultyLevel, nDifficultyLevel);
    CLASS_PROPERTY_READONLY(int, m_nSceneId, nSceneId);
    CLASS_PROPERTY_READONLY(int, m_nDanceType, nDanceType);                 //舞蹈类型
    

    CLASS_PROPERTY_READONLY(int, m_nJoinRewardBindMB, nJoinRewardBindMB);   //参与bindMB奖励
    CLASS_PROPERTY_READONLY(int, m_nJoinRewardMoney, nJoinRewardMoney);    //参与金券奖励
    CLASS_PROPERTY_READONLY(int, m_nJoinRewardMB, nJoinRewardMB);           //参与MB奖励
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMaleJoinItems, strMaleJoinItems);   //参与道具奖励
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strFemaleJoinItems, strFemaleJoinItems);   //参与道具奖励

    CLASS_PROPERTY_READONLY(int, m_nPassRewardBindMB, nPassRewardBindMB);   //过关bindMB奖励
    CLASS_PROPERTY_READONLY(int, m_nPassRewardMoney, nPassRewardMoney);    //过关金券奖励
    CLASS_PROPERTY_READONLY(int, m_nPassRewardMB, nPassRewardMB);           //过关MB奖励
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMalePassItems, strMalePassItems);   //过关道具奖励
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strFemalePassItems, strFemalePassItems);   //过关道具奖励

    CLASS_PROPERTY_READONLY(int, m_nMaleDropPoolId, nMaleDropPoolId);           //过关掉落库id
    CLASS_PROPERTY_READONLY(int, m_nFemaleDropPoolId, nFemaleDropPoolId);           //过关掉落库id
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMaleShowItems, strMaleShowItems);   //关卡用于显示的奖励（只看不发）
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strFemaleShowItems, strFemaleShowItems);   //关卡用于显示的奖励（只看不发）
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strStorageBoxReward, strStorageBoxReward);   //储物箱奖励
};

class CDanceGroupFairyLandRankCfg
{
public:
    CDanceGroupFairyLandRankCfg();
    virtual ~CDanceGroupFairyLandRankCfg();
public:
    bool LoadInfoFromFile(CFileStream &rFileStream);
    
    CLASS_PROPERTY_READONLY(int, m_nRankId, nRankId);           //排行榜名词
    CLASS_PROPERTY_READONLY(int, m_nBindMb, nBindMb);   //bindMB奖励
    CLASS_PROPERTY_READONLY(int, m_nMoney, nMoney);    //金券奖励

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strMaleRewardItem, strMaleRewardItem);
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strFemaleRewardItem, strFemaleRewardItem);
public:
};
#endif