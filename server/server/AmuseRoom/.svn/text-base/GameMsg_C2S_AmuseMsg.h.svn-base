#ifndef __GAMEMSG_C2S_AMUSEROOM_MSG_H__
#define __GAMEMSG_C2S_AMUSEROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "AmuseParam.h"


class CParamPool;
class CAmuseActionParam_Base;


///////////////////////////////////////////////////////
class GameMsg_C2S_GetAmuseRoomList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetAmuseRoomList);

	GameMsg_C2S_GetAmuseRoomList(void);
	~GameMsg_C2S_GetAmuseRoomList(void);

	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned short m_nPage;				//页数从0开始
};


///////////////////////////////////////////////////////
class GameMsg_C2S_CreateAmuseRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreateAmuseRoom);

	GameMsg_C2S_CreateAmuseRoom(void);
	~GameMsg_C2S_CreateAmuseRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
	char m_chRoomTheme;
	char m_chRoomScene;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_TryEnterAmuseRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_TryEnterAmuseRoom);

	GameMsg_C2S_TryEnterAmuseRoom(void);
	~GameMsg_C2S_TryEnterAmuseRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_EnterAmuseRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterAmuseRoom);

	GameMsg_C2S_EnterAmuseRoom(void);
	~GameMsg_C2S_EnterAmuseRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nRoomID;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_QuitAmuseRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitAmuseRoom);

	GameMsg_C2S_QuitAmuseRoom(void);
	~GameMsg_C2S_QuitAmuseRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
    char        m_nQuitTo;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ChangeAmuseRoomInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangeAmuseRoomInfo);

	GameMsg_C2S_ChangeAmuseRoomInfo(void);
	~GameMsg_C2S_ChangeAmuseRoomInfo(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::string m_strRoomName;
	std::string m_strRoomPwd;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_AmuseKickPlayer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AmuseKickPlayer);

	GameMsg_C2S_AmuseKickPlayer(void);
	~GameMsg_C2S_AmuseKickPlayer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nPlayerID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_AmusePromoteHost : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AmusePromoteHost);
	GameMsg_C2S_AmusePromoteHost();
	~GameMsg_C2S_AmusePromoteHost();

public:
	virtual bool doDecode(CParamPool & IOBuff);

public:
	unsigned int m_nNewHostID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_AmuseInvitePlayer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AmuseInvitePlayer);

	GameMsg_C2S_AmuseInvitePlayer(void);
	~GameMsg_C2S_AmuseInvitePlayer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	std::vector<int> m_vecInviteeID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_AmuseInviteeResponse : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AmuseInviteeResponse);

	GameMsg_C2S_AmuseInviteeResponse(void);
	~GameMsg_C2S_AmuseInviteeResponse(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bAccept;
	unsigned int m_nRoomID;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_DoAmuseAction : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_DoAmuseAction);

	GameMsg_C2S_DoAmuseAction(void);
	~GameMsg_C2S_DoAmuseAction(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	CAmuseActionParam_Base* m_pActionParam;		//heap memory, controlled by self
};
///////////////////////////////////////////////////////
class GameMsg_C2S_AmuseApplyMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_AmuseApplyMatch);

	GameMsg_C2S_AmuseApplyMatch(void);
	~GameMsg_C2S_AmuseApplyMatch(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bCreate;//没有空闲房间就自动创建房间，以下参数用于房间创建，类同GameMsg_C2S_CreateAmuseRoom中的字段
	std::string m_strRoomName;
	char m_chRoomTheme;
	char m_chRoomScene;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_PlayMakerSync : public GameMsg_Base
{
public:
	enum eSenceLocationType
	{
		eSenceLocationType_Unknown,
		eSenceLocationType_AmuseRoom,
		eSenceLocationType_WeddingRoom,
	};

	enum eReceiverType
	{
		eReceiverType_Unknown,
		eReceiverType_All,
		eReceiverType_Lover,
		eReceiverType_SpecificConsignee,
	};

public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PlayMakerSync);

	GameMsg_C2S_PlayMakerSync(void);
	~GameMsg_C2S_PlayMakerSync(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned char m_nLocationType;
	int m_nRoomID;
	unsigned char m_nReceiverType;
	unsigned int m_nSpecificID;

	PlayMakerInfo m_playerMakerInfo;
};


#endif
// end of file


