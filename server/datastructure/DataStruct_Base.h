//////////////////////////////////////////////////////////////////////////
// File		:	DataStruct_Base.h
// Desc		:	��Ҫͨ�����紫�͵�һЩ���ݵ����ݽṹ��
//				����ļ��ͻ��˺ͷ���������Ҫʹ��
// Date		:	2010-11-29
// Author	:	meijiuhua
//////////////////////////////////////////////////////////////////////////
#ifndef __DATASTRUCT_BASE_H__
#define __DATASTRUCT_BASE_H__


#include <string.h>
#include "../socket/Windefine.h"
#include "../socket/Utility.h"
#include "../socket/ParamPool.h"
#include <map>
#include <vector>

#include "Macro_Define.h"
#include "../datastructure/datainfor/ItemType.h"
#include "../datastructure/DanceGroupInfoDef.h"
//#include "../datastructure/DataStruct_Couple.h"

#ifdef _MSC_VER
#pragma warning(disable:4786)
#pragma warning(disable:4201)//warning C4201: ʹ���˷Ǳ�׼��չ : �����ƵĽṹ/����

#endif

#include <vector>
#include <list>
#include <string>
using namespace std;

//#pragma pack(1)

//��ͣ��־��ʱ��
#define STATE_BIT_LOGIN		0x00000001	//�Ƿ��Ѿ���¼�ı�־
#define STATE_BIT_BLOCK		0x00000002	//��3-5λ�Ƿ�ͣ����
#define STATE_BIT_DISABLE	0x00000100	//�˺��Ƿ���õı�־
extern unsigned int s_nBlockTime[8];

const unsigned int CMD_Announce = 0x00000001;//����
const unsigned int CMD_KickRole = 0x00000002;//�ߵ���ɫ
const unsigned int CMD_Shutdown = 0x00000003;//ͣ������
const unsigned int CMD_AdminMsg = 0x00000004;//��������Ϣ

class CQuestDoc;
class AchievementConfig;

//////////////////////////////////////////////////////////////////////////
//�˺���Ϣ������ֻ�г��ӿͻ��˴��������У������ݽṹ��ע������վ��ɣ�
//���ﲻ��Ҫ���������ݽṹ��
typedef class AccountInfo
{
public:
    unsigned int        nAccount;                 //��Ϸ�ڲ��˺� Ψһ
    int                 nVersionType;             //�汾ID,��ѻ��շ�
    unsigned short      nPID;                     //ƽ̨id
    char                szPUID[MAX_PUID_LEN+1];   //ƽ̨�û�id
    int                 nType;                    //�˺����� �Զ�ע���˺Ż��Ƿ��Զ�ע���˺�.0�ֹ�ע�ᣬ1�Զ�ע��
    __int64             SessionID;                //Gate Session ID
    char                szName[MAX_NAME_LEN];
    char                szPass[MAX_PASS_LEN];     //����  ������ʱ����������֤
    char                szIDCard[MAX_IDCARD_LEN]; //ע�����֤����
    char                szPhone[MAX_PHONE_LEN];   //�绰����
    char                szLoginIp[MAX_IP_LEN];    //��½IP
    unsigned int        nBlockTime;               //�˺ű�����ʱ��,��1970��1��1��0ʱ0��0�뵽��ʱ��������������
    unsigned int        nBlockState;              //�˺�״̬ �ɵ͵��߸�BIT���壺 0 - �Ƿ���룻 1 - �Ƿ��� �� ��2��3,4��-BLOCK����
    unsigned int        nLastLoginTime;           //�������ʱ��
    unsigned int        nLastLogoutTime;          //����ǳ�ʱ��
    unsigned int        nLoginError;              //��������¼
    unsigned int        nLoginState;              //��¼��״̬
    unsigned int        nTimeLogin;               //����ʱ��
    unsigned short      nSlot;                    //��Ҷ�Ӧ��Gate����socket����
    __int64             nAccountSessionID;        //Account Session ID
    std::string         strDeviceID;              //��������豸�ţ�һ���豸�ָ��������ú�ᷢ���仯��

public:
    AccountInfo()
    {
        nAccount = 0;
        nVersionType = 0;
        nPID = 0;
        nType = 0;
        SessionID = 0;
        nBlockTime = 0;
        nBlockState = 0;
        nLastLoginTime = 0;
        nLastLogoutTime = 0;
        nLoginError = 0;
        nLoginState = 0;
        nTimeLogin = 0;
        nSlot = 0;
        nAccountSessionID = 0;
        strDeviceID = "";
        memset(szPUID, 0, sizeof(szPUID));
        memset(szName, 0, sizeof(szName));
        memset(szPass, 0, sizeof(szPass));
        memset(szIDCard, 0, sizeof(szIDCard));
        memset(szPhone, 0, sizeof(szPhone));
        memset(szLoginIp, 0, sizeof(szLoginIp));
    }
}*LPACCOUNTINFO;

typedef struct ACCOUNTINFO_CREATE
{
    unsigned int nAccount;
    char szRoleName[MAX_NAME_LEN];
    char szPass[MAX_PASS_LEN];
    unsigned char nSex;

    ACCOUNTINFO_CREATE(){
        nAccount = 0;
        nSex = 1;
        memset(szRoleName,0,MAX_NAME_LEN);
        memset(szPass, 0, MAX_PASS_LEN);
    }
}*LPACCOUNTINFO_CREATE;

class CreateRoleInfo
{
public:
    CreateRoleInfo();
    virtual ~CreateRoleInfo() {}

    void SetBirthday(int nBirthday);
    void SetBirthday(const std::string &rBirthday);
    const std::string& GetBirthday() const { return m_strBirthday; }

public:
    unsigned int    nAccount;
    char            szRoleName[MAX_NAME_LEN];
    unsigned char   nSex;
    char            szSignature[MAX_SIGNATURE_LEN];    //ǩ��
    unsigned char   nColor;
    unsigned char   nInstallerId;
    unsigned char   m_nVersionType;

protected:
    std::string     m_strBirthday;              // ����, ��ʽΪ: "19900601"
};

typedef struct ROLEINFO_CHECKISEXIST
{
    unsigned int nRoleId;

    ROLEINFO_CHECKISEXIST(){
        nRoleId = 0;
    }
}*LPROLEINFO_CHECKISEXIST;

//////////////////////////////////////////////////////////////////////////
//��ɫ������Ϣ
class RoleBaseInfo
{
public:
    RoleBaseInfo();
    RoleBaseInfo(unsigned int nRoleID);
    RoleBaseInfo(const std::string &rName);
    RoleBaseInfo(const RoleBaseInfo &rRoleBaseInfo);
    virtual ~RoleBaseInfo() {}

    RoleBaseInfo& operator=(const RoleBaseInfo &rRoleBaseInfo);
    bool operator==(const RoleBaseInfo &rRoleBaseInfo) const;

    void SetName(const std::string &rName);
    const std::string& GetName() const { return m_strName; }

    bool FindByID(unsigned int nRoleID) const;
    bool FindByName(const std::string &rName) const;

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

    friend bool operator<(const RoleBaseInfo& lhs, const RoleBaseInfo& rhs);

public:
    unsigned int    m_nRoleID;              //��ɫ���к�

protected:
    std::string     m_strName;              // ��ɫ����
};

struct OtherRoleInfo_Attr :public RoleBaseInfo
{
    unsigned char  nSex;//��������,���Ա�,Sex_Type
    unsigned int   nExp;
    unsigned short nLevel;	
    unsigned short nPre;
    unsigned short nAPt;		//����
};

class RoleAttrInfo : public RoleBaseInfo
{
public:
    RoleAttrInfo();
    RoleAttrInfo(const RoleAttrInfo &rRoleAttrInfo);
    virtual ~RoleAttrInfo() {}

