//////////////////////////////////////////////////////////////////////////
// File		:	DataStruct_Base.h
// Desc		:	需要通过网络传送的一些数据的数据结构，
//				这个文件客户端和服务器都需要使用
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
#pragma warning(disable:4201)//warning C4201: 使用了非标准扩展 : 无名称的结构/联合

#endif

#include <vector>
#include <list>
#include <string>
using namespace std;

//#pragma pack(1)

//封停标志及时间
#define STATE_BIT_LOGIN		0x00000001	//是否已经登录的标志
#define STATE_BIT_BLOCK		0x00000002	//低3-5位是封停类型
#define STATE_BIT_DISABLE	0x00000100	//账号是否禁用的标志
extern unsigned int s_nBlockTime[8];

const unsigned int CMD_Announce = 0x00000001;//公告
const unsigned int CMD_KickRole = 0x00000002;//踢掉角色
const unsigned int CMD_Shutdown = 0x00000003;//停机公告
const unsigned int CMD_AdminMsg = 0x00000004;//管理公告信息

class CQuestDoc;
class AchievementConfig;

//////////////////////////////////////////////////////////////////////////
//账号信息（这里只列出从客户端传到服务端校验的数据结构，注册在网站完成，
//这里不需要完整的数据结构）
typedef class AccountInfo
{
public:
    unsigned int        nAccount;                 //游戏内部账号 唯一
    int                 nVersionType;             //版本ID,免费或收费
    unsigned short      nPID;                     //平台id
    char                szPUID[MAX_PUID_LEN+1];   //平台用户id
    int                 nType;                    //账号类型 自动注册账号还是非自动注册账号.0手工注册，1自动注册
    __int64             SessionID;                //Gate Session ID
    char                szName[MAX_NAME_LEN];
    char                szPass[MAX_PASS_LEN];     //密码  留着暂时不用密码验证
    char                szIDCard[MAX_IDCARD_LEN]; //注册身份证号码
    char                szPhone[MAX_PHONE_LEN];   //电话号码
    char                szLoginIp[MAX_IP_LEN];    //登陆IP
    unsigned int        nBlockTime;               //账号被锁的时间,从1970年1月1日0时0分0秒到该时间所经历的秒数
    unsigned int        nBlockState;              //账号状态 由低到高各BIT意义： 0 - 是否登入； 1 - 是否被锁 ； （2，3,4）-BLOCK类型
    unsigned int        nLastLoginTime;           //最近登入时间
    unsigned int        nLastLogoutTime;          //最近登出时间
    unsigned int        nLoginError;              //登入错误纪录
    unsigned int        nLoginState;              //登录的状态
    unsigned int        nTimeLogin;               //上线时间
    unsigned short      nSlot;                    //玩家对应的Gate所在socket连接
    __int64             nAccountSessionID;        //Account Session ID
    std::string         strDeviceID;              //玩家虚拟设备号（一般设备恢复出厂设置后会发生变化）

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
    char            szSignature[MAX_SIGNATURE_LEN];    //签名
    unsigned char   nColor;
    unsigned char   nInstallerId;
    unsigned char   m_nVersionType;

protected:
    std::string     m_strBirthday;              // 生日, 格式为: "19900601"
};

typedef struct ROLEINFO_CHECKISEXIST
{
    unsigned int nRoleId;

    ROLEINFO_CHECKISEXIST(){
        nRoleId = 0;
    }
}*LPROLEINFO_CHECKISEXIST;

//////////////////////////////////////////////////////////////////////////
//角色基本信息
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
    unsigned int    m_nRoleID;              //角色序列号

protected:
    std::string     m_strName;              // 角色名称
};

struct OtherRoleInfo_Attr :public RoleBaseInfo
{
    unsigned char  nSex;//身体类型,即性别,Sex_Type
    unsigned int   nExp;
    unsigned short nLevel;	
    unsigned short nPre;
    unsigned short nAPt;		//体力
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
    unsigned short  m_nPID;                         // 平台ID
    unsigned char   m_nSex;                         // 性别, Sex_Type
    int             m_nMoney;                       // 金钱
    unsigned int    m_nExp;                         // 经验
    unsigned short  m_nLevel;                       // 级别

    unsigned int    m_nHot;                         // 人气
    unsigned int    m_nPre;                         // 声望
    unsigned int    m_nInt;                         // 智力
    unsigned int    m_nAPt;                         // 体力

