#ifndef		__GAMEMSG_S2C_PHOTOROOMIO_H__
#define		__GAMEMSG_S2C_PHOTOROOMIO_H__

#include "../../socket/GameMsg_Base.h"
#include "PhotoRoomDataStruct.h"

class CParamPool;

enum ECreatePhotoRoomError
{
	ECreatePhotoRoomError_Success = 0,					//成功
	ECreatePhotoRoomError_RoomTypeError,				//房间类型不合法
	ECreatePhotoRoomError_LevelNotEnough,				//等级不够
	ECreatePhotoRoomError_NotHasRequireItem,			//没有物品
	ECreatePhotoRoomError_AlreadyHasOwnRoomCurLine,		//已经在本线创建了房间
	ECreatePhotoRoomError_InPhotoRoom,					//已经在摄影房间
	ECreatePhotoRoomError_InDanceRoom,					//已经在跳舞房间
	ECreatePhotoRoomError_InWeddingRoom,				//已经在婚房
	ECreatePhotoRoomError_InLobby,						//已经在大厅
	ECreatePhotoRoomError_RoomsLimited,					//超过摄影棚房间最大数
	ECreatePhotoRoomError_InAmuseRoom,
    ECreatePhotoRoomError_InCeremonyRoom,               
	ECreatePhotoRoomError_NotOpen,						//暂未开放
	ECreatePhotoRoomError_Unkonwn,						//未知
};

enum EQuitPhotoRoomError
{
	EQuitPhotoRoomError_Success = 0,					//成功
	EQuitPhotoRoomError_NotInRoom,						//不在房间
};

enum EClosePhotoRoomError
{
	EClosePhotoRoomError_Success = 0,					//成功
	EClosePhotoRoomError_NotInRoom,						//不在房间内
	EClosePhotoRoomError_NotHost,						//不是房主
};

enum EInvitePhotoRoomError
{
	EInvitePhotoRoomError_Success = 0,					//邀请成功
	EInvitePhotoRoomError_NotInRoom,					//不在房间
	EInvitePhotoRoomError_RoomNotSupport,				//房间不能邀请
	EInvitePhotoRoomError_NotHost,						//非房主
	EInvitePhotoRoomError_NotCouple,					//非情侣
	EInvitePhotoRoomError_TooMany,						//邀请超过上限
	EInvitePhotoRoomError_TooFrequent,					//太频繁
	EInvitePhotoRoomError_InOtherPlace,					//已经在其他位置 
	EInvitePhotoRoomError_InviteSelf,					//无法邀请自己
	EInvitePhotoRoomError_InvalidData,					//非正常数据
	EInvitePhotoRoomError_AlreadyInPhotoRoom,			//已经在房间
};

enum ETryEnterMyPhotoRoomError
{
	ETryEnterMyPhotoRoomError_Success = 0,				//成功
	ETryEnterMyPhotoRoomError_NotCreateSuchRoom,		//没有自己创建的房间
	ETryEnterMyPhotoRoomError_InPhotoRoom,				//已经在摄影房间
	ETryEnterMyPhotoRoomError_InDanceRoom,				//已经在跳舞房间
	ETryEnterMyPhotoRoomError_InWeddingRoom,			//已经在婚房
	ETryEnterMyPhotoRoomError_InLobby,					//已经在大厅
	ETryEnterMyPhotoRoomError_HasAnotherRoom,			//已经拥有其他类型的房间
	ETryEnterMyPhotoRoomError_InAmuseRoom,
	ETryEnterMyPhotoRoomError_Unkonwn,					//未知
    ETryEnterMyPhotoRoomError_InCeremonyRoom,
};

enum EKickPhotoRoomPlayerError
{
	EKickPhotoRoomPlayerError_Success = 0,				//成功
	EKickPhotoRoomPlayerError_NotHost,					//不是房主
	EKickPhotoRoomPlayerError_NotInRoom,				//不在房间
	EKickPhotoRoomPlayerError_BeKickedPlayerNotInRoom,	//被踢玩家不在房间
	EKickPhotoRoomPlayerError_KickSelf,					//踢除自己
	EKickPhotoRoomPlayerError_UnKnown,					//未知错误
};

