#ifndef		__GMDEFINE_H__
#define		__GMDEFINE_H__

const int GM_MAX_NAME_LEN = 20;
const int GM_MAX_PASSWD_LEN = 20;




enum EKickOutReason
{
	EKickOutReason_Begin = 0,

	EKickOutReason_ReLogin,			// ÖØµÇÂ½
};

enum ETimerType
{
	ETimerType_None,
	ETimerType_Announce,
};

enum EUserPrivType
{
	EUserPrivType_None =					0x00000000,
	EUserPrivType_SendAnnounce =			0x00000001,
	EUserPrivType_SendSystemMail =			0x00000002,
	EUserPrivType_GenGiftCode =				0x00000004,
	EUserPrivType_SendMCoin	=				0x00000008,
	EUserPrivType_PlayerFrozen =			0x00000010,
	EUserPrivType_Compsition =				0x00000020,
	EUserPrivType_UserPriv =				0x00000040,
	EUserPrivType_QueryGMLog =				0x00000080,
	EUserPrivType_RoomManager =				0x00000100,
	EUserPrivType_SetMallUpdateTime =		0x00000200,
	EUserPrivType_GroupByControl =			0x00000400,
	EUserPrivType_PrivMCoin =				0x00000800,
	EUserPrivType_ServerPlayerCount =		0x00001000,
	EUserPrivType_ChangeAccountPassword =	0x00002000,
	EUserPrivType_SendBlackDevice =			0x00004000,
    EUserPrivType_InfoCenter =				0x00008000,
    EUserPrivType_Medal =                   0x00010000,
    EUserPrivType_Harass =				    0x00020000,
};

enum EGMActionType
{
    EGMActionType_None = 0,
    EGMActionType_SendAnnounce,
    EGMActionType_SendSystemMail,
    EGMActionType_GenGiftCode,              // xx
    EGMActionType_QueryGiftCode,
    EGMActionType_SendMCoin,
    EGMActionType_PlayerFrozen,
    EGMActionType_Compsition,
    EGMActionType_SetUserPriv,
    EGMActionType_SetMallUpdateTime,
    EGMActionType_GroupBuyControl,
    EGMActionType_AddPrivMCoin,
    EGMActionType_SendBlackDeviceList,
    EGMActionType_Medal, // 
    EGMActionType_InfoCenter, // 
    EGMActionType_ChangeAccountPassword,

};

enum EGMErrorType
{
	EGMErrorType_Success = 0,
	EGMErrorType_UserExist = 1,
	EGMErrorType_UserNotExist = 2,
	EGMErrorType_Databases = 3,
};

#endif




