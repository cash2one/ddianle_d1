/********************************************************************
	created:	20010-11-04
	author:		mei jiuhua
	filename: 	dataStructure\DataStruct_DB.h	
	purpose:	这里定义访问数据库需要的一些结构和算法函数，
				这个文件和对应的cpp文件没必要也不能被客户端程序所包含
*********************************************************************/

#ifndef __DATASTRUCT_DB_H__
#define __DATASTRUCT_DB_H__

#include "datainfor/ServerConfig.h"
#include "./DataStruct_Couple.h"
#include "DataStruct_Base.h"
#include "datainfor/XQMap.h"
#include "../server/Handbook/HandbookStruct.h"

#include <functional>
#include <map>
#include <set>
#include <list>
#include <bitset>


class CParamPool;

#define ROLEDATAEX_SIZE		128
//////////////////////////////////////////////////////////////////////////


//更新物品的数量
typedef struct ITEMINFO_Count
{
	__int64			nItemID		;//物品ID
	unsigned short  nCount		;//数量
}*LPITEMINFO_Count;

//更新一个物品的位置
typedef struct ITEMINFO_Position
{
	__int64			nItemID		;//物品ID
	unsigned short  nIndex		;
	unsigned short  nColumn		;//在那个地方，比如身上，或者装备栏
}*LPITEMINFO_Position;
//更新2个物品的位置
typedef struct ITEMINFO_Position2
{
	ITEMINFO_Position aItemData[2];

}*LPITEMINFO_Position2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#pragma pack(1)
//计费信息
typedef struct BILLINFO
{
	unsigned int nAccount;  //账号id
	unsigned int nPoint;	//剩余点数
	unsigned int nBindPoint;//绑定币
	unsigned int nChange;   //抵用卷
	unsigned int nQueryIdx;
	unsigned int nTotalPoint;  //充值总点数
	unsigned int nTotalBindPoint; //累计绑定点数
	unsigned int nTotalGift;//已领取累计礼品的点数

	BILLINFO()
	{
		memset(this, 0, sizeof(BILLINFO));
	}

	const BILLINFO& operator=(const BILLINFO& rhs)
	{
		memcpy(this,&rhs,sizeof(BILLINFO));
		return *this;
	};

	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);

}*LPBILLINFO;


struct CoolDownData
{
	unsigned short m_nType		;	//type标志
	int            m_nDuration	;	//到期时间
	CoolDownData():m_nType(1),m_nDuration(0)
	{
	}
	CoolDownData(unsigned short nType,int nDuration):m_nType(nType),m_nDuration(nDuration)
	{
	}
	CoolDownData& operator=(const CoolDownData& src)
	{
		memcpy(this, &src, sizeof(CoolDownData));
		return *this;
	}

	bool doEncode(CParamPool &IOBuff)const;
	bool doDecode(CParamPool &IOBuff);
};

enum GENE_TYPE 
{
	GeneType_None,
	GeneType_Duration,		//时长持续
	GeneType_Matchs,		//比赛场次
	GeneType_InRole,		//物品在人物身上
	GeneType_Equip,			//装备基因
	GeneType_OneTime,		//一次使用
	GeneType_DurationByParam,//时长持续，如果nParam1相同则累加时间，否则重置时间
	GeneType_Couple,		// 夫妻双方在线穿戴
};

enum ESyncToDBType
{
    ESyncToDBType_None,
    ESyncToDBType_Insert,
    ESyncToDBType_Update,
};


enum EGeneNotifyAction
{
    EGeneNotifyAction_None,
    EGeneNotifyAction_Add,
    EGeneNotifyAction_Del,
    EGeneNotifyAction_Reset,
};


struct GeneData
{
	unsigned short   nGeneId;	                        //基因id
	unsigned char    nType;		                        //执行类型
	int              nParam1;			                //参数  作用参数
	int              nParam2;			                //参数  作用时间或者作用次数
	char             szCmdParam[MAX_GENE_PARAM_LEN];    //参数  备用
	
	GeneData()
	{
		memset( this, 0x0, sizeof(GeneData) );
	}

	GeneData(unsigned short _nGeneId,int _nParam1,int _nParam2,const char *_szCmdParam)
	{
		nGeneId = _nGeneId;
		nType = 0;
		nParam1 = _nParam1;
		nParam2 = _nParam2;

		if( strlen( _szCmdParam ) < MAX_GENE_PARAM_LEN )
		{
			strcpy( szCmdParam, _szCmdParam );
		}
	}

	GeneData(const GeneData& src)
	{
		*this = src;
	}

	GeneData& operator=(const GeneData& src)
	{
		if ( &src != this )
		{
			nGeneId = src.nGeneId;
			nType = src.nType;
			nParam1 = src.nParam1;
			nParam2 = src.nParam2;
			memcpy(szCmdParam, src.szCmdParam, MAX_GENE_PARAM_LEN-1); 
		}

		return *this;
	}

	bool doEncode(CParamPool & IOBuff) const;
	bool doDecode(CParamPool & IOBuff);
};


class MallGood
{
public:
    MallGood();
    MallGood& operator = (const MallGood& rhs);

    bool doEncode(CParamPool& IOBuff);

    void AddDesireCount(int nAddCount = 1);
    int DesireCount() const;

public:
    itemtype_t     m_nType;             // 物品类型
    unsigned char  m_nState;            // 是否贩卖
    unsigned char  m_nDiscount;         // 折扣百分比，10表示10%

    unsigned char  m_nIsVip;            // 是否vip商品
    unsigned char  m_nIsPrecious;       // 是否珍藏版
    unsigned char  m_nIsNew;            // 是否新品
    unsigned char  m_nHot;              // 热卖
    unsigned char  m_nIsCoupon;         // 收券
    unsigned char  m_nUnique;           // 尊享

    unsigned char  m_nPayType;          // 支付类型
    unsigned char  m_nUseChangePercent; // 金券最大使用百分比，10表示10%

    unsigned int   m_nPrice;            // 价格[1天时效或1个道具]
    unsigned int   m_nPrice1;           // 价格[7天时效或5个道具]
    unsigned int   m_nPrice2;           // 价格[30天时效或10个道具]
    unsigned int   m_nPrice3;           // 价格[永久时效或20个道具]

    unsigned int   m_nSell_time0;       // 出售开始时间
    unsigned int   m_nSell_time1;       // 出售结束时间
    unsigned int   m_nDiscount_time0;   // 打折开始时间
    unsigned int   m_nDiscount_time1;   // 打折结束时间

    unsigned int   m_nDesireCount;      // 许愿次数

    unsigned short m_nHotIdx;           // 热卖排序优先级
};

//////////////////////////////////////////////////////////////////////////

class FriendInfo
{
public:
    FriendInfo();
    explicit FriendInfo( unsigned int nRoleID );
    FriendInfo( const FriendInfo &info );

    explicit FriendInfo( unsigned int nRoleID, const std::string &rRoleName, const std::string &rNotes, 
        unsigned char nSex, unsigned short nLeve, unsigned char nRelationType, unsigned char nCallBy, 
        unsigned char nCallTo, bool bIsVIP, unsigned short nVIPLevel );

    bool operator == ( const FriendInfo &rhs ) const;

    bool FindByID( unsigned int nRoleID ) const;
    bool FindByName( const std::string &rRoleName ) const;

    void SetName( const std::string &rRoleName );
    inline const std::string& GetName() const { return m_strName; }

