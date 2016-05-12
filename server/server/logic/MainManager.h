/*
 * MainManager.h
 * Description: 游戏逻辑服务器内所有配置和管理器的管理器，用于初始化大多游戏数据，启动游戏内的服务
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: xieshaobin
 * Create time: 2015-6-24 15:50:31
 */

#ifndef __MAIN_MANAGER_H__
#define __MAIN_MANAGER_H__

class MainManager
{
public:
    MainManager();
    virtual ~MainManager();

public:
    bool Init();
    void OnUpdate(unsigned long lTimeElapsed);
};

#endif //__MAIN_MANAGER_H__

