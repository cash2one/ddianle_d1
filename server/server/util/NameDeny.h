/*
* NameDeny.h
* Description: ÆÁ±Î×ÖÀà
* Copyright (C) 2015 ddianle Inc. All rights reserved.
* Author: XieJiangBei
* Create time: 2015-9-6 14:12:24
*/

#ifndef __NAMEDENY_H__
#define __NAMEDENY_H__

#include "../Macros/Macros.h"
#include <map>
#include <string>

/**
* NameDeny:
*
*/

class NameDeny
{
protected:
    NameDeny();
    virtual ~NameDeny();

public:
    CLASS_INSTANCE_FUNC(NameDeny);

public:
    bool ReadInfoFromFile(const std::string &rFileName);

    bool IsValidText(const std::string &rText) const;
    void FilterInvalidText(std::string &rText) const;

    static void MakeLower(std::string &rText);
    static unsigned int CalcGBKStringLength(const std::string &rText);

private:
    std::map<std::string, unsigned int> m_mapDenies;    // string -> GBK length
};

#endif // __NAMEDENY_H__

