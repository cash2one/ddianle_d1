#include "GameMsg_C2S_RoomMsg.h"
#include "../../datastructure/Macro_Define.h"
#include "../share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_C2S_EnterLobby::GameMsg_C2S_EnterLobby(void)
:GameMsg_Base(MSG_C2S_EnterLobby)
{
}

GameMsg_C2S_EnterLobby::~GameMsg_C2S_EnterLobby(void)
{
}

bool GameMsg_C2S_EnterLobby::doDecode(CParamPool &IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ExitLobby::GameMsg_C2S_ExitLobby(void)
:GameMsg_Base(MSG_C2S_ExitLobby)
{
}

GameMsg_C2S_ExitLobby::~GameMsg_C2S_ExitLobby(void)
{
}

bool GameMsg_C2S_ExitLobby::doDecode(CParamPool &IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_GetRoomList::GameMsg_C2S_GetRoomList(void)
:GameMsg_Base(MSG_C2S_GetRoomList)
,m_bGetAll(false)
,m_chMode(MusicMode_None)
,m_nPage(0)
{
}

GameMsg_C2S_GetRoomList::~GameMsg_C2S_GetRoomList(void)
{
}

bool GameMsg_C2S_GetRoomList::doDecode(CParamPool &IOBuff)
{
	m_bGetAll = IOBuff.GetBool();
	m_chMode = IOBuff.GetUChar();
	m_nPage = IOBuff.GetUShort();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_CreateRoom::GameMsg_C2S_CreateRoom(void)
:GameMsg_Base(MSG_C2S_CreateRoom)
,m_strRoomName("")
,m_strRoomPwd("")
,m_chRoomType(0)
,m_chPhoneOS(OS_None)
{
}

GameMsg_C2S_CreateRoom::~GameMsg_C2S_CreateRoom(void)
{
}

bool GameMsg_C2S_CreateRoom::doDecode(CParamPool& IOBuff)
{
	IOBuff.GetStringW( m_strRoomName );
	IOBuff.GetStringW( m_strRoomPwd );
	m_chRoomType = IOBuff.GetUChar();
	m_chPhoneOS = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_TryEnterRoom::GameMsg_C2S_TryEnterRoom(void)
:GameMsg_Base(MSG_C2S_TryEnterRoom)
,m_bAudience(false)
,m_chPhoneOS(OS_None)
,m_nRoomID(0)
{
}

GameMsg_C2S_TryEnterRoom::~GameMsg_C2S_TryEnterRoom(void)
{
}

bool GameMsg_C2S_TryEnterRoom::doDecode(CParamPool& IOBuff)
{
	m_bAudience = IOBuff.GetBool();
	m_chPhoneOS = IOBuff.GetUChar();
	m_nRoomID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ApplyMatch::GameMsg_C2S_ApplyMatch(void)
:GameMsg_Base(MSG_C2S_ApplyMatch)
,m_chMode(MusicMode_None)
,m_chPhoneOS(OS_None)
{
}

GameMsg_C2S_ApplyMatch::~GameMsg_C2S_ApplyMatch(void)
{
}

bool GameMsg_C2S_ApplyMatch::doDecode(CParamPool& IOBuff)
{
	m_chMode = IOBuff.GetUChar();
	m_chPhoneOS = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_CancelMatch::GameMsg_C2S_CancelMatch(void)
:GameMsg_Base(MSG_C2S_CancelMatch)
{
}

GameMsg_C2S_CancelMatch::~GameMsg_C2S_CancelMatch(void)
{
}

bool GameMsg_C2S_CancelMatch::doDecode(CParamPool& IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_QuitRoom::GameMsg_C2S_QuitRoom(void)
:GameMsg_Base(MSG_C2S_QuitRoom)
,m_chQuitTo(RoomQuitTo_Lobby)
{
}

GameMsg_C2S_QuitRoom::~GameMsg_C2S_QuitRoom(void)
{
}

bool GameMsg_C2S_QuitRoom::doDecode(CParamPool& IOBuff)
{
	m_chQuitTo = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_StartRoom::GameMsg_C2S_StartRoom(void)
:GameMsg_Base(MSG_C2S_StartRoom)
,m_chRoomType(0)
{
}

GameMsg_C2S_StartRoom::~GameMsg_C2S_StartRoom(void)
{
}

bool GameMsg_C2S_StartRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoomType);
    return true;
}


bool GameMsg_C2S_StartRoom::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ReadyRoom::GameMsg_C2S_ReadyRoom(void)
:GameMsg_Base(MSG_C2S_ReadyRoom)
,m_chRoomType(0)
{
}

GameMsg_C2S_ReadyRoom::~GameMsg_C2S_ReadyRoom(void)
{
}

bool GameMsg_C2S_ReadyRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoomType);
    return true;
}

bool GameMsg_C2S_ReadyRoom::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_EndRoom::GameMsg_C2S_EndRoom(void)
:GameMsg_Base(MSG_C2S_EndRoom)
,m_chRoomType(0)
{
}

GameMsg_C2S_EndRoom::~GameMsg_C2S_EndRoom(void)
{
}

bool GameMsg_C2S_EndRoom::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_chRoomType);

    return true;
}

bool GameMsg_C2S_EndRoom::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_KickPlayer::GameMsg_C2S_KickPlayer(void)
:GameMsg_Base(MSG_C2S_KickPlayer)
,m_chRoleRoomType(RoleRoomType_None)
,m_chRoleRoomPos(-1)
,m_nRoleID(0)
{
}

GameMsg_C2S_KickPlayer::~GameMsg_C2S_KickPlayer(void)
{
}

bool GameMsg_C2S_KickPlayer::doDecode(CParamPool& IOBuff)
{
	m_chRoleRoomType = IOBuff.GetUChar();
	m_chRoleRoomPos = IOBuff.GetUChar();
	m_nRoleID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_GetInviteeList::GameMsg_C2S_GetInviteeList(void)
:GameMsg_Base(MSG_C2S_GetInviteeList)
{
}

GameMsg_C2S_GetInviteeList::~GameMsg_C2S_GetInviteeList(void)
{
}

bool GameMsg_C2S_GetInviteeList::doDecode(CParamPool& IOBuff)
{
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_InvitePlayer::GameMsg_C2S_InvitePlayer(void)
:GameMsg_Base(MSG_C2S_InvitePlayer)
,m_nInviteType(0)
{
}

GameMsg_C2S_InvitePlayer::~GameMsg_C2S_InvitePlayer(void)
{
}

bool GameMsg_C2S_InvitePlayer::doDecode(CParamPool& IOBuff)
{
	m_nInviteType = IOBuff.GetUChar();	

	unsigned short nCount = IOBuff.GetUShort();
	for ( int i = 0; i < nCount; ++i )
	{
		unsigned int nInviteID = IOBuff.GetUInt();
		m_vecInviteID.push_back( nInviteID );
	}

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_InviteeResponse::GameMsg_C2S_InviteeResponse(void)
:GameMsg_Base(MSG_C2S_InviteeResponse)
,m_bAccept(false)
,m_chPhoneOS(OS_None)
,m_nRoomID(0)
{
}

GameMsg_C2S_InviteeResponse::~GameMsg_C2S_InviteeResponse(void)
{
}

bool GameMsg_C2S_InviteeResponse::doDecode(CParamPool& IOBuff)
{
	m_bAccept = IOBuff.GetBool();
	m_chPhoneOS = IOBuff.GetUChar();
	m_nRoomID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_InEquip::GameMsg_C2S_InEquip(void)
:GameMsg_Base(MSG_C2S_InEquip)
{
	m_chRoomType = 0;
}

GameMsg_C2S_InEquip::~GameMsg_C2S_InEquip(void)
{
}

bool GameMsg_C2S_InEquip::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_OutEquip::GameMsg_C2S_OutEquip(void)
:GameMsg_Base(MSG_C2S_OutEquip)
{
	m_chRoomType = 0;
}

GameMsg_C2S_OutEquip::~GameMsg_C2S_OutEquip(void)
{
}

bool GameMsg_C2S_OutEquip::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeRoomPosState::GameMsg_C2S_ChangeRoomPosState(void)
:GameMsg_Base(MSG_C2S_ChangeRoomPosState)
,m_chRoleRoomType(RoleRoomType_None)
,m_chRoleRoomPos(-1)
,m_chRoomPosState(RoomPos_Open)
{
}

GameMsg_C2S_ChangeRoomPosState::~GameMsg_C2S_ChangeRoomPosState(void)
{
}

bool GameMsg_C2S_ChangeRoomPosState::doDecode(CParamPool& IOBuff)
{
	m_chRoleRoomType = IOBuff.GetUChar();
	m_chRoleRoomPos = IOBuff.GetUChar();
	m_chRoomPosState = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeRoomInfo::GameMsg_C2S_ChangeRoomInfo(void)
:GameMsg_Base(MSG_C2S_ChangeRoomInfo)
,m_strRoomName("")
,m_strRoomPwd("")
{
}

GameMsg_C2S_ChangeRoomInfo::~GameMsg_C2S_ChangeRoomInfo(void)
{
}

bool GameMsg_C2S_ChangeRoomInfo::doDecode(CParamPool& IOBuff)
{
	IOBuff.GetStringW( m_strRoomName );
	IOBuff.GetStringW( m_strRoomPwd );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeMusicInfo::GameMsg_C2S_ChangeMusicInfo(void)
:GameMsg_Base(MSG_C2S_ChangeMusicInfo)
,m_chRoomType(0)
,m_chScene(RAN_SCENE_ID)
,m_nMusic(RAN_MUSIC_ID)
,m_chMode(MusicMode_None)
,m_chLevel(MusicLevel_None)
{
}

GameMsg_C2S_ChangeMusicInfo::~GameMsg_C2S_ChangeMusicInfo(void)
{
}

bool GameMsg_C2S_ChangeMusicInfo::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	m_chScene = IOBuff.GetUChar();
	m_nMusic = IOBuff.GetShort();
	m_chMode = IOBuff.GetUChar();
	m_chLevel = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeRoleRoomType::GameMsg_C2S_ChangeRoleRoomType(void)
:GameMsg_Base(MSG_C2S_ChangeRoleRoomType)
,m_chRoleRoomType(RoleRoomType_None)
{
}

GameMsg_C2S_ChangeRoleRoomType::~GameMsg_C2S_ChangeRoleRoomType(void)
{
}

bool GameMsg_C2S_ChangeRoleRoomType::doDecode(CParamPool& IOBuff)
{
	m_chRoleRoomType = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeRoleRoomState::GameMsg_C2S_ChangeRoleRoomState(void)
:GameMsg_Base(MSG_C2S_ChangeRoleRoomState)
,m_chRoomType(0)
,m_chRoleRoomState(RoleRoomState_None)
{
}

GameMsg_C2S_ChangeRoleRoomState::~GameMsg_C2S_ChangeRoleRoomState(void)
{
}

bool GameMsg_C2S_ChangeRoleRoomState::doDecode(CParamPool& IOBuff)
{
	m_chRoomType = IOBuff.GetUChar();
	m_chRoleRoomState = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ReportRoundMark::GameMsg_C2S_ReportRoundMark(void)
    : GameMsg_Base(MSG_C2S_ReportRoundMark)
    , m_nRound(0)
    , m_chKeyRank(RoomRoundRank_None)
    , m_nMark(0)
{
}

GameMsg_C2S_ReportRoundMark::~GameMsg_C2S_ReportRoundMark(void)
{
}

bool GameMsg_C2S_ReportRoundMark::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUChar(m_nRound);
    IOBuff.AddUChar(m_chKeyRank);
    IOBuff.AddUInt(m_nMark);
    IOBuff.AddUShort((unsigned short)m_vecRoundRank.size());
    for (size_t i = 0; i < m_vecRoundRank.size(); ++i)
    {
        IOBuff.AddUChar(m_vecRoundRank[i]);
    }
    IOBuff.AddStringW(m_strMD5Code);

    return true;
}

bool GameMsg_C2S_ReportRoundMark::doDecode(CParamPool& IOBuff)
{
    m_nRound = IOBuff.GetUChar();
    m_chKeyRank = IOBuff.GetUChar();
    m_nMark = IOBuff.GetUInt();

    unsigned short nCount = IOBuff.GetUShort();
    for ( int i = 0; i < nCount; ++i )
    {
        char chRank = IOBuff.GetUChar();
        m_vecRoundRank.push_back( chRank );
    }

    IOBuff.GetStringW(m_strMD5Code);

    return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_QuitMarkAward::GameMsg_C2S_QuitMarkAward(void)
:GameMsg_Base(MSG_C2S_QuitMarkAward)
{
}

GameMsg_C2S_QuitMarkAward::~GameMsg_C2S_QuitMarkAward(void)
{
}

bool GameMsg_C2S_QuitMarkAward::doDecode(CParamPool& IOBuff)
{
	return true;
}


GameMsg_C2S_EnterRoom::GameMsg_C2S_EnterRoom() 
:GameMsg_Base(MSG_C2S_EnterRoom)
,m_nRoomID(0)
,m_bAudience(false)
,m_strPwd("")
{

}

GameMsg_C2S_EnterRoom::~GameMsg_C2S_EnterRoom()
{

}

bool GameMsg_C2S_EnterRoom::doDecode( CParamPool & IOBuff)
{
	m_nRoomID = IOBuff.GetUInt();
	m_bAudience = IOBuff.GetBool();
	IOBuff.GetStringW(m_strPwd);

	return true;	
}

GameMsg_C2S_PromoteRoomHost::GameMsg_C2S_PromoteRoomHost()
:GameMsg_Base(MSG_C2S_PromoteRoomHost)
,m_chNewHostRoleRoomType(0)
,m_chNewHostRoleRoomPos(0)
{

}

GameMsg_C2S_PromoteRoomHost::~GameMsg_C2S_PromoteRoomHost()
{

}

bool GameMsg_C2S_PromoteRoomHost::doDecode(CParamPool & IOBuff)
{
	m_chNewHostRoleRoomType = IOBuff.GetUChar();
	m_chNewHostRoleRoomPos = IOBuff.GetUChar();

	return true;
}

GameMsg_C2S_ReportEffectChange::GameMsg_C2S_ReportEffectChange()
:GameMsg_Base(MSG_C2S_ReportEffectChange)
{

}

GameMsg_C2S_ReportEffectChange::~GameMsg_C2S_ReportEffectChange()
{
}

bool GameMsg_C2S_ReportEffectChange::doEncode(CParamPool& IOBuff)
{
    IOBuff.AddUInt(m_nMark);
    IOBuff.AddInt(m_nSpecail);
    IOBuff.AddUChar(m_chRound);
    IOBuff.AddStringW(m_strMD5Code);

    return true;
}

bool GameMsg_C2S_ReportEffectChange::doDecode( CParamPool & IOBuff )
{
	m_nMark = IOBuff.GetUInt();
	m_nSpecail = IOBuff.GetInt();
	m_chRound = IOBuff.GetUChar();
	IOBuff.GetStringW( m_strMD5Code );

	return true;
}

GameMsg_C2S_LoadingStartRoomProgress::GameMsg_C2S_LoadingStartRoomProgress()
:GameMsg_Base(MSG_C2S_LoadingStartRoomProgress)
,m_nRate(0)
{

}

GameMsg_C2S_LoadingStartRoomProgress::~GameMsg_C2S_LoadingStartRoomProgress()
{

}

bool GameMsg_C2S_LoadingStartRoomProgress::doEncode(CParamPool & IOBuff)
{
    IOBuff.AddUChar(m_nRate);

    return true;
}


bool GameMsg_C2S_LoadingStartRoomProgress::doDecode( CParamPool & IOBuff )
{
	m_nRate = IOBuff.GetUChar();

	return true;
}

GameMsg_C2S_SwitchDanceRoomTeamMode::GameMsg_C2S_SwitchDanceRoomTeamMode()
:GameMsg_Base(MSG_C2S_SwitchDanceRoomTeamMode)
,m_nDanceRoomTeamMode(0)
,m_nDanceRoomColorTeam(0)
{
	
}

GameMsg_C2S_SwitchDanceRoomTeamMode::~GameMsg_C2S_SwitchDanceRoomTeamMode()
{

}

bool GameMsg_C2S_SwitchDanceRoomTeamMode::doDecode( CParamPool & IOBuff )
{
	m_nDanceRoomTeamMode = IOBuff.GetUChar();
	m_nDanceRoomColorTeam = IOBuff.GetUChar();

	return true;
}

GameMsg_C2S_JoinDanceRoomTeam::GameMsg_C2S_JoinDanceRoomTeam()
:GameMsg_Base(MSG_C2S_JoinDanceRoomTeam)
,m_nRoomColorTeam(0)
{

}

GameMsg_C2S_JoinDanceRoomTeam::~GameMsg_C2S_JoinDanceRoomTeam()
{

}

bool GameMsg_C2S_JoinDanceRoomTeam::doDecode( CParamPool & IOBuff )
{
	m_nRoomColorTeam = IOBuff.GetUChar();

	return true;
}

GameMsg_C2S_SelectPartner::GameMsg_C2S_SelectPartner()
:GameMsg_Base(MSG_C2S_SelectPartner)
{
	m_nPartnerRoleID = 0;
}

GameMsg_C2S_SelectPartner::~GameMsg_C2S_SelectPartner()
{

}

bool GameMsg_C2S_SelectPartner::doDecode( CParamPool & IOBuff )
{
	m_nPartnerRoleID = IOBuff.GetUInt();
	return true;
}

