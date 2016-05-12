#ifndef __DATASTRUCT_DANCE_GROUP_H__
#define __DATASTRUCT_DANCE_GROUP_H__

#include <map>
#include <list>
#include <string>
#include <set>
#include <time.h>
#include <functional>

#include "DanceGroupInfoDef.h"
#include "Macro_Define.h"
#include "DataStruct_Base.h"

#include "../server/Macros/Macros.h"
#include "../socket/json/json.h"
class CParamPool;

//根据等级获取舞团最大人数上限

enum EMemberProperty
{
    EMemberProperty_Creator,               // 创始人
    EMemberProperty_OldMember,             // 老团员
    EMemberProperty_MengXin,               // 小萌新
};

unsigned int GetDanceGroupMaxMemberCount(unsigned int nLevel);

class CChangeDanceGroupNameQuery
{
public:
    CChangeDanceGroupNameQuery();
    ~CChangeDanceGroupNameQuery() {}

public:
    unsigned int        m_nGroupID;
    std::string         m_strNewName;
    unsigned int        m_nChangeTime;
};

//舞团活动信息
class CDanceGroupActivityInfo
{
public:
    CDanceGroupActivityInfo()
        :m_nDayPacket(1)
        , m_nStarLevel(1)
        , m_nCurrentPowerValue(0)
        , m_nProcessTime(0)
        , m_nRecordProcessTime((unsigned int)time(NULL))
    {}
    ~CDanceGroupActivityInfo(){}

public:
    void doEncodeToClient(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    unsigned short m_nDayPacket;        //第几天的礼包数据
    unsigned short m_nStarLevel;        //当前到第几个星星
    unsigned int m_nCurrentPowerValue;  //当前的动力值
    unsigned int m_nProcessTime;        //最后操作时间
    unsigned int m_nRecordProcessTime;  //舞团记录操作时间
};

class DanceGroupQuitInfo
{
public:
    DanceGroupQuitInfo();
    ~DanceGroupQuitInfo();

    void Set(unsigned int nRoleID, unsigned int nDanceGroupID, unsigned int nQuitStamp);

    unsigned int m_nRoleID;             // 退出的role
    unsigned int m_nDanceGroupID;       // 最后退出的帮会
    unsigned int m_nQuitStamp;          // 退出的时间戳
};

class CDanceGroupMember
{
public:
	CDanceGroupMember();
	~CDanceGroupMember();
public:
	void doEncodeToClient(CParamPool &IOBuff);
	void doEncode(CParamPool &IOBuff);
	void doDecode(CParamPool &IOBuff);

public:
	unsigned int		m_nRoleID;			//角色ID
	std::string			m_strRoleName;		//角色名称
	unsigned char		m_nTitle;			//舞团中职位
	unsigned char		m_nSex;				//性别
	unsigned short		m_nLevel;			//等级
	unsigned char		m_nState;			//当前状态
	unsigned int		m_nContribution;	//贡献度
	unsigned int     m_nTotalContribution; //贡献度总值
	unsigned int		m_nLastOffLineTime;	//最近的登出时间
	bool				m_bIsVIP;			// 是否VIP 
	unsigned short		m_nVIPLevel;		// VIP等级 
	unsigned int m_nInTime; //加入舞团时间
	unsigned int m_nLastProcessTime; //最近操作时间（重置有关的数据操作时才记录该时间，否则不记录）
	unsigned char		m_nProperty;			// 小萌新/老团员/创始人

	//活动信息
	unsigned short m_nNormalCount;
	unsigned short m_nMoneyCount;
	unsigned short m_nPointCount;
	std::string	m_strPacketUnlockList;
	std::map<unsigned short, unsigned short> m_mShopExchangeCount;// index -> use count

public:
	void ChangeContribution( int nValue );
};

//申请者信息
class CRequestDanceGroupInfo
{
public:
    CRequestDanceGroupInfo();
    CRequestDanceGroupInfo(unsigned int nRoleID, std::string strRoleName, unsigned char nSex, unsigned short nLevel, unsigned int nExpiredTime, bool bIsVIP, unsigned short nVIPLevel);
    ~CRequestDanceGroupInfo() {}

public:
    void doEncodeToClient(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nRoleID;          // 申请者ID
    std::string         m_strRoleName;      // 申请者名称
    unsigned char       m_nSex;             // 性别
    unsigned short      m_nLevel;           // 等级
    unsigned int        m_nExpiredTime;     // 失效时间
    bool                m_bIsVIP;           // 是否VIP
    unsigned short      m_nVIPLevel;        // VIP等级
};

class CDanceGroupRecordsInfo
{
public:
    CDanceGroupRecordsInfo();
    CDanceGroupRecordsInfo(unsigned int nGroupid, unsigned int nRoleId, std::string strContent,unsigned int nRecordTime, unsigned short nDay);
    ~CDanceGroupRecordsInfo() {}

