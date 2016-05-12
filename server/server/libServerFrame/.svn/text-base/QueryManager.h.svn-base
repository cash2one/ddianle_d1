// QueryManager.h: interface for the CQueryManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __QUERYMANAGER_H__
#define __QUERYMANAGER_H__

#include "DBInterface.h"
#include "../../socket/Query.h"
#include "../../socket/QueryManagerBase.h"
#include <map>
using namespace std;

#define QUERYTAG_CheckName		1
#define QUERYTAG_Friend			2
#define QUERYTAG_Blacklist		3
#define QUERYTAG_Login			4

enum QUERY_TYPE
{
    QUERY_TYPE_BEGIN = 0x00000001,

    QUERY_GetRegistedRoleCount			,	//得到注册角色数量
    QUERY_GetRegistedPlayerCount		,	//得到注册玩家数量
    QUERY_GetBatchID64					,	//得到一批64位id

    QUERY_Bill_LoadInfo					,	//读取计费信息
    QUERY_Bill_UpdatePt					,	//扣点
    QUERY_Bill_ChongZhi					,	//充值
    QUERY_Bill_ChongZhiByPUID			,	//PUID充值
    QUERY_Bill_GetGift					,	//领取充值礼品，m_nAttachment是领取的礼品id,m_nTag是领取的礼品数量, m_pTag是领取礼品的帐号名称（char指针,注意分配和释放空间）
    QUERY_Bill_GiveGift					,	//送礼给玩家,m_nAttachment是礼品id,m_nTag是的礼品数量, m_pTag是送礼品的帐号名称（char指针,注意分配和释放空间）

    //获取所有可领取礼品信息(m_pAttachment必须是一个list< BILLGIFT >指针，m_pTag是领取礼品的帐号名称（char指针）)
    //m_nAttachment为entity下标，m_nTag为roleID,这2个都需要保留返回
    QUERY_Bill_LoadGiftList				,

    QUERY_Bill_UpdateChange				,
    QUERY_Bill_UpdateTotalGift			,	//更新已领取礼品的总点数,m_nAttachment是新的已领取总奖励的点数，m_nTag为玩家的帐号（即Account）
    QUERY_Bill_SaveChargeAction         ,   // 保存充值行为

    //QUERY_Item_LoadGiftActivateCode		,	
    QUERY_Item_GetActivateGiftInfo				,	//查询赠送礼包信息
    QUERY_Item_GetActivateGift				,	//激活礼包

    QUERY_Mall_LoadInfo		,
    QUERY_Mall_SaveLog		,
    QUERY_Mall_SigleItemLog ,				//单个物品log
    QUERY_Mall_DesireNum	,

    QUERY_Bill_UpdateChangeByUsername   ,	//以用户名为查询条件更新抵用券，m_pAttachment必须是new好的char[MAX_NAME]，存放的帐号名，查询结束后数据库层负责delete
    QUERY_Change_Getlog			        ,

    QUERY_CreateAccount					,	//创建账号信息
//    QUERY_GetAccountInfo				,	//得到账号信息
    QUERY_AccountLogin					,	//非合作版本登陆需要验证密码
    QUERY_AcountQuickCreate				,	//快速创建账号
    QUERY_CheckAccountIsExist			,	//查询账号是否在游戏中
//    QUERY_GetInviteAccountInfo			,	//得到被邀请人账号信息
    QUERY_CreateRole					,	//创建角色
    QUERY_CheckRoleIsExist				,	//查询角色是否在游戏中
    QUERY_UpdateRole					,	//修改角色信息
    QUERY_DeleteRole					,	//删除角色
    QUERY_GetRoleInfo					,	//通过RoleID得到角色信息
    QUERY_UpdateAccountState			,	//更新账号状态
    QUERY_UpdateAccountLockState		,	//更新账号锁定状态
    QUERY_ACCOUNT_LoadAutoAccount       ,	//读取自动生成的账号
    QUERY_ACCOUNT_UpdateAutoAccountState,	//更新自动生成的随机账号的状态0不可用，1可用
    QUERY_ACCOUNT_CreateNewAutoAccount,		//产生随机账号
    QUERY_ACCOUNT_LoadActivateCode,			//产生激活码
    QUERY_ACCOUNT_ActivateCode,				//使用激活码成功的数据库存储过程

    QUERY_QuickCreateAccount			,	//快速创建账号
    QUERY_ResetAccountState				,	//重置所有账号状态
    QUERY_AccountLogout,                    //账号登出
    QUERY_LoadCommands					,	//读取需要执行的命令
    QUERY_GetRoleInfoBase				,	//得到角色的基本信息
    QUERY_GetRoleInfoAttrSetting		,	//得到角色的属性和设定
    QUERY_LoadRoleOfflineTime           ,   //上次登录时间

