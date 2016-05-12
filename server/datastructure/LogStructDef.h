#ifndef __LOG_STRUCT_DEF_H__
#define __LOG_STRUCT_DEF_H__

#include "Macro_Define.h"
#include "../socket/Windefine.h"
#include "DataStruct_Base.h"
#include <string>

class ISQLLog
{
public:
    ISQLLog(){}
    virtual ~ISQLLog(){}
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen) = 0;
};
//角色登陆登出
class CPlayerActionLog : public ISQLLog
{
public:
    enum EPlayerAction
    {
        EPlayerAction_Unknown,	//未知
        EPlayerAction_Login,	//登陆
        EPlayerAction_Logout	//登出
    };
    CPlayerActionLog();
    ~CPlayerActionLog();
    unsigned int m_nRoleID;			//角色编号
    EPlayerAction m_ePlayerAction;	//动作
    int m_nMoney;			//G币
    unsigned int m_nCash;			//M币

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
}; 

// 角色切线
class CPlayerSwitchLineLog : public ISQLLog
{
public:
    CPlayerSwitchLineLog();
    virtual ~CPlayerSwitchLineLog();

    enum EPlayerAction
    {
        EPlayerAction_Unknown,	//未知
        EPlayerAction_Login,	//登陆
        EPlayerAction_Logout	//登出
    };

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EPlayerAction       m_eAction;
    unsigned int        m_nLine;
};

//角色经验
class CRoleExpLog : public ISQLLog
{
public:
    enum EAddExpCause
    {
        EAddExpCause_Unknown,		//未知
        EAddExpCause_Quest,			//从任务中获得
        EAddExpCause_Match,			//从比赛中获得
        EAddExpCause_Item,			//从道具获得
        EAddExpCause_GMCommand,		//GM命令获得
        EAddExpCause_AmuseRoom,		//开放场景获得
    };
    CRoleExpLog();
    ~CRoleExpLog();
    unsigned int m_nRoleID;			//角色编号
    EAddExpCause m_eAddExpCause;	//增加经验的原因
    unsigned int m_nAddExp;			//增加的经验
    unsigned int m_nNewLevel;		//新的等级
    unsigned int m_nNewExp;			//新的经验值

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//角色任务
class CRoleQuestLog : public ISQLLog
{
public:
    CRoleQuestLog();
    ~CRoleQuestLog();
    unsigned int m_nRoleID;			//角色编号
    unsigned int m_nQuestID;		//任务编号

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//角色成就
class CRoleAchievementLog : public ISQLLog
{
public:
    enum EAchievementAction
    {
        EAchievementAction_Unknown,		//未知
        EAchievementAction_Complete,	//完成成就
        EAchievementAction_GetReward,	//领取成就奖励
    };
    CRoleAchievementLog();
    ~CRoleAchievementLog();
    unsigned int m_nRoleID;						//角色编号
    EAchievementAction m_eAchievementAction;	//成就动作
    unsigned int m_nAchievementID;				//成就编号

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//角色跳舞
class CRoleDancingLog : public ISQLLog
{
public:
    CRoleDancingLog();
    ~CRoleDancingLog();

    enum EDancingAction
    {
        EDancingAction_Unknown,		//未知状态
        EDancingAction_Start,		//开始跳舞
        EDancingAction_End			//结束跳舞
    };

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;          //角色编号
    EDancingAction      m_eDancingAction;   //跳舞动作
    int                 m_nScene;           //场景
    int                 m_nMusicID;         //歌曲编号
    int                 m_nMusicMode;       //模式
    int                 m_nMusicLevel;      //难度
    int                 m_nPlayerCount;     //玩家数量
    int                 m_nRank;            //排名
    int                 m_nScore;           //得分
    int                 m_nTeamMode;        //组队模式
    bool                m_bIsDungeon;       //是否是魔法神灯副本
};

//角色签到
class CRoleCheckInLog : public ISQLLog
{
public:
    enum ECheckInAction
    {
        ECheckInAction_Unknown,		//未知状态
        ECheckInAction_Check,		//签到
        ECheckInAction_Recheck,		//补签
    };
    CRoleCheckInLog();
    ~CRoleCheckInLog();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;			//角色编号
    ECheckInAction m_eCheckInAction;//签到动作
    bool m_bIsVip;
    unsigned int m_nVipLevel;
    unsigned int m_nDayIndex;
};

//角色赠送
class CRoleSendLog : public ISQLLog
{
public:
    enum ESendType
    {
        ESendType_Unknown,		//未知状态
        ESendType_Buy,			//购买赠送
        ESendType_Wish,			//许愿赠送
        ESendType_Ask,			//索要赠送
    };
    CRoleSendLog();
    ~CRoleSendLog();
    unsigned int m_nRoleID;			//角色编号
    ESendType m_eSendType;			//签到动作
    itemtype_t m_nItemType;		    //物品类型
    unsigned short m_nItemCount;	//物品数量
    int m_nDuration;				//物品期限
    unsigned int m_nTargetRoleID;	//受赠人编号
    __int64 m_nMailID;				//邮件编号

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//角色改变M币值的Log
class CRoleChangeBillLog : public ISQLLog
{
public:
    CRoleChangeBillLog();
    ~CRoleChangeBillLog();

