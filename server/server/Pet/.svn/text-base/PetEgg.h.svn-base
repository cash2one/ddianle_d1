/*
 * PetEgg.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-5 16:14:06
 */

#ifndef __PETEGG_H__
#define __PETEGG_H__

#include "../Macros/Macros.h"
#include "../Pattern/Observer.h"

enum ePetEggState
{
    ePetEggState_Incubating = 0,        // 孵化状态
    ePetEggState_Finished,              // 孵化结束
    ePetEggState_Opened,                // 宠物蛋已开启

    ePetEggState_Max
};

/**
* PetEgg:
* 
*/

class PetEgg : public CommonPublisher
{
protected:
    PetEgg();

public:
    CLASS_CREATE_FUNC(PetEgg);
    virtual ~PetEgg();

public:
    void OnUpdate(unsigned int nNow);

    bool IsIncubating() const;
    bool IsFinished() const;

    void SpeedUp();
    void Opened();

    void SetState(unsigned char nState);

    CLASS_PROPERTY(unsigned int, m_nID, ID);                        // 宠物蛋ID，由数据库生成
    CLASS_PROPERTY_READONLY(unsigned char, m_nState, State);        // 宠物蛋状态
    CLASS_PROPERTY(unsigned int, m_nFinishedTime, FinishedTime);    // 孵化结束时间
    CLASS_PROPERTY(unsigned int, m_nTypeID, TypeID);                // 宠物蛋类型ID，和物品typeid一致
};

#endif // __PETEGG_H__