    const RoleAttrInfo& operator=(const RoleAttrInfo &rRoleAttrInfo);

    void SetBirthday(const std::string &rBirthday);
    const std::string& GetBirthday() const { return m_strBirthday; }

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    unsigned short  m_nPID;                         // ƽ̨ID
    unsigned char   m_nSex;                         // �Ա�, Sex_Type
    int             m_nMoney;                       // ��Ǯ
    unsigned int    m_nExp;                         // ����
    unsigned short  m_nLevel;                       // ����

    unsigned int    m_nHot;                         // ����
    unsigned int    m_nPre;                         // ����
    unsigned int    m_nInt;                         // ����
    unsigned int    m_nAPt;                         // ����

    unsigned char   m_nConstellation;               // ����
    unsigned char   m_nSkinColor;                   // ��ɫ

    unsigned int    m_nPriv;                        // ��ɫȨ��
    unsigned int    m_nFamilyId;                    // ����ID

    char            m_szFamilyName[MAX_FAMILY_NAME];
    char            m_szSignature[MAX_SIGNATURE_LEN];       //ǩ��

    unsigned char   m_anBeiBaoKuozhan[BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM];

    unsigned char   m_nBadgeGridNum;
    unsigned int    m_nRoleCreateTime;              // ��ɫ����ʱ��
    unsigned int    m_nLastLoginTime;               // �ϴε�¼ʱ��
    unsigned int    m_nLastLogoutTime;              // �ϴεǳ�ʱ��
    unsigned int    m_nTimeOnline;
    unsigned int    m_nAddictionTime;               // ����ʱ��
    unsigned int    m_nAddictionOfflineTime;        // ������-�ۼ�����ʱ��
    unsigned int    m_nQuestDone;                   // ���������
    unsigned int    m_nLastMoneyResetTime;
    unsigned int    m_nLastMoneyResetMailRemindTime;
    unsigned int    m_nLastMoneyResetMallRemindTime;
    unsigned int    m_nDanceGroupID;

    char            m_szInvitationCode[MAX_INVITATION_CODE_LEN];    //������
    unsigned char   m_nClothGroupId;                // ��ǰ���δ���id
    unsigned int    m_nDivorceAgreementID;          // ���Э����ID

    unsigned int    m_nAccountCreateTime;           // �˺Ŵ���ʱ��

protected:
    std::string     m_strBirthday;                  // ����, ��ʽΪ: "19900601"
};


//////////////////////////////////////////////////////////////////////////
//���������Ϣ
class CQuestDoc;
class CQuestMusic;
class CQuestItem;
class CQuestDocNew;

struct TargetInfo 
{
    unsigned char  m_nType;																// ���� ��Ʒ���� ��Ʒ�Ѽ�
    unsigned int m_nIndex;															// ����ID ��ƷID ��ƷID
    unsigned short m_nCount;															// �������͵�����CQuestDoc�޸��ֶΣ�ÿ�׸�ֻ��һ�Ρ���ɸ�������ֶ�Ϊ1��
public:
    TargetInfo(unsigned char nType = 0, unsigned int nIndex = 0,unsigned short nCount = 0);
    ~TargetInfo();
    TargetInfo(const TargetInfo &rhs);
    bool operator==(const TargetInfo &rhs)const;
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
};

typedef class QUESTNEWINFO
{
public:
    QUESTNEWINFO()
    {
        m_RoleID = 0;
        m_QuestID = 0;
        m_State = 0;
        m_Times = 0;
        m_DayTimes = 0;
        m_CreateTime = 0;
        m_UpdateTime = 0;
        m_DestroyType = 0;
        m_Musics = "";
        m_UseItems = "";
        m_Functions = "";
    }

    QUESTNEWINFO(const QUESTNEWINFO& info)
    {
        m_RoleID = info.m_RoleID;
        m_QuestID = info.m_QuestID;
        m_State = info.m_State;
        m_Times = info.m_Times;
        m_DayTimes = info.m_DayTimes;
        m_CreateTime = info.m_CreateTime;
        m_UpdateTime = info.m_UpdateTime;
        m_DestroyType = info.m_DestroyType;
        m_Musics = info.m_Musics;
        m_UseItems = info.m_UseItems;
        m_Functions = info.m_Functions;
        m_strJsonQuestParam = info.m_strJsonQuestParam;
    }

    unsigned int m_RoleID;
    unsigned short m_QuestID;
    unsigned short m_State;
    unsigned short m_Times;
    unsigned short m_DayTimes;
    unsigned int m_CreateTime;
    unsigned int m_UpdateTime;
    unsigned short m_DestroyType;
    std::string m_Musics;
    std::string m_UseItems;
    std::string m_Functions;
    std::string m_strJsonQuestParam;

    CQuestDocNew* m_pQuestDoc;

public:
    bool doEncode(CParamPool& IOBuff) const;
    bool doDecode(CParamPool& IOBuff);
}*LPQUESTNEWINFO;

typedef class QUESTINFO
{
public:
    unsigned short m_nQuestID;										// ���� ID
    unsigned int   m_nTime;											// �������ʱ��
    map< unsigned char, list<TargetInfo > >	m_mapTarget;			// < ���� ����� >
    unsigned char m_nState;											// ��ö��eQuestState����
    CQuestDoc *m_pQuest;	                                        // ��ָ��ָ��̬�����ڴ棬���������ͷţ�
    CQuestDocNew * m_pQuestNew;										// ��ָ��ָ��̬�����ڴ棬���������ͷţ���������ϵͳ��

public:
    bool IsValid(){return m_pQuest != NULL;};
    CQuestDoc* GetQuest() { return m_pQuest; };
    void SetQuestDocData(CQuestDoc* __pQuest) { m_pQuest = __pQuest; };
    list<TargetInfo>* GetTargetListBy(unsigned char nType);
    unsigned int GetAcceptTime()const { return m_nTime; }
    unsigned int GetTargetCompletedCountBy(unsigned char nType, unsigned int nIndex);
    unsigned short GetQuestID();

public:	
    unsigned char GetState()const { return m_nState;};
    void SetState(unsigned char nState) { m_nState = nState; }
    bool IsCanAccept()const { return m_nState == eQuestState_CanAccept;};
    bool IsRead()const { return m_nState == eQuestState_HaveRead;};
    bool IsAccepted()const { return m_nState >= eQuestState_Accepted;};
    bool IsCompleted()const { return m_nState == eQuestState_Completed;};

public:	
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
public:
    QUESTINFO(const QUESTINFO &rhs);
    QUESTINFO& operator=(const QUESTINFO& rhs);
    bool operator==(const QUESTINFO &rhs) const;
    bool operator==(unsigned short nQuestID)const;
public:
    QUESTINFO(CQuestDoc *pQuest = NULL ,CQuestDocNew* pQuestNew = NULL ); 
    ~QUESTINFO();

private:
    void Init(const QUESTINFO& rhs);

}*LPQUESTINFO;


//////////////////////////////////////////////////////////////////////////
//������ʷ��Ϣ
class CRepeatedHistoryQuest
{
public:
    CRepeatedHistoryQuest (unsigned short nQuestID = 0, unsigned int ttime = 0, unsigned int nCount = 0 );
    bool operator==(const CRepeatedHistoryQuest &rhs);
    bool operator==(unsigned short nQuestID);
    CRepeatedHistoryQuest& operator=(const CRepeatedHistoryQuest& rhs);
public:
    unsigned short	m_nQuestID;		// ���� ID
    unsigned int	m_nTime;		// ��������ʱ��
    unsigned int	m_nCount;		// ��ɵĴ���	
public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
};

