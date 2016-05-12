#ifndef __DANCEGROUP_ENVELOPE_CONFIGMANAGER_H__
#define __DANCEGROUP_ENVELOPE_CONFIGMANAGER_H__

#include <string>
#include <map>
#include "../../../socket/ParamPool.h"

class RedEnvelopeConfig
{
public:
    RedEnvelopeConfig();
    virtual ~RedEnvelopeConfig();

public:
    unsigned int        m_nRedEnvelopeNum;
    unsigned int        m_nRedEnvelopeContinueTime;
    unsigned int        m_nRedEnvelopeGetAllDisappearTime;
    unsigned int        m_nActivityBeginTime;
    unsigned int        m_nActivityEndTime;
    unsigned int        m_nShowBeginTime;
    unsigned int        m_nShowEndTime;
    bool                m_bExhibit;
    int                 m_nWeight;
    std::string         m_strTitle;
    std::string         m_strContent;
    std::string         m_strRegularImageName;
    std::string         m_strThumbnailName;
    std::string         m_strAnonymityName;
};

class RedEnvelopeRechareConfig
{
public:
    RedEnvelopeRechareConfig();
    virtual ~RedEnvelopeRechareConfig();

public:
    void doEncode(CParamPool &IOBuff);

public:
    int                 m_nRechargeMCoin;
    unsigned int        m_nCurrencyType;
    unsigned int        m_nCurrencyValue;
    int                 m_nMinRate;
    int                 m_nFluctuateRate;
};

class RedEnvelopeActivityData
{
public:
    RedEnvelopeActivityData();
    virtual ~RedEnvelopeActivityData();

public:
    void doEncode(CParamPool & IOBuff);

public:
    int                 m_nActivityId;
    RedEnvelopeConfig   m_RedEnvelopeConfig;
    std::map<int,RedEnvelopeRechareConfig>     m_mapRedEnvelopeRechargeConfig;
};

class RedEnvelopeConfigManager
{
public:
    RedEnvelopeConfigManager();
    virtual ~RedEnvelopeConfigManager();

    static RedEnvelopeConfigManager & Instance();

public:
    // 加载配置
    void AddRedEnvelopeConfig(const RedEnvelopeConfig & config);
    void AddRedEnvelopeRechargeConfig(const RedEnvelopeRechareConfig & config);

    // 获取充值金额对应的红包配置
    RedEnvelopeRechareConfig * GetRedEnvelopeRechargeConfig(int nRechargeMCoin);

    // 获取每份红包的份数
    unsigned int GetRedEnvelopeCount();

    // 获取匿名姓名
    std::string GetAnonymityName();

    // 全部领取后消失时间
    unsigned int GetAllDisappearTime();

    // 持续时间
    unsigned int GetContinueTime();

    // 是否在活动时间内
    bool IsInActivity(unsigned int nNowTime);

    // 是否获得显示
    bool IsActivityShow(unsigned int nNowTime);

    // 获取活动开始、结束时间
    unsigned int GetActivityBeginTime();
    unsigned int GetActivityEndTime();

    // 显示开始、结束时间
    unsigned int GetShowBeginTime();
    unsigned int GetShowEndTime();
    
    //
    void GetRedEnvelopeConfig(RedEnvelopeActivityData & data);

public:
   RedEnvelopeActivityData              m_RedEnvelopeActivityConfig;
};
#endif