    //2012-2-15
    QUERY_CheckRoleName					,	//检查名字是否重复
    QUERY_PlayerGetRoleInfo				,	//玩家获取另一玩家的信息


    //这些操作不需要处理结果，指针查询完成后由查询线程释放
    QUERY_LoadOfflineRoleData           , //查询一个角色物品信息
    QUERY_LoadListOfflineRoleData       , //查询一个角色物品信息
    QUERY_AddRoleItem					, //给一个角色添加一个物品，参数m_nAttachment为角色RoleID，参数m_pAttachment为ITEMINFO指针
    QUERY_UpdateRoleItem				, //修改一个角色的一个物品，参数m_nAttachment为物品新主人角色RoleID，参数m_pAttachment为ITEMINFO指针
    QUERY_RemoveRoleItem				, //删除角色的一个物品，参数m_nTag为item的nItemID
    QUERY_RemoveRoleItemByType			, //删除角色的一个物品，通过roleid + itemtype
    QUERY_RemoveDynamicBoxRelated		, //删除角色动态宝箱关联数据，通过roleid+itemid
    QUERY_UpdateCoolDown				, //更新物品CD时间
    QUERY_UpdateItemCount				, //修改物品数量,m_nAttachment为新数量，参数m_nTag为item的nItemID
    QUERY_UpdateItemPosition			, //修改物品的位置,数m_pAttachment为ITEMINFO_Position指针
    QUERY_Update2ItemPosition			, //修改2个物品的位置,数m_pAttachment为ITEMINFO_Position2指针
    QUERY_UpdateItemOwner				, //修改物品所有者,m_nAttachment为新RoleID，参数m_nTag为item的nItemID
    QUERY_AddRenewItemLog               , //增加一个续费记录
    QUERY_AddRoleClothGroup				, //添加服饰搭配
    QUERY_UpdateRoleClothGroup			, //更新服饰搭配
    QUERY_InsertClothEffect             , //给服饰添加特效
    QUERY_UpdateClothEffect             , //更新服饰特效
    QUERY_InsertRoleEffect              , //添加角色衣服特效
    QUERY_InsertHandbookProgress        , //插入图鉴套装进度
    QUERY_UpdateHandbookProgress        , //更新图鉴套装进度
    QUERY_InsertColorProgress           , //插入七色花瓣进度
    QUERY_UpdateColorProgress           , //更新七色花瓣进度
    QUERY_InsertStorageCapacity         , //插入仓库进度
    QUERY_UpdateStorageCapacity         , //更新仓库进度

    //更新玩家的单独的数据
    QUERY_UpdateRoleMoney				, //改变玩家的金钱	，因为要和物品操作同步，参数m_nAttachment为角色RoleID，参数m_nTag为item的金钱
    QUERY_UpdateRoleExpTime				, //改变玩家双倍经验值的剩余时间	，参数m_nAttachment为角色RoleID，参数m_nTag为剩余的双倍经验值时间，参数m_pAttachment为最后重置剩余的双倍经验值时间的时间
    QUERY_UpdateRoleBadgeNum            , //改变角色的徽章格子数
    QUERY_UpdateRoleSignature			, //修改玩家签名
    QUERY_UpdateRoleMoneyByAccountId	, //改变玩家的金券(充值奖励)


    // friend
    QUERY_GetBeAppliedPlayerInfo		,   // 获取要添加的玩家的信息
    QUERY_AddBlackListFriend			,	// 从黑名单添加好友					
    QUERY_AddRoleFriend					,	// 添加好友或黑名单，参数m_nAttachment为角色的RoleID,参数m_pAttachment为好友的FriendInfo指针，m_nTag为类型（0表示好友，1表示黑名单）
    QUERY_RemoveRoleFriend				,	// 删除好友或黑名单，参数m_nAttachment为角色的RoleID,参数m_nTag为好友的RoleID
    QUERY_AddBlackList					,	// 添加黑名单
    QUERY_RemoveBlackList				,	// 删除黑名单
    QUERY_UpdateFriendEntry				,	// 更新朋友类型，好友 黑名单转换
    QUERY_BecomeFriend					,	// 可能是由黑名单变至好友，也有可能直接INSERT进去
    QUERY_ReworkFriendNotes				,	// 修改好友备注
    QUERY_LoadFriendOfflineTime		    ,	// 加载好友离线时间


    QUERY_LockLotteryCode				,	// 验证邀请码