//�����������
typedef struct QUESTUPDATEINFO
{
    int				m_nModeID;				// ģʽ
    int				m_nSceneID;				// ����
    int				m_nPlayerNum;			// ������Ҹ���
    int				m_nMusicDifficulty;		// �����Ѷ�
    int				m_nMusicID;				// ����
    int				m_nRank;				// �������
    unsigned int	m_nScore;				// �������
    int				m_nPerfectNum;			// ����p����
    int				m_nFailNum;
    int				m_nConPerfectNum;	    // ������p����
    int				m_nComboNum;		    // ��Сcombo����
    int             m_nGrade;				// ����

    int				m_FriendNum;			//�ڳ����Ѹ���
    int				m_GroupNum;				//�ڳ�ͬ���������
    int				m_LoverNum;				//�ڳ����¸���
    bool			m_bFree;				//�Ƿ��������
	bool			m_bIsMatch;				//�Ƿ���ƥ���ʱ�����
public:
    QUESTUPDATEINFO() 
    { 
        memset(this, 0, sizeof(QUESTUPDATEINFO)); 
    }
    ~QUESTUPDATEINFO()	
    {
    }
}*LPQUESTUPDATEINFO;

class	CAchievementInfo	
{
public:
    CAchievementInfo(unsigned short nAchievementID = 0, unsigned int nCompletedTime = 0, AchievementConfig *pAchievementDoc = NULL, unsigned char nState = 0);
    ~CAchievementInfo();
    void SetAchievementDoc(AchievementConfig *pAchievementDoc);
    time_t GetCompletedTime() const;
    AchievementConfig * GetAchievementDoc() const;
    bool operator==(unsigned int nAchievementID) const;
    bool operator<(CAchievementInfo &rhs) const;
public:
    //static bool GreaterTime(CAchievementInfo &first, CAchievementInfo &second);
    bool doEncode(CParamPool & IOBuff) const;
    bool doDecode(CParamPool & IOBuff);
public:
    unsigned short		m_nAchievementID;
    unsigned int		m_nCompletedTime;
    AchievementConfig *   m_pAchievementDoc;
    unsigned char		m_nState;	//	�� AchivementState: AchievementProcessing = 0 AchievementCompleted = 1,	AchievementGotReward = 2
};

class CWeeklyEvent
{
public:
    CWeeklyEvent(unsigned short nCount = 0, unsigned int nLastEventTime = 0);
    ~CWeeklyEvent();
    void Clear();
public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);
public:
    unsigned short	m_nCount;
    unsigned int	m_nLastEventTime;	
};
//class CAchievementSerialize
//{
//public:
//	unsigned short	m_nAchievementID;
//	unsigned int		m_nCompletedTime;
//	unsigned int		m_nCurrenValue;
//	unsigned char	m_nState;
//public:
//	CAchievementSerialize();
//	~CAchievementSerialize();		
//public:	
//	void doEncode(CParamPool &IOBuff);
//};


class CTitleInfo
{
public:
    CTitleInfo(unsigned int nTitleID = 0, unsigned int tGainTitleTime = 0);
    ~CTitleInfo();
public:
    bool operator==(unsigned int nTitleID);
    bool operator==(const CTitleInfo &rhs);
public:
    unsigned int	m_nTitleID;
    unsigned int 	m_nGainTitleTime;

    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);
};

class CFightResult
{
public:
    CFightResult();
    ~CFightResult();
    CFightResult & operator=(const CFightResult & rhs);
public:
    unsigned char m_nModeID;
    unsigned char m_nDifficulty;
    unsigned int m_nResult[7];	// 0- 7 perfect cool, good, bad, miss
    unsigned int m_nHistoryTotalScore;
    unsigned int m_nConPerfectNum;
public:
    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);
};

//////////////////////////////////////////////////////////////////////////
//gmϵͳ�ύ�Ŀ���ָ��
typedef struct CMDINFO
{
    unsigned int nServer;//ִ��ָ���server
    unsigned int nCmdID;//ָ������
    unsigned int timeCmd;//ִ��ָ���ʱ��
    unsigned int nCmdParam;//ָ�����
    char szCmdParam[MAX_ANNOUNCE_LEN];//ָ�����
}*LPCMDINFO;


//////////////////////////////////////////////////////////////////////////


struct PLAYER_TAG
{
    PLAYER_TAG(unsigned int _ID = 0, unsigned short _idx = -1) 
    {
        nRoleID = _ID;
        nIndex = _idx;
    };
    unsigned int   nRoleID;
    unsigned short	nIndex;//gameserver�ϣ�playermanager���index����1��ʼ
};


struct FRIENDINFO : public RoleBaseInfo
{
    friend bool operator<(const FRIENDINFO& lhs, const FRIENDINFO& rhs);
    char status;//0���ߺ��� 1���߷Ǻ��� 2 ���ߺ����� 3������
    int  nLine;
    unsigned short nLevel;

    char szFamilyName[MAX_FAMILY_NAME];	//������

    PLAYER_TAG tag;
    FRIENDINFO() {
        status = 3;
        nLine = -1;
        nLevel = 0;
        memset(szFamilyName,0,MAX_FAMILY_NAME);
        szFamilyName[0] = '\0';
    }
    FRIENDINFO(unsigned int _nRoleID):RoleBaseInfo(_nRoleID)	{
        status = 3;
        nLine = -1;
        nLevel = 0;
        memset(szFamilyName,0,MAX_FAMILY_NAME);
        szFamilyName[0] = '\0';
    }
    FRIENDINFO& operator=(const FRIENDINFO& rhs)	{
        if(this == &rhs) return *this;
        *(RoleBaseInfo*)this = (RoleBaseInfo)rhs;
        status = rhs.status;
        nLine = rhs.nLine;
        nLevel = rhs.nLevel;

        memcpy(szFamilyName, rhs.szFamilyName, MAX_FAMILY_NAME - 1);

        tag = rhs.tag;

        return *this;
    }	
};

inline bool operator<(const FRIENDINFO& lhs, const FRIENDINFO& rhs)
{
    return lhs.m_nRoleID < rhs.m_nRoleID;
}

inline bool operator<(const RoleBaseInfo& lhs, const RoleBaseInfo& rhs)
{
    return lhs.m_nRoleID < rhs.m_nRoleID;
}

typedef unsigned int itemtype_t;
typedef unsigned short itemcount_t;
typedef __int64 itemid_t;
typedef std::pair<int, int> itemgrid_t; 



//��ͨ��Ʒ,ITEMINFO
class ItemBase;
class DBItem
{
public:
    DBItem();
    virtual ~DBItem(){};

public:
    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);
	void Initialize(itemid_t nItemID, itemtype_t nItemType, int nColumn, int nSlot, int nCount, int nUseTimes,
		int nDuration, int nDynamiceBoxRelatedValue, bool bEquiped);

public:
    __int64         m_nItemID;                  // ��ƷID
    itemtype_t      m_nTypeID;                  // ��Ʒ����
    unsigned short  m_nIndex;                   // ��Ʒ����
    unsigned short  m_nColumn;                  // ���Ǹ��ط����������ϣ�����װ����
    unsigned short  m_nCount;                   // ����
    short           m_nUseTimes;                // ʣ��ʹ�ô���
    int             m_nDuration;                // ��Чʱ�䣬-1��ʾû��ʱ������
    bool            m_bEquiped;                 // �Ƿ�װ�������� 0��װ�� 1��װ��
    int             m_nDynamicBoxRelatedValue;  // ��̬��й���ֵ
};

