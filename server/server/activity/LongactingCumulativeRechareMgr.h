/** MallShareMgr.h
* Description: ��Ч�۳����ݹ�����
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
    // ��ȡ��Ч�۳�����
    void GetLongactingCumulativeRechargeConfig(std::list<LongactingCumulativeRechargeConfig> & listConfig);
    
    // ���ؽ�������
    void AddLongactingCumulativeRechargeConfig(const LongactingCumulativeRechargeConfig & config);

    // �����ʼ���Ϣ
    void AddLongactingCumulativeRechargeMail(const std::string & strMailTtle, const std::string & strMailContent);

    // ��ȡ��һ������
    const LongactingCumulativeRechargeConfig * GetNextConfig(unsigned int nCurrentIndex);

    // ��ȡ����
    const LongactingCumulativeRechargeConfig * GetCurrentConfig(unsigned int nCurrentIndex);

public:
    // ��sql���ش���
    void OnGetRewardCallBack(QueryBase & rQuery);

    // ��ȡ����
    int GetLongactingCumulativeRechargeReward(unsigned int nRoleID, char cSex, std::string & strRoleName, unsigned int nTotalPoint, 
            unsigned int nHaveRewardIndex, unsigned int nRewardIndex);

    // log
    void UpdateRoleActionLog(unsigned int nRoleID, RoleLongactingRechargeLog::ELongactingAction eAction, unsigned int nRewardIndex, 
        bool bIsSucess, bool bIsOnline, unsigned int nTotalMPoint);

private:
    std::string                                                     m_strMailTitle;                 // �ʼ�����
    std::string                                                     m_strMailContent;               // �ʼ�����
    std::map<unsigned int,LongactingCumulativeRechargeConfig>       m_mapLongactingRechargeConfig;  // ��Ч�۳�����
};

#endif

