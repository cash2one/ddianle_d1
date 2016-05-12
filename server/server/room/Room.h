/********************************************************************
created:	2010/12/20
filename: 	datastructure\Room.h
author:		Mei jiuhua

purpose:	服务器端的房间
还没有实现跨服务器操作
*********************************************************************/
#ifdef _NSYT_CLIENT
#error 服务器文件，不应包括在客户端
#endif

#ifndef __CROOM__H__
#define __CROOM__H__

#include "../../socket/Windefine.h"
#include "../../datastructure/Macro_Define.h"
#include "../attribute/EntityAttributeComponent.h"
#include "../Rank/RankMgr.h"
#include "../roomLib/RoomDataStruct.h"
// #include "../roomLib/GameMsg_S2C_RoomMsg.h"


struct SweetheartsInfo;
class CRoomComponent;
class GameMsg_Base;
class RoomBriefInfo;
class RoomWholeInfo;
class CWholeDanceRoomTeamInfo;

class LiveRoomBriefInfo;
class LiveRoomWholeInfo;
class LiveRoomPlayerInfo;
class ITimer;
class CRoomMarkInfo;
class GMRoomInfo;
class MusicConfig;

typedef CRankValue_Base CAuditionPointRank;
typedef CRankValue_Base CLiveRoomHotRank;

class CRoom
{
public:
    CRoom(ERoomType eType, unsigned int nRoomID);
    virtual ~CRoom();

public:
    unsigned int RoomID() const;
    ERoomType RoomType() const;

    const std::string& RoomName() const;
    const std::string& RoomPwd() const;
    void ChangeRoomInfo(const char* pRoomName, const char* pRoomPwd = "", const char *pDesc = "");
    void SetMatch(bool bMatch = true);

    int ChoosedScene() const;
    int ChoosedMusicID() const;
    int ChoosedMusicMode() const;
    int ChoosedMusicLevel() const;

    int PlayScene() const;
    int PlayMusicID() const;
    int PlayMusicLevel() const;
    int PlayMusicMode() const;

    const std::string& PlayCheckCode() const;
    const std::string& PlayMusicStage() const;

    const SelectedMap& GetSelectedMap();

    void ChangeChoosedScene(int nScene);
    void ChangeChoosedMusic(int nMusicID, int nMusicMode, int nMusicLevel);

    bool HasPwd() const;
    bool PwdPassed(const char* pRoomPwd) const;

    bool IsMatch() const;
    bool HasStart() const;

    bool IsStartReady() const;
    bool IsPlayReady() const;
    bool IsEndReady() const;

    bool HasDancer() const;
    bool HasAudience() const;
    bool HasGM() const;

    void SetDancer(int nPos, CRoomComponent *pRole);// api for create test room
    void SetLiveRoomDancer(int nPos, unsigned int nRoleID, const std::string &strRoleName);
    void RemoveLiveRoomDancer(unsigned int nRoleID);
    std::string GetLiveRoomDancerName(unsigned int nRoleID);
    bool IsLiveRoomDancer(unsigned int nRoleID);
    void SetLiveRoomGroup(int nPos, unsigned int nGroupID, const std::string &strGroupName);

    int MaleDancerCount() const;
    int FemaleDancerCount() const;

    int GetFriendCount(unsigned int nRoleID);
    int GetDanceGroupPlayerCount(unsigned int nDanceGroupID);
    bool HasLover(unsigned int nRoldID);

    int MaxDancerPos() const;
    int MaxAudiencePos() const;
    int MaxGMPos() const;

    int DancerCount() const;
    int AudienceCount() const;
    int GMCount() const;

    int DancerCapacity() const;
    int AudienceCapacity() const;
    int GMCapacity() const;

    void SetAudienceCapacity(int nCount);

    int StartTime() const;
    int CreateTime() const;

    int GetCheckTime() const;
    void SetCheckTime(int nCheckTime);
    bool HasEndCountDown() const;

    int PosState(int nRoleRoomType, int nRoleRoomPos) const;
    bool ChangePosState(int nRoleRoomType, int nRoleRoomPos, int nRoomPosState);

    void SetRoomHost(CRoomComponent *pRoleRoom);
    CRoomComponent* AssignHost();
    CRoomComponent* ReassignHost(int nRoleRoomType, int nPos);
    CRoomComponent* RoomHost();

    int RoomState() const;
    void SetState(ERoomState eState);

    virtual void RoomPrepare();

    void RoomStart();
    void RoomEnd();

    void BeginToSelectPartner();
    int SelectPartner(unsigned int nSelectorRoleID, unsigned int nBeSelectedRoleID, std::string& strRes);
    void EndToSelectPartner();
    void UpdateSweethearsInfo();
    void GetSweethearsInfo(std::vector<SweetheartsInfo>& vecSweetheartsInfo);
    void AskMakeFriend();

