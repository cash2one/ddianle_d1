/** GameMsg_Currency.h
* Description: 货币相关消息.
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2015-01-29 20:03:11
*/

#ifndef _GAMEMSG_CURRENCY_H_
#define _GAMEMSG_CURRENCY_H_

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"

#include <list>
#include <string>

class CParamPool;

class GameMsg_S2C_AllowedCurrencyList : public GameMsg_Base
{
public:
    GameMsg_S2C_AllowedCurrencyList();
    ~GameMsg_S2C_AllowedCurrencyList();

public:
    bool doEncode(CParamPool &ioBuffer);

public:
    std::list<char> m_listCurrency;

};

class GameMsg_C2S_ClientDeviceInfo: public GameMsg_Base
{
public:
    GameMsg_C2S_ClientDeviceInfo();
    ~GameMsg_C2S_ClientDeviceInfo();
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ClientDeviceInfo);

public:
    bool doDecode(CParamPool &ioBuffer);

    void ToCurrencyString(std::string &strCurrencyList);

public:
    std::string m_strDeviceId;
    std::list<char> m_listCurrency;
};


class GameMsg_S2C_ValidChargeDevice
    : public GameMsg_Base
{
public:
    GameMsg_S2C_ValidChargeDevice();
    ~GameMsg_S2C_ValidChargeDevice();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    bool m_bIsInBlackList;          // 是否在充值黑名单中
};

class GameMsg_C2S_SaveChargeAction: public GameMsg_Base
{
public:
    GameMsg_C2S_SaveChargeAction();
    ~GameMsg_C2S_SaveChargeAction();
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_SaveChargeAction);

public:
    bool doDecode(CParamPool &ioBuffer);

public:
    ChargeAction        m_Action;
};


#endif //_GAMEMSG_CURRENCY_H_

