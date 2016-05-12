#include "GameMsg_S2C_RoomMsg.h"
#include "../../datastructure/Macro_Define.h"
#include "../share/ServerMsgDef.h"
// #include "../Pet/PetService.h"
// #include "../Medal/EntityMedalComponent.h"
// #include "../Horse/EntityHorseComponent.h"


///////////////////////////////////////////////////////
GameMsg_S2C_GetRoomListResult::GameMsg_S2C_GetRoomListResult(void)
:GameMsg_Base(MSG_S2C_GetRoomListRes)
,m_nCurPage(0)
,m_nRoomTotalNum(0)
{
}

GameMsg_S2C_GetRoomListResult::~GameMsg_S2C_GetRoomListResult(void)
{
}

bool GameMsg_S2C_GetRoomListResult::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort( m_nCurPage );
	IOBuff.AddUShort( m_nRoomTotalNum );
	IOBuff.AddUShort( (unsigned short)m_vecRoomInfo.size() );

	for ( size_t i = 0; i < m_vecRoomInfo.size(); i++ )
	{
		m_vecRoomInfo[i].doEncode( IOBuff );
	}

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CreateRoomSuc::GameMsg_S2C_CreateRoomSuc(void)
:GameMsg_Base(MSG_S2C_CreateRoomSuc)
{
}

GameMsg_S2C_CreateRoomSuc::~GameMsg_S2C_CreateRoomSuc(void)
{
}

bool GameMsg_S2C_CreateRoomSuc::doEncode(CParamPool& IOBuff)
{
	m_RoomInfo.doEncode( IOBuff );

	return true;
}
bool GameMsg_S2C_CreateRoomSuc::doDecode(CParamPool& IOBuff)
{
    m_RoomInfo.doDecode(IOBuff);
    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_CreateRoomFail::GameMsg_S2C_CreateRoomFail(void)
:GameMsg_Base(MSG_S2C_CreateRoomFail)
,m_strError("")
{
}

GameMsg_S2C_CreateRoomFail::~GameMsg_S2C_CreateRoomFail(void)
{
}

bool GameMsg_S2C_CreateRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_CreateRoomFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);

    return true;
}



///////////////////////////////////////////////////////
GameMsg_S2C_EnterRoomFail::GameMsg_S2C_EnterRoomFail(void)
:GameMsg_Base(MSG_S2C_EnterRoomFail)
,m_strError("")
{
}

GameMsg_S2C_EnterRoomFail::~GameMsg_S2C_EnterRoomFail(void)
{
}

bool GameMsg_S2C_EnterRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_EnterRoomFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);

    return true;
}



///////////////////////////////////////////////////////
GameMsg_S2C_PrepareMatch::GameMsg_S2C_PrepareMatch(void)
:GameMsg_Base(MSG_S2C_PrepareMatch)
{
}

GameMsg_S2C_PrepareMatch::~GameMsg_S2C_PrepareMatch(void)
{
}

bool GameMsg_S2C_PrepareMatch::doEncode(CParamPool& IOBuff)
{
	m_RoomInfo.doEncode( IOBuff );
	
	IOBuff.AddUChar( m_chPlayScene );
	IOBuff.AddShort( m_nPlayMusic );
	IOBuff.AddUChar( m_chPlayMode );
	IOBuff.AddUChar( m_chPlayLevel );
	IOBuff.AddStringW( m_strCheckKey.c_str() );
	IOBuff.AddPerFixSizeMemory( &m_vecStage[0], (unsigned short)m_vecStage.size() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitRoomSuc::GameMsg_S2C_QuitRoomSuc(void)
:GameMsg_Base(MSG_S2C_QuitRoomSuc)
,m_chQuitType(RoomQuitType_Active)
,m_chQuitTo(RoomQuitTo_Lobby)
,m_nRoleIDKicked(0)
{
}

GameMsg_S2C_QuitRoomSuc::~GameMsg_S2C_QuitRoomSuc(void)
{
}

bool GameMsg_S2C_QuitRoomSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chQuitType );
	IOBuff.AddUChar( m_chQuitTo );
	IOBuff.AddUInt(m_nRoleIDKicked);

	return true;
}

