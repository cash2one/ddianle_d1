#ifndef __GAMEMSG_S2C_ROOM_MSG_H__
#define __GAMEMSG_S2C_ROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/Macro_Define.h"
// #include "../item/EntityItemComponent.h"
// #include "../gene/EntityGeneComponent.h"
// #include "../Pet/PetComponent.h"
#include "RoomDataStruct.h"
#include "../../datastructure/ImageData.h"
#include <list>

// class CEntityMedalComponent;
// class CEntityHorseComponent;



///////////////////////////////////////////////////////
class GameMsg_S2C_GetRoomListResult : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetRoomListResult);

public:
	GameMsg_S2C_GetRoomListResult(void);
	~GameMsg_S2C_GetRoomListResult(void);

	bool virtual doEncode(CParamPool& IOBuff);

public:
	unsigned short m_nCurPage;
	unsigned short m_nRoomTotalNum;

	std::vector<RoomBriefInfo> m_vecRoomInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateRoomSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CreateRoomSuc);
public:
	GameMsg_S2C_CreateRoomSuc(void);
	~GameMsg_S2C_CreateRoomSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	RoomWholeInfo m_RoomInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_CreateRoomFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CreateRoomFail);
public:
	GameMsg_S2C_CreateRoomFail(void);
	~GameMsg_S2C_CreateRoomFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_EnterRoomFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_EnterRoomFail);
public:
	GameMsg_S2C_EnterRoomFail(void);
	~GameMsg_S2C_EnterRoomFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_PrepareMatch : public GameMsg_Base
{
public:
	GameMsg_S2C_PrepareMatch(void);
	~GameMsg_S2C_PrepareMatch(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	RoomWholeInfo m_RoomInfo;

	char m_chPlayScene;
	short m_nPlayMusic;
	char m_chPlayMode;
	char m_chPlayLevel;
	std::string m_strCheckKey;
	std::vector<char> m_vecStage;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitRoomSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuitRoomSuc)
public:
	GameMsg_S2C_QuitRoomSuc(void);
	~GameMsg_S2C_QuitRoomSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chQuitType;
	char m_chQuitTo;		// 0 - lobby, 1 - world
	unsigned int m_nRoleIDKicked;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitRoomFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuitRoomFail)
public:
	GameMsg_S2C_QuitRoomFail(void);
	~GameMsg_S2C_QuitRoomFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_PrepareRoom : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_PrepareRoom)
public:
	GameMsg_S2C_PrepareRoom(void);
	~GameMsg_S2C_PrepareRoom(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chScene;
	short m_nMusic;
	char m_chMode;
	char m_chLevel;
	std::string m_strCheckKey;
	std::vector<char> m_vecStage;
	unsigned short m_nCountDownTime;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_StartRoomSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_StartRoomSuc);
public:
	GameMsg_S2C_StartRoomSuc(void);
	~GameMsg_S2C_StartRoomSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_S2C_StartRoomFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_StartRoomFail)
public:
	GameMsg_S2C_StartRoomFail(void);
	~GameMsg_S2C_StartRoomFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};

///////////////////////////////////////////////////////
struct SweetheartsInfo
{
	unsigned char m_nGroupID;
	unsigned int m_nSweetValue;
	unsigned int m_nMarkValue;

	SweetheartsInfo():m_nGroupID(0), m_nSweetValue(0), m_nMarkValue(0){}
};