    CRoomComponent* Player(int nRoleRoomType, int nRoleRoomPos);

    int PlayerIn(CRoomComponent& rRoleRoomCOM, int nRoleRoomType, int nRoleRoomState, bool bHost);
    CRoomComponent* PlayerOut(int nRoleRoomType, int nRoleRoomPos);

    CRoomComponent* GetAuditionRoomRivalPos(CRoomComponent * pRoleRoomCOM);
    const AuditionPlayerInfo * GetAuditionRivalPlayerInfo(unsigned int nRoleID);
    int PlayerChangeType(CRoomComponent& rRoleRoomCOM, int nRoleRoomType);

    void EncodeBriefInfo(RoomBriefInfo& rBriefInfo) const;
    void EncodeWholeInfo(RoomWholeInfo& rWholeInfo, bool bHost) const;
    void EncodeWholeRoomTeamInfo(CWholeDanceRoomTeamInfo &rWholeInfo) const;

    void EncodeLiveBriefInfo(LiveRoomBriefInfo& rBriefInfo) const;
    void EncodeLiveWholeInfo(LiveRoomWholeInfo& rWholeInfo) const;
    void EncodeLivePlayerInfo(LiveRoomPlayerInfo& rInfo, CRoomComponent* pRoleRoomCOM) const;

    void SendMsgToAll(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM = NULL);
    void SendMsgToGM(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM = NULL);
    void SendMsgToAllDancer(GameMsg_Base* pMsg, CRoomComponent* pExceptRoomCOM = NULL);

    int  RoomColor() const;
    bool RoomTop() const;

    bool LiveRoomDefaultCloth() const;
    void SetDefaultCloth(bool bDefaultCloth);

    ERoomType RoomType();

    void SetRoomTeamMode(EDanceRoomTeamMode eRoomTeamMode);
    EDanceRoomTeamMode TeamMode() const;

    int StartRoomDancersCount();

    int StartTeamsCount() const;

    void SwitchTeamMode(EDanceRoomTeamMode eTeamMode, EDanceColorTeam eCHostColorTeam);

    void PlayerJoinTeam(CRoomComponent& rRoleRoomCOM, EDanceColorTeam eTeamColor);
    void PlayerExitTeam(CRoomComponent& rRoleRoomCOM);

    CDanceTeam * GetDanceTeam(EDanceColorTeam eColorTeam);

    int TeamMemberCount(EDanceColorTeam eColorTeam) const;
    EDanceColorTeam GetAutoSelectedColorTeam();

    bool IsTeamMatched();

    void AddTeamMarkInfo(EDanceColorTeam eColorTeam, int nMark);
    int TotalTeamMark(EDanceColorTeam eColorTeam);
    void ResetTeamMark();

    int TeamExp(EDanceColorTeam eColorTeam) const;

    int TryEnterLiveRoomRoleType(CRoomComponent& rRoleRoomCOM);

    std::map<unsigned int, CRoomComponent*> & LiveRoomAudicenceMap();

    void AddDancerScore(unsigned int nTime, CLiveRoomRoundDancerScore &score);
    const std::map<unsigned int, CLiveRoomRoundDancerScore> & GetDancerScore() const;

    bool IsLiveRoom() const;
    bool IsNormalRoom() const;
    bool IsAuditionRoom() const;
    bool IsDungeonRoom() const;

    void OpenVote(bool bOpen);
    bool IsOpenVote() const;
    void OnUpdateDancerHot(int nRoleID = 0, int nHot = 0);
    void OnGetHotList(CRoomComponent &rRoleRoomCOM, int nPage);

    void AddTimer(ITimer* pTimer);

    void SetRoundRange(int nMode, int nBegin, int nEnd);//[nBegin, nEnd)
    void RemoveFrontRoundRange();
    void ClearRoundRanges();
    int GetMusicModeByRound(int nRound);
    unsigned int GetSweetRankByGroupId(unsigned int nGroupId);
    void CalcSweetRank();

    CRoomComponent* GetDancerByPos(int nPos);
    bool GetDancerInfoByPos(int nPos, unsigned int& nPlayerID, std::string& playerName);
    CRoomComponent* GetAudienceByPos(int nPos);
    bool GetAudienceInfoByPos(int nPos, unsigned int& nPlayerID, std::string& playerName);

    void ToGMRoomInfo(GMRoomInfo& gmRoomInfo);

    void AddAudiationPlayerInfo(AuditionPlayerInfo & playerinfo);

protected:
    MusicConfig* PrepareMusicAndScene();
    void PrepareCheckCode();

    CRoomComponent* _GetDancerRoomCOM(unsigned int nRoleID);
    CRoomComponent* _GetAudienceRoomCOM(unsigned int nRoleID);
    CRoomComponent* _GetRoleRoomCOM(unsigned int nRoleID);

