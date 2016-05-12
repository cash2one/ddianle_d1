#ifndef		__ACTIVITYDATASTRUCT_H__
#define		__ACTIVITYDATASTRUCT_H__

#include <vector>
#include <list>
#include "../../datastructure/DataStruct_Base.h"


class CFestivalBlessEntry;
class CParamPool;

//商城分享活动数据
class CMallShareActivityData
{
public:
	CMallShareActivityData():m_nActivityID(EActivity_None), m_bExhibit(0), m_nWeight(0), m_nRewardItemTime(0){}
	~CMallShareActivityData(){}
public://virtual
	bool doDecode(CParamPool &IOBuff);
	bool doEncode(CParamPool &IOBuff);
public:
	unsigned char m_nActivityID;
	bool m_bExhibit;					//是否展示在大厅
	int m_nWeight;						//显示顺序
	std::string m_strTitle;				//标题
	std::string m_strContent;			//内容
	std::string m_strBigImageName;		//大图片
	std::string m_strSmallImageName;	//小图片
	std::string m_strActionName;		//动作
	std::string m_strShareTextMale;		//分享字符串（男）
	std::string m_strShareTextFemale;	//分享字符串（女）

	//不用于doDecode，doEncode
	std::string m_strBeginTimeShow;
	std::string m_strEndTimeShow;
	std::string m_strBeginTime;
	std::string m_strEndTime;
	std::string m_strRewardDesc;
	std::string m_strMailTitle;
	std::string m_strMailContent;
	int	m_nRewardItemTime;				//奖励试穿服饰期限，单位天
	std::set<int> m_setClothPos;		//奖励试穿服饰的部位
};

class CRoleXmasFreeRewardInfo
{
public:
	CRoleXmasFreeRewardInfo();
	~CRoleXmasFreeRewardInfo();
public:
	void Reset();
public:
	unsigned int		m_nLastGetRewardTime;
	std::vector<int>	m_vecFreeReward;
};

///////////////////////////////////////////////
class CRoleFestivalWishActivityInfo
{
public:
	CRoleFestivalWishActivityInfo();	
	~CRoleFestivalWishActivityInfo();
public:
	void Reset();
public:
	int m_nWishedCount;
	unsigned int m_nLastGetRewardTime;
};

//////////////////////////////////////////////
class CRoleBlessActivityInfo
{
public:
	CRoleBlessActivityInfo();
	~CRoleBlessActivityInfo();
public:
	int									m_nBeBlessedCount;
	std::list<CFestivalBlessEntry>		m_listBlesses;
	std::vector<int>					m_vecRewards;
	bool								m_bHasSentInfoToClient;
};
///////////////////////////////////////////////
class CFestivalBlessWords
{
public:
	CFestivalBlessWords();
	~CFestivalBlessWords();
public:
	void doEncode(CParamPool & IOBuff);
public:
	int m_nIndex;
	std::string m_strBlessWords;
};

class CFestivalSendBlessReward
{
public:
	CFestivalSendBlessReward();
	~CFestivalSendBlessReward();
public:
	std::list<CItem> m_listMaleRewards;
	std::list<CItem> m_listFemaleRewards;
	int m_nMoney;
};

//////////////////////////////////////////////////
class CFestivalBlessedReward
{
public:
	CFestivalBlessedReward();
	~CFestivalBlessedReward();
public:
	int					m_nLevel;
	int					m_nBeBlessedCount;
	std::list<CItem>	m_listMaleReward;
	std::list<CItem>	m_listFemaleReward;
	int					m_nMoney;
};

////////////////////////////////////////////
class CRolePuzzleActivityInfo
{
public:
	CRolePuzzleActivityInfo();
	~CRolePuzzleActivityInfo();
public:
	unsigned int m_nOwnerID;

	int m_nPuzzleID;
	int m_nUpdateTime;
	int m_nFinishTime;
	char m_arFragments[PUZZLE_FRAGMENTS_COUNT + 1];
};


/**
* 玩家招募对应的奖励
*/

// 被邀请者的奖励
class CInvitationAcceptorReward
{
public:
	CInvitationAcceptorReward();
	~CInvitationAcceptorReward();

public:
	bool doEncode(CParamPool & IOBuff) const;

public:
	std::list<CItem> m_listItem;
	int m_nMoney;
};