    void doEncode( CParamPool &IOBuff ) const;
    void doDecode( CParamPool &IOBuff );
public:
    unsigned int    m_nRoleID;              // 角色序列号
    unsigned char   m_nSex;                 // 性别
    string          m_strName;              // 角色名称
    string          m_strNotes;             // 备注
    unsigned short  m_nLevel;               // 等级	
    unsigned char   m_nRelationType;        // 称呼类型
    unsigned char   m_nCallBy;              // 朋友对我的称呼
    unsigned char   m_nCallTo;	            // 对朋友的称呼	
    bool            m_bIsVIP;				// 好友是否是VIP
    int             m_nVIPLevel;			// 好友VIP等级
    bool            m_bIsOnline;            // 是否在线
    unsigned int    m_nOfflineTime;         // 离线时间(如果玩家登陆好友不在线，无法获取离线时间)
};

//////////////////////////////////////////////////////////////////////////

//附件
struct MailItem
{
	itemtype_t      m_nType;         //物品类型
	itemcount_t     m_nCount;	     //数量
	int             m_nValidTime;    //有效期
	unsigned char   m_nState;        //是否领取
	int				m_nDynamicBoxRelatedValue; // 动态礼盒关联值

	MailItem(){
		m_nType = 0;
		m_nCount = 0;
		m_nValidTime = 0;
		m_nState = 0;
		m_nDynamicBoxRelatedValue = 0;
	}

	MailItem(itemtype_t nType,unsigned short cCount,int nValidTime, int nDynamicBoxRelatedValue = 0 )
	{
		m_nType = nType;
		m_nCount = cCount;
		m_nValidTime = nValidTime;
		m_nState = 0;
		m_nDynamicBoxRelatedValue = nDynamicBoxRelatedValue;
	}

	MailItem& operator =(const MailItem& rhs)
	{
		if(this == &rhs)	return *this;

		m_nType = rhs.m_nType;
		m_nCount = rhs.m_nCount;
		m_nValidTime = rhs.m_nValidTime;
		m_nState = rhs.m_nState;
		m_nDynamicBoxRelatedValue = rhs.m_nDynamicBoxRelatedValue;
		return *this;
	}

	void doEncode(CParamPool &IOBuff) const
	{
		IOBuff.AddUInt(m_nType);
		IOBuff.AddUShort(m_nCount);
		IOBuff.AddInt(m_nValidTime);
		IOBuff.AddUChar(m_nState);
	}

	void doDecode(CParamPool &IOBuff)
	{
		m_nType = IOBuff.GetUInt();
		m_nCount = IOBuff.GetUShort();
		m_nValidTime = IOBuff.GetInt();
		m_nState = IOBuff.GetUChar();
	}
};

typedef std::multimap<itemtype_t,MailItem>  mapMailItems;

class MailBaseInfo
{
public:
    MailBaseInfo();
    MailBaseInfo(const MailBaseInfo &rMailBaseInfo);
    virtual ~MailBaseInfo() {}

    MailBaseInfo& operator =(const MailBaseInfo& rMailBaseInfo);

    unsigned int GetCurValidTime() const;

    void SetSenderName(const std::string &rSenderName);
    const std::string& GetSenderName() const { return m_strSenderName; }

    void SetTitle(const std::string &rTitle);
    const std::string& GetTitle() const { return m_strTitle; }

    void doEncode(CParamPool &IOBuff) const;
    void doEncodeToServer(CParamPool &IOBuff) const;

    void doDecodeFromServer(CParamPool &IOBuff);

public:
    __int64         m_nMailID;
    short           m_nType;
    unsigned int    m_nSendTime;
    unsigned char   m_bRead;
    unsigned char   m_nHasItem;
    int             m_nMoney;
    int             m_nBindMCoin;
    unsigned int    m_nSenderID;

protected:
    std::string     m_strSenderName;
    std::string     m_strTitle;
};

struct BILLGIFT //充值送礼
{
	int nGiftType;//礼品类型
	int nGiftCount;//礼品数量
	char szDesc[MAX_GIFT_DESC_LEN];

	BILLGIFT()
	{
		nGiftType = 0;
		nGiftCount = 0;
		memset(szDesc,0,MAX_GIFT_DESC_LEN);
	}
};

struct LotterInfo
{
	__int64 uuid;
	//char szUsername[MAX_NAME_LEN];
	char szCode[MAX_LOTTERCODE_NUM+1];
	LotterInfo()
	{
		//memset(szUsername,0,MAX_NAME_LEN);
		memset(szCode,0,MAX_LOTTERCODE_NUM+1);
		uuid = 0;
	}
};

class MailInfo : public MailBaseInfo
{
public:
    MailInfo();
    MailInfo(const MailInfo &rMailInfo);
    MailInfo& operator=(const MailInfo &rMailInfo);

    void SetContent(const std::string &rContent);
    const std::string& GetContent() const { return m_strContent; }

    bool doEncode(CParamPool &IOBuff) const;

    bool doEncodeToServer(CParamPool &IOBuff) const;
    bool doDecodeFromServer(CParamPool & IOBuff);

    // 加动态礼盒关联值
    bool doEncodeToServer1(CParamPool &IOBuff) const;
    bool doDecodeFromServer1(CParamPool & IOBuff);

protected:
    std::string     m_strContent;

public:
    mapMailItems    m_mapItems;
};

class ChargeRewardInfo
{
public:
	unsigned int m_nRoleID;
	stringw m_strRoleName;
	int m_nChargeRewardType;
	int m_nRewardBindMCoin;
	int m_nRewardMoney;
	MailInfo m_mailInfo;
	int m_nVersionType;
	ChargeRewardInfo():m_nRoleID(0), m_nChargeRewardType(0), m_nRewardBindMCoin(0), m_nRewardMoney(0), m_nVersionType(0){}
};


class ChongZhiInfo
{
public:
    unsigned short  nPID;                               // 平台ID
    std::string strOrderID;                             // 订单ID
    unsigned int nAccount;                              // 角色账户ID
    unsigned int nRoleID;                               // 角色ID
    int nVersionType;                                   // 版本类型(充值|免费)
    unsigned int nAmount;                               // 充值M币
    unsigned int nAllAmount;                            // 充值得到的所有M币
    unsigned int nCurrency;                             // 实际货币
    int nFeedVersionMoneyRewardRatio;                   // 收费版充值 金券返利比例( 将作废 )
    int nFeedVersionMoneyReward;                        // 收费版充值 金券奖励，用于单独记录log
    int nRechargeBindMCoinReward;                       // 充值 绑定Mcoin 奖励
    int nRechargeMoneyReward;                           // 充值 金券 奖励
    EChargeRewardType eChargeRewardType;                // 充值奖励类型，例如首充
    EChangeBillCause eChangeBillCause;                  // mcoin本金变化原因
    EChangeBindBillCause eChangeBindBillRewardCause;    // 绑定mcoin奖励原因
    EChangeMoneyCause eChangeMoneyRewardCause;          // 金券奖励原因
    bool bOnline;                                       // 玩家是否在线
    std::string strDeviceID;                            // 充值设备号
    unsigned int nChargeTime;                           // 充值时间
    bool bIsRetryOrder;                                 // 是否是掉单补发的订单（用于在GameServer上给玩家发邮件提醒）
    std::string strPartnerOrderID;                      // 平台订单号

	unsigned int m_nFixedChargeBindPoint;				// 固定首冲赠送的绑定M币

    ChargeRewardInfo* pChargeReward;                    // 充值奖励结构，用于服务器之间交互