    QUERY_AddRoleGene					, //增加基因
    QUERY_DeleteRoleGene				, //删除基因
    QUERY_UpdateRoleGene				, //更新基因
    QUERY_UpdateRoleBagExpand			, //背包扩展

    QUERY_AddRoleSuggestion				, //增加角色反馈建议
    QUERY_AddOnlineState				, //增加在线人数信息
    QUERY_ChangeRoleName				, //修改角色名字
    QUERY_ModifyRoleSex					, //修改角色性别
    QUERY_Bill_LoadCBChongzhiValue		, //读取内测充值总额
    QUERY_Bill_DestoryCBChongzhiValue	, //领取完成，充值标志位为不可再次领取状态
    QUERY_Save_LoginNum					, //保存活跃用户数据
    QUERY_ChangeBirthday				, //修改角色生日


    //首页公告以及商城公告
    QUERY_LoadHomeNews					,
    QUERY_LoadShopNews					,


    //邮件
    QUERY_LoadMail                      ,
    QUERY_AddMail                       ,
    QUERY_AddSysMail                    ,
    QUERY_GetAllMailItem                ,
    QUERY_GetMailMoney                  ,
    QUERY_ReadMail                      ,
    QUERY_DeleteMail                    ,
    QUERY_MailGetRoleID					,   //通过角色名获取角色id
    QUERY_LoadMailStrangerSetting       ,   // 加载是否接受陌生人邮件设置
    QUERY_UpdateMailStrangerSetting     ,   // 设置自己是否接受陌生人邮件


    // 任务
    QUERY_AddRoleQuest					, 
    QUERY_UpdateRoleQuest				,	// Target和QuestInfo一起
    QUERY_RemoveRoleQuest				, 
    QUERY_UpdateTargetInfo				,
    QUERY_UpdateQuestInfo				,	// 仅仅是QuestInfo更新

    QUERY_AddRoleQuestHistory			,	
    QUERY_UpdateRoleQuestHistory		, 
    QUERY_RemoveRoleQuestHistory	, 
    QUERY_AddRepeatedQuestDone,
    QUERY_UpdateRepeatedQuestDone,

    // 成就
    QUERY_AddAchievement,
    QUERY_UpdateAchievementState,

    // 称号
    QUERY_AddTitle,

    // 签到
    QUERY_InsertCheckInInfo,
    QUERY_UpdateCheckInInfo,
    QUERY_AddCheckInGift,
    QUERY_RemoveCheckInGift,
    //QUERY_RemoveCheckInGiftByList,

    // VIP
    QUERY_SaveVIPInfo,
    QUERY_SaveVIPConsumedPt,

    // event
    QUERY_AddFirstPaymentInfo,			// 记录首充事件,FinishFirstPayment = 8
    QUERY_AddSendFirstPaymentReward,	// 记录发送首充奖励事件,HasSentFirstPaymentReward = 9

    QUERY_AddFirstPaymentInfoFeed,		// FinishFirstPayment_feed = 11,
    QUERY_AddSendFirstPaymentRewardFeed,// HasSentFirstPaymentReward_feed = 12

    QUERY_GetFirstPaymentReward,
    QUERY_GetFirstPaymentRewardByAccountId,
    QUERY_AddCompleteTeachGuideEvent,
    QUERY_AddCompleteProcGuideEvent,
    QUERY_UpdateCompleteProcGuideEvent,
    QUERY_AddOrUpdateEvent,
	QUERY_UpdateFixedChargeReward,			// 更新固定金额首冲状态
	QUERY_LoadFixedChargeReward,			// 加在固定金额首冲状态
    QUERY_UpdateMallSendItemEvent,
	QUERY_UpdateGuideDataEvent,

    // desired item
    QUERY_Desire_Insert,
    QUERY_Desire_Delete,

    // money
    QUERY_ResetMoney,
    QUERY_UpdateResetMoneyTime,
    QUERY_UpdateMoneyMailRemindTime,
    QUERY_UpdateMoneyMallRemindTime,

    // online reward
    QUERY_UpdateOnlineRewardInfo,

    // GM
    GetRcvGMSysMailRoleInfo,
    QUERY_QueryGiftByTimeRange,
    QUERY_QueryMultiGift,

    //gift
    QUERY_Activity_LoadGift,			//加载礼物
    QUERY_Activity_GetGiftReward,		//获取礼物

    //累计充值奖励
    QUERY_LoadCumulativeRechargeInfo,				//读取玩家已领取的充值奖励
    QUERY_SaveCumulativeRecharge,

