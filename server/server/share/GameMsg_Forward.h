/** GameMsg_Forward.h
* Description: ת����Ϣ�����ڷ�װgame server��group server֮���ת����Ϣ
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
    std::list<unsigned int> m_RoleIDList;	// ���ս�����Ϣ����ҵ��б�, key: roleid
    unsigned short m_nForwardMsgID;			// Ҫת������ϢID
    GameMsg_Base *m_forwardMsg;				// Ҫת������Ϣ�壬ע��:encodeʱֻencode����Ϣ�壬û����Ϣͷ(account/msgid/sessionid��)

    bool m_bDeleteMsg;						// �Ƿ���fowardMsg�ͷ���Ϣ�ڴ�
};

#endif //__GAMEMSG_FORWARD_H__

