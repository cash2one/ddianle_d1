#include "RedEnvelope.h"
#include "GameMsg_GS_RedEnvelope.h"
#include "RedEnvelopeConfigManager.h"
#include "../../../socket/Log.h"
#include "../../../socket/Utility.h"
#include <algorithm>
#include "../../libServerFrame/QueryManager.h"
#include <sstream>

extern CQueryManager* g_pQueryMgr;

BriefOpenRoleInfo::BriefOpenRoleInfo()
    : m_nRoleID(0)
    , m_nOpenTime(0)
{

}

BriefOpenRoleInfo::~BriefOpenRoleInfo()
{

}

OpenRoleInfo::OpenRoleInfo()
    : m_strRoleName("")
    , m_nCurrencyValue(0)
{

}

OpenRoleInfo::~OpenRoleInfo()
{

}

void OpenRoleInfo::doDecode(CParamPool &IOBuff)
{
    m_nRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strRoleName);
    m_nCurrencyValue = IOBuff.GetUInt();
    m_nOpenTime = IOBuff.GetUInt();
}

void OpenRoleInfo::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRoleID);
    IOBuff.AddStringW(m_strRoleName);
    IOBuff.AddUInt(m_nCurrencyValue);
    IOBuff.AddUInt(m_nOpenTime);
}

bool OpenRoleInfo::operator==(const OpenRoleInfo & info)
{
    if (info.m_nRoleID == m_nRoleID)
    {
        return true;
    }

    return false;
}

RedEnvelopeSenderInfo::RedEnvelopeSenderInfo()
    : m_nRoleID(0)
    , m_strRoleName("")
    , m_nDanceGroupID(0)
    , m_bIsAnonymity(false)
{

}

RedEnvelopeSenderInfo::~RedEnvelopeSenderInfo()
{

}

BriefRedEnvelope::BriefRedEnvelope()
    : m_nRedEnvelopeID(0)
    , m_strSendRoleName("")
    , m_bGetFinish(false)
    , m_bHasOpen(false)
    , m_nOpenTime(0)
{

}

BriefRedEnvelope::~BriefRedEnvelope()
{

}

void BriefRedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nRedEnvelopeID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strSendRoleName);
    m_bGetFinish = IOBuff.GetBool();
    m_bHasOpen = IOBuff.GetBool();
    m_nOpenTime = IOBuff.GetUInt();
}

void BriefRedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeID);
    IOBuff.AddStringW(m_strSendRoleName);
    IOBuff.AddBool(m_bGetFinish);
    IOBuff.AddBool(m_bHasOpen);
    IOBuff.AddUInt(m_nOpenTime);
}

RedEnvelope::RedEnvelope()
    : m_nRedEnvelopeID(0)
    , m_nDanceGroupID(0)
    , m_nSendRoleID(0)
    , m_strSendRoleName("")
    , m_nCurrencyType(0)
    , m_nCurrencyValue(0)
    , m_nCreateTime(0)
    , m_bIsAnonymity(false)
    , m_nExpirationTime(0)
{
}

RedEnvelope::RedEnvelope(unsigned int nEnvelopeID)
    : m_nRedEnvelopeID(nEnvelopeID)
    , m_nDanceGroupID(0)
    , m_nSendRoleID(0)
    , m_strSendRoleName("")
    , m_nCurrencyType(0)
    , m_nCurrencyValue(0)
    , m_nCreateTime(0)
    , m_bIsAnonymity(false)
    , m_nExpirationTime(0)
{

}

RedEnvelope::RedEnvelope(const RedEnvelope & red)
{
    m_nRedEnvelopeID = red.m_nRedEnvelopeID;
    m_nDanceGroupID = red.m_nDanceGroupID;
    m_nSendRoleID = red.m_nSendRoleID;
    m_strSendRoleName = red.m_strSendRoleName;
    m_nCurrencyType = red.m_nCurrencyType;
    m_nCurrencyValue = red.m_nCurrencyValue;
    m_nCreateTime = red.m_nCreateTime;
    m_bIsAnonymity = red.m_bIsAnonymity;
    m_mapDetail = red.m_mapDetail;
    m_mapOpenRole = red.m_mapOpenRole;
    m_nExpirationTime = red.m_nExpirationTime;
}

void RedEnvelope::GetOpenValue(unsigned int nRoleID, unsigned int & nGetMoney, unsigned int & nOpenTime)
{
    std::map<unsigned int, OpenRoleInfo>::iterator it = m_mapOpenRole.find(nRoleID);
    if (it != m_mapOpenRole.end())
    {
        nGetMoney = it->second.m_nCurrencyValue;
        nOpenTime = it->second.m_nOpenTime;
    }
}