    const CDanceGroupRecordsInfo& operator= (const CDanceGroupRecordsInfo &rRecordInfo);

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int        m_nId;              // 数据库Id
    unsigned int        m_nGroupId;
    unsigned int        m_nRoleId;          // 申请者ID
    std::string         m_strContent;
    unsigned int        m_nRecordTime;
    unsigned short      m_nDay;
};

class CDanceGroupManager;
//舞团信息
class CDanceGroupBaseInfo
{
public:
	CDanceGroupBaseInfo();
	~CDanceGroupBaseInfo();
public:
	void doEncodeAllInfoToClient(CParamPool &IOBuff);
	void doEncodeToClientData(CParamPool &IOBuff);
	void doEncode(CParamPool &IOBuff);
	void doDecode(CParamPool &IOBuff);
    void ResetCeremonyStamp(time_t now);

	unsigned int		m_nGroupID;						//舞团ID
	std::string			m_strGroupName;					//舞团名称
	unsigned int		m_nLeaderRoleID;				//舞团团长RoleID
	std::string			m_strLeaderRoleName;			//舞团团长名称
	unsigned short		m_nLevel;						//舞团等级
	unsigned short		m_nColor;						//舞团颜色
	unsigned short		m_nBadge;						//舞团徽章
	unsigned int		m_nResA;						//舞团资源A
	unsigned int		m_nResB;						//舞团资源B
	unsigned int		m_nResC;						//舞团资源C
	unsigned int		m_nHonor;						//舞团荣誉
	std::string			m_strProfile;					//舞团简介
	std::string			m_strAnnouncement;				//舞团公告
	unsigned int		m_nCreateTime;					//创建日期
	bool				m_bCanRequestEnter;				//是否可以申请加入
	unsigned int		m_nLastDismissTime;				//上次解散时间
	unsigned int		m_nLastCancelDismissTime;		//上次取消解散时间
	unsigned int		m_nLastChangeGroupNameTime;		//上次修改舞团名称时间
	std::string			m_nTitleName1;					//一级职位名称－团长
	std::string			m_nTitleName2;					//二级职位名称－副团长
	std::string			m_nTitleName3;					//三级职位名称－团务
	std::string			m_nTitleName4;					//四级职位名称－正常团员
	std::string			m_nTitleName5;					//五级职位名称－见习团员

    unsigned short      m_nEffect; //徽图案团徽特效
	std::string         m_strBadgeEffectLock; //团徽特效解锁后的ID存放(需要解锁的Id才会被放入该string中)

    // 处理逻辑：
    // 数据加载完毕之后，检测时间戳的有效性
    // 每日0点处理该时间戳，并广播
    // gameserver 更新这个时间戳，通知到 groupserver
	unsigned int		m_nCeremonyStamp;				// 入团仪式时间戳， 客户端不需要这个数据
	int         		m_nCeremonyTimes;				// 入团仪式举办的次数

