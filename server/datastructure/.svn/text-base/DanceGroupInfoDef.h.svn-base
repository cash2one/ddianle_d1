#ifndef	__DANCE_GROUP_INFO_DEF_H__
#define __DANCE_GROUP_INFO_DEF_H__

const unsigned short cMAX_DANCE_GROUP_NAME_LEN					= 40;		//舞团名称最大长度
const unsigned short cMAX_DANCE_GROUP_TITILE_NAME_LEN			= 40;		//舞团职位名称最大长度
const unsigned short cMAX_DANCE_GROUP_PROFILE_LEN				= 500;		//舞团简介最大长度
const unsigned short cMAX_DANCE_GROUP_ANNOUNCEMENT_LEN			= 500;		//舞团公告最大长度

const unsigned int cMAX_DANCE_GROUP_HONOR						= 999999999;
const unsigned int cMAX_ROLE_CONTRIBUTION						= 999999999;

const unsigned int cMAX_GROUP_RES_A								= 999999999;	//舞团资源A上限
const unsigned int cMAX_GROUP_RES_B								= 999999999;	//舞团资源B上限
const unsigned int cMAX_GROUP_RES_C								= 999999999;	//舞团资源C上限

const unsigned int cITEM_TYPE_RES_A										= 31012;	//物品资源A									
const unsigned int cITEM_TYPE_RES_B										= 31013;	//物品资源B								
const unsigned int cITEM_TYPE_RES_C										= 31014;	//物品资源C							
const unsigned int cITEM_CHANGE_DANCE_GOURP_NAME_CARD					    = 31023;	//物品舞团卡

const int cCHANGE_DANCE_GROUP_NAME_CD_TIME						= 24 * 60 * 60;

const unsigned short cMAX_DANCE_GROUP_LEADER_NUM				= 1;		//舞团最多团长数
const unsigned short cMAX_DANCE_GROUP_VICE_LEADER_NUM			= 2;		//舞团最多副团长数
const unsigned short cMAX_DANCE_GROUP_ASSISTANT_NUM				= 4;		//舞团最多团务数

const unsigned short cMIN_LEVEL_FOR_CREATE_DANCE_GROUP			= 10;		//创建舞团者的最小等级需求
const unsigned int   cITEM_TYPE_FOR_CREATE_DANCE_GROUP			= 31010;	//创建舞团所需要的物品

const unsigned int cREQUEST_ENTER_DANCE_GROUP_EXPIRED_TIME		= 259200;	//申请加入舞团的过期时间－72小时

const unsigned short cMAX_REQUEST_ENTER_DANCE_GROUP_COUNT		= 5;		//每个玩家最多可同时申请的舞团数
const unsigned short cMAX_REQUEST_ENTER_DANCE_GROUP_QUEUE_NUM	= 100;		//每个舞团的的最大可申请数

const unsigned int cDANCE_GROUP_DISMISS_RETENTION_TIME			= 604800;	//解散舞团的保留期－7天
const unsigned int cDANCE_GROUP_DISMISS_CD_TIME					= 86400;	//解散功能使用CD－24小时

#define MIN_CHALLENGE_STOREY    1           // 舞团试炼最小层数
#define MAX_CHALLENGE_ACTIVE    99999999    // 舞团试炼活力值上限

enum EDanceGroupBaseInfoUpdateType
{
	EDanceGroupBaseInfoUpdateType_Data,
	EDanceGroupBaseInfoUpdateType_Announcement,
    EDanceGroupBaseInfoUpdateType_Profile,
    EDanceGroupBaseInfoUpdateType_CermonyTimes,                             //入团仪式举办次数
};

enum EGroupTitle
{
	EGroupTitle_JuniorMember,	//见习团员
	EGroupTitle_SeniorMember,	//正常团员
	EGroupTitle_Assistant,		//团务
	EGroupTitle_ViceLeader,		//副团长
	EGroupTitle_Leader,			//团长
};