    unsigned int m_nRoleID;					//角色编号
    int m_nChangeValue;						//改变的值
    EChangeBillCause m_eChangeBillCause;	//改变原因
    unsigned int m_nNewPT;					//改变后的M币值

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

//角色改变绑定M币值的Log
class CRoleChangeBindBillLog : public ISQLLog
{
public:
    CRoleChangeBindBillLog();
    ~CRoleChangeBindBillLog();

    unsigned int m_nRoleID;					//角色编号
    int m_nChangeValue;						//改变的值
    EChangeBindBillCause m_eChangeBindBillCause;//改变原因
    unsigned int m_nNewBindPT;				//改变后的绑定M币值

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};


//角色改变金券值的Log
class CRoleChangeMoneyLog : public ISQLLog
{
public:
    CRoleChangeMoneyLog();
    ~CRoleChangeMoneyLog();

    unsigned int m_nRoleID;					//角色编号
    int m_nChangeValue;						//改变的值
    EChangeMoneyCause m_eChangeMoneyCause;	//改变原因
    unsigned int m_nNewMoney;				//改变后的金券值

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
};

class CRoleItemLog : public ISQLLog
{
public:
    CRoleItemLog();
    ~CRoleItemLog();
    unsigned int m_nRoleID;				//角色编号
    EItemAction m_eItemAction;			//动作
    EItemColumn m_eItemColumn;			//物品栏位
    __int64 m_nItemID;					//物品唯一编号
    itemtype_t m_nItemType;			    //物品类型
    unsigned short m_nOldCount;			//旧物品数量
    int m_nOldDuration;					//旧物品期限
    unsigned short m_nNewCount;			//新物品数量
    int m_nNewDuration;					//新物品期限
    __int64 m_nAdditionalValue;			//附加值

    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

};

class CRoleChatLog : public ISQLLog
{
public:
    CRoleChatLog();
    ~CRoleChatLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    eChatChannel m_eChannel;
    bool m_bVoiceChat;
    std::string m_strText;
};

class CRoleAmuseLog : public ISQLLog
{
public:
    enum EAction
    {
        EAction_None,
        EAction_Enter,
        EAction_Exit
    };
public:
    CRoleAmuseLog();
    ~CRoleAmuseLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    int m_nSceneID;
    EAction m_nAction;
    int m_nStayTime;
};
class CRoleTimeEggLog : public ISQLLog
{
public:
    enum ETimeEggAction
    {
        ETimeEggAction_None,
        ETimeEggAction_Create,
        ETimeEggAction_Open
    };
public:
    CRoleTimeEggLog();
    ~CRoleTimeEggLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    ETimeEggAction m_eEggAction;
    int m_nSceneID;
};

//舞团log
enum EDanceGroupLogAction
{
    EDanceGroupLog_None = 0,
    EDanceGroupLog_Create = 1,//创建舞团
    EDanceGroupLog_Destory, //销毁舞团
    EDanceGroupLog_MemberJoin,//成员加入
    EDanceGroupLog_MemberLeave,//成员退出
    EDanceGroupLog_MemberKicked,//开除成员
    EDanceGroupLog_MemberContributionAdd,//成员贡献度增加
    EDanceGroupLog_MemberContributionDec,//成员贡献度减少
    EDanceGroupLog_MemberIntergralAdd,//成员积分增加
    EDanceGroupLog_HonourAdd,//舞团荣誉增加
    EDanceGroupLog_HonourDec,//舞团荣誉减少
    EDanceGroupLog_Levelup,//舞团升级
    EDanceGroupLog_ResConsume,//舞团资源消耗
    EDanceGroupLog_ResDonate,//舞团资源捐献
    EDanceGroupLog_LeaderChange,//团长变更
    EDanceGroupLog_MemberTitleChange,//舞团成员职位变更
    EDanceGroupLog_NameChange,//舞团名称变更
    EDanceGroupLog_ProptyChange, // 小萌新变更
    EDanceGroupLog_Activity_PowerAdd, //舞团活动（星星动力之增加）
    EDanceGroupLog_ChallengeActiveAdd,  // 试炼活力值增加
    EDanceGroupLog_ChallengeActiveDec,  // 试炼活力值减少
};
class CDanceGroupLog : public ISQLLog
{
public:
    CDanceGroupLog();
    virtual ~CDanceGroupLog(){}

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    //使用时注意参数个数
    void ValuesToStr(EDanceGroupLogAction eAction, va_list ap);

public:
    EDanceGroupLogAction m_eAction;
    unsigned int m_nGroupId;
    std::string m_strDGName;
    unsigned int m_nRoleId;
    std::string m_strRoleName;
    std::string m_strParam;
};

//////////////////////////////////////////////////////////////////////////
class CRoleConstellationMatchLog : public ISQLLog
{
public:
    enum EConstellationMatch
    {
        EConstellationMatchAction_None,
        EConstellationMatchAction_EnterLobby,	// 进入星恋魔法阵大厅
        EConstellationMatchAction_UseCard,		// 使用牌进行配对
        EConstellationMatchAction_MatchFail,	// 配对失败
        EConstellationMatchAction_MatchSuc,		// 配对成功
        EConstellationMatchAction_EnterRoom,	// 进入场景房间
    };
public:
    CRoleConstellationMatchLog();
    virtual ~CRoleConstellationMatchLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    EConstellationMatch m_eAction;
    int m_nCardConstellation;
    unsigned int m_nMatchRoleID;
    std::string m_strMatchRoleName;
    int m_nSceneID;
    std::string m_strSceneName;
};

//////////////////////////////////////////////////////////////////////////
class CRoleVipLog : public ISQLLog
{
public:
    enum ERoleVipAction
    {
        ERoleVipAction_None,
        ERoleVipAction_OpenVip,
        ERoleVipAction_LevelUp,
        ERoleVipAction_AddExp,
        ERoleVipAction_AddDuration,
        ERoleVipAction_Expired,
    };

public:
    CRoleVipLog();
    virtual ~CRoleVipLog();
public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);
public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    ERoleVipAction m_eAction;
    ERoleVipExpCause m_eExpCause;
    int m_nAddValue;
};


class CMeetActivityLog : public ISQLLog
{
public:
    enum EMeetAction
    {
        EMeetAction_None,
        EMeetAction_Join,
        EMeetAction_Sucess,
        EMeetAction_Fail,
        EMeetAction_Photo,
        EMeetAction_Reward,
        EMeetAction_InAmuse,
    };

public:
    CMeetActivityLog();
    virtual ~CMeetActivityLog();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    EMeetAction m_eMeetAction;
    unsigned int m_nCoupleRoleID;
    unsigned int m_nAmuseID;
    unsigned int m_nTime;
};

class CRoleAttrChangeLog : public ISQLLog
{
public:
    CRoleAttrChangeLog();
    virtual ~CRoleAttrChangeLog();

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    ERoleAttrChangeCause m_eChangeCause;

