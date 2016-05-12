#include "GameMsg_C2S_AmuseMsg.h"
#include "AmuseParam.h"
#include "../../datastructure/Macro_Define.h"
#include "../share/ServerMsgDef.h"


///////////////////////////////////////////////////////
GameMsg_C2S_GetAmuseRoomList::GameMsg_C2S_GetAmuseRoomList(void)
:GameMsg_Base(MSG_C2S_GetAmuseRoomList)
,m_nPage(0)
{
}

GameMsg_C2S_GetAmuseRoomList::~GameMsg_C2S_GetAmuseRoomList(void)
{
}

bool GameMsg_C2S_GetAmuseRoomList::doDecode(CParamPool &IOBuff)
{
	m_nPage = IOBuff.GetUShort();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_CreateAmuseRoom::GameMsg_C2S_CreateAmuseRoom(void)
:GameMsg_Base(MSG_C2S_CreateAmuseRoom)
,m_strRoomName("")
,m_strRoomPwd("")
,m_chRoomTheme(eAmuseTheme_None)
,m_chRoomScene(0)
{
}

GameMsg_C2S_CreateAmuseRoom::~GameMsg_C2S_CreateAmuseRoom(void)
{
}

bool GameMsg_C2S_CreateAmuseRoom::doDecode(CParamPool& IOBuff)
{
	IOBuff.GetStringW( m_strRoomName );
	IOBuff.GetStringW( m_strRoomPwd );
	m_chRoomTheme = IOBuff.GetUChar();
	m_chRoomScene = IOBuff.GetUChar();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_TryEnterAmuseRoom::GameMsg_C2S_TryEnterAmuseRoom(void)
:GameMsg_Base(MSG_C2S_TryEnterAmuseRoom)
,m_nRoomID(0)
{
}

GameMsg_C2S_TryEnterAmuseRoom::~GameMsg_C2S_TryEnterAmuseRoom(void)
{
}

bool GameMsg_C2S_TryEnterAmuseRoom::doDecode(CParamPool& IOBuff)
{
	m_nRoomID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_EnterAmuseRoom::GameMsg_C2S_EnterAmuseRoom(void)
:GameMsg_Base(MSG_C2S_EnterAmuseRoom)
,m_nRoomID(0)
{
}

GameMsg_C2S_EnterAmuseRoom::~GameMsg_C2S_EnterAmuseRoom(void)
{
}

bool GameMsg_C2S_EnterAmuseRoom::doDecode(CParamPool& IOBuff)
{
	m_nRoomID = IOBuff.GetUInt();
	IOBuff.GetStringW( m_strRoomPwd );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_QuitAmuseRoom::GameMsg_C2S_QuitAmuseRoom(void)
:GameMsg_Base(MSG_C2S_QuitAmuseRoom)
,m_nQuitTo(0)
{
}

GameMsg_C2S_QuitAmuseRoom::~GameMsg_C2S_QuitAmuseRoom(void)
{
}

bool GameMsg_C2S_QuitAmuseRoom::doDecode(CParamPool& IOBuff)
{
    m_nQuitTo = IOBuff.GetUChar();
	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_ChangeAmuseRoomInfo::GameMsg_C2S_ChangeAmuseRoomInfo(void)
:GameMsg_Base(MSG_C2S_ChangeAmuseRoomInfo)
,m_strRoomName("")
,m_strRoomPwd("")
{
}

GameMsg_C2S_ChangeAmuseRoomInfo::~GameMsg_C2S_ChangeAmuseRoomInfo(void)
{
}

bool GameMsg_C2S_ChangeAmuseRoomInfo::doDecode(CParamPool& IOBuff)
{
	IOBuff.GetStringW( m_strRoomName );
	IOBuff.GetStringW( m_strRoomPwd );

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_AmuseKickPlayer::GameMsg_C2S_AmuseKickPlayer(void)
:GameMsg_Base(MSG_C2S_AmuseKickPlayer)
,m_nPlayerID(0)
{
}

GameMsg_C2S_AmuseKickPlayer::~GameMsg_C2S_AmuseKickPlayer(void)
{
}

bool GameMsg_C2S_AmuseKickPlayer::doDecode(CParamPool& IOBuff)
{
	m_nPlayerID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_AmusePromoteHost::GameMsg_C2S_AmusePromoteHost()
:GameMsg_Base(MSG_C2S_AmusePromoteHost)
,m_nNewHostID(0)
{

}

GameMsg_C2S_AmusePromoteHost::~GameMsg_C2S_AmusePromoteHost()
{

}

bool GameMsg_C2S_AmusePromoteHost::doDecode(CParamPool & IOBuff)
{
	m_nNewHostID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_AmuseInvitePlayer::GameMsg_C2S_AmuseInvitePlayer(void)
:GameMsg_Base(MSG_C2S_AmuseInvitePlayer)
{
}

GameMsg_C2S_AmuseInvitePlayer::~GameMsg_C2S_AmuseInvitePlayer(void)
{
}

bool GameMsg_C2S_AmuseInvitePlayer::doDecode(CParamPool& IOBuff)
{
	unsigned short nCount = IOBuff.GetUShort();
	for ( int i = 0; i < nCount; ++i )
	{
		int nInvitedID = IOBuff.GetInt();
		m_vecInviteeID.push_back( nInvitedID );
	}

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_AmuseInviteeResponse::GameMsg_C2S_AmuseInviteeResponse(void)
:GameMsg_Base(MSG_C2S_AmuseInviteeResponse)
,m_bAccept(false)
,m_nRoomID(0)
{
}

GameMsg_C2S_AmuseInviteeResponse::~GameMsg_C2S_AmuseInviteeResponse(void)
{
}

bool GameMsg_C2S_AmuseInviteeResponse::doDecode(CParamPool& IOBuff)
{
	m_bAccept = IOBuff.GetBool();
	m_nRoomID = IOBuff.GetUInt();

	return true;
}


///////////////////////////////////////////////////////
GameMsg_C2S_DoAmuseAction::GameMsg_C2S_DoAmuseAction(void)
:GameMsg_Base(MSG_C2S_DoAmuseAction)
,m_pActionParam(NULL)
{
}

GameMsg_C2S_DoAmuseAction::~GameMsg_C2S_DoAmuseAction(void)
{
	if ( m_pActionParam != NULL )
	{
		delete m_pActionParam;
		m_pActionParam = NULL;
	}
}

bool GameMsg_C2S_DoAmuseAction::doDecode(CParamPool& IOBuff)
{
	int nActionType = IOBuff.GetUChar();
	m_pActionParam = CAmuseActionParamFactory::BuildActionParam( nActionType );

	if ( m_pActionParam != NULL )
	{
		m_pActionParam->doDecode( IOBuff );
	}

	return true;
}

GameMsg_C2S_AmuseApplyMatch::GameMsg_C2S_AmuseApplyMatch( void )
:GameMsg_Base(MSG_C2S_AmuseApplyMatch)
{
	m_bCreate = true;
	m_chRoomScene = 0;
	m_chRoomTheme = 0;
}

GameMsg_C2S_AmuseApplyMatch::~GameMsg_C2S_AmuseApplyMatch( void )
{

}

bool GameMsg_C2S_AmuseApplyMatch::doDecode( CParamPool& IOBuff )
{
	m_bCreate = IOBuff.GetBool();
	IOBuff.GetStringW(m_strRoomName);
	m_chRoomTheme = IOBuff.GetUChar();
	m_chRoomScene = IOBuff.GetUChar();
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameMsg_C2S_PlayMakerSync::GameMsg_C2S_PlayMakerSync( void )
	:GameMsg_Base(MSG_C2S_PlayMakerSync)
{
	m_nLocationType = 0;
	m_nRoomID = 0;
	m_nReceiverType = 0;
	m_nSpecificID = 0;
}

GameMsg_C2S_PlayMakerSync::~GameMsg_C2S_PlayMakerSync( void )
{

}

bool GameMsg_C2S_PlayMakerSync::doDecode( CParamPool& IOBuff )
{
	m_nLocationType = IOBuff.GetUChar();
	m_nRoomID = IOBuff.GetInt();
	m_nReceiverType = IOBuff.GetUChar();
	m_nSpecificID = IOBuff.GetUInt();
	m_playerMakerInfo.doDecode(IOBuff);

	return true;
}

