/** GameMsg_Forward.h
* Description: 转发消息，用于封装game server和group server之间的转发消息
* Copyright (C) 2014 ddianle Inc. All rights reserved.
* Author: xieshaobin
* Create time: 2014-12-08 20:35:09
*/

#ifndef __GAMEMSG_FORWARD_H__
#define __GAMEMSG_FORWARD_H__

#include "../../socket/GameMsg_Base.h"

#include <list>
#include <string>


class CParamPool;

class GameMsg_Forward :
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_Forward);
    GameMsg_Forward(void);
    ~GameMsg_Forward(void);

public:
    virtual bool doDecode(CParamPool & IoBuff);
    virtual bool doEncode(CParamPool & IoBuff);

public:
    std::list<unsigned int> m_RoleIDList;	// 最终接收消息的玩家的列表, key: roleid
    unsigned short m_nForwardMsgID;			// 要转发的消息ID
    GameMsg_Base *m_forwardMsg;				// 要转发的消息体，注意:encode时只encode了消息体，没有消息头(account/msgid/sessionid等)

    bool m_bDeleteMsg;						// 是否由fowardMsg释放消息内存
};

#endif //__GAMEMSG_FORWARD_H__