bool RedEnvelope::IsOpenByRoleID(unsigned int nRoleID)
{
    return m_mapOpenRole.end() != m_mapOpenRole.find(nRoleID);
}

void RedEnvelope::GetRoleOpenIndex(std::list<std::string> & listGetDetail)
{
    std::map<unsigned int, OpenRoleInfo>::iterator it = m_mapOpenRole.begin();
    for (; it != m_mapOpenRole.end(); ++it)
    {
        std::string strBrief;
        std::stringstream ss;
        ss<<it->second.m_nRoleID;
        ss<<":";
        ss<<it->second.m_nOpenIndex;
        ss<<":";
        ss<<it->second.m_nOpenTime;

        ss>>strBrief;
        listGetDetail.push_back(strBrief);
    }
}

void RedEnvelope::OpenRedEnvelope(OpenRoleInfo & openinfo)
{
    if (IsAllOpen())
    {
        return;
    }

    int nIndex = Random(1, (int)m_vecLeftRedEnvelopeIndex.size());
    int nRollValue = m_vecLeftRedEnvelopeIndex[nIndex-1];
    std::map<int,int>::iterator it = m_mapDetail.find(nRollValue);
    if (it != m_mapDetail.end())
    {
        std::vector<int>::iterator itr = std::find(m_vecLeftRedEnvelopeIndex.begin(), m_vecLeftRedEnvelopeIndex.end(), nRollValue);
        if (itr != m_vecLeftRedEnvelopeIndex.end())
        {
            m_vecLeftRedEnvelopeIndex.erase(itr);
        }

        openinfo.m_nOpenTime = (unsigned int)time(NULL);
        openinfo.m_nOpenIndex = nRollValue;
        openinfo.m_nCurrencyValue = it->second;
        m_mapOpenRole[openinfo.m_nRoleID] = openinfo;
    }
}

bool RedEnvelope::IsAllOpen()
{
    if (m_vecLeftRedEnvelopeIndex.empty())
    {
        return true;
    }

    return false;
}

void RedEnvelope::doEncode(CParamPool &IOBuff)
{
    IOBuff.AddUInt(m_nRedEnvelopeID);
    IOBuff.AddUInt(m_nDanceGroupID);
    IOBuff.AddUInt(m_nSendRoleID);
    IOBuff.AddStringW(m_strSendRoleName);
    IOBuff.AddUShort((unsigned short)m_nCurrencyType);
    IOBuff.AddInt(m_nCurrencyValue);
    IOBuff.AddUInt(m_nCreateTime);
    IOBuff.AddBool(m_bIsAnonymity);

    IOBuff.AddUShort((unsigned short)m_mapDetail.size());
    std::map<int,int>::iterator it = m_mapDetail.begin();
    for (; it != m_mapDetail.end(); ++it)
    {
        IOBuff.AddInt(it->first);
        IOBuff.AddInt(it->second);
    }

    IOBuff.AddUShort((unsigned short)m_mapOpenRole.size());
    std::map<unsigned int,OpenRoleInfo>::iterator itr = m_mapOpenRole.begin();
    for (; itr != m_mapOpenRole.end(); ++itr)
    {
        itr->second.doEncode(IOBuff);
    }
}

void RedEnvelope::InitializeLeftIndex()
{
    std::map<int,int>::iterator itt = m_mapDetail.begin();
    for (; itt != m_mapDetail.end(); ++itt)
    {
        m_vecLeftRedEnvelopeIndex.push_back(itt->first);
    }

    std::map<unsigned int, OpenRoleInfo>::iterator it = m_mapOpenRole.begin();
    for (; it != m_mapOpenRole.end(); ++it)
    {
        std::vector<int>::iterator itr = std::find(m_vecLeftRedEnvelopeIndex.begin(), m_vecLeftRedEnvelopeIndex.end(), (int)it->second.m_nOpenIndex);
        if (itr != m_vecLeftRedEnvelopeIndex.end())
        {
            m_vecLeftRedEnvelopeIndex.erase(itr);
        }
    }
}

