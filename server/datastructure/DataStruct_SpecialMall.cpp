#include "DataStruct_SpecialMall.h"
#include "../socket/ParamPool.h"

//////////////////////////////////////////////////////////////////////////

CGroupBuyingItemData::CGroupBuyingItemData()
{
    m_uIdx = 0;
    m_uPrice = (unsigned int)-1;
    m_nPriceType = 1;
    m_uDiscount = (unsigned int)-1;
    m_nDiscountType = 1;
    m_nCustomCount = 0;
    m_bAlreadyBuy = false;
}

void CGroupBuyingItemData::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uPrice = IOBuff.GetUInt();
    m_nPriceType = IOBuff.GetUChar();
    m_uDiscount = IOBuff.GetUInt();
    m_nDiscountType = IOBuff.GetUChar();
    m_nCustomCount = IOBuff.GetInt();
    m_bAlreadyBuy = IOBuff.GetBool();
}

void CGroupBuyingItemData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uPrice);
    IOBuff.AddUChar(m_nPriceType);
    IOBuff.AddUInt(m_uDiscount);
    IOBuff.AddUChar(m_nDiscountType);
    IOBuff.AddInt(m_nCustomCount);
    IOBuff.AddBool(m_bAlreadyBuy);
}

//////////////////////////////////////////////////////////////////////////

CLimitedEditionItemData::CLimitedEditionItemData()
{
    m_uIdx = 0;
    m_uPrice = (unsigned int)-1;
    m_nPriceType = 1;
    m_uDiscount = (unsigned int)-1;
    m_nDiscountType = 1;
    m_nRemainder = 0;
    m_bAlreadyBuy = false;
}

void CLimitedEditionItemData::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uPrice = IOBuff.GetUInt();
    m_nPriceType = IOBuff.GetUChar();
    m_uDiscount = IOBuff.GetUInt();
    m_nDiscountType = IOBuff.GetUChar();
    m_nRemainder = IOBuff.GetInt();
    m_bAlreadyBuy = IOBuff.GetBool();
}

void CLimitedEditionItemData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uPrice);
    IOBuff.AddUChar(m_nPriceType);
    IOBuff.AddUInt(m_uDiscount);
    IOBuff.AddUChar(m_nDiscountType);
    IOBuff.AddInt(m_nRemainder);
    IOBuff.AddBool(m_bAlreadyBuy);
}

//////////////////////////////////////////////////////////////////////////

CAuctionItemData::CAuctionItemData()
{
    m_uIdx = 0;
    m_uCurPrice = (unsigned int)-1;
    m_uRoleID = 0;
    m_uMinAdd = 0;
    m_fCountDown = 0;
    m_bAlreayTop = false;
    m_bAlreadyBuy = false;
}

void CAuctionItemData::doDecode(CParamPool &IOBuff)
{
    m_uIdx = IOBuff.GetUShort();
    m_cItem.doDecode(IOBuff);
    m_uCurPrice = IOBuff.GetUInt();
    m_uRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_uMinAdd = IOBuff.GetUInt();
    m_fCountDown = IOBuff.GetFloat();
    m_bAlreayTop = IOBuff.GetBool();
    m_bAlreadyBuy = IOBuff.GetBool();
}

void CAuctionItemData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUShort(m_uIdx);
    m_cItem.doEncode(IOBuff);
    IOBuff.AddUInt(m_uCurPrice);
    IOBuff.AddUInt(m_uRoleID);
    IOBuff.AddStringW(m_strRoleName.c_str());
    IOBuff.AddUInt(m_uMinAdd);
    IOBuff.AddFloat(m_fCountDown);
    IOBuff.AddBool(m_bAlreayTop);
    IOBuff.AddBool(m_bAlreadyBuy);
}

//////////////////////////////////////////////////////////////////////////

CAuctionTopPriceData::CAuctionTopPriceData()
{
    m_lLogID = 0;
    m_uIdx = 0;
    m_tEndTime = 0;
}

void CAuctionTopPriceData::doDecode(CParamPool &IOBuff)
{
    m_lLogID = IOBuff.GetInt64();
    m_uIdx = IOBuff.GetUShort();
    m_tEndTime = IOBuff.GetUInt();
}

void CAuctionTopPriceData::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddInt64(m_lLogID);
    IOBuff.AddUShort(m_uIdx);
    IOBuff.AddUInt(m_tEndTime);
}


