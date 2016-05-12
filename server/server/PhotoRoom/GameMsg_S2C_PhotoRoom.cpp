#include "GameMsg_S2C_PhotoRoom.h"
#include "PhotoRoomMsgDef.h"

//������Ӱ��ʧ��
//---------------------------------------------------------------GameMsg_S2C_CreatePhotoRoomFail
GameMsg_S2C_CreatePhotoRoomFail::GameMsg_S2C_CreatePhotoRoomFail()
:GameMsg_Base(MSG_S2C_CreatePhotoRoomFail)
,m_eErrno(ECreatePhotoRoomError_Success)
{

}

GameMsg_S2C_CreatePhotoRoomFail::~GameMsg_S2C_CreatePhotoRoomFail()
{

}

bool GameMsg_S2C_CreatePhotoRoomFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eErrno);

	return true;
}

//������Ӱ��ɹ�
//------------------------------------------------------------GameMsg_S2C_EnterPhotoRoomSuc
GameMsg_S2C_EnterPhotoRoomSuc::GameMsg_S2C_EnterPhotoRoomSuc()
:GameMsg_Base(MSG_S2C_EnterPhotoRoomSuc)
{

}

GameMsg_S2C_EnterPhotoRoomSuc::~GameMsg_S2C_EnterPhotoRoomSuc()
{

}

bool GameMsg_S2C_EnterPhotoRoomSuc::doEncode( CParamPool &IOBuff )
{
	m_wholeRoomInfo.doEncode(IOBuff);

	return true;
}

//����ҽ�����Ӱ��
//------------------------------------------------------------GameMsg_S2C_EnterPhotoRoomPlayer
GameMsg_S2C_EnterPhotoRoomPlayer::GameMsg_S2C_EnterPhotoRoomPlayer()
:GameMsg_Base(MSG_S2C_EnterPhotoRoomPlayer)
{

}

GameMsg_S2C_EnterPhotoRoomPlayer::~GameMsg_S2C_EnterPhotoRoomPlayer()
{

}

bool GameMsg_S2C_EnterPhotoRoomPlayer::doEncode( CParamPool &IOBuff )
{
	m_photoRoomPlayerInfo.doEncode(IOBuff);

	return true;
}

//����˳���Ӱ��ɹ�
//----------------------------------------------------------GameMsg_S2C_QuitPhotoRoomSuc
GameMsg_S2C_QuitPhotoRoomSuc::GameMsg_S2C_QuitPhotoRoomSuc()
:GameMsg_Base(MSG_S2C_QuitPhotoRoomSuc)
,m_nKickedBy(0)
{

}

GameMsg_S2C_QuitPhotoRoomSuc::~GameMsg_S2C_QuitPhotoRoomSuc()
{

}

bool GameMsg_S2C_QuitPhotoRoomSuc::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUInt(m_nKickedBy);

	return true;
}

//����˳���Ӱ��ʧ��
//-----------------------------------------------------------GameMsg_S2C_QuitPhotoRoomFail
GameMsg_S2C_QuitPhotoRoomFail::GameMsg_S2C_QuitPhotoRoomFail()
:GameMsg_Base(MSG_S2C_QuitPhotoRoomFail)
,m_eErrno(EQuitPhotoRoomError_Success)
{

}

GameMsg_S2C_QuitPhotoRoomFail::~GameMsg_S2C_QuitPhotoRoomFail()
{

}

bool GameMsg_S2C_QuitPhotoRoomFail::doEncode( CParamPool &IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eErrno);

	return true;
}

//����˳��˷���
//------------------------------------------------------GameMsg_S2C_PlayerQuitPhotoRoom
GameMsg_S2C_PlayerQuitPhotoRoom::GameMsg_S2C_PlayerQuitPhotoRoom()
:GameMsg_Base(MSG_S2C_PlayerQuitPhotoRoom)
,m_nRoleID(0)
{
	
}

GameMsg_S2C_PlayerQuitPhotoRoom::~GameMsg_S2C_PlayerQuitPhotoRoom()
{

}

bool GameMsg_S2C_PlayerQuitPhotoRoom::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);

	return true;
}


