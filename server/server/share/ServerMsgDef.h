#ifndef __SERVERMSGDEF_H__
#define __SERVERMSGDEF_H__

#include "CommonMsgDef.h"

///////////////////////////////////////////////////////////////////////////////////////
//所有0x0000 - 0x00FF之间的消息值保留给底层使用
//All messages between 0x0000 to 0x00FF are reserved, please do not use them in game!!!
///////////////////////////////////////////////////////////////////////////////////////

#define GetMsgType(nMsg)		((nMsg) & 0xFF00)

enum eMSG_SERVER
{
	MSG_SERVER_MSGBEGIN = MSG_TYPE_SERVER,
	MSG_SERVER_Connected,
	MSG_SERVER_LoginInfo,
	MSG_SERVER_Login,
	MSG_SERVER_LoginResult,

	MSG_ACCOUNT_BillingInfo	,				//账号billing信息
	MSG_ACCOUNT_RefreshBillInfo,			//刷新账号billing信息

	MSG_SERVER_State,		//服务器状态
	MSG_SERVER_Version,		//服务器版本
	MSG_SERVER_DropClient,	//踢掉客户

	MSG_PLAYER_LOGOUT,		//这几个先放这儿吧。。。
	MSG_PLAYER_C2S_LOGOUTFORSWITCHSERVER,//切线，需要下线后给回复消息。
	MSG_PLAYER_S2C_LOGOUTFORSWITCHSERVERCOMPLETE,
	MSG_C2S_AskPlayerInfo,
	MSG_S2C_AskPlayerInfo,

	//以下消息用于通过groupserver转发一些不需要回应的消息
	MSG_S2G_ToClient,			//发送消息到其他线上的客户端；GameServer调用DirectClientMsg(LPMSG, UINT)
	MSG_G2S_ToClient,			//配合上面一个消息完成消息传递
	MSG_S2G_ToGameServer,		//发送消息到其他线上的服务器；GameServer调用DirectServerMsg(LPMSG, UINT)
	MSG_G2S_ToGameServer,		//配合上面一个消息完成消息传递

	//host
	MSG_S2G_GameServerInfo,			//GameServer向GroupServer报告自己的信息 // not used any more, see MSG_S2G_GameServerLinkInfo, by myLike
	MSG_Host_Announce,			//公告
	MSG_Host_KickRole,			//踢掉角色
	MSG_Host_Shutdown,			//停机


	//agent msg
	MSG_AGENT_Stat,				//汇报ServerState
	MSG_AGENT_SysInfo,			//汇报系统CPU，mem信息
	MSG_AGENT_Execute,			//Monitor通知Agent执行系统指令
	MSG_AGENT_Close,			//关闭服务器
	MSG_AGENT_Start,			//开启服务
	MSG_AGENT_Setting,			//改变Agent的设定
	MSG_AGENT_Announce,			//向GameServer全员通告
	MSG_AGENT_Exit,				//关闭Agent自己
	MSG_AGENT_Kill,				//强制关闭服务器进程
	MSG_AGENT_ShutdownInTime,	//定时关闭服务器



	/////////////////
	MSG_SERVER_TRANSMIT				,	 	//转发的消息
	MSG_SERVER_LinkCount			,	 	//连接服务器的连接数
	MSG_SERVER_Command				,	 	//控制消息
	MSG_SERVER_TAG					,	 	//传递TAG
	MSG_SERVER_LinkInfo				,	 	//传递连接地址信息 // not used any more, see MSG_S2G_GameServerLinkInfo, by myLike
	MSG_SERVER_AccountLogout		,		//账号登出
	MSG_SERVER_AccountLogin			,		//账号登入

	MSG_S2G_ToClientForSendGift		,		//通知有人给您送礼物了
	MSG_G2S_ToClientForSendGift		,

	MSG_C2S_GIVE_SUGGESTION			,		//用户反馈
	MSG_Game2GatePlayerLogout		,		//角色下线，通知GateServer

	MSG_A2GBill_GiveGift			,		//充值送礼，账号服务器完成充值送礼后通知game
	MSG_QuestRandomNum				,		//groupserver随机每日任务
	MSG_SendGift_Control			,		//充值送礼开关
	MSG_A2GFreshBill				,       //充值成功，账号服务器通知游戏服务器刷新bill信息
	MSG_A2S_ChargeNotify            ,       //在线玩家充值
	MSG_SERVER_ChargeByAccountID    ,       //不在线玩家充值
	MSG_A2S_SyncChargeSetting,
	MSG_SERVER_ChargeByPUID         ,       //玩家PUID充值
	MSG_A2S_ChargeReward			,		//充值奖励，包括首充奖励和普通充值奖励
	MSG_A2S_FixedChargeReward		,		// 固定金额首冲		
    MSG_A2S_RedEnvelope             ,       // 舞团红包

	MSG_G2S_Announce				,		//公告
	MSG_G2S_AddMCoinByGm			,		//通过GMServer加M币
	MSG_S2A_AddMCoinByGm			,

	MSG_S2A_AddMCoinByCmd			,		//gameserver通知accountserver通过cmd添加m币
	MSG_G2S_LockOrUnLockAccountByGm ,		//锁定或者解锁账号
	MSG_S2A_LockOrUnLockAccountByGm ,		//锁定或者解锁账号

	MSG_G2S_SyncCompensation,
	MSG_G2S_NewCompensation,

	MSG_S2A_AddPtByFirstChargeReward,		// Gameser to Account 根据首充配置加 pt
	MSG_S2A_AddPtByRechargeReward,			// Gameser to Account 根据 充值配置加 pt

	MSG_G2S_GetRoomListByGM,				// GM查看房间
	MSG_S2G_GetRoomListByGMRes,				// GM查看房间

	MSG_S2S_Forward,

	MSG_A2S_SendPrivMCoin,					//发送M币（不记录log）

    MSG_S2G_GameServerClosed,               // GameServer关闭

    MSG_S2G_GameServerLinkInfo,             // 传递Game Server连接信息
    MSG_G2S_GameServerLinkInfoResult,

    MSG_PlayerCacheTransferStart,
    MSG_PlayerCacheTransfer,
    MSG_PlayerCacheTransferEnd,
    MSG_S2A_RequestPlayerCache,
    MSG_A2S_RequestPlayerCacheRes,

    MSG_S2A_VerifyAccount,
    MSG_A2S_VerifyAccountRes,
};

//plsyer attr
enum eMSG_PLAYER	//添加消息ID务必在最后面添加
{
	MSG_PLAYER_MSGBEGIN = MSG_TYPE_PLAYER,

	MSG_C2S_CreateRole,				//创建角色
	MSG_S2C_CreateRoleSuc,			//此消息Gate需要使用,需保证数值一致性
	MSG_S2C_CreateRoleFail,

	MSG_C2S_SwitchLine,				// 客户端到源服务器请求
	MSG_S2C_SwitchLineRes,			// 客户端切线结果。客户端只需要监听该消息。

	MSG_Role_CheckRoleName,			//检查角色名称是否重复
	MSG_Role_CheckRoleNameResult,	//

	MSG_Role_CreateQuestList,		//创建任务列表（原来的创建角色消息分成两部分，中间插上动态任务的处理）
	MSG_S2C_SwapItem,
	MSG_C2S_IncItemLevel,
	MSG_S2C_RefreshItem,
    MSG_S2C_BatchRefreshItem,
//	MSG_S2C_RefreshItemForOther,
	MSG_S2C_UseItemFailed,			//刷新多个物品
	MSG_C2S_UseItem,				//客户端发出使用物品的动作，双击装备也发这个消息
	MSG_C2S_BatchUseItem,			//批量使用物品
	MSG_S2C_PatchUseItemEffect,     //批量使用物品效果提示
	MSG_C2S_PickItem,				//客户端发出捡起物品的动作
	MSG_S2C_PickItem,				//server....
	MSG_C2S_SwapItem,
	MSG_C2S_RemoveItem,
	MSG_S2C_UpdateCoolDown,			//更新冷却时间
	MSG_C2S_GetCoolDown,			//获取冷却时间
	MSG_S2C_GetCoolDown,			//返回冷却时间
	MSG_C2S_ItemSend,				//物品赠送
	MSG_S2C_ItemSendResult,			
	MSG_C2S_EquipOperate,			//装备的操作，包括穿和卸
	MSG_C2S_ItemColumn,
	MSG_S2C_ItemColumn,				//物品栏扩展
	MSG_C2S_GetRoleInfo,			//请求获得玩家信息
	MSG_S2C_GetRoleInfo,			//返回玩家信息
    MSG_C2S_TouristSignInfo,        //记录玩家是否是游客