enum EGroupRight
{
    EGroupRight_Announcement,   // 修改公告
    EGroupRight_ModifyInfo,     // 信息修改
    EGroupRight_MemberMgr,      // 成员管理
    EGroupRight_AddMember,      // 添加成员(审核、邀请)
    EGroupRight_Dismiss,        // 解散
	EGroupRight_Ceremony,		// 举办入团仪式
    EGroupRight_OpenChallenge,  // 开启舞团试炼
	EGroupRight_Max,
};

static bool s_bTitleRight[] = 
{
	//修改公告,	信息修改,	成员管理,	添加成员,	解散,	举办入团仪式  开启舞团试炼
	false,		false,		false,		false,		false,	false,	      false,    //见习团员
	false,		false,		false,		false,		false,	false,	      false,    //正常团员
	true,		false,		false,		true,		false,	false,	      false,    //团务
	true,		false,		true,		true,		false,	true,	      true,     //副团长
	true,		true,		true,		true,		true,	true,	      true,     //团长
};

inline bool CheckRight(unsigned char nTitle, unsigned char nRight)
{
	if (nTitle > EGroupTitle_Leader)
	{
		return false;
	}
	if (nRight >= EGroupRight_Max)
	{
		return false;
	}
	return s_bTitleRight[nTitle*(EGroupRight_Max) + nRight];
}

//是否具有添加玩家权限(审核、邀请)
inline bool HasAddMemberAuthority(unsigned char nTitle) { return CheckRight(nTitle, EGroupRight_AddMember); }
//是否具有改变职位权限
inline bool CanChangeTitle(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_MemberMgr);}
//是否具有开除玩家权限
inline bool CanKickOut(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_MemberMgr);}
//是否具有修改颜色的权限
inline bool CanUpdateColor(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_ModifyInfo);}
//是否具有修改徽章的权限
inline bool CanUpdateBadge(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_ModifyInfo);}
//是否具有修改职位名称的权限
inline bool CanUpdateTitleName(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_ModifyInfo);}
//是否具有修改简介的权限
inline bool CanUpdateProfile(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_ModifyInfo);}
//是否具有修改公告的权限
inline bool CanUpdateAnnouncement(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_Announcement);}
//是否具有解散的权限
inline bool CanDismiss(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_Dismiss);}
//是否可以举办入团仪式
inline bool CanRunCeremony(unsigned char nTitle){return CheckRight(nTitle, EGroupRight_Ceremony);}
inline bool CanOpenChallenge( unsigned char nTitle ) { return CheckRight( nTitle, EGroupRight_OpenChallenge ); }

enum EPlayerState
{
	EPlayerState_Begin,
	EPlayerState_Offline = EPlayerState_Begin,	//离线
	EPlayerState_Idle,							//空闲
	EPlayerState_Waiting,						//等待
	EPlayerState_Audience,						//观战
	EPlayerState_Dance,							//比赛
	EPlayerState_End
};

