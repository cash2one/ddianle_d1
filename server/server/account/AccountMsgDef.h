#ifndef __ACCOUNTMSGDEF_H__
#define __ACCOUNTMSGDEF_H__
#include "../share/CommonMsgDef.h"

enum eMSG_ACCOUNT //帐号相关消息。
{
	MSG_ACCOUNT_MSGBEGIN=MSG_TYPE_ACCOUNT,
	MSG_ACCOUNT_Login,						//账号登录=MSG_TYPE_ACCOUNT+1
	MSG_ACCOUNT_LoginResult	,				//账号登录结果
	MSG_ACCOUNT_ServerState	,				//获取服务器状态
	
	
	MSG_S2C_IsCanInviteFriend,				//这两个是内测期间 邀请好友加入
	MSG_C2S_IsCanInviteFriend,				//这两个是内测期间 邀请好友加入

	MSG_C2S_ChongZhi,						//在游戏内充值
	MSG_S2C_ChongZhiResult,
	
	MSG_C2S_CheckAccountIsExist,			//查询账号在游戏中存不存在
	MSG_S2C_CheckAccountIsExist,			
	
	MSG_S2C_KBINFO,							//客户端收到的KB信息
	MSG_C2C_BUISNESSID,						//客户端收到的交易号。

	MSG_ACCOUNT_CreateAccount,				//创建账号
	MSG_ACCOUNT_QuickCreateAccoout,			//快速创建账号(系统自动生成name,accout,uuid,password，返回name以及passwo
	MSG_ACCOUNT_QuickCreateAccooutResult,	//快速创建账号结果
	MSG_ACCOUNT_CreateAccountResult,		//创建账号的结果，失败的时候才能收到
	MSG_ACCOUNT_NotActivated,				//未激活
	MSG_ACCOUNT_CheckActivate,				//验证激活码
	MSG_ACCOUNT_CheckActivateResult,		//验证激活码结果
	MSG_ACCOUNT_HeartBeatRequest,			//心跳请求
	MSG_ACCOUNT_HeartBeatResponse,			//心跳回应
	MSG_ACCOUNT_C2S_PlayerLogout,			//玩家退出

	MSG_Account_C2S_GetGameServersInfo,		//获取服务器全部信息
	MSG_Account_S2C_GetGameServersInfoRes,	//AccountServer返回全部服务器的信息

    MSG_ACCOUNT_S2C_AllowedCurrencyList,    //支持的支付货币列表
    MSG_ACCOUNT_C2S_ClientDeviceInfo,       //玩家设备信息
    MSG_ACCOUNT_S2C_ValidChargeDevice,      //是否是合法的充值设备
    MSG_ACCOUNT_C2S_SaveChargeAction,       //玩家充值流程

	MSG_S2A_PlayerSwitchLine,				// 通知账号服务器玩家已切线

	MSG_ACCOUNT_RebindAccount,				// 玩家使用激活码绑定新账号
	MSG_ACCOUNT_RebindAccountResult,		// 返回消息
};

enum eAccountResult
{
	NONE,
	CREATE_SUCCESS,
	INVALID_PASSWORD_LENGTH,	//密码长度过长	
	INVALID_NAME_LENGTH,		//名称长度过长
	CONTAIN_INVALID_WORDS,		//名称包含非法字符
	NAME_NOT_EXITST,			// 名字不存在
	INVALID_PASSWORD,			//名称不对
	DUPLICATED_ACCOUNT_NAME,	//重复的注册名称
	DUPLICATED_ACCOUNT_UUID,	//重复UUID,由合作平台传递
	TRANSCODE_NOT_EXIST,		//账号迁移码不存在
	PLATFORM_FORBIDDEN_REG = 11,		//平台账户禁止注册,这个错误码不能动。

};

#endif



//file end