//邀请者的奖励
class CInvitationInviterReward
{
public:
	CInvitationInviterReward();
	~CInvitationInviterReward();

public:
	bool doEncode(CParamPool & IOBuff) const;

public:
	unsigned int m_nLevel;			//第几档次的奖励,对应index
	unsigned int m_nCumulation;		//累计次数
	std::list<CItem> m_listItem;
	int m_nMoney;
};

typedef std::map< unsigned int, CInvitationInviterReward> InvitationCumulativeReward;


class CInvitationActivityInfo
{
public:
	CInvitationActivityInfo();
	~CInvitationActivityInfo();
public:
	void doEncode(CParamPool & IOBuff);
public:
	bool						m_bIsOpen;
	unsigned char				m_nActivityID;
	bool						m_bExhibit;
	int							m_nWeight;
	std::string					m_strRegularImageName;
	std::string					m_strThumbnailImageName;
	unsigned int				m_nActivityBeginTime;
	unsigned int				m_nActivityEndTime;
	std::string					m_strActivityName;
	std::string					m_strActivityContent;
	CInvitationAcceptorReward	m_reward;
};



enum eRewardStatus
{
	RewardStatus_Invalid,			//非法状态
	RewardStatus_Received,			//已经领取
	RewardStatus_UnReceived,		//未领取
	RewardStatus_CannotReceive,		//不能领取
	RewardStatus_ReceiveAgain,		//可补领
};

class CFresherReward
{
public:
	CFresherReward();
	~CFresherReward();

public:
	void doEncode(CParamPool &buff);

public:
	int m_nDay;
	std::list<CItem> m_listItem;
	int m_nMoney;
    int m_nBindCoin;
    std::list<CItem>    m_listVipItem;
    int m_nVipMoney;
    int m_nVipBindCoin;
	unsigned char m_nRewardStatus;
};


class CFresherActivityInfo
{
public:
	CFresherActivityInfo();
	~CFresherActivityInfo();

public:
	void doEncode(CParamPool &buff);

public:
	bool m_bIsOpen;	// 活动是否开放

	unsigned char m_nDay; //活动第几天

	std::string m_strActivityTitle;
	std::string m_strActivityContent;

	int m_nRecvAgainMoney;	// 补收奖励需要的金券

	std::vector<CFresherReward> m_vecReward;
};


class CPlatofromPrivilegeReward
{
public:
	CPlatofromPrivilegeReward();
	~CPlatofromPrivilegeReward(){}

public:
	void doEncode(CParamPool & buff);

public:
	int							nVipIndex; //特权等级
	int							nLowLevel;
	int							nHighLevel;
	std::list<CItem>		listItem;
};



//////////////////////////////////////////////////////////////

enum MeetPlayerStatus
{
	MPS_MATCHING = 0,   // 等待中 
	MPS_RUNNING  = 1,   // 活动中
};

struct MeetCouple
{
	unsigned int        nID;
	unsigned int        nOppositeID[2];
	bool                nIsMaleGet;
	bool                nIsFemaleGet;
	std::vector<bool>   maleIsPhotoFlag;
	std::vector<bool>   femaleIsPhotoFlag;
	int                 nAmuseSceneID;

	MeetCouple( unsigned int nPhotoCount )
	{
		nID				     = 0;
		nOppositeID[0]  	 = 0;
		nOppositeID[1]  	 = 0;
		nAmuseSceneID		 = 0;
		nIsMaleGet           = false;
		nIsFemaleGet         = false;

		maleIsPhotoFlag.resize( nPhotoCount );
		femaleIsPhotoFlag.resize( nPhotoCount );
	}
};
typedef map<unsigned int,MeetCouple> MeetCoupleTable;
typedef MeetCoupleTable::iterator    MeetCoupleTableLink;

struct MeetWait
{
	unsigned int         roleid;
	unsigned int         intime;
};
typedef std::list<MeetWait> MeetWaitList;
typedef std::list<MeetWait>::iterator MeetWaitListLink;

struct MeetPlayerObject
{
	unsigned int         roleid;
	unsigned int         sex;
	MeetPlayerStatus     status;
	MeetWaitListLink     waitlink;
	MeetCoupleTableLink  couplelink;

	MeetPlayerObject()
	{
		roleid = 0;
		sex    = 0;
	}
};
typedef map<unsigned int,MeetPlayerObject> MeetPlayerObjectTable;

//////////////////////////////////////////////////////////////////////////

class CRewardTitleInfo
{
public:
    CRewardTitleInfo();
    virtual ~CRewardTitleInfo() {}

public:
    virtual bool doEncode(CParamPool &IOBuff, bool bFemale = false);

public:
    unsigned short      m_nIdx;
    std::string         m_strTitle;             // 奖励标题
};

