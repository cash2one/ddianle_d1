#ifndef		__PHOTOROOMMACRODEFINE_H__
#define		__PHOTOROOMMACRODEFINE_H__


enum EPhotoRoomState 
{
	EPhotoRoomState_None,
	EPhotoRoomState_Free,
	EPhotoRoomState_Start,
	EPhotoRoomState_Destroy,
	EPhotoRoomState_Max,
};

enum EPhotoRoomPlayerState
{
	EPhotoRoomPlayerState_Free,						//玩家摄影棚状态 - free
	EPhotoRoomPlayerState_Ready,					//玩家摄影棚状态 - 准备
};

enum EPhotoRoomRange
{
	EPhotoRoomRange_Common_Begin		= 1,		//摄影棚房间开始编号
	EPhotoRoomRange_Common_End			= 999,		//摄影棚房间结束编号
};


//对应静态数据"主题表"
enum EPhotoRoomInviteType	
{
	EPhotoRoomInviteType_None,
	EPhotoRoomInviteType_InviteFriend,
	EPhotoRoomInviteType_Couple,
};

enum EPhotoRoomTheme
{
	EPhotoRoomTheme_None,
	EPhotoRoomTheme_Single,
	EPhotoRoomTheme_Normal,
	EPhotoRoomTheme_Couple,
	EPhotoRoomTheme_Star,
	EPhotoRoomTheme_Max,
};

enum ERoomExpiredNotiyType
{
	ERoomExpiredNotiyType_SystemTip,
	ERoomExpiredNotiyType_WindowTip,
	ERoomExpiredNotiyType_SysWindowTip,
};

enum ERoomClosedCause
{
	ERoomClosedCause_None = 0,
	ERoomClosedCause_HostClosed,
	ERoomClosedCause_TimeExpired,
};



#endif


