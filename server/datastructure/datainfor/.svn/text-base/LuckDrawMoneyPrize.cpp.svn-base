#include "LuckDrawMoneyPrize.h"
#include "FileStream.h"

#include "../../socket/ParamPool.h"
#include "../../socket/Windefine.h"

#define CURRENCY_TYPE_MAX   4


CLuckDrawMoneyPrize::CLuckDrawMoneyPrize(void)
    : CLuckDrawPrizeBase(ELuckDrawPrizeType_Money)
    , m_nCurrencyType(1)
    , m_nAmount(0)
{
}

CLuckDrawMoneyPrize::CLuckDrawMoneyPrize(const CLuckDrawMoneyPrize &ref)
    : CLuckDrawPrizeBase(ref)
    , m_nCurrencyType(ref.m_nCurrencyType)
    , m_nAmount(ref.m_nAmount)
{
}

void CLuckDrawMoneyPrize::LoadFromFile(CFileStream &file)
{
    CLuckDrawPrizeBase::LoadFromFile(file);

    file.ReadUChar(m_nCurrencyType);
    file.ReadUInt(m_nAmount);

    ASSERT(m_nCurrencyType < CURRENCY_TYPE_MAX);// 1, 2, 3
}

void CLuckDrawMoneyPrize::doEncodeToClient(CParamPool &IOBuff) const
{
    CLuckDrawPrizeBase::doEncodeToClient(IOBuff);

    doEncodeSelf(IOBuff);
}

void CLuckDrawMoneyPrize::doDecodeFromClient(CParamPool &IOBuff)
{
    CLuckDrawPrizeBase::doDecodeFromClient(IOBuff);

    doDecodeSelf(IOBuff);
}

void CLuckDrawMoneyPrize::doEncodeToServer(CParamPool &IOBuff) const
{
    CLuckDrawPrizeBase::doEncodeToServer(IOBuff);

    doEncodeSelf(IOBuff);
}

void CLuckDrawMoneyPrize::doDecodeFromServer(CParamPool &IOBuff)
{
    CLuckDrawPrizeBase::doDecodeFromServer(IOBuff);

    doDecodeSelf(IOBuff);
}

void CLuckDrawMoneyPrize::doEncodeSelf(CParamPool &IOBuff) const
{
    IOBuff.AddUChar(m_nCurrencyType);
    IOBuff.AddUInt(m_nAmount);
}

void CLuckDrawMoneyPrize::doDecodeSelf(CParamPool &IOBuff)
{
    m_nCurrencyType = IOBuff.GetUChar();
    m_nAmount = IOBuff.GetUInt();
}

