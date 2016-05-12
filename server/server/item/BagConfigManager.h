#ifndef __ITEMCONFIGMANAGER__
#define __ITEMCONFIGMANAGER__

#include <string>

class BagConfigManager
{
public:
    BagConfigManager();
    virtual ~BagConfigManager();
    static BagConfigManager & Instance();

public:
    // 加载配置
    void AddBagConfig(int nMaxStorageCapacity, int nMinStorageCapacity, std::string & strDescribe, int nCurrencyType, int nCurrencyValue);

    // 获取开通仓库格子货币类型和值
    int GetAddStorageCurrencyType();
    int GetAddStorageCurrencyValue();

    // 获取背包说明
    std::string GetBagDescribe();

    // 仓库最小容量
    int GetStorageMinCapacity();
    int GetStorageMaxCapacity();

private:
    int                 m_nStorageMaxCapacity;
    std::string         m_strDescribe;
    int                 m_nAddStorageCapacityCurrencyType;
    int                 m_nAddStorageCapacityCurrencyValue;
    int                 m_nStoageMinCapacity;
};

#endif