//创建舞团返回
enum ECreateDanceGroupResult
{
	ECreateDanceGroupResult_Success,			//创建成功
	ECreateDanceGroupResult_HaveAGroup,			//已经在一个舞团中
	ECreateDanceGroupResult_LevelLimit,			//等级不到
	ECreateDanceGroupResult_NoCreateItem,		//没有创建舞团物品
	ECreateDanceGroupResult_NameDuplicated,		//名称已经被使用
	ECreateDanceGroupResult_Error,				//系统错误
	ECreateDanceGroupResult_InvalidName,		//非法名字
	ECreateDanceGroupResult_Initializing,		// 初始化中
    ECreateDanceGroupResult_CreateCD,           // 舞团创建冷却中
};
//申请加入舞团返回
enum ERequestEnterDanceGroupResult
{
	ERequestEnterDanceGroupResult_Success,				//申请成功
	ERequestEnterDanceGroupResult_GroupNotExist,		//申请的舞团不存在
	ERequestEnterDanceGroupResult_HaveAGroup,			//已经在一个舞团中
	ERequestEnterDanceGroupResult_HaveRequested,		//已经提交过申请
	ERequestEnterDanceGroupResult_MaxRequestCount,		//同时申请次数过多
	ERequestEnterDanceGroupResult_DisableRequest,		//舞团拒绝申请
    ERequestEnterDanceGroupResult_RequestQueueFull,		//舞团审核名单已满
    ERequestEnterDanceGroupResult_DataLoaded,		    //数据正在加载中
    ERequestEnterDanceGroupResult_CDRepeatGroup,		//加入老团冷却时间
    ERequestEnterDanceGroupResult_CDNewGroup,		    //加入新团冷却时间
};
//申请加入舞团返回
enum ECancelRequestEnterDanceGroupResult
{
	ECancelRequestEnterDanceGroupResult_Success,				//取消申请成功
	ECancelRequestEnterDanceGroupResult_NoRequestInfo,			//无申请记录
	ECancelRequestEnterDanceGroupResult_GroupNotExist,			//取消申请的舞团不存在
};
//获取舞团信息返回
enum EGetDanceGroupInfoResult
{
	EGetDanceGroupInfoResult_Success,				//成功
	EGetDanceGroupInfoResult_NotExist,				//舞团不存在
};
//批复申请回复
enum EReplyRequestEnterDanceGroupResult
{
	EReplyRequestEnterDanceGroupResult_Success,				//批复成功
	EReplyRequestEnterDanceGroupResult_NotInGroup,			//批复者未加入任何舞团
	EReplyRequestEnterDanceGroupResult_NotInGroupErr,		//批复者未在舞团中
	EReplyRequestEnterDanceGroupResult_GroupNotExist,		//舞团不存在
	EReplyRequestEnterDanceGroupResult_NoAuthority,			//无权限
	EReplyRequestEnterDanceGroupResult_NoInRequestQueue,	//不在申请列表中
	EReplyRequestEnterDanceGroupResult_MaxCount,			//舞團人員已經到達上限
};
//退出舞团回复
enum EExitDanceGroupResult
{
	EExitDanceGroupResult_Success,				//成功退出
	EExitDanceGroupResult_NotInGroup,			//未加入任何舞团
	EExitDanceGroupResult_NotInGroupErr,		//批复者未在舞团中
	EExitDanceGroupResult_GroupNotExist,		//舞团不存在
	EExitDanceGroupResult_Leader,				//团长不允许退出
};
//舞团传位回复
enum EChangeDanceGroupLeaderResult
{
	EChangeDanceGroupLeaderResult_Success,				//成功
	EChangeDanceGroupLeaderResult_NotInGroup,			//发起人未加入任何舞团
	EChangeDanceGroupLeaderResult_GroupNotExist,		//舞团不存在
	EChangeDanceGroupLeaderResult_LeaderNotMember,		//发起人不是舞团成员
	EChangeDanceGroupLeaderResult_NewLeaderNotMember,	//新团长不是舞团成员
	EChangeDanceGroupLeaderResult_LeaderIsNotLeader,	//发起人不是舞团团长
    EChangeDanceGroupLeaderResult_ContributionNotEnough,//被传位者贡献度不足
    EChangeDanceGroupLeaderResult_CanNotMengXin,        //不能是小萌新
};
//解散舞团回复
enum EDismissDanceGroupResult
{
	EDismissDanceGroupResult_Success,			//成功
	EDismissDanceGroupResult_NotInGroup,		//发起人未加入任何舞团
	EDismissDanceGroupResult_GroupNotExist,		//舞团不存在
	EDismissDanceGroupResult_IsDismissed,		//舞团已经解散
	EDismissDanceGroupResult_IsInDismissCD,		//还在解散CD中
	EDismissDanceGroupResult_NotGroupMember,	//发起人不是舞团成员
	EDismissDanceGroupResult_NotLeader,			//发起人不是舞团团长
};

//取消解散舞团回复
enum ECancelDismissDanceGroupResult
{
	ECancelDismissDanceGroupResult_Success,			//成功
	ECancelDismissDanceGroupResult_NotInGroup,		//发起人未加入任何舞团
	ECancelDismissDanceGroupResult_GroupNotExist,	//舞团不存在
	ECancelDismissDanceGroupResult_NotDismissed,	//舞团未解散
	ECancelDismissDanceGroupResult_InValid,			//舞团已经彻底解散
	ECancelDismissDanceGroupResult_NotGroupMember,	//发起人不是舞团成员
	ECancelDismissDanceGroupResult_NotLeader,		//发起人不是舞团团长
};

