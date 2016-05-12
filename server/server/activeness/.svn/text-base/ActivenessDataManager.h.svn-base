/**
* Filename: ActivenessDataManager
* Description: 活跃度系统数据配置模块
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-05-14 17:40:23
*/

#ifndef _ACTIVENESS_DATA_MANAGER_H_
#define _ACTIVENESS_DATA_MANAGER_H_

#include "../../datastructure/DataStruct_Base.h"
#include "GameMsg_Activeness.h"

class ActivenessDataManager
{
public:
    ActivenessDataManager();
    ~ActivenessDataManager();

    static ActivenessDataManager &Instance();

public:
    void SetActiveness(const char *strName, const char *strDesc, const char *strStartTime, const char *strEndTime,
        const char *strMailSender, const char *strMailTitle, const char *strMailContent, const char *strVIPMailTitle,
        const char *strVIPMailContent);

    void AddActivenessTarget(int nTargetType, int nTargetValue, int nTargetScore, const char *strDesc);

    void AddActivenessReward(int nRequireScore, const char *strMaleItem, const char *strFemaleItem, int nMoney,
        int nBindMCoin, const char *strVIPMaleItem, const char *strVIPFemaleItem, int nVIPMoney, int nVIPBindMCoin);

    bool IsOpen() const;

    bool GetTarget(int nTargetType, ActivenessTargetConfig & targetConfig) const;

    bool GetReward(int nRewardIndex, ActivenessRewardConfig &reward) const;

    void GetTargetRewardConfigList(std::list<ActivenessRewardConfig> & listRewardConfig);

    void GetTargetConfigList(std::list<ActivenessTargetConfig> &listTarget);

public:

    // activeness system information
    std::string m_strActivenessName;
    std::string m_strActivenessDesc;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;

    std::string m_strMailSender;
    std::string m_strMailTitle;
    std::string m_strMailContent;
    std::string m_strVIPMailTitle;
    std::string m_strVIPMailContent;

    // target info, key: target type, value: ActivenessTarget
    std::map<int, ActivenessTargetConfig> m_mapTarget;

    // reward info, key: require score, value: ActivenessReward
    std::map<int, ActivenessRewardConfig> m_mapReward;
};


#endif //_ACTIVENESS_DATA_MANAGER_H_