// ��ƷID
const itemtype_t ITEMID_CHANGENAMECARD =	31003;							// ������
const itemtype_t ITEMID_WEEKLYVIPCARD	=	31005;							// VIP�ܿ�
const itemtype_t ITEMID_MONTHLYVIPCARD =	31006;							// VIP�¿�
const itemtype_t ITEMID_SEASONVIPCARD	=	31007;							// VIP����
const itemtype_t ITEMID_YEARLYVIPCARD	=	31008;							// VIP�꿨
const itemtype_t ITEMID_SMALLSPEAKER	=	32001;							// С����
const itemtype_t ITEMID_BIGSPEAKER		=	32002;							// ������
const itemtype_t ITEMID_RECHECKCARD	=	31004;							// ��ǩ��
const itemtype_t ITEMID_TULIP			=	32005;							// ������
const itemtype_t ITEMID_LILY			=	32006;							// �ٺ�
const itemtype_t ITEMID_ROSE			=	32007;							// õ��
const itemtype_t ITEMID_BOUQUET		=	12345;							// ����
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_PRIMARY = 31000;			// �������¿�
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_INTERMEDIATE = 31001;		// �м����¿�
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_LEVEL_ADVANCED = 31002;	// �߼������¿�
const itemtype_t C_COUPLE_REQUEST_ITEM = 31017;					// ��������������Ʒ
const itemtype_t C_COUPLE_CREATE_MARRIAGE_ROOM_ITEM = 31018;	// �������·���������Ʒ
const itemtype_t C_DIVORCE_ITEM = 31019;						// ���������Ʒ
const itemtype_t C_ITEM_COMMON_RING = 21000;					// ��鷢�ŵ���ͨ����ָ

struct DynamicBoxRelatedInfo
{
    __int64		nItemID;
    int			nRelatedValue;
    int			nUseTime;

    DynamicBoxRelatedInfo()
    {
        nItemID = 0;
        nRelatedValue = 0;
        nUseTime = 0;
    }
};

class CItem
{
public:
    CItem()
    {
        m_nItemType = 0; 
        m_nItemCount = 0;
        m_nValidTime = 0;	
        m_bAnnounce = false;
        m_nDynamicBoxRelatedValue = 0;
    }
    CItem(itemtype_t nItemType, unsigned short nItemCount,int nValidTime, bool bAnnounce, int nDynamicBoxRelatedValue = 0 )
    {
        Set(nItemType, nItemCount, nValidTime, bAnnounce, nDynamicBoxRelatedValue);
    }
    ~CItem()
    {
    }
public:
    bool operator <(const CItem& rItem) const
    {
        if ( m_nItemType < rItem.m_nItemType )
        {
            return true;
        }
        else if ( m_nItemType == rItem.m_nItemType )
        {
            if ( m_nItemCount < rItem.m_nItemCount )
            {
                return true;
            }
            else if ( m_nItemCount == rItem.m_nItemCount )
            {
                if ( m_nValidTime < rItem.m_nValidTime )
                {
                    return true;
                }
            }
        }

        return false;
    }

    CItem& operator =( const CItem &rItem )
    {
        if( this ==&rItem ) return *this;

        m_nItemType = rItem.m_nItemType;
        m_nItemCount = rItem.m_nItemCount;
        m_bAnnounce = rItem.m_bAnnounce;
        m_nValidTime = rItem.m_nValidTime;
        m_nDynamicBoxRelatedValue = rItem.m_nDynamicBoxRelatedValue;

        return *this;
    }

    void Set(itemtype_t nItemType, unsigned short nItemCount, int nValidTime, bool bAnnounce = false, int nDynamicBoxRelatedValue = 0)
    {
        m_nItemType = nItemType;
        m_nItemCount = nItemCount;
        m_nValidTime = nValidTime;
        m_bAnnounce = bAnnounce;
        m_nDynamicBoxRelatedValue = nDynamicBoxRelatedValue;
    }

    bool doEncode(CParamPool &IOBuff) const
    {
        IOBuff.AddUInt(m_nItemType);
        IOBuff.AddUShort(m_nItemCount);
        IOBuff.AddInt(m_nValidTime);
        return true;
    }
    bool doDecode(CParamPool &IOBuff)
    {
        m_nItemType = IOBuff.GetUInt();
        m_nItemCount = IOBuff.GetUShort();
        m_nValidTime = IOBuff.GetInt();
        return true;
    }
public:
    itemtype_t m_nItemType;
    itemcount_t m_nItemCount;
    int m_nValidTime;
    bool m_bAnnounce;
    int m_nDynamicBoxRelatedValue;
};

class CCheckInInfo
{
public:
    CCheckInInfo();
    virtual ~CCheckInInfo();

public:
    CCheckInInfo(const CCheckInInfo &rhs);	
    CCheckInInfo & operator= (const CCheckInInfo &rhs);	

public:
    bool doEncode(CParamPool & IOBuff) const;
    bool doDecode(CParamPool & IOBuff);

public:
	unsigned int	m_nDayIndex;
	unsigned int	m_nCheckInTime;
	unsigned int    m_nVipOutTimeDayIndex; //vip ʧЧʱ�� dayindex
};


class CCheckInReward
{
public:
    CCheckInReward( unsigned char nDay = 0, int nMoney = 0);
    ~CCheckInReward();
public:
    void doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nDay;
    int m_nMoney;
    list<CItem> m_listItem;
};

class CVIPCard
{
public:
    CVIPCard(unsigned char nType = 0, unsigned int nUseTime = 0)
    {
        m_nType = nType;
        m_nUseTime = nUseTime;
    }
    ~CVIPCard()
    {
    }
public:
    unsigned char	m_nType;
    unsigned int	m_nUseTime;
};

class CRoleVIPInfo
{
public:
    CRoleVIPInfo();
    ~CRoleVIPInfo();
public:
    bool IsVIP();
    bool IsExpiredToday() const;
    bool CanGetDailyReward();

    void SetConsumedPt(int nConsumedPt);
    int ConsumedPt();

    void SetExchangedPt(int nExchangedPt);
    int ExchangedPt();

    void doEncode(CParamPool &IOBuff);
    void doEncodeToGame(CParamPool & IOBuff);
    void doDecodeFromGame(CParamPool & IOBuff);
public:
    int m_nLevel;
    unsigned int m_nOpenTime;
    int m_nExp;
    long long m_nExpiredTime;
    unsigned int m_nLastGetRewardTime;
    unsigned int m_nLastOnlineAnnounceTime;
    int m_nConsumedPt;
    int m_nExchangedPt;
    bool m_bValid;
};

class CFriendOnlineInfo
{
public:
    CFriendOnlineInfo();
    ~CFriendOnlineInfo();

public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    unsigned int m_nRoleID;
    string m_strName;
    string m_strNotes;
    unsigned char m_nSex;
    unsigned short m_nLevel;
    int m_nLine;                        // -1 ��ʾ������
    string m_strLineName;               // ������
    unsigned int m_nRoomID;             // ����ID
    unsigned char m_nState;             // 0���У�1�ȴ� 2��ս 3����
    unsigned char m_nRelationType;
    unsigned char m_nCallBy;
    unsigned char m_nCallTo;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned int   m_nOfflineTime;      // ����ʱ��
};

struct CFindPlayerInfo
{
public:
    CFindPlayerInfo();
    ~CFindPlayerInfo();
    void doEncode(CParamPool &ioBuff);
    void doDecode(CParamPool &ioBuff);

public:
    unsigned int m_nRoleID;
    std::string m_strRoleName;
    unsigned short m_nLevel;
    unsigned char m_nSex;
    unsigned char m_nConstellation;
    bool m_bIsVip;
    unsigned short m_nVipLevel;
};

class CWorldRoleUpdateAttrInfo
{
public:
    CWorldRoleUpdateAttrInfo();
    ~CWorldRoleUpdateAttrInfo();
public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
public:
    unsigned int		m_nRoleID;
    char				m_szRoleName[MAX_NAME_LEN]; //��ɫ����
    unsigned short		m_nLevel;
    int		m_nMoney;
};

