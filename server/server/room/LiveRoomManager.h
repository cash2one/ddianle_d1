/********************************************************************
	created:	2010/12/20
	filename: 	datastructure\Room.h
	author:		Meijiuhua
	
	purpose:	服务器端的房间管理
				还没有实现跨服务器操作
*********************************************************************/

#ifndef __CLiveRoomManager__H__
#define __CLiveRoomManager__H__

#include "Room.h"
#include "../Rank/RankMgr.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"

class CRoomComponent;

class CLiveRoomManager : public GameMsg_Processor
{
public:
	typedef std::list<unsigned int> RoomIDList;
	typedef std::multimap<int, CRoomComponent*> AuditionMatchMap;	//key - score
	typedef std::map<unsigned int, CRoomComponent*> PlayerMap;
	typedef std::map<unsigned int, CRoom*> AllRoomMap;				//key - roomID
	typedef std::multimap<unsigned long, CRoom*> TimeRoomMap;		//key - time
	typedef std::list<CRoom*> TopRoomList;

protected:
	CLiveRoomManager();
	~CLiveRoomManager();

protected:
	void RegNetMessage();

	void OnDBGetDancerName(QueryBase &query);
	void OnDBUpdateDancerName(QueryBase &query);

public:
	static CLiveRoomManager& Instance();

public:
	void Init();

public:
	CRoom* FindRoom(unsigned int nRoomID);
	void DestroyRoom(bool bOldTopState, unsigned int nRoomID, int nFlag);

	bool AddRoom(CRoom* pRoom);
	CRoom* RemoveRoom(unsigned int nRoomID);

	unsigned long GetMgrTime();

	const AllRoomMap& GetRooms(unsigned char nMode);

	void OnUpdate(const unsigned long& lTimeElapsed);

	void RefreshMaxAudienceCount();

public:
	void OnEnterLiveLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnExitLiveLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnGetLiveRoomList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnCreateLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnEnterLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnQuitLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnQuitRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnCloseLiveRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnStartRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnReadyRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnEndRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnInEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnOutEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnChangeRoleRoomState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnChangeMusicInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnSetLiveRoomDefaultCloth(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnGetLiveRoomScore(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnKickPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnUpdateLiveRoomDancer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnSetLiveRoomDancerToAudience(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnLoadingStartGameProgress(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnReportRoundMark(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnReportEffectChange(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnQuitMarkAward(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnRoleLogin(CRoomComponent& rRoleRoomCOM);
	void OnRoleLogout(CRoomComponent& rRoleRoomCOM);

	//////////////////////////////////////////////////////////////////////////
	void OnOpenVote(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnGetLiveRoomHotList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnGetLiveRoomAuditionScoreList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnGetMyAuditionScore(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
	void OnVote(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnLiveRoomJoinAudition(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void _OnLiveRoomJoinAudition( GameMsg_Base& rMsg, CSlotPeer&rSlotPeer);
    void _OnSyncAuditionScore( GameMsg_Base& rMsg, CSlotPeer&rSlotPeer );
	void OnLiveRoomCancelAudition(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

	void OnGetLiveRoomHotListRes(GameMsg_Base& rMsg, CSlotPeer &rSlot);
	void OnGetLiveRoomAuditionScoreListRes(GameMsg_Base& rMsg, CSlotPeer &rSlot);
	void OnGetMyAuditionScoreRes(GameMsg_Base& rMsg, CSlotPeer &rSlot);

public:
	CRoomComponent* GetLobbyPlayer(unsigned int nRoleID);
	void SendMsgInLobby(GameMsg_Base* pMsg);
	void SendLobbyChatMsg(unsigned int nSenderID ,GameMsg_Base *pMsg);

	void ExitLobby(CRoomComponent& rRoleRoomCOM);

	void GetRoomListByGM(std::vector<GMRoomInfo>& roomList);

protected:
	unsigned int _GenerateRoomID(unsigned char nRoomType);
	void _ReleaseRoomID(unsigned int nRoomId);
	void _EnterPlayer( CRoomComponent& rRoleRoomCOM, CRoom *pRoom );

	int _PlayerInRoom(CRoomComponent& rRoleRoomCOM, CRoom* pRoom, int nRoleType, int nRoleState, bool bHost, int nInType);
	void _PlayerOutRoom(CRoomComponent& rOutRoleRoomCOM, int nOutType, int nOutTo, CRoomComponent * pKickedPlayerRoomCOM);

	int _PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleType);
	void _PlayerChangeState(CRoomComponent& rRoleRoomCOM, int nRoleState, bool bNotice);

	bool _ChangeRoomPosState(CRoom* pRoom, int nRoleRoomType, int nRoleRoomPos, int nRoomPosState);

	void _PlayerOutLobby(CRoomComponent& rRoleRoomCOM);

	void _InStartingList(CRoom* pRoom);
	void _OutStartingList(CRoom* pRoom);

	void _InEndingList(CRoom* pRoom);
	void _OutEndingList(CRoom* pRoom);

	void _RoomStart(CRoom* pRoom);
	void _RoomEnd(CRoom* pRoom);

    // 海选房间获取积分
    int GetAuditionRoomResult(CRoom * pRoom, CRoomComponent * pRoomComponent);

	void _AutoStart(CRoom* pRoom);
	void _AutoEnd(CRoom* pRoom);

	void _RoomSwitch(int nOldMode, CRoom* pRoom);

	void _RoomRanking(CRoom* pRoom);
	void _RoomReward(CRoom* pRoom);

	int _CalcuGrade(CRoomComponent& rRoleRoomCOM);
	unsigned int _CalcuExp(CRoomComponent& rRoleRoomCOM, int nDancerCount, int nMusicMode, int nMusicLevel);

	void _NoticePlayerChange(CRoom* pRoom);

	void _NoticeGroupState(unsigned int nRoleID, unsigned int nRoomID, int nRoleRoomType, int nRoleRoomState);

	

	void _AuditionMatch();
	void _QuitAuditionList(CRoomComponent& rRoleRoomCOM);

	CRoom* _CreateMatchRoom();
    void CheckDestroyRoom( CRoom * pRoom );
	void _PrepareMatchRoom(CRoom* pRoom);

	//记录角色跳舞信息
	void _LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
		int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore);

    // 海选日志
    void InsertAuditionActionLog(CRoleAuditionLog::EAuditionAction eAction, unsigned int nRoleID, unsigned int nMatchRoleID);

protected:
	bool m_bInited;
	unsigned long m_nMgrNowTime;

	RoomIDList m_IdleAuditionRoomID;
	RoomIDList m_IdleLiveRoomID;

	PlayerMap m_LobbyPlayer;

	AllRoomMap m_AllRooms[MusicMode_Max];

	TimeRoomMap m_NeedStartRooms;
	TimeRoomMap m_NeedEndRooms;

	AuditionMatchMap m_matchMap;

	int m_nMatchTime;

	unsigned int m_nLastSyncVoteTime;


	std::set<CRoom*>	m_setDestroyedRoom;                 //  已销毁房间set集合
	map<unsigned int, CRoom*>  m_mapDestroyedRoom;          //  已销毁房间key --> value :  房间ID: 房间指针

    std::vector<CRoom*> m_TempNeedEndRooms;                 //  update 需要销毁房间零时拷贝
};

#endif

//file end