    //累计消费
    QUERY_LoadCumulativeSpendInfo,
    QUERY_LoadCumulativeSpendInfoForMedal,
    QUERY_Update_CumulativeReward,
    QUERY_Insert_CumulativeReward,
    QUERY_Insert_CumulativeSpendBill,
    QUERY_Insert_CumulativeSpendBindBill,

    // 累计消费获取勋章
    QUERY_LoadCumulativeSpendGotMedalReward,
    QUERY_InsertCumulativeSpendGotMedalReward,

    // 购买物品获取勋章
    QUERY_LoadBuyItemGotMedalRewardedInfo,
    QUERY_SetBuyItemGotMedalRewarded,
    QUERY_InsertBuyItemGotMedalInfo,
    QUERY_UpdateBuyItemGotMedalInfo,

    //圣诞活动-免费领取礼物
    QUERY_LoadXmasFreeRewardInfo,
    QUERY_SaveXmasFreeRewardInfo,

    //节日许愿
    QUERY_LoadFestivalWishInfo,
    QUERY_SaveFestivalWishInfo,

    //节日祝福活动
    QUERY_SaveFestivalBlessEntry,
    QUERY_SaveRoleFestivalBlessRewardInfo,
    QUERY_LoadRoleBlessActivityInfo,

    //IOS专有版奖励领取
    QUERY_AddProprietaryActivityReward,

    // 舞团红包
    QUERY_RedEnvelope_LoadInfo,                 // 加载红包
    QUERY_RedEnvelope_InsertNew,                // 插入红包
    QUERY_RedEnvelope_Update,                   // 更新红包信息
    QUERY_RedEnvelope_UpdateAnonymityEvent,     // 更新匿名设置
    QUERY_RedEnvelope_LoadRoleInfo,             // 舞团红包加载玩家信息

    //DanceGroup
    QUERY_DanceGroupLoadAllInfo,				//加载所有的舞团信息
    QUERY_DanceGroup_Create,					//创建舞团
    QUERY_DanceGroup_Remove,					//移除舞团
    QUERY_DanceGroup_UpdateDanceGroupBaseInfo,	//更新舞团基本信息
    QUERY_DanceGroup_AddDanceGroupMember,		//添加舞团成员
    QUERY_DanceGroup_UpdateMember,				//更新舞团成员信息
    QUERY_DanceGroup_UpdateMemberWithTime,		//加入时间（在对隔天重置数据做修改后该时间会被重置）
    QUERY_DanceGroup_DelDanceGroupMember,		//删除舞团成员
    QUERY_DanceGroup_UpdateRequestDanceGroup,	//更新舞团申请信息
    QUERY_DanceGroup_RemoveRequestDanceGroup,	//移除舞团申请信息
    QUERY_DanceGroup_ChangeDanceGroupName,		//修改舞团名称
    QUERY_DanceGroup_UpdateActivityInfo, //更新舞团活动信息
    QUERY_DanceGroup_AddRecord, //舞团记录添加
    QUERY_DanceGroup_WriteQuitInfo,             // 记录玩家退出舞团信息
    QUERY_DanceGroup_CeremonyLoadLog,           // 加载入团仪式日志
    QUERY_DanceGroup_CeremonyWriteLog,          // 记录玩家入团仪式日志

    QUERY_DanceGroup_LoadBigMamaGroupInfo,              // 舞团大妈舞团数据加载
    QUERY_DanceGroup_UpdateOrInsertBigMamaGroupInfo,    // 舞团大妈舞团数据写入
//    QUERY_DanceGroup_LoadBigMamaRoleInfo,               // 舞团大妈个人数据加载
    QUERY_DanceGroup_UpdateOrInsertBigMamaRoleInfo,     // 舞团大妈个人数据写入

    //External
    QUERY_External_SendMail,					//外部系统发送邮件
    QUERY_External_SendMailGift,				//外部系统发送Gift邮件

    //玩家个人设置
    QUERY_Player_SavePlayerSetting,

    //情侣婚姻
    QUERY_Couple_AddCouple,
    QUERY_Couple_DelCouple,
    QUERY_Couple_UpdateCoupleInfo,
    QUERY_Couple_UpdateIntimacy,
    QUERY_Couple_UpdateLoveDec,
    QUERY_Couple_UpdateCoupleBlessingValue,
    QUERY_Couple_AddBlessingRankInfo,
    QUERY_Couple_DelBlessingRankInfo,
    QUERY_Couple_UpdateBlessingRankValue,
    QUERY_Couple_LoadBlessingValueRank,
    QUERY_Couple_LoadOfflineCoupleInfo,
    QUERY_Couple_UpdateMarriageGiftDate,
    QUERY_Couple_AddDivorceAgreement,
    QUERY_Couple_AddDivorceAction,
    QUERY_Couple_UpdateCoupleAgreementID,
    QUERY_Couple_UpdateDivorceAgreementState,
    QUERY_Couple_UpdateDivorceAgreement,