enum PhotoRoomInviteeResponseError
{
	PhotoRoomInviteeResponseError_Success,				//成功
	PhotoRoomInviteeResponseError_InDancingRoom,		//在跳舞房间
	PhotoRoomInviteeResponseError_InWeddingRoom,		//在婚房
	PhotoRoomInviteeResponseError_NoPhotoRoom,			//不存在摄影棚
	PhotoRoomInviteeResponseError_NotSupportInvite,		//不支持邀请
	PhotoRoomInviteeResponseError_RoomFull,				//摄影棚房满
	PhotoRoomInviteeResponseError_NoInvitation,			//没被邀请或者已超时
	PhotoRoomInviteeResponseError_InOhterPlace,			//在别的场景
	PhotoRoomInviteeResponseError_InvalidData,			//不合法数据
	PhotoRoomInviteeResponseError_Started,				//摄影棚已经开始
	PhotoRoomInviteeResponseError_InAmuseRoom,
    PhotoRoomInviteeResponseError_InCeremonyRoom,
};

enum EPhotoRoomPlayerChangeStateError
{
	EPhotoRoomPlayerChangeStateError_Success,			//成功
	EPhotoRoomPlayerChangeStateError_NotInPhotoRoom,	//不在房间
	EPhotoRoomPlayerChangeStateError_RoomStart,			//房间已处于准备状态
	EPhotoRoomPlayerChangeStateError_InvalidData,		//非法数据
};

enum EChangePhotoRoomStateError
{
	EChangePhotoRoomStateError_Success,					//成功
	EChangePhotoRoomStateError_NotInPhotoRoom,			//不在摄影棚房间
	EChangePhotoRoomStateError_NotHost,					//不是房主
	EChangePhotoRoomStateError_NotAllPlayerReady,		//有人没准备	
	EChangePhotoRoomStateError_RoomStateNotExist,		//状态不存在
	EChangePhotoRoomStateError_NotChange,				//状态没改变
	EChangePhotoRoomStateError_InvalidData,				//非法数据
};

enum EChangePhotoRoomActionError
{
	EChangePhotoRoomActionError_Success,				//成功
	EChangePhotoRoomActionError_NotInPhotoRoom,			//不在摄影棚
	EChangePhotoRoomActionError_NotHost,				//非房主
	EChangePhotoRoomActionError_RoomNotStarted,			//房间已经开始
	EChangePhotoRoomActionError_NotSuchAction,			//不存在该动作
	EChangePhotoRoomActionError_RoomNotSupport,			//房间不支持
	EChangePhotoRoomActionError_NotEnoughItemCount,		//所需物品不足
	EChangePhotoRoomActionError_InvalidData,			//非法数据
	EChangePhotoRoomActionError_Unkonwn,				//未知原因
};

enum EChangePhotoRoomBGError
{
	EChangePhotoRoomBGError_Success,					//成功
	EChangePhotoRoomBGError_NotInPhotoRoom,				//不在摄影棚
	EChangePhotoRoomBGError_NotHost,					//非房主
	EChangePhotoRoomBGError_RoomStarted,				//房间已经开始
	EChangePhotoRoomBGError_NotSuchBG,					//不存在该背景
	EChangePhotoRoomBGError_RoomNotSupport,				//房间不支持
	EChangePhotoRoomBGError_NotEnoughtItemCount,		//所需物品不足
	EChangePhotoRoomBGError_InvidData,					//非法数据
	EChangePhotoRoomBGError_Unknown,					//未知原因
};

enum EGetMyPhotoRoomInfoError
{
	EGetMyPhotoRoomInfoError_Success,					//成功
	EGetMyPhotoRoomInfoError_unknown,					//未知
};