//��ҹر���Ӱ��ɹ�
//-----------------------------------GameMsg_S2C_ClosePhotoRoomSuc
//GameMsg_S2C_ClosePhotoRoomSuc::GameMsg_S2C_ClosePhotoRoomSuc()
//:GameMsg_Base(MSG_S2C_ClosePhotoRoomSuc)
//{
//
//}
//
//GameMsg_S2C_ClosePhotoRoomSuc::~GameMsg_S2C_ClosePhotoRoomSuc()
//{
//
//}
//
//bool GameMsg_S2C_ClosePhotoRoomSuc::doEncode( CParamPool & IOBuff )
//{
//	return true;
//}

//��ҹر���Ӱ��ʧ��
//----------------------------------GameMsg_S2C_ClosePhotoRoomFail
GameMsg_S2C_ClosePhotoRoomFail::GameMsg_S2C_ClosePhotoRoomFail()
:GameMsg_Base(MSG_S2C_ClosePhotoRoomFail)
,m_eErrno(EClosePhotoRoomError_Success)
{

}

GameMsg_S2C_ClosePhotoRoomFail::~GameMsg_S2C_ClosePhotoRoomFail()
{

}

bool GameMsg_S2C_ClosePhotoRoomFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eErrno);
	
	return true;
}

//������ҳɹ�
//----------------------------------GameMsg_S2C_InvitePhotoRoomPlayerSuc
GameMsg_S2C_InvitePhotoRoomPlayerSuc::GameMsg_S2C_InvitePhotoRoomPlayerSuc()
:GameMsg_Base(MSG_S2C_InvitePhotoRoomPlayerSuc)
{

}

GameMsg_S2C_InvitePhotoRoomPlayerSuc::~GameMsg_S2C_InvitePhotoRoomPlayerSuc()
{

}

bool GameMsg_S2C_InvitePhotoRoomPlayerSuc::doEncode( CParamPool & IOBuff )
{
	return true;
}

//�������ʧ��
//----------------------------------GameMsg_S2C_InvitePhotoRoomPlayerFail
GameMsg_S2C_InvitePhotoRoomPlayerFail::GameMsg_S2C_InvitePhotoRoomPlayerFail()
:GameMsg_Base(MSG_S2C_InvitePhotoRoomPlayerFail)
,m_eError(EInvitePhotoRoomError_Success)
{

}

GameMsg_S2C_InvitePhotoRoomPlayerFail::~GameMsg_S2C_InvitePhotoRoomPlayerFail()
{

}

bool GameMsg_S2C_InvitePhotoRoomPlayerFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError); 

	return true;
}

//֪ͨ���������
//----------------------------------GameMsg_S2C_PhotoRoomInviteeNotification
GameMsg_S2C_PhotoRoomInviteeNotification::GameMsg_S2C_PhotoRoomInviteeNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomInviteeNotification)
,m_nRoomID(0)
,m_nRoomTheme(0)
,m_nInviterID(0)
,m_strInviterName("")
{

}

GameMsg_S2C_PhotoRoomInviteeNotification::~GameMsg_S2C_PhotoRoomInviteeNotification()
{

}

bool GameMsg_S2C_PhotoRoomInviteeNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddUChar(m_nRoomTheme);
	IOBuff.AddUInt(m_nInviterID);
	IOBuff.AddStringW(m_strInviterName.c_str());

	return true;
}

//������ظ�ʧ��
//----------------------------------GameMsg_S2C_PhotoRoomInviteeResponseFail
GameMsg_S2C_PhotoRoomInviteeResponseFail::GameMsg_S2C_PhotoRoomInviteeResponseFail()
:GameMsg_Base(MSG_S2C_PhotoRoomInviteeResponseFail)
,m_eError(PhotoRoomInviteeResponseError_Success)
{

}

GameMsg_S2C_PhotoRoomInviteeResponseFail::~GameMsg_S2C_PhotoRoomInviteeResponseFail()
{

}

bool GameMsg_S2C_PhotoRoomInviteeResponseFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}


//���Խ����ҵ���Ӱ��ʧ��
//----------------------------------------GameMsg_S2C_TryEnterMyPhotoRoomFail
GameMsg_S2C_TryEnterMyPhotoRoomFail::GameMsg_S2C_TryEnterMyPhotoRoomFail()
:GameMsg_Base(MSG_S2C_TryEnterMyPhotoRoomFail)
,m_eError(ETryEnterMyPhotoRoomError_Success)
{

}

GameMsg_S2C_TryEnterMyPhotoRoomFail::~GameMsg_S2C_TryEnterMyPhotoRoomFail()
{

}

