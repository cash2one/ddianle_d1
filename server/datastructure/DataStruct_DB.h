/********************************************************************
	created:	20010-11-04
	author:		mei jiuhua
	filename: 	dataStructure\DataStruct_DB.h	
	purpose:	���ﶨ��������ݿ���Ҫ��һЩ�ṹ���㷨������
				����ļ��Ͷ�Ӧ��cpp�ļ�û��ҪҲ���ܱ��ͻ��˳���������
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


//������Ʒ������
typedef struct ITEMINFO_Count
{
	__int64			nItemID		;//��ƷID
	unsigned short  nCount		;//����
}*LPITEMINFO_Count;

//����һ����Ʒ��λ��
typedef struct ITEMINFO_Position
{
	__int64			nItemID		;//��ƷID
	unsigned short  nIndex		;
	unsigned short  nColumn		;//���Ǹ��ط����������ϣ�����װ����
}*LPITEMINFO_Position;
//����2����Ʒ��λ��
typedef struct ITEMINFO_Position2
{
	ITEMINFO_Position aItemData[2];

}*LPITEMINFO_Position2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#pragma pack(1)
//�Ʒ���Ϣ
typedef struct BILLINFO
{
	unsigned int nAccount;  //�˺�id
	unsigned int nPoint;	//ʣ�����
	unsigned int nBindPoint;//�󶨱�
	unsigned int nChange;   //���þ�
	unsigned int nQueryIdx;
	unsigned int nTotalPoint;  //��ֵ�ܵ���
	unsigned int nTotalBindPoint; //�ۼư󶨵���
	unsigned int nTotalGift;//����ȡ�ۼ���Ʒ�ĵ���

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
	unsigned short m_nType		;	//type��־
	int            m_nDuration	;	//����ʱ��
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
	GeneType_Duration,		//ʱ������
	GeneType_Matchs,		//��������
	GeneType_InRole,		//��Ʒ����������
	GeneType_Equip,			//װ������
	GeneType_OneTime,		//һ��ʹ��
	GeneType_DurationByParam,//ʱ�����������nParam1��ͬ���ۼ�ʱ�䣬��������ʱ��
	GeneType_Couple,		// ����˫�����ߴ���
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
	unsigned short   nGeneId;	                        //����id
	unsigned char    nType;		                        //ִ������
	int              nParam1;			                //����  ���ò���
	int              nParam2;			                //����  ����ʱ��������ô���
	char             szCmdParam[MAX_GENE_PARAM_LEN];    //����  ����
	
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
    itemtype_t     m_nType;             // ��Ʒ����
    unsigned char  m_nState;            // �Ƿ���
    unsigned char  m_nDiscount;         // �ۿ۰ٷֱȣ�10��ʾ10%

    unsigned char  m_nIsVip;            // �Ƿ�vip��Ʒ
    unsigned char  m_nIsPrecious;       // �Ƿ���ذ�
    unsigned char  m_nIsNew;            // �Ƿ���Ʒ
    unsigned char  m_nHot;              // ����
    unsigned char  m_nIsCoupon;         // ��ȯ
    unsigned char  m_nUnique;           // ����

    unsigned char  m_nPayType;          // ֧������
    unsigned char  m_nUseChangePercent; // ��ȯ���ʹ�ðٷֱȣ�10��ʾ10%

    unsigned int   m_nPrice;            // �۸�[1��ʱЧ��1������]
    unsigned int   m_nPrice1;           // �۸�[7��ʱЧ��5������]
    unsigned int   m_nPrice2;           // �۸�[30��ʱЧ��10������]
    unsigned int   m_nPrice3;           // �۸�[����ʱЧ��20������]

    unsigned int   m_nSell_time0;       // ���ۿ�ʼʱ��
    unsigned int   m_nSell_time1;       // ���۽���ʱ��
    unsigned int   m_nDiscount_time0;   // ���ۿ�ʼʱ��
    unsigned int   m_nDiscount_time1;   // ���۽���ʱ��

    unsigned int   m_nDesireCount;      // ��Ը����

    unsigned short m_nHotIdx;           // �����������ȼ�
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
    unsigned int    m_nRoleID;              // ��ɫ���к�
    unsigned char   m_nSex;                 // �Ա�
    string          m_strName;              // ��ɫ����
    string          m_strNotes;             // ��ע
    unsigned short  m_nLevel;               // �ȼ�	
    unsigned char   m_nRelationType;        // �ƺ�����
    unsigned char   m_nCallBy;              // ���Ѷ��ҵĳƺ�
    unsigned char   m_nCallTo;	            // �����ѵĳƺ�	
    bool            m_bIsVIP;				// �����Ƿ���VIP
    int             m_nVIPLevel;			// ����VIP�ȼ�
    bool            m_bIsOnline;            // �Ƿ�����
    unsigned int    m_nOfflineTime;         // ����ʱ��(�����ҵ�½���Ѳ����ߣ��޷���ȡ����ʱ��)
};

//////////////////////////////////////////////////////////////////////////

//����
struct MailItem
{
	itemtype_t      m_nType;         //��Ʒ����
	itemcount_t     m_nCount;	     //����
	int             m_nValidTime;    //��Ч��
	unsigned char   m_nState;        //�Ƿ���ȡ
	int				m_nDynamicBoxRelatedValue; // ��̬��й���ֵ

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

struct BILLGIFT //��ֵ����
{
	int nGiftType;//��Ʒ����
	int nGiftCount;//��Ʒ����
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

    // �Ӷ�̬��й���ֵ
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
    unsigned short  nPID;                               // ƽ̨ID
    std::string strOrderID;                             // ����ID
    unsigned int nAccount;                              // ��ɫ�˻�ID
    unsigned int nRoleID;                               // ��ɫID
    int nVersionType;                                   // �汾����(��ֵ|���)
    unsigned int nAmount;                               // ��ֵM��
    unsigned int nAllAmount;                            // ��ֵ�õ�������M��
    unsigned int nCurrency;                             // ʵ�ʻ���
    int nFeedVersionMoneyRewardRatio;                   // �շѰ��ֵ ��ȯ��������( ������ )
    int nFeedVersionMoneyReward;                        // �շѰ��ֵ ��ȯ���������ڵ�����¼log
    int nRechargeBindMCoinReward;                       // ��ֵ ��Mcoin ����
    int nRechargeMoneyReward;                           // ��ֵ ��ȯ ����
    EChargeRewardType eChargeRewardType;                // ��ֵ�������ͣ������׳�
    EChangeBillCause eChangeBillCause;                  // mcoin����仯ԭ��
    EChangeBindBillCause eChangeBindBillRewardCause;    // ��mcoin����ԭ��
    EChangeMoneyCause eChangeMoneyRewardCause;          // ��ȯ����ԭ��
    bool bOnline;                                       // ����Ƿ�����
    std::string strDeviceID;                            // ��ֵ�豸��
    unsigned int nChargeTime;                           // ��ֵʱ��
    bool bIsRetryOrder;                                 // �Ƿ��ǵ��������Ķ�����������GameServer�ϸ���ҷ��ʼ����ѣ�
    std::string strPartnerOrderID;                      // ƽ̨������

	unsigned int m_nFixedChargeBindPoint;				// �̶��׳����͵İ�M��

    ChargeRewardInfo* pChargeReward;                    // ��ֵ�����ṹ�����ڷ�����֮�佻��

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
	unsigned int m_nTime;//����ֵ��0ʱ���ײ��ȡ��ǰʱ��

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
	EventInfoDB m_firstChargeEvent;//�׳��¼�
	EventInfoDB m_firstChargeRewardEvent;//�׳佱���¼�
	FixedChargeRecordItem m_fixedChargeRecord; // �̶��׳��¼

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
	// ��Ʒ֧����Ϣ
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


	int nPushRatedLevel;				// �����������
	unsigned int nNoPushRatingTime;		// "����"��Ҫ����, "����"��ʱ��
	unsigned int nPushRatedOnlineTime;	// ����ʱ������
	unsigned int nPushRatedDailyQuest;	// �����������
	unsigned int nPushRatedDailyShoping;// �̳ǹ�������
	int nPushRatedCount;				// �������͵Ĵ���
	unsigned int nPushRatedCountTime;	// ���ܵ�һ�����͵�ʱ��
};


// ��ȫ��������
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

    int				m_nGiftID;			//��Ʒ���
    unsigned int	m_nExpiredTime;		//����ʱ��
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
    unsigned int        nID;                          // ����ID�������ݿ�����
    unsigned char       nState;                       // ״̬
    unsigned short      nLevel;                       // �ȼ�
    unsigned int        nExp;                         // ��ǰ�ȼ�����
    unsigned char       nQualityType;                 // Ʒ������
    unsigned char       nEvolutionTimes;              // ��������
    unsigned char       nAttributeType;               // ��������
    unsigned short      nTypeID;                      // ��������ID, Set������д
    unsigned char       nPotentialID;                 // ����Ǳ��, Set������д
    std::string         strName;                      // ������
    unsigned short      nEndurance;                   // ��ǰ����
    unsigned int        nEnduranceRecoveryTime;       // ��ǰ�����ָ�ʱ��
    unsigned int        nResetSkillTime;              // ��������ʱ��
    int                 nResetSkillTimes;             // ���켼�����ô���
    std::string         strSkillList;                   // �����б�

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
    eMedalState_Underway = 0,       //������
    eMedalState_Activated = 1,      //����
    eMedalState_Invalid  = 2,       //ʧЧ
    eMedalState_Show     = 3,       //չʾ
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
    int m_nCurrentState;     //��ǰѫ��״̬
    int m_nGetTime;          //����ʱ��
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
	unsigned int  nAccount;							//�˺�

	std::list<DBItem>  listItem;

	// ����
	std::list<FriendInfo> listFriend;						// �����б�
	std::list<FriendInfo> listBlackList;					// ������

	//std::list< QUESTINFO >listQuest;						// ���� // �ѷ�����
	std::list< QUESTNEWINFO > listQuestNew;					// ����
	//std::map<short, __int64>mapQuestHistory;				// ��ʷ���� // �ѷ�����
	//std::list< CRepeatedHistoryQuest >listRepeatedQuest;	// ������ʷ // �ѷ�����
	int m_nFresherBestMusicGrade;							// ������óɼ�
	int m_nMallBuyCount;									// �̳ǹ������

	std::list< CoolDownData >  listCoolDown;
	std::list< GeneData >  listGene;						// �������Чʱ���ǵ��ڵ�ʱ���
	// �ƺ�
	std::list< CTitleInfo > listTitle;
	//�ɾ�
	std::list<CAchievementInfo>	listAchievement;
	// �ɾ���Ʒ���
	unsigned int	nHistoryClothNum;						// �ۼ�ӵ�е�����ʱЧ���·����� 
	unsigned int	nHistoryWishItemNum;					// �ۼƼ���Ը��������Ʒ����
	unsigned int	nGainWishItemNum;						// �ۼƻ�õ�Ը����Ʒ���� 
	unsigned int	nHistoryBadageNum;						// �ۼƻ�õĻ��µ�����
	unsigned int	nHistoryUseBouquetNum;					// �ۼ�ʹ�û���������

	CWeeklyEvent	bigSpeakersUseNumWeekly;				// һ����ʹ�ô����ȵ����� 
	CWeeklyEvent	smallSpeakersUseNumWeekly;				// һ����ʹ��С���ȵ����� 
	CWeeklyEvent	sendFlowersNumInSevenWeekly;			// һ�����ͻ�����
	// �ɾ�ս�����
	unsigned short	nHistoryTaikoComboMusicNum;				// �ۼ�ȫ���ĸ�����
	unsigned int	nAutoMatchWithOppositeSexNum;		// ͨ������ƥ��ģʽ������һ����Ϸ������
	std::map<int, map<int, CFightResult > >	mapFightResult;	// ��ģʽ�����Ѷȼ�¼	
	// ǩ��
	CCheckInInfo	   checkInInfo;
	//list<unsigned char>	listCheckInGift; // �ѷ�����
	// VIP
	CRoleVIPInfo VIPInfo;
    unsigned int nMallSendItemCostBill;                //�̳�������Ʒ����M������
    unsigned int nMallSendItemTime;                    //�̳�������Ʒʱ��
	map<unsigned char, itemtype_t> mapVIPCards;		// ���ͣ�����
	BILLINFO bi;

	//UCVIP(�¼�)
	std::set<int>  setUCVIPReward; //UCVIP��Ȩ��ȡ��־

	//�¼�
	bool			bCompleteTeachGuide;
	unsigned int	m_nGuideData;
	char			nCompleteProcGuide;
	bool			bCompleteAnimation;
	bool			bCompleteFirstPaymentTag;
	bool			bCanSendFirstPaymentReward;
	bool			bReceivedInvitationReward;
	int             m_nVersionType;			//�汾ID
	unsigned int m_nFirstPaymentMoneyCount;
	unsigned int m_nFirstPaymentTime;		//�׳�ʱ��
	unsigned int m_nGetFirstPaymentRewardTime;//��ȡ�׳佱��ʱ��
	std::set<int>   m_setFresherReward;		//���������ֻ������ȡ��־
	std::vector<int>	m_CompensationReward;  //������ȡ����
	std::set<int>   m_setFresherQuestReward;//��������Ŀ�꽱����ȡ��־
	bool			m_bHasEnterConstellationLobby; //�Ƿ���������ħ����
	std::list<SConstellationCardInfo> m_listCard;
	int             m_nConstellationMatchCount;
	unsigned int    m_nConstellationMatchTime;
	unsigned int    m_nCosntellationRoomID;
	unsigned int    m_nCosntellationRoomCreateTime;
	int             m_nCosntellationRoomLine;
	bool          m_bDanceGroupFirstAddPowerReward; //����ÿ���״ε�¼����
	unsigned int m_nDanceGroupFirstAddPowerRewardTime; //�ϴ���ȡ������ʱ��
	bool          m_bPropretaryActivityReward; //IOSר�а潱����ȡ
	bool			m_bReceiveProcGuideReward;		//��ȡ������������

    unsigned int    m_nLastChangeBirthdayTime;      // �ϴθ������յ�ʱ��

    unsigned int    m_nRecommendRefreshCount;       // �Ƽ�����ˢ�´���
    unsigned int    m_nLastRecommendResetTime;      // �Ƽ������ϴ�����ʱ��
    bool            m_bRedEnvelopeAnonymity;        // �������

    // ������������/����ʱ��
    unsigned short  m_nDanceGroupChallengeChance;
    unsigned int    m_nDanceGroupChallengeChanceResetTime;
    // ���������������/����ʱ��
    unsigned short  m_nDanceGroupChallengeBuyChance;
    unsigned int    m_nDanceGroupChallengeBuyChanceResetTime;

	//mail
	XQMap<__int64, MailInfo *>	mapMailBox;

	//�ۼ�����ʱ�佱��
	CKeepOnlineReward keepOnlineRewardInfo;
	
	//����Զ�������
	CPlayerSetting m_playerSetting;

	// ���£�����Ϣ
	CCoupleInfo m_coupleInfo;

	// ���Э����
	CDivorceAgreementInfo m_DivorceAgreementInfo;
	std::list<CDivorceAction> m_DivorceAction;

	//SecurityQuiz_DB m_securityQuiz;			// �˺Ű�ȫ�������� // �ѷ�����
	RolePushRatingDB		sPushRatingDB;	// ������Ϣ�����¼�
	
	// ħ�����
	MagicLamp	m_MagicLamp;
	std::list<MainlineParcloseRecord>  m_MainlineParcloseList; // ���߹ؿ��б�
    std::list<ThemeParcloseRecord> m_ThemeParcloseList; // ����ؿ��б�
	std::set<int> m_LuckStarReward;                  // ��Ը�ǽ���
    std::map<unsigned int,unsigned int> m_PlayerDebrisInfo; //��Ƭ��Ϣ
    std::map<unsigned int,FriendContactInfo>    m_mapFriendContactInfo; // ���ѽ�����Ϣ

	//���δ���
	std::list<RoleClothGroup> m_listClothGroup;

	//�罻����
	std::map<int, unsigned int> m_mapSocialShareFirstTime;

	// �¿�
	MonthCardDBTable m_mapMonthCard;

    // ��������
    std::map<unsigned short, unsigned int>  m_mapLimitedEditionBuy;
    // ����δ������Ʒ
    std::map<long long, CAuctionNotGivePointInfo>   m_mapAuctionNotGiveInfo;
	bool m_bKeyDataModified;	// �Ƿ��йؼ����ݸ���, ����: level, exp, clothGroup,

    // ��Ծ��ϵͳ����
    RoleActivenessDB m_activeness;

	// ���鿨���ݣ�
	std::vector<itemtype_t> m_vecExperiencedProducts;
	unsigned short m_experiencedTimes;
	unsigned int m_experiencedStamp; 

    // ������Ϣ
    std::map<unsigned int, PetDB, std::greater<unsigned int> >  m_mapPet;
    int m_nPetGuideStep;//������������

	// ��Ϣ����
	std::vector<std::pair<unsigned int, unsigned int> > m_vecInfoCenterReaded; 

    // ��Ч�۳����콱Index
    unsigned int                                        m_nLongactingCumulativeRechargeIndex; 
    FixedChargeRecord m_fixedChargeRecord;

    // ѫ��ϵͳ
    std::map<int, MedalUnitDB> m_mapMedalUnit; 
    std::map<int, MedalConditionDB> m_mapMedalCondition;
    int m_nMedalWearScore;
    int m_nMedalCumulativeScore;


    // �·���Ч
    std::map<itemid_t,unsigned int>                     mapClothEffect;         // �·���Ч
    std::set<unsigned int>                              setRoleEffect;          // ��ɫӵ����Ч
    std::map<unsigned short, std::map<unsigned char, std::string> >  mapHandbookProgress;
    std::map<unsigned short, std::map<unsigned char, std::string> >  mapColorProgress;
    
    // ͼ��ϵͳ
//     std::map<int, std::list<CHandbookPersonalInfo*> > m_MapCostumePersonalInfo;
//     std::map<int, std::list<CHandbookPersonalInfo*> > m_MapEffectPersonalInfo;
    std::map<int, std::string > m_MapCostumePersonalInfo;
    std::map<int, std::string > m_MapEffectPersonalInfo;

    // Ը��
    std::list<itemtype_t>                             m_listDesiredItem;

    // �
    std::list<CRolePuzzleActivityDB>    m_listPuzzleActivityDB; // ƴͼ
    std::map<int, GiftDB>               m_mapGift;              // ��һع�

    // ħ��������
    std::map<unsigned int, MagicArrayRecord>            m_mapMagicArrayRecord;
    MagicArrayExchangeRecord                            m_mapMagicArrayExchangeRecord;

    // �ֿ�����
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
    // encode ��ⷵ��ֵ,decode����쳣������ֵ��ͬ
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
    std::bitset<ERoleCacheInfoEnumMax>  m_cachedFlags;  // Cache���ع���flags

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
	unsigned short      m_nPID;                     //ƽ̨id
	std::string         m_strPUID;                  //ƽ̨�û�id
	unsigned int		m_nReceiveID;				//������ID
	std::string			m_strTitle;					//�ʼ�����
	std::string			m_strContent;				//�ʼ�����
	unsigned int		m_nMoney;					//��Ǯ
	unsigned int		m_nItemType;				//����
	unsigned short		m_nCount;					//����
	int					m_nValidTime;				//ʱЧ
	std::string			m_strOrderID;				//���к�
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
	unsigned short      m_nPID;                     //ƽ̨id
	std::string         m_strPUID;                  //ƽ̨�û�id
	unsigned int		m_nReceiveID;				//������ID
	bool				m_nbMale;					//�Ƿ�����
	unsigned int	    m_nMailGiftID;				//�]���YƷ�ɣ�
	std::string			m_strOrderID;				//���к�
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
// �������ļ�
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
	itemtype_t			m_uProductID;			// ��ƷID
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