enum ERenewPhotoRoomError
{
	ERenewPhotoRoomError_Success,						//成功
	ERenewPhotoRoomError_NoRoom,						//没房间
	ERenewPhotoRoomError_NotInRoom,						//不在房间
	ERenewPhotoRoomError_RoomNotSupport,				//房间不支持
	ERenewPhotoRoomError_NotEnoughItem,					//物品不足
	ERenewPhotoRoomError_Unknown,						//未知
};

//创建摄影棚失败
//-------------------------------------------GameMsg_S2C_CreatePhotoRoomFail
class GameMsg_S2C_CreatePhotoRoomFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_CreatePhotoRoomFail();
	~GameMsg_S2C_CreatePhotoRoomFail();
public:
	ECreatePhotoRoomError m_eErrno;
public:
	bool doEncode(CParamPool & IOBuff);	
};

//进入摄影棚成功
//-------------------------------------------GameMsg_S2C_EnterPhotoRoomSuc
class GameMsg_S2C_EnterPhotoRoomSuc : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_EnterPhotoRoomSuc();
	~GameMsg_S2C_EnterPhotoRoomSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CPhotoRoomWholeInfo m_wholeRoomInfo;		
};

//有玩家进入摄影棚
//-------------------------------------------GameMsg_S2C_EnterPhotoRoomPlayer
class GameMsg_S2C_EnterPhotoRoomPlayer : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_EnterPhotoRoomPlayer();
	~GameMsg_S2C_EnterPhotoRoomPlayer();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CPhotoRoomPlayerInfo m_photoRoomPlayerInfo;
};

//玩家退出摄影棚成功
//-------------------------------------------GameMsg_S2C_QuitPhotoRoomSuc
class GameMsg_S2C_QuitPhotoRoomSuc : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_QuitPhotoRoomSuc();
	~GameMsg_S2C_QuitPhotoRoomSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nKickedBy;
};

//玩家退出摄影棚失败
//-------------------------------------------GameMsg_S2C_QuitPhotoRoomFail
class GameMsg_S2C_QuitPhotoRoomFail : 
	public GameMsg_Base
{
public:
	GameMsg_S2C_QuitPhotoRoomFail();
	~GameMsg_S2C_QuitPhotoRoomFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	EQuitPhotoRoomError m_eErrno;
};

//玩家退出了房间
//-------------------------------------------GameMsg_S2C_EnterPhotoRoomPlayer
class GameMsg_S2C_PlayerQuitPhotoRoom :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PlayerQuitPhotoRoom();	
	~GameMsg_S2C_PlayerQuitPhotoRoom();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nRoleID;
};

//玩家关闭摄影棚成功
//-----------------------------------GameMsg_S2C_ClosePhotoRoomSuc
//class GameMsg_S2C_ClosePhotoRoomSuc :
//	public GameMsg_Base
//{
//public:
//	GameMsg_S2C_ClosePhotoRoomSuc();	
//	~GameMsg_S2C_ClosePhotoRoomSuc();
//public:
//	virtual bool doEncode(CParamPool & IOBuff);
//};

//玩家关闭摄影棚失败
//----------------------------------GameMsg_S2C_ClosePhotoRoomFail
class GameMsg_S2C_ClosePhotoRoomFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ClosePhotoRoomFail();	
	~GameMsg_S2C_ClosePhotoRoomFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EClosePhotoRoomError m_eErrno;
};

//邀请玩家成功
//----------------------------------GameMsg_S2C_InvitePhotoRoomPlayerSuc
class GameMsg_S2C_InvitePhotoRoomPlayerSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_InvitePhotoRoomPlayerSuc();	
	~GameMsg_S2C_InvitePhotoRoomPlayerSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
};

//邀请玩家失败
//----------------------------------GameMsg_S2C_InvitePhotoRoomPlayerFail
class GameMsg_S2C_InvitePhotoRoomPlayerFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_InvitePhotoRoomPlayerFail();	
	~GameMsg_S2C_InvitePhotoRoomPlayerFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EInvitePhotoRoomError m_eError;
};

