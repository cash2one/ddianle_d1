#ifndef		__COUPLEMSGDEF_H__
#define		__COUPLEMSGDEF_H__

#include "../share/CommonMsgDef.h"

enum eMSG_COUPLE
{
	MSG_MARRIAGE_Begin = MSG_TYPE_MARRIAGE,

	MSG_C2S_SendCoupleRequest,
	MSG_S2C_SendCoupleRequestSuc,
	MSG_S2C_SendCoupleRequestFail,

	MSG_S2C_ReceiveCoupleRequest,

	MSG_C2S_AcceptCoupleRequest,
	MSG_S2C_AcceptCoupleRequestSuc,
	MSG_S2C_AcceptCoupleRequestFail,
	MSG_C2S_RefuseCoupleRequest,

	MSG_S2C_CoupleRequestReply,

	MSG_C2S_CoupleSeparateRequest,
	MSG_S2C_CoupleSeparateRequestSuc,
	MSG_S2C_CoupleSeparateRequestFail,
	MSG_S2C_NotifyHasBeenSeparated,

	MSG_C2S_RequestMarry,
	MSG_S2C_RequestGotMarriedFail,
	MSG_S2C_StartWedding,

	MSG_C2S_RequestDivorce,
	MSG_S2C_RequestDivorceSuc,
	MSG_S2C_RequestDivorceFail,
	MSG_S2C_NotifyHasBeenDivorced,

	MSG_S2C_UpdateCoupleIntimacy,
	MSG_S2C_UpdateCoupleTitle,
	MSG_S2C_UpdateCoupleName,

	MSG_C2S_ChangeLoveDeclaration,
	MSG_S2C_ChangeLoveDeclarationSuc,
	MSG_S2C_ChangeLoveDeclarationFail,
	MSG_S2C_NotifyChangeLoveDeclaration,

	MSG_C2S_GetWeddingRoomList,
	MSG_S2C_GetWeddingRoomRes,

	MSG_C2S_CreateWeddingRoom,
	MSG_S2C_CreateWeddingRoomFail,

	MSG_C2S_TryEnterWeddingRoom,
	MSG_S2C_TryEnterWeddingRoomFail,

	MSG_C2S_EnterWeddingRoom,
	MSG_S2C_EnterWeddingRoomSuc,	// MSG_S2C_EnterWeddingRoomSuc
	MSG_S2C_EnterWeddingRoomFail,

	MSG_C2S_RequestEnterMyWeddingRoom,
	MSG_S2C_EnterMyWeddingRoomFail,

	MSG_C2S_ExitWeddingRoom,
	MSG_S2C_ExitWeddingRoomSuc,
	MSG_S2C_ExitWeddingRoomFail,

	MSG_S2C_EnterWeddingPlayer,	// MSG_S2C_EnterWeddingPlayer
	MSG_S2C_PlayerExitedRoom,

	MSG_C2S_ChangeWeddingRoomPwd,
	MSG_S2C_ChangeWeddingRoomPwdSuc,
	MSG_S2C_ChangeWeddingRoomPwdFail,

	MSG_C2S_SetWeddingPlayerType,
	MSG_S2C_SetWeddingPlayerTypeSuc,
	MSG_S2C_SetWeddingPlayerTypeFail,
	MSG_S2C_NotifyChangeWeddingPlayerType,

	MSG_S2C_WeddingRoomBeginNotification,
	MSG_S2C_AutoStartWeddingRoomFail,

	MSG_C2S_KickWeddingRoomPlayer,
	MSG_S2C_KickWeddingPlayerSuc,
	MSG_S2C_KickWeddingPlayerFail,

	MSG_S2C_WeddingRoomStateChanged,
	MSG_S2C_BlessingNotify,
	MSG_C2S_GetBlessingRankList,
	MSG_S2C_GetBlessingRankListResult,
	MSG_C2S_GetWeddingRecordList,
	MSG_S2C_GetWddingRecordListResult,
	MSG_C2S_GetCoupleInfo,
	MSG_S2C_GetCoupleInfoResult,
    MSG_C2S_GetBlessingValue,
	MSG_S2C_GetBlessingValueReslt,
	MSG_C2S_BlessingPair,
	MSG_S2C_BlessingPairSuc,
	MSG_C2S_MoneyBlessingPair,
	MSG_S2C_MoneyBlessingPairSuc,
	MSG_S2C_BlessingError,
	MSG_C2S_GetPersonInfo,
	MSG_S2C_GetPersonInfoResult,
	MSG_C2S_GetCoupleRedEnvelopeGiftInfo,
	MSG_S2C_GetCoupleRedEnvelopeGiftResult,

