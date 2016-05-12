/********************************************************************
created:	2010/12/20
filename: 	datastructure\Room.h
author:		Meijiuhua

purpose:	服务器端的房间管理
还没有实现跨服务器操作
*********************************************************************/

#ifndef __CRoomManager__H__
#define __CRoomManager__H__

#include "Room.h"
#include "../../datastructure/Macro_Define.h"
#include "../../socket/GameMsg_Processor.h"
#include "../../datastructure/datainfor/GMDataInfo.h"

class CRoomComponent;
// enum ESwitchDanceRoomTeamModeRes;
// enum EJoinDanceRoomTeamRes;

class CRoomManager
{
private:
    enum ERoomRange
    {
        ERoomRange_Common_Begin = 1,		//普通房间开始房间号
        ERoomRange_Common_End = 999,		//普通房间结束房间号
        ERoomRange_NewPlayer_Begin = 10001,	//新手房间开始房间号
        ERoomRange_NewPlayer_End = 10999,	//新手房间结束房间号
    };
public:
    typedef std::list<unsigned int> RoomIDList;
    typedef std::list<CRoomComponent*> MatchList;
    typedef std::map<unsigned int, CRoomComponent*> PlayerMap;
    typedef std::map<unsigned int, CRoom*> AllRoomMap;					//key - roomID
    typedef std::multimap<unsigned long, CRoom*> TimeRoomMap;		//key - time
    typedef std::list<CRoom*> TopRoomList;

protected:
    CRoomManager();
    ~CRoomManager();

public:
    //根据房间号获取房间类型
    static ERoomType RoomType(unsigned int nRoomID);
    static CRoomManager& Instance();

    CRoom* FindRoom(unsigned int nRoomID);
    void DestroyRoom(bool bOldTopState, unsigned int nRoomID, int nFlag, int nErr = 0);

    bool AddRoom(CRoom* pRoom);
    CRoom* RemoveRoom(unsigned int nRoomID);
    void RemoveTopRoom(CRoom* pRoom);

    void HandleTopRoom(bool bOldTopState, CRoom* pRoom);

    unsigned long GetMgrTime();

    const AllRoomMap& GetRooms(unsigned char nMode);
    const TopRoomList& GetTopRooms(unsigned char nMode);

    void OnUpdate(const unsigned long& lTimeElapsed);

