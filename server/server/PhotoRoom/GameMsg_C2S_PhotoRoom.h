#ifndef		__GAMEMSG_C2S_PHOTOROOM_H__
#define		__GAMEMSG_C2S_PHOTOROOM_H__

#include <list>
#include "../../socket/GameMsg_Base.h"

//������Ӱ������
//--------------------------------GameMsg_C2S_CreatePhotoRoom
class GameMsg_C2S_CreatePhotoRoom : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_CreatePhotoRoom);
	GameMsg_C2S_CreatePhotoRoom();
	~GameMsg_C2S_CreatePhotoRoom();
public:
	unsigned char m_nRoomTheme;
	bool doDecode(CParamPool & IOBuff);
};

//�˳���Ӱ������
//---------------------------------GameMsg_C2S_QuitPhotoRoom
class GameMsg_C2S_QuitPhotoRoom : 
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_QuitPhotoRoom);
	GameMsg_C2S_QuitPhotoRoom();
	~GameMsg_C2S_QuitPhotoRoom();
public:
	bool doDecode(CParamPool & IOBuff);
};

//�ر���Ӱ������
//-----------------------------------GameMsg_C2S_ClosePhotoRoom
class GameMsg_C2S_ClosePhotoRoom :
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ClosePhotoRoom);
	GameMsg_C2S_ClosePhotoRoom();
	~GameMsg_C2S_ClosePhotoRoom();
public:
	bool doDecode(CParamPool & IOBuff);
};

//�������
//-----------------------------------GameMsg_C2S_InvitePhotoRoomPlayer
class GameMsg_C2S_InvitePhotoRoomPlayer:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_InvitePhotoRoomPlayer);
	GameMsg_C2S_InvitePhotoRoomPlayer();
	~GameMsg_C2S_InvitePhotoRoomPlayer();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	std::list<unsigned int> m_listInvitees;
};

//�������߻�Ӧ
//------------------------------------GameMsg_C2S_PhotoRoomInviteeResponse
class GameMsg_C2S_PhotoRoomInviteeResponse:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_PhotoRoomInviteeResponse);
	GameMsg_C2S_PhotoRoomInviteeResponse();
	~GameMsg_C2S_PhotoRoomInviteeResponse();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	bool m_bAccept;
	unsigned int m_nRoomID;
};

//���Խ����Լ�����Ӱ��
//------------------------------------GameMsg_C2S_TryEnterMyPhotoRoom
class GameMsg_C2S_TryEnterMyPhotoRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_TryEnterMyPhotoRoom);
	GameMsg_C2S_TryEnterMyPhotoRoom();
	~GameMsg_C2S_TryEnterMyPhotoRoom();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nRoomType;
};

//����
//------------------------------------GameMsg_C2S_KickPhotoRoomPlayer
class GameMsg_C2S_KickPhotoRoomPlayer:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_KickPhotoRoomPlayer);
	GameMsg_C2S_KickPhotoRoomPlayer();
	~GameMsg_C2S_KickPhotoRoomPlayer();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nBeKickedID;
};

//��Ҹı�״̬
//------------------------------------GameMsg_C2S_ChangePhotoRoomPlayerState
class GameMsg_C2S_ChangePhotoRoomPlayerState:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangePhotoRoomPlayerState);
	GameMsg_C2S_ChangePhotoRoomPlayerState();
	~GameMsg_C2S_ChangePhotoRoomPlayerState();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nState;
};

//��Ҹı䷿��״̬
//------------------------------------GameMsg_C2S_KickPhotoRoomPlayer
class GameMsg_C2S_ChangePhotoRoomState:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangePhotoRoomState);
	GameMsg_C2S_ChangePhotoRoomState();
	~GameMsg_C2S_ChangePhotoRoomState();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nRoomState;
};

//�ı���ҵķ��䶯��
//------------------------------------GameMsg_C2S_ChangePhotoRoomAction
class GameMsg_C2S_ChangePhotoRoomAction:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangePhotoRoomAction);
	GameMsg_C2S_ChangePhotoRoomAction();
	~GameMsg_C2S_ChangePhotoRoomAction();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nActionID;
};

//�ı���ҵķ��䱳��
//------------------------------------GameMsg_C2S_ChangePhotoRoomBG
class GameMsg_C2S_ChangePhotoRoomBG:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ChangePhotoRoomBG);
	GameMsg_C2S_ChangePhotoRoomBG();
	~GameMsg_C2S_ChangePhotoRoomBG();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nBGID;
};

//��ȡ����������Ӱ�﷿����Ϣ
//------------------------------------GameMsg_C2S_GetMyPhotoRoomInfo
class GameMsg_C2S_GetMyPhotoRoomInfo:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetMyPhotoRoomInfo);
	GameMsg_C2S_GetMyPhotoRoomInfo();
	~GameMsg_C2S_GetMyPhotoRoomInfo();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

//��Ӱ������
//------------------------------------GameMsg_C2S_RenewPhotoRoom
class GameMsg_C2S_RenewPhotoRoom:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_RenewPhotoRoom);
	GameMsg_C2S_RenewPhotoRoom();
	~GameMsg_C2S_RenewPhotoRoom();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

// ��ҵ�����ఴť
class GameMsg_C2S_PressPhotoButton:
    public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_PressPhotoButton);
    GameMsg_C2S_PressPhotoButton();
    ~GameMsg_C2S_PressPhotoButton();
public:
    virtual bool doDecode(CParamPool & IOBuff);
};

#endif


