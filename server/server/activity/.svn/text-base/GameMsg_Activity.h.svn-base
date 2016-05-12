#ifndef		__GAMEMSG_ACTIVITY_H__
#define		__GAMEMSG_ACTIVITY_H__

#include "../../socket/GameMsg_Base.h"
#include "../../datastructure/DataStruct_DB.h"
#include <list>
#include "../../datastructure/Macro_Define.h"
#include "ActivityDataStruct.h"
#include "ProprietaryActivity.h"
#include "../gene/EntityGeneComponent.h"
#include "../item/EntityItemComponent.h"
#include "RedEnvelope/RedEnvelopeConfigManager.h"
#include "EffectStoneExchange/EffectStoneExchangeManager.h"



enum EGetXmasFreeRewardErrno
{
	EGetXmasFreeRewardErrno_Success,			//成功
	EGetXmasFreeRewardErrno_ActivityNotOpen,	//活动不开启
	EGetXmasFreeRewardErrno_NoSuchReward,		//没有该奖励
	EGetXmasFreeRewardErrno_HasGotTheReward,	//该奖励已经领取
};

enum EFestivalWishErrno
{
	EFestivalWishFailErrno_Success,				//成功
	EFestivalWishFailErrno_NotOpen,				//活动不开启
	EFestivalWishFailErrno_ReachLimitTimes,		//许愿达到次数
	EFestivalWishFailErrno_NoEnoughItem,		//物品不足
	EFestivalWishFailErrno_Unkonw,				//未知原因
};

enum EBlessFriendErrno
{
	EBlessFriendError_Success,					//成功
	EBlessFriendError_NotOpen,					//活动不开放
	EBlessFriendError_WordsNotExists,			//该祝福语不存在
	EBlessFriendError_NotFriend,				//不是朋友
	EBlessFriendError_NotEnoughItem,			//物品不足
};

enum EGetFestivalBlessRewarErrno
{
	EGetFestivalBlessRewarError_Success,				//成功
	EGetFestivalBlessRewarError_NotOpen,				//活动不开启
	EGetFestivalBlessRewarError_NotSuchLevel,			//不存在该档次奖励
	EGetFestivalBlessRewarError_NotReachRequirement,	//条件尚未满足
	EGetFestivalBlessRewarError_AlreadyGot,				//已领取奖励
};

enum EFillPuzzleErrno
{
	EFillPuzzleError_Success,				//成功
	EFillPuzzleError_NotOpen,				//活动未开启
	EFillPuzzleError_NoItem,				//缺少物品
	EFillPuzzleError_HasFill,				//已经拼过
};

enum EMeetErr
{
	EMeetErr_Success,                       // 成功
	EMeetErr_HaveMatch,                     // 已经在匹配
	EMeetErr_Waiting,                       // 等待
	EMeetErr_TimeOut,                       // 时间到
	EMeetErr_InWaitList,                    // 进入到等待列表
	EMeetErr_NotFindPlayer,                 // 没找到玩家
	EMeetErr_PhotoNotEnoughItem,            // 拍照消耗物品不够
	EMeetErr_PhotoInIndexErr,               // 插入拍照序列错误
	EMeetErr_PhotoConfigErr,                // 拍照配置错误
	EMeetErr_PlayerStatusErr,               // 玩家状态不对
	EMeetErr_ClientErr,                     // 玩家状态不对
	EMeetErr_PhotoIndexHave,                // 拍照点已经拍摄过
	EMeetErr_CreateSceneErr,                // 创建开放场景错误	
};


//////////////////////////////////////////////////////////////////////////////////////////
class CCumulativeRechargeRewardSend
{
public:
	CCumulativeRechargeRewardSend();
	~CCumulativeRechargeRewardSend();
public:
	int					m_nLevel;		//第几档次的奖励,对应index
	int					m_nRequiredNum;	//需要达到的数值
	std::list<CItem>	m_listItem;
	int					m_nMoney;
    int                 m_nBindMCoin;
	bool				m_bHasReceived;	//是否已经领取

public:
	void doEncode(CParamPool & IOBuff);
};

//////////////////////////////////////////////////////////////////////////////////////////
class CCumulativeSpendRewardSend
{
public:
	CCumulativeSpendRewardSend();
	~CCumulativeSpendRewardSend();

public:
	int					m_nLevel;		//第几档次的奖励,对应index
	int					m_nRequiredNum;	//需要达到的数值
	std::list<CItem>	m_listItem;
	int					m_nMoney;
	int					m_nBindMCoin;
	bool				m_bHasReceived;	//是否已经领取

public:
	void doEncode(CParamPool & IOBuff);
};


//////////////////////////////////////////////////////////////////////////////////////////
class CCummulativeRechareActivityInfo
{
public:
	CCummulativeRechareActivityInfo();
	~CCummulativeRechareActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity m_nActivityID;
	bool m_bExhibit;
	int m_nWeight;
	std::string	m_strRegularImageName;
	std::string m_strThumbnailsName;
	unsigned int m_nRechargeStartTime;
	unsigned int m_nRechargeStopTime;
	std::string m_strActivityName;
	std::string m_strDetail;
	int m_nCurRechargeNum;
	std::list<CCumulativeRechargeRewardSend>	m_listCumulativeReward;
};

//////////////////////////////////////////////////////////////////////////////////////////

class CCummulativeSpendActivityInfo
{
public:
	CCummulativeSpendActivityInfo();
	~CCummulativeSpendActivityInfo();

public:
	void doEncode(CParamPool & IOBuff);

public:
	EActivity m_nActivityID;
	bool m_bExhibit;
	int m_nWeight;
	char m_nSpendType;
	std::string	m_strRegularImageName;
	std::string m_strThumbnailsName;
	unsigned int m_nSpendStartTime;
	unsigned int m_nSpendStopTime;
	std::string m_strActivityName;
	std::string m_strDetail;
	int m_nCurRechargeNum;
	std::list<CCumulativeSpendRewardSend>	m_listCumulativeReward;
};