	//更新客户端数据，这里都是同步客户端数据
	MSG_S2C_RefreshPt,
	MSG_S2C_RefreshChange,
	MSG_S2C_RefreshLuk,
	MSG_S2C_RefreshPre,
	MSG_S2C_RefreshInt,
	MSG_S2C_RefreshAPt,
	MSG_S2C_RefreshMoney,
	MSG_S2C_RefreshExp,
	MSG_S2C_RefreshLevel,
	MSG_S2C_RefreshAPoint,
	MSG_S2C_RefreshMoneyGain,
	MSG_S2C_RefreshExpGain,
	MSG_S2C_RefreshPreGain,
	MSG_S2C_RefreshIgnoreMis,
	MSG_S2C_RefreshVip,
	MSG_S2C_RefreshChatColor,
	MSG_S2C_RefreshBagCapacity,
	MSG_S2C_RefreshTransform,
//	MSG_S2C_AddGene,
//	MSG_S2C_DelGene,
//	MSG_S2C_ResetGene,

    MSG_S2C_RefreshPerpetualEquipCount, // 更新永久服饰总数
    MSG_S2C_RefreshPerpetualEffectScore,// 更新光效闪亮值

    MSG_S2C_SerializeItemInfo,          // 同步物品信息

	MSG_TITLE_SET,
	MSG_TITLE_QUERYLIST,

	MSG_C2S_RequireChongzhiItem,
	MSG_S2C_ChongzhiItemInfo,
	MSG_S2C_RefreshChongzhiItemInfo,

	MSG_C2S_CheckRoleIsExist,			//查询账号在游戏中存不存在
	MSG_S2C_CheckRoleIsExist,

	MSG_S2C_RefreshMusic,				//添加了或者删除了某音乐
	MSG_S2C_RefreshScene,				//添加了或者删除了某场景

	MSG_C2S_UpdateSex,
	MSG_S2C_UpdateSex,
	MSG_C2S_UpdateRoleName,
	MSG_S2C_UpdateRoleName,
	MSG_S2C_FreshRoleName,
	MSG_S2C_DropItemError,				//丢弃物品的结果
	MSG_C2S_RoleLottery,				//角色抽奖

	MSG_S2C_RequireCreateRole,			//告知客户端需要创建角色

	MSG_C2S_ChangeRoleName,				// 改变角色昵称
//	MSG_S2C_ChangeRoleNameResult,

	MSG_C2S_ChangeRoleSignature,		// 修改签名
	MSG_S2C_ChangeRoleSignatureResult,	// 
	MSG_S2C_RefreshHot,

	MSG_C2S_ChangeEquipItem,
	MSG_S2C_ChangeEquipItem,

	MSG_C2S_ItemAward,
	MSG_S2C_ItemAwardSuc,
	MSG_S2C_ItemAwardFail,

	// 首次充值奖励
	MSG_S2C_FinishedFirstPayment,			// 完成首次充值
	MSG_C2S_GetFirstPaymentRewards,
	MSG_S2C_GetFirstPaymentRewardsRes,
	MSG_S2C_FirstPaymentStateChange,		// 服务器 返回 日常充值奖励 是否开放
    MSG_C2S_FixedchargeReq,                 // 请求定额首冲信息
    MSG_S2C_FixedchargeComplete,            // 定额首冲完成通知


	MSG_C2S_GetAnnoucement,					// 获取公告
	MSG_S2C_UpdateAnnouncement,				// 更新公告
	MSG_S2C_AnnoucementUpdated,				// 公告已是最新的

	MSG_C2S_EnterZone,						// 进去某个位置，比如大厅，商城，房间

	MSG_C2S_CompletedTeachGuide,
	MSG_S2C_CompletedTeachGuideRes,

	MSG_C2S_CompletedProcGuide,				
	MSG_C2S_CompletedProcGuideRes,				

	MSG_S2C_SendPlayerSetting,
	MSG_C2S_OptionSetting,
	MSG_S2C_OptionSettingSuc,
	MSG_S2C_OptionSettingFail,

	MSG_S2C_SimulateRandomItem,

	MSG_C2S_PlayerMoveTo,
	MSG_S2C_PlayerMoveTo,
	MSG_C2S_PlayerMoveType,
	MSG_S2C_PlayerMoveType,
	MSG_C2S_PlayerMotion,
	MSG_S2C_PlayerMotion,

	
	MSG_S2C_OpenBoxResult,
	MSG_S2C_OpenPackageResult,

	MSG_S2C_ChangeEquipItemFail,
	
 

	MSG_S2C_SynExtendItemBinData,

	MSG_S2C_AntiAddictionTip,
	MSG_C2S_SynAdult,						//同步是否成年
	
	MSG_C2S_SetSecurityQuiz,
	MSG_S2C_SetSecurityQuizSuc,
	MSG_S2C_SetSecurityQuizFail,
	
	MSG_C2S_BugReport,						// 错误报告

	MSG_S2C_GetClothGroups,					//返回所有服饰搭配
	MSG_C2S_SelectClothGroup,				//选择服饰搭配
	MSG_S2C_SelectClothGroup,				//选择服饰搭配结果
	MSG_C2S_UpdateClothGroup,				//更新服饰搭配
	MSG_S2C_UpdateClothGroup,				//更新服饰搭配结果
	MSG_C2S_UpdateClothGroupName,			//更新服饰搭配名字
	MSG_S2C_UpdateClothGroupName,			//更新服饰搭配名字结果

	MSG_C2S_SelectGuideFace,				
	MSG_S2C_SelectGuideFaceRet,
	MSG_C2S_CompleteAnimation,
	MSG_S2C_CompleteAnimationRet,

	// 日常充值奖励
	MSG_C2S_RechargeRewardEnable,			// 客户端 询问 日常充值奖励 是否开放
	MSG_S2C_RechargeRewardsEnableSuc,		// 服务器 返回 日常充值奖励 是否开放

	MSG_S2C_GetRechargeRewardsRulesSuc,		// 服务器响应 充值奖励规则
	MSG_C2S_GetRechargeRewardsRules,		// 客户端请求 充值奖励规则

	MSG_S2C_SendRechargeRewardFinished,		// 奖励 发送 完成

	MSG_C2S_UpdateLBSPositon,
	MSG_C2S_GetNearbyPlayers,
	MSG_S2C_NearbyPlayers,

	MSG_C2S_GetAllBuff,						// 查询所有buff
	MSG_S2C_GetAllBuff,						// 返回所有buff
	MSG_C2S_UnApplyBuff,					// 解除buff
	MSG_S2C_UnApplyBuff,					// 解除buff返回
	MSG_S2C_RefreshBindPt,					// 刷新绑定M币

	MSG_C2S_ChangeBirthday,					// 更改生日
	MSG_S2C_ChangeBirthdayRes,

	MSG_C2S_ReciveGuideExperienceCard,		// 引导中领取体验卡
	MSG_S2C_ReciveGuideExperienceCardRes,	// 引导中领取体验卡回复

    MSG_C2S_ReplaceClothEffect,             // 特效替换
    MSG_S2C_ReplaceClothEffectFail,         // 特效替换失败
    MSG_S2C_ReplaceClothEffectResult,       // 特效替换结果
    MSG_C2S_LevelUpEffect,                  // 升级特效
    MSG_S2C_LevelUpEffectFail,              // 升级特效不能执行
    MSG_S2C_LevelUpEffectResult,            // 升级特效结果
//    MSG_S2C_RefreshEffectChangeToOthers,    // 广播衣服特效变化
    MSG_S2C_RefreshHandbookProgress,        // 刷新图鉴进度
    MSG_S2C_RefreshColorProgress,           // 刷新七色进度
//    MSG_S2C_RefreshColorProgressToOthers,   // 广播七色进度

    MSG_C2S_PlayerDownloading,              // 玩家下载状态同步
    MSG_S2C_PlayerDownloading,

    // 魔法幸运阵
	MSG_C2S_RequestMagicArrayConfig,        // 请求配置信息
	MSG_S2C_RequestMagicArrayConfigResult,  // 请求配置信息(结果)
    MSG_C2S_PreviewAllReward,               // 预览产出
    MSG_S2C_PreviewAllRewardResult,         // 预览产出(结果)
    MSG_C2S_RequestExchangeConfig,          // 请求兑换物品列表
    MSG_C2S_RefreshExchangeItem,            // 刷新兑换物品列表
    MSG_S2C_RequestExchangeConfigResult,    // 请求兑换物品列表(结果)
    MSG_S2C_RequestExchangeConfigFail,      // 请求兑换物品列表(失败)
    MSG_C2S_RequestExchange,                // 请求兑换物品
    MSG_S2C_RequestExchangeResult,          // 请求兑换物品(结果)
    MSG_C2S_MagicCall,                      // 召唤
    MSG_S2C_MagicCallResult,                // 召唤结果
    MSG_S2C_NoticeCanFreeCall,              // 通知可以免费召唤
    


