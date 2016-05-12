#ifndef __ROOMDATASTRUCT_H__
#define __ROOMDATASTRUCT_H__

#include <string>
#include <map>

#include "../../datastructure/Macro_Define.h"
#include "../../datastructure/DataStruct_Base.h"
#include "../../socket/ParamPool.h"

class CRoomComponent;

#define MAX_ROOMKEY_LENGTH 8

enum ERoomState
{
    eRoomState_Idle         = 0x0,  //idle
    eRoomState_Preparing,
    eRoomState_Waiting,
    eRoomState_Playing,
    eRoomState_SelectPartner,
    eRoomState_Ending,
    eRoomState_End,
};

enum ERoomType
{
    eRoomType_None,                             // 表示玩家未在房间中
    eRoomType_Unknown = eRoomType_None,          //未定义的房间
    
    eRoomType_Common,           //普通房间
    eRoomType_NewPlayer,        //新手房间

    eRoomType_LiveRoomPlayer,   //直播房间：玩家比赛
    eRoomType_LiveRoomGroup,    //直播房间：舞团比赛
    eRoomType_LiveRoomOther,    //直播房间：其它类型

    eRoomType_LiveRoomAudition, //直播房间：海选房间
    eRoomType_Dungeon,          //副本

    eRoomType_DanceGroupCeremony,   // 舞团入团仪式
    eRoomType_DanceGroupChallenge,  // 舞团试炼

    eRoomType_BigMama,              // 大妈房间

    eRoomType_FairyLandRoom,       // 秘境房间

    eRoomType_Max,
};



enum EGetInviteeListRes
{
    GetInviteeList_NoRoom,
    GetInviteeList_NotHost,
    GetInviteeList_TooFrequent,

    GetInviteeList_Success,
};

enum ECreateRoomRes
{
    CreateRoom_Unknow,
    CreateRoom_InRoom,
    CreateRoom_EmptyName,
    CreateRoom_NameTooLong,
    CreateRoom_PwdTooLong,
    CreateRoom_RoomEnough,
    CreateRoom_NoPriv,
    CreateRoom_InvalidRoomType,
    CreateRoom_DancerNotExist,

    CreateRoom_Success,

    //new
    CreateRoom_InWeddingRoom,
    CreateRoom_InPhotoRoom,
    CreateRoom_InAmuseRoom,
    CreateRoom_InCeremonyRoom,

    CreateRoom_NotInOpenTime,
    CreateRoom_NoMoreFailyLandRoomStage,
    CreateRoom_NotHasDanceGroup,
    CreateRoom_NotXiaoMengXin,
};

enum EEnterRoomRes
{
    EnterRoom_Unknow,
    EnterRoom_InRoom,
    EnterRoom_NoRoom,
    EnterRoom_IncorrectPwd,
    EnterRoom_RoomStart,
    EnterRoom_RoomEnd,
    EnterRoom_AudienceFull,
    EnterRoom_DancerFull,
    EnterRoom_RequirePwd,

    EnterRoom_Success,
    EnterRoom_NewPlayer,		//新手房间

    //added
    EnterRoom_InWeddingRoom,
    EnterRoom_InPhotoRoom,
    EnterRoom_InAmuseRoom,
    EnterRoom_RoomTypeNotMatch,
    EnterRoom_InCeremonyRoom,

    EnterRoom_NotSameDanceGroup,            // 玩家不是同一舞团
    EnterRoom_NotXiaoMengXin,               // 不能是小萌新
    EnterRoom_NotInOpenTime,                // 非活动时间
    EnterRoom_NoMoreFailyLandRoomStage,     // 活动关卡都打完了
};

enum EQuitRoomRes
{
    QuitRoom_NoRoom,
    QuitRoom_HasReady,
    QuitRoom_HasStart,

    QuitRoom_Success,
};

enum EStartRoomRes
{
    StartRoom_NoRoom,
    StartRoom_NotHost,
    StartRoom_HasStart,
    StartRoom_NotReady,

    StartRoom_Success,
    StartRoom_NotMatchTeamMode,
    StartRoom_NotMatchPlayerCount,

    StartLiveRoom_NoPriv,
    StartLiveRoom_NoDancer,
    StartLiveRoom_HasEnd,
    
    StartRoom_NoMusic,                  // missed music configs.
    StartRoom_NoDanceRoom,              // not dance room.
    StartRoom_NotInOpenTime,            // 不在开启时间
    StartRoom_NoFailyLandRoomTimes,     // 没有活动次数了。
    StartRoom_NoMoreFailyLandRoomStage, // 所有关卡打完了
    StartRoom_Unknown,
};

