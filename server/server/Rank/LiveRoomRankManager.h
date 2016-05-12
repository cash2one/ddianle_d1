#ifndef _LIVE_ROOM_RANK_MANAGER_H_
#define _LIVE_ROOM_RANK_MANAGER_H_

#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_DB.h"
#include "RankMgr.h"

typedef CRankValue_Base CAuditionPointRank;
typedef CRankValue_Base CLiveRoomHotRank;

class CLiveRoomRankManager : public GameMsg_Processor
{
protected:
	CLiveRoomRankManager();
	~CLiveRoomRankManager();

public:
	static CLiveRoomRankManager &Instance();

public:
	void Init();
	void SetLiveRoomConfig(const char *pstrStartTime, const char *pstrEndTime, 
		int nLeaderBoardSize, const char *pstrFirstUpdateTime, int nUpdateInterval, int nAuditionBaseScore);
	void OnUpdate(int nTimeElapse);

protected:
	void RegMessageMap();
	void DoUpdate();

public:
	// Message begin
	void OnGetAuditionScoreList(GameMsg_Base &rMsg, CSlotPeer &rSlot);
	void OnGetHotList(GameMsg_Base &rMsg, CSlotPeer &rSlot);
	void OnGetMyAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot);

    void OnUpdateAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot);
	void OnUpdateLiveRoomHot(GameMsg_Base &rMsg, CSlotPeer &rSlot);
	// Message end

    void OnAddAuditionPunishedPlayer(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void IsPunishedPlayer(GameMsg_Base &rMsg, CSlotPeer &rSlot);
    void OnSyncAuditionScore(GameMsg_Base &rMsg, CSlotPeer &rSlot);

	// query begin
	void OnLoadPointAndHot(QueryBase &rQuery);
	// query end

protected:
	bool m_bInited;
	
	unsigned int m_nStartTime;
	unsigned int m_nEndTime;
	int m_nLeaderBoardSize;
	int m_nTideUpdateInterval;
	int m_nTideUpdateTime;
	int m_nFirstUpdateTime;
	bool m_bHasUpdateBoard;
    int m_nAuditionBaseScore;

	CRankManager_Base m_pointRankManager;
	CRankManager_Base m_hotRankManager;

    std::map<unsigned int,unsigned int>         m_AuditionPunishedPlayer;
};

#endif //_LIVE_ROOM_RANK_MANAGER_H_