    unsigned char   m_nConstellation;               // 星座
    unsigned char   m_nSkinColor;                   // 肤色

    unsigned int    m_nPriv;                        // 角色权限
    unsigned int    m_nFamilyId;                    // 家族ID

    char            m_szFamilyName[MAX_FAMILY_NAME];
    char            m_szSignature[MAX_SIGNATURE_LEN];       //签名

    unsigned char   m_anBeiBaoKuozhan[BEIBAOKUOZHAN_MAX_ITEM_TYPE_NUM];

    unsigned char   m_nBadgeGridNum;
    unsigned int    m_nRoleCreateTime;              // 角色创建时间
    unsigned int    m_nLastLoginTime;               // 上次登录时间
    unsigned int    m_nLastLogoutTime;              // 上次登出时间
    unsigned int    m_nTimeOnline;
    unsigned int    m_nAddictionTime;               // 沉迷时间
    unsigned int    m_nAddictionOfflineTime;        // 防沉迷-累计下线时间
    unsigned int    m_nQuestDone;                   // 任务完成数
    unsigned int    m_nLastMoneyResetTime;
    unsigned int    m_nLastMoneyResetMailRemindTime;
    unsigned int    m_nLastMoneyResetMallRemindTime;
    unsigned int    m_nDanceGroupID;

    char            m_szInvitationCode[MAX_INVITATION_CODE_LEN];    //邀请码
    unsigned char   m_nClothGroupId;                // 当前服饰搭配id
    unsigned int    m_nDivorceAgreementID;          // 离婚协议书ID

    unsigned int    m_nAccountCreateTime;           // 账号创建时间

protected:
    std::string     m_strBirthday;                  // 生日, 格式为: "19900601"
};


//////////////////////////////////////////////////////////////////////////
//任务基本信息
class CQuestDoc;
class CQuestMusic;
class CQuestItem;
class CQuestDocNew;