    // 幸运魔法阵
    QUERY_Insert_MagicArrayRecord,
    QUERY_Update_MagicArrayRecord,
    QUERY_Insert_MagicArrayExchange,
    QUERY_Update_MagicArrayExchange,

    //拼图活动
    QUERY_Puzzle_LoadPicture,
    QUERY_Puzzle_InsertPicture,
    QUERY_Puzzle_UpdatePicture,

    //新玩家招募活动
    QUERY_Invitation_UpdateInvitationCode,
    QUERY_Invitation_CheckInvitationCode,
    QUERY_Invitation_AddInvitation,
    QUERY_Invitation_GetInvitationInfo,
    QUERY_Invitation_AddInviterReward,
    QUERY_Invitation_AddAcceptorReward,

    //VIP上线公告
    QUERY_VIP_UpdateOnlineAnnounceTime,

    //评论消息推送
    QUERY_Rating_NoPushRating,
    QUERY_Rating_PushRatedLevelUp,
    QUERY_Rating_PushRatedOnlineTime,
    QUERY_Rating_PushRatedDailyQuest,
    QUERY_Rating_PushRatedDailyShoping,
    QUERY_Rating_PushRatedCount,

    //安全保护问题
    Query_Security_SetSecurityQuiz,

    //新手七天乐活动
    QUERY_FresherActivity_AddReward,

    //新手七天目标
    QUERY_FresherQuest_AddReward,
    QUERY_FresherQuest_BestMusicGrade,
    QUERY_Mall_AddMallBuyCount,

    //直播房间
    Query_LiveRoom_GetDancerName,
    Query_LiveRoom_UpdateDancerName,
    Query_LiveRoom_LoadPointAndHot,
    Query_LiveRoom_GetPointAndHot,
    Query_LiveRoom_UpdateAuditionPoint,
    Query_LiveRoom_UpdateDancerHot,

    //补偿
    Query_LoadCompensation,				//从数据库加载
    Query_InsertCompensation,			//GM添加补偿
    Query_InsertReceiveCompensation,	//玩家领取补偿

    //平台特权
    Query_PlatformPrivilege_AddReward,

    //时光蛋
    Query_CreateTimeEgg,
    Query_GetTimeEggsByRoleID,
    Query_OpenTimeEgg,
    Query_GetTimeEggs,

    //星恋魔法阵
    Query_Constellation_EnterLobby,
    Query_Constellation_UpdateCardInfo,
    Query_Constellation_MatchCount,
    Query_Constellation_UpdateMatchRoom,

    //任务
    QUERY_Add_Quest,
    QUERY_Update_Quest,

    // 副本
    Query_Insert_MagicLampInfo,
    Query_Update_MagicLampInfo,
    Query_Load_FriendParcloseInfo,
    Query_Update_ParcloseInfo,
    Query_Insert_ParcloseInfo,
    Query_Insert_StarRewardIndex,
    Query_Update_DebrisInfo,
    Query_Update_ThemeParcloseInfo,
    Query_Insert_ThemeParcloseInfo,
    Query_Insert_FriendContactInfo,
    Query_Update_FriendContactInfo,
    Query_Update_ClearGiveGetVitInfo,
    Query_Update_ClearMailineQuickFinishNum,
    Query_Update_ClearThemeQuickFinishNum,
    Query_Update_Offline_FriendContactInfo,


    QUERY_Add_DanceGroupReward,
    QUERY_Del_DanceGroupReward,

    QUERY_AddCompleteAnimation,

    // 清除首次充值状态
    QUERY_ClearFirstPaymentInfo,

    QUERY_Bill_UpdateBindPt, // 更新绑定M币信息

    QUERY_SaveSocialShare,//保存社交分享信息

    QUERY_InsertProcGuideReward,	//领取新手奖励

    // 月卡
    Query_Insert_MonthCardInfo,
    Query_Update_MonthCardInfo,

    QUERY_SaveSpecialMall,          // 保存特殊商店信息
    QUERY_SaveAuctionBid,           // 保存竞价信息
    QUERY_SaveGroupBuyingCount,     // 保存团购人数
    QUERY_SetAuctionStateSuccess,   // 设置拍卖成功
    QUERY_AuctionReturnPoint,       // 返还竞拍M币
    QUERY_GetGroupBuyingCount,      // 查询团购数量
    QUERY_GetLimitedEditionCount,   // 查询限量数量
    QUERY_GetAuctionBidInfo,        // 查询竞价信息