    MSG_C2S_MoveItem,                       // 移动物品到背包或者仓库
    MSG_S2C_MoveItemFail,                   // 移动物品到背包或者仓库(失败)
    MSG_S2C_MoveItemScuess,                 // 移动物品到背包或者仓库(成功)
    MSG_C2S_AddStorageCapacity,             // 增加仓库容量
    MSG_S2C_AddStorageCapacityFail,         // 增加仓库容量(失败)
    MSG_S2C_AddStorageCapacityScuess,       // 增加仓库容量（成功）
    MSG_S2C_SyncBagConfig,                  // 同步背包配置
    MSG_S2C_NoticeStorageItemUsed,          // 提示仓库物品被使用
    MSG_S2C_NoticeItemInStorage,            // 提示物品进入仓库
    MSG_C2S_RequestGuideFinish,             // 仓库引导完成
    MSG_S2C_RequestGuideFinishResult,       // 仓库引导完成(结果)

	/* 与GroupServer相关 */
	MSG_S2G_Login,							// GameServer告知GroupServer玩家登入
	MSG_S2G_Logout,							// GameServer告知GroupServer玩家登出
	MSG_S2G_UpdateAttrInfo,					// 更新玩家属性
	MSG_S2G_UpdateRoomInfo,					// 更新房间信息
	MSG_S2G_UpdateRoleName,
	MSG_S2G_UpdateRoleLevel,

	MSG_S2G_UpdateRoleLocation,
	MSG_S2G_UpdateRoleMoney,
	MSG_S2G_GetPlayerInfo_Forward,
	MSG_G2S_GetPlayerInfo_Forward,
	MSG_S2G_GetPlayerInfo_Backward,
	MSG_G2S_GetPlayerInfo_Backward,
    MSG_S2G_GetOfflinePlayerInfo,
    MSG_G2S_GetOfflinePlayerInfo,
	MSG_S2G_WishComeTrue,
	MSG_G2S_WishComeTrue,

	MSG_S2G_UpdateLBSPositon,
	MSG_S2G_GetNearbyPlayers,
	MSG_G2S_NearbyPlayers,
	MSG_S2G_CloseLBS,

    MSG_S2G_UpdateRoleAge,                  // 更新年龄MSG_C2S_ChangeBirthday

    // AccountServer相关
    MSG_A2S_ValidChargeDevice,              // 是否是合法的充值设备/充值设备是否在黑名单中
};

//room
enum eMSG_ROOM
{
	//room
	Room_Msg_BeginID = MSG_TYPE_ROOM,

	MSG_C2S_EnterLobby,
	MSG_C2S_ExitLobby,

	MSG_C2S_GetRoomList,
	MSG_S2C_GetRoomListRes,

	MSG_C2S_CreateRoom,
	MSG_S2C_CreateRoomSuc,
	MSG_S2C_CreateRoomFail,

	MSG_C2S_TryEnterRoom,
	MSG_S2C_RoomRequirePwd,

	MSG_C2S_EnterRoom,
	MSG_S2C_EnterRoomFail,

	MSG_C2S_ApplyMatch,
	MSG_C2S_CancelMatch,
	MSG_S2C_PrepareMatch,

	MSG_C2S_QuitRoom,
	MSG_S2C_QuitRoomSuc,
	MSG_S2C_QuitRoomFail,

	MSG_C2S_StartRoom,
	MSG_C2S_ReadyRoom,
	MSG_S2C_PrepareRoom,
	MSG_S2C_StartRoomSuc,
	MSG_S2C_StartRoomFail,

	MSG_C2S_EndRoom,
	MSG_S2C_EndRoomSuc,

	MSG_C2S_KickPlayer,
	MSG_S2C_KickPlayerFail,

	MSG_S2C_EnterPlayer,
	MSG_S2C_QuitPlayer,
	MSG_S2C_QuitMany,

	MSG_C2S_GetInviteeList,
	MSG_S2C_GetInviteeListSuc,
	MSG_S2C_GetInviteeListFail,

	MSG_C2S_InvitePlayer,
	MSG_S2C_InvitePlayerSuc,
	MSG_S2C_InvitePlayerFail,

	MSG_C2S_InviteeResponse,
	MSG_S2C_InviteeFail,
	MSG_S2C_InviteeNotice,

	MSG_C2S_InEquip,
	MSG_C2S_OutEquip,

	MSG_C2S_ChangeRoomPosState,
	MSG_S2C_ChangeRoomPosStateSuc,
	MSG_S2C_ChangeRoomPosStateFail,

	MSG_C2S_ChangeRoomInfo,
	MSG_S2C_ChangeRoomInfoSuc,
	MSG_S2C_ChangeRoomInfoFail,

	MSG_C2S_ChangeMusicInfo,
	MSG_S2C_ChangeMusicInfoSuc,
	MSG_S2C_ChangeMusicInfoFail,

	MSG_C2S_ChangeRoleRoomType,
	MSG_S2C_ChangeRoleRoomTypeSuc,
	MSG_S2C_ChangeRoleRoomTypeFail,

	MSG_C2S_ChangeRoleRoomState,
	MSG_S2C_ChangeRoleRoomStateSuc,
	MSG_S2C_ChangeRoleRoomStateFail,

	MSG_C2S_ReportRoundMark,
	MSG_S2C_SyncRoundMark,

	MSG_C2S_QuitMarkAward,

	MSG_C2S_PromoteRoomHost,
	MSG_S2C_PromoteRoomHostFail,
	MSG_S2C_PromoteRoomHostSuc,

	MSG_C2S_ReportEffectChange,
	MSG_S2C_SyncEffectState,
	
	MSG_C2S_LoadingStartRoomProgress,
	MSG_S2C_LoadingStartRoomProgress,

	MSG_S2C_RoomReward,

	MSG_C2S_SwitchDanceRoomTeamMode,
	MSG_S2C_SwitchDanceRoomTeamModeSuc,
	MSG_S2C_SwitchDanceRoomTeamModeFail,

	MSG_C2S_JoinDanceRoomTeam,
	MSG_S2C_JoinDanceRoomTeamSuc,
	MSG_S2C_JoinDanceRoomTeamFail,

	MSG_S2C_UpdateRoleDanceTeamInfo,		//个人组队信息修改:譬如加入别的队伍
	MSG_S2C_UpdateWholeDanceRoomTeamInfo,	//更新整个房间组队信息

	MSG_S2C_BeginToSelectPartner,
	MSG_S2C_EndToSelectPartner,
	MSG_S2C_SelectPartner,
	MSG_S2C_SelectPartnerFail,
	MSG_C2S_SelectPartner,
	MSG_S2C_UpdateSweetValue,

	MSG_S2C_ChangeRoomColor,

	Room_Msg_EndID
};

//live room
enum eMSG_LIVEROOM
{
	//room
	LiveRoom_Msg_BeginID = MSG_TYPE_LIVEROOM,
	
	MSG_C2S_EnterLiveLobby,
	MSG_S2C_EnterLiveLobbyRes,

	MSG_C2S_ExitLiveLobby,

	MSG_C2S_GetLiveRoomList,
	MSG_S2C_GetLiveRoomListRes,

	MSG_C2S_CreateLiveRoom,
	MSG_S2C_CreateLiveRoomSuc,
	MSG_S2C_CreateLiveRoomFail,

	MSG_C2S_EnterLiveRoom,
	MSG_S2C_EnterLiveRoomFail,
	MSG_S2C_LiveRoomEnterPlayer,
	MSG_S2C_LiveRoomEnterAudience,

	MSG_C2S_QuitLiveRoom,
	MSG_S2C_QuitLiveRoomSuc,
	MSG_S2C_QuitLiveRoomFail,

	MSG_C2S_CloseLiveRoom,
	MSG_S2C_CloseLiveRoomSuc,
	MSG_S2C_CloseLiveRoomFail,

	MSG_C2S_SetLiveRoomDefaultCloth,
	MSG_S2C_SetLiveRoomDefaultClothRes,

	MSG_C2S_GetLiveRoomScore,
	MSG_S2C_GetLiveRoomScoreRes,