bool GameMsg_S2C_QuitRoomSuc::doDecode(CParamPool& IOBuff)
{
    m_chQuitType = IOBuff.GetUChar();
    m_chQuitTo = IOBuff.GetUChar();
    m_nRoleIDKicked = IOBuff.GetUInt();

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitRoomFail::GameMsg_S2C_QuitRoomFail(void)
:GameMsg_Base(MSG_S2C_QuitRoomFail)
,m_strError("")
{
}

GameMsg_S2C_QuitRoomFail::~GameMsg_S2C_QuitRoomFail(void)
{
}

bool GameMsg_S2C_QuitRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_QuitRoomFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_PrepareRoom::GameMsg_S2C_PrepareRoom(void)
:GameMsg_Base(MSG_S2C_PrepareRoom)
,m_chScene(RAN_SCENE_ID)
,m_nMusic(RAN_MUSIC_ID)
,m_chMode(MusicMode_None)
,m_chLevel(MusicLevel_None)
,m_nCountDownTime(0)
{
}

GameMsg_S2C_PrepareRoom::~GameMsg_S2C_PrepareRoom(void)
{
}

bool GameMsg_S2C_PrepareRoom::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chScene );
	IOBuff.AddShort( m_nMusic );
	IOBuff.AddUChar( m_chMode );
	IOBuff.AddUChar( m_chLevel );
	IOBuff.AddStringW( m_strCheckKey.c_str() );
	IOBuff.AddPerFixSizeMemory( &m_vecStage[0], (unsigned short)m_vecStage.size() );
	IOBuff.AddUShort( m_nCountDownTime );

	return true;
}

bool GameMsg_S2C_PrepareRoom::doDecode(CParamPool& IOBuff)
{
    m_chScene = IOBuff.GetUChar();
    m_nMusic = IOBuff.GetShort();
    m_chMode = IOBuff.GetUChar();
    m_chLevel = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strCheckKey);
    std::string tempString;
    IOBuff.GetStringW(tempString);
    m_vecStage.assign(tempString.begin(), tempString.end());
    m_nCountDownTime = IOBuff.GetUShort();

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_StartRoomSuc::GameMsg_S2C_StartRoomSuc(void)
:GameMsg_Base(MSG_S2C_StartRoomSuc)
{
}

GameMsg_S2C_StartRoomSuc::~GameMsg_S2C_StartRoomSuc(void)
{
}

bool GameMsg_S2C_StartRoomSuc::doEncode(CParamPool& IOBuff)
{
	return true;
}