    // 排行榜
    QUERY_Rank_GetRoleData,         // 查询角色排行榜数据
    QUERY_Rank_GetAllDatas,         // 加载排行榜所有数据
    QUERY_Rank_SaveData,            // 记录数据
    QUERY_Rank_UpdateDataState,     // 更新排行榜数据状态
    QUERY_Rank_SaveHistory,         // 记录历史数据

    //GM工具充值（不记录Log）
    QUERY_GMPrivRecharge,

    Query_System_LoadBlackDevice,   // 加载充值黑名单

	QUERY_SendDeviceList,			//GM添加设备黑名单
	
	QUERY_ChangeAccountPassword,	//GM修改玩家密码

    QUERY_DeleteDeviceList,         //GM解封设备黑名单

    // 推荐好友
    QUERY_Recommend_UpdateCount,    // 更新刷新次数
    QUERY_Recommend_UpdateResetTime,// 更新重置时间

    // 活跃度系统
	QUERY_Activeness_UpdateOrInsert,
//    QUERY_Activeness_UpdateScore,   // 达成目标，增加分数
//     QUERY_Activeness_UpdateTarget,  // 更新目标进度
//     QUERY_Activeness_Reward,        // 领取奖励，扣除分数

    // 宠物系统
    QUERY_PetEgg_LoadIncubator,         // 获取孵化器信息
    QUERY_PetEgg_UpdateIncubatorLevel,  // 更新孵化器等级信息
    QUERY_PetEgg_Load,                  // 获取宠物蛋信息
    QUERY_PetEgg_Create,                // 创建宠物蛋信息
    QUERY_PetEgg_UpdateState,           // 更新宠物蛋状态
    QUERY_Pet_LoadExtendGridCount,      // 获取扩展栏位个数
    QUERY_Pet_UpdateExtendGridCount,    // 更新扩展栏位个数
    QUERY_Pet_Create,                   // 创建宠物信息
    QUERY_Pet_ChangeName,               // 修改宠物昵称
    QUERY_Pet_UpdateState,              // 修改宠物状态
    QUERY_Pet_UpdateQuality,            // 修改宠物品质
    QUERY_Pet_UpdateEvolution,          // 修改宠物进化度
    QUERY_Pet_UpdateEndurance,          // 修改宠物耐力
    QUERY_Pet_UpdateLevelAndExp,        // 宠物升级
    QUERY_Pet_UpdateSkill,              // 技能发生变化
    QUERY_Pet_ResetSkill,               // 重置技能
    QUERY_Pet_UpdateGuideStep,          // 更改引导步骤

	// 信息中心加载请求：
	QUERY_InfoCenter_LoadAllInfo,		// 加载所有已经发布的信息
	QUERY_InfoCenter_AddInfo,			// 新发布一条消息
	QUERY_InfoCenter_UpdateInfo,		// 更新一条消息
	QUERY_InfoCenter_RemoveInfo,		// 删除一条信息
	QUERY_InfoCenter_LoadRoleReadInfo,	// 加载玩家已读列表
	QUERY_InfoCenter_UpdateRoleReadInfo,// 写入玩家已读信息

    // 长效累冲
    QUERY_LongactingCumulativeRechagre_UpdateIndex,   // 长效累冲更新领奖序列
	// Experineced cards;
	QUERY_ExperienceCard_UpdateTime,
	QUERY_ExperienceCard_AddExperiencedItem,
    //勋章
    QUERY_Medal_UpdateMedal,                        //更新勋章
    QUERY_Medal_UpdateProgress,                     //更新获得勋章条件的进度
    QUERY_Medal_UpdateScore,                        //更新勋章积分
    QUERY_Medal_DeleteProgress,                     //删除一个条件进度
    QUERY_Medal_DeleteVersionMedal,                 //删除版本勋章
    QUERY_Medal_AddMedalByGm,                       //Gm命令添加一个激活勋章

    //勋章活动
    QUERY_Medal_UpdateActConsumeValue,              //勋章活动期间的消费
    QUERY_Medal_UpdateBuyGiftCount,                 //勋章活动期间的购买指定礼包的数目

    //图鉴系统
    QUERY_Handnbook_LoadGolbalCostumeData,                 //图鉴系统导入全局服饰数据
    QUERY_Handnbook_LoadGolbalEffectData,                  //图鉴系统导入全局特效数据

    QUERY_Handnbook_SaveGlobalEffectData,                  //图鉴系统保存特效数据
    QUERY_Handnbook_SaveGlobalCostumeData,                  //图鉴系统保存全局服饰数据