	MSG_C2S_UpdateLiveRoomDancer,
	MSG_S2C_UpdateLiveRoomDancerSuc,
	MSG_S2C_UpdateLiveRoomDancerFail,

	MSG_C2S_SetLiveRoomDancerToAudience,
	MSG_S2C_SetLiveRoomDancerToAudienceFail,

	//////////////////////////////////////////////////////////////////////////
	MSG_C2S_OpenVote,
	MSG_S2C_OpenVoteSuc,
	MSG_S2C_OpenVoteFail,

	MSG_C2S_GetLiveRoomHotList,
	MSG_S2C_GetLiveRoomHotListRes,

	MSG_C2S_GetLiveRoomAuditionScoreList,
	MSG_S2C_GetLiveRoomAuditionScoreListRes,

	MSG_C2S_Vote,
	MSG_S2C_VoteSuc,
	MSG_S2C_VoteFail,

	MSG_S2C_DancerVoteInfo,	// 比赛房间所有玩家热度值

	MSG_C2S_GetMyAuditionScore,
	MSG_S2C_GetMyAuditionScore,

	MSG_C2S_LiveRoomJoinAudition,
	MSG_S2C_LiveRoomJoinAuditionFail,

	MSG_C2S_LiveRoomCancelAudition,
	MSG_S2C_LiveRoomPrepareAudition,

	MSG_S2C_SyncMyAuditionScore, // 海选单场得分

	// game server <=> group server
	MSG_S2G_LiveRoom_UpdateDancerHot,
	MSG_S2G_LiveRoom_UpdateDancerPoint,


    MSG_S2G_LiveRoom_AddPunishedPlayer,
    MSG_S2G_LiveRoom_IsPunishedPlayer,
    MSG_G2S_LiveRoom_IsPunishedPlayer,
    MSG_S2G_LiveRoom_SyncAuditionScore,
    MSG_G2S_LiveRoom_SyncAuditionScore,


};

enum EMSG_NewRoom
{
    MSG_C2S_NewCreateRoom = MSG_TYPE_NEWROOM + 1,
    MSG_C2S_NewTryEnterRoom,
    MSG_C2S_NewQuitRoom,
    MSG_C2S_NewPromoteRoomHost,
    MSG_C2S_NewKickPlayer,
    MSG_C2S_NewChangeRoleRoomState,
    MSG_C2S_NewChangeRoomPosState,
    MSG_S2C_NewCreateRoomSuc,

    MSG_S2G_TransFromIDUpdate,
    MSG_S2G_OnEquipNotify,
    MSG_S2G_UnEquipNotify,
    MSG_S2G_EffectChangeNotify,
    MSG_S2G_RefreshSevenColor,
    MSG_S2G_RefreshGeneNotify,
    MSG_S2G_AddMedals,
};


enum EMSG_RoomBroadCast
{
    MSG_S2C_RefreshItemForOtherNew = MSG_TYPE_NEWROOMBROADCAST + 1,
    MSG_S2C_RefreshEffectChangeToOthersNew,
    MSG_S2C_RefreshColorProgressToOthersNew,
    MSG_S2C_ChangeRoleNameResultNew,
    MSG_S2C_NotifyRoleVIPInfoChangedNew,
    MSG_S2C_UpdateDanceGroupNameNew,
    MSG_S2C_UpdateDanceGroupTitleNew,
    MSG_S2C_ChangeDanceGroupBadgeOrEffectResultNew,
    MSG_S2C_SyncMedalInfoToOthersNew,
    MSG_S2C_SYNC_HorseDataNew,
    MSG_S2C_ResetGeneNew,
    MSG_S2C_DelGeneNew,
    MSG_S2C_AddGeneNew,

};

//super market
enum eMSG_MALL
{
    MALL_MSG_BEGINID = MSG_TYPE_MALL,

    MSG_C2S_MALL_GETGOODS,
    MSG_C2S_MALL_GETSOMEGOODS,
    MSG_C2S_MALL_BUY,
    MSG_C2S_MALL_SEND,
    MSG_C2S_MALL_ASKFOR,
    
    MSG_C2S_MALL_RENEW,					// 续费
    MSG_C2S_MALL_SENDWISHITEM,			// 赠送愿望物品

    MSG_S2C_MALL_GETGOODSRESULT,
    MSG_S2C_MALL_GETSOMEGOODSRESULT,
    MSG_S2C_MALL_BUYRESULT,
    MSG_S2C_MALL_SENDRESULT,
    MSG_S2C_MALL_ASKFORRESULT,
    MSG_S2C_MALL_BEASKED,
    MSG_S2C_MALL_RENEWRESULT,
    MSG_S2C_MALL_SENDWISHITEM_SUC,
    MSG_S2C_MALL_SENDWISHITEM_FAIL,

    MSG_C2S_MALL_BEASKED_SEND,			// 索要赠送
    MSG_S2C_MALL_BEASKED_SENDRESULT,

    MSG_C2S_MALL_EnterMall,
    MSG_S2C_MALL_EnterMallRes,

    MSG_S2C_MALL_AllGoodsInfo,
    MSG_C2S_MALLAction_EnterMall,       // 进入商城


    // special mall
    MSG_C2S_SpecialMall_State,
    MSG_S2C_SpecialMall_StateResult,

    // group buying
    MSG_C2S_GroupBuying_Items,
    MSG_S2C_GroupBuying_ItemsResult,
    MSG_C2S_GroupBuying_Buy,
    MSG_S2C_GroupBuying_BuyResult,

    // limited edition
    MSG_C2S_LimitedEdition_Items,
    MSG_S2C_LimitedEdition_ItemsResult,
    MSG_C2S_LimitedEdition_Buy,
    MSG_S2C_LimitedEdition_BuyResult,

    // auction
    MSG_C2S_Auction_Items,
    MSG_S2C_Auction_ItemsResult,
    MSG_C2S_Auction_Bid,
    MSG_S2C_Auction_BidResult,
    MSG_S2C_Auction_Surpassed,
    MSG_S2C_Auction_BidSuccess,

    // experience card info 
    MSG_C2S_MallExperienceCardInfo,
    MSG_S2C_MallExperienceCardInfoRes,
    MSG_S2C_MallExperienceCardInfoNtf,

    // desire
    MSG_C2S_Desire,
    MSG_S2C_DesireResult,
    MSG_C2S_RemoveDesire,			// remove wish item
    MSG_S2C_RemoveDesireFail,
    MSG_S2C_RemoveDesireScuess,
    MSG_C2S_RequestDesireList,
    MSG_S2C_RequestDesireListResult,


	MSG_S2S_MALL_ASKFOR,
	MSG_S2G_MALLDESIRE,
	MSG_G2S_MALLDESIRE,

    // special mall
    MSG_G2S_SpecialMall_Pay,
    MSG_S2G_SpecialMall_PayResult,
    MSG_G2S_Auction_UpdateState,
    MSG_G2S_Auction_BidSuccess,
    MSG_G2S_Auction_BidResult,
    MSG_G2S_Auction_Surpassed,
    MSG_G2S_Auction_ItemsResult,
    MSG_S2G_Auction_CheckTopPrice,
    MSG_G2S_Auction_CheckTopPriceResult,

    // mall reload
    MSG_G2S_Mall_Reload,

	MALL_MSG_ENDID
};

//MSG_TYPE_MAIL
enum eMSG_MAIL
{
	MAIL_MSG_BEGINID = MSG_TYPE_MAIL,

	MSG_C2S_GETMAILBOX,
	MSG_C2S_GETMAILINFO,
	MSG_C2S_SENDMAIL,
	MSG_C2S_DELETEMAIL,
	MSG_C2S_GETMAILITEM,

	MSG_S2C_GETMAILBOX,
	MSG_S2C_GETMAILINFO,
	MSG_S2C_SENDMAIL,
	MSG_S2C_DELETEMAIL,
	MSG_S2C_GETMAILITEM,
	MSG_S2C_RECEIVEMAIL,
	MSG_S2C_UNREADMAIL,


    Msg_S2C_ReceiveStrangeMailFlag,
    Msg_C2S_SetReceiveStrangeMailFlag,
    Msg_S2C_SetReceiveStrangeMailFlag, 

	MSG_S2G_SENDMAIL,
	MSG_G2S_SENDMAIL,
	MSG_A2S_SENDMAIL,

    MSG_S2G_ValidSendMail,
    MSG_G2S_ValidSendMail,

