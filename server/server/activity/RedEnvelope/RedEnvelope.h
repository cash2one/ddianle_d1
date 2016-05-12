#ifndef  __DATASTRUCT_REDENVELOPE_H____
#define __DATASTRUCT_REDENVELOPE_H____

#include <map>
#include <list>
#include "../../../socket/ParamPool.h"


enum ERedEnvelopeError
{
    ERedEnvelopeError_Scuess,
    ERedEnvelopeError_NotExist,
    ERedEnvelopeError_DanceGroupError,
    ERedEnvelopeError_HasOpen,
    ERedEnvelopeError_AllBeOpen,
    ERedEnvelopeError_CreateRedEnvelopeError,
};

class BriefOpenRoleInfo
{
public:
    BriefOpenRoleInfo();
    virtual ~BriefOpenRoleInfo();

public:
    unsigned int        m_nRoleID;
    unsigned int        m_nOpenIndex;
    unsigned int        m_nOpenTime;
};

class OpenRoleInfo : public BriefOpenRoleInfo
{
public:
    OpenRoleInfo();
    virtual ~OpenRoleInfo();

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);
    bool operator==(const OpenRoleInfo & info);

public:
    std::string         m_strRoleName;
    unsigned int        m_nCurrencyValue;
};

class RedEnvelopeSenderInfo
{
public:
    RedEnvelopeSenderInfo();
    virtual ~RedEnvelopeSenderInfo();

public:
    unsigned int        m_nRoleID;
    std::string         m_strRoleName;
    unsigned int        m_nDanceGroupID;
    bool                m_bIsAnonymity;
};

class BriefRedEnvelope
{
public:
    BriefRedEnvelope();
    virtual ~BriefRedEnvelope();

public:
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);

public:
    unsigned int  m_nRedEnvelopeID;
    std::string   m_strSendRoleName;
    bool          m_bGetFinish;
    bool          m_bHasOpen;
    unsigned int  m_nOpenTime;
};

class RedEnvelope : public BriefRedEnvelope
{
public:
    RedEnvelope();
    RedEnvelope(const RedEnvelope & red);
    RedEnvelope(unsigned int nEnvelopeID);
    virtual ~RedEnvelope();

public:
    void InitializeLeftIndex();
    void doDecode(CParamPool &IOBuff);
    void doEncode(CParamPool &IOBuff);
    bool IsOpenByRoleID(unsigned int nRoleID);
    void GetOpenValue(unsigned int nRoleID, unsigned int & nGetMoney, unsigned int & nOpenTime);
    void GetRoleOpenIndex(std::list<std::string> & listGetDetail);
    bool IsAllOpen();
    void OpenRedEnvelope(OpenRoleInfo & openinfo);

public:
    unsigned int        m_nRedEnvelopeID;
    unsigned int        m_nDanceGroupID;
    unsigned int        m_nSendRoleID;
    std::string         m_strSendRoleName;
    unsigned int        m_nCurrencyType;
    int                 m_nCurrencyValue;
    std::map<int,int>   m_mapDetail;
    std::map<unsigned int, OpenRoleInfo>  m_mapOpenRole;
    std::vector<int>    m_vecLeftRedEnvelopeIndex;
    unsigned int        m_nCreateTime;
    bool                m_bIsAnonymity;
    unsigned int        m_nExpirationTime;
};

class RedEnvelopeManager
{
public:
    RedEnvelopeManager();
    virtual ~RedEnvelopeManager();

    static RedEnvelopeManager & Instance();

public:
    // 加载红包
    void LoadRedEnvelope();

    // Initialize
    void Initialize(unsigned int nMaxRedEnvelopeID);

    // 时间判断
    void Update(time_t tNow);

    // 创建红包
    unsigned int CreateRedEnvelope(unsigned int nRoleID, const std::string & strRoleName, int nMCoin, 
            unsigned int nDanceGroupID, bool bAnonymity);

    // get redenvelope info
    RedEnvelope * GetRedEnvelope(unsigned int nRedEnvelopeID);
    std::map<unsigned int,RedEnvelope*> * GetAllRedEnvelope();

    // get dance group redenvelope list
    void GetDanceGroupRedEnvelopeList(unsigned int nDanceGroupID, std::list<unsigned int> & listRedEnvelope);

    // open redenvelope
    int OpenRedEnvelope(unsigned int nRedEnvelopeID, unsigned int nDanceGroupID, OpenRoleInfo & openinfo);

    // insert into new red envelope
    void SyncToDBbyInsert(const RedEnvelope & redenvelop);

    // update
    void SyncToDBbyUpdate(const RedEnvelope & redenvelop);

    // get can open count
    unsigned int GetCanOpenRedEnvelopeCount(unsigned int nRoleID, unsigned int nDanceGroupID);

private:
    // 生成红包ID
    unsigned int CreateRedEnvelopeID();

    // 生成红包每份份额
	void CreateRedEnvelopeDetails(int nRedEnvelopeValue, int nRedEnvelopeCount, int nMinRate, int nFluctuateRate, std::map<int, int> & details);

public:
    std::map<unsigned int,RedEnvelope*>              m_mapRedEnvelope;
    std::map<unsigned int, std::list<unsigned int> > m_mapDanceGroupRedEnvelope;
    unsigned int                                     m_nMaxRedEnvelopeID;
};

#endif
