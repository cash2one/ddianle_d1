/************************************************************************
 * HandbookStuct.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-15 10:22:20
 ***********************************************************************/
#ifndef __HANDBOOKSTRUCT_H__
#define __HANDBOOKSTRUCT_H__

#include "../../server/Macros/Macros.h"

enum EHandbookType
{
    eHandbookType_Costume = 1,       //服饰类型
    eHandbookType_Effect = 2,       //特效类型
};

enum EHandbookDBIndex
{
    eHandbookDB_ID = 0,             //ID
    eHandbookDB_Praise = 1,         //是否被赞美
    eHandbookDB_Envy = 2,           //是否被嫉妒
    eHandbookDB_Collect = 3,        //是否被收藏
    eHandbookDB_End = 4,
};

class CHandbookUnit
{
public:
    CHandbookUnit():m_ID(0),m_PraisedCount(0), m_EnvyCount(0),m_CollectCount(0){}
    CHandbookUnit(int id, int PraisedCount, int EnvyCount, int CollectCount);
    virtual ~CHandbookUnit() {}

    CLASS_PROPERTY(int, m_ID, ID);
    CLASS_PROPERTY(int, m_PraisedCount, PraisedCount);
    CLASS_PROPERTY(int, m_EnvyCount, EnvyCount);
    CLASS_PROPERTY(int, m_CollectCount, CollectCount);
};

class CHandbookPersonalInfo
{
public:
    CHandbookPersonalInfo(){}
    CHandbookPersonalInfo(int id, int Praised, int Envyed, int Collected);
    virtual ~CHandbookPersonalInfo(){}

    CLASS_PROPERTY(int, m_ID, ID);
    CLASS_PROPERTY(int, m_Praised, Praised );
    CLASS_PROPERTY(int, m_Envy, Envyed );
    CLASS_PROPERTY(int, m_Collect, Collected );

};

#endif//__HANDBOOKSTRUCT_H__