    std::string m_strOldAttrValue;
    std::string m_strNewAttrValue;
};

class CRoleCoupleLog : public ISQLLog
{
public:
    CRoleCoupleLog();
    virtual ~CRoleCoupleLog();

    enum ERoleCoupleCause
    {
        ERoleCoupleCause_None,
        ERoleCoupleCause_ToLover,
        ERoleCoupleCause_OutLover,
        ERoleCoupleCause_ToCouple,
        ERoleCoupleCause_AddBlessingValue,
        ERoleCoupleCause_AddIntimacy,
        ERoleCoupleCause_OutCouple,
        ERoleCoupleCause_RedEvnelopeCount,	 // 红包个数
        ERoleCoupleCause_CreateWeddingRoom,	 // 创建婚房
        ERoleCoupleCause_DestoryWeddingRoom, // 销毁婚房
        ERoleCoupleCause_CoupleReward,       // 情侣红包
    };

    enum ECoupleDestoryCase
    {
        ECoupleDestoryCase_None,
        ECoupleDestoryCase_Update,                  // 时间到
        ECoupleDestoryCase_OutCoupleSeparated,      // 离婚
        ECoupleDestoryCase_OutCoupleHandle,         // 离婚
        ECoupleDestoryCase_OutCoupleRequest,        // 离婚
        ECoupleDestoryCase_Marriage,                // 结婚
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nMaleRoleID;
    unsigned int m_nFemaleRoleID;
    ERoleCoupleCause m_eCoupleCause;