class CStringRewardInfo : public CRewardTitleInfo
{
public:
    CStringRewardInfo();
    virtual ~CStringRewardInfo() {}

public:
    virtual bool doEncode(CParamPool &IOBuff, bool bFemale = false);

public:
    std::string         m_strMaleRewardText;    // 奖励描述(男)
    std::string         m_strFemaleRewardText;  // 奖励描述(女)
};

//////////////////////////////////////////////////////////////////////////

class CItemRewardInfo : public CRewardTitleInfo
{
public:
    CItemRewardInfo();
    virtual ~CItemRewardInfo() {}

public:
    virtual bool doEncode(CParamPool &IOBuff, bool bFemale = false);

public:
    std::list<CReward>  m_listMaleRewards;
    std::list<CReward>  m_listFemaleRewards;
};

//////////////////////////////////////////////////////////////////////////

class CSocialShareActivityData
{
public:
    CSocialShareActivityData();
    virtual ~CSocialShareActivityData();

public:
    virtual bool doEncode(CParamPool &IOBuff, bool bFemale = false);

public:
    unsigned char   m_nActivityType;            // 活动ID
    bool            m_bExhibit;                 // 是否展示大图片
    int             m_nWeight;                  // 权重
    unsigned int    m_nBeginTime;               // 开始时间
    unsigned int    m_nEndTime;                 // 结束时间
    std::string     m_strTitle;                 // 标题
    std::string     m_strContent;               // 内容
    std::string     m_strBigImageName;          // 大图片
    std::string     m_strSmallImageName;        // 小图片
    std::list<CRewardTitleInfo *> m_listReward; // 奖励物品信息
    bool            m_bIsReward;                // 是否获得奖励
    bool            m_bIsOpen;                  // 是否开启
    bool            m_bIsShow;                  // 是否显示

    // 发送其中一个
    std::string     m_strMaleShareText;         // 分享文字信息(男)
    std::string     m_strFemaleShareText;       // 分享文字信息(女)

    // 不需要发送给客户端
    std::string     m_strMailTitle;             // 邮件标题
    std::string     m_strMailContent;           // 邮件内容
};

class CMallSocialShareActivityData : public CSocialShareActivityData
{
public:
    CMallSocialShareActivityData() {}
    ~CMallSocialShareActivityData() {}

public:
    bool doEncode(CParamPool &IOBuff, bool bFemale = false);

    void CopyForEncode(const CMallSocialShareActivityData *pData);

public:
    // 发送其中一个
    std::string     m_strMaleActionName;        // 男性动作名
    std::string     m_strFemaleActionName;      // 女性动作名

    // 不需要发送给客户端
    int             m_nRewardItemTime;          // 奖励试穿服饰期限，单位天
    std::set<int>   m_setClothPos;              // 奖励试穿服饰的部位
};

class CPhotoSocialShareActivityData: public CSocialShareActivityData
{
public:
    CPhotoSocialShareActivityData() {}
    ~CPhotoSocialShareActivityData() {}

public:
    void CopyForEncode(const CPhotoSocialShareActivityData *pData);

public:
    // 不需要发送给客户端
    int             m_nPeopleNumber;            // 拍照人数下限
};

enum ELongactingCumulativeRechagreError
{
    ELCRE_Scuess,
    ELCRE_HaveGetReward,           //已经领取
    ELCRE_RewardIndexError,        //领奖序列错误
    ELCRE_ConfigError,             //配置错误
    ELCRE_TotalPointNotEnough,     //总充值不够
};

class LongactingCumulativeRechargeConfig
{
public:
    LongactingCumulativeRechargeConfig();
    virtual ~LongactingCumulativeRechargeConfig();

public:
    void doEncode(CParamPool &IOBuff, char cSex) const;

public:
    unsigned int        m_nRewardIndex;         // 奖励序列
    unsigned int        m_nNeedMCoin;           // 需要的M币
    std::list<CItem>    m_listMaleItemReward;   // 男性物品奖励
    std::list<CItem>    m_listFemaleItemReward; // 女性物品奖励
    unsigned int        m_nMoney;               // 金券奖励
    unsigned int        m_nBindMCoin;           // 绑定M币奖励

};

class LongactingRechargeRole
{
public:
    LongactingRechargeRole();
    virtual ~LongactingRechargeRole();

public:
    int             m_nSex;
    std::string     m_strRoleName;
    unsigned int    m_nTotalMPoint;
};

#endif