    void GetLobbyPlayersID(std::vector<unsigned int> &rPlayersID) const;

public:
    void OnEnterLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnExitLobby(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnGetRoomList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnGetInviteeList(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnCreateRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnQuitRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnStartRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnReadyRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnEndRoom(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);


    void OnTryEnterPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnEnterPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnKickPlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnApplyMatch(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnCancelMatch(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnInvitePlayer(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnInviteeResponse(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnInEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnOutEquip(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnChangeRoomInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnChangeMusicInfo(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnChangeRoleRoomType(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnChangeRoleRoomState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnChangeRoomPosState(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnPromoteRoomHost(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnLoadingStartGameProgress(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnSwitchDanceRoomMode(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnJoinDanceRoomTeam(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);

    void OnReportRoundMark(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnReportEffectChange(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnQuitMarkAward(const GameMsg_Base& rMsg, CRoomComponent& rRoleRoomCOM);
    void OnRoleLogout(CRoomComponent& rRoleRoomCOM);

    CRoomComponent* GetLobbyPlayer(unsigned int nRoleID);
    void SendMsgInLobby(GameMsg_Base* pMsg);
    void SendLobbyChatMsg(unsigned int nSenderID, GameMsg_Base *pMsg);

    void ExitLobby(CRoomComponent& rRoleRoomCOM);

    void CreateTestRoom(int nRoomCount, CRoomComponent *pHost);

    static void StartSweetheartsMode(CRoom* pRoom, std::vector<char>& vecAndroidData, std::vector<char>& vecIosData);

    void GetRoomListByGM(std::vector<GMRoomInfo>& roomList);

protected:
    unsigned int _GenerateRoomID(unsigned char nRoomType);
    unsigned int _GenerateCommonRoomID();
    unsigned int _GenerateNewPlayerRoomID();
    void _ReleaseRoomID(unsigned int nRoomId);

    void _EnterPlayer(CRoomComponent& rRoleRoomCOM, CRoom *pRoom, const std::string& strPwd, bool bAudience);

    int _PlayerInRoom(CRoomComponent& rRoleRoomCOM, CRoom* pRoom, int nRoleType, int nRoleState, bool bHost, int nInType);
    void _PlayerOutRoom(CRoomComponent& rOutRoleRoomCOM, int nOutType, int nOutTo, CRoomComponent * pKickedPlayerRoomCOM);

    int _PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleType);
    void _PlayerChangeState(CRoomComponent& rRoleRoomCOM, int nRoleState, bool bNotice);

    void _SpecificModeMatch();
    void _RandomModeMatch();

    CRoom* _CreateMatchRoom(int nScene, int nMode, int nLevel);
    void _PrepareMatchRoom(CRoom* pRoom);

    bool _ChangeRoomPosState(CRoom* pRoom, int nRoleRoomType, int nRoleRoomPos, int nRoomPosState);

    void _QuitMatchList(CRoomComponent& rRoleRoomCOM);

    void _PlayerOutLobby(CRoomComponent& rRoleRoomCOM);

    void _InStartingList(CRoom* pRoom);
    void _OutStartingList(CRoom* pRoom);

    void _InEndingList(CRoom* pRoom);
    void _OutEndingList(CRoom* pRoom);

    void _RoomStart(CRoom* pRoom);
    void _RoomEnd(CRoom* pRoom);

    void _AutoStart(CRoom* pRoom);
    void _AutoEnd(CRoom* pRoom);

    void _RoomSwitch(int nOldMode, CRoom* pRoom);

    void _RoomRanking(CRoom* pRoom);
    void _RoomReward(CRoom* pRoom);

    int _CalcuGrade(CRoomComponent& rRoleRoomCOM);
    unsigned int _CalcuExp(CRoomComponent& rRoleRoomCOM, int nDancerCount, int nMusicMode, int nMusicLevel);

    void _TeamRank(CRoom *pRoom);
    void _CalcuTeamExp(CRoom *pRoom);

    void _NoticeGroupState(unsigned int nRoleID, unsigned int nRoomID, int nRoleRoomType, int nRoleRoomState);

    bool _CanSwitchRoomTeamMode(CRoomComponent& rRoleRoomCOM, EDanceRoomTeamMode eTeamMode, EDanceColorTeam eColorTeam, ESwitchDanceRoomTeamModeRes &eRes, std::string & strError);
    bool _CanJoinTeam(CRoomComponent& rRoleRoomCOM, EDanceColorTeam eColorTeam, EJoinDanceRoomTeamRes &eRes, std::string & strError);

    //记录角色跳舞信息
    void _LogRoleDancing(unsigned int nRoleID, CRoleDancingLog::EDancingAction eDancingAction,
        int nScene, int nTeamMode, int nMusicID, int nMusicMode, int nMusicLevel, int nPlayerCount, int nRank, int nScore);

    void _CreateMusicUpdateInfo(CRoomComponent* pRoomCOM, QUESTUPDATEINFO& info);

protected:
    unsigned long m_nMgrNowTime;
    unsigned long m_nMatchTime;

    RoomIDList m_IdleID;
    RoomIDList m_StoreID;
    RoomIDList m_NewPlayerIdleID;
    RoomIDList m_NewPlayerStoreID;

    PlayerMap m_LobbyPlayer;
    TimeRoomMap m_NeedStartRooms;
    TimeRoomMap m_NeedEndRooms;

    MatchList m_AllMatchs[MusicMode_Max];

    AllRoomMap m_AllRooms[MusicMode_Max];
    TopRoomList m_TopRooms[MusicMode_Max];
};

#endif