//////////////////////////////////////////////////////////////////////////

class CumulativeSpendGotMedalRewardData
{
public:
    CumulativeSpendGotMedalRewardData();
    ~CumulativeSpendGotMedalRewardData() {}

    bool doEncode( CParamPool &IOBuff ) const;

public:
    unsigned char m_nIndex;
    int m_nRequiredNum;
    std::list<int> m_listReward;
    bool m_bHasReceived;
};

class CumulativeSpendGotMedalActivityData
{
public:
    CumulativeSpendGotMedalActivityData();
    ~CumulativeSpendGotMedalActivityData() {}

public:
    bool doEncode( CParamPool &IOBuff ) const;

public:
    EActivity m_nActivityID;
    bool m_bExhibit;
    int m_nWeight;
    std::string m_strBigImage;
    std::string m_strSmallImage;
    unsigned int m_nOpenTime;
    unsigned int m_nCloseTime;
    std::string m_strTitle;
    std::string m_strContent;
    unsigned char m_nCumulativeType;
    int m_nCumulativeAmount;

    std::list<CumulativeSpendGotMedalRewardData> m_listReward;
};

//////////////////////////////////////////////////////////////////////////

class BuyItemGotMedalRewardData
{
public:
    BuyItemGotMedalRewardData();
    ~BuyItemGotMedalRewardData() {}

    bool doEncode( CParamPool &IOBuff ) const;

public:
    unsigned char m_nIndex;
    unsigned int m_nItemType;
    unsigned short m_nItemCount;
    unsigned short m_nCurrentCount;
    std::list<int> m_listReward;
    bool m_bHasReceived;
};

class BuyItemGotMedalActivityData
{
public:
    BuyItemGotMedalActivityData();
    ~BuyItemGotMedalActivityData() {}

public:
    bool doEncode( CParamPool &IOBuff ) const;

public:
    EActivity m_nActivityID;
    bool m_bExhibit;
    int m_nWeight;
    std::string m_strBigImage;
    std::string m_strSmallImage;
    unsigned int m_nOpenTime;
    unsigned int m_nCloseTime;
    std::string m_strTitle;
    std::string m_strContent;

    std::list<BuyItemGotMedalRewardData> m_listReward;
};

//////////////////////////////////////////////////////////////////////////////////////////
class CExchangeItemRewardSend
{
public:
	CExchangeItemRewardSend();
	~CExchangeItemRewardSend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	int					m_nLevel;				//第几档次的奖励,对应index
	itemtype_t  		m_nRequiredItemType;	//所需物品
	int					m_nRequiredItemNum;		//需要的物品数
	std::list<CItem>	m_listItem;				//奖励
	int					m_nMoney;				//金券
};

class CInTimeOnlineActivitySend
{
public:
	CInTimeOnlineActivitySend();
	~CInTimeOnlineActivitySend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	time_t m_tActivityBeginTime;
	time_t m_tActivityEndTime;
	int m_nTriggerHour;
	int m_nTriggerMin;
	std::list<CItem> m_listRewards;
	int m_nMoney;
};

//////////////////////////////////////////////////////////////////////////////////////////
class CExchangeItemActivityInfo
{
public:
	CExchangeItemActivityInfo();
	~CExchangeItemActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity									m_nActivityID;
	bool										m_bExhibit;
	int											m_nWeight;
	std::string									m_strRegularImageName;
	std::string									m_strThumbnailsName;
	unsigned int								m_nActivityBeginTime;
	unsigned int								m_nActivityEndTime;
	std::string									m_strActivityName;
	std::string									m_strActivityIntro;
	itemtype_t	    							m_nRequiredItemType;
	int											m_nCurRequiredItemNum;
	std::list<CExchangeItemRewardSend>			m_listExchangeItemReward;
};

class CShowInTimeOnlineActivity
{
public:
	CShowInTimeOnlineActivity();
	~CShowInTimeOnlineActivity();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity									m_nActivityID;
	bool										m_bExhibit;
	int											m_nWeight;
	std::string									m_strRegularImageName;
	std::string									m_strThumbnailsName;
	std::string									m_strActivityName;
	std::string									m_strActivityIntro;
	std::list<CInTimeOnlineActivitySend>		m_listActivities;
};

class CPlainTextActivityRewardSend
{
public:
	CPlainTextActivityRewardSend();
	~CPlainTextActivityRewardSend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	std::string m_strRewardName;
	std::list<CItem> m_listItems;
	int m_nMoney;
};

class CPlainTextActivityInfo
{
public:
	CPlainTextActivityInfo();
	~CPlainTextActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity									m_nActivityID;
	bool										m_bExhibit;
	int											m_nWeight;
	std::string									m_strRegularImageName;
	std::string									m_strThumbnailsName;
	std::string									m_strActivityTitle;
	std::string									m_strActivityContent;
	std::list<CPlainTextActivityRewardSend>		m_listRewards;
};

class CGiftCodeActivityInfo
{
public:
	CGiftCodeActivityInfo();
	~CGiftCodeActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity									m_nActivityID;
	bool										m_bExhibit;
	int											m_nWeight;
	std::string									m_strRegularImageName;
	std::string									m_strThumbnailsName;
	std::string									m_strActivityTitle;
	std::string									m_strActivityContent;
};