bool GameMsg_S2C_TryEnterMyPhotoRoomFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//���˳ɹ�
//----------------------------------------GameMsg_S2C_KickPhotoRoomPlayerSuc
GameMsg_S2C_KickPhotoRoomPlayerSuc::GameMsg_S2C_KickPhotoRoomPlayerSuc()
:GameMsg_Base(MSG_S2C_KickPhotoRoomPlayerSuc)
,m_nKickedID(0)
{

}

GameMsg_S2C_KickPhotoRoomPlayerSuc::~GameMsg_S2C_KickPhotoRoomPlayerSuc()
{

}

bool GameMsg_S2C_KickPhotoRoomPlayerSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nKickedID);

	return true;
}

//����ʧ��
//----------------------------------------GameMsg_S2C_TryEnterMyPhotoRoomFail
GameMsg_S2C_KickPhotoRoomPlayerFail::GameMsg_S2C_KickPhotoRoomPlayerFail()
:GameMsg_Base(MSG_S2C_KickPhotoRoomPlayerFail)
,m_eError(EKickPhotoRoomPlayerError_Success )
{

}

GameMsg_S2C_KickPhotoRoomPlayerFail::~GameMsg_S2C_KickPhotoRoomPlayerFail()
{

}

bool GameMsg_S2C_KickPhotoRoomPlayerFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//��Ҹı�״̬�ɹ�
//----------------------------------------GameMsg_S2C_ChangeRoomPlayerStateSuc
GameMsg_S2C_ChangeRoomPlayerStateSuc::GameMsg_S2C_ChangeRoomPlayerStateSuc()
:GameMsg_Base(MSG_S2C_ChangeRoomPlayerStateSuc)
,m_eState(EPhotoRoomPlayerState_Free)
{

}

GameMsg_S2C_ChangeRoomPlayerStateSuc::~GameMsg_S2C_ChangeRoomPlayerStateSuc()
{
	
}

bool GameMsg_S2C_ChangeRoomPlayerStateSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eState);

	return true;
}


//��Ҹı�״̬ʧ��
//--------------------------------------GameMsg_S2C_ChangeRoomPlayerStateFail
GameMsg_S2C_ChangeRoomPlayerStateFail::GameMsg_S2C_ChangeRoomPlayerStateFail()
:GameMsg_Base(MSG_S2C_ChangeRoomPlayerStateFail)
,m_eError(EPhotoRoomPlayerChangeStateError_Success)
{

}

GameMsg_S2C_ChangeRoomPlayerStateFail::~GameMsg_S2C_ChangeRoomPlayerStateFail()
{

}

bool GameMsg_S2C_ChangeRoomPlayerStateFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//֪ͨ��Ҹı�״̬
//--------------------------------------GameMsg_S2C_ChangeRoomPlayerStateFail
GameMsg_S2C_PhotoRoomPlayerChangeStateNotification::GameMsg_S2C_PhotoRoomPlayerChangeStateNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomPlayerChangeStateNotification)
,m_nRoleID(0)
,m_eState(EPhotoRoomPlayerState_Free)
{

}

GameMsg_S2C_PhotoRoomPlayerChangeStateNotification::~GameMsg_S2C_PhotoRoomPlayerChangeStateNotification()
{

}

bool GameMsg_S2C_PhotoRoomPlayerChangeStateNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoleID);
	IOBuff.AddUChar((unsigned char)m_eState);
	
	return true;
}

//�ı���Ӱ�﷿��״̬
//--------------------------------------GameMsg_S2C_ChangePhotoRoomStateSuc
GameMsg_S2C_ChangePhotoRoomStateSuc::GameMsg_S2C_ChangePhotoRoomStateSuc()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomStateSuc)
,m_eRoomState(EPhotoRoomState_None)
{

}

GameMsg_S2C_ChangePhotoRoomStateSuc::~GameMsg_S2C_ChangePhotoRoomStateSuc()
{

}

bool GameMsg_S2C_ChangePhotoRoomStateSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eRoomState);

	return true;
}

//�ı���Ӱ�﷿��״̬ʧ��
//--------------------------------------GameMsg_S2C_ChangePhotoRoomStateFail
GameMsg_S2C_ChangePhotoRoomStateFail::GameMsg_S2C_ChangePhotoRoomStateFail()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomStateFail)
,m_eError(EChangePhotoRoomStateError_Success)
{

}

