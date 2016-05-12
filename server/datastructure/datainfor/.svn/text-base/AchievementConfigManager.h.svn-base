/********************************************************************
    created:    2015-07-13
    filename:   DataStructure\datainfor\AchievementConfigManager.h
    author:     

    purpose:    对管成就配置数据进行管理
*********************************************************************/

#ifndef     __ACHIEVEMENTCONFIGMANAGER_H__
#define     __ACHIEVEMENTCONFIGMANAGER_H__

#include <map>
#include "AchievementConfig.h"

class AchievementConfigManager
{
public:
    AchievementConfigManager();
    ~AchievementConfigManager();

public:
    // 加载/保存配置
    bool LoadAchieveInfo(const char* pszFileName);
    bool Add(AchievementConfig *pAchievementInfo);
    bool SaveAchieveInfo(const char* pszFileName);

    // 获取成就信息
    AchievementConfig * GetByID(unsigned short nAchievementID) const;

    // 获取成就数目
    unsigned short GetNumber() const;

    // 销毁
    void Destroy();

public:
    map<unsigned short, AchievementConfig *>               m_mapAchievement;
    map<unsigned short, std::list<AchievementConfig *> >   m_mapAchievementBySeriesID;
};


#endif


//end file


