/*
 * DataStruct_SpecialMall.h
 * Description: �����̵����ݽṹ
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-10 13:54:08
 */

#ifndef __DATASTRUCT_SPECIALMALL_H__
#define __DATASTRUCT_SPECIALMALL_H__

#include "DataStruct_Base.h"

class CParamPool;

//////////////////////////////////////////////////////////////////////////

/**
* CGroupBuyingItemData:
* �Ź���Ʒ����
*/

class CGroupBuyingItemData
{
public:
    CGroupBuyingItemData();
    ~CGroupBuyingItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // ����
    CItem           m_cItem;            // ��Ʒ
    unsigned int    m_uPrice;           // �г���
    unsigned char   m_nPriceType;       // �г�������
    unsigned int    m_uDiscount;        // �ۿۼ�
    unsigned char   m_nDiscountType;    // ֧������
    int             m_nCustomCount;     // ��������
    bool            m_bAlreadyBuy;      // �Ƿ������÷���
};

//////////////////////////////////////////////////////////////////////////

/**
* CLimitedEditionItemData:
* ����������Ʒ����
*/

class CLimitedEditionItemData
{
public:
    CLimitedEditionItemData();
    ~CLimitedEditionItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // ����
    CItem           m_cItem;            // ��Ʒ
    unsigned int    m_uPrice;           // �г���
    unsigned char   m_nPriceType;       // �г�������
    unsigned int    m_uDiscount;        // �ۿۼ�
    unsigned char   m_nDiscountType;    // ֧������
    int             m_nRemainder;       // ʣ������
    bool            m_bAlreadyBuy;      // �Ƿ������÷���
};

//////////////////////////////////////////////////////////////////////////

/**
* CAuctionItemData:
* ��������Ʒ����
*/

class CAuctionItemData
{
public:
    CAuctionItemData();
    ~CAuctionItemData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned short  m_uIdx;             // ����
    CItem           m_cItem;            // ��Ʒ
    unsigned int    m_uCurPrice;        // ��ǰ��
    unsigned int    m_uRoleID;          // ������
    std::string     m_strRoleName;      // ����������
    unsigned int    m_uMinAdd;          // ��С�Ӽ�
    float           m_fCountDown;       // ����ʱ
    bool            m_bAlreayTop;       // �Ƿ���߼�
    bool            m_bAlreadyBuy;      // �Ƿ������÷���
};

//////////////////////////////////////////////////////////////////////////

class CAuctionTopPriceData
{
public:
    CAuctionTopPriceData();
    ~CAuctionTopPriceData() {}

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    long long           m_lLogID;
    unsigned short      m_uIdx;
    unsigned int        m_tEndTime;
};

#endif // __DATASTRUCT_SPECIALMALL_H__