class CProprietaryActivityInfo
{
public:
	CProprietaryActivityInfo();
	~CProprietaryActivityInfo(){}

public:
	void doEncode(CParamPool & IOBuff);

public:
	EActivity m_eActivityId;
	bool m_bExhibit;
	int m_nWeight;
	std::string m_strRegularImageName;
	std::string m_strThumbnailsName;
	bool m_bHasReward;
	std::string m_strActivityTitle;
	std::string m_strActivityContent;
	CProprietaryActivityReward m_listReward;
	bool m_bFreeOrCharge;
	unsigned char m_nSex; 
};

class CXmasRewardSend
{
public:
	CXmasRewardSend();
	~CXmasRewardSend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nIndex;
	unsigned char m_nState;		//0表示未领，1表示已领 
};

class CXmasFreeRewardActivitySend
{
public:
	CXmasFreeRewardActivitySend();
	~CXmasFreeRewardActivitySend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nActivity;
	std::list<CXmasRewardSend> m_listFreeRewardInfo;
};

////////////////////////////////////////////////////////////////////////////////////////////
class CFestivalWishInfo
{
public:
	CFestivalWishInfo();
	~CFestivalWishInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	EActivity m_nActivityID;
	unsigned short m_nWishRemainTimes;
	unsigned short m_nWishItemCount;
	std::list<CItem> m_listItems;
	int m_nMoney;
	bool m_bMysteriousReward;
};

//////////////////////////////////////////////////////////////////////////////////////////
class CFestivalBlessRewardSend
{
public:
	CFestivalBlessRewardSend();
	~CFestivalBlessRewardSend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nLevel;
	unsigned int  m_nBeBlessedCount;
	std::list<CItem> m_listRewards;
	int m_nMoney;
	bool m_bAlreadyGot;
};
/////////////////////////////////////////////////////////////////////////////////////////
class COnlineRewardInfoSend
{
public:
	COnlineRewardInfoSend();
	~COnlineRewardInfoSend();

public:
	void doEncode(CParamPool & IOBuff);

public:
	int					m_nIndex;
	unsigned int		m_nRequireTime;
	std::list<CItem>	m_listRewards;
	int                 m_nMoney;
	bool				m_bAlreadyGot;
};

class COnlineRewardSend
{
public:
	COnlineRewardSend();
	~COnlineRewardSend();
public:
	void doEncode(CParamPool & IOBuff);
public:
	CKeepOnlineReward m_onlineRewardInfo;
};

////////////////////////////////////////////////////////////////////////////////////////
class CRolePuzzleSync
{
public:
	CRolePuzzleSync();
	~CRolePuzzleSync();

	void doEncode(CParamPool & IOBuff);
public:
	int m_nPuzzleID;
	std::string m_strTitle;
	std::string m_strTexture;

	CItem m_NormalCost;
	CItem m_SpecialCost;

	int m_nFinishTime;
	char m_arFragments[PUZZLE_FRAGMENTS_COUNT + 1];

	std::list<CItem> m_lstItemReward;
	int m_nMoneyReward;
};

class GameMsg_S2C_UpdateOnlineRewardInfo:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateOnlineRewardInfo();
	~GameMsg_S2C_UpdateOnlineRewardInfo();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	COnlineRewardSend m_onlieReward;
};

class COnlineRewardActivity
{
public:
	COnlineRewardActivity();
	~COnlineRewardActivity();
public:
	void doEncode(CParamPool & IOBuff);
public:
	unsigned char m_nActivityID;
	bool m_bExhibit;
	int m_nWeight;
	std::string m_strRegularImageName;
	std::string m_strThumbnailName;
	unsigned int m_nActivityBeginTime;
	unsigned int m_nActivityEndTime;
	std::string m_strActivityName;
	std::string m_strContent;
	COnlineRewardSend m_nextRewardInfo;
	std::list<COnlineRewardInfoSend> m_listOnlineRewardInfo;
};
////////////////////////////////////////////////////////////////////////////////////////
class COnlineRewardActivityInfo
{
public:
	COnlineRewardActivityInfo();
	~COnlineRewardActivityInfo();
public:

};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetOnlineReward:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetOnlineReward);
	GameMsg_C2S_GetOnlineReward();	
	~GameMsg_C2S_GetOnlineReward();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetOnlineRewardSuc:
	public	GameMsg_Base
{
public:
	GameMsg_S2C_GetOnlineRewardSuc();
	~GameMsg_S2C_GetOnlineRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned short m_nCurIndex;
	COnlineRewardSend m_NextRewardInfo;
};

class GameMsg_S2C_GetOnlineRewardFail:
	public	GameMsg_Base
{
public:
	GameMsg_S2C_GetOnlineRewardFail();
	~GameMsg_S2C_GetOnlineRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetAllActivitiesInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetAllActivitiesInfo);
	GameMsg_C2S_GetAllActivitiesInfo();
	~GameMsg_C2S_GetAllActivitiesInfo();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};




//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetCDKeyInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCDKeyInfo);
	GameMsg_C2S_GetCDKeyInfo();
	~GameMsg_C2S_GetCDKeyInfo();

public:
	virtual bool doDecode(CParamPool &IOBuff);

};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetCDKeyInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCDKeyInfoSuc();
	~GameMsg_S2C_GetCDKeyInfoSuc();

public:
	virtual bool doEncode(CParamPool & IOBuff);

public:
	CGiftCodeActivityInfo m_GiftCodeActivity;
};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetBindActivationInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetBindActivationInfo);
	GameMsg_C2S_GetBindActivationInfo();
	~GameMsg_C2S_GetBindActivationInfo();

public:
	virtual bool doDecode(CParamPool &IOBuff);

};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetBindActivationInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetBindActivationInfoSuc();
	~GameMsg_S2C_GetBindActivationInfoSuc();

