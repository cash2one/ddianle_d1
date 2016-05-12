/*
 * SpecialMallSettings.h
 * Description: Ãÿ ‚…ÃµÍ≈‰÷√¿‡
 * Copyright (C) 2014 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2014-12-10 10:23:39
 */

#ifndef __SPECIALMALLSETTINGS_H__
#define __SPECIALMALLSETTINGS_H__

#include <string>

/**
* SpecialMallSettings:
* 
*/

class CSpecialMallSettings
{
public:
    CSpecialMallSettings() {}
    virtual ~CSpecialMallSettings() {}

public:
    std::string     m_strMailTitle;
    std::string     m_strMailContent;
};

#endif // __SPECIALMALLSETTINGS_H__