    // 占位，暂不用
    Msg_C2S_CanSendMail,
    Msg_S2C_CanSendMail,
    Msg_C2S_ReceiveStrangeMailFlag,
    MAIL_MSG_ENDID
};

enum eMSG_QUEST
{
	MSG_QUEST_BEGIN = MSG_TYPE_QUEST,

	// msg for quests   
	MSG_C2S_GetQuestReward,
	MSG_C2S_GetQuestList,
	MSG_C2S_QuestRead,
	MSG_C2S_AcceptQuest,

	MSG_S2C_AutoPushQuest,
	MSG_S2C_QuestStateChanged,
	MSG_S2C_GetQuestListResult,
	MSG_S2C_GetQuestRewardResult,
	MSG_S2C_AcceptQuestResult,
	MSG_S2C_SendQuestStaticBinData,
	MSG_S2C_UnRewardQuest,

	MSG_C2S_GetFresherQuestList,
	MSG_S2C_SendFresherQuest,
	MSG_C2S_GetFresherQuestReward,
	MSG_S2C_GetFresherQuestRewardSuc,
	MSG_S2C_GetFresherQuestRewardFail,
	MSG_S2C_UnRewardFresherQuest,
    MSG_S2C_SendQuestNewStaticBinData,
	MSG_C2S_ClientCompleteQuest,
	MSG_C2S_GetQuestState,
	MSG_S2C_GetQuestStateRes,
};


//MSG for friend
enum eMSG_FRIEND
{
	MSG_FRIEND_BEGIN = MSG_TYPE_FRIEND,
	MSG_C2S_FriendApplication,				// 申请成为好友
	MSG_C2S_AddBlackFriend,					// 添加黑名单好友(从黑名单拉至好友列表)
	MSG_C2S_AcceptApplication,				// 接受好友申请
	MSG_C2S_RefuseApplication,				// 拒绝好友申请
	MSG_C2S_AddBlackList,					// 添加黑名单
	MSG_C2S_RemoveBlackList,				// 删除黑名单
	MSG_C2S_RemoveFriend,					// 删除好友
	MSG_C2S_GetBlackListOnlineInfo,			// 获得好友在线信息
	MSG_C2S_GetTempListPlayerOnlineInfo,	// 最近玩伴、最近聊天的列表玩家信息
	MSG_C2S_FindPlayer,					    // 模糊搜索在线玩家
    MSG_C2S_ReworkFriendNotes,              // 修改好友备注

	MSG_S2C_FriendApplication,				// 通知客户端玩家申请成为好友

	MSG_S2C_FriendApplicationSuc,			// 发送申请成功
	MSG_S2C_FriendApplicationFail,			// 发送申请失败

	MSG_S2C_AddBlackFriendFail,				// 添加黑名单好友失败

	MSG_S2C_AcceptApplicationSuc,			// 同意添加好友消息成功返回
	MSG_S2C_AcceptApplicationFail,			// 同意添加好友消息失败返回

	MSG_S2C_RefuseApplicationSuc,			// 拒绝好友申请成功返回
	MSG_S2C_RefuseApplicationFail,			// 拒绝好友申请失败返回

	MSG_S2C_GetFriendOnlineInfoReuslt,		// 好友在线信息

	MSG_S2C_RemoveFriendSuc,				// 删除好友成功
	MSG_S2C_RemoveFriendFail,				// 删除好友失败

	MSG_S2C_AddBlackListSuc,				// 加入黑名单成功
	MSG_S2C_AddBlackListFail,				// 删除黑名单失败 

	MSG_S2C_RemoveBlackListSuc,				// 删除黑名单成功
	MSG_S2C_RemoveBlackListFail,			// 删除黑名单失败

	MSG_S2C_UpdateFriendInfo,				// 更新好友信息
	MSG_S2C_NotityHasBeenAccepted,			// 已经被添加为好友

	MSG_S2C_HasBeenRemovedFriend,			// 通知客户端好友被删除
	MSG_S2C_HasBeenRemovedBlackList,		// 通知客户端黑名单被删除

	MSG_S2C_GetTempListPlayerInfoSuc,		// 最近玩伴、最近聊天的列表玩家信息 	
	MSG_S2C_GetTempListPlayerInfoFail,		// 

	MSG_S2C_GetBlackListOnlineInfoSuc,		// 黑名单在线信息 
	MSG_S2C_GetBlackListOnlineInfoFail,		// 获取黑名单失败
	MSG_S2C_BlackList,						// 登陆返回好友黑名单

	MSG_S2C_FindPlayerResult,
	MSG_S2C_FindPlayerFail,

	MSG_S2C_MakeFriend,						// 系统询问是否要互加好友
	MSG_S2C_FindPlayerSetting,				// 查找玩家的条件描述信息

    MSG_C2S_GetRecommendFriends,            // 获得推荐好友
    MSG_S2C_GetRecommendFriendsResult,      // 获得推荐好友结果

	MSG_S2G_AddBlackListFriend,
	MSG_S2G_BecomeFriends,
	MSG_S2G_RemoveFriend,
	MSG_S2G_AddBlackList,
	MSG_S2G_RemoveBlackList,
	MSG_S2G_AddMuteList,
	MSG_S2G_RemoveMuteList,
	MSG_S2G_GetFriendOnlineInfo,
	MSG_S2G_FriendApplication,
	MSG_S2G_NotifyGameRemoveFriend,
	MSG_S2G_NotifyGameRemoveBlacklist,
	MSG_S2G_NotifyGameRemoveFollowedFriend,
	MSG_S2G_FriendLogin,
	MSG_S2G_NotityHasBeenAccepted,
	MSG_S2G_NotifyHasBeenDeletedFriend,
	MSG_S2G_NotifyGameHasBeenDeletedFriend,
	MSG_S2G_NotifyGameHasBeenDeletedBlacklist,
	MSG_S2G_GetBlackListOnlineInfo,
	MSG_S2G_GetTempListOnlineInfo,
	MSG_S2G_RequestFriendOnlineInfo,
	MSG_S2G_PushOnlineInfoToAllFriends,
	MSG_S2G_UpdateAttrbuteToAllFriends,
	MSG_S2G_FindPlayer,

	MSG_G2S_BecomeFriends,
	MSG_G2S_GetFriendOnlineInfoResult,
	MSG_G2S_FriendApplicationBack2Sender,
	MSG_G2S_FriendApplicationForward2Receiver,
	MSG_G2S_NotifyGameRemoveFollowedFriend,
	MSG_G2S_NotifyGameRemoveFriend,
	MSG_G2S_FriendLogin,
	MSG_G2S_GameServerInfoResult,   // not used any more, see MSG_S2G_GameServerLinkInfo, by myLike
	MSG_G2S_NotifiedHasBeenAccepted,
	MSG_G2S_NotifyGameHasBeenDeletedFriend,
	MSG_G2S_NotifyGameHasBeenDeletedBlacklist,
	MSG_G2S_FriendLogout,
	MSG_G2S_PushOnlineInfoToAllFriends,
	MSG_G2S_GetBlackListOnlineInfo,
	MSG_G2S_GetTempListOnlineInfo,
	MSG_G2S_UpdateAttrbuteToAllFriends,
	MSG_G2S_FindPlayerResult,

};

//MSG for rankinf
enum ROOM_MODE
{
	ROMM_MODE_TAIKO = 1,	//传统模式
	ROMM_MODE_RHYTHM,		//节奏模式

	ROMM_MODE_MAX
};

enum eMSG_ACHIEVEMENT
{
	MSG_ACHIEVEMENT_BEGIN = MSG_TYPE_ACHIEVEMENT,	

	// msg for achievement
	MSG_C2S_GetAchievementList,
	MSG_C2S_AchievementGetReward,

	MSG_S2C_AchievementCompleted,
	MSG_S2C_GetAchievementList,
	MSG_S2C_GetAchievementDetailList,
	MSG_S2C_AchievementGetRewardResult
};

enum eMSG_CheckIn
{
    MSG_CHECKIN_BEGIN =  MSG_TYPE_CHECKIN,
    MSG_C2S_GetCheckInInfo,
    MSG_C2S_Check,								// 签到

    MSG_S2C_GetCheckInInfoResult,
    MSG_S2C_CheckInSuccess,
    MSG_S2C_CheckResultFail,
    MSG_S2C_CheckInRewardMailNotice,            // 签到补发奖励邮件提示
};

enum eMSG_MonthCard
{
	MSG_MonthCard_BEGIN =  MSG_TYPE_MonthCard,
	MSG_C2S_GetMonthCardInfo,
	MSG_S2C_GetMonthCardInfoResult,

