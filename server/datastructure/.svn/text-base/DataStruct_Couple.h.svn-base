#ifndef		__DATASTRUCT_COUPLE_H__
#define		__DATASTRUCT_COUPLE_H__

#include <string>
#include <vector>
#include <algorithm>
#include "DataStruct_Base.h"

class CParamPool;

enum eDivorceState
{
	eDivorceState_None,
	eDivorceState_Running,
	eDivorceState_End,
};

enum eDivorceAction
{
	eDivorceAction_None,
	eDivorceAction_Apply,			// 申请     
	eDivorceAction_CancelaApply,	// 取消
	eDivorceAction_Agree,			// 同意
	eDivorceAction_Compulsory,		// 强制
	eDivorceAction_HaveNew,			// 新的情侣
	eDivorceAction_Chat,			// 聊天
	eDivorceAction_Gift,			// 送礼
    eDivorceAction_HaveNewHomosexual,			// 新的情侣(同性)
};

enum EHomesexual
{
    EHomesexual_Nomal,
    EHomesexual_Seme,       // 小攻
    EHomesexual_Small,      // 小受
};

class CCoupleInfoDB
{
public:
	CCoupleInfoDB();
	~CCoupleInfoDB();
public:
	unsigned int m_nPairID;
	unsigned int m_nMaleID;
	unsigned int m_nFemaleID;
	int m_nIntimacy;
	unsigned int m_nCoupleDate;
	unsigned int m_nMarriageDate;
    unsigned int m_nSmallRoleID; 
};

class CCoupleInfo
{
public:
	CCoupleInfo();
	CCoupleInfo( const CCoupleInfo & coupleinfo );
	~CCoupleInfo();

public:
	void doEncode(CParamPool & IOBuff);
	void doDecode(CParamPool & IOBuff);
	void Serialize(CParamPool & IOBuff);

public:
	int GetIntimacy(){ return m_nIntimacy; }
	void SetIntimacy( int nIntimacy ){ m_nIntimacy = nIntimacy; }

public:
	unsigned int      m_nPairID;
	unsigned int      m_nCoupleID;
	std::string       m_strCoupleName;
	int               m_nIntimacy;
	int				  m_nTempIntimacy;
	unsigned int      m_nCoupleDate;
	unsigned int      m_nMarriageDate;
	std::string       m_strLoveDeclaration;
	unsigned int      m_nBlessingValue;           // 祝福值
	unsigned int      m_nMarriageGiftDate;        // 结婚纪念日礼物领取时间
	unsigned int      m_nDivorceAgreementID;
    unsigned int      m_nSmallRoleID;
};


// 祝福值排行榜
class CBlessingValueRankInfo
{
public:
	CBlessingValueRankInfo();
	CBlessingValueRankInfo( const CBlessingValueRankInfo & rankinfo );
	virtual ~CBlessingValueRankInfo();

	bool operator==(const CBlessingValueRankInfo & rankinfo )
	{
		if ( rankinfo.m_nPairID == m_nPairID )
		{
			return true;
		}
		
		return false;
	}

public:
	void doEncode( CParamPool &IOBuff );
	void doDecode( CParamPool &IOBuff );


public:
	unsigned int	 m_nPairID;
	unsigned int     m_nMaleRoleID;
	std::string      m_strMaleRoleName;
	unsigned int     m_nFemaleRoleID;
	std::string      m_strFemaleRoleName;
	unsigned int	 m_nBlessingValue;
};


// 
class COfflineCoupleInfo
{
public:
    COfflineCoupleInfo();
    virtual ~COfflineCoupleInfo(){};

public:
    void doEncode(CParamPool &IOBuff);
    void doDecode(CParamPool &IOBuff);

public:
    unsigned int      m_nPairID;
    unsigned int      m_nMaleRoleID;
    unsigned int      m_nFemaleRoleID;
    int               m_nIntimacy;
    unsigned int      m_nCoupleDate;
    unsigned int      m_nMarriageDate;
    unsigned int      m_nBlessingValue;
    unsigned int      m_nSmallRoleID;
    std::string       m_strMaleRoleName;
    std::string       m_strFemaleRoleName;
    std::string       m_strLoveDeclaration;
};


