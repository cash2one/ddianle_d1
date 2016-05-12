/*
 * DanceGroupChallengeProcessor.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-9-17 19:11:33
 */

#ifndef __DANCEGROUPCHALLENGEPROCESSOR_H__
#define __DANCEGROUPCHALLENGEPROCESSOR_H__

#include "../Macros/Macros.h"
#include "../../socket/GameMsg_Processor.h"
#include <map>

class DanceGroupChallengeInfo;
class DanceGroupChallengeContribution;

/**
* DanceGroupChallengeProcessor:
* 
*/

class DanceGroupChallengeProcessor : public GameMsg_Processor
{
private:
    DanceGroupChallengeProcessor();
    virtual ~DanceGroupChallengeProcessor();

public:
    CLASS_INSTANCE_FUNC(DanceGroupChallengeProcessor)

    void Initialize();
    void OnUpdate( time_t tNow );

public:
    void SetInspireInfos( unsigned int nPercentForAll, unsigned int nPercentForSelf, 
        unsigned int nMaxCountForAll, unsigned int nMaxCountForSelf, int nRewardContribution );
    void SetResetInfos( int nWeekday, int nHour );

private:
    void RegisterMessage();

    void Clear();

    void ResetStartTime();

    unsigned int GetAllInspire( const DanceGroupChallengeContribution *pContribution ) const;
    unsigned int CalcScoreAdditional( unsigned int nMyInspire, unsigned int nAllInspire ) const;

private:
    void OnEnterChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetChallengeStoreyInfo(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnGetChallengeRankList(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnChallengeInspire(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnOpenChallengeStorey(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnPrepareChallenge(GameMsg_Base &msg, CSlotPeer &slotPeer);
    void OnReportChallengeScore(GameMsg_Base &msg, CSlotPeer &slotPeer);

    void OnLoadAllInfo(QueryBase &rQuery);
    void OnRecordRanking(QueryBase &rQuery);

private:
    bool    m_bIsOpening;
    bool    m_bLoadComplete;

    unsigned int m_nInspirePercentForAll;
    unsigned int m_nInspirePercentForSelf;
    unsigned int m_nInspireMaxCountForAll;
    unsigned int m_nInspireMaxCountForSelf;
    int m_nInspireRewardGroupContribution;

    int     m_nResetWeekday;
    int     m_nResetHour;
    unsigned int m_nStartTime;
    unsigned int m_nNextStartTime;

    std::map<unsigned int, DanceGroupChallengeInfo *> m_mapAllInfo;
};

#endif // __DANCEGROUPCHALLENGEPROCESSOR_H__

