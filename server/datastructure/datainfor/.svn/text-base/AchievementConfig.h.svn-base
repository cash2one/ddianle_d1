/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\AmuseRoomConfig.h
    author:     

    purpose:    achievement.bytes配置中相对应的数据结构
*********************************************************************/

#ifndef     __ACHIEVEMENT_H__
#define     __ACHIEVEMENT_H__

#include <string>
#include <list>
#include "../DataStruct_Base.h"

class CFileStream;

// 成就物品奖励
class AchievementItemReward
{
public:
    AchievementItemReward(itemtype_t nItemID = 0, unsigned short nItemCount = 0);
    ~AchievementItemReward();

public:
    void Read(CFileStream &file);
    void Write(CFileStream  &file);

    bool IsValid() const;
    itemtype_t GetItemID();
    unsigned short GetItemCount();

public:
    itemtype_t          m_nItemID;
    unsigned short      m_nItemCount;
};

//成就配置数据结构
class AchievementConfig
{
public:
    AchievementConfig();
    ~AchievementConfig();

public:
    unsigned short      m_nAchievementID;       // 成就编号
    unsigned short      m_nClassID;             // 成就分类编号
    unsigned short      m_nSeriesID;            // 系列编号
    std::string         m_strAchievementName;   // 成就名称
    unsigned char       m_nLevel;               // 层次
    unsigned short      m_nTitleID;             // 成就称号
    std::string         m_strDetail;            // 成就介绍
    std::string         m_strIcon;              // Icon
    unsigned short      m_nHandlerID;           // 成就处理器ID
    unsigned int        m_nCount;               // 达标的数值
    unsigned int        m_nScore;               // 成就积分
    unsigned int        m_nRewardMoney;         // 金钱奖励
    std::list<AchievementItemReward>   m_listRewardItem;  // 成就物品奖励
    bool                m_bNotify;              // 是否全服公告

public:
    // 获取成就ID
    unsigned short	GetAchievementID();

    // 获取成就名字
    const char * GetAchievementName() const;

    // 获取成就称号ID
    unsigned short GetRewardTitleID() const;

    // 获取成就介绍
    const char * GetDetail() const;

    // 获取成就积分
    unsigned int GetScore();

    // 获取金钱奖励
    unsigned int GetRewardMoney();

    // 获取成就处理器ID
    unsigned short GetHanlerID();

    // 获取成就分类编号
    unsigned short GetClassID();

    // 获取系列编号
    unsigned short GetSeriesID();

    // 获取达成的数值
    unsigned int GetRequireValue();

    // 获取成就奖励物品
    std::list<AchievementItemReward> & GetRewardItemList();

public:
    bool Load(CFileStream&file);
    bool Save(CFileStream&file);
};

#endif



//end file