    ChongZhiInfo()
    {
        nPID = 0;
        strOrderID = "";
        nAccount = 0;
        nRoleID = 0;
        nVersionType = 0;
        nAmount = 0;
        nAllAmount = 0;
        nCurrency = 0;
        nFeedVersionMoneyRewardRatio = 0;
        nFeedVersionMoneyReward = 0;
        nRechargeBindMCoinReward = 0;
        nRechargeMoneyReward = 0;
        eChargeRewardType = EChargeRewardType_None;
        eChangeBillCause = EChangeBillCause_Unknown;
        eChangeBindBillRewardCause = EChangeBindBillCause_Unknown;
        eChangeMoneyRewardCause = EChangeMoneyCause_Unknown;
        bOnline = false;
        nChargeTime = 0;
        bIsRetryOrder = false;
        strPartnerOrderID = "";
        pChargeReward = NULL;
		m_nFixedChargeBindPoint = 0;
    }

    ~ChongZhiInfo()
    {
        if ( pChargeReward != NULL )
        {
            delete pChargeReward;
            pChargeReward = NULL;
        }
    }
};


class ChongZhiByPUIDInfo
{
public:
	ChongZhiInfo czInfo;
	std::string strPUID;

	ChongZhiByPUIDInfo()
	{
		strPUID = "";
	}
};

struct EventInfoDB
{
	unsigned int m_nRoleID;
	unsigned char m_nEventID;
	unsigned int m_nCount;
	unsigned int m_nTime;//当此值是0时，底层会取当前时间

	EventInfoDB():m_nRoleID(0), m_nEventID(0), m_nCount(0), m_nTime(0){}
	EventInfoDB(unsigned int nRoleID, unsigned char nEventID, unsigned int nCount, unsigned int nTime):
	m_nRoleID(nRoleID), m_nEventID(nEventID), m_nCount(nCount), m_nTime(nTime){}
};

struct FixedChargeRecordSingle
{
	unsigned int m_nAccount;
	unsigned int m_nMCoin;
	unsigned int m_nChargeTime;

	FixedChargeRecordSingle();
};

struct FixedChargeRecordItem
{
	unsigned int m_nMCoin;
	unsigned int m_nChargeTime;

	FixedChargeRecordItem();
};

struct FixedChargeRecord
{
	unsigned int m_nAccount;

	std::vector<FixedChargeRecordItem> vecRecords;

	FixedChargeRecord();
};

struct RechargeRewardInfo
{
	ChongZhiInfo* pChongZhiInfo;
	ChongZhiByPUIDInfo* pChongZhiByPUIDInfo;
	unsigned int nAccountID;
	unsigned int nRoleID;
	string strRoleName;
	unsigned char m_nSex;
	int m_nMoney;
	Version_Type m_eVersionType;
	EventInfoDB m_firstChargeEvent;//首充事件
	EventInfoDB m_firstChargeRewardEvent;//首充奖励事件
	FixedChargeRecordItem m_fixedChargeRecord; // 固定首冲记录

	RechargeRewardInfo()
	{
		pChongZhiInfo = NULL;
		pChongZhiByPUIDInfo = NULL;
		nAccountID = 0;
		nRoleID = 0;
		m_nSex = 0;
		m_nMoney = 0;
		m_eVersionType = Version_Free;
	}
};

class ChargeAction
{
public:
    ChargeAction();
    ChargeAction(const ChargeAction & action);
    virtual ~ChargeAction();

public:
    void doDecode(CParamPool &IOBuff);

public:
    std::string     m_strPUID;
    char            m_nPID;
    unsigned int    m_nRoleID;
    char            m_nAction;
    int             m_nMoney;
    std::string     m_strOrderID;
};

struct LockAccountInfo
{
	unsigned short nPID;
	std::string strPUID;
	std::string strTransID;
	int nBlockType;
	unsigned int nBlockTime;
	unsigned int nAccount;
	LockAccountInfo()
	{
		nPID = 0;
		nBlockType = 0;
		nAccount = 0;
		nBlockTime = 0;
	}
};
struct SendGift
{
	__int64 uuid;
	unsigned int nAccount;

	int nItemType;
	short nCount;

	SendGift()
	{
		uuid = 0;
		nAccount = 0;
		nItemType = 0;
		nCount = 0;
	}
};

struct Suggestion
{
	char rolename[MAX_NAME_LEN];
	char content[MAX_SUGGESTION_LEN];

	Suggestion()
	{
		memset(rolename,0,MAX_NAME_LEN);
		memset(content,0,MAX_SUGGESTION_LEN);
	}
};

//#pragma pack()

class MALL_LOG
{
public:
	// 物品支付信息
	struct GoodsPayInfo
	{
		itemtype_t m_itemType;
		unsigned int m_uMoney;
		unsigned int m_uBindMCoin;
		unsigned int m_uMCoin;
		unsigned char m_nSelType;
	};

	unsigned int    m_nRoleID;
	unsigned int    m_nCostPoint;
	unsigned int    m_nCostBindPoint;
	unsigned int    m_nCostChange;
	unsigned int    m_nCostMoney;
	unsigned int    m_nDestSendRoleID;
	__int64         m_nItemID;

//	std::map<itemtype_t,unsigned char> listItem;
	typedef std::vector<GoodsPayInfo> VecGoodsPayInfo;
	VecGoodsPayInfo m_vecGoodsPayInfo;

	MALL_LOG(){
		m_nRoleID = 0;
		m_nCostPoint = 0;
		m_nCostBindPoint = 0;
		m_nCostChange = 0;
		m_nCostMoney = 0;
		m_nDestSendRoleID = 0;
		m_nItemID = 0;
	}
};

typedef MALL_LOG* LPMALL_LOG;

struct MallSigleItemLog
{
	unsigned int nRoleID;
	unsigned int nPayType;
	itemtype_t nItemType;
	unsigned int nPoint;
	unsigned int nBindPoint;
	unsigned int nMoney;

	MallSigleItemLog()
	{
		nRoleID = 0;
		nPayType = 0;
		nItemType = 0;
		nPoint = 0;
		nBindPoint = 0;
		nMoney = 0;
	}
};

class CKeepOnlineReward
{
public:
	CKeepOnlineReward();
	~CKeepOnlineReward();

public:
	void doEncode(CParamPool & IOBuff);
    bool doEncodeToCache( CParamPool &IOBuff ) const;
    bool doDecodeFromCache( CParamPool &IOBuff );

	int GetRemainTime();

public:
	int m_nIndex;
	int m_nRequireTime;
	int m_nCumulativeTime;
	unsigned int m_nTimingDay;
	bool m_bFinished;
	bool m_bValid;
	bool m_bGotReward;
};

struct RolePushRatingDB
{
	RolePushRatingDB()
	{
		nNoPushRatingTime = 0;
		nPushRatedLevel = 0;
		nPushRatedOnlineTime = 0;
		nPushRatedDailyQuest = 0;
		nPushRatedDailyShoping = 0;
		nPushRatedCount = 0;
		nPushRatedCountTime = 0;
	}


	int nPushRatedLevel;				// 玩家升级推送
	unsigned int nNoPushRatingTime;		// "今天"不要推送, "今天"的时间
	unsigned int nPushRatedOnlineTime;	// 在线时长推送
	unsigned int nPushRatedDailyQuest;	// 完成任务推送
	unsigned int nPushRatedDailyShoping;// 商城购买推送
	int nPushRatedCount;				// 本周推送的次数
	unsigned int nPushRatedCountTime;	// 本周第一次推送的时间
};


// 安全保护问题
struct SecurityQuiz_DB
{
	SecurityQuiz_DB()
	{
		memset(nQuizID, 0, sizeof(nQuizID));
		memset(szAnswer, 0, sizeof(szAnswer));
	}

	unsigned char	nQuizID[MAX_SQUIZ_COUNT];
	char			szAnswer[MAX_SQUIZ_COUNT][MAX_SQUIZ_LEN];
};

class CAuctionNotGivePointInfo;