void RedEnvelope::doDecode(CParamPool &IOBuff)
{
    m_nRedEnvelopeID = IOBuff.GetUInt();
    m_nDanceGroupID = IOBuff.GetUInt();
    m_nSendRoleID = IOBuff.GetUInt();
    IOBuff.GetStringW(m_strSendRoleName);
    m_nCurrencyType = IOBuff.GetUShort();
    m_nCurrencyValue = IOBuff.GetInt();
    m_nCreateTime = IOBuff.GetUInt();
    m_bIsAnonymity = IOBuff.GetBool();

    unsigned short nDetailsCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nDetailsCount; ++i)
    {
        int nIndex = IOBuff.GetInt();
        int nValue = IOBuff.GetInt();
        m_mapDetail.insert(make_pair(nIndex, nValue));
    }

    unsigned short nGetCount = IOBuff.GetUShort();
    for (unsigned short i = 0; i < nGetCount; ++i)
    {
        OpenRoleInfo info;
        info.doDecode(IOBuff);

        m_mapOpenRole[info.m_nRoleID] = info;
    }
}


RedEnvelope::~RedEnvelope()
{

}

RedEnvelopeManager::RedEnvelopeManager()
: m_nMaxRedEnvelopeID(0)
{

}

RedEnvelopeManager::~RedEnvelopeManager()
{

}

void RedEnvelopeManager::LoadRedEnvelope()
{

}

RedEnvelopeManager & RedEnvelopeManager::Instance()
{
    static RedEnvelopeManager s_RedEnvelopeMgr;

    return s_RedEnvelopeMgr;
}

void RedEnvelopeManager::Initialize(unsigned int nMaxRedEnvelopeID)
{
    m_nMaxRedEnvelopeID = nMaxRedEnvelopeID;
    std::map<unsigned int,RedEnvelope*>::iterator it = m_mapRedEnvelope.begin();
    for (; it != m_mapRedEnvelope.end(); ++it)
    {
        if (it->second != NULL)
        {
            it->second->InitializeLeftIndex();
            m_mapDanceGroupRedEnvelope[it->second->m_nDanceGroupID].push_back(it->first);
        }
    }
}

void RedEnvelopeManager::Update(time_t tNow)
{
    std::map<unsigned int,RedEnvelope*>::iterator it = m_mapRedEnvelope.begin();
    for (; it != m_mapRedEnvelope.end();)
    {
        if (it->second->m_mapOpenRole.size() == it->second->m_mapDetail.size())
        {
            unsigned int nDisappearTime = RedEnvelopeConfigManager::Instance().GetAllDisappearTime();
            if (tNow >= time_t(it->second->m_nCreateTime + nDisappearTime))
            {
                it->second->m_nExpirationTime = (unsigned int)tNow;
                SyncToDBbyUpdate(*it->second);

                delete it->second; it->second = NULL;
                m_mapRedEnvelope.erase(it++);
            }
            else
            {
                ++it;
            }
        }
        else
        {
            unsigned int nContinueTime = RedEnvelopeConfigManager::Instance().GetContinueTime();
            if (tNow >= (time_t)(it->second->m_nCreateTime + nContinueTime))
            {
                it->second->m_nExpirationTime = (unsigned int)tNow;
                SyncToDBbyUpdate(*it->second);

                delete it->second;it->second = NULL;
                m_mapRedEnvelope.erase(it++);
            }
            else
            {
                ++it;
            }
        }
    }
}

unsigned int RedEnvelopeManager::CreateRedEnvelope(unsigned int nRoleID, const std::string & strRoleName, int nMCoin, 
            unsigned int nDanceGroupID, bool bAnonymity)
{
    // 红包配置是否存在
    RedEnvelopeRechareConfig * pRechargeConfig = RedEnvelopeConfigManager::Instance().GetRedEnvelopeRechargeConfig(nMCoin);
    if (pRechargeConfig == NULL)
    {
        return 0;
    }

    // 创建红包
    unsigned int nRedEnvelopeID = CreateRedEnvelopeID();
    RedEnvelope * pRedEnvelope = new RedEnvelope(nRedEnvelopeID);
    if (pRedEnvelope == NULL)
    {
        return 0;
    }

    unsigned int nRedEnvelopeNum = RedEnvelopeConfigManager::Instance().GetRedEnvelopeCount();
    std::string strAnonymityName = RedEnvelopeConfigManager::Instance().GetAnonymityName();
	CreateRedEnvelopeDetails(pRechargeConfig->m_nCurrencyValue, (int)nRedEnvelopeNum, pRechargeConfig->m_nMinRate, pRechargeConfig->m_nFluctuateRate, pRedEnvelope->m_mapDetail);
    pRedEnvelope->m_nSendRoleID = nRoleID;
    pRedEnvelope->m_nDanceGroupID = nDanceGroupID;
    pRedEnvelope->m_strSendRoleName = bAnonymity ? strAnonymityName : strRoleName;
    pRedEnvelope->m_nCurrencyType = pRechargeConfig->m_nCurrencyType;
	pRedEnvelope->m_nCurrencyValue = pRechargeConfig->m_nCurrencyValue;
    pRedEnvelope->m_nCreateTime = (unsigned int)time(NULL);
    pRedEnvelope->m_bIsAnonymity = bAnonymity;
    pRedEnvelope->InitializeLeftIndex();
    m_mapRedEnvelope.insert(make_pair(nRedEnvelopeID, pRedEnvelope));
    m_mapDanceGroupRedEnvelope[nDanceGroupID].push_back(nRedEnvelopeID);
    
    // insert into db
    SyncToDBbyInsert(*pRedEnvelope);

    return nRedEnvelopeID;
}