//通知被邀请玩家
//----------------------------------GameMsg_S2C_PhotoRoomInviteeNotification
class GameMsg_S2C_PhotoRoomInviteeNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomInviteeNotification();	
	~GameMsg_S2C_PhotoRoomInviteeNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nRoomID;
	unsigned char m_nRoomTheme;
	unsigned int m_nInviterID;
	std::string m_strInviterName;
};

//被邀请回复失败
//----------------------------------GameMsg_S2C_PhotoRoomInviteeResponseFail
class GameMsg_S2C_PhotoRoomInviteeResponseFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomInviteeResponseFail();	
	~GameMsg_S2C_PhotoRoomInviteeResponseFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	PhotoRoomInviteeResponseError m_eError;
};

//尝试进入我的摄影棚失败
//----------------------------------------GameMsg_S2C_TryEnterMyPhotoRoomFail
class GameMsg_S2C_TryEnterMyPhotoRoomFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_TryEnterMyPhotoRoomFail();	
	~GameMsg_S2C_TryEnterMyPhotoRoomFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	ETryEnterMyPhotoRoomError m_eError;
};

//踢人成功
//----------------------------------------GameMsg_S2C_KickPhotoRoomPlayerSuc
class GameMsg_S2C_KickPhotoRoomPlayerSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_KickPhotoRoomPlayerSuc();	
	~GameMsg_S2C_KickPhotoRoomPlayerSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nKickedID;
};

//踢人失败
//----------------------------------------GameMsg_S2C_TryEnterMyPhotoRoomFail
class GameMsg_S2C_KickPhotoRoomPlayerFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_KickPhotoRoomPlayerFail();	
	~GameMsg_S2C_KickPhotoRoomPlayerFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EKickPhotoRoomPlayerError m_eError;
};

//玩家改变状态成功
//----------------------------------------GameMsg_S2C_ChangeRoomPlayerStateSuc
class GameMsg_S2C_ChangeRoomPlayerStateSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoomPlayerStateSuc();	
	~GameMsg_S2C_ChangeRoomPlayerStateSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EPhotoRoomPlayerState m_eState;
};

//玩家改变状态失败
//--------------------------------------GameMsg_S2C_ChangeRoomPlayerStateFail
class GameMsg_S2C_ChangeRoomPlayerStateFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangeRoomPlayerStateFail();	
	~GameMsg_S2C_ChangeRoomPlayerStateFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EPhotoRoomPlayerChangeStateError m_eError;
};

//通知玩家改变状态
//--------------------------------------GameMsg_S2C_ChangeRoomPlayerStateFail

class GameMsg_S2C_PhotoRoomPlayerChangeStateNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomPlayerChangeStateNotification();	
	~GameMsg_S2C_PhotoRoomPlayerChangeStateNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nRoleID;
	EPhotoRoomPlayerState m_eState;
};

//改变摄影棚房间状态成功
//--------------------------------------GameMsg_S2C_ChangePhotoRoomStateSuc
class GameMsg_S2C_ChangePhotoRoomStateSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomStateSuc();	
	~GameMsg_S2C_ChangePhotoRoomStateSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EPhotoRoomState m_eRoomState;
};

//改变摄影棚房间状态失败
//--------------------------------------GameMsg_S2C_ChangePhotoRoomStateFail
class GameMsg_S2C_ChangePhotoRoomStateFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomStateFail();	
	~GameMsg_S2C_ChangePhotoRoomStateFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EChangePhotoRoomStateError m_eError;
};

//通知改变摄影棚房间状态
//--------------------------------------GameMsg_S2C_PhotoRoomStateChangedNotification
class GameMsg_S2C_PhotoRoomStateChangedNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomStateChangedNotification();	
	~GameMsg_S2C_PhotoRoomStateChangedNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EPhotoRoomState m_eRoomState;
};

//改变摄影棚房间动作成功
//--------------------------------------GameMsg_S2C_ChangePhotoRoomActionSuc
class GameMsg_S2C_ChangePhotoRoomActionSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomActionSuc();	
	~GameMsg_S2C_ChangePhotoRoomActionSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nActionID;
};

