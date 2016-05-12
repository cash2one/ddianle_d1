/************************************************************************
 * MedalProcess.cpp
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-19 10:22:20
 ***********************************************************************/

#include "MedalCondition.h"

IMedalCondition::IMedalCondition() 
    : m_pOwner(NULL)
    , m_nConditionID(0)
    , m_nCurProgress(0)
    , m_nAreadyReward(0)
{
}

IMedalCondition::IMedalCondition(IEntity *owner)
    : m_pOwner(owner)
    , m_nConditionID(0)
    , m_nCurProgress(0)
    , m_nAreadyReward(0)
{
}

IMedalCondition::~IMedalCondition()
{
}

bool IMedalCondition::process(IParam &param)
{
    return true;
}

bool IMedalCondition::check()
{
    return true;
}

bool IMedalCondition::CheckIsAllowGet(int nTotalProgress)
{
    return (GetCurProgress() >= nTotalProgress && GetAreadyReward() == 0);
}


void IMedalCondition::SetOwner(IEntity *owner)
{
    m_pOwner = owner;
}

void IMedalCondition::SetConditionID(int conditionID)
{
    m_nConditionID = conditionID;
}

void IMedalCondition::SetAreadyReward(int areadyReward)
{
    m_nAreadyReward = areadyReward;
}

void IMedalCondition::SetCurProgress(int curProgress)
{
    m_nCurProgress = curProgress;
}
