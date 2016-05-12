/** CurrencyTable.h
* Description: ��Ϸ֧�ֵ�֧�������б������ã����ȼ���
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

    // �Ƿ��ǺϷ���֧�����ң��Ϸ�����true�����򷵻�false
    bool IsValidCurrency(int nCurrency) const;

    // �豸ID�Ƿ��ں������У��Ƿ���true�����򷵻�false
    bool IsBlackListDevice(const std::string &strDeviceID) const;

    // ��ȡ�Ϸ���֧�������б�
    void GetCurrencyList(std::list<char> &currencyList) const;

    // ÿ��һ��ʱ�䣬��������������ͻ����б�
    void Reload();

protected:

    void ReloadSupportedCurrency();
    void ReloadBlackDeviceList();

    void OnDBLoadBlackDevice(QueryBase &query);

protected:
    bool m_bHasInited;                          // �Ƿ��Ѿ���ʼ���������ж���Ϣ�Ƿ�ע�ᣩ

    std::set<int> m_supportedCurrency;          // ֧�ֵĻ����б�
    std::set<std::string> m_blackListDevice;    // �������豸�б�
};

#endif //_CURRENCY_TABLE_H_

