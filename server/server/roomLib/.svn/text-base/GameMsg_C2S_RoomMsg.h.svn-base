#ifndef __GAMEMSG_C2S_ROOM_MSG_H__
#define __GAMEMSG_C2S_ROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"


///////////////////////////////////////////////////////
class GameMsg_C2S_EnterLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterLobby);

	GameMsg_C2S_EnterLobby(void);
	~GameMsg_C2S_EnterLobby(void);

	virtual bool doDecode( CParamPool &IOBuff );
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ExitLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExitLobby);

	GameMsg_C2S_ExitLobby(void);
	~GameMsg_C2S_ExitLobby(void);

	virtual bool doDecode( CParamPool &IOBuff );
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetRoomList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetRoomList);

	GameMsg_C2S_GetRoomList(void);
	~GameMsg_C2S_GetRoomList(void);

	virtual bool doDecode( CParamPool &IOBuff );

public:
	bool m_bGetAll;						//全部房间、等待房间
	char m_chMode;
	unsigned short m_nPage;				//页数从0开始
};


///////////////////////////////////////////////////////
class GameMsg_C2S_CreateRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreateRoom);

	GameMsg_C2S_CreateRoom(void);
	~GameMsg_C2S_CreateRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
	char m_chRoomType;
	char m_chPhoneOS;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_TryEnterRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_TryEnterRoom);

	GameMsg_C2S_TryEnterRoom(void);
	~GameMsg_C2S_TryEnterRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bAudience;
	char m_chPhoneOS;
	unsigned int m_nRoomID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ApplyMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ApplyMatch);

	GameMsg_C2S_ApplyMatch(void);
	~GameMsg_C2S_ApplyMatch(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chMode;
	char m_chPhoneOS;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_CancelMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CancelMatch);

	GameMsg_C2S_CancelMatch(void);
	~GameMsg_C2S_CancelMatch(void);

	virtual bool doDecode(CParamPool& IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_C2S_QuitRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitRoom);

	GameMsg_C2S_QuitRoom(void);
	~GameMsg_C2S_QuitRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chQuitTo;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_StartRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_StartRoom);

	GameMsg_C2S_StartRoom(void);
	~GameMsg_C2S_StartRoom(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);
public:
	char m_chRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ReadyRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReadyRoom);

	GameMsg_C2S_ReadyRoom(void);
	~GameMsg_C2S_ReadyRoom(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);
public:
	char m_chRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_EndRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EndRoom);

	GameMsg_C2S_EndRoom(void);
	~GameMsg_C2S_EndRoom(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);
public:
	char m_chRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_KickPlayer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickPlayer);

	GameMsg_C2S_KickPlayer(void);
	~GameMsg_C2S_KickPlayer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoleRoomType;
	char m_chRoleRoomPos;
	unsigned int m_nRoleID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetInviteeList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetInviteeList);

	GameMsg_C2S_GetInviteeList(void);
	~GameMsg_C2S_GetInviteeList(void);

	virtual bool doDecode(CParamPool& IOBuff);
};


///////////////////////////////////////////////////////
class GameMsg_C2S_InvitePlayer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InvitePlayer);

	GameMsg_C2S_InvitePlayer(void);
	~GameMsg_C2S_InvitePlayer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_nInviteType;
	std::vector< int > m_vecInviteID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_InviteeResponse : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InviteeResponse);

	GameMsg_C2S_InviteeResponse(void);
	~GameMsg_C2S_InviteeResponse(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bAccept;
	char m_chPhoneOS;
	unsigned int m_nRoomID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_InEquip : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InEquip);

	GameMsg_C2S_InEquip(void);
	~GameMsg_C2S_InEquip(void);

	virtual bool doDecode(CParamPool& IOBuff);
public:
	char m_chRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_OutEquip : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_OutEquip);

	GameMsg_C2S_OutEquip(void);
	~GameMsg_C2S_OutEquip(void);

	virtual bool doDecode(CParamPool& IOBuff);
