#ifndef		__ACTIVITYDATASTRUCT_H__
#define		__ACTIVITYDATASTRUCT_H__

#include <vector>
#include <list>
#include "../../datastructure/DataStruct_Base.h"


class CFestivalBlessEntry;
class CParamPool;

//�̳Ƿ�������
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
	bool m_bExhibit;					//�Ƿ�չʾ�ڴ���
	int m_nWeight;						//��ʾ˳��
	std::string m_strTitle;				//����
	std::string m_strContent;			//����
	std::string m_strBigImageName;		//��ͼƬ
	std::string m_strSmallImageName;	//СͼƬ
	std::string m_strActionName;		//����
	std::string m_strShareTextMale;		//�����ַ������У�
	std::string m_strShareTextFemale;	//�����ַ�����Ů��

	//������doDecode��doEncode
	std::string m_strBeginTimeShow;
	std::string m_strEndTimeShow;
	std::string m_strBeginTime;
	std::string m_strEndTime;
	std::string m_strRewardDesc;
	std::string m_strMailTitle;
	std::string m_strMailContent;
	int	m_nRewardItemTime;				//�����Դ��������ޣ���λ��
	std::set<int> m_setClothPos;		//�����Դ����εĲ�λ
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
* �����ļ��Ӧ�Ľ���
*/

// �������ߵĽ���
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

//�����ߵĽ���
class CInvitationInviterReward
{
public:
	CInvitationInviterReward();
	~CInvitationInviterReward();

public:
	bool doEncode(CParamPool & IOBuff) const;

public:
	unsigned int m_nLevel;			//�ڼ����εĽ���,��Ӧindex
	unsigned int m_nCumulation;		//�ۼƴ���
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
	RewardStatus_Invalid,			//�Ƿ�״̬
	RewardStatus_Received,			//�Ѿ���ȡ
	RewardStatus_UnReceived,		//δ��ȡ
	RewardStatus_CannotReceive,		//������ȡ
	RewardStatus_ReceiveAgain,		//�ɲ���
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
	bool m_bIsOpen;	// ��Ƿ񿪷�

	unsigned char m_nDay; //��ڼ���

	std::string m_strActivityTitle;
	std::string m_strActivityContent;

	int m_nRecvAgainMoney;	// ���ս�����Ҫ�Ľ�ȯ

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
	int							nVipIndex; //��Ȩ�ȼ�
	int							nLowLevel;
	int							nHighLevel;
	std::list<CItem>		listItem;
};



//////////////////////////////////////////////////////////////

enum MeetPlayerStatus
{
	MPS_MATCHING = 0,   // �ȴ��� 
	MPS_RUNNING  = 1,   // ���
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
    std::string         m_strTitle;             // ��������
};

class CStringRewardInfo : public CRewardTitleInfo
{
public:
    CStringRewardInfo();
    virtual ~CStringRewardInfo() {}

public:
    virtual bool doEncode(CParamPool &IOBuff, bool bFemale = false);

public:
    std::string         m_strMaleRewardText;    // ��������(��)
    std::string         m_strFemaleRewardText;  // ��������(Ů)
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
    unsigned char   m_nActivityType;            // �ID
    bool            m_bExhibit;                 // �Ƿ�չʾ��ͼƬ
    int             m_nWeight;                  // Ȩ��
    unsigned int    m_nBeginTime;               // ��ʼʱ��
    unsigned int    m_nEndTime;                 // ����ʱ��
    std::string     m_strTitle;                 // ����
    std::string     m_strContent;               // ����
    std::string     m_strBigImageName;          // ��ͼƬ
    std::string     m_strSmallImageName;        // СͼƬ
    std::list<CRewardTitleInfo *> m_listReward; // ������Ʒ��Ϣ
    bool            m_bIsReward;                // �Ƿ��ý���
    bool            m_bIsOpen;                  // �Ƿ���
    bool            m_bIsShow;                  // �Ƿ���ʾ

    // ��������һ��
    std::string     m_strMaleShareText;         // ����������Ϣ(��)
    std::string     m_strFemaleShareText;       // ����������Ϣ(Ů)

    // ����Ҫ���͸��ͻ���
    std::string     m_strMailTitle;             // �ʼ�����
    std::string     m_strMailContent;           // �ʼ�����
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
    // ��������һ��
    std::string     m_strMaleActionName;        // ���Զ�����
    std::string     m_strFemaleActionName;      // Ů�Զ�����

    // ����Ҫ���͸��ͻ���
    int             m_nRewardItemTime;          // �����Դ��������ޣ���λ��
    std::set<int>   m_setClothPos;              // �����Դ����εĲ�λ
};

class CPhotoSocialShareActivityData: public CSocialShareActivityData
{
public:
    CPhotoSocialShareActivityData() {}
    ~CPhotoSocialShareActivityData() {}

public:
    void CopyForEncode(const CPhotoSocialShareActivityData *pData);

public:
    // ����Ҫ���͸��ͻ���
    int             m_nPeopleNumber;            // ������������
};

enum ELongactingCumulativeRechagreError
{
    ELCRE_Scuess,
    ELCRE_HaveGetReward,           //�Ѿ���ȡ
    ELCRE_RewardIndexError,        //�콱���д���
    ELCRE_ConfigError,             //���ô���
    ELCRE_TotalPointNotEnough,     //�ܳ�ֵ����
};

class LongactingCumulativeRechargeConfig
{
public:
    LongactingCumulativeRechargeConfig();
    virtual ~LongactingCumulativeRechargeConfig();

public:
    void doEncode(CParamPool &IOBuff, char cSex) const;

public:
    unsigned int        m_nRewardIndex;         // ��������
    unsigned int        m_nNeedMCoin;           // ��Ҫ��M��
    std::list<CItem>    m_listMaleItemReward;   // ������Ʒ����
    std::list<CItem>    m_listFemaleItemReward; // Ů����Ʒ����
    unsigned int        m_nMoney;               // ��ȯ����
    unsigned int        m_nBindMCoin;           // ��M�ҽ���

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