    unsigned int        m_nChallengeActive;             // 舞团挑战活力值
};

class CDanceGroupInfo
{
	friend class CDanceGroupManager;
public:
	CDanceGroupInfo();
    CDanceGroupInfo(const CDanceGroupInfo &rInfo);
	~CDanceGroupInfo();
public:
	void doEncodeAllInfoToClient(CParamPool &IOBuff);
	void doEncode(CParamPool &IOBuff);
	void doDecode(CParamPool &IOBuff);

public:
	//舞团升级升级
	void Upgrade();
	//更新基本信息
	void UpdateBaseInfo(const CDanceGroupBaseInfo & rDGBaseInfo);
	//添加成员信息
	bool AddMemberInfo(const CDanceGroupMember & rDGMemberInfo);
	//更新成员信息
	bool UpdateMemberInfo(const CDanceGroupMember & rDGMemberInfo);
	//移除成员信息
	void RemoveMemberInfo(unsigned int nRoleID);
	//添加申请加入舞团信息
	void AddRequestInfo(const CRequestDanceGroupInfo & rDGRequestInfo);
	//移除玩家的请求信息
	unsigned int RemoveRequestInfo(unsigned int nRoleID);
	//修改舞团荣誉值
	void ChangeDanceGroupHonor(int nChangeValue,EChangeDanceGroupHonorCause eCause, int nPara);
public:
	//是否有效，未被解散
	bool IsValid(time_t tNow);
	//是否已经解散
	bool IsDismissed();
	//是否在解散CD中
	bool IsInDismissCD();
	//玩家是否已经在申请队列中
	bool IsInRequestQueue(unsigned int nRoleID);
	//获取成员信息
	CDanceGroupMember * GetGroupMember(unsigned int nRoleID);
    const CDanceGroupMember* GetGroupMember( unsigned int nRoleID ) const;
	//获取申请玩家信息
	const CRequestDanceGroupInfo & GetRequestDanceGroupInfo(unsigned int nRoleID);
	//当前成员数
	unsigned int GetMemberCount(){return (unsigned int)m_MemberMap.size();}
	//查看职位人数
	unsigned int GetTitleMemberCount(EGroupTitle eTitle);
	//获取舞团所有成员的ID
	void GetAllMember(std::list<unsigned int> &memberList);
    // 获取舞团管理成员ID
    void GetManagerMember(std::list<unsigned int> &rListManager);
    const std::map<unsigned int, CDanceGroupMember *>& GetAllMember() const { return m_MemberMap; }

private:
public:
	CDanceGroupBaseInfo	m_DGBaseInfo;								//基本信息
	std::map< unsigned int, CRequestDanceGroupInfo > m_RequestMap;	//申请加入的列表: RoleID -> CRequestDanceGroupInfo
	std::map< unsigned int, CDanceGroupMember * > m_MemberMap;		//舞团成员: RoleID -> CDanceGroupMember
	std::map<unsigned int, CDanceGroupRecordsInfo> m_recordsMap; //舞团记录 RecordTime ->CDanceGroupRecordsInfo

	CDanceGroupActivityInfo m_DGActivityInfo; //舞团活动公共信息
};

//舞团管理器
class CDanceGroupProcessor;
class CDanceGroupManager
{
	friend class CDanceGroupProcessor;
public:
	CDanceGroupManager();
	virtual ~CDanceGroupManager();
public:
	//重置所有数据
	void Reset();
	//获取舞团信息
	CDanceGroupInfo * GetDanceGroupInfo(unsigned int nGroupID);
    CDanceGroupInfo * GetDanceGroupInfo(const std::string &strDanceGroupName);
	//获取舞团列表
	std::map< unsigned int, CDanceGroupInfo * > & GetDanceGroupInfoList();
	//获取角色的申请加入舞团列表
	const std::set< unsigned int > & GetRequestGroupList(unsigned int nRoleID);
public:
	//检查申请进入舞团的条件
	ERequestEnterDanceGroupResult CheckRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查取消申请进入舞团的条件
	ECancelRequestEnterDanceGroupResult CheckCancelRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查批复条件
	EReplyRequestEnterDanceGroupResult CheckReplyRequestEnterDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID, unsigned int nRequestRoleID);
	//检查退出舞团条件
	EExitDanceGroupResult CheckExitDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查舞团传位条件
	EChangeDanceGroupLeaderResult CheckChangeDanceGroupLeaderCond(unsigned int nLeaderID, unsigned int nGroupID, unsigned int nNewLeaderID);
	//检查解散舞团条件
	EDismissDanceGroupResult CheckDismissDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查取消解散舞团条件
	ECancelDismissDanceGroupResult CheckCancelDismissDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查改变舞团职位条件
	EChangeDanceGroupTitleResult CheckChangeDanceGroupTitleCond(unsigned int nOperatorRoleID, unsigned int nGroupID, unsigned int nTargetRoleID, unsigned char nNewTitle);
	//检查开除舞团成员条件
	EKickOutDanceGroupMemberResult CheckKickOutDanceGroupMemberCond(unsigned int nOperatorRoleID, unsigned int nGroupID, unsigned int nTargetRoleID);
	//检查修改舞团颜色条件
	EChangeDanceGroupColorResult CheckChangeDanceGroupColorCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查修改舞团徽章条件
	EChangeDanceGroupBadgeResult CheckChangeDanceGroupBadgeCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查修改舞团职位名称条件
	EChangeDanceGroupTitleNameResult CheckChangeDanceGroupTitleNameCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查修改舞团简介条件
	EChangeDanceGroupProfileResult CheckChangeDanceGroupProfileCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查修改舞团公告条件
	EChangeDanceGroupAnnouncementResult CheckChangeDanceGroupAnnouncementCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查舞团捐赠资源条件
	EDonateDanceGroupResourceResult CheckDonateDanceGroupResourceCond(unsigned int nRoleID, unsigned int nGroupID, unsigned int nResA, unsigned int nResB, unsigned int nResC);
	//检查升级舞团条件
	EUpgradeGroupDanceResult CheckUpgradeDanceGroupCond(unsigned int nRoleID, unsigned int nGroupID);
	//检查修改舞团名字条件
	EChangeDanceGroupNameResult CheckChangeDanceGroupNameCond(unsigned int nRoleID, unsigned int nGroupID, time_t tTimeNow, unsigned int & nTimeCoolDown);

