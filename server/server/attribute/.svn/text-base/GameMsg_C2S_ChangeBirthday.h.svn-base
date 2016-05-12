/*class
 * GameMsg_C2S_ChangeBirthday.h
 * Description: change birthday
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-11-10 17:16:55
 */

#ifndef __GAMEMSG_C2S_CHANGEBIRTHDAY_H__
#define __GAMEMSG_C2S_CHANGEBIRTHDAY_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"

class CParamPool;

/**
* GameMsg_C2S_ChangeBirthday:
* 修改生日协议
*/

class GameMsg_C2S_ChangeBirthday : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeBirthday);
    GameMsg_C2S_ChangeBirthday();
    ~GameMsg_C2S_ChangeBirthday();

public:
    bool doDecode(CParamPool &IOBuff);

public:
    char m_szYear[MAX_YEAR_LEN];
    char m_szMonth[MAX_MONTH_LEN];
    char m_szDay[MAX_DAY_LEN];
};

#endif // __GAMEMSG_C2S_CHANGEBIRTHDAY_H__

