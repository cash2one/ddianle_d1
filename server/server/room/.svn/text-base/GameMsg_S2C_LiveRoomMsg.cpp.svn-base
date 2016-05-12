#include "GameMsg_S2C_LiveRoomMsg.h"
#include "../../datastructure/Macro_Define.h"
#include "../share/ServerMsgDef.h"

GameMsg_S2C_EnterLiveLobbyRes::GameMsg_S2C_EnterLiveLobbyRes()
: GameMsg_Base(MSG_S2C_EnterLiveLobbyRes)
, m_bCanCreateLiveRoom(false)
{

}

GameMsg_S2C_EnterLiveLobbyRes::~GameMsg_S2C_EnterLiveLobbyRes()
{

}

bool GameMsg_S2C_EnterLiveLobbyRes::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddBool(m_bCanCreateLiveRoom);
	IOBuff.AddStringW(m_strTitle.c_str());
	IOBuff.AddStringW(m_strRule.c_str());

	IOBuff.AddUShort((unsigned short)m_listMaleDefaultCloth.size());
	for (std::list<itemtype_t>::iterator it = m_listMaleDefaultCloth.begin();
		it != m_listMaleDefaultCloth.end(); ++it)
	{
		IOBuff.AddUInt(*it);
	}

	IOBuff.AddUShort((unsigned short)m_listFemaleDefaultCloth.size());
	for (std::list<itemtype_t>::iterator it = m_listFemaleDefaultCloth.begin();
		it != m_listFemaleDefaultCloth.end(); ++it)
	{
		IOBuff.AddUInt(*it);
	}

	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_GetLiveRoomListResult::GameMsg_S2C_GetLiveRoomListResult(void)
: GameMsg_Base(MSG_S2C_GetLiveRoomListRes)
{
	m_nCurPage = 0;
	m_nRoomTotalNum = 0;
}
GameMsg_S2C_GetLiveRoomListResult::~GameMsg_S2C_GetLiveRoomListResult(void)
{	
}

bool GameMsg_S2C_GetLiveRoomListResult::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort(m_nCurPage);
	IOBuff.AddUShort(m_nRoomTotalNum);
	IOBuff.AddUShort((unsigned short)m_vecRoomInfo.size());
	for (size_t i = 0; i < m_vecRoomInfo.size(); i++)
	{
		m_vecRoomInfo[i].doEncode(IOBuff);
	}
	return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_CreateLiveRoomSuc::GameMsg_S2C_CreateLiveRoomSuc(void)
: GameMsg_Base(MSG_S2C_CreateLiveRoomSuc)
{
}
GameMsg_S2C_CreateLiveRoomSuc::~GameMsg_S2C_CreateLiveRoomSuc(void)
{

}

bool GameMsg_S2C_CreateLiveRoomSuc::doEncode(CParamPool& IOBuff)
{
	m_RoomInfo.doEncode(IOBuff);
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CreateLiveRoomFail::GameMsg_S2C_CreateLiveRoomFail(void)
: GameMsg_Base(MSG_S2C_CreateLiveRoomFail)
{
}
GameMsg_S2C_CreateLiveRoomFail::~GameMsg_S2C_CreateLiveRoomFail(void)
{
}

bool GameMsg_S2C_CreateLiveRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_EnterLiveRoomFail::GameMsg_S2C_EnterLiveRoomFail(void)
: GameMsg_Base(MSG_S2C_EnterLiveRoomFail)
{
}
GameMsg_S2C_EnterLiveRoomFail::~GameMsg_S2C_EnterLiveRoomFail(void)
{
}

bool GameMsg_S2C_EnterLiveRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}



///////////////////////////////////////////////////////
GameMsg_S2C_LiveRoomEnterPlayer::GameMsg_S2C_LiveRoomEnterPlayer()
: GameMsg_Base(MSG_S2C_LiveRoomEnterPlayer)
{
}
GameMsg_S2C_LiveRoomEnterPlayer::~GameMsg_S2C_LiveRoomEnterPlayer()
{
}