bool GameMsg_S2C_StartRoomSuc::doDecode(CParamPool& IOBuff)
{
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_StartRoomFail::GameMsg_S2C_StartRoomFail(void)
:GameMsg_Base(MSG_S2C_StartRoomFail)
,m_strError("")
{
}

GameMsg_S2C_StartRoomFail::~GameMsg_S2C_StartRoomFail(void)
{
}

bool GameMsg_S2C_StartRoomFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_StartRoomFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EndRoomSuc::GameMsg_S2C_EndRoomSuc(void)
:GameMsg_Base(MSG_S2C_EndRoomSuc)
{
	for ( int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i )
	{
		m_szDancerPlace[i] = 0;
		m_szDancerGrade[i] = 0;
		m_szDancerMark[i] = 0;
		m_szDancerExp[i] = 0;
		m_szDancerPerfect[i] = 0;
		m_szDancerSpecial[i] = 0;
	}
}

GameMsg_S2C_EndRoomSuc::~GameMsg_S2C_EndRoomSuc(void)
{
}

bool GameMsg_S2C_EndRoomSuc::doEncode(CParamPool& IOBuff)
{
	for ( int i = 0; i < MAX_ROOM_PLAYER_COUNT; ++i )
	{
		IOBuff.AddUChar( m_szDancerPlace[i] );
		IOBuff.AddUChar( m_szDancerGrade[i] );
		IOBuff.AddUInt( m_szDancerMark[i] );
		IOBuff.AddUInt( m_szDancerExp[i] );
		IOBuff.AddInt( m_szDancerPerfect[i] );
		IOBuff.AddInt( m_szDancerCool[i] );
		IOBuff.AddInt( m_szDancerGood[i] );
		IOBuff.AddInt( m_szDancerBad[i] );
		IOBuff.AddInt( m_szDancerMiss[i] );
		IOBuff.AddInt( m_szDancerSpecial[i] );
	}

	IOBuff.AddUShort( (unsigned short)m_listTeamMark.size() );
	for (std::list<CDanceTeamMarkSend>::iterator itr = m_listTeamMark.begin(); itr != m_listTeamMark.end(); ++itr ) {
		itr->doEncode(IOBuff);
	}
	IOBuff.AddUShort((unsigned short)m_vecSweetheartsInfo.size());
	for (std::vector<SweetheartsInfo>::iterator it = m_vecSweetheartsInfo.begin(); it != m_vecSweetheartsInfo.end(); ++it)
	{
		IOBuff.AddUChar(it->m_nGroupID);
		IOBuff.AddUInt(it->m_nSweetValue);
		IOBuff.AddUInt(it->m_nMarkValue);
	}
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_KickPlayerFail::GameMsg_S2C_KickPlayerFail(void)
:GameMsg_Base(MSG_S2C_KickPlayerFail)
,m_strError("")
{
}

GameMsg_S2C_KickPlayerFail::~GameMsg_S2C_KickPlayerFail(void)
{
}

bool GameMsg_S2C_KickPlayerFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_KickPlayerFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_EnterPlayer::GameMsg_S2C_EnterPlayer(void)
:GameMsg_Base(MSG_S2C_EnterPlayer)
{
}

GameMsg_S2C_EnterPlayer::~GameMsg_S2C_EnterPlayer(void)
{
}

bool GameMsg_S2C_EnterPlayer::doEncode(CParamPool& IOBuff)
{
	m_PlayerInfo.doEncode( IOBuff );

	return true;
}

bool GameMsg_S2C_EnterPlayer::doDecode(CParamPool& IOBuff)
{
    m_PlayerInfo.doDecode(IOBuff);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitPlayer::GameMsg_S2C_QuitPlayer(void)
:GameMsg_Base(MSG_S2C_QuitPlayer)
,m_chQuitType(RoomQuitType_Active)
,m_chRoleRoomType(RoleRoomType_None)
,m_chRoleRoomPos(-1)
,m_chHostRoomType(RoleRoomType_None)
,m_chHostRoomPos(-1)
,m_chHostRoomState(RoleRoomState_None)
{
}

GameMsg_S2C_QuitPlayer::~GameMsg_S2C_QuitPlayer(void)
{
}

bool GameMsg_S2C_QuitPlayer::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chQuitType );
	IOBuff.AddUChar( m_chRoleRoomType );
	IOBuff.AddUChar( m_chRoleRoomPos );
	IOBuff.AddUChar( m_chHostRoomType );
	IOBuff.AddUChar( m_chHostRoomPos );
	IOBuff.AddUChar( m_chHostRoomState );
	IOBuff.AddStringW( m_strRoomPwd.c_str() );

	return true;
}

bool GameMsg_S2C_QuitPlayer::doDecode(CParamPool& IOBuff)
{
    m_chQuitType = IOBuff.GetUChar();
    m_chRoleRoomType = IOBuff.GetUChar();
    m_chRoleRoomPos = IOBuff.GetUChar();
    m_chHostRoomType = IOBuff.GetUChar();
    m_chHostRoomPos = IOBuff.GetUChar();
    m_chHostRoomState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoomPwd);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_QuitMany::GameMsg_S2C_QuitMany(void)
:GameMsg_Base(MSG_S2C_QuitMany)
,m_chQuitType(RoomQuitType_Active)
,m_chRoleRoomType(RoleRoomType_None)
,m_chHostRoomType(RoleRoomType_None)
,m_chHostRoomPos(-1)
,m_chHostRoomState(RoleRoomState_None)
{
}

GameMsg_S2C_QuitMany::~GameMsg_S2C_QuitMany(void)
{
}

bool GameMsg_S2C_QuitMany::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chQuitType );
	IOBuff.AddUChar( m_chRoleRoomType );

	IOBuff.AddUShort( (unsigned short)m_vecRoleRoomPos.size() );
	for ( size_t i = 0; i < m_vecRoleRoomPos.size(); ++i )
	{
		IOBuff.AddUChar( m_vecRoleRoomPos[i] );
	}

	IOBuff.AddUChar( m_chHostRoomType );
	IOBuff.AddUChar( m_chHostRoomPos );
	IOBuff.AddUChar( m_chHostRoomState );
	IOBuff.AddStringW( m_strRoomPwd.c_str() );

	return true;
}

bool GameMsg_S2C_QuitMany::doDecode(CParamPool& IOBuff)
{
    m_chQuitType = IOBuff.GetUChar();
    m_chRoleRoomType = IOBuff.GetUChar();
    unsigned short rolePosSize = IOBuff.GetUShort();
    for (unsigned short i = 0; i < rolePosSize; ++i)
    {
        m_vecRoleRoomPos[i] = IOBuff.GetUChar();
    }

    m_chHostRoomType = IOBuff.GetUChar();
    m_chHostRoomPos = IOBuff.GetUChar();
    m_chHostRoomState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoomPwd);

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_GetInviteeListSuc::GameMsg_S2C_GetInviteeListSuc(void)
:GameMsg_Base(MSG_S2C_GetInviteeListSuc)
{
}