struct TargetInfo 
{
    unsigned char  m_nType;																// 歌曲 物品消耗 物品搜集
    unsigned int m_nIndex;															// 歌曲ID 物品ID 物品ID
    unsigned short m_nCount;															// 歌曲类型的任务，CQuestDoc无该字段，每首歌只有一次。完成歌曲后该字段为1。
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
    unsigned short m_nQuestID;										// 任务 ID
    unsigned int   m_nTime;											// 接任务的时间
    map< unsigned char, list<TargetInfo > >	m_mapTarget;			// < 类型 任务表 >
    unsigned char m_nState;											// 见枚举eQuestState含义
    CQuestDoc *m_pQuest;	                                        // 该指针指向静态数据内存，析构不能释放！
    CQuestDocNew * m_pQuestNew;										// 该指针指向静态数据内存，析构不能释放！（新任务系统）

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
//任务历史信息
class CRepeatedHistoryQuest
{
public:
    CRepeatedHistoryQuest (unsigned short nQuestID = 0, unsigned int ttime = 0, unsigned int nCount = 0 );
    bool operator==(const CRepeatedHistoryQuest &rhs);
    bool operator==(unsigned short nQuestID);
    CRepeatedHistoryQuest& operator=(const CRepeatedHistoryQuest& rhs);
public:
    unsigned short	m_nQuestID;		// 任务 ID
    unsigned int	m_nTime;		// 完成任务的时间
    unsigned int	m_nCount;		// 完成的次数	
public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
};

//任务更新数据
typedef struct QUESTUPDATEINFO
{
    int				m_nModeID;				// 模式
    int				m_nSceneID;				// 场景
    int				m_nPlayerNum;			// 最少玩家个数
    int				m_nMusicDifficulty;		// 音乐难度
    int				m_nMusicID;				// 音乐
    int				m_nRank;				// 最低名次
    unsigned int	m_nScore;				// 最低评分
    int				m_nPerfectNum;			// 最少p数量
    int				m_nFailNum;
    int				m_nConPerfectNum;	    // 最少连p数量
    int				m_nComboNum;		    // 最小combo数量
    int             m_nGrade;				// 评定

    int				m_FriendNum;			//在场好友个数
    int				m_GroupNum;				//在场同舞团玩家数
    int				m_LoverNum;				//在场情侣个数
    bool			m_bFree;				//是否自由组队
	bool			m_bIsMatch;				//是否是匹配的时候加入
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
    unsigned char		m_nState;	//	见 AchivementState: AchievementProcessing = 0 AchievementCompleted = 1,	AchievementGotReward = 2
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
//gm系统提交的控制指令
typedef struct CMDINFO
{
    unsigned int nServer;//执行指令的server
    unsigned int nCmdID;//指令类型
    unsigned int timeCmd;//执行指令的时间
    unsigned int nCmdParam;//指令参数
    char szCmdParam[MAX_ANNOUNCE_LEN];//指令参数
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
    unsigned short	nIndex;//gameserver上，playermanager里的index，从1开始
};


struct FRIENDINFO : public RoleBaseInfo
{
    friend bool operator<(const FRIENDINFO& lhs, const FRIENDINFO& rhs);
    char status;//0在线好友 1在线非好友 2 在线黑名单 3不在线
    int  nLine;
    unsigned short nLevel;

    char szFamilyName[MAX_FAMILY_NAME];	//家族名

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



//普通物品,ITEMINFO
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
    __int64         m_nItemID;                  // 物品ID
    itemtype_t      m_nTypeID;                  // 物品类型
    unsigned short  m_nIndex;                   // 物品格子
    unsigned short  m_nColumn;                  // 在那个地方，比如身上，或者装备栏
    unsigned short  m_nCount;                   // 数量
    short           m_nUseTimes;                // 剩余使用次数
    int             m_nDuration;                // 有效时间，-1表示没有时间限制
    bool            m_bEquiped;                 // 是否装备在身上 0不装备 1已装备
    int             m_nDynamicBoxRelatedValue;  // 动态礼盒关联值
};

// 物品ID
const itemtype_t ITEMID_CHANGENAMECARD =	31003;							// 改名卡
const itemtype_t ITEMID_WEEKLYVIPCARD	=	31005;							// VIP周卡
const itemtype_t ITEMID_MONTHLYVIPCARD =	31006;							// VIP月卡
const itemtype_t ITEMID_SEASONVIPCARD	=	31007;							// VIP季卡
const itemtype_t ITEMID_YEARLYVIPCARD	=	31008;							// VIP年卡
const itemtype_t ITEMID_SMALLSPEAKER	=	32001;							// 小喇叭
const itemtype_t ITEMID_BIGSPEAKER		=	32002;							// 大喇叭
const itemtype_t ITEMID_RECHECKCARD	=	31004;							// 补签卡
const itemtype_t ITEMID_TULIP			=	32005;							// 郁金香
const itemtype_t ITEMID_LILY			=	32006;							// 百合
const itemtype_t ITEMID_ROSE			=	32007;							// 玫瑰
const itemtype_t ITEMID_BOUQUET		=	12345;							// 花束
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_PRIMARY = 31000;			// 初级徽章卡
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_INTERMEDIATE = 31001;		// 中级徽章卡
const itemtype_t ITEMID_EXTEND_BADAGE_GRID_CARD_LEVEL_ADVANCED = 31002;	// 高级级徽章卡
const itemtype_t C_COUPLE_REQUEST_ITEM = 31017;					// 情侣请求所需物品
const itemtype_t C_COUPLE_CREATE_MARRIAGE_ROOM_ITEM = 31018;	// 创建情侣房间所需物品
const itemtype_t C_DIVORCE_ITEM = 31019;						// 离婚所需物品
const itemtype_t C_ITEM_COMMON_RING = 21000;					// 结婚发放的普通结婚戒指

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
	unsigned int    m_nVipOutTimeDayIndex; //vip 失效时的 dayindex
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
    int m_nLine;                        // -1 表示不在线
    string m_strLineName;               // 线名字
    unsigned int m_nRoomID;             // 房间ID
    unsigned char m_nState;             // 0空闲，1等待 2观战 3比赛
    unsigned char m_nRelationType;
    unsigned char m_nCallBy;
    unsigned char m_nCallTo;
    bool m_bIsVIP;
    unsigned short m_nVIPLevel;
    unsigned int   m_nOfflineTime;      // 离线时间
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
    char				m_szRoleName[MAX_NAME_LEN]; //角色名称
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
    unsigned short m_nResult; // 操作结果
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

// 技能类型
typedef unsigned char SkillType;
typedef unsigned short SkillID;

// 以下数据结构用于宠物信息同步
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
    unsigned int   m_nPetID;               // 宠物ID
    std::string    m_strName;              // 宠物昵称
    std::string    m_strDefaultName;       // 宠物默认昵称
    unsigned char  m_nPotentialID;         // 宠物潜质
    unsigned char  m_nPetAttributeType;    // 宠物属性类型
    unsigned short m_nPetType;             // 宠物类型
    unsigned short m_nLevel;               // 当前等级
    unsigned short m_nMaxLevel;            // 当前品质最大等级
    unsigned int   m_nExp;                 // 宠物经验
    unsigned int   m_nMaxExpOfLevel;       // 宠物当前等级最大经验值
    unsigned char  m_nQualityType;         // 当前品质
    unsigned char  m_nEvolutionTimes;      // 进化次数
    unsigned short m_nEndurance;           // 当前耐力
    float          m_nEnduranceRecoveryTime;// 当前耐力恢复时间（倒计时秒数）
    unsigned short m_nMaxEndurance;        // 最大耐力
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
    unsigned int           m_nPetID;            // 宠物ID
    int                    m_nDanceSkillCount;  // 跳舞技能数
    int                    m_nFightSkillCount;  // 战斗技能数
    std::list<PetMsgSkill> m_listDanceSkill;    // 跳舞技能列表
    std::list<PetMsgSkill> m_listFightSkill;    // 战斗技能列表
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
    unsigned char               m_nSex;                     // 性别,Sex_Type
    unsigned char               m_nConstellation;           // 星座
    unsigned char               m_nSkinColor;               // 皮肤颜色
    bool                        m_bVIP;

    unsigned short              m_nLevel;
    unsigned short              m_nTitle;                   // 称号
    unsigned int                m_nHot;                     // 人气
    unsigned int                m_nSocial;                  // 社交指数

    unsigned int                m_nExp;                     // 当前的EXP
    int                         m_nCurLine;                 // 当前所在线
    unsigned short              m_nRoomID;                  // 房间ID 
    unsigned char               m_nRoomState;               // 状态(游戏中/等待中 0/1)

    std::vector<itemtype_t> m_vecEquipedItem;               // 主要是装备的物品
    std::vector<itemtype_t> m_vecWishItem;                  // 愿望物品
    unsigned char               m_nVIPLevel;                // 好友VIP等级
    unsigned int                m_nAchScore;                // 好友成就积分
    std::vector<unsigned int>   m_vecAch;                   // 好友最新完成成就
    unsigned int                m_nDanceGroupID;            // 舞团编号
    unsigned int                m_nCoupleID;                // 情侣ID
    unsigned int                m_nCoupleDate;              // 交往纪念日
    unsigned int                m_nMarriageDate;            // 结婚纪念日
    unsigned int                m_nSmallRoleID;             // 同性结婚小受ID

    bool                        m_bHasPet;                  // 是否携带宠物
    PlayerPet                   m_playerPet;                // 宠物基础信息
    PlayerPetSkill              m_playerPetSkill;           // 宠物技能信息

    std::list<PlayerMedal>      m_listActivityMedals;       // 激活勋章
    std::list<PlayerMedal>      m_listShowMedals;           // 显示勋章
    unsigned int                m_nMedalScore;              // 勋章积分

    unsigned int                m_nPerpetualEquipCount;     // 永久服饰总数
    unsigned int                m_nPerpetualEffectScore;    // 光效闪亮值

protected:
    std::string                 m_strRoleName;              // 角色名称
    std::string                 m_strBirthday;              // 生日, 格式为: "19900601"
    std::string                 m_strSignature;             // 个人签名
    std::string                 m_strCurLineName;           // 当前所在线名字
    std::string                 m_strDanceGroupName;        // 舞团名字
    std::string                 m_strCoupleName;            // 情侣名称
};


enum EOfflinePlayerDataFrom
{
    EOfflinePlayerDataFrom_None,
    EOfflinePlayerDataFrom_Couple,
    EOfflinePlayerDataFrom_Divorce,
    EOfflinePlayerDataFrom_Logout,
};

// 玩家形象离线信息
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
    unsigned int            m_nRoleID;          //角色ID
    std::string             m_strRoleName;      //角色名称
    char                    m_nSex;             //玩家性别
    char                    m_nSkin;            //玩家皮肤
    unsigned short          m_nLevel;           //玩家等级
    bool                    m_bVIP;             //是否VIP
    unsigned short          m_nVIPLevel;        //vip等级
    std::vector<DBItem>     m_vecRoleItem;      //玩家物品
    bool                    m_bHasCarriedPet;   //是否携带宠物
    PlayerPet               m_playerPet;        //宠物
    PlayerPetSkill          m_playerPetSkill;   //宠物技能
    std::map<itemid_t,unsigned int> m_mapClothEffect;   // 衣服特效
    std::map<unsigned short,std::set<ESevenColorType> >  m_mapClothEffectSevenColor; // 衣服特效七色彩虹
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
    int m_nLine;				// 服务器线
    string m_strServerName;		// 服务器名称
    int m_nOnlinePlayersNum;	// 在线玩家数
    int m_nLimitNum;			// 可承载玩家上限
    unsigned char m_nInstallerID;// 支持的安装包版本:新手包或者完整包
    bool m_bEnableAudition;		// 是否是海选线
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
//时光蛋
struct TimeEggEntry
{
    unsigned int m_nEggID;
    unsigned int m_nSceneID;
    unsigned int m_nBeginTime;
    unsigned int m_nEndTime;
    unsigned int m_nDestroyTime;
    unsigned int m_nRelation;
    unsigned int m_nRoleID;//埋蛋对方id
    string m_strRoleName;//埋蛋对方名字
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
//玩家存储在时光蛋中的信息
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
//时光蛋创建信息
struct CreateTimeEggEntry
{
    TimeEggEntry m_egg;
    vector<RoleTimeEggEntry> m_roleinfo;
    int nError;
    string strError;
};
//玩家时光蛋信息查询
struct QueryRoleTimeEggEntry
{
    unsigned int m_nRoleID;
    vector<TimeEggEntry> m_eggs;