//改变舞团职位回复
enum EChangeDanceGroupTitleResult
{
	EChangeDanceGroupTitleResult_Success,				//成功
	EChangeDanceGroupTitleResult_NotInGroup,			//操作者未加入任何舞团
	EChangeDanceGroupTitleResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupTitleResult_NotGroupMember,		//操作者不是舞团成员
	EChangeDanceGroupTitleResult_TargetNotGroupMember,	//操作对象不是舞团成员
	EChangeDanceGroupTitleResult_NoAuthority,			//没有操作权限
	EChangeDanceGroupTitleResult_LowTitle,				//不能对同级及上级进行操作
	EChangeDanceGroupTitleResult_TitleError,			//职位错误
	EChangeDanceGroupTitleResult_Limited,				//操作职位比自己职位高
	EChangeDanceGroupTitleResult_TitleSame,				//职位没变动
	EChangeDanceGroupTitleResult_Full,					//该职位已经满员
    EChangeDanceGroupTitleResult_CanNotMengxin,            // 小萌新不能做职位变动
};
//开除舞团成员回复
enum EKickOutDanceGroupMemberResult
{
	EKickOutDanceGroupMemberResult_Success,					//成功
	EKickOutDanceGroupMemberResult_NotInGroup,				//操作者未加入任何舞团
	EKickOutDanceGroupMemberResult_GroupNotExist,			//舞团不存在
	EKickOutDanceGroupMemberResult_NotGroupMember,			//操作者不是舞团成员
	EKickOutDanceGroupMemberResult_TargetNotGroupMember,	//操作对象不是舞团成员
	EKickOutDanceGroupMemberResult_NoAuthority,				//没有操作权限
	EKickOutDanceGroupMemberResult_LowTitle,				//不能对同级及上级进行操作
};
//修改舞团颜色回复
enum EChangeDanceGroupColorResult
{
	EChangeDanceGroupColorResult_Success,				//成功
	EChangeDanceGroupColorResult_NotInGroup,			//未加入任何舞团
	EChangeDanceGroupColorResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupColorResult_NotGroupMember,		//不是舞团成员
	EChangeDanceGroupColorResult_NoAuthority,			//没有操作权限
};

//修改舞团徽章回复
enum EChangeDanceGroupBadgeResult
{
	EChangeDanceGroupBadgeResult_Success,				//成功
	EChangeDanceGroupBadgeResult_NotInGroup,			//未加入任何舞团
	EChangeDanceGroupBadgeResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupBadgeResult_NotGroupMember,		//不是舞团成员
	EChangeDanceGroupBadgeResult_NoAuthority,			//没有操作权限
};
//修改舞团职位名称回复
enum EChangeDanceGroupTitleNameResult
{
	EChangeDanceGroupTitleNameResult_Success,				//成功
	EChangeDanceGroupTitleNameResult_NotInGroup,			//未加入任何舞团
	EChangeDanceGroupTitleNameResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupTitleNameResult_NotGroupMember,		//不是舞团成员
	EChangeDanceGroupTitleNameResult_NoAuthority,			//没有操作权限
};
//修改舞团简介回复
enum EChangeDanceGroupProfileResult
{
	EChangeDanceGroupProfileResult_Success,				//成功
	EChangeDanceGroupProfileResult_NotInGroup,			//未加入任何舞团
	EChangeDanceGroupProfileResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupProfileResult_NotGroupMember,		//不是舞团成员
	EChangeDanceGroupProfileResult_NoAuthority,			//没有操作权限
};
//修改舞团公告回复
enum EChangeDanceGroupAnnouncementResult
{
	EChangeDanceGroupAnnouncementResult_Success,				//成功
	EChangeDanceGroupAnnouncementResult_NotInGroup,			//未加入任何舞团
	EChangeDanceGroupAnnouncementResult_GroupNotExist,			//舞团不存在
	EChangeDanceGroupAnnouncementResult_NotGroupMember,		//不是舞团成员
	EChangeDanceGroupAnnouncementResult_NoAuthority,			//没有操作权限
};

