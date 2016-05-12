/*
 * GameServerLinkInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-11-12 13:32:21
 */

#ifndef __GAMESERVERLINKINFO_H__
#define __GAMESERVERLINKINFO_H__

#include "../Macros/Macros.h"
#include <string>

class CParamPool;

#define INVALID_GROUP   -1
#define INVALID_LINE    -1

//定义服务器类型，用于在客户端区分连接了哪个服务器
const unsigned char SERVER_LOGIN = 0;   // AccountServer
const unsigned char SERVER_GAME = 1;    // GameServer
const unsigned char SERVER_GROUP = 2;   // GroupServer

/**
* GameServerLinkInfo:
* 
*/

class GameServerLinkInfo
{
public:
    GameServerLinkInfo();
    virtual ~GameServerLinkInfo() {}

    void Reset();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

    CLASS_PROPERTY(unsigned int, m_nAddr, Addr)                     // 地址
    CLASS_PROPERTY(unsigned short, m_nPort, Port)                   // 端口
    CLASS_PROPERTY(int, m_nGroup, Group)                            // 组
    CLASS_PROPERTY(int, m_nLine, Line)                              // 线
    CLASS_PROPERTY(unsigned short, m_nSlot, Slot)
    CLASS_PROPERTY(unsigned short, m_nClient, Client)               // 当前连接的客户数
    CLASS_PROPERTY_BY_REF(std::string, m_strName, Name)
    CLASS_PROPERTY(unsigned char, m_nInstallerType, InstallerType)  // 安装包类型
    CLASS_BOOL_PROPERTY(m_bEnableAudition, EnableAudition)          // 是否是海选线
};

#endif // __GAMESERVERLINKINFO_H__

