#ifndef __QUESTDOC_H__
#define __QUESTDOC_H__

#include "../socket/ParamPool.h"
#include "NsytDoc.h"
#include <string>
#include <map>
#include "../datastructure/Macro_Define.h"
#include "QuestDef.h"
#include "QuestTrigger.h"
#include "../datastructure/DataStruct_Base.h"

using namespace std;

struct QUESTUPDATEINFO;



class CQusetBaseInfo
{
public:
	CQusetBaseInfo();
	~CQusetBaseInfo();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff)
	{
		IOBuff.AddUShort(m_nGroupID);
		IOBuff.AddUShort(m_nQuestID);
		IOBuff.AddString(m_strQustName.c_str());
		IOBuff.AddUChar(m_nType);
	}
public:
	unsigned short m_nGroupID;				// 对游戏数据无影响
	unsigned short m_nQuestID;
	string m_strQustName;
	unsigned char m_nType;					// 譬如情侣任务
};

class CQuestItem
{
public:
	CQuestItem();
	~CQuestItem();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff)
	{
		IOBuff.AddUInt(m_nItemID);
		IOBuff.AddUShort(m_nItemCount);
		IOBuff.AddUInt(m_nValidTime);
	}
public:
	itemtype_t m_nItemID;
	itemcount_t m_nItemCount;
	int m_nValidTime;
};

class CQuestRequirement
{
public:
	CQuestRequirement();
	~CQuestRequirement();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned short	m_nLevel;
	unsigned short	m_nEndLevel;
	unsigned char	m_nSex;
	unsigned int	m_nHotValue;
	unsigned char	m_nTitleID;
	unsigned char	m_nPartyLevel;			// 门派
	unsigned char	m_nGroupLevel;			// 舞团
	unsigned char	m_nMarriageLevel;
	unsigned char	m_nWeekDay;				// 0表示整个星期都有效，1-7表示周日至周六
	unsigned int	m_nBeginTime;
	unsigned int	m_nEndTime;
	unsigned short	m_nPreviousQusetID;
	unsigned short	m_nExclusiveQustID;	
	unsigned char	m_nTriggerType;	
	map<itemtype_t, CQuestItem *> m_mapRequredItem;
	unsigned int	m_nLimitedTime;
};

class CQuestMusic
{
public:
	CQuestMusic();
	~CQuestMusic();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned short		m_nMusicID;
	unsigned char		m_nModelID;
	unsigned char		m_nSceneID;
	unsigned char		m_nDifficulty;
	unsigned char		m_nMinPlayerNum;
	unsigned char		m_nMinRank;
	unsigned int		m_nMinScore;
	unsigned short		m_nMinPerfect;
	unsigned short		m_nMinConPerfect;
	unsigned short		m_nMaxFail;
	unsigned short		m_nMinCombo;
	bool				m_bConRequired;
	unsigned short		m_nRequiredMatchNum;
	map<itemtype_t, CQuestItem *> m_mapCloth;
};

class CAttrTarget
{
public:
	CAttrTarget();
	~CAttrTarget();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned short	m_nTargetLevel;				// 等级
	unsigned int	m_nTargetHotValue;			// 人气
	unsigned short	m_nTargetFriendNum;			// 好友数
	unsigned char	m_nTargetMarriageLevel;		// 婚姻等级
	unsigned char	m_nTargetGroupLevel;		// 舞团
	unsigned char	m_nTargetSchool;			// 门派

	int				m_nPayCount;                // 支付成功次数
	unsigned char	m_nTargetTitle;			    // 称号
	int				m_nTargetEx1;
	int				m_nTargetEx2;
	int				m_nTargetEx3;
};

class CQuestReward
{
public:
	CQuestReward();
	~CQuestReward();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs);
public:
	void doEncode(CParamPool &IOBuff);
public:
	unsigned int m_nRewardExp;
	unsigned int m_nRewardMoney;
	map<itemtype_t, CQuestItem *> m_mapRewardItemMale;
	map<itemtype_t, CQuestItem *> m_mapRewardItemFemale;
    map<itemtype_t, CQuestItem *> m_mapRewardVipItemMale;
    map<itemtype_t, CQuestItem *> m_mapRewardVipItemFemale;
};

class CQuestDoc
{
public:
	CQuestDoc();
	~CQuestDoc();
public:
	void ReadDoc(CFileStream* pFs);
	void WriteDoc(CFileStream* pFs); 

	void doEncode(CParamPool &IOBuff);
public:
	unsigned short GetID(); 
	unsigned char  GetType()const;
	const char *   GetQuestName()const;

	unsigned char  GetWeekDay();
	unsigned int   GetLimitedTime() const;
	unsigned int   GetStartTime()const;
	unsigned int   GetEndTime()const;
	const char   * GetDescription()const;
	CQuestReward & GetQuestReward();
	CQuestMusic  * GetQuestMusic(unsigned short nIndex);
	map<itemtype_t, CQuestItem *> * GetQuestMusicRequiredCloth(unsigned short nIndex)const;
	unsigned short GetExclusiveQuestID();