public:
	virtual bool doEncode(CParamPool & IOBuff);

public:
	CInvitationActivityInfo m_InvitationActivity;
};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetActivitieInfoFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetActivitieInfoFail();
	~GameMsg_S2C_GetActivitieInfoFail();

public:
	virtual bool doEncode(CParamPool & IOBuff);

public:
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetAllActivitiesInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetAllActivitiesInfoSuc();
	~GameMsg_S2C_GetAllActivitiesInfoSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
    bool m_bFemale;
	CCummulativeRechareActivityInfo m_CumulativeRechargeActivity;
	CExchangeItemActivityInfo m_ExchangeItemActivity;
	CShowInTimeOnlineActivity m_ShowInTimeOnlineActivity;
	CPlainTextActivityInfo	m_plainTextActivity;
	CGiftCodeActivityInfo m_GiftCodeActivity;
	COnlineRewardActivity m_OnlineRewardActivity;
	CInvitationActivityInfo m_InvitationActivity;
	CFresherActivityInfo m_FresherActivity;
	CCummulativeSpendActivityInfo m_CumulativeSpendActivity;
    CMallSocialShareActivityData m_MallActivity;
    CPhotoSocialShareActivityData m_PhotoActivity;
    CumulativeSpendGotMedalActivityData m_cumulativeSpendGotMedalActivity;
    BuyItemGotMedalActivityData m_buyItemGotMedalActivity;
	CProprietaryActivityInfo m_proprietaryActivity;
    RedEnvelopeActivityData  m_redenvelopeActivity;
    EffectStoneExchangeData  m_effectStoneExchangeActivity;
    
};

//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetAllActivitiesInfoFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetAllActivitiesInfoFail();
	~GameMsg_S2C_GetAllActivitiesInfoFail();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	std::string m_strError;
};


//////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_UpdateCumulativeActivityState:
	public GameMsg_Base
{
public:
	GameMsg_S2C_UpdateCumulativeActivityState();
	~GameMsg_S2C_UpdateCumulativeActivityState();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CCummulativeRechareActivityInfo m_CumulativeRechargeActivity;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetCumulativeRechargeReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCumulativeRechargeReward);
	GameMsg_C2S_GetCumulativeRechargeReward();
	~GameMsg_C2S_GetCumulativeRechargeReward();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	char m_nLevel;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetCumulativeRechargeRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCumulativeRechargeRewardSuc();
	~GameMsg_S2C_GetCumulativeRechargeRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	char m_nLevel;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetCumulativeRechargeRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCumulativeRechargeRewardFail();
	~GameMsg_S2C_GetCumulativeRechargeRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};
/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetProprietaryReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetProprietaryReward);
	GameMsg_C2S_GetProprietaryReward();
	~GameMsg_C2S_GetProprietaryReward(){}
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

class GameMsg_S2C_GetProprietaryRewardResult : public GameMsg_Base
{
public:
	GameMsg_S2C_GetProprietaryRewardResult();
	~GameMsg_S2C_GetProprietaryRewardResult(){}
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};
/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetCumulativeSpendReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetCumulativeSpendReward);
	GameMsg_C2S_GetCumulativeSpendReward();
	~GameMsg_C2S_GetCumulativeSpendReward();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	char m_nLevel;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetCumulativeSpendRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCumulativeSpendRewardSuc();
	~GameMsg_S2C_GetCumulativeSpendRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	char m_nLevel;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_GetCumulativeSpendRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetCumulativeSpendRewardFail();
	~GameMsg_S2C_GetCumulativeSpendRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_GetCumulativeSpendGotMedalReward : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_GetCumulativeSpendGotMedalReward);

public:
    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned char m_nIndex;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_GetCumulativeSpendGotMedalRewardSuc);

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_nIndex;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_GetCumulativeSpendGotMedalRewardFail);

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    std::string m_strError;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_UpdateCumulativeSpendNum:
    public GameMsg_Base
{
public:
    GameMsg_S2C_UpdateCumulativeSpendNum();
    ~GameMsg_S2C_UpdateCumulativeSpendNum();
public:
    virtual bool doEncode(CParamPool &IOBuff);
public:
    int m_nSpendNum;
};

/////////////////////////////////////////////////////////////////////////////////////////
class GameMsg_S2C_UpdateCumulativeRechargeNum:
	public GameMsg_Base
{
public:
 GameMsg_S2C_UpdateCumulativeRechargeNum();
 ~GameMsg_S2C_UpdateCumulativeRechargeNum();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	int m_nRechargeNum;
};

/************************************************************************/
/*						兑换物品                                  */
/************************************************************************/
class GameMsg_C2S_ExchangeItem
:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ExchangeItem);
	GameMsg_C2S_ExchangeItem();
	~GameMsg_C2S_ExchangeItem();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	char m_nLevel;
};


class GameMsg_S2C_ExchangeItemSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_ExchangeItemSuc();
	~GameMsg_S2C_ExchangeItemSuc();
public:
	virtual bool doEncode(CParamPool & IOBuff);
public:
	char m_nLevel;
};

class GameMsg_S2C_ExchangeItemFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_ExchangeItemFail();
	~GameMsg_S2C_ExchangeItemFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};

/////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetXmasFreeRewardActivityInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetXmasFreeRewardActivityInfo);
	GameMsg_C2S_GetXmasFreeRewardActivityInfo();
	~GameMsg_C2S_GetXmasFreeRewardActivityInfo();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

/////////////////////////////////////////////////////
class GameMsg_S2C_GetXmasActivityInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetXmasActivityInfoSuc();
	~GameMsg_S2C_GetXmasActivityInfoSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CXmasFreeRewardActivitySend m_XmasFreeRewardActivity;
};

