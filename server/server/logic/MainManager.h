/*
 * MainManager.h
 * Description: ��Ϸ�߼����������������ú͹������Ĺ����������ڳ�ʼ�������Ϸ���ݣ�������Ϸ�ڵķ���
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