	//舞团团徽操作条件
	EDanceGroupBadgeResult CheckDanceGroupBadge( unsigned int nRoleId, unsigned int nGroup, bool bLeader = false );
	//舞团活动条件
	EDanceGroupActivityStarResult CheckDanceGroupActivityStar( unsigned int nRoleId, unsigned int nGroupId );
	//舞团商店
	EDanceGroupShopResult CheckDanceGroupShop( unsigned int nRoleId, unsigned int nGroupId );
	//舞团记录
	EDanceGroupRecrodsResult CheckDanceGroupRecrods( unsigned int nRoleId, unsigned int nGroupId );

    // 接受舞团成员邀请条件
    EAcceptDanceGroupMemberInvitationResult CheckAcceptInvitationCond(unsigned int nGroupID, unsigned int nSelfGroupID);

public:
	//添加舞团信息
	bool AddDanceGroupInfo(CDanceGroupInfo * pDGInfo);
	//移除舞团信息
	void RemoveDanceGroupInfo(unsigned int nGroupID);
	//更新舞团基本信息，返回当前被更新的舞团
	CDanceGroupInfo * UpdateDanceGroupBaseInfo(CDanceGroupBaseInfo & rDGBaseInfo);
	//添加舞团成员信息，返回当前添加的舞团
	CDanceGroupInfo * AddDanceGroupMemberInfo(unsigned int nGroupID, CDanceGroupMember & rDGMemberInfo);
	//更新舞团成员信息，返回当前被更新的舞团
	CDanceGroupInfo * UpdateDanceGroupMemberInfo(unsigned int nGroupID, CDanceGroupMember & rDGMemberInfo);
	//移除舞团成员信息，返回当前被操作的舞团
	CDanceGroupInfo * RemoveDanceGroupMember(unsigned int nGroupID, unsigned int nRoleID);
	//添加舞团申请信息，返回当前被操作的舞团
	CDanceGroupInfo * AddRequestDanceGroupInfo(unsigned int nGroupID, CRequestDanceGroupInfo & rRequestDGInfo);
	//移除舞团申请信息，返回当前被操作的舞团
	CDanceGroupInfo * RemoveRequestDanceGroupInfo(unsigned int nGroupID, unsigned int nRoleID, bool bRemoveFromDG);
	//舞团升级
	void DanceGroupUpgrade(CDanceGroupInfo *pDanceGroupInfo);

	//
	void AddDanceGroupRecord( unsigned int nGroupId, CDanceGroupRecordsInfo &rRecordsInfo );
    // 处理移除舞团
    bool ProcessRemoveDanceGroup(unsigned int nGroupID);
    // 处理取消移除舞团
    bool ProcessCancelRemoveDanceGroup(unsigned int nGroupID);


    // 添加一个玩家的 退出数据
    void InsertRoleQuitInfo(DanceGroupQuitInfo &quitInfo);
    DanceGroupQuitInfo* GetQuitInfo(unsigned int nRoleID);

private:
	//添加角色申请记录
	void _AddRequestEnterDanceGroupInfo(unsigned int nRoleID, unsigned int nGroupID, unsigned int nExpiredTime);
	//移除角色申请记录
	void _RemoveRequestEnterDanceGroupInfo(unsigned int nRoleID, unsigned int nGroupID, unsigned int nExpiredTime);
protected:
	std::map< unsigned int, CDanceGroupInfo * > m_GroupMap;						//舞团列表: GroupID -> CDanceGroupInfo
    std::multimap<unsigned int, unsigned int> m_multimapRemovedGroup;           // 舞团删除列表: remove time -> GroupID
	std::map< unsigned int, std::set< unsigned int > > m_RoleRequestGroupMap;	//角色申请加入舞团的列表: RoleID -> GroupID Set
    std::map<unsigned int, std::map<unsigned int, unsigned int> > m_mapRequestTime;           // 申请列表: request time -> RoleID, GroupID
	std::list< CDanceGroupInfo * > m_GroupList;									//舞团列表,临时存放数据,主要用于加载时排行榜数据的保存