//////////////////////////////////////////////////
class GameMsg_C2S_GetXmasFreeReward
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetXmasFreeReward);
	GameMsg_C2S_GetXmasFreeReward();
	~GameMsg_C2S_GetXmasFreeReward();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned char m_nLevel;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_GetXmasFreeRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetXmasFreeRewardSuc();
	~GameMsg_S2C_GetXmasFreeRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nLevel;
	std::list<CItem> m_listItems;
	int m_nMoney;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_GetXmasFreeRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetXmasFreeRewardFail();
	~GameMsg_S2C_GetXmasFreeRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
};

///////////////////////////////////////////////////
class GameMsg_C2S_GetFestivalWishInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFestivalWishInfo);
	GameMsg_C2S_GetFestivalWishInfo();
	~GameMsg_C2S_GetFestivalWishInfo();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

///////////////////////////////////////////////////
class GameMsg_S2C_GetFestivalWishInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFestivalWishInfoSuc();
	~GameMsg_S2C_GetFestivalWishInfoSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	CFestivalWishInfo m_FestivalWishInfo;
};

//////////////////////////////////////////////////
class GameMsg_C2S_FestivalWish
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_FestivalWish);
	GameMsg_C2S_FestivalWish();
	~GameMsg_C2S_FestivalWish();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};

/////////////////////////////////////////////////////
class GameMsg_S2C_FestivalWishSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FestivalWishSuc();
	~GameMsg_S2C_FestivalWishSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list<CItem>	m_listCurRewards;
	int					m_nCurMoney;	
	std::list<CItem>	m_listNextReward;
	int					m_nNextMoney;
	bool				m_bNextMysterious;
	unsigned short		m_nWishTimesRemain;
	unsigned short		m_nNextWishItemCountNeed;
};

/////////////////////////////////////////////////////
class GameMsg_S2C_FestivalWishFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FestivalWishFail();
	~GameMsg_S2C_FestivalWishFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_GetFestivalBlessActivityInfo
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFestivalBlessActivityInfo);
	GameMsg_C2S_GetFestivalBlessActivityInfo();
	~GameMsg_C2S_GetFestivalBlessActivityInfo();
public:
	virtual bool doDecode(CParamPool & IOBuff);
};
///////////////////////////////////////////////////
class GameMsg_S2C_GetFestivalBlessActivityInfoSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFestivalBlessActivityInfoSuc();
	~GameMsg_S2C_GetFestivalBlessActivityInfoSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	EActivity m_nActivityID;
	unsigned int m_nBeBlessedCount;
	std::list<CFestivalBlessEntry>		m_BeBlessWords;
	std::list<CFestivalBlessRewardSend> m_listBlessRewardInfo;
	std::list<CFestivalBlessWords>		m_listBlessWordsData;
};

/////////////////////////////////////////////////////
class GameMsg_C2S_BlessFriend
	:public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BlessFriend);
	GameMsg_C2S_BlessFriend();
	~GameMsg_C2S_BlessFriend();
public:
	virtual bool doDecode(CParamPool & IOBuff);
public:
	unsigned int m_nFriendID;
	unsigned char m_nWordsIndex;
};

///////////////////////////////////////////////////
class GameMsg_S2C_BlessFriendSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_BlessFriendSuc();
	~GameMsg_S2C_BlessFriendSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::list<CItem> m_listRewards;
	int m_nMoney;
};

///////////////////////////////////////////////////
class GameMsg_S2C_BlessFriendFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_BlessFriendFail();
	~GameMsg_S2C_BlessFriendFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
};

///////////////////////////////////////////////////
class GameMsg_S2C_GetBlessFromFriend:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetBlessFromFriend();
	~GameMsg_S2C_GetBlessFromFriend();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strSenderName;
	unsigned char m_nWordsIndex;
};

///////////////////////////////////////////////////
class GameMsg_C2S_GetFestivalBlessReward:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFestivalBlessReward);
	GameMsg_C2S_GetFestivalBlessReward();
	~GameMsg_C2S_GetFestivalBlessReward();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nLevel;
};

///////////////////////////////////////////////////
class GameMsg_S2C_GetFestivalBlessRewardSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFestivalBlessRewardSuc();
	~GameMsg_S2C_GetFestivalBlessRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nLevel;
};

///////////////////////////////////////////////////
class GameMsg_S2C_GetFestivalBlessRewardFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_GetFestivalBlessRewardFail();
	~GameMsg_S2C_GetFestivalBlessRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
};

///////////////////////////////////////////////////
class GameMsg_S2C_SyncPuzzleData:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SyncPuzzleData);

	GameMsg_S2C_SyncPuzzleData();
	~GameMsg_S2C_SyncPuzzleData();

	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strPuzzleHelpTitle;
	std::string m_strPuzzleHelpContent;

	std::vector<CRolePuzzleSync> m_vecRolePuzzle;
};

///////////////////////////////////////////////////
class GameMsg_C2S_FillPuzzle:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_FillPuzzle);

	GameMsg_C2S_FillPuzzle();
	~GameMsg_C2S_FillPuzzle();

	virtual bool doDecode(CParamPool &IOBuff);
public:
	int m_nPuzzleID;
	int m_nPieceIndex;
	bool m_bUseSpecial;
};

///////////////////////////////////////////////////
class GameMsg_S2C_FillPuzzleSuc:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FillPuzzleSuc();
	~GameMsg_S2C_FillPuzzleSuc();

	virtual bool doEncode(CParamPool &IOBuff);
public:
	int m_nPuzzleID;
	int m_nPieceIndex;
	int m_nFinishTime;
};