    QUERY_Handnbook_UpdatePersonalData,                 //图鉴系统个人数据更新
    QUERY_Handnbook_InsertPersonalData,                 //图鉴系统个人数据插入
    
	// 账号迁移系统
	QUERY_AccountTran_CheckTranCode,	// 账号迁移激活码检测
	QUERY_AccountTran_UseTranCode,		// 使用trancode更新account账户
	
    // 舞团试炼
    QUERY_DanceGroupChallenge_ResetChance,          // 重置试炼次数
    QUERY_DanceGroupChallenge_UpdateChance,         // 更新试炼次数
    QUERY_DanceGroupChallenge_ResetBuyChance,       // 重置试炼购买次数
    QUERY_DanceGroupChallenge_UpdateBuyChance,      // 更新试炼购买次数
    QUERY_DanceGroupChallenge_LoadRanking,          // 加载试炼排名
    QUERY_DanceGroupChallenge_UpdateRankingState,   // 更新排名状态
    QUERY_DanceGroupChallenge_RecordRanking,        // 记录排名
    QUERY_DanceGroupChallenge_LoadAllInfo,          // 加载本周试炼信息
    QUERY_DanceGroupChallenge_UpdateActive,         // 更新活力值
    QUERY_DanceGroupChallenge_UpdateContribution,   // 更新贡献
    QUERY_DanceGroupChallenge_OpenStorey,           // 开启试练塔
    QUERY_DanceGroupChallenge_UpdateStoreyScore,    // 更新分数

    // 舞团秘境
    QUERY_DanceGroupFairyLandLoadAllInfo,          // 加载舞团秘境信息
//     QUERY_DanceGroupFairyLandLoadLastStartTime,        // 加载最后活动开启的时间戳
    QUERY_DanceGroupFairyLandUpdateLastStartTime,      // 更新最后活动开启的时间戳
    QUERY_DanceGroupFairyLandUpdateSystemStamp,    // 更新时间戳，

    QUERY_DanceGroupFairyLandDelGroupInfo,         // 删除舞团数据(这里需要删除 GroupInfo 和PersonalInfo)
    QUERY_DanceGroupFairyLandAddGroupInfo,         // 添加舞团数据
    QUERY_DanceGroupFairyLandDelPersonalInfo,      // 删除成员数据
    QUERY_DanceGroupFairyLandAddPersonalInfo,      // 添加成员数据

    QUERY_DanceGroupFairyLandUpdateCurChatper,      // 更新舞团层数
    QUERY_DanceGroupFairyLandUpdateBlassRank,       // 更新舞团祝福榜
    QUERY_DanceGroupFairyLandUpdateTodayRank,       // 更新舞团今日分数榜
    QUERY_DacneGroupFairyLandUpdateBlessInfo,    // 更新舞团祝福结束时间戳

    QUERY_DanceGroupFairyLandUpdateTotalScore,                 //更新个人总分 （这里更新时间戳）
    QUERY_DanceGroupFairyLandClearYesterdayScore,              //清除昨日分数
//    QUERY_DanceGroupFairyLandUpdateLastChallengeChapterLvl,    //更新个最后挑战层数
    QUERY_DanceGroupFairyLandUpdateBlassTimes,                 //更新个人祝福次数
    QUERY_DanceGroupFairyLandUpdateCurChapterChallengeTimes,   //更新当前层数挑战次数
    QUERY_DanceGroupFairyLandUpdateStorageBox,                 //储物箱奖励道具

    QUERY_DanceGroupFairyLandUpdateJoinRewardFlag,             //更新参与奖标志位
    QUERY_DanceGroupFairyLandUpdatePassedRewardFlag,           //更新过关奖标志位
    QUERY_DanceGroupFairyLandUpdatePassedRewardRecord,         //更新过关奖励记录

    QUERY_DanceGroupFairyLandUpdateGroupScore,       //更新分数
    QUERY_DacneGroupFairyLandUpdateBlessEndTime,               //更新祝福结束时间

    QUERY_DanceGroupFairyLandClearAllPassedReward,        // 清除所有过关奖励（删除数据，删除标志位）
    QUERY_DanceGroupFairyLandClearAllStorageBoxReward,    // 清除所有储物箱奖励
    QUERY_DanceGroupFairyNewDay,                          // 

    //log查询类型
    QUERY_TYPE_LOG_MIN,

