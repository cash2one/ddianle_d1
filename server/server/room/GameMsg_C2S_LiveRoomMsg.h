#ifndef __GAMEMSG_C2S_LIVEROOM_MSG_H__
#define __GAMEMSG_C2S_LIVEROOM_MSG_H__

#include "../../socket/GameMsg_Base.h"
#include "../roomLib/RoomDataStruct.h"
#include "GameMsg_LiveRoom.h"

///////////////////////////////////////////////////////
class GameMsg_C2S_EnterLiveLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterLiveLobby);

	GameMsg_C2S_EnterLiveLobby(void);
	~GameMsg_C2S_EnterLiveLobby(void);

	virtual bool doDecode( CParamPool &IOBuff );
};


///////////////////////////////////////////////////////
class GameMsg_C2S_ExitLiveLobby : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExitLiveLobby);

	GameMsg_C2S_ExitLiveLobby(void);
	~GameMsg_C2S_ExitLiveLobby(void);

	virtual bool doDecode( CParamPool &IOBuff );
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetLiveRoomList : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLiveRoomList);

	GameMsg_C2S_GetLiveRoomList(void);
	~GameMsg_C2S_GetLiveRoomList(void);

	virtual bool doDecode( CParamPool &IOBuff );

public:
	unsigned short m_nPage;				//页数从0开始
};


///////////////////////////////////////////////////////
class GameMsg_C2S_CreateLiveRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreateLiveRoom);

	GameMsg_C2S_CreateLiveRoom(void);
	~GameMsg_C2S_CreateLiveRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned char m_nRoomType;
	unsigned int m_szPlayerID[MAX_ROOM_PLAYER_COUNT];// 玩家或者舞团ID
	std::string m_strLiveRoomDesc;
	char m_chPhoneOS;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_EnterLiveRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_EnterLiveRoom);

	GameMsg_C2S_EnterLiveRoom(void);
	~GameMsg_C2S_EnterLiveRoom(void);

	virtual bool doDecode( CParamPool &IOBuff );

public:
	unsigned int m_nRoomID;
	char m_chPhoneOS;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_QuitLiveRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitLiveRoom);

	GameMsg_C2S_QuitLiveRoom(void);
	~GameMsg_C2S_QuitLiveRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chQuitTo;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_CloseLiveRoom : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CloseLiveRoom);

	GameMsg_C2S_CloseLiveRoom(void);
	~GameMsg_C2S_CloseLiveRoom(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
};

///////////////////////////////////////////////////////
class GameMsg_C2S_SetLiveRoomDefaultCloth : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetLiveRoomDefaultCloth);

	GameMsg_C2S_SetLiveRoomDefaultCloth(void);
	~GameMsg_C2S_SetLiveRoomDefaultCloth(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bSet;
};


///////////////////////////////////////////////////////
class GameMsg_C2S_GetLiveRoomScore : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLiveRoomScore);

	GameMsg_C2S_GetLiveRoomScore(void);
	~GameMsg_C2S_GetLiveRoomScore(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned short m_nPage;
};

///////////////////////////////////////////////////////
class GameMsg_C2S_UpdateLiveRoomDancer : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_UpdateLiveRoomDancer);

	GameMsg_C2S_UpdateLiveRoomDancer(void);
	~GameMsg_C2S_UpdateLiveRoomDancer(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_szPlayerID[MAX_ROOM_PLAYER_COUNT];
};

///////////////////////////////////////////////////////
class GameMsg_C2S_SetLiveRoomDancerToAudience : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SetLiveRoomDancerToAudience);

	GameMsg_C2S_SetLiveRoomDancerToAudience(void);
	~GameMsg_C2S_SetLiveRoomDancerToAudience(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_OpenVote : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_OpenVote);

	GameMsg_C2S_OpenVote(void);
	~GameMsg_C2S_OpenVote(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	bool m_bOpen;
};


//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_Vote : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_Vote);

	GameMsg_C2S_Vote(void);
	~GameMsg_C2S_Vote(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	unsigned int m_nRoleID;
	unsigned int m_nItemCount;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_LiveRoomJoinAudition : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_LiveRoomJoinAudition);

	GameMsg_C2S_LiveRoomJoinAudition(void);
	~GameMsg_C2S_LiveRoomJoinAudition(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
	char m_chPhoneOS;
};

//////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_LiveRoomCancelAudition : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_LiveRoomCancelAudition);

	GameMsg_C2S_LiveRoomCancelAudition(void);
	~GameMsg_C2S_LiveRoomCancelAudition(void);

	virtual bool doDecode(CParamPool& IOBuff);

public:
};
#endif


// end of file