class RoleActivenessDB
{
public:
    RoleActivenessDB()
        : m_nRoleID(0)
        , m_nScore(0)
        , m_nUpdateTime(0)
        , m_strRewardLevelScore("")
        , m_strTargetProgress("")
        , m_strDanceMode("")
    {
    }

    ~RoleActivenessDB() {}

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << m_nRoleID << m_nScore << m_nUpdateTime << m_strRewardLevelScore
            << m_strTargetProgress << m_strDanceMode;
        return true;
    }
    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> m_nRoleID >> m_nScore >> m_nUpdateTime >> m_strRewardLevelScore
            >> m_strTargetProgress >> m_strDanceMode;
        return true;
    }

public:
    unsigned int m_nRoleID;
    int m_nScore;
    unsigned int m_nUpdateTime;
    std::string m_strRewardLevelScore;
    std::string m_strTargetProgress;
    std::string m_strDanceMode;
};

class CRolePuzzleActivityDB
{
public:
    CRolePuzzleActivityDB();
    ~CRolePuzzleActivityDB();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    int m_nPuzzleID;
    int m_nUpdateTime;
    int m_nFinishTime;
    char m_arFragments[PUZZLE_FRAGMENTS_COUNT + 1];
};

class GiftDB
{
public:
    GiftDB()
    {
        m_nGiftID = 0;
        m_nExpiredTime = 0;
    }
    ~GiftDB(){}

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << m_nGiftID << m_nExpiredTime;
        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> m_nGiftID >> m_nExpiredTime;
        return true;
    }

    int				m_nGiftID;			//礼品编号
    unsigned int	m_nExpiredTime;		//到期时间
};

class MagicArrayRecord
{
public:
    MagicArrayRecord();
    MagicArrayRecord(const MagicArrayRecord & magic);
    virtual ~MagicArrayRecord();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
	unsigned int		m_nMagicArrayID;
    unsigned int        m_nAccumulatedCallCount;
    unsigned int        m_nResetTime;
    unsigned int        m_nFreeCallTime;
    bool                m_bHaveFirstCall;
    unsigned int        m_nTodayOnceCallCount;
};

class MagicArrayExchangeRecord
{
public:
    MagicArrayExchangeRecord();
    MagicArrayExchangeRecord(const MagicArrayExchangeRecord & record);
    virtual ~MagicArrayExchangeRecord();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    unsigned int       m_nRefreshTime;
    std::map<unsigned int, unsigned int>    m_mapRefreshGroup;
};


struct PetDB
{
public:
    unsigned int        nID;                          // 宠物ID，由数据库生成
    unsigned char       nState;                       // 状态
    unsigned short      nLevel;                       // 等级
    unsigned int        nExp;                         // 当前等级经验
    unsigned char       nQualityType;                 // 品质类型
    unsigned char       nEvolutionTimes;              // 进化次数
    unsigned char       nAttributeType;               // 宠物属性
    unsigned short      nTypeID;                      // 宠物类型ID, Set方法手写
    unsigned char       nPotentialID;                 // 宠物潜质, Set方法手写
    std::string         strName;                      // 宠物名
    unsigned short      nEndurance;                   // 当前耐力
    unsigned int        nEnduranceRecoveryTime;       // 当前耐力恢复时间
    unsigned int        nResetSkillTime;              // 技能重置时间
    int                 nResetSkillTimes;             // 当天技能重置次数
    std::string         strSkillList;                   // 技能列表

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << nID << nState << nLevel << nExp << nQualityType << nEvolutionTimes
            << nAttributeType << nTypeID << nPotentialID << strName << nEndurance
            << nEnduranceRecoveryTime << nResetSkillTime << nResetSkillTimes
            << strSkillList;
        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> nID >> nState >> nLevel >> nExp >> nQualityType >> nEvolutionTimes
            >> nAttributeType >> nTypeID >> nPotentialID >> strName >> nEndurance
            >> nEnduranceRecoveryTime >> nResetSkillTime >> nResetSkillTimes
            >> strSkillList;
        return true;
    }
};

enum EMedalState
{
    eMedalState_Underway = 0,       //进行中
    eMedalState_Activated = 1,      //激活
    eMedalState_Invalid  = 2,       //失效
    eMedalState_Show     = 3,       //展示
};

class MedalUnitDB
{
public:
    MedalUnitDB();
    virtual ~MedalUnitDB();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

    int  getID() const { return m_nMedalID; }
    void setID( int nMedalId ) { m_nMedalID = nMedalId; }

    int  getCurState() const { return m_nCurrentState; }
    void setCurState( int nState ) { m_nCurrentState = nState; }

    int  getGetTime() const { return m_nGetTime; }
    void setGetTime( int nGetTime ) { m_nGetTime = nGetTime; }

private:
    int m_nMedalID;          //id
    int m_nCurrentState;     //当前勋章状态
    int m_nGetTime;          //激活时间
};

class MedalScoreDB
{
public:
    MedalScoreDB();
    virtual ~MedalScoreDB()
    {}

    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);

    int m_nMedalWearScore;
    int m_nMedalCumulativeScore;
};


class MedalConditionDB
{
public:
    MedalConditionDB();
    MedalConditionDB( int nID, int nCurProgress, int nAreadyReward );
    ~MedalConditionDB();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

    int GetID() const { return m_nID; }
    int GetCurProgress() const { return m_nCurProgress; }
    int GetAreadyReward() const { return m_nAreadyReward; }

private:
    int m_nID;
    int m_nCurProgress;
    int m_nAreadyReward;
};

enum ERoleInfoCache
{
    ERoleInfoCacheQuestNew,
    ERoleInfoCacheItem,
    ERoleInfoCacheClothEffect,
    ERoleInfoCacheSettings,
    ERoleInfoCacheActivity,
    ERoleInfoCacheOther,

    ERoleCacheInfoEnumMax
};

class BigMamaRoleInfoDB
{
public:
    BigMamaRoleInfoDB();
    ~BigMamaRoleInfoDB();

    unsigned int m_nPrizeStamp;
    unsigned int m_nJoinStamp;
};

