/*
* GameMsg_S2C_ChangeBirthdayRes.h
* Description: change birthday result
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: XieJiangBei
* Create time: 2014-11-12 15:27:21
*/

#ifndef __GAMEMSG_S2C_CHANGEBIRTHDAYRES_H__
#define __GAMEMSG_S2C_CHANGEBIRTHDAYRES_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

class CParamPool;

enum ChangeBirthdayResult
{
    CHANGEBIRTHDAY_SUCCESS = 0,
    CHANGEBIRTHDAY_INVALID_BIRTHDAY,        //生日无效
    CHANGEBIRTHDAY_IS_SAME,                 //生日相同
    CHANGEBIRTHDAY_CANNOT_CHANGE,           //已修改过
};

/**
* GameMsg_S2C_ChangeBirthdayRes:
* 修改生日结果协议
*/

class GameMsg_S2C_ChangeBirthdayRes : public GameMsg_Base
{
public:
    GameMsg_S2C_ChangeBirthdayRes(unsigned char nResult = CHANGEBIRTHDAY_SUCCESS);
    ~GameMsg_S2C_ChangeBirthdayRes();

public:
    void SetBirthday(const std::string &rBirthday);

    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nResult;
    unsigned char   m_nConstellation;

protected:
    std::string     m_strBirthday;
};

#endif // __GAMEMSG_S2C_CHANGEBIRTHDAYRES_H__