void RedEnvelopeManager::SyncToDBbyInsert(const RedEnvelope & redenvelop)
{
    RedEnvelope * pRedEnvelope = new RedEnvelope(redenvelop);
    CDelHelper helper(pRedEnvelope);
    g_pQueryMgr->AddQuery(QUERY_RedEnvelope_InsertNew, 0, pRedEnvelope, 0, NULL, &helper);
}

void RedEnvelopeManager::SyncToDBbyUpdate(const RedEnvelope & redenvelop)
{
    RedEnvelope * pRedEnvelope = new RedEnvelope(redenvelop);
    CDelHelper helper(pRedEnvelope);
    g_pQueryMgr->AddQuery(QUERY_RedEnvelope_Update, 0, pRedEnvelope, 0, NULL, &helper);
}

void RedEnvelopeManager::GetDanceGroupRedEnvelopeList(unsigned int nDanceGroupID, std::list<unsigned int> & listRedEnvelope)
{
    std::map<unsigned int,std::list<unsigned int> >::iterator it = m_mapDanceGroupRedEnvelope.find(nDanceGroupID);
    if (it != m_mapDanceGroupRedEnvelope.end())
    {
        listRedEnvelope = it->second;
    }
}

unsigned int RedEnvelopeManager::GetCanOpenRedEnvelopeCount(unsigned int nRoleID, unsigned int nDanceGroupID)
{
    unsigned int nCanOpenCount = 0;
    std::map<unsigned int,std::list<unsigned int> >::iterator it = m_mapDanceGroupRedEnvelope.find(nDanceGroupID);
    if (it != m_mapDanceGroupRedEnvelope.end())
    {
        std::list<unsigned int>::iterator itr = it->second.begin();
        for (; itr != it->second.end(); ++itr)
        {
            RedEnvelope * pRedEnvelope = GetRedEnvelope(*itr);
            if (pRedEnvelope != NULL && !pRedEnvelope->IsOpenByRoleID(nRoleID) && !pRedEnvelope->IsAllOpen())
            {
                nCanOpenCount++;
            }
        }
        
    }

    return nCanOpenCount;
}

int RedEnvelopeManager::OpenRedEnvelope(unsigned int nRedEnvelopeID, unsigned int nDanceGroupID, OpenRoleInfo & openinfo)
{
    RedEnvelope * pRedEnvelope = GetRedEnvelope(nRedEnvelopeID);
    if (pRedEnvelope == NULL)
    {
        return ERedEnvelopeError_NotExist;
    }

    if (pRedEnvelope->m_nDanceGroupID != nDanceGroupID)
    {
        return ERedEnvelopeError_DanceGroupError;
    }

    if (pRedEnvelope->IsOpenByRoleID(openinfo.m_nRoleID))
    {
        pRedEnvelope->GetOpenValue(openinfo.m_nRoleID, openinfo.m_nCurrencyValue, openinfo.m_nOpenTime);
        return ERedEnvelopeError_HasOpen;
    }

    if (pRedEnvelope->IsAllOpen())
    {
        return ERedEnvelopeError_AllBeOpen;
    }

    pRedEnvelope->OpenRedEnvelope(openinfo);

    if (openinfo.m_nOpenIndex > 0)
    {
        SyncToDBbyUpdate(*pRedEnvelope);
    }
    return ERedEnvelopeError_Scuess;
}

RedEnvelope * RedEnvelopeManager::GetRedEnvelope(unsigned int nRedEnvelopeID)
{
    std::map<unsigned int,RedEnvelope*>::iterator it = m_mapRedEnvelope.find(nRedEnvelopeID);
    if (it != m_mapRedEnvelope.end())
    {
        return it->second;
    }

    return NULL;
}