///////////////////////////////////////////////////
class GameMsg_S2C_FillPuzzleFail:
	public GameMsg_Base
{
public:
	GameMsg_S2C_FillPuzzleFail();
	~GameMsg_S2C_FillPuzzleFail();

	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nError;
};


//////////////////////////////////////////////////////////////////////////
// 新玩家招募活动
// 1. GetInvitationInviterInfo
class GameMsg_C2S_GetInvitationInviterInfo:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetInvitationInviterInfo);
	GameMsg_C2S_GetInvitationInviterInfo();	
	~GameMsg_C2S_GetInvitationInviterInfo();
public:
	virtual bool doDecode(CParamPool &IOBuff);
};

enum eInvitation_Reward_State
{
	INVITATION_REWARD_STATE_Not_Received = 0,	//未领取
	INVITATION_REWARD_STATE_Received = 1,		//已领取
	INVITATION_REWARD_STATE_Not_Enough = 2,		//未达到条件
};

class CInvitationInviterRewardInfo
{
public:
	CInvitationInviterRewardInfo();
	~CInvitationInviterRewardInfo();

public:
	bool doEncode(CParamPool & IOBuff) const;

public:
	unsigned char m_nReceivedFlag;		//eInvitation_Reward_State
	unsigned int m_nLevel;			//第几档次的奖励,对应index
	int  m_nCumulation;				//累计次数
	int m_nMoney;
	std::list<CItem> m_listItem;
};

typedef std::map<int, CInvitationInviterRewardInfo> CInvitationInviterRewardInfoMap;


class GameMsg_S2C_GetInvitationInviterInfoSuc:
	public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetInvitationInviterInfoSuc);
	GameMsg_S2C_GetInvitationInviterInfoSuc();	
	~GameMsg_S2C_GetInvitationInviterInfoSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	//玩家邀请码和邀请人数信息
	std::string m_strInvitationCode;
	int m_nInvitationCount;

	//活动信息
	std::string m_strActivityName;
	std::string m_strActivityContent;
	CInvitationInviterRewardInfoMap m_cumulativeReward;

	//随机的人物动画
	std::string m_strShareAnimation;
};

class GameMsg_S2C_GetInvitationInviterInfoFail:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetInvitationInviterInfoFail);
	GameMsg_S2C_GetInvitationInviterInfoFail();	
	~GameMsg_S2C_GetInvitationInviterInfoFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string m_strError;
};

// 2. BindInvitationCode
class GameMsg_C2S_BindInvitationCode:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_BindInvitationCode);
	GameMsg_C2S_BindInvitationCode();	
	~GameMsg_C2S_BindInvitationCode();
public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	char m_szInvitationCode[MAX_INVITATION_CODE_LEN];
};

class GameMsg_S2C_BindInvitationCodeSuc:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BindInvitationCodeSuc);
	GameMsg_S2C_BindInvitationCodeSuc();	
	~GameMsg_S2C_BindInvitationCodeSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	//CInvitationAcceptorReward m_reward;
};

class GameMsg_S2C_BindInvitationCodeFail:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_BindInvitationCodeFail);
	GameMsg_S2C_BindInvitationCodeFail();	
	~GameMsg_S2C_BindInvitationCodeFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;

};

enum eInvitation_Reward_Type
{
	Invitation_Reward_Type_Cumulation = 1,	//1-累计奖励
	Invitation_Reward_Type_Cycle = 2,		//2-循环奖励
};

// 3. GetInvitationInviterReward
class GameMsg_C2S_GetInvitationInviterReward:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetInvitationInviterReward);
	GameMsg_C2S_GetInvitationInviterReward();	
	~GameMsg_C2S_GetInvitationInviterReward();
public:
	virtual bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nType;	// eInvitation_Reward_Type 
	unsigned char m_nLevel;
};

class GameMsg_S2C_GetInvitationInviterRewardSuc:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetInvitationInviterRewardSuc);
	GameMsg_S2C_GetInvitationInviterRewardSuc();	
	~GameMsg_S2C_GetInvitationInviterRewardSuc();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nType;	// eInvitation_Reward_Type 
	unsigned char m_nLevel;
	int m_nCumulation;
};


class GameMsg_S2C_GetInvitationInviterRewardFail:
	public	GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetInvitationInviterRewardFail);
	GameMsg_S2C_GetInvitationInviterRewardFail();	
	~GameMsg_S2C_GetInvitationInviterRewardFail();
public:
	virtual bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strError;
};


///////////////////////////////////////////////////////////////////////////////
class GameMsg_C2S_GetFresherReward : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetFresherReward);
	GameMsg_C2S_GetFresherReward();
	~GameMsg_C2S_GetFresherReward();

public:
	bool doDecode(CParamPool &IOBuff);

public:
	unsigned char m_nDay;
};

class GameMsg_S2C_GetFresherRewardSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetFresherRewardSuc();
	~GameMsg_S2C_GetFresherRewardSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned char m_nDay;
};


class GameMsg_S2C_GetFresherRewardFail : public GameMsg_Base
{
public:
	GameMsg_S2C_GetFresherRewardFail();
	~GameMsg_S2C_GetFresherRewardFail();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string m_strError;
};


class GameMsg_S2C_GetFresherAllReward : public GameMsg_Base
{
public:
	GameMsg_S2C_GetFresherAllReward();
	~GameMsg_S2C_GetFresherAllReward();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
};

class GameMsg_C2S_GetExtraActivityInfo : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetExtraActivityInfo);
	GameMsg_C2S_GetExtraActivityInfo();
	~GameMsg_C2S_GetExtraActivityInfo();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetExtraActivityInfoSuc : public GameMsg_Base
{
public:
	GameMsg_S2C_GetExtraActivityInfoSuc();
	~GameMsg_S2C_GetExtraActivityInfoSuc();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	unsigned char m_nSceneID;
};