//同步心动值
class GameMsg_S2C_UpdateSweetheartsInfo :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_UpdateSweetheartsInfo);
	GameMsg_S2C_UpdateSweetheartsInfo();
	~GameMsg_S2C_UpdateSweetheartsInfo();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	std::vector<SweetheartsInfo> m_vecSweetheartsInfo;
};
///////////////////////////////////////////////////////
class GameMsg_S2C_EndRoomSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_EndRoomSuc(void);
	~GameMsg_S2C_EndRoomSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_szDancerPlace[MAX_ROOM_PLAYER_COUNT];
	char m_szDancerGrade[MAX_ROOM_PLAYER_COUNT];
	unsigned int m_szDancerMark[MAX_ROOM_PLAYER_COUNT];
	unsigned int m_szDancerExp[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerPerfect[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerCool[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerGood[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerBad[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerMiss[MAX_ROOM_PLAYER_COUNT];
	int m_szDancerSpecial[MAX_ROOM_PLAYER_COUNT];
	std::list<CDanceTeamMarkSend> m_listTeamMark;
	std::vector<SweetheartsInfo> m_vecSweetheartsInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_KickPlayerFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_KickPlayerFail)
public:
	GameMsg_S2C_KickPlayerFail(void);
	~GameMsg_S2C_KickPlayerFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_EnterPlayer : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_EnterPlayer)
public:
	GameMsg_S2C_EnterPlayer(void);
	~GameMsg_S2C_EnterPlayer(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	RoomPlayerInfo m_PlayerInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitPlayer : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuitPlayer)
public:
	GameMsg_S2C_QuitPlayer(void);
	~GameMsg_S2C_QuitPlayer(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chQuitType;
    // quit role info.
	char m_chRoleRoomType;
	char m_chRoleRoomPos;
    // new host info.
	char m_chHostRoomType;
	char m_chHostRoomPos;
	char m_chHostRoomState;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_QuitMany : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_QuitMany)
public:
	GameMsg_S2C_QuitMany(void);
	~GameMsg_S2C_QuitMany(void);

	virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chQuitType;

	char m_chRoleRoomType;
	std::vector<char> m_vecRoleRoomPos;

	char m_chHostRoomType;
	char m_chHostRoomPos;
	char m_chHostRoomState;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_GetInviteeListSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetInviteeListSuc(void);
	~GameMsg_S2C_GetInviteeListSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::vector<InivteePlayerInfo> m_vecInviteeInfo;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_GetInviteeListFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GetInviteeListFail(void);
	~GameMsg_S2C_GetInviteeListFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_InvitePlayerSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_InvitePlayerSuc(void);
	~GameMsg_S2C_InvitePlayerSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_S2C_InviteeFail : public GameMsg_Base
{
public:
	GameMsg_S2C_InviteeFail(void);
	~GameMsg_S2C_InviteeFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_InviteeNotice : public GameMsg_Base
{
public:
	GameMsg_S2C_InviteeNotice(void);
	~GameMsg_S2C_InviteeNotice(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;
	char m_chMusicMode;

	unsigned int m_nInviterRoleID;
	std::string m_strInviterName;

	bool m_bIsVIP;
	unsigned short m_nVIPLevel;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_InvitePlayerFail : public GameMsg_Base
{
public:
	GameMsg_S2C_InvitePlayerFail(void);
	~GameMsg_S2C_InvitePlayerFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoomPosStateSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoomPosStateSuc)
public:
	GameMsg_S2C_ChangeRoomPosStateSuc(void);
	~GameMsg_S2C_ChangeRoomPosStateSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoleRoomType;
	char m_chRoleRoomPos;
	char m_chRoomPosState;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoomPosStateFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoomPosStateFail)
public:
	GameMsg_S2C_ChangeRoomPosStateFail(void);
	~GameMsg_S2C_ChangeRoomPosStateFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoomInfoSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoomInfoSuc(void);
	~GameMsg_S2C_ChangeRoomInfoSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
	bool m_bHasPwd;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoomInfoFail : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoomInfoFail(void);
	~GameMsg_S2C_ChangeRoomInfoFail(void);

	virtual bool doEncode(CParamPool& IOBuff);
	
public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeMusicInfoSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeMusicInfoSuc(void);
	~GameMsg_S2C_ChangeMusicInfoSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_chScene;
	short m_nMusic;
	char m_chMode;
	char m_chLevel;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeMusicInfoFail : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeMusicInfoFail(void);
	~GameMsg_S2C_ChangeMusicInfoFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoleRoomTypeSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoleRoomTypeSuc(void);
	~GameMsg_S2C_ChangeRoleRoomTypeSuc(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	char m_chSrcRoleRoomType;
	char m_chSrcRoleRoomPos;
	char m_chDesRoleRoomType;
	char m_chDesRoleRoomPos;
	char m_chRoleRoomState;
	EDanceColorTeam m_eColorTeam;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoleRoomTypeFail : public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoleRoomTypeFail(void);
	~GameMsg_S2C_ChangeRoleRoomTypeFail(void);

	virtual bool doEncode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoleRoomStateSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoleRoomStateSuc)
public:
	GameMsg_S2C_ChangeRoleRoomStateSuc(void);
	~GameMsg_S2C_ChangeRoleRoomStateSuc(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoleRoomType;
	char m_chRoleRoomPos;
	char m_chRoleRoomState;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_ChangeRoleRoomStateFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoleRoomStateFail)
public:
	GameMsg_S2C_ChangeRoleRoomStateFail(void);
	~GameMsg_S2C_ChangeRoleRoomStateFail(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strError;
};


///////////////////////////////////////////////////////
class GameMsg_S2C_SyncRoundMark : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncRoundMark);
public:
	GameMsg_S2C_SyncRoundMark(void);
	~GameMsg_S2C_SyncRoundMark(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chDancerPos;
	char m_chRound;				//current round
	char m_chRank;				//current round rank
	unsigned int m_nMark;		//mark till current round
	int m_nSpecial;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_RoomRequriePwd : public GameMsg_Base
{
public:
	GameMsg_S2C_RoomRequriePwd();
	~GameMsg_S2C_RoomRequriePwd();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nRoomID;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_PromoteRoomHostSuc : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_PromoteRoomHostSuc)
public:
	GameMsg_S2C_PromoteRoomHostSuc();
	~GameMsg_S2C_PromoteRoomHostSuc();
public:
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);
public:
	char m_chOldHostRoomType;
	char m_chOldHostRoomPos;
	char m_chOldHostState;

	char m_chNewHostRoomType;
	char m_chNewHostRoomPos;
	char m_chNewHostRoomState;

	std::string m_strRoomPwd;
};

///////////////////////////////////////////////////////
class GameMsg_S2C_PromoteRoomHostFail : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_PromoteRoomHostFail)
public:
	GameMsg_S2C_PromoteRoomHostFail();
	~GameMsg_S2C_PromoteRoomHostFail();
public:
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);
public:
	std::string m_strError;
};
////////////////////////////////////////////////////////
class GameMsg_S2C_SyncEffectState : public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncEffectState);
public:
	GameMsg_S2C_SyncEffectState();
	~GameMsg_S2C_SyncEffectState();
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);
public:
	int m_nSpecial;
	unsigned int m_nMark;
	char m_chRound;			//current round
	char m_nDancerPos;
};
////////////////////////////////////////////////////////
class GameMsg_S2C_LoadingStartRoomProgress:
	public GameMsg_Base
{
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_LoadingStartRoomProgress);
public:
	GameMsg_S2C_LoadingStartRoomProgress();
	~GameMsg_S2C_LoadingStartRoomProgress();