    typedef std::map<unsigned int, DanceGroupQuitInfo> DanceGroupQuitInfoMap;
    DanceGroupQuitInfoMap m_quitInfoMap;

};

//////////////////////////////////////////////////////////////////////////

class DanceGroupChallengeChanceEventDB
{
public:
    DanceGroupChallengeChanceEventDB();
    virtual ~DanceGroupChallengeChanceEventDB();

    CLASS_PROPERTY(unsigned int, m_nRoleID, RoleID)
    CLASS_PROPERTY(unsigned int, m_nValue, Value)
    CLASS_PROPERTY(unsigned int, m_nResetTime, ResetTime)
};

enum EDanceGroupChallengeRankingState
{
    EDanceGroupChallengeRankingState_None = 0,
    EDanceGroupChallengeRankingState_GotReward,
    EDanceGroupChallengeRankingState_Invalid,

    EDanceGroupChallengeRankingState_Max
};

class DanceGroupChallengeRanking
{
public:
    DanceGroupChallengeRanking();
    DanceGroupChallengeRanking( int nStorey, short nRanking, 
        EDanceGroupChallengeRankingState eState = EDanceGroupChallengeRankingState_None );
    virtual ~DanceGroupChallengeRanking();

    CLASS_PROPERTY(int, m_nStorey, Storey)
    CLASS_PROPERTY(short, m_nRanking, Ranking)
    CLASS_PROPERTY(unsigned char, m_nState, State)
};

class DanceGroupChallengeRecordRankingInfo
{
public:
    DanceGroupChallengeRecordRankingInfo( unsigned int nStartTime, int nStorey );
    virtual ~DanceGroupChallengeRecordRankingInfo();

    void SetAllRoleID( std::list<unsigned int> &rAll );

    CLASS_PROPERTY(unsigned int, m_nStartTime, StartTime)
    CLASS_PROPERTY(int, m_nStorey, Storey)
    CLASS_PROPERTY_READONLY_BY_REF(std::list<unsigned int>, m_listRoleID, AllRoleID)
};

class DanceGroupChallengeOpendStoreyInfo
{
public:
    DanceGroupChallengeOpendStoreyInfo();
    virtual ~DanceGroupChallengeOpendStoreyInfo();

    bool IsFinished() const;
    void AddScore( unsigned int nScore );

    CLASS_PROPERTY(int, m_nStorey, Storey)
    CLASS_PROPERTY(int, m_nScene, Scene)
    CLASS_PROPERTY(int, m_nMusic, Music)
    CLASS_PROPERTY(int, m_nMusicMode, MusicMode)
    CLASS_PROPERTY(int, m_nMusicLevel, MusicLevel)
    CLASS_PROPERTY(unsigned int, m_nTargetScore, TargetScore)
    CLASS_PROPERTY(unsigned int, m_nCurrentScore, CurrentScore)
};

class DanceGroupChallengeRoleInfo
{
public:
    DanceGroupChallengeRoleInfo();
    DanceGroupChallengeRoleInfo( unsigned int nRoleID, const std::string &rName, 
        unsigned int nScore, unsigned int nInspire );
    virtual ~DanceGroupChallengeRoleInfo();

    CLASS_PROPERTY(unsigned int, m_nRoleID, RoleID)
    CLASS_PROPERTY_BY_REF(std::string, m_strRoleName, RoleName)
    CLASS_PROPERTY(unsigned int, m_nScore, Score)
    CLASS_PROPERTY(unsigned int, m_nInspire, Inspire)
};

class DanceGroupChallengeContribution
{
public:
    explicit DanceGroupChallengeContribution( int nStorey );
    virtual ~DanceGroupChallengeContribution();

    unsigned int GetRoleScore( unsigned int nRoleID ) const;
    unsigned int GetRoleInspire( unsigned int nRoleID ) const;

    const std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> >& 
        GetAllScoreInfos() const { return m_mapScore; }
    const std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> >& 
        GetAllInspireInfos() const { return m_mapInspire; }

    void AddRoleScore( unsigned int nRoleID, const std::string &rName, unsigned int nAddScore );
    void AddRoleInspire( unsigned int nRoleID, const std::string &rName, unsigned int nAddInspire );

private:
    void UpdateRank( std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> > &rMap, 
        unsigned int nOldKey, unsigned int nNewKey, DanceGroupChallengeRoleInfo *pInfo );