enum EKickPlayerRes
{
    KickPlayer_NoRoom,
    KickPlayer_NotHost,
    KickPlayer_HasStart,
    KickPlayer_NoPlayer,
    KickPlayer_IsSelf,
    KickPlayer_PoorVip,
    KickPlayer_NoAuthority,
    KickPlayer_CanNotKickHost,
    KickPlayer_CanNotKickNewPlayer,
    KickPlayer_AudienceCanNotKickPlayer,
    KickPlayer_CanNotKickGM,

    KickPlayer_Success,
};

enum EInvitePlayerRes
{
    InvitePlayer_NoRoom,
    InvitePlayer_NotHost,
    InvitePlayer_TooMany,
    InvitePlayer_TooFrequent,

    InvitePlayer_Success,
    InvitePlayer_NewPlayer,		//新手房间不能邀请
    InvitePlayer_InWeddingRoom,
    InvitePlayer_InPhotoRoom,
    InvitePlayer_InAmuseRoom,
    InvitePlayer_InCeremonyRoom,
};

enum EInviteeResponseRes
{
    InviteeResponse_InRoom,
    InviteeResponse_NoRoom,
    InviteeResponse_NoPos,
    InviteeResponse_NoAuth,
    InviteeResponse_HasStart,

    InviteeResponse_Success,
    InviteeResponse_NewPlayer,		//新手房间不能进入邀请
    InviteeResponse_InWeddingRoom,
    InviteeResponse_InPhotoRoom,
    InviteeResponse_InAmuseRoom,
    InviteeResponse_InCeremonyRoom,
};

enum EChangeRoomInfoRes
{
    ChangeRoomInfo_NoRoom,
    ChangeRoomInfo_NotHost,
    ChangeRoomInfo_EmptyName,
    ChangeRoomInfo_NameTooLong,
    ChangeRoomInfo_PwdTooLong,
    ChangeRoomInfo_HasStart,

    ChangeRoomInfo_Success,
};

enum EChangeMusicInfoRes
{
    ChangeMusicInfo_NoRoom,
    ChangeMusicInfo_NotHost,
    ChangeMusicInfo_NoPriv,
    ChangeMusicInfo_HasStart,
    ChangeMusicInfo_NoScene,
    ChangeMusicInfo_NoMode,
    ChangeMusicInfo_NoLevel,
    ChangeMusicInfo_NoMusic,
    ChangeMusicInfo_InvalidInstaller,

    ChangeMusicInfo_Success,
};

enum EChangeRoleRoomTypeRes
{
    ChangeRoleRoomType_NoRoom,
    ChangeRoleRoomType_IsHost,
    ChangeRoleRoomType_HasStart,
    ChangeRoleRoomType_NoType,
    ChangeRoleRoomType_NoChange,
    ChangeRoleRoomType_IsReady,
    ChangeRoleRoomType_NoPos,

    ChangeRoleRoomType_Success,
};

enum EChangeRoleRoomStateRes
{
    ChangeRoleRoomState_NoRoom,
    ChangeRoleRoomState_IsHost,
    ChangeRoleRoomState_NotDancer,
    ChangeRoleRoomState_HasStart,
    ChangeRoleRoomState_NoState,
    ChangeRoleRoomState_NoChange,
    ChangeRoleRoomState_NoChanllageTime, // 没有挑战次数

    ChangeRoleRoomState_Success,

    ChangeRoleRoomState_Unknown,
};

enum EChangeRoomPosStateRes
{
    ChangeRoomPosState_NoRoom,
    ChangeRoomPosState_NotHost,
    ChangeRoomPosState_HasStart,
    ChangeRoomPosState_Occupied,
    ChangeRoomPosState_NoState,
    ChangeRoomPosState_NoChange,

    ChangeRoomPosState_Success,
};

enum EPromoteRoomHost
{
    PromoteRoomHost_Sucess,
    PromoteRoomHost_None = PromoteRoomHost_Sucess,
    PromoteRoomHost_OldHostNotInRoom,
    PromoteRoomHost_RoomNotExist,
    PromoteRoomHost_NewHostNotInRoom,
    PromoteRoomHost_NoAuthority,
    PromoteRoomHost_UnKnow,
};

enum ESwitchDanceRoomTeamModeRes
{
    ESwitchDanceRoomTeamModeRes_Suc,
    ESwitchDanceRoomTeamModeRes_NotInRoom,
    ESwitchDanceRoomTeamModeRes_NotRoomHost,
    ESwitchDanceRoomTeamModeRes_InvalidTeamMode,
    ESwitchDanceRoomTeamModeRes_InvalidTeamColor,
    ESwitchDanceRoomTeamModeRes_TeamModeNotChange,
};