bool GameMsg_S2C_LiveRoomEnterPlayer::doEncode(CParamPool& IOBuff)
{
	m_PlayerInfo.doEncode(IOBuff);
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_LiveRoomEnterAudience::GameMsg_S2C_LiveRoomEnterAudience(void)
: GameMsg_Base(MSG_S2C_LiveRoomEnterAudience)
{
}
GameMsg_S2C_LiveRoomEnterAudience::~GameMsg_S2C_LiveRoomEnterAudience(void)
{
}

bool GameMsg_S2C_LiveRoomEnterAudience::doEncode(CParamPool& IOBuff)
{
	m_AudienceInfo.doEncode(IOBuff);
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitLiveRoomSuc::GameMsg_S2C_QuitLiveRoomSuc(void)
: GameMsg_Base(MSG_S2C_QuitLiveRoomSuc)
{
	m_chRoomRoleType = 0;
	m_chRoomRolePos = 0;
	m_nRoleID = 0;
	m_chQuitType = 0;
	m_chQuitTo = 0;
	m_nRoleIDKicked = 0;
}

GameMsg_S2C_QuitLiveRoomSuc::~GameMsg_S2C_QuitLiveRoomSuc(void)
{

}

bool GameMsg_S2C_QuitLiveRoomSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar(m_chRoomRoleType);
	IOBuff.AddUChar(m_chRoomRolePos);
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUChar(m_chQuitType);
	IOBuff.AddUChar(m_chQuitTo);
	IOBuff.AddUInt(m_nRoleIDKicked);
	return true;
}



///////////////////////////////////////////////////////
GameMsg_S2C_QuitLiveRoomFail::GameMsg_S2C_QuitLiveRoomFail(void)
: GameMsg_Base(MSG_S2C_QuitLiveRoomFail)
{
}
GameMsg_S2C_QuitLiveRoomFail::~GameMsg_S2C_QuitLiveRoomFail(void)
{
}

bool GameMsg_S2C_QuitLiveRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CloseLiveRoomSuc::GameMsg_S2C_CloseLiveRoomSuc(void)
: GameMsg_Base(MSG_S2C_CloseLiveRoomSuc)
{
}
GameMsg_S2C_CloseLiveRoomSuc::~GameMsg_S2C_CloseLiveRoomSuc(void)
{
}

bool GameMsg_S2C_CloseLiveRoomSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddInt(m_nCloseCountDownTime);
	return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_CloseLiveRoomFail::GameMsg_S2C_CloseLiveRoomFail(void)
: GameMsg_Base(MSG_S2C_CloseLiveRoomFail)
{
}
GameMsg_S2C_CloseLiveRoomFail::~GameMsg_S2C_CloseLiveRoomFail(void)
{
}

bool GameMsg_S2C_CloseLiveRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SetLiveRoomDefaultClothRes::GameMsg_S2C_SetLiveRoomDefaultClothRes()
: GameMsg_Base(MSG_S2C_SetLiveRoomDefaultClothRes)
{
	m_bSet = false;
}

GameMsg_S2C_SetLiveRoomDefaultClothRes::~GameMsg_S2C_SetLiveRoomDefaultClothRes()
{
}

bool GameMsg_S2C_SetLiveRoomDefaultClothRes::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddBool(m_bSet);
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_GetLiveRoomScoreRes::GameMsg_S2C_GetLiveRoomScoreRes()
: GameMsg_Base(MSG_S2C_GetLiveRoomScoreRes)
{
	m_nCurrPage = 0;
	m_nTotalPage = 0;
}

GameMsg_S2C_GetLiveRoomScoreRes::~GameMsg_S2C_GetLiveRoomScoreRes()
{
}

bool GameMsg_S2C_GetLiveRoomScoreRes::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort(m_nCurrPage);
	IOBuff.AddUShort(m_nTotalPage);
	m_score.doEncode(IOBuff);
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_UpdateLiveRoomDancerSuc::GameMsg_S2C_UpdateLiveRoomDancerSuc(void)
: GameMsg_Base(MSG_S2C_UpdateLiveRoomDancerSuc)
{
}
GameMsg_S2C_UpdateLiveRoomDancerSuc::~GameMsg_S2C_UpdateLiveRoomDancerSuc(void)
{
}

bool GameMsg_S2C_UpdateLiveRoomDancerSuc::doEncode(CParamPool& IOBuff)
{
	m_info.doEncode(IOBuff);
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_UpdateLiveRoomDancerFail::GameMsg_S2C_UpdateLiveRoomDancerFail(void)
: GameMsg_Base(MSG_S2C_UpdateLiveRoomDancerFail)
{
}
GameMsg_S2C_UpdateLiveRoomDancerFail::~GameMsg_S2C_UpdateLiveRoomDancerFail(void)
{
}

bool GameMsg_S2C_UpdateLiveRoomDancerFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_SetLiveRoomDancerToAudienceFail::GameMsg_S2C_SetLiveRoomDancerToAudienceFail(void)
: GameMsg_Base(MSG_S2C_SetLiveRoomDancerToAudienceFail)
{
}
GameMsg_S2C_SetLiveRoomDancerToAudienceFail::~GameMsg_S2C_SetLiveRoomDancerToAudienceFail(void)
{
}

bool GameMsg_S2C_SetLiveRoomDancerToAudienceFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_OpenVoteSuc::GameMsg_S2C_OpenVoteSuc(void)
: GameMsg_Base(MSG_S2C_OpenVoteSuc)
{
	m_bOpen = false;
}
GameMsg_S2C_OpenVoteSuc::~GameMsg_S2C_OpenVoteSuc(void)
{
}

bool GameMsg_S2C_OpenVoteSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddBool(m_bOpen);
	return true;
}

GameMsg_S2C_OpenVoteFail::GameMsg_S2C_OpenVoteFail(void)
: GameMsg_Base(MSG_S2C_OpenVoteFail)
{
}
GameMsg_S2C_OpenVoteFail::~GameMsg_S2C_OpenVoteFail(void)
{
}

bool GameMsg_S2C_OpenVoteFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_VoteSuc::GameMsg_S2C_VoteSuc(void)
: GameMsg_Base(MSG_S2C_VoteSuc)
{
}
GameMsg_S2C_VoteSuc::~GameMsg_S2C_VoteSuc(void)
{
}

bool GameMsg_S2C_VoteSuc::doEncode(CParamPool& IOBuff)
{
	return true;
}

GameMsg_S2C_VoteFail::GameMsg_S2C_VoteFail(void)
: GameMsg_Base(MSG_S2C_VoteFail)
{
}
GameMsg_S2C_VoteFail::~GameMsg_S2C_VoteFail(void)
{
}

bool GameMsg_S2C_VoteFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_DancerVoteInfo::GameMsg_S2C_DancerVoteInfo(void)
: GameMsg_Base(MSG_S2C_DancerVoteInfo)
{
}
GameMsg_S2C_DancerVoteInfo::~GameMsg_S2C_DancerVoteInfo(void)
{
}

bool GameMsg_S2C_DancerVoteInfo::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort((unsigned short)m_hotList.size());
	for (std::list<LiveRoomDancerVoteInfo>::iterator i = m_hotList.begin(); i != m_hotList.end(); ++i)
	{
		i->doEncode(IOBuff);
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_LiveRoomJoinAuditionFail::GameMsg_S2C_LiveRoomJoinAuditionFail(void)
: GameMsg_Base(MSG_S2C_LiveRoomJoinAuditionFail)
,m_nLeftPunishedTime(0)
{
}
GameMsg_S2C_LiveRoomJoinAuditionFail::~GameMsg_S2C_LiveRoomJoinAuditionFail(void)
{
}

bool GameMsg_S2C_LiveRoomJoinAuditionFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW(m_strError.c_str());
    IOBuff.AddUInt(m_nLeftPunishedTime);
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_LiveRoomPrepareAudition::GameMsg_S2C_LiveRoomPrepareAudition(void)
: GameMsg_Base(MSG_S2C_LiveRoomPrepareAudition)
{
	m_chScenceID = 0;
	m_nMusicID = 0;
	m_chMode = 0;
	m_chLevel = 0;
}
GameMsg_S2C_LiveRoomPrepareAudition::~GameMsg_S2C_LiveRoomPrepareAudition(void)
{
}

bool GameMsg_S2C_LiveRoomPrepareAudition::doEncode(CParamPool& IOBuff)
{
	m_roomInfo.doEncode(IOBuff);

	IOBuff.AddUChar((unsigned char)m_chScenceID);
	IOBuff.AddShort(m_nMusicID);
	IOBuff.AddUChar((unsigned char)m_chMode);
	IOBuff.AddUChar((unsigned char)m_chLevel);
	IOBuff.AddStringW(m_strCheckKey.c_str());
	IOBuff.AddPerFixSizeMemory(&m_vecStage[0], (unsigned short)m_vecStage.size());

	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_S2C_SyncMyAuditionScore::GameMsg_S2C_SyncMyAuditionScore(void)
: GameMsg_Base(MSG_S2C_SyncMyAuditionScore)
{
	m_nTotalScore = 0;
	m_nScore = 0;
}
GameMsg_S2C_SyncMyAuditionScore::~GameMsg_S2C_SyncMyAuditionScore(void)
{
}

bool GameMsg_S2C_SyncMyAuditionScore::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddInt(m_nTotalScore);
	IOBuff.AddInt(m_nScore);

	return true;
}

