#include "GameMsg_Currency.h"

#include "../../socket/ParamPool.h"
#include "../../socket/Formatter.h"
#include "AccountMsgDef.h"
#include <stdio.h>


GameMsg_S2C_AllowedCurrencyList::GameMsg_S2C_AllowedCurrencyList()
    : GameMsg_Base(MSG_ACCOUNT_S2C_AllowedCurrencyList)
{
}
GameMsg_S2C_AllowedCurrencyList::~GameMsg_S2C_AllowedCurrencyList()
{
}

bool GameMsg_S2C_AllowedCurrencyList::doEncode(CParamPool &ioBuffer)
{
    ioBuffer.AddUShort((unsigned short)m_listCurrency.size());
    for (std::list<char>::iterator it = m_listCurrency.begin(); it != m_listCurrency.end(); ++it)
    {
        ioBuffer.AddUChar((unsigned char)*it);
    }
    return true;
}


GameMsg_C2S_ClientDeviceInfo::GameMsg_C2S_ClientDeviceInfo()
    : GameMsg_Base(MSG_ACCOUNT_C2S_ClientDeviceInfo)
{

}
GameMsg_C2S_ClientDeviceInfo::~GameMsg_C2S_ClientDeviceInfo()
{

}

bool GameMsg_C2S_ClientDeviceInfo::doDecode(CParamPool &ioBuffer)
{
    ioBuffer.GetStringW(m_strDeviceId);
    int count = ioBuffer.GetUShort();
    for (int i = 0; i < count; ++i)
    {
        char currency = (char)ioBuffer.GetUChar();
        m_listCurrency.push_back(currency);
    }

    return true;
}

void GameMsg_C2S_ClientDeviceInfo::ToCurrencyString(std::string &strCurrencyList)
{
    for (std::list<char>::const_iterator it = m_listCurrency.begin(); it != m_listCurrency.end(); ++it)
    {
        std::string strCurrency;
        Formatter(strCurrency) << (int)*it << ',';
        strCurrencyList.append(strCurrency);
    }
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_ValidChargeDevice::GameMsg_S2C_ValidChargeDevice()
    : GameMsg_Base(MSG_ACCOUNT_S2C_ValidChargeDevice)
{
    m_bIsInBlackList = false;
}

GameMsg_S2C_ValidChargeDevice::~GameMsg_S2C_ValidChargeDevice()
{

}

bool GameMsg_S2C_ValidChargeDevice::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddBool(m_bIsInBlackList);

    return true;
}

GameMsg_C2S_SaveChargeAction::GameMsg_C2S_SaveChargeAction()
    : GameMsg_Base(MSG_ACCOUNT_C2S_SaveChargeAction)
{

}
GameMsg_C2S_SaveChargeAction::~GameMsg_C2S_SaveChargeAction()
{

}

bool GameMsg_C2S_SaveChargeAction::doDecode(CParamPool &ioBuffer)
{
    m_Action.doDecode(ioBuffer);
    return true;
}