enum EJoinDanceRoomTeamRes
{
    EJoinDanceRoomTeamRes_Suc,
    EJoinDanceRoomTeamRes_NotInRoom,
    EJoinDanceRoomTeamRes_InvalidColorTeam,
    EJoinDanceRoomTeamRes_NotChange,
    EJoinDanceRoomTeamRes_NotDancer,
    EJoinDanceRoomTeamRes_InReadyState,
};

enum EInvitedType
{
    Invited_None,
    Invited_Lobby = 1,
    Invited_Friend = 2,
};

enum ESelectPartner
{
    ESelectPartner_Success,
    ESelectPartner_NoRoom,
    ESelectPartner_NotRightPartner,
    ESelectPartner_Selected
};


#define MAX_DANCE_TEAM_MARK 999999999

class CDanceTeam
{
public:
    CDanceTeam();
    ~CDanceTeam();

    bool operator < (const CDanceTeam & rRhs);

public:
    void Reset();

    void SetTeamColor(EDanceColorTeam eColor);
    EDanceColorTeam TeamColor() const;

    void SetRank(int nRank);
    int Rank() const;

    void AddTeamMark(int nMark);
    int TotalTeamMark() const;

    void AddTeamExp(int nExp);
    int TeamExp() const;

public:
    EDanceColorTeam m_eColor;
    int m_nTotalMark;
    int m_nRank;
    int m_nExp;
};

class DanceTeamRankCompare
{
public:
    DanceTeamRankCompare();
    ~DanceTeamRankCompare();
public:
    bool operator()(const CDanceTeam & lhs, const CDanceTeam & rhs) const;
};

class CRoomDanceScoreInfo
{
public:
    CRoomDanceScoreInfo();
    ~CRoomDanceScoreInfo();

    void Clear();
    void doEncode(CParamPool &IOBuff);

public:
    char m_szDancerPlace;
    char m_szDancerGrade;
    unsigned int m_szDancerMark;
    unsigned int m_szDancerExp;
    int m_szDancerPerfect;
    int m_szDancerCool;
    int m_szDancerGood;
    int m_szDancerBad;
    int m_szDancerMiss;
    int m_szDancerSpecial;
};


class CLiveRoomDancerScore
{
public:
    CLiveRoomDancerScore();
    ~CLiveRoomDancerScore();
    void Clear();
    void doEncode(CParamPool &IOBuff);

public:
    std::string m_strRoleName;
    CRoomDanceScoreInfo m_score;
    int m_nHotValue;
};

class CLiveRoomRoundDancerScore
{
public:
    CLiveRoomRoundDancerScore();
    ~CLiveRoomRoundDancerScore();

    void doEncode(CParamPool &IOBuff);

public:
    unsigned int m_nRoundEndTime;
    int m_nDancerCount;
    CLiveRoomDancerScore m_score[MAX_ROOM_PLAYER_COUNT];
};

class SweetheartsGroup
{
public:
    std::vector<unsigned int> m_vecRoleId;
    unsigned int m_nAccumulatedSweetValue;//掉线玩家的心动值累计
    unsigned int m_nAccumulatedMarkValue;//掉线玩家的总分累计
    unsigned int m_nSweetValue;//心动值
    unsigned int m_nMarkValue;//总分
    bool m_bSelectEachOther;//是否是互选
    int m_nRank;//组的名次

    SweetheartsGroup()
        : m_nAccumulatedSweetValue(0)
        , m_nAccumulatedMarkValue(0)
        , m_nSweetValue(0)
        , m_nMarkValue(0)
        , m_bSelectEachOther(false)
        , m_nRank(0)
    {
    }
};

typedef std::map<unsigned int, unsigned int> SelectingMap;
typedef std::map<unsigned char, SweetheartsGroup> SelectedMap;

struct RoundRange
{
    int nBegin;
    int nEnd;
    int nMode;
};

class AuditionPlayerInfo
{
public:
    AuditionPlayerInfo();
    ~AuditionPlayerInfo() { }

public:
    unsigned int m_nRoleID;
    std::string  m_strRoleName;
    unsigned int m_nAuditionScore;
};

/**
* 玩家在房间中的状态信息，目前主要用于存储玩家的Buff状态，用于处理宠物技能效果
*/
class PlayerRoomStatus
{
public:
    PlayerRoomStatus();
    ~PlayerRoomStatus();

    void Clear();