    QUERY_LogPlayerAction,
    QUERY_LogRoleExp,
    QUERY_LogRoleQuest,
    QUERY_LogRoleAchievement,
    QUERY_LogRoleDancing,
    QUERY_LogRoleCheckIn,
    QUERY_LogRoleSend,
    QUERY_LogRoleChangeBill,
    QUERY_LogRoleChangeBindBill,
    QUERY_LogRoleChangeMoney,
    QUERY_LogRoleItem,
    QUERY_LogRoleChat,
    QUERY_LogRoleAmuse,
    QUERY_LogRoleTimeEgg,
    QUERY_LogDanceGroup,
    QUERY_LogRoleConstellationMatch,
    QUERY_LogRoleVip,
    QUERY_LogRoleMeetActivity,
    QUERY_LogRoleAttrChange,
    QUERY_LogRoleCouple,
    QUERY_LogRoleDebris,
    QUERY_LogRoleVisitRank,
    QUERY_LogRoleAddFriends,
    QUERY_LogRoleActiveness,
    QUERY_LogRoleDungeon,
    QUERY_LogRoleLuckDraw,
    QUERY_LogRoleMall,
    QUERY_LogRolePhotoRoom,
    QUERY_LogRoleAudition,
    QUERY_LogRoleLongacting,
    QUERY_LogRoleCheckingMarkFailed,
    QUERY_LogRoleClothEffect,
    QUERY_LogRoleClothEffectProgress,
    QUERY_LogRoleMedalLog,
    QUERY_LogRoleRedEnvelopeLog,
    QUERY_LogRoleMagicArray,
    QUERY_LogRoleSwicthLine,
    QUERY_LogRoleBigMamaDanceRecord,
    QUERY_LogGroupBigMamaDanceRecord,
    QUERY_LogPlayerType,

    QUERY_TYPE_LOG_MAX,                 // log查询类型
};

class CQueryLoad
{
public:
    int m_nTimes;//调用次数
    int m_nCost;//耗时（毫秒）
    CQueryLoad()
    {
        m_nTimes = 0;
        m_nCost = 0;
    }
    void operator += (int nCostTime)
    {
        m_nTimes++;
        m_nCost += nCostTime;
    }
protected:
private:
};
class CQueryManager : public CQueryManagerBase
{
public:
    CQueryManager();
    virtual ~CQueryManager();

public:
    bool OpenDatabase(bool bOpenAccountDB = true, bool bOpenRoleDB = true, bool bOpenLogDB = false, bool bOpenSubLogDB = false);

    bool OpenBackupAccountDB();

    int ProcessQuery(QueryBase* &pQuery);
    int GetQueueSize();
    bool IsQueueEmpty();
    int GetThreadState();
    bool RecreateThread();

    virtual void Release();

    virtual void AddQuery(int nQueryType, int nAttachment = 0, void* pAttachment = NULL, __int64 nTag = 0, void* pTag = NULL, CDelHelper* pDelHelper = NULL);
    virtual void AddQuery(QueryBase *query);

    template <typename T>
    void AddQuery( int nQueryType, SharedPtr<T> sharedAttachment, int nAttachment = 0, __int64 nTag = 0 )
    {
        QueryBase* pQuery = new Query<T>( nQueryType, sharedAttachment );
        pQuery->m_nAttachment = nAttachment;
        pQuery->m_nTag = nTag;
        AddQuery( pQuery );
    }

    QueryBase* GetLogQuery();

protected:
#ifdef WIN32
    static DWORD WINAPI LogQueryThreadProc(void* pParam);
#else
    static void* LogQueryThreadProc(void* pParam);
#endif

protected:
#ifdef WIN32
    HANDLE m_hThreadLogQuery;
    CRITICAL_SECTION m_lockLogQuery;
    HANDLE m_hEventLogQuery;
#else
    pthread_t m_hThreadLogQuery;
    pthread_mutex_t m_lockLogQuery; 
    pthread_cond_t  m_condLogQuery;
    pthread_mutex_t *m_pMutexLogQuery; 
    pthread_cond_t  *m_pCondLogQuery;
#endif

    // 日志类查询队列，用于处理log DB的查询请求，由日志查询线程和逻辑线程使用
    // 查询的结果会存入正常的m_apQueryFinishedQueue中
    list< QueryBase* >m_apLogQueryQueue;

protected:
    CDBInterface m_dbiRole;
    CDBInterface m_dbiAccount;
    CDBInterface m_dbiLog;
    CDBInterface m_dbiSubLog;

    bool m_bEnableBackupAccountDB;            // 是否开启另一个AccountDB连接
    CDBInterface m_dbiBackupAccountDB;        // 另一个AccountDB，临时用于iOS开新区账号创建和校验

    time_t m_timeRecord;//记录队列长度的时间
    //map< int, CQueryLoad > mapQueryLoad;
private:
    int m_nLotteryMode;
};

#endif // __QUERYMANAGER_H__


//file end