////////////////////////////////////////////
struct WeddingRecordInfo
{
	unsigned int      m_nPairID;
	unsigned int      m_nMaleID;
	std::string       m_strMaleName;
	unsigned int      m_nFemaleID;
	std::string       m_strFemaleName;
	unsigned int      m_nIsRunning;
	std::vector<unsigned int> m_BlessingRoleIDList;
	unsigned int      m_nLine;
	unsigned int      m_nBlessingValue;
    unsigned int      m_nSmallRoleID;

	WeddingRecordInfo()
	{
		m_nPairID               = 0;
		m_nMaleID               = 0;
		m_strMaleName           = "";
		m_nFemaleID             = 0;
		m_strFemaleName         = "";
		m_nIsRunning            = 0;
		m_nLine                 = 0;
		m_nBlessingValue        = 0;
        m_nSmallRoleID          = 0;
	}

	WeddingRecordInfo( const WeddingRecordInfo & Info )
	{
		m_nPairID        	= Info.m_nPairID;
		m_nMaleID        	= Info.m_nMaleID;
		m_strMaleName     	= Info.m_strMaleName;
		m_nFemaleID      	 = Info.m_nFemaleID;
		m_strFemaleName  	 = Info.m_strFemaleName;
		m_nBlessingValue 	 = Info.m_nBlessingValue;
		m_nIsRunning     	 = Info.m_nIsRunning;
		m_nLine          	 = Info.m_nLine;
        m_BlessingRoleIDList = Info.m_BlessingRoleIDList;
        m_nSmallRoleID       = Info.m_nSmallRoleID;
	}

	bool IsInBlessingRoleList( unsigned int nRoleID )
	{
		std::vector<unsigned int>::iterator it = std::find( m_BlessingRoleIDList.begin(), m_BlessingRoleIDList.end(), nRoleID );
		if ( it !=  m_BlessingRoleIDList.end() )
		{
			return true;
		}

		return false;
	}		
};


class CDivorceAgreementInfo
{
public:
	CDivorceAgreementInfo();
	CDivorceAgreementInfo( const CDivorceAgreementInfo & info );
	virtual ~CDivorceAgreementInfo(){};

public:
	void doEncode( CParamPool &IOBuff );
	void doDecode( CParamPool &IOBuff );

public:
	// 清理
	void clear();

	// 获取/设置协议书ID
	unsigned int GetAgreementID(){ return m_nAgreementID;}
	void SetAgreementID( unsigned int nAgreementID){ m_nAgreementID = nAgreementID; }

	// 获取/设置男方是否按手印
	bool IsMaleAgree(){ return m_bMaleIsAgree; }
	void SetMaleAgree( bool bIsAgree ){ m_bMaleIsAgree = bIsAgree; }

	// 获取/设置女方是否按手印
	bool IsFemaleAgree(){ return m_bFemaleIsAgree; }
	void SetFemaleAgree( bool bIsAgree ){ m_bFemaleIsAgree = bIsAgree; }

	// 获取/设置状态
	eDivorceState GetState(){ return m_eState; }

public:
	unsigned int	m_nAgreementID;	     // 协议书ID        
	unsigned int	m_nMaleRoleID;		 // 男方ID
	std::string		m_strMaleRoleName;   // 男方姓名
	bool			m_bMaleIsAgree;      // 男方是否按手印
	unsigned int	m_nFemaleRoleID;     // 女方ID
	std::string		m_strFemaleRoleName; // 女方姓名
	bool			m_bFemaleIsAgree;    // 女方是否按手印
	unsigned int	m_nProposerID;       // 申请人ID
	eDivorceState	m_eState;            // 状态
    unsigned int    m_nSmallRoleID;      // 小受角色ID
};

class CDivorceAction
{
public:
	CDivorceAction();
	CDivorceAction( const CDivorceAction & action );
	virtual ~CDivorceAction(){};

public:
	void doEncode( CParamPool &IOBuff );
	void doDecode( CParamPool &IOBuff );

public:
	unsigned int	m_nAgreementID;
	eDivorceAction  m_eType;
	unsigned int	m_nProposerID;			// 发起人
	CItem			m_SendItem;				// 赠送礼物
	std::string		m_strChatContent;		// 聊天内容
	unsigned int	m_nTime;

};
typedef std::list<CDivorceAction>::iterator CDivorceActionIter;

#endif