    int	m_nBlessingValue;
    int m_nIntimacy;
    int m_nRedEvnelope;
};

class CRoleDungeonDebrisLog : public ISQLLog
{
public:
    CRoleDungeonDebrisLog();
    virtual ~CRoleDungeonDebrisLog();

    enum EDebrisAction
    {
        EDebrisAction_None,
        EDebrisAction_Get,
        EDebrisAction_Exchange,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    EDebrisAction m_eAction;
    unsigned int m_nIsMainline;
    unsigned int m_nDungeonID;
    unsigned int m_nParcloseID;
    unsigned int m_nDebrisID;
    unsigned int m_nDebrisCount;
    unsigned int m_nLeftDebrisCount;
};

//////////////////////////////////////////////////////////////////////////

class CRoleVisitRankLog : public ISQLLog
{
public:
    CRoleVisitRankLog();
    virtual ~CRoleVisitRankLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EVisitRankAction
    {
        EVisitRankAction_Hall,          // 进入大厅
        EVisitRankAction_MCoin,         // 购物狂
        EVisitRankAction_Horn,          // 喇叭王
        EVisitRankAction_Exp,           // 升级达人
        EVisitRankAction_Group_Honor,   // 荣誉舞团
        EVisitRankAction_Intimacy,      // 明星夫妻
        EVisitRankAction_Cloths,        // 收藏家
        EVisitRankAction_ClothEffect,   // 闪亮之星
        EVisitRankAction_Medal,         // 勋章积分
    };

    unsigned int        m_nRoleID;
    EVisitRankAction    m_eAction;
    unsigned short      m_nDetailType;  // see ERankDetailType
    unsigned short      m_nMyRank;
};

//////////////////////////////////////////////////////////////////////////

class CRoleFriendsLog : public ISQLLog
{
public:
    CRoleFriendsLog();
    ~CRoleFriendsLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EFriendsAction
    {
        EFriendsAction_Delete = 0,      // 删除好友
        EFriendsAction_Add,             // 添加好友

        EFriendsAction_Max
    };

    enum EFriendsMode
    {
        EFriendsMode_None = 0,

        EFriendsMode_Del_Normal,        // 删除好友-普通
        EFriendsMode_Del_ToBlack,       // 删除好友-删除到黑名单
        EFriendsMode_Del_FromBlack,     // 删除好友-从黑名单删除

        EFriendsMode_Add_Normal = 10,   // 加好友-普通
        EFriendsMode_Add_Recommend,     // 加好友-推荐
        EFriendsMode_Add_Black,         // 加好友-黑名单
    };

    unsigned int        m_nRoleID;
    unsigned int        m_nFriendID;
    EFriendsAction      m_eAction;
    EFriendsMode        m_eMode;
};


class CRoleActivenessLog : public ISQLLog
{
public:
    CRoleActivenessLog();
    ~CRoleActivenessLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    enum EActivenessAction
    {
        EActivenessAction_Unknown = 0,
        EActivenessAction_AddScore,     // 增加积分
        EActivenessAction_Reward,       // 领取奖励
        EActivenessAction_FinishTarget, // 完成目标
        EActivenessAction_DailyReset,   // 日切重置

        EActivenessAction_Max
    };