//捐赠舞团资源
enum EDonateDanceGroupResourceResult
{
	EDonateDanceGroupResourceResult_Success,						//成功
	EDonateDanceGroupResourceResult_NoInGroup,						//未加入任何舞团
	EDonateDanceGroupResourceResult_GroupNotExist,					//舞团不存在
	EDonateDanceGroupResourceResult_NotGroupMember,					//不是舞团成员
	EDonateDanceGroupResourceResult_NotEnoughResource,				//捐赠的资源不足
	EDonateDanceGroupResourceResult_DanceGroupResourceExceedLimit,	//捐赠的资源使舞团资源超出上限
};

//舞团升级
enum EUpgradeGroupDanceResult
{
	EUpgradeGroupDanceResult_Success,								//成功
	EUpgradeGroupDanceResult_NoInGroup,								//未加入任何舞团
	EUpgradeGroupDanceResult_GroupNotExist,							//舞团不存在
	EUpgradeGroupDanceResult_NotGroupMember,						//不是舞团成员
	EUpgradeGroupDanceResult_NoAuthority,							//无权限
	EUpgradeGroupDanceResult_GroupDanceIsMaxLevel,					//舞团等级已经是或超过最高等级
	EUpgradeGroupDanceResult_NotEnoughLevelUpRequireResource,		//升级舞团等级所需资源不足
	EUpgradeGroupDanceResult_Unkonw,								//未知原因(静态数据有误)
};

//舞团改名
enum EChangeDanceGroupNameResult
{
	EChangeDanceGroupNameResult_Success,							//成功
	EChangeDanceGroupNameResult_NoInGroup,							//未加入任何舞团
	EChangeDanceGroupNameResult_GroupNotExist,						//舞团不存在
	EChangeDanceGroupNameResult_NotGroupMember,						//不是舞团成员
	EChangeDanceGroupNameResult_NoAuthority,						//无权限
	EChangeDanceGroupNameResult_InvalidName,						//名称不合法
	EChangeDanceGroupNameResult_CD,									//冷却中
	EChangeDanceGroupNameResult_NoRequiredItem,						//没有舞团改名卡物品
	EChangeDanceGroupNameResult_NameDuplicated,						//舞团名字已存在
	EChangeDanceGroupNameResult_Unkown,								//未知原因
};

//舞团团徽
enum EDanceGroupBadgeResult
{
	EDanceGroupBadgeResult_Success, //成功
	EDanceGroupBadgeResult_NoInDG,
	EDanceGroupBadgeResult_DGNotExist,
	EDanceGroupBadgeResult_NoPrivilege,
	EDanceGroupBadgeResult_NotExistBadgeOrEffect, //不存在该Id对应的信息
	EDanceGroupBadgeResult_NoEnoughRes,//没有足够的资源来解锁
	EDanceGroupBadgeResult_BadgeLock, //徽章未解锁
	EDanceGroupBadgeResult_EffectLock,//特效未解锁
	EDanceGroupBadgeResult_EffectHasUnlock,//特效已经解锁
	EDanceGroupBadgeResult_Notify,
};

enum EUnlockDanceGroupBadgeType
{
	EUnlockDanceGroupBadgeType_Badge = 1,
	EUnlockDanceGroupBadgeType_Effect = 2,
};

#define DANCEGROUP_ACTIVITY_STAR_MAX_NUM 5 //最大星星个数
//舞团活动（摘星星）
enum EDanceGroupActivityStarResult
{
	EDanceGroupActivityStar_Success, //成功
	EDanceGroupActivityStar_NoInDG,//未加入如何舞团
	EDanceGroupActivityStar_DGNotExist,//舞团不存在
	EDanceGroupActivityStar_NotGroupMember, //不是舞团成员
	EDanceGroupActivityStar_NoCount, //操作次数达到上限
	EDanceGroupActivityStar_MaxStar, //星星已经被摘完
	EDanceGroupActivityStar_PacketOpened, //礼包已经被打开过了
	EDanceGroupActivityStar_NoEnoughMoney,
	EDanceGroupActivtyStar_NoEnoughMCoin,
	EDanceGroupActivityStar_JoinDGSameDay, //当前加入舞团
    EDanceGroupActivityStar_Failed,
    EDanceGroupActivityStar_CanNotMengxin,
};