	// divorce
	MSG_C2S_RequestCancelDivorce,			//申请撤销离婚
	MSG_S2C_RequestCancelDivorceSuc,		//申请撤销离婚成功
	MSG_S2C_RequestCancelDivorceFail,	    //申请撤销离婚失败
	MSG_C2S_RequestCompulsoryDivorce,	    //申请强制离婚（即销毁分手信）
	MSG_S2C_RequestCompulsoryDivorceSuc,	//申请强制离婚成功
	MSG_S2C_RequestCompulsoryDivorceFail,	//申请强制离婚失败
	MSG_C2S_AgreeDivorce,					//同意离婚
	MSG_S2C_AgreeDivorceSuc,				//同意离婚成功
	MSG_S2C_AgreeDivorceFail,				//同意离婚失败
	MSG_C2S_DivorceChat,					//离婚聊天
	MSG_S2C_DivorceChatSuc,					//离婚聊天成功
	MSG_S2C_DivorceChatFail,				//离婚聊天失败
	MSG_C2S_DivorceSendGift,				//离婚送礼（分手礼物）
	MSG_S2C_DivorceSendGiftSuc,				//离婚送礼成功
	MSG_S2C_DivorceSendGiftFail,			//离婚送礼失败
	MSG_S2C_ReciveDivorceAction,			//离婚行为通知
	MSG_C2S_RequestCoupleWish,				//请求对方愿望列表
	MSG_S2C_RequestCoupleWishRes,			//获取对方愿望列表
	MSG_C2S_RequestEnterDivorceAnimation,	//请求进入离婚动画场景
	MSG_S2C_EnterDivorceAnimationSuc,		//进入离婚动画场景
	MSG_S2C_EnterDivorceAnimationFail,		//进入离婚动画场景失败
	MSG_S2C_DivorceHaveNewCouple,		    //有了新情侣，消耗协议书通知


	// between servers
	MSG_S2G_AcceptCoupleRequst,
	MSG_S2G_CoupleSeparate,
	MSG_G2S_NotifyHasBeenSeparated,

	MSG_S2G_GetMarried,

	MSG_S2G_Divorce,
	MSG_G2S_NofiyHasBeenDivorced,

	MSG_S2G_WeddingRoomStartingNofication,
	MSG_G2S_WeddingRoomStaringNofication,

	MSG_S2G_AutoStartWeddingRoomFail,
	MSG_G2S_AutoStartWeddingRoomFail,

	MSG_S2G_UpdateCoupleIntimacy,
	MSG_G2S_UpdateCoupleIntimacy,

	MSG_G2S_CoupleNameChanged,
	MSG_S2G_BlessingNotify,
	MSG_G2S_BlessingNotify,
	MSG_S2G_UpdateWeddingRecord,
	MSG_S2G_BlessingPair,
	MSG_G2S_BlessingCouple,
	MSG_S2G_GetWeddingRecordList,
	MSG_G2S_GetWeddingRecordList,
	MSG_S2G_AddWeddingRecord,
	MSG_S2G_UpateWeddingRecordState,
	MSG_S2G_GetBlessingRankList,
	MSG_G2S_GetBlessingRankList,
	MSG_S2G_MoneyBlessingPair,
	MSG_G2S_AddRedEnvelopeBlessingValue,
	MSG_G2S_RedEnvelopeDelMoney,
	MSG_S2G_GetCoupleInfo,
	MSG_G2S_GetCoupleInfo,
	MSG_S2G_GetBlessingValue,
	MSG_G2S_GetBlessingValue,
	MSG_S2G_GetPersonInfo,
	MSG_G2S_GetPersonInfo,
	MSG_G2S_BlessingError,
	MSG_G2S_BlessingPairSuc,
	MSG_G2S_MoneyBlessingPairSuc,
	MSG_S2G_WeddingNotice,
	MSG_G2S_WeddingNotice,
	MSG_S2G_AddRedEnvelopeBlessingValue,

	// divorce
	MSG_S2G_RefreshDivorceAction,
	MSG_G2S_RefreshDivorceAction,
	MSG_G2S_GetDivorceAnimationPersonInfo,
	MSG_S2G_GetDivorceAnimationPersonInfo,
	
};


#endif


