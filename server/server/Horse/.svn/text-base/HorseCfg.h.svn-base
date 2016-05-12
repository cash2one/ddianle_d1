/*
 * HorseCfg.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-19 10:22:20
 */

#ifndef __HORSECFG_H__
#define __HORSECFG_H__

#include "../../server/Macros/Macros.h"

#include <map>
#include <string>

class CFileStream;

class CHorseCfg
{
public:
    CHorseCfg();
    virtual ~CHorseCfg();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(unsigned int, m_nHorseId, HorseId);
    CLASS_PROPERTY_READONLY(unsigned int, m_nHorseItemType, HorseItemType);
    CLASS_PROPERTY_READONLY(unsigned char, m_nHorseType, m_nHorseType);
    CLASS_PROPERTY_READONLY( int, m_nMaxPassager, MaxPassager);
    CLASS_PROPERTY_READONLY( int, m_nSpeed, Speed);
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strHorseName, HorseName);
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strResourceName, ResourceName);
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strHorseAniName, HorseAniName);
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strStandbyAniName, StandbyAniName);
    CLASS_PROPERTY_READONLY_BY_REF( std::string, m_strMovingAniName, MovingAniName);
    
};

class CHorseCfgMgr
{
public:
    CLASS_INSTANCE_FUNC(CHorseCfgMgr);
    void Initialize();
    int GetMaxPassager(int nHorseId);
    int GetTypeId(int nHorseItemId);
private:
    std::map<int, CHorseCfg> m_mapHorseCfg;
};

#endif


