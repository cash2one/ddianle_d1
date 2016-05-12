/*
 * GroupSystemSetting.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-3-10 17:42:44
 */

#ifndef __GROUPSYSTEMSETTING_H__
#define __GROUPSYSTEMSETTING_H__

#include <map>

/**
* CGroupSystemSetting:
* 
*/

class CGroupSystemSetting
{
protected:
    CGroupSystemSetting();

public:
    static CGroupSystemSetting& Instance();
    virtual ~CGroupSystemSetting();

public:
    void SetFunctionSwitch(unsigned short nFunction, bool bOpen);
    bool IsFunctionOpen(unsigned short nFunction) const;

private:
    std::map<unsigned short, bool>  m_mapFunctionSwitch;
};

#endif // __GROUPSYSTEMSETTING_H__