std::map<unsigned int,RedEnvelope*> * RedEnvelopeManager::GetAllRedEnvelope()
{
    return &m_mapRedEnvelope;
}

unsigned int RedEnvelopeManager::CreateRedEnvelopeID()
{
    return ++m_nMaxRedEnvelopeID;
}

int cmp(const void *a,const void *b)
{
    return(*(int *)a-*(int *)b);
}

void RedEnvelopeArithmetic1(std::map<int,int> & details, int nEnvelopeTotalValue, int nEnvelopeCount, int nMinValue, int nMaxValue)
{
    int nRollTotalValue = 0;
    for (int i = 0; i < nEnvelopeCount; ++i)
    {
        if (nEnvelopeCount == 1)
        {
            details.insert(make_pair(i+1, nEnvelopeTotalValue));
            break;
        }

        int nRollValue = Random(nMinValue, nMaxValue);
        details.insert(make_pair(i+1, nRollValue));
        nRollTotalValue += nRollValue;

        if (i == nEnvelopeCount - 2)
        {
            nMinValue = nEnvelopeTotalValue - nRollTotalValue;
            nMaxValue = nEnvelopeTotalValue - nRollTotalValue;
        }
        else
        {
            int nTemp = nEnvelopeTotalValue - nRollTotalValue - (nEnvelopeCount - i - 2)*nMinValue;
            nMaxValue = nTemp >= nMaxValue ? nMaxValue : nTemp;
        }
    }
}

void RedEnvelopeArithmetic2(std::map<int,int> & details, int nEnvelopeTotalValue, int nEnvelopeCount)
{
    std::vector<int> vecTemp;
    vecTemp.resize(nEnvelopeCount+1);
    vecTemp[0] = 0;
    vecTemp[nEnvelopeCount] = nEnvelopeTotalValue;

    for (int i = 1; i < nEnvelopeCount; i++)
    {
        vecTemp[i] = Random()%nEnvelopeTotalValue;
    }
    sort(vecTemp.begin(), vecTemp.end());

    for (int i = 1; i < nEnvelopeCount + 1; i++)
    {
        int nRollValue = vecTemp[i] - vecTemp[i-1];
        details.insert(make_pair(i, nRollValue));
    }
}

void RedEnvelopeManager::CreateRedEnvelopeDetails(int nRedEnvelopeValue, int nRedEnvelopeCount, int nMinRate, int nFluctuateRate, std::map<int,int> & details)
{
    int nMinValue = (int)(((double)nMinRate/(double)100)*nRedEnvelopeValue);
    int nLeftMaxValue = nRedEnvelopeValue - (nRedEnvelopeCount - 1)*nMinValue;
    int nMaxValue = nMinValue*nFluctuateRate >= nLeftMaxValue ? nLeftMaxValue : nMinValue*nFluctuateRate;
    
    if (nMinValue > nMaxValue)
    {
        return;
    }

    std::map<int,int> details_first;
    RedEnvelopeArithmetic1(details_first, nRedEnvelopeValue, nRedEnvelopeCount, nMinValue, nMaxValue);

    int nFirstTotal = 0;
    int nFirstRedEnvelopeCount = nRedEnvelopeCount/5;
    for (int i = 0; i < nFirstRedEnvelopeCount; ++i)
    {
        std::map<int,int>::iterator it = details_first.find(i+1);
        if (it != details_first.end())
        {
            nFirstTotal += it->second;
            details.insert(make_pair(it->first,it->second));
        }
    }

    std::map<int,int> details_second;
    int nLeftValue = nRedEnvelopeValue - nFirstTotal - nMinValue*(nRedEnvelopeCount - nFirstRedEnvelopeCount);
    if (nLeftValue > 0)
    {
        RedEnvelopeArithmetic2(details_second, nLeftValue, nRedEnvelopeCount - nFirstRedEnvelopeCount);
    }

    std::map<int,int>::iterator itr = details_second.begin();
    for (; itr != details_second.end(); ++itr)
    {
        details.insert(make_pair(itr->first + nFirstRedEnvelopeCount, itr->second + nMinValue));
    }

    if (details_second.empty())
    {
        for (int i = 0 ; i < ((int)nRedEnvelopeCount - nFirstRedEnvelopeCount); ++i)
        {
            details.insert(make_pair(i + nFirstRedEnvelopeCount + 1, nMinValue));
        }
    }

    // show details
    std::map<int,int>::iterator it = details.begin();
    for (; it != details.end(); ++it)
    {
        WriteLog(LOGLEVEL_ERROR, "RedEnvelope Index : %d--Value: %d", it->first, it->second);
    }
}





