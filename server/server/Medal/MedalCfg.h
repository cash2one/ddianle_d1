/*
 * MedalCfg.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-19 10:22:20
 */

#ifndef __MEDALCFG_H__
#define __MEDALCFG_H__


#include "../../server/Macros/Macros.h"

#include <list>
#include <string>

class CFileStream;


class CMedalCfg
{
public:
    CMedalCfg();
    virtual ~CMedalCfg();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_nId, Id);
    CLASS_BOOL_PROPERTY_READONLY(m_bIsForever, Forever);            //是否永久
    CLASS_PROPERTY_READONLY(int, m_nDuration, Duration);            //持续时间(秒)
    CLASS_PROPERTY_READONLY(int, m_nType, Type);                    //勋章类型，用于判断同类勋章只有一个可以“激活”
    CLASS_PROPERTY_READONLY(int, m_nNeedMedalMoney, NeedMedalMoney);//兑换需要的勋章币
    CLASS_PROPERTY_READONLY(int, m_nSortKey, SortKey);              //排序优先级
    CLASS_PROPERTY_READONLY(int, m_nMedalScore, MedalScore);        //勋章积分

    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strName, Name);                   //勋章名字
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strTypeDescribe, TypeDescribe);   //勋章类型描述
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strDescribe, Describe);           //勋章描述
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strIcon, Icon);                   //icon
};


class CMedalProgressCfg
{
public:
    CMedalProgressCfg();
    virtual ~CMedalProgressCfg();

public:
    bool LoadInfoFromFile(CFileStream &rFileStream);

    CLASS_PROPERTY_READONLY(int, m_nId, Id);                                //进度ID
    CLASS_PROPERTY_READONLY(int, m_nTotalProgress, TotalProgress);          //总进度值
    CLASS_PROPERTY_READONLY(int, m_nType, Type);                            //进度类型
    CLASS_PROPERTY_READONLY(int, m_nMedalID, MedalID);                      //可以获得的勋章id
    CLASS_PROPERTY_READONLY(int, m_nSortKey, SortKey);                      //排序优先级
    CLASS_PROPERTY_READONLY(int, m_nVisibility, Visibility);                      //性别可见性
    CLASS_PROPERTY_READONLY(unsigned int, m_nAllowGetStartTime, AllowGetStartTime);//允许领取的开始时间
    CLASS_PROPERTY_READONLY(unsigned int, m_nAllowGetEndTime, AllowGetEndTime);//允许领取的结束时间
    CLASS_PROPERTY_READONLY_BY_REF(std::string, m_strDescribe, Describe);   //条件描述
    CLASS_PROPERTY_READONLY_BY_REF(std::list<unsigned int>, m_listSpecialParams, SpecialParams); //完成特殊需求的参数，一般 填写 itemid
};


#endif //__MEDALCFG_H__