GameMsg_S2C_ChangePhotoRoomStateFail::~GameMsg_S2C_ChangePhotoRoomStateFail()
{

}

bool GameMsg_S2C_ChangePhotoRoomStateFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//֪ͨ�ı���Ӱ�﷿��״̬
//--------------------------------------GameMsg_S2C_PhotoRoomStateChangedNotification
GameMsg_S2C_PhotoRoomStateChangedNotification::GameMsg_S2C_PhotoRoomStateChangedNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomStateChangedNotification)
,m_eRoomState(EPhotoRoomState_None)
{

}

GameMsg_S2C_PhotoRoomStateChangedNotification::~GameMsg_S2C_PhotoRoomStateChangedNotification()
{

}

bool GameMsg_S2C_PhotoRoomStateChangedNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eRoomState);

	return true;
}

//�ı���Ӱ�﷿�䶯���ɹ�
//--------------------------------------GameMsg_S2C_ChangePhotoRoomActionSuc
GameMsg_S2C_ChangePhotoRoomActionSuc::GameMsg_S2C_ChangePhotoRoomActionSuc()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomActionSuc)
,m_nActionID(0)
{

}

GameMsg_S2C_ChangePhotoRoomActionSuc::~GameMsg_S2C_ChangePhotoRoomActionSuc()
{

}

bool GameMsg_S2C_ChangePhotoRoomActionSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar(m_nActionID);

	return true;
}

//�ı���Ӱ�﷿�䶯��ʧ��
//--------------------------------------GameMsg_S2C_ChangePhotoRoomActionFail
GameMsg_S2C_ChangePhotoRoomActionFail::GameMsg_S2C_ChangePhotoRoomActionFail()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomActionFail)
,m_eError(EChangePhotoRoomActionError_Success)
{

}

GameMsg_S2C_ChangePhotoRoomActionFail::~GameMsg_S2C_ChangePhotoRoomActionFail()
{

}

bool GameMsg_S2C_ChangePhotoRoomActionFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//֪ͨ�ı���Ӱ�﷿�䶯��
//--------------------------------------GameMsg_S2C_PhotoRoomActionChangedNotification
GameMsg_S2C_PhotoRoomActionChangedNotification::GameMsg_S2C_PhotoRoomActionChangedNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomActionNotification)
,m_nActionID(0)
{

}

GameMsg_S2C_PhotoRoomActionChangedNotification::~GameMsg_S2C_PhotoRoomActionChangedNotification()
{

}

bool GameMsg_S2C_PhotoRoomActionChangedNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar(m_nActionID);

	return true;
}

//�ı���Ӱ�﷿�䱳���ɹ�
//--------------------------------------GameMsg_S2C_ChangePhotoRoomBGSuc
GameMsg_S2C_ChangePhotoRoomBGSuc::GameMsg_S2C_ChangePhotoRoomBGSuc()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomBGSuc)
,m_nBGID(0)
{

}

GameMsg_S2C_ChangePhotoRoomBGSuc::~GameMsg_S2C_ChangePhotoRoomBGSuc()
{

}

bool GameMsg_S2C_ChangePhotoRoomBGSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_nBGID);

	return true;
}

//�ı���Ӱ�﷿�䱳��ʧ��
//--------------------------------------GameMsg_S2C_ChangePhotoRoomBGFail
GameMsg_S2C_ChangePhotoRoomBGFail::GameMsg_S2C_ChangePhotoRoomBGFail()
:GameMsg_Base(MSG_S2C_ChangePhotoRoomBGFail)
,m_eError(EChangePhotoRoomBGError_Success)
{

}

GameMsg_S2C_ChangePhotoRoomBGFail::~GameMsg_S2C_ChangePhotoRoomBGFail()
{

}

bool GameMsg_S2C_ChangePhotoRoomBGFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//֪ͨ�ı���Ӱ�﷿�䱳��
//--------------------------------------GameMsg_S2C_PhotoRoomBGChangedNotification
GameMsg_S2C_PhotoRoomBGChangedNotification::GameMsg_S2C_PhotoRoomBGChangedNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomBGNotification)
,m_nBGID(0)
{

}

GameMsg_S2C_PhotoRoomBGChangedNotification::~GameMsg_S2C_PhotoRoomBGChangedNotification()
{

}

