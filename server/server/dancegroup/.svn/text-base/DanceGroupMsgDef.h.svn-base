#ifndef __DANCE_GROUP_MSG_DEF_H__
#define __DANCE_GROUP_MSG_DEF_H__

#include "../share/CommonMsgDef.h"

//舞团消息
enum eMSG_DanceGroup
{
	MSG_DANCEGROUP_BEGIN =  MSG_TYPE_DANCEGROUP,

	// C2S
	MSG_C2S_RequestMyDanceGroupInfo,				//请求自己的舞团信息
	MSG_C2S_CreateDanceGroup,						//创建舞团
	MSG_C2S_RequestEnterDanceGroup,					//申请
	MSG_C2S_CancelRequestEnterDanceGroup,			//取消申请
	MSG_C2S_ReplyRequestEnterDanceGroup,			//批复申请
	MSG_C2S_ReplyRequestListEnterDanceGroup,		//批量批复申请功能
	MSG_C2S_GetDanceGroupInfo,						//获取舞团信息
	MSG_C2S_GetDanceGroupInfoList,					//获取舞团信息列表
	MSG_C2S_ExitDanceGroup,							//退出舞团
	MSG_C2S_ChangeDanceGroupLeader,					//舞团传位
	MSG_C2S_DismissDanceGroup,						//解散舞团
	MSG_C2S_CancelDismissDanceGroup,				//取消解散舞团
	MSG_C2S_ChangeDanceGroupTitle,					//改变舞团职位
	MSG_C2S_KickOutDanceGroupMember,				//开除舞团成员
	MSG_C2S_ChangeDanceGroupColor,					//修改舞团颜色
	MSG_C2S_ChangeDanceGroupBadge,					//修改舞团徽章
	MSG_C2S_ChangeDanceGroupTitleName,				//修改舞团职位名称
	MSG_C2S_ChangeDanceGroupProfile,				//修改舞团简介
	MSG_C2S_ChangeDanceGroupAnnouncement,			//修改舞团公告
	MSG_C2S_GetDanceGroupBadgeInfos,				//获取舞团团徽信息
	MSG_C2S_UnlockDanceGroupBadge,					//解锁团徽特效(徽章)
	MSG_C2S_ChangeDanceGroupBadgeOrEffect,			//修改舞团徽章或效果
	MSG_C2S_DanceGroupActivityGetStarInfo,			//获取舞团活动基本信息
	MSG_C2S_DanceGroupActivityGetStarPowerInfo,
	MSG_C2S_DanceGroupActivityAddStarPower,			//舞团活动增加动力值操作
	MSG_C2S_DanceGroupActivityAddStarPowerPoint2Money,
	MSG_C2S_DanceGroupActivityOpenStarPacket,		//舞团活动打开礼包
	MSG_C2S_DanceGroupShopGetInfo,					//舞团商店信息获取
	MSG_C2S_DanceGroupShopExchange,					//舞团商店物品兑换
	MSG_C2S_GetDanceGroupRecords,					//获取舞团记录