    unsigned int      m_nRoleID;
    EActivenessAction m_eAction;
    int               m_nScore;
    int               m_nTarget;
};

// 魔法神灯
class CRoleDungeonLog : public ISQLLog
{
public:
    CRoleDungeonLog();
    ~CRoleDungeonLog() {}

    enum EDungeonAction
    {
        EDungeonAction_UnKnown = 0,
        EDungeonAction_JoinDungeon,      // 进入副本
        EDungeonAction_BeInvite,         // 被邀请进入副本
        EDungeonAction_UseCrystal,       // 消耗水晶
        EDungeonAction_BuyCrystal,       // 购买水晶（体力）
        EDungeonAction_UseSkipItem,      // 使用跳过关卡道具（法力纹章）
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    EDungeonAction  m_eAction;
    bool            m_bIsMainline;
    unsigned int    m_nDungeonID;
    unsigned int    m_nParcloseID;
    int             m_nValue;

};

// 商城
class CRoleMallLog : public ISQLLog
{
public:
    CRoleMallLog();
    virtual ~CRoleMallLog();

    enum EMallAction
    {
        EMallAction_UnKnown = 0,
        EMallAction_Enter,            // 抽奖
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    EMallAction     m_eAction;
};

// 照相馆
class CRolePhotoRoomLog : public ISQLLog
{
public:
    CRolePhotoRoomLog();
    virtual ~CRolePhotoRoomLog();

    enum EPhotoRoomAction
    {
        ERolePhotoRoomAction_UnKnown = 0,
        ERolePhotoRoomAction_Enter,            // 进入
        ERolePhotoRoomAction_UseButton,        // 使用照相按钮
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EPhotoRoomAction    m_eAction;
};

// 海选
class CRoleAuditionLog : public ISQLLog
{
public:
    CRoleAuditionLog();
    virtual ~CRoleAuditionLog();

    enum EAuditionAction
    {
        EAuditionAction_UnKnown = 0,
        EAuditionAction_Match,              // 申请匹配
        EAuditionAction_MatchSuc,           // 匹配成功
        EAuditionAction_MatchFail,          // 匹配失败
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int        m_nRoleID;
    EAuditionAction     m_eAction;
    unsigned int        m_nMatchPlayer;
};

// 长效累冲
class RoleLongactingRechargeLog : public ISQLLog
{
public:
    RoleLongactingRechargeLog();
    virtual ~RoleLongactingRechargeLog();

    enum ELongactingAction
    {
        ELongactingAction_UnKnown = 0,
        ELongactingAction_Reward = 1,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int            m_nRoleID;
    ELongactingAction       m_eAction;
    unsigned int            m_nRewardIndex;
    bool                    m_bIsSucess;
    bool                    m_bIsOnline;
    unsigned int            m_nTotalMPoint;
};

//////////////////////////////////////////////////////////////////////////

class CRoleCheckingMarkFailedLog : public ISQLLog
{
public:
    CRoleCheckingMarkFailedLog();
    ~CRoleCheckingMarkFailedLog() {}

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int    m_nRoleID;
    short           m_nRoomType;
    int             m_nScene;
    short           m_nTeamMode;
    int             m_nMusicID;
    int             m_nMusicMode;
    int             m_nMusicLevel;
    int             m_nPlayerCount;
    int             m_nStartTime;
    int             m_nRound;
    int             m_nKeyRank;
    int             m_nLastSpecial;
    std::string     m_strRoundRank;
    unsigned int    m_nMark;
    unsigned int    m_nCheckingMark;
};

// 衣服特效日志
class ClothEffectLog : public ISQLLog
{
public:
    ClothEffectLog();
    virtual ~ClothEffectLog();

    enum EClothEffectAction
    {
        EClothEffectAction_UnKnown = 0,
        EClothEffectAction_GM = 1, // GM添加
        EClothEffectAction_Replace = 2, // 替换
        EClothEffectAction_Upgrade = 3, // 升级
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int             m_nRoleID;
    EClothEffectAction       m_eAction;
    itemid_t                 m_nItemID;
    unsigned int             m_nOldEffectID;
    unsigned int             m_nNewEffectID;
};

//衣服特效进度日志
class ClothEffectProgressLog : public  ISQLLog
{
public:
    ClothEffectProgressLog();
    virtual ~ClothEffectProgressLog();

