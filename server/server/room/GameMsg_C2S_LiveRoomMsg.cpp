#include "GameMsg_C2S_LiveRoomMsg.h"
#include "../../datastructure/Macro_Define.h"
#include "../share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_C2S_EnterLiveLobby::GameMsg_C2S_EnterLiveLobby(void)
:GameMsg_Base(MSG_C2S_EnterLiveLobby)
{
}

GameMsg_C2S_EnterLiveLobby::~GameMsg_C2S_EnterLiveLobby(void)
{
}

bool GameMsg_C2S_EnterLiveLobby::doDecode(CParamPool &IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ExitLiveLobby::GameMsg_C2S_ExitLiveLobby(void)
:GameMsg_Base(MSG_C2S_ExitLiveLobby)
{
}

GameMsg_C2S_ExitLiveLobby::~GameMsg_C2S_ExitLiveLobby(void)
{
}

bool GameMsg_C2S_ExitLiveLobby::doDecode(CParamPool &IOBuff)
{
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_GetLiveRoomList::GameMsg_C2S_GetLiveRoomList(void)
:GameMsg_Base(MSG_C2S_GetLiveRoomList)
,m_nPage(0)
{
}

GameMsg_C2S_GetLiveRoomList::~GameMsg_C2S_GetLiveRoomList(void)
{
}

bool GameMsg_C2S_GetLiveRoomList::doDecode(CParamPool &IOBuff)
{
	m_nPage = IOBuff.GetUShort();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_CreateLiveRoom::GameMsg_C2S_CreateLiveRoom(void)
: GameMsg_Base(MSG_C2S_CreateLiveRoom)
, m_nRoomType(eRoomType_Unknown)
, m_chPhoneOS(OS_None)
{
	memset(m_szPlayerID, 0, sizeof(m_szPlayerID));
}

GameMsg_C2S_CreateLiveRoom::~GameMsg_C2S_CreateLiveRoom(void)
{
}

bool GameMsg_C2S_CreateLiveRoom::doDecode(CParamPool& IOBuff)
{
	m_nRoomType = IOBuff.GetUChar();
	for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
	{
		m_szPlayerID[i] = IOBuff.GetUInt();
	}
	IOBuff.GetStringW(m_strLiveRoomDesc);
	m_chPhoneOS = IOBuff.GetUChar();
	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_EnterLiveRoom::GameMsg_C2S_EnterLiveRoom(void)
: GameMsg_Base(MSG_C2S_EnterLiveRoom)
, m_nRoomID(0)
, m_chPhoneOS(OS_None)
{
}

GameMsg_C2S_EnterLiveRoom::~GameMsg_C2S_EnterLiveRoom(void)
{
}

bool GameMsg_C2S_EnterLiveRoom::doDecode(CParamPool &IOBuff)
{
	m_nRoomID = IOBuff.GetUInt();
	m_chPhoneOS = IOBuff.GetUChar();

	return true;
}

///////////////////////////////////////////////////////
GameMsg_C2S_QuitLiveRoom::GameMsg_C2S_QuitLiveRoom()
: GameMsg_Base(MSG_C2S_QuitLiveRoom)
{
	m_chQuitTo = 0;
}

GameMsg_C2S_QuitLiveRoom::~GameMsg_C2S_QuitLiveRoom()
{

}

bool GameMsg_C2S_QuitLiveRoom::doDecode(CParamPool& IOBuff)
{
	m_chQuitTo = IOBuff.GetUChar();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_CloseLiveRoom::GameMsg_C2S_CloseLiveRoom()
: GameMsg_Base(MSG_C2S_CloseLiveRoom)
{

}


GameMsg_C2S_CloseLiveRoom::~GameMsg_C2S_CloseLiveRoom()
{
}

bool GameMsg_C2S_CloseLiveRoom::doDecode(CParamPool& IOBuff)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_SetLiveRoomDefaultCloth::GameMsg_C2S_SetLiveRoomDefaultCloth()
: GameMsg_Base(MSG_C2S_SetLiveRoomDefaultCloth)
{
	m_bSet = false;
}


GameMsg_C2S_SetLiveRoomDefaultCloth::~GameMsg_C2S_SetLiveRoomDefaultCloth()
{
}

bool GameMsg_C2S_SetLiveRoomDefaultCloth::doDecode(CParamPool& IOBuff)
{
	m_bSet = IOBuff.GetBool();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_GetLiveRoomScore::GameMsg_C2S_GetLiveRoomScore()
: GameMsg_Base(MSG_C2S_GetLiveRoomScore)
{
	m_nPage = 0;
}


GameMsg_C2S_GetLiveRoomScore::~GameMsg_C2S_GetLiveRoomScore()
{
}

bool GameMsg_C2S_GetLiveRoomScore::doDecode(CParamPool& IOBuff)
{
	m_nPage = IOBuff.GetUShort();
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_UpdateLiveRoomDancer::GameMsg_C2S_UpdateLiveRoomDancer()
: GameMsg_Base(MSG_C2S_UpdateLiveRoomDancer)
{
	memset(m_szPlayerID, 0, sizeof(m_szPlayerID));
}

GameMsg_C2S_UpdateLiveRoomDancer::~GameMsg_C2S_UpdateLiveRoomDancer()
{
}

bool GameMsg_C2S_UpdateLiveRoomDancer::doDecode(CParamPool& IOBuff)
{
	for (int i = 0; i < MAX_ROOM_PLAYER_COUNT; i++)
	{
		m_szPlayerID[i] = IOBuff.GetUInt();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_SetLiveRoomDancerToAudience::GameMsg_C2S_SetLiveRoomDancerToAudience()
: GameMsg_Base(MSG_C2S_SetLiveRoomDancerToAudience)
{
	m_nRoleID = 0;
}

GameMsg_C2S_SetLiveRoomDancerToAudience::~GameMsg_C2S_SetLiveRoomDancerToAudience()
{
}

bool GameMsg_C2S_SetLiveRoomDancerToAudience::doDecode(CParamPool& IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_OpenVote::GameMsg_C2S_OpenVote()
: GameMsg_Base(MSG_C2S_OpenVote)
{
	m_bOpen = false;
}

GameMsg_C2S_OpenVote::~GameMsg_C2S_OpenVote()
{
}

bool GameMsg_C2S_OpenVote::doDecode(CParamPool& IOBuff)
{
	m_bOpen = IOBuff.GetBool();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_Vote::GameMsg_C2S_Vote()
: GameMsg_Base(MSG_C2S_Vote)
{
}

GameMsg_C2S_Vote::~GameMsg_C2S_Vote()
{
}

bool GameMsg_C2S_Vote::doDecode(CParamPool& IOBuff)
{
	m_nRoleID = IOBuff.GetUInt();
	m_nItemCount = IOBuff.GetUInt();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_LiveRoomJoinAudition::GameMsg_C2S_LiveRoomJoinAudition()
: GameMsg_Base(MSG_C2S_LiveRoomJoinAudition)
{
	m_chPhoneOS = OS_None;
}

GameMsg_C2S_LiveRoomJoinAudition::~GameMsg_C2S_LiveRoomJoinAudition()
{
}

bool GameMsg_C2S_LiveRoomJoinAudition::doDecode(CParamPool& IOBuff)
{
	m_chPhoneOS = IOBuff.GetUChar();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_LiveRoomCancelAudition::GameMsg_C2S_LiveRoomCancelAudition()
: GameMsg_Base(MSG_C2S_LiveRoomCancelAudition)
{
}

GameMsg_C2S_LiveRoomCancelAudition::~GameMsg_C2S_LiveRoomCancelAudition()
{
}

bool GameMsg_C2S_LiveRoomCancelAudition::doDecode(CParamPool& IOBuff)
{
	return true;
}