    CLASS_PROPERTY_READONLY(int, m_nStorey, Storey)
    CLASS_PROPERTY_READONLY(unsigned int, m_nTotalInspire, TotalInspire)

private:
    std::map<unsigned int, DanceGroupChallengeRoleInfo *> m_mapRoleInfo;    // key role id
    std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> > m_mapScore;     // key score
    std::multimap<unsigned int, DanceGroupChallengeRoleInfo *, std::greater<unsigned int> > m_mapInspire;   // key inspire
};

enum EDanceGroupChallengeUpdateRoleContributionType
{
    EDanceGroupChallengeUpdateRoleContributionType_ALL = 0,
    EDanceGroupChallengeUpdateRoleContributionType_Score,
    EDanceGroupChallengeUpdateRoleContributionType_Inspire,

    EDanceGroupChallengeUpdateRoleContributionType_Max
};

class DanceGroupChallengeUpdateRoleContribution
{
public:
    DanceGroupChallengeUpdateRoleContribution( unsigned int nGroupID, unsigned int nStartTime, unsigned int nRoleID );
    virtual ~DanceGroupChallengeUpdateRoleContribution();

    void SetType( EDanceGroupChallengeUpdateRoleContributionType eType );

    bool IsValid() const;

    CLASS_PROPERTY_READONLY(unsigned char, m_nType, Type)
    CLASS_PROPERTY_READONLY(unsigned int, m_nGroupID, GroupID)
    CLASS_PROPERTY_READONLY(unsigned int, m_nStartTime, StartTime)
    CLASS_PROPERTY(int, m_nStorey, Storey)
    CLASS_PROPERTY_READONLY(unsigned int, m_nRoleID, RoleID)
    CLASS_PROPERTY(unsigned int, m_nScore, Score)
    CLASS_PROPERTY(unsigned int, m_nInspire, Inspire)
};

class DanceGroupChallengeInfo
{
public:
    DanceGroupChallengeInfo( unsigned int nGroupID, unsigned int nStartTime );
    ~DanceGroupChallengeInfo();

    DanceGroupChallengeOpendStoreyInfo* GetOpendStoreyInfo( int nStorey ) const;
    void AddOpendStoreyInfo( DanceGroupChallengeOpendStoreyInfo *pOpendInfo );

    bool IsStoreyOpened( int nStorey ) const;
    bool IsStoreyFinished( int nStorey ) const;
    bool IsPrevStoreyFinished( int nStorey ) const;

    int GetCurrentStorey( unsigned char *pState ) const;

    DanceGroupChallengeContribution* GetStoreyContribution( int nStorey );
    void AddStoreyContribution( DanceGroupChallengeContribution *pContribution );

private:
    unsigned int    m_nGroupID;
    unsigned int    m_nStartTime;
    std::map<int, DanceGroupChallengeOpendStoreyInfo *> m_mapOpendStorey;       // key storey
    std::map<int, DanceGroupChallengeContribution *> m_mapStoreyContribution;   // key storey
};


//////////////////////////////////////////////////////////////////////////
/// 舞团秘境功能 BEGIN
//////////////////////////////////////////////////////////////////////////

const std::string keyItemId = "ItemId";
const std::string keyDuration = "Duration";
const std::string keyCount = "Count";

class FairyLandRewardItem
{
public:
    FairyLandRewardItem();
    ~FairyLandRewardItem(){}

    itemtype_t m_nItemId;
    int m_nDuration;
    itemcount_t m_nCount;

    Json::Value toJson();
    void Parse(const Json::Value &Jsval);
    bool operator == (const FairyLandRewardItem& rOther);
};

const std::string keyRewardBindMB = "BindMB";
const std::string keyRewardMoney = "Money";
const std::string keyRewardItems = "Items";

class FairyLandChapterReward
{
public:
    FairyLandChapterReward();
    FairyLandChapterReward(const FairyLandChapterReward& other);
    virtual ~FairyLandChapterReward(){}

    int m_nBindMB;
    int m_nMoney;
    std::list < FairyLandRewardItem >  m_listItems;

    Json::Value toJson();
    void Parse(const Json::Value &Jsval);
    void clear(){ m_nBindMB = 0; m_nMoney = 0; m_listItems.clear(); }
    void operator += (const FairyLandChapterReward& rOther);
};

/// <summary>
/// 秘境排行榜
/// <summary>
const std::string RankValue = "value";
const std::string RankName = "name";
const std::string RankRoleId = "id";
const std::string Rank = "rank";

class FairyLandRankCell
{
    friend class DanceGroupFairyLandGroupInfo;
public:
    FairyLandRankCell();
    virtual ~FairyLandRankCell(){}
    FairyLandRankCell(int, int ,const std::string &);
    int m_nRoleId;