	MSG_C2S_GetMonthCardReward,
	MSG_S2C_GetMonthCardRewardSuc,
	MSG_S2C_GetMonthCardRewardFail,

	MSG_C2S_OpenMonthCard,
	MSG_S2C_OpenMonthCardSuc,
	MSG_S2C_OpenMonthCardFail,

	MSG_C2S_RenewMonthCard,
	MSG_S2C_RenewMonthCardSuc,
	MSG_S2C_RenewMonthCardFail,

};

enum eMSG_Activity
{
	MSG_ACTIVITY_BEGIN =  MSG_TYPE_ACTIVITY,
	MSG_S2C_InTimeOnlineReward,				//整点在线奖励

	MSG_C2S_GetOnlineReward,
	MSG_S2C_GetOnlineRewardSuc,
	MSG_S2C_GetOnlineRewardFail,

	MSG_S2C_GetLevelUpRewardInfoRes,

	MSG_S2C_UpgradeInstaller,

	MSG_S2C_UpdateExtraExpActivityState,	//额外经验奖励活动状态更新
	MSG_S2C_SendGift,						//直接发送礼品给玩家
	MSG_S2C_UpdateGiftInfo,					//礼品信息
	MSG_C2S_GetGift,						//获取礼物
	MSG_S2C_GetGiftResult,					//获取礼物反馈

	MSG_S2C_UpdateOnlineRewardInfo,			// online reward

	MSG_C2S_GetAllActivitiesInfo,
	MSG_S2C_GetAllActivitiesInfoSuc,
	MSG_S2C_GetAllActivitiesInfoFail,

	MSG_C2S_GetCDKeyInfo,
	MSG_S2C_GetCDKeyInfoSuc,

	MSG_C2S_GetBindActivationInfo,
	MSG_S2C_GetBindInfoSuc,
	

	MSG_S2C_UpdateCumulativeActivityState,
	MSG_S2C_UpdateCumulativeRechargeNum,
	MSG_C2S_GetCumulativeRechargeReward,
	MSG_S2C_GetCumulativeRechargeRewardSuc,
	MSG_S2C_GetCumulativeRechargeRewardFail,

    MSG_S2C_UpdateCumulativeSpendNum,
	//IOS专有活动
	MSG_C2S_GetProprietaryReward,
	MSG_S2C_GetProprietaryRewardResult,

	MSG_C2S_GetCumulativeSpendReward,
	MSG_S2C_GetCumulativeSpendRewardSuc,
	MSG_S2C_GetCumulativeSpendRewardFail,

    MSG_C2S_GetCumulativeSpendGotMedalReward,
    MSG_S2C_GetCumulativeSpendGotMedalRewardSuc,
    MSG_S2C_GetCumulativeSpendGotMedalRewardFail,

	MSG_C2S_ExchangeItem,
	MSG_S2C_ExchangeItemSuc,
	MSG_S2C_ExchangeItemFail,

	//获取圣诞免费活动信息
	MSG_C2S_GetXmasAcitivyInfo,
	MSG_S2C_GetXmasActivityInfoSuc,

	//获取许愿活动信息
	MSG_C2S_GetFestivalWishInfo,
	MSG_S2C_GetFestivalWishInfoSuc,

	//获得祝福活动信息
	MSG_C2S_GetFestivalBlessActivityInfo,
	MSG_S2C_GetFestivalBlessInfoSuc,

	//获取礼包
	MSG_C2S_GetXmaxFreeReward,
	MSG_S2C_GetXmaxFreeRewardSuc,
	MSG_S2C_GetXmaxFreeRewardFail,

	//许愿
	MSG_C2S_FestivalWish,
	MSG_S2C_FestivalWishSuc,
	MSG_S2C_FestivalWishFail,

	//累计祝福奖励
	MSG_C2S_GetFestivalBlessReward,
	MSG_S2C_GetFestivalBlessRewardSuc,
	MSG_S2C_GetFestivalBlessRewardFail,

	//祝福好友
	MSG_C2S_BlessFriend,
	MSG_S2C_BlessFriendSuc,
	MSG_S2C_BlessFriendFail,

	MSG_S2C_GetBlessFromFriend,

	//拼图活动
	MSG_S2C_SyncPuzzleData,

	MSG_C2S_FillPuzzle,
	MSG_S2C_FillPuzzleSuc,
	MSG_S2C_FillPuzzleFail,

	//新玩家招募活动
	MSG_C2S_BindInvitationCode,				//被邀请者绑定邀请码并领取奖励
	MSG_S2C_BindInvitationCodeSuc,
	MSG_S2C_BindInvitationCodeFail,
	
	MSG_C2S_GetInvitationInviterInfo,		//邀请者查询活动信息, 自己的邀请码和邀请人数
	MSG_S2C_GetInvitationInviterInfoSuc,
	MSG_S2C_GetInvitationInviterInfoFail,
	
	MSG_C2S_GetInvitationInviterReward,		//邀请者获取奖励
	MSG_S2C_GetInvitationInviterRewardSuc,
	MSG_S2C_GetInvitationInviterRewardFail,

	MSG_C2S_GetFresherReward,				//新手七天乐活动
	MSG_S2C_GetFresherRewardSuc,
	MSG_S2C_GetFresherRewardFail,
	MSG_S2C_GetFresherAllReward,

	MSG_C2S_GetExtraActivityInfo,
	MSG_S2C_GetExtraActivityInfoSuc,

	MSG_S2C_CompensationData,				//通知补偿
	MSG_C2S_GetCompensation,				//领取补偿
	MSG_S2C_GetCompensationSuc,				//领取补偿成功
	MSG_S2C_GetCompensationFail,			//领取补偿失败

	// 邂逅
	MSG_C2S_MeetMatch,          		    //匹配
	MSG_S2C_MeetMatchSucceed, 				//匹配成功
	MSG_S2C_MeetMatchFaild, 				//匹配失败
	MSG_C2S_MeetCancelMatch,                //取消匹配
	MSG_C2S_MeetLeaveScene,                 //离开活动场景
	MSG_C2S_MeetCameraEvent,                //照相
	MSG_S2C_MeetPhotoSucceed,               //照相成功
	MSG_S2C_MeetPhotoFaild,                 //照相失败
	MSG_C2S_MeetEnd,						//邂逅结束
	MSG_S2C_MeetEndResult,					//邂逅结果
	MSG_C2S_MeetEnterAmuseScene,            //进入开放场景

	//社交分享
	MSG_C2S_SocialShare,
	MSG_S2C_SocialShare,

	//社交分享活动
	MSG_C2S_ActivitySocialShare,
	MSG_S2C_ActivitySocialShare,

	MSG_S2C_CanSocialShareFirst,			//能否首次分享

    MSG_C2S_RequestVipExtraReward,          // 补发VIP奖励
    MSG_S2C_RequestVipExtraRewardRes,

    // 长效累冲
    MSG_C2S_GetLongactingCumulativeRecharge,              //获取长效累冲信息
    MSG_S2C_GetLongactingCumulativeRechargeResult,        //获取长效累冲信息（返回）
    MSG_C2S_GetLongactingCumulativeRechargeBroadcase,     //获取长效累冲广播
    MSG_S2C_GetLongactingCumulativeRechargeBCResult,      //获取长效累冲广播（返回）
    MSG_C2S_GetLongactingCumulativeRechargeReward,        //获取长效累冲奖励
    MSG_S2C_GetLongactingCumulativeRechargeFail,          //获取长效累冲奖励（失败）
    MSG_S2C_GetLongactingCumulativeRechargeSucess,        //获取长效累冲奖励（返回）

    MSG_C2S_RequestBuyItemGotMedalReward,
    MSG_S2C_RequestBuyItemGotMedalRewardSuc,
    MSG_S2C_RequestBuyItemGotMedalRewardFail,
    MSG_S2C_UpdateBuyItemGotMedalCurrentCount,

    // 舞团红包
    MSG_C2S_GetCanOpenRedEnevlopeCount,               // 获取可以领取红包数目
    MSG_S2C_UpdateCanOpenRedEnevlopeCount,            // 更新可以领取红包数目
    MSG_C2S_GetRedEnvelopeList,                    // 获取红包列表
    MSG_S2C_GetRedEnvelopeListResult,              // 获取红包列表(结果)
    MSG_C2S_GetRedEnvelopeDetailes,                // 获取红包详情
    MSG_S2C_GetRedEnvelopeDetailesFail,            // 获取红包详情(失败)
    MSG_S2C_GetRedEnvelopeDetailesResult,          // 获取红包详情(结果)
    MSG_C2S_OpenRedEnvelope,                       // 领取红包
    MSG_S2C_OpenRedEnvelopesResult,                // 领取红包(结果)
    MSG_C2S_SetAnonymity,                          // 设置匿名
    MSG_S2C_UpdateAnonymity,                       // 更新匿名
    MSG_S2C_NewRedEnvelopesNotice,                 // 新红包通知