public:
	char m_chRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeRoomPosState : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoomPosState);

	GameMsg_C2S_ChangeRoomPosState(void);
	~GameMsg_C2S_ChangeRoomPosState(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoleRoomType;
	char m_chRoleRoomPos;
	char m_chRoomPosState;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeRoomInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoomInfo);

	GameMsg_C2S_ChangeRoomInfo(void);
	~GameMsg_C2S_ChangeRoomInfo(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeMusicInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeMusicInfo);

	GameMsg_C2S_ChangeMusicInfo(void);
	~GameMsg_C2S_ChangeMusicInfo(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoomType;
	char m_chScene;
	short m_nMusic;
	char m_chMode;
	char m_chLevel;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeRoleRoomType : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoleRoomType);

	GameMsg_C2S_ChangeRoleRoomType(void);
	~GameMsg_C2S_ChangeRoleRoomType(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoleRoomType;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeRoleRoomState : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeRoleRoomState);

	GameMsg_C2S_ChangeRoleRoomState(void);
	~GameMsg_C2S_ChangeRoleRoomState(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chRoomType;
	char m_chRoleRoomState;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ReportRoundMark : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReportRoundMark);

    GameMsg_C2S_ReportRoundMark(void);
    ~GameMsg_C2S_ReportRoundMark(void);

public:
    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
    unsigned char m_nRound; //current round
    char m_chKeyRank;       //current round rank
    unsigned int m_nMark;   //current round mark
    std::vector<char> m_vecRoundRank;
    std::string m_strMD5Code;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_QuitMarkAward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitMarkAward);

	GameMsg_C2S_QuitMarkAward(void);
	~GameMsg_C2S_QuitMarkAward(void);

	virtual bool doDecode(CParamPool& IOBuff);
};

///////////////////////////////////////////////////////
class GameMsg_C2S_EnterRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterRoom);
	GameMsg_C2S_EnterRoom();
	~GameMsg_C2S_EnterRoom();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nRoomID;
	bool m_bAudience;
	std::string m_strPwd;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_PromoteRoomHost : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PromoteRoomHost);
	GameMsg_C2S_PromoteRoomHost();
	~GameMsg_C2S_PromoteRoomHost();

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	char m_chNewHostRoleRoomType;
	char m_chNewHostRoleRoomPos;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_ReportEffectChange : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ReportEffectChange);

	GameMsg_C2S_ReportEffectChange(void);
	~GameMsg_C2S_ReportEffectChange(void);

    virtual bool doEncode(CParamPool& IOBuff);
    virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nMark;	//current round mark
	int m_nSpecail;
	char m_chRound;			//current round
	std::string m_strMD5Code;
};

class GameMsg_C2S_LoadingStartRoomProgress :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_LoadingStartRoomProgress);
	GameMsg_C2S_LoadingStartRoomProgress();
	~GameMsg_C2S_LoadingStartRoomProgress();
public:
    virtual bool doEncode(CParamPool & IOBuff);
    virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nRate;
};

class GameMsg_C2S_SwitchDanceRoomTeamMode :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SwitchDanceRoomTeamMode);
	GameMsg_C2S_SwitchDanceRoomTeamMode();
	~GameMsg_C2S_SwitchDanceRoomTeamMode();
public:
	virtual bool doDecode(CParamPool & IOBuff);
	unsigned char m_nDanceRoomTeamMode;
	unsigned char m_nDanceRoomColorTeam;
};

class GameMsg_C2S_JoinDanceRoomTeam :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_JoinDanceRoomTeam);
	GameMsg_C2S_JoinDanceRoomTeam();
	~GameMsg_C2S_JoinDanceRoomTeam();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nRoomColorTeam;
};

//选择搭档
class GameMsg_C2S_SelectPartner :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SelectPartner);
	GameMsg_C2S_SelectPartner();
	~GameMsg_C2S_SelectPartner();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nPartnerRoleID;
};

#endif


// end of file