class CWorldRoleUpdateRoomInfo
{
public:
    CWorldRoleUpdateRoomInfo();
    ~CWorldRoleUpdateRoomInfo();

public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);

public:
    unsigned int m_nRoomID;
    int m_nRoleRoomType;
    int m_nRoleRoomState;
};

class	CVIPUpdateInfo
{
public:
    CVIPUpdateInfo()
    {
        m_bVIP = false;
        m_nLevel = 0;
        m_nExp = 0;
        m_nExpiredTime = 0;
        m_bCanGetDailyReward = false;
    }
    ~CVIPUpdateInfo()
    {
    }
public:
    void doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddBool(m_bVIP);
        IOBuff.AddUShort(m_nLevel);
        IOBuff.AddInt(m_nExp);
        IOBuff.AddInt64(m_nExpiredTime);
        IOBuff.AddBool(m_bCanGetDailyReward);
    }
public:
    bool m_bVIP;
    unsigned short m_nLevel;
    int m_nExp;
    long long m_nExpiredTime;
    bool m_bCanGetDailyReward;
};

class CAutoAccount
{
public:
    CAutoAccount();
    ~CAutoAccount();
private:
    CAutoAccount(const CAutoAccount & rhs){};
    CAutoAccount & operator=(const CAutoAccount & rhs){ return *this; };
public:
    unsigned int m_nAccountID;
    char m_szName[MAX_NAME_LEN];
    char m_szPasswd[MAX_PASS_LEN];
};

class RebindAccountInfo
{
public:
    RebindAccountInfo();
    ~RebindAccountInfo();


public:
    unsigned short m_nResult; // �������
    unsigned short m_nSlot;
    __int64 m_nGateSessionID;
    int m_nServerid;
    int m_nPidOld;
    int m_nPid;
    int m_nAccount;
    unsigned int m_nRoleid;

    std::string m_strActcode;
    std::string m_strName;
    std::string m_strPass;
    std::string m_strPuidOld;
    std::string m_strPuid;
};

class CEntityItemComponent;

//////////////////////////////////////////////////////////////////////////

// ��������
typedef unsigned char SkillType;
typedef unsigned short SkillID;

// �������ݽṹ���ڳ�����Ϣͬ��
class PlayerPet
{
public:
    PlayerPet();
    ~PlayerPet();

    void Reset();

public:
    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);

public: 
    unsigned int   m_nPetID;               // ����ID
    std::string    m_strName;              // �����ǳ�
    std::string    m_strDefaultName;       // ����Ĭ���ǳ�
    unsigned char  m_nPotentialID;         // ����Ǳ��
    unsigned char  m_nPetAttributeType;    // ������������
    unsigned short m_nPetType;             // ��������
    unsigned short m_nLevel;               // ��ǰ�ȼ�
    unsigned short m_nMaxLevel;            // ��ǰƷ�����ȼ�
    unsigned int   m_nExp;                 // ���ﾭ��
    unsigned int   m_nMaxExpOfLevel;       // ���ﵱǰ�ȼ������ֵ
    unsigned char  m_nQualityType;         // ��ǰƷ��
    unsigned char  m_nEvolutionTimes;      // ��������
    unsigned short m_nEndurance;           // ��ǰ����
    float          m_nEnduranceRecoveryTime;// ��ǰ�����ָ�ʱ�䣨����ʱ������
    unsigned short m_nMaxEndurance;        // �������
};

class PetMsgSkill
{
public:
    PetMsgSkill();
    ~PetMsgSkill();

public:
    bool doEncode(CParamPool &ioBuffer) const;
    bool doDecode(CParamPool &ioBuffer);

public:
    SkillID m_nSkillID;
    std::string m_strName;
    std::string m_strAtlasName;
    std::string m_strSpriteName;
    bool m_bActivated;
    unsigned short m_nLevel;
    unsigned short m_nMaxLevel;
    unsigned char m_nLevelUpCostType;
    unsigned int m_nLevelUpCostAmount;
    std::string m_strTip;
    std::string m_strNextTip;
    std::string m_strActivationTip;
};

class PlayerPetSkill
{
public:
    PlayerPetSkill();
    ~PlayerPetSkill();

    void Reset();

public:
    bool doEncode(CParamPool &IOBuff) const;
    bool doDecode(CParamPool &IOBuff);

public:
    unsigned int           m_nPetID;            // ����ID
    int                    m_nDanceSkillCount;  // ���輼����
    int                    m_nFightSkillCount;  // ս��������
    std::list<PetMsgSkill> m_listDanceSkill;    // ���輼���б�
    std::list<PetMsgSkill> m_listFightSkill;    // ս�������б�
};

//////////////////////////////////////////////////////////////////////////

class PlayerMedal
{
public:
    PlayerMedal();
    ~PlayerMedal() {}

public:
    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    int m_nType;
    int m_nID;
};

//////////////////////////////////////////////////////////////////////////
class PlayerInfo
{
public:
    PlayerInfo();
    virtual ~PlayerInfo() {}

    void SetRoleName(const std::string &rRoleName);
    const std::string& GetRoleName() const { return m_strRoleName; }

    void SetBirthday(const std::string &rBirthday);
    const std::string& GetBirthday() const { return m_strBirthday; }

    void SetSignature(const std::string &rSignature);
    const std::string& GetSignature() const { return m_strSignature; }

    void SetCurLineName(const std::string &rCurLineName);
    const std::string& GetCurLineName() const { return m_strCurLineName; }

    void SetDanceGroupName(const std::string &rDanceGroupName);
    const std::string& GetDanceGroupName() const { return m_strDanceGroupName; }

    void SetCoupleName(const std::string &rCoupleName);
    const std::string& GetCoupleName() const { return m_strCoupleName; }

    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    int                         m_nItemBuffLen;
    char                        m_szItemBuff[65536];

    int                         m_nGeneBuffLen;
    char                        m_szGeneBuff[10240];

    unsigned                    m_nRoleID;
    unsigned char               m_nSex;                     // �Ա�,Sex_Type
    unsigned char               m_nConstellation;           // ����
    unsigned char               m_nSkinColor;               // Ƥ����ɫ
    bool                        m_bVIP;

    unsigned short              m_nLevel;
    unsigned short              m_nTitle;                   // �ƺ�
    unsigned int                m_nHot;                     // ����
    unsigned int                m_nSocial;                  // �罻ָ��

    unsigned int                m_nExp;                     // ��ǰ��EXP
    int                         m_nCurLine;                 // ��ǰ������
    unsigned short              m_nRoomID;                  // ����ID 
    unsigned char               m_nRoomState;               // ״̬(��Ϸ��/�ȴ��� 0/1)

    std::vector<itemtype_t> m_vecEquipedItem;               // ��Ҫ��װ������Ʒ
    std::vector<itemtype_t> m_vecWishItem;                  // Ը����Ʒ
    unsigned char               m_nVIPLevel;                // ����VIP�ȼ�
    unsigned int                m_nAchScore;                // ���ѳɾͻ���
    std::vector<unsigned int>   m_vecAch;                   // ����������ɳɾ�
    unsigned int                m_nDanceGroupID;            // ���ű��
    unsigned int                m_nCoupleID;                // ����ID
    unsigned int                m_nCoupleDate;              // ����������
    unsigned int                m_nMarriageDate;            // ��������
    unsigned int                m_nSmallRoleID;             // ͬ�Խ��С��ID

    bool                        m_bHasPet;                  // �Ƿ�Я������
    PlayerPet                   m_playerPet;                // ���������Ϣ
    PlayerPetSkill              m_playerPetSkill;           // ���＼����Ϣ

    std::list<PlayerMedal>      m_listActivityMedals;       // ����ѫ��
    std::list<PlayerMedal>      m_listShowMedals;           // ��ʾѫ��
    unsigned int                m_nMedalScore;              // ѫ�»���

    unsigned int                m_nPerpetualEquipCount;     // ���÷�������
    unsigned int                m_nPerpetualEffectScore;    // ��Ч����ֵ

protected:
    std::string                 m_strRoleName;              // ��ɫ����
    std::string                 m_strBirthday;              // ����, ��ʽΪ: "19900601"
    std::string                 m_strSignature;             // ����ǩ��
    std::string                 m_strCurLineName;           // ��ǰ����������
    std::string                 m_strDanceGroupName;        // ��������
    std::string                 m_strCoupleName;            // ��������
};


enum EOfflinePlayerDataFrom
{
    EOfflinePlayerDataFrom_None,
    EOfflinePlayerDataFrom_Couple,
    EOfflinePlayerDataFrom_Divorce,
    EOfflinePlayerDataFrom_Logout,
};

// �������������Ϣ
class RoleImageOfflineData
{
public:
    RoleImageOfflineData();
    virtual ~RoleImageOfflineData();
    RoleImageOfflineData & operator=(const RoleImageOfflineData &src);

public:
    void Reset();
    void doEncode( CParamPool &IOBuff );
    void doDecode( CParamPool &IOBuff );

public:
    unsigned int            m_nRoleID;          //��ɫID
    std::string             m_strRoleName;      //��ɫ����
    char                    m_nSex;             //����Ա�
    char                    m_nSkin;            //���Ƥ��
    unsigned short          m_nLevel;           //��ҵȼ�
    bool                    m_bVIP;             //�Ƿ�VIP
    unsigned short          m_nVIPLevel;        //vip�ȼ�
    std::vector<DBItem>     m_vecRoleItem;      //�����Ʒ
    bool                    m_bHasCarriedPet;   //�Ƿ�Я������
    PlayerPet               m_playerPet;        //����
    PlayerPetSkill          m_playerPetSkill;   //���＼��
    std::map<itemid_t,unsigned int> m_mapClothEffect;   // �·���Ч
    std::map<unsigned short,std::set<ESevenColorType> >  m_mapClothEffectSevenColor; // �·���Ч��ɫ�ʺ�
};

class CActivateGiftInfo
{
public:
    CActivateGiftInfo();
    CActivateGiftInfo(const CActivateGiftInfo &rhs);
    ~CActivateGiftInfo();
    CActivateGiftInfo &operator=(const CActivateGiftInfo &rhs);
public:
    string m_strCode;
    unsigned int m_nGiftID;
    unsigned int m_nGiftType;
    itemtype_t   m_nItemID;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
    unsigned int m_nItemCount;
    unsigned int m_nOwnRoleID;
    unsigned int m_nDrawTime;
};

class CFriendRelation
{
public:
    CFriendRelation( int nRelation = 0, int nCallBy = 0, int nCallTo = 0 );
    bool operator==(const CFriendRelation &rhs) const;
    ~CFriendRelation();
public:
    int m_nRelation;
    int m_nCallBy;		
    int m_nCallTo;
};

class CCalling
{
public:
    CCalling(int nCodeNum = 0, std::string strName = "", int reqGender = -1);
    ~CCalling();
    bool operator==(const CCalling &rhs) const;
public:
    int m_nCodeNum;
    std::string m_strName;
    int m_nReqGender;
};

class CGameServerInfo
{
public:
    CGameServerInfo(int nLine = 0, string strServerName = "", int nOnlinePlayersNum = 0, int nLimitNum = 0, unsigned char nInstallerID = 0, bool bEnableAudition = false);
    CGameServerInfo(const CGameServerInfo & rhs);
    CGameServerInfo & operator=(const CGameServerInfo &rhs);
    ~CGameServerInfo();
public:
    void doEncode(CParamPool & IOBuff);
public:
    int m_nLine;				// ��������
    string m_strServerName;		// ����������
    int m_nOnlinePlayersNum;	// ���������
    int m_nLimitNum;			// �ɳ����������
    unsigned char m_nInstallerID;// ֧�ֵİ�װ���汾:���ְ�����������
    bool m_bEnableAudition;		// �Ƿ��Ǻ�ѡ��
};

class CAnnouncement
{
public:
    CAnnouncement();
    ~CAnnouncement();
public:
    void doEncode(CParamPool &IOBuff);
public:
    unsigned char m_nType;
    string m_strTitle;
    string m_strContent;
    string m_strDate;
    bool m_bHot;
    vector<string> m_vecStrIcon;
    unsigned char m_nLinkTo;
    unsigned short m_nPID;
};

class CMailInfo
{
public:
    CMailInfo();
    ~CMailInfo();
public:
    string			m_strTitle;
    string			m_strContent;
    itemtype_t  	m_nItemType;
    int				m_nItemCount;
    int				m_nItemDuration;
    int				m_nMoney;
public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);
};

class CQueryGiftInfo
{
public:
    CQueryGiftInfo();
    ~CQueryGiftInfo();
public:
    unsigned short	m_nGiftID;
    string			m_strStartTime;
    string			m_strEndTime;
    int				m_nActivatedNum;
};

class CRcvSysMailRoleInfo
{
public:
    CRcvSysMailRoleInfo();
    ~CRcvSysMailRoleInfo();
public:
    void doEncodeServ(CParamPool & IOBuff);
    void doDecodeServ(CParamPool & IOBuff);

    void doEncodeCli(CParamPool & IOBuff);
    void doDecodeCli(CParamPool & IOBuff);
public:
    unsigned short	m_nPid;
    std::string	    m_strPUID;
    CMailInfo m_MailInfo;
    bool m_bSuc;

    string m_strRoleName;	// for log
public:
    unsigned int m_nRoleID;	// for SysSendMail
};

class CQueryGiftState
{
public:
    CQueryGiftState();	
    ~CQueryGiftState();	
public:
    bool doEncode(CParamPool & IOBuff);
    bool doDecode(CParamPool & IOBuff);
public:
    unsigned int m_nGiftID;
    unsigned int m_nDrawTime;
    unsigned int m_nRoleID;
    unsigned int m_nStartTime;
    unsigned int m_nEndTime;
    bool m_bValid;
    string m_strGiftCode;
    string m_strDrawTime;
};

class CAskMallGoodInfo
{
public:
    CAskMallGoodInfo();	
    ~CAskMallGoodInfo();
public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);
public:
    itemtype_t      m_nSellGoodType;
    unsigned char   m_nSellIndex;
};

class CAnnounceBase
{
public:
    CAnnounceBase();
    ~CAnnounceBase();
public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);
public:
    bool m_bShowInChatWindow;
    bool m_bIsAdminChat;
    std::string m_strContent;
    unsigned short m_nPID;
};

class CPlayerSetting
{
public:
    CPlayerSetting();
    ~CPlayerSetting();

public:
    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    std::map<unsigned char, int> m_mapOption;
};

class CPlayerPosition
{
public:
    CPlayerPosition();
    ~CPlayerPosition();
public:
    void doEncode(CParamPool & IOBuff);
    void doDecode(CParamPool & IOBuff);
    void Reset();
public:
    ESceneMove m_ESceneID;
    unsigned char m_nMoveType;
    float m_fOrient;
    float m_fPosX;
    float m_fPosY;
    float m_fPosZ;
};


////////////////////////////////////////
class CFestivalBlessEntry
{
public:
    CFestivalBlessEntry();
    ~CFestivalBlessEntry();
public:
    void doEncode(CParamPool & IOBuff);
public:
    unsigned int m_nSenderID;
    std::string m_strSenderName;
    unsigned short m_nWordsIndex;
    unsigned int m_nBlessTime;
};
enum ECreateTimeEggError
{
    ECreateTimeEggError_OK = 0,
    ECreateTimeEggError_ExistInScene,
    ECreateTimeEggError_DunplicateEgg,
    ECreateTimeEggError_Unknown,
};
//ʱ�⵰
struct TimeEggEntry
{
    unsigned int m_nEggID;
    unsigned int m_nSceneID;
    unsigned int m_nBeginTime;
    unsigned int m_nEndTime;
    unsigned int m_nDestroyTime;
    unsigned int m_nRelation;
    unsigned int m_nRoleID;//�񵰶Է�id
    string m_strRoleName;//�񵰶Է�����
    unsigned int m_nOpened;

