#include "Currency.h"
#include "CurrencyType.h"

#include "../../socket/GameMsg_Base.h"

Currency::Currency(void)
    : m_nType(EMallCurrencyType_MCoin)
    , m_nAmount(0)
{
}

Currency::~Currency(void)
{
}

void Currency::SetType(unsigned char nType)
{
    m_nType = nType;
}

void Currency::SetAmount(unsigned int nAmount)
{
    m_nAmount = nAmount;
}

bool Currency::IsValid() const
{
    if (EMallCurrencyType_None < m_nType && m_nType < EMallCurrencyType_Max && m_nAmount > 0)
        return true;

    return false;
}

void Currency::AddIntoCurrencyMap(std::map<unsigned char, unsigned int> &rMapBills, bool bInit, double nRatio) const
{
    if (bInit)
    {
        rMapBills.insert(std::make_pair(EMallCurrencyType_MCoin, 0));
        rMapBills.insert(std::make_pair(EMallCurrencyType_BindMCoin, 0));
        rMapBills.insert(std::make_pair(EMallCurrencyType_Money, 0));
    }

    if (IsValid())
    {
        std::map<unsigned char, unsigned int>::iterator itr = rMapBills.find(m_nType);
        unsigned int nAmount = (unsigned int)(m_nAmount * nRatio);

        if (itr != rMapBills.end())
        {
            itr->second += nAmount;
        }
        else if (!bInit)
        {
            rMapBills.insert(std::make_pair(m_nType, nAmount));
        }
    }
}

void Currency::doEncode(CParamPool &IOBuff)
{
    IOBuff << m_nType << m_nAmount;
}