    union
    {
        int m_nValue;
        int m_nBlessTimes;
        int m_nTotalSocre;
    };
    std::string m_strName;
    bool operator<(const FairyLandRankCell &rhs) const;
    bool operator>(const FairyLandRankCell &rhs) const;

protected:
    Json::Value toJson();
    void Parse(const Json::Value &Jsval);
};

/************************************************************************
CLASS： DanceGroupFairlLandPersonalInfo.h
Description:舞团秘境 ，每个舞团成员的秘境数据
************************************************************************/
const std::string keyStorageBox = "StroageBox";
const std::string keyPassedRewardRecord = "PassedRewardRecord";

class DanceGroupFairyLandPersonalInfo
{
public:

    DanceGroupFairyLandPersonalInfo();
    virtual ~DanceGroupFairyLandPersonalInfo(){}

    void AddPassedChapterReward(FairyLandChapterReward &cell);
    void GetPassedChapterReward(FairyLandChapterReward& ret);
    void ClearPassedChapterReward(){ m_PassedChapterReward.clear(); }

    void SetRoleId(int nRoleId) { m_nRoleId = nRoleId; }
    int GetRoleId() { return m_nRoleId; }

    void SetGroupId(int nGroupId) { m_nGroupId = nGroupId; }
    int GetGroupId() { return m_nGroupId; }

    void AddTotalScore(int nAddScore){ m_nTotalScore += nAddScore; };
    int GetTotalScore(){ return m_nTotalScore; }
    void TotalScoreOnNewDay(){ m_nYesterdayTotalScore = m_nTotalScore; m_nTotalScore = 0; }
    int GetYesterdayTotalScore(){ return m_nYesterdayTotalScore; }
    void ClearYesterDaySocer(){ m_nYesterdayTotalScore = 0; }

    void AddBlassTimes(int nAdd){ m_nBlassTimes += nAdd; }
    int GetBlassTimes(){ return m_nBlassTimes; }
    void ClearBlassTimes() { m_nBlassTimes = 0; }

    void SetCurChapterChallengeTimes(int nTimes);
    int GetCurChapterChallengeTimes(int nChapter);

    void SetLastChallengeChapterLvl(int nChapterLvl){ m_nLastChallengeChapterLvl = nChapterLvl; }
    int GetLastChallengeChapterLvl(){ return m_nLastChallengeChapterLvl; }
    void ClearLastChallenge() { m_nLastChallengeChapterLvl = 0; m_nLastChallengeChapterLvl = 0; }

    void AddStorageBoxItem(FairyLandRewardItem & item);
    void GetStorageBoxItems(std::list<FairyLandRewardItem>& listItems);
    void ClearStorageBoxItems();

    bool CheckIsHaveJoinReward(int nChapter);

    void SetPassRewardFlag(int nChapter);
    void SetJoinRewardFlag(int nChapter);

    void ParseRewardFlag(const std::string &str, unsigned char &sex, std::set<int> &setRewardFlag);
    void RewardFlagToString(unsigned char sex, const std::set<int> &setRewardFlag, std::string &str);

    void ParsePassedReward(const std::string &JsonFmt);
    std::string PassedRewardToJson();

    void ParseStorageBox(const std::string& JsonFmt);
    std::string StorageBoxToJson();

    FairyLandChapterReward m_PassedChapterReward;      // 玩家通关奖励
    std::list<FairyLandRewardItem> m_listStorageBoxItems;        // 玩家储物箱奖励

    unsigned char m_nSex;                   // 发奖励使用
//    std::set<int> m_setPassRewardFalg;      // 记录某一关卡已经拿到过通关奖励
    std::set<int> m_setJoinRewardFalg;      // 记录某一关卡参与情况（这个奖励是需要重复发放的）

private:

    int m_nRoleId;                          
    int m_nGroupId;
    int m_nTotalScore;                      // 总分
    int m_nYesterdayTotalScore;             // 昨日总分
    int m_nBlassTimes;                      // 祝福发次数
    int m_nLastChallengeChapterLvl;         // 挑战次数对应的关卡
    int m_nCurChapterChallengeTimes;        // 挑战次数

};

/************************************************************************
CLASS： DanceGroupFairlLandGroupInfo.h
 Description:舞团秘境 ，每个舞团的数据
************************************************************************/
class DanceGroupFairyLandGroupInfo
{
public:
    DanceGroupFairyLandGroupInfo();
    virtual ~DanceGroupFairyLandGroupInfo(){}