typedef struct ROLEINFO_DB : public RoleAttrInfo
{
	ROLEINFO_DB();
	~ROLEINFO_DB();
	unsigned int  nAccount;							//账号

	std::list<DBItem>  listItem;

	// 好友
	std::list<FriendInfo> listFriend;						// 好友列表
	std::list<FriendInfo> listBlackList;					// 黑名单

	//std::list< QUESTINFO >listQuest;						// 任务 // 已废弃！
	std::list< QUESTNEWINFO > listQuestNew;					// 任务
	//std::map<short, __int64>mapQuestHistory;				// 历史任务 // 已废弃！
	//std::list< CRepeatedHistoryQuest >listRepeatedQuest;	// 任务历史 // 已废弃！
	int m_nFresherBestMusicGrade;							// 新手最好成绩
	int m_nMallBuyCount;									// 商城购买次数

	std::list< CoolDownData >  listCoolDown;
	std::list< GeneData >  listGene;						// 这里的有效时间是到期的时间点
	// 称号
	std::list< CTitleInfo > listTitle;
	//成就
	std::list<CAchievementInfo>	listAchievement;
	// 成就物品相关
	unsigned int	nHistoryClothNum;						// 累计拥有的任意时效的衣服数量 
	unsigned int	nHistoryWishItemNum;					// 累计加入愿望栏的物品数量
	unsigned int	nGainWishItemNum;						// 累计获得的愿望物品数量 
	unsigned int	nHistoryBadageNum;						// 累计获得的徽章的数量
	unsigned int	nHistoryUseBouquetNum;					// 累计使用花束的数量

	CWeeklyEvent	bigSpeakersUseNumWeekly;				// 一周内使用大喇叭的数量 
	CWeeklyEvent	smallSpeakersUseNumWeekly;				// 一周内使用小喇叭的数量 
	CWeeklyEvent	sendFlowersNumInSevenWeekly;			// 一周内送花数量
	// 成就战斗相关
	unsigned short	nHistoryTaikoComboMusicNum;				// 累计全连的歌曲数
	unsigned int	nAutoMatchWithOppositeSexNum;		// 通过快速匹配模式与异性一起游戏的人数
	std::map<int, map<int, CFightResult > >	mapFightResult;	// 按模式，按难度记录	
	// 签到
	CCheckInInfo	   checkInInfo;
	//list<unsigned char>	listCheckInGift; // 已废弃！
	// VIP
	CRoleVIPInfo VIPInfo;
    unsigned int nMallSendItemCostBill;                //商城赠送物品消耗M币数量
    unsigned int nMallSendItemTime;                    //商城赠送物品时间
	map<unsigned char, itemtype_t> mapVIPCards;		// 类型，数量
	BILLINFO bi;

	//UCVIP(事件)
	std::set<int>  setUCVIPReward; //UCVIP特权领取标志

	//事件
	bool			bCompleteTeachGuide;
	unsigned int	m_nGuideData;
	char			nCompleteProcGuide;
	bool			bCompleteAnimation;
	bool			bCompleteFirstPaymentTag;
	bool			bCanSendFirstPaymentReward;
	bool			bReceivedInvitationReward;
	int             m_nVersionType;			//版本ID
	unsigned int m_nFirstPaymentMoneyCount;
	unsigned int m_nFirstPaymentTime;		//首充时间
	unsigned int m_nGetFirstPaymentRewardTime;//领取首充奖励时间
	std::set<int>   m_setFresherReward;		//新手七天乐活动奖励领取标志
	std::vector<int>	m_CompensationReward;  //补偿领取数据
	std::set<int>   m_setFresherQuestReward;//新手七天目标奖励领取标志
	bool			m_bHasEnterConstellationLobby; //是否进入过星恋魔法阵
	std::list<SConstellationCardInfo> m_listCard;
	int             m_nConstellationMatchCount;
	unsigned int    m_nConstellationMatchTime;
	unsigned int    m_nCosntellationRoomID;
	unsigned int    m_nCosntellationRoomCreateTime;
	int             m_nCosntellationRoomLine;
	bool          m_bDanceGroupFirstAddPowerReward; //舞团每日首次登录奖励
	unsigned int m_nDanceGroupFirstAddPowerRewardTime; //上次领取奖励的时间
	bool          m_bPropretaryActivityReward; //IOS专有版奖励领取
	bool			m_bReceiveProcGuideReward;		//领取新手引导奖励

    unsigned int    m_nLastChangeBirthdayTime;      // 上次更改生日的时间

    unsigned int    m_nRecommendRefreshCount;       // 推荐好友刷新次数
    unsigned int    m_nLastRecommendResetTime;      // 推荐好友上次重置时间
    bool            m_bRedEnvelopeAnonymity;        // 红包匿名

    // 舞团试炼次数/重置时间
    unsigned short  m_nDanceGroupChallengeChance;
    unsigned int    m_nDanceGroupChallengeChanceResetTime;
    // 舞团试炼购买次数/重置时间
    unsigned short  m_nDanceGroupChallengeBuyChance;
    unsigned int    m_nDanceGroupChallengeBuyChanceResetTime;

	//mail
	XQMap<__int64, MailInfo *>	mapMailBox;

	//累计在线时间奖励
	CKeepOnlineReward keepOnlineRewardInfo;
	
	//玩家自定义配置
	CPlayerSetting m_playerSetting;

	// 情侣，夫妇信息
	CCoupleInfo m_coupleInfo;

	// 离婚协议书
	CDivorceAgreementInfo m_DivorceAgreementInfo;
	std::list<CDivorceAction> m_DivorceAction;

	//SecurityQuiz_DB m_securityQuiz;			// 账号安全保护问题 // 已废弃！
	RolePushRatingDB		sPushRatingDB;	// 评价消息推送事件
	
	// 魔法神灯
	MagicLamp	m_MagicLamp;
	std::list<MainlineParcloseRecord>  m_MainlineParcloseList; // 主线关卡列表
    std::list<ThemeParcloseRecord> m_ThemeParcloseList; // 主题关卡列表
	std::set<int> m_LuckStarReward;                  // 许愿星奖励
    std::map<unsigned int,unsigned int> m_PlayerDebrisInfo; //碎片信息
    std::map<unsigned int,FriendContactInfo>    m_mapFriendContactInfo; // 好友交互信息

	//服饰搭配
	std::list<RoleClothGroup> m_listClothGroup;

	//社交分享
	std::map<int, unsigned int> m_mapSocialShareFirstTime;

	// 月卡
	MonthCardDBTable m_mapMonthCard;

    // 限量销售
    std::map<unsigned short, unsigned int>  m_mapLimitedEditionBuy;
    // 竞价未发放物品
    std::map<long long, CAuctionNotGivePointInfo>   m_mapAuctionNotGiveInfo;
	bool m_bKeyDataModified;	// 是否有关键数据更新, 包括: level, exp, clothGroup,

    // 活跃度系统数据
    RoleActivenessDB m_activeness;

	// 体验卡数据：
	std::vector<itemtype_t> m_vecExperiencedProducts;
	unsigned short m_experiencedTimes;
	unsigned int m_experiencedStamp; 

    // 宠物信息
    std::map<unsigned int, PetDB, std::greater<unsigned int> >  m_mapPet;
    int m_nPetGuideStep;//宠物引导步骤

	// 信息中心
	std::vector<std::pair<unsigned int, unsigned int> > m_vecInfoCenterReaded; 

    // 长效累冲已领奖Index
    unsigned int                                        m_nLongactingCumulativeRechargeIndex; 
    FixedChargeRecord m_fixedChargeRecord;

    // 勋章系统
    std::map<int, MedalUnitDB> m_mapMedalUnit; 
    std::map<int, MedalConditionDB> m_mapMedalCondition;
    int m_nMedalWearScore;
    int m_nMedalCumulativeScore;


    // 衣服特效
    std::map<itemid_t,unsigned int>                     mapClothEffect;         // 衣服特效
    std::set<unsigned int>                              setRoleEffect;          // 角色拥有特效
    std::map<unsigned short, std::map<unsigned char, std::string> >  mapHandbookProgress;
    std::map<unsigned short, std::map<unsigned char, std::string> >  mapColorProgress;
    
    // 图鉴系统
//     std::map<int, std::list<CHandbookPersonalInfo*> > m_MapCostumePersonalInfo;
//     std::map<int, std::list<CHandbookPersonalInfo*> > m_MapEffectPersonalInfo;
    std::map<int, std::string > m_MapCostumePersonalInfo;
    std::map<int, std::string > m_MapEffectPersonalInfo;

    // 愿望
    std::list<itemtype_t>                             m_listDesiredItem;

    // 活动
    std::list<CRolePuzzleActivityDB>    m_listPuzzleActivityDB; // 拼图
    std::map<int, GiftDB>               m_mapGift;              // 玩家回归

    // 魔法幸运阵
    std::map<unsigned int, MagicArrayRecord>            m_mapMagicArrayRecord;
    MagicArrayExchangeRecord                            m_mapMagicArrayExchangeRecord;

    // 仓库容量
    StorageCapacityRecord                               m_StorageCapacity;

    BigMamaRoleInfoDB                   m_bigMamaRoleInfo;

	void doEncode(CParamPool &IOBuff);
	void doDecode(CParamPool &IOBuff);

public:
    typedef bool(ROLEINFO_DB::*CacheEncodeFunctor)(CParamPool &IOBuff);
    typedef void(ROLEINFO_DB::*CacheDecodeFunctor)(CParamPool &IOBuff);

public:
    void SetCachedFlag( size_t nFlag );
    bool IsFlagSet( size_t nFlag );
    std::string FlagsToString() const { return m_cachedFlags.to_string(); }

    static CacheEncodeFunctor GetCachEncode( ERoleInfoCache key );
    static CacheDecodeFunctor GetCacheDecode( ERoleInfoCache key );

private:
    // encode 检测返回值,decode检测异常，返回值不同
    bool CacheEncodeQuestNew( CParamPool &IOBuff );
    void CacheDecodeQuestNew( CParamPool &IOBuff );

    bool CacheEncodeItem( CParamPool &IOBuff );
    void CacheDecodeItem( CParamPool &IOBuff );

    bool CacheEncodeClothEffect( CParamPool &IOBuff );
    void CacheDecodeClothEffect( CParamPool &IOBuff );

    bool CacheEncodeSettings( CParamPool &IOBuff );
    void CacheDecodeSettings( CParamPool &IOBuff );

    bool CacheEncodeActivity( CParamPool &IOBuff );
    void CacheDecodeActivity( CParamPool &IOBuff );

    bool CacheEncodeOther( CParamPool &IOBuff );
    void CacheDecodeOther( CParamPool &IOBuff );

private:
    bool doEncodeQuestNew( CParamPool &IOBuff ) const;
    bool doDecodeQuestNew( CParamPool &IOBuff );

    bool doEncodeStorageCapacity( CParamPool &IOBuff ) const;
    bool doDecodeStorageCapacity( CParamPool &IOBuff );

    bool doEncodeClothGroup( CParamPool &IOBuff ) const;
    bool doDecodeClothGroup( CParamPool &IOBuff );

    bool doEncodeItem( CParamPool &IOBuff ) const;
    bool doDecodeItem( CParamPool &IOBuff );

    bool doEncodeCoolDown( CParamPool &IOBuff ) const;
    bool doDecodeCoolDown( CParamPool &IOBuff );

    bool doEncodeGene( CParamPool &IOBuff ) const;
    bool doDecodeGene( CParamPool &IOBuff );

    bool doEncodeClothEffect( CParamPool &IOBuff ) const;
    bool doDecodeClothEffect( CParamPool &IOBuff );

    bool doEncodePlayerSetting( CParamPool &IOBuff ) const;
    bool doDecodePlayerSetting( CParamPool &IOBuff );

    bool doEncodeCheckIn( CParamPool &IOBuff ) const;
    bool doDecodeCheckIn( CParamPool &IOBuff );

    bool doEncodeMonthCard( CParamPool &IOBuff ) const;
    bool doDecodeMonthCard( CParamPool &IOBuff );

    bool doEncodeSocialShare( CParamPool &IOBuff ) const;
    bool doDecodeSocialShare( CParamPool &IOBuff );

    bool doEncodePuzzle( CParamPool &IOBuff ) const;
    bool doDecodePuzzle( CParamPool &IOBuff );

    bool doEncodeGift( CParamPool &IOBuff ) const;
    bool doDecodeGift( CParamPool &IOBuff );

    bool doEncodeKeepOnlineReward( CParamPool &IOBuff ) const;
    bool doDecodeKeepOnlineReward( CParamPool &IOBuff );

    bool doEncodeExperienced( CParamPool &IOBuff ) const;
    bool doDecodeExperienced( CParamPool &IOBuff );

    bool doEncodeMagicArray( CParamPool &IOBuff ) const;
    bool doDecodeMagicArray( CParamPool &IOBuff );

    bool doEncodeAchievment( CParamPool &IOBuff ) const;
    bool doDecodeAchievment( CParamPool &IOBuff );

    bool doEncodeInfoCenter( CParamPool &IOBuff ) const;
    bool doDecodeInfoCenter( CParamPool &IOBuff );

    bool doEncodeActiveness( CParamPool &IOBuff ) const;
    bool doDecodeActiveness( CParamPool &IOBuff );

    bool doEncodeDesiredItem( CParamPool &IOBuff ) const;
    bool doDecodeDesiredItem( CParamPool &IOBuff );

    bool doEncodePet( CParamPool &IOBuff ) const;
    bool doDecodePet( CParamPool &IOBuff );

    bool doEncodeHandbook( CParamPool &IOBuff ) const;
    bool doDecodeHandbook( CParamPool &IOBuff );

    bool doEncodeCompensation( CParamPool &IOBuff ) const;
    bool doDecodeCompensation( CParamPool &IOBuff );

    bool doEncodeMedal( CParamPool &IOBuff ) const;
    bool doDecodeMedal( CParamPool &IOBuff );

    bool doEncodeMagicLamp( CParamPool &IOBuff ) const;
    bool doDecodeMagicLamp( CParamPool &IOBuff );

    bool doEncodeBigMamaRoleInfo(CParamPool &IOBuff) const;
    bool doDecodeBigMamaRoleInfo(CParamPool &IOBuff);

private:
    std::bitset<ERoleCacheInfoEnumMax>  m_cachedFlags;  // Cache加载过的flags

    struct CacheStruct
    {
        CacheStruct( unsigned short _key, CacheEncodeFunctor _efun, CacheDecodeFunctor _dfun ) : key( _key ), efun( _efun ), dfun( _dfun ){}
        unsigned short key;
        CacheEncodeFunctor efun;
        CacheDecodeFunctor dfun;
    };
    static const CacheStruct            m_cacheFuncMap[ERoleCacheInfoEnumMax];

}*LPROLEINFO_DB;