//操作类型
enum EDanceGroupActivityStarType
{
    EDanceGroupActivityStarType_Normal      = 1,    // Free
    EDanceGroupActivityStarType_Money       = 2,    // Money
    EDanceGroupActivityStarType_Point       = 3,    // Point
    EDanceGroupActivityStarType_Point2Money = 4,
};

enum EDanceGroupShopResult
{
	EDanceGroupShopResult_Success,
	EDanceGroupShopResult_NoInDG,//未加入如何舞团
	EDanceGroupShopResult_DGNotExist,//舞团不存在
	EDanceGroupShopResult_NotGroupMember, //不是舞团成员
	EDanceGroupShopResult_NotEnoughContribution,
	EDanceGroupShopResult_LimitExchangCount, //兑换次数达到上限
	EDanceGroupShopResult_Failed,
    EDanceGroupShopResult_ShouldNotMengXin,     // 
    EDanceGroupShopResult_MoneyNotEnough,     // 钱不足
    EDanceGroupShopResult_ItemNotEnough,     //  物品不足


};

enum EDanceGroupRecrodsResult
{
	EDanceGroupRecrodsResult_Success,
	EDanceGroupRecrodsResult_NoInDG,
	EDanceGroupRecrodsResult_DGNotExist,
	EDanceGroupRecrodsResult_NotGroupMember,
	EDanceGroupRecrodsResult_Failed
};

// 邀请舞团成员结果
enum EInviteDanceGroupMemberResult
{
    EInviteDanceGroupMemberResult_Success,
    EInviteDanceGroupMemberResult_Initializing,             // 初始化中
    EInviteDanceGroupMemberResult_NotInGroup,               // 邀请者未加入舞团
    EInviteDanceGroupMemberResult_NoAuthority,              // 邀请者无权限
    EInviteDanceGroupMemberResult_MaxCount,                 // 舞团人员已满
    EInviteDanceGroupMemberResult_NotInSameLine,            // 不在同一线
    EInviteDanceGroupMemberResult_InviteeHasGroup,          // 被邀请者已加入舞团
    EInviteDanceGroupMemberResult_InBlackList,              // 在被邀请者黑名单中
    EInviteDanceGroupMemberResult_InCD,                     // 处在加团冷却中
};

// 接受舞团成员邀请结果
enum EAcceptDanceGroupMemberInvitationResult
{
    EAcceptDanceGroupMemberInvitationResult_Success,
    EAcceptDanceGroupMemberInvitationResult_GroupNotExist,  // 邀请舞团不存在
    EAcceptDanceGroupMemberInvitationResult_MaxCount,       // 舞团人员已满
    EAcceptDanceGroupMemberInvitationResult_HasGroup,       // 已加入舞团
};

// 舞团试炼目标类型
enum EDanceGroupChallengeTargetType
{
    EDanceGroupChallengeTargetType_Score = 0,
    EDanceGroupChallengeTargetType_Perfect,
    EDanceGroupChallengeTargetType_Max
};

// 舞团试炼塔状态
enum EDanceGroupChallengeStoreyState
{
    EDanceGroupChallengeStoreyState_NotStart = 0,   // 未开启
    EDanceGroupChallengeStoreyState_Challenging,    // 试炼中
    EDanceGroupChallengeStoreyState_Finished,       // 已完成
};

// 舞团试炼排名类型
enum EDanceGroupChallengeRankType
{
    EDanceGroupChallengeRankType_ScoreRank = 0, // 贡献榜
    EDanceGroupChallengeRankType_InspireRank,   // 鼓舞榜
    EDanceGroupChallengeRankType_Max
};

#endif