	//S2S
	MSG_S2G_BlessFriend,
	MSG_G2S_BlessFriend,
    MSG_S2G_AddLongactingRechargeBroadCase,
    MSG_S2G_GetLongactingRechargeBroadCase,
    MSG_G2S_GetLongactingRechargeBroadCase,

    // 舞团红包
    MSG_S2G_CreateRedEnvelope,                      // 创建红包
    MSG_G2S_NewRedEnvelope,                         // 新红包向团员广播
    MSG_S2G_GetRedEnvelopeList,                     // 获取红包列表
    MSG_G2S_GetRedEnvelopeList,                     // 获取红包列表(返回)
    MSG_S2G_GetRedEnvelopeDetails,                  // 获取红包详细信息
    MSG_G2S_GetRedEnvelopeDetails,                  // 获取红包详细信息(返回)
    MSG_S2G_OpenRedEnvelope,                        // 领取红包
    MSG_G2S_OpenRedEnvelope,                        // 领取红包
    MSG_S2G_GetCanOpenRedEnvelopeCount,             // 获取能领取红包的个数
    MSG_G2S_GetCanOpenRedEnvelopeCount,             // 获取能领取红包的个数

};
////舞团消息
//enum eMSG_DanceGroup
//{
//	MSG_DANCEGROUP_BEGIN =  MSG_TYPE_DANCEGROUP,
//	MSG_C2S_CreateDanceGroup,				//创建舞团
//	MSG_S2C_CreateDanceGroupResult,			//创建舞团结果
//	MSG_C2S_RequestEnterDanceGroup,			//申请
//	MSG_S2C_ResponseEnterDanceGroup,		//申请回复
//};


enum eMSG_PlatformPrivilege
{
	MSG_PLATFORMPRIVILEGE_BEGIN = MSG_TYPE_PLATFORMPRIVILEGE,

	MSG_C2S_GetUCVIPRewardStatue, //获取UCVIP特权领取状态
	MSG_S2C_GetUCVIPRewardStatueSuc,
	MSG_S2C_GetUCVIPRewardStatueFail,

	MSG_C2S_GetUCVIPRewardDetail, //获取特权详细信息
	MSG_C2S_GetUCVIPRewardDetailSuc,
	MSG_C2S_GetUCVIPRewardDetailFail,

	MSG_C2S_GainUCVIPReward, //领取UCVIP特权
	MSG_S2C_GainUCVIPRewardSuc,
	MSG_S2C_GainUCVIPRewardFail
};

enum eMSG_VIP
{
    MSG_VIP_BEGIN = MSG_TYPE_VIP,

    MSG_C2S_GetVIPRewardInfo,
    MSG_C2S_GetActivatedVIPRewardInfo,
    MSG_C2S_ActivateVIP,
    MSG_C2S_GetDailyVIPReward,
    MSG_C2S_GetYearlyVIPReward,

    MSG_S2C_GetVIPRewardInfoResult,
    MSG_S2C_GetActivatedVIPRewardInfoResult,
    MSG_S2C_ActivateVIPResult,
    MSG_S2C_GetDailyVIPRewardResult,
    MSG_S2C_GetYearlyVIPRewardResult,

    MSG_S2C_UpdateVIPInfo,
    MSG_S2C_LevelUpReward,
    MSG_C2S_GetVIPDailyReward,
    MSG_S2C_GetVIPDailyRewardSuc,

//    MSG_S2C_NotifyRoleVIPInfoChanged,

    MSG_C2S_RequestBuyVip,
    MSG_S2C_RequestBuyVipRes,

    MSG_C2S_SendOpenVipFrom,
    MSG_S2C_GetOpenVipFrom,

    MSG_S2G_UpdateVIPInfo,
};

enum eMSG_AMUSE
{
	MSG_AMUSE_BEGIN = MSG_TYPE_AMUSEMENT,

	MSG_C2S_GetAmuseRoomList,
	MSG_S2C_GetAmuseRoomListRes,

	MSG_C2S_CreateAmuseRoom,
	MSG_S2C_CreateAmuseRoomSuc,
	MSG_S2C_CreateAmuseRoomFail,

	MSG_C2S_TryEnterAmuseRoom,
	MSG_S2C_AmuseRoomRequirePwd,

	MSG_C2S_EnterAmuseRoom,
	MSG_S2C_EnterAmuseRoomFail,

	MSG_C2S_QuitAmuseRoom,
	MSG_S2C_QuitAmuseRoomSuc,
	MSG_S2C_QuitAmuseRoomFail,

	MSG_C2S_ChangeAmuseRoomInfo,
	MSG_S2C_ChangeAmuseRoomInfoSuc,
	MSG_S2C_ChangeAmuseRoomInfoFail,

	MSG_C2S_AmuseKickPlayer,
	MSG_S2C_AmuseKickPlayerFail,

	MSG_S2C_AmuseEnterPlayer,
	MSG_S2C_AmuseQuitPlayer,

	MSG_C2S_AmusePromoteHost,
	MSG_S2C_AmusePromoteHostFail,
	MSG_S2C_AmusePromoteHostSuc,

	MSG_C2S_AmuseInvitePlayer,
	MSG_S2C_AmuseInvitePlayerSuc,
	MSG_S2C_AmuseInvitePlayerFail,

	MSG_C2S_AmuseInviteeResponse,
	MSG_S2C_AmuseInviteeFail,
	MSG_S2C_AmuseInviteeNotice,

	MSG_C2S_DoAmuseAction,
	MSG_S2C_DoAmuseActionSuc,
	MSG_S2C_DoAmuseActionFail,

	MSG_S2C_AmuseEventNotice,
	MSG_C2S_AmuseApplyMatch,

	//时光蛋
	MSG_C2S_QueryTimeEggReward,
	MSG_S2C_QueryTimeEggReward,
	MSG_S2C_SynRoleTimeEggInfo,

	MSG_S2C_GetConstellationMatchInfoRes,

	MSG_C2S_EnterConstellationLobby,
	MSG_S2C_EnterConstellationLobbySuc,
	MSG_S2C_EnterConstellationLobbyFail,

	MSG_C2S_QuitConstellationLobby,

	MSG_C2S_ConstellationMatch,
	MSG_S2C_ConstellationMatchSuc,
	MSG_S2C_ConstellationMatchFail,

	MSG_C2S_EnterConstellationRoom,

	MSG_C2S_SendQuestionAnswer,

	MSG_C2S_PlayMakerSync,
	MSG_S2C_PlayMakerBroadcast,

	//S2G
	MSG_S2G_CreateTimeEgg,
	MSG_S2G_OpenTimeEgg,
	MSG_S2G_TimeEggSetting,
	//G2S
	MSG_G2S_OpenTimeEgg,
};


enum eMSG_Notification
{
	MSG_NOTIFICATION_BEGIN = MSG_TYPE_NOTIFICATION,

	MSG_C2S_NoPushRatingNotification,

	MSG_S2C_PushRatingNotification,

	// 主动通知消息更新，增、改、删
	Msg_S2C_InfoCenterNotification,
	Msg_C2S_InfoCenterGetIDRes,
	Msg_S2C_InfoCenterGetIDRes,
	Msg_C2S_InfoCenterMarkReaded,

};

enum eMsg_Dungeon
{
	MSG_DUNGEON_BEGIN = MSG_TYPE_DUNGEON,

	MSG_C2S_GetLanternBaseInfo,                            // 获取神灯基本信息
	MSG_S2C_GetLanternBaseInfoResult,                      // 获取神灯基本信息反馈

    MSG_C2S_GetDungeonStory,                               // 获取剧情
	MSG_S2C_GetDungeonStoryResult,                         // 获取剧情结果
    MSG_S2C_GetDungeonStoryFail,                           // 获取剧情失败

	MSG_C2S_GetDungeonInfoDetail,                          // 获取副本详细信息
	MSG_S2C_GetDungeonInfoDetailResult,                    // 获取副本详细信息反馈
	MSG_S2C_GetDungeonInfoDetailFail,                      // 获取副本详细信息失败
    
	MSG_C2S_GetParcloseInfoDetail,                         // 获取关卡详细信息
	MSG_S2C_GetParcloseInfoDetailResult,                   // 获取关卡详细信息反馈
	MSG_S2C_GetParcloseInfoDetailFail,                     // 获取关卡详细信息失败