    enum EClothEffectProgressAction
    {
        EClothEffectProgressAction_UnKnown = 0,
        EClothEffectProgressAction_Handbook,
        EClothEffectProgressAction_Color,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    EClothEffectProgressAction      m_eAction;
    unsigned int                    m_nSuitID;
    unsigned int                    m_nParam1;
    std::string                     m_strParam2;
};

//勋章日志
class CMedalLog : public ISQLLog
{
public:
    CMedalLog();
    virtual ~CMedalLog();

    enum EMedalSource
    {
        EMedalUnKonw = 0,                           // 未知
        EMedalActivityBuyItem = 1,                  // 购买指定道具活动
        EMedalActivityCumulativeSpend = 2,          // 累计消费活动
        EMedalCondition = 3,                        // 条件领取
        EMedalExchage = 4,                          // 勋章兑换
    };

    enum EMedalAction
    {
        EMedalUnKonwAction = 0,
        EMedalActivated = 1,
        EMedalInvalid = 2,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    int                             m_nMedalID;
    int                             m_nConditionID;
    int                             m_nTimeStamp;
    EMedalSource                    m_eActivity;
    EMedalAction                    m_eAction;
};

// 舞团红包日志
class CRedEnvelopeLog : public  ISQLLog
{
public:
    CRedEnvelopeLog();
    virtual ~CRedEnvelopeLog();

    enum ERedEnvelopeAction
    {
        ERedEnvelopeAction_UnKnown = 0,
        ERedEnvelopeAction_Recharge = 1,            // 充值
        ERedEnvelopeAction_CreateRedEnvelope = 2,   // 创建成功
        ERedEnvelopeAction_Open = 3,                // 开红包
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    ERedEnvelopeAction              m_eAction;
    unsigned int                    m_nRedEnvelopeID;
    int                             m_nOpenIndex;
    int                             m_nRedEnvelopeMoney;
};

// 幸运魔法阵日志
class CMagicArrayLog : public ISQLLog
{
public:
    CMagicArrayLog();
    virtual ~CMagicArrayLog();

    enum EMagicArrayAction
    {
        EMagicArrayAction_UnKnown = 0,
        EMagicArrayAction_FreeCall = 1,         // 免费单抽
        EMagicArrayAction_OnceCall = 2,         // 单抽
        EMagicArrayAction_TenCall = 3,          // 十连抽
        EMagicArrayAction_Exchange = 4,         // 兑换
        EMagicArrayAction_RefreshExchange = 5,  // 刷新兑换物品
        EMagicArrayAction_ExchangeReset = 6,  // 兑换活动重置
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nRoleID;
    EMagicArrayAction               m_eAction;
    unsigned int                    m_nMagicArrayID;
};



// 玩家大妈日志
class CRoleBigMamaDanceRecord : public ISQLLog
{
public:
    CRoleBigMamaDanceRecord();
    CRoleBigMamaDanceRecord(unsigned int nRoleID, unsigned int nDanceGroupID, int nDanceRound);
    virtual ~CRoleBigMamaDanceRecord();

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);
    void InsertIntoLog();
public:
    unsigned int                    m_nRoleID;
    unsigned int                    m_nDanceDanceGroupID;
    int                             m_nDanceRound;
};

class CGroupBigMamaDanceRecord : public ISQLLog
{
public:
    CGroupBigMamaDanceRecord();
    CGroupBigMamaDanceRecord(unsigned int nDanceGroupID, unsigned int nDanceRound, int nBeatType, unsigned int nBeatRole);
    virtual ~CGroupBigMamaDanceRecord();

    enum
    {
        EBeatType_Role = 0,
        EBeatType_XiaoLian,
        EBeatType_XiaoRuan,
    };

public:
    virtual void ToSQL(char *szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int                    m_nDanceGroupID;
    unsigned int                    m_nDanceRound;
    int                             m_nBeatType; 
    unsigned int                    m_nBeatRole;
};

//玩家为游客类型的登录日志
class CPlayerType : public ISQLLog
{
public:
    CPlayerType();
    virtual ~CPlayerType();

public:
    virtual void ToSQL(char * szSQL, unsigned int nMaxSQLLen);

public:
    unsigned int m_nRoleID;
    unsigned int m_nLastLoginTime;
    int          m_nUserType;
};


#endif //__LOG_STRUCT_DEF_H__