class MailInfoDB
{
public:
    MailInfoDB();
    MailInfoDB(unsigned int nReceiverID, std::string szReceiverName, std::string strTitle, std::string strContext,
        int nMoney = 0, itemtype_t nItemType = 0, itemcount_t nCount = 0, int nValidTime = 0);
    virtual ~MailInfoDB() {}

    void SetTitle(const std::string &rTitle);
    const std::string& GetTitle() const { return m_strTitle; }

    void SetContent(const std::string &rContent);
    const std::string& GetContent() const { return m_strContent; }

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

    void Set(unsigned int nReceiverID, std::string szReceiverName, std::string strTitle, std::string strContext,
        int nMoney = 0, itemtype_t nItemType = 0, itemcount_t nCount = 0, int nValidTime = 0);

public:
    unsigned int    m_nReceiverID;
    char            m_szReceiverName[MAX_NAME_LEN];

    int             m_nMoney;
    itemtype_t      m_nItemType;
    itemcount_t     m_nCount;
    int             m_nValidTime;

protected:
    std::string     m_strTitle;
    std::string     m_strContent;
};

typedef XQMap<__int64, MailInfo *>     mapXQMail;
typedef std::map<__int64, MailInfo *>  mapMail;

class Comp_ItemWithPos
{
public:
	unsigned short m_nColumn;
    unsigned short m_nIndex;

public :
	Comp_ItemWithPos(unsigned short nColumn,unsigned short nIndex)
	{
		m_nColumn = nColumn;m_nIndex=nIndex;
	}

	bool operator()(const DBItem& ItemInfo)
	{
		return ItemInfo.m_nColumn==m_nColumn && ItemInfo.m_nIndex==m_nIndex;
	}
};

class CQueryTag
{
public:
	CQueryTag()
	{
		m_nSession = 0;
		m_nSlotPeer = 0;
		m_nAccount = 0;
	}
	~CQueryTag(){};
public:
	__int64				m_nSession;
	unsigned short	m_nSlotPeer;
	unsigned int		m_nAccount;
};

