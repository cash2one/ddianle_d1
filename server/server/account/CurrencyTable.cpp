#include "CurrencyTable.h"
#include "../util/CommonFunc.h"
#include "../share/ServerDataStruct.h"
#include "../../socket/Log.h"
#include "../libServerFrame/QueryManager.h"

#include <fstream>
#include <string.h>
#include <stdlib.h>


extern CQueryManager* g_pQueryMgr;

CurrencyTable::CurrencyTable()
    : GameMsg_Processor(true, true)
    , m_bHasInited(false)
{
}

CurrencyTable::~CurrencyTable()
{
}

bool CurrencyTable::HasInited() const
{
    return m_bHasInited;
}

void CurrencyTable::Init()
{
    m_bHasInited = true;

    RegQueryProcessor(Query_System_LoadBlackDevice, &CurrencyTable::OnDBLoadBlackDevice);
}

CurrencyTable &CurrencyTable::Instance()
{
    static CurrencyTable s_instance;
    if (!s_instance.HasInited())
    {
        s_instance.Init();
    }
    return s_instance;
}

// 是否是合法的支付货币，合法返回true，否则返回false
// 如果客户端收集不到货币类型（nCurrency==0）
// 或者服务器不对支付货币做限制（m_supportedCurrency.contains(0) == true）
// 再或者，客户端的货币类型在服务器的货币类型列表中（m_supportedCurrency.contains(nCurrency) == true）
// 都认为是合法的支付货币，其它任何情况都是非法支付货币
bool CurrencyTable::IsValidCurrency(int nCurrency) const
{
    if (nCurrency == 0 || m_supportedCurrency.find(0) != m_supportedCurrency.end())
    {
        return true;
    }

    if (m_supportedCurrency.find(nCurrency) != m_supportedCurrency.end())
    {
        return true;
    }

    return false;
}

bool CurrencyTable::IsBlackListDevice(const std::string &strDeviceID) const
{
    if (strDeviceID.empty() || m_blackListDevice.empty())
    {
        return false;
    }

    if (m_blackListDevice.find(strDeviceID) != m_blackListDevice.end())
    {
        return true;
    }

    return false;
}

void CurrencyTable::GetCurrencyList(std::list<char> &currencyList) const
{
    for (std::set<int>::const_iterator i = m_supportedCurrency.begin(); i != m_supportedCurrency.end(); ++i)
    {
        currencyList.push_back((char)*i);
    }
}

void CurrencyTable::Reload()
{
    ReloadSupportedCurrency();
    ReloadBlackDeviceList();
}

void CurrencyTable::ReloadSupportedCurrency()
{
    std::set<int> newCurrencySet;

    static const char *kCurrencyConfigFile = "data/currency.ini";
    extern std::string g_sPath;

    std::string strPath = g_sPath + kCurrencyConfigFile;

    std::fstream file;
    file.open(strPath.c_str());
    if (file.good())
    {
        while (true)
        {
            char line[1024] = {0};
            file.getline(line, sizeof(line));

            std::string strLine = line;
            TrimSpace(strLine);

            if (strLine.empty())
            {
                break;
            }

            int nCurrency = strtoul(strLine.c_str(), NULL, 10);
            if (nCurrency >= 0 && nCurrency < (int)ECurrencyType_Max)
            {
                newCurrencySet.insert(nCurrency);
            }
        }
    }
    file.close();

    if (!newCurrencySet.empty())
    {
        m_supportedCurrency = newCurrencySet;

        WriteLog(LOGLEVEL_DEBUG, "[CURRENCY]Supported currency set updating: ");
        for (std::set<int>::const_iterator it = newCurrencySet.begin(); it != newCurrencySet.end(); ++it)
        {
            WriteLog(LOGLEVEL_DEBUG, "currency: %d", *it);
        }
    }
}

void CurrencyTable::ReloadBlackDeviceList()
{
    if (g_pQueryMgr != NULL)
    {
        std::list<std::string> *pBlackList = new std::list<std::string>();
        g_pQueryMgr->AddQuery(Query_System_LoadBlackDevice, 0, pBlackList);
    }
}

void CurrencyTable::OnDBLoadBlackDevice(QueryBase &query)
{
    std::list<std::string> *pBlackList = (std::list<std::string> *)query.m_pAttachment;
    if (pBlackList != NULL)
    {
        m_blackListDevice.clear();

        WriteLog(LOGLEVEL_DEBUG, "[CURRENCY]Charge black list device: ");
        for (std::list<std::string>::const_iterator it = pBlackList->begin();
            it != pBlackList->end(); ++it)
        {
            m_blackListDevice.insert(*it);
            WriteLog(LOGLEVEL_DEBUG, "deviceid: %s", it->c_str());
        }

        delete (std::list<std::string> *)query.m_pAttachment, query.m_pAttachment = NULL;
    }
}