    void CostEndurance(bool bCost = true);
    std::map<SkillID, int> &SkillMap();

    bool HasCastPetSkill() const;

    int GetSkillDanceMarkRatio(int nMusicMode) const;
    int GetSkillRankMark(int nMusicMode, int nMusicLevel, int nRank);
    int GetSkillRewardExpRatio(int nMusicMode, int nMusicLevel);
    int GetSkillRewardMoney() const;
    int GetSkillRewardItemRatio() const;

    void SetBonusExp(int nBonusExp);
    int GetBonusExp() const;

protected:
    bool m_bCostEndurance;              // 是否扣除耐力
    std::map<SkillID, int> m_mapSkill;  // 宠物技能表
    int m_nBonusExp;                    // 奖励的经验，目前记录的是给宠物奖励的经验
};


//////////////////////////////////////////////////////////////////////////

class RoomPlayerInfo
{
public:
    RoomPlayerInfo();
    ~RoomPlayerInfo();

    void doEncode(CParamPool& IOBuff);
    void doDecode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    bool m_bIsHost;
    char m_chRoleType;
    char m_chRolePos;
    char m_chRoleState;
    EDanceColorTeam m_eColorTeam;
    char m_nRoleSex;
    char m_nRoleSkin;
    char m_nGroupTitle;
    std::string m_strDanceGroupName;
    unsigned short m_nDanceGroupBadge;
    unsigned short m_nDanceGroupEffect;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned char  m_nMoveType;
    unsigned short m_nTransFormID;

    // 其他信息
    //     CEntityItemComponent* m_pItemCOM;
    //     CEntityGeneComponent* m_pGeneCOM;
    //     PetComponent*         m_pPetCOM;
    //     CEntityMedalComponent   *m_pMedalCOM;
    //     CEntityHorseComponent   *m_pHorseCOM;
    //    RoleRoomImageData m_roleRoomImageData;
    std::string m_strData; // 存储二进制信息
};


class RoomBriefInfo
{
public:
    RoomBriefInfo();
    ~RoomBriefInfo();

    void doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoomID;
    std::string m_strRoomName;
    bool m_bHasPwd;
    bool m_bHasStart;
    char m_chMode;
    char m_chRoomColor;
    char m_chRoomCapacity;
    bool m_bTop;
    char m_szRoleSex[MAX_ROOM_PLAYER_COUNT];
};


class RoomWholeInfo
{
public:
    RoomWholeInfo();
    ~RoomWholeInfo();

    void doEncode(CParamPool& IOBuff);
    void doDecode(CParamPool& IOBuff);

public:
    unsigned int m_nRoomID;
    std::string m_strRoomName;
    char m_chRoomColor;
    std::string m_strRoomPwd;
    bool m_bHasPwd;
    bool m_bIsHost;
    char m_chScene;
    short m_nMusic;
    char m_chMode;
    char m_chLevel;
    EDanceRoomTeamMode m_eTeamMode;
    char m_szPosDancer[MAX_ROOM_PLAYER_COUNT];
    char m_szPosAudience[MAX_ROOM_AUDIENCE_COUNT];

    std::list<RoomPlayerInfo> m_lstDancer;
    std::list<RoomPlayerInfo> m_lstAudience;

};



class CDanceTeamMarkSend
{
public:
    CDanceTeamMarkSend();
    ~CDanceTeamMarkSend();
public:
    void doEncode(CParamPool & IOBuff);
public:
    EDanceColorTeam m_eColorTeam;
    int m_nTotalMark;
};



class InivteePlayerInfo
{
public:
    InivteePlayerInfo();
    ~InivteePlayerInfo();

    void doEncode(CParamPool& IOBuff);

public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    char m_chRoleLevel;
    char m_chRoleSex;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
};



class CRoleDanceRoomTeamInfo
{
public:
    CRoleDanceRoomTeamInfo();
    ~CRoleDanceRoomTeamInfo();
public:
    void doEncoede(CParamPool & IOBuff);
public:
    unsigned int m_nRoleID;
    EDanceRoomTeamMode m_eRoomTeamMode;
    EDanceColorTeam m_eRoomColorTeam;
};

class CWholeDanceRoomTeamInfo
{
public:
    CWholeDanceRoomTeamInfo();
    ~CWholeDanceRoomTeamInfo();
public:
    void doEncode(CParamPool & IOBuff);
public:
    EDanceRoomTeamMode m_eRoomTeamMode;
    std::list<CRoleDanceRoomTeamInfo> m_listRoleDanceRoomTeamInfo;
};


#endif //__ROOMDATASTRUCT_H__