	// S2C
	MSG_S2C_CreateDanceGroupResult,					//创建舞团结果
	MSG_S2C_RequestEnterDanceGroupResult,			//申请回复
	MSG_S2C_CancelRequestEnterDanceGroupResult,		//取消申请回复
	MSG_S2C_EnterDanceGroupSuccess,					//成功加入舞团
	MSG_S2C_GetDanceGroupInfoResult,				//获取舞团结果
	MSG_S2C_SendDanceGroupInfo,						//发送舞团信息
	MSG_S2C_SendSimpleDanceGroupInfo,				//发送简单舞团信息
	MSG_S2C_SendDanceGroupInfoList,					//发送舞团信息列表
	MSG_S2C_ExitDanceGroupResult,					//退出舞团结果
	MSG_S2C_ChangeDanceGroupLeaderResult,			//舞团传位结果
	MSG_S2C_DismissDanceGroupResult,				//解散舞团结果
	MSG_S2C_CancelDismissDanceGroupResult,			//取消解散舞团结果
	MSG_S2C_ChangeDanceGroupTitleResult,			//改变舞团职位结果
	MSG_S2C_KickOutDanceGroupMemberResult,			//开除舞团成员结果
	MSG_S2C_KickedOutDanceGroup,					//被开除出舞团
	MSG_S2C_ChangeDanceGroupColorResult,			//修改舞团颜色结果
	MSG_S2C_ChangeDanceGroupBadgeResult,			//修改舞团徽章结果
	MSG_S2C_ChangeDanceGroupTitleNameResult,		//修改舞团职位名称结果
	MSG_S2C_ChangeDanceGroupProfileResult,			//修改舞团简介结果
	MSG_S2C_ChangeDanceGroupAnnouncementResult,		//修改舞团公告结果
	MSG_S2C_DanceGroupRemoved,						//舞团被移除
	MSG_S2C_UpdateDanceGroupBaseInfo,				//舞团基本信息变更
	MSG_S2C_AddDanceGroupMember,					//舞团成员信息增加
	MSG_S2C_UpdateDanceGroupMemberInfo,				//舞团成员信息变更
	MSG_S2C_RemoveDanceGroupMember,					//舞团成员移除
	MSG_S2C_NotifyRequestEnterDanceGroup,			//通知有新玩家申请加入舞团
	MSG_S2C_NotifyRemoveRequestEnter,				//通知有移除申请
//	MSG_S2C_UpdateDanceGroupName,					//通知舞团名字发生变更
//	MSG_S2C_UpdateDanceGroupTitle,					//通知舞团职位信息发生变更
	MSG_S2C_ReplyRequestEnterDanceGroupResult,		//批复申请结果
	MSG_S2C_ReplyRequestListEnterDanceGroupResult,	//批量批复申请结果
	MSG_S2C_GetDanceGroupBadgeInfosResult,			//获取团徽信息结果
	MSG_S2C_UnlockDanceGroupBadgeResult,			//解锁团徽特效(徽章)结果
//	MSG_S2C_ChangeDanceGroupBadgeOrEffectResult,	//修改徽章或效果结果
	MSG_S2C_DanceGroupActivityGetStarInfoResult,	//舞团活动获得基本信息
	MSG_S2C_DanceGroupActivityGetStarPowerInfoResult,
	MSG_S2C_DanceGroupActivityAddStarPowerResult,	//舞团活动增加动力值操作
	MSG_S2C_DanceGroupActivityStarAddPowerResultMCoin2MoneyResult,
	MSG_S2C_NotifyDanceGroupActivityStarState,
	MSG_S2C_DanceGroupActivityOpenStarPacketResult,	//舞团活动打开礼包
	MSG_S2C_DanceGroupActivityReset, //服务器重置活动信息
	MSG_S2C_DanceGroupShopGetInfoResult,			//舞团商店信息获取
	MSG_S2C_DanceGroupShopExchangeResult,			//舞团商店兑换
	MSG_S2C_GetDanceGroupRecrodsResult,
	MSG_S2C_DanceGroupAddRecordsNotify,//舞团商店通知
	MSG_S2C_DanceGroupChangeDayNotify,//每天更新一次舞团记录
	MSG_S2C_DanceGroupBeRefuseRefresh,// 申请被拒绝刷新大厅申请信息

	//added
	MSG_C2S_DonateDanceGroupResource,				//捐赠资源
	MSG_C2S_UpgradeDanceGroup,						//升级舞团
	MSG_S2C_DonateDanceGroupResourceResult,			//捐赠资源结果
	MSG_S2C_UpgradeDanceGroupResult,				//升级舞团结果
	MSG_C2S_ChangeDanceGroupName,					//修改舞团名字
	MSG_S2C_ChangeDanceGroupNameResult,				//修改舞团名字结果

	MSG_S2C_UpdateDanceGroupAnnounce,				//更新舞团公告
	MSG_S2C_UpdateDanceGroupProfile,				//更新舞团简介
	MSG_S2C_DanceGroupInfo,							//所在舞团全部信息

    // 邀请舞团成员
    MSG_C2S_InviteDanceGroupMember,                 // 邀请舞团成员
    MSG_S2C_InviteDanceGroupMemberResult,           // 邀请舞团成员结果
    MSG_S2C_BeInvitedAsDanceGroupMember,            // 被邀请加入舞团
    MSG_C2S_AcceptDanceGroupMemberInvitation,       // 接受舞团成员邀请
    MSG_S2C_AcceptDanceGroupMemberInvitationResult, // 接受舞团成员邀请结果

	// liguanghao add, 入团仪式
    MSG_S2C_SyncCeremonyConfig,                     //入团仪式配置信息
    MSG_S2C_SyncCermonyApplyTimes,                  //同步入团仪式已经申请次数
    MSG_C2S_RequestApplyCeremony,					//申请入团仪式
	MSG_S2C_RequestApplyCeremonyRes,				//申请入团仪式结果
	MSG_S2C_SyncCeremonyState,						//同步入团仪式状态

	MSG_C2S_RequestEnterCeremony,					//请求进入仪式房间
	MSG_S2C_EnterCeremonyRes,						//进入仪式房间结果