    TimeEggEntry():m_nEggID(0), m_nSceneID(0), m_nBeginTime(0), 
        m_nEndTime(0), m_nDestroyTime(0), m_nRelation(0), m_nRoleID(0), m_nOpened(0){}

    bool doEncode(CParamPool &IOBuff);
    bool doDecode(CParamPool &IOBuff);
};
enum OpenEggType
{
    OpenEggType_None,
    OpenEggType_Together,
    OpenEggType_OpenAlone,
    OpenEggType_BeOpen,
    OpenEggType_System
};
//��Ҵ洢��ʱ�⵰�е���Ϣ
struct RoleTimeEggEntry
{
    unsigned int m_nRoleID;
    string m_strRoleName;
    unsigned int m_nSceneID;
    unsigned int m_nEggID;
    CItem m_item;
    string m_words;
    OpenEggType m_nOpenType;
    unsigned int m_nSex;

    RoleTimeEggEntry():m_nRoleID(0), m_nSceneID(0), m_nEggID(0), m_nOpenType(OpenEggType_None), m_nSex(0){}
};
//ʱ�⵰������Ϣ
struct CreateTimeEggEntry
{
    TimeEggEntry m_egg;
    vector<RoleTimeEggEntry> m_roleinfo;
    int nError;
    string strError;
};
//���ʱ�⵰��Ϣ��ѯ
struct QueryRoleTimeEggEntry
{
    unsigned int m_nRoleID;
    vector<TimeEggEntry> m_eggs;

    QueryRoleTimeEggEntry():m_nRoleID(0){}
};

//��ʱ�⵰
struct OpenTimeEggEntry
{
    unsigned int m_nEggID;
    OpenEggType m_nOpenType;
    unsigned int m_nOpenRoleID;
    std::vector<RoleTimeEggEntry> m_roleinfo;
    int nError;
    string strError;

    OpenTimeEggEntry():m_nEggID(0), m_nOpenType(OpenEggType_None), m_nOpenRoleID(0), nError(0){}
};

struct TimeEggSetting
{
    unsigned int m_nTimeEggOpenDuration;//����ʱ�ޣ�����
    unsigned int m_nTimeEggDestroyDuration;//����ʱ�ޣ�����
    std::list<CItem> m_listCostItem;
    std::list<CItem> m_listGiftItem;
    std::list<CItem> m_listTogetherMaleItem;
    std::list<CItem> m_listTogetherFemaleItem;
    std::list<CItem> m_listMaleItem;
    std::list<CItem> m_listFemaleItem;
    int m_nFixedMoneyReward;//�̶�������ȯ
    int m_nExtraMoneyReward;//���⽱����ȯ
    int m_nGiftDiscount;//����Ʒ�ۿ�
    string m_strHelpTitle;
    string m_strHelpContent;

    TimeEggSetting():m_nTimeEggOpenDuration(0), m_nTimeEggDestroyDuration(0), m_nFixedMoneyReward(0), m_nExtraMoneyReward(0), m_nGiftDiscount(100){}

    bool doEncode(CParamPool & IOBuff) const;
    bool doDecode(CParamPool & IOBuff);
};


struct SConstellationCardInfo 
{
    SConstellationCardInfo()
    {
        m_nIndex = 0;
        m_nConstellation = MAX_CONSTELLATION_COUNT;
        m_nCoolDownTime = 0;
        m_nCoolDownEndTime = 0;
    }

    ~SConstellationCardInfo()
    {
    }

    void doEncode(CParamPool &IOBuff)
    {
        IOBuff.AddUShort(m_nIndex);
        IOBuff.AddUChar(m_nConstellation);
        IOBuff.AddInt(m_nCoolDownTime);
    }

    unsigned short m_nIndex;
    unsigned char m_nConstellation;
    int m_nCoolDownTime;
    unsigned int m_nCoolDownEndTime;
};

class MagicLamp
{
public:
    MagicLamp();
    MagicLamp(const MagicLamp & record);
    virtual ~MagicLamp();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    int m_nCrytsal;
    int m_nLuckStar;
    int m_nMagicDust;
    int m_nMaxMainline;
    int m_nMaxTheme;
    int m_nCrystalRestoreTime;
    int m_nNextDayResetTime;
    int m_nTodayHaveGetVitNum;
    int m_nFavorableBuyVitCount;
};

struct MainlineParcloseRecord
{
    unsigned int      nParcloseID;
    bool              bIsActived;
    unsigned int      nStarCount;
    unsigned int      nUpdateTime;
    unsigned int      nQuickFinishNum;

    MainlineParcloseRecord(const MainlineParcloseRecord & record)
    {
        nParcloseID = record.nParcloseID;
        bIsActived = record.bIsActived;
        nStarCount = record.nStarCount;
        nUpdateTime = record.nUpdateTime;
        nQuickFinishNum = record.nQuickFinishNum;
    }

    MainlineParcloseRecord()
    {
        nParcloseID      = 0;
        nStarCount       = 0;
        bIsActived       = false;
        nUpdateTime      = 0;
        nQuickFinishNum  = 0;
    }

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << nParcloseID << bIsActived << nStarCount << nUpdateTime << nQuickFinishNum;
        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> nParcloseID >> bIsActived >> nStarCount >> nUpdateTime >> nQuickFinishNum;
        return true;
    }
};
typedef std::map<unsigned int,MainlineParcloseRecord> MapMainlineParcloseRecord;

struct ThemeParcloseRecord
{
    unsigned int nThemeID;
    unsigned int nParcloseID;
    bool    bIsActived;
    unsigned short nStarCount;
    unsigned int nUpdateTime;
    unsigned int nQuickFinishNum;

    ThemeParcloseRecord( const ThemeParcloseRecord & record )
    {
        nThemeID = record.nThemeID;
        nParcloseID = record.nParcloseID;
        bIsActived = record.bIsActived;
        nStarCount = record.nStarCount;
        nUpdateTime = record.nUpdateTime;
        nQuickFinishNum = record.nQuickFinishNum;
    }

    ThemeParcloseRecord()
    {
        nThemeID = 0;
        nParcloseID = 0;
        bIsActived = false;
        nStarCount = 0;
        nUpdateTime = 0;
        nQuickFinishNum = 0;
    }

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << nThemeID << nParcloseID << bIsActived << nStarCount << nUpdateTime << nQuickFinishNum;
        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> nThemeID >> nParcloseID >> bIsActived >> nStarCount >> nUpdateTime >> nQuickFinishNum;
        return true;
    }
};
typedef std::map<unsigned int, std::map<unsigned int,ThemeParcloseRecord> > MapThemeParcloseRecord;

struct DungeonRankInfo
{
    unsigned int nRoleID;
    std::string  strRoleName;
    unsigned int nParcloseID;
    unsigned int nStarCount;
    unsigned int nUpdateTime;

    bool operator== (const DungeonRankInfo& rhs )
    {
        if ( nRoleID == rhs.nRoleID )
        {
            return true;
        }

        return false;
    }

    DungeonRankInfo()
    {
        nRoleID = 0;
        strRoleName = "";
        nParcloseID = 0;
        nStarCount = 0;
        nUpdateTime = 0;
    }
};

