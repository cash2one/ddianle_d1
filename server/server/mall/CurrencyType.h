/*
 * CurrencyType.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-4 13:56:30
 */

#ifndef __CURRENCYTYPE_H__
#define __CURRENCYTYPE_H__

#define MAX_CURRENCY_TYPE  3

enum EMallCurrencyType
{
    EMallCurrencyType_None             = 0, // ������
    EMallCurrencyType_MCoin            = 1, // M�� ���� ���������Ϸ�еĴ��ң�Ψһ��Դ������ҳ�ֵ
    EMallCurrencyType_Money            = 2, // ��ȯ ���� ����������ϵĴ��ң���ͨ����ֵ���ܻ���⣬��Ϸ�ڸ��ֻ�����Ի�ã��ܹ����̳���С������Ʒ
    EMallCurrencyType_BindMCoin        = 3, // ��M�� ���� ��ֵ�õ��Ľ���M�ң�����������ϣ��ܹ����̳��д󲿷���Ʒ���������������͸�����
    EMallCurrencyType_Max
};

#endif // __CURRENCYTYPE_H__