GameMsg_S2C_GetInviteeListSuc::~GameMsg_S2C_GetInviteeListSuc(void)
{
}

bool GameMsg_S2C_GetInviteeListSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUShort( (unsigned short)m_vecInviteeInfo.size() );
	for ( size_t i = 0; i < m_vecInviteeInfo.size(); i++ )
	{
		m_vecInviteeInfo[i].doEncode( IOBuff );
	}

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_GetInviteeListFail::GameMsg_S2C_GetInviteeListFail(void)
:GameMsg_Base(MSG_S2C_GetInviteeListFail)
,m_strError("")
{
}

GameMsg_S2C_GetInviteeListFail::~GameMsg_S2C_GetInviteeListFail(void)
{
}

bool GameMsg_S2C_GetInviteeListFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_InvitePlayerSuc::GameMsg_S2C_InvitePlayerSuc(void)
:GameMsg_Base(MSG_S2C_InvitePlayerSuc)
{
}

GameMsg_S2C_InvitePlayerSuc::~GameMsg_S2C_InvitePlayerSuc(void)
{
}

bool GameMsg_S2C_InvitePlayerSuc::doEncode(CParamPool& IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_InviteeFail::GameMsg_S2C_InviteeFail(void)
:GameMsg_Base(MSG_S2C_InviteeFail)
,m_strError("")
{
}

GameMsg_S2C_InviteeFail::~GameMsg_S2C_InviteeFail(void)
{
}

bool GameMsg_S2C_InviteeFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_InviteeNotice::GameMsg_S2C_InviteeNotice(void)
:GameMsg_Base(MSG_S2C_InviteeNotice)
,m_nRoomID(0)
,m_chMusicMode(MusicMode_None)
,m_nInviterRoleID(0)
,m_strInviterName("")
,m_bIsVIP(false)
,m_nVIPLevel(0)
{
}

GameMsg_S2C_InviteeNotice::~GameMsg_S2C_InviteeNotice(void)
{
}

bool GameMsg_S2C_InviteeNotice::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUInt( m_nRoomID );
	IOBuff.AddUChar( m_chMusicMode );
	IOBuff.AddUInt(m_nInviterRoleID);
	IOBuff.AddStringW( m_strInviterName.c_str() );
	IOBuff.AddBool(m_bIsVIP);
	IOBuff.AddUShort(m_nVIPLevel);

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_InvitePlayerFail::GameMsg_S2C_InvitePlayerFail(void)
:GameMsg_Base(MSG_S2C_InvitePlayerFail)
,m_strError("")
{
}

GameMsg_S2C_InvitePlayerFail::~GameMsg_S2C_InvitePlayerFail(void)
{
}

bool GameMsg_S2C_InvitePlayerFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoomPosStateSuc::GameMsg_S2C_ChangeRoomPosStateSuc(void)
:GameMsg_Base(MSG_S2C_ChangeRoomPosStateSuc)
,m_chRoleRoomType(RoleRoomType_None)
,m_chRoleRoomPos(-1)
,m_chRoomPosState(RoomPos_Open)
{
}

GameMsg_S2C_ChangeRoomPosStateSuc::~GameMsg_S2C_ChangeRoomPosStateSuc(void)
{
}

bool GameMsg_S2C_ChangeRoomPosStateSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chRoleRoomType );
	IOBuff.AddUChar( m_chRoleRoomPos );
	IOBuff.AddUChar( m_chRoomPosState );

	return true;
}

bool GameMsg_S2C_ChangeRoomPosStateSuc::doDecode(CParamPool& IOBuff)
{
    m_chRoleRoomType = IOBuff.GetUChar();
    m_chRoleRoomPos = IOBuff.GetUChar();
    m_chRoomPosState = IOBuff.GetUChar();

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoomPosStateFail::GameMsg_S2C_ChangeRoomPosStateFail(void)
:GameMsg_Base(MSG_S2C_ChangeRoomPosStateFail)
,m_strError("")
{
}

GameMsg_S2C_ChangeRoomPosStateFail::~GameMsg_S2C_ChangeRoomPosStateFail(void)
{
}

bool GameMsg_S2C_ChangeRoomPosStateFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_ChangeRoomPosStateFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoomInfoSuc::GameMsg_S2C_ChangeRoomInfoSuc(void)
:GameMsg_Base(MSG_S2C_ChangeRoomInfoSuc)
,m_strRoomName("")
,m_strRoomPwd("")
,m_bHasPwd(false)
{
}

GameMsg_S2C_ChangeRoomInfoSuc::~GameMsg_S2C_ChangeRoomInfoSuc(void)
{
}

bool GameMsg_S2C_ChangeRoomInfoSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strRoomName.c_str() );
	IOBuff.AddStringW( m_strRoomPwd.c_str() );
	IOBuff.AddBool( m_bHasPwd );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoomInfoFail::GameMsg_S2C_ChangeRoomInfoFail(void)
