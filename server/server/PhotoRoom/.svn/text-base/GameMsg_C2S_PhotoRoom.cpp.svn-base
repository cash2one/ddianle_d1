#include "GameMsg_C2S_PhotoRoom.h"
#include "../../socket/ParamPool.h"
#include "PhotoRoomMsgDef.h"


//创建摄影棚请求
//-----------------------------------------------------GameMsg_C2S_CreatePhotoRoom
GameMsg_C2S_CreatePhotoRoom::GameMsg_C2S_CreatePhotoRoom()
:GameMsg_Base(MSG_C2S_CreatePhotoRoom)
{

}

GameMsg_C2S_CreatePhotoRoom::~GameMsg_C2S_CreatePhotoRoom()
{

}

bool GameMsg_C2S_CreatePhotoRoom::doDecode( CParamPool & IOBuff )
{
	m_nRoomTheme = IOBuff.GetUChar();

	return true;
}

//退出摄影棚请求
//---------------------------------------------------GameMsg_C2S_QuitPhotoRoom
GameMsg_C2S_QuitPhotoRoom::GameMsg_C2S_QuitPhotoRoom()
:GameMsg_Base(MSG_C2S_QuitPhotoRoom)
{

}

GameMsg_C2S_QuitPhotoRoom::~GameMsg_C2S_QuitPhotoRoom()
{

}

bool GameMsg_C2S_QuitPhotoRoom::doDecode( CParamPool & IOBuff )
{
	return true;
}

//关闭摄影棚请求
//-----------------------------------GameMsg_C2S_ClosePhotoRoom
GameMsg_C2S_ClosePhotoRoom::GameMsg_C2S_ClosePhotoRoom()
:GameMsg_Base(MSG_C2S_ClosePhotoRoom)
{

}

GameMsg_C2S_ClosePhotoRoom::~GameMsg_C2S_ClosePhotoRoom()
{

}

bool GameMsg_C2S_ClosePhotoRoom::doDecode( CParamPool & IOBuff )
{
	return true;
}

//邀请玩家
//-----------------------------------GameMsg_C2S_InvitePhotoRoomPlayer
GameMsg_C2S_InvitePhotoRoomPlayer::GameMsg_C2S_InvitePhotoRoomPlayer()
:GameMsg_Base(MSG_C2S_InvitePhotoRoomPlayer)
{

}

GameMsg_C2S_InvitePhotoRoomPlayer::~GameMsg_C2S_InvitePhotoRoomPlayer()
{

}

bool GameMsg_C2S_InvitePhotoRoomPlayer::doDecode( CParamPool & IOBuff )
{
	int nCount = IOBuff.GetUShort();
	while(nCount > 0) {
		unsigned int nRoleID = IOBuff.GetUInt();	
		m_listInvitees.push_back(nRoleID);
		--nCount;
	}

	return true;
}

//被邀请者回应
//------------------------------------GameMsg_C2S_PhotoRoomInviteeResponse
GameMsg_C2S_PhotoRoomInviteeResponse::GameMsg_C2S_PhotoRoomInviteeResponse()
:GameMsg_Base(MSG_C2S_PhotoRoomInviteeResponse)
,m_bAccept(false)
,m_nRoomID(0)
{

}

GameMsg_C2S_PhotoRoomInviteeResponse::~GameMsg_C2S_PhotoRoomInviteeResponse()
{

}

bool GameMsg_C2S_PhotoRoomInviteeResponse::doDecode( CParamPool & IOBuff )
{
	m_bAccept = IOBuff.GetBool();
	m_nRoomID = IOBuff.GetUInt();

	return true;
}


//尝试进入自己的摄影棚
//--------------------------------------GameMsg_C2S_TryEnterMyPhotoRoom
GameMsg_C2S_TryEnterMyPhotoRoom::GameMsg_C2S_TryEnterMyPhotoRoom()
:GameMsg_Base(MSG_C2S_TryEnterMyPhotoRoom)
,m_nRoomType(0)
{

}

GameMsg_C2S_TryEnterMyPhotoRoom::~GameMsg_C2S_TryEnterMyPhotoRoom()
{

}

bool GameMsg_C2S_TryEnterMyPhotoRoom::doDecode( CParamPool & IOBuff )
{
	m_nRoomType = IOBuff.GetUChar();

	return true;
}

//踢人
//------------------------------------GameMsg_C2S_KickPhotoRoomPlayer
GameMsg_C2S_KickPhotoRoomPlayer::GameMsg_C2S_KickPhotoRoomPlayer()
:GameMsg_Base(MSG_C2S_KickPhotoRoomPlayer)
,m_nBeKickedID(0)
{

}