public:
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nRoleID;
	unsigned char m_nRate;
};
////////////////////////////////////////////////////////
class GameMsg_S2C_RoomReward:
	public GameMsg_Base
{
public:
	GameMsg_S2C_RoomReward();
	~GameMsg_S2C_RoomReward();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	std::list< CItem > m_ItemList;
};

////////////////////////////////////////////////////////
class GameMsg_S2C_SwitchTeamModeSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SwitchTeamModeSuc();
	~GameMsg_S2C_SwitchTeamModeSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
};

////////////////////////////////////////////////////////
class GameMsg_S2C_SwitchTeamModeFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_SwitchTeamModeFail();
	~GameMsg_S2C_SwitchTeamModeFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	ESwitchDanceRoomTeamModeRes m_eRes;
	std::string m_strError;
};

////////////////////////////////////////////////////////
class GameMsg_S2C_JoinDanceRoomTeamSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_JoinDanceRoomTeamSuc();
	~GameMsg_S2C_JoinDanceRoomTeamSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
};

////////////////////////////////////////////////////////
class GameMsg_S2C_JoinDanceRoomTeamFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_JoinDanceRoomTeamFail();
	~GameMsg_S2C_JoinDanceRoomTeamFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EJoinDanceRoomTeamRes m_eError;
	std::string m_strEroor;
};

////////////////////////////////////////////////////////
class GameMsg_S2C_UpdateRoleDanceRoomTeamInfo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateRoleDanceRoomTeamInfo();
	~GameMsg_S2C_UpdateRoleDanceRoomTeamInfo();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	CRoleDanceRoomTeamInfo m_SelfRoomTeamInfo;
};

class GameMsg_S2C_UpdateWholeDanceRoomTeamInfo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateWholeDanceRoomTeamInfo();
	~GameMsg_S2C_UpdateWholeDanceRoomTeamInfo();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	CWholeDanceRoomTeamInfo m_WholeRoomTeamInfo;
};

//开始选择搭档
class GameMsg_S2C_BeginToSelectPartner :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BeginToSelectPartner);
	GameMsg_S2C_BeginToSelectPartner();
	~GameMsg_S2C_BeginToSelectPartner();
public:
	virtual bool doEncode(CParamPool & IOBuff);
};

//结束选择搭档
class GameMsg_S2C_EndToSelectPartner :
	public GameMsg_Base
{
public:
	struct Sweethearts
	{
		unsigned char m_nGroupID;
		bool m_bSelectEachOther;
		std::vector<unsigned int> m_vecRoleID;
	};
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_EndToSelectPartner);
	GameMsg_S2C_EndToSelectPartner();
	~GameMsg_S2C_EndToSelectPartner();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	std::vector<Sweethearts> m_vecGroup;
};

//选择搭档
class GameMsg_S2C_SelectPartner :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SelectPartner);
	GameMsg_S2C_SelectPartner();
	~GameMsg_S2C_SelectPartner();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nSelectorRoleID;	//选择者id
	unsigned int m_nPartnerRoleID;	//选择的搭档id
};

//选择搭档失败
class GameMsg_S2C_SelectPartnerFail :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SelectPartnerFail);
	GameMsg_S2C_SelectPartnerFail();
	~GameMsg_S2C_SelectPartnerFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	std::string m_strError;
};

class GameMsg_S2C_ChangeRoomColor :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_ChangeRoomColor);
	GameMsg_S2C_ChangeRoomColor();
	~GameMsg_S2C_ChangeRoomColor();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_chColor;
};

#endif


// end of file