	MSG_C2S_RequestQuitCeremony,					//请求退出仪式房间
	MSG_S2C_QuitCeremonyRes,						//退出仪式房间结果

	MSG_C2S_RequestRefreshCeremony,					//请求刷新仪式房间
	MSG_S2C_RefreshCeremonyRes,						//刷新仪式房间结果

	MSG_C2S_RequestStartCeremony,					//请求开始仪式
	MSG_S2C_StartCeremonyRes,						//开始仪式结果
    MSG_S2C_CeremonyRoomStartFail,                  //自动仪式开启失败          

	MSG_S2C_OtherPlayerEnterCeremony,				//其他玩家进入仪式房间
	MSG_S2C_OtherPlayerQuitCeremony,				//其他玩家退出仪式房间

	MSG_C2S_SetCeremonyRookie,						//设为仪式新人
	MSG_S2C_SetCeremonyRookieRes,					//设为仪式新人结果
	MSG_C2S_CancelCeremonyRookie,					//取消仪式新人
	MSG_S2C_CancelCeremonyRookieRes,				//取消仪式新人结果
	MSG_S2C_SyncCeremonyRookie,						//同步仪式新人信息

	MSG_C2S_KickCeremonyPlayer,						//将玩家请出仪式房间
	MSG_S2C_KickCeremonyPlayerRes,					//将玩家请出仪式房间结果

	MSG_C2S_SaveCeremonyIntruduction,				//保存自我介绍
    MSG_S2C_SaveCeremonyIntruductionRes,            
	MSG_C2S_RequestCeremonyRecord,					//请求仪式日志
	MSG_S2C_RequestCeremonyRecordRes,				//请求仪式日志结果
	MSG_C2S_RequestCeremonyPraise,
	MSG_S2C_RequestCeremonyPraiseRes,
	MSG_C2S_RequestPraiseRookie,
	MSG_S2C_RequestPraiseRookieRes,
    MSG_S2C_SyncPraiseInfo,                         // 同步点赞信息

    // 舞团试炼
    MSG_S2C_SyncChallengeConfig,                    // 同步试炼配置信息
    MSG_C2S_RequestEnterChallenge,                  // 请求进入
    MSG_S2C_RequestEnterChallengeRes,
    MSG_C2S_RequestChallengeStoreyList,             // 请求简易列表
    MSG_S2C_RequestChallengeStoreyListRes,
    MSG_C2S_RequestChallengeStoreyInfo,             // 请求详细信息
    MSG_S2C_RequestChallengeStoreyInfoRes,
    MSG_C2S_RequestChallengeRankList,               // 请求排行
    MSG_S2C_RequestChallengeRankListRes,
    MSG_C2S_RequestGetChallengeReward,              // 请求获取奖励
    MSG_S2C_RequestGetChallengeRewardRes,
    MSG_S2C_ChallengeRewardNotice,                  // 可以获取奖励通知
    MSG_C2S_RequestChallengeInspire,                // 请求鼓舞
    MSG_S2C_RequestChallengeInspireRes,
    MSG_C2S_RequestAddChallengeChance,              // 请求增加次数
    MSG_S2C_RequestAddChallengeChanceRes,
    MSG_S2C_RefreshChallengeChanceNotice,           // 刷新次数通知
    MSG_C2S_RequestOpenChallengeStorey,             // 请求开启
    MSG_S2C_RequestOpenChallengeStoreyRes,
    MSG_C2S_RequestPrepareChallenge,                // 请求准备挑战
    MSG_S2C_RequestPrepareChallengeRes,
    MSG_C2S_RequestStartChallenge,                  // 请求开始挑战
    MSG_S2C_RequestStartChallengeRes,
    MSG_S2C_ChallengeStageEnd,                      // 关卡结束
    MSG_S2C_ChallengeStoreyFinishedNotice,          // 当前层结束通知
    MSG_S2C_ChallengeClosedNotice,                  // 试炼关闭通知


    // 舞团大妈：
    MSG_S2C_SyncBigMamaConfig,
    MSG_C2S_RequestBigMamaStageInfo,
    MSG_S2C_SyncBigMamaStageInfo,
    MSG_C2S_RequestFightBigMama,
    MSG_S2C_RequestFightBigMamaRes,
    MSG_C2S_RequestGetBigMamaReward,
    MSG_S2C_RequestGetBigMamaRewardRes,
    MSG_C2S_CompleteBigMamaGuide,
    MSG_C2S_RequestBigMamaGuideReward,
    MSG_S2C_NoticeBigMamaState,
    MSG_S2C_BigMamaIntroduce,
};