//改变摄影棚房间动作失败
//--------------------------------------GameMsg_S2C_ChangePhotoRoomActionFail
class GameMsg_S2C_ChangePhotoRoomActionFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomActionFail();	
	~GameMsg_S2C_ChangePhotoRoomActionFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EChangePhotoRoomActionError m_eError;
};

//通知改变摄影棚房间动作
//--------------------------------------GameMsg_S2C_PhotoRoomActionChangedNotification
class GameMsg_S2C_PhotoRoomActionChangedNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomActionChangedNotification();	
	~GameMsg_S2C_PhotoRoomActionChangedNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nActionID;
};


//改变摄影棚房间背景成功
//--------------------------------------GameMsg_S2C_ChangePhotoRoomBGSuc
class GameMsg_S2C_ChangePhotoRoomBGSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomBGSuc();	
	~GameMsg_S2C_ChangePhotoRoomBGSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nBGID;
};

//改变摄影棚房间背景失败
//--------------------------------------GameMsg_S2C_ChangePhotoRoomBGFail
class GameMsg_S2C_ChangePhotoRoomBGFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_ChangePhotoRoomBGFail();	
	~GameMsg_S2C_ChangePhotoRoomBGFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EChangePhotoRoomBGError m_eError;
};

//通知改变摄影棚房间背景
//--------------------------------------GameMsg_S2C_PhotoRoomBGChangedNotification
class GameMsg_S2C_PhotoRoomBGChangedNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomBGChangedNotification();	
	~GameMsg_S2C_PhotoRoomBGChangedNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nBGID;
};

//摄影棚结束提醒
//--------------------------------------GameMsg_S2C_PhotoRoomExpiredNotification
class GameMsg_S2C_PhotoRoomExpiredNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomExpiredNotification();	
	~GameMsg_S2C_PhotoRoomExpiredNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	int m_nTimeRemain;
	ERoomExpiredNotiyType m_eNotifyType;
};

//获取玩家自身摄影棚信息成功
//--------------------------------------GameMsg_S2C_GetMyPhotoRoomInfoSuc
class GameMsg_S2C_GetMyPhotoRoomInfoSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetMyPhotoRoomInfoSuc();	
	~GameMsg_S2C_GetMyPhotoRoomInfoSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nRoomID;
	EPhotoRoomTheme m_eThemeID;
	int m_nTimeRemain;
};

//获取玩家自身摄影棚信息失败
//--------------------------------------GameMsg_S2C_GetMyPhotoRoomInfoFail
class GameMsg_S2C_GetMyPhotoRoomInfoFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetMyPhotoRoomInfoFail();	
	~GameMsg_S2C_GetMyPhotoRoomInfoFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	EGetMyPhotoRoomInfoError m_eError;
};

//关闭摄影棚通知房主
//--------------------------------------GameMsg_S2C_PhotoRoomCloseNotification
class GameMsg_S2C_PhotoRoomCloseNotification :
	public GameMsg_Base
{
public:
	GameMsg_S2C_PhotoRoomCloseNotification();	
	~GameMsg_S2C_PhotoRoomCloseNotification();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	ERoomClosedCause m_eRoomClosedCause;
};

//摄影棚续费成功
//--------------------------------------GameMsg_S2C_RenewPhotoRoomSuc
class GameMsg_S2C_RenewPhotoRoomSuc :
	public GameMsg_Base
{
public:
	GameMsg_S2C_RenewPhotoRoomSuc();	
	~GameMsg_S2C_RenewPhotoRoomSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	unsigned int m_nTime;
};

//摄影棚续费失败
//--------------------------------------GameMsg_S2C_RenewPhotoRoomFail
class GameMsg_S2C_RenewPhotoRoomFail :
	public GameMsg_Base
{
public:
	GameMsg_S2C_RenewPhotoRoomFail();	
	~GameMsg_S2C_RenewPhotoRoomFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	ERenewPhotoRoomError m_eError;
};



#endif