class CCumulativeRechargeDB
{
public:
	CCumulativeRechargeDB();
	~CCumulativeRechargeDB();
public:
	int m_nRechargeNum;		
	unsigned int m_nActivityBeginTime;
	unsigned int m_nActivityEndTime;
	unsigned int m_nRechargeStartTime;
	unsigned int m_nRechargeStopTime;
};

class CCumulativeSpendDB
{
public:
	CCumulativeSpendDB();
	virtual ~CCumulativeSpendDB();

public:
	unsigned int m_nSpendStartTime;
	unsigned int m_nSpendStopTime;
	int	m_nBillNum;
	int m_nBindBillNum;
};

class CumulativeSpendGotMedalDB
{
public:
    CumulativeSpendGotMedalDB();
    ~CumulativeSpendGotMedalDB() {}

public:
    unsigned int m_nRoleID;
    unsigned int m_nOpenTime;
    unsigned int m_nCloseTime;
};

//////////////////////////////////////////////////////////////////////////

class BuyItemGotMedalDB
{
public:
    BuyItemGotMedalDB();
    ~BuyItemGotMedalDB() {}

public:
    unsigned int m_nRoleID;
    unsigned int m_nOpenTime;
    unsigned int m_nCloseTime;
    unsigned char m_nIndex;
    unsigned short m_nBuyNum;
};

class BuyItemGotMedalRewardedInfo
{
public:
    BuyItemGotMedalRewardedInfo();
    ~BuyItemGotMedalRewardedInfo() {}

public:
    unsigned short m_nBuyNum;
    bool m_bRewarded;
};

typedef std::map<unsigned char, BuyItemGotMedalRewardedInfo>    BuyItemGotMedalRewardedInfoMap;

///////////////////////////////////////////
class CXmasFreeRewardDB
{
public:
	CXmasFreeRewardDB();
	~CXmasFreeRewardDB();
public:
	unsigned int m_nRoleID;
	unsigned int m_nLastGetRewardTime;
	unsigned int m_nActivityStartTime;
	unsigned int m_nActivityEndTime;
	std::vector<int> m_vecReward;
};
///////////////////////////////////////////
class CFestivalWishInfoDB
{
public:
	CFestivalWishInfoDB();
	~CFestivalWishInfoDB();
public:
	unsigned int m_nRoleID;
	unsigned int m_nWishCount;
	unsigned int m_nLastGetRewardTime;
	unsigned int m_nActivityStartTime;
	unsigned int m_nActivityEndTime;
};
///////////////////////////////////////////
class CRoleBlessActivityDB
{
public:
	CRoleBlessActivityDB();
	~CRoleBlessActivityDB();
public:
	unsigned int						m_nRoleID;
	unsigned int						m_nBlessStartTime;
	unsigned int						m_nBlessEndTime;
	unsigned int						m_nBeBlessedCount;
	std::list<CFestivalBlessEntry>		m_listBlesses;
	std::vector<int>					m_vecRewards;
};

class CFestivalBlessInfoDB 
{
public:
	CFestivalBlessInfoDB();
	~CFestivalBlessInfoDB();
public:
	unsigned int	m_nSenderID;
	unsigned int	m_nRcverID;
	unsigned short	m_nWordsIndex;
	unsigned int	m_nBlessTime;
};

////////////////////////////////////////////
struct ExternalSendMailInfo
{
public:
	ExternalSendMailInfo()
		: m_nPID(0)
		, m_strPUID("")
		, m_nReceiveID(0)
		, m_strTitle("")
		, m_strContent("")
		, m_nMoney(0)
		, m_nItemType(0)
		, m_nCount(0)
		, m_nValidTime(0)
		, m_strOrderID("")
	{

	}
public:
	unsigned short      m_nPID;                     //平台id
	std::string         m_strPUID;                  //平台用户id
	unsigned int		m_nReceiveID;				//接收者ID
	std::string			m_strTitle;					//邮件标题
	std::string			m_strContent;				//邮件内容
	unsigned int		m_nMoney;					//金钱
	unsigned int		m_nItemType;				//类型
	unsigned short		m_nCount;					//数量
	int					m_nValidTime;				//时效
	std::string			m_strOrderID;				//序列号
};

struct ExternalSendMailGiftInfo
{
public:
	ExternalSendMailGiftInfo()
		: m_nPID(0)
		, m_strPUID("")
		, m_nReceiveID(0)
		, m_nbMale(true)
		, m_nMailGiftID(0)
		, m_strOrderID("")
	{

	}
public:
	unsigned short      m_nPID;                     //平台id
	std::string         m_strPUID;                  //平台用户id
	unsigned int		m_nReceiveID;				//接收者ID
	bool				m_nbMale;					//是否男性
	unsigned int	    m_nMailGiftID;				//郵件禮品ＩＤ
	std::string			m_strOrderID;				//序列号
};

////////////////////////////////////////////

class CRoleAllPuzzleDB
{
public:
	CRoleAllPuzzleDB();
	~CRoleAllPuzzleDB();
public:
	unsigned int m_nRoleID;

	int m_nPuzzleBeginTime;
	int m_nPuzzleEndTime;

	std::map<int, CRolePuzzleActivityDB> m_AllPuzzle;
};

//////////////////////////////////////////////////////////////////////////
// 新玩家招募活动
class CRoleInvitationRewardDB
{
public:
	CRoleInvitationRewardDB();
	~CRoleInvitationRewardDB();

public:
	unsigned short m_nLevel;
	bool m_bReceived;
	unsigned int m_nReceivedTime;
};


typedef std::map<unsigned short, CRoleInvitationRewardDB>  CRoleInvitationRewardDBMap;

class CRoleAllInvitationInfoDB
{
public:
	CRoleAllInvitationInfoDB();
	~CRoleAllInvitationInfoDB();

public:
	unsigned int m_nRoleID;
	std::string m_strInvitationCode;
	unsigned int m_nInvitationBindCount;
	CRoleInvitationRewardDBMap m_InvitationRewards;
};

class CLiveRoomDancerDB
{
public:
	CLiveRoomDancerDB();
	~CLiveRoomDancerDB();
	void Clear();
public:
	bool m_bIsDanceGroup;
	unsigned int m_nDancerID[MAX_ROOM_PLAYER_COUNT];
	std::string m_strDancerName[MAX_ROOM_PLAYER_COUNT];
};

class CLiveRoomScoreDB
{
public:
	CLiveRoomScoreDB();
	~CLiveRoomScoreDB();
public:
	unsigned int m_nRoleID;
	std::string m_strRoleName;

	int m_nPoint;
	int m_nPointChanged;
	unsigned int m_nPointUpdateTime;

	int m_nHot;
	int m_nHotChanged;
	unsigned int m_nHotUpdateTime;
};

class CAllLiveRoomScoreDB
{
public:
	CAllLiveRoomScoreDB();
	~CAllLiveRoomScoreDB();

public:
	std::list<CLiveRoomScoreDB> m_listScore;
};

class CompensationInfo
{
public:
	CompensationInfo();
	~CompensationInfo();

	void EncodeInfo(CParamPool &IOBuff);
	void DecodeInfo(CParamPool &IOBuff);

public:
	int			m_nID;
	int			m_nEndTime;
	int			m_nMoney;
	std::string	m_strTitle;
	std::string m_strContent;
	std::string m_strMaleItem;
	std::string m_strFemaleItem;
	std::list<CItem> m_MaleItems;
	std::list<CItem> m_FemaleItems;
};

class CompensationInfoDB
{
public:
	CompensationInfoDB() {}
	~CompensationInfoDB() {}

public:
	std::map<int,CompensationInfo> m_Compensations;
};