class GameMsg_C2S_MeetMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetMatch);
	GameMsg_C2S_MeetMatch();
	~GameMsg_C2S_MeetMatch();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_C2S_MeetCancelMatch : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetCancelMatch);
	GameMsg_C2S_MeetCancelMatch();
	~GameMsg_C2S_MeetCancelMatch();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_C2S_MeetLeaveScene : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetLeaveScene);
	GameMsg_C2S_MeetLeaveScene();
	~GameMsg_C2S_MeetLeaveScene();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_C2S_MeetCameraEvent : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetCameraEvent);
	GameMsg_C2S_MeetCameraEvent();
	~GameMsg_C2S_MeetCameraEvent();

public:
	virtual bool doDecode(CParamPool &IOBuff);

public:
	unsigned int        m_nIndex;
};

struct MeetPhotoInfo
{
	unsigned int     nIndex;
	std::string      strName;
	bool             nIsMalePhoto;
	bool             nIsFemalePhoto;

	MeetPhotoInfo()
	{
		nIndex          = 0;
		strName         = "";
		nIsMalePhoto    = false;
		nIsFemalePhoto  = false;
	}
};
typedef std::vector<MeetPhotoInfo> MeetPhotoInfoList;

class GameMsg_C2S_MeetEnd : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetEnd);
	GameMsg_C2S_MeetEnd();
	~GameMsg_C2S_MeetEnd();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_C2S_MeetEnterAmuseScene : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_MeetEnterAmuseScene);
	GameMsg_C2S_MeetEnterAmuseScene();
	~GameMsg_C2S_MeetEnterAmuseScene();

public:
	virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_MeetEndResult : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MeetEndResult);
	GameMsg_S2C_MeetEndResult();
	~GameMsg_S2C_MeetEndResult();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public :
	std::vector<MeetPhotoInfo>     m_photosInfo;
	std::list<CItem>               m_listItems;
};


struct PlayerShowMsg
{
	unsigned int           m_nRoleID;
	std::string            m_strRoleName;
	unsigned char          m_eRoleSex;
	char				   m_nRoleSkin;
	std::string			   m_strDanceGroupName;
	char				   m_nGroupTitle;
    unsigned short         m_nDanceGroupBadge;
	unsigned short		   m_nDanceGroupEffect;
	bool                   m_bIsHost;
	unsigned char          m_eRoleType;
	bool                   m_nIsVip;
	unsigned short         m_nVipLv;
	unsigned char          m_nMoveType;

	CEntityItemComponent*  m_pRoleItem;
	CEntityGeneComponent*  m_pRoleGene;

	PlayerShowMsg()
	{
		m_nRoleID            =  0;
		m_strRoleName        = "";
		m_eRoleSex           = 0;
		m_nRoleSkin          = 1;
		m_strDanceGroupName  = "";
		m_nGroupTitle        = 0;
		m_nDanceGroupBadge	 = 0;
		m_nDanceGroupEffect  = 0;
		m_bIsHost            = false;
		m_eRoleType          = 0;
		m_nIsVip             = false;
		m_nVipLv             = 0;
		m_nMoveType          = 0;

		m_pRoleGene          = NULL;
		m_pRoleItem          = NULL;
	}
};

class GameMsg_S2C_MeetMatchSucceed : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MeetMatchSucceed);
	GameMsg_S2C_MeetMatchSucceed();
	~GameMsg_S2C_MeetMatchSucceed();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::vector<PlayerShowMsg>   m_MatchPlayers;
};

class GameMsg_S2C_MeetMatchFailed : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MeetMatchFailed);
	GameMsg_S2C_MeetMatchFailed();
	~GameMsg_S2C_MeetMatchFailed();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string       m_strError;
};

class GameMsg_S2C_MeetPhotoSucceed : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MeetPhotoSucceed);
	GameMsg_S2C_MeetPhotoSucceed();
	~GameMsg_S2C_MeetPhotoSucceed();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nPhotoIndex;
};

class GameMsg_S2C_MeetPhotoFailed : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_MeetPhotoFailed);
	GameMsg_S2C_MeetPhotoFailed();
	~GameMsg_S2C_MeetPhotoFailed();

public:
	virtual bool doEncode(CParamPool &IOBuff);

public:
	std::string       m_strError;
};

//社交分享数据基类，用于GameMsg_C2S_SocialShare消息
class CActivitySocialShareDataBase
{
public:
	CActivitySocialShareDataBase(){}
	virtual ~CActivitySocialShareDataBase(){}

	virtual bool doDecode(CParamPool &IOBuff) = 0;
	virtual bool doEncode(CParamPool &IOBuff) = 0;
};

//商城社交分享数据
class CMallSocialShareData : public CActivitySocialShareDataBase
{
public:
	CMallSocialShareData(){}
	~CMallSocialShareData(){}
public:
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:
	std::list<itemtype_t> m_listTryCloth;//试穿的服饰
};

//照相馆社交分享数据
class CPhotoSocialShareData : public CActivitySocialShareDataBase
{
public:
    CPhotoSocialShareData() {}
    ~CPhotoSocialShareData() {}

public:
    bool doDecode(CParamPool &IOBuff);
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char   m_nRoomTheme;
    int             m_nPeopleNum;
};

//社交分享
class GameMsg_C2S_SocialShare : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_SocialShare);
	GameMsg_C2S_SocialShare();
	~GameMsg_C2S_SocialShare();