bool GameMsg_S2C_PhotoRoomBGChangedNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_nBGID);

	return true;
}

//��Ӱ���������
//--------------------------------------GameMsg_S2C_PhotoRoomExpiredNotification
GameMsg_S2C_PhotoRoomExpiredNotification::GameMsg_S2C_PhotoRoomExpiredNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomExpiredNotification)
,m_eNotifyType(ERoomExpiredNotiyType_SystemTip)
{

}

GameMsg_S2C_PhotoRoomExpiredNotification::~GameMsg_S2C_PhotoRoomExpiredNotification()
{

}

bool GameMsg_S2C_PhotoRoomExpiredNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddInt(m_nTimeRemain);
	IOBuff.AddUChar((unsigned char)m_eNotifyType);

	return true;
}

//��ȡ���������Ӱ����Ϣ�ɹ�
//--------------------------------------GameMsg_S2C_GetMyPhotoRoomInfoSuc
GameMsg_S2C_GetMyPhotoRoomInfoSuc::GameMsg_S2C_GetMyPhotoRoomInfoSuc()
:GameMsg_Base(MSG_S2C_GetMyPhotoRoomsSuc)
,m_nRoomID(0)
,m_eThemeID(EPhotoRoomTheme_None)
,m_nTimeRemain(0)
{

}

GameMsg_S2C_GetMyPhotoRoomInfoSuc::~GameMsg_S2C_GetMyPhotoRoomInfoSuc()
{

}

bool GameMsg_S2C_GetMyPhotoRoomInfoSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nRoomID);
	IOBuff.AddUChar((unsigned char)m_eThemeID);
	IOBuff.AddInt(m_nTimeRemain);

	return true;
}

//��ȡ���������Ӱ����Ϣʧ��
//--------------------------------------GameMsg_S2C_GetMyPhotoRoomInfoFail
GameMsg_S2C_GetMyPhotoRoomInfoFail::GameMsg_S2C_GetMyPhotoRoomInfoFail()
:GameMsg_Base(MSG_S2C_GetMyPhotoRoomsFail)
,m_eError(EGetMyPhotoRoomInfoError_Success)
{

}

GameMsg_S2C_GetMyPhotoRoomInfoFail::~GameMsg_S2C_GetMyPhotoRoomInfoFail()
{

}

bool GameMsg_S2C_GetMyPhotoRoomInfoFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);

	return true;
}

//�ر���Ӱ��֪ͨ����
//--------------------------------------GameMsg_S2C_PhotoRoomCloseNotification
GameMsg_S2C_PhotoRoomCloseNotification::GameMsg_S2C_PhotoRoomCloseNotification()
:GameMsg_Base(MSG_S2C_PhotoRoomClosedNotification)
,m_eRoomClosedCause(ERoomClosedCause_None)
{

}

GameMsg_S2C_PhotoRoomCloseNotification::~GameMsg_S2C_PhotoRoomCloseNotification()
{

}

bool GameMsg_S2C_PhotoRoomCloseNotification::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eRoomClosedCause);

	return true;
}

//��Ӱ�����ѳɹ�
//--------------------------------------GameMsg_S2C_RenewPhotoRoomSuc
GameMsg_S2C_RenewPhotoRoomSuc::GameMsg_S2C_RenewPhotoRoomSuc()
:GameMsg_Base(MSG_S2C_RenewPhotoRoomSuc)
,m_nTime(0)
{

}

GameMsg_S2C_RenewPhotoRoomSuc::~GameMsg_S2C_RenewPhotoRoomSuc()
{

}

bool GameMsg_S2C_RenewPhotoRoomSuc::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUInt(m_nTime);

	return true;
}

//��Ӱ������ʧ��
//--------------------------------------GameMsg_S2C_RenewPhotoRoomFail
GameMsg_S2C_RenewPhotoRoomFail::GameMsg_S2C_RenewPhotoRoomFail()
:GameMsg_Base(MSG_S2C_RenewPhotoRoomFail)
,m_eError(ERenewPhotoRoomError_Success)
{

}

GameMsg_S2C_RenewPhotoRoomFail::~GameMsg_S2C_RenewPhotoRoomFail()
{

}

bool GameMsg_S2C_RenewPhotoRoomFail::doEncode( CParamPool & IOBuff )
{
	IOBuff.AddUChar((unsigned char)m_eError);
	
	return true;
}