GameMsg_C2S_KickPhotoRoomPlayer::~GameMsg_C2S_KickPhotoRoomPlayer()
{

}

bool GameMsg_C2S_KickPhotoRoomPlayer::doDecode( CParamPool & IOBuff )
{
	m_nBeKickedID = IOBuff.GetUInt();

	return true;
}

//玩家改变状态
//------------------------------------GameMsg_C2S_ChangePhotoRoomPlayerState
GameMsg_C2S_ChangePhotoRoomPlayerState::GameMsg_C2S_ChangePhotoRoomPlayerState()
:GameMsg_Base(MSG_C2S_ChangeRoomPlayerState)
,m_nState(0)
{

}

GameMsg_C2S_ChangePhotoRoomPlayerState::~GameMsg_C2S_ChangePhotoRoomPlayerState()
{

}

bool GameMsg_C2S_ChangePhotoRoomPlayerState::doDecode( CParamPool & IOBuff )
{
	m_nState = IOBuff.GetUChar();

	return true;
}

//玩家改变房间状态
//------------------------------------GameMsg_C2S_KickPhotoRoomPlayer
GameMsg_C2S_ChangePhotoRoomState::GameMsg_C2S_ChangePhotoRoomState()
:GameMsg_Base(MSG_C2S_ChangePhotoRoomState)
,m_nRoomState(0)
{

}

GameMsg_C2S_ChangePhotoRoomState::~GameMsg_C2S_ChangePhotoRoomState()
{

}

bool GameMsg_C2S_ChangePhotoRoomState::doDecode( CParamPool & IOBuff )
{
		m_nRoomState = IOBuff.GetUChar();

		return true;
}

//改变玩家的房间动作
//------------------------------------GameMsg_C2S_ChangePhotoRoomAction
GameMsg_C2S_ChangePhotoRoomAction::GameMsg_C2S_ChangePhotoRoomAction()
:GameMsg_Base(MSG_C2S_ChangePhotoRoomAction)
,m_nActionID(0)
{

}

GameMsg_C2S_ChangePhotoRoomAction::~GameMsg_C2S_ChangePhotoRoomAction()
{

}

bool GameMsg_C2S_ChangePhotoRoomAction::doDecode( CParamPool & IOBuff )
{
	m_nActionID = IOBuff.GetUChar();

	return true;
}

//改变玩家的房间背景
//------------------------------------GameMsg_C2S_ChangePhotoRoomBG
GameMsg_C2S_ChangePhotoRoomBG::GameMsg_C2S_ChangePhotoRoomBG()
:GameMsg_Base(MSG_C2S_ChangePhotoRoomBG)
,m_nBGID(0)
{

}

GameMsg_C2S_ChangePhotoRoomBG::~GameMsg_C2S_ChangePhotoRoomBG()
{

}

bool GameMsg_C2S_ChangePhotoRoomBG::doDecode( CParamPool & IOBuff )
{
	m_nBGID = IOBuff.GetUChar();

	return true;
}

//获取玩家自身的摄影棚房间信息
//------------------------------------GameMsg_C2S_GetMyPhotoRoomInfo
GameMsg_C2S_GetMyPhotoRoomInfo::GameMsg_C2S_GetMyPhotoRoomInfo()
:GameMsg_Base(MSG_C2S_GetMyPhotoRoomInfo)
{

}

GameMsg_C2S_GetMyPhotoRoomInfo::~GameMsg_C2S_GetMyPhotoRoomInfo()
{

}

bool GameMsg_C2S_GetMyPhotoRoomInfo::doDecode( CParamPool & IOBuff )
{
	return true;
}

//摄影棚续费
//------------------------------------GameMsg_C2S_RenewPhotoRoom
GameMsg_C2S_RenewPhotoRoom::GameMsg_C2S_RenewPhotoRoom()
:GameMsg_Base(MSG_C2S_RenewPhotoRoom)
{

}

GameMsg_C2S_RenewPhotoRoom::~GameMsg_C2S_RenewPhotoRoom()
{

}

bool GameMsg_C2S_RenewPhotoRoom::doDecode( CParamPool & IOBuff )
{
	return true;
}


//------------------------------------
GameMsg_C2S_PressPhotoButton::GameMsg_C2S_PressPhotoButton()
    :GameMsg_Base(MSG_C2S_PressPhotoButtonAction)
{

}

GameMsg_C2S_PressPhotoButton::~GameMsg_C2S_PressPhotoButton()
{

}

bool GameMsg_C2S_PressPhotoButton::doDecode( CParamPool & IOBuff )
{
    return true;
}