	bool IsDailyQuest()const;
	bool IsGrowQuest()const;
	bool IsActivityQuest()const;
	bool IsPartyQuest()const;
	bool IsDanceGroupQuest()const;
	bool IsLoversQuest()const;
	bool IsRepeatedQuest()const; 

	bool IsValidTime();

public:
	// Base info
	CQusetBaseInfo m_CQusetBaseInfo;	
	// Requirements
	CQuestRequirement m_CQusetRequirement;
	// Music
	map<unsigned short, CQuestMusic *> m_mapMusicTarget;
	// Item use
	map<itemtype_t, CQuestItem *> m_mapItemUse;
	// Item Collection
	map<itemtype_t, CQuestItem *> m_mapItemCollection;
	// Target
	CAttrTarget m_attrTarget;
	// Reward
	CQuestReward m_nRewrad;
	// Describe
	string m_strDescribe;
	// Prompts
	string m_strPrompts;
	// Final Words
	string m_strFinalWords;	// 提示
};

class CQuestNewBaseInfo
{
public:
	CQuestNewBaseInfo();
	~CQuestNewBaseInfo(){}

public:
	bool ReadDoc(CFileStream* pFs);

public:
	unsigned short ID ;
	/// <summary>
	/// 名称
	/// </summary>
	string Name ;
	/// <summary>
	/// 组ID
	/// </summary>
	unsigned short GroupID ;
	/// <summary>
	/// 可完成次数
	/// </summary>
	unsigned int CompleteTimes ;
	/// <summary>
	/// 目标，个人或团体
	/// </summary>
	short QuestTarget ;
	/// <summary>
	/// 时限类型
	/// </summary>
	char  TimeType ;
	/// <summary>
	/// 开启时间
	/// </summary>
	unsigned int StartTime ;
	/// <summary>
	/// 结束时间
	/// </summary>
	unsigned int EndTime ;
	/// <summary>
	/// 持续时间（接任务后必须在该时间内完成，否则销毁，-1表示不限时间）
	/// </summary>
	int LastTime ;
	/// <summary>
	/// 任务描述
	/// </summary>
	string Description ;
	/// <summary>
	/// 任务类型
	/// </summary>
	char Type ;
    /// <summary>
    /// 任务统计类型
    /// </summary>
    char statisticsType;
	/// <summary>
	/// 任务级别
	/// </summary>
	char Level ;
	/// <summary>
	/// 每日可完成次数
	/// </summary>
	unsigned short DayTimes;
};

class CQuestDocGuide
{
public:
    unsigned short  m_nAction;
    unsigned short  m_nStep;
    int             m_nParam;
    std::string     m_strParam2;
    bool            m_bForce;
    std::string     m_strExtendText;

// 	std::string		m_string1;
// 	std::string		m_string1;
// 	std::string		m_string1;
// 	std::string		m_string1;

public:
    bool ReadDoc(CFileStream* pFs);
};

class CItem;

class CQuestDocNew
{
public:
    CQuestDocNew();
    ~CQuestDocNew();

public:
    
    unsigned short QuestID() const {return m_BaseInfo.ID;}
    eQuestTypeNew QuestType() const {return (eQuestTypeNew)m_BaseInfo.Type;}
    bool ReadDoc(CFileStream* pFs);
    bool CanDestroyByLevel(unsigned short level);
    unsigned short GetMusicCount(unsigned short musicID);
    unsigned short GetUseItemCount(itemtype_t itemID);
    unsigned short GetUseFunctionCount(unsigned short nFuncID);
    IQuestComplete* GetComplete(EQuest_Complete_Type type);
    bool GetReward(int nSex,unsigned int& nExp,unsigned int& nMoney,unsigned int& nHonor,unsigned int& contribution,unsigned int& vipExp,unsigned int& Intimacy,std::list<CItem>& items , int& bindMCoin);
    bool GetVipReward(int nSex, unsigned int& nExp, unsigned int& nMoney, unsigned int& nHonor, unsigned int& contribution, unsigned int& vipExp, unsigned int& Intimacy, std::list<CItem>& items , int& bindMCoin);
    //是否是兑换任务
    bool IsExchangeQuest();
    void GetExchangeItem(std::vector<CQuestItem>& items);
    bool IsDivorceDestroyQuest();
    bool IsDanceGroupDestroyQuest();
    bool CheckTimeType(unsigned int nDay);
    bool IsReAcceptType();                    

public:
    CQuestNewBaseInfo	m_BaseInfo;
    map<EQuest_Condition_Type,IQuestCondition*>	m_Condition;
    map<EQuest_Complete_Type,IQuestComplete*>	m_Complete;
    map<EQuest_Destroy_Type,IQuestDestroy*>		m_Destroy;
    map<EQuest_Reward_Type,IQuestReward*>		m_Reward;
};

#endif // __QUESTDOC_H__



//end file