:GameMsg_Base(MSG_S2C_ChangeRoomInfoFail)
,m_strError("")
{
}

GameMsg_S2C_ChangeRoomInfoFail::~GameMsg_S2C_ChangeRoomInfoFail(void)
{
}

bool GameMsg_S2C_ChangeRoomInfoFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeMusicInfoSuc::GameMsg_S2C_ChangeMusicInfoSuc(void)
:GameMsg_Base(MSG_S2C_ChangeMusicInfoSuc)
,m_chScene(RAN_SCENE_ID)
,m_nMusic(RAN_MUSIC_ID)
,m_chMode(MusicMode_None)
,m_chLevel(MusicLevel_None)
{
}

GameMsg_S2C_ChangeMusicInfoSuc::~GameMsg_S2C_ChangeMusicInfoSuc(void)
{
}

bool GameMsg_S2C_ChangeMusicInfoSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chScene );
	IOBuff.AddShort( m_nMusic );
	IOBuff.AddUChar( m_chMode );
	IOBuff.AddUChar( m_chLevel );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeMusicInfoFail::GameMsg_S2C_ChangeMusicInfoFail(void)
:GameMsg_Base(MSG_S2C_ChangeMusicInfoFail)
,m_strError("")
{
}

GameMsg_S2C_ChangeMusicInfoFail::~GameMsg_S2C_ChangeMusicInfoFail(void)
{
}

bool GameMsg_S2C_ChangeMusicInfoFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoleRoomTypeSuc::GameMsg_S2C_ChangeRoleRoomTypeSuc(void)
:GameMsg_Base(MSG_S2C_ChangeRoleRoomTypeSuc)
,m_chSrcRoleRoomType(RoleRoomType_None)
,m_chSrcRoleRoomPos(-1)
,m_chDesRoleRoomType(RoleRoomType_None)
,m_chDesRoleRoomPos(-1)
,m_chRoleRoomState(RoleRoomState_None)
,m_eColorTeam(EDanceColorTeam_None)
{
}

GameMsg_S2C_ChangeRoleRoomTypeSuc::~GameMsg_S2C_ChangeRoleRoomTypeSuc(void)
{
}

bool GameMsg_S2C_ChangeRoleRoomTypeSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chSrcRoleRoomType );
	IOBuff.AddUChar( m_chSrcRoleRoomPos );
	IOBuff.AddUChar( m_chDesRoleRoomType );
	IOBuff.AddUChar( m_chDesRoleRoomPos );
	IOBuff.AddUChar( m_chRoleRoomState );
	IOBuff.AddUChar( (unsigned char)m_eColorTeam );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoleRoomTypeFail::GameMsg_S2C_ChangeRoleRoomTypeFail(void)
:GameMsg_Base(MSG_S2C_ChangeRoleRoomTypeFail)
,m_strError("")
{
}

GameMsg_S2C_ChangeRoleRoomTypeFail::~GameMsg_S2C_ChangeRoleRoomTypeFail(void)
{
}

bool GameMsg_S2C_ChangeRoleRoomTypeFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoleRoomStateSuc::GameMsg_S2C_ChangeRoleRoomStateSuc(void)
:GameMsg_Base(MSG_S2C_ChangeRoleRoomStateSuc)
,m_chRoleRoomType(RoleRoomType_None)
,m_chRoleRoomPos(-1)
,m_chRoleRoomState(RoleRoomState_None)
{
}

GameMsg_S2C_ChangeRoleRoomStateSuc::~GameMsg_S2C_ChangeRoleRoomStateSuc(void)
{
}

bool GameMsg_S2C_ChangeRoleRoomStateSuc::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chRoleRoomType );
	IOBuff.AddUChar( m_chRoleRoomPos );
	IOBuff.AddUChar( m_chRoleRoomState );

	return true;
}

