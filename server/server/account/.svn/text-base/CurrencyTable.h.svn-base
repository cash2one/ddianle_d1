/** CurrencyTable.h
* Description: 游戏支持的支付货币列表，可配置，可热加载
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-01-29 20:23:00
*/

#include <set>
#include <list>
#include <string>

#ifndef _CURRENCY_TABLE_H_
#define _CURRENCY_TABLE_H_

#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"

class CurrencyTable : public GameMsg_Processor
{
public:
    CurrencyTable();
    ~CurrencyTable();

    static CurrencyTable &Instance();

public:

    bool HasInited() const;
    void Init();

    // 是否是合法的支付货币，合法返回true，否则返回false
    bool IsValidCurrency(int nCurrency) const;

    // 设备ID是否在黑名单中，是返回true，否则返回false
    bool IsBlackListDevice(const std::string &strDeviceID) const;

    // 获取合法的支付货币列表
    void GetCurrencyList(std::list<char> &currencyList) const;

    // 每隔一定时间，重新载入黑名单和货币列表
    void Reload();

protected:

    void ReloadSupportedCurrency();
    void ReloadBlackDeviceList();

    void OnDBLoadBlackDevice(QueryBase &query);

protected:
    bool m_bHasInited;                          // 是否已经初始化（用于判断消息是否注册）

    std::set<int> m_supportedCurrency;          // 支持的货币列表
    std::set<std::string> m_blackListDevice;    // 黑名单设备列表
};

#endif //_CURRENCY_TABLE_H_

