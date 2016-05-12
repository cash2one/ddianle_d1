#ifndef		__GMMSGDEF_H__
#define		__GMMSGDEF_H__

enum GMMsgDef
{
	GM_Msg_Begin = 0x0100,

	GM_C2S_Login,
	GM_S2C_LoginSuc,
	GM_S2C_LoginFail,

	GM_C2S_SendMail,
	GM_S2C_SendMailRes,

	GM_C2S_QueryRangeGift,
	GM_S2C_QueryRangeGiftRes,

	GM_C2S_QueryMultiGift,
	GM_S2C_QueryMultiGiftRes,

	GM_C2S_GrantGMCmds,
	GM_S2C_GrantGMCmdsSuc,
	GM_S2C_GrantGMCmdsFail,

	GM_C2S_RemoveGMCmds,
	GM_C2S_RemoveGMCmdsSuc,
	GM_C2S_RemoveGMCmdsFail,

	GM_C2S_SendAnnounceOnce,
	GM_C2S_SendAnnounceInterval,
	GM_S2C_SendAnnounceSuc,
	GM_S2C_SendAnnounceFail,

	GM_C2S_GMAddMCoin,

	GM_C2S_GMLockOrUnLockAccount,

	GM_C2S_SendCompensation,
	GM_S2C_SendCompensationRes,


	GM_S2C_PrivError,

	GM_C2S_GetUsers,
	GM_S2C_GetUserRes,

	GM_C2S_SetPriv,
	GM_S2C_SetPrivRes,

	GM_C2S_AddUser,
	GM_S2C_AddUserRes,

	GM_C2S_DelUser,
	GM_S2C_DelUserRes,
	
	GM_C2S_GetServerLine,
	GM_S2C_GetServerLineRes,

	GM_C2S_GetServerRoomList,
	GM_S2C_GetServerRoomListRes,

	GM_C2S_SetMallUpdateTime,
	GM_C2S_GroupBuyControl,

	GM_S2C_SetMallUpdateTimeRet,
	GM_S2C_GroupBuyControlRet,

	GM_C2S_ChangeSelfPassword,
	GM_S2C_ChangeSelfPasswordRet,
	GM_C2S_ChangePassword,

	GM_C2S_SendPrivMCoin,
	GM_S2C_SendPrivMCoinRet,

	GM_C2S_GetServerPlayerInfo,
	GM_S2C_GetServerPlayerInfoRet,

	GM_C2S_SetServerPlayerInfo,
	GM_S2C_SetServerPlayerInfoRet,

	GM_S2C_ServerPlayerLimit,

	GM_C2S_SendBlackDeviceList,
	GM_S2C_SendBlackDeviceListRes,

	GM_C2S_ChangeAccountPassword,
	GM_S2C_ChangeAccountPasswordRes,

	GM_C2S_GetAnnounce,
	GM_S2C_GetAnnounceRes,
	GM_C2S_DeleteAnnounce,
	GM_S2C_DeleteAnnounceRes,

	GM_C2S_QueryLog,
    GM_S2C_QueryLogRet,
    GM_C2S_QueryLogItem,
    GM_S2C_QueryLogItemRet,

	GM_C2M_GetInfoCenterRes,
	GM_M2C_GetInfoCenterRes,
	GM_C2M_AddInfoCenterRes,
	GM_M2C_AddInfoCenterRes,
	GM_C2M_UpdateInfoCenterRes,
	GM_M2C_UpdateInfoCenterRes,
	GM_C2M_RemoveInfoCenterRes,
	GM_M2C_RemoveInfoCenterRes,

    GM_M2G_DeleteVersionMedal,
    GM_G2M_DeleteVersionMedal,
    GM_C2M_AddMedal,
    GM_M2C_AddMedal,

    // 查看封号列表
    GM_C2M_RequestLockList,
    GM_M2C_ResponseLockList,
    GM_Msg_C2M_HarassList,
    GM_Msg_M2C_HarassList,
    GM_Msg_C2M_RoleHarassChatInfo,
    GM_Msg_M2C_RoleHarassChatInfo,

    GM_C2M_RequestRechargeRecordList,
    GM_M2C_ResponseRechargeRecordList,

};


#endif