public:
	bool doDecode(CParamPool &IOBuff);
};
class GameMsg_S2C_SocialShare : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_S2C_SocialShare);
	GameMsg_S2C_SocialShare();
	~GameMsg_S2C_SocialShare();
public:
	bool doEncode(CParamPool &IOBuff);
public:
	std::string m_strTip;//返回字符串
};

//社交分享活动
class GameMsg_C2S_ActivitySocialShare : public GameMsg_Base
{
public:
	GAMEMSG_CREATEDECLARE(GameMsg_C2S_ActivitySocialShare);
	GameMsg_C2S_ActivitySocialShare();
	~GameMsg_C2S_ActivitySocialShare();
public:
	bool doDecode(CParamPool &IOBuff);
public:
	unsigned char m_nUI;//界面编号
	CActivitySocialShareDataBase* m_pData;//具体活动的数据，基类指针
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_ActivitySocialShare : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_ActivitySocialShare);
    GameMsg_S2C_ActivitySocialShare();
    ~GameMsg_S2C_ActivitySocialShare();

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char           m_nUI;          //界面编号
    std::list<CReward>      m_listRewards;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_S2C_CanSocialShareFirst : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_CanSocialShareFirst);
    GameMsg_S2C_CanSocialShareFirst();
    ~GameMsg_S2C_CanSocialShareFirst();
public:
    bool doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nUI;//界面编号
    bool m_bCanDoFirstShare;//能否首次分享
    std::string m_strActionName;//动画名
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestVIPExtraReward : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_RequestVIPExtraReward);
    GameMsg_C2S_RequestVIPExtraReward();
    ~GameMsg_C2S_RequestVIPExtraReward() {}

public:
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned char       m_nOpenVipFromType;
};

//////////////////////////////////////////////////////////////////////////

enum ERequestVIPExtraRewardErrorType
{
    ERequestVIPExtraRewardErrorType_Success = 0,
    ERequestVIPExtraRewardErrorType_AlreadyGot,     // 已经获得
};

class GameMsg_S2C_RequestVIPExtraRewardRes : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_RequestVIPExtraRewardRes);
    GameMsg_S2C_RequestVIPExtraRewardRes();
    ~GameMsg_S2C_RequestVIPExtraRewardRes() {}

public:
    bool doEncode(CParamPool &IOBuff);

public:
    unsigned char                   m_nErrorType;
    unsigned char                   m_nOpenVipFromType;
    std::list<CReward>              m_listRewards;
};


class GameMsg_C2S_GetLongactingCumulativeRecharge : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLongactingCumulativeRecharge);
    GameMsg_C2S_GetLongactingCumulativeRecharge();
    ~GameMsg_C2S_GetLongactingCumulativeRecharge();

public:
    virtual bool doDecode(CParamPool &IOBuff);
};


class GameMsg_S2C_GetLongactingCumulativeRechargeResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLongactingCumulativeRechargeResult);
    GameMsg_S2C_GetLongactingCumulativeRechargeResult();
    ~GameMsg_S2C_GetLongactingCumulativeRechargeResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<LongactingCumulativeRechargeConfig>            m_listLongactingRechargeConfig;
    unsigned int                                             m_nHaveRewardIndex;
    char                                                     m_cSex;
};

class GameMsg_C2S_GetLongactingCumulativeRechargeReward : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLongactingCumulativeRechargeReward);
    GameMsg_C2S_GetLongactingCumulativeRechargeReward();
    ~GameMsg_C2S_GetLongactingCumulativeRechargeReward();

public:
    virtual bool doDecode(CParamPool &IOBuff);

public:
    unsigned int        m_nRewardIndex;
};


class GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail);
    GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail();
    ~GameMsg_S2C_GetLongactingCumulativeRechargeRewardFail();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nErrorFlag;
};

class GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess);
    GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess();
    ~GameMsg_S2C_GetLongactingCumulativeRechargeRewardScuess();

public:
    virtual bool doEncode(CParamPool &IOBuff);
};

class GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast);
    GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast();
    ~GameMsg_C2S_GetLongactingCumulativeRechargeBoardCast();

public:
    virtual bool doDecode(CParamPool &IOBuff);
};

class GameMsg_S2C_GetLongactingCumulativeRechargeBCResult : public GameMsg_Base
{
public:
    GAMEMSG_CREATEDECLARE(GameMsg_S2C_GetLongactingCumulativeRechargeBCResult);
    GameMsg_S2C_GetLongactingCumulativeRechargeBCResult();
    ~GameMsg_S2C_GetLongactingCumulativeRechargeBCResult();

public:
    virtual bool doEncode(CParamPool &IOBuff);

public:
    std::list<LongactingCumulativeRechargeBoardCast>       m_listLongactingBoardCast;
};

//////////////////////////////////////////////////////////////////////////

class GameMsg_C2S_RequestBuyItemGotMedalReward : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_C2S_RequestBuyItemGotMedalReward)

public:
    virtual bool doDecode( CParamPool &IOBuff );

public:
    unsigned char m_nIndex;
};

class GameMsg_S2C_RequestBuyItemGotMedalRewardSuc : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestBuyItemGotMedalRewardSuc)

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_nIndex;
};

class GameMsg_S2C_RequestBuyItemGotMedalRewardFail : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_RequestBuyItemGotMedalRewardFail)

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    std::string m_strError;
};

class GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount : public GameMsg_Base
{
    GAMEMSG_CLASSHEADER_DECLARE(GameMsg_S2C_UpdateBuyItemGotMedalCurrentCount)

public:
    virtual bool doEncode( CParamPool &IOBuff );

public:
    unsigned char m_nIndex;
    unsigned short m_nCurrentCount;
};

#endif