enum EMsgDanceGroupServer
{
    MSG_DANCEGROUP_SERVER_BEGIN = MSG_TYPE_DANCEGROUP_SERVER,
    // S2G
    MSG_S2G_CreateDanceGroup,						//创建舞团
    MSG_S2G_RequestEnterDanceGroup,					//申请
    MSG_S2G_RequestEnterDanceGroupByName,           //机器人专用，通过groupName加入舞团
    MSG_S2G_ReplyRequestEnterDanceGroup,			//批复申请
    MSG_S2G_ReplyRequestListEnterDanceGroup,//批量批复申请
    MSG_S2G_ExitDanceGroup,							//退出舞团
    MSG_S2G_ChangeDanceGroupLeader,					//舞团传位
    MSG_S2G_DismissDanceGroup,						//解散舞团
    MSG_S2G_CancelDismissDanceGroup,				//取消解散舞团
    MSG_S2G_ChangeDanceGroupTitle,					//改变舞团职位
    MSG_S2G_KickOutDanceGroupMember,				//开除舞团成员
    MSG_S2G_ChangeDanceGroupColor,					//修改舞团颜色
    MSG_S2G_ChangeDanceGroupBadge,					//修改舞团徽章
    MSG_S2G_ChangeDanceGroupTitleName,				//修改舞团职位名称
    MSG_S2G_ChangeDanceGroupProfile,				//修改舞团简介
    MSG_S2G_ChangeDanceGroupAnnouncement,			//修改舞团公告
    MSG_S2G_UpdateDanceGroupMemberInfo,				//更新成员信息
    MSG_S2G_DonateDanceGroupResource,				//捐赠资源
    MSG_S2G_UpgradeDanceGroup,						//升级舞团			
    MSG_S2G_ChangeDanceGroupName,					//修改舞团名字
    MSG_S2G_ChangeDanceGroupHonor,					//修改舞团荣誉
    MSG_S2G_UnlockDanceGroupBadge,					//解锁团徽
    MSG_S2G_ChangeDanceGroupBadgeOrEffect,			//修改徽章或效果
    MSG_S2G_DanceGroupTuanhuiPerDayConsume,//舞团团徽每天消耗操作
    MSG_S2G_DanceGroupActivityAddStarPower,			//舞团活动增加动力值
    MSG_S2G_DanceGroupActivityOpenStarPacket,		//开礼包操作
    MSG_S2G_UpdateDanceGroupActivityReward,
    MSG_S2G_DanceGroupShopExchange,					//舞团商店兑换
    MSG_S2G_DanceGroupShopResetExchangeCount,//舞团商店重置兑换次数
    MSG_S2G_GetDanceGroupRecords,					//获得舞团记录
    MSG_S2G_DanceGroupAddRecord, //添加舞团记录
    MSG_S2G_AddDanceGroupContribution,				//增加舞团贡献值
    MSG_S2G_ReduceDanceGroupContribution,			// 减少舞团贡献值
    MSG_S2G_GetDanceGroupInfo,                      // 获取舞团信息
    MSG_S2G_GetMyDanceGroupInfo,                    // 获取舞团信息
    MSG_S2G_DanceGroupActivityGetStarInfo,          // 获取舞团星星信息
    MSG_S2G_DanceGroupActivityGetStarPowerInfo,     // 获取舞团动力值
    MSG_S2G_DanceGroupShopGetInfo,                  // 获取舞团商店信息
    MSG_S2G_AcceptDanceGroupMemberInvitation,       // 接受舞团成员邀请
    MSG_S2G_AddDanceGroupChallengeActive,           // 增加舞团试炼活力值
    MSG_S2G_CanBeInviteCheck,                       // 检测玩家是否可以进入舞团

    // G2S
    MSG_G2S_CreateDanceGroupResult,					//创建舞团结果
    MSG_G2S_EnterDanceGroupSuccess,					//成功加入舞团
    MSG_G2S_DonateDanceGroupResourceResult,			//捐赠资源结果
    MSG_G2S_UpdateDanceGroupActivityStarState,		//更新舞团活动(星星变的时候更新)
    MSG_G2S_UpdateDanceGroupActivityInfo,
    MSG_G2S_DanceGroupActivityReset,
    MSG_G2S_DanceGroupAddRecordResult, //添加舞团记录
    MSG_G2S_DanceGroupShopGetInfoResult,

