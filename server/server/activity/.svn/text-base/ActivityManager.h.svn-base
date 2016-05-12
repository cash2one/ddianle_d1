#ifndef _ACTIVITY_MANAGER_H_
#define _ACTIVITY_MANAGER_H_

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_DB.h"
#include <deque>

class ActivityManager : public GameMsg_Processor
{
protected:
	ActivityManager();
	~ActivityManager();

public:
	static ActivityManager & Instance();

public:
    void Initialize();
    void RegMessageMap();

public://......长效累冲......
    void OnAddLongactingRechargeBoardCast(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnGetLongactingRechargeBoardCast(GameMsg_Base &rMsg, CSlotPeer &rSlot);

public://......舞团红包..........
    // 请求创建红包
    void OnRequestCreateRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // 初始化红包信息
    void InitializeRedEnvelope();

    // 加载完成红包信息
    void OnLoadDanceGroupRedEnvelope(QueryBase &rQuery);

    // get dancegroup all redenvelope list
    void OnGetRedEnvelopeList(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // redenvelope details
    void OnGetRedEnvelopeDetails(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // open redenvelope
    void OnOpenRedEnvelope(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // get redenvelope left count
    void OnGetCanOpenRedEnvelopeCount(GameMsg_Base &msg, CSlotPeer &slotPeer);

    // log 日志
    void InsertIntoLog(unsigned int nRoleID, CRedEnvelopeLog::ERedEnvelopeAction eAction,
        unsigned int nRedEnvelopeID, int nOpenIndex, int nMoney);

public:
    void SendPlayerMsgByRoleID(unsigned int nRoleID, GameMsg_Base &msg);

private:
    std::deque<LongactingCumulativeRechargeBoardCast>      m_deqBoardCase;
    bool                                                   m_bRedEnvelopeComplete;    // 是否加载完毕
};

#endif //_LIVE_ROOM_RANK_MANAGER_H_

