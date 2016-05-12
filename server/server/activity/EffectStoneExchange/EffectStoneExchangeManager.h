#ifndef	__ACTIVITY_EFFECTSTONEEXCHANGE_H__
#define	__ACTIVITY_EFFECTSTONEEXCHANGE_H__

#include <string>
#include <map>
#include "../../../datastructure/DataStruct_Base.h"

class EffectStoneExchangeData
{
public:
    EffectStoneExchangeData();
    virtual ~EffectStoneExchangeData();

public:
    void doEncode(CParamPool & IOBuff);

public:
    int                 m_nActivityId;
    std::string         m_strRegularImageName;
    std::string         m_strThumbnailName;
    std::string         m_strActivityTitle;
    std::string         m_strActivityContent;
    std::list<CItem>    m_listExchangeItem;
};

class EffectStoneExchangeManager
{
public:
    EffectStoneExchangeManager();
    virtual ~EffectStoneExchangeManager();

    static EffectStoneExchangeManager & Instance();

public:
    // 加载配置
    void AddActivityConfig(std::string & strTtile, std::string & strContent, unsigned int nBeginTime, unsigned int nEndTime,
            std::string strRegularImageName, std::string strThumbnailName);
    void AddExchangeItem(itemtype_t nOldItem, const CItem & newItem);

    // 是否在活动中
    bool IsActivityOpen();

    // 获取交换物品
    const std::map<itemtype_t, CItem> & GetExchangeItem();

    // 设置数据
    void GetEffectStoneExchangeConfig(EffectStoneExchangeData & data);

public:
    std::string         m_strActivityTitle;
    std::string         m_strActivityContent;
    unsigned int        m_nActivityBeginTime;
    unsigned int        m_nActivityEndTime;
    std::string         m_strRegularImageName;
    std::string         m_strThumbnailName;
    std::map<itemtype_t, CItem>     m_mapExchange;
};




#endif