	MSG_C2S_GetStarReward,                                 // 领取幸运星奖励
	MSG_S2C_GetStarRewardSuc,                              // 领取幸运星成功
	MSG_S2C_GetStarRewardFail,                             // 领取幸运星失败

	MSG_C2S_ExchangeDustReward,                            // 搓神灯奖励
	MSG_S2C_ExchangeDustRewardSuc,                         // 搓神灯奖励成功
	MSG_S2C_ExchangeDustRewardFail,                        // 搓神灯奖励失败

    MSG_S2C_BeInviteByFriend,                              // 被好友邀请
	MSG_C2S_InviteFriendResponse,                          // 好友被邀请回馈
	MSG_S2C_InviteFriendResponseResult,                    // 好友被邀请回馈结果

	MSG_C2S_GetDungeonRankList,                            // 获取好友排行
	MSG_S2C_GetDungeonRankListResult,                      // 获取好友排行结果
	MSG_S2C_UpdateSelfRank,                                // 更新自己的排名

    MSG_C2S_GetBuyCrystalInfo,                             // 获取购买体力信息
    MSG_S2C_GetBuyCrystalInfoResult,                       // 获取购买体力信息结果
	MSG_C2S_BuyCrystal,                                    // 购买水晶
	MSG_S2C_BuyCrystalSuc,                                 // 购买水晶成功
	MSG_S2C_BuyCrystalFail,                                // 购买水晶失败
	MSG_S2C_UpdateCrystal,                                 // 更新水晶

	MSG_C2S_SkipParclose,                                  // 付钱跳过关卡
	MSG_S2C_SkipParcloseSuc,                               // 付钱跳过关卡成功
	MSG_S2C_SkipParcloseFail,                              // 付钱跳过关卡失败

	MSG_C2S_StartParclose,                                 // 开始关卡
	MSG_S2C_StartParcloseSuc,                              // 开始关卡成功
	MSG_S2C_StartParcloseFail,                             // 开始关卡失败

	MSG_S2C_ParcloseEnd,                                   // 关卡结束

    MSG_C2S_GetMainlineInfo,                               // 
    MSG_S2C_GetMainlineInfoResult,

    MSG_C2S_GetThemeInfo,
    MSG_S2C_GetThemeInfoResult,

    MSG_C2S_ExchangeDebrisEquip,                           // 碎片兑换装备
    MSG_S2C_ExchangeDebrisEquiSuc,                         // 碎片兑换装备成功
    MSG_S2C_ExchangeDebrisEquiFail,                        // 碎片兑换装备失败

    MSG_S2C_UpdateInvieFriend,

    MSG_C2S_GetFriendContactInfo,                         // 获取好友交互信息
    MSG_S2C_GetFriendContactInfoResult,                   // 获取好友交互信息(返回)
    MSG_C2S_GiveVitToFriend,                              // 赠送好友体力
    MSG_S2C_GiveVitToFriendResult,                        // 赠送好友体力(返回)
    MSG_S2C_GiveVitToFriendNotice,                        // 赠送体力给好友提示
    MSG_C2S_GetVitFromFrined,                             // 收取体力
    MSG_S2C_GetVitFromFrined,                             // 收取体力(返回)
    MSG_C2S_QuickFinishParclose,                          // 关卡扫荡
    MSG_S2C_QuickFinishParcloseSucess,                    // 关卡扫荡(成功)
    MSG_S2C_QuickFinishParcloseFail,                      // 关卡扫荡(失败)

	// S2G
	MSG_S2G_GetRankList,                                  // 获取排行列表
	MSG_S2G_UpdateParcloseInfo,                           // 更新关卡信息
    MSG_S2G_ClearInviteFriendInfo,
    MSG_S2G_AddInviteFriendInfo,    
	MSG_S2G_DelInviteFriendInfo,    
    MSG_S2G_GiveVitToFriend,                              // 赠送体力

    // G2S
    MSG_G2S_GetRankListResult,                            // 获取排行列表结果
    MSG_G2S_SyncInviteFriendInfo,
    MSG_G2S_GiveVitToFriend,                              // 赠送体力
};

enum eMSG_RANK
{
    MSG_RANK_BEGIN = MSG_TYPE_RANKING,

    MSG_C2S_Rank_GetHall,               // 获取排行榜大厅列表
    MSG_S2C_Rank_GetHallResult,

    MSG_C2S_Rank_GetDatas,              // 获取排行榜数据
    MSG_S2C_Rank_GetDatasResult,

    MSG_C2S_Rank_GetRewardRule,         // 获取奖励规则
    MSG_S2C_Rank_GetRewardRuleResult,

    // S2G/G2S
    MSG_S2G_Rank_GetHall,               // 获取排行榜大厅列表
    MSG_G2S_Rank_GetHallResult,

    MSG_S2G_Rank_GetDatas,              // 获取排行榜数据
    MSG_G2S_Rank_GetDatasResult,

    MSG_S2G_Rank_ChangeValue,           // 更新排行榜值
    MSG_G2S_Rank_UpdateMinValue,        // 更新排行榜最小值

    MSG_S2G_Rank_GetAllRankDataForMedal,        // 获取所有排行榜的数据
    MSG_G2S_Rank_GetAllRankDataResultForMedal,        // 返回所有排行榜的数据

    MSG_S2G_Rank_GetAllRankData,            // 获取所有排行榜的数据
    MSG_G2S_Rank_GetAllRankDataResult,      // 返回所有排行榜的数据
};

enum eMSG_GM
{
	MSG_GM_BEGIN = MSG_TYPE_GM,

	MSG_GM_M2G_SendGroupServerInfo,
	MSG_GM_G2S_SendGroupServerInofRes,

	MSG_GM_M2S_SendMail,
	MSG_GM_S2M_SendMailRes,

	MSG_GM_M2S_QueryRangeGift,
	MSG_GM_S2M_QueryRangeGiftRes,

	MSG_GM_M2S_QueryMultiGift,
	MSG_GM_S2M_QueryMultiGiftRes,

	MSG_GM_M2S_Announce,
	MSG_GM_S2M_Announce,

	MSG_GM_M2G_LockOrUnLockAccountByGm,
	
	MSG_GM_M2G_AddMCoinByGm,

	MSG_GM_M2G_UpdateCompensation,
	MSG_GM_G2M_UpdateCompensationRes,

	MSG_GM_M2G_GetServerLine,
	MSG_GM_G2M_GetServerLineRes,

	MSG_GM_M2G_GetRoomList,
	MSG_GM_G2M_GetRoomListRes,

	MSG_GM_M2G_SetMallUpdateTime,
	MSG_GM_M2G_GroupBuyControl,

	MSG_GM_M2G_SendPrivMCoin,
	MSG_GM_A2M_SendPrivMCoinRet,

	MSG_GM_M2G_GetServerPlayerCountInfo,
	MSG_GM_G2M_GetServerPlayerCountInfoRet,

	MSG_GM_G2A_GetServerPlayerLimit,
	MSG_GM_A2M_GetServerPlayerLimitRet,

	MSG_GM_M2A_SetServerPlayerLimit,

	MSG_GM_M2A_ChangeAccountPassword,
	MSG_GM_A2M_ChangeAccountPasswordRet,

	MSG_GM_M2A_SendBlackDeviceList,
	MSG_GM_A2M_SendBlackDeviceListRet,
	
    // 
	GM_M2G_GetInfoCenterRes,
	GM_G2M_GetInfoCenterRes,
	GM_M2G_AddInfoCenterRes,
	GM_G2M_AddInfoCenterRes,
	GM_M2G_UpdateInfoCenterRes,
	GM_G2M_UpdateInfoCenterRes,
	GM_M2G_RemoveInfoCenterRes,
	GM_G2M_RemoveInfoCenterRes,

    MSG_GM_G2S_DeleteVersionMedal,
    MSG_GM_S2G_DeleteVersionMedal,
    MSG_GM_M2G_DeleteVersionMedal,
    MSG_GM_G2M_DeleteVersionMedal,

    MSG_GM_M2G_AddMedalByGm,
    MSG_GM_G2M_AddMedalByGm,
};

enum eMSG_EXTERNAL
{
	MSG_EXTERNAL_BEGIN = MSG_TYPE_EXTERNAL,

	MSG_EX2G_SendMail,					//外部系统申请发邮件
	MSG_EX2G_SendMailGift,				//外部系统申请发邮件Gift
};

#endif



