/************************************************************************
 * HandbookGlobalInfo.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: luhaoting
 * Create time: 2015-9-15 10:22:20
 ***********************************************************************/

#ifndef __HANDBOOKGLOBALINFO_H__
#define __HANDBOOKGLOBALINFO_H__

#include "../group/Global.h"
#include "../../socket/GameMsg_Processor.h"
#include "HandbookStruct.h"
#include "../Macros/Macros.h"

#include <map>

class CHandbookGlobalMgr : public GameMsg_Processor
{
protected:
    CHandbookGlobalMgr();
    virtual ~CHandbookGlobalMgr();

public:
    CLASS_INSTANCE_FUNC(CHandbookGlobalMgr);
    void Initialize();
    void OnUpdate(const unsigned long & nTimeElapsed);

    void RegComponentNetMsgMap();

public:

    void SetClosed();

    void RetMsgGolbalCostumeData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void RetMsgGolbalEffectData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void ChangeGolbalData(GameMsg_Base& rMsg, CSlotPeer& rSlotPeer);
    void ChangeData(std::map<int , CHandbookUnit>& destMap, int Id , int index , bool isAdd);
private:

    bool m_bIsOpening;
    long m_nInterval;
    std::map<int , CHandbookUnit> m_MapCostume;
    std::map<int , CHandbookUnit> m_MapEffect;
};

#endif//__HANDBOOKGLOBALINFO_H__