bool GameMsg_S2C_ChangeRoleRoomStateSuc::doDecode(CParamPool& IOBuff)
{
    m_chRoleRoomType = IOBuff.GetUChar();
    m_chRoleRoomPos = IOBuff.GetUChar();
    m_chRoleRoomState = IOBuff.GetUChar();

    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_ChangeRoleRoomStateFail::GameMsg_S2C_ChangeRoleRoomStateFail(void)
:GameMsg_Base(MSG_S2C_ChangeRoleRoomStateFail)
,m_strError("")
{
}

GameMsg_S2C_ChangeRoleRoomStateFail::~GameMsg_S2C_ChangeRoleRoomStateFail(void)
{
}

bool GameMsg_S2C_ChangeRoleRoomStateFail::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddStringW( m_strError.c_str() );

	return true;
}

bool GameMsg_S2C_ChangeRoleRoomStateFail::doDecode(CParamPool& IOBuff)
{
    IOBuff.GetStringW(m_strError);
    return true;
}

///////////////////////////////////////////////////////
GameMsg_S2C_SyncRoundMark::GameMsg_S2C_SyncRoundMark(void)
:GameMsg_Base(MSG_S2C_SyncRoundMark)
,m_chDancerPos(-1)
,m_chRound(0)
,m_chRank(RoomRoundRank_None)
,m_nMark(0)
,m_nSpecial(0)
{
}

GameMsg_S2C_SyncRoundMark::~GameMsg_S2C_SyncRoundMark(void)
{
}

bool GameMsg_S2C_SyncRoundMark::doEncode(CParamPool& IOBuff)
{
	IOBuff.AddUChar( m_chDancerPos );
	IOBuff.AddUChar( m_chRound );
	IOBuff.AddUChar( m_chRank );
	IOBuff.AddUInt( m_nMark );
	IOBuff.AddInt( m_nSpecial );

	return true;
}

bool GameMsg_S2C_SyncRoundMark::doDecode(CParamPool& IOBuff)
{
    m_chDancerPos = IOBuff.GetUChar();
    m_chRound = IOBuff.GetUChar();
    m_chRank = IOBuff.GetUChar();
    m_nMark = IOBuff.GetUInt();
    m_nSpecial = IOBuff.GetInt();

    return true;
}


GameMsg_S2C_RoomRequriePwd::GameMsg_S2C_RoomRequriePwd()
:GameMsg_Base(MSG_S2C_RoomRequirePwd)
{

}

GameMsg_S2C_RoomRequriePwd::~GameMsg_S2C_RoomRequriePwd()
{

}

bool GameMsg_S2C_RoomRequriePwd::doEncode(CParamPool & IOBuff)
{
	IOBuff.AddUInt(m_nRoomID);

	return true;
}

GameMsg_S2C_PromoteRoomHostSuc::GameMsg_S2C_PromoteRoomHostSuc()
:GameMsg_Base(MSG_S2C_PromoteRoomHostSuc)
,m_chOldHostRoomType(0)
,m_chOldHostRoomPos(0)
,m_chOldHostState(0)
,m_chNewHostRoomType(0)
,m_chNewHostRoomPos(0)
,m_chNewHostRoomState(0)
,m_strRoomPwd("")
{

}

GameMsg_S2C_PromoteRoomHostSuc::~GameMsg_S2C_PromoteRoomHostSuc()
{

}

bool GameMsg_S2C_PromoteRoomHostSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar(m_chOldHostRoomType);
	IOBuff.AddUChar(m_chOldHostRoomPos);
	IOBuff.AddUChar(m_chOldHostState);
	IOBuff.AddUChar(m_chNewHostRoomType);
	IOBuff.AddUChar(m_chNewHostRoomPos);
	IOBuff.AddUChar(m_chNewHostRoomState);
	IOBuff.AddStringW(m_strRoomPwd.c_str());

	return true;
}

bool GameMsg_S2C_PromoteRoomHostSuc::doDecode(CParamPool & IOBuff)
{
    m_chOldHostRoomType = IOBuff.GetUChar();
    m_chOldHostRoomPos = IOBuff.GetUChar();
    m_chOldHostState = IOBuff.GetUChar();
    m_chNewHostRoomType = IOBuff.GetUChar();
    m_chNewHostRoomPos = IOBuff.GetUChar();
    m_chNewHostRoomState = IOBuff.GetUChar();
    IOBuff.GetStringW(m_strRoomPwd);

    return true;
}


GameMsg_S2C_PromoteRoomHostFail::GameMsg_S2C_PromoteRoomHostFail()
:GameMsg_Base(MSG_S2C_PromoteRoomHostFail)
,m_strError("")
{

}