    MSG_G2S_UpdateDanceGroupBaseInfo,				//更新舞团基本信息
    MSG_G2S_AddDanceGroupMemberInfo,				//添加舞团成员信息
    MSG_G2S_UpdateDanceGroupMemberInfo,				//更新舞团成员信息
    MSG_G2S_RemoveDanceGroupMemberInfo,				//移除舞团成员信息
    MSG_G2S_AddRequestDanceGroupInfo,				//添加舞团申请信息
    MSG_G2S_RemoveRequestDanceGroupInfo,			//移除舞团申请信息
    MSG_G2S_GetMyDanceGroupInfo,                    // 获取舞团信息
    MSG_G2S_AcceptDanceGroupMemberInvitationResult, // 接受舞团成员邀请结果
    MSG_G2S_BroadcastMemberKicked,                  // 广播玩家被踢出的消息
    MSG_G2S_BroadcastMemberQuit,                    // 广播玩家退出舞团的消息
    MSG_G2S_UpdateDanceGroupChallengeActive,        // 更新舞团试炼活力值
    MSG_G2S_CanBeInviteCheck,                       // 检测玩家是否可以进入舞团回复
    MSG_G2S_ChangeDanceGroupBadgeOrEffectResult,	//修改徽章或效果结果

    // ligh add 入团仪式
    MSG_S2G_CeremonyApplyCreate,					// 申请创建房间
    MSG_G2S_CeremonyApplyCreateRet,					// 申请创建房间
    MSG_S2G_CeremonyCreateRoomNotify,				// 创建房间通知（创建成功的时候）
    MSG_S2G_CeremonyApplyCreateCancel,				// 取消申请（创建失败的时候）
    MSG_S2G_CeremonyCloseRoomNotify,				// 房间关闭申请
    MSG_S2G_CeremonyRoomStateChange,				// 房间状态改变
    MSG_G2S_CeremonyRoomStateChange,				// 房间状态改变
    MSG_S2G_CeremonyRoomNewMember,				    // 新人通知
    // 开始发送,投票信息,发送结束
    MSG_S2G_CeremonyRoomLogsStart,				    // 日志记录
    MSG_S2G_CeremonyRoomLogsItem,                   // 每个人的投票信息
    MSG_S2G_CeremonyRoomLogsEnd,                    // 发送结束
    MSG_S2G_CeremonyRoomStatics,                    // 请求glbal数据统计
    MSG_G2S_CeremonyRoomStatics,                    // 回复数据统计
    Msg_S2G_CeremonySyncRoomInfo,                   // 仪式房间信息同步。	


    // 舞团试炼
    MSG_S2G_EnterChallenge,                         // 进入舞团试炼
    MSG_G2S_EnterChallengeRes,
    MSG_S2G_GetChallengeStoreyInfo,                 // 获取试炼塔信息
    MSG_G2S_GetChallengeStoreyInfoRes,
    MSG_S2G_GetChallengeRankList,                   // 获取排行榜信息
    MSG_G2S_GetChallengeRankListRes,
    MSG_S2G_ChallengeInspire,                       // 鼓舞
    MSG_G2S_ChallengeInspireRes,
    MSG_S2G_OpenChallengeStorey,                    // 开启
    MSG_G2S_OpenChallengeStoreyRes,
    MSG_S2G_PrepareChallenge,                       // 准备挑战
    MSG_G2S_PrepareChallengeRes,
    MSG_S2G_ReportChallengeScore,                   // 提交挑战分数
    MSG_G2S_ReportChallengeScoreRes,
    MSG_G2S_ChallengeStoreyFinishedNotice,          // 当前层结束通知
    MSG_G2S_ChallengeRewardNotice,                  // 可以获取奖励通知

    // 舞团大妈内部消息：
    MSG_S2G_BigMamaStageInfoRequest,                // 请求大妈关卡信息
    MSG_G2S_BigMamaStageInfoNotify,                 // 回复关卡信息
    MSG_S2G_BigMamaBeatSucess,                      // 斗舞成功消息
    MSG_G2S_BigMamaStateNotify,                     // 开、关状态变更通知
    MSG_G2S_BigMamaSyncStageInfo,                   // 推送新的关卡消息
    MSG_G2S_BigMamaInfoNotifyOnLogin,               // 上线推送
    MSG_G2S_BigMamaInfoNotifyOnJoinDanceGroup,      // 上线推送
    MSG_S2G_BigMamaFightRequest,                    // 请求战斗
    MSG_G2S_BigMamaFightResult,                     // 请求战斗回复
    MSG_S2G_BigMamaGetRewardRequest,                // 请求领奖
    MSG_G2S_BigMamaGetRewardResult,                 // 请求领奖回复

};

#endif