    QueryRoleTimeEggEntry():m_nRoleID(0){}
};

//打开时光蛋
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
    unsigned int m_nTimeEggOpenDuration;//开蛋时限，分钟
    unsigned int m_nTimeEggDestroyDuration;//销毁时限，分钟
    std::list<CItem> m_listCostItem;
    std::list<CItem> m_listGiftItem;
    std::list<CItem> m_listTogetherMaleItem;
    std::list<CItem> m_listTogetherFemaleItem;
    std::list<CItem> m_listMaleItem;
    std::list<CItem> m_listFemaleItem;
    int m_nFixedMoneyReward;//固定奖励金券
    int m_nExtraMoneyReward;//额外奖励金券
    int m_nGiftDiscount;//纪念品折扣
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

// 魔法神灯好友交互信息
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


// 充值奖励规则
class CChargePlan
{
public:
    CChargePlan( )
    {
        m_nChargeRangeLower = 0;	//充值区间 下限
        m_nChargeRangeUpper = 0;	//上限
        m_nChargeMcoinRatio = 0;	//返还比例( 绑定M币 )
        m_nChargeMcoinQuota = 0;	//固定值( 绑定M币 )
        m_nChargeMoneyRatio = 0;	//返还比例( 金券 )
        m_nChargeMoneyQuota = 0;	//固定值( 金券 )
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
    unsigned int m_nChargeRangeUpper;		//充值区间

    unsigned int m_nChargeMcoinRatio;	
    unsigned int m_nChargeMcoinQuota;		//Mcoin

    unsigned int m_nChargeMoneyRatio;	
    unsigned int m_nChargeMoneyQuota;		//金券
};

// 月卡
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
    std::list<CItem>    m_listItems;    // 物品
    int                 m_nMoney;       // 金券
    int                 m_nBindMCoin;   // 绑定M币
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
    ERewardType_Normal = 0,     // 普通玩家奖励
    ERewardType_VIP,            // VIP玩家奖励
};

class CReward : public CBonus
{
public:
    CReward();
    virtual ~CReward() {}

public:
    void doEncode(CParamPool &IOBuff);

public:
    unsigned char       m_nRewardType;  // 奖励类型
};

//////////////////////////////////////////////////////////////////////////

// 服饰搭配
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
    unsigned char                       m_nGroupID;     // 服饰搭配组ID
    std::map<unsigned char, itemtype_t> m_mapClothes;   // 装备位置 -> 服饰
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
    unsigned short  m_nVIPRequest;      // VIP等级
    std::string     m_strGroupName;     // 服饰搭配组名
};


//#pragma pack()

#endif//__DATASTRUCT_BASE_H__