GameMsg_S2C_PromoteRoomHostFail::~GameMsg_S2C_PromoteRoomHostFail()
{

}

bool GameMsg_S2C_PromoteRoomHostFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

bool GameMsg_S2C_PromoteRoomHostFail::doDecode(CParamPool & IOBuff)
{
    IOBuff.GetStringW(m_strError);
    return true;
}


GameMsg_S2C_SyncEffectState::GameMsg_S2C_SyncEffectState()
:GameMsg_Base(MSG_S2C_SyncEffectState)
{

}
GameMsg_S2C_SyncEffectState::~GameMsg_S2C_SyncEffectState()
{

}
bool GameMsg_S2C_SyncEffectState::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nMark);
	IOBuff.AddInt(m_nSpecial);
	IOBuff.AddUChar(m_chRound);
	IOBuff.AddUChar(m_nDancerPos);
	return true;
}

bool GameMsg_S2C_SyncEffectState::doDecode(CParamPool & IOBuff)
{
    m_nMark = IOBuff.GetUInt();
    m_nSpecial = IOBuff.GetInt();
    m_chRound = IOBuff.GetUChar();
    m_nDancerPos = IOBuff.GetUChar();

    return true;
}

GameMsg_S2C_LoadingStartRoomProgress::GameMsg_S2C_LoadingStartRoomProgress()
:GameMsg_Base(MSG_S2C_LoadingStartRoomProgress)
,m_nRoleID(0)
,m_nRate(0)
{

}

GameMsg_S2C_LoadingStartRoomProgress::~GameMsg_S2C_LoadingStartRoomProgress()
{

}

bool GameMsg_S2C_LoadingStartRoomProgress::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUChar(m_nRate);

	return true;
}

bool GameMsg_S2C_LoadingStartRoomProgress::doDecode(CParamPool & IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    m_nRate = IOBuff.GetUChar();

    return true;
}


GameMsg_S2C_RoomReward::GameMsg_S2C_RoomReward()
:GameMsg_Base(MSG_S2C_RoomReward)
{

}

GameMsg_S2C_RoomReward::~GameMsg_S2C_RoomReward()
{

}

bool GameMsg_S2C_RoomReward::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_ItemList.size());
	if (!m_ItemList.empty())
	{
		std::list< CItem >::iterator itr = m_ItemList.begin();
		for (; itr != m_ItemList.end(); ++itr)
		{
			(*itr).doEncode(IOBuff);
		}
	}

	return true;
}



GameMsg_S2C_SwitchTeamModeSuc::GameMsg_S2C_SwitchTeamModeSuc()
:GameMsg_Base(MSG_S2C_SwitchDanceRoomTeamModeSuc)
{

}

GameMsg_S2C_SwitchTeamModeSuc::~GameMsg_S2C_SwitchTeamModeSuc()
{

}

bool GameMsg_S2C_SwitchTeamModeSuc::doEncode( CParamPool & IOBuff )
{
	return true;
}


GameMsg_S2C_SwitchTeamModeFail::GameMsg_S2C_SwitchTeamModeFail()
:GameMsg_Base(MSG_S2C_SwitchDanceRoomTeamModeFail)
,m_eRes(ESwitchDanceRoomTeamModeRes_Suc)
,m_strError("")
{

}

GameMsg_S2C_SwitchTeamModeFail::~GameMsg_S2C_SwitchTeamModeFail()
{

}

bool GameMsg_S2C_SwitchTeamModeFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eRes);
	IOBuff.AddStringW(m_strError.c_str());

	return true;
}

GameMsg_S2C_JoinDanceRoomTeamFail::GameMsg_S2C_JoinDanceRoomTeamFail()
:GameMsg_Base(MSG_S2C_JoinDanceRoomTeamFail)
,m_eError(EJoinDanceRoomTeamRes_Suc)
{

}

GameMsg_S2C_JoinDanceRoomTeamFail::~GameMsg_S2C_JoinDanceRoomTeamFail()
{

}

bool GameMsg_S2C_JoinDanceRoomTeamFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);
	IOBuff.AddStringW(m_strEroor.c_str());
	
	return true;
}


GameMsg_S2C_UpdateRoleDanceRoomTeamInfo::GameMsg_S2C_UpdateRoleDanceRoomTeamInfo()
:GameMsg_Base(MSG_S2C_UpdateRoleDanceTeamInfo)
{

}

GameMsg_S2C_UpdateRoleDanceRoomTeamInfo::~GameMsg_S2C_UpdateRoleDanceRoomTeamInfo()
{

}