//////////////////////////////////////////////////////////////////////////

class CSpecialMallInfoDB
{
public:
    CSpecialMallInfoDB()
    {
        m_uRoleID = 0;
        m_uIdx = 0;
        m_uItemType = 0;
        m_uItemCount = 0;
        m_nValidTime = 0;
        m_nBuyType = 1;
        m_uBuyPrice = 0;
        m_uUsedMoney = 0;
        m_uUsedBindPt = 0;
        m_uUsedPt = 0;
        m_tEndTime = 0;
        m_eType = ESpecialMallType_None;
    }
    ~CSpecialMallInfoDB() {}

public:
    unsigned int        m_uRoleID;
    unsigned short      m_uIdx;
    itemtype_t          m_uItemType;
    unsigned short      m_uItemCount;
    int                 m_nValidTime;
    unsigned char       m_nBuyType;
    unsigned int        m_uBuyPrice;
    unsigned int        m_uUsedMoney;
    unsigned int        m_uUsedBindPt;
    unsigned int        m_uUsedPt;
    time_t              m_tEndTime;
    ESpecialMallType    m_eType;
};

//////////////////////////////////////////////////////////////////////////

enum EAuctionBidState
{
    EAuctionBidState_ReturnedMoney = 0,
    EAuctionBidState_NotReturnMoney,
    EAuctionBidState_BidSuccess,
};

class CAuctionBidInfoDB
{
public:
    CAuctionBidInfoDB()
    {
        m_uRoleID = 0;
        m_uIdx = 0;
        m_uItemType = 0;
        m_uItemCount = 0;
        m_nValidTime = 0;
        m_uBidPrice = 0;
        m_tEndTime = 0;
    }
    ~CAuctionBidInfoDB() {}

public:
    unsigned int        m_uRoleID;
    std::string         m_strRoleName;
    unsigned short      m_uIdx;
    itemtype_t          m_uItemType;
    unsigned short      m_uItemCount;
    int                 m_nValidTime;
    unsigned int        m_uBidPrice;
    time_t              m_tEndTime;
};

//////////////////////////////////////////////////////////////////////////

class CSaveGroupBuyingCount
{
public:
    CSaveGroupBuyingCount()
    {
        m_uIdx = 0;
        m_uCount = 0;
        m_tEndTime = 0;
    }
    ~CSaveGroupBuyingCount() {}

public:
    unsigned short      m_uIdx;
    unsigned int        m_uCount;
    time_t              m_tEndTime;
};

//////////////////////////////////////////////////////////////////////////

class CSetAuctionStateSuccessInfo
{
public:
    CSetAuctionStateSuccessInfo()
    {
        m_uRoleID = 0;
        m_uIdx = 0;
        m_uItemType = 0;
        m_uItemCount = 0;
        m_nValidTime = 0;
        m_uBidPrice = 0;
        m_tEndTime = 0;
        m_bSuccess = false;
    }
    ~CSetAuctionStateSuccessInfo() {}

public:
    unsigned int        m_uRoleID;
    unsigned short      m_uIdx;
    itemtype_t          m_uItemType;
    unsigned short      m_uItemCount;
    int                 m_nValidTime;
    unsigned int        m_uBidPrice;
    time_t              m_tEndTime;
    bool                m_bSuccess;
};

//////////////////////////////////////////////////////////////////////////

class CAuctionNotGivePointInfo
{
public:
    CAuctionNotGivePointInfo()
    {
        m_lLogID = 0;
        m_uIdx = 0;
        m_uBidPrice = 0;
        m_tEndTime = 0;
        m_bTopPrice = false;
    }
    ~CAuctionNotGivePointInfo() {}

public:
    long long           m_lLogID;
    unsigned short      m_uIdx;
    unsigned int        m_uBidPrice;
    time_t              m_tEndTime;
    bool                m_bTopPrice;
};

//////////////////////////////////////////////////////////////////////////

class CAuctionReturnPointDB
{
public:
    CAuctionReturnPointDB()
    {
        m_lLogID = 0;
        m_uRoleID = 0;
        m_uAccountID = 0;
        m_uIdx = 0;
        m_nEndTime = 0;
        m_nPoint = 0;
        m_nNewPoint = 0;
        m_eChangeBillCause = EChangeBillCause_Add_Auction;
        m_bSuccess = false;
    }
    ~CAuctionReturnPointDB() {}

public:
    long long           m_lLogID;
    unsigned int        m_uRoleID;
    unsigned int        m_uAccountID;
    unsigned short      m_uIdx;
    unsigned int        m_nEndTime;
    int                 m_nPoint;
    int                 m_nNewPoint;
    EChangeBillCause    m_eChangeBillCause;
    bool                m_bSuccess;
};

//////////////////////////////////////////////////////////////////////////

class CGetLimitedSalesCount
{
public:
    CGetLimitedSalesCount()
    {
        m_uIdx = 0;
        m_uCount = 0;
    }
    ~CGetLimitedSalesCount() {}

public:
    unsigned short      m_uIdx;
    unsigned int        m_uCount;
};

//////////////////////////////////////////////////////////////////////////

class CGetAuctionBidInfo
{
public:
    CGetAuctionBidInfo()
    {
        m_uIdx = 0;
        m_uCurPrice = 0;
        m_uRoleID = 0;
        m_nTime = 0;
    }
    ~CGetAuctionBidInfo() {}

public:
    unsigned short      m_uIdx;
    unsigned int        m_uCurPrice;
    unsigned int        m_uRoleID;
    std::string         m_strRoleName;
    unsigned int        m_nTime;
};


class LongactingCumulativeRechargeBoardCast
{
public:
    LongactingCumulativeRechargeBoardCast();
    ~LongactingCumulativeRechargeBoardCast() {}

public:
    void EncodeInfo(CParamPool &IOBuff);
    void DecodeInfo(CParamPool &IOBuff);

public:
    unsigned int      m_nRoleID;
    unsigned int      m_nRewardIndex;
    std::string       m_strRoleName;
    unsigned int      m_nTime;
};
//////////////////////////////////////////////////////////////////////////
class CExperiencedCardProduct
{
public:
 	CExperiencedCardProduct() : m_uRoleID(0), m_uProductID(0) {}
 	CExperiencedCardProduct(unsigned int roleID, itemtype_t productID) : m_uRoleID(roleID), m_uProductID(productID) {}
	~CExperiencedCardProduct() {}

public:
	unsigned int		m_uRoleID;				// roleID
	itemtype_t			m_uProductID;			// 商品ID
};


class CExperiencedCardUseNum
{
public:
  	CExperiencedCardUseNum() : m_uRoleID(0), m_uUseNum(0), m_timeUpdate(0) {}
  	CExperiencedCardUseNum(unsigned int roleID, unsigned short useNum, unsigned long timeUpdate) 
		: m_uRoleID(roleID), m_uUseNum(useNum), m_timeUpdate(timeUpdate) {}
	~CExperiencedCardUseNum() {};

public:
	unsigned int		m_uRoleID;				// role ID
	unsigned short		m_uUseNum;				// used num/
	unsigned int		m_timeUpdate;			// updated time.
};

class ClothEffectDB
{
public:
    ClothEffectDB();
    virtual ~ClothEffectDB();

public:
    unsigned int    m_nRoleID;
    itemid_t        m_nItemID;
    unsigned int    m_nEffectID;
};

class ClothEffectHandbookDB
{
public:
    ClothEffectHandbookDB();
    virtual ~ClothEffectHandbookDB();

public:
    unsigned short        m_nSuitID;
    unsigned short        m_nParam1;
    std::string           m_strParam2;
};

#endif//__DATASTRUCT_DB_H__
//file end

