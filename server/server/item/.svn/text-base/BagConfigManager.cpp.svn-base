#include "BagConfigManager.h"

BagConfigManager::BagConfigManager()
:m_nStorageMaxCapacity(0)
, m_strDescribe("")
, m_nAddStorageCapacityCurrencyType(0)
, m_nAddStorageCapacityCurrencyValue(0)
{

}

BagConfigManager::~BagConfigManager()
{

}

BagConfigManager & BagConfigManager::Instance()
{
    static BagConfigManager g_StaticConfig;
    return g_StaticConfig;
}

void BagConfigManager::AddBagConfig(int nMaxStorageCapacity, int nMinStorageCapacity, std::string & strDescribe, int nCurrencyType, int nCurrencyValue)
{
    m_nStorageMaxCapacity = nMaxStorageCapacity;
    m_nStoageMinCapacity = nMinStorageCapacity;
    m_strDescribe = strDescribe;
    m_nAddStorageCapacityCurrencyType = nCurrencyType;
    m_nAddStorageCapacityCurrencyValue = nCurrencyValue;
}

int BagConfigManager::GetAddStorageCurrencyType()
{
    return m_nAddStorageCapacityCurrencyType;
}

int BagConfigManager::GetAddStorageCurrencyValue()
{
    return m_nAddStorageCapacityCurrencyValue;
}

std::string BagConfigManager::GetBagDescribe()
{
    return m_strDescribe;
}

int BagConfigManager::GetStorageMinCapacity()
{
    return m_nStoageMinCapacity;
}

int BagConfigManager::GetStorageMaxCapacity()
{
    return m_nStorageMaxCapacity;
}