bool GameMsg_S2C_UpdateRoleDanceRoomTeamInfo::doEncode( CParamPool & IOBuff )
{
	m_SelfRoomTeamInfo.doEncoede(IOBuff);

	return true;
}



GameMsg_S2C_UpdateWholeDanceRoomTeamInfo::GameMsg_S2C_UpdateWholeDanceRoomTeamInfo()
:GameMsg_Base(MSG_S2C_UpdateWholeDanceRoomTeamInfo)
{

}

GameMsg_S2C_UpdateWholeDanceRoomTeamInfo::~GameMsg_S2C_UpdateWholeDanceRoomTeamInfo()
{

}

bool GameMsg_S2C_UpdateWholeDanceRoomTeamInfo::doEncode( CParamPool & IOBuff )
{
	m_WholeRoomTeamInfo.doEncode(IOBuff);

	return true;
}

GameMsg_S2C_BeginToSelectPartner::GameMsg_S2C_BeginToSelectPartner()
:GameMsg_Base(MSG_S2C_BeginToSelectPartner)
{
}

GameMsg_S2C_BeginToSelectPartner::~GameMsg_S2C_BeginToSelectPartner()
{

}

bool GameMsg_S2C_BeginToSelectPartner::doEncode( CParamPool & IOBuff )
{
	return true;
}

GameMsg_S2C_EndToSelectPartner::GameMsg_S2C_EndToSelectPartner()
:GameMsg_Base(MSG_S2C_EndToSelectPartner)
{
}

GameMsg_S2C_EndToSelectPartner::~GameMsg_S2C_EndToSelectPartner()
{

}

bool GameMsg_S2C_EndToSelectPartner::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort((unsigned short)(m_vecGroup.size()));
	for (std::vector<Sweethearts>::iterator it = m_vecGroup.begin(); it != m_vecGroup.end(); ++it)
	{
		IOBuff.AddUChar(it->m_nGroupID);
		IOBuff.AddBool(it->m_bSelectEachOther);
		IOBuff.AddUShort((unsigned short)(it->m_vecRoleID.size()));
		for (std::vector<unsigned int>::iterator iter = it->m_vecRoleID.begin(); iter != it->m_vecRoleID.end(); ++iter)
		{
			IOBuff.AddUInt(*iter);
		}
	}
	return true;
}

GameMsg_S2C_SelectPartner::GameMsg_S2C_SelectPartner()
:GameMsg_Base(MSG_S2C_SelectPartner)
{
	m_nSelectorRoleID = 0;
	m_nPartnerRoleID = 0;
}

GameMsg_S2C_SelectPartner::~GameMsg_S2C_SelectPartner()
{

}

bool GameMsg_S2C_SelectPartner::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nSelectorRoleID);
	IOBuff.AddUInt(m_nPartnerRoleID);
	return true;
}

GameMsg_S2C_UpdateSweetheartsInfo::GameMsg_S2C_UpdateSweetheartsInfo()
:GameMsg_Base(MSG_S2C_UpdateSweetValue)
{

}

GameMsg_S2C_UpdateSweetheartsInfo::~GameMsg_S2C_UpdateSweetheartsInfo()
{

}

bool GameMsg_S2C_UpdateSweetheartsInfo::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUShort((unsigned short)m_vecSweetheartsInfo.size());
	for (std::vector<SweetheartsInfo>::iterator it = m_vecSweetheartsInfo.begin(); it != m_vecSweetheartsInfo.end(); ++it)
	{
		IOBuff.AddUChar(it->m_nGroupID);
		IOBuff.AddUInt(it->m_nSweetValue);
		IOBuff.AddUInt(it->m_nMarkValue);
	}
	return true;
}

GameMsg_S2C_SelectPartnerFail::GameMsg_S2C_SelectPartnerFail()
:GameMsg_Base(MSG_S2C_SelectPartnerFail)
{

}

GameMsg_S2C_SelectPartnerFail::~GameMsg_S2C_SelectPartnerFail()
{

}

bool GameMsg_S2C_SelectPartnerFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddStringW(m_strError.c_str());
	return true;
}

GameMsg_S2C_ChangeRoomColor::GameMsg_S2C_ChangeRoomColor()
:GameMsg_Base(MSG_S2C_ChangeRoomColor)
{
	m_chColor = 0;
}

GameMsg_S2C_ChangeRoomColor::~GameMsg_S2C_ChangeRoomColor()
{

}

bool GameMsg_S2C_ChangeRoomColor::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar(m_chColor);
	return true;
}