// ħ����ƺ��ѽ�����Ϣ
class FriendContactInfo
{
public:
    FriendContactInfo();
    FriendContactInfo(const FriendContactInfo & contactinfo);
    virtual ~FriendContactInfo();

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int    m_nFriendRoleID;
    unsigned int    m_nContactNum;
    unsigned int    m_nGiveVitNum;
    unsigned int    m_nBeGiveVitNum;
    unsigned int    m_nGiveVitTime;
    unsigned int    m_nBeGiveVitTime;
};


// ��ֵ��������
class CChargePlan
{
public:
    CChargePlan( )
    {
        m_nChargeRangeLower = 0;	//��ֵ���� ����
        m_nChargeRangeUpper = 0;	//����
        m_nChargeMcoinRatio = 0;	//��������( ��M�� )
        m_nChargeMcoinQuota = 0;	//�̶�ֵ( ��M�� )
        m_nChargeMoneyRatio = 0;	//��������( ��ȯ )
        m_nChargeMoneyQuota = 0;	//�̶�ֵ( ��ȯ )
    }
    CChargePlan( unsigned int nChargeRangeLower, unsigned int nChargeRangeUpper,
        unsigned int nChargeMcoinRatio, unsigned int nChargeMcoinQuota,
        unsigned int nChargeMoneyRatio, unsigned int nChargeMoneyQuota )
    {
        m_nChargeRangeLower = nChargeRangeLower;		
        m_nChargeRangeUpper = nChargeRangeUpper;

        m_nChargeMcoinRatio = nChargeMcoinRatio;		
        m_nChargeMcoinQuota = nChargeMcoinQuota;

        m_nChargeMoneyRatio = nChargeMoneyRatio;		
        m_nChargeMoneyQuota = nChargeMoneyQuota;
    }
    ~CChargePlan( )
    {
    }
    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff.AddInt( m_nChargeRangeLower );	
        IOBuff.AddInt( m_nChargeRangeUpper );

        IOBuff.AddInt( m_nChargeMcoinRatio );	
        IOBuff.AddInt( m_nChargeMcoinQuota );

        IOBuff.AddInt( m_nChargeMoneyRatio );	
        IOBuff.AddInt( m_nChargeMoneyQuota );
        return true;
    }
public:
    unsigned int m_nChargeRangeLower;	
    unsigned int m_nChargeRangeUpper;		//��ֵ����

    unsigned int m_nChargeMcoinRatio;	
    unsigned int m_nChargeMcoinQuota;		//Mcoin

    unsigned int m_nChargeMoneyRatio;	
    unsigned int m_nChargeMoneyQuota;		//��ȯ
};

// �¿�
class MonthCardDB
{
public:
    unsigned int	nCardType;
    unsigned int nOpenTime;
    unsigned int nRewardTime;

    MonthCardDB()
    {
        nCardType = 0;
        nOpenTime = 0;
        nRewardTime = 0;
    }

    MonthCardDB( const MonthCardDB & card )
    {
        nCardType = card.nCardType;
        nOpenTime = card.nOpenTime;
        nRewardTime = card.nRewardTime;
    }

    bool doEncode( CParamPool &IOBuff ) const
    {
        IOBuff << nCardType << nOpenTime << nRewardTime;
        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        IOBuff >> nCardType >> nOpenTime >> nRewardTime;
        return true;
    }
};
typedef std::map<unsigned int,MonthCardDB> MonthCardDBTable;

class CDungeonInviteInfo
{
public:
    CDungeonInviteInfo()
    {
        m_nCostInviteNum = 0;
        m_nLastInviteTime = 0;
    }

    virtual ~CDungeonInviteInfo(){};

public:
    CDungeonInviteInfo( const CDungeonInviteInfo & info )
    {
        m_nCostInviteNum = info.m_nCostInviteNum;
        m_nLastInviteTime = info.m_nLastInviteTime;
        m_setInviteFriends = info.m_setInviteFriends;
    }

    void Reset()
    {
        m_nCostInviteNum = 0;
        m_nLastInviteTime = 0;
        m_setInviteFriends.clear();
    }
    
    bool doEncode( CParamPool &IOBuff )
    {
        IOBuff.AddUInt( m_nCostInviteNum );
        IOBuff.AddUInt( m_nLastInviteTime );
        IOBuff.AddUShort( (unsigned short)m_setInviteFriends.size());
        std::set<unsigned int>::iterator it = m_setInviteFriends.begin();
        for ( ; it != m_setInviteFriends.end(); ++it )
        {
            IOBuff.AddUInt( *it );
        }

        return true;
    }

    bool doDecode( CParamPool &IOBuff )
    {
        m_nCostInviteNum = IOBuff.GetUInt();
        m_nLastInviteTime = IOBuff.GetUInt();
        unsigned short nPlayerCount = IOBuff.GetUShort();
        for ( unsigned short i = 0; i < nPlayerCount; ++i )
        {
            unsigned int nRoleID = IOBuff.GetUInt();
            m_setInviteFriends.insert( nRoleID );
        }

        return true;
    }

public:
    unsigned int m_nCostInviteNum;
    unsigned int m_nLastInviteTime;
    std::set<unsigned int>  m_setInviteFriends;
};

//////////////////////////////////////////////////////////////////////////

class CBonus
{
public:
    CBonus();
    virtual ~CBonus() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    std::list<CItem>    m_listItems;    // ��Ʒ
    int                 m_nMoney;       // ��ȯ
    int                 m_nBindMCoin;   // ��M��
};

class StorageCapacityRecord
{
public:
    StorageCapacityRecord();
    StorageCapacityRecord(const StorageCapacityRecord & record);
    virtual ~StorageCapacityRecord();

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

public:
    int         m_nVipLevel;
    int         m_nAddCapacity;
};

//////////////////////////////////////////////////////////////////////////

enum ERewardType
{
    ERewardType_Normal = 0,     // ��ͨ��ҽ���
    ERewardType_VIP,            // VIP��ҽ���
};

class CReward : public CBonus
{
public:
    CReward();
    virtual ~CReward() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    unsigned char       m_nRewardType;  // ��������
};

//////////////////////////////////////////////////////////////////////////

// ���δ���
class ClothGroup
{
public:
    ClothGroup();
    virtual ~ClothGroup();

    unsigned char GetGroupID() const { return m_nGroupID; }
    void SetGroupID( unsigned char nGroupID ) { m_nGroupID = nGroupID; }

    bool IsEmpty() const { return m_mapClothes.empty(); }
    bool IsPosEmpty( unsigned char nItemPos ) const;

    void AddCloth( unsigned char nItemPos, itemtype_t nItemType );

    const std::map<unsigned char, itemtype_t>& GetClothes() const { return m_mapClothes; }
    void SetClothes( const std::map<unsigned char, itemtype_t> &rItems ) { m_mapClothes = rItems; }

    itemtype_t GetClothItemType( unsigned char nItemPos ) const;

protected:
    unsigned char                       m_nGroupID;     // ���δ�����ID
    std::map<unsigned char, itemtype_t> m_mapClothes;   // װ��λ�� -> ����
};

class RoleClothGroup : public ClothGroup
{
public:
    RoleClothGroup();
    virtual ~RoleClothGroup();

    unsigned short GetVIPRequest() const { return m_nVIPRequest; }
    void SetVIPRequest( unsigned short nVIPLevel ) { m_nVIPRequest = nVIPLevel; }

    const std::string& GetGroupName() const { return m_strGroupName; }
    void SetGroupName( const std::string &rGroupName ) { m_strGroupName = rGroupName; }

    bool doEncode( CParamPool &IOBuff ) const;
    bool doDecode( CParamPool &IOBuff );

private:
    unsigned short  m_nVIPRequest;      // VIP�ȼ�
    std::string     m_strGroupName;     // ���δ�������
};


//#pragma pack()

#endif//__DATASTRUCT_BASE_H__

