#ifndef		__PHOTOSTUDIOMSGDEF_H__
#define		__PHOTOSTUDIOMSGDEF_H__


#include "../share/CommonMsgDef.h"

enum EMSG_PhotoStudio
{
	MSG_PhotoStudio_Begin = MSG_TYPE_PHOTOSTUDIO,

	MSG_C2S_CreatePhotoRoom,						//创建摄影房间
	MSG_S2C_CreatePhotoRoomFail,					//创建摄影房间结果

	MSG_C2S_ClosePhotoRoom,							//关闭摄影棚
	//MSG_S2C_ClosePhotoRoomSuc,					//关闭摄影棚成功
	MSG_S2C_ClosePhotoRoomFail,						//关闭摄影棚失败

	MSG_C2S_TryEnterMyPhotoRoom,					//尝试进入自己创建的摄影棚请求(成功发送MSG_S2C_EnterPhotoRoomSuc)
	MSG_S2C_TryEnterMyPhotoRoomFail,				//尝试进入自己创建的摄影棚失败

	MSG_C2S_EnterPhotoRoom,							//申请进入房间
	MSG_S2C_EnterPhotoRoomSuc,						//进入摄影房间成功
	MSG_S2C_EnterPhotoRoomFail,						//进入摄影房间失败
	MSG_S2C_EnterPhotoRoomPlayer,					//有玩家进入房间

	MSG_C2S_QuitPhotoRoom,							//退出摄影棚
	MSG_S2C_QuitPhotoRoomSuc,						//退出摄影棚成功
	MSG_S2C_QuitPhotoRoomFail,						//退出摄影棚失败
	MSG_S2C_PlayerQuitPhotoRoom,					//有玩家退出了摄影棚

	MSG_C2S_InvitePhotoRoomPlayer,					//邀请摄影棚玩家
	MSG_S2C_InvitePhotoRoomPlayerSuc,				//邀请摄影棚玩家成功
	MSG_S2C_InvitePhotoRoomPlayerFail,				//邀请摄影棚玩家失败
	MSG_S2C_PhotoRoomInviteeNotification,			//通知被邀请者
	MSG_C2S_PhotoRoomInviteeResponse,				//被邀请者回应
	MSG_S2C_PhotoRoomInviteeResponseFail,			//被邀请者回应反馈

	MSG_C2S_KickPhotoRoomPlayer,					//踢人
	MSG_S2C_KickPhotoRoomPlayerSuc,					//踢人成功
	MSG_S2C_KickPhotoRoomPlayerFail,				//踢人失败

	MSG_C2S_ChangeRoomPlayerState,					//玩家状态改变
	MSG_S2C_ChangeRoomPlayerStateSuc,				//改变状态成功		
	MSG_S2C_ChangeRoomPlayerStateFail,				//改变状态失败	
	MSG_S2C_PhotoRoomPlayerChangeStateNotification,	//通知状态改变

	MSG_C2S_ChangePhotoRoomState,					//改变摄影棚状态
	MSG_S2C_ChangePhotoRoomStateSuc,				//改变摄影棚状态成功
	MSG_S2C_ChangePhotoRoomStateFail,				//改变摄影棚状态失败
	MSG_S2C_PhotoRoomStateChangedNotification,		//通知摄影棚状态改变

	MSG_C2S_ChangePhotoRoomAction,					//改变摄影棚动作
	MSG_S2C_ChangePhotoRoomActionSuc,				//改变摄影棚动作成功
	MSG_S2C_ChangePhotoRoomActionFail,				//改变摄影棚动作失败
	MSG_S2C_PhotoRoomActionNotification,			//通知摄影棚动作改变

	MSG_C2S_ChangePhotoRoomBG,						//改变摄影棚背景
	MSG_S2C_ChangePhotoRoomBGSuc,					//改变摄影棚背景成功
	MSG_S2C_ChangePhotoRoomBGFail,					//改变摄影棚背景失败
	MSG_S2C_PhotoRoomBGNotification,				//通知摄影棚背景改变

	MSG_S2C_PhotoRoomExpiredNotification,			//房间剩余时间提醒

	MSG_C2S_GetMyPhotoRoomInfo,						//获取我的摄影棚的房间信息
	MSG_S2C_GetMyPhotoRoomsSuc,						//返回获取我的摄影棚的房间信息成功
	MSG_S2C_GetMyPhotoRoomsFail,					//返回获取我的摄影棚的房间信息失败

	MSG_S2C_PhotoRoomClosedNotification,			//摄影棚关闭提醒

	MSG_C2S_RenewPhotoRoom,							//摄影棚续费
	MSG_S2C_RenewPhotoRoomSuc,						//摄影棚续费成功
	MSG_S2C_RenewPhotoRoomFail,						//摄影棚续费失败

    MSG_C2S_PressPhotoButtonAction,				    //使用照相按钮
};



#endif





