/************************************************************************
 * MedalProcess.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-8-19 10:22:20
 ***********************************************************************/

#ifndef __MEDALPROCESS_H__
#define __MEDALPROCESS_H__

#include <string>
#include "../Macros/Macros.h"
#include "../Pattern/Param.h"

#include "../logic/EntityComponent.h"

class IMedalCondition
{
public:
    IMedalCondition();
    IMedalCondition(IEntity *owner);
    virtual ~IMedalCondition();
    
public:
    virtual bool process(IParam &param);
    virtual bool check();
    virtual bool CheckIsAllowGet(int nTotalProgress);

public:
    int GetConditionID() const { return m_nConditionID; }
    int GetCurProgress() const { return m_nCurProgress; }
    int GetAreadyReward() const { return m_nAreadyReward; }
    void SetOwner(IEntity *owner);
    void SetConditionID(int conditionID);
    void SetAreadyReward(int areadyReward);
    void SetCurProgress(int curProgress);

protected:
    IEntity *m_pOwner;
    int m_nConditionID;
    int m_nCurProgress;
    int m_nAreadyReward;
};

#endif

