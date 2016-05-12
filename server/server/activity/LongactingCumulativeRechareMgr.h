/** MallShareMgr.h
* Description: 长效累冲数据管理器
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: yangzhifu
* Create time: 2015-7-22 21:16:28
*/
#ifndef __LONGACTINGCUMULATIVERECHARGE_DATA_MGR_H__
#define __LONGACTINGCUMULATIVERECHARGE_DATA_MGR_H__

#include "ActivityDataStruct.h"

class LongactingCumulativeRechargeMgr : public GameMsg_Processor
{
public:
    LongactingCumulativeRechargeMgr();
    ~LongactingCumulativeRechargeMgr();

public:
    static LongactingCumulativeRechargeMgr & Instance();

    void Init();

    void RegisterMessage();

public:
    // 获取长效累冲配置
    void GetLongactingCumulativeRechargeConfig(std::list<LongactingCumulativeRechargeConfig> & listConfig);
    
    // 加载奖励配置
    void AddLongactingCumulativeRechargeConfig(const LongactingCumulativeRechargeConfig & config);

    // 加载邮件信息
    void AddLongactingCumulativeRechargeMail(const std::string & strMailTtle, const std::string & strMailContent);

    // 获取下一个配置
    const LongactingCumulativeRechargeConfig * GetNextConfig(unsigned int nCurrentIndex);

    // 获取配置
    const LongactingCumulativeRechargeConfig * GetCurrentConfig(unsigned int nCurrentIndex);

public:
    // 对sql返回处理
    void OnGetRewardCallBack(QueryBase & rQuery);

    // 获取奖励
    int GetLongactingCumulativeRechargeReward(unsigned int nRoleID, char cSex, std::string & strRoleName, unsigned int nTotalPoint, 
            unsigned int nHaveRewardIndex, unsigned int nRewardIndex);

    // log
    void UpdateRoleActionLog(unsigned int nRoleID, RoleLongactingRechargeLog::ELongactingAction eAction, unsigned int nRewardIndex, 
        bool bIsSucess, bool bIsOnline, unsigned int nTotalMPoint);

private:
    std::string                                                     m_strMailTitle;                 // 邮件标题
    std::string                                                     m_strMailContent;               // 邮件内容
    std::map<unsigned int,LongactingCumulativeRechargeConfig>       m_mapLongactingRechargeConfig;  // 长效累冲配置
};

#endif