    bool ParseScoreTodayRank(const std::string &strJsonFormat);
    std::string ScoreTodayRankToJson();
    bool ParseScoreYesterdayRank(const std::string &strJsonFormat);
    std::string ScoreYesterdayToJson();
    bool ParseBlessRank(const std::string &strJsonFormat);
    std::string BlessRankToJson();

    int GetGroupId(){ return m_nGroupId; }
    void SetGroupId(int id){ m_nGroupId = id; }

    int GetCurChapterLvl(){ return m_nCurChapterLvl; }
    void SetCurChapterLvl(int nChatperLvl);

    void AddBlessTime(unsigned int nSecond)
    {
        if (m_nBlessBuffEndTimeStamp > (unsigned int )time(NULL))
        {
            m_nBlessBuffEndTimeStamp += nSecond;
        }
        else
        {
            m_nBlessBuffEndTimeStamp = (unsigned int)time(NULL) + nSecond;
        }
    }

    int GetTotalScore(){ return m_nTotalScore; }
    void SetTotalScore(int nTotalScore){ m_nTotalScore = nTotalScore; }

    bool TryAddTodayRank(int nRoleId, int nScore, const std::string& name);
    void SetYestedayRank(std::list<FairyLandRankCell> &list);
    bool TryAddBlessRank(int nRoleId, int nScore ,const std::string& name);

    void GetScoreTodayRank(std::list<FairyLandRankCell> &list){ list = m_listTodayScoreRank; }
    void GetScoreYesterdayRank(std::list<FairyLandRankCell> &list){ list = m_listYesterdayScoreRank; }
    void GetBlessRank(std::list<FairyLandRankCell> &list){ list = m_listBlessRank; }

    void SetBlessBuffEndTimeStamp(unsigned int nTimeStamp){ m_nBlessBuffEndTimeStamp += nTimeStamp; }
    unsigned int GetBlessBuffEndTimeStamp(){ return m_nBlessBuffEndTimeStamp; }

    void ClearTodayRank(){ m_listTodayScoreRank.clear(); }
    void ClearBlessRank(){ m_listBlessRank.clear(); }

    void SetLastBlessRole(unsigned int nRoleID, std::string strRoleName);
    unsigned int GetLastBlessRoleID() const;
    std::string GetLastBlessRoleName() const;

    static std::string JValueToString(Json::Value jv)
    {
        Json::FastWriter js;
        return js.write(jv);
    }
private:
    int RANKLEN;
    int m_nGroupId;
    int m_nCurChapterLvl;
    int m_nTotalScore;
    unsigned int m_nBlessBuffEndTimeStamp;
    std::string m_strLastBlessRoleName;
    unsigned int m_nLastBlessRoleID;

    std::list<FairyLandRankCell> m_listTodayScoreRank;
    std::list<FairyLandRankCell> m_listYesterdayScoreRank;
    std::list<FairyLandRankCell> m_listBlessRank;
    


    bool ParseRank(const std::string &strJsonFormat, std::list<FairyLandRankCell>& listRank);
    std::string RanktoJson(std::list<FairyLandRankCell>& listRank);

    bool TryAddRank(int nRole, int nValue, const std::string &name, std::list<FairyLandRankCell> &destRank);
};


class FairyLandLoadAllDB
{
public:
    FairyLandLoadAllDB();
    ~FairyLandLoadAllDB();

    std::map<int, DanceGroupFairyLandGroupInfo> *m_pGInfo;
    std::map<int, std::map<int, DanceGroupFairyLandPersonalInfo > > *m_pGPInfo;

    time_t m_nNewDayStamp;
    time_t m_nPassRewardStamp;
    time_t m_nStorgeRewardStamp;
};

class FairyLandLastBlessInfoDB
{
public:
    FairyLandLastBlessInfoDB(unsigned int nGroupID, unsigned int nLastRoleID, std::string strBlessRoleName, time_t endStamp);
    unsigned int m_nGroupID;
    time_t m_nBlessEndTimeStamp;
    unsigned int m_nLastBlessRoleID;
    std::string m_strLastBlessRoleName;
};

//////////////////////////////////////////////////////////////////////////
/// 舞团秘境功能 END
//////////////////////////////////////////////////////////////////////////

#endif //__DATASTRUCT_DANCE_GROUP_H__