    bool _DancerSettleIn(CRoomComponent& rRoleRoomCOM, int nDancerPos);
    bool _AudienceSettleIn(CRoomComponent& rRoleRoomCOM, int nAudiencePos);
    bool _GMSettleIn(CRoomComponent& rRoleRoomCOM, int nPos);

    CRoomComponent* _DancerMoveOut(int nDancerPos);
    CRoomComponent* _AudienceMoveOut(int nAudiencePos);
    CRoomComponent* _GMMoveOut(int nGMPos);

    bool _ChangeToDancer(CRoomComponent& rRoleRoomCOM, int nDancerPos);
    bool _ChangeToAudience(CRoomComponent& rRoleRoomCOM, int nAudiencePos);

    bool _ChangePosDancerState(int nDancerPos, int nPosState);
    bool _ChangePosAudienceState(int nAudiencePos, int nPosState);
    bool _ChangePosGMState(int nPos, int nState);

    int _GetIdleDancerPos();
    int _GetIdleAudiencePos();
    int _GetIdleGMPos();

    void _CalcSweetheartsInfo();//计算情侣组信息，例如心动值...
    void _SaveSweetheartsInfoOnPlayerLogout(unsigned int nOfflineRoleID);//玩家下线或者掉线时记下心动值和分数
    unsigned int _CalcSweetValue(CRoomMarkInfo& left, CRoomMarkInfo& right, bool bClear);

protected:
    ERoomType  m_eRoomType;
    ERoomState m_eState;
    unsigned int m_nRoomID;
    std::string m_strRoomName;
    std::string m_strRoomPwd;

    bool m_bMatch;
    bool m_bStart;

    int m_nStartTime;
    int m_nCreateTime;
    int m_nCheckTime;
    int m_nStartDeadlineTime;

    int m_nChoosedScene;
    int m_nChoosedMusicID;
    int m_nChoosedMusicMode;
    int m_nChoosedMusicLevel;

    int m_nPlayScene;
    int m_nPlayMusicID;
    int m_nPlayMusicMode;
    int m_nPlayMusicLevel;

    std::string m_strCheckCode;
    std::string m_strStageFile;

    int m_nDancerCount;
    int m_nAudienceCount;
    int m_nGMCount;

    int m_nDancerCapacity;
    int m_nAudienceCapacity;
    int m_nGMCapacity;

    int m_nStartRoomDancersCount;

    int m_nStartTeamsCount;

    bool m_bLiveRoomUseDefaultCloth;

    EDanceRoomTeamMode m_eRoomTeamMode;
    CDanceTeam m_szDanceTeam[EDanceColorTeam_Max];

    int m_szPosDancer[MAX_ROOM_PLAYER_COUNT];
    int m_szPosAudience[MAX_ROOM_AUDIENCE_COUNT];
    int m_szPosGM[MAX_ROOM_GM_COUNT];

    CRoomComponent* m_szDancer[MAX_ROOM_PLAYER_COUNT];
    CRoomComponent* m_szAudience[MAX_ROOM_AUDIENCE_COUNT];
    CRoomComponent* m_szGM[MAX_ROOM_GM_COUNT];

    std::map<unsigned int, CRoomComponent*> m_mapLiveRoomAudience;
    unsigned int m_szLiveRoomDancerID[MAX_ROOM_PLAYER_COUNT];
    std::string m_szLiveRoomDancerName[MAX_ROOM_PLAYER_COUNT];
    unsigned int m_szLiveRoomGroupID[MAX_ROOM_PLAYER_COUNT];
    std::string m_szLiveRoomGroupName[MAX_ROOM_PLAYER_COUNT];
    std::string m_strLiveRoomDescription;
    std::map<unsigned int, AuditionPlayerInfo> m_AuditionRoleInfo;

    std::map<unsigned int, CLiveRoomRoundDancerScore> m_mapLiveDanceScore;

    bool m_bOpenVote;
    unsigned int m_nLastSendHotValueTime;
    CRankManager_Base m_hotRankManager;

    CRoomComponent* m_pHostRoomCOM;

    std::set<unsigned int> m_setSexDancer[ESexType_Max];//按性别区分舞者
    SelectingMap m_mapSelectingPartner;//情侣模式搭档选择记录
    SelectedMap m_mapSelectedPartner;//情侣模式确定的搭档组合 <组号,组信息>
    std::map<unsigned int, unsigned int> m_mapDancer2Sex;//情侣模式玩家的性别记录，游戏中即使玩家下线也不删除
    std::vector<ITimer*> m_vecTimersOnStart;//在房间开始游戏时开启一些计时器
    std::list<RoundRange> m_listRoundRange;//情侣模式中各个Round范围对应的MusicMode
};

#endif

